#pragma once
#include "LEX.h"

//==============================
//�m�[�h�񋓃N���X
//==============================
enum class NODE {
	Null,		//
	STRING,		//
	NUMBER,		//
	SPAR,		//
	EPAR,		//
	SBRACKET,	//
	EBRACKET,	//
	VBAR,		//
	DOLL,		//
	VARIABLE,	//
	SHARP,		//
	TAB,		//
	OP,			// ���Z�q�m�[�h
};

//==============================
//���ۍ\���؂̊�b�N���X
//==============================
class AST {
private:
protected:
public:
	virtual NODE GetNodeType() = 0;
};

//=================================
//���l�m�[�h
//=================================
class NODE_NUMBER:public AST {
public:
	int value;
	virtual NODE GetNodeType() {
		return NODE::NUMBER;
	}

	NODE_NUMBER(int v):value(v){}	//����������
};

//=================================
//�ϐ��m�[�h
//=================================
class NODE_VARIABLE :public AST {
public:
	string varName;		//�ϐ���
	virtual NODE GetNodeType() {
		return NODE::VARIABLE;
	}
};

//=================================
//���Z�q�m�[�h
//=================================
class NODE_OP :public AST {
public:
	char op;		//���Z�q
	AST *left;		//���ӂ̃m�[�h
	AST *right;		//�E�ӂ̃m�[�h
	virtual NODE GetNodeType() { return NODE::OP; }

	NODE_OP(char _op, AST *_left, AST *_right) :
		op(_op), left(_left), right(_right) {}
};

//====================================
//�֐��m�[�h
//====================================
class NODE_FUNCTION :public AST {
public:
	string name;		//�֐���
	AST *left;			//���Ӄm�[�h
	AST *right;			//�E�Ӄm�[�h
	virtual NODE GetNodeType() {}

};