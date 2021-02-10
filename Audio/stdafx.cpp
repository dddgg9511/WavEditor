
// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// Prototype.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"


REAL ByteToCoordinate(const int& byte, const int& bytePerSec, const int& timeStep)
{
	REAL pos = static_cast<REAL>(byte) / bytePerSec;
	pos *= timeStep;
	pos += 5.f;
	return pos;
}

int CoordinateToByte(const REAL& pos, const int& bytePerSec, const int& timeStep)
{
	REAL byte = pos - 5.f;
	byte /= timeStep;
	byte *= bytePerSec;
	return static_cast<int>(byte);
}

CString ByteToTime( const int& byte, const int& byteperSec )
{
	int time = byte / byteperSec;
	
	CString str;
	UINT hour	  = time / 3600;
	UINT minute = time % 3600 / 60;
	UINT second = time % 60;
	str.Format(_T("%02d:%02d:%02d"), hour, minute, second);
	return str;
}
