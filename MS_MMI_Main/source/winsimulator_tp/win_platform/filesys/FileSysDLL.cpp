// CtrlPan_DLL.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
//#include "resource.h"
#include "FeatureDll.h"
#include "FileSysDlag.h"

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

// Pointer to Control Panel main wnd.
extern CWnd * gs_pWndConPan ;
extern HINSTANCE g_hInstance;


/**---------------------------------------------------------------------------*
 **                         Exported Functions                                *
 **---------------------------------------------------------------------------*/
BOOL CP_AddControlPage()
{	
//	AfxSetResourceHandle( g_hInstance );
	CFileSysDlg* page = new CFileSysDlg();	
	gs_pWndConPan = page;

	AddControlPage( IDD_FILESYSTEM_PAGE );	
	return TRUE;
}

void CP_CloseControlPage()
{
	CloseControlPage();
}

void CP_RegisterFuns(CP_FUNC_LIST *pFuncList)
{
	RegisterFuns( pFuncList );
}
