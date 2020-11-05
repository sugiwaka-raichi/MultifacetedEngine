//*******************************************
//include
//*******************************************
#include "Win.h"
#include "game.h"

//*******************************************
//�O���[�o���ϐ�
//*******************************************
int g_nCountFPS = 0;
float g_dispFPS = 0;
HWND			hWnd;						//�E�B���h�E�n���h��


//*******************************************
//�v���g�^�C�v�錾
//*******************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				//�E�B���h�E�v���V�[�W��

//======================================================================
//���C���֐�
//hInstance		:	���̃A�v���P�[�V�����̃C���X�^���X�n���h��(���ʎq)
//hPrevInstance	:	���NULL(16bit����̖��c)
//lpCmdLine		:	�R�}���h���C������󂯎���������ւ̃|�C���^
//nCmdShow		:	�A�v���P�[�V���������E�B���h�D�\�����@�̎w��
//======================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG				msg;						//���b�Z�[�W�\����
	WNDCLASSEX		wcex;						//�E�B���h�E�N���X�\����
	int				width = SCREEN_WIDTH;		//�E�B���h�E�̕� �v�Z�p���[�N
	int				height = SCREEN_HEIGHT;		//�E�B���h�E�̍��� �v�Z�p���[�N
	DWORD			dwExecLastTime;				//�Ō�Ɏ��s��������
	DWORD			dwFPSLastTime;				//�Ō�Ɍv����������
	DWORD			dwCurrentTime;				//���ݎ���
	DWORD			dwFrameCount;				//�t���[����

	//���������[�N�����m
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//�E�B���h�E�N���X���̃Z�b�g
	wcex.hInstance		= hInstance;			//�C���X�^���X�l�̃Z�b�g
	wcex.lpszClassName  = CLASS_NAME;			//�N���X��
	wcex.lpfnWndProc	= (WNDPROC)WndProc;		//�E�B���h�E���b�Z�[�W�֐�
	wcex.style			= 0;					//�E�B���h�E�X�^�C��
	wcex.cbSize			= sizeof(WNDCLASSEX);	//�\���̂̃T�C�Y
	wcex.hIcon			= LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);	//���[�W�A�C�R��
	wcex.hIconSm		= LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);		//�X���[���A�C�R��
	wcex.hCursor		= LoadCursor((HINSTANCE)NULL, IDC_ARROW);		//�J�[�\���X�^�C��
	wcex.lpszMenuName	= 0;					//���j���[�Ȃ�
	wcex.cbClsExtra		= 0;					//�G�L�X�g���Ȃ�
	wcex.cbWndExtra		= 0;					//
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//�w�i��

	if (!RegisterClassEx(&wcex)) return FALSE;		//�E�B���h�E�N���X�̓o�^

	if (FULLSCREEN) {
		hWnd = CreateWindow(
			CLASS_NAME,						//�E�B���h�E�N���X�̖��O
			WINDOW_NAME,					//�^�C�g��
			WS_VISIBLE | WS_POPUP,			//�E�B���h�E�X�^�C��
			0, 0,							//�E�B���h�E�ʒu �c�C��
			SCREEN_WIDTH, SCREEN_HEIGHT,	//�E�B���h�E�T�C�Y
			NULL,							//�e�E�B���h�E�Ȃ�
			(HMENU)nullptr,					//���j���[�Ȃ�
			hInstance,						//�C���X�^���X�n���h��
			(LPVOID)nullptr					//�ǉ�����
		);
	}
	else {
		//�^�C�g���o�[�ƃE�B���h�E�g�̕��܂߂ăE�B���h�E�T�C�Y��ݒ�
		RECT rect;
		SetRect(&rect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		int window_width = rect.right - rect.left;
		int window_height = rect.bottom - rect.top;
		rect.top = 0;
		rect.left = 0;
		hWnd = CreateWindowEx(0,				//�g���E�B���h�E�X�^�C��
			CLASS_NAME,						//�E�B���h�E�N���X�̖��O
			WINDOW_NAME,					//�^�C�g��
			WS_OVERLAPPEDWINDOW,			//�E�B���h�E�X�^�C��
			CW_USEDEFAULT,					//�E�B���h�E�̍���X���W
			CW_USEDEFAULT,					//�E�B���h�E�̍���Y���W
			window_width,					//�E�B���h�E�̕�
			window_height,					//�E�B���h�E�̍���
			nullptr,						//�e�E�B���h�E�̃n���h��
			nullptr,						//���j���[�n���h���܂��͎q�E�B���h�EID
			hInstance,						//�C���X�^���X�n���h��
			nullptr							//�E�B���h�E�쐬�f�[�^
		);

	}

	if (!hWnd) return FALSE;

	timeBeginPeriod(1);				//�^�C�}�̕���\�͂�1ms�ɂ���
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;
	//�E�B���h�E��\������
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//�Q�[���̏�������
	if (!GameInit(hInstance, hWnd, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN)) {
		GameExit();
		MessageBox(hWnd, TEXT("ERROR"), TEXT("GameInit Error"), MB_OK);
		return false;
	}

	//���b�Z�[�W���[�v
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {			//���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
			//Windows�̏���
			if (msg.message == WM_QUIT) {
				//PostQuitMessage()���Ă΂�AWM_QUIT���b�Z�[�W�������烋�[�v�I��
				break;
			}
			else {
				//���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			//DirectX�̏���

			//-----------------------
			//FPS��艻����
			//-----------------------
			dwCurrentTime = timeGetTime();			//���ݎ������擾
			//0.5�b���ƂɎ��s
			if ((dwCurrentTime - dwFPSLastTime) > 500) {
				//FPS���v�Z
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				//FPS�v�����ԂɌ��ݎ������Z�b�g
				dwFPSLastTime = dwCurrentTime;

				//�t���[�������[���N���A
				dwFrameCount = 0;
			}

			//16ms�o�߂�����
			if ((dwCurrentTime - dwExecLastTime) >= (1000.0 / 60.0)) {
				//�ŏI���s���ԂɌ��ݎ��Ԃ��Z�b�g
				dwExecLastTime = dwCurrentTime;
				GameMain();
				//�t���[�������C���N�������g
				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);			//�^�C�}�̕���\�͌��ɖ߂�
	GameExit();
	return (int)msg.wParam;
}

//==================================================
//WindowProc
//hwnd			:		�E�B���h�E�n���h��
//message		:		���b�Z�[�W���ʎq
//wParam		:		�t�я��1
//lParam		:		�t�я��2
//==================================================
LRESULT WINAPI WndProc(HWND _hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_KEYDOWN:
		switch (wParam) {
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(_hwnd, message, wParam, lParam);
	}
	return 0;
}

HWND GetHandle() {
	return hWnd;
}
