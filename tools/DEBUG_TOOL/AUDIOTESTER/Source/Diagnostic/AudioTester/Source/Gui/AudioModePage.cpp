// AudioModePage.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "AudioModePage.h"

#include "AdjustDlg.h"
#include "AudioTesterSheet.h"

#include "VolumeCtrlDlg.h"
#include "ModifyGainDlg.h"
#include "EQDlg.h"
#include "AgcDlg.h"
#include "AecDialog.h"

#include "ConvertFunc.h"

#include "DigitalGainDlg.h"
#include "PopDialog.h"
#include "NGDlg.h"

#include "CoefficientDlg.h"

#include "ExtDataDlg.h"

#include "xagcdlg.h"

#include "Extend2Dlg.h"
#include "Extend3Dlg.h"

#include "STNGDlg.h"
#include "VolumeDlg.h"
#include "DlgEQSet.h"
#include <math.h>

#include "DlgArmVolume.h"

#include "FirAutoDlg.h"
#include "DlgNxpRx.h"
#include "DlgNxpTx.h"
#include "DlgByte.h"
#include "DlgLineinGain.h"
#include "DlgCNG.h"

#include <dbt.h>

#include "FirAutoDlgEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*static*/
//BOOL CAudioModePage::m_bInitResource = FALSE;

/*static*/
//std::vector<HBITMAP> CAudioModePage::m_vResource;

/*static*/
//COtherPicFormatToBMP CAudioModePage::m_jpgToBmp;
/////////////////////////////////////////////////////////////////////////////
// CAudioModePage property page

IMPLEMENT_DYNCREATE(CAudioModePage, CPropertyPage)

CAudioModePage::CAudioModePage() : CPropertyPage(CAudioModePage::IDD)
{
	//{{AFX_DATA_INIT(CAudioModePage)
	m_strVolume = _T("");
	m_strSidetoneGain = _T("");
	m_strAgcSwitch = _T("");
	m_nSideToneCp = 0;
	m_nSideToneEq = 0;
	m_nLcfPosition = -1;
	m_bMixerSw = 0;
	//}}AFX_DATA_INIT

    m_dwADDevice = 0;
    m_dwDADevice = 0;
    m_dwDlPgaGain = 0;
    m_dwUlPgaGain = 0;

    m_nSideTone = 0;
    m_nVolume = 0;
    m_nAec = 0;
    m_nSampleRate = 0;
    m_nSampleRateValue = 0;

    m_strTitle = "";

    m_bActive = FALSE;

    m_bFir = FALSE;
	m_bNewMoreBandDL = FALSE;
    m_bXagcEnable = FALSE;
    
	m_bAudio3Switch = FALSE;

    memset( m_ExtendData,0,(EXTEND_DATA_SIZE+DSP_VOLUME_SIZE+ARM_VOLUME_SIZE)*2 );

	m_bVolumeSwitch = 0;

	m_nSideToneCp = 0;
	m_nSideToneEq = 0;
	m_nMp3AgcSwitch = 0;
	m_nMidiAgcSwitch = 0;
	m_nMp4AgcSwitch = 0;
	m_nVoiceAgcSwitch = 0;
	m_nMp3LcfSwitch = 0;
	m_nMidiLcfSwitch = 0;
	m_nMp4LcfSwitch = 0;
	m_nVoiceLcfSwitch = 0;
	m_dMp3AgcInputGain = 0.00000000;
	m_dMidiAgcInputGain = 0.00000000;
	m_dMp4AgcInputGain = 0.00000000;
	m_dVoiceAgcInputGain = 0.00000000;
	m_dMp3AgcInputGainBak = 0.00000000;
	m_dMidiAgcInputGainBak = 0.00000000;
	m_dMp4AgcInputGainBak = 0.00000000;
	m_dVoiceAgcInputGainBak = 0.00000000;
	m_nDigitalGain = 0;
	m_nEQSelect = 0;

	//teana hu 2011.02.14
	m_bAudio4 = FALSE;
	
	m_nSamplingRate = 0;
	m_nLowcutOffFrequency = 0;
	m_nBoostingGain = 0;
	m_nClarityLevel = 0;
	m_nVolumeType = 0;
	memset(&m_arrMp3AgcInputGain, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrMidiAgcInputGain, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrMp4AgcInputGain, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrVoiceAgcInputGain, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));

	memset(&m_arrMp3AgcInputGainBak, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrMidiAgcInputGainBak, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrMp4AgcInputGainBak, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrVoiceAgcInputGainBak, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));

	memset(&m_arrMp3AgcStartGain, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrMidiAgcStartGain, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrMp4AgcStartGain, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrVoiceAgcStartGain, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	
	memset(&m_arrMp3AgcStartGainBak, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrMidiAgcStartGainBak, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrMp4AgcStartGainBak, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	memset(&m_arrVoiceAgcStartGainBak, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(double));
	
	m_nOldIndex = -1;
	m_nNewIndex = -1;
	m_nVolumeCount = 0;

	m_bExistLVVE = FALSE;
	memset(&m_lvve_data, 0, sizeof(LVVE_CTRL_PARAM_NV_T));
	m_wDumpPcmCtl = 0;
	m_wNXPBoostCtl = 0;
	m_nVolumeLevel = -1;
	
	//teana hu 2011.06.15
	m_nMp3AgcZCSwitch = 0;
	m_nMidiAgcZCSwitch = 0;
	m_nMp4AgcZCSwitch = 0;
	m_nVoiceAgcZCSwitch = 0;
	m_dMp3AgcStartGain = 0.00000000;
	m_dMidiAgcStartGain = 0.00000000;
	m_dMp4AgcStartGain = 0.00000000;
	m_dVoiceAgcStartGain = 0.00000000;
	m_dMp3AgcStartGainBak = 0.00000000;
	m_dMidiAgcStartGainBak = 0.00000000;
	m_dMp4AgcStartGainBak = 0.00000000;
	m_dVoiceAgcStartGainBak = 0.00000000;
	m_nCBChannelSelLeft = 0;
	m_nCBChannelSelRight = 0;
	m_nPolarLeft = 0;
	m_nPolarRight = 0;
	m_nAgcAttack = 0;
	m_nAgcRelease = 0;
	m_nAgcHoldHc = 0;
	m_nAgcSdelay = 0;

	m_nChannelSelLeft = 0;
	m_nChannelSelRight = 0;

	m_nSoundEqVer = sound_eq_ver_none;

	m_dwAdcSet = 0;
	m_dwDacSet = 0;
	m_dwPathSet = 0;

	m_bExistDspCodec = FALSE;
	m_bDspCodecInfo = FALSE;
	m_bSmartPhone = FALSE;

	m_dwCNGGain = 0;
	m_dwCNGSW = 0;
}

CAudioModePage::~CAudioModePage()
{
    ClearAreas();
}

void CAudioModePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAudioModePage)
	DDX_Control(pDX, IDC_STATIC_SIDETONE_GAIN, m_edtSidetone);
	DDX_Text(pDX, IDC_STATIC_VOLUME, m_strVolume);
	DDX_Text(pDX, IDC_STATIC_SIDETONE_GAIN, m_strSidetoneGain);
	DDX_Text(pDX, IDC_STATIC_AGC_SWITCH, m_strAgcSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_SIDETONE_CP, m_nSideToneCp);
	DDX_CBIndex(pDX, IDC_COMBO_SIDETONE_EQ, m_nSideToneEq);
	DDX_CBIndex(pDX, IDC_COMBO_MP3_AGC_SWITCH, m_nMp3AgcSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_MIDI_AGC_SWITCH, m_nMidiAgcSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_MP4_AGC_SWITCH, m_nMp4AgcSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_VOICE_AGC_SWITCH, m_nVoiceAgcSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_MP3_LCF_SWITCH, m_nMp3LcfSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_MIDI_LCF_SWITCH, m_nMidiLcfSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_MP4_LCF_SWITCH, m_nMp4LcfSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_VOICE_LCF_SWITCH, m_nVoiceLcfSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_MP3_AGC_ZC_SWITCH, m_nMp3AgcZCSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_MIDI_AGC_ZC_SWITCH, m_nMidiAgcZCSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_MP4_AGC_ZC_SWITCH, m_nMp4AgcZCSwitch);
	DDX_CBIndex(pDX, IDC_COMBO_VOICE_AGC_ZC_SWITCH, m_nVoiceAgcZCSwitch);
	DDX_Text(pDX, IDC_EDT_MP3_AGC_START_GAIN, m_dMp3AgcStartGain);
	DDV_MinMaxDouble(pDX, m_dMp3AgcStartGain, -65535, 30);
	DDX_Text(pDX, IDC_EDT_MIDI_AGC_START_GAIN, m_dMidiAgcStartGain);
	DDV_MinMaxDouble(pDX, m_dMidiAgcStartGain, -65535, 30);
	DDX_Text(pDX, IDC_EDT_MP4_AGC_START_GAIN, m_dMp4AgcStartGain);
	DDV_MinMaxDouble(pDX, m_dMp4AgcStartGain, -65535, 30);
	DDX_Text(pDX, IDC_EDT_VOICE_AGC_START_GAIN, m_dVoiceAgcStartGain);
	DDV_MinMaxDouble(pDX, m_dVoiceAgcStartGain, -65535, 30);
	DDX_Text(pDX, IDC_EDT_MP3_AGC_INPUT_GAIN, m_dMp3AgcInputGain);
	DDV_MinMaxDouble(pDX, m_dMp3AgcInputGain, -65535, 30);
	DDX_Text(pDX, IDC_EDT_MIDI_AGC_INPUT_GAIN, m_dMidiAgcInputGain);
	DDV_MinMaxDouble(pDX, m_dMidiAgcInputGain, -65535, 30);
	DDX_Text(pDX, IDC_EDT_MP4_AGC_INPUT_GAIN, m_dMp4AgcInputGain);
	DDV_MinMaxDouble(pDX, m_dMp4AgcInputGain, -65535, 30);
	DDX_Text(pDX, IDC_EDT_VOICE_AGC_INPUT_GAIN, m_dVoiceAgcInputGain);
	DDV_MinMaxDouble(pDX, m_dVoiceAgcInputGain, -65535, 30);
	DDX_CBIndex(pDX, IDC_COMBO_VDG_POSITION, m_nDigitalGain);
	DDX_CBIndex(pDX, IDC_COMBO_EQ_SELECT, m_nEQSelect);
	DDX_CBIndex(pDX, IDC_COMBO_SAMPLING_RATE, m_nSamplingRate);
	DDX_CBIndex(pDX, IDC_COMBO_LOWCUT_OFF_FREQUENCY, m_nLowcutOffFrequency);
	DDX_CBIndex(pDX, IDC_COMBO_BOOSTING_GAIN, m_nBoostingGain);
	DDX_CBIndex(pDX, IDC_COMBO_CLARITY_LEVEL, m_nClarityLevel);
	DDX_CBIndex(pDX, IDC_COMBO_VOLUME_TYPE, m_nVolumeType);
	DDX_Control(pDX, IDC_COMBO_VOLUME_TYPE, m_boxVolumeTYpe);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_SEL_LEFT, m_nCBChannelSelLeft);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_SEL_RIGHT, m_nCBChannelSelRight);
	DDX_CBIndex(pDX, IDC_COMBO_POLAR_LEFT, m_nPolarLeft);
	DDX_CBIndex(pDX, IDC_COMBO_POLAR_RIGHT, m_nPolarRight);
	DDX_Text(pDX, IDC_EDIT_AGC_ATTACK, m_nAgcAttack);
	DDV_MinMaxInt(pDX, m_nAgcAttack, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_AGC_RELEASE, m_nAgcRelease);
	DDV_MinMaxInt(pDX, m_nAgcRelease, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_AGC_HOLD_HC, m_nAgcHoldHc);
	DDV_MinMaxInt(pDX, m_nAgcHoldHc, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_AGC_SDELAY, m_nAgcSdelay);
	DDV_MinMaxInt(pDX, m_nAgcSdelay, 0, 32767);
	DDX_CBIndex(pDX, IDC_COMBO_LCF_POSITION, m_nLcfPosition);
	DDX_CBIndex(pDX, IDC_COMBO_MIXER_SW, m_bMixerSw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAudioModePage, CPropertyPage)
	//{{AFX_MSG_MAP(CAudioModePage)
	ON_BN_CLICKED(IDC_BUTTON_AD_DEVICE, OnButtonAdDevice)
	ON_BN_CLICKED(IDC_BUTTON_DA_DEVICE, OnButtonDaDevice)
	ON_BN_CLICKED(IDC_BUTTON_DL_PGA_GAIN, OnButtonDlPgaGain)
	ON_BN_CLICKED(IDC_BUTTON_UL_PGA_GAIN, OnButtonUlPgaGain)
	ON_BN_CLICKED(IDC_BUTTON_VOLUME_CTRL, OnButtonVolumeCtrl)
	ON_BN_CLICKED(IDC_BUTTON_DL_DIGITAL_GAIN, OnButtonDlDigitalGain)
	ON_BN_CLICKED(IDC_BUTTON_UL_DIGITAL_GAIN, OnButtonUlDigitalGain)
	ON_BN_CLICKED(IDC_BUTTON_DL_EQ, OnButtonDlEq)
	ON_BN_CLICKED(IDC_BUTTON_UL_EQ, OnButtonUlEq)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_SIDETONE, OnSelchangeComboSidetone)
	ON_CBN_SELCHANGE(IDC_COMBO_VOLUME_MODE, OnSelchangeComboVolumeMode)
	ON_CBN_SELCHANGE(IDC_COMBO_AEC, OnSelchangeComboAec)
	ON_CBN_SELCHANGE(IDC_COMBO_SAMPLERATE, OnSelchangeComboSamplerate)
	ON_BN_CLICKED(IDC_BUTTON_SIDETONE_GAIN, OnButtonSidetoneGain)
	ON_BN_CLICKED(IDC_BUTTON_AGC, OnButtonAgc)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_AUDIO_WRITE_CURRENT_MODE_TO_PHONE, OnButtonAudioWriteCurrentModeToPhone)
	ON_COMMAND(ID_BUTTON_AUDIO_WRITE_TO_PHONE, OnButtonAudioWriteToPhone)
	ON_BN_CLICKED(IDC_BUTTON_POP, OnButtonPop)
	ON_BN_CLICKED(IDC_BUTTON_NG, OnButtonNg)
	ON_BN_CLICKED(IDC_BUTTON_AEC, OnButtonAec)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEAKER, OnSelchangeComboSpeaker)
	ON_CBN_SELCHANGE(IDC_COMBO_MUTE, OnSelchangeComboMute)
	ON_BN_CLICKED(IDC_BUTTON_EXTEND_DATA, OnButtonExtendData)
	ON_BN_CLICKED(IDC_BUTTON_DL_FIR_EQ_COEFF, OnButtonDlFirEqCoeff)
	ON_BN_CLICKED(IDC_BUTTON_ST_FIR_EQ_COEFF, OnButtonStFirEqCoeff)
	ON_BN_CLICKED(IDC_BUTTON_UL_FIR_EQ_COEFF, OnButtonUlFirEqCoeff)
	ON_COMMAND(ID_BUTTON_AUDIO_WRITE_CURRENT_MODE_TO_FLASH, OnButtonAudioWriteCurrentModeToFlash)
	ON_BN_CLICKED(IDC_BUTTON_SET_DEFAULT_FIR, OnButtonSetDefaultFir)
	ON_BN_CLICKED(IDC_BUTTON_MORE_PARAMETER2, OnButtonMoreParameter2)
	ON_BN_CLICKED(IDC_BUTTON_MORE_PARAMETER3, OnButtonMoreParameter3)
	ON_BN_CLICKED(IDC_BUTTON_SET_DEFAULT_UL_FIR, OnButtonSetDefaultUlFir)
	ON_BN_CLICKED(IDC_BUTTON_SIDETONE_NG, OnButtonSidetoneNg)
	ON_BN_CLICKED(IDC_BUTTON_VOLUME, OnButtonVolume)
	ON_BN_CLICKED(IDC_BTN_EQ_SET, OnBtnEqSet)
	ON_COMMAND(ID_BUTTON_AUDIO_LOAD_CURRENT_MODE_FROM_FLASH, OnButtonAudioLoadCurrentModeFromFlash)
	ON_COMMAND(ID_BUTTON_AUDIO_LOAD_CURRENT_MODE_FROM_PHONE, OnButtonAudioLoadCurrentModeFromPhone)
	ON_BN_CLICKED(IDC_BTN_VOLUME_SET, OnBtnVolumeSet)
	ON_CBN_SELCHANGE(IDC_COMBO_SIDETONE_EQ, OnSelchangeComboSidetoneEq)
	ON_CBN_SELCHANGE(IDC_COMBO_SIDETONE_CP, OnSelchangeComboSidetoneCp)
	ON_CBN_SELCHANGE(IDC_COMBO_VOLUME_TYPE, OnSelchangeComboVolumeType)
	ON_BN_CLICKED(IDC_BUTTON_NXP_TX_PARAMETER, OnButtonNxpTxParameter)
	ON_BN_CLICKED(IDC_BUTTON_NXP_RX_PARAMETER, OnButtonNxpRxParameter)
	ON_BN_CLICKED(IDC_BUTTON_NXP_VOICE_BOOST_CTL, OnButtonNxpVoiceBoostCtl)
	ON_BN_CLICKED(IDC_BUTTON_DUMP_PCM_CTL, OnButtonDumpPcmCtl)
	ON_BN_CLICKED(IDC_BTN_SHOW_GRAPH, OnBtnShowGraph)
	ON_CBN_SELCHANGE(IDC_COMBO_EQ_SELECT, OnSelchangeComboEqSelect)
	ON_BN_CLICKED(IDC_BUTTON_PATH_SET, OnButtonPathSet)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CNG, OnButtonCng)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAudioModePage message handlers

void CAudioModePage::OnButtonAdDevice() 
{
    /*
    CString strText;
    m_edtAdDevice.GetWindowText( strText );
    if( !strText.Left(2).CompareNoCase( "0x" ) )
    {
        strText.Delete( 0,2 );
    }
    */
/*
    UINT uData = 0;
    sscanf( strText,"%x",&uData );
*/
	CAdjustDlg dlg;

	//teana hu 2012.01.31
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
	if(pSheet != NULL && pSheet->m_bLoadDspCodec)
	{
		dlg.Init( m_dwAdcSet ,IDS_ADC_TITLE );
		
		if( IDOK == dlg.DoModal() )
		{
			m_dwAdcSet = dlg.GetResult();
		}  	
	}
	else
	{
		dlg.Init( m_dwADDevice ,IDS_AD_TITLE );
		
		if( IDOK == dlg.DoModal() )
		{
			m_dwADDevice = dlg.GetResult();
			//    strText.Format( "0x%X",m_dwADDevice );
			//    m_edtAdDevice.SetWindowText( strText );
		}  
	}  
}

void CAudioModePage::OnButtonDaDevice() 
{
    /*
    CString strText;
    m_edtDaDevice.GetWindowText( strText );
    if( !strText.Left(2).CompareNoCase( "0x" ) )
    {
        strText.Delete( 0,2 );
    }
    
    UINT uData = 0;
    sscanf( strText,"%x",&uData );
    */

    CAdjustDlg dlg;
	
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
	if(pSheet != NULL && pSheet->m_bLoadDspCodec)
	{
		dlg.Init( m_dwDacSet,IDS_DAC_TITLE );
		
		if( IDOK == dlg.DoModal() )
		{
			m_dwDacSet = dlg.GetResult();
		} 
	}
	else
	{
		dlg.Init( m_dwDADevice,IDS_DA_TITLE );
		
		if( IDOK == dlg.DoModal() )
		{
			m_dwDADevice = dlg.GetResult();
			//strText.Format( "0x%X",m_dwDADevice );
			//m_edtDaDevice.SetWindowText( strText );
		} 
	}  
}

void CAudioModePage::OnButtonDlPgaGain() 
{
    /*
    CString strText;
    m_edtDlPgaGain.GetWindowText( strText );
    if( !strText.Left(2).CompareNoCase( "0x" ) )
    {
        strText.Delete( 0,2 );
    }
    
    UINT uData = 0;
    sscanf( strText,"%x",&uData );
    */
    CAdjustDlg dlg;
    dlg.Init( m_dwDlPgaGain,IDS_DPG_TITLE );
    
    if( IDOK == dlg.DoModal() )
    {
        m_dwDlPgaGain = dlg.GetResult();
        //strText.Format( "0x%X",m_dwDlPgaGain );
        //m_edtDlPgaGain.SetWindowText( strText );
    }   
}

void CAudioModePage::OnButtonUlPgaGain() 
{
    CAdjustDlg dlg;
    dlg.Init( m_dwUlPgaGain,IDS_UPG_TITLE );
    
    if( IDOK == dlg.DoModal() )
    {
        m_dwUlPgaGain = dlg.GetResult();
        //strText.Format( "0x%X",m_dwUlPgaGain );
        //m_edtUlPgaGain.SetWindowText( strText );
    } 
}

BOOL CAudioModePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    InitToolbar();

    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    pSheet->ChangeFont( this,&(pSheet->m_Font) );
    
	//teana hu 2011.03.22
	if(pSheet->m_bLoadENHA)
	{
		ShowSoundEffectsWindow(SW_HIDE);
	}
	else
	{
		ShowSoundEffectsWindow(SW_SHOW);
	}
	//

    RefreshControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAudioModePage::OnSetActive() 
{
    m_bActive = TRUE;

    CString strText;
    if( m_bXagcEnable )
    {
        strText.LoadString( IDS_XAGC );    
    }
    else
    {
        strText.LoadString( IDS_AEC );
    }

    GetDlgItem( IDC_BUTTON_AEC )->SetWindowText( strText );

	// When this page is created,the sound eq parameter is not ready
	// So we have to update sound eq version and relative controls here
	RefreshControls();

	return CPropertyPage::OnSetActive();
}

/*
void CAudioModePage::InitResouces()
{
    m_jpgToBmp.Init( IDR_JPG_DSP_SW1,"JPG" );
    HBITMAP hBitmap = (HBITMAP)m_jpgToBmp;
    m_vResource.push_back( hBitmap );    

    m_jpgToBmp.Init( IDR_JPG_DSP_SW1,"JPG" );
    hBitmap = (HBITMAP)m_jpgToBmp;
    m_vResource.push_back( hBitmap );   

    m_jpgToBmp.Init( IDR_JPG_DSP_CHIP1,"JPG" );
    hBitmap = (HBITMAP)m_jpgToBmp;
    m_vResource.push_back( hBitmap );   

    m_jpgToBmp.Init( IDR_JPG_DSP_CHIP2,"JPG" );
    hBitmap = (HBITMAP)m_jpgToBmp;
    m_vResource.push_back( hBitmap );   
    
    m_jpgToBmp.Init( IDR_JPG_DSP_CHIP3,"JPG" );
    hBitmap = (HBITMAP)m_jpgToBmp;
    m_vResource.push_back( hBitmap );   

    m_jpgToBmp.Init( IDR_JPG_DSP_CHIP4,"JPG" );
    hBitmap = (HBITMAP)m_jpgToBmp;
    m_vResource.push_back( hBitmap );   

    m_jpgToBmp.Init( IDR_JPG_DSP_CHIP5,"JPG" );
    hBitmap = (HBITMAP)m_jpgToBmp;
    m_vResource.push_back( hBitmap );   

    m_jpgToBmp.Init( IDR_JPG_DSP_CHIP6,"JPG" );
    hBitmap = (HBITMAP)m_jpgToBmp;
    m_vResource.push_back( hBitmap );   

    m_jpgToBmp.Init( IDR_JPG_DSP_HW1,"JPG" );
    hBitmap = (HBITMAP)m_jpgToBmp;
    m_vResource.push_back( hBitmap );   

    m_jpgToBmp.Init( IDR_JPG_DSP_HW2,"JPG" );
    hBitmap = (HBITMAP)m_jpgToBmp;
    m_vResource.push_back( hBitmap );   
}
*/

void CAudioModePage::OnButtonVolumeCtrl() 
{
    CVolumeCtrlDlg dlg;
    dlg.m_pVolumeCtrlArea = (SpecialRectOfVolumeCtrl*)FindAreadByName("Volume control");
	//teana hu 2011.05.09
	SpecialRectOfVolume* pArea = (SpecialRectOfVolume*)FindAreadByName("volume_settings");
	CAudioTesterSheet *pSheet = (CAudioTesterSheet *)GetParent();
	if(pSheet == NULL)
	{
		return;
	}
	if(pArea != NULL)
	{
		//teana hu 2012.03.29
		if(pSheet->m_bDspCodec)
		{
			//dlg.m_pVolumeCtrlArea->m_nLevel = pArea->m_nLevel6530;
			dlg.m_pVolumeCtrlArea->m_nLevel = pArea->m_nLevel;
		}
		else
		{
			dlg.m_pVolumeCtrlArea->m_nLevel = pArea->m_nLevel;
		}
		if(m_nVolumeLevel == -1)
		{
			dlg.m_pVolumeCtrlArea->m_iValue = dlg.m_pVolumeCtrlArea->m_nLevel - 1;
		}
	}
	//
    if( IDOK == dlg.DoModal() )
    {
        m_strVolume = dlg.m_pVolumeCtrlArea->m_strOtherValue;
        UpdateData( FALSE );
       
        pSheet->SetVolume( dlg.m_pVolumeCtrlArea->m_iValue );
		m_nVolumeLevel = dlg.m_pVolumeCtrlArea->m_iValue;
    }
}

void CAudioModePage::InitAreas( void* pPara )
{
    SpecialRect::AREA_OF_MAP areasOfSinglePage;
    
    //Volume control
    SpecialRect* pTempArea = new SpecialRectOfVolumeCtrl;
    pTempArea->m_rectArea = CRect(145,36,236,61);
    pTempArea->m_rectDefaultArea = CRect(145,36,236,61);
    pTempArea->m_rectValueArea = CRect(145,36,236,61);
    pTempArea->m_strOtherValue = "0db";
    pTempArea->m_strUnit = "";
    pTempArea->m_strName = "Volume control";
    pTempArea->m_enumParaType = SpecialRect::volume_ctrl;
    ((SpecialRectOfVolumeCtrl*)pTempArea)->m_iValue = 8;
    areasOfSinglePage.push_back( pTempArea );   

    //DL_Digital_gain
    pTempArea = new SpecialRectOfGain;
    pTempArea->m_rectArea = CRect(299,50,436,68);
    pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
    pTempArea->m_rectValueArea = CRect(299,50,436,79);
    pTempArea->m_strOtherValue = "0.0";
    pTempArea->m_strUnit = " db ";
    pTempArea->m_strName = "DL_Digital_gain";
    pTempArea->m_enumParaType = SpecialRect::dl_digital_gain;
    areasOfSinglePage.push_back( pTempArea );
    
    //UL_digital_gain
    pTempArea = new SpecialRectOfGain;
    pTempArea->m_rectArea = CRect(299,50,436,68);
    pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
    pTempArea->m_rectValueArea = CRect(299,50,436,79);
    pTempArea->m_strOtherValue = "0.0";
    pTempArea->m_strUnit = " db ";
    pTempArea->m_strName = "UL_digital_gain";
    pTempArea->m_enumParaType = SpecialRect::ul_digital_gain;
    areasOfSinglePage.push_back( pTempArea );

    if( m_bFir )
    {
        // Use old fir adjustment method
        pTempArea = new SpecialRectOfCoefficient;
        pTempArea->m_rectArea = CRect(299,50,436,68);
        pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
        pTempArea->m_rectValueArea = CRect(299,50,436,79);
        pTempArea->m_strOtherValue = "0.0";
        pTempArea->m_strUnit = " db ";
        pTempArea->m_strName = "DL_FIR";
        pTempArea->m_enumParaType = SpecialRect::dl_fir;
        ((SpecialRectOfEQ*)pTempArea)->m_pSampleRate = &m_nSampleRateValue;
        areasOfSinglePage.push_back( pTempArea );
    }
    else
    {
        // DL EQ
        pTempArea = new SpecialRectOfEQ;
        pTempArea->m_rectArea = CRect(299,50,436,68);
        pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
        pTempArea->m_rectValueArea = CRect(299,50,436,79);
        pTempArea->m_strOtherValue = "0.0";
        pTempArea->m_strUnit = " db ";
        pTempArea->m_strName = "DL_EQ";
        pTempArea->m_enumParaType = SpecialRect::dl_eq;
        ((SpecialRectOfEQ*)pTempArea)->m_pSampleRate = &m_nSampleRateValue;
        areasOfSinglePage.push_back( pTempArea );
    }
    
    //UL EQ
    pTempArea = new SpecialRectOfEQ;
    pTempArea->m_rectArea = CRect(299,50,436,68);
    pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
    pTempArea->m_rectValueArea = CRect(299,50,436,79);
    pTempArea->m_strOtherValue = "0.0";
    pTempArea->m_strUnit = " db ";
    pTempArea->m_strName = "UL EQ";
    pTempArea->m_enumParaType = SpecialRect::ul_eq;
    ((SpecialRectOfEQ*)pTempArea)->m_pSampleRate = &m_nSampleRateValue;
    areasOfSinglePage.push_back( pTempArea );

    //digital sidetone gain
    pTempArea = new SpecialRect;
    pTempArea->m_rectArea = CRect(299,50,436,68);
    pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
    pTempArea->m_rectValueArea = CRect(299,50,436,79);
    pTempArea->m_strOtherValue = "0.0";
    pTempArea->m_strUnit = " db ";
    pTempArea->m_strName = "digital_sidetone_gain";
    pTempArea->m_enumParaType = SpecialRect::digital_sidetone_gain;
    areasOfSinglePage.push_back( pTempArea );

    // Agc switch
    pTempArea = new SpecialRectOfAgc;
    pTempArea->m_rectArea = CRect(299,50,436,68);
    pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
    pTempArea->m_rectValueArea = CRect(299,50,436,79);
    pTempArea->m_strOtherValue = "";
    pTempArea->m_strUnit = "";
    pTempArea->m_strName = "AGC Switch";
    pTempArea->m_enumParaType = SpecialRect::agc_switch;
    areasOfSinglePage.push_back( pTempArea );

    // Pop
    pTempArea = new SpecialRectOfPop;
    pTempArea->m_rectArea = CRect(299,50,436,68);
    pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
    pTempArea->m_rectValueArea = CRect(299,50,436,79);
    pTempArea->m_strOtherValue = "";
    pTempArea->m_strUnit = "";
    pTempArea->m_strName = "Pop";
    pTempArea->m_enumParaType = SpecialRect::pop;
    areasOfSinglePage.push_back( pTempArea );
    
    // Ng
    if( !m_bFir )
    {
        pTempArea = new SpecialRectOfNg;
        pTempArea->m_rectArea = CRect(299,50,436,68);
        pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
        pTempArea->m_rectValueArea = CRect(299,50,436,79);
        pTempArea->m_strOtherValue = "";
        pTempArea->m_strUnit = "";
        pTempArea->m_strName = "Ng";
        pTempArea->m_enumParaType = SpecialRect::ng;
        areasOfSinglePage.push_back( pTempArea );
    }
    
    // AEC
    pTempArea = new SpecialRectOfAec;
    pTempArea->m_rectArea = CRect(299,50,436,68);
    pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
    pTempArea->m_rectValueArea = CRect(299,50,436,79);
    pTempArea->m_strOtherValue = "";
    pTempArea->m_strUnit = "";
    pTempArea->m_strName = "AEC";
    pTempArea->m_enumParaType = SpecialRect::aec;
    areasOfSinglePage.push_back( pTempArea );
    
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)pPara;
    
    if( pSheet->IsExtend() )
    {
        // DL fir eq coeff
        pTempArea = new SpecialRectOfCoefficient;
        pTempArea->m_rectArea = CRect(299,50,436,68);
        pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
        pTempArea->m_rectValueArea = CRect(299,50,436,79);
        pTempArea->m_strOtherValue = "0.0";
        pTempArea->m_strUnit = " db ";
        pTempArea->m_strName = "DL_FIR_EQ_COEFF";
        pTempArea->m_enumParaType = SpecialRect::dl_fir_eq_coeff;
        ((SpecialRectOfEQ*)pTempArea)->m_pSampleRate = &m_nSampleRateValue;
        areasOfSinglePage.push_back( pTempArea );
        
        // UL fir eq coeff
        pTempArea = new SpecialRectOfCoefficient;
        pTempArea->m_rectArea = CRect(299,50,436,68);
        pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
        pTempArea->m_rectValueArea = CRect(299,50,436,79);
        pTempArea->m_strOtherValue = "0.0";
        pTempArea->m_strUnit = " db ";
        pTempArea->m_strName = "UL_FIR_EQ_COEFF";
        pTempArea->m_enumParaType = SpecialRect::ul_fir_eq_coeff;
        ((SpecialRectOfEQ*)pTempArea)->m_pSampleRate = &m_nSampleRateValue;
        areasOfSinglePage.push_back( pTempArea );
        
        // ST fir eq coeff
        pTempArea = new SpecialRectOfCoefficient;
        pTempArea->m_rectArea = CRect(299,50,436,68);
        pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
        pTempArea->m_rectValueArea = CRect(299,50,436,79);
        pTempArea->m_strOtherValue = "0.0";
        pTempArea->m_strUnit = " db ";
        pTempArea->m_strName = "ST_FIR_EQ_COEFF";
        pTempArea->m_enumParaType = SpecialRect::st_fir_eq_coeff;
        ((SpecialRectOfEQ*)pTempArea)->m_pSampleRate = &m_nSampleRateValue;
        areasOfSinglePage.push_back( pTempArea );

		if( pSheet->IsVer3() )
		{
			// dl settings
			pTempArea = new SpecialRectOfDlSettings;
			pTempArea->m_rectArea = CRect(299,50,436,68);
			pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
			pTempArea->m_rectValueArea = CRect(299,50,436,79);
			pTempArea->m_strOtherValue = "0.0";
			pTempArea->m_strUnit = " db ";
			pTempArea->m_strName = "Dl_Setting";
			pTempArea->m_enumParaType = SpecialRect::dl_settings;
			areasOfSinglePage.push_back( pTempArea );
			
			// ul settings
			pTempArea = new SpecialRectOfUlSettings;
			pTempArea->m_rectArea = CRect(299,50,436,68);
			pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
			pTempArea->m_rectValueArea = CRect(299,50,436,79);
			pTempArea->m_strOtherValue = "0.0";
			pTempArea->m_strUnit = " db ";
			pTempArea->m_strName = "Ul_Setting";
			pTempArea->m_enumParaType = SpecialRect::ul_settings;
			areasOfSinglePage.push_back( pTempArea );
		}

// 		if( m_bAudio3Switch )
//		{
			
//		}
		// st ng settings
		pTempArea = new SpecialRectOfStng;
		pTempArea->m_rectArea = CRect(299,50,436,68);
		pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
		pTempArea->m_rectValueArea = CRect(299,50,436,79);
		pTempArea->m_strOtherValue = "0.0";
		pTempArea->m_strUnit = " db ";
		pTempArea->m_strName = "st_ng";
		pTempArea->m_enumParaType = SpecialRect::st_ng;
		areasOfSinglePage.push_back( pTempArea );
			
		// audio 3.0 volume settings
		pTempArea = new SpecialRectOfVolume;
		pTempArea->m_rectArea = CRect(299,50,436,68);
		pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
		pTempArea->m_rectValueArea = CRect(299,50,436,79);
		pTempArea->m_strOtherValue = "0.0";
		pTempArea->m_strUnit = " db ";
		pTempArea->m_strName = "volume_settings";
		pTempArea->m_enumParaType = SpecialRect::volume_settings;
		areasOfSinglePage.push_back( pTempArea );

		//teana hu 2010.08.13
		//arm volume
		pTempArea = new SpecialRectOfArmVolume;
		pTempArea->m_rectArea = CRect(299,50,436,68);
		pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
		pTempArea->m_rectValueArea = CRect(299,50,436,79);
		pTempArea->m_strOtherValue = "0.0";
		pTempArea->m_strUnit = " db ";
		pTempArea->m_strName = "arm_volume_settings";
		pTempArea->m_enumParaType = SpecialRect::arm_volume_setting;
		areasOfSinglePage.push_back( pTempArea );

		// DL fir eq coeff
        pTempArea = new SpecialRectOfCoefficientEx;
        pTempArea->m_rectArea = CRect(299,50,436,68);
        pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
        pTempArea->m_rectValueArea = CRect(299,50,436,79);
        pTempArea->m_strOtherValue = "0.0";
        pTempArea->m_strUnit = " db ";
        pTempArea->m_strName = "DL_FIR_EQ_COEFF_EX";
        pTempArea->m_enumParaType = SpecialRect::dl_fir_eq_coeff;
        ((SpecialRectOfEQ*)pTempArea)->m_pSampleRate = &m_nSampleRateValue;
        areasOfSinglePage.push_back( pTempArea );
        
        // UL fir eq coeff
        pTempArea = new SpecialRectOfCoefficientEx;
        pTempArea->m_rectArea = CRect(299,50,436,68);
        pTempArea->m_rectDefaultArea = CRect(299,50,436,68);
        pTempArea->m_rectValueArea = CRect(299,50,436,79);
        pTempArea->m_strOtherValue = "0.0";
        pTempArea->m_strUnit = " db ";
        pTempArea->m_strName = "UL_FIR_EQ_COEFF_EX";
        pTempArea->m_enumParaType = SpecialRect::ul_fir_eq_coeff;
        ((SpecialRectOfEQ*)pTempArea)->m_pSampleRate = &m_nSampleRateValue;
        areasOfSinglePage.push_back( pTempArea );

    }

    m_vAllAreas = areasOfSinglePage;
}

void CAudioModePage::OnButtonDlDigitalGain() 
{
	CDigitalGainDlg dlg;
    dlg.m_pArea = (SpecialRectOfGain*)FindAreadByName( "DL_Digital_gain" );

    dlg.DoModal();
}

void CAudioModePage::OnButtonUlDigitalGain() 
{
    CDigitalGainDlg dlg;
    dlg.m_pArea = (SpecialRectOfGain*)FindAreadByName( "UL_digital_gain" );
    
    dlg.DoModal();
}

int gnFirType;

void CAudioModePage::OnButtonDlEq() 
{
    if( m_bFir )
    {
        gnFirType = SpecialRect::dl_fir;
        CCoefficientDlg dlg;
        dlg.m_pArea = (SpecialRectOfCoefficient*)FindAreadByName( "DL_FIR" );
        dlg.m_nSampleRate = m_nSampleRateValue;
        dlg.DoModal();
    }
    else
    {
        CEqDlg dlg;	
        dlg.m_pArea = (SpecialRectOfEQ*)FindAreadByName( "DL_EQ" );
        dlg.EnableMid( TRUE );
        dlg.DoModal();
    }
}

void CAudioModePage::OnButtonUlEq() 
{
    CEqDlg dlg;	
    dlg.m_pArea = (SpecialRectOfEQ*)FindAreadByName( "UL EQ" );
    dlg.EnableMid( FALSE );
    dlg.DoModal();
}

void CAudioModePage::ClearAreas()
{
    for( UINT i=0;i<m_vAllAreas.size();i++ )
    {
        delete m_vAllAreas[i];
    }

    m_vAllAreas.clear();
}

void CAudioModePage::OnDestroy() 
{
	CPropertyPage::OnDestroy();
}

void CAudioModePage::WriteToFile( CArchive& /*ofFile*/ )
{
    // There is nothing need to be written to the file
}

void CAudioModePage::LoadFromFile( CArchive& /*ofFile*/ )
{
    // Nothing to load
}

void CAudioModePage::OnSelchangeComboSidetone() 
{
    m_nSideTone = ((CComboBox*)GetDlgItem( IDC_COMBO_SIDETONE ))->GetCurSel();    	
}

void CAudioModePage::OnSelchangeComboVolumeMode() 
{
	m_nVolume = ((CComboBox*)GetDlgItem( IDC_COMBO_VOLUME_MODE ))->GetCurSel();    	
}

void CAudioModePage::OnSelchangeComboAec() 
{
	m_nAec = ((CComboBox*)GetDlgItem( IDC_COMBO_AEC ))->GetCurSel();    		
}

void CAudioModePage::OnSelchangeComboSamplerate() 
{
	m_nSampleRate = ((CComboBox*)GetDlgItem( IDC_COMBO_SAMPLERATE ))->GetCurSel();
    m_nSampleRateValue = ::CalcSampleRate( m_nSampleRate );
//    SetSampleRate( m_nSampleRateValue );
}

void CAudioModePage::RefreshControls()
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();

	if( m_nEQSelect > 0 && pSheet->m_EQData.eq_count > 0 )
	{
		m_nSoundEqVer = GET_SOUND_EQ_VER( pSheet->m_EQData.lpEQMode[m_nEQSelect-1].eq_mode_control );
	}
	else
	{
		m_nSoundEqVer = sound_eq_ver_none;
	}


	if(pSheet->m_bLoadVolume)
	{
		EnableSoundEffectsWindow(TRUE);

		//teana hu 2011.06.16
		GetDlgItem(IDC_BTN_SHOW_GRAPH)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CHANNEL_SEL_LEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CHANNEL_SEL_RIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_POLAR_LEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_POLAR_RIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_AGC_ATTACK)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_AGC_RELEASE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_AGC_HOLD_HC)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_AGC_SDELAY)->EnableWindow(TRUE);
		//
		GetDlgItem( IDC_COMBO_MIXER_SW )->EnableWindow( TRUE );

		if(m_nVolumeCount == 0)
		{
			GetDlgItem(IDC_COMBO_VOLUME_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_VOLUME_TYPE)->ShowWindow(SW_HIDE);
			if(!pSheet->m_bLoadEQ)
			{
				EnableSoundEffectsWindow(pSheet->m_bLoadEQ);
			}
			//teana hu 2011.06.16
			GetDlgItem(IDC_BTN_SHOW_GRAPH)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_CHANNEL_SEL_LEFT)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_CHANNEL_SEL_RIGHT)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_POLAR_LEFT)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_POLAR_RIGHT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_AGC_ATTACK)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_AGC_RELEASE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_AGC_HOLD_HC)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_AGC_SDELAY)->EnableWindow(FALSE);
			//
			GetDlgItem( IDC_COMBO_MIXER_SW )->EnableWindow( FALSE );
		}
		else
		{
			for(int i = 0; i <m_nVolumeCount; i++)
			{
				CString strText;
				strText.Format(_T("%d"), i);
				m_boxVolumeTYpe.AddString(strText);
			}
			m_boxVolumeTYpe.SetCurSel(0);
		}
	}
	else
	{
		EnableSoundEffectsWindow(pSheet->m_bLoadEQ);
		GetDlgItem(IDC_COMBO_VOLUME_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_VOLUME_TYPE)->ShowWindow(SW_HIDE);
		//teana hu 2011.06.16
		GetDlgItem(IDC_BTN_SHOW_GRAPH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CHANNEL_SEL_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CHANNEL_SEL_RIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_POLAR_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_POLAR_RIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AGC_ATTACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AGC_RELEASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AGC_HOLD_HC)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AGC_SDELAY)->EnableWindow(FALSE);
		//
		GetDlgItem( IDC_COMBO_MIXER_SW )->EnableWindow( FALSE );
	}

	BOOL b_sound_effect = sound_eq_ver_4 == m_nSoundEqVer;
	BOOL b_zc_switch_show = ( pSheet->m_bLoadVolume && pSheet->m_bLoadEQ && eq_platform_6800_only == pSheet->m_EQData.eq_platform )  || 
		                    ( pSheet->m_bLoadEQ && eq_platform_all == pSheet->m_EQData.eq_platform );
	b_zc_switch_show = b_zc_switch_show && b_sound_effect;

	BOOL b_mp4_switch = b_zc_switch_show && eq_platform_6800_only == pSheet->m_EQData.eq_platform && sound_eq_ver_4 == m_nSoundEqVer;
	GetDlgItem(IDC_COMBO_MP3_AGC_ZC_SWITCH)->EnableWindow(b_zc_switch_show);
	GetDlgItem(IDC_COMBO_MIDI_AGC_ZC_SWITCH)->EnableWindow(b_zc_switch_show);
	GetDlgItem(IDC_COMBO_MP4_AGC_ZC_SWITCH)->EnableWindow(b_mp4_switch);
	GetDlgItem(IDC_EDT_MP3_AGC_START_GAIN)->EnableWindow(b_zc_switch_show);
	GetDlgItem(IDC_EDT_MIDI_AGC_START_GAIN)->EnableWindow(b_zc_switch_show);
	GetDlgItem(IDC_EDT_MP4_AGC_START_GAIN)->EnableWindow(b_mp4_switch);

	GetDlgItem(IDC_COMBO_MP4_AGC_SWITCH)->EnableWindow(b_mp4_switch);
	GetDlgItem(IDC_COMBO_MP4_LCF_SWITCH)->EnableWindow(b_mp4_switch);
	GetDlgItem(IDC_EDT_MP4_AGC_INPUT_GAIN)->EnableWindow(b_mp4_switch);

	GetDlgItem( IDC_COMBO_MP3_AGC_SWITCH )->EnableWindow( pSheet->m_bLoadEQ );
	GetDlgItem(IDC_COMBO_MP3_LCF_SWITCH)->EnableWindow( pSheet->m_bLoadEQ );
	GetDlgItem(IDC_EDT_MP3_AGC_INPUT_GAIN)->EnableWindow( pSheet->m_bLoadEQ );
	GetDlgItem(IDC_COMBO_MIDI_AGC_SWITCH)->EnableWindow( pSheet->m_bLoadEQ );
	GetDlgItem(IDC_COMBO_MIDI_LCF_SWITCH)->EnableWindow( pSheet->m_bLoadEQ );
	GetDlgItem(IDC_EDT_MIDI_AGC_INPUT_GAIN)->EnableWindow( pSheet->m_bLoadEQ );


	GetDlgItem(IDC_BTN_EQ_SET)->EnableWindow( b_zc_switch_show && pSheet->m_bLoadEQ && m_nEQSelect > 0 );

	GetDlgItem(IDC_COMBO_VOICE_AGC_SWITCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_VOICE_LCF_SWITCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDT_VOICE_AGC_INPUT_GAIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_VOICE_AGC_ZC_SWITCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDT_VOICE_AGC_START_GAIN)->EnableWindow(FALSE);
	//

	//teana hu 2011.02.14
	if(m_bAudio4)
	{
		GetDlgItem(IDC_BUTTON_AGC)->SetWindowText(_T("ALC Switch"));
	}
	else
	{
		GetDlgItem(IDC_BUTTON_AGC)->SetWindowText(_T("AGC Switch"));
		GetDlgItem(IDC_BUTTON_AGC)->EnableWindow(FALSE);
	}
	
	//

	//teana hu 2011.04.20
	if(m_bExistLVVE)
	{
		GetDlgItem(IDC_BUTTON_NXP_TX_PARAMETER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_NXP_RX_PARAMETER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_NXP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_NXP_VOICE_BOOST_CTL)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_NXP_TX_PARAMETER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_NXP_RX_PARAMETER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_NXP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_NXP_VOICE_BOOST_CTL)->ShowWindow(SW_HIDE);
	}

	if(m_strTitle == _T("Handsfree") && pSheet->m_bLoadVolume)
	{
		GetDlgItem(IDC_BUTTON_DUMP_PCM_CTL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_NXP)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_DUMP_PCM_CTL)->ShowWindow(SW_HIDE);
		if(!m_bExistLVVE)
		{
			GetDlgItem(IDC_STATIC_NXP)->ShowWindow(SW_HIDE);
		}
	}
	//

	//teana hu 2011.05.20
	if(pSheet->m_bLoadLVVE && (m_strTitle == _T("HeadsetTD") || m_strTitle == _T("HandsfreeTD") || m_strTitle == _T("HeadfreeTD") || m_strTitle == _T("HandsetTD")))
	{
		GetDlgItem(IDC_BUTTON_NXP_VOICE_BOOST_CTL)->ShowWindow(SW_SHOW);
	}
	
    ((CComboBox*)GetDlgItem( IDC_COMBO_SIDETONE ))->SetCurSel( m_nSideTone );
    ((CComboBox*)GetDlgItem( IDC_COMBO_VOLUME_MODE ))->SetCurSel( m_nVolume );    	
	((CComboBox*)GetDlgItem( IDC_COMBO_AEC ))->SetCurSel( m_nAec );    
    ((CComboBox*)GetDlgItem( IDC_COMBO_SAMPLERATE ))->SetCurSel( m_nSampleRate );
 
    // Volume data
    SpecialRect* pArea = FindAreadByName( "Volume control" );
    m_strVolume = pArea->m_strOtherValue;

    // Sidetone gain
    pArea = FindAreadByName( "digital_sidetone_gain" );
    m_strSidetoneGain = pArea->m_strOtherValue+pArea->m_strUnit;

    pArea = FindAreadByName( "AGC Switch" );
    RefreshAgcCtrl( ((SpecialRectOfAgc*)pArea)->m_iValue );

    CString strText;
   
    if( m_bFir )
    {
        strText.LoadString( IDS_BUTTON_DL_FIR );
    }
    else
    {
        strText.LoadString( IDS_BUTTON_DL_EQ );
    }
    GetDlgItem( IDC_BUTTON_DL_EQ )->SetWindowText( strText );
    GetDlgItem( IDC_BUTTON_NG )->EnableWindow( !m_bFir );
    
	if( !pSheet->IsExtend() )
    {
        // Disable some buttons
        GetDlgItem( IDC_BUTTON_ST_FIR_EQ_COEFF )->EnableWindow( FALSE );
        GetDlgItem( IDC_BUTTON_DL_FIR_EQ_COEFF )->EnableWindow( FALSE );
        GetDlgItem( IDC_BUTTON_UL_FIR_EQ_COEFF )->EnableWindow( FALSE );
        GetDlgItem( IDC_BUTTON_EXTEND_DATA )->EnableWindow( FALSE );
    }

	if( !pSheet->IsVer3() || !m_bAudio3Switch )
	{
        GetDlgItem( IDC_BUTTON_MORE_PARAMETER2 )->EnableWindow( FALSE );
        GetDlgItem( IDC_BUTTON_MORE_PARAMETER3 )->EnableWindow( FALSE );
	}

	if( m_bAudio3Switch )
	{
		GetDlgItem( IDC_BUTTON_NG )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_DL_DIGITAL_GAIN )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_UL_DIGITAL_GAIN )->EnableWindow( FALSE );
	}

	if(m_bVolumeSwitch)
	{
		GetDlgItem( IDC_BUTTON_VOLUME )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON_SIDETONE_GAIN )->EnableWindow( TRUE );
		GetDlgItem(IDC_BUTTON_UL_PGA_GAIN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem( IDC_BUTTON_VOLUME )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_SIDETONE_GAIN )->EnableWindow( FALSE );
		GetDlgItem(IDC_BUTTON_UL_PGA_GAIN)->EnableWindow(TRUE);
	}
	
	((CComboBox*)GetDlgItem( IDC_COMBO_SAMPLERATE ))->SetCurSel( m_nSampleRate );

// 	if( m_nEQSelect == 0 || !pSheet->m_bLoadENHA )
// 	{
// 		GetDlgItem( IDC_BTN_EQ_SET )->EnableWindow( FALSE );
// 	}
// 	else
// 	{
// 		GetDlgItem( IDC_BTN_EQ_SET )->EnableWindow( TRUE );
// 	}

	if( sound_eq_ver_4 == m_nSoundEqVer )
	{
		GetDlgItem( IDC_COMBO_VOICE_AGC_ZC_SWITCH )->EnableWindow( pSheet->m_bLoadENHA );
		
		GetDlgItem( IDC_EDT_VOICE_AGC_START_GAIN )->EnableWindow( pSheet->m_bLoadENHA );
		
		GetDlgItem( IDC_COMBO_CHANNEL_SEL_LEFT )->EnableWindow( TRUE );
		GetDlgItem( IDC_COMBO_POLAR_LEFT )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_AGC_ATTACK )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_AGC_HOLD_HC )->EnableWindow( TRUE );
		
		GetDlgItem( IDC_COMBO_CHANNEL_SEL_RIGHT )->EnableWindow( TRUE );
		GetDlgItem( IDC_COMBO_POLAR_RIGHT )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_AGC_RELEASE )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_AGC_SDELAY )->EnableWindow( TRUE );

		GetDlgItem( IDC_COMBO_MIXER_SW )->EnableWindow( TRUE );
		
		GetDlgItem( IDC_BTN_SHOW_GRAPH )->EnableWindow( TRUE );
		
		GetDlgItem( IDC_COMBO_LCF_POSITION )->EnableWindow( TRUE );
		GetDlgItem( IDC_COMBO_VDG_POSITION )->EnableWindow( FALSE );
	}
	else
	{
		GetDlgItem( IDC_COMBO_VOICE_AGC_ZC_SWITCH )->EnableWindow( FALSE );
		
		GetDlgItem( IDC_EDT_VOICE_AGC_START_GAIN )->EnableWindow( FALSE );
		
		GetDlgItem( IDC_COMBO_CHANNEL_SEL_LEFT )->EnableWindow( FALSE );
		GetDlgItem( IDC_COMBO_POLAR_LEFT )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_AGC_ATTACK )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_AGC_HOLD_HC )->EnableWindow( FALSE );
		
		GetDlgItem( IDC_COMBO_CHANNEL_SEL_RIGHT )->EnableWindow( FALSE );
		GetDlgItem( IDC_COMBO_POLAR_RIGHT )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_AGC_RELEASE )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_AGC_SDELAY )->EnableWindow( FALSE );

		GetDlgItem( IDC_COMBO_MIXER_SW )->EnableWindow( FALSE );
		
		GetDlgItem( IDC_BTN_SHOW_GRAPH )->EnableWindow( FALSE );

		GetDlgItem( IDC_COMBO_LCF_POSITION )->EnableWindow( FALSE );
		GetDlgItem( IDC_COMBO_VDG_POSITION )->EnableWindow( TRUE );
	}

	//teana hu 2012.01.31
	if(m_bExistDspCodec && m_bDspCodecInfo)
	{
		GetDlgItem(IDC_BUTTON_PATH_SET)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_DA_DEVICE)->SetWindowText(_T("Dac Set"));
		GetDlgItem(IDC_BUTTON_AD_DEVICE)->SetWindowText(_T("Adc Set"));
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PATH_SET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_DA_DEVICE)->SetWindowText(_T("DA Device"));
		GetDlgItem(IDC_BUTTON_AD_DEVICE)->SetWindowText(_T("AD Device"));
	}
	//


	//teana hu 2012.07.02
	if(m_bDspCodecInfo && (m_strTitle == _T("Headset") || m_strTitle == _T("Handset") || m_strTitle == _T("Handsfree") || m_strTitle == _T("BT")))
	{
		GetDlgItem(IDC_BUTTON_CNG)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CNG)->ShowWindow(SW_HIDE);
	}
	//

    // Hide some buttons for every mode
    pSheet->m_CtrlStatus.RefreshControl( m_strTitle,(DWORD)-1,this );

    UpdateData( FALSE );
}

void CAudioModePage::RefreshAgcCtrl( BOOL bSwitch )
{
    if( bSwitch )
    {
        m_strAgcSwitch.LoadString( IDS_SWITCH_ON );
    }
    else
    {
        m_strAgcSwitch.LoadString( IDS_SWITCH_OFF );
    }
}

void CAudioModePage::OnButtonSidetoneGain() 
{
	CModifyGainDlg dlg;
    dlg.m_pArea = FindAreadByName( "digital_sidetone_gain" );
    dlg.m_strParaName = dlg.m_pArea->m_strName;
    dlg.m_strUnit = dlg.m_pArea->m_strUnit;

    if( IDOK == dlg.DoModal() )
    {
        m_strSidetoneGain = dlg.m_pArea->m_strOtherValue+dlg.m_pArea->m_strUnit;
        UpdateData( FALSE );
    }
}

BOOL CAudioModePage::FetchValue( AUDIO_MODE& value )
{
    if( !m_bActive )
    {
        // The page has not been active,all data keep unchanged.
        return FALSE;
    }

	if(!UpdateData())
	{
		return FALSE;
	}
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,m_strTitle.GetBuffer(m_strTitle.GetLength()),-1,(char *)value.mode_name,m_strTitle.GetLength(),NULL,NULL);
	value.mode_name[m_strTitle.GetLength()] = '\0';
#else
	strcpy( (char *)value.mode_name,m_strTitle );
#endif   
    
    value.audio_data.dl_device_external = (WORD)( ( m_dwDADevice & 0xffff0000 ) >> 16 );
    value.audio_data.dl_device_internal = (WORD)( m_dwDADevice & 0xffff );

    value.audio_data.ul_device_external = (WORD)( ( m_dwADDevice & 0xffff0000 ) >> 16 );
    value.audio_data.ul_device_internal = (WORD)( m_dwADDevice & 0xffff );

    value.audio_data.dl_pga_gain_h = (WORD)( ( m_dwDlPgaGain & 0xffff0000 ) >> 16 );
    value.audio_data.dl_pga_gain_l = (WORD)( m_dwDlPgaGain & 0xffff );
    value.audio_data.ul_pga_gain_h = (WORD)( ( m_dwUlPgaGain & 0xffff0000 ) >> 16 );
    value.audio_data.ul_pga_gain_l = (WORD)( m_dwUlPgaGain & 0xffff );
    
    value.audio_data.sidetone_switch &=0xfc00;
    value.audio_data.sidetone_switch |= ( ( m_nSideToneCp << 9 ) | ( m_nSideToneEq << 8 ) | m_nSideTone );;
    value.audio_data.aec_switch = (WORD)m_nAec;
    value.audio_data.volume_mode = (WORD)m_nVolume;
/* test code for steward 
    value.audio_data.volume_mode = m_nVolume | ( ( m_nSampleRateValue / 1000 ) << 1 );
*/
    value.audio_data.sample_rate = (WORD)(m_nSampleRateValue / 1000);
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
	
	//teana hu 2010.06.02
	if(m_nVolumeCount == 0)
	{
		int n = value.audio_data.reserved5[8];
		n &= 0x7fff;
		n |= (m_nMp3AgcSwitch << 15);
		n &= 0xbfff;
		n |= (m_nMidiAgcSwitch << 14);
		n &= 0xdfff;
		n |= (m_nMp4AgcSwitch << 13);
		n &= 0xefff;
		n |= (m_nVoiceAgcSwitch << 12);
		n &= 0xf7ff;
		n |= (m_nMp3LcfSwitch << 11);
		n &= 0xfbff;
		n |= (m_nMidiLcfSwitch << 10);
		n &= 0xfdff;
		n |= (m_nMp4LcfSwitch << 9);
		n &= 0xfeff;
		n |= (m_nVoiceLcfSwitch << 8);
		n &= 0xff7f;
		n |= (m_nDigitalGain << 7);
		n &= 0xfff0;
		n |= m_nEQSelect;
		value.audio_data.reserved5[8] = n;
		if(pSheet != NULL && pSheet->m_bLoadEQ)
		{
			if(m_dMp3AgcInputGainBak != m_dMp3AgcInputGain)
			{
				value.audio_data.reserved5[9] = ((double)pow(10, m_dMp3AgcInputGain/20.0)) * 1024;
			}
			if(m_dMidiAgcInputGainBak != m_dMidiAgcInputGain)
			{
				value.audio_data.reserved5[10] = ((double)pow(10, m_dMidiAgcInputGain/20.0)) * 1024;
			}
			if(m_dMp4AgcInputGainBak != m_dMp4AgcInputGain)
			{
				value.audio_data.reserved5[11] = ((double)pow(10, m_dMp4AgcInputGain/20.0)) * 1024;
			}
			if(m_dVoiceAgcInputGainBak != m_dVoiceAgcInputGain)
			{
				value.audio_data.reserved5[12] = ((double)pow(10, m_dVoiceAgcInputGain/20.0)) * 1024;
			}
		}
	}		
	//

	//teana hu 2011.04.22
	value.audio_data.dl_reserved[0] = m_wNXPBoostCtl;
	//

    int nSize = m_vAllAreas.size();
    for( int i=0;i<nSize;i++ )
    {
        m_vAllAreas[i]->FetchValue( value );
    }
    
    if( pSheet->IsExtend() )
    {
        if( pSheet->IsRocky() )
        {
			//teana hu 2010.08.16
            //memcpy( value.audio_data.arm_volume,m_ExtendData + EXTEND_DATA_SIZE * 2, ARM_VOLUME_SIZE * 2 );
            //memcpy( value.audio_data.dsp_volume,m_ExtendData + ( EXTEND_DATA_SIZE + ARM_VOLUME_SIZE ) * 2, DSP_VOLUME_SIZE * 2 );
        }

    }

	//teana hu 2012.07.02
	value.audio_data.dl_dp_gain = m_dwCNGGain;
	value.audio_data.dl_dp_attenu = m_dwCNGSW;
	//

	if( pSheet->m_bLoadEQ && eq_platform_all == pSheet->m_EQData.eq_platform )
	{		
		value.audio_data.reserved5[8] = ( value.audio_data.reserved5[8] & 0xFFDF ) | ( m_bMixerSw << 5 );

		value.audio_data.reserved5[15] = (value.audio_data.reserved5[15] & 0x7fff) | (m_nMp3AgcZCSwitch << 15);
		value.audio_data.reserved5[15] = (value.audio_data.reserved5[15] & 0xbfff) | (m_nMidiAgcZCSwitch << 14);
		value.audio_data.reserved5[15] = (value.audio_data.reserved5[15] & 0xdfff) | (m_nMp4AgcZCSwitch << 13);
		value.audio_data.reserved5[15] = (value.audio_data.reserved5[15] & 0xefff) | (m_nVoiceAgcZCSwitch << 12);

		int nIndex = FindEqMode();
		if( -1 != nIndex )
		{
			if( m_dMp3AgcStartGain != m_dMp3AgcStartGainBak )
			{
				pSheet->m_EQData.lpEQMode[nIndex].extend_array[0] = 1024 * pow( 10,m_dMp3AgcStartGain/20.0 );
			}
			if( m_dMidiAgcStartGain != m_dMidiAgcStartGainBak )
			{
				pSheet->m_EQData.lpEQMode[nIndex].extend_array[1] = 1024 * pow( 10,m_dMidiAgcStartGain/20.0 );
			}
			if( m_dMp4AgcStartGain != m_dMp4AgcStartGainBak )
			{
				pSheet->m_EQData.lpEQMode[nIndex].extend_array[2] = 1024 * pow( 10,m_dMp4AgcStartGain/20.0 );
			}
			if( m_dVoiceAgcStartGain != m_dVoiceAgcStartGainBak )
			{
				pSheet->m_EQData.lpEQMode[nIndex].extend_array[3] = 1024 * pow( 10,m_dVoiceAgcStartGain/20.0 );
			}
		}		

		if(m_nCBChannelSelLeft < 5)
		{
			value.audio_data.reserved5[16] = (value.audio_data.reserved5[16] & 0xff8f) | (m_nCBChannelSelLeft << 4);
		}
		else
		{
			value.audio_data.reserved5[16] = (value.audio_data.reserved5[16] & 0xff8f) | (m_nChannelSelLeft << 4);
		}
		
		if(m_nCBChannelSelRight < 5)
		{
			value.audio_data.reserved5[16] = (value.audio_data.reserved5[16] & 0xfff8) | m_nCBChannelSelRight;
		}
		else
		{
			value.audio_data.reserved5[16] = (value.audio_data.reserved5[16] & 0xfff8) | m_nChannelSelRight;
		}
		
		value.audio_data.reserved5[16] = (value.audio_data.reserved5[16] & 0xff7f) | (m_nPolarLeft << 7);
		value.audio_data.reserved5[16] = (value.audio_data.reserved5[16] & 0xfff7) | (m_nPolarRight << 3);
		
		if( -1 != nIndex )
		{
			pSheet->m_EQData.lpEQMode[nIndex].extend_array[7] = m_nAgcRelease;
			pSheet->m_EQData.lpEQMode[nIndex].extend_array[6] = m_nAgcAttack;
			pSheet->m_EQData.lpEQMode[nIndex].extend_array[5] = m_nAgcHoldHc;
			pSheet->m_EQData.lpEQMode[nIndex].extend_array[4] = m_nAgcSdelay;
		}

		value.audio_data.reserved5[8] = (value.audio_data.reserved5[8] & 0xFFBF) | ((!m_nLcfPosition) << 6);
	}

	return TRUE;
}

void CAudioModePage::OnButtonAgc() 
{
    CAgcDlg dlg;
    dlg.m_pArea = FindAreadByName( "AGC Switch" );
    
    if( IDOK == dlg.DoModal() )
    {
        RefreshAgcCtrl( ((SpecialRectOfAgc*)dlg.m_pArea)->m_iValue );
        UpdateData( FALSE );
    }
}

void CAudioModePage::SetAudioData( AUDIO_STRUCT& audio_data,void* pPara )
{
	//teana hu 2011.02.14
	if(audio_data.reserved5[4] & 0x1 ==1)
	{
		m_bAudio4 = TRUE;
	}
	else
	{
		m_bAudio4 = FALSE;
	}
	//
	
    m_dwADDevice = audio_data.ul_device_external;
    m_dwADDevice = ( m_dwADDevice << 16 );
    m_dwADDevice |= audio_data.ul_device_internal;

    m_dwDADevice = audio_data.dl_device_external;
    m_dwDADevice = ( m_dwDADevice << 16 );
    m_dwDADevice |= audio_data.dl_device_internal;
    
    m_dwDlPgaGain = audio_data.dl_pga_gain_h;
    m_dwDlPgaGain = ( m_dwDlPgaGain << 16 );
    m_dwDlPgaGain |= audio_data.dl_pga_gain_l;

    m_dwUlPgaGain = audio_data.ul_pga_gain_h;
    m_dwUlPgaGain = ( m_dwUlPgaGain << 16 );
    m_dwUlPgaGain |= audio_data.ul_pga_gain_l;

    m_nSampleRate = CalcSampleRateRev( audio_data.sample_rate );
    m_nSampleRateValue = audio_data.sample_rate * 1000;
    m_nAec = audio_data.aec_switch;
    m_nSideTone = audio_data.sidetone_switch & 0xFF;
    m_nVolume = audio_data.volume_mode;
/* test code for steward
    m_nVolume = audio_data.volume_mode & 0xF;
*/
    m_nSideToneEq = ( audio_data.sidetone_switch >> 8 ) & 0x1;
    m_nSideToneCp = ( audio_data.sidetone_switch >> 9 ) & 0x1;
    
    m_bFir = audio_data.dl_eq_switch & 0x10;
	m_bNewMoreBandDL = (audio_data.dl_eq_switch & 0x200) >> 9;
	m_bNewMoreBandUL = (audio_data.ul_fir_hpf_enable & 0x2) >> 1;

    if( !m_strTitle.Compare( _T("MP4HFTR") ) && ( audio_data.coeff_norm_shift & 0x1 ) )
    {
        m_bXagcEnable = TRUE;
    }

    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)pPara;
    if( pSheet->IsExtend() )
    {
        if( pSheet->IsRocky() )
        {
            memcpy( m_ExtendData + EXTEND_DATA_SIZE * 2,audio_data.arm_volume,ARM_VOLUME_SIZE * 2);
            memcpy( m_ExtendData + ( EXTEND_DATA_SIZE + ARM_VOLUME_SIZE ) * 2,audio_data.dsp_volume,DSP_VOLUME_SIZE * 2);
        }
    }

	m_bAudio3Switch = ( audio_data.audio_three_switch & 0x1 );
	m_bVolumeSwitch = ( audio_data.audio_three_switch & 0x100 );
	BOOL bVolumeConv = ( audio_data.audio_three_switch & 0x1000 );

	if( bVolumeConv )
	{
		//::VolumeConv( &audio_data ); 
	}	

    InitAreas( pSheet );
    
//    SetSampleRate( m_nSampleRateValue );

    for( UINT i=0;i<m_vAllAreas.size();i++ )
    {
        m_vAllAreas[i]->SetValue( audio_data );
    }

	//teana hu 2010.06.02
	m_nMp3AgcSwitch = (audio_data.reserved5[8] & 0x8000) >> 15;
	m_nMidiAgcSwitch = (audio_data.reserved5[8] & 0x4000) >> 14;
	m_nMp4AgcSwitch = (audio_data.reserved5[8] & 0x2000) >> 13;
	m_nVoiceAgcSwitch = (audio_data.reserved5[8] & 0x1000) >> 12;
	m_nMp3LcfSwitch = (audio_data.reserved5[8] & 0x0800) >> 11;
	m_nMidiLcfSwitch = (audio_data.reserved5[8] & 0x0400) >> 10;
	m_nMp4LcfSwitch = (audio_data.reserved5[8] & 0x0200) >> 9;
	m_nVoiceLcfSwitch = (audio_data.reserved5[8] & 0x0100) >> 8;
	
	m_nDigitalGain = (audio_data.reserved5[8] & 0x0080) >> 7;
	m_nEQSelect = (audio_data.reserved5[8] & 0x000f);

//	if(pSheet->m_bLoadEQ)
	{
		int nValue = audio_data.reserved5[9];
		if(nValue == 0)
		{
			nValue = 1;
		}
		m_dMp3AgcInputGain = (double)(20 * (double)log10((double)nValue/(double)1024));
		m_dMp3AgcInputGain = (double)((short)(m_dMp3AgcInputGain * 10) / 10.0);
		m_dMp3AgcInputGain = m_dMp3AgcInputGain < -65535 ? -65535 : m_dMp3AgcInputGain;

		nValue = audio_data.reserved5[10];
		if(nValue == 0)
		{
			nValue = 1;
		}
		
		m_dMidiAgcInputGain = (double)(20 * (double)log10((double)nValue/(double)1024));
		m_dMidiAgcInputGain = (double)((short)(m_dMidiAgcInputGain * 10) / 10.0);
		m_dMidiAgcInputGain = m_dMidiAgcInputGain < -65535 ? -65535 : m_dMidiAgcInputGain;

		nValue = audio_data.reserved5[11];
		if(nValue == 0)
		{
			nValue = 1;
		}
		
		m_dMp4AgcInputGain = (double)(20 * (double)log10((double)nValue/(double)1024));
		m_dMp4AgcInputGain = (double)((short)(m_dMp4AgcInputGain * 10) / 10.0);
		m_dMp4AgcInputGain = m_dMp4AgcInputGain < -65535 ? -65535 : m_dMp4AgcInputGain;

		nValue = audio_data.reserved5[12];
		if(nValue == 0)
		{
			nValue = 1;
		}

		m_dVoiceAgcInputGain = (double)(20 * (double)log10((double)nValue/(double)1024));
		m_dVoiceAgcInputGain = (double)((short)(m_dVoiceAgcInputGain * 10) / 10.0);
		m_dVoiceAgcInputGain = m_dVoiceAgcInputGain < -65535 ? -65535 : m_dVoiceAgcInputGain;
		
		m_dMp3AgcInputGainBak = m_dMp3AgcInputGain;
		m_dMidiAgcInputGainBak = m_dMidiAgcInputGain;
		m_dMp4AgcInputGainBak = m_dMp4AgcInputGain;
		m_dVoiceAgcInputGainBak = m_dVoiceAgcInputGain;
	}

	//

	//teana hu 2012.07.02
	m_dwCNGGain = audio_data.dl_dp_gain;
	m_dwCNGSW = audio_data.dl_dp_attenu;
	//

	//teana hu 2011.04.22
	m_wNXPBoostCtl = audio_data.dl_reserved[0];
	//

	if( pSheet->m_bLoadEQ && eq_platform_all == pSheet->m_EQData.eq_platform )
	{
		m_bMixerSw = ( audio_data.reserved5[8] & 0x0020 ) >> 5;

		m_nMp3AgcZCSwitch = ( audio_data.reserved5[15] & 0x8000 ) >> 15;
		m_nMidiAgcZCSwitch = ( audio_data.reserved5[15] & 0x4000 ) >> 14;
		m_nMp4AgcZCSwitch = ( audio_data.reserved5[15] & 0x2000 ) >> 13;
		m_nVoiceAgcZCSwitch = ( audio_data.reserved5[15] & 0x1000 ) >> 12;

		int nIndex = FindEqMode();
		if( -1 != nIndex )
		{
			int nValue = pSheet->m_EQData.lpEQMode[nIndex].extend_array[0];
			if( nValue > 0 )
			{
				nValue = 1;
			}
			m_dMp3AgcStartGain = (double)(20 * (double)log10((double)nValue/(double)1024));
			m_dMp3AgcStartGain = int( m_dMp3AgcStartGain * 10 ) / 10.0;
			m_dMp3AgcStartGain = m_dMp3AgcStartGain < -65535 ? -65535 : m_dMp3AgcStartGain;

			nValue = pSheet->m_EQData.lpEQMode[nIndex].extend_array[1];
			if( nValue > 0 )
			{
				nValue = 1;
			}

			m_dMidiAgcStartGain = (double)(20 * (double)log10((double)nValue/(double)1024));
			m_dMidiAgcStartGain = int( m_dMidiAgcStartGain * 10 ) / 10.0;
			m_dMidiAgcStartGain = m_dMidiAgcStartGain < -65535 ? -65535 : m_dMidiAgcStartGain;

			nValue = pSheet->m_EQData.lpEQMode[nIndex].extend_array[2];
			if( nValue > 0 )
			{
				nValue = 1;
			}

			m_dMp4AgcStartGain = (double)(20 * (double)log10((double)nValue/(double)1024));
			m_dMp4AgcStartGain = int( m_dMp4AgcStartGain * 10 ) / 10.0;
			m_dMp4AgcStartGain = m_dMp4AgcStartGain < -65535 ? -65535 : m_dMp4AgcStartGain;


			nValue = pSheet->m_EQData.lpEQMode[nIndex].extend_array[3];
			if( nValue > 0 )
			{
				nValue = 1;
			}
			m_dVoiceAgcStartGain = (double)(20 * (double)log10((double)nValue/(double)1024));
			m_dVoiceAgcStartGain = int( m_dVoiceAgcStartGain * 10 ) / 10.0;
			m_dVoiceAgcStartGain = m_dVoiceAgcStartGain < -65535 ? -65535 : m_dVoiceAgcStartGain;

			m_dMp3AgcStartGainBak = m_dMp3AgcStartGain;
			m_dMidiAgcStartGainBak = m_dMidiAgcStartGain;
			m_dMp4AgcStartGainBak = m_dMp4AgcStartGain;
			m_dVoiceAgcStartGainBak = m_dVoiceAgcStartGain;
		}	
		
		m_nChannelSelLeft = (audio_data.reserved5[16] & 0x70) >> 4;
		if(m_nChannelSelLeft >= 5)
		{
			m_nCBChannelSelLeft = 5;
		}
		else
		{
			m_nCBChannelSelLeft = m_nChannelSelLeft;
		}
		m_nChannelSelRight = audio_data.reserved5[16] & 0x7;
		if(m_nChannelSelRight >= 5)
		{
			m_nCBChannelSelRight = 5;
		}
		else
		{
			m_nCBChannelSelRight = m_nChannelSelRight;
		}
		m_nPolarLeft = (audio_data.reserved5[16] & 0x80) >> 7;
		m_nPolarRight = (audio_data.reserved5[16] & 0x8) >> 3;
		if( -1 != nIndex )
		{
			m_nAgcRelease = pSheet->m_EQData.lpEQMode[nIndex].extend_array[7];
			m_nAgcAttack = pSheet->m_EQData.lpEQMode[nIndex].extend_array[6];
			m_nAgcHoldHc = pSheet->m_EQData.lpEQMode[nIndex].extend_array[5];
			m_nAgcSdelay = pSheet->m_EQData.lpEQMode[nIndex].extend_array[4];
		}
		m_nLcfPosition = !( (audio_data.reserved5[8] & 0x40) >> 6 );	
	}
}

void CAudioModePage::InitToolbar()
{
    CRect reResultPos;
    GetDlgItem( IDC_STATIC_TOOLBAR_PLACEHOLDER )->GetWindowRect( &reResultPos );
    ScreenToClient( &reResultPos );
    m_toolBarAudio.MoveWindow( &reResultPos );
//    m_toolBarAudio.GetToolBarCtrl().EnableButton( ID_BUTTON_AUDIO_TYPE, TRUE );
//    m_toolBarAudio.GetToolBarCtrl().EnableButton( ID_BUTTON_AUDIO_TYPE, TRUE );
    
//    m_toolBarAudio.GetToolBarCtrl().SetState(ID_BUTTON_AUDIO_MUTE_BMP,TBSTATE_HIDDEN );
//    m_toolBarAudio.GetToolBarCtrl().SetState( ID_BUTTON_CHIPD,TBSTATE_HIDDEN );
    
    m_toolBarAudio.ShowWindow( SW_SHOW );
}

int CAudioModePage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    if (!m_toolBarAudio.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_toolBarAudio.LoadToolBar(IDR_TOOLBAR_AUDIO))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }
    
	CToolBarCtrl &ctl = m_toolBarAudio.GetToolBarCtrl();	
	DWORD dwStyle = ctl.GetStyle();
	dwStyle |= CBRS_TOOLTIPS;
	ctl.SetStyle(dwStyle);

	return 0;
}

void CAudioModePage::OnButtonAudioWriteCurrentModeToPhone() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();

	if( !pSheet->FetchValue( pSheet->GetActiveIndex() - 1 ) )
	{
		pSheet->Log( _T( "Get audio data from gui failed" ),TRUE );
		return;
	}

	if( pSheet->m_bLoadEQ && m_nEQSelect > 0 )
	{
		pSheet->WriteEQParaToPhone(m_nEQSelect - 1);
	}
    pSheet->WriteParaToPhone( pSheet->GetActiveIndex() - 1 ); 
    Sleep( 500 );
    SpecialRect* pArea = FindAreadByName( "Volume control" );
    pSheet->SetVolume( ((SpecialRectOfVolumeCtrl*)pArea)->m_iValue );
}

void CAudioModePage::OnButtonAudioWriteToPhone() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    pSheet->WriteAllParaToPhone();    
}

void CAudioModePage::OnButtonPop() 
{
	CPopDialog dlg;
    dlg.m_pArea = (SpecialRectOfPop*)FindAreadByName( "Pop" );

    dlg.DoModal();    
}

void CAudioModePage::OnButtonNg() 
{
    CNGDlg dlg;
    dlg.m_pArea = (SpecialRectOfNg*)FindAreadByName( "Ng" );

    dlg.DoModal();
}

void CAudioModePage::OnButtonAec() 
{
    SpecialRect* pArea = FindAreadByName( "AEC" );
    if( m_bXagcEnable )
    {
        CXagcDlg dlg;
        dlg.m_pArea = (SpecialRectOfAec*)pArea;
        dlg.DoModal();
    }
    else
    {
        CAecDialog dlg;
        dlg.m_pArea = (SpecialRectOfAec*)pArea;
        dlg.DoModal();
    }
}

void CAudioModePage::OnSelchangeComboSpeaker() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    pSheet->SetSpeaker( ( (CComboBox*)GetDlgItem( IDC_COMBO_SPEAKER ) ) ->GetCurSel() );
}

void CAudioModePage::OnSelchangeComboMute() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    pSheet->SetMute( ( (CComboBox*)GetDlgItem( IDC_COMBO_MUTE ) ) ->GetCurSel() );
}

void CAudioModePage::OnButtonExtendData() 
{
	CExtDataDlg dlg;

    dlg.m_lpBuffer = m_ExtendData + EXTEND_DATA_SIZE*2;

//    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    
    dlg.m_nDataSize = DSP_VOLUME_SIZE + ARM_VOLUME_SIZE;
    
    if( IDOK == dlg.DoModal() )
    {
        int nSize;
        memcpy( m_ExtendData + EXTEND_DATA_SIZE*2,dlg.m_wndBuffer.GetData( nSize ),dlg.m_nDataSize*2 );
    }
}

int g_nYOffSize = 0;

void CAudioModePage::OnButtonDlFirEqCoeff() 
{
    g_nYOffSize = 0;
    gnFirType = SpecialRect::dl_fir_eq_coeff;
    //CCoefficientDlg dlg;

	//teana hu 2012.07.11
/*	if(m_bNewMoreBandDL)
	{
		CFirAutoDlgEx dlg;
		dlg.m_bAuto = FALSE;
		dlg.m_bVisibleFirButton = TRUE;
		dlg.m_pArea = (SpecialRectOfCoefficientEx*)FindAreadByName( "DL_FIR_EQ_COEFF_EX" );
		dlg.m_nSampleRate = m_nSampleRateValue;
		dlg.DoModal();
	}
	else*/
	{
		CFirAutoDlg dlg;
		dlg.m_bAuto = FALSE;
		dlg.m_bVisibleFirButton = TRUE;
		dlg.m_pArea = (SpecialRectOfCoefficient*)FindAreadByName( "DL_FIR_EQ_COEFF" );
		dlg.m_nSampleRate = m_nSampleRateValue;
		dlg.DoModal();
	}
}

void CAudioModePage::OnButtonStFirEqCoeff() 
{
    g_nYOffSize = 0;
    gnFirType = SpecialRect::st_fir_eq_coeff;
    //CCoefficientDlg dlg;
	CFirAutoDlg dlg;
 	dlg.m_bAuto = FALSE;
	dlg.m_bVisibleFirButton = FALSE;
    dlg.m_pArea = (SpecialRectOfCoefficient*)FindAreadByName( "ST_FIR_EQ_COEFF" );
    dlg.m_nSampleRate = m_nSampleRateValue;
    dlg.DoModal();
}

void CAudioModePage::OnButtonUlFirEqCoeff() 
{
//	g_nYOffSize = -6;
    g_nYOffSize = -20;
    gnFirType = SpecialRect::ul_fir_eq_coeff;

//    CCoefficientDlg dlg;
	//teana hu 2012.07.11
/*	if(m_bNewMoreBandUL)
	{
		CFirAutoDlgEx dlg;
		dlg.m_bAuto = FALSE;
		dlg.m_bVisibleFirButton = TRUE;
		dlg.m_pArea = (SpecialRectOfCoefficientEx*)FindAreadByName( "UL_FIR_EQ_COEFF_EX" );
		dlg.m_nSampleRate = m_nSampleRateValue;
		dlg.DoModal();
	}
	else*/
	{
		CFirAutoDlg dlg;
		dlg.m_bAuto = FALSE;
		dlg.m_bVisibleFirButton = TRUE;
		dlg.m_pArea = (SpecialRectOfCoefficient*)FindAreadByName( "UL_FIR_EQ_COEFF" );
		dlg.m_nSampleRate = m_nSampleRateValue;
		dlg.DoModal();
	}
}

SpecialRect* CAudioModePage::FindAreadByName( CString strName )
{
    for( UINT i=0;i<m_vAllAreas.size();i++ )
    {
        if( !strName.Compare( m_vAllAreas[i]->m_strName ) )
        {
            return m_vAllAreas[i];
        }
    }

    // Not found
    return NULL;
}

void CAudioModePage::OnButtonAudioWriteCurrentModeToFlash() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();

	if( !pSheet->FetchValue( pSheet->GetActiveIndex() - 1 ) )
	{
		pSheet->Log( _T( "Get audio data from gui failed" ),TRUE );
		return;
	}

	if( pSheet->m_bLoadEQ && m_nEQSelect > 0 )
	{
		pSheet->WriteEQParaToFlash(m_nEQSelect - 1);
	}
    pSheet->WriteParaToFlash( pSheet->GetActiveIndex() - 1 ); 

}

void CAudioModePage::OnButtonSetDefaultFir() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    pSheet->WriteDefaultFirToPhone( pSheet->GetActiveIndex() - 1 ); 

	if(m_bNewMoreBandDL)
	{
		//teana hu 2011.02.23
		SpecialRectOfCoefficientEx *pArea = (SpecialRectOfCoefficientEx*)FindAreadByName( "DL_FIR_EQ_COEFF_EX" );
		if(pArea != NULL)
		{
			((SpecialRectOfCoefficientEx *)pArea)->m_bModified = FALSE;
		}
		//
	}
	else
	{
		//teana hu 2011.02.23
		SpecialRectOfCoefficient *pArea = (SpecialRectOfCoefficient*)FindAreadByName( "DL_FIR_EQ_COEFF" );
		if(pArea != NULL)
		{
			((SpecialRectOfCoefficient *)pArea)->m_bModified = FALSE;
		}
		//
	}
}

void CAudioModePage::SetDefaultFir( AUDIO_MODE& _value )
{
	// Set dl fir to default value

    // dl fir
    for( int i=0;i<16;i++ )
    {
        _value.audio_data.dl_fir_eq_coeff[i] = 0;
    }
    _value.audio_data.dl_fir_eq_coeff[16] = 4096;
    
    // st fir
    for( i=0;i<33;i++ )
    {
        _value.audio_data.st_fir_eq_coeff[i] = 0;
    }
    _value.audio_data.st_fir_eq_coeff[16] = 4096;
    
	//teana hu 2012.07.12
	SpecialRect* pArea = NULL;
	if(m_bNewMoreBandDL)
	{
		pArea = FindAreadByName( "DL_FIR_EQ_COEFF_EX" );
		if( pArea != NULL )
		{
			pArea->SetValue( _value.audio_data );
		}
	}
	else
	{
		pArea = FindAreadByName( "DL_FIR_EQ_COEFF" );
		if( pArea != NULL )
		{
			pArea->SetValue( _value.audio_data );
		}
	}
	//

    pArea = FindAreadByName( "ST_FIR_EQ_COEFF" );
    if( pArea != NULL )
    {
        pArea->SetValue( _value.audio_data );
    }
}

void CAudioModePage::OnButtonMoreParameter2() 
{
    CExtend2Dlg dlg;
    dlg.m_pArea = (SpecialRectOfDlSettings*)FindAreadByName( "Dl_Setting" );

    dlg.DoModal();
}

void CAudioModePage::OnButtonMoreParameter3() 
{
    CExtend3Dlg dlg;
    dlg.m_pArea = (SpecialRectOfUlSettings*)FindAreadByName( "Ul_Setting" );

    dlg.DoModal();
}

void CAudioModePage::OnButtonSetDefaultUlFir() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    pSheet->WriteDefaultUlFirToPhone( pSheet->GetActiveIndex() - 1 );
	
	if(m_bNewMoreBandUL)
	{
		//teana hu 2011.02.23
		SpecialRectOfCoefficientEx *pArea = (SpecialRectOfCoefficientEx*)FindAreadByName( "UL_FIR_EQ_COEFF_EX" );
		if(pArea != NULL)
		{
			((SpecialRectOfCoefficientEx *)pArea)->m_bModified = FALSE;
		}
		//
	}
	else
	{
		//teana hu 2011.02.23
		SpecialRectOfCoefficient *pArea = (SpecialRectOfCoefficient*)FindAreadByName( "UL_FIR_EQ_COEFF" );
		if(pArea != NULL)
		{
			((SpecialRectOfCoefficient *)pArea)->m_bModified = FALSE;
		}
		//
	}

}

void CAudioModePage::SetDefaultUlFir( AUDIO_MODE& _value )
{
	// ul fir
    for( int i=0;i<33;i++ )
    {
        _value.audio_data.ul_fir_eq_coeff[i] = 0;
    }
    _value.audio_data.ul_fir_eq_coeff[16] = 4096;
	
	//teana hu 2012.07.12
	if(m_bNewMoreBandUL)
	{
		SpecialRect* pArea = FindAreadByName( "UL_FIR_EQ_COEFF_EX" );
		if( pArea != NULL )
		{
			pArea->SetValue( _value.audio_data );
		}
	}
	else
	{
		SpecialRect* pArea = FindAreadByName( "UL_FIR_EQ_COEFF" );
		if( pArea != NULL )
		{
			pArea->SetValue( _value.audio_data );
		}
	}
	//
	
}

void CAudioModePage::OnButtonSidetoneNg() 
{
	CStNgDlg dlg;
	dlg.m_pArea = (SpecialRectOfStng*)FindAreadByName( "st_ng" );
	dlg.DoModal();
}

void CAudioModePage::OnButtonVolume() 
{
	CVolumeDlg dlg;
	dlg.m_pArea = (SpecialRectOfVolume*)FindAreadByName( "volume_settings" );
	//teana hu 2012.06.12
	dlg.m_bSmartPhone = m_bSmartPhone;
	dlg.SetDspCodecFlag(m_bDspCodecInfo);
	//
	dlg.DoModal();
}

BOOL CAudioModePage::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	TOOLTIPTEXT *pTip = (TOOLTIPTEXT *)lParam;  

	CString strTip;  
	
	switch(pTip->hdr.code)  
	{  	
	case TTN_NEEDTEXT: 
		{
			switch(pTip->hdr.idFrom)  
			{  
			case ID_BUTTON_AUDIO_WRITE_CURRENT_MODE_TO_PHONE:   
				strTip.LoadString(IDS_WRITE_MS);
				break;  
			case ID_BUTTON_AUDIO_WRITE_CURRENT_MODE_TO_FLASH:  
				strTip.LoadString(IDS_WRITE_FLASH);  
				break;
			case ID_BUTTON_AUDIO_LOAD_CURRENT_MODE_FROM_PHONE:
				strTip.LoadString(IDS_LOAD_MS);
				break;
			case ID_BUTTON_AUDIO_LOAD_CURRENT_MODE_FROM_FLASH:
				strTip.LoadString(IDS_LOAD_FLASH);
				break;
			default:
				break;
			}  

			_tcscpy(pTip->szText, strTip.GetBuffer(strTip.GetLength()));  
			break;  
		}
	default:
		break;
	}
	
	strTip.ReleaseBuffer( strTip.GetLength() ); 
	
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void CAudioModePage::OnBtnEqSet() 
{
	// TODO: Add your control notification handler code here
	CDlgEQSet dlg;
	dlg.m_pSheet = (CAudioTesterSheet *)AfxGetMainWnd();
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_EQ_SELECT))->GetCurSel();
	if(nSel > 0)
	{
		dlg.m_nCurrentPage = nSel - 1;
	}
	
	dlg.DoModal();
	
}

void CAudioModePage::OnButtonAudioLoadCurrentModeFromFlash() 
{
	// TODO: Add your command handler code here

	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
	pSheet->m_bLoadEQ = pSheet->LoadEQModes();
    pSheet->LoadParaFromFlash( pSheet->GetActiveIndex() - 1 ); 
}

void CAudioModePage::OnButtonAudioLoadCurrentModeFromPhone() 
{
	// TODO: Add your command handler code here
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
	pSheet->m_bLoadEQ = pSheet->LoadEQModes();
    pSheet->LoadParaFromPhone( pSheet->GetActiveIndex() - 1 ); 
}
//teana hu 2010.08.13
void CAudioModePage::OnBtnVolumeSet() 
{
	// TODO: Add your control notification handler code here
	CDlgArmVolume dlg;
	dlg.m_pArea = (SpecialRectOfArmVolume*)FindAreadByName( "arm_volume_settings" );
	//teana hu 2011.02.21
	dlg.m_pArea->m_nCurType = m_nVolumeType;
	//
	dlg.DoModal();
	
}

//teana hu 2010.10.25
void CAudioModePage::EnableSoundEffectsWindow(BOOL bEnable)
{
	//GetDlgItem(IDC_COMBO_MP4_AGC_SWITCH)->EnableWindow(bEnable);
	//GetDlgItem(IDC_COMBO_VOICE_AGC_SWITCH)->EnableWindow(bEnable);
	//GetDlgItem(IDC_COMBO_MP4_LCF_SWITCH)->EnableWindow(bEnable);
	//GetDlgItem(IDC_COMBO_VOICE_LCF_SWITCH)->EnableWindow(bEnable);
	//GetDlgItem(IDC_EDT_MP4_AGC_INPUT_GAIN)->EnableWindow(bEnable);
	//GetDlgItem(IDC_EDT_VOICE_AGC_INPUT_GAIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_VDG_POSITION)->EnableWindow(bEnable && m_nSoundEqVer == sound_eq_ver_3);
	GetDlgItem(IDC_COMBO_LCF_POSITION)->EnableWindow(bEnable && m_nSoundEqVer == sound_eq_ver_4);
	GetDlgItem(IDC_COMBO_EQ_SELECT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_EQ_SET)->EnableWindow(bEnable && m_nEQSelect > 0 );
//	GetDlgItem(IDC_BTN_VOLUME_SET)->EnableWindow(bEnable);
}

void CAudioModePage::ShowSoundEffectsWindow(int nCmdShow)
{
	GetDlgItem(IDC_STATIC_SOUND_EFFECTS)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_DIVISION)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_MP3)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_MIDI)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_MP4)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_VOICE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_AGC_SWTC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_LCF_SWTC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_AGC_INPUT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_DB_MP3)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_DB_MIDI)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_DB_MP4)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_DB_VOICE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_VDG_POSITION)->ShowWindow(nCmdShow);
	
	GetDlgItem(IDC_STATIC_EQ_SELECT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_VOLUME_TYPE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_MP3_AGC_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_MIDI_AGC_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_MP4_AGC_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_VOICE_AGC_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_MP3_LCF_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_MIDI_LCF_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_MP4_LCF_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_VOICE_LCF_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDT_MP3_AGC_INPUT_GAIN)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDT_MIDI_AGC_INPUT_GAIN)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDT_MP4_AGC_INPUT_GAIN)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDT_VOICE_AGC_INPUT_GAIN)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_VDG_POSITION)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_EQ_SELECT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_BTN_EQ_SET)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_VOLUME_TYPE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_BTN_VOLUME_SET)->ShowWindow(nCmdShow);
	//teana hu 2011.06.16
	GetDlgItem(IDC_COMBO_MP3_AGC_ZC_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_MIDI_AGC_ZC_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_MP4_AGC_ZC_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_VOICE_AGC_ZC_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDT_MP3_AGC_START_GAIN)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDT_MIDI_AGC_START_GAIN)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDT_MP4_AGC_START_GAIN)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDT_VOICE_AGC_START_GAIN)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_BTN_SHOW_GRAPH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_CHANNEL_SEL_LEFT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_CHANNEL_SEL_RIGHT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_POLAR_LEFT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_POLAR_RIGHT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDIT_AGC_ATTACK)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDIT_AGC_RELEASE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDIT_AGC_HOLD_HC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDIT_AGC_SDELAY)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_AGC_ZC_SWITCH)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_AGC_START_GAIN)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_CHANNEL_SEL_LEFT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_CHANNEL_SEL_RIGHT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_POLAR_LEFT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_POLAR_RIGHT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_AGC_ATTACK)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_AGC_RELEASE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_AGC_HOLD_HC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_AGC_SDELAY)->ShowWindow(nCmdShow);
	//

	GetDlgItem( IDC_COMBO_MIXER_SW )->ShowWindow( nCmdShow );
	GetDlgItem(IDC_STATIC_MIXER_SW)->ShowWindow(nCmdShow);

	GetDlgItem(IDC_STATIC_DB_MP5)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_DB_MIDI2)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_DB_MP6)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_DB_VOICE2)->ShowWindow(nCmdShow);

	GetDlgItem(IDC_STATIC_AGC_ATTACK_MS)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_AGC_RELEASE_MS)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_AGC_HOLD_HC_MS)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_AGC_SDELAY_MS)->ShowWindow(nCmdShow);

	GetDlgItem(IDC_STATIC_LCF_POSITION)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_LCF_POSITION)->ShowWindow(nCmdShow);

}

void CAudioModePage::OnSelchangeComboSidetoneEq() 
{
    m_nSideToneEq = ((CComboBox*)GetDlgItem( IDC_COMBO_SIDETONE_EQ ))->GetCurSel();    	
}

void CAudioModePage::OnSelchangeComboSidetoneCp() 
{
    m_nSideToneCp = ((CComboBox*)GetDlgItem( IDC_COMBO_SIDETONE_CP ))->GetCurSel();    	
}


BOOL CAudioModePage::FetchVolumeValue( VOLUME_MODE& value )
{
	if( !m_bActive )
    {
        // The page has not been active,all data keep unchanged.
        return FALSE;
    }

	if(!UpdateData())
	{
		return FALSE;
	}
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,m_strTitle.GetBuffer(m_strTitle.GetLength()),-1,(char *)value.mode_name,m_strTitle.GetLength(),NULL,NULL);
	value.mode_name[m_strTitle.GetLength()] = '\0';
#else
	strcpy( (char *)value.mode_name,m_strTitle );
#endif
	
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();

	value.volume_data.app_config_info_set.aud_proc_exp_control[0] = (value.volume_data.app_config_info_set.aud_proc_exp_control[0] & 0xfff0) | m_nEQSelect;
	value.volume_data.app_config_info_set.aud_proc_exp_control[0] = (value.volume_data.app_config_info_set.aud_proc_exp_control[0] & 0xFF7F) | (m_nDigitalGain << 7);
	
	if( pSheet->m_bLoadEQ && eq_platform_all != pSheet->m_EQData.eq_platform )
	{
		value.volume_data.app_config_info_set.aud_proc_exp_control[0] = ( value.volume_data.app_config_info_set.aud_proc_exp_control[0] & 0xFFDF ) | ( m_bMixerSw << 5 );

		value.volume_data.app_config_info_set.aud_proc_exp_control[0] = (value.volume_data.app_config_info_set.aud_proc_exp_control[0] & 0xFFBF) | ((!m_nLcfPosition) << 6);
		value.volume_data.app_config_info_set.aud_proc_exp_control[0] = (value.volume_data.app_config_info_set.aud_proc_exp_control[0] & 0xfeff) | (m_nMp3AgcZCSwitch << 8);
		value.volume_data.app_config_info_set.aud_proc_exp_control[0] = (value.volume_data.app_config_info_set.aud_proc_exp_control[0] & 0xFDFF) | (m_nMidiAgcZCSwitch << 9);
		value.volume_data.app_config_info_set.aud_proc_exp_control[0] = (value.volume_data.app_config_info_set.aud_proc_exp_control[0] & 0xFBFF) | (m_nMp4AgcZCSwitch << 10);
	}

	value.volume_data.app_config_info_set.aud_proc_exp_control[1] = (value.volume_data.app_config_info_set.aud_proc_exp_control[1] & 0xFFFE) | m_nMp3LcfSwitch;
	value.volume_data.app_config_info_set.aud_proc_exp_control[1] = (value.volume_data.app_config_info_set.aud_proc_exp_control[1] & 0xFFFD) | (m_nMidiLcfSwitch << 1);
	value.volume_data.app_config_info_set.aud_proc_exp_control[1] = (value.volume_data.app_config_info_set.aud_proc_exp_control[1] & 0xFFFB) | (m_nMp4LcfSwitch << 2);
	value.volume_data.app_config_info_set.aud_proc_exp_control[1] = (value.volume_data.app_config_info_set.aud_proc_exp_control[1] & 0xFEFF) | (m_nMp3AgcSwitch << 8);
	value.volume_data.app_config_info_set.aud_proc_exp_control[1] = (value.volume_data.app_config_info_set.aud_proc_exp_control[1] & 0xFDFF) | (m_nMidiAgcSwitch << 9);
	value.volume_data.app_config_info_set.aud_proc_exp_control[1] = (value.volume_data.app_config_info_set.aud_proc_exp_control[1] & 0xFBFF) | (m_nMp4AgcSwitch << 10);

	if( pSheet->m_bLoadEQ && eq_platform_all != pSheet->m_EQData.eq_platform )
	{
		m_arrMp3AgcInputGain[m_nVolumeType] = m_dMp3AgcInputGain;
		m_arrMidiAgcInputGain[m_nVolumeType] = m_dMidiAgcInputGain;
		m_arrMp4AgcInputGain[m_nVolumeType] = m_dMp4AgcInputGain;
	}

	//teana hu 2011.06.15
	m_arrMp3AgcStartGain[m_nVolumeType] = m_dMp3AgcStartGain;
	m_arrMidiAgcStartGain[m_nVolumeType] = m_dMidiAgcStartGain;
	m_arrMp4AgcStartGain[m_nVolumeType] = m_dMp4AgcStartGain;
	//

	int nCount = value.volume_data.app_config_info_set.valid_app_set_count;

	for(int i = 0; i <nCount; i++)
	{	
		if(m_arrMp3AgcInputGainBak[i] != m_arrMp3AgcInputGain[i])
		{
			value.volume_data.app_config_info_set.app_config_info[i].agc_input_gain[0] = ((double)pow(10, m_arrMp3AgcInputGain[i]/20.0)) * 1024;
		}
		
		if(m_arrMidiAgcInputGainBak[i] != m_arrMidiAgcInputGain[i])
		{
			value.volume_data.app_config_info_set.app_config_info[i].agc_input_gain[1] = ((double)pow(10, m_arrMidiAgcInputGain[i]/20.0)) * 1024;
		}

		if(m_arrMp4AgcInputGainBak[i] != m_arrMp4AgcInputGain[i])
		{
			value.volume_data.app_config_info_set.app_config_info[i].agc_input_gain[2] = ((double)pow(10, m_arrMp4AgcInputGain[i]/20.0)) * 1024;
		}

		//teana hu 2011.06.15
		if(m_arrMp3AgcStartGainBak[i] != m_arrMp3AgcStartGain[i])
		{
			value.volume_data.app_config_info_set.app_config_info[i].reserve[0] = ((double)pow(10, m_arrMp3AgcStartGain[i]/20.0)) * 1024;
		}
		
		if(m_arrMidiAgcStartGainBak[i] != m_arrMidiAgcStartGain[i])
		{
			value.volume_data.app_config_info_set.app_config_info[i].reserve[1] = ((double)pow(10, m_arrMidiAgcStartGain[i]/20.0)) * 1024;
		}
		
		if(m_arrMp4AgcStartGainBak[i] != m_arrMp4AgcStartGain[i])
		{
			value.volume_data.app_config_info_set.app_config_info[i].reserve[2] = ((double)pow(10, m_arrMp4AgcStartGain[i]/20.0)) * 1024;
		}
		//
	}

	//teana hu 2011.04.22
	value.volume_data.reserve[63] = m_wDumpPcmCtl;
	//

	if( pSheet->m_bLoadEQ && eq_platform_all != pSheet->m_EQData.eq_platform )
	{
		//teana hu 2011.06.16
		if(m_nCBChannelSelLeft < 5)
		{
			value.volume_data.aud_dev = (value.volume_data.aud_dev & 0xff8f) | (m_nCBChannelSelLeft << 4);
		}
		else
		{
			value.volume_data.aud_dev = (value.volume_data.aud_dev & 0xff8f) | (m_nChannelSelLeft << 4);
		}
		
		if(m_nCBChannelSelRight < 5)
		{
			value.volume_data.aud_dev = (value.volume_data.aud_dev & 0xfff8) | m_nCBChannelSelRight;
		}
		else
		{
			value.volume_data.aud_dev = (value.volume_data.aud_dev & 0xfff8) | m_nChannelSelRight;
		}
		
		value.volume_data.aud_dev = (value.volume_data.aud_dev & 0xff7f) | (m_nPolarLeft << 7);
		value.volume_data.aud_dev = (value.volume_data.aud_dev & 0xfff7) | (m_nPolarRight << 3);
		
		value.volume_data.reserve[16] = m_nAgcRelease;
		value.volume_data.reserve[15] = m_nAgcAttack;
		value.volume_data.reserve[14] = m_nAgcHoldHc;
		value.volume_data.reserve[13] = m_nAgcSdelay;
		//
	}

    int nSize = m_vAllAreas.size();
	for(int j = 0; j < nCount; j++)
	{
		for( int i=0;i<nSize;i++ )
		{
			if(m_vAllAreas[i]->m_strName== _T("arm_volume_settings"))
			{
				((SpecialRectOfArmVolume *)m_vAllAreas[i])->FetchVolume( value, j );
				break;
			}
		}
	}

	return TRUE;
}

void CAudioModePage::SetVolumeData( VOLUME_STRUCT& volume_data,void* lpSheet )
{
	//teana hu 2012.06.12
// 	if((volume_data.reserve[40] & 0xff) == 0x2)
// 	{
// 		m_bSmartPhone = TRUE;
// 	}
// 	else
// 	{
// 		m_bSmartPhone = FALSE;
// 	}
	//
	m_nEQSelect = volume_data.app_config_info_set.aud_proc_exp_control[0] & 0xF;
	m_nDigitalGain = (volume_data.app_config_info_set.aud_proc_exp_control[0] & 0x80) >> 7;	
	m_nMp3LcfSwitch = volume_data.app_config_info_set.aud_proc_exp_control[1] & 0x1;
	m_nMidiLcfSwitch = (volume_data.app_config_info_set.aud_proc_exp_control[1] & 0x2) >> 1;
	m_nMp4LcfSwitch = (volume_data.app_config_info_set.aud_proc_exp_control[1] & 0x4) >> 2;
	m_nMp3AgcSwitch = (volume_data.app_config_info_set.aud_proc_exp_control[1] & 0x100) >> 8;
	m_nMidiAgcSwitch = (volume_data.app_config_info_set.aud_proc_exp_control[1] & 0x200) >> 9;
	m_nMp4AgcSwitch = (volume_data.app_config_info_set.aud_proc_exp_control[1] & 0x400) >> 10;

	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)lpSheet;
	if( pSheet->m_bLoadEQ && eq_platform_all != pSheet->m_EQData.eq_platform )
	{
		m_bMixerSw = ( volume_data.app_config_info_set.aud_proc_exp_control[0] & 0x0020 ) >> 5;

		//teana hu 2011.06.15
		m_nMp3AgcZCSwitch = (volume_data.app_config_info_set.aud_proc_exp_control[0] & 0x100) >> 8;
		m_nMidiAgcZCSwitch = (volume_data.app_config_info_set.aud_proc_exp_control[0] & 0x200) >> 9;
		m_nMp4AgcZCSwitch = (volume_data.app_config_info_set.aud_proc_exp_control[0] & 0x400) >> 10;
		//
		m_nLcfPosition = !( (volume_data.app_config_info_set.aud_proc_exp_control[0] & 0x40) >> 6 );	
	}

	m_nVolumeCount = volume_data.app_config_info_set.valid_app_set_count;

	if( pSheet->m_bLoadEQ && eq_platform_all != pSheet->m_EQData.eq_platform )
	{
		for(int i = 0; i <m_nVolumeCount; i++)
		{	
			int nValue = volume_data.app_config_info_set.app_config_info[i].agc_input_gain[0];
			if(nValue == 0)
			{
				nValue = 1;
			}
			m_arrMp3AgcInputGain[i] = (double)(20 * (double)log10((double)nValue/(double)1024));
			m_arrMp3AgcInputGain[i] = (double)((short)(m_arrMp3AgcInputGain[i] * 10) / 10.0);
			m_arrMp3AgcInputGain[i] = m_arrMp3AgcInputGain[i] < -65535 ? -65535 : m_arrMp3AgcInputGain[i];

			nValue = volume_data.app_config_info_set.app_config_info[i].agc_input_gain[1];
			if(nValue == 0)
			{
				nValue = 1;
			}
			m_arrMidiAgcInputGain[i] = (double)(20 * (double)log10((double)nValue/(double)1024));
			m_arrMidiAgcInputGain[i] = (double)((short)(m_arrMidiAgcInputGain[i] * 10) / 10.0);
			m_arrMidiAgcInputGain[i] = m_arrMidiAgcInputGain[i] < -65535 ? -65535 : m_arrMidiAgcInputGain[i];
			
			nValue = volume_data.app_config_info_set.app_config_info[i].agc_input_gain[2];
			if(nValue == 0)
			{
				nValue = 1;
			}
			m_arrMp4AgcInputGain[i] = (double)(20 * (double)log10((double)nValue/(double)1024));
			m_arrMp4AgcInputGain[i] = (double)((short)(m_arrMp4AgcInputGain[i] * 10) / 10.0);
			m_arrMp4AgcInputGain[i] = m_arrMp4AgcInputGain[i] < -65535 ? -65535 : m_arrMp4AgcInputGain[i];
			
			m_arrMp3AgcInputGainBak[i] = m_arrMp3AgcInputGain[i];
			m_arrMidiAgcInputGainBak[i] = m_arrMidiAgcInputGain[i];
			m_arrMp4AgcInputGainBak[i] = m_arrMp4AgcInputGain[i];
			
			//teana hu 2011.06.15
			nValue = volume_data.app_config_info_set.app_config_info[i].reserve[0];
			if(nValue == 0)
			{
				nValue = 1;
			}
			m_arrMp3AgcStartGain[i] = (double)(20 * (double)log10((double)nValue/(double)1024));
			m_arrMp3AgcStartGain[i] = (double)((short)(m_arrMp3AgcStartGain[i] * 10) / 10.0);
			m_arrMp3AgcStartGain[i] = m_arrMp3AgcStartGain[i] < -65535 ? -65535 : m_arrMp3AgcStartGain[i];
			
			nValue = volume_data.app_config_info_set.app_config_info[i].reserve[1];
			if(nValue == 0)
			{
				nValue = 1;
			}
			m_arrMidiAgcStartGain[i] = (double)(20 * (double)log10((double)nValue/(double)1024));
			m_arrMidiAgcStartGain[i] = (double)((short)(m_arrMidiAgcStartGain[i] * 10) / 10.0);
			m_arrMidiAgcStartGain[i] = m_arrMidiAgcStartGain[i] < -65535 ? -65535 : m_arrMidiAgcStartGain[i];
			
			nValue = volume_data.app_config_info_set.app_config_info[i].reserve[2];
			if(nValue == 0)
			{
				nValue = 1;
			}
			m_arrMp4AgcStartGain[i] = (double)(20 * (double)log10((double)nValue/(double)1024));
			m_arrMp4AgcStartGain[i] = (double)((short)(m_arrMp4AgcStartGain[i] * 10) / 10.0);
			m_arrMp4AgcStartGain[i] = m_arrMp4AgcStartGain[i] < -65535 ? -65535 : m_arrMp4AgcStartGain[i];
			
			m_arrMp3AgcStartGainBak[i] = m_arrMp3AgcStartGain[i];
			m_arrMidiAgcStartGainBak[i] = m_arrMidiAgcStartGain[i];
			m_arrMp4AgcStartGainBak[i] = m_arrMp4AgcStartGain[i];
			//
		}
	}
	
	m_dMp3AgcInputGain = m_arrMp3AgcInputGain[m_nVolumeType];
	m_dMidiAgcInputGain = m_arrMidiAgcInputGain[m_nVolumeType];
	m_dMp4AgcInputGain = m_arrMp4AgcInputGain[m_nVolumeType];

	//teana hu 2011.06.15
	m_dMp3AgcStartGain = m_arrMp3AgcStartGain[m_nVolumeType];
	m_dMidiAgcStartGain = m_arrMidiAgcStartGain[m_nVolumeType];
	m_dMp4AgcStartGain = m_arrMp4AgcStartGain[m_nVolumeType];
	//

	//teana hu 2011.04.22
	m_wDumpPcmCtl = volume_data.reserve[63];
	//

	if( pSheet->m_bLoadEQ && eq_platform_all != pSheet->m_EQData.eq_platform )
	{
		//teana hu 2011.06.16
		m_nChannelSelLeft = (volume_data.aud_dev & 0x70) >> 4;
		if(m_nChannelSelLeft >= 5)
		{
			m_nCBChannelSelLeft = 5;
		}
		else
		{
			m_nCBChannelSelLeft = m_nChannelSelLeft;
		}
		m_nChannelSelRight = volume_data.aud_dev & 0x7;
		if(m_nChannelSelRight >= 5)
		{
			m_nCBChannelSelRight = 5;
		}
		else
		{
			m_nCBChannelSelRight = m_nChannelSelRight;
		}
		m_nPolarLeft = (volume_data.aud_dev & 0x80) >> 7;
		m_nPolarRight = (volume_data.aud_dev & 0x8) >> 3;
		m_nAgcRelease = volume_data.reserve[16];
		m_nAgcRelease = m_nAgcRelease < 0 ? 0 : m_nAgcRelease;
		m_nAgcRelease = m_nAgcRelease > 32767 ? 32767 : m_nAgcRelease;
		m_nAgcAttack = volume_data.reserve[15];
		m_nAgcAttack = m_nAgcAttack < 0 ? 0 : m_nAgcAttack;
		m_nAgcAttack = m_nAgcAttack > 32767 ? 32767 : m_nAgcAttack;
		m_nAgcHoldHc = volume_data.reserve[14];
		m_nAgcHoldHc = m_nAgcHoldHc < 0 ? 0 : m_nAgcHoldHc;
		m_nAgcHoldHc = m_nAgcHoldHc > 32767 ? 32767 : m_nAgcHoldHc;
		m_nAgcSdelay = volume_data.reserve[13];
		m_nAgcSdelay = m_nAgcSdelay < 0 ? 0 : m_nAgcSdelay;
		m_nAgcSdelay = m_nAgcSdelay > 32767 ? 32767 : m_nAgcSdelay;
		//
	}

	for(int i = 0; i < m_nVolumeCount; i++)
	{
		int nArmLevel = volume_data.app_config_info_set.app_config_info[i].valid_volume_level_count;
		if(nArmLevel > 0 )
		{
			nArmLevel += 1;
		}
		for( UINT j=0;j<m_vAllAreas.size();j++ )
		{
			if(m_vAllAreas[j]->m_strName ==  _T("arm_volume_settings"))
			{
				((SpecialRectOfArmVolume *)m_vAllAreas[j])->SetVolumeValue( volume_data, i );
				((SpecialRectOfArmVolume *)m_vAllAreas[j])->SetArmVolumeLevel(nArmLevel, i);
				((SpecialRectOfArmVolume *)m_vAllAreas[j])->m_bOldVer = FALSE;
				break;
			}
		}
	}
}

void CAudioModePage::OnSelchangeComboVolumeType() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nOldIndex = m_nNewIndex;
	if(m_nOldIndex >=0 )
	{
		m_arrMp3AgcInputGain[m_nOldIndex] = m_dMp3AgcInputGain;
		m_arrMidiAgcInputGain[m_nOldIndex] = m_dMidiAgcInputGain;
		m_arrMp4AgcInputGain[m_nOldIndex] = m_dMp4AgcInputGain;

		//teana hu 2011.06.15
		m_arrMp3AgcStartGain[m_nOldIndex] = m_dMp3AgcStartGain;
		m_arrMidiAgcStartGain[m_nOldIndex] = m_dMidiAgcStartGain;
		m_arrMp4AgcStartGain[m_nOldIndex] = m_dMp4AgcStartGain;
		//
	}
	m_nNewIndex = m_nVolumeType;
	m_dMp3AgcInputGain = m_arrMp3AgcInputGain[m_nVolumeType];
	m_dMidiAgcInputGain = m_arrMidiAgcInputGain[m_nVolumeType];
	m_dMp4AgcInputGain = m_arrMp4AgcInputGain[m_nVolumeType];

	//teana hu 2011.06.15
	m_dMp3AgcStartGain = m_arrMp3AgcStartGain[m_nVolumeType];
	m_dMidiAgcStartGain = m_arrMidiAgcStartGain[m_nVolumeType];
	m_dMp4AgcStartGain = m_arrMp4AgcStartGain[m_nVolumeType];
	//

	UpdateData(FALSE);
	
}

void CAudioModePage::OnButtonNxpTxParameter() 
{
	// TODO: Add your control notification handler code here
	CDlgNxpTx dlg;
	CAudioTesterSheet *pSheet = (CAudioTesterSheet *)GetParent();
	if(pSheet != NULL)
	{
		dlg.SetLVVEType(pSheet->m_dwLvveType);
	}
	dlg.SetValue(m_lvve_data);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_lvve_data);
	}
	
}

void CAudioModePage::OnButtonNxpRxParameter() 
{
	// TODO: Add your control notification handler code here
	CDlgNxpRx dlg;
	CAudioTesterSheet *pSheet = (CAudioTesterSheet *)GetParent();
	if(pSheet != NULL)
	{
		dlg.SetLVVEType(pSheet->m_dwLvveType);
	}
	dlg.SetValue(m_lvve_data);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_lvve_data);
	}
}

void CAudioModePage::SetLVVEData(LVVE_CTRL_PARAM_NV_T& lvve_data)
{
	memcpy(&m_lvve_data, &lvve_data, sizeof(LVVE_CTRL_PARAM_NV_T));
}

void CAudioModePage::FetchLVVEData(LVVE_CTRL_PARAM_NV_T& lvve_data)
{
	memcpy(&lvve_data, &m_lvve_data, sizeof(LVVE_CTRL_PARAM_NV_T));
}
void CAudioModePage::OnButtonNxpVoiceBoostCtl() 
{
	// TODO: Add your control notification handler code here
	CDlgByte dlg;
	dlg.SetValue(m_wNXPBoostCtl);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_wNXPBoostCtl);
	}
	
}

void CAudioModePage::OnButtonDumpPcmCtl() 
{
	// TODO: Add your control notification handler code here
	CDlgByte dlg;
	dlg.SetValue(m_wDumpPcmCtl);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_wDumpPcmCtl);
	}
	
}

LRESULT CAudioModePage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( message == WM_DEVICECHANGE )
    {        
        switch(wParam)
        {
		case DBT_DEVICEREMOVECOMPLETE:
			{
				CAudioTesterSheet *pSheet = (CAudioTesterSheet *)GetParent();
				if(pSheet != NULL)
				{
					pSheet->m_SettingsPage.ClearState();
				}
			}	
			break;
        default:
            break;
        }
    }	
	
	return CPropertyPage::WindowProc(message, wParam, lParam);
}

void CAudioModePage::OnBtnShowGraph() 
{
	// TODO: Add your control notification handler code here
	TCHAR szCfgPathName[MAX_PATH];
	::GetModuleFileName( NULL, szCfgPathName, _MAX_PATH );
	CString strPath = szCfgPathName;
	int nIndex = strPath.ReverseFind('\\');
	if(nIndex != -1)
	{
		strPath = strPath.Left(nIndex + 1);
		strPath += _T("showgraph.bmp");
		strPath.Insert( 0,_T("\"") );
		strPath += _T( "\"" );
		CString strCmd = _T("mspaint");

		SHELLEXECUTEINFO shellInfo;
		memset(&shellInfo, 0, sizeof(SHELLEXECUTEINFO));
		shellInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		shellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		shellInfo.lpParameters = strPath.GetBuffer(strCmd.GetLength());
		shellInfo.lpFile = strCmd.GetBuffer(strCmd.GetLength());
		shellInfo.nShow = SW_SHOW;
		ShellExecuteEx(&shellInfo);
		strPath.ReleaseBuffer();
		strCmd.ReleaseBuffer();
	}
	
}

void CAudioModePage::OnSelchangeComboEqSelect() 
{
	m_nEQSelect = ((CComboBox*)GetDlgItem( IDC_COMBO_EQ_SELECT ))->GetCurSel();	

	if( m_nEQSelect > 0 )
	{
		CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
		m_nSoundEqVer = GET_SOUND_EQ_VER( pSheet->m_EQData.lpEQMode[m_nEQSelect-1].eq_mode_control );
	}
	else
	{
		m_nSoundEqVer = sound_eq_ver_none;
	}
	
	RefreshControls();
}

int CAudioModePage::FindEqMode()
{
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();

	if( !pSheet->m_bLoadEQ )
	{
		return -1;
	}

	for( int i=0;i<pSheet->m_EQData.eq_count;i++ )
	{
#ifdef _UNICODE
		TCHAR szName[1024] = {0};
		MultiByteToWideChar(CP_ACP,0,(char*)pSheet->m_EQData.lpEQMode[i].ucParaSetName,-1,szName,1024);
		if( !m_strTitle.Compare( szName+3 ) )
#else
		if( !m_strTitle.Compare( pSheet->m_EQData.lpEQMode[i].ucParaSetName + 3) )
#endif
		{
			return i;
		}
	}

	return -1;
}

void CAudioModePage::SetDspData(AUDIO_NV_DSP_MODE_STRUCT_T& dsp_data, int nLevel)
{
	m_dwAdcSet = dsp_data.adc_set;
	m_dwDacSet = dsp_data.dac_set;
	m_dwPathSet = dsp_data.path_set;

	if(m_bDspCodecInfo)
	{
		for( UINT j=0;j<m_vAllAreas.size();j++ )
		{
			if(m_vAllAreas[j]->m_strName ==  _T("volume_settings"))
			{
				((SpecialRectOfVolume *)m_vAllAreas[j])->SetVolumeLevelFor6530(nLevel);
				
				((SpecialRectOfVolume *)m_vAllAreas[j])->SetDspCodecValue( dsp_data );
				((SpecialRectOfVolume *)m_vAllAreas[j])->SetDspCodecInfo(TRUE);
				break;
			}
		}
	}
}

void CAudioModePage::FetchDspData(AUDIO_NV_DSP_MODE_STRUCT_T& dsp_data)
{
	dsp_data.adc_set = m_dwAdcSet;
	dsp_data.dac_set = m_dwDacSet;
	dsp_data.path_set = m_dwPathSet;

	if(m_bDspCodecInfo)
	{
		for( UINT j=0;j<m_vAllAreas.size();j++ )
		{
			if(m_vAllAreas[j]->m_strName ==  _T("volume_settings"))
			{
				((SpecialRectOfVolume *)m_vAllAreas[j])->FetchDspCodecValue( dsp_data );
				break;
			}
		}
	}
}

void CAudioModePage::OnButtonPathSet() 
{
	// TODO: Add your control notification handler code here
	CAdjustDlg dlg;
	
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
	if(pSheet != NULL && pSheet->m_bLoadDspCodec)
	{
		dlg.Init( m_dwPathSet,IDS_PATH_SET_TITLE );
		
		if( IDOK == dlg.DoModal() )
		{
			m_dwPathSet = dlg.GetResult();
		} 
	}  	
}

UINT CAudioModePage::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//	return CPropertyPage::OnNcHitTest(point);
	return HTTRANSPARENT;
}

void CAudioModePage::OnButtonCng() 
{
	// TODO: Add your control notification handler code here
	CDlgCNG dlg;
	dlg.SetValue(m_dwCNGGain, m_dwCNGSW);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_dwCNGGain, m_dwCNGSW);
	}
	
}
