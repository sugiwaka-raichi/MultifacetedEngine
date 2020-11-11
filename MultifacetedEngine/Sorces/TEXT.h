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
	int mode;		//�\���ʒu���[�h
public:
	LPD3DXFONT font;		//�t�H���g
	Text();					//Default�ݒ�̃R���X�g���N�^�̃R���X�g���N�^
	//Text(int font_width, int font_height, int r, int g, int b);	//�ʐݒ�

	~Text();

	bool InitText();
	bool InitText(int font_width, int font_height, int r, int g, int b);
	void TCreateFont();
	//=====================================
	//�ύX����
	//=====================================
	void ChangeFontSize(int, int);
	void ChangeTextPosition(int left, int top, int right, int bottom);
	void ChangeTextPositionAdd(float left, float top, float right, float bottom);
	void ChangeText(string);
	void ChangeColor(int r, int g, int b);

	//�e�L�X�g�`��
	void TextDraw();

	//=========================
	//�e�L�X�g�ʒu
	//=========================
	void TextCenter();
	void TextLeft();
	void TextRight();

};

