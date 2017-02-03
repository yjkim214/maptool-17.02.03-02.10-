#pragma once
#include "gameNode.h"
#include "tileNode.h"

class tankMap : public gameNode
{
private:
	tagTile _tiles[TILEX * TILEY];		//타일
	DWORD _attribute[TILEX * TILEY];	//속성

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//타일맵 로드
	void load(void);

	//타일 가져오기
	tagTile* getTiles(void) { return _tiles; }
	//속성 가져오기
	DWORD* getAttribute(void) { return _attribute; }

	tankMap() {}
	~tankMap() {}
};

