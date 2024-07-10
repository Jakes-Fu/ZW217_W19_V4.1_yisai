// MarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "MarkDlg.h"

#include "MarkSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarkDlg dialog


CMarkDlg::CMarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkDlg)
    m_strMark = _T("");
	//}}AFX_DATA_INIT
    m_pMarkManage = NULL;
    m_bMarkDlg    = TRUE;  
}

CMarkDlg::CMarkDlg(CMarkSet* pMarkManage,BOOL bMarkDlg,CWnd* pParent)
    : CDialog(CMarkDlg::IDD,pParent)
{
  	m_strMark = _T("");
    m_pMarkManage = (CMarkSet*)pMarkManage;
    m_bMarkDlg    = bMarkDlg;
}

void CMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkDlg)
	DDX_Text(pDX, IDC_EDIT_MARK, m_strMark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarkDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkDlg)
	ON_LBN_DBLCLK(IDC_LIST_MARKS, OnDblclkListMarks)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkDlg message handlers

BOOL CMarkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    if(m_pMarkManage != NULL)
    {
        CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_MARKS);
        int nCount = m_pMarkManage->GetMarkCount();

        for(int i=0;i<nCount;i++)
        {
            CString strMark;
            m_pMarkManage->GetMarkString(i,strMark);
            pList->AddString(strMark);
        }
    }

    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MARK);
    if(m_bMarkDlg)
    {
        // Used to mark an item
        pEdit->SetFocus();
        return FALSE;
    }
    else
    {
        // Used to select a mark only
        pEdit->EnableWindow(FALSE);
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
    }
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMarkDlg::OnDblclkListMarks() 
{
	CListBox* pBox = (CListBox*)GetDlgItem(IDC_LIST_MARKS);

    int nIndex = pBox->GetCurSel();
    ASSERT(nIndex != LB_ERR);

    CString strText;
    pBox->GetText(nIndex,strText);
    GetDlgItem(IDC_EDIT_MARK)->SetWindowText(strText);

    OnOK();
}
