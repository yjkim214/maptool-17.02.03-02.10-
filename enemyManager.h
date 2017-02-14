#pragma once
#include "gameNode.h"
#include "Slime.h"
#include "Blueslime.h"
#include "Skeleton.h"
#include "Dragon.h"

class tileMap;
class player;

class enemyManager : public gameNode

{
private:
	vector<cEnemy*> _vEnemy;
	vector<cEnemy*>::iterator _viEnemy;

	tileMap* _tileMap;
	player* _player;

	int count;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void setEnemy(void);

	void remobeEnemy(int index);
	void addenemy(string KEY, POINT index);
	void collision();
	void setLinkTileMap(tileMap* tileMap) { _tileMap = tileMap; }
	void setLinkPlayer(player* player) { _player = player; }

	vector<cEnemy*> getEnemyList() { return _vEnemy; }

	enemyManager() {}
	~enemyManager() {}
};

