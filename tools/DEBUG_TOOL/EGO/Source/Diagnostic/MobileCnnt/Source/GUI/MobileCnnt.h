// MobileCnnt.h : main header file for the MOBILECNNT application
//

#if !defined(AFX_MOBILECNNT_H__57C243F2_F07A_4793_9F5C_AEFCCE1C4061__INCLUDED_)
#define AFX_MOBILECNNT_H__57C243F2_F07A_4793_9F5C_AEFCCE1C4061__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "SMSDoc.h"
#include "SMSFrame.h"
#include "SMSView.h"
#include "Sprite\InfoWrapper.h"
#include "IPort.h"
#include "IEtion.h"

#include <ras.h>

#include "RasSpdM.h"
#include "isms.h"
#include "IPB.h"

#include "ipbg.h"
#include "IMemo.h"
#include "Sprite\phone.h"
#include "SpLog.h"

typedef IPB * PIPB;
typedef IPBG * PIPBG;
typedef IMemo * PIMEMO;

#define BK_COLOR RGB(240,240,240)
#define SMS_FRAME_HEIGHT 300

#define SIM_PLUGED_SINGLE     0
#define SIM_PLUGED_ONLY_SIM1  1
#define SIM_PLUGED_ONLY_SIM2  2
#define SIM_PLUGED_SIM1_2     3
#define SIM_PLUGED_BOTH       3
#define SIM_PLUGED_ONLY_SIM3  4
#define SIM_PLUGED_SIM1_3     5
#define SIM_PLUGED_SIM2_3     6
#define SIM_PLUGED_SIM1_2_3   7
#define SIM_PLUGED_ONLY_SIM4  8
#define SIM_PLUGED_SIM1_4     9
#define SIM_PLUGED_SIM2_4     10
#define SIM_PLUGED_SIM1_2_4   11
#define SIM_PLUGED_SIM3_4     12
#define SIM_PLUGED_SIM1_3_4   13
#define SIM_PLUGED_SIM2_3_4   14
#define SIM_PLUGED_SIM1_2_3_4 15
#define SIM_PLUGED_STATE_MAX  15

#define MAX_SIM_NUM           4
  
/////////////////////////////////////////////////////////////////////////////
// CMobileCnntApp:
// See MobileCnnt.cpp for the implementation of this class
//
class CMobileCnntApp : public CWinApp
{
public:
	CMobileCnntApp();

public:
	CMultiDocTemplate *m_pSMSTemplate;
	CSMSFrame *m_pSMSFrame;

	void CreateSMSFrame();
	

	void CreateMSInfoView();
	BOOL HangUp();

	BOOL InitWrapper();
	void ReleaseWrapper();


//move from mobilesprite

public:
	CInfoWrapper * GetInfoWrapper(void) const { return m_pInfoWrapper; }
	BOOL TestIsConnected(BOOL bShowInfo = TRUE, BOOL bTestMore = FALSE);
	BOOL InitSMSPB();
	BOOL IsPluggedSIM(int nSIMNum);


public:
	int          m_nSIMState; //SIM_PLUGED_SINGLE,etc.
	int          m_nSIMCount;   
	int          m_nMMIVer;
	ISMS       * m_pSMSFactory;
	IPB        * m_pPBFactory; 
	IPBG       * m_pPBGFactory;
	IMemo      * m_pMemoFactory;
    COleDateTimeSpan    m_tsPcPhone;
	CUIntArray   m_arrBaudrate;
	
	UINT         m_nValidPBNameLen;
	UINT         m_nValidPBMailLen;
	UINT         m_nValidPBNumLen;
	
	BOOL		 m_bMuxUsed;
	BOOL		 m_bSkinUsed;
	_TCHAR       m_szCfgPathName[_MAX_PATH];
	
	BOOL         m_bUSIM;
	
	BOOL         m_bSupportSync;     // only for ego
	BOOL         m_bSupportMemo;     // only for ego
	BOOL         m_bSupportLongSMS;  // only for ego
	
	//teana hu 2010.07.26
	BOOL		 m_bLoadAudioPage;


private:
	CInfoWrapper * m_pInfoWrapper;
	CPhone         m_Phone;
	int            m_nPortOpenNum;
	int            m_nPortOpenInternal;

public:
	
	IPort      * m_pPortMain;
	IPort	   * m_pPortSub;
	IEtion	   * m_pEtionFactory;
	_PORT_ATTRIBUTE * m_pPortAttrMain;
	_PORT_ATTRIBUTE * m_pPortAttrSub;
	_PORT_ATTRIBUTE * m_pPortAttrVModem1;
	_PORT_ATTRIBUTE * m_pPortAttrVModem2;
	_PORT_ATTRIBUTE * m_pPortAttrUsbModem;
	BOOL         m_bConnected;
//	BOOL		 m_bHangUp;  //@ hongliang: the same as m_bHangup, notice "u"and "U"
	HRASCONN m_hRasConn;
	CRasSpdM	m_RasSpd;
	BOOL     m_bHangup;
	

protected:
	BOOL CreateMSInfo(void);
	void ReleaseMSInfo(void);
	BOOL IsAdmin();
	
public:
	BOOL OpenPort();
	BOOL ClosePort();

public:
	CString m_strVersion;
    CString m_strBuild;
    void GetVersion();

public:
	
	BOOL m_bManual;
	int  m_nManualPort;
	int  m_nManualBaud;
	BOOL m_bLoadDialPage;
	//teana hu 2010.08.12
	_TCHAR m_szResDllPathName[_MAX_PATH];
	CSpLog m_log;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMobileCnntApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMobileCnntApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CMobileCnntApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOBILECNNT_H__57C243F2_F07A_4793_9F5C_AEFCCE1C4061__INCLUDED_)
