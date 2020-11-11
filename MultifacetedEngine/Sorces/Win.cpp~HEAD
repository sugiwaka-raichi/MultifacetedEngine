//*******************************************
//include
//*******************************************
#include "Win.h"
#include "game.h"

//*******************************************
//グローバル変数
//*******************************************
int g_nCountFPS = 0;
float g_dispFPS = 0;
HWND			hWnd;						//ウィンドウハンドル


//*******************************************
//プロトタイプ宣言
//*******************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				//ウィンドウプロシージャ

//======================================================================
//メイン関数
//hInstance		:	このアプリケーションのインスタンスハンドル(識別子)
//hPrevInstance	:	常にNULL(16bit時代の名残)
//lpCmdLine		:	コマンドラインから受け取った引数へのポインタ
//nCmdShow		:	アプリケーション初期ウィンドゥ表示方法の指定
//======================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG				msg;						//メッセージ構造体
	WNDCLASSEX		wcex;						//ウィンドウクラス構造体
	int				width = SCREEN_WIDTH;		//ウィンドウの幅 計算用ワーク
	int				height = SCREEN_HEIGHT;		//ウィンドウの高さ 計算用ワーク
	DWORD			dwExecLastTime;				//最後に実行した時間
	DWORD			dwFPSLastTime;				//最後に計測した時間
	DWORD			dwCurrentTime;				//現在時刻
	DWORD			dwFrameCount;				//フレーム数

	//メモリリークを検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//ウィンドウクラス情報のセット
	wcex.hInstance		= hInstance;			//インスタンス値のセット
	wcex.lpszClassName  = CLASS_NAME;			//クラス名
	wcex.lpfnWndProc	= (WNDPROC)WndProc;		//ウィンドウメッセージ関数
	wcex.style			= 0;					//ウィンドウスタイル
	wcex.cbSize			= sizeof(WNDCLASSEX);	//構造体のサイズ
	wcex.hIcon			= LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);	//ラージアイコン
	wcex.hIconSm		= LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);		//スモールアイコン
	wcex.hCursor		= LoadCursor((HINSTANCE)NULL, IDC_ARROW);		//カーソルスタイル
	wcex.lpszMenuName	= 0;					//メニューなし
	wcex.cbClsExtra		= 0;					//エキストラなし
	wcex.cbWndExtra		= 0;					//
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//背景白

	if (!RegisterClassEx(&wcex)) return FALSE;		//ウィンドウクラスの登録

	if (FULLSCREEN) {
		hWnd = CreateWindow(
			CLASS_NAME,						//ウィンドウクラスの名前
			WINDOW_NAME,					//タイトル
			WS_VISIBLE | WS_POPUP,			//ウィンドウスタイル
			0, 0,							//ウィンドウ位置 縦，横
			SCREEN_WIDTH, SCREEN_HEIGHT,	//ウィンドウサイズ
			NULL,							//親ウィンドウなし
			(HMENU)nullptr,					//メニューなし
			hInstance,						//インスタンスハンドル
			(LPVOID)nullptr					//追加引数
		);
	}
	else {
		//タイトルバーとウィンドウ枠の分含めてウィンドウサイズを設定
		RECT rect;
		SetRect(&rect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		int window_width = rect.right - rect.left;
		int window_height = rect.bottom - rect.top;
		rect.top = 0;
		rect.left = 0;
		hWnd = CreateWindowEx(0,				//拡張ウィンドウスタイル
			CLASS_NAME,						//ウィンドウクラスの名前
			WINDOW_NAME,					//タイトル
			WS_OVERLAPPEDWINDOW,			//ウィンドウスタイル
			CW_USEDEFAULT,					//ウィンドウの左上X座標
			CW_USEDEFAULT,					//ウィンドウの左上Y座標
			window_width,					//ウィンドウの幅
			window_height,					//ウィンドウの高さ
			nullptr,						//親ウィンドウのハンドル
			nullptr,						//メニューハンドルまたは子ウィンドウID
			hInstance,						//インスタンスハンドル
			nullptr							//ウィンドウ作成データ
		);

	}

	if (!hWnd) return FALSE;

	timeBeginPeriod(1);				//タイマの分解能力を1msにする
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;
	//ウィンドウを表示する
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//ゲームの初期処理
	if (!GameInit(hInstance, hWnd, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN)) {
		GameExit();
		MessageBox(hWnd, TEXT("ERROR"), TEXT("GameInit Error"), MB_OK);
		return false;
	}

	//メッセージループ
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {			//メッセージを取得しなかった場合"0"を返す
			//Windowsの処理
			if (msg.message == WM_QUIT) {
				//PostQuitMessage()が呼ばれ、WM_QUITメッセージが来たらループ終了
				break;
			}
			else {
				//メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			//DirectXの処理

			//-----------------------
			//FPS一定化処理
			//-----------------------
			dwCurrentTime = timeGetTime();			//現在時刻を取得
			//0.5秒ごとに実行
			if ((dwCurrentTime - dwFPSLastTime) > 500) {
				//FPSを計算
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				//FPS計測時間に現在時刻をセット
				dwFPSLastTime = dwCurrentTime;

				//フレーム数をゼロクリア
				dwFrameCount = 0;
			}

			//16ms経過したら
			if ((dwCurrentTime - dwExecLastTime) >= (1000.0 / 60.0)) {
				//最終実行時間に現在時間をセット
				dwExecLastTime = dwCurrentTime;
				GameMain();
				//フレーム数をインクリメント
				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);			//タイマの分解能力元に戻す
	GameExit();
	return (int)msg.wParam;
}

//==================================================
//WindowProc
//hwnd			:		ウィンドウハンドル
//message		:		メッセージ識別子
//wParam		:		付帯情報1
//lParam		:		付帯情報2
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
