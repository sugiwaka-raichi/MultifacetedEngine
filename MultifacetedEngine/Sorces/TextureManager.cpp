#include "TextureManager.h"
#include "ERROR.h"

TEXTURE_DATA TextureManager::texture_data;	//�e�N�X�`���̏��
string TextureManager::tex_directory;		//�e�N�X�`���f�B���N�g����

TextureManager::TextureManager() {
	//FileDataInit();
}

//==================================================
//�t�@�C�����𐔂���
//==================================================
/*
bool TextureManager::FolderFilenum() {
	HANDLE hwnd;				//�n���h��
	WIN32_FIND_DATA filedata;	//file���\����
	filenum = 0;		//�K�����̏������s���Ƃ���0�ɂ��邱��(�łȂ��Ƒ��d(n��Ƃ���)�ďo���������ꍇn�{�̃f�[�^����邱�ƂɂȂ�

	hwnd = FindFirstFile("Asset/Texture/*.*", &filedata);		//�擪�t�@�C����T��
	if (hwnd == INVALID_HANDLE_VALUE) {
		return false;		//���s
	}
	do {
		if(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){		//�t�@�C�����f�B���N�g���ł���� 
			//directory�������ꍇ�̏����͂����ɏ���
		}
		else {
			//file�������ꍇ�̏���
			filenum++;		//�t�@�C�������J�E���g
			temp_filename = filedata.cFileName;		//�t�@�C�������L�^
			temp_filename = ',';					//��؂蕶����ǉ�
		}
	} while (FindNextFile(hwnd, &filedata));		//���̃t�@�C����T��
	
	FindClose(hwnd);		//�n���h���N���[�Y
	return true;
}
*/

//--------------------------------------------------------
//����������
//--------------------------------------------------------
bool TextureManager::FileDataInit() {
	tex_directory = DEFAULT_TEXTURE_PATH;		//�e�N�X�`���f�B���N�g����
	texture_data.filedata = FileOperation::GetDirectoryData(tex_directory);	//�f�B���N�g�����̏����擾
	if (texture_data.filedata == nullptr) {
		ErrorMessage::ErrorMessageBox(LPCSTR("�f�B���N�g���̏��擾�Ɏ��s���܂����B"), LPCSTR("Texture Init Error"), MB_OK);
		exit(1);
		return false;
	}
	texture_data.loadflag.resize(texture_data.filedata->filenum);		//�t���O�̗v�f���ύX
	texture_data.texture.resize(texture_data.filedata->filenum);			//�e�N�X�`���̗v�f���ύX
	//���[�h�t���O�����ׂ�false��
	for (int i = 0; i < texture_data.filedata->filenum; i++) {
		texture_data.loadflag[i] = false;
	}
	return true;
}

//-------------------------------------------------------
//���[�h����
//-------------------------------------------------------
bool TextureManager::TextureLoad(string filename) {
	//Texture�t�H���_�̍Ō�܂Ń��[�v	
	for (int i = 0; i < texture_data.filedata->filenum; i++) {
		//�Ώۂ̃t�@�C�����t�@�C�����Ŋm�F
		if (texture_data.filedata->filename[i] == filename) {
			string filepath = tex_directory + TEXT("/") + filename;	//�ǂݍ��݂Ɏg���t�@�C���p�X���쐬
			//���[�h����
			if (FAILED(D3DXCreateTextureFromFile(GetDevice(), filepath.c_str(), &texture_data.texture[i]))) {
				//���s�����ꍇ�G���[���b�Z�[�W���o��
				string mes = TEXT("�摜�f�[�^:") + texture_data.filedata->filename[i] + TEXT("���A\n�������ǂݍ��܂�܂���ł����B");
				ErrorMessage::ErrorMessageBox((LPCSTR)mes.c_str(), (LPCSTR)"TextureLoadError", MB_OK);
				texture_data.texture[i] = nullptr;
			}
			texture_data.loadflag[i] = true;		//���[�h
			return true;	//���[�h����
		}
	}
	//�t�H���_���ɂȂ���΃G���[���b�Z�[�W��\��
	string str = TEXT("�摜�f�[�^:") + filename + TEXT("���A\n������܂���ł����B");
	ErrorMessage::ErrorMessageBox((LPCSTR)str.c_str(), (LPCSTR)"TextureLoadError", MB_OK);
	return false;
}

//---------------------------------------------------------------
//�e�N�X�`���J��
//---------------------------------------------------------------
void TextureManager::DeleteTexture(string filename) {
	for (int i = 0; i < texture_data.filedata->filenum; i++) {
		if (texture_data.filedata->filename[i] == filename) {
			delete texture_data.texture[i];
			texture_data.texture[i] = nullptr;		//�e�N�X�`���J��
			texture_data.loadflag[i] = false;		//�񃍁[�h
		}
	}
}

//---------------------------------------------------------------------
//�w�肳�ꂽ�t�@�C�����̃e�N�X�`����n��
//---------------------------------------------------------------------
LPDIRECT3DTEXTURE9 TextureManager::GetTexture(string texturename) {
	for (int i = 0; i < texture_data.filedata->filenum; i++) {
		//�Ώۂ̃t�@�C���������f���A�����[�h����Ă��邩
		if (texturename == texture_data.filedata->filename[i]) {
			if (texture_data.loadflag[i]) {
				return texture_data.texture[i];
			}
			break;
		}
	}
	//���[�h����Ă��Ȃ���΃��[�h���s�� ���G���[�������[�v
	//TextureLoad(texturename);

	return nullptr;		//��v����e�N�X�`�����Ȃ�������null��Ԃ�
}

//---------------------------------------------------------
//�Ǎ���̃f�B���N�g���̕ύX
//---------------------------------------------------------
void TextureManager::SetDirectory(string directory) {
	tex_directory = directory;
}


/*
�������͂��܂����悭�킩��Ȃ������̂ŏ�L�̃T���v�������Ă����ނ˗�������
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
	texturelist.AddList(new_tex);		//�e�N�X�`�����X�g�ɒǉ�

	return true;
}
*/