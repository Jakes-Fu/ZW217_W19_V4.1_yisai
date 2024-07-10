// FileNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "FileNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileNameDlg dialog


CFileNameDlg::CFileNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileNameDlg)
	m_strFileName = _T("");
	//}}AFX_DATA_INIT
	m_nLimitText = 0;
}


void CFileNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileNameDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDT_FILENAME, m_strFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileNameDlg, CDialog)
	//{{AFX_MSG_MAP(CFileNameDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileNameDlg message handlers

BOOL CFileNameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	VERIFY( m_edtFileName.SubclassDlgItem(IDC_EDT_FILENAME, this) );
	m_edtFileName.SetLimitText(m_nLimitText);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
