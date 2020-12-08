#include "TEXT.h"

//===========================================================
//�R���X�g���N�^
//===========================================================
Font::Font() {
	fontSizeW = fontSizeH = 10;		//�t�H���g�T�C�Y10x10
	lineSize = FW_REGULAR;			//���̑��� ����
	underLine = false;				//�����͖���
	italic = false;					//�Α̂ɂ��Ȃ�
	fontName = L"�l�r�@�o�S�V�b�N";	//�l�r�@�o�S�V�b�N���g��
	FCreateFont();
}

//===========================================================
//�t�H���g�T�C�Y�ύX
//===========================================================
void Font::ChangeFontSize(float _w, float _h) {
	fontSizeW = _w;
	fontSizeH = _h;
	FCreateFont();
}

//===========================================================
//�t�H���g�f�[�^��ς���
//===========================================================
void Font::ChangeFontData(LPCWSTR _fontName) {
	fontName = _fontName;
	FCreateFont();
}

//===========================================================
//�t�H���g�f�[�^�쐬
//===========================================================
void Font::FCreateFont() {
	if (font != nullptr) {
		Release();		//�������
	}
	//�t�H���g�ݒ�
	D3DXCreateFont(
		GetDevice(),				// Direct3D�f�o�C�X
		fontSizeH,					// ����
		fontSizeW,					// ��
		lineSize,					// �t�H���g�̑���
		underLine,					// ����
		italic,						// �Α�
		SHIFTJIS_CHARSET,			// �����Z�b�g
		OUT_DEFAULT_PRECIS,			// �o�͐��x�͕���
		PROOF_QUALITY,				// �����i�����d��
		FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
		fontName,					// �t�H���g��
		&font);						// ID3DXFont�|�C���^
}

//==============================================
//�������
//==============================================
void Font::Release() {
	font->Release();
}

//==============================================
//�t�H���g�f�[�^�擾
//==============================================
LPD3DXFONT* Font::GetFont() {
	return &font;
}
