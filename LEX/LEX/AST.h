#pragma once
#include "LEX.h"

//==============================
//ノード列挙クラス
//==============================
enum class NODE {
	Null,		//
	STRING,		//
	NUMBER,		//
	VARIABLE,	//
	SHARP,		//
	OP,			// 演算子ノード

	//---------------------
	//解析木（？）
	VBAR,		//
	DOLL,		//
	TAB,		//
	SPAR,		//
	EPAR,		//
	SBRACKET,	//
	EBRACKET,	//
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
private:
	int value;
public:
	virtual NODE GetNodeType() {
		return NODE::NUMBER;
	}

	NODE_NUMBER(int v):value(v){}	//初期化処理
};

//=================================
//変数ノード
//=================================
class NODE_VARIABLE :public AST {
private:
	string varName;		//変数名
public:
	virtual NODE GetNodeType() {
		return NODE::VARIABLE;
	}
};

//=================================
//演算子ノード
//=================================
class NODE_OP :public AST {
private:
	string op;		//演算子
	AST *left;		//左辺のノード
	AST *right;		//右辺のノード
public:
	virtual NODE GetNodeType() { return NODE::OP; }

	NODE_OP(string _op, AST *_left, AST *_right) :
		op(_op), left(_left), right(_right) {}
};

//====================================
//関数ノード
//====================================
class NODE_FUNC :public AST {
private:
	string funcName;			//関数名
	vector<AST*> childNode;		//子ノード(引数)
public:

};

//====================================
//引数ノード
//====================================
class NODE_ARGUMENT :public AST {
private:
	vector<AST*> childNode;		//子ノード
public:

};


