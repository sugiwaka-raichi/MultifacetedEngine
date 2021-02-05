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
	vector<TOKEN>().swap(tokenList);		//�g�[�N�����X�g��������
	//-----------------------------------
	//����t���O�Q
	//-----------------------------------
	bool number = false;		//���l�Ɖ��肷�邩�ǂ���
	bool minus = false;			//�����Ɖ��肷�邩�ǂ���
	bool period = false;		//�����_�����������ǂ���
	bool strFlg = false;		//������Ƃ��Ĉ����Ă���
	bool comment = false;		//�R�����g�Ƃ��ď�������
	string buff;				//�o�b�t�@

	//�����͂̃��C�����[�v

	for (int i = 0; i < str.size(); i++) {
		//\r�͓ǂݔ�΂�
		if (str[i] == '\r') {
			continue;
		}
		//---------------------
		//�R�����g�����̏���
		//---------------------
		if (str[i] == '/') {		//���ڂ�/
			if (str.size() > 0) {	//�������������
				if (str[i + 1] == '/') {		//���ڂ�/
					//����̓R�����g�Ȃ̂Ŏ��啪�����s��Ȃ�
					comment = true;
				}
			}
		}
		//�R�����g�t���O�������Ă����
		if (comment) {
			//���s�����܂œǂݔ�΂�
			if (str[i] == '\n') {
				comment = false;		//���s�����΃R�����g�t���O�������ăR�����g�I��
			}
			continue;
		}
		//7bit�͈̔�(ASCII CODE)
#ifdef UNICODE
		if (str[i] < 0x7F) {	//7bit(ASCII)�͈̔͂𒴂���̂�
#else
		if (str[i] >= 0 ) {		//8bit�ڂ�1�ł���Ε����ɂȂ�̂�

#endif
			//�}�C�i�X�ł���Ε����ł���\��������
			if (str[i] == '-') {
				minus = true;
				buff.push_back('-');
				continue;
			}
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
				if (number && !period) {
					buff += str[i];
					period = true;
					continue;
				}
				else {						//�s���I�h����ȏ�������Ƃ����l�Ƃ��Ĉ���Ȃ�
					number = false;
					period = false;
				}
			}
			else if (minus && !number) {		//�}�C�i�X�t���O�������Ă����
				//�����l�ł͂Ȃ�����
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
		//ASCII code�Ƃ��͈̔͂���o��T�C�Y�̕����R�[�h
		strFlg = true;
		if (number) {
			//���l�t���O�������Ă����
			number = false;
			TOKEN tmp;
			tmp.str = buff;		//�����g�[�N���Ƃ��ċL�^
			tmp.type = TOKEN_TYPE::TT_NUMBER;
			buff.clear();		//�N���A
			tokenList.push_back(tmp);		//���X�g�ɋL�^
		}
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
	//�ŏI�����E�������擾
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
		tmp.str = buff;		//�����g�[�N���Ƃ��ċL�^
		tmp.type = TOKEN_TYPE::TT_NUMBER;
		buff.clear();		//�N���A
		tokenList.push_back(tmp);		//���X�g�ɋL�^
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
//�L���̃g�[�N����ʕ���
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
//�g�[�N�����X�g��n���֐�
//======================================
vector<TOKEN> LEX::GetTokenList()
{
	return tokenList;
}
