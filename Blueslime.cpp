#include "stdafx.h"
#include "Blueslime.h"


HRESULT Blueslime::init(void)
{
	return S_OK;
}

HRESULT Blueslime::init(POINT pos)
{
	_image = IMAGEMANAGER->findImage("Blueslime");

	_x = pos.x;
	_y = pos.y;

	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());

	_maxHp = 2;
	_hp = _maxHp;
	return S_OK;
}

void Blueslime::release(void)
{
}

void Blueslime::update(void)
{
	move();
	animation();
}

void Blueslime::render(void)
{
	draw();
}

void Blueslime::move()
{
}

void Blueslime::draw()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top, _image->getFrameX(), _image->getFrameY());
}

void Blueslime::animation()
{
	_animcount++;
	if (_animcount % 8 == 0)
	{
		if (_image->getFrameX() == _image->getMaxFrameX())
		{
			_image->setFrameX(0);
		}

		else
		{
			_image->setFrameX(_image->getFrameX() + 1);
		}

		_animcount = 0;
	}
}
