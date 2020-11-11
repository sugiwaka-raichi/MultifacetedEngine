#include "TEXT.h"

//===================================================
//コンストラクタ
//===================================================
Text::Text() {

}

bool Text::InitText() {
	size_width = 14;
	size_height = 14;
	TCreateFont();
	ChangeTextPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	ChangeText(TEXT("No Text"));
	ChangeColor(255, 255, 255);
	mode = TextMode::TM_LEFT;
	return true;
}

bool Text::InitText(int font_width, int font_height, int r, int g, int b) {
	ChangeFontSize(font_width, font_height);
	ChangeTextPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	ChangeText(TEXT("No Text"));
	ChangeColor(r, g, b);
	mode = TextMode::TM_LEFT;
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
	//フォント設定
	D3DXCreateFont(
		GetDevice(),				// Direct3Dデバイス
		size_height,				// 高さ
		size_width,					// 幅
		FW_REGULAR,					// フォントの太さ 普通
		NULL,						// 下線
		FALSE,						// 斜体
		SHIFTJIS_CHARSET,			// 文字セット
		OUT_DEFAULT_PRECIS,			// 出力制度は普通
		PROOF_QUALITY,				// 文字品質を重視
		FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
		TEXT("ＭＳ　Ｐゴシック"),	// フォント名
		&font);						// ID3DXFontポインタ
}

//=====================================================
//描画
//=====================================================
void Text::TextDraw() {
	LPCSTR disp_text = (LPCSTR)text.c_str();
	//描画
	if (font != NULL) {
		switch (mode) {
		case TextMode::TM_CENTER:
			font->DrawText(
				NULL,					// NULL
				disp_text,			// 描画テキスト
				-1,						// 全て表示
				&rect,						// 表示範囲
				DT_CENTER | DT_VCENTER,		// 中央
				D3DCOLOR_XRGB(Red, Green, Blue)	// 色
			);
			break;
		case TextMode::TM_RIGHT:
			font->DrawText(
				NULL,					// NULL
				disp_text,			// 描画テキスト
				-1,						// 全て表示
				&rect,					// 表示範囲
				DT_RIGHT,				// 右揃え
				D3DCOLOR_XRGB(Red, Green, Blue)	// 色
			);
			break;
		case TextMode::TM_LEFT:
		default:
			font->DrawText(
				NULL,					// NULL
				disp_text,			// 描画テキスト
				-1,						// 全て表示
				&rect,						// 表示範囲
				DT_LEFT,					// 左寄せ
				D3DCOLOR_XRGB(Red, Green, Blue)	// 色
			);
			break;
		}
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
	size_height = height;
	size_width = width;
	//フォントを解放する、しないとメモリリークする
	if (font != NULL) {
		font->Release();
		font = NULL;
	}
	
	//フォント設定(更新)
	D3DXCreateFont(
		GetDevice(),			// Direct3Dデバイス
		size_height,			// 高さ
		size_width,				// 幅
		FW_REGULAR,				// フォントの太さ 普通
		NULL,					// 下線
		FALSE,					// 斜体
		SHIFTJIS_CHARSET,		// 文字セット
		OUT_DEFAULT_PRECIS,		// 出力制度は普通
		PROOF_QUALITY,			// 文字品質を重視
		FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
		TEXT("Consolas"),	// フォント名
		&font);				// ID3DXFontポインタ
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
	mode = TextMode::TM_CENTER;
}

void Text::TextLeft() {
	mode = TextMode::TM_LEFT;
}

void Text::TextRight() {
	mode = TextMode::TM_RIGHT;
}
