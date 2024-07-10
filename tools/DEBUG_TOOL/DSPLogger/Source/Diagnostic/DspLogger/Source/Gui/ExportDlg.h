#if !defined(AFX_EXPORTDLG_H__479710DD_4B27_46B4_8660_C677E0531975__INCLUDED_)
#define AFX_EXPORTDLG_H__479710DD_4B27_46B4_8660_C677E0531975__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDlg.h : header file
//

#include "DspKernelExport.h"
/////////////////////////////////////////////////////////////////////////////
// CExportDlg dialog

class CExportDlg : public CDialog
{
// Construction
public:
	CExportDlg(CWnd* pParent = NULL);   // standard constructor

public:
    CString  GetSrcFile();
    CString  GetDesFile();
    LONGLONG GetRangeFrom();
    LONGLONG GetRangeTo();

// Dialog Data
	//{{AFX_DATA(CExportDlg)
	enum { IDD = IDD_DIALOG_EXPORT };
	CComboBox	m_cmbTo;
	CEdit	m_edtTo;
	CButton	m_btnBrowseDes;
	CButton	m_btnBrowseSrc;
	CEdit	m_edtFrom;
	CEdit	m_edtSrc;
	CEdit	m_edtDes;
	CString	m_strDesFile;
	CString	m_strSrcFile;
	CString	m_strFrom;
	CString	m_strTo;
	CButton m_btnLog;
	CButton m_btnOrg;
	BOOL	m_bAddDes;
	//}}AFX_DATA

public:
	DWORD m_dwType;

protected:
    CButton m_btnCurFile;
    CButton m_btnChooseFile;
    CButton m_btnAll;
    CButton m_btnRange;

protected:
    BOOL m_bExportAll;
    BOOL m_bCurFile;
    
protected:
    void ShowControls();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportDlg)
	afx_msg void OnRadioAll();
	afx_msg void OnRadioChooseFile();
	afx_msg void OnRadioCurFile();
	afx_msg void OnRadioRange();
	afx_msg void OnButtonBrowseSrc();
	afx_msg void OnButtonBrowseDes();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioLogBtn();
	afx_msg void OnRadioOrgBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTDLG_H__479710DD_4B27_46B4_8660_C677E0531975__INCLUDED_)
