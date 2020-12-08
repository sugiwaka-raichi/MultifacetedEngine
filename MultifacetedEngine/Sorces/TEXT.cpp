#include "TEXT.h"
#include "ERROR.h"

//===================================================
//コンストラクタ
//===================================================
Text::Text() {

}

bool Text::InitText() {
	time = 0;
	if (font == nullptr) {
		font = new Font;
	}
	font->ChangeFontSize(14, 14);
	TCreateFont();
	ChangeTextPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	ChangeText(L"No Text");
	ChangeColor(255, 255, 255);
	TextLeft();
	return true;
}

bool Text::InitText(int font_width, int font_height, int r, int g, int b) {
	time = 0;
	if (font == nullptr) {
		font = new Font;			//フォントの作成
	}
	font->ChangeFontSize(font_width, font_height);				//フォントサイズ変更
	ChangeTextPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);		//ウィンドウ全体表示エリア
	ChangeText(L"No Text");	//デフォルトテキスト
	ChangeColor(r, g, b);	//文字色指定
	TextLeft();				//左寄せ
	return true;
}

Text::~Text() {
	if (font != NULL) {
		font->Release();		//解放しないとメモリリークを起こす
	}
	else {
	}
}

//==================================================
//フォント作成 ※必要性......
//==================================================
void Text::TCreateFont() {
	////フォント設定
	//D3DXCreateFont(
	//	GetDevice(),				// Direct3Dデバイス
	//	size_height,				// 高さ
	//	size_width,					// 幅
	//	FW_REGULAR,					// フォントの太さ 普通
	//	NULL,						// 下線
	//	FALSE,						// 斜体
	//	SHIFTJIS_CHARSET,			// 文字セット
	//	OUT_DEFAULT_PRECIS,			// 出力制度は普通
	//	PROOF_QUALITY,				// 文字品質を重視
	//	FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
	//	TEXT("ＭＳ　Ｐゴシック"),	// フォント名
	//	&font);						// ID3DXFontポインタ
	font->FCreateFont();
}

//===================================================
//バッファ内の文字を全表示
//===================================================
bool Text::AllDispText() {
	if (buff.size() > 0) {		//バッファにデータがあれば
		//time = 0;		//カウントを強制終了する
		text += buff.substr(0, buff.size());	//全表示
		buff.clear();	//バッファを削除
		return true;	//実行ができた
	}
	else {		//なければ
		return false;	//できなかった(バッファがない)
	}
}

//===================================
//ウェイトの設定
//===================================
void Text::SetWait(float _time) {
	wait = _time;
}

//=====================================================
//描画
//=====================================================
void Text::TextDraw() {
	//描画
	LPD3DXFONT fdata = *font->GetFont();				//作成したフォントの情報を取得

	if (fdata != NULL) {
		fdata->DrawText(
			NULL,					// NULL
			text.c_str(),			// 描画テキスト
			-1,						// 全て表示
			&rect,					// 表示範囲
			mode,					// 表示位置
			D3DCOLOR_XRGB(Red, Green, Blue)	// 色
		);
	}
	else {
		ErrorMessage::ErrorMessageBox(L"fontが作成されていません", L"フォントエラー", MB_OK);
	}
}

void Text::ChangeTextPosition(int left, int top, int right, int bottom) {
	//描画範囲設定
	//Left,Top,Right,Bottom
	rect = { left, top, right, bottom };
}

void Text::ChangeTextPosition(RECT _rect) {
	rect = _rect;
}

void Text::ChangeTextPositionAdd(float left, float top, float right, float bottom) {
	rect.left += left;
	rect.top += top;
	rect.right += right;
	rect.bottom += bottom;
}

void Text::ChangeTextPositionAdd(float _x, float _y) {
	rect.left += _x;
	rect.right += _x;
	rect.top += _y;
	rect.bottom += _y;
}

//===========================================================
//文字サイズ変更
//===========================================================
void Text::ChangeFontSize(int width, int height) {
	//size_height = height;
	//size_width = width;
	////フォントを解放する、しないとメモリリークする
	//if (font != NULL) {
	//	font->Release();
	//	font = NULL;
	//}
	//
	////フォント設定(更新)
	//D3DXCreateFont(
	//	GetDevice(),			// Direct3Dデバイス
	//	size_height,			// 高さ
	//	size_width,				// 幅
	//	FW_REGULAR,				// フォントの太さ 普通
	//	NULL,					// 下線
	//	FALSE,					// 斜体
	//	SHIFTJIS_CHARSET,		// 文字セット
	//	OUT_DEFAULT_PRECIS,		// 出力制度は普通
	//	PROOF_QUALITY,			// 文字品質を重視
	//	FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
	//	TEXT("Consolas"),	// フォント名
	//	&font);				// ID3DXFontポインタ
}

//=========================================
//テキスト変更
//=========================================
void Text::ChangeText(string t) {
	text = t;
}

//=========================================
//バッファと表示タイミングの変更
//=========================================
void Text::ChangeText(float _time, string _text) {
	time = _time * 60;
	buff = _text;
	text.clear();
}

//=========================================
//表示間隔を変更する
//=========================================
void Text::ChangeSpeed(float _time) {
	time = _time;
}

//=========================================
//文字色変更
//=========================================
void Text::ChangeColor(int r, int g, int b) {
	Red = r;
	Green = g;
	Blue = b;
}

void Text::TextCenter() {
	mode = DT_CENTER | DT_VCENTER;
}

void Text::TextLeft() {
	mode = DT_LEFT;
}

void Text::TextRight() {
	mode = DT_RIGHT;
}

void Text::SetFont(Font * _font) {
	font->Release();
	font = _font;
}

void Text::Update() {
	//-----------------------------
	//文字列が1以上あれば
	//-----------------------------
	if (buff.size() > 0) {
		if (currentTime - wait < time) {
			currentTime++;
		}
		else {
			text += buff.substr(0, 1);		//先頭文字取り出し
			buff.erase(buff.begin());
			currentTime = 0;
			wait = 0;
		}
	}
	else {
		
	}
}

void Text::UnInit()
{
}

void Text::Init()
{
}

void Text::Draw() {
	TextDraw();
}