#pragma once
#include "gameNode.h"

enum WEAPON_KIND
{
	WEAPON_DAGGER,
	WEAPON_LONGSWORD,
	WEAPON_BROADSWORD,
	WEAPON_DAGGER_TITANIUM,
	WEAPON_LONGSWORD_TITANIUM,
	WEAPON_BROADSWORD_TITANIUM,
};

class weapon : public gameNode
{
private:
	WEAPON_KIND _kind;

	int _att;
	
	float _x, _y;
	RECT _rc;

	POINT _index;

	bool _isEquip;

public:
	HRESULT init(WEAPON_KIND kind);
	void release(void);
	void update(void);
	void render(void);

	void setKind(WEAPON_KIND kind) { _kind = kind; }
	void setPos();
	void setIndex(POINT index);
	void setIsEquip(bool isEquip);
	void setAtt(float att) { _att = att; }

	WEAPON_KIND getKind() { return _kind; }
	int getAtt() { return _att; }
	POINT getIndex() { return _index; }
	bool getIsEquip() { return _isEquip; }

	weapon() {}
	~weapon() {}
};