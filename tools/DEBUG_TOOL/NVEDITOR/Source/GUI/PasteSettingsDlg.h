#if !defined(AFX_PASTESETTINGSDLG_H__35049113_DDBF_4130_ACFC_1AF635EA9ADD__INCLUDED_)
#define AFX_PASTESETTINGSDLG_H__35049113_DDBF_4130_ACFC_1AF635EA9ADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasteSettingsDlg.h : header file
//

#define INVALID_ID -1
/////////////////////////////////////////////////////////////////////////////
// CPasteSettingsDlg dialog

class CPasteSettingsDlg : public CDialog
{
// Construction
public:
	CPasteSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPasteSettingsDlg)
	enum { IDD = IDD_DIALOG_PASTE_SETTINGS };
	BOOL	m_bKeepId;
	BOOL	m_bKeepName;
	int		m_nNewId;
	CString	m_strNewName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasteSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
    int GetNewId();
    CString GetNewName();

protected:

	// Generated message map functions
	//{{AFX_MSG(CPasteSettingsDlg)
	afx_msg void OnCheckKeepId();
	afx_msg void OnCheckKeepName();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASTESETTINGSDLG_H__35049113_DDBF_4130_ACFC_1AF635EA9ADD__INCLUDED_)
