#include "input.h"

//================================================================
//入力処理の初期化
//================================================================
HRESULT DirectInput::InitInput(HINSTANCE hInst, HWND hWnd) {
	HRESULT hr;

	if (!g_dinput) {
		//DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_dinput, NULL);
	}

	//キーボード初期化
	InitKeyboard(hInst, hWnd);

	//マウスの初期化
	InitMouse(hInst, hWnd);

	g_hwnd = hWnd;

	return hr;
}

//===================================================================
//入力処理の終了処理
//===================================================================
void  DirectInput::UninitInput(void) {
	//キーボードの終了処理
	UninitKeyboard();

	//マウスの終了処理
	UninitMouse();

	if (g_dinput) {
		g_dinput->Release();
		g_dinput = NULL;
	}
}

//====================================================================
//入力処理の更新処理
//====================================================================
void DirectInput::UpdateInput(void) {
	//キーボード更新
	UpdateKeyboard();

	//マウス更新
	UpdateMouse();
}

//=====================================================================
//キーボードの初期化
//=====================================================================
HRESULT DirectInput::InitKeyboard(HINSTANCE hInst, HWND hWnd) {
	HRESULT hr;

	//デバイスオブジェクトを作成
	hr = g_dinput->CreateDevice(GUID_SysKeyboard, &g_dikeyboard, NULL);
	if (FAILED(hr) || g_dikeyboard == NULL) {
		MessageBox(hWnd, "キーボードが見つかりませんでした。", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//データフォーマット設定
	hr = g_dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//協調モードを設定(フォアグラウンド＆非排他モード)
	hr = g_dikeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr)) {
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//キーボードへのアクセス権を獲得(入力制御開始)
	g_dikeyboard->Acquire();

	return S_OK;
}

//============================================================================
//キーボードの終了処理
//============================================================================
void DirectInput::UninitKeyboard(void) {
	if (g_dikeyboard) {
		g_dikeyboard->Unacquire();		//アクセス権解除
		g_dikeyboard->Release();		//解放
		g_dikeyboard = NULL;
	}
}

//============================================================================
//キーボードの更新処理
//============================================================================
HRESULT DirectInput::UpdateKeyboard(void) {
	HRESULT hr;
	BYTE keyStateOld[256];

	//前回のデータを保存
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	//デバイスからデータを取得
	hr = g_dikeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if (SUCCEEDED(hr)) {
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++) {
			//トリガーとリリースを取得
			g_keyStateTrigger[cnt] = (g_keyState[cnt] ^ keyStateOld[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (g_keyState[cnt] ^ keyStateOld[cnt]) & keyStateOld[cnt];

			//キーが押されているならリピートの判定処理
			if (g_keyState[cnt]) {
				if (g_keyStateRepeatCnt[cnt] < 20) {
					g_keyStateRepeatCnt[cnt]++;
					//初回入力、もしくはボタンを押してから20フレーム経過
					//した場合、リピート用配列のデータを有効にする
					//上記条件以外はリピート用配列のデータを無効にする
					if (g_keyStateRepeatCnt[cnt] == 1 || g_keyStateRepeatCnt[cnt] >= 20) {
						g_keyStateRepeat[cnt] = 0x80;
					}
					else {
						g_keyStateRepeat[cnt] = 0;
					}
				}
			}
			else {
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else {
		//キーボードへのアクセス権を取得
		hr = g_dikeyboard->Acquire();
	}

	return hr;
}

//============================================================================
//キーボードのプレス状態を取得
//============================================================================
bool DirectInput::GetKeyboardPress(int key) {
	return ((g_keyState[key] & 0x80) != 0);
}

//============================================================================
//キーボードのトリガー状態を取得
//============================================================================
bool DirectInput::GetKeyboardTrigger(int key) {
	return ((g_keyStateTrigger[key] & 0x80) != 0);
}

//============================================================================
//キーボードのリピート状態を取得
//============================================================================
bool DirectInput::GetKeyboardRepeat(int key) {
	return ((g_keyStateRepeat[key] & 0x80) != 0);
}

//============================================================================
//キーボードのリリース状態を取得
//============================================================================
bool DirectInput::GetKeyboardRelease(int key) {
	return ((g_keyStateRelease[key] & 0x80) != 0);
}

//============================================================================
//マウスの初期化
//============================================================================
HRESULT DirectInput::InitMouse(HINSTANCE hInst, HWND hWnd) {
	HRESULT hr;

	//デバイスオブジェクトを作成
	hr = g_dinput->CreateDevice(GUID_SysMouse, &g_dimouse, NULL);
	if (FAILED(hr) || g_dimouse == NULL) {
		MessageBox(hWnd, "マウスが見つかりませんでした。", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//データフォーマット設定
	hr = g_dimouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) {
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_dimouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr)) {
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//デバイスの設定
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = g_dimouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	//マウスへのアクセス権を獲得(入力制御開始)
	g_dimouse->Acquire();

	return S_OK;
}

//================================================================
//マウスの終了処理
//================================================================
void DirectInput::UninitMouse(void) {
	if (g_dimouse) {
		g_dimouse->Unacquire();
		g_dimouse->Release();
		g_dimouse = NULL;
	}
}

//================================================================
//マウスの更新処理
//================================================================
HRESULT DirectInput::UpdateMouse(void) {
	HRESULT hr;
	DIMOUSESTATE2 mouseStateOld;

	//前回のデータを保存
	mouseStateOld = g_MouseState;
	ScreenToClient(g_hwnd, &g_MousePoint);

	//デバイスからデータを取得
	hr = g_dimouse->GetDeviceState(sizeof(g_MouseState), &g_MouseState);
	if (SUCCEEDED(hr)) {
		//トリガーの取得
		for (int cnt = 0; cnt < 8; cnt++) {
			g_MouseStateTrigger.rgbButtons[cnt] = ((mouseStateOld.rgbButtons[cnt] ^ g_MouseState.rgbButtons[cnt]) & g_MouseState.rgbButtons[cnt]);
		}
	}
	else {
		g_dimouse->Acquire();
	}
	return S_OK;
}

//================================================================
//マウスデータ取得（左プレス）
//================================================================
bool DirectInput::GetMouseLeftPress(void) {
	return ((g_MouseState.rgbButtons[0] & 0x80) != 0);
}

//================================================================
//マウスデータ取得（左トリガー）
//================================================================
bool DirectInput::GetMouseLeftTrigger(void) {
	return ((g_MouseStateTrigger.rgbButtons[0] & 0x80 != 0));
}

//================================================================
//マウスデータ取得（右プレス）
//================================================================
bool DirectInput::GetMouseRightPress(void) {
	return ((g_MouseState.rgbButtons[1] & 0x80) != 0);
}

//================================================================
//マウスデータ取得（右トリガー）
//================================================================
bool DirectInput::GetMouseRightTrigger(void) {
	return ((g_MouseStateTrigger.rgbButtons[1] & 0x80) != 0);
}

//================================================================
//マウスデータ取得（中央プレス）
//================================================================
bool DirectInput::GetMouseCenterPress(void) {
	return ((g_MouseState.rgbButtons[2] & 0x80) != 0);
}

//================================================================
//マウスデータ取得（中央トリガー）
//================================================================
bool DirectInput::GetMouseCenterTrigger(void) {
	return ((g_MouseStateTrigger.rgbButtons[2] & 0x80) != 0);
}

//================================================================
//マウスデータ取得（Ｘ軸移動）
//================================================================
long DirectInput::GetMouseAxisX(void) {
	return g_MouseState.lX;
}

//================================================================
//マウスデータ取得（Ｙ軸移動）
//================================================================
long DirectInput::GetMouseAxisY(void) {
	return g_MouseState.lY;
}

//================================================================
//マウスデータ取得（Ｚ軸移動）
//================================================================
long DirectInput::GetMouseAxisZ(void) {
	return g_MouseState.lZ;
}

//================================================================
//マウス座標取得（Ｘ）
//================================================================
long DirectInput::GetMouseX(void) {
	return g_MousePoint.x;
}

//================================================================
//マウス座標取得（Ｙ）
//================================================================
long DirectInput::GetMouseY(void) {
	return g_MousePoint.y;
}