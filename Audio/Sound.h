#pragma once
struct RIFF_CHUNK 
{
	UCHAR IdentifierString[4];
	DWORD dwLength;
};
struct WAVE_FILE_HEADER 
{
	WORD  wFormatTag;         
	WORD  wChannels;          
	DWORD dwSamplesPerSec;    
	DWORD dwAvgBytesPerSec;   
	WORD  wBlockAlign;        
	WORD  wBitsPerSample;
};

struct SOUND
{
	SOUND(const CString& fileName);
	~SOUND(void);
	CString      m_strFileName;		//���� �̸�
	WAVEFORMATEX m_WaveFormatEx;	//Wav Format ����ü
	UINT         m_nSize;					//ũ��
	UINT         m_nDuration;			//���� (Second)
	char*        m_pWaveData;			//Data
};

struct COPYSOUND
{

	COPYSOUND(const WAVEFORMATEX& format, short* left, short* right);
	~COPYSOUND();
	//PcmData LeftChannel
	short* m_leftChannel;
	//PcmData RightChannel
	short* m_rightChannel;
	//WaveFormat
	WAVEFORMATEX m_WaveFormatEx;
};

