#pragma once
#include "gameNode.h"

#define CELL_WIDTH 100
#define CELL_HEIGHT 50
#define RADIUS_WIDTH CELL_WIDTH / 2
#define RADIUS_HEIGHT CELL_HEIGHT / 2
#define INIT_X 400
#define INIT_Y 20
#define TILE_COUNT_X 30
#define TILE_COUNT_Y 30

class isometricScene : public gameNode
{
private:
	int _tilemap[TILE_COUNT_X][TILE_COUNT_X];
	int _isoX, _isoY;
	int _corner;
	char str[128];

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void drawTileMap(void);
	void drawRhombus(int left, int top);
	int getCornerIndex(int isoX, int isoY);
	bool isInRhombus(int corner, int isoX, int isoY);

	isometricScene() {}
	~isometricScene() {}
};

