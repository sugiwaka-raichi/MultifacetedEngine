#pragma once
#include "game.h"
#include "LIST.h"
#include "FileOperation.h"

#define DEFAULT_TEXTURE_PATH 	TEXT("./Asset/Texture/")

typedef struct {
	DIRECTORY_DATA*				filedata;	//ファイル名とファイル数
	vector<bool>				loadflag;	//ロード状態のフラグ
	vector<LPDIRECT3DTEXTURE9>	texture;	//テクスチャ

}TEXTURE_DATA;

static class TextureManager
{
private:
	static TEXTURE_DATA texture_data;	//テクスチャの情報
	static wstring tex_directory;		//テクスチャディレクトリ名

public:
	TextureManager();
	static bool FileDataInit();		//ファイルデータ初期化
	//bool TextureFilePath();		//テクスチャファイルのファイルパス取得(後にスクリプトファイルの情報から行うため不要?)
	static bool TextureLoad(wstring texturename);		//テクスチャ読込
	static void DeleteTexture(wstring texturename);		//テクスチャ開放
	static LPDIRECT3DTEXTURE9* GetTexture(wstring texturename);	//テクスチャゲッター
	static void SetDirectory(wstring directory);		//ディレクトリ設定
};
