#pragma once
//=============================================================
//	## using namespace MY_UTIL ##
//=============================================================

#define PI 3.141592654f
#define PI2 (PI * 2)

//���߿� �̹��� ȸ���� ����� ��
#define PI_2 (PI / 2)
#define PI_4 (PI / 4)
#define PI_8 (PI / 8)

namespace MY_UTIL //�̸��� ���ϴ� �̸����� �����ص� �ȴ�
{
	//���� ������ �Ÿ�
	float getDistance(float startX, float startY, float endX, float endY);
	//���� ������ ����
	float getAngle(float startX, float startY, float endX, float endY);
};

