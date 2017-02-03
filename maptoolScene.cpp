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
}

void maptoolScene::render(void)
{
	//타일맵 이미지 렌더
	IMAGEMANAGER->render("tilemap", getMemDC(), 660, 0);

	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		//게임타일 렉트 렌더
		for (int i = 0; i < TILEX * TILEY; i++)
		{
			RectangleMake(getMemDC(), _tiles[i].rc);
		}
		//이미지타일 렉트 렌더
		for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
		{
			RectangleMake(getMemDC(), _sampleTiles[i].rc);
		}
	}

	//전체화면 왼쪽에 지형을 그린다
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		IMAGEMANAGER->frameRender("tilemap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
	}
	//전체화면 왼쪽에 오브젝트를 그린다
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (_tiles[i].obj == OBJECT_NONE) continue;
		IMAGEMANAGER->frameRender("tilemap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].objFrameX, _tiles[i].objFrameY);
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

}

void maptoolScene::maptoolSetup(void)
{
	//렉트위치 초기화
	_rcSave = RectMake(660, 400, 100, 50);
	_rcLoad = RectMake(660 + 120, 400, 100, 50);
	_rcTerrain = RectMake(660, 400 + 100, 100, 50);
	_rcObject = RectMake(660 + 120, 400 + 100, 100, 50);
	_rcEraser = RectMake(660 + 240, 400 + 100, 100, 50);

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
			_sampleTiles[i * SAMPLETILEX + j].rc = RectMake(660 + j * TILESIZE, i * TILESIZE, TILESIZE, TILESIZE);
		
			//지형세팅
			_sampleTiles[i * SAMPLETILEX + j].terrainFrameX = j;
			_sampleTiles[i * SAMPLETILEX + j].terrainFrameY = i;
		}
	}

	//왼쪽화면을 모두 물로 기본타일 설정하기
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		_tiles[i].terrainFrameX = 4;
		_tiles[i].terrainFrameY = 0;
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
				_tiles[i].objFrameX = _currentTile.x;
				_tiles[i].objFrameY = _currentTile.y;
				_tiles[i].obj = objectSelect(_currentTile.x, _currentTile.y);
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
	if (frameX == 1 && frameY == 0)
	{
		return TR_CEMENT;
	}
	if (frameX == 2 && frameY == 0)
	{
		return TR_GROUND;
	}
	if (frameX == 3 && frameY == 0)
	{
		return TR_GRASS;
	}
	if (frameX == 4 && frameY == 0)
	{
		return TR_WATER;
	}

	return TR_GROUND;
}

OBJECT maptoolScene::objectSelect(int frameX, int frameY)
{
	return OBJECT_BLOCK1;
}
