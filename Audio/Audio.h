
// Prototype.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CAudioApp:
// �� Ŭ������ ������ ���ؼ��� Audio.cpp�� �����Ͻʽÿ�.
//

class CAudioApp : public CWinApp
{
public:
	CAudioApp();
	ULONG_PTR						m_gditoken;	  		//GDI+ ��ü
	GdiplusStartupInput gpsi;			  		//GDI+ ��ü
// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CAudioApp theApp;