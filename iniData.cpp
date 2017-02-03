#include "stdafx.h"
#include "iniData.h"

HRESULT iniData::init(void)
{
	return S_OK;
}

void iniData::release(void)
{
}
//������ �߰��ϱ�(����, Ű, ���)
void iniData::addData(char * section, char * key, char * value)
{
	//ini������ ����ü ������ �ʱ�ȭ
	tagIniData data;
	data.section = section;
	data.key = key;
	data.value = value;

	//���Ϳ� ini������ ����ü ���
	_vIniData.push_back(data);
}

//���̺�(�����̸�)
void iniData::saveINI(char * fileName)
{
	char str[256];
	char dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	sprintf(str, "\\%s.ini", fileName);
	GetCurrentDirectory(256, dir);
	strcat(dir, str);

	for (int i = 0; i < _vIniData.size(); i++)
	{
		WritePrivateProfileString(_vIniData[i].section, _vIniData[i].key, _vIniData[i].value, dir);
	}
}
//���ڷ� ���� �ε��ϱ�(�����̸�, ����, Ű)
char * iniData::loadDataString(char * fileName, char * section, char * key)
{
	char str[256];
	char dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	sprintf(str, "\\%s.ini", fileName);
	GetCurrentDirectory(256, dir);
	strcat(dir, str);

	char data[64] = { NULL };
	GetPrivateProfileString(section, key, "", data, 64, dir);

	return data;
}

//������ ���� �ε��ϱ�(�����̸�, ����, Ű)
int iniData::loadDataInteger(char * fileName, char * section, char * key)
{
	char str[256];
	char dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	sprintf(str, "\\%s.ini", fileName);
	GetCurrentDirectory(256, dir);
	strcat(dir, str);

	return GetPrivateProfileInt(section, key, 0, dir);
}
