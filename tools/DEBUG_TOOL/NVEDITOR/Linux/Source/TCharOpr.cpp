// TCharOpr.cpp: implementation of the CTCharOpr class.
//
//////////////////////////////////////////////////////////////////////

#include "TCharOpr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTCharOpr::CTCharOpr()
{

}

CTCharOpr::~CTCharOpr()
{

}



void CTCharOpr::StrTrim(TCHAR* pStr)  
{  
	if(NULL == pStr)
	{
		return;
	}
    TCHAR* pTmp = pStr;  
	
    while ( *pTmp != _T('\0') )   
    {  
        if ( *pStr != _T(' ') &&  *pStr != _T('\t'))  
        {  
            *pTmp++ = *pStr;  
        }  
        ++pStr;  
    }  
    *pTmp = _T('\0');  
}  


void CTCharOpr::StrRTrim(TCHAR* pStr)  
{  
	if(NULL == pStr)
	{
		return;
	}
    TCHAR* pTmp = pStr+_tcslen(pStr)-1;  
	
    while ( *pTmp == _T(' ') || *pTmp == _T('\t') )   
    {  
        *pTmp = _T('\0');
        --pTmp;  
    }  
}  

void CTCharOpr::StrRTrim(TCHAR* pStr,TCHAR ch,BOOL bRmSpace /*= TRUE*/)  
{  
	if(NULL == pStr)
	{
		return;
	}
    TCHAR* pTmp = pStr+_tcslen(pStr)-1;  
	if(bRmSpace)
	{
		while ( *pTmp == _T(' ') || *pTmp == _T('\t') || *pTmp == ch )   
	    {  
	        *pTmp = _T('\0');
	        --pTmp;  
	    }  
	}
	else
	{
		while ( *pTmp == ch )   
	    {  
	        *pTmp = _T('\0');
	        --pTmp;  
	    }  
	}
    
} 


void CTCharOpr::StrLTrim(TCHAR* pStr)  
{  
	if(NULL == pStr)
	{
		return;
	}
    TCHAR* pTmp = pStr;  
	
    while (*pTmp == _T(' ') || *pTmp == _T('\t') )   
    {  
        pTmp++;  
    }  
    while( *pTmp != _T('\0') )  
    {  
        *pStr = *pTmp;  
        ++pStr;  
        ++pTmp;  
    }  
    *pStr = _T('\0');  
}

void CTCharOpr::StrLTrim(TCHAR* pStr,TCHAR ch)  
{  
	if(NULL == pStr)
	{
		return;
	}
    TCHAR* pTmp = pStr;  
	
    while (*pTmp == _T(' ') || *pTmp == _T('\t') || *pTmp == ch )   
    {  
        pTmp++;  
    }  
    while( *pTmp != _T('\0') )  
    {  
        *pStr = *pTmp;  
        ++pStr;  
        ++pTmp;  
    }  
    *pStr = _T('\0');  
}
void CTCharOpr::MakeUpper(TCHAR* pStr)  
{
    if(NULL == pStr || 0 == _tcslen( pStr ))
    {
        return ;
    }
    int nLen = _tcslen( pStr );  
#ifdef WIN32
    _tcsupr_s(pStr,nLen+1);
#else //linux
    char* pTmp = pStr;
    while (_T('\0') != (*pTmp))
    {
        *pTmp = (TCHAR)toupper(*pTmp);
        ++pTmp;
    }
#endif
}
void CTCharOpr::MakeLower(TCHAR* pStr)
{
    if(NULL == pStr || 0 == _tcslen( pStr ))
    {
        return ;
    }

    int nLen = _tcslen( pStr );  
#ifdef WIN32
    _tcslwr_s(pStr,nLen+1);
#else //linux
    char* pTmp = pStr;
    while (_T('\0') != (*pTmp))
    {
        *pTmp = (TCHAR)tolower(*pTmp);
        ++pTmp;
    }
#endif
}   
BOOL CTCharOpr::IsValidValue( TCHAR* lpszValue )
{
    if(NULL == lpszValue || 0 == _tcslen( lpszValue ))
    {
        return FALSE;
    }
    StrLTrim(lpszValue);
    StrRTrim(lpszValue);
    int nLen = _tcslen( lpszValue );  
    MakeLower(lpszValue);

    if (_tcsnicmp(lpszValue,_T( "0x" ),2))
    {
        int nIndex = _tcsspn( lpszValue, _T("0123456789") ); 
        return ( nLen == nIndex ) ? TRUE : FALSE;  
    }
    else
    {
        int nIndex = _tcsspn( lpszValue+2, _T("0123456789abcdef") ); 
        return ( nLen == nIndex ) ? TRUE : FALSE;  
    }
}