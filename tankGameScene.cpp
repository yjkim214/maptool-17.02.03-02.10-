#include "stdafx.h"
#include "tankGameScene.h"

HRESULT tankGameScene::init(void)
{
	//Ÿ���̹���
	IMAGEMANAGER->addFrameImage("tilemap", "tilemap.bmp", 640, 256, 20, 8);

	//��ũ�� Ŭ���� ����
	_tankMap = new tankMap;
	_tankMap->init();

	//��ũ Ŭ���� ����
	_tank = new tank;
	_tank->init();
	_tank->setTankMapLink(_tankMap);
	_tank->setTankPosition(0);

	return S_OK;
}

void tankGameScene::release(void)
{
	SAFE_DELETE(_tankMap);
	SAFE_DELETE(_tank);
}

void tankGameScene::update(void)
{
	_tankMap->update();
	_tank->update();
}

void tankGameScene::render(void)
{
	_tankMap->render();
	_tank->render();
}
