#if !defined(AFX_DLGLOADTEXT_H__B76BDA9C_76CF_467A_B4E6_F7A51E16F65D__INCLUDED_)
#define AFX_DLGLOADTEXT_H__B76BDA9C_76CF_467A_B4E6_F7A51E16F65D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLoadText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadText dialog

class CDlgLoadText : public CDialog
{
// Construction
public:
	CDlgLoadText(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLoadText();

// Dialog Data
	//{{AFX_DATA(CDlgLoadText)
	enum { IDD = IDD_DLG_LOAD_TEXT };
	CButton	m_btnDel;
	CButton	m_btnAdd;
	CListCtrl	m_lstNumberFiles;
	CString	m_strCityFile;
	//}}AFX_DATA

	CStringArray m_agNumberFiles;
	BOOL  m_bSelCityFile;

private:
	HBITMAP m_hAddBitmap;
	HBITMAP m_hDelBitmap;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLoadText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLoadText)
	virtual BOOL OnInitDialog();
	afx_msg void OnDltBtnAdd();
	afx_msg void OnDltBtnDel();
	afx_msg void OnDltSel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOADTEXT_H__B76BDA9C_76CF_467A_B4E6_F7A51E16F65D__INCLUDED_)
