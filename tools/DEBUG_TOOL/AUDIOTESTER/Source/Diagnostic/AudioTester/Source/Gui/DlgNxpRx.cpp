// DlgNxpRx.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgNxpRx.h"
#include "DlgEqCoefs.h"
#include "AudioTesterSheet.h"
#include "Command.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNxpRx dialog


CDlgNxpRx::CDlgNxpRx(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNxpRx::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNxpRx)
		// NOTE: the ClassWizard will add member initialization here
	m_nCBMute = 0;
	m_nCBVolOperatingMode = 0;
	m_nCBOperatingMode = 0;
	m_nCBNlppOperatingMode = 0;
	m_nCBVoiceOperatingMode = 0;
	m_nCBEqOperatingMode = 0;
	m_nCBFensOperatingMode = 0;
	m_nVolGain = -96;
	m_nFensLimitNs = 0;
	m_nMode = 0;
	m_nNlppLimit = -24;
	m_nNlppHpfCornerfreq = 50;
//	m_nNoiseSensitivity = -6709;
	m_nAvlRelease = 0;
	m_nAvlMaxGainLin = 0;
	m_nSpdetectThreshold = 0;
	m_nAvlTargetLevelLin = 0;
	m_nVoiceMode = 0;
	m_nAvlReleaseMax = 0;
	m_nAvlNominalGain = 0;
	m_nAvlLimitMaxoutput = 0;
	m_nAvlMinGainLin = 0;
	m_nGainLinMax = 0;
	m_nAvlAttack = 0;
	m_nEqLength = 0;
	m_nNoiseSensitivity = 0;
	m_nCBDrcOperatingMode = 0;
	m_nDrcNumknees = 1;
	m_nDrcAttackTime = 0;
	m_nDrcReleaseTime = 0;
	m_nCBDrcLimiterOperatingMode = 0;
	m_nDrcLimitLevel = 0;
	m_nCBNgOperatingMode = 0;
	m_nNgNumknees = 1;
	m_nNgAttackTime = 0;
	m_nNgReleaseTime = 0;
	m_nCBNgLimiterOperatingMode = 0;
	m_nNgLimitLevel = 0;
	m_nCBHPFOperatingMode = 0;
	m_nHPFCornerFreq = 0;
	m_nCBCNGOperatingMode = 0;
	m_nCNGVolume = -96;
	m_nCBWMOperatingMode = 0;
	m_nWMMode = 0;
	m_nWMAVLMinGainLin = 0;
	m_nWMAVLMaxGainLin = 512;
	m_nWMAVLTargetLevelLin = 0;
	m_nWMAVLAttack = 0;
	m_nWMAVLRelease = 0;
	m_nWMAVLLimitMaxOutPut = 0;
	m_nWMSpDetectThreshold = 0;
	//}}AFX_DATA_INIT
	memset(&m_lvve_data, 0, sizeof(LVVE_CTRL_PARAM_NV_T));
	m_dwLVVEType = LVVE_RESERVE;
}


void CDlgNxpRx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNxpRx)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_CBIndex(pDX,IDC_COMBO_RX_MUTE,m_nCBMute);
	DDX_CBIndex(pDX,IDC_COMBO_RX_VOL_OPERATINGMODE,m_nCBVolOperatingMode);
	DDX_CBIndex(pDX,IDC_COMBO_RX_OPERATINGMODE,m_nCBOperatingMode);
	DDX_CBIndex(pDX,IDC_COMBO_RX_NLPP_OPERATINGMODE,m_nCBNlppOperatingMode);
	DDX_CBIndex(pDX,IDC_COMBO_RX_VOICE_OPERATINGMODE,m_nCBVoiceOperatingMode);
	DDX_CBIndex(pDX,IDC_COMBO_RX_EQ_OPERATINGMODE,m_nCBEqOperatingMode);
	DDX_CBIndex(pDX, IDC_COMBO_RX_FENS_OPERATINGMODE, m_nCBFensOperatingMode);
	DDX_CBIndex(pDX, IDC_COMBO_RX_DRC_OPER_MODE, m_nCBDrcOperatingMode);
	DDX_CBIndex(pDX, IDC_COMBO_RX_DRC_LIMITEROPER_MODE, m_nCBDrcLimiterOperatingMode);
	DDX_CBIndex(pDX, IDC_COMBO_RX_NG_OPER_MODE, m_nCBNgOperatingMode);
	DDX_CBIndex(pDX, IDC_COMBO_RX_NG_LIMITEROPER_MODE, m_nCBNgLimiterOperatingMode);
	DDX_CBIndex(pDX, IDC_COMBO_RX_WM_OPERATINGMODE, m_nCBWMOperatingMode);
	DDX_CBIndex(pDX, IDC_COMBO_RX_HPF_OPER_MODE, m_nCBHPFOperatingMode);
	DDX_CBIndex(pDX, IDC_COMBO_RX_CNG_OPER_MODE, m_nCBCNGOperatingMode);
	DDX_Text(pDX,IDC_EDIT_RX_VOL_GAIN,m_nVolGain);
	DDV_MinMaxInt(pDX,m_nVolGain,-96,24);
	DDX_Text(pDX,IDC_EDIT_RX_FENS_LIMIT_NS,m_nFensLimitNs);
	DDV_MinMaxInt(pDX,m_nFensLimitNs,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_MODE,m_nMode);
	DDV_MinMaxInt(pDX,m_nMode,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_NLPP_LIMIT,m_nNlppLimit);
	DDV_MinMaxInt(pDX,m_nNlppLimit,-24,0);
	DDX_Text(pDX,IDC_EDIT_RX_NLPP_HPF_CORNERFREQ,m_nNlppHpfCornerfreq);
	DDV_MinMaxInt(pDX,m_nNlppHpfCornerfreq,50,1000);
//	DDX_Text(pDX,IDC_EDIT_RX_NOISE_SENSITIVITY,m_nNoiseSensitivity);
//	DDV_MinMaxInt(pDX,m_nNoiseSensitivity,-6709,32767);
	DDX_Text(pDX,IDC_EDIT_RX_AVL_RELEASE,m_nAvlRelease);
	DDV_MinMaxInt(pDX,m_nAvlRelease,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_AVL_MAXGAINLIN,m_nAvlMaxGainLin);
	DDV_MinMaxInt(pDX,m_nAvlMaxGainLin,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_SPDETECT_THRESHOLD,m_nSpdetectThreshold);
	DDV_MinMaxInt(pDX,m_nSpdetectThreshold,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_AVL_TARGET_LEVEL_LIN,m_nAvlTargetLevelLin);
	DDV_MinMaxInt(pDX,m_nAvlTargetLevelLin,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_VOICE_MODE,m_nVoiceMode);
	DDV_MinMaxInt(pDX,m_nVoiceMode,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_AVL_RELEASEMAX,m_nAvlReleaseMax);
	DDV_MinMaxInt(pDX,m_nAvlReleaseMax,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_AVL_NOMINALGAIN,m_nAvlNominalGain);
	DDV_MinMaxInt(pDX,m_nAvlNominalGain,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_AVL_LIMIT_MAXOUTPUT,m_nAvlLimitMaxoutput);
	DDV_MinMaxInt(pDX,m_nAvlLimitMaxoutput,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_AVL_MINGAINLIN,m_nAvlMinGainLin);
	DDV_MinMaxInt(pDX,m_nAvlMinGainLin,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_GAIN_LIN_MAX,m_nGainLinMax);
	DDV_MinMaxInt(pDX,m_nGainLinMax,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_AVL_ATTACK,m_nAvlAttack);
	DDV_MinMaxInt(pDX,m_nAvlAttack,0,32767);
	DDX_Text(pDX,IDC_EDIT_RX_EQ_LENGTH,m_nEqLength);
	DDV_MinMaxInt(pDX,m_nEqLength,0,32);
	DDX_Text(pDX, IDC_EDIT_RX_NOISE_SENSITIVITY, m_nNoiseSensitivity);
	DDV_MinMaxInt(pDX, m_nNoiseSensitivity, -6709, 32767);
	DDX_Text(pDX, IDC_EDIT_RX_DRC_NUMKNEES, m_nDrcNumknees);
	DDV_MinMaxInt(pDX,m_nDrcNumknees,1,5);
	DDX_Text(pDX, IDC_EDIT_RX_DRC_ATTACKTIME, m_nDrcAttackTime);
	DDV_MinMaxInt(pDX,m_nDrcAttackTime,0,32767);
	DDX_Text(pDX, IDC_EDIT_RX_DRC_RELEASETIME, m_nDrcReleaseTime);
	DDV_MinMaxInt(pDX,m_nDrcReleaseTime,0,32767);
	DDX_Text(pDX, IDC_EDIT_RX_DRC_LIMITLEVEL, m_nDrcLimitLevel);
	DDV_MinMaxInt(pDX,m_nDrcLimitLevel,-96,0);
	DDX_Text(pDX, IDC_EDIT_RX_NG_NUMKNEES, m_nNgNumknees);
	DDV_MinMaxInt(pDX,m_nNgNumknees,1,5);
	DDX_Text(pDX, IDC_EDIT_RX_NG_ATTACKTIME, m_nNgAttackTime);
	DDV_MinMaxInt(pDX,m_nNgAttackTime,0,32767);
	DDX_Text(pDX, IDC_EDIT_RX_NG_RELEASETIME, m_nNgReleaseTime);
	DDV_MinMaxInt(pDX,m_nNgReleaseTime,0,32767);
	DDX_Text(pDX, IDC_EDIT_RX_NG_LIMITLEVEL, m_nNgLimitLevel);
	DDV_MinMaxInt(pDX,m_nNgLimitLevel,-96,0);
	DDX_Text(pDX, IDC_EDIT_RX_HPF_CORNERFREQ, m_nHPFCornerFreq);
	DDV_MinMaxInt(pDX,m_nHPFCornerFreq,0,32767);
	DDX_Text(pDX, IDC_EDIT_RX_CNG_VOLUME, m_nCNGVolume);
	DDV_MinMaxInt(pDX,m_nCNGVolume,-96,-60);
	DDX_Text(pDX, IDC_EDIT_RX_WM_MODE, m_nWMMode);
	DDV_MinMaxInt(pDX,m_nWMMode,0,255);
	DDX_Text(pDX, IDC_EDIT_RX_WM_AVL_MINGAINLIN, m_nWMAVLMinGainLin);
	DDV_MinMaxInt(pDX,m_nWMAVLMinGainLin,0,512);
	DDX_Text(pDX, IDC_EDIT_RX_WM_AVL_MAXGAINLIN, m_nWMAVLMaxGainLin);
	DDV_MinMaxInt(pDX,m_nWMAVLMaxGainLin,512,32767);
	DDX_Text(pDX, IDC_EDIT_RX_WM_AVL_TARGET_LEVEL_LIN, m_nWMAVLTargetLevelLin);
	DDV_MinMaxInt(pDX,m_nWMAVLTargetLevelLin,0,32767);
	DDX_Text(pDX, IDC_EDIT_RX_WM_AVL_ATTACK, m_nWMAVLAttack);
	DDV_MinMaxInt(pDX,m_nWMAVLAttack,0,32767);
	DDX_Text(pDX, IDC_EDIT_RX_WM_AVL_RELEASE, m_nWMAVLRelease);
	DDV_MinMaxInt(pDX,m_nWMAVLRelease,0,32767);
	DDX_Text(pDX, IDC_EDIT_RX_WM_AVL_LIMIT_MAXOUTPUT, m_nWMAVLLimitMaxOutPut);
	DDV_MinMaxInt(pDX,m_nWMAVLLimitMaxOutPut,0,32767);
	DDX_Text(pDX, IDC_EDIT_RX_WM_SPDETECT_THRESHOLD, m_nWMSpDetectThreshold);
	DDV_MinMaxInt(pDX,m_nWMSpDetectThreshold,0,32767);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNxpRx, CDialog)
	//{{AFX_MSG_MAP(CDlgNxpRx)
	ON_BN_CLICKED(IDC_BUTTON_RX_PEQ_COEFS, OnButtonRxPeqCoefs)
	ON_BN_CLICKED(IDC_BUTTON_NOISE_SENSITIVITY, OnButtonNoiseSensitivity)
	ON_BN_CLICKED(IDC_BUTTON_RX_DRC_COMPRESSOR_INPUT_LEVEL, OnButtonRxDrcCompressorInputLevel)
	ON_BN_CLICKED(IDC_BUTTON_RX_DRC_COMPRESSOR_OUTPUT_LEVEL, OnButtonRxDrcCompressorOutputLevel)
	ON_BN_CLICKED(IDC_BUTTON_RX_NG_COMPRESSOR_INPUT_LEVEL, OnButtonRxNgCompressorInputLevel)
	ON_BN_CLICKED(IDC_BUTTON_RX_NG_COMPRESSOR_OUTPUT_LEVEL, OnButtonRxNgCompressorOutputLevel)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNxpRx message handlers

BOOL CDlgNxpRx::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	/*
	CAudioTesterSheet *pSheet = (CAudioTesterSheet *)AfxGetMainWnd();
	if(pSheet != NULL && pSheet->m_bLVVEMin)
	{
		GetDlgItem(IDC_BUTTON_NOISE_SENSITIVITY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_NOISE_SENSITIVITY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RX_NOISE_SENSITIVITY)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_NOISE_SENSITIVITY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_NOISE_SENSITIVITY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RX_NOISE_SENSITIVITY)->ShowWindow(SW_HIDE);
	}
	*/
	InitValue(m_lvve_data);

	EnableControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNxpRx::SetValue(LVVE_CTRL_PARAM_NV_T lvve_data)
{
	memcpy(&m_lvve_data, &lvve_data, sizeof(LVVE_CTRL_PARAM_NV_T));
}

void CDlgNxpRx::FetchValue(LVVE_CTRL_PARAM_NV_T& lvve_data)
{
	memcpy(&lvve_data, &m_lvve_data, sizeof(LVVE_CTRL_PARAM_NV_T));
}

void CDlgNxpRx::InitValue(LVVE_CTRL_PARAM_NV_T lvve_data)
{
	m_nCBOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode;
	if(m_nCBOperatingMode == LVM_MAXENUM)
	{
		m_nCBOperatingMode = 3;
	}

	m_nCBMute = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute;
	if(m_nCBMute == LVM_MAXENUM)
	{
		m_nCBMute = 2;
	}
	m_nCBVolOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode;
	if(m_nCBVolOperatingMode == LVM_MAXENUM)
	{
		m_nCBVolOperatingMode = 2;
	}
	m_nVolGain = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain;

	m_nCBFensOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.OperatingMode;

	if(m_nCBFensOperatingMode == LVM_MAXENUM)
	{
		m_nCBFensOperatingMode = 2;
	}

	m_nFensLimitNs = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.FENS_limit_NS;
	m_nMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.Mode;

	m_nCBNlppOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode;
	if(m_nCBNlppOperatingMode == LVM_MAXENUM)
	{
		m_nCBNlppOperatingMode = 2;
	}
	m_nNlppLimit = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams.NLPP_Limit;
	m_nNlppHpfCornerfreq = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams.NLPP_HPF_CornerFreq;

	m_nCBVoiceOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.OperatingMode;
	if(m_nCBVoiceOperatingMode == LVM_MAXENUM)
	{
		m_nCBVoiceOperatingMode = 2;
	}
	m_nVoiceMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.mode;
	m_nGainLinMax = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Gain_Lin_Max;
//	m_nNoiseSensitivity = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Noise_Sensitivity;
	m_nAvlTargetLevelLin = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Target_level_lin;
	m_nAvlMinGainLin = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_MinGainLin;
	m_nAvlMaxGainLin = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_MaxGainLin;
	m_nAvlNominalGain = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_NominalGain;
	m_nAvlAttack = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Attack;
	m_nAvlRelease = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Release;
	m_nAvlReleaseMax = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_ReleaseMax;
	m_nAvlLimitMaxoutput = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Limit_MaxOutputLin;
	m_nSpdetectThreshold = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.SpDetect_Threshold;

	m_nCBEqOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode;
	if(m_nCBEqOperatingMode == LVM_MAXENUM)
	{
		m_nCBEqOperatingMode = 2;
	}
	m_nEqLength = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams.EQ_Length;

	m_nNoiseSensitivity = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Noise_Sensitivity;


	//teana hu 2012.01.11 drc
	if(m_dwLVVEType == LVVE_DUAL_MIC)
	{
		m_nCBDrcOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.OperatingMode;
		if(m_nCBDrcOperatingMode == LVM_MAXENUM)
		{
			m_nCBDrcOperatingMode = 2;
		}
		
		m_nCBDrcLimiterOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.LimiterOperatingMode;
		if(m_nCBDrcLimiterOperatingMode == LVM_MAXENUM)
		{
			m_nCBDrcLimiterOperatingMode = 2;
		}
		
		m_nDrcNumknees = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.NumKnees;
		m_nDrcAttackTime = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.AttackTime;
		m_nDrcReleaseTime = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.ReleaseTime;
		m_nDrcLimitLevel = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.LimitLevel;
		
		//NG
		m_nCBNgOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.OperatingMode;
		if(m_nCBNgOperatingMode == LVM_MAXENUM)
		{
			m_nCBNgOperatingMode = 2;
		}
		
		m_nCBNgLimiterOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.LimiterOperatingMode;
		if(m_nCBNgLimiterOperatingMode == LVM_MAXENUM)
		{
			m_nCBNgLimiterOperatingMode = 2;
		}
		m_nNgNumknees = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.NumKnees;
		m_nNgAttackTime = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.AttackTime;
		m_nNgReleaseTime = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.ReleaseTime;
		m_nNgLimitLevel = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.LimitLevel;
	}

	if(m_dwLVVEType == LVVE_DUAL_MIC || m_dwLVVEType == LVVE_SINGLE_MIC)
	{
		m_nCBHPFOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_OperatingMode;
		if(m_nCBHPFOperatingMode == LVM_MAXENUM)
		{
			m_nCBHPFOperatingMode = 2;
		}
		
		m_nHPFCornerFreq = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_CornerFreq;
		
		m_nCBCNGOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_OperatingMode;
		if(m_nCBCNGOperatingMode == LVM_MAXENUM)
		{
			m_nCBCNGOperatingMode = 2;
		}
		m_nCNGVolume = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_ControlParams.CNG_Volume;

		//wm
		m_nCBWMOperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.OperatingMode;
		if(m_nCBWMOperatingMode == LVM_MAXENUM)
		{
			m_nCBWMOperatingMode = 2;
		}
		
		m_nWMMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.mode;
		m_nWMAVLMinGainLin = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_MinGainLin;
		m_nWMAVLMaxGainLin = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_MaxGainLin;
		m_nWMAVLTargetLevelLin = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Target_level_lin;
		m_nWMAVLAttack = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Attack;
		m_nWMAVLRelease = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Release;
		m_nWMAVLLimitMaxOutPut = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Limit_MaxOutputLin;
		m_nWMSpDetectThreshold = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.SpDetect_Threshold;
	}

	//
	UpdateData(FALSE);

}

void CDlgNxpRx::GetValue(LVVE_CTRL_PARAM_NV_T& lvve_data)
{
	if(!UpdateData())
	{
		return;
	}

	if(m_nCBOperatingMode == 3)
	{
		m_nCBOperatingMode = LVM_MAXENUM;
	}

	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode = m_nCBOperatingMode;
	if(m_nCBMute == 2)
	{
		m_nCBMute = LVM_MAXENUM;
	}
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute = m_nCBMute;
	if(m_nCBVolOperatingMode == 2)
	{
		m_nCBVolOperatingMode = LVM_MAXENUM;
	}
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode = m_nCBVolOperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain = m_nVolGain;
	if(m_nCBFensOperatingMode == 2)
	{
		m_nCBFensOperatingMode = LVM_MAXENUM;
	}

	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.OperatingMode = m_nCBFensOperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.FENS_limit_NS = m_nFensLimitNs;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.Mode = m_nMode;

	if(m_nCBNlppOperatingMode == 2)
	{
		m_nCBNlppOperatingMode = LVM_MAXENUM;
	}
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode = m_nCBNlppOperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams.NLPP_Limit = m_nNlppLimit;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams.NLPP_HPF_CornerFreq = m_nNlppHpfCornerfreq;
	if(m_nCBVoiceOperatingMode == 2)
	{
		m_nCBVoiceOperatingMode = LVM_MAXENUM;
	}


	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.OperatingMode = m_nCBVoiceOperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.mode = m_nVoiceMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Gain_Lin_Max = m_nGainLinMax;
//	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Noise_Sensitivity = m_nNoiseSensitivity;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Target_level_lin = 	m_nAvlTargetLevelLin;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_MinGainLin = m_nAvlMinGainLin;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_MaxGainLin = m_nAvlMaxGainLin;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_NominalGain = m_nAvlNominalGain;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Attack = m_nAvlAttack;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Release = m_nAvlRelease;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_ReleaseMax = m_nAvlReleaseMax;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Limit_MaxOutputLin = m_nAvlLimitMaxoutput;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.SpDetect_Threshold = m_nSpdetectThreshold;

	if(m_nCBEqOperatingMode == 2)
	{
		m_nCBEqOperatingMode = LVM_MAXENUM;
	}
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode = m_nCBEqOperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams.EQ_Length = m_nEqLength;

	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Noise_Sensitivity = m_nNoiseSensitivity;


	//teana hu 2012.01.11 drc
	if(m_dwLVVEType == LVVE_DUAL_MIC)
	{
		if(m_nCBDrcOperatingMode == 2)
		{
			m_nCBDrcOperatingMode = LVM_MAXENUM;
		}
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.OperatingMode = m_nCBDrcOperatingMode;
		
		if(m_nCBDrcLimiterOperatingMode == 2)
		{
			m_nCBDrcLimiterOperatingMode = LVM_MAXENUM;
		}
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.LimiterOperatingMode = m_nCBDrcLimiterOperatingMode;
		if(m_dwLVVEType == LVVE_DUAL_MIC)
		{
			lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.NumKnees = m_nDrcNumknees;
		}
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.AttackTime = m_nDrcAttackTime;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.ReleaseTime = m_nDrcReleaseTime;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.LimitLevel = m_nDrcLimitLevel;
		
		//NG
		
		if(m_nCBNgOperatingMode == 2)
		{
			m_nCBNgOperatingMode = LVM_MAXENUM;
		}
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.OperatingMode = m_nCBNgOperatingMode;
		
		if(m_nCBNgLimiterOperatingMode == 2)
		{
			m_nCBNgLimiterOperatingMode = LVM_MAXENUM;
		}
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.LimiterOperatingMode = m_nCBNgLimiterOperatingMode;
		if(m_dwLVVEType == LVVE_DUAL_MIC)
		{
			lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.NumKnees = m_nNgNumknees;
		}
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.AttackTime = m_nNgAttackTime;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.ReleaseTime = m_nNgReleaseTime;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.LimitLevel = m_nNgLimitLevel;
	}
	
	if(m_dwLVVEType == LVVE_DUAL_MIC || m_dwLVVEType == LVVE_SINGLE_MIC)
	{
		if(m_nCBHPFOperatingMode == 2)
		{
			m_nCBHPFOperatingMode = LVM_MAXENUM;
		}
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_OperatingMode = m_nCBHPFOperatingMode;
		
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_CornerFreq = m_nHPFCornerFreq;
		
		if(m_nCBCNGOperatingMode == 2)
		{
			m_nCBCNGOperatingMode = LVM_MAXENUM;
		}
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_OperatingMode = m_nCBCNGOperatingMode;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_ControlParams.CNG_Volume = m_nCNGVolume;

		//wm
		if(m_nCBWMOperatingMode == 2)
		{
			m_nCBWMOperatingMode = LVM_MAXENUM;
		}
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.OperatingMode = m_nCBWMOperatingMode;
		
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.mode = m_nWMMode;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_MinGainLin = m_nWMAVLMinGainLin;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_MaxGainLin = m_nWMAVLMaxGainLin;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Target_level_lin = m_nWMAVLTargetLevelLin;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Attack = m_nWMAVLAttack;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Release = m_nWMAVLRelease;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Limit_MaxOutputLin = m_nWMAVLLimitMaxOutPut;
		lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.SpDetect_Threshold = m_nWMSpDetectThreshold;
	}
	//
}

void CDlgNxpRx::OnOK() 
{
	// TODO: Add extra validation here
	GetValue(m_lvve_data);
	
	CDialog::OnOK();
}

void CDlgNxpRx::OnButtonRxPeqCoefs() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgEqCoefs dlg;
	dlg.SetDlgTitle(_T("EqCoef"));
	dlg.SetItemValue(m_lvve_data.lvve_ctrl_param_t.RxEqCoefs, m_nEqLength);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetItemValue(m_lvve_data.lvve_ctrl_param_t.RxEqCoefs);
	}
	
}

void CDlgNxpRx::OnButtonNoiseSensitivity() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgEqCoefs dlg;
	dlg.SetDlgTitle(_T("Noise_Sensitivity"));
	dlg.SetItemValue(&m_lvve_data.lvve_reserve[1], 9);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetItemValue(&m_lvve_data.lvve_reserve[1]);
	}
}

void CDlgNxpRx::OnButtonRxDrcCompressorInputLevel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgEqCoefs dlg;
	dlg.SetDlgTitle(_T("CompressorCurveInputLevels"));
	dlg.SetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.CompressorCurveInputLevels, LVMDRC_MAX_KNEES);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.CompressorCurveInputLevels);
	}
	
}

void CDlgNxpRx::OnButtonRxDrcCompressorOutputLevel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgEqCoefs dlg;
	dlg.SetDlgTitle(_T("CompressorCurveOutputLevels"));
	dlg.SetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.CompressorCurveOutputLevels, LVMDRC_MAX_KNEES);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.CompressorCurveOutputLevels);
	}
	
}

void CDlgNxpRx::OnButtonRxNgCompressorInputLevel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgEqCoefs dlg;
	dlg.SetDlgTitle(_T("CompressorCurveInputLevels"));
	dlg.SetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.CompressorCurveInputLevels, LVMDRC_MAX_KNEES);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.CompressorCurveInputLevels);
	}
	
}

void CDlgNxpRx::OnButtonRxNgCompressorOutputLevel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDlgEqCoefs dlg;
	dlg.SetDlgTitle(_T("CompressorCurveOutputLevels"));
	dlg.SetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.CompressorCurveOutputLevels, LVMDRC_MAX_KNEES);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetItemValue(m_lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.CompressorCurveOutputLevels);
	}
	
}

void CDlgNxpRx::EnableControl()
{
	GetDlgItem(IDC_BUTTON_NOISE_SENSITIVITY)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_NOISE_SENSITIVITY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_RX_NOISE_SENSITIVITY)->ShowWindow(SW_HIDE);

	if(m_dwLVVEType == LVVE_NO_RESERVE)
	{
		GetDlgItem(IDC_BUTTON_NOISE_SENSITIVITY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_NOISE_SENSITIVITY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RX_NOISE_SENSITIVITY)->ShowWindow(SW_SHOW);
		
		GetDlgItem(IDC_BUTTON_RX_DRC_COMPRESSOR_INPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RX_DRC_COMPRESSOR_OUTPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_ATTACKTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_LIMITLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_NUMKNEES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_RELEASETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_DRC_LIMITEROPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_DRC_OPER_MODE)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_RX_NG_COMPRESSOR_INPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RX_NG_COMPRESSOR_OUTPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_ATTACKTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_LIMITLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_NUMKNEES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_RELEASETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_NG_LIMITEROPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_NG_OPER_MODE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_RX_WM_AVL_ATTACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_LIMIT_MAXOUTPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_MAXGAINLIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_MINGAINLIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_TARGET_LEVEL_LIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_RELEASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_SPDETECT_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_WM_OPERATINGMODE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_RX_HPF_CORNERFREQ)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_HPF_OPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_CNG_VOLUME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_CNG_OPER_MODE)->EnableWindow(FALSE);


	}
	else if(m_dwLVVEType == LVVE_RESERVE)
	{
		GetDlgItem(IDC_BUTTON_RX_DRC_COMPRESSOR_INPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RX_DRC_COMPRESSOR_OUTPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_ATTACKTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_LIMITLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_NUMKNEES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_RELEASETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_DRC_LIMITEROPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_DRC_OPER_MODE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_BUTTON_RX_NG_COMPRESSOR_INPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RX_NG_COMPRESSOR_OUTPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_ATTACKTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_LIMITLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_NUMKNEES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_RELEASETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_NG_LIMITEROPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_NG_OPER_MODE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_RX_WM_AVL_ATTACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_LIMIT_MAXOUTPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_MAXGAINLIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_MINGAINLIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_TARGET_LEVEL_LIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_RELEASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_SPDETECT_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_WM_OPERATINGMODE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_RX_HPF_CORNERFREQ)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_HPF_OPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_CNG_VOLUME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_CNG_OPER_MODE)->EnableWindow(FALSE);
	}
	else if(m_dwLVVEType == LVVE_JUNIPER)
	{
		GetDlgItem(IDC_BUTTON_RX_DRC_COMPRESSOR_INPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RX_DRC_COMPRESSOR_OUTPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_ATTACKTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_LIMITLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_NUMKNEES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_RELEASETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_DRC_LIMITEROPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_DRC_OPER_MODE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_BUTTON_RX_NG_COMPRESSOR_INPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RX_NG_COMPRESSOR_OUTPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_ATTACKTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_LIMITLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_NUMKNEES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_RELEASETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_NG_LIMITEROPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_NG_OPER_MODE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_RX_WM_AVL_ATTACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_LIMIT_MAXOUTPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_MAXGAINLIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_MINGAINLIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_TARGET_LEVEL_LIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_AVL_RELEASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_WM_SPDETECT_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_WM_OPERATINGMODE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_RX_HPF_CORNERFREQ)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_HPF_OPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_CNG_VOLUME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_CNG_OPER_MODE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_RX_FENS_LIMIT_NS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_FENS_OPERATINGMODE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_RX_VOICE_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_GAIN_LIN_MAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_AVL_MAXGAINLIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_AVL_TARGET_LEVEL_LIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_AVL_MINGAINLIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_AVL_RELEASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_AVL_NOMINALGAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_AVL_ATTACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_SPDETECT_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_AVL_RELEASEMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_AVL_LIMIT_MAXOUTPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NOISE_SENSITIVITY)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_VOICE_OPERATINGMODE)->EnableWindow(FALSE);


	}
	else if(m_dwLVVEType == LVVE_SINGLE_MIC)
	{
		GetDlgItem(IDC_BUTTON_RX_DRC_COMPRESSOR_INPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RX_DRC_COMPRESSOR_OUTPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_ATTACKTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_LIMITLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_NUMKNEES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_DRC_RELEASETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_DRC_LIMITEROPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_DRC_OPER_MODE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_BUTTON_RX_NG_COMPRESSOR_INPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RX_NG_COMPRESSOR_OUTPUT_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_ATTACKTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_LIMITLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_NUMKNEES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RX_NG_RELEASETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_NG_LIMITEROPER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_NG_OPER_MODE)->EnableWindow(FALSE);
	}
}

UINT CDlgNxpRx::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CPropertySheet::OnNcHitTest(point);
	CRect rc;
	GetClientRect(&rc);
	
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);
}
