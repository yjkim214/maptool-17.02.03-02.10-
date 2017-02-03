#pragma once
#include "singletonBase.h"
//==================================================================
//		## sceneManager ## (�� �Ŵ���)
//==================================================================

//�̱����� ��ӹ��� ���̵��� static���� ����� ���⶧����
//�Ϲ�Ŭ������ ���ӳ�带 ����Ϸ��� �ϸ� ������

//���ӳ�� Ŭ���� ���漱��
class gameNode;

class sceneManager : public singletonBase <sceneManager>
{
private:
	typedef map<string, gameNode*> mSceneList;
	typedef map<string, gameNode*>::iterator miSceneList;

private:
	mSceneList _mSceneList;		//������ ��(ȭ��)���� ��Ƶ� ��
	gameNode* _currentScene;	//�����

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//�� �ʱ�ȭ �����ִ� ���
	//1. �����ڿ��� �ٷ� �ʱ�ȭ
	//2. ����̴ϼȶ���� ���ؼ� �ʱ�ȭ
	//3. �����Լ��� ���ؼ�

	//���߰�
	gameNode* addScene(string sceneName, gameNode* scene);

	//������
	HRESULT changeScene(string sceneName);

	sceneManager() : _currentScene(NULL) {}
	~sceneManager() {}
};

