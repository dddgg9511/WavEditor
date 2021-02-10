
// PrototypeDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Audio.h"
#include "AudioDlg.h"
#include "afxdialogex.h"
#include "Sound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPrototypeDlg 대화 상자



CAudioDlg::CAudioDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAudioDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAudioDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_CUSTOM1, m_ctrl);
	DDX_Control(pDX, IDC_CUSTOM2, m_ctrl2);
	DDX_Control(pDX, IDC_CUSTOM3, m_ctrl3);
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CAudioDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CAudioDlg::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(CH_COPYDATA, &CAudioDlg::OnChDatacopy)
	ON_MESSAGE(CH_PASTEDATA, &CAudioDlg::OnChPastedata)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, &CAudioDlg::OnBnClickedCancel)
	ON_MESSAGE(CH_PLAY, &CAudioDlg::OnChPlay)
END_MESSAGE_MAP()


// CPrototypeDlg 메시지 처리기

BOOL CAudioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	ShowWindow(SW_SHOW);
	m_arrSoundCtrls[0] = &m_ctrl;
	m_arrSoundCtrls[1] = &m_ctrl2;
	m_arrSoundCtrls[2] = &m_ctrl3;
	for(int i = 0; i < 3; i++)
	{
		m_arrSoundCtrls[i]->SetParent(this);
		m_arrSoundCtrls[i]->CreateAudioThread();
	}

	
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CAudioDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CAudioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAudioDlg::OnBnClickedOk()
{


}


BOOL CAudioDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return CDialogEx::OnEraseBkgnd(pDC);
}


afx_msg LRESULT CAudioDlg::OnChDatacopy(WPARAM wParam, LPARAM lParam)
{
	if(m_copy)
	{
		delete m_copy;
	}
	m_copy = (COPYSOUND*)wParam;

	return 0;
}


afx_msg LRESULT CAudioDlg::OnChPastedata(WPARAM wParam, LPARAM lParam)
{
	if(!m_copy)
	{
		return 0;
	}
	SoundCtrl* temp = (SoundCtrl*)wParam;
	int pos = (int)lParam;
	temp->PasteData(m_copy, pos);
	return 0;
}


void CAudioDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if(m_copy)
	{
		delete m_copy;
	}
	// TODO: Add your message handler code here
}


void CAudioDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	for(int i = 0; i < 3; i++)
	{
		m_arrSoundCtrls[i]->TerminateAudioThread();
	}
	CDialogEx::OnCancel();
}


afx_msg LRESULT CAudioDlg::OnChPlay(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)wParam;
	for(int i = 0; i < 3; i++)
	{
		if(m_arrSoundCtrls[i]->m_hWnd != hwnd)
		{
			m_arrSoundCtrls[i]->PostMsg(PAUSE, NULL);
		}
	}
	return 0;
}


