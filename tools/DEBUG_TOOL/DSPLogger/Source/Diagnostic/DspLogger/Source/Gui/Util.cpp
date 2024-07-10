#include "Util.h"
#include "StdAfx.h"
//
// little_endian and big_endian convert	
//
DWORD ConvertEndian(DWORD  t) 
{ 
    DWORD tResult = 0; 
    for (int  i = 0; i < sizeof(DWORD); ++i) 
    { 
        tResult <<= 8; 
        tResult |= (t & 0xFF) ; 			
        t >>= 8; 
    }
    return  tResult; 
} 

CString GetModuleDir(BOOL bPostfix/* = TRUE*/)//{"D:\View\Debug"}
{
	CString strModuleDir = _T("");
	TCHAR szPathName[MAX_PATH] = {0};
	::GetModuleFileName( NULL, szPathName, _MAX_PATH );
	strModuleDir= szPathName;
	int nIndex = strModuleDir.ReverseFind(_T('\\'));
	if(nIndex != -1)
	{
		strModuleDir = strModuleDir.Left(nIndex + 1);
		if (!bPostfix)
		{
			strModuleDir.TrimRight(_T('\\'));
		}
		
	}
	return strModuleDir;
}
BOOL CreateDir( LPCTSTR lpPathName)
{   
    TCHAR	szFileName[MAX_PATH] = { 0 };
	BOOL    bRet = TRUE;
    TCHAR*  lpChr = NULL;
    TCHAR*  lpHead = NULL;
    lstrcpyn(szFileName, lpPathName, sizeof(szFileName)-1);
	lpHead = szFileName+2;
    while (true)
    {
		lpChr = _tcschr(lpHead, _T('\\'));
        if (NULL == lpChr)
        {
            bRet &= CreateDirectory(szFileName, NULL);
            if (!bRet && (GetLastError() == ERROR_ALREADY_EXISTS))
                bRet = TRUE;
            break;
        }
        else
        {
            *lpChr = NULL;
            bRet &= CreateDirectory(szFileName, NULL);
            if (!bRet && (GetLastError() == ERROR_ALREADY_EXISTS))
                bRet = TRUE;
            *lpChr = _T('\\');
            lpHead = lpChr+1;
        }
    }
    return bRet;
}

BOOL CopyDirectory(LPCTSTR lpSrcPath,LPCTSTR lpDestPath)    
{   
	 
	WIN32_FIND_DATA   FindFileData;   
	HANDLE  hFindFile				= NULL;  
	BOOL    bResult					= FALSE; 
	TCHAR	szSrcFile[MAX_PATH*2]	= {0};
	TCHAR	szDestFile[MAX_PATH*2]	= {0};	

	
	do 
	{
		hFindFile = FindFirstFile(lpDestPath, &FindFileData);   
		if(INVALID_HANDLE_VALUE == hFindFile)   
		{   
			if( !CreateDir(lpDestPath) )
			{
				break;
			}
		}   

		_tcscpy(szSrcFile,lpSrcPath);   
		_tcscat(szSrcFile,_T("\\*"));   
		hFindFile  = FindFirstFile(szSrcFile,&FindFileData);
		if(INVALID_HANDLE_VALUE == hFindFile)   
		{   
			break;  
		}   
		if(_tcscmp(FindFileData.cFileName,_T(".")) && _tcscmp(FindFileData.cFileName ,_T("..")))   
		{   
			_tcscpy(szSrcFile,lpSrcPath);   
			_tcscat(szSrcFile,_T("\\"));   
			_tcscat(szSrcFile,FindFileData.cFileName);  
			
			_tcscpy(szDestFile,lpDestPath);   
			_tcscat(szDestFile,_T("\\")); 
			_tcscat(szDestFile,FindFileData.cFileName);   
			
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)   		
			{    
				if (!CopyDirectory(szSrcFile,szDestFile))
				{
					break;
				} 
			}   
			else   
			{  
				if (!CopyFile(szSrcFile,szDestFile,FALSE))
				{	
					break;
				}
			}   
		} 
		while(TRUE)   
		{   
			if(!FindNextFile(hFindFile,&FindFileData))   
			{
				bResult = TRUE;
				break;   
			} 
			
			if(_tcscmp(FindFileData.cFileName , _T(".")) && _tcscmp(FindFileData.cFileName   ,_T(".."))   )   
			{   
				_tcscpy(szSrcFile,lpSrcPath);   
				_tcscat(szSrcFile,_T("\\"));   
				_tcscat(szSrcFile,FindFileData.cFileName);  
				
				_tcscpy(szDestFile,lpDestPath);   
				_tcscat(szDestFile,_T("\\")); 
				_tcscat(szDestFile,FindFileData.cFileName);   
				
				if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)   		
				{    
					if (!CopyDirectory(szSrcFile,szDestFile))
					{
						break;
					}
				}   
				else   
				{  
					if (!CopyFile(szSrcFile,szDestFile,FALSE))
					{	
						break;
					}
				}   
			}   
		}   
	} while(0);
	
	if (INVALID_HANDLE_VALUE != hFindFile)
	{
		FindClose(hFindFile);
		hFindFile = NULL;
	}
	return   bResult;   
}

BOOL MoveDirectory(LPCTSTR lpSrcPath,LPCTSTR lpDestPath)    
{   
	
	WIN32_FIND_DATA   FindFileData;   
	HANDLE  hFindFile				= NULL;  
	BOOL    bResult					= FALSE; 
	TCHAR	szSrcFile[MAX_PATH]		= {0};
	TCHAR	szDestFile[MAX_PATH]	= {0};	
	
	
	do 
	{
		hFindFile = FindFirstFile(lpDestPath, &FindFileData);   
		if(INVALID_HANDLE_VALUE == hFindFile)   
		{   
			if( !CreateDir(lpDestPath) )
			{
				break;
			}
		}   
		
		_tcscpy(szSrcFile,lpSrcPath);   
		_tcscat(szSrcFile,_T("\\*"));   
		hFindFile  = FindFirstFile(szSrcFile,&FindFileData);
		if(INVALID_HANDLE_VALUE == hFindFile)   
		{   
			break;  
		}   
		if(_tcscmp(FindFileData.cFileName,_T(".")) && _tcscmp(FindFileData.cFileName ,_T("..")))   
		{   
			_tcscpy(szSrcFile,lpSrcPath);   
			_tcscat(szSrcFile,_T("\\"));   
			_tcscat(szSrcFile,FindFileData.cFileName);  
			
			_tcscpy(szDestFile,lpDestPath);   
			_tcscat(szDestFile,_T("\\")); 
			_tcscat(szDestFile,FindFileData.cFileName);   
			
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)   		
			{    
				if (!MoveDirectory(szSrcFile,szDestFile))
				{
					break;
				} 
			}   
			else   
			{  
				//if (!CopyFile(szSrcFile,szDestFile,FALSE))
				if (!MoveFile(szSrcFile,szDestFile))
				{	
					break;
				}
			}   
		} 
		while(TRUE)   
		{   
			if(!FindNextFile(hFindFile,&FindFileData))   
			{
				bResult = TRUE;
				break;   
			} 
			
			if(_tcscmp(FindFileData.cFileName , _T(".")) && _tcscmp(FindFileData.cFileName   ,_T(".."))   )   
			{   
				_tcscpy(szSrcFile,lpSrcPath);   
				_tcscat(szSrcFile,_T("\\"));   
				_tcscat(szSrcFile,FindFileData.cFileName);  
				
				_tcscpy(szDestFile,lpDestPath);   
				_tcscat(szDestFile,_T("\\")); 
				_tcscat(szDestFile,FindFileData.cFileName);   
				
				if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)   		
				{    
					if (!MoveDirectory(szSrcFile,szDestFile))
					{
						break;
					}
				}   
				else   
				{  
					//if (!CopyFile(szSrcFile,szDestFile,FALSE))
					if (!MoveFile(szSrcFile,szDestFile))
					{	
						break;
					}
				}   
			}   
		}   
	} while(0);
	
	if (INVALID_HANDLE_VALUE != hFindFile)
	{
		FindClose(hFindFile);
		hFindFile = NULL;
	}
	return   bResult;   
}