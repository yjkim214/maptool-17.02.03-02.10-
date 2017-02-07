#pragma once
#include "gameNode.h"

enum ENEMY_MOVEDIRECTION
{
	MOVEDIRECTION_LEFT,
	MOVEDIRECTION_TOP,
	MOVEDIRECTION_RIGHT,
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

	//�������� �ƴ���
	bool _isLeft;
	//�̵����� ������
	bool _isMove;

	//���ʹ� �̹���
	image* _image;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);


	virtual void move();
	virtual void draw();
	virtual void animation();

	virtual void setSpeed(float speed) {}
	virtual float getSpeed() { return 0.0f; }

	virtual void setHp(int HP) {}
	virtual int getHp() { return 0; }
	virtual void setDmg(int Dmg) {}
	virtual int getDmg() { return 0; }

	virtual float getX() { return 0.0f; }
	virtual float getY() { return 0.0f; }

	virtual RECT getRC() { return _rc; }

	cEnemy() : _direct(MOVEDIRECTION_LEFT), _movespeed(4.8f), _movecount(0), _animcount(0),
		_isLeft(false), _isMove(false)
	{}
	~cEnemy() {}
};

