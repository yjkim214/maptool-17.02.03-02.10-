#include "stdafx.h"
#include "tileMap.h"
#include"enemyManager.h"

HRESULT tileMap::init(void)
{
	//맵 로드
	slot = 1;

	this->load();

	turrainCount = 1;

	setStartPos();

	return S_OK;
}

void tileMap::release(void)
{

}

void tileMap::update(void)
{
	//초기 위치에서 카메라의 위치를 빼준다
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		_tiles[i].rc.left = _initRect[i].left - DRAWRECTMANAGER->getX();
		_tiles[i].rc.right = _initRect[i].right - DRAWRECTMANAGER->getX();
		_tiles[i].rc.top = _initRect[i].top - DRAWRECTMANAGER->getY();
		_tiles[i].rc.bottom = _initRect[i].bottom - DRAWRECTMANAGER->getY();
	}

	if (_em->getEnemyList().empty())
	{
		_attribute[goalIndex] = ATTR_GOAL_OPEN;
	}

	if (turrainCount % 100 == 0)
	{
		if (turrainCount<200)
		{
			for (int i = 0; i < TILEX*TILEY; i++)
			{
				if (_tiles[i].terrain != TR_NONE && ((i / TILEX) + (i%TILEX)) % 2 == 1)
				{
					turrainOnoff[i] = true;
				}
			}

		}
		else
		{
			for (int i = 0; i < TILEX*TILEY; i++)
			{
				if (_tiles[i].terrain != TR_NONE)
				{
					turrainOnoff[i] = !turrainOnoff[i];
				}

			}
			turrainCount = 201;
		}

	}

	turrainCount++;

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
				if (turrainOnoff[i])
				{
					image* temp = IMAGEMANAGER->findImage("colorTile");
					IMAGEMANAGER->render("colorTile", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, (((i / TILEX) + (i%TILEX)) % 2)*temp->getFrameWidth(), 0,
						DRAWRECTMANAGER->getRect().right - _tiles[i].rc.left, temp->getFrameHeight());
				}
				else
				{
					image* temp = IMAGEMANAGER->findImage("tileMapBaseBig");
					IMAGEMANAGER->render("tileMapBaseBig", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX*temp->getFrameWidth(), _tiles[i].terrainFrameY*temp->getFrameHeight(),
						DRAWRECTMANAGER->getRect().right - _tiles[i].rc.left, temp->getFrameHeight());
				}

			}

			else
			{
				if (turrainOnoff[i])
				{
					IMAGEMANAGER->frameRender("colorTile", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, ((i / TILEX) + (i%TILEX)) % 2, 0);
				}
				else
				{
					IMAGEMANAGER->frameRender("tileMapBaseBig", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
				}
			}
		}

		else
		{
			continue;
		}
	}

	if (KEYMANAGER->isToggleKey(VK_F4))
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

void tileMap::objRender(void)
{
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		RECT object = RectMake(_tiles[i].rc.left, _tiles[i].rc.top - 32, 48, 80);
		if (IntersectRect(&DRAWRECTMANAGER->getRect(), &object))
		{
			if (_tiles[i].rc.right > DRAWRECTMANAGER->getRect().right)
			{
				if (!(_tiles[i].obj == OBJECT_BLOCK || _tiles[i].obj == OBJECT_GOLDBLOCK || _tiles[i].obj == OBJECT_BOX|| _tiles[i].obj == OBJECT_GOAL)) continue;
				if (_attribute[i] == ATTR_GOAL_CLOSE)
				{
					image* temp = IMAGEMANAGER->findImage("stairs");
					IMAGEMANAGER->render("stairs", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("stairs")->getFrameHeight() - TILESIZEGAME), 1*temp->getFrameWidth(), 0,
						DRAWRECTMANAGER->getRect().right - _tiles[i].rc.left, temp->getFrameHeight());
				}
				else if (_attribute[i] == ATTR_GOAL_OPEN)
				{
					image* temp = IMAGEMANAGER->findImage("stairs");
					IMAGEMANAGER->render("stairs", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("stairs")->getFrameHeight() - TILESIZEGAME), 0, 0,
						DRAWRECTMANAGER->getRect().right - _tiles[i].rc.left, temp->getFrameHeight());
				}
				else
				{
					image* temp = IMAGEMANAGER->findImage("tileMapBaseBig");
					IMAGEMANAGER->render("tileMapBaseBig", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("tileMapBaseBig")->getFrameHeight() - TILESIZEGAME), _tiles[i].objFrameX*temp->getFrameWidth(), _tiles[i].objFrameY*temp->getFrameHeight(),
						DRAWRECTMANAGER->getRect().right - _tiles[i].rc.left, temp->getFrameHeight());
				}
				
			}

			else
			{
				if (!(_tiles[i].obj == OBJECT_BLOCK || _tiles[i].obj == OBJECT_GOLDBLOCK || _tiles[i].obj == OBJECT_BOX || _tiles[i].obj == OBJECT_GOAL)) continue;
				if (_attribute[i] ==ATTR_GOAL_CLOSE)
				{
					IMAGEMANAGER->frameRender("stairs", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("stairs")->getFrameHeight() - TILESIZEGAME), 1,0);
				}
				else if (_attribute[i] == ATTR_GOAL_OPEN)
				{
					IMAGEMANAGER->frameRender("stairs", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("stairs")->getFrameHeight() - TILESIZEGAME),0,0);
				}
				else
				{
					IMAGEMANAGER->frameRender("tileMapBaseBig", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("tileMapBaseBig")->getFrameHeight() - TILESIZEGAME), _tiles[i].objFrameX, _tiles[i].objFrameY);
				}
				
			}
		}

		else
		{
			continue;
		}
	}
}

void tileMap::load(void)
{
	HANDLE file;
	DWORD read;

	if (slot == 1)
	{
		file = CreateFile("save1.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	}

	else if (slot == 2)
	{
		file = CreateFile("save2.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	}
	
	else if (slot == 3)
	{
		file = CreateFile("save3.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	}

	CloseHandle(file);

	//맵 속성 정의
	memset(_attribute, 0, sizeof(DWORD)* TILEX * TILEY);
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (_tiles[i].obj == OBJECT_BLOCK) _attribute[i] |= ATTR_UNMOVAL;
		if (_tiles[i].obj == OBJECT_GOLDBLOCK) _attribute[i] |= ATTR_UNMOVAL;
		if (_tiles[i].obj == OBJECT_PLAYER) _attribute[i] |= ATTR_PLAYER;
		if (_tiles[i].obj == OBJECT_ENEMY1) _attribute[i] |= ATTR_ENEMY1;
		if (_tiles[i].obj == OBJECT_ENEMY2) _attribute[i] |= ATTR_ENEMY2;
		if (_tiles[i].obj == OBJECT_ENEMY3) _attribute[i] |= ATTR_ENEMY3;
		if (_tiles[i].terrain == TR_NONE) _attribute[i] |= ATTR_NONE;
		if (_tiles[i].obj == OBJECT_BOSS) _attribute[i] |= ATTR_BOSS;
		if (_tiles[i].obj == OBJECT_BOX) _attribute[i] |= ATTR_BOX;
		if (_tiles[i].obj == OBJECT_GOAL)
		{
			goalIndex = i;
			_attribute[i] |= ATTR_GOAL_CLOSE;
		}
	}

	//맵사이즈 수정
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		_tiles[i].rc = RectMake((i % TILEX) * TILESIZEGAME, (i / TILEX) * TILESIZEGAME, TILESIZEGAME, TILESIZEGAME);
		_initRect[i] = _tiles[i].rc;
	}

	for (int i = 0; i < TILEX*TILEY; i++)
	{
		turrainOnoff[i] = false;
	}
	turrainCount = 1;
}

void tileMap::setStartPos()
{
	int indexX;
	int indexY;

	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (_tiles[i].obj == OBJECT_PLAYER)
		{
			indexX = i % TILEX;
			indexY = i / TILEX;
			break;
		}

		else
		{
			indexX = 0;
			indexY = 0;
		}
	}

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

void tileMap::changeSlot(int index)
{
	slot = index;
	load();
	DRAWRECTMANAGER->init();

	setStartPos();
}