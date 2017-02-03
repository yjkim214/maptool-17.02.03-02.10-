#pragma once
#include "singletonBase.h"
//=============================================================
//	## iniData ## (INI ������)
//=============================================================

//ini������ ����ü
struct tagIniData
{
	char* section;
	char* key;
	char* value;
};

class iniData : public singletonBase <iniData>
{
private:
	vector<tagIniData> _vIniData;

public:
	HRESULT init(void);
	void release(void);

	//������ �߰��ϱ�(����, Ű, ���)
	void addData(char* section, char* key, char* value);
	//���̺�(�����̸�)
	void saveINI(char* fileName);

	//���ڷ� ���� �ε��ϱ�(�����̸�, ����, Ű)
	char* loadDataString(char* fileName, char* section, char* key);
	//������ ���� �ε��ϱ�(�����̸�, ����, Ű)
	int loadDataInteger(char* fileName, char* section, char* key);

	iniData() {}
	~iniData() {}
};

