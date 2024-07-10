#if !defined(AFX_DLGDIALNETWORK_H__DDC0E1D6_36CB_4E07_9429_2F385E0974FE__INCLUDED_)
#define AFX_DLGDIALNETWORK_H__DDC0E1D6_36CB_4E07_9429_2F385E0974FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDialNetwork.h : header file
//
const CString strSpeedUp[]=
{
	_T("8"),
	_T("16"),
	_T("32"),
	_T("64"),
	_T("128"),
	_T("384"),
	_T("512"),
	_T("1024"),
	_T("1536"),
	_T("2048"),
	_T("2240")
};

const CString strSpeedDown[]=
{
	_T("8"),
	_T("16"),
	_T("32"),
	_T("64"),
	_T("128"),
	_T("256"),
	_T("384"),
	_T("512"),
	_T("1024"),
	_T("1100"),
	_T("1536"),
	_T("1600"),
	_T("2048"),
	_T("2200"),
	_T("2240"),
	_T("2800")
};

#define NUM_SPEED_UP 11
#define NUM_SPEED_DOWN 16

/////////////////////////////////////////////////////////////////////////////
// CDlgDialNetwork dialog

class CDlgDialNetwork : public CDialog
{
// Construction
public:
	CDlgDialNetwork(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDialNetwork)
	enum { IDD = IDD_DLG_DIAL_NETWORK };
	UINT m_uDialSecond;
	UINT m_uDialNum;
	CString m_strContIn;
	CComboBox m_boxSpeedUP;
	CComboBox m_boxSpeedDown;
	CButton m_btnApply;
	CComboBox m_typeBox;
	UINT  m_uSpeedUp;
	UINT  m_uSpeedDown;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

protected:
	_TCHAR m_szFileName[_MAX_PATH];
	UINT m_uComboxIndexBak;
	UINT m_uUpSpeedBak;
	UINT m_uDownSpeedBak;
	CString m_strContInBak;
	
	CBrush  m_brshDlgBckGrnd;

public:
	void WriteIniFile();
	void ShowRespondContent(LPCSTR szRet);
	void ClearAll();
	void SendCheckRequest();

private:
	void SendSetRequest();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDialNetwork)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDialNetwork)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnNetworkApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDIALNETWORK_H__DDC0E1D6_36CB_4E07_9429_2F385E0974FE__INCLUDED_)
