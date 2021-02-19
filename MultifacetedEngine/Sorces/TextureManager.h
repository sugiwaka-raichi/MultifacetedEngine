#pragma once
#include "game.h"
#include "LIST.h"
#include "FileOperation.h"

#define DEFAULT_TEXTURE_PATH 	TEXT("./Asset/Texture/")

typedef struct {
	DIRECTORY_DATA*				filedata;	//�t�@�C�����ƃt�@�C����
	vector<bool>				loadflag;	//���[�h��Ԃ̃t���O
	vector<LPDIRECT3DTEXTURE9>	texture;	//�e�N�X�`��

}TEXTURE_DATA;

static class TextureManager
{
private:
	static TEXTURE_DATA texture_data;	//�e�N�X�`���̏��
	static wstring tex_directory;		//�e�N�X�`���f�B���N�g����

public:
	TextureManager();
	static bool FileDataInit();		//�t�@�C���f�[�^������
	//bool TextureFilePath();		//�e�N�X�`���t�@�C���̃t�@�C���p�X�擾(��ɃX�N���v�g�t�@�C���̏�񂩂�s�����ߕs�v?)
	static bool TextureLoad(wstring texturename);		//�e�N�X�`���Ǎ�
	static void DeleteTexture(wstring texturename);		//�e�N�X�`���J��
	static LPDIRECT3DTEXTURE9* GetTexture(wstring texturename);	//�e�N�X�`���Q�b�^�[
	static void SetDirectory(wstring directory);		//�f�B���N�g���ݒ�
};
