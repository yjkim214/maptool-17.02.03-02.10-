#include "stdafx.h"
#include "introScene.h"

HRESULT introScene::init(void)
{
	//��Ʈ�δ� �ε� ���̱� ������ ���⼭ �̹����� �߰��Ѵ�
	IMAGEMANAGER->addImage("blackscreen", "blackscreen.bmp", WINSIZEX, WINSIZEY);
	IMAGEMANAGER->addImage("splash_screen", "splash_screen.bmp", WINSIZEX, WINSIZEY);

	_isStart = false;

	return S_OK;
}

void introScene::release(void)
{
}

void introScene::update(void)
{
	//���İ��� 0���� ũ�� ��� �ٿ��ش�
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		if (!_isStart)
		{
			_isStart = true;
		}
	}

	if (_isStart)
	{
		if (_alpha > 0) { _alpha -= 5; }
		//���İ��� 0���� �۾����ٸ� �ε�ȭ������ �Ѿ��
		else { SCENEMANAGER->changeScene("�ε�ȭ��"); }
	}
}

void introScene::render(void)
{
	//���� ȭ��� ��Ʈ�� ����� �׷��ش�
	IMAGEMANAGER->findImage("blackscreen")->render(getMemDC());
	IMAGEMANAGER->findImage("splash_screen")->alphaRender(getMemDC(), _alpha);
}