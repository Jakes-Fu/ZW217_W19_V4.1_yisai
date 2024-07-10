#if !defined(AFX_DLGMAIN_H__C3CEFFF1_8671_442F_942C_3D62301A22F8__INCLUDED_)
#define AFX_DLGMAIN_H__C3CEFFF1_8671_442F_942C_3D62301A22F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMain.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMain dialog

class CDlgMain : public CDialog
{
// Construction
public:
	CDlgMain(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMain)
	enum { IDD = IDD_DLG_MAIN };
	CString	m_strAnimArrayFileName;
	CString	m_strAnimDirName;
	CString	m_strBmpDirArrayFileName;
	CString	m_strBmpDirName;
	CString	m_strBmpArrayFileName;
	CString	m_strBmpFileName;
	BOOL	m_bBigEndian;
	int		m_nCmpType;
	//}}AFX_DATA

    CString m_strBmpArrName;
    CString m_strBmpDirArrName;
    CString m_strAniArrName;

	CString	m_strBmpBinFileName;
	CString	m_strBmpDirBinPath;
	CString	m_strAnimBinFile;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMain)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnumFiles(CStringArray &arrFiles, LPCTSTR pDir);
	CString DistillFileName(LPCTSTR pPath);
	BOOL GenerateArray();

	// Generated message map functions
	//{{AFX_MSG(CDlgMain)
	afx_msg void OnBtnSelBmpFile();
	afx_msg void OnBtnSelBmpDir();
	afx_msg void OnBtnSelAnimDir();
	afx_msg void OnBtnSelBmpFileArrayFile();
	afx_msg void OnBtnSelBmpDirArrayFile();
	afx_msg void OnBtnSelAnimDirArrayFile();
	afx_msg void OnBtnGen();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbCmpImgType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:	
    static const _TCHAR SZEXTENSION[];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAIN_H__C3CEFFF1_8671_442F_942C_3D62301A22F8__INCLUDED_)
