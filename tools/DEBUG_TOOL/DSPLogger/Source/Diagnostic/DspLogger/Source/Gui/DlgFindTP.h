#if !defined(AFX_DLGFINDTP_H__9DE4EF6F_9EA0_47EC_8BDC_4F88D9CD0CBD__INCLUDED_)
#define AFX_DLGFINDTP_H__9DE4EF6F_9EA0_47EC_8BDC_4F88D9CD0CBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFindTP.h : header file
//
#include "MruComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CFindTpDlg dialog

class CTestPointsView;
class CDlgFindTP : public CDialog
{
// Construction
public:
	CDlgFindTP(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFindTP)
	enum { IDD = IDD_DIALOG_FIND_TESTPOINT };
	CString	m_strAddr;
	CString	m_strData;
	CString m_strDesc;
	//}}AFX_DATA

    CMruComboBox m_cmbAddr;
    CMruComboBox m_cmbData;
	CTestPointsView* m_pView;

public:
    BOOL GetDire()
    {
        return m_bDire;
    }

    CString& GetAddress()
    {
        return m_strAddr;
    }

    CString& GetData()
    {
        return m_strData;
    }

    BOOL m_bDire;
protected:
    void LoadStrings();
    void SaveStrings();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFindTP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CDlgFindTP)
	afx_msg void OnRadioUp();
	afx_msg void OnRadioDown();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFINDTP_H__9DE4EF6F_9EA0_47EC_8BDC_4F88D9CD0CBD__INCLUDED_)
