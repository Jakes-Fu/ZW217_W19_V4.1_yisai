// 
// FileName: SizingControlBar.cpp
// anli.wei 整理完成
// Copyright (C) 2004 Spreadtrum Corporation
// All rights reserved.

#include "stdafx.h"
#include "SizingControlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
// CSizingControlBar

IMPLEMENT_DYNAMIC(CSizingControlBar, CControlBar);

CSizingControlBar::CSizingControlBar() : m_nMargin(4)
{
	m_bTracking        = FALSE;
    m_bKeepSize        = FALSE;
    m_bParentSizing    = FALSE;
	m_bDragShowContent = FALSE;

	m_nDockBarID   = 0;
	m_dwSCBStyle   = 0;

    m_sizeHorz     = CSize(200, 200);
	m_sizeVert     = CSize(200, 200);
    m_sizeFloat    = CSize(200, 200);
	m_sizeMinHorz  = CSize(64, 32);
    m_sizeMinVert  = CSize(32, 32);
    m_sizeMinFloat = CSize(64, 32);   

	m_hHorzCursor  = NULL;
	m_hVertCursor  = NULL;
	m_pActiveWnd   = NULL;

	m_nTrackPosMin = 32;
	m_nTrackPosMax = 32;
	m_nTrackPosOld  = 0;
	m_nTrackEdgeOffset = 0;
	m_htEdge = 0;
}

CSizingControlBar::~CSizingControlBar()
{
	m_hHorzCursor  = NULL;
	m_hVertCursor  = NULL;
	m_pActiveWnd   = NULL;
}

BEGIN_MESSAGE_MAP(CSizingControlBar, CControlBar)
    //{{AFX_MSG_MAP(CSizingControlBar)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_NCPAINT()
    ON_WM_NCCALCSIZE()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_CAPTURECHANGED()
    ON_WM_SETTINGCHANGE()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_NCMOUSEMOVE()
    ON_WM_NCHITTEST()
    ON_WM_CLOSE()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSizingControlBar::Create( LPCTSTR lpszWindowName, CWnd* pParentWnd, 
							    const CSize &sizeDefault, UINT nID, 
								BOOL /* bHasGripper  = FALSE */,
								DWORD dwStyle     /* = WS_CHILD | WS_VISIBLE | CBRS_TOP*/ )
{
    m_sizeHorz = m_sizeVert = m_sizeFloat = sizeDefault;

    return Create(lpszWindowName, pParentWnd, nID, dwStyle);
}

BOOL CSizingControlBar::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd, 
							   UINT nID, DWORD dwStyle)
{
    // must have a parent
	_ASSERTE( pParentWnd != NULL );
    ASSERT_VALID( pParentWnd );

    // cannot be both fixed and dynamic
    // (CBRS_SIZE_DYNAMIC is used for resizng when floating)
    _ASSERTE( !((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)) );

    m_dwStyle = dwStyle & CBRS_ALL; // save the control bar styles

    // register and create the window - skip CControlBar::Create()
    CString strClassName = ::AfxRegisterWndClass( CS_DBLCLKS,
                                                  ::LoadCursor(NULL, IDC_ARROW),
                                                  ::GetSysColorBrush(COLOR_BTNFACE), 
												  0);

    dwStyle &= ~CBRS_ALL;       // keep only the generic window styles
    dwStyle |= WS_CLIPCHILDREN; // prevents flashing
   
    if( !CWnd::Create(strClassName, lpszWindowName, dwStyle,
                      CRect(0, 0, 0, 0), pParentWnd, nID) )
	{
        return FALSE;
	}

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// CSizingControlBar message handlers

int CSizingControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CControlBar::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_dwSCBStyle |= SCBS_SHOWEDGES;

    return 0;
}

const BOOL CSizingControlBar::IsFloating() const
{
    return ( !IsHorzDocked() && !IsVertDocked() );
}

const BOOL CSizingControlBar::IsHorzDocked() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_TOP ||
            m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);
}

const BOOL CSizingControlBar::IsVertDocked() const
{
    return ( m_nDockBarID == AFX_IDW_DOCKBAR_LEFT ||
             m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT );
}

const BOOL CSizingControlBar::IsSideTracking() const
{
    // don't call this when not tracking
    _ASSERTE( m_bTracking && !IsFloating() );

    return ( m_htEdge == HTLEFT || m_htEdge == HTRIGHT) ? IsHorzDocked() : IsVertDocked();
}

CSize CSizingControlBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
    if (bStretch) // the bar is stretched (is not the child of a dockbar)
	{
        if (bHorz)
            return CSize(32767, m_sizeHorz.cy);
        else
            return CSize(m_sizeVert.cx, 32767);
	}
    // dirty cast - we need access to protected CDockBar members
    CSCBDockBar* pDockBar = (CSCBDockBar*) m_pDockBar;

    // force imediate RecalcDelayShow() for all sizing bars on the row
    // with delayShow/delayHide flags set to avoid IsVisible() problems
    CSCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);

    AFX_SIZEPARENTPARAMS layout;
    layout.hDWP = pDockBar->m_bLayoutQuery ?
                  NULL : ::BeginDeferWindowPos(arrSCBars.GetSize());
    for (int i = 0; i < arrSCBars.GetSize(); i++)
	{
        if (arrSCBars[i]->m_nStateFlags & (delayHide|delayShow))
            arrSCBars[i]->RecalcDelayShow(&layout);
	}
    if (layout.hDWP != NULL)
        ::EndDeferWindowPos(layout.hDWP);

    // get available length
    CRect rc = pDockBar->m_rectLayout;
    if (rc.IsRectEmpty())
        m_pDockSite->GetClientRect(&rc);

    int nLengthTotal = bHorz ? rc.Width() + 2 : rc.Height() - 2;

    if (IsVisible() && !IsFloating() &&
        m_bParentSizing && arrSCBars[0] == this)
	{
        if (NegotiateSpace(nLengthTotal, (bHorz != FALSE)))
            AlignControlBars();
	}

    m_bParentSizing = FALSE;

    if (bHorz)
        return CSize(max(m_sizeMinHorz.cx, m_sizeHorz.cx),
                     max(m_sizeMinHorz.cy, m_sizeHorz.cy));

    return CSize(max(m_sizeMinVert.cx, m_sizeVert.cx),
                 max(m_sizeMinVert.cy, m_sizeVert.cy));
}

CSize CSizingControlBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
    if (dwMode & (LM_HORZDOCK | LM_VERTDOCK)) // docked ?
    {
        if (nLength == -1)
            m_bParentSizing = TRUE;

        return CControlBar::CalcDynamicLayout(nLength, dwMode);
    }

    if (dwMode & LM_MRUWIDTH) return m_sizeFloat;
    if (dwMode & LM_COMMIT)   return m_sizeFloat; // already committed

    // check for dialgonal resizing hit test
    int nHitTest = m_pDockContext->m_nHitTest;

    if (IsFloating() &&
        (nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMLEFT ||
        nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMRIGHT))
    {
        CPoint ptCursor;
        ::GetCursorPos(&ptCursor);

        CRect rcFrame, rcBar;
        GetParentFrame()->GetWindowRect(&rcFrame);
        GetWindowRect(&rcBar);
        
        if (nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMLEFT)
        {
            m_sizeFloat.cx = rcFrame.left + rcBar.Width() - ptCursor.x;
            m_pDockContext->m_rectFrameDragHorz.left = 
                min(ptCursor.x, rcFrame.left + rcBar.Width() - m_sizeMinFloat.cx);
        }

        if (nHitTest == HTTOPLEFT || nHitTest == HTTOPRIGHT)
        {
            m_sizeFloat.cy = rcFrame.top + rcBar.Height() - ptCursor.y;
            m_pDockContext->m_rectFrameDragHorz.top =
                min(ptCursor.y, rcFrame.top + rcBar.Height() - m_sizeMinFloat.cy);
        }

        if (nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMRIGHT)
            m_sizeFloat.cx = rcBar.Width() + ptCursor.x - rcFrame.right;

        if (nHitTest == HTBOTTOMLEFT || nHitTest == HTBOTTOMRIGHT)
            m_sizeFloat.cy = rcBar.Height() + ptCursor.y - rcFrame.bottom;
    }
    else
	{
        ((dwMode & LM_LENGTHY) ? m_sizeFloat.cy : m_sizeFloat.cx) = nLength;
	}

    m_sizeFloat.cx = max(m_sizeFloat.cx, m_sizeMinFloat.cx);
    m_sizeFloat.cy = max(m_sizeFloat.cy, m_sizeMinFloat.cy);

    return m_sizeFloat;
}

void CSizingControlBar::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
    // force non-client recalc if moved or resized
    lpwndpos->flags |= SWP_FRAMECHANGED;

    CControlBar::OnWindowPosChanging(lpwndpos);

    // find on which side are we docked
    m_nDockBarID = GetParent()->GetDlgCtrlID();

    if (!IsFloating())
	{
        if (lpwndpos->flags & SWP_SHOWWINDOW)
		{
            m_bKeepSize = TRUE;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//
void CSizingControlBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    if(m_pDockBar != NULL)
    {
        // start the drag
        _ASSERTE(m_pDockContext != NULL);

        ClientToScreen(&point);
        m_pDockContext->StartDrag(point);
    }
    else
	{
        CWnd::OnLButtonDown(nFlags, point);
	}
}

void CSizingControlBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (m_pDockBar != NULL)
    {
        // toggle docking
        _ASSERTE(m_pDockContext != NULL);
        m_pDockContext->ToggleDocking();
    }
    else
	{
        CWnd::OnLButtonDblClk(nFlags, point);
	}
}

void CSizingControlBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
    UNUSED_ALWAYS(point);

    if (m_bTracking || IsFloating())
	{
        return;
	}

    if ((nHitTest >= HTSIZEFIRST) && (nHitTest <= HTSIZELAST))
	{
		StartTracking(nHitTest, point); // sizing edge hit
	}
}

void CSizingControlBar::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bTracking)
	{
        StopTracking();
	}
	else
	{
		CControlBar::OnLButtonUp(nFlags, point);
	}
}

void CSizingControlBar::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (m_bTracking)
        StopTracking();

    CControlBar::OnRButtonDown(nFlags, point);
}

void CSizingControlBar::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bTracking)
    {
        CPoint ptScreen = point;
        ClientToScreen(&ptScreen);

        OnTrackUpdateSize(ptScreen);
    }

    CControlBar::OnMouseMove(nFlags, point);
}

void CSizingControlBar::OnCaptureChanged(CWnd *pWnd)
{
    if (m_bTracking && (pWnd != this))
        StopTracking();

    CControlBar::OnCaptureChanged(pWnd);
}

void CSizingControlBar::OnNcCalcSize(BOOL bCalcValidRects,
                                     NCCALCSIZE_PARAMS FAR* lpncsp)
{
    UNUSED_ALWAYS(bCalcValidRects);

    // compute the the client area
    m_dwSCBStyle &= ~SCBS_EDGEALL;

    // add resizing edges between bars on the same row
    if (!IsFloating() && m_pDockBar != NULL)
    {
        CSCBArray arrSCBars;
        int nThis;
        GetRowSizingBars(arrSCBars, nThis);

        BOOL bHorz = IsHorzDocked();
        if (nThis > 0)
            m_dwSCBStyle |= bHorz ? SCBS_EDGELEFT : SCBS_EDGETOP;

        if (nThis < arrSCBars.GetUpperBound())
            m_dwSCBStyle |= bHorz ? SCBS_EDGERIGHT : SCBS_EDGEBOTTOM;
    }

    NcCalcClient(&lpncsp->rgrc[0], m_nDockBarID);
}

void CSizingControlBar::NcPaintGripper(CDC* pDC, LPCRECT pRect)
{
    UNUSED_ALWAYS(pDC);
    UNUSED_ALWAYS(pRect);
}

void CSizingControlBar::NcCalcClient(LPRECT pRect, UINT nDockBarID)
{
	_ASSERTE( pRect != NULL );

    CRect rc(pRect);

    rc.DeflateRect(3, 5, 3, 3);
    if (nDockBarID != AFX_IDW_DOCKBAR_FLOAT)
        rc.DeflateRect(2, 0, 2, 2);

    switch(nDockBarID)
    {
    case AFX_IDW_DOCKBAR_TOP:
        m_dwSCBStyle |= SCBS_EDGEBOTTOM;
        break;
    case AFX_IDW_DOCKBAR_BOTTOM:
        m_dwSCBStyle |= SCBS_EDGETOP;
        break;
    case AFX_IDW_DOCKBAR_LEFT:
        m_dwSCBStyle |= SCBS_EDGERIGHT;
        break;
    case AFX_IDW_DOCKBAR_RIGHT:
        m_dwSCBStyle |= SCBS_EDGELEFT;
        break;
    }

    // make room for edges only if they will be painted
    if (m_dwSCBStyle & SCBS_SHOWEDGES)
	{
        rc.DeflateRect( (m_dwSCBStyle & SCBS_EDGELEFT)   ? m_nMargin : 0,
                        (m_dwSCBStyle & SCBS_EDGETOP)    ? m_nMargin : 0,
                        (m_dwSCBStyle & SCBS_EDGERIGHT)  ? m_nMargin : 0,
                        (m_dwSCBStyle & SCBS_EDGEBOTTOM) ? m_nMargin : 0 );
	}
    *pRect = rc;
}

void CSizingControlBar::OnNcPaint()
{
    // get window DC that is clipped to the non-client area
    CWindowDC dc(this);

    CRect rcClient, rcBar;
    GetClientRect(&rcClient);
    ClientToScreen(&rcClient);
    GetWindowRect(&rcBar);

    rcClient.OffsetRect( -rcBar.TopLeft() );
    rcBar.OffsetRect( -rcBar.TopLeft() );

    CDC dcMem;
    VERIFY( dcMem.CreateCompatibleDC(&dc) );
    
    CBitmap bmp;
    VERIFY( bmp.CreateCompatibleBitmap(&dc, rcBar.Width(), rcBar.Height()) );
    CBitmap* pOldBm = dcMem.SelectObject(&bmp);

    // draw borders in non-client area
    CRect rcDraw = rcBar;
    DrawBorders(&dcMem, rcDraw);

    // erase the NC background
    dcMem.FillRect(rcDraw, 
		CBrush::FromHandle((HBRUSH)GetClassLong(m_hWnd, GCL_HBRBACKGROUND)));

    if (m_dwSCBStyle & SCBS_SHOWEDGES)
    {
        CRect rcEdge; // paint the sizing edges
		UINT  uHitTest;
        for (int i = 0; i < 4; i++)
		{
			uHitTest = GetEdgeHTCode(i);
            if (GetEdgeRect(rcBar, uHitTest, rcEdge))
			{
				if( HTRIGHT == uHitTest || HTBOTTOM == uHitTest )
				{
					dcMem.Draw3dRect( rcEdge, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                                ::GetSysColor(COLOR_3DDKSHADOW) );
				}
				else
				{
					if( HTLEFT == uHitTest ) rcEdge.right++;
					if( HTTOP  == uHitTest ) rcEdge.bottom++;

					dcMem.DrawEdge( rcEdge, EDGE_RAISED, BF_RECT);
				}
			}
		}
    }

	NcPaintGripper(&dcMem, rcClient);

    // client area is not our bussiness :)
    dc.IntersectClipRect(&rcBar);
    dc.ExcludeClipRect(&rcClient);

    dc.BitBlt(0, 0, rcBar.Width(), rcBar.Height(), &dcMem, 0, 0, SRCCOPY);

    ReleaseDC(&dc);

    dcMem.SelectObject(pOldBm);
    bmp.DeleteObject();
    dcMem.DeleteDC();
}

void CSizingControlBar::OnPaint()
{
    // overridden to skip border painting based on clientrect
	// don't delete or annotate CPaintDC dc(this); !!!!! anli.wei
	CPaintDC dc(this);
}

void CSizingControlBar::OnSize(UINT nType, int cx, int cy) 
{
	UNUSED_ALWAYS(nType);

	// TODO: Add your message handler code here
	if( m_pActiveWnd != NULL )
	{
		m_pActiveWnd->MoveWindow(0, 0, cx, cy);
	}
}

UINT CSizingControlBar::OnNcHitTest(CPoint point)
{
    CRect rcBar, rcEdge;
    GetWindowRect(rcBar);

    if (!IsFloating())
	{
		UINT uHitTest = 0;
        for (int i = 0; i < 4; i++)
		{
			uHitTest = GetEdgeHTCode(i);
			if (GetEdgeRect(rcBar, uHitTest, rcEdge))
			{
                if (rcEdge.PtInRect(point))
				{
					if( m_hHorzCursor != NULL && 
						(HTLEFT == uHitTest || HTRIGHT == uHitTest) )
					{
						SetCursor(m_hHorzCursor);
					}
					if( m_hVertCursor != NULL &&
						(HTTOP == uHitTest || HTBOTTOM == uHitTest) )
					{
						SetCursor(m_hVertCursor);
					}
                    return uHitTest;
				}
			}
		}
	}
    return HTCLIENT;
}

void CSizingControlBar::OnClose()
{
    // do nothing: protection against accidentally destruction by the
    //   child control (i.e. if user hits Esc in a child editctrl)
}

/////////////////////////////////////////////////////////////////////////
// CSizingControlBar implementation helpers

void CSizingControlBar::StartTracking(UINT nHitTest, const CPoint &pt)
{
	m_pDockSite->LockWindowUpdate();

    SetCapture();

    // make sure no updates are pending
    if(!m_bDragShowContent)
	{
        RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);
	}

    m_bTracking = TRUE;
    m_htEdge    = nHitTest;

    BOOL bHorz         = IsHorzDocked();
    BOOL bHorzTracking = ( m_htEdge == HTLEFT || m_htEdge == HTRIGHT );

    m_nTrackPosOld = bHorzTracking ? pt.x : pt.y;

    CRect rcBar, rcEdge;
    GetWindowRect(&rcBar);
    GetEdgeRect(rcBar, m_htEdge, rcEdge);

	CPoint ptCenter   = rcEdge.CenterPoint(); //@x
    m_nTrackEdgeOffset = m_nTrackPosOld - (bHorzTracking ? ptCenter.x : ptCenter.y);
    
    CSCBArray arrSCBars;
    int nThis;
    GetRowSizingBars(arrSCBars, nThis);

    m_nTrackPosMin = m_nTrackPosMax = m_nTrackPosOld;
    if(!IsSideTracking())
    {
        // calc minwidth as the max minwidth of the sizing bars on row
        int nMinWidth = bHorz ? m_sizeMinHorz.cy : m_sizeMinVert.cx;
        for (int i = 0; i < arrSCBars.GetSize(); i++)
		{
            nMinWidth = max(nMinWidth, bHorz ? arrSCBars[i]->m_sizeMinHorz.cy :
                                       arrSCBars[i]->m_sizeMinVert.cx);
		}

        int nExcessWidth = (bHorz ? m_sizeHorz.cy : m_sizeVert.cx) - nMinWidth;

        // the control bar cannot grow with more than the width of
        // remaining client area of the mainframe
        CRect rcT;
        m_pDockSite->RepositionBars(0, 0xFFFF, AFX_IDW_PANE_FIRST,
                                    reposQuery, &rcT, NULL, TRUE);
        int nMaxWidth = bHorz ? rcT.Height() - 2 : rcT.Width() - 2;

        BOOL bTopOrLeft = ( m_htEdge == HTTOP || m_htEdge == HTLEFT );

        m_nTrackPosMin -= bTopOrLeft ? nMaxWidth    : nExcessWidth;
        m_nTrackPosMax += bTopOrLeft ? nExcessWidth : nMaxWidth;
    }
    else
    {
        // side tracking:
        // max size is the actual size plus the amount the other
        // sizing bars can be decreased until they reach their minsize
        if (m_htEdge == HTBOTTOM || m_htEdge == HTRIGHT)
            nThis++;

		CSizingControlBar * pBar = NULL;
        for (int i = 0; i < arrSCBars.GetSize(); i++)
        {
			pBar = arrSCBars[i];
            int nExcessWidth = bHorz ? 
                               pBar->m_sizeHorz.cx - pBar->m_sizeMinHorz.cx :
                               pBar->m_sizeVert.cy - pBar->m_sizeMinVert.cy;

            if (i < nThis)
                m_nTrackPosMin -= nExcessWidth;
            else
                m_nTrackPosMax += nExcessWidth;
        }
    }

    OnInvertTracker(); // draw tracker
}

void CSizingControlBar::StopTracking()
{
    OnInvertTracker(); // erase tracker

    m_bTracking = FALSE;
    ReleaseCapture();

    m_pDockSite->DelayRecalcLayout();

	m_pDockSite->UnlockWindowUpdate();
}

void CSizingControlBar::OnTrackUpdateSize(CPoint& point)
{
    _ASSERTE( !IsFloating() );

    BOOL bHorzTrack = ( m_htEdge == HTLEFT || m_htEdge == HTRIGHT );

    int nTrackPos = bHorzTrack ? point.x : point.y;
    nTrackPos     = max(m_nTrackPosMin, min(m_nTrackPosMax, nTrackPos));

    int nDelta    = nTrackPos - m_nTrackPosOld;

    if (nDelta == 0)
        return; // no pos change

    OnInvertTracker(); // erase tracker

    m_nTrackPosOld = nTrackPos;
    
    BOOL bHorz = IsHorzDocked();

    CSize sizeNew = bHorz ? m_sizeHorz : m_sizeVert;
    switch (m_htEdge)
    {
    case HTLEFT:    sizeNew -= CSize(nDelta, 0); break;
    case HTTOP:     sizeNew -= CSize(0, nDelta); break;
    case HTRIGHT:   sizeNew += CSize(nDelta, 0); break;
    case HTBOTTOM:  sizeNew += CSize(0, nDelta); break;
    }

    CSCBArray arrSCBars;
    int nThis;
    GetRowSizingBars(arrSCBars, nThis);

    if(!IsSideTracking())
	{
		CSizingControlBar* pBar = NULL;
        for (int i = 0; i < arrSCBars.GetSize(); i++)
        {
            pBar = arrSCBars[i];
            // make same width (or height)
            (bHorz ? pBar->m_sizeHorz.cy : pBar->m_sizeVert.cx) =
             bHorz ? sizeNew.cy : sizeNew.cx;
        }
	}
    else
    {
        int nGrowingBar = nThis;
        BOOL bBefore    = ( m_htEdge == HTTOP || m_htEdge == HTLEFT );

        if (bBefore && nDelta > 0)
            nGrowingBar--;

        if (!bBefore && nDelta < 0)
            nGrowingBar++;

        if (nGrowingBar != nThis)
            bBefore = !bBefore;

        // nGrowing is growing
        nDelta = abs(nDelta);
        CSizingControlBar* pBar = arrSCBars[nGrowingBar];
        (bHorz ? pBar->m_sizeHorz.cx : pBar->m_sizeVert.cy) += nDelta;

        // the others are shrinking
        int nFirst = bBefore ? nGrowingBar - 1 : nGrowingBar + 1;
        int nLimit = bBefore ? -1 : arrSCBars.GetSize();

        for (int i = nFirst; nDelta != 0 && i != nLimit; i += (bBefore ? -1 : 1))
        {
            pBar = arrSCBars[i];
                
            int nDeltaT = min(nDelta,
                              (bHorz ? pBar->m_sizeHorz.cx    : pBar->m_sizeVert.cy) -
                              (bHorz ? pBar->m_sizeMinHorz.cx : pBar->m_sizeMinVert.cy));

            (bHorz ? pBar->m_sizeHorz.cx : pBar->m_sizeVert.cy) -= nDeltaT;
            nDelta -= nDeltaT;
        }
    }

    OnInvertTracker(); // redraw tracker at new pos

    if (m_bDragShowContent)
	{
        m_pDockSite->DelayRecalcLayout();
	}
}

void CSizingControlBar::OnInvertTracker()
{
    _ASSERTE( m_bTracking );

    if (m_bDragShowContent)
        return; // don't show tracker if DragFullWindows is on

    BOOL bHorz = IsHorzDocked();
    CRect rc, rcBar, rcDock, rcFrame;
    GetWindowRect(&rcBar);

    m_pDockBar->GetWindowRect(&rcDock);
    m_pDockSite->GetWindowRect(&rcFrame);

    VERIFY( GetEdgeRect(rcBar, m_htEdge, rc) );
    if (!IsSideTracking())
	{
        rc = bHorz ? 
             CRect(rcDock.left + 1, rc.top, rcDock.right - 1, rc.bottom) :
             CRect(rc.left, rcDock.top + 1, rc.right, rcDock.bottom - 1);
	}

    BOOL bHorzTracking = ( m_htEdge == HTLEFT || m_htEdge == HTRIGHT );

    int nOfs = m_nTrackPosOld - m_nTrackEdgeOffset;
    nOfs -= bHorzTracking ? rc.CenterPoint().x : rc.CenterPoint().y;

    rc.OffsetRect(bHorzTracking ? nOfs : 0, bHorzTracking ? 0 : nOfs);
    rc.OffsetRect( -rcFrame.TopLeft() );

    CDC    * pDC       = m_pDockSite->GetDCEx(NULL, 
		                      DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
    CBrush * pBrush    = CDC::GetHalftoneBrush();
    CBrush * pBrushOld = pDC->SelectObject(pBrush);

   pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATINVERT);
    
    pDC->SelectObject(pBrushOld);
    m_pDockSite->ReleaseDC(pDC);
}

BOOL CSizingControlBar::GetEdgeRect(const CRect &rcWnd, UINT nHitTest, CRect& rcEdge)
{
    rcEdge = rcWnd;
    if (m_dwSCBStyle & SCBS_SHOWEDGES)
	{
        rcEdge.DeflateRect(1, 1);
	}

    BOOL bHorz = IsHorzDocked();

    switch (nHitTest)
    {
    case HTLEFT:
        if ( !(m_dwSCBStyle & SCBS_EDGELEFT) ) return FALSE;

        rcEdge.right = rcEdge.left + m_nMargin;
        rcEdge.DeflateRect(0, bHorz ? m_nMargin: 0);
        break;
    case HTTOP:
        if ( !(m_dwSCBStyle & SCBS_EDGETOP) ) return FALSE;

        rcEdge.bottom = rcEdge.top + m_nMargin;
        rcEdge.DeflateRect(bHorz ? 0 : m_nMargin, 0);
        break;
    case HTRIGHT:
        if ( !(m_dwSCBStyle & SCBS_EDGERIGHT) ) return FALSE;

        rcEdge.left = rcEdge.right - m_nMargin;
        rcEdge.DeflateRect(0, bHorz ? m_nMargin: 0);
        break;
    case HTBOTTOM:
        if ( !(m_dwSCBStyle & SCBS_EDGEBOTTOM) ) return FALSE;

        rcEdge.top = rcEdge.bottom - m_nMargin;
        rcEdge.DeflateRect(bHorz ? 0 : m_nMargin, 0);
        break;
    default:
        _ASSERTE(FALSE); // invalid hit test code
    }

    return TRUE;
}

UINT CSizingControlBar::GetEdgeHTCode(int nEdge)
{
	switch( nEdge )
	{
    case 0: return HTLEFT;
    case 1: return HTTOP;
    case 2: return HTRIGHT;
    case 3: return HTBOTTOM;
	default:
		_ASSERTE(FALSE); // invalid edge code
	}
    return HTNOWHERE;
}

void CSizingControlBar::GetRowInfo(int& nFirst, int& nLast, int& nThis)
{
	_ASSERTE( m_pDockBar != NULL );
    ASSERT_VALID(m_pDockBar); // verify bounds

    nThis = m_pDockBar->FindBar(this);
    _ASSERTE(nThis != -1);

	int i = -1;
    // find the first and the last bar in row
    for (nFirst = -1, i = nThis - 1; i >= 0 && nFirst == -1; i--)
	{
        if (m_pDockBar->m_arrBars[i] == NULL)
        {
			nFirst = i + 1;
		}
	}

	int nBars = m_pDockBar->m_arrBars.GetSize();
    for (nLast = -1, i = nThis + 1; i < nBars && nLast == -1; i++)
	{
		if (m_pDockBar->m_arrBars[i] == NULL)
		{
            nLast = i - 1;
		}
	}

    _ASSERTE( (nLast != -1) && (nFirst != -1) );
}

void CSizingControlBar::GetRowSizingBars(CSCBArray& arrSCBars)
{
    int nThis; // dummy
    GetRowSizingBars(arrSCBars, nThis);
}

void CSizingControlBar::GetRowSizingBars(CSCBArray& arrSCBars, int& nThis)
{
    arrSCBars.RemoveAll();

    int nFirst, nLast, nThisTmp;
    GetRowInfo(nFirst, nLast, nThisTmp);

    nThis = -1;
	CSizingControlBar* pBar = NULL;
    for (int i = nFirst; i <= nLast; i++)
    {
        pBar = (CSizingControlBar*) m_pDockBar->m_arrBars[i];
        if(HIWORD(pBar) == 0 || !pBar->IsVisible() ) 
		{
			continue; // placeholder
		}
       
        if (pBar->IsKindOf(RUNTIME_CLASS(CSizingControlBar)))
        {
            if (pBar == this)
			{
                nThis = arrSCBars.GetSize();
			}

            arrSCBars.Add(pBar);
        }
    }
}

BOOL CSizingControlBar::NegotiateSpace(int nLengthTotal, BOOL bHorz)
{
    if( bHorz != IsHorzDocked() )
		return FALSE;

    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);

    int nLengthAvail  = nLengthTotal;
    int nLengthActual = 0;
    int nLengthMin    = 2;
    int nWidthMax     = 0;

    CSizingControlBar* pBar = NULL;

    for (int i = nFirst; i <= nLast; i++)
    {
        pBar = (CSizingControlBar*) m_pDockBar->m_arrBars[i];
        if( HIWORD(pBar) == 0 || !pBar->IsVisible() ) 
		{
			continue; // placeholder
		}
       
        BOOL bIsSizingBar = pBar->IsKindOf(RUNTIME_CLASS(CSizingControlBar));

        int nLengthBar; // minimum length of the bar
        if (bIsSizingBar)
		{
            nLengthBar = bHorz ? pBar->m_sizeMinHorz.cx - 2 : pBar->m_sizeMinVert.cy - 2;
		}
        else
        {
            CRect rcBar;
            pBar->GetWindowRect(&rcBar);
            nLengthBar = bHorz ? rcBar.Width() - 2 : rcBar.Height() - 2;
        }

        nLengthMin += nLengthBar;
        if (nLengthMin > nLengthTotal)
        {
            // split the row after fixed bar
            if (i < nThis)
            {
                m_pDockBar->m_arrBars.InsertAt(i + 1, (CControlBar*)NULL);
                return FALSE;
            }
            
            // only this sizebar remains on the row, adjust it to minsize
            if (i == nThis)
            {
                if (bHorz)
                    m_sizeHorz.cx = m_sizeMinHorz.cx;
                else
                    m_sizeVert.cy = m_sizeMinVert.cy;

                return TRUE; // the dockbar will split the row for us
            }

            // we have enough bars - go negotiate with them
            m_pDockBar->m_arrBars.InsertAt(i, (CControlBar*)NULL);
            nLast = i - 1;
            break;
        }

        if (bIsSizingBar)
        {
            nLengthActual += bHorz ? pBar->m_sizeHorz.cx - 2 : pBar->m_sizeVert.cy - 2;
            nWidthMax = max(nWidthMax, bHorz ? pBar->m_sizeHorz.cy : pBar->m_sizeVert.cx);
        }
        else
		{
            nLengthAvail -= nLengthBar;
		}
    }

    CSCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);

    int nNumBars = arrSCBars.GetSize();
    int nDelta   = nLengthAvail - nLengthActual;

    // return faster when there is only one sizing bar per row (this one)
    if (nNumBars == 1)
    {
        _ASSERTE( arrSCBars[0] == this );

        if (nDelta == 0)  return TRUE;
        
        m_bKeepSize = FALSE;
        (bHorz ? m_sizeHorz.cx : m_sizeVert.cy) += nDelta;

        return TRUE;
    }

    // make all the bars the same width
    for(i = 0; i < nNumBars; i++)
	{
        if (bHorz)
            arrSCBars[i]->m_sizeHorz.cy = nWidthMax;
        else
            arrSCBars[i]->m_sizeVert.cx = nWidthMax;
	}
    // distribute the difference between the bars,
    // but don't shrink them below their minsizes
    for(; nDelta != 0; )
    {
        int nDeltaOld = nDelta;
        for (i = 0; i < nNumBars; i++)
        {
            pBar = arrSCBars[i];
            int nLMin = bHorz ? pBar->m_sizeMinHorz.cx : pBar->m_sizeMinVert.cy;
            int nL    = bHorz ? pBar->m_sizeHorz.cx : pBar->m_sizeVert.cy;

			// already at min length or wants to keep its size
            if( (nL == nLMin) && (nDelta < 0) || pBar->m_bKeepSize) 
                continue;

            // sign of nDelta
            int nDelta2 = (nDelta == 0) ? 0 : ((nDelta < 0) ? -1 : 1);

            (bHorz ? pBar->m_sizeHorz.cx : pBar->m_sizeVert.cy) += nDelta2;
            nDelta -= nDelta2;
            if (nDelta == 0) 
			{
				break;
			}
        }
        // clear m_bKeepSize flags
        if ((nDeltaOld == nDelta) || (nDelta == 0))
		{
            for (i = 0; i < nNumBars; i++)
                arrSCBars[i]->m_bKeepSize = FALSE;
		}
    }

    return TRUE;
}

void CSizingControlBar::AlignControlBars()
{
    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);

    BOOL bHorz       = IsHorzDocked();
    BOOL bNeedRecalc = FALSE;
    int  nAlign      = bHorz ? -2 : 0;

    CRect rc, rcDock;
    m_pDockBar->GetWindowRect(&rcDock);

	CSizingControlBar* pBar = NULL;
    for (int i = nFirst; i <= nLast; i++)
    {
        pBar = (CSizingControlBar*) m_pDockBar->m_arrBars[i];
        if (HIWORD(pBar) == 0 || !pBar->IsVisible() ) 
		{
			continue; // placeholder
		}
        
        pBar->GetWindowRect(&rc);
        rc.OffsetRect(-rcDock.TopLeft());

        if(pBar->IsKindOf(RUNTIME_CLASS(CSizingControlBar)))
		{
            rc = CRect(rc.TopLeft(), bHorz ? pBar->m_sizeHorz : pBar->m_sizeVert);
		}

        if ((bHorz ? rc.left : rc.top) != nAlign)
        {
            if (!bHorz)
                rc.OffsetRect(0, nAlign - rc.top - 2);
            else if (m_nDockBarID == AFX_IDW_DOCKBAR_TOP)
                rc.OffsetRect(nAlign - rc.left, -2);
            else
                rc.OffsetRect(nAlign - rc.left, 0);

            pBar->MoveWindow(rc);
            bNeedRecalc = TRUE;
        }

        nAlign += (bHorz ? rc.Width() : rc.Height()) - 2;
    }

    if (bNeedRecalc)
        m_pDockSite->DelayRecalcLayout();
}

void CSizingControlBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    UNUSED_ALWAYS(bDisableIfNoHndler);
    UNUSED_ALWAYS(pTarget);
}
