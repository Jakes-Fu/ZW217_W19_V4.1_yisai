// NVEditor.h : main header file for the NVEDITOR application
//

#if !defined(AFX_NVEDITOR_H__3A96C5AC_3CCD_434D_98DF_B69CBF88D5F0__INCLUDED_)
#define AFX_NVEDITOR_H__3A96C5AC_3CCD_434D_98DF_B69CBF88D5F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include <afxadv.h>
#include <algorithm>
#include "NVItemDef.h"
#define WM_CREATE_CUST_TREE			(WM_USER + 401)
#define WM_DESTROY_CUST_TREE		(WM_USER + 402)
#define WM_REFRESH_CUST_TREE		(WM_USER + 403)
#define WM_SAVE_CUST_BASE			(WM_USER + 404)
#define WM_SAVEAS_CUST_BASE			(WM_USER + 405)

typedef struct _FUNC_ITEM_T
{
	TCHAR szFuncName[_MAX_PATH];
	TCHAR szFuncDesc[_MAX_PATH];
	CStringArray agRules;
	_FUNC_ITEM_T(){
		agRules.RemoveAll();
		memset(szFuncName,0,sizeof(szFuncName));
		memset(szFuncDesc,0,sizeof(szFuncDesc));
	}

}FUNC_ITEM_T,*FUNC_ITEM_PTR;

class CNVCmdLine : public CCommandLineInfo
{
public:
	CNVCmdLine() { 
		m_nParaCount = 0;  
		m_bBigEndian = TRUE;
		m_nMaxParaCount = 5; 
		m_bWait = FALSE;
		m_bTail = TRUE;  // add by Joly
		m_bVersion = FALSE;
		m_bHeaderFile = TRUE;
		m_bCheckItemName = TRUE;
		m_strMergePath  = _T("");
		m_strHeaderFile = _T("");
		m_strImportFile = _T("");
		m_strErrorFile = _T("");
		m_strDspBinFile = _T("");
		m_strFixBinFile = _T("");
		m_strHashHeader = _T("");
	}
	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );
	//[[ by Joly
	void ParseCommandLine();
	VEC_FILEINFO	m_vecFileInfo;
	//]]
	
	CString m_strImportFile;
	BOOL    m_bBigEndian;
	CString m_strErrorFile;
	BOOL    m_bWait;
	BOOL    m_bTail;  // add by Joly
	BOOL    m_bVersion;
	BOOL    m_bHeaderFile;
	BOOL    m_bCheckItemName;
	CString m_strMergePath;
	CString m_strHeaderFile;
	CString m_strDspBinFile;
	CString m_strFixBinFile;
	CString m_strHashHeader;

	
protected:
	int m_nParaCount;
	int m_nMaxParaCount;
};

/////////////////////////////////////////////////////////////////////////////
// CNVEditorApp:
// See NVEditor.cpp for the implementation of this class
//

class CNVEditorApp : public CWinApp
{
public:
	CNVEditorApp();
	BOOL GetIniFilePath(TCHAR* pName);
	int  GetPermission();
	BOOL WriteConsole(
				HANDLE hConsoleOutput,
				CONST VOID *lpBuffer,
				DWORD nNumberOfCharsToWrite,
				LPDWORD lpNumberOfCharsWritten,
				LPVOID lpReserved
				);

    CString& GetMruFileName(UINT nIndex)
    {
        return m_pRecentFileList->operator [](nIndex);
    }
    
    CString m_strVersion;
    CString m_strBuild;

	// Command line mode
	BOOL m_bCmdMode;
	// Exit code for the program in command mode
	int m_nCmdRet;
	
	CString m_strErrorFile;
	FILE * m_pErrorFile;
	
	DWORD m_dwPermission;
	BOOL m_bWriteReadOnlyZeroItem;	

	BOOL	m_bNvmFormat;	// TRUE		: keep the old format, .nvm and .prj
							// FALSE	: just support xml format(.xml and .xprj)
	BOOL    m_bShowWorning;
	DWORD	m_dwEnumPortTimeOut;

protected:
    void GetVersion();

	// open a nv project file and generate nvitem.bin without gui
	// [[ modify by Joly for SamSung NV update
	BOOL DoCmdLine(CNVCmdLine& cmdInfo);

private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNVEditorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNVEditorApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CNVEditorApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NVEDITOR_H__3A96C5AC_3CCD_434D_98DF_B69CBF88D5F0__INCLUDED_)
