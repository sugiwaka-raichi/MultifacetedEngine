#pragma once
#define _MAIN_H

//*********************************************************
//include file
//*********************************************************
#include <Windows.h>
#include <string>
#include <list>
#include <time.h>
#include "d3dx9.h"
#include "dinput.h"
#include "input.h"
#include "XAudio2.h"

//******************************************
//using
//******************************************
using namespace std;

//*********************************************************
//ライブラリのリンク
//*********************************************************
#if 1
#pragma comment (lib, "d3d9.lib")		//描画に必要
#pragma comment (lib, "d3dx9.lib")		//[d3d9.lib]の拡張ライブラリ
#pragma comment (lib, "dxguid.lib")		//DirectXコンポーネント仕様に必要
#pragma comment (lib, "winmm.lib")		//現在時刻取得などに必要
#pragma comment(lib,"dinput8.lib")		//入力に必要?
#endif

//************************************************************
//マクロ定義
//************************************************************
#define SCREEN_WIDTH  (800)		//ウィンドウの幅
#define SCREEN_HEIGHT (600)		//ウィンドウの高さ

#define CLASS_NAME	TEXT("Class Name")
#define WINDOW_NAME	TEXT("Title")
//#ifdef UNICODE
////ユニコード用
//#define string wstring	//ユニコード用string
//#define LPCSTR LPWSTR	//ユニコード用LPCSTR
//#else
////マルチバイト用
//#define string string		//マルチバイトstring
//#define LPCSTR LPCSTR		//マルチバイトLPSTR
//#endif

#define FULLSCREEN 	false				//フルスクリーンﾌﾗｸﾞ
#define WINDOWSTYLE WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX	//ウィンドウサイズの変更を禁止したスタイル
//#define WINDOWSTYLE WS_OVERLAPPEDWINDOW									//ウィンドウサイズの変更が可能なスタイル

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//FPS設定
#define FPS (60)

//円周率を省略
#define PI D3DX_PI

//************************************************************
//構造体定義
//************************************************************
//2D用頂点情報
typedef struct {
	float x, y, z;		//頂点座標
	float rhw;			//rhw
	D3DCOLOR col;		//カラー
	float u, v;			//テクスチャ座標
}VERTEX_2D;

//************************************************************
//プロトタイプ宣言
//************************************************************
void ErrorMessageBox(LPCWSTR, LPCWSTR, UINT);
HWND GetHandle();

//************************************************************
//グローバル変数
//************************************************************
//FPS管理用
extern int CountFPS;
extern float dispFPS;
