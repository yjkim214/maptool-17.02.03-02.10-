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
		if (_player->getCurrentSlot() <= 3)
		{
			if (_alpha < 255) { _alpha += 5; }
			else
			{
				_alpha = 0;
				_tileMap->changeSlot(_player->getCurrentSlot());
				_player->release();
				_player->setIndex();
				_player->setIsClear(false);
				_heartbeat->release();
				_heartbeat->init();
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
		}

		else
		{
			if (_alpha < 255) { _alpha += 5; }
			else
			{
				SOUNDMANAGER->stop("zone1_1");
			}
		}
	}

	_player->update();
	_heartbeat->update();
	_enemymanager->update();

	if (_player->getIsDead())
	{
		if (_alpha < 255) { _alpha += 5; }
		else 
		{
			SOUNDMANAGER->stop("zone1_1");
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&IMAGEMANAGER->findImage("replay_button")->boundingBox(), _ptMouse))
		{
			release();
			init();
		}

		if (PtInRect(&IMAGEMANAGER->findImage("menu_button")->boundingBox(), _ptMouse))
		{
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

	if (_player->getIsDead())
	{
		IMAGEMANAGER->findImage("blackscreen")->alphaRender(getMemDC(), _alpha);

		if (_alpha >= 255)
		{
			IMAGEMANAGER->findImage("gameover")->render(getMemDC());
			IMAGEMANAGER->findImage("menu_button")->render(getMemDC(), 92, 269);
			IMAGEMANAGER->findImage("replay_button")->render(getMemDC(), 92, 343);
		}
	}

	if (_player->getIsClear())
	{
		IMAGEMANAGER->findImage("blackscreen")->alphaRender(getMemDC(), _alpha);
		
		if (_alpha >= 255 && _player->getCurrentSlot() > 3)
		{
			IMAGEMANAGER->findImage("gameclear")->render(getMemDC());
			IMAGEMANAGER->findImage("menu_button")->render(getMemDC(), 165, 233);
			IMAGEMANAGER->findImage("replay_button")->render(getMemDC(), 165, 302);
		}
	}
}