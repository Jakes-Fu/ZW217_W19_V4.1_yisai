// ChannelImpuleStc.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "ChannelImpuleStc.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelImpuleStc

CChannelImpuleStc::CChannelImpuleStc()
{
 	memset(m_wData, 0, CHANNEL_IMPULSE_NUM*sizeof(WORD));
	m_nIndex = 0;
	m_dIdX = 0.0;
	m_dIdY = 0.0;
	m_nMaxValue = 0;
}

CChannelImpuleStc::~CChannelImpuleStc()
{
}


BEGIN_MESSAGE_MAP(CChannelImpuleStc, CStatic)
	//{{AFX_MSG_MAP(CChannelImpuleStc)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelImpuleStc message handlers

void CChannelImpuleStc::OnPaint() 
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
	DrawChart(&memdc, rect);

	dc.BitBlt(0,0,nWidth,nHeight,&memdc,0,0,SRCCOPY);

	memdc.SelectObject(pold);
	memdc.DeleteDC();
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CChannelImpuleStc::DrawAxis(CDC *pDC, CRect rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0,128,0));
	CPen *oldPen = pDC->SelectObject(&pen);

	CBrush brush(RGB(0,128,0));

	pDC->FrameRect(&rect, &brush);

	m_dIdY = (double)rect.Height() / (double)4;
	m_dIdX = (double)rect.Width() / (double)129;

	for(int i = 1; i < 4; i++)
	{
		pDC->MoveTo(rect.left, rect.top + (int)((m_dIdY * i)+0.5));
		pDC->LineTo(rect.right, rect.top + (int)((m_dIdY * i)+0.5));
	}
	
	for(i = 1; i < 129; i++)
	{
		pDC->MoveTo(rect.left + (int)((m_dIdX * i)+0.5), rect.top);
		pDC->LineTo(rect.left + (int)((m_dIdX * i)+0.5), rect.bottom);
	}

	pDC->SelectObject(&oldPen);
}

void CChannelImpuleStc::DrawChart(CDC *pDC, CRect rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	CPen *oldPen = pDC->SelectObject(&pen);

	int nIndex = m_nIndex;

	for(int i = 0; i < nIndex; i += 2)
	{
		int nValue = m_wData[i]*m_wData[i]+m_wData[i+1]*m_wData[i+1];
		//nValue = sqrt(nValue);
		if(nValue > m_nMaxValue)
		{
			m_nMaxValue = nValue;
		}
	}

	LOGFONT logfont;
	HFONT hFont, hOldFont;
	lstrcpy((LPSTR)logfont.lfFaceName,(LPSTR)"MS Sans Serif");   
	logfont.lfWeight=20;   
	logfont.lfWidth=7;   
	logfont.lfHeight=15;   
	logfont.lfEscapement=0;   
	logfont.lfUnderline=FALSE;   
	logfont.lfItalic=FALSE;   
	logfont.lfStrikeOut=FALSE;   
	logfont.lfCharSet=GB2312_CHARSET;   
    CFont myFont;
	myFont.CreateFontIndirect(&logfont);   
	hFont = myFont;   
    
	hOldFont=(HFONT__ *)pDC->SelectObject(hFont);   
	
	pDC->SetTextColor(RGB(120,0,120));
	pDC->SetBkColor(RGB(128,128,192));
	char szText[50] = {0};
	sprintf(szText, "%d", m_nMaxValue);
	pDC->DrawText(szText, CRect(rect.left, rect.top, rect.left+50, rect.top+15), DT_LEFT);
	sprintf(szText, "%d", m_nMaxValue*3/4);
	pDC->DrawText(szText, CRect(rect.left, (int)(rect.top+m_dIdY), rect.left+50, (int)(rect.top+m_dIdY+15)), DT_LEFT);
	sprintf(szText, "%d", m_nMaxValue/2);
	pDC->DrawText(szText, CRect(rect.left, (int)(rect.top+2*m_dIdY), rect.left+50, (int)(rect.top+2*m_dIdY+15)), DT_LEFT);
	sprintf(szText, "%d", m_nMaxValue/4);
	pDC->DrawText(szText, CRect(rect.left, (int)(rect.top+3*m_dIdY), rect.left+50, (int)(rect.top+3*m_dIdY+15)), DT_LEFT);

	pDC->SelectObject(hOldFont);
	
	for(i = 0; i < nIndex; i += 2)
	{
		int nValue = m_wData[i]*m_wData[i]+m_wData[i+1]*m_wData[i+1];
		//nValue = sqrt(nValue);
		if(nValue > 0)
		{
			int nX = rect.left+(int)((i/2+1)*m_dIdX+0.5);
			int nY = rect.bottom - nValue*rect.Height()/m_nMaxValue;
			pDC->MoveTo(nX, rect.bottom);
			pDC->LineTo(nX, nY);
			CPoint point(nX,nY);
			DrawArrowhead(pDC, point);
		}
	}

	pDC->SelectObject(&oldPen);
}

void CChannelImpuleStc::DrawArrowhead(
							 CDC *pDC,
							 CPoint point)
{
	CBrush brush;
	brush.CreateSolidBrush(RGB(255,0,0));
	pDC->SelectObject(&brush);
	CPoint pt[3];
	pt[0] = CPoint(point.x, point.y);
	pt[1] = CPoint(point.x - 2, point.y + 4);
	pt[2] = CPoint(point.x + 2, point.y + 4);
	pDC->Polygon(pt, 3);
}

void CChannelImpuleStc::AddData(WORD wData, BOOL bClear)
{
	if(bClear || m_nIndex >= CHANNEL_IMPULSE_NUM)
	{
		Clear();	
	}
	m_wData[m_nIndex] = wData;
	m_nIndex ++;
}

void CChannelImpuleStc::Clear()
{
	memset(m_wData, 0, CHANNEL_IMPULSE_NUM*sizeof(WORD));
	m_nIndex = 0;
	m_nMaxValue = 0;
}