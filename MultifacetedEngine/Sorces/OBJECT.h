#pragma once
//=================================
//�I�u�W�F�N�g�̊��N���X
//=================================
class OBJECT {
private:
protected:
	int layer = 0;			//���C���[�@�\��������Ɏg��
	bool dispFlag = true;	//�\���t���O
public:
	virtual void Init() = 0;		//�������������z�֐�
	virtual void Update() = 0;		//�X�V�������z�֐�
	virtual void Draw() = 0;		//�`�揃�����z�֐�
	virtual void UnInit() = 0;		//����������z�֐�
	
	void SetLayernum(int _layer);
	void SetDispFlag(bool _flag);
	int GetLayernum();				//���C���[�ԍ��擾
	bool GetDispFlag();				//�\���t���O�擾

};

