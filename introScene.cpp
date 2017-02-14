#include "stdafx.h"
#include "introScene.h"

HRESULT introScene::init(void)
{
	//인트로는 로딩 전이기 때문에 여기서 이미지를 추가한다
	IMAGEMANAGER->addImage("blackscreen", "blackscreen.bmp", WINSIZEX, WINSIZEY);
	IMAGEMANAGER->addImage("splash_screen", "splash_screen.bmp", WINSIZEX, WINSIZEY);

	_isStart = false;

	return S_OK;
}

void introScene::release(void)
{
}

void introScene::update(void)
{
	//알파값이 0보다 크면 계속 줄여준다
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		if (!_isStart)
		{
			_isStart = true;
		}
	}

	if (_isStart)
	{
		if (_alpha > 0) { _alpha -= 5; }
		//알파값이 0보다 작아진다면 로딩화면으로 넘어간다
		else { SCENEMANAGER->changeScene("로딩화면"); }
	}
}

void introScene::render(void)
{
	//검은 화면과 인트로 배경을 그려준다
	IMAGEMANAGER->findImage("blackscreen")->render(getMemDC());
	IMAGEMANAGER->findImage("splash_screen")->alphaRender(getMemDC(), _alpha);
}