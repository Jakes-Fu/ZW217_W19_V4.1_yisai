// 
// FileName: CoolBar.cpp
// anli.wei 整理完成
// Copyright (C) 2004 Spreadtrum Corporation
// All rights reserved.

#include "stdafx.h"
#include "SizingControlBar.h"
#include "CoolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
// CSCBButton

CSCBButton::CSCBButton()
{
    m_bRaised     = FALSE;
    m_bPushed     = FALSE;

	m_clrForeGrnd = RGB(255, 255, 255); // white color
}

void CSCBButton::Paint(CDC* pDC)
{
	_ASSERTE( pDC != NULL );

    const CRect lpRect = GetRect();

    if (m_bPushed)
	{
        pDC->Draw3dRect( lpRect, ::GetSysColor(COLOR_BTNSHADOW),
                         ::GetSysColor(COLOR_BTNHIGHLIGHT) );
	}
    else if (m_bRaised)
	{
        pDC->Draw3dRect( lpRect, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                         ::GetSysColor(COLOR_BTNSHADOW) );
	}

    COLORREF clrOldTextColor = pDC->GetTextColor();
    int      nPrevBkMode     = pDC->SetBkMode(TRANSPARENT);

    CFont font;
    int ppi = pDC->GetDeviceCaps(LOGPIXELSX);
    int pointsize = MulDiv(80, 96, ppi); // 6 points at 96 ppi
    // don't modified "Marlett", becuase we will look like "x" button
	// add by anli.wei
	font.CreatePointFont(pointsize, _T("Marlett"));  
    CFont* oldfont = pDC->SelectObject(&font);

	pDC->SetTextColor( m_clrForeGrnd );
	const _TCHAR szText[] = _T("r");
    pDC->TextOut(m_ptOrg.x + 1, m_ptOrg.y + 1, szText, _tcslen(szText)); // x-like

    pDC->SelectObject(oldfont);
    pDC->SetBkMode(nPrevBkMode);
    pDC->SetTextColor(clrOldTextColor);
	
	font.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////
// CCoolBar

const COLORREF CCoolBar::CLR_GRIPPER_FORE = RGB( 255, 255, 255 );
const COLORREF CCoolBar::CLR_GRIPPER_BACK = RGB( 98, 115, 156 );

IMPLEMENT_DYNCREATE(CCoolBar, CSizingControlBar);

CCoolBar::CCoolBar()
{
	m_bHasGripper = TRUE;
	m_bActive     = FALSE;
    m_nGripper    = 12;
	m_hNotifyWnd  = NULL;
	
	m_clrGripperFore = CLR_GRIPPER_FORE;
	m_clrGripperBack = CLR_GRIPPER_BACK;
}

CCoolBar::~CCoolBar()
{
	m_hNotifyWnd  = NULL;
}

BEGIN_MESSAGE_MAP(CCoolBar, CSizingControlBar)
    //{{AFX_MSG_MAP(CCoolBar)
    ON_WM_NCLBUTTONUP()
    ON_WM_NCHITTEST()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
// CCoolBar message handlers

// 目前我们只使用一个child，且不使用lpszLabel和hIcon
BOOL CCoolBar::AddPage( CRuntimeClass *pRtc, CCreateContext *pContext /* = NULL */,
					    LPCTSTR lpszLabel /* = NULL */, HICON hIcon /* = NULL */ )
{
	UNUSED_ALWAYS(lpszLabel);
	UNUSED_ALWAYS(hIcon);

	ASSERT_VALID(this);
	_ASSERTE( this->m_hWnd != NULL );
	_ASSERTE( pRtc != NULL );
	_ASSERTE( pRtc->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	_ASSERTE( AfxIsValidAddress(pRtc, sizeof(CRuntimeClass), FALSE) );
	_ASSERTE( lpszLabel == NULL && hIcon == NULL );
	_ASSERTE( m_pActiveWnd == NULL );

	CWnd * pWnd = (CWnd *)pRtc->CreateObject();
	if( NULL == pWnd  )
	{
		TRACE1( "Warning: Dynamic create of wnd type %hs failed.\n",
				pRtc->m_lpszClassName );
		return FALSE;
	}
    ASSERT_VALID( pWnd );

	CRect rect(0, 0, 0, 0);
	if( !pWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
		              rect, this, 0, pContext) ) 
	{
		TRACE0("Warning: could not create wnd for frame.\n");
		return FALSE;    
	}

	pWnd->ModifyStyleEx( 0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED );
	
	m_pActiveWnd = pWnd;

	return TRUE;
}

BOOL CCoolBar::Create( LPCTSTR lpszWindowName, CWnd* pParentWnd, 
					   const CSize &sizeDefault, UINT nID, 
					   BOOL  bHasGripper /* = FALSE */,
					   DWORD dwStyle     /* = WS_CHILD | WS_VISIBLE | CBRS_TOP */ )
{
	ASSERT_VALID(pParentWnd);

	m_hNotifyWnd     = pParentWnd->m_hWnd;
    m_bHasGripper    = bHasGripper;
    m_strGripperText = lpszWindowName;

    return CSizingControlBar::Create(lpszWindowName, pParentWnd, 
		                             sizeDefault, nID, bHasGripper, dwStyle);
}

BOOL CCoolBar::Create( LPCTSTR lpszWindowName, CWnd* pParentWnd, UINT nID,
                       DWORD dwStyle /* = WS_CHILD | WS_VISIBLE | CBRS_TOP */ )
{
	ASSERT_VALID(pParentWnd);

	m_hNotifyWnd     = pParentWnd->m_hWnd;
    m_strGripperText = lpszWindowName;

	return CSizingControlBar::Create(lpszWindowName, pParentWnd, nID, dwStyle);
}

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//
void CCoolBar::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
    if(nHitTest == HTCLOSE)
	{
        m_pDockSite->ShowControlBar(this, FALSE, FALSE); // hide
	}

    CSizingControlBar::OnNcLButtonUp(nHitTest, point);
}

void CCoolBar::NcCalcClient(LPRECT pRect, UINT nDockBarID)
{
	_ASSERTE( pRect != NULL );

    CRect rcBar(pRect); // save the bar rect

    // subtract edges
    CSizingControlBar::NcCalcClient(pRect, nDockBarID);

	if( IsFloating() )
	{
		return;
	}

    CRect rc(pRect); // the client rect as calculated by the base class

    BOOL bHorz = ( (nDockBarID == AFX_IDW_DOCKBAR_TOP) ||
                   (nDockBarID == AFX_IDW_DOCKBAR_BOTTOM) );

    if (bHorz)
        rc.DeflateRect(m_nGripper, 0, 0, 0);
    else
        rc.DeflateRect(0, m_nGripper, 0, 0);

    // set position for the "x" (hide bar) button
    CPoint ptOrgBtn;
    if (bHorz)
        ptOrgBtn = CPoint(rc.left - 14, rc.top);
    else
        ptOrgBtn = CPoint(rc.right - 12, rc.top - 14);

    m_btnHide.Move(ptOrgBtn - rcBar.TopLeft());

    *pRect = rc;
}

void CCoolBar::NcPaintGripper(CDC* pDC, LPCRECT pRect)
{
    _ASSERTE( pDC != NULL && pRect != NULL );
    
	if( IsFloating() )
	{
		return;
	}

	if( m_bHasGripper )
	{
		// compute the caption rectangle
		BOOL  bHorz  = IsHorzDocked();
		CRect rcGrip = pRect;
		CRect rcBtn  = m_btnHide.GetRect();
		if (bHorz)
		{   // right side gripper
			rcGrip.left -= m_nGripper  + 1;
			rcGrip.right = rcGrip.left + m_nGripper - 1;
			rcGrip.top   = rcBtn.top;
		}
		else
		{   // gripper at top
			rcGrip.top   -= m_nGripper + 1;
			rcGrip.bottom = rcGrip.top + m_nGripper - 1;
			rcGrip.right  = rcBtn.right ;
		}
		rcGrip.InflateRect(bHorz ? 1 : 0, bHorz ? 0 : 1);
		
		// draw the caption background
		COLORREF clrCptn = m_clrGripperBack;

		// 非活动状态
		//if( !m_bActive )
		//{
		//	clrCptn = m_clrGripperBack / 2;
		//}

		pDC->FillSolidRect(&rcGrip, clrCptn); 

		// draw the caption text - first select a font
		CFont font;
		int ppi = pDC->GetDeviceCaps(LOGPIXELSX);
		int pointsize = MulDiv(96, 96, ppi); // 8.5 points at 96 ppi
		LOGFONT lf;
		::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
		
		BOOL bFont = font.CreatePointFont(pointsize, lf.lfFaceName);
		if (bFont)
		{
			// get the text color
			COLORREF clrCptnText = m_bActive ? m_clrGripperFore : m_clrGripperFore / 3;
          			
			int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
			COLORREF clrOldText = pDC->SetTextColor(clrCptnText);
			
			if (bHorz)
			{
				// rotate text 90 degrees CCW if horizontally docked
				LOGFONT _lf;
				font.GetLogFont(&_lf);
				font.DeleteObject();
				_lf.lfEscapement = 900;
				font.CreateFontIndirect(&_lf);
			}
			
			CFont* pOldFont = pDC->SelectObject(&font);
			
			CPoint ptOrg = bHorz ?
				           CPoint(rcGrip.left - 2, rcGrip.bottom - 2) :
                           CPoint(rcGrip.left + 2, rcGrip.top - 2);
			
			pDC->ExtTextOut(ptOrg.x, ptOrg.y, ETO_CLIPPED, rcGrip, m_strGripperText, NULL);
			
			pDC->SelectObject(pOldFont);
			pDC->SetBkMode(nOldBkMode);
			pDC->SetTextColor(clrOldText);
			font.DeleteObject();
		}
	}
	else
	{
		CRect gripper = pRect;
		CRect rcbtn   = m_btnHide.GetRect();
		BOOL  bHorz   = IsHorzDocked();
		
		gripper.DeflateRect(1, 1);
		if (bHorz)
		{   // gripper at left
			gripper.left -= m_nGripper;
			gripper.right = gripper.left + 3;
			gripper.top = rcbtn.bottom + 3;
		}
		else
		{   // gripper at top
			gripper.top -= m_nGripper;
			gripper.bottom = gripper.top + 3;
			gripper.right = rcbtn.left - 3;
		}
		
		pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
			::GetSysColor(COLOR_BTNSHADOW));
		
		gripper.OffsetRect(bHorz ? 3 : 0, bHorz ? 0 : 3);
		
		pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
			::GetSysColor(COLOR_BTNSHADOW));
	}

	// draw the button
	m_btnHide.Paint(pDC);
}

UINT CCoolBar::OnNcHitTest(CPoint point)
{
    UINT nRet = CSizingControlBar::OnNcHitTest(point);
    if(nRet != HTCLIENT)
        return nRet;

	CRect rcBar;
    GetWindowRect(rcBar);

    CRect rc = m_btnHide.GetRect();
    rc.OffsetRect(rcBar.TopLeft());

    if (rc.PtInRect(point))
        return HTCLOSE;

    return HTCLIENT;
}

/////////////////////////////////////////////////////////////////////////
// CCoolBar implementation helpers

void CCoolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    UNUSED_ALWAYS(bDisableIfNoHndler);
    UNUSED_ALWAYS(pTarget);

	if( IsFloating() )
	{
		return;
	}

    BOOL bNeedPaint = FALSE;

    CPoint pt;
    ::GetCursorPos(&pt);
    BOOL bHit         = (OnNcHitTest(pt) == HTCLOSE);
    BOOL bLButtonDown = (::GetKeyState(VK_LBUTTON) < 0);

    BOOL bWasPushed     = m_btnHide.m_bPushed;
    m_btnHide.m_bPushed = bHit && bLButtonDown;

    BOOL bWasRaised     = m_btnHide.m_bRaised;
    m_btnHide.m_bRaised = bHit && !bLButtonDown;

	CWnd * pFocus   = GetFocus();
    BOOL bActiveOld = m_bActive;

    m_bActive = (pFocus->GetSafeHwnd() && IsChild(pFocus));
    if (m_bActive != bActiveOld)
        bNeedPaint = TRUE;

    bNeedPaint |= ( (m_btnHide.m_bPushed ^ bWasPushed) ||
                    (m_btnHide.m_bRaised ^ bWasRaised) ); //lint !e514

    if(bNeedPaint)
	{
        SendMessage(WM_NCPAINT);
	}
}

BOOL CCoolBar::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	_ASSERTE( ::IsWindow(m_hNotifyWnd) );

	// 将WM_NOTIFY消息转交给消息通知窗口处理
	::SendMessage(m_hNotifyWnd, WM_NOTIFY, wParam, lParam);

	*pResult = 0;
	
	return TRUE;
}

void CCoolBar::SetNotifyWindow(HWND hNotifyWnd)
{
	_ASSERTE( ::IsWindow(m_hNotifyWnd) );

	m_hNotifyWnd = hNotifyWnd;
}

void CCoolBar::SetGripperInfo( LPCTSTR pszText, COLORREF clrForeGrnd, COLORREF clrBackGrnd )
{
	if( pszText == NULL )
	{
		m_strGripperText.Empty();
	}
	else
	{
		m_strGripperText = pszText;
	}

	m_clrGripperFore = clrForeGrnd;
	m_clrGripperBack = clrBackGrnd;
}


