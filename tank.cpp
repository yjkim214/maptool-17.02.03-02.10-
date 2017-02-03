#include "stdafx.h"
#include "tank.h"
#include "tankMap.h"

HRESULT tank::init(void)
{
	//탱크 프레임 이미지
	_image = IMAGEMANAGER->addFrameImage("tank", "tank.bmp", 256, 128, 8, 4, true, RGB(255, 0, 255));

	//탱크방향 초기화
	_direction = TANKDIRECTION_UP;

	//탱크 속도
	_speed = 5.0f;

	return S_OK;
}

void tank::release(void)
{
}

void tank::update(void)
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_direction = TANKDIRECTION_LEFT;
		tankMove();
	}
	else if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_direction = TANKDIRECTION_RIGHT;
		tankMove();
	}
	else if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_direction = TANKDIRECTION_UP;
		tankMove();
	}
	else if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		_direction = TANKDIRECTION_DOWN;
		tankMove();
	}
}

void tank::render(void)
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);
}

void tank::tankMove(void)
{
	//가상으로 움직일 렉트
	RECT rcCollision;
	rcCollision = _rc;

	//검사할 타일 인덱스(최소 2개 필요함)
	int tileIndex[2];

	//현재 탱크가 밟고 있는 타일
	int tileX, tileY;

	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		_image->setFrameX(0);
		_image->setFrameY(3);
		_x -= _speed;
		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	case TANKDIRECTION_RIGHT:
		_image->setFrameX(0);
		_image->setFrameY(2);
		_x += _speed;
		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	case TANKDIRECTION_UP:
		_image->setFrameX(0);
		_image->setFrameY(0);
		_y -= _speed;
		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	case TANKDIRECTION_DOWN:
		_image->setFrameX(0);
		_image->setFrameY(1);
		_y += _speed;
		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	}

	//충돌렉트 크기를 타일크기보다 더 작게 깍아준다
	//32 * 32 때문에 0.1오차라도 있으면 타일 사이를 빠져나갈 수가 없다
	rcCollision.left += 1;
	rcCollision.right -= 1;
	rcCollision.top += 1;
	rcCollision.bottom -= 1;

	//탱크의 인덱스를 확인할 수 있다
	tileX = rcCollision.left / TILESIZE;
	tileY = rcCollision.top / TILESIZE;

	//타일맵 검사하기(상하좌우 각각 2개씩)
	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		//왼쪽, 왼쪽아래
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = tileX + (tileY + 1) * TILEX;
		break;
	case TANKDIRECTION_RIGHT:
		//오른쪽, 오른쪽아래
		tileIndex[0] = (tileX + 1) + tileY * TILEX;
		tileIndex[1] = (tileX + 1) + (tileY + 1) * TILEX;
		break;
	case TANKDIRECTION_UP:
		//위, 오른쪽위
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = (tileX + 1) + tileY * TILEX;
		break;
	case TANKDIRECTION_DOWN:
		//아래, 오른쪽아래
		tileIndex[0] = tileX + (tileY + 1) * TILEX;
		tileIndex[1] = (tileX + 1) + (tileY + 1) * TILEX;
		break;
	}

	for (int i = 0; i < 2; i++)
	{
		RECT rc;
		if (IntersectRect(&rc, &_tankMap->getTiles()[tileIndex[i]].rc, &rcCollision) &&
			(_tankMap->getAttribute()[tileIndex[i]] & ATTR_UNMOVAL) == ATTR_UNMOVAL)
		{
			switch (_direction)
			{
			case TANKDIRECTION_LEFT:
				_rc.left = _tankMap->getTiles()[tileIndex[i]].rc.right;
				_rc.right = _rc.left + TILESIZE;
				_x = _rc.left + (_rc.right - _rc.left) / 2;
				break;
			case TANKDIRECTION_RIGHT:
				_rc.right = _tankMap->getTiles()[tileIndex[i]].rc.left;
				_rc.left = _rc.right - TILESIZE;
				_x = _rc.left + (_rc.right - _rc.left) / 2;
				break;
			case TANKDIRECTION_UP:
				_rc.top = _tankMap->getTiles()[tileIndex[i]].rc.bottom;
				_rc.bottom = _rc.top + TILESIZE;
				_y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			case TANKDIRECTION_DOWN:
				_rc.bottom = _tankMap->getTiles()[tileIndex[i]].rc.top;
				_rc.top = _rc.bottom - TILESIZE;
				_y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			}
		}
	}

	rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_rc = rcCollision;
}

void tank::setTankPosition(int index)
{
	_rc = _tankMap->getTiles()[index].rc;
	_x = _rc.left + (_rc.right - _rc.left) / 2;
	_y = _rc.top + (_rc.bottom - _rc.top) / 2;
}
