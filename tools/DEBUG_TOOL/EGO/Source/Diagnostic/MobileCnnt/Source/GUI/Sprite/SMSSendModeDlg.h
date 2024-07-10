#if !defined(AFX_SMSSENDMODEDLG_H__08539E2F_9BC5_4F09_953A_F4060336A8E4__INCLUDED_)
#define AFX_SMSSENDMODEDLG_H__08539E2F_9BC5_4F09_953A_F4060336A8E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSSendModeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSMSSendModeDlg dialog

const CString SEND_MODE[] =
{
	_T("ps"),
	_T("cs"),
	_T("ps_pref"),
	_T("cs_pref"),
	_T("Unknow")
};
#define MODE_NUM 4

class CSMSSendModeDlg : public CDialog
{
// Construction
public:
	CSMSSendModeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSMSSendModeDlg)
	enum { IDD = IDD_DLG_SMS_SEND_MODE };
	CButton m_btnOK;
	CButton m_btnCancel;
	CComboBox m_modeBox;
	CString  m_strCurMode;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMSSendModeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSMSSendModeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSSENDMODEDLG_H__08539E2F_9BC5_4F09_953A_F4060336A8E4__INCLUDED_)
