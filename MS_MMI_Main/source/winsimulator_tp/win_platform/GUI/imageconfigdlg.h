#if !defined(AFX_IMAGECONFIGDLG_H__EFD4FF65_B093_4F81_90A0_36E9D5483351__INCLUDED_)
#define AFX_IMAGECONFIGDLG_H__EFD4FF65_B093_4F81_90A0_36E9D5483351__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageConfigDlg dialog

class CImageConfigDlg : public CDialog
{
// Construction
public:
	CImageConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImageConfigDlg)
	enum { IDD = IDD_IMAGE_CONFIG };
	CString	m_strIniPath;
	CString	m_strBmpPath;
	CString m_strBinPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageConfigDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImageConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOpenBmpFile();
	afx_msg void OnOpenIniFile();
	afx_msg void OnOpenBinFile(); 
	virtual void OnOK();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Functions
private:
    void RefreshHandsetImage();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGECONFIGDLG_H__EFD4FF65_B093_4F81_90A0_36E9D5483351__INCLUDED_)
