// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Log.h"

const int CLog::MAX_BUF_LEN = 512;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog()
{
    m_hFile = INVALID_HANDLE_VALUE;
	ZeroMemory(m_szBufA,sizeof(m_szBufA));
	ZeroMemory(m_szBufW,sizeof(m_szBufW));
}

CLog::~CLog()
{
    Close();
}

BOOL CLog::Open( LPCTSTR pszFileName )
{
    //_ASSERTE( pszFileName != NULL );
	if(pszFileName == NULL)
	{
		return FALSE;
	}

    m_hFile = CreateFile( pszFileName, 
                          GENERIC_WRITE, 
                          FILE_SHARE_READ,  //@hongliang.xin 2008-1-23 for share read log file
                          NULL, 
                          CREATE_ALWAYS,
						  FILE_ATTRIBUTE_NORMAL, 
                          NULL 
                         );
    if( INVALID_HANDLE_VALUE != m_hFile )
    {
        return TRUE;
    }
    else
    {
        TRACE1( _T("Create log file fail: %d"), GetLastError() );
        return FALSE;
    }
}

BOOL CLog::Log( LPVOID pBuffer, int nBufLen )
{
    if( INVALID_HANDLE_VALUE == m_hFile )
    {
        //TRACE0("Not log the content! \r\n");
        return FALSE;
    }

    //_ASSERTE( pBuffer != NULL && nBufLen > 0 );
	if(pBuffer == NULL || nBufLen == 0 )
		return FALSE;

    DWORD dwWritten = 0;
    BOOL  bRet = WriteFile( m_hFile, pBuffer, nBufLen, &dwWritten, NULL );
    if( !bRet )
    {
        TRACE1( _T("Write log file fail: %d"), GetLastError() );
    }

    return bRet;
}

BOOL CLog::LogEx( LPCTSTR pszFormat, ... )
{
    va_list vlst;

    va_start( vlst, pszFormat );

    //_TCHAR szBuf[MAX_BUF_LEN];

    _vstprintf( m_szBufW, pszFormat, vlst );

    va_end( vlst );
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,m_szBufW,-1,m_szBufA,LOG_MAX_BUF_LEN,NULL,NULL);
	 return Log( (LPVOID)m_szBufA, strlen(m_szBufA));
#else
	 return Log( (LPVOID)m_szBufW, _tcslen(m_szBufW) * sizeof(_TCHAR) );
#endif
}

void CLog::Close( void )
{
    if( INVALID_HANDLE_VALUE != m_hFile )
    {
        CloseHandle( m_hFile );

        m_hFile = INVALID_HANDLE_VALUE;
    }
}