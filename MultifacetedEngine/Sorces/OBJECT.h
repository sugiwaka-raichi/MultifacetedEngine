#pragma once
//=================================
//�I�u�W�F�N�g�̊��N���X
//=================================
class OBJECT {
private:
protected:
	int layer;		//���C���[�@�\��������Ɏg��
	bool show_flag;	//�\���t���O
public:
	virtual void Init() = 0;		//�������������z�֐�
	virtual void Update() = 0;		//�X�V�������z�֐�
	virtual void Draw() = 0;		//�`�揃�����z�֐�
	virtual void UnInit() = 0;		//����������z�֐�
};

