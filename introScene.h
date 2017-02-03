#pragma once
#include "gameNode.h"
class introScene : public gameNode
{
private:
	//인트로 배경화면 알파값
	int _alpha;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	introScene() : _alpha(255) {}
	~introScene() {}
};