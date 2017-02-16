#pragma once
#include "gameNode.h"
class potion : public gameNode
{
private:
	int _healPt;

	float _x, _y;
	RECT _rc;

	POINT _index;

	bool _isEquip;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void setPos();
	void setIndex(POINT index);
	void setIsEquip(bool isEquip);

	int getHealPt() { return _healPt; }
	POINT getIndex() { return _index; }
	bool getIsEquip() { return _isEquip; }

	potion() {}
	~potion() {}
};

