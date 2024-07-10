#if !defined(AFX_DLGSELECTNETWORK_H__C3BA5722_C249_41CC_9EC7_DD4EDEEF8B00__INCLUDED_)
#define AFX_DLGSELECTNETWORK_H__C3BA5722_C249_41CC_9EC7_DD4EDEEF8B00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectNetwork.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectNetwork dialog

class CDlgSelectNetwork : public CDialog
{
// Construction
public:
	CDlgSelectNetwork(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CDlgSelectNetwork)

// Dialog Data
	//{{AFX_DATA(CDlgSelectNetwork)
	enum { IDD = IDD_DLG_SELECT_NETWORK };
	CComboBox   m_boxMode;
	CButton		m_btnModifyMode;
	CString     m_strCurMode;
	CString     m_strPlmn;
	CListBox	m_lstPlmn;
	CString		m_strInfo;
	CButton     m_btnApply;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


private:
	BOOL m_bSearchPlmn;
	CStringArray m_strArray;
	BOOL m_bApply;
	CBrush  m_brshDlgBckGrnd;


public:
	void CheckPLMN();
	void SendCheckRequest();
	BOOL GetInfo(LPCSTR szRet);
	void ShowRespondContent(LPCSTR szRet);
	CString GetNetWorkMode(UINT uID);
	void ChangeIdToString(int nSign);
	void ClearAll();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectNetwork)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectNetwork)
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckModify();
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnBtdSelNetApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTNETWORK_H__C3BA5722_C249_41CC_9EC7_DD4EDEEF8B00__INCLUDED_)
