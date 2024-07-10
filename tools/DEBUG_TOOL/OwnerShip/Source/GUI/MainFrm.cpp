// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Ownership.h"

#include "MainFrm.h"
#include "OwnershipListView.h"
#include "CityQuhaoListView.h"
#include "DlgEditCity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_CITY, OnEditCity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
//	cs.style &= ~WS_MAXIMIZEBOX;
//	cs.style &= ~WS_MAXIMIZE;
//	cs.style &= ~WS_THICKFRAME;
    cs.style &= ~FWS_ADDTOTITLE;
//	cs.style |= WS_DLGFRAME;
	cs.cx = 450;
	cs.cy = 300;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	VERIFY( m_Splitter.CreateStatic(this, 1, 2) );
	int cx = lpcs->cx;
//	int cy = lpcs->cy;
	m_Splitter.CreateView( 0, 0, RUNTIME_CLASS(COwnershipListView), CSize(0,0), pContext );	
	m_Splitter.CreateView( 0, 1, RUNTIME_CLASS(CCityQuhaoListView), CSize(0,0), pContext );
 	m_Splitter.SetColumnInfo(0,cx/2,10);
	m_Splitter.SetColumnInfo(1,cx/2,10);
	RecalcLayout();
		
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
	return TRUE;
}

void CMainFrame::OnEditCity() 
{
	// TODO: Add your command handler code here
	CDlgEditCity dlg;
	dlg.DoModal();
}
