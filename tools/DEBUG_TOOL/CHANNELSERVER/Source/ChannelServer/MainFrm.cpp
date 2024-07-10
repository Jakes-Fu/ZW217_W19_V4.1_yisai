// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Function.h"
#include "ChannelServer.h"
#include "MainFrm.h"
#include <dbt.h>
#include <Winsock2.h>
#include "CrashReport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
static FILE *fpLog = NULL;
#endif

//lint -esym(613,*::m_pSettings)

// Thread to receive high level data
UINT HiLvlRcvThread( LPVOID pParam )
{
	CControlCenter* lpCtrl = (CControlCenter*)pParam;
	if( NULL == lpCtrl )
	{
		return 0xFFFFFFFF;
	}

	MSG msg;
	while( GetMessage( &msg,NULL,0,0 ) )
	{
		if( msg.message == WM_SCM_SENDDATA )
		{
			lpCtrl->OnHiLvlData( (void*)msg.wParam,msg.lParam );
		}
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_COMMAND(ID_CHANSVR_CONFIG, OnChansvrConfig)
    ON_WM_CLOSE()
    ON_WM_TIMER()
    ON_UPDATE_COMMAND_UI(ID_CHANSVR_CONFIG, OnUpdateChansvrConfig)
    ON_COMMAND(ID_CONNECT_MSSIM, OnConnectMssim)
    ON_UPDATE_COMMAND_UI(ID_CONNECT_MSSIM, OnUpdateConnectMssim)
    ON_COMMAND(ID_DISCONNECT_MSSIM, OnDisconnectMssim)
    ON_COMMAND(ID_LOG_DATA, OnLogData)
    ON_UPDATE_COMMAND_UI(ID_LOG_DATA, OnUpdateLogData)
    ON_UPDATE_COMMAND_UI(ID_DISCONNECT_MSSIM, OnUpdateDisconnectMssim)
    ON_UPDATE_COMMAND_UI(ID_ASSERT, OnUpdateAssert)
//}}AFX_MSG_MAP
/*lint -save -e665 */
    ON_MESSAGE(WM_NIF_CHANSVR,OnNifChanSvr)
    ON_MESSAGE(WM_SCM_SENDDATA,OnCommSockData)
    ON_MESSAGE(WM_CMM_CLOSE,OnDisconnectMssim2)
	ON_MESSAGE(WM_NOTIFY_ASSERT,OnAssert)
	ON_MESSAGE( WM_EVENT_ENDIAN,OnEndianEvent)
	ON_MESSAGE(WM_RECV_DEVICE_CHANGE, OnRecvDeviceChange)
	ON_MESSAGE(WM_PORT_RESET, OnPortReset)
	ON_MESSAGE(WM_FILE_READ_OVER, OnFileReadOver)
/*lint -restore */
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
    //Load resources
    m_hInstance = AfxGetInstanceHandle();
    
    // m_hIconLowest = AfxGetApp()->LoadIcon(IDI_LOWEST);
    m_hIconInactive = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_INACTIVE) );
    m_hIconActive   = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_ACTIVE) );
    m_hIconLower    = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_LOWER)  );
    m_hIconLow      = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_LOW)    );
    m_hIconDown   = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_DOWN) );
    m_hIconUp     = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_UP)   );
    m_hIconUpDown   = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_UPDOWN) );
	m_hIconUpAlert     = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_UP_ALERT)   );
    m_hIconUpDownAlert   = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_UPDOWN_ALERT) );
    m_hIconAssert  = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_ASSERT));
    
    m_cMenuChanSvr.LoadMenu(IDR_MAINFRAME);
    
    m_bInConfigure    = FALSE;
    m_bInConnectPro   = FALSE;
	m_bInConnectLog   = FALSE;
	m_bServerActive   = FALSE;
    
	m_pProChan  = NULL;
	m_pLogChan  = NULL;
    m_pSvrChan  = NULL;
    
    m_nEndian        = ENDIAN_LITTLE;
    m_nRealEndian    = m_nEndian;

    m_bInAssert         = FALSE;
    
    //Added For Data Showing
    m_bEnableLogData    = FALSE;
    //m_pConvertor = NULL;
	
	m_dwDownData = 0;
	m_dwUpData = 0;
	m_dwTickcount= 0;

	memset( &m_tndChannelServer,0,sizeof( m_tndChannelServer ) );

	m_dwRcvThreadId = 0;
	//m_bSocketTimerActive =FALSE;
	m_bSocketMonitor = FALSE;
	m_hSocketMonitor = NULL;

	InitializeCriticalSection(&m_csConnect);
	InitializeCriticalSection(&m_csDisconnect);

	m_pDevHound = NULL;

	CreateDevHound(&m_pDevHound);
}

CMainFrame::~CMainFrame()
{
    /*lint -save -e1540 */
    // All pointer member will be freed in OnClose
    /*lint -restore */
	DeleteCriticalSection(&m_csConnect);
	DeleteCriticalSection(&m_csDisconnect);
	StopSocketMonitor();
	if(m_pDevHound)
	{
		ReleaseDevHound(m_pDevHound);
	}

	if(m_pProChan)
	{
		ReleaseProtoChannel(m_pProChan);
		m_pProChan = NULL;
	}

	if(m_pLogChan)
	{
		ReleaseProtoChannel(m_pLogChan);
		m_pLogChan = NULL;
	}

	if(m_pSvrChan)
	{
		ReleaseChannel(m_pSvrChan);
		m_pSvrChan = NULL;
	}
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    
    cs.style = WS_POPUP; //lint !e569               //Make the main window invisible
    cs.dwExStyle |= WS_EX_TOOLWINDOW;   //Hide the task window
    
    if(!CFrameWnd::PreCreateWindow(cs))
        return FALSE;
    
    return TRUE;
}

int CrashCallBack (_EXCEPTION_POINTERS*)
{
	// Just tell the CrashReport lib doesn't show the default 
	// message box.
	return CRCB_NEED_NO_PROCESS;
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


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

	CString strLog;
    
	if(m_dlgCfg.m_PageCfg.m_bUILogEnable)
	{
		m_spLog.Open(_T("ChannelServer"),ISpLog::defaultTextFlag | ISpLog::modeNoTruncate);
		
		strLog.Format(_T("===================start========================="));
		Log(strLog);
		strLog.Format(_T("CMainFrame::OnCreate +++"));
		Log(strLog);
	}

    TCHAR szTip[MAX_PATH] = {0};
	//BOOL m_bParserOpened = FALSE;
    //CString strParserFileName; 
    
    CWinThread* pThread = AfxBeginThread( HiLvlRcvThread,&m_CC );
	if( pThread )
	{
		m_dwRcvThreadId = pThread->m_nThreadID;
	}

#if defined(_DEBUG)
#else
	HOOKCALLBACK lpcb = NULL;
	if( m_dlgCfg.m_PageCfg.m_bAutoTest )
	{
		lpcb = CrashCallBack;
	}
	InitCrashReport( NULL,FALSE,NULL,lpcb );
#endif

	OpenServer();
	/*
    try
    {
        IDPSrvFactory *pDPSrvFactory = NULL;
        
        DllGetClassFactoryObject((void **)&pDPSrvFactory);
        if(NULL != pDPSrvFactory)
        {
            m_pConvertor = NULL;
            pDPSrvFactory->CreateDP(&m_pConvertor);
            pDPSrvFactory->Release();
            
            if(NULL == m_pConvertor)
            {
                return -1;
            }
            
        }
        
        TCHAR szFileName[MAX_PATH];        
        GetAppPath(szFileName);
        _tcscat(szFileName, _T("\\Message_define.def"));
		strParserFileName = szFileName;
        m_pConvertor->Init();
#ifdef _UNICODE
		char szFileNameA[MAX_PATH];
		WideCharToMultiByte( CP_ACP,0,szFileName,_tcslen( szFileName ) + 1,szFileNameA,MAX_PATH,NULL,NULL );
		m_pConvertor->SetFile(szFileNameA);
#else
        m_pConvertor->SetFile((LPSTR)(LPCTSTR)szFileName);
#endif
        m_bParserOpened = TRUE;
    }
	catch(CCDFException *e) //lint !e1752
	{
        TRACE(_T("%s"), m_pConvertor->GetErrorMessage(e));
        e->Delete();
        m_bParserOpened = FALSE;
    }

	if( m_bParserOpened )
	{
		m_CC.m_pConvertor = m_pConvertor;
	}

    //End of Endian Converting
	*/
    
	m_CC.m_hWnd = m_hWnd;
	m_CC.m_pWnd = (CWnd*)this;

    m_nEndian     = m_dlgCfg.m_PageCfg.GetEndian() ;
	m_nRealEndian = m_nEndian;
	/*
	if( m_nEndian <= BIG_ENDIAN )
	{
	    m_pConvertor->SetEndian((EndianType)m_nEndian);
	}
	*/

	if( !CreateProtoChannel( &m_pProChan, PT_DIAG ) )
	{
		AfxMessageBox(_T("Create diag channel failed"));
		CloseServer();
		return -1;
	}
	else
	{
		m_CC.m_lpLowChan = m_pProChan;
		m_pProChan->AddObserver( &m_CC );
	}

	if( !CreateProtoChannel( &m_pLogChan, PT_SMP) )
	{
		AfxMessageBox(_T("Create log channel failed"));
		CloseServer();
		return -1;
	}
	else
	{
		m_CC.m_lpLogChan = m_pLogChan;
		m_pLogChan->AddObserver( &m_CC );
	}

	
    memset(&m_tndChannelServer,0,sizeof(NOTIFYICONDATA));
    
    m_tndChannelServer.cbSize = sizeof(NOTIFYICONDATA);
    m_tndChannelServer.hWnd   = m_hWnd;
    m_tndChannelServer.uID    = IDR_MAINFRAME;
    m_tndChannelServer.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    m_tndChannelServer.hIcon  = m_hIconInactive;
    m_tndChannelServer.uCallbackMessage = WM_NIF_CHANSVR;
    _stprintf(szTip,_T("Channel Server: Not Connect To MS"));
    _tcscpy(m_tndChannelServer.szTip ,szTip);
    
    Shell_NotifyIcon(NIM_ADD,&m_tndChannelServer);	

	if(FILES != m_dlgCfg.m_PageCfg.m_nChannelType)
	{
		  OnConnectMssim();
	}  
    
    SetTimer(SYSTEM_TIMER,SYSTEM_TIMER_INTERVAL,NULL);

	if(WINSOCKET == m_dlgCfg.m_PageCfg.m_nChannelType && !m_bInConnectPro )
	{
		StartSocketMonitor();
	}

	//teana hu 2012.07.18
	//m_devHound.SetReceiver((DWORD)this->GetSafeHwnd(), WM_RECV_DEVICE_CHANGE, FALSE);
	//m_devHound.Start();
	//
	m_pDevHound->SetReceiver((DWORD)this->GetSafeHwnd(), WM_RECV_DEVICE_CHANGE, FALSE);
	m_pDevHound->Start();

	strLog.Format(_T("CMainFrame::OnCreate ---"));
	Log(strLog);
    
    return 0;
}

LRESULT CMainFrame::OnNifChanSvr(WPARAM wParam,LPARAM lParam)
{
    UINT   uID;
    UINT   uMouseMsg;
    POINT  ptPoint;
    
    uID       = (UINT)wParam;
    uMouseMsg = (UINT)lParam;
    
    if(uID==IDR_MAINFRAME)
    {
        if(uMouseMsg == WM_RBUTTONDOWN)
        {
            
            // get the mouse point 
            GetCursorPos(&ptPoint); 
            
            // Actives the back ground of the window
            SetForegroundWindow(); 
            
            // Display the popup menu
            m_cMenuChanSvr.GetSubMenu(0)->TrackPopupMenu( 
                TPM_LEFTBUTTON|TPM_RIGHTBUTTON,  
                ptPoint.x , ptPoint.y, this, NULL);  
            
            // 增加一个额外消息，使菜单操作正确 
            PostMessage(WM_USER, 0, 0); 
            
        }
        else if(uMouseMsg == WM_LBUTTONDOWN)
        {
			if( m_bInConfigure )
			{
				// The config dialog is already opened,bring it to top		
				SetForegroundWindow();
			}
			else
			{
				//if clicks the left button of mouse
				//on the main icon  then enter configuration
				SendMessage(WM_COMMAND,ID_CHANSVR_CONFIG);
			}            
        }        
    }  

	return 0;
}

void CMainFrame::OnChansvrConfig() 
{
	CString strLog;
	strLog.Format(_T("CMainFrame::OnChansvrConfig +++"));
	Log(strLog);

    if(!m_bInConfigure)
    {
        m_bInConfigure = TRUE;
        UINT nServerPort = m_dlgCfg.m_PageCfg.m_nServerPort;
		UINT nChannelType = m_dlgCfg.m_PageCfg.m_nChannelType;
	    if(m_dlgCfg.DoModal () == IDOK)
        {
            m_nEndian = m_dlgCfg.m_PageCfg.GetEndian();
			m_nRealEndian = m_nEndian;
            m_CC.m_nTimeout = m_dlgCfg.m_PageCfg.m_nTimeouts;
            
            if( nServerPort != m_dlgCfg.m_PageCfg.m_nServerPort )
            {
                // User changed the server port,we must close the previous
                // server port and open the new port.All client must be 
                // reconnect the server
                CloseServer();
                OpenServer();                
            }            
   	
			if(nChannelType == WINSOCKET)
			{
				StopSocketMonitor();			
			}
						
			OnDisconnectMssim();

			// [[
			// after disconnect MSSIM, maybe some Disconnect Message is comming soon,
			// that will cause invoking OnDisconnectMssim after OnConnectMssim.
			MSG msg;
			while(PeekMessage(&msg,this->GetSafeHwnd(),0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			// ]]

			OnConnectMssim();		
        }
        
        m_bInConfigure = FALSE;
	}

	strLog.Format(_T("CMainFrame::OnChansvrConfig ---"));
	Log(strLog);
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bInConfigure)
	{
		::MessageBox(::GetDesktopWindow(),_T("The Configure is Running,Please Exit Configure First!")
			       ,_T("Exclamation"),MB_OK | MB_ICONEXCLAMATION);
		return;
	}

    if(FALSE == m_dlgCfg.m_PageCfg.SaveConfiguration())
	{
		::MessageBox(::GetDesktopWindow(),_T("The Configure File is not writable, Save Configure Failed!")
			       ,_T("Exclamation"),MB_OK | MB_ICONEXCLAMATION);
	}

	OnDisconnectMssim();

	StopSocketMonitor();

	//Delete the program icon
    CloseServer();
    
	//teana hu 2012.07.18
	//m_devHound.Stop();
	//	
	m_pDevHound->Stop();

	Shell_NotifyIcon(NIM_DELETE,&m_tndChannelServer);

	/*
    if(NULL != m_pConvertor)
    {
        m_pConvertor->Release();
        m_pConvertor = NULL;
    }
	*/
	CString strLog;
	strLog.Format(_T("===================stop========================="));
	Log(strLog);

	m_spLog.Close();

	CFrameWnd::OnClose();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	DWORD dwDownData = m_CC.m_dwDownData - m_dwDownData;
	DWORD dwUpData = m_CC.m_dwUpData - m_dwUpData;

	m_dwDownData = m_CC.m_dwDownData;
	m_dwUpData = m_CC.m_dwUpData;
	

	if( dwUpData < 10240 )
	{
		_stprintf( m_tndChannelServer.szTip,_T("Up:%d B/s\nDown:%d B/s"),dwUpData,dwDownData );
	}
	else
	{
		dwUpData /= 1024;
		_stprintf( m_tndChannelServer.szTip,_T("Up:%d KB/s\nDown:%d B/s"),dwUpData,dwDownData );
	}

	if( (m_bInConnectPro || m_bInConnectLog) && !m_bInAssert )
	{
		if( dwUpData > 0 && dwDownData > 0 )
		{
			if(dwUpData > (DWORD)(m_dlgCfg.m_PageCfg.m_nUpDataSpeedAlert) )
			{
				m_tndChannelServer.hIcon = m_hIconUpDownAlert;
			}
			else
			{
				m_tndChannelServer.hIcon = m_hIconUpDown;
			}
			
		}
		else if( dwUpData > 0 && dwDownData == 0 )
		{			
			if(dwUpData > (DWORD)(m_dlgCfg.m_PageCfg.m_nUpDataSpeedAlert) )
			{
				m_tndChannelServer.hIcon = m_hIconUpAlert;
			}
			else
			{
				m_tndChannelServer.hIcon = m_hIconUp;
			}
		}
		else if( dwDownData > 0 && dwUpData == 0 ) //lint !e774
		{
			m_tndChannelServer.hIcon = m_hIconDown;
		}
		else
		{
			m_tndChannelServer.hIcon = m_hIconActive;
		}
	}

	Shell_NotifyIcon(NIM_MODIFY,&m_tndChannelServer);	

	m_CC.OnTimerInterval();

	if(m_dlgCfg.m_PageCfg.m_bEnableUartReconnect && (m_bInConnectPro || m_bInConnectLog) )
	{
		if(dwUpData >  0 || m_dwTickcount == 0)
		{
			m_dwTickcount = GetTickCount();
		}
		else
		{
			DWORD dwCure = GetTickCount() ;
			if( (dwCure- m_dwTickcount) > m_dlgCfg.m_PageCfg.m_dwReconnectGap)
			{		
				SendMessage(WM_COMMAND,ID_DISCONNECT_MSSIM,0);
				SendMessage(WM_COMMAND,ID_CONNECT_MSSIM,0);
				m_dwTickcount = 0;
			}
		}
	}


	CFrameWnd::OnTimer(nIDEvent);
}


LRESULT CMainFrame::OnCommSockData(WPARAM wParam,LPARAM lParam)
{
	m_CC.OnHiLvlData( (void*)wParam,lParam );

	return 0;
}

BOOL CMainFrame::OpenServer()
{
	if( NULL == m_pSvrChan )
	{
		if( !CreateChannel( &m_pSvrChan,CHANNEL_TYPE_SOCKET ) )
		{			
			return FALSE; 
		}		
		m_CC.m_lpHighChan = m_pSvrChan;
		m_pSvrChan->SetReceiver( WM_SCM_SENDDATA,TRUE,(void*)m_dwRcvThreadId );
	}
	else if( m_bServerActive ) 
	{
		m_pSvrChan->Close();
	}

   	CHANNEL_ATTRIBUTE ca;
	ca.ChannelType = CHANNEL_TYPE_SOCKET;
	ca.Socket.dwFlag = SC_TYPE_SERVER;
	m_dlgCfg.m_PageCfg.GetServerParam(ca.Socket.dwPort,ca.Socket.dwIP);
	m_bServerActive  = m_pSvrChan->Open( &ca );
	
	return m_bServerActive;
}

BOOL CMainFrame::CloseServer()
{
	if( m_pSvrChan )
	{
		m_pSvrChan->Close();
	}		
	m_bServerActive = FALSE;	
	return TRUE;
}

void CMainFrame::OnUpdateChansvrConfig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (!m_bInConfigure); //lint !e730
}

void CMainFrame::OnConnectMssim() 
{
	CString strLog;
	strLog.Format(_T("CMainFrame::OnConnectMssim +++"));
	Log(strLog);

	CWaitCursor wait;
	EnterCriticalSection( &m_csConnect );
	
	if(m_bInConnectPro && m_bInConnectLog)
	{
		LeaveCriticalSection( &m_csConnect );
		strLog.Format(_T("CMainFrame::OnConnectMssim ---"));
		Log(strLog);
		return;
	}

    //Open Channel Manager
   	CHANNEL_ATTRIBUTE caPro;   
	CHANNEL_ATTRIBUTE caLog;   
	caPro.ChannelType = CHANNEL_TYPE_COM;
	caLog.Com.dwPortNum = 0;
	caLog.Com.dwBaudRate = 0;
	BOOL bParamOK = FALSE;
	int  nFlowType = FLOW_TYPE_DIAG;	
	CString strFile;
	switch(m_dlgCfg.m_PageCfg.m_nChannelType )
	{       
	case WINSOCKET:
		{
			caPro.ChannelType = CHANNEL_TYPE_SOCKET;
			caPro.Socket.dwFlag = SC_TYPE_CLIENT;
			m_dlgCfg.m_PageCfg.GetSocketParam(caPro.Socket.dwPort,caPro.Socket.dwIP,nFlowType);
			if(caPro.Socket.dwPort!=0 && caPro.Socket.dwIP != 0 && caPro.Socket.dwIP != MAXDWORD)
				bParamOK = TRUE;
		}
		break;
	case UART:
		{
			caLog.ChannelType = CHANNEL_TYPE_COM;			
			m_dlgCfg.m_PageCfg.GetUartParam(caPro.Com.dwPortNum,caPro.Com.dwBaudRate,caLog.Com.dwPortNum,caLog.Com.dwBaudRate);
			if(caPro.Com.dwPortNum != 0 && caPro.Com.dwBaudRate != 0)
				bParamOK = TRUE;
		}
		break;
	case FILES:
		{
			caPro.ChannelType = CHANNEL_TYPE_FILE;
			caPro.File.dwPackSize = (DWORD)-1;
			caPro.File.dwPackFreq = (DWORD)-1;
			caPro.File.pFilePath =  NULL;				
			if( m_dlgCfg.m_PageCfg.GetFileParam(nFlowType,strFile) && !strFile.IsEmpty() )
			{
				caPro.File.pFilePath =  (LPTSTR)strFile.operator LPCTSTR();
			}
			bParamOK = TRUE;
			break;
		}
	default:
		LeaveCriticalSection( &m_csConnect );
		strLog.Format(_T("CMainFrame::OnConnectMssim ---"));
		Log(strLog);
		return;
	}
	
	
	m_pProChan->Close();	
	m_pLogChan->Close();

	m_CC.m_nEndian = m_nEndian;
	m_CC.m_nChannelType = caPro.ChannelType;
	//m_pProChan->SetProperty( 0,PPI_Endian,(void*)m_nEndian );

	// Open Channel
	if( bParamOK )
	{
		if( FLOW_TYPE_DIAG == nFlowType) // only open diag channel
		{
			if(!TryToOpen(m_pProChan,caPro))		
			{			
				LeaveCriticalSection( &m_csConnect );
				strLog.Format(_T("CMainFrame::OnConnectMssim ---"));
				Log(strLog);
				return;
			}
			else
			{
				m_bInConnectPro = TRUE;		
			}
		}
		else // only open smp channel
		{
			if(!TryToOpen(m_pLogChan,caPro))	
			{			
				LeaveCriticalSection( &m_csConnect );
				strLog.Format(_T("CMainFrame::OnConnectMssim ---"));
				Log(strLog);
				return;
			}
			else
			{
				m_bInConnectLog = TRUE;		
			}
		}
	}

	// For UART type, the DIAG channel is opened as above
	// so only open smp channel for UART type if satisfied port and baud not zero.
	if(m_dlgCfg.m_PageCfg.m_nChannelType == UART)
	{
		if(caLog.Com.dwPortNum != 0 && caLog.Com.dwBaudRate != 0)
		{
			if(!TryToOpen(m_pLogChan,caLog)) // Can not open port	
			{				
				LeaveCriticalSection( &m_csConnect );
				strLog.Format(_T("CMainFrame::OnConnectMssim ---"));
				Log(strLog);
				return;
			}
			else
			{
				m_bInConnectLog = TRUE;	
			}		
		}
	}

    //For ARM Assert
    m_bInAssert					= FALSE;
	m_CC.m_bAssert				= FALSE;
	m_CC.m_bGotOneRightPacket	= FALSE;     
    
	if( WINSOCKET == m_dlgCfg.m_PageCfg.m_nChannelType)
	{
		StopSocketMonitor();
	}
	ChangeIcon();

	LeaveCriticalSection( &m_csConnect );

	strLog.Format(_T("CMainFrame::OnConnectMssim ---"));
	Log(strLog);
}

void CMainFrame::OnUpdateConnectMssim(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (!m_bInConnectPro && !m_bInConnectLog); //lint !e730
}

void CMainFrame::OnDisconnectMssim() 
{
	CString strLog;
	strLog.Format(_T("CMainFrame::OnDisconnectMssim +++"));
	Log(strLog);

	EnterCriticalSection(&m_csDisconnect);
	if( m_pProChan && m_bInConnectPro)
	{
		m_pProChan->Close();
        m_bInConnectPro = FALSE;   
		ChangeIcon();

		if( FILES == m_dlgCfg.m_PageCfg.m_nChannelType )
		{				
			//m_dlgCfg.m_PageCfg.DeleteMergedFile();
		}
		//[[ hongliang.xin, try to re-connect to socket server
		else if( WINSOCKET == m_dlgCfg.m_PageCfg.m_nChannelType )
		{
			StartSocketMonitor();
		}
		//]]
	}

	if( m_pLogChan && m_bInConnectLog)
	{
		m_pLogChan->Close();
        m_bInConnectLog = FALSE;   
		ChangeIcon();
	}
	LeaveCriticalSection(&m_csDisconnect);

	strLog.Format(_T("CMainFrame::OnDisconnectMssim ---"));
	Log(strLog);
}

LRESULT CMainFrame::OnDisconnectMssim2(WPARAM,LPARAM)
{
	CString strLog;
	strLog.Format(_T("CMainFrame::OnDisconnectMssim2 +++"));
	Log(strLog);

	OnDisconnectMssim();

	strLog.Format(_T("CMainFrame::OnDisconnectMssim2 ---"));
	Log(strLog);

	return 0;
}

LRESULT  CMainFrame::OnFileReadOver(WPARAM,LPARAM)
{
	OnDisconnectMssim();
	return 0;
}

void CMainFrame::OnUpdateDisconnectMssim(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->Enable(m_bInConnectPro || m_bInConnectLog);
}

void CMainFrame::OnLogData() 
{
	// TODO: Add your command handler code here
	m_bEnableLogData = !m_bEnableLogData;
	if( m_bEnableLogData )
	{
		m_CC.StartLog();
	}
	else
	{
		m_CC.StopLog();
	}
}

void CMainFrame::OnUpdateLogData(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_bEnableLogData);
}

LRESULT CMainFrame::OnAssert( WPARAM wParam,LPARAM ) 
{
	m_bInAssert = wParam;
	ChangeIcon();	

	return 0;
}

void CMainFrame::OnUpdateAssert(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bInConnectLog || m_bInConnectPro);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CFrameWnd::WindowProc(message, wParam, lParam);
/*
    if( message == WM_DEVICECHANGE )
    {
        
        PDEV_BROADCAST_HDR pdbh = (PDEV_BROADCAST_HDR)lParam;
        
        switch(wParam)
        {
        case DBT_DEVICEREMOVECOMPLETE:
            if( pdbh != NULL && pdbh->dbch_devicetype == DBT_DEVTYP_PORT )
            {
                PDEV_BROADCAST_PORT pdbp = (PDEV_BROADCAST_PORT)lParam;
                _ASSERTE( _tcsstr(pdbp->dbcp_name, _T("COM")) != NULL );
                // dbcp_name is a pointer in fact
                UINT nPort = (UINT)_ttol( pdbp->dbcp_name + 3 );
                _ASSERTE( nPort > 0 && nPort < 256 );
				if( m_bInConfigure )
				{
					m_dlgCfg.m_PageCfg.GetUartInfo();
				}
                if( m_dlgCfg.m_PageCfg.m_uUartPort == nPort )
                {
                    OnDisconnectMssim();
                }                
            }
            break;
		case DBT_DEVICEARRIVAL:
            if( pdbh != NULL && pdbh->dbch_devicetype == DBT_DEVTYP_PORT )
            {
                PDEV_BROADCAST_PORT pdbp = (PDEV_BROADCAST_PORT)lParam;
                _ASSERTE( _tcsstr(pdbp->dbcp_name, _T("COM")) != NULL );
                // dbcp_name is a pointer in fact
                UINT nPort = (UINT)_ttol( pdbp->dbcp_name + 3 );
                _ASSERTE( nPort > 0 && nPort < 256 );
				if( m_bInConfigure )
				{
					m_dlgCfg.m_PageCfg.GetUartInfo();
				}

				if( m_dlgCfg.m_PageCfg.m_uUartPort == nPort )
				{
					// Wait for port initial complete
					Sleep( 2000 );						
					// Connect 
					OnConnectMssim();
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

void CMainFrame::ChangeIcon()
{
    TCHAR  szTip[MAX_PATH];

	if( m_bInConnectPro || m_bInConnectLog)
	{
		_stprintf(szTip,_T("Channel Server: Connected To MS"));
        _tcscpy(m_tndChannelServer.szTip ,szTip);
        m_tndChannelServer.hIcon = m_hIconActive;
        Shell_NotifyIcon(NIM_MODIFY,&m_tndChannelServer);
	}
	else
    {
        _stprintf(szTip,_T("Channel Server: Not Connect To MS"));
        _tcscpy(m_tndChannelServer.szTip ,szTip);
        m_tndChannelServer.hIcon = m_hIconInactive;
        Shell_NotifyIcon(NIM_MODIFY,&m_tndChannelServer);
		return;
    }

	if( m_bInAssert )
	{
		m_tndChannelServer.hIcon = m_hIconAssert;
		Shell_NotifyIcon(NIM_MODIFY,&m_tndChannelServer);
		return;
	}
}

LRESULT CMainFrame::OnEndianEvent( WPARAM wParam,LPARAM )
{
	m_nRealEndian = wParam;
	//m_pConvertor->SetEndian( (EndianType)m_nRealEndian );

	return 0;
}



LRESULT CMainFrame::OnRecvDeviceChange(WPARAM wParam,LPARAM lParam)
{
	DWORD dwFlag = (DWORD)lParam;
	DWORD dwPort = (DWORD)wParam;
	UINT nPort = dwPort;

	if(m_dlgCfg.m_PageCfg.m_nChannelType != UART)
		return 0;

	CString strLog;
	strLog.Format(_T("CMainFrame::OnRecvDeviceChange +++"));
	Log(strLog);


	if(dwFlag == 0) //device out
	{
		strLog.Format(_T("CMainFrame::OnRecvDeviceChange: device [%d] out"),dwPort);
		Log(strLog);

		if( m_bInConfigure )
		{
			m_dlgCfg.m_PageCfg.GetUartInfo();
		}
		if( m_dlgCfg.m_PageCfg.IsDiagPort(dwPort) || m_dlgCfg.m_PageCfg.IsLogPort(dwPort)  )
		{
			

			OnDisconnectMssim();
			OnNotify2Client(FALSE);
        }  		
	}
	else if(dwFlag == 1) // device in
	{	
		strLog.Format(_T("CMainFrame::OnRecvDeviceChange: device [%d] in"),dwPort);
		Log(strLog);

		if(m_bInConnectLog && m_bInConnectPro)
		{
			strLog.Format(_T("CMainFrame::OnRecvDeviceChange ---, already connected"));
			Log(strLog);
			return 0;
		}

		if( m_bInConfigure )
		{
			m_dlgCfg.m_PageCfg.GetUartInfo();
		}

		TCHAR szName[DEV_NAME_MAX_LEN] = {0};
		//m_devHound.GetPortName(nPort,szName);
		m_pDevHound->GetPortName(nPort,szName);
		
		if( !m_bInConnectPro && m_dlgCfg.m_PageCfg.IsRightPort(nPort,szName,TRUE))
		{//diag port is not connected, so check if this port is diag port
			// Wait for port initial complete
			strLog.Format(_T("CMainFrame::OnRecvDeviceChange: device [%s:%d] in"),szName,dwPort);
			Log(strLog);

			Sleep( 2000 );			
			// Connect 
			OnConnectMssim();
            OnNotify2Client(TRUE);
		}
		else if(!m_bInConnectLog && m_dlgCfg.m_PageCfg.IsRightPort(nPort,szName,FALSE))
		{//log port is not connected, so check if this port is log port
			// Wait for port initial complete
			strLog.Format(_T("CMainFrame::OnRecvDeviceChange: device [%s:%d] in"),szName,dwPort);
			Log(strLog);

			Sleep( 2000 );			
			// Connect 
			OnConnectMssim();
			OnNotify2Client( m_bInConnectPro || m_bInConnectLog);
		}
	}

	strLog.Format(_T("CMainFrame::OnRecvDeviceChange ---"));
	Log(strLog);

	return 0;
}

LRESULT CMainFrame::OnPortReset(WPARAM wParam,LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	UNUSED_ALWAYS(lParam);

	CString strLog;
	strLog.Format(_T("CMainFrame::OnPortReset +++"));
	Log(strLog);

	if(m_dlgCfg.m_PageCfg.IsNeedPortReset())
	{
		strLog.Format(_T("CMainFrame::OnPortReset: reset port"));

		OnDisconnectMssim();
		Sleep(m_dlgCfg.m_PageCfg.GetEnumPortDelay());
		OnConnectMssim();
	}	

	strLog.Format(_T("CMainFrame::OnPortReset ---"));
	Log(strLog);

	return 0;
}

DWORD WINAPI CMainFrame::GetThreadFunc(LPVOID lpParam)
{
	CMainFrame * pThis = (CMainFrame *)lpParam;
	return pThis->SocketMonitorFunc();
}

DWORD CMainFrame::SocketMonitorFunc()
{
	while(!m_bInConnectPro && m_bSocketMonitor)
	{
		OnConnectMssim();
		if(!m_bInConnectPro && m_bSocketMonitor)
		{
			Sleep(3000);
		}
	}
	return 0;
}

void CMainFrame::StartSocketMonitor()
{
	CString strLog;
	strLog.Format(_T("CMainFrame::StartSocketMonitor +++"));
	Log(strLog);


	if(m_bSocketMonitor)
	{
		return;
	}
	m_bSocketMonitor = TRUE;
	DWORD dwThreadID = 0;
	m_hSocketMonitor = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)GetThreadFunc,
		(LPVOID)this,
		NULL,
        &dwThreadID);

	if(m_hSocketMonitor == NULL)
	{
		m_bSocketMonitor = FALSE;
	}	

	strLog.Format(_T("CMainFrame::StartSocketMonitor ---"));
	Log(strLog);
}

void CMainFrame::StopSocketMonitor()
{
	CString strLog;
	strLog.Format(_T("CMainFrame::StopSocketMonitor +++"));
	Log(strLog);

	m_bSocketMonitor = FALSE;
	if(m_hSocketMonitor != NULL)
	{
		CloseHandle( m_hSocketMonitor);
		m_hSocketMonitor = NULL;
	}

	strLog.Format(_T("CMainFrame::StopSocketMonitor ---"));
	Log(strLog);
}

BOOL CMainFrame::TryToOpen(IProtocolChannel *pChan, CHANNEL_ATTRIBUTE &ca)
{
	CString strLog;
	strLog.Format(_T("CMainFrame::TryToOpen +++"));
	Log(strLog);

	BOOL bConnect = FALSE;
	int nTryNum = 3;
	do 
	{
		bConnect = pChan->Open( &ca );
		if(bConnect)
		{
			break;
		}
		pChan->Close();
		Sleep(800);
	} while( (--nTryNum) > 0 );

	strLog.Format(_T("CMainFrame::TryToOpen ---, return %d"),bConnect);
	Log(strLog);

	return bConnect;
}

void CMainFrame::OnNotify2Client(BOOL bConnect)
{
	CString strLog;
	strLog.Format(_T("CMainFrame::OnNotify2Client +++"));
	Log(strLog);

    m_CC.TellClientsConnectState(bConnect);

	strLog.Format(_T("CMainFrame::OnNotify2Client ---"));
	Log(strLog);
}


void CMainFrame::Log(CString &str)
{
	if(m_dlgCfg.m_PageCfg.m_bUILogEnable)
	{
		m_spLog.LogRawStr(SPLOGLV_INFO,str);
		m_spLog.FlushBuff();
	}
}