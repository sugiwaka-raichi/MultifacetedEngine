#pragma once
#include "PARSE.h"

//------------------------------------
//命令一覧
//------------------------------------
enum OP_CODE {
	//PUSH,		//プッシュ
	OP = 0x01,			//演算子
	VAL,		//数値
	VAR,		//変数
	LABEL,		//ラベル
	ARG,		//引数
	TEXT,		//テキスト
	FUNC,		//関数実行
};

//---------------------------
//詳細な演算子
//---------------------------
enum OPERATOR {
	O_ERROR = 0x00,	//エラー			
	//-----------------------
	//算術演算子
	ADD = 0x01,		//加算
	SUBT,			//減算
	MUL,			//乗算
	DIV,			//除算
	SUBS,			//代入
	//-----------------------
	//比較演算子
	EQUAL,			//==
	NOT,			//!=
	SMALL,			//<
	GREATER,		//>
	SEQUAL,			//<=
	GEQUAL,			//>=
};

//---------------------------
//変数の種類
//---------------------------
enum VAR_TYPE {
	VAR_ERROR = 0x00,		//エラー			
	GLOBAL = 0x01,		//グローバル変数
	LOCAL,				//ローカル変数
};

//---------------------------
//数値の種類
//---------------------------
enum VAL_TYPE {
	VAL_ERROR = 0x00,
	INT = 0x01,
	FLOAT,
	UNSIGNED_INT,
	UNSIGNED_FLOAT,
};

//----------------------------
//テキストの種類
//----------------------------
enum TEXT_TYPE {
	TT_ERROR = 0x00,
	NAME_TEXT = 0x01,
	TEXT_ONLY
};

//==========================
//書出し用の命令データ構造
//==========================
typedef struct {
	char code;			//命令用コード
	ORDER token;		//内容
}ORDER_DATA;

typedef struct {
	int priority;		//優先度
	ORDER_DATA order;	//命令内容
}ORDER_PRIORITY;


class SEMANTIC {
private:
	SEMANTIC();
	vector<vector<ORDER>> orderList;
	vector<vector<ORDER_DATA>> result;

	void Soushajo(vector<ORDER> _in);
	void Compile();							//書出し
public:
	void SetScript(vector<vector<ORDER>> _order);	//構文解析の結果を設定する
	void Analysis();						//解析処理
	static SEMANTIC& GetInstance();
};

