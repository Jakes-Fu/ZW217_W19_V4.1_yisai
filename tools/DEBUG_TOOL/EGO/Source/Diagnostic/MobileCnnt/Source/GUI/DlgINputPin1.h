#if !defined(AFX_DLGINPUTPIN1_H__C8F656F3_3380_43A1_9AC9_FF0EE21EE04C__INCLUDED_)
#define AFX_DLGINPUTPIN1_H__C8F656F3_3380_43A1_9AC9_FF0EE21EE04C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgINputPin1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgINputPin1 dialog

class CDlgINputPin1 : public CDialog
{
// Construction
public:
	CDlgINputPin1(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CDlgINputPin1)

// Dialog Data
	//{{AFX_DATA(CDlgINputPin1)
	enum { IDD = IDD_DLG_INPUT_PIN1 };
	CButton m_btnOK;
	CButton m_btnCancel;
	CString m_strPin;
	UINT m_uRemainNum;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	void ShowRespondContent(LPCSTR szRet);

private:
	void SendCheckRequest();
	BOOL SendSetRequest();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgINputPin1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgINputPin1)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTPIN1_H__C8F656F3_3380_43A1_9AC9_FF0EE21EE04C__INCLUDED_)
