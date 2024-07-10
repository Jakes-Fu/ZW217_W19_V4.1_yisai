#if !defined(AFX_DLGOTHERS_H__CBAD445C_CD70_4527_AC9E_2CBA09CBC322__INCLUDED_)
#define AFX_DLGOTHERS_H__CBAD445C_CD70_4527_AC9E_2CBA09CBC322__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOthers.h : header file
//

#define INDEX_NUM 21
#define UPLMN_TYPE_NUM 2

const CString UPLMN_TYPE[] =
{
	_T("UTRAN"),
	_T("GSM")
};

typedef enum
{
	FOR_PLMN,
	FOR_AOC,
	R_VERSION
};
/////////////////////////////////////////////////////////////////////////////
// CDlgOthers dialog

class CDlgOthers : public CDialog
{
// Construction
public:
	CDlgOthers(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOthers)
	enum { IDD = IDD_DLG_OTHERS };
	CListCtrl m_lstUplmn;
	CComboBox m_boxIndex;
	CComboBox m_boxType;
	CString   m_strPlmn;
	CButton m_btnApplyPlmn;
	CButton m_btnApplyAoc;	
	CButton m_btnAOCOn;
	CButton m_btnAOCOff;
	CButton m_btnR4;
	CButton m_btnR5;
	CButton m_btnR7;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	void ShowRespondContent(LPCSTR szRet);
	void SendCheckRequest();
	void SendSetRequest(UINT uFlag);
	void ClearAll();

protected:
	int m_nUplmnIndex;
	
	CBrush  m_brshDlgBckGrnd;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOthers)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOthers)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnUplmnApply();
	afx_msg void OnBtnAocApply();
	afx_msg void OnBtnVersionApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOTHERS_H__CBAD445C_CD70_4527_AC9E_2CBA09CBC322__INCLUDED_)
