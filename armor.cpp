#include "stdafx.h"
#include "armor.h"

HRESULT armor::init(ARMOR_KIND kind)
{
	_kind = kind;
	
	_hp = 1;

	_x = 0.0f;
	_y = 0.0f;
	_rc = RectMake(_x, _y, 48, 48);

	_index = { 0, 0 };

	_isEquip = false;

	return S_OK;
}

void armor::release(void)
{
}

void armor::update(void)
{
	if (_isEquip)
	{
		_x = 71;
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

void armor::render(void)
{
	switch (_kind)
	{
	case ARMOR_TUNIC:
		IMAGEMANAGER->findImage("armor_tunic")->render(getMemDC(), _rc.left, _rc.top);
		break;
	case ARMOR_LEATHER:
		IMAGEMANAGER->findImage("armor_leather")->render(getMemDC(), _rc.left, _rc.top);
		break;
	case ARMOR_CHAINMAIL:
		IMAGEMANAGER->findImage("armor_chainmail")->render(getMemDC(), _rc.left, _rc.top);
		break;
	case ARMOR_PLATE:
		IMAGEMANAGER->findImage("armor_plate")->render(getMemDC(), _rc.left, _rc.top);
		break;
	case ARMOR_HEAVY:
		IMAGEMANAGER->findImage("armor_heavyplate")->render(getMemDC(), _rc.left, _rc.top);
		break;
	case ARMOR_KARATE:
		IMAGEMANAGER->findImage("armor_karate")->render(getMemDC(), _rc.left, _rc.top);
		break;
	case ARMOR_OBSIDIAN01:
		IMAGEMANAGER->findImage("armor_obsidian01")->render(getMemDC(), _rc.left, _rc.top);
		break;
	case ARMOR_OBSIDIAN02:
		IMAGEMANAGER->findImage("armor_obsidian02")->render(getMemDC(), _rc.left - 2, _rc.top - 2);
		break;
	case ARMOR_OBSIDIAN03:
		IMAGEMANAGER->findImage("armor_obsidian03")->render(getMemDC(), _rc.left - 2, _rc.top - 2);
		break;
	case ARMOR_GLASS:
		IMAGEMANAGER->findImage("armor_glass")->render(getMemDC(), _rc.left, _rc.top);
		break;
	}
}

void armor::setPos()
{
	_x = _index.x * 48 - DRAWRECTMANAGER->getX();
	_y = _index.y * 48 - DRAWRECTMANAGER->getY();
	_rc = RectMake(_x, _y, 48, 48);
}

void armor::setIndex(POINT index)
{
	_index = index;
}

void armor::setIsEquip(bool isEquip)
{
	if (isEquip) { _index = { -1, -1 }; }
	_isEquip = isEquip;
}