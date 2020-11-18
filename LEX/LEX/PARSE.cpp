#include "PARSE.h"
#include <iostream>

using namespace std;

PARSE::PARSE() {
}

//=============================================================
//�X�^�b�N�Ɠ��̓f�[�^���r���ēK�����郋�[���ԍ���Ԃ��֐�
//=============================================================
int PARSE::RulesNum(STACK_TYPE _stack, TOKEN_TYPE _in) {
	switch (_stack) {
		//-----------------------------
		//�X�^�b�NS
		//-----------------------------
	case STACK_TYPE::ST_S:
		if (_in == TOKEN_TYPE::TT_EBRACKET || _in == TOKEN_TYPE::TT_EPAR) {
			//]��)���擪�ɗ��邱�Ƃ͂��蓾�Ȃ��̂ō\���G���[�ƂȂ�
			//�\���G���[
			break;
		}
		return 1;		//���[���ԍ�1��n��(��͊J�n)
	//-----------------------------
	//�X�^�b�NS'
	//-----------------------------
	case STACK_TYPE::ST_Sd:
		if (_in == TOKEN_TYPE::TT_CRLF) {
			return 2;		//���[���ԍ�2��n��(���s)
		}
		else if (_in == TOKEN_TYPE::TT_END) {
			//���s�����ł������ꍇ
			return 3;		//���[���ԍ�3�n��(��͏I��)
		}
		else {
			//�\���G���[
		}
		break;
	//-----------------------------
	//�X�^�b�NL
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
			return 0;		//�\���G���[
		}
		break;
	case STACK_TYPE::ST_Ld:
		//�Ȃ����
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
			return 0;		//������؂ꂠ��
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
	//�X�^�b�NE'
	//-----------------------------
	case STACK_TYPE::ST_Ed:
		switch (_in) {
		case TOKEN_TYPE::TT_CRLF:
		case TOKEN_TYPE::TT_EBRACKET:
		case TOKEN_TYPE::TT_EPAR:
			return 12;
		//���Z�q�P�ʂ̃g�[�N���ɓ��ꂩ����
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
	//�X�^�b�NT (������/���l)
	//-----------------------------
	case STACK_TYPE::ST_T:
		switch (_in) {
		case TOKEN_TYPE::TT_STRING:
			return 17;
		case TOKEN_TYPE::TT_NUMBER:
			return 18;
		}
	//-----------------------------
	//�X�^�b�NT' (������/���l)
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
	//�X�^�b�NO (�Z�p���Z�q)
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
	//�X�^�b�NL
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
	//�X�^�b�NA (����)
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
	//�X�^�b�N$ �I�[�L��
	//-----------------------------
	case STACK_TYPE::ST_$:
	case STACK_TYPE::ST_TOKEN:
		//�X�^�b�N�Ɠ��͂���擪����菜���m�[�h�ŊǗ�����֐����Ăяo��
		return 19;
		break;

	default:
		break;
	}
	return 0;		//�\���G���[
}

//============================================
//���[�����ƂɃX�^�b�N�̏������s���֐�
//============================================
void PARSE::Rules(int _rulenum) {
	//STACK st;		//�ǉ�����X�^�b�N�̏����i�[����ϐ�

	switch (_rulenum)
	{
	case 1:		//1.S��LS'
		stack[0] = ST_Sd;						//���擪�X�^�b�N��S��S'�ɕϊ�
		stack.insert(stack.begin(), ST_L);		//�Ő擪��L��
		break;
	case 2:		//2.S��CRLF LS'
		stack[0] = ST_Sd;		//���擪�X�^�b�N��S��S'�ɕϊ�
		stack.insert(stack.begin(), ST_L);
		stack.insert(stack.begin(), ST_TOKEN);	//�擪���g�[�N��
		
		break;
	case 3:		//3.S'����
		stack.erase(stack.begin());		//�擪�X�^�b�N���폜
		break;
	case 4:
		stack.erase(stack.begin());		//�擪�X�^�b�N�폜
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
		stack.erase(stack.begin());					//�폜
		break;
	case 13:
		//E' �� O
		stack[0] = ST_O;		// O
		break;
	case 14:
		stack[0] = ST_E;
		stack.insert(stack.begin(), ST_TOKEN);		//���Z�q
		break;
	case 15:
		stack[0] = ST_T;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		//�g�[�N��
		break;
	case 16:
		stack[0] = ST_TOKEN;
		break;
	case 17:
		//T��strT'
		stack[0] = ST_Td;
		stack.insert(stack.begin(), ST_TOKEN);
		break;
	case 18:
		//T��numT'
		stack[0] = ST_Td;
		stack.insert(stack.begin(), ST_TOKEN);
		break;
	case 19:
		stack.erase(stack.begin());
		input.erase(input.begin());
		break;
	case 20:
		//A���� ��������
		stack.erase(stack.begin());
		break;
	case 21:
		//A��VA  �ϐ��܂��͐��l
		stack[0] = STACK_TYPE::ST_A;
		stack.insert(stack.begin(), STACK_TYPE::ST_V);
		break;
	case 22:
		//A��TA	������
		stack[0] = STACK_TYPE::ST_A;
		stack.insert(stack.begin(), STACK_TYPE::ST_T);
		break;
	case 23:
		//A��OA	�Z�p���Z�q
		stack[0] = STACK_TYPE::ST_A;
		stack.insert(stack.begin(), STACK_TYPE::ST_O);
		break;
	case 24:
		//T'��numT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		break;
	case 25:
		//T'��strT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		break;
	case 26:
		//T'����
		stack.erase(stack.begin());
		break;
		
	default:
		//�G���[ ���[�����Ȃ�
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
//�C���X�^���X��n���֐�
//=========================================
PARSE & PARSE::GetInstance() {
	static PARSE* instance = new PARSE;		//�p�[�T�[���쐬
	return *instance;
}

//=========================================
//�\����͊֐�
//=========================================
void PARSE::Analysis(vector<TOKEN> _tokenList) {
	//�X�^�b�N�̏�����
	//stack.resize(2);					//�T�C�Y���J�n�L���ƏI�[�L�����i�[�ł���悤�ɓ�T�C�Y�m��
	stack.insert(stack.begin(), ST_S);	//�擪�ɊJ�n�L��������
	stack.push_back(ST_$);				//�����ɏI�[�L��������

	//���͒l�̎擾
	input = _tokenList;		//���͒l���m��
	vector<TOKEN>().swap(_tokenList);		//_tokenList���
	TOKEN end;
	end.str = L"��";		//�I�[�L��(�ϐ���$���g���̂őS�p���Ŏ���)
	end.type = TOKEN_TYPE::TT_END;
	input.push_back(end);	//�I�[�L���̓o�^
	//for (int i = 0; i < _tokenList.size(); i++) {
	while (true) {
		if (input[0].type == TOKEN_TYPE::TT_SPACE) {
			//�X�y�[�X�ł���Γǂݔ�΂�
			input.erase(input.begin());
		}

		int rulesnum = RulesNum(stack[0], input[0].type);		//�X�^�b�N�ƃg�[�N���̐擪���דK�����ׂ����[�����擾����
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
		
		Rules(rulesnum);		//��͕\����擾�������[���ԍ��ɂ������������s��
		if (stack[0] == ST_$) {
			if (input[0].type == TOKEN_TYPE::TT_END) {
				//�\����͐���I��
				break;
			}
			else {
				//���͂��c���Ă���
				cout << "�G���[:���͒l���c���Ă���" << endl;
				return;
			}
		}
	}
	cout << stack[0] << "	" << input[0].type << "	" << " " << "	";
	OutPut();
	cout << endl;
}
