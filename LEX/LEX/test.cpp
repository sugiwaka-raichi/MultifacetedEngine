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
//#define SCRIPT L"#START\n//[dev0]\n�N�h	�u��Ӂ`10�v\n|a10 = (10.0 / |a*(|k + |d))\n[if] 10 < $kud\n#END"
//#else
//#define SCRIPT "�J�n0#START\n(10.0 + 20)\n�N�h	�u��Ӂ`�v\n(>��<)/\nfiva[2]"
//#endif

//=======================================
//�v���g�^�C�v�錾
//=======================================
string Load();
string Load(string filepath);
string StringLine(string* _str_p);

int main() {
	LEX lex = LEX::GetInstance();		//�����͂̃C���X�^���X���擾
	PARSE parse = PARSE::GetInstance();		//�\����͂̃C���X�^���X���擾
	SEMANTIC sem = SEMANTIC::GetInstance();
	setlocale(LC_ALL, "Japanese");

	while (true) {
		string script = Load();

		//script = SCRIPT;
		vector<string> line_script = StringOperation::Split(script,L'\n');		//�s���Ƃɕ�����

		wcout << L"�X�N���v�g�̓��e" << endl;
		wcout << script << "\n" << endl;
		//--------------------------------------------------
		//��s����
		//--------------------------------------------------
		for (int i = 0; i < line_script.size(); i++) {
#if SYSTEM_MESSAGE >= 1
			wcout << L"\033[32m";
			wcout << line_script[i] << endl;
			wcout << L"\033[m";
#endif 
			lex.Analysis(line_script[i]);

			vector<TOKEN> token = lex.GetTokenList();	//�g�[�N�����擾
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
		sem.SetScript(parse.GetOrder());		//�\����͂̌��ʂ�n��
		sem.Analysis();

		cout << "��͏I��" << endl;
		getchar();			//�ĉ�͂��s��
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
	text.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));					//utf8�`���̃��C�h������
	string script((istreambuf_iterator<wchar_t>(text)), istreambuf_iterator<wchar_t>());
	//while (getline(text,line))
	//{
	//	script += line;
	//	i++;
	//}
	text.close();
	return script;
}
