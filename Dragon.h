#pragma once
#include "cEnemy.h"
enum state
{
	MOVE,
	ATTCK

};
class Dragon : public cEnemy
{
private:
	state _st;
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
	void attack();
	Dragon() {}
	~Dragon() {}
};