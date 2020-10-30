#include "XAudio2.h"
#include "Win.h"
#include "ERROR.h"

//======================================================================
//マクロ定義
//======================================================================
#define fourccRIFF	'FFIR'
#define fourccDATA	'atad'
#define fourccFMT	'tmf'
#define fourccWAVE	'EVAW'
#define fourccXWMA	'AMWX'
#define fourccDPDS	'adpd'

//======================================================================
//コンストラクタ
//======================================================================
XAudio2::XAudio2() {
	//InitSound();
}

//======================================================================
//デストラクタ
//======================================================================
XAudio2::~XAudio2() {
	//UninitSound();
}

//======================================================================
//初期設定
//======================================================================
HRESULT XAudio2::InitSound() {
	HRESULT hr;
	HANDLE  hFile;
	DWORD	dwChunkSize;
	DWORD	dwChunkPosition;
	DWORD	filetype;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	
	/*** Create XAudio2 ***/
	hr = XAudio2Create(&g_pXAudio2, 0);
	if (FAILED(hr)) {
		CoUninitialize();
		return -1;
	}

	//*** Create Mastering Voice ***/
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if (FAILED(hr)) {
		if (g_pXAudio2) g_pXAudio2->Release();
		CoUninitialize();
		return -1;
	}

	/*** Initalize Sound ***/
	for (int i = 0; i < SOUND_LABEL_MAX; i++) {
		memset(&wfx[i], 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer[i], 0, sizeof(XAUDIO2_BUFFER));

		hFile = CreateFile(g_param[i].filename, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
			return HRESULT_FROM_WIN32(GetLastError());
		}

		//check the file type, should be fourccWAVE or 'XWMA'
		FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (filetype != fourccWAVE)	return S_FALSE;

		FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &wfx[i], dwChunkSize, dwChunkPosition);

		//fill out the audio data buffer with the contents of the forccDATA chunk
		FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		pDataBuffer[i] = new BYTE[dwChunkSize];
		ReadChunkData(hFile, pDataBuffer[i], dwChunkSize, dwChunkPosition);

		CloseHandle(hFile);

		//サブミットボイスで利用する差分とバッファの設定
		buffer[i].AudioBytes = dwChunkSize;
		buffer[i].pAudioData = pDataBuffer[i];
		buffer[i].Flags = XAUDIO2_END_OF_STREAM;
		if (g_param[i].bLoop) {
			buffer[i].LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		else {
			buffer[i].LoopCount = 0;
		}

		g_pXAudio2->CreateSourceVoice(&g_pSourceVoice[i], &(wfx[i].Format));
	}
	return hr;
}

//======================================================================
//終了処理
//======================================================================
void XAudio2::UninitSound() {
	for (int i = 0; i < SOUND_LABEL_MAX; i++) {
		if (g_pSourceVoice[i]) {
			g_pSourceVoice[i]->Stop(0);
			g_pSourceVoice[i]->FlushSourceBuffers();
			g_pSourceVoice[i]->DestroyVoice();				//オーディオグラフからソースボイスを削除
			delete[] pDataBuffer[i];
		}
	}

	g_pMasteringVoice->DestroyVoice();

	if (g_pXAudio2) g_pXAudio2->Release();

	CoUninitialize();
}

//=======================================================================
//再生
//=======================================================================
void XAudio2::PlaySound(SOUND_LABEL _label) {
	//ソースボイス作成
	//if (g_pSourceVoice[(int)_label] == NULL) {
	//	ErrorMessage error;
	//	error.ErrorMessageBox(((int)_label) + "がNULLでした。", "XAudio2ERROR", MB_OK);
	//	return;
	//}
	//else {
		g_pXAudio2->CreateSourceVoice(&(g_pSourceVoice[(int)_label]), &(wfx[(int)_label].Format));
		g_pSourceVoice[(int)_label]->SubmitSourceBuffer(&(buffer[(int)_label]));				//ボイスキューに新しいオーディオバッファーを追加

		//再生
		g_pSourceVoice[(int)_label]->Start(0);
	//}
}

//=======================================================================
//停止
//=======================================================================
void XAudio2::StopSound(SOUND_LABEL _label) {
	//ソースボイスがなければリターン
	if (g_pSourceVoice[(int)_label] == NULL) {
		return;
	}
	XAUDIO2_VOICE_STATE xa2state;
	g_pSourceVoice[(int)_label]->GetState(&xa2state);
	if (xa2state.BuffersQueued) {
		g_pSourceVoice[(int)_label]->Stop(0);
	}
}

//========================================================================
//一時停止
//========================================================================
void XAudio2::PauseSound(SOUND_LABEL _label) {
	//ソースボイスがなければリターン
	if (g_pSourceVoice[(int)_label] == NULL) {
		return;
	}
	XAUDIO2_VOICE_STATE xa2state;
	g_pSourceVoice[(int)_label]->GetState(&xa2state);
	if (xa2state.BuffersQueued) {
		g_pSourceVoice[(int)_label]->Stop(10000);
	}
}

//========================================================================
//ユーティリティ関数群
//========================================================================
HRESULT XAudio2::FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition) {
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;
	while (hr == S_OK) {
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL)) {
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL)) {
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
		switch (dwChunkType) {
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL)) {
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;
		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT)) {
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}
		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == fourcc) {
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}
		dwOffset += dwChunkDataSize;
		if (bytesRead >= dwRIFFDataSize) {
			return S_FALSE;
		}
	}
	return S_OK;
}

HRESULT XAudio2::ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset) {
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN)) {
		return HRESULT_FROM_WIN32(GetLastError());
	}
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL)) {
		hr = HRESULT_FROM_WIN32(GetLastError());
	}
	return hr;
}


