#include "stdafx.h"
#include "sceneTest.h"


HRESULT sceneTest::init(void)
{
	return S_OK;
}

void sceneTest::release(void)
{
}

void sceneTest::update(void)
{
	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		SCENEMANAGER->changeScene("�ȼ��浹");
	}
}

void sceneTest::render(void)
{
	char str[128];
	sprintf(str, "���׽�Ʈ ����~~~~");
	TextOut(getMemDC(), 100, 100, str, strlen(str));
}
