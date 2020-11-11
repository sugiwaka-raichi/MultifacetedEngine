#pragma once
#include <string>
#include <vector>
#include "Win.h"

typedef enum {
	FM_FILE_EXISTANCE = 0,		//�t�@�C�������邩�ǂ������ׂ�
	FM_FILE_NONE,				//�t�@�C�����Ȃ����ǂ������ׂ�
	FM_NONE,					//�����Ȃ��t�H���_�����ׂ�
}F_FIND_MODE;

typedef struct DATA{
	int filenum = 0;			//�t�H���_���̃t�@�C����
	int foldernum = 0;			//�t�H���_���̃t�H���_��
	vector<string> filename;	//�t�H���_���̃t�@�C����
	vector<string> foldername;	//�t�H���_���̃t�H���_��
	string folderpath;			//���݃t�H���_�܂ł̃p�X
	vector<DATA*>  data;		//���ʑw�t�H���_�[�̏��
}DIRECTORY_DATA;

static class FileOperation {
private:
	//DIRECTORY_DATA* FindDirectory(string _path);
public:
	static DIRECTORY_DATA* GetDirectoryData(string _path);	//�f�B���N�g�������擾����֐�
	static int GetFolderSum(DIRECTORY_DATA* _data);			//�f�B���N�g�����̃t�H���_�����𒲂ׂ�
	static int GetFolderSum(DIRECTORY_DATA* _data, int _findmode);	//�����t���f�B���N�g���̃t�H���_��
	static int GetFileSum(DIRECTORY_DATA* _data, string _findmode);	//�����t���f�B���N�g���̃t�H���_��
	static vector<string> GetFolderPath(DIRECTORY_DATA* _data, int _findmode);	//�����t���t�H���_�p�X�̒T��
	static vector<string> GetFileName(string filepath);
//	static FILE_DATA* GetFileData(string directory);		//�t�@�C�����ƃt�@�C�������擾����֐�
//	static FOLDER_DATA* GetFolderData(string directory);	//�t�H���_���ƃt�H���_�����擾����֐�
};

