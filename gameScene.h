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

	image* _menuBtn;
	image* _replayBtn;

	RECT _menuRc;
	RECT _replayRc;

	int _alpha;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	gameScene() {}
	~gameScene() {}
};