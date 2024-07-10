#if !defined(AFX_DLGUNLOCKPIN2_H__9E38A550_8C7E_485E_8B2D_52CE336D136C__INCLUDED_)
#define AFX_DLGUNLOCKPIN2_H__9E38A550_8C7E_485E_8B2D_52CE336D136C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUnlockPin2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUnlockPin2 dialog

class CDlgUnlockPin2 : public CDialog
{
// Construction
public:
	CDlgUnlockPin2(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CDlgUnlockPin2)

// Dialog Data
	//{{AFX_DATA(CDlgUnlockPin2)
	enum { IDD = IDD_UNLOCK_PIN2 };
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
	//{{AFX_VIRTUAL(CDlgUnlockPin2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUnlockPin2)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUNLOCKPIN2_H__9E38A550_8C7E_485E_8B2D_52CE336D136C__INCLUDED_)
