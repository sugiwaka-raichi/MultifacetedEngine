#include "TEXT.h"
#include "ERROR.h"

//===================================================
//コンストラクタ
//===================================================
Text::Text() {

}

bool Text::InitText() {
	if (font == nullptr) {
		font = new Font;
	}
	font->ChangeFontSize(14, 14);
	TCreateFont();
	ChangeTextPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	ChangeText(TEXT("No Text"));
	ChangeColor(255, 255, 255);
	TextLeft();
	return true;
}

bool Text::InitText(int font_width, int font_height, int r, int g, int b) {
	if (font == nullptr) {
		font = new Font;			//フォントの作成
	}
	font->ChangeFontSize(font_width, font_height);
	ChangeTextPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	ChangeText(L"No Text");
	ChangeColor(r, g, b);
	TextLeft();
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
//フォント作成作成
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

void Text::ChangeTextPositionAdd(float left, float top, float right, float bottom) {
	rect.left += left;
	rect.top += top;
	rect.right += right;
	rect.bottom += bottom;
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
	font = _font;
}

void Text::Update() {

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