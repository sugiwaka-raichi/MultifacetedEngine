#pragma once
#define _XAUDIO2_H_

#include <xaudio2.h>

//サウンドファイル
typedef enum {
	SOUND_LABEL_BGM000 = 0,		//BGM0
	SOUND_LABEL_BGM001,			//
//	SOUND_LABEL_BGM002,			//
//	SOUND_LABEL_SE000,			//SE
	SOUND_LABEL_MAX				//サウンド合計
}SOUND_LABEL;

//パラメータ構造体
typedef struct {
	LPCSTR filename;		//音声ファイルまでのパス設定
	bool bLoop;				//trueでループ。通常ＢＧＭはtrue、ＳＥはfalse
}PARAM;

//ファイルパスとループの設定
//####################################################################
//ロード関数でファイルパスのリストとtrue,falseの初期設定を取得したい
//####################################################################
const PARAM g_param[SOUND_LABEL_MAX] = {
	{/*"0.wav"*/"Asset/Sound/BGM/play.wav",true},	//BGM000(ループ有)
	{"Asset/Sound/BGM/room.wav",true},
	//	{"Asset/Sound/BGM/bgm002.wav",true},
	//	{"Asset/Sound/SE/se000.wav",false}
};

class XAudio2 {
private:

	//----------------------------------------------------------------------
	//グローバル変数
	//----------------------------------------------------------------------
	IXAudio2				*g_pXAudio2 = NULL;
	IXAudio2MasteringVoice	*g_pMasteringVoice = NULL;
	IXAudio2SourceVoice		*g_pSourceVoice[SOUND_LABEL_MAX];

	WAVEFORMATEXTENSIBLE	wfx[SOUND_LABEL_MAX];			//WAVフォーマット
	XAUDIO2_BUFFER			buffer[SOUND_LABEL_MAX];		//
	BYTE					*pDataBuffer[SOUND_LABEL_MAX];	//

	//===========================================================
	//プロトタイプ宣言
	//===========================================================
	HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);		//
	HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);		//
public:
	//===========================================================
	//プロトタイプ宣言
	//===========================================================
	XAudio2();
	~XAudio2();
	HRESULT InitSound(void);		//サウンド初期化
	void UninitSound(void);			//サウンド解放
	void PlaySound(SOUND_LABEL _label);			//サウンド再生
	void StopSound(SOUND_LABEL _label);			//サウンド停止
	void PauseSound(SOUND_LABEL _label);		//サウンド一時停止
	void LoadSound(SOUND_LABEL _label);			//サウンドロード
	void UnLoadSound(SOUND_LABEL _label);		//サウンドアンロード
};