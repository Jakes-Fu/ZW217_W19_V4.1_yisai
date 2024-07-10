// CoolButton.cpp : implementation file
//

#include "stdafx.h"
#include "CoolButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//const COLORREF CCoolButton::CLR_DISABLE_TEXT = RGB(127, 127, 127);
/////////////////////////////////////////////////////////////////////////////
// CCoolButton

CCoolButton::CCoolButton()
{
	m_bHover      = FALSE;
	m_bAutoSize   = FALSE;
	m_bShowText   = TRUE;
	m_bUsedColors = TRUE;
	m_bClicked = FALSE;

	m_clrNormal   = GetSysColor(COLOR_BTNFACE);
	m_clrHover    = RGB(210, 225, 251);
	m_clrClicked  = RGB(133, 174, 243);
	m_clrDisabled = RGB(230, 230, 230);	
}

CCoolButton::~CCoolButton()
{
}

BEGIN_MESSAGE_MAP(CCoolButton, CButton)
	//{{AFX_MSG_MAP(CCoolButton)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolButton message handlers

LRESULT CCoolButton::OnMouseHover(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
//	m_bHover = TRUE;

//	Invalidate();

	return 0L;
}

LRESULT CCoolButton::OnMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
//	m_bHover = FALSE;

//	Invalidate();

	return 0L;
}

void CCoolButton::OnMouseMove(UINT nFlags, const CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( !m_bHover )
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize      = sizeof(TRACKMOUSEEVENT);
		tme.hwndTrack   = m_hWnd;
		tme.dwFlags     = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 4;
		
		_TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}

void CCoolButton::SetAutoSize(BOOL bAutoSize)
{
	m_bAutoSize = bAutoSize;
}

void CCoolButton::SetShowText(BOOL bShow)
{
	m_bShowText = bShow;
}

BOOL CCoolButton::LoadBitmaps( UINT nNormal, UINT nHover, 
							   UINT nClicked/* = 0 */, UINT nDisabled/* = 0 */)
{
	
	return LoadBitmaps( MAKEINTRESOURCE(nNormal),  MAKEINTRESOURCE(nHover),
		                MAKEINTRESOURCE(nClicked), MAKEINTRESOURCE(nDisabled) );
}

BOOL CCoolButton::LoadBitmaps( LPCTSTR pszNormal, LPCTSTR pszHover, 
							   LPCTSTR pszClicked/* = NULL */, LPCTSTR pszDisabled/* = NULL */)
{
	_ASSERTE( pszNormal != NULL );
	
	ModifyStyle(0, BS_OWNERDRAW);

	VERIFY( m_bmpNormal.LoadBitmap(pszNormal) );

	if( pszHover != NULL )
	{
		VERIFY( m_bmpHover.LoadBitmap(pszHover) );
	}
	else
	{
		VERIFY( m_bmpHover.LoadBitmap(pszNormal) );
	}

	if( pszClicked != NULL )
	{
		VERIFY( m_bmpClicked.LoadBitmap(pszClicked) );
	}
	else
	{
		VERIFY( m_bmpClicked.LoadBitmap(pszNormal) );
	}

	if( pszDisabled != NULL )
	{
		VERIFY( m_bmpDisabled.LoadBitmap(pszDisabled) );
	}
	else
	{
		VERIFY( m_bmpDisabled.LoadBitmap(pszNormal) );
	}

	m_bUsedColors = FALSE;
	m_bShowText   = FALSE;
	m_bAutoSize   = TRUE;

	return TRUE;
}

void CCoolButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item

	CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

//	m_bClicked = FALSE;

	CBitmap * pBmp       = NULL;
	COLORREF  clrBckGrnd = 0;
// 	if( ODS_DISABLED & lpDrawItemStruct->itemState )
// 	{
// 		pBmp       = &m_bmpDisabled;
// 		clrBckGrnd = m_clrDisabled;
// 
// 		pDC->SetTextColor( CLR_DISABLE_TEXT );
// 	}
// 	else if( /*ODS_SELECTED & lpDrawItemStruct->itemState ||*/
// 		     ODS_CHECKED  & lpDrawItemStruct->itemState )
// 	{
// 		pBmp       = &m_bmpClicked;
// 		clrBckGrnd = m_clrClicked;
// 
// 		m_bClicked   = TRUE;
// 	}
// 	else if( m_bHover )//|| ODS_FOCUS & lpDrawItemStruct->itemState )
// 	{
// 		pBmp       = &m_bmpHover;
// 		clrBckGrnd = m_clrHover;
// 	}
// 	else
// 	{
// 		pBmp       = &m_bmpNormal;
// 		clrBckGrnd = m_clrNormal;
// 	}
// 	
	if(m_bClicked)
	{
 		pBmp       = &m_bmpClicked;
		clrBckGrnd = m_clrClicked; 

	}
	else
	{
 		pBmp       = &m_bmpNormal;
		clrBckGrnd = m_clrNormal;
	}
	if( m_bUsedColors )
	{
		DrawBackGround( pDC, clrBckGrnd );
	}
	else
	{
		DrawBackGround( pDC, pBmp );
	}

	if( m_bShowText )
	{
		DrawText(pDC);	
	}
}

void CCoolButton::DrawText(CDC *pDC)
{
	static _TCHAR szText[32] = { 0 };

	GetWindowText(szText, 31);

	//CSize sizeText = pDC->GetOutputTextExtent(szText, _tcslen(szText));
	
	CRect rc;
	GetClientRect(rc);
	
	pDC->SetBkMode(TRANSPARENT);
    //pDC->TextOut((rc.Width() - sizeText.cx) / 2, (rc.Height() - sizeText.cy) / 2, szText);
	pDC->DrawText(szText, -1, rc,  DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CCoolButton::SetColors(COLORREF clrNormal, COLORREF clrHover, COLORREF clrClicked, COLORREF clrDisabled)
{
	ModifyStyle(0, BS_OWNERDRAW);

	m_clrNormal   = clrNormal;
	m_clrHover    = clrHover;
	m_clrClicked  = clrClicked;
	m_clrDisabled = clrDisabled;
}

void CCoolButton::DrawBackGround(CDC *pDC, COLORREF clrBckGrnd)
{
	CRect rc;
	GetClientRect(rc);

	COLORREF clrTopLeft     = GetSysColor(COLOR_BTNHILIGHT);
	COLORREF clrBottomRight = GetSysColor(COLOR_BTNSHADOW);

	CBrush brsh;
	VERIFY( brsh.CreateSolidBrush(clrBckGrnd) );
	pDC->FillRect(rc, &brsh);
	if( m_bHover && !m_bClicked)
	{
		pDC->Draw3dRect(rc, clrTopLeft, clrBottomRight);
	}
	else if( m_bClicked )
	{
		pDC->DrawEdge( rc, EDGE_SUNKEN, BF_RECT);
	}
	
	brsh.DeleteObject();
}

void CCoolButton::DrawBackGround(CDC * pDC, CBitmap * pBmp)
{
	CRect rc;
	GetClientRect(rc);

//	COLORREF clrTopLeft     = GetSysColor(COLOR_BTNHILIGHT);
//	COLORREF clrBottomRight = GetSysColor(COLOR_BTNSHADOW);

	BITMAP bmpInfo;
	VERIFY( pBmp->GetBitmap(&bmpInfo) );
	if( !m_bAutoSize )
	{
		rc.right  = rc.left + bmpInfo.bmWidth;
		rc.bottom = rc.top  + bmpInfo.bmHeight;
	}
	
	CDC memDC;
	VERIFY( memDC.CreateCompatibleDC(pDC) );
	CBitmap * pOldBmp = memDC.SelectObject(pBmp);
	/* 不使用3D效果
	if( m_bHover && !m_bClicked )
	{
		memDC.Draw3dRect(rc, clrTopLeft, clrBottomRight);
	}
	else if( m_bClicked )
	{
		memDC.DrawEdge( rc, EDGE_SUNKEN, BF_RECT);
	}
	*/
	
	pDC->StretchBlt( 0, 0, rc.Width(), rc.Height(),
		&memDC,
		0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,
		SRCCOPY );
	
	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
}

void CCoolButton::AutoSize()
{
	if( !m_bAutoSize )
		return;

	BITMAP bmpInfo;
	VERIFY( m_bmpNormal.GetBitmap(&bmpInfo) );

	CWnd * pParent = GetParent();
	_ASSERTE( pParent != NULL );
	if(pParent == NULL)
	{
		return;
	}

	CRect rcParent;
	pParent->GetClientRect(rcParent);
	pParent->ClientToScreen(rcParent);

	CRect rcThis;
	GetWindowRect(rcThis);

	rcThis.DeflateRect(-rcParent.left, -rcParent.top);

	rcThis.right  = rcThis.left + bmpInfo.bmWidth;
	rcThis.bottom = rcThis.top  + bmpInfo.bmHeight;

	MoveWindow(rcThis, FALSE);
}
void CCoolButton::SetClicked(BOOL bClicked)
{
	m_bClicked = bClicked;
	Invalidate();
}