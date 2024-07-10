// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__B930B844_93DB_4E78_ADD0_A4744691C543__INCLUDED_)
#define AFX_MAINFRM_H__B930B844_93DB_4E78_ADD0_A4744691C543__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DPSrv.h"
#include "AllPropertyPage.h"
#include "ControlCenter.h"
#include <afxtempl.h>
#include "DiagChan.h"
#include "ProtoChan.h"
#include "IDevHound.h"

#define WM_RECV_DEVICE_CHANGE (WM_USER + 2012)
#define WM_PORT_RESET         (WM_USER + 2013)
#define WM_FILE_READ_OVER     (WM_USER + 2014)

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:  
	IProtocolChannel* m_pProChan;   // common protocol channel
	IProtocolChannel* m_pLogChan;   // SMP protocol channel
	ICommChannel*     m_pSvrChan;   // socket server channel

	//teana hu 2012.07.18
	//CDevHound m_devHound;
	//

	IDevHound * m_pDevHound;

// Operations
public:
   	BOOL CloseServer();
	BOOL OpenServer();

	void Log(CString &str);

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

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChansvrConfig();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateChansvrConfig(CCmdUI* pCmdUI);
	afx_msg void OnConnectMssim();
	afx_msg void OnUpdateConnectMssim(CCmdUI* pCmdUI);
	afx_msg void OnDisconnectMssim();
	afx_msg void OnLogData();
	afx_msg void OnUpdateLogData(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisconnectMssim(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDataView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssert(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT  OnNifChanSvr(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT  OnCommSockData(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT  OnDisconnectMssim2(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT  OnAssert( WPARAM,LPARAM );
	afx_msg LRESULT  OnEndianEvent( WPARAM,LPARAM );
	afx_msg LRESULT  OnRecvDeviceChange(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT  OnPortReset(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT  OnFileReadOver(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	void ChangeIcon();

public:
	CControlCenter m_CC;

	BOOL IsNeedResetPort() {return m_dlgCfg.m_PageCfg.IsNeedPortReset();}

	static DWORD WINAPI GetThreadFunc(LPVOID lpParam);
	DWORD SocketMonitorFunc();
	void StartSocketMonitor();
	void StopSocketMonitor();
	BOOL TryToOpen(IProtocolChannel *pChan, CHANNEL_ATTRIBUTE &ca);
    void OnNotify2Client(BOOL bConnect);

private:
	NOTIFYICONDATA m_tndChannelServer;
 
	HINSTANCE m_hInstance; 
    HICON     m_hIconInactive;
	HICON     m_hIconActive;
    HICON     m_hIconLower;
	HICON     m_hIconLow;
	HICON     m_hIconDown;
	HICON     m_hIconUp;
	HICON     m_hIconUpDown;
	HICON     m_hIconUpAlert;
	HICON     m_hIconUpDownAlert;
	HICON     m_hIconAssert;
    CMenu     m_cMenuChanSvr; 

    BOOL      m_bInConfigure;
	BOOL      m_bInConnectPro;  // connect protocol channel
	BOOL      m_bInConnectLog;  // connect SMP log channel
	BOOL      m_bServerActive;

	// Endian set by user
 	int m_nEndian;
	// Endian decided by lower channel
	int m_nRealEndian;

    // For ARM Assert
    BOOL m_bInAssert;

    //For Endian Converting
    //IDP*    m_pConvertor;

    BOOL              m_bEnableShowData;
    BOOL              m_bEnableLogData;

    CAllPropertyPage  m_dlgCfg;

	DWORD m_dwDownData;
	DWORD m_dwUpData;
	DWORD m_dwTickcount;	

	DWORD m_dwRcvThreadId;

	//BOOL  m_bSocketTimerActive;   // flag to mark the socket timer state
	CRITICAL_SECTION m_csConnect; // sync for OnConnectMSSim
	CRITICAL_SECTION m_csDisconnect; // sync for OnDisconnectMSSim
	HANDLE m_hSocketMonitor;
	BOOL   m_bSocketMonitor;

	CSpLog  m_spLog;
}; 

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__B930B844_93DB_4E78_ADD0_A4744691C543__INCLUDED_)
