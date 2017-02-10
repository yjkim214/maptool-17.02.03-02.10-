#include "stdafx.h"
#include "gameNode.h"

//==================================================================
//		## �ʱ�ȭ ## init(void)
//==================================================================
HRESULT gameNode::init(void)
{
	return S_OK;
}

HRESULT gameNode::init(bool managerInit)
{
	_hdc = GetDC(_hWnd);
	_managerInit = managerInit;
	
	if (managerInit)
	{
		//SetTimer(_hWnd, 1, 10, NULL);		//Ÿ�̸� �ʱ�ȭ
		KEYMANAGER->init();					//Ű�Ŵ��� �ʱ�ȭ
		RND->init();						//������� �ʱ�ȭ
		IMAGEMANAGER->init();				//�̹����Ŵ��� �ʱ�ȭ
		PLAYERDATA->init();					//�÷��̾���� �ʱ�ȭ
		TXTDATA->init();					//�ؽ�Ʈ������ �ʱ�ȭ
		SCENEMANAGER->init();				//���Ŵ��� �ʱ�ȭ
		INIDATA->init();					//INI������ �ʱ�ȭ
		TIMEMANAGER->init();				//Ÿ�ӸŴ��� �ʱ�ȭ
		SOUNDMANAGER->init();				//����Ŵ��� �ʱ�ȭ
		DRAWRECTMANAGER->init();
	}

	return S_OK;
}

//==================================================================
//		## ���� ## release(void)
//==================================================================
void gameNode::release(void)
{
	if (_managerInit)
	{
		//Ÿ�̸� ����
		//KillTimer(_hWnd, 1);
		//Ű�Ŵ��� �̱��� ����
		KEYMANAGER->releaseSingleton();
		//������� �̱��� ����
		RND->releaseSingleton();
		//�̹����Ŵ��� ����, �̱��� ����
		IMAGEMANAGER->release();
		IMAGEMANAGER->releaseSingleton();
		//�÷��̾���� �̱��� ����
		PLAYERDATA->releaseSingleton();
		//�ؽ�Ʈ������ �̱��� ����
		TXTDATA->releaseSingleton();
		//INI������ �̱��� ����
		INIDATA->releaseSingleton();
		//���Ŵ��� ����, �̱��� ����
		SCENEMANAGER->release();
		SCENEMANAGER->releaseSingleton();
		//Ÿ�ӸŴ��� �̱��� ����
		TIMEMANAGER->release();
		TIMEMANAGER->releaseSingleton();
		//����Ŵ��� ����, �̱��� ����
		SOUNDMANAGER->release();
		SOUNDMANAGER->releaseSingleton();
		//��ο췺Ʈ�Ŵ��� �̱��� ����
		DRAWRECTMANAGER->releaseSingleton();
		//����Ʈ�Ŵ��� �̱��� ����
		EFFECTMANAGER->releaseSingleton();
	}

	//DC ����
	ReleaseDC(_hWnd, _hdc);
}

//==================================================================
//		## ������Ʈ ## update(void)
//==================================================================
void gameNode::update(void)
{
	//���ΰ�ħ (���߿� ���� Ÿ�̸Ӹ� ���� �Ŀ��� ������� �ʴ´�)
	//������۸� ���� ������� �ʴ´� (true => false)
	//���߿� �׽�Ʈ �ϱ� (true / false or �������)
	//InvalidateRect(_hWnd, NULL, FALSE);
}
//==================================================================
//		## ���� ## render(void)
//==================================================================
void gameNode::render(void)
{
}

//==================================================================
//		## MainProc ## �������ν���
//==================================================================
LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_MOUSEMOVE:
		_ptMouse.x = LOWORD(lParam);
		_ptMouse.y = HIWORD(lParam);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}