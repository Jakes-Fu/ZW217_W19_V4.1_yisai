#if !defined(AFX_SMSCENTERDLG_H__A7E275E4_D8D1_4A5D_B933_6C41478088CD__INCLUDED_)
#define AFX_SMSCENTERDLG_H__A7E275E4_D8D1_4A5D_B933_6C41478088CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSCenterDlg.h : header file
//

#include "..\effect\CoolEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CSMSCenterDlg dialog
class ISMS;
class CSMSCenterDlg : public CDialog
{
// Construction
public:
	CSMSCenterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSMSCenterDlg)
	enum { IDD = IDD_DLG_SMSCENTER };
	CButton	m_btnCancel;
	CButton	m_btnSet;
	CString	m_strCenterNum;
	CString	m_strCenterNum2;
	CString	m_strCenterNum3;
	CString	m_strCenterNum4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMSCenterDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL SetSMSCenterNum(void);

	// Generated message map functions
	//{{AFX_MSG(CSMSCenterDlg)
	afx_msg void OnBtnSet();
	afx_msg void OnBtnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CCoolEdit m_edtCenterNum;
	CCoolEdit m_edtCenterNum2;
	CCoolEdit m_edtCenterNum3;
	CCoolEdit m_edtCenterNum4;

	ISMS * m_pSMSFactory;

	static const int CENTERNUM_MAX_LEN;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSCENTERDLG_H__A7E275E4_D8D1_4A5D_B933_6C41478088CD__INCLUDED_)
