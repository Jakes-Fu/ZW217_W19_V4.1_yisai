#if !defined(AFX_NGDLG_H__5881D503_CC2E_4C06_9B11_B654E03520B0__INCLUDED_)
#define AFX_NGDLG_H__5881D503_CC2E_4C06_9B11_B654E03520B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NGDlg.h : header file
//

class SpecialRectOfNg;
/////////////////////////////////////////////////////////////////////////////
// CNGDlg dialog

class CNGDlg : public CDialog
{
// Construction
public:
	CNGDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNGDlg)
	enum { IDD = IDD_DIALOG_NG };
	CStatic	m_stcBetaDistor;
	CStatic	m_stcAlphaDistor;
	CEdit	m_edtUlBetaDistor;
	CEdit	m_edtUlAlphaDistor;
	CButton	m_btnAdvance;
	short	m_nUlNgClkAtt;
	short	m_nUlNgClkHoldc;
	short	m_nUlNgClkWpyyA;
	short	m_nUlNgClkWpyyN;
	short	m_nUlNgPlkAtt;
	short	m_nUlNgPlkHoldc;
	short	m_nUlNgPlkWpyyA;
	short	m_nUlNgPlkWpyyN;
    short	m_nDlNgClkAtt;
    short	m_nDlNgClkHoldc;
    short	m_nDlNgClkWpyyA;
    short	m_nDlNgClkWpyyN;
    short	m_nDlNgPlkAtt;
    short	m_nDlNgPlkHoldc;
    short	m_nDlNgPlkWpyyA;
    short	m_nDlNgPlkWpyyN;
	BOOL	m_bWriteToPhone;
	short	m_nAlphaDistor;
	short	m_nBetaDistor;
	int		m_nUlNgSelect;
	int		m_nDlNgSelect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNGDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
    SpecialRectOfNg* m_pArea;
    
    BOOL m_bAdvance;

// Implementation
protected:
    void ShowAdvance();

	// Generated message map functions
	//{{AFX_MSG(CNGDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonAdvance();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NGDLG_H__5881D503_CC2E_4C06_9B11_B654E03520B0__INCLUDED_)
