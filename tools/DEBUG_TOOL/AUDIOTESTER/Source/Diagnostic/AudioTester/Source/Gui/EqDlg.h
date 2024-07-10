#if !defined(AFX_EQDLG_H__2BD4A656_5EB8_4F1C_B111_58272261832D__INCLUDED_)
#define AFX_EQDLG_H__2BD4A656_5EB8_4F1C_B111_58272261832D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EqDlg.h : header file
//

#include "EQGraphStatic.h"
#include "NumericEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CEqDlg dialog

class SpecialRectOfEQ;

class CEqDlg : public CDialog
{
// Construction
public:
	CEqDlg(CWnd* pParent = NULL);   // standard constructor
    ~CEqDlg();

    SpecialRectOfEQ* m_pArea;
    SpecialRectOfEQ* m_pTempArea;

// Dialog Data
	//{{AFX_DATA(CEqDlg)
	enum { IDD = IDD_DIALOG_EQ };
	CNumericEdit	m_edtMidGama;
	CNumericEdit	m_edtMidGain;
	CNumericEdit	m_edtMidBeta;
	CNumericEdit	m_edtMidAlpha;
	CNumericEdit	m_edtTrebleGama;
	CNumericEdit	m_edtTrebleGain;
	CNumericEdit	m_edtTrebleBeta;
	CNumericEdit	m_edtTrebleAlpha;
	CNumericEdit	m_edtBassGama;
	CNumericEdit	m_edtBassGain;
	CNumericEdit	m_edtBassBeta;
	CNumericEdit	m_edtBassAlpha;
	CEqGraphStatic	m_eqGraph;
	double	m_dDbBass;
	double	m_dDbTreble;
	double	m_dDfBass;
	double	m_dDfTreble;
	double	m_dF0Bass;
	double	m_dF0Treble;
	double	m_dGainBass;
	double	m_dGainTreble;
	CString	m_strBassAlpha;
	CString	m_strBassBeta;
	CString	m_strBassGain;
	CString	m_strBassGama;
	CString	m_strTrebleAlpha;
	CString	m_strTrebleBeta;
	CString	m_strTrebleGain;
	CString	m_strTrebleGama;
	double	m_dDbMid;
	double	m_dDfMid;
	double	m_dF0Mid;
	double	m_dGainMid;
	CString	m_strMidAlpha;
	CString	m_strMidBeta;
	CString	m_strMidGain;
	CString	m_strMidGama;
	//}}AFX_DATA

	short	m_wBassAlpha;
	short	m_wBassBeta;
	short	m_wBassGain;
	short	m_wBassGama;
    short	m_wTrebleAlpha;
    short	m_wTrebleBeta;
    short	m_wTrebleGain;
    short	m_wTrebleGama;
    short	m_wMidAlpha;
    short	m_wMidBeta;
    short	m_wMidGain;
    short	m_wMidGama;

public:
    void EnableMid( BOOL bEnable );
    
protected:
    void ChangeFont();
    void UpdateHexData(BOOL bSaveAndValidate = TRUE);

    void RefreshData( BOOL );
    void RefreshPaint();

    BOOL m_bEnableMid;
    
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEqDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEqDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRefresh();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQDLG_H__2BD4A656_5EB8_4F1C_B111_58272261832D__INCLUDED_)
