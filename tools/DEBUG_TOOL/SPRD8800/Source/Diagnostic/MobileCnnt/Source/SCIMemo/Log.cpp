// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Log.h"

#include <stdarg.h>

const int CLog::MAX_BUF_LEN = 512;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog()
{
    m_hFile = INVALID_HANDLE_VALUE;
}

CLog::~CLog()
{
    if( INVALID_HANDLE_VALUE != m_hFile )
    {
        CloseHandle( m_hFile );

        m_hFile = INVALID_HANDLE_VALUE;
    }
}

BOOL CLog::Open( LPCTSTR pszFileName )
{
    _ASSERTE( pszFileName != NULL );

    m_hFile = CreateFile( pszFileName, 
                          GENERIC_WRITE, 
                          FILE_SHARE_READ, 
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
        TRACE1( "Create log file fail: %d", GetLastError() );
        return FALSE;
    }
}

BOOL CLog::Log( LPVOID pBuffer, int nBufLen )
{
//	TRACE((LPCTSTR)pBuffer);

    if( INVALID_HANDLE_VALUE == m_hFile )
    {
        TRACE0("Not log the content! \r\n");
        return FALSE;
    }
    _ASSERTE( pBuffer != NULL && nBufLen > 0 );

    DWORD dwWritten = 0;
    BOOL  bRet = WriteFile( m_hFile, pBuffer, nBufLen, &dwWritten, NULL );	

    if( !bRet )
    {
        TRACE1( "Write log file fail: %d", GetLastError() );
    }
	
    return bRet;
}

BOOL CLog::LogEx( LPCTSTR pszFormat, ... )
{
    va_list vlst;

    va_start( vlst, pszFormat );

    _TCHAR szBuf[MAX_BUF_LEN];

    _vstprintf( szBuf, pszFormat, vlst );

    va_end( vlst );

    return Log( (LPVOID)szBuf, _tcslen(szBuf) * sizeof(_TCHAR) );
}

void CLog::Close( void )
{
    if( INVALID_HANDLE_VALUE != m_hFile )
    {
        CloseHandle( m_hFile );

        m_hFile = INVALID_HANDLE_VALUE;
    }
}