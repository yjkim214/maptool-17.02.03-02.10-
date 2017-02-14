#pragma once
#include "gameNode.h"
#include "tileNode.h"

class tileMap : public gameNode
{
private:
	//타일 관련 변수
	tagTile _tiles[TILEX * TILEY];		//타일
	DWORD _attribute[TILEX * TILEY];	//속성
	RECT _initRect[TILEX * TILEY];		//타일 고정 위치

	//맵 변경 슬롯
	int slot;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	void objRender(void);

	//타일맵 로드
	void load(void);

	void setStartPos();

	//슬롯 변경 슬롯 변경시 플레이어의 위치도 변경해주어야하기 때문에 함수로 변환
	void changeSlot(int index);

	//타일 가져오기
	tagTile* getTiles(void) { return _tiles; }
	//속성 가져오기
	DWORD* getAttribute(void) { return _attribute; }

	void releaseObject(int indexX, int indexY)
	{
		_tiles[indexY*TILEX + indexX].obj = OBJECT_NONE;
		_attribute[indexY*TILEX + indexX] &= 0;
	}

	void releaseEnemyAttribute(int indexX, int indexY)
	{
		_tiles[indexY*TILEX + indexX].obj = OBJECT_NONE;
		_attribute[indexY*TILEX + indexX] &= 0;
	}

	void setPlayerAttribute(int indexX, int indexY)
	{
		_tiles[indexY*TILEX + indexX].obj = OBJECT_PLAYER;
		_attribute[indexY*TILEX + indexX] |= ATTR_PLAYER;
	}

	void setEnemyAttribute(int indexX, int indexY, int enemyType)
	{
		_tiles[indexY*TILEX + indexX].obj = (OBJECT)(enemyType + 2);
		_attribute[indexY*TILEX + indexX] |= ATTR_ENEMY1 << (enemyType - 1);
	}

	void setEnemyAttribute(int indexX, int indexY)
	{
		_tiles[indexY*TILEX + indexX].obj = OBJECT_BOSS;
		_attribute[indexY*TILEX + indexX] |= ATTR_BOSS;
	}

	tileMap() {}
	~tileMap() {}
};