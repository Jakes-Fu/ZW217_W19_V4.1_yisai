// DialUpView.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DialUpView.h"
#include "MainFrm.h"
#include "MobileCnntDoc.h"
#include <process.h>
#include "RASERROR.H"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT WM_RASEVENT = ::RegisterWindowMessageA(RASDIALEVENT);

/////////////////////////////////////////////////////////////////////////////
// CDialUpView

IMPLEMENT_DYNCREATE(CDialUpView, CFormView)

CDialUpView::CDialUpView(): 
CCoolFormView(CDialUpView::IDD)
{
	//{{AFX_DATA_INIT(CDialUpView)
	//}}AFX_DATA_INIT
	m_bHandCnnt = FALSE;
	m_bSpdTm = FALSE;
	m_fConnSpeed = 0.0;
	m_upSpeed = 0;
	m_downSpeed = 0;
	m_uRcvData = 0;
	m_uSendData = 0;
	m_strConnTime = _T("0:00:00:00");
	m_strConnSpeed = _T("0");

	m_dwEntryInfoSize = 0;
    m_dwDeviceInfoSize= 0;
    m_lpRasEntry = NULL;
    m_lpDeviceInfo= NULL; 
	m_dwDialNum = 5;
	m_dwActualTime = 1;
	m_bKillTimer = FALSE;
	m_nDialTime = 0;
	m_lastState = RASCS_Disconnected;
}

CDialUpView::~CDialUpView()
{
	m_lpRasEntry = NULL;
    m_lpDeviceInfo= NULL;
}

void CDialUpView::DoDataExchange(CDataExchange* pDX)
{
	CCoolFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialUpView)
	DDX_Control(pDX, IDC_FDU_CMB_SIM, m_cmbSim);
	DDX_Control(pDX, IDC_BTN_HAND_DIAL, m_btnHandDial);
	DDX_Text(pDX, IDC_STC_RCV_DATA, m_uRcvData);
	DDX_Text(pDX, IDC_STC_SEND_DATA, m_uSendData);
	DDX_Text(pDX, IDC_STC_CONN_SPEED, m_strConnSpeed);
	DDX_Text(pDX, IDC_STC_CONN_TIME, m_strConnTime);
	DDX_Text(pDX, IDC_STC_CONN_MODEM, m_strModem);
	DDX_Text(pDX, IDC_EDT_APN, m_strAPN);
	DDX_Control(pDX, IDC_EDT_APN, m_edtAPN);
	DDX_Control(pDX, IDC_BUTTON_SET, m_btnSet);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialUpView,CCoolFormView )				
	//{{AFX_MSG_MAP(CDialUpView)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_HAND_DIAL, OnBtnHandDial)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_FDU_CMB_SIM, OnSelchangeFduCmbSim)
	ON_REGISTERED_MESSAGE(WM_RASEVENT, OnRasEvent)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialUpView diagnostics

#ifdef _DEBUG
void CDialUpView::AssertValid() const
{
	CCoolFormView::AssertValid();
}

void CDialUpView::Dump(CDumpContext& dc) const
{
	CCoolFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDialUpView message handlers

void CDialUpView::OnInitialUpdate() 
{
	CCoolFormView::OnInitialUpdate();	
	// TODO: Add your specialized code here and/or call the base class
	CheckModem();	

	int nIdx = 0;
	m_cmbSim.ResetContent();
	for(int i = 1; i<= MAX_SIM_NUM; i++)
	{
		if(theApp.IsPluggedSIM(i))
		{
			CString str;
			str.Format(_T("SIM%d"),i);
			m_cmbSim.AddString(str);
			m_cmbSim.SetItemData(nIdx++,i);
		}
	}

	if(nIdx > 0)
	{
		m_cmbSim.SetCurSel(0);
	}

	if(theApp.m_nSIMCount ==  1 )
	{
		m_cmbSim.EnableWindow(FALSE);
		GetDlgItem(IDC_FDU_STC_SIM)->EnableWindow(FALSE);	
	}

#ifdef _EGO
	ActiveCurSIM();
#endif

	InitConnect(m_strModemName);

    m_nDialTime = GetPrivateProfileInt( _T("Net"), _T("DialTime"), 0, theApp.m_szCfgPathName );
	m_dwDialNum = GetPrivateProfileInt( _T("Net"), _T("DialNum"), 0, theApp.m_szCfgPathName );

#ifdef _EGO
	SendCheckRequest();
#else
	m_edtAPN.EnableWindow(FALSE);
	m_btnSet.EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_APN)->EnableWindow(FALSE);
#endif

}

HBRUSH CDialUpView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	return CCoolFormView::OnCtlColor(pDC,pWnd,nCtlColor);
}

void CDialUpView::CheckModem()
{	
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm->m_pCnntView != NULL)
	{
		m_strModemName = pFrm->m_pCnntView->m_strModemName;
		m_strModem = pFrm->m_pCnntView->m_strModemName;
		UpdateData(FALSE);
	}
}

void CDialUpView::OnBtnHandDial() 
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm->m_pCnntView == NULL)
	{
		return;
	}	
	
	CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetDocument();

	if(theApp.m_pSMSFrame != NULL && !pDoc->m_bShowSMSWnd)
	{
		pFrm->SendMessage(WM_COMMAND,IDC_LOAD_SMS,0);
	}

	m_bHandCnnt = !m_bHandCnnt;

	if(m_bHandCnnt)
	{
		m_dwActualTime = 1;
		ClearAll();
		pFrm->m_bDialCnnt = FALSE;
		
		ChangeHandBtnString(m_bHandCnnt);
		if(pDoc != NULL)
		{
			pDoc->m_bDisDialUP = TRUE;
		}
		
		if(!theApp.m_bMuxUsed)
		{
			if(theApp.m_bConnected)
			{
				pFrm->m_pCnntView->CnntOrDisCnntDevice(FALSE);
			}
		}

		SetStateText(IDS_DIAL_START); 
		Dial_up();
		m_dwActualTime ++;

	}
	else
	{
		CWaitCursor cursor;

		SetStateText(IDS_DIAL_DISCONNECTING);

		Release();

		pFrm->m_bDialCnnt = FALSE;

		while(!theApp.m_bHangup)
		{
			Sleep(800);
		}

		//for test
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(CHECK_CONNECT, 0, NULL, 0);

		ChangeHandBtnString(m_bHandCnnt);

		::PostMessage(pFrm->m_hWnd, WM_DISCONNECT_DIAL_UP, NULL, NULL);

// 		if(pDoc != NULL)
// 		{
// 			pDoc->m_bDisDialUP = FALSE;
// 		}
// 		if(!theApp.m_bMuxUsed)
// 		{
// 			pFrm->SwitchToForm(IDD_FORMVIEW_CONNECT);
// 			pDoc->m_uTbCheckFlag[DIAL_CHECK] = 0;
// 
// 			if(!theApp.m_bConnected)
// 			{
// 				pFrm->m_pCnntView->CnntOrDisCnntDevice();
// 			}
// 		}

	}
}

void CDialUpView::ChangeHandBtnString(BOOL bCnnt)
{
	CString strText;
	if(bCnnt)
	{
		strText.LoadString(IDS_DIALUP_DISCONNECT);
		m_btnHandDial.SetWindowText(strText);
	}
	else
	{
		strText.LoadString(IDS_DIALUP_HAND);
		m_btnHandDial.SetWindowText(strText);
	}
}

void CDialUpView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UNREFERENCED_PARAMETER(pSender);

	// TODO: Add your specialized code here and/or call the base class
	
	if(lHint == WM_SET_TIMER)
	{
		if(!m_bSpdTm)
		{
			SetTimer(ID_EVENT_REFLASH, 0, NULL);
			m_bSpdTm = TRUE;
			m_BeginTime = CTime::GetCurrentTime();
			MSINFO_HINT *pMsInfoHint = (MSINFO_HINT *)pHint;
			m_fConnSpeed = (float)(float(pMsInfoHint->dwData_connspeed) / 1000.0);
			m_strConnSpeed.Format(_T("%.1f"), m_fConnSpeed);
		}
	}
}

void CDialUpView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == ID_EVENT_REFLASH)
	{
		CTime curTime = CTime::GetCurrentTime();
		

		CTimeSpan ts= curTime - m_BeginTime;
		
		static MSINFO_HINT msinfoHint;
		msinfoHint.time = ts;

		DWORD dwR,dwS,dwSP;//总接收，总发送
		dwR = 0;
		dwS = 0;
		dwSP = 0;

		theApp.m_RasSpd.GetRASSpeed(&dwS,&dwR, &dwSP);

		msinfoHint.dwData_rcv = dwR;
		msinfoHint.dwData_send = dwS;
		msinfoHint.dwData_connspeed = dwSP;
		
		ReflashItem((CObject *)&msinfoHint);
	}

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

    CCoolFormView::OnTimer(nIDEvent);
}

void CDialUpView::ReflashItem(CObject* pHint)
{
	MSINFO_HINT* pMsinfoHint = (MSINFO_HINT *)pHint;

	m_strConnTime = ConvercTStoS((CTimeSpan)pMsinfoHint->time);

	m_uRcvData = pMsinfoHint->dwData_rcv;
	
	m_uSendData = pMsinfoHint->dwData_send;

//	m_fConnSpeed = (float)(float(pMsInfoHint->dwData_connspeed) / 1000.0);
//	m_strConnSpeed.Format(_T("%.1f"), m_fConnSpeed);


	UpdateData(FALSE);
}

CString CDialUpView::ConvercTStoS(CTimeSpan ts)
{
	CString strTime;
	strTime.Format(_T("%ld:%02ld:%02ld:%02ld"),
	ts.GetDays(), ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());
	return strTime;
}

void CDialUpView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	Release();
	
}

void CDialUpView::Release()
{
	if(m_bSpdTm)
	{
		KillTimer(ID_EVENT_REFLASH);
		m_bSpdTm = FALSE;
	}

	if(m_bKillTimer)
	{
		m_bKillTimer = FALSE;
		KillTimer(EVENT_CHECK_CONNECTED);
	}

	theApp.HangUp();

	SetStateText(IDS_DIAL_DISCONNECTED);
	
	m_dwEntryInfoSize = 0;
    m_dwDeviceInfoSize= 0;
    m_lpRasEntry = NULL;
    m_lpDeviceInfo= NULL;
}

void CDialUpView::ClearAll()
{
	Release();
	
	m_uRcvData = 0;
	m_uSendData = 0;
	m_fConnSpeed = 0.0;
	m_strConnSpeed = _T("0");
	m_strConnTime = _T("0:00:00:00");

	UpdateData(FALSE);
}

void CDialUpView::SendDetachPS()
{	
	
	int nDepatch = GetPrivateProfileInt( _T("PS Depatch"), _T("Depatch"), 0, theApp.m_szCfgPathName );
	
	if(nDepatch == 1)
	{
		if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
		{	
			theApp.m_pEtionFactory->SendATCmd(PS_DEPATCH, 0, NULL, 0);
		}
	}
		
}
BOOL CDialUpView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

    return CCoolFormView::OnEraseBkgnd(pDC);

}
void CDialUpView::OnSelchangeFduCmbSim() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ActiveCurSIM();
	SendCheckRequest();

}
BOOL CDialUpView::ActiveCurSIM()
{
	BOOL bOK = TRUE;

#ifndef _EGO
	if(theApp.m_pPortMain != NULL && theApp.m_bConnected)	
	{
		theApp.m_pPortMain->StopAutoReceive();
	}
#endif

	if(theApp.m_pSMSFactory != NULL && theApp.m_bConnected)
	{
		int nCurSim = m_cmbSim.GetCurSel();
		if(nCurSim != -1)
		{
			nCurSim = (int)m_cmbSim.GetItemData(nCurSim);
			bOK = theApp.m_pSMSFactory->SetActiveSim((_SMS_STORE_PLACE_E)nCurSim);
		}
		
	}

#ifndef _EGO
	if(theApp.m_pPortMain != NULL && theApp.m_bConnected)	
	{
		CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->StartAutoReport();
	}
#endif

	return bOK;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

BOOL CDialUpView::Dial_up()
{
	InitConnect(m_strModemName);

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

BOOL CDialUpView::InitConnect(LPCTSTR lpszModemName)
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

void CDialUpView::GetErrorString(DWORD dwError)
{
	TCHAR szMessage[256];

	RasGetErrorString( dwError, szMessage, 256 ); 
		
	AfxMessageBox(szMessage);
}

void CDialUpView::SetStateText(UINT uID)
{
	CString strText;
	VERIFY(strText.LoadString(uID));
	if(!::IsWindow(this->m_hWnd))
	{
		return;
	}

	CStatic *pStc = ((CStatic *)GetDlgItem(IDC_STATIC_PROMPT));

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
		else
		{
			OnBtnHandDial();
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

void CDialUpView::ProcessDialEvent(RASCONNSTATE rasconnstate)
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
				break;
			}

			((CMainFrame*)(theApp.m_pMainWnd))->m_bDialCnnt = TRUE;
			::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_NOTICE_SET_TIMER, NULL, NULL);
			
			SetStateText(IDS_DIAL_CONNECTED);
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

void CDialUpView::EnumDevices()
{
	LPRASDEVINFO lpdevInfo;
	
	lpdevInfo = new RASDEVINFO[10];
	lpdevInfo->dwSize = sizeof(RASDEVINFO);
	DWORD dwPcb = 10 * sizeof(RASDEVINFO);
	DWORD dwpcDevice = 0;
	DWORD dwRet = RasEnumDevices(lpdevInfo, &dwPcb, &dwpcDevice);

	UNREFERENCED_PARAMETER(dwRet);

	for(UINT i = 0; i < dwpcDevice; i++)
	{
		lpdevInfo ++;
	}
}

BOOL CDialUpView::CheckingConnected()
{
	RASCONNSTATUS rStatus;
	DWORD dwRet;
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
	rStatus.dwSize = sizeof(RASCONNSTATUS);
	dwRet = RasGetConnectStatus(theApp.m_hRasConn, &rStatus);
//	RASCONNSTATE rcs = rStatus.rasconnstate;
	if (ERROR_INVALID_HANDLE == dwRet)
	{
		OnBtnHandDial();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

LRESULT CDialUpView::OnRasEvent(WPARAM wParam, LPARAM lParam)
{
	if(!m_bHandCnnt)
	{
		return 1;
	}
	RASCONNSTATE rasconnstate = (RASCONNSTATE)wParam;
	ProcessDialEvent(rasconnstate);


	DWORD dwError = (DWORD)lParam;
	if(dwError)
	{
		if(m_bHandCnnt)
		{
			GetErrorString(dwError);

			SetStateText(IDS_DIALUP_FAILED);
		}
	}

	return 1;
}

void CDialUpView::OnButtonSet() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest();
}

void CDialUpView::SendCheckRequest()
{
	UpdateData();
	
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{	
		theApp.m_pEtionFactory->SendATCmd(CGDCONT_READ, 0, NULL, 0);
	}
}

void CDialUpView::SendSetRequest()
{
	UpdateData();
	BOOL bFailed = FALSE;

	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};	
		TCHAR szPara[MAX_PATH] = {0};
		
		if(!m_strAPN.IsEmpty())
		{
			_tcscpy(szPara, m_strAPN);
			LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(CONT_IN_SET, 0, (LPVOID)szPara, _tcslen(szPara));
			if(lpRet == NULL)
			{
				ZeroMemory(szRet, MAX_PATH);
			}
			else
			{
				strcpy(szRet, lpRet);
			}
			
			if(strstr(szRet, "ERROR") != NULL || szRet[0] == '\0')
			{
				AfxMessageBox(IDS_CONNIN_FAILED, MB_ICONERROR);
				bFailed = TRUE;
			}
		}

		if(!bFailed)
		{
			AfxMessageBox(IDS_SET_SUCCESS);
			UpdateData(FALSE);
		}
		
	}

	
}

void CDialUpView::ShowRespondContent(LPCSTR szRet)
{
	CString strText = szRet;
	int nIndex = -1;

	if((nIndex = strText.Find(_T("+CGDCONT:1,")) ) != -1)
	{
		strText.Delete(0, nIndex + 11);
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
				m_strAPN = strText.Left(nIndex);
			}
		}

	}
	UpdateData(FALSE);
}