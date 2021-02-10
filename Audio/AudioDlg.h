
// AudioDlg.h : ��� ����
//

#pragma once
#include "SoundCtrl.h"
struct COPYSOUND;

// CAudioDlg ��ȭ ����
class CAudioDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CAudioDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PROTOTYPE_DIALOG };

	protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnChDatacopy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChPastedata(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnChPlay(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
public:
	//SoundCtrl �迭
	SoundCtrl* m_arrSoundCtrls[3];
	//SoundCtrl
	SoundCtrl m_ctrl;
	//SoundCtrl
	SoundCtrl m_ctrl2;
	//SoundCtrl
	SoundCtrl m_ctrl3;
	//����� ������
	COPYSOUND* m_copy;
};
