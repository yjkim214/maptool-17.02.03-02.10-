#pragma once
#include "gameNode.h"

class tileMap;

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
	int _directioncount;
	//�ִϸ��̼� ���� ī��Ʈ
	int _animcount;

	//�������� �ƴ���
	bool _isLeft;
	//�̵����� ������
	bool _isMove;

	//���ʹ� �̹���
	image* _image;

	//���ϸ��
	//TEST
	float _destX;
	float _destY;

	//Ÿ�ϸ� ��ȣ����
	tileMap* _tileMap;

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

	//
	void setSpeed(float speed) {}
	float getSpeed() { return 0.0f; }

	void setHp(int HP) {}
	int getHp() { return 0; }
	void setDmg(int Dmg) {}
	int getDmg() { return 0; }

	float getX() { return 0.0f; }
	float getY() { return 0.0f; }

	RECT getRC() { return _rc; }

	void setLinkTileMap(tileMap* tileMap) { _tileMap = tileMap; }

	cEnemy() : _direct(MOVEDIRECTION_LEFT), _movespeed(4.8f), _movecount(0), _animcount(0),
		_isLeft(false), _isMove(false)
	{}
	~cEnemy() {}
};