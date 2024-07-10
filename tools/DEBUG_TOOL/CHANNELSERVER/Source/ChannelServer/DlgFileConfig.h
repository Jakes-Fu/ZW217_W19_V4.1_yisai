#if !defined(AFX_DLGFILECONFIG_H__644A372E_FA17_440D_9589_00035882844F__INCLUDED_)
#define AFX_DLGFILECONFIG_H__644A372E_FA17_440D_9589_00035882844F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFileConfig.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgFileConfig dialog

class CDlgFileConfig : public CDialog
{
// Construction
public:
	CDlgFileConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFileConfig)
	enum { IDD = IDD_FILE_CONFIG };
	CString	m_strTmpFiles;
	int		m_nTmpFlowType;
	//}}AFX_DATA

	CString m_strFiles;
	int     m_nFlowType;
	int     m_nEndian;

	CString m_strMergedFile;
	CString m_strCurFile;

	void LoadConfig(LPCTSTR pszCfgFile);
	void SaveConfig(LPCTSTR pszCfgFile);
	BOOL CheckConfig();
	void UpdateConfig();
	BOOL MergeChannelFiles();	
	void DeleteMergedFile();
	LPCTSTR GetCurFile(int nIndex);
	int GetFlowType();

	void ChangeDialogFont(CFont *pFont);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFileConfig)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDlgFileConfig)
	afx_msg void OnBtnBrw();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFILECONFIG_H__644A372E_FA17_440D_9589_00035882844F__INCLUDED_)
