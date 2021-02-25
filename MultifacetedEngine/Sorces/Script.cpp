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
	int strSeek = 0;
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
	ORDER_DATA data;		//�Ǎ��񂾖��ߏ��
	ifstream file;			//�t�@�C��
	wstring str;			//�Ǎ��񂾕�����
	bool loadFlg = true;	//���[�h�t���O
	int strIndex = 0;		//���蕶���̈ʒu
	int readBytes = 0;		//1���߂̓Ǎ��ރo�C�g��
	int readLines = 0;		//�Ǎ���ł���s
	float tempValue = 0;	//�ꎞ���l�i�[�ϐ�

	file.open(fileName, ios::in | ios::binary);
	if (!file) {
		ErrorMessage::ErrorMessageBox(L"�X�N���v�g�t�@�C�����J���̂Ɏ��s���܂����B", L"Script FileOpen Error", MB_OK);
		return;
	}

	file.seekg(_seek);		//�Ǎ��ʒu��ݒ�
	while (loadFlg) {
		file.read((char*)&data.lines, sizeof(int));		//n�s�ڂ̖��߂�
		file.read((char*)&data.byte, sizeof(int));		//���߂̃o�C�g��
		file.read(&data.op_code, sizeof(char));			//���߂̊T�v
		readBytes = data.byte - 1;						//�Ǎ��ޓ��e�̃o�C�g��

		if (readLines == 0) {
			readLines = data.lines;				//�Ǎ���ł���s
		}
		else if(readLines != data.lines) {		//�ǂ�ł����s�ƈ�v���Ȃ��Ƃ����[�v���甲����
			seek = file.tellg();				//�Ǎ���ł���ʒu���擾
			seek -= sizeof(ORDER_DATA) * 8;		//�Ǎ��񂾏�񕪈ʒu��߂�
			break;
		}
		
		switch (data.op_code & 0x0F) {		//����4bit�Ŕ���
		case 0:		//0�͓ǂݍ��݃G���[�܂��͓ǂݍ��ݏI�����Ă���
			loadFlg = false;
			break;
		case OP_CODE::LABEL:
			str = ReadString(&file, readBytes);
			label[str] = file.tellg();		//���x���Ǘ�(�V�[�N�ʒu)
			loadFlg = false;
			break;
		case OP_CODE::TEXT:
			str = ReadString(&file, readBytes);		//������̓ǂݍ���
			strIndex = str.find(0x09);
			if (strIndex != wstring::npos) {		//TAB�ŉ��s
				str[strIndex] = L'\n';
			}
			if ((data.op_code & 0xF0) >> 4 != TEXT_TYPE::STRING) {
				if (backLog < dialogue.size()) {
					dialogue.erase(dialogue.begin());		//�擪�f�[�^������
				}
				dialogue.push_back(str);		//�Z���t�ꗗ�ɓ����
				loadFlg = false;
			}
			else {
				arg.push_back(str);		//�����Ƃ��Ĉ�����
			}
			break;
		case OP_CODE::FUNC:
			str = ReadString(&file, readBytes);		//�֐����̎擾
			switch (data.op_code & 0xF0) {
			case 0x00:		//������
				//�����Ȃ炻�̂܂܏����X�^�b�N�ɐς�
				func.push_back(str);
				break;
			case 0x10:		//�����L
				//�L��Ȃ�����̏I��������܂ŏ����҂�
				
				break;
			}
			break;
		case OP_CODE::ARG:
			switch (data.op_code & 0xF0)
			{
			case 0x00:			//�����̎n�܂�
				break;
			case 0x10:			//�����̏I���
				break;
			}
			break;
		case OP_CODE::VAR:
			//ToDo:LOCAL��GLOBAL������
			str = ReadString(&file, readBytes);		//�ϐ����̓ǂݍ���
			variable[str] = 0;		//�ϐ����̓o�^�Ə�����
			break;
		case OP_CODE::VAL:
			//���l�擾
			switch ((data.op_code & 0xF0) >> 4) {
			case VAL_TYPE::VAL_INT:
				file.read((char*)&tempValue, sizeof(int));
				break;
			case VAL_TYPE::VAL_FLOAT:
				file.read((char*)&tempValue, sizeof(float));
				break;
			}
			value.push_back(tempValue);
			break;
		case OP_CODE::OP:
			switch ((data.op_code & 0xF0) >> 4)	{
			case OPERATOR::ADD:
			case OPERATOR::SUBT:
			case OPERATOR::MUL:
			case OPERATOR::DIV:
				break;
			}
		}
		lastOPCode = data.op_code;		//
		str.clear();
	}
	seek = file.tellg();
	file.close();
	return;
}

wstring Script::GetNowDialogue()
{
	return dialogue.back();
}

int Script::GetSeek(wstring _label) {
	return label[_label];
}

char Script::GetLastOperation()
{
	return lastOPCode;
}

void Script::SetSeek(int _seek) {
	seek = _seek;
	return;
}
