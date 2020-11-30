//=============================================================================
//
// �T�E���h���� [XAudio2.cpp]
//
//=============================================================================

#include "XAudio2.h"
#include "ERROR.h"
#include "TextureManager.h"
#include "StringOperation.h"

// �p�����[�^�\����
typedef struct {
	int				filenum;	//�t�@�C����
	vector<string>	filename;	//�t�@�C����
	vector<bool>	bLoop;		// �f�t�H���g��false�B
	vector<bool>	loadflag;	//���[�h�t���O
	vector<IXAudio2SourceVoice*>    g_pSourceVoice;
	vector<WAVEFORMATEXTENSIBLE>	wfx;			// WAV�t�H�[�}�b�g
	vector<XAUDIO2_BUFFER>			buffer;
	vector<BYTE*>					pDataBuffer;
} SOUND_FILE_DATA;

typedef struct {
	int				foldernum;			//�t�H���_��
	vector<string>	type;				//�t�@�C���̎��(SE,BGM�Ȃ�)
	vector<string>  folderpath;			//�t�H���_�̃p�X
	vector<SOUND_FILE_DATA>	filedata;	//�t�@�C�����
}SOUND_FOLDER_DATA;

/*
typedef struct {
	vector<SOUND_FILE_DATA>	sound_file_data;		//�T�E���h�t�@�C���f�[�^
} SOUND_FILE_INFO;
SOUND_FILE_INFO soundfileinfo;		//�T�E���h�t�@�C�����
*/

/*
PARAM g_param[SOUND_LABEL_MAX] =
{
	{"Asset/Sound/BGM/play.wav", true},	// �T���v��BGM�i���[�v������̂�true�ݒ�j
	{"Asset/Sound/BGM/room.wav", true},	// �T���v��BGM
};
*/

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif
#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

//�`�����N�f�[�^�̊�{�\��
struct Chunk {
	char id[4];		//�`�����N���Ƃ�ID
	int32_t size;	//�`�����N�T�C�Y
};

//RIFF�w�b�_�[
struct RiffHeader {
	Chunk	chunk;		//"RIFF"
	char	type[4];	//"WAVE"
};

//FMT�`�����N
struct FormatChunk {
	Chunk		chunk;		//"fmt"
	WAVEFORMAT	fmt;		//�g�`�t�H�[�}�b�g
};

//-----------------------------------------------------------------
//    �O���[�o���ϐ�
//-----------------------------------------------------------------
IXAudio2               *g_pXAudio2        = NULL;
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;

char* pbuffer;
string DefaultDirectory = TEXT("./Asset/Sound/") ;		//�f�t�H���g�f�B���N�g���p�X
SOUND_FOLDER_DATA sound_fodata;						//�T�E���h�t�@�C�����
bool sound_order_state = false;						//�T�E���h���ߋ����

//-----------------------------------------------------------------
//    �v���g�^�C�v�錾
//-----------------------------------------------------------------
void Normalization(DIRECTORY_DATA* _data);
int FindFolderNum(string path);
int FindFileNum(int foldernum,string filename);

HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
HRESULT ReadChunkData( HANDLE , void* , DWORD , DWORD );


//=============================================================================
// ������
//=============================================================================
HRESULT InitSound() {
	HRESULT      hr;
	Normalization(FileOperation::GetDirectoryData(DefaultDirectory));	//�T�E���h�t�H���_���擾

	//Com�R���|�[�l���g������
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		ErrorMessage::ErrorMessageBox(TEXT("Com�R���|�[�l���g�̏������Ɏ��s���܂����B"), TEXT("XAudio2 InitError"), MB_OK);
		exit(1);
	}

	//XAudio2�̏�����
	hr = XAudio2Create(&g_pXAudio2);		//XAudio2�̃C���X�^���X��Ԃ�
	if (FAILED(hr)) {
		ErrorMessage::ErrorMessageBox(TEXT("XAudio2�̍쐬�Ɏ��s���܂����B"), TEXT("XAudio2 InitError"), MB_OK);
		exit(1);
	}

	//�}�X�^�[�{�C�X�͍Đ������{�C�X���ׂĂ̍ŏI���B�n�_
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);		//�}�X�^�[�{�C�X�쐬
	if (FAILED(hr)) {
		ErrorMessage::ErrorMessageBox(TEXT("�}�X�^�[�{�C�X�̍쐬�Ɏ��s���܂����B"), TEXT("XAudio2 InitError"), MB_OK);
		exit(1);
	}

	//�\�[�X�{�C�X�쐬 wfex�@�t�H�[�}�b�g
	/*
	hr = g_pXAudio2->CreateSourceVoice(&g_pSourceVoice, &wfex);
	if (FAILED(hr)) {
		ErrorMessage::ErrorMessageBox("�\�[�X�{�C�X�̍쐬�Ɏ��s���܂����B", "XAudio2 Init Error", MB_OK);
	}
	*/

																/*
	HANDLE               hFile;
	DWORD                dwChunkSize;
	DWORD                dwChunkPosition;
	DWORD                filetype;
		
	CoInitializeEx( NULL , COINIT_MULTITHREADED );
	
	//Create XAudio2 
	hr = XAudio2Create( &g_pXAudio2, 0/* , XAUDIO2_DEFAULT_PROCESSOR);
	//hr=XAudio2Create(&g_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if( FAILED( hr ) ){
		CoUninitialize();
		return -1;
	}
	
	/**** Create Mastering Voice 
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice/*, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL);
	if( FAILED( hr ) ){
		if( g_pXAudio2 )	g_pXAudio2->Release();
		CoUninitialize();
		return -1;
	}

	/**** Initalize Sound 
	for(int i=0; i<SOUND_LABEL_MAX; i++)
	{
		memset( &wfx[i] , 0 , sizeof( WAVEFORMATEXTENSIBLE ) );
		memset( &buffer[i] , 0 , sizeof( XAUDIO2_BUFFER ) );
	
		hFile = CreateFile( g_param[i].filename , GENERIC_READ , FILE_SHARE_READ , NULL ,
							OPEN_EXISTING , 0 , NULL );
		if ( hFile == INVALID_HANDLE_VALUE ){
			return HRESULT_FROM_WIN32( GetLastError() );
		}
		if ( SetFilePointer( hFile , 0 , NULL , FILE_BEGIN ) == INVALID_SET_FILE_POINTER ){
			return HRESULT_FROM_WIN32( GetLastError() );
		}

		//check the file type, should be fourccWAVE or 'XWMA'
		FindChunk(hFile,fourccRIFF,dwChunkSize, dwChunkPosition );
		ReadChunkData(hFile,&filetype,sizeof(DWORD),dwChunkPosition);
		if (filetype != fourccWAVE)		return S_FALSE;

		FindChunk(hFile,fourccFMT, dwChunkSize, dwChunkPosition );
		ReadChunkData(hFile, &wfx[i], dwChunkSize, dwChunkPosition );

		//fill out the audio data buffer with the contents of the fourccDATA chunk
		FindChunk(hFile,fourccDATA,dwChunkSize, dwChunkPosition );
		pDataBuffer[i] = new BYTE[dwChunkSize];
		ReadChunkData(hFile, pDataBuffer[i], dwChunkSize, dwChunkPosition);
		
		CloseHandle(hFile);

		// 	�T�u�~�b�g�{�C�X�ŗ��p����T�u�~�b�g�o�b�t�@�̐ݒ�
		buffer[i].AudioBytes = dwChunkSize;
		buffer[i].pAudioData = pDataBuffer[i];
		buffer[i].Flags = XAUDIO2_END_OF_STREAM;
		if(g_param[i].bLoop)
			buffer[i].LoopCount = XAUDIO2_LOOP_INFINITE;
		else
			buffer[i].LoopCount = 0;

		g_pXAudio2->CreateSourceVoice( &g_pSourceVoice[i] , &(wfx[i].Format) );
	}
	*/
	return hr;
}

//=============================================================================
// �J������
//=============================================================================
void UninitSound(void)
{
	/*
	for(int i=0; i<SOUND_LABEL_MAX; i++)
	{
		if(g_pSourceVoice[i])
		{
			g_pSourceVoice[i]->Stop( 0 );
			g_pSourceVoice[i]->FlushSourceBuffers();
			g_pSourceVoice[i]->DestroyVoice();			// �I�[�f�B�I�O���t����\�[�X�{�C�X���폜
			delete[]  pDataBuffer[i];
		}
	}

	g_pMasteringVoice->DestroyVoice();
	
	if ( g_pXAudio2 ) g_pXAudio2->Release();
	*/
	
	if (g_pMasteringVoice) {
		g_pMasteringVoice->DestroyVoice();
		g_pMasteringVoice = nullptr;
	}
	if (g_pXAudio2) {
		g_pXAudio2->Release();
		g_pXAudio2 = nullptr;
	}
	CoUninitialize();
}

//=============================================================================
// �Đ�
//=============================================================================
void PlaySound(string path, string filename) {
	if (sound_order_state) {
		int fonum = FindFolderNum(path);
		int finum = FindFileNum(fonum, filename);

		//�\�[�X�{�C�X�����邩�ǂ���
		if (sound_fodata.filedata[fonum].g_pSourceVoice[finum] == NULL) {
			//�Ȃ���΃\�[�X�{�C�X�쐬
			g_pXAudio2->CreateSourceVoice(&sound_fodata.filedata[fonum].g_pSourceVoice[finum], &(sound_fodata.filedata[fonum].wfx[finum].Format));
		}

		sound_fodata.filedata[fonum].g_pSourceVoice[finum]->SubmitSourceBuffer(&(sound_fodata.filedata[fonum].buffer[finum]));	// �{�C�X�L���[�ɐV�����I�[�f�B�I�o�b�t�@�[��ǉ�
		// �Đ�
		sound_fodata.filedata[fonum].g_pSourceVoice[finum]->Start(0);
	}
	else {
		string str = TEXT("filename:") + filename + TEXT("�͓ǂݍ��܂�Ă��܂���B\n�I������ꍇ��OK");
		if (IDOK == ErrorMessage::ErrorMessageBox(str.c_str(), TEXT("XAudio2 Play Error"), MB_OKCANCEL)) {
			exit(1);
		}
	}
}

//=============================================================================
// ��~
//=============================================================================
void StopSound(string path, string filename) {
	if (sound_order_state) {
		int fonum = FindFolderNum(path);
		int finum = FindFileNum(fonum, filename);
		//�\�[�X�{�C�X���쐬����Ă��Ȃ���Ή������Ȃ�
		if (sound_fodata.filedata[fonum].g_pSourceVoice[finum] == NULL) {
			return;
		}

		XAUDIO2_VOICE_STATE	xa2state;

		sound_fodata.filedata[fonum].g_pSourceVoice[finum]->GetState(&xa2state);
		if (xa2state.BuffersQueued) {
			// ��~
			sound_fodata.filedata[fonum].g_pSourceVoice[finum]->Stop(0);
			sound_fodata.filedata[fonum].g_pSourceVoice[finum] = NULL;		//�\�[�X�{�C�X�����Z�b�g
		}
	}
	else {
		string str = TEXT("filename:") + filename + TEXT("�͓ǂݍ��܂�Ă��܂���B");
		ErrorMessage::ErrorMessageBox(str.c_str(), TEXT("XAudio2 Stop Error"), MB_OK);
	}
	/*
	//int label = FindFileNum(filename);
	if(g_pSourceVoice[label] == NULL) return;

	XAUDIO2_VOICE_STATE xa2state;
	g_pSourceVoice[label]->GetState( &xa2state );
	if(xa2state.BuffersQueued)
	{
		g_pSourceVoice[label]->Stop( 0 );
	}
	*/
}

//=============================================================================
// �ꎞ��~
//=============================================================================
void PauseSound(string path, string filename) {
	if (sound_order_state) {
		int fonum = FindFolderNum(path);
		int finum = FindFileNum(fonum, filename);
		//�\�[�X�{�C�X���쐬����Ă��Ȃ���Ή������Ȃ�
		if (sound_fodata.filedata[fonum].g_pSourceVoice[finum] == NULL) {
			return;
		}

		XAUDIO2_VOICE_STATE	xa2state;

		sound_fodata.filedata[fonum].g_pSourceVoice[finum]->GetState(&xa2state);
		if (xa2state.BuffersQueued) {
			// ��~
			sound_fodata.filedata[fonum].g_pSourceVoice[finum]->Stop(0);
		}
	}
	else {
		string str = TEXT("filename:") + filename + TEXT("�͓ǂݍ��܂�Ă��܂���B");
		ErrorMessage::ErrorMessageBox(str.c_str(), TEXT("XAudio2 Pause Error"), MB_OK);
	}
}

//=============================================================================
//���[�h����(?)
//=============================================================================
void LoadWave(string path, string filename, bool loop) {
	//if (sound_order_state) {
		FILE* file = nullptr;
		HANDLE hFile;
		DWORD	dwChunkSize;
		DWORD	dwChunkPosition;
		DWORD	filetype;

		int fonum = FindFolderNum(path);
		int finum = FindFileNum(fonum, filename);

		if (finum < 0) {
			string str = TEXT("�t�@�C����:") + filename + TEXT("�̎Q�ƂɎ��s���܂����B\n�t�@�C����������܂���ł����B");
			ErrorMessage::ErrorMessageBox(str.c_str(), TEXT("Xaudio2 Load Error"), MB_OK);
			
			return;
		}
		//Wave�t�@�C�����J��
		string filepath = path + filename;		//�t�@�C���p�X�쐬
		hFile = CreateFile(filepath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			//�G���[���b�Z
			return;
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
			//�G���[���b�Z
			return;
		}
		/*
		if (fopen_s(&file, filepath.c_str(), "rb") != 0) {
			string str = "�t�@�C����:" + filename + "�̓Ǎ��Ɏ��s���܂����B\n�t�@�C����������܂���ł����B";
			ErrorMessage::ErrorMessageBox(str.c_str(), "XAudio2 Load Error", MB_OK);
			return;
		}
		*/
		//check the file type,should be fourccWAVE or 'XWMA'
		FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (filetype != fourccWAVE) {
			return;		//�G���[���b�Z
		}

		FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &sound_fodata.filedata[fonum].wfx[finum], dwChunkSize, dwChunkPosition);

		//fill out the audio data buffer with the contents of the fourccDATA chunk
		FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		sound_fodata.filedata[fonum].pDataBuffer[finum] = new BYTE[dwChunkSize];
		ReadChunkData(hFile, sound_fodata.filedata[fonum].pDataBuffer[finum], dwChunkSize, dwChunkPosition);

		CloseHandle(hFile);

		sound_fodata.filedata[fonum].bLoop[finum] = loop;		//���[�v�t���O������
		/*
		//�Ⴄ�Ǎ��̎d��
		//RIFF�w�b�_�O�̓Ǎ�
		RiffHeader riff;
		fread(&riff, sizeof(riff), 1, file);

		//Format�`�����N�̓Ǎ�
		FormatChunk format;
		fread(&format, sizeof(format), 1, file);

		//Data�`�����N�̓Ǎ�
		Chunk data;
		fread(&data, sizeof(data), 1, file);

		//Data�`�����N�̃f�[�^���@�i�g�`�f�[�^�j�̓Ǎ�
		pbuffer = (char*)malloc(data.size);
		fread(pbuffer, data.size, 1, file);

		fclose(file);

		//WAVEFORMATEX wfex{	};
		//�g�`�t�H�[�}�b�g�̐ݒ�
		memcpy(&wfx[num], &format.fmt, sizeof(format.fmt));
		//1�T���v���ӂ�̃o�b�t�@�T�C�Y���Z�o���܂�
		wfx[num].wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;


		// �\�[�X�{�C�X�쐬
		if (FAILED(g_pXAudio2->CreateSourceVoice(&(sound_fodata.g_pSourceVoice[num]), &(wfx[num].Format)))) {
			//free(pbuffer);
			//�G���[���b�Z
			return;
		}
		//�Đ�����g�`�f�[�^�̐ݒ�
		buffer[num].pAudioData = (BYTE*)pbuffer;
		buffer[num].Flags = XAUDIO2_END_OF_STREAM;
		buffer[num].AudioBytes = data.size;

		//FindFileNum(filename);

		//�\�[�X�{�C�X�쐬
		//g_pXAudio2->CreateSourceVoice(&sound_fodata.g_pSourceVoice[num], &wfx[num]);
		//�G���[���b�Z�[�W�o���悤��
		*/

		//�T�u�~�b�g�{�C�X�ŗ��p���鍷���o�b�t�@�̐ݒ�
		sound_fodata.filedata[fonum].buffer[finum].AudioBytes = dwChunkSize;
		sound_fodata.filedata[fonum].buffer[finum].pAudioData = sound_fodata.filedata[fonum].pDataBuffer[finum];
		sound_fodata.filedata[fonum].buffer[finum].Flags = XAUDIO2_END_OF_STREAM;
		if (sound_fodata.filedata[fonum].bLoop[finum]) {
			sound_fodata.filedata[fonum].buffer[finum].LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		else {
			sound_fodata.filedata[fonum].buffer[finum].LoopCount = 0;
		}
		g_pXAudio2->CreateSourceVoice(&sound_fodata.filedata[fonum].g_pSourceVoice[finum], &(sound_fodata.filedata[fonum].wfx[finum].Format));
	//}
}

int FindFileNum(int foldernum, string filename) {
	if (foldernum > -1) {
		//�w��t�H���_���̊e�t�@�C����T��
		for (int i = 0; i < sound_fodata.filedata[foldernum].filenum; i++) {
			//�Y���̃t�@�C�������݂��邩�ǂ���
			if (filename == sound_fodata.filedata[foldernum].filename[i]) {
				return i;		//������Ό��������n�_�̗v�f��Ԃ�
			}

		}
	}
	return -1;		//�G���[

}

int FindFolderNum(string path) {
	//����/���܂܂�Ă��Ȃ����
	if (path.find_first_of(TEXT("/")) == string::npos) {
		for (int i = 0; i < sound_fodata.foldernum; i++) {
			if (path == sound_fodata.type[i]) {
				return i;
			}
		}
	}
	for (int i = 0; i < sound_fodata.foldernum; i++) {
		//�w��t�H���_�ł����
		if (path == sound_fodata.folderpath[i]) {
			return i;
		}
	}
	return -1;
}

//------------------------------------------------
//�擾�f�B���N�g�����̐����֐�
//------------------------------------------------
void Normalization(DIRECTORY_DATA* _data) {

	if (_data != NULL) {
		int foldernum = FileOperation::GetFolderSum(_data, F_FIND_MODE::FM_FILE_EXISTANCE);		//�t�@�C���̑��݂���t�H���_�̐����擾
		int num = FileOperation::GetFileSum(_data, TEXT(".wav"));
		if (num > 0) {
			vector<string> path;
			sound_fodata.foldernum = foldernum;				//���p����t�H���_���擾
			//sound_fodata.folderpath.resize(foldernum);		//�t�H���_�p�X�v�f���ύX(�t�@�C���̑��݂���t�H���_����)
			sound_fodata.filedata.resize(sound_fodata.foldernum);		//�t�@�C�����v�f���ύX
			//sound_fodata.folderpath[0] = DefaultDirectory;				//�擪�f�B���N�g����ێ�
			path = FileOperation::GetFolderPath(_data, F_FIND_MODE::FM_FILE_EXISTANCE);		//�t�@�C���̑��݂���t�H���_�[�p�X���擾
			sound_fodata.folderpath.insert(sound_fodata.folderpath.end(), path.begin(), path.end());		//�t�H���_���X�g�\�z
			sound_fodata.type.resize(foldernum);			//�t�H���_������ނ��m��
			//�t�@�C�����擾
			for (int i = 0; i < sound_fodata.folderpath.size(); i++) {
				sound_fodata.filedata[i].filename = FileOperation::GetFileName(sound_fodata.folderpath[i]);		//�t�@�C�����擾
				sound_fodata.filedata[i].filenum = sound_fodata.filedata[i].filename.size();		//�t�@�C������ێ�
				sound_fodata.filedata[i].loadflag.resize(sound_fodata.filedata[i].filenum);			//�t�@�C�������f�[�^�쐬(�\�[�X�{�C�X)
				sound_fodata.filedata[i].bLoop.resize(sound_fodata.filedata[i].filenum);			//�t�@�C�������f�[�^�쐬(�\�[�X�{�C�X)
				sound_fodata.filedata[i].g_pSourceVoice.resize(sound_fodata.filedata[i].filenum);	//�t�@�C�������f�[�^�쐬(�\�[�X�{�C�X)
				sound_fodata.filedata[i].wfx.resize(sound_fodata.filedata[i].filenum);				//�t�@�C�������f�[�^�쐬(wfx)
				sound_fodata.filedata[i].buffer.resize(sound_fodata.filedata[i].filenum);			//�t�@�C�������f�[�^�쐬(�o�b�t�@)
				sound_fodata.filedata[i].pDataBuffer.resize(sound_fodata.filedata[i].filenum);		//�t�@�C�������f�[�^�쐬(�o�b�t�@BYTE)
			}
			SetSoundType();
			/*
			sound_fodata.g_pSourceVoice.resize(num);
			wfx.resize(num);
			buffer.resize(num);
			*/
			sound_order_state = true;
			return;
		}
		string str = TEXT("�t�@�C�������݂��Ă��܂���B");
		ErrorMessage::ErrorMessageBox(str.c_str(), TEXT("XAudio2 Init Error"), MB_OK);
		return;
	}
	string str = TEXT("�f�B���N�g�����̎擾�Ɏ��s���Ă��܂��B");
	ErrorMessage::ErrorMessageBox(str.c_str(), TEXT("XAudio2 Init Error"), MB_OK);
}

void SetVolume(float per) {
	
}

void SetVolume(float per, string type) {
	//�W�����������̎d�g�݂�K���p�ӂ���
}

void SetSoundType() {
	int foldersum = sound_fodata.foldernum;		//�t�H���_�[�����擾

	for (int i = 0; i < foldersum; i++) {
		sound_fodata.type[i] = StringOperation::Split(sound_fodata.folderpath[i], '/').back();		//�����̃t�H���_�����擾
	}
	
}

void SetSoundType(string oldname, string newname) {
	//��v�����ނ𒲂ׂĂ���
	for (int i = 0; i < sound_fodata.foldernum; i++) {
		if (sound_fodata.type[i] == oldname) {
			sound_fodata.type[i] = newname;		//�V�K���ɕύX
			return;
		}
	}
	//������Ȃ���΃G���[���b�Z�[�W��\��
	ErrorMessage::ErrorMessageBox(TEXT("�ύX�悪������܂���ł���"), TEXT("XAudio2 Set Error"), MB_OK);
	exit(1);
}



//=============================================================================
// ���[�e�B���e�B�֐��Q
//=============================================================================
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;
	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}
		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}
		dwOffset += dwChunkDataSize;
		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}
	return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}