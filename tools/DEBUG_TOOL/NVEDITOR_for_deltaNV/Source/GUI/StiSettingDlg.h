#if !defined(AFX_STISETTINGDLG_H__9310EF3E_293D_4079_AE08_7B26A4EF676D__INCLUDED_)
#define AFX_STISETTINGDLG_H__9310EF3E_293D_4079_AE08_7B26A4EF676D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StiSettingDlg.h : header file

/////////////////////////////////////////////////////////////////////////////
// CStiSettingDlg dialog

class CStiSettingDlg : public CDialog
{
// Construction
public:
	CStiSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStiSettingDlg)
	enum { IDD = IDD_STI_SETTING };
	CEdit	m_ctlDspFile;
	CEdit   m_ctlFixBin;
	CButton	m_ctlBrowse;
	CButton m_ctlBrowse2;
	CString	m_strDspFileName;
	CString m_strFixBinName;
	BOOL	m_bWriteDsp;
	BOOL    m_bWriteFix;
	//[[ by Joly
	BOOL       m_bActiveFiles;   // for multiple DSP file write 
	CListCtrl  m_lstWriteFile;   
	BOOL       m_bVerFlag;
	//]]
	//}}AFX_DATA

	VEC_FILEINFO	m_vecFileInfo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStiSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//[[ by Joly
	void    InitList(CListCtrl& ctrList);
	void    ClickList(NMHDR* pNMHDR,CListCtrl& ctrList);
	void    FillList();
	void    SaveList();
	void    CheckID(UINT nID,CString& strMsg);
	//]]

	// Generated message map functions
	//{{AFX_MSG(CStiSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnStiWritedsp();
	afx_msg void OnStiBrowse();
	virtual void OnOK();
	afx_msg void OnStiWriteFix();
	afx_msg void OnStiBrowse2();
	afx_msg void OnCheckWriteFiles();
	afx_msg void onClearButton();
	afx_msg void OnClickClear(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditLstDsp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStiVersionFlag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STISETTINGDLG_H__9310EF3E_293D_4079_AE08_7B26A4EF676D__INCLUDED_)
