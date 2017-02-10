#pragma once
#include "gameNode.h"

struct BEAT_MARKER
{
	RECT markerRc[2];
	float markerSpeed;
	image* markerImg;
};

class heartbeat : public gameNode
{
private:
	//��Ʈ�� ī��Ʈ�Ǵ� �������� ��Ƴ��� ����
	vector<int> _vBeatTimer;
	//��Ʈ�� üũ �ڽ�
	vector<BEAT_MARKER> _vBeatMarker;

	//��Ʈ ī��Ʈ
	int _beatCount;
	//��Ʈ Ÿ�̸�
	int _beatTimer;

	//��Ŀ�� ���� �� �ִ°�?
	bool _isMade;
	//��Ʈ �����Ѱ�
	bool _isBeat;

	//������ ��Ʈ
	RECT _heartRc;
	
	//��Ʈ �ڽ��� üũ�Ǵ� ��Ʈ
	RECT _beatRc;
	int _heartbeatTimer;
	bool _isHeartbeat;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void removeMarker();
	bool getIsBeat() { return _isBeat; }

	heartbeat() {}
	~heartbeat() {}
};