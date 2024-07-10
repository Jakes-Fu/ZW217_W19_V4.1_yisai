#if !defined(AFX_DIALUPVIEW_H__99FE7694_A662_4959_94B5_ED3BB6DC87B1__INCLUDED_)
#define AFX_DIALUPVIEW_H__99FE7694_A662_4959_94B5_ED3BB6DC87B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialUpView.h : header file
//
#include "ModemInfoClass.h"
#include "CoolFormView.h"
#include <ras.h>

#define WM_NOTICE_SET_TIMER			(WM_USER + 209)
#define EVENT_CHECK_CONNECTED   914
/////////////////////////////////////////////////////////////////////////////
// CDialUpView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDialUpView : public CCoolFormView
{
public:
	CDialUpView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDialUpView)

// Form Data
public:
	//{{AFX_DATA(CDialUpView)
	enum { IDD = IDD_FORMVIEW_DIALUP };
	CComboBox	m_cmbSim;
	CButton	   m_btnHandDial;
	UINT m_uRcvData;
	UINT m_uSendData;
	float m_fConnSpeed;
	CString m_strConnSpeed;
	CString m_strConnTime;
	CString m_strModem;
	CString m_strAPN;
	CButton m_btnSet;
	CEdit m_edtAPN;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void ChangeHandBtnString(BOOL bCnnt);
	void SendDetachPS();
	void ClearAll();
	void Release();
	BOOL Dial_up();

	void GetErrorString(DWORD dwError);
	void SetStateText(UINT uID);
	void ProcessDialEvent(RASCONNSTATE rasconnstate);
	void ShowRespondContent(LPCSTR szRet);

private:
	void CheckModem();
	void ReflashItem(CObject* pHint);
	CString ConvercTStoS(CTimeSpan ts);
	BOOL ActiveCurSIM();

	BOOL InitConnect(LPCTSTR lpszModemName);
	void EnumDevices();
	BOOL CheckingConnected();
	void SendCheckRequest();
	void SendSetRequest();
	
public:
	BOOL m_bHandCnnt;

	CTime m_BeginTime;
	BOOL m_bSpdTm;

	UINT  m_upSpeed;
	UINT  m_downSpeed;

private:
	CModemInfo m_modemInfo;
	CString    m_strModemName;
	
	CBitmap m_bmpDlgBckGrnd;
	CBrush  m_brshDlgBckGrnd;

private:

	DWORD m_dwEntryInfoSize;
    DWORD m_dwDeviceInfoSize;
    LPRASENTRY m_lpRasEntry;
    LPBYTE m_lpDeviceInfo; 
	DWORD m_dwDialNum;
	DWORD m_dwActualTime;
	UINT  m_nDialTime;
	BOOL  m_bKillTimer;
	
	RASCONNSTATE m_lastState;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialUpView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

// Implementation
protected:
	virtual ~CDialUpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDialUpView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnHandDial();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeFduCmbSim();
	afx_msg LRESULT OnRasEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALUPVIEW_H__99FE7694_A662_4959_94B5_ED3BB6DC87B1__INCLUDED_)
