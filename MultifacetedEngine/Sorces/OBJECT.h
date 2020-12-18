#pragma once
#include "game.h"
//=================================
//�I�u�W�F�N�g�̊��N���X
//=================================
class OBJECT {
private:
protected:
	int layer = 0;			//���C���[�@�\��������Ɏg��
	bool dispFlag = true;	//�\���t���O
	string name = L"temp";	//�ꎞ�I�u�W�F�N�g����
	string tag = L"none";	//�^�O����
public:
	virtual void Init() = 0;		//�������������z�֐�
	virtual void Update() = 0;		//�X�V�������z�֐�
	virtual void Draw() = 0;		//�`�揃�����z�֐�
	virtual void UnInit() = 0;		//����������z�֐�
	
	void SetLayernum(int _layer);	//�\���D��x
	void SetDispFlag(bool _flag);	//�\���t���O
	void SetName(string _name);		//�I�u�W�F�N�g��
	
	int GetLayernum();				//���C���[�ԍ��擾
	bool GetDispFlag();				//�\���t���O�擾
	string GetObjName();			//�I�u�W�F�N�g�����擾

};

