#include "stdafx.h"
#include "player.h"
#include "tileMap.h"
#include "heartbeat.h"

HRESULT player::init(void)
{
	//���ݷ� �ʱ�ȭ
	_att = INIT_ATT;

	//hp�ʱ�ȭ
	_maxHp = INIT_MAXHP;
	_hp = INIT_MAXHP;

	//���� �ʱ�ȭ
	_direct = PLAYERDIRECTION_LEFT;

	//��ġ �ʱ�ȭ
	_x = DRAWRECTMANAGER->getX();
	_y = DRAWRECTMANAGER->getY();
	
	//��Ʈ �ʱ�ȭ
	_rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, PLAYER_SIZEX, PLAYER_SIZEY);

	//�ε��� �ʱ�ȭ
	_index.x = (_x + WINSIZEX / 2 - TILESIZEGAME / 2) / PLAYER_SIZEX;
	_index.y = (_y + WINSIZEY / 2 - TILESIZEGAME / 2) / PLAYER_SIZEY;

	//�÷��̾� �ӵ� �ʱ�ȭ
	_speed = 5.0f;

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

	_tileMap = NULL;
	_heartbeat = NULL;

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
		if (_heartbeat->getIsBeat())
		{
			//���� Ű�� ������ ��
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				//�÷��̾��� ������ ����
				_direct = PLAYERDIRECTION_LEFT;
				//�������� �ƴ��� �Ǻ�
				_isLeft = true;

				if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] != ATTR_UNMOVAL)
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosX = _x - PLAYER_SIZEX;

					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x - 1, _index.y);
				}

				//ó���� ��Ŀ�� ������
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				_direct = PLAYERDIRECTION_RIGHT;
				//���� ������ ������ �Ǻ�
				_isLeft = false;

				if (_index.x != TILEX - 1 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] != ATTR_UNMOVAL)
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosX = _x + PLAYER_SIZEX;
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x + 1, _index.y);
				}

				//ó���� ��Ŀ�� ������
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_UP))
			{
				_direct = PLAYERDIRECTION_UP;

				if (_index.y != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] != ATTR_UNMOVAL)
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosY = _y - PLAYER_SIZEY;
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x, _index.y - 1);
				}

				//ó���� ��Ŀ�� ������
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
			{
				_direct = PLAYERDIRECTION_DOWN;

				if (_index.y != TILEY - 1 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] != ATTR_UNMOVAL)
				{
					//�÷��̾ �����̴� ���·�
					_isMove = true;

					//TEST
					_destPosY = _y + PLAYER_SIZEY;
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x, _index.y + 1);
				}

				//ó���� ��Ŀ�� ������
				_heartbeat->removeMarker();
			}
		}

		else
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				_heartbeat->removeMarker();
			}
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
		//ī�޶� ��ġ �缳��
		DRAWRECTMANAGER->setPos(_x, _y);

		//ĳ���� ��ġ�� ���� �ε��� ����
		_index.x = (_x + WINSIZEX / 2 - TILESIZEGAME / 2) / PLAYER_SIZEX;
		_index.y = (_y + WINSIZEY / 2 - TILESIZEGAME / 2) / PLAYER_SIZEY;
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