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
	ST_P,		//区切り文字
	ST_TOKEN,	//取り除くトークン
	ST_$,		//終端記号
};

//---------------------------------------------------------
//命令のトークン単位種別
//---------------------------------------------------------
typedef enum class _order {
	STRING,		//文字列
	FUNCTION,	//関数
	LABEL,		//ラベル
	VALUE,		//値
	GVARIABLE,	//グローバル変数
	LVARIABLE,	//ローカル変数
	OPERATION,	//演算子
	ARGUMENT,	//引数
	PAR,		//(
	END,		//区切り文字や括弧終わり
}ORDER_TOKEN;

//---------------------------------------------------------
//1命令のデータ
//---------------------------------------------------------
typedef struct {
	string script;		//1トークンのスクリプトの内容
	ORDER_TOKEN token;	//種別
}ORDER;

////-------------------------
////ノード構造体
////-------------------------
//typedef struct _Node {
//	TOKEN			token;		//ノードの内容
//	STACK_TYPE		stack;		//スタックの内容
//	_Node*			parent;		//親ノード
//	vector<_Node*>  child;		//子ノード
//}NODE;


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
//	NODE* root;		//ルートノード
	vector<STACK_TYPE> stack;		//スタック
	vector<TOKEN_TYPE> token_stack;	//取り出すトークン
	vector<TOKEN> input;		//入力
	vector<ORDER> order;		//一行の命令一覧
	vector<vector<ORDER>> orderList;		//構文解析後の命令一覧
	ORDER_TOKEN nowToken;		//現在判別しているトークン

	PARSE();
	int RulesNum(STACK_TYPE _stack,TOKEN_TYPE _in);		//解析表から対応するルールを識別する関数
	void Rules(int _rulenum);						//解析表から取得したルールを実行してスタックに返す関数
	void Tree(STACK_TYPE _stack, TOKEN _token);					//取り出したスタックから構文木を作成
	void OutPut();
public:
	static PARSE &GetInstance();
	vector<vector<ORDER>> GetOrder();
	void Analysis(vector<TOKEN> _tokenList);		//解析処理
//	NODE GetSyntaxTree();	//構文木を取得
};

