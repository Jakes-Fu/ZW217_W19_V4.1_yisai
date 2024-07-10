// stdafx.cpp : source file that includes just the standard includes
//	NVWinGen.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
DWORD GetFileSize( LPCTSTR lpFileName)
{
	HANDLE				hResult =  NULL;    
	WIN32_FIND_DATA		FindFileData;  
	FindFileData.nFileSizeHigh = FindFileData.nFileSizeLow = 0;
	DWORD dwSize = 0;
	
	hResult  =  FindFirstFile(lpFileName, &FindFileData);   
	if(hResult  !=  INVALID_HANDLE_VALUE)   
	{   
		dwSize = FindFileData.nFileSizeLow;
		FindClose(hResult);
		hResult = NULL;
	}   
	
	return dwSize;	
}

BOOL LoadFileFromLocal(LPCTSTR pszFileName, LPBYTE &pBuf,DWORD &dwSize)
{	
    FILE *pFile = _tfopen(pszFileName,_T("rb"));
    if(pFile == NULL)
        return FALSE;
    fseek(pFile,0,SEEK_END);
    dwSize = ftell(pFile);
    fseek(pFile,0,SEEK_SET);
	
    pBuf = new BYTE[dwSize];
    if(pBuf == NULL)
        return FALSE;
	
    DWORD dwRead = fread(pBuf,1,dwSize,pFile);
    fclose(pFile);
	
    if(dwRead != dwSize)
    {
        SAFE_DELETE_ARRAY(pBuf);
        return FALSE;
    }
	
    return TRUE;
}
