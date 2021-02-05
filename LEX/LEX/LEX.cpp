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
	vector<TOKEN>().swap(tokenList);		//トークンリストを初期化
	//-----------------------------------
	//仮定フラグ群
	//-----------------------------------
	bool number = false;		//数値と仮定するかどうか
	bool minus = false;			//負数と仮定するかどうか
	bool period = false;		//小数点が入ったかどうか
	bool strFlg = false;		//文字列として扱っている
	bool comment = false;		//コメントとして処理する
	string buff;				//バッファ

	//字句解析のメインループ

	for (int i = 0; i < str.size(); i++) {
		//\rは読み飛ばす
		if (str[i] == '\r') {
			continue;
		}
		//---------------------
		//コメント部分の処理
		//---------------------
		if (str[i] == '/') {		//一回目の/
			if (str.size() > 0) {	//次文字があれば
				if (str[i + 1] == '/') {		//二回目の/
					//これはコメントなので字句分割を行わない
					comment = true;
				}
			}
		}
		//コメントフラグがたっていれば
		if (comment) {
			//改行されるまで読み飛ばす
			if (str[i] == '\n') {
				comment = false;		//改行されればコメントフラグをおってコメント終了
			}
			continue;
		}
		//7bitの範囲(ASCII CODE)
#ifdef UNICODE
		if (str[i] < 0x7F) {	//7bit(ASCII)の範囲を超えるので
#else
		if (str[i] >= 0 ) {		//8bit目が1であれば負数になるので

#endif
			//マイナスであれば負数である可能性を入れる
			if (str[i] == '-') {
				minus = true;
				buff.push_back('-');
				continue;
			}
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
				if (number && !period) {
					buff += str[i];
					period = true;
					continue;
				}
				else {						//ピリオドが二つ以上入ったとき数値として扱わない
					number = false;
					period = false;
				}
			}
			else if (minus && !number) {		//マイナスフラグがたっていれば
				//負数値ではなかった
				minus = false;
				TOKEN tmp;
				tmp.str = buff;
				tmp.type = TOKEN_TYPE::TT_OP;
				buff.clear();
				tokenList.push_back(tmp);
			}
			else {
				if (number) {
					number = false;
					minus = false;
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
		//ASCII codeとその範囲から出るサイズの文字コード
		strFlg = true;
		if (number) {
			//数値フラグがたっていれば
			number = false;
			TOKEN tmp;
			tmp.str = buff;		//情報をトークンとして記録
			tmp.type = TOKEN_TYPE::TT_NUMBER;
			buff.clear();		//クリア
			tokenList.push_back(tmp);		//リストに記録
		}
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
	//最終文字・数字を取得
	//---------------------
	if (strFlg) {
		TOKEN tmp;
		tmp.str = buff;
		buff.clear();
		tmp.type = TOKEN_TYPE::TT_STRING;
		tokenList.push_back(tmp);
	}
	else if(number) {
		number = false;
		TOKEN tmp;
		tmp.str = buff;		//情報をトークンとして記録
		tmp.type = TOKEN_TYPE::TT_NUMBER;
		buff.clear();		//クリア
		tokenList.push_back(tmp);		//リストに記録
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
	case '=':
		//break;
	case '+':
		//break;
	case '-':
		//break;
	case '*':
		//break;
	case '/':
		//break;
	case '%':
	case '<':
	case '>':
	case '!':
		break;
	case '\r':
		return false;
	case '"':
		break;
	//case '<':
	//	break;
	//case '>':
	//	break;
	//case '!':
	//	break;
	default:
		return false;
	}
	return true;
}

//======================================
//記号のトークン種別分類
//======================================
TOKEN_TYPE LEX::JudgeSymbol(char _symbol)
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
		//return TOKEN_TYPE::TT_EQUAL;
	case '+':
		//return TOKEN_TYPE::TT_PLUS;
	case '-':
		//return TOKEN_TYPE::TT_MINUS;
	case '*':
		//return TOKEN_TYPE::TT_ASTERISK;
	case '/':
		//return TOKEN_TYPE::TT_SLASH;
	case '%':
		//return TOKEN_TYPE::TT_PERCENT;
	case '<':
	case '>':
	case '!':
		return TOKEN_TYPE::TT_OP;
	//case '<':
	//	return TOKEN_TYPE::TT_LESS;
	//case '>':
	//	return TOKEN_TYPE::TT_GREATOR;
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
	case '"':
		return TOKEN_TYPE::TT_WQORT;
	//case '!':
	//	return TOKEN_TYPE::TT_NOT;
	//case '\r':
	//default:
	}
}

//======================================
//トークンリストを渡す関数
//======================================
vector<TOKEN> LEX::GetTokenList()
{
	return tokenList;
}
