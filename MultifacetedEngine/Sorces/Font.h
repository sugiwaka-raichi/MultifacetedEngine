#pragma once
#include "game.h"

//==============================
//�t�H���g�N���X
//==============================
class Font {
private:
	float fontSizeW, fontSizeH;		//�t�H���g�̑傫��
	int lineSize;					//���̑���
	bool underLine;					//����
	bool italic;					//�Α�
	LPCWSTR fontName;				//�g�p����t�H���g�̖��O

	LPD3DXFONT font = nullptr;				//�쐬�����t�H���g�̏��
public:
	Font();

	void ChangeFontSize(float, float);			//�t�H���g�T�C�Y�̕ύX
	void ChangeFontData(LPCWSTR _fontMame);		//�����t�H���g�ύX
	void FCreateFont();
	void Release();

	LPD3DXFONT* GetFont();
};