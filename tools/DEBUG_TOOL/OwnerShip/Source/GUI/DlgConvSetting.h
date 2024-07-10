#if !defined(AFX_DLGCONVSETTING_H__797FCBA0_4112_48A1_92BB_486C570B5887__INCLUDED_)
#define AFX_DLGCONVSETTING_H__797FCBA0_4112_48A1_92BB_486C570B5887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConvSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConvSetting dialog

class CDlgConvSetting : public CDialog
{
// Construction
public:
	CDlgConvSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgConvSetting)
	enum { IDD = IDD_DLG_CONV_SETTING };
	CString	m_strQCMidFile;
	CString	m_strQCTxtFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConvSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgConvSetting)
	virtual void OnOK();
	afx_msg void OnCsBtnQcmBrow();
	afx_msg void OnCsBtnQctBrow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONVSETTING_H__797FCBA0_4112_48A1_92BB_486C570B5887__INCLUDED_)
