#include <iostream>
#include <vector>
#include <stdio.h>
#include "LEX.h"
#include "PARSE.h"

using namespace std;

#ifdef  UNICODE
#define SCRIPT L"#START\n|a = (10.0 + 20)\nクド	「わふ～10」\n[if] 10 < $kud\n#END"
#else
#define SCRIPT "開始0#START\n(10.0 + 20)\nクド	「わふ～」\n(>ω<)/\nfiva[2]"
#endif

int main() {
	setlocale(LC_ALL, "Japanese");
	LEX lex = LEX::GetInstance();		//字句解析のインスタンスを取得
	lex.Analysis(SCRIPT);

	vector<TOKEN> token = lex.GetTokenList();	//トークンを取得

	for (int i = 0; i < token.size(); i++) {

#ifdef UNICODE
		wcout << "type:" << token[i].type;
		wcout << "	|str :" << token[i].str << endl;
#else
		cout << "type:" << token[i].type;
		cout << "	|str :" << token[i].str << endl;
#endif
	}

	PARSE parse = PARSE::GetInstance();		//構文解析のインスタンスを取得
	parse.Analysis(token);
}
