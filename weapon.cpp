#include "stdafx.h"
#include "weapon.h"

HRESULT weapon::init(WEAPON_KIND kind)
{
	_kind = kind;

	_att = 1;

	_x = 0.0f;
	_y = 0.0f;
	_rc = RectMake(_x, _y, 48, 48);

	_index = { 0, 0 };

	_isEquip = false;

	return S_OK;
}

void weapon::release(void)
{
}

void weapon::update(void)
{
	if (_isEquip)
	{
		_x = 6;
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

void weapon::render(void)
{
	switch (_kind)
	{
	case WEAPON_DAGGER:
		IMAGEMANAGER->findImage("weapon_dagger")->render(getMemDC(), _rc.left, _rc.top);
		break;
	case WEAPON_LONGSWORD:
		IMAGEMANAGER->findImage("weapon_longsword")->render(getMemDC(), _rc.left, _rc.top);
		break;
	case WEAPON_BROADSWORD:
		IMAGEMANAGER->findImage("weapon_broadsword")->render(getMemDC(), _rc.left, _rc.top);
		break;
	}
}

void weapon::setPos()
{
	_x = _index.x * 48 - DRAWRECTMANAGER->getX();
	_y = _index.y * 48 - DRAWRECTMANAGER->getY();
	_rc = RectMake(_x, _y, 48, 48);
}

void weapon::setIndex(POINT index)
{
	_index = index;
}

void weapon::setIsEquip(bool isEquip)
{
	if (isEquip) { _index = { -1, -1 }; }
	_isEquip = isEquip;
}