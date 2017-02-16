#include "stdafx.h"
#include "potion.h"

HRESULT potion::init(void)
{
	_healPt = 1;

	_x = 0.0f;
	_y = 0.0f;
	_rc = RectMake(_x, _y, 48, 48);

	_index = { 0, 0 };

	_isEquip = false;

	return S_OK;
}

void potion::release(void)
{
}

void potion::update(void)
{
	if (_isEquip)
	{
		_x = 136;
		_y = 12;
		_rc = RectMake(_x, _y, 48, 48);
	}

	else
	{
		_x = _index.x * 48 - DRAWRECTMANAGER->getX();
		_y = _index.y * 48 - DRAWRECTMANAGER->getY();
		_rc = RectMake(_x, _y, 48, 48);
	}
}

void potion::render(void)
{
	IMAGEMANAGER->findImage("potion")->render(getMemDC(), _rc.left, _rc.top);
}

void potion::setPos()
{
	_x = _index.x * 48 - DRAWRECTMANAGER->getX();
	_y = _index.y * 48 - DRAWRECTMANAGER->getY();
	_rc = RectMake(_x, _y, 48, 48);
}

void potion::setIndex(POINT index)
{
	_index = index;
}

void potion::setIsEquip(bool isEquip)
{
	if (isEquip) { _index = { -1, -1 }; }
	_isEquip = isEquip;
}