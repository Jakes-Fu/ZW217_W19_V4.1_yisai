#if !defined(AFX_PROMPTINFODLG_H__EE229770_B34E_4C87_8A59_C9710BA6B18A__INCLUDED_)
#define AFX_PROMPTINFODLG_H__EE229770_B34E_4C87_8A59_C9710BA6B18A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PromptInfoDlg.h : header file
//
#include "ResizableDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CPromptInfoDlg dialog

class CPromptInfoDlg : public CResizableDialog
{
// Construction
public:
	CPromptInfoDlg(CWnd* pParent = NULL);   // standard constructor
	void SetPromptText(CStringArray &agText);

// Dialog Data
	//{{AFX_DATA(CPromptInfoDlg)
	enum { IDD = IDD_DIALOG_PROMPT_TEXT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPromptInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPromptInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CStringArray m_agPromptInfo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMPTINFODLG_H__EE229770_B34E_4C87_8A59_C9710BA6B18A__INCLUDED_)
