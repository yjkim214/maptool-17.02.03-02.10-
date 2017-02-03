#include "stdafx.h"
#include "isometric.h"

HRESULT isometric::init(void)
{
	//Ÿ�� ����ü �ʱ�ȭ
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			_tile[i][j].x = STARTX + (j * (TILESIZEX_HALF)) - (i * (TILESIZEX_HALF));
			_tile[i][j].y = STARTY + (j * (TILESIZEY_HALF)) + (i * (TILESIZEY_HALF));
			_tile[i][j].rc = RectMakeCenter(_tile[i][j].x, _tile[i][j].y, TILESIZEX, TILESIZEY);
		}
	}

	//�ε��� �ʱ�ȭ
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
			//������ ��Ʈ �����ֱ�
			if (KEYMANAGER->isToggleKey(VK_F1))
			{
				Rectangle(getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top, _tile[i][j].rc.right, _tile[i][j].rc.bottom);
			}

			//������ �׸���
			MoveToEx(getMemDC(), _tile[i][j].x - (TILESIZEX / 2), _tile[i][j].y, NULL);
			LineTo(getMemDC(), _tile[i][j].x, _tile[i][j].y - (TILESIZEY / 2));
			LineTo(getMemDC(), _tile[i][j].x + (TILESIZEX / 2), _tile[i][j].y);
			LineTo(getMemDC(), _tile[i][j].x, _tile[i][j].y + (TILESIZEY / 2));
			LineTo(getMemDC(), _tile[i][j].x - (TILESIZEX / 2), _tile[i][j].y);

			//Ÿ�Ͼȿ� �ε��� ǥ��
			sprintf(str, "[%d, %d]", j, i);
			TextOut(getMemDC(), _tile[i][j].x, _tile[i][j].y - 15, str, strlen(str));
		}
	}

	sprintf(str, "�����ε��� : [%d,%d]", _index.x, _index.y);
	TextOut(getMemDC(), STARTX + 200, STARTY - 50, str, strlen(str));
}
