#pragma once
#define _INPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#pragma comment (lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

//******************************************************
//�萔��`
//******************************************************
const int NUM_KEY_MAX = 256;

class DirectInput {
private:

	LPDIRECTINPUT8			g_dinput;			//DirectInput8�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8	g_dikeyboard;		//�L�[�{�[�h�f�o�C�X
	LPDIRECTINPUTDEVICE8	g_dimouse;			//�}�E�X�f�o�C�X

	//�L�[�{�[�h���
	char					g_keybuffer[NUM_KEY_MAX];				//�L�[�{�[�h�o�b�t�@
	char					g_oldkeybuffer[NUM_KEY_MAX];			//�O����͂̃L�[�{�[�h�o�b�t�@
	BYTE					g_keyState[NUM_KEY_MAX];				//�L�[�{�[�h�̏����󂯎�郏�[�N
	BYTE					g_keyStateTrigger[NUM_KEY_MAX];			//�L�[�{�[�h�̏����󂯎�郏�[�N
	BYTE					g_keyStateRepeat[NUM_KEY_MAX];			//�L�[�{�[�h�̏����󂯎�郏�[�N
	BYTE					g_keyStateRelease[NUM_KEY_MAX];			//�L�[�{�[�h�̏����󂯎�郏�[�N
	int						g_keyStateRepeatCnt[NUM_KEY_MAX];		//�L�[�{�[�h�̃��s�[�g�J�E���^�[
	
	//�}�E�X���
	DIMOUSESTATE2			g_MouseState;			//�}�E�X�̏��
	DIMOUSESTATE2			g_MouseStateTrigger;	//�}�E�X�̏��
	POINT					g_MousePoint;			//�}�E�X���W
	int						g_width;				//�}�E�X��X���W�ő�
	int						g_height;				//�}�E�X��Y���W�ő�
	
	HWND					g_hwnd;					//�n���h��

	//�L�[�{�[�h����
	HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
	void UninitKeyboard(void);
	HRESULT UpdateKeyboard(void);

	//�}�E�X����
	HRESULT InitMouse(HINSTANCE hInst, HWND hWnd);
	void UninitMouse(void);
	HRESULT UpdateMouse(void);

public:
	//�R���X�g���N�^ :������
	DirectInput() :g_dinput(nullptr), g_dikeyboard(nullptr), g_dimouse(nullptr) {
	}
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
	long GetMouseX(void);
	long GetMouseY(void);
};