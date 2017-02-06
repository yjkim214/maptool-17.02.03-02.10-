#include "stdafx.h"
#include "tileMap.h"

HRESULT tileMap::init(void)
{
	//맵 로드
	this->load();
	mapscreen = RectMake(0, 0, WINSIZEX, WINSIZEY);
	return S_OK;
}

void tileMap::release(void)
{
}

void tileMap::update(void)
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			_tiles[i].rc.left -= 2;
			_tiles[i].rc.right -= 2;
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			_tiles[i].rc.left += 2;
			_tiles[i].rc.right += 2;
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			_tiles[i].rc.top -= 2;
			_tiles[i].rc.bottom -= 2;
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			_tiles[i].rc.top += 2;
			_tiles[i].rc.bottom += 2;
		}
	}
}

void tileMap::render(void)
{
	//전체화면 지형을 그린다
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		RECT col;
		if (IntersectRect(&col, &mapscreen, &_tiles[i].rc))
		{
			if (_tiles[i].rc.right > mapscreen.right)
			{
				image* temp = IMAGEMANAGER->findImage("tileMapBaseBig");
				IMAGEMANAGER->render("tileMapBaseBig", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX*temp->getFrameWidth(), _tiles[i].terrainFrameY*temp->getFrameHeight(),
					mapscreen.right - _tiles[i].rc.left, temp->getFrameHeight());
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
		if (IntersectRect(&col, &mapscreen, &_tiles[i].rc))
		{
			if (_tiles[i].rc.right > mapscreen.right)
			{
				if (_tiles[i].obj == OBJECT_NONE) continue;
				image* temp = IMAGEMANAGER->findImage("tileMapBaseBig");
				IMAGEMANAGER->render("tileMapBaseBig", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("tileMapBaseBig")->getFrameHeight() - TILESIZEGAME), _tiles[i].objFrameX*temp->getFrameWidth(), _tiles[i].objFrameY*temp->getFrameHeight(),
					mapscreen.right - _tiles[i].rc.left, temp->getFrameHeight());
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
			if (IntersectRect(&col, &mapscreen, &_tiles[i].rc))
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

	file = CreateFile("save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);

	CloseHandle(file);

	

	//맵 속성 정의
	memset(_attribute, 0, sizeof(DWORD)* TILEX * TILEY);
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		//물지형 및 블럭오브젝트들은 탱크가 지나다닐 수 없으니
		//속성을 attr_unmoval로 정의한다
		
		if (_tiles[i].obj == OBJECT_BLOCK) _attribute[i] |= ATTR_UNMOVAL;
		if (_tiles[i].obj == OBJECT_GOLDBLOCK) _attribute[i] |= ATTR_UNMOVAL;
	}

	//맵사이즈 수정
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		//물지형 및 블럭오브젝트들은 탱크가 지나다닐 수 없으니
		//속성을 attr_unmoval로 정의한다
		
		_tiles[i].rc = RectMake((i%TILEX)*TILESIZEGAME, (i/TILEX)*TILESIZEGAME, TILESIZEGAME, TILESIZEGAME);
	}
}

void tileMap::setStartPos(int indexX, int indexY)
{
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		_tiles[i].rc.left -= TILESIZEGAME*indexX+WINSIZEX/2;
		_tiles[i].rc.right -= TILESIZEGAME*indexX + WINSIZEX / 2;
		_tiles[i].rc.top -= TILESIZEGAME*indexY + WINSIZEY / 2;
		_tiles[i].rc.bottom -= TILESIZEGAME*indexY + WINSIZEY / 2;
	}
}
