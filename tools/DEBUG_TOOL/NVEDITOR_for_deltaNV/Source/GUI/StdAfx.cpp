// stdafx.cpp : source file that includes just the standard includes
//	NVEditor.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"



int EnumKeys(TCHAR* pSection,CStringArray* pKeys)
{
    ASSERT(pSection != NULL);
    ASSERT(pKeys != NULL);

	if(pKeys == NULL || pSection== NULL)
	{
		return 0;
	}
    
    int number = 0;
    while(TRUE)
    {
        CString strLine = pSection;
        int nLen = strLine.GetLength();
        if(0 == nLen)
        {
            // All keys are read
            break;
        }
        
        int nIndex = strLine.Find('=');
        if(-1 == nIndex)
        {
            // Not a valid key
            continue;
        }
        CString strKey = strLine.Left(nIndex);
        strKey.TrimLeft();
        strKey.TrimRight();
        pKeys->Add(strKey);
        
        CString strData = strLine.Right(nLen - nIndex - 1);
        strData.TrimLeft();
        strData.TrimRight();
        pKeys->Add(strData);
        
        ++number;
        
        pSection += nLen + 1;
        
    }
    
    return number;
}

DWORD GetDigit(LPCTSTR lpszText)
{
	if(lpszText == NULL)
		return 0;
	
    CString strText = lpszText;
	strText.MakeLower();
	
    DWORD dwRet = 0 ;
    if(!strText.Left(2).CompareNoCase(_T("0x")))
    {
        // Hex mode
        _stscanf(strText,_T("%x"),&dwRet);
    }
    else
    {
        // Decimal mode
        _stscanf(strText,_T("%d"),&dwRet);
    }
	
    return dwRet;
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


BOOL IsExistFile(LPCTSTR lpFile)
{
    BOOL bOK = FALSE;
    if(lpFile && _tcslen(lpFile)&& -1 != _taccess(lpFile, 0) )
    {
        bOK = TRUE;
    }
    else
    {
        _tprintf(_T("File[%s] don't exist\n"),lpFile );
    }
	
    return bOK;
}

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

BOOL IsSpecifiedFile(TCHAR* lpFile,int nFileType)
{
    BOOL bRet = FALSE;
    if(NULL == lpFile)
    {
        return FALSE;
    }
	
    TCHAR* pFileExt = _tcsrchr(lpFile,_T('.'));
    if(pFileExt)
    {
        switch(nFileType)
        {
        case E_DOT_H_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".h")) ) 
            {
                bRet = TRUE;
            }		
            break;
        case E_DOT_BIN_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".bin")) ) 
            {
                bRet = TRUE;
            }		
            break;
        case E_DOT_PRJ_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".prj")) ) 
            {
                bRet = TRUE;
            }
            break;
        case E_DOT_XPRJ_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".xprj")) ) 
            {
                bRet = TRUE;
            }
            break;
			
        case E_DOT_NV_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".nv")) ) 
            {
                bRet = TRUE;
            }		
            break;
        case E_DOT_TXT_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".txt")) ) 
            {
                bRet = TRUE;
            }		
            break;
			
        default:
            break;
        }
    }
	
    return bRet;
	
}

CString GetStrFilePath(LPCTSTR lpFile,BOOL bMakeUpper, BOOL bPostfix)//{"C:\Test"}
{
	CString str(lpFile);
	if (str.Find(_T('\\')) != -1)
		str = bPostfix ? str.Left(str.ReverseFind(_T('\\')) + 1) : str.Left(str.ReverseFind(_T('\\')));
	else if (str.Find(_T('/')) != -1)
		str = bPostfix ? str.Left(str.ReverseFind(_T('/')) + 1) : str.Left(str.ReverseFind(_T('/')));
	if (bMakeUpper)
		str.MakeUpper();
	return str;
}

CString GetStrFileTitle(LPCTSTR lpFile, BOOL bMakeUpper, BOOL bPostfix)
{
	CString str(lpFile);
	
	if (str.Find(_T('\\')) != -1)
		str = str.Right(str.GetLength()-str.ReverseFind(_T('\\'))-1);
	else if (str.Find(_T('/')) != -1)
		str = str.Right(str.GetLength()-str.ReverseFind(_T('/'))-1);
	
	if (!bPostfix)
	{
		if (str.ReverseFind(_T('.')) != -1)
			str = str.Left(str.ReverseFind(_T('.')));
	}
	if (bMakeUpper)
		str.MakeUpper();
	return str;
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
		delete[] pBuf;
		pBuf = NULL;
		return FALSE;
	}
	
	return TRUE;
}
