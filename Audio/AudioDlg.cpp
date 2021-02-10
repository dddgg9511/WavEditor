
// PrototypeDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Audio.h"
#include "AudioDlg.h"
#include "afxdialogex.h"
#include "Sound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPrototypeDlg ��ȭ ����



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


// CPrototypeDlg �޽��� ó����

BOOL CAudioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	ShowWindow(SW_SHOW);
	m_arrSoundCtrls[0] = &m_ctrl;
	m_arrSoundCtrls[1] = &m_ctrl2;
	m_arrSoundCtrls[2] = &m_ctrl3;
	for(int i = 0; i < 3; i++)
	{
		m_arrSoundCtrls[i]->SetParent(this);
		m_arrSoundCtrls[i]->CreateAudioThread();
	}

	
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CAudioDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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


