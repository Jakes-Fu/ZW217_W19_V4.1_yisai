#if !defined(AFX_DLGEXPORT_H__9ECAA714_6E60_41C4_B23A_34954089A244__INCLUDED_)
#define AFX_DLGEXPORT_H__9ECAA714_6E60_41C4_B23A_34954089A244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExport dialog

class CDlgExport : public CDialog
{
// Construction
public:
	CDlgExport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgExport)
	enum { IDD = IDD_DIALOG_EXPORT };
	BOOL	m_bAppendMode;
	BOOL	m_bChanged;
	BOOL	m_bReadOnly;
	CString	m_strFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExport)
	afx_msg void OnBrow();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXPORT_H__9ECAA714_6E60_41C4_B23A_34954089A244__INCLUDED_)
