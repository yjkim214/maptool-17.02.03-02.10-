#pragma once
#include "gameNode.h"

//Ÿ�ϰ���
#define TILEX 5
#define TILEY 5
//Ÿ��ũ��
#define TILESIZEX 256
#define TILESIZEY (TILESIZEX / 2)
//Ÿ������ũ��
#define TILESIZEX_HALF (TILESIZEX / 2)
#define TILESIZEY_HALF (TILESIZEY / 2)
//���̼�Ÿ�� ������ǥ(0, 0)
#define STARTX (WINSIZEX / 2)
#define STARTY 100

//Ÿ�� ����ü
struct tagCell
{
	int x, y;
	RECT rc;
};

class isometric : public gameNode
{
private:
	tagCell _tile[TILEX][TILEY];		//Ÿ�ϱ���ü �� 25
	POINT _index;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	isometric() {}
	~isometric() {}
};

