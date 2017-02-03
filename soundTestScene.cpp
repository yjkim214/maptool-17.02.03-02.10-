#include "stdafx.h"
#include "soundTestScene.h"

HRESULT soundTestScene::init(void)
{
	//���� �߰�
	SOUNDMANAGER->addSound("����1", "���.mp3");
	SOUNDMANAGER->addSound("����2", "���1.mp3");

	SOUNDMANAGER->play("����1");

	return S_OK;
}

void soundTestScene::release(void)
{
}

void soundTestScene::update(void)
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (SOUNDMANAGER->isPlaySound("����1"))
		{
			SOUNDMANAGER->stop("����1");
			SOUNDMANAGER->play("����2");
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		SOUNDMANAGER->pause("����2");
	}
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		SOUNDMANAGER->resume("����2");
	}
}

void soundTestScene::render(void)
{
}
