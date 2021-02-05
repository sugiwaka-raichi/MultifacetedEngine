#include "SEMANTIC.h"
#include <iostream>

template<typename T>
vector<char> ToByte(T _data);

void SEMANTIC::Soushajo(vector<ORDER> _in) {
	vector<ORDER_PRIORITY> stack;		//���Z�q�X�^�b�N
	vector<ORDER_DATA> out;		//�o��
	int argument = 0;		//�����̐�
	//-------------------------------------------------------
	//�t�|�[�����h�L�@�ɒu��������
	//-------------------------------------------------------
	for (int i = 0; i < _in.size(); i++) {
		ORDER_PRIORITY tmp;
		ORDER_DATA orderData;
		bool stackFlag = false;		//�X�^�b�N�������̂��ǂ���

		if (argument == 0) {
			switch (_in[i].token)
			{
			case ORDER_TOKEN::LABEL:
				//�����x���͒n�_�L�^�Ȃ̂ŏ����Ȃ��A�����߂͔F�߂��I��
				orderData.token = _in[i];
				orderData.code = OP_CODE::LABEL;
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::FUNCTION:
				//�D��x���� �o�͂֒��ړ�����
				//tmp.code = OP_CODE::FUNC;
				//tmp.yuusendo = 4;
				orderData.token = _in[i];
				orderData.code = OP_CODE::FUNC;
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::ARGUMENT:
				//�D��x���� ���O�̊֐��̈���
				orderData.token = _in[i];
				orderData.code = OP_CODE::ARG;
				out.push_back(orderData);
				argument++;
				break;
			case ORDER_TOKEN::VALUE:
				//�D��x���� �o�͂֒��ڃv�b�V��
				orderData.token = _in[i];
				orderData.code = OP_CODE::VAL;
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::GVARIABLE:
				orderData.token = _in[i];
				orderData.code = OP_CODE::VAR | (VAR_TYPE::GLOBAL << 4);
				out.push_back(orderData);
				break; 
			case ORDER_TOKEN::LVARIABLE:
				//�D��x���� �o�͂֒��ڃv�b�V�� ���s���錾�ƎQ�Ƃ��s��
				orderData.token = _in[i];
				orderData.code = OP_CODE::VAR | (VAR_TYPE::LOCAL << 4);
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::OPERATION:
				switch (_in[i].script.front()) {		//���Z�q�̓��e�𒲂ׂ�
				case '=':
					tmp.priority = 1;
					tmp.order.code = OPERATOR::SUBS << 4;			//�������
					break;
				case '+':
					tmp.priority = 2;
					tmp.order.code = OPERATOR::ADD << 4;			//���Z����
					break;
				case '-':
					tmp.priority = 2;
					tmp.order.code = OPERATOR::SUBT << 4;
					break;
				case '*':
					tmp.priority = 4;
					tmp.order.code = OPERATOR::MUL;
					break;
				case '/':
					tmp.priority = 4;
					tmp.order.code = OPERATOR::DIV;
					break;
				default:
					break;
				}
				tmp.order.code = tmp.order.code | OP_CODE::OP;
				tmp.order.token = _in[i];

				stackFlag = true;

				break;
			case ORDER_TOKEN::STRING:
				orderData.token = _in[i];
				orderData.code = OP_CODE::VAR;
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::PAR:
				tmp.order.token = _in[i];
				stackFlag = true;
				break;
			case ORDER_TOKEN::END:
				//��؂蕶������

				//)�͓��ꏈ��
				if (_in[i].script == L")") {
					//(������܂ŃX�^�b�N�����o��
					for (int j = stack.size() - 1; j >= 0; j--) {
						if (stack[j].order.token.token != ORDER_TOKEN::PAR) {
							//(�łȂ���Ώo�͂փX�^�b�N�̓��e������
							out.push_back(stack[j].order);
							stack.pop_back();
						}
						else {
							//(�ł���Ύ�菜���ďI��
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
				tmp.order.code = OP_CODE::TEXT;
				break;
			case ORDER_TOKEN::FUNCTION:
				tmp.order.code = OP_CODE::FUNC;
				break;
			case ORDER_TOKEN::ARGUMENT:
				argument++;
				break;
			case ORDER_TOKEN::VALUE:
			case ORDER_TOKEN::GVARIABLE:
				//out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::OPERATION:
				//���Z�q�ȉ��ɕ����ď������s��*/
				if (_in[i].script.size() > 1) {
					//�񕶎��ō\������鉉�Z�q
					switch (_in[i].script.front()) {
					case '=':
						tmp.order.code = OPERATOR::EQUAL << 4;
						break;
					case '<':
						tmp.order.code = OPERATOR::GEQUAL << 4;
						break;
					case '>':
						tmp.order.code = OPERATOR::SEQUAL << 4;
						break;
					case '!':
						tmp.order.code = OPERATOR::NOT << 4;
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
						tmp.order.code = OPERATOR::SMALL << 4;
						break;
					case '>':
						tmp.order.code = OPERATOR::GREATER << 4;
						break;
					default:
						break;
					}
				}
				tmp.order.code = tmp.order.code | OP_CODE::OP;			//��r���Z�q
				break;
			case ORDER_TOKEN::END:
				if (_in[i].script == L")") {
					argument--;		//���������炷
				}
				break;
			}

			//out.push_back(_in[i]);
		}

		if (stackFlag) {
			//�X�^�b�N������o������
			if (stack.size() > 0 && stack.back().priority >= tmp.priority) {
				//���o�����̂������
				for (int j = stack.size() - 1; j >= 0; j--) {
					//()�͏������Ȃ�
					if (tmp.order.token.token == ORDER_TOKEN::PAR) {
						break;
					}
					if (stack[j].order.token.token == ORDER_TOKEN::PAR) {
						break;
					}

					//�X�^�b�N�̗D��x��茻�݂̗D��x���Ⴂ�������͓��������
					if (stack[j].priority >= tmp.priority) {
						out.push_back(stack.back().order);
						stack.pop_back();
					}
				}
			}
			if (tmp.order.token.token != ORDER_TOKEN::END) {
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
		wcout << L"����:" << argument << endl;
#endif // SYSTEM_MESSAGE

		//wcout << endl;
		//�o�͂��t�@�C���Ƃ��ď����o�����������Ă���
	}

	//�X�^�b�N�̎c�������
	for (int i = stack.size() - 1; i >= 0; i--) {
		out.push_back(stack[i].order);
		stack.pop_back();
	}

	/*for (int i = 0; i < _in.size(); i++) {
		wcout << _in[i].script;
	}*/
#if SYSTEM_MESSAGE >= 1
	wcout << L"����:";
	for (int i = 0; i < out.size(); i++) {
		wcout << out[i].token.script << ' ';
	}
	wcout << endl;
#endif
	result.push_back(out);
}

//================================================
//���߂̏��o��
//================================================
void SEMANTIC::Compile() {
	vector<char> byte;		//�����o�����e��1�o�C�g�f�[�^���i�[����
	//�w�b�_�[�����쐬
	{
		//ToDo: �X�N���v�g�t�@�C�����̂̏��
		//�X�N���v�g�t�@�C�����̒���
		//�X�N���v�g�t�@�C��������
		//���X�N���v�g�t�@�C���̎��ʎq��������΂�������?
		//���œK���͗]�T�o��������ɂł�
		//ToDo: end
		vector<char> Lines = ToByte(result.size());		//�s�����o�C�g�P�ʂɒ������f�[�^
		byte.insert(byte.end(), Lines.begin(), Lines.end());
	}
	//���o���p�̓��e��
	for (int i = 0; i < result.size(); i++) {
		vector<char> dataSize = ToByte(i+1);		//�L�^����s�����o��
		byte.insert(byte.end(), dataSize.begin(), dataSize.end());		//�s���L�^
		for (int j = 0; j < result[i].size(); j++) {
			vector<char> temp;
			//i+1���X�N���v�g�t�@�C���̌��ݓǂ�ł���s
			//�o�C�g�ϊ�
			switch (result[i][j].code & 0x0F) {		//���4bit�͍l�����Ȃ�
			case OP_CODE::TEXT:
				//������
				temp = ToByte(1 + sizeof(wchar_t) * result[i][j].token.script.size());
				byte.insert(byte.end(), temp.begin(), temp.end());
				byte.push_back(result[i][j].code);
				for (int k = 0; k < result[i][j].token.script.size(); k++) {
					temp = ToByte(result[i][j].token.script[k]);			//������1�o�C�g�f�[�^�ɕ�����
					byte.insert(byte.end(), temp.begin(), temp.end());		//�����f�[�^���L�^
				}
				break;
			case OP_CODE::VAL:
				//���l
				temp = ToByte(1 + sizeof(wchar_t) * result[i][j].token.script.size());
				byte.insert(byte.end(), temp.begin(), temp.end());
				byte.push_back(result[i][j].code);
				for (int k = 0; k < result[i][j].token.script.size(); k++) {
					temp = ToByte(result[i][j].token.script[k]);			//������1�o�C�g�f�[�^�ɕ�����
					byte.insert(byte.end(), temp.begin(), temp.end());		//�����f�[�^���L�^
				}
			case OP_CODE::FUNC:
				//�֐�
				break;
			case OP_CODE::ARG:
				//����
				break;
			case OP_CODE::OP:
				//���Z�q
				byte.push_back(1);		//���Z�q�͖��ߊT�v��1�o�C�g�Ŋ�������
				byte.insert(byte.end(), temp.begin(), temp.end());		//1���߂̃f�[�^�T�C�Y
				byte.push_back(result[i][j].code);		//���ߊT�v
				break;
			case OP_CODE::LABEL:
				//���x���ł����
				//�ꖽ�߂̃f�[�^�T�C�Y ���ߊT�v1�o�C�g ���x����n�o�C�g
				temp = ToByte(1 + sizeof(wchar_t) * result[i][j].token.script.size());
				byte.insert(byte.end(), temp.begin(), temp.end());
				byte.push_back(result[i][j].code);		//���ߊT�v:���x�����L�^
				for (int k = 0; k < result[i][j].token.script.size(); k++) {
					temp = ToByte(result[i][j].token.script[k]);			//������1�o�C�g�f�[�^�ɕ�����
					byte.insert(byte.end(), temp.begin(), temp.end());		//�����f�[�^���L�^
				}
			default:
				break;
			}
		}
	}

	for (int i = 0; i < byte.size(); i++) {
		cout << byte[i] << endl;
	}

	//�t�@�C���̍쐬
	ofstream file;
	file.open("SCRIPT.ric", ios::out | ios::binary | ios::trunc);		//�t�@�C���̐V�K�쐬

	if (!file) {
		cout << "�X�N���v�g�t�@�C�����J���܂���" << endl;
		return;
	}
	//for (int i = 0; i < orderList.size(); i++) {
	//	for (int j = 0; j < orderList[i].size(); j++) {
	//		//file.write((char*)&orderList[i], sizeof(char));
	//	}
	//}
	for(int i = 0;i<byte.size();i++){
		file.write(&byte[i] ,sizeof(char));
	}
	file.close();
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
template <typename T>
vector<char> ToByte(T _data) {
	int dataSize = sizeof(_data);			//�f�[�^�̃o�C�g���𒲂ׂ�
	vector<char> result;					//���ʂ̃f�[�^

	//�f�[�^�T�C�Y��4�o�C�g�͈̔͂ŋL�^
	for (int i = 0; i < dataSize; i++) {	//�f�[�^�T�C�Y�����[�v����6
		result.push_back(_data >> (dataSize - 1 - i) * 8);
	}
	return result;
}
