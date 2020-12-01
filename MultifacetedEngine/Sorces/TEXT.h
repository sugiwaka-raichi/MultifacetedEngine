#pragma once
#include "Font.h"

enum TextPosMode {
	TM_CENTER,
	TM_LEFT,
	TM_RIGHT
};

//==============================
//�e�L�X�g�N���X
//==============================
class Text:public OBJECT
{
protected:
	int size_width, size_height;
	RECT rect;
	string text;
	int Red, Green, Blue;
	int mode;		//�\���ʒu���[�h
public:
	//LPD3DXFONT font;		//�t�H���g
	Font* font;
	Text();					//Default�ݒ�̃R���X�g���N�^�̃R���X�g���N�^
	//Text(int font_width, int font_height, int r, int g, int b);	//�ʐݒ�

	~Text();

	bool InitText();
	bool InitText(int font_width, int font_height, int r, int g, int b);
	void TCreateFont();
	//=====================================
	//�ύX����
	//=====================================
	void ChangeTextPosition(int left, int top, int right, int bottom);
	void ChangeTextPositionAdd(float left, float top, float right, float bottom);
	void ChangeText(string);
	void ChangeColor(int r, int g, int b);
	void ChangeFontSize(int, int);

	//�e�L�X�g�`��
	void TextDraw();

	//=========================
	//�e�L�X�g�ʒu
	//=========================
	void TextCenter();
	void TextLeft();
	void TextRight();

	void SetFont(Font* _font);

	//-----------------------------------
	//�I�[�o�[���C�h
	//-----------------------------------
	void Init();
	void Draw();
	void Update();
	void UnInit();

};

