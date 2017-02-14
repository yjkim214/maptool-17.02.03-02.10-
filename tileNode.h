#pragma once

//게임화면타일 (화면 왼쪽) 
#define TILESIZE 24
#define TILEX 30
#define TILEY 30
#define TILESIZEX (TILEX * TILESIZE)
#define TILESIZEY (TILEY * TILESIZE)
#define SCREENSIZEX (TILESIZE*20)


//이미지타일 (화면 오른쪽상단)
#define SAMPLETILEX 8
#define SAMPLETILEY 4
#define SAMPLESTARTX (SCREENSIZEX+TILESIZE*2)
#define SAMPLESTARTY 20
//이미지타일(인게임)
#define TILESIZEGAME 48

//지형속성을 내맘대로 정의하기
//||(or), &&(and)
//|(or), &(and)
//0001 | 0000 => 0001
//0001 & 0000 => 0000
#define ATTR_UNMOVAL 0x00000001
#define ATTR_NONE	0x00000002
#define ATTR_PLAYER	0x00000004
#define ATTR_ENEMY1	0x00000008
#define ATTR_ENEMY2	0x00000010
#define ATTR_ENEMY3	0x00000020
#define ATTR_BOSS	0x00000040
#define ATTR_GOAL_CLOSE	0x00000080
#define ATTR_BOX	0x00000100
#define ATTR_GOAL_OPEN	0x00000200




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
	TR_NONE, TR_GROUND
};

//오브젝트
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
	OBJECT_BOX,
	OBJECT_NONE
};

//위치좌표
enum POS
{
	POS_PLAYER, POS_MONSTER1, POS_MONSTER2, POS_MONSTER3
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