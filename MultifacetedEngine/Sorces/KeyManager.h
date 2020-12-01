#pragma once
#include "game.h"
#include "input.h"
#include <map>
#include <vector>

//========================
//�L�[�̏�ԊǗ��\����
//========================
typedef struct {
	BYTE key;		//���͂��ꂽ�L�[
	bool press;		//������Ă�����
	bool trigger;	//�����ꂽ
	bool repeat;	//
	bool releases;	//�����ꂽ
	
}KEY_STATE;

static class KeyManager {
private:
	map<string, vector<KEY_STATE>> keyList;		//�������ƂɃL�[�̏�Ԃ�ێ����鎫��
public:
	void KeyManagerUpdate();				//�L�[�̓��͏�Ԃ����ׂĎ擾
	bool GetKeyState(string _process);		//���鏈���̓��͏�Ԃ��擾

};
