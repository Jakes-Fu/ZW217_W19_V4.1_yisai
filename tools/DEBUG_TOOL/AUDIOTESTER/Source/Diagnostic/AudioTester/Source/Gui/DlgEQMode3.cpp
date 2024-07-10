// DlgEQMode3.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgEQMode3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEQMode3 dialog


CDlgEQMode3::CDlgEQMode3(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEQMode3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEQMode3)
	//}}AFX_DATA_INIT
	m_bInit = FALSE;
	m_bFirstDraw = FALSE;
}


void CDlgEQMode3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEQMode3)
	DDX_Control(pDX, IDC_STATIC_F1F1_TREBLE_EQ, m_stcF1f1Treble);
	DDX_Control(pDX, IDC_STATIC_F1F1_LCF, m_stcF1f1Lcf);
	DDX_Control(pDX, IDC_STATIC_F1F1_BASS_EQ, m_stcF1f1Bass);
	DDX_Control(pDX, IDC_STATIC_F1F1_ALL, m_stcF1f1All);
	DDX_Text(pDX, IDC_EDT_F1_G0, m_ff1g0);
	DDV_MinMaxFloat(pDX, m_ff1g0, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDT_F1_G1, m_ff1g1);
	DDV_MinMaxFloat(pDX, m_ff1g1, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDT_F1_FP, m_nf1fp);
	DDV_MinMaxInt(pDX, m_nf1fp, 0, 22049);
	DDX_Text(pDX, IDC_EDT_F2_G0, m_ff2g0);
	DDV_MinMaxFloat(pDX, m_ff2g0, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDT_F2_G1, m_ff2g1);
	DDV_MinMaxFloat(pDX, m_ff2g1, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDT_F2_FP, m_nf2fp);
	DDV_MinMaxInt(pDX, m_nf2fp, 0, 22049);
	DDX_Text(pDX, IDC_EDT_BASS_F0, m_nBassf0);
	DDV_MinMaxInt(pDX, m_nBassf0, 0, 22050);
	DDX_Text(pDX, IDC_EDT_BASS_Q, m_fBassq);
	DDV_MinMaxFloat(pDX, m_fBassq, m_fMinq, m_fMaxq);
	DDX_Text(pDX, IDC_EDT_BASS_BOOST, m_fBassboost);
	DDV_MinMaxFloat(pDX, m_fBassboost, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDT_BASS_GAIN, m_fBassgain);
	DDV_MinMaxFloat(pDX, m_fBassgain, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDT_TREBLE_F0, m_nTreblef0);
	DDV_MinMaxInt(pDX, m_nTreblef0, 0, 22050);
	DDX_Text(pDX, IDC_EDT_TREBLE_Q, m_fTrebleq);
	DDV_MinMaxFloat(pDX, m_fTrebleq, m_fMinq, m_fMaxq);
	DDX_Text(pDX, IDC_EDT_TREBLE_BOOST, m_fTrebleboost);
	DDV_MinMaxFloat(pDX, m_fTrebleboost, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDT_TREBLE_GAIN, m_fTreblegain);
	DDV_MinMaxFloat(pDX, m_fTreblegain, -72.0, 18.0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEQMode3, CDialog)
	//{{AFX_MSG_MAP(CDlgEQMode3)
	ON_BN_CLICKED(IDC_BUTTON_F1F1_REFRESH, OnButtonF1f1Refresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEQMode3 message handlers

BOOL CDlgEQMode3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_stcF1f1Lcf.m_nType = CEqModeStatic::Draw_Lcf;	
	m_stcF1f1Bass.m_nType = CEqModeStatic::Draw_Bass;
	m_stcF1f1Treble.m_nType = CEqModeStatic::Draw_Treble;
	
	m_fMinq = 0;
	m_fMaxq = 64;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEQMode3::SetPara(EQ_MODE_NV_PARAM_T EQMode,short eqControl)
{
	int nVersion = GET_SOUND_EQ_VER( eqControl );
	m_stcF1f1All.m_nSoundEqVer = nVersion;
	m_stcF1f1Bass.m_nSoundEqVer = nVersion;
	m_stcF1f1Treble.m_nSoundEqVer = nVersion;
	m_stcF1f1Lcf.m_nSoundEqVer = nVersion;

	m_ff1g0 = (float)(EQMode.lcf_filter.f1_g0) / (float)10.0;
	m_ff1g0 = (short)(m_ff1g0 * 10) / (float)10.0;
	m_ff1g0 = m_ff1g0 > 18.0 ? 18.0 : m_ff1g0;
	m_ff1g0 = m_ff1g0 < -72.0 ? -72.0 : m_ff1g0;
	m_ff1g1 = (float)(EQMode.lcf_filter.f1_g1) / (float)10.0;
	m_ff1g1 = (short)(m_ff1g1 * 10) / 10.0;
	m_ff1g1 = m_ff1g1 > 18.0 ? 18.0 : m_ff1g1;
	m_ff1g1 = m_ff1g1 < -72.0 ? -72.0 : m_ff1g1;
	m_ff2g0 = (float)(EQMode.lcf_filter.f2_g0) / (float)10.0;
	m_ff2g0 = (short)(m_ff2g0 * 10) / 10.0;
	m_ff2g0 = m_ff2g0 > 18.0 ? 18.0 : m_ff2g0;
	m_ff2g0 = m_ff2g0 < -72.0 ? -72.0 : m_ff2g0;
	m_ff2g1 = (float)(EQMode.lcf_filter.f2_g1) / (float)10.0;
	m_ff2g1 = (short)(m_ff2g1 * 10) / 10.0;
	m_ff2g1 = m_ff2g1 > 18.0 ? 18.0 : m_ff2g1;
	m_ff2g1 = m_ff2g1 < -72.0 ? -72.0 : m_ff2g1;
	m_nf1fp = EQMode.lcf_filter.f1_fp;
	m_nf1fp = m_nf1fp < 0 ? 0 : m_nf1fp;
	m_nf1fp = m_nf1fp > 22049 ? 22049 : m_nf1fp;
	m_nf2fp = EQMode.lcf_filter.f2_fp;
	m_nf2fp = m_nf2fp < 0 ? 0 : m_nf2fp;
	m_nf2fp = m_nf2fp > 22049 ? 22049 : m_nf2fp;
	m_nBassf0 = EQMode.eq_bass.bass_f0;
	m_nBassf0 = m_nBassf0 < 0 ? 0 : m_nBassf0;
	m_nBassf0 = m_nBassf0 > 22050 ? 22050 : m_nBassf0;
	m_fBassboost = (float)EQMode.eq_bass.bass_boostdB / (float)10.0;
	m_fBassboost = (short)(m_fBassboost * 10) / 10.0;
	m_fBassboost = m_fBassboost > 18.0 ? 18.0 : m_fBassboost;
	m_fBassboost = m_fBassboost < -72.0 ? -72.0 : m_fBassboost;
	m_fBassgain = (float)EQMode.eq_bass.bass_gaindB / (float)10.0;
	m_fBassgain = (short)(m_fBassgain * 10) / 10.0;
	m_fBassgain = m_fBassgain > 18.0 ? 18.0 : m_fBassgain;
	m_fBassgain = m_fBassgain < -72.0 ? -72.0 : m_fBassgain;
	m_nTreblef0 = EQMode.eq_treble.treble_f0;
	m_nTreblef0 = m_nTreblef0 < 0 ? 0 : m_nTreblef0;
	m_nTreblef0 = m_nTreblef0 > 22050 ? 22050 : m_nTreblef0;
	m_fTrebleboost = (float)EQMode.eq_treble.treble_boostdB / (float)10.0;
	m_fTrebleboost = (short)(m_fTrebleboost * 10) / 10.0;
	m_fTrebleboost = m_fTrebleboost > 18.0 ? 18.0 : m_fTrebleboost;
	m_fTrebleboost = m_fTrebleboost < -72.0 ? -72.0 : m_fTrebleboost;
	m_fTreblegain = (float)EQMode.eq_treble.treble_gaindB / (float)10.0;
	m_fTreblegain = (short)(m_fTreblegain * 10) / 10.0;
	m_fTreblegain = m_fTreblegain > 18.0 ? 18.0 : m_fTreblegain;
	m_fTreblegain = m_fTreblegain < -72.0 ? -72.0 : m_fTreblegain;

	if( sound_eq_ver_3 == nVersion )
	{
		m_fTrebleq = (float)EQMode.eq_treble.treble_q / 512;
		m_fTrebleq = (short)(m_fTrebleq * 1000) / 1000.000;
		m_fBassq = (float)EQMode.eq_bass.bass_q / 512;
		m_fBassq = (short)(m_fBassq * 1000) / 1000.000;

		m_fMinq = 0;
		m_fMaxq = 64;

		GetDlgItem( IDC_STATIC_BASS_Q )->SetWindowText( _T("q") );
		GetDlgItem( IDC_STATIC_TREBLE_Q )->SetWindowText( _T("q") );
		GetDlgItem( IDC_STATIC_F1F1_BASS_Q )->SetWindowText( _T( " 0,64 " ) );
		GetDlgItem( IDC_STATIC_F1F1_TREBLE_Q )->SetWindowText( _T( " 0,64 " ) );
	}
	else
	{
		m_fTrebleq = EQMode.eq_treble.treble_q;
		m_fBassq = EQMode.eq_bass.bass_q;
		m_fMinq = 0;
		m_fMaxq = 22049;

		GetDlgItem( IDC_STATIC_BASS_Q )->SetWindowText( _T("df") );
		GetDlgItem( IDC_STATIC_TREBLE_Q )->SetWindowText( _T("df") );
		GetDlgItem( IDC_STATIC_F1F1_BASS_Q )->SetWindowText( _T( "Hz 0,22049" ) );
		GetDlgItem( IDC_STATIC_F1F1_TREBLE_Q )->SetWindowText( _T( "Hz 0,22049" ) );
	}

	UpdateData( FALSE );

	if( !m_bInit && m_bFirstDraw )
	{
		OnButtonF1f1Refresh();
	}
	m_bInit = TRUE;
}

BOOL CDlgEQMode3::GetPara(EQ_MODE_NV_PARAM_T &EQMode)
{
	if(!UpdateData())
	{
		return FALSE;
	}
	
	EQMode.lcf_filter.f1_g0 = (short)(m_ff1g0 * 10);
	EQMode.lcf_filter.f1_g1 = (short)(m_ff1g1 * 10);
	EQMode.lcf_filter.f2_g0 = (short)(m_ff2g0 * 10);
	EQMode.lcf_filter.f2_g1 = (short)(m_ff2g1 * 10);
	EQMode.lcf_filter.f1_fp = m_nf1fp;
	EQMode.lcf_filter.f2_fp = m_nf2fp;
	EQMode.eq_bass.bass_f0 = m_nBassf0;
	EQMode.eq_bass.bass_boostdB = (short)(m_fBassboost * 10);
	EQMode.eq_bass.bass_gaindB = (short)(m_fBassgain * 10);
	EQMode.eq_treble.treble_f0 = m_nTreblef0;
	EQMode.eq_treble.treble_boostdB = (short)(m_fTrebleboost * 10);
	EQMode.eq_treble.treble_gaindB = (short)(m_fTreblegain * 10);

 	if( sound_eq_ver_3 == m_stcF1f1Lcf.m_nSoundEqVer )
	{
		EQMode.eq_bass.bass_q = (short)(m_fBassq * 512);
		EQMode.eq_treble.treble_q = (short)(m_fTrebleq * 512);
	}
	else
	{
		EQMode.eq_bass.bass_q = (short)m_fBassq;
		EQMode.eq_treble.treble_q = (short)m_fTrebleq;
	}

	return TRUE;
}

void CDlgEQMode3::OnButtonF1f1Refresh() 
{
	EQ_MODE_NV_PARAM_T eqMode;
	if( !GetPara( eqMode ) )
	{
		// Input parameter is wrong,don't redraw curves
		return;
	}
	m_stcF1f1Treble.SetData( eqMode );
	m_stcF1f1Treble.Invalidate();
	m_stcF1f1Lcf.SetData( eqMode );
	m_stcF1f1Lcf.Invalidate();
	m_stcF1f1Bass.SetData( eqMode );
	m_stcF1f1Bass.Invalidate();
	m_stcF1f1All.SetData( eqMode );
	m_stcF1f1All.Invalidate();
}

void CDlgEQMode3::OnOK()
{
	return;
}

void CDlgEQMode3::ParentShow()
{
	if( !m_bFirstDraw && m_bInit )
	{
		OnButtonF1f1Refresh();
	}
	m_bFirstDraw = TRUE;
}

BOOL CDlgEQMode3::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message)
    {
        // Process when receive ENTER
        if (VK_RETURN == pMsg->wParam || pMsg->wParam == VK_ESCAPE)
        {
            return TRUE;
        }
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}