// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MobileCnnt.h"

#include "MainFrm.h"
#include "htmlhelp.h"
#include "MobileCnntDoc.h"
#include "DlgInputPuk1.h"
#include "DlgINputPin1.h"
#include "DlgUnlockPin2.h"
#include "ModifyPin.h"
#include <dbt.h>
#include "DlgHsdpaVersion.h"

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
	ON_WM_GETMINMAXINFO()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_COMMAND(ID_TOOL_OPEN, OnToolOpen)
	ON_MESSAGE(WM_RCV_AUTO_REPORT, OnRcvAutoReport)
	ON_MESSAGE(WM_OPEN_PUK, OnOpenPukDlg)
	ON_MESSAGE(WM_DISCONNECT_DIAL_UP, OnDisConnectDialUP)
	ON_WM_TIMER()
	ON_MESSAGE(WM_NIF_TOOL,OnNifTool)//lint !e665
	ON_MESSAGE(WM_NOTICE_SET_TIMER, OnNoticeSetTimer)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//static UINT indicators[] =
//{
//	ID_SEPARATOR,           // status line indicator
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
//};

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_hInstance = AfxGetInstanceHandle();
	m_cMenuTool.LoadMenu(IDR_TOOL);
	
	m_pMSInfoView = NULL;
	m_pDialUPView = NULL;
	m_pCallView = NULL;
	m_pEngineerView = NULL;
	m_pCfgView = NULL;
	m_pCnntView = NULL;
	m_uCount = 0;
	m_bTimer = FALSE;
	m_bDialCnnt = FALSE;
	m_hSignalIcon = NULL;
	m_hOldExeIcon = NULL;
	m_bSimInvalid = FALSE;
	memset(&m_tndTool, 0, sizeof(NOTIFYICONDATA));
	
	
}

CMainFrame::~CMainFrame()
{
	Shell_NotifyIcon(NIM_DELETE, &m_tndTool);
	m_pMSInfoView = NULL;
	m_pDialUPView = NULL;
	m_pCallView = NULL;
	m_pEngineerView = NULL;
	m_pCfgView = NULL;
	m_pCnntView = NULL;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAIN_USED))
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

	m_wndStatusBar.SetPaneStyle(0, SBPS_NORMAL | SBPS_STRETCH );

    m_wndStatusBar.SetPaneInfo( 0, ID_SEPARATOR, SBPS_NORMAL, 83 );
	m_wndStatusBar.SetPaneInfo( PROMT_INFO_IND, ID_SEPARATOR, SBPS_NORMAL, 197 );
	m_wndStatusBar.SetPaneInfo( ROAM_INFO_IND, ID_SEPARATOR, SBPS_NORMAL, 50 );        
    m_wndStatusBar.SetPaneInfo( NETYPE_INFO_IND, ID_SEPARATOR, SBPS_NORMAL, 105 );
	m_wndStatusBar.SetPaneInfo( SIGNAL_INFO_IND, ID_SEPARATOR, SBPS_NORMAL, 40 );        
    m_wndStatusBar.SetPaneInfo( CONNECT_INFO_IND, ID_SEPARATOR, SBPS_NORMAL, 60 );

	SetStatusBarState(CONNECT_INFO_IND, IDS_DISCONNECTED, COLOR_DISCONNECT);
#ifdef _EGO
	SetSiganalIcon(IDI_ICON_SIGNAL0);
#else
	SetSiganalIcon(IDI_ICON_SIGNAL00);
#endif

	UINT uStyle = m_wndToolBar.GetButtonStyle(1);
	uStyle |=  BS_FLAT;
	m_wndToolBar.SetButtonStyle(1,uStyle);

	m_wndToolBar.SetBitmaps(IDB_TB_ONE,IDB_TB_TWO,IDB_TB_THREE);

	//test
//	m_wndToolBar.GetToolBarCtrl().HideButton(IDC_LOAD_CALL);
//	CMenu * pMenu = GetMenu();
//	_ASSERTE( pMenu != NULL );
//	pMenu->GetSubMenu(1)->DeleteMenu(3,MF_BYPOSITION);
	//test

	CMenu * pMenu = GetMenu();
	_ASSERTE( pMenu != NULL );
	
#ifndef SPRD8800
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_HOME);
	m_wndToolBar.GetToolBarCtrl().HideButton(IDC_LOAD_ENGENEERING);
	m_wndToolBar.GetToolBarCtrl().HideButton(IDC_LOAD_CONFIG);
	m_wndToolBar.GetToolBarCtrl().HideButton(IDC_LOAD_CALL);

	if(!theApp.m_bLoadDialPage)
	{
		m_wndToolBar.GetToolBarCtrl().HideButton(IDC_DIAL_UP);
	}
	
	if(!theApp.m_bSupportSync)
	{
		m_wndToolBar.GetToolBarCtrl().HideButton(IDC_LOAD_SMS);
	}

    pMenu->GetSubMenu(0)->DeleteMenu(0,MF_BYPOSITION);
	pMenu->GetSubMenu(1)->DeleteMenu(0,MF_BYPOSITION);
	pMenu->GetSubMenu(1)->DeleteMenu(3,MF_BYPOSITION);
	pMenu->GetSubMenu(1)->DeleteMenu(3,MF_BYPOSITION);
	pMenu->GetSubMenu(1)->DeleteMenu(2,MF_BYPOSITION);
	if(!theApp.m_bLoadDialPage)
	{
		pMenu->GetSubMenu(1)->DeleteMenu(0,MF_BYPOSITION);
	}
	if(!theApp.m_bSupportSync)
	{
		pMenu->GetSubMenu(1)->DeleteMenu(IDC_LOAD_SMS,MF_BYCOMMAND);
	}

	pMenu->GetSubMenu(2)->DeleteMenu(2,MF_BYPOSITION);
#endif

#ifdef FOR_DPHONE
	//teana hu 2009.05.12 only for dphone
	m_wndToolBar.GetToolBarCtrl().HideButton(IDC_LOAD_ENGENEERING);
	m_wndToolBar.GetToolBarCtrl().HideButton(IDC_LOAD_SMS);
	m_wndToolBar.GetToolBarCtrl().HideButton(IDC_LOAD_CALL);

	pMenu->GetSubMenu(1)->DeleteMenu(2,MF_BYPOSITION);
	pMenu->GetSubMenu(1)->DeleteMenu(2,MF_BYPOSITION);
	pMenu->GetSubMenu(1)->DeleteMenu(2,MF_BYPOSITION);
	//
#endif	
	
	//remove help menu
	pMenu->GetSubMenu(2)->DeleteMenu(0,MF_BYPOSITION);
	
	//teana hu 2010.07.26

	if(!theApp.m_bLoadAudioPage)
	{
		m_wndToolBar.GetToolBarCtrl().HideButton(IDC_LOAD_CALL);
		pMenu->GetSubMenu(1)->DeleteMenu(3,MF_BYPOSITION);
	}
	

	AddLogoOnTB();

	memset(&m_tndTool, 0, sizeof(NOTIFYICONDATA));
	m_tndTool.cbSize = sizeof(NOTIFYICONDATA);
	m_tndTool.hWnd = m_hWnd;
	m_tndTool.uID = IDR_TOOL;
	m_tndTool.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_tndTool.uCallbackMessage = WM_NIF_TOOL;
	m_tndTool.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
#ifdef SPRD8800
	_tcscpy(m_tndTool.szTip, GetAppTile());
#else
	_tcscpy(m_tndTool.szTip, GetAppTile());
	SetWindowText(GetAppTile());
#endif
	Shell_NotifyIcon(NIM_ADD, &m_tndTool);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style &= ~WS_MAXIMIZEBOX;
	cs.style &= ~FWS_ADDTOTITLE;

	BOOL bMutex = GetPrivateProfileInt( _T("Window"), _T("Mutex"), 0, theApp.m_szCfgPathName );
	int nID = GetPrivateProfileInt( _T("Window"), _T("WindowID"), 1, theApp.m_szCfgPathName );
	if(bMutex)
	{
		m_strTitle.Format(_T("SPRD8800(%d)"), nID);
	}
	

	return TRUE;
}

UINT CMainFrame::OnNcHitTest(CPoint point)
{
	if(CWnd::OnNcHitTest(point) == HTRIGHT || CWnd::OnNcHitTest(point) == HTLEFT || 
		CWnd::OnNcHitTest(point) == HTTOP || CWnd::OnNcHitTest(point) == HTBOTTOM ||
		CWnd::OnNcHitTest(point) == HTTOPLEFT || CWnd::OnNcHitTest(point) == HTTOPRIGHT ||
		CWnd::OnNcHitTest(point) == HTBOTTOMLEFT || CWnd::OnNcHitTest(point) == HTBOTTOMRIGHT)
		return HTCLIENT;
	return CWnd::OnNcHitTest(point);
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


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize.x = WINDOW_WIDTH;  
	lpMMI->ptMinTrackSize.y = WINDOW_HEIGHT; 
	lpMMI->ptMaxTrackSize.x = WINDOW_WIDTH;
    lpMMI->ptMaxTrackSize.y = WINDOW_HEIGHT;
	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
	if(theApp.m_pSMSFrame != NULL)
	{			
		CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetActiveDocument();
		if(pDoc != NULL && !pDoc->m_bShowSMSWnd)
		{
			CRect rt;
			GetWindowRect(&rt);
			theApp.m_pSMSFrame->SetWindowPos(&CWnd::wndTop, rt.left, rt.bottom, rt.Width(), SMS_FRAME_HEIGHT, SWP_SHOWWINDOW|SWP_NOACTIVATE);
		}
	}
	
}

void CMainFrame::OnNifTool(WPARAM wParam,LPARAM lParam)
{
    UINT   uID;
    UINT   uMouseMsg;
    POINT  ptPoint;
    
    uID       = (UINT)wParam;
    uMouseMsg = (UINT)lParam;
    
    if(uID==IDR_TOOL)
    {
        if(uMouseMsg == WM_RBUTTONDOWN)
        {
            
            // get the mouse point 
            GetCursorPos(&ptPoint); 
            
            // Actives the back ground of the window
            SetForegroundWindow(); 
            
            // Display the popup menu
            m_cMenuTool.GetSubMenu(0)->TrackPopupMenu( 
                TPM_LEFTBUTTON|TPM_RIGHTBUTTON,  
                ptPoint.x , ptPoint.y, this, NULL);  
            
            // 增加一个额外消息，使菜单操作正确 
            PostMessage(WM_USER, 0, 0); 
            
        }
        if(uMouseMsg == WM_LBUTTONDBLCLK)
		{
			KillCallingTimer();
			OnToolOpen();
			this->SetActiveWindow();
		}
    }
    
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(nType == SIZE_MINIMIZED)
	{
		this->ShowWindow(SW_HIDE);
		if(theApp.m_pSMSFrame != NULL)
		{
			theApp.m_pSMSFrame->ShowWindow(SW_HIDE);
		}
	}
	
}

void CMainFrame::OnToolOpen()
{
	this->SetWindowPos(&CWnd::wndTop, 0,0,0,0,SWP_SHOWWINDOW);
	this->ShowWindow(SW_SHOWNORMAL);
	if(theApp.m_pSMSFrame != NULL)
	{
		CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetActiveDocument();
		if(pDoc != NULL && pDoc->m_bShowSMSWnd)
		{
			theApp.m_pSMSFrame->ShowWindow(SW_HIDE);
		}
	}
}

void CMainFrame::SwitchToForm(int nForm)               //进行视图切换的代码函数
{
    CView *pOldActiveView = (CView *)GetActiveView();           // Store old view
	
    if ( pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CMSInfoView) )
    {
        pOldActiveView->SetDlgCtrlID(IDD_FORMVIEW_MSINFO);
    }
    else if ( pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CDialUpView) )
    {
        pOldActiveView->SetDlgCtrlID(IDD_FORMVIEW_DIALUP);
    }
    else if ( pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CCallView) )
    {
        pOldActiveView->SetDlgCtrlID(IDD_FORMVIEW_CALL);
    }
    else if ( pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CEngineeringView) )
    {
        pOldActiveView->SetDlgCtrlID(IDD_FORMVIEW_ENGINEERING);
    }
	else if ( pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CConfigView) )
    {
        pOldActiveView->SetDlgCtrlID(IDD_FORMVIEW_CONFIG);
    }
    else if ( pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CConnectView) )
    {
        pOldActiveView->SetDlgCtrlID(IDD_FORMVIEW_CONNECT);
    }
	
    CView *pNewActiveView = (CView *)GetDlgItem(nForm);   // Get new view
    if (pNewActiveView == pOldActiveView)
    {
        /* Same view, just return */
        return ;
    }
	
    if (NULL == pNewActiveView)
    {
        switch(nForm)
        {
        case IDD_FORMVIEW_MSINFO:
            pNewActiveView = (CView *) new CMSInfoView();
			m_pMSInfoView = (CMSInfoView *)pNewActiveView;
            break;
            
        case IDD_FORMVIEW_DIALUP:
            pNewActiveView = (CView *) new CDialUpView();
			m_pDialUPView = (CDialUpView *)pNewActiveView;
            break;
			
        case IDD_FORMVIEW_CALL:
            pNewActiveView = (CView *) new CCallView();
			m_pCallView = (CCallView *)pNewActiveView;
            break;
			
		case IDD_FORMVIEW_ENGINEERING:
            pNewActiveView = (CView *) new CEngineeringView();
			m_pEngineerView = (CEngineeringView *)pNewActiveView;
            break;
			
		case IDD_FORMVIEW_CONFIG:
            pNewActiveView = (CView *) new CConfigView();
			m_pCfgView = (CConfigView *)pNewActiveView;
            break;
			
		case IDD_FORMVIEW_CONNECT:
            pNewActiveView = (CView *) new CConnectView();
			m_pCnntView = (CConnectView *)pNewActiveView;
            break;
			
        default:
            _ASSERTE( 0 );
            break;
        }
		
        // Attach document to new view
        CCreateContext context;
        context.m_pCurrentDoc   = pOldActiveView->GetDocument();
        context.m_pCurrentFrame = this;
        
        pNewActiveView->Create( NULL, 
			NULL, 
			WS_BORDER|WS_CHILD, 
			CFrameWnd::rectDefault, 
			this, 
			nForm, 
			&context );
		
        pNewActiveView->OnInitialUpdate();
    }
	
    SetActiveView(pNewActiveView);
    // Show new view and hide old view
    pNewActiveView->ShowWindow(SW_SHOW);
    pOldActiveView->ShowWindow(SW_HIDE);
	
    
    pNewActiveView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
	
    // Delete old view 
    // delete pOldActiveView;
	
    // Resize window
    RecalcLayout();
}

void CMainFrame::ModifyIcon(UINT uID)
{
	m_tndTool.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(uID));
	Shell_NotifyIcon(NIM_MODIFY, &m_tndTool);
}

void CMainFrame::SetSiganalIcon(UINT uIconID)
{
	m_hSignalIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(uIconID));

#ifdef _EGO
	m_wndStatusBar.SetPaneBmp(SIGNAL_INFO_IND, m_hSignalIcon);
#else
	m_wndStatusBar.GetStatusBarCtrl().SetIcon(SIGNAL_INFO_IND, m_hSignalIcon);
#endif	
}

void CMainFrame::SetStatusBarState(UINT uIndicatorsID, UINT uStrID, COLORREF clText)
{
	CString strText;
	VERIFY(strText.LoadString(uStrID));
#ifdef _EGO
	m_wndStatusBar.SetPaneTextClr(uIndicatorsID, clText);
#endif
	
	m_wndStatusBar.SetPaneText(uIndicatorsID,strText);

}

void CMainFrame::SetStatusBarState(UINT uIndicatorsID, LPCTSTR lpszText, COLORREF clText )
{
	CString strText;
	strText.Empty();
	if(lpszText != NULL)
	{
		strText = lpszText;
	}
#ifdef _EGO
	m_wndStatusBar.SetPaneTextClr(uIndicatorsID, clText);
#endif
	
	m_wndStatusBar.SetPaneText(uIndicatorsID,strText);
}

BOOL CMainFrame::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	//
// 	_TCHAR szHelpFile[ _MAX_PATH ];
// 	::GetModuleFileName(NULL, szHelpFile, _MAX_PATH );
// 
// 	LPTSTR pExt = _tcsrchr(szHelpFile, _T('.'));
// 	if( NULL == pExt )
// 		return FALSE;
// 
// 	_tcscpy(pExt, _T(".chm"));
// 
// 	HtmlHelp( this->m_hWnd, szHelpFile, HH_DISPLAY_TOPIC, NULL );
// 
// 	return CFrameWnd::OnHelpInfo(pHelpInfo);

	UNREFERENCED_PARAMETER(pHelpInfo);

	theApp.OnAppAbout();
	return TRUE;
}

void CMainFrame::AddLogoOnTB()
{
    int     nIndex;
    CRect   rcItem;


    nIndex = m_wndToolBar.CommandToIndex(ID_STATIC_LOGO);
    m_wndToolBar.GetItemRect(nIndex, &rcItem);
    m_wndToolBar.SetButtonInfo(nIndex, ID_STATIC_LOGO, TBBS_SEPARATOR, 0);    // The width will be adjusted in OnSize()
    
    rcItem.top    += 3;
    rcItem.bottom -= 3;
//	rcItem.left += 290;
//	rcItem.right  += 380;

#ifdef SPRD8800
	rcItem.left += 170;
	rcItem.right  += 260;
#else
	rcItem.left += 290;
	rcItem.right  += 380;
#endif


    m_StcLogo.CreateEx
    (
        WS_EX_STATICEDGE,
        _T("STATIC"),
        _T(""),
        WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | SS_SIMPLE,
        rcItem,
        &m_wndToolBar,
        ID_STATIC_LOGO
    );

	DWORD dwType =  m_StcLogo.GetStyle();

	dwType &= ~WS_BORDER;

	m_StcLogo.ModifyStyle(WS_BORDER, 0, 0);

}

void CMainFrame::StartAutoReport()
{
	if(theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->AutoReportStart(this->GetSafeHwnd(), NULL, 0, WM_RCV_AUTO_REPORT);
	}
}

LRESULT CMainFrame::OnDisConnectDialUP(  WPARAM wParam, LPARAM lParam  )
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

/*	if(m_pDialUPView != NULL)
	{
		if(!m_bDialCnnt)
		{
			m_pDialUPView->Release();
			if(m_pDialUPView->m_bHandCnnt)
			{
				m_pDialUPView->m_bHandCnnt = ! m_pDialUPView->m_bHandCnnt;
				m_pDialUPView->ChangeHandBtnString(m_pDialUPView->m_bHandCnnt);
			}
		}

	}*/	
	
	CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetActiveDocument();
	if(pDoc != NULL && !m_bDialCnnt)
	{
		pDoc->m_bDisDialUP = FALSE;
	}

	if(!theApp.m_bMuxUsed)
	{
		SwitchToForm(IDD_FORMVIEW_CONNECT);
		pDoc->m_uTbCheckFlag[DIAL_CHECK] = 0;


		if(!theApp.m_bConnected && !m_bDialCnnt)
		{
			if(m_pCnntView != NULL)
			{
				m_pCnntView->CnntOrDisCnntDevice();
			}
		}
	}
	else
	{
		SetStatusBarState(PROMT_INFO_IND, IDS_EMPTY, RGB(0,0,0));
		ModifyIcon(IDI_HIGH);
	}

	return 0;
}

LRESULT CMainFrame::OnRcvAutoReport(  WPARAM wParam, LPARAM lParam  )
{
	char szTemp[1024] = {0};
	memcpy(szTemp, (LPSTR)wParam, (DWORD)lParam);

	if(strstr(szTemp, "+CSQ:") != NULL)
	{
		CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetActiveDocument();
		
		DATA_HINT data_pint;
		memset(&data_pint, 0, sizeof(DATA_HINT));
		data_pint.dwLen = (DWORD)lParam;
		data_pint.lpString = szTemp;
		
		if(pDoc != NULL)
		{
			pDoc->UpdateAllViews(NULL, WM_RCV_SIGNAL, (CObject *)&data_pint);
		}
	}

	if(strstr(szTemp, "+COPS:") != NULL && strstr(szTemp, "+COPS: (") == NULL)
	{
		CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetActiveDocument();
		
		DATA_HINT data_pint;
		memset(&data_pint, 0, sizeof(DATA_HINT));
		data_pint.dwLen = (DWORD)lParam;
		data_pint.lpString = szTemp;
		
		if(pDoc != NULL)
		{
			pDoc->UpdateAllViews(NULL, WM_RCV_NETWORKINFO, (CObject *)&data_pint);

			if(m_pCfgView != NULL)
			{
				if(m_pCfgView->m_pSelNetDlg->GetSafeHwnd() != NULL)
				{
					m_pCfgView->m_pSelNetDlg->ShowRespondContent(szTemp);
				}
			}
		}

	}

	if(strstr(szTemp, "+CREG:") != NULL)
	{
		CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetActiveDocument();
		
		DATA_HINT data_pint;
		memset(&data_pint, 0, sizeof(DATA_HINT));
		data_pint.dwLen = (DWORD)lParam;
		data_pint.lpString = szTemp;
		
		if(pDoc != NULL)
		{
			pDoc->UpdateAllViews(NULL, WM_GET_SYSTEM_SERVICE, (CObject *)&data_pint);
		}
	}
	if(strstr(szTemp, "+CLIP:") != NULL || (strstr(szTemp, "RING") != NULL && strstr(szTemp, "WINSTRING") == NULL))
	{		
		if(!m_bTimer && (m_pCallView == NULL || (m_pCallView != NULL && m_pCallView->GetCallState() == -1)))
		{
			SetTimer(EVENT_CALL_TIMER, 500, NULL);
			m_bTimer = TRUE;
			m_hOldExeIcon = m_tndTool.hIcon;
			
			GetPhoneNum(szTemp);

			TRACE(_T("-------------------------coming\n"));
		}
		if(m_pCallView != NULL)
		{
			m_pCallView->PromtComingCall((LPCTSTR)m_strPhoneNum);
		}
	}

	if(strstr(szTemp, "+CMTI:") != NULL)
	{
		SetStatusBarState(PROMT_INFO_IND, IDS_COMING_SMS, RGB(0,0,0));

		//teana hu 2009.04.13	
		int nLoadSMS = GetPrivateProfileInt( _T("AutoTest"), _T("AutoLoadSMS"), 1, theApp.m_szCfgPathName );

//		AfxMessageBox(IDS_COMING_SMS);

		if(nLoadSMS == 1)
		{
			if(theApp.m_pSMSFrame == NULL || ((CMobileCnntDoc *)GetActiveDocument())->m_bShowSMSWnd == TRUE)
			{
				SendMessage(WM_COMMAND, IDC_LOAD_SMS, NULL);
			}
			if(theApp.m_pSMSFrame != NULL)
			{
				theApp.m_pSMSFrame->SendMessage(WM_COMMAND, ID_OPTION_LOAD_SMS, NULL);
			}
			
			SetStatusBarState(PROMT_INFO_IND, IDS_EMPTY, RGB(0,0,0));
		}
	}
	
	if(strstr(szTemp, "+CMT:") != NULL)
	{
		SMS_INFO smsinfo;
		if(theApp.m_pSMSFactory != NULL)
		{
			theApp.m_pSMSFactory->ParseSMSContent(szTemp, &smsinfo);
			if(smsinfo.szContent[0] != '\0')
			{
				CString strSMS;
				strSMS.LoadString(IDS_COMING_SMS);
				strSMS = strSMS + _T(": ") + smsinfo.szContent;
				AfxMessageBox(strSMS);
			}
		}
	}

	if(strstr(szTemp, "+CDS:") != NULL)
	{
		//GetSMSStatus(szTemp);
		SMS_INFO smsinfo;
		if(theApp.m_pSMSFactory != NULL)
		{
			theApp.m_pSMSFactory->ParseSMSStatus(szTemp, &smsinfo);
			if(smsinfo.szContent[0] != '\0')
			{
				CString strSMS;
				//strSMS.LoadString(IDS_COMING_SMS);
				strSMS = smsinfo.szContent;
				CString strPrompt;
				if(strSMS == _T("00"))
				{
					strPrompt.LoadString(IDS_SMS_SUCCESS);
					strPrompt += smsinfo.szNumber;
				}
				else
				{
					strPrompt.LoadString(IDS_SMS_FAILED);
					strPrompt += smsinfo.szNumber;
				}
				AfxMessageBox(strPrompt);
			}
		}
	}
	
	if(strstr(szTemp, "+CGDCONT:") != NULL || strstr(szTemp, "+SPPSRATE:") != NULL || strstr(szTemp, "+SPFRQ:") != NULL
		|| strstr(szTemp, "+SAUTOATT:") != NULL || strstr(szTemp, "+SPTEST:") != NULL || strstr(szTemp, "+SPID:") != NULL
		|| strstr(szTemp, "+SPUPLMN:") != NULL || strstr(szTemp, "TMSI:") != NULL || strstr(szTemp, "IMEISV:") != NULL)
	{

		if(m_pCfgView != NULL)
		{
			if(m_pCfgView->m_pDialNetDlg->GetSafeHwnd() != NULL)
			{
				m_pCfgView->m_pDialNetDlg->ShowRespondContent(szTemp);
			}
		}

		if(m_pCfgView != NULL)
		{
			if(m_pCfgView->m_pOthersDlg->GetSafeHwnd() != NULL)
			{
				m_pCfgView->m_pOthersDlg->ShowRespondContent(szTemp);
			}
		}

		if(strstr(szTemp, "+CGDCONT:") != NULL)
		{
			if(m_pDialUPView != NULL)
			{
				m_pDialUPView->ShowRespondContent(szTemp);
			}
		}

//		if(m_pEngineerView != NULL)
//		{
//			m_pEngineerView->ShowRespondContent(szTemp);
//		}
		
	}
	if(strstr(szTemp, "+COPS: (") != NULL)
	{	
		if(m_pCfgView != NULL)
		{
			if(m_pCfgView->m_pSelNetDlg->GetSafeHwnd() != NULL)
			{
				m_pCfgView->m_pSelNetDlg->ShowRespondContent(szTemp);
			}
		}
		
	}
	if(strstr(szTemp, "+XX") != NULL)
	{
		CWnd *pWnd = GetActiveWindow();
		if(pWnd->IsKindOf(RUNTIME_CLASS(CDlgInputPuk1)))
		{
			((CDlgInputPuk1 *)pWnd)->ShowRespondContent(szTemp);
		}
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CDlgINputPin1)))
		{
			((CDlgINputPin1 *)pWnd)->ShowRespondContent(szTemp);
		}
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CDlgUnlockPin2)))
		{
			((CDlgUnlockPin2 *)pWnd)->ShowRespondContent(szTemp);
		}
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CModifyPin)))
		{
			((CModifyPin *)pWnd)->ShowRespondContent(szTemp);
		}
		else
		{
			if(m_pCfgView != NULL)
			{
				if(m_pCfgView->m_pSecurityDlg->GetSafeHwnd() != NULL)
				{
					m_pCfgView->m_pSecurityDlg->ShowRespondContent(szTemp);
				}
			}
		}
	}

	if(strstr(szTemp, "+SIND: 9") != NULL)
	{
		if(m_pCallView != NULL)
		{
			if(m_pCallView->GetCallState() == TOOL_HOLD_ON || m_pCallView->m_bComCalling)
			{
				m_pCallView->StartHoldOn();
			}
			else
			{
				m_pCallView->StartCalling();
			}
		}
		
	}

	if(strstr(szTemp, "+SIND: 6") != NULL || strstr(szTemp, "NO CARRIER") != NULL/*&& strstr(szTemp, "+SIND: 6,") == NULL*/)
	{
		KillCallingTimer();

		//teana.hu 2010.04.30
		if(m_pCallView->GetSafeHwnd() != NULL && m_pCallView->GetCallState()  == -1 && m_pCallView->m_bComCalling == FALSE)
		{
			return 1;
		}
		//
		
		if(m_pCallView != NULL)
		{
			m_pCallView->StopCalling();
		}

	}
#ifdef _EGO	
	if(strstr(szTemp, "OK") != NULL)
	{
		CWnd *pWnd = GetActiveWindow();
		if(pWnd->IsKindOf(RUNTIME_CLASS(CCallView)))
		{
			m_pCallView->StartCalling();
		}
	}
#endif

	if(strstr(szTemp, "+CLCK") != NULL)
	{
		if(m_pCfgView != NULL)
		{
			if(m_pCfgView->m_pSecurityDlg->GetSafeHwnd() != NULL)
			{
				m_pCfgView->m_pSecurityDlg->ShowRespondContent(szTemp);
			}
		}
	}
	
	if(strstr(szTemp, "^SYSINFO") != NULL || IsIMEI(szTemp)
		|| strstr(szTemp, "^MODE") != NULL || strstr(szTemp, "+SIND") != NULL)
	{
		if(m_pMSInfoView != NULL)
		{
			m_pMSInfoView->ShowRespondContent(szTemp);
		}
	}

	if(strstr(szTemp, "^SYSCONFIG") != NULL)
	{
//		if(m_pEngineerView != NULL)
//		{
//			m_pEngineerView->ShowRespondContent(szTemp);
//		}

		if(m_pCfgView != NULL)
		{
			if(m_pCfgView->m_pDlgTDGSM->GetSafeHwnd() != NULL)
			{
				m_pCfgView->m_pDlgTDGSM->ShowRespondContent(szTemp);
			}
		}
	}
	
	//teana hu 2009.11.02
	if(m_pEngineerView != NULL)
	{
		m_pEngineerView->ShowRespondContent(szTemp);
	}

	return 1;
}

BOOL CMainFrame::IsIMEI(LPCSTR lpString)
{
	CString strText = lpString;

	int nIndex = strText.Find(_T("\r\n"));
	if(nIndex != -1)
	{
		strText = strText.Left(nIndex);
	}
	int nCount = strText.GetLength();
	if(nCount != 15)
	{
		return FALSE;
	}
	else
	{
		for(int i = 0; i < nCount; i++)
		{
			if(0 == isdigit(strText.GetAt(i)))
			{
				return FALSE;
			}
		}
		return TRUE;
	}
}

LRESULT CMainFrame::OnOpenPukDlg(  WPARAM wParam, LPARAM lParam  )
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	CDlgInputPuk1 dlg;
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	dlg.DoModal();
	return 1;
}

void CMainFrame::SetProviderStatus(UINT uIndex)
{
	CString strProviderStatus;
	switch(uIndex) 
	{
	case GSM_MODE:
		strProviderStatus.LoadString(IDS_CMCC_GSM);
		break;
	case W_MODE:
		strProviderStatus.LoadString(IDS_WCDMA);
		break;
	case TD_MODE:	
		strProviderStatus.LoadString(IDS_CMCC_TD);
		break;
	default:
		strProviderStatus.LoadString(IDS_EMPTY);
		break;
	}

	CString strText;
	if(m_pMSInfoView != NULL)
		strText.Format(_T("%s%s"),m_pMSInfoView->m_strNetProvider,strProviderStatus);
	else
		strText = strProviderStatus;
	SetStatusBarState(NETYPE_INFO_IND,strText.operator LPCTSTR(), RGB(0,0,0));
}

void CMainFrame::ClearAll(BOOL bClearStatusBar)
{
	m_pMSInfoView->ClearAll();
	if(bClearStatusBar)
	{
		if(!theApp.m_bMuxUsed)
		{
#ifdef _EGO
			SetSiganalIcon(IDI_ICON_SIGNAL0);
#else
			SetSiganalIcon(IDI_ICON_SIGNAL00);
#endif
		}
		SetStatusBarState(NETYPE_INFO_IND, IDS_EMPTY, RGB(0,0,0));
		SetStatusBarState(PROMT_INFO_IND, IDS_EMPTY, RGB(0,0,0));

		if(m_pCfgView != NULL)
		{
			m_pCfgView->ClearAll();
		}
		if(m_pEngineerView != NULL)
		{
			m_pEngineerView->ClearAll();
		}
	}
	CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetActiveDocument();
	if(pDoc != NULL)
	{
		pDoc->m_bEnableDialUP = TRUE;
		pDoc->m_bEnableCall = TRUE;
	}
	if(m_pCallView != NULL)
	{
		m_pCallView->ClearAll();
	}
	if(m_pDialUPView != NULL)
	{
		m_pDialUPView->ClearAll();
	}
	m_bSimInvalid = FALSE;

}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == EVENT_CALL_TIMER)
	{
		m_uCount ++;
		if(m_uCount %2 != 0)
		{
			ModifyIcon(IDI_COMING_CALL);
		}
		else
		{
			m_tndTool.hIcon = m_hOldExeIcon;
			Shell_NotifyIcon(NIM_MODIFY, &m_tndTool);
		}
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::KillCallingTimer(BOOL bDisp)
{
	if(m_bTimer)
	{
		m_wndStatusBar.SetPaneText(PROMT_INFO_IND,_T(" "));

//		SwitchToForm(IDD_FORMVIEW_CALL);

		if(bDisp)
		{
			if(m_pCallView != NULL)
			{
				m_pCallView->PromtComingCall(m_strPhoneNum);
			}
		}
		
		KillTimer(EVENT_CALL_TIMER);
		m_bTimer = FALSE;
		m_uCount = 0;
		
		m_tndTool.hIcon = m_hOldExeIcon;
		Shell_NotifyIcon(NIM_MODIFY, &m_tndTool);

		TRACE(_T("---------------------------------------------kill timer\n"));
	}
}

void CMainFrame::GetPhoneNum(LPCSTR szRet)
{
	CString strText = szRet;

	int nIndex = strText.Find(_T("\""));

	strText.Delete(0, nIndex + 1);

	nIndex = strText.Find(_T("\""));

	m_strPhoneNum =  strText.Left(nIndex);
	
	//teana hu 2009.08.28
	CString  strFlag = strText.Right(1);
	if(strFlag == _T("2"))
	{
		m_strPhoneNum = _T("");
	}
}

CString CMainFrame::MapNetProvider(CString strText)
{
	CString strProvider;

	if(strText.Find(_T("46000")) != -1)
	{
		strProvider.LoadString(IDS_CMCC);
	}
	if(strText.Find(_T("46001")) != -1)
	{
		strProvider.LoadString(IDS_CUC);
	}
	if(strText.Find(_T("46007")) != -1)
	{
		strProvider.LoadString(IDS_CMCC);
	}

	return strProvider;
}

LRESULT CMainFrame::OnNoticeSetTimer(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	CDocument * pDoc = NULL;
	pDoc = (CDocument *)GetActiveDocument();
	if(pDoc != NULL)
	{
		TCHAR szStr[MAX_PATH] = {0};
		_tcscpy(szStr, m_pCnntView->m_strModemName);
		MSINFO_HINT modem_hint;
		modem_hint.dwData_connspeed = m_pCnntView->m_dwBaudRate;
		modem_hint.lpModemString = szStr;
		pDoc->UpdateAllViews(NULL, WM_SET_TIMER, (CObject *)&modem_hint);
	}
	ModifyIcon(IDI_HIGHER);
	return 1;
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(theApp.m_pSMSFrame != NULL)
	{			
		CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetActiveDocument();
		if(pDoc != NULL && !pDoc->m_bShowSMSWnd)
		{
			CString strWarn;
			strWarn.LoadString(IDS_WARN_CLOSE_SMS_FIRST);
			MessageBox(strWarn,GetAppTile(),MB_ICONWARNING|MB_OK);
			return;
		}
	}

	if(theApp.m_bConnected)
	{		
		CString strWarn;
		strWarn.LoadString(IDS_CONFIRM_EXIT);
		if(MessageBox(strWarn,GetAppTile(),MB_ICONWARNING|MB_OKCANCEL) == IDOK)
		{
			if(theApp.m_bMuxUsed)
			{
				if((m_pDialUPView != NULL) &&(m_pDialUPView->m_bHandCnnt))
				{
					SetStatusBarState(PROMT_INFO_IND, IDS_DIAL_DISCONNECTING, RGB(0,0,0));
					theApp.HangUp();
				}
			}
			CFrameWnd::OnClose();
		}
	}
	else if((m_pDialUPView != NULL) &&(m_pDialUPView->m_bHandCnnt))
	{
		if(AfxMessageBox(IDS_CONFIRM_EXIT, IDOK, IDCANCEL) == IDOK)
		{
			SetStatusBarState(PROMT_INFO_IND, IDS_DIAL_DISCONNECTING, RGB(0,0,0));

			theApp.HangUp();
			CFrameWnd::OnClose();
		}
	}
	else
	{
		CFrameWnd::OnClose();
	}
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_DEVICECHANGE && theApp.m_pPortMain != NULL )
    {
        PDEV_BROADCAST_HDR pdbh = (PDEV_BROADCAST_HDR)lParam;
		
        switch(wParam)
        {
        case DBT_DEVICEARRIVAL:
            if( pdbh != NULL && pdbh->dbch_devicetype == DBT_DEVTYP_PORT )
            {				
				if(!theApp.m_bConnected)
				{
					if(m_pCnntView != NULL)
					{
						m_pCnntView->Connect(FALSE);
					}
				}
				
            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if( pdbh != NULL && pdbh->dbch_devicetype == DBT_DEVTYP_PORT )
            {
				if(m_pCnntView != NULL)
				{
					PDEV_BROADCAST_PORT pdbp = (PDEV_BROADCAST_PORT)lParam;
					PORT_ATTRIBUTE attr;
					//theApp.m_pPortMain->GetAttribute(&attr);
					memcpy(&attr, theApp.m_pPortAttrUsbModem, sizeof(PORT_ATTRIBUTE));

					PORT_ATTRIBUTE attr_main;
					PORT_ATTRIBUTE attr_sub;
					PORT_ATTRIBUTE attr_m1;
					PORT_ATTRIBUTE attr_m2;

					BOOL bDevice = FALSE;
					if(_tcsstr(pdbp->dbcp_name, attr.Com.szName) != NULL)
					{
						bDevice = TRUE;
					}

					if(theApp.m_bMuxUsed)
					{
						memcpy(&attr_main, theApp.m_pPortAttrMain, sizeof(PORT_ATTRIBUTE));
						memcpy(&attr_sub, theApp.m_pPortAttrSub, sizeof(PORT_ATTRIBUTE));
						memcpy(&attr_m1, theApp.m_pPortAttrVModem1, sizeof(PORT_ATTRIBUTE));
						memcpy(&attr_m2, theApp.m_pPortAttrVModem1, sizeof(PORT_ATTRIBUTE));
						if(_tcsstr(pdbp->dbcp_name, attr_main.Com.szName) != NULL
							|| _tcsstr(pdbp->dbcp_name, attr_sub.Com.szName) != NULL
							|| _tcsstr(pdbp->dbcp_name, attr_m1.Com.szName) != NULL
							|| _tcsstr(pdbp->dbcp_name, attr_m2.Com.szName) != NULL)
						{
							bDevice = TRUE;
						}
					}															
					
					if(bDevice)
					{
						if(theApp.m_bConnected)
						{
							m_pCnntView->CnntOrDisCnntDevice();
						}

						if(m_pDialUPView != NULL)
						{
							m_pDialUPView->Release();
							
							if(m_pDialUPView->m_bHandCnnt)
							{
								m_pDialUPView->m_bHandCnnt = ! m_pDialUPView->m_bHandCnnt;
								m_pDialUPView->ChangeHandBtnString(m_pDialUPView->m_bHandCnnt);
							}
							
						}
						
						CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetActiveDocument();
						if(pDoc != NULL)
						{
							pDoc->m_bDisDialUP = FALSE;
							pDoc->m_bEnableDialUP = FALSE;
							pDoc->SetTBCheck(7); //max is 6, in oder to set all uncheck
						}
							
						SwitchToForm(IDD_FORMVIEW_CONNECT);
						SetStatusBarState(PROMT_INFO_IND, IDS_PLEASE_CONFIRM_DEVICE, RGB(0,0,0));

						if(theApp.m_pSMSFrame != NULL)
						{
							
							theApp.m_pSMSFrame->PostMessage(WM_CLOSE, NULL, NULL);
							
						}
					}
					
				}
            }
            break;
        default:
            break;
        }
    }
	
	//for windows from sleep to awaken
	if(message == WM_POWERBROADCAST && theApp.m_pPortMain != NULL)
	{
		switch(wParam) 
		{
		case PBT_APMRESUMEAUTOMATIC:
		case PBT_APMRESUMESUSPEND:
			if(!theApp.m_bConnected)
			{
				if(m_pCnntView != NULL)
				{
					m_pCnntView->Connect(FALSE);
				}
			}
			break;
		default:
			break;
		}
	}
	
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

CString CMainFrame::GetAppTile()
{
	CString strTile;
#ifdef _EGO
	strTile.LoadString(IDS_EGO_APP_TITLE);
#else
	strTile.LoadString(AFX_IDS_APP_TITLE);
#endif

	return strTile;

}
