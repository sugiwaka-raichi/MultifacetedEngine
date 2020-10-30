#include "FileOperation.h"

DIRECTORY_DATA* FileOperation::GetDirectoryData(string _path) {
	DIRECTORY_DATA* data;			//ファイル数とファイル名
	HANDLE hwnd;				//ハンドル
	WIN32_FIND_DATA	find_data;	//ファイル情報
	
	data = new DIRECTORY_DATA;		//ディレクトリ情報を生成
	data->folderpath = _path;

	//_path += "*.*";		//ワイルドカード設定
	string wild = _path + "*.*";
	hwnd = FindFirstFile(wild.c_str(), &find_data);		//先頭ファイルを探す
	if (hwnd == INVALID_HANDLE_VALUE) {
		return nullptr;		//失敗
	}
	do {
		string str = find_data.cFileName;
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {		//ファイルがディレクトリであれば 
			//directoryだった場合の処理はここに書く
			if (str != "." && str != "..") {
				data->foldernum++;		//フォルダー数記録
				data->foldername.resize(data->foldernum);		//要素数変更
				data->foldername[data->foldernum - 1] = str;		//フォルダ名記録
			}
		}
		else {
			//fileだった場合の処理
			data->filenum++;		//ファイル数をカウント
			data->filename.resize(data->filenum);		//要素数変更(追加)
			data->filename[data->filenum - 1] = str;		//ファイル名記録
		}
	} while (FindNextFile(hwnd, &find_data));		//次のファイルを探す

	/*
	if (data->foldernum > 0) {		//フォルダがあれば処理
		data->data.resize(data->foldernum);		//フォルダ数分要素を増やす
		data->folderpath.resize(data->foldernum);		//フォルダ数分要素を増やす
		for (int i = 0; i < data->foldernum; i++) {		//フォルダ数分探索
			data->data[i] = GetDirectoryData(_path + data->foldername[i] + "/");		//再帰による下位directoryの参照
			data->folderpath[i] = _path + data->foldername[i] + "/";					//フォルダパスの記録
		}
	}
	*/
	if (data->foldernum > 0) {			//フォルダがあれば処理
		data->data.resize(data->foldernum);
		for (int i = 0; i < data->foldernum; i++) {
			data->data[i] = GetDirectoryData(_path + data->foldername[i] + "/");
		}

	}

	FindClose(hwnd);		//ハンドルクローズ
	return data;		//作成したデータを渡す
}

int FileOperation::GetFolderSum(DIRECTORY_DATA* _data) {
	return GetFolderSum(_data, F_FIND_MODE::FM_NONE);
}

int FileOperation::GetFolderSum(DIRECTORY_DATA* _data, int _findmode) {
	int cnt = 0;
	
	//Dataがない
	if (_data == nullptr) {
		return 0;		
	}
	//フォルダ探索
	for (int i = 0; i < _data->foldernum; i++) {
		cnt += GetFolderSum(_data->data[i], F_FIND_MODE::FM_FILE_EXISTANCE);
	}

	switch (_findmode) {
	case F_FIND_MODE::FM_FILE_EXISTANCE:
		//ファイルが存在するかどうか
		if (_data->filenum > 0) {
			cnt++;	//存在すれば増やす
		}
		break;
	case F_FIND_MODE::FM_FILE_NONE:
		//ファイルが存在しないかどうか
		if (_data->filenum <= 0) {
			cnt++;	//なければカウント
		}
		break;
	case F_FIND_MODE::FM_NONE:
		//何でもいいからフォルダ数カウント
	default:
		//条件に問題があるがとりあえずフォルダを無条件で
		cnt++;
		break;
	}

	return cnt;
}

int FileOperation::GetFileSum(DIRECTORY_DATA* _data, string _findmode) {	//条件付きディレクトリのフォルダ数
	int cnt = 0;

	if (_data == NULL) {
		return 0;		//存在していない
	}
	cnt = _data->filenum;
	for (int i = 0; i < _data->foldernum; i++) {
		cnt += GetFileSum(_data->data[i], ".wav");
	}
	
	return cnt;		//現在のフォルダ数を返す
}

vector<string> FileOperation::GetFolderPath(DIRECTORY_DATA* _data, int _findmode) {
	vector<string> path;
	vector<string> get;
	DIRECTORY_DATA* current_data = _data;

	if (_data == NULL) {
		return path;
	}

	//path.resize(GetFolderSum(_data, F_FIND_MODE::FM_FILE_EXISTANCE));		//ファイルの存在するフォルダ分要素確保

	//ファイルが存在すれば処理
	if (current_data->filenum > 0) {
		path.resize(1);		//要素数１にする
		path[0] = current_data->folderpath;
		//cnt++;
	}
	/*
	for (int i = 0, cnt = 0; i < current_data->foldernum && cnt < path.size(); i++) {

	}
	*/
	//フォルダーが存在すればループ
	/*
	for (int i = 0; i < path.size(); i++) {
		//ファイルが存在すれば処理
		if (current_data->filenum > 0) {
			path[i] = current_data->folderpath[i];
		}
	}*/

	//情報があれば
	for (int j = 0,k = 0; j < current_data->data.size(); j++) {
		get = GetFolderPath(current_data->data[j], F_FIND_MODE::FM_FILE_EXISTANCE);		//下位層の分データ取得
		//取得できた情報があれば
		if (get.size() > 0) {
			//取得した情報を連結
			path.insert(path.begin(), get.begin(), get.end());
		}
	}

	return path;		//フォルダパスを渡す
}

vector<string> FileOperation::GetFileName(string filedata) {
	vector<string> filename;
	HANDLE hwnd;
	WIN32_FIND_DATA find_data;

	string wild = filedata + "*.*";
	hwnd = FindFirstFile(wild.c_str(), &find_data);		//先頭ファイルを探す
	if (hwnd == INVALID_HANDLE_VALUE) {
		return filename;		//失敗
	}

	do {
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {		//ファイルがディレクトリであれば 
			//directoryだった場合の処理はここに書く
		}
		else {
			//fileだった場合の処理
			//filename.resize(filename.size() + 1);		//要素を一つ増やす
			filename.insert(filename.end(), find_data.cFileName);		//末尾にファイル名を追加
		}
	} while (FindNextFile(hwnd, &find_data));		//次のファイルを探す

	FindClose(hwnd);		//ハンドルクローズ
	return filename;		//作成したデータを渡す
}


/*
FOLDER_DATA FileOperation::GetFolderData(string directory) {
	return GetFolderFileData(directory).folderdata;		//フォルダー、ファイル情報取得
}

FILE_DATA FileOperation::GetFileData(string directory) {
	return GetFolderFileData(directory).filedata;
}
*/