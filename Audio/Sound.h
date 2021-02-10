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
	CString      m_strFileName;		//파일 이름
	WAVEFORMATEX m_WaveFormatEx;	//Wav Format 구조체
	UINT         m_nSize;					//크기
	UINT         m_nDuration;			//길이 (Second)
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

