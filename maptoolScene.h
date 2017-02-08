#pragma once
#include "gameNode.h"
#include "tileNode.h"

class maptoolScene : public gameNode
{
private:
	tagTile _tiles[TILEX * TILEY];	//왼쪽화면 타일(총 400개)
	tagSampleTile _sampleTiles[SAMPLETILEX * SAMPLETILEY];	//오른쪽화면 타일(총 160개)
	tagCurrentTile _currentTile;	//현재타일

									//버튼 5개(세이브, 로드, 지형, 오브젝트, 지우개)
	RECT _rcSave, _rcLoad, _rcTerrain, _rcObject, _rcEraser;

	RECT _slot1, _slot2, _slot3;

	CTRL _ctrlSelect;

	RECT mapscreen;

	int playerX;
	int playerY;
	int goalX;
	int goalY;


	int slot;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//맵툴세팅
	void maptoolSetup(void); //초기화에 넣어두면 된다
							 //맵세팅
	void setMap(void);
	//세이브
	void save(void);
	//로드
	void load(void);

	//지형, 오브젝트 셋터
	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objectSelect(int frameX, int frameY);

	maptoolScene() {}
	~maptoolScene() {}
};

