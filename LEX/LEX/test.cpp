#include <iostream>
#include <vector>
#include <stdio.h>
#include "LEX.h"
#include "PARSE.h"

using namespace std;

#ifdef  UNICODE
#define SCRIPT L"#START\n|a = (10.0 + 20)\n�N�h	�u��Ӂ`10�v\n[if] 10 < $kud\n#END"
#else
#define SCRIPT "�J�n0#START\n(10.0 + 20)\n�N�h	�u��Ӂ`�v\n(>��<)/\nfiva[2]"
#endif

int main() {
	setlocale(LC_ALL, "Japanese");
	LEX lex = LEX::GetInstance();		//�����͂̃C���X�^���X���擾
	lex.Analysis(SCRIPT);

	vector<TOKEN> token = lex.GetTokenList();	//�g�[�N�����擾

	for (int i = 0; i < token.size(); i++) {

#ifdef UNICODE
		wcout << "type:" << token[i].type;
		wcout << "	|str :" << token[i].str << endl;
#else
		cout << "type:" << token[i].type;
		cout << "	|str :" << token[i].str << endl;
#endif
	}

	PARSE parse = PARSE::GetInstance();		//�\����͂̃C���X�^���X���擾
	parse.Analysis(token);
}
