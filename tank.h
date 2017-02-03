#pragma once
#include "gameNode.h"

//��ũ�ʿ� ���� �����ϵ��� Ŭ���� ���漱��
class tankMap;

//��ũ 4����
enum TANKDIRECTION
{
	TANKDIRECTION_LEFT,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_UP,
	TANKDIRECTION_DOWN
};

class tank : public gameNode
{
private:
	tankMap* _tankMap;
	
	TANKDIRECTION _direction;	//��ũ ����
	image* _image;				//��ũ �̹���
	RECT _rc;					//��ũ ��Ʈ
	float _x, _y;				//��ũ ��ġ��ǥ
	float _speed;				//��ũ �ӵ�

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void tankMove(void);
	void setTankPosition(int index);
	
	
	//��ũ�� �޸� ��ũ
	void setTankMapLink(tankMap* tm) { _tankMap = tm; }

	tank() {}
	~tank() {}
};

