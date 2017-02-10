#pragma once
#include "gameNode.h"

class tileMap;
class player;
class heartbeat;
class enemyManager;

class gameScene : public gameNode
{
private:
	tileMap* _tileMap;
	player* _player;
	heartbeat* _heartbeat;
	enemyManager* _enemymanager;

	vector<image*> _heartImg;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	gameScene() {}
	~gameScene() {}
};