#pragma once
#include "gameNode.h"
class habar :public gameNode
{
private:
	RECT _rchpbar;
	float _x, _y;
	float _width;
	float _height;

	bool currenthp;

	image* _hpbarFront;
	image* _hpbarBack;

public:
	HRESULT init(char* frontkey, char* backkey, float x, float y, float width, float height);
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
	//ü�¹� ��Ʈ ��������
	RECT getRect(void) { return _rchpbar; }

	habar() {}
	~habar() {}
};

