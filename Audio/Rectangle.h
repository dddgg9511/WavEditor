#pragma once
class CRectangle
{	
public:
	CRectangle(const RectF& rect, const Color& color);
	~CRectangle(void);
	/**
	@���콺 Ŭ�� �˻�
	@Pt			: ���콺 ��ġ
	@page		: ���� ������
	@width	: ���� ���� 
	*/
	BOOL Clicked(const CPoint& pt, const int& page, const int& width);
	/**
	@ȭ�鿡 Draw
	@page		: ���� ����
	@width	: ���� ����
	@g			: �׷��� ��ü
	*/
	void Draw(const int& page, const int& width, Graphics* g);
	void Draw(const int& page, const int& width, const int& hieght, Graphics* g);
	/**
	@��ǥ �̵�
	@pt			: ���콺 ����Ʈ ��ġ
	@width	: ���� ����
	@maxPos : X ��ǥ �ִ� ��
	@minPos : X ��ǥ �ּ� ��
	@page		: ���� ������
	*/
	void Move(const CPoint& pt, const int& width,const REAL& maxPos,
					  const REAL& minPos, const int& page);
	/**
	@��ǥ�� Byte�� ����
	@bytePerSec : �ʴ� ���� byte ũ��
	@timeStep		: �ð� ����
	@size				: ��ü ũ��
	*/
	void ByteSetUp(const int& bytePerSec, const int& timeStep, const int& size);
	/**
	@Byte ������ ��ǥ ����
	@bytePerSec : �ʴ� ���� Byte ũ��
	@timeStop		: �ð� ����
	*/
	void PosSetUp(const int& bytePerSec, const int& timeStep);
	/**
	@��ġ�� ��ǥ ����
	@byte : ������ byte ��
	@pos	: ������ ��ġ ��
	*/
	void SetPos(const int& byte, const REAL& pos);
	/**
	@���� ��ġ ��ȯ
	@return : ���� ��ġ
	*/
	REAL GetPos() const { return m_Rect.X; }
	/**
	@������ ����
	@page : ������ ���� ��
	*/
	void SetPage(const int& page) { m_nPage = page;}
	/**
	@���� ������ ��ȯ
	@return : ���� ������
	*/
	int GetPage() const { return m_nPage; }
	/**
	@���� Byte ��ȯ
	@reurn : ���� Byte
	*/
	int GetByte() const { return m_nByte; }
	/**
	@Click ���� ����
	*/
	void SetClickState();
	/**
	@Click ���� ��ȯ
	@return ���� Ŭ�� ����
	*/
	BOOL GetClickState() const { return m_bClicked; }
	/**
	@byte �� ����
	@byte ������ byte ��
	*/
	void SetByte(const int& byte) { m_nByte = byte; }
private:
	//�簢���� �׷����� ��ġ
	RectF m_Rect;
	//Byte 
	int m_nByte;
	//�簢���� �׷����� ������
	int m_nPage;
	//Brush
	SolidBrush m_brush;
	//Pen
	Pen m_pen;
	//Ŭ�� ����
	BOOL m_bClicked;
};

