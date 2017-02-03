#pragma once
#include "gameNode.h"
#include "bullet.h"			//�̻���Ŭ������ ����ϱ� ����
#include "progressBar.h"	//ü�¹� Ŭ���� ����ϱ� ����

//���Ŵ��� ��ȣ�����ϱ� ���� Ŭ���� ���漱��
class enemyManager;

class player : public gameNode
{
private:
	image* _rocket;			//����(�÷��̾�) �̹���
	missile* _missile;		//�̻��� Ŭ����
	missileM1* _missileM1;	//�̻���M1 Ŭ����

	enemyManager* _em;		//�����Ҵ� �ϸ� �ȵȴ�!!!

	progressBar* _hpBar;	//ü�¹�
	float _maxHp, _currentHp;//�ִ�ü��, ����ü��	

	//int _x, _y;			//��ġ ��ǥ
	//RECT _rc;				//�÷��̾� ��Ʈ
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//�÷��̾� ����
	//void move();

	//�̻���M1 ����
	void removeMissileM1(int index);

	//�浹�Լ�(�÷��̾��� �Ѿ�Ŭ������ �Ѿ˺��Ͱ� ���Ŵ����� �̴Ͼ���)
	void collision();

	//ü�¹� ������
	void hitDamage(float damage);

	//�̻���M1 ��������
	missileM1* getMissileM1() { return _missileM1; }
	image* getPlayerImage() { return _rocket; }

	//��ȣ������ ���Ŵ��������͸� �ѱ� �����Լ�
	void setEMLink(enemyManager* em) { _em = em; }

	player() {}
	~player() {}
};

