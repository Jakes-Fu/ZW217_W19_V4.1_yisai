#if !defined(AFX_MSINFOVIEW_H__047BC72F_B2A6_40B0_B774_E56641587D81__INCLUDED_)
#define AFX_MSINFOVIEW_H__047BC72F_B2A6_40B0_B774_E56641587D81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MSInfoView.h : header file
//
#include "CoolFormView.h"
typedef enum
{
	SYSTEM_SERVICE,
	SYSTEM_DOMAIN,
	SYSTEM_ROAM,
	SYSTEM_MODE,
	SYSTEM_SIM_STATUES,
	SYSTEM_CELL_SIGN
};
/////////////////////////////////////////////////////////////////////////////
// CMSInfoView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMSInfoView : public CCoolFormView
{
public:
	CMSInfoView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMSInfoView)

// Form Data
public:
	//{{AFX_DATA(CMSInfoView)
	enum { IDD = IDD_FORMVIEW_MSINFO };
	CString m_strSignal;
	CString m_strNetType;
	CString m_strNetProvider;
	CString m_strImei;

	CString m_strSysSrv;
	CString m_strDomain;
	CString m_strRoam;
	CString m_strMode;
	CString m_strSim;
	CString m_strCellSign;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	BOOL m_bSysTm;
	BOOL m_bGSM; //true:gsm; false:td
	BOOL m_bUrgency;
	BOOL m_bNoService;
	int m_nLastSimState;

// Operations
public:
	void SendCheckRequest();

	void GetSignalIntensity(LPCSTR szRet);
	void GetNetworkInfo(LPCSTR szRet);
	void GetSystemInfo(LPCSTR szRet);
	void ShowRespondContent(LPCSTR szRet);
	void ClearAll();

private:
	CString GetSysStringByIndex(DWORD dwType, UINT uIndex);
	void SetStatusSignal(CString strSignal);
	void SendSystemRequest();
	void SendSystemServiceRequest();
	void GetSystemService(LPCSTR szRet);
	void GetSystemMode(LPCTSTR szRet);
	void GetSIND(LPCTSTR szRet);
	void JudeSimInvalid(int nSimState);
	void StartupCheck();
	void StopCheck();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSInfoView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMSInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMSInfoView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSINFOVIEW_H__047BC72F_B2A6_40B0_B774_E56641587D81__INCLUDED_)
