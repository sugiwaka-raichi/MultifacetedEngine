//=============================================================================
//
// ���͏��� [input.h]
//
//=============================================================================
//#ifndef _INPUT_H_
//#define _INPUT_H_
#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

<<<<<<< HEAD
enum MOUSE_KEYTYPE {
	LEFT = 0,
	RIGHT,
	CENTER,
	FORWARD,
};
=======
typedef enum {
	LEFT,
	RIGHT,
	CENTER,
	BACK,
	FORWARD
}MOUSE_KEYTYPE;
>>>>>>> dev_/Update

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

bool GetKeyboardPress(int key);
bool GetKeyboardTrigger(int key);
bool GetKeyboardRepeat(int key);
bool GetKeyboardRelease(int key);

bool GetMouseLeftPress(void);
bool GetMouseLeftTrigger(void);
bool GetMouseRightPress(void);
bool GetMouseRightTrigger(void);
bool GetMouseCenterPress(void);
bool GetMouseCenterTrigger(void);
long GetMouseAxisX(void);
long GetMouseAxisY(void);
long GetMouseAxisZ(void);
long GetMouseScreenX(void);
long GetMouseScreenY(void);
long GetMouseX(void);
long GetMouseY(void);
DIMOUSESTATE2 GetMousePress(void);		//MOUSE�N���b�N��(������Ă��邩)
BYTE GetMousePress(int);				//�擾�f�[�^����
DIMOUSESTATE2 GetMouseTrigger(void);	//MOUSE�N���b�N��(�����ꂽ���j
BYTE GetMouseTrigger(int);				//�擾�f�[�^����

//#endif