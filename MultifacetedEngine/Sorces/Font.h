#pragma once
#include "game.h"

//==============================
//フォントクラス
//==============================
class Font {
private:
	float fontSizeW, fontSizeH;		//フォントの大きさ
	int lineSize;					//線の太さ
	bool underLine;					//下線
	bool italic;					//斜体
	LPCWSTR fontName;				//使用するフォントの名前

	LPD3DXFONT font = nullptr;				//作成したフォントの情報
public:
	Font();

	void ChangeFontSize(float, float);			//フォントサイズの変更
	void ChangeFontData(LPCWSTR _fontMame);		//文字フォント変更
	void FCreateFont();
	void Release();

	LPD3DXFONT* GetFont();
};