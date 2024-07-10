// ConsDiagStc.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "ConsDiagStc.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConsDiagStc

CConsDiagStc::CConsDiagStc()
{
	memset(m_wData, 0, CONST_DIAG_NUM*sizeof(int));
	m_nIndex = 0;
	m_nMaxValue = 0;
}

CConsDiagStc::~CConsDiagStc()
{
}


BEGIN_MESSAGE_MAP(CConsDiagStc, CStatic)
	//{{AFX_MSG_MAP(CConsDiagStc)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConsDiagStc message handlers

void CConsDiagStc::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	
	CDC memdc;
	CBitmap map;
	memdc.CreateCompatibleDC(&dc);
	int nWidth = rect.Width();
	int nHeight = rect.Height();
	map.CreateCompatibleBitmap(&dc, nWidth, nHeight);
	CBitmap *pold = memdc.SelectObject(&map);
	CBrush brush;
	brush.CreateSolidBrush(RGB(0,0,0));
	memdc.FillRect(rect, &brush);

	//draw axis
	DrawAxis(&memdc, rect);

	//draw chart
	CRect client_rt(rect.left + 25, rect.top + 25, rect.right - 25, rect.bottom - 25);
	if(m_nMaxValue > 0)
	{
		DrawChart(&memdc, client_rt);
	}
	
	dc.BitBlt(0,0,nWidth,nHeight,&memdc,0,0,SRCCOPY);

	memdc.SelectObject(pold);
	memdc.DeleteDC();
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CConsDiagStc::DrawAxis(CDC *pDC, CRect rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0,128,0));
	CPen *oldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(rect.left, rect.Height()/2);
	pDC->LineTo(rect.right, rect.Height()/2);

	pDC->MoveTo(rect.left + rect.Width()/2, rect.top);
	pDC->LineTo(rect.left + rect.Width()/2, rect.bottom);

	CBrush brush(RGB(0,128,0));
	CRect rt_new(rect.left + 25, rect.top + 25, rect.right - 25, rect.bottom - 25);
	pDC->FrameRect(&rt_new, &brush);


	pDC->SelectObject(&oldPen);
}

void CConsDiagStc::DrawChart(CDC *pDC, CRect rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 3, RGB(255,0,0));
	CPen *oldPen = pDC->SelectObject(&pen);
	
	CPoint pt_origin(rect.left + rect.Width()/2, rect.top+rect.Height()/2);

	int nIndex = m_nIndex;
	
	for(int i = 0; i < nIndex; i += 2)
	{
		CPoint point;
		point.x = pt_origin.x + (int)(m_wData[i]*rect.Width()/2/m_nMaxValue + 0.5);
		point.y = pt_origin.y + (int)(m_wData[i+1]*rect.Height()/2/m_nMaxValue + 0.5);

		if(point.x == pt_origin.x && point.y == pt_origin.y)
		{
			continue;
		}
		pDC->MoveTo(point.x, point.y);
		pDC->LineTo(point.x, point.y);
	}

	pDC->SelectObject(&oldPen);
}

void CConsDiagStc::AddData(WORD wData, BOOL bClear)
{
	if(bClear || m_nIndex >= CONST_DIAG_NUM)
	{
		Clear();
	}

	if(wData <= 0x7FFF)
	{
		m_wData[m_nIndex] = wData;
	}
	else
	{
		m_wData[m_nIndex] = -(0xFFFF-wData+1);
	}

	if(fabs(m_wData[m_nIndex]) > m_nMaxValue)
	{
		m_nMaxValue = (int)fabs(m_wData[m_nIndex]);
	}
	m_nIndex ++;
}

void CConsDiagStc::Clear()
{
	memset(m_wData, 0, CONST_DIAG_NUM*sizeof(int));
	m_nIndex = 0;
	m_nMaxValue = 0;
}