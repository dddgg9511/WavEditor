#pragma once
#include <queue>
struct SOUND;
struct COPYSOUND;
class CRectangle;

const UINT PLAYPAUSE		= 1;
const UINT STOP					= 2;
const UINT NEXTSKIP			= 3;
const UINT PREVSKIP			= 4;
const UINT CHANGESOUND	= 5;
const UINT EXIT					= 6;
const UINT SETPOSITION	= 7;
const UINT DEL					= 8;
const UINT SETDATA			= 9;
const UINT SETSOUND			= 10;
const UINT PAUSE				= 11;
const UINT CLOSE				= 12;


const UINT CH_COPYDATA = WM_USER + 1;
const UINT CH_PASTEDATA = WM_USER + 2;
const UINT CH_PLAY = WM_USER + 3;
// SoundCtrl
struct Message
{
	Message(const int& id, void* data);
	UINT m_ID;
	void* m_data;
};

//버퍼의 크기

//버퍼의 개수
const int BUFFERCOUNT = 4;

class SoundCtrl : public CWnd
{
	DECLARE_DYNAMIC(SoundCtrl)

public:
	SoundCtrl();
	virtual ~SoundCtrl();
	static BOOL RegisterWindowClass();
	virtual void PreSubclassWindow();
	/*
	@부모 윈도우 설정
	@pParent : 부모 윈도우
	*/
	void SetParent(CWnd* pParents) { m_pParent = pParents;}

	/**
	@Thread 함수
	@pDataInPut : SoundControll 객체
	*/
	static UINT AudioThread( LPVOID pDataInput );
		/**
	@AudioBuffer 변경
	@Idx		: 변경할 Buffer의 인덱스
	@return : Sound의 재생하지 않은 Byte
	*/
	UINT BufferSeting(const UINT& Idx);
	/**
	@AudioThread의 프로시저
	@return : Thread 종료시 -1 반환
	*/
	UINT AudioProc();
	/**
	@Wav 파일 변경 
	@sound : 변경할 Sound 객체
	*/
	void SetSound(SOUND* sound);
	/**
	@재생 버튼 클릭
	*/
	void PlayAndPause();
	/**
	@정지
	*/
	void Stop();
	/**
	@재생
	*/
	void Play();
		/**
	@일시정지
	*/
	void Pause();
	/**
	@WaveOut 초기화 작업
	*/
	void OpenWave();
  /**
  @Event 활성
  */
	void SetAudioEvent();
		/**
	@WaveOut 제거 작업
	*/
	void CloseWave();
	
	/**
	@프로시저에 메시지를 전달
	@msgID : Message의 ID
	@data	 : 전달 파라미터
	*/
	void PostMsg( const UINT Id, void* data );
	/**
	@PCM Data 변경
	@data : 변경할 PCM Data
	*/
	void SetData(char* data);

	/**
	@선택 구간 데이터 제거
	*/
	void DeleteData();
	/**
	@PCM data에서 Left Channel, Right Channel 추출
	*/
	void Sample();
	/**
	@TimeStep 설정
	@step : 설정 값
	*/
	void SetTimeStep( const UINT& step );
	/**
	@Channel이 1일때 데이터 추출
	*/
	void OneChannel();
	/**
	@Channel이 2일때 데이터 추룰
	*/
	void TwoChannel();
	/**
	@재생 위치 변경
	@sec : 변경할 위치 (second)
	*/
	void setPosition(const int& val);
	/**
	@데이터 변환
	@format : 변환할 데이터에 포맷
	@dst		:
	@src		: 변환할 데이터
	@return	: 변환된 데이터
	*/
	short* ConvertData(const WAVEFORMATEX& format, short* src);
	/**
	@Channel이 1일때 그리기 함수
	*/
	void DrawOneChannel();
	/**
	@Channel이 2일때 그리기 함수
	*/
	void DrawTwoChannel();
	/**
	@data 붙여넣기
	@sound : 붙여넣을 데이터 정보
	*/
	void PasteData(COPYSOUND* sound, const int& position);
	/**
	@Audio Thread 생성
	*/
	void CreateAudioThread();
	/**
	@Thread 종료
	*/
	void TerminateAudioThread();
	/**
	@재생 상태 변경
	*/
	void SetPlayState(BOOL flag); 
  /**
  @보간법
  @Pos    : 데이터 위치
  @arr    : PCM Data
  @return : 변환된 데이터
  */
  short interpolation(const double& pos, short* arr);\
  /**
  @Sound 제거
  */
	void Close();
	/**
	@8Bit PCM 을 16Bit PCM으로 변환
	@sound : 변환할 Sound
	*/
	void Pcm8To16(SOUND* sound);
	/**
	@재생 위치 이동
	*/
	void MovePosition();
	void EnableButton(BOOL flag);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSoundSetplayposition();
	afx_msg void OnSoundSetstartposition();
	afx_msg void OnSoundSetendposition();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSoundPasteendposition();
	afx_msg void OnSoundPastestartposition();
  void OnBnClickedBtnClose();
  void OnBnClickedBtnOpen();
  void OnBnClickedBtnCheck();
  void OnBnClickedBtnCopy();
  void OnBnClickedBtnPaste();
  void OnBnClickedBtnDelete();
  void OnBnClickedBtnStop();
  void OnBnClickedBtnPlay();
	void OnBnClickedBtnSave();

private:
	CRect m_clientRect;
	//AudioThread와 WaveoutProc의 동기화 객체
	HANDLE				m_hAudioEvent;	
	//Message가 저장될 자료구조
	std::queue<Message*> m_queue;
	//Waveout Handle
	HWAVEOUT m_hWaveOut;									
	//WaveoutHeader
	WAVEHDR	m_waveHdr[BUFFERCOUNT];

	//AudioFile의 Buffer
	char*	m_buffer[BUFFERCOUNT];
	//현재 재생 위치 (byte)
	int	m_nCurByte;
	//정지 여부
	BOOL	m_bIsStoped;
	//재생한 Byte 1초 분량 재생할때마다 초기화
	int	m_nTime;
	//Thread의 객체
	CWinThread*		m_pAudiothread;
	//Graph의 시간 배율(TimePerPixel)
	int m_nTimeStep;
	//Wave Format 구조체
	WAVEFORMATEX m_WaveFormatEx;
	//Wav 파일의 이름
	CString m_strFileName;
	//Wav 파일의 PCM Data
	char* m_pData;
	//PCM Data의 크기
	int	m_nSize;
	//Graph 영역의 가로 길이
	int	m_nWidth;
	//Controll 영역
	RectF m_rcfBtn;
	//Graph 영역
	RectF m_rcfGraph;
	//Left Channel Graph 영역
	RectF m_rcfLchannel;
	//Right Channel Graph 영역
	RectF m_rcfRchannel;

	//Start 위치
	CRectangle* m_start;
	//End 위치
	CRectangle* m_end;
	//Play 위치
	CRectangle* m_play;
	//Invalidate 영역
	CRect m_rcDraw;
	//컨트롤간 간격
	REAL m_fMargin;
	
	//정지 버튼
	CButton* m_btnStop;
	//재생, 일시 정지 버튼
	CButton* m_btnPlay;
	//복사 버튼
	CButton* m_btnCopy;
	//붙여넣기 버튼
	CButton* m_btnPaste;
	//삭제 버튼
	CButton* m_btnDelete;
	//오픈 버튼
	CButton* m_btnOpen;
	//클로즈 버튼
	CButton* m_btnClose;
	//저장 버튼
	CButton* m_btnSave;
	//Region 버튼
	CButton* chkRegion;
	//드래그 시간표시  
	CStatic* staticTime;
	//파일 이름 표시
	CStatic* m_staticFileName;
	//재생 시간 표시
	CStatic* m_staticPlayTime;
	//Wav 파일의 길이 표시
	CStatic* m_staticEndTime;
	//현재 페이지
	int m_nCurPage;
	//최대 페이지
	int m_nMaxPage;
	//Rbutton Down 위치
	int m_nRButtonX;
	//Graph Bitmap
	Bitmap* m_bmpGraph;
	//Parent Window
	CWnd* m_pParent;
	//PCM Data의 Left Channel Data
	short* m_left;
	//PCM Data의 Right Channel Data
	short* m_right;
	//Button의 가로 길이
	int m_btnWidth;
  //Buffer 의 크기
	int m_nBufferSize;

};



