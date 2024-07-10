#if !defined(AFX_NEWPROJECT_H__BADB227D_0B85_43DB_BD76_61670193262F__INCLUDED_)
#define AFX_NEWPROJECT_H__BADB227D_0B85_43DB_BD76_61670193262F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewProject.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewProject dialog

class CNewProject : public CDialog
{
// Construction
public:
	CNewProject(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewProject)
	enum { IDD = IDD_NEW_PROJECT };
	CEdit	m_ctlPrjName;
	CEdit	m_ctlTargetFileName;
	CEdit	m_ctlPrjFileName;
	CString	m_szPrjFileName;
	CString	m_szPrjDesc;
	CString	m_szPrjName;
	CString	m_szTargetFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProject)
	protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewProject)
	virtual void OnOK();
	afx_msg void OnButtonBrowseFolder();
	afx_msg void OnButtonBrowseFolder2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECT_H__BADB227D_0B85_43DB_BD76_61670193262F__INCLUDED_)
