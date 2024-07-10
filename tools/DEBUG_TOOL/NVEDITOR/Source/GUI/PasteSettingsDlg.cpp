// PasteSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "PasteSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasteSettingsDlg dialog


CPasteSettingsDlg::CPasteSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasteSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasteSettingsDlg)
	m_bKeepId = FALSE;
	m_bKeepName = FALSE;
	m_nNewId = 1;
	m_strNewName = _T("");
	//}}AFX_DATA_INIT
}


void CPasteSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasteSettingsDlg)
	DDX_Check(pDX, IDC_CHECK_KEEP_ID, m_bKeepId);
	DDX_Check(pDX, IDC_CHECK_KEEP_NAME, m_bKeepName);
	DDX_Text(pDX, IDC_EDIT_NEW_ID, m_nNewId);
	DDV_MinMaxInt(pDX, m_nNewId, 1, 32767);
	DDX_Text(pDX, IDC_EDIT_NEW_NAME, m_strNewName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasteSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CPasteSettingsDlg)
	ON_BN_CLICKED(IDC_CHECK_KEEP_ID, OnCheckKeepId)
	ON_BN_CLICKED(IDC_CHECK_KEEP_NAME, OnCheckKeepName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasteSettingsDlg message handlers

void CPasteSettingsDlg::OnCheckKeepId() 
{
    m_bKeepId = !m_bKeepId;	
    GetDlgItem( IDC_EDIT_NEW_ID )->EnableWindow( !m_bKeepId );
}

void CPasteSettingsDlg::OnCheckKeepName() 
{
	m_bKeepName = !m_bKeepName;
    GetDlgItem( IDC_EDIT_NEW_NAME )->EnableWindow( !m_bKeepName );
}


int CPasteSettingsDlg::GetNewId()
{
    if( m_bKeepId )
    {
        return -1;
    }
    else
    {
        return m_nNewId;
    }
}

CString CPasteSettingsDlg::GetNewName()
{
    if( m_bKeepName )
    {
        m_strNewName.Empty();
    }

    return m_strNewName;
}

void CPasteSettingsDlg::OnOK() 
{
	UpdateData();
    
    if( m_strNewName.IsEmpty() && !m_bKeepName )
    {
        return;
    }

    if( m_nNewId == INVALID_ID )
    {
        return;
    }

    EndDialog( IDOK );
}

BOOL CPasteSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    
    CEdit* pEdit = (CEdit*)GetDlgItem( IDC_EDIT_NEW_NAME );
    pEdit->SetSel( 0,-1 );
    pEdit->SetFocus();

    if( m_nNewId != INVALID_ID )
    {
        pEdit = (CEdit*)GetDlgItem( IDC_EDIT_NEW_ID ); 
        pEdit->SetReadOnly();
    }

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
