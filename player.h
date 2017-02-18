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
	//�÷��̾� ���ݷ�
	float _att;
	//�÷��̾� HP
	float _maxHp;
	float _hp;

	//�÷��̾ ���� �ִ� Ÿ���� �ε���
	POINT _index;

	//�÷��̾��� ����
	PLAYERDIRECTION _direct;
	//�÷��̾��� ��ġ
	float _x, _y;
	//�÷��̾��� ��Ʈ
	RECT _rc;

	//�̵����� ����
	float _speed;
	bool _isAttack;
	bool _isLeft;
	bool _isMove;

	//�ִϸ��̼� ���� ����
	int _count;
	int _currentFrameX;
	image* _playerImg;

	//TEST
	float _destPosX;
	float _destPosY;

	//�׾��� ���׾���
	bool _isDead;

	//���� Ÿ�ϸ� ����
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

	//������
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
	//������
	//�ʹ� ī�޶� ��ġ�� ���� �ε����� �ٲپ���
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

	//�÷��̾� ������
	void move();

	//�÷��̾� �ִϸ��̼�
	void animation();

	//
	void setHpbar();

	//�÷��̾� ����
	void attack(int index);

	//
	void useItem();

	//Ÿ�ϸ� ��ȣ����
	void setLinkTileMap(tileMap* tileMap) { _tileMap = tileMap; }
	void setLinkHeartbeat(heartbeat* heartbeat) { _heartbeat = heartbeat; }
	void setLinkEnemyManager(enemyManager* enemyMg) { _enemyMg = enemyMg; }
	void addhpbar(float x, float y);

	player() {}
	~player() {}
};