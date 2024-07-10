#if !defined(AFX_DLGFUNCTIONS_H__DA0BCC19_1562_448B_A0FB_176BD57802E8__INCLUDED_)
#define AFX_DLGFUNCTIONS_H__DA0BCC19_1562_448B_A0FB_176BD57802E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFunctions.h : header file
//
#include "NVEditor.h"
#include "MySuperGrid.h"

#pragma  warning(push,3)
#include <vector>
#pragma  warning(pop)

/////////////////////////////////////////////////////////////////////////////
// CDlgFunctions dialog

class CDlgFunctions : public CDialog
{
// Construction
public:
	CDlgFunctions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFunctions)
	enum { IDD = IDD_DIALOG_FUCTIONS };
	CListCtrl	m_lstFuns;
	BOOL	m_bCheckAll;
	//}}AFX_DATA

	CMySuperGrid * m_pList;
	std::vector<FUNC_ITEM_PTR> * m_pFucList;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFunctions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFunctions)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkAll();
	afx_msg void OnExcute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFUNCTIONS_H__DA0BCC19_1562_448B_A0FB_176BD57802E8__INCLUDED_)
