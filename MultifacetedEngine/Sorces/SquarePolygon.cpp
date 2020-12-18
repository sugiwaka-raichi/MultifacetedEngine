#include "SquarePolygon.h"

//�R���X�g���N�^
SquarePolygon::SquarePolygon() {
	tex = nullptr;		//�e�N�X�`���|�C���^��null
	//����������
	CreatePolygon(0, 0, 0, 0);
	u = v = 1.0f;
	return;
}

//�����t���R���X�g���N�^
SquarePolygon::SquarePolygon(float get_x, float get_y, float get_w, float get_h) {
	tex = nullptr;		//�e�N�X�`���|�C���^��null
	//����������
	CreatePolygon(get_x, get_y, get_w, get_h);
	u = v = 1.0f;			//�����l�e�N�X�`���S�\��
}

//�f�X�g���N�^
SquarePolygon::~SquarePolygon() {

}

//�|���S���̍쐬
void SquarePolygon::CreatePolygon(float get_x, float get_y, float get_w, float get_h) {
	CreatePolygon(get_x, get_y, get_w, get_h, 0);
}

//�|���S���̍쐬�i�p�x�t���j
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

//���_���쐬
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
	vertex[1].u = u * (up + 1.0f);
	vertex[1].v = v * vp;

	vertex[2].x = x - w / 2;
	vertex[2].y = y + h / 2;
	vertex[2].u = u * up;
	vertex[2].v = v * (vp + 1.0f);

	vertex[3].x = x + w / 2;
	vertex[3].y = y + h / 2;
	vertex[3].u = u * (up + 1.0f);
	vertex[3].v = v * (vp + 1.0f);
	
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

void SquarePolygon::DirectionMove(float _amount) {
	DirectionMove(_amount, 0);
}

void SquarePolygon::DirectionMove(float _amount, float _angle) {
	float rad;
	rad = (angle + _angle) * PI / 180.0f;
	x += cos(rad) * _amount;
	y += sin(rad) * _amount;
}

void SquarePolygon::Move(float _x, float _y, int _t) {
	x = _x / _t;		//�ړ��Ԋu���v�Z
	y = _y / _t;

}

//�`��(�e�N�X�`���Ȃ�)
void SquarePolygon::DrawPolygon() {
	CreateVertex();
	if (tex == nullptr) {
		//�e�N�X�`�����`��
		GetDevice()->SetTexture(0, nullptr);
	}
	else {
		//�e�N�X�`���L�`��
		GetDevice()->SetTexture(0, *tex);
	}
	GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX_2D));
}

//�`��(�w��e�N�X�`���|�C���^)
void SquarePolygon::DrawPolygon(LPDIRECT3DTEXTURE9* texture) {
	CreateVertex();
	//�e�N�X�`���t���`��
	GetDevice()->SetTexture(0, *texture);
	GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX_2D));
}

//�`��(�w��e�N�X�`��)
void SquarePolygon::DrawPolygon(LPDIRECT3DTEXTURE9 texture) {
	CreateVertex();
	//�e�N�X�`���t���`��
	GetDevice()->SetTexture(0, texture);
	GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX_2D));
}

void SquarePolygon::Init()
{
}

void SquarePolygon::Draw() {
	DrawPolygon();
}

void SquarePolygon::Update() {
	currentTime++;
}

void SquarePolygon::UnInit()
{
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

void SquarePolygon::SetLayer(int _layer) {
	layer = _layer;
}

void SquarePolygon::SetTag(string _tag) {
	tag = _tag;
}

void SquarePolygon::SetTexture(LPDIRECT3DTEXTURE9* p_tex) {
	tex = p_tex;		//�e�N�X�`���|�C���^�ێ�
}

void SquarePolygon::SetColor(D3DCOLOR _color) {
	for (int i = 0; i < 4; i++) {
		vertex[i].col = _color;
	}
}

void SquarePolygon::SetColor(COLOR _col, int _value) {
	_value & 0xFF;		//1�o�C�g�Ɏ��߂�
	D3DCOLOR col = GetColor();
	switch (_col) 	{
	case COLOR::ALPHA:
		_value = _value << 24;
		col = col & 0x00FFFFFF;		//alpha�̃r�b�g�����Z�b�g
		col = col | _value;			//�Valpha�̃r�b�g���Z�b�g
		break;
	case COLOR::RED:
		_value = _value << 16;
		col = col & 0xFF00FFFF;		//�r�b�g�����Z�b�g
		col = col | _value;			//�Z�b�g
		break;
	case COLOR::GREEN:
		_value = _value << 8;
		col = col & 0xFFFF00FF;		//�r�b�g�����Z�b�g
		col = col | _value;			//�Z�b�g
		break;
	case COLOR::BLUE:
		col = col & 0xFFFFFF00;		//�r�b�g�����Z�b�g
		col = col | _value;			//�Z�b�g
		break;
	}
	SetColor(col);
}

void SquarePolygon::SetPos(float _x, float _y) {
	x = _x;
	y = _y;
}

void SquarePolygon::SetSize(float _w, float _h) {
	w = _w;
	h = _h;
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

RECT SquarePolygon::GetRect()
{
	RECT rect;
	rect.left = x - w / 2;
	rect.top = y - h / 2;
	rect.right = x + w / 2;
	rect.bottom = y + h / 2;
	return rect;
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

int SquarePolygon::GetLayer() {
	return layer;
}

string SquarePolygon::GetTag() {
	return tag;
}

D3DCOLOR SquarePolygon::GetColor()
{
	return vertex[0].col;			//�|���S���̐F(���_�F�͓���̂��̂������󈵂��Ȃ��̂ł���Ŗ��Ȃ�)
}

int SquarePolygon::GetColor(COLOR _col)
{
	//==================================================
	//D3DCOLOR �� ARGB�̍\���Ŋe�l1�o�C�g�ŕ\�������
	//�r�b�g���Z�ŕK�v�ȏ�񂾂����o����̂��ȉ�
	//==================================================
	D3DCOLOR col = GetColor();
	int temp;
	switch (_col)
	{
	case COLOR::ALPHA:
		temp = col >> 24;
		return temp;
	case COLOR::RED:
		temp = col & 0x00FF0000;
		temp = temp >> 16;
		return temp;
	case COLOR::GREEN:
		temp = col & 0x0000FF00;
		temp = temp >> 8;
		return temp;
	case COLOR::BLUE:
		temp = col & 0x000000FF;
		return temp;
	}
}
