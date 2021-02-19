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
	wstring text;		//�\������e�L�X�g
	wstring buff;		//�\�����Ă����e�L�X�g
	int Red, Green, Blue;
	int mode;		//�\���ʒu���[�h
	float time;
	float currentTime = 0;
	float wait = 0;
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
	void ChangeTextPosition(RECT _rect);
	void ChangeTextPositionAdd(float left, float top, float right, float bottom);
	void ChangeTextPositionAdd(float _x,float _y);
	void ChangeColor(int r, int g, int b);
	void ChangeFontSize(int, int);
	void ChangeText(wstring);

	//====================================
	//�e�L�X�g����
	//====================================
	void ChangeText(float _time, wstring _text);
	void ChangeSpeed(float _time);
	bool AllDispText();		//���o�b�t�@��S���\������
	void SetWait(float _time);

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

