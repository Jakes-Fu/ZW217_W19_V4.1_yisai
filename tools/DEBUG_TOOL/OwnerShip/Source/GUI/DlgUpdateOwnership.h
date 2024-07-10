#if !defined(AFX_DLGUPDATEOWNERSHIP_H__9CB1E574_A07F_4039_B367_4B05C585E398__INCLUDED_)
#define AFX_DLGUPDATEOWNERSHIP_H__9CB1E574_A07F_4039_B367_4B05C585E398__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUpdateOwnership.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdateOwnership dialog

class CDlgUpdateOwnership : public CDialog
{
// Construction
public:
	CDlgUpdateOwnership(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUpdateOwnership)
	enum { IDD = IDD_DLG_UPDATE_OWNERSHIP };
	CComboBox	m_cmbCities;
	int		m_nCityID;
	CString	m_strEndNumber;
	CString	m_strStartNumber;
	//}}AFX_DATA

	CStringArray * m_pagCities;
	UINT    m_nMaxEndNum;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUpdateOwnership)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUpdateOwnership)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbCity();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUPDATEOWNERSHIP_H__9CB1E574_A07F_4039_B367_4B05C585E398__INCLUDED_)
