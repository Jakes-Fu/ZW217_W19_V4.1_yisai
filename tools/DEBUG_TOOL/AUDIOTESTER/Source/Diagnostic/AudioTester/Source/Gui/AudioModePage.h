#if !defined(AFX_AUDIOMODEPAGE_H__7E67F528_8DD9_4996_B40A_40D5480C3F79__INCLUDED_)
#define AFX_AUDIOMODEPAGE_H__7E67F528_8DD9_4996_B40A_40D5480C3F79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AudioModePage.h : header file
//

#include "BMPCtrlWithSpecRect.h"
#include "OtherPicFormatToBmp.h"
/////////////////////////////////////////////////////////////////////////////
// CAudioModePage dialog

class CAudioModePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAudioModePage)

// Construction
public:
	CAudioModePage();
	~CAudioModePage();

// Dialog Data
	//{{AFX_DATA(CAudioModePage)
	enum { IDD = IDD_DIALOG_AUDIO_MODE };
	CStatic	m_edtSidetone;
	CString	m_strVolume;
	CString	m_strSidetoneGain;
	CString	m_strAgcSwitch;
	int		m_nSideToneCp;
	int		m_nSideToneEq;
	int		m_nMp3AgcSwitch;
	int		m_nMidiAgcSwitch;
	int		m_nMp4AgcSwitch;
	int		m_nVoiceAgcSwitch;
	int		m_nMp3LcfSwitch;
	int		m_nMidiLcfSwitch;
	int		m_nMp4LcfSwitch;
	int		m_nVoiceLcfSwitch;
	int		m_nMp3AgcZCSwitch;
	int		m_nMidiAgcZCSwitch;
	int		m_nMp4AgcZCSwitch;
	int		m_nVoiceAgcZCSwitch;
	double		m_dMp3AgcStartGain;
	double		m_dMidiAgcStartGain;
	double		m_dMp4AgcStartGain;
	double		m_dVoiceAgcStartGain;
	double		m_dMp3AgcInputGain;
	double		m_dMidiAgcInputGain;
	double		m_dMp4AgcInputGain;
	double		m_dVoiceAgcInputGain;
	int		m_nDigitalGain;
	int		m_nEQSelect;
	int		m_nSamplingRate;
	int		m_nLowcutOffFrequency;
	int		m_nBoostingGain;
	int		m_nClarityLevel;
	int		m_nVolumeType;
	CComboBox m_boxVolumeTYpe;
	int		m_nCBChannelSelLeft;
	int		m_nCBChannelSelRight;
	int		m_nPolarLeft;
	int		m_nPolarRight;
	int		m_nAgcAttack;
	int		m_nAgcRelease;
	int		m_nAgcHoldHc;
	int		m_nAgcSdelay;
	int		m_nLcfPosition;
	int		m_bMixerSw;
	//}}AFX_DATA

protected:
	/*
    static void InitResouces();
    static std::vector<HBITMAP>m_vResource;
    static BOOL m_bInitResource;
    static COtherPicFormatToBMP m_jpgToBmp;
	*/

	double	m_dMp3AgcInputGainBak;
	double	m_dMidiAgcInputGainBak;
	double	m_dMp4AgcInputGainBak;
	double	m_dVoiceAgcInputGainBak;

	double	m_dMp3AgcStartGainBak;
	double	m_dMidiAgcStartGainBak;
	double	m_dMp4AgcStartGainBak;
	double	m_dVoiceAgcStartGainBak;

    DWORD m_dwDADevice;
    DWORD m_dwADDevice;
    DWORD m_dwUlPgaGain;
    DWORD m_dwDlPgaGain;

    int m_nSideTone;
    int m_nAec;
    int m_nVolume;
    int m_nSampleRate;

    BYTE m_ExtendData[(EXTEND_DATA_SIZE+DSP_VOLUME_SIZE+ARM_VOLUME_SIZE)*2];

    CString m_strTitle;

    CToolBar m_toolBarAudio;

	//teana hu 2012.01.31
	DWORD m_dwAdcSet;
	DWORD m_dwDacSet;
	DWORD m_dwPathSet;
	BOOL m_bDspCodecInfo; 
	
	//teana hu 2011.02.21
	double	m_arrMp3AgcInputGain[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrMidiAgcInputGain[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrMp4AgcInputGain[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrVoiceAgcInputGain[AUDIO_ARM_APP_TYPE_MAX];

	double	m_arrMp3AgcInputGainBak[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrMidiAgcInputGainBak[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrMp4AgcInputGainBak[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrVoiceAgcInputGainBak[AUDIO_ARM_APP_TYPE_MAX];

	double	m_arrMp3AgcStartGain[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrMidiAgcStartGain[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrMp4AgcStartGain[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrVoiceAgcStartGain[AUDIO_ARM_APP_TYPE_MAX];
	
	double	m_arrMp3AgcStartGainBak[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrMidiAgcStartGainBak[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrMp4AgcStartGainBak[AUDIO_ARM_APP_TYPE_MAX];
	double	m_arrVoiceAgcStartGainBak[AUDIO_ARM_APP_TYPE_MAX];

	int m_nOldIndex;
	int m_nNewIndex;

	int m_nVolumeCount;

	//

	//teana hu 2011.04.18
	LVVE_CTRL_PARAM_NV_T m_lvve_data;
	//

	//teana hu 2011.04.22
	WORD m_wNXPBoostCtl;
	WORD m_wDumpPcmCtl;
	//

	//teana hu 2011.05.09
	int m_nVolumeLevel;
	//

	int		m_nChannelSelLeft;
	int		m_nChannelSelRight;

	//teana hu 2012.07.02
	DWORD m_dwCNGGain;
	DWORD m_dwCNGSW;
	//
    
public:
    CBMPCtrlWithSpecRect m_bmpDspSw;
    CBMPCtrlWithSpecRect m_bmpDspChipDA;
    CBMPCtrlWithSpecRect m_bmpDspChipAD;
    CBMPCtrlWithSpecRect m_bmpDspHwMain;
    CBMPCtrlWithSpecRect m_bmpDspHwAssi;
    CBMPCtrlWithSpecRect m_bmpDspHwLR;
    CBMPCtrlWithSpecRect m_bmpDspHwUl;

    int m_nSampleRateValue;

	//teana hu 2011.04.18
	BOOL m_bExistLVVE;
	//

	//teana hu 2012.01.31
	BOOL m_bExistDspCodec;
	//
    

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAudioModePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAudioModePage)
	afx_msg void OnButtonAdDevice();
	afx_msg void OnButtonDaDevice();
	afx_msg void OnButtonDlPgaGain();
	afx_msg void OnButtonUlPgaGain();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonVolumeCtrl();
	afx_msg void OnButtonDlDigitalGain();
	afx_msg void OnButtonUlDigitalGain();
	afx_msg void OnButtonDlEq();
	afx_msg void OnButtonUlEq();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeComboSidetone();
	afx_msg void OnSelchangeComboVolumeMode();
	afx_msg void OnSelchangeComboAec();
	afx_msg void OnSelchangeComboSamplerate();
	afx_msg void OnButtonSidetoneGain();
	afx_msg void OnButtonAgc();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonAudioWriteCurrentModeToPhone();
	afx_msg void OnButtonAudioWriteToPhone();
	afx_msg void OnButtonPop();
	afx_msg void OnButtonNg();
	afx_msg void OnButtonAec();
	afx_msg void OnSelchangeComboSpeaker();
	afx_msg void OnSelchangeComboMute();
	afx_msg void OnButtonExtendData();
	afx_msg void OnButtonDlFirEqCoeff();
	afx_msg void OnButtonStFirEqCoeff();
	afx_msg void OnButtonUlFirEqCoeff();
	afx_msg void OnButtonAudioWriteCurrentModeToFlash();
	afx_msg void OnButtonSetDefaultFir();
	afx_msg void OnButtonMoreParameter2();
	afx_msg void OnButtonMoreParameter3();
	afx_msg void OnButtonSetDefaultUlFir();
	afx_msg void OnButtonSidetoneNg();
	afx_msg void OnButtonVolume();
	afx_msg void OnBtnEqSet();
	afx_msg void OnButtonAudioLoadCurrentModeFromFlash();
	afx_msg void OnButtonAudioLoadCurrentModeFromPhone();
	afx_msg void OnBtnVolumeSet();
	afx_msg void OnSelchangeComboSidetoneEq();
	afx_msg void OnSelchangeComboSidetoneCp();
	afx_msg void OnSelchangeComboVolumeType();
	afx_msg void OnButtonNxpTxParameter();
	afx_msg void OnButtonNxpRxParameter();
	afx_msg void OnButtonNxpVoiceBoostCtl();
	afx_msg void OnButtonDumpPcmCtl();
	afx_msg void OnBtnShowGraph();
	afx_msg void OnSelchangeComboEqSelect();
	afx_msg void OnButtonPathSet();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnButtonCng();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    SpecialRect::AREA_OF_MAP m_vAllAreas;

public:
    void WriteToFile( CArchive& ofFile );
    void LoadFromFile( CArchive& ofFile );
    BOOL FetchValue( AUDIO_MODE& value );
    void SetAudioData( AUDIO_STRUCT& audio_data,void* pSheet );
    void SetDefaultFir( AUDIO_MODE& value );
	void SetDefaultUlFir( AUDIO_MODE& value );

    void GetTitle( CString& strTitle)
    {
        strTitle = m_strTitle;
    }

    void SetTitle( CString& strTitle )
    {
        m_strTitle = strTitle;
    }

    BOOL m_bActive;
	
	//teana hu 2011.02.16
	BOOL FetchVolumeValue( VOLUME_MODE& value );
    void SetVolumeData( VOLUME_STRUCT& volume_data,void* pSheet );
	//

	SpecialRect* FindAreadByName( CString strName );

	//teana hu 2011.04.18
	void SetLVVEData(LVVE_CTRL_PARAM_NV_T& lvve_data);
	void FetchLVVEData(LVVE_CTRL_PARAM_NV_T& lvve_data);
	//

	//teana hu 2012.01.31
	void SetDspData(AUDIO_NV_DSP_MODE_STRUCT_T& dsp_data, int nLevel);
	void FetchDspData(AUDIO_NV_DSP_MODE_STRUCT_T& dsp_data);
	void SetDspCodecFlag(BOOL bDspCodecInfo){m_bDspCodecInfo = bDspCodecInfo;}
	void SetSmartPhoneFlag(BOOL bSmartPhone){m_bSmartPhone = bSmartPhone;}
	//

protected:
    BOOL m_bFir;
	//teana hu 2012.07.10
	BOOL m_bNewMoreBandDL;
	BOOL m_bNewMoreBandUL;
	//
    
    // if true,aec parameter should be used 
    // as xagc parameter
    BOOL m_bXagcEnable;

	BOOL m_bAudio3Switch;
	BOOL m_bVolumeSwitch;
	
	//teana hu 2011.02.14
	BOOL m_bAudio4;

	int m_nSoundEqVer;

	//teana hu 2012.06.12
	BOOL m_bSmartPhone;
	//

private:
    //init all rect area position which onto the related bmp
    void InitAreas( void* pPara);
    void InitToolbar();
    void ClearAreas();
    void RefreshAgcCtrl( BOOL bSwitch );
	
	void EnableSoundEffectsWindow(BOOL bEnable);
	void ShowSoundEffectsWindow(int nCmdShow);
	void RefreshControls();

	int FindEqMode();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDIOMODEPAGE_H__7E67F528_8DD9_4996_B40A_40D5480C3F79__INCLUDED_)
