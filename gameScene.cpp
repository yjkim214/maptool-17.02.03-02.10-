#include "stdafx.h"
#include "gameScene.h"
#include "tileMap.h"
#include "player.h"

HRESULT gameScene::init(void)
{
	//타일맵 클래스 생성
	_tileMap = new tileMap;
	_tileMap->init();

	_player = new player;
	_player->init();
	_player->setLinkTileMap(_tileMap);
	
	return S_OK;
}

void gameScene::release(void)
{
	_tileMap->release();
	SAFE_DELETE(_tileMap);

	_player->release();
	SAFE_DELETE(_player);
}

void gameScene::update(void)
{
	_tileMap->update();
	_player->update();
}

void gameScene::render(void)
{
	_tileMap->render();
	_player->render();
	_tileMap->objRender();
}