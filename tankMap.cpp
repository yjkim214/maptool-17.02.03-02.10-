#include "stdafx.h"
#include "tankMap.h"

HRESULT tankMap::init(void)
{
	//맵 로드
	this->load();

	return S_OK;
}

void tankMap::release(void)
{
}

void tankMap::update(void)
{
}

void tankMap::render(void)
{
	//전체화면 지형을 그린다
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		IMAGEMANAGER->frameRender("tilemap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
	}
	//전체화면 오브젝트를 그린다
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (_tiles[i].obj == OBJECT_NONE) continue;
		IMAGEMANAGER->frameRender("tilemap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].objFrameX, _tiles[i].objFrameY);
	}
}

void tankMap::load(void)
{
	HANDLE file;
	DWORD read;

	file = CreateFile("tiles.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);

	CloseHandle(file);

	//오브젝트정의
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (_tiles[i].obj == OBJECT_TANK1 || _tiles[i].obj == OBJECT_TANK2)
		{
			_tiles[i].obj = OBJECT_NONE;
		}
	}

	//맵 속성 정의
	memset(_attribute, 0, sizeof(DWORD)* TILEX * TILEY);
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		//물지형 및 블럭오브젝트들은 탱크가 지나다닐 수 없으니
		//속성을 attr_unmoval로 정의한다
		if (_tiles[i].terrain == TR_WATER) _attribute[i] |= ATTR_UNMOVAL;
		if (_tiles[i].obj == OBJECT_BLOCK1) _attribute[i] |= ATTR_UNMOVAL;
		if (_tiles[i].obj == OBJECT_BLOCK3) _attribute[i] |= ATTR_UNMOVAL;
		if (_tiles[i].obj == OBJECT_BLOCKS) _attribute[i] |= ATTR_UNMOVAL;
	}
}
