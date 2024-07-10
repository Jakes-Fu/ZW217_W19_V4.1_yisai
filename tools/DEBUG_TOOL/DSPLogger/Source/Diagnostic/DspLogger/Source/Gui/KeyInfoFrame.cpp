// KeyInfoFrame.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "KeyInfoFrame.h"
#include "KeyInfoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyInfoFrame

IMPLEMENT_DYNCREATE(CKeyInfoFrame, CFrameWnd)

CKeyInfoFrame::CKeyInfoFrame()
{
}

CKeyInfoFrame::~CKeyInfoFrame()
{
}


BEGIN_MESSAGE_MAP(CKeyInfoFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CKeyInfoFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(IDR_EXIT, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyInfoFrame message handlers

int CKeyInfoFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN |
        CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_KEYINFO))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	
	return 0;
}

void CKeyInfoFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	g_theApp.WriteProfileBinary("KeyInfoFrame", "Window Position", (BYTE*)&wp, sizeof(WINDOWPLACEMENT));
	g_theApp.m_bCreateKeyInfoFrame = FALSE;
	g_theApp.m_pKeyInfoDoc = NULL;
	
	CFrameWnd::OnClose();
}

BOOL CKeyInfoFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	/*UINT size;
	WINDOWPLACEMENT *pWP;
	if(g_theApp.GetProfileBinary("KeyInfoFrame","Window Position", (BYTE **)&pWP, &size))
	{
		cs.x = pWP->rcNormalPosition.left;
		cs.y = pWP->rcNormalPosition.top;
		cs.cx = pWP->rcNormalPosition.right - pWP->rcNormalPosition.left;
		cs.cy = pWP->rcNormalPosition.bottom - pWP->rcNormalPosition.top;
		delete[]pWP;
	}
	else*/
	{
		cs.cy = 678; 
		cs.cx = 475*2; 
	}

    cs.style |= WS_MAXIMIZEBOX|WS_MAXIMIZE; 
	cs.hwndParent = AfxGetMainWnd()->m_hWnd;
	cs.hMenu = NULL;
	
	return CFrameWnd::PreCreateWindow(cs);
}

void CKeyInfoFrame::OnExit() 
{
	// TODO: Add your command handler code here
	PostMessage(WM_CLOSE);
}

void CKeyInfoFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	UNREFERENCED_PARAMETER(bAddToTitle);
	CString strTitle;
	strTitle.Format("KeyInfo");
    SetWindowText( strTitle );
}

BOOL CKeyInfoFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_ESCAPE)
        {
            OnClose();
			return TRUE;
        }
    }
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}
