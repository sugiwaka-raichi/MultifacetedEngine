#include "PARSE.h"
#include <iostream>

using namespace std;

PARSE::PARSE() {
}

//=============================================================
//スタックと入力データを比較して適応するルール番号を返す関数
//=============================================================
int PARSE::RulesNum(STACK_TYPE _stack, TOKEN_TYPE _in) {
	switch (_stack) {
		//-----------------------------
		//スタックS
		//-----------------------------
	case STACK_TYPE::ST_S:
		if (_in == TOKEN_TYPE::TT_EBRACKET || _in == TOKEN_TYPE::TT_EPAR) {
			//]と)が先頭に来ることはあり得ないので構文エラーとなる
			//構文エラー
			break;
		}
		return 1;		//ルール番号1を渡す(解析開始)
	//-----------------------------
	//スタックS'
	//-----------------------------
	case STACK_TYPE::ST_Sd:
		if (_in == TOKEN_TYPE::TT_CRLF) {
			return 2;		//ルール番号2を渡す(改行)
		}
		else if (_in == TOKEN_TYPE::TT_END) {
			//改行文字であった場合
			return 3;		//ルール番号3渡す(解析終了)
		}
		else {
			//構文エラー
		}
		break;
	//-----------------------------
	//スタックL
	//-----------------------------
	case STACK_TYPE::ST_L:
		switch (_in) {
		case TOKEN_TYPE::TT_CRLF:
		case TOKEN_TYPE::TT_END:
			return 4;
		case TOKEN_TYPE::TT_SHARP:
			return 5;
		case TOKEN_TYPE::TT_STRING:
		case TOKEN_TYPE::TT_NUMBER:
		case TOKEN_TYPE::TT_TAB:
		case TOKEN_TYPE::TT_SPACE:
		case TOKEN_TYPE::TT_SPAR:
		case TOKEN_TYPE::TT_SBRACKET:
		case TOKEN_TYPE::TT_VBAR:
		case TOKEN_TYPE::TT_DOLL:
			return 6;
		default:
			return 0;		//構文エラー
		}
		break;
	case STACK_TYPE::ST_Ld:
		//ないやんけ
		break;
	case STACK_TYPE::ST_E:
		switch (_in)
		{
		case TT_STRING:
			return 11;
		case TT_NUMBER:
			return 9;
		case TT_TAB:
			return 10;
		case TT_CRLF:
		case TT_SPACE:
			return 0;		//式を区切れあほ
		case TT_SPAR:
			return 7;
		case TT_SBRACKET:
			return 8;
		case TT_VBAR:
		case TT_DOLL:
			return 9;
		default:
			return 0;
		}
	//-----------------------------
	//スタックE'
	//-----------------------------
	case STACK_TYPE::ST_Ed:
		switch (_in) {
		case TOKEN_TYPE::TT_CRLF:
		case TOKEN_TYPE::TT_EBRACKET:
		case TOKEN_TYPE::TT_EPAR:
			return 12;
		//演算子単位のトークンに入れかえる
		//case TT_PLUS:
		//case TT_MINUS:
		//case TT_ASTERISK:
		//case TT_SLASH:
		//case TT_PERCENT:
		case TOKEN_TYPE::TT_OP:
			return 14;
		default:
			return 0;
		}
	//-----------------------------
	//スタックT (文字列/数値)
	//-----------------------------
	case STACK_TYPE::ST_T:
		switch (_in) {
		case TOKEN_TYPE::TT_STRING:
			return 17;
		case TOKEN_TYPE::TT_NUMBER:
			return 18;
		}
	//-----------------------------
	//スタックT' (文字列/数値)
	//-----------------------------
	case STACK_TYPE::ST_Td:
		switch (_in) {
		case TOKEN_TYPE::TT_NUMBER:
			return 24;
		case TOKEN_TYPE::TT_STRING:
			return 25;
		case TOKEN_TYPE::TT_CRLF:
		case TOKEN_TYPE::TT_TAB:
		case TOKEN_TYPE::TT_EBRACKET:
			return 26;
		default:
			return 0;
		}
		break;
	//-----------------------------
	//スタックO (算術演算子)
	//-----------------------------
	case STACK_TYPE::ST_O:
		switch (_in) {
		//case TT_PLUS:
		//case TT_MINUS:
		//case TT_ASTERISK:
		//case TT_SLASH:
		//case TT_PERCENT:
		case TOKEN_TYPE::TT_OP:
			return 15;
		default:
			break;
		}
	//-----------------------------
	//スタックL
	//-----------------------------
	case STACK_TYPE::ST_V:
		switch (_in) {
		case TOKEN_TYPE::TT_NUMBER:
			return 16;
		case TOKEN_TYPE::TT_VBAR:
		case TOKEN_TYPE::TT_DOLL:
			return 15;
		default:
			break;
		}
	//-----------------------------
	//スタックA (引数)
	//-----------------------------
	case STACK_TYPE::ST_A:
		switch (_in) {
		case TOKEN_TYPE::TT_CRLF:
		case TOKEN_TYPE::TT_END:
			return 20;
		case TOKEN_TYPE::TT_DOLL:
		case TOKEN_TYPE::TT_VBAR:
		case TOKEN_TYPE::TT_NUMBER:
			return 21;
		case TOKEN_TYPE::TT_STRING:
			return 22;
		//case TOKEN_TYPE::TT_PLUS:
		//case TOKEN_TYPE::TT_MINUS:
		//case TOKEN_TYPE::TT_ASTERISK:
		//case TOKEN_TYPE::TT_SLASH:
		//case TOKEN_TYPE::TT_PERCENT:
		case TOKEN_TYPE::TT_OP:
			return 23;

		}
	//-----------------------------
	//スタック$ 終端記号
	//-----------------------------
	case STACK_TYPE::ST_$:
	case STACK_TYPE::ST_TOKEN:
		//スタックと入力から先頭を取り除きノードで管理する関数を呼び出す
		return 19;
		break;

	default:
		break;
	}
	return 0;		//構文エラー
}

//============================================
//ルールごとにスタックの処理を行う関数
//============================================
void PARSE::Rules(int _rulenum) {
	//STACK st;		//追加するスタックの情報を格納する変数

	switch (_rulenum)
	{
	case 1:		//1.S→LS'
		stack[0] = ST_Sd;						//現先頭スタックをS→S'に変換
		stack.insert(stack.begin(), ST_L);		//最先頭にLを
		break;
	case 2:		//2.S→CRLF LS'
		stack[0] = ST_Sd;		//現先頭スタックをS→S'に変換
		stack.insert(stack.begin(), ST_L);
		stack.insert(stack.begin(), ST_TOKEN);	//先頭がトークン
		
		break;
	case 3:		//3.S'→ε
		stack.erase(stack.begin());		//先頭スタックを削除
		break;
	case 4:
		stack.erase(stack.begin());		//先頭スタック削除
		break;
	case 5:
		stack[0] = ST_L;
		stack.insert(stack.begin(), ST_T);
		stack.insert(stack.begin(), ST_TOKEN);
		break;
	case 6:
		stack[0] = ST_E;
		break;
	case 7:
		stack[0] = ST_Ed;							// E'
		stack.insert(stack.begin(), ST_TOKEN);		// )
		stack.insert(stack.begin(), ST_E);			// E
		stack.insert(stack.begin(), ST_TOKEN);		// (
		break;
	case 8:
		stack[0] = ST_A;							// A
		stack.insert(stack.begin(), ST_TOKEN);		// ]
		stack.insert(stack.begin(), ST_T);			// T
		stack.insert(stack.begin(), ST_TOKEN);		// [
		break;
	case 9:
		//VE'
		stack[0] = ST_Ed;
		stack.insert(stack.begin(), ST_V);
		break;
	case 10:
		//TAB T
		stack[0] = ST_T;
		stack.insert(stack.begin(), ST_TOKEN);		// TAB
		break;
	case 11:
		//T TAB T
		stack[0] = ST_T;							// T
		stack.insert(stack.begin(), ST_TOKEN);		// TAB
		stack.insert(stack.begin(), ST_T);			// T
		break;
	case 12:
		stack.erase(stack.begin());					//削除
		break;
	case 13:
		//E' → O
		stack[0] = ST_O;		// O
		break;
	case 14:
		stack[0] = ST_E;
		stack.insert(stack.begin(), ST_TOKEN);		//演算子
		break;
	case 15:
		stack[0] = ST_T;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		//トークン
		break;
	case 16:
		stack[0] = ST_TOKEN;
		break;
	case 17:
		//T→strT'
		stack[0] = ST_Td;
		stack.insert(stack.begin(), ST_TOKEN);
		break;
	case 18:
		//T→numT'
		stack[0] = ST_Td;
		stack.insert(stack.begin(), ST_TOKEN);
		break;
	case 19:
		stack.erase(stack.begin());
		input.erase(input.begin());
		break;
	case 20:
		//A→ε 引数無し
		stack.erase(stack.begin());
		break;
	case 21:
		//A→VA  変数または数値
		stack[0] = STACK_TYPE::ST_A;
		stack.insert(stack.begin(), STACK_TYPE::ST_V);
		break;
	case 22:
		//A→TA	文字列
		stack[0] = STACK_TYPE::ST_A;
		stack.insert(stack.begin(), STACK_TYPE::ST_T);
		break;
	case 23:
		//A→OA	算術演算子
		stack[0] = STACK_TYPE::ST_A;
		stack.insert(stack.begin(), STACK_TYPE::ST_O);
		break;
	case 24:
		//T'→numT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		break;
	case 25:
		//T'→strT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		break;
	case 26:
		//T'→ε
		stack.erase(stack.begin());
		break;
		
	default:
		//エラー ルールがない
		break;
	}
}

void PARSE::OutPut()
{

	for (int i = 0; i < stack.size(); i++) {
		STACK_TYPE st = stack[i];
		switch (st)
		{
		case PARSE::ST_S:
			cout << "S";
			break;
		case PARSE::ST_Sd:
			cout << "S'";
			break;
		case PARSE::ST_L:
			cout << "L";
			break;
		case PARSE::ST_Ld:
			cout << "L'";
			break;
		case PARSE::ST_T:
			cout << "T";
			break;
		case PARSE::ST_Td:
			cout << "T'";
			break;
		case PARSE::ST_E:
			cout << "E";
			break;
		case PARSE::ST_Ed:
			cout << "E'";
			break;
		case PARSE::ST_O:
			cout << "O";
			break;
		case PARSE::ST_V:
			cout << "V";
			break;
		case PARSE::ST_A:
			cout << "A";
			break;
		case PARSE::ST_TOKEN:
			cout << "t";
			break;
		case PARSE::ST_$:
			cout << "$";
			break;
		default:
			break;
		}
	}
	cout << "			";
	for (int i = 0; i < input.size(); i++) {
		if (input[i].type == TOKEN_TYPE::TT_CRLF) {
			wcout << "\\n";
			continue;
		}
		else if (input[i].type == TOKEN_TYPE::TT_TAB) {
			wcout << " ";
			continue;
		}
		wcout << input[i].str;
	}

}

//=========================================
//インスタンスを渡す関数
//=========================================
PARSE & PARSE::GetInstance() {
	static PARSE* instance = new PARSE;		//パーサーを作成
	return *instance;
}

//=========================================
//構文解析関数
//=========================================
void PARSE::Analysis(vector<TOKEN> _tokenList) {
	//スタックの初期化
	//stack.resize(2);					//サイズを開始記号と終端記号を格納できるように二サイズ確保
	stack.insert(stack.begin(), ST_S);	//先頭に開始記号を入れる
	stack.push_back(ST_$);				//末尾に終端記号を入れる

	//入力値の取得
	input = _tokenList;		//入力値を確保
	vector<TOKEN>().swap(_tokenList);		//_tokenList解放
	TOKEN end;
	end.str = L"＄";		//終端記号(変数で$を使うので全角＄で識別)
	end.type = TOKEN_TYPE::TT_END;
	input.push_back(end);	//終端記号の登録
	//for (int i = 0; i < _tokenList.size(); i++) {
	while (true) {
		if (input[0].type == TOKEN_TYPE::TT_SPACE) {
			//スペースであれば読み飛ばす
			input.erase(input.begin());
		}

		int rulesnum = RulesNum(stack[0], input[0].type);		//スタックとトークンの先頭を比べ適応すべきルールを取得する
		if (rulesnum == 0) {
			cout << "ERROR" << input[0].type;
			getchar();
		}
		else {
			//for (int i = 0; i < stack.size(); i++) {
			//	cout << stack[i] << " ";
			//}
			//cout << "	";
			//for (int i = 0; i < input.size(); i++) {
			//	cout << input[i].type;
			//}
			cout << stack[0] << "	" << input[0].type << "	" << rulesnum << "	";
			OutPut();
			cout << endl;
		}
		
		Rules(rulesnum);		//解析表から取得したルール番号にあった処理を行う
		if (stack[0] == ST_$) {
			if (input[0].type == TOKEN_TYPE::TT_END) {
				//構文解析正常終了
				break;
			}
			else {
				//入力が残っている
				cout << "エラー:入力値が残っている" << endl;
				return;
			}
		}
	}
	cout << stack[0] << "	" << input[0].type << "	" << " " << "	";
	OutPut();
	cout << endl;
}
