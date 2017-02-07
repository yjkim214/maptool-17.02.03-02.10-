#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init(void)
{
	//�ε�Ŭ���� ���� �� �ʱ�ȭ
	_loading = new loading;
	_loading->init();

	//�̹��� �� ���� �ε�
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release(void)
{
	//�ε�Ŭ���� ����
	SAFE_DELETE(_loading);
}

void loadingScene::update(void)
{
	//�ε�Ŭ���� ������Ʈ
	_loading->update();

	//�ε��� �����ٸ�
	if (_loading->loadingDone())
	{
		//ESCŰ�� ������
		if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE)) 
		{
			//���μ��� ����
			PostQuitMessage(0); 
		}

		else if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			//ESC Ű�� ������ Ű�� ������ �� ��ȯ
			SCENEMANAGER->changeScene("�޴�ȭ��");
		}
	}
}

void loadingScene::render(void)
{
	//�ε�Ŭ���� ����
	_loading->render();
	//�ε��� ��� �Ϸ�Ǹ� 
	if (_loading->loadingDone()) { IMAGEMANAGER->render("guiLoadingScene", getMemDC(), 217.5f, 390); }
}

//�ε��̹��� �Լ�(�̰��� �̹����� ���� �־��)
void loadingScene::loadingImage(void)
{
	//_loading->loadImage();
	//_loading->loadFrameImage();

	//�ε��� �ʹ� ������ ����Ǽ� õõ�� ���ư����� �׽�Ʈ������ ����� ����
	for (int i = 0; i < 100; i++)
	{
		_loading->loadImage("�׽�Ʈ", WINSIZEX, WINSIZEY);
	}

	//�ε�ȭ�� �̹���
	_loading->loadImage("guiLoadingScene", "guiLoadingScene.bmp", 365, 30, true, RGB(255, 0, 255));

	//�޴�ȭ�� �̹���
	_loading->loadImage("mainmenu", "mainmenu.bmp", WINSIZEX, WINSIZEY);
	_loading->loadImage("menu_playbutton", "menu_playbutton.bmp", -85, 240, 301, 34, true, RGB(255, 0, 255));
	_loading->loadImage("menu_mapbutton", "menu_mapbutton.bmp", -85, 300, 301, 34, true, RGB(255, 0, 255));
	_loading->loadImage("menu_quitbutton", "menu_quitbutton.bmp", -85, 360, 301, 34, true, RGB(255, 0, 255));
	_loading->loadSound("main_menu", "main_menu.ogg", true, true);

	//��׶��� �̹���
	_loading->loadImage("��׶���", "background.bmp", WINSIZEX, WINSIZEY);
	
	//����ȭ��
	_loading->loadFrameImage("tileMapBase", "tileMapBase.bmp", 168, 160, 7, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("tileMapBaseBig", "tileMapBaseBig.bmp", 336, 320, 7, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("stairs", "stairs.bmp", 48, 24, 2, 1);

	//����ȭ��
	_loading->loadFrameImage("player", "player.bmp", 384, 480, 8, 10, true, RGB(255, 0, 255));
	_loading->loadFrameImage("player_jump", "player_jump.bmp", 120, 480, 5, 10, true, RGB(255, 0, 255));
}
//�ε����� �Լ�(�̰��� ���带 ���� �־��)
void loadingScene::loadingSound(void)
{
}