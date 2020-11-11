#include "LEX.h"

LEX::LEX() {

}

//===============================
//�C���X�^���X��n���֐�
//===============================
LEX & LEX::GetInstance()
{
	static LEX* instance = new LEX;		//LEX���쐬
	return *instance;
}

//======================================
//��͊֐�
//======================================
void LEX::Analysis(string _str) {
	str = _str;
	str.push_back('\0');		//�I��������o�^
	//-----------------------------------
	//����t���O�Q
	//-----------------------------------
	bool number = false;		//���l�Ɖ��肷�邩�ǂ���
	bool strFlg = false;		//������Ƃ��Ĉ����Ă���

	string buff;				//�o�b�t�@

	//�����͂̃��C�����[�v

	for (int i = 0; i < str.size(); i++) {

		//7bit�͈̔�(ASCII CODE)
#ifdef UNICODE
		if (str[i] < 0x7F) {	//7bit(ASCII)�͈̔͂𒴂���̂�
#else
		if (str[i] >= 0 ) {		//8bit�ڂ�1�ł���Ε����ɂȂ�̂�

#endif
			//------------------
			//���l
			//------------------
			if (isdigit(str[i])) {
				number = true;		//���l�Ɖ��肷��
				if (strFlg) {
					strFlg = false;
					TOKEN tmp;
					tmp.str = buff;							//�����g�[�N���Ƃ��ċL�^
					tmp.type = TOKEN_TYPE::TT_STRING;		//������
					buff.clear();		//�N���A
					tokenList.push_back(tmp);		//���X�g�ɋL�^
				}
				buff += str[i];
				continue;
			}
			else if (str[i] == '.') {		//�s���I�h�ł����
				if (number) {
					buff += str[i];
					continue;
				}
			}
			else {
				if (number) {
					number = false;
					TOKEN tmp;
					tmp.str = buff;		//�����g�[�N���Ƃ��ċL�^
					tmp.type = TOKEN_TYPE::TT_NUMBER;
					buff.clear();		//�N���A
					tokenList.push_back(tmp);		//���X�g�ɋL�^
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
		//ASCII code�͈̔͂���o��T�C�Y�̕����R�[�h
		strFlg = true;
		buff += str[i];
		if (i + 1 < str.size()) {
			i++;
			//�L�����Ȃ����
			if (!Symbol(str[i])) {
				buff += str[i];
				continue;
			}
			else {
				//�����i�����ɖ߂�
				i--;
			}
		}
	}

	//---------------------
	//�ŏI��������擾
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
//����L�����ǂ����̔���
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
//�L���̃g�[�N����ʕ���
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
//�g�[�N�����X�g��n���֐�
//======================================
vector<TOKEN> LEX::GetTokenList()
{
	return tokenList;
}
