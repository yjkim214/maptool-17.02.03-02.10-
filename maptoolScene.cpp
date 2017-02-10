#include "stdafx.h"
#include "maptoolScene.h"

HRESULT maptoolScene::init(void)
{


	//맵툴셋팅
	this->maptoolSetup();

	//지형그리기 버튼으로 초기화
	_ctrlSelect = CTRL_TERRAINDRAW;

	slot = 1;

	mapscreen = RectMake(0, 0, SCREENSIZEX, WINSIZEY);


	playerX = -1;
	playerY = -1;
	goalX = -1;
	goalY = -1;

	return S_OK;
}

void maptoolScene::release(void)
{
}

void maptoolScene::update(void)
{
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON)) this->setMap();
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&_rcSave, _ptMouse))
		{
			_ctrlSelect = CTRL_SAVE;
			this->save();
		}
		if (PtInRect(&_rcLoad, _ptMouse))
		{
			_ctrlSelect = CTRL_LOAD;
			this->load();
		}
		/*if (PtInRect(&_rcTerrain, _ptMouse))
		{
		_ctrlSelect = CTRL_TERRAINDRAW;
		}
		if (PtInRect(&_rcObject, _ptMouse))
		{
		_ctrlSelect = CTRL_OBJDRAW;
		}*/
		if (PtInRect(&_rcEraser, _ptMouse))
		{
			_ctrlSelect = CTRL_ERASER;
		}

		if (PtInRect(&_slot1, _ptMouse))
		{
			slot = 1;
		}

		if (PtInRect(&_slot2, _ptMouse))
		{
			slot = 2;
		}

		if (PtInRect(&_slot3, _ptMouse))
		{
			slot = 3;
		}

	}

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

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		SCENEMANAGER->changeScene("게임화면");
	}

}

void maptoolScene::render(void)
{
	//타일맵 이미지 렌더


	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		//게임타일 렉트 렌더
		/*for (int i = 0; i < TILEX * TILEY; i++)
		{
		RectangleMake(getMemDC(), _tiles[i].rc);
		}*/
		//이미지타일 렉트 렌더
		for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
		{
			RectangleMake(getMemDC(), _sampleTiles[i].rc);
		}
	}

	//전체화면 왼쪽에 지형을 그린다
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		RECT col;
		if (IntersectRect(&col, &mapscreen, &_tiles[i].rc))
		{
			if (_tiles[i].rc.right > mapscreen.right)
			{
				image* temp = IMAGEMANAGER->findImage("tileMapBase");
				IMAGEMANAGER->render("tileMapBase", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX*temp->getFrameWidth(), _tiles[i].terrainFrameY*temp->getFrameHeight(),
					mapscreen.right - _tiles[i].rc.left, temp->getFrameHeight());
			}
			else
			{
				IMAGEMANAGER->frameRender("tileMapBase", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
			}

		}

	}
	//전체화면 왼쪽에 오브젝트를 그린다
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		RECT col;
		if (IntersectRect(&col, &mapscreen, &_tiles[i].rc))
		{
			if (_tiles[i].rc.right > mapscreen.right)
			{
				if (_tiles[i].obj == OBJECT_NONE) continue;
				else if (_tiles[i].obj == OBJECT_BOSS)
				{
					image* temp = IMAGEMANAGER->findImage("dradragon_green_biggon_green");
					IMAGEMANAGER->render("dragon_green_big", getMemDC(), _tiles[i].rc.left - 20, _tiles[i].rc.top - 15);
				}
				else
				{
					image* temp = IMAGEMANAGER->findImage("tileMapBase");
					IMAGEMANAGER->render("tileMapBase", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("tileMapBase")->getFrameHeight() - TILESIZE), _tiles[i].objFrameX*temp->getFrameWidth(), _tiles[i].objFrameY*temp->getFrameHeight(),
						mapscreen.right - _tiles[i].rc.left, temp->getFrameHeight());
				}

			}
			else
			{
				if (_tiles[i].obj == OBJECT_NONE) continue;
				else if (_tiles[i].obj == OBJECT_BOSS)
				{
					image* temp = IMAGEMANAGER->findImage("dragon_green_big");
					IMAGEMANAGER->render("dragon_green_big", getMemDC(), _tiles[i].rc.left - 15, _tiles[i].rc.top - 30);
				}
				else
				{
					IMAGEMANAGER->frameRender("tileMapBase", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("tileMapBase")->getFrameHeight() - TILESIZE), _tiles[i].objFrameX, _tiles[i].objFrameY);
				}

			}

		}

	}

	if (KEYMANAGER->isToggleKey(VK_F1))
	{

		//이미지타일 렉트 렌더
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			RectangleMake(getMemDC(), _tiles[i].rc);
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



	IMAGEMANAGER->render("maptoolCase", getMemDC(), TILESIZE * 20, 0);


	IMAGEMANAGER->render("maptoolframe", getMemDC(), SAMPLESTARTX - 6, SAMPLESTARTY - 4);

	IMAGEMANAGER->render("tileMapBase", getMemDC(), SAMPLESTARTX, SAMPLESTARTY);


	IMAGEMANAGER->render("save", getMemDC(), _rcSave.left, _rcSave.top);
	IMAGEMANAGER->render("load", getMemDC(), _rcLoad.left, _rcLoad.top);


	IMAGEMANAGER->frameRender("slot1", getMemDC(), _slot1.left, _slot1.top, 0, (slot == 1));
	IMAGEMANAGER->frameRender("slot2", getMemDC(), _slot2.left, _slot2.top, 0, (slot == 2));
	IMAGEMANAGER->frameRender("slot3", getMemDC(), _slot3.left, _slot3.top, 0, (slot == 3));
	IMAGEMANAGER->frameRender("eraser", getMemDC(), _rcEraser.left, _rcEraser.top, 0, (_ctrlSelect == CTRL_ERASER));


	IMAGEMANAGER->render("select", getMemDC(), SAMPLESTARTX, 200);
	if (_currentTile.y == 2)
	{
		IMAGEMANAGER->frameRender("tileMapBase", getMemDC(), SAMPLESTARTX + 85, 212, _currentTile.x, _currentTile.y);
	}
	else if (_currentTile.y == 3)
	{
		if (_currentTile.x == 6 && _currentTile.y == 3)
		{
			IMAGEMANAGER->render("dragon_green", getMemDC(), SAMPLESTARTX + 85, 207);
		}
		else
		{
			IMAGEMANAGER->frameRender("tileMapBase", getMemDC(), SAMPLESTARTX + 85, 197, _currentTile.x, _currentTile.y);
		}

	}
	else if (_currentTile.x == 5 && _currentTile.y == 1)
	{
		IMAGEMANAGER->frameRender("tileMapBase", getMemDC(), SAMPLESTARTX + 85, 197, _currentTile.x, _currentTile.y);
	}
	else
	{
		IMAGEMANAGER->frameRender("tileMapBase", getMemDC(), SAMPLESTARTX + 85, 205, _currentTile.x, _currentTile.y);
	}

}

void maptoolScene::maptoolSetup(void)
{
	//렉트위치 초기화
	_rcSave = RectMake(SAMPLESTARTX, 270, IMAGEMANAGER->findImage("save")->getWidth(), IMAGEMANAGER->findImage("save")->getHeight());
	_rcLoad = RectMake(SAMPLESTARTX + 120, 270, IMAGEMANAGER->findImage("load")->getWidth(), IMAGEMANAGER->findImage("load")->getHeight());

	_rcEraser = RectMake(SAMPLESTARTX, 270 + 50, IMAGEMANAGER->findImage("eraser")->getFrameWidth(), IMAGEMANAGER->findImage("eraser")->getFrameHeight());

	_slot1 = RectMake(SAMPLESTARTX, 270 + 120, IMAGEMANAGER->findImage("slot1")->getFrameWidth(), IMAGEMANAGER->findImage("slot1")->getFrameHeight());

	_slot2 = RectMake(SAMPLESTARTX + 80, 270 + 120, IMAGEMANAGER->findImage("slot2")->getFrameWidth(), IMAGEMANAGER->findImage("slot2")->getFrameHeight());

	_slot3 = RectMake(SAMPLESTARTX + 160, 270 + 120, IMAGEMANAGER->findImage("slot3")->getFrameWidth(), IMAGEMANAGER->findImage("slot3")->getFrameHeight());

	//왼쪽 게임화면 렉트 초기화
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			_tiles[i * TILEX + j].rc = RectMake(j * TILESIZE, i * TILESIZE, TILESIZE, TILESIZE);
		}
	}

	//오른쪽 샘플타일 렉트 초기화
	for (int i = 0; i < SAMPLETILEY; i++)
	{
		for (int j = 0; j < SAMPLETILEX; j++)
		{
			_sampleTiles[i * SAMPLETILEX + j].rc = RectMake(SAMPLESTARTX + j * TILESIZE, (i * (TILESIZE + 16)) + SAMPLESTARTY, TILESIZE, TILESIZE + 16);

			//지형세팅
			_sampleTiles[i * SAMPLETILEX + j].terrainFrameX = j;
			_sampleTiles[i * SAMPLETILEX + j].terrainFrameY = i;
		}
	}

	//왼쪽화면을 모두 물로 기본타일 설정하기
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		_tiles[i].terrainFrameX = 4;
		_tiles[i].terrainFrameY = 2;
		_tiles[i].objFrameX = 0;
		_tiles[i].objFrameY = 0;
		_tiles[i].terrain = terrainSelect(_tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
		_tiles[i].obj = OBJECT_NONE;
	}

	_currentTile.x = _sampleTiles[SAMPLETILEX * 2 + 4].terrainFrameX;
	_currentTile.y = _sampleTiles[SAMPLETILEX * 2 + 4].terrainFrameY;

}

void maptoolScene::setMap(void)
{
	for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
	{
		if (PtInRect(&_sampleTiles[i].rc, _ptMouse))
		{
			int indexX = i % SAMPLETILEX;
			int indexY = i / SAMPLETILEX;
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("tileMapBase")->getMemDC(), _ptMouse.x - SAMPLESTARTX, _ptMouse.y - SAMPLESTARTY);
			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (!(r == 255 && g == 0 && b == 255))
			{
				if (i / SAMPLETILEX == 2)
				{
					_ctrlSelect = CTRL_TERRAINDRAW;
				}
				else
				{
					_ctrlSelect = CTRL_OBJDRAW;
				}

				if (indexX >= 5 && indexX <= 7 && indexY >= 2 && indexY <= 3)
				{
					if (indexX == 6 && indexY == 3)
					{
						_currentTile.x = indexX;
						_currentTile.y = indexY;
					}
				}
				else
				{
					_currentTile.x = _sampleTiles[i].terrainFrameX;
					_currentTile.y = _sampleTiles[i].terrainFrameY;
				}
			}
			else
			{

			}
			break;
		}
	}

	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (PtInRect(&mapscreen, _ptMouse))
		{
			if (PtInRect(&_tiles[i].rc, _ptMouse))
			{
				//현재버튼이 지형이냐?
				if (_ctrlSelect == CTRL_TERRAINDRAW)
				{
					_tiles[i].terrainFrameX = _currentTile.x;
					_tiles[i].terrainFrameY = _currentTile.y;
					_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
				}
				//현재버튼이 오브젝트냐?
				if (_ctrlSelect == CTRL_OBJDRAW)
				{
					if (_tiles[i].terrain != TR_NONE)
					{
						if ((objectSelect(_currentTile.x, _currentTile.y) == OBJECT_PLAYER))
						{
							if (playerX != -1 && playerY != -1)
							{
								_tiles[playerY*TILEX + playerX].objFrameX = 0;
								_tiles[playerY*TILEX + playerX].objFrameY = 0;
								_tiles[playerY*TILEX + playerX].obj = OBJECT_NONE;
							}

							_tiles[i].objFrameX = _currentTile.x;
							_tiles[i].objFrameY = _currentTile.y;
							_tiles[i].obj = objectSelect(_currentTile.x, _currentTile.y);
							playerX = i%TILEX;
							playerY = i / TILEX;

						}

						else if ((objectSelect(_currentTile.x, _currentTile.y) == OBJECT_GOAL))
						{
							if (goalX != -1 && goalY != -1)
							{
								_tiles[goalY*TILEX + goalX].objFrameX = 0;
								_tiles[goalY*TILEX + goalX].objFrameY = 0;
								_tiles[goalY*TILEX + goalX].obj = OBJECT_NONE;
							}

							_tiles[i].objFrameX = _currentTile.x;
							_tiles[i].objFrameY = _currentTile.y;
							_tiles[i].obj = objectSelect(_currentTile.x, _currentTile.y);
							goalX = i%TILEX;
							goalY = i / TILEX;
						}

						else
						{
							_tiles[i].objFrameX = _currentTile.x;
							_tiles[i].objFrameY = _currentTile.y;
							_tiles[i].obj = objectSelect(_currentTile.x, _currentTile.y);
						}


					}



				}
				//현재버튼이 지우개냐?
				if (_ctrlSelect == CTRL_ERASER)
				{
					_tiles[i].objFrameX = 0;
					_tiles[i].objFrameY = 0;
					_tiles[i].obj = OBJECT_NONE;
				}
			}
		}
	}
}

void maptoolScene::save(void)
{
	HANDLE file;
	DWORD write;

	/*file = CreateFile("save.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
	FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &write, NULL);*/


	if (slot == 1)
	{
		file = CreateFile("save1.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &write, NULL);
	}
	else if (slot == 2)
	{
		file = CreateFile("save2.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &write, NULL);
	}
	else if (slot == 3)
	{
		file = CreateFile("save3.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &write, NULL);
	}


	CloseHandle(file);
}

void maptoolScene::load(void)
{
	HANDLE file;
	DWORD read;

	/*file = CreateFile("save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
	FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	*/
	if (slot == 1)
	{
		file = CreateFile("save1.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	}
	else if (slot == 2)
	{
		file = CreateFile("save2.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	}
	else if (slot == 3)
	{
		file = CreateFile("save3.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	}


	CloseHandle(file);

	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (_tiles[i].obj == OBJECT_PLAYER)
		{
			playerX = i%TILEX;
			playerY = i / TILEX;
		}

		if (_tiles[i].obj == OBJECT_GOAL)
		{
			goalX = i%TILEX;
			goalY = i / TILEX;
		}

	}

}

TERRAIN maptoolScene::terrainSelect(int frameX, int frameY)
{
	if (frameX == 4 && frameY == 2)
	{
		return TR_NONE;
	}

	return TR_GROUND;
}

OBJECT maptoolScene::objectSelect(int frameX, int frameY)
{
	if (frameX == 4 && frameY == 1)
	{
		return OBJECT_GOLDBLOCK;
	}
	if (frameX == 5 && frameY == 1)
	{
		return OBJECT_GOAL;
	}
	if (frameX == 6 && frameY == 1)
	{
		return OBJECT_BOX;
	}
	if (frameX == 0 && frameY == 3)
	{
		return OBJECT_ENEMY1;
	}
	if (frameX == 1 && frameY == 3)
	{
		return OBJECT_ENEMY2;
	}
	if (frameX == 2 && frameY == 3)
	{
		return OBJECT_ENEMY3;
	}
	if (frameX == 3 && frameY == 3)
	{
		return OBJECT_PLAYER;
	}
	if (frameX == 6 && frameY == 3)
	{
		return OBJECT_BOSS;
	}
	return OBJECT_BLOCK;
}
