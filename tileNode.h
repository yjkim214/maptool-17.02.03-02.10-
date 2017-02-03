#pragma once

//게임화면타일 (화면 왼쪽) 
#define TILESIZE 32
#define TILEX 20
#define TILEY 20
#define TILESIZEX (TILEX * TILESIZE)
#define TILESIZEY (TILEY * TILESIZE)

//이미지타일 (화면 오른쪽상단)
#define SAMPLETILEX 20
#define SAMPLETILEY 8

//지형속성을 내맘대로 정의하기
//||(or), &&(and)
//|(or), &(and)
//0001 | 0000 => 0001
//0001 & 0000 => 0000
#define ATTR_UNMOVAL 0x00000001
#define ATTR_POISON	0x00000002


//맵툴에서 사용할 이넘문
enum CTRL
{
	CTRL_SAVE, CTRL_LOAD,
	CTRL_TERRAINDRAW, CTRL_OBJDRAW, CTRL_ERASER,
	CTRL_MAP1, CTRL_MAP2, CTRL_MAP3, CTRL_END
};

//지형
enum TERRAIN
{
	TR_CEMENT, TR_GROUND, TR_GRASS, TR_WATER, TR_END
};

//오브젝트
enum OBJECT
{
	OBJECT_BLOCK1, OBJECT_BLOCK3, OBJECT_BLOCKS,
	OBJECT_TANK1, OBJECT_TANK2,
	OBJECT_FLAG1, OBJECT_FLAG2,
	OBJECT_NONE
};

//위치좌표
enum POS
{
	POS_TANK1, POS_TANK2, POS_FLAG1, POS_FLAG2
};

/*타일구조체*/
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

//이미지 타일구조체
struct tagSampleTile
{
	RECT rc;
	int terrainFrameX;
	int terrainFrameY;
};

//현재타일 구조체
struct tagCurrentTile
{
	int x, y;
};