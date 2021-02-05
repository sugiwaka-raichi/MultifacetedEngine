#pragma once
#include <string>
#include <vector>
#include <fstream>

using namespace std;
#define SYSTEM_MESSAGE 	1	//0��\�� 1���ʕ\�� 2�S�\��

#ifdef UNICODE
#define string wstring
#else
#define string string
#endif

//================================
//�g�[�N���̎��
//================================
typedef enum class TOKEN_TYPE {
	TT_NULL = 0,		//null

	TT_STRING,			//������
	TT_VARIABLE,		//�ϐ�(��)
	TT_NUMBER,			//���l
	TT_SEPARATION,		//��؂蕶��(��)

	//-----------------
	//�L����
	TT_CRLF,			// ���s
	TT_SHARP,			// #
	TT_TAB,				// TAB
	TT_SPACE,			// SPACE
	TT_SPAR,			// (
	TT_EPAR,			// )
	TT_SBRACKET,		// [
	TT_EBRACKET,		// ]
	TT_VBAR,			// |
	TT_DOLL,			// $
	TT_NOT,				// !
	TT_WQORT,			// "

	//----------------
	//���Z�q
	TT_OP,			// ���Z�q
	TT_EQUAL,		// =
	TT_PLUS,		// +
	TT_MINUS,		// -
	TT_ASTERISK,	// *
	TT_SLASH,		// /
	TT_PERCENT,		// %
	TT_LESS,		// <
	TT_GREATOR,		// >

	TT_END			//�I�[�L��
};

//===========================
//�g�[�N����`
//===========================
typedef struct TOKEN {
	string str;
	TOKEN_TYPE type;
};

class LEX {
private:
	vector<TOKEN> tokenList;		//��͂�������̋L�^�ϐ�
	string str;						//��舵��������

	LEX();			//�R���X�g���N�^���O������̌Ăяo���֎~
	bool Symbol(char _symbol);		//�L�����菈��
	TOKEN_TYPE JudgeSymbol(char _symbol);	//�L�����ʏ���

public:
	static LEX& GetInstance();		//�C���X�^���X���擾����֐�
	void Analysis(string _str);		//��͊֐�
	vector<TOKEN> GetTokenList();	//�g�[�N�����X�g�p�Q�b�^�[

};