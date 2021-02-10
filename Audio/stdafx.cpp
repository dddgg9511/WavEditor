
// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// Prototype.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

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
