#pragma once
#include "LEX.h"

//==============================
//ノード列挙クラス
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
	OP,			// 演算子ノード
};

//==============================
//抽象構文木の基礎クラス
//==============================
class AST {
private:
protected:
public:
	virtual NODE GetNodeType() = 0;
};

//=================================
//数値ノード
//=================================
class NODE_NUMBER:public AST {
public:
	int value;
	virtual NODE GetNodeType() {
		return NODE::NUMBER;
	}

	NODE_NUMBER(int v):value(v){}	//初期化処理
};

//=================================
//変数ノード
//=================================
class NODE_VARIABLE :public AST {
public:
	string varName;		//変数名
	virtual NODE GetNodeType() {
		return NODE::VARIABLE;
	}
};

//=================================
//演算子ノード
//=================================
class NODE_OP :public AST {
public:
	char op;		//演算子
	AST *left;		//左辺のノード
	AST *right;		//右辺のノード
	virtual NODE GetNodeType() { return NODE::OP; }

	NODE_OP(char _op, AST *_left, AST *_right) :
		op(_op), left(_left), right(_right) {}
};

//====================================
//関数ノード
//====================================
class NODE_FUNCTION :public AST {
public:
	string name;		//関数名
	AST *left;			//左辺ノード
	AST *right;			//右辺ノード
	virtual NODE GetNodeType() {}

};