#if !defined(AFX_MUSICMODEPAGE_H__7E610EFF_3137_490D_AA83_1555E2B20ACF__INCLUDED_)
#define AFX_MUSICMODEPAGE_H__7E610EFF_3137_490D_AA83_1555E2B20ACF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MusicModePage.h : header file
//
#include "typedef.h"
#include "ListCtrlEx.h"
#include "BMPCtrlWithSpecRect.h"


#define ERROR_INDEX 255
#define NUM_FORMAT 3
const CString strFormat[]=
{
	_T("mp3"),
		_T("midi"),
		_T("amr"),
		_T("wav"),
		_T("aac"),
		_T("wma"),
		_T("dra"),
		_T("other")
};

const int g_nSampleRate[]={44100,22050,8000,44100,44100,44100,44100,44100};

/////////////////////////////////////////////////////////////////////////////
// CMusicModePage dialog
class SpecialRectOfArmVolume;
class CMusicModePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMusicModePage)

// Construction
public:
	CMusicModePage();
	~CMusicModePage();

// Dialog Data
	//{{AFX_DATA(CMusicModePage)
	enum { IDD = IDD_DIALOG_EQUALIZER };
		// NOTE - ClassWizard will add data members here.
	CListBox m_lstDeviceType;
	int m_nCBInputSelect;
	int m_nCBOutputSelect;
	int m_nCBDa0Reverse;
	int m_nCBDa1Reverse;
	int m_nCBStereoOutput;
	int m_nCBSpeakerAmp;
	int m_nCBHeadsetAmp;
	int m_nCBMixerInput;
	int m_nCBEqSel;
	float m_dLimit;
	CListBox m_lstAppType;
	CString m_strAppType;
	CString m_strNote;
	float m_dMp3Gain;
	float m_dMidiGain;
	float m_dAmrGain;
	CListCtrlEx m_lstVolume;
	int m_nArmRecord;
	int m_nLineinRecord;
	int m_nCBPostProcess;
	CComboBox m_boxFormat;
	int m_nCBFormat;
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

	SpecialRect::AREA_OF_MAP m_vAllAreas;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMusicModePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL


public:
	void SetPageIndex(int nIndex) {m_nPageIndex = nIndex;}
	void SetValue( VOLUME_STRUCT& volume_data );
	void FetchValue( VOLUME_STRUCT& volume_data );
	void SetEQValue( EQ_STRUCT_G_T eq_data);
	void SetTunValue( TUN_STRUCT_G_T tun_data);
	void FetchENHAValue( EQ_STRUCT_G_T eq_data, TUN_STRUCT_G_T tun_data);
	void FetchParaSetIndex(int &nIndex){nIndex = m_nParaSetIndex;}
		
	void SetEQIndex(int nEQIndex){m_nEQIndex = nEQIndex; }
	void SetTunIndex(int nTunIndex){m_nTunIndex = nTunIndex;}
		
	void GetTitle( CString& strTitle)
	{
		strTitle = m_strTitle;
	}
		
	void SetTitle( CString& strTitle )
	{
		m_strTitle = strTitle;
	}

	//teana hu 2012.02.10
	void SetDspCodecFlag(BOOL bDspCodecInfo){m_bDspCodecInfo = bDspCodecInfo;};
	//

	SpecialRect* FindAreadByName( CString strName );
		
private:
	CString GetDeviceType(int nIndex);
	CString GetEQSel(int nIndex);
	int GetEQSelValue(CString strEQSel);
	void GetValue();
	void InitValue(VOLUME_STRUCT& volume_data);
	void SetVolumeList();

	void InitAreas( void* pPara);

	void RefreshControls();
		
private:
	CToolBar m_toolBarMusic;
	CToolBar m_toolBarEQ;
		
	int m_nValidDevSetCount;
	VOLUME_STRUCT m_VolumeData;
		
	int m_nOldIndex;
	int m_nNewIndex;
		
	float m_dMp3GainBak;
	float m_dMidiGainBak;
	float m_dAmrGainBak;
	float m_limitBak;
		
	int m_nPageIndex;
		
	CString m_strTitle;
		
	EQ_STRUCT_G_T m_eq_data;
	TUN_STRUCT_G_T m_tun_data;
	int m_nEQIndex;
	int m_nTunIndex;
	//teana hu 2012.02.01
	BOOL m_bDspCodecInfo;
	BOOL m_bInit;
	//

	//teana hu 2012.06.11
	BOOL m_bSmartPhone;
	//

	//teana hu 2012.06.14
	BOOL m_bSerialPara;
	EQ_DATA_G m_eqData_g;
	int m_nParaSetIndex;
	int m_nOldParaSetIndex;
	int m_nNewParaSetIndex;
	int m_nEQPresetsIndex;
	//
		
		///for volume set
public:
	void InitListCtrl();
	int CalcVdg ( double& vdg, short volume );
	int CalcP2 ( double& p2, unsigned short volume );
	int CalcP1 ( double& p1, unsigned short volume );

	int CalcVdg_l ( double& vdg, LONG volume );
	int CalcP1_l ( double& p1, LONG volume );
	int CalcP2_l ( double& p2, LONG volume );
	
	//teana hu 2012.07.06
	int CalcVdg_bths(double& vdg, short volume );
	//

	//teana hu 2012.06.12
	int CalcPA( double& pa, LONG volume );
	//
		
private:
	void InitToolbar();
		
private:
	void SetVolume(int n);
	BOOL WriteVolumeToPhone();
		
protected:
	typedef CListCtrlEx::COLUMN_DATA LIST_COLUME;
	typedef std::vector<LIST_COLUME>ROW_DATA;   
	typedef std::vector<ROW_DATA> LINE_DATA;
	LINE_DATA m_vAllLinesData;
		
	static const float vdg_dB_array[128];
	static const short vdg_value_array[128];
	static const float p2_dB_array[32];
	static const short p2_value_array[32];
	static const float p1_dB_array[16];
	static const short p1_value_array[16];

	static const float p1_dB_handsfree_array[16];	
	static const float p1_dB_handset_array[16];	
	static const float p1_dB_headset_array[16];	
	static const float p1_dB_headfree_array[16];	
	static const float p2_dB_headfree_array[32];	
	static const short p_value_6530_array[16];
	static const short pa_value_handsfree_array[16];
	static const float pa_dB_handsfree_array[16];

	static const float vdg_bths_dB_array[97];

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMusicModePage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnLcfSet();
	afx_msg void OnBtnEqSet();
	afx_msg void OnBtnAlcSet();
	afx_msg void OnButtonApply();
	afx_msg void OnSelchangeListDeviceType();
	afx_msg void OnSelchangeListAppType();
	afx_msg void OnButtonMusicLoadCurrentModeFromFlash();
	afx_msg void OnButtonMusicLoadCurrentModeFromPhone();
	afx_msg void OnButtonMusicWriteCurrentModeToFlash();
	afx_msg void OnButtonMusicWriteCurrentModeToPhone();
	afx_msg void OnButtonLineinGain();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnBtnRecordDb();
	afx_msg void OnSelchangeComboFormat();
	afx_msg void OnButtonSseqGLoadCurrentModeFromFlash();
	afx_msg void OnButtonSseqGLoadCurrentModeFromPhone();
	afx_msg void OnButtonSseqGWriteCurrentModeToFlash();
	afx_msg void OnButtonSseqGWriteCurrentModeToPhone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUSICMODEPAGE_H__7E610EFF_3137_490D_AA83_1555E2B20ACF__INCLUDED_)
