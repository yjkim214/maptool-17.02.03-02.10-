#pragma once
#include "gameNode.h"

class tileMap;

#define INIT_ATT		1
#define INIT_MAXHP		3
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
	bool _isLeft;
	bool _isMove;

	//애니메이션 관련 변수
	int _count;
	int _currentFrameX;
	image* _playerImg;

	//TEST
	float _destPosX;
	float _destPosY;

	tileMap* _tileMap;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//접근자
	float getAtt() { return _att; }
	float getHp() { return _hp; }
	POINT getIndex() { return _index; }
	PLAYERDIRECTION getDirect() { return _direct; }
	bool isMove() { return _isMove; }

	//설정자
	void setHp(int hp) { _hp = hp; }
	//플레이어 움직임
	void move();

	//플레이어 애니메이션
	void animation();

	//타일맵 상호참조
	void setLinkTileMap(tileMap* tileMap) { _tileMap = tileMap; }

	player() {}
	~player() {}
};