#include "stdafx.h"
#include "Dragon.h"
#include "tilemap.h"
#include "player.h"

HRESULT Dragon::init(void)
{
	return S_OK;
}

HRESULT Dragon::init(POINT index)
{
	_image = IMAGEMANAGER->findImage("Dragon");
	_fireimg = IMAGEMANAGER->findImage("fire");
	_index = index;

	_currentframe = 0;
	_x = (MON_SIZE / 2) + index.x * MON_SIZE - DRAWRECTMANAGER->getX();
	_y = (MON_SIZE / 2) + index.y * MON_SIZE - DRAWRECTMANAGER->getY();


	_initx = (MON_SIZE / 2) + index.x * MON_SIZE;
	_inity = (MON_SIZE / 2) + index.y * MON_SIZE;
	_fireimg->setFrameY(1);
	cout << "_init : " << _initx << endl;
	cout << "_init : " << _inity << endl;
	_st = MOVE;
	_dmg = 1;
	cultime = 0;
	bresscount = 0;
	_direct = MOVEDIRECTION_LEFT;
	// 디렉션 카운터 별로 들어감 위에 주석을 순서

	//TEST
	//잘하면되
	_destX = _initx;
	_destY = _inity;

	shakwindow = RectMakeCenter(_x, _y, 300, 300);
	_rc = RectMakeCenter(_x, _y, 48, 48);

	_maxHp = 9;
	_hp = _maxHp;

	for (int i = 0; i < _maxHp; i++)
	{
		addhpbar(_x - 12 * i, _y);
	}

	initAstar();

	return S_OK;
}

void Dragon::release(void)
{
	for (int i = 0; i < _hpbarlist.size(); i++)
	{
		_hpbarlist[i]->release();
		SAFE_DELETE(_hpbarlist[i]);
	}

	_hpbarlist.clear();
}

void Dragon::update(void)
{
	shakwindow = RectMakeCenter(_x, _y, 400, 400);
	if (_player->getIndex().x > _index.x)
	{
		_image->setFrameY(1);
	}
	else if (_player->getIndex().x < _index.x)
	{
		_image->setFrameY(0);
	}
	if (_currentframe >= 5)
	{
		if (_fireimg->getFrameX() != _fireimg->getMaxFrameX())
		{
			_fireimg->setFrameX(_fireimg->getFrameX() + 1);
		}

	}

	// move 관련 
	if (_st == MOVE && bresscount == 0)
	{
		move();
		_fireimg->setFrameX(0);
	}

	if (_player->getIndex().y == _index.y && cultime == 0 && _isMove == false)
	{

		attack();


	}

	if (bresscount != 0)
	{
		bresscount--;

	}
	else if (bresscount == 0)
	{
		_st = MOVE;


	}

	if (cultime != 0 && _st == MOVE)
	{
		cultime--;
	}


	if (_st == ATTCK)
	{
		_x = _initx - DRAWRECTMANAGER->getX();
		_y = _inity - DRAWRECTMANAGER->getY();

		_rc = RectMakeCenter(_x, _y, 48, 48);

		_index.x = (_x + DRAWRECTMANAGER->getX()) / TILESIZEGAME;
		_index.y = (_y + DRAWRECTMANAGER->getY()) / TILESIZEGAME;
	}
	animation();

	//  현재 hp량 을 표시해준다 . 
	for (int i = 0; i < _maxHp - _hp; i++)
	{
		if (i >= _hpbarlist.size())
		{
			continue;
		}
		_hpbarlist[i]->setcurrent(false);
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
	if (KEYMANAGER->isOnceKeyDown(VK_F6))
	{
		searchRoute();
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

void Dragon::render(void)
{
	draw();

	for (int i = 0; i < _hpbarlist.size(); i++)
	{
		_hpbarlist[i]->render();
	}
}

void Dragon::move()
{
	if (_isMove == false)
	{

		_movecount++;
		if (_movecount % 100 == 96)
		{
			while (true)
			{
				searchRouteV2();
				if (findRoute == true)
				{
					break;
				}
				if (_routeState == NO_ROUTE)
				{
					break;
				}
			}
			while (true)
			{
				findBestV2();

				if (_routeState == BEST)
				{
					break;
				}
				if (_routeState == NO_ROUTE)
				{
					break;
				}
			}
		}
		else if (_movecount % 100 == 0)
		{
			int rand = settingMove();
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
			initAstar();
			if (IntersectRect(&shakwindow, &_player->getRect())) {
				DRAWRECTMANAGER->shakeWindow();
			}
		}


	}

	_x = _initx - DRAWRECTMANAGER->getX();
	_y = _inity - DRAWRECTMANAGER->getY();

	_rc = RectMakeCenter(_x, _y, 48, 48);


}

void Dragon::draw()
{

	_image->frameRender(getMemDC(), _rc.left - 37, _rc.top - 54, _currentframe, _image->getFrameY());
	if (_currentframe >= 5 && _fireimg->getFrameX() != _fireimg->getMaxFrameX() && _st == ATTCK)
	{
		if (_index.x > _player->getIndex().x) {
			for (int i = 0; i < _index.x; i++)
			{
				_fireimg->frameRender(getMemDC(), _tileMap->getTiles()[i].rc.left, _rc.top, _fireimg->getFrameX(), _fireimg->getFrameY());
			}
		}
		if (_index.x < _player->getIndex().x) {
			for (int i = _index.x + 1; i < TILEX - 1; i++)
			{
				_fireimg->frameRender(getMemDC(), _tileMap->getTiles()[i].rc.left, _rc.top, _fireimg->getFrameX(), _fireimg->getFrameY());
			}
		}
	}
}

void Dragon::animation()
{
	if (_st == MOVE)
	{

		_animcount++;
		if (_animcount % 15 == 0)
		{
			if (_currentframe >= 1)
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

	else if (_st == ATTCK)
	{
		_animcount++;
		if (_animcount % 12 == 0)
		{
			if (_currentframe != _image->getMaxFrameX())
			{
				_currentframe++;
				_image->setFrameX(_currentframe);
			}
			else if (_currentframe == _image->getMaxFrameX())
			{
				cultime = 350;


			}



		}
	}

}

void Dragon::addhpbar(float x, float y)
{
	habar* newhpbar = new habar;
	newhpbar->init("Mon_hart_front", "Mon_hart_back", x, y - 24, 12, 12);
	_hpbarlist.push_back(newhpbar);
	//it("Mon_hart_front", "Mon_hart_back", x, y - 24, 12, 12);
}

void Dragon::initAstar()
{
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		astar[i].F = 0;
		astar[i].G = 0;
		astar[i].H = 0;
		astar[i].state = NONE;
	}

	findRoute = false;
	_routeState = STANDARD;
	_openList.clear();
	_closeList.clear();
	_finalList.clear();
}

void Dragon::searchRoute()
{
	int selectIndex = -1;
	if (findRoute)
	{
		return;
	}

	if (_openList.empty() && _closeList.empty())
	{
		_openList.push_back(_index.y * TILEX + _index.x);
	}

	else if (_openList.empty() && !_closeList.empty())
	{
		_routeState = NO_ROUTE;
		return;
	}


	selectIndex = findMinimum();


	if (selectIndex >= 0)
	{
		if (selectIndex > TILEX)
		{
			if (selectIndex - TILEX == _player->getIndex().x + _player->getIndex().y*TILEX)
			{
				findRoute = true;
				astar[selectIndex].state = CLOSE;
				_closeList.push_back(selectIndex);

				return;
			}
			else
			{
				if (_tileMap->getAttribute()[selectIndex - TILEX] == 0)
				{

					if (astar[selectIndex - TILEX].state == NONE)
					{
						astar[selectIndex - TILEX].G += astar[selectIndex].G;
						astar[selectIndex - TILEX].H = (abs((selectIndex%TILEX) - _player->getIndex().x) + abs(((selectIndex - TILEX) / TILEX) - _player->getIndex().y)) * 10;
						astar[selectIndex - TILEX].F = astar[selectIndex - TILEX].G + astar[selectIndex - TILEX].H;
						_openList.push_back(selectIndex - TILEX);
						astar[selectIndex - TILEX].state = OPEN;
					}
					else if (astar[selectIndex - TILEX].state == OPEN)
					{
						int tempG = astar[selectIndex - TILEX].G += astar[selectIndex].G;
						if (tempG < astar[selectIndex - TILEX].G)
						{
							astar[selectIndex - TILEX].G = tempG;
						}
						astar[selectIndex - TILEX].F = astar[selectIndex - TILEX].G + astar[selectIndex - TILEX].H;
					}
				}
			}

		}

		if (selectIndex < TILEX*(TILEY - 1))
		{
			if (selectIndex + TILEX == _player->getIndex().x + _player->getIndex().y*TILEX)
			{
				findRoute = true;
				astar[selectIndex].state = CLOSE;
				_closeList.push_back(selectIndex);
				return;
			}
			else
			{
				if (_tileMap->getAttribute()[selectIndex + TILEX] == 0)
				{

					if (astar[selectIndex + TILEX].state == NONE)
					{
						astar[selectIndex + TILEX].G += astar[selectIndex].G;
						astar[selectIndex + TILEX].H = (abs((selectIndex%TILEX) - _player->getIndex().x) + abs(((selectIndex + TILEX) / TILEX) - _player->getIndex().y)) * 10;
						astar[selectIndex + TILEX].F = astar[selectIndex + TILEX].G + astar[selectIndex + TILEX].H;
						_openList.push_back(selectIndex + TILEX);
						astar[selectIndex + TILEX].state = OPEN;
					}
					else if (astar[selectIndex + TILEX].state == OPEN)
					{
						int tempG = astar[selectIndex + TILEX].G += astar[selectIndex].G;
						if (tempG < astar[selectIndex + TILEX].G)
						{
							astar[selectIndex + TILEX].G = tempG;
						}
						astar[selectIndex + TILEX].F = astar[selectIndex + TILEX].G + astar[selectIndex + TILEX].H;
					}
				}
			}

		}

		if (selectIndex%TILEX < TILEX - 1)
		{
			if (selectIndex + 1 == _player->getIndex().x + _player->getIndex().y*TILEX)
			{
				findRoute = true;
				astar[selectIndex].state = CLOSE;
				_closeList.push_back(selectIndex);
				return;
			}
			else
			{
				if (_tileMap->getAttribute()[selectIndex + 1] == 0)
				{

					if (astar[selectIndex + 1].state == NONE)
					{
						astar[selectIndex + 1].G += astar[selectIndex].G;
						astar[selectIndex + 1].H = (abs(((selectIndex%TILEX) + 1) - _player->getIndex().x) + abs((selectIndex / TILEX) - _player->getIndex().y)) * 10;
						astar[selectIndex + 1].F = astar[selectIndex + 1].G + astar[selectIndex + 1].H;
						_openList.push_back(selectIndex + 1);
						astar[selectIndex + 1].state = OPEN;
					}
					else if (astar[selectIndex + 1].state == OPEN)
					{
						int tempG = astar[selectIndex + 1].G += astar[selectIndex].G;
						if (tempG < astar[selectIndex + 1].G)
						{
							astar[selectIndex + 1].G = tempG;
						}
						astar[selectIndex + 1].F = astar[selectIndex + 1].G + astar[selectIndex + 1].H;
					}
				}
			}

		}


		if (selectIndex%TILEX > 0)
		{
			if (selectIndex - 1 == _player->getIndex().x + _player->getIndex().y*TILEX)
			{
				findRoute = true;
				astar[selectIndex].state = CLOSE;
				_closeList.push_back(selectIndex);
				return;
			}
			else
			{
				if (_tileMap->getAttribute()[selectIndex - 1] == 0)
				{

					if (astar[selectIndex - 1].state == NONE)
					{
						astar[selectIndex - 1].G += astar[selectIndex].G;
						astar[selectIndex - 1].H = (abs(((selectIndex%TILEX) - 1) - _player->getIndex().x) + abs((selectIndex / TILEX) - _player->getIndex().y)) * 10;
						astar[selectIndex - 1].F = astar[selectIndex - 1].G + astar[selectIndex - 1].H;
						_openList.push_back(selectIndex - 1);
						astar[selectIndex - 1].state = OPEN;
					}
					else if (astar[selectIndex - 1].state == OPEN)
					{
						int tempG = astar[selectIndex - 1].G += astar[selectIndex].G;
						if (tempG < astar[selectIndex - 1].G)
						{
							astar[selectIndex - 1].G = tempG;
						}
						astar[selectIndex - 1].F = astar[selectIndex - 1].G + astar[selectIndex - 1].H;
					}
				}
			}

		}

		astar[selectIndex].state = CLOSE;
		_closeList.push_back(selectIndex);

	}
	else
	{
		return;
	}

}

void Dragon::searchRouteV2()
{
	int selectIndex = -1;
	if (findRoute)
	{
		return;
	}

	if (_openList.empty() && _closeList.empty())
	{
		_openList.push_back(_index.y * TILEX + _index.x);
	}

	else if (_openList.empty() && !_closeList.empty())
	{
		_routeState = NO_ROUTE;
		return;
	}


	selectIndex = findMinimum();

	if (selectIndex >= 0)
	{
		for (int i = -TILEX; i < TILEX + 1; i += TILEX)
		{
			if (selectIndex + i<0 || selectIndex + i >TILEX*TILEY)
			{
				continue;
			}
			for (int j = -1; j < 2; j++)
			{
				if (selectIndex + i + j == _player->getIndex().x + _player->getIndex().y*TILEX)
				{
					astar[selectIndex].state = CLOSE;
					_routeState = FIND;
					_closeList.push_back(selectIndex);
					findRoute = true;
					return;
				}

				if (j == 1 && selectIndex%TILEX == TILEX - 1)
				{
					continue;
				}

				if (j == -1 && selectIndex%TILEX == 0)
				{
					continue;
				}
				if (astar[selectIndex + i + j].state == CLOSE || _tileMap->getAttribute()[selectIndex + i + j] != 0)
				{
					continue;
				}
				if (i == 0 && j == 0)
				{
					continue;
				}



				int tempG;

				if (i == 0 || j == 0)
				{
					tempG = astar[selectIndex].G + 10;
					if (astar[selectIndex + i + j].state == OPEN)
					{
						if (astar[selectIndex + i + j].G > tempG)
						{
							astar[selectIndex + i + j].G = tempG;
							astar[selectIndex + i + j].F = astar[selectIndex + i + j].G + astar[selectIndex + i + j].H;
						}
					}
					else
					{

						astar[selectIndex + i + j].G = tempG;
						astar[selectIndex + i + j].H = (abs(((selectIndex + i + j) % TILEX) - (_player->getIndex().x)) + abs(((selectIndex + i + j) / TILEX) - (_player->getIndex().y))) * 10;
						astar[selectIndex + i + j].F = astar[selectIndex + i + j].G + astar[selectIndex + i + j].H;
						astar[selectIndex + i + j].state = OPEN;
						_openList.push_back(selectIndex + i + j);
					}
				}
				else
				{
					if (_tileMap->getAttribute()[selectIndex + i] != 0 && _tileMap->getAttribute()[selectIndex + j] != 0)
					{
						continue;
					}
					tempG = astar[selectIndex].G + 14;
					if (astar[selectIndex + i + j].state == OPEN)
					{
						if (astar[selectIndex + i + j].G > tempG)
						{
							astar[selectIndex + i + j].G = tempG;
							astar[selectIndex + i + j].F = astar[selectIndex + i + j].G + astar[selectIndex + i + j].H;
						}
					}
					else
					{
						astar[selectIndex + i + j].G = tempG;
						astar[selectIndex + i + j].H = (abs(((selectIndex + i + j) % TILEX) - (_player->getIndex().x)) + abs(((selectIndex + i + j) / TILEX) - (_player->getIndex().y))) * 10;
						astar[selectIndex + i + j].F = astar[selectIndex + i + j].G + astar[selectIndex + i + j].H;
						astar[selectIndex + i + j].state = OPEN;
						_openList.push_back(selectIndex + i + j);
					}
				}

			}
		}
		astar[selectIndex].state = CLOSE;
		_closeList.push_back(selectIndex);
	}
}

void Dragon::findBest()
{
	int selectIndex;
	int bestIndex = -1;
	int tempG = -1;
	int tempH = -1;
	if (!findRoute)
	{
		return;
	}
	if (_routeState == NO_ROUTE)
	{
		return;
	}

	if (_finalList.empty())
	{
		selectIndex = _player->getIndex().x + _player->getIndex().y*TILEX;
		astar[selectIndex].state = T_BEST;
	}
	else
	{
		selectIndex = _finalList[_finalList.size() - 1];
	}

	if (selectIndex > TILEX)
	{
		if (selectIndex - TILEX == _index.x + _index.y*TILEX)
		{
			_routeState = BEST;

			return;
		}
		else if (astar[selectIndex - TILEX].state == CLOSE)
		{
			if (tempG == -1 && tempH == -1)
			{
				tempG = astar[selectIndex - TILEX].G;
				tempH = astar[selectIndex - TILEX].H;
				bestIndex = selectIndex - TILEX;
			}
			else if (tempG > astar[selectIndex - TILEX].G&&tempH > astar[selectIndex - TILEX].H)
			{
				tempG = astar[selectIndex - TILEX].G;
				tempH = astar[selectIndex - TILEX].H;
				bestIndex = selectIndex - TILEX;
			}
		}

	}

	if (selectIndex < TILEX*(TILEY - 1))
	{
		if (selectIndex + TILEX == _index.x + _index.y*TILEX)
		{
			_routeState = BEST;

			return;
		}
		else if (astar[selectIndex + TILEX].state == CLOSE)
		{
			if (tempG == -1 && tempH == -1)
			{
				tempG = astar[selectIndex + TILEX].G;
				tempH = astar[selectIndex + TILEX].H;
				bestIndex = selectIndex + TILEX;
			}
			else if (tempG > astar[selectIndex + TILEX].G&&tempH > astar[selectIndex + TILEX].H)
			{
				tempG = astar[selectIndex + TILEX].G;
				tempH = astar[selectIndex + TILEX].H;
				bestIndex = selectIndex + TILEX;
			}
		}

	}

	if (selectIndex%TILEX < TILEX)
	{
		if (selectIndex + 1 == _index.x + _index.y*TILEX)
		{
			_routeState = BEST;

			return;
		}
		else if (astar[selectIndex + 1].state == CLOSE)
		{
			if (tempG == -1 && tempH == -1)
			{
				tempG = astar[selectIndex + 1].G;
				tempH = astar[selectIndex + 1].H;
				bestIndex = selectIndex + 1;
			}
			else if (tempG > astar[selectIndex + 1].G&&tempH > astar[selectIndex + 1].H)
			{
				tempG = astar[selectIndex + 1].G;
				tempH = astar[selectIndex + 1].H;
				bestIndex = selectIndex + 1;
			}
		}

	}
	if (selectIndex%TILEX > 0)
	{
		if (selectIndex - 1 == _index.x + _index.y*TILEX)
		{
			_routeState = BEST;

			return;
		}
		else if (astar[selectIndex - 1].state == CLOSE)
		{
			if (tempG == -1 && tempH == -1)
			{
				tempG = astar[selectIndex - 1].G;
				tempH = astar[selectIndex - 1].H;
				bestIndex = selectIndex - 1;
			}
			else if (tempG > astar[selectIndex - 1].G&&tempH > astar[selectIndex - 1].H)
			{
				tempG = astar[selectIndex - 1].G;
				tempH = astar[selectIndex - 1].H;
				bestIndex = selectIndex - 1;
			}
		}

	}

	if (bestIndex == -1)
	{
		_routeState = NO_ROUTE;
	}
	else
	{
		astar[bestIndex].state = T_BEST;
		_finalList.push_back(bestIndex);
	}
}

void Dragon::findBestV2()
{
	int selectIndex;
	int bestIndex = -1;
	int tempG = -1;
	int subIndex = -1;

	if (!findRoute)
	{
		return;
	}
	if (_routeState == NO_ROUTE)
	{
		return;
	}

	if (_finalList.empty())
	{
		selectIndex = _player->getIndex().x + _player->getIndex().y*TILEX;
		astar[selectIndex].state = T_BEST;
	}
	else
	{
		selectIndex = _finalList[_finalList.size() - 1];
	}

	if (selectIndex > -1)
	{
		int times = -1;
		for (int i = -TILEX; i < TILEX + 1; i += TILEX)
		{
			for (int j = -1; j < 2; j++)
			{
				times++;

				if (selectIndex + i + j == _index.x + _index.y*TILEX)
				{
					subIndex = times;
					_routeState = BEST;
					i = 100;
					j = 100;
					continue;
				}

				if (selectIndex + i<0 || selectIndex + i >TILEX*TILEY)
				{
					continue;
				}

				if (j == 1 && selectIndex%TILEX == TILEX - 1)
				{
					continue;
				}

				if (j == -1 && selectIndex%TILEX == 0)
				{
					continue;
				}
				if (astar[selectIndex + i + j].state != CLOSE)
				{
					continue;
				}
				if (i == 0 && j == 0)
				{
					continue;
				}

				if (i == 0 || j == 0)
				{
					if (tempG == -1)
					{
						tempG = astar[selectIndex + i + j].G;
						bestIndex = selectIndex + i + j;
						subIndex = times;
					}
					else if (tempG>astar[selectIndex + i + j].G)
					{
						tempG = astar[selectIndex + i + j].G;
						bestIndex = selectIndex + i + j;
						subIndex = times;
					}
				}
				else
				{
					if (_tileMap->getAttribute()[selectIndex + i] != 0 && _tileMap->getAttribute()[selectIndex + j] != 0)
					{
						continue;
					}

					if (tempG == -1)
					{
						tempG = astar[selectIndex + i + j].G;
						bestIndex = selectIndex + i + j;
						subIndex = times;
					}
					else if (tempG>astar[selectIndex + i + j].G)
					{
						tempG = astar[selectIndex + i + j].G;
						bestIndex = selectIndex + i + j;
						subIndex = times;
					}
				}

			}
		}

		if (subIndex > -1)
		{
			if (_routeState == BEST)
			{
				if (subIndex % 2 == 1)  //십자일경우
				{
					_finalList.push_back(selectIndex);
				}
				else					//대각일 경우
				{
					switch (subIndex)
					{
					case 0:					//좌측상단
						if (_tileMap->getAttribute()[selectIndex - TILEX] != 0)
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex - 1);

							astar[selectIndex - 1].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}
						else if (_tileMap->getAttribute()[selectIndex - 1] != 0)
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex - TILEX);

							astar[selectIndex - TILEX].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}
						else
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex - 1);
							astar[selectIndex - 1].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}
						break;
					case 2:
						if (_tileMap->getAttribute()[selectIndex - TILEX] != 0)
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex + 1);
							astar[selectIndex + 1].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}
						else if (_tileMap->getAttribute()[selectIndex + 1] != 0)
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex - TILEX);

							astar[selectIndex - TILEX].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}
						else
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex + 1);

							astar[selectIndex + 1].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}


						break;
					case 6:
						if (_tileMap->getAttribute()[selectIndex + TILEX] != 0)
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex - 1);

							astar[selectIndex - 1].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}
						else if (_tileMap->getAttribute()[selectIndex - 1] != 0)
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex + TILEX);

							astar[selectIndex + TILEX].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}
						else
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex - 1);

							astar[selectIndex - 1].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}


						break;
					case 8:

						if (_tileMap->getAttribute()[selectIndex + TILEX] != 0)
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex + 1);

							astar[selectIndex + 1].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}
						else if (_tileMap->getAttribute()[selectIndex + 1] != 0)
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex + TILEX);
							astar[selectIndex + TILEX].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}
						else
						{
							_finalList.push_back(selectIndex);
							_finalList.push_back(selectIndex + 1);
							astar[selectIndex + 1].state = T_BEST;
							astar[selectIndex].state = T_BEST;
						}

						break;

					}
				}
			}
			else
			{
				if (subIndex % 2 == 1)  //십자일경우
				{
					_finalList.push_back(bestIndex);
					astar[bestIndex].state = T_BEST;
				}
				else					//대각일 경우
				{
					switch (subIndex)
					{
					case 0:					//좌측상단
						if (_tileMap->getAttribute()[selectIndex - TILEX] != 0)
						{
							_finalList.push_back(selectIndex - 1);
							_finalList.push_back(bestIndex);
							astar[selectIndex - 1].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}
						else if (_tileMap->getAttribute()[selectIndex - 1] != 0)
						{
							_finalList.push_back(selectIndex - TILEX);
							_finalList.push_back(bestIndex);
							astar[selectIndex - TILEX].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}
						else
						{
							_finalList.push_back(selectIndex - 1);
							_finalList.push_back(bestIndex);
							astar[selectIndex - 1].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}
						break;
					case 2:
						if (_tileMap->getAttribute()[selectIndex - TILEX] != 0)
						{
							_finalList.push_back(selectIndex + 1);
							_finalList.push_back(bestIndex);
							astar[selectIndex + 1].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}
						else if (_tileMap->getAttribute()[selectIndex + 1] != 0)
						{
							_finalList.push_back(selectIndex - TILEX);
							_finalList.push_back(bestIndex);
							astar[selectIndex - TILEX].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}
						else
						{
							_finalList.push_back(selectIndex + 1);
							_finalList.push_back(bestIndex);
							astar[selectIndex + 1].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}


						break;
					case 6:
						if (_tileMap->getAttribute()[selectIndex + TILEX] != 0)
						{
							_finalList.push_back(selectIndex - 1);
							_finalList.push_back(bestIndex);
							astar[selectIndex - 1].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}
						else if (_tileMap->getAttribute()[selectIndex - 1] != 0)
						{
							_finalList.push_back(selectIndex + TILEX);
							_finalList.push_back(bestIndex);
							astar[selectIndex + TILEX].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}
						else
						{
							_finalList.push_back(selectIndex - 1);
							_finalList.push_back(bestIndex);
							astar[selectIndex - 1].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}


						break;
					case 8:

						if (_tileMap->getAttribute()[selectIndex + TILEX] != 0)
						{
							_finalList.push_back(selectIndex + 1);
							_finalList.push_back(bestIndex);
							astar[selectIndex + 1].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}
						else if (_tileMap->getAttribute()[selectIndex + 1] != 0)
						{
							_finalList.push_back(selectIndex + TILEX);
							_finalList.push_back(bestIndex);
							astar[selectIndex + TILEX].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}
						else
						{
							_finalList.push_back(selectIndex + 1);
							_finalList.push_back(bestIndex);
							astar[selectIndex + 1].state = T_BEST;
							astar[bestIndex].state = T_BEST;
						}

						break;

					}
				}
			}



		}

	}
}

void Dragon::deleteOpen(int key)
{
	for (int i = 0; i < _openList.size(); i++)
	{
		if (_openList[i] == key)
		{
			_openList.erase(_openList.begin() + i);
		}
	}
}

int Dragon::settingMove()
{
	if (_routeState == BEST)
	{
		if (_finalList.empty())
		{
			_finalList.push_back(_player->getIndex().x + _player->getIndex().y*TILEX);
		}
		int last = _finalList.size() - 1;
		if (_finalList[last] == _index.x + (_index.y - 1)*TILEX)
		{
			return MOVEDIRECTION_TOP;
		}
		else if (_finalList[last] == _index.x + (_index.y + 1)*TILEX)
		{
			return MOVEDIRECTION_BOTTOM;
		}
		else if (_finalList[last] == _index.x - 1 + (_index.y)*TILEX)
		{
			return MOVEDIRECTION_LEFT;
		}
		else if (_finalList[last] == _index.x + 1 + (_index.y)*TILEX)
		{
			return MOVEDIRECTION_RIGHT;
		}
	}
	else
	{
		return RND->getInt(4);
	}
}

int Dragon::findAttribute(int input)
{
	if (input == 2)
	{
		for (int i = 0; i < TILEX*TILEY; i++)
		{

			if (_tileMap->getAttribute()[i] == ATTR_PLAYER)
			{
				return i;
			}
		}
	}
}

int Dragon::findMinimum()
{
	if (_openList.empty())
	{
		return -1;
	}
	int listIndex = 0;
	int number = astar[_openList[listIndex]].F;
	int tileIndex = _openList[listIndex];

	for (int i = 1; i < _openList.size(); i++)
	{
		if (number >= astar[_openList[i]].F)
		{
			listIndex = i;
			number = astar[_openList[i]].F;
			tileIndex = _openList[listIndex];
		}
	}
	_openList.erase(_openList.begin() + listIndex);

	return tileIndex;
}

void Dragon::attack()
{

	_st = ATTCK;
	bresscount = 80;

	if (_currentframe == 6)
	{

		_player->setHp(_player->getHp() - _dmg);

	}



}