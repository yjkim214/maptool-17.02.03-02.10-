#pragma once
#include "cEnemy.h"
class tileMap;
class player;

class Slime : public cEnemy
{
public:
	tileMap* _tileMap;
	player* _player;
	virtual HRESULT init(void);
	virtual HRESULT init(POINT index);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void move();
	virtual void draw();
	virtual void animation();

	void addhpbar(float x, float y);
	void setLinkTileMap(tileMap* tileMap) { _tileMap = tileMap; }
	void setLinkPlayer(player* player) { _player = player; }

	Slime() {}
	~Slime() {}
};