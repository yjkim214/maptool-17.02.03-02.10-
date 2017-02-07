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

	//������
	void setPos(float x, float y);

	//������
	float getX() { return _x; }
	float getY() { return _y; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	RECT getRect() { return _rc; }

	drawRectManager() {}
	~drawRectManager() {}
};