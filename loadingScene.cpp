#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init(void)
{
	//로딩클래스 생성 및 초기화
	_loading = new loading;
	_loading->init();

	//이미지 및 사운드 로딩
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release(void)
{
	//로딩클래스 해제
	SAFE_DELETE(_loading);
}

void loadingScene::update(void)
{
	//로딩클래스 업데이트
	_loading->update();

	//로딩이 끝난다면
	if (_loading->loadingDone())
	{
		//ESC키를 누르면
		if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE)) 
		{
			//프로세스 종료
			PostQuitMessage(0); 
		}

		else if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			//ESC 키를 제외한 키를 누르면 씬 전환
			SCENEMANAGER->changeScene("메뉴화면");
		}
	}
}

void loadingScene::render(void)
{
	//로딩클래스 렌더
	_loading->render();
	//로딩이 모두 완료되면 
	if (_loading->loadingDone()) { IMAGEMANAGER->render("guiLoadingScene", getMemDC(), 217.5f, 390); }
}

//로딩이미지 함수(이곳에 이미지를 전부 넣어라)
void loadingScene::loadingImage(void)
{
	//_loading->loadImage();
	//_loading->loadFrameImage();

	//로딩이 너무 빠르게 진행되서 천천히 돌아가도록 테스트용으로 만들어 놓자
	for (int i = 0; i < 100; i++)
	{
		_loading->loadImage("테스트", WINSIZEX, WINSIZEY);
	}

	//로딩화면 이미지
	_loading->loadImage("guiLoadingScene", "guiLoadingScene.bmp", 365, 30, true, RGB(255, 0, 255));

	//메뉴화면 이미지
	_loading->loadImage("mainmenu", "mainmenu.bmp", WINSIZEX, WINSIZEY);
	_loading->loadImage("menu_playbutton", "menu_playbutton.bmp", -85, 240, 301, 34, true, RGB(255, 0, 255));
	_loading->loadImage("menu_mapbutton", "menu_mapbutton.bmp", -85, 300, 301, 34, true, RGB(255, 0, 255));
	_loading->loadImage("menu_quitbutton", "menu_quitbutton.bmp", -85, 360, 301, 34, true, RGB(255, 0, 255));
	_loading->loadSound("main_menu", "main_menu.ogg", true, true);

	//백그라운드 이미지
	_loading->loadImage("백그라운드", "background.bmp", WINSIZEX, WINSIZEY);
	
	//맵툴화면
	_loading->loadFrameImage("tileMapBase", "tileMapBase.bmp", 168, 160, 7, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("tileMapBaseBig", "tileMapBaseBig.bmp", 336, 320, 7, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("stairs", "stairs.bmp", 48, 24, 2, 1);

	//게임화면
	_loading->loadFrameImage("player", "player.bmp", 384, 480, 8, 10, true, RGB(255, 0, 255));
	_loading->loadFrameImage("player_jump", "player_jump.bmp", 120, 480, 5, 10, true, RGB(255, 0, 255));
}
//로딩사운드 함수(이곳에 사운드를 전부 넣어라)
void loadingScene::loadingSound(void)
{
}