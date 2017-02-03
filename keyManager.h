#pragma once
#include "singletonBase.h"
//==================================================================
//		## keyManager ## (Ű �Ŵ���)
//==================================================================

#define KEYMAX 256

class keyManager : public singletonBase <keyManager>
{
private:
	BOOL _keyUp[KEYMAX];
	BOOL _keyDown[KEYMAX];

public:
	//Ű�Ŵ��� �ʱ�ȭ
	HRESULT init();

	//Ű�� �ѹ��� ���ȳ�?
	bool isOnceKeyDown(int key);
	//Ű�� �ѹ� ������ �����?
	bool isOnceKeyUp(int key);
	//Ű�� ��� ���� �ֳ�?
	bool isStayKeyDown(int key);
	//���Ű��?
	bool isToggleKey(int key);

	keyManager() {}
	~keyManager() {}
};