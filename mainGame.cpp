#include "stdafx.h"
#include "mainGame.h"
//=============================================================
//	## init(void) ## 초기화
//=============================================================
HRESULT mainGame::init(void)
{
	gameNode::init(TRUE);
	
	//앞으로 메인게임 클래스 안에서는 씬들만 관리한다
	//그리고 각각의 씬들이 메인게임역할을 하면 된다
	/*씬추가*/
	SCENEMANAGER->addScene("인트로화면", new introScene);
	SCENEMANAGER->addScene("로딩화면", new loadingScene);
	SCENEMANAGER->addScene("메뉴화면", new menuScene);
	SCENEMANAGER->addScene("맵툴화면", new maptoolScene);
	SCENEMANAGER->addScene("맵화면", new tileMap);
	/*현재씬 설정*/
	SCENEMANAGER->changeScene("인트로화면");

	return S_OK;
}
//=============================================================
//	## release(void) ## 해제
//=============================================================
void mainGame::release(void)
{
	gameNode::release();
}
//=============================================================
//	## update(void) ## 업데이트
//=============================================================
void mainGame::update(void)
{
	gameNode::update();

	//씬매니져 업데이트
	SCENEMANAGER->update();
}
//=============================================================
//	## render(void) ## 렌더
//=============================================================
void mainGame::render(void)
{
	//흰색 빈 비트맵 (렌더에 그냥 그대로 둘것!!)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
//=============================================================
	
	//씬매니져 렌더
	SCENEMANAGER->render();

	//타임매니져 렌더
	//TIMEMANAGER->render(getMemDC());

//=============================================================
	//백버퍼의 내용을 HDC에 그린다 (렌더에 그대로 둘것!!)
	this->getBackBuffer()->render(getHDC());
}