#pragma once
#include "game.h"

enum class COLOR{
	ALPHA,
	RED,
	GREEN,
	BLUE
};

class SquarePolygon:public OBJECT
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
	int up = 0, vp = 0;		//�e�N�X�`���\���ʒu
	float angle;	//��]�p
	int timer;		//�ݒ肳��Ă��鎞��
	int currentTime;		//���̎���

	LPDIRECT3DTEXTURE9*	tex = nullptr;		//�e�N�X�`���|�C���^

	void CreateVertex();		//���_���̍쐬
	//void Rotation();		//��]
public:
	SquarePolygon();		//�R���X�g���N�^
	SquarePolygon(float get_x, float get_y, float get_w, float get_h);		//�����ݒ�t���|���S���̍쐬
	~SquarePolygon();		//�f�X�g���N�^
	void CreatePolygon(float get_x,float get_y,float get_w,float get_h);	//�|���S���̍쐬
	void CreatePolygon(float get_x,float get_y,float get_w,float get_h, float get_angle);	//�|���S���̍쐬��]�t��
	//void RelativeX(float _x);			//��Βl�ړ�X	
	//void RelativeY(float _y);			//��Βl�ړ�Y
	void DirectionMove(float _amount);	//�����ړ�
	void DirectionMove(float _amount, float _angle);	//�����ړ�
	void Move(float _x, float _y, int _t);			//���Ԉړ�
	void DrawPolygon();		//�`��
	void DrawPolygon(LPDIRECT3DTEXTURE9* texture);		//�`��
	void DrawPolygon(LPDIRECT3DTEXTURE9 texture);		//�e�N�X�`���w��`��

	//-----------------------------------
	//�I�[�o�[���C�h
	//-----------------------------------
	void Init();
	void Draw();
	void Update();
	void UnInit();

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
	void SetTag(wstring _tag);
	void SetTexture(LPDIRECT3DTEXTURE9* p_tex);
	void SetColor(D3DCOLOR _color);
	void SetColor(COLOR _col, int _value);		//�w�肵���F��ύX����
	void SetPos(float, float);		//�ʒu�ݒ�
	void SetSize(float, float);		//�T�C�Y�ݒ�
	

	//-----------------------------------
	//�Q�b�^�[
	//-----------------------------------
	float GetX();
	float GetY();
	float GetW();
	float GetH();
	float GetU();
	float GetV();
	RECT  GetRect();
	int GetUP();
	int GetVP();
	float GetAngle();
	int GetLayer();
	wstring GetTag();
	D3DCOLOR GetColor();
	int GetColor(COLOR _col);		//�擾�������F���w��
};

