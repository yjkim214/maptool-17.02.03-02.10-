#pragma once
#include "gameNode.h"
class endScene : public gameNode
{
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	endScene() {}
	~endScene() {}
};