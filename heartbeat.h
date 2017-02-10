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
	//비트의 카운트되는 구간들을 모아놓은 벡터
	vector<int> _vBeatTimer;
	//비트의 체크 박스
	vector<BEAT_MARKER> _vBeatMarker;

	//비트 카운트
	int _beatCount;
	//비트 타이머
	int _beatTimer;

	//마커를 만들 수 있는가?
	bool _isMade;
	//비트 가능한가
	bool _isBeat;

	//심장의 렉트
	RECT _heartRc;
	
	//비트 박스가 체크되는 렉트
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