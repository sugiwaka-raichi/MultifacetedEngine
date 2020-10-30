#include "SquarePolygon.h"

//コンストラクタ
SquarePolygon::SquarePolygon() {
	//初期化処理
	CreatePolygon(0, 0, 0, 0);
	return;
}

//引数付きコンストラクタ
SquarePolygon::SquarePolygon(float get_x, float get_y, float get_w, float get_h) {
	//初期化処理
	CreatePolygon(get_x, get_y, get_w, get_h);
}

//デストラクタ
SquarePolygon::~SquarePolygon() {

}

void SquarePolygon::CreatePolygon(float get_x, float get_y, float get_w, float get_h) {
	CreatePolygon(get_x, get_y, get_w, get_h, 0);
}

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

void SquarePolygon::DrawPolygon() {
	CreateVertex();
	//テクスチャなし描画
	GetDevice()->SetTexture(0, nullptr);
	GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX_2D));
}

void SquarePolygon::DrawPolygon(LPDIRECT3DTEXTURE9 texture) {
	CreateVertex();
	//テクスチャ付き描画
	GetDevice()->SetTexture(0, texture);
	GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX_2D));
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

int SquarePolygon::GetUP() {
	return up;
}

int SquarePolygon::GetVP() {
	return vp;
}

float SquarePolygon::GetAngle() {
	return angle;
}

