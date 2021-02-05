#pragma once
#include "LEX.h"

#define RULES (20)		//���[���̐�

enum STACK_TYPE {
	ST_S = 0,	//�X�N���v�g(�J�n)
	ST_Sd,		//�X�N���v�g S'
	ST_L,		//�P��
	ST_T,		//����
	ST_Td,		//���� T'
	ST_E,		//��
	ST_Ed,		//�� E'
	ST_O,		//�Z�p���Z�q
	ST_V,		//�ϐ�/���l
	ST_F,		//�֐�/�ϐ����擪����
	ST_Fd,		//�֐�/�ϐ����擪�ȍ~����
	ST_A,		//����
	ST_Ad,		//����
	ST_P,		//��؂蕶��
	ST_TOKEN,	//��菜���g�[�N��
	ST_$,		//�I�[�L��
};

//---------------------------------------------------------
//���߂̃g�[�N���P�ʎ��
//---------------------------------------------------------
typedef enum class _order {
	STRING,		//������
	FUNCTION,	//�֐�
	LABEL,		//���x��
	VALUE,		//�l
	GVARIABLE,	//�O���[�o���ϐ�
	LVARIABLE,	//���[�J���ϐ�
	OPERATION,	//���Z�q
	ARGUMENT,	//����
	PAR,		//(
	END,		//��؂蕶���⊇�ʏI���
}ORDER_TOKEN;

//---------------------------------------------------------
//1���߂̃f�[�^
//---------------------------------------------------------
typedef struct {
	string script;		//1�g�[�N���̃X�N���v�g�̓��e
	ORDER_TOKEN token;	//���
}ORDER;

////-------------------------
////�m�[�h�\����
////-------------------------
//typedef struct _Node {
//	TOKEN			token;		//�m�[�h�̓��e
//	STACK_TYPE		stack;		//�X�^�b�N�̓��e
//	_Node*			parent;		//�e�m�[�h
//	vector<_Node*>  child;		//�q�m�[�h
//}NODE;


//==============================
//�\����̓N���X
//==============================
class PARSE {
struct STACK {
	STACK_TYPE	stack;
	TOKEN		token;
};

private:
	static PARSE Instance;
//	NODE* root;		//���[�g�m�[�h
	vector<STACK_TYPE> stack;		//�X�^�b�N
	vector<TOKEN_TYPE> token_stack;	//���o���g�[�N��
	vector<TOKEN> input;		//����
	vector<ORDER> order;		//��s�̖��߈ꗗ
	vector<vector<ORDER>> orderList;		//�\����͌�̖��߈ꗗ
	ORDER_TOKEN nowToken;		//���ݔ��ʂ��Ă���g�[�N��

	PARSE();
	int RulesNum(STACK_TYPE _stack,TOKEN_TYPE _in);		//��͕\����Ή����郋�[�������ʂ���֐�
	void Rules(int _rulenum);						//��͕\����擾�������[�������s���ăX�^�b�N�ɕԂ��֐�
	void Tree(STACK_TYPE _stack, TOKEN _token);					//���o�����X�^�b�N����\���؂��쐬
	void OutPut();
public:
	static PARSE &GetInstance();
	vector<vector<ORDER>> GetOrder();
	void Analysis(vector<TOKEN> _tokenList);		//��͏���
//	NODE GetSyntaxTree();	//�\���؂��擾
};

