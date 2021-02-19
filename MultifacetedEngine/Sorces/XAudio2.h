//=============================================================================
//
// �T�E���h���� [XAudio2.h]
//
//=============================================================================
//#ifndef _XAUDIO2_H_
//#define _XAUDIO2_H_
#pragma once
#include <xaudio2.h>
#include "Win.h"

using namespace std;

//#ifdef UNICODE
//#define string wstring
//#else
//#define string string
//#endif
// �T�E���h�t�@�C��
/*
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// �T���v��BGM
	SOUND_LABEL_BGM001,			// �T���v��BGM
	SOUND_LABEL_MAX,
} SOUND_LABEL;
*/

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(void);
void UninitSound(void);
void PlaySound(wstring path, wstring filename);
void StopSound(wstring path, wstring filename);
void PauseSound(wstring path, wstring filename);
void LoadWave(wstring path, wstring filename, bool loop);
void SetAudio(float per);
void SetSoundType();
void SetSoundType(wstring oldname, wstring newname);
//void AddSoundType(string type);