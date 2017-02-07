#include "stdafx.h"
#include "tileMap.h"

HRESULT tileMap::init(void)
{
	//맵 로드
	this->load();

	setStartPos(1, 0);

	cameraPosX = DRAWRECTMANAGER->getX();
	cameraPosY = DRAWRECTMANAGER->getY();

	return S_OK;
}

void tileMap::release(void)
{
}

void tileMap::update(void)
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		DRAWRECTMANAGER->setX(DRAWRECTMANAGER->getX() - 2);
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		DRAWRECTMANAGER->setX(DRAWRECTMANAGER->getX() + 2);
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		DRAWRECTMANAGER->setY(DRAWRECTMANAGER->getY() - 2);
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		DRAWRECTMANAGER->setY(DRAWRECTMANAGER->getY() + 2);
	}

	if (cameraPosX != DRAWRECTMANAGER->getX())
	{
		float speed = cameraPosX - DRAWRECTMANAGER->getX();
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			_tiles[i].rc.left += speed;
			_tiles[i].rc.right += speed;
		}
		cameraPosX = DRAWRECTMANAGER->getX();
	}

	if (cameraPosY != DRAWRECTMANAGER->getY())
	{
		float speed = cameraPosY - DRAWRECTMANAGER->getY();
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			_tiles[i].rc.top += speed;
			_tiles[i].rc.bottom += speed;
		}
		cameraPosY = DRAWRECTMANAGER->getY();
	}

}

void tileMap::render(void)
{
	//전체화면 지형을 그린다
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		RECT col;
		if (IntersectRect(&col, &DRAWRECTMANAGER->getRect(), &_tiles[i].rc))
		{
			if (_tiles[i].rc.right > DRAWRECTMANAGER->getRect().right)
			{
				image* temp = IMAGEMANAGER->findImage("tileMapBaseBig");
				IMAGEMANAGER->render("tileMapBaseBig", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX*temp->getFrameWidth(), _tiles[i].terrainFrameY*temp->getFrameHeight(),
					DRAWRECTMANAGER->getRect().right - _tiles[i].rc.left, temp->getFrameHeight());
			}

			else
			{
				IMAGEMANAGER->frameRender("tileMapBaseBig", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
			}
		}

		else
		{
			continue;
		}
	}

	for (int i = 0; i < TILEX * TILEY; i++)
	{
		RECT col;
		if (IntersectRect(&col, &DRAWRECTMANAGER->getRect(), &_tiles[i].rc))
		{
			if (_tiles[i].rc.right > DRAWRECTMANAGER->getRect().right)
			{
				if (_tiles[i].obj == OBJECT_NONE) continue;
				image* temp = IMAGEMANAGER->findImage("tileMapBaseBig");
				IMAGEMANAGER->render("tileMapBaseBig", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("tileMapBaseBig")->getFrameHeight() - TILESIZEGAME), _tiles[i].objFrameX*temp->getFrameWidth(), _tiles[i].objFrameY*temp->getFrameHeight(),
					DRAWRECTMANAGER->getRect().right - _tiles[i].rc.left, temp->getFrameHeight());
			}

			else
			{
				if (_tiles[i].obj == OBJECT_NONE) continue;
				IMAGEMANAGER->frameRender("tileMapBaseBig", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("tileMapBaseBig")->getFrameHeight() - TILESIZEGAME), _tiles[i].objFrameX, _tiles[i].objFrameY);
			}
		}

		else
		{
			continue;
		}
	}

	if (KEYMANAGER->isToggleKey(VK_F2))
	{
		//이미지타일 렉트 렌더
		RECT col;
		HBRUSH o_brush = (HBRUSH)SelectObject(getMemDC(), GetStockObject(NULL_BRUSH));
		HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		HPEN o_pen = (HPEN)SelectObject(getMemDC(), pen);

		for (int i = 0; i < TILEX * TILEY; i++)
		{
			if (IntersectRect(&col, &DRAWRECTMANAGER->getRect(), &_tiles[i].rc))
			{
				RectangleMake(getMemDC(), _tiles[i].rc);
			}
		}

		SelectObject(getMemDC(), o_pen);
		SelectObject(getMemDC(), o_brush);
		DeleteObject(pen);
	}
}

void tileMap::load(void)
{
	HANDLE file;
	DWORD read;

	file = CreateFile("save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);

	CloseHandle(file);

	//맵 속성 정의
	memset(_attribute, 0, sizeof(DWORD)* TILEX * TILEY);
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (_tiles[i].obj == OBJECT_BLOCK) _attribute[i] |= ATTR_UNMOVAL;
		if (_tiles[i].obj == OBJECT_GOLDBLOCK) _attribute[i] |= ATTR_UNMOVAL;
	}

	//맵사이즈 수정
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		_tiles[i].rc = RectMake((i % TILEX) * TILESIZEGAME, (i / TILEX) * TILESIZEGAME, TILESIZEGAME, TILESIZEGAME);
	}
}

void tileMap::setStartPos(int indexX, int indexY)
{
	DRAWRECTMANAGER->setX(TILESIZEGAME * indexX - DRAWRECTMANAGER->getX() + TILESIZEGAME / 2);
	DRAWRECTMANAGER->setY(TILESIZEGAME * indexY - DRAWRECTMANAGER->getY() + TILESIZEGAME / 2);
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		_tiles[i].rc.left -= DRAWRECTMANAGER->getX();
		_tiles[i].rc.right -= DRAWRECTMANAGER->getX();
		_tiles[i].rc.top -= DRAWRECTMANAGER->getY();
		_tiles[i].rc.bottom -= DRAWRECTMANAGER->getY();
	}
}