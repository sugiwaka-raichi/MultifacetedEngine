#include "SquarePolygon.h"

//コンストラクタ
SquarePolygon::SquarePolygon() {
	tex = nullptr;		//テクスチャポインタにnull
	//初期化処理
	CreatePolygon(0, 0, 0, 0);
	u = v = 1.0f;
	return;
}

//引数付きコンストラクタ
SquarePolygon::SquarePolygon(float get_x, float get_y, float get_w, float get_h) {
	tex = nullptr;		//テクスチャポインタにnull
	//初期化処理
	CreatePolygon(get_x, get_y, get_w, get_h);
	u = v = 1.0f;			//初期値テクスチャ全表示
}

//デストラクタ
SquarePolygon::~SquarePolygon() {

}

//ポリゴンの作成
void SquarePolygon::CreatePolygon(float get_x, float get_y, float get_w, float get_h) {
	CreatePolygon(get_x, get_y, get_w, get_h, 0);
}

//ポリゴンの作成（角度付き）
void SquarePolygon::CreatePolygon(float get_x, float get_y, float get_w, float get_h, float get_angle) {
	x = get_x;		//x座標
	y = get_y;		//y座標
	w = get_w;		//xサイズ
	h = get_h;		//yサイズ
	angle = get_angle;		//回転角

	//頂点情報の作成
	CreateVertex();
	//残りの頂点事項の設定
	for (int i = 0; i < 4; i++) {
		vertex[i].z = 0;
		vertex[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//デフォルト白
		vertex[i].rhw = 1.0f;
	}
}

//頂点情報作成
void SquarePolygon::CreateVertex() {
	//----------------------------
	//頂点情報の作成
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
	//回転の計算
	//----------------------------
	float work_x[4], work_y[4];		//回転前の座標値を格納
	float center_x = x/* + w / 2*/;		//回転の中心X
	float center_y = y/* + h / 2*/;		//回転の中心Y
	float Radian = angle * PI / 180;		//ラジアン変換

	//格納処理
	for (int i = 0; i < 4; i++) {
		work_x[i] = vertex[i].x;
		work_y[i] = vertex[i].y;
	}
	//回転処理
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
	x = _x / _t;		//移動間隔を計算
	y = _y / _t;

}

//描画(テクスチャなし)
void SquarePolygon::DrawPolygon() {
	CreateVertex();
	if (tex == nullptr) {
		//テクスチャ無描画
		GetDevice()->SetTexture(0, nullptr);
	}
	else {
		//テクスチャ有描画
		GetDevice()->SetTexture(0, *tex);
	}
	GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX_2D));
}

//描画(指定テクスチャポインタ)
void SquarePolygon::DrawPolygon(LPDIRECT3DTEXTURE9* texture) {
	CreateVertex();
	//テクスチャ付き描画
	GetDevice()->SetTexture(0, *texture);
	GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX_2D));
}

//描画(指定テクスチャ)
void SquarePolygon::DrawPolygon(LPDIRECT3DTEXTURE9 texture) {
	CreateVertex();
	//テクスチャ付き描画
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
//セッター
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
	tex = p_tex;		//テクスチャポインタ保持
}

void SquarePolygon::SetColor(D3DCOLOR _color) {
	for (int i = 0; i < 4; i++) {
		vertex[i].col = _color;
	}
}

void SquarePolygon::SetColor(COLOR _col, int _value) {
	_value & 0xFF;		//1バイトに収める
	D3DCOLOR col = GetColor();
	switch (_col) 	{
	case COLOR::ALPHA:
		_value = _value << 24;
		col = col & 0x00FFFFFF;		//alphaのビットをリセット
		col = col | _value;			//新alphaのビットをセット
		break;
	case COLOR::RED:
		_value = _value << 16;
		col = col & 0xFF00FFFF;		//ビットをリセット
		col = col | _value;			//セット
		break;
	case COLOR::GREEN:
		_value = _value << 8;
		col = col & 0xFFFF00FF;		//ビットをリセット
		col = col | _value;			//セット
		break;
	case COLOR::BLUE:
		col = col & 0xFFFFFF00;		//ビットをリセット
		col = col | _value;			//セット
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
//ゲッター
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
	return vertex[0].col;			//ポリゴンの色(頂点色は同一のものしか現状扱えないのでこれで問題なし)
}

int SquarePolygon::GetColor(COLOR _col)
{
	//==================================================
	//D3DCOLOR は ARGBの構成で各値1バイトで表現される
	//ビット演算で必要な情報だけ抽出するのが以下
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
