#include "stdafx.h"
#include "tank.h"
#include "tankMap.h"

HRESULT tank::init(void)
{
	//��ũ ������ �̹���
	_image = IMAGEMANAGER->addFrameImage("tank", "tank.bmp", 256, 128, 8, 4, true, RGB(255, 0, 255));

	//��ũ���� �ʱ�ȭ
	_direction = TANKDIRECTION_UP;

	//��ũ �ӵ�
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
	//�������� ������ ��Ʈ
	RECT rcCollision;
	rcCollision = _rc;

	//�˻��� Ÿ�� �ε���(�ּ� 2�� �ʿ���)
	int tileIndex[2];

	//���� ��ũ�� ��� �ִ� Ÿ��
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

	//�浹��Ʈ ũ�⸦ Ÿ��ũ�⺸�� �� �۰� ����ش�
	//32 * 32 ������ 0.1������ ������ Ÿ�� ���̸� �������� ���� ����
	rcCollision.left += 1;
	rcCollision.right -= 1;
	rcCollision.top += 1;
	rcCollision.bottom -= 1;

	//��ũ�� �ε����� Ȯ���� �� �ִ�
	tileX = rcCollision.left / TILESIZE;
	tileY = rcCollision.top / TILESIZE;

	//Ÿ�ϸ� �˻��ϱ�(�����¿� ���� 2����)
	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		//����, ���ʾƷ�
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = tileX + (tileY + 1) * TILEX;
		break;
	case TANKDIRECTION_RIGHT:
		//������, �����ʾƷ�
		tileIndex[0] = (tileX + 1) + tileY * TILEX;
		tileIndex[1] = (tileX + 1) + (tileY + 1) * TILEX;
		break;
	case TANKDIRECTION_UP:
		//��, ��������
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = (tileX + 1) + tileY * TILEX;
		break;
	case TANKDIRECTION_DOWN:
		//�Ʒ�, �����ʾƷ�
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
