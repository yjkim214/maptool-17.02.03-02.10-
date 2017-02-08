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
	bool _isLeft;
	bool _isMove;

	//�ִϸ��̼� ���� ����
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

	//������
	float getAtt() { return _att; }
	float getHp() { return _hp; }
	POINT getIndex() { return _index; }
	PLAYERDIRECTION getDirect() { return _direct; }
	bool isMove() { return _isMove; }

	//������
	void setHp(int hp) { _hp = hp; }
	//�÷��̾� ������
	void move();

	//�÷��̾� �ִϸ��̼�
	void animation();

	//Ÿ�ϸ� ��ȣ����
	void setLinkTileMap(tileMap* tileMap) { _tileMap = tileMap; }

	player() {}
	~player() {}
};