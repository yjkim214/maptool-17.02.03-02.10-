#include "stdafx.h"
#include "effectManager.h"

void effectManager::addEffect(float x, float y, char * effectImageKey)
{
	//����Ʈ�� �߰��Ѵ�
	tagEffect newEffect;
	newEffect.effectImg = IMAGEMANAGER->findImage(effectImageKey);
	//�̹����� �����̶� �ٽ� �������� ���� �־���Ѵ�
	newEffect.currentFrameX = 0;
	newEffect.x = x;
	newEffect.y = y;
	newEffect.width = newEffect.effectImg->getFrameWidth();
	newEffect.height = newEffect.effectImg->getFrameHeight();
	newEffect.rc = RectMakeCenter(newEffect.x, newEffect.y, newEffect.width, newEffect.height);
	newEffect.count = 0;

	_vEffect.push_back(newEffect);
}

void effectManager::update()
{
	//����Ʈ�� ������ ��ŭ �׸���
	for (int i = 0; i < _vEffect.size(); i++)
	{
		//�� ����Ʈ�� ī��Ʈ�� ����
		_vEffect[i].count++;
		//�ϳ��� ����Ʈ�� ī��Ʈ�� 5�� �Ǿ��� ��
		if (_vEffect[i].count % 5 == 0)
		{
			//���� ������ �Ѱ��ش�
			_vEffect[i].currentFrameX++;
			//�̶� ���� ���� ���� ���
			if (_vEffect[i].currentFrameX >= _vEffect[i].effectImg->getMaxFrameX())
			{
				//����Ʈ�� �����ش�
				_vEffect.erase(_vEffect.begin() + i);
			}
			//���� ���� ���� ���
			else
			{
				//ī��Ʈ�� 0���� �ؼ� ���� ���� �׸� �ð��� �ʱ�ȭ ���ش�
				_vEffect[i].count = 0;
			}
		}
	}
}

void effectManager::render(HDC hdc)
{
	// ��� �̹��� �׸���
	for (int i = 0; i < _vEffect.size(); i++)
	{
		_vEffect[i].effectImg->frameRender(hdc, _vEffect[i].x, _vEffect[i].y, _vEffect[i].currentFrameX, 0);
	}
}