// CtrlPan_DLL.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
//#include "resource.h"
#include "FeatureDll.h"
#include "NetstateDlg.h"

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
	CNetstateDlg* page = new CNetstateDlg();	
	gs_pWndConPan = page;

	AddControlPage( IDD_NETSTATE_PANAL );	
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
