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
//���C�u�����̃����N
//*********************************************************
#if 1
#pragma comment (lib, "d3d9.lib")		//�`��ɕK�v
#pragma comment (lib, "d3dx9.lib")		//[d3d9.lib]�̊g�����C�u����
#pragma comment (lib, "dxguid.lib")		//DirectX�R���|�[�l���g�d�l�ɕK�v
#pragma comment (lib, "winmm.lib")		//���ݎ����擾�ȂǂɕK�v
#pragma comment(lib,"dinput8.lib")		//���͂ɕK�v?
#endif

//************************************************************
//�}�N����`
//************************************************************
#define SCREEN_WIDTH  (800)		//�E�B���h�E�̕�
#define SCREEN_HEIGHT (600)		//�E�B���h�E�̍���

#define CLASS_NAME	TEXT("Class Name")
#define WINDOW_NAME	TEXT("Title")
//#ifdef UNICODE
////���j�R�[�h�p
//#define string wstring	//���j�R�[�h�pstring
//#define LPCSTR LPWSTR	//���j�R�[�h�pLPCSTR
//#else
////�}���`�o�C�g�p
//#define string string		//�}���`�o�C�gstring
//#define LPCSTR LPCSTR		//�}���`�o�C�gLPSTR
//#endif

#define FULLSCREEN 	false				//�t���X�N���[���׸�
#define WINDOWSTYLE WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX	//�E�B���h�E�T�C�Y�̕ύX���֎~�����X�^�C��
//#define WINDOWSTYLE WS_OVERLAPPEDWINDOW									//�E�B���h�E�T�C�Y�̕ύX���\�ȃX�^�C��

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//FPS�ݒ�
#define FPS (60)

//�~�������ȗ�
#define PI D3DX_PI

//************************************************************
//�\���̒�`
//************************************************************
//2D�p���_���
typedef struct {
	float x, y, z;		//���_���W
	float rhw;			//rhw
	D3DCOLOR col;		//�J���[
	float u, v;			//�e�N�X�`�����W
}VERTEX_2D;

//************************************************************
//�v���g�^�C�v�錾
//************************************************************
void ErrorMessageBox(LPCWSTR, LPCWSTR, UINT);
HWND GetHandle();

//************************************************************
//�O���[�o���ϐ�
//************************************************************
//FPS�Ǘ��p
extern int CountFPS;
extern float dispFPS;
