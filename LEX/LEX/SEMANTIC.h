#pragma once
#include "PARSE.h"
#include "OperationsStruct.h"

////------------------------------------
////���߈ꗗ
////------------------------------------
//enum OP_CODE {
//	//PUSH,		//�v�b�V��
//	OP = 0x01,			//���Z�q
//	VAL,		//���l
//	VAR,		//�ϐ�
//	LABEL,		//���x��
//	TEXT,		//�e�L�X�g
//	FUNC,		//�֐����s
//	ARG,		//����
//};
//
////---------------------------
////�ڍׂȉ��Z�q
////---------------------------
//enum OPERATOR {
//	NONE = 0x00,	//���Z�q�Ƃ��Ă̌��ʂȂ�		
//	//-----------------------
//	//�Z�p���Z�q L�V�t�g4bit
//	ADD = 0x01,		//���Z
//	SUBT,			//���Z
//	MUL,			//��Z
//	DIV,			//���Z
//	//-----------------------
//	//��r���Z�q
//	SMALL,			//< ��菬����
//	GREATER,		//> ���傫��
//	SUBS,			//���
//	//-----------------------
//	//��̉��Z�q 0x80��or���Z
//	NOT,			//!= �������Ȃ�
//	ADDS,			//���Z���
//	SUBTS,			//���Z���
//	MULS,			//��Z���
//	DIVS,			//���Z���
//	SEQUAL,			//<= �ȉ�
//	GEQUAL,			//>= �ȏ�
//	EQUAL,			//== ������
//};
//
////---------------------------
////�ϐ��̎��
////---------------------------
//enum VAR_TYPE {
//	VAR_ERROR = 0x00,		//�G���[			
//	GLOBAL = 0x01,		//�O���[�o���ϐ�
//	LOCAL,				//���[�J���ϐ�
//};
//
////---------------------------
////���l�̎��
////---------------------------
//enum VAL_TYPE {
//	VAL_ERROR = 0x00,
//	INT = 0x01,
//	FLOAT,
//	UNSIGNED_INT,
//	UNSIGNED_FLOAT,
//};
//
////----------------------------
////�e�L�X�g�̎��
////----------------------------
//enum TEXT_TYPE {
//	STRING = 0x00,		//�����̕�����
//	NAME_TEXT,
//	TEXT_ONLY
//};

//==========================
//���o���p�̖��߃f�[�^�\��
//==========================
typedef struct {
	char code;			//���ߗp�R�[�h
	ORDER token;		//���e
}ORDER_DATA;

//===================================
//���ԏ�A���S���Y���ň����\����
//===================================
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

