#if !defined(AFX_CONNECTPAGE_H__0535EA3F_DA06_49B3_A628_9BF1A9F110A6__INCLUDED_)
#define AFX_CONNECTPAGE_H__0535EA3F_DA06_49B3_A628_9BF1A9F110A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConnectView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ModemInfoClass.h"
#include "CoolFormView.h"
#include "EnumPorts.h"

class CConnectView : public CCoolFormView
{
public:
	CConnectView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CConnectView)

// Form Data
public:
	//{{AFX_DATA(CConnectView)
	enum { IDD = IDD_FORMVIEW_CONNECT };
	CStatic m_stc;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	void CnntOrDisCnntDevice(BOOL bClearStatusBar = TRUE);
	BOOL CheckModem(BOOL bShowMsgBox);

	BOOL FindSerialUsbModemComm();
	BOOL FindSerialMuxComm();
	
	void Connect(BOOL bShowMsgBox = TRUE);
	void SetStcBmp(UINT uID);
	BOOL CheckAT();

// Operations
public:
	CString m_strModemName;
	DWORD   m_dwBaudRate;

private:
	CString m_strCom;
	CModemInfo m_modemInfo;
	CEnumPort m_EnumPort;
	int m_nDeviceIndex;
	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CConnectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CConnectView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTPAGE_H__0535EA3F_DA06_49B3_A628_9BF1A9F110A6__INCLUDED_)
