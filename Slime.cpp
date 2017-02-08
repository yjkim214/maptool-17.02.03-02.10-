#include "stdafx.h"
#include "Slime.h"
#include "tileMap.h"

HRESULT Slime::init(void)
{
	return S_OK;
}

HRESULT Slime::init(POINT index)
{
	_image = IMAGEMANAGER->findImage("Slime");

	_index = index;

	_x = (MON_SIZE / 2) + index.x * MON_SIZE - DRAWRECTMANAGER->getX();
	_y = (MON_SIZE / 2) + index.y * MON_SIZE - DRAWRECTMANAGER->getY();

	_initx = (MON_SIZE / 2) + index.x * MON_SIZE;
	_inity = (MON_SIZE / 2) + index.y * MON_SIZE;

	cout << "_init : " << _initx << endl;
	cout << "_init : " << _inity << endl;

	_dmg = 1;
	_direct = MOVEDIRECTION_LEFT;
	// 디렉션 카운터 별로 들어감 위에 주석을 순서
	_directioncount = RND->getFromIntTo(0, 3);
	//TEST
	//잘하면되
	_destX = _initx;
	_destY = _inity;

	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());

	_maxHp = 1;
	_hp = _maxHp;

	return S_OK;
}

void Slime::release(void)
{
}

void Slime::update(void)
{
	move();
	animation();

	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		cout << "_index : " << _index.x << endl;
		cout << "_index : " << _index.y << endl;
	}
}

void Slime::render(void)
{
	draw();
}

void Slime::move()
{
	if (_isMove == false)
	{
		_movecount++;
		if (_movecount % 100 == 0)
		{
			int rand = RND->getInt(4);
			_direct = static_cast<ENEMY_MOVEDIRECTION>(rand);

			switch (_direct)
			{
			case MOVEDIRECTION_LEFT:
				if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] != ATTR_UNMOVAL)
				{
					_destX = _destX - MON_SIZE;
					_isMove = true;
				}
				break;
			case MOVEDIRECTION_RIGHT:
				if (_index.x != TILEX - 1 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] != ATTR_UNMOVAL)
				{
					_destX = _destX + MON_SIZE;
					_isMove = true;
				}
				break;
			case MOVEDIRECTION_TOP:
				if (_index.y != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] != ATTR_UNMOVAL)
				{
					_destY = _destY - MON_SIZE;
					_isMove = true;
				}
				break;
			case MOVEDIRECTION_BOTTOM:
				if (_index.y != TILEY - 1 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] != ATTR_UNMOVAL)
				{
					_destY = _destY + MON_SIZE;
					_isMove = true;
				}
				break;
			}

			_movecount = 100;
		}
	}

	else if (_isMove)
	{
		switch (_direct)
		{
		case MOVEDIRECTION_LEFT:
			_initx -= _movespeed;
			break;
		case MOVEDIRECTION_TOP:
			_inity -= _movespeed;
			break;
		case MOVEDIRECTION_RIGHT:
			_initx += _movespeed;
			break;
		case MOVEDIRECTION_BOTTOM:
			_inity += _movespeed;
			break;
		}

		if (getDistance(_initx, _inity, _destX, _destY) < _movespeed)
		{
			//다음 인덱스의 위치로 플레이어를 고정
			_initx = _destX;
			_inity = _destY;
			//움직이지 않는 상태로 만든다
			_isMove = false;
		}
	}

	_x = _initx - DRAWRECTMANAGER->getX();
	_y = _inity - DRAWRECTMANAGER->getY();
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());

	_index.x = (_x + DRAWRECTMANAGER->getX()) / TILESIZEGAME;
	_index.y = (_y + DRAWRECTMANAGER->getY()) / TILESIZEGAME;
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
