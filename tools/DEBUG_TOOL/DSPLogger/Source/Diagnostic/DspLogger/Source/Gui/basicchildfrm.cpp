// BasicChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "DspLogger.h"
#include "BasicChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasicChildFrame

IMPLEMENT_DYNCREATE(CBasicChildFrame, CMDIChildWnd)

CBasicChildFrame::CBasicChildFrame()
{
}

CBasicChildFrame::~CBasicChildFrame()
{
}


BEGIN_MESSAGE_MAP(CBasicChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CBasicChildFrame)
	ON_WM_GETMINMAXINFO()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicChildFrame message handlers

void CBasicChildFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
    lpMMI->ptMinTrackSize = CPoint( m_szMinFrmSize.cx, m_szMinFrmSize.cy );
	
	CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
}

void CBasicChildFrame::SetMinFrmSize(
    UINT  nMinWidth,
    UINT  nMinHeight
)
{
    m_szMinFrmSize.cx = nMinWidth;
    m_szMinFrmSize.cy = nMinHeight;
}

void CBasicChildFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if(SC_CLOSE == nID)
    {
        //don't close,just hide
        //ShowWindow(SW_HIDE);
        return;
    }

	CMDIChildWnd::OnSysCommand(nID, lParam);
}

BOOL CBasicChildFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	//make it look like SDI
	cs.style &= ~WS_SYSMENU;
	cs.style &= ~FWS_ADDTOTITLE;
	return CMDIChildWnd::PreCreateWindow(cs);
}
