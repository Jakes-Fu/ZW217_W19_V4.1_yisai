// RightWnd.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "FindRightWnd.h"
#include "CoolBar.h"
#include "FindChartCtrl.h"
#include "FindChartView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindRightWnd

IMPLEMENT_DYNCREATE(CFindRightWnd, CFrameWnd)

CFindRightWnd::CFindRightWnd()
{
    m_pBarExt  = NULL;
}

CFindRightWnd::~CFindRightWnd()
{
}//lint !e1540


BEGIN_MESSAGE_MAP(CFindRightWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CFindRightWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindRightWnd message handlers

BOOL CFindRightWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	pContext->m_pNewViewClass = RUNTIME_CLASS(CFindChartView);
	
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

int CFindRightWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	    
	CRuntimeClass * prtc = RUNTIME_CLASS( CCoolBar );
	m_pBarExt = ( CCoolBar * )prtc->CreateObject();
	if( NULL == m_pBarExt )
	{
		TRACE0("Failed to create category bar object\n");
		return -1;
	}

	m_pBarExt->m_bAutoDelete = TRUE;
    
	if (!m_pBarExt->Create(_T("Chart Control"), this, CSize(80, 110), 0, TRUE))
	{
		TRACE0("Failed to create category bar\n");
	    return -1;
	}

	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	HCURSOR hHorz = LoadCursor(hInstance, MAKEINTRESOURCE(AFX_IDC_HSPLITBAR));
	HCURSOR hVert = LoadCursor(hInstance, MAKEINTRESOURCE(AFX_IDC_VSPLITBAR));
	m_pBarExt->SetStretchCursor( hHorz, hVert );

	CCreateContext * pContext = (CCreateContext *)(lpCreateStruct->lpCreateParams);
	if( !m_pBarExt->AddPage( RUNTIME_CLASS(CFindChartCtrl), pContext) )
	{
		TRACE0("Failed to create category view\n");
	    return -1;
	}

	m_pBarExt->SetBarStyle( CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	m_pBarExt->EnableDocking(CBRS_ALIGN_BOTTOM);
    EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(m_pBarExt, AFX_IDW_DOCKBAR_BOTTOM);
	return 0;
}

BOOL CFindRightWnd::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;//CFrameWnd::OnEraseBkgnd(pDC);
}

BOOL CFindRightWnd::IsShowBar(void)
{
    BOOL bRet = FALSE;
    if( m_pBarExt != NULL )
    {
        bRet = (m_pBarExt->GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
    }
    
    return bRet;
}

void CFindRightWnd::ShowOrHideBar(void)
{
    _ASSERTE( m_pBarExt != NULL );
    
    ShowControlBar(m_pBarExt, !IsShowBar(), FALSE);
}

BOOL CFindRightWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_CLIPCHILDREN;
	return CFrameWnd::PreCreateWindow(cs);
}
