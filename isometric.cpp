#include "stdafx.h"
#include "isometric.h"

HRESULT isometric::init(void)
{
	//타일 구조체 초기화
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			_tile[i][j].x = STARTX + (j * (TILESIZEX_HALF)) - (i * (TILESIZEX_HALF));
			_tile[i][j].y = STARTY + (j * (TILESIZEY_HALF)) + (i * (TILESIZEY_HALF));
			_tile[i][j].rc = RectMakeCenter(_tile[i][j].x, _tile[i][j].y, TILESIZEX, TILESIZEY);
		}
	}

	//인덱스 초기화
	_index = { 0, 0 };

	return S_OK;
}

void isometric::release(void)
{
}

void isometric::update(void)
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (PtInRect(&_tile[i][j].rc, _ptMouse))
			{
				if (_ptMouse.y > _tile[i][j].y)
				{
					if (_ptMouse.x > _tile[i][j].x)
					{
						if ((_ptMouse.x) <= _tile[i][j].x + TILESIZEX_HALF + _tile[i][j].y - (_ptMouse.y - _tile[i][j].y))
						{
							_index.x = j;
							_index.y = i;
						}
					}
					else
					{
						if (_ptMouse.x >= (_tile[i][j].x - TILESIZEX_HALF) + ((_ptMouse.y - _tile[i][j].y) * 2))
						{
							_index.x = j;
							_index.y = i;
						}
					}
				}
				else
				{
					if (_ptMouse.x < _tile[i][j].x)
					{
						if (_ptMouse.x >= (_tile[i][j].x - TILESIZEX_HALF) + ((_tile[i][j].y - _ptMouse.y) * 2))
						{
							_index.x = j;
							_index.y = i;
						}
					}
					else
					{
						if (_ptMouse.x <= (_tile[i][j].x + TILESIZEX_HALF) - ((_tile[i][j].y - _ptMouse.y) * 2))
						{
							_index.x = j;
							_index.y = i;
						}
					}
				}
			}
		}
	}


}

void isometric::render(void)
{
	char str[128];

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//디버깅용 렉트 보여주기
			if (KEYMANAGER->isToggleKey(VK_F1))
			{
				Rectangle(getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top, _tile[i][j].rc.right, _tile[i][j].rc.bottom);
			}

			//마름모 그리기
			MoveToEx(getMemDC(), _tile[i][j].x - (TILESIZEX / 2), _tile[i][j].y, NULL);
			LineTo(getMemDC(), _tile[i][j].x, _tile[i][j].y - (TILESIZEY / 2));
			LineTo(getMemDC(), _tile[i][j].x + (TILESIZEX / 2), _tile[i][j].y);
			LineTo(getMemDC(), _tile[i][j].x, _tile[i][j].y + (TILESIZEY / 2));
			LineTo(getMemDC(), _tile[i][j].x - (TILESIZEX / 2), _tile[i][j].y);

			//타일안에 인덱스 표시
			sprintf(str, "[%d, %d]", j, i);
			TextOut(getMemDC(), _tile[i][j].x, _tile[i][j].y - 15, str, strlen(str));
		}
	}

	sprintf(str, "현재인덱스 : [%d,%d]", _index.x, _index.y);
	TextOut(getMemDC(), STARTX + 200, STARTY - 50, str, strlen(str));
}
