#include "stdafx.h"
#include "loading.h"
//=============================================================
//	## loadItem ## (로드아이템 클래스)
//=============================================================
HRESULT loadItem::init(string keyName, int width, int height)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_0;
	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.width = width;
	_imageResource.height = height;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, int width, int height, BOOL isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_1;
	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, float x, float y, int width, int height, BOOL isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_2;
	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_FRAMEIMAGE_0;
	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.frameX = maxFrameX;
	_imageResource.frameY = maxFrameY;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, float x, float y, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_FRAMEIMAGE_1;
	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.frameX = maxFrameX;
	_imageResource.frameY = maxFrameY;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string keyName, string soundName, bool bgm, bool loop)
{
	_kind = LOAD_KIND_SOUND;

	_soundResource.keyName = keyName;
	_soundResource.soundName = soundName;
	_soundResource.bgm = bgm;
	_soundResource.loop = loop;

	return S_OK;
}


//=============================================================
//	## loading ## (로딩 클래스)
//=============================================================

HRESULT loading::init(void)
{
	//로딩화면 백그라운드 이미지 초기화
	_background = IMAGEMANAGER->addImage("bgLoadingScene", "bgLoadingScene.bmp", WINSIZEX, WINSIZEY);
	//로딩바 초기화
	_loadingBar = new progressBar;
	_loadingBar->init("loadingBarFront", "loadingBarBack", 5, 430, WINSIZEX - 10, 15);
	_loadingBar->setGauge(0, 0);
	//현재 게이지 초기화
	_currentGauge = 0;

	IMAGEMANAGER->addFrameImage("loading_skeleton", "loading_skeleton.bmp", 192, 25, 8, 1, true, RGB(255, 0, 255));
	_currentFrameX = 0;
	_count = 0;

	return S_OK;
}

void loading::release(void)
{
	//로딩바 해제
	SAFE_DELETE(_loadingBar);
}

void loading::update(void)
{
	//로딩바 업데이트
	_loadingBar->update();
	_count++;
	if (_count % 10 == 0)
	{
		_currentFrameX++;
		if (_currentFrameX > IMAGEMANAGER->findImage("loading_skeleton")->getMaxFrameX())
		{
			_currentFrameX = 0;
		}
		_count = 0;
	}
}

void loading::render(void)
{
	//백그라운드 렌더
	_background->render(getMemDC());

	//로딩바 렌더
	_loadingBar->render();

	IMAGEMANAGER->frameRender("loading_skeleton", getMemDC(), _loadingBar->getWidth() - 12, _loadingBar->getRect().top - 8, _currentFrameX, 0);
}

void loading::loadImage(string strKey, int width, int height)
{
	loadItem* item = new loadItem;
	item->init(strKey, width, height);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string strKey, const char * fileName, int width, int height, BOOL isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, width, height, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string strKey, const char * fileName, float x, float y, int width, int height, BOOL isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, x, y, width, height, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string strKey, const char * fileName, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, width, height, maxFrameX, maxFrameY, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string strKey, const char * fileName, float x, float y, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, x, y, width, height, maxFrameX, maxFrameY, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadSound(string keyName, string soundName, bool bgm, bool loop)
{
	loadItem* item = new loadItem;
	item->init(keyName, soundName, bgm, loop);
	_vLoadItem.push_back(item);
}

BOOL loading::loadingDone(void)
{
	//로딩이 완료됨
	if (_currentGauge >= _vLoadItem.size())
	{
		return TRUE;
	}

	loadItem* item = _vLoadItem[_currentGauge];

	switch (item->getLoadingKind())
	{
	case LOAD_KIND_IMAGE_0:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->addImage(img.keyName, img.width, img.height);
	}
	break;
	
	case LOAD_KIND_IMAGE_1:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->addImage(img.keyName, img.fileName, img.width, img.height, img.trans, img.transColor);
	}
	break;

	case LOAD_KIND_IMAGE_2:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->addImage(img.keyName, img.fileName, img.x, img.y, img.width, img.height, img.trans, img.transColor);
	}
	break;

	case LOAD_KIND_FRAMEIMAGE_0:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->addFrameImage(img.keyName, img.fileName, img.width, img.height, img.frameX, img.frameY, img.trans, img.transColor);
	}
	break;

	case LOAD_KIND_FRAMEIMAGE_1:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->addFrameImage(img.keyName, img.fileName, img.x, img.y, img.width, img.height, img.frameX, img.frameY, img.trans, img.transColor);
	}
	break;
	//나중에 사운드 배우고 난후 알아서...
	case LOAD_KIND_SOUND:
	{
		tagSoundResource sound = item->getSoundResource();
		SOUNDMANAGER->addSound(sound.keyName, sound.soundName, sound.bgm, sound.loop);
	}
	break;
	}

	//현재게이지 증가
	_currentGauge++;

	//로딩바 이미지 변경
	_loadingBar->setGauge(_currentGauge, _vLoadItem.size());

	return FALSE;
}
