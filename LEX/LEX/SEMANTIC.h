#pragma once
#include "PARSE.h"

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

enum OPERATOR {
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

typedef struct {
	ORDER token;		//���e
	char code;			//���ߗp�R�[�h
	int yuusendo;		//�D��x
}TOKEN_YUSEN;

class SEMANTIC {
private:
	SEMANTIC();
	vector<vector<ORDER>> orderList;
	vector<vector<ORDER>> result;

	void Soushajo(vector<ORDER> _in);
	void Compile();							//���o��
public:
	void SetScript(vector<vector<ORDER>> _order);	//�\����͂̌��ʂ�ݒ肷��
	void Analysis();						//��͏���
	static SEMANTIC& GetInstance();
};

