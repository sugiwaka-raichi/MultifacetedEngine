#pragma once
#include "PARSE.h"

//------------------------------------
//���߈ꗗ
//------------------------------------
enum OP_CODE {
	//PUSH,		//�v�b�V��
	OP = 0x01,			//���Z�q
	VAL,		//���l
	VAR,		//�ϐ�
	LABEL,		//���x��
	ARG,		//����
	TEXT,		//�e�L�X�g
	FUNC,		//�֐����s
};

//---------------------------
//�ڍׂȉ��Z�q
//---------------------------
enum OPERATOR {
	NONE = 0x00,	//���Z�q�Ƃ��Ă̌��ʂȂ�		
	//-----------------------
	//�Z�p���Z�q
	ADD = 0x01,		//���Z
	SUBT,			//���Z
	MUL,			//��Z
	DIV,			//���Z
	SUBS,			//���
	//-----------------------
	//��r���Z�q
	EQUAL,			//==
	NOT,			//!=
	SMALL,			//<
	GREATER,		//>
	SEQUAL,			//<=
	GEQUAL,			//>=
};

//---------------------------
//�ϐ��̎��
//---------------------------
enum VAR_TYPE {
	VAR_ERROR = 0x00,		//�G���[			
	GLOBAL = 0x01,		//�O���[�o���ϐ�
	LOCAL,				//���[�J���ϐ�
};

//---------------------------
//���l�̎��
//---------------------------
enum VAL_TYPE {
	VAL_ERROR = 0x00,
	INT = 0x01,
	FLOAT,
	UNSIGNED_INT,
	UNSIGNED_FLOAT,
};

//----------------------------
//�e�L�X�g�̎��
//----------------------------
enum TEXT_TYPE {
	STRING = 0x00,		//�����̕�����
	NAME_TEXT,
	TEXT_ONLY
};

//==========================
//���o���p�̖��߃f�[�^�\��
//==========================
typedef struct {
	char code;			//���ߗp�R�[�h
	ORDER token;		//���e
}ORDER_DATA;

typedef struct {
	int priority;		//�D��x
	ORDER_DATA order;	//���ߓ��e
}ORDER_PRIORITY;


class SEMANTIC {
private:
	SEMANTIC();
	vector<vector<ORDER>> orderList;
	vector<vector<ORDER_DATA>> result;

	void Soushajo(vector<ORDER> _in);
	void Compile();							//���o��
public:
	void SetScript(vector<vector<ORDER>> _order);	//�\����͂̌��ʂ�ݒ肷��
	void Analysis();						//��͏���
	static SEMANTIC& GetInstance();
};

