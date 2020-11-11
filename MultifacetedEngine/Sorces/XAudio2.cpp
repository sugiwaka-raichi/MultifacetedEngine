//=============================================================================
//
// サウンド処理 [XAudio2.cpp]
//
//=============================================================================

#include "XAudio2.h"
#include "ERROR.h"
#include "TextureManager.h"
#include "StringOperation.h"

// パラメータ構造体
typedef struct {
	int				filenum;	//ファイル数
	vector<string>	filename;	//ファイル名
	vector<bool>	bLoop;		// デフォルトでfalse。
	vector<bool>	loadflag;	//ロードフラグ
	vector<IXAudio2SourceVoice*>    g_pSourceVoice;
	vector<WAVEFORMATEXTENSIBLE>	wfx;			// WAVフォーマット
	vector<XAUDIO2_BUFFER>			buffer;
	vector<BYTE*>					pDataBuffer;
} SOUND_FILE_DATA;

typedef struct {
	int				foldernum;			//フォルダ数
	vector<string>	type;				//ファイルの種類(SE,BGMなど)
	vector<string>  folderpath;			//フォルダのパス
	vector<SOUND_FILE_DATA>	filedata;	//ファイル情報
}SOUND_FOLDER_DATA;

/*
typedef struct {
	vector<SOUND_FILE_DATA>	sound_file_data;		//サウンドファイルデータ
} SOUND_FILE_INFO;
SOUND_FILE_INFO soundfileinfo;		//サウンドファイル情報
*/

/*
PARAM g_param[SOUND_LABEL_MAX] =
{
	{"Asset/Sound/BGM/play.wav", true},	// サンプルBGM（ループさせるのでtrue設定）
	{"Asset/Sound/BGM/room.wav", true},	// サンプルBGM
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

//チャンクデータの基本構造
struct Chunk {
	char id[4];		//チャンクごとのID
	int32_t size;	//チャンクサイズ
};

//RIFFヘッダー
struct RiffHeader {
	Chunk	chunk;		//"RIFF"
	char	type[4];	//"WAVE"
};

//FMTチャンク
struct FormatChunk {
	Chunk		chunk;		//"fmt"
	WAVEFORMAT	fmt;		//波形フォーマット
};

//-----------------------------------------------------------------
//    グローバル変数
//-----------------------------------------------------------------
IXAudio2               *g_pXAudio2        = NULL;
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;

char* pbuffer;
string DefaultDirectory = TEXT("./Asset/Sound/") ;		//デフォルトディレクトリパス
SOUND_FOLDER_DATA sound_fodata;						//サウンドファイル情報
bool sound_order_state = false;						//サウンド命令許可状態

//-----------------------------------------------------------------
//    プロトタイプ宣言
//-----------------------------------------------------------------
void Normalization(DIRECTORY_DATA* _data);
int FindFolderNum(string path);
int FindFileNum(int foldernum,string filename);

HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
HRESULT ReadChunkData( HANDLE , void* , DWORD , DWORD );


//=============================================================================
// 初期化
//=============================================================================
HRESULT InitSound() {
	HRESULT      hr;
	Normalization(FileOperation::GetDirectoryData(DefaultDirectory));	//サウンドフォルダ情報取得

	//Comコンポーネント初期化
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		ErrorMessage::ErrorMessageBox(TEXT("Comコンポーネントの初期化に失敗しました。"), TEXT("XAudio2 InitError"), MB_OK);
		exit(1);
	}

	//XAudio2の初期化
	hr = XAudio2Create(&g_pXAudio2);		//XAudio2のインスタンスを返す
	if (FAILED(hr)) {
		ErrorMessage::ErrorMessageBox(TEXT("XAudio2の作成に失敗しました。"), TEXT("XAudio2 InitError"), MB_OK);
		exit(1);
	}

	//マスターボイスは再生されるボイスすべての最終到達地点
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);		//マスターボイス作成
	if (FAILED(hr)) {
		ErrorMessage::ErrorMessageBox(TEXT("マスターボイスの作成に失敗しました。"), TEXT("XAudio2 InitError"), MB_OK);
		exit(1);
	}

	//ソースボイス作成 wfex　フォーマット
	/*
	hr = g_pXAudio2->CreateSourceVoice(&g_pSourceVoice, &wfex);
	if (FAILED(hr)) {
		ErrorMessage::ErrorMessageBox("ソースボイスの作成に失敗しました。", "XAudio2 Init Error", MB_OK);
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

		// 	サブミットボイスで利用するサブミットバッファの設定
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
// 開放処理
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
			g_pSourceVoice[i]->DestroyVoice();			// オーディオグラフからソースボイスを削除
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
// 再生
//=============================================================================
void PlaySound(string path, string filename) {
	if (sound_order_state) {
		int fonum = FindFolderNum(path);
		int finum = FindFileNum(fonum, filename);

		//ソースボイスがあるかどうか
		if (sound_fodata.filedata[fonum].g_pSourceVoice[finum] == NULL) {
			//なければソースボイス作成
			g_pXAudio2->CreateSourceVoice(&sound_fodata.filedata[fonum].g_pSourceVoice[finum], &(sound_fodata.filedata[fonum].wfx[finum].Format));
		}

		sound_fodata.filedata[fonum].g_pSourceVoice[finum]->SubmitSourceBuffer(&(sound_fodata.filedata[fonum].buffer[finum]));	// ボイスキューに新しいオーディオバッファーを追加
		// 再生
		sound_fodata.filedata[fonum].g_pSourceVoice[finum]->Start(0);
	}
	else {
		string str = TEXT("filename:") + filename + TEXT("は読み込まれていません。\n終了する場合はOK");
		if (IDOK == ErrorMessage::ErrorMessageBox(str.c_str(), TEXT("XAudio2 Play Error"), MB_OKCANCEL)) {
			exit(1);
		}
	}
}

//=============================================================================
// 停止
//=============================================================================
void StopSound(string path, string filename) {
	if (sound_order_state) {
		int fonum = FindFolderNum(path);
		int finum = FindFileNum(fonum, filename);
		//ソースボイスが作成されていなければ何もしない
		if (sound_fodata.filedata[fonum].g_pSourceVoice[finum] == NULL) {
			return;
		}

		XAUDIO2_VOICE_STATE	xa2state;

		sound_fodata.filedata[fonum].g_pSourceVoice[finum]->GetState(&xa2state);
		if (xa2state.BuffersQueued) {
			// 停止
			sound_fodata.filedata[fonum].g_pSourceVoice[finum]->Stop(0);
			sound_fodata.filedata[fonum].g_pSourceVoice[finum] = NULL;		//ソースボイスをリセット
		}
	}
	else {
		string str = TEXT("filename:") + filename + TEXT("は読み込まれていません。");
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
// 一時停止
//=============================================================================
void PauseSound(string path, string filename) {
	if (sound_order_state) {
		int fonum = FindFolderNum(path);
		int finum = FindFileNum(fonum, filename);
		//ソースボイスが作成されていなければ何もしない
		if (sound_fodata.filedata[fonum].g_pSourceVoice[finum] == NULL) {
			return;
		}

		XAUDIO2_VOICE_STATE	xa2state;

		sound_fodata.filedata[fonum].g_pSourceVoice[finum]->GetState(&xa2state);
		if (xa2state.BuffersQueued) {
			// 停止
			sound_fodata.filedata[fonum].g_pSourceVoice[finum]->Stop(0);
		}
	}
	else {
		string str = TEXT("filename:") + filename + TEXT("は読み込まれていません。");
		ErrorMessage::ErrorMessageBox(str.c_str(), TEXT("XAudio2 Pause Error"), MB_OK);
	}
}

//=============================================================================
//ロード処理(?)
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
			string str = TEXT("ファイル名:") + filename + TEXT("の参照に失敗しました。\nファイルが見つかりませんでした。");
			ErrorMessage::ErrorMessageBox(str.c_str(), TEXT("Xaudio2 Load Error"), MB_OK);
			
			return;
		}
		//Waveファイルを開く
		string filepath = path + filename;		//ファイルパス作成
		hFile = CreateFile(filepath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			//エラーメッセ
			return;
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
			//エラーメッセ
			return;
		}
		/*
		if (fopen_s(&file, filepath.c_str(), "rb") != 0) {
			string str = "ファイル名:" + filename + "の読込に失敗しました。\nファイルが見つかりませんでした。";
			ErrorMessage::ErrorMessageBox(str.c_str(), "XAudio2 Load Error", MB_OK);
			return;
		}
		*/
		//check the file type,should be fourccWAVE or 'XWMA'
		FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (filetype != fourccWAVE) {
			return;		//エラーメッセ
		}

		FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &sound_fodata.filedata[fonum].wfx[finum], dwChunkSize, dwChunkPosition);

		//fill out the audio data buffer with the contents of the fourccDATA chunk
		FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		sound_fodata.filedata[fonum].pDataBuffer[finum] = new BYTE[dwChunkSize];
		ReadChunkData(hFile, sound_fodata.filedata[fonum].pDataBuffer[finum], dwChunkSize, dwChunkPosition);

		CloseHandle(hFile);

		sound_fodata.filedata[fonum].bLoop[finum] = loop;		//ループフラグを入れる
		/*
		//違う読込の仕方
		//RIFFヘッダ０の読込
		RiffHeader riff;
		fread(&riff, sizeof(riff), 1, file);

		//Formatチャンクの読込
		FormatChunk format;
		fread(&format, sizeof(format), 1, file);

		//Dataチャンクの読込
		Chunk data;
		fread(&data, sizeof(data), 1, file);

		//Dataチャンクのデータ部　（波形データ）の読込
		pbuffer = (char*)malloc(data.size);
		fread(pbuffer, data.size, 1, file);

		fclose(file);

		//WAVEFORMATEX wfex{	};
		//波形フォーマットの設定
		memcpy(&wfx[num], &format.fmt, sizeof(format.fmt));
		//1サンプル辺りのバッファサイズを算出します
		wfx[num].wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;


		// ソースボイス作成
		if (FAILED(g_pXAudio2->CreateSourceVoice(&(sound_fodata.g_pSourceVoice[num]), &(wfx[num].Format)))) {
			//free(pbuffer);
			//エラーメッセ
			return;
		}
		//再生する波形データの設定
		buffer[num].pAudioData = (BYTE*)pbuffer;
		buffer[num].Flags = XAUDIO2_END_OF_STREAM;
		buffer[num].AudioBytes = data.size;

		//FindFileNum(filename);

		//ソースボイス作成
		//g_pXAudio2->CreateSourceVoice(&sound_fodata.g_pSourceVoice[num], &wfx[num]);
		//エラーメッセージ出すように
		*/

		//サブミットボイスで利用する差分バッファの設定
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
		//指定フォルダ内の各ファイルを探索
		for (int i = 0; i < sound_fodata.filedata[foldernum].filenum; i++) {
			//該当のファイルが存在するかどうか
			if (filename == sound_fodata.filedata[foldernum].filename[i]) {
				return i;		//見つかれば見つかった地点の要素を返す
			}

		}
	}
	return -1;		//エラー

}

int FindFolderNum(string path) {
	//もし/が含まれていなければ
	if (path.find_first_of(TEXT("/")) == string::npos) {
		for (int i = 0; i < sound_fodata.foldernum; i++) {
			if (path == sound_fodata.type[i]) {
				return i;
			}
		}
	}
	for (int i = 0; i < sound_fodata.foldernum; i++) {
		//指定フォルダであれば
		if (path == sound_fodata.folderpath[i]) {
			return i;
		}
	}
	return -1;
}

//------------------------------------------------
//取得ディレクトリ情報の整理関数
//------------------------------------------------
void Normalization(DIRECTORY_DATA* _data) {

	if (_data != NULL) {
		int foldernum = FileOperation::GetFolderSum(_data, F_FIND_MODE::FM_FILE_EXISTANCE);		//ファイルの存在するフォルダの数を取得
		int num = FileOperation::GetFileSum(_data, TEXT(".wav"));
		if (num > 0) {
			vector<string> path;
			sound_fodata.foldernum = foldernum;				//利用するフォルダ数取得
			//sound_fodata.folderpath.resize(foldernum);		//フォルダパス要素数変更(ファイルの存在するフォルダ数分)
			sound_fodata.filedata.resize(sound_fodata.foldernum);		//ファイル情報要素数変更
			//sound_fodata.folderpath[0] = DefaultDirectory;				//先頭ディレクトリを保持
			path = FileOperation::GetFolderPath(_data, F_FIND_MODE::FM_FILE_EXISTANCE);		//ファイルの存在するフォルダーパスを取得
			sound_fodata.folderpath.insert(sound_fodata.folderpath.end(), path.begin(), path.end());		//フォルダリスト構築
			sound_fodata.type.resize(foldernum);			//フォルダ数分種類を確保
			//ファイル情報取得
			for (int i = 0; i < sound_fodata.folderpath.size(); i++) {
				sound_fodata.filedata[i].filename = FileOperation::GetFileName(sound_fodata.folderpath[i]);		//ファイル名取得
				sound_fodata.filedata[i].filenum = sound_fodata.filedata[i].filename.size();		//ファイル数を保持
				sound_fodata.filedata[i].loadflag.resize(sound_fodata.filedata[i].filenum);			//ファイル数分データ作成(ソースボイス)
				sound_fodata.filedata[i].bLoop.resize(sound_fodata.filedata[i].filenum);			//ファイル数分データ作成(ソースボイス)
				sound_fodata.filedata[i].g_pSourceVoice.resize(sound_fodata.filedata[i].filenum);	//ファイル数分データ作成(ソースボイス)
				sound_fodata.filedata[i].wfx.resize(sound_fodata.filedata[i].filenum);				//ファイル数分データ作成(wfx)
				sound_fodata.filedata[i].buffer.resize(sound_fodata.filedata[i].filenum);			//ファイル数分データ作成(バッファ)
				sound_fodata.filedata[i].pDataBuffer.resize(sound_fodata.filedata[i].filenum);		//ファイル数分データ作成(バッファBYTE)
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
		string str = TEXT("ファイルが存在していません。");
		ErrorMessage::ErrorMessageBox(str.c_str(), TEXT("XAudio2 Init Error"), MB_OK);
		return;
	}
	string str = TEXT("ディレクトリ情報の取得に失敗しています。");
	ErrorMessage::ErrorMessageBox(str.c_str(), TEXT("XAudio2 Init Error"), MB_OK);
}

void SetVolume(float per) {
	
}

void SetVolume(float per, string type) {
	//ジャンル分けの仕組みを必ず用意する
}

void SetSoundType() {
	int foldersum = sound_fodata.foldernum;		//フォルダー総数取得

	for (int i = 0; i < foldersum; i++) {
		sound_fodata.type[i] = StringOperation::Split(sound_fodata.folderpath[i], '/').back();		//末尾のフォルダ名を取得
	}
	
}

void SetSoundType(string oldname, string newname) {
	//一致する種類を調べていく
	for (int i = 0; i < sound_fodata.foldernum; i++) {
		if (sound_fodata.type[i] == oldname) {
			sound_fodata.type[i] = newname;		//新規名に変更
			return;
		}
	}
	//見つからなければエラーメッセージを表示
	ErrorMessage::ErrorMessageBox(TEXT("変更先が見つかりませんでした"), TEXT("XAudio2 Set Error"), MB_OK);
	exit(1);
}



//=============================================================================
// ユーティリティ関数群
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