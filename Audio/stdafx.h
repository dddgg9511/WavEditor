
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����

#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdiplus")
#include <gdiplus.h>
using namespace Gdiplus;


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

/**
@Byte�� ��ǥ�� ��ȯ
@byte				: ��ȯ�� Byte
@bytePerSec : �ʴ� ���� byte ũ��
@timeStep		: �ð� ����
@return			: ��ȯ�� ��ǥ
*/
REAL ByteToCoordinate(const int& byte, const int& bytePerSec, const int& timeStep);
/**
@��ǥ�� Byte�� ��ȯ
@pos				: ��ȯ�� ��ǥ
@bytePerSec : �ʴ� ���� byte ũ��
@timestep		: �ð� ����
@return			: ��ȯ�� Byte
*/
int CoordinateToByte(const REAL& pos, const int& bytePerSec, const int& timeStep);
/**
@Byte�� �ð� ���ڿ��� ��ȯ
@byte				: ��ȯ�� ��ǥ
@bytePerSEc : �ʴ� ���� byte ũ��
@return			: ��ȯ�� �ð� ���ڿ�
*/
CString ByteToTime(const int& byte, const int& byteperSec);