#include "PARSE.h"
#include <iostream>

using namespace std;

typedef enum class _order{
	STRING,		//������
	FUNCTION,	//�֐�
	LABEL,		//���x��
	VALUE,		//�l
	VARIABLE,	//�ϐ�
	OPERATION,	//���Z�q
	ARGUMENT,	//����
	END,		//��؂蕶���⊇�ʏI���
}ORDER_TOKEN;

typedef struct {
	string script;		//1�g�[�N���̃X�N���v�g�̓��e
	ORDER_TOKEN token;	//���
}ORDER;

vector<string> data;
vector<ORDER> order;
ORDER_TOKEN nowToken;		//���ݔ��ʂ��Ă���g�[�N��

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
		if (_in == TOKEN_TYPE::TT_EBRACKET || _in == TOKEN_TYPE::TT_EPAR || _in == TOKEN_TYPE::TT_SPACE || _in == TOKEN_TYPE::TT_OP) {
			//]��),space,���Z�q���擪�ɗ��邱�Ƃ͂��蓾�Ȃ��̂ō\���G���[�ƂȂ�
			//�\���G���[
			return -1;
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
			return -1;
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
		case TOKEN_TYPE::TT_TAB:
			return 7;
		case TOKEN_TYPE::TT_STRING:
			return 8;
		case TOKEN_TYPE::TT_NUMBER:
		case TOKEN_TYPE::TT_SPACE:
		case TOKEN_TYPE::TT_SPAR:
		case TOKEN_TYPE::TT_SBRACKET:
		case TOKEN_TYPE::TT_VBAR:
		case TOKEN_TYPE::TT_DOLL:
			return 6;
		default:
			return -1;		//�\���G���[
		}
		break;
	//-----------------------------
	//�X�^�b�NE
	//-----------------------------
	case STACK_TYPE::ST_E:
		switch (_in)
		{
		case TOKEN_TYPE::TT_NUMBER:
		case TOKEN_TYPE::TT_VBAR:
		case TOKEN_TYPE::TT_DOLL:
			return 11;
		case TOKEN_TYPE::TT_SPACE:
			return 12;		//��؂蕶��
		case TOKEN_TYPE::TT_SPAR:
			return 9;
		case TOKEN_TYPE::TT_SBRACKET:
			return 10;
		default:
			return -1;
		}
	//-----------------------------
	//�X�^�b�NE'
	//-----------------------------
	case STACK_TYPE::ST_Ed:
		switch (_in) {
		case TOKEN_TYPE::TT_CRLF:
		case TOKEN_TYPE::TT_EBRACKET:
		case TOKEN_TYPE::TT_EPAR:
		case TOKEN_TYPE::TT_END:
			return 13;
		//���Z�q�P�ʂ̃g�[�N���ɓ��ꂩ����
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
	//�X�^�b�NT (������/���l)
	//-----------------------------
	case STACK_TYPE::ST_T:
		switch (_in) {
		case TOKEN_TYPE::TT_STRING:
			return 21;
		case TOKEN_TYPE::TT_NUMBER:
			return 22;
		}
	//-----------------------------
	//�X�^�b�NT' (������/���l)
	//-----------------------------
	case STACK_TYPE::ST_Td:
		switch (_in) {
		case TOKEN_TYPE::TT_NUMBER:
			return 24;
		case TOKEN_TYPE::TT_STRING:
			return 23;
		case TOKEN_TYPE::TT_CRLF:
			return 25;		//��
		case TOKEN_TYPE::TT_TAB:		//������I��
			return 25;
		case TOKEN_TYPE::TT_SPACE:
			return 26;
		case TOKEN_TYPE::TT_SHARP:
			return 27;
		case TOKEN_TYPE::TT_SPAR:
			return 28;
		case TOKEN_TYPE::TT_EPAR:
			return 29;
		case TOKEN_TYPE::TT_SBRACKET:
			return 30;
		case TOKEN_TYPE::TT_EBRACKET:
			return 31;
		case TOKEN_TYPE::TT_VBAR:
			return 32;
		case TOKEN_TYPE::TT_DOLL:
			return 33;
		case TOKEN_TYPE::TT_OP:
			return 34;
		case TOKEN_TYPE::TT_END:
			return 25;
		default:
			return -1;
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
			return 16;
		case TOKEN_TYPE::TT_DOLL:
		case TOKEN_TYPE::TT_VBAR:
		case TOKEN_TYPE::TT_NUMBER:
		case TOKEN_TYPE::TT_SBRACKET:
		case TOKEN_TYPE::TT_SPAR:
			break;
		case TOKEN_TYPE::TT_SPACE:
			return 17;
		default:
			break;
		}
	//-----------------------------
	//�X�^�b�NV
	//-----------------------------
	case STACK_TYPE::ST_V:
		switch (_in) {
		case TOKEN_TYPE::TT_NUMBER:
			return 20;
		case TOKEN_TYPE::TT_VBAR:
			return 19;
		case TOKEN_TYPE::TT_DOLL:
			return 18;
		case TOKEN_TYPE::TT_END:
			//return 4;			//�{����V���Â��Ă�
		default:
			return -1;
		}
	//-----------------------------
	//�X�^�b�NF
	//-----------------------------
	case STACK_TYPE::ST_F:
		switch (_in) {
		case TOKEN_TYPE::TT_STRING:
			return 35;
		default:
			return -1;
		}
	//-----------------------------
	//�X�^�b�NF'
	//-----------------------------
	case STACK_TYPE::ST_Fd:
		switch (_in) {
		case TOKEN_TYPE::TT_STRING:
			return 36;
		case TOKEN_TYPE::TT_NUMBER:
			return 37;
		case TOKEN_TYPE::TT_SPACE:
			//return 38;
		case TOKEN_TYPE::TT_CRLF:
		case TOKEN_TYPE::TT_EBRACKET:
		case TOKEN_TYPE::TT_OP:
		case TOKEN_TYPE::TT_EPAR:
		case TOKEN_TYPE::TT_END:
			return 39;		//��
		default:
			return -1;
		}
	//-----------------------------
	//�X�^�b�NA (����)
	//-----------------------------
	case STACK_TYPE::ST_A:
		switch (_in) {
		case TOKEN_TYPE::TT_SPAR:
			return 40;
		case TOKEN_TYPE::TT_STRING:
		case TOKEN_TYPE::TT_NUMBER:
		case TOKEN_TYPE::TT_TAB:
		case TOKEN_TYPE::TT_SPACE:
		case TOKEN_TYPE::TT_SBRACKET:
		case TOKEN_TYPE::TT_VBAR:
		case TOKEN_TYPE::TT_DOLL:
			return 42;
		case TOKEN_TYPE::TT_EPAR:
		case TOKEN_TYPE::TT_CRLF:
		case TOKEN_TYPE::TT_END:
			return 41;
		case TOKEN_TYPE::TT_OP:
			return 43;
		default:
			return -1;
		}
	//-----------------------------
	//�X�^�b�NA' (����)
	//-----------------------------
	case STACK_TYPE::ST_Ad:
		switch (_in) {
		case TOKEN_TYPE::TT_EPAR:
			return 44;
		case TOKEN_TYPE::TT_DOLL:
		case TOKEN_TYPE::TT_VBAR:
		case TOKEN_TYPE::TT_NUMBER:
			return 45;
		case TOKEN_TYPE::TT_STRING:
			return 46;
		case TOKEN_TYPE::TT_SPACE:
			return 47;
		case TOKEN_TYPE::TT_SBRACKET:
			return 49;
		case TOKEN_TYPE::TT_OP:
			return 48;
		default:
			return -1;
		}
	//-----------------------------
	//�X�^�b�N$ �I�[�L��
	//-----------------------------
	case STACK_TYPE::ST_$:
	case STACK_TYPE::ST_TOKEN:
		//�X�^�b�N�Ɠ��͂���擪����菜���m�[�h�ŊǗ�����֐����Ăяo��
		return 0;
		break;

	default:
		break;
	}
	return -1;		//�\���G���[
}

//============================================
//���[�����ƂɃX�^�b�N�̏������s���֐�
//============================================
void PARSE::Rules(int _rulenum) {
	//STACK st;		//�ǉ�����X�^�b�N�̏����i�[����ϐ�
	for (int i = 0; i < token_stack.size(); i++) {
		cout << "��菜���g�[�N��" << i << ":" << (int)token_stack[i] << endl;
	}
	switch (_rulenum)
	{
	case 0:		//�X�^�b�N�Ɠ��͎�菜��
		if (token_stack.size() > 0 && input[0].type == token_stack.back()) {
			cout << "��菜�����g�[�N��:" << (int)token_stack.back() << endl;
			if (order.size() == 0 || order.back().token != nowToken) {
				ORDER tmp;
				tmp.token = nowToken;
				order.push_back(tmp);
			}
			else if (token_stack.back() == TOKEN_TYPE::TT_EPAR) {
				ORDER tmp;
				tmp.token = ORDER_TOKEN::END;
				order.push_back(tmp);
			}
			order.back().script += input.front().str;		//���͎擾
			stack.erase(stack.begin());
			input.erase(input.begin());
			token_stack.pop_back();
		}
		else {
			//�G���[
			cout << "\033[31m";
			cout << "�s��v\n����:" << (int)input[0].type << "	�X�^�b�N:" << (int)token_stack.back() << endl;

			getchar();
			for (int i = 0; i < token_stack.size(); i++) {
				cout << "�X�^�b�N" << i << ":" << (int)token_stack[i] << endl;
			}
			cout << "\033[m";

			getchar();
		}
		break;
	case 1:		//1.S��LS'
		stack[0] = STACK_TYPE::ST_Sd;						//���擪�X�^�b�N��S��S'�ɕϊ�
		stack.insert(stack.begin(), STACK_TYPE::ST_L);		//�Ő擪��L��
		break;
	case 2:		//2.S��CRLF LS'
		stack[0] = ST_Sd;		//���擪�X�^�b�N��S��S'�ɕϊ�
		stack.insert(stack.begin(), STACK_TYPE::ST_L);
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);	//�擪���g�[�N��
		token_stack.push_back(TOKEN_TYPE::TT_CRLF);			// ���s���g�[�N���X�^�b�N��
		break;
	case 3:		//3.S'����
		stack.erase(stack.begin());		//�擪�X�^�b�N���폜
		break;
	case 4:		//4.L����
		stack.erase(stack.begin());		//�擪�X�^�b�N�폜
		break;
	case 5:		//5.L��# F
		stack[0] = STACK_TYPE::ST_F;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SHARP);			// #
		nowToken = ORDER_TOKEN::LABEL;		//���x�����L�^
		break;
	case 6:		//6.L��E
		stack[0] = STACK_TYPE::ST_E;
		break;
	case 7:	//7.L��TAB T
		stack[0] = STACK_TYPE::ST_T;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// TAB
		token_stack.push_back(TOKEN_TYPE::TT_TAB);			// TAB
		//���߃p�^�[���𖼑O�t���e�L�X�g
		nowToken = ORDER_TOKEN::STRING;
		break;
	case 8:	//8.L��T TAB T
		stack[0] = STACK_TYPE::ST_T;							// T
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// TAB
		stack.insert(stack.begin(), STACK_TYPE::ST_T);			// T
		token_stack.push_back(TOKEN_TYPE::TT_TAB);			// TAB
		//���߃p�^�[�����e�L�X�g�̂�
		nowToken = ORDER_TOKEN::STRING;
		break;
	case 9:		//9.E��(E)E'
		stack[0] = STACK_TYPE::ST_Ed;							// E'
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// )
		stack.insert(stack.begin(), STACK_TYPE::ST_E);			// E
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// (
		token_stack.push_back(TOKEN_TYPE::TT_EPAR);				// )
		token_stack.push_back(TOKEN_TYPE::TT_SPAR);				// (
		//���߃p�^�[����(��)�Ƃ��čŗD��
		break;
	case 10:	//10.E��[F]A
		stack[0] = ST_A;							// A
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// ]
		stack.insert(stack.begin(), STACK_TYPE::ST_F);			// F
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// [
		token_stack.push_back(TOKEN_TYPE::TT_EBRACKET);			// ]
		token_stack.push_back(TOKEN_TYPE::TT_SBRACKET);			// [
		//���߃p�^�[�����֐��ďo
		nowToken = ORDER_TOKEN::FUNCTION;
		break;
	case 11:		//11.E��VE'
		stack[0] = STACK_TYPE::ST_Ed;
		stack.insert(stack.begin(), STACK_TYPE::ST_V);
		//���߃p�^�[����ϐ���
		break;
	case 12:	//12.E��spE
		stack[0] = STACK_TYPE::ST_E;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SPACE);			// SPACE
		break;
	case 13:	//13.E'����
		stack.erase(stack.begin());					//�X�^�b�N�I��
		//���߃p�^�[���̏I�[
		break;
	case 14:	//14.E'��O
		stack[0] = STACK_TYPE::ST_O;		// ���Z�q
		//���߃p�^�[���ɉ��Z�q��ǉ�
		nowToken = ORDER_TOKEN::OPERATION;
		break;
	case 15:	//15.E'��spE'
		stack[0] = STACK_TYPE::ST_Ed;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// SPACE
		token_stack.push_back(TOKEN_TYPE::TT_SPACE);			// SPACE
		break;
	case 16:	//16.O��opE
		stack[0] = STACK_TYPE::ST_E;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		//���Z�q
		token_stack.push_back(TOKEN_TYPE::TT_OP);			//���Z�q
		//�E�ӂ̎���ǉ�
		break;
	case 17:	//17.O��spO
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		//��
		token_stack.push_back(TOKEN_TYPE::TT_SPACE);
	case 18:	//18.V��$F		//�ϐ�
		stack[0] = ST_F;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		//�g�[�N��
		token_stack.push_back(TOKEN_TYPE::TT_DOLL);				// $
		//�O���[�o���ϐ��ďo
		nowToken = ORDER_TOKEN::VARIABLE;
		break;
	case 19:	//19.V��|F		//�ϐ�
		stack[0] = ST_F;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		//�g�[�N��
		token_stack.push_back(TOKEN_TYPE::TT_VBAR);				// |
		//���[�J���ϐ��ďo
		nowToken = ORDER_TOKEN::VARIABLE;
		break;
	case 20:	//20.V��num		//���l
		stack[0] = STACK_TYPE::ST_TOKEN;
		token_stack.push_back(TOKEN_TYPE::TT_NUMBER);			// number
		nowToken = ORDER_TOKEN::VALUE;
		break;
	case 21:	//21.T��strT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_STRING);			// string
		//�����񖽗߃p�^�[���ɒǉ�
		break;
	case 22:	//22.T��numT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_NUMBER);			// number
		//�����񖽗߃p�^�[���ɒǉ�
		break;
	case 23:	//23.T'��strT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_STRING);			// string
		//�����񖽗߃p�^�[���ɒǉ�
		break;
	case 24:	//24.T'��numT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_NUMBER);			// number
		//�����񖽗߃p�^�[���ɒǉ�
		break;
	case 25:	//25.T'����
		stack.erase(stack.begin());
		//�����񖽗߃p�^�[���I��
		break;
	case 26:	//26.T'��sp��
		//stack.erase(stack.begin());
		stack[0] = STACK_TYPE::ST_TOKEN;		
		token_stack.push_back(TOKEN_TYPE::TT_SPACE);
		//�����񖽗߃p�^�[���I��
		break;
	case 27:	//27.T'��#T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SHARP);			// #���g�[�N���X�^�b�N��
		break;
	case 28:	//28.T'��(T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SPAR);			// (���g�[�N���X�^�b�N��
		break;
	case 29:	//29.T'��)T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_EPAR);			// )���g�[�N���X�^�b�N��
		break;
	case 30:	//30.T'��[T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SBRACKET);			// [���g�[�N���X�^�b�N��
		break;
	case 31:	//31.T'��]T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_EBRACKET);			// ]���g�[�N���X�^�b�N��
		break;
	case 32:	//32.T'��|T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_VBAR);			// |���g�[�N���X�^�b�N��
		break;
	case 33:	//33.T'��$T'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_DOLL);			// $���g�[�N���X�^�b�N��
		break;
	case 34:	//34.T'��opT'
		stack[0] = STACK_TYPE::ST_Td;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_OP);			// ���Z�q���g�[�N���X�^�b�N��
		break;
	case 35:	//35.F��strF'
		stack[0] = STACK_TYPE::ST_Fd;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_STRING);			// string
		break;
	case 36:	//36.F'��strF'
		stack[0] = STACK_TYPE::ST_Fd;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_STRING);			// string
		break;
	case 37:	//37.F'��numF'
		stack[0] = STACK_TYPE::ST_Fd;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_NUMBER);			// number
		break;
	case 38:	//38.F'��sp��
		stack.erase(stack.begin());
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SPACE);			// SPACE
		break;
	case 39:	//39.F'����
		stack.erase(stack.begin());
		break;
	case 40:	//40.A��(A')E'
		stack[0] = STACK_TYPE::ST_Ed;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		stack.insert(stack.begin(), STACK_TYPE::ST_Ad);
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_EPAR);			// )
		token_stack.push_back(TOKEN_TYPE::TT_SPAR);			// (
		nowToken = ORDER_TOKEN::ARGUMENT;
		break;
	case 41:	//41.A����
		stack.erase(stack.begin());
		break;
	case 42:	//42.A��E'
		stack[0] = STACK_TYPE::ST_Ed;
		break;
	case 43:	//43.A��opE
		stack[0] = ST_E;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_OP);
		nowToken = ORDER_TOKEN::OPERATION;		//�֐� ���Z�q ���̐���
		break;
	case 44:	//44.A'���� ��������
		stack.erase(stack.begin());
		break;
	case 45:	//45.A'��VA'  �ϐ��܂��͐��l
		stack[0] = STACK_TYPE::ST_Ad;
		stack.insert(stack.begin(), STACK_TYPE::ST_V);
		break;
	case 46:	//46.A'��TA'	������
		stack[0] = STACK_TYPE::ST_Ad;
		stack.insert(stack.begin(), STACK_TYPE::ST_T);
		break;
	case 47:	//47.A'��spA'	//�����̋�؂�
		stack[0] = STACK_TYPE::ST_Ad;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_SPACE);			// SPACE
		nowToken = ORDER_TOKEN::END;
		break;
	case 48:	//48.A'��opA'
		stack[0] = STACK_TYPE::ST_Ad;
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);
		token_stack.push_back(TOKEN_TYPE::TT_OP);			// OP
		nowToken = ORDER_TOKEN::OPERATION;
		break;
	case 49:	//49.A'��[F]AA'
		stack[0] = ST_Ad;							// A'
		stack.insert(stack.begin(), STACK_TYPE::ST_A);			// A
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// ]
		stack.insert(stack.begin(), STACK_TYPE::ST_F);			// F
		stack.insert(stack.begin(), STACK_TYPE::ST_TOKEN);		// [
		token_stack.push_back(TOKEN_TYPE::TT_EBRACKET);			// ]
		token_stack.push_back(TOKEN_TYPE::TT_SBRACKET);			// [
		nowToken = ORDER_TOKEN::FUNCTION;
		break;
	default:
		//�G���[ ���[�����Ȃ�
		break;
	}
}

void PARSE::Tree(STACK_TYPE _stack, TOKEN _token) {
	if (root == nullptr) {
		root = new NODE;
	}
	else {
		//root->child.push_back(new NODE);		//�q�̃m�[�h�Ƀm�[�h�쐬
	}
	root->stack = _stack;		//�X�^�b�N���L�^
	root->token = _token;		//�g�[�N�����L�^
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
		//if (input[0].type == TOKEN_TYPE::TT_SPACE) {
		//	//�X�y�[�X�ł���Γǂݔ�΂�
		//	input.erase(input.begin());
		//}

		int rulesnum = RulesNum(stack[0], input[0].type);		//�X�^�b�N�ƃg�[�N���̐擪���דK�����ׂ����[�����擾����
		if (rulesnum == -1) {
			wcout << L"\033[31m";
			cout << "ERROR" << (int)input[0].type;
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
		cout << stack[0] << "	" << (int)input[0].type << "	" << rulesnum << "	";
		OutPut();
		cout << "\r\n";
		Rules(rulesnum);		//��͕\����擾�������[���ԍ��ɂ������������s��

		if (stack[0] == ST_$) {
			if (input[0].type == TOKEN_TYPE::TT_END) {
				//�\����͐���I��
				break;
			}
			else {
				//���͂��c���Ă���
				wcout << L"\033[31m";

				cout << "�G���[:���͒l���c���Ă���" << endl;
				wcout << L"\033[m";

				return;
			}
		}

	}
	wcout << "\n\n";

	for (int i = 0; i < order.size(); i++) {
		wcout << to_wstring((int)order[i].token) << ':' << order[i].script << endl;
	}
	order.clear();
}
