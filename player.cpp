#include "stdafx.h"
#include "player.h"
#include "tileMap.h"
#include "heartbeat.h"
#include "enemyManager.h"
#include "armor.h"
#include "weapon.h"

HRESULT player::init(void)
{
	//���ݷ� �ʱ�ȭ
	_att = INIT_ATT;

	//hp�ʱ�ȭ
	_maxHp = INIT_MAXHP;
	_hp = INIT_MAXHP;

	//���� �ʱ�ȭ
	_direct = PLAYERDIRECTION_LEFT;

	//��Ʈ �ʱ�ȭ
	_rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, PLAYER_SIZEX, PLAYER_SIZEY);

	//�ε��� �ʱ�ȭ
	setIndex();

	//�÷��̾� �ӵ� �ʱ�ȭ
	_speed = 5.0f;
	_isAttack = false;
	//�����̳�?
	_isLeft = false;
	//�������� �ʴ� ����
	_isMove = false;

	//�ִϸ��̼� ���� ���� �ʱ�ȭ
	_count = 0;
	_currentFrameX = 0;
	_playerImg = IMAGEMANAGER->findImage("player");

	_isDead = false;

	_tileMap = NULL;
	_heartbeat = NULL;
	_enemyMg = NULL;

	armor* initArmor = new armor;
	initArmor->init(ARMOR_TUNIC);
	initArmor->setIndex(_index);
	initArmor->setPos();
	initArmor->setIsEquip(true);
	_vArmorList.push_back(initArmor);
	_equipArmor = initArmor;

	weapon* initWeapon = new weapon;
	initWeapon->init(WEAPON_DAGGER);
	initWeapon->setIndex(_index);
	initWeapon->setPos();
	initWeapon->setIsEquip(true);
	_vWeaponList.push_back(initWeapon);
	_equipWeapon = initWeapon;

	//���� ���� �ʱ�ȭ
	_isClear = false;
	_currentSlot = 1;
	
	for (int i = 0; i < _maxHp / 2; i++)
	{
		addhpbar(WINSIZEX - i * 48 - 48, 0);
	}

	return S_OK;
}

void player::release(void)
{
	_vWeaponList.clear();
	_vWeaponList.push_back(_equipWeapon);

	_vArmorList.clear();
	_vArmorList.push_back(_equipArmor);
}

void player::update(void)
{
	//�Լ� ȣ��
	//������
	this->move();
	//�ִϸ��̼�
	this->animation();

	for (int i = 0; i < _vWeaponList.size(); i++)
	{
		_vWeaponList[i]->update();
	}

	for (int i = 0; i < _vArmorList.size(); i++)
	{
		_vArmorList[i]->update();
	}
	int tempHP = _hp;

	for (int i = 0; tempHP >= 0; i++)
	{
		if (tempHP >= 2)
		{
			_hpbarlist[_hpbarlist.size() - i - 1]->setcurrent(true);
			_hpbarlist[_hpbarlist.size() - i - 1]->sethalf(false);
		}

		else if (tempHP == 1)
		{
			_hpbarlist[_hpbarlist.size() - i - 1]->sethalf(true);
			_hpbarlist[_hpbarlist.size() - i - 1]->setcurrent(false);
		}
		
		else
		{
			if (i >= _hpbarlist.size())
			{
				break;
			}
			_hpbarlist[_hpbarlist.size() - i - 1]->sethalf(false);
			_hpbarlist[_hpbarlist.size() - i - 1]->setcurrent(false);
		}
		tempHP -= 2;
	}
}

void player::render(void)
{
	//�÷��̾� ��Ʈ�� �׷���
	if (KEYMANAGER->isToggleKey(VK_F4))
	{
		RectangleMake(getMemDC(), _rc);
	}

	for (int i = 0; i < _vWeaponList.size(); i++)
	{
		if (_vWeaponList[i]->getisEquip()) { continue; }
		_vWeaponList[i]->render();
	}

	for (int i = 0; i < _vArmorList.size(); i++)
	{
		if (_vArmorList[i]->getIsEquip()) { continue; }
		_vArmorList[i]->render();
	}

	//�÷��̾� �̹����� �׷���
	_playerImg->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX + 4 * static_cast<int>(_isLeft), static_cast<int>(_equipArmor->getKind()));
}

void player::equipRender(void)
{
	for (int i = 0; i < _vWeaponList.size(); i++)
	{
		if (!_vWeaponList[i]->getisEquip()) { continue; }
		_vWeaponList[i]->render();
	}

	for (int i = 0; i < _vArmorList.size(); i++)
	{
		if (!_vArmorList[i]->getIsEquip()) { continue; }
		_vArmorList[i]->render();
	}
}

void player::heartRender(void)
{
	for (int i = 0; i < _hpbarlist.size(); i++)
	{
		_hpbarlist[i]->render();
	}
}

bool player::getIsDead()
{
	if (_hp <= 0) { return true; }
	return false;
}

void player::setIndex()
{
	//���� x, y�� ī�޶� ��ǥ�� ����
	_x = DRAWRECTMANAGER->getX();
	_y = DRAWRECTMANAGER->getY();

	//���� �������� ��ǥ�� �����ϱ� ���� ���� ������ x, y������ ����
	_destPosX = _x;
	_destPosY = _y;

	//x, y�� ������ �ε����� ����
	_index.x = (_x + WINSIZEX / 2 - TILESIZEGAME / 2) / PLAYER_SIZEX;
	_index.y = (_y + WINSIZEY / 2 - TILESIZEGAME / 2) / PLAYER_SIZEY;
}

void player::move()
{
	//�������� �ʰ� �ִ� ���¶��
	if (!_isMove)
	{
		if (_heartbeat->getIsBeat())
		{
			//���� Ű�� ������ ��
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				//�÷��̾��� ������ ����
				_direct = PLAYERDIRECTION_LEFT;
				//�������� �ƴ��� �Ǻ�
				_isLeft = true;

				switch (_equipWeapon->getKind())
				{
				case WEAPON_DAGGER:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if (_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x - 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 - 72, WINSIZEY / 2 - 24, "swipe_dagger_left");
						
						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}
					break;
				case WEAPON_LONGSWORD:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if ((_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x - 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0))
						|| (_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x - 2]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
							
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 2 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 - 120, WINSIZEY / 2 - 24, "swipe_longsword_left");

						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}
					break;
				case WEAPON_BROADSWORD:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if ((_index.x != 0 && (((_tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x - 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0))
						|| (_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x - 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0))
						|| (_index.x != 0 && (((_tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}

							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}

							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 - 72, WINSIZEY / 2 - 72, "swipe_broadsword_left");

						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}
					break;
				}

				//������ �� �ִ� Ÿ���� ��
				if ((_index.x != 0) && (!_isAttack) && (_tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == 0))
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosX = _x - PLAYER_SIZEX;

					//�ε��� ����
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x - 1, _index.y);

					for (int i = 0; i < _vWeaponList.size(); i++)
					{
						if (_index.x - 1 == _vWeaponList[i]->getIndex().x && _index.y == _vWeaponList[i]->getIndex().y)
						{
							_equipWeapon->setIsEquip(false);
							_equipWeapon->setIndex({ _index.x - 1, _index.y });
							_equipWeapon->setPos();
							_equipWeapon = _vWeaponList[i];
							_equipWeapon->setIsEquip(true);
							break;
						}
					}

					for (int i = 0; i < _vArmorList.size(); i++)
					{
						if (_index.x - 1 == _vArmorList[i]->getIndex().x && _index.y == _vArmorList[i]->getIndex().y)
						{
							_equipArmor->setIsEquip(false);
							_equipArmor->setIndex({ _index.x - 1, _index.y });
							_equipArmor->setPos();
							_equipArmor = _vArmorList[i];
							_equipArmor->setIsEquip(true);
							break;
						}
					}

					_index.x = _index.x - 1;
				}

				else if (_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x - 1]) & (ATTR_BOX)) != 0))
				{
					int randNum = RND->getInt(2);
					if (randNum == 0)
					{
						armor* newArmor = new armor;
						newArmor->init(static_cast<ARMOR_KIND>(RND->getInt(10)));
						newArmor->setIndex({ _index.x - 1, _index.y });
						newArmor->setPos();
						_vArmorList.push_back(newArmor);
					}

					else if (randNum == 1)
					{
						weapon* newWeapon = new weapon;
						newWeapon->init(static_cast<WEAPON_KIND>(RND->getInt(3)));
						newWeapon->setIndex({ _index.x - 1, _index.y });
						newWeapon->setPos();
						_vWeaponList.push_back(newWeapon);
					}

					_tileMap->releaseObject(_index.x - 1, _index.y);
				}

				else if (_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x - 1]) & (ATTR_GOAL_OPEN)) != 0))
				{
					_isClear = true;
				}

				//ó���� ��Ŀ�� ������
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				_direct = PLAYERDIRECTION_RIGHT;
				//���� ������ ������ �Ǻ�
				_isLeft = false;

				switch (_equipWeapon->getKind())
				{
				case WEAPON_DAGGER:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if (_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x + 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 + 24, WINSIZEY / 2 - 24, "swipe_dagger_right");

						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}
					break;
				case WEAPON_LONGSWORD:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if ((_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x + 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)) ||
						(_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x + 2]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}

							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 2 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 + 24, WINSIZEY / 2 - 24, "swipe_longsword_right");

						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}
					break;
				case WEAPON_BROADSWORD:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if ((_index.x != 0 && (((_tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x + 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)) ||
						(_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x + 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)) ||
						(_index.x != 0 && (((_tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x + 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}

							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}

							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 + 24, WINSIZEY / 2 - 72, "swipe_broadsword_right");

						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}
					break;
				}

				if (_index.x != TILEX - 1 && !_isAttack && (_tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] == 0))
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosX = _x + PLAYER_SIZEX;

					//�ε��� ����
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x + 1, _index.y);

					for (int i = 0; i < _vWeaponList.size(); i++)
					{
						if (_index.x + 1 == _vWeaponList[i]->getIndex().x && _index.y == _vWeaponList[i]->getIndex().y)
						{
							_equipWeapon->setIsEquip(false);
							_equipWeapon->setIndex({ _index.x + 1, _index.y });
							_equipWeapon->setPos();
							_equipWeapon = _vWeaponList[i];
							_equipWeapon->setIsEquip(true);
							break;
						}
					}

					for (int i = 0; i < _vArmorList.size(); i++)
					{
						if (_index.x + 1 == _vArmorList[i]->getIndex().x && _index.y == _vArmorList[i]->getIndex().y)
						{
							_equipArmor->setIsEquip(false);
							_equipArmor->setIndex({ _index.x + 1, _index.y });
							_equipArmor->setPos();
							_equipArmor = _vArmorList[i];
							_equipArmor->setIsEquip(true);
							break;
						}
					}

					_index.x = _index.x + 1;
				}

				else if (_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x + 1]) & (ATTR_BOX)) != 0))
				{
					int randNum = RND->getInt(2);
					if (randNum == 0)
					{
						armor* newArmor = new armor;
						newArmor->init(static_cast<ARMOR_KIND>(RND->getInt(10)));
						newArmor->setIndex({ _index.x + 1, _index.y });
						newArmor->setPos();
						_vArmorList.push_back(newArmor);
					}

					else if (randNum == 1)
					{
						weapon* newWeapon = new weapon;
						newWeapon->init(static_cast<WEAPON_KIND>(RND->getInt(3)));
						newWeapon->setIndex({ _index.x + 1, _index.y });
						newWeapon->setPos();
						_vWeaponList.push_back(newWeapon);
					}

					_tileMap->releaseObject(_index.x + 1, _index.y);
				}

				else if (_index.x != 0 && (((_tileMap->getAttribute()[_index.y * TILEX + _index.x + 1]) & (ATTR_GOAL_OPEN)) != 0))
				{
					_isClear = true;
				}

				//ó���� ��Ŀ�� ������
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_UP))
			{
				_direct = PLAYERDIRECTION_UP;

				switch (_equipWeapon->getKind())
				{
				case WEAPON_DAGGER:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if (_index.x != 0 && (((_tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 - 24, WINSIZEY / 2 - 72, "swipe_dagger_up");

						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}
					break;
				case WEAPON_LONGSWORD:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if ((_index.x != 0 && (((_tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)) ||
						(_index.x != 0 && (((_tileMap->getAttribute()[(_index.y - 2) * TILEX + _index.x]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}

							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 - 24, WINSIZEY / 2 - 120, "swipe_longsword_up");

						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}
					break;
				case WEAPON_BROADSWORD:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if ((_index.x != 0 && (((_tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x + 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)) ||
						(_index.x != 0 && (((_tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x - 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)) ||
						(_index.x != 0 && (((_tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}

							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}

							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 - 72, WINSIZEY / 2 - 72, "swipe_broadsword_up");

						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}

					break;
				}

				if (_index.y != 0 && !_isAttack && (_tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] == 0))
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosY = _y - PLAYER_SIZEY;

					//�ε��� ����
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x, _index.y - 1);

					for (int i = 0; i < _vWeaponList.size(); i++)
					{
						if ((_index.x == _vWeaponList[i]->getIndex().x) && (_index.y - 1 == _vWeaponList[i]->getIndex().y))
						{
							_equipWeapon->setIsEquip(false);
							_equipWeapon->setIndex({ _index.x, _index.y - 1 });
							_equipWeapon->setPos();
							_equipWeapon = _vWeaponList[i];
							_equipWeapon->setIsEquip(true);
							break;
						}
					}

					for (int i = 0; i < _vArmorList.size(); i++)
					{
						if ((_index.x == _vArmorList[i]->getIndex().x) && (_index.y - 1 == _vArmorList[i]->getIndex().y))
						{
							_equipArmor->setIsEquip(false);
							_equipArmor->setIndex({ _index.x, _index.y - 1 });
							_equipArmor->setPos();
							_equipArmor = _vArmorList[i];
							_equipArmor->setIsEquip(true);
							break;
						}
					}

					_index.y = _index.y - 1;
				}

				else if (_index.x != 0 && (((_tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x]) & (ATTR_BOX)) != 0))
				{
					int randNum = RND->getInt(2);
					if (randNum == 0)
					{
						armor* newArmor = new armor;
						newArmor->init(static_cast<ARMOR_KIND>(RND->getInt(10)));
						newArmor->setIndex({ _index.x, _index.y - 1 });
						newArmor->setPos();
						_vArmorList.push_back(newArmor);
					}

					else if (randNum == 1)
					{
						weapon* newWeapon = new weapon;
						newWeapon->init(static_cast<WEAPON_KIND>(RND->getInt(3)));
						newWeapon->setIndex({ _index.x, _index.y - 1 });
						newWeapon->setPos();
						_vWeaponList.push_back(newWeapon);
					}

					_tileMap->releaseObject(_index.x, _index.y - 1);
				}

				else if (_index.x != 0 && (((_tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x]) & (ATTR_GOAL_OPEN)) != 0))
				{
					_isClear = true;
				}

				//ó���� ��Ŀ�� ������
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
			{
				_direct = PLAYERDIRECTION_DOWN;

				switch (_equipWeapon->getKind())
				{
				case WEAPON_DAGGER:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if (_index.x != 0 && (((_tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 - 24, WINSIZEY / 2 + 24, "swipe_dagger_down");

						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}
					break;
				case WEAPON_LONGSWORD:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if ((_index.x != 0 && (((_tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)) ||
						(_index.x != 0 && (((_tileMap->getAttribute()[(_index.y + 2) * TILEX + _index.x]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}

							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 2)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 - 24, WINSIZEY / 2 + 24, "swipe_longsword_down");

						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}
					break;
				case WEAPON_BROADSWORD:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if ((_index.x != 0 && (((_tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)) ||
						(_index.x != 0 && (((_tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)) ||
						(_index.x != 0 && (((_tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x + 1]) & (ATTR_ENEMY1 | ATTR_ENEMY2 | ATTR_ENEMY3 | ATTR_BOSS)) != 0)))
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
							
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}

							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 1)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}

						DRAWRECTMANAGER->shakeWindow();
						EFFECTMANAGER->addEffect(WINSIZEX / 2 - 72, WINSIZEY / 2 + 24, "swipe_broadsword_down");

						if (SOUNDMANAGER->isPlaySound("vo_ari_melee_1_01"))
						{
							SOUNDMANAGER->stop("vo_ari_melee_1_01");
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						else
						{
							SOUNDMANAGER->play("vo_ari_melee_1_01");
						}

						_isAttack = true;
					}

					else
					{
						_isAttack = false;
					}

					break;
				}

				if (_index.y != TILEY - 1 && !_isAttack && (_tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] == 0))
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosY = _y + PLAYER_SIZEY;

					//�ε��� ����
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x, _index.y + 1);

					for (int i = 0; i < _vWeaponList.size(); i++)
					{
						if (_index.x == _vWeaponList[i]->getIndex().x && _index.y + 1 == _vWeaponList[i]->getIndex().y)
						{
							_equipWeapon->setIsEquip(false);
							_equipWeapon->setIndex({ _index.x, _index.y + 1 });
							_equipWeapon->setPos();
							_equipWeapon = _vWeaponList[i];
							_equipWeapon->setIsEquip(true);
							break;
						}
					}

					for (int i = 0; i < _vArmorList.size(); i++)
					{
						if (_index.x == _vArmorList[i]->getIndex().x && _index.y + 1 == _vArmorList[i]->getIndex().y)
						{
							_equipArmor->setIsEquip(false);
							_equipArmor->setIndex({ _index.x, _index.y + 1 });
							_equipArmor->setPos();
							_equipArmor = _vArmorList[i];
							_equipArmor->setIsEquip(true);
							break;
						}
					}

					_index.y = _index.y + 1;
				}

				else if (_index.x != 0 && (((_tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x]) & (ATTR_BOX)) != 0))
				{
					int randNum = RND->getInt(2);
					if (randNum == 0)
					{
						armor* newArmor = new armor;
						newArmor->init(static_cast<ARMOR_KIND>(RND->getInt(10)));
						newArmor->setIndex({ _index.x, _index.y + 1 });
						newArmor->setPos();
						_vArmorList.push_back(newArmor);
					}

					else if (randNum == 1)
					{
						weapon* newWeapon = new weapon;
						newWeapon->init(static_cast<WEAPON_KIND>(RND->getInt(3)));
						newWeapon->setIndex({ _index.x, _index.y + 1 });
						newWeapon->setPos();
						_vWeaponList.push_back(newWeapon);
					}

					_tileMap->releaseObject(_index.x, _index.y + 1);
				}

				else if (_index.x != 0 && (((_tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x]) & (ATTR_GOAL_OPEN)) != 0))
				{
					/*
					if (_currentSlot < 3)
					{
						_currentSlot++;
					}

					_tileMap->changeSlot(_currentSlot);
					setIndex();
					*/
					_isClear = true;
				}

				//ó���� ��Ŀ�� ������
				_heartbeat->removeMarker();
			}
		}

		//���ڸ� �����߸�
		else
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_UP))
			{
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
			{
				_heartbeat->removeMarker();
			}
		}
	}

	//�÷��̾ �����̴� ������ ��
	else if (_isMove)
	{
		//���⿡ ���� �÷��̾ �����δ�
		switch (_direct)
		{
		case PLAYERDIRECTION_LEFT:
			_x -= _speed;
			break;
		case PLAYERDIRECTION_RIGHT:
			_x += _speed;
			break;
		case PLAYERDIRECTION_UP:
			_y -= _speed;
			break;
		case PLAYERDIRECTION_DOWN:
			_y += _speed;
			break;
		}

		//�����̴� ������ ��
		//�������� �ε����� �÷��̾��� �Ÿ��� �۾����� ��
		if (getDistance(_x, _y, _destPosX, _destPosY) < _speed)
		{
			//���� �ε����� ��ġ�� �÷��̾ ����
			_x = _destPosX;
			_y = _destPosY;
			//�������� �ʴ� ���·� �����
			_isMove = false;
		}

		//ī�޶� ��ġ �缳��
		DRAWRECTMANAGER->setPos(_x, _y);
	}
}

void player::animation()
{
	_count++;
	if (_count % 5 == 0)
	{
		_currentFrameX++;

		if (IMAGEMANAGER->findImage("player")->getMaxFrameX() / 2 < _currentFrameX)
		{
			_currentFrameX = 0;
		}

		_count = 0;
	}
}
void player::addhpbar(float x, float y)
{

	habar* newhpbar = new habar;
	newhpbar->init("heart", "heart_half", "heart_empty", x, y, 48, 44);
	_hpbarlist.push_back(newhpbar);

}
