#pragma once
#include "LEX.h"

#define RULES (20)		//ルールの数

enum STACK_TYPE {
	ST_S = 0,	//スクリプト(開始)
	ST_Sd,		//スクリプト S'
	ST_L,		//単体
	ST_T,		//文字
	ST_Td,		//文字 T'
	ST_E,		//式
	ST_Ed,		//式 E'
	ST_O,		//算術演算子
	ST_V,		//変数/数値
	ST_F,		//関数/変数名先頭文字
	ST_Fd,		//関数/変数名先頭以降文字
	ST_A,		//引数
	ST_Ad,		//引数
	ST_TOKEN,	//取り除くトークン
	ST_$,		//終端記号
};

//-------------------------
//ノード構造体
//-------------------------
typedef struct _Node {
	TOKEN			token;		//ノードの内容
	STACK_TYPE		stack;		//スタックの内容
	_Node*			parent;		//親ノード
	vector<_Node*>  child;		//子ノード
}NODE;


//==============================
//構文解析クラス
//==============================
class PARSE {
struct STACK {
	STACK_TYPE	stack;
	TOKEN		token;
};

private:
	static PARSE Instance;
	NODE* root;		//ルートノード
	vector<STACK_TYPE> stack;		//スタック
	vector<TOKEN_TYPE> token_stack;	//取り出すトークン
	vector<TOKEN> input;		//入力

	PARSE();
	int RulesNum(STACK_TYPE _stack,TOKEN_TYPE _in);		//解析表から対応するルールを識別する関数
	void Rules(int _rulenum);						//解析表から取得したルールを実行してスタックに返す関数
	void Tree(STACK_TYPE _stack, TOKEN _token);					//取り出したスタックから構文木を作成
	void OutPut();
public:
	static PARSE &GetInstance();
	void Analysis(vector<TOKEN> _tokenList);		//解析処理
	NODE GetSyntaxTree();	//構文木を取得
};

