// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_MAINFRM_H__9A12121C_B0BD_487F_9779_86EBA310C487__INCLUDED_)
#define AFX_MAINFRM_H__9A12121C_B0BD_487F_9779_86EBA310C487__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DspKernelExport.h"
#include "TestPointsView.h"
#include "DspCommon.h"
#include "SetComDlg.h"
#include "ExportDlg.h"
#include "ReplaySetting.h"
#include "DevHound.h"

#define WM_TP_ARRIVE (WM_USER+211)
#define WM_RESET_ALL (WM_USER+212)
#define WM_AUTO_SAVE_AS (WM_USER+213)
#define WM_SET_PROMPT_TIMER (WM_USER+214)
#define WM_KILL_PROMPT_TIMER (WM_USER+215)
#define WM_AUTO_CONNECT (WM_USER+216)
#define WM_AUTO_DISCONNECT (WM_USER+217)
#define WM_FIND_RELATED_ITEM (WM_USER+218)
#define WM_RENEW_DSPLOG (WM_USER+400)

#define MARK_LEN 128

typedef enum
{
	READ_MODE,
	WRITE_MODE
};

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();
	
public:	
	BOOL             m_bStop;
	IDspKernel     * m_pDspKernel;	
	CTestPointsView* m_pView;

    CExportDlg       m_ExportDlg;
    CSetComDlg       m_SettingsDlg;
	DWORD			 m_dwGenerateTPTime;


public:
	void Pause();
    void LoadLogFile(CString strPathName);   
    void UpdateStatusBar();

	long GetFreeDiskSpaceInKB(CString strPath);
	void SetStatusBarData();
	BOOL OpenMarkFile(WORD wOpenMode); //0: rb; 1:wb
	void WriteMarkFile(DWORD dwIndex, LPSTR lpString, DWORD dwSize);
	void LoadMarkFile();
	void CloseMarkFile();
	void SaveAllMarks();
	int GetFilesInFolderCount(CString strFolder, CString& strFileName);
	void UpdatedTitleInfo();
	BOOL ProcExport(LPCTSTR lpTPfile,LPCTSTR lpLogFile,BOOL bAddDes = FALSE);

public:
	CString m_strPlaybackFile;
	BOOL m_bPlaybackTP;
	BOOL m_bConnected;
	int  m_nLogType;

protected: 
    BOOL m_bPause; 
	//teana hu 2009.10.26
    CString m_strLeading;
	FILE *m_fpMark;


	// control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
 	char m_lfd[MAX_PATH];  //tony
	CDevHound  m_usbMoniter;
protected:
   virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnConnectDsp();
	afx_msg void OnDisconnectDsp();
	afx_msg void OnUpdateConnectDsp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisconnectDsp(CCmdUI* pCmdUI);
	afx_msg void OnClear();
	afx_msg void OnSettings();
	afx_msg void OnClose();
	afx_msg void OnStart();
	afx_msg void OnUpdateStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStop();
	afx_msg void OnUpdateStop(CCmdUI* pCmdUI);
	afx_msg void OnPause();
	afx_msg void OnUpdatePause(CCmdUI* pCmdUI);
	afx_msg void OnFileExport();
	afx_msg void OnUpdateSettings(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFindTp(CCmdUI* pCmdUI);
	afx_msg void OnFileConvert();
	afx_msg void OnFileSaveTP();
	afx_msg LRESULT OnClearState(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReflashStatusBar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveMarkFile(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPlaybackOrg();
	afx_msg void OnUpdatePlaybackOrg(CCmdUI* pCmdUI);
	afx_msg BOOL OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct );
	//}}AFX_MSG
    afx_msg LRESULT OnTpArrive( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnAutoSaveAs(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAutoConnect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAutoDisConnect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFindRelatedItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRewDSPLog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNoticeEndian(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDevHound(WPARAM wParam,LPARAM lpParam);
	DECLARE_MESSAGE_MAP()
    
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9A12121C_B0BD_487F_9779_86EBA310C487__INCLUDED_)
