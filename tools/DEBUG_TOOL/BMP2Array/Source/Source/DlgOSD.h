#if !defined(AFX_DLGOSD_H__EE2228BD_7EBC_4694_867B_5B8670326CBA__INCLUDED_)
#define AFX_DLGOSD_H__EE2228BD_7EBC_4694_867B_5B8670326CBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOSD.h : header file
//
#include "CoolStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgOSD dialog
#define MAX_BMP_RECT_NUM  100


class CDlgOSD : public CDialog
{
// Construction
public:
	CDlgOSD(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOSD();

public:
	BMP_RECT_PTR m_parrBmpRect[MAX_BMP_RECT_NUM];	
	int          m_nArrSize;
	CUIntArray   m_narrExpandFactor;
	int          m_nColorSpace;

// Dialog Data
	//{{AFX_DATA(CDlgOSD)
	enum { IDD = IDD_DLG_OSD };
	CComboBox	m_cmbExpandFactor;
	CCoolStatic	m_stcColor;
	CString	m_strBmpDir;
	CString	m_strBmpDirArrayFile;
	CString	m_strTranspColor;
	CString	m_strExpandFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOSD)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString DistillFileName(LPCTSTR pPath);
	void    EnumFiles(CStringArray &arrFiles, LPCTSTR pDir);	
	BOOL    CheckTransparentColor();	
	BOOL    GenerateTargetFile(void);
	void    SetColor(CString strColor);
	BOOL    LoadConfig();
	
	CString m_strBmpArrName;
    CString m_strBmpDirArrName;

	// Generated message map functions
	//{{AFX_MSG(CDlgOSD)
	afx_msg void OnBtnSelBmpDir();
	afx_msg void OnBtnSelBmpDirArrayFile();
	afx_msg void OnBtnGen();
	afx_msg void OnMaxtextEdtTransparentColor();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:	
    static const _TCHAR SZEXTENSION[];
	UINT m_nTransparentColor;
//	BOOL m_bFresh;

	static const TCHAR SZ_SEC_BMPRECT[];
	static const TCHAR SZ_KEY_BMPRECT_INDEX[];
	
	static const TCHAR SZ_SEC_TRANSPARENT_COLOR[];
	static const TCHAR SZ_KEY_TRANSPARENT_COLOR[];
	
	static const TCHAR SZ_SEC_EXPAND_FACTOR[];
	static const TCHAR SZ_KEY_EXPAND_FACTOR[];
	
	static const TCHAR SZ_SEC_COLOR_SPACE[];
	static const TCHAR SZ_KEY_COLOR_SPACE[];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOSD_H__EE2228BD_7EBC_4694_867B_5B8670326CBA__INCLUDED_)
