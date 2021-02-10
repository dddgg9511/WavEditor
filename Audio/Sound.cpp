#include "stdafx.h"
#include "Sound.h"


SOUND::SOUND(const CString& fileName)
{
	int flag = fileName.ReverseFind(_T('\\'));
	m_strFileName = fileName.Mid(flag + 1);

	HANDLE hFile;

	if(hFile = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL))
	{
		char szIdentifier[5] = {0};

		SetFilePointer(hFile, 12, NULL, FILE_CURRENT);

		RIFF_CHUNK RiffChunk = {0};
		DWORD dwBytes;
		WAVE_FILE_HEADER WaveFileHeader;
		ReadFile(hFile, &RiffChunk, sizeof(RiffChunk), &dwBytes, NULL);
		ReadFile(hFile, &WaveFileHeader, sizeof(WaveFileHeader), &dwBytes, NULL);

		m_WaveFormatEx.wFormatTag      = WaveFileHeader.wFormatTag;         
		m_WaveFormatEx.nChannels       = WaveFileHeader.wChannels;          
		m_WaveFormatEx.nSamplesPerSec  = WaveFileHeader.dwSamplesPerSec;    
		m_WaveFormatEx.nAvgBytesPerSec = WaveFileHeader.dwAvgBytesPerSec;   
		m_WaveFormatEx.nBlockAlign     = WaveFileHeader.wBlockAlign;  
		m_WaveFormatEx.wBitsPerSample  = WaveFileHeader.wBitsPerSample;
		m_WaveFormatEx.cbSize          = 0;
		DWORD dwIncrementBytes;

		dwIncrementBytes = dwBytes; 
		DWORD dwErrorCheck;
		do {
			SetFilePointer(hFile, RiffChunk.dwLength - dwIncrementBytes, NULL, FILE_CURRENT);

			dwErrorCheck = GetLastError();

			if(dwErrorCheck == 0)
			{
				dwBytes = ReadFile(hFile, &RiffChunk, sizeof(RiffChunk), &dwBytes, NULL);

				dwIncrementBytes = 0;

				memcpy(szIdentifier, RiffChunk.IdentifierString, 4); 
			}

		} while(_stricmp(szIdentifier, "data") && dwErrorCheck == 0) ;

		if(dwErrorCheck == 0)
		{
			m_pWaveData = new char[RiffChunk.dwLength];

			m_nSize = RiffChunk.dwLength;
			m_nDuration = m_nSize / m_WaveFormatEx.nAvgBytesPerSec;

			ReadFile(hFile, m_pWaveData, RiffChunk.dwLength, &dwBytes, NULL);

			CloseHandle(hFile);
		}
	}
}


SOUND::~SOUND(void)
{
	delete[] m_pWaveData;
}



COPYSOUND::COPYSOUND( const WAVEFORMATEX& format, short* left, short* right )
	:	m_WaveFormatEx(format),
		m_leftChannel(left),
		m_rightChannel(right)
{
	
}

COPYSOUND::~COPYSOUND()
{
	delete m_leftChannel;
	if(m_rightChannel)
	{
		delete m_rightChannel;
	}
}
