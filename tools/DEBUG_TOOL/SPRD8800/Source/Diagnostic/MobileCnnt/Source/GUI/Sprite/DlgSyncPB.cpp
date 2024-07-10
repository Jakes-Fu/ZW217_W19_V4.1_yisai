// DlgSyncPB.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "DlgSyncPB.h"
#include "InfoWrapper.h"
#include "IPB.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSyncPB dialog


CDlgSyncPB::CDlgSyncPB(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSyncPB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSyncPB)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nFocusItemPC = -1;
	m_nFocusItemMS = -1;
	m_nFocusItemRET = -1;
	
	m_nUsedInPhone = 0;
	m_nTotalInPhone = 0;
	m_nUsedInSIM = 0;
	m_nTotalInSIM = 0;

	m_nOldStore = 0;
	m_pPB = NULL;
	m_pIW = NULL;
	m_pPBFactory = NULL;
}


void CDlgSyncPB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSyncPB)
	DDX_Control(pDX, IDC_BTN_UP, m_btnUp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_LST_SYNC_RST, m_lstRET);
	DDX_Control(pDX, IDC_LST_SYNC_PC, m_lstPC);
	DDX_Control(pDX, IDC_LST_SYNC_MS, m_lstMS);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSyncPB, CDialog)
	//{{AFX_MSG_MAP(CDlgSyncPB)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_SYNC_RST, OnDblclkLstSyncRst)
	ON_NOTIFY(NM_CLICK, IDC_LST_SYNC_PC, OnClickLstSyncPc)
	ON_NOTIFY(NM_CLICK, IDC_LST_SYNC_MS, OnClickLstSyncMs)
	ON_NOTIFY(NM_CLICK, IDC_LST_SYNC_RST, OnClickLstSyncRst)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_SYNC_MS, OnItemchangedLstSyncMs)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_SYNC_PC, OnItemchangedLstSyncPc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_SYNC_RST, OnItemchangedLstSyncRst)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSyncPB message handlers

BOOL CDlgSyncPB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pIW = theApp.GetInfoWrapper();
	_ASSERTE( m_pIW != NULL );

	m_pPBFactory = theApp.m_pPBFactory;
	_ASSERTE( m_pPBFactory != NULL );

    SetListStyle(m_lstPC);
	SetListStyle(m_lstMS);
	SetListStyle(m_lstRET);

	InitColomn();

	for(int i=0;i<m_arrPBInPC.GetSize();i++)
	{
		FillList(m_lstPC,m_arrPBInPC.GetAt(i));
	}
	for(int j=0;j<m_arrPBInMS.GetSize();j++)
	{
		FillList(m_lstMS,m_arrPBInMS.GetAt(j));
	}

	m_pPB = NULL;



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSyncPB::OnOK() 
{
	// TODO: Add extra validation here
	int nCount;
	int i;
	//±¸·ÝPB
/*	nCount = m_arrPBInRET.GetSize();
	if(nCount != 0)
	{
		for(i=0;i<nCount;i++)
		{
			m_pPB = m_arrPBInRET.GetAt(i);
			PPB pPB = new PB;
			memcpy(pPB,m_pPB,sizeof(_PB));
			m_arrPBInRET.SetAt(i,pPB);		
		}
	}
*/
	int nMaxInBM = 0;
	int nUsedInBM = 0;
	if( !theApp.TestIsConnected(FALSE) )
	{
		AfxMessageBox( IDS_ERR_NOT_CONNECT );
		return;
	}
	if( !theApp.m_pPBFactory->GetPBNum(&nUsedInBM,&nMaxInBM) )
	{
		AfxMessageBox(IDS_ERR_DELETE);
		return;
	}
	
	nCount = m_arrPBInPC.GetSize();
	if(nCount != 0)
	{
		for(i=0;i<nCount;i++)
		{
			m_pPB = m_arrPBInPC.GetAt(i);
			m_pIW->RemovePB(m_pPB);			
		}
	}

	nCount = m_arrPBInMS.GetSize();
	if(nCount != 0)
	{
		for(i=0;i<nCount;i++)
		{
			m_pPB = m_arrPBInMS.GetAt(i);

			if( !theApp.TestIsConnected(FALSE) )
			{
				AfxMessageBox(IDS_ERR_NOT_CONNECT);
				return;
			}
			if( !m_pPBFactory->DeletePB(m_pPB) )
			{
				AfxMessageBox(IDS_ERR_DELETE);
				return;
			}
			nUsedInBM--;

			PPBGROUP pPBG = m_pIW->GetPBGByIdInPhone(m_pPB->nGroupIndex);
			if(pPBG != NULL)
			{
				pPBG->nMemberNum--;
			}
			
		
			m_pIW->RemovePB(m_pPB);			
		}
	}

	nCount = m_arrPBInRET.GetSize();
	if(nCount != 0)
	{
		for(i=0;i<nCount;i++)
		{
			m_pPB = m_arrPBInRET.GetAt(i);
			PPB pPB = NULL;
			if(m_pPB->wStore == ID_STORE_PC)
			{
				if(nUsedInBM >= nMaxInBM)
				{
					AfxMessageBox(IDS_ERR_SYN_PHONE_FULL);
					CDialog::OnOK();
					return;
				}
				if( !theApp.TestIsConnected(FALSE) )
				{
					AfxMessageBox(IDS_ERR_NOT_CONNECT);
					return;
				}

				pPB = new PB;
				memcpy(pPB,m_pPB,sizeof(_PB));
				pPB->wStore = ID_STORE_PHONE;
				if(m_pIW->GetPBGCount(ID_STORE_ALL)>MAX_SIM_NUM)
					pPB->nGroupIndex = m_pIW->GetPBG(0)->nID;
				else
					pPB->nGroupIndex = 0;

				if( _tcslen(pPB->szPhone) == 0 &&
					_tcslen(pPB->szHomePhone) == 0 &&
					_tcslen(pPB->szOfficePhone) == 0 )
				{
					_tcscpy(pPB->szPhone,_T("0"));
				}


				if( !m_pPBFactory->AddPB(pPB) )
				{
					AfxMessageBox(IDS_ERR_ADD_FAIL);
					delete pPB;
					return;
				}
				if(m_pIW->GetPBGCount(ID_STORE_ALL)>MAX_SIM_NUM)
					m_pIW->GetPBG(0)->nMemberNum++;			
				
				m_pIW->AddPB(pPB);
				
				nUsedInBM++;
			}
			else
			{
				pPB = new PB;
				memcpy(pPB,m_pPB,sizeof(_PB));
				pPB->wStore = ID_STORE_PC;
				m_pIW->AddPB(pPB);
			}
		}
	}
	CDialog::OnOK();
}

void CDlgSyncPB::OnBtnDown() 
{
	// TODO: Add your control notification handler code here
	m_pPB = GetFocusItemData();
	if(m_pPB!=NULL)
	{
		m_arrPBInRET.Add(m_pPB);
		FillList(m_lstRET,m_pPB);
		if(m_pPB->wStore == ID_STORE_PC)
		{
		  ModifyList(m_lstPC,m_nFocusItemPC);
		  m_nFocusItemPC =-1;
		}
		else
		{
		  ModifyList(m_lstMS,m_nFocusItemMS);
		  m_nFocusItemMS =-1;
		}

	}
}

void CDlgSyncPB::OnBtnUp() 
{
	// TODO: Add your control notification handler code here
	if(m_nFocusItemRET != -1)
	{
		ModifyList(m_lstRET,m_nFocusItemRET);
		m_nFocusItemRET = -1;
	}
}

void CDlgSyncPB::OnDblclkLstSyncRst(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgSyncPB::InitColomn()
{
	_ASSERTE( m_pIW != NULL );

	DWORD dwFlag = ID_CATEG_PB;
	dwFlag = dwFlag << 16;

	m_pIW->m_bSyncDlg = TRUE;

	int nNum = m_pIW->GetColumnCount(dwFlag);

	m_pIW->m_bSyncDlg = FALSE;

	for( int i = 0; i < nNum; ++i )
	{
		m_lstMS.InsertColumn( i, m_pIW->GetColumn(dwFlag, i), LVCFMT_LEFT, 
				          m_pIW->GetColumnWidth(dwFlag, i) );
		m_lstPC.InsertColumn( i, m_pIW->GetColumn(dwFlag, i), LVCFMT_LEFT, 
				          m_pIW->GetColumnWidth(dwFlag, i) );
		
		m_lstRET.InsertColumn( i, m_pIW->GetColumn(dwFlag, i), LVCFMT_LEFT, 
				          m_pIW->GetColumnWidth(dwFlag, i) );
	}

}
void CDlgSyncPB::FillList(CListCtrl &lstCtrl,_PB * pPB)
{
	int nItem = lstCtrl.GetItemCount();
	CString strStore;
	GetStorePlace(pPB->wStore, strStore);
	lstCtrl.InsertItem(nItem,strStore);
	lstCtrl.SetItemText(nItem,1,pPB->szName);
	lstCtrl.SetItemText(nItem,2,pPB->szPhone);
	lstCtrl.SetItemText(nItem,3,pPB->szOfficePhone);
	lstCtrl.SetItemText(nItem,4,pPB->szHomePhone);
	lstCtrl.SetItemText(nItem,4,pPB->szEmail);
	if(lstCtrl == m_lstRET)
			lstCtrl.SetItemText(nItem,0,_T("=="));

	lstCtrl.SetItemData(nItem,(DWORD)pPB);
}

void CDlgSyncPB::GetStorePlace(WORD wStore, CString &strStore)
{
	switch( wStore )
	{
	case ID_STORE_PC:
		VERIFY( strStore.LoadString(IDS_STORE_PC) );
		break;
	case ID_STORE_PHONE:
		VERIFY( strStore.LoadString(IDS_STORE_PHONE) );
		break;
	case ID_STORE_SIM:
		VERIFY( strStore.LoadString(IDS_STORE_SIM) );
		break;
	default:
		strStore.Empty();
		break;
	}
}
void CDlgSyncPB::SetListStyle(CListCtrl &lstCtrl)
{
	DWORD dwExStyle = lstCtrl.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES;
	dwExStyle |= LVS_EX_FULLROWSELECT;

	lstCtrl.SetExtendedStyle(dwExStyle);

	CHeaderCtrl * pHeaderCtrl = lstCtrl.GetHeaderCtrl();
	pHeaderCtrl->ModifyStyle(HDS_BUTTONS, HDS_HOTTRACK);
}

void CDlgSyncPB::OnClickLstSyncPc(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgSyncPB::OnClickLstSyncMs(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgSyncPB::OnClickLstSyncRst(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgSyncPB::OnItemchangedLstSyncMs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	m_nFocusItemMS = pNMListView->iItem;
	m_nFocusItemPC = -1;
	
	*pResult = 0;
}

void CDlgSyncPB::OnItemchangedLstSyncPc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nFocusItemPC = pNMListView->iItem;
	m_nFocusItemMS = -1;	
	*pResult = 0;
}

void CDlgSyncPB::OnItemchangedLstSyncRst(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nFocusItemRET = pNMListView->iItem;	
	*pResult = 0;
}

_PB* CDlgSyncPB::GetFocusItemData(BOOL bDown /*=TRUE*/)
{
	if(bDown)
	{
		if(m_nFocusItemPC != -1)
			return (_PB*)m_lstPC.GetItemData(m_nFocusItemPC);
		if(m_nFocusItemMS != -1)
			return (_PB*)m_lstMS.GetItemData(m_nFocusItemMS);
		return NULL;
	}
	else
		return (_PB*)m_lstRET.GetItemData(m_nFocusItemRET);
}

void CDlgSyncPB::ModifyList(CListCtrl &lstCtrl,int nItem)
{
	if(nItem != -1)
	{
		m_pPB = (_PB*)lstCtrl.GetItemData(nItem);
		lstCtrl.DeleteItem(nItem);
		if(lstCtrl == m_lstPC)
		{
			m_arrPBInPC.RemoveAt(nItem);
			return;
		}
		if(lstCtrl == m_lstMS)
		{
			m_arrPBInMS.RemoveAt(nItem);
			return;
		}
		if(lstCtrl == m_lstRET)
		{
			
			if(m_pPB->wStore == ID_STORE_PC)
			{
				m_arrPBInPC.Add(m_pPB);
				FillList(m_lstPC,m_pPB);
			}
			else
			{
				m_arrPBInMS.Add(m_pPB);
				FillList(m_lstMS,m_pPB);
			}
			return;
		}
	}
}
