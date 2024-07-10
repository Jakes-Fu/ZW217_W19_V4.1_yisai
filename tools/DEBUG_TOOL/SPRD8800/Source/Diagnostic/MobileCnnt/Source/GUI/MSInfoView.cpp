// MSInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "MSInfoView.h"
#include "MainFrm.h"
#include "MobileCnntDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMSInfoView

IMPLEMENT_DYNCREATE(CMSInfoView, CFormView)

CMSInfoView::CMSInfoView()
	: CCoolFormView(CMSInfoView::IDD)
{
	//{{AFX_DATA_INIT(CMSInfoView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bSysTm = FALSE;
	m_bGSM = FALSE;
	m_bUrgency= FALSE;
	m_nLastSimState = -1;
	m_bNoService = FALSE;
}

CMSInfoView::~CMSInfoView()
{
}

void CMSInfoView::DoDataExchange(CDataExchange* pDX)
{
	CCoolFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMSInfoView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_STC_SIGNAL, m_strSignal);
	DDX_Text(pDX, IDC_STC_NETWORK_TYPE, m_strNetType);
	DDX_Text(pDX, IDC_STC_NETWORK_PROVIDER, m_strNetProvider);
	DDX_Text(pDX, IDC_STC_SYSTEM_SRV, m_strSysSrv);
	DDX_Text(pDX, IDC_STC_SYSTEM_DOMAIN, m_strDomain);
	DDX_Text(pDX, IDC_STC_ROAM, m_strRoam);
	DDX_Text(pDX, IDC_STC_SYSTEM_MODE, m_strMode);
	DDX_Text(pDX, IDC_STC_SIM, m_strSim);
	DDX_Text(pDX, IDC_STC_IMEI, m_strImei);
	DDX_Text(pDX, IDC_STC_CELL_SIGN, m_strCellSign);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMSInfoView, CCoolFormView)
	//{{AFX_MSG_MAP(CMSInfoView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSInfoView diagnostics

#ifdef _DEBUG
void CMSInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMSInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMSInfoView message handlers

void CMSInfoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UNREFERENCED_PARAMETER(pSender);
	// TODO: Add your specialized code here and/or call the base class
	
	if(lHint == WM_MSINFO_SET)
	{
		SendCheckRequest();
		SendSystemServiceRequest();
		SendSystemRequest();
		UpdateData(FALSE);
		StartupCheck();
	}
	else if(lHint == WM_RCV_SIGNAL)
	{
		char szRet[MAX_PATH] = {0};
		DATA_HINT data_hint;

		memcpy(&data_hint, (DATA_HINT *)pHint, sizeof(DATA_HINT));
		
		memcpy(szRet, data_hint.lpString, data_hint.dwLen);
		
		GetSignalIntensity(szRet);
		
		UpdateData(FALSE);
	}
	else if(lHint == WM_RCV_NETWORKINFO)
	{
		char szRet[MAX_PATH] = {0};
		DATA_HINT data_hint;

		memcpy(&data_hint, (DATA_HINT *)pHint, sizeof(DATA_HINT));
		
		memcpy(szRet, data_hint.lpString, data_hint.dwLen);
		
		GetNetworkInfo(szRet);
		
		UpdateData(FALSE);
	}
	else if(lHint == WM_GET_SYSTEM_SERVICE)
	{
		char szRet[MAX_PATH] = {0};
		DATA_HINT data_hint;

		memcpy(&data_hint, (DATA_HINT *)pHint, sizeof(DATA_HINT));
		
		memcpy(szRet, data_hint.lpString, data_hint.dwLen);
		
		GetSystemService(szRet);
		
		UpdateData(FALSE);
	}
	else if(lHint == WM_STARTUP_CHECK)
	{
		StartupCheck();
	}
	else if(lHint == WM_STOP_CHECK)
	{
		StopCheck();
	}
	
}

void CMSInfoView::SendCheckRequest()
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(NETWORK, 0, NULL, 0);

		theApp.m_pEtionFactory->SendATCmd(CGSN_CHECK, 0, NULL, 0);

		theApp.m_pEtionFactory->SendATCmd(SIGNAL, 0, NULL, 0);	
		
		UpdateData(FALSE);
		
	}
}

void CMSInfoView::GetSignalIntensity(LPCSTR szRet)
{
	CString strText = szRet;
	int nIndex = strText.Find(_T(":"));
	
	strText.Delete(0, nIndex + 1);

	nIndex = strText.Find(_T(","));

	m_strSignal =  strText.Left(nIndex);
	
	SetStatusSignal(m_strSignal);
}

void CMSInfoView::GetNetworkInfo(LPCSTR szRet)
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm == NULL)
	{
		return;
	}

	CString strText = szRet;
	
	int nIndex = strText.Find(_T(","));
	
	if(nIndex == -1)
	{
		m_strNetProvider.Empty();
		return;
	}

	strText.Delete(0, nIndex + 1);

	nIndex = strText.Find(_T(","));
	if(nIndex == -1)
	{
		m_strNetProvider.Empty();
		return;
	}

	if(strText.Left(nIndex) == _T("0"))
	{
		m_strNetType = _T("GSM");
	}
	if(strText.Left(nIndex) == _T("1"))
	{
		m_strNetType = _T("GSM Compact");
	}
	if(strText.Left(nIndex) == _T("2"))
	{
		m_strNetType = _T("UTRAN");
	}

	strText.Delete(0, nIndex + 1);

	nIndex = strText.Find(_T(","));
	if(nIndex == -1)
	{
		m_strNetProvider = strText;
	}
	else
	{
		m_strNetProvider = strText.Left(nIndex);
	}

//	if((m_strNetProvider.Find(_T("46000")) != -1) || (m_strNetProvider.Find(_T("46001")) != -1))
//	{
//		m_bGSM = TRUE;
//	}
//	if(m_strNetProvider.Find(_T("46007")) != -1)
//	{
//		m_bGSM = FALSE;
//	}
//
//	UINT uID = 0;
//
//#ifdef _UNICODE
//	swscanf(m_strNetProvider, _T("%d"), &uID);
//#else
//	sscanf(m_strNetProvider, _T("%d"), &uID);
//#endif
//
//	pFrm->SetProviderStatus(uID);

	m_strNetProvider = pFrm->MapNetProvider(m_strNetProvider);

	
}


void CMSInfoView::GetSystemInfo(LPCSTR szRet)
{
	CString strText = szRet;
	int nIndex = strText.Find(_T("^SYSINFO:"));
	UINT uId = 0;

	if(nIndex != -1)
	{
		strText.Delete(0, 9);
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
#ifdef _UNICODE
			swscanf(strText.Left(nIndex), _T("%d"), &uId);
#else
			sscanf(strText.Left(nIndex), _T("%d"), &uId);
#endif
			
			//m_strSysSrv = GetSysStringByIndex(SYSTEM_SERVICE ,uId);
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
#ifdef _UNICODE
				swscanf(strText.Left(nIndex), _T("%d"), &uId);
#else
				sscanf(strText.Left(nIndex), _T("%d"), &uId);
#endif
				
				m_strDomain = GetSysStringByIndex(SYSTEM_DOMAIN ,uId);
				strText.Delete(0, nIndex + 1);
				nIndex = strText.Find(_T(","));
				if(nIndex != -1)
				{
#ifdef _UNICODE
					swscanf(strText.Left(nIndex), _T("%d"), &uId);
#else
					sscanf(strText.Left(nIndex), _T("%d"), &uId);
#endif
					
					m_strRoam = GetSysStringByIndex(SYSTEM_ROAM ,uId);
					strText.Delete(0, nIndex + 1);
					nIndex = strText.Find(_T(","));
					if(nIndex != -1)
					{
#ifdef _UNICODE
						swscanf(strText.Left(nIndex), _T("%d"), &uId);
#else
						sscanf(strText.Left(nIndex), _T("%d"), &uId);
#endif
						if(uId == 3)
						{
							m_bGSM = TRUE;
							((CMainFrame *)AfxGetMainWnd())->SetProviderStatus(GSM_MODE);
						}
						else if(uId == 15)
						{
							m_bGSM = FALSE;
							((CMainFrame *)AfxGetMainWnd())->SetProviderStatus(TD_MODE);
						}
						else if(uId == 5)
						{
							m_bGSM = FALSE;
							((CMainFrame *)AfxGetMainWnd())->SetProviderStatus(W_MODE);
						}
						else
						{
							m_bGSM = FALSE;
							((CMainFrame *)AfxGetMainWnd())->SetProviderStatus(UNKNOWN);
						}
						
						m_strMode = GetSysStringByIndex(SYSTEM_MODE ,uId);
						strText.Delete(0, nIndex + 1);

						//teana hu 2009.05.25
						nIndex = strText.Find(_T(","));
						if(nIndex == -1)
						{
#ifdef _UNICODE
							swscanf(strText, _T("%d"), &uId);
#else
							sscanf(strText, _T("%d"), &uId);
#endif
							m_strSim = GetSysStringByIndex(SYSTEM_SIM_STATUES, uId);
						}
						else
						{
#ifdef _UNICODE
							swscanf(strText.Left(nIndex), _T("%d"), &uId);
#else
							sscanf(strText.Left(nIndex), _T("%d"), &uId);
#endif
							m_strSim = GetSysStringByIndex(SYSTEM_SIM_STATUES, uId);
							
							strText.Delete(0, nIndex + 1);

							nIndex = strText.Find(_T(","));
							if(nIndex != -1)
							{
								strText.Delete(0, nIndex + 1); //delete reserve param
								nIndex = strText.Find(_T(","));
								if(nIndex == -1)
								{
#ifdef _UNICODE
									swscanf(strText, _T("%d"), &uId);
#else
									sscanf(strText, _T("%d"), &uId);
#endif
								}
								else
								{
#ifdef _UNICODE
									swscanf(strText.Left(nIndex), _T("%d"), &uId);
#else
									sscanf(strText.Left(nIndex), _T("%d"), &uId);
#endif
								}
								m_strCellSign = GetSysStringByIndex(SYSTEM_CELL_SIGN, uId);
							}
						}
						
					}
				}
			}
		}
		
	}
}

CString CMSInfoView::GetSysStringByIndex(DWORD dwType, UINT uIndex)
{
	CString strText;
	
	CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetDocument();
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();

	switch(dwType)
	{
	case SYSTEM_SERVICE:
		if(uIndex == 0)
		{
			strText.LoadString(IDS_SYSTEM_SERVICE_0);
			m_bUrgency = FALSE;
			m_bNoService = TRUE;
			if(pDoc != NULL)
			{
				pDoc->m_bEnableCall = FALSE;
			}
		}
		if(uIndex == 1)
		{
			strText.LoadString(IDS_SYSTEM_SERVICE_1);
			m_bUrgency = FALSE;
			m_bNoService = FALSE;
			if(pDoc != NULL)
			{
				pDoc->m_bEnableSMS = TRUE;
			}
		}
		if(uIndex == 2)
		{
			strText.LoadString(IDS_SYSTEM_SERVICE_2);
			m_bUrgency = FALSE;
			m_bNoService = FALSE;
		}
		if(uIndex == 3)
		{
			strText.LoadString(IDS_SYSTEM_SERVICE_3);
			m_bUrgency = TRUE;
			m_bNoService = FALSE;
			if(pDoc != NULL)
			{
				pDoc->m_bEnableCall = TRUE;
			}
		}
		if(uIndex == 4)
		{
			strText.LoadString(IDS_SYSTEM_SERVICE_4);
			m_bUrgency = FALSE;
			m_bNoService = FALSE;
		}
		if(uIndex == 5)
		{
			strText.LoadString(IDS_SYSTEM_SERVICE_1);
			m_bUrgency = FALSE;
			m_bNoService = FALSE;
			if(pDoc != NULL)
			{
				pDoc->m_bEnableSMS = TRUE;
			}
		}
		break;
	case SYSTEM_DOMAIN:
		if(uIndex == 0)
		{
			strText.LoadString(IDS_SYSTEM_DOMAIN_0);
			if(pDoc != NULL)
			{
				pDoc->m_bEnableDialUP = FALSE;
				if(!m_bUrgency)
				{
					pDoc->m_bEnableCall = FALSE;
				}
			}
		}
		if(uIndex == 1)
		{
			strText.LoadString(IDS_SYSTEM_DOMAIN_1);
			if(pDoc != NULL)
			{
				pDoc->m_bEnableDialUP = TRUE;
				pDoc->m_bEnableCall = TRUE;
			}
		}
		if(uIndex == 2)
		{
			strText.LoadString(IDS_SYSTEM_DOMAIN_2);
			if(pDoc != NULL)
			{
				pDoc->m_bEnableDialUP = TRUE;
				pDoc->m_bEnableCall = FALSE;
			}
		}
		if(uIndex == 3)
		{
			strText.LoadString(IDS_SYSTEM_DOMAIN_3);
			if(pDoc != NULL)
			{
				pDoc->m_bEnableDialUP = TRUE;
				pDoc->m_bEnableCall = TRUE;
			}
		}
		break;
	case SYSTEM_ROAM:
		if(uIndex == 0)
		{
			strText.LoadString(IDS_SYSTEM_ROAM_0);
		}
		if(uIndex == 1)
		{
			strText.LoadString(IDS_SYSTEM_ROAM_1);
			pFrm->SetStatusBarState(ROAM_INFO_IND, IDS_MOBILE_ROAM, RGB(0,0,0));
		}
		break;
	case SYSTEM_MODE:
		if(uIndex == 0)
		{
			strText.LoadString(IDS_SYSTEM_MODE_0);
		}
		if(uIndex == 1)
		{
			strText.LoadString(IDS_SYSTEM_MODE_1);
		}
		if(uIndex == 2)
		{
			strText.LoadString(IDS_SYSTEM_MODE_2);
		}
		if(uIndex == 3)
		{
			strText.LoadString(IDS_SYSTEM_MODE_3);
		}
		if(uIndex == 4)
		{
			strText.LoadString(IDS_SYSTEM_MODE_4);
		}
		if(uIndex == 5)
		{
			strText.LoadString(IDS_SYSTEM_MODE_5);
		}
		if(uIndex == 15)
		{
			strText.LoadString(IDS_SYSTEM_MODE_6);
		}
		break;
	case SYSTEM_SIM_STATUES:
//		if(pFrm != NULL && pFrm->m_bSimInvalid)
//		{
//			uIndex = 255;
//		}
		if(uIndex == 0)
		{
			strText.LoadString(IDS_SIM_STATUES_0);
			if(!m_bNoService)
			{
				m_bUrgency = TRUE;
			}
			else
			{
				m_bUrgency = FALSE;
			}
			
			if(pDoc != NULL)
			{
				pDoc->m_bEnableSMS = FALSE;
				pDoc->m_bEnableCall = TRUE;
			}
		}
		if(uIndex == 1)
		{
			strText.LoadString(IDS_SIM_STATUES_1);
			m_bUrgency = FALSE;
			pFrm->m_bSimInvalid = FALSE;
			if(pDoc != NULL)
			{
				pDoc->m_bEnableSMS = TRUE;
			}
			if(pFrm->m_pCfgView != NULL &&
				pFrm->m_pCfgView->m_pSecurityDlg != NULL &&
				pFrm->m_pCfgView->m_pSecurityDlg->GetSafeHwnd() != NULL)
			{
				pFrm->m_pCfgView->m_pSecurityDlg->EnableWindowAll(TRUE);
			}
			pFrm->m_wndStatusBar.SetPaneText(PROMT_INFO_IND,_T(" "));
		}
		if(uIndex == 255)
		{
			strText.LoadString(IDS_SIM_STATUES_2);
			if(!m_bNoService)
			{
				m_bUrgency = TRUE;
			}
			else
			{
				m_bUrgency = FALSE;
			}
			
			if(pDoc != NULL)
			{
				pDoc->m_bEnableSMS = FALSE;
				pDoc->m_bEnableCall = TRUE;
			}
		}
		if(!pFrm->m_bSimInvalid)
		{
			JudeSimInvalid(uIndex);
		}
		
		m_nLastSimState = uIndex;
		break;
	case SYSTEM_CELL_SIGN:
		if(uIndex == 0)
		{
			strText.LoadString(IDS_SYSTEM_CELL_SIGN_0);
		}
		if(uIndex == 1)
		{
			strText.LoadString(IDS_SYSTEM_CELL_SIGN_1);
		}
		if(uIndex == 2)
		{
			strText.LoadString(IDS_SYSTEM_CELL_SIGN_2);
		}
		if(uIndex == 3)
		{
			strText.LoadString(IDS_SYSTEM_CELL_SIGN_3);
		}
		if(uIndex == 4)
		{
			strText.LoadString(IDS_SYSTEM_CELL_SIGN_4);
		}
		if(uIndex == 5)
		{
			strText.LoadString(IDS_SYSTEM_CELL_SIGN_5);
		}
		if(uIndex == 6)
		{
			strText.LoadString(IDS_SYSTEM_CELL_SIGN_6);
		}
		if(uIndex == 7)
		{
			strText.LoadString(IDS_SYSTEM_CELL_SIGN_7);
		}
		if(uIndex == 8)
		{
			strText.LoadString(IDS_SYSTEM_CELL_SIGN_8);
		}

		break;
	default:
		break;
	}
	return strText;
}

void CMSInfoView::SetStatusSignal(CString strSignal)
{
	UINT uCsq = 0;
#ifdef _UNICODE
	swscanf(strSignal, _T("%d"), &uCsq);
#else
	sscanf(strSignal, _T("%d"), &uCsq);
#endif

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm == NULL)
	{
		return;
	}

	if(m_bGSM)
	{
		if(uCsq > 0 && uCsq <=6)
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL1);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL01);
#endif
			
		}
		else if(uCsq >= 7 && uCsq <= 13)
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL2);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL02);
#endif
			
		}
		else if(uCsq >= 14 && uCsq <= 20)
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL3);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL03);
#endif
			
		}
		else if(uCsq >= 21 && uCsq <= 27)
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL4);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL04);
#endif
			
		}
		else if(uCsq >= 28 && uCsq <= 33)
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL5);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL05);
#endif
			
		}
		else
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL0);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL00);
#endif
			
		}
	}
	else
	{
		if(uCsq > 100 && uCsq <=117)
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL1);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL01);
#endif
			
		}
		else if(uCsq >= 118 && uCsq <= 135)
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL2);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL02);
#endif
			
		}
		else if(uCsq >= 136 && uCsq <= 153)
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL3);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL03);
#endif
			
		}
		else if(uCsq >= 154 && uCsq <= 171)
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL4);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL04);
#endif
			
		}
		else if(uCsq >= 172 && uCsq <= 191)
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL5);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL05);
#endif
		}
		else
		{
#ifdef _EGO
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL0);
#else
			pFrm->SetSiganalIcon(IDI_ICON_SIGNAL00);
#endif
			
		}
	}
}

void CMSInfoView::SendSystemServiceRequest()
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(SYSTEM_SERVICE_AUTO, 0, NULL, 0);
		
		theApp.m_pEtionFactory->SendATCmd(SYSTEM_SERVICE_CHECK, 0, NULL, 0);
	}
}

void CMSInfoView::GetSystemService(LPCSTR szRet)
{
	CString strText = szRet;
	int nIndex = -1;

	if(strText.Find(_T("+CREG:")) != -1)
	{
		int nSign = 0;
		CString strTmp = strText;
		int nInd = strTmp.Find(_T(","));
		while(nInd != -1)
		{
			nSign ++;
			strTmp.Delete(0, nInd + 1);
			nInd = strTmp.Find(_T(","));
		}

		nIndex = strText.Find(_T(","));
		if(nIndex != 0)
		{
			if(nSign == 4)
			{
				strText.Delete(0,nIndex + 1);  //区分主动上报(4 Param)和自动查询(5 Param)
			}
#ifdef _UNICODE
			swscanf(strText.Left(1), _T("%d"), &nIndex);
#else
			sscanf(strText.Left(1), _T("%d"), &nIndex);
#endif
			
			//m_strSysSrv = SYSTEM_SERVICE[nIndex];
			m_strSysSrv = GetSysStringByIndex(SYSTEM_SERVICE ,nIndex);

			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
				strText.Delete(0,nIndex + 1);
				nIndex = strText.Find(_T(","));
				if(nIndex != -1)
				{
					strText.Delete(0,nIndex + 1);
					nIndex = strText.Find(_T(","));
					if(nIndex != -1)
					{
						strText.Delete(0, nIndex + 1);
						int nCellSign= 0;
#ifdef _UNICODE
						swscanf(strText, _T("%d"), &nCellSign);
#else
						sscanf(strText, _T("%d"), &nCellSign);
#endif
						
						if(nCellSign == 0)
						{
							m_strCellSign.LoadString(IDS_SYSTEM_CELL_SIGN_1);
							//m_strCellSign = _T("GSM");
						}
						
						if(nCellSign == 1)
						{
							m_strCellSign.LoadString(IDS_SYSTEM_CELL_SIGN_1);
							//m_strCellSign = _T("GSM Compact");
						}
						
						if(nCellSign == 2)
						{
							m_strCellSign.LoadString(IDS_SYSTEM_CELL_SIGN_8);
							m_strCellSign = _T(" UTRAN");
						}
						
						if(nCellSign == 3)
						{
							m_strCellSign.LoadString(IDS_SYSTEM_CELL_SIGN_3);
							//m_strCellSign = _T(" GSM w/EGPRS");
						}
						
						if(nCellSign == 4)
						{
							m_strCellSign.LoadString(IDS_SYSTEM_CELL_SIGN_5);
							//m_strCellSign = _T("UTRAN w/HSDPA");
						}

						if(nCellSign == 5)
						{
							m_strCellSign.LoadString(IDS_SYSTEM_CELL_SIGN_6);
							//m_strCellSign = _T("UTRAN w/HSUPA");
						}

						if(nCellSign == 6)
						{
							m_strCellSign.LoadString(IDS_SYSTEM_CELL_SIGN_7);
							//m_strCellSign = _T("UTRAN w/HSDPA and HSUPA");
						}

					}
				}
			}
		}
		UpdateData(FALSE);		
	}
}

void CMSInfoView::SendSystemRequest()
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(SYSTEM_CHECK, 0, NULL, 0);
	}
}

void CMSInfoView::ClearAll()
{
	m_strSignal.Empty();
	m_strNetType.Empty();
	m_strNetProvider.Empty();

	m_strSysSrv.Empty();
	m_strDomain.Empty();
	m_strRoam.Empty();
	m_strMode.Empty();
	m_strSim.Empty();
	UpdateData(FALSE);
	m_nLastSimState = -1;
}

void CMSInfoView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == ID_EVENT_SYSTEM)
	{
		SendCheckRequest();
		SendSystemRequest();
		UpdateData(FALSE);
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CMSInfoView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	StopCheck();
	
}

void CMSInfoView::ShowRespondContent(LPCSTR szRet)
{
	CString strText = szRet;
	if(strText.Find(_T("^SYSINFO")) != -1)
	{
		GetSystemInfo(szRet);
	}
	if(((CMainFrame *)AfxGetMainWnd())->IsIMEI(szRet))
	{
		m_strImei = szRet;
	}
	if(strText.Find(_T("^MODE")) != -1)
	{
		GetSystemMode(strText);
	}
	if(strText.Find(_T("+SIND:")) != -1)
	{
		GetSIND(strText);
	}
	UpdateData(FALSE);
}

void CMSInfoView::GetSystemMode(LPCTSTR szRet)
{
	CString strText = szRet;
	int nIndex = strText.Find(_T("^MODE:"));
	UINT uId = 0;

	if(nIndex != -1)
	{
			strText.Delete(0, 6);
#ifdef _UNICODE
			swscanf(strText, _T("%d"), &uId);
#else
			sscanf(strText, _T("%d"), &uId);
#endif
			m_strMode = GetSysStringByIndex(SYSTEM_MODE ,uId);
		
	}

}

void CMSInfoView::GetSIND(LPCTSTR szRet)
{
	CString strText = szRet;
	int nIndex = strText.Find(_T("+SIND: "));
	UINT uId = 0;

	if(nIndex != -1)
	{
		strText.Delete(0, 7);
		nIndex = strText.Find(_T(","));
		if(nIndex == -1)
		{
#ifdef _UNICODE
			swscanf(strText, _T("%d"), &uId);
#else
			sscanf(strText, _T("%d"), &uId);
#endif
		}
		else
		{
#ifdef _UNICODE
			swscanf(strText.Left(nIndex), _T("%d"), &uId);
#else
			sscanf(strText.Left(nIndex), _T("%d"), &uId);
#endif
		}

		
	}

	CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetDocument();
	
	switch(uId) {
			case 0:
				JudeSimInvalid(uId);
				m_strSim.LoadString(IDS_SIM_SIND);
				m_nLastSimState = uId;
				break;
			case 1:
				m_strSim.LoadString(IDS_SIM_STATUES_1);
				m_nLastSimState = uId;
				((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(PROMT_INFO_IND,_T(" "));
				break;
			case 7:
				m_strSysSrv.LoadString(IDS_SYSTEM_SERVICE_3);
				if(!m_bNoService)
				{
					m_bUrgency = TRUE;
				}
				else
				{
					m_bUrgency = FALSE;
				}
				
				if(pDoc != NULL)
				{
					pDoc->m_bEnableCall = TRUE;
				}
				break;
			case 8:
				m_strDomain.LoadString(IDS_SYSTEM_DOMAIN_0);
				break;
			case 11:
				{
					m_strSysSrv.LoadString(IDS_SYSTEM_SERVICE_1);
					m_strDomain.LoadString(IDS_SYSTEM_DOMAIN_3);
					m_bUrgency = FALSE;
					if(pDoc != NULL)
					{
						pDoc->m_bEnableDialUP = TRUE;
						pDoc->m_bEnableCall = TRUE;
						pDoc->m_bEnableSMS = TRUE;
					}
					break;
				}
			default:
				break;
			}
}

void CMSInfoView::JudeSimInvalid(int nSimState)
{
	if(m_nLastSimState == 1 && (nSimState == 0 || nSimState == 255))
	{
		CString strText;
		strText.LoadString(IDS_REMOVE_SIM_CARD);
		CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->m_wndStatusBar.SetPaneText(PROMT_INFO_IND,strText);
//		AfxMessageBox(strText);
		
//		if(pFrm != NULL)
//		{
//			::PostMessage(pFrm->m_hWnd, WM_DISCONNECT_DIAL_UP, NULL, NULL);
//		}
		pFrm->m_bSimInvalid = TRUE;
		if(pFrm->m_pCfgView != NULL &&
			pFrm->m_pCfgView->m_pSecurityDlg != NULL &&
			pFrm->m_pCfgView->m_pSecurityDlg->GetSafeHwnd() != NULL)
		{
			pFrm->m_pCfgView->m_pSecurityDlg->EnableWindowAll(FALSE);
		}

		CMobileCnntDoc *pDoc = (CMobileCnntDoc *)GetDocument();
		if(pDoc != NULL)
		{
			pDoc->m_bEnableDialUP = FALSE;
		}
		
	}
}

void CMSInfoView::StartupCheck()
{
	if(!m_bSysTm)
	{
		SetTimer(ID_EVENT_SYSTEM, SYSTEM_REFLASH_TIME, NULL);
		m_bSysTm = TRUE;
	}
}

void CMSInfoView::StopCheck()
{
	if(m_bSysTm)
	{
		KillTimer(ID_EVENT_SYSTEM);
		m_bSysTm = FALSE;
	}
}
