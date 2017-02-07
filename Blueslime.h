#pragma once
#include "cEnemy.h"
class Blueslime : public cEnemy
{
public:

	virtual HRESULT init(void);
	virtual HRESULT init(POINT pos);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void move();
	virtual void draw();
	virtual void animation();

	float getSpeed() { return _movespeed; }

	void setHp(int HP) { _hp = HP; }
	int getHp() { return _hp; }

	void setDmg(int Dmg) { _dmg = Dmg; }
	int getDmg() { return _dmg; }

	float getX() { return _x; }
	float getY() { return _y; }

	RECT getRC() { return _rc; }


	Blueslime() {}
	~Blueslime() {}
};

