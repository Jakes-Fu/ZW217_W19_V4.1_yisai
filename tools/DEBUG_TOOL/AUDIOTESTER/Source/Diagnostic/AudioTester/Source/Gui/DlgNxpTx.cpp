// DlgNxpTx.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgNxpTx.h"
#include "DlgEqCoefs.h"
#include "Command.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNxpTx dialog


CDlgNxpTx::CDlgNxpTx(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNxpTx::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNxpTx)
		// NOTE: the ClassWizard will add member initialization here
	m_nCBGeneralOperMode = 0;
	m_nCBMute = 0;
	m_nCBVolMode = 0;
	m_nCBBdMode = 0;
	m_nCBHpfMode = 0;
	m_nCBOperMode = 0;
	m_nCBWhisperOperMode = 0;
	m_nCBEqOperatingMod = 0;
	m_nBulkDelay = 80;
	m_nVolGain = -96;
	m_nMode = 0;
	m_nHpfFreq = 50;
	m_nTuningMode = 0;
	m_nOutputGain = 0;
	m_nInputGain = 0;
	m_nNLMSLimit = -24;
	m_nNLMSLbTaps = 16;
	m_nNLMSLbAlpha = 0;
	m_nPcdThreshold = 0;
	m_nPcdErl = 64;
	m_nPcdErlStep = 0;
	m_nPcdTaps = 16;
	m_nPcdMinimumErl = 64;
	m_nPcdGammaERescue = 0;
	m_nWhisperMode = 0;
	m_nNLMSLbErl = 0;
	m_nTargetLevelLi = 0;
	m_nAvlMaxgainlin = 0;
	m_nAvlMingainlin = 0;
	m_nAvlAttack = 0;
	m_nAvlLimitMaxoutput = 0;
	m_nAvlRelease = 0;
	m_nSpdetectThreshold = 0;
	m_nNLMSHbTaps = 0;
	m_nEqLength = 0;
	m_nNLMSHbAlpha = 0;
	m_nNLMSHbErl = 0;
	m_nNLMSPresetCoefs = 0;
	m_nNLMSOffset = 0;
	m_nSpdetNear = 0;
	m_nSpdetAct = 0;
	m_nNfeBlocks = 0;
	m_nGammaAlpha = 0;
	m_nGammaN = 0;
	m_nLimitNs = 0;
	m_nCniGain = 0;
	m_nNlAtten = 0;
	m_nGammaLow = 0;
	m_nGammaDt = 0;
	m_nGammaHigh = 0;
	m_nTailPortion = 0;
	m_nTailAlpha = 0;
	m_nSpdetFar = 0;
	m_nSpdetMic = 0;
	m_nSpdetXClip = 0;
	m_nCBDrcOperatingMode = 0;
	m_nDrcNumknees = 1;
	m_nDrcAttackTime = 0;
	m_nDrcReleaseTime = 0;
	m_nCBDrcLimiterOperatingMode = 0;
	m_nDrcLimitLevel = 0;
	//}}AFX_DATA_INIT
	memset(&m_lvve_data, 0, sizeof(LVVE_CTRL_PARAM_NV_T));
	m_dwLVVEType = LVVE_RESERVE;
}


void CDlgNxpTx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNxpTx)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_CBIndex(pDX,IDC_COMBO_TX_GENERAL_OPER_MODE,m_nCBGeneralOperMode);
	DDX_CBIndex(pDX,IDC_COMBO_TX_MUTE,m_nCBMute);
	DDX_CBIndex(pDX,IDC_COMBO_TX_VOL_MODE,m_nCBVolMode);
	DDX_CBIndex(pDX,IDC_COMBO_TX_BD_MODE,m_nCBBdMode);
	DDX_CBIndex(pDX,IDC_COMBO_TX_HPF_MODE,m_nCBHpfMode);
	DDX_CBIndex(pDX,IDC_COMBO_TX_OPER_MODE,m_nCBOperMode);
	DDX_CBIndex(pDX,IDC_COMBO_TX_WHISPER_OPER_MODE,m_nCBWhisperOperMode);
	DDX_CBIndex(pDX,IDC_COMBO_TX_EQ_OPERATINGMod,m_nCBEqOperatingMod);
	DDX_CBIndex(pDX, IDC_COMBO_TX_DRC_OPER_MODE, m_nCBDrcOperatingMode);
	DDX_CBIndex(pDX, IDC_COMBO_TX_DRC_LIMITEROPER_MODE, m_nCBDrcLimiterOperatingMode);
	DDX_Text(pDX,IDC_EDIT_TX_BULK_DELAY,m_nBulkDelay);
	//DDV_MinMaxInt(pDX,m_nBulkDelay,80,3200);
	DDX_Text(pDX,IDC_EDIT_TX_VOL_GAIN,m_nVolGain);
	DDV_MinMaxInt(pDX,m_nVolGain,-96,2504);
	DDX_Text(pDX,IDC_EDIT_TX_MODE,m_nMode);
	DDV_MinMaxInt(pDX,m_nMode,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_HPF_FREQ,m_nHpfFreq);
	DDV_MinMaxInt(pDX,m_nHpfFreq,50,1000);
	DDX_Text(pDX,IDC_EDIT_TX_TUNING_MODE,m_nTuningMode);
	DDV_MinMaxInt(pDX,m_nTuningMode,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_UOTPUT_GAIN,m_nOutputGain);
	DDV_MinMaxInt(pDX,m_nOutputGain,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_INPUT_GAIN,m_nInputGain);
	DDV_MinMaxInt(pDX,m_nInputGain,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_NLMS_LIMIT,m_nNLMSLimit);
	DDV_MinMaxInt(pDX,m_nNLMSLimit,-24,0);
	DDX_Text(pDX,IDC_EDIT_TX_NLMS_LB_TAPS,m_nNLMSLbTaps);
	DDV_MinMaxInt(pDX,m_nNLMSLbTaps,16,200);
	DDX_Text(pDX,IDC_EDIT_TX_NLMS_LB_ALPHA,m_nNLMSLbAlpha);
	DDV_MinMaxInt(pDX,m_nNLMSLbAlpha,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_PCD_THRESHOLD,m_nPcdThreshold);
	DDV_MinMaxInt(pDX,m_nPcdThreshold,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_PCD_ERL,m_nPcdErl);
	//DDV_MinMaxInt(pDX,m_nPcdErl,64,32767);
	DDX_Text(pDX,IDC_EDIT_TX_PCD_ERL_STEP,m_nPcdErlStep);
	DDV_MinMaxInt(pDX,m_nPcdErlStep,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_PCD_TAPS,m_nPcdTaps);
	DDV_MinMaxInt(pDX,m_nPcdTaps,16,64);
	DDX_Text(pDX,IDC_EDIT_TX_PCD_MINIMUM_ERL,m_nPcdMinimumErl);
	//DDV_MinMaxInt(pDX,m_nPcdMinimumErl,64,32767);
	DDX_Text(pDX,IDC_EDIT_TX_PCD_GAMMA_E_RESCUE,m_nPcdGammaERescue);
	DDV_MinMaxInt(pDX,m_nPcdGammaERescue,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_WHISPER_MODE,m_nWhisperMode);
	DDV_MinMaxInt(pDX,m_nWhisperMode,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_NLMS_LB_ERL,m_nNLMSLbErl);
	DDV_MinMaxInt(pDX,m_nNLMSLbErl,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_TARGET_LEVEL_LI,m_nTargetLevelLi);
	DDV_MinMaxInt(pDX,m_nTargetLevelLi,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_AVL_MAXGAINLIN,m_nAvlMaxgainlin);
	DDV_MinMaxInt(pDX,m_nAvlMaxgainlin,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_AVL_MINGAINLIN,m_nAvlMingainlin);
	DDV_MinMaxInt(pDX,m_nAvlMingainlin,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_AVL_ATTACK,m_nAvlAttack);
	DDV_MinMaxInt(pDX,m_nAvlAttack,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_AVL_LIMIT_MAXOUTPUT,m_nAvlLimitMaxoutput);
	DDV_MinMaxInt(pDX,m_nAvlLimitMaxoutput,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_AVL_RELEASE,m_nAvlRelease);
	DDV_MinMaxInt(pDX,m_nAvlRelease,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_SPDETECT_THRESHOLD,m_nSpdetectThreshold);
	DDV_MinMaxInt(pDX,m_nSpdetectThreshold,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_NLMS_HB_TAPS,m_nNLMSHbTaps);
	DDV_MinMaxInt(pDX,m_nNLMSHbTaps,0,200);
	DDX_Text(pDX,IDC_EDIT_TX_EQ_LENGTH,m_nEqLength);
	DDV_MinMaxInt(pDX,m_nEqLength,0,32);
	DDX_Text(pDX,IDC_EDIT_TX_NLMS_HB_ALPHA,m_nNLMSHbAlpha);
	DDV_MinMaxInt(pDX,m_nNLMSHbAlpha,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_NLMS_HB_ERL,m_nNLMSHbErl);
	DDV_MinMaxInt(pDX,m_nNLMSHbErl,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_NLMS_PRESET_COEFS,m_nNLMSPresetCoefs);
	DDV_MinMaxInt(pDX,m_nNLMSPresetCoefs,0,2);
	DDX_Text(pDX,IDC_EDIT_TX_NLMS_OFFSET,m_nNLMSOffset);
	DDV_MinMaxInt(pDX,m_nNLMSOffset,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_SPDET_NEAR,m_nSpdetNear);
	DDV_MinMaxInt(pDX,m_nSpdetNear,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_SPDET_ACT,m_nSpdetAct);
	DDV_MinMaxInt(pDX,m_nSpdetAct,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_NFE_BLOCKS,m_nNfeBlocks);
	DDV_MinMaxInt(pDX,m_nNfeBlocks,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_GAMMA_ALPHA,m_nGammaAlpha);
	DDV_MinMaxInt(pDX,m_nGammaAlpha,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_GAMMA_N,m_nGammaN);
	DDV_MinMaxInt(pDX,m_nGammaN,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_LIMIT_NS,m_nLimitNs);
	DDV_MinMaxInt(pDX,m_nLimitNs,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_CNI_GAIN,m_nCniGain);
	DDV_MinMaxInt(pDX,m_nCniGain,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_NL_ATTEN,m_nNlAtten);
	DDV_MinMaxInt(pDX,m_nNlAtten,0,2048);
	DDX_Text(pDX,IDC_EDIT_TX_GAMMA_LOW,m_nGammaLow);
	DDV_MinMaxInt(pDX,m_nGammaLow,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_GAMMA_DT,m_nGammaDt);
	DDV_MinMaxInt(pDX,m_nGammaDt,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_GAMMA_HIGH,m_nGammaHigh);
	DDV_MinMaxInt(pDX,m_nGammaHigh,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_TAIL_PORTION,m_nTailPortion);
	DDV_MinMaxInt(pDX,m_nTailPortion,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_TAIL_ALPHA,m_nTailAlpha);
	DDV_MinMaxInt(pDX,m_nTailAlpha,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_SPDET_FAR,m_nSpdetFar);
	DDV_MinMaxInt(pDX,m_nSpdetFar,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_SPDET_MIC,m_nSpdetMic);
	//DDV_MinMaxInt(pDX,m_nSpdetMic,0,32767);
	DDX_Text(pDX,IDC_EDIT_TX_SPDET_X_CLIP,m_nSpdetXClip);
	DDV_MinMaxInt(pDX,m_nSpdetXClip,0,32767);
	DDX_Text(pDX, IDC_EDIT_TX_DRC_NUMKNEES, m_nDrcNumknees);
	DDV_MinMaxInt(pDX,m_nDrcNumknees,1,5);
	DDX_Text(pDX, IDC_EDIT_TX_DRC_ATTACKTIME, m_nDrcAttackTime);
	DDV_MinMaxInt(pDX,m_nDrcAttackTime,0,32767);
	DDX_Text(pDX, IDC_EDIT_TX_DRC_RELEASETIME, m_nDrcReleaseTime);
	DDV_MinMaxInt(pDX,m_nDrcReleaseTime,0,32767);
	DDX_Text(pDX, IDC_EDIT_TX_DRC_LIMITLEVEL, m_nDrcLimitLevel);
	DDV_MinMaxInt(pDX,m_nDrcLimitLevel,-96,0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNxpTx, CDialog)
	//{{AFX_MSG_MAP(CDlgNxpTx)
	ON_BN_CLICKED(IDC_BUTTON_TX_PEQ_COEFS, OnButtonTxPeqCoefs)
	ON_BN_CLICKED(IDC_BUTTON_TX_NV_PARAM, OnButtonTxNvParam)
	ON_BN_CLICKED(IDC_BUTTON_TX_DRC_COMPRESSOR_INPUT_LEVEL, OnButtonTxDrcCompressorInputLevel)
	ON_BN_CLICKED(IDC_BUTTON_TX_DRC_COMPRESSOR_OUTPUT_LEVEL, OnButtonTxDrcCompressorOutputLevel)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNxpTx message handlers

BOOL CDlgNxpTx::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitValue(m_lvve_data);
	EnableControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNxpTx::SetValue(LVVE_CTRL_PARAM_NV_T lvve_data)
{
	memcpy(&m_lvve_data, &lvve_data, sizeof(LVVE_CTRL_PARAM_NV_T));
}

void CDlgNxpTx::FetchValue(LVVE_CTRL_PARAM_NV_T& lvve_data)
{
	memcpy(&lvve_data, &m_lvve_data, sizeof(LVVE_CTRL_PARAM_NV_T));
}

void CDlgNxpTx::InitValue(LVVE_CTRL_PARAM_NV_T lvve_data)
{
	m_nCBGeneralOperMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode;
	if(m_nCBGeneralOperMode == LVM_MAXENUM)
	{
		m_nCBGeneralOperMode = 5;
	}
	m_nCBMute = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute;
	if(m_nCBMute == LVM_MAXENUM)
	{
		m_nCBMute = 2;
	}
	m_nCBBdMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode;
	if(m_nCBBdMode == LVM_MAXENUM)
	{
		m_nCBBdMode = 2;
	}
	m_nBulkDelay = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay;
	m_nCBVolMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode;
	if(m_nCBVolMode == LVM_MAXENUM)
	{
		m_nCBVolMode = 2;
	}
	m_nVolGain = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain;
	m_nCBHpfMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode;
	if(m_nCBHpfMode == LVM_MAXENUM)
	{
		m_nCBHpfMode = 2;
	}
	m_nHpfFreq = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq;
	m_nCBOperMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.OperatingMode;
	if(m_nCBOperMode == LVM_MAXENUM)
	{
		m_nCBOperMode = 2;
	}
	m_nMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.Mode;
	m_nTuningMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.TuningMode;
	m_nInputGain = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.InputGain;
	m_nOutputGain = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.OutputGain;
	m_nNLMSLimit = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_limit;
	m_nNLMSLbTaps = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_LB_taps;
	m_nNLMSLbAlpha = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_LB_two_alpha;
	m_nNLMSLbErl = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_LB_erl;
	m_nNLMSHbTaps = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_HB_taps;
	m_nNLMSHbAlpha = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_HB_two_alpha;
	m_nNLMSHbErl = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_HB_erl;
	m_nNLMSPresetCoefs = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_preset_coefs;
	m_nNLMSOffset = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_offset;
	m_nTailAlpha = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_tail_alpha;
	m_nTailPortion = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_tail_portion;
	m_nGammaHigh = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_high;
	m_nGammaDt = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_dt;
	m_nGammaLow = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_low;
	m_nGammaAlpha = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_alpha;
	m_nGammaN = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_n;
	m_nLimitNs = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_limit_ns;
	m_nCniGain = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_CNI_Gain;
	m_nNlAtten = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_NL_atten;
	m_nSpdetNear = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_spdet_near;
	m_nSpdetAct = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_spdet_act;
	m_nNfeBlocks = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_NFE_blocksize;
	m_nSpdetFar = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.SPDET_far;
	m_nSpdetMic = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.SPDET_mic;
	m_nSpdetXClip = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.SPDET_x_clip;
	m_nPcdThreshold = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_threshold;
	m_nPcdTaps = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_taps;
	m_nPcdErl = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_erl;
	m_nPcdMinimumErl = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_minimum_erl;
	m_nPcdErlStep = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_erl_step;
	m_nPcdGammaERescue = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_gamma_e_rescue;

	m_nCBWhisperOperMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.OperatingMode;
	if(m_nCBWhisperOperMode == LVM_MAXENUM)
	{
		m_nCBWhisperOperMode = 2;
	}
	m_nWhisperMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.mode;
	m_nTargetLevelLi = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Target_level_lin;
	m_nAvlMingainlin = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_MinGainLin;
	m_nAvlMaxgainlin = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_MaxGainLin;
	m_nAvlAttack = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Attack;
	m_nAvlRelease = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Release;
	m_nAvlLimitMaxoutput = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Limit_MaxOutputLin;
	m_nSpdetectThreshold = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.SpDetect_Threshold;

	m_nCBEqOperatingMod = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode;
	if(m_nCBEqOperatingMod == LVM_MAXENUM)
	{
		m_nCBEqOperatingMod = 2;
	}

	m_nEqLength = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams.EQ_Length;

	//teana hu 2012.01.10 drc
	if(m_dwLVVEType == LVVE_DUAL_MIC || m_dwLVVEType == LVVE_SINGLE_MIC)
	{
		m_nCBDrcOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.OperatingMode;
		if(m_nCBDrcOperatingMode == LVM_MAXENUM)
		{
			m_nCBDrcOperatingMode = 2;
		}
		
		m_nCBDrcLimiterOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.LimiterOperatingMode;
		if(m_nCBDrcLimiterOperatingMode == LVM_MAXENUM)
		{
			m_nCBDrcLimiterOperatingMode = 2;
		}
		
		
		m_nDrcNumknees = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.NumKnees;
		
		m_nDrcAttackTime = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.AttackTime;
		m_nDrcReleaseTime = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.ReleaseTime;
		m_nDrcLimitLevel = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.LimitLevel;
	}
	//

	UpdateData(FALSE);
}

void CDlgNxpTx::GetValue(LVVE_CTRL_PARAM_NV_T& lvve_data)
{
	UpdateData();
	if(m_nCBGeneralOperMode == 5)
	{
		m_nCBGeneralOperMode = LVM_MAXENUM;
	}
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode = m_nCBGeneralOperMode;
	if(m_nCBMute == 2)
	{
		m_nCBMute = LVM_MAXENUM;
	}
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute = m_nCBMute;
	if(m_nCBBdMode == 2)
	{
		m_nCBBdMode = LVM_MAXENUM;
	}
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode = m_nCBBdMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay = m_nBulkDelay;
	if(m_nCBVolMode == 2)
	{
		m_nCBVolMode = LVM_MAXENUM;
	}
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode = m_nCBVolMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain = m_nVolGain;
	if(m_nCBHpfMode == 2)
	{
		m_nCBHpfMode = LVM_MAXENUM;
	}
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode = m_nCBHpfMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq = m_nHpfFreq;
	if(m_nCBOperMode == 2)
	{
		m_nCBOperMode = LVM_MAXENUM;
	}
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.OperatingMode = m_nCBOperMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.Mode = m_nMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.TuningMode = m_nTuningMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.InputGain = m_nInputGain;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.OutputGain = m_nOutputGain;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_limit = m_nNLMSLimit;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_LB_taps = m_nNLMSLbTaps;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_LB_two_alpha = m_nNLMSLbAlpha;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_LB_erl = m_nNLMSLbErl;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_HB_taps = m_nNLMSHbTaps;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_HB_two_alpha = m_nNLMSHbAlpha;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_HB_erl = 	m_nNLMSHbErl;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_preset_coefs = m_nNLMSPresetCoefs;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_offset = m_nNLMSOffset;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_tail_alpha = m_nTailAlpha;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_tail_portion = m_nTailPortion;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_high = m_nGammaHigh;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_dt = m_nGammaDt;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_low = m_nGammaLow;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_alpha = m_nGammaAlpha;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_n = m_nGammaN;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_limit_ns = m_nLimitNs;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_CNI_Gain = m_nCniGain;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_NL_atten = m_nNlAtten;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_spdet_near = m_nSpdetNear;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_spdet_act = m_nSpdetAct;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_NFE_blocksize = m_nNfeBlocks;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.SPDET_far = m_nSpdetFar;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.SPDET_mic = m_nSpdetMic;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.SPDET_x_clip = m_nSpdetXClip;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_threshold = m_nPcdThreshold;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_taps = m_nPcdTaps;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_erl = m_nPcdErl;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_minimum_erl = m_nPcdMinimumErl;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_erl_step = m_nPcdErlStep;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_gamma_e_rescue = m_nPcdGammaERescue;
	if(m_nCBWhisperOperMode == 2)
	{
		m_nCBWhisperOperMode = LVM_MAXENUM;
	}

	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.OperatingMode = m_nCBWhisperOperMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.mode = m_nWhisperMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Target_level_lin = m_nTargetLevelLi;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_MinGainLin = m_nAvlMingainlin;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_MaxGainLin = m_nAvlMaxgainlin;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Attack = m_nAvlAttack;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Release = m_nAvlRelease;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Limit_MaxOutputLin = m_nAvlLimitMaxoutput;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.SpDetect_Threshold = m_nSpdetectThreshold;

	if(m_nCBEqOperatingMod == 2)
	{
		m_nCBEqOperatingMod = LVM_MAXENUM;
	}
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode = m_nCBEqOperatingMod;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams.EQ_Length = m_nEqLength;

	//teana hu 2012.01.10 drc
	if(m_dwLVVEType == LVVE_DUAL_MIC || m_dwLVVEType == LVVE_SINGLE_MIC)
	{
		if(m_nCBDrcOperatingMode == 2)
		{
			m_nCBDrcOperatingMode = LVM_MAXENUM;
		}
		lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.OperatingMode = m_nCBDrcOperatingMode;
		
		if(m_nCBDrcLimiterOperatingMode == 2)
		{
			m_nCBDrcLimiterOperatingMode = LVM_MAXENUM;
		}
		lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.LimiterOperatingMode = m_nCBDrcLimiterOperatingMode;
		
		lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.NumKnees = m_nDrcNumknees;
		
		lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.AttackTime = m_nDrcAttackTime;
		lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.ReleaseTime = m_nDrcReleaseTime;
		lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.LimitLevel = m_nDrcLimitLevel;
	}
	//
}

void CDlgNxpTx::OnOK() 
{
	// TODO: Add extra validation here
	GetValue(m_lvve_data);
	
	CDialog::OnOK();
}

void CDlgNxpTx::OnButtonTxPeqCoefs() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgEqCoefs dlg;
	dlg.SetDlgTitle(_T("EqCoef"));
	dlg.SetItemValue(m_lvve_data.lvve_ctrl_param_t.TxEqCoefs, m_nEqLength);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetItemValue(m_lvve_data.lvve_ctrl_param_t.TxEqCoefs);
	}
	
}

void CDlgNxpTx::OnButtonTxNvParam() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgTxNVParam dlg;
	dlg.SetValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams);
	}
	
}

void CDlgNxpTx::OnButtonTxDrcCompressorInputLevel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgEqCoefs dlg;
	dlg.SetDlgTitle(_T("CompressorCurveInputLevels"));
	dlg.SetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.CompressorCurveInputLevels, LVMDRC_MAX_KNEES);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.CompressorCurveInputLevels);
	}
}

void CDlgNxpTx::OnButtonTxDrcCompressorOutputLevel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgEqCoefs dlg;
	dlg.SetDlgTitle(_T("CompressorCurveOutputLevels"));
	dlg.SetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.CompressorCurveOutputLevels, LVMDRC_MAX_KNEES);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.CompressorCurveOutputLevels);
	}
}

void CDlgNxpTx::EnableControl()
{
	if(m_dwLVVEType == LVVE_NO_RESERVE || m_dwLVVEType == LVVE_RESERVE)
	{
		GetDlgItem(IDC_BUTTON_TX_NV_PARAM)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TX_DRC_COMPRESSOR_INPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TX_DRC_COMPRESSOR_OUTPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_DRC_ATTACKTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_DRC_LIMITLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_DRC_NUMKNEES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_DRC_RELEASETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TX_DRC_LIMITEROPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TX_DRC_OPER_MODE)->EnableWindow(FALSE);
	}
	else if(m_dwLVVEType == LVVE_JUNIPER)
	{
		GetDlgItem(IDC_BUTTON_TX_NV_PARAM)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TX_DRC_COMPRESSOR_INPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TX_DRC_COMPRESSOR_OUTPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_DRC_ATTACKTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_DRC_LIMITLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_DRC_NUMKNEES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_DRC_RELEASETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TX_DRC_LIMITEROPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TX_DRC_OPER_MODE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_TX_WHISPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_TARGET_LEVEL_LI)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_AVL_ATTACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_AVL_LIMIT_MAXOUTPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_AVL_MAXGAINLIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_AVL_MINGAINLIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_AVL_RELEASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TX_SPDETECT_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TX_WHISPER_OPER_MODE)->EnableWindow(FALSE);
	}
	else if(m_dwLVVEType == LVVE_SINGLE_MIC)
	{
		GetDlgItem(IDC_BUTTON_TX_NV_PARAM)->EnableWindow(FALSE);
	}
}

UINT CDlgNxpTx::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CPropertySheet::OnNcHitTest(point);
	CRect rc;
	GetClientRect(&rc);
	
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);
}