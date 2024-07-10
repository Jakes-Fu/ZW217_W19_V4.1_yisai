#if !defined(AFX_DLGBKMK_H__825D73E4_DB9F_4D04_8434_EF03C426574B__INCLUDED_)
#define AFX_DLGBKMK_H__825D73E4_DB9F_4D04_8434_EF03C426574B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBkmk.h : header file
//

#include "BMParser.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBkmk dialog

class CDlgBkmk : public CDialog
{
// Construction
public:
	CDlgBkmk(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBkmk)
	enum { IDD = IDD_DLG_BOOKMARK };
	CString	m_strPos;
	CString	m_strUsed;
	UINT	m_nMcc[10];
	UINT	m_nMnc[10];

	CString	m_strName1;
	CString	m_strName10;
	CString	m_strName2;
	CString	m_strName3;
	CString	m_strName4;
	CString	m_strName5;
	CString	m_strName6;
	CString	m_strName7;
	CString	m_strName8;
	CString	m_strName9;
	CString	m_strURL1;
	CString	m_strURL10;
	CString	m_strURL2;
	CString	m_strURL3;
	CString	m_strURL4;
	CString	m_strURL5;
	CString	m_strURL6;
	CString	m_strURL7;
	CString	m_strURL8;
	CString	m_strURL9;
	//}}AFX_DATA


	CString   m_strBKMK;
	BOOL      m_bViewOnly;

public:
	BOOL SetBuf(LPBYTE pBuf,DWORD dwSize, LPCTSTR lpszTmpPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBkmk)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ShowCurBkmk();
	void UpdateCtrlState();
	void SaveCurBkmk();
	BOOL CheckValid();
	void OnCheck(UINT nID);
	// Generated message map functions
	//{{AFX_MSG(CDlgBkmk)
	virtual BOOL OnInitDialog();
	afx_msg void OnPrev();
	afx_msg void OnNext();
	afx_msg void OnImport();
	afx_msg void OnExport();
	virtual void OnOK();
	afx_msg void OnChkState1();
	afx_msg void OnChkState2();
	afx_msg void OnChkState3();
	afx_msg void OnChkState4();
	afx_msg void OnChkState5();
	afx_msg void OnChkState6();
	afx_msg void OnChkState7();
	afx_msg void OnChkState8();
	afx_msg void OnChkState9();
	afx_msg void OnChkState10();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBMParser  m_Parser;
	CString    m_strTmpPath;
	int		   m_nCurIndex;
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBKMK_H__825D73E4_DB9F_4D04_8434_EF03C426574B__INCLUDED_)
