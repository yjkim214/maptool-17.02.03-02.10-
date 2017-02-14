#pragma once
#include "cEnemy.h"
class Skeleton : public cEnemy
{
public:
	virtual HRESULT init(void);
	virtual HRESULT init(POINT index);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void move();
	virtual void draw();
	virtual void animation();

	void addhpbar(float x, float y);

	Skeleton() {}
	~Skeleton() {}
};

