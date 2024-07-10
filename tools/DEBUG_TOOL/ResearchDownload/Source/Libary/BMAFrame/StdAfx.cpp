// stdafx.cpp : source file that includes just the standard includes
//	BMAFramework.pch will be the pre-compiled header
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


int SplitStr(LPCTSTR lpszStr,CStringArray &agStrs,TCHAR chSeparate /*= _T(',')*/)
{
	agStrs.RemoveAll();
	
    CString strSrc(lpszStr);
	int nLen = _tcslen(lpszStr);
	
	if(nLen == 0)
		return 0;
	
    LPTSTR  lpBuf  = (LPTSTR)strSrc.operator LPCTSTR();
	LPTSTR  lpFind = _tcschr(lpBuf, chSeparate);
	CString strTmp;
	while(lpFind != NULL)
	{
		*lpFind = _T('\0');
		strTmp = lpBuf;
		strTmp.TrimLeft();
		strTmp.TrimRight();
		if(!strTmp.IsEmpty())
			agStrs.Add(strTmp);
		lpBuf = lpFind + 1;
		lpFind = _tcschr(lpBuf, chSeparate);
	}
	
	strTmp = lpBuf;
	strTmp.TrimLeft();
	strTmp.TrimRight();
	if(!strTmp.IsEmpty())
		agStrs.Add(strTmp);
	
	return agStrs.GetSize();
}


