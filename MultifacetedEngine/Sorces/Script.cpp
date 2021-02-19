#include "Script.h"
#include <fstream>

Script* Script::instance = nullptr;

Script::Script() {
	
}

Script::~Script()
{
}

//==================================================================
//������̓ǂݍ���
//ifstream:�t�@�C���|�C���^
//int:�Ǎ��ރo�C�g��
//==================================================================
wstring Script::ReadString(ifstream * _file, int _readByte)
{
	wstring str;
	//�Ǎ��ރo�C�g�� / �Ǎ��ތ^�̃T�C�Y�����[�v
	for (int i = 0; i < _readByte / sizeof(wchar_t); i++) {
		wchar_t tempData = '\0';		//null�����ŏ�����
		_file->read((char*)&tempData, sizeof(wchar_t));		//�t�@�C���ǂݍ���
		str.push_back(tempData);		//�Ǎ��񂾕����𕶎���^�ɒǉ�
	}
	return str;		//�Ǎ��ݏI�����f�[�^��n��
}

Script & Script::GetScriptInstance()
{
	if (instance == nullptr) {
		instance = new Script;
	}
	return *instance;
}

void Script::LoadHeader() {
	wstring str;			//�ꎞ�f�[�^
	ifstream file;			//�t�@�C��
	
	//file.open(fileName, ios::in | ios::binary);
	file.open("SCRIPT.ric", ios::in | ios::binary);
	if (!file) {
		ErrorMessage::ErrorMessageBox(L"�X�N���v�g�t�@�C�����J���̂Ɏ��s���܂����B", L"Script FileOpen Error", MB_OK);
		return;
	}

	file.read((char*)&fileLine, sizeof(int));

	seek = file.tellg();
	file.close();
	return;
}

void Script::LoadScript() {
	//ORDER_DATA data;
	//ifstream file;
	//int fileLine = 0;

	LoadScript(seek);		//���݂̃|�C���g��Ǎ���
}

void Script::LoadScript(int _seek) {
	ORDER_DATA data;
	ifstream file;
	wstring str;
	
	//file.open(fileName, ios::in | ios::binary);
	file.open("SCRIPT.ric", ios::in | ios::binary);
	if (!file) {
		ErrorMessage::ErrorMessageBox(L"�X�N���v�g�t�@�C�����J���̂Ɏ��s���܂����B", L"Script FileOpen Error", MB_OK);
		return;
	}

	int readBytes = 0;
	int tempValue = 0;
	file.seekg(_seek);
	file.read((char*)&data.lines, sizeof(int));		//n�s�ڂ̖��߂�
	file.read((char*)&data.byte, sizeof(int));		//���߂̃o�C�g��
	file.read(&data.op_code, sizeof(char));			//���߂̊T�v
	readBytes = data.byte - 1;						//�Ǎ��ޓ��e�̃o�C�g��

	switch (data.op_code & 0x0F) {		//����4bit�Ŕ���
	case OP_CODE::LABEL:
		str = ReadString(&file, readBytes);
		label[str] = data.lines;		//���x���Ǘ�
		break;
	case OP_CODE::TEXT:
		str = ReadString(&file, readBytes);
		if (backLog < dialogue.size()) {
			dialogue.erase(dialogue.begin());		//�擪�f�[�^������
		}
		dialogue.push_back(str);		//�Z���t�ꗗ�ɓ����
		break;
	case OP_CODE::FUNC:
		break;
	case OP_CODE::ARG:
		break;
	}

	str.clear();
	seek = file.tellg();
	lastOPCode = data.op_code;		//
	file.close();
	return;
}

wstring Script::GetNowDialogue()
{
	return dialogue.back();
}

char Script::GetLastOperation()
{
	return lastOPCode;
}
