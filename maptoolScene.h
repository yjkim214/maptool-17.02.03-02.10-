#pragma once
#include "gameNode.h"
#include "tileNode.h"

class maptoolScene : public gameNode
{
private:
	tagTile _tiles[TILEX * TILEY];	//����ȭ�� Ÿ��(�� 400��)
	tagSampleTile _sampleTiles[SAMPLETILEX * SAMPLETILEY];	//������ȭ�� Ÿ��(�� 160��)
	tagCurrentTile _currentTile;	//����Ÿ��

									//��ư 5��(���̺�, �ε�, ����, ������Ʈ, ���찳)
	RECT _rcSave, _rcLoad, _rcTerrain, _rcObject, _rcEraser;

	RECT _slot1, _slot2, _slot3;

	CTRL _ctrlSelect;

	RECT mapscreen;

	int playerX;
	int playerY;
	int goalX;
	int goalY;


	int slot;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//��������
	void maptoolSetup(void); //�ʱ�ȭ�� �־�θ� �ȴ�
							 //�ʼ���
	void setMap(void);
	//���̺�
	void save(void);
	//�ε�
	void load(void);

	//����, ������Ʈ ����
	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objectSelect(int frameX, int frameY);

	maptoolScene() {}
	~maptoolScene() {}
};

