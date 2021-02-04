#include "SEMANTIC.h"
#include <iostream>

template<typename T>
vector<char> ToChar(T data);

void SEMANTIC::Soushajo(vector<ORDER> _in) {
	vector<TOKEN_YUSEN> stack;		//���Z�q�X�^�b�N
	vector<ORDER> out;		//�o��
	bool argument = false;		//�������ǂ���
	//-------------------------------------------------------
	//�t�|�[�����h�L�@�ɒu��������
	//-------------------------------------------------------
	for (int i = 0; i < _in.size(); i++) {
		TOKEN_YUSEN tmp;
		bool stackFlag = false;		//�X�^�b�N�������̂��ǂ���

		if (!argument) {
			switch (_in[i].token)
			{
			case ORDER_TOKEN::LABEL:
				//�����x���͒n�_�L�^�Ȃ̂ŏ����Ȃ��A�����߂͔F�߂��I��
				out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::FUNCTION:
				//�D��x���� �o�͂֒��ړ�����
				//tmp.code = OP_CODE::FUNC;
				//tmp.yuusendo = 4;
				out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::ARGUMENT:
				//�D��x���� ���O�̊֐��̈���
				out.push_back(_in[i]);
				argument = true;
				break;
			case ORDER_TOKEN::VALUE:
				//�D��x���� �o�͂֒��ڃv�b�V��
				out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::VARIABLE:
				//�D��x���� �o�͂֒��ڃv�b�V�� ���s���錾�ƎQ�Ƃ��s��
				out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::OPERATION:
				switch (_in[i].script.front()) {		//���Z�q�̓��e�𒲂ׂ�
				case '=':
					tmp.yuusendo = 1;
					tmp.code = OPERATOR::SUBS << 4;			//�������
					break;
				case '+':
					tmp.yuusendo = 2;
					tmp.code = OPERATOR::ADD << 4;			//���Z����
					break;
				case '-':
					tmp.yuusendo = 2;
					tmp.code = OPERATOR::SUBT << 4;
					break;
				case '*':
					tmp.yuusendo = 4;
					tmp.code = OPERATOR::MUL;
					break;
				case '/':
					tmp.yuusendo = 4;
					tmp.code = OPERATOR::DIV;
					break;
				default:
					break;
				}
				tmp.code = tmp.code | OP_CODE::OP;
				tmp.token = _in[i];

				stackFlag = true;

				break;
			case ORDER_TOKEN::STRING:
				out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::PAR:
				tmp.token = _in[i];
				stackFlag = true;
				break;
			case ORDER_TOKEN::END:
				//��؂蕶������

				//)�͓��ꏈ��
				if (_in[i].script == L")") {
					//(������܂ŃX�^�b�N�����o��
					for (int j = stack.size() - 1; j >= 0; j--) {
						if (stack[j].token.token != ORDER_TOKEN::PAR) {
							out.push_back(stack[j].token);
							stack.pop_back();
						}
						else {
							stack.pop_back();
							break;
						}
					}
				}
				else {
					continue;
				}
				break;
			default:
				break;
			}
		}
		else {		//�����ł����
			switch (_in[i].token) {
			case ORDER_TOKEN::STRING:
				tmp.code = OP_CODE::TEXT;
				break;
			case ORDER_TOKEN::FUNCTION:
				tmp.code = OP_CODE::FUNC;
				break;
			case ORDER_TOKEN::ARGUMENT:
				//�G���[
				break;
			case ORDER_TOKEN::VALUE:
				tmp.code = OP_CODE::VAL;
				break;
			case ORDER_TOKEN::VARIABLE:
				tmp.code = OP_CODE::VAR;
				break;
			case ORDER_TOKEN::OPERATION:
				//���Z�q�ȉ��ɕ����ď������s��*/
				if (_in[i].script.size() > 1) {
					//�񕶎��ō\������鉉�Z�q
					switch (_in[i].script.front()) {
					case '=':
						tmp.code = OPERATOR::EQUAL << 4;
						break;
					case '<':
						tmp.code = OPERATOR::GEQUAL << 4;
						break;
					case '>':
						tmp.code = OPERATOR::SEQUAL << 4;
						break;
					case '!':
						tmp.code = OPERATOR::NOT << 4;
						break;
					default:
						//�G���[ ������Ă��Ȃ����Z�q
						wcout << L"�G���[:" << _in[i].script << L"�͉��Z�q�Ƃ��ċ��e����Ă��܂���B" << endl;
						getchar();
						break;
					}
					//���̕�����=���ǂ���
					if (_in[i].script[1] == '=') {
						//���Ȃ��Ȃ̂ł���Ŋm��
					}
					else {
						//�G���[ =�ȊO������d�l�゠�肦�Ȃ�
						wcout << L"�G���[:" << _in[i].script << L"�͉��Z�q�Ƃ��ċ��e����Ă��܂���B" << endl;
					}
				}//if_in[i].script.size() > 1)
				else {
					//���Z�q����̏ꍇ��������
					switch (_in[i].script.front())
					{
					case '<':
						tmp.code = OPERATOR::SMALL << 4;
						break;
					case '>':
						tmp.code = OPERATOR::GREATER << 4;
						break;
					default:
						break;
					}
				}
				tmp.code = tmp.code | OP_CODE::OP;			//��r���Z�q
			case ORDER_TOKEN::END:
				if (_in[i].script == L")") {
					argument = false;
				}
				break;
			}

			out.push_back(_in[i]);
		}

		if (stackFlag) {
			//�X�^�b�N������o������
			if (stack.size() > 0 && stack.back().yuusendo >= tmp.yuusendo) {
				//���o�����̂������
				for (int j = stack.size() - 1; j >= 0; j--) {
					//()�͏������Ȃ�
					if (tmp.token.token == ORDER_TOKEN::PAR) {
						break;
					}

					//�X�^�b�N�̗D��x��茻�݂̗D��x���Ⴂ�������͓��������
					if (stack[j].yuusendo >= tmp.yuusendo) {
						out.push_back(stack.back().token);
						stack.pop_back();
					}
				}
			}
			if (tmp.token.token != ORDER_TOKEN::END) {
				stack.push_back(tmp);
			}
		}

		//-----------------------------------------------------
		//�o�͂̌��ʂ�\��
		//-----------------------------------------------------
#if SYSTEM_MESSAGE >= 2

		wcout << "out:";
		for (int j = 0; j < out.size(); j++) {
			wcout << out[j].script;
		}
		wcout << endl;
		wcout << "stack:";
		for (int j = stack.size() - 1; j >= 0; j--) {
			wcout << stack[j].token.script;
		}
		wcout << endl;
#endif // SYSTEM_MESSAGE

		//wcout << endl;
		//�o�͂��t�@�C���Ƃ��ď����o�����������Ă���
	}

	//�X�^�b�N�̎c�������
	for (int i = stack.size() - 1; i >= 0; i--) {
		out.push_back(stack[i].token);
		stack.pop_back();
	}

	/*for (int i = 0; i < _in.size(); i++) {
		wcout << _in[i].script;
	}*/
#if SYSTEM_MESSAGE >= 1
	wcout << L"����:";
	for (int i = 0; i < out.size(); i++) {
		wcout << out[i].script << ' ';
	}
	wcout << endl;
#endif
	result.push_back(out);
}

//================================================
//���߂̏��o��
//================================================
void SEMANTIC::Compile() {
	vector<char> byte;		//1�o�C�g�f�[�^���i�[����
	//���o���p�̓��e��
	for (int i = 0; i < result.size(); i++) {
		for (int j = 0; j < result[i].size(); j++) {
			//i+1���X�N���v�g�t�@�C���̌��ݓǂ�ł���s
			//�o�C�g�ϊ�
			switch (result[i][j].token) {
			case ORDER_TOKEN::LABEL:
				//���x���ł����

			default:
				break;
			}
		}
	}

	//�t�@�C���̍쐬
	ofstream file;
	file.open("SCRIPT.ric", ios::out | ios::binary | ios::trunc);		//�t�@�C���̐V�K�쐬

	if (!file) {
		cout << "�X�N���v�g�t�@�C�����J���܂���" << endl;
		return;
	}
	byte = ToChar(2);		//�e�X�g
	//for (int i = 0; i < orderList.size(); i++) {
	//	for (int j = 0; j < orderList[i].size(); j++) {
	//		//file.write((char*)&orderList[i], sizeof(char));
	//	}
	//}
	for(int i = 0;i<result.size();i++){
	}
}

void SEMANTIC::SetScript(vector<vector<ORDER>> _order) {
	orderList = _order;
}

void SEMANTIC::Analysis() {
	for (int i = 0; i < orderList.size(); i++) {
		Soushajo(orderList[i]);
	}
	//���s���ʂ����Ƃɖ��߃f�[�^���쐬
	Compile();
}

SEMANTIC& SEMANTIC::GetInstance()
{
	static SEMANTIC *instance = new SEMANTIC;
	return *instance;
}

SEMANTIC::SEMANTIC() {

}

//=========================================
//1�o�C�g�f�[�^�ɕϊ�����֐�
//=========================================
template<typename T>
vector<char> ToChar(T data) {
	int dataSize = sizeof(data);		//���o�C�g�̃f�[�^���𒲂ׂ�
	vector<char> result;				//�o�C�g�������ꂽ�f�[�^
	//�f�[�^�̃o�C�g���ɒB����܂Ń��[�v����
	for (int i = 0; i < dataSize; i++) {
		char byteData = data & 0xFF;		//1�o�C�g�������o��
		data = data >> 8;		//�����ς݃r�b�g�͔j��
		//���U���g�Ƀf�[�^���i�[
		result.push_back(byteData);
	}
	return result;		//������
}
