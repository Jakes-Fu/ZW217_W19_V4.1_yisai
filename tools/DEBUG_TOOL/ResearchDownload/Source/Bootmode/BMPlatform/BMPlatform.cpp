// BMPlatform.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "BMPlatform.h"
#include "BootModeObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CBMPlatformApp

BEGIN_MESSAGE_MAP(CBMPlatformApp, CWinApp)
	//{{AFX_MSG_MAP(CBMPlatformApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMPlatformApp construction

CBMPlatformApp::CBMPlatformApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pfCreateChannel = NULL;
	m_pfReleaseChannel = NULL;
	m_hChannelLib = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBMPlatformApp object

CBMPlatformApp g_theApp;

BOOL CBMPlatformApp::InitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFilePath(NULL,szPath);

#if _MSC_VER < 1300  //VC 6.0
#ifdef _DEBUG
	_tcscat(szPath,_T("\\ChannelD.dll"));
#else
	_tcscat(szPath,_T("\\Channel.dll"));
#endif
#else
#ifdef _DEBUG
	_tcscat(szPath,_T("\\Channel9.dll"));
#else
	_tcscat(szPath,_T("\\Channel9.dll"));
#endif
#endif

	m_hChannelLib = LoadLibrary(szPath);

	if(m_hChannelLib == NULL)
	{
		CString strErr;
		strErr.Format(_T("Cannot load Channel DLL (%s)."),szPath);
        _tprintf(strErr.operator LPCTSTR());
		return FALSE;
	}

	m_pfCreateChannel = (pfCreateChannel)GetProcAddress(m_hChannelLib,"CreateChannel");
	m_pfReleaseChannel = (pfReleaseChannel)GetProcAddress(m_hChannelLib,"ReleaseChannel");

	if(m_pfCreateChannel == NULL || m_pfReleaseChannel == NULL )
	{
		CString strErr;
		strErr.Format(_T("Cannot load Channel function (%s)."),szPath);
        _tprintf(strErr.operator LPCTSTR());
		return FALSE;
	}

	return CWinApp::InitInstance();
}

int CBMPlatformApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_hChannelLib)
	{
		FreeLibrary(m_hChannelLib);
		m_hChannelLib = NULL;

		//m_pfCreateChannel = NULL;
		//m_pfReleaseChannel = NULL;
	}

	return CWinApp::ExitInstance();
}



BMP_EXPORT_API BOOL CreateBMObject( IBootModeHandler ** pBMObj)
{
	CBootModeObject * pbmo = new CBootModeObject();
	if(pbmo == NULL)
	{
		return FALSE;
	}

	*pBMObj = (IBootModeHandler*)pbmo;

	return TRUE;
}

BMP_EXPORT_API void ReleaseBMObject( IBootModeHandler * pBMObj)
{
	if(pBMObj != NULL)
	{
		delete pBMObj;
	}
}

