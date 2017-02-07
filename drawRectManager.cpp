#include "stdafx.h"
#include "drawRectManager.h"

HRESULT drawRectManager::init(void)
{
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;

	_rc = RectMakeCenter(_x, _y, WINSIZEX, WINSIZEY);

	return S_OK;
}

bool drawRectManager::isImgRender(image * img)
{
	if (IntersectRect(&img->boundingBox(), &_rc)) { return true; }
	return false;
}

bool drawRectManager::isFrameImgRender(image * img)
{
	if (IntersectRect(&img->boundingBoxWithFrame(), &_rc)) { return true; }
	return false;
}

void drawRectManager::setPos(float x, float y)
{
	_x = x;
	_y = y;

	_rc = RectMakeCenter(_x, _y, WINSIZEX, WINSIZEY);
}