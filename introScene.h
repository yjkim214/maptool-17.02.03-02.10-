#pragma once
#include "gameNode.h"
class introScene : public gameNode
{
private:
	//��Ʈ�� ���ȭ�� ���İ�
	int _alpha;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	introScene() : _alpha(255) {}
	~introScene() {}
};