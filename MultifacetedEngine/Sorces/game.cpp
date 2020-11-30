//***********************************************
//インクルード
//***********************************************
#include "game.h"
#include "input.h"
#include "XAudio2.h"
#include "ERROR.h"
#include "SquarePolygon.h"
#include "TextureManager.h"
#include "TEXT.h"

//==================================================
//グローバル変数
//==================================================
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPD3DXFONT	g_pD3DXFont = nullptr;		//フォント
//DirectInput xinput;
//XAudio2 xaudio;
SquarePolygon test(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,100,100);
TextureManager TexManager;
Text testText;

bool GameInit(HINSTANCE hinst, HWND hWnd, int width, int height, bool fullscreen) {
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	bool sts;

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));				//ワークを０クリア
	d3dpp.BackBufferWidth = width;					//ゲーム画面サイズ（幅）
	d3dpp.BackBufferHeight = height;				//ゲーム画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;			//カラーモード指定
	d3dpp.BackBufferCount = 1;						//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		//映像信号に同期してフリップする
	d3dpp.Windowed = !fullscreen;					//ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;			//デブスバッファ(Zバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		//デブスバッファとして16bitを使う
	
	if (fullscreen) {
		//ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;		//リフレッシュレート（指定できないので０固定）
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		//インターバル(VSyncを待たずに描画)
	}
	else {
		//フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		//リフレッシュレート（現在の速度に合わせる）
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		//インターバル(VSyncを待って描画)
	}
	
	//デバイスオブジェクト作成
	//[デバイス作成制御]<描画>と<頂点処理>をハードウェアで行う
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		//ディスプレイアダプター
		D3DDEVTYPE_HAL,										//デバイスタイプ
		hWnd,												//フォーカスするウィンドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,				//デバイス作成制御の組み合わせ
		&d3dpp,
		&g_pD3DDevice))) {
		//上記の設定が失敗したとき
		//[デバイス作成制御]<描画>と<頂点処理>をCPUで行う
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_REF,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice))) {
			//初期化失敗
			return false;
		}
	}
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//テクスチャの透明色を反映
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//フォント作成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);
	
	InitInput(hinst, hWnd);
	/*xaudio.*/InitSound();
	LoadWave("./Asset/Sound/BGM/", "play.wav", true);
	TextureManager::FileDataInit();
	TextureManager::TextureLoad("IMG_0914.PNG");

	//テスト
	test.SetU(1);
	test.SetV(1);
	test.SetUP(1);
	test.SetVP(1);
	testText.InitText();
	return true;
}

void GameUpdate() {
	//#####################
	//test
	//#####################
	static bool soundflg = false;
	if (GetKeyboardTrigger(DIK_SPACE)) {
		soundflg = true;
	}
	if (soundflg) {
		/*xaudio.*/PlaySound("./Asset/Sound/BGM/","play.wav");
		soundflg = false;
	}

	static int ang = 0;
	ang++;
	test.SetAngle(10 + ang);
	
	if (ang % 601 == 120) {
		StopSound("./Asset/Sound/BGM/", "play.wav");
	}
	
}

void GameRender() {
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	
	//Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene())) {
		g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		//g_pD3DDevice->SetTexture(0, TextureManager::GetTexture("IMG_0914.PNG"));
		test.DrawPolygon(TextureManager::GetTexture("IMG_0914.PNG"));
		testText.TextDraw();
		g_pD3DDevice->EndScene();
	}
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void GameMain() {
	GameInput();		//入力
	GameUpdate();		//更新
	GameRender();		//描画
}

void GameExit() {
	
}

void GameInput() {
	UpdateInput();
}

LPDIRECT3DDEVICE9 GetDevice() {
	return g_pD3DDevice;
}