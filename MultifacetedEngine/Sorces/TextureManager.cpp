#include "TextureManager.h"
#include "ERROR.h"

TEXTURE_DATA TextureManager::texture_data;	//テクスチャの情報
string TextureManager::tex_directory;		//テクスチャディレクトリ名

TextureManager::TextureManager() {
	//FileDataInit();
}

//==================================================
//ファイル数を数える
//==================================================
/*
bool TextureManager::FolderFilenum() {
	HANDLE hwnd;				//ハンドル
	WIN32_FIND_DATA filedata;	//file情報構造体
	filenum = 0;		//必ずこの処理を行うときは0にすること(でないと多重(n回とする)呼出があった場合n倍のデータを取ることになる

	hwnd = FindFirstFile("Asset/Texture/*.*", &filedata);		//先頭ファイルを探す
	if (hwnd == INVALID_HANDLE_VALUE) {
		return false;		//失敗
	}
	do {
		if(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){		//ファイルがディレクトリであれば 
			//directoryだった場合の処理はここに書く
		}
		else {
			//fileだった場合の処理
			filenum++;		//ファイル数をカウント
			temp_filename = filedata.cFileName;		//ファイル名を記録
			temp_filename = ',';					//区切り文字を追加
		}
	} while (FindNextFile(hwnd, &filedata));		//次のファイルを探す
	
	FindClose(hwnd);		//ハンドルクローズ
	return true;
}
*/

//--------------------------------------------------------
//初期化処理
//--------------------------------------------------------
bool TextureManager::FileDataInit() {
	tex_directory = DEFAULT_TEXTURE_PATH;		//テクスチャディレクトリ名
	texture_data.filedata = FileOperation::GetDirectoryData(tex_directory);	//ディレクトリ内の情報を取得
	if (texture_data.filedata == nullptr) {
		ErrorMessage::ErrorMessageBox(LPCSTR("ディレクトリの情報取得に失敗しました。"), LPCSTR("Texture Init Error"), MB_OK);
		exit(1);
		return false;
	}
	texture_data.loadflag.resize(texture_data.filedata->filenum);		//フラグの要素数変更
	texture_data.texture.resize(texture_data.filedata->filenum);			//テクスチャの要素数変更
	//ロードフラグをすべてfalseへ
	for (int i = 0; i < texture_data.filedata->filenum; i++) {
		texture_data.loadflag[i] = false;
	}
	return true;
}

//-------------------------------------------------------
//ロード処理
//-------------------------------------------------------
bool TextureManager::TextureLoad(string filename) {
	//Textureフォルダの最後までループ	
	for (int i = 0; i < texture_data.filedata->filenum; i++) {
		//対象のファイルかファイル名で確認
		if (texture_data.filedata->filename[i] == filename) {
			string filepath = tex_directory + TEXT("/") + filename;	//読み込みに使うファイルパスを作成
			//ロード処理
			if (FAILED(D3DXCreateTextureFromFile(GetDevice(), filepath.c_str(), &texture_data.texture[i]))) {
				//失敗した場合エラーメッセージを出す
				string mes = TEXT("画像データ:") + texture_data.filedata->filename[i] + TEXT("が、\n正しく読み込まれませんでした。");
				ErrorMessage::ErrorMessageBox((LPCSTR)mes.c_str(), (LPCSTR)"TextureLoadError", MB_OK);
				texture_data.texture[i] = nullptr;
			}
			texture_data.loadflag[i] = true;		//ロード
			return true;	//ロード成功
		}
	}
	//フォルダ内になければエラーメッセージを表示
	string str = TEXT("画像データ:") + filename + TEXT("が、\n見つかりませんでした。");
	ErrorMessage::ErrorMessageBox((LPCSTR)str.c_str(), (LPCSTR)"TextureLoadError", MB_OK);
	return false;
}

//---------------------------------------------------------------
//テクスチャ開放
//---------------------------------------------------------------
void TextureManager::DeleteTexture(string filename) {
	for (int i = 0; i < texture_data.filedata->filenum; i++) {
		if (texture_data.filedata->filename[i] == filename) {
			delete texture_data.texture[i];
			texture_data.texture[i] = nullptr;		//テクスチャ開放
			texture_data.loadflag[i] = false;		//非ロード
		}
	}
}

//---------------------------------------------------------------------
//指定されたファイル名のテクスチャを渡す
//---------------------------------------------------------------------
LPDIRECT3DTEXTURE9 TextureManager::GetTexture(string texturename) {
	for (int i = 0; i < texture_data.filedata->filenum; i++) {
		//対象のファイル名か判断し、かつロードされているか
		if (texturename == texture_data.filedata->filename[i]) {
			if (texture_data.loadflag[i]) {
				return texture_data.texture[i];
			}
			break;
		}
	}
	//ロードされていなければロードを行う ※エラー無限ループ
	//TextureLoad(texturename);

	return nullptr;		//一致するテクスチャがなかったらnullを返す
}

//---------------------------------------------------------
//読込先のディレクトリの変更
//---------------------------------------------------------
void TextureManager::SetDirectory(string directory) {
	tex_directory = directory;
}


/*
こっちはいまいちよくわからなかったので上記のサンプルを見ておおむね理解した
std::size_t file_count_native(const std::basic_string<TCHAR>&src) {
	std::basic_string<TCHAR> root = src + _T("\\*.*");
	WIN32_FIND_DATA wfd = {};
	HANDLE handle = FindFirstFile(root.c_str(), &wfd);
	if (handle == INVALID_HANDLE_VALUE) return 0;

	std::size_t result = 0;
	do {
		std::basic_string<TCHAR> path(wfd.cFileName);
		if (path != _T(".") && path != _T("..")) {
			++result;
			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				result += file_count_native(src + _T('\\') + path);
			}
		}
	} while (FindNextFile(handle, &wfd));
	FindClose(handle);

	return result;
}
*/

/*
bool TextureManager::TextureLoad(string texturename) {
	TextureData *new_tex = nullptr;
	new_tex->tag = texturename;
	if (FAILED(D3DXCreateTextureFromFile(GetDevice(), texturename.c_str(), &new_tex->texture))) {
		ErrorMessage error;
		ErrorMessageBox("LoadError","TextureLoadError",MB_OK);
	}
	texturelist.AddList(new_tex);		//テクスチャリストに追加

	return true;
}
*/