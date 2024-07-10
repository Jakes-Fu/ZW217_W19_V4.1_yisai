// DigitalGainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DigitalGainDlg.h"

#include "ConvertFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDigitalGainDlg dialog


CDigitalGainDlg::CDigitalGainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDigitalGainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDigitalGainDlg)
	m_dDpAtt = 0.0;
	m_dGain = 0.0;
	m_strDpatt = _T("");
	m_strGain = _T("");
	//}}AFX_DATA_INIT

    m_pArea = NULL;
}


void CDigitalGainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDigitalGainDlg)
	DDX_Text(pDX, IDC_EDIT_DPATT, m_dDpAtt);
	DDV_MinMaxDouble(pDX, m_dDpAtt, -96., 17.9);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dGain);
	DDV_MinMaxDouble(pDX, m_dGain, -96., 17.9);
	DDX_Text(pDX, IDC_STATIC_DPATT, m_strDpatt);
	DDX_Text(pDX, IDC_STATIC_GAIN, m_strGain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDigitalGainDlg, CDialog)
	//{{AFX_MSG_MAP(CDigitalGainDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_DPATT, OnKillfocusEditDpatt)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, OnKillfocusEditGain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDigitalGainDlg message handlers

BOOL CDigitalGainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_dGain = m_pArea->m_dGain;
    m_dDpAtt = m_pArea->m_dDpAtt;

    unsigned short nGain;
    ::CalcDigGain( m_dDpAtt,nGain );
    m_strDpatt.Format( _T("0x%04x"),nGain );
    
    ::CalcDigGain( m_dGain,nGain );
    m_strGain.Format( _T("0x%04x"),nGain );
    
    UpdateData( FALSE );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDigitalGainDlg::OnOK() 
{
    if( !UpdateData() )
    {
        return;
    }    
    
    m_pArea->m_dGain = m_dGain;
    m_pArea->m_dDpAtt = m_dDpAtt;

    EndDialog( IDOK );
}

void CDigitalGainDlg::OnKillfocusEditGain() 
{
    CString strText;
    GetDlgItem( IDC_EDIT_GAIN )->GetWindowText( strText );
    
    double dGain;
#ifdef _UNICODE
	swscanf( strText,_T("%lf"),&dGain );
#else
	sscanf( (LPSTR)(LPCTSTR)strText,"%lf",&dGain );
#endif
    
    unsigned short nGain;
    ::CalcDigGain( dGain,nGain );
    
    strText.Format( _T("0x%04x"),nGain );
    GetDlgItem( IDC_STATIC_GAIN )->SetWindowText( strText );
}

void CDigitalGainDlg::OnKillfocusEditDpatt() 
{
    CString strText;
    GetDlgItem( IDC_EDIT_DPATT )->GetWindowText( strText );
    
    double dGain;
#ifdef _UNICODE
	swscanf( strText,_T("%lf"),&dGain );
#else
	sscanf( (LPSTR)(LPCTSTR)strText,"%lf",&dGain );
#endif
    
    unsigned short nGain;
    ::CalcDigGain( dGain,nGain );
    
    strText.Format( _T("0x%04x"),nGain );
    GetDlgItem( IDC_STATIC_DPATT )->SetWindowText( strText );
}
