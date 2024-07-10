
/******************************************************************************
**                        Edit History                                       *
** ------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                               *
** 09/22/2009   Apple.Gao          Create.                                 *
******************************************************************************/

#ifndef __LOADLIBRARY_H__
#define __LOADLIBRARY_H__

enum 
{
	FILESYSTEM_FEATURE,
	BLUETOOTH_FEATURE,
	MP3_MP4_FEATURE,
	DC_FEATURE,
	NET_SETTING_FEATURE,
	FEATURE_COUNT
};


typedef struct dll_info_s{
	char szDllFilePath[MAX_PATH];
	HMODULE hDll;
}DLL_INFO_S;

typedef BOOL (CALLBACK* CP_AddControlPage)( );


#endif//__LOADLIBRARY_H__