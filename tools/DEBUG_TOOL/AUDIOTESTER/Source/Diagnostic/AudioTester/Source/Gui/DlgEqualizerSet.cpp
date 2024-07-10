// DlgEqualizerSet.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgEqualizerSet.h"
#include <math.h>
#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define FLOAT_MIN (0 - FLT_MAX)
/////////////////////////////////////////////////////////////////////////////
// CDlgEqualizerSet dialog


CDlgEqualizerSet::CDlgEqualizerSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEqualizerSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEqualizerSet)
		// NOTE: the ClassWizard will add member initialization here
	m_nCBEqType = 0;
	m_nCBLowShelve = 0;
	m_nCBHighShelve = 0;
	m_nCBSmooth = 0;
	m_nCBBandNum = 0;
	m_nLevelStep = 0;
	m_nLevelN = 0;
	//m_nSampleRate = 100;
	m_fLeft = 0.0;
	m_fRight = 0.0;
	m_fRight1 = 0.0;
	m_fRight2 = 0.0;
	m_fRight3 = 0.0;
	m_fRight4 = 0.0;
	m_fRight5 = 0.0;
	m_fRight6 = 0.0;
	m_fRight7 = 0.0;
	m_fRight8 = 0.0;
	m_fRight9 = 0.0;
	m_fRight10 = 0.0;
	m_fRight11 = 0.0;
	m_fRight12 = 0.0;
	m_fRight13 = 0.0;
	m_fRight14 = 0.0;
	m_fRight15 = 0.0;
	m_fRight16 = 0.0;
	m_fCentralFrequency1 = 0;
	m_fCentralFrequency2 = 0;
	m_fCentralFrequency3 = 0;
	m_fCentralFrequency4 = 0;
	m_fCentralFrequency5 = 0;
	m_fCentralFrequency6 = 0;
	m_fCentralFrequency7 = 0;
	m_fCentralFrequency8 = 0;
	m_fWidth1 = 0.0;
	m_fWidth2 = 0.0;
	m_fWidth3 = 0.0;
	m_fWidth4 = 0.0;
	m_fWidth5 = 0.0;
	m_fWidth6 = 0.0;
	m_fWidth7 = 0.0;
	m_fWidth8 = 0.0;
	m_fGain1 = 0.0;
	m_fGain2 = 0.0;
	m_fGain3 = 0.0;
	m_fGain4 = 0.0;
	m_fGain5 = 0.0;
	m_fGain6 = 0.0;
	m_fGain7 = 0.0;
	m_fGain8 = 0.0;
	m_fMasterGain = -72;
	m_fLowShelfCutoff = 0;
	m_fHighShelfCutoff = 0;
	//}}AFX_DATA_INIT
	memset(&m_eq_data, 0, sizeof(EQ_STRUCT_G_T));
	memset(&m_tun_data, 0, sizeof(TUN_STRUCT_G_T));
	memset(&m_VolumeData, 0, sizeof(VOLUME_STRUCT));

	m_pSheet = NULL;
	m_nEQIndex = -1;
	m_nTunIndex = -1;

	m_nOldIndex = -1;
	m_nNewIndex = -1;
	m_fMasterGainBak = -72;
	m_bInit = FALSE;
	m_crBack = RGB(192,192,192);//::GetSysColor(COLOR_3DFACE);
	m_bSetTunBuddy = FALSE;
	m_nOldLevelStep = -1;
	m_nPrestesIndex = 1;
}


void CDlgEqualizerSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEqualizerSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here

	DDX_CBIndex(pDX, IDC_COMBO_EQ_TYPE, m_nCBEqType);
	DDX_CBIndex(pDX, IDC_COMBO_BAND_NUM, m_nCBBandNum);
	DDX_CBIndex(pDX, IDC_COMBO_LOW_SHELVE, m_nCBLowShelve);
	DDX_CBIndex(pDX, IDC_COMBO_HIGH_SHELVE, m_nCBHighShelve);
	DDX_CBIndex(pDX, IDC_COMBO_SMOOTH, m_nCBSmooth);
	DDX_Text(pDX,IDC_EDIT_LEVEL_STEP,m_nLevelStep);
	DDX_Text(pDX,IDC_EDIT_LEVEL_N,m_nLevelN);
	DDV_MinMaxInt(pDX, m_nLevelN, 0, 32767);
	DDX_Text(pDX,IDC_EDIT_SAMPLE_RATE,m_nSampleRate);
	DDX_Text(pDX,IDC_EDIT_LEFT,m_fLeft);
	DDX_Text(pDX,IDC_EDIT_RIGHT,m_fRight);
	DDX_Text(pDX,IDC_EDIT_RIGHT1,m_fRight1);
	DDV_MinMaxFloat(pDX, m_fRight1, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT2,m_fRight2);
	DDV_MinMaxFloat(pDX, m_fRight2, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT3,m_fRight3);
	DDV_MinMaxFloat(pDX, m_fRight3, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT4,m_fRight4);
	DDV_MinMaxFloat(pDX, m_fRight4, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT5,m_fRight5);
	DDV_MinMaxFloat(pDX, m_fRight5, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT6,m_fRight6);
//	DDV_MinMaxFloat(pDX, m_fRight6, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT7,m_fRight7);
//	DDV_MinMaxFloat(pDX, m_fRight7, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT8,m_fRight8);
//	DDV_MinMaxFloat(pDX, m_fRight8, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT9,m_fRight9);
	DDV_MinMaxFloat(pDX, m_fRight9, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT10,m_fRight10);
	DDV_MinMaxFloat(pDX, m_fRight10, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT11,m_fRight11);
	DDV_MinMaxFloat(pDX, m_fRight11, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT12,m_fRight12);
	DDV_MinMaxFloat(pDX, m_fRight12, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT13,m_fRight13);
	DDV_MinMaxFloat(pDX, m_fRight13, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT14,m_fRight14);
//	DDV_MinMaxFloat(pDX, m_fRight14, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT15,m_fRight15);
//	DDV_MinMaxFloat(pDX, m_fRight15, -72, 18);
	DDX_Text(pDX,IDC_EDIT_RIGHT16,m_fRight16);
//	DDV_MinMaxFloat(pDX, m_fRight16, -72, 18);
	DDX_Text(pDX,IDC_EDIT_LOW_SHELF_COTOFF,m_fLowShelfCutoff);
	DDX_Text(pDX,IDC_EDIT_HIGH_SHELF_COTOFF,m_fHighShelfCutoff);
	DDX_Text(pDX,IDC_EDIT_CENTRAL_FREQUENCY1,m_fCentralFrequency1);
	DDV_MinMaxFloat(pDX, m_fCentralFrequency1, 0, 22050);
	DDX_Text(pDX,IDC_EDIT_CENTRAL_FREQUENCY2,m_fCentralFrequency2);
	DDV_MinMaxFloat(pDX, m_fCentralFrequency2, 0, 22050);
	DDX_Text(pDX,IDC_EDIT_CENTRAL_FREQUENCY3,m_fCentralFrequency3);
	DDV_MinMaxFloat(pDX, m_fCentralFrequency3, 0, 22050);
	DDX_Text(pDX,IDC_EDIT_CENTRAL_FREQUENCY4,m_fCentralFrequency4);
	DDV_MinMaxFloat(pDX, m_fCentralFrequency4, 0, 22050);
	DDX_Text(pDX,IDC_EDIT_CENTRAL_FREQUENCY5,m_fCentralFrequency5);
	DDV_MinMaxFloat(pDX, m_fCentralFrequency5, 0, 22050);
	DDX_Text(pDX,IDC_EDIT_CENTRAL_FREQUENCY6,m_fCentralFrequency6);
//	DDV_MinMaxFloat(pDX, m_fCentralFrequency6, 0, 22050);
	DDX_Text(pDX,IDC_EDIT_CENTRAL_FREQUENCY7,m_fCentralFrequency7);
//	DDV_MinMaxFloat(pDX, m_fCentralFrequency7, 0, 22050);
	DDX_Text(pDX,IDC_EDIT_CENTRAL_FREQUENCY8,m_fCentralFrequency8);
//	DDV_MinMaxFloat(pDX, m_fCentralFrequency8, 0, 22050);
	DDX_Text(pDX,IDC_EDIT_WIDTH1,m_fWidth1);
	DDV_MinMaxFloat(pDX, m_fWidth1, 0, 64);
	DDX_Text(pDX,IDC_EDIT_WIDTH2,m_fWidth2);
	DDV_MinMaxFloat(pDX, m_fWidth2, 0, 64);
	DDX_Text(pDX,IDC_EDIT_WIDTH3,m_fWidth3);
	DDV_MinMaxFloat(pDX, m_fWidth3, 0, 64);
	DDX_Text(pDX,IDC_EDIT_WIDTH4,m_fWidth4);
	DDV_MinMaxFloat(pDX, m_fWidth4, 0, 64);
	DDX_Text(pDX,IDC_EDIT_WIDTH5,m_fWidth5);
	DDV_MinMaxFloat(pDX, m_fWidth5, 0, 64);
	DDX_Text(pDX,IDC_EDIT_WIDTH6,m_fWidth6);
//	DDV_MinMaxFloat(pDX, m_fWidth6, 0, 64);
	DDX_Text(pDX,IDC_EDIT_WIDTH7,m_fWidth7);
//	DDV_MinMaxFloat(pDX, m_fWidth7, 0, 64);
	DDX_Text(pDX,IDC_EDIT_WIDTH8,m_fWidth8);
//	DDV_MinMaxFloat(pDX, m_fWidth8, 0, 64);
	DDX_Text(pDX,IDC_EDIT_GAIN1,m_fGain1);
	DDV_MinMaxFloat(pDX, m_fGain1, -72, 18);
	DDX_Text(pDX,IDC_EDIT_GAIN2,m_fGain2);
	DDV_MinMaxFloat(pDX, m_fGain2, -72, 18);
	DDX_Text(pDX,IDC_EDIT_GAIN3,m_fGain3);
	DDV_MinMaxFloat(pDX, m_fGain3, -72, 18);
	DDX_Text(pDX,IDC_EDIT_GAIN4,m_fGain4);
	DDV_MinMaxFloat(pDX, m_fGain4, -72, 18);
	DDX_Text(pDX,IDC_EDIT_GAIN5,m_fGain5);
	DDV_MinMaxFloat(pDX, m_fGain5, -72, 18);
	DDX_Text(pDX,IDC_EDIT_GAIN6,m_fGain6);
//	DDV_MinMaxFloat(pDX, m_fGain6, -72, 18);
	DDX_Text(pDX,IDC_EDIT_GAIN7,m_fGain7);
//	DDV_MinMaxFloat(pDX, m_fGain7, -72, 18);
	DDX_Text(pDX,IDC_EDIT_GAIN8,m_fGain8);
//	DDV_MinMaxFloat(pDX, m_fGain8, -72, 18);
	DDX_Text(pDX,IDC_EDIT_MATSER_GAIN,m_fMasterGain);
	DDV_MinMaxFloat(pDX, m_fMasterGain, -72, 18);
	DDX_Control(pDX,IDC_STATIC_EQ_GRAPH,m_stcGraph);
	DDX_Control(pDX,IDC_SLIDER_LEFT,m_sctrlLeft);
	DDX_Control(pDX,IDC_SLIDER_RIGHT,m_sctrlRight);
	DDX_Control(pDX,IDC_SLIDER_RIGHT1,m_sctrlRight1);
	DDX_Control(pDX,IDC_SLIDER_RIGHT2,m_sctrlRight2);
	DDX_Control(pDX,IDC_SLIDER_RIGHT3,m_sctrlRight3);
	DDX_Control(pDX,IDC_SLIDER_RIGHT4,m_sctrlRight4);
	DDX_Control(pDX,IDC_SLIDER_RIGHT5,m_sctrlRight5);
	DDX_Control(pDX,IDC_SLIDER_RIGHT6,m_sctrlRight6);
	DDX_Control(pDX,IDC_SLIDER_RIGHT7,m_sctrlRight7);
	DDX_Control(pDX,IDC_SLIDER_RIGHT8,m_sctrlRight8);
	DDX_Control(pDX,IDC_SLIDER_RIGHT9,m_sctrlRight9);
	DDX_Control(pDX,IDC_SLIDER_RIGHT10,m_sctrlRight10);
	DDX_Control(pDX,IDC_SLIDER_RIGHT11,m_sctrlRight11);
	DDX_Control(pDX,IDC_SLIDER_RIGHT12,m_sctrlRight12);
	DDX_Control(pDX,IDC_SLIDER_RIGHT13,m_sctrlRight13);
	DDX_Control(pDX,IDC_SLIDER_RIGHT14,m_sctrlRight14);
	DDX_Control(pDX,IDC_SLIDER_RIGHT15,m_sctrlRight15);
	DDX_Control(pDX,IDC_SLIDER_RIGHT16,m_sctrlRight16);
	DDX_Control(pDX,IDC_SLIDER_LOW_SHELF_CUTOFF,m_sctrlLowShelfCutoff);
	DDX_Control(pDX,IDC_SLIDER_HIGH_SHELF_CUTOFF,m_sctrlHighShelfCutoff);
	DDX_Control(pDX,IDC_SLIDER_CENTRAL_FREQUENCY1,m_sctrlCentralFrequency1);
	DDX_Control(pDX,IDC_SLIDER_CENTRAL_FREQUENCY2,m_sctrlCentralFrequency2);
	DDX_Control(pDX,IDC_SLIDER_CENTRAL_FREQUENCY3,m_sctrlCentralFrequency3);
	DDX_Control(pDX,IDC_SLIDER_CENTRAL_FREQUENCY4,m_sctrlCentralFrequency4);
	DDX_Control(pDX,IDC_SLIDER_CENTRAL_FREQUENCY5,m_sctrlCentralFrequency5);
	DDX_Control(pDX,IDC_SLIDER_CENTRAL_FREQUENCY6,m_sctrlCentralFrequency6);
	DDX_Control(pDX,IDC_SLIDER_CENTRAL_FREQUENCY7,m_sctrlCentralFrequency7);
	DDX_Control(pDX,IDC_SLIDER_CENTRAL_FREQUENCY8,m_sctrlCentralFrequency8);
	DDX_Control(pDX,IDC_RADIO_CONSTANT_WIDTH,m_rdConstantWidth);
	DDX_Control(pDX,IDC_RADIO_CONSTANT_Q,m_rdConstantQ);
	DDX_Control(pDX,IDC_CHECK1,m_chk1);
	DDX_Control(pDX,IDC_CHECK2,m_chk2);
	DDX_Control(pDX,IDC_CHECK3,m_chk3);
	DDX_Control(pDX,IDC_CHECK4,m_chk4);
	DDX_Control(pDX,IDC_CHECK5,m_chk5);
	DDX_Control(pDX,IDC_CHECK6,m_chk6);
	DDX_Control(pDX,IDC_CHECK7,m_chk7);
	DDX_Control(pDX,IDC_CHECK8,m_chk8);
	DDX_Control(pDX, IDC_LIST_PRESETS, m_lstPresets);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEqualizerSet, CDialog)
	//{{AFX_MSG_MAP(CDlgEqualizerSet)
 	ON_LBN_SELCHANGE(IDC_LIST_PRESETS, OnSelchangeListPresets)
 	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_EQ_G_LOAD_CURRENT_MODE_FROM_FLASH, OnButtonEqGLoadCurrentModeFromFlash)
	ON_COMMAND(ID_BUTTON_EQ_G_LOAD_CURRENT_MODE_FROM_PHONE, OnButtonEqGLoadCurrentModeFromPhone)
	ON_COMMAND(ID_BUTTON_EQ_G_WRITE_CURRENT_MODE_TO_FLASH, OnButtonEqGWriteCurrentModeToFlash)
	ON_COMMAND(ID_BUTTON_EQ_G_WRITE_CURRENT_MODE_TO_PHONE, OnButtonEqGWriteCurrentModeToPhone)
	ON_CBN_SELCHANGE(IDC_COMBO_EQ_TYPE, OnSelchangeComboEqType)
	ON_CBN_SELCHANGE(IDC_COMBO_BAND_NUM, OnSelchangeComboBandNum)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_BTN_EQ_REFLASH, OnBtnEqReflash)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RIGHT1, OnReleasedcaptureSliderRight1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RIGHT, OnReleasedcaptureSliderRight)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RIGHT2, OnReleasedcaptureSliderRight2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RIGHT3, OnReleasedcaptureSliderRight3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RIGHT4, OnReleasedcaptureSliderRight4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RIGHT5, OnReleasedcaptureSliderRight5)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RIGHT6, OnReleasedcaptureSliderRight6)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RIGHT7, OnReleasedcaptureSliderRight7)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RIGHT8, OnReleasedcaptureSliderRight8)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_LOW_SHELF_CUTOFF, OnReleasedcaptureSliderLowShelfCutoff)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_LEFT, OnReleasedcaptureSliderLeft)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_HIGH_SHELF_CUTOFF, OnReleasedcaptureSliderHighShelfCutoff)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CENTRAL_FREQUENCY8, OnReleasedcaptureSliderCentralFrequency8)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CENTRAL_FREQUENCY7, OnReleasedcaptureSliderCentralFrequency7)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CENTRAL_FREQUENCY6, OnReleasedcaptureSliderCentralFrequency6)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CENTRAL_FREQUENCY5, OnReleasedcaptureSliderCentralFrequency5)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CENTRAL_FREQUENCY4, OnReleasedcaptureSliderCentralFrequency4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CENTRAL_FREQUENCY3, OnReleasedcaptureSliderCentralFrequency3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CENTRAL_FREQUENCY2, OnReleasedcaptureSliderCentralFrequency2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CENTRAL_FREQUENCY1, OnReleasedcaptureSliderCentralFrequency1)
	ON_EN_CHANGE(IDC_EDIT_RIGHT1, OnChangeEditRight1)
	ON_EN_CHANGE(IDC_EDIT_RIGHT, OnChangeEditRight)
	ON_EN_CHANGE(IDC_EDIT_RIGHT2, OnChangeEditRight2)
	ON_EN_CHANGE(IDC_EDIT_RIGHT3, OnChangeEditRight3)
	ON_EN_CHANGE(IDC_EDIT_RIGHT4, OnChangeEditRight4)
	ON_EN_CHANGE(IDC_EDIT_RIGHT5, OnChangeEditRight5)
	ON_EN_CHANGE(IDC_EDIT_RIGHT6, OnChangeEditRight6)
	ON_EN_CHANGE(IDC_EDIT_RIGHT7, OnChangeEditRight7)
	ON_EN_CHANGE(IDC_EDIT_RIGHT8, OnChangeEditRight8)
	ON_EN_CHANGE(IDC_EDIT_LOW_SHELF_COTOFF, OnChangeEditLowShelfCotoff)
	ON_EN_CHANGE(IDC_EDIT_LEFT, OnChangeEditLeft)
	ON_EN_CHANGE(IDC_EDIT_HIGH_SHELF_COTOFF, OnChangeEditHighShelfCotoff)
	ON_EN_CHANGE(IDC_EDIT_CENTRAL_FREQUENCY1, OnChangeEditCentralFrequency1)
	ON_EN_CHANGE(IDC_EDIT_CENTRAL_FREQUENCY2, OnChangeEditCentralFrequency2)
	ON_EN_CHANGE(IDC_EDIT_CENTRAL_FREQUENCY3, OnChangeEditCentralFrequency3)
	ON_EN_CHANGE(IDC_EDIT_CENTRAL_FREQUENCY4, OnChangeEditCentralFrequency4)
	ON_EN_CHANGE(IDC_EDIT_CENTRAL_FREQUENCY5, OnChangeEditCentralFrequency5)
	ON_EN_CHANGE(IDC_EDIT_CENTRAL_FREQUENCY6, OnChangeEditCentralFrequency6)
	ON_EN_CHANGE(IDC_EDIT_CENTRAL_FREQUENCY7, OnChangeEditCentralFrequency7)
	ON_EN_CHANGE(IDC_EDIT_CENTRAL_FREQUENCY8, OnChangeEditCentralFrequency8)
	ON_EN_CHANGE(IDC_EDIT_GAIN1, OnChangeEditGain1)
	ON_EN_CHANGE(IDC_EDIT_GAIN2, OnChangeEditGain2)
	ON_EN_CHANGE(IDC_EDIT_GAIN3, OnChangeEditGain3)
	ON_EN_CHANGE(IDC_EDIT_GAIN4, OnChangeEditGain4)
	ON_EN_CHANGE(IDC_EDIT_GAIN5, OnChangeEditGain5)
	ON_EN_CHANGE(IDC_EDIT_GAIN6, OnChangeEditGain6)
	ON_EN_CHANGE(IDC_EDIT_GAIN7, OnChangeEditGain7)
	ON_EN_CHANGE(IDC_EDIT_GAIN8, OnChangeEditGain8)
	ON_EN_CHANGE(IDC_EDIT_WIDTH1, OnChangeEditWidth1)
	ON_EN_CHANGE(IDC_EDIT_WIDTH2, OnChangeEditWidth2)
	ON_EN_CHANGE(IDC_EDIT_WIDTH3, OnChangeEditWidth3)
	ON_EN_CHANGE(IDC_EDIT_WIDTH4, OnChangeEditWidth4)
	ON_EN_CHANGE(IDC_EDIT_WIDTH5, OnChangeEditWidth5)
	ON_EN_CHANGE(IDC_EDIT_WIDTH6, OnChangeEditWidth6)
	ON_EN_CHANGE(IDC_EDIT_WIDTH7, OnChangeEditWidth7)
	ON_EN_CHANGE(IDC_EDIT_WIDTH8, OnChangeEditWidth8)
	ON_EN_CHANGE(IDC_EDIT_MATSER_GAIN, OnChangeEditMatserGain)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FROM_DEFAULT, OnButtonLoadFromDefault)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TO_DEFAULT, OnButtonSaveToDefault)
	ON_EN_CHANGE(IDC_EDIT_LEVEL_STEP, OnChangeEditLevelStep)
	ON_EN_CHANGE(IDC_EDIT_LEVEL_N, OnChangeEditLevelN)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEqualizerSet message handlers

BOOL CDlgEqualizerSet::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitToolbar();
	//teana hu 2012.06.14
	if(m_bSerialPara)
	{
		m_toolBarEQ.ShowWindow(SW_HIDE);
	}
	//

	m_sctrlLeft.EnableWindow(FALSE);
	m_sctrlRight.EnableWindow(FALSE);
	m_sctrlLowShelfCutoff.EnableWindow(FALSE);
	m_sctrlHighShelfCutoff.EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LEFT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LOW_SHELF_COTOFF)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HIGH_SHELF_COTOFF)->EnableWindow(FALSE);

	GetDlgItem(IDC_COMBO_LOW_SHELVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HIGH_SHELVE)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_SAMPLE_RATE)->EnableWindow(FALSE);

	GetDlgItem(IDC_COMBO_BAND_NUM)->EnableWindow(FALSE);

	GetDlgItem(IDC_COMBO_SMOOTH)->EnableWindow(FALSE);

	for(int i = 0; i < EQ_MODE_G_NUM; i++)
	{
		m_lstPresets.AddString(strPresets[i]);
	}
	
	m_lstPresets.SetCurSel(0);
	m_nPrestesIndex = 1;

	m_rdConstantQ.SetCheck(1);
	m_rdConstantQ.EnableWindow(FALSE);
	m_rdConstantWidth.EnableWindow(FALSE);
	
	// TODO: Add extra initialization here
	InitValue(m_VolumeData);
	InitSLider();
	InitEQValue(m_eq_data, m_tun_data);
	

	int nBandNum = 0;
	if(m_nCBBandNum == 0)
	{
		nBandNum = 5;
	}
	else
	{
		nBandNum = 8;
	}
	
	m_stcGraph.SetBandNum(nBandNum);
	m_stcGraph.SetMasterGain(m_fMasterGain);
	m_stcGraph.SetSampleRate(m_nSampleRate);
	for( i =0; i < nBandNum; i++)
	{
		if(i == 0)
		{
			m_stcGraph.SetValue(i,m_chk1.GetCheck(), m_fCentralFrequency1, m_fWidth1, m_fRight1, m_fGain1);
		}
		else if(i == 1)
		{
			m_stcGraph.SetValue(i,m_chk2.GetCheck(), m_fCentralFrequency2, m_fWidth2, m_fRight2, m_fGain2);
		}
		else if(i == 2)
		{
			m_stcGraph.SetValue(i,m_chk3.GetCheck(), m_fCentralFrequency3, m_fWidth3, m_fRight3, m_fGain3);
		}
		else if(i == 3)
		{
			m_stcGraph.SetValue(i,m_chk4.GetCheck(), m_fCentralFrequency4, m_fWidth4, m_fRight4, m_fGain4);
		}
		else if(i == 4)
		{
			m_stcGraph.SetValue(i,m_chk5.GetCheck(), m_fCentralFrequency5, m_fWidth5, m_fRight5, m_fGain5);
		}
		else if(i == 5)
		{
			m_stcGraph.SetValue(i,m_chk6.GetCheck(), m_fCentralFrequency6, m_fWidth6, m_fRight6, m_fGain6);
		}
		else if(i == 6)
		{
			m_stcGraph.SetValue(i,m_chk7.GetCheck(), m_fCentralFrequency7, m_fWidth7, m_fRight7, m_fGain7);
		}
		else if(i == 7)
		{
			m_stcGraph.SetValue(i,m_chk8.GetCheck(), m_fCentralFrequency8, m_fWidth8, m_fRight8, m_fGain8);
		}
	}

	UpdateData(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEqualizerSet::InitToolbar()
{
    CRect reResultPos;
    GetDlgItem( IDC_STATIC_TOOLBAR_PLACEHOLDER_AUDIO )->GetWindowRect( &reResultPos );
    ScreenToClient( &reResultPos );
    m_toolBarEQ.MoveWindow( &reResultPos );
    
    m_toolBarEQ.ShowWindow( SW_SHOW );
}

void CDlgEqualizerSet::SetValue(VOLUME_STRUCT& volume_data)
{
	memcpy(&m_VolumeData, &volume_data, sizeof(VOLUME_STRUCT));
}

void CDlgEqualizerSet::GetValue(VOLUME_STRUCT& volume_data)
{
	if(!UpdateData())
	{
		return;
	}
	volume_data.reserve[0] = (volume_data.reserve[0] & 0xf7ff) | (m_nCBEqType << 11);
}

void CDlgEqualizerSet::SetEQValue(EQ_STRUCT_G_T& eq_data, TUN_STRUCT_G_T& tun_data)
{
	memcpy(&m_eq_data, &eq_data, sizeof(EQ_STRUCT_G_T));
	memcpy(&m_tun_data, &tun_data, sizeof(TUN_STRUCT_G_T));
}

void CDlgEqualizerSet::InitValue(VOLUME_STRUCT& volume_data)
{
	m_nCBEqType = (volume_data.reserve[0] & 0x800) >> 11;
	ChangeState();
	
	UpdateData(FALSE);
}

void CDlgEqualizerSet::InitEQValue(EQ_STRUCT_G_T& eq_data, TUN_STRUCT_G_T& tun_data)
{
	int nIndex = m_lstPresets.GetCurSel();
	m_nPrestesIndex = nIndex + 1;

	if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
	{
		m_nCBBandNum = (eq_data.eq_control & 0x8000) >> 15;
		m_fRight1 = (float)eq_data.eq_mode[nIndex].eq_band[0].boostdB / 10.0;
		m_fRight1 = (float)((short)floorf(m_fRight1 * 10) / 10.0);
		m_fRight2 = (float)eq_data.eq_mode[nIndex].eq_band[1].boostdB / 10.0;
		m_fRight2 = (float)((short)floorf(m_fRight2 * 10) / 10.0);
		m_fRight3 = (float)eq_data.eq_mode[nIndex].eq_band[2].boostdB / 10.0;
		m_fRight3 = (float)((short)floorf(m_fRight3 * 10) / 10.0);
		m_fRight4 = (float)eq_data.eq_mode[nIndex].eq_band[3].boostdB / 10.0;
		m_fRight4 = (float)((short)floorf(m_fRight4 * 10) / 10.0);
		m_fRight5 = (float)eq_data.eq_mode[nIndex].eq_band[4].boostdB / 10.0;
		m_fRight5 = (float)((short)floorf(m_fRight5 * 10) / 10.0);
		m_fRight6 = (float)eq_data.eq_mode[nIndex].eq_band[5].boostdB / 10.0;
		m_fRight6 = (float)((short)floorf(m_fRight6 * 10) / 10.0);
		m_fRight7 = (float)eq_data.eq_mode[nIndex].eq_band[6].boostdB / 10.0;
		m_fRight7 = (float)((short)floorf(m_fRight7 * 10) / 10.0);
		m_fRight8 = (float)eq_data.eq_mode[nIndex].eq_band[7].boostdB / 10.0;
		m_fRight8 = (float)((short)floorf(m_fRight8 * 10) / 10.0);
		int nValue = eq_data.eq_mode[nIndex].band_control;
		if((nValue & 0x8000) >> 15 == 1)
		{
			m_chk1.SetCheck(1);
			m_sctrlCentralFrequency1.EnableWindow(TRUE);
			m_sctrlCentralFrequency1.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight1.EnableWindow(TRUE);
			m_sctrlRight1.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT1)->EnableWindow(TRUE);
			
		}
		else
		{
			m_chk1.SetCheck(0);
			m_sctrlCentralFrequency1.EnableWindow(FALSE);
			m_sctrlCentralFrequency1.SetPrimaryColor(m_crBack);
			m_sctrlRight1.EnableWindow(FALSE);
			m_sctrlRight1.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT1)->EnableWindow(FALSE);
		}
		
		if((nValue & 0x4000) >> 14 == 1)
		{
			m_chk2.SetCheck(1);
			m_sctrlCentralFrequency2.EnableWindow(TRUE);
			m_sctrlCentralFrequency2.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight2.EnableWindow(TRUE);
			m_sctrlRight2.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT2)->EnableWindow(TRUE);
			
		}
		else
		{
			m_chk2.SetCheck(0);
			m_sctrlCentralFrequency2.EnableWindow(FALSE);
			m_sctrlCentralFrequency2.SetPrimaryColor(m_crBack);
			m_sctrlRight2.EnableWindow(FALSE);
			m_sctrlRight2.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT2)->EnableWindow(FALSE);
		}
		
		if((nValue & 0x2000) >> 13 == 1)
		{
			m_chk3.SetCheck(1);
			m_sctrlCentralFrequency3.EnableWindow(TRUE);
			m_sctrlCentralFrequency3.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight3.EnableWindow(TRUE);
			m_sctrlRight3.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT3)->EnableWindow(TRUE);
			
		}
		else
		{
			m_chk3.SetCheck(0);
			m_sctrlCentralFrequency3.EnableWindow(FALSE);
			m_sctrlCentralFrequency3.SetPrimaryColor(m_crBack);
			m_sctrlRight3.EnableWindow(FALSE);
			m_sctrlRight3.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT3)->EnableWindow(FALSE);
		}
		
		if((nValue & 0x1000) >> 12 == 1)
		{
			m_chk4.SetCheck(1);
			m_sctrlCentralFrequency4.EnableWindow(TRUE);
			m_sctrlCentralFrequency4.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight4.EnableWindow(TRUE);
			m_sctrlRight4.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY4)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH4)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN4)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT4)->EnableWindow(TRUE);
		}
		else
		{
			m_chk4.SetCheck(0);
			m_sctrlCentralFrequency4.EnableWindow(FALSE);
			m_sctrlCentralFrequency4.SetPrimaryColor(m_crBack);
			m_sctrlRight4.EnableWindow(FALSE);
			m_sctrlRight4.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY4)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH4)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN4)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT4)->EnableWindow(FALSE);
		}
		
		if((nValue & 0x800) >> 11 == 1)
		{
			m_chk5.SetCheck(1);
			m_sctrlCentralFrequency5.EnableWindow(TRUE);
			m_sctrlCentralFrequency5.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight5.EnableWindow(TRUE);
			m_sctrlRight5.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY5)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH5)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN5)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT5)->EnableWindow(TRUE);
		}
		else
		{
			m_chk5.SetCheck(0);
			m_sctrlCentralFrequency5.EnableWindow(FALSE);
			m_sctrlCentralFrequency5.SetPrimaryColor(m_crBack);
			m_sctrlRight5.EnableWindow(FALSE);
			m_sctrlRight5.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY5)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH5)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN5)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT5)->EnableWindow(FALSE);
		}

		if((nValue & 0x400) >> 10 == 1)
		{
			m_chk6.SetCheck(1);
			m_sctrlCentralFrequency6.EnableWindow(TRUE);
			m_sctrlCentralFrequency6.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight6.EnableWindow(TRUE);
			m_sctrlRight6.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT6)->EnableWindow(TRUE);
		}
		else
		{
			m_chk6.SetCheck(0);
			m_sctrlCentralFrequency6.EnableWindow(FALSE);
			m_sctrlCentralFrequency6.SetPrimaryColor(m_crBack);
			m_sctrlRight6.EnableWindow(FALSE);
			m_sctrlRight6.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH6)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN6)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT6)->EnableWindow(FALSE);
		}

		if((nValue & 0x200) >> 9 == 1)
		{
			m_chk7.SetCheck(1);
			m_sctrlCentralFrequency7.EnableWindow(TRUE);
			m_sctrlCentralFrequency7.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight7.EnableWindow(TRUE);
			m_sctrlRight7.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY7)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH7)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN7)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT7)->EnableWindow(TRUE);
		}
		else
		{
			m_chk7.SetCheck(0);
			m_sctrlCentralFrequency7.EnableWindow(FALSE);
			m_sctrlCentralFrequency7.SetPrimaryColor(m_crBack);
			m_sctrlRight7.EnableWindow(FALSE);
			m_sctrlRight7.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY7)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH7)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN7)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT7)->EnableWindow(FALSE);
		}

		if((nValue & 0x100) >> 8 == 1)
		{
			m_chk8.SetCheck(1);
			m_sctrlCentralFrequency8.EnableWindow(TRUE);
			m_sctrlCentralFrequency8.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight8.EnableWindow(TRUE);
			m_sctrlRight8.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT6)->EnableWindow(TRUE);
		}
		else
		{
			m_chk8.SetCheck(0);
			m_sctrlCentralFrequency8.EnableWindow(FALSE);
			m_sctrlCentralFrequency8.SetPrimaryColor(m_crBack);
			m_sctrlRight8.EnableWindow(FALSE);
			m_sctrlRight8.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY8)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH8)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN8)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT8)->EnableWindow(FALSE);
		}

		m_fCentralFrequency1 = eq_data.eq_mode[nIndex].eq_band[0].f0;
		m_fCentralFrequency2 = eq_data.eq_mode[nIndex].eq_band[1].f0;
		m_fCentralFrequency3 = eq_data.eq_mode[nIndex].eq_band[2].f0;
		m_fCentralFrequency4 = eq_data.eq_mode[nIndex].eq_band[3].f0;
		m_fCentralFrequency5 = eq_data.eq_mode[nIndex].eq_band[4].f0;
		m_fCentralFrequency6 = eq_data.eq_mode[nIndex].eq_band[5].f0;
		m_fCentralFrequency7 = eq_data.eq_mode[nIndex].eq_band[6].f0;
		m_fCentralFrequency8 = eq_data.eq_mode[nIndex].eq_band[7].f0;

		m_fWidth1 = (float)eq_data.eq_mode[nIndex].eq_band[0].q / 512.0;
		m_fWidth1 = (float)((short)floorf(m_fWidth1 * 1000) / 1000.0);
		m_fWidth2 = (float)eq_data.eq_mode[nIndex].eq_band[1].q / 512.0;
		m_fWidth2 = (float)((short)floorf(m_fWidth2 * 1000) / 1000.0);
		m_fWidth3 = (float)eq_data.eq_mode[nIndex].eq_band[2].q / 512.0;
		m_fWidth3 = (float)((short)floorf(m_fWidth3 * 1000) / 1000.0);
		m_fWidth4 = (float)eq_data.eq_mode[nIndex].eq_band[3].q / 512.0;
		m_fWidth4 = (float)((short)floorf(m_fWidth4 * 1000) / 1000.0);
		m_fWidth5 = (float)eq_data.eq_mode[nIndex].eq_band[4].q / 512.0;
		m_fWidth5 = (float)((short)floorf(m_fWidth5 * 1000) / 1000.0);
		m_fWidth6 = (float)eq_data.eq_mode[nIndex].eq_band[5].q / 512.0;
		m_fWidth6 = (float)((short)floorf(m_fWidth6 * 1000) / 1000.0);
		m_fWidth7 = (float)eq_data.eq_mode[nIndex].eq_band[6].q / 512.0;
		m_fWidth7 = (float)((short)floorf(m_fWidth7 * 1000) / 1000.0);
		m_fWidth8 = (float)eq_data.eq_mode[nIndex].eq_band[7].q / 512.0;
		m_fWidth8 = (float)((short)floorf(m_fWidth8 * 1000) / 1000.0);

		GetDlgItem(IDC_STATIC_GAIN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GAIN1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GAIN2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GAIN3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GAIN4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GAIN5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GAIN6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GAIN7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GAIN8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_G1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_G2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_G3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_G4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_G5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_G6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_G7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_G8)->ShowWindow(SW_SHOW);

		m_fGain1 = (float)eq_data.eq_mode[nIndex].eq_band[0].gaindB / 10.0;
		m_fGain1 = (float)((short)floorf(m_fGain1 * 10) / 10.0);
		m_fGain2 = (float)eq_data.eq_mode[nIndex].eq_band[1].gaindB / 10.0;
		m_fGain2 = (float)((short)floorf(m_fGain2 * 10) / 10.0);
		m_fGain3 = (float)eq_data.eq_mode[nIndex].eq_band[2].gaindB / 10.0;
		m_fGain3 = (float)((short)floorf(m_fGain3 * 10) / 10.0);
		m_fGain4 = (float)eq_data.eq_mode[nIndex].eq_band[3].gaindB / 10.0;
		m_fGain4 = (float)((short)floorf(m_fGain4 * 10) / 10.0);
		m_fGain5 = (float)eq_data.eq_mode[nIndex].eq_band[4].gaindB / 10.0;
		m_fGain5 = (float)((short)floorf(m_fGain5 * 10) / 10.0);
		m_fGain6 = (float)eq_data.eq_mode[nIndex].eq_band[5].gaindB / 10.0;
		m_fGain6 = (float)((short)floorf(m_fGain6 * 10) / 10.0);
		m_fGain7 = (float)eq_data.eq_mode[nIndex].eq_band[6].gaindB / 10.0;
		m_fGain7 = (float)((short)floorf(m_fGain7 * 10) / 10.0);
		m_fGain8 = (float)eq_data.eq_mode[nIndex].eq_band[7].gaindB / 10.0;
		m_fGain8 = (float)((short)floorf(m_fGain8 * 10) / 10.0);

		nValue = eq_data.eq_mode[nIndex].agc_in_gain;
		if(nValue == 0)
		{
			nValue = 1;
		}
		m_fMasterGain = (double)(20 * (double)log10((double)nValue/(double)4096));
		m_fMasterGain = (double)((short)floorf(m_fMasterGain * 10) / 10.0);
		m_fMasterGain = m_fMasterGain < -72 ? -72 : m_fMasterGain;
		m_fMasterGainBak = m_fMasterGain;

		m_nCBLowShelve = eq_data.eq_mode[nIndex].band_control & 0x1;
		m_nCBHighShelve = (eq_data.eq_mode[nIndex].band_control & 0x2) >> 1;

	}
	else
	{
		m_nCBBandNum = (tun_data.eq_control & 0x8000) >> 15;
		m_nLevelStep = (tun_data.eq_control & 0x03ff) / 10;
		m_nOldLevelStep = m_nLevelStep;
		m_nCBSmooth = (tun_data.eq_control & 0x3000) >> 12;
		m_fRight9 = tun_data.eq_mode[nIndex].boostdB_current_band[0] * m_nLevelStep;
		m_fRight10 = tun_data.eq_mode[nIndex].boostdB_current_band[1] * m_nLevelStep;
		m_fRight11 = tun_data.eq_mode[nIndex].boostdB_current_band[2] * m_nLevelStep;
		m_fRight12 = tun_data.eq_mode[nIndex].boostdB_current_band[3] * m_nLevelStep;
		m_fRight13 = tun_data.eq_mode[nIndex].boostdB_current_band[4] * m_nLevelStep;
		m_fRight14 = tun_data.eq_mode[nIndex].boostdB_current_band[5] * m_nLevelStep;
		m_fRight15 = tun_data.eq_mode[nIndex].boostdB_current_band[6] * m_nLevelStep;
		m_fRight16 = tun_data.eq_mode[nIndex].boostdB_current_band[7] * m_nLevelStep;

		m_fCentralFrequency1 = tun_data.f0_band[0];
		m_fCentralFrequency2 = tun_data.f0_band[1];
		m_fCentralFrequency3 = tun_data.f0_band[2];
		m_fCentralFrequency4 = tun_data.f0_band[3];
		m_fCentralFrequency5 = tun_data.f0_band[4];
		m_fCentralFrequency6 = tun_data.f0_band[5];
		m_fCentralFrequency7 = tun_data.f0_band[6];
		m_fCentralFrequency8 = tun_data.f0_band[7];

		m_fWidth1 = (float)tun_data.q[0] / 512.0;
		m_fWidth1 = (float)((short)floorf(m_fWidth1 * 1000) / 1000.0);
		m_fWidth2 = (float)tun_data.q[1] / 512.0;
		m_fWidth2 = (float)((short)floorf(m_fWidth2 * 1000) / 1000.0);
		m_fWidth3 = (float)tun_data.q[2] / 512.0;
		m_fWidth3 = (float)((short)floorf(m_fWidth3 * 1000) / 1000.0);
		m_fWidth4 = (float)tun_data.q[3] / 512.0;
		m_fWidth4 = (float)((short)floorf(m_fWidth4 * 1000) / 1000.0);
		m_fWidth5 = (float)tun_data.q[4] / 512.0;
		m_fWidth5 = (float)((short)floorf(m_fWidth5 * 1000) / 1000.0);
		m_fWidth6 = (float)tun_data.q[5] / 512.0;
		m_fWidth6 = (float)((short)floorf(m_fWidth6 * 1000) / 1000.0);
		m_fWidth7 = (float)tun_data.q[6] / 512.0;
		m_fWidth7 = (float)((short)floorf(m_fWidth7 * 1000) / 1000.0);
		m_fWidth8 = (float)tun_data.q[7] / 512.0;
		m_fWidth8 = (float)((short)floorf(m_fWidth8 * 1000) / 1000.0);

		//m_fGain1 = (float)tun_data.gaindB / 10.0;
		//m_fGain1 = (float)((short)(m_fGain1 * 10) / 10.0);
		m_nLevelN = tun_data.level_n;
		m_fGain1 = 0;
		m_fGain2 = 0;
		m_fGain3 = 0;
		m_fGain4 = 0;
		m_fGain5 = 0;
		m_fGain6 = 0;
		m_fGain7 = 0;
		m_fGain8 = 0;
		GetDlgItem(IDC_STATIC_GAIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_G1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_G2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_G3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_G4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_G5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_G6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_G7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_G8)->ShowWindow(SW_HIDE);

		int nValue = tun_data.eq_mode[nIndex].agc_in_gain;
		if(nValue == 0)
		{
			nValue = 1;
		}
		m_fMasterGain = (double)(20 * (double)log10((double)nValue/(double)4096));
		m_fMasterGain = (double)((short)floorf(m_fMasterGain * 10) / 10.0);
		m_fMasterGain = m_fMasterGain < -72 ? -72 : m_fMasterGain;
		m_fMasterGainBak = m_fMasterGain;

		m_nCBLowShelve = tun_data.eq_mode[nIndex].band_control & 0x1;
		m_nCBHighShelve = (tun_data.eq_mode[nIndex].band_control & 0x2) >> 1;
		
	}
	UpdateData(FALSE);

	SetSliderPos();
	ChangeSctrlState();

	UpdateData(FALSE);
	m_bInit = TRUE;
}

void CDlgEqualizerSet::GetEQValue()
{
	if(!UpdateData())
	{
		return;
	}
	int nIndex = m_lstPresets.GetCurSel();
	m_nPrestesIndex = nIndex + 1;
	ChangeEQValue(nIndex);

	if(m_nCBEqType == 0  || m_lstPresets.GetCurSel() == 0)
	{
		//m_nCBBandNum = (m_eq_data.eq_control & 0x8000) >> 15;
		m_eq_data.eq_control &= 0x7fff;
		m_eq_data.eq_control |= (m_nCBBandNum << 15);

	}
	else
	{
		int n = m_tun_data.eq_control;
		n &= 0x7fff;
		n |= (m_nCBBandNum << 15);
		n &= 0xfc00;
		n |= (m_nLevelStep*10);
		n &= 0xcfff;
		n |= (m_nCBSmooth << 12);
		m_tun_data.eq_control = n;

		m_tun_data.f0_band[0] = m_fCentralFrequency1;
		m_tun_data.f0_band[1] = m_fCentralFrequency2;
		m_tun_data.f0_band[2] = m_fCentralFrequency3;
		m_tun_data.f0_band[3] = m_fCentralFrequency4;
		m_tun_data.f0_band[4] = m_fCentralFrequency5;
		m_tun_data.f0_band[5] = m_fCentralFrequency6;
		m_tun_data.f0_band[6] = m_fCentralFrequency7;
		m_tun_data.f0_band[7] = m_fCentralFrequency8;

		m_tun_data.q[0] = ceilf(m_fWidth1 * 512.0);
		m_tun_data.q[1] = ceilf(m_fWidth2 * 512.0);
		m_tun_data.q[2] = ceilf(m_fWidth3 * 512.0);
		m_tun_data.q[3] = ceilf(m_fWidth4 * 512.0);
		m_tun_data.q[4] = ceilf(m_fWidth5 * 512.0);
		m_tun_data.q[5] = ceilf(m_fWidth6 * 512.0);
		m_tun_data.q[6] = ceilf(m_fWidth7 * 512.0);
		m_tun_data.q[7] = ceilf(m_fWidth8 * 512.0);

		m_tun_data.level_n = m_nLevelN;
		
	}

}

void CDlgEqualizerSet::OnSelchangeListPresets() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData())
	{
		return;
	}
	//ChangeValue();

	int nIndex = m_lstPresets.GetCurSel();
	m_nPrestesIndex = nIndex + 1;
	
	m_nOldIndex = m_nNewIndex;
	if(m_nOldIndex >=0 )
	{
		ChangeEQValue(m_nOldIndex);
	}
	m_nNewIndex = nIndex;

	ChangeState();
	InitEQValue(m_eq_data, m_tun_data);

	ReflashGraph();
	
}

void CDlgEqualizerSet::FetchEQValue(EQ_STRUCT_G_T& eq_data, TUN_STRUCT_G_T& tun_data)
{
	memcpy(&eq_data, &m_eq_data, sizeof(EQ_STRUCT_G_T));
	memcpy(&tun_data, &m_tun_data, sizeof(TUN_STRUCT_G_T));
}

void CDlgEqualizerSet::FetchValue(VOLUME_STRUCT& volume_data)
{
	memcpy(&volume_data, &m_VolumeData, sizeof(VOLUME_STRUCT));
}

int CDlgEqualizerSet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	if (!m_toolBarEQ.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_toolBarEQ.LoadToolBar(IDR_TOOLBAR_EQ_G))
	{
		DWORD dwErr = GetLastError();
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	CToolBarCtrl &ctl = m_toolBarEQ.GetToolBarCtrl();
	DWORD dwStyle = ctl.GetStyle();
	dwStyle |= CBRS_TOOLTIPS;
	ctl.SetStyle(dwStyle);
	
	return 0;
}

void CDlgEqualizerSet::OnOK() 
{
	// TODO: Add extra validation here
	GetEQValue();
	GetValue(m_VolumeData);

// 	CMusicModePage *pPage = (CMusicModePage *)GetParent();
// 	if(pPage != NULL)
// 	{
// 		pPage
// 	}
	CDialog::OnOK();
}

void CDlgEqualizerSet::OnButtonEqGLoadCurrentModeFromFlash() 
{
	// TODO: Add your command handler code here
	if(m_pSheet != NULL)
	{
		
		m_pSheet->LoadENHAModes(TYPE_EQ, GET_ENHA_DATA_FROM_FLASH);
		m_pSheet->LoadENHAModes(TYPE_TUN, GET_ENHA_DATA_FROM_FLASH);
		SetEQValue(m_pSheet->m_eqData_g.lpEQMode[m_nEQIndex], m_pSheet->m_tunData_g.lpTunMode[m_nTunIndex]);
		
		InitEQValue(m_eq_data, m_tun_data);
	}
	ReflashGraph();
}

void CDlgEqualizerSet::OnButtonEqGLoadCurrentModeFromPhone() 
{
	// TODO: Add your command handler code here
	if(m_pSheet != NULL)
	{
		m_pSheet->LoadENHAModes(TYPE_EQ, GET_ENHA_DATA_FROM_RAM);
		m_pSheet->LoadENHAModes(TYPE_TUN, GET_ENHA_DATA_FROM_RAM);
		SetEQValue(m_pSheet->m_eqData_g.lpEQMode[m_nEQIndex], m_pSheet->m_tunData_g.lpTunMode[m_nTunIndex]);
		
		InitEQValue(m_eq_data, m_tun_data);
	}
	ReflashGraph();
}

void CDlgEqualizerSet::OnButtonEqGWriteCurrentModeToFlash() 
{
	// TODO: Add your command handler code here
	if(m_pSheet != NULL)
	{
		GetEQValue();
		FetchEQValue(m_pSheet->m_eqData_g.lpEQMode[m_nEQIndex], m_pSheet->m_tunData_g.lpTunMode[m_nTunIndex]);
		if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
		{
			m_pSheet->WriteENHAParaToPhone(m_nEQIndex, TYPE_EQ, SET_ENHA_DATA_TO_FLASH,0);
		}
		else
		{
			m_pSheet->WriteENHAParaToPhone(m_nTunIndex, TYPE_TUN, SET_ENHA_DATA_TO_FLASH,0);
		}
	}
	ReflashGraph();
}

void CDlgEqualizerSet::OnButtonEqGWriteCurrentModeToPhone() 
{
	// TODO: Add your command handler code here
	if(m_pSheet != NULL)
	{
		int nIndex= m_lstPresets.GetCurSel()+1;
		GetEQValue();
		FetchEQValue(m_pSheet->m_eqData_g.lpEQMode[m_nEQIndex], m_pSheet->m_tunData_g.lpTunMode[m_nTunIndex]);
		if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
		{
			m_pSheet->WriteENHAParaToPhone(m_nEQIndex, TYPE_EQ, SET_ENHA_DATA_TO_RAM, nIndex);
		}
		else
		{
			m_pSheet->WriteENHAParaToPhone(m_nTunIndex, TYPE_TUN, SET_ENHA_DATA_TO_RAM, nIndex);
		}
		
	}
	ReflashGraph();
}

void CDlgEqualizerSet::OnSelchangeComboEqType() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData())
	{
		return;
	}
	ChangeState();
	InitEQValue(m_eq_data, m_tun_data);
//	ChangeValue();
	m_stcGraph.Clear();
	ReflashGraph();
	
}

void CDlgEqualizerSet::OnSelchangeComboBandNum() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData())
	{
		return;
	}
	ChangeSctrlState();
	ReflashGraph();
}

void CDlgEqualizerSet::ChangeValue()
{
	int nIndex = m_lstPresets.GetCurSel();
	m_nPrestesIndex = nIndex + 1;
	if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
	{
		m_fRight1 = (float)m_eq_data.eq_mode[nIndex].eq_band[0].boostdB / 10.0;
		m_fRight1 = (float)((short)floorf(m_fRight1 * 10) / 10.0);
		m_fRight2 = (float)m_eq_data.eq_mode[nIndex].eq_band[1].boostdB / 10.0;
		m_fRight2 = (float)((short)floorf(m_fRight2 * 10) / 10.0);
		m_fRight3 = (float)m_eq_data.eq_mode[nIndex].eq_band[2].boostdB / 10.0;
		m_fRight3 = (float)((short)floorf(m_fRight3 * 10) / 10.0);
		m_fRight4 = (float)m_eq_data.eq_mode[nIndex].eq_band[3].boostdB / 10.0;
		m_fRight4 = (float)((short)floorf(m_fRight4 * 10) / 10.0);
		m_fRight5 = (float)m_eq_data.eq_mode[nIndex].eq_band[4].boostdB / 10.0;
		m_fRight5 = (float)((short)floorf(m_fRight5 * 10) / 10.0);
		m_fRight6 = (float)m_eq_data.eq_mode[nIndex].eq_band[5].boostdB / 10.0;
		m_fRight6 = (float)((short)floorf(m_fRight6 * 10) / 10.0);
		m_fRight7 = (float)m_eq_data.eq_mode[nIndex].eq_band[6].boostdB / 10.0;
		m_fRight7 = (float)((short)floorf(m_fRight7 * 10) / 10.0);
		m_fRight8 = (float)m_eq_data.eq_mode[nIndex].eq_band[7].boostdB / 10.0;
		m_fRight8 = (float)((short)floorf(m_fRight8 * 10) / 10.0);
		int nValue = m_eq_data.eq_mode[nIndex].band_control;
		if((nValue & 0x8000) >> 15 == 1)
		{
			m_chk1.SetCheck(1);
			m_sctrlCentralFrequency1.EnableWindow(TRUE);
			m_sctrlCentralFrequency1.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight1.EnableWindow(TRUE);
			m_sctrlRight1.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT1)->EnableWindow(TRUE);
			
		}
		else
		{
			m_chk1.SetCheck(0);
			m_sctrlCentralFrequency1.EnableWindow(FALSE);
			m_sctrlCentralFrequency1.SetPrimaryColor(m_crBack);
			m_sctrlRight1.EnableWindow(FALSE);
			m_sctrlRight1.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT1)->EnableWindow(FALSE);
		}
		
		if((nValue & 0x4000) >> 14 == 1)
		{
			m_chk2.SetCheck(1);
			m_sctrlCentralFrequency2.EnableWindow(TRUE);
			m_sctrlCentralFrequency2.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight2.EnableWindow(TRUE);
			m_sctrlRight2.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT2)->EnableWindow(TRUE);
			
		}
		else
		{
			m_chk2.SetCheck(0);
			m_sctrlCentralFrequency2.EnableWindow(FALSE);
			m_sctrlCentralFrequency2.SetPrimaryColor(m_crBack);
			m_sctrlRight2.EnableWindow(FALSE);
			m_sctrlRight2.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT2)->EnableWindow(FALSE);
		}
		
		if((nValue & 0x2000) >> 13 == 1)
		{
			m_chk3.SetCheck(1);
			m_sctrlCentralFrequency3.EnableWindow(TRUE);
			m_sctrlCentralFrequency3.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight3.EnableWindow(TRUE);
			m_sctrlRight3.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT3)->EnableWindow(TRUE);
			
		}
		else
		{
			m_chk3.SetCheck(0);
			m_sctrlCentralFrequency3.EnableWindow(FALSE);
			m_sctrlCentralFrequency3.SetPrimaryColor(m_crBack);
			m_sctrlRight3.EnableWindow(FALSE);
			m_sctrlRight3.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT3)->EnableWindow(FALSE);
		}
		
		if((nValue & 0x1000) >> 12 == 1)
		{
			m_chk4.SetCheck(1);
			m_sctrlCentralFrequency4.EnableWindow(TRUE);
			m_sctrlCentralFrequency4.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight4.EnableWindow(TRUE);
			m_sctrlRight4.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY4)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH4)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN4)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT4)->EnableWindow(TRUE);
			
		}
		else
		{
			m_chk4.SetCheck(0);
			m_sctrlCentralFrequency4.EnableWindow(FALSE);
			m_sctrlCentralFrequency4.SetPrimaryColor(m_crBack);
			m_sctrlRight4.EnableWindow(FALSE);
			m_sctrlRight4.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY4)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH4)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN4)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT4)->EnableWindow(FALSE);
		}
		
		if((nValue & 0x800) >> 11 == 1)
		{
			m_chk5.SetCheck(1);
			m_sctrlCentralFrequency5.EnableWindow(TRUE);
			m_sctrlCentralFrequency5.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight5.EnableWindow(TRUE);
			m_sctrlRight5.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY5)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH5)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN5)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT5)->EnableWindow(TRUE);
			
		}
		else
		{
			m_chk5.SetCheck(0);
			m_sctrlCentralFrequency5.EnableWindow(FALSE);
			m_sctrlCentralFrequency5.SetPrimaryColor(m_crBack);
			m_sctrlRight5.EnableWindow(FALSE);
			m_sctrlRight5.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY5)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH5)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN5)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT5)->EnableWindow(FALSE);
		}

		if((nValue & 0x400) >> 10 == 1)
		{
			m_chk6.SetCheck(1);
			m_sctrlCentralFrequency6.EnableWindow(TRUE);
			m_sctrlCentralFrequency6.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight6.EnableWindow(TRUE);
			m_sctrlRight6.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT6)->EnableWindow(TRUE);
		}
		else
		{
			m_chk6.SetCheck(0);
			m_sctrlCentralFrequency6.EnableWindow(FALSE);
			m_sctrlCentralFrequency6.SetPrimaryColor(m_crBack);
			m_sctrlRight6.EnableWindow(FALSE);
			m_sctrlRight6.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH6)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN6)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT6)->EnableWindow(FALSE);
		}
		
		if((nValue & 0x200) >> 9 == 1)
		{
			m_chk7.SetCheck(1);
			m_sctrlCentralFrequency7.EnableWindow(TRUE);
			m_sctrlCentralFrequency7.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight7.EnableWindow(TRUE);
			m_sctrlRight7.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY7)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH7)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN7)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT7)->EnableWindow(TRUE);
		}
		else
		{
			m_chk7.SetCheck(0);
			m_sctrlCentralFrequency7.EnableWindow(FALSE);
			m_sctrlCentralFrequency7.SetPrimaryColor(m_crBack);
			m_sctrlRight7.EnableWindow(FALSE);
			m_sctrlRight7.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY7)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH7)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN7)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT7)->EnableWindow(FALSE);
		}
		
		if((nValue & 0x100) >> 8 == 1)
		{
			m_chk8.SetCheck(1);
			m_sctrlCentralFrequency8.EnableWindow(TRUE);
			m_sctrlCentralFrequency8.SetPrimaryColor(RGB(128,128,128));
			m_sctrlRight8.EnableWindow(TRUE);
			m_sctrlRight8.SetPrimaryColor(RGB(128,128,128));
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_WIDTH6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN6)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT6)->EnableWindow(TRUE);
		}
		else
		{
			m_chk8.SetCheck(0);
			m_sctrlCentralFrequency8.EnableWindow(FALSE);
			m_sctrlCentralFrequency8.SetPrimaryColor(m_crBack);
			m_sctrlRight8.EnableWindow(FALSE);
			m_sctrlRight8.SetPrimaryColor(m_crBack);
			GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY8)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_WIDTH8)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GAIN8)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT8)->EnableWindow(FALSE);
		}

		m_fCentralFrequency1 = m_eq_data.eq_mode[nIndex].eq_band[0].f0;
		m_fCentralFrequency2 = m_eq_data.eq_mode[nIndex].eq_band[1].f0;
		m_fCentralFrequency3 = m_eq_data.eq_mode[nIndex].eq_band[2].f0;
		m_fCentralFrequency4 = m_eq_data.eq_mode[nIndex].eq_band[3].f0;
		m_fCentralFrequency5 = m_eq_data.eq_mode[nIndex].eq_band[4].f0;
		m_fCentralFrequency6 = m_eq_data.eq_mode[nIndex].eq_band[5].f0;
		m_fCentralFrequency7 = m_eq_data.eq_mode[nIndex].eq_band[6].f0;
		m_fCentralFrequency8 = m_eq_data.eq_mode[nIndex].eq_band[7].f0;

		m_fWidth1 = (float)m_eq_data.eq_mode[nIndex].eq_band[0].q / 512.0;
		m_fWidth1 = (float)((short)floorf(m_fWidth1 * 1000) / 1000.0);
		m_fWidth2 = (float)m_eq_data.eq_mode[nIndex].eq_band[1].q / 512.0;
		m_fWidth2 = (float)((short)floorf(m_fWidth2 * 1000) / 1000.0);
		m_fWidth3 = (float)m_eq_data.eq_mode[nIndex].eq_band[2].q / 512.0;
		m_fWidth3 = (float)((short)floorf(m_fWidth3 * 1000) / 1000.0);
		m_fWidth4 = (float)m_eq_data.eq_mode[nIndex].eq_band[3].q / 512.0;
		m_fWidth4 = (float)((short)floorf(m_fWidth4 * 1000) / 1000.0);
		m_fWidth5 = (float)m_eq_data.eq_mode[nIndex].eq_band[4].q / 512.0;
		m_fWidth5 = (float)((short)floorf(m_fWidth5 * 1000) / 1000.0);
		m_fWidth6 = (float)m_eq_data.eq_mode[nIndex].eq_band[5].q / 512.0;
		m_fWidth6 = (float)((short)floorf(m_fWidth6 * 1000) / 1000.0);
		m_fWidth7 = (float)m_eq_data.eq_mode[nIndex].eq_band[6].q / 512.0;
		m_fWidth7 = (float)((short)floorf(m_fWidth7 * 1000) / 1000.0);
		m_fWidth8 = (float)m_eq_data.eq_mode[nIndex].eq_band[7].q / 512.0;
		m_fWidth8 = (float)((short)floorf(m_fWidth8 * 1000) / 1000.0);

		m_fGain1 = (float)m_eq_data.eq_mode[nIndex].eq_band[0].gaindB / 10.0;
		m_fGain1 = (float)((short)floorf(m_fGain1 * 10) / 10.0);
		m_fGain2 = (float)m_eq_data.eq_mode[nIndex].eq_band[1].gaindB / 10.0;
		m_fGain2 = (float)((short)floorf(m_fGain2 * 10) / 10.0);
		m_fGain3 = (float)m_eq_data.eq_mode[nIndex].eq_band[2].gaindB / 10.0;
		m_fGain3 = (float)((short)floorf(m_fGain3 * 10) / 10.0);
		m_fGain4 = (float)m_eq_data.eq_mode[nIndex].eq_band[3].gaindB / 10.0;
		m_fGain4 = (float)((short)floorf(m_fGain4 * 10) / 10.0);
		m_fGain5 = (float)m_eq_data.eq_mode[nIndex].eq_band[4].gaindB / 10.0;
		m_fGain5 = (float)((short)floorf(m_fGain5 * 10) / 10.0);
		m_fGain6 = (float)m_eq_data.eq_mode[nIndex].eq_band[5].gaindB / 10.0;
		m_fGain6 = (float)((short)floorf(m_fGain6 * 10) / 10.0);
		m_fGain7 = (float)m_eq_data.eq_mode[nIndex].eq_band[6].gaindB / 10.0;
		m_fGain7 = (float)((short)floorf(m_fGain7 * 10) / 10.0);
		m_fGain8 = (float)m_eq_data.eq_mode[nIndex].eq_band[7].gaindB / 10.0;
		m_fGain8 = (float)((short)floorf(m_fGain8 * 10) / 10.0);

		nValue = m_eq_data.eq_mode[nIndex].agc_in_gain;
		if(nValue == 0)
		{
			nValue = 1;
		}
		m_fMasterGain = (double)(20 * (double)log10((double)nValue/(double)4096));
		m_fMasterGain = (double)((short)floorf(m_fMasterGain * 10) / 10.0);
		m_fMasterGain = m_fMasterGain < -72 ? -72 : m_fMasterGain;
		m_fMasterGainBak = m_fMasterGain;


		m_nCBLowShelve = m_eq_data.eq_mode[nIndex].band_control & 0x1;
		m_nCBHighShelve = (m_eq_data.eq_mode[nIndex].band_control & 0x2) >> 1;
		
	}
	else
	{
		m_fRight9 = m_tun_data.eq_mode[nIndex].boostdB_current_band[0] * m_nLevelStep;
		m_fRight10 = m_tun_data.eq_mode[nIndex].boostdB_current_band[1] * m_nLevelStep;
		m_fRight11 = m_tun_data.eq_mode[nIndex].boostdB_current_band[2] * m_nLevelStep;
		m_fRight12 = m_tun_data.eq_mode[nIndex].boostdB_current_band[3] * m_nLevelStep;
		m_fRight13 = m_tun_data.eq_mode[nIndex].boostdB_current_band[4] * m_nLevelStep;
		m_fRight14 = m_tun_data.eq_mode[nIndex].boostdB_current_band[5] * m_nLevelStep;
		m_fRight15 = m_tun_data.eq_mode[nIndex].boostdB_current_band[6] * m_nLevelStep;
		m_fRight16 = m_tun_data.eq_mode[nIndex].boostdB_current_band[7] * m_nLevelStep;

		int nValue = m_tun_data.eq_mode[nIndex].agc_in_gain;
		if(nValue == 0)
		{
			nValue = 1;
		}
		m_fMasterGain = (double)(20 * (double)log10((double)nValue/(double)4096));
		m_fMasterGain = (double)((short)floorf(m_fMasterGain * 10) / 10.0);
		m_fMasterGain = m_fMasterGain < -72 ? -72 : m_fMasterGain;
		m_fMasterGainBak = m_fMasterGain;

		m_nCBLowShelve = m_tun_data.eq_mode[nIndex].band_control & 0x1;
		m_nCBHighShelve = (m_tun_data.eq_mode[nIndex].band_control & 0x2) >> 1;
	}
}

void CDlgEqualizerSet::ChangeState()
{
	if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
	{
		GetDlgItem(IDC_EDIT_LEVEL_STEP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_SMOOTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LEVEL_STEP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SMOOTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LEVEL_STEP_DB)->ShowWindow(SW_HIDE);
		m_nCBSmooth = -1;
		m_chk1.EnableWindow(TRUE);
		m_chk2.EnableWindow(TRUE);
		m_chk3.EnableWindow(TRUE);
		m_chk4.EnableWindow(TRUE);
		m_chk5.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LEVEL_N)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LEVEL_N)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_LOAD_FROM_DEFAULT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_SAVE_TO_DEFAULT)->ShowWindow(SW_HIDE);
		m_sctrlRight1.ShowWindow(SW_SHOW);
		m_sctrlRight2.ShowWindow(SW_SHOW);
		m_sctrlRight3.ShowWindow(SW_SHOW);
		m_sctrlRight4.ShowWindow(SW_SHOW);
		m_sctrlRight5.ShowWindow(SW_SHOW);
		m_sctrlRight6.ShowWindow(SW_SHOW);
		m_sctrlRight7.ShowWindow(SW_SHOW);
		m_sctrlRight8.ShowWindow(SW_SHOW);
		m_sctrlRight9.ShowWindow(SW_HIDE);
		m_sctrlRight10.ShowWindow(SW_HIDE);
		m_sctrlRight11.ShowWindow(SW_HIDE);
		m_sctrlRight12.ShowWindow(SW_HIDE);
		m_sctrlRight13.ShowWindow(SW_HIDE);
		m_sctrlRight14.ShowWindow(SW_HIDE);
		m_sctrlRight15.ShowWindow(SW_HIDE);
		m_sctrlRight16.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT9)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT10)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT11)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT12)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT13)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT14)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT15)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT16)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT12)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT14)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT15)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT16)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT8)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_EDIT_LEVEL_STEP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_SMOOTH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_LEVEL_STEP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SMOOTH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_LEVEL_STEP_DB)->ShowWindow(SW_SHOW);
		m_chk1.EnableWindow(FALSE);
		m_chk2.EnableWindow(FALSE);
		m_chk3.EnableWindow(FALSE);
		m_chk4.EnableWindow(FALSE);
		m_chk5.EnableWindow(FALSE);
		m_chk1.SetCheck(1);
		m_chk2.SetCheck(1);
		m_chk3.SetCheck(1);
		m_chk4.SetCheck(1);
		m_chk5.SetCheck(1);
		m_sctrlCentralFrequency1.EnableWindow(TRUE);
		m_sctrlCentralFrequency1.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight1.EnableWindow(TRUE);
		m_sctrlRight1.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT1)->EnableWindow(TRUE);

		m_sctrlCentralFrequency2.EnableWindow(TRUE);
		m_sctrlCentralFrequency2.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight2.EnableWindow(TRUE);
		m_sctrlRight2.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT2)->EnableWindow(TRUE);

		m_sctrlCentralFrequency3.EnableWindow(TRUE);
		m_sctrlCentralFrequency3.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight3.EnableWindow(TRUE);
		m_sctrlRight3.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT3)->EnableWindow(TRUE);

		m_sctrlCentralFrequency4.EnableWindow(TRUE);
		m_sctrlCentralFrequency4.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight4.EnableWindow(TRUE);
		m_sctrlRight4.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT4)->EnableWindow(TRUE);

		m_sctrlCentralFrequency5.EnableWindow(TRUE);
		m_sctrlCentralFrequency5.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight5.EnableWindow(TRUE);
		m_sctrlRight5.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT5)->EnableWindow(TRUE);

		m_sctrlCentralFrequency6.EnableWindow(TRUE);
		m_sctrlCentralFrequency6.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight6.EnableWindow(TRUE);
		m_sctrlRight6.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT6)->EnableWindow(TRUE);

		m_sctrlCentralFrequency7.EnableWindow(TRUE);
		m_sctrlCentralFrequency7.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight7.EnableWindow(TRUE);
		m_sctrlRight7.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY7)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH7)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN7)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT7)->EnableWindow(TRUE);

		m_sctrlCentralFrequency8.EnableWindow(TRUE);
		m_sctrlCentralFrequency8.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight8.EnableWindow(TRUE);
		m_sctrlRight8.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY8)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH8)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN8)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT8)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_LEVEL_N)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_LEVEL_N)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_BUTTON_LOAD_FROM_DEFAULT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_SAVE_TO_DEFAULT)->ShowWindow(SW_SHOW);

		m_sctrlRight1.ShowWindow(SW_HIDE);
		m_sctrlRight2.ShowWindow(SW_HIDE);
		m_sctrlRight3.ShowWindow(SW_HIDE);
		m_sctrlRight4.ShowWindow(SW_HIDE);
		m_sctrlRight5.ShowWindow(SW_HIDE);
		m_sctrlRight6.ShowWindow(SW_HIDE);
		m_sctrlRight7.ShowWindow(SW_HIDE);
		m_sctrlRight8.ShowWindow(SW_HIDE);
		m_sctrlRight9.ShowWindow(SW_SHOW);
		m_sctrlRight10.ShowWindow(SW_SHOW);
		m_sctrlRight11.ShowWindow(SW_SHOW);
		m_sctrlRight12.ShowWindow(SW_SHOW);
		m_sctrlRight13.ShowWindow(SW_SHOW);
		m_sctrlRight14.ShowWindow(SW_SHOW);
		m_sctrlRight15.ShowWindow(SW_SHOW);
		m_sctrlRight16.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT10)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT11)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT12)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT14)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT15)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT16)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT9)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT10)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT11)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT12)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT13)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT14)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT15)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT16)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT8)->ShowWindow(SW_HIDE);

	}
}

void CDlgEqualizerSet::ChangeSctrlState()
{
	if(m_nCBBandNum == 0)
	{
		m_sctrlRight6.ShowWindow(SW_HIDE);
		m_sctrlRight7.ShowWindow(SW_HIDE);
		m_sctrlRight8.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_BAND6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_BAND7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_BAND8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BD6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BD7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BD8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC8)->ShowWindow(SW_HIDE);
		m_chk6.ShowWindow(SW_HIDE);
		m_chk7.ShowWindow(SW_HIDE);
		m_chk8.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CK6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CK7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CK8)->ShowWindow(SW_HIDE);
		m_sctrlCentralFrequency6.ShowWindow(SW_HIDE);
		m_sctrlCentralFrequency7.ShowWindow(SW_HIDE);
		m_sctrlCentralFrequency8.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_WIDTH6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_WIDTH7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_WIDTH8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FQ6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FQ7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FQ8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_Q6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_Q7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_Q8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_G6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_G7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_G8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DB1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DB2)->ShowWindow(SW_HIDE);
		m_sctrlRight14.ShowWindow(SW_HIDE);
		m_sctrlRight15.ShowWindow(SW_HIDE);
		m_sctrlRight16.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT14)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT15)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RIGHT16)->ShowWindow(SW_HIDE);
	}
	else if(m_nCBBandNum == 1)
	{
		m_sctrlRight6.ShowWindow(SW_SHOW);
		m_sctrlRight7.ShowWindow(SW_SHOW);
		m_sctrlRight8.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STC_BAND6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STC_BAND7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STC_BAND8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BD6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BD7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BD8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC8)->ShowWindow(SW_SHOW);
		m_chk6.ShowWindow(SW_SHOW);
		m_chk7.ShowWindow(SW_SHOW);
		m_chk8.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_CK6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_CK7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_CK8)->ShowWindow(SW_SHOW);
		m_sctrlCentralFrequency6.ShowWindow(SW_SHOW);
		m_sctrlCentralFrequency7.ShowWindow(SW_SHOW);
		m_sctrlCentralFrequency8.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_WIDTH6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_WIDTH7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_WIDTH8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GAIN6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GAIN7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GAIN8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FQ6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FQ7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FQ8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_Q6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_Q7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_Q8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_G6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_G7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_G8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DB1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DB2)->ShowWindow(SW_SHOW);
		m_sctrlRight14.ShowWindow(SW_SHOW);
		m_sctrlRight15.ShowWindow(SW_SHOW);
		m_sctrlRight16.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT14)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT15)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RIGHT16)->ShowWindow(SW_SHOW);
	}
}

void CDlgEqualizerSet::ChangeEQValue(int nIndex)
{
	if(m_nCBEqType == 0  || m_lstPresets.GetCurSel() == 0)
	{
		m_eq_data.eq_mode[nIndex].eq_band[0].boostdB = ceilf(m_fRight1 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[1].boostdB = ceilf(m_fRight2 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[2].boostdB = ceilf(m_fRight3 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[3].boostdB = ceilf(m_fRight4 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[4].boostdB = ceilf(m_fRight5 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[5].boostdB = ceilf(m_fRight6 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[6].boostdB = ceilf(m_fRight7 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[7].boostdB = ceilf(m_fRight8 * 10.0 );

		int nValue = m_chk1.GetCheck();

		int n = m_eq_data.eq_mode[nIndex].band_control;
		n = (n & 0x7fff) | (nValue << 15);

		nValue = m_chk2.GetCheck();
		n = (n & 0xbfff) | (nValue << 14);

		nValue = m_chk3.GetCheck();
		n = (n & 0xdfff) | (nValue << 13);

		nValue = m_chk4.GetCheck();
		n = (n & 0xefff) | (nValue << 12);

		nValue = m_chk5.GetCheck();
		n = (n & 0xf7ff) | (nValue << 11);

		m_eq_data.eq_mode[nIndex].band_control = n;

		m_eq_data.eq_mode[nIndex].eq_band[0].f0 = m_fCentralFrequency1;
		m_eq_data.eq_mode[nIndex].eq_band[1].f0 = m_fCentralFrequency2;
		m_eq_data.eq_mode[nIndex].eq_band[2].f0 = m_fCentralFrequency3;
		m_eq_data.eq_mode[nIndex].eq_band[3].f0 = m_fCentralFrequency4;
		m_eq_data.eq_mode[nIndex].eq_band[4].f0 = m_fCentralFrequency5;
		m_eq_data.eq_mode[nIndex].eq_band[5].f0 = m_fCentralFrequency6;
		m_eq_data.eq_mode[nIndex].eq_band[6].f0 = m_fCentralFrequency7;
		m_eq_data.eq_mode[nIndex].eq_band[7].f0 = m_fCentralFrequency8;

		m_eq_data.eq_mode[nIndex].eq_band[0].q = ceilf(m_fWidth1 * 512.0);
		m_eq_data.eq_mode[nIndex].eq_band[1].q = ceilf(m_fWidth2 * 512.0);
		m_eq_data.eq_mode[nIndex].eq_band[2].q = ceilf(m_fWidth3 * 512.0);
		m_eq_data.eq_mode[nIndex].eq_band[3].q = ceilf(m_fWidth4 * 512.0);
		m_eq_data.eq_mode[nIndex].eq_band[4].q = ceilf(m_fWidth5 * 512.0);
		m_eq_data.eq_mode[nIndex].eq_band[5].q = ceilf(m_fWidth6 * 512.0);
		m_eq_data.eq_mode[nIndex].eq_band[6].q = ceilf(m_fWidth7 * 512.0);
		m_eq_data.eq_mode[nIndex].eq_band[7].q = ceilf(m_fWidth8 * 512.0);

		m_eq_data.eq_mode[nIndex].eq_band[0].gaindB = ceilf(m_fGain1 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[1].gaindB = ceilf(m_fGain2 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[2].gaindB = ceilf(m_fGain3 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[3].gaindB = ceilf(m_fGain4 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[4].gaindB = ceilf(m_fGain5 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[5].gaindB = ceilf(m_fGain6 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[6].gaindB = ceilf(m_fGain7 * 10.0 );
		m_eq_data.eq_mode[nIndex].eq_band[7].gaindB = ceilf(m_fGain8 * 10.0 );

// 		if(m_fMasterGain > 18)
// 		{
// 			((CEdit *)GetDlgItem(IDC_EDIT_MATSER_GAIN))->SetFocus();
// 			((CEdit *)GetDlgItem(IDC_EDIT_MATSER_GAIN))->SetSel(0,-1);
// 			AfxMessageBox(_T("Please input a right value!"));
// 			return;
// 		}
		if(m_fMasterGain != m_fMasterGainBak)
		{
			m_eq_data.eq_mode[nIndex].agc_in_gain = ceilf(((double)pow(10, m_fMasterGain/20.0)) * 4096.0);
		}

		n = m_eq_data.eq_mode[nIndex].band_control;
		n = (n & 0xfffe) | m_nCBLowShelve;
		n = (n & 0xfffd) | (m_nCBHighShelve << 1);
		m_eq_data.eq_mode[nIndex].band_control = n;
	}
	else
	{
		m_tun_data.eq_mode[nIndex].boostdB_current_band[0] = m_fRight9 / m_nLevelStep;
		m_tun_data.eq_mode[nIndex].boostdB_current_band[1] = m_fRight10 / m_nLevelStep;
		m_tun_data.eq_mode[nIndex].boostdB_current_band[2] = m_fRight11 / m_nLevelStep;
		m_tun_data.eq_mode[nIndex].boostdB_current_band[3] = m_fRight12 / m_nLevelStep;
		m_tun_data.eq_mode[nIndex].boostdB_current_band[4] = m_fRight13 / m_nLevelStep;
		m_tun_data.eq_mode[nIndex].boostdB_current_band[5] = m_fRight14 / m_nLevelStep;
		m_tun_data.eq_mode[nIndex].boostdB_current_band[6] = m_fRight15 / m_nLevelStep;
		m_tun_data.eq_mode[nIndex].boostdB_current_band[7] = m_fRight16 / m_nLevelStep;
		
// 		if(m_fMasterGain > 18)
// 		{
// 			((CEdit *)GetDlgItem(IDC_EDIT_MATSER_GAIN))->SetFocus();
// 			((CEdit *)GetDlgItem(IDC_EDIT_MATSER_GAIN))->SetSel(0,-1);
// 			AfxMessageBox(_T("Please input a right value!"));
// 			return;
// 		}
		if(m_fMasterGain != m_fMasterGainBak)
		{
			m_tun_data.eq_mode[nIndex].agc_in_gain = ceilf(((double)pow(10, m_fMasterGain/20.0)) * 4096.0);
		}

		int n = m_tun_data.eq_mode[nIndex].band_control;
		n = (n & 0xfffe) | m_nCBLowShelve;
		n = (n & 0xfffd) | (m_nCBHighShelve << 1);
		m_tun_data.eq_mode[nIndex].band_control = n;
	}
}

void CDlgEqualizerSet::InitSLider()
{
	if(!UpdateData())
	{
		return;
	}
	if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
	{
		m_sctrlLeft.SetBuddy( GetDlgItem( IDC_EDIT_LEFT ));
		m_sctrlLeft.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
		m_sctrlLeft.SetPos(m_fLeft, STEP_SLIDER_V);
		m_sctrlRight.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT ));
		m_sctrlRight.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
		m_sctrlRight.SetPos(m_fRight, STEP_SLIDER_V);
		
		m_sctrlRight1.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT1 ));
		m_sctrlRight1.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
		m_sctrlRight1.SetPos(m_fRight1, STEP_SLIDER_V);
		m_sctrlRight2.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT2 ));
		m_sctrlRight2.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
		m_sctrlRight2.SetPos(m_fRight2, STEP_SLIDER_V);
		m_sctrlRight3.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT3 ));
		m_sctrlRight3.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
		m_sctrlRight3.SetPos(m_fRight3, STEP_SLIDER_V);
		m_sctrlRight4.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT4 ));
		m_sctrlRight4.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
		m_sctrlRight4.SetPos(m_fRight4, STEP_SLIDER_V);
		m_sctrlRight5.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT5 ));
		m_sctrlRight5.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
		m_sctrlRight5.SetPos(m_fRight5, STEP_SLIDER_V);
		m_sctrlRight6.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT6 ));
		m_sctrlRight6.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
		m_sctrlRight6.SetPos(m_fRight6, STEP_SLIDER_V);
		m_sctrlRight7.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT7 ));
		m_sctrlRight7.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
		m_sctrlRight7.SetPos(m_fRight7, STEP_SLIDER_V);
		m_sctrlRight8.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT8 ));
		m_sctrlRight8.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
		m_sctrlRight8.SetPos(m_fRight8, STEP_SLIDER_V);
	}
	else
	{
		m_sctrlLeft.SetBuddy( GetDlgItem( IDC_EDIT_LEFT ));
		m_sctrlLeft.SetRange(0,m_nLevelN*2);
		m_sctrlLeft.SetPos(m_fLeft, m_nLevelN, 2);
		m_sctrlRight.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT ));
		m_sctrlRight.SetRange(0,m_nLevelN*2);
		m_sctrlRight.SetPos(m_fRight, m_nLevelN, 2);
		
		m_sctrlRight9.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT9 ));
		m_sctrlRight9.SetRange(0,m_nLevelN*2);
		m_sctrlRight9.SetPos(m_fRight9/m_nLevelStep, m_nLevelN, 2, m_nLevelStep);
		m_sctrlRight10.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT10 ));
		m_sctrlRight10.SetRange(0,m_nLevelN*2);
		m_sctrlRight10.SetPos(m_fRight10/m_nLevelStep, m_nLevelN, 2, m_nLevelStep);
		m_sctrlRight11.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT11 ));
		m_sctrlRight11.SetRange(0,m_nLevelN*2);
		m_sctrlRight11.SetPos(m_fRight11/m_nLevelStep, m_nLevelN, 2, m_nLevelStep);
		m_sctrlRight12.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT12 ));
		m_sctrlRight12.SetRange(0,m_nLevelN*2);
		m_sctrlRight12.SetPos(m_fRight12/m_nLevelStep, m_nLevelN, 2, m_nLevelStep);
		m_sctrlRight13.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT13 ));
		m_sctrlRight13.SetRange(0,m_nLevelN*2);
		m_sctrlRight13.SetPos(m_fRight13/m_nLevelStep, m_nLevelN, 2, m_nLevelStep);
		m_sctrlRight14.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT14 ));
		m_sctrlRight14.SetRange(0,m_nLevelN*2);
		m_sctrlRight14.SetPos(m_fRight14/m_nLevelStep, m_nLevelN, 2, m_nLevelStep);
		m_sctrlRight15.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT15 ));
		m_sctrlRight15.SetRange(0,m_nLevelN*2);
		m_sctrlRight15.SetPos(m_fRight15/m_nLevelStep, m_nLevelN, 2, m_nLevelStep);
		m_sctrlRight16.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT16 ));
		m_sctrlRight16.SetRange(0,m_nLevelN*2);
		m_sctrlRight16.SetPos(m_fRight16/m_nLevelStep, m_nLevelN, 2, m_nLevelStep);
		m_bSetTunBuddy = TRUE;
	}
	
	m_sctrlLowShelfCutoff.SetBuddy( GetDlgItem( IDC_EDIT_LOW_SHELF_COTOFF ) );
	m_sctrlLowShelfCutoff.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_sctrlLowShelfCutoff.SetPos(m_fLowShelfCutoff,0,1);
	m_sctrlHighShelfCutoff.SetBuddy( GetDlgItem( IDC_EDIT_HIGH_SHELF_COTOFF ) );
	m_sctrlHighShelfCutoff.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_sctrlHighShelfCutoff.SetPos(m_fHighShelfCutoff,0,1);
	
	m_sctrlCentralFrequency1.SetBuddy( GetDlgItem( IDC_EDIT_CENTRAL_FREQUENCY1 ) );
	m_sctrlCentralFrequency1.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_sctrlCentralFrequency1.SetPos(m_fCentralFrequency1,0,1);
	m_sctrlCentralFrequency2.SetBuddy( GetDlgItem( IDC_EDIT_CENTRAL_FREQUENCY2 ) );
	m_sctrlCentralFrequency2.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_sctrlCentralFrequency2.SetPos(m_fCentralFrequency2,0,1);
	m_sctrlCentralFrequency3.SetBuddy( GetDlgItem( IDC_EDIT_CENTRAL_FREQUENCY3 ) );
	m_sctrlCentralFrequency3.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_sctrlCentralFrequency3.SetPos(m_fCentralFrequency3,0,1);
	m_sctrlCentralFrequency4.SetBuddy( GetDlgItem( IDC_EDIT_CENTRAL_FREQUENCY4 ) );
	m_sctrlCentralFrequency4.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_sctrlCentralFrequency4.SetPos(m_fCentralFrequency4,0,1);
	m_sctrlCentralFrequency5.SetBuddy( GetDlgItem( IDC_EDIT_CENTRAL_FREQUENCY5 ) );
	m_sctrlCentralFrequency5.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_sctrlCentralFrequency5.SetPos(m_fCentralFrequency5,0,1);
	m_sctrlCentralFrequency6.SetBuddy( GetDlgItem( IDC_EDIT_CENTRAL_FREQUENCY6 ) );
	m_sctrlCentralFrequency6.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_sctrlCentralFrequency6.SetPos(m_fCentralFrequency6,0,1);
	m_sctrlCentralFrequency7.SetBuddy( GetDlgItem( IDC_EDIT_CENTRAL_FREQUENCY7 ) );
	m_sctrlCentralFrequency7.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_sctrlCentralFrequency7.SetPos(m_fCentralFrequency7,0,1);
	m_sctrlCentralFrequency8.SetBuddy( GetDlgItem( IDC_EDIT_CENTRAL_FREQUENCY8 ) );
	m_sctrlCentralFrequency8.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_sctrlCentralFrequency8.SetPos(m_fCentralFrequency8,0,1);

	m_sctrlLeft.SetPrimaryColor(m_crBack);
	m_sctrlRight.SetPrimaryColor(m_crBack);
	m_sctrlLowShelfCutoff.SetPrimaryColor(m_crBack);
	m_sctrlHighShelfCutoff.SetPrimaryColor(m_crBack);
	
}

void CDlgEqualizerSet::SetSliderPos()
{
	if(!UpdateData())
	{
		return;
	}

	if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
	{
		m_sctrlLeft.SetPos(m_fLeft, STEP_SLIDER_V,0,0,FALSE);
		m_sctrlRight.SetPos(m_fRight, STEP_SLIDER_V,0,0,FALSE);
		m_sctrlRight1.SetPos(m_fRight1, STEP_SLIDER_V,0,0,FALSE);
		m_sctrlRight2.SetPos(m_fRight2, STEP_SLIDER_V,0,0,FALSE);
		m_sctrlRight3.SetPos(m_fRight3, STEP_SLIDER_V,0,0,FALSE);
		m_sctrlRight4.SetPos(m_fRight4, STEP_SLIDER_V,0,0,FALSE);
		m_sctrlRight5.SetPos(m_fRight5, STEP_SLIDER_V,0,0,FALSE);
		m_sctrlRight6.SetPos(m_fRight6, STEP_SLIDER_V,0,0,FALSE);
		m_sctrlRight7.SetPos(m_fRight7, STEP_SLIDER_V,0,0,FALSE);
		m_sctrlRight8.SetPos(m_fRight8, STEP_SLIDER_V,0,0,FALSE);
	}
	else
	{	
		if(!m_bSetTunBuddy)
		{	
			m_sctrlRight9.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT9 ));
			m_sctrlRight10.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT10 ));
			m_sctrlRight11.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT11 ));
			m_sctrlRight12.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT12 ));
			m_sctrlRight13.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT13 ));
			if(m_nCBBandNum == 1)
			{
				m_sctrlRight14.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT14 ));
				m_sctrlRight15.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT15 ));
				m_sctrlRight16.SetBuddy( GetDlgItem( IDC_EDIT_RIGHT16 ));
			}
			m_bSetTunBuddy = TRUE;
		}
		m_sctrlRight9.SetRange(0,m_nLevelN*2, TRUE);
		m_sctrlRight9.SetPos(m_fRight9/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
		m_sctrlRight10.SetRange(0,m_nLevelN*2,TRUE);
		m_sctrlRight10.SetPos(m_fRight10/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
		m_sctrlRight11.SetRange(0,m_nLevelN*2, TRUE);
		m_sctrlRight11.SetPos(m_fRight11/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
		m_sctrlRight12.SetRange(0,m_nLevelN*2, TRUE);
		m_sctrlRight12.SetPos(m_fRight12/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
		m_sctrlRight13.SetRange(0,m_nLevelN*2, TRUE);
		m_sctrlRight13.SetPos(m_fRight13/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
		m_sctrlRight14.SetRange(0,m_nLevelN*2, TRUE);
		m_sctrlRight14.SetPos(m_fRight14/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
		m_sctrlRight15.SetRange(0,m_nLevelN*2, TRUE);
		m_sctrlRight15.SetPos(m_fRight15/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
		m_sctrlRight16.SetRange(0,m_nLevelN*2, TRUE);
		m_sctrlRight16.SetPos(m_fRight16/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
	}
	
	m_sctrlLowShelfCutoff.SetPos(m_fLowShelfCutoff,0,1,0,FALSE);
	m_sctrlHighShelfCutoff.SetPos(m_fHighShelfCutoff,0,1,0,FALSE);
	m_sctrlCentralFrequency1.SetPos(m_fCentralFrequency1,0,1,0,FALSE);
	m_sctrlCentralFrequency2.SetPos(m_fCentralFrequency2,0,1,0,FALSE);
	m_sctrlCentralFrequency3.SetPos(m_fCentralFrequency3,0,1,0,FALSE);
	m_sctrlCentralFrequency4.SetPos(m_fCentralFrequency4,0,1,0,FALSE);
	m_sctrlCentralFrequency5.SetPos(m_fCentralFrequency5,0,1,0,FALSE);
	m_sctrlCentralFrequency6.SetPos(m_fCentralFrequency6,0,1,0,FALSE);
	m_sctrlCentralFrequency7.SetPos(m_fCentralFrequency7,0,1,0,FALSE);
	m_sctrlCentralFrequency8.SetPos(m_fCentralFrequency8,0,1,0,FALSE);
}

void CDlgEqualizerSet::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CBuddiedSliderCtrl* pSlider = (CBuddiedSliderCtrl*)pScrollBar;
	pSlider->ReflectedScrollMessage(0,1);
	ReflashGraph();
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgEqualizerSet::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

 	CBuddiedSliderCtrl* pSlider = (CBuddiedSliderCtrl*)pScrollBar;
	if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
	{
		pSlider->ReflectedScrollMessage(STEP_SLIDER_V);
	}
	else
	{
		pSlider->ReflectedScrollMessage(m_nLevelN, 2, m_nLevelStep);
		CString strtext;
		pSlider->m_pBuddyWnd->GetWindowText(strtext);
		int nValue = 0;
#ifdef _UNICODE
		char szTmp[16] = {0};
		WideCharToMultiByte(CP_ACP,0,strtext,-1,szTmp,16,NULL,NULL);
		nValue = atoi(szTmp);
#else
		nValue= atoi( (LPSTR)(LPCTSTR)strtext );   
#endif  
		if(nValue < -72 || nValue > 18)
		{
			//pSlider->SetPrimaryColor(RGB(255,0,0));
			if(nValue < -72)
			{
				nValue = -72;
			}
			if(nValue > 18)
			{
				nValue = 18;
			}

			pSlider->SetPos(nValue/m_nLevelStep, m_nLevelN,2, m_nLevelStep,FALSE);
			AfxMessageBox(_T("Please adjust the slider or LevelN/Level Step to insure the value in \[-72,18\]"));
			CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
			return;
		}
		else
		{
			//pSlider->SetPrimaryColor(RGB(128,128,128));
		}
	}
	
	ReflashGraph();
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDlgEqualizerSet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(pMsg->wParam == VK_RETURN) 
	{
		if(!UpdateData())
		{
			return FALSE;
		}

		if(pMsg->hwnd == GetDlgItem(IDC_EDIT_LEFT)->GetSafeHwnd())
		{
			m_sctrlLeft.SetPos(m_fLeft, STEP_SLIDER_V);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_RIGHT)->GetSafeHwnd())
		{
			m_sctrlRight.SetPos(m_fRight, STEP_SLIDER_V);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_RIGHT1)->GetSafeHwnd())
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_sctrlRight1.SetPos(m_fRight1, STEP_SLIDER_V);
			}
			else
			{
				m_sctrlRight1.SetPos(m_fRight9/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
			}
			
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_RIGHT2)->GetSafeHwnd())
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_sctrlRight2.SetPos(m_fRight2, STEP_SLIDER_V);
			}
			else
			{
				m_sctrlRight2.SetPos(m_fRight10/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
			}
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_RIGHT3)->GetSafeHwnd())
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_sctrlRight3.SetPos(m_fRight3, STEP_SLIDER_V);
			}
			else
			{
				m_sctrlRight3.SetPos(m_fRight11/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
			}
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_RIGHT4)->GetSafeHwnd())
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_sctrlRight4.SetPos(m_fRight4, STEP_SLIDER_V);
			}
			else
			{
				m_sctrlRight4.SetPos(m_fRight12/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
			}
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_RIGHT5)->GetSafeHwnd())
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_sctrlRight5.SetPos(m_fRight5, STEP_SLIDER_V);
			}
			else
			{
				m_sctrlRight5.SetPos(m_fRight13/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
			}
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_RIGHT6)->GetSafeHwnd())
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_sctrlRight6.SetPos(m_fRight6, STEP_SLIDER_V);
			}
			else
			{
				m_sctrlRight6.SetPos(m_fRight14/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
			}
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_RIGHT7)->GetSafeHwnd())
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_sctrlRight7.SetPos(m_fRight7, STEP_SLIDER_V);
			}
			else
			{
				m_sctrlRight7.SetPos(m_fRight15/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
			}
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_RIGHT8)->GetSafeHwnd())
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_sctrlRight8.SetPos(m_fRight8, STEP_SLIDER_V);
			}
			else
			{
				m_sctrlRight8.SetPos(m_fRight16/m_nLevelStep, m_nLevelN,2, m_nLevelStep);
			}
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_LOW_SHELF_COTOFF)->GetSafeHwnd())
		{
			m_sctrlLowShelfCutoff.SetPos(m_fLowShelfCutoff,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_HIGH_SHELF_COTOFF)->GetSafeHwnd())
		{
			m_sctrlHighShelfCutoff.SetPos(m_fHighShelfCutoff,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY1)->GetSafeHwnd())
		{
			m_sctrlCentralFrequency1.SetPos(m_fCentralFrequency1,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY2)->GetSafeHwnd())
		{
			m_sctrlCentralFrequency2.SetPos(m_fCentralFrequency2,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY3)->GetSafeHwnd())
		{
			m_sctrlCentralFrequency3.SetPos(m_fCentralFrequency3,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY4)->GetSafeHwnd())
		{
			m_sctrlCentralFrequency4.SetPos(m_fCentralFrequency4,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY5)->GetSafeHwnd())
		{
			m_sctrlCentralFrequency5.SetPos(m_fCentralFrequency5,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->GetSafeHwnd())
		{
			m_sctrlCentralFrequency6.SetPos(m_fCentralFrequency6,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY7)->GetSafeHwnd())
		{
			m_sctrlCentralFrequency7.SetPos(m_fCentralFrequency7,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY8)->GetSafeHwnd())
		{
			m_sctrlCentralFrequency8.SetPos(m_fCentralFrequency8,0,1);
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgEqualizerSet::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	if(m_chk1.GetCheck() == 1)
	{
		m_sctrlCentralFrequency1.EnableWindow(TRUE);
		m_sctrlCentralFrequency1.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight1.EnableWindow(TRUE);
		m_sctrlRight1.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT1)->EnableWindow(TRUE);
	}
	else
	{
		m_sctrlCentralFrequency1.EnableWindow(FALSE);
		m_sctrlCentralFrequency1.SetPrimaryColor(m_crBack);
		m_sctrlRight1.EnableWindow(FALSE);
		m_sctrlRight1.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT1)->EnableWindow(FALSE);
	}
	ReflashGraph();
}

void CDlgEqualizerSet::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	if(m_chk2.GetCheck() == 1)
	{
		m_sctrlCentralFrequency2.EnableWindow(TRUE);
		m_sctrlCentralFrequency2.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight2.EnableWindow(TRUE);
		m_sctrlRight2.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT2)->EnableWindow(TRUE);
	}
	else
	{
		m_sctrlCentralFrequency2.EnableWindow(FALSE);
		m_sctrlCentralFrequency2.SetPrimaryColor(m_crBack);
		m_sctrlRight2.EnableWindow(FALSE);
		m_sctrlRight2.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT2)->EnableWindow(FALSE);
	}
	ReflashGraph();
}

void CDlgEqualizerSet::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	if(m_chk3.GetCheck() == 1)
	{
		m_sctrlCentralFrequency3.EnableWindow(TRUE);
		m_sctrlCentralFrequency3.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight3.EnableWindow(TRUE);
		m_sctrlRight3.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT3)->EnableWindow(TRUE);
	}
	else
	{
		m_sctrlCentralFrequency3.EnableWindow(FALSE);
		m_sctrlCentralFrequency3.SetPrimaryColor(m_crBack);
		m_sctrlRight3.EnableWindow(FALSE);
		m_sctrlRight3.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT3)->EnableWindow(FALSE);
	}
	ReflashGraph();
}

void CDlgEqualizerSet::OnCheck4() 
{
	// TODO: Add your control notification handler code here
	if(m_chk4.GetCheck() == 1)
	{
		m_sctrlCentralFrequency4.EnableWindow(TRUE);
		m_sctrlCentralFrequency4.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight4.EnableWindow(TRUE);
		m_sctrlRight4.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT4)->EnableWindow(TRUE);
	}
	else
	{
		m_sctrlCentralFrequency4.EnableWindow(FALSE);
		m_sctrlCentralFrequency4.SetPrimaryColor(m_crBack);
		m_sctrlRight4.EnableWindow(FALSE);
		m_sctrlRight4.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT4)->EnableWindow(FALSE);
	}
	ReflashGraph();
}

void CDlgEqualizerSet::OnCheck5() 
{
	// TODO: Add your control notification handler code here
	if(m_chk5.GetCheck() == 1)
	{
		m_sctrlCentralFrequency5.EnableWindow(TRUE);
		m_sctrlCentralFrequency5.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight5.EnableWindow(TRUE);
		m_sctrlRight5.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT5)->EnableWindow(TRUE);
	}
	else
	{
		m_sctrlCentralFrequency5.EnableWindow(FALSE);
		m_sctrlCentralFrequency5.SetPrimaryColor(m_crBack);
		m_sctrlRight5.EnableWindow(FALSE);
		m_sctrlRight5.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT5)->EnableWindow(FALSE);
	}
	ReflashGraph();
}

void CDlgEqualizerSet::OnCheck6() 
{
	// TODO: Add your control notification handler code here
	if(m_chk6.GetCheck() == 1)
	{
		m_sctrlCentralFrequency6.EnableWindow(TRUE);
		m_sctrlCentralFrequency6.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight6.EnableWindow(TRUE);
		m_sctrlRight6.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT6)->EnableWindow(TRUE);
	}
	else
	{
		m_sctrlCentralFrequency6.EnableWindow(FALSE);
		m_sctrlCentralFrequency6.SetPrimaryColor(m_crBack);
		m_sctrlRight6.EnableWindow(FALSE);
		m_sctrlRight6.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT6)->EnableWindow(FALSE);
	}
	ReflashGraph();
}

void CDlgEqualizerSet::OnCheck7() 
{
	// TODO: Add your control notification handler code here
	if(m_chk7.GetCheck() == 1)
	{
		m_sctrlCentralFrequency7.EnableWindow(TRUE);
		m_sctrlCentralFrequency7.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight7.EnableWindow(TRUE);
		m_sctrlRight7.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY7)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH7)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN7)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT7)->EnableWindow(TRUE);
	}
	else
	{
		m_sctrlCentralFrequency7.EnableWindow(FALSE);
		m_sctrlCentralFrequency7.SetPrimaryColor(m_crBack);
		m_sctrlRight7.EnableWindow(FALSE);
		m_sctrlRight7.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY7)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH7)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN7)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT7)->EnableWindow(FALSE);
	}
	ReflashGraph();
}

void CDlgEqualizerSet::OnCheck8() 
{
	// TODO: Add your control notification handler code here
	if(m_chk8.GetCheck() == 1)
	{
		m_sctrlCentralFrequency8.EnableWindow(TRUE);
		m_sctrlCentralFrequency8.SetPrimaryColor(RGB(128,128,128));
		m_sctrlRight8.EnableWindow(TRUE);
		m_sctrlRight8.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY8)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH8)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN8)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT8)->EnableWindow(TRUE);
	}
	else
	{
		m_sctrlCentralFrequency8.EnableWindow(FALSE);
		m_sctrlCentralFrequency8.SetPrimaryColor(m_crBack);
		m_sctrlRight8.EnableWindow(FALSE);
		m_sctrlRight8.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_CENTRAL_FREQUENCY8)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH8)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN8)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT8)->EnableWindow(FALSE);
	}
	ReflashGraph();
}

BOOL CDlgEqualizerSet::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
			case ID_BUTTON_EQ_G_WRITE_CURRENT_MODE_TO_PHONE:
				strTip.LoadString(IDS_WRITE_EQ_MS);
				break;
			case ID_BUTTON_EQ_G_WRITE_CURRENT_MODE_TO_FLASH:
				strTip.LoadString(IDS_WRITE_EQ_FLASH);
				break;
			case ID_BUTTON_EQ_G_LOAD_CURRENT_MODE_FROM_PHONE:
				strTip.LoadString(IDS_LOAD_EQ_MS);
				break;
			case ID_BUTTON_EQ_G_LOAD_CURRENT_MODE_FROM_FLASH:
				strTip.LoadString(IDS_LOAD_EQ_FLASH);
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
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CDlgEqualizerSet::ReflashGraph()
{	
	if(!UpdateData())
	{
		return;
	}
	int nBandNum = 0;
	if(m_nCBBandNum == 0)
	{
		nBandNum = 5;
	}
	else
	{
		nBandNum = 8;
	}

	m_stcGraph.SetBandNum(nBandNum);
	m_stcGraph.SetMasterGain(m_fMasterGain);
	m_stcGraph.SetSampleRate(m_nSampleRate);
	
	for(int i =0; i < nBandNum; i++)
	{
		if(i == 0)
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_stcGraph.SetValue(i,m_chk1.GetCheck(), m_fCentralFrequency1, m_fWidth1, m_fRight1, m_fGain1);
			}
			else
			{
				m_stcGraph.SetValue(i,m_chk1.GetCheck(), m_fCentralFrequency1, m_fWidth1, m_fRight9, m_fGain1);
			}
			
		}
		else if(i == 1)
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_stcGraph.SetValue(i,m_chk2.GetCheck(), m_fCentralFrequency2, m_fWidth2, m_fRight2, m_fGain2);
			}
			else
			{
				m_stcGraph.SetValue(i,m_chk2.GetCheck(), m_fCentralFrequency2, m_fWidth2, m_fRight10, m_fGain2);
			}
		}
		else if(i == 2)
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_stcGraph.SetValue(i,m_chk3.GetCheck(), m_fCentralFrequency3, m_fWidth3, m_fRight3, m_fGain3);
			}
			else
			{
				m_stcGraph.SetValue(i,m_chk3.GetCheck(), m_fCentralFrequency3, m_fWidth3, m_fRight11, m_fGain3);
			}
		}
		else if(i == 3)
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_stcGraph.SetValue(i,m_chk4.GetCheck(), m_fCentralFrequency4, m_fWidth4, m_fRight4, m_fGain4);
			}
			else
			{
				m_stcGraph.SetValue(i,m_chk4.GetCheck(), m_fCentralFrequency4, m_fWidth4, m_fRight12, m_fGain4);
			}
		}
		else if(i == 4)
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_stcGraph.SetValue(i,m_chk5.GetCheck(), m_fCentralFrequency5, m_fWidth5, m_fRight5, m_fGain5);
			}
			else
			{
				m_stcGraph.SetValue(i,m_chk5.GetCheck(), m_fCentralFrequency5, m_fWidth5, m_fRight13, m_fGain5);
			}
		}
		else if(i == 5)
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_stcGraph.SetValue(i,m_chk6.GetCheck(), m_fCentralFrequency6, m_fWidth6, m_fRight6, m_fGain6);
			}
			else
			{
				m_stcGraph.SetValue(i,m_chk6.GetCheck(), m_fCentralFrequency6, m_fWidth6, m_fRight14, m_fGain6);
			}
		}
		else if(i == 6)
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_stcGraph.SetValue(i,m_chk7.GetCheck(), m_fCentralFrequency7, m_fWidth7, m_fRight7, m_fGain7);
			}
			else
			{
				m_stcGraph.SetValue(i,m_chk7.GetCheck(), m_fCentralFrequency7, m_fWidth7, m_fRight15, m_fGain7);
			}
		}
		else if(i == 7)
		{
			if(m_nCBEqType == 0 || m_lstPresets.GetCurSel() == 0)
			{
				m_stcGraph.SetValue(i,m_chk8.GetCheck(), m_fCentralFrequency8, m_fWidth8, m_fRight8, m_fGain8);
			}
			else
			{
				m_stcGraph.SetValue(i,m_chk8.GetCheck(), m_fCentralFrequency8, m_fWidth8, m_fRight16, m_fGain8);
			}
		}
	}
	//m_stcGraph.EraseBK();
	m_stcGraph.Invalidate(FALSE);
	
}

void CDlgEqualizerSet::OnBtnEqReflash() 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
}

HBRUSH CDlgEqualizerSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( pWnd->GetDlgCtrlID() == IDC_STC_BAND1 )
    {
        return CreateSolidBrush( RGB( 255,255,0 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STC_BAND2 )
    {
        return CreateSolidBrush( RGB( 255,0,0 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STC_BAND3 )
    {
        return CreateSolidBrush( RGB( 128,0,255 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STC_BAND4 )
    {
        return CreateSolidBrush( RGB( 0,255,255 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STC_BAND5 )
    {
        return CreateSolidBrush( RGB( 0,0,255 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STC_BAND6 )
    {
        return CreateSolidBrush( RGB( 128,64,64 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STC_BAND7 )
    {
        return CreateSolidBrush( RGB( 0,255,0 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STC_BAND8 )
    {
        return CreateSolidBrush( RGB( 255,128,255 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STC_RESULT )
	{
		return CreateSolidBrush( RGB( 0,255,0 ) );
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderRight1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderRight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderRight2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderRight3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderRight4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderRight5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderRight6(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderRight7(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderRight8(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderLowShelfCutoff(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderLeft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderHighShelfCutoff(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderCentralFrequency8(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderCentralFrequency7(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderCentralFrequency6(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderCentralFrequency5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderCentralFrequency4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderCentralFrequency3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderCentralFrequency2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnReleasedcaptureSliderCentralFrequency1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgEqualizerSet::OnChangeEditRight1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_RIGHT1)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlRight1.SetPos(m_fRight1, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditRight() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_RIGHT)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlRight.SetPos(m_fRight, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditRight2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_RIGHT2)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlRight2.SetPos(m_fRight2, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditRight3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_RIGHT3)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlRight3.SetPos(m_fRight3, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditRight4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_RIGHT4)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlRight4.SetPos(m_fRight4, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditRight5() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_RIGHT5)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlRight5.SetPos(m_fRight5, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditRight6() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_RIGHT6)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlRight6.SetPos(m_fRight6, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditRight7() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_RIGHT7)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlRight7.SetPos(m_fRight7, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditRight8() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_RIGHT8)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlRight8.SetPos(m_fRight8, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditLowShelfCotoff() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditLeft() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_LEFT)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditHighShelfCotoff() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditCentralFrequency1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlCentralFrequency1.SetPos(m_fCentralFrequency1, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditCentralFrequency2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlCentralFrequency2.SetPos(m_fCentralFrequency2, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditCentralFrequency3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlCentralFrequency3.SetPos(m_fCentralFrequency3, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditCentralFrequency4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlCentralFrequency4.SetPos(m_fCentralFrequency4, 0,1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditCentralFrequency5() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlCentralFrequency5.SetPos(m_fCentralFrequency5, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditCentralFrequency6() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlCentralFrequency6.SetPos(m_fCentralFrequency6, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditCentralFrequency7() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlCentralFrequency7.SetPos(m_fCentralFrequency7, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditCentralFrequency8() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		m_sctrlCentralFrequency8.SetPos(m_fCentralFrequency8, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditGain1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_GAIN1)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditGain2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_GAIN2)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditGain3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_GAIN3)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditGain4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_GAIN4)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditGain5() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_GAIN5)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditGain6() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_GAIN6)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditGain7() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_GAIN7)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditGain8() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_GAIN8)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditWidth1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditWidth2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditWidth3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditWidth4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditWidth5() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditWidth6() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditWidth7() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditWidth8() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnChangeEditMatserGain() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_MATSER_GAIN)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgEqualizerSet::OnButtonLoadFromDefault() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_lstPresets.GetCurSel();
	m_nPrestesIndex = nIndex + 1;
	m_fRight9 = m_tun_data.eq_mode[nIndex].boostdB_default_band[0] * m_nLevelStep;
	m_fRight10 = m_tun_data.eq_mode[nIndex].boostdB_default_band[1] * m_nLevelStep;
	m_fRight11 = m_tun_data.eq_mode[nIndex].boostdB_default_band[2] * m_nLevelStep;
	m_fRight12 = m_tun_data.eq_mode[nIndex].boostdB_default_band[3] * m_nLevelStep;
	m_fRight13 = m_tun_data.eq_mode[nIndex].boostdB_default_band[4] * m_nLevelStep;
	m_fRight14 = m_tun_data.eq_mode[nIndex].boostdB_default_band[5] * m_nLevelStep;
	m_fRight15 = m_tun_data.eq_mode[nIndex].boostdB_default_band[6] * m_nLevelStep;
	m_fRight16 = m_tun_data.eq_mode[nIndex].boostdB_default_band[7] * m_nLevelStep;

	UpdateData(FALSE);
	SetSliderPos();
	
}

void CDlgEqualizerSet::OnButtonSaveToDefault() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData())
	{
		return;
	}
	int nIndex = m_lstPresets.GetCurSel();
	m_nPrestesIndex = nIndex + 1;
	m_tun_data.eq_mode[nIndex].boostdB_default_band[0] = m_fRight9/m_nLevelStep;
	m_tun_data.eq_mode[nIndex].boostdB_default_band[1] = m_fRight10/m_nLevelStep;
	m_tun_data.eq_mode[nIndex].boostdB_default_band[2] = m_fRight11/m_nLevelStep;
	m_tun_data.eq_mode[nIndex].boostdB_default_band[3] = m_fRight12/m_nLevelStep;
	m_tun_data.eq_mode[nIndex].boostdB_default_band[4] = m_fRight13/m_nLevelStep;
	m_tun_data.eq_mode[nIndex].boostdB_default_band[5] = m_fRight14/m_nLevelStep;
	m_tun_data.eq_mode[nIndex].boostdB_default_band[6] = m_fRight15/m_nLevelStep;
	m_tun_data.eq_mode[nIndex].boostdB_default_band[7] = m_fRight16/m_nLevelStep;
	
}

void CDlgEqualizerSet::OnChangeEditLevelStep() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(!UpdateData())
	{
		return;
	}
	if(m_nOldLevelStep != -1)
	{
		m_fRight9 = m_fRight9 / m_nOldLevelStep;
		m_fRight10 = m_fRight10 / m_nOldLevelStep;
		m_fRight11 = m_fRight11 / m_nOldLevelStep;
		m_fRight12 = m_fRight12 / m_nOldLevelStep;
		m_fRight13 = m_fRight13 / m_nOldLevelStep;
		m_fRight14 = m_fRight14 / m_nOldLevelStep;
		m_fRight15 = m_fRight15 / m_nOldLevelStep;
		m_fRight16 = m_fRight16 / m_nOldLevelStep;
	}
	
	m_nOldLevelStep = m_nLevelStep;
	int nValue = 0;
	nValue = m_fRight9 * m_nLevelStep;
	if(nValue > 18 || nValue < -72)
	{
		AfxMessageBox(_T("Please adjust Level Step to insure the value in [-72,18]"));
		return;
	}
	m_fRight9 = nValue;
	nValue = m_fRight10 * m_nLevelStep;
	if(nValue > 18 || nValue < -72)
	{
		AfxMessageBox(_T("Please adjust Level Step to insure the value in [-72,18]"));
		return;
	}
	m_fRight10 = nValue;
	nValue = m_fRight11 * m_nLevelStep;
	if(nValue > 18 || nValue < -72)
	{
		AfxMessageBox(_T("Please adjust Level Step to insure the value in [-72,18]"));
		return;
	}
	m_fRight11 = nValue;
	nValue = m_fRight12 * m_nLevelStep;
	if(nValue > 18 || nValue < -72)
	{
		AfxMessageBox(_T("Please adjust Level Step to insure the value in [-72,18]"));
		return;
	}
	m_fRight12 = nValue;
	nValue = m_fRight13 * m_nLevelStep;
	if(nValue > 18 || nValue < -72)
	{
		AfxMessageBox(_T("Please adjust Level Step to insure the value in [-72,18]"));
		return;
	}
	m_fRight13 = nValue;
	nValue = m_fRight14 * m_nLevelStep;
	if(nValue > 18 || nValue < -72)
	{
		AfxMessageBox(_T("Please adjust Level Step to insure the value in [-72,18]"));
		return;
	}
	m_fRight14 = nValue;
	nValue = m_fRight15 * m_nLevelStep;
	if(nValue > 18 || nValue < -72)
	{
		AfxMessageBox(_T("Please adjust Level Step to insure the value in [-72,18]"));
		return;
	}
	m_fRight15 = nValue;
	nValue = m_fRight16 * m_nLevelStep;	
	if(nValue > 18 || nValue < -72)
	{
		AfxMessageBox(_T("Please adjust Level Step to insure the value in [-72,18]"));
		return;
	}
	m_fRight16 = nValue;
	
	UpdateData(FALSE);
	SetSliderPos();
}

void CDlgEqualizerSet::OnChangeEditLevelN() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(!UpdateData())
	{
		return;
	}
	SetSliderPos();
}

UINT CDlgEqualizerSet::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CPropertySheet::OnNcHitTest(point);
	CRect rc;
	GetClientRect(&rc);
	
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);
}