#include "stdafx.h"
#include "maptoolScene.h"

HRESULT maptoolScene::init(void)
{
	//타일맵 이미지 초기화
	IMAGEMANAGER->addFrameImage("tilemap", "tilemap.bmp", 640, 256, SAMPLETILEX, SAMPLETILEY);

	//맵툴셋팅
	this->maptoolSetup();

	//지형그리기 버튼으로 초기화
	_ctrlSelect = CTRL_TERRAINDRAW;

	mapscreen = RectMake(0, 0, SCREENSIZEX, WINSIZEY);

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
		if (PtInRect(&_rcTerrain, _ptMouse))
		{
			_ctrlSelect = CTRL_TERRAINDRAW;
		}
		if (PtInRect(&_rcObject, _ptMouse))
		{
			_ctrlSelect = CTRL_OBJDRAW;
		}
		if (PtInRect(&_rcEraser, _ptMouse))
		{
			_ctrlSelect = CTRL_ERASER;
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
		SCENEMANAGER->changeScene("맵화면");
	}

}

void maptoolScene::render(void)
{
	//타일맵 이미지 렌더
	IMAGEMANAGER->render("tileMapBase", getMemDC(), SAMPLESTARTX, 0);

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
				image* temp = IMAGEMANAGER->findImage("tileMapBase");
				IMAGEMANAGER->render("tileMapBase", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("tileMapBase")->getFrameHeight() - TILESIZE), _tiles[i].objFrameX*temp->getFrameWidth(), _tiles[i].objFrameY*temp->getFrameHeight(),
					mapscreen.right - _tiles[i].rc.left, temp->getFrameHeight());
			}
			else
			{
				if (_tiles[i].obj == OBJECT_NONE) continue;
				IMAGEMANAGER->frameRender("tileMapBase", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top - (IMAGEMANAGER->findImage("tileMapBase")->getFrameHeight() - TILESIZE), _tiles[i].objFrameX, _tiles[i].objFrameY);
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
		HBRUSH o_brush=(HBRUSH)SelectObject(getMemDC(),GetStockObject(NULL_BRUSH));
		HPEN pen=CreatePen(PS_SOLID,1,RGB(255,255,255));
		HPEN o_pen = (HPEN)SelectObject(getMemDC(), pen);
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			if (IntersectRect(&col, &mapscreen, &_tiles[i].rc))
			{
				RectangleMake(getMemDC(), _tiles[i].rc);
			}
		}
		SelectObject(getMemDC(),o_pen);
		SelectObject(getMemDC(), o_brush);
		DeleteObject(pen);

		
	}



	//버튼렉트 렌더
	RectangleMake(getMemDC(), _rcSave);
	RectangleMake(getMemDC(), _rcLoad);
	RectangleMake(getMemDC(), _rcTerrain);
	RectangleMake(getMemDC(), _rcObject);
	RectangleMake(getMemDC(), _rcEraser);

	//버튼렉트 글씨
	SetBkMode(getMemDC(), TRANSPARENT);
	TextOut(getMemDC(), _rcSave.left + 10, _rcSave.top + 10, "SAVE", strlen("SAVE"));
	TextOut(getMemDC(), _rcLoad.left + 10, _rcLoad.top + 10, "LOAD", strlen("LOAD"));
	TextOut(getMemDC(), _rcTerrain.left + 10, _rcTerrain.top + 10, "TERRAIN", strlen("TERRAIN"));
	TextOut(getMemDC(), _rcObject.left + 10, _rcObject.top + 10, "OBJECT", strlen("OBJECT"));
	TextOut(getMemDC(), _rcEraser.left + 10, _rcEraser.top + 10, "ERASER", strlen("ERASER"));

	switch (_ctrlSelect)
	{
	case CTRL_SAVE:

		break;
	case CTRL_LOAD:
		break;
	case CTRL_TERRAINDRAW:
		TextOut(getMemDC(), SAMPLESTARTX, 350, "TERRAIN", strlen("TERRAIN"));
		break;
	case CTRL_OBJDRAW:
		TextOut(getMemDC(), SAMPLESTARTX, 350, "OBJECT", strlen("OBJECT"));
		break;
	case CTRL_ERASER:
		TextOut(getMemDC(), SAMPLESTARTX, 350, "ERASER", strlen("ERASER"));
		break;
	case CTRL_MAP1:
		break;
	case CTRL_MAP2:
		break;
	case CTRL_MAP3:
		break;
	case CTRL_END:
		break;
	default:
		break;
	}


}

void maptoolScene::maptoolSetup(void)
{
	//렉트위치 초기화
	_rcSave = RectMake(SAMPLESTARTX, 200, 100, 50);
	_rcLoad = RectMake(SAMPLESTARTX + 120, 200, 100, 50);
	_rcTerrain = RectMake(SAMPLESTARTX, 200 + 50, 100, 50);
	_rcObject = RectMake(SAMPLESTARTX + 120, 200 + 50, 100, 50);
	_rcEraser = RectMake(SAMPLESTARTX, 200 + 100, 100, 50);

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
			_sampleTiles[i * SAMPLETILEX + j].rc = RectMake(SAMPLESTARTX + j * TILESIZE, i * (TILESIZE+ 16), TILESIZE, TILESIZE+16);
		
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
}

void maptoolScene::setMap(void)
{
	for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
	{
		if (PtInRect(&_sampleTiles[i].rc, _ptMouse))
		{
			_currentTile.x = _sampleTiles[i].terrainFrameX;
			_currentTile.y = _sampleTiles[i].terrainFrameY;
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
						_tiles[i].objFrameX = _currentTile.x;
						_tiles[i].objFrameY = _currentTile.y;
						_tiles[i].obj = objectSelect(_currentTile.x, _currentTile.y);
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

	file = CreateFile("save.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &write, NULL);

	CloseHandle(file);
}

void maptoolScene::load(void)
{
	HANDLE file;
	DWORD read;

	file = CreateFile("save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);

	CloseHandle(file);
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
	return OBJECT_BLOCK;
}
