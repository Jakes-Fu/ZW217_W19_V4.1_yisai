// stdafx.cpp : source file that includes just the standard includes
//	BMPlatform.pch will be the pre-compiled header
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

BOOL CreateDeepDirectory(LPCTSTR lpszDir)
{
	if(lpszDir == NULL)
		return FALSE;

	CString			strDir = lpszDir;
	HANDLE			hFile;  
	WIN32_FIND_DATA fileinfo;  
	CStringArray    agDir;  
	BOOL			bOK;  
	int				nCount = 0;  
	CString			strTemp= _T("");  


	hFile   =   FindFirstFile(strDir,&fileinfo);  

	// if the file exists and it is a directory  
	if(fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)  
	{  
		//     Directory   Exists   close   file   and   return  
		FindClose(hFile);  
		return   TRUE;  
	}  
	agDir.RemoveAll();  
	for( nCount = 0; nCount < strDir.GetLength(); nCount++ )  
	{  
		if( strDir.GetAt(nCount) != _T('\\') )  
			strTemp += strDir.GetAt(nCount);  
		else  
		{  
			agDir.Add(strTemp);  
			strTemp += _T("\\");  
		}  
		if(nCount ==   strDir.GetLength()-1) 
		{
			agDir.Add(strTemp);  
		}
	}  


	//   Close   the   file  
	FindClose(hFile);  


	// Now lets cycle through the String Array and create each directory in turn  
	for(nCount = 1; nCount < agDir.GetSize(); nCount++)  
	{  
		strTemp = agDir.GetAt(nCount);  
		bOK = CreateDirectory(strTemp,NULL);  

		// If the Directory exists it will return a false  
		if(bOK)
		{
			// If we were successful we set the attributes to normal  
			SetFileAttributes(strTemp,FILE_ATTRIBUTE_NORMAL);  
		}
		else
		{
			if( GetLastError() != ERROR_ALREADY_EXISTS )
			{ 	
				agDir.RemoveAll(); 
				return FALSE;
			}  
		}

	}  
	// Now lets see if the directory was successfully created  
	hFile   =   FindFirstFile(strDir,&fileinfo);  

	agDir.RemoveAll();  
	if(fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)  
	{  
		// Directory Exists close file and return  
		FindClose(hFile);  
		return   TRUE;  
	}  
	else  
	{  
		// For Some reason the Function Failed Return FALSE  
		FindClose(hFile);  
		return   FALSE;  
	}  

}

