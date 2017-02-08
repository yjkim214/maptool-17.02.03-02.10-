#pragma once
#include "gameNode.h"
#include "tileNode.h"

class tileMap : public gameNode
{
private:
	tagTile _tiles[TILEX * TILEY];		//타일
	DWORD _attribute[TILEX * TILEY];	//속성

	RECT _initRect[TILEX * TILEY];		//타일 고정 위치

	int slot;

	float cameraPosX;
	float cameraPosY;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	void objRender(void);

	//타일맵 로드

	void load(void);



	void setStartPos();

	//타일 가져오기
	tagTile* getTiles(void) { return _tiles; }
	//속성 가져오기
	DWORD* getAttribute(void) { return _attribute; }

	void releasePlayerAttribute(int indexX, int indexY)
	{
		_tiles[indexY*TILEX + indexX].obj = OBJECT_NONE;
		_attribute[indexY*TILEX + indexX] |= 0;
	}

	void releaseEnemyAttribute(int indexX, int indexY)
	{
		_tiles[indexY*TILEX + indexX].obj = OBJECT_NONE;
		_attribute[indexY*TILEX + indexX] |= 0;
	}

	void releaseObject(int indexX, int indexY)
	{
		_tiles[indexY*TILEX + indexX].obj = OBJECT_NONE;
		_attribute[indexY*TILEX + indexX] |= 0;
	}

	void setPlayerAttribute(int indexX, int indexY)
	{
		_tiles[indexY*TILEX + indexX].obj = OBJECT_PLAYER;


		_attribute[indexY*TILEX + indexX] |= ATTR_PLAYER;
	}

	void setEnemyAttribute(int indexX, int indexY, int enemyType)
	{
		_tiles[indexY*TILEX + indexX].obj = (OBJECT)(enemyType + 3);


		_attribute[indexY*TILEX + indexX] |= ATTR_ENEMY1 << (enemyType - 1);
	}

	tileMap() {}
	~tileMap() {}
};