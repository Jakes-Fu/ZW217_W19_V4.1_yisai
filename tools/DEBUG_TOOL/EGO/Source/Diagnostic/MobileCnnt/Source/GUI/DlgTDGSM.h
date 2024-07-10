#if !defined(AFX_DLGTDGSM_H__B14AC866_8194_45F4_AAF7_E9A96C3C945C__INCLUDED_)
#define AFX_DLGTDGSM_H__B14AC866_8194_45F4_AAF7_E9A96C3C945C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTDGSM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTDGSM dialog

class CDlgTDGSM : public CDialog
{
// Construction
public:
	CDlgTDGSM(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTDGSM)
	enum { IDD = IDD_DLG_TDGSM };
	CComboBox m_boxMode;
	CComboBox m_boxOrder;
	CButton   m_btnApply;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
public:
	void ShowRespondContent(LPCSTR szRet);
	void SendCheckRequest();
	void SendSetRequest();
	void ClearAll();

private:
	int m_nRoam;
	int m_nDomain;
	CBrush  m_brshDlgBckGrnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTDGSM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTDGSM)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnApplySysconfig();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTDGSM_H__B14AC866_8194_45F4_AAF7_E9A96C3C945C__INCLUDED_)
