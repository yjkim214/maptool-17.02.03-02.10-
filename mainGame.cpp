#include "stdafx.h"
#include "mainGame.h"
//=============================================================
//	## init(void) ## �ʱ�ȭ
//=============================================================
HRESULT mainGame::init(void)
{
	gameNode::init(TRUE);
	
	//������ ���ΰ��� Ŭ���� �ȿ����� ���鸸 �����Ѵ�
	//�׸��� ������ ������ ���ΰ��ӿ����� �ϸ� �ȴ�
	/*���߰�*/
	SCENEMANAGER->addScene("��Ʈ��ȭ��", new introScene);
	SCENEMANAGER->addScene("�ε�ȭ��", new loadingScene);
	SCENEMANAGER->addScene("�޴�ȭ��", new menuScene);
	SCENEMANAGER->addScene("����ȭ��", new maptoolScene);
	SCENEMANAGER->addScene("��ȭ��", new tileMap);
	/*����� ����*/
	SCENEMANAGER->changeScene("��Ʈ��ȭ��");

	return S_OK;
}
//=============================================================
//	## release(void) ## ����
//=============================================================
void mainGame::release(void)
{
	gameNode::release();
}
//=============================================================
//	## update(void) ## ������Ʈ
//=============================================================
void mainGame::update(void)
{
	gameNode::update();

	//���Ŵ��� ������Ʈ
	SCENEMANAGER->update();
}
//=============================================================
//	## render(void) ## ����
//=============================================================
void mainGame::render(void)
{
	//��� �� ��Ʈ�� (������ �׳� �״�� �Ѱ�!!)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
//=============================================================
	
	//���Ŵ��� ����
	SCENEMANAGER->render();

	//Ÿ�ӸŴ��� ����
	//TIMEMANAGER->render(getMemDC());

//=============================================================
	//������� ������ HDC�� �׸��� (������ �״�� �Ѱ�!!)
	this->getBackBuffer()->render(getHDC());
}