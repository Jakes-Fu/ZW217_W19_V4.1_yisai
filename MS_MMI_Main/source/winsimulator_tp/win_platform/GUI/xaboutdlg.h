/******************************************************************************
 ** File Name:      XAboutDlg.h                                               *
 ** Author:         Hongliang.Xin                                             *
 ** DATE:           2009/10/22                                                *
 ** Copyright:      2009 Spreadtrum Communications Inc. All Rights Reserved.  *
 ** Description:    Header of the CXAboutDlg and CXVerInfo                    *
 **                 This is for "about dialog" with a unique style            *
 ** Note:           None                                                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2009/10/22     Hongliang.Xin    Create.                                   *
 ******************************************************************************/
 
#if !defined(AFX_XABOUTDLG_H__29277FAD_DD5E_4522_8C31_4177886123BD__INCLUDED_)
#define AFX_XABOUTDLG_H__29277FAD_DD5E_4522_8C31_4177886123BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XAboutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXAboutDlg dialog

class CXAboutDlg : public CDialog
{
// Construction
public:
/**
 * lpszCodeVer: Mobile Code Version string
 * 
 */
	CXAboutDlg(LPCTSTR lpszCodeVer = NULL,CWnd* pParent = NULL);   // standard constructor
	~CXAboutDlg();

	void SetProductName(LPCTSTR lpszProductName);
// Dialog Data
	//{{AFX_DATA(CXAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CBitmap m_bmpDlgBckGrnd;   // not used 
	CBrush  m_brshDlgBckGrnd;
	CString m_strVersion;
	CString m_strBuild;
	CString m_strCopyright;
	CString m_strProductName;
	HBITMAP m_hBitmap;        // background bmp handle

	// Generated message map functions
	//{{AFX_MSG(CXAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	const static BYTE m_bmpdata[];
	CString  m_strCodeVer;  // Mobile Code Version 
	int      m_nCodeVerHight;

};

/////////////////////////////////////////////////////////////////////////////
// CXVerInfo

class CXVerInfo 
{
public:
	LPCTSTR GetSpecialBuild(void);
	LPCTSTR GetPrivateBuild(void);
	LPCTSTR GetProductVersion(void);
	LPCTSTR GetProductName(void);
	LPCTSTR GetLegalTrademarks(void);
	LPCTSTR GetLegalCopyright(void);
	LPCTSTR GetCompanyName(void);
	LPCTSTR GetComments(void);
	BOOL Init(LPCTSTR pszFileName = NULL);
public:
	CXVerInfo();
	~CXVerInfo();
private:
	LPBYTE m_pVerInfoBuf;
	_TCHAR m_szSubBlock[ 64 ];
	_TCHAR m_szFileName[ _MAX_PATH ];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XABOUTDLG_H__29277FAD_DD5E_4522_8C31_4177886123BD__INCLUDED_)
