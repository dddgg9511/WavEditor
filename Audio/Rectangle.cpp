#include "stdafx.h"
#include "Rectangle.h"


CRectangle::CRectangle( const RectF& rect, const Color& color ) 
	: m_Rect(rect),
		m_nByte(0),
		m_nPage(0),
		m_brush(color),
		m_pen(color,1),
		m_bClicked(FALSE)
{
}


CRectangle::~CRectangle(void)
{
}

void CRectangle::Draw(const int& page, const int& width, Graphics* g )
{
	if(m_nPage != page)
	{
		return;
	}
	RectF rect(m_Rect.X - page * width, m_Rect.Y, m_Rect.Width, m_Rect.Height);
	g->FillRectangle(&m_brush, rect);
}

void CRectangle::Draw(const int& page, const int& width, const int& hieght, 
											Graphics* g)
{
	if(m_nPage != page)
	{
		return;
	}
	REAL xPos = m_Rect.X - page * width; 
	RectF rect(xPos, m_Rect.Y, m_Rect.Width, m_Rect.Height);
	REAL bottom = rect.GetBottom();
	g->DrawLine(&m_pen, PointF(xPos + 5.f, bottom), PointF(xPos + 5.f, bottom+ hieght));
	g->FillRectangle(&m_brush, rect);
}


void CRectangle::SetPos( const int& byte, const REAL& pos )
{
	m_nByte = byte;
	m_Rect.X = pos;
}

void CRectangle::PosSetUp( const int& bytePerSec, const int& timeStep )
{
	m_Rect.X = ByteToCoordinate(m_nByte, bytePerSec, timeStep);
}

BOOL CRectangle::Clicked( const CPoint& pt, const int& page, const int& width )
{
	PointF ptf(static_cast<REAL>(pt.x + page * width), static_cast<REAL>(pt.y));
	m_bClicked = m_Rect.Contains(ptf);
	return m_bClicked;
}

void CRectangle::SetClickState()
{
	m_bClicked = FALSE;
}

void CRectangle::Move(const CPoint& pt, const int& width,const REAL& maxPos,
											const REAL& minPos, const int& page )
{
	REAL temp = m_Rect.X;
	m_Rect.X = static_cast<REAL>(pt.x - 5 + width * page);
	if(m_Rect.X < minPos)
	{
		m_Rect.X = minPos;
	}
	else if(pt.x < 5)
	{
		if(page > 0)
		{
			m_nPage--;	
			m_Rect.X = static_cast<REAL>((m_nPage + 1) * width - 5);
		}
		else
		{
			m_Rect.X = 5;
		}
	}
	else if(pt.x > width || 
		(pt.x - 5 + m_nPage * width) > maxPos)
	{
		if((pt.x - 5 + m_nPage * width) < maxPos)
		{
			m_nPage++;
			m_Rect.X = static_cast<REAL>(m_nPage * width + 5);
		}
		else
		{
			m_Rect.X = temp;
		}
	}
}

void CRectangle::ByteSetUp( const int& bytePerSec, const int& timeStep, const int& size )
{
	m_nByte = CoordinateToByte(m_Rect.X, bytePerSec, timeStep);
	if(size < m_nByte)
	{
		m_nByte = size;
	}
}

