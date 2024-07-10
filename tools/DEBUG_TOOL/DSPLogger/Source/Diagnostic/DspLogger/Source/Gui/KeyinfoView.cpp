// KeyinfoView.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "KeyinfoView.h"
#include "LUA_FUNC.h"
#include "MainFrm.h"
#include "KeyInfoDoc.h"
#include "FindDoc.h"
#include "DspLoggerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MAX_GSM_INFO_NUM 60
#define MAX_TD_INFO_NUM 60
#define BK_COLOR RGB(228,228,228)

#define ITEM_TEXT_COLOR RGB(155,19,19)
#define ITEM_BK_COLOR RGB(200,200,200)

#define EVENT_GET_DATA 0x6767
/////////////////////////////////////////////////////////////////////////////
// CKeyinfoView

IMPLEMENT_DYNCREATE(CKeyinfoView, CFormView)

CKeyinfoView::CKeyinfoView()
	: CFormView(CKeyinfoView::IDD)
{
	//{{AFX_DATA_INIT(CKeyinfoView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_wAddrCD = 0;
	m_wAddrCIRP = 0;
	m_nTPCount = 0;
}

CKeyinfoView::~CKeyinfoView()
{
//	KillTimer(EVENT_GET_DATA);
	m_vecKeyInfoGSM.clear();
	m_vecKeyInfoTD.clear();
	m_vecKey.clear();
	m_vecItemGSM.clear();
	m_vecItemTD.clear();

	m_brshBckGrnd.DeleteObject();
}

void CKeyinfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyinfoView)
	DDX_Control(pDX, IDC_LIST_GSM_INFO, m_lstGSM);
	DDX_Control(pDX, IDC_LIST_TD_INFO, m_lstTD);
	DDX_Control(pDX, IDC_STC_CONS_DIAG, m_stcConsDiag);
	DDX_Control(pDX, IDC_STC_CHANNEL_IMPULE, m_stcChannelImpule);
	DDX_Text(pDX, IDC_STC_CONST, m_strConstLine);
	DDX_Text(pDX, IDC_STC_IMPULSE, m_strImpulseLine);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyinfoView, CFormView)
	//{{AFX_MSG_MAP(CKeyinfoView)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CONST, OnBtnConst)
	ON_BN_CLICKED(IDC_BTN_IMPULSE, OnBtnImpulse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyinfoView diagnostics

#ifdef _DEBUG
void CKeyinfoView::AssertValid() const
{
	CFormView::AssertValid();
}

void CKeyinfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKeyinfoView message handlers

void CKeyinfoView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_vecKeyInfoGSM.clear();
	m_vecKeyInfoTD.clear();
	m_vecKey.clear();
	m_vecItemGSM.clear();
	m_vecItemTD.clear();

	VERIFY( m_brshBckGrnd.CreateSolidBrush(BK_COLOR) );

	DWORD dwStyle = m_lstGSM.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_lstGSM.SetExtendedStyle(dwStyle);
    m_lstGSM.InsertColumn(0, _T(""), LVCFMT_LEFT, 130);
	m_lstGSM.InsertColumn(1, _T(""), LVCFMT_LEFT, 100);

	dwStyle = m_lstTD.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_lstTD.SetExtendedStyle(dwStyle);
    m_lstTD.InsertColumn(0, _T(""), LVCFMT_LEFT, 130);
	m_lstTD.InsertColumn(1, _T(""), LVCFMT_LEFT, 100);

	//get gsm item
	lua_item("gsm_item", &m_vecItemGSM);

	//get td item
	lua_item("td_item", &m_vecItemTD);

	//gsm table

	char szItem[50] = {0};
	int nNum = m_vecItemGSM.size();
	int nStartItem = 0;
	for(int i = 0; i < nNum; i++)
	{
		_tcscpy(szItem, m_vecItemGSM.at(i));
		LPSTR lpFind = strstr(szItem, "_");
		if(lpFind != NULL)
		{
			lpFind = lpFind + 1;
		}
		m_lstGSM.InsertItem(nStartItem, lpFind);
		m_lstGSM.SetItemColors(nStartItem,ITEM_TEXT_COLOR,ITEM_BK_COLOR);
		nStartItem ++;
		m_vecKeyInfoGSM.push_back(0xAAAA);
		lua_table(m_vecItemGSM.at(i), &m_vecKey, &m_vecKeyInfoGSM);	
		int nsize = m_vecKey.size();
		for(int j =0; j < nsize; j++)
		{
			m_lstGSM.InsertItem(j+nStartItem, m_vecKey.at(j));
		}
		m_vecKey.clear(); 
		nStartItem += j;
	}

	//td table
	nNum = m_vecItemTD.size();
	nStartItem = 0;
	for(i = 0; i < nNum; i++)
	{
		_tcscpy(szItem, m_vecItemTD.at(i));
		LPSTR lpFind = strstr(szItem, "_");
		if(lpFind != NULL)
		{
			lpFind = lpFind + 1;
		}
		m_lstTD.InsertItem(nStartItem, lpFind);
		m_lstTD.SetItemColors(nStartItem,ITEM_TEXT_COLOR,ITEM_BK_COLOR);
		nStartItem ++;
		m_vecKeyInfoTD.push_back(0xAAAA);
		lua_table(m_vecItemTD.at(i), &m_vecKey, &m_vecKeyInfoTD);	
		int nsize = m_vecKey.size();
		for(int j =0; j < nsize; j++)
		{
			m_lstTD.InsertItem(j+nStartItem, m_vecKey.at(j));
		}
		m_vecKey.clear(); 
		nStartItem += j;
	}

	int	new_data = 0;
	lua_chart("Constellation Diagram",&new_data);
	m_wAddrCD=(WORD)new_data;
	lua_chart("Channel Impulse Response Power",&new_data);
	m_wAddrCIRP=(WORD)new_data;


	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm != NULL &&  pFrm->m_bConnected)
	{
		m_nTPCount = pFrm->m_pDspKernel->GetTpSet()->GetTpCount();
		SetTimer(EVENT_GET_DATA, 200, 0);
	}
}

HBRUSH CKeyinfoView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshBckGrnd.m_hObject);
}

void CKeyinfoView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == EVENT_GET_DATA)
	{
		CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
		if(pFrm == NULL)
		{
			return;
		}

		static BOOL bCD = FALSE;
		static BOOL bCIRP = FALSE;
		ITPSet *pTPSet = pFrm->m_pDspKernel->GetTpSet();
		int nCount = pTPSet->GetTpCount();
		//int nNewCount = nCount > 200 ? 200 : nCount;

		if(nCount > m_nTPCount)
		{
			for(int i = m_nTPCount; i < nCount; i++)
			{
				WORD wAddr = pTPSet->GetTPAddr(i);
				WORD wData = pTPSet->GetTPData(i);
				//for gsm table
				int nSize = m_vecKeyInfoGSM.size();
				for(int j =0 ; j < nSize; j++)
				{
					if(m_vecKeyInfoGSM.at(j) == wAddr)
					{
						if(bCD)
						{
							m_stcConsDiag.Invalidate(FALSE);
						}
						if(bCIRP)
						{
							m_stcChannelImpule.Invalidate(FALSE);
						}
						bCD = FALSE;
 						bCIRP = FALSE;
						char szRet[256] = {0};
						char* str_tmp=(char *)malloc(256);
						char sz_text[50] = {0};
						m_lstGSM.GetItemText(j, 0, sz_text, 50);
						lua_keyinfo((int)wAddr, (int)wData, sz_text, &str_tmp);
						sprintf(szRet,"%s",str_tmp);
						free(str_tmp);
						if(szRet[0] != '\0')
						{
							m_lstGSM.SetItemText(j, 1, szRet);
						}						
					}
				}
				
				//for td table
				nSize = m_vecKeyInfoTD.size();
				for(j =0 ; j < nSize; j++)
				{
					if(m_vecKeyInfoTD.at(j) == wAddr)
					{
						if(bCD)
						{
							m_stcConsDiag.Invalidate(FALSE);
						}
						if(bCIRP)
						{
							m_stcChannelImpule.Invalidate(FALSE);
						}
						bCD = FALSE;
						bCIRP = FALSE;
						char szRet[256] = {0};
						char* str_tmp=(char *)malloc(256);
						char sz_text[50] = {0};
						m_lstTD.GetItemText(j, 0, sz_text, 50);
						lua_keyinfo((int)wAddr, (int)wData, sz_text, &str_tmp);
						sprintf(szRet,"%s",str_tmp);
						free(str_tmp);
						if(szRet[0] != '\0')
						{
							m_lstTD.SetItemText(j, 1, szRet);
						}
					}
				}

				if(wAddr == m_wAddrCD)
				{
					if(!bCD)
					{
						m_strConstLine.Format("line: %d", i);
						UpdateData(FALSE);
					}
					m_stcConsDiag.AddData(wData, !bCD);
					//m_stcConsDiag.Invalidate(FALSE);
					bCD = TRUE;
					bCIRP = FALSE;
				}
				else if(wAddr == m_wAddrCIRP)
				{
					if(!bCIRP)
					{
						m_strImpulseLine.Format("line: %d", i);
						UpdateData(FALSE);
					}
					m_stcChannelImpule.AddData(wData, !bCIRP);
					//m_stcChannelImpule.Invalidate(FALSE);
					bCIRP = TRUE;
					bCD = FALSE;
				}
			}

			m_nTPCount = nCount;
		}
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CKeyinfoView::Clear()
{
	m_nTPCount = 0;
	KillTimer(EVENT_GET_DATA);
}

void CKeyinfoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(lHint == WM_UPDATE_CLEAR)
	{
		Clear();
	}
	else if(lHint == WM_UPDATE_SETTIMER)
	{
		SetTimer(EVENT_GET_DATA, 200, 0);
	}
	else if(WM_FIND_ITEM_IN_DOCVIEW && pHint != NULL)
	{
		CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
		if(pFrm == NULL)
		{
			return;
		}
		
		if(!pFrm->m_bPlaybackTP)
		{
			return;
		}
		
		BOOL bCD = FALSE;
		BOOL bCIRP = FALSE;
		BOOL bFirstCD = FALSE;
		BOOL bFirstCIRP = FALSE;
		
		m_stcConsDiag.Clear();
		m_stcChannelImpule.Clear();
		
		PUC_HINT pUcHint = (PUC_HINT)pHint;
		m_nTPCount = pUcHint->wParam;
		
		ITPSet *pTPSet = pFrm->m_pDspKernel->GetTpSet();
		int nCount = pTPSet->GetTpCount();
		
		if(nCount > m_nTPCount)
		{
			for(int i = m_nTPCount; i < nCount; i++)
			{
				WORD wAddr = pTPSet->GetTPAddr(i);
				WORD wData = pTPSet->GetTPData(i);
				//for gsm table
				int nSize = m_vecKeyInfoGSM.size();
				for(int j =0 ; j < nSize; j++)
				{
					if(m_vecKeyInfoGSM.at(j) == wAddr)
					{
						bCD = FALSE;
						bCIRP = FALSE;
						char szRet[256] = {0};
						char* str_tmp=(char *)malloc(256);
						char sz_text[50] = {0};
						m_lstGSM.GetItemText(j, 0, sz_text, 50);
						lua_keyinfo((int)wAddr, (int)wData, sz_text, &str_tmp);
						sprintf(szRet,"%s",str_tmp);
						free(str_tmp);
						if(szRet[0] != '\0')
						{
							m_lstGSM.SetItemText(j, 1, szRet);
						}						
					}
				}
				
				//for td table
				nSize = m_vecKeyInfoTD.size();
				for(j =0 ; j < nSize; j++)
				{
					if(m_vecKeyInfoTD.at(j) == wAddr)
					{
						bCD = FALSE;
						bCIRP = FALSE;
						char szRet[256] = {0};
						char* str_tmp=(char *)malloc(256);
						char sz_text[50] = {0};
						m_lstTD.GetItemText(j, 0, sz_text, 50);
						lua_keyinfo((int)wAddr, (int)wData, sz_text, &str_tmp);
						sprintf(szRet,"%s",str_tmp);
						free(str_tmp);
						if(szRet[0] != '\0')
						{
							m_lstTD.SetItemText(j, 1, szRet);
						}
					}
				}
				
				if(wAddr == m_wAddrCD)
				{
					if(!bCD)
					{
						if(!bFirstCD)
						{
							bFirstCD = TRUE;
						}
						m_strConstLine.Format("line: %d", i);
						UpdateData(FALSE);
					}
					m_stcConsDiag.AddData(wData, !bCD);
					m_stcConsDiag.Invalidate(FALSE);
					bCD = TRUE;
					bCIRP = FALSE;
				}

				if(bFirstCD && wAddr == m_wAddrCIRP)
				{
					if(!bCIRP)
					{
						if(!bFirstCIRP)
						{
							bFirstCIRP = TRUE;
						}
						else
						{
							break;
						}
						m_strImpulseLine.Format("line: %d", i);
						UpdateData(FALSE);
					}
					m_stcChannelImpule.AddData(wData, !bCIRP);
					m_stcChannelImpule.Invalidate(FALSE);
					bCIRP = TRUE;
					bCD = FALSE;
				}
			}
			
			m_nTPCount = nCount;
		}
		
	}
	
}

void CKeyinfoView::OnBtnConst() 
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm == NULL)
	{
		return;
	}
	CDspLoggerDoc * pDoc =(CDspLoggerDoc*) pFrm->m_pView->GetDocument();
	if(pDoc == NULL)
	{
		return;
	}

	UpdateData();
	if(m_strConstLine.IsEmpty())
	{
		return;
	}

	m_strConstLine.Delete(0, 6);
	int nIndex = atoi(m_strConstLine);

	static UC_HINT ucHint;
	ucHint.nIdx   = 0;
	ucHint.wParam = nIndex;
	
	//all views of DspLoggerDocment
	pDoc->UpdateAllViews( this, WM_GOTO_ITEM_BY_FIND,(CObject *)&ucHint);

}

void CKeyinfoView::OnBtnImpulse() 
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm == NULL)
	{
		return;
	}
	CDspLoggerDoc * pDoc =(CDspLoggerDoc*) pFrm->m_pView->GetDocument();
	if(pDoc == NULL)
	{
		return;
	}

	UpdateData();
	if(m_strImpulseLine.IsEmpty())
	{
		return;
	}

	m_strImpulseLine.Delete(0, 6);
	int nIndex = atoi(m_strImpulseLine);

	static UC_HINT ucHint;
	ucHint.nIdx   = 0;
	ucHint.wParam = nIndex;
	
	//all views of DspLoggerDocment
	pDoc->UpdateAllViews( this, WM_GOTO_ITEM_BY_FIND,(CObject *)&ucHint);
}
