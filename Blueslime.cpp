#include "stdafx.h"
#include "Blueslime.h"
#include "tilemap.h"
#include "player.h"

HRESULT Blueslime::init(void)
{
	return S_OK;
}

HRESULT Blueslime::init(POINT index)
{
	_image = IMAGEMANAGER->findImage("Blueslime");

	_index = index;

	_currentframe = 0;
	_x = (MON_SIZE / 2) + index.x * MON_SIZE - DRAWRECTMANAGER->getX();
	_y = (MON_SIZE / 2) + index.y * MON_SIZE - DRAWRECTMANAGER->getY();


	_initx = (MON_SIZE / 2) + index.x * MON_SIZE;
	_inity = (MON_SIZE / 2) + index.y * MON_SIZE;

	cout << "_init : " << _initx << endl;
	cout << "_init : " << _inity << endl;

	_dmg = 1;
	_direct = MOVEDIRECTION_LEFT;
	// 디렉션 카운터 별로 들어감 위에 주석을 순서

	//TEST
	//잘하면되
	_destX = _initx;
	_destY = _inity;

	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());

	_maxHp = 4;
	_hp = _maxHp;

	for (int i = 0; i < _maxHp; i++)
	{
		addhpbar(_x - 12 * i, _y);
	}


	return S_OK;
}

void Blueslime::release(void)
{
	for (int i = 0; i < _hpbarlist.size(); i++)
	{
		_hpbarlist[i]->release();
		SAFE_DELETE(_hpbarlist[i]);
	}

	_hpbarlist.clear();
}

void Blueslime::update(void)
{
	move();
	animation();

	//  현재 hp량 을 표시해준다 . 
	if (_hp != 0)
	{
		for (int i = 0; i < _maxHp - _hp; i++)
		{
			_hpbarlist[i]->setcurrent(false);
			if (i >= _hpbarlist.size())
			{
				continue;
			}
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		/*cout << "_index : " << _index.x << endl;
		cout << "_index : " << _index.y << endl;*/
		for (int i = 0; i < _hpbarlist.size(); i++)
		{
			cout << " hp bar x : " << _hpbarlist[i]->getX() << endl;
			cout << " hp bar y : " << _hpbarlist[i]->getY() << endl;
			cout << " player X : " << _x << endl;
			cout << " player Y : " << _y << endl;
			cout << " player hp : " << _hp << endl;
		}
	}

	//테스트용 
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		_hp--;
	}

	// hp 바를 위에 표시해준다 
	for (int i = 0; i < _hpbarlist.size(); i++)
	{
		_hpbarlist[i]->update();
		_hpbarlist[i]->setX(_x - (12 * (float)(((float)_hpbarlist.size() / 2) - i)));
		_hpbarlist[i]->setY(_y - 24);
	}
	if (_hp <= 0)
	{
		for (int i = 0; i < _hpbarlist.size(); i++)
		{
			_hpbarlist.clear();
		}
	}
}

void Blueslime::render(void)
{
	draw();

	for (int i = 0; i < _hpbarlist.size(); i++)
	{
		_hpbarlist[i]->render();
	}
}

void Blueslime::move()
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
				if (_index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == 0 || _index.x != 0 && _tileMap->getAttribute()[_index.y * TILEX + _index.x - 1] == ATTR_PLAYER)
				{
					if (_player->getIndex().x == _index.x - 1 && _player->getIndex().y == _index.y && !_player->getIsDead())
					{
						_player->setHp(_player->getHp() - _dmg);
						EFFECTMANAGER->addEffect(WINSIZEX / 2, WINSIZEY / 2, "enemy_swipe");
					}

					else {
						_destX = _destX - MON_SIZE;
						_isMove = true;
						_tileMap->releaseEnemyAttribute(_index.x, _index.y);
						_tileMap->setEnemyAttribute(_index.x - 1, _index.y, 1);

						_index.x = _index.x - 1;
					}
				}
				break;
			case MOVEDIRECTION_RIGHT:
				if (_index.x != TILEX - 1 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] == 0 || _index.x != TILEX - 1 && _tileMap->getAttribute()[_index.y * TILEX + _index.x + 1] == ATTR_PLAYER)
				{
					if (_player->getIndex().x == _index.x + 1 && _player->getIndex().y == _index.y && !_player->getIsDead())
					{
						_player->setHp(_player->getHp() - _dmg);
						EFFECTMANAGER->addEffect(WINSIZEX / 2, WINSIZEY / 2, "enemy_swipe");
					}

					else
					{
						_destX = _destX + MON_SIZE;
						_isMove = true;
						_tileMap->releaseEnemyAttribute(_index.x, _index.y);
						_tileMap->setEnemyAttribute(_index.x + 1, _index.y, 1);

						_index.x = _index.x + 1;
					}
				}
				break;
			case MOVEDIRECTION_TOP:
				if (_index.y != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] == 0 || _index.y != 0 && _tileMap->getAttribute()[(_index.y - 1) * TILEX + _index.x] == ATTR_PLAYER)
				{
					if (_player->getIndex().x == _index.x && _player->getIndex().y == _index.y - 1 && !_player->getIsDead())
					{
						_player->setHp(_player->getHp() - _dmg);
						EFFECTMANAGER->addEffect(WINSIZEX / 2, WINSIZEY / 2, "enemy_swipe");
					}

					else {
						_destY = _destY - MON_SIZE;
						_isMove = true;
						_tileMap->releaseEnemyAttribute(_index.x, _index.y);
						_tileMap->setEnemyAttribute(_index.x, _index.y - 1, 1);

						_index.y = _index.y - 1;
					}
				}

				break;
			case MOVEDIRECTION_BOTTOM:
				if (_index.y != TILEY - 1 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] == 0 || _index.y != TILEY - 1 && _tileMap->getAttribute()[(_index.y + 1) * TILEX + _index.x] == ATTR_PLAYER)
				{
					if (_player->getIndex().x == _index.x && _player->getIndex().y == _index.y + 1 && !_player->getIsDead())
					{
						_player->setHp(_player->getHp() - _dmg);
						EFFECTMANAGER->addEffect(WINSIZEX / 2, WINSIZEY / 2, "enemy_swipe");
					}

					else
					{
						_destY = _destY + MON_SIZE;
						_isMove = true;

						_tileMap->releaseEnemyAttribute(_index.x, _index.y);
						_tileMap->setEnemyAttribute(_index.x, _index.y + 1, 1);

						_index.y = _index.y + 1;
					}
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

}

void Blueslime::draw()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top, _currentframe, _image->getFrameY());
}

void Blueslime::animation()
{
	_animcount++;
	if (_animcount % 8 == 0)
	{
		if (_currentframe == _image->getMaxFrameX())
		{
			_currentframe = 0;
		}

		else
		{
			_currentframe++;
			_image->setFrameX(_currentframe);
		}
		_animcount = 0;
	}
}

void Blueslime::addhpbar(float x, float y)
{
	habar* newhpbar = new habar;
	newhpbar->init("Mon_hart_front", "Mon_hart_back", x, y - 24, 12, 12);
	_hpbarlist.push_back(newhpbar);
	//it("Mon_hart_front", "Mon_hart_back", x, y - 24, 12, 12);
}