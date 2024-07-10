// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DspLogger.h"
#include "MainFrm.h"
#include <dbt.h>
#include "FindDoc.h"
#include "DlgPlaybackOrg.h"
#include <math.h>
#include "DspLoggerDoc.h"
#include "LUA_FUNC.h"
#include "DlgSaveSetting.h"
#include "CrashReport.h"
#include "XAboutDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable:4100)
#define TIMER_REFRESH 1

extern CFindDoc *g_pFindDoc;

BOOL WINAPI CBGetDescription(int nAddr, int nData, LPSTR lpDesc, int nMatch)
{
	return g_theApp.GetDescription(nAddr, nData, lpDesc, nMatch);
}

void WINAPI CBLookUpDescription(int nAddr, int nData,char* szDesCription,int nMaxLen)
{
	g_theApp.LookUpDescription(nAddr, nData,szDesCription,nMaxLen);
}
int CrashCallBack (_EXCEPTION_POINTERS*)
{
	// Just tell the CrashReport lib doesn't show the default 
	// message box.
	return CRCB_NEED_NO_PROCESS;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

static UINT g_uGuiMsg = ::RegisterWindowMessage(GUI_MSG_NAME);

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_CONNECT_DSP, OnConnectDsp)
	ON_COMMAND(ID_DISCONNECT_DSP, OnDisconnectDsp)
	ON_UPDATE_COMMAND_UI(ID_CONNECT_DSP, OnUpdateConnectDsp)
	ON_UPDATE_COMMAND_UI(ID_DISCONNECT_DSP, OnUpdateDisconnectDsp)
	ON_COMMAND(ID_CLEAR, OnClear)
	ON_COMMAND(ID_SETTINGS, OnSettings)
	ON_WM_CLOSE()
	ON_COMMAND(ID_START, OnStart)
	ON_UPDATE_COMMAND_UI(ID_START, OnUpdateStart)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_WM_TIMER()
	ON_COMMAND(ID_STOP, OnStop)
	ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateStop)
	ON_COMMAND(ID_PAUSE, OnPause)
	ON_UPDATE_COMMAND_UI(ID_PAUSE, OnUpdatePause)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS, OnUpdateSettings)
	ON_UPDATE_COMMAND_UI(ID_FIND_TP, OnUpdateFindTp)
	ON_COMMAND(ID_FILE_CONVERT, OnFileConvert)
	ON_COMMAND(ID_FILE_SAVETP, OnFileSaveTP)
	ON_COMMAND(ID_PLAYBACK_ORG, OnPlaybackOrg)
	ON_UPDATE_COMMAND_UI(ID_PLAYBACK_ORG, OnUpdatePlaybackOrg)
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_TP_ARRIVE, OnTpArrive)
	ON_MESSAGE(WM_CLEAR_STATE, OnClearState)
	ON_MESSAGE(WM_SAVE_MARK, OnSaveMarkFile)
	ON_MESSAGE(WM_REFLASH_STATUSBAR, OnReflashStatusBar)
	ON_MESSAGE(WM_AUTO_SAVE_AS, OnAutoSaveAs)
	ON_MESSAGE(WM_AUTO_CONNECT, OnAutoConnect)
	ON_MESSAGE(WM_AUTO_DISCONNECT, OnAutoDisConnect)
	ON_MESSAGE(WM_FIND_RELATED_ITEM, OnFindRelatedItem)
	ON_MESSAGE(WM_RENEW_DSPLOG, OnRewDSPLog)
	ON_MESSAGE(WM_NOTICE_ENDIAN, OnNoticeEndian)
	ON_MESSAGE(WM_DEV_HOUND,OnDevHound)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_STATE,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	ID_INDICATOR_DUMP,
	
};
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
    m_bConnected = FALSE;
    
    m_pView = NULL;

    m_bPause = TRUE;
    m_bStop = TRUE;

    m_pDspKernel = NULL;
	m_strPlaybackFile = "";
	m_fpMark = NULL;
	m_bPlaybackTP = FALSE;
	m_dwGenerateTPTime = TOOL_TIME;
	memset(m_lfd, 0, MAX_PATH);
	m_nLogType = E_COMMON_DSP;
}

CMainFrame::~CMainFrame()
{
}//lint !e1540

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
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

	m_wndStatusBar.SetPaneStyle(0, SBPS_NORMAL | SBPS_STRETCH );
    m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_STATE,SBPS_NORMAL,250);
	m_wndStatusBar.SetPaneInfo(2,ID_INDICATOR_CAPS,SBPS_NORMAL,50);
	m_wndStatusBar.SetPaneInfo(3,ID_INDICATOR_NUM,SBPS_NORMAL,50);
	m_wndStatusBar.SetPaneInfo(4,ID_INDICATOR_SCRL,SBPS_NORMAL,50);
	m_wndStatusBar.SetPaneInfo(5,ID_INDICATOR_DUMP,SBPS_NORMAL,40);	

    UpdateStatusBar();

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

    m_SettingsDlg.LoadSettings();
	UpdatedTitleInfo();
	m_dwGenerateTPTime = m_SettingsDlg.m_dwGenerateTPTime;

	HOOKCALLBACK lpcb = NULL;
	if( m_SettingsDlg.m_bAutotest )
	{
		lpcb = CrashCallBack;
	}

	InitCrashReport( NULL,FALSE,NULL,lpcb );

    m_pDspKernel = ::CreateDspKernel();
    if(NULL == m_pDspKernel)
    {
        return -1;
    }

    m_pDspKernel->GetTpSet()->SetLogFilePath( m_SettingsDlg.m_strLogPath.GetBuffer(MAX_PATH) );
	//teana hu 2009.05.18
	m_pDspKernel->SetLogFilePath(m_SettingsDlg.m_strLogPath.GetBuffer(MAX_PATH));
	//
    m_SettingsDlg.m_strLogPath.ReleaseBuffer();

    //--SetTimer(1,100,NULL);
	m_pDspKernel->GetTpSet()->SetCBFunc(CBGetDescription);
	m_pDspKernel->SetCBDescripTionFunc(CBLookUpDescription);

	m_usbMoniter.SetReceiver((DWORD)this->GetSafeHwnd(),WM_DEV_HOUND,FALSE);
	m_usbMoniter.Start();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	/*UINT size;
	WINDOWPLACEMENT *pWP = NULL;
	if(g_theApp.GetProfileBinary("DspLogger","Window Position", (BYTE **)&pWP, &size) && 
		pWP && pWP->length == sizeof(WINDOWPLACEMENT) )
	{
		cs.x = pWP->rcNormalPosition.left;
		cs.y = pWP->rcNormalPosition.top;
		cs.cx = pWP->rcNormalPosition.right - pWP->rcNormalPosition.left;
		cs.cy = pWP->rcNormalPosition.bottom - pWP->rcNormalPosition.top;
		//		SetWindowPlacement(pWP);
		delete[]pWP;
	}
	*/
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnConnectDsp() 
{
	m_bPlaybackTP = FALSE;
	UpdatedTitleInfo();
    if(m_pView != NULL)
    {
        m_pView->PrepareForDisp();
    }
	if (!m_SettingsDlg.CheckLogPath())
	{
		return;
	}
	if (m_SettingsDlg.m_bSaveRecent)
	{
		m_pDspKernel->GetTpSet()->SetMaxCount(m_SettingsDlg.m_nTPNum * 1000000);
	}
	else
	{
		m_pDspKernel->GetTpSet()->SetMaxCount(m_SettingsDlg.m_nMaxNum * 1000000);	
		
	}
	
/*	//teana hu 2011.03.18 for beijing test
	
	CWinApp* pApp = AfxGetApp();
	if(pApp != NULL)
	{
		int nMemSize = pApp->GetProfileInt("DSPMEM", "memfilesize", 0);
		m_pDspKernel->SetMemSize(nMemSize);
		pApp->WriteProfileInt("DSPMEM", "memfilesize", nMemSize);
	}
	//
	*/
	//polo.jiang change cfg from reg to ini
	m_pDspKernel->SetMemSize(m_SettingsDlg.m_dwMemFileSize);
	m_pDspKernel->EnableLogSN(m_SettingsDlg.m_bLogSN);
	m_pDspKernel->SetUnFilterAddrFlag(m_SettingsDlg.m_bNotFilterAddr);


	m_pDspKernel->GetTpSet()->SetLogFilePath( m_SettingsDlg.m_strLogPath.GetBuffer(MAX_PATH) );
	m_pDspKernel->SetLogFilePath(m_SettingsDlg.m_strLogPath.GetBuffer(MAX_PATH));


	/*
	//teana hu 2009.08.12
	//CWinApp* pApp  = AfxGetApp();
	m_pDspKernel->SetGenerateFlag(pApp->GetProfileInt("GenerateTime", "GenerateTime", 1),
		pApp->GetProfileInt("GenerateOrgFile", "GenerateOrgFile", 1));
	//
	*/
	//polo.jiang change cfg from reg to ini
	m_pDspKernel->SetGenerateFlag(m_SettingsDlg.m_dwGenerateTPTime,m_SettingsDlg.m_bGenerateOrgFile);

	if( m_SettingsDlg.m_nChannelType == TOOL_CHANNEL_SOCKET_CLIENT )
	{
		m_pDspKernel->SetEndian( m_SettingsDlg.m_nEndian );
	}
	else
	{
		m_pDspKernel->SetEndian( 1 );
	}
	m_pDspKernel->SetDspLogType(m_nLogType);


    DWORD dwArg[3];
    m_SettingsDlg.GetChannelArgs(dwArg);
    m_bConnected = m_pDspKernel->Connect(m_hWnd,
                                         m_SettingsDlg.m_nChannelType,
                                         dwArg,
										 WM_TP_ARRIVE
										 );
 
	if(m_pView != NULL)
    {
        m_pView->RefreshList(0);
    }

    if(!m_bConnected)
    {
        CString strError;
		if(m_SettingsDlg.m_nChannelType == TOOL_CHANNEL_UART)
		{
			strError.Format(IDS_CONNECT_FAILED,m_SettingsDlg.m_nPortIndex,m_SettingsDlg.m_nBaudIndex);
		}
		else
		{
			strError.Format(IDS_SOCKET_FAILED,m_SettingsDlg.m_nSocketPort, FIRST_IPADDRESS(m_SettingsDlg.m_dwIpAddr), 
				SECOND_IPADDRESS(m_SettingsDlg.m_dwIpAddr),
			THIRD_IPADDRESS(m_SettingsDlg.m_dwIpAddr),
			FOURTH_IPADDRESS(m_SettingsDlg.m_dwIpAddr));
		}
        AfxMessageBox(strError);
        return;
    }

	//teana hu 2009.12.17
	if(g_theApp.m_pKeyInfoDoc != NULL)
	{
		g_theApp.m_pKeyInfoDoc->UpdateAllViews(NULL, WM_UPDATE_SETTIMER);
	}

    SendMessageToDescendants(WM_RESET_ALL);
    UpdateStatusBar();
}

LRESULT CMainFrame::OnTpArrive( WPARAM wParam, LPARAM lParam )
{
	
    return 0L;
}

void CMainFrame::OnDisconnectDsp() 
{
	if(!m_bStop)
	{
		OnStop();
	}
    m_pDspKernel->DisConnect();
    m_bConnected = FALSE;

	if(m_pView != NULL)
    {
        m_pView->RefreshList(0);
    }

	if(g_theApp.m_pKeyInfoDoc != NULL)
	{
		g_theApp.m_pKeyInfoDoc->UpdateAllViews(NULL,WM_UPDATE_CLEAR);
	}
	
    UpdateStatusBar();
}

void CMainFrame::OnUpdateConnectDsp(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(!m_bConnected);
}

void CMainFrame::OnUpdateDisconnectDsp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bConnected );
}

void CMainFrame::LoadLogFile(CString strPathName)
{
    if(m_pView != NULL)
    {
        m_pView->PrepareForDisp();
    }

	//teana hu 2009.10.26
	m_bPlaybackTP = TRUE;
	SaveAllMarks();
	OnClear();

	CReplaySetting   replaySetDlg;

    if( m_pDspKernel->Load(strPathName) )
    {
		replaySetDlg.m_nEdtTo = m_pDspKernel->GetTpSet()->GetTpCount() - 1;
		replaySetDlg.m_nMaxNum = replaySetDlg.m_nEdtTo + 1;
		replaySetDlg.m_nEdtFrom = 0;
		if(replaySetDlg.DoModal() == IDOK)
		{
			ULONG nReplayCount = replaySetDlg.m_nEdtTo - replaySetDlg.m_nEdtFrom + 1;
			m_pView->m_lBaseIdx = replaySetDlg.m_nEdtFrom;
			if(m_pView != NULL)
			{
				m_pView->RefreshList(nReplayCount);
				//remove all marks
				m_pView->OnRemoveAllMarks() ;
			}
			m_pDspKernel->GetTpSet()->SetStartPos(replaySetDlg.m_nEdtFrom);
			m_pDspKernel->GetTpSet()->SetEndPos(replaySetDlg.m_nEdtTo + 1);
		}
    }

    m_strTitle = strPathName;
    SetWindowText(m_strTitle);	
}

void CMainFrame::OnClear() 
{
    if(m_pView)
    {
        m_pView->Clear();  
    }
}

void CMainFrame::OnSettings() 
{
    m_SettingsDlg.m_bReadOnly = m_bConnected;
    if(m_SettingsDlg.DoModal() == IDOK)
    {       
		UpdatedTitleInfo();
        m_SettingsDlg.m_strLogPath.ReleaseBuffer();
		m_pDspKernel->SetFrameHead(m_SettingsDlg.GetFrameHead());
		//teana hu 2009.05.15
		m_dwGenerateTPTime = m_SettingsDlg.m_dwGenerateTPTime;
		m_pDspKernel->SetGenerateFlag(m_SettingsDlg.m_dwGenerateTPTime, m_SettingsDlg.m_bGenerateOrgFile);
		
    }	
}

void CMainFrame::OnClose() 
{
	for(int i=0; i<MAX_CHART_WND_NUM; i++)
	{
		if( g_theApp.m_pFindWnd[i] != NULL )
		{
			g_theApp.m_pFindWnd[i]->SendMessage(WM_CLOSE);
			g_theApp.m_pFindWnd[i] = NULL;
		}
	}
	
    if(m_bConnected)
    {
        CString strText;
        strText.LoadString(IDS_CANT_CLOSE);
        if(AfxMessageBox(strText,MB_OKCANCEL) != IDOK)
            return;
		
        m_pDspKernel->DisConnect();
    }
	
    m_SettingsDlg.SaveSettings();

	//save size of screen
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	g_theApp.WriteProfileBinary("DspLogger", "Window Position", (BYTE*)&wp, sizeof(WINDOWPLACEMENT));

	SaveAllMarks();
	m_usbMoniter.Stop(); 
    m_pDspKernel->Release();
	
	CMDIFrameWnd::OnClose();
}

void CMainFrame::Pause()
{
    m_bPause = TRUE;
    m_pDspKernel->Pause();   
	KillTimer(TIMER_REFRESH);
}

void CMainFrame::OnStart() 
{
    if( m_bStop )
    {
        if(m_pView != NULL)
        {
            m_pView->PrepareForDisp();
        }

		//If Disk space less than 100M give a Warning
		if (100 > GetFreeDiskSpaceInKB(m_SettingsDlg.m_strLogPath)/1024)
		{
			MessageBox(TEXT("Disk Space less than 100M !!!"), TEXT("Warning"),
	    										MB_OK | MB_ICONINFORMATION);
		}

		CString  strFileName = m_SettingsDlg.GetLogName();
		//teana hu 2009.05.18
		if(!strFileName.IsEmpty())
		{
			int nIndex = strFileName.Find(_T("."));
			char szPreFix[MAX_PATH] = {0};
			if(nIndex != -1)
			{
				_tcscpy(szPreFix, strFileName.Left(nIndex));
			}
			else
			{
				_tcscpy(szPreFix, strFileName);
			}

			m_pDspKernel->SetFilePreFix(szPreFix);
		}
		//
		m_pDspKernel->GetTpSet()->SetLogFileName((LPSTR)(LPCTSTR)strFileName);
		m_pDspKernel->GetTpSet()->SaveRecent(m_SettingsDlg.m_bSaveRecent);
        m_pDspKernel->Start();

        if(m_pView != NULL)
        {
            m_pView->RefreshList(0);
        }
    }
    else
    {
        m_pDspKernel->Resume();
    }
    
    m_bPause = FALSE;
    m_bStop  = FALSE;

	SetTimer(TIMER_REFRESH, 200, NULL);
	
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
    //CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
    SetWindowText(m_strTitle);
}

void CMainFrame::OnUpdateStart(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( ( m_bPause || m_bStop ) && m_bConnected );
}

void CMainFrame::UpdateStatusBar()
{
    CString strText;

    if(m_bConnected)
    {
		//teana hu 2009.08.12
		if(m_SettingsDlg.m_nChannelType == TOOL_CHANNEL_UART)
		{
			strText.Format("COM%d   %d",m_SettingsDlg.m_nPortIndex,m_SettingsDlg.m_nBaudIndex);
		}
		else if(m_SettingsDlg.m_nChannelType == TOOL_CHANNEL_SOCKET_CLIENT)
		{
			strText.Format("IP: %s, Port:%d",m_SettingsDlg.m_strIpAddr,m_SettingsDlg.m_nSocketPort);
		}
    }
    else
    {
        strText = "Disconnected";
    }

    m_wndStatusBar.SetPaneText(1,strText);
}

void CMainFrame::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(	!m_bConnected );
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
    if(m_pView != NULL)
    {
        m_pView->RefreshList(0);
    }

	CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnStop() 
{ 
	m_pDspKernel->Stop(); 

	m_bStop = TRUE;
		
	KillTimer(TIMER_REFRESH);
	
	if(m_pView != NULL)
    {
        m_pView->RefreshList(0);
    }

}

void CMainFrame::OnUpdateStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !m_bStop );
}

void CMainFrame::OnPause() 
{
    m_bPause = TRUE;
    m_pDspKernel->Pause();  
	
	KillTimer(TIMER_REFRESH);
}

void CMainFrame::OnUpdatePause(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( !m_bPause && !m_bStop );
}

void CMainFrame::OnFileExport() 
{
	if( IDOK == m_ExportDlg.DoModal() )
    {
        LONGLONG llStart = m_ExportDlg.GetRangeFrom();
        LONGLONG llCount = m_ExportDlg.GetRangeTo();
		if( llStart >= llCount && llCount != -1 )
		{
            AfxMessageBox("Export Range Error !",MB_ICONSTOP);
			return;
		}

        if( llCount != -1 )
        {
            llCount = llCount - llStart;
        }

		CString strDesFile=m_ExportDlg.GetDesFile();
		if( strDesFile.GetLength() == 0)
		{
			// Destinate file can not be null
			AfxMessageBox("Destinate file path name can not be null !");
			return ;
		}
		
        CWaitCursor wc;
        if(!m_pDspKernel->Export(m_ExportDlg.GetSrcFile(),
								strDesFile,
								llStart,
								llCount,
								m_ExportDlg.m_dwType
								)
          )
        {
            AfxMessageBox(IDS_EXPORT_FILE_FAIL,MB_ICONSTOP);
        }
    }
}
BOOL CMainFrame::ProcExport(LPCTSTR lpTPfile,LPCTSTR lpLogFile,BOOL bAddDes/* = FALSE*/)
{
	BOOL bRet = FALSE;
	DWORD dwConvertType = bAddDes ? FILE_LOG_ADDDES : FILE_LOG_UNDES;
	bRet = m_pDspKernel->Export(lpTPfile,lpLogFile,0,-1,dwConvertType);
	return bRet;
}
void CMainFrame::OnUpdateSettings(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStop);
}

void CMainFrame::OnUpdateFindTp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bStop);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CFrameWnd::WindowProc(message, wParam, lParam);
	/*
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_DEVICECHANGE )
    {
        PDEV_BROADCAST_HDR pdbh = (PDEV_BROADCAST_HDR)lParam;

        switch(wParam)
        {
        case DBT_DEVICEARRIVAL:
            if( pdbh != NULL && pdbh->dbch_devicetype == DBT_DEVTYP_PORT )
            {
                PDEV_BROADCAST_PORT pdbp = (PDEV_BROADCAST_PORT)lParam;
                _ASSERTE( _tcsstr(pdbp->dbcp_name, _T("COM")) != NULL );

                int nPort = (int)_ttol( pdbp->dbcp_name + 3 ); //lint !e416

                _ASSERTE( nPort > 0 && nPort < 256 );
            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if( pdbh != NULL && pdbh->dbch_devicetype == DBT_DEVTYP_PORT )
            {
                 PDEV_BROADCAST_PORT pdbp = (PDEV_BROADCAST_PORT)lParam;
                _ASSERTE( _tcsstr(pdbp->dbcp_name, _T("COM")) != NULL );

                const int nPort = (int)_ttol( pdbp->dbcp_name + 3 );//lint !e416
                _ASSERTE( nPort > 0 && nPort < 256 );

                if( 
					!m_bStop							&&	 
					nPort == m_SettingsDlg.m_nPortIndex &&
					TOOL_CHANNEL_UART == m_SettingsDlg.m_nChannelType
				  )
                {
                    OnStop();
                    OnDisconnectDsp();
                }
            }
            break;
        default:
            break;
        }
    }

	return CFrameWnd::WindowProc(message, wParam, lParam);
	*/
}

LRESULT CMainFrame::OnDevHound(WPARAM wParam,LPARAM lpParam)
{
	unsigned int nPort = (unsigned int)wParam;
	switch(lpParam)
	{
        case 1: //DBT_DEVICEARRIVAL     
			if( 	 
				m_bStop && nPort == m_SettingsDlg.m_nPortIndex		&&
				TOOL_CHANNEL_UART == m_SettingsDlg.m_nChannelType	&&
				m_SettingsDlg.m_bAutoConnect
			  )
			{
				OnConnectDsp();
				OnStart();
			}
			break;
        case 0: //DBT_DEVICEREMOVECOMPLETE

			if( 
				!m_bStop							&&	 
				nPort == m_SettingsDlg.m_nPortIndex &&
				TOOL_CHANNEL_UART == m_SettingsDlg.m_nChannelType
				)
			{
				OnStop();
				OnDisconnectDsp();
              }
			break;
		default:
            break;
	}
	return 0L;
}
void CMainFrame::OnFileConvert() 
{
    CFileDialog filedlg( TRUE, _T("log"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
								_T("dsplogger files (*.log)|*.log|"), NULL );
	
    if( filedlg.DoModal() != IDOK )
    {
        return;
    }
	
    CWaitCursor waitCursor;
    CString strLogFile = filedlg.GetPathName();

    ULARGE_INTEGER uiLen;
	LPBYTE	  lpRead    = NULL;
	LPBYTE	  lpWrite   = NULL;

	LONG	  lByteCount= 0;
    LPTSTR    pszEnd;

	//create log file map
	HANDLE hLogFile = ::CreateFile(strLogFile,
							GENERIC_READ,
					        0,
					        NULL,
					        OPEN_ALWAYS,
					        FILE_ATTRIBUTE_NORMAL,
							NULL);
    
    if( INVALID_HANDLE_VALUE == hLogFile )
	{
		AfxMessageBox("Open log file failed!");
		return ;
	}
    uiLen.LowPart = GetFileSize(hLogFile, &uiLen.HighPart);

    HANDLE hLogMap = ::CreateFileMapping(hLogFile,NULL,PAGE_READONLY,uiLen.HighPart,uiLen.LowPart,NULL);	
    if( NULL == hLogMap )
    {
        CloseHandle(hLogFile);
        hLogFile = INVALID_HANDLE_VALUE;
		AfxMessageBox("create log file map failed!");
        return ;
    }

    lpRead = (LPBYTE)::MapViewOfFile(hLogMap, FILE_MAP_READ, 0, 0, 0);
    if( NULL == lpRead )
    {
        CloseHandle(hLogMap);
        hLogMap = NULL;
        CloseHandle(hLogFile);
        hLogFile = INVALID_HANDLE_VALUE;
		AfxMessageBox("create log file map failed!");
        return ;
    }

	//create tp file map
	CString strTPFile=strLogFile.Left(strLogFile.ReverseFind('.')+1);
	strTPFile +="tp";
    HANDLE hTPFile = ::CreateFile(strTPFile,
							  GENERIC_READ | GENERIC_WRITE,
							  0,
							  NULL,
							  OPEN_ALWAYS,
							  FILE_ATTRIBUTE_NORMAL,
							  NULL);

    if( hTPFile == INVALID_HANDLE_VALUE )
	{		
		UnmapViewOfFile(lpRead);

        CloseHandle(hLogMap);
        hLogMap = NULL;
        CloseHandle(hLogFile);
        hLogFile = INVALID_HANDLE_VALUE;

		AfxMessageBox("create tp file map failed!");
		return;
	}
	
	HANDLE hTPMap = ::CreateFileMapping(hTPFile,NULL,PAGE_READWRITE,uiLen.HighPart,uiLen.LowPart,NULL);	
    if( hTPMap == NULL )
    {
		UnmapViewOfFile(lpRead);

        CloseHandle(hLogMap);
        hLogMap = NULL;
        CloseHandle(hLogFile);
        hLogFile = INVALID_HANDLE_VALUE;
	
		CloseHandle(hTPFile);
		hTPFile = INVALID_HANDLE_VALUE;

		AfxMessageBox("create tp file map failed!");
		return;  
    }

	lpWrite = (LPBYTE)::MapViewOfFile(hTPMap, FILE_MAP_WRITE, 0, 0, 0);
    if( NULL == lpWrite )
    {
		UnmapViewOfFile(lpRead);
        
		CloseHandle(hLogMap);
        hLogMap = NULL;
        CloseHandle(hLogFile);
        hLogFile = INVALID_HANDLE_VALUE;

        CloseHandle(hTPMap);
        hTPMap = NULL;	
		CloseHandle(hTPFile);
		hTPFile = INVALID_HANDLE_VALUE;

		AfxMessageBox("create tp file map failed!");
		return;  
    }
	
	//
	// begine convert 
	//
	
	BOOL   bLogType= TRUE;
	DWORD  curPos	  = 0; 
	DWORD  oldPos	  = 0; 

	//check file type
	while (*(LPWORD(lpRead+curPos)) != 0x0A0D) 
	{ 
		curPos++;
		if (*(lpRead+curPos) == 0x09 )
		{
			bLogType=FALSE;
			break;
		}
	} 
	
	if (bLogType)
	{
		curPos	   =0;
		oldPos     =0;
		lByteCount =0;

		while( *(lpRead+curPos) == _T(' ') ) 
		{
			curPos++;
		}
		oldPos =curPos;

		while(curPos < uiLen.LowPart)
		{
			//get new line			
			while (*(LPWORD(lpRead+curPos)) != 0x0A0D)
			{
				curPos++;
				if (curPos+1 >= uiLen.LowPart)
					break;
			}
			curPos +=2;//next line head
			
			LPSTR lpLine= (LPSTR)(lpRead +oldPos);
			oldPos = curPos;

			WORD wAddr = (WORD)_tcstol(lpLine, &pszEnd, 16);
			lpLine += 4;
			while( *lpLine == _T(' ')) 
			{
				lpLine++;
			}
			
			WORD wData = (WORD)_tcstol(lpLine, &pszEnd, 16);
			lpLine += 4;
			while( *lpLine == _T(' ')) 
			{
				lpLine++;
			}
			
			DWORD dwTime = (DWORD)_tcstol(lpLine, &pszEnd, 10);
					
			*((LPWORD)lpWrite)		= MAKEWORD(HIBYTE(wAddr), LOBYTE(wAddr));
			*((LPWORD)(lpWrite+2))  = MAKEWORD(HIBYTE(wData), LOBYTE(wData));	
			*((LPDWORD)(lpWrite+4)) = dwTime;
			lpWrite		+=8;
			lByteCount	+=8;
		} 

	}
	else
	{
		curPos	   =0;
		oldPos     =0;
		lByteCount =0;

		while( *(lpRead+curPos) == _T(' ') ) 
		{
			curPos++;
		}
		oldPos =curPos;

		while(curPos < uiLen.LowPart)
		{
			//get new line			
			while (*(lpRead+curPos) != 0x0D || *(lpRead+curPos+1) != 0x0A)
			{
				curPos++;
				if (curPos+1 >= uiLen.LowPart) 
					break;
			}
			curPos +=2;//next line head
			
			LPSTR lpLine= (LPSTR)(lpRead +oldPos);
			oldPos = curPos;

			WORD wAddr = (WORD)_tcstol(lpLine, &pszEnd, 16);
			while( *lpLine != 0x09) 
			{
				lpLine++;
			}
			lpLine++;

			WORD wData = (WORD)_tcstol(lpLine, &pszEnd, 16);
			while( *lpLine != 0x09) 
			{
				lpLine++;
			}
			lpLine++;

			DWORD dwTime = (DWORD)_tcstol(lpLine, &pszEnd, 10);
			
			*((LPWORD)lpWrite)		= MAKEWORD(HIBYTE(wAddr), LOBYTE(wAddr));
			*((LPWORD)(lpWrite+2))  = MAKEWORD(HIBYTE(wData), LOBYTE(wData));	
			*((LPDWORD)(lpWrite+4)) = dwTime;
			lpWrite		+=8;
			lByteCount	+=8;
		} 	
	}
	
	//
	//end convert
	//
	if (lpRead != NULL)
	{
		UnmapViewOfFile(lpRead);
	}
	if (lpWrite != NULL)
	{
		UnmapViewOfFile(lpWrite);
	}
	CloseHandle(hLogMap);
	CloseHandle(hTPMap);
	
    if(hTPFile != INVALID_HANDLE_VALUE  && lpWrite != NULL )
    {        
        // Change to real size		
		LARGE_INTEGER qwLength;
        qwLength.QuadPart = lByteCount;
        SetFilePointer(hTPFile, qwLength.LowPart, &qwLength.HighPart,FILE_BEGIN);
		
        if( !SetEndOfFile(hTPFile) )
        {
            _ASSERTE( GetLastError() == 0 );
        }          
    }
	
	CloseHandle(hTPFile);
	CloseHandle(hLogFile);
}

void CMainFrame::OnFileSaveTP() 
{
	DWORD dwSaveMode = MODE_ALL;
	if(!m_bPlaybackTP)
	{
		CDlgSaveSetting dlg;
		if(dlg.DoModal() == IDOK)
		{
			dwSaveMode = dlg.m_dwSaveMode;
		}
	}
	
    char filter[] = "Log Files (*.tp)|*.tp||";
    CFileDialog dlgFile(FALSE,"*.tp",NULL,OFN_FILEMUSTEXIST,filter);
    if(IDOK == dlgFile.DoModal())
    {
		//teana hu 2010.07.26
		SaveAllMarks();
		//
		CWaitCursor waitCursor;
		//teana hu 2009.10.26
		if(m_bPlaybackTP || dwSaveMode == MODE_CURRENT)
		{
			m_pDspKernel->SaveTP(dlgFile.GetPathName());

			//teana hu 2010.07.26
			char szFile[MAX_PATH] = {0}; 
			char szOldMarkFile[MAX_PATH] = {0};
			char szNewMarkFile[MAX_PATH] = {0};
			strcpy(szFile,m_pDspKernel->GetTpSet()->GetTPFileName());
			if(szFile[0] != '\0')
			{
				strncpy(szOldMarkFile, szFile, strlen(szFile) - 3);
				strcat(szOldMarkFile, ".mark");
			}

			strcpy(szFile, dlgFile.GetPathName());
			if(szFile[0] != '\0')
			{
				strncpy(szNewMarkFile, szFile, strlen(szFile) - 3);
				strcat(szNewMarkFile, ".mark");
			}
			::CopyFile(szOldMarkFile, szNewMarkFile, FALSE);
			//
		}
		else if(dwSaveMode == MODE_RECENTLY)
		{
			char szTPDirectory[MAX_PATH] = {0};
			strcpy(szTPDirectory, m_pDspKernel->GetTpSet()->GetTPDirectory());
			CString strFile2Name = "";

			char szNewDtrector[MAX_PATH] = {0};
			strncpy(szNewDtrector, dlgFile.GetPathName(), strlen(dlgFile.GetPathName())-3);
			if(!CreateDirectory( szNewDtrector, NULL ))
			{
				DWORD dwErr = GetLastError();
				if(dwErr != ERROR_ALREADY_EXISTS)
				{
					AfxMessageBox("Create directory failed!");
					return;
				}
			}

			char szCurTPFile[MAX_PATH] = {0};
			char szTmp[MAX_PATH] = {0};
			strcpy(szTmp, m_pDspKernel->GetTpSet()->GetTPFileName());
			LPSTR lpFind = strrchr(szTmp, '\\');
			if(lpFind != NULL)
			{
				int nLen = lpFind - szTmp;
				strncpy(szCurTPFile, szTmp + nLen, strlen(szTmp) - nLen);
			}

			char szFileName1[MAX_PATH] = {0};
			char szFileName2[MAX_PATH] = {0};
			sprintf(szFileName2, "%s%s", szNewDtrector, szCurTPFile);

			//teana hu 2010.07.26
			char szOldMarkFile[MAX_PATH] = {0};
			char szNewMarkFile[MAX_PATH] = {0};
			//

			if(GetFilesInFolderCount(szTPDirectory, strFile2Name) <= 1)
			{
				m_pDspKernel->SaveTP(szFileName2);
				//teana hu 2010.07.26
				strncpy(szOldMarkFile, szTmp, strlen(szTmp) - 3);
				strcat(szOldMarkFile, ".mark");

				strncpy(szNewMarkFile, szFileName2, strlen(szFileName2) - 3);
				strcat(szNewMarkFile, ".mark");

				::CopyFile(szOldMarkFile, szNewMarkFile, FALSE);

				//
			}
			else
			{
				sprintf(szFileName1, "%s\\%s", szNewDtrector, strFile2Name);			
				strFile2Name = _T("\\") + strFile2Name;
				strFile2Name = szTPDirectory + strFile2Name;	
				::CopyFile(strFile2Name, szFileName1, FALSE);
				m_pDspKernel->SaveTP(szFileName2);
				//teana hu 2010.07.26
				//save mark1
				strncpy(szOldMarkFile, strFile2Name, strFile2Name.GetLength() - 3);
				strcat(szOldMarkFile, ".mark");

				strncpy(szNewMarkFile, szFileName1, strlen(szFileName1) - 3);
				strcat(szNewMarkFile, ".mark");

				::CopyFile(szOldMarkFile, szNewMarkFile, FALSE);

				//save mark2
				ZeroMemory(szOldMarkFile, MAX_PATH);
				ZeroMemory(szNewMarkFile, MAX_PATH);
				strncpy(szOldMarkFile, szTmp, strlen(szTmp) - 3);
				strcat(szOldMarkFile, ".mark");
				
				strncpy(szNewMarkFile, szFileName2, strlen(szFileName2) - 3);
				strcat(szNewMarkFile, ".mark");

				::CopyFile(szOldMarkFile, szNewMarkFile, FALSE);

				//
			}		
		}
		else //SAVE ALL
		{
			char szTPDirectory[MAX_PATH] = {0};
			strcpy(szTPDirectory, m_pDspKernel->GetTpSet()->GetTPDirectory());
			if(szTPDirectory[0] != '\0')
			{		
				char szTo[MAX_PATH] = {0};
				char szTemp[MAX_PATH] = {0};
				strcpy(szTemp, dlgFile.GetPathName());
				strncpy(szTo, szTemp, strlen(szTemp) - 3);
				if(!CreateDirectory( szTo, NULL ))
				{
					DWORD dwErr = GetLastError();
					if(dwErr != ERROR_ALREADY_EXISTS)
					{
						AfxMessageBox("Create directory failed!");
						return;
					}
				}

				CopyDirectory(szTPDirectory, szTo);
				char szNewPath[MAX_PATH] = {0};
				strcpy(szNewPath, szTo);
				memset(szTemp, 0, MAX_PATH);
				memset(szTo, 0, MAX_PATH);
				strcpy(szTemp, m_pDspKernel->GetTpSet()->GetTPFileName());
				
				LPSTR lpFind = strrchr(szTemp, '\\');
				if(lpFind != NULL)
				{
					int nLen = lpFind - szTemp;
					strncpy(szTo, szTemp + nLen, strlen(szTemp) - nLen);
					strcat(szNewPath, szTo);
				}
				m_pDspKernel->SaveTP(szNewPath);
				//teana hu 2010.07.26
				char szFile[MAX_PATH] = {0}; 
				char szOldMarkFile[MAX_PATH] = {0};
				char szNewMarkFile[MAX_PATH] = {0};
				strcpy(szFile,m_pDspKernel->GetTpSet()->GetTPFileName());
				if(szFile[0] != '\0')
				{
					strncpy(szOldMarkFile, szFile, strlen(szFile) - 3);
					strcat(szOldMarkFile, ".mark");
				}
				
				strncpy(szNewMarkFile, szNewPath, strlen(szNewPath) - 3);
				strcat(szNewMarkFile, ".mark");
				::CopyFile(szOldMarkFile, szNewMarkFile, FALSE);
				//
			}
			else
			{
				m_pDspKernel->SaveTP(dlgFile.GetPathName());
				//teana hu 2010.07.26
				char szFile[MAX_PATH] = {0}; 
				char szOldMarkFile[MAX_PATH] = {0};
				char szNewMarkFile[MAX_PATH] = {0};
				strcpy(szFile,m_pDspKernel->GetTpSet()->GetTPFileName());
				if(szFile[0] != '\0')
				{
					strncpy(szOldMarkFile, szFile, strlen(szFile) - 3);
					strcat(szOldMarkFile, ".mark");
				}
				ZeroMemory(szFile, MAX_PATH);
				strcpy(szFile, dlgFile.GetPathName());
				if(szFile[0] != '\0')
				{
					strncpy(szNewMarkFile, szFile, strlen(szFile) - 3);
					strcat(szNewMarkFile, ".mark");
				}
				::CopyFile(szOldMarkFile, szNewMarkFile, FALSE);
				//
			}			
		}
    }	
	
}

long CMainFrame::GetFreeDiskSpaceInKB(CString strPath)
{
    DWORD dwFreeClusters, dwBytesPerSector, dwSectorsPerCluster, dwClusters;

	int nPos=strPath.FindOneOf(_T("\\")) ;
	ASSERT( -1 != nPos );
	if( -1 == nPos ) return (-1);
	
	CString strRootName= strPath.Left(nPos+1); 

    if (!GetDiskFreeSpace(strRootName,
		&dwSectorsPerCluster,
		&dwBytesPerSector,
		&dwFreeClusters,
		&dwClusters)) {
	    MessageBox(TEXT("Can't measure free disk space."), TEXT("Warning"),
	    		MB_OK | MB_ICONINFORMATION);
	    return (-1);
    }

	int nSize= 
		MulDiv (dwSectorsPerCluster * dwBytesPerSector,dwFreeClusters,1024);
		
    return nSize;
}

LRESULT CMainFrame::OnClearState(WPARAM wParam, LPARAM lParam)
{
	if(m_pView == NULL)
	{
		return 0;
	}
	//teana hu 2009.12.22
	if(g_theApp.m_pKeyInfoDoc != NULL)
	{
		g_theApp.m_pKeyInfoDoc->UpdateAllViews(NULL,WM_UPDATE_CLEAR);
		g_theApp.m_pKeyInfoDoc->UpdateAllViews(NULL,WM_UPDATE_SETTIMER);
	}
	//
	if(g_pFindDoc != NULL)
	{
		BOOL bExist = FALSE;
		for(int i=0; i<MAX_CHART_WND_NUM; i++)
		{
			if( g_theApp.m_pFindWnd[i] != NULL )
			{
				bExist = TRUE;
				break;
			}
		}
		if(!bExist)
		{
			return 0;
		}

		g_pFindDoc->UpdateAllViews(NULL,WM_UPDATE_CLEAR_STATE);
		g_pFindDoc->UpdateAllViews(NULL,WM_UPDATE_CLEAR);
	}
	return 1;
}

//teana hu 2010.07.26
LRESULT CMainFrame::OnSaveMarkFile(WPARAM wParam, LPARAM lParam)
{
	SaveAllMarks();
	return 1;
}

void CMainFrame::SetStatusBarData()
{
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_DUMP);
	HICON m_hIcon = NULL;
	HINSTANCE m_hInstance = AfxGetInstanceHandle();

	static int nCount = 1;

	if(nCount % 3 == 1)
	{
		m_hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDR_MEM_FIR));
	}
	else if(nCount % 3 == 2)
	{
		m_hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDR_MEM_SEC));
	}
	else
	{
		m_hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDR_MEM_THI));
		
	}
	m_wndStatusBar.GetStatusBarCtrl().SetIcon(nIndex, m_hIcon);

	nCount ++;

}

LRESULT CMainFrame::OnReflashStatusBar(WPARAM wParam, LPARAM lParam)
{
	SetStatusBarData();

	return 1;
}

LRESULT CMainFrame::OnNoticeEndian(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	
    if(m_bConnected)
    {
		//teana hu 2009.08.12
		if(m_SettingsDlg.m_nChannelType == TOOL_CHANNEL_UART)
		{
			strText.Format("COM%d   %d",m_SettingsDlg.m_nPortIndex,m_SettingsDlg.m_nBaudIndex);
		}
		else if(m_SettingsDlg.m_nChannelType == TOOL_CHANNEL_SOCKET_CLIENT)
		{
			strText.Format("IP: %s, Port:%d",m_SettingsDlg.m_strIpAddr,m_SettingsDlg.m_nSocketPort);
		}
    }
    else
    {
        strText = "Disconnected";
    }
	int nEndian = (int)lParam;
	if(nEndian == 0)
	{
		strText += ", Little Endian";
	}
	else if(nEndian == 1)
	{
		strText += ", Big Endian";
	}
	else
	{
		strText += ", Default Endian";
	}

    m_wndStatusBar.SetPaneText(1,strText);

	return 1;
}

void CMainFrame::OnPlaybackOrg() 
{
	// TODO: Add your command handler code here
/*
	//teana hu 2012.05.25
	CWinApp* pApp  = AfxGetApp();
	m_pDspKernel->SetGenerateFlag(pApp->GetProfileInt("GenerateTime", "GenerateTime", 1),
		pApp->GetProfileInt("GenerateOrgFile", "GenerateOrgFile", 1));
	//
	*/
	//polo.jiang change cfg from reg to ini
	m_pDspKernel->SetGenerateFlag(m_SettingsDlg.m_dwGenerateTPTime,m_SettingsDlg.m_bGenerateOrgFile);	

	CDlgPlaybackOrg dlg;
	dlg.DoModal();
	
}

void CMainFrame::OnUpdatePlaybackOrg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStop);
	
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default

    switch ( pCopyDataStruct->dwData )
    {
	case WM_RENEW_DSPLOG:
		return OnRewDSPLog((WPARAM)pCopyDataStruct->lpData, (LPARAM)pCopyDataStruct->cbData);
    case WM_AUTO_SAVE_AS:
        return OnAutoSaveAs( (WPARAM)pCopyDataStruct->lpData, (LPARAM)pCopyDataStruct->cbData );
	case WM_AUTO_CONNECT:
		return OnAutoConnect( (WPARAM)pCopyDataStruct->lpData, (LPARAM)pCopyDataStruct->cbData );
	case WM_AUTO_DISCONNECT:
		return OnAutoDisConnect( (WPARAM)pCopyDataStruct->lpData, (LPARAM)pCopyDataStruct->cbData );
	case WM_FIND_RELATED_ITEM:
		return OnFindRelatedItem((WPARAM)pCopyDataStruct->lpData, (LPARAM)pCopyDataStruct->cbData);
    default:
        break;
    }

    return FALSE;
}
/********************************************************************************/
/********Function: save log according to the message
/********author: tony
/*******date:2010.8.15
/*********************************************************************************/

LRESULT CMainFrame::OnRewDSPLog(WPARAM wParam, LPARAM lParam)
{
	char sfile[MAX_PATH] = {0};
	char sfilelex[MAX_PATH] = {0};
	char slf[MAX_PATH]={0};
	unsigned int id;
	HANDLE m_lf;
	BOOL bfd, bfl;

	m_lf = INVALID_HANDLE_VALUE;
	OnStop();
	memcpy(sfile, (void *)wParam, (int)lParam);
	
	strcpy(sfilelex, sfile);
	strcat(sfilelex, ".tp");
	m_pDspKernel->SaveTP(sfilelex);
	strcpy(slf, m_pDspKernel->GetTpSet()->GetTPDirectory());
    if(!strlen(slf))
	{
		return 0;
	}
	if(strcmp(slf, m_lfd))
	{
		memset(m_lfd, 0, MAX_PATH);
		strcpy(m_lfd, slf);
        strcat(slf, "\\*.*");
		CFileFind fd;
		bfd = fd.FindFile(slf);
		if(bfd)
		{
			bfl = fd.FindNextFile();
			bfl = fd.FindNextFile();
			bfl = fd.FindNextFile();
			id =0;
			while(bfl)
			{
				if(!fd.IsDirectory())
				{
					CString it;
					it = fd.GetFilePath();
					memset(sfilelex, 0, MAX_PATH); 
					sprintf(sfilelex, "%s%d%s", sfile, ++id, ".tp");
					m_lf = ::CreateFile(sfilelex,
						GENERIC_READ | GENERIC_WRITE,
						0,
						NULL,
						OPEN_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL);
					if(m_lf == INVALID_HANDLE_VALUE)
					{
						return 0;
					}
					else
					{
						CloseHandle(m_lf);
						m_lf = INVALID_HANDLE_VALUE;
						CopyFile(it, sfilelex, FALSE);
						DeleteFile(it);
						
					}
				}
			
				bfl = fd.FindNextFile();
				
			}

		}
		fd.Close();
	}
	OnStart();
    return 1;	
}

LRESULT CMainFrame::OnAutoSaveAs(WPARAM wParam, LPARAM lParam)
{
	/*
	char szFileName[MAX_PATH] = {0};
	memcpy(szFileName, (void *)wParam, (int )lParam);
	m_pDspKernel->SaveTP(szFileName);
	*/
	char szDstDir[MAX_PATH] = {0};
	char szTPLogDir[MAX_PATH] = {0};
	BOOL bRet = FALSE;
	memcpy(szDstDir, (void *)wParam, (int )lParam);
	strcpy(szTPLogDir, m_pDspKernel->GetTpSet()->GetTPDirectory());
	bRet =CreateDirectory( szDstDir, NULL );
	if(bRet)
	{
		bRet =CopyDirectory(szTPLogDir,szDstDir);
	}

	return bRet ? 1 : 0;
}

LRESULT CMainFrame::OnAutoConnect(WPARAM wParam, LPARAM lParam)
{
	SendMessage(WM_COMMAND, ID_CONNECT_DSP, NULL);
	SendMessage(WM_COMMAND, ID_START, NULL);

	return 1;
}

LRESULT CMainFrame::OnAutoDisConnect(WPARAM wParam, LPARAM lParam)
{
	SendMessage(WM_COMMAND, ID_STOP, NULL);
	SendMessage(WM_COMMAND, ID_DISCONNECT_DSP, NULL);

	return 1;
}

LRESULT CMainFrame::OnFindRelatedItem(WPARAM wParam, LPARAM lParam)
{
	char szTime[50] = {0};
	strncpy(szTime, (LPSTR)wParam, (ULONG)lParam);
	ULONG ulTime = atoi(szTime);
	int nItem = -1;

	if(m_dwGenerateTPTime == D0A0_TIME)
	{
		ULONG ulCount = m_pDspKernel->GetTpSet()->GetTpCount();
		ULONG ulLeft = 1;
		ULONG ulRight = ulCount -1;
		
		
		while (ulLeft < ulRight)
		{
			ULONG ulMiddle = (ulLeft + ulRight) / 2;
			ULONG ulCurTime = m_pDspKernel->GetTpSet()->GetTPTime(ulMiddle);
			if(fabs(ulTime - ulCurTime) <= 1000)
			{
				nItem = ulMiddle;
				break;
			}
			if (ulTime > ulCurTime)
			{
				ulLeft = ulMiddle + 1;
			}
			else
			{
				ulRight = ulMiddle - 1;
			}
		}
	}
	else
	{
		ULONG ulStartTime = 0;
		if(m_pDspKernel->GetTpSet()->GetTPAddr(0) == 0 && m_pDspKernel->GetTpSet()->GetTPData(0) == 0)
		{
			ulStartTime = m_pDspKernel->GetTpSet()->GetTPTime(0);
		}
		else
		{
			return 0;
		}
		
		ULONG ulCount = m_pDspKernel->GetTpSet()->GetTpCount();
		ULONG ulLeft = 1;
		ULONG ulRight = ulCount -1;
		
		while (ulLeft < ulRight)
		{
			ULONG ulMiddle = (ulLeft + ulRight) / 2;
			ULONG ulCurTime = m_pDspKernel->GetTpSet()->GetTPTime(ulMiddle);
			if(fabs(ulTime - (ulStartTime + ulCurTime)) <= 1000)
			{
				nItem = ulMiddle;
				break;
			}
			if (ulTime > (ulCurTime + ulStartTime))
			{
				ulLeft = ulMiddle + 1;
			}
			else
			{
				ulRight = ulMiddle - 1;
			}
		}
	}

	if(nItem == -1)
	{
		AfxMessageBox("No related item to match!");
		return 0;
	}
	m_pView->GotoItem(nItem);
	return 1;
}

BOOL CMainFrame::OpenMarkFile(WORD wOpenMode)
{
	char szFile[MAX_PATH] = {0}; 
	char szMarkFile[MAX_PATH] = {0};
	strcpy(szFile,m_pDspKernel->GetTpSet()->GetTPFileName());
	
	if(szFile[0] != '\0')
	{
		strncpy(szMarkFile, szFile, strlen(szFile) - 3);
		strcat(szMarkFile, ".mark");
	}
	else
	{
		return FALSE;
	}

	char szMode[5] = {0};

	if(wOpenMode == READ_MODE)
	{
		strcpy(szMode, "rb");
	}
	else
	{
		strcpy(szMode, "wb");
	}
	
	m_fpMark = fopen(szMarkFile, szMode);

	if(m_fpMark == NULL)
	{
		return FALSE;
	}
	return TRUE;
}

void CMainFrame::WriteMarkFile(DWORD dwIndex, LPSTR lpString, DWORD dwSize)
{
	if(m_fpMark != NULL)
	{
		fwrite(&dwIndex, sizeof(DWORD), 1, m_fpMark);
		fwrite(&dwSize, sizeof(DWORD), 1, m_fpMark);
		fwrite(lpString, dwSize, 1, m_fpMark);
		fflush(m_fpMark);
	}
}

void CMainFrame::LoadMarkFile()
{
	if(m_fpMark != NULL)
	{
		char szMark[MARK_LEN] = {0};
		DWORD dwItem = (DWORD)-1;
		DWORD dwMarkLen = 0;
		
		while(!feof(m_fpMark))
		{
			fread(&dwItem, sizeof(DWORD), 1, m_fpMark);
			fread(&dwMarkLen, sizeof(DWORD), 1, m_fpMark);
			fread(szMark, dwMarkLen, 1, m_fpMark);
			
			m_pView->AddMarkItem(dwItem, szMark);
			memset(szMark, 0, MARK_LEN);
		}
	}
}

void CMainFrame::CloseMarkFile()
{
	if(m_fpMark != NULL)
	{
		fclose(m_fpMark);
		m_fpMark = NULL;
	}
}

void CMainFrame::SaveAllMarks()
{
	int nCount = m_pView->GetMarkCount();

	if(nCount > 0 && OpenMarkFile(WRITE_MODE))
	{
		char szMark[MARK_LEN] = {0};
		int nItem = -1;
		for(int i = 0; i < nCount; i++)
		{
			strcpy(szMark, m_pView->GetItemMark(i, nItem));
			if(szMark[0] != '\0')
			{
				WriteMarkFile(nItem, szMark, strlen(szMark));
			}
			memset(szMark, 0, MARK_LEN);
			nItem = -1;
		}

		CloseMarkFile();
		m_pView->SendMessage(WM_COMMAND, ID_REMOVE_ALL_MARKS, 0);
	}
}

int CMainFrame::GetFilesInFolderCount(CString strFolder, CString& strFileName)
{
    CFileFind ff;
    BOOL bFinded = ff.FindFile(strFolder + _T("\\*.*"));
    int nCount = 0;
	CString strTemp;
    while (bFinded)
    {
        bFinded = ff.FindNextFile();
		strTemp = ff.GetFileName();

        if (!ff.IsDirectory() && strTemp.Find(".tp") != -1)
		{
			nCount ++;
		}

		if(!bFinded)
		{
			break;
		}
		if(strTemp.Find(".tp") != -1)
		{
			strFileName = strTemp;
		}
    }

    ff.Close();

    return nCount;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_LBUTTONDOWN == pMsg->message) 
	{
		CRect rectFindUp;
		CRect rectFindDown;
		
		m_wndToolBar.GetItemRect(11,&rectFindUp);	//ID_TP_FIND_UP
		ClientToScreen(&rectFindUp);

		m_wndToolBar.GetItemRect(12,&rectFindDown);	//ID_TP_FIND_DOWN
		ClientToScreen(&rectFindDown);
		
		if(rectFindUp.PtInRect(pMsg->pt) )			//Auto FindUp
		{   
			m_pView->SetAutoFindTimer(TRUE);
		} 
		if(rectFindDown.PtInRect(pMsg->pt) )		//Auto FindDown
		{   
			m_pView->SetAutoFindTimer(FALSE);
		} 
		
	}
	else if(WM_LBUTTONUP == pMsg->message) 
	{		
		m_pView->KillAutoFindTimer();
	}
	
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}



void CMainFrame::UpdatedTitleInfo()
{
	CString strTitle;
	if( m_SettingsDlg.m_nChannelType == TOOL_CHANNEL_SOCKET_CLIENT )
	{
		
		m_nLogType = m_SettingsDlg.m_nLogType;
	}
	else
	{
		m_nLogType = E_COMMON_DSP;
	}

	switch(m_nLogType)
	{
	case E_LTE_DSP:
		strTitle ="DSP Logger LTE - [Test Points]";
		break;
	case E_TG_DSP:
		strTitle ="DSP Logger TG - [Test Points]";
		break;
	case E_W_DSP:
		strTitle ="DSP Logger W - [Test Points]";
		break;
	case E_AG_DSP:
		strTitle ="DSP Logger AG - [Test Points]";
		break;
	default:
		strTitle ="DSP Logger COMMON - [Test Points]";
		break;
	}

	CString strVersion;
	_TCHAR szTitle[_MAX_PATH]={0};
	_TCHAR szConfigFile[_MAX_PATH]={0};
	_stprintf(szConfigFile,_T("%s\\DspLogger.ini"),GetModuleDir(FALSE));
	
	GetPrivateProfileString(_T("GUI"),_T("Title"),_T(""),szTitle,_MAX_PATH,szConfigFile);
	BOOL bShowVer = GetPrivateProfileInt(_T("GUI"),_T("ShowVer"),1,szConfigFile);      
	
	if(bShowVer)
	{
		CXVerInfo ver;
		strVersion = ver.GetSPRDVersionString();
	}

	
	if(_tcslen(szTitle) != 0)
	{
		strTitle += _T("[");
		strTitle += szTitle;
		strTitle += _T("] ");
	}	
	
	if(bShowVer)
	{
#ifdef _DEBUG
		strTitle += _T("- D");
#else 
		strTitle += _T("- R");
#endif
		strTitle += strVersion;	
	}

	m_strTitle = strTitle;
	SetWindowText(strTitle);	
	
}