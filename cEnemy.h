#pragma once
#include "gameNode.h"
#include "habar.h"

class tileMap;
class player;

#define MON_SIZE 48

enum ENEMY_MOVEDIRECTION
{
	MOVEDIRECTION_LEFT,
	MOVEDIRECTION_RIGHT,
	MOVEDIRECTION_TOP,
	MOVEDIRECTION_BOTTOM,
};

class cEnemy :public gameNode
{
protected:
	vector<habar*> _hpbarlist;
	//�̵� ����
	ENEMY_MOVEDIRECTION _direct;

	//��ġ �� ��Ʈ
	float _x;
	float _y;
	RECT _rc;

	//Ÿ�� ���� �ε���
	POINT _index;
	float _initx;
	float _inity;

	//�̵� �ӵ�
	float _movespeed;

	//ü��
	int _maxHp;
	int _hp;

	//������
	int _dmg;

	//�̵� ���� ī��Ʈ
	int _movecount;
	//�ִϸ��̼� ���� ī��Ʈ
	int _animcount;
	int _currentframe;

	//�������� �ƴ���
	bool _isLeft;
	//�̵����� ������
	bool _isMove;

	//���ʹ� �̹���
	image* _image;
	image* _attack;

	//���ϸ��
	//TEST
	float _destX;
	float _destY;

	//Ÿ�ϸ� ��ȣ����
	tileMap* _tileMap;
	player* _player;

public:
	//
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	//
	virtual void move();
	virtual void draw();
	virtual void animation();
	virtual void addhpbar(float x, float y);

	//
	void setHp(int hp) { _hp = hp; }
	int getHp() { return _hp; }

	int getMaxHp() { return _maxHp; }

	int getDmg() { return _dmg; }

	float getX() { return _x; }
	float getY() { return _y; }

	POINT getIndex() { return _index; }

	void setLinkTileMap(tileMap* tileMap) { _tileMap = tileMap; }
	void setLinkPlayer(player* player) { _player = player; }

	cEnemy() : _direct(MOVEDIRECTION_LEFT), _movespeed(4.8f), _movecount(0), _animcount(0),
		_isLeft(false), _isMove(false)
	{}
	~cEnemy() {}
};