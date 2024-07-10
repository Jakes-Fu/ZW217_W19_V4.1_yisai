#if !defined(AFX_AUDIOTESTERSHEET_H__0CE7743A_1B50_476F_8F3F_97B47D22F7A8__INCLUDED_)
#define AFX_AUDIOTESTERSHEET_H__0CE7743A_1B50_476F_8F3F_97B47D22F7A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AudioTesterSheet.h : header file
//

#include "SettingsPage.h"
#include "AudioModePage.h"
#include <Afxtempl.h>
#include "Command.h"
#include "ColorListBox.h"

#include "CtrlStatus.h"
#include "MusicModePage.h"

#define ERROR_COLOR RGB(255,0,0 )

typedef enum
{
	MS_MODE,
	FLASH_MODE
};

/////////////////////////////////////////////////////////////////////////////
// CAudioTesterSheet

class CAudioTesterSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CAudioTesterSheet)

// Construction
public:
	CAudioTesterSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    void ChangeFont(CWnd* pWnd, CFont* pFont);
    
private:
	CAudioTesterSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
        
// Attributes
public:
    CSettingsPage m_SettingsPage;
	//teana hu 2010.06.08
	EQ_DATA m_EQData;
	BOOL m_bLoadEQ;
	
	//teana hu 2011.02.16
	BOOL m_bLoadVolume;
	VOLUME_DATA m_VolumeData;

	//teana hu 2011.03.22
	BOOL m_bLoadENHA;
	EQ_DATA_G m_eqData_g;
	TUN_DATA_G m_tunData_g;
	//
	
	//teana hu 2011.04.18
	BOOL m_bLoadLVVE;
	LVVE_CTRL_PARAM_NV_LIST_T m_LVVEPara;
	//

	UINT m_nArmSerialCount;
	
	//teana hu 2011.06.07
	//BOOL m_bLVVEMin;
	//

	//teana hu 2012.01.09
	DWORD m_dwLvveType;
	//

	//teana hu 2012.01.31
	BOOL m_bLoadDspCodec;
	AUDIO_NV_DSP_MODE_LIST_T m_DspModePara;
	short m_nDspCodecInfo;
	int m_nVolumeLevel_6530;
	BOOL m_bDspCodec;
	CString m_strVersion;
	
	//

	//teana hu 2012.05.02
	BOOL m_bExportOldNv;
	//

	//teana hu 2012.06.14
	BOOL m_bExportOldEQNv;
	BOOL m_bSerialPara;
	//
	

protected:
    CArray<CAudioModePage*,CAudioModePage*>m_arrAudioModePages;

	//teana hu 2011.03.28
	CArray<CMusicModePage*,CMusicModePage*>m_arrMusicModePages;
	//

    typedef struct 
    {
       float m_fFileVersion;
       CString m_strProducer;
       CTime m_Datetime;
       int m_nAudioDataSize;    // Audio data size in bytes
	   //teana hu 2010.06.10
	   int m_nEQDataSize;
	   //
	   //teana hu 2011.02.17
	   int m_nVolumeDataSize;
	   //
    }FILE_HEADER_INFO;
    
    FILE_HEADER_INFO m_structFileHead;

    void GetFileHeadInfo();
    void UpdatePageTitle();
	void UpdateMusicPageTitle();

// Operations
public:
    BOOL Connect( CString& strAddr,DWORD dwPort );
	BOOL ConnectUart( DWORD dwPort,DWORD dwBaudrate );
    BOOL Disconnect();

    BOOL LoadAudioModes(DWORD dwType = MS_MODE);
    void ClearAllModes();

    BOOL WriteParaToPhone( int nIndex );
    BOOL WriteAllParaToPhone();
    BOOL WriteParaToFlash( int nIndex );
    BOOL WriteDefaultFirToPhone( int nIndex );
    BOOL WriteDefaultUlFirToPhone( int nIndex );
	BOOL LoadParaFromPhone(int nIndex,BOOL bConfirm = TRUE );
	BOOL LoadParaFromFlash(int nIndex,BOOL bConfirm = TRUE );
    
    BOOL SetVolume( int nVolume );
    BOOL SetSpeaker( int nSpeaker );
    BOOL SetMute( int nMute );

    BOOL MakeCall( CString strPhoneNumber );
    BOOL Hangup();

    BOOL Save();
    BOOL Load();
    void Export( BOOL bCallDataOnly );
	//teana hu 2010.05.26
	BOOL Import();
	BOOL ImportAudio(CString strFileName);
	//
	//teana hu 2011.04.01
	void ExportMusic();
	BOOL ImportMusic(CString strFileName);
	//
    void Log( LPCTSTR lpszInfo,BOOL bErrMsg = FALSE );

    BOOL IsExtend(){ return m_bExtendData; }
    BOOL IsRocky(){ return m_bRocky; }
	BOOL IsVer3(){ return m_bAudio3; }
	
	//teana hu 2009.08.24
	BOOL SetMixModes();
	BOOL OpenMixMode();
	
	//teana hu 2010.06.08
	BOOL LoadEQModes(int nPos = 0);
	BOOL WriteEQParaToPhone( int nIndex );
    BOOL WriteEQParaToFlash( int nIndex );
	void SetLoadVar(BOOL bLoad){m_bLoad = bLoad;}
	BOOL WriteVolumeParam(int nIndex, int nType);
	BOOL GetVolumeParam(int nIndex, int nType);

	//teana hu 2012.01.31
	BOOL WriteDspCodecParam(int nIndex, int nType);
	BOOL LoadDspCodecParam(int nIndex, int nType);
	//

	int GetActiveVolumePageIndex();

	//teana hu 2011.03.21
	CAudioModePage* GetAudioModePage(int nIndex) {return m_arrAudioModePages[nIndex];}

	//teana hu 2011.03.22
	BOOL LoadENHAModes(int nType, int nPos);
	BOOL WriteENHAParaToPhone( int nIndex, int nType, int nPos, int nEQModeSelType);
	BOOL SetVolumePara(int nVolumeIndex, int nAppType);

	BOOL LoadArmAudioModes(DWORD dwType = MS_MODE);
	void ClearAllMusicModes();
	BOOL IsSurpportEnha(DWORD dwType = TYPE_EQ);
	//
	
	BOOL FetchValue( UINT nIndex );
    BOOL ConfirmLoad(CString strText = _T("handset"), BOOL bClear = TRUE);

protected:
    void FetchValue( AUDIO_DATA& audio_data );
	//teana hu 2011.02.21
	void FetchVolumeValue( VOLUME_DATA& volume_data );
	//

	//teana hu 2011.04.20
	void FetchLVVEValue( LVVE_CTRL_PARAM_NV_LIST_T& lvve_data );
	//
	
	//teana hu 2012.02.10
	void FetchDspvalue(AUDIO_NV_DSP_MODE_LIST_T& dsp_data);
	//

	//teana hu 2011.04.01
	void FetchMusicValue(VOLUME_DATA& volume_data , EQ_DATA_G& eqData, TUN_DATA_G& tunData);
	//

    void AddLogWindow();
    void ClearLog();

    void DecideAudioVersion();

	//teana hu 2011.03.28
	BOOL ConfirmLoadMusic(CString strText = _T("handset"), BOOL bClear = TRUE);
	//
	
public:
    CFont m_Font;
    CCtrlStatus m_CtrlStatus;
    
protected:
    BOOL m_bLoad;
    CCommand m_Cmd;
    CColorListBox m_wndLog;
    
    AUDIO_DATA m_AudioData;
    
    // if false,the audio data size is 88 
    // words;if true,extend data
    // is added into audio data,and total audio 
    // size if 230 words
    BOOL m_bExtendData;

    // if true,audio data size is 250 words,in this
    // case,m_bExtendData must be true;if false,audio
    // data size is decided by m_bExtendData
    BOOL m_bRocky;

	// if true,audio data size if 370 words
	BOOL m_bAudio3;
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAudioTesterSheet)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAudioTesterSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAudioTesterSheet)
	virtual BOOL OnInitDialog();
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDIOTESTERSHEET_H__0CE7743A_1B50_476F_8F3F_97B47D22F7A8__INCLUDED_)
