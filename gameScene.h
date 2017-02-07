#pragma once
#include "gameNode.h"

class tileMap;
class player;

class gameScene : public gameNode
{
private:
	tileMap* _tileMap;
	player* _player;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	gameScene() {}
	~gameScene() {}
};