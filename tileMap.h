#pragma once
#include "gameNode.h"
#include "tileNode.h"

class tileMap : public gameNode
{
private:
	//Ÿ�� ���� ����
	tagTile _tiles[TILEX * TILEY];		//Ÿ��
	DWORD _attribute[TILEX * TILEY];	//�Ӽ�
	RECT _initRect[TILEX * TILEY];		//Ÿ�� ���� ��ġ

	//�� ���� ����
	int slot;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	void objRender(void);

	//Ÿ�ϸ� �ε�
	void load(void);

	void setStartPos();

	//���� ���� ���� ����� �÷��̾��� ��ġ�� �������־���ϱ� ������ �Լ��� ��ȯ
	void changeSlot(int index);

	//Ÿ�� ��������
	tagTile* getTiles(void) { return _tiles; }
	//�Ӽ� ��������
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