#pragma once
#include "gameNode.h"
#include "tileNode.h"

class tileMap : public gameNode
{
private:
	tagTile _tiles[TILEX * TILEY];		//Ÿ��
	DWORD _attribute[TILEX * TILEY];	//�Ӽ�

	float cameraPosX;
	float cameraPosY;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//Ÿ�ϸ� �ε�
	void load(void);

	void setStartPos(int indexX,int indexY);

	//Ÿ�� ��������
	tagTile* getTiles(void) { return _tiles; }
	//�Ӽ� ��������
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

	void setPlayerAttribute(int indexX, int indexY)
	{
		_tiles[indexY*TILEX + indexX].obj = OBJECT_PLAYER;
		_attribute[indexY*TILEX + indexX] |= ATTR_UNMOVAL;
	}

	void setEnemyAttribute(int indexX, int indexY,int enemyType)
	{
		_tiles[indexY*TILEX + indexX].obj = (OBJECT)(enemyType+3);
		_attribute[indexY*TILEX + indexX] |= ATTR_UNMOVAL;
	}

	tileMap() {}
	~tileMap() {}
};

