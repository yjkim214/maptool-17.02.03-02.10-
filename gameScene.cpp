#include "stdafx.h"
#include "gameScene.h"
#include "tileMap.h"
#include "player.h"
#include "heartbeat.h"
#include "enemyManager.h"

HRESULT gameScene::init(void)
{
	//타일맵 클래스 생성
	_tileMap = new tileMap;
	_tileMap->init();

	_player = new player;
	_player->init();
	
	_heartbeat = new heartbeat;
	_heartbeat->init();

	_enemymanager = new enemyManager;
	_enemymanager->setLinkTileMap(_tileMap);
	_enemymanager->setLinkPlayer(_player);
	_enemymanager->init();

	_player->setLinkTileMap(_tileMap);
	_player->setLinkHeartbeat(_heartbeat);

	for (int i = 0; i < _player->getHp() / 2; i++)
	{
		_heartImg.push_back(IMAGEMANAGER->findImage("heart"));
	}

	SOUNDMANAGER->stop("main_menu");
	SOUNDMANAGER->play("zone1_1");

	return S_OK;
}

void gameScene::release(void)
{
	_tileMap->release();
	SAFE_DELETE(_tileMap);

	_player->release();
	SAFE_DELETE(_player);

	_heartbeat->release();
	SAFE_DELETE(_heartbeat);

	_enemymanager->release();
	SAFE_DELETE(_enemymanager);
}

void gameScene::update(void)
{
	_tileMap->update();
	_player->update();
	_heartbeat->update();
	_enemymanager->update();
}

void gameScene::render(void)
{
	_tileMap->render();
	_enemymanager->render(); 
	_player->render();
	_tileMap->objRender(); 
	_heartbeat->render();

	for (int i = 0; i < _heartImg.size(); i++)
	{
		_heartImg[i]->render(getMemDC(), WINSIZEX - i * 48 - 48, 0);
	}
}