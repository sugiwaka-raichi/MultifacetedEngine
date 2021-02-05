#pragma once
#include <string>
#include <vector>
#include <fstream>

using namespace std;
#define SYSTEM_MESSAGE 	1	//0非表示 1結果表示 2全表示

#ifdef UNICODE
#define string wstring
#else
#define string string
#endif

//================================
//トークンの種類
//================================
typedef enum class TOKEN_TYPE {
	TT_NULL = 0,		//null

	TT_STRING,			//文字列
	TT_VARIABLE,		//変数(欠)
	TT_NUMBER,			//数値
	TT_SEPARATION,		//区切り文字(欠)

	//-----------------
	//記号類
	TT_CRLF,			// 改行
	TT_SHARP,			// #
	TT_TAB,				// TAB
	TT_SPACE,			// SPACE
	TT_SPAR,			// (
	TT_EPAR,			// )
	TT_SBRACKET,		// [
	TT_EBRACKET,		// ]
	TT_VBAR,			// |
	TT_DOLL,			// $
	TT_NOT,				// !
	TT_WQORT,			// "

	//----------------
	//演算子
	TT_OP,			// 演算子
	TT_EQUAL,		// =
	TT_PLUS,		// +
	TT_MINUS,		// -
	TT_ASTERISK,	// *
	TT_SLASH,		// /
	TT_PERCENT,		// %
	TT_LESS,		// <
	TT_GREATOR,		// >

	TT_END			//終端記号
};

//===========================
//トークン定義
//===========================
typedef struct TOKEN {
	string str;
	TOKEN_TYPE type;
};

class LEX {
private:
	vector<TOKEN> tokenList;		//解析した字句の記録変数
	string str;						//取り扱う文字列

	LEX();			//コンストラクタを外部からの呼び出し禁止
	bool Symbol(char _symbol);		//記号判定処理
	TOKEN_TYPE JudgeSymbol(char _symbol);	//記号識別処理

public:
	static LEX& GetInstance();		//インスタンスを取得する関数
	void Analysis(string _str);		//解析関数
	vector<TOKEN> GetTokenList();	//トークンリスト用ゲッター

};