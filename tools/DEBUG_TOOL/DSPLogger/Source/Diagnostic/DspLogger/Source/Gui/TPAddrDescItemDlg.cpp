// TPAddrDescItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "TPAddrDescItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTPAddrDescItemDlg dialog


CTPAddrDescItemDlg::CTPAddrDescItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTPAddrDescItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTPAddrDescItemDlg)
	m_strAddr = _T("");
	m_strData = _T("****");
	m_strDesc = _T("");
	m_strFilter = _T("");
	//}}AFX_DATA_INIT

    m_dwColor = RGB(0,0,0);
}


void CTPAddrDescItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTPAddrDescItemDlg)
	DDX_Control(pDX, IDC_STC_COLOR, m_stcColor);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_strAddr);
	DDV_MaxChars(pDX, m_strAddr, 4);
	DDX_Text(pDX, IDC_EDIT_DATA, m_strData);
	DDV_MaxChars(pDX, m_strData, 4);
	DDX_Text(pDX, IDC_EDIT_DESC, m_strDesc);
	DDV_MaxChars(pDX, m_strDesc, 50);
	DDX_Text(pDX, IDC_EDIT_FILTER, m_strFilter);
	DDV_MaxChars(pDX, m_strFilter, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTPAddrDescItemDlg, CDialog)
	//{{AFX_MSG_MAP(CTPAddrDescItemDlg)
    ON_COMMAND(IDC_BTN_COLOR, OnBtnColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTPAddrDescItemDlg message handlers

/*@ Liu Kai CR4895 2003-9-23 */
BOOL CTPAddrDescItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ADDR);
    pEdit->SetSel(0,-1);
    pEdit->SetFocus();	

    m_stcColor.SetBkColor(m_dwColor);
	
	//teana hu 2009.11.12
	GetDlgItem(IDC_EDIT_DESC)->EnableWindow(FALSE);
        
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTPAddrDescItemDlg::OnBtnColor(void)
{
    CColorDialog clrDlg(m_dwColor);

    if( IDOK == clrDlg.DoModal() )
    {
        m_dwColor = clrDlg.GetColor();

        m_stcColor.SetBkColor(m_dwColor);
        m_stcColor.Invalidate();
    }
}

void CTPAddrDescItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	CString strDesc = m_strDesc;
	if(strDesc.Find("%") != -1)
	{
		int nLeft = strDesc.Replace("(", "");
		int nRight = strDesc.Replace(")", "");

		CEdit *pEdt = (CEdit *)GetDlgItem(IDC_EDIT_DESC);
		
		if(nLeft != nRight)
		{
			AfxMessageBox("Description: \"()\" is not matched, please check!");
			pEdt->SetSel(0, -1);
			pEdt->SetFocus();
			return;
		}

		int nFind = strDesc.Replace("\"", "");
		if(nFind % 2 != 0)
		{
			AfxMessageBox("Description: \"\" is not matched, please check!");
			pEdt->SetSel(0, -1);
			pEdt->SetFocus();
			return;
		}

	}
	
	CDialog::OnOK();
}
