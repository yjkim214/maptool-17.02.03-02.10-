#pragma once
#include "gameNode.h"
#include "minion.h"
#include "bullet.h"

//��ȣ������ �÷��̾� Ŭ���� ���漱��
class player;

class enemyManager : public gameNode
{
private:
	vector<enemy*> _vMinion;
	vector<enemy*>::iterator _viMinion;

	bullet* _bullet;
	int _count;

	player* _player;	//�����Ҵ� �ϸ� �ȵȴ�!!!

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//�̴Ͼ� ���� => �ʱ�ȭ�� �־��ٰ�
	void setMinion(void);

	//�̴Ͼ� �Ѿ˹߻�
	void minionBulletFire(void);

	//�̴Ͼ� ����
	void removeMinion(int index);

	//�浹�Լ�
	void collision();

	//�̴Ͼ� ���� ��������
	vector<enemy*> getVMinion() { return _vMinion; }

	//��ȣ������ �÷��̾������͸� �ѱ� �����Լ�
	void setPlayerLink(player* player) { _player = player; }

	enemyManager() {}
	~enemyManager() {}
};

