// DialUpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DialUpDlg.h"

#include <process.h>
#include "RASERROR.H"
#include "MainFrm.h"
#include "MSInfoView.h"
#include "MobileCnntDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDialUpDlg *g_pdial_up;


void WINAPI RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);

const UINT WM_RASEVENT = ::RegisterWindowMessageA(RASDIALEVENT);

/////////////////////////////////////////////////////////////////////////////
// CDialUpDlg dialog


CDialUpDlg::CDialUpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDialUpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialUpDlg)
		// NOTE: the ClassWizard will add member initialization here

	//}}AFX_DATA_INIT

	m_dwEntryInfoSize = 0;
    m_dwDeviceInfoSize= 0;
    m_lpRasEntry = NULL;
    m_lpDeviceInfo= NULL; 
	m_dwDialNum = 5;
	m_dwActualTime = 1;
	m_bKillTimer = FALSE;
	m_dwSpd = 0;
	m_nDialTime = 0;
	m_lastState = RASCS_Disconnected;
	m_bExistCnnt = FALSE;
}


void CDialUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialUpDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDCANCEL, m_btnCsl);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CDialUpDlg, CDialog)
BEGIN_MESSAGE_MAP(CDialUpDlg, CDialog)
	//{{AFX_MSG_MAP(CDialUpDlg)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_DIAL_BEGIAN, OnDialBegain)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_REGISTERED_MESSAGE(WM_RASEVENT, OnRasEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void WINAPI RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError)
{
	UNREFERENCED_PARAMETER(unMsg);

	if(g_pdial_up != NULL)
	{
		g_pdial_up->ProcessDialEvent(rasconnstate);
		
		if(dwError)
		{
			g_pdial_up->GetErrorString(dwError);
			if(g_pdial_up != NULL)
			{
				g_pdial_up->SetStateText(IDS_DIALUP_FAILED);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDialUpDlg message handlers

void CDialUpDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
//	if(!m_bExistCnnt)
//	{
//		SetStateText(IDS_DIAL_DISCONNECTING);
//		Release();
//	}
//
//	((CMainFrame*)(theApp.m_pMainWnd))->m_bDialCnnt = FALSE;
//	::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_DISCONNECT_DIAL_UP, NULL, NULL);

//	CDialog::OnCancel();
	OnClose();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void CDialUpDlg::Release()
{
	if(g_pdial_up != NULL)
	{
		g_pdial_up = NULL;
	}
	
	theApp.HangUp();
	
	m_dwEntryInfoSize = 0;
    m_dwDeviceInfoSize= 0;
    m_lpRasEntry = NULL;
    m_lpDeviceInfo= NULL;
	
}

BOOL CDialUpDlg::Dial_up()
{
	CString strPhoneBook;
	strPhoneBook.LoadString(IDS_PHONEBOOK_ENTRYNAME);
	theApp.m_hRasConn = NULL;
	RASDIALPARAMS rdParams;
	ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));
	rdParams.dwSize = sizeof(RASDIALPARAMS);
	lstrcpy(rdParams.szEntryName, strPhoneBook);
	
	lstrcpy(rdParams.szPhoneNumber, _T("P*99#"));
	lstrcpy(rdParams.szUserName, _T(""));
	lstrcpy(rdParams.szPassword, _T(""));
	rdParams.szCallbackNumber[0] = '*';
	rdParams.szDomain[0] = '*';

	DWORD dwRet;

//	dwRet = RasDial(NULL, NULL, &rdParams, 0L, (LPVOID)RasDialFunc, &theApp.m_hRasConn);
	dwRet = RasDial(NULL, NULL, &rdParams, 0xFFFFFFFF, this->GetSafeHwnd(), &theApp.m_hRasConn);
	if(dwRet)
	{
		GetErrorString(dwRet);
		OnClose();
		//SetStateText(IDS_DIALUP_FAILED);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CDialUpDlg::InitConnect(LPCTSTR lpszModemName)
{
	m_dwEntryInfoSize = 0;
    m_dwDeviceInfoSize= 0;
    m_lpRasEntry = NULL;
    m_lpDeviceInfo= NULL;

	DWORD dwRet;
    dwRet  = RasGetEntryProperties(NULL,_T(""),NULL,
        &m_dwEntryInfoSize,NULL,
        &m_dwDeviceInfoSize);
    if(dwRet != 0)
	{
		if( dwRet != ERROR_BUFFER_TOO_SMALL )
			return FALSE;
	}
    
    m_lpRasEntry = (LPRASENTRY) GlobalAlloc(GPTR, m_dwEntryInfoSize);
    if( m_dwDeviceInfoSize == 0)
        m_lpDeviceInfo = NULL;
    else
        m_lpDeviceInfo = (LPBYTE)GlobalAlloc( GPTR, m_dwDeviceInfoSize );
    
    m_lpRasEntry->dwSize = sizeof( RASENTRY );
    
    
    CString strEntryName;
    strEntryName.LoadString( IDS_PHONEBOOK_ENTRYNAME );
    
    dwRet = RasValidateEntryName(NULL,strEntryName );
    if( dwRet == ERROR_SUCCESS )
    {
		dwRet  = RasGetEntryProperties(NULL,_T(""),m_lpRasEntry,
			&m_dwEntryInfoSize,m_lpDeviceInfo,
			&m_dwDeviceInfoSize);
		if(dwRet != 0)
			return FALSE;
		
        _tcscpy(m_lpRasEntry->szLocalPhoneNumber ,_T("*99#"));
        _tcscpy( m_lpRasEntry->szDeviceType, _T("RASDT_Modem"));
		_tcscpy( m_lpRasEntry->szDeviceName, lpszModemName);

		//ADD MORE        
        
        dwRet  = RasSetEntryProperties(NULL,strEntryName,m_lpRasEntry,
            m_dwEntryInfoSize,m_lpDeviceInfo,
            m_dwDeviceInfoSize);
        if(dwRet != 0)
            return FALSE;    
    }
    else
    {
        if( dwRet != ERROR_ALREADY_EXISTS )
            return FALSE;
		
		dwRet  = RasGetEntryProperties(NULL,strEntryName,m_lpRasEntry,
			&m_dwEntryInfoSize,m_lpDeviceInfo,
			&m_dwDeviceInfoSize);
		if(dwRet != 0)
			return FALSE;

		if(_tcscmp(m_lpRasEntry->szDeviceName, lpszModemName) != 0)
		{
			//if is exist, create it too.
			_tcscpy(m_lpRasEntry->szLocalPhoneNumber ,_T("*99#"));
			_tcscpy( m_lpRasEntry->szDeviceType, _T("RASDT_Modem"));
			_tcscpy( m_lpRasEntry->szDeviceName, lpszModemName);
			
			//ADD MORE        
			
			dwRet  = RasSetEntryProperties(NULL,strEntryName,m_lpRasEntry,
				m_dwEntryInfoSize,m_lpDeviceInfo,
				m_dwDeviceInfoSize);
			if(dwRet != 0)
				return FALSE; 
		}  
    }

	return TRUE;
}

void CDialUpDlg::GetErrorString(DWORD dwError)
{
	TCHAR szMessage[256];

	RasGetErrorString( dwError, szMessage, 256 ); 
		
	AfxMessageBox(szMessage);
}

void CDialUpDlg::SetStateText(UINT uID)
{
	CString strText;
	VERIFY(strText.LoadString(uID));
	if(!::IsWindow(this->m_hWnd))
	{
		return;
	}

	CStatic *pStc = ((CStatic *)GetDlgItem(IDC_STATIC_DIAL_UP));

	if(pStc == NULL)
	{
		return;
	}
 
	if(uID == IDS_DIALUP_FAILED)
	{
		if(m_dwActualTime <= m_dwDialNum)
		{
			CString strTmp;
			strTmp.Format(_T("\r\n%d"), m_nDialTime);
			CString strAgain;
			strAgain.LoadString(IDS_DIALUP_AGAIN);
			strText += strTmp + strAgain;
		}
		pStc->SetWindowText(strText); 

		if(m_dwActualTime <= m_dwDialNum)
		{
			Sleep(m_nDialTime * 1000);
			theApp.HangUp();
	
			SetStateText(IDS_DIAL_START);
			m_dwActualTime ++;
			
			Dial_up();
		}
		
	}
	else
	{
		pStc->SetWindowText(strText); 
	}
}

void CDialUpDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
//	Release();
	
}


void CDialUpDlg::SetModemName(CString strModemName)
{
	m_strModemName = strModemName;
}

void CDialUpDlg::SetConnSpeed(DWORD dwSpd)
{
	m_dwSpd = dwSpd;
}

BOOL CDialUpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	VERIFY( m_br.CreateSolidBrush(RGB(232,232,232)) );

	g_pdial_up = this;

	InitConnect(m_strModemName);

    m_nDialTime = GetPrivateProfileInt( _T("Net"), _T("DialTime"), 0, theApp.m_szCfgPathName );
	m_dwDialNum = GetPrivateProfileInt( _T("Net"), _T("DialNum"), 0, theApp.m_szCfgPathName );

	SetStateText(IDS_DIAL_START); 
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialUpDlg::ProcessDialEvent(RASCONNSTATE rasconnstate)
{
	switch( rasconnstate  )
    {
    case RASCS_OpenPort:
		SetStateText(IDS_DIAL_OPEN_PORT);
        break;
    case RASCS_PortOpened: 
		SetStateText(IDS_DIAL_PORT_OPENED);
        break;
    case RASCS_ConnectDevice:
		SetStateText(IDS_DIAL_CONNECT_DEVICE);
        break;
    case RASCS_DeviceConnected:
		SetStateText(IDS_DIAL_DEVICE_CONNECTED);
        break;
    case RASCS_Authenticate:
		SetStateText(IDS_DIAL_AUTHENTICATE);
        break;
    case RASCS_Authenticated:
		m_lastState = RASCS_Authenticated;
		SetStateText(IDS_DIAL_AUTHENTICATED);
        break;
    case RASCS_Connected:
		{
			if(m_lastState != RASCS_Authenticated)
			{
				SetStateText(IDS_CONNECT_EXIST);
				m_bExistCnnt = TRUE;
				break;
			}

			m_bExistCnnt = FALSE;

			((CMainFrame*)(theApp.m_pMainWnd))->m_bDialCnnt = TRUE;
			::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_NOTICE_SET_TIMER, NULL, NULL);
			
			SetStateText(IDS_DIAL_CONNECTED);
		//	Release();
		//	this->SendMessage(WM_CLOSE, NULL, NULL);	
		//	this->ShowWindow(SW_HIDE);
			::PostMessage(this->GetSafeHwnd(), WM_SHOWWINDOW, FALSE, SW_OTHERZOOM);
			SetTimer(EVENT_CHECK_CONNECTED, 200, NULL);
			m_bKillTimer = TRUE;
			break;
		}
    case RASCS_Disconnected:
		this->SendMessage(WM_CLOSE, NULL, NULL);

        break;
    default:
        break;
    }
}

HBRUSH CDialUpDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_br.m_hObject);
//	return hbr;


}

void CDialUpDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	if(!m_bExistCnnt)
	{
		if(m_bKillTimer)
		{
			m_bKillTimer = FALSE;
			KillTimer(EVENT_CHECK_CONNECTED);
		}
		
		SetStateText(IDS_DIAL_DISCONNECTING);
		
		Release();
	}

	((CMainFrame*)(theApp.m_pMainWnd))->m_bDialCnnt = FALSE;
		
	::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_DISCONNECT_DIAL_UP, NULL, NULL);
		
	CDialog::OnClose();
}

LRESULT CDialUpDlg::OnDialBegain(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	Dial_up();
	m_dwActualTime ++;
	
	return 0;
}

void CDialUpDlg::EnumDevices()
{
	LPRASDEVINFO lpdevInfo;
	
	lpdevInfo = new RASDEVINFO[10];
	lpdevInfo->dwSize = sizeof(RASDEVINFO);
	DWORD dwPcb = 10 * sizeof(RASDEVINFO);
	DWORD dwpcDevice = 0;
	DWORD dwRet = RasEnumDevices(lpdevInfo, &dwPcb, &dwpcDevice);

	for(UINT i = 0; i < dwpcDevice; i++)
	{
		lpdevInfo ++;
	}
}

BOOL CDialUpDlg::CheckingConnected()
{
	RASCONNSTATUS rStatus;
	DWORD dwRet;
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
	rStatus.dwSize = sizeof(RASCONNSTATUS);
	dwRet = RasGetConnectStatus(theApp.m_hRasConn, &rStatus);
	RASCONNSTATE rcs = rStatus.rasconnstate;
	if (ERROR_INVALID_HANDLE == dwRet)
	{
		if(theApp.m_pMainWnd != NULL)
		{
			((CMainFrame*)(theApp.m_pMainWnd))->m_bDialCnnt = FALSE;
		}
		this->SendMessage(WM_CLOSE, NULL, NULL);
		//HangUp(FALSE);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void CDialUpDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent == EVENT_CHECK_CONNECTED)
	{
		if(CheckingConnected())
		{
			if(m_bKillTimer)
			{
				m_bKillTimer = FALSE;
				KillTimer(nIDEvent);
			}
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

LRESULT CDialUpDlg::OnRasEvent(WPARAM wParam, LPARAM lParam)
{
	RASCONNSTATE rasconnstate = (RASCONNSTATE)wParam;
	ProcessDialEvent(rasconnstate);


	DWORD dwError = (DWORD)lParam;
	if(dwError)
	{
		GetErrorString(dwError);

		SetStateText(IDS_DIALUP_FAILED);
	}

	return 1;
}