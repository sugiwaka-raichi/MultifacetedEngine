#pragma once
#define _XAUDIO2_H_

#include <xaudio2.h>

//�T�E���h�t�@�C��
typedef enum {
	SOUND_LABEL_BGM000 = 0,		//BGM0
	SOUND_LABEL_BGM001,			//
//	SOUND_LABEL_BGM002,			//
//	SOUND_LABEL_SE000,			//SE
	SOUND_LABEL_MAX				//�T�E���h���v
}SOUND_LABEL;

//�p�����[�^�\����
typedef struct {
	LPCSTR filename;		//�����t�@�C���܂ł̃p�X�ݒ�
	bool bLoop;				//true�Ń��[�v�B�ʏ�a�f�l��true�A�r�d��false
}PARAM;

//�t�@�C���p�X�ƃ��[�v�̐ݒ�
//####################################################################
//���[�h�֐��Ńt�@�C���p�X�̃��X�g��true,false�̏����ݒ���擾������
//####################################################################
const PARAM g_param[SOUND_LABEL_MAX] = {
	{/*"0.wav"*/"Asset/Sound/BGM/play.wav",true},	//BGM000(���[�v�L)
	{"Asset/Sound/BGM/room.wav",true},
	//	{"Asset/Sound/BGM/bgm002.wav",true},
	//	{"Asset/Sound/SE/se000.wav",false}
};

class XAudio2 {
private:

	//----------------------------------------------------------------------
	//�O���[�o���ϐ�
	//----------------------------------------------------------------------
	IXAudio2				*g_pXAudio2 = NULL;
	IXAudio2MasteringVoice	*g_pMasteringVoice = NULL;
	IXAudio2SourceVoice		*g_pSourceVoice[SOUND_LABEL_MAX];

	WAVEFORMATEXTENSIBLE	wfx[SOUND_LABEL_MAX];			//WAV�t�H�[�}�b�g
	XAUDIO2_BUFFER			buffer[SOUND_LABEL_MAX];		//
	BYTE					*pDataBuffer[SOUND_LABEL_MAX];	//

	//===========================================================
	//�v���g�^�C�v�錾
	//===========================================================
	HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);		//
	HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);		//
public:
	//===========================================================
	//�v���g�^�C�v�錾
	//===========================================================
	XAudio2();
	~XAudio2();
	HRESULT InitSound(void);		//�T�E���h������
	void UninitSound(void);			//�T�E���h���
	void PlaySound(SOUND_LABEL _label);			//�T�E���h�Đ�
	void StopSound(SOUND_LABEL _label);			//�T�E���h��~
	void PauseSound(SOUND_LABEL _label);		//�T�E���h�ꎞ��~
	void LoadSound(SOUND_LABEL _label);			//�T�E���h���[�h
	void UnLoadSound(SOUND_LABEL _label);		//�T�E���h�A�����[�h
};