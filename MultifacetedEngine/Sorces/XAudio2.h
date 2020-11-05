//=============================================================================
//
// サウンド処理 [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include <string>
#include <xaudio2.h>

using namespace std;

// サウンドファイル
/*
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// サンプルBGM
	SOUND_LABEL_BGM001,			// サンプルBGM
	SOUND_LABEL_MAX,
} SOUND_LABEL;
*/

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(void);
void UninitSound(void);
void PlaySound(string path, string filename);
void StopSound(string path, string filename);
void PauseSound(string path, string filename);
void LoadWave(string path, string filename, bool loop);
void SetAudio(float per);
void SetSoundType();
void SetSoundType(string oldname, string newname);
//void AddSoundType(string type);

#endif
