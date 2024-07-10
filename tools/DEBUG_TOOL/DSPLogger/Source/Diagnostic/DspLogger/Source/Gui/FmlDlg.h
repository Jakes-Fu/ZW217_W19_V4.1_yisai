#if !defined(AFX_FMLDLG_H__055CD0EB_5596_46AD_BBD1_3CB93EF7300C__INCLUDED_)
#define AFX_FMLDLG_H__055CD0EB_5596_46AD_BBD1_3CB93EF7300C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FmlDlg.h : header file
//
#include "MathFormula.h"
#include "Filter.h"

/////////////////////////////////////////////////////////////////////////////
// CFmlDlg dialog

class CFmlDlg : public CDialog
{
// Construction
public:
	CFmlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFmlDlg)
	enum { IDD = IDD_DIALOG_FORMULA };
	CString	m_strFml;
	UINT	m_nStartPos;
	//}}AFX_DATA
public:
    CMathFormula*  m_pMathFml;
    TP_FILTER_DATA m_Filter_Data;
    

private:

    BOOL     ParserFml(CString strFml);
    BOOL     CheckToken(CString strFml);

    int      GetToken(CString strFml, int token_idx, CString& strToken);
    int      GetAddr(CString strFml, CString& strAddr);
    int      GetSign(CString strFml,int idx, CString& strSign); 
	
	double   GetCoeff(CString strFml, CString strToken, int nSymbol);
	BOOL	 IsOnlyOneOperator(CString strFml);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFmlDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMLDLG_H__055CD0EB_5596_46AD_BBD1_3CB93EF7300C__INCLUDED_)
