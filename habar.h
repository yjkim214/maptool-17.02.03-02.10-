#pragma once
#include "gameNode.h"
class habar :public gameNode
{
private:
	RECT _rchpbar;
	float _x, _y;
	float _width;
	float _height;

	bool or ;
	bool currenthp;
	bool halfhp;
	image* _hpbarFront;
	image* _hpbarBack;
	image* _hpbarHalf;

public:
	HRESULT init(char* frontkey, char* backkey, float x, float y, float width, float height);
	HRESULT init(char* frontkey, char* halfkey, char* backkey, float x, float y, float width, float height);
	void release(void);
	void update(void);
	void render(void);

	// ü�¹� �����ϱ�
	void setGauge(int currentHp, int maxHp);

	//ü�¹� ��ġ ��ǥ ����
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	float getX() { return _x; }
	float getY() { return _y; }

	void setcurrent(bool set) { currenthp = set; }
	void sethalf(bool set) { halfhp = set; }

	//ü�¹� ��Ʈ ��������
	RECT getRect(void) { return _rchpbar; }

	habar() {}
	~habar() {}
};

