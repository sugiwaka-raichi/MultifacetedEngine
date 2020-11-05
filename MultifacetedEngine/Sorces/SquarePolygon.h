#pragma once
#include "game.h"

class SquarePolygon
{
private:
protected:
	//***********************************************
	//�\���̒�`
	//***********************************************
	typedef struct {
		float x, y, z;
		float rhw;
		D3DCOLOR col;
		float u, v;
	}VERTEX_2D;

	VERTEX_2D vertex[4];		//���_���
	float x, y;		//���W
	float w, h;		//�傫��
	float u, v;		//�e�N�X�`���\���͈�
	int up, vp;		//�e�N�X�`���\���ʒu
	float angle;	//��]�p
	int layer;		//���C���[�@�\��������Ɏg��
	bool show_flag;	//�\���t���O
	LPDIRECT3DTEXTURE9*	tex;		//�e�N�X�`���|�C���^
	char* tag;						//�O���Q�Ɨp�I�u�W�F�N�g��

	void CreateVertex();		//���_���̍쐬
	//void Rotation();		//��]
public:
	SquarePolygon();		//�R���X�g���N�^
	SquarePolygon(float get_x, float get_y, float get_w, float get_h);		//�����ݒ�t���|���S���̍쐬
	~SquarePolygon();		//�f�X�g���N�^
	void CreatePolygon(float get_x,float get_y,float get_w,float get_h);	//�|���S���̍쐬
	void CreatePolygon(float get_x,float get_y,float get_w,float get_h, float get_angle);	//�|���S���̍쐬��]�t��
	void RelativeX(float _x);			//���Έړ�X
	void RelativeY(float _y);			//���Έړ�Y
	void DirectionMove(float _amount);	//�����ړ�
	void DirectionMove(float _amount, float _angle);	//�����ړ�
	void DrawPolygon();		//�`��
	void DrawPolygon(LPDIRECT3DTEXTURE9* texture);		//�`��
	void DrawPolygon(LPDIRECT3DTEXTURE9 texture);		//�e�N�X�`���w��`��

	//-----------------------------------
	//�Z�b�^�[
	//-----------------------------------
	void SetX(float _x);
	void SetY(float _y);
	void SetW(float _w);
	void SetH(float _h);
	void SetU(float _u);
	void SetV(float _v);
	void SetUP(int _up);
	void SetVP(int _vp);
	void SetAngle(float _angle);
	void SetLayer(int _layer);
	void SetTag(char* tag);
	void SetTexture(LPDIRECT3DTEXTURE9* p_tex);

	//-----------------------------------
	//�Q�b�^�[
	//-----------------------------------
	float GetX();
	float GetY();
	float GetW();
	float GetH();
	float GetU();
	float GetV();
	int GetUP();
	int GetVP();
	float GetAngle();
	int GetLayer();
	char* GetTag();
};

