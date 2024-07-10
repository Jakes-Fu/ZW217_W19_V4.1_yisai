// ModifyGainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "ModifyGainDlg.h"
#include "BMPCtrlWithSpecRect.h"

#include "ConvertFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyGainDlg dialog


CModifyGainDlg::CModifyGainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyGainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifyGainDlg)
	m_bWriteImmediately = FALSE;
	m_fGain = 0.0f;
	m_strParaName = _T("");
	m_strUnit = _T("");
	m_strGain = _T("");
	//}}AFX_DATA_INIT
    
    m_pArea = NULL;
}


void CModifyGainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyGainDlg)
	DDX_Check(pDX, IDC_CHECK_WRITE_IMMEDIATELY, m_bWriteImmediately);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_fGain);
	//DDV_MinMaxFloat(pDX, m_fGain, -96.f, 17.9f);
	DDX_Text(pDX, IDC_STATIC_PARA_NAME, m_strParaName);
	DDX_Text(pDX, IDC_STATIC_UNIT, m_strUnit);
	DDX_Text(pDX, IDC_STATIC_GAIN, m_strGain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifyGainDlg, CDialog)
	//{{AFX_MSG_MAP(CModifyGainDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, OnKillfocusEditGain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyGainDlg message handlers

BOOL CModifyGainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//    SetFont( GetParent()->GetFont() );
//    ChangeFont(this,this->GetParent()->GetFont());
    CString strRange;
    //strRange.Format(_T("[%.1f,%.1f]"),GAIN_MIN_VALUE,GAIN_MAX_VALUE);
	strRange.Format(_T("<=6dB"));
    
    SetWindowText("Modify " + m_pArea->m_strName +" "+strRange );
#ifdef _UNICODE
	swscanf( m_pArea->m_strOtherValue,_T("%f"),&m_fGain );
#else
	sscanf( (LPSTR)(LPCTSTR)(m_pArea->m_strOtherValue),"%f",&m_fGain );
#endif

    unsigned short nGain;
    ::CalcDigGain( m_fGain,nGain );
    
    m_strGain.Format( _T("0x%04x"),nGain );
    
    UpdateData( FALSE );
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CModifyGainDlg::OnOK() 
{
    if( !UpdateData() )
    {
        return;
    } 
	
	//teana hu 2011.03.01
	if(m_fGain > 6)
	{
		((CEdit *)GetDlgItem(IDC_EDIT_GAIN))->SetFocus();
		((CEdit *)GetDlgItem(IDC_EDIT_GAIN))->SetSel(0,-1);
		AfxMessageBox(_T("Please input a value that <= 6dB."));
		return;
	}
	//
   
    SpecialRect::m_bImmediateWrite = m_bWriteImmediately;    
    CString strValue;
    strValue.Format(_T("%.1f"),m_fGain);
    m_pArea->m_strOtherValue = strValue;

	CDialog::OnOK();
}

void CModifyGainDlg::ChangeFont(CWnd* pWnd, CFont* pFont)
{    
    // iterate through and move all child windows and change their font.
    CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);
    
    while (pChildWnd)
    {
        pChildWnd->SetFont(pFont); 
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }
}

void CModifyGainDlg::OnKillfocusEditGain() 
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
