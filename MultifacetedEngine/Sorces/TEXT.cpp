#include "TEXT.h"
#include "ERROR.h"

//===================================================
//�R���X�g���N�^
//===================================================
Text::Text() {

}

bool Text::InitText() {
	time = 0;
	if (font == nullptr) {
		font = new Font;
	}
	font->ChangeFontSize(14, 14);
	TCreateFont();
	ChangeTextPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	ChangeText(L"No Text");
	ChangeColor(255, 255, 255);
	TextLeft();
	return true;
}

bool Text::InitText(int font_width, int font_height, int r, int g, int b) {
	time = 0;
	if (font == nullptr) {
		font = new Font;			//�t�H���g�̍쐬
	}
	font->ChangeFontSize(font_width, font_height);				//�t�H���g�T�C�Y�ύX
	ChangeTextPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);		//�E�B���h�E�S�̕\���G���A
	ChangeText(L"No Text");	//�f�t�H���g�e�L�X�g
	ChangeColor(r, g, b);	//�����F�w��
	TextLeft();				//����
	return true;
}

Text::~Text() {
	if (font != NULL) {
		font->Release();		//������Ȃ��ƃ��������[�N���N����
	}
	else {
	}
}

//==================================================
//�t�H���g�쐬 ���K�v��......
//==================================================
void Text::TCreateFont() {
	////�t�H���g�ݒ�
	//D3DXCreateFont(
	//	GetDevice(),				// Direct3D�f�o�C�X
	//	size_height,				// ����
	//	size_width,					// ��
	//	FW_REGULAR,					// �t�H���g�̑��� ����
	//	NULL,						// ����
	//	FALSE,						// �Α�
	//	SHIFTJIS_CHARSET,			// �����Z�b�g
	//	OUT_DEFAULT_PRECIS,			// �o�͐��x�͕���
	//	PROOF_QUALITY,				// �����i�����d��
	//	FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
	//	TEXT("�l�r�@�o�S�V�b�N"),	// �t�H���g��
	//	&font);						// ID3DXFont�|�C���^
	font->FCreateFont();
}

//===================================================
//�o�b�t�@���̕�����S�\��
//===================================================
bool Text::AllDispText() {
	if (buff.size() > 0) {		//�o�b�t�@�Ƀf�[�^�������
		//time = 0;		//�J�E���g�������I������
		text += buff.substr(0, buff.size());	//�S�\��
		buff.clear();	//�o�b�t�@���폜
		return true;	//���s���ł���
	}
	else {		//�Ȃ����
		return false;	//�ł��Ȃ�����(�o�b�t�@���Ȃ�)
	}
}

//===================================
//�E�F�C�g�̐ݒ�
//===================================
void Text::SetWait(float _time) {
	wait = _time;
}

//=====================================================
//�`��
//=====================================================
void Text::TextDraw() {
	//�`��
	LPD3DXFONT fdata = *font->GetFont();				//�쐬�����t�H���g�̏����擾

	if (fdata != NULL) {
		fdata->DrawText(
			NULL,					// NULL
			text.c_str(),			// �`��e�L�X�g
			-1,						// �S�ĕ\��
			&rect,					// �\���͈�
			mode,					// �\���ʒu
			D3DCOLOR_XRGB(Red, Green, Blue)	// �F
		);
	}
	else {
		ErrorMessage::ErrorMessageBox(L"font���쐬����Ă��܂���", L"�t�H���g�G���[", MB_OK);
	}
}

void Text::ChangeTextPosition(int left, int top, int right, int bottom) {
	//�`��͈͐ݒ�
	//Left,Top,Right,Bottom
	rect = { left, top, right, bottom };
}

void Text::ChangeTextPosition(RECT _rect) {
	rect = _rect;
}

void Text::ChangeTextPositionAdd(float left, float top, float right, float bottom) {
	rect.left += left;
	rect.top += top;
	rect.right += right;
	rect.bottom += bottom;
}

void Text::ChangeTextPositionAdd(float _x, float _y) {
	rect.left += _x;
	rect.right += _x;
	rect.top += _y;
	rect.bottom += _y;
}

//===========================================================
//�����T�C�Y�ύX
//===========================================================
void Text::ChangeFontSize(int width, int height) {
	//size_height = height;
	//size_width = width;
	////�t�H���g���������A���Ȃ��ƃ��������[�N����
	//if (font != NULL) {
	//	font->Release();
	//	font = NULL;
	//}
	//
	////�t�H���g�ݒ�(�X�V)
	//D3DXCreateFont(
	//	GetDevice(),			// Direct3D�f�o�C�X
	//	size_height,			// ����
	//	size_width,				// ��
	//	FW_REGULAR,				// �t�H���g�̑��� ����
	//	NULL,					// ����
	//	FALSE,					// �Α�
	//	SHIFTJIS_CHARSET,		// �����Z�b�g
	//	OUT_DEFAULT_PRECIS,		// �o�͐��x�͕���
	//	PROOF_QUALITY,			// �����i�����d��
	//	FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
	//	TEXT("Consolas"),	// �t�H���g��
	//	&font);				// ID3DXFont�|�C���^
}

//=========================================
//�e�L�X�g�ύX
//=========================================
void Text::ChangeText(string t) {
	text = t;
}

//=========================================
//�o�b�t�@�ƕ\���^�C�~���O�̕ύX
//=========================================
void Text::ChangeText(float _time, string _text) {
	time = _time * 60;
	buff = _text;
	text.clear();
}

//=========================================
//�\���Ԋu��ύX����
//=========================================
void Text::ChangeSpeed(float _time) {
	time = _time;
}

//=========================================
//�����F�ύX
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
	font->Release();
	font = _font;
}

void Text::Update() {
	//-----------------------------
	//������1�ȏ゠���
	//-----------------------------
	if (buff.size() > 0) {
		if (currentTime - wait < time) {
			currentTime++;
		}
		else {
			text += buff.substr(0, 1);		//�擪�������o��
			buff.erase(buff.begin());
			currentTime = 0;
			wait = 0;
		}
	}
	else {
		
	}
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