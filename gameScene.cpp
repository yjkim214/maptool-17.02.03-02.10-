#include "stdafx.h"
#include "gameScene.h"
#include "player.h"

HRESULT gameScene::init(void)
{
	_player = new player;
	_player->init();
	return S_OK;
}

void gameScene::release(void)
{
	_player->release();
}

void gameScene::update(void)
{
	_player->update();
}

void gameScene::render(void)
{
	_player->render();
}