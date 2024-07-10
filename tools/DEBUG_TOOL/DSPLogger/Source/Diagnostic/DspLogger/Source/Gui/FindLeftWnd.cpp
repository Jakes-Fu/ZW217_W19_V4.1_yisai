// LeftWnd.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "FindLeftWnd.h"

#include "CoolBar.h"
#include "FindFindView.h"
#include "FindListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindLeftWnd

IMPLEMENT_DYNCREATE(CFindLeftWnd, CFrameWnd)

CFindLeftWnd::CFindLeftWnd()
{
    m_bShowBar = TRUE;
    m_pBarExt  = NULL;
}

CFindLeftWnd::~CFindLeftWnd()
{
}//lint !e1540


BEGIN_MESSAGE_MAP(CFindLeftWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CFindLeftWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindLeftWnd message handlers

BOOL CFindLeftWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	pContext->m_pNewViewClass = RUNTIME_CLASS(CFindListView);

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

int CFindLeftWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRuntimeClass * prtc = RUNTIME_CLASS( CCoolBar );
	m_pBarExt = ( CCoolBar * )prtc->CreateObject();
	if( NULL == m_pBarExt )
	{
		TRACE0("Failed to create categorybar object\n");
		return -1;
	}

	m_pBarExt->m_bAutoDelete = TRUE;
    
	if (!m_pBarExt->Create(_T("Find Test Points and Draw"), this, CSize(80, 520), 0, TRUE))
	{
		TRACE0("Failed to create category bar\n");
	    return -1;
	}

	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	HCURSOR hHorz = LoadCursor(hInstance, MAKEINTRESOURCE(AFX_IDC_HSPLITBAR));
	HCURSOR hVert = LoadCursor(hInstance, MAKEINTRESOURCE(AFX_IDC_VSPLITBAR));
	m_pBarExt->SetStretchCursor( hHorz, hVert );

	CCreateContext * pContext = (CCreateContext *)(lpCreateStruct->lpCreateParams);
	if( !m_pBarExt->AddPage( RUNTIME_CLASS(CFindFindView), pContext) )
	{
		TRACE0("Failed to create category view\n");
	    return -1;
	}

	m_pBarExt->SetBarStyle( CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	
	m_pBarExt->EnableDocking(CBRS_ALIGN_TOP);

    EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(m_pBarExt, AFX_IDW_DOCKBAR_TOP);

	return 0;
}

BOOL CFindLeftWnd::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return FALSE;//CFrameWnd::OnEraseBkgnd(pDC);
}

BOOL CFindLeftWnd::IsShowBar(void)
{
    BOOL bRet = FALSE;
    if( m_pBarExt != NULL )
    {
        bRet = (m_pBarExt->GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
    }
    
    return bRet;
}

void CFindLeftWnd::ShowOrHideBar(void)
{
    _ASSERTE( m_pBarExt != NULL );
    
    ShowControlBar(m_pBarExt, !IsShowBar(), FALSE);
}

void CFindLeftWnd::SetInputItem()
{
    _ASSERTE( m_pBarExt != NULL );
	CWnd*pWnd= m_pBarExt->GetActiveWnd();
	_ASSERTE(pWnd != NULL);
	CComboBox*pcombox=(CComboBox*) pWnd->GetDlgItem(IDC_COMBO_ADDR);
	pcombox->SetFocus();
	pcombox->SetEditSel(0,0);


}