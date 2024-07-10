#if !defined(AFX_FILENAMEDLG_H__06213CD9_206A_4110_B180_07D7B5283186__INCLUDED_)
#define AFX_FILENAMEDLG_H__06213CD9_206A_4110_B180_07D7B5283186__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileNameDlg.h : header file
//

#include "..\effect\CoolEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CFileNameDlg dialog

class CFileNameDlg : public CDialog
{
// Construction
public:
	CFileNameDlg(CWnd* pParent = NULL);   // standard constructor

	void SetLimitText( int nLimit ) { m_nLimitText = nLimit; }
// Dialog Data
	//{{AFX_DATA(CFileNameDlg)
	enum { IDD = IDD_DLG_FILENAME };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CString	m_strFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileNameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileNameDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	int       m_nLimitText;

	CCoolEdit m_edtFileName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILENAMEDLG_H__06213CD9_206A_4110_B180_07D7B5283186__INCLUDED_)
