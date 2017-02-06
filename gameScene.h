#pragma once
#include "gameNode.h"

class player;

class gameScene : public gameNode
{
private:
	player* _player;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	gameScene() {}
	~gameScene() {}
};