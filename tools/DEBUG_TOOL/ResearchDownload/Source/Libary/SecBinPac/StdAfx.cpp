// stdafx.cpp : source file that includes just the standard includes
//	SecBinPac.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if _MSC_VER >= 1300    // for VC 7.0 
// from ATL 7.0 sources 
#ifndef _delayimp_h
extern "C" IMAGE_DOS_HEADER __ImageBase; 
#endif 
#endif     

HMODULE GetCurrentModule() 
{ 
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)           
	// Here's a trick that will get you the handle of the module     
	// you're running in without any a-priori knowledge:    
	MEMORY_BASIC_INFORMATION mbi;     
	static int dummy;     
	VirtualQuery( &dummy, &mbi, sizeof(mbi) );

	return reinterpret_cast<HMODULE>(mbi.AllocationBase); 
#else // VC 7.0     
	// from ATL 7.0 sources     
	return reinterpret_cast<HMODULE>(&__ImageBase); 
#endif 
} 

void GetModuleFilePath( HMODULE hModule, LPTSTR lpszAppPath )
{
	//Get the current running path
	UNUSED_ALWAYS( hModule );
	_TCHAR Path[MAX_PATH];
	_TCHAR *pFoundChar,*pNextChar;

	GetModuleFileName( GetCurrentModule(), Path, MAX_PATH );

	pFoundChar = pNextChar = Path;
	while(pFoundChar)
	{
		//Search the first char '\'
		pFoundChar = _tcschr( pNextChar + 1,_T('\\'));
		//find the char '\'
		if(pFoundChar != NULL)
		{
			//move to the next char pointer to the current address 
			pNextChar = pFoundChar;
			continue;
		}
	}

	//escape the program name 
	*pNextChar = _T('\0');

	_tcscpy( lpszAppPath,Path);
}

unsigned __int64 GetFileSizeEx( LPCTSTR lpFileName)
{
	if (NULL == lpFileName)
	{
		return 0;
	}
	HANDLE			 hResult =  NULL;    
	WIN32_FIND_DATA	 FindFileData;  
	unsigned __int64 llSize = 0;
	FindFileData.nFileSizeHigh = FindFileData.nFileSizeLow = 0;

	hResult  =  FindFirstFile(lpFileName, &FindFileData);   
	if(hResult  !=  INVALID_HANDLE_VALUE)   
	{   
		llSize = FindFileData.nFileSizeHigh ;
		llSize = llSize<<32;
		llSize |= FindFileData.nFileSizeLow;
		FindClose(hResult);
		hResult = NULL;
	}   
	return llSize;

} 