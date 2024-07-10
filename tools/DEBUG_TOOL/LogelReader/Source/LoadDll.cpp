// LoadDll.cpp: implementation of the CLoadDll class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SUC.h"
#include "LoadDll.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef  (*DllInitDeinitFn)(void);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoadDll::CLoadDll()
{

}

CLoadDll::~CLoadDll()
{

}
	
BOOL CLoadDll::LoadCommHandler(LPCTSTR lpszDllName )
{
	m_hCommDll = LoadLibrary( lpszDllName );
	if( m_hCommDll == NULL )
		return FALSE;

	DllInitDeinitFn  init_fn;
	init_fn = (DllInitDeinitFn)GetProcAddress( m_hCommDll, "InitDll");
	if( init_fn == NULL )
		return FALSE;
	BOOL bSuccess = init_fn();
	if( !bSuccess )
		return FALSE;
	
	m_pCommhandler = *((ICommunicationChannel**)GetProcAddress( m_hCommDll, "commhandler_p"));
	if( m_pCommhandler == NULL )
		return FALSE;
	
	return TRUE;
}

BOOL CLoadDll::UnloadCommHandler()
{
	DllInitDeinitFn  deinit_fn;
	deinit_fn = (DllInitDeinitFn)GetProcAddress( m_hCommDll, "DeinitDll");
	if( deinit_fn == NULL )
		return FALSE;

	deinit_fn();
       return TRUE;

}
