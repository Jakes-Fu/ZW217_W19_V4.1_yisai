// DspLogger.h : main header file for the DSPLOGGER application
//
#if !defined(AFX_DSPLOGGER_H__83653D1A_D2D7_4B98_B797_B26FED063A6E__INCLUDED_)
#define AFX_DSPLOGGER_H__83653D1A_D2D7_4B98_B797_B26FED063A6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"   
#include "BasicChildFrm.h"
#include "KeyInfoDoc.h"
#include "DlgImport.h"
#include "XmlParse.h"

#define MAX_FIND_DATA_NUM  20
#define DEFAULT_FIND_DATA_NUM  16
#define MAX_CHART_WND_NUM  30

#define DO_LUA_DATA_PROCESSING 1
/////////////////////////////////////////////////////////////////////////////
// CDspLoggerApp:

class CDspLoggerApp : public CWinApp
{
public:
	CDspLoggerApp();
    ~CDspLoggerApp();

public:
    CFont*  GetAppFont();
	BOOL GetDescription(WORD wAddr, WORD wData, LPSTR lpDesc, int nMatchMode);
	void LookUpDescription(int nAddr, int nData,char* szDesCription,int nMaxLen = MAX_LUA_DESCTRIPTION_LEN);
	BOOL ParseCMDParam(int argc, TCHAR** argv);
	

public:
	//docment template
	CMultiDocTemplate * m_pTestPointDocTemplate;
    CMultiDocTemplate * m_pFindTemplate;
    CMultiDocTemplate * m_pFilterTemplate;
	CMultiDocTemplate * m_pKeyinfoTemplate;
    
	//find module Frames
    CFrameWnd         * m_pFindWnd[MAX_CHART_WND_NUM];
	CBasicChildFrame  * m_pTestPointFrame;   

	CString m_strVersion;
    CString m_strBuild;
	BOOL m_bCreateKeyInfoFrame;
	BOOL m_bSupportMultiChart;
	BOOL m_bCmdMode;
	CString m_strTpFile;
	CString m_strLogFile;

	//teana hu 2009.12.17
	CKeyInfoDoc *m_pKeyInfoDoc;
	CDlgImport m_dlgImport;
	PGRAPH_INFO_PTR		m_pGraphInfo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDspLoggerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDspLoggerApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    void CreateChildFrames();
    void GetVersion();

private:
    CFont              m_AppFont;
    HICON              m_hRssiIcon;
	CXmlParse			m_cXmlParse;

};

extern CDspLoggerApp g_theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPLOGGER_H__83653D1A_D2D7_4B98_B797_B26FED063A6E__INCLUDED_)
