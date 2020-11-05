#pragma once
#include "game.h"

enum TextMode {
	TM_CENTER,
	TM_LEFT,
	TM_RIGHT
};

class Text
{
protected:
	int size_width, size_height;
	RECT rect;
	string text;
	int Red, Green, Blue;
	int mode;		//表示位置モード
public:
	LPD3DXFONT font;		//フォント
	Text();					//Default設定のコンストラクタのコンストラクタ
	//Text(int font_width, int font_height, int r, int g, int b);	//個別設定

	~Text();

	bool InitText();
	bool InitText(int font_width, int font_height, int r, int g, int b);
	void TCreateFont();
	//=====================================
	//変更処理
	//=====================================
	void ChangeFontSize(int, int);
	void ChangeTextPosition(int left, int top, int right, int bottom);
	void ChangeTextPositionAdd(float left, float top, float right, float bottom);
	void ChangeText(string);
	void ChangeColor(int r, int g, int b);

	//テキスト描画
	void TextDraw();

	//=========================
	//テキスト位置
	//=========================
	void TextCenter();
	void TextLeft();
	void TextRight();

};

