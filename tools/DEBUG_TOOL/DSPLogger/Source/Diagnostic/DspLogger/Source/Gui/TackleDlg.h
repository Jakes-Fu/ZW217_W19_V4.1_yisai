#if !defined(AFX_TACKLEDLG_H__A081C168_E39E_4FAA_81B1_4E2CAEA0ECE1__INCLUDED_)
#define AFX_TACKLEDLG_H__A081C168_E39E_4FAA_81B1_4E2CAEA0ECE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TackleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTackleDlg dialog

class CTackleDlg : public CDialog
{
// Construction
public:
	CTackleDlg(CWnd* pParent = NULL);   // standard constructor

    long m_lIndex;

// Dialog Data
	//{{AFX_DATA(CTackleDlg)
	enum { IDD = IDD_DIALOG_TACKLE };
	CString	m_strFilter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTackleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CWnd* m_pParentWnd;
	// Generated message map functions
	//{{AFX_MSG(CTackleDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TACKLEDLG_H__A081C168_E39E_4FAA_81B1_4E2CAEA0ECE1__INCLUDED_)
