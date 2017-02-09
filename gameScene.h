#pragma once
#include "gameNode.h"

class tileMap;
class player;
class enemyManager;
class gameScene : public gameNode
{
private:
	tileMap* _tileMap;
	player* _player;
	enemyManager* _enemymanager;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	gameScene() {}
	~gameScene() {}
};