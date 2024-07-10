// MusicModePage.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "MusicModePage.h"
#include "DlgArmVolume.h"
#include "DlgAlcSet.h"
#include "DlgLcfSet.h"
#include "DlgEqualizerSet.h"
#include <math.h>
#include "DlgLineinGain.h"
#include "DlgRecord.h"
#include "AudioTesterSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
////8800G table/////////////////////////////////////////////////////////////
//Vdg Table
//客户可选值 : Vdg dB
const float CMusicModePage::vdg_dB_array[128] ={18.06, 17.28, 16.59, 15.83, 15.00, 14.30, 13.53, 12.82, 12.04, 11.26, 10.57, 9.81, 8.98, 8.28, 7.51, 6.80, 6.02, 5.24, 4.54, 3.79, 2.96, 2.26, 1.49, 0.78, 0.00, -0.78, -1.48, -2.23, -3.06, -3.76, -4.53, -5.24, -6.02, -6.80, -7.50, -8.25, -9.08, -9.78, -10.55, -11.26, -12.04, -12.82, -13.52, -14.27, -15.10, -15.80, -16.57, -17.28, -18.06, -18.84, -19.54, -20.29, -21.12, -21.82, -22.59, -23.30, -24.08, -24.86, -25.56, -26.31, -27.14, -27.85, -28.61, -29.32, -30.10, -30.88, -31.58, -32.33, -33.16, -33.87, -34.63, -35.35, -36.12, -36.90, -37.60, -38.36, -39.18, -39.89, -40.65, -41.37, -42.14, -42.92, -43.62, -44.38, -45.20, -45.91, -46.67, -47.39, -48.16, -48.95, -49.64, -50.40, -51.22, -51.93, -52.69, -53.41, -54.19, -54.97, -55.66, -56.42, -57.24, -57.95, -58.71, -59.43, -60.21, -60.99, -61.68, -62.44, -63.27, -63.97, -64.73, -65.45, -66.23, -67.01, -67.70, -68.46, -69.29, -69.99, -70.75, -71.47, -72.25, -73.03, -73.72, -74.48, -75.31, -76.01, -76.78, -77.49};

// nv 设定值 : Vdg value      对应于 volume value: bit15~bit 9  unsigned
const short CMusicModePage::vdg_value_array[128]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127};

//  P2(GOL/R) Table
//客户可选值 P2(GOL/R)  dB
const float CMusicModePage::p2_dB_array[32]={4.5,4.0,3.5,3.0,2.5,2.0,1.5,1.0,0.5,-0.5,-1.5,-2.5,-3.5,-4.5,-5.5,-6.5,-7.5,-8.5,-9.5,-10.5,-11.5,-13.5,-15.5,-17.5,-19.5,-21.5,-23.5,-25.5,-27.5,-29.5,-31.5,-33.5};

// nv 设定值 : P2(GOL/R) value 对应于 volume value: bit8 ~ bit4
const short CMusicModePage::p2_value_array[32]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

//  P1(GODL/R) Table
//客户可选值   P1(GODL/R)  dB
const float CMusicModePage::p1_dB_array[16]={0.0,-1.5,-3.0,-4.5,-6.0,-7.5,-9.0,-10.5,-12.0,-13.5,-15.0,-16.5,-18.0,-19.5,-21.0,-22.5};

// nv 设定值 :   P1(GODL/R) value 对应于 volume value: bit3 ~ bit0
const short CMusicModePage::p1_value_array[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

//for 6530
const float CMusicModePage::p1_dB_handsfree_array[16]={-255,-21,-18,-15,-12,-9,-6,-3,0,3,6,9,12,15,18,21};//-255 = mute

const float CMusicModePage::p1_dB_handset_array[16]={-255,-33,-30,-27,-24,-21,-18,-15,-12,-9,-6,-3,0,3,6,9};

const float CMusicModePage::p1_dB_headset_array[16]={-255,-33,-30,-27,-24,-21,-18,-15,-12,-9,-6,-3,0,3,6,9};

const float CMusicModePage::p1_dB_headfree_array[16]={-255,-21,-18,-15,-12,-9,-6,-3,0,3,6,9,12,15,18,21};

const float CMusicModePage::p2_dB_headfree_array[32]={-255,-33,-30,-27,-24,-21,-18,-15,-12,-9,-6,-3,0,3,6,9};

const short CMusicModePage::p_value_6530_array[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};


// nv 设定值 :   inner PA
const short CMusicModePage::pa_value_handsfree_array[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

//for smartphone
const float CMusicModePage::pa_dB_handsfree_array[16]={-255,-21,-18,-15,-12,-9,-6,-3,0,3,6,9,12,15,18,21};//-255 = mute


//Vdg Table
//客户可选值 : Vdg dB
const float CMusicModePage::vdg_bths_dB_array[97] ={24,23.25,22.5,21.75,21,20.25,19.5,18.75,18,17.25,16.5,15.75,15,14.25,13.5,12.75,12,11.25,10.5,9.75,9,8.25,7.5,6.75,6,5.25,4.5,3.75,3,2.25,1.5,0.75,0,-0.75,-1.5,-2.25,-3,-3.75,-4.5,-5.25,-6,-6.75,-7.5,-8.25,-9,-9.75,-10.5,-11.25,-12,-12.75,-13.5,-14.25,-15,-15.75,-16.5,-17.25,-18,-18.75,-19.5,-20.25,-21,-21.75,-22.5,-23.25,-24,-24.75,-25.5,-26.25,-27,-27.75,-28.5,-29.25,-30,-30.75,-31.5,-32.25,-33,-33.75,-34.5,-35.25,-36,-36.75,-37.5,-38.25,-39,-39.75,-40.5,-41.25,-42,-42.75,-43.5,-44.25,-45,-45.75,-46.5,-47.25,-48};

/////////////////////////////////////////////////////////////////////////////
// CMusicModePage property page

IMPLEMENT_DYNCREATE(CMusicModePage, CPropertyPage)

CMusicModePage::CMusicModePage() : CPropertyPage(CMusicModePage::IDD)
{
	//{{AFX_DATA_INIT(CMusicModePage)
		// NOTE: the ClassWizard will add member initialization here
	m_nCBInputSelect = 0;
	m_nCBOutputSelect = 0;
	m_nCBDa0Reverse = 0;
	m_nCBDa1Reverse = 0;
	m_nCBStereoOutput = 0;
	m_nCBSpeakerAmp = 0;
	m_nCBHeadsetAmp = 0;
	m_nCBMixerInput = 0;
	m_nCBEqSel = 0;
	m_dLimit = 0.0;
	m_strAppType = _T("");
	m_strNote = _T("");
	m_dMp3Gain = -60;
	m_dMidiGain = -60;
	m_dAmrGain = -60;
	m_nArmRecord = 0;
	m_nLineinRecord = 0;
	m_nCBPostProcess = 0;
	m_nCBFormat = 0;
	//}}AFX_DATA_INIT
	m_nValidDevSetCount = 0;
	memset(&m_VolumeData, 0, sizeof(VOLUME_STRUCT));
	memset(&m_eq_data, 0, sizeof(EQ_STRUCT_G_T));
	memset(&m_tun_data, 0, sizeof(TUN_STRUCT_G_T));
	m_nOldIndex = -1;
	m_nNewIndex = -1;
	m_dMp3GainBak = -60;
	m_dMidiGainBak = -60;
	m_dAmrGainBak = -60;
	
	m_nPageIndex = 0;
	
	m_nEQIndex = -1;
	m_nTunIndex = -1;
	
	m_strTitle = _T("");
	m_bDspCodecInfo = FALSE;
	m_bInit = FALSE;
	m_bSmartPhone = FALSE;
	m_bSerialPara = FALSE;
	m_eqData_g.lpEQMode = NULL;
	m_eqData_g.eq_count = 0;
	m_nParaSetIndex = 0;
	m_nOldParaSetIndex = -1;
	m_nNewParaSetIndex = -1;
	m_nEQPresetsIndex = 1;
}

CMusicModePage::~CMusicModePage()
{
	if(m_eqData_g.lpEQMode != NULL)
	{
		delete m_eqData_g.lpEQMode;
		m_eqData_g.lpEQMode = NULL;
	}
}

void CMusicModePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMusicModePage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST_DEVICE_TYPE, m_lstDeviceType);
	DDX_Control(pDX, IDC_LIST_APP_TYPE, m_lstAppType);
	DDX_Control(pDX, IDC_LIST_EQ_VOLUME, m_lstVolume);
	DDX_Text(pDX, IDC_EDT_LIMIT, m_dLimit);
	DDV_MinMaxDouble(pDX, m_dLimit, -42, 0);
	DDX_Text(pDX, IDC_EDT_MP3_GAIN, m_dMp3Gain);
	DDV_MinMaxFloat(pDX, m_dMp3Gain, -60, 30);
	DDX_Text(pDX, IDC_EDT_MIDI_GAIN, m_dMidiGain);
	DDV_MinMaxFloat(pDX, m_dMidiGain, -60, 30);
	DDX_Text(pDX, IDC_EDT_AMR_GAIN, m_dAmrGain);
	DDV_MinMaxFloat(pDX, m_dAmrGain, -60, 30);
	DDX_Text(pDX, IDC_EDT_ARM_RECORD, m_nArmRecord);
	DDX_Text(pDX, IDC_EDT_LINEIN_RECORD, m_nLineinRecord);
	DDX_CBIndex(pDX, IDC_COMBO_INPUT_SELECT, m_nCBInputSelect);
	DDX_CBIndex(pDX, IDC_COMBO_OUTPUT_SELECT, m_nCBOutputSelect);
	DDX_CBIndex(pDX, IDC_COMBO_DA0_REVERSE, m_nCBDa0Reverse);
	DDX_CBIndex(pDX, IDC_COMBO_DA1_REVERSE, m_nCBDa1Reverse);
	DDX_CBIndex(pDX, IDC_COMBO_STEREO_OUTPUT, m_nCBStereoOutput);
	DDX_CBIndex(pDX, IDC_COMBO_SPEAKER_AMP, m_nCBSpeakerAmp);
	DDX_CBIndex(pDX, IDC_COMBO_HEADSET_AMP, m_nCBHeadsetAmp);
	DDX_CBIndex(pDX, IDC_COMBO_MIXER_INPUT, m_nCBMixerInput);
	DDX_CBIndex(pDX, IDC_COMBO_EQ_SEL, m_nCBEqSel);
	DDX_CBIndex(pDX, IDC_COMBO_POST_PROCESS, m_nCBPostProcess);
	DDX_Control(pDX, IDC_COMBO_FORMAT, m_boxFormat);
	DDX_CBIndex(pDX, IDC_COMBO_FORMAT, m_nCBFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMusicModePage, CPropertyPage)
	//{{AFX_MSG_MAP(CMusicModePage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_LCF_SET, OnBtnLcfSet)
	ON_BN_CLICKED(IDC_BTN_EQ_SET, OnBtnEqSet)
	ON_BN_CLICKED(IDC_BTN_ALC_SET, OnBtnAlcSet)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_TYPE, OnSelchangeListDeviceType)
	ON_LBN_SELCHANGE(IDC_LIST_APP_TYPE, OnSelchangeListAppType)
	ON_COMMAND(ID_BUTTON_MUSIC_LOAD_CURRENT_MODE_FROM_FLASH, OnButtonMusicLoadCurrentModeFromFlash)
	ON_COMMAND(ID_BUTTON_MUSIC_LOAD_CURRENT_MODE_FROM_PHONE, OnButtonMusicLoadCurrentModeFromPhone)
	ON_COMMAND(ID_BUTTON_MUSIC_WRITE_CURRENT_MODE_TO_FLASH, OnButtonMusicWriteCurrentModeToFlash)
	ON_COMMAND(ID_BUTTON_MUSIC_WRITE_CURRENT_MODE_TO_PHONE, OnButtonMusicWriteCurrentModeToPhone)
	ON_BN_CLICKED(IDC_BUTTON_LINEIN_GAIN, OnButtonLineinGain)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BTN_RECORD_DP, OnBtnRecordDb)
	ON_CBN_SELCHANGE(IDC_COMBO_FORMAT, OnSelchangeComboFormat)
	ON_COMMAND(ID_BUTTON_SSEQ_G_LOAD_CURRENT_MODE_FROM_FLASH, OnButtonSseqGLoadCurrentModeFromFlash)
	ON_COMMAND(ID_BUTTON_SSEQ_G_LOAD_CURRENT_MODE_FROM_PHONE, OnButtonSseqGLoadCurrentModeFromPhone)
	ON_COMMAND(ID_BUTTON_SSEQ_G_WRITE_CURRENT_MODE_TO_FLASH, OnButtonSseqGWriteCurrentModeToFlash)
	ON_COMMAND(ID_BUTTON_SSEQ_G_WRITE_CURRENT_MODE_TO_PHONE, OnButtonSseqGWriteCurrentModeToPhone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMusicModePage message handlers

int CMusicModePage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!m_toolBarMusic.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_toolBarMusic.LoadToolBar(IDR_TOOLBAR_MUSIC))
	{
		DWORD dwErr = GetLastError();
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	CToolBarCtrl &ctl = m_toolBarMusic.GetToolBarCtrl();
	DWORD dwStyle = ctl.GetStyle();
	dwStyle |= CBRS_TOOLTIPS;
	ctl.SetStyle(dwStyle);


	if (!m_toolBarEQ.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_toolBarEQ.LoadToolBar(IDR_TOOLBAR_SS_EQ))
	{
		DWORD dwErr = GetLastError();
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	CToolBarCtrl &ctl1 = m_toolBarEQ.GetToolBarCtrl();
	DWORD dwStyle1 = ctl1.GetStyle();
	dwStyle1 |= CBRS_TOOLTIPS;
	ctl1.SetStyle(dwStyle1);
	
	return 0;
}

BOOL CMusicModePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitToolbar();

	CAudioTesterSheet* pSheet = (CAudioTesterSheet *)GetParent();
	//teana hu 2012.06.14
	for(int i = 0; i < NUM_FORMAT; i++)
	{
		m_boxFormat.AddString(strFormat[i]);
	}
	m_boxFormat.SetCurSel(0);
	//
	
	m_bSerialPara = pSheet->m_bSerialPara;
	m_eqData_g.eq_count = pSheet->m_eqData_g.eq_count;
	m_eqData_g.lpEQMode = new EQ_STRUCT_G_T[m_eqData_g.eq_count];
	
	for(i = 0; i < m_eqData_g.eq_count; i++)
	{
		memcpy(&m_eqData_g.lpEQMode[i], &(pSheet->m_eqData_g.lpEQMode[i]), sizeof(EQ_STRUCT_G_T));
	}

//	if(!m_bSerialPara)
	{
		m_toolBarEQ.ShowWindow(SW_HIDE);
	}
	//

	// Set style
	DWORD dwStyle = m_lstVolume.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES;    
    dwStyle &= ~LVS_EX_HEADERDRAGDROP;
    m_lstVolume.SetExtendedStyle( dwStyle );

	
	pSheet->ChangeFont( this,&(pSheet->m_Font) );
	InitAreas(pSheet);
	InitValue(m_VolumeData);
	InitListCtrl();

	m_lstVolume.SetItemState(5, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	//teana hu 2011.05.11
	if((m_strTitle == _T("Handsfree") || m_strTitle == _T("Headset")) && pSheet->m_bLoadVolume)
	{
		GetDlgItem(IDC_BUTTON_LINEIN_GAIN)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_LINEIN_GAIN)->ShowWindow(SW_HIDE);
	}
	//

	//teana hu 2012.03.02
	if(m_bSmartPhone)
	{
		if((m_strTitle == _T("Handsfree") || m_strTitle == _T("Headset")) && pSheet->m_bLoadENHA)
		{
			GetDlgItem(IDC_BTN_RECORD_DP)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BTN_RECORD_DP)->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		if((m_strTitle == _T("Handsfree") || m_strTitle == _T("Headset")) && pSheet->m_bLoadENHA)
		{
			GetDlgItem(IDC_BTN_RECORD_DP)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BTN_RECORD_DP)->ShowWindow(SW_HIDE);
		}
	}
	//

	//teana hu 2012.02.01
	if( m_bDspCodecInfo)
	{
		GetDlgItem(IDC_EDT_ARM_RECORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_LINEIN_RECORD)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDT_ARM_RECORD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_LINEIN_RECORD)->EnableWindow(FALSE);
	}
	//
	
	//teana hu 2012.03.29
	if(m_nEQIndex == -1)
	{
		GetDlgItem(IDC_BTN_EQ_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RECORD_DP)->EnableWindow(FALSE);
	}
	//

	RefreshControls();
	m_bInit = TRUE;

	return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CMusicModePage::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_LISTEX_COLLUM_INDEX )
	{
        int nRow = m_lstVolume.GetCurSel();
        int nCol = m_lstVolume.GetCurSelCol();
        int nSpinLow = m_vAllLinesData[nRow].at(nCol).m_nSpinLow;
        int nSpinHigh = m_vAllLinesData[nRow].at(nCol).m_nSpinHigh;        
        
        m_lstVolume.SetShowType( m_vAllLinesData[nRow].at( nCol).m_enShowType );
        if( m_vAllLinesData[nRow].at( nCol).m_bShowTitle )
        {
            CString sNum;
            
            CString sTitle(m_vAllLinesData[nRow].at( nCol ).m_strShowString);
            if( CInPlaceEdit::HEX_NUMBER == m_vAllLinesData[nRow].at( nCol ).m_enumEditType )
            {
                sNum.Format(_T("%X"),m_vAllLinesData[nRow].at( nCol ).m_lValue);
            }
            else
            {
                sNum.Format(_T("%d"),m_vAllLinesData[nRow].at( nCol ).m_lValue);                
            }            
            sTitle.Replace(sNum,_T(""));
            m_lstVolume.SetSpinTitle( sTitle );
        }    
        else
        {
            m_lstVolume.SetSpinTitle("");
        }
        
        if( CListCtrlEx::COMBOX_TYPE ==  m_vAllLinesData[nRow].at( nCol ).m_enShowType )
        {
            m_lstVolume.SetComBoxStrings( m_vAllLinesData[nRow].at( nCol ).m_vComboxStrings );
        }
        // set spin range of every cell
        if( -1 != nSpinHigh && -1 != nSpinLow )
            m_lstVolume.SetSpinRange( nSpinLow, nSpinHigh );
        return TRUE;
    }
    else if( message == WM_INPLACE_KILL_FOCUS )
    {
        return TRUE;
    }
    else if ( message == WM_INPLACE_COMBOX )
    {
        LPCOMBOX_DATA pData = (LPCOMBOX_DATA)lParam;
        int nRow = m_lstVolume.GetCurSel();
        int nCol = m_lstVolume.GetCurSelCol();      
        ASSERT( nRow >=0 && nRow < (int)m_vAllLinesData.size() );
        
        m_vAllLinesData[nRow].at( nCol ).m_lValue = pData->m_nSelectIdx;
        m_vAllLinesData[nRow].at( nCol ).m_strShowString 
            = m_vAllLinesData[nRow].at( nCol ).m_vComboxStrings[ pData->m_nSelectIdx ];   
        CString sTest(m_vAllLinesData[ nRow ].at(nCol).m_vComboxStrings[pData->m_nSelectIdx]);
        m_vAllLinesData[ nRow ].at(nCol).m_strShowString
            = m_vAllLinesData[ nRow ].at(nCol).m_vComboxStrings[pData->m_nSelectIdx];
		//teana hu 2012.02.22
		int nIndexVdg = (int)(m_vAllLinesData[nRow].at( 1 ).m_lValue);
		int nIndexP2 = (int)(m_vAllLinesData[nRow].at( 2 ).m_lValue);
		int nIndexP1 = (int)(m_vAllLinesData[nRow].at( 3 ).m_lValue);
		
		LONG lValue = 0;
		int nValue = 0;
		CString strValue;

		strValue = m_lstVolume.GetItemText(nRow, 5); //5 is column No. of  "Volume value"

		if(m_bDspCodecInfo)
		{
			LONG lTemp = 0;
#ifdef _UNICODE
			char szTmp[16] = {0};
			WideCharToMultiByte(CP_ACP,0,strValue,-1,szTmp,16,NULL,NULL);
			if(strstr(szTmp, "0x") != NULL)
			{
				char* pEnd = NULL;
				lTemp = (LONG)strtol(szTmp, &pEnd, 16);
			}
			else
			{
				lTemp = atol(szTmp);
			}
#else
			if(strValue.Find(_T("0x")) != -1)
			{
				char* pEnd = NULL;
				lTemp = (LONG)strtol((LPSTR)(LPCTSTR)strValue, &pEnd, 16);
			}
			else
			{
				lTemp = atol((LPSTR)(LPCTSTR)strValue);
			}
			
#endif

			if(m_strTitle == _T("Headfree"))
			{
				lValue = vdg_value_array[nIndexVdg];
				//lValue = (lValue << 9) << 16;
				lValue = lValue << 16;
				lValue = lValue | (p_value_6530_array[nIndexP2] << 4) |p_value_6530_array[nIndexP1];
				lTemp &= 0xff00;
				lValue |= lTemp;
				strValue.Format( _T("0x%x"), (LONG)lValue);
			}
			else if(m_strTitle == _T("BTHS"))
			{
				float fValue = vdg_bths_dB_array[nIndexVdg];
				fValue /= 0.75;
				lValue = fValue;
				
				int n = (unsigned short)m_VolumeData.reserve[49+nIndexVdg];
				n &= 0xff80;
				
				if(lValue < 0)
				{
					n |= 0x40;
					lValue = fabs(lValue);
					lValue = lValue - 1;
					lValue = ~lValue;
				}
				
				n = n | lValue;
				lValue = n;
				strValue.Format( _T("0x%x"), (unsigned short)lValue);
			}
			else
			{
				lValue = vdg_value_array[nIndexVdg];
				lValue = lValue << 16;
				if(m_strTitle != _T("BTHS"))
				{
					lValue = lValue | p_value_6530_array[nIndexP1];
				}
				lTemp &= 0xff00;
				lValue |= lTemp;
				strValue.Format( _T("0x%x"), (LONG)lValue);
			}
		}
		else
		{
			nValue = (vdg_value_array[nIndexVdg] << 9) | (p2_value_array[nIndexP2] << 4) | p1_value_array[nIndexP1];
			strValue.Format( _T("0x%x"), (SHORT)nValue);
		}
		
		
		m_lstVolume.SetItemText(nRow, 5, strValue);
		//
        return TRUE;
		
    }
	
	return CPropertyPage::DefWindowProc(message, wParam, lParam);
}

void CMusicModePage::OnBtnLcfSet() 
{
	// TODO: Add your control notification handler code here
	CDlgLcfSet dlg;
	dlg.SetValue(m_VolumeData);
	//dlg.SetEQValue(m_eqData_g.lpEQMode[m_nParaSetIndex]);
	dlg.SetEQValue(m_eq_data);
	dlg.SetSerialPara(m_bSerialPara);
	if(m_bSerialPara)
	{
		dlg.SetSampleRate(g_nSampleRate[m_nCBFormat]);
	}
	else
	{
		dlg.SetSampleRate(44100);
	}
	
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_VolumeData);
		//dlg.FetchEQValue(m_eqData_g.lpEQMode[m_nParaSetIndex]);
		dlg.FetchEQValue(m_eq_data);
	}
}

void CMusicModePage::OnBtnEqSet() 
{
	// TODO: Add your control notification handler code here
	CDlgEqualizerSet dlg;
	dlg.m_pSheet = (CAudioTesterSheet *)GetParent();
	dlg.SetIndex(m_nEQIndex, m_nTunIndex);
	dlg.SetValue(m_VolumeData);
	
	//teana hu 2012.06.14
	dlg.SetSerialPara(m_bSerialPara);
	
	if(m_bSerialPara)
	{
		//dlg.SetEQValue(m_eqData_g.lpEQMode[m_nParaSetIndex], m_tun_data);
		dlg.SetSampleRate(g_nSampleRate[m_nCBFormat]);
	}
	else
	{
		//dlg.SetEQValue(m_eq_data, m_tun_data);
		dlg.SetSampleRate(44100);
	}
	dlg.SetEQValue(m_eq_data, m_tun_data);
	//
	
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_VolumeData);
		//teana hu 2012.06.14
		/*
		if(m_bSerialPara)
		{
			dlg.FetchEQValue(m_eqData_g.lpEQMode[m_nParaSetIndex], m_tun_data);
		}
		else
		{
			dlg.FetchEQValue(m_eq_data, m_tun_data);
		}
		*/
		dlg.FetchEQValue(m_eq_data, m_tun_data);
		dlg.FetchPresetsIndex(m_nEQPresetsIndex);
		//
	}
}

void CMusicModePage::OnBtnAlcSet() 
{
	// TODO: Add your control notification handler code here
	CDlgAlcSet dlg;
	dlg.SetValue(m_VolumeData);
	//dlg.SetEQValue(m_eqData_g.lpEQMode[m_nParaSetIndex]);
	dlg.SetEQValue(m_eq_data);
	dlg.SetSerialPara(m_bSerialPara);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_VolumeData);
		//dlg.FetchEQValue(m_eqData_g.lpEQMode[m_nParaSetIndex]);
		dlg.FetchEQValue(m_eq_data);
	}
}

void CMusicModePage::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData())
	{
		return;
	}
	int n = m_lstAppType.GetCurSel();
	SetVolume(n);
	
	CAudioTesterSheet *pSheet = (CAudioTesterSheet *)GetParent();
	
	if(pSheet != NULL)
	{
		int nAppType = m_lstAppType.GetCurSel();
		//int nVolumeIndex= m_lstVolume.GetCurSel() + 1;
		int nVolumeIndex = -1;
		POSITION pos = m_lstVolume.GetFirstSelectedItemPosition();
		if(NULL != pos)
		{
			nVolumeIndex = m_lstVolume.GetNextSelectedItem(pos)+1;
		}

		pSheet->SetVolumePara(nVolumeIndex, nAppType);
	}
}

void CMusicModePage::OnSelchangeListDeviceType() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData())
	{
		return;
	}
	int nIndex = m_lstDeviceType.GetCurSel();
	SHORT nDevSet = m_VolumeData.dev_path_set.dev_set[nIndex];
	
	m_nCBInputSelect = nDevSet & 0x1;
	m_nCBOutputSelect = (nDevSet & 0x2) >> 1;
	m_nCBDa0Reverse = (nDevSet & 0x4) >> 2;
	m_nCBDa1Reverse = (nDevSet & 0x8) >> 3;
	m_nCBStereoOutput = (nDevSet & 0x10) >> 4;
	m_nCBSpeakerAmp = (nDevSet & 0x20) >> 5;
	m_nCBHeadsetAmp = (nDevSet & 0x40) >> 6;
	m_nCBMixerInput = (nDevSet & 0x80) >> 7;
	
	UpdateData(FALSE);
}

void CMusicModePage::OnSelchangeListAppType() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData())
	{
		return;
	}
	
	int nIndex = m_lstAppType.GetCurSel();
	
	m_nOldIndex = m_nNewIndex;
	if(m_nOldIndex >=0 )
	{
		int n = m_VolumeData.app_config_info_set.app_config_info[m_nOldIndex].eq_switch;
		int nEqSel = -1;
		if(m_nCBEqSel == 7)
		{
			nEqSel = 15;
		}
		else
		{
			nEqSel = m_nCBEqSel;
		}
		if(nEqSel != -1)
		{
			
			n = (n & 0xf0) | nEqSel;
			m_VolumeData.app_config_info_set.app_config_info[m_nOldIndex].eq_switch = n;
		}
		if(m_dMp3Gain != m_dMp3GainBak)
		{
			m_VolumeData.app_config_info_set.app_config_info[m_nOldIndex].agc_input_gain[0] = ceilf(((double)pow(10, m_dMp3Gain/20.0)) * 1024.0);
		}
		if(m_dMidiGain != m_dMidiGainBak)
		{
			m_VolumeData.app_config_info_set.app_config_info[m_nOldIndex].agc_input_gain[1] = ceilf(((double)pow(10, m_dMidiGain/20.0)) * 1024.0);
		}
		if(m_dAmrGain != m_dAmrGainBak)
		{
			m_VolumeData.app_config_info_set.app_config_info[m_nOldIndex].agc_input_gain[2] = ceilf(((double)pow(10, m_dAmrGain/20.0)) * 1024.0);
		}

		//teana hu 2012.06.14
		n = m_VolumeData.app_config_info_set.app_config_info[m_nOldIndex].eq_switch;
		n &= 0xffef;
		n |= (m_nCBPostProcess << 4);
		m_VolumeData.app_config_info_set.app_config_info[m_nOldIndex].eq_switch = n;
	//
		SetVolume(m_nOldIndex);
	}
	m_nNewIndex = nIndex;
	
	int nID = 0;
	
	switch(nIndex)
	{
	case 0:
		nID = IDS_APP_TYPE0;
		break;
	case 1:
		nID = IDS_APP_TYPE1;
		break;
	case 2:
		nID = IDS_APP_TYPE2;
		break;
	case 3:
		nID = IDS_APP_TYPE3;
		break;
	case 4:
		nID = IDS_APP_TYPE4;
		break;
	default:
		break;
	}
	
	CString strText = _T("");
	if(nID != 0)
	{
		strText.LoadString(nID);
	}
	
	GetDlgItem(IDC_STATIC_NOTE)->SetWindowText(strText);
	
	int nValue = m_VolumeData.app_config_info_set.app_config_info[nIndex].eq_switch;
	nValue = nValue & 0xF;
	if(nValue == 15)
	{
		m_nCBEqSel = 7;
	}
	else
	{
		m_nCBEqSel = nValue;
	}
	
	nValue = m_VolumeData.app_config_info_set.app_config_info[nIndex].agc_input_gain[0];
	if(nValue > 0 && nValue <= 32767)
	{
		m_dMp3Gain = (double)(20 * (double)log10((double)nValue/(double)1024));
		m_dMp3Gain = (double)((short)floorf(m_dMp3Gain * 10) / 10.0);
		m_dMp3Gain = (m_dMp3Gain < -60) ? -60 : m_dMp3Gain;
		m_dMp3Gain = (m_dMp3Gain > 30) ? 30 : m_dMp3Gain;
	}
	
	nValue = m_VolumeData.app_config_info_set.app_config_info[nIndex].agc_input_gain[1];
	if(nValue > 0 && nValue <= 32767)
	{
		m_dMidiGain = (double)(20 * (double)log10((double)nValue/(double)1024));
		m_dMidiGain = (double)((short)floorf(m_dMidiGain * 10) / 10.0);
		m_dMidiGain = (m_dMidiGain < -60) ? -60 : m_dMidiGain;
		m_dMidiGain = (m_dMidiGain > 30) ? 30 : m_dMidiGain;
	}
	
	nValue = m_VolumeData.app_config_info_set.app_config_info[nIndex].agc_input_gain[2];
	if(nValue > 0 && nValue <= 32767)
	{
		m_dAmrGain = (double)(20 * (double)log10((double)nValue/(double)1024));
		m_dAmrGain = (double)((short)floorf(m_dAmrGain * 10) / 10.0);
		m_dAmrGain = (m_dAmrGain < -60) ? -60 : m_dAmrGain;
		m_dAmrGain = (m_dAmrGain > 30) ? 30 : m_dAmrGain;
	}
	
	m_dMp3GainBak = m_dMp3Gain;
	m_dMidiGainBak = m_dMidiGain;
	m_dAmrGainBak = m_dAmrGain;

	//teana hu 2012.06.14
	m_nCBPostProcess = (m_VolumeData.app_config_info_set.app_config_info[nIndex].eq_switch & 0x10) >> 4;
	//


	SetVolumeList();
	
	UpdateData(FALSE);
}

void CMusicModePage::OnButtonMusicLoadCurrentModeFromFlash() 
{
	// TODO: Add your command handler code here
	CAudioTesterSheet *pSheet = (CAudioTesterSheet *)GetParent();
	
	if(pSheet != NULL)
	{
		if(m_bSerialPara)
		{
			pSheet->LoadENHAModes(TYPE_EQ, GET_ENHA_DATA_FROM_FLASH);
			pSheet->LoadENHAModes(TYPE_TUN, GET_ENHA_DATA_FROM_FLASH);
			
			for(int i = 0; i < m_eqData_g.eq_count; i++)
			{
				memcpy(&m_eqData_g.lpEQMode[i], &(pSheet->m_eqData_g.lpEQMode[i]), sizeof(EQ_STRUCT_G_T));
			}
			
			SetEQValue(m_eqData_g.lpEQMode[m_nParaSetIndex]);
			SetTunValue(pSheet->m_tunData_g.lpTunMode[m_nTunIndex]);
		}
		pSheet->GetVolumeParam(m_nPageIndex, GET_ARM_VOLUME_DATA_FROM_FLASH);
		memcpy(&m_VolumeData, &pSheet->m_VolumeData.lpVolumeMode[m_nPageIndex].volume_data, sizeof(VOLUME_STRUCT));
		SetValue(m_VolumeData);
		InitValue(m_VolumeData);
		SetVolumeList();
	}
}

void CMusicModePage::OnButtonMusicLoadCurrentModeFromPhone() 
{
	// TODO: Add your command handler code here
	CAudioTesterSheet *pSheet = (CAudioTesterSheet *)GetParent();
	
	if(pSheet != NULL)
	{
		if(m_bSerialPara)
		{
			pSheet->LoadENHAModes(TYPE_EQ, GET_ENHA_DATA_FROM_RAM);
			pSheet->LoadENHAModes(TYPE_TUN, GET_ENHA_DATA_FROM_RAM);
			
			for(int i = 0; i < m_eqData_g.eq_count; i++)
			{
				memcpy(&m_eqData_g.lpEQMode[i], &(pSheet->m_eqData_g.lpEQMode[i]), sizeof(EQ_STRUCT_G_T));
			}
			
			SetEQValue(m_eqData_g.lpEQMode[m_nParaSetIndex]);
			SetTunValue(pSheet->m_tunData_g.lpTunMode[m_nTunIndex]);
		}
		pSheet->GetVolumeParam(m_nPageIndex, GET_ARM_VOLUME_DATA_FROM_RAM);
		memcpy(&m_VolumeData, &pSheet->m_VolumeData.lpVolumeMode[m_nPageIndex].volume_data, sizeof(VOLUME_STRUCT));
		SetValue(m_VolumeData);
		InitValue(m_VolumeData);
		SetVolumeList();
	}
}

void CMusicModePage::OnButtonMusicWriteCurrentModeToFlash() 
{
	// TODO: Add your command handler code here
	GetValue();// 
	
	CAudioTesterSheet *pSheet = (CAudioTesterSheet *)GetParent();
	
	if(pSheet != NULL)
	{
		if(m_bSerialPara)
		{
			memcpy(&(pSheet->m_eqData_g.lpEQMode[m_nParaSetIndex]), &m_eq_data, sizeof(EQ_STRUCT_G_T));
			memcpy(&(pSheet->m_tunData_g.lpTunMode[m_nTunIndex]), &m_tun_data, sizeof(TUN_STRUCT_G_T));
			pSheet->WriteENHAParaToPhone(m_nParaSetIndex, TYPE_EQ, SET_ENHA_DATA_TO_FLASH,0);
			pSheet->WriteENHAParaToPhone(m_nTunIndex, TYPE_TUN, SET_ENHA_DATA_TO_FLASH,0);
		}
		memcpy(&pSheet->m_VolumeData.lpVolumeMode[m_nPageIndex].volume_data, &m_VolumeData, sizeof(VOLUME_STRUCT));
		pSheet->WriteVolumeParam(m_nPageIndex, SET_ARM_VOLUME_DATA_TO_FLASH);
	}
}

void CMusicModePage::OnButtonMusicWriteCurrentModeToPhone() 
{
	// TODO: Add your command handler code here
	GetValue();
	
	CAudioTesterSheet *pSheet = (CAudioTesterSheet *)GetParent();
	
	if(pSheet != NULL)
	{
		if(m_bSerialPara)
		{
			memcpy(&(pSheet->m_eqData_g.lpEQMode[m_nParaSetIndex]), &m_eq_data, sizeof(EQ_STRUCT_G_T));
			memcpy(&(pSheet->m_tunData_g.lpTunMode[m_nTunIndex]), &m_tun_data, sizeof(TUN_STRUCT_G_T));
			pSheet->WriteENHAParaToPhone(m_nParaSetIndex, TYPE_EQ, SET_ENHA_DATA_TO_RAM,m_nEQPresetsIndex);
			pSheet->WriteENHAParaToPhone(m_nTunIndex, TYPE_TUN, SET_ENHA_DATA_TO_RAM,m_nEQPresetsIndex);
		}
		memcpy(&pSheet->m_VolumeData.lpVolumeMode[m_nPageIndex].volume_data, &m_VolumeData, sizeof(VOLUME_STRUCT));
		// memcpy(&pSheet->m_VolumeData.lpVolumeMode[m_nPageIndex].volume_data, &pSheet->m_VolumeData.lpVolumeMode[m_nPageIndex+2].volume_data, sizeof(VOLUME_STRUCT));
		pSheet->WriteVolumeParam(m_nPageIndex, SET_ARM_VOLUME_DATA_TO_RAM);
	}
}

void CMusicModePage::SetValue( VOLUME_STRUCT& volume_data )
{
	memcpy(&m_VolumeData, &volume_data, sizeof(VOLUME_STRUCT));
}

void CMusicModePage::InitValue(VOLUME_STRUCT& volume_data)
{
	//teana hu 2012.06.12
	if((volume_data.reserve[40] & 0xff) == 0x2)
	{
		m_bSmartPhone = TRUE;
	}
	else
	{
		m_bSmartPhone = FALSE;
	}
	//

	//teana hu 2012.06.14
	if(m_nCBFormat == 0)
	{
		m_nParaSetIndex = (m_VolumeData.app_config_info_set.aud_proc_exp_control[0] & 0x7c00) >> 10;
	}
	else if(m_nCBFormat == 1)
	{
		m_nParaSetIndex = (m_VolumeData.app_config_info_set.aud_proc_exp_control[0] & 0x03e0) >> 5;
	}
	else if(m_nCBFormat == 2)
	{
		m_nParaSetIndex = m_VolumeData.app_config_info_set.aud_proc_exp_control[0] & 0x001f;
	}
	m_nParaSetIndex --;
	if(m_bSerialPara)
	{
		if(m_nParaSetIndex >= m_eqData_g.eq_count || m_nParaSetIndex < 0)
		{
			AfxMessageBox(_T("Error occur, when getting para set index for EQ!"));
		}
		else
		{
			memcpy(&m_eq_data, &m_eqData_g.lpEQMode[m_nParaSetIndex], sizeof(EQ_STRUCT_G_T));
		}
		
	}
	//

	m_lstDeviceType.ResetContent();

	m_lstAppType.ResetContent();
	
	m_nValidDevSetCount = volume_data.dev_path_set.valid_dev_set_count;
	for(int i = 0; i < m_nValidDevSetCount; i++)
	{
		CString strText = GetDeviceType(i);
		m_lstDeviceType.AddString(strText);
	}
	m_lstDeviceType.SetCurSel(0);
	SHORT nDevSet = volume_data.dev_path_set.dev_set[0];
	
	m_nCBInputSelect = nDevSet & 0x1;
	m_nCBOutputSelect = (nDevSet & 0x2) >> 1;
	m_nCBDa0Reverse = (nDevSet & 0x4) >> 2;
	m_nCBDa1Reverse = (nDevSet & 0x8) >> 3;
	m_nCBStereoOutput = (nDevSet & 0x10) >> 4;
	m_nCBSpeakerAmp = (nDevSet & 0x20) >> 5;
	m_nCBHeadsetAmp = (nDevSet & 0x40) >> 6;
	m_nCBMixerInput = (nDevSet & 0x80) >> 7;
	
	//teana hu 2012.06.14
	SHORT nlimit_linear = 0;
	if(m_bSerialPara)
	{
		//nlimit_linear = m_eqData_g.lpEQMode[m_nParaSetIndex].eq_control & 0x00ff;
		nlimit_linear = m_eq_data.eq_control & 0x00ff;
		
	}
	else
	{
		nlimit_linear = volume_data.reserve[0] & 0x00ff;
	}
	if(nlimit_linear >= 1 && nlimit_linear <= 127)
	{
		m_dLimit = (double)(20 * (double)log10((double)nlimit_linear/(double)127));
		m_dLimit = (double)((short)floorf(m_dLimit * 10) / 10.0);
		m_limitBak = m_dLimit;
	}
	//
	
	int nValidAppSetCount = volume_data.app_config_info_set.valid_app_set_count;
	for(i = 0; i < nValidAppSetCount; i++)
	{
		CString strText;
		strText.Format(_T("%d"), i);
		m_lstAppType.AddString(strText);
	}
	
	m_lstAppType.SetCurSel(0);
	
	CString strText;
	strText.LoadString(IDS_APP_TYPE0);
	((CStatic *)GetDlgItem(IDC_STATIC_NOTE))->SetWindowText(strText);
	
	int nValue = volume_data.app_config_info_set.app_config_info[0].eq_switch;
	nValue = nValue & 0xF;
	if(nValue == 15)
	{
		m_nCBEqSel = 7;
	}
	else
	{
		m_nCBEqSel = nValue;
	}
	
	nValue = volume_data.app_config_info_set.app_config_info[0].agc_input_gain[0];
	if(nValue > 0 && nValue <= 32767)
	{
		m_dMp3Gain = (double)(20 * (double)log10((double)nValue/(double)1024));
		m_dMp3Gain = (double)((short)floorf(m_dMp3Gain * 10) / 10.0);
		m_dMp3Gain = (m_dMp3Gain < -60) ? -60 : m_dMp3Gain;
		m_dMp3Gain = (m_dMp3Gain > 30) ? 30 : m_dMp3Gain;
	}
	
	nValue = volume_data.app_config_info_set.app_config_info[0].agc_input_gain[1];
	if(nValue > 0 && nValue <= 32767)
	{
		m_dMidiGain = (double)(20 * (double)log10((double)nValue/(double)1024));
		m_dMidiGain = (double)((short)floorf(m_dMidiGain * 10) / 10.0);
		m_dMidiGain = (m_dMidiGain < -60) ? -60 : m_dMidiGain;
		m_dMidiGain = (m_dMidiGain > 30) ? 30 : m_dMidiGain;
	}
	
	nValue = volume_data.app_config_info_set.app_config_info[0].agc_input_gain[2];
	if(nValue > 0 && nValue <= 32767)
	{
		m_dAmrGain = (double)(20 * (double)log10((double)nValue/(double)1024));
		m_dAmrGain = (double)((short)floorf(m_dAmrGain * 10) / 10.0);
		m_dAmrGain = (m_dAmrGain < -60) ? -60 : m_dAmrGain;
		m_dAmrGain = (m_dAmrGain > 30) ? 30 : m_dAmrGain;
	}
	
	m_dMp3GainBak = m_dMp3Gain;
	m_dMidiGainBak = m_dMidiGain;
	m_dAmrGainBak = m_dAmrGain;

	//teana hu 2012.02.01
	m_nArmRecord = volume_data.reserve[46];
	m_nLineinRecord = volume_data.reserve[47];
	//

	//teana hu 2012.06.14
	int nIndex = m_lstAppType.GetCurSel();
	m_nCBPostProcess = (m_VolumeData.app_config_info_set.app_config_info[nIndex].eq_switch & 0x10) >> 4;
	//


	strText.LoadString(IDS_APP_TYPE0);
	GetDlgItem(IDC_STATIC_NOTE)->SetWindowText(strText);

	
	UpdateData(FALSE);
}
	
void CMusicModePage::FetchValue( VOLUME_STRUCT& volume_data )
{
	if(m_bInit)
	{
		GetValue();
	}
	
	memcpy(&volume_data, &m_VolumeData, sizeof(VOLUME_STRUCT));
}

void CMusicModePage::SetEQValue( EQ_STRUCT_G_T eq_data)
{
	memcpy(&m_eq_data, &eq_data, sizeof(EQ_STRUCT_G_T));
}

void CMusicModePage::SetTunValue( TUN_STRUCT_G_T tun_data)
{
	memcpy(&m_tun_data, &tun_data, sizeof(TUN_STRUCT_G_T));
}

void CMusicModePage::FetchENHAValue( EQ_STRUCT_G_T eq_data, TUN_STRUCT_G_T tun_data)
{
	memcpy(&eq_data, &m_eq_data, sizeof(EQ_STRUCT_G_T));
	memcpy(&tun_data, &m_tun_data, sizeof(TUN_STRUCT_G_T));
}

void CMusicModePage::GetValue()
{
	if(!UpdateData())
	{
		return;
	}
	if(m_dLimit != m_limitBak)
	{
		int nValue = ceilf(((double)pow(10, m_dLimit/20.0)) * 127.0);
		if(m_bSerialPara)
		{
			//m_eqData_g.lpEQMode[m_nParaSetIndex].eq_control = (m_eqData_g.lpEQMode[m_nParaSetIndex].eq_control & 0xff00) | nValue;
			m_eq_data.eq_control = (m_eq_data.eq_control & 0xff00) | nValue;
		}
		else
		{
			m_VolumeData.reserve[0] = (m_VolumeData.reserve[0] & 0xff00) | nValue;
		}
	}

	int nIndex = m_lstAppType.GetCurSel();
	
	int n = m_VolumeData.app_config_info_set.app_config_info[nIndex].eq_switch;
	int nEqSel = -1;
	if(m_nCBEqSel == 7)
	{
		nEqSel = 15;
	}
	else
	{
		nEqSel = m_nCBEqSel;
		}
	if(nEqSel != -1)
	{
		
		n = (n & 0xf0) | nEqSel;
		m_VolumeData.app_config_info_set.app_config_info[nIndex].eq_switch = n;
	}

	if(m_dMp3Gain != m_dMp3GainBak)
	{
		m_VolumeData.app_config_info_set.app_config_info[nIndex].agc_input_gain[0] = ceilf(((double)pow(10, m_dMp3Gain/20.0)) * 1024.0);
	}

	if(m_dMidiGain != m_dMidiGainBak)
	{
		m_VolumeData.app_config_info_set.app_config_info[nIndex].agc_input_gain[1] = ceilf(((double)pow(10, m_dMidiGain/20.0)) * 1024.0);
	}

	if(m_dAmrGain != m_dAmrGainBak)
	{
		m_VolumeData.app_config_info_set.app_config_info[nIndex].agc_input_gain[2] = ceilf(((double)pow(10, m_dAmrGain/20.0)) * 1024.0);
	}

	//teana hu 2012.02.01
	m_VolumeData.reserve[46] = m_nArmRecord;
	m_VolumeData.reserve[47] = m_nLineinRecord;
	//

	m_dMp3GainBak = m_dMp3Gain;
	m_dMidiGainBak = m_dMidiGain;
	m_dAmrGainBak = m_dAmrGain;

	//teana hu 2012.06.14
	n = m_VolumeData.app_config_info_set.app_config_info[nIndex].eq_switch;
	n &= 0xffef;
	n |= (m_nCBPostProcess << 4);
	m_VolumeData.app_config_info_set.app_config_info[nIndex].eq_switch = n;
	//

	SetVolume(nIndex);
}

CString CMusicModePage::GetDeviceType(int nIndex)
{
	CString strDeviceType = _T("");
	switch(nIndex)
	{
	case 0:
		strDeviceType = _T("play");
		break;
	case 1:
		strDeviceType = _T("record");
		break;
	case 2:
		strDeviceType = _T("record linein");
	    break;
	default:
		strDeviceType = _T("default");
	    break;
	}

	return strDeviceType;
}

CString CMusicModePage::GetEQSel(int nIndex)
{
	CString strEQSel = _T("");
	switch(nIndex)
	{
	case 0:
		strEQSel = _T("EQMode_Sel_Off");
		break;
	case 1:
		strEQSel = _T("EQMode_Sel_Regular");
		break;
	case 2:
		strEQSel = _T("EQMode_Sel_Classic");
		break;
	case 3:
		strEQSel = _T("EQMode_Sel_Odeum");
		break;
	case 4:
		strEQSel = _T("EQMode_Sel_Jazz");
		break;
	case 5:
		strEQSel = _T("EQMode_Sel_Rock");
		break;
	case 6:
		strEQSel = _T("EQMode_Sel_Softrock");
		break;
	case 15:
		strEQSel = _T("EQMode_Sel_byMmi");
		break;
	default:
		strEQSel = _T("EQMode_Sel_Error");
		break;
	}
	
	return strEQSel;
}

int CMusicModePage::GetEQSelValue(CString strEQSel)
{
	int nValue = -1;

	strEQSel.MakeLower();
	
	if(strEQSel ==  _T("eqmode_sel_off"))
	{
		nValue = 0;
	}
	else if(strEQSel == _T("eqmode_sel_regular"))
	{
		nValue = 1;
	}
	else if(strEQSel == _T("eqmode_sel_classic"))
	{
		nValue = 2;
	}
	else if(strEQSel == _T("eqmode_sel_odeum"))
	{
		nValue = 3;
	}
	else if(strEQSel == _T("eqmode_sel_jazz"))
	{
		nValue = 4;
	}
	else if(strEQSel == _T("eqmode_sel_rock"))
	{
		nValue = 5;
	}
	else if(strEQSel == _T("eqmode_sel_softrock"))
	{
		nValue = 6;
	}
	else if(strEQSel == _T("eqmode_sel_byMmi"))
	{
		nValue = 15;
	}
	else if(strEQSel == _T("eqmode_sel_error"))
	{
		nValue = -1;
	}
	else
	{
		nValue = -1;
	}
	
	return nValue;
}

void CMusicModePage::InitListCtrl()
{
	// Set Data range
	m_lstVolume.SetEditCheckRange(TRUE,18,-100 );
	m_lstVolume.SetInplaceEditType( CInPlaceEdit::FLOAT_NUMBER,5 );

	// Insert columns
    LV_COLUMN	lvc;
    lvc.mask = LVCF_WIDTH | LVCF_TEXT ;
	const int lstColumnNum = 6;
	int lstVolLevel = 0;

	int n = m_lstAppType.GetCurSel();
	
	lstVolLevel = m_VolumeData.app_config_info_set.app_config_info[n].valid_volume_level_count;
	//
    char columnLabel[lstColumnNum][128];
    strcpy( columnLabel[0], "Volume level" );	
    strcpy( columnLabel[1], "Vdg(dB)" );		
    strcpy( columnLabel[2], "P2(GOL/R)(dB)" );		
    strcpy( columnLabel[3], "P1(GODL/R)(dB)" );	
	strcpy( columnLabel[4], "inner PA" );
    strcpy( columnLabel[5], "Volume value" );

	// Set column styles
	int nClWidth = m_lstVolume.GetColumnWidth(0);
	if(nClWidth != 120)
	{
		//teana hu 2012.02.01
		int DefaultColumnWidth[] = { 120,100,120,130,120,120 };
		if(m_bDspCodecInfo && m_strTitle != _T("Headfree"))
		{
			DefaultColumnWidth[2] = 0;
			if(m_strTitle == _T("BTHS"))
			{
				DefaultColumnWidth[3] = 0;
			}
		}
		//

		//teana hu 2012.06.12
		if(m_bSmartPhone)
		{
			if((m_VolumeData.reserve[44] & 0x00ff) != 1 || ((m_strTitle != _T("Handsfree")) && (m_strTitle != _T("Headfree"))))
			{
				DefaultColumnWidth[4] = 0;
			}
		}
		else
		{
			DefaultColumnWidth[4] = 0;
		}
		//
		
		for( int i=0; i<lstColumnNum; i++ )
		{
#ifdef _UNICODE
			TCHAR szTmp[128] = {0};
			MultiByteToWideChar(CP_ACP,0,columnLabel[i],-1,szTmp,128);
			lvc.pszText = szTmp;
#else
			lvc.pszText = columnLabel[i];
#endif
			lvc.cx = DefaultColumnWidth[i];
			lvc.fmt = 0;   
			
			m_lstVolume.InsertColumn( i, &lvc );
		} 
	}
	
	m_vAllLinesData.clear();
	CString strFormat;

    for(int i=0; i<lstVolLevel; ++i )
    {
        ROW_DATA singleLineData; 
        CListCtrlEx::COLUMN_DATA columnData;    

        // row = i, column = 0;
        columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
        columnData.m_strShowString.Format(_T("Level %d"),i+1 );
        columnData.m_lValue = -1;
        columnData.m_dReserved = 0.0;
        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;
        columnData.m_bShowTitle = FALSE;
        columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
        singleLineData.push_back( columnData );


        // row = i, column =1;
        columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;
		int nIndex  = 0;
		//teana hu 2012.02.01
		if(m_bDspCodecInfo)
		{
			if(m_strTitle == _T("BTHS"))
			{
				nIndex = CalcVdg_bths( columnData.m_dReserved,m_VolumeData.reserve[49+i]);
			}
			else
			{
				nIndex = CalcVdg_l( columnData.m_dReserved,m_VolumeData.app_config_info_set.app_config_info[n].arm_volume_l[i+1]);
			}	
		}
		else
		{
			nIndex = CalcVdg( columnData.m_dReserved,m_VolumeData.app_config_info_set.app_config_info[n].arm_volume[i+1] );	
		}
		//	
			 
        columnData.m_strShowString.Format( _T("%.2f"), columnData.m_dReserved );
		columnData.m_lValue = nIndex;
        columnData.m_vComboxStrings.clear();

		if(m_bDspCodecInfo && m_strTitle == _T("BTHS"))
		{
			for( int j=0;j<97;j++ )
			{
				strFormat.Format( _T("%.2f"),vdg_bths_dB_array[j] );
				
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}
		else
		{
			for( int j=0;j<128;j++ )
			{
				strFormat.Format( _T("%.2f"),vdg_dB_array[j] );
				
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}
		

        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;                
        columnData.m_bShowTitle = FALSE;               
        singleLineData.push_back( columnData );


        // row = i, column = 2;
        columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;  
		//teana hu 2012.02.14
		if(m_bDspCodecInfo)
		{
			if(m_strTitle == _T("Headfree"))
			{
				nIndex = CalcP2_l( columnData.m_dReserved,(unsigned short)m_VolumeData.app_config_info_set.app_config_info[n].arm_volume_l[i+1] );
				if(nIndex == 0)
				{
					columnData.m_strShowString = _T("mute");
				}
				else
				{
					columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
				}
			}
		}
		else
		{
			nIndex = CalcP2( columnData.m_dReserved,(unsigned short)m_VolumeData.app_config_info_set.app_config_info[n].arm_volume[i+1] );
			columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
		}
		//
	    columnData.m_lValue = nIndex;
        columnData.m_vComboxStrings.clear();
		//teana hu 2012.02.14
		if(m_bDspCodecInfo)
		{
			for( int j=0;j<16;j++ )
			{
				if(j == 0)
				{
					strFormat = _T("mute");
				}
				else
				{
					strFormat.Format( _T("%.1f"),p2_dB_headfree_array[j] );
				}
				
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}
		else
		{
			for( int j=0;j<32;j++ )
			{
				strFormat.Format( _T("%.1f"),p2_dB_array[j] );
				
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}
		//

        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;                
        columnData.m_bShowTitle = FALSE;               
        singleLineData.push_back( columnData );


        // row = i, column =3;
        columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;  
		//teana hu 2012.02.01
		if(m_bDspCodecInfo)
		{
			nIndex = CalcP1_l( columnData.m_dReserved,m_VolumeData.app_config_info_set.app_config_info[n].arm_volume_l[i+1]);
			if(nIndex == 0)
			{
				columnData.m_strShowString = _T("mute");
			}
			else
			{
				columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
			}
		}
		else
		{
			nIndex = CalcP1( columnData.m_dReserved,(unsigned short)m_VolumeData.app_config_info_set.app_config_info[n].arm_volume[i+1] );
			columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
		}
		//		

        
	    columnData.m_lValue = nIndex;
		columnData.m_vComboxStrings.clear();

		if(m_bDspCodecInfo)
		{
			for(int j=0;j<16;j++ )
			{
				if(j == 0)
				{
					strFormat = _T("mute");
				}
				else
				{
					if(m_strTitle == _T("Handsfree"))
					{
						strFormat.Format( _T("%.1f"),p1_dB_handsfree_array[j] );
					}
					else if(m_strTitle == _T("Handset"))
					{
						strFormat.Format( _T("%.1f"),p1_dB_handset_array[j] );
					}
					else if(m_strTitle == _T("Headset"))
					{
						strFormat.Format( _T("%.1f"),p1_dB_headset_array[j] );
					}
					else if(m_strTitle == _T("Headfree"))
					{
						strFormat.Format( _T("%.1f"),p1_dB_headfree_array[j] );
					}
				}
				
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}
		else
		{
			for(int j=0;j<16;j++ )
			{
				strFormat.Format( _T("%.1f"),p1_dB_array[j] );
				
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}

        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;                
        columnData.m_bShowTitle = FALSE;               
        singleLineData.push_back( columnData ); 
		
		//teana hu 2012.06.12
		// row = i, column =4;
        columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;  
		nIndex = CalcPA( columnData.m_dReserved,(unsigned short)m_VolumeData.reserve[45] );
		if(nIndex == 0)
		{
			columnData.m_strShowString = _T("mute");
		}
		else
		{
			columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
		}		
        
		columnData.m_lValue = nIndex;
		columnData.m_vComboxStrings.clear();
		
		for(int  j=0;j<16;j++ )
		{
			if(j == 0)
			{
				strFormat = _T("mute");
			}
			else
			{
				strFormat.Format( _T("%.1f"),pa_dB_handsfree_array[j] );
			}
			
			columnData.m_vComboxStrings.push_back(strFormat);
		}
		
        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;                
        columnData.m_bShowTitle = FALSE;               
        singleLineData.push_back( columnData ); 
		//
		
		// row = i, column =5;
        columnData.m_enShowType = CListCtrlEx::NOT_EDIT;

		//teana hu 2012.02.01
		if(m_bDspCodecInfo)
		{
			if(m_strTitle == _T("BTHS"))
			{
				columnData.m_lValue = (unsigned short)m_VolumeData.reserve[49+i];
				columnData.m_strShowString.Format( _T("0x%x"), (unsigned short)m_VolumeData.reserve[49+i]);
			}
			else
			{
				columnData.m_lValue = m_VolumeData.app_config_info_set.app_config_info[n].arm_volume_l[i+1];
				columnData.m_strShowString.Format( _T("0x%x"), (LONG)m_VolumeData.app_config_info_set.app_config_info[n].arm_volume_l[i+1]);
			}
		}
		else
		{
			columnData.m_lValue = (unsigned short)m_VolumeData.app_config_info_set.app_config_info[n].arm_volume[i+1];
			columnData.m_strShowString.Format( _T("0x%x"), (unsigned short)m_VolumeData.app_config_info_set.app_config_info[n].arm_volume[i+1]);
		}
		//
		
        columnData.m_nSpinLow=0;
        columnData.m_nSpinHigh = 32767;
        columnData.m_bShowTitle = FALSE;
        columnData.m_enumEditType = CInPlaceEdit::HEX_NUMBER;
        singleLineData.push_back( columnData );

		m_vAllLinesData.push_back( singleLineData );
    }   
	
	for( i=0; i<(signed)m_vAllLinesData.size(); ++i )
    {        
        int nColumnCount = m_vAllLinesData[i].size();
        m_lstVolume.InsertItem( i,_T("") );
        for( int j=0; j<nColumnCount; ++j )
        {            
            m_lstVolume.SetItemText( i, j, m_vAllLinesData[i].at( j).m_strShowString );
        }        
    }
}

void CMusicModePage::SetVolumeList()
{
	m_lstVolume.DeleteAllItems();
	m_vAllLinesData.clear();
	InitListCtrl();
}

int CMusicModePage::CalcVdg ( double& vdg, short volume )
{
	short value = (volume & 0xFE00) >> 9;
	if(m_bDspCodecInfo)
	{
		value = volume;
	}
	for(int i = 0; i < 128; i++)
	{
		if(vdg_value_array[i] == value)
		{
			vdg = vdg_dB_array[i];
			break;
		}
	}
	return i;
}

int CMusicModePage::CalcP2 ( double& p2, unsigned short volume )
{
	unsigned short value = (volume & 0x01F0) >> 4;
	for(int i = 0; i < 128; i++)
	{
		if(p2_value_array[i] == value)
		{
			p2 = p2_dB_array[i];
			break;
		}
	}
	return i;
}

int CMusicModePage::CalcP1 ( double& p1, unsigned short volume )
{
	unsigned short value = volume & 0x000F;
	if(m_bDspCodecInfo)
	{
		value = volume;
	}
	for(int i = 0; i < 128; i++)
	{
		if(p1_value_array[i] == value)
		{
			p1 = p1_dB_array[i];
			break;
		}
	}
	return i;
}

int CMusicModePage::CalcVdg_l ( double& vdg, LONG volume )
{
	short value = (volume & 0xffff0000) >> 16;
	//value = (value & 0xFE00) >> 9;
	for(int i = 0; i < 128; i++)
	{
		if(vdg_value_array[i] == value)
		{
			vdg = vdg_dB_array[i];
			break;
		}
	}
	if(i >= 128)
	{
		i = ERROR_INDEX;
	}
	return i;
}

int CMusicModePage::CalcP1_l ( double& p1, LONG volume )
{
	short value = volume & 0x0000ffff;
	value = value & 0xf;
	for(int i = 0; i < 16; i++)
	{
		if(m_strTitle == _T("Handsfree"))
		{
			if(p_value_6530_array[i] == value)
			{
				p1 = p1_dB_handsfree_array[i];
				break;
			}
		}
		else if(m_strTitle == _T("Handset"))
		{
			if(p_value_6530_array[i] == value)
			{
				p1 = p1_dB_handset_array[i];
				break;
			}
		}
		else if(m_strTitle == _T("Headset"))
		{
			if(p_value_6530_array[i] == value)
			{
				p1 = p1_dB_headset_array[i];
				break;
			}
		}
		else if(m_strTitle == _T("Headfree"))
		{
			if(p_value_6530_array[i] == value)
			{
				p1 = p1_dB_headfree_array[i];
				break;
			}
		}
	}
	if(i >= 16)
	{
		i = ERROR_INDEX;
	}
	return i;
}

int CMusicModePage::CalcP2_l ( double& p2, LONG volume )
{
	unsigned short value = volume & 0x0000ffff;
	value = (value & 0xf0) >> 4;
	for(int i = 0; i < 128; i++)
	{
		if(m_strTitle == _T("Headfree"))
		{
			if(p_value_6530_array[i] == value)
			{
				p2 = p2_dB_headfree_array[i];
				break;
			}
		}
	}
	if(i >= 128)
	{
		i = ERROR_INDEX;
	}
	return i;
}


int CMusicModePage::CalcPA( double& pa, LONG volume )
{
	unsigned short value = volume & 0x00ff;
	for(int i = 0; i < 16; i++)
	{
		if(m_strTitle == _T("Handsfree") || m_strTitle == _T("Headfree"))
		{
			if(pa_value_handsfree_array[i] == value)
			{
				pa = pa_dB_handsfree_array[i];
				break;
			}
		}
	}
	if(i >= 16)
	{
		i = ERROR_INDEX;
	}
	return i;
}

int CMusicModePage::CalcVdg_bths(double& vdg, short volume )
{
    short value = volume & 0x3f;
	int nFlag = (volume & 0x40) >> 6;
	float fValue = 0.0;
	if(nFlag == 1)
	{
		value = ((~value) & 0x3f) + 1;
		fValue = value * 0.75;
		fValue = 0 - fValue;
	}
	else
	{
		fValue = value * 0.75;
	}

	for(int i = 0; i < 97; i++)
	{
		if(vdg_bths_dB_array[i] == fValue)
		{
			vdg = vdg_bths_dB_array[i];
			break;
		}
	}
	return i;
}

void CMusicModePage::SetVolume(int n)
{
// 	if (!UpdateData(TRUE))
//     {
//         TRACE0("UpdateData failed during dialog termination.\n");
//         // the UpdateData routine will set focus to correct item
//         return;
//     }
	
	int nValue = 0;
	int nSize = 0;
	
	nSize = m_VolumeData.app_config_info_set.app_config_info[n].valid_volume_level_count;
	for( int i=0;i<nSize;i++ )
	{
		int nIndexVdg = (int)(m_vAllLinesData[i].at( 1 ).m_lValue);
		int nIndexP2 = (int)(m_vAllLinesData[i].at( 2 ).m_lValue);
		int nIndexP1 = (int)(m_vAllLinesData[i].at( 3 ).m_lValue);

// 		if(nIndexP1 == ERROR_INDEX || nIndexP2 == ERROR_INDEX || nIndexVdg == ERROR_INDEX)
// 		{
// 			continue;
// 		}
		
		LONG lValue = 0;
		//teana hu 2012.02.01
		if(m_bDspCodecInfo)
		{
			if(m_strTitle == _T("Headfree"))
			{
				lValue = vdg_value_array[nIndexVdg];
				//lValue = (lValue << 9) << 16;
				lValue = lValue << 16;
				lValue = lValue | (p_value_6530_array[nIndexP2] << 4) |p_value_6530_array[nIndexP1];

				LONG lTemp = m_VolumeData.app_config_info_set.app_config_info[n].arm_volume_l[i+1];
				lTemp &= 0xff00;
				m_VolumeData.app_config_info_set.app_config_info[n].arm_volume_l[i+1] = lValue | lTemp;
			}
			else if(m_strTitle == _T("BTHS"))
			{
				float fValue = vdg_bths_dB_array[nIndexVdg];
				fValue /= 0.75;
				lValue = fValue;

				int n = (unsigned short)m_VolumeData.reserve[49+i];
				n &= 0xff80;

				if(lValue < 0)
				{
					n |= 0x40;
					lValue = fabs(lValue);
					lValue = lValue - 1;
					lValue = ~lValue;
				}
				
				n = n | lValue;
				m_VolumeData.reserve[49+i] = n;
			}
			else
			{
				lValue = vdg_value_array[nIndexVdg];
				lValue = lValue << 16;
				if(m_strTitle != _T("BTHS"))
				{
					lValue = lValue | p_value_6530_array[nIndexP1];
				}

				LONG lTemp = m_VolumeData.app_config_info_set.app_config_info[n].arm_volume_l[i+1];
				lTemp &= 0xff00;
				m_VolumeData.app_config_info_set.app_config_info[n].arm_volume_l[i+1] = lValue | lTemp;
			}
		}
		else
		{
			nValue = (vdg_value_array[nIndexVdg] << 9) | (p2_value_array[nIndexP2] << 4) | p1_value_array[nIndexP1];
			m_VolumeData.app_config_info_set.app_config_info[n].arm_volume[i+1] = nValue;
			//teana hu 2012.06.12
			if(m_bSmartPhone && ((m_strTitle == _T("Handsfree")) || (m_strTitle == _T("Headfree"))) && (m_VolumeData.reserve[44] & 0xff) == 1)
			{
				int nIndexPA = (int)(m_vAllLinesData[i].at( 4 ).m_lValue);
				lValue = pa_value_handsfree_array[nIndexPA];
				m_VolumeData.reserve[45] &= 0xff00;
				m_VolumeData.reserve[45] |= lValue;
			}
			//
		}
		//
	}
}

BOOL CMusicModePage::WriteVolumeToPhone()
{
	return TRUE;
}

void CMusicModePage::InitToolbar()
{
    CRect reResultPos;
    GetDlgItem( IDC_STATIC_TOOLBAR_PLACEHOLDER_AUDIO )->GetWindowRect( &reResultPos );
    ScreenToClient( &reResultPos );
    m_toolBarMusic.MoveWindow( &reResultPos );
    
    m_toolBarMusic.ShowWindow( SW_SHOW );


    GetDlgItem( IDC_STATIC_TOOLBAR_PLACEHOLDER_EQ )->GetWindowRect( &reResultPos );
    ScreenToClient( &reResultPos );
    m_toolBarEQ.MoveWindow( &reResultPos );
    
    m_toolBarEQ.ShowWindow( SW_SHOW );
}

void CMusicModePage::InitAreas( void* pPara )
{
    SpecialRect::AREA_OF_MAP areasOfSinglePage;
    
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)pPara;

	SpecialRect* pTempArea = new SpecialRectOfVolumeCtrl;
	
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
	
	
    m_vAllAreas = areasOfSinglePage;
}

SpecialRect* CMusicModePage::FindAreadByName( CString strName )
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

BOOL CMusicModePage::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
			case ID_BUTTON_MUSIC_WRITE_CURRENT_MODE_TO_PHONE:   
				strTip.LoadString(IDS_WRITE_MS);
				break;  
			case ID_BUTTON_MUSIC_WRITE_CURRENT_MODE_TO_FLASH:  
				strTip.LoadString(IDS_WRITE_FLASH);  
				break;
			case ID_BUTTON_MUSIC_LOAD_CURRENT_MODE_FROM_PHONE:
				strTip.LoadString(IDS_LOAD_MS);
				break;
			case ID_BUTTON_MUSIC_LOAD_CURRENT_MODE_FROM_FLASH:
				strTip.LoadString(IDS_LOAD_FLASH);
				break;
			case ID_BUTTON_SSEQ_G_WRITE_CURRENT_MODE_TO_PHONE:
				strTip.LoadString(IDS_WRITE_EQ_MS);
				break;
			case ID_BUTTON_SSEQ_G_WRITE_CURRENT_MODE_TO_FLASH:
				strTip.LoadString(IDS_WRITE_EQ_FLASH);
				break;
			case ID_BUTTON_SSEQ_G_LOAD_CURRENT_MODE_FROM_PHONE:
				strTip.LoadString(IDS_LOAD_EQ_MS);
				break;
			case ID_BUTTON_SSEQ_G_LOAD_CURRENT_MODE_FROM_FLASH:
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
	
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void CMusicModePage::OnButtonLineinGain() 
{
	// TODO: Add your control notification handler code here
	CDlgLineinGain dlg;
	dlg.m_bDspCodecInfo = m_bDspCodecInfo;
	dlg.m_bSmartPhone = m_bSmartPhone;
	dlg.SetTitle(m_strTitle);
	dlg.SetValue(m_VolumeData);
	//teana hu 2012.06.12
	if(m_bSmartPhone)
	{
		dlg.m_nLstLevel = m_VolumeData.app_config_info_set.app_config_info[0].valid_volume_level_count;
	}
	else
	{
		dlg.m_nLstLevel = 15;
	}
	//
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_VolumeData);
	}
}

UINT CMusicModePage::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//	return CPropertyPage::OnNcHitTest(point);
	return HTTRANSPARENT;
}

void CMusicModePage::OnBtnRecordDb() 
{
	// TODO: Add your control notification handler code here
	CDlgRecord dlg;
	dlg.m_pSheet = (CAudioTesterSheet *)GetParent();
	dlg.SetIndex(m_nEQIndex, m_nPageIndex);
	//teana hu 2012.06.14
	if(m_bSerialPara)
	{
		//memcpy(&m_eq_data, &m_eqData_g.lpEQMode[m_nEQIndex], sizeof(EQ_STRUCT_G_T));
		dlg.SetValue(m_eqData_g.lpEQMode[m_nEQIndex]);
	}
	else
	{
		dlg.SetValue(m_eq_data);
	}
	//
	
	dlg.SetArmValue(m_VolumeData);
	dlg.SetControlFlag(m_bDspCodecInfo, m_bSmartPhone);

	dlg.DoModal();
	
	//teana hu 2012.06.14
	if(m_bSerialPara)
	{
		//memcpy(&m_eqData_g.lpEQMode[m_nEQIndex], &m_eq_data, sizeof(EQ_STRUCT_G_T));
		dlg.FetchValueEx(m_eqData_g.lpEQMode[m_nEQIndex]);
	}
	else
	{
		dlg.FetchValue(m_eq_data);
	}
	//
	dlg.FetchArmValue(m_VolumeData);
	
}

void CMusicModePage::RefreshControls()
{
	if(m_bSerialPara)
	{
		GetDlgItem(IDC_COMBO_POST_PROCESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_POST_PROCESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(FALSE);
	}
}

void CMusicModePage::OnSelchangeComboFormat() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	m_nOldParaSetIndex = m_nNewParaSetIndex;
	/*
	if(m_nOldParaSetIndex >=0 )
	{
		if(m_dLimit != m_limitBak)
		{
			int nValue = ceilf(((double)pow(10, m_dLimit/20.0)) * 127.0);
			if(m_bSerialPara)
			{
				m_eqData_g.lpEQMode[m_nOldParaSetIndex].eq_control = (m_eqData_g.lpEQMode[m_nOldParaSetIndex].eq_control & 0xff00) | nValue;
			}
		}
	}
	*/

	if(m_nCBFormat == 0)
	{
		m_nParaSetIndex = (m_VolumeData.app_config_info_set.aud_proc_exp_control[0] & 0x7c00) >> 10;
	}
	else if(m_nCBFormat == 1)
	{
		m_nParaSetIndex = (m_VolumeData.app_config_info_set.aud_proc_exp_control[0] & 0x03e0) >> 5;
	}
	else if(m_nCBFormat == 2)
	{
		m_nParaSetIndex = m_VolumeData.app_config_info_set.aud_proc_exp_control[0] & 0x001f;
	}
	m_nParaSetIndex --;

	m_nNewParaSetIndex = m_nParaSetIndex;
	if(m_bSerialPara)
	{
		if(m_nParaSetIndex >= m_eqData_g.eq_count || m_nParaSetIndex < 0)
		{
			AfxMessageBox(_T("Error occur, when getting para set index for EQ!"));
		}
		else
		{
			memcpy(&m_eq_data, &m_eqData_g.lpEQMode[m_nParaSetIndex], sizeof(EQ_STRUCT_G_T));
		}
	}
	
}

void CMusicModePage::OnButtonSseqGLoadCurrentModeFromFlash() 
{
	// TODO: Add your command handler code here
	CAudioTesterSheet* pSheet = (CAudioTesterSheet *)GetParent();
	if(pSheet != NULL)
	{
		
		pSheet->LoadENHAModes(TYPE_EQ, GET_ENHA_DATA_FROM_FLASH);
		pSheet->LoadENHAModes(TYPE_TUN, GET_ENHA_DATA_FROM_FLASH);

		for(int i = 0; i < m_eqData_g.eq_count; i++)
		{
			memcpy(&m_eqData_g.lpEQMode[i], &(pSheet->m_eqData_g.lpEQMode[i]), sizeof(EQ_STRUCT_G_T));
		}

		SetEQValue(m_eqData_g.lpEQMode[m_nParaSetIndex]);
		SetTunValue(pSheet->m_tunData_g.lpTunMode[m_nTunIndex]);

		InitValue(m_VolumeData);
	}
	
}

void CMusicModePage::OnButtonSseqGLoadCurrentModeFromPhone() 
{
	// TODO: Add your command handler code here
	CAudioTesterSheet* pSheet = (CAudioTesterSheet *)GetParent();
	if(pSheet != NULL)
	{
		
		pSheet->LoadENHAModes(TYPE_EQ, GET_ENHA_DATA_FROM_RAM);
		pSheet->LoadENHAModes(TYPE_TUN, GET_ENHA_DATA_FROM_RAM);
		
		for(int i = 0; i < m_eqData_g.eq_count; i++)
		{
			memcpy(&m_eqData_g.lpEQMode[i], &(pSheet->m_eqData_g.lpEQMode[i]), sizeof(EQ_STRUCT_G_T));
		}

		SetEQValue(m_eqData_g.lpEQMode[m_nParaSetIndex]);
		SetTunValue(pSheet->m_tunData_g.lpTunMode[m_nTunIndex]);

		InitValue(m_VolumeData);
	}
}

void CMusicModePage::OnButtonSseqGWriteCurrentModeToFlash() 
{
	// TODO: Add your command handler code here
	CAudioTesterSheet* pSheet = (CAudioTesterSheet *)GetParent();
	if(pSheet != NULL)
	{
		GetValue();
		//memcpy(&(pSheet->m_eqData_g.lpEQMode[m_nParaSetIndex]), &m_eqData_g.lpEQMode[m_nParaSetIndex], sizeof(EQ_STRUCT_G_T));
		memcpy(&(pSheet->m_eqData_g.lpEQMode[m_nParaSetIndex]), &m_eq_data, sizeof(EQ_STRUCT_G_T));
		memcpy(&(pSheet->m_tunData_g.lpTunMode[m_nTunIndex]), &m_tun_data, sizeof(TUN_STRUCT_G_T));
		pSheet->WriteENHAParaToPhone(m_nParaSetIndex, TYPE_EQ, SET_ENHA_DATA_TO_FLASH,0);
		pSheet->WriteENHAParaToPhone(m_nTunIndex, TYPE_TUN, SET_ENHA_DATA_TO_FLASH,0);
	}
	
}

void CMusicModePage::OnButtonSseqGWriteCurrentModeToPhone() 
{
	// TODO: Add your command handler code here
	CAudioTesterSheet* pSheet = (CAudioTesterSheet *)GetParent();
	if(pSheet != NULL)
	{
		GetValue();
		//memcpy(&(pSheet->m_eqData_g.lpEQMode[m_nParaSetIndex]), &m_eqData_g.lpEQMode[m_nParaSetIndex], sizeof(EQ_STRUCT_G_T));
		memcpy(&(pSheet->m_eqData_g.lpEQMode[m_nParaSetIndex]), &m_eq_data, sizeof(EQ_STRUCT_G_T));
		memcpy(&(pSheet->m_tunData_g.lpTunMode[m_nTunIndex]), &m_tun_data, sizeof(TUN_STRUCT_G_T));
		pSheet->WriteENHAParaToPhone(m_nParaSetIndex, TYPE_EQ, SET_ENHA_DATA_TO_RAM,m_nEQPresetsIndex);
		pSheet->WriteENHAParaToPhone(m_nTunIndex, TYPE_TUN, SET_ENHA_DATA_TO_RAM,m_nEQPresetsIndex);
	}
}
