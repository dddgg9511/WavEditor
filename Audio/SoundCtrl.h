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

//������ ũ��

//������ ����
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
	@�θ� ������ ����
	@pParent : �θ� ������
	*/
	void SetParent(CWnd* pParents) { m_pParent = pParents;}

	/**
	@Thread �Լ�
	@pDataInPut : SoundControll ��ü
	*/
	static UINT AudioThread( LPVOID pDataInput );
		/**
	@AudioBuffer ����
	@Idx		: ������ Buffer�� �ε���
	@return : Sound�� ������� ���� Byte
	*/
	UINT BufferSeting(const UINT& Idx);
	/**
	@AudioThread�� ���ν���
	@return : Thread ����� -1 ��ȯ
	*/
	UINT AudioProc();
	/**
	@Wav ���� ���� 
	@sound : ������ Sound ��ü
	*/
	void SetSound(SOUND* sound);
	/**
	@��� ��ư Ŭ��
	*/
	void PlayAndPause();
	/**
	@����
	*/
	void Stop();
	/**
	@���
	*/
	void Play();
		/**
	@�Ͻ�����
	*/
	void Pause();
	/**
	@WaveOut �ʱ�ȭ �۾�
	*/
	void OpenWave();
  /**
  @Event Ȱ��
  */
	void SetAudioEvent();
		/**
	@WaveOut ���� �۾�
	*/
	void CloseWave();
	
	/**
	@���ν����� �޽����� ����
	@msgID : Message�� ID
	@data	 : ���� �Ķ����
	*/
	void PostMsg( const UINT Id, void* data );
	/**
	@PCM Data ����
	@data : ������ PCM Data
	*/
	void SetData(char* data);

	/**
	@���� ���� ������ ����
	*/
	void DeleteData();
	/**
	@PCM data���� Left Channel, Right Channel ����
	*/
	void Sample();
	/**
	@TimeStep ����
	@step : ���� ��
	*/
	void SetTimeStep( const UINT& step );
	/**
	@Channel�� 1�϶� ������ ����
	*/
	void OneChannel();
	/**
	@Channel�� 2�϶� ������ �߷�
	*/
	void TwoChannel();
	/**
	@��� ��ġ ����
	@sec : ������ ��ġ (second)
	*/
	void setPosition(const int& val);
	/**
	@������ ��ȯ
	@format : ��ȯ�� �����Ϳ� ����
	@dst		:
	@src		: ��ȯ�� ������
	@return	: ��ȯ�� ������
	*/
	short* ConvertData(const WAVEFORMATEX& format, short* src);
	/**
	@Channel�� 1�϶� �׸��� �Լ�
	*/
	void DrawOneChannel();
	/**
	@Channel�� 2�϶� �׸��� �Լ�
	*/
	void DrawTwoChannel();
	/**
	@data �ٿ��ֱ�
	@sound : �ٿ����� ������ ����
	*/
	void PasteData(COPYSOUND* sound, const int& position);
	/**
	@Audio Thread ����
	*/
	void CreateAudioThread();
	/**
	@Thread ����
	*/
	void TerminateAudioThread();
	/**
	@��� ���� ����
	*/
	void SetPlayState(BOOL flag); 
  /**
  @������
  @Pos    : ������ ��ġ
  @arr    : PCM Data
  @return : ��ȯ�� ������
  */
  short interpolation(const double& pos, short* arr);\
  /**
  @Sound ����
  */
	void Close();
	/**
	@8Bit PCM �� 16Bit PCM���� ��ȯ
	@sound : ��ȯ�� Sound
	*/
	void Pcm8To16(SOUND* sound);
	/**
	@��� ��ġ �̵�
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
	//AudioThread�� WaveoutProc�� ����ȭ ��ü
	HANDLE				m_hAudioEvent;	
	//Message�� ����� �ڷᱸ��
	std::queue<Message*> m_queue;
	//Waveout Handle
	HWAVEOUT m_hWaveOut;									
	//WaveoutHeader
	WAVEHDR	m_waveHdr[BUFFERCOUNT];

	//AudioFile�� Buffer
	char*	m_buffer[BUFFERCOUNT];
	//���� ��� ��ġ (byte)
	int	m_nCurByte;
	//���� ����
	BOOL	m_bIsStoped;
	//����� Byte 1�� �з� ����Ҷ����� �ʱ�ȭ
	int	m_nTime;
	//Thread�� ��ü
	CWinThread*		m_pAudiothread;
	//Graph�� �ð� ����(TimePerPixel)
	int m_nTimeStep;
	//Wave Format ����ü
	WAVEFORMATEX m_WaveFormatEx;
	//Wav ������ �̸�
	CString m_strFileName;
	//Wav ������ PCM Data
	char* m_pData;
	//PCM Data�� ũ��
	int	m_nSize;
	//Graph ������ ���� ����
	int	m_nWidth;
	//Controll ����
	RectF m_rcfBtn;
	//Graph ����
	RectF m_rcfGraph;
	//Left Channel Graph ����
	RectF m_rcfLchannel;
	//Right Channel Graph ����
	RectF m_rcfRchannel;

	//Start ��ġ
	CRectangle* m_start;
	//End ��ġ
	CRectangle* m_end;
	//Play ��ġ
	CRectangle* m_play;
	//Invalidate ����
	CRect m_rcDraw;
	//��Ʈ�Ѱ� ����
	REAL m_fMargin;
	
	//���� ��ư
	CButton* m_btnStop;
	//���, �Ͻ� ���� ��ư
	CButton* m_btnPlay;
	//���� ��ư
	CButton* m_btnCopy;
	//�ٿ��ֱ� ��ư
	CButton* m_btnPaste;
	//���� ��ư
	CButton* m_btnDelete;
	//���� ��ư
	CButton* m_btnOpen;
	//Ŭ���� ��ư
	CButton* m_btnClose;
	//���� ��ư
	CButton* m_btnSave;
	//Region ��ư
	CButton* chkRegion;
	//�巡�� �ð�ǥ��  
	CStatic* staticTime;
	//���� �̸� ǥ��
	CStatic* m_staticFileName;
	//��� �ð� ǥ��
	CStatic* m_staticPlayTime;
	//Wav ������ ���� ǥ��
	CStatic* m_staticEndTime;
	//���� ������
	int m_nCurPage;
	//�ִ� ������
	int m_nMaxPage;
	//Rbutton Down ��ġ
	int m_nRButtonX;
	//Graph Bitmap
	Bitmap* m_bmpGraph;
	//Parent Window
	CWnd* m_pParent;
	//PCM Data�� Left Channel Data
	short* m_left;
	//PCM Data�� Right Channel Data
	short* m_right;
	//Button�� ���� ����
	int m_btnWidth;
  //Buffer �� ũ��
	int m_nBufferSize;

};



