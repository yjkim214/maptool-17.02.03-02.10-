#include "stdafx.h"
#include "menuScene.h"

HRESULT menuScene::init(void)
{
	//버튼들의 초기 x값
	_initPlayX = IMAGEMANAGER->findImage("menu_playbutton")->getX();
	_initMapX = IMAGEMANAGER->findImage("menu_mapbutton")->getX();
	_initQuitX = IMAGEMANAGER->findImage("menu_quitbutton")->getX();

	//초기 x에서 20을 더한값
	_changePlayX = _initPlayX + 20;
	_changeMapX = _initMapX + 20;
	_changeQuitX = _initQuitX + 20;

	//알파값은 처음엔 255 MAX
	_alphaPlay = 255;
	_alphaMap = 255;
	_alphaQuit = 255;

	SOUNDMANAGER->play("main_menu");

	return S_OK;
}

void menuScene::release(void)
{
}

void menuScene::update(void)
{
	//플레이 버튼 안에 마우스가 있다면
	if (PtInRect(&IMAGEMANAGER->findImage("menu_playbutton")->boundingBox(), _ptMouse))
	{
		//플레이 버튼의 x값을 조정
		IMAGEMANAGER->findImage("menu_playbutton")->setX(_changePlayX);
		//버튼 안에 마우스가 있고 클릭을 한다면 알파값을 변경
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) { _alphaPlay = 150; }
		//버튼 안에 마우스가 있고 마우스를 눌렀다 뗏다면 씬을 변경해준다
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON)){}
	}

	//플레이 버튼 안에 마우스가 없다면
	else
	{
		//플레이 버튼을 원래 위치로
		IMAGEMANAGER->findImage("menu_playbutton")->setX(_initPlayX);
	}

	//맵 버튼 안에 마우스가 있다면 
	if (PtInRect(&IMAGEMANAGER->findImage("menu_mapbutton")->boundingBox(), _ptMouse))
	{
		//맵 버튼의 x값을 조정
		IMAGEMANAGER->findImage("menu_mapbutton")->setX(_changeMapX);
		//만약 마우스가 맵 버튼 안에 있고 왼쪽버튼을 누르게 된다면 알파값을 조정
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) { _alphaMap = 150; }
		//버튼안에 마우스가 있고 마우스 왼쪽 클릭을 뗀다면 씬 전환
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON)) {}
	}

	//맵 버튼 안에 마우스가 없다면
	else
	{
		//맵 버튼을 원래 위치로 조정
		IMAGEMANAGER->findImage("menu_mapbutton")->setX(_initMapX);
	}

	if (PtInRect(&IMAGEMANAGER->findImage("menu_quitbutton")->boundingBox(), _ptMouse))
	{
		IMAGEMANAGER->findImage("menu_quitbutton")->setX(_changeQuitX);
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) { _alphaQuit = 150; }
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON)) { PostQuitMessage(0); }
	}

	else
	{
		IMAGEMANAGER->findImage("menu_quitbutton")->setX(_initQuitX);
	}

	//만약 다른 위치에서 마우스를 뗀다면
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		//알파값을 다시 돌려줌
		_alphaPlay = 255;
		_alphaMap = 255;
		_alphaQuit = 255;
	}
}

void menuScene::render(void)
{
	//알파렌더가 원래 위치를 못잡는다.. ㅠㅠ
	//어쩔수없이 add할 시 집어넣은 x, y 값을 구해서 그려줌
	IMAGEMANAGER->findImage("mainmenu")->render(getMemDC());
	IMAGEMANAGER->findImage("menu_playbutton")->alphaRender(getMemDC(), IMAGEMANAGER->findImage("menu_playbutton")->getX(), IMAGEMANAGER->findImage("menu_playbutton")->getY(), _alphaPlay);
	IMAGEMANAGER->findImage("menu_mapbutton")->alphaRender(getMemDC(), IMAGEMANAGER->findImage("menu_mapbutton")->getX(), IMAGEMANAGER->findImage("menu_mapbutton")->getY(), _alphaMap);
	IMAGEMANAGER->findImage("menu_quitbutton")->alphaRender(getMemDC(), IMAGEMANAGER->findImage("menu_quitbutton")->getX(), IMAGEMANAGER->findImage("menu_quitbutton")->getY(), _alphaQuit);
}