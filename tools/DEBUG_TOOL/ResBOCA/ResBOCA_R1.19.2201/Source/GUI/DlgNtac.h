#if !defined(AFX_DLGNTAC_H__C7D17CD5_B31B_4146_AEA1_8145867B349A__INCLUDED_)
#define AFX_DLGNTAC_H__C7D17CD5_B31B_4146_AEA1_8145867B349A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNtac.h : header file
//
#include "NAParserV2.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgNtac dialog

class CDlgNtac : public CDialog
{
// Construction
public:
	CDlgNtac(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgNtac)
	enum { IDD = IDD_DLG_NETWORK_ACCOUNT };
	CIPAddressCtrl	m_ipDns2;
	CIPAddressCtrl	m_ipDns1;
	int		m_nAccessType;
	int		m_nAccountType;
	int		m_nIPType;
	int		m_nMvnoType;
	int		m_nRoamingPro;
	CString	m_strApn;
	CString	m_strHomePage;
	CString	m_strSpn;
	UINT	m_nUiorder;
	UINT	m_nMcc;
	UINT	m_nMnc;
	CString	m_strPassword;
	UINT	m_nPort;
	CString	m_strName;
	CString	m_strUserName;
	int		m_nAccessOption;
	int		m_nAuthType,m_nAuthType_PAP,m_nAuthType_CHAP,m_nAuthType_PAPCHAP,m_nAuthType_NONE;
	BOOL	m_bActive;
	BOOL    m_bVisible;
	BOOL	m_bRoaming;
	CString	m_strPos;
	CString	m_strUsed;
	int		m_nIndex;
	CString	m_strGateWay;
	//}}AFX_DATA

	CString   m_strNTAC;
	BOOL      m_bViewOnly;

public:
	BOOL SetBuf(LPBYTE pBuf,DWORD dwSize, LPCTSTR lpszTmpPath);

	//BOOL Create(UINT nIDTemplate, CWnd* pParentWnd /* = NULL */);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNtac)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ShowCurNtac();
	void UpdateCtrlState();
	void SaveCurNtac();


	// Generated message map functions
	//{{AFX_MSG(CDlgNtac)
	virtual BOOL OnInitDialog();
	afx_msg void OnPrev();
	afx_msg void OnNext();
	afx_msg void OnChkActive();
	virtual void OnOK();
	afx_msg void OnSelchangeAccessType();
	afx_msg void OnImport();
	afx_msg void OnExport();
	afx_msg void OnGo2Pos();
	afx_msg void OnPreIstNtac();
	//}}AFX_MSG
	afx_msg void OnParsePos();
	DECLARE_MESSAGE_MAP()

private:
	CNAParserV2 m_Parser;
	int		  m_nCurIndex;
	
	CString   m_strTmpPath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNTAC_H__C7D17CD5_B31B_4146_AEA1_8145867B349A__INCLUDED_)
