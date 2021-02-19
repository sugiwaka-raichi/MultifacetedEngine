#pragma once
#include <vector>
#include <map>
#include <string>
//#include <fstream>
#include "ERROR.h"
#include "OperationsStruct.h"

using namespace std;

//========================================================
//���߂̍\����
//========================================================
typedef struct {
	int lines = 0;				//n�s��
	int byte = 0;				//���߂̑��o�C�g��
	char op_code = 0x00;		//���ߓ��e
}ORDER_DATA;

//==============================================
//���x���Ǘ��p�\����
//==============================================
typedef struct {
	wstring labelName;		//���x����
	int	line;				//�s
	//wstring fileName;		//�t�@�C����(��)
}LABEL_DATA;

//==============================================
//�X�N���v�g���Ǘ�����N���X
//==============================================
class Script {
private:
	const int backLog = 10;			//10���o�b�N���O�Ƃ��ĕ\������
	static Script* instance;
	
	int fileLine = 0;
	int seek;						//�Ǎ���ł���ʒu
	map<wstring, int> label;		//���x���ʒu�̋L�^
	vector<wstring> dialogue;			//�Z���t�ꗗ(���݂̃Z���t+�o�b�N���O)
	wstring fileName = L"SCRIPT.ric";
	char lastOPCode;			//�Ō�̖��ߓ��e

	Script();

	wstring ReadString(ifstream* file, int _readByte);
public:
	~Script();
	static Script& GetScriptInstance();
	void LoadHeader();
	void LoadScript();
	void LoadScript(int _seek);
	wstring GetNowDialogue();
	char GetLastOperation();		//�Ō�Ɏ擾�������߂̊T�v��n���֐�
};

