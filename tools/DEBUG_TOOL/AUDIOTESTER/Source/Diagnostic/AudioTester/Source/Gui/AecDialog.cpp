// AecDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AudioTester.h"
#include "AecDialog.h"
#include "AudioTesterSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAecDialog dialog


CAecDialog::CAecDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAecDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAecDialog)
	m_nUlFirHpf = -1;
	m_nAecEnable = -1;
	m_nPdelay = 0;
	m_nDlRefHpfData = -1;
	m_nDlRefHpfPga = -1;
	m_nFirTaps = 0;
	m_nAecFrozen = -1;
	m_nCoeffFrozen = -1;
	m_nDtDectThreshold = 0;
	m_nDtNoiseFloor = 0;
	m_nStepSize = 0;
	m_nCoeffNormShift = 0;
	m_nSendAttenuInDt = 0;
	m_nSendAttenuInRv = 0;
	m_nSendThreshold = 0;
	m_nR2dtThreshold = 0;
	m_nRecvThreshold = 0;
	m_nS2dtThreshold = 0;
	m_nBn40 = 0;
	m_bWriteToPhone = FALSE;
	m_nSaDelay = 0;
	m_nSignalDelay = 0;
	m_nSaCtrl = -1;
	m_nDecorFilter = 0;
	//teana hu 2011.02.14
	m_nCGNGain = 0;
	//}}AFX_DATA_INIT

    m_pArea = NULL;
    m_bAdvance = FALSE;
	m_bAudio4 = FALSE;
}


void CAecDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAecDialog)
	DDX_Control(pDX, IDC_BUTTON_AEC_ADVANCE, m_btnAdvance);
	DDX_CBIndex(pDX, IDC_COMBO_UL_FIR_HPF, m_nUlFirHpf);
	DDX_CBIndex(pDX, IDC_COMBO_AEC_ENABLE, m_nAecEnable);
	DDX_Text(pDX, IDC_EDIT_PDELAY, m_nPdelay);
	DDV_MinMaxInt(pDX, m_nPdelay, 0, 160);
	DDX_CBIndex(pDX, IDC_COMBO_DL_REF_HPF_DL_DATA, m_nDlRefHpfData);
	DDX_CBIndex(pDX, IDC_COMBO_DL_REF_HPF_DL_PGA, m_nDlRefHpfPga);
	DDX_Text(pDX, IDC_EDIT_FIR_TAPS, m_nFirTaps);
	DDV_MinMaxInt(pDX, m_nFirTaps, 32, 256);
	DDX_CBIndex(pDX, IDC_COMBO_AEC_FROZEN, m_nAecFrozen);
	DDX_CBIndex(pDX, IDC_COMBO_COEFF_FROZEN, m_nCoeffFrozen);
	DDX_Text(pDX, IDC_EDIT_DT_DECT_THRESHOLD, m_nDtDectThreshold);
	DDV_MinMaxInt(pDX, m_nDtDectThreshold, 1, 32767);
	DDX_Text(pDX, IDC_EDIT_DT_NOISE_FLOOR, m_nDtNoiseFloor);
	DDV_MinMaxInt(pDX, m_nDtNoiseFloor, 1, 32767);
	DDX_Text(pDX, IDC_EDIT_STEP_SIZE, m_nStepSize);
	DDV_MinMaxInt(pDX, m_nStepSize, 1, 8191);
	DDX_Text(pDX, IDC_EDIT_COEFF_NORM_SHIFT, m_nCoeffNormShift);
	DDV_MinMaxInt(pDX, m_nCoeffNormShift, -31, 31);
	DDX_Text(pDX, IDC_EDIT_SEND_ATTENU_IN_DT, m_nSendAttenuInDt);
	DDV_MinMaxInt(pDX, m_nSendAttenuInDt, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_SEND_ATTENU_IN_RV, m_nSendAttenuInRv);
	DDV_MinMaxInt(pDX, m_nSendAttenuInRv, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_SEND_THRESHOLD, m_nSendThreshold);
	//DDV_MinMaxInt(pDX, m_nSendThreshold, 1, 32767);
	DDX_Text(pDX, IDC_EDIT_R2DT_THRESHOLD, m_nR2dtThreshold);
	DDV_MinMaxInt(pDX, m_nR2dtThreshold, 1, 32767);
	DDX_Text(pDX, IDC_EDIT_RECV_THRESHOLD, m_nRecvThreshold);
	//DDV_MinMaxInt(pDX, m_nRecvThreshold, 1, 32767);
	DDX_Text(pDX, IDC_EDIT_S2DT_THRESHOLD, m_nS2dtThreshold);
	DDV_MinMaxInt(pDX, m_nS2dtThreshold, 1, 32767);
	DDX_Text(pDX, IDC_EDIT_BN40, m_nBn40);
	DDV_MinMaxInt(pDX, m_nBn40, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_SIGNAL_DELAY, m_nSignalDelay);
	DDV_MinMaxInt(pDX, m_nSignalDelay, 0, 255);
	DDX_Text(pDX, IDC_EDIT_SA_DELAY, m_nSaDelay);
	DDV_MinMaxInt(pDX, m_nSaDelay, 0, 255);	
	DDX_CBIndex(pDX, IDC_COMBO_SA_CTRL, m_nSaCtrl);
	DDX_Text(pDX, IDC_EDIT_DECOR_FILTER_SWITCH, m_nDecorFilter);
	DDV_MinMaxInt(pDX, m_nDecorFilter, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_CGN_GAIN, m_nCGNGain);
	DDV_MinMaxInt(pDX, m_nCGNGain, 0, 32767);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAecDialog, CDialog)
	//{{AFX_MSG_MAP(CAecDialog)
	ON_BN_CLICKED(IDC_BUTTON_AEC_ADVANCE, OnButtonAecAdvance)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAecDialog message handlers

BOOL CAecDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//teana hu 2011.02.14
	m_bAudio4 = m_pArea->m_bAudio4;
	if(m_bAudio4)
	{
		GetDlgItem(IDC_STATIC_SA)->SetWindowText(_T("SA && CNG"));
		GetDlgItem(IDC_STATIC_R2DT_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_S2DT_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SIGNAL_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SA_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_R2DT_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_S2DT_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SIGNAL_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SA_DELAY)->ShowWindow(SW_HIDE);
		m_nCGNGain = m_pArea->m_nCGNGain;
	}
	else
	{
		GetDlgItem(IDC_STATIC_SA)->SetWindowText(_T("SA"));
		GetDlgItem(IDC_STC_CGN_GAIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CGN_GAIN)->ShowWindow(SW_HIDE);
	}
	//
	
    m_nUlFirHpf = m_pArea->m_nUlFirHpf;
    m_nAecEnable = m_pArea->m_nAecEnable;
    m_nPdelay = m_pArea->m_nPdelay;
    m_nDlRefHpfData = ( m_pArea->m_nDlRefHpf & 1 );
    m_nDlRefHpfPga = ( ( m_pArea->m_nDlRefHpf & 0x100 ) >> 8 );
    m_nDecorFilter = m_pArea->m_nDecorFilter;
    m_nFirTaps = m_pArea->m_nFirTaps;
    m_nCoeffFrozen = m_pArea->m_nCoeffFrozen;
    m_nAecFrozen = m_pArea->m_nAecFrozen;
    m_nDtDectThreshold = m_pArea->m_nDtDectThreshold;
    m_nDtNoiseFloor = m_pArea->m_nDtNoiseFloor;
    m_nStepSize = m_pArea->m_nStepSize;
    m_nCoeffNormShift = m_pArea->m_nCoeffNormShift;
    m_nSaCtrl = m_pArea->m_nSaCtrl;
    m_nSendAttenuInDt = m_pArea->m_nSendAttenuInDt;
    m_nSendAttenuInRv = m_pArea->m_nSendAttenuInRv;
    m_nSendThreshold = m_pArea->m_nSendThreshold;
    m_nR2dtThreshold = m_pArea->m_nR2dtThreshold;
    m_nS2dtThreshold = m_pArea->m_nS2dtThreshold;
    m_nRecvThreshold = m_pArea->m_nRecvThreshold;
    m_nBn40 = m_pArea->m_nBn40;
    m_nSaDelay = m_pArea->m_nSaDelay;
    m_nSignalDelay = m_pArea->m_nSignalDelay;
    
    m_bWriteToPhone = m_pArea->m_bImmediateWrite;

    ShowAdvance();
	
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
	CString strTitle;
	( (CAudioModePage*)pSheet->GetActivePage() )->GetTitle( strTitle );
	pSheet->m_CtrlStatus.RefreshControl( strTitle,IDD_DIALOG_AEC,this );

    UpdateData( FALSE );
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAecDialog::OnOK() 
{
    if (!UpdateData(TRUE))
    {
        TRACE0("UpdateData failed during dialog termination.\n");
        // the UpdateData routine will set focus to correct item
        return;
    }
    
    m_pArea->m_nUlFirHpf = (short)m_nUlFirHpf;
    m_pArea->m_nAecEnable = (short)m_nAecEnable;
    m_pArea->m_nPdelay = m_nPdelay;
    m_pArea->m_nDlRefHpf = (short)( m_nDlRefHpfData + ( m_nDlRefHpfPga << 8 ) );
    m_pArea->m_nDecorFilter = (short)m_nDecorFilter;
    m_pArea->m_nFirTaps = m_nFirTaps;
    m_pArea->m_nCoeffFrozen = (short)m_nCoeffFrozen;
    m_pArea->m_nAecFrozen = (short)m_nAecFrozen;
    m_pArea->m_nDtDectThreshold = m_nDtDectThreshold;
    m_pArea->m_nDtNoiseFloor = m_nDtNoiseFloor;
    m_pArea->m_nStepSize = m_nStepSize;
    m_pArea->m_nCoeffNormShift = m_nCoeffNormShift;
    m_pArea->m_nSaCtrl = (short)m_nSaCtrl;
    m_pArea->m_nSendAttenuInDt = m_nSendAttenuInDt;
    m_pArea->m_nSendAttenuInRv = m_nSendAttenuInRv;
    m_pArea->m_nSendThreshold = m_nSendThreshold;
    m_pArea->m_nR2dtThreshold = m_nR2dtThreshold;
    m_pArea->m_nS2dtThreshold = m_nS2dtThreshold;
    m_pArea->m_nRecvThreshold = m_nRecvThreshold;
    m_pArea->m_nBn40 = m_nBn40;
    m_pArea->m_nSaDelay = m_nSaDelay;
    m_pArea->m_nSignalDelay = m_nSignalDelay;
    m_pArea->m_bImmediateWrite = m_bWriteToPhone;
	//teanahu 2011.02.14
	if(m_bAudio4)
	{
		m_pArea->m_nCGNGain = m_nCGNGain;
		CString strText;
		strText.Format(IDS_PRIMPT_THRESHOLD_RANGE, -32768, 32767);
		if(m_nSendThreshold < -32768 || m_nSendThreshold > 32767)
		{
			((CEdit *)GetDlgItem(IDC_EDIT_SEND_THRESHOLD))->SetFocus();
			((CEdit *)GetDlgItem(IDC_EDIT_SEND_THRESHOLD))->SetSel(0,-1);
			AfxMessageBox(strText);
			return;
		}

		if(m_nRecvThreshold < -32768 || m_nRecvThreshold > 32767)
		{
			((CEdit *)GetDlgItem(IDC_EDIT_RECV_THRESHOLD))->SetFocus();
			((CEdit *)GetDlgItem(IDC_EDIT_RECV_THRESHOLD))->SetSel(0,-1);
			AfxMessageBox(strText);
			return;
		}
	}
	else
	{
		CString strText;
		strText.Format(IDS_PRIMPT_THRESHOLD_RANGE, 1, 32767);
		if(m_nSendThreshold < 1 || m_nSendThreshold > 32767)
		{
			((CEdit *)GetDlgItem(IDC_EDIT_SEND_THRESHOLD))->SetFocus();
			((CEdit *)GetDlgItem(IDC_EDIT_SEND_THRESHOLD))->SetSel(0,-1);
			AfxMessageBox(strText);
			return;
		}

		if(m_nRecvThreshold < 1 || m_nRecvThreshold > 32767)
		{
			((CEdit *)GetDlgItem(IDC_EDIT_RECV_THRESHOLD))->SetFocus();
			((CEdit *)GetDlgItem(IDC_EDIT_RECV_THRESHOLD))->SetSel(0,-1);
			AfxMessageBox(strText);
			return;
		}
	}
	//
    
    EndDialog(IDOK);
}

void CAecDialog::ShowAdvance()
{
    CWnd* pLocator = GetDlgItem( IDC_STATIC_AEC_LOCATOR );
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
    
    m_btnAdvance.SetWindowText( strText );
}

void CAecDialog::OnButtonAecAdvance() 
{
    m_bAdvance = !m_bAdvance;
    ShowAdvance();
}
