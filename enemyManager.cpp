#include "stdafx.h"
#include "enemyManager.h"
#include "tileMap.h"

HRESULT enemyManager::init(void)
{
	count = 0;
	setEnemy();
	return S_OK;
}

void enemyManager::release(void)
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->release();
		SAFE_DELETE(_vEnemy[i]);
	}
}

void enemyManager::update(void)
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->update();
	}
	//  에너미 리무브 
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		if (_vEnemy[i]->getHp() <= 0)
		{
			_vEnemy.erase(_vEnemy.begin() + i);
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_F3))
	{
		_vEnemy[0]->setHp(_vEnemy[0]->getHp() - 1);
		cout << "체력" << _vEnemy[0]->getHp() << endl;
	}
}

void enemyManager::render(void)
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->render();
	}
}

void enemyManager::setEnemy(void)
{
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (_tileMap->getTiles()[i].obj == OBJECT_ENEMY1)
		{
			addenemy("슬라임", { i % TILEX, i / TILEX });
		}
	}
}

void enemyManager::remobeEnemy(int index)
{
}

void enemyManager::addenemy(string KEY, POINT pos)
{
	if (KEY == "슬라임")
	{
		Slime* temp = new Slime;
		temp->init({ pos.x, pos.y });
		temp->setLinkTileMap(_tileMap);
		temp->setLinkPlayer(_player);
		temp->setHp(2);
		_vEnemy.push_back(temp);
	}
}

void enemyManager::collision()
{
}
