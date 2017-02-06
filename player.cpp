#include "stdafx.h"
#include "player.h"

HRESULT player::init(void)
{
	//���ݷ� �ʱ�ȭ
	_att = INIT_ATT;

	//hp�ʱ�ȭ
	_maxHp = INIT_MAXHP;
	_hp = INIT_MAXHP;

	//�ε��� �ʱ�ȭ
	_index = { 0, 0 };

	//���� �ʱ�ȭ
	_direct = PLAYERDIRECTION_LEFT;
	//��ġ �ʱ�ȭ
	_x = 0.0f;
	_y = 0.0f;

	//��Ʈ �ʱ�ȭ
	_rc = RectMake(_x, _y, PLAYER_SIZEX, PLAYER_SIZEY);

	//�÷��̾� �ӵ� �ʱ�ȭ
	_speed = 4.8f;

	//�����̳�?
	_isLeft = false;
	//�������� �ʴ� ����
	_isMove = false;

	//�ִϸ��̼� ���� ���� �ʱ�ȭ
	_count = 0;
	_currentFrameX = 0;
	_playerImg = IMAGEMANAGER->findImage("player");

	//TEST
	_destPosX = _x;
	_destPosY = _y;

	return S_OK;
}

void player::release(void)
{
}

void player::update(void)
{
	//�Լ� ȣ��
	//������
	this->move();
	//�ִϸ��̼�
	this->animation();
}

void player::render(void)
{
	//�÷��̾� ��Ʈ�� �׷���
	RectangleMake(getMemDC(), _rc);
	//�÷��̾� �̹����� �׷���
	_playerImg->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX + 4 * static_cast<int>(_isLeft), 0);
}

void player::move()
{
	//�������� �ʰ� �ִ� ���¶��
	if (!_isMove)
	{
		//���� Ű�� ������ ��
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			//�÷��̾��� ������ ����
			_direct = PLAYERDIRECTION_LEFT;
			//�÷��̾ �����̴� ���·�
			_isMove = true;

			//�������� �ƴ��� �Ǻ�
			_isLeft = true;
			//TEST
			_destPosX = _x - PLAYER_SIZEX;
		}

		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_direct = PLAYERDIRECTION_RIGHT;
			//�÷��̾ �����̴� ���·�
			_isMove = true;

			_isLeft = false;
			//TEST
			_destPosX = _x + PLAYER_SIZEX;
		}

		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			_direct = PLAYERDIRECTION_UP;
			//�÷��̾ �����̴� ���·�
			_isMove = true;

			//TEST
			_destPosY = _y - PLAYER_SIZEY;
		}

		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			_direct = PLAYERDIRECTION_DOWN;
			//�÷��̾ �����̴� ���·�
			_isMove = true;

			//TEST
			_destPosY = _y + PLAYER_SIZEY;
		}
	}

	//�÷��̾ �����̴� ������ ��
	else if (_isMove)
	{
		//���⿡ ���� �÷��̾ �����δ�
		switch (_direct)
		{
		case PLAYERDIRECTION_LEFT:
			_x -= _speed;
			break;
		case PLAYERDIRECTION_RIGHT:
			_x += _speed;
			break;
		case PLAYERDIRECTION_UP:
			_y -= _speed;
			break;
		case PLAYERDIRECTION_DOWN:
			_y += _speed;
			break;
		}

		//�����̴� ������ ��
		//�������� �ε����� �÷��̾��� �Ÿ��� �۾����� ��
		if (getDistance(_x, _y, _destPosX, _destPosY) < _speed)
		{
			//���� �ε����� ��ġ�� �÷��̾ ����
			_x = _destPosX;
			_y = _destPosY;
			//�������� �ʴ� ���·� �����
			_isMove = false;
		}
		//ĳ���� ��Ʈ �缳��
		_rc = RectMake(_x, _y, PLAYER_SIZEX, PLAYER_SIZEY);

		//ĳ���� ��ġ�� ���� �ε��� ����
		_index.x = _x / PLAYER_SIZEX;
		_index.y = _y / PLAYER_SIZEY;
	}
}

void player::animation()
{
	_count++;
	if (_count % 5 == 0)
	{
		_currentFrameX++;

		if (IMAGEMANAGER->findImage("player")->getMaxFrameX() / 2 < _currentFrameX)
		{
			_currentFrameX = 0;
		}

		_count = 0;
	}
}