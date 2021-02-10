// SoundCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Audio.h"
#include "SoundCtrl.h"
#include "Sound.h"

#include <iostream>
#include <cmath>
#include "Rectangle.h"
using namespace std;

// SoundCtrl

IMPLEMENT_DYNAMIC(SoundCtrl, CWnd)

 
short SoundCtrl::interpolation(const double& pos, short* arr)
{
	int prev = static_cast<int>(floor(pos));
	int next = static_cast<int>(ceil(pos));
	int size = static_cast<int>(_msize(arr));
	if(next > size / 2)
	{
		return prev;
	}
	double dVal = arr[prev] + (arr[next] - arr[prev]) * (pos - prev);
	short val = static_cast<short>(dVal);
	if(val > 10)
	{
		int a = 0;
	}
	return val;
}


SoundCtrl::SoundCtrl() 
	:	CWnd(),
		m_nCurByte(0),
		m_nTimeStep(4),
		m_btnWidth(45),
		m_pData(nullptr),
		m_nSize(0),
		m_fMargin(5.f),
		m_nTime(0),
		m_nCurPage(0),
		m_nMaxPage(0),
		m_left(nullptr),
		m_right(nullptr)
{
	memset(m_waveHdr, 0, sizeof(m_waveHdr));
	for(int i = 0; i < BUFFERCOUNT; i++)
	{
		m_buffer[i] = nullptr;
	}
}

SoundCtrl::~SoundCtrl()
{
	delete chkRegion;
	delete m_btnStop;
	delete m_btnPlay;
	delete m_btnCopy;
	delete m_btnPaste;
	delete m_btnDelete;
	delete m_btnSave;
	delete m_btnOpen;
	delete m_btnClose;
	delete m_play;
	delete m_start;
	delete m_end;
	delete staticTime;
	delete m_staticFileName;
	delete m_staticEndTime;
	delete m_staticPlayTime;
	if(m_pData)
	{
		delete m_pData;
	}
	if(m_left)
	{
		delete m_left;
	}
	if(m_right)
	{
		delete m_right;
	}
	delete m_bmpGraph;

	for(int i = 0; i < BUFFERCOUNT; i++)
	{
		delete[] m_buffer[i];
	}
	
	CloseHandle(m_hAudioEvent);
}


BEGIN_MESSAGE_MAP(SoundCtrl, CWnd)
	ON_WM_PAINT()
	ON_BN_CLICKED(1, &SoundCtrl::OnBnClickedBtnPlay)
	ON_BN_CLICKED(2, &SoundCtrl::OnBnClickedBtnStop)
	ON_BN_CLICKED(3, &SoundCtrl::OnBnClickedBtnCopy)
	ON_BN_CLICKED(4, &SoundCtrl::OnBnClickedBtnPaste)
	ON_BN_CLICKED(5, &SoundCtrl::OnBnClickedBtnDelete)
	ON_BN_CLICKED(6, &SoundCtrl::OnBnClickedBtnSave)
	ON_BN_CLICKED(7, &SoundCtrl::OnBnClickedBtnOpen)
	ON_BN_CLICKED(8, &SoundCtrl::OnBnClickedBtnClose)
	ON_BN_CLICKED(10, &SoundCtrl::OnBnClickedBtnCheck)
	
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_SOUND_SETPLAYPOSITION, &SoundCtrl::OnSoundSetplayposition)
	ON_COMMAND(ID_SOUND_SETSTARTPOSITION, &SoundCtrl::OnSoundSetstartposition)
	ON_COMMAND(ID_SOUND_SETENDPOSITION, &SoundCtrl::OnSoundSetendposition)
	ON_COMMAND(ID_SOUND_PASTEENDPOSITION, &SoundCtrl::OnSoundPasteendposition)
	ON_COMMAND(ID_SOUND_PASTESTARTPOSITION, &SoundCtrl::OnSoundPastestartposition)
END_MESSAGE_MAP()



// SoundCtrl message handlers


void CALLBACK WaveOutProc(HWAVEOUT hwo, UINT uMsg, 
													DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	SoundCtrl* pPlayer = (SoundCtrl*)dwInstance;
	switch(uMsg)
	{
	case WOM_DONE:
		pPlayer->SetAudioEvent();
		pPlayer->MovePosition();
		break;
	}
}


void SoundCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	GetClientRect(&m_clientRect);
	int clientWidth = m_clientRect.Width();
	
	CRect btnRect(clientWidth / 2 - 80, 15, clientWidth / 2 - 80 + m_btnWidth, 35);

	CRect chkRect(clientWidth / 2 - 80, 15, clientWidth / 2 - 80 + 80, 35);

	chkRegion = new CButton();
	chkRegion->Create(_T("Region"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
									 chkRect, this, 10);
	btnRect.left += 80;
	btnRect.right += 80;

	m_btnPlay = new CButton();
	m_btnPlay->Create(_T("▶"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, btnRect, 
										this, 1);
	btnRect.left += (m_btnWidth + 5);
	btnRect.right += (m_btnWidth + 5);

	m_btnStop = new CButton();
	m_btnStop->Create(_T("■"), WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON, btnRect, this, 2);
	btnRect.left += (m_btnWidth + 5);
	btnRect.right += (m_btnWidth + 5);

	m_btnCopy = new CButton();
	m_btnCopy->Create(_T("Copy"), WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON, btnRect, this, 3);
	btnRect.left += (m_btnWidth + 5);
	btnRect.right += (m_btnWidth + 5);
	m_btnPaste = new CButton(); 
	m_btnPaste->Create(_T("Paste"), WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON,btnRect, this, 4);
	btnRect.left += (m_btnWidth + 5);
	btnRect.right += (m_btnWidth + 5);

	m_btnDelete = new CButton();
	m_btnDelete->Create(_T("Delete"), WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON, btnRect, this, 5);
	btnRect.left += (m_btnWidth + 5);
	btnRect.right += (m_btnWidth + 5);

	m_btnSave = new CButton();
	m_btnSave->Create(_T("Save"), WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON, btnRect, this, 6);
	btnRect.left += (m_btnWidth + 5);
	btnRect.right += (m_btnWidth + 5);

	m_btnOpen = new CButton();
	m_btnOpen->Create(_T("Open"), WS_CHILD | WS_VISIBLE |
									BS_PUSHBUTTON, btnRect, this, 7);
	btnRect.left += (m_btnWidth + 5);
	btnRect.right += (m_btnWidth + 5);

	m_btnClose = new CButton();
	m_btnClose->Create(_T("Close"), WS_CHILD | WS_VISIBLE |
									 BS_PUSHBUTTON, btnRect, this, 8);

	staticTime = new CStatic();
	staticTime->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER,
									   CRect(0, 0, 0, 0), this);
	staticTime->ShowWindow(SW_HIDE);

	int top = btnRect.top - 10;
	int bottom = top + btnRect.Height();
	CRect staticRect(10, top, 210, bottom);
	m_staticFileName = new CStatic();
	m_staticFileName->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER,
												staticRect, this);
	m_staticFileName->ShowWindow(SW_HIDE);
	staticRect.right = staticRect.left + 90;
	staticRect.MoveToY(staticRect.Height() + 10);
	m_staticPlayTime = new CStatic();
	m_staticPlayTime->Create(_T("00:00:00"), WS_CHILD | WS_VISIBLE | SS_CENTER,
												staticRect, this);
	m_staticPlayTime->ShowWindow(SW_HIDE);
	staticRect.MoveToX(staticRect.Width() + 30);
	m_staticEndTime = new CStatic();
	m_staticEndTime->Create(_T("00:00:00"), WS_CHILD | WS_VISIBLE | SS_CENTER,
												staticRect, this);
	m_staticEndTime->ShowWindow(SW_HIDE);


	m_nWidth = m_clientRect.Width() - 20;

	REAL width = static_cast<REAL>(m_nWidth);
	m_rcfBtn = RectF(10, 0.f, width, 50.f);

	RectF StartPos = RectF(5.f, m_rcfBtn.GetBottom(), 10.f, 10.f);
	RectF EndPos = RectF(10.f + m_nWidth - 5, m_rcfBtn.GetBottom(), 10.f, 10.f);
	RectF PlayPos = RectF(5.f, StartPos.GetBottom(), 10.f, 10.f);

	m_start = new CRectangle(StartPos, Color(0, 0, 255));
	m_end	 = new CRectangle(EndPos, Color(0, 0, 255));
	m_play = new CRectangle(PlayPos, Color(255, 0, 0));

	REAL graphHeight = (m_clientRect.Height() - m_rcfBtn.Height - 20.f);
	REAL channelHeight = (graphHeight - m_fMargin * 3.f - 40.f) / 2.f; 
	m_rcfGraph = RectF(10.f, PlayPos.GetBottom(), width, graphHeight);


	m_rcfLchannel = RectF(10.f, m_rcfGraph.Y + m_fMargin + 20.f, width, channelHeight);
	m_rcfRchannel = RectF(10.f, m_rcfLchannel.GetBottom() + m_fMargin, width, 
												channelHeight);
	int y = static_cast<int>(StartPos.Y);
	m_rcDraw = CRect(0, y, m_clientRect.Width(), m_clientRect.Height());

	SetScrollRange(SB_HORZ, 0, 0, FALSE);

	CWnd::PreSubclassWindow();
}

BOOL SoundCtrl::RegisterWindowClass()
{
	WNDCLASS    sttClass;
	HINSTANCE   hInstance = AfxGetInstanceHandle();

	if( GetClassInfo( hInstance, _T("TESTCLASS"), &sttClass ) == FALSE )
	{
		sttClass.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		sttClass.lpfnWndProc      = ::DefWindowProc;
		sttClass.cbClsExtra       = 0;
		sttClass.cbWndExtra       = 0;
		sttClass.hInstance        = hInstance;
		sttClass.hIcon            = NULL;
		sttClass.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		sttClass.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		sttClass.lpszMenuName     = NULL;
		sttClass.lpszClassName    = _T("TESTCLASS");

		if( !AfxRegisterClass( &sttClass ) )
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
	return TRUE;
}

void SoundCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(&dc, m_clientRect.Width(), 
		m_clientRect.Height());
	CBitmap* pOldBitmap = MemDC.SelectObject(&memBitmap);
	Graphics mainG(MemDC.GetSafeHdc());

	SolidBrush background(Color(255,255,255));
	mainG.FillRectangle(&background, 0, 0, m_clientRect.Width(), m_clientRect.Height());

	SolidBrush Graph(Color(0,0,0));
	mainG.FillRectangle(&Graph,m_rcfGraph);

	if(m_bmpGraph)
	{
		Bitmap* bmp;
		int bmpWidth = m_bmpGraph->GetWidth();
		if(m_nWidth * (m_nCurPage + 1) >= bmpWidth)
		{
			bmp = m_bmpGraph->Clone(m_nWidth * m_nCurPage, 0, 
						bmpWidth - m_nWidth * m_nCurPage,
						m_bmpGraph->GetHeight(),PixelFormatDontCare);
		}
		else
		{
			bmp = m_bmpGraph->Clone(m_nWidth * m_nCurPage, 0, m_nWidth,
						m_bmpGraph->GetHeight(),PixelFormatDontCare);
		}
		REAL width = static_cast<REAL>(m_nWidth);
		mainG.DrawImage(bmp, m_rcfGraph, 0.f, 0.f, width, m_rcfGraph.Height, UnitPixel);
		REAL RegionWidth = m_end->GetPos() - m_start->GetPos();
		REAL RegionX = m_start->GetPos() + 5 - m_nCurPage * m_nWidth;
		SolidBrush back(Color(100,0,200,0));
		mainG.IntersectClip(m_rcfGraph);
		mainG.FillRectangle(&back, RegionX, m_rcfGraph.Y, RegionWidth, 
												m_rcfGraph.Height);
		mainG.ResetClip();
	
		m_start->Draw(m_nCurPage, m_nWidth, &mainG);
		m_end->Draw(m_nCurPage, m_nWidth, &mainG);
		m_play->Draw(m_nCurPage, m_nWidth, m_bmpGraph->GetHeight(), &mainG);
		delete bmp;
	}
	dc.BitBlt(0, 0, m_clientRect.Width(), m_clientRect.Height() ,&MemDC, 0, 0, 
						SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
	DeleteDC(MemDC);
	DeleteObject(memBitmap);
}




UINT SoundCtrl::AudioThread( LPVOID pDataInput )
{
	SoundCtrl* pPlayer = static_cast<SoundCtrl*>(pDataInput);
	while(TRUE)
	{
		//장치에 전달된 Data가 재생이 완료될 때 까지 대기
		WaitForSingleObject(pPlayer->m_hAudioEvent, INFINITE);

		for(UINT i = 0; i < BUFFERCOUNT; i++)  
		{
			UINT id = pPlayer->AudioProc();
			if(id == -1)
			{
				return 0;
			}
			else if( id == 1)
			{
				break;
			}
			if(!pPlayer->m_bIsStoped && pPlayer->m_waveHdr[i].dwFlags & WHDR_DONE)
			{
				UINT reByte = pPlayer->BufferSeting(i);
				waveOutWrite(pPlayer->m_hWaveOut, &pPlayer->m_waveHdr[i], sizeof(WAVEHDR));
				if(reByte == 0)
				{
					pPlayer->Stop();
				}
			}
		}
	}
	return 0;
}

UINT SoundCtrl::BufferSeting( const UINT& Idx )
{
	const int bytePerSec = m_WaveFormatEx.nAvgBytesPerSec;
	int byte = m_nBufferSize;
	m_waveHdr[Idx].dwFlags &= ~WHDR_DONE;
	//BufferSize가 남은 Byte보다 큰 경우
	int size = m_nSize;
	if(chkRegion->GetCheck())
	{
		size = m_end->GetByte();
	}
	if(size - m_nCurByte < byte)
	{
		byte = (size - m_nCurByte);

		memcpy(m_buffer[Idx], m_pData + m_nCurByte, byte);
		m_nCurByte += byte;
		m_nTime		 += byte;
	}
	else
	{
		memcpy(m_buffer[Idx], m_pData + m_nCurByte, byte);
		m_nCurByte += m_nBufferSize;
		m_nTime		 += m_nBufferSize;
		byte = 0;
	}
	
	m_waveHdr[Idx].lpData					= m_buffer[Idx];
	m_waveHdr[Idx].dwBufferLength = m_nBufferSize - byte;

	return size - m_nCurByte;
}

void SoundCtrl::MovePosition()
{
	const int bytePerSec = m_WaveFormatEx.nAvgBytesPerSec;

	if(!m_play->GetClickState() /*&& m_nTime > bytePerSec / m_nTimeStep*/)
	{
		int byte = m_play->GetByte();
		byte += m_nBufferSize;
		CString str = ByteToTime(byte, m_WaveFormatEx.nAvgBytesPerSec);
		m_staticPlayTime->SetWindowText(str);
		REAL pos = ByteToCoordinate(byte, bytePerSec, m_nTimeStep);
		m_play->SetPos(byte, pos);
		int page = m_play->GetPage();
		m_nTime = 0;
		if(pos > (page + 1) * m_nWidth)
		{
			if(m_nMaxPage >= page + 1)
			{
				m_play->SetPage(++page);
				if(!m_start->GetClickState() && !m_end->GetClickState())
				{
					m_nCurPage = page;
					SetScrollPos(SB_HORZ, m_nCurPage, TRUE);
				}
			}
		}
		InvalidateRect(m_rcDraw, TRUE);
	} 
}

UINT SoundCtrl::AudioProc()
{
	if(m_queue.empty())
	{
		return 0;
	}
	int flag = 0;
	Message* msg = m_queue.front();
	m_queue.pop();

	switch (msg->m_ID)
	{
	case PLAYPAUSE:
		{
			PlayAndPause();
			break;
		}
	case STOP:
		{
			Stop();
			flag = 1;
			break;
		}
	
	case EXIT:
		{
			flag = -1;
			break;
		}
	case DEL:
		{
			DeleteData();
			break;
		}
	case SETPOSITION:
		{
			int pos = *(int*)msg->m_data;
			setPosition(pos);
			delete msg->m_data;
			flag = 1;
			break;
		}
	case SETDATA:
		{
			char* data = (char*)msg->m_data;
			SetData(data);
			delete data;
			flag = 1;
			break;
		}
	case SETSOUND:
		{
			SOUND* sound = (SOUND*)msg->m_data;
			SetSound(sound);
			delete sound;
			flag = 1;
			break;
		}
	case PAUSE:
		{
			Pause();
			flag = 1;
			break;
		}
	case CLOSE:
		{
			Close();
			flag = 1;
			break;
		}
	default:
		break;
	}
	delete msg;
	return flag;
}

void SoundCtrl::PlayAndPause()
{
	if(m_bIsStoped)
	{
		Play(); 
	}
	else
	{
		Pause();
	}
}

void SoundCtrl::Stop()
{
	
	waveOutReset(m_hWaveOut);
	SetPlayState(TRUE);
	if(m_pData)
	{
		for(int i = 0; i < BUFFERCOUNT; i++)
		{
			memset(m_buffer[i], 0, _msize(m_buffer[i]));
		}
	}
	m_nCurByte = 0;
	if(chkRegion->GetCheck())
	{
		m_nCurByte = m_start->GetByte();
		if(m_nCurByte % 2 != 0)
		{
			m_nCurByte++;
		}
		m_play->SetPos(m_nCurByte, m_start->GetPos());
		m_nCurPage = m_start->GetPage();
	}
	else
	{
		m_nCurByte = 0;
		m_play->SetPos(0, 5);
		m_nCurPage = m_start->GetPage();
	}
	m_play->SetPage(m_nCurPage);
	
	SetScrollPos(SB_HORZ, 0, TRUE);

	
	InvalidateRect(m_rcDraw, FALSE);
}

void SoundCtrl::Play()
{
	if(!m_pData)
	{
		return;
	}
	if(!m_hWaveOut)
	{
		OpenWave();
	}
	::SendMessage(m_pParent->m_hWnd, CH_PLAY, WPARAM(this->m_hWnd), NULL);
	SetPlayState(FALSE);
	SetEvent(m_hAudioEvent);
}

void SoundCtrl::Pause()
{
	if(!m_pData)
	{
		return;
	}
	SetPlayState(TRUE);
}

void SoundCtrl::OpenWave()
{
	if(!m_pData)
	{
		return;
	}
	waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &m_WaveFormatEx, 
						  (DWORD)WaveOutProc, (DWORD)this, CALLBACK_FUNCTION);
	
	waveOutPause(m_hWaveOut);
	SetPlayState(TRUE);
	
	for(UINT Index = 0; Index < BUFFERCOUNT; Index++)
	{
		m_waveHdr[Index].dwBufferLength = m_nBufferSize;
		m_waveHdr[Index].lpData					= m_buffer[Index]; 

		waveOutPrepareHeader(m_hWaveOut, &m_waveHdr[Index], sizeof(WAVEHDR));
		BufferSeting(Index);
		waveOutWrite(m_hWaveOut, &m_waveHdr[Index], sizeof(WAVEHDR));
	}
	
}

void SoundCtrl::SetSound( SOUND* pSound )
{
	if(m_hWaveOut)
	{
		SetPlayState(TRUE);
		waveOutReset(m_hWaveOut);
		CloseWave();
	}
	m_WaveFormatEx = pSound->m_WaveFormatEx;
	m_strFileName = pSound->m_strFileName;
	m_staticFileName->SetWindowText(m_strFileName);
	m_staticFileName->ShowWindow(SW_SHOW);
	SetData(pSound->m_pWaveData);
	OpenWave();
}

void SoundCtrl::SetAudioEvent()
{
	SetEvent(m_hAudioEvent);
}
void SoundCtrl::OnBnClickedBtnPlay()
{
	PlayAndPause();
}


void SoundCtrl::OnBnClickedBtnSave()
{
	if(!m_pData)
	{
		return;
	}
	static TCHAR BASED_CODE szFilter[] = _T("오디오 파일(*.WAV, *.wav) | *.WAV;*.wav |");
	CString File;
	CString strFileList;
	CFileDialog dlg(FALSE, _T("*.wav"), _T("Wav"),
		OFN_FILEMUSTEXIST, szFilter);
	
	if(dlg.DoModal() == IDOK)
	{
		CString fileName = dlg.GetPathName();

		// Save .WAV file
		FILE* file;
		_tfopen_s(&file, fileName,_T("wb"));
		if (file != NULL)
		{
			int size = m_nSize + 36;
			int chunksize = 16;
			fwrite("RIFF", 4, 1, file);
			fwrite(&size, 4, 1, file);
			fwrite("WAVE", 4, 1, file);
			fwrite("fmt ", 4, 1, file);
			fwrite(&chunksize, 4, 1, file);
			int audioFormat = 1;
			fwrite(&audioFormat, 2, 1, file);
			fwrite(&m_WaveFormatEx.nChannels, 2, 1, file);
			fwrite(&m_WaveFormatEx.nSamplesPerSec, 4, 1, file);
			fwrite(&m_WaveFormatEx.nAvgBytesPerSec, 4, 1, file);
			fwrite(&m_WaveFormatEx.nBlockAlign, 2, 1, file);
			fwrite(&m_WaveFormatEx.wBitsPerSample, 2, 1, file);
			fwrite("data", 4, 1, file);
			fwrite(&m_nSize, 4, 1, file);
			fwrite(m_pData, m_nSize, 1, file);
			fclose(file);
		}
	}
}

void SoundCtrl::OnBnClickedBtnOpen()
{
	static TCHAR BASED_CODE szFilter[] = _T("오디오 파일(*.WAV, *.wav) | *.WAV;*.wav |");
	CString File;
	CString strFileList;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		CString File = dlg.GetPathName();
		SOUND* sound = new SOUND(File);
		if(sound->m_WaveFormatEx.wBitsPerSample == 8)
		{
			Pcm8To16(sound);
		}
		PostMsg(SETSOUND,(void*)sound);
		SetAudioEvent();
	}
}
void SoundCtrl::OnBnClickedBtnCheck()
{
	if(chkRegion->GetCheck())
	{
		if(m_play->GetPos() > m_end->GetPos() ||
			 m_play->GetPos() < m_start->GetPos())
		{
			PostMsg(STOP, NULL);
		}
	}
}
void SoundCtrl::OnBnClickedBtnClose()
{
	PostMsg(CLOSE, NULL);
}

void SoundCtrl::Close()
{
	Stop();
	CloseWave();
	SetPlayState(TRUE);
	if(m_pData)
	{
		delete m_pData;
		delete m_left;
		m_left = nullptr;
		delete m_right;
		m_right = nullptr;
		delete m_bmpGraph;
		m_pData = nullptr;
		m_nSize = 0;
		m_bmpGraph = nullptr;
	}
	m_strFileName = _T("");
	m_staticFileName->SetWindowText(_T(""));
	m_staticFileName->ShowWindow(SW_HIDE);
  m_staticEndTime->SetWindowText(_T("00:00:00"));
	m_staticEndTime->ShowWindow(SW_HIDE);
  m_staticPlayTime->SetWindowText(_T("00:00:00"));
	m_staticPlayTime->ShowWindow(SW_HIDE);
	InvalidateRect(m_rcDraw, FALSE);
}
void SoundCtrl::OnBnClickedBtnCopy()
{
	if(!m_pData)
	{
		return;
	}
	int ch = 1;
	short* leftCh = nullptr;
	short* rightCh = nullptr;
	int size = (m_end->GetByte() - m_start->GetByte());
	if(m_WaveFormatEx.nChannels == 2)
	{
		ch = 2;
		size /= ch;
		rightCh = new short[size / 2];
		memcpy(rightCh, m_right + (m_start->GetByte() / ch) / sizeof(short), size);
	}
	leftCh = new short[size / 2];
	memcpy(leftCh, m_left + (m_start->GetByte() / ch) / sizeof(short), size);
	COPYSOUND* copy = new COPYSOUND(m_WaveFormatEx, leftCh, rightCh);
	::SendMessage(m_pParent->m_hWnd, CH_COPYDATA, WPARAM(copy), LPARAM(NULL));
}

void SoundCtrl::OnBnClickedBtnPaste()
{
	int pos = m_start->GetByte();
	::SendMessage(m_pParent->m_hWnd,CH_PASTEDATA,WPARAM(this), LPARAM(pos));
}

void SoundCtrl::OnBnClickedBtnDelete()
{
	PostMsg(DEL,NULL);
}

void SoundCtrl::OnBnClickedBtnStop()
{
	PostMsg(STOP, NULL);
}

void SoundCtrl::CloseWave()
{
	for(UINT i = 0; i < BUFFERCOUNT; i++)
	{
		waveOutUnprepareHeader(m_hWaveOut,&m_waveHdr[i],sizeof(WAVEHDR));
	}
	waveOutClose(m_hWaveOut);
	m_hWaveOut = nullptr;
}

void SoundCtrl::PostMsg( const UINT msgID, void* data )
{
	Message* msg = new Message(msgID, data);
	m_queue.push(msg);
	SetEvent(m_hAudioEvent);
}

void SoundCtrl::SetData( char* data )
{
	EnableButton(FALSE);
	if(m_pData)
	{
		delete m_pData;
	}
	m_nSize = _msize(data);
	m_pData = new char[m_nSize];
	m_nBufferSize = m_WaveFormatEx.nSamplesPerSec / 5 * m_WaveFormatEx.nChannels;
	for(int i = 0; i < BUFFERCOUNT; i++)
	{
		if(m_buffer[i])
		{
			delete m_buffer[i];
		}
		m_buffer[i] = new char[m_nBufferSize];
		memset(m_buffer[i], 0, _msize(m_buffer[i]));
	}
	
	memcpy(m_pData, data, m_nSize);

	Sample();
	REAL pos = ByteToCoordinate(m_nSize, m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep);
	m_start->SetPos(0, 5);
	m_start->SetPage(0);
	m_end->SetPos(m_nSize, pos);
	Stop();
	m_nCurPage = 0;
	int pageSize = (m_nWidth * m_WaveFormatEx.nAvgBytesPerSec / m_nTimeStep);
	m_nMaxPage = (m_nSize) / pageSize + 1;
	SetScrollRange(SB_HORZ, 0, m_nMaxPage - 1, FALSE);
	m_end->SetPage(m_nMaxPage - 1);
	CString str = ByteToTime(m_nSize, m_WaveFormatEx.nAvgBytesPerSec);
	m_staticEndTime->SetWindowText(str);
	m_staticEndTime->ShowWindow(SW_SHOW);
	m_staticPlayTime->SetWindowText(_T("00:00:00"));
	m_staticPlayTime->ShowWindow(SW_SHOW);
	EnableButton(TRUE);
	Invalidate(TRUE);
}

void SoundCtrl::DeleteData()
{
	if(!m_pData)
	{
		return;
	}
	int endByte = m_end->GetByte();
	int startByte = m_start->GetByte();
	UINT size = m_nSize - (endByte - startByte);
	char* temp = new char[size];
	memcpy(temp, m_pData, startByte);
	memcpy(temp + startByte, m_pData + endByte, m_nSize - endByte);
	
	PostMsg(SETDATA,(void*)temp);
	if(size == 0)
	{
		waveOutReset(m_hWaveOut);
		CloseWave();
	}
	Stop();
}


void SoundCtrl::Sample()
{
	if(m_WaveFormatEx.nChannels == 1)
	{
		OneChannel();
	}
	else if(m_WaveFormatEx.nChannels ==  2)
	{
		TwoChannel();
	}
}

void SoundCtrl::DrawOneChannel()
{
	std::vector<REAL> lMin;
	std::vector<REAL> lMax;

	const int size = _msize(m_left);
	const int sampleSize = m_nSize / m_WaveFormatEx.nAvgBytesPerSec * m_nTimeStep;

	lMin.reserve(sampleSize);
	lMax.reserve(sampleSize);
	
	const int step = m_WaveFormatEx.nSamplesPerSec / m_nTimeStep;
	for(int i = 0; i <= size / 2 - step; i+= step)
	{
		long double avgLeft = m_left[i];
		short leftmaxv = SHORT_MIN; 
		short leftminv = SHORT_MAX; 
		for(int j = i; j < i + step; j++)
		{
			if(m_left[j] > leftmaxv) 
			{ 
				leftmaxv = m_left[j];
			} 
			if(m_left[j] < leftminv)
			{ 
				leftminv = m_left[j];
			} 
		}
		lMin.push_back(static_cast<REAL>(leftminv) / SHORT_MAX);
		lMax.push_back(static_cast<REAL>(leftmaxv) / SHORT_MAX);
	}

	if(m_bmpGraph)
	{
		delete m_bmpGraph;
	}
	int Width = m_nSize / m_WaveFormatEx.nAvgBytesPerSec * m_nTimeStep;
	m_bmpGraph = new Bitmap(Width, static_cast<int>(m_rcfGraph.Height));
	Graphics g(m_bmpGraph);
	Pen pen(Color(0,200,0),1);

	REAL hh = m_rcfLchannel.Height / 2.f;

	SolidBrush brs(Color(0, 0, 0));
	REAL lChannelY = m_rcfLchannel.Y - m_rcfGraph.Y;
	g.FillRectangle(&brs, 0,0, m_bmpGraph->GetWidth(), m_bmpGraph->GetHeight());

	for(int i = 0; i < sampleSize; i++)
	{
		PointF bottom = PointF(static_cast<REAL>(i), lChannelY + hh - lMin[i] * hh);
		PointF top = PointF(static_cast<REAL>(i), lChannelY + hh - lMax[i] * hh);
		g.DrawLine(&pen, bottom, top);
	}
	Pen linePen(Color(200, 200, 200), 1);
	int bmpWidth = m_bmpGraph->GetWidth();
	for(int i = 0; i < bmpWidth / m_nTimeStep; i+= 1)
	{
		REAL X = static_cast<REAL>(i * m_nTimeStep);
		if(i % 5 * m_nTimeStep == 0)
		{
			g.DrawLine(&linePen, PointF(X, 0), PointF(X, 20));
		}
		else
		{
			g.DrawLine(&linePen, PointF(X, 0), PointF(X, 10));
		}
	}
}

void SoundCtrl::DrawTwoChannel()
{
	std::vector<REAL> lMin;
	std::vector<REAL> lMax;
	std::vector<REAL> rMin;
	std::vector<REAL> rMax;
	const int size = _msize(m_left);
	const int sampleSize = m_nSize / m_WaveFormatEx.nAvgBytesPerSec * m_nTimeStep;

	lMin.reserve(sampleSize);
	lMax.reserve(sampleSize);
	rMin.reserve(sampleSize);
	rMax.reserve(sampleSize);

	const int step = m_WaveFormatEx.nSamplesPerSec / m_nTimeStep;
	for(int i = 0; i < size / 2 - step; i+= step)
	{
		short leftmaxv = SHORT_MIN; 
		short leftminv = SHORT_MAX; 
		short rightmaxv = SHORT_MIN; 
		short rightminv = SHORT_MAX;
		for(int j = i; j < i + step; j++)
		{
			if(m_left[j] > leftmaxv) 
			{ 
				leftmaxv = m_left[j];
			} 
			if(m_left[j] < leftminv)
			{ 
				leftminv = m_left[j];
			} 
			if(m_right[j] > rightmaxv)
			{
				rightmaxv = m_right[j];
			}
			if(m_right[j] < rightminv)
			{
				rightminv = m_right[j];
			}
		}
		lMin.push_back(static_cast<REAL>(leftminv) / SHORT_MAX);
		lMax.push_back(static_cast<REAL>(leftmaxv) / SHORT_MAX);
		rMin.push_back(static_cast<REAL>(rightminv) / SHORT_MAX);
		rMax.push_back(static_cast<REAL>(rightmaxv) / SHORT_MAX);
	}

	if(m_bmpGraph)
	{
		delete m_bmpGraph;
	}
	int Width = m_nSize / m_WaveFormatEx.nAvgBytesPerSec * m_nTimeStep;
	m_bmpGraph = new Bitmap(Width, static_cast<int>(m_rcfGraph.Height));
	Graphics g(m_bmpGraph);
	Pen pen(Color(0,200,0),1);

	REAL hh = m_rcfLchannel.Height / 2.f;

	SolidBrush brs(Color(0, 0, 0));
	REAL lChannelY = m_rcfLchannel.Y - m_rcfGraph.Y;
	REAL rChannelY = m_rcfRchannel.Y - m_rcfGraph.Y;
	g.FillRectangle(&brs, 0,0, m_bmpGraph->GetWidth(), m_bmpGraph->GetHeight());

	for(int i = 0; i < sampleSize; i++)
	{
		REAL f = static_cast<REAL>(i);
		PointF bottom = PointF(f, lChannelY + hh - lMin[i] * hh);
		PointF top = PointF(f, lChannelY + hh - lMax[i] * hh);
		g.DrawLine(&pen, bottom, top);
		bottom = PointF(f, rChannelY + hh - rMin[i] * hh);
		top = PointF(f, rChannelY + hh - rMax[i] * hh);
		g.DrawLine(&pen, bottom, top);
	}
	Pen linePen(Color(200, 200, 200), 1);
	int bmpWidth = m_bmpGraph->GetWidth();
	for(int i = 0; i < bmpWidth / m_nTimeStep; i+= 1)
	{
		REAL X = static_cast<REAL>(i * m_nTimeStep);
		if(i % 5 * m_nTimeStep == 0)
		{
			g.DrawLine(&linePen, PointF(X, 0), PointF(X, 20));
		}
		else
		{
			g.DrawLine(&linePen, PointF(X, 0), PointF(X, 10));
		}
	}
}


void SoundCtrl::OneChannel()
{
	if(m_left)
	{
		delete m_left;
	}
	m_left = new short[m_nSize / 2];
	int number = 0;
	for(int i = 0; i < m_nSize / 2; i++, number++)
	{
		m_left[number] = ((short*)m_pData)[i];
	}
	DrawOneChannel();
}


void SoundCtrl::TwoChannel()
{
	if(m_left)
	{
		delete m_left;
	}
	if(m_right)
	{
		delete m_right;
	}
	m_left = new short[m_nSize / 4];
	m_right = new short[m_nSize / 4];
	int number = 0;
	for(int i = 0; i < m_nSize / 2; i++, number++)
	{
		m_left[number] = ((short*)m_pData)[i++];
		m_right[number] = ((short*)m_pData)[i];
	}
	DrawTwoChannel();
}


Message::Message( const int& id, void* data )
	:	m_ID(id),
	m_data(data)
{
	
}


void SoundCtrl::OnSize(UINT nType, int cx, int cy)
{
	
	
	// TODO: Add your message handler code here
}

void SoundCtrl::SetTimeStep( const UINT& step )
{
	//m_nCurPage = (m_nCurPage * step * (m_nWidth + 1)) / (m_nTimeStep * m_nWidth);
	
	m_nTimeStep = step;
	if(m_WaveFormatEx.nChannels == 1)
	{
		DrawOneChannel();
	}
	else if(m_WaveFormatEx.nChannels == 2)
	{
		DrawTwoChannel();
	}
	int pageSize = (m_nWidth * m_WaveFormatEx.nAvgBytesPerSec / m_nTimeStep);
	m_nMaxPage = (m_nSize) / pageSize + 1;
	SetScrollRange(SB_HORZ, 0, m_nMaxPage - 1, FALSE);
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS;
	scrollInfo.nPos = m_nCurPage;
	scrollInfo.nMax = m_nMaxPage;
	SetScrollInfo( SB_HORZ, &scrollInfo);
	m_start->PosSetUp(m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep);
	m_start->SetPage(m_start->GetByte() / pageSize);
	m_end->PosSetUp(m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep);
	m_end->SetPage(m_end->GetByte() / pageSize);

	m_play->PosSetUp(m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep);
	m_play->SetPage(m_play->GetByte() / pageSize);
	m_nCurPage = m_play->GetPage();
	SetScrollPos(SB_HORZ, m_nCurPage, TRUE);
	InvalidateRect(m_rcDraw, FALSE);
}


void SoundCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	::SetCapture(this->GetSafeHwnd());

	if(m_start->Clicked(point, m_nCurPage, m_nWidth))
	{
		staticTime->ShowWindow(SW_SHOW);
	}
	if(m_end->Clicked(point, m_nCurPage, m_nWidth))
	{
		staticTime->ShowWindow(SW_SHOW);
	}
	if(m_play->Clicked(point, m_nCurPage, m_nWidth))
	{
		staticTime->ShowWindow(SW_SHOW);
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void SoundCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();
	if(m_start->GetClickState())
	{
		m_start->ByteSetUp(m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep, m_nSize);
		m_start->SetPage(m_nCurPage);
		if(chkRegion->GetCheck() && m_start->GetPos() > m_play->GetPos())
		{
			PostMsg(STOP, NULL);
		}
		
		m_start->SetClickState();
	}
	if(m_end->GetClickState())
	{
		m_end->ByteSetUp(m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep, m_nSize);
		m_end->SetPage(m_nCurPage);
		if(chkRegion->GetCheck() && m_end->GetPos() < m_play->GetPos())
		{
			PostMsg(STOP, NULL);
		}
		m_end->SetClickState();
	}
	if(m_play->GetClickState())
	{
		if(chkRegion->GetCheck() && (m_play->GetPos() > m_end->GetPos() ||
			 m_play->GetPos() < m_start->GetPos()))
		{
			PostMsg(STOP, NULL);
		}
		else
		{
			m_play->ByteSetUp(m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep, m_nSize);
			m_play->SetPage(m_nCurPage);
			int* byte = new int(m_play->GetByte());
			CString str = ByteToTime(*byte, m_WaveFormatEx.nAvgBytesPerSec);
			m_staticPlayTime->SetWindowText(str);
			waveOutReset(m_hWaveOut);
			waveOutPause(m_hWaveOut);
			PostMsg(SETPOSITION, byte);
			SetAudioEvent();
		}
		m_play->SetClickState();
	}
	staticTime->ShowWindow(SW_HIDE);
	CWnd::OnLButtonUp(nFlags, point);
}


void SoundCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	if(m_start->GetClickState())
	{
		REAL maxPos = m_end->GetPos() - 15.f;
		m_start->Move(point, m_nWidth, maxPos, 5.f, m_nCurPage);
		
		SetScrollPos(SB_HORZ, m_nCurPage, TRUE);
		m_nCurPage = m_start->GetPage();
		REAL pos = m_start->GetPos();
		int byte = CoordinateToByte(pos,m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep);
		CString str = ByteToTime(byte, m_WaveFormatEx.nAvgBytesPerSec);
		
		int xPos = point.x;
		int diff = m_clientRect.right - (xPos + 120);
		if(diff < 0)
		{
			xPos += diff;
		}
		staticTime->MoveWindow(xPos, static_cast<int>(m_rcfBtn.Y), 120, 20);
		staticTime->SetWindowText(str);
		int X = static_cast<int>(pos) - m_nCurPage * m_nWidth;
		CPoint pt(X, point.y);
		ClientToScreen(&pt);
		SetCursorPos(pt.x + 5, pt.y);
		InvalidateRect(m_rcDraw, FALSE);
	}
	else if(m_end->GetClickState())
	{
		REAL maxPos = ByteToCoordinate(m_nSize, m_WaveFormatEx.nAvgBytesPerSec, 
																	 m_nTimeStep);
		REAL minPos = m_start->GetPos() + 5;
		m_end->Move(point, m_nWidth, maxPos, minPos, m_nCurPage);
		
		SetScrollPos(SB_HORZ, m_nCurPage, TRUE);
		m_nCurPage = m_end->GetPage();

		REAL pos = m_end->GetPos();
		int byte = CoordinateToByte(pos,m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep);
		CString str = ByteToTime(byte, m_WaveFormatEx.nAvgBytesPerSec);

		int xPos = point.x;
		int diff = m_clientRect.right - (xPos + 120);
		if(diff < 0)
		{
			xPos += diff;
		}
		staticTime->MoveWindow(xPos, static_cast<int>(m_rcfBtn.Y), 120, 20);
		staticTime->SetWindowText(str);
		int X = static_cast<int>(m_end->GetPos()) - m_nCurPage * m_nWidth;
		CPoint pt(X, point.y);
		ClientToScreen(&pt);
		SetCursorPos(pt.x + 5, pt.y);
		InvalidateRect(m_rcDraw, FALSE);
	}
	else if(m_play->GetClickState())
	{
		REAL maxPos = ByteToCoordinate(m_nSize, m_WaveFormatEx.nAvgBytesPerSec, 
																	 m_nTimeStep);
		m_play->Move(point, m_nWidth, maxPos - 5, 5.f, m_nCurPage);
		 
		SetScrollPos(SB_HORZ, m_nCurPage, TRUE);
		m_nCurPage = m_play->GetPage();
		REAL pos = m_play->GetPos();
		int byte = CoordinateToByte(pos,m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep);
		CString str = ByteToTime(byte, m_WaveFormatEx.nAvgBytesPerSec);
		int xPos = point.x;
		int diff = m_clientRect.right - (xPos + 120);
		if(diff < 0)
		{
			xPos += diff;
		}
		staticTime->MoveWindow(xPos, static_cast<int>(m_rcfBtn.Y), 120, 20);
		staticTime->SetWindowText(str);
		int X = static_cast<int>(m_play->GetPos()) - m_nCurPage * m_nWidth;
		CPoint pt(X, point.y);
		ClientToScreen(&pt);
		SetCursorPos(pt.x + 5, pt.y);
		InvalidateRect(m_rcDraw, FALSE);
	}
	CWnd::OnMouseMove(nFlags, point);
}

void SoundCtrl::setPosition( const int& val )
{
	waveOutReset(m_hWaveOut);
	int pos = val;
	if(pos % 2 != 0)
	{
		pos++;
	}
	m_nCurByte = pos;
	m_nTime = 0;
	if(m_bIsStoped)
	{
		waveOutPause(m_hWaveOut);
	}
}

BOOL SoundCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	return CWnd::OnEraseBkgnd(pDC);
}


void SoundCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	GetScrollInfo(SB_HORZ,&scrollInfo);

	int fixedValue = 0;

	switch(nSBCode)
	{
	case SB_LINEDOWN:
		if(m_nCurPage < m_nMaxPage - 1)
		{
			m_nCurPage++;
		}
		break;

	case SB_LINEUP:
		if(m_nCurPage > 0)
		{
			m_nCurPage--;
		}
		break;

	case SB_THUMBTRACK:
	case SB_THUMBPOSITION :
		{
			if (nSBCode == SB_THUMBPOSITION)
				m_nCurPage = scrollInfo.nPos;
			else
				m_nCurPage = scrollInfo.nTrackPos;
			break;
		}
		//채널을 클릭한 경우
	case TB_PAGEUP:
	case TB_PAGEDOWN:
		{
			CPoint point;
			::GetCursorPos(&point);
			::ScreenToClient(this->m_hWnd,&point);
			
			int width = m_clientRect.Width();
			int pageSize = width / (m_nMaxPage);
			scrollInfo.nPage = pageSize;
			m_nCurPage = point.x / pageSize;
			break;
		}
	case SB_ENDSCROLL:
		return;
	}
	
	scrollInfo.fMask = SIF_POS;
	scrollInfo.nPos = m_nCurPage;
	SetScrollInfo( SB_HORZ, &scrollInfo);
	InvalidateRect(m_rcDraw, FALSE);
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

short* SoundCtrl::ConvertData( const WAVEFORMATEX& format, short* src )
{
	short *dst = nullptr;
	if(m_WaveFormatEx.nAvgBytesPerSec == format.nAvgBytesPerSec)
	{
		int size = _msize(src);
		dst = new short[size / 2];
		memcpy(dst, src, size);
		
		return dst;
	}
	int test =  _msize(src);

	int leftSize;
	int size;
	leftSize = _msize(src);
	size = leftSize / format.nAvgBytesPerSec * format.nChannels;
	size *= m_WaveFormatEx.nAvgBytesPerSec / m_WaveFormatEx.nChannels / 2;
	double rate = static_cast<double>(format.nSamplesPerSec) / 
								static_cast<double>(m_WaveFormatEx.nSamplesPerSec);
	short* left = new short[size];
	int num = 0;
	for(double i = 0.f; i < leftSize; i += rate)
	{
		short leftval = interpolation(i, src);
		if(num == size)
		{
			break;
		}
		if(leftval != 0)
		{
			int a = 0;
		}
		left[num++] = leftval;
	}
	dst = new short[size];
	memcpy(dst, left, size * 2);
	delete[] left;
	return dst;
}

void SoundCtrl::PasteData( COPYSOUND* sound, const int& position )
{
	EnableButton(FALSE);
	short* left = nullptr;
	short* right = nullptr;
	char* temp;
	char* copy;
	int size;

	if(!m_pData)
	{
		m_WaveFormatEx = sound->m_WaveFormatEx;
	}
	left = ConvertData(sound->m_WaveFormatEx,sound->m_leftChannel);
	if(m_WaveFormatEx.nChannels == sound->m_WaveFormatEx.nChannels)
	{
		if(m_WaveFormatEx.nChannels == 1)
		{
			if(m_left)
			{ 
				delete m_left;
			}
			m_left = left;
			size = _msize(left);
			copy = new char[size];
			int num = 0;
			for(int i = 0; i < size / 2; i++)
			{
				copy[num++] = (char)(left[i] & 0xff);
				copy[num++] = (char)((left[i] >> 8) & 0xff);
			}
			temp = new char[m_nSize + size];
		}
		else
		{
			right = ConvertData(sound->m_WaveFormatEx,sound->m_rightChannel);
			if(m_left)
			{
				delete m_left;
			}
			if(m_right)
			{
				delete m_right;
			}
			m_left = left;
			m_right = right;
			size = _msize(left);
			copy = new char[size * 4];
 			int num = 0;
			for(int i = 0; i < size / 2; i++)
			{
				copy[num++] = (char)(left[i] & 0xff);
				copy[num++] = (char)((left[i] >> 8) & 0xff);
				copy[num++] = (char)(right[i] & 0xff);
				copy[num++] = (char)((right[i] >> 8) & 0xff);
			}
			size *= 2;
			temp = new char[m_nSize + size];
		}
	}
	else
	{
		if(m_WaveFormatEx.nChannels == 1)
		{
			if(m_left)
			{
				delete m_left;
			}
			m_left = left;
			size = _msize(left);
			copy = new char[size];
			int num = 0;
			for(int i = 0; i < size / 2; i++)
			{
				copy[num++] = (char)(left[i] & 0xff);
				copy[num++] = (char)((left[i] >> 8) & 0xff);
			}
			temp = new char[m_nSize + size];
		}
		else
		{
			if(m_left)
			{
				delete m_left;
			}
			if(m_right)
			{
				delete m_right;
			}
			m_left = left;
			right = ConvertData(sound->m_WaveFormatEx,sound->m_leftChannel);
			m_right = right;
			size = _msize(left);
		
			copy = new char[size * 4];
			int num = 0;
			for(int i = 0; i < size / 2; i++)
			{
				copy[num++] = (char)(left[i] & 0xff);
				copy[num++] = (char)((left[i] >> 8) & 0xff);
				copy[num++] = (char)(left[i] & 0xff);
				copy[num++] = (char)((left[i] >> 8) & 0xff);
			}
			size *= 2;
			temp = new char[m_nSize + size];
		}
	}
	int startPos = m_start->GetByte();
	if(m_pData == nullptr)
	{
		memcpy(temp + startPos, copy, size);
	}
	else
	{
		memcpy(temp, m_pData, position);
		memcpy(temp + position, copy, size);
		memcpy(temp + position + size, m_pData + position, m_nSize - position);
	}
	delete[] copy;
	PostMsg(SETDATA,(void*)temp);
	EnableButton(TRUE);
}

BOOL SoundCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta > 0)
	{
		/*if(m_nTimeStep * 2 > 32)
		{
			return CWnd::OnMouseWheel(nFlags, zDelta, pt);
		}*/
		SetTimeStep(m_nTimeStep * 2);
	}
	else
	{
		if(m_nTimeStep > 1)
		{
			SetTimeStep(m_nTimeStep / 2);
		}

	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void SoundCtrl::CreateAudioThread()
{
	m_hAudioEvent	 = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_pAudiothread = AfxBeginThread(AudioThread,this);
}

void SoundCtrl::TerminateAudioThread()
{
	m_bIsStoped = TRUE;
	CloseWave();
	PostMsg(EXIT, NULL);
	SetAudioEvent();
	SetAudioEvent();
	WaitForSingleObject(m_pAudiothread->m_hThread, INFINITE);
}

void SoundCtrl::SetPlayState(BOOL flag)
{
	m_bIsStoped = flag;
	if(flag)
	{
		waveOutPause(m_hWaveOut);
		m_btnPlay->SetWindowText(_T("▶"));
	}
	else
	{
		waveOutRestart(m_hWaveOut);
		m_btnPlay->SetWindowText(_T("||"));
	}
}

void SoundCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	PointF pt(static_cast<REAL>(point.x), static_cast<REAL>(point.y));
	m_nRButtonX = point.x - 5;
	if(m_rcfGraph.Contains(pt))
	{
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);

		CMenu* pMenu = menu.GetSubMenu(0);
		ClientToScreen(&point);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x,	point.y, this);
	}
	CWnd::OnRButtonDown(nFlags, point);
}


void SoundCtrl::OnSoundSetplayposition()
{
	// TODO: Add your command handler code here
	REAL maxPos = ByteToCoordinate(m_nSize, m_WaveFormatEx.nAvgBytesPerSec, 
																 m_nTimeStep);
	m_play->Move(CPoint(m_nRButtonX, 0), m_nWidth, maxPos - 5, 5.f, m_nCurPage);

	if(chkRegion->GetCheck() && (m_play->GetPos() > m_end->GetPos() ||
		m_play->GetPos() < m_start->GetPos()))
	{
		PostMsg(STOP, NULL);
	}
	else
	{
		m_play->ByteSetUp(m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep, m_nSize);
		m_play->SetPage(m_nCurPage);
		int* byte = new int(m_play->GetByte());
		PostMsg(SETPOSITION, byte);
		SetAudioEvent();
	}
	InvalidateRect(m_rcDraw, FALSE);
}


void SoundCtrl::OnSoundSetstartposition()
{
	// TODO: Add your command handler code here
	REAL maxPos = m_end->GetPos() - 15.f;
	m_start->Move(CPoint(m_nRButtonX, 0), m_nWidth, maxPos, 5.f, m_nCurPage);
	m_start->ByteSetUp(m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep, m_nSize);
	m_start->SetPage(m_nCurPage);
	if(chkRegion->GetCheck() && m_start->GetPos() > m_play->GetPos())
	{
		PostMsg(STOP, NULL);
	}
	InvalidateRect(m_rcDraw, FALSE);
}


void SoundCtrl::OnSoundSetendposition()
{
	// TODO: Add your command handler code here
	REAL maxPos = ByteToCoordinate(m_nSize, m_WaveFormatEx.nAvgBytesPerSec, 
																 m_nTimeStep);
	REAL minPos = m_start->GetPos() + 5;
	m_end->Move(CPoint(m_nRButtonX, 0), m_nWidth, maxPos, minPos, m_nCurPage);
	m_end->ByteSetUp(m_WaveFormatEx.nAvgBytesPerSec, m_nTimeStep, m_nSize);
	m_end->SetPage(m_nCurPage);
	if(chkRegion->GetCheck() && m_end->GetPos() < m_play->GetPos())
	{
		PostMsg(STOP, NULL);
	}
	InvalidateRect(m_rcDraw, FALSE);
}

void SoundCtrl::Pcm8To16( SOUND* sound ) 
{
	char* temp = new char[sound->m_nSize * 2];
	int num = 0;
	for(UINT i = 0; i < sound->m_nSize; i++)
	{
		int sample16 = (int)(sound->m_pWaveData[i] - 0x80) << 8;
		temp[num++] = (char)(sample16 & 0xff);
		temp[num++] = (char)((sample16 >> 8) & 0xff);
	}
	delete sound->m_pWaveData;
	sound->m_pWaveData = temp;
	sound->m_nSize *= 2;
	sound->m_WaveFormatEx.nAvgBytesPerSec *= 2;
	sound->m_WaveFormatEx.nBlockAlign = 2;
	sound->m_WaveFormatEx.wBitsPerSample = 16;
}

void SoundCtrl::OnSoundPasteendposition()
{
	// TODO: Add your command handler code here
	int pos = m_end->GetByte();
	::SendMessage(m_pParent->m_hWnd,CH_PASTEDATA ,WPARAM(this), LPARAM(pos));
}


void SoundCtrl::OnSoundPastestartposition()
{
	// TODO: Add your command handler code here
	int pos = m_start->GetByte();
	::SendMessage(m_pParent->m_hWnd,CH_PASTEDATA ,WPARAM(this), LPARAM(pos));
}


void SoundCtrl::EnableButton( BOOL flag )
{
	m_btnStop->EnableWindow(flag);
	m_btnPlay->EnableWindow(flag);
	m_btnCopy->EnableWindow(flag);
	m_btnPaste->EnableWindow(flag);
	m_btnDelete->EnableWindow(flag);
	m_btnOpen->EnableWindow(flag);
	m_btnClose->EnableWindow(flag);
	m_btnSave->EnableWindow(flag);
	chkRegion->EnableWindow(flag);
}