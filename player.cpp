#include "stdafx.h"
#include "player.h"
#include "tileMap.h"
#include "heartbeat.h"
#include "enemyManager.h"
#include "armor.h"
#include "weapon.h"

HRESULT player::init(void)
{
	//공격력 초기화
	_att = INIT_ATT;

	//hp초기화
	_maxHp = INIT_MAXHP;
	_hp = INIT_MAXHP;

	//방향 초기화
	_direct = PLAYERDIRECTION_LEFT;

	//렉트 초기화
	_rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, PLAYER_SIZEX, PLAYER_SIZEY);

	//인덱스 초기화
	setIndex();

	//플레이어 속도 초기화
	_speed = 5.0f;

	//왼쪽이냐?
	_isLeft = false;
	//움직이지 않는 상태
	_isMove = false;

	//애니메이션 관련 변수 초기화
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
	//함수 호출
	//움직임
	this->move();
	//애니메이션
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
	//플레이어 렉트를 그려줌
	if (KEYMANAGER->isToggleKey(VK_F4))
	{
		RectangleMake(getMemDC(), _rc);
	}

	//플레이어 이미지를 그려줌
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
	//실제 x, y를 카메라 좌표로 설정
	_x = DRAWRECTMANAGER->getX();
	_y = DRAWRECTMANAGER->getY();

	//다음 목적지의 좌표를 설정하기 위한 값을 현재의 x, y값으로 설정
	_destPosX = _x;
	_destPosY = _y;

	//x, y의 값으로 인덱스를 도출
	_index.x = (_x + WINSIZEX / 2 - TILESIZEGAME / 2) / PLAYER_SIZEX;
	_index.y = (_y + WINSIZEY / 2 - TILESIZEGAME / 2) / PLAYER_SIZEY;
}

void player::move()
{
	//움직이지 않고 있는 상태라면
	if (!_isMove)
	{
		if (_heartbeat->getIsBeat())
		{
			//왼쪽 키를 눌렀을 때
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				//플레이어의 방향은 왼쪽
				_direct = PLAYERDIRECTION_LEFT;
				//왼쪽인지 아닌지 판별
				_isLeft = true;

				//움직일 수 있는 타일일 때
				if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == 0)
				{
					//플레이어를 움직이는 상태로
					_isMove = true;

					//TEST
					_destPosX = _x - PLAYER_SIZEX;

					//인덱스 변경
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
					//밟을려는 타일의 속성이 몬스터라면
					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY1)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}
					//밟을려는 타일의 속성이 몬스터라면
					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY1)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1] == ATTR_ENEMY2)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY3)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_BOSS)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}
					break;
				case WEAPON_LONGSWORD:
					//밟을려는 타일의 속성이 몬스터라면
					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY1)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 2] == ATTR_ENEMY1)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 2 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY2)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 2] == ATTR_ENEMY2)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 2 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY3)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 2] == ATTR_ENEMY3)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 2 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_BOSS)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 2] == ATTR_BOSS)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 2 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}
					break;
				case WEAPON_BROADSWORD:
					//밟을려는 타일의 속성이 몬스터라면
					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x - 1] == ATTR_ENEMY1)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY1)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1] == ATTR_ENEMY1)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 1)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x - 1] == ATTR_ENEMY2)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY2)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1] == ATTR_ENEMY2)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 1)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					//ENEMY3
					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x - 1] == ATTR_ENEMY3)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_ENEMY3)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1] == ATTR_ENEMY3)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 1)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					//BOSS
					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x - 1] == ATTR_BOSS)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y - 1)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_BOSS)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}

					if (_index.x != 0 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x - 1] == ATTR_BOSS)
					{
						//모든 몬스터의 배열에서
						for (int i = 0; i < _enemyMg->getEnemyList().size(); i++)
						{
							//같은 인덱스를 가진 몬스터를 찾아서
							if (_enemyMg->getEnemyList()[i]->getIndex().x == _index.x - 1 && _enemyMg->getEnemyList()[i]->getIndex().y == _index.y + 1)
							{
								//그 인덱스의 몬스터에게 데미지를 준다
								_enemyMg->getEnemyList()[i]->setHp(_enemyMg->getEnemyList()[i]->getHp() - _att);
							}
						}
					}
					break;
				}

				//처음의 마커를 지워줌
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				_direct = PLAYERDIRECTION_RIGHT;
				//고개를 돌릴지 말지를 판별
				_isLeft = false;

				if (_index.x != TILEX - 1 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] == 0)
				{
					//플레이어를 움직이는 상태로
					_isMove = true;

					//TEST
					_destPosX = _x + PLAYER_SIZEX;

					//인덱스 변경
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

				//처음의 마커를 지워줌
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_UP))
			{
				_direct = PLAYERDIRECTION_UP;

				if (_index.y != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] == 0)
				{
					//플레이어를 움직이는 상태로
					_isMove = true;

					//TEST
					_destPosY = _y - PLAYER_SIZEY;
					
					//인덱스 변경
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

				//처음의 마커를 지워줌
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
			{
				_direct = PLAYERDIRECTION_DOWN;

				if (_index.y != TILEY - 1 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] == 0)
				{
					//플레이어를 움직이는 상태로
					_isMove = true;

					//TEST
					_destPosY = _y + PLAYER_SIZEY;

					//인덱스 변경
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

				//처음의 마커를 지워줌
				_heartbeat->removeMarker();
			}
		}

		//박자를 못맞추면
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

	//플레이어가 움직이는 상태일 때
	else if (_isMove)
	{
		//방향에 따라 플레이어를 움직인다
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

		//움직이는 상태일 때
		//목적지의 인덱스와 플레이어의 거리가 작아졌을 때
		if (getDistance(_x, _y, _destPosX, _destPosY) < _speed)
		{
			//다음 인덱스의 위치로 플레이어를 고정
			_x = _destPosX;
			_y = _destPosY;
			//움직이지 않는 상태로 만든다
			_isMove = false;
		}

		//카메라 위치 재설정
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