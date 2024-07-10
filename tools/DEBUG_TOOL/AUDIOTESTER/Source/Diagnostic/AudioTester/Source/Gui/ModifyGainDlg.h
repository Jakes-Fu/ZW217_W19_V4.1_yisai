#if !defined(AFX_MODIFYGAINDLG_H__A6452461_947F_477C_AA5B_116569423783__INCLUDED_)
#define AFX_MODIFYGAINDLG_H__A6452461_947F_477C_AA5B_116569423783__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyGainDlg.h : header file
//
#define GAIN_MIN_VALUE -96.0
#define GAIN_MAX_VALUE 17.9
/////////////////////////////////////////////////////////////////////////////
// CModifyGainDlg dialog

class SpecialRect;

class CModifyGainDlg : public CDialog
{
// Construction
public:
	CModifyGainDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModifyGainDlg)
	enum { IDD = IDD_DIALOG_MODIFY_GAIN };
	BOOL	m_bWriteImmediately;
	float	m_fGain;
	CString	m_strParaName;
	CString	m_strUnit;
	CString	m_strGain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyGainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModifyGainDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillfocusEditGain();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    SpecialRect* m_pArea;

private:
    void ChangeFont(CWnd* pWnd, CFont* pFont);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYGAINDLG_H__A6452461_947F_477C_AA5B_116569423783__INCLUDED_)
