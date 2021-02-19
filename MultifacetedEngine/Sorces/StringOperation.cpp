#include "StringOperation.h"

//�w�蕶������
vector<wstring> StringOperation::Split(wstring str, char delimiter) {
	//find�֐���p���ď������s��
	int first = 0;		//�������Ă��镶����̐擪
	int last = str.find_first_of(delimiter);	//��؂蕶�����o���ʒu

	vector<wstring> result;		//��������
	
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
	return result;		//�擾�������ʂ�Ԃ�
}