#include "StringOperation.h"

//�w�蕶������
vector<wstring> StringOperation::Split(wstring str, wchar_t delimiter) {
	//find�֐���p���ď������s��
	int first = 0;		//�������Ă��镶����̐擪
	int last = str.find_first_of(delimiter);	//��؂蕶�����o���ʒu

	vector<wstring> result;		//��������
	
	if (last != wstring::npos) {
		//first������������������ԃ��[�v
		while (first < str.size()) {
			wstring subStr(str, first, last - first);
			result.push_back(subStr);
			first = last + 1;		//�w�蕶���̎��̕������w��
			last = str.find_first_of(delimiter, first);		//���̕����ʒu����݂Ď��̋�؂蕶���̈ʒu��Ԃ�

			//��؂蕶����������Ȃ����
			if (last == wstring::npos) {
				last = str.size();		//�Ō�̕����ʒu��
			}
		}
	}
	else {
		result.push_back(str);		//��؂蕶�����Ȃ��̂Ō��ʂɓ���ĕԂ�
	}
	return result;		//�擾�������ʂ�Ԃ�
}

//�w�蕶������
vector<string> StringOperation::Split(string str, char delimiter) {
	//find�֐���p���ď������s��
	int first = 0;		//�������Ă��镶����̐擪
	int last = str.find_first_of(delimiter);	//��؂蕶�����o���ʒu

	vector<string> result;		//��������

	//first������������������ԃ��[�v
	while (first < str.size()) {
		string subStr(str, first, last - first);
		result.push_back(subStr);
		first = last + 1;		//�w�蕶���̎��̕������w��
		last = str.find_first_of(delimiter, first);		//���̕����ʒu����݂Ď��̋�؂蕶���̈ʒu��Ԃ�

		//��؂蕶����������Ȃ����
		if (last == string::npos) {
			last = str.size();		//�Ō�̕����ʒu��
		}
	}
	return result;		//�擾�������ʂ�Ԃ�
}