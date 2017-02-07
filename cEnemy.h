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
	//이동 방향
	ENEMY_MOVEDIRECTION _direct;

	//위치 및 렉트
	float _x;
	float _y;
	RECT _rc;

	//이동 속도
	float _movespeed;

	//체력
	int _maxHp;
	int _hp;

	//데미지
	int _dmg;

	//이동 관련 카운트
	int _movecount;
	//애니메이션 관련 카운트
	int _animcount;

	//왼쪽인지 아닌지
	bool _isLeft;
	//이동할지 안할지
	bool _isMove;

	//에너미 이미지
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

