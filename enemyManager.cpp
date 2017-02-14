#include "stdafx.h"
#include "enemyManager.h"
#include "tileMap.h"

HRESULT enemyManager::init(void)
{
	count = 0;

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
		{//
			_tileMap->releaseObject(_vEnemy[i]->getIndex().x, _vEnemy[i]->getIndex().y);
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
		if (_tileMap->getTiles()[i].obj == OBJECT_ENEMY2)
		{
			addenemy("블루슬라임", { i % TILEX , i / TILEX });
		}
		if (_tileMap->getTiles()[i].obj == OBJECT_ENEMY3)
		{
			addenemy("Skeleton", { i % TILEX , i / TILEX });
		}
		if (_tileMap->getTiles()[i].obj == OBJECT_BOSS)
		{
			addenemy("Dragon", { i % TILEX , i / TILEX });
		}


	}
}

void enemyManager::remobeEnemy(int index)
{
}

void enemyManager::addenemy(string KEY, POINT index)
{
		if (KEY == "슬라임")
		{
			Slime* temp = new Slime;
			temp->init({ index.x, index.y });
			temp->setHp(temp->getMaxHp());
			temp->setLinkTileMap(_tileMap);
			temp->setLinkPlayer(_player);
			_vEnemy.push_back(temp);
		}
		if (KEY == "블루슬라임")
		{
			Blueslime* temp = new Blueslime;
			temp->init({ index.x , index.y });
			temp->setHp(temp->getMaxHp());
			temp->setLinkTileMap(_tileMap);
			temp->setLinkPlayer(_player);
			_vEnemy.push_back(temp);
		}
		if (KEY == "Skeleton")
		{
			Skeleton* temp = new Skeleton;
			temp->init({ index.x , index.y });
			temp->setHp(temp->getMaxHp());
			temp->setLinkTileMap(_tileMap);
			temp->setLinkPlayer(_player);
			_vEnemy.push_back(temp);
		}

		if (KEY == "Dragon")
		{
			Dragon* temp = new Dragon;
			temp->init({ index.x , index.y });
			temp->setHp(temp->getMaxHp());
			temp->setLinkTileMap(_tileMap);
			temp->setLinkPlayer(_player);
			_vEnemy.push_back(temp);
		}
	}


void enemyManager::collision()
{
}