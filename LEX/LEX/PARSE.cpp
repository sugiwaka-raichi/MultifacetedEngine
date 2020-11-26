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
		if (_in == TOKEN_TYPE::TT_EBRACKET || _in == TOKEN_TYPE::TT_EPAR || _in == TOKEN_TYPE::TT_SPACE || _in == TOKEN_TYPE::TT_OP) {
			//]と),space,演算子が先頭に来ることはあり得ないので構文エラーとなる
			//構文エラー
			return -1;
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
			return -1;
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
			return -1;		//構文エラー
		}
		break;
	//-----------------------------
	//スタックE
	//-----------------------------
	case STACK_TYPE::ST_E:
		switch (_in)
		{
		case TOKEN_TYPE::TT_STRING:
			return 11;
		case TOKEN_TYPE::TT_NUMBER:
		case TOKEN_TYPE::TT_VBAR:
		case TOKEN_TYPE::TT_DOLL:
			return 9;
		case TOKEN_TYPE::TT_TAB:
			return 10;
		case TOKEN_TYPE::TT_SPACE:
			return 12;		//区切り文字
		case TOKEN_TYPE::TT_SPAR:
			return 7;
		case TOKEN_TYPE::TT_SBRACKET:
			return 8;
		default:
			return -1;
		}
	//-----------------------------
	//スタックE'
	//-----------------------------
	case STACK_TYPE::ST_Ed:
		switch (_in) {
		case TOKEN_TYPE::TT_CRLF:
		case TOKEN_TYPE::TT_EBRACKET:
		case TOKEN_TYPE::TT_EPAR:
			return 13;
		//演算子単位のトークンに入れかえる
		//case TT_PLUS:
		//case TT_MINUS:
		//case TT_ASTERISK:
		//case TT_SLASH:
		//case TT_PERCENT:
		case TOKEN_TYPE::TT_OP:
			return 14;
		case TOKEN_TYPE::TT_SPACE:
			return 15;
		default:
			return -1;
		}
	//-----------------------------
	//スタックT (文字列/数値)
	//-----------------------------
	case STACK_TYPE::ST_T:
		switch (_in) {
		case TOKEN_TYPE::TT_STRING:
			return 20;
		case TOKEN_TYPE::TT_NUMBER:
			return 21;
		}
	//-----------------------------
	//スタックT' (文字列/数値)
	//-----------------------------
	case STACK_TYPE::ST_Td:
		switch (_in) {
		case TOKEN_TYPE::TT_NUMBER:
			return 23;
		case TOKEN_TYPE::TT_STRING:
			return 22;
		case TOKEN_TYPE::TT_CRLF:
			return 24;		//仮
		case TOKEN_TYPE::TT_TAB:		//文字列終了
			return 24;
		case TOKEN_TYPE::TT_SPACE:
			return 25;
		case TOKEN_TYPE::TT_SHARP:
			return 26;
		case TOKEN_TYPE::TT_SPAR:
			return 27;
		case TOKEN_TYPE::TT_EPAR:
			return 28;
		case TOKEN_TYPE::TT_SBRACKET:
			return 29;
		case TOKEN_TYPE::TT_EBRACKET:
			return 30;
		case TOKEN_TYPE::TT_VBAR:
			return 31;
		case TOKEN_TYPE::TT_DOLL:
			return 32;
		case TOKEN_TYPE::TT_OP:
			return 33;
		case TOKEN_TYPE::TT_END:
			return 24;
		default:
			return -1;
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
			return 16;
		default:
			break;
		}
	//-----------------------------
	//スタックV
	//-----------------------------
	case STACK_TYPE::ST_V:
		switch (_in) {
		case TOKEN_TYPE::TT_NUMBER:
			return 19;
		case TOKEN_TYPE::TT_VBAR:
			return 18;
		case TOKEN_TYPE::TT_DOLL:
			return 17;
		case TOKEN_TYPE::TT_END:
			//return 4;			//本来はV→εを呼ぶ
		default:
			return -1;
		}
	//-----------------------------
	//スタックF
	//-----------------------------
	case STACK_TYPE::ST_F:
		switch (_in) {
		case TT_STRING:
			return 34;
		default:
			return -1;
		}
	//-----------------------------
	//スタックF'
	//-----------------------------
	case STACK_TYPE::ST_Fd:
		switch (_in) {
		case TT_STRING:
			return 35;
		case TT_NUMBER:
			return 36;
		case TT_CRLF:
		case TT_EBRACKET:
			return 38;		//仮
		case TT_SPACE:
			return 37;
		case TT_OP:
		case TT_EPAR:
			return 38;
		case TT_END:
			return 38;		//仮
		default:
			return -1;
		}
	//-----------------------------
	//スタックA (引数)
	//-----------------------------
	case STACK_TYPE::ST_A:
		switch (_in) {
		case TOKEN_TYPE::TT_SPAR:
			return 39;
		case TOKEN_TYPE::TT_CRLF:
		//case TOKEN_TYPE::TT_EPAR:
			return 40;
		case TOKEN_TYPE::TT_STRING:
		case TOKEN_TYPE::TT_NUMBER:
		case TOKEN_TYPE::TT_TAB:
		case TOKEN_TYPE::TT_SPACE:
		case TOKEN_TYPE::TT_SBRACKET:
		case TOKEN_TYPE::TT_VBAR:
		case TOKEN_TYPE::TT_DOLL:
			return 41;
		case TOKEN_TYPE::TT_END:
			return 40;
		default:
			return -1;
		}
	//-----------------------------
	//スタックA' (引数)
	//-----------------------------
	case STACK_TYPE::ST_Ad:
		switch (_in) {
		case TOKEN_TYPE::TT_EPAR:
			return 42;
		case TOKEN_TYPE::TT_DOLL:
		case TOKEN_TYPE::TT_VBAR:
		case TOKEN_TYPE::TT_NUMBER:
			return 43;
		case TOKEN_TYPE::TT_STRING:
			return 44;
		case TOKEN_TYPE::TT_SPACE:
			return 45;
		case TOKEN_TYPE::TT_SBRACKET:
			return 47;
		case TOKEN_TYPE::TT_OP:
			return 46;
		default:
			return -1;
		}
	//-----------------------------
	//スタック$ 終端記号
	//-----------------------------
	case STACK_TYPE::ST_$:
	case STACK_TYPE::ST_TOKEN:
		//スタックと入力から先頭を取り除きノードで管理する関数を呼び出す
		return 0;
		break;

	default:
		break;
	}
	return -1;		//構文エラー
}

//============================================
//ルールごとにスタックの処理を行う関数
//============================================
void PARSE::Rules(int _rulenum) {
	//STACK st;		//追加するスタックの情報を格納する変数
	for (int i = 0; i < token_stack.size(); i++) {
		cout << "取り除くトークン" << i << ":" << token_stack[i] << endl;
	}
	switch (_rulenum)
	{
	case 0:		//スタックと入力取り除き
		if (token_stack.size() > 0 && input[0].type == token_stack.back()) {
			cout << "取り除いたトークン:" << token_stack.back() << endl;
			stack.erase(stack.begin());
			input.erase(input.begin());
			token_stack.pop_back();
		}
		else {
			//エラー
			cout << "\033[31m";
			cout << "不一致\n入力:" << input[0].type << "	スタック:" << token_stack.back() << endl;

			getchar();
			for (int i = 0; i < token_stack.size(); i++) {
				cout << "スタック" << i << ":" << token_stack[i] << endl;
			}
			cout << "\033[m";

			getchar();
		}
		break;
	case 1:		//1.S→LS'
		stack[0] = STACK_TYPE::ST_Sd;						//現先頭スタックをS→S'に変換
		stack.insert(stack.begin(), STACK_TYPE::ST_L);		//最先頭にLを
		break;
	case 2:		//2.S→CRLF LS'
		stack[0] = ST_Sd;		//現先頭スタックをS→S'に変換
		stack.insert(stack.begin(), STACK_TYPE::ST_L);
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);	//先頭がトークン
		token_stack.push_back(TOKEN_TYPE::TT_CRLF);			// 改行をトークンスタックに
		break;
	case 3:		//3.S'→ε
		stack.erase(stack.begin());		//先頭スタックを削除
		break;
	case 4:		//4.L→ε
		stack.erase(stack.begin());		//先頭スタック削除
		break;
	case 5:		//5.L→# F
		stack[0] = STACK_TYPE::ST_F;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SHARP);			// #
		break;
	case 6:		//6.L→E
		stack[0] = STACK_TYPE::ST_E;
		break;
	case 7:		//7.E→(E)E'
		stack[0] = STACK_TYPE::ST_Ed;							// E'
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// )
		stack.insert(stack.begin(), STACK_TYPE::ST_E);			// E
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// (
		token_stack.push_back(TOKEN_TYPE::TT_EPAR);				// )
		token_stack.push_back(TOKEN_TYPE::TT_SPAR);				// (
		break;
	case 8:		//8.E→[F]A
		stack[0] = ST_A;							// A
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// ]
		stack.insert(stack.begin(), STACK_TYPE::ST_F);			// F
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// [
		token_stack.push_back(TOKEN_TYPE::TT_EBRACKET);			// ]
		token_stack.push_back(TOKEN_TYPE::TT_SBRACKET);			// [
		break;
	case 9:		//9.E→VE'
		stack[0] = STACK_TYPE::ST_Ed;
		stack.insert(stack.begin(), STACK_TYPE::ST_V);
		break;
	case 10:	//10.E→TAB T
		stack[0] = STACK_TYPE::ST_T;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// TAB
		token_stack.push_back(TOKEN_TYPE::TT_TAB);			// TAB
		break;
	case 11:	//11.E→T TAB T
		stack[0] = STACK_TYPE::ST_T;							// T
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// TAB
		stack.insert(stack.begin(), STACK_TYPE::ST_T);			// T
		token_stack.push_back(TOKEN_TYPE::TT_TAB);			// TAB
		break;
	case 12:	//12.E→spE
		stack[0] = STACK_TYPE::ST_E;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SPACE);			// SPACE
		break;
	case 13:	//13.E'→ε
		stack.erase(stack.begin());					//スタック終了
		break;
	case 14:	//14.E'→O
		stack[0] = STACK_TYPE::ST_O;		// 演算子
		break;
	case 15:	//15.E'→spE'
		stack[0] = STACK_TYPE::ST_Ed;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// SPACE
		token_stack.push_back(TOKEN_TYPE::TT_SPACE);			// SPACE
		break;
	case 16:	//16.O→opE
		stack[0] = STACK_TYPE::ST_E;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		//演算子
		token_stack.push_back(TOKEN_TYPE::TT_OP);			//演算子
		break;
	case 17:	//17.V→$F		//変数
		stack[0] = ST_F;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		//トークン
		token_stack.push_back(TOKEN_TYPE::TT_DOLL);				// $
		break;
	case 18:	//18.V→|F		//変数
		stack[0] = ST_F;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		//トークン
		token_stack.push_back(TOKEN_TYPE::TT_VBAR);				// |
		break;
	case 19:	//19.V→num		//数値
		stack[0] = STACK_TYPE::ST_TOKEN;
		token_stack.push_back(TOKEN_TYPE::TT_NUMBER);			// number
		break;
	case 20:	//20.T→strT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_STRING);			// string
		break;
	case 21:	//21.T→numT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_NUMBER);			// number
		break;
	case 22:	//22.T'→strT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_STRING);			// string
		break;
	case 23:	//23.T'→numT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_NUMBER);			// number
		break;
	case 24:	//24.T'→ε
		stack.erase(stack.begin());
		break;
	case 25:	//25.T'→spε
		//stack.erase(stack.begin());
		stack[0] = STACK_TYPE::ST_TOKEN;		
		token_stack.push_back(TOKEN_TYPE::TT_SPACE);
		break;
	case 26:	//26.T'→#T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SHARP);			// #をトークンスタックに
		break;
	case 27:	//27.T'→(T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SPAR);			// (をトークンスタックに
		break;
	case 28:	//28.T'→)T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_EPAR);			// )をトークンスタックに
		break;
	case 29:	//29.T'→[T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SBRACKET);			// [をトークンスタックに
		break;
	case 30:	//30.T'→]T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_EBRACKET);			// ]をトークンスタックに
		break;
	case 31:	//31.T'→|T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_VBAR);			// |をトークンスタックに
		break;
	case 32:	//32.T'→$T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_DOLL);			// $をトークンスタックに
		break;
	case 33:	//33.T'→opT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_OP);			// 演算子をトークンスタックに
		break;
	case 34:	//34.F→strF'
		stack[0] = STACK_TYPE::ST_Fd;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_STRING);			// string
		break;
	case 35:	//26.F'→strF'
		stack[0] = STACK_TYPE::ST_Fd;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_STRING);			// string
		break;
	case 36:	//27.F'→numF'
		stack[0] = STACK_TYPE::ST_Fd;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_NUMBER);			// number
		break;
	case 37:	//28.F'→spε
		stack.erase(stack.begin());
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SPACE);			// SPACE
		break;
	case 38:	//38.F'→ε
		stack.erase(stack.begin());
		break;
	case 39:	//39.A→(A')
		stack[0] = STACK_TYPE::ST_TOKEN;
		stack.insert(stack.begin(), STACK_TYPE::ST_Ad);
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_EPAR);			// )
		token_stack.push_back(TOKEN_TYPE::TT_SPAR);			// (
		break;
	case 40:	//40.A→ε
		stack.erase(stack.begin());
		break;
	case 41:	//41.A→E
		stack[0] = STACK_TYPE::ST_E;
		break;
	case 42:	//42.A'→ε 引数無し
		stack.erase(stack.begin());
		break;
	case 43:	//43.A'→VA'  変数または数値
		stack[0] = STACK_TYPE::ST_Ad;
		stack.insert(stack.begin(), STACK_TYPE::ST_V);
		break;
	case 44:	//44.A'→TA'	文字列
		stack[0] = STACK_TYPE::ST_Ad;
		stack.insert(stack.begin(), STACK_TYPE::ST_T);
		break;
	case 45:	//45.A'→spA'
		stack[0] = STACK_TYPE::ST_Ad;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SPACE);			// SPACE
		break;
	case 46:	//46.A'→opA'
		stack[0] = STACK_TYPE::ST_Ad;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_OP);			// OP
		break;
	case 47:	//47.A'→[F]AA'
		stack[0] = ST_Ad;							// A'
		stack.insert(stack.begin(), STACK_TYPE::ST_A);			// A
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// ]
		stack.insert(stack.begin(), STACK_TYPE::ST_F);			// F
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// [
		token_stack.push_back(TOKEN_TYPE::TT_EBRACKET);			// ]
		token_stack.push_back(TOKEN_TYPE::TT_SBRACKET);			// [
		break;
	default:
		//エラー ルールがない
		break;
	}
}

void PARSE::Tree(STACK_TYPE _stack, TOKEN _token) {
	if (root == nullptr) {
		root = new NODE;
	}
	else {
		//root->child.push_back(new NODE);		//子のノードにノード作成
	}
	root->stack = _stack;		//スタックを記録
	root->token = _token;		//トークンを記録
}

void PARSE::OutPut()
{

	for (int i = 0; i < stack.size(); i++) {
		STACK_TYPE st = stack[i];
		switch (st)
		{
		case STACK_TYPE::ST_S:
			cout << "S";
			break;
		case STACK_TYPE::ST_Sd:
			cout << "S'";
			break;
		case STACK_TYPE::ST_L:
			cout << "L";
			break;
		case STACK_TYPE::ST_T:
			cout << "T";
			break;
		case STACK_TYPE::ST_Td:
			cout << "T'";
			break;
		case STACK_TYPE::ST_E:
			cout << "E";
			break;
		case STACK_TYPE::ST_Ed:
			cout << "E'";
			break;
		case STACK_TYPE::ST_O:
			cout << "O";
			break;
		case STACK_TYPE::ST_V:
			cout << "V";
			break;
		case STACK_TYPE::ST_F:
			cout << "F";
			break;
		case STACK_TYPE::ST_Fd:
			cout << "F'";
			break;
		case STACK_TYPE::ST_A:
			cout << "A";
			break;
		case STACK_TYPE::ST_Ad:
			cout << "A'";
			break;
		case STACK_TYPE::ST_TOKEN:
			cout << "t";
			break;
		case STACK_TYPE::ST_$:
			cout << "$";
			break;
		default:
			break;
		}
	}
	cout << "		";
	for (int i = 0; i < input.size(); i++) {
		if (input[i].type == TOKEN_TYPE::TT_CRLF) {
			cout << "\\n";
			continue;
		}
		else if (input[i].type == TOKEN_TYPE::TT_TAB) {
			cout << " ";
			continue;
		}
		else if (input[i].type == TOKEN_TYPE::TT_SPACE) {
			cout << "sp";
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
		//if (input[0].type == TOKEN_TYPE::TT_SPACE) {
		//	//スペースであれば読み飛ばす
		//	input.erase(input.begin());
		//}

		int rulesnum = RulesNum(stack[0], input[0].type);		//スタックとトークンの先頭を比べ適応すべきルールを取得する
		if (rulesnum == -1) {
			wcout << L"\033[31m";
			cout << "ERROR" << input[0].type;
			wcout << L"\033[m";

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
			//cout << stack[0] << "	" << input[0].type << "	" << rulesnum << "	";
			//OutPut();
			//cout << endl;
		}
		cout << stack[0] << "	" << input[0].type << "	" << rulesnum << "	";
		OutPut();
		cout << "\r\n";
		Rules(rulesnum);		//解析表から取得したルール番号にあった処理を行う

		if (stack[0] == ST_$) {
			if (input[0].type == TOKEN_TYPE::TT_END) {
				//構文解析正常終了
				break;
			}
			else {
				//入力が残っている
				wcout << L"\033[31m";

				cout << "エラー:入力値が残っている" << endl;
				wcout << L"\033[m";

				return;
			}
		}

	}

}
