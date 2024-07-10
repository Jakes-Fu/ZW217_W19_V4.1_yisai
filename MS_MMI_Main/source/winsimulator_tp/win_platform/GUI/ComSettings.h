#if !defined(AFX_COMSETTINGS_H__D5EECADC_E111_40EE_8AB9_1C00C9174349__INCLUDED_)
#define AFX_COMSETTINGS_H__D5EECADC_E111_40EE_8AB9_1C00C9174349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComSettings.h : header file
//
#define OPEN_PORT 20
#define CLOSE_PORT 21
/////////////////////////////////////////////////////////////////////////////
// CComSettings dialog

class CComSettings : public CDialog
{
// Construction
public:
	CComSettings(CWnd* pParent = NULL);   // standard constructor
    void WriteIni();
// Dialog Data
	//{{AFX_DATA(CComSettings)
	enum { IDD = IDD_DIALOG1 };
	CComboBox	m_portList;
	UINT	m_port;
	UINT	m_baud;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComSettings)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMSETTINGS_H__D5EECADC_E111_40EE_8AB9_1C00C9174349__INCLUDED_)
