// SMSFrame.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "SMSFrame.h"
#include "MainFrm.h"
#include "CategoryView.h"
#include "Sprite\MsgIDDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CSMSToolBar::CreateBar(CWnd* pParentWnd)
{         
    if (!CreateEx(pParentWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !LoadToolBar(IDR_SMS))
    {
        TRACE0("Failed to create toolbar\n");
        return FALSE;      
    }
    
    //create items  
    CreateItems();
    
    return TRUE;
}

int CSMSToolBar:: CreateItems( )
{
    //...
    return 0;
}


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
/////////////////////////////////////////////////////////////////////////////
// CSMSFrame

IMPLEMENT_DYNCREATE(CSMSFrame, CFrameWnd)

CSMSFrame::CSMSFrame()
{
	m_pSMSView = NULL;
	m_bShowContentBar = TRUE;
	m_bShowCategBar = TRUE;
	m_pCategoryBar = NULL;
}

CSMSFrame::~CSMSFrame()
{
	m_pSMSView = NULL;
	m_pCategoryBar = NULL;
}


BEGIN_MESSAGE_MAP(CSMSFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CSMSFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_CONTENT_BAR, OnViewContentBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CONTENT_BAR, OnUpdateViewContentBar)
	ON_COMMAND(ID_VIEW_CATEG_BAR, OnViewCategBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CATEG_BAR, OnUpdateViewCategBar)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PROGRESS, OnProgressMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSFrame message handlers

int CSMSFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_wndToolBar.CreateBar(this);

#ifndef SPRD8800
	m_wndToolBar.LoadToolBar(IDR_SMS_EGO);
#endif

	UINT uStyle = m_wndToolBar.GetButtonStyle(1);
	uStyle |=  BS_FLAT;
	m_wndToolBar.SetButtonStyle(1,uStyle);


	CRect rect;
	
	theApp.m_pMainWnd->GetWindowRect(&rect);

	CRect newRect(rect.left, rect.bottom, rect.right, rect.bottom + SMS_FRAME_HEIGHT);

	SetWindowPos(this, newRect.left,newRect.top,newRect.Width(),newRect.Height(), SWP_SHOWWINDOW);
	MoveWindow(newRect, TRUE);

	//////////////////////////////////////////////////////////////////////////
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

//  m_wndStatusBar.SetPaneStyle(0, SBPS_NORMAL | SBPS_STRETCH ); 
// 	m_wndStatusBar.SetPaneInfo( 0, ID_SEPARATOR, SBPS_NORMAL| SBPS_STRETCH, 100 );
// 	for(int i = 1 ;i< sizeof(indicators)/sizeof(UINT); i++)
// 	{
// 		m_wndStatusBar.SetPaneInfo( i, m_wndStatusBar.GetItemID(i), SBPS_NORMAL, 30 );
// 	}

	
	CRuntimeClass * prtc = RUNTIME_CLASS( CCoolBar );
	m_pCategoryBar = ( CCoolBar * )prtc->CreateObject();
	if( NULL == m_pCategoryBar )
	{
		TRACE0("Failed to create categorybar object\n");
		return -1;
	}

	m_pCategoryBar->m_bAutoDelete = TRUE;

	CString strCateg;
	if( !strCateg.LoadString(IDS_CATEG_NAME) )
		strCateg = _T("Category");

	if (!m_pCategoryBar->Create(strCateg, this, CSize(180, 120), 0, TRUE ))
	{
		TRACE0("Failed to create category bar\n");
	    return -1;
	}
   

	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	HCURSOR hHorz = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_HORZ));
	HCURSOR hVert = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_VERT));
	m_pCategoryBar->SetStretchCursor( hHorz, hVert );

	CCreateContext * pContext = (CCreateContext *)(lpCreateStruct->lpCreateParams);
	if( !m_pCategoryBar->AddPage( RUNTIME_CLASS(CCategoryView), pContext) )
	{
		TRACE0("Failed to create category view\n");
	    return -1;
	}

	m_pCategoryBar->SetBarStyle( CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_pCategoryBar->EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	DockControlBar(m_pCategoryBar, AFX_IDW_DOCKBAR_LEFT);



	//by teana

	CMenu * pMenu = GetMenu();
	_ASSERTE( pMenu != NULL );

#ifdef SPRD8800
    pMenu->GetSubMenu(1)->DeleteMenu(ID_EDIT_NEW_MEMO,MF_BYCOMMAND);
	pMenu->GetSubMenu(2)->DeleteMenu(ID_OPTION_LOAD_MS_ALL,MF_BYCOMMAND);
	pMenu->GetSubMenu(2)->DeleteMenu(ID_OPTION_LOAD_MEMO,MF_BYCOMMAND);
	pMenu->GetSubMenu(2)->DeleteMenu(ID_OPTION_LOAD_PBG,MF_BYCOMMAND);
#else
	if(!theApp.m_bSupportMemo)
	{
		pMenu->GetSubMenu(1)->DeleteMenu(ID_EDIT_NEW_MEMO,MF_BYCOMMAND);
		pMenu->GetSubMenu(2)->DeleteMenu(ID_OPTION_LOAD_MEMO,MF_BYCOMMAND);
		m_wndToolBar.GetToolBarCtrl().HideButton(ID_OPTION_LOAD_MEMO);
	}
	pMenu->GetSubMenu(2)->DeleteMenu(ID_OPTION_LOAD_PBG,MF_BYCOMMAND);
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_OPTION_LOAD_PBG);
	pMenu->GetSubMenu(2)->GetSubMenu(1)->DeleteMenu(ID_OPTION_SMS_SEND_MODE,MF_BYCOMMAND);
	pMenu->GetSubMenu(2)->GetSubMenu(1)->DeleteMenu(ID_OPTION_SMS_STORE,MF_BYCOMMAND);
	pMenu->GetSubMenu(2)->DeleteMenu(2,MF_BYPOSITION);
#endif
	
	return 0;
}

void CSMSFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	UNREFERENCED_PARAMETER(bAddToTitle);

	CString strText;
#ifdef SPRD8800
	strText.LoadString(IDS_MODULE_NAME);
#else
	strText.LoadString(IDS_SMS_TITLE);
#endif
    SetWindowText(strText);
}

void CSMSFrame::OnClose() 
{
	if(!IsWindowEnabled())
	{
		return;
	}

	// TODO: Add your message handler code here and/or call default	
	CFrameWnd::OnClose();

	theApp.ReleaseWrapper();
	theApp.m_pSMSFrame = NULL;

//	GetActiveDocument()->OnCloseDocument();

	
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CSMSFrame::OnViewContentBar() 
{
	// TODO: Add your command handler code here
	_ASSERTE( m_pSMSView->m_pFrameWnd != NULL );

	m_bShowContentBar = !m_bShowContentBar;
	m_pSMSView->m_pFrameWnd->ShowContentBar(m_bShowContentBar);
}

void CSMSFrame::OnUpdateViewContentBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	_ASSERTE( m_pSMSView->m_pFrameWnd != NULL );

	if( m_pSMSView->m_pFrameWnd->ContentBarIsVisible() )
        m_bShowContentBar = TRUE;
    else
        m_bShowContentBar = FALSE;

    pCmdUI->SetCheck(m_bShowContentBar);

	switch( HIWORD(m_pSMSView->m_dwCategID) )
	{
	case ID_CATEG_SMS:
	case ID_CATEG_MEMO:		
		pCmdUI->Enable(TRUE);
		break;
	case ID_CATEG_PB:
	case ID_CATEG_PBG:
	case ID_CATEG_PBGM:
        pCmdUI->Enable(FALSE);
		break;
	default:
		TRACE(_T("CSMSView::m_dwCategID is [%.8X]\r\n"),m_pSMSView->m_dwCategID);
		_ASSERTE( 0 );
		break;
	}
}

void CSMSFrame::OnViewCategBar() 
{
	// TODO: Add your command handler code here
	ShowControlBar(m_pCategoryBar, !m_bShowCategBar, TRUE);
}

void CSMSFrame::OnUpdateViewCategBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pCategoryBar->GetStyle() & WS_VISIBLE)
    {
        m_bShowCategBar = TRUE;
    }
    else
    {
        m_bShowCategBar = FALSE;
    }

    pCmdUI->SetCheck(m_bShowCategBar);
}


LRESULT CSMSFrame::OnProgressMsg(WPARAM wParam, LPARAM lParam)
{
	CProgressCtrl & prgCtrl = m_wndStatusBar.m_prgrsCtrl;
	switch(wParam)
	{
	case SMS_START:
		prgCtrl.ShowWindow(SW_SHOW);
		prgCtrl.SetRange32(0, lParam);
		prgCtrl.SetPos(0);
		break;
	case SMS_PROCEED:
		prgCtrl.SetPos(lParam);
		break;
	case SMS_END:
		if(lParam == 100)
		{
			int nLower = 0;
			int nUpper = 0;
			prgCtrl.GetRange(nLower,nUpper);
			prgCtrl.SetPos(nUpper);
		}
		prgCtrl.ShowWindow(SW_HIDE);
		break;
	default:
		_ASSERTE(0);
	}

	return 0L;
}