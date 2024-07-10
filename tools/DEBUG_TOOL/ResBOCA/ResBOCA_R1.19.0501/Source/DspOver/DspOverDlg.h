// DspOverDlg.h : header file
//

#if !defined(AFX_DSPOVERDLG_H__D76DE907_DB6D_4E4A_B677_11D2F7ECE430__INCLUDED_)
#define AFX_DSPOVERDLG_H__D76DE907_DB6D_4E4A_B677_11D2F7ECE430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDspOverDlg dialog

class CDspOverDlg : public CDialog
{
// Construction
public:
	CDspOverDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDspOverDlg)
	enum { IDD = IDD_DSPOVER_DIALOG };
	CComboBox	m_cbExtTbl;
	CString	m_strFullFile;
	CString	m_strDspFile;
	CString	m_strOutFile;
	int m_nCBCmpType;
	int		m_nCmpLevel;
	BOOL	m_bOverExtPart;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDspOverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDspOverDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnOverWrite();
	afx_msg void OnBtnIputFullFile();
	afx_msg void OnUpdateEditIntegratedFile();
	afx_msg void OnUpdateEditDspFile();
	afx_msg void OnUpdateEditOutput();
	afx_msg void OnChkExtPartition();
	//}}AFX_MSG
	afx_msg void OnBtnInputFile(UINT nID);
	DECLARE_MESSAGE_MAP()
private:
	bool bDebugOn;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPOVERDLG_H__D76DE907_DB6D_4E4A_B677_11D2F7ECE430__INCLUDED_)
