#pragma once
#include "LEX.h"

//==============================
//�m�[�h�񋓃N���X
//==============================
enum class NODE {
	Null,		//
	STRING,		//
	NUMBER,		//
	VARIABLE,	//
	SHARP,		//
	OP,			// ���Z�q�m�[�h

	//---------------------
	//��͖؁i�H�j
	VBAR,		//
	DOLL,		//
	TAB,		//
	SPAR,		//
	EPAR,		//
	SBRACKET,	//
	EBRACKET,	//
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
private:
	int value;
public:
	virtual NODE GetNodeType() {
		return NODE::NUMBER;
	}

	NODE_NUMBER(int v):value(v){}	//����������
};

//=================================
//�ϐ��m�[�h
//=================================
class NODE_VARIABLE :public AST {
private:
	string varName;		//�ϐ���
public:
	virtual NODE GetNodeType() {
		return NODE::VARIABLE;
	}
};

//=================================
//���Z�q�m�[�h
//=================================
class NODE_OP :public AST {
private:
	string op;		//���Z�q
	AST *left;		//���ӂ̃m�[�h
	AST *right;		//�E�ӂ̃m�[�h
public:
	virtual NODE GetNodeType() { return NODE::OP; }

	NODE_OP(string _op, AST *_left, AST *_right) :
		op(_op), left(_left), right(_right) {}
};

//====================================
//�֐��m�[�h
//====================================
class NODE_FUNC :public AST {
private:
	string funcName;			//�֐���
	vector<AST*> childNode;		//�q�m�[�h(����)
public:

};

//====================================
//�����m�[�h
//====================================
class NODE_ARGUMENT :public AST {
private:
	vector<AST*> childNode;		//�q�m�[�h
public:

};


