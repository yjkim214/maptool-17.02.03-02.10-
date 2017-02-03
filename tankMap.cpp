#include "stdafx.h"
#include "tankMap.h"

HRESULT tankMap::init(void)
{
	//�� �ε�
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
	//��üȭ�� ������ �׸���
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		IMAGEMANAGER->frameRender("tilemap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
	}
	//��üȭ�� ������Ʈ�� �׸���
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

	//������Ʈ����
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (_tiles[i].obj == OBJECT_TANK1 || _tiles[i].obj == OBJECT_TANK2)
		{
			_tiles[i].obj = OBJECT_NONE;
		}
	}

	//�� �Ӽ� ����
	memset(_attribute, 0, sizeof(DWORD)* TILEX * TILEY);
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		//������ �� ��������Ʈ���� ��ũ�� �����ٴ� �� ������
		//�Ӽ��� attr_unmoval�� �����Ѵ�
		if (_tiles[i].terrain == TR_WATER) _attribute[i] |= ATTR_UNMOVAL;
		if (_tiles[i].obj == OBJECT_BLOCK1) _attribute[i] |= ATTR_UNMOVAL;
		if (_tiles[i].obj == OBJECT_BLOCK3) _attribute[i] |= ATTR_UNMOVAL;
		if (_tiles[i].obj == OBJECT_BLOCKS) _attribute[i] |= ATTR_UNMOVAL;
	}
}
