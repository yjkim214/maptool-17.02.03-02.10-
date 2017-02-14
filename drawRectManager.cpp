#include "stdafx.h"
#include "drawRectManager.h"

HRESULT drawRectManager::init(void)
{
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;

	_rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, WINSIZEX, WINSIZEY);

	_oldx = _x;
	_oldy = _y;

	_isShake = false;
	_count = SHAKETIMER;

	return S_OK;
}

void drawRectManager::update(void)
{
	if (_isShake)
	{
		_count--;

		if (_count > 0)
		{
			if (_count % 2 == 0)
			{
				_x -= RND->getFromIntTo(5, 10);
				_y -= RND->getFromIntTo(5, 10);
			}

			else
			{
				_x += RND->getFromIntTo(5, 10);
				_y += RND->getFromIntTo(5, 10);
			}
		}

		else
		{
			_isShake = false;
			_count = SHAKETIMER;
			_x = _oldx;
			_y = _oldy;
		}
	}
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
}

void drawRectManager::shakeWindow()
{
	if (!_isShake)
	{
		_oldx = _x;
		_oldy = _y;

		_isShake = true;
	}
}