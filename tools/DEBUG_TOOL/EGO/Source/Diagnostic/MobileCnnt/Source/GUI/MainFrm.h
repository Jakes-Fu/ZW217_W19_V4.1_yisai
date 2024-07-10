// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__18A8EA13_3DD1_41CC_A712_9BFDC492006B__INCLUDED_)
#define AFX_MAINFRM_H__18A8EA13_3DD1_41CC_A712_9BFDC492006B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CoolToolBar.h"
#include "CoolStatusBar.h"
#include "CoolStatic.h"

#ifndef SPRD8800
#include "CoolMenu.h"
#endif

#include "MSInfoView.h"
#include "DialUpView.h"
#include "CallView.h"
#include "EngineeringView.h"
#include "ConfigView.h"
#include "ConnectView.h"

#define WINDOW_WIDTH 605
#define WINDOW_HEIGHT 445

#define  WM_NIF_TOOL                WM_USER + 1001
#define COLOR_DISCONNECT RGB(240,0,0)
#define COLOR_CONNECT RGB(0, 160, 0)
#define COLOR_DEFAULT RGB(89,147,232)

#define PROMT_INFO_IND		1
#define ROAM_INFO_IND		2
#define NETYPE_INFO_IND		3
#define SIGNAL_INFO_IND		4
#define CONNECT_INFO_IND	5

#define WM_RCV_AUTO_REPORT		(WM_USER + 202)
#define WM_MSINFO_SET		    (WM_USER + 203)
#define WM_REFLASH_ITEM		    (WM_USER + 204)
#define WM_SET_TIMER			(WM_USER + 205)
#define WM_RCV_SIGNAL			(WM_USER + 206)
#define WM_RCV_NETWORKINFO		(WM_USER + 207)
#define WM_OPEN_PUK				(WM_USER + 208)
#define WM_GET_SYSTEM_SERVICE	(WM_USER + 209)
#define WM_CLOSE_DIALUP_WND	    (WM_USER + 210)
#define WM_CLOSE_CHILD	        (WM_USER + 211)
#define WM_DIAL_BEGIAN	        (WM_USER + 212)
#define WM_DISCONNECT_DIAL_UP	(WM_USER + 213)
#define WM_HANG_UP				(WM_USER + 214)
#define WM_STARTUP_CHECK		(WM_USER + 215)
#define WM_STOP_CHECK			(WM_USER + 216)


#define ID_EVENT_REFLASH 889
#define ID_EVENT_SYSTEM 1889
#define SYSTEM_REFLASH_TIME 30000
#define EVENT_CALL_TIMER 808

typedef enum
{
	GSM_MODE,
	TD_MODE,
	W_MODE,
	UNKNOWN
};

typedef struct _MSINFO_HINT
{
	DWORD dwData_rcv;
	DWORD dwData_send;
	DWORD dwData_connspeed;
	CTimeSpan time;
	LPTSTR lpModemString;
}MSINFO_HINT, * PMSINFO_HINT;

typedef struct _DATA_HINT
{
	DWORD dwLen;
	LPSTR lpString;
}DATA_HINT, * PDATA_HINT;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	CString GetAppTile();

public:
	void SwitchToForm(int nForm);
	void ModifyIcon(UINT uID);
	void SetStatusBarState(UINT uIndicatorsID, UINT uStrID, COLORREF clText);
	void SetStatusBarState(UINT uIndicatorsID, LPCTSTR lpszText, COLORREF clText );
	BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	void StartAutoReport();
	void SetSiganalIcon(UINT uIconID);
	void SetProviderStatus(UINT uIndex);
	void ClearAll(BOOL bClearStatusBar);
	void KillCallingTimer(BOOL bDisp = TRUE);
	CString MapNetProvider(CString strText);
	BOOL IsIMEI(LPCSTR lpString);

private:
	void GetPhoneNum(LPCSTR szRet);

//private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	
#ifdef _EGO
	CCoolStatusBar  m_wndStatusBar;
#else
	CStatusBar  m_wndStatusBar;
#endif
	
	CCoolToolBar    m_wndToolBar;
	HICON			m_hSignalIcon;
	HICON           m_hOldExeIcon;
	CMSInfoView*	m_pMSInfoView;
	CDialUpView*	m_pDialUPView;
	CCallView*		m_pCallView;
	CEngineeringView*	m_pEngineerView;
	CConfigView* m_pCfgView;
	CConnectView* m_pCnntView;
	CString m_strPhoneNum;
	BOOL    m_bDialCnnt;
	BOOL    m_bSimInvalid;


public:
	NOTIFYICONDATA m_tndTool;
	HINSTANCE m_hInstance;
#ifdef SPRD8800
	CMenu m_cMenuTool;
#else
    CCoolMenu m_cMenuTool;
#endif
	CCoolStatic m_StcLogo;
	UINT     m_uCount;
	BOOL     m_bTimer;

private:
	void AddLogoOnTB();


public:
	CToolBar * GetToolBar(void) { return &m_wndToolBar; }

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnToolOpen();
	afx_msg LRESULT OnRcvAutoReport(  WPARAM wParam, LPARAM lParam  );
	afx_msg LRESULT OnOpenPukDlg(  WPARAM wParam, LPARAM lParam  );
	afx_msg LRESULT OnDisConnectDialUP(  WPARAM wParam, LPARAM lParam  );
	afx_msg LRESULT OnNoticeSetTimer(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNifTool(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__18A8EA13_3DD1_41CC_A712_9BFDC492006B__INCLUDED_)
