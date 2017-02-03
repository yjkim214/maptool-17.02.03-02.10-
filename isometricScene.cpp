#include "stdafx.h"
#include "isometricScene.h"

HRESULT isometricScene::init(void)
{
	//타일맵 0으로 초기화
	memset(_tilemap, 0, sizeof(_tilemap));

	_isoX = _isoY = 0;
	_corner = 0;

	return S_OK;
}

void isometricScene::release(void)
{
}

void isometricScene::update(void)
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{

		float cellX = (float)(_ptMouse.x - INIT_X);
		if (cellX < 0)
		{
			cellX = (cellX - CELL_WIDTH) / CELL_WIDTH;
		}
		else
		{
			cellX = cellX / (float)CELL_WIDTH;
		}

		int cellY = abs(_ptMouse.y - INIT_Y) / CELL_HEIGHT;
		cellY = (_ptMouse.y< INIT_Y) ? cellY * -1 : cellY;

		int isoX = (int)cellX + (int)cellY;
		int isoY = (int)cellY - (int)cellX;
		if (isoX >= 0 && isoX < TILE_COUNT_X
			&& isoY >= 0 && isoY < TILE_COUNT_Y)
		{

			//1,2,3,4분면체크해서 (1:Left_Top, 2:Right_Top, 3:Left_Bottom, 4:Right_Bottom)
			//1 이면 [x-1][y]
			//2 이면 [x][y-1]
			//3 이면 [x][y+1]
			//4 이면 [x+1][y]
			int corner = getCornerIndex(isoX, isoY);
			if (isInRhombus(corner, isoX, isoY))
			{
				corner = 0;
			}

			_corner = corner;
			_isoX = isoX;
			_isoY = isoY;
			switch (corner)
			{
			case 1:
				isoX = isoX - 1;
				break;
			case 2:
				isoY = isoY - 1;
				break;
			case 3:
				isoY = isoY + 1;
				break;
			case 4:
				isoX = isoX + 1;
				break;
			}

			_tilemap[isoX][isoY] = 1;
		}
	}
}

void isometricScene::render(void)
{
	drawTileMap();
	sprintf(str, "참고사항 (isoX:%d,isoY:%d, corner:%d)", _isoX, _isoY, _corner);
	TextOut(getMemDC(), 700, 20, str, strlen(str));
}

void isometricScene::drawTileMap(void)
{
	for (int i = 0; i < TILE_COUNT_X; i++)
	{
		for (int j = 0; j < TILE_COUNT_Y; j++)
		{
			int left = INIT_X + (i * RADIUS_WIDTH) - (j * RADIUS_WIDTH);
			int top = INIT_Y + (i * RADIUS_HEIGHT) + (j * RADIUS_HEIGHT);

			int centerX = left + RADIUS_WIDTH;
			int centerY = top + RADIUS_HEIGHT;

			if (_tilemap[i][j] == 1)
			{
				HPEN hMyPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
				HPEN hOldPen = (HPEN)SelectObject(getMemDC(), hMyPen);

				drawRhombus(left, top);
				DeleteObject(SelectObject(getMemDC(), hOldPen));
			}
			else
			{
				drawRhombus(left, top);
			}

			sprintf(str, "[%d,%d]", i, j);
			TextOut(getMemDC(), centerX, centerY, str, strlen(str));
		}
	}
}

void isometricScene::drawRhombus(int left, int top)
{
	int centerX = left + RADIUS_WIDTH;
	int centerY = top + RADIUS_HEIGHT;
	POINT p[5];
	p[0].x = centerX;
	p[0].y = centerY - RADIUS_HEIGHT;
	p[1].x = centerX + RADIUS_WIDTH;
	p[1].y = centerY;
	p[2].x = centerX;
	p[2].y = centerY + RADIUS_HEIGHT;
	p[3].x = centerX - RADIUS_WIDTH;
	p[3].y = centerY;
	p[4].x = centerX;
	p[4].y = centerY - RADIUS_HEIGHT;
	MoveToEx(getMemDC(), p[0].x, p[0].y, NULL);
	for (int i = 1; i < 5; i++)
	{
		LineTo(getMemDC(), p[i].x, p[i].y);
	}
}

int isometricScene::getCornerIndex(int isoX, int isoY)
{
	int left = INIT_X + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
	int top = INIT_Y + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

	int right = left + CELL_WIDTH;
	int bottom = top + CELL_HEIGHT;

	//마우스좌표의 사각안에서의 상대적 비율을 기준으로 분면을 구분한다.
	//(Left_Top : 1, Right_Top : 2, Left_Bottom : 3, Right_Bottom :4)
	float dx = (_ptMouse.x - left) / (float)CELL_WIDTH;
	float dy = (_ptMouse.y - top) / (float)CELL_HEIGHT;

	if (dx < 0.5f && dy < 0.5f)		return 1;
	if (dx >= 0.5f && dy < 0.5f)	return 2;
	if (dx < 0.5f && dy >= 0.5f)	return 3;
	if (dx >= 0.5f && dy >= 0.5f)	return 4;

	return 0;
}

bool isometricScene::isInRhombus(int corner, int isoX, int isoY)
{
	if (corner == 1)//Left_Top
	{
		int left = INIT_X + (isoX * RADIUS_WIDTH) - (isoY* RADIUS_WIDTH);
		int top = INIT_Y + (isoX*RADIUS_HEIGHT) + (isoY*RADIUS_HEIGHT);

		float dx = (float)(_ptMouse.x - left) / (CELL_WIDTH / 2.0f);
		float dy = (float)(_ptMouse.y - top) / (CELL_HEIGHT / 2.0f);
		if ((1.0f - dy) <= dx) return true;
		else return false;

	}
	else if (corner == 2)//Right_Top
	{
		int left = INIT_X + (isoX * RADIUS_WIDTH) - (isoY* RADIUS_WIDTH);
		left += (CELL_WIDTH / 2.0f);
		int top = INIT_Y + (isoX*RADIUS_HEIGHT) + (isoY*RADIUS_HEIGHT);


		float dx = (float)(_ptMouse.x - left) / (CELL_WIDTH / 2.0f);
		float dy = (float)(_ptMouse.y - top) / (CELL_HEIGHT / 2.0f);
		if (dy >= dx) return true;
		else return false;

	}
	else if (corner == 3)//Left_Bottom
	{
		int left = INIT_X + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		int top = INIT_Y + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);
		top += (CELL_HEIGHT / 2.0f);

		float dx = (float)(_ptMouse.x - left) / (CELL_WIDTH / 2.0f);
		float dy = (float)(_ptMouse.y - top) / (CELL_HEIGHT / 2.0f);
		if (dy <= dx) return true;
		else return false;

	}
	else if (corner == 4)//Right_Bottom
	{
		int left = INIT_X + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		left += (CELL_WIDTH / 2.0f);
		int top = INIT_Y + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);
		top += (CELL_HEIGHT / 2.0f);

		float dx = (float)(_ptMouse.x - left) / (CELL_WIDTH / 2.0f);
		float dy = (float)(_ptMouse.y - top) / (CELL_HEIGHT / 2.0f);
		if ((1.0f - dy) >= dx) return true;
		else return false;

	}
	return false;
}
