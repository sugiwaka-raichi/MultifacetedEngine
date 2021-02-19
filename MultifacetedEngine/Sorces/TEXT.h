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
	wstring text;		//表示するテキスト
	wstring buff;		//表示していくテキスト
	int Red, Green, Blue;
	int mode;		//表示位置モード
	float time;
	float currentTime = 0;
	float wait = 0;
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
	void ChangeTextPosition(RECT _rect);
	void ChangeTextPositionAdd(float left, float top, float right, float bottom);
	void ChangeTextPositionAdd(float _x,float _y);
	void ChangeColor(int r, int g, int b);
	void ChangeFontSize(int, int);
	void ChangeText(wstring);

	//====================================
	//テキスト効果
	//====================================
	void ChangeText(float _time, wstring _text);
	void ChangeSpeed(float _time);
	bool AllDispText();		//現バッファを全部表示する
	void SetWait(float _time);

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

