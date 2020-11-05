#include "TEXT.h"

//===================================================
//�R���X�g���N�^
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
		font->Release();		//������Ȃ��ƃ��������[�N���N����
	}
	else {
	}
}

//==================================================
//�t�H���g�쐬�쐬
//==================================================
void Text::TCreateFont() {
	//�t�H���g�ݒ�
	D3DXCreateFont(
		GetDevice(),				// Direct3D�f�o�C�X
		size_height,				// ����
		size_width,					// ��
		FW_REGULAR,					// �t�H���g�̑��� ����
		NULL,						// ����
		FALSE,						// �Α�
		SHIFTJIS_CHARSET,			// �����Z�b�g
		OUT_DEFAULT_PRECIS,			// �o�͐��x�͕���
		PROOF_QUALITY,				// �����i�����d��
		FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
		TEXT("�l�r�@�o�S�V�b�N"),	// �t�H���g��
		&font);						// ID3DXFont�|�C���^
}

//=====================================================
//�`��
//=====================================================
void Text::TextDraw() {
	LPCSTR disp_text = (LPCSTR)text.c_str();
	//�`��
	if (font != NULL) {
		switch (mode) {
		case TextMode::TM_CENTER:
			font->DrawText(
				NULL,					// NULL
				disp_text,			// �`��e�L�X�g
				-1,						// �S�ĕ\��
				&rect,						// �\���͈�
				DT_CENTER | DT_VCENTER,		// ����
				D3DCOLOR_XRGB(Red, Green, Blue)	// �F
			);
			break;
		case TextMode::TM_RIGHT:
			font->DrawText(
				NULL,					// NULL
				disp_text,			// �`��e�L�X�g
				-1,						// �S�ĕ\��
				&rect,					// �\���͈�
				DT_RIGHT,				// �E����
				D3DCOLOR_XRGB(Red, Green, Blue)	// �F
			);
			break;
		case TextMode::TM_LEFT:
		default:
			font->DrawText(
				NULL,					// NULL
				disp_text,			// �`��e�L�X�g
				-1,						// �S�ĕ\��
				&rect,						// �\���͈�
				DT_LEFT,					// ����
				D3DCOLOR_XRGB(Red, Green, Blue)	// �F
			);
			break;
		}
	}
}

void Text::ChangeTextPosition(int left, int top, int right, int bottom) {
	//�`��͈͐ݒ�
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
//�����T�C�Y�ύX
//===========================================================
void Text::ChangeFontSize(int width, int height) {
	size_height = height;
	size_width = width;
	//�t�H���g���������A���Ȃ��ƃ��������[�N����
	if (font != NULL) {
		font->Release();
		font = NULL;
	}
	
	//�t�H���g�ݒ�(�X�V)
	D3DXCreateFont(
		GetDevice(),			// Direct3D�f�o�C�X
		size_height,			// ����
		size_width,				// ��
		FW_REGULAR,				// �t�H���g�̑��� ����
		NULL,					// ����
		FALSE,					// �Α�
		SHIFTJIS_CHARSET,		// �����Z�b�g
		OUT_DEFAULT_PRECIS,		// �o�͐��x�͕���
		PROOF_QUALITY,			// �����i�����d��
		FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
		TEXT("Consolas"),	// �t�H���g��
		&font);				// ID3DXFont�|�C���^
}

//=========================================
//�e�L�X�g�ύX
//=========================================
void Text::ChangeText(string t) {
	text = t;
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
	mode = TextMode::TM_CENTER;
}

void Text::TextLeft() {
	mode = TextMode::TM_LEFT;
}

void Text::TextRight() {
	mode = TextMode::TM_RIGHT;
}
