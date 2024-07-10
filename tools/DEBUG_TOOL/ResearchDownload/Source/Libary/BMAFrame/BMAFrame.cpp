// BMAFrame.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "BMAFrame.h"
#include "XmlConfigParse.h"
#include "BMAFImp.h"

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
// CBMAFrameApp

BEGIN_MESSAGE_MAP(CBMAFrameApp, CWinApp)
	//{{AFX_MSG_MAP(CBMAFrameApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMAFrameApp construction

CBMAFrameApp::CBMAFrameApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pfCreateBMObject = NULL;
	m_pfReleaseBMObject = NULL;

	m_hBMPlatfromLib= NULL;

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBMAFrameApp object

CBMAFrameApp g_theApp;

/////////////////////////////////////////////////////////////////////////////
// CBMAFrameApp initialization

BOOL CBMAFrameApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	//COleObjectFactory::RegisterAll();

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFilePath(NULL,szPath);

#if _MSC_VER < 1300  //VC 6.0
#ifdef _DEBUG
	_tcscat(szPath,_T("\\BMPlatformD.dll"));
#else
	_tcscat(szPath,_T("\\BMPlatform.dll"));
#endif
#else
#ifdef _DEBUG
	_tcscat(szPath,_T("\\BMPlatform9.dll"));
#else
	_tcscat(szPath,_T("\\BMPlatform9.dll"));
#endif
#endif

	m_hBMPlatfromLib = LoadLibrary(szPath);

	if(m_hBMPlatfromLib == NULL)
	{
		CString strErr;
		strErr.Format(_T("Cannot load BMPlatform DLL (%s)."),szPath);
        _tprintf(strErr.operator LPCTSTR());
		return FALSE;
	}

	m_pfCreateBMObject = (pfCreateBMObject)GetProcAddress(m_hBMPlatfromLib,"CreateBMObject");
	m_pfReleaseBMObject = (pfReleaseBMObject)GetProcAddress(m_hBMPlatfromLib,"ReleaseBMObject");

	if(m_pfCreateBMObject == NULL || m_pfReleaseBMObject == NULL )
	{
		CString strErr;
		strErr.Format(_T("Cannot load BMPlatform function (%s)."),szPath);
        _tprintf(strErr.operator LPCTSTR());
		return FALSE;
	}

	return CWinApp::InitInstance();
}

int CBMAFrameApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	if(m_hBMPlatfromLib)
	{
		FreeLibrary(m_hBMPlatfromLib);
		m_hBMPlatfromLib = NULL;

		//m_pfCreateBMObject = NULL;
		//m_pfReleaseBMObject = NULL;
	}


	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

// STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
// {
// 	AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 	return AfxDllGetClassObject(rclsid, riid, ppv);
// }
// 
// STDAPI DllCanUnloadNow(void)
// {
// 	AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 	return AfxDllCanUnloadNow();
// }
// 
// // by exporting DllRegisterServer, you can use regsvr.exe
// STDAPI DllRegisterServer(void)
// {
// 	AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 	COleObjectFactory::UpdateRegistryAll();
// 	return S_OK;
// }
IBMAFramework::~IBMAFramework(void)
{

}

IXmlConfigParse::~IXmlConfigParse()
{

}

BMAF_EXPORT_API BOOL CreateXmlConfigParse( IXmlConfigParse ** pXmlConfigParse )
{
	*pXmlConfigParse = new CXmlConfigParse;
	if( *pXmlConfigParse == NULL )
    {
		return FALSE;
    }
	
	return TRUE;
}

BMAF_EXPORT_API BOOL CreateBMAFramework( IBMAFramework ** pBMAFramework )
{
	*pBMAFramework = new CBMAFImp;
	if( *pBMAFramework == NULL )
    {
		return FALSE;
    }
	
	return TRUE;
}
