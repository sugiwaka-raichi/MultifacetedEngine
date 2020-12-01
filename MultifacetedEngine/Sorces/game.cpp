//***********************************************
//�C���N���[�h
//***********************************************
#include "game.h"
#include "input.h"
#include "XAudio2.h"
#include "ERROR.h"
#include "SquarePolygon.h"
#include "TextureManager.h"
#include "TEXT.h"
#include "Button.h"

//==================================================
//�O���[�o���ϐ�
//==================================================
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
SquarePolygon test(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,100,100);
TextureManager TexManager;

Text testtex;
Button button;

vector<OBJECT*> stack;			//�I�u�W�F�N�g�����s���邽�߂̔�

bool GameInit(HINSTANCE hinst, HWND hWnd, int width, int height, bool fullscreen) {
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	bool sts;

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));				//���[�N���O�N���A
	d3dpp.BackBufferWidth = width;					//�Q�[����ʃT�C�Y�i���j
	d3dpp.BackBufferHeight = height;				//�Q�[����ʃT�C�Y�i�����j
	d3dpp.BackBufferFormat = d3ddm.Format;			//�J���[���[�h�w��
	d3dpp.BackBufferCount = 1;						//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		//�f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = !fullscreen;					//�E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;			//�f�u�X�o�b�t�@(Z�o�b�t�@)�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		//�f�u�X�o�b�t�@�Ƃ���16bit���g��
	
	if (fullscreen) {
		//�E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;		//���t���b�V�����[�g�i�w��ł��Ȃ��̂łO�Œ�j
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		//�C���^�[�o��(VSync��҂����ɕ`��)
	}
	else {
		//�t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		//���t���b�V�����[�g�i���݂̑��x�ɍ��킹��j
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		//�C���^�[�o��(VSync��҂��ĕ`��)
	}
	
	//�f�o�C�X�I�u�W�F�N�g�쐬
	//[�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs��
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		//�f�B�X�v���C�A�_�v�^�[
		D3DDEVTYPE_HAL,										//�f�o�C�X�^�C�v
		hWnd,												//�t�H�[�J�X����E�B���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,				//�f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,
		&g_pD3DDevice))) {
		//��L�̐ݒ肪���s�����Ƃ�
		//[�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs��
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_REF,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice))) {
			//���������s
			return false;
		}
	}
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//�e�N�X�`���̓����F�𔽉f
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	InitInput(hinst, hWnd);
	InitSound();

	LoadWave(TEXT("./Asset/Sound/BGM/"), TEXT("play.wav"), true);
	TextureManager::FileDataInit();
	TextureManager::TextureLoad(TEXT("IMG_0914.PNG"));		//�e�N�X�`���ǂݍ���
	test.SetTexture(TextureManager::GetTexture(TEXT("IMG_0914.PNG")));
	testtex.InitText();

	//�e�X�g
	test.SetU(1);
	test.SetV(1);
	test.SetUP(1);
	test.SetVP(1);
	testtex.InitText();
	button.CreatePolygon(100, 100, 100, 100);

	stack.push_back(&button);
	stack.push_back(&test);
	stack.push_back(&testtex);
	return true;
}

void GameUpdate() {
	//-----------------------------
	//���s����
	//-----------------------------
	

	//#####################
	//test
	//#####################
	static bool soundflg = false;
	if (GetKeyboardTrigger(DIK_SPACE)) {
		soundflg = true;
	}
	if (soundflg) {
		/*xaudio.*/PlaySound(TEXT("BGM"), TEXT("play.wav"));
		soundflg = false;
	}

	static int ang = 0;
	ang++;
	test.SetAngle(10 + ang);
	
	if (ang % 601 == 120) {
		StopSound(TEXT("./Asset/Sound/BGM/"), TEXT("play.wav"));
	}
	testtex.ChangeTextPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (GetMousePress(MOUSE_KEYTYPE::BACK)) {
		testtex.ChangeText(L"back");	//back
	}
	if (GetMousePress(MOUSE_KEYTYPE::FORWARD)) {
		testtex.ChangeText(L"forward"); //fo
	}

	if (GetMousePress(MOUSE_KEYTYPE::LEFT)) {
		testtex.ChangeText(to_wstring(GetMouseScreenX()) + L":" + to_wstring(GetMouseScreenY()));
	}

	if (button.OnCursor()) {
		button.SetColor(D3DCOLOR_RGBA(255,0,0,255));
		if (button.PushKey(MOUSE_KEYTYPE::LEFT)) {
			button.SetColor(D3DCOLOR_RGBA(0, 255, 0, 255));
			testtex.ChangeText(L"���");
		}
	}
	else {
		button.SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
	//########################
	//End test
	//########################
}

void GameRender() {
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	
	//Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene())) {
		g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		//g_pD3DDevice->SetTexture(0, TextureManager::GetTexture("IMG_0914.PNG"));
		//button.DrawPolygon();
		//test.DrawPolygon(TextureManager::GetTexture(TEXT("IMG_0914.PNG")));
		//testtex.TextDraw();
		for (int i = 0; i < stack.size(); i++) {
			stack[i]->Draw();		//�X�^�b�N�ɂ���`�悷��I�u�W�F�N�g�����ɕ`��
		}

		g_pD3DDevice->EndScene();
	}
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void GameMain() {
	GameInput();		//����
	GameUpdate();		//�X�V
	GameRender();		//�`��
}

void GameExit() {
	
}

void GameInput() {
	UpdateInput();
}

LPDIRECT3DDEVICE9 GetDevice() {
	return g_pD3DDevice;
}