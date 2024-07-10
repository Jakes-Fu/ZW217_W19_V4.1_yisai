#if !defined(AFX_DLSETTINGSDLG_H__B7E0E835_6899_40E5_9D9B_9F4AC1FEABD8__INCLUDED_)
#define AFX_DLSETTINGSDLG_H__B7E0E835_6899_40E5_9D9B_9F4AC1FEABD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLSettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLSettingsDlg dialog

class CDLSettingsDlg : public CDialog
{
// Construction
public:
	CDLSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLSettingsDlg)
	enum { IDD = IDD_DIALOG_DLSETTING };
	CEdit	m_edtNotice;
	CButton	m_ctlDspBrowse;
	CEdit	m_ctlDsp;
	BOOL	m_bWriteDsp;
	CString	m_strDspFileName;
	//}}AFX_DATA
//    UINT m_uBaudRate;
//    UINT m_uPort;

public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLSettingsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDspbrowse();
	afx_msg void OnCheckWritedsp();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLSETTINGSDLG_H__B7E0E835_6899_40E5_9D9B_9F4AC1FEABD8__INCLUDED_)
