#include "stdafx.h"
#include "habar.h"


HRESULT habar::init(char * frontkey, char * backkey, float x, float y, float width, float height)
{
	_x = x;
	_y = y;

	_width = width;
	_height = height;

	_rchpbar = RectMake(x, y, width, height);
	currenthp = true;
	//키값으로 이미지 이름
	char frontImage[128];
	char backImage[128];

	//메모리 깔끔하게 밀어주기
	ZeroMemory(frontImage, sizeof(frontImage));
	ZeroMemory(backImage, sizeof(backImage));
	// ~ . bmp로 만들기
	sprintf(frontImage, "%s.bmp", frontkey);
	sprintf(backImage, "%s.bmp", backkey);

	//체력바 이미지 초기화
	_hpbarFront = IMAGEMANAGER->addImage(frontkey, frontImage, x, y, width, height, true, RGB(255, 0, 255));
	_hpbarBack = IMAGEMANAGER->addImage(backkey, backImage, x, y, width, height, true, RGB(255, 0, 255));

	return S_OK;
}

void habar::release(void)
{
}

void habar::update(void)
{
	_rchpbar = RectMake(_x, _y, _hpbarBack->getWidth(), _hpbarBack->getHeight());
}

void habar::render(void)
{
	_hpbarBack->render(getMemDC(), _rchpbar.left, _y);

	if (currenthp)
	{
		_hpbarFront->render(getMemDC(), _rchpbar.left, _y);
	}
}

void habar::setGauge(int currentHp, int maxHp)
{

}