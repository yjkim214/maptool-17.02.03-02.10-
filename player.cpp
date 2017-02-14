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
	_vWeaponLiat.push_back(initWeapon);

	_equipWeapon = initWeapon;

	return S_OK;
}

void player::release(void)
{
}

void player::update(void)
{
	//�Լ� ȣ��
	//������
	this->move();
	//�ִϸ��̼�
	this->animation();

	for (int i = 0; i < _vWeaponLiat.size(); i++)
	{
		_vWeaponLiat[i]->update();
	}

	for (int i = 0; i < _vArmorList.size(); i++)
	{
		_vArmorList[i]->update();
	}
}

void player::render(void)
{
	//�÷��̾� ��Ʈ�� �׷���
	if (KEYMANAGER->isToggleKey(VK_F4))
	{
		RectangleMake(getMemDC(), _rc);
	}

	//�÷��̾� �̹����� �׷���
	_playerImg->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX + 4 * static_cast<int>(_isLeft), static_cast<int>(_equipArmor->getKind()));

	for (int i = 0; i < _maxHp / 2; i++)
	{
		IMAGEMANAGER->findImage("heart")->render(getMemDC(), WINSIZEX - i * 48 - 48, 0);
	}

	for (int i = 0; i < _vWeaponLiat.size(); i++)
	{
		_vWeaponLiat[i]->render();
	}

	for (int i = 0; i < _vArmorList.size(); i++)
	{
		_vArmorList[i]->render();
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

				//������ �� �ִ� Ÿ���� ��
				if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == 0)
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosX = _x - PLAYER_SIZEX;

					//�ε��� ����
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x - 1, _index.y);

					for (int i = 0; i < _vWeaponLiat.size(); i++)
					{
						if (_index.x - 1 == _vWeaponLiat[i]->getIndex().x && _index.y == _vWeaponLiat[i]->getIndex().y)
						{
							_equipWeapon->setIsEquip(false);
							_equipWeapon->setIndex({ _index.x - 1, _index.y });
							_equipWeapon->setPos();
							_equipWeapon = _vWeaponLiat[i];
							_equipWeapon->setIsEquip(true);
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
						}
					}

					_index.x = _index.x - 1;
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] & ATTR_BOX != 0)
				{
					int randNum = RND->getInt(2);
					if (randNum == 0)
					{
						armor* newArmor;
						newArmor->init(static_cast<ARMOR_KIND>(RND->getInt(10)));
						newArmor->setIndex({ _index.x - 1, _index.y });
						newArmor->setPos();
						_vArmorList.push_back(newArmor);
					}

					else if (randNum == 1)
					{
						weapon* newWeapon;
						newWeapon->init(static_cast<WEAPON_KIND>(RND->getInt(3)));
						newWeapon->setIndex({ _index.x - 1, _index.y });
						newWeapon->setPos();
						_vWeaponLiat.push_back(newWeapon);
					}

					_tileMap->releaseObject(_index.x - 1, _index.y);
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] & ATTR_GOAL != 0)
				{

				}

				switch (_equipWeapon->getKind())
				{
				case WEAPON_DAGGER:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY1)
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
					}
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY1)
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
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1] == ATTR_ENEMY2)
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
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY3)
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
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_BOSS)
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
					}
					break;
				case WEAPON_LONGSWORD:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY1)
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
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 2] == ATTR_ENEMY1)
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 2 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY2)
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
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 2] == ATTR_ENEMY2)
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 2 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY3)
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
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 2] == ATTR_ENEMY3)
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 2 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_BOSS)
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
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 2] == ATTR_BOSS)
					{
						//��� ������ �迭����
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//���� �ε����� ���� ���͸� ã�Ƽ�
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 2 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//�� �ε����� ���Ϳ��� �������� �ش�
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}
					break;
				case WEAPON_BROADSWORD:
					//�������� Ÿ���� �Ӽ��� ���Ͷ��
					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x - 1] == ATTR_ENEMY1)
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
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY1)
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
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1] == ATTR_ENEMY1)
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
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x - 1] == ATTR_ENEMY2)
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
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY2)
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
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1] == ATTR_ENEMY2)
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
						}
					}

					//ENEMY3
					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x - 1] == ATTR_ENEMY3)
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
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY3)
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
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1] == ATTR_ENEMY3)
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
						}
					}

					//BOSS
					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x - 1] == ATTR_BOSS)
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
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_BOSS)
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
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1] == ATTR_BOSS)
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
						}
					}
					break;
				}

				//ó���� ��Ŀ�� ������
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				_direct = PLAYERDIRECTION_RIGHT;
				//���� ������ ������ �Ǻ�
				_isLeft = false;

				if (_index.x != TILEX - 1 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] == 0)
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosX = _x + PLAYER_SIZEX;

					//�ε��� ����
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x + 1, _index.y);

					for (int i = 0; i < _vWeaponLiat.size(); i++)
					{
						if (_index.x + 1 == _vWeaponLiat[i]->getIndex().x && _index.y == _vWeaponLiat[i]->getIndex().y)
						{
							_equipWeapon->setIsEquip(false);
							_equipWeapon->setIndex({ _index.x + 1, _index.y });
							_equipWeapon->setPos();
							_equipWeapon = _vWeaponLiat[i];
							_equipWeapon->setIsEquip(true);
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
						}
					}

					_index.x = _index.x + 1;
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] == ATTR_ENEMY1)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] == ATTR_ENEMY2)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] == ATTR_ENEMY3)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] == ATTR_BOSS)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x + 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] & ATTR_BOX != 0)
				{
					int randNum = RND->getInt(2);
					if (randNum == 0)
					{
						armor* newArmor;
						newArmor->init(static_cast<ARMOR_KIND>(RND->getInt(10)));
						newArmor->setIndex({ _index.x + 1, _index.y });
						newArmor->setPos();
						_vArmorList.push_back(newArmor);
					}

					else if (randNum == 1)
					{
						weapon* newWeapon;
						newWeapon->init(static_cast<WEAPON_KIND>(RND->getInt(3)));
						newWeapon->setIndex({ _index.x + 1, _index.y });
						newWeapon->setPos();
						_vWeaponLiat.push_back(newWeapon);
					}

					_tileMap->releaseObject(_index.x + 1, _index.y);
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] & ATTR_GOAL != 0)
				{

				}

				//ó���� ��Ŀ�� ������
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_UP))
			{
				_direct = PLAYERDIRECTION_UP;

				if (_index.y != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] == 0)
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosY = _y - PLAYER_SIZEY;
					
					//�ε��� ����
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x, _index.y - 1);

					for (int i = 0; i < _vWeaponLiat.size(); i++)
					{
						if (_index.x == _vWeaponLiat[i]->getIndex().x && _index.y - 1 == _vWeaponLiat[i]->getIndex().y)
						{
							_equipWeapon->setIsEquip(false);
							_equipWeapon->setIndex({ _index.x, _index.y - 1 });
							_equipWeapon->setPos();
							_equipWeapon = _vWeaponLiat[i];
							_equipWeapon->setIsEquip(true);
						}
					}

					for (int i = 0; i < _vArmorList.size(); i++)
					{
						if (_index.x == _vArmorList[i]->getIndex().x && _index.y - 1 == _vArmorList[i]->getIndex().y)
						{
							_equipArmor->setIsEquip(false);
							_equipArmor->setIndex({ _index.x, _index.y - 1 });
							_equipArmor->setPos();
							_equipArmor = _vArmorList[i];
							_equipArmor->setIsEquip(true);
						}
					}

					_index.y = _index.y - 1;
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] == ATTR_ENEMY1)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] == ATTR_ENEMY2)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] == ATTR_ENEMY3)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] == ATTR_BOSS)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] & ATTR_BOX != 0)
				{
					int randNum = RND->getInt(2);
					if (randNum == 0)
					{
						armor* newArmor;
						newArmor->init(static_cast<ARMOR_KIND>(RND->getInt(10)));
						newArmor->setIndex({ _index.x, _index.y - 1 });
						newArmor->setPos();
						_vArmorList.push_back(newArmor);
					}

					else if (randNum == 1)
					{
						weapon* newWeapon;
						newWeapon->init(static_cast<WEAPON_KIND>(RND->getInt(3)));
						newWeapon->setIndex({ _index.x, _index.y - 1 });
						newWeapon->setPos();
						_vWeaponLiat.push_back(newWeapon);
					}

					_tileMap->releaseObject(_index.x, _index.y - 1);
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] & ATTR_GOAL != 0)
				{

				}

				//ó���� ��Ŀ�� ������
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
			{
				_direct = PLAYERDIRECTION_DOWN;

				if (_index.y != TILEY - 1 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] == 0)
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosY = _y + PLAYER_SIZEY;

					//�ε��� ����
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x, _index.y + 1);

					for (int i = 0; i < _vWeaponLiat.size(); i++)
					{
						if (_index.x == _vWeaponLiat[i]->getIndex().x && _index.y + 1 == _vWeaponLiat[i]->getIndex().y)
						{
							_equipWeapon->setIsEquip(false);
							_equipWeapon->setIndex({ _index.x, _index.y + 1 });
							_equipWeapon->setPos();
							_equipWeapon = _vWeaponLiat[i];
							_equipWeapon->setIsEquip(true);
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
						}
					}

					_index.y = _index.y + 1;
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] == ATTR_ENEMY1)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().y == (_index.y + 1))
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] == ATTR_ENEMY2)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().x == (_index.y + 1))
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] == ATTR_ENEMY3)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().x == (_index.y + 1))
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] == ATTR_BOSS)
				{
					for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
					{
						if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x && _enemyMg->getEnemyList()[i]->getIndex().x == (_index.y + 1))
						{
							_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
						}
					}
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] & ATTR_BOX != 0)
				{
					int randNum = RND->getInt(2);
					if (randNum == 0)
					{
						armor* newArmor;
						newArmor->init(static_cast<ARMOR_KIND>(RND->getInt(10)));
						newArmor->setIndex({ _index.x, _index.y + 1 });
						newArmor->setPos();
						_vArmorList.push_back(newArmor);
					}

					else if (randNum == 1)
					{
						weapon* newWeapon;
						newWeapon->init(static_cast<WEAPON_KIND>(RND->getInt(3)));
						newWeapon->setIndex({ _index.x, _index.y + 1 });
						newWeapon->setPos();
						_vWeaponLiat.push_back(newWeapon);
					}

					_tileMap->releaseObject(_index.x, _index.y + 1);
				}

				else if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] & ATTR_GOAL != 0)
				{

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