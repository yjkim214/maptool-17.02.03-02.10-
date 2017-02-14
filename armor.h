#pragma once
#include "gameNode.h"

enum ARMOR_KIND
{
	ARMOR_TUNIC, 
	ARMOR_LEATHER, 
	ARMOR_CHAINMAIL, 
	ARMOR_PLATE, 
	ARMOR_HEAVY, 
	ARMOR_KARATE, 
	ARMOR_OBSIDIAN01, 
	ARMOR_OBSIDIAN02, 
	ARMOR_OBSIDIAN03, 
	ARMOR_GLASS, 
};

class armor : public gameNode
{
private:
	ARMOR_KIND _kind;

	int _hp;

	float _x, _y;
	RECT _rc;

	POINT _index;

	bool _isEquip;

public:
	HRESULT init(ARMOR_KIND kind);
	void release(void);
	void update(void);
	void render(void);

	void setPos();
	void setIndex(POINT index);
	void setIsEquip(bool isEquip);

	ARMOR_KIND getKind() { return _kind; }
	int getHp() { return _hp; }
	POINT getIndex() { return _index; }
	bool getIsEquip() { return _isEquip; }
	RECT getRect() { return _rc; }

	armor() {}
	~armor() {}
};