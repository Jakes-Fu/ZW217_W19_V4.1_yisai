/*************************************************************************
* MediaCoder - The universal audio/video transcoder
*
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*************************************************************************/

#include <windows.h>

#ifdef _USRDLL

#define EXPORT __declspec(dllexport)

HINSTANCE hDllInst;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	hDllInst = (HINSTANCE)hModule;
    return TRUE;
}
#endif