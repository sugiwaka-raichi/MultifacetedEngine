#pragma once
#include "Font.h"

enum TextPosMode {
	TM_CENTER,
	TM_LEFT,
	TM_RIGHT
};

//==============================
//テキストクラス
//==============================
class Text:public OBJECT
{
protected:
	int size_width, size_height;
	RECT rect;
	string text;
	int Red, Green, Blue;
	int mode;		//表示位置モード
public:
	//LPD3DXFONT font;		//フォント
	Font* font;
	Text();					//Default設定のコンストラクタのコンストラクタ
	//Text(int font_width, int font_height, int r, int g, int b);	//個別設定

	~Text();

	bool InitText();
	bool InitText(int font_width, int font_height, int r, int g, int b);
	void TCreateFont();
	//=====================================
	//変更処理
	//=====================================
	void ChangeTextPosition(int left, int top, int right, int bottom);
	void ChangeTextPositionAdd(float left, float top, float right, float bottom);
	void ChangeText(string);
	void ChangeColor(int r, int g, int b);
	void ChangeFontSize(int, int);

	//テキスト描画
	void TextDraw();

	//=========================
	//テキスト位置
	//=========================
	void TextCenter();
	void TextLeft();
	void TextRight();

	void SetFont(Font* _font);

	//-----------------------------------
	//オーバーライド
	//-----------------------------------
	void Init();
	void Draw();
	void Update();
	void UnInit();

};

