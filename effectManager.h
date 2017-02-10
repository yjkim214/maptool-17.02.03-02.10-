#pragma once
#include "singletonBase.h"

//이펙트 구조체
struct tagEffect
{
	RECT rc;
	float x, y;
	float width, height;
	int currentFrameX;
	int count;

	image* effectImg;
};

class effectManager : public singletonBase<effectManager>
{
private:
	vector<tagEffect> _vEffect;

public:
	void addEffect(float x, float y, char* effectImageKey);
	void update();
	void render(HDC hdc);

	effectManager() {}
	~effectManager() {}
};