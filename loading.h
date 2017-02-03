#pragma once
#include "gameNode.h"
#include "progressBar.h"
//=============================================================
//	## loadItem ## (�ε������ Ŭ����)
//=============================================================

//�ε������ ����
enum LOAD_KIND
{
	LOAD_KIND_IMAGE_0,		//���Ʈ�� �̹���
	LOAD_KIND_IMAGE_1,		//�Ϲ� �̹���
	LOAD_KIND_IMAGE_2,		//�Ϲ� �̹���(��ġ��ǥ)
	LOAD_KIND_FRAMEIMAGE_0,	//������ �̹���
	LOAD_KIND_FRAMEIMAGE_1, //������ �̹���(��ġ��ǥ)
	LOAD_KIND_SOUND,		//����
	LOAD_KIND_END
};

//�̹��� ���ҽ� ����ü
struct tagImageResource
{
	string keyName;			//�̹��� Ű��
	const char* fileName;	//�����̸�
	int x, y;				//��ġ��ǥ
	int width, height;		//����,����ũ��
	int frameX, frameY;		//������ x, y
	bool trans;				//������ﲨ��?
	COLORREF transColor;	//�������?
};

//���� ���ҽ� ����ü
struct tagSoundResource
{
	string keyName;
	string soundName;
	bool bgm;
	bool loop;
};

class loadItem
{
private:
	LOAD_KIND _kind;
	tagImageResource _imageResource;
	tagSoundResource _soundResource;
public:
	//�� ��Ʈ������ �ʱ�ȭ
	HRESULT init(string keyName, int width, int height);
	//�̹��� ���Ϸ� �ʱ�ȭ(�� ���)
	HRESULT init(string keyName, const char* fileName, int width, int height, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	HRESULT init(string keyName, const char* fileName, float x, float y, int width, int height, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	//������ �̹��� ���Ϸ� �ʱ�ȭ(�� ���)
	HRESULT init(string keyName, const char* fileName, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	HRESULT init(string keyName, const char* fileName, float x, float y, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	//���� �ʱ�ȭ
	HRESULT init(string keyName, string soundName, bool bgm = FALSE, bool loop = FALSE);

	//�ε������� ���� ��������
	LOAD_KIND getLoadingKind(void) { return _kind; }
	//�̹��� ���ҽ� ��������
	tagImageResource getImageResource(void) { return _imageResource; }
	//���� ���ҽ� ��������
	tagSoundResource getSoundResource() { return _soundResource; }

	loadItem() {}
	~loadItem() {}
};

//=============================================================
//	## loading ## (�ε� Ŭ����)
//=============================================================
class loading : public gameNode
{
private:
	//�ε������ Ŭ������ ���� ���� �� �ݺ���
	typedef vector<loadItem*> arrLoadItem;
	typedef vector<loadItem*>::iterator arrLoadItemIter;

private:
	arrLoadItem _vLoadItem;

	//�̰��� �ε�ȭ�鿡�� ����� �̹����� �ε��� �����Ұ�!!
	//�ε�ȭ�鿡�� ����� �̹��� �� �ε���
	image* _background;
	progressBar* _loadingBar;
	int _currentGauge;

	int _currentFrameX;
	int _count;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//�� ��Ʈ������ �ʱ�ȭ
	void loadImage(string strKey, int width, int height);
	//�̹��� ���Ϸ� �ʱ�ȭ(�� ���)
	void loadImage(string strKey, const char* fileName, int width, int height, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	void loadImage(string strKey, const char* fileName, float x, float y, int width, int height, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	//������ �̹��� ���Ϸ� �ʱ�ȭ(�� ���)
	void loadFrameImage(string strKey, const char* fileName, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	void loadFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	//���� ���� �ʱ�ȭ
	void loadSound(string keyName, string soundName, bool bgm = FALSE, bool loop = FALSE);

	/*�߿���*/
	//�ε��Ϸ� �Ƴ�?(�ε��Ϸ��� ȭ����ȯ)
	BOOL loadingDone(void);

	//�ε������Ŭ������ ���� ���� ��������
	//vector<loadItem*> getLoadItem(void)
	arrLoadItem getLoadItem(void) { return _vLoadItem; }

	loading() {}
	~loading() {}
};