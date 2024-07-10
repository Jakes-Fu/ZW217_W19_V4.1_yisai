// ReadFlashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "ReadFlashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReadFlashDlg dialog


CReadFlashDlg::CReadFlashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReadFlashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReadFlashDlg)
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT
	m_dwStartAddr = 0x30000000;
	m_dwBlockSize = 0x400000;
}


void CReadFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReadFlashDlg)
	DDX_Control(pDX, IDC_EDIT_SIZE, m_ctlSize);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_ctlAddr);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReadFlashDlg, CDialog)
	//{{AFX_MSG_MAP(CReadFlashDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadFlashDlg message handlers

BOOL CReadFlashDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strHex;

	strHex.Format( _T( "0x%x" ), m_dwStartAddr );
	m_ctlAddr.SetWindowText( strHex );

	strHex.Format( _T("0x%x" ), m_dwBlockSize );
	m_ctlSize.SetWindowText( strHex );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReadFlashDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( TRUE );

	CString strValue;
	
	m_ctlAddr.GetWindowText( strValue );
	if( strValue.Left( 2 ).CompareNoCase( _T( "0x" ) ) != 0 )
    {
        // Decimal
        m_dwStartAddr = ::_tcstoul( strValue, NULL, 10 );
    }
    else
    {
        // Heximal
        m_dwStartAddr = ::_tcstoul( strValue, NULL, 16 );
    }

	m_ctlSize.GetWindowText( strValue );
	if( strValue.Left( 2 ).CompareNoCase( _T( "0x" ) ) != 0 )
    {
        // Decimal
        m_dwBlockSize = ::_tcstoul( strValue, NULL, 10 );
    }
    else
    {
        // Heximal
        m_dwBlockSize = ::_tcstoul( strValue, NULL, 16 );
    }

	if ( m_strFilePath.IsEmpty() )
	{
		::AfxMessageBox( _T( "You must specify a file path" ) );
		return;
	}

	EndDialog( IDOK );
//	CDialog::OnOK();
}

void CReadFlashDlg::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg( FALSE , _T( "*.dat" ), NULL, OFN_HIDEREADONLY | OFN_EXPLORER,
		_T( "Data Files (*.dat)|*.dat|All Files (*.*)|*.*|" ), this );

    if( dlg.DoModal() == IDOK )
    {
        // Copy the file path name
        m_strFilePath = dlg.GetPathName();
        UpdateData(FALSE);
    }
	
}
