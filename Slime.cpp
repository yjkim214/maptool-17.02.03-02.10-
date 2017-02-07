#include "stdafx.h"
#include "Slime.h"

HRESULT Slime::init(void)
{
	return S_OK;
}

HRESULT Slime::init(POINT pos)
{
	_image = IMAGEMANAGER->findImage("Slime");

	_x = pos.x;
	_y = pos.y;

	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());

	_maxHp = 1;
	_hp = 1;

	return S_OK;
}

void Slime::release(void)
{
}

void Slime::update(void)
{
	move();
	animation();
}

void Slime::render(void)
{
	draw();
}

void Slime::move()
{

}

void Slime::draw()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top, _image->getFrameX(), _image->getFrameY());
}

void Slime::animation()
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
