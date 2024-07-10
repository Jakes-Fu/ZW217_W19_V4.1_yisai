#if !defined(AFX_DLGHSDPAVERSION_H__3EBD2598_5EE4_401F_91C2_EEBC72D8ACB3__INCLUDED_)
#define AFX_DLGHSDPAVERSION_H__3EBD2598_5EE4_401F_91C2_EEBC72D8ACB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHsdpaVersion.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHsdpaVersion dialog

class CDlgHsdpaVersion : public CDialog
{
// Construction
public:
	CDlgHsdpaVersion(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CDlgHsdpaVersion)

// Dialog Data
	//{{AFX_DATA(CDlgHsdpaVersion)
	enum { IDD = IDD_DLG_HSDPA_VERSION };
	CString m_strVersion;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	void ShowRespondContent (LPCSTR szRet);

protected:
	void SendCheckRequest();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHsdpaVersion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHsdpaVersion)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHSDPAVERSION_H__3EBD2598_5EE4_401F_91C2_EEBC72D8ACB3__INCLUDED_)
