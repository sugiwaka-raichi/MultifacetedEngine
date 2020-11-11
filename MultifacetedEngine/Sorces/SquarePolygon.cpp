#include "SquarePolygon.h"

//�R���X�g���N�^
SquarePolygon::SquarePolygon() {
	//����������
	CreatePolygon(0, 0, 0, 0);
	return;
}

//�����t���R���X�g���N�^
SquarePolygon::SquarePolygon(float get_x, float get_y, float get_w, float get_h) {
	//����������
	CreatePolygon(get_x, get_y, get_w, get_h);
}

//�f�X�g���N�^
SquarePolygon::~SquarePolygon() {

}

void SquarePolygon::CreatePolygon(float get_x, float get_y, float get_w, float get_h) {
	CreatePolygon(get_x, get_y, get_w, get_h, 0);
}

void SquarePolygon::CreatePolygon(float get_x, float get_y, float get_w, float get_h, float get_angle) {
	x = get_x;		//x���W
	y = get_y;		//y���W
	w = get_w;		//x�T�C�Y
	h = get_h;		//y�T�C�Y
	angle = get_angle;		//��]�p

	//���_���̍쐬
	CreateVertex();
	//�c��̒��_�����̐ݒ�
	for (int i = 0; i < 4; i++) {
		vertex[i].z = 0;
		vertex[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//�f�t�H���g��
		vertex[i].rhw = 1.0f;
	}
}

void SquarePolygon::CreateVertex() {
	//----------------------------
	//���_���̍쐬
	//----------------------------
	vertex[0].x = x - w / 2;
	vertex[0].y = y - h / 2;
	vertex[0].u = u * up;
	vertex[0].v = v * vp;

	vertex[1].x = x + w / 2;
	vertex[1].y = y - h / 2;
	vertex[1].u = u * (up + 1);
	vertex[1].v = v * vp;

	vertex[2].x = x - w / 2;
	vertex[2].y = y + h / 2;
	vertex[2].u = u * up;
	vertex[2].v = v * (vp + 1);

	vertex[3].x = x + w / 2;
	vertex[3].y = y + h / 2;
	vertex[3].u = u * (up + 1);
	vertex[3].v = v * (vp + 1);
	
	/*
	vertex[0].x = x;
	vertex[0].y = y;
	vertex[0].u = u * up;
	vertex[0].v = v * vp;
	vertex[1].x = x + w;
	vertex[1].y = y;
	vertex[1].u = u * (up + 1);
	vertex[1].v = v * vp;
	vertex[2].x = x;
	vertex[2].y = y + h;
	vertex[2].u = u * up;
	vertex[2].v = v * (vp + 1);
	vertex[3].x = x + w;
	vertex[3].y = y + h;
	vertex[3].u = u * (up + 1);
	vertex[3].v = v * (vp + 1);
	*/
	//----------------------------
	//��]�̌v�Z
	//----------------------------
	float work_x[4], work_y[4];		//��]�O�̍��W�l���i�[
	float center_x = x/* + w / 2*/;		//��]�̒��SX
	float center_y = y/* + h / 2*/;		//��]�̒��SY
	float Radian = angle * PI / 180;		//���W�A���ϊ�

	//�i�[����
	for (int i = 0; i < 4; i++) {
		work_x[i] = vertex[i].x;
		work_y[i] = vertex[i].y;
	}
	//��]����
	for (int i = 0; i < 4; i++) {
		vertex[i].x = ((work_x[i] - center_x)*cos(Radian)) - ((work_y[i] - center_y)*sin(Radian)) + center_x;
		vertex[i].y = ((work_y[i] - center_y)*cos(Radian)) + ((work_x[i] - center_x)*sin(Radian)) + center_y;
	}
}

void SquarePolygon::DrawPolygon() {
	CreateVertex();
	//�e�N�X�`���Ȃ��`��
	GetDevice()->SetTexture(0, nullptr);
	GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX_2D));
}

void SquarePolygon::DrawPolygon(LPDIRECT3DTEXTURE9 texture) {
	CreateVertex();
	//�e�N�X�`���t���`��
	GetDevice()->SetTexture(0, texture);
	GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX_2D));
}

//====================================================
//�Z�b�^�[
//====================================================
void SquarePolygon::SetX(float get_x) {
	x = get_x;
}

void SquarePolygon::SetY(float get_y) {
	y = get_y;
}

void SquarePolygon::SetW(float get_w) {
	w = get_w;
}

void SquarePolygon::SetH(float get_h) {
	h = get_h;
}

void SquarePolygon::SetU(float get_u) {
	u = get_u;
}

void SquarePolygon::SetV(float get_v) {
	v = get_v;
}

void SquarePolygon::SetUP(int get_up) {
	up = get_up;
}

void SquarePolygon::SetVP(int get_vp) {
	vp = get_vp;
}

void SquarePolygon::SetAngle(float get_angle) {
	angle = get_angle;
}

//================================================
//�Q�b�^�[
//================================================
float SquarePolygon::GetX() {
	return x;
}

float SquarePolygon::GetY() {
	return y;
}

float SquarePolygon::GetW() {
	return w;
}

float SquarePolygon::GetH() {
	return h;
}

float SquarePolygon::GetU() {
	return u;
}

float SquarePolygon::GetV() {
	return v;
}

int SquarePolygon::GetUP() {
	return up;
}

int SquarePolygon::GetVP() {
	return vp;
}

float SquarePolygon::GetAngle() {
	return angle;
}

