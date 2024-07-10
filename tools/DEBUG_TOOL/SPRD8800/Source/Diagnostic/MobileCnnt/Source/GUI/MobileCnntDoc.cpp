// MobileCnntDoc.cpp : implementation of the CMobileCnntDoc class
//

#include "stdafx.h"
#include "MobileCnnt.h"

#include "MobileCnntDoc.h"
#include "MainFrm.h"
#include "MSInfoView.h"
#include "DialUpView.h"
#include "CallView.h"
#include "EngineeringView.h"
#include "ConfigView.h"
#include "ConnectView.h"
#include "DlgINputPin1.h"
#include "DlgInputPuk1.h"
#include "DialUpDlg.h"
#include "DlgUnlockPin2.h"
#include "DialUpDlg.h"
#include "DlgHsdpaVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntDoc

IMPLEMENT_DYNCREATE(CMobileCnntDoc, CDocument)

BEGIN_MESSAGE_MAP(CMobileCnntDoc, CDocument)
	//{{AFX_MSG_MAP(CMobileCnntDoc)
	ON_COMMAND(IDC_LOAD_SMS, OnLoadSMS)
	ON_COMMAND(IDC_DIAL_UP, OnDialUp)
	ON_UPDATE_COMMAND_UI(IDC_DIAL_UP, OnUpdateDialUp)
	ON_COMMAND(IDC_LOAD_CALL, OnLoadCall)
	ON_UPDATE_COMMAND_UI(IDC_LOAD_CALL, OnUpdateLoadCall)
	ON_COMMAND(IDC_LOAD_CONFIG, OnLoadConfig)
	ON_UPDATE_COMMAND_UI(IDC_LOAD_CONFIG, OnUpdateLoadConfig)
	ON_COMMAND(IDC_LOAD_ENGENEERING, OnLoadEngeneering)
	ON_UPDATE_COMMAND_UI(IDC_LOAD_ENGENEERING, OnUpdateLoadEngeneering)
	ON_COMMAND(IDC_LOAD_HELP, OnLoadHelp)
	ON_UPDATE_COMMAND_UI(IDC_LOAD_HELP, OnUpdateLoadHelp)
	ON_COMMAND(ID_HOME, OnHome)
	ON_UPDATE_COMMAND_UI(IDC_LOAD_SMS, OnUpdateLoadSms)
	ON_UPDATE_COMMAND_UI(ID_HOME, OnUpdateHome)
	ON_COMMAND(ID_DIS_DIAL_UP, OnDisDialUp)
	ON_UPDATE_COMMAND_UI(ID_DIS_DIAL_UP, OnUpdateDisDialUp)
	ON_COMMAND(ID_OPTION_LOAD_ALL, OnLoadAll)
	ON_UPDATE_COMMAND_UI(ID_OPTION_LOAD_ALL, OnUpdateLoadAll)
	ON_COMMAND(IDC_POWER_OFF, OnPowerOff)
	ON_UPDATE_COMMAND_UI(IDC_POWER_OFF, OnUpdatePowerOff)
	ON_COMMAND(ID_LOAD_HSDPA_VERSION, OnHsdpaVersion)
	ON_UPDATE_COMMAND_UI(ID_LOAD_HSDPA_VERSION, OnUpdateHsdpaVersion)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntDoc construction/destruction

CMobileCnntDoc::CMobileCnntDoc()
{
	// TODO: add one-time construction code here
	for(int i =0; i < TB_NUM; i++)
	{
		m_uTbCheckFlag[i] = 0;
	}
	m_bDisDialUP = FALSE;
	m_bEnableDialUP = FALSE;
	m_bEnableCall = FALSE;
	m_bEnableMSinfo = FALSE;
	m_bEnableSMS = FALSE;
	m_bEnableConfig = FALSE;
	m_bEnableEngineer = FALSE;
	m_bEnablePoweroff = FALSE;
	m_bShowSMSWnd = FALSE;

}

CMobileCnntDoc::~CMobileCnntDoc()
{
	m_bShowSMSWnd = FALSE;
}

BOOL CMobileCnntDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;


	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMobileCnntDoc serialization

void CMobileCnntDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntDoc diagnostics

#ifdef _DEBUG
void CMobileCnntDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMobileCnntDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntDoc commands
void CMobileCnntDoc::OnLoadSMS()
{
	if(theApp.m_pSMSFrame == NULL)
	{
		theApp.CreateSMSFrame();
		m_bShowSMSWnd = TRUE;
	}
	else
	{
		if(m_bShowSMSWnd)
		{
			theApp.m_pSMSFrame->ShowWindow(SW_SHOW);
		}
		else
		{
			theApp.m_pSMSFrame->ShowWindow(SW_HIDE);
		}
	}
	m_bShowSMSWnd = !m_bShowSMSWnd;

	SetTBCheck(SMS_CHECK);
}

void CMobileCnntDoc::OnDialUp() 
{
	// TODO: Add your command handler code here
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->SwitchToForm(IDD_FORMVIEW_DIALUP);
	SetTBCheck(DIAL_CHECK);

	
}

void CMobileCnntDoc::OnUpdateDialUp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_uTbCheckFlag[DIAL_CHECK]);
	pCmdUI->Enable(m_bEnableDialUP);
}

void CMobileCnntDoc::OnLoadCall() 
{
	// TODO: Add your command handler code here
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->SwitchToForm(IDD_FORMVIEW_CALL);
	SetTBCheck(CALL_CHECK);
	
}

void CMobileCnntDoc::OnUpdateLoadCall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_uTbCheckFlag[CALL_CHECK]);
	pCmdUI->Enable(m_bEnableCall);

}

void CMobileCnntDoc::OnLoadConfig() 
{
	// TODO: Add your command handler code here
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->SwitchToForm(IDD_FORMVIEW_CONFIG);
	SetTBCheck(CONFIG_CHECK);

	
}

void CMobileCnntDoc::OnUpdateLoadConfig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_uTbCheckFlag[CONFIG_CHECK]);
	pCmdUI->Enable(m_bEnableConfig);
	
}

void CMobileCnntDoc::OnLoadEngeneering() 
{
	// TODO: Add your command handler code here
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->SwitchToForm(IDD_FORMVIEW_ENGINEERING);
	SetTBCheck(ENGINEERING_CHECK);

	
}

void CMobileCnntDoc::OnUpdateLoadEngeneering(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_uTbCheckFlag[ENGINEERING_CHECK]);
	pCmdUI->Enable(m_bEnableEngineer);
	
}

void CMobileCnntDoc::OnLoadHelp() 
{
	// TODO: Add your command handler code here
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->OnHelpInfo(NULL);
	
}

void CMobileCnntDoc::OnUpdateLoadHelp(CCmdUI* pCmdUI) 
{
	UNREFERENCED_PARAMETER(pCmdUI);

	// TODO: Add your command update UI handler code here
	
}

void CMobileCnntDoc::OnHome()
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->SwitchToForm(IDD_FORMVIEW_MSINFO);
	SetTBCheck(HOME_CHECK);
}

void CMobileCnntDoc::ChangeTBButton(UINT nOrigID, UINT nDestID)
{
    UINT nStyle;
    int  nOrigImage;
    int  nDestImage;

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();

    UINT nOrigIndex = pFrm->m_wndToolBar.CommandToIndex(nOrigID);
    UINT nDestIndex = pFrm->m_wndToolBar.CommandToIndex(nDestID);

    pFrm->m_wndToolBar.GetButtonInfo(nOrigIndex, nOrigID, nStyle, nOrigImage);
    pFrm->m_wndToolBar.GetButtonInfo(nDestIndex, nDestID, nStyle, nDestImage);

    pFrm->m_wndToolBar.SetButtonInfo(nOrigIndex, nDestID, TBBS_BUTTON, nDestImage);
    pFrm->m_wndToolBar.SetButtonInfo(nDestIndex, nOrigID, TBBS_BUTTON, nOrigImage);
    pFrm->m_wndToolBar.GetToolBarCtrl().HideButton(nOrigID);
}

void CMobileCnntDoc::OnUpdateLoadSms(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(theApp.m_pSMSFrame == NULL)
	{
		m_uTbCheckFlag[SMS_CHECK] = FALSE;		
	}	

	pCmdUI->SetCheck(m_uTbCheckFlag[SMS_CHECK]);
	pCmdUI->Enable(m_bEnableSMS);
	
}

void CMobileCnntDoc::OnUpdateHome(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_uTbCheckFlag[HOME_CHECK]);
	pCmdUI->Enable(m_bEnableMSinfo);
	
}

void CMobileCnntDoc::SetTBCheck(DWORD dwTBFlag)
{
	for(int i = 0; i < TB_NUM; i++)
	{
		if(i == (int)dwTBFlag)
		{
			m_uTbCheckFlag[i] = (m_uTbCheckFlag[i] == 0) ?1:0; //lint !e416 !e415
		}
		else
		{
			if(i != SMS_CHECK)
			{
				m_uTbCheckFlag[i] = 0;
			}
		}
	}
}

void CMobileCnntDoc::OnDisDialUp() 
{
	// TODO: Add your command handler code here
	CWaitCursor cursor;
	
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm == NULL)
	{
		return;
	}


	CDialUpView *pView = pFrm->m_pDialUPView;

	if(pView != NULL)
	{
		pView->Release();

		while(!theApp.m_bHangup)
		{
			Sleep(800);
		}

		if(pView->m_bHandCnnt)
		{
			pView->m_bHandCnnt = ! pView->m_bHandCnnt;
			pView->ChangeHandBtnString(pView->m_bHandCnnt);
		}

		pFrm->m_bDialCnnt = FALSE;
		::PostMessage(pFrm->m_hWnd, WM_DISCONNECT_DIAL_UP, NULL, NULL);
	}

//	theApp.HangUp();

	m_bDisDialUP = FALSE;

	if(!theApp.m_bMuxUsed)
	{
		pFrm->SwitchToForm(IDD_FORMVIEW_CONNECT);
		m_uTbCheckFlag[DIAL_CHECK] = 0;
		
		if(!theApp.m_bConnected)
		{
			if(pFrm->m_pCnntView != NULL)
			{
				pFrm->m_pCnntView->CnntOrDisCnntDevice();
			}
		}
	}
	
}

void CMobileCnntDoc::OnUpdateDisDialUp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bDisDialUP);
	
}

void CMobileCnntDoc::OnLoadAll()
{
	CWaitCursor cursor;

	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
		if(pFrm == NULL)
		{
			return;
		}
		pFrm->SetStatusBarState(PROMT_INFO_IND, IDS_INIT, RGB(0,0,0));
		
		char szRet[MAX_PATH] = {0};
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(PIN_OR_PUK, 0, NULL, 0);
		if(lpRet != NULL)
		{
			strcpy(szRet, lpRet);
		}
		
		if(strstr(szRet, "ERROR") == NULL)
		{
			CString strText = szRet;
			int nIndex = strText.Find(_T("+CPIN:"));
			if(nIndex != -1)
			{
				if(strText.Find(_T("SIM PIN")) != -1)
				{
					CDlgINputPin1 dlg;
					dlg.DoModal();
				}
				if(strText.Find(_T("SIM PUK")) != -1 && strText.Find(_T("SIM PUK2")) == -1)
				{
					CDlgInputPuk1 dlg;
					dlg.DoModal();
				}
//				if(strText.Find(_T("SIM PUK2")) != -1)
//				{
//					CDlgUnlockPin2 dlg;
//					dlg.DoModal();
//				}
			}
		}
		
		//teana hu 2009.05.06
		lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(CFUN_CHECK, 0, NULL, 0);
		if(lpRet == NULL)
		{
			ZeroMemory(szRet, MAX_PATH);
		}
		else
		{
			strcpy(szRet, lpRet);
		}
		
		if(strstr(szRet, "+CFUN:") != NULL)
		{
			CString strFun = szRet;
			int nNum = strFun.Find(_T("+CFUN:"));
			if(nNum != -1)
			{
				strFun.Delete(0, nNum + 7);
				nNum = strFun.Find(_T("\r\n"));
				if(nNum != -1)
				{
					strFun = strFun.Left(nNum);
					int nCmd = 0;
#ifdef _UNICODE
					swscanf(strFun, _T("%d"), &nCmd);
#else
					sscanf(strFun, _T("%d"), &nCmd);
#endif	
					if(nCmd != 1)
					{
						theApp.m_pEtionFactory->SendATCmd(CFUN_SET, 0, NULL, 0);
					}
				}
			}
		}
		
		lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(OPEN_CMEE, 0, NULL, 0);
		if(lpRet == NULL)
		{
			ZeroMemory(szRet, MAX_PATH);
		}
		else
		{
			strcpy(szRet, lpRet);
		}
		
		if(strstr(szRet, "ERROR") != NULL)
		{
			pFrm->SetStatusBarState(PROMT_INFO_IND, IDS_CMEE_FAILED, RGB(0,0,0));
		}
		lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(OPEN_AUTO_PROMT, 0, NULL, 0);
		if(lpRet == NULL)
		{
			ZeroMemory(szRet, MAX_PATH);
		}
		else
		{
			strcpy(szRet, lpRet);
		}
		
		if(strstr(szRet, "ERROR") != NULL)
		{
			pFrm->SetStatusBarState(PROMT_INFO_IND, IDS_SYS_PROMT_FAILED, RGB(0,0,0));
		}
		
		UpdateAllViews(NULL, WM_MSINFO_SET,NULL);

		if(theApp.m_pSMSFactory != NULL && theApp.m_bConnected)
		{

			theApp.m_pSMSFactory->RegisterIncomingSMS();
		}

		pFrm->SetStatusBarState(PROMT_INFO_IND, IDS_INIT_SUCCESS, RGB(0,0,0));
		
	}
}

void CMobileCnntDoc::OnUpdateLoadAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( theApp.m_bConnected );
}

void CMobileCnntDoc::OnPowerOff() 
{
	// TODO: Add your command handler code here
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(POWER_OFF, 0, NULL, 0);
		CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
		if(theApp.m_bConnected)
		{
			if(pFrm->m_pCnntView != NULL)
			{
				pFrm->m_pCnntView->CnntOrDisCnntDevice();
				::SendMessage(pFrm->m_hWnd, WM_CLOSE, NULL, NULL);
			}
		}
	}
	
}

void CMobileCnntDoc::OnUpdatePowerOff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bEnablePoweroff);
	
}

void CMobileCnntDoc::OnHsdpaVersion()
{
	CDlgHsdpaVersion dlg;
	dlg.DoModal();
}

void CMobileCnntDoc::OnUpdateHsdpaVersion(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(theApp.m_bConnected);
}

void CMobileCnntDoc::EnableAllItems(BOOL bEnable)
{
	m_bEnableDialUP = FALSE;
	m_bEnableCall = FALSE;
	if(m_bDisDialUP)
	{
		m_bEnableDialUP = TRUE;
	}
	m_bEnableSMS = FALSE;
#ifndef SPRD8800
	m_bEnableCall = bEnable;
	m_bEnableSMS = bEnable;
	m_bEnableDialUP = bEnable;
#endif
	m_bEnableMSinfo = bEnable;
	m_bEnableConfig = bEnable;
	m_bEnableEngineer = bEnable;
	m_bEnablePoweroff = bEnable;
}