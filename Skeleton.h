#pragma once
#include "cEnemy.h"
#include<algorithm>

class Skeleton : public cEnemy
{
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
private:
	tileState astar[30 * 30];
	vector<int> _openList;
	vector<int> _closeList;
	vector<int> _finalList;
	bool findRoute;
	findState _routeState;
public:
	virtual HRESULT init(void);
	virtual HRESULT init(POINT index);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void move();
	virtual void draw();
	virtual void animation();
	void initAstar();
	void searchRoute();
	void searchRouteV2();
	void findBest();
	void findBestV2();
	void deleteOpen(int key);
	int settingMove();
	int findAttribute(int input);
	int findMinimum();
	void addhpbar(float x, float y);

	Skeleton() {}
	~Skeleton() {}
};

