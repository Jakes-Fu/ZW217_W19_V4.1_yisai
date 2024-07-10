// DlgEQMode2.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgEQMode2.h"

#define MIN_FP_LR 1
#define MAX_FP_LR 22049

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEQMode2 dialog


CDlgEQMode2::CDlgEQMode2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEQMode2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEQMode2)
	m_nFPL = 0;
	m_nFPR = 0;
	m_nLcfGainL = 0;
	m_nLcfGainR = 0;
	//}}AFX_DATA_INIT
	m_bInit = FALSE;
	m_bFirstDraw = FALSE;
}


void CDlgEQMode2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEQMode2)
	DDX_Control(pDX, IDC_STATIC_BUTTERWORTH_TREBLE_EQ, m_stcTreble);
	DDX_Control(pDX, IDC_STATIC_BUTTERWORTH_LCF_L_R, m_stcLcf);
	DDX_Control(pDX, IDC_STATIC_BUTTERWORTH_BASS_EQ, m_stcBass);
	DDX_Control(pDX, IDC_STATIC_BUTTERWORTH_ALL, m_stcAll);
	DDX_Text(pDX, IDC_EDT_FP_L, m_nFPL);
	DDV_MinMaxInt(pDX, m_nFPL, MIN_FP_LR, MAX_FP_LR);
	DDX_Text(pDX, IDC_EDT_FP_R, m_nFPR);
	DDV_MinMaxInt(pDX, m_nFPR, MIN_FP_LR, MAX_FP_LR);
	DDX_Text(pDX, IDC_EDT_BASS_F0, m_nBassf0);
	DDV_MinMaxInt(pDX, m_nBassf0, 0, 22050);
	DDX_Text(pDX, IDC_EDT_BASS_Q, m_fBassq);
	DDV_MinMaxFloat(pDX, m_fBassq, 0, 22049);
	DDX_Text(pDX, IDC_EDT_BASS_BOOST, m_fBassboost);
	DDV_MinMaxFloat(pDX, m_fBassboost, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDT_BASS_GAIN, m_fBassgain);
	DDV_MinMaxFloat(pDX, m_fBassgain, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDT_TREBLE_F0, m_nTreblef0);
	DDV_MinMaxInt(pDX, m_nTreblef0, 0, 22050);
	DDX_Text(pDX, IDC_EDT_TREBLE_Q, m_fTrebleq);
	DDV_MinMaxFloat(pDX, m_fTrebleq, 0, 22049);
	DDX_Text(pDX, IDC_EDT_TREBLE_BOOST, m_fTrebleboost);
	DDV_MinMaxFloat(pDX, m_fTrebleboost, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDT_TREBLE_GAIN, m_fTreblegain);
	DDV_MinMaxFloat(pDX, m_fTreblegain, -72.0, 18.0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEQMode2, CDialog)
	//{{AFX_MSG_MAP(CDlgEQMode2)
	ON_BN_CLICKED(IDC_BUTTON_BUTTERWORTH_REFRESH, OnButtonButterworthRefresh)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEQMode2 message handlers

BOOL CDlgEQMode2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_stcLcf.m_nType = CEqModeStatic::Draw_Lcf_L_R;
	m_stcBass.m_nType = CEqModeStatic::Draw_Bass;
	m_stcTreble.m_nType = CEqModeStatic::Draw_Treble;
	m_stcAll.m_nType = CEqModeStatic::Draw_All_L_R;	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEQMode2::SetPara(EQ_MODE_NV_PARAM_T EQMode,BUTTERWORTH_LCF_T lcf_data)
{
	m_nBassf0 = EQMode.eq_bass.bass_f0;
	m_nBassf0 = m_nBassf0 < 0 ? 0 : m_nBassf0;
	m_nBassf0 = m_nBassf0 > 22050 ? 22050 : m_nBassf0;
	m_fBassq = EQMode.eq_bass.bass_q;
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
	m_fTrebleq = EQMode.eq_treble.treble_q;
	m_fTrebleboost = (float)EQMode.eq_treble.treble_boostdB / (float)10.0;
	m_fTrebleboost = (short)(m_fTrebleboost * 10) / 10.0;
	m_fTrebleboost = m_fTrebleboost > 18.0 ? 18.0 : m_fTrebleboost;
	m_fTrebleboost = m_fTrebleboost < -72.0 ? -72.0 : m_fTrebleboost;
	m_fTreblegain = (float)EQMode.eq_treble.treble_gaindB / (float)10.0;
	m_fTreblegain = (short)(m_fTreblegain * 10) / 10.0;
	m_fTreblegain = m_fTreblegain > 18.0 ? 18.0 : m_fTreblegain;
	m_fTreblegain = m_fTreblegain < -72.0 ? -72.0 : m_fTreblegain;

	m_nFPL = lcf_data.fp_l;
	m_nFPR = lcf_data.fp_r;	

	if( m_nFPL < MIN_FP_LR )
	{
		m_nFPL = MIN_FP_LR;
	}
	if( m_nFPL > MAX_FP_LR )
	{
		m_nFPL = MAX_FP_LR;
	}
	if( m_nFPR < MIN_FP_LR )
	{
		m_nFPR = MIN_FP_LR;
	}
	if( m_nFPR > MAX_FP_LR )
	{
		m_nFPR = MAX_FP_LR;
	}
	
	// These two value are used to draw curves,so use the original value read from phone
	m_nLcfGainL = lcf_data.lcf_gain_l;
	m_nLcfGainR = lcf_data.lcf_gain_r;

	UpdateData( FALSE );

	if( !m_bInit && m_bFirstDraw )
	{
		OnButtonButterworthRefresh();
	}
	m_bInit = TRUE;
}
	
BOOL CDlgEQMode2::GetPara(EQ_MODE_NV_PARAM_T &EQMode,BUTTERWORTH_LCF_T& lcf_data)
{
	if(!UpdateData())
	{
		return FALSE;
	}

	EQMode.eq_bass.bass_f0 = m_nBassf0;
	EQMode.eq_bass.bass_q = m_fBassq;
	EQMode.eq_bass.bass_boostdB = (short)(m_fBassboost * 10);
	EQMode.eq_bass.bass_gaindB = (short)(m_fBassgain * 10);
	EQMode.eq_treble.treble_f0 = m_nTreblef0;
	EQMode.eq_treble.treble_q = m_fTrebleq;
	EQMode.eq_treble.treble_boostdB = (short)(m_fTrebleboost * 10);
	EQMode.eq_treble.treble_gaindB = (short)(m_fTreblegain * 10);

	lcf_data.fp_l = m_nFPL;
	lcf_data.fp_r = m_nFPR;
	lcf_data.lcf_gain_l = m_nLcfGainL;
	lcf_data.lcf_gain_r = m_nLcfGainR;

	return TRUE;
}

void CDlgEQMode2::OnButtonButterworthRefresh() 
{
	EQ_MODE_NV_PARAM_T eqMode;
	BUTTERWORTH_LCF_T lcf_data;
	if( !GetPara( eqMode,lcf_data ) )
	{
		// Input value is wrong,don't redraw curves
		return;
	}

	eqMode.lcf_filter.f1_fp = lcf_data.fp_l;
	eqMode.lcf_filter.f1_g0 = lcf_data.lcf_gain_l;
	eqMode.lcf_filter.f2_fp = lcf_data.fp_r;
	eqMode.lcf_filter.f2_g0 = lcf_data.lcf_gain_r;

	m_stcLcf.SetData( eqMode );
	m_stcLcf.Invalidate();
	m_stcBass.SetData( eqMode );
	m_stcBass.Invalidate();
	m_stcTreble.SetData( eqMode );
	m_stcTreble.Invalidate();
	m_stcAll.SetData( eqMode );
	m_stcAll.Invalidate();
}

void CDlgEQMode2::UpdateGain( double lcf_gain_l,double lcf_gain_r )
{ 
	m_nLcfGainL = 16384 * pow( 10,lcf_gain_l / 20.0 );
	m_nLcfGainR = 16384 * pow( 10,lcf_gain_r / 20.0 );
}

HBRUSH CDlgEQMode2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_LEFT )
	{
		return CreateSolidBrush( RGB( 0,255,0 ) );
	}
	else if( pWnd->GetDlgCtrlID() == IDC_STATIC_RIGHT )
	{
		return CreateSolidBrush( RGB( 0,0,255 ) );
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgEQMode2::ParentShow()
{
	if( !m_bFirstDraw && m_bInit )
	{
		OnButtonButterworthRefresh();
	}
	m_bFirstDraw = TRUE;
}

BOOL CDlgEQMode2::PreTranslateMessage(MSG* pMsg) 
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