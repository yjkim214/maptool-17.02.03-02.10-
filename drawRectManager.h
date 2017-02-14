#pragma once
#include "singletonBase.h"

#define SHAKETIMER 5

class drawRectManager : public singletonBase <drawRectManager>
{
private:
	float _x, _y;
	RECT _rc;

	float _oldx, _oldy;
	bool _isShake;
	int _count;
public:
	HRESULT init(void);
	void update(void);
	bool isImgRender(image* img);
	bool isFrameImgRender(image* img);
	//접근자
	void setPos(float x, float y);

	//설정자
	float getX() { return _x; }
	float getY() { return _y; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	RECT getRect() { return _rc; }

	void shakeWindow();

	drawRectManager() {}
	~drawRectManager() {}
};