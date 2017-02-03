#pragma once
#include "gameNode.h"

class menuScene : public gameNode
{
private:
	//버튼의 초기 x
	int _initPlayX;
	int _initMapX;
	int _initQuitX;
	//마우스가 버튼 안에 있을 때 버튼의 x
	int _changePlayX;
	int _changeMapX;
	int _changeQuitX;

	//버튼들의 알파값
	int _alphaPlay;
	int _alphaMap;
	int _alphaQuit;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	menuScene() {}
	~menuScene() {}
};