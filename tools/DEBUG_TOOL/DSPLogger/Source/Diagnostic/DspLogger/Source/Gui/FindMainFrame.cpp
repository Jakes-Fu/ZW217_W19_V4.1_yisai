// FindFrame.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "FindMainFrame.h"
#include "FindLeftWnd.h"
#include "FindRightWnd.h"
#include "FindDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFindMainFrame
IMPLEMENT_DYNCREATE(CFindMainFrame, CFrameWnd)

CFindMainFrame::CFindMainFrame()
{
	m_nFrameIndex=0;
}

CFindMainFrame::~CFindMainFrame()
{
}


BEGIN_MESSAGE_MAP(CFindMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CFindMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_SHOW_CHART_CTRL, OnShowChartCtrl)
	ON_COMMAND(ID_SHOW_FIND, OnShowFind)
	ON_UPDATE_COMMAND_UI(ID_SHOW_CHART_CTRL, OnUpdateShowChartCtrl)
	ON_UPDATE_COMMAND_UI(ID_SHOW_FIND, OnUpdateShowFind)
	ON_COMMAND(ID_FIND_CLOSE, OnFindClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindMainFrame message handlers

int CFindMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN |
        CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_FIND))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//set Frame position

//	MoveWindow(0,::GetSystemMetrics(SM_CYSCREEN)/2-30,::GetSystemMetrics(SM_CXSCREEN),::GetSystemMetrics(SM_CYSCREEN)/2);
 
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);

	return 0;
}

BOOL CFindMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{

    // Size the window to  screen size and center it 
	/*UINT size;
	WINDOWPLACEMENT *pWP;
	if(g_theApp.GetProfileBinary("ChartFrame","Window Position", (BYTE **)&pWP, &size))
	{
		cs.x = pWP->rcNormalPosition.left;
		cs.y = pWP->rcNormalPosition.top;
		cs.cx = pWP->rcNormalPosition.right - pWP->rcNormalPosition.left;
		cs.cy = pWP->rcNormalPosition.bottom - pWP->rcNormalPosition.top;
		delete[]pWP;
	}
	else*/
	{
		cs.cy = ::GetSystemMetrics(SM_CYSCREEN)/2; 
		cs.cx = ::GetSystemMetrics(SM_CXSCREEN) ; 
	}

    cs.style |= WS_MAXIMIZEBOX|WS_MAXIMIZE; 
	cs.hwndParent = AfxGetMainWnd()->m_hWnd;
	cs.hMenu = NULL;
	return CFrameWnd::PreCreateWindow(cs);
}

void CFindMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	UNREFERENCED_PARAMETER(bAddToTitle);
	CString strTitle;
	strTitle.Format("Chart: %d" ,m_nFrameIndex+1);
    SetWindowText( strTitle );
}

void CFindMainFrame::OnClose() 
{
	//it is a flag so need set it null when close 
	g_theApp.m_pFindWnd[m_nFrameIndex]=NULL;
	
	//save size of screen
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	g_theApp.WriteProfileBinary("ChartFrame", "Window Position", (BYTE*)&wp, sizeof(WINDOWPLACEMENT));

	CFrameWnd::OnClose();
}


BOOL CFindMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	UNREFERENCED_PARAMETER(lpcs);
    pContext->m_pNewViewClass = NULL;
	m_Splitter.CreateStatic( this, 1, 2 );
    m_Splitter.CreateView( 0, 0, RUNTIME_CLASS(CFindLeftWnd), CSize(280, 0), pContext);
    m_Splitter.CreateView( 0, 1, RUNTIME_CLASS(CFindRightWnd), CSize(100, 0), pContext);
	return TRUE;
}

void CFindMainFrame::OnShowChartCtrl() 
{
	CFindRightWnd * pWnd = (CFindRightWnd *)m_Splitter.GetPane(0, 1);
    _ASSERTE( pWnd != NULL );
	
    pWnd->ShowOrHideBar();
}

void CFindMainFrame::OnShowFind() 
{
	CFindLeftWnd * pWnd = (CFindLeftWnd *)m_Splitter.GetPane(0, 0);
    _ASSERTE( pWnd != NULL );

    pWnd->ShowOrHideBar();
}

void CFindMainFrame::OnUpdateShowChartCtrl(CCmdUI* pCmdUI) 
{
	CFindRightWnd * pWnd = (CFindRightWnd *)m_Splitter.GetPane(0, 1);
    _ASSERTE( pWnd != NULL );

    pCmdUI->SetCheck( pWnd->IsShowBar() );
}

void CFindMainFrame::OnUpdateShowFind(CCmdUI* pCmdUI) 
{
	CFindLeftWnd * pWnd = (CFindLeftWnd *)m_Splitter.GetPane(0, 0);
    _ASSERTE( pWnd != NULL );

    pCmdUI->SetCheck( pWnd->IsShowBar() );
}

void CFindMainFrame::OnFindClose() 
{
	PostMessage(WM_CLOSE);
}

void CFindMainFrame::SetInputItem()
{
	CFindLeftWnd * pWnd = (CFindLeftWnd *)m_Splitter.GetPane(0, 0);
    _ASSERTE( pWnd != NULL );
	pWnd->SetInputItem();
}