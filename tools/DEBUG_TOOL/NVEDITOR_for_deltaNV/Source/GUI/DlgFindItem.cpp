// DlgFindItem.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "DlgFindItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFindItem dialog


CDlgFindItem::CDlgFindItem(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFindItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFindItem)
	m_bFullMatch = FALSE;
	m_bItem = TRUE;
	m_bValue = FALSE;
	m_strTarget = _T("");
	m_bChanged = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgFindItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFindItem)
	DDX_Check(pDX, IDC_DF_CHK_FULL_MATCH, m_bFullMatch);
	DDX_Check(pDX, IDC_DF_CHK_ITEM, m_bItem);
	DDX_Check(pDX, IDC_DF_CHK_VALUE, m_bValue);
	DDX_Text(pDX, IDC_DF_EDT_TARGET, m_strTarget);
	DDX_Check(pDX, IDC_DF_CHK_CHANGED, m_bChanged);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFindItem, CDialog)
	//{{AFX_MSG_MAP(CDlgFindItem)
	ON_BN_CLICKED(IDOK, OnFind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFindItem message handlers

void CDlgFindItem::OnFind() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_strTarget.IsEmpty() && !m_bChanged && m_bFullMatch)
	{
		AfxMessageBox(_T("Target string can not be empty!"));
		return;
	}
	m_dwFlag = 0;
	//SET_FIND_FLAG_ITEM(m_dwFlag,m_bItem);
	//SET_FIND_FLAG_VALUE(m_dwFlag,m_bValue);
	SET_FIND_FLAG_FULLMATCH(m_dwFlag,m_bFullMatch);
	SET_FIND_FLAG_CHANGED(m_dwFlag,m_bChanged);

	CDialog::OnOK();
}
