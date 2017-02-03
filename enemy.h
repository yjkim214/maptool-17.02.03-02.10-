#pragma once
#include "gameNode.h"

//�θ�Ŭ���� => �̳��� ��ӹ޾Ƽ� ����, �Ϲݸ��͵��� �����
class enemy : public gameNode
{
private:
	RECT _rc;			//�浹�� ��Ʈ
	image* _image;		//���ʹ� �̹���

	//������ �̹����� ������ ���� ������
	int _count;
	int _currentFrameX;
	int _currentFrameY;

	//�������� �Ѿ� �߻� ��Ÿ���ֱ�
	int _fireCount;
	int _rndFireCount;

public:
	virtual HRESULT init(void);
	virtual HRESULT init(char* imageName, POINT position);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void move();			//������Ʈ�� �ֱ�
	virtual void draw();			//���� �ȿ� �ֱ�
	virtual void animation();		//������Ʈ�� �ֱ�

	bool bulletCountFire(void);

	RECT getRect() { return _rc; }

	enemy() {}
	virtual ~enemy() {}
};

