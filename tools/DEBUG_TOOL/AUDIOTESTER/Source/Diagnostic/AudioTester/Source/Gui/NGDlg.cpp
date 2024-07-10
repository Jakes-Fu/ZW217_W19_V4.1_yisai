// NGDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AudioTester.h"
#include "NGDlg.h"

#include "BMPCtrlWithSpecRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNGDlg dialog


CNGDlg::CNGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNGDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNGDlg)
	m_nUlNgClkAtt = 0;
	m_nUlNgClkHoldc = 0;
	m_nUlNgClkWpyyA = 0;
	m_nUlNgClkWpyyN = 0;
	m_nUlNgPlkAtt = 0;
	m_nUlNgPlkHoldc = 0;
	m_nUlNgPlkWpyyA = 0;
	m_nUlNgPlkWpyyN = 0;
	m_bWriteToPhone = FALSE;
	m_nAlphaDistor = 0;
	m_nBetaDistor = 0;
	m_nUlNgSelect = -1;
	m_nDlNgSelect = -1;

    m_nDlNgClkAtt = 0;
    m_nDlNgClkHoldc = 0;
    m_nDlNgClkWpyyA = 0;
    m_nDlNgClkWpyyN = 0;
    m_nDlNgPlkAtt = 0;
    m_nDlNgPlkHoldc = 0;
    m_nDlNgPlkWpyyA = 0;
    m_nDlNgPlkWpyyN = 0;
    m_nDlNgSelect = -1;
    m_nDlNgSelect = -1;
	//}}AFX_DATA_INIT
    
    m_bAdvance = FALSE;
    m_pArea = NULL;
}


void CNGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNGDlg)
	DDX_Control(pDX, IDC_STATIC_BETA_DISTOR, m_stcBetaDistor);
	DDX_Control(pDX, IDC_STATIC_ALPHA_DISTOR, m_stcAlphaDistor);
	DDX_Control(pDX, IDC_EDIT_UL_BETA_DISTOR, m_edtUlBetaDistor);
	DDX_Control(pDX, IDC_EDIT_UL_ALPHA_DISTOR, m_edtUlAlphaDistor);
	DDX_Control(pDX, IDC_BUTTON_ADVANCE, m_btnAdvance);
	DDX_Text(pDX, IDC_EDIT_UL_NG_CLK_ATT, m_nUlNgClkAtt);
	DDV_MinMaxInt(pDX, m_nUlNgClkAtt, 0, 4096);
	DDX_Text(pDX, IDC_EDIT_UL_NG_CLK_HOLDC, m_nUlNgClkHoldc);
	DDV_MinMaxInt(pDX, m_nUlNgClkHoldc, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_UL_NG_CLK_WPYY_A, m_nUlNgClkWpyyA);
	DDV_MinMaxInt(pDX, m_nUlNgClkWpyyA, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_UL_NG_CLK_WPYY_N, m_nUlNgClkWpyyN);
	DDV_MinMaxInt(pDX, m_nUlNgClkWpyyN, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_UL_NG_PLK_ATT, m_nUlNgPlkAtt);
	DDV_MinMaxInt(pDX, m_nUlNgPlkAtt, 0, 4096);
	DDX_Text(pDX, IDC_EDIT_UL_NG_PLK_HOLDC, m_nUlNgPlkHoldc);
	DDV_MinMaxInt(pDX, m_nUlNgPlkHoldc, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_UL_NG_PLK_WPYY_A, m_nUlNgPlkWpyyA);
	DDV_MinMaxInt(pDX, m_nUlNgPlkWpyyA, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_UL_NG_PLK_WPYY_N, m_nUlNgPlkWpyyN);
	DDV_MinMaxInt(pDX, m_nUlNgPlkWpyyN, 0, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_NG_CLK_ATT, m_nDlNgClkAtt);
    DDV_MinMaxInt(pDX, m_nDlNgClkAtt, 0, 4096);
    DDX_Text(pDX, IDC_EDIT_DL_NG_CLK_HOLDC, m_nDlNgClkHoldc);
    DDV_MinMaxInt(pDX, m_nDlNgClkHoldc, 0, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_NG_CLK_WPYY_A, m_nDlNgClkWpyyA);
    DDV_MinMaxInt(pDX, m_nDlNgClkWpyyA, 0, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_NG_CLK_WPYY_N, m_nDlNgClkWpyyN);
    DDV_MinMaxInt(pDX, m_nDlNgClkWpyyN, 0, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_NG_PLK_ATT, m_nDlNgPlkAtt);
    DDV_MinMaxInt(pDX, m_nDlNgPlkAtt, 0, 4096);
    DDX_Text(pDX, IDC_EDIT_DL_NG_PLK_HOLDC, m_nDlNgPlkHoldc);
    DDV_MinMaxInt(pDX, m_nDlNgPlkHoldc, 0, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_NG_PLK_WPYY_A, m_nDlNgPlkWpyyA);
    DDV_MinMaxInt(pDX, m_nDlNgPlkWpyyA, 0, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_NG_PLK_WPYY_N, m_nDlNgPlkWpyyN);
    DDV_MinMaxInt(pDX, m_nDlNgPlkWpyyN, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_UL_ALPHA_DISTOR, m_nAlphaDistor);
	DDV_MinMaxInt(pDX, m_nAlphaDistor, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_UL_BETA_DISTOR, m_nBetaDistor);
	DDV_MinMaxInt(pDX, m_nBetaDistor, -32768, 32767);
	DDX_CBIndex(pDX, IDC_COMBO_UL_NG_SELECT, m_nUlNgSelect);
	DDX_CBIndex(pDX, IDC_COMBO_DL_NG_SELECT, m_nDlNgSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNGDlg, CDialog)
	//{{AFX_MSG_MAP(CNGDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCE, OnButtonAdvance)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNGDlg message handlers

BOOL CNGDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_nUlNgSelect = ( m_pArea->m_nNgSelect & 0xff );
    m_nDlNgSelect = ( ( m_pArea->m_nNgSelect & 0xff00 ) >> 8 );
    
    m_nAlphaDistor = m_pArea->m_nAlphaDistor;
    m_nBetaDistor = m_pArea->m_nBetaDistor;

    m_nUlNgPlkWpyyA = m_pArea->m_nUlNgPlkWpyyA;
    m_nUlNgPlkWpyyN = m_pArea->m_nUlNgPlkWpyyN;
    m_nUlNgPlkHoldc = m_pArea->m_nUlNgPlkHoldc;
    m_nUlNgPlkAtt = m_pArea->m_nUlNgPlkAtt;
    m_nUlNgClkWpyyA = m_pArea->m_nUlNgClkWpyyA;
    m_nUlNgClkWpyyN = m_pArea->m_nUlNgClkWpyyN;
    m_nUlNgClkHoldc = m_pArea->m_nUlNgClkHoldc;
    m_nUlNgClkAtt = m_pArea->m_nUlNgClkAtt;

    m_nDlNgPlkWpyyA = m_pArea->m_nDlNgPlkWpyyA;
    m_nDlNgPlkWpyyN = m_pArea->m_nDlNgPlkWpyyN;
    m_nDlNgPlkHoldc = m_pArea->m_nDlNgPlkHoldc;
    m_nDlNgPlkAtt = m_pArea->m_nDlNgPlkAtt;
    m_nDlNgClkWpyyA = m_pArea->m_nDlNgClkWpyyA;
    m_nDlNgClkWpyyN = m_pArea->m_nDlNgClkWpyyN;
    m_nDlNgClkHoldc = m_pArea->m_nDlNgClkHoldc;
    m_nDlNgClkAtt = m_pArea->m_nDlNgClkAtt;

    UpdateData( FALSE );

    ShowAdvance();
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNGDlg::OnOK() 
{
	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

    m_pArea->m_nNgSelect = (short)( m_nUlNgSelect | ( m_nDlNgSelect << 8 ) );
    
    m_pArea->m_nAlphaDistor = m_nAlphaDistor;
    m_pArea->m_nBetaDistor = m_nBetaDistor;
    
    m_pArea->m_nUlNgPlkWpyyA = m_nUlNgPlkWpyyA;
    m_pArea->m_nUlNgPlkWpyyN = m_nUlNgPlkWpyyN;
    m_pArea->m_nUlNgPlkHoldc = m_nUlNgPlkHoldc;
    m_pArea->m_nUlNgPlkAtt = m_nUlNgPlkAtt;
    m_pArea->m_nUlNgClkWpyyA = m_nUlNgClkWpyyA;
    m_pArea->m_nUlNgClkWpyyN = m_nUlNgClkWpyyN;
    m_pArea->m_nUlNgClkHoldc = m_nUlNgClkHoldc;
    m_pArea->m_nUlNgClkAtt = m_nUlNgClkAtt;
    
    m_pArea->m_nDlNgPlkWpyyA = m_nDlNgPlkWpyyA;
    m_pArea->m_nDlNgPlkWpyyN = m_nDlNgPlkWpyyN;
    m_pArea->m_nDlNgPlkHoldc = m_nDlNgPlkHoldc;
    m_pArea->m_nDlNgPlkAtt = m_nDlNgPlkAtt;
    m_pArea->m_nDlNgClkWpyyA = m_nDlNgClkWpyyA;
    m_pArea->m_nDlNgClkWpyyN = m_nDlNgClkWpyyN;
    m_pArea->m_nDlNgClkHoldc = m_nDlNgClkHoldc;
    m_pArea->m_nDlNgClkAtt = m_nDlNgClkAtt;

    EndDialog(IDOK);
}

void CNGDlg::ShowAdvance()
{
    CWnd* pLocator = GetDlgItem( IDC_STATIC_NG_LOCATOR );
    CRect rc;
    pLocator->GetWindowRect( rc );
    int nMove = rc.Height();

    CRect rcDlg;
    GetWindowRect( rcDlg );

    int nShow;
    CString strText;
    if( m_bAdvance )
    {
        strText.LoadString( IDS_HIDE_ADVANCE );
        nShow = SW_SHOW;
    }
    else
    {
        nMove = -nMove;
        nShow = SW_HIDE;
        strText.LoadString( IDS_SHOW_ADVANCE );
    }
    
    rcDlg.bottom += nMove;
    MoveWindow( rcDlg );
    
    m_stcAlphaDistor.ShowWindow( nShow );
    m_stcBetaDistor.ShowWindow( nShow );
    m_edtUlAlphaDistor.ShowWindow( nShow );
    m_edtUlBetaDistor.ShowWindow( nShow );

    m_btnAdvance.SetWindowText( strText );
}

void CNGDlg::OnButtonAdvance() 
{
    m_bAdvance = !m_bAdvance;
    ShowAdvance();
}
