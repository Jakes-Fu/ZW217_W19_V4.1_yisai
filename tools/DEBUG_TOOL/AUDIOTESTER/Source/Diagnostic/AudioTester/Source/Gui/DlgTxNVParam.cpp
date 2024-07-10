// DlgTxNVParam.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgTxNVParam.h"
#include "DlgEqCoefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTxNVParam dialog


CDlgTxNVParam::CDlgTxNVParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTxNVParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTxNVParam)
		// NOTE: the ClassWizard will add member initialization here
	m_nCBOperatingMode = 0;
	m_nMode = 0;
	m_nMode2 = 0;
	m_nTuningMode = 0;
	m_nInputGainMic0 = 0;
	m_nInputGainMic1 = 0;
	m_nOutputGain = 0;
	m_nNLMS0LbTaps = 0;
	m_nNLMS0LbTwoalpha = 0;
	m_nNLMS0LbErl = 0;
	m_nNLMS0HbTaps = 0;
	m_nNLMS0HbTwoalpha = 0;
	m_nNLMS0HbErl = 0;
	m_nNLMS0PresetCoefs = 0;
	m_nNLMS0Offset = 0;
	m_nNLMS1LbTaps = 0;
	m_nNLMS1LbTwoalpha = 0;
	m_nNLMS1LbErl = 0;
	m_nNLMS1HbTaps = 0;
	m_nNLMS1HbTwoalpha = 0;
	m_nNLMS1HbErl = 0;
	m_nNLMS1PresetCoefs = 0;
	m_nNLMS1Offset = 0;
	m_nCalMicpowFloorMin = 0;
	m_nWgThreshold = 0;
	m_nMpThreshold = 0;
	m_nFsbTaps = 0;
	m_nFsbTwoalpha = 0;
	m_nFsbRefGain = 0;
	m_nGscTaps = 0;
	m_nGscTwoalpha = 0;
	m_nGscErl = 0;
	m_nGscOffset = 0;
	m_nDnnsEchoGammaHi = 0;
	m_nDnnsEchoGammaLo = 0;
	m_nDnnsEchoAlphaRev = 0;
	m_nDnnsEchoTailPortion = 0;
	m_nDnnsNlAtten = 0;
	m_nDnnsNoiseGammaS = 0;
	m_nDnnsNoiseGammaN = 0;
	m_nDnnsNoiseGainMinS = 0;
	m_nDnnsNoiseGainMinN = 0;
	m_nDnnsNoiseBiasComp = 0;
	m_nDnnsGainEta = 0;
	m_nDnnsAcThreshold = 0;
	m_nDnnsWbThreshold = 0;
	m_nDnnsLostBeamThreshold = 0;
	m_nPcdBeta = 0;
	m_nPcdThreshold = 0;
	//}}AFX_DATA_INIT
	memset(&m_NvControlParams, 0, sizeof(LVNV_ControlParams_st));
}


void CDlgTxNVParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTxNVParam)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_CBIndex(pDX, IDC_COMBO_TX_NV_OPER_MODE, m_nCBOperatingMode);
	DDX_Text(pDX, IDC_EDIT_TX_NV_MODE, m_nMode);
	DDV_MinMaxInt(pDX,m_nMode, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_TX_NV_MODE2, m_nMode2);
	DDV_MinMaxInt(pDX,m_nMode2, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_TUNING_MODE, m_nTuningMode);
	DDV_MinMaxInt(pDX,m_nTuningMode, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_TX_NV_INPUT_GAIN_MIC0, m_nInputGainMic0);
	DDV_MinMaxInt(pDX,m_nInputGainMic0, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_INPUT_GAIN_MIC1, m_nInputGainMic1);
	DDV_MinMaxInt(pDX,m_nInputGainMic1, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_OUTPUT_GAIN, m_nOutputGain);
	DDV_MinMaxInt(pDX,m_nOutputGain, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS0_LB_TAPS, m_nNLMS0LbTaps);
	DDV_MinMaxInt(pDX,m_nNLMS0LbTaps, 16, 64);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS0_LB_TWOALPHA, m_nNLMS0LbTwoalpha);
	DDV_MinMaxInt(pDX,m_nNLMS0LbTwoalpha, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS0_LB_ERL, m_nNLMS0LbErl);
	DDV_MinMaxInt(pDX,m_nNLMS0LbErl, 64, 4096);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS0_HB_TAPS, m_nNLMS0HbTaps);
	DDV_MinMaxInt(pDX,m_nNLMS0HbTaps, 16, 64);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS0_HB_TWOALPHA, m_nNLMS0HbTwoalpha);
	DDV_MinMaxInt(pDX,m_nNLMS0HbTwoalpha, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS0_HB_ERL, m_nNLMS0HbErl);
	DDV_MinMaxInt(pDX,m_nNLMS0HbErl, 64, 4096);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS0_PRESET_COEFS, m_nNLMS0PresetCoefs);
	DDV_MinMaxInt(pDX,m_nNLMS0PresetCoefs, 0, 2);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS0_OFFSET, m_nNLMS0Offset);
	DDV_MinMaxInt(pDX,m_nNLMS0Offset, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS1_LB_TAPS, m_nNLMS1LbTaps);
	DDV_MinMaxInt(pDX,m_nNLMS1LbTaps, 16, 32);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS1_LB_TWOALPHA, m_nNLMS1LbTwoalpha);
	DDV_MinMaxInt(pDX,m_nNLMS1LbTwoalpha, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS1_LB_ERL, m_nNLMS1LbErl);
	DDV_MinMaxInt(pDX,m_nNLMS1LbErl, 64, 4096);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS1_HB_TAPS, m_nNLMS1HbTaps);
	DDV_MinMaxInt(pDX,m_nNLMS1HbTaps, 16, 32);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS1_HB_TWOALPHA, m_nNLMS1HbTwoalpha);
	DDV_MinMaxInt(pDX,m_nNLMS1HbTwoalpha, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS1_HB_ERL, m_nNLMS1HbErl);
	DDV_MinMaxInt(pDX,m_nNLMS1HbErl, 64, 4096);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS1_PRESET_COEFS, m_nNLMS1PresetCoefs);
	DDV_MinMaxInt(pDX,m_nNLMS1PresetCoefs, 0, 2);
	DDX_Text(pDX, IDC_EDIT_TX_NV_NLMS1_OFFSET, m_nNLMS1Offset);
	DDV_MinMaxInt(pDX,m_nNLMS1Offset, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_CAL_MICPOWFLOOR, m_nCalMicpowFloorMin);
	DDV_MinMaxInt(pDX,m_nCalMicpowFloorMin, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_WGTHRESHOLD, m_nWgThreshold);
	DDV_MinMaxInt(pDX,m_nWgThreshold, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_MPTHRESHOLD, m_nMpThreshold);
	DDV_MinMaxInt(pDX,m_nMpThreshold, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_FSB_TAPS, m_nFsbTaps);
	DDV_MinMaxInt(pDX,m_nFsbTaps, 8, 16);
	DDX_Text(pDX, IDC_EDIT_TX_NV_FSB_TWOALPHA, m_nFsbTwoalpha);
	DDV_MinMaxInt(pDX,m_nFsbTwoalpha, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_FSB_REF_GAIN, m_nFsbRefGain);
	DDV_MinMaxInt(pDX,m_nMode, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_GSC_TAPS, m_nGscTaps);
	DDV_MinMaxInt(pDX,m_nGscTaps, 8, 48);
	DDX_Text(pDX, IDC_EDIT_TX_NV_GSC_TWOALPHA, m_nGscTwoalpha);
	DDV_MinMaxInt(pDX,m_nGscTwoalpha, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_GSC_ERL, m_nGscErl);
	DDV_MinMaxInt(pDX,m_nGscErl, 64, 4096);
	DDX_Text(pDX, IDC_EDIT_TX_NV_GSC_OFFSET, m_nGscOffset);
	DDV_MinMaxInt(pDX,m_nGscOffset, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_ECHOGAMMAHI, m_nDnnsEchoGammaHi);
	DDV_MinMaxInt(pDX,m_nDnnsEchoGammaHi, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_ECHOGAMMALO, m_nDnnsEchoGammaLo);
	DDV_MinMaxInt(pDX,m_nDnnsEchoGammaLo, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_ECHOALPHAREV, m_nDnnsEchoAlphaRev);
	DDV_MinMaxInt(pDX,m_nDnnsEchoAlphaRev, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_ECHOTAILPORTION, m_nDnnsEchoTailPortion);
	DDV_MinMaxInt(pDX,m_nDnnsEchoTailPortion, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_NLATTEN, m_nDnnsNlAtten);
	DDV_MinMaxInt(pDX,m_nDnnsNlAtten, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_NOISEGAMMAS, m_nDnnsNoiseGammaS);
	DDV_MinMaxInt(pDX,m_nDnnsNoiseGammaS, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_NOISEGAMMAN, m_nDnnsNoiseGammaN);
	DDV_MinMaxInt(pDX,m_nDnnsNoiseGammaN, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_NOISEGAINMINS, m_nDnnsNoiseGainMinS);
	DDV_MinMaxInt(pDX,m_nDnnsNoiseGainMinS, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_NOISEGAINMINN, m_nDnnsNoiseGainMinN);
	DDV_MinMaxInt(pDX,m_nDnnsNoiseGainMinN, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_NOISEBIASCOMP, m_nDnnsNoiseBiasComp);
	DDV_MinMaxInt(pDX,m_nDnnsNoiseBiasComp, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_GAINETA, m_nDnnsGainEta);
	DDV_MinMaxInt(pDX,m_nDnnsGainEta, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_ACTHRESHOLD, m_nDnnsAcThreshold);
	DDV_MinMaxInt(pDX,m_nDnnsAcThreshold, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_WBTHRESHOLD, m_nDnnsWbThreshold);
	DDV_MinMaxInt(pDX,m_nDnnsWbThreshold, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_DNNS_LOSTBEAMTHRESHOLD, m_nDnnsLostBeamThreshold);
	DDV_MinMaxInt(pDX,m_nDnnsLostBeamThreshold, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_PCD_BETA, m_nPcdBeta);
	DDV_MinMaxInt(pDX,m_nPcdBeta, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_TX_NV_PCD_THRESHOLD, m_nPcdThreshold);
	DDV_MinMaxInt(pDX,m_nPcdThreshold, 0, 32767);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTxNVParam, CDialog)
	//{{AFX_MSG_MAP(CDlgTxNVParam)
	ON_BN_CLICKED(IDC_BUTTON_TX_NV_FSB_INIT_TABLE0, OnButtonTxNvFsbInitTable0)
	ON_BN_CLICKED(IDC_BUTTON_TX_NV_FSB_INIT_TABLE1, OnButtonTxNvFsbInitTable1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTxNVParam message handlers

void CDlgTxNVParam::OnButtonTxNvFsbInitTable0() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgEqCoefs dlg;
	dlg.SetDlgTitle(_T("FSB_init_table0"));
	dlg.SetItemValue(m_NvControlParams.FSB_init_table0, 8);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetItemValue(m_NvControlParams.FSB_init_table0);
	}
}

void CDlgTxNVParam::OnButtonTxNvFsbInitTable1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgEqCoefs dlg;
	dlg.SetDlgTitle(_T("FSB_init_table1"));
	dlg.SetItemValue(m_NvControlParams.FSB_init_table1, 8);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetItemValue(m_NvControlParams.FSB_init_table1);
	}
}

void CDlgTxNVParam::SetValue(LVNV_ControlParams_st NvControlParams)
{
	memcpy(&m_NvControlParams, &NvControlParams, sizeof(LVNV_ControlParams_st));
}

void CDlgTxNVParam::FetchValue(LVNV_ControlParams_st& NvControlParams)
{
	memcpy(&NvControlParams, &m_NvControlParams, sizeof(LVNV_ControlParams_st));
}

void CDlgTxNVParam::InitValue(LVNV_ControlParams_st NvControlParams)
{
	m_nCBOperatingMode = NvControlParams.OperatingMode;
	if(m_nCBOperatingMode == LVM_MAXENUM)
	{
		m_nCBOperatingMode = 2;
	}
	m_nMode = NvControlParams.Mode;
	m_nMode2 = NvControlParams.Mode2;
	m_nTuningMode = NvControlParams.Tuning_mode;
	m_nInputGainMic0 = NvControlParams.Input_Gain_Mic0;
	m_nInputGainMic1 = NvControlParams.Input_Gain_Mic1;
	m_nOutputGain = NvControlParams.Output_Gain;
	m_nNLMS0LbTaps = NvControlParams.NLMS0_LB_taps;
	m_nNLMS0LbTwoalpha = NvControlParams.NLMS0_LB_twoalpha;
	m_nNLMS0LbErl = NvControlParams.NLMS0_LB_erl;
	m_nNLMS0HbTaps = NvControlParams.NLMS0_HB_taps;
	m_nNLMS0HbTwoalpha = NvControlParams.NLMS0_HB_twoalpha;
	m_nNLMS0HbErl = NvControlParams.NLMS0_HB_erl;
	m_nNLMS0PresetCoefs = NvControlParams.NLMS0_preset_coefs;
	m_nNLMS0Offset = NvControlParams.NLMS0_offset;
	m_nNLMS1LbTaps = NvControlParams.NLMS1_LB_taps;
	m_nNLMS1LbTwoalpha = NvControlParams.NLMS1_LB_twoalpha;
	m_nNLMS1LbErl = NvControlParams.NLMS1_LB_erl;
	m_nNLMS1HbTaps = NvControlParams.NLMS1_HB_taps;
	m_nNLMS1HbTwoalpha = NvControlParams.NLMS1_HB_twoalpha;
	m_nNLMS1HbErl = NvControlParams.NLMS1_HB_erl;
	m_nNLMS1PresetCoefs = NvControlParams.NLMS1_preset_coefs;
	m_nNLMS1Offset = NvControlParams.NLMS1_offset;
	m_nCalMicpowFloorMin = NvControlParams.CAL_micPowFloorMin;
	m_nWgThreshold = NvControlParams.WgThreshold;
	m_nMpThreshold = NvControlParams.MpThreshold;
	m_nFsbTaps = NvControlParams.FSB_taps;
	m_nFsbTwoalpha = NvControlParams.FSB_twoalpha;
	m_nFsbRefGain = NvControlParams.FSB_ref_gain;
	m_nGscTaps = NvControlParams.GSC_taps;
	m_nGscTwoalpha = NvControlParams.GSC_twoalpha;
	m_nGscErl = NvControlParams.GSC_erl;
	m_nGscOffset = NvControlParams.GSC_offset;
	m_nDnnsEchoGammaHi = NvControlParams.DNNS_EchoGammaHi;
	m_nDnnsEchoGammaLo = NvControlParams.DNNS_EchoGammaLo;
	m_nDnnsEchoAlphaRev = NvControlParams.DNNS_EchoAlphaRev;
	m_nDnnsEchoTailPortion = NvControlParams.DNNS_EchoTailPortion;
	m_nDnnsNlAtten = NvControlParams.DNNS_NlAtten;
	m_nDnnsNoiseGammaS = NvControlParams.DNNS_NoiseGammaS;
	m_nDnnsNoiseGammaN = NvControlParams.DNNS_NoiseGammaN;
	m_nDnnsNoiseGainMinS = NvControlParams.DNNS_NoiseGainMinS;
	m_nDnnsNoiseGainMinN = NvControlParams.DNNS_NoiseGainMinN;
	m_nDnnsNoiseBiasComp = NvControlParams.DNNS_NoiseBiasComp;
	m_nDnnsGainEta = NvControlParams.DNNS_GainEta;
	m_nDnnsAcThreshold = NvControlParams.DNNS_AcThreshold;
	m_nDnnsWbThreshold = NvControlParams.DNNS_WbThreshold;
	m_nDnnsLostBeamThreshold = NvControlParams.DNNS_LostBeamThreshold;
	m_nPcdBeta = NvControlParams.PCD_beta;
	m_nPcdThreshold = NvControlParams.PCD_Threshold;

	UpdateData(FALSE);
}

void CDlgTxNVParam::GetValue(LVNV_ControlParams_st& NvControlParams)
{
	UpdateData();

	if(m_nCBOperatingMode == 2)
	{
		m_nCBOperatingMode = LVM_MAXENUM;
	}
	NvControlParams.OperatingMode = m_nCBOperatingMode;

	NvControlParams.Mode = m_nMode;
	NvControlParams.Mode2 = m_nMode2;
	NvControlParams.Tuning_mode = m_nTuningMode;
	NvControlParams.Input_Gain_Mic0 = m_nInputGainMic0;
	NvControlParams.Input_Gain_Mic1 = m_nInputGainMic1;
	NvControlParams.Output_Gain = m_nOutputGain;
	NvControlParams.NLMS0_LB_taps = m_nNLMS0LbTaps;
	NvControlParams.NLMS0_LB_twoalpha = m_nNLMS0LbTwoalpha;
	NvControlParams.NLMS0_LB_erl = m_nNLMS0LbErl;
	NvControlParams.NLMS0_HB_taps = m_nNLMS0HbTaps;
	NvControlParams.NLMS0_HB_twoalpha = m_nNLMS0HbTwoalpha;
	NvControlParams.NLMS0_HB_erl = m_nNLMS0HbErl;
	NvControlParams.NLMS0_preset_coefs = m_nNLMS0PresetCoefs;
	NvControlParams.NLMS0_offset = m_nNLMS0Offset;
	NvControlParams.NLMS1_LB_taps = m_nNLMS1LbTaps;
	NvControlParams.NLMS1_LB_twoalpha = m_nNLMS1LbTwoalpha;
	NvControlParams.NLMS1_LB_erl = m_nNLMS1LbErl;
	NvControlParams.NLMS1_HB_taps = m_nNLMS1HbTaps;
	NvControlParams.NLMS1_HB_twoalpha = m_nNLMS1HbTwoalpha;
	NvControlParams.NLMS1_HB_erl = m_nNLMS1HbErl;
	NvControlParams.NLMS1_preset_coefs = m_nNLMS1PresetCoefs;
	NvControlParams.NLMS1_offset = m_nNLMS1Offset;
	NvControlParams.CAL_micPowFloorMin = m_nCalMicpowFloorMin;
	NvControlParams.WgThreshold = m_nWgThreshold;
	NvControlParams.MpThreshold = m_nMpThreshold;
	NvControlParams.FSB_taps = m_nFsbTaps;
	NvControlParams.FSB_twoalpha = m_nFsbTwoalpha;
	NvControlParams.FSB_ref_gain = m_nFsbRefGain;
	NvControlParams.GSC_taps = m_nGscTaps;
	NvControlParams.GSC_twoalpha = m_nGscTwoalpha;
	NvControlParams.GSC_erl = m_nGscErl;
	NvControlParams.GSC_offset = m_nGscOffset;
	NvControlParams.DNNS_EchoGammaHi = m_nDnnsEchoGammaHi;
	NvControlParams.DNNS_EchoGammaLo = m_nDnnsEchoGammaLo;
	NvControlParams.DNNS_EchoAlphaRev = m_nDnnsEchoAlphaRev;
	NvControlParams.DNNS_EchoTailPortion = m_nDnnsEchoTailPortion;
	NvControlParams.DNNS_NlAtten = m_nDnnsNlAtten;
	NvControlParams.DNNS_NoiseGammaS = m_nDnnsNoiseGammaS;
	NvControlParams.DNNS_NoiseGammaN = m_nDnnsNoiseGammaN;
	NvControlParams.DNNS_NoiseGainMinS = m_nDnnsNoiseGainMinS;
	NvControlParams.DNNS_NoiseGainMinN = m_nDnnsNoiseGainMinN;
	NvControlParams.DNNS_NoiseBiasComp = m_nDnnsNoiseBiasComp;
	NvControlParams.DNNS_GainEta = m_nDnnsGainEta;
	NvControlParams.DNNS_AcThreshold = m_nDnnsAcThreshold;
	NvControlParams.DNNS_WbThreshold = m_nDnnsWbThreshold;
	NvControlParams.DNNS_LostBeamThreshold = m_nDnnsLostBeamThreshold;
	NvControlParams.PCD_beta = m_nPcdBeta;
	NvControlParams.PCD_Threshold = m_nPcdThreshold;

}

BOOL CDlgTxNVParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitValue(m_NvControlParams);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTxNVParam::OnOK() 
{
	// TODO: Add extra validation here
	GetValue(m_NvControlParams);
	
	CDialog::OnOK();
}
