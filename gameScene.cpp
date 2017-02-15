#include "stdafx.h"
#include "gameScene.h"
#include "tileMap.h"
#include "player.h"
#include "heartbeat.h"
#include "enemyManager.h"

HRESULT gameScene::init(void)
{
	//타일맵 클래스 생성
	DRAWRECTMANAGER->init();

	_tileMap = new tileMap;
	_tileMap->init();

	_player = new player;
	_player->init();
	
	_heartbeat = new heartbeat;
	_heartbeat->init();

	_enemymanager = new enemyManager;
	_enemymanager->init();

	_tileMap->setEnemyLint(_enemymanager);

	_enemymanager->setLinkTileMap(_tileMap);
	_enemymanager->setLinkPlayer(_player);

	_enemymanager->setEnemy();

	_player->setLinkTileMap(_tileMap);
	_player->setLinkHeartbeat(_heartbeat);
	_player->setLinkEnemyManager(_enemymanager);

	SOUNDMANAGER->stop("main_menu");
	SOUNDMANAGER->play("zone1_1");

	_alpha = 0;

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
	
	if (_player->getIsClear())
	{
		_tileMap->changeSlot(_player->getCurrentSlot());
		_player->release();
		_player->setIndex();
		_player->setIsClear(false);
		_enemymanager->setEnemy();

		if (SOUNDMANAGER->isPlaySound("zone1_1"))
		{
			SOUNDMANAGER->stop("zone1_1");
			SOUNDMANAGER->play("zone1_1");
		}

		else
		{
			SOUNDMANAGER->play("zone1_1");
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		_tileMap->changeSlot(1);
		_player->release();
		_player->setIndex(); 
		_enemymanager->setEnemy();
	}
	
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		_tileMap->changeSlot(2);
		_player->release();
		_player->setIndex();
		_enemymanager->setEnemy();
	}
	
	if (KEYMANAGER->isOnceKeyDown(VK_F3))
	{
		_tileMap->changeSlot(3);
		_player->release();
		_player->setIndex();
		_enemymanager->setEnemy();
	}

	_player->update();
	_heartbeat->update();
	_enemymanager->update();

	if (_player->getIsDead() || _player->getIsClear())
	{
		if (_alpha < 255) { _alpha += 5; }
		else 
		{ 
			SOUNDMANAGER->stop("zone1_1");
			SCENEMANAGER->changeScene("메뉴화면"); 
		}
	}
}

void gameScene::render(void)
{
	_tileMap->render();
	_enemymanager->render(); 
	_player->render();
	_tileMap->objRender(); 
	_heartbeat->render();
	_player->heartRender();

	IMAGEMANAGER->findImage("hud_weapon")->render(getMemDC(), 0, 0);
	IMAGEMANAGER->findImage("hud_armor")->render(getMemDC(), 65, 0);
	_player->equipRender();

	if (_player->getIsDead() || _player->getIsClear())
	{
		IMAGEMANAGER->findImage("blackscreen")->alphaRender(getMemDC(), _alpha);
	}
}