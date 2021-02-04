#pragma once
#include "PARSE.h"

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

enum OPERATOR {
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

typedef struct {
	ORDER token;		//内容
	char code;			//命令用コード
	int yuusendo;		//優先度
}TOKEN_YUSEN;

class SEMANTIC {
private:
	SEMANTIC();
	vector<vector<ORDER>> orderList;
	vector<vector<ORDER>> result;

	void Soushajo(vector<ORDER> _in);
	void Compile();							//書出し
public:
	void SetScript(vector<vector<ORDER>> _order);	//構文解析の結果を設定する
	void Analysis();						//解析処理
	static SEMANTIC& GetInstance();
};

