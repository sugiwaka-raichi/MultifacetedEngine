#pragma once
#include "game.h"

class SquarePolygon
{
private:
protected:
	//***********************************************
	//構造体定義
	//***********************************************
	typedef struct {
		float x, y, z;
		float rhw;
		D3DCOLOR col;
		float u, v;
	}VERTEX_2D;

	VERTEX_2D vertex[4];		//頂点情報
	float x, y;		//座標
	float w, h;		//大きさ
	float u, v;		//テクスチャ表示範囲
	int up, vp;		//テクスチャ表示位置
	float angle;	//回転角
	int layer;		//レイヤー　表示順決定に使う
	bool show_flag;	//表示フラグ
	LPDIRECT3DTEXTURE9*	tex;		//テクスチャポインタ
	char* tag;						//外部参照用オブジェクト名

	void CreateVertex();		//頂点情報の作成
	//void Rotation();		//回転
public:
	SquarePolygon();		//コンストラクタ
	SquarePolygon(float get_x, float get_y, float get_w, float get_h);		//初期設定付きポリゴンの作成
	~SquarePolygon();		//デストラクタ
	void CreatePolygon(float get_x,float get_y,float get_w,float get_h);	//ポリゴンの作成
	void CreatePolygon(float get_x,float get_y,float get_w,float get_h, float get_angle);	//ポリゴンの作成回転付き
	void RelativeX(float _x);			//相対移動X
	void RelativeY(float _y);			//相対移動Y
	void DirectionMove(float _amount);	//方向移動
	void DirectionMove(float _amount, float _angle);	//方向移動
	void DrawPolygon();		//描画
	void DrawPolygon(LPDIRECT3DTEXTURE9* texture);		//描画
	void DrawPolygon(LPDIRECT3DTEXTURE9 texture);		//テクスチャ指定描画

	//-----------------------------------
	//セッター
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
	//ゲッター
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

