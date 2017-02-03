#include "stdafx.h"
#include "menuScene.h"

HRESULT menuScene::init(void)
{
	//��ư���� �ʱ� x��
	_initPlayX = IMAGEMANAGER->findImage("menu_playbutton")->getX();
	_initMapX = IMAGEMANAGER->findImage("menu_mapbutton")->getX();
	_initQuitX = IMAGEMANAGER->findImage("menu_quitbutton")->getX();

	//�ʱ� x���� 20�� ���Ѱ�
	_changePlayX = _initPlayX + 20;
	_changeMapX = _initMapX + 20;
	_changeQuitX = _initQuitX + 20;

	//���İ��� ó���� 255 MAX
	_alphaPlay = 255;
	_alphaMap = 255;
	_alphaQuit = 255;

	SOUNDMANAGER->play("main_menu");

	return S_OK;
}

void menuScene::release(void)
{
}

void menuScene::update(void)
{
	//�÷��� ��ư �ȿ� ���콺�� �ִٸ�
	if (PtInRect(&IMAGEMANAGER->findImage("menu_playbutton")->boundingBox(), _ptMouse))
	{
		//�÷��� ��ư�� x���� ����
		IMAGEMANAGER->findImage("menu_playbutton")->setX(_changePlayX);
		//��ư �ȿ� ���콺�� �ְ� Ŭ���� �Ѵٸ� ���İ��� ����
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) { _alphaPlay = 150; }
		//��ư �ȿ� ���콺�� �ְ� ���콺�� ������ �´ٸ� ���� �������ش�
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON)){}
	}

	//�÷��� ��ư �ȿ� ���콺�� ���ٸ�
	else
	{
		//�÷��� ��ư�� ���� ��ġ��
		IMAGEMANAGER->findImage("menu_playbutton")->setX(_initPlayX);
	}

	//�� ��ư �ȿ� ���콺�� �ִٸ� 
	if (PtInRect(&IMAGEMANAGER->findImage("menu_mapbutton")->boundingBox(), _ptMouse))
	{
		//�� ��ư�� x���� ����
		IMAGEMANAGER->findImage("menu_mapbutton")->setX(_changeMapX);
		//���� ���콺�� �� ��ư �ȿ� �ְ� ���ʹ�ư�� ������ �ȴٸ� ���İ��� ����
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) { _alphaMap = 150; }
		//��ư�ȿ� ���콺�� �ְ� ���콺 ���� Ŭ���� ���ٸ� �� ��ȯ
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON)) {}
	}

	//�� ��ư �ȿ� ���콺�� ���ٸ�
	else
	{
		//�� ��ư�� ���� ��ġ�� ����
		IMAGEMANAGER->findImage("menu_mapbutton")->setX(_initMapX);
	}

	if (PtInRect(&IMAGEMANAGER->findImage("menu_quitbutton")->boundingBox(), _ptMouse))
	{
		IMAGEMANAGER->findImage("menu_quitbutton")->setX(_changeQuitX);
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) { _alphaQuit = 150; }
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON)) { PostQuitMessage(0); }
	}

	else
	{
		IMAGEMANAGER->findImage("menu_quitbutton")->setX(_initQuitX);
	}

	//���� �ٸ� ��ġ���� ���콺�� ���ٸ�
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		//���İ��� �ٽ� ������
		_alphaPlay = 255;
		_alphaMap = 255;
		_alphaQuit = 255;
	}
}

void menuScene::render(void)
{
	//���ķ����� ���� ��ġ�� ����´�.. �Ф�
	//��¿������ add�� �� ������� x, y ���� ���ؼ� �׷���
	IMAGEMANAGER->findImage("mainmenu")->render(getMemDC());
	IMAGEMANAGER->findImage("menu_playbutton")->alphaRender(getMemDC(), IMAGEMANAGER->findImage("menu_playbutton")->getX(), IMAGEMANAGER->findImage("menu_playbutton")->getY(), _alphaPlay);
	IMAGEMANAGER->findImage("menu_mapbutton")->alphaRender(getMemDC(), IMAGEMANAGER->findImage("menu_mapbutton")->getX(), IMAGEMANAGER->findImage("menu_mapbutton")->getY(), _alphaMap);
	IMAGEMANAGER->findImage("menu_quitbutton")->alphaRender(getMemDC(), IMAGEMANAGER->findImage("menu_quitbutton")->getX(), IMAGEMANAGER->findImage("menu_quitbutton")->getY(), _alphaQuit);
}