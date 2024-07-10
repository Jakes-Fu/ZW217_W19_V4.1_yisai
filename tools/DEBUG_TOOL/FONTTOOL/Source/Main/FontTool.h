// FontTool.h : main header file for the FONTTOOL application
//

#if !defined(AFX_FONTTOOL_H__E7CE21CF_E62D_49A1_8D40_F6020719B779__INCLUDED_)
#define AFX_FONTTOOL_H__E7CE21CF_E62D_49A1_8D40_F6020719B779__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Global.h"

#define WM_LOAD_SETTING (WM_USER + 20000)

/////////////////////////////////////////////////////////////////////////////
// CFontToolApp:
// See FontTool.cpp for the implementation of this class
//

class CFontToolApp : public CWinApp
{
public:
	CFontToolApp();

	void ReleaseCSI();
    BOOL LoadSetting();
	CMap<CString,LPCTSTR,UINT,UINT> m_mapFontType;	
	CStringArray m_aFontType;
	MAP_CHAR_SET_INFO m_mapCSI;

	CStringArray      m_aCharSetName;
	
	BOOL m_nLogMode;

	BOOL m_nBdfCalcBearingYStrategy;
	
	BOOL m_bSupportSubGlyf;
	CMap<CString,LPCTSTR,UINT,UINT> m_mapCSSupportSubGlyfFlag;	

	BOOL IsSupportSubGlyph(LPCTSTR pszCSTag);
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontToolApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFontToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()

private:	
	BOOL LoadIni(LPCTSTR lpszFile);
    int  EnumKeys(LPCTSTR pSection,CStringArray* pKeys);	
	BOOL LoadGlobalSetting(LPCTSTR lpszFile);
	void DeleteCSI(CHAR_SET_INFO_PTR pCSI);
};

extern CFontToolApp g_theApp;
extern BYTE * g_buf_ptr;
extern UINT   g_buf_size;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTTOOL_H__E7CE21CF_E62D_49A1_8D40_F6020719B779__INCLUDED_)
