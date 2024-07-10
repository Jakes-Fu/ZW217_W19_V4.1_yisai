// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Log.h"
#include <atlconv.h>

#include <stdarg.h>

const int CLog::MAX_BUF_LEN = 1024;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog()
{
    m_hFile = INVALID_HANDLE_VALUE;
	m_pLogFile = NULL;
}

CLog::~CLog()
{
    if( INVALID_HANDLE_VALUE != m_hFile )
    {
        CloseHandle( m_hFile );

        m_hFile = INVALID_HANDLE_VALUE;
    }

    if( m_pLogFile != NULL )
    {
        fclose( m_pLogFile );
        m_pLogFile = NULL;
    }    
}

BOOL CLog::Open( LPCTSTR pszFileName )
{
    _ASSERTE( pszFileName != NULL );

    m_hFile = CreateFile( pszFileName, 
                          GENERIC_WRITE, 
                          0, 
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


////////////////////////////
void CLog::XLog(LPCTSTR lpszLog)
{
    if( m_pLogFile != NULL )
    {
        SYSTEMTIME  currentTime;
        GetLocalTime( &currentTime);
		int nByte = WideCharToMultiByte(CP_ACP,0,lpszLog,-1,NULL,0,NULL,NULL);
		char *psz = new char[nByte];
		WideCharToMultiByte(CP_ACP,0,lpszLog,-1,psz,nByte,NULL,NULL);
        fprintf( m_pLogFile, "%02d:%02d:%02d  %s\n",  
			currentTime.wHour,
			currentTime.wMinute,
			currentTime.wSecond,
			psz );
        TRACE( _T("====    %02d:%02d:%02d  %s\n"),  
			currentTime.wHour,
			currentTime.wMinute,
			currentTime.wSecond,
			lpszLog);

		delete []psz;
        
    }
}
BOOL CLog::OpenLog()
{	
	CloseLog();
	
    _TCHAR szLogFileName[ MAX_PATH ];
    GetModuleFilePath( NULL, szLogFileName );	
	_tcscat( szLogFileName, _T("\\Log"));
	CreateDirectory(szLogFileName,NULL);

    SYSTEMTIME  currentTime ;
    GetLocalTime( &currentTime);
    _stprintf( szLogFileName, _T("%s\\BMAF_%04d_%02d_%02d.log"), 
		szLogFileName,
		currentTime.wYear,
		currentTime.wMonth,
		currentTime.wDay );
	
    m_pLogFile = _tfopen( szLogFileName, _T("a+") );  
    if( m_pLogFile != NULL )
    {
        GetModuleFileName( AfxGetInstanceHandle(), 
			szLogFileName, MAX_PATH );
		int nByte = WideCharToMultiByte(CP_ACP,0,szLogFileName,-1,NULL,0,NULL,NULL);
		char *psz = new char[nByte];
		WideCharToMultiByte(CP_ACP,0,szLogFileName,-1,psz,nByte,NULL,NULL);
        fprintf( m_pLogFile, "===============================%s\n",  psz );
		delete []psz;
        return TRUE;
    }
    return FALSE;
}

void CLog::CloseLog()
{
    if( m_pLogFile != NULL )
    {
        fclose( m_pLogFile );
        m_pLogFile = NULL;
    }    
}

BOOL CLog::IsOpenLog()
{
	if(m_pLogFile != NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}