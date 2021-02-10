
// AudioDlg.h : 헤더 파일
//

#pragma once
#include "SoundCtrl.h"
struct COPYSOUND;

// CAudioDlg 대화 상자
class CAudioDlg : public CDialogEx
{
// 생성입니다.
public:
	CAudioDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROTOTYPE_DIALOG };

	protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
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
	//SoundCtrl 배열
	SoundCtrl* m_arrSoundCtrls[3];
	//SoundCtrl
	SoundCtrl m_ctrl;
	//SoundCtrl
	SoundCtrl m_ctrl2;
	//SoundCtrl
	SoundCtrl m_ctrl3;
	//복사된 데이터
	COPYSOUND* m_copy;
};
