#include "FileOperation.h"

DIRECTORY_DATA* FileOperation::GetDirectoryData(string _path) {
	DIRECTORY_DATA* data;			//�t�@�C�����ƃt�@�C����
	HANDLE hwnd;				//�n���h��
	WIN32_FIND_DATA	find_data;	//�t�@�C�����
	
	data = new DIRECTORY_DATA;		//�f�B���N�g�����𐶐�
	data->folderpath = _path;

	//_path += "*.*";		//���C���h�J�[�h�ݒ�
	string wild = _path + "*.*";
	hwnd = FindFirstFile(wild.c_str(), &find_data);		//�擪�t�@�C����T��
	if (hwnd == INVALID_HANDLE_VALUE) {
		return nullptr;		//���s
	}
	do {
		string str = find_data.cFileName;
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {		//�t�@�C�����f�B���N�g���ł���� 
			//directory�������ꍇ�̏����͂����ɏ���
			if (str != "." && str != "..") {
				data->foldernum++;		//�t�H���_�[���L�^
				data->foldername.resize(data->foldernum);		//�v�f���ύX
				data->foldername[data->foldernum - 1] = str;		//�t�H���_���L�^
			}
		}
		else {
			//file�������ꍇ�̏���
			data->filenum++;		//�t�@�C�������J�E���g
			data->filename.resize(data->filenum);		//�v�f���ύX(�ǉ�)
			data->filename[data->filenum - 1] = str;		//�t�@�C�����L�^
		}
	} while (FindNextFile(hwnd, &find_data));		//���̃t�@�C����T��

	/*
	if (data->foldernum > 0) {		//�t�H���_������Ώ���
		data->data.resize(data->foldernum);		//�t�H���_�����v�f�𑝂₷
		data->folderpath.resize(data->foldernum);		//�t�H���_�����v�f�𑝂₷
		for (int i = 0; i < data->foldernum; i++) {		//�t�H���_�����T��
			data->data[i] = GetDirectoryData(_path + data->foldername[i] + "/");		//�ċA�ɂ�鉺��directory�̎Q��
			data->folderpath[i] = _path + data->foldername[i] + "/";					//�t�H���_�p�X�̋L�^
		}
	}
	*/
	if (data->foldernum > 0) {			//�t�H���_������Ώ���
		data->data.resize(data->foldernum);
		for (int i = 0; i < data->foldernum; i++) {
			data->data[i] = GetDirectoryData(_path + data->foldername[i] + "/");
		}

	}

	FindClose(hwnd);		//�n���h���N���[�Y
	return data;		//�쐬�����f�[�^��n��
}

int FileOperation::GetFolderSum(DIRECTORY_DATA* _data) {
	return GetFolderSum(_data, F_FIND_MODE::FM_NONE);
}

int FileOperation::GetFolderSum(DIRECTORY_DATA* _data, int _findmode) {
	int cnt = 0;
	
	//Data���Ȃ�
	if (_data == nullptr) {
		return 0;		
	}
	//�t�H���_�T��
	for (int i = 0; i < _data->foldernum; i++) {
		cnt += GetFolderSum(_data->data[i], F_FIND_MODE::FM_FILE_EXISTANCE);
	}

	switch (_findmode) {
	case F_FIND_MODE::FM_FILE_EXISTANCE:
		//�t�@�C�������݂��邩�ǂ���
		if (_data->filenum > 0) {
			cnt++;	//���݂���Α��₷
		}
		break;
	case F_FIND_MODE::FM_FILE_NONE:
		//�t�@�C�������݂��Ȃ����ǂ���
		if (_data->filenum <= 0) {
			cnt++;	//�Ȃ���΃J�E���g
		}
		break;
	case F_FIND_MODE::FM_NONE:
		//���ł���������t�H���_���J�E���g
	default:
		//�����ɖ�肪���邪�Ƃ肠�����t�H���_�𖳏�����
		cnt++;
		break;
	}

	return cnt;
}

int FileOperation::GetFileSum(DIRECTORY_DATA* _data, string _findmode) {	//�����t���f�B���N�g���̃t�H���_��
	int cnt = 0;

	if (_data == NULL) {
		return 0;		//���݂��Ă��Ȃ�
	}
	cnt = _data->filenum;
	for (int i = 0; i < _data->foldernum; i++) {
		cnt += GetFileSum(_data->data[i], ".wav");
	}
	
	return cnt;		//���݂̃t�H���_����Ԃ�
}

vector<string> FileOperation::GetFolderPath(DIRECTORY_DATA* _data, int _findmode) {
	vector<string> path;
	vector<string> get;
	DIRECTORY_DATA* current_data = _data;

	if (_data == NULL) {
		return path;
	}

	//path.resize(GetFolderSum(_data, F_FIND_MODE::FM_FILE_EXISTANCE));		//�t�@�C���̑��݂���t�H���_���v�f�m��

	//�t�@�C�������݂���Ώ���
	if (current_data->filenum > 0) {
		path.resize(1);		//�v�f���P�ɂ���
		path[0] = current_data->folderpath;
		//cnt++;
	}
	/*
	for (int i = 0, cnt = 0; i < current_data->foldernum && cnt < path.size(); i++) {

	}
	*/
	//�t�H���_�[�����݂���΃��[�v
	/*
	for (int i = 0; i < path.size(); i++) {
		//�t�@�C�������݂���Ώ���
		if (current_data->filenum > 0) {
			path[i] = current_data->folderpath[i];
		}
	}*/

	//��񂪂����
	for (int j = 0,k = 0; j < current_data->data.size(); j++) {
		get = GetFolderPath(current_data->data[j], F_FIND_MODE::FM_FILE_EXISTANCE);		//���ʑw�̕��f�[�^�擾
		//�擾�ł�����񂪂����
		if (get.size() > 0) {
			//�擾��������A��
			path.insert(path.begin(), get.begin(), get.end());
		}
	}

	return path;		//�t�H���_�p�X��n��
}

vector<string> FileOperation::GetFileName(string filedata) {
	vector<string> filename;
	HANDLE hwnd;
	WIN32_FIND_DATA find_data;

	string wild = filedata + "*.*";
	hwnd = FindFirstFile(wild.c_str(), &find_data);		//�擪�t�@�C����T��
	if (hwnd == INVALID_HANDLE_VALUE) {
		return filename;		//���s
	}

	do {
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {		//�t�@�C�����f�B���N�g���ł���� 
			//directory�������ꍇ�̏����͂����ɏ���
		}
		else {
			//file�������ꍇ�̏���
			//filename.resize(filename.size() + 1);		//�v�f������₷
			filename.insert(filename.end(), find_data.cFileName);		//�����Ƀt�@�C������ǉ�
		}
	} while (FindNextFile(hwnd, &find_data));		//���̃t�@�C����T��

	FindClose(hwnd);		//�n���h���N���[�Y
	return filename;		//�쐬�����f�[�^��n��
}


/*
FOLDER_DATA FileOperation::GetFolderData(string directory) {
	return GetFolderFileData(directory).folderdata;		//�t�H���_�[�A�t�@�C�����擾
}

FILE_DATA FileOperation::GetFileData(string directory) {
	return GetFolderFileData(directory).filedata;
}
*/