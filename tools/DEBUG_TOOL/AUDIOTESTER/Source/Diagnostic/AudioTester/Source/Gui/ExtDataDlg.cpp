// ExtDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "ExtDataDlg.h"

#include "typedef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtDataDlg dialog


CExtDataDlg::CExtDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExtDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtDataDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    m_lpBuffer = NULL;
    m_nDataSize = 0;
}


void CExtDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtDataDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtDataDlg, CDialog)
	//{{AFX_MSG_MAP(CExtDataDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtDataDlg message handlers

BOOL CExtDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CRect rcBuffer;
    GetDlgItem( IDC_STATIC_HEX_EDIT )->GetWindowRect( rcBuffer );	
    ScreenToClient( rcBuffer );

    m_wndBuffer.Create( NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, rcBuffer, this, IDC_BUFFER );

    m_wndBuffer.SetDigitsInData( 4 );
    m_wndBuffer.SetData( m_lpBuffer,m_nDataSize*2 );

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
