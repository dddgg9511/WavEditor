#pragma once
class CRectangle
{	
public:
	CRectangle(const RectF& rect, const Color& color);
	~CRectangle(void);
	/**
	@마우스 클릭 검사
	@Pt			: 마우스 위치
	@page		: 현재 페이지
	@width	: 가로 길이 
	*/
	BOOL Clicked(const CPoint& pt, const int& page, const int& width);
	/**
	@화면에 Draw
	@page		: 현재 페잊
	@width	: 가로 길이
	@g			: 그래픽 객체
	*/
	void Draw(const int& page, const int& width, Graphics* g);
	void Draw(const int& page, const int& width, const int& hieght, Graphics* g);
	/**
	@좌표 이동
	@pt			: 마우스 포인트 위치
	@width	: 가로 길이
	@maxPos : X 좌표 최댓 값
	@minPos : X 좌표 최솟 값
	@page		: 현재 페이지
	*/
	void Move(const CPoint& pt, const int& width,const REAL& maxPos,
					  const REAL& minPos, const int& page);
	/**
	@좌표로 Byte값 설정
	@bytePerSec : 초당 전송 byte 크기
	@timeStep		: 시간 배율
	@size				: 전체 크기
	*/
	void ByteSetUp(const int& bytePerSec, const int& timeStep, const int& size);
	/**
	@Byte 값으로 좌표 설정
	@bytePerSec : 초당 전송 Byte 크기
	@timeStop		: 시간 배율
	*/
	void PosSetUp(const int& bytePerSec, const int& timeStep);
	/**
	@위치와 좌표 설정
	@byte : 설정할 byte 값
	@pos	: 설정할 위치 값
	*/
	void SetPos(const int& byte, const REAL& pos);
	/**
	@현재 위치 반환
	@return : 현재 위치
	*/
	REAL GetPos() const { return m_Rect.X; }
	/**
	@페이지 설정
	@page : 페이지 설정 값
	*/
	void SetPage(const int& page) { m_nPage = page;}
	/**
	@현재 페이지 반환
	@return : 현재 페이지
	*/
	int GetPage() const { return m_nPage; }
	/**
	@현재 Byte 반환
	@reurn : 현재 Byte
	*/
	int GetByte() const { return m_nByte; }
	/**
	@Click 상태 변경
	*/
	void SetClickState();
	/**
	@Click 상태 반환
	@return 현재 클릭 상태
	*/
	BOOL GetClickState() const { return m_bClicked; }
	/**
	@byte 값 설정
	@byte 설정할 byte 값
	*/
	void SetByte(const int& byte) { m_nByte = byte; }
private:
	//사각형이 그려지는 위치
	RectF m_Rect;
	//Byte 
	int m_nByte;
	//사각형이 그려지는 페이지
	int m_nPage;
	//Brush
	SolidBrush m_brush;
	//Pen
	Pen m_pen;
	//클릭 상태
	BOOL m_bClicked;
};

