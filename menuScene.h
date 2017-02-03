#pragma once
#include "gameNode.h"

class menuScene : public gameNode
{
private:
	//��ư�� �ʱ� x
	int _initPlayX;
	int _initMapX;
	int _initQuitX;
	//���콺�� ��ư �ȿ� ���� �� ��ư�� x
	int _changePlayX;
	int _changeMapX;
	int _changeQuitX;

	//��ư���� ���İ�
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