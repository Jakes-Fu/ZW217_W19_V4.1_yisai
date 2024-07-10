#include "windows.h"
#include "LoadLibrary.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
		
	/**--------------------------------------------------------------------------*
	**                         GOUBLE DEFINITION                                *
	**--------------------------------------------------------------------------*/
	DLL_INFO_S g_dll_info[FEATURE_COUNT];
	//HMODULE g_hfilesys_dll= NULL;
	/**--------------------------------------------------------------------------*
	**                         MODULES LOAD									 *
	**--------------------------------------------------------------------------*/
	//const char* module_name[]={"T:\\Tools\\Common\\Bin\\filesys.dll"};	
	/**--------------------------------------------------------------------------*
	**                         FUNCTIONS										 *
	**--------------------------------------------------------------------------*/	
	CP_AddControlPage pfn_startctrlpan = NULL;
	
	void LoadDllModule( int nFeatureIndex )
	{	
		g_dll_info[nFeatureIndex].hDll = ::LoadLibrary( g_dll_info[nFeatureIndex].szDllFilePath ); 
		if ( g_dll_info[nFeatureIndex].hDll  == NULL )
		{				
			MessageBox(NULL,g_dll_info[nFeatureIndex].szDllFilePath,"load library error. ",MB_OK|MB_ICONWARNING);
			return;
		}
		
		//Start control panel
		CP_AddControlPage pfn_startctrlpan = NULL;
		pfn_startctrlpan= GetProcAddress( g_dll_info[nFeatureIndex].hDll,"CP_AddControlPage");
		if( pfn_startctrlpan == NULL )
		{
			MessageBox(NULL,g_dll_info[nFeatureIndex].szDllFilePath,"Can't find the enter function. ",MB_OK|MB_ICONWARNING);
			return;			
		}
		(*pfn_startctrlpan)();	
		
	}
		
#ifdef __cplusplus
	}
#endif