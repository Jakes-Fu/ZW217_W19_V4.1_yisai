// BmpToArray.h : main header file for the BMPTOARRAY application
//

#if !defined(AFX_BMPTOARRAY_H__A5F01A7C_3DDB_48CC_8558_2645715C5170__INCLUDED_)
#define AFX_BMPTOARRAY_H__A5F01A7C_3DDB_48CC_8558_2645715C5170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

enum _FUNC_TYPE
{
	FT_B2A,
	FT_3DTABLE
};

enum _BTA_TYPE
{
	BMP_FILE,
	BMP_FOLDER,
	ANIM_FOLDER,
	JPG_FILE,
	JPG_FOLDER,
	GIF_FILE,
	GIF_FOLDER,
	PNG_FILE,
	PNG_FOLDER
};

enum _CMP_TYPE
{
	BMP565_RLE,
	SJPG,
	SGIF,
	SPNG
};



/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayApp:
// See BmpToArray.cpp for the implementation of this class
//
typedef struct _BMP_RECT_T{
	int nHeight;
	int nWidth;
}BMP_RECT_T,*BMP_RECT_PTR;

class CBmpToArrayApp : public CWinApp
{
public:
	CBmpToArrayApp();

	CString SelectFolder(HWND hWnd, LPCTSTR lpcszTitle);
    CString SelectFile(BOOL bOpen, LPCTSTR pExtension);
	
	void XDelete3DFiles(LPCTSTR lpszPath);
	BOOL   GenBmpArray(LPCTSTR lpszSrcPath, LPCTSTR lpszDstPath,int nType,BOOL bBigEndian=TRUE);

private:
	BOOL   Gen3DtableFile(UINT uSWidth, UINT uSHeight, UINT uDWidth, UINT uDHeight, CString strOutputPath);
//	BOOL   GenBmpArray(LPCTSTR lpszSrcPath, LPCTSTR lpszDstPath,int nType,BOOL bBigEndian=TRUE);
	void   EnumFiles(CStringArray &arrFiles, LPCTSTR pDir,int nCmpType);
	CString DistillFileName(LPCTSTR pPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpToArrayApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBmpToArrayApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static const TCHAR SZ_LOGON_FILE[];
	static const TCHAR SZ_SEC_BMPRECT[];
	static const TCHAR SZ_KEY_BMPRECT_INDEX[];
	
	static const TCHAR SZ_SEC_TRANSPARENT_COLOR[];
	static const TCHAR SZ_KEY_TRANSPARENT_COLOR[];
	
	static const TCHAR SZ_SEC_EXPAND_FACTOR[];
	static const TCHAR SZ_KEY_EXPAND_FACTOR[];
	
	static const TCHAR SZ_SEC_COLOR_SPACE[];
	static const TCHAR SZ_KEY_COLOR_SPACE[];

public:
	BOOL m_bCmd;
	
	BOOL m_bShowOSD;
	
	BOOL m_bGenBinFile;
	TCHAR m_szBinSuffixed[10];

};

extern CBmpToArrayApp g_theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPTOARRAY_H__A5F01A7C_3DDB_48CC_8558_2645715C5170__INCLUDED_)
