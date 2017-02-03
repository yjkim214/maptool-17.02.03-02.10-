#pragma once
#include "gameNode.h"
#include "tankMap.h"
#include "tank.h"

class tankGameScene : public gameNode
{
private:
	tankMap* _tankMap;
	tank* _tank;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	tankGameScene() {}
	~tankGameScene() {}
};

