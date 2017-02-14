#include "stdafx.h"
#include "heartbeat.h"

HRESULT heartbeat::init(void)
{
	//��Ʈ ī��Ʈ ������ ������ ������ �ҷ���
	vector<string> vStr;
	vStr = TXTDATA->txtLoad("zone1_1.txt");
	//�ҷ��� ������ ��Ʈ�� ���� atoi�Լ��� int������ �����ؼ� _vHeartbeat ���Ϳ� ��Ƶ�
	for (int i = 0; i < vStr.size(); i++)
	{
		_vBeatTimer.push_back(atoi(vStr[i].c_str()));
	}

	//
	_beatCount = 0;
	//��Ʈ�� Ÿ�̸� �ʱ�ȭ
	_beatTimer = 0;
	//��Ʈ Ÿ�̹��� �Ǿ�����
	_isMade = false;
	_isBeat = false;

	_beatRc = RectMakeCenter(WINSIZEX / 2, WINSIZEY - 52, 100, 104);

	//���� ��ġ �ʱ�ȭ
	_heartRc = RectMakeCenter(WINSIZEX / 2, WINSIZEY - 52, 82, 104);
	_heartbeatTimer = 0;
	_isHeartbeat = false;

	//�ʹݿ� 3���� ��Ʈ üũ �ڽ��� ����
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
	//�̹� 3���� ������� ������ 3��°���� ������ش�
	//�� ��Ʈ�� ũ���� - 1�����̳� 3���� �̹� ������⿡ - 4�� ���ش�
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

	//�� ��Ʈ�� ����
	if (_beatCount < _vBeatTimer.size())
	{
		_beatTimer += TIMEMANAGER->getElapedTime() * 1000;

		if (_beatTimer > _vBeatTimer[_beatCount])
		{
			//������ �۶����� ����
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

		//������ ��Ŀ�� ����� ���̸� �����ش�
		if (_vBeatMarker[i].markerRc[0].left > WINSIZEX / 2)
		{
			_vBeatMarker.erase(_vBeatMarker.begin() + i);
		}
	}

	//��Ŀ�� �ϳ��� ������
	if (!_vBeatMarker.empty())
	{
		//��Ŀ�� ���� ��Ʈ�� ��Ŀ üũ ��Ʈ �ȿ� �ִٸ�
		if (IntersectRect(&_vBeatMarker[0].markerRc[0], &_beatRc))
		{
			_isBeat = true;
		}

		else
		{
			_isBeat = false;
		}
	}

	//������ �ٰ� ������ش�
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
			//�ΰ��� ��Ŀ�� �׷��ش�
			_vBeatMarker[i].markerImg->render(getMemDC(), _vBeatMarker[i].markerRc[j].left, _vBeatMarker[i].markerRc[j].top);
		}
	}

	//����ڵ� �׸��� �׷��ش�
	IMAGEMANAGER->findImage("heartbeat")->frameRender(getMemDC(), _heartRc.left, _heartRc.top, _isHeartbeat, 0);
}

//ó���� ��Ŀ�� �����ش�
void heartbeat::removeMarker()
{
	if (!_vBeatMarker.empty())
	{
		_vBeatMarker.erase(_vBeatMarker.begin());
	}
}