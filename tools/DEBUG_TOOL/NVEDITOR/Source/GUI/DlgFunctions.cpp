// DlgFunctions.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "DlgFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFunctions dialog


CDlgFunctions::CDlgFunctions(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFunctions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFunctions)
	m_bCheckAll = FALSE;
	//}}AFX_DATA_INIT
	m_pFucList = NULL;
	m_pList = NULL;
}


void CDlgFunctions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFunctions)
	DDX_Control(pDX, IDC_DMF_LST_FUNS, m_lstFuns);
	DDX_Check(pDX, IDC_DMF_CHK_ALL, m_bCheckAll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFunctions, CDialog)
	//{{AFX_MSG_MAP(CDlgFunctions)
	ON_BN_CLICKED(IDC_DMF_CHK_ALL, OnChkAll)
	ON_BN_CLICKED(IDOK, OnExcute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFunctions message handlers

BOOL CDlgFunctions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_lstFuns.SetBkColor(RGB(232, 232, 232));	
	m_lstFuns.ModifyStyle(0, LVS_SHOWSELALWAYS);
	
	DWORD dwExStyle = m_lstFuns.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	dwExStyle |= LVS_EX_CHECKBOXES;
	m_lstFuns.SetExtendedStyle(dwExStyle);

	m_lstFuns.InsertColumn(0, _T("Function Name"), LVCFMT_LEFT, 150 );
	m_lstFuns.InsertColumn(1, _T("Function Description"), LVCFMT_LEFT, 220 );
	m_lstFuns.InsertColumn(2, _T("Excute Result"), LVCFMT_LEFT, 100 );

	if(m_pFucList != NULL)
	{
		std::vector<FUNC_ITEM_PTR> &vFuncList = *m_pFucList;
		for(UINT i=0; i< m_pFucList->size(); i++)
		{
			m_lstFuns.InsertItem(i,vFuncList[i]->szFuncName);
			m_lstFuns.SetItemText(i,1,vFuncList[i]->szFuncDesc);
			m_lstFuns.SetItemData(i,(DWORD)vFuncList[i]);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFunctions::OnChkAll() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int nItemCount = m_lstFuns.GetItemCount();
	for(int i = 0; i< nItemCount; i++)
	{
		m_lstFuns.SetCheck(i,m_bCheckAll);
		m_lstFuns.SetItemText(i,2,_T(""));
	}

}

void CDlgFunctions::OnExcute() 
{
	// TODO: Add your control notification handler code here
	if(m_pList == NULL)
		return;

	CWaitCursor wc; //lint  !e1502
	this->EnableWindow(FALSE);
	int nItemCount = m_lstFuns.GetItemCount();	
	for( int i = 0; i< nItemCount; i++)
	{
		if(m_lstFuns.GetCheck(i))
		{
			FUNC_ITEM_PTR pFuncItem = (FUNC_ITEM_PTR)m_lstFuns.GetItemData(i);
			BOOL bOK = m_pList->ImportItem(pFuncItem->agRules);
			if(bOK)
			{
				m_lstFuns.SetItemText(i,2,_T("SUCCESS"));
			}
			else
			{
				m_lstFuns.SetItemText(i,2,_T("FAIL"));
			}
		}
	}
	this->EnableWindow(TRUE);
}
