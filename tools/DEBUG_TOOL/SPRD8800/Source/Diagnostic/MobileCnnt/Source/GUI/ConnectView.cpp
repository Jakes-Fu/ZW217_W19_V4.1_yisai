// ConnectPage.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "ConnectView.h"
#include "MainFrm.h"
#include "MobileCnntDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CConnectView

IMPLEMENT_DYNCREATE(CConnectView, CFormView)

CConnectView::CConnectView()
	: CCoolFormView(CConnectView::IDD)
{
	//{{AFX_DATA_INIT(CConnectView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strCom.Empty();
	m_strModemName.Empty();
	m_dwBaudRate = 115200;
	m_nDeviceIndex = 0;
}

CConnectView::~CConnectView()
{
}

void CConnectView::DoDataExchange(CDataExchange* pDX)
{
	CCoolFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectView)
	DDX_Control(pDX, IDC_STATIC_BITMAP, m_stc);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectView, CCoolFormView)
	//{{AFX_MSG_MAP(CConnectView)
#ifdef SPRD8800
	ON_WM_CTLCOLOR()
#endif
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectView diagnostics

#ifdef _DEBUG
void CConnectView::AssertValid() const
{
	CCoolFormView::AssertValid();
}

void CConnectView::Dump(CDumpContext& dc) const
{
	CCoolFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConnectView message handlers

HBRUSH CConnectView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	return CCoolFormView::OnCtlColor(pDC,pWnd,nCtlColor);
}

void CConnectView::OnInitialUpdate() 
{
	CCoolFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_stc.ModifyStyle(0, SS_BITMAP|SS_CENTERIMAGE);
	((CMainFrame *)AfxGetMainWnd())->m_pCnntView = this;
	SetStcBmp(IDB_DIS_START);
	
	Connect();	
	
}

void CConnectView::SetStcBmp(UINT uID)
{
	CBitmap bitmap;
	bitmap.LoadBitmap( uID );
	m_stc.SetBitmap((HBITMAP)bitmap.Detach());
}

void CConnectView::CnntOrDisCnntDevice(BOOL bClearStatusBar)
{	
	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm == NULL)
	{
		return;
	}
	pFrm->ModifyIcon(IDR_MAINFRAME);

	pFrm->SetStatusBarState(PROMT_INFO_IND, IDS_EMPTY, RGB(0,0,0));

	CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetDocument();
	if(pDoc == NULL)
	{
		return;
	}

	if(!theApp.m_bConnected)
	{
		if(theApp.OpenPort())
		{
			if(!CheckAT())
			{
				theApp.ClosePort();
				pFrm->SetStatusBarState(PROMT_INFO_IND, IDS_CONNT_FAILED, RGB(0,0,0));
				return;
			}
			SetStcBmp(IDB_START);
			pFrm->SetStatusBarState(CONNECT_INFO_IND, IDS_CONNECTED, COLOR_CONNECT);
			pFrm->ModifyIcon(IDI_HIGH);
			pDoc->EnableAllItems(TRUE);
#ifdef SPRD8800
			pFrm->PostMessage(WM_COMMAND,ID_OPTION_LOAD_ALL,0);
#endif
		}
		else
		{
			pFrm->SetStatusBarState(PROMT_INFO_IND, IDS_CONNT_FAILED, RGB(0,0,0));
		}

	}
	else
	{
		SetStcBmp(IDB_DIS_START);
		theApp.ClosePort();
		pFrm->ModifyIcon(IDR_MAINFRAME);
		pFrm->ClearAll(bClearStatusBar);
		if(bClearStatusBar)
		{
			pFrm->SetStatusBarState(CONNECT_INFO_IND, IDS_DISCONNECTED, COLOR_DISCONNECT);
		}
		pDoc->EnableAllItems(FALSE);
	}
	
}

BOOL CConnectView::OnEraseBkgnd( CDC* pDC )
{

	CRect rect;
	GetClientRect(rect);	
	CBrush brush(BK_COLOR);
	pDC->FillRect(&rect,&brush);

	return TRUE;
}

BOOL CConnectView::CheckModem(BOOL bShowMsgBox)
{	
	theApp.m_log.LogFmtStr(SPLOGLV_INFO,_T("CheckModem..."));
	if(FindSerialUsbModemComm())
	{		
		if(theApp.m_bMuxUsed)
		{
			if(!FindSerialMuxComm())
			{
				theApp.m_log.LogFmtStr(SPLOGLV_INFO,_T("Mux,FindSerialMuxComm failed."));
//				AfxMessageBox(IDS_CHECK_MUX_DRIVER);
				CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
				if(pFrm == NULL)
				{
					return FALSE;
				}
				pFrm->SetStatusBarState(PROMT_INFO_IND, IDS_CHECK_MUX_DRIVER, RGB(0,0,0));
				
				return FALSE;
			}
		}

		int nCount = m_modemInfo.GetModemCount();

		int nPort = 0;
		CString srCom;
		
		for(int i = 0; i < nCount; i++)
		{
			nPort = m_modemInfo.GetModemComPortByIndex(i);
			srCom.Format(_T("COM%d"), nPort);
			
			if(srCom == m_strCom)
			{
				m_strModemName = m_modemInfo.GetModemFriendlyName(i);
				break;
			}
		}

		m_dwBaudRate = GetPrivateProfileInt( _T("DeviceBaudRate"), _T("BaudRate"), 115200, theApp.m_szCfgPathName);
		
		theApp.m_pPortAttrMain->Com.dwBaudRate = m_dwBaudRate;
		if(theApp.m_bMuxUsed)
		{
			theApp.m_pPortAttrSub->Com.dwBaudRate = m_dwBaudRate;
			theApp.m_pPortAttrVModem1->Com.dwBaudRate = m_dwBaudRate;
			theApp.m_pPortAttrVModem2->Com.dwBaudRate = m_dwBaudRate;
		}
		return TRUE;
	}
	else
	{
		theApp.m_log.LogFmtStr(SPLOGLV_INFO,_T("FindSerialUsbModemComm failed."));
		if(bShowMsgBox)
		{			
#ifdef SPRD8800
//			AfxMessageBox(IDS_PLEASE_CONFIRM_DEVICE);
#else
//			CString strMsg;
//			strMsg.LoadString(IDS_PLEASE_CONFIRM_DEVICE);
//			MessageBox(strMsg,_T("eGo"),MB_OK | MB_ICONWARNING);
#endif
		}
		CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
		if(pFrm == NULL)
		{
			return FALSE;
		}
		pFrm->SetStatusBarState(PROMT_INFO_IND, IDS_PLEASE_CONFIRM_DEVICE, RGB(0,0,0));
		
		return FALSE;
	}
}

BOOL CConnectView::FindSerialUsbModemComm()
{
	_TCHAR szDevice[128] = {0};
	
	int nCount = GetPrivateProfileInt(_T("DeviceCount"), _T("Count"), 1, theApp.m_szCfgPathName);
	int nPort = 0;

	for(int i = 0; i < nCount; i++)
	{
		_TCHAR szSection[20] = {0};
		swprintf(szSection, _T("DeviceGeneral%d"), i+1);

		GetPrivateProfileString(szSection, _T("Device"),_T("SciModem"),szDevice, MAX_PATH, theApp.m_szCfgPathName);
		m_strModemName = szDevice;
		nPort = m_EnumPort.FindPort(szDevice, FALSE);
		if(nPort != 0)
		{
			m_nDeviceIndex = i+1;
			break;
		}
	}

	if(nPort != 0)
	{
		m_strCom.Format(_T("COM%d"), nPort);
		_tcscpy(theApp.m_pPortAttrMain->Com.szName, m_strCom);
		theApp.m_pPortAttrMain->Com.dwBaudRate = 115200;
		_tcscpy(theApp.m_pPortAttrUsbModem->Com.szName, m_strCom);
		theApp.m_pPortAttrUsbModem->Com.dwBaudRate = 115200;
		return TRUE;
	}

	return FALSE;
}

BOOL CConnectView::FindSerialMuxComm()
{
	_TCHAR szDevice[128] = {0};
	int nDeviceCountSer = 0;
	int nDeviceCountMod = 0;
	CStringArray strDeviceArraySer;
	CStringArray strDeviceArrayMod;
	_TCHAR szTmp[128] = {0};

	_TCHAR szSection[20] = {0};
	swprintf(szSection, _T("DeviceMux%d"), m_nDeviceIndex);

	nDeviceCountSer = GetPrivateProfileInt(szSection, _T("DeviceSerCount"), 2, theApp.m_szCfgPathName);
	nDeviceCountMod = GetPrivateProfileInt(szSection, _T("DeviceModCount"), 2, theApp.m_szCfgPathName);

	for(int i = 1; i <= nDeviceCountSer; i++)
	{
		_stprintf(szTmp, _T("DeviceSer%d"), i);

		GetPrivateProfileString( szSection, szTmp,_T(""),szDevice, MAX_PATH, theApp.m_szCfgPathName);

		strDeviceArraySer.Add(szDevice);
	}

	for(i = 1; i <= nDeviceCountMod; i++)
	{
		_stprintf(szTmp, _T("DeviceMod%d"), i);

		GetPrivateProfileString( szSection, szTmp,_T(""),szDevice, MAX_PATH, theApp.m_szCfgPathName);

		strDeviceArrayMod.Add(szDevice);
	}

	m_strModemName = strDeviceArrayMod.GetAt(0);

	int nSerPort0 = m_EnumPort.FindPort(strDeviceArraySer.GetAt(0), FALSE);
	int nSerPort1 = m_EnumPort.FindPort(strDeviceArraySer.GetAt(1), FALSE);
	int nModPort0 = m_EnumPort.FindPort(strDeviceArrayMod.GetAt(0), FALSE);
	int nModPort1 = m_EnumPort.FindPort(strDeviceArrayMod.GetAt(1), FALSE);

	if(nSerPort0 == 0 || nSerPort1 == 0 || nModPort0 == 0 || nModPort1 == 0)
	{
		return FALSE;
	}

	CString strPort;
	strPort.Format(_T("COM%d"), nSerPort0);	
	_tcscpy(theApp.m_pPortAttrMain->Com.szName, strPort);
	strPort.Format(_T("COM%d"), nSerPort1);	
	_tcscpy(theApp.m_pPortAttrSub->Com.szName, strPort);
	strPort.Format(_T("COM%d"), nModPort0);	
	_tcscpy(theApp.m_pPortAttrVModem1->Com.szName, strPort);
	m_strCom = theApp.m_pPortAttrVModem1->Com.szName;
	strPort.Format(_T("COM%d"), nModPort1);	
	_tcscpy(theApp.m_pPortAttrVModem2->Com.szName, strPort);

	return TRUE;
}

void CConnectView::Connect(BOOL bShowMsgBox)
{
	theApp.m_log.LogFmtStr(SPLOGLV_INFO,_T("Connect."));
	m_EnumPort.SetLog((ISpLog*)&theApp.m_log);
	m_EnumPort.EnumPort(NULL, NULL,true);
#if defined(_EGO)
	if(theApp.m_bManual)
	{
		CnntOrDisCnntDevice();
	}
	else
	{
		if(CheckModem(bShowMsgBox))
		{
			CnntOrDisCnntDevice();
		}
	}
#else
	if(CheckModem(bShowMsgBox))
	{
		CnntOrDisCnntDevice();
	}
	else
	{
		CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->SetStatusBarState(PROMT_INFO_IND, IDS_DISCONNECTED_CHECKMODEM, COLOR_DISCONNECT);
	}
#endif
}

BOOL CConnectView::CheckAT()
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(CHECK_CONNECT, 0, NULL, 0);
		if(lpRet != NULL)
		{
			strcpy(szRet, lpRet);
		}
		
		if(strstr(szRet, "ERROR") != NULL)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
		
	}
	else
	{
		return FALSE;
	}
}
