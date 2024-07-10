// BitmapCtrl.cpp: implementation of the CBitmapCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Audiotester.h"
#include "BitmapCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitmapCtrl::~CBitmapCtrl()
{
	if(memDC.GetSafeHdc() != NULL)
	{
		memDC.SelectObject(pOldBitmap);
		if(pBitmap != NULL)
			delete pBitmap;
	}
}
BEGIN_MESSAGE_MAP(CBitmapCtrl, CWnd)
	//{{AFX_MSG_MAP(CBitmapCtrl)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CBitmapCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	m_iSrollbarWidth = GetSystemMetrics(SM_CXVSCROLL);
	
	// create window
	BOOL ret;
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	ret = CWnd::CreateEx(WS_EX_CLIENTEDGE,className,NULL,dwStyle, 
		rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(),0);
	// init virtual screen
	GetClientRect(m_rcClient);
    
	CClientDC dc(this);
	if(memDC.GetSafeHdc() == NULL)
	{
		memDC.CreateCompatibleDC(&dc);
		if((pBitmap = new CBitmap()) == NULL) return FALSE;
		pBitmap->CreateCompatibleBitmap(&dc,m_rcClient.Width(),m_rcClient.Height());
		pOldBitmap = memDC.SelectObject(pBitmap);
		memDC.SelectStockObject(ANSI_VAR_FONT);
		memDC.SetBkMode(TRANSPARENT);
	}
	m_iXOffset = 0;
	m_iYOffset = 0;    
	memDC.FillSolidRect(&m_rcClient,RGB(128,128,128));
    

	return ret;
}

void CBitmapCtrl::OnPaint() 
{
	CPaintDC dc(this);
	if(memDC.GetSafeHdc() != NULL)
		dc.BitBlt(0,0,m_rcClient.Width(),m_rcClient.Height(),&memDC,0,0,SRCCOPY);
}


void CBitmapCtrl::SetBitmap(HBITMAP hBmp)
{
	//reset offsets
	m_iXOffset = 0;
	m_iYOffset = 0;

	//reset scrollbar
	SCROLLINFO si;
	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nMin = 0;
	si.nMax = 0;
	si.nPage = 0;
	SetScrollInfo(SB_HORZ, &si, TRUE);
	SetScrollInfo(SB_VERT, &si, TRUE);
	
	//redraw background
	GetClientRect(&m_rcClient);
    //m_rcClient.right += 150;
	memDC.FillSolidRect(&m_rcClient,RGB(128,128,128));
		
	//get bitmap handle
	m_hBitmap = hBmp;

	if (m_hBitmap == 0)
	{
		memDC.TextOut(0,0,"File not found.");
		Invalidate();
		return;
	}
	
	//get image size
	CPoint point(m_iXOffset,m_iYOffset);
	BITMAP bmpInfo;
	CBitmap::FromHandle(m_hBitmap)->GetBitmap(&bmpInfo);
	m_sizeImage.cx = bmpInfo.bmWidth ;
	m_sizeImage.cy = bmpInfo.bmHeight;
	
	//check image size
	BOOL x_fit;
	BOOL y_fit;
	x_fit = (bmpInfo.bmWidth <= m_rcClient.Width());
	if (!x_fit)
		m_rcClient.bottom -= m_iSrollbarWidth;
	y_fit = (bmpInfo.bmHeight <= m_rcClient.Height());
	if (!y_fit)
	{
		m_rcClient.right -= m_iSrollbarWidth;
		x_fit = (bmpInfo.bmWidth <= m_rcClient.Width());
	}
	if (!x_fit)
	{
		// show scrollbar
		ShowScrollBar(SB_HORZ);	
		// update scrollbar
		SCROLLINFO sii;
		sii.fMask = SIF_PAGE | SIF_RANGE;
		sii.nMin = 0;
		sii.nMax = bmpInfo.bmWidth-1;
		sii.nPage = m_rcClient.Width();
		SetScrollInfo(SB_HORZ, &sii, TRUE);
	}
	if (!y_fit)
	{
		// show scrollbar
		ShowScrollBar(SB_VERT);
		// update scrollbar
		SCROLLINFO sii;
		sii.fMask = SIF_PAGE | SIF_RANGE;
		sii.nMin = 0;
		sii.nMax = bmpInfo.bmHeight-1;
		sii.nPage = m_rcClient.Height();
		SetScrollInfo(SB_VERT, &sii, TRUE);
	}
	
	DrawBitmap();
}

void CBitmapCtrl::DrawBitmap()
{/*
	CPoint point(-m_iXOffset,-m_iYOffset);
    
	memDC.DrawState(point,m_sizeImage,m_hBitmap,DST_BITMAP|DSS_NORMAL);
    
    PaintSpecialValue(&memDC);
	Invalidate();
    */

    int zx = m_rcClient.Width() / 7;
    int zy = m_rcClient.Height() / 4;

    CPoint point( 0,0 );
    CSize sz( zx * 4,zy * 4 );
    if( m_hBitmaps[0] )
    {
        memDC.DrawState(point,sz,m_hBitmaps[0],DST_BITMAP|DSS_NORMAL);
    }
    
    point.x = zx * 4;
    sz.cx = zx * 2;
    if( m_hBitmaps[1] )
    {
        memDC.DrawState(point,sz,m_hBitmaps[1],DST_BITMAP|DSS_NORMAL);
    }

    point.y = zy * 3;
    sz.cy = zy;
    if( m_hBitmaps[2] )
    {
        memDC.DrawState(point,sz,m_hBitmaps[2],DST_BITMAP|DSS_NORMAL);
    }

    point.x = zx * 6;
    point.y = 0;
    sz.cx = zx;
    sz.cy = zy;
    if( m_hBitmaps[3] )
    {
        memDC.DrawState(point,sz,m_hBitmaps[3],DST_BITMAP|DSS_NORMAL);
    }

    point.y += zy;
    if( m_hBitmaps[4] )
    {
        memDC.DrawState(point,sz,m_hBitmaps[4],DST_BITMAP|DSS_NORMAL);
    }

    point.y += zy;
    if( m_hBitmaps[5] )
    {
        memDC.DrawState(point,sz,m_hBitmaps[5],DST_BITMAP|DSS_NORMAL);
    }

    point.y += zy;
    if( m_hBitmaps[6] )
    {
        memDC.DrawState(point,sz,m_hBitmaps[6],DST_BITMAP|DSS_NORMAL);
    }

    PaintSpecialValue(&memDC);
    Invalidate();    
}

void CBitmapCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nSmall = 1;
	int nLarge = 20;

	if(pScrollBar == GetScrollBarCtrl(SB_HORZ))
	{
		if(nSBCode == SB_LEFT || nSBCode == SB_LINELEFT)
			m_iXOffset -= nSmall;
		if(nSBCode == SB_PAGELEFT)
			m_iXOffset -= nLarge;
		if(nSBCode == SB_RIGHT || nSBCode == SB_LINERIGHT)
			m_iXOffset += nSmall;
		if(nSBCode == SB_PAGERIGHT)
			m_iXOffset += nLarge;
		if(nSBCode == SB_THUMBPOSITION)
			m_iXOffset = (int)nPos;
		if(nSBCode == SB_THUMBTRACK)
			m_iXOffset = (int)nPos;
		if (m_iXOffset<0)
			m_iXOffset = 0;
		if (m_iXOffset>m_sizeImage.cx - m_rcClient.Width())
			m_iXOffset = m_sizeImage.cx - m_rcClient.Width();
		SetScrollPos(SB_HORZ, m_iXOffset, TRUE);
		DrawBitmap();
	}
}

void CBitmapCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nSmall = 1;
	int nLarge = 20;

	if(pScrollBar == GetScrollBarCtrl(SB_VERT))
	{
		if(nSBCode == SB_TOP || nSBCode == SB_LINEUP)
			m_iYOffset -= nSmall;
		if(nSBCode == SB_PAGEUP)
			m_iYOffset -= nLarge;
		if(nSBCode == SB_BOTTOM || nSBCode == SB_LINEDOWN)
			m_iYOffset += nSmall;
		if(nSBCode == SB_PAGEDOWN)
			m_iYOffset += nLarge;
		if(nSBCode == SB_THUMBPOSITION)
			m_iYOffset = (int)nPos;
		if(nSBCode == SB_THUMBTRACK)
			m_iYOffset = (int)nPos;
		if (m_iYOffset<0)
			m_iYOffset = 0;
		if (m_iYOffset>m_sizeImage.cy - m_rcClient.Height())
			m_iYOffset = m_sizeImage.cy - m_rcClient.Height();
		SetScrollPos(SB_VERT, m_iYOffset, TRUE);
		DrawBitmap();
	}
}

void CBitmapCtrl::SetBitmap( HBITMAP* phBmp )
{
    ASSERT( phBmp );
    if( NULL == phBmp )
    {
        return;
    }

    for( int i=0;i<BITMAP_COUNT;i++ )
    {
        m_hBitmaps[i] = phBmp[i];
    }

    //redraw background
    GetClientRect(&m_rcClient);
    //m_rcClient.right += 150;
    memDC.FillSolidRect(&m_rcClient,RGB(128,128,128));

    DrawBitmap();
}
