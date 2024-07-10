// ConfigureSheet.cpp : implementation file
//

#include "stdafx.h"
#include "Function.h"
#include "ChannelServer.h"
#include "ConfigureSheet.h"
#include "MainFrm.h"
#include <Winsock2.h>
#include <ATLCONV.H>

#define ARRAY_SIZE( a )     ( sizeof (a) / sizeof (a)[0] )

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigureSheet property page

IMPLEMENT_DYNCREATE(CConfigureSheet, CPropertyPage)

CConfigureSheet::CConfigureSheet() : CPropertyPage(CConfigureSheet::IDD)
{
	//{{AFX_DATA_INIT(CConfigureSheet)
	m_nTmpServerPort = 36666;
	m_nTmpTimeouts = 3;
	//}}AFX_DATA_INIT
	m_bAutoTest = FALSE;
	m_nTimeouts = m_nTmpTimeouts;
	m_nServerPort = m_nTmpServerPort;
	m_nServerIP   = 0x7F000001;
	m_bEnableUartReconnect = FALSE;
	m_dwReconnectGap = 5000;
	m_nUpDataSpeedAlert = -1;
	m_bUILogEnable = FALSE;
    GetConfiguration();
}

CConfigureSheet::~CConfigureSheet()
{
}

void CConfigureSheet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureSheet)
	DDX_Control(pDX, IDC_MSSIM_TYPE, m_cmbChannelType);
	DDX_Text(pDX, IDC_CHANSVR_PORT, m_nTmpServerPort);
	DDX_Text(pDX, IDC_MSG_TIMEOUT, m_nTmpTimeouts);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigureSheet, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigureSheet)
	ON_CBN_SELCHANGE(IDC_MSSIM_TYPE, OnSelchangeChannelType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureSheet message handlers


BOOL CConfigureSheet::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CreateCtrls(); 
	m_cmbChannelType.SetCurSel( m_nChannelType );

    switch ( m_nChannelType ) {
    case WINSOCKET:
		m_dlgSocketConfig.ShowWindow( SW_SHOW );
		m_dlgUartConfig.ShowWindow( SW_HIDE );
		m_dlgFileConfig.ShowWindow( SW_HIDE );
        break;
    case UART:
		m_dlgSocketConfig.ShowWindow( SW_HIDE );
		m_dlgUartConfig.ShowWindow( SW_SHOW );
		m_dlgFileConfig.ShowWindow( SW_HIDE );
		break;
	case FILES:
		m_dlgSocketConfig.ShowWindow( SW_HIDE );
		m_dlgUartConfig.ShowWindow( SW_HIDE );
		m_dlgFileConfig.ShowWindow( SW_SHOW );
		break;
    default:
		m_dlgSocketConfig.ShowWindow( SW_HIDE );
		m_dlgUartConfig.ShowWindow( SW_HIDE );
		m_dlgFileConfig.ShowWindow( SW_HIDE );   
        break;
    }	
	
	GetUartInfo();

	m_nTmpTimeouts = m_nTimeouts;
	m_nTmpServerPort = m_nServerPort;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#define CONFIG_FILE		_T("\\ChannelServer.ini")

void CConfigureSheet::GetConfiguration()
{
	USES_CONVERSION;
	TCHAR szCfgFile[ MAX_PATH ] = {0};
	CString strSec;
	
	GetAppPath( szCfgFile );
	_tcscat( szCfgFile, CONFIG_FILE );
    
	TCHAR szTemp[MAX_PATH] = {0};
	strSec = _T("Server");
	m_nServerPort = GetPrivateProfileInt( strSec, _T("Port"), 36666, szCfgFile );
	m_nTmpServerPort = m_nServerPort;
	GetPrivateProfileString( strSec, _T("IP"), _T("127.0.0.1"), szTemp, MAX_PATH, szCfgFile );
	m_nServerIP = inet_addr(T2A(szTemp));
	m_nServerIP = htonl(m_nServerIP);
	
	strSec = "Channel";
	m_nChannelType = GetPrivateProfileInt( strSec, _T("ChannelType"), 1, szCfgFile );
	
	strSec = _T( "Test" );
	m_bAutoTest = GetPrivateProfileInt(strSec,_T("Autotest"),0,szCfgFile );
	
	strSec = _T( "MessageTimeout" );
	m_nTimeouts = GetPrivateProfileInt(strSec,_T("Timeout"),3,szCfgFile );
	m_nTmpTimeouts = m_nTimeouts;

	m_bEnableUartReconnect = GetPrivateProfileInt(_T("Settings"),_T("EnableUartReconnect"),0,szCfgFile );
	m_dwReconnectGap = GetPrivateProfileInt(_T("Settings"),_T("ReconnectInterval "),5000,szCfgFile );
	m_bUILogEnable = GetPrivateProfileInt(_T("Settings"),_T("UILogEnable"),0,szCfgFile );

	m_nUpDataSpeedAlert = -1;
	memset(szTemp,0,sizeof(szTemp));
	GetPrivateProfileString( _T("Settings"), _T("UpDataSpeedAlert"), _T(""), szTemp, MAX_PATH, szCfgFile );
	if(_tcslen(szTemp) != 0)
	{
		CString strTmp = szTemp;
		strTmp.MakeLower();
		int nFind = -1;
		int nData = 0;
		if( (nFind = strTmp.Find(_T("mbps"))) != -1)
		{
			strTmp = strTmp.Left(nFind);
			strTmp.TrimRight();
			if(!strTmp.IsEmpty())
			{
				_stscanf(strTmp,_T("%d"),&nData);
				if(nData > 0)
				{
					m_nUpDataSpeedAlert = nData * 1024*1024;
				}
			}
		}
		else if((nFind = strTmp.Find(_T("kbps"))) != -1)
		{
			strTmp = strTmp.Left(nFind);
			strTmp.TrimRight();
			if(!strTmp.IsEmpty())
			{
				_stscanf(strTmp,_T("%d"),&nData);
				if(nData > 0)
				{
					m_nUpDataSpeedAlert = nData * 1024;
				}
			}
		}
		else if((nFind = strTmp.Find(_T("bps")))!= -1)
		{
			strTmp = strTmp.Left(nFind);
			strTmp.TrimRight();
			if(!strTmp.IsEmpty())
			{
				_stscanf(strTmp,_T("%d"),&nData);
				if(nData > 0)
				{
					m_nUpDataSpeedAlert = nData;
				}
			}
		}

	}


	
	m_dlgUartConfig.LoadConfig(szCfgFile);
	m_dlgSocketConfig.LoadConfig(szCfgFile);
	m_dlgFileConfig.LoadConfig(szCfgFile);
}	              

BOOL CConfigureSheet::SaveConfiguration()
{
    TCHAR szCfgFile[ MAX_PATH ];

	CString strSec;
	CString strTmp;
    
	GetAppPath( szCfgFile );
    _tcscat( szCfgFile, CONFIG_FILE );

	CFileFind finder;
	if(finder.FindFile(szCfgFile))
	{
		DWORD dwAtt = GetFileAttributes(szCfgFile);
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		SetFileAttributes(szCfgFile,dwAtt);
	}
     
    strSec = _T("Server");
	strTmp.Format(_T("%d"),m_nServerPort);	
	WritePrivateProfileString( strSec, _T("Port"), strTmp, szCfgFile ) ;
    //WritePrivateProfileString( strSec, _T("IP"), m_strServerAddress, szCfgFile );

	strSec = "Channel";
	strTmp.Format(_T("%d"),m_nChannelType);	
    WritePrivateProfileString( strSec, _T("ChannelType"), strTmp, szCfgFile );   

	m_dlgUartConfig.SaveConfig(szCfgFile);
	m_dlgSocketConfig.SaveConfig(szCfgFile);
	m_dlgFileConfig.SaveConfig(szCfgFile);
 
	return TRUE;
}

void CConfigureSheet::OnOK() 
{	// TODO: Add your specialized code here and/or call the base class	
    if ( !UpdateData() )
        return;

	m_nChannelType = m_cmbChannelType.GetCurSel();
	m_nServerPort = m_nTmpServerPort;
	m_nTimeouts = m_nTmpTimeouts;

	switch ( m_nChannelType )
	{
	case WINSOCKET:
		m_dlgSocketConfig.UpdateConfig();
		break;
	case UART:
		m_dlgUartConfig.UpdateConfig();
		break;
	case FILES:
		m_dlgFileConfig.UpdateConfig();
		break;
	default:
		break;
	}

	SaveConfiguration();

	CPropertyPage::OnOK();
}

void CConfigureSheet::OnSelchangeChannelType() 
{
	// TODO: Add your control notification handler code here  
    CString strTemp;
	int nType = m_cmbChannelType.GetCurSel();
	switch ( nType )
	{
	case WINSOCKET:
		m_dlgSocketConfig.ShowWindow( SW_SHOW );
		m_dlgUartConfig.ShowWindow( SW_HIDE );
		m_dlgFileConfig.ShowWindow( SW_HIDE );
		break;

	case UART:
		m_dlgSocketConfig.ShowWindow( SW_HIDE );
		m_dlgUartConfig.ShowWindow( SW_SHOW );
		m_dlgFileConfig.ShowWindow( SW_HIDE );		
		break;
	case FILES:
		m_dlgSocketConfig.ShowWindow( SW_HIDE );
		m_dlgUartConfig.ShowWindow( SW_HIDE );
		m_dlgFileConfig.ShowWindow( SW_SHOW );
		break;
    default:
        break;
    }
}


BOOL CConfigureSheet::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
    if ( !CheckData() )
        return FALSE;

	return CPropertyPage::OnKillActive();
}

BOOL CConfigureSheet::CheckServerPort()
{
	UpdateData();
	if( m_nTmpServerPort >= 1024  && m_nTmpServerPort <= 65536 )
		return TRUE;
	m_nTmpServerPort = m_nServerPort;
	AfxMessageBox( _T("Input Error: [Server port]"), MB_OK | MB_ICONERROR );
    UpdateData(FALSE);
    return FALSE;
}

BOOL CConfigureSheet::CheckTimeout()
{
	UpdateData();
	if( m_nTmpTimeouts >= 1  && m_nTmpTimeouts <= 60 )
		return TRUE;
	m_nTmpTimeouts = m_nTimeouts;
	AfxMessageBox( _T("Input Error: [Timeouts]"), MB_OK | MB_ICONERROR );
    UpdateData(FALSE);
    return FALSE;
}


BOOL CConfigureSheet::CheckData()
{
    // TODO: Add your specialized code here and/or call the base class   
    if ( !CheckServerPort() )
        return FALSE;

    switch ( m_cmbChannelType.GetCurSel() ) {
    case WINSOCKET:
        if ( !m_dlgSocketConfig.CheckConfig() )
            return FALSE;
        break;

    case UART:
        if ( !m_dlgUartConfig.CheckConfig() )
            return FALSE;
        break;
	case FILES:
		if ( !m_dlgFileConfig.CheckConfig() )
            return FALSE;
        break;
		break;	
    default:
        break;
    }

    if ( !CheckTimeout() )
        return FALSE;

    return TRUE;
}

void CConfigureSheet::GetUartParam(DWORD &nDiagPort,DWORD &nDiagBaudRate,DWORD &nLogPort,DWORD &nLogBaudRate)
{
	nDiagPort = m_dlgUartConfig.m_nDiagPort;
	nDiagBaudRate = m_dlgUartConfig.m_nDiagBaudrate;

	nLogPort = m_dlgUartConfig.m_nLogPort;
	nLogBaudRate = m_dlgUartConfig.m_nLogBaudrate;
}

BOOL CConfigureSheet::IsNeedPortReset()
{
	if(m_nChannelType == UART && m_dlgUartConfig.m_bPortReset)
		return TRUE;

	return FALSE;
}

void CConfigureSheet::CreateCtrls()
{
	CWnd *pWnd = GetDlgItem(IDC_STC_CONFIG);
	CRect rect;
	pWnd->GetClientRect(&rect);
	//ClientToScreen(&rect);
	
	m_dlgUartConfig.Create(IDD_UART_CONFIG,NULL);
	m_dlgUartConfig.SetParent(pWnd); // this is the key step, must be set after change style
	m_dlgUartConfig.SetWindowPos(&wndTop,0,0, rect.Width(),rect.Height(),SWP_FRAMECHANGED );
	m_dlgUartConfig.ShowWindow(SW_HIDE);

	m_dlgSocketConfig.Create(IDD_SOCKET_CONFIG,NULL);
	m_dlgSocketConfig.SetParent(pWnd);
	m_dlgSocketConfig.SetWindowPos(&wndTop,0,0, rect.Width(),rect.Height(),SWP_FRAMECHANGED);
	m_dlgSocketConfig.ShowWindow(SW_HIDE);

	m_dlgFileConfig.Create(IDD_FILE_CONFIG,NULL);
	m_dlgFileConfig.SetParent(pWnd);	
	m_dlgFileConfig.SetWindowPos(&wndTop,0,0, rect.Width(),rect.Height(),SWP_FRAMECHANGED);
	m_dlgFileConfig.ShowWindow(SW_HIDE);	
}

void CConfigureSheet::GetUartInfo()
{
	m_dlgUartConfig.GetUartInfo();
}

BOOL CConfigureSheet::IsRightPort(DWORD nPort,LPCTSTR lpszPortName,BOOL bDiag)
{
	return m_dlgUartConfig.IsRightPort(nPort,lpszPortName,bDiag);
}

void CConfigureSheet::GetServerParam(DWORD &nPort,DWORD &nIP)
{
	nPort = m_nServerPort;
	nIP   = m_nServerIP;
}
void CConfigureSheet::GetSocketParam(DWORD &nPort,DWORD &nIP,int &nFlowType)
{
	nPort = m_dlgSocketConfig.m_nPort;
	nIP   = m_dlgSocketConfig.m_nIP;
	nFlowType = m_dlgSocketConfig.m_nFlowType;
}

BOOL CConfigureSheet::GetFileParam(int &nFlowType,CString &strFile)
{
	//nFlowType = m_dlgFileConfig.m_nFlowType;	
	//BOOL bOK  = m_dlgFileConfig.MergeChannelFiles();
	//if(bOK) strFile = m_dlgFileConfig.m_strMergedFile;
	strFile = m_dlgFileConfig.m_strFiles;
	nFlowType = m_dlgFileConfig.GetFlowType();
	return TRUE;
}

void CConfigureSheet::DeleteMergedFile()
{
	m_dlgFileConfig.DeleteMergedFile();
}


BOOL CConfigureSheet::IsDiagPort(DWORD nPort)
{	
	return (m_dlgUartConfig.m_nDiagPort == nPort);

}
BOOL CConfigureSheet::IsLogPort(DWORD nPort)
{
	return (m_dlgUartConfig.m_nLogPort == nPort);
}

DWORD CConfigureSheet::GetEnumPortDelay()
{
	return m_dlgUartConfig.m_nEnumPortDelay;
}

int CConfigureSheet::GetEndian()
{
	switch(m_nChannelType)
	{		
	case WINSOCKET:
		return m_dlgSocketConfig.m_nEndian;
		break;
    case UART:
		return  m_dlgUartConfig.m_nEndian;	
		break;
	case FILES:
		return  m_dlgFileConfig.m_nEndian;
		break;
    default:
		return ENDIAN_LITTLE;
	}
}


