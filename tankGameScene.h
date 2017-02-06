#pragma once
#include "gameNode.h"
#include "tileMap.h"
#include "tank.h"

class tankGameScene : public gameNode
{
private:
	tileMap* _tankMap;
	tank* _tank;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	tankGameScene() {}
	~tankGameScene() {}
};

