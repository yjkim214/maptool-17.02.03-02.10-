#pragma once

//����ȭ��Ÿ�� (ȭ�� ����) 
#define TILESIZE 32
#define TILEX 20
#define TILEY 20
#define TILESIZEX (TILEX * TILESIZE)
#define TILESIZEY (TILEY * TILESIZE)

//�̹���Ÿ�� (ȭ�� �����ʻ��)
#define SAMPLETILEX 20
#define SAMPLETILEY 8

//�����Ӽ��� ������� �����ϱ�
//||(or), &&(and)
//|(or), &(and)
//0001 | 0000 => 0001
//0001 & 0000 => 0000
#define ATTR_UNMOVAL 0x00000001
#define ATTR_POISON	0x00000002


//�������� ����� �̳ѹ�
enum CTRL
{
	CTRL_SAVE, CTRL_LOAD,
	CTRL_TERRAINDRAW, CTRL_OBJDRAW, CTRL_ERASER,
	CTRL_MAP1, CTRL_MAP2, CTRL_MAP3, CTRL_END
};

//����
enum TERRAIN
{
	TR_CEMENT, TR_GROUND, TR_GRASS, TR_WATER, TR_END
};

//������Ʈ
enum OBJECT
{
	OBJECT_BLOCK1, OBJECT_BLOCK3, OBJECT_BLOCKS,
	OBJECT_TANK1, OBJECT_TANK2,
	OBJECT_FLAG1, OBJECT_FLAG2,
	OBJECT_NONE
};

//��ġ��ǥ
enum POS
{
	POS_TANK1, POS_TANK2, POS_FLAG1, POS_FLAG2
};

/*Ÿ�ϱ���ü*/
struct tagTile
{
	TERRAIN terrain;
	OBJECT obj;
	RECT rc;

	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
};

//�̹��� Ÿ�ϱ���ü
struct tagSampleTile
{
	RECT rc;
	int terrainFrameX;
	int terrainFrameY;
};

//����Ÿ�� ����ü
struct tagCurrentTile
{
	int x, y;
};