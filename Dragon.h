#pragma once
#include "cEnemy.h"
#include<algorithm>
enum state
{
	MOVE,
	ATTCK

};
enum astarState
{
	NONE,
	OPEN,
	CLOSE,
	T_BEST
};

enum findState
{
	STANDARD,
	NO_ROUTE,
	FIND,
	BEST
};
struct tileState
{
	int F;
	int G;
	int H;
	astarState state;
};


class Dragon : public cEnemy
{
	tileState astar[30 * 30];
	vector<int> _openList;
	vector<int> _closeList;
	vector<int> _finalList;
	bool findRoute;
	findState _routeState;

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

	void initAstar();
	void searchRoute();
	void searchRouteV2();
	void findBest();
	void findBestV2();
	void deleteOpen(int key);
	int settingMove();
	int findAttribute(int input);
	int findMinimum();


	Dragon() {}
	~Dragon() {}
};