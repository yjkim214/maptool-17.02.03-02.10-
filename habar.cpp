#include "stdafx.h"
#include "habar.h"


HRESULT habar::init(char * frontkey, char * backkey, float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	or = false;
	_width = width;
	_height = height;

	_rchpbar = RectMake(x, y, width, height);
	currenthp = true;
	halfhp = false;
	//Ű������ �̹��� �̸�
	char frontImage[128];
	char backImage[128];

	//�޸� ����ϰ� �о��ֱ�
	ZeroMemory(frontImage, sizeof(frontImage));
	ZeroMemory(backImage, sizeof(backImage));
	// ~ . bmp�� �����
	sprintf(frontImage, "%s.bmp", frontkey);
	sprintf(backImage, "%s.bmp", backkey);

	//ü�¹� �̹��� �ʱ�ȭ
	_hpbarFront = IMAGEMANAGER->addImage(frontkey, frontImage, x, y, width, height, true, RGB(255, 0, 255));
	_hpbarBack = IMAGEMANAGER->addImage(backkey, backImage, x, y, width, height, true, RGB(255, 0, 255));

	return S_OK;
}

HRESULT habar::init(char * frontkey, char * halfkey, char * backkey, float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	or = true;
	_width = width;
	_height = height;
	halfhp = false;

	_rchpbar = RectMake(x, y, width, height);
	currenthp = true;
	//Ű������ �̹��� �̸�
	char frontImage[128];
	char halfImage[128];
	char backImage[128];



	//�޸� ����ϰ� �о��ֱ�
	ZeroMemory(frontImage, sizeof(frontImage));
	ZeroMemory(backImage, sizeof(backImage));
	ZeroMemory(halfImage, sizeof(halfImage));
	// ~ . bmp�� �����
	sprintf(frontImage, "%s.bmp", frontkey);
	sprintf(backImage, "%s.bmp", backkey);
	sprintf(halfImage, "%s.bmp", halfkey);

	//ü�¹� �̹��� �ʱ�ȭ
	_hpbarFront = IMAGEMANAGER->addImage(frontkey, frontImage, x, y, width, height, true, RGB(255, 0, 255));
	_hpbarHalf = IMAGEMANAGER->addImage(halfkey, backImage, x, y, width, height, true, RGB(255, 0, 255));
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



	if (or ) {
		if (currenthp && !halfhp)
		{
			_hpbarFront->render(getMemDC(), _rchpbar.left, _y);
		}
		else if (!currenthp && halfhp)
		{
			_hpbarHalf->render(getMemDC(), _rchpbar.left, _y);
		}


	}
	else if (!or )
	{
		if (currenthp)
		{
			_hpbarFront->render(getMemDC(), _rchpbar.left, _y);
		}
	}
}

void habar::setGauge(int currentHp, int maxHp)
{

}