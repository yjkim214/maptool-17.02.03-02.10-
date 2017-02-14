#include "stdafx.h"
#include "heartbeat.h"

HRESULT heartbeat::init(void)
{
	//비트 카운트 구간을 저장한 파일을 불러옴
	vector<string> vStr;
	vStr = TXTDATA->txtLoad("zone1_1.txt");
	//불러온 파일의 스트링 값을 atoi함수로 int형으로 변경해서 _vHeartbeat 벡터에 담아둠
	for (int i = 0; i < vStr.size(); i++)
	{
		_vBeatTimer.push_back(atoi(vStr[i].c_str()));
	}

	//
	_beatCount = 0;
	//비트의 타이머 초기화
	_beatTimer = 0;
	//비트 타이밍이 되었느냐
	_isMade = false;
	_isBeat = false;

	_beatRc = RectMakeCenter(WINSIZEX / 2, WINSIZEY - 52, 100, 104);

	//심장 위치 초기화
	_heartRc = RectMakeCenter(WINSIZEX / 2, WINSIZEY - 52, 82, 104);
	_heartbeatTimer = 0;
	_isHeartbeat = false;

	//초반에 3개의 비트 체크 박스를 세팅
	for (int i = 0; i < 3; i++)
	{
		BEAT_MARKER newBeatMark;

		newBeatMark.markerRc[0] = RectMakeCenter(WINSIZEX / 2 - _vBeatTimer[i] / 3, WINSIZEY - 52, 12, 64);
		newBeatMark.markerRc[1] = RectMakeCenter(WINSIZEX / 2 + _vBeatTimer[i] / 3, WINSIZEY - 52, 12, 64);

		newBeatMark.markerSpeed = 5.0f;
		newBeatMark.markerImg = IMAGEMANAGER->findImage("TEMP_beat_marker_green");

		_vBeatMarker.push_back(newBeatMark);
	}

	return S_OK;
}

void heartbeat::release(void)
{
	_vBeatTimer.clear();
	_vBeatMarker.clear();
}

void heartbeat::update(void)
{
	//이미 3개를 만들었기 때문에 3번째부터 만들어준다
	//총 비트의 크기의 - 1까지이나 3개를 이미 만들었기에 - 4를 해준다
	if ((_beatTimer > _vBeatTimer[_beatCount + 3] - _vBeatTimer[3] - 200) && (_beatCount < _vBeatTimer.size() - 4) && _isMade)
	{
		BEAT_MARKER newBeatMark;

		newBeatMark.markerRc[0] = RectMakeCenter(0, WINSIZEY - 52, 12, 64);
		newBeatMark.markerRc[1] = RectMakeCenter(WINSIZEX, WINSIZEY - 52, 12, 64);

		newBeatMark.markerSpeed = 5.0f;
		newBeatMark.markerImg = IMAGEMANAGER->findImage("TEMP_beat_marker_green");

		_vBeatMarker.push_back(newBeatMark);

		_isMade = false;
	}

	//총 비트의 개수
	if (_beatCount < _vBeatTimer.size())
	{
		_beatTimer += TIMEMANAGER->getElapedTime() * 1000;

		if (_beatTimer > _vBeatTimer[_beatCount])
		{
			//심장이 뛸때마다 갱신
			_isHeartbeat = true;
			_isMade = true;
			_beatCount++;
		}
	}

	for (int i = 0; i < _vBeatMarker.size(); i++)
	{
		_vBeatMarker[i].markerRc[0].left += _vBeatMarker[i].markerSpeed;
		_vBeatMarker[i].markerRc[0].right += _vBeatMarker[i].markerSpeed;
		_vBeatMarker[i].markerRc[1].left -= _vBeatMarker[i].markerSpeed;
		_vBeatMarker[i].markerRc[1].right -= _vBeatMarker[i].markerSpeed;

		//왼쪽의 마커가 가운데로 모이면 없애준다
		if (_vBeatMarker[i].markerRc[0].left > WINSIZEX / 2)
		{
			_vBeatMarker.erase(_vBeatMarker.begin() + i);
		}
	}

	//마커가 하나라도 있으면
	if (!_vBeatMarker.empty())
	{
		//마커의 왼쪽 렉트가 마커 체크 렉트 안에 있다면
		if (IntersectRect(&_vBeatMarker[0].markerRc[0], &_beatRc))
		{
			_isBeat = true;
		}

		else
		{
			_isBeat = false;
		}
	}

	//심장을 뛰게 만들어준다
	if (_isHeartbeat)
	{
		_heartbeatTimer += TIMEMANAGER->getElapedTime() * 1000;
		if (_heartbeatTimer > 200)
		{
			_isHeartbeat = false;
			_heartbeatTimer = 0;
		}
	}
}

void heartbeat::render(void)
{
	if (KEYMANAGER->isToggleKey(VK_F3))
	{
		RectangleMake(getMemDC(), _beatRc);
	}

	for (int i = 0; i < _vBeatMarker.size(); i++)
	{
		for (int j = 0; j < 2; j++)
		{
			//두개의 마커를 그려준다
			_vBeatMarker[i].markerImg->render(getMemDC(), _vBeatMarker[i].markerRc[j].left, _vBeatMarker[i].markerRc[j].top);
		}
	}

	//심장박동 그림을 그려준다
	IMAGEMANAGER->findImage("heartbeat")->frameRender(getMemDC(), _heartRc.left, _heartRc.top, _isHeartbeat, 0);
}

//처음의 마커를 지워준다
void heartbeat::removeMarker()
{
	if (!_vBeatMarker.empty())
	{
		_vBeatMarker.erase(_vBeatMarker.begin());
	}
}