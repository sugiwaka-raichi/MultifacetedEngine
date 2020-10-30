#pragma once
#define _INPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#pragma comment (lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

//******************************************************
//定数定義
//******************************************************
const int NUM_KEY_MAX = 256;

class DirectInput {
private:

	LPDIRECTINPUT8			g_dinput;			//DirectInput8オブジェクト
	LPDIRECTINPUTDEVICE8	g_dikeyboard;		//キーボードデバイス
	LPDIRECTINPUTDEVICE8	g_dimouse;			//マウスデバイス

	//キーボード情報
	char					g_keybuffer[NUM_KEY_MAX];				//キーボードバッファ
	char					g_oldkeybuffer[NUM_KEY_MAX];			//前回入力のキーボードバッファ
	BYTE					g_keyState[NUM_KEY_MAX];				//キーボードの情報を受け取るワーク
	BYTE					g_keyStateTrigger[NUM_KEY_MAX];			//キーボードの情報を受け取るワーク
	BYTE					g_keyStateRepeat[NUM_KEY_MAX];			//キーボードの情報を受け取るワーク
	BYTE					g_keyStateRelease[NUM_KEY_MAX];			//キーボードの情報を受け取るワーク
	int						g_keyStateRepeatCnt[NUM_KEY_MAX];		//キーボードのリピートカウンター
	
	//マウス情報
	DIMOUSESTATE2			g_MouseState;			//マウスの状態
	DIMOUSESTATE2			g_MouseStateTrigger;	//マウスの状態
	POINT					g_MousePoint;			//マウス座標
	int						g_width;				//マウスのX座標最大
	int						g_height;				//マウスのY座標最大
	
	HWND					g_hwnd;					//ハンドル

	//キーボード処理
	HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
	void UninitKeyboard(void);
	HRESULT UpdateKeyboard(void);

	//マウス処理
	HRESULT InitMouse(HINSTANCE hInst, HWND hWnd);
	void UninitMouse(void);
	HRESULT UpdateMouse(void);

public:
	//コンストラクタ :初期化
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