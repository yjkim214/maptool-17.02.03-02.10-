#include "stdafx.h"
#include "iniTestScene.h"

HRESULT iniTestScene::init(void)
{
	return S_OK;
}

void iniTestScene::release(void)
{
}

void iniTestScene::update(void)
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		//����, Ű, ���
		INIDATA->addData("����", "���", "99");
		INIDATA->addData("ȣ��", "�ڵ���", "101");
		INIDATA->addData("ȣ��", "������", "98");
		INIDATA->addData("����", "������", "88");
		INIDATA->addData("����", "�米��", "110");
		INIDATA->addData("����", "�˸Ӹ�", "7");
		INIDATA->addData("����", "�ַ�", "150");
		INIDATA->addData("����", "����", "38");
		INIDATA->addData("���", "���밨��", "120");
		INIDATA->addData("ȣ��", "�ڵ���", "150");
		//�����̸����� ���̺�
		INIDATA->saveINI("����3��");
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		int test = INIDATA->loadDataInteger("����3��", "ȣ��", "�ڵ���");
		cout << "ȣ���� �ڵ���: " << test << endl;
	}
}

void iniTestScene::render(void)
{
}
