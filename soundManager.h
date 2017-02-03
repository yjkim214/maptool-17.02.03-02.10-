#pragma once
#include "singletonBase.h"
/*FMOD�� ����ϱ� ����*/
#include "inc/fmod.hpp"
#pragma comment(lib, "lib/fmodex_vc.lib")
using namespace FMOD;
//==================================================================
//		## soundManager ## (���� �Ŵ���)
//==================================================================

//ä��, �������
#define EXTRACHANNELBUFFER 5
#define SOUNDBUFFER 20
#define TOTALSOUNDBUFFER (EXTRACHANNELBUFFER + SOUNDBUFFER)

class soundManager : public singletonBase <soundManager>
{
private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundIter;

private:
	System* _system;			//�ý��� Ŭ����
	Sound** _sound;				//���� Ŭ����
	Channel** _channel;			//ä�� Ŭ����

	arrSounds _mTotalSounds;	//�ʿ� ��Ƶ� �����

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//���� �߰�(Ű��, �����̸�, BGM?, ������?)
	void addSound(string keyName, string soundName, bool bgm = FALSE, bool loop = FALSE);

	//���� ��� (���� 0.0 ~ 1.0 Max)
	void play(string keyName, float volume = 1.0f);
	//���� ����
	void stop(string keyName);
	//���� �Ͻ�����
	void pause(string keyName);
	//���� �ٽ����
	void resume(string keyName);

	//�÷��� ���̳�?
	bool isPlaySound(string keyName);
	//�Ͻ����� ���̳�?
	bool isPauseSound(string keyName);

	soundManager() : _system(NULL), _sound(NULL), _channel(NULL) {}
	~soundManager() {}
};

