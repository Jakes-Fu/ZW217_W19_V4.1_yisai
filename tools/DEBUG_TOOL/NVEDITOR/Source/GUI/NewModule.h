#if !defined(AFX_NEWMODULE_H__17E80110_5074_4C8B_9CA6_92B6355A008B__INCLUDED_)
#define AFX_NEWMODULE_H__17E80110_5074_4C8B_9CA6_92B6355A008B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewModule.h : header file
//

class CFileParser;

/////////////////////////////////////////////////////////////////////////////
// CNewModule dialog

class CNewModule : public CDialog
{
// Construction
public:
	void SetProject(CFileParser *pProject);
	CNewModule(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewModule)
	enum { IDD = IDD_NEW_MODULE_DLG };
	CButton	m_ctlBtnNext;
	CButton	m_ctlBtnOk;
	CButton	m_ctlLastModule;
	CEdit	m_ctlModuleFileName;
	CString	m_szModuleName;
	CString	m_szModuleDesc;
	CString	m_szModuleFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewModule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewModule)
	afx_msg void OnBtnNext();
	afx_msg void OnBtnLast();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeModuleName();
	virtual void OnOK();
	afx_msg void OnButtonBrowseFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CFileParser * m_pProject;

    INT m_nCurrModuleIndex;
	INT m_nTotalModuleCount;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWMODULE_H__17E80110_5074_4C8B_9CA6_92B6355A008B__INCLUDED_)
