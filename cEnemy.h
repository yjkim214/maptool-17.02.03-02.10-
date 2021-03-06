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
	//이동 방향
	ENEMY_MOVEDIRECTION _direct;

	//위치 및 렉트
	float _x;
	float _y;
	RECT _rc;

	//타일 위의 인덱스
	POINT _index;
	float _initx;
	float _inity;

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
	int _currentframe;

	//왼쪽인지 아닌지
	bool _isLeft;
	//이동할지 안할지
	bool _isMove;

	//에너미 이미지
	image* _image;
	image* _attack;

	//잘하면되
	//TEST
	float _destX;
	float _destY;

	//타일맵 상호참조
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