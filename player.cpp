#include "stdafx.h"
#include "player.h"
#include "tileMap.h"
#include "heartbeat.h"

HRESULT player::init(void)
{
	//공격력 초기화
	_att = INIT_ATT;

	//hp초기화
	_maxHp = INIT_MAXHP;
	_hp = INIT_MAXHP;

	//방향 초기화
	_direct = PLAYERDIRECTION_LEFT;

	//위치 초기화
	_x = DRAWRECTMANAGER->getX();
	_y = DRAWRECTMANAGER->getY();
	
	//렉트 초기화
	_rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, PLAYER_SIZEX, PLAYER_SIZEY);

	//인덱스 초기화
	_index.x = (_x + WINSIZEX / 2 - TILESIZEGAME / 2) / PLAYER_SIZEX;
	_index.y = (_y + WINSIZEY / 2 - TILESIZEGAME / 2) / PLAYER_SIZEY;

	//플레이어 속도 초기화
	_speed = 5.0f;

	//왼쪽이냐?
	_isLeft = false;
	//움직이지 않는 상태
	_isMove = false;

	//애니메이션 관련 변수 초기화
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
	//함수 호출
	//움직임
	this->move();
	//애니메이션
	this->animation();
}

void player::render(void)
{
	//플레이어 렉트를 그려줌
	RectangleMake(getMemDC(), _rc);
	//플레이어 이미지를 그려줌
	_playerImg->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX + 4 * static_cast<int>(_isLeft), 0);
}

void player::move()
{
	//움직이지 않고 있는 상태라면
	if (!_isMove)
	{
		if (_heartbeat->getIsBeat())
		{
			//왼쪽 키를 눌렀을 때
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				//플레이어의 방향은 왼쪽
				_direct = PLAYERDIRECTION_LEFT;
				//왼쪽인지 아닌지 판별
				_isLeft = true;

				if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] != ATTR_UNMOVAL)
				{
					//플레이어를 움직이는 상태로
					_isMove = true;

					//TEST
					_destPosX = _x - PLAYER_SIZEX;

					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x - 1, _index.y);
				}

				//처음의 마커를 지워줌
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				_direct = PLAYERDIRECTION_RIGHT;
				//고개를 돌릴지 말지를 판별
				_isLeft = false;

				if (_index.x != TILEX - 1 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] != ATTR_UNMOVAL)
				{
					//플레이어를 움직이는 상태로
					_isMove = true;

					//TEST
					_destPosX = _x + PLAYER_SIZEX;
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x + 1, _index.y);
				}

				//처음의 마커를 지워줌
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_UP))
			{
				_direct = PLAYERDIRECTION_UP;

				if (_index.y != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] != ATTR_UNMOVAL)
				{
					//플레이어를 움직이는 상태로
					_isMove = true;

					//TEST
					_destPosY = _y - PLAYER_SIZEY;
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x, _index.y - 1);
				}

				//처음의 마커를 지워줌
				_heartbeat->removeMarker();
			}

			else if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
			{
				_direct = PLAYERDIRECTION_DOWN;

				if (_index.y != TILEY - 1 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] != ATTR_UNMOVAL)
				{
					//플레이어를 움직이는 상태로
					_isMove = true;

					//TEST
					_destPosY = _y + PLAYER_SIZEY;
					_tileMap->releaseObject(_index.x, _index.y);
					_tileMap->setPlayerAttribute(_index.x, _index.y + 1);
				}

				//처음의 마커를 지워줌
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

	//플레이어가 움직이는 상태일 때
	else if (_isMove)
	{
		//방향에 따라 플레이어를 움직인다
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

		//움직이는 상태일 때
		//목적지의 인덱스와 플레이어의 거리가 작아졌을 때
		if (getDistance(_x, _y, _destPosX, _destPosY) < _speed)
		{
			//다음 인덱스의 위치로 플레이어를 고정
			_x = _destPosX;
			_y = _destPosY;
			//움직이지 않는 상태로 만든다
			_isMove = false;
		}
		//카메라 위치 재설정
		DRAWRECTMANAGER->setPos(_x, _y);

		//캐릭터 위치에 따른 인덱스 설정
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