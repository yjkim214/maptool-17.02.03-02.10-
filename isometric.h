#pragma once
#include "gameNode.h"

//타일갯수
#define TILEX 5
#define TILEY 5
//타일크기
#define TILESIZEX 256
#define TILESIZEY (TILESIZEX / 2)
//타일절반크기
#define TILESIZEX_HALF (TILESIZEX / 2)
#define TILESIZEY_HALF (TILESIZEY / 2)
//아이소타일 시작좌표(0, 0)
#define STARTX (WINSIZEX / 2)
#define STARTY 100

//타일 구조체
struct tagCell
{
	int x, y;
	RECT rc;
};

class isometric : public gameNode
{
private:
	tagCell _tile[TILEX][TILEY];		//타일구조체 총 25
	POINT _index;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	isometric() {}
	~isometric() {}
};

