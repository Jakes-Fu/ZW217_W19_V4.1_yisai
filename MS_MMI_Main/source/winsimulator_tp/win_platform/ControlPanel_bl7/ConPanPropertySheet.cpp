// ConPanPageSheet.cpp : implementation file
//

#include "stdafx.h"

#include "ConPanPropertySheet.h"
#include "SysInfoPropertyPage.h"
#include "SSConPropertyPage.h"
#include "CallConPropertyPage.h"
#include "SmsConPropertyPage.h"
#include "MsgFlowPropertyPage.h"
#include "CBConPropertyPage.h"
#include "GSensorProperyPage.h"
#include "ATestPropertyPage.h" //ATEST MODULE
#include "ConPanCfgAccess.h"   //ATEST MODULE
#include "ConPanLayer.h"
#include "ConPanFunc.h"
#include "ConPan.h"
#include "signals_header.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define CP_SIM_NUM  (MN_SYS_NUMBER)
#define CP_SIM_NUM  (4)
/////////////////////////////////////////////////////////////////////////////
// CConPanPropertySheet

//IMPLEMENT_DYNAMIC(CConPanPropertySheet, CPropertySheet)

CConPanPropertySheet::CConPanPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_pSysInfoPage	= NULL;
	m_pMsgFlowPage	= NULL;

	memset(m_pSSConPage,0,sizeof(CPropertyPagePtr)*MAX_SIM_NUM);
	memset(m_pCallConPage,0,sizeof(CPropertyPagePtr)*MAX_SIM_NUM);
	memset(m_pSmsConPage,0,sizeof(CPropertyPagePtr)*MAX_SIM_NUM);
	memset(m_pCBConPage,0,sizeof(CPropertyPagePtr)*MAX_SIM_NUM);
/*
	m_pSSConPage	= NULL;
	m_pCallConPage	= NULL;
	m_pSmsConPage	= NULL;
	m_pMsgFlowPage	= NULL;
	m_pCBConPage	= NULL;
    
    // For the second sim card
    m_pSSConPage2	= NULL;
    m_pCallConPage2	= NULL;
    m_pSmsConPage2	= NULL;
    m_pCBConPage2	= NULL;

	// For the third sim card
    m_pSSConPage3	= NULL;
    m_pCallConPage3	= NULL;
    m_pSmsConPage3	= NULL;
    m_pCBConPage3	= NULL;

	// For the fourth sim card
    m_pSSConPage4	= NULL;
    m_pCallConPage4	= NULL;
    m_pSmsConPage4	= NULL;
    m_pCBConPage4	= NULL;
*/	
	// GSensor
	m_pGSensorPage	= NULL;
    // ATest
    m_pATestPage    = NULL;
}

CConPanPropertySheet::CConPanPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pSysInfoPage	= NULL;
	m_pMsgFlowPage	= NULL;

	memset(m_pSSConPage,0,sizeof(CPropertyPagePtr)*MAX_SIM_NUM);
	memset(m_pCallConPage,0,sizeof(CPropertyPagePtr)*MAX_SIM_NUM);
	memset(m_pSmsConPage,0,sizeof(CPropertyPagePtr)*MAX_SIM_NUM);
	memset(m_pCBConPage,0,sizeof(CPropertyPagePtr)*MAX_SIM_NUM);

/* 
	m_pSSConPage	= NULL;
	m_pCallConPage	= NULL;
	m_pSmsConPage	= NULL;
	m_pMsgFlowPage	= NULL;
	m_pCBConPage	= NULL;


  
    // For the second sim card
    m_pSSConPage2	= NULL;
    m_pCallConPage2	= NULL;
    m_pSmsConPage2	= NULL;
    m_pCBConPage2	= NULL;

	// For the third sim card
    m_pSSConPage3	= NULL;
    m_pCallConPage3	= NULL;
    m_pSmsConPage3	= NULL;
    m_pCBConPage3	= NULL;
	
	// For the fourth sim card
    m_pSSConPage4	= NULL;
    m_pCallConPage4	= NULL;
    m_pSmsConPage4	= NULL;
    m_pCBConPage4	= NULL;
*/	
	// GSensor
	m_pGSensorPage	= NULL;
    // ATest
    m_pATestPage    = NULL;
}

CConPanPropertySheet::~CConPanPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CConPanPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CConPanPropertySheet)
	ON_WM_CONTEXTMENU()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(CONPAN_DATA, OnRcvCPData)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConPanPropertySheet message handlers


void CConPanPropertySheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class	
	CPropertySheet::PostNcDestroy();
	
}


BOOL CConPanPropertySheet::CreateConPan(CWnd *pParent)
{		
    
    m_pSysInfoPage  = new CSysInfoPropertyPage();
//	m_pMsgFlowPage  = new CMsgFlowPropertyPage();

	AddPage(m_pSysInfoPage);    

	for(int i = 0; i < CP_SIM_NUM ; i++)
	{
		m_pSSConPage[i]    = new CSSConPropertyPage();
		m_pCallConPage[i]  = new CCallConPropertyPage();		
		m_pSmsConPage[i]   = new CSmsConPropertyPage();   
		m_pCBConPage[i]    = new CCBConPropertyPage();

		((CSSConPropertyPage*)m_pSSConPage[i])->sys_id = i+1;
		((CCallConPropertyPage*)m_pCallConPage[i])->sys_id = i+1;
		((CSmsConPropertyPage*)m_pSmsConPage[i])->sys_id = i+1;
		((CCBConPropertyPage*)m_pCBConPage[i])->sys_id = i+1;			

		m_pSSConPage[i]->Construct(IDD_DIALOG_SSCON);
		m_strSSTitle[i].Format(_T("SS Control %d"),i+1);
		m_pSSConPage[i]->m_psp.dwFlags |= PSP_USETITLE;
		m_pSSConPage[i]->m_psp.pszTitle = m_strSSTitle[i].operator LPCTSTR();

		AddPage(m_pSSConPage[i]);
 	
 		m_pCallConPage[i]->Construct(IDD_DIALOG_CALLCON);
		m_strCallTitle[i].Format(_T("Call Control %d"),i+1);
		m_pCallConPage[i]->m_psp.dwFlags |= PSP_USETITLE;
		m_pCallConPage[i]->m_psp.pszTitle = m_strCallTitle[i].operator LPCTSTR();

		AddPage(m_pCallConPage[i]);

 		m_pSmsConPage[i]->Construct(IDD_DIALOG_SMSCON);
		m_strSmsTitle[i].Format(_T("Sms Control %d"),i+1);
		m_pSmsConPage[i]->m_psp.dwFlags |= PSP_USETITLE;
		m_pSmsConPage[i]->m_psp.pszTitle = m_strSmsTitle[i].operator LPCTSTR();

		AddPage(m_pSmsConPage[i]);

 		m_pCBConPage[i]->Construct(IDD_DIALOG_CBCON);
		m_strCBTitle[i].Format(_T("Cell BroadCast %d"),i+1);
		m_pCBConPage[i]->m_psp.dwFlags |= PSP_USETITLE;
		m_pCBConPage[i]->m_psp.pszTitle = m_strCBTitle[i].operator LPCTSTR();
		
		AddPage(m_pCBConPage[i]);


	}

/*
 
	((CSSConPropertyPage*)m_pSSConPage)->sys_id = 1;
	((CCallConPropertyPage*)m_pCallConPage)->sys_id = 1;
	((CSmsConPropertyPage*)m_pSmsConPage)->sys_id = 1;
	((CCBConPropertyPage*)m_pCBConPage)->sys_id = 1;
	
    m_pSSConPage2   = new CSSConPropertyPage();
    m_pCallConPage2 = new CCallConPropertyPage();		
    m_pSmsConPage2  = new CSmsConPropertyPage();		
    m_pCBConPage2   = new CCBConPropertyPage();
    
	((CSSConPropertyPage*)m_pSSConPage2)->sys_id = 2;
	((CCallConPropertyPage*)m_pCallConPage2)->sys_id = 2;
	((CSmsConPropertyPage*)m_pSmsConPage2)->sys_id = 2;
	((CCBConPropertyPage*)m_pCBConPage2)->sys_id = 2;
	
    m_pSSConPage2->Construct( IDD_DIALOG_SSCON,IDS_SSCON2_TITLE );
    m_pCallConPage2->Construct( IDD_DIALOG_CALLCON,IDS_CALLCON2_TITLE );
    m_pSmsConPage2->Construct( IDD_DIALOG_SMSCON,IDS_SMSCON2_TITLE );
    m_pCBConPage2->Construct( IDD_DIALOG_CBCON,IDS_CELLBROADCAST2_TITLE );

	m_pSSConPage3   = new CSSConPropertyPage();
    m_pCallConPage3 = new CCallConPropertyPage();		
    m_pSmsConPage3  = new CSmsConPropertyPage();		
    m_pCBConPage3   = new CCBConPropertyPage();
    
	((CSSConPropertyPage*)m_pSSConPage3)->sys_id = 3;
	((CCallConPropertyPage*)m_pCallConPage3)->sys_id = 3;
	((CSmsConPropertyPage*)m_pSmsConPage3)->sys_id = 3;
	((CCBConPropertyPage*)m_pCBConPage3)->sys_id = 3;
	
    m_pSSConPage3->Construct( IDD_DIALOG_SSCON,IDS_SSCON3_TITLE );
    m_pCallConPage3->Construct( IDD_DIALOG_CALLCON,IDS_CALLCON3_TITLE );
    m_pSmsConPage3->Construct( IDD_DIALOG_SMSCON,IDS_SMSCON3_TITLE );
    m_pCBConPage3->Construct( IDD_DIALOG_CBCON,IDS_CELLBROADCAST3_TITLE );

	m_pSSConPage4   = new CSSConPropertyPage();
    m_pCallConPage4 = new CCallConPropertyPage();		
    m_pSmsConPage4  = new CSmsConPropertyPage();		
    m_pCBConPage4   = new CCBConPropertyPage();
    
	((CSSConPropertyPage*)m_pSSConPage4)->sys_id = 4;
	((CCallConPropertyPage*)m_pCallConPage4)->sys_id = 4;
	((CSmsConPropertyPage*)m_pSmsConPage4)->sys_id = 4;
	((CCBConPropertyPage*)m_pCBConPage4)->sys_id = 4;
	
    m_pSSConPage4->Construct( IDD_DIALOG_SSCON,IDS_SSCON4_TITLE );
    m_pCallConPage4->Construct( IDD_DIALOG_CALLCON,IDS_CALLCON4_TITLE );
    m_pSmsConPage4->Construct( IDD_DIALOG_SMSCON,IDS_SMSCON4_TITLE );
    m_pCBConPage4->Construct( IDD_DIALOG_CBCON,IDS_CELLBROADCAST4_TITLE );
*/
	m_pGSensorPage = new CGSensorProperyPage();

    if(CConPanCfgAccess::GetATestControlFlag())
    {
        m_pATestPage    = new CATestPropertyPage();        // ATEST MODULE
        ((CATestPropertyPage*)m_pATestPage)->sys_id = 1;   // ATEST MODULE
        AddPage(m_pATestPage);                             // ATEST MODULE
    }
 /*   
    ASSERT(m_pSysInfoPage);    
    ASSERT(m_pSSConPage);    
    ASSERT(m_pCallConPage);    
    ASSERT(m_pSmsConPage);    
    ASSERT(m_pMsgFlowPage);    
    ASSERT(m_pCBConPage);    
 */   
 //   AddPage(m_pSysInfoPage);    
 /*   AddPage(m_pCallConPage);    
    AddPage(m_pSmsConPage);    
    AddPage(m_pCBConPage); 
    AddPage(m_pSSConPage);    
//    AddPage(m_pMsgFlowPage); 

    AddPage(m_pCallConPage2);    
    AddPage(m_pSmsConPage2);    
    AddPage(m_pCBConPage2); 
    AddPage(m_pSSConPage2);    

	AddPage(m_pCallConPage3);    
    AddPage(m_pSmsConPage3);    
    AddPage(m_pCBConPage3); 
    AddPage(m_pSSConPage3);   


	AddPage(m_pCallConPage4);    
    AddPage(m_pSmsConPage4);    
    AddPage(m_pCBConPage4); 
    AddPage(m_pSSConPage4);   
*/
// 	for(i = 0; i < CP_SIM_NUM ; i++)
// 	{
// 		AddPage(m_pCallConPage[i]);    
// 		AddPage(m_pSmsConPage[i]);    
// 		AddPage(m_pCBConPage[i]); 
// 		AddPage(m_pSSConPage[i]); 	
// 	}
// 	
	
	AddPage(m_pGSensorPage);
    




    //default style for CPropertySheet.
    DWORD dwSheetStyle = WS_SYSMENU | 
                          WS_POPUP | 
                          WS_CAPTION | 
                          DS_MODALFRAME | 
                          DS_CONTEXTHELP | 
                          WS_VISIBLE;   
    dwSheetStyle &= ~WS_VISIBLE;  //make sheet invisiable 

    Create(pParent, dwSheetStyle);

    //set the page to show.
    SetActivePage(m_pSysInfoPage);    

    CMenu *pmenu=GetSystemMenu(FALSE);
    pmenu->DeleteMenu(SC_CLOSE,MF_BYCOMMAND);

    if(CConPanCfgAccess::GetATestControlFlag())
    {
        if(CConPanCfgAccess::GetCMDAutoRunFlag())
        {
            SetTimer(1,15*1000,NULL);  // set the timer of the auto testing started by command line.
        }
    }
    
    return TRUE;
}

void CConPanPropertySheet::ReleaseConPan()
{
	if (m_pSysInfoPage)
	{
		delete m_pSysInfoPage;
		m_pSysInfoPage = NULL;
	}
	if (m_pMsgFlowPage)
	{
		delete m_pMsgFlowPage;
		m_pMsgFlowPage = NULL;
	}

/*    if (m_pSSConPage)
	{
		delete m_pSSConPage;
		m_pSSConPage = NULL;
	}
	if (m_pCallConPage)
	{
		delete m_pCallConPage;
		m_pCallConPage = NULL;
	}
	if (m_pSmsConPage)
	{
		delete m_pSmsConPage;
		m_pSmsConPage = NULL;
	}

    if (m_pCBConPage)
    {
        delete m_pCBConPage;
        m_pCBConPage = NULL;
    }
  
    if (m_pCBConPage2)
    {
        delete m_pCBConPage2;
        m_pCBConPage2 = NULL;
    }
    
    if (m_pSSConPage2)
    {
        delete m_pSSConPage2;
        m_pSSConPage2 = NULL;
    }
    if (m_pCallConPage2)
    {
        delete m_pCallConPage2;
        m_pCallConPage2 = NULL;
    }
    if (m_pSmsConPage2)
    {
        delete m_pSmsConPage2;
        m_pSmsConPage2 = NULL;
    }
    if (m_pCBConPage2)
    {
        delete m_pCBConPage2;
        m_pCBConPage2 = NULL;
    }
    
    if (m_pSSConPage3)
    {
        delete m_pSSConPage3;
        m_pSSConPage3 = NULL;
    }
    if (m_pCallConPage3)
    {
        delete m_pCallConPage3;
        m_pCallConPage3 = NULL;
    }
    if (m_pSmsConPage3)
    {
        delete m_pSmsConPage3;
        m_pSmsConPage3 = NULL;
    }
    if (m_pCBConPage3)
    {
        delete m_pCBConPage3;
        m_pCBConPage3 = NULL;
    }


	if (m_pSSConPage4)
    {
        delete m_pSSConPage4;
        m_pSSConPage4 = NULL;
    }
    if (m_pCallConPage4)
    {
        delete m_pCallConPage4;
        m_pCallConPage4 = NULL;
    }
    if (m_pSmsConPage4)
    {
        delete m_pSmsConPage4;
        m_pSmsConPage4 = NULL;
    }
    if (m_pCBConPage4)
    {
        delete m_pCBConPage4;
        m_pCBConPage4 = NULL;
    }
*/
	for(int i = 0; i < CP_SIM_NUM ; i++)
	{
		if(m_pCallConPage[i])
		{
			delete m_pCallConPage[i];
			m_pCallConPage[i] = NULL;
		}
		if(m_pSmsConPage[i])
		{
			delete m_pSmsConPage[i];
			m_pSmsConPage[i] = NULL;
		}
		if(m_pCBConPage[i])
		{
			delete m_pCBConPage[i];
			m_pCBConPage[i] = NULL;
		}
		if(m_pSSConPage[i])
		{
			delete m_pSSConPage[i];
			m_pSSConPage[i] = NULL;
		}
	}

	if(m_pGSensorPage)
	{

		delete m_pGSensorPage;
		m_pGSensorPage = NULL;
	}
    if (m_pATestPage)  // ATEST MODULE
    {
        delete m_pATestPage;
        m_pATestPage = NULL;
    }
}
LRESULT CConPanPropertySheet::OnRcvCPData(WPARAM wParam,LPARAM lParam)
{
	DealCPMsg((void *)wParam);
	return 0;
}
void CConPanPropertySheet::DealCPMsg(void *pmsg)
{
	ASSERT(pmsg);
	
    CPropertyPage *pCurPage = GetActivePage();
    DSSignalHeader *psig = (DSSignalHeader *) pmsg;

	if(psig->dual_sys < CP_SIM_NUM)
	{
		int nSysIDIndx = psig->dual_sys;
		if (psig->SignalCode == MN_SETUP_REQ ||
			psig->SignalCode == MN_SETUP_RSP ||
			psig->SignalCode == MN_DISC_REQ  ||
			psig->SignalCode == MN_HOLD_REQ  ||
			psig->SignalCode == MN_RETRIEVE_REQ ||
			psig->SignalCode == MN_CALL_SS_REQ 
	#ifdef MN_START_DTMF_REQ
			|| psig->SignalCode == MN_START_DTMF_REQ
	#endif			
			)
		{	
			if (IsAutoPage() && pCurPage != m_pCallConPage[nSysIDIndx])
			{
				SetActivePage(m_pCallConPage[nSysIDIndx]);
			}
			((CCallConPropertyPage *)m_pCallConPage[nSysIDIndx])->DealCPMsg(psig);		
		}
		else if ((psig->SignalCode == MN_SMS_REQ) 
			|| (psig->SignalCode == MN_RL_REPORT_REQ)
			|| (psig->SignalCode == MN_SM_RL_MEM_AVAIL_REQ)
            || (psig->SignalCode == ATEST_CP_SEND_PDU_IND))
		{
			if (IsAutoPage() && pCurPage != m_pSmsConPage[nSysIDIndx])
			{
				SetActivePage(m_pSmsConPage[nSysIDIndx]);
			}
			((CSmsConPropertyPage *)m_pSmsConPage[nSysIDIndx])->DealCPMsg(psig);				
		}
		else if (psig->SignalCode == MN_CAMP_ON_REQ || 
			psig->SignalCode == MN_REG_REQ ||
			psig->SignalCode == MN_NREG_REQ)
		{
			if (psig->SignalCode == MN_NREG_REQ)
			{
				((CCallConPropertyPage *)m_pCallConPage[nSysIDIndx])->OnPowerOff();				
			}
			if (IsAutoPage() && pCurPage != m_pSysInfoPage)
			{
				SetActivePage(m_pSysInfoPage);
			}		
			((CSysInfoPropertyPage *)m_pSysInfoPage)->DealCPMsg(psig);				
		}
		else if (psig->SignalCode == MN_REGISTER_SS_REQ || 
			psig->SignalCode == MN_ERASE_SS_REQ || 
			psig->SignalCode == MN_ACTIVATE_SS_REQ || 
			psig->SignalCode == MN_DEACTIVATE_SS_REQ ||
			psig->SignalCode == MN_INTERROGATE_SS_REQ ||
			psig->SignalCode == MN_MODIFY_PASSWARD_REQ ||
			psig->SignalCode == MN_PASSWARD_RSP ||
			psig->SignalCode == MN_PROC_USSD_REQ ||
			psig->SignalCode == MN_USSD_RSP)
		{
			
			if (IsAutoPage() && pCurPage != m_pSSConPage[nSysIDIndx])
			{
				SetActivePage(m_pSSConPage[nSysIDIndx]);
			}			
			((CSSConPropertyPage *)m_pSSConPage[nSysIDIndx])->DealCPMsg(psig);				
		}
        else if(psig->SignalCode == ATEST_CPANEL_REQ)  // ATEST MODULE - added by Mingxuan.Zhu
        {
            if(CConPanCfgAccess::GetATestControlFlag())
            {
                if (IsAutoPage() && pCurPage != m_pATestPage)
                {
                    SetActivePage(m_pATestPage);
                }			
                ((CATestPropertyPage *)m_pATestPage)->DealCPMsg(psig);
            }
        }
		else
		{
			TRACE("Control Panel received a message with unknown type.");
		}
	}

/*
	if (MN_DUAL_SYS_1 == psig->dual_sys)
	{
		if (psig->SignalCode == MN_SETUP_REQ ||
			psig->SignalCode == MN_SETUP_RSP ||
			psig->SignalCode == MN_DISC_REQ  ||
			psig->SignalCode == MN_HOLD_REQ  ||
			psig->SignalCode == MN_RETRIEVE_REQ ||
			psig->SignalCode == MN_CALL_SS_REQ 
#ifdef MN_START_DTMF_REQ
			|| psig->SignalCode == MN_START_DTMF_REQ
#endif			
			)
		{	
			if (IsAutoPage() && pCurPage != m_pCallConPage)
			{
				SetActivePage(m_pCallConPage);
			}
			((CCallConPropertyPage *)m_pCallConPage)->DealCPMsg(psig);		
		}
		else if ((psig->SignalCode == MN_SMS_REQ) 
			|| (psig->SignalCode == MN_RL_REPORT_REQ)
			|| (psig->SignalCode == MN_SM_RL_MEM_AVAIL_REQ))
		{
			if (IsAutoPage() && pCurPage != m_pSmsConPage)
			{
				SetActivePage(m_pSmsConPage);
			}
			((CSmsConPropertyPage *)m_pSmsConPage)->DealCPMsg(psig);				
		}
		else if (psig->SignalCode == MN_CAMP_ON_REQ || 
				 psig->SignalCode == MN_REG_REQ ||
				 psig->SignalCode == MN_NREG_REQ)
		{
			if (psig->SignalCode == MN_NREG_REQ)
			{
				((CCallConPropertyPage *)m_pCallConPage)->OnPowerOff();				
			}
			 if (IsAutoPage() && pCurPage != m_pSysInfoPage)
			{
				SetActivePage(m_pSysInfoPage);
			}		
			((CSysInfoPropertyPage *)m_pSysInfoPage)->DealCPMsg(psig);				
		}
		else if (psig->SignalCode == MN_REGISTER_SS_REQ || 
				 psig->SignalCode == MN_ERASE_SS_REQ || 
				 psig->SignalCode == MN_ACTIVATE_SS_REQ || 
				 psig->SignalCode == MN_DEACTIVATE_SS_REQ ||
				 psig->SignalCode == MN_INTERROGATE_SS_REQ ||
				 psig->SignalCode == MN_MODIFY_PASSWARD_REQ ||
				 psig->SignalCode == MN_PASSWARD_RSP ||
				 psig->SignalCode == MN_PROC_USSD_REQ ||
				 psig->SignalCode == MN_USSD_RSP)
		{

			if (IsAutoPage() && pCurPage != m_pSSConPage)
			{
				SetActivePage(m_pSSConPage);
			}			
			((CSSConPropertyPage *)m_pSSConPage)->DealCPMsg(psig);				
		}
		else
		{
			TRACE("Control Panel received a message with unknown type.");
		}
	}
	else if (MN_DUAL_SYS_2 == psig->dual_sys)
	{
		if (psig->SignalCode == MN_SETUP_REQ ||
			psig->SignalCode == MN_SETUP_RSP ||
			psig->SignalCode == MN_DISC_REQ  ||
			psig->SignalCode == MN_HOLD_REQ  ||
			psig->SignalCode == MN_RETRIEVE_REQ ||
			psig->SignalCode == MN_CALL_SS_REQ
#ifdef MN_START_DTMF_REQ
			|| psig->SignalCode == MN_START_DTMF_REQ
#endif	
			)
		{	
			if (IsAutoPage() && pCurPage != m_pCallConPage2)
			{
				SetActivePage(m_pCallConPage2);
			}
			((CCallConPropertyPage *)m_pCallConPage2)->DealCPMsg(psig);		
		}
		else if ((psig->SignalCode == MN_SMS_REQ) 
			|| (psig->SignalCode == MN_RL_REPORT_REQ)
			|| (psig->SignalCode == MN_SM_RL_MEM_AVAIL_REQ))
		{
			if (IsAutoPage() && pCurPage != m_pSmsConPage2)
			{
				SetActivePage(m_pSmsConPage2);
			}
			((CSmsConPropertyPage *)m_pSmsConPage2)->DealCPMsg(psig);				
		}
		else if (psig->SignalCode == MN_CAMP_ON_REQ || 
				 psig->SignalCode == MN_REG_REQ ||
				 psig->SignalCode == MN_NREG_REQ)
		{
			if (psig->SignalCode == MN_NREG_REQ)
			{
				((CCallConPropertyPage *)m_pCallConPage2)->OnPowerOff();				
			}
			 if (IsAutoPage() && pCurPage != m_pSysInfoPage)
			{
				SetActivePage(m_pSysInfoPage);
			}		
			((CSysInfoPropertyPage *)m_pSysInfoPage)->DealCPMsg(psig);				
		}
		else if (psig->SignalCode == MN_REGISTER_SS_REQ || 
				 psig->SignalCode == MN_ERASE_SS_REQ || 
				 psig->SignalCode == MN_ACTIVATE_SS_REQ || 
				 psig->SignalCode == MN_DEACTIVATE_SS_REQ ||
				 psig->SignalCode == MN_INTERROGATE_SS_REQ ||
				 psig->SignalCode == MN_MODIFY_PASSWARD_REQ ||
				 psig->SignalCode == MN_PASSWARD_RSP ||
				 psig->SignalCode == MN_PROC_USSD_REQ ||
				 psig->SignalCode == MN_USSD_RSP)
		{

			if (IsAutoPage() && pCurPage != m_pSSConPage2)
			{
				SetActivePage(m_pSSConPage2);
			}			
			((CSSConPropertyPage *)m_pSSConPage2)->DealCPMsg(psig);				
		}
		else
		{
			TRACE("Control Panel received a message with unknown type.");
		}
	}
	else if(MN_DUAL_SYS_3 == psig->dual_sys)
	{
		if (psig->SignalCode == MN_SETUP_REQ ||
			psig->SignalCode == MN_SETUP_RSP ||
			psig->SignalCode == MN_DISC_REQ  ||
			psig->SignalCode == MN_HOLD_REQ  ||
			psig->SignalCode == MN_RETRIEVE_REQ ||
			psig->SignalCode == MN_CALL_SS_REQ
#ifdef MN_START_DTMF_REQ
			|| psig->SignalCode == MN_START_DTMF_REQ
#endif	
			)

		{	
			if (IsAutoPage() && pCurPage != m_pCallConPage3)
			{
				SetActivePage(m_pCallConPage3);
			}
			((CCallConPropertyPage *)m_pCallConPage3)->DealCPMsg(psig);		
		}
		else if ((psig->SignalCode == MN_SMS_REQ) 
			|| (psig->SignalCode == MN_RL_REPORT_REQ)
			|| (psig->SignalCode == MN_SM_RL_MEM_AVAIL_REQ))
		{
			if (IsAutoPage() && pCurPage != m_pSmsConPage3)
			{
				SetActivePage(m_pSmsConPage3);
			}
			((CSmsConPropertyPage *)m_pSmsConPage3)->DealCPMsg(psig);				
		}
		else if (psig->SignalCode == MN_CAMP_ON_REQ || 
			psig->SignalCode == MN_REG_REQ ||
			psig->SignalCode == MN_NREG_REQ)
		{
			if (psig->SignalCode == MN_NREG_REQ)
			{
				((CCallConPropertyPage *)m_pCallConPage3)->OnPowerOff();				
			}
			if (IsAutoPage() && pCurPage != m_pSysInfoPage)
			{
				SetActivePage(m_pSysInfoPage);
			}		
			((CSysInfoPropertyPage *)m_pSysInfoPage)->DealCPMsg(psig);				
		}
		else if (psig->SignalCode == MN_REGISTER_SS_REQ || 
			psig->SignalCode == MN_ERASE_SS_REQ || 
			psig->SignalCode == MN_ACTIVATE_SS_REQ || 
			psig->SignalCode == MN_DEACTIVATE_SS_REQ ||
			psig->SignalCode == MN_INTERROGATE_SS_REQ ||
			psig->SignalCode == MN_MODIFY_PASSWARD_REQ ||
			psig->SignalCode == MN_PASSWARD_RSP ||
			psig->SignalCode == MN_PROC_USSD_REQ ||
			psig->SignalCode == MN_USSD_RSP)
		{
			
			if (IsAutoPage() && pCurPage != m_pSSConPage3)
			{
				SetActivePage(m_pSSConPage3);
			}			
			((CSSConPropertyPage *)m_pSSConPage3)->DealCPMsg(psig);				
		}
		else
		{
			TRACE("Control Panel received a message with unknown type.");
		}
	}
	*/
	xSignalHeaderRec *pfreemsg = (xSignalHeaderRec *) pmsg;
	InnerSCIFreeSignal(pfreemsg);
    
    return;
		
}

BOOL CConPanPropertySheet::IsPowerOn(UINT dual_sys)
{
	if (dual_sys == 1)
	{
		return ((CSysInfoPropertyPage *)m_pSysInfoPage)->m_SysInfoData.m_nPowerStatus;
	}
	else if (dual_sys == 2)
	{
		return ((CSysInfoPropertyPage *)m_pSysInfoPage)->m_SysInfoData.m_nPowerStatus2;
	}
	else if (dual_sys == 3)
	{
		return ((CSysInfoPropertyPage *)m_pSysInfoPage)->m_SysInfoData.m_nPowerStatus3;
	}
	else if (dual_sys == 4)
	{
		return ((CSysInfoPropertyPage *)m_pSysInfoPage)->m_SysInfoData.m_nPowerStatus4;
	}
	else
	{
		return false;
	}
}

BOOL CConPanPropertySheet::IsServiceAvailable(UINT dual_sys)
{
   if (dual_sys == 1)
   {
	   return (((CSysInfoPropertyPage *)m_pSysInfoPage)->m_SysInfoData.m_nPLMNSvrState == 1);
   }
   else if (dual_sys == 2)
   {
	   return (((CSysInfoPropertyPage *)m_pSysInfoPage)->m_SysInfoData.m_nPLMNSvrState2 == 1);
   }
   else if (dual_sys == 3)
   {
	   return (((CSysInfoPropertyPage *)m_pSysInfoPage)->m_SysInfoData.m_nPLMNSvrState3 == 1);
   }
   else if (dual_sys == 4)
   {
	   return (((CSysInfoPropertyPage *)m_pSysInfoPage)->m_SysInfoData.m_nPLMNSvrState4 == 1);
   }
   else
   {
	   return false;
   }
}

BOOL CConPanPropertySheet::IsAutoPage()
{
	return ((CSysInfoPropertyPage *)m_pSysInfoPage)->m_bAutoPage;

}

/**************************************************************************************/
// Description      : invoke the corresponding atest module by timer's ID 
//--------------------------------------------------------------------------------------
// Input Parameter  : UINT nIDEvent  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CConPanPropertySheet::OnTimer(UINT nIDEvent) 
{
    int sys_id = 1;  // SIM1
    CPropertyPage *pCurPage = GetActivePage();
    
    if (!IsPowerOn(sys_id))
    {
        MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
        KillTimer(nIDEvent);
        return ;
    }
    
    if (IsAutoPage() && pCurPage != m_pATestPage)
    {
        SetActivePage(m_pATestPage);
    }
    
    if( nIDEvent == 1 ) // ATEST Auto Testing
    {
        KillTimer(nIDEvent);
        ((CATestPropertyPage*)m_pATestPage)->OnButtonComboRun();
    }
    
    CPropertySheet::OnTimer(nIDEvent);
}
