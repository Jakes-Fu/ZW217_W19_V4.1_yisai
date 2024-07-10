#if !defined(AFX_DLGINPUTPUK1_H__3EFC1AEE_14F1_49D8_8664_6A9E74D20085__INCLUDED_)
#define AFX_DLGINPUTPUK1_H__3EFC1AEE_14F1_49D8_8664_6A9E74D20085__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputPuk1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputPuk1 dialog

class CDlgInputPuk1 : public CDialog
{
// Construction
public:
	CDlgInputPuk1(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CDlgInputPuk1)

// Dialog Data
	//{{AFX_DATA(CDlgInputPuk1)
	enum { IDD = IDD_DLG_INPUT_PUK1 };
	CButton m_btnOK;
	CButton m_btnCancel;
	CString m_strPin;
	CString m_strPuk;
	CString m_strPinCfm;
	UINT m_uRemainNum;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	void ShowRespondContent(LPCSTR szRet);

protected:
	void SendCheckRequest();
	BOOL SendSetRequest();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputPuk1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputPuk1)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTPUK1_H__3EFC1AEE_14F1_49D8_8664_6A9E74D20085__INCLUDED_)
