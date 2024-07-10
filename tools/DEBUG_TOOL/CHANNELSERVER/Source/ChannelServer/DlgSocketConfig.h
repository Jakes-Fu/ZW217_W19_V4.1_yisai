#if !defined(AFX_DLGSOCKETCONFIG_H__05380878_B740_43A1_A324_FBA623DAF3B0__INCLUDED_)
#define AFX_DLGSOCKETCONFIG_H__05380878_B740_43A1_A324_FBA623DAF3B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSocketConfig.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgSocketConfig dialog

class CDlgSocketConfig : public CDialog
{
// Construction
public:
	CDlgSocketConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSocketConfig)
	enum { IDD = IDD_SOCKET_CONFIG };
	CIPAddressCtrl	m_ctrIP;
	UINT	m_nTmpPort;
	int		m_nTmpFlowType;
	//}}AFX_DATA

	CString m_strIP;
	DWORD   m_nPort;
	DWORD   m_nIP;
	int     m_nFlowType;
	int     m_nEndian;


	void LoadConfig(LPCTSTR pszCfgFile);
	void SaveConfig(LPCTSTR pszCfgFile);
	BOOL CheckConfig();
	void UpdateConfig();
	BOOL CheckPort();
	BOOL CheckIP();

	void ChangeDialogFont(CFont *pFont);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSocketConfig)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSocketConfig)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSOCKETCONFIG_H__05380878_B740_43A1_A324_FBA623DAF3B0__INCLUDED_)
