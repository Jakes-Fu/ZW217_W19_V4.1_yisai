#if !defined(AFX_DLGSELNUM_H__73687309_5BFE_4C6B_8CC7_A1AA3039A6F5__INCLUDED_)
#define AFX_DLGSELNUM_H__73687309_5BFE_4C6B_8CC7_A1AA3039A6F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelNum.h : header file
//
#include "..\effect\CoolEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSelNum dialog

class CDlgSelNum : public CDialog
{
// Construction
public:
	CDlgSelNum(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelNum)
	enum { IDD = IDD_DLG_SEL_NUM };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnFind;
	CListCtrl	m_lst;
	CString	m_strName;
	//}}AFX_DATA
    
	CUIntArray m_arrSel;
	CString m_strSearchOldName;
	int     m_nIndex;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelNum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitLstCtrl(void);

	// Generated message map functions
	//{{AFX_MSG(CDlgSelNum)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnFind();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CCoolEdit m_edtName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELNUM_H__73687309_5BFE_4C6B_8CC7_A1AA3039A6F5__INCLUDED_)
