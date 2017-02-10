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
	
	//백그라운드 이미지
	_loading->loadImage("백그라운드", "background.bmp", WINSIZEX, WINSIZEY);
	
	//맵툴화면
	_loading->loadFrameImage("tileMapBase", "tileMapBase.bmp", 192, 160, 8, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("tileMapBaseBig", "tileMapBaseBig.bmp", 336, 320, 7, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("stairs", "stairs.bmp", 48, 24, 2, 1);

	_loading->loadImage("dragon_green", "dragon_green.bmp", 27, 26, true, RGB(255, 0, 255));
	_loading->loadImage("dragon_green_big", "dragon_green_big.bmp", 54, 52, true, RGB(1, 0, 0));
	_loading->loadImage("maptoolCase", "maptoolCase.bmp", 380, 450, true, RGB(255, 0, 255));
	_loading->loadImage("save", "save.bmp", 71, 44, true, RGB(255, 0, 255));
	_loading->loadImage("load", "load.bmp", 71, 44, true, RGB(255, 0, 255));
	_loading->loadImage("select", "select.bmp", 128, 49, true, RGB(255, 0, 255));
	_loading->loadImage("maptoolframe", "maptoolframe.bmp", 202, 170, true, RGB(255, 0, 255));
	_loading->loadFrameImage("slot1", "slot1.bmp", 76, 88, 1, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("slot2", "slot2.bmp", 76, 88, 1, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("slot3", "slot3.bmp", 76, 88, 1, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("eraser", "eraser.bmp", 86, 88, 1, 2, true, RGB(255, 0, 255));

	//몬스터 
	_loading->loadFrameImage("Slime", "Slime.bmp", 192, 96, 4, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("Blueslime", "Blueslime.bmp", 384, 96, 8, 2, true, RGB(255, 0, 255));

	//게임화면
	_loading->loadFrameImage("player", "player.bmp", 384, 480, 8, 10, true, RGB(255, 0, 255));
	_loading->loadFrameImage("heartbeat", "heartbeat.bmp", 164, 104, 2, 1, true, RGB(255, 0, 255));
	_loading->loadImage("heart", "heart.bmp", 48, 44, true, RGB(255, 0, 255));
	_loading->loadImage("heart_half", "heart_half.bmp", 48, 44, true, RGB(255, 0, 255));
	_loading->loadImage("heart_empty", "heart_empty.bmp", 48, 44, true, RGB(255, 0, 255));
	_loading->loadImage("TEMP_beat_marker_green", "TEMP_beat_marker_green.bmp", 12, 64, true, RGB(255, 0, 255));
}
//로딩사운드 함수(이곳에 사운드를 전부 넣어라)
void loadingScene::loadingSound(void)
{
	//메뉴 사운드 로딩
	_loading->loadSound("main_menu", "main_menu.ogg", TRUE, TRUE);

	//게임씬 사운드 로딩
	_loading->loadSound("zone1_1", "zone1_1.ogg", TRUE, FALSE);
}