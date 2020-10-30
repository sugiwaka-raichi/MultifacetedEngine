#include "input.h"

//================================================================
//���͏����̏�����
//================================================================
HRESULT DirectInput::InitInput(HINSTANCE hInst, HWND hWnd) {
	HRESULT hr;

	if (!g_dinput) {
		//DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_dinput, NULL);
	}

	//�L�[�{�[�h������
	InitKeyboard(hInst, hWnd);

	//�}�E�X�̏�����
	InitMouse(hInst, hWnd);

	g_hwnd = hWnd;

	return hr;
}

//===================================================================
//���͏����̏I������
//===================================================================
void  DirectInput::UninitInput(void) {
	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//�}�E�X�̏I������
	UninitMouse();

	if (g_dinput) {
		g_dinput->Release();
		g_dinput = NULL;
	}
}

//====================================================================
//���͏����̍X�V����
//====================================================================
void DirectInput::UpdateInput(void) {
	//�L�[�{�[�h�X�V
	UpdateKeyboard();

	//�}�E�X�X�V
	UpdateMouse();
}

//=====================================================================
//�L�[�{�[�h�̏�����
//=====================================================================
HRESULT DirectInput::InitKeyboard(HINSTANCE hInst, HWND hWnd) {
	HRESULT hr;

	//�f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_dinput->CreateDevice(GUID_SysKeyboard, &g_dikeyboard, NULL);
	if (FAILED(hr) || g_dikeyboard == NULL) {
		MessageBox(hWnd, "�L�[�{�[�h��������܂���ł����B", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//�f�[�^�t�H�[�}�b�g�ݒ�
	hr = g_dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//�������[�h��ݒ�(�t�H�A�O���E���h����r�����[�h)
	hr = g_dikeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr)) {
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_dikeyboard->Acquire();

	return S_OK;
}

//============================================================================
//�L�[�{�[�h�̏I������
//============================================================================
void DirectInput::UninitKeyboard(void) {
	if (g_dikeyboard) {
		g_dikeyboard->Unacquire();		//�A�N�Z�X������
		g_dikeyboard->Release();		//���
		g_dikeyboard = NULL;
	}
}

//============================================================================
//�L�[�{�[�h�̍X�V����
//============================================================================
HRESULT DirectInput::UpdateKeyboard(void) {
	HRESULT hr;
	BYTE keyStateOld[256];

	//�O��̃f�[�^��ۑ�
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	//�f�o�C�X����f�[�^���擾
	hr = g_dikeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if (SUCCEEDED(hr)) {
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++) {
			//�g���K�[�ƃ����[�X���擾
			g_keyStateTrigger[cnt] = (g_keyState[cnt] ^ keyStateOld[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (g_keyState[cnt] ^ keyStateOld[cnt]) & keyStateOld[cnt];

			//�L�[��������Ă���Ȃ烊�s�[�g�̔��菈��
			if (g_keyState[cnt]) {
				if (g_keyStateRepeatCnt[cnt] < 20) {
					g_keyStateRepeatCnt[cnt]++;
					//������́A�������̓{�^���������Ă���20�t���[���o��
					//�����ꍇ�A���s�[�g�p�z��̃f�[�^��L���ɂ���
					//��L�����ȊO�̓��s�[�g�p�z��̃f�[�^�𖳌��ɂ���
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
		//�L�[�{�[�h�ւ̃A�N�Z�X�����擾
		hr = g_dikeyboard->Acquire();
	}

	return hr;
}

//============================================================================
//�L�[�{�[�h�̃v���X��Ԃ��擾
//============================================================================
bool DirectInput::GetKeyboardPress(int key) {
	return ((g_keyState[key] & 0x80) != 0);
}

//============================================================================
//�L�[�{�[�h�̃g���K�[��Ԃ��擾
//============================================================================
bool DirectInput::GetKeyboardTrigger(int key) {
	return ((g_keyStateTrigger[key] & 0x80) != 0);
}

//============================================================================
//�L�[�{�[�h�̃��s�[�g��Ԃ��擾
//============================================================================
bool DirectInput::GetKeyboardRepeat(int key) {
	return ((g_keyStateRepeat[key] & 0x80) != 0);
}

//============================================================================
//�L�[�{�[�h�̃����[�X��Ԃ��擾
//============================================================================
bool DirectInput::GetKeyboardRelease(int key) {
	return ((g_keyStateRelease[key] & 0x80) != 0);
}

//============================================================================
//�}�E�X�̏�����
//============================================================================
HRESULT DirectInput::InitMouse(HINSTANCE hInst, HWND hWnd) {
	HRESULT hr;

	//�f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_dinput->CreateDevice(GUID_SysMouse, &g_dimouse, NULL);
	if (FAILED(hr) || g_dimouse == NULL) {
		MessageBox(hWnd, "�}�E�X��������܂���ł����B", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//�f�[�^�t�H�[�}�b�g�ݒ�
	hr = g_dimouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) {
		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//�������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_dimouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr)) {
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "ERROR!", MB_ICONWARNING);
		return hr;
	}

	//�f�o�C�X�̐ݒ�
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = g_dimouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	//�}�E�X�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_dimouse->Acquire();

	return S_OK;
}

//================================================================
//�}�E�X�̏I������
//================================================================
void DirectInput::UninitMouse(void) {
	if (g_dimouse) {
		g_dimouse->Unacquire();
		g_dimouse->Release();
		g_dimouse = NULL;
	}
}

//================================================================
//�}�E�X�̍X�V����
//================================================================
HRESULT DirectInput::UpdateMouse(void) {
	HRESULT hr;
	DIMOUSESTATE2 mouseStateOld;

	//�O��̃f�[�^��ۑ�
	mouseStateOld = g_MouseState;
	ScreenToClient(g_hwnd, &g_MousePoint);

	//�f�o�C�X����f�[�^���擾
	hr = g_dimouse->GetDeviceState(sizeof(g_MouseState), &g_MouseState);
	if (SUCCEEDED(hr)) {
		//�g���K�[�̎擾
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
//�}�E�X�f�[�^�擾�i���v���X�j
//================================================================
bool DirectInput::GetMouseLeftPress(void) {
	return ((g_MouseState.rgbButtons[0] & 0x80) != 0);
}

//================================================================
//�}�E�X�f�[�^�擾�i���g���K�[�j
//================================================================
bool DirectInput::GetMouseLeftTrigger(void) {
	return ((g_MouseStateTrigger.rgbButtons[0] & 0x80 != 0));
}

//================================================================
//�}�E�X�f�[�^�擾�i�E�v���X�j
//================================================================
bool DirectInput::GetMouseRightPress(void) {
	return ((g_MouseState.rgbButtons[1] & 0x80) != 0);
}

//================================================================
//�}�E�X�f�[�^�擾�i�E�g���K�[�j
//================================================================
bool DirectInput::GetMouseRightTrigger(void) {
	return ((g_MouseStateTrigger.rgbButtons[1] & 0x80) != 0);
}

//================================================================
//�}�E�X�f�[�^�擾�i�����v���X�j
//================================================================
bool DirectInput::GetMouseCenterPress(void) {
	return ((g_MouseState.rgbButtons[2] & 0x80) != 0);
}

//================================================================
//�}�E�X�f�[�^�擾�i�����g���K�[�j
//================================================================
bool DirectInput::GetMouseCenterTrigger(void) {
	return ((g_MouseStateTrigger.rgbButtons[2] & 0x80) != 0);
}

//================================================================
//�}�E�X�f�[�^�擾�i�w���ړ��j
//================================================================
long DirectInput::GetMouseAxisX(void) {
	return g_MouseState.lX;
}

//================================================================
//�}�E�X�f�[�^�擾�i�x���ړ��j
//================================================================
long DirectInput::GetMouseAxisY(void) {
	return g_MouseState.lY;
}

//================================================================
//�}�E�X�f�[�^�擾�i�y���ړ��j
//================================================================
long DirectInput::GetMouseAxisZ(void) {
	return g_MouseState.lZ;
}

//================================================================
//�}�E�X���W�擾�i�w�j
//================================================================
long DirectInput::GetMouseX(void) {
	return g_MousePoint.x;
}

//================================================================
//�}�E�X���W�擾�i�x�j
//================================================================
long DirectInput::GetMouseY(void) {
	return g_MousePoint.y;
}