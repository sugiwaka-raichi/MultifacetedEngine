#pragma once
#include <string>
#include <vector>
#include "Win.h"

typedef enum {
	FM_FILE_EXISTANCE = 0,		//ファイルがあるかどうか調べる
	FM_FILE_NONE,				//ファイルがないかどうか調べる
	FM_NONE,					//何もないフォルダか調べる
}F_FIND_MODE;

typedef struct DATA{
	int filenum = 0;			//フォルダ内のファイル数
	int foldernum = 0;			//フォルダ内のフォルダ数
	vector<string> filename;	//フォルダ内のファイル名
	vector<string> foldername;	//フォルダ内のフォルダ名
	string folderpath;			//現在フォルダまでのパス
	vector<DATA*>  data;		//下位層フォルダーの情報
}DIRECTORY_DATA;

static class FileOperation {
private:
	//DIRECTORY_DATA* FindDirectory(string _path);
public:
	static DIRECTORY_DATA* GetDirectoryData(string _path);	//ディレクトリ情報を取得する関数
	static int GetFolderSum(DIRECTORY_DATA* _data);			//ディレクトリ内のフォルダ総数を調べる
	static int GetFolderSum(DIRECTORY_DATA* _data, int _findmode);	//条件付きディレクトリのフォルダ数
	static int GetFileSum(DIRECTORY_DATA* _data, string _findmode);	//条件付きディレクトリのフォルダ数
	static vector<string> GetFolderPath(DIRECTORY_DATA* _data, int _findmode);	//条件付きフォルダパスの探索
	static vector<string> GetFileName(string filepath);
//	static FILE_DATA* GetFileData(string directory);		//ファイル数とファイル名を取得する関数
//	static FOLDER_DATA* GetFolderData(string directory);	//フォルダ数とフォルダ名を取得する関数
};

