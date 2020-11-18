#pragma once
#include "LEX.h"

#define RULES (20)		//ルールの数

//-------------------------
//ノード構造体
//-------------------------
typedef struct _Node {
	TOKEN			token;		//ノードの内容
	_Node*			parent;		//親ノード
	vector<_Node*>  child;		//子ノード
}NODE;


//==============================
//構文解析クラス
//==============================
class PARSE {
enum STACK_TYPE {
	ST_S = 0,	//スクリプト(開始)
	ST_Sd,		//スクリプト S'
	ST_L,		//トークン
	ST_Ld,		//トークン L'
	ST_T,		//文字
	ST_Td,		//文字 T'
	ST_E,		//式
	ST_Ed,		//式 E'
	ST_O,		//算術演算子
	ST_V,		//変数/数値
	ST_A,		//引数
	ST_TOKEN,	//トークン
	ST_$,		//終端記号
};

struct STACK {
	STACK_TYPE	stack;
	TOKEN		token;
};

private:
	static PARSE Instance;
	NODE root;		//ルートノード
	vector<STACK_TYPE> stack;		//スタック
	vector<TOKEN> input;		//入力

	PARSE();
	int RulesNum(STACK_TYPE _stack,TOKEN_TYPE _in);		//解析表から対応するルールを識別する関数
	void Rules(int _rulenum);						//解析表から取得したルールを実行してスタックに返す関数

	void OutPut();
public:
	static PARSE &GetInstance();
	void Analysis(vector<TOKEN> _tokenList);		//解析処理
	NODE GetSyntaxTree();	//構文木を取得
};

