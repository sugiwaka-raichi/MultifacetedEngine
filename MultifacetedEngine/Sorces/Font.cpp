#include "TEXT.h"

//===========================================================
//コンストラクタ
//===========================================================
Font::Font() {
	fontSizeW = fontSizeH = 10;		//フォントサイズ10x10
	lineSize = FW_REGULAR;			//線の太さ 普通
	underLine = false;				//下線は無し
	italic = false;					//斜体にしない
	fontName = L"ＭＳ　Ｐゴシック";	//ＭＳ　Ｐゴシックを使う
	FCreateFont();
}

//===========================================================
//フォントサイズ変更
//===========================================================
void Font::ChangeFontSize(float _w, float _h) {
	fontSizeW = _w;
	fontSizeH = _h;
	FCreateFont();
}

//===========================================================
//フォントデータを変える
//===========================================================
void Font::ChangeFontData(LPCWSTR _fontName) {
	fontName = _fontName;
	FCreateFont();
}

//===========================================================
//フォントデータ作成
//===========================================================
void Font::FCreateFont() {
	if (font != nullptr) {
		Release();		//解放処理
	}
	//フォント設定
	D3DXCreateFont(
		GetDevice(),				// Direct3Dデバイス
		fontSizeH,					// 高さ
		fontSizeW,					// 幅
		lineSize,					// フォントの太さ
		underLine,					// 下線
		italic,						// 斜体
		SHIFTJIS_CHARSET,			// 文字セット
		OUT_DEFAULT_PRECIS,			// 出力制度は普通
		PROOF_QUALITY,				// 文字品質を重視
		FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
		fontName,					// フォント名
		&font);						// ID3DXFontポインタ
}

//==============================================
//解放処理
//==============================================
void Font::Release() {
	font->Release();
}

//==============================================
//フォントデータ取得
//==============================================
LPD3DXFONT* Font::GetFont() {
	return &font;
}
