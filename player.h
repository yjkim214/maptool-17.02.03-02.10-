#pragma once
#include "gameNode.h"
#include "habar.h"

class tileMap;
class heartbeat;
class enemyManager;
class armor;
class weapon;
class potion;

#define INIT_ATT		0
#define INIT_MAXHP		6
#define PLAYER_SIZEX	48
#define PLAYER_SIZEY	48

enum PLAYERDIRECTION
{
	PLAYERDIRECTION_LEFT,
	PLAYERDIRECTION_RIGHT,
	PLAYERDIRECTION_UP,
	PLAYERDIRECTION_DOWN,
};

class player : public gameNode
{
private:
	vector<habar*> _hpbarlist;
	//플레이어 공격력
	float _att;
	//플레이어 HP
	float _maxHp;
	float _hp;

	//플레이어가 현재 있는 타일의 인덱스
	POINT _index;

	//플레이어의 방향
	PLAYERDIRECTION _direct;
	//플레이어의 위치
	float _x, _y;
	//플레이어의 렉트
	RECT _rc;

	//이동관련 변수
	float _speed;
	bool _isAttack;
	bool _isLeft;
	bool _isMove;

	//애니메이션 관련 변수
	int _count;
	int _currentFrameX;
	image* _playerImg;

	//TEST
	float _destPosX;
	float _destPosY;

	//죽었냐 안죽었냐
	bool _isDead;

	//현재 타일맵 슬롯
	bool _isClear;
	int _currentSlot;

private:
	tileMap* _tileMap;
	heartbeat* _heartbeat;
	enemyManager* _enemyMg;

private:
	armor* _equipArmor;
	vector<armor*> _vArmorList;

	weapon* _equipWeapon;
	vector<weapon*> _vWeaponList;

	potion* _equipPotion;
	vector<potion*> _vPotionList;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	void equipItemRender(void);
	void heartRender(void);

	//접근자
	float getAtt() { return _att; }
	float getHp()
	{
		if (SOUNDMANAGER->isPlaySound("vo_ari_hurt_01"))
		{
			SOUNDMANAGER->stop("vo_ari_hurt_01");
			SOUNDMANAGER->play("vo_ari_hurt_01");
		}

		else
		{
			SOUNDMANAGER->play("vo_ari_hurt_01");
		}
		return _hp;
	}
	POINT getIndex() { return _index; }
	PLAYERDIRECTION getDirect() { return _direct; }
	bool getIsDead();
	bool getIsClear() { return _isClear; }
	int getCurrentSlot() { return _currentSlot; }
	RECT getRect() { return _rc; }
	//설정자
	//초반 카메라 위치에 따라 인덱스를 바꾸어줌
	void setIndex();
	void setHp(int hp) 
	{ 
		_hp = hp;
		if (_hp > _maxHp)
		{
			_hp = _maxHp;
		}

		if (_hp <= 0)
		{
			if (SOUNDMANAGER->isPlaySound("vo_ari_death_01"))
			{
				SOUNDMANAGER->stop("vo_ari_death_01");
				SOUNDMANAGER->play("vo_ari_death_01");
			}

			else
			{
				SOUNDMANAGER->play("vo_ari_death_01");
			}
		}
	}
	void setIsClear(bool clear) { _isClear = clear; }

	//플레이어 움직임
	void move();

	//플레이어 애니메이션
	void animation();

	//
	void setHpbar();

	//플레이어 공격
	void attack(int index);

	//
	void useItem();

	//타일맵 상호참조
	void setLinkTileMap(tileMap* tileMap) { _tileMap = tileMap; }
	void setLinkHeartbeat(heartbeat* heartbeat) { _heartbeat = heartbeat; }
	void setLinkEnemyManager(enemyManager* enemyMg) { _enemyMg = enemyMg; }
	void addhpbar(float x, float y);

	player() {}
	~player() {}
};