#pragma once
//***********************************************
//インクルード
//***********************************************
#include "Win.h"
#include "OBJECT.h"

//***********************************************
//プロトタイプ宣言
//***********************************************
bool GameInit(HINSTANCE hinst, HWND hWnd, int width, int height, bool fullscreen);
void GameMain();
void GameExit();
void GameInput();
void GameUpdate();
void GameRender();

LPDIRECT3DDEVICE9 GetDevice();		//デバイスゲッター

float ToRad(float deg);			//ラジアン→デグリー
float ToDeg(float rad);			//デグリー→ラジアン