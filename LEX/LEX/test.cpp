#include <iostream>
#include <vector>
#include <fstream>
#include <codecvt>
#include "LEX.h"
#include "PARSE.h"
#include "StringOperation.h"
#include "SEMANTIC.h"

using namespace std;

//#ifdef  UNICODE
//#define SCRIPT L"#START\n//[dev0]\nクド	「わふ～10」\n|a10 = (10.0 / |a*(|k + |d))\n[if] 10 < $kud\n#END"
//#else
//#define SCRIPT "開始0#START\n(10.0 + 20)\nクド	「わふ～」\n(>ω<)/\nfiva[2]"
//#endif

//=======================================
//プロトタイプ宣言
//=======================================
string Load();
string Load(string filepath);
string StringLine(string* _str_p);

int main() {
	LEX lex = LEX::GetInstance();		//字句解析のインスタンスを取得
	PARSE parse = PARSE::GetInstance();		//構文解析のインスタンスを取得
	SEMANTIC sem = SEMANTIC::GetInstance();
	setlocale(LC_ALL, "Japanese");

	while (true) {
		string script = Load();

		//script = SCRIPT;
		vector<string> line_script = StringOperation::Split(script,L'\n');		//行ごとに分ける

		wcout << L"スクリプトの内容" << endl;
		wcout << script << "\n" << endl;
		//--------------------------------------------------
		//一行処理
		//--------------------------------------------------
		for (int i = 0; i < line_script.size(); i++) {
#if SYSTEM_MESSAGE >= 1
			wcout << L"\033[32m";
			wcout << line_script[i] << endl;
			wcout << L"\033[m";
#endif 
			lex.Analysis(line_script[i]);

			vector<TOKEN> token = lex.GetTokenList();	//トークンを取得
#if SYSTEM_MESSAGE >= 1
			for (int i = 0; i < token.size(); i++) {

#ifdef UNICODE
				wcout << L"\033[33m";

				wcout << "type:" << (int)token[i].type;
				wcout << "	|str :" << token[i].str << endl;
				wcout << L"\033[m";

#else
				cout << "\033[33m";

				cout << "type:" << token[i].type;
				cout << "	|str :" << token[i].str << endl;
				cout << "\033[m";
#endif
			}
			cout << "\n";
			parse.Analysis(token);
			cout << endl;
#endif
		}
		sem.SetScript(parse.GetOrder());		//構文解析の結果を渡す
		sem.Analysis();

		cout << "解析終了" << endl;
		getchar();			//再解析を行う
		system("CLS");
	}
}

string Load() {
	string script = Load(L"script.txt");
	return script;
}

string Load(string filepath) {
	//string line;
	int i = 0;
	wifstream text(filepath);
	if (text.fail()) {
		string script = L"file open fail";
		wcout << script << endl;
		return script;
	}
	text.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));					//utf8形式のワイド文字に
	string script((istreambuf_iterator<wchar_t>(text)), istreambuf_iterator<wchar_t>());
	//while (getline(text,line))
	//{
	//	script += line;
	//	i++;
	//}
	text.close();
	return script;
}
