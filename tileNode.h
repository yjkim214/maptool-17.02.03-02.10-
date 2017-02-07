#pragma once

//����ȭ��Ÿ�� (ȭ�� ����) 
#define TILESIZE 24
#define TILEX 30
#define TILEY 30
#define TILESIZEX (TILEX * TILESIZE)
#define TILESIZEY (TILEY * TILESIZE)
#define SCREENSIZEX (TILESIZE*20)


//�̹���Ÿ�� (ȭ�� �����ʻ��)
#define SAMPLETILEX 7
#define SAMPLETILEY 4
#define SAMPLESTARTX (SCREENSIZEX+TILESIZE*2)

//�̹���Ÿ��(�ΰ���)
#define TILESIZEGAME 48

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
	TR_NONE,TR_GROUND 
};

//������Ʈ
enum OBJECT
{
	OBJECT_BLOCK,
	OBJECT_GOLDBLOCK,
	OBJECT_PLAYER,
	OBJECT_ENEMY1,
	OBJECT_ENEMY2,
	OBJECT_ENEMY3,
	OBJECT_BOSS,
	OBJECT_GOAL,
	OBJECT_NONE
};

//��ġ��ǥ
enum POS
{
	POS_PLAYER,POS_MONSTER1,POS_MONSTER2,POS_MONSTER3
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