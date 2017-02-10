#include "stdafx.h"
#include "effectManager.h"

void effectManager::addEffect(float x, float y, char * effectImageKey)
{
	//이펙트를 추가한다
	tagEffect newEffect;
	newEffect.effectImg = IMAGEMANAGER->findImage(effectImageKey);
	//이미지가 한장이라 다시 프레임을 맞춰 주어야한다
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
	//이펙트의 사이즈 만큼 그린다
	for (int i = 0; i < _vEffect.size(); i++)
	{
		//각 이펙트의 카운트를 증가
		_vEffect[i].count++;
		//하나의 이펙트의 카운트가 5가 되었을 때
		if (_vEffect[i].count % 5 == 0)
		{
			//다음 장으로 넘겨준다
			_vEffect[i].currentFrameX++;
			//이때 다음 장이 없을 경우
			if (_vEffect[i].currentFrameX >= _vEffect[i].effectImg->getMaxFrameX())
			{
				//이펙트를 지워준다
				_vEffect.erase(_vEffect.begin() + i);
			}
			//다음 장이 있을 경우
			else
			{
				//카운트를 0으로 해서 다음 장을 그릴 시간을 초기화 해준다
				_vEffect[i].count = 0;
			}
		}
	}
}

void effectManager::render(HDC hdc)
{
	// 모든 이미지 그리기
	for (int i = 0; i < _vEffect.size(); i++)
	{
		_vEffect[i].effectImg->frameRender(hdc, _vEffect[i].x, _vEffect[i].y, _vEffect[i].currentFrameX, 0);
	}
}