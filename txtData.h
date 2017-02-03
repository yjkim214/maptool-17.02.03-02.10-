#pragma once
#include "singletonBase.h"
//=============================================================
//	## txtData ## (�ؽ�Ʈ ������)
//=============================================================

class txtData : public singletonBase <txtData>
{
public:
	HRESULT init(void);
	void release(void);

	//���̺�
	void txtSave(char* fileName, vector<string> vStr);
	char* vectorArrayCombine(vector<string> vArray);

	//�ε�
	vector<string> txtLoad(char* fileName);
	vector<string> charArraySeparation(char charArray[]);

	txtData() {}
	~txtData() {}
};

