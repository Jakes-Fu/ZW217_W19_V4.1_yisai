#if !defined(AFX_DLGIMPORT_H__9A951265_0D57_4C62_9C5C_529B29E44905__INCLUDED_)
#define AFX_DLGIMPORT_H__9A951265_0D57_4C62_9C5C_529B29E44905__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgImport dialog

class CDlgImport : public CDialog
{
// Construction
public:
	CDlgImport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgImport)
	enum { IDD = IDD_DIALOG_IMPORT };
		// NOTE: the ClassWizard will add data members here
	CString m_strPath;
	CButton m_btnConvert;
	//}}AFX_DATA
public:
	CString m_strConvertFile;
	int m_nConvertCheck;
protected:
	BOOL ConvertTP(LPSTR lpInputFile, LPSTR lpOutputFile);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImport)
public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImport)
	afx_msg void OnButtonImportBrowse();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPORT_H__9A951265_0D57_4C62_9C5C_529B29E44905__INCLUDED_)
