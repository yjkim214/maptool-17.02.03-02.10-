#include "stdafx.h"
#include "pixelCollision.h"


HRESULT pixelCollision::init(void)
{
	//��׶��� �̹���
	_mountain = IMAGEMANAGER->addImage("mountain", "mountain.bmp", 800, 600, true, RGB(255, 0, 255));

	//�� �̹���
	_ball = IMAGEMANAGER->addImage("ball", "ball.bmp", 60, 60, true, RGB(255, 0, 255));

	//�� ��ġ �ʱ�ȭ
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());

	//y������ Ž��(���� �ϴܿ� �簢�� �����)
	_probeY = _y + _ball->getHeight() / 2;

	_rc1 = RectMakeCenter(_ptMouse.x, _ptMouse.y, 100, 100);

	return S_OK;
}

void pixelCollision::release(void)
{
}

void pixelCollision::update(void)
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_x -= 3.0f;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_x += 3.0f;
	}
	//���� ��Ʈ�� ����������
	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());
	//y������ Ž��(���� �ϴܿ� �簢�� �����)
	_probeY = _y + _ball->getHeight() / 2;

	/*�̺κ��� �ȼ��浹 �ٽ�*/
	//Ž���� Y���� ������ ���Ʒ��� ���ϴ� ��ŭ �˻縦 �Ѵ�
	for (int i = _probeY - 20; i < _probeY + 20; i++)
	{
		COLORREF color = GetPixel(_mountain->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = i - _ball->getHeight() / 2;
			break;
		}
	}


	_rc1 = RectMakeCenter(_ptMouse.x, _ptMouse.y, 100, 100);

	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		SCENEMANAGER->changeScene("���׽�Ʈ");
	}

}

void pixelCollision::render(void)
{
	//��׶��� ����
	//_mountain->render(getMemDC());
	IMAGEMANAGER->render("��׶���", getMemDC());

	//�� �̹��� ����
	_ball->render(getMemDC(), _rc.left, _rc.top);

	if (KEYMANAGER->isToggleKey('A'))
	{
		//���� �簢�� ����
		Rectangle(getMemDC(), _x, _probeY, _x + 10, _probeY + 10);
	}

	Rectangle(getMemDC(), _rc1.left, _rc1.top, _rc1.right, _rc1.bottom);

}
