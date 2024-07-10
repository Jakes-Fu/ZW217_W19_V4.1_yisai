// FilterMainFrame.cpp : implementation file
//
#include "stdafx.h"
#include "DspLogger.h"
#include "CoolBar.h"
#include "FindChartCtrl.h"
#include "FilterMainFrame.h"
#include "FilterChartView.h"
#include "FilterChartCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterMainFrame

IMPLEMENT_DYNCREATE(CFilterMainFrame, CFrameWnd)

CFilterMainFrame::CFilterMainFrame()
{
    m_pDoc      = NULL;
    m_pBarCtrl  = NULL;
    m_bShowBar  = TRUE;
}

CFilterMainFrame::~CFilterMainFrame()
{
}//lint !e1540


BEGIN_MESSAGE_MAP(CFilterMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CFilterMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(IDR_CHARTCTRL, OnUpdateChartctrl)
	ON_COMMAND(IDR_CHARTCTRL, OnChartctrl)
	ON_COMMAND(IDR_EXIT, OnExit)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterMainFrame message handlers
BOOL CFilterMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
    // Size the window to  screen size and center it
	/*UINT size;
	WINDOWPLACEMENT *pWP;
	if(g_theApp.GetProfileBinary("FilterFrame","Window Position", (BYTE **)&pWP, &size))
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

int CFilterMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN |
        CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_FILTER))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//set Frame position
	
//	MoveWindow(0,::GetSystemMetrics(SM_CYSCREEN)/2-30,::GetSystemMetrics(SM_CXSCREEN),::GetSystemMetrics(SM_CYSCREEN)/2);
	
    //enable dockable control bars in a frame window
    EnableDocking(CBRS_ALIGN_ANY);	

//    m_pBarFind =CreateCoolBar(RUNTIME_CLASS(CFilterChartCtrl), lpCreateStruct ,CSize(180,100));
//    DockControlBar(m_pBarFind, AFX_IDW_DOCKBAR_LEFT);

    m_pBarCtrl =CreateCoolBar(RUNTIME_CLASS(CFilterChartCtrl), lpCreateStruct ,CSize(60,120));
    DockControlBar(m_pBarCtrl, AFX_IDW_DOCKBAR_BOTTOM);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	return 0;
}

CCoolBar* CFilterMainFrame:: CreateCoolBar(CRuntimeClass*pRT , LPCREATESTRUCT lpCreateStruct, CSize size )
{
	CRuntimeClass * prtc = RUNTIME_CLASS( CCoolBar );
	CCoolBar* pCoolBar = ( CCoolBar * )prtc->CreateObject();
	if( NULL == pCoolBar )
	{
		TRACE0("Failed to create category bar object\n");
		return FALSE;
	}

	pCoolBar->m_bAutoDelete = TRUE;
    
	if (!pCoolBar->Create(_T(""), this, size, 0, TRUE))
	{
		TRACE0("Failed to create category bar\n");
	    return FALSE;
	}

	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	HCURSOR hHorz = LoadCursor(hInstance, MAKEINTRESOURCE(AFX_IDC_HSPLITBAR));
	HCURSOR hVert = LoadCursor(hInstance, MAKEINTRESOURCE(AFX_IDC_VSPLITBAR));
	pCoolBar->SetStretchCursor( hHorz, hVert );

	CCreateContext * pContext = (CCreateContext *)(lpCreateStruct->lpCreateParams);
	if( !pCoolBar->AddPage( pRT, pContext) )
	{
		TRACE0("Failed to create category view\n");
	    return FALSE;
	}


	pCoolBar->SetBarStyle( CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	
	pCoolBar->EnableDocking(CBRS_ALIGN_BOTTOM);
    return pCoolBar;
}

BOOL CFilterMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}


void CFilterMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);	
}



void CFilterMainFrame::OnUpdateChartctrl(CCmdUI* pCmdUI) 
{
    BOOL bStatus= (m_pBarCtrl->GetStyle() & WS_VISIBLE) ? TRUE : FALSE;    
    pCmdUI->SetCheck(bStatus);
	
}

void CFilterMainFrame::OnChartctrl() 
{
    BOOL bStatus= (m_pBarCtrl->GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
    ShowControlBar(m_pBarCtrl, !bStatus, FALSE);
	
}

void CFilterMainFrame::OnExit() 
{
	PostMessage(WM_CLOSE);
}

void CFilterMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//save size of screen
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	g_theApp.WriteProfileBinary("FilterFrame", "Window Position", (BYTE*)&wp, sizeof(WINDOWPLACEMENT));

	
	CFrameWnd::OnClose();
}
