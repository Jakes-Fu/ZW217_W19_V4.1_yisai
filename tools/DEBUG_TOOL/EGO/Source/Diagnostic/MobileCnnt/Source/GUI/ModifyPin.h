#if !defined(AFX_MODIFYPIN_H__42FC710F_EF1C_4D9D_93AA_7C4A9265D35C__INCLUDED_)
#define AFX_MODIFYPIN_H__42FC710F_EF1C_4D9D_93AA_7C4A9265D35C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyPin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModifyPin dialog

class CModifyPin : public CDialog
{
// Construction
public:
	CModifyPin(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CModifyPin)

// Dialog Data
	//{{AFX_DATA(CModifyPin)
	enum { IDD = IDD_MODIFY_PIN };
	CString m_strOldPin;
	CString m_strNewPin;
	CString m_strNewPinConfirm;
	CEdit   m_edtOldPin;
	CEdit   m_edtNewPin;
	CEdit   m_edtNewPinConfirm;
	UINT	m_uRemainNum;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyPin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


public:
	BOOL m_bModifyPin1OrPin2;  //True: for pin1; False: for pin2;

public:
	BOOL SendSetRequest();
	void SendCheckRequest();
	void ShowRespondContent(LPCSTR szRet);


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModifyPin)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYPIN_H__42FC710F_EF1C_4D9D_93AA_7C4A9265D35C__INCLUDED_)
