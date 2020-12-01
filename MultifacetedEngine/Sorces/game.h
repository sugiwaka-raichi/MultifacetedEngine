#pragma once
//***********************************************
//�C���N���[�h
//***********************************************
#include "Win.h"
#include "OBJECT.h"

//***********************************************
//�v���g�^�C�v�錾
//***********************************************
bool GameInit(HINSTANCE hinst, HWND hWnd, int width, int height, bool fullscreen);
void GameMain();
void GameExit();
void GameInput();
void GameUpdate();
void GameRender();

LPDIRECT3DDEVICE9 GetDevice();		//�f�o�C�X�Q�b�^�[

float ToRad(float deg);			//���W�A�����f�O���[
float ToDeg(float rad);			//�f�O���[�����W�A��