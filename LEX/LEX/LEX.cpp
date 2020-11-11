#include "LEX.h"

LEX::LEX() {

}

//===============================
//インスタンスを渡す関数
//===============================
LEX & LEX::GetInstance()
{
	static LEX* instance = new LEX;		//LEXを作成
	return *instance;
}

//======================================
//解析関数
//======================================
void LEX::Analysis(string _str) {
	str = _str;
	str.push_back('\0');		//終末文字を登録
	//-----------------------------------
	//仮定フラグ群
	//-----------------------------------
	bool number = false;		//数値と仮定するかどうか
	bool strFlg = false;		//文字列として扱っている

	string buff;				//バッファ

	//字句解析のメインループ

	for (int i = 0; i < str.size(); i++) {

		//7bitの範囲(ASCII CODE)
#ifdef UNICODE
		if (str[i] < 0x7F) {	//7bit(ASCII)の範囲を超えるので
#else
		if (str[i] >= 0 ) {		//8bit目が1であれば負数になるので

#endif
			//------------------
			//数値
			//------------------
			if (isdigit(str[i])) {
				number = true;		//数値と仮定する
				if (strFlg) {
					strFlg = false;
					TOKEN tmp;
					tmp.str = buff;							//情報をトークンとして記録
					tmp.type = TOKEN_TYPE::TT_STRING;		//文字列
					buff.clear();		//クリア
					tokenList.push_back(tmp);		//リストに記録
				}
				buff += str[i];
				continue;
			}
			else if (str[i] == '.') {		//ピリオドであれば
				if (number) {
					buff += str[i];
					continue;
				}
			}
			else {
				if (number) {
					number = false;
					TOKEN tmp;
					tmp.str = buff;		//情報をトークンとして記録
					tmp.type = TOKEN_TYPE::TT_NUMBER;
					buff.clear();		//クリア
					tokenList.push_back(tmp);		//リストに記録
				}
			}

			if (Symbol(str[i])) {
				if (strFlg) {
					TOKEN tmp;
					tmp.str = buff;
					tmp.type = TOKEN_TYPE::TT_STRING;
					tokenList.push_back(tmp);
					buff.clear();
					strFlg = false;
				}
				TOKEN tmp;
				tmp.str = str[i];
				tmp.type = (TOKEN_TYPE)JudgeSymbol(str[i]);
				tokenList.push_back(tmp);
				continue;
			}

		}
		//ASCII codeの範囲から出るサイズの文字コード
		strFlg = true;
		buff += str[i];
		if (i + 1 < str.size()) {
			i++;
			//記号がなければ
			if (!Symbol(str[i])) {
				buff += str[i];
				continue;
			}
			else {
				//あればiを元に戻す
				i--;
			}
		}
	}

	//---------------------
	//最終文字列を取得
	//---------------------
	if (strFlg) {
		TOKEN tmp;
		tmp.str = buff;
		buff.clear();
		tmp.type = TOKEN_TYPE::TT_STRING;
		tokenList.push_back(tmp);
	}
}

//======================================
//特定記号かどうかの判定
//======================================
bool LEX::Symbol(char _symbol) {
	switch (_symbol) {
	case '	':		//HTAB
		break;
	case ' ':		//SPACE
		break;
	case '\n':		//NEW LINE
		break;
	case '#':		//SHARP
		break;
	case '=':
		break;
	case '+':
		break;
	case '-':
		break;
	case '*':
		break;
	case '/':
		break;
	case '%':
		break;
	case '<':
		break;
	case '>':
		break;
	case '|':
		break;
	case '(':
		break;
	case ')':
		break;
	case '[':
		break;
	case ']':
		break;
	case '$':
		break;
	case '!':
		break;
	default:
		return false;
	}
	return true;
}

//======================================
//記号のトークン種別分類
//======================================
int LEX::JudgeSymbol(char _symbol)
{
	switch (_symbol) {
	case '	':		//HTAB
		return TOKEN_TYPE::TT_TAB;
	case ' ':		//SPACE
		return TOKEN_TYPE::TT_SPACE;
	case '\n':		//NEW LINE
		return TOKEN_TYPE::TT_CRLF;
	case '#':		//SHARP
		return TOKEN_TYPE::TT_SHARP;
	case '=':
		return TOKEN_TYPE::TT_EQUAL;
	case '+':
		return TOKEN_TYPE::TT_PLUS;
	case '-':
		return TOKEN_TYPE::TT_MINUS;
	case '*':
		return TOKEN_TYPE::TT_ASTERISK;
	case '/':
		return TOKEN_TYPE::TT_SLASH;
	case '%':
		return TOKEN_TYPE::TT_PERCENT;
	case '<':
		return TOKEN_TYPE::TT_LESS;
	case '>':
		return TOKEN_TYPE::TT_GREATOR;
	case '|':
		return TOKEN_TYPE::TT_VBAR;
	case '(':
		return TOKEN_TYPE::TT_SPAR;
	case ')':
		return TOKEN_TYPE::TT_EPAR;
	case '[':
		return TOKEN_TYPE::TT_SBRACKET;
	case ']':
		return TOKEN_TYPE::TT_EBRACKET;
	case '$':
		return TOKEN_TYPE::TT_DOLL;
	case '!':
		return TOKEN_TYPE::TT_NOT;
	default:
		return -1;
	}
}

//======================================
//トークンリストを渡す関数
//======================================
vector<TOKEN> LEX::GetTokenList()
{
	return tokenList;
}
