#include "stdafx.h"
#include "gameScene.h"
#include "tileMap.h"
#include "player.h"
#include "enemyManager.h"

HRESULT gameScene::init(void)
{
	//타일맵 클래스 생성
	_tileMap = new tileMap;
	_tileMap->init();

	_player = new player;
	_player->init();
	_player->setLinkTileMap(_tileMap);
	
	_enemymanager = new enemyManager;
	_enemymanager->setLinkTileMap(_tileMap);
	_enemymanager->setLinkPlayer(_player);
	_enemymanager->init();

	return S_OK;
}

void gameScene::release(void)
{
	_tileMap->release();
	SAFE_DELETE(_tileMap);
	_enemymanager->release();
	SAFE_DELETE(_enemymanager);
	_player->release();
	SAFE_DELETE(_player);
}

void gameScene::update(void)
{
	_tileMap->update();
	_player->update();
	_enemymanager->update();
}

void gameScene::render(void)
{
	_tileMap->render();
	_player->render();
	_enemymanager->render();
	_tileMap->objRender();
}