#pragma once
#include "gameNode.h"
#include "tileNode.h"

class tankMap : public gameNode
{
private:
	tagTile _tiles[TILEX * TILEY];		//Ÿ��
	DWORD _attribute[TILEX * TILEY];	//�Ӽ�

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//Ÿ�ϸ� �ε�
	void load(void);

	//Ÿ�� ��������
	tagTile* getTiles(void) { return _tiles; }
	//�Ӽ� ��������
	DWORD* getAttribute(void) { return _attribute; }

	tankMap() {}
	~tankMap() {}
};

