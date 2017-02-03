#pragma once
#include "gameNode.h"

//탱크맵에 접근 가능하도록 클래스 전방선언
class tankMap;

//탱크 4방향
enum TANKDIRECTION
{
	TANKDIRECTION_LEFT,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_UP,
	TANKDIRECTION_DOWN
};

class tank : public gameNode
{
private:
	tankMap* _tankMap;
	
	TANKDIRECTION _direction;	//탱크 방향
	image* _image;				//탱크 이미지
	RECT _rc;					//탱크 렉트
	float _x, _y;				//탱크 위치좌표
	float _speed;				//탱크 속도

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void tankMove(void);
	void setTankPosition(int index);
	
	
	//탱크맵 메모리 링크
	void setTankMapLink(tankMap* tm) { _tankMap = tm; }

	tank() {}
	~tank() {}
};

