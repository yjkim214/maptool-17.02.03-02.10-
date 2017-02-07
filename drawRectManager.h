#pragma once
#include "singletonBase.h"
class drawRectManager : public singletonBase <drawRectManager>
{
private:
	float _x, _y;
	RECT _rc;

public:
	HRESULT init(void);
	bool isImgRender(image* img);
	bool isFrameImgRender(image* img);

	//접근자
	void setPos(float x, float y);

	//설정자
	float getX() { return _x; }
	float getY() { return _y; }
	RECT getRect() { return _rc; }

	drawRectManager() {}
	~drawRectManager() {}
};