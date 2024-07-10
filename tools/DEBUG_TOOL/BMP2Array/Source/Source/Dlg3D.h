#if !defined(AFX_DLG3D_H__391E7A89_DAF3_4460_A3AB_C683D6E640AC__INCLUDED_)
#define AFX_DLG3D_H__391E7A89_DAF3_4460_A3AB_C683D6E640AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg3D.h : header file
//

#define SE_DESKTOP_ROTATE_FILE	_T("SE_DESKTOP_ROTATE_table")
#define SE_DISTORT_MOVE_FILE	_T("SE_DISTORT_MOVE_table")
#define SE_SLIDE_MOVE_FILE		_T("SE_SLIDE_MOVE_table")
#define SE_MICORS				_T("SE_MICORS")
#define OUT_PUT_FILE			_T("ui_special_effect_table.c")

#define ICON_FLAG				"//#MICRO_DEFINITION_BEGIN"
#define DISTORT_FLAG			"//#DISTORT_MOVE_TABLE_BEGIN"
#define DESKTOP_FLAG			"//#DESKTOP_ROTATION_TABLE_BEGIN"
#define SLIDE_FLAG				"//#SLIDE_MOVE__TABLE_BEGIN"

#define EVENT_PROCESS 3388
/////////////////////////////////////////////////////////////////////////////
// CDlg3D dialog

class CDlg3D : public CDialog
{
// Construction
public:
	CDlg3D(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg3D)
	enum { IDD = IDD_DLG_3D };
	UINT	m_uWSlide;
	UINT	m_uWDesktop;
	UINT	m_uHSlide;
	UINT	m_uHDesktop;
	CString	m_strOutPut;
	CString m_strImportPath;

	CEdit	m_edtOutPut;
	CButton m_btnBrowse;
	CButton m_btnImport;

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg3D)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlg3D)
	virtual BOOL OnInitDialog();
	afx_msg void OnCalc();
	afx_msg void OnBtnBrowse();
	afx_msg void OnBtnImport();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	_TCHAR m_szTmpPath[_MAX_PATH];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG3D_H__391E7A89_DAF3_4460_A3AB_C683D6E640AC__INCLUDED_)
