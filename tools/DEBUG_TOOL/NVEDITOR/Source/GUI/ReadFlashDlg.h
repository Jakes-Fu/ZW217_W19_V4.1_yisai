#if !defined(AFX_READFLASHDLG_H__0AECA688_1824_4937_A483_FCEDB1B10CAF__INCLUDED_)
#define AFX_READFLASHDLG_H__0AECA688_1824_4937_A483_FCEDB1B10CAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReadFlashDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReadFlashDlg dialog

class CReadFlashDlg : public CDialog
{
// Construction
public:
	CReadFlashDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReadFlashDlg)
	enum { IDD = IDD_READ_FLASH };
	CEdit	m_ctlSize;
	CEdit	m_ctlAddr;
	CString	m_strFilePath;
	//}}AFX_DATA
	DWORD m_dwStartAddr;
	DWORD m_dwBlockSize;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReadFlashDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReadFlashDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READFLASHDLG_H__0AECA688_1824_4937_A483_FCEDB1B10CAF__INCLUDED_)
