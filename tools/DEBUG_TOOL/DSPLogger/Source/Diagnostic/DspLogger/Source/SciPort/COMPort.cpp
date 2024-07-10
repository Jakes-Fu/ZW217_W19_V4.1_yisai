// File Name:   ComPort.cpp 
// Author:      anli.wei
// Date:        2004-09-20
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.

/************************************************************************/
/* 实现连接手机的接口之串口方式。在打开串口时采用了重叠操作，			*/
/* 但目前的读写是采用的阻塞方式，这里采用重叠操作是为了方便				*/
/* 将来可能需要的扩充。                                                 */
/************************************************************************/
 
#include "stdafx.h"
#include "COMPort.h"

#pragma warning( disable : 4127 4100 )

const int CCOMPort::COM_INPUT_BUF_SIZE      = 1024*8;
const int CCOMPort::COM_OUTPUT_BUF_SIZE     = 1024*2;

_TCHAR CCOMPort::m_szErrMsg[MAX_ERRMSG_LEN] = { 0 };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCOMPort::CCOMPort()
{
    m_hFile          = INVALID_HANDLE_VALUE;
    m_osRead.hEvent  = NULL;
    m_osWrite.hEvent = NULL;
    m_osWait.hEvent  = NULL;
    m_nCount         = 0; 
    m_dwMsgID        = INVALID_MSG_ID;
    m_bReceive       = FALSE;
	m_hRcvProc       = NULL;
    m_dwAllocCount   = 0;

    m_arrOL[0] = &m_osRead;
    m_arrOL[1] = &m_osWrite;
    m_arrOL[2] = &m_osWait;
	m_fpOrg = NULL;
	m_fpTPOrg = NULL;
	memset(m_szOrgFile, 0, MAX_PATH);
	m_bGenerateOrgFile = TRUE;
}//lint !e1401

CCOMPort::~CCOMPort()
{
    Close();
}

BOOL CCOMPort::StartLog( LPCTSTR pszFileName )
{
    _ASSERTE( pszFileName != NULL );
    return m_Log.Open(pszFileName);
}

void CCOMPort::StopLog( void )
{
    m_Log.Close();
}

int CCOMPort::GetPortCount(BOOL bUseable /* = TRUE */)
{
    m_nCount = 0;
    memset(m_arrName, 0, sizeof(m_arrName));

    _TCHAR szName[PORT_NAME_MAX_LEN] = { 0 };
    HANDLE hFile = INVALID_HANDLE_VALUE;

    for( int i = 1; i < MAX_COMPORT_NUM; ++i )
    {
        _stprintf(szName, _T("\\\\.\\COM%d"), i);
        hFile = ::CreateFile( szName,
                              GENERIC_READ | GENERIC_WRITE,
		                      0,
                              NULL,
                              OPEN_EXISTING,
		                      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		                      NULL );

        if( hFile != INVALID_HANDLE_VALUE || 
            (!bUseable && ERROR_ACCESS_DENIED == GetLastError()) )
        {
              
            _tcscpy( m_arrName[m_nCount], szName + 4 );// delete 4 char
            m_nCount++;
            CloseHandle(hFile);
        }
    }
    

    return m_nCount;
}

int CCOMPort::GetUseableCount(void)
{
    return GetPortCount(TRUE);
}

int CCOMPort::GetPortCount(void) 
{
    return GetPortCount(FALSE);
}

LPCTSTR CCOMPort::GetName(int nIdx)
{
    if( nIdx < 0 || nIdx >= m_nCount )
    {
        return NULL;
    }
    
    return m_arrName[nIdx];
}

BOOL CCOMPort::Open(PCPORT_ATTRIBUTE pAttr)
{
	_ASSERTE( pAttr != NULL );
	
	memcpy(&m_PortAttr, pAttr, sizeof(m_PortAttr));

	return Open( pAttr->Com.szName, pAttr->Com.dwBaudRate );
}

BOOL CCOMPort::Open( LPCTSTR pszName, DWORD dwBaudrate )
{
    _ASSERTE( pszName != NULL );
    _ASSERTE( dwBaudrate > 0 );

    const _TCHAR szPre[] = _T("\\\\.\\");

    _TCHAR szName[PORT_NAME_MAX_LEN] = { 0 };
    LPCTSTR pPrefix = _tcsstr(pszName, szPre);

    if( pPrefix != NULL )
    {
        _tcscpy(szName, pszName);
    }
    else
    {
        _tcscpy(szName, szPre);
        _tcscat(szName, pszName);
    }
    
    m_hFile = ::CreateFile( szName,
                            GENERIC_READ|GENERIC_WRITE,
		                    0,
                            NULL,
                            OPEN_EXISTING,
		                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		                    NULL );

	if( INVALID_HANDLE_VALUE == m_hFile )
	{		
        TRACE2(_T(" Open %s Error, ErrCode: %d \r\n"), pszName, GetLastError() );
        m_Log.LogEx(_T(" Open %s Error, ErrCode: %d \r\n"), pszName, GetLastError() );

		return FALSE;
	}
	
	#define FAIL_RET_FALSE() { Close(); return FALSE; }

    // Clean up the UART buffer
    if( !::PurgeComm(m_hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR) )
    {
		TRACE2(_T(" PurgeComm %s Error %d\r\n"), pszName, GetLastError() );
        m_Log.LogEx(_T(" PurgeComm %s Error %d\r\n"), pszName, GetLastError() );

        FAIL_RET_FALSE();
    }

    DCB dcb = { sizeof(DCB) };
    if( !::GetCommState( m_hFile, &dcb ) )
    {
		TRACE2(_T(" GetCommState %s Error %d\r\n"), pszName, GetLastError() );
        m_Log.LogEx(_T(" GetCommState %s Error %d\r\n"), pszName, GetLastError() );

        FAIL_RET_FALSE();
    }
	
    dcb.BaudRate  = dwBaudrate;
    dcb.ByteSize  = 8;
    dcb.Parity    = NOPARITY;
    dcb.StopBits  = ONESTOPBIT;
    dcb.fBinary   = TRUE;
    dcb.fParity   = FALSE;

    if( !::SetCommState(m_hFile, &dcb) )
    {
		TRACE2(_T(" SetCommState %s Error %d\r\n"), pszName, GetLastError() );
        m_Log.LogEx(_T(" SetCommState %s Error %d\r\n"), pszName, GetLastError() );

        FAIL_RET_FALSE();
    }

    if( !::SetupComm(m_hFile, COM_INPUT_BUF_SIZE, COM_OUTPUT_BUF_SIZE) )
    {
		TRACE2(_T(" SetupComm %s Error %d\r\n"), pszName, GetLastError() );
        m_Log.LogEx(_T(" SetupComm %s Error %d\r\n"), pszName, GetLastError() );

        FAIL_RET_FALSE();
    }

    // Set the timeout option
    COMMTIMEOUTS cto;
/*
    cto.ReadIntervalTimeout         = 5;
    cto.ReadTotalTimeoutMultiplier  = 1;
    cto.ReadTotalTimeoutConstant    = 100;
    cto.WriteTotalTimeoutMultiplier = 50;
    cto.WriteTotalTimeoutConstant   = 2000;
*/

	cto.ReadIntervalTimeout          = 5;
    cto.ReadTotalTimeoutMultiplier   = 1;
    cto.ReadTotalTimeoutConstant     = 100;
    cto.WriteTotalTimeoutMultiplier  = 50;
    cto.WriteTotalTimeoutConstant    = 2000;

    if( !::SetCommTimeouts(m_hFile, &cto) )
    {
		TRACE2(_T(" SetCommTimeouts %s Error %d\r\n"), pszName, GetLastError() );
        m_Log.LogEx(_T(" SetCommTimeouts %s Error %d\r\n"), pszName, GetLastError() );

        FAIL_RET_FALSE();
    }

    if( !::SetCommMask(m_hFile, EV_RXCHAR) )
	{
		TRACE2(_T(" SetCommMask %s Error %d\r\n"), pszName, GetLastError() );
        m_Log.LogEx(_T(" SetCommMask %s Error %d\r\n"), pszName, GetLastError() );

        FAIL_RET_FALSE();
	}


    _ASSERTE( sizeof(m_arrOL) / sizeof(m_arrOL[0]) == MAX_OVL_COUNT );

    // Event must be manual reset
    for( int i = 0; i < MAX_OVL_COUNT; ++i )
    {
        m_arrOL[i]->Offset     = 0;
        m_arrOL[i]->OffsetHigh = 0;
        m_arrOL[i]->hEvent     = CreateEvent(NULL, TRUE, FALSE, NULL);
        if( NULL == m_arrOL[i]->hEvent )
        {
            FAIL_RET_FALSE();
        }
    }

    return TRUE;
}

BOOL CCOMPort::GetAttribute(PPORT_ATTRIBUTE pAttr)
{
	_ASSERTE( pAttr != NULL );

	memcpy( pAttr, &m_PortAttr, sizeof(m_PortAttr) );

	return m_hFile != INVALID_HANDLE_VALUE;
}

BOOL CCOMPort::SetDTR( BOOL bSignal )
{
	_ASSERTE( m_hFile != INVALID_HANDLE_VALUE );

	DWORD dwFun = bSignal ? SETDTR : CLRDTR;

	BOOL bRet = ::EscapeCommFunction( m_hFile, dwFun );
	if( !bRet ) 
    {
		TRACE1(_T("SetDTR fail, ErrCode is: %d \r\n"), GetLastError() );
        m_Log.LogEx(_T("SetDTR fail, ErrCode is: %d \r\n"), GetLastError() );
    }

	return bRet;
}

BOOL CCOMPort::GetDSR(BOOL *pSignal)
{
	_ASSERTE( m_hFile != INVALID_HANDLE_VALUE );
    _ASSERTE( pSignal != NULL );

	*pSignal = FALSE;

	DWORD dwStat = 0;
	BOOL  bRet   = GetCommModemStatus( m_hFile, &dwStat );
	if( dwStat & MS_DSR_ON )
    {
		*pSignal = TRUE;
    }

	if( !bRet ) 
    {
		TRACE1(_T("GetDSR fail, ErrCode is: %d \r\n"), GetLastError() );
        m_Log.LogEx(_T("GetDSR fail, ErrCode is: %d \r\n"), GetLastError() );
    }

	return bRet;
}

BOOL CCOMPort::PurgeAll(void)
{
	_ASSERTE( m_hFile != INVALID_HANDLE_VALUE );

	BOOL bRet = ::PurgeComm( m_hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );

	return bRet;
}

BOOL CCOMPort::PurgeInput(void)
{
	_ASSERTE( m_hFile != INVALID_HANDLE_VALUE );

	return ::PurgeComm( m_hFile, PURGE_RXABORT | PURGE_RXCLEAR );
}

BOOL CCOMPort::PurgeOutput(void)
{
	_ASSERTE( m_hFile != INVALID_HANDLE_VALUE );

	return ::PurgeComm( m_hFile, PURGE_TXABORT | PURGE_TXCLEAR );
}

BOOL CCOMPort::Read( LPVOID  pBuf, 
                     DWORD   dwBytesToRead, 
					 LPDWORD pReadedBytes /* = NULL */, 
                     DWORD   dwTimeOut /* = 0 */
                   )
{
    _ASSERTE( pBuf    != NULL );
    _ASSERTE( m_hFile != INVALID_HANDLE_VALUE );

    if( pReadedBytes != NULL )
        *pReadedBytes = 0;

    COMSTAT comStat   = { 0 };
	DWORD   dwError   = 0;
	if( !::ClearCommError(m_hFile, &dwError, &comStat) )
    {
        TRACE1(_T("ClearCommError fail for read %d\r\n"), GetLastError());
        m_Log.LogEx( _T("ClearCommError fail read %d\r\n"), GetLastError() );

        return FALSE;
    }

    DWORD   dwEvtMask = 0;
    ::ResetEvent(m_osWait.hEvent);

    BOOL bWait = ::WaitCommEvent(m_hFile, &dwEvtMask, &m_osWait);
    if( !comStat.cbInQue && !bWait )
    {
		//TRACE1(_T("WaitCommEvent mask is : %d\r\n"), dwEvtMask);
        if( ERROR_IO_PENDING == ::GetLastError() )
        {			
			DWORD dwWait = ::WaitForSingleObject(m_osWait.hEvent, dwTimeOut);
			ResetEvent(m_osWait.hEvent);
			//TRACE1(_T("WaitForSingleObject result is : %d\r\n"), dwWait);
			if( WAIT_TIMEOUT == dwWait )
			{
				// ! Important!
				// 这里调用Get、Set CommMask只是为了让WaitCommEvent完成等待，
				// 否则第二次调用WaitCommEvent是无效的
				DWORD dwMask = 0;
				GetCommMask(m_hFile, &dwMask);
				SetCommMask(m_hFile, dwMask );
			
				TRACE1(_T("== Read Com Timeout: %d(ms) ==\r\n"), dwTimeOut);
                m_Log.LogEx(_T("== Read Com Timeout: %d(ms) ==\r\n"), dwTimeOut);

				return FALSE;
			}
            else
            {
                //TRACE0(_T("== Read some char ==\r\n"));
            }
		}
    }

    BOOL  bRet        = TRUE;
	DWORD dwWait      = 0;
    DWORD dwReadBytes = 0;   

	::GetOverlappedResult(m_hFile, &m_osWait, &dwWait, FALSE);
	::ResetEvent(m_osWait.hEvent);	
    ::ResetEvent(m_osRead.hEvent);
    
    if( !::ReadFile(m_hFile, pBuf, dwBytesToRead, &dwReadBytes, &m_osRead) )
    {
        if( ERROR_IO_PENDING == GetLastError() )
        {
            DWORD dwRead = 0;
            if( !::GetOverlappedResult(m_hFile, &m_osRead, &dwRead, TRUE) )
            {	
                TRACE1(_T(" GetOverlappedResult fail in Read, ErrCode is %d \r\n"), GetLastError());
                m_Log.LogEx(_T(" GetOverlappedResult fail in Read, ErrCode is %d \r\n"), GetLastError());

				bRet = FALSE;
            }
            dwReadBytes += dwRead;
        }
		else
		{
			TRACE1(_T(" Read COM fail, the ErrCode is %d \r\n"), GetLastError() );
			bRet = FALSE;
		}
    }

    if( dwReadBytes )
    {
		static _TCHAR szReadFlag[] = _T("======== Read ========\r\n");
#ifndef _UNICODE        
        m_Log.Log( szReadFlag, _tcslen(szReadFlag) * sizeof(_TCHAR) );
#else
		m_Log.LogEx(szReadFlag);
#endif
		
        m_Log.Log( pBuf, dwReadBytes );
        m_Log.LogEx(_T("\r\n"));
    }

    if( pReadedBytes != NULL )
        *pReadedBytes = dwReadBytes;

	return (0 == dwReadBytes ? FALSE : bRet);
}

BOOL CCOMPort::QuickRead( LPVOID  pBuf,  
                          DWORD   dwBytesToRead,  
		                  LPDWORD pReadedBytes /* = NULL */
                        )
{
    _ASSERTE( pBuf    != NULL );
    _ASSERTE( m_hFile != INVALID_HANDLE_VALUE );

    if( pReadedBytes != NULL )
        *pReadedBytes = 0;

    COMSTAT comStat   = { 0 };
	DWORD   dwError   = 0;
	if( !::ClearCommError(m_hFile, &dwError, &comStat) )
    {
        TRACE1(_T("ClearCommError fail for read %d\r\n"), GetLastError());
        m_Log.LogEx( _T("ClearCommError fail read %d\r\n"), GetLastError() );

        return FALSE;
    }

    if( 0 == comStat.cbInQue )
    {
        return FALSE;
    }

    DWORD dwReadBytes = 0;
    BOOL  bRet        = TRUE;
    ::ResetEvent(m_osRead.hEvent);
    if( !::ReadFile(m_hFile, pBuf, comStat.cbInQue, &dwReadBytes, &m_osRead) )
    {
        if( ERROR_IO_PENDING == GetLastError() )
        {
            DWORD dwRead = 0;
            if( !::GetOverlappedResult(m_hFile, &m_osRead, &dwRead, TRUE) )
            {	
                bRet = FALSE;
                TRACE1(_T(" GetOverlappedResult fail in Read, ErrCode is %d \r\n"), GetLastError());
                m_Log.LogEx(_T(" GetOverlappedResult fail in Read, ErrCode is %d \r\n"), GetLastError());
            }
            dwReadBytes += dwRead;
        }
		else
		{
            bRet = FALSE;
			TRACE1(_T(" Read COM fail, the ErrCode is %d \r\n"), GetLastError() );
		}
    }

    if( dwReadBytes )
    {
		static _TCHAR szReadFlag[] = _T("======== Read ========\r\n");
#ifndef _UNICODE        
        m_Log.Log( szReadFlag, _tcslen(szReadFlag) * sizeof(_TCHAR) );
#else
		m_Log.LogEx(szReadFlag);
#endif
		
        m_Log.Log( pBuf, dwReadBytes );
        m_Log.LogEx(_T("\r\n"));
    }

    if( pReadedBytes != NULL )
        *pReadedBytes = dwReadBytes;

	return (0 == dwReadBytes ? FALSE : bRet);
}

void CCOMPort::CancelRead(void)
{
	if( m_hFile != INVALID_HANDLE_VALUE )
	{
		DWORD dwMask = 0;
		GetCommMask(m_hFile, &dwMask);
		SetCommMask(m_hFile, dwMask );
		if( m_osWait.hEvent != NULL )
			SetEvent(m_osWait.hEvent);

		if( m_osRead.hEvent != NULL )
			SetEvent(m_osRead.hEvent);

		PurgeInput();
	}
}

BOOL CCOMPort::Write(LPVOID pBuf, DWORD dwBytesToWrite, LPDWORD pWritedBytes/* = NULL */)
{
    _ASSERTE( pBuf != NULL && dwBytesToWrite > 0 );
    _ASSERTE( m_hFile != INVALID_HANDLE_VALUE );

	if( pWritedBytes != NULL )
		*pWritedBytes = 0;

    DWORD   dwError;
    COMSTAT comStat;
    if( !ClearCommError(m_hFile, &dwError, &comStat) )
    {
        TRACE1(_T("ClearCommError fail for write %d\r\n"), GetLastError());
        m_Log.LogEx( _T("ClearCommError fail for write %d\r\n"), GetLastError() );

        return FALSE;
    }

    DWORD dwWritedBytes = 0;
    ::ResetEvent( m_osWrite.hEvent );
    if( !WriteFile(m_hFile, pBuf, dwBytesToWrite, &dwWritedBytes, &m_osWrite) )
    {
        if( ERROR_IO_PENDING == GetLastError() )
        {
            DWORD dwWrite = 0;
            if( !GetOverlappedResult( m_hFile, &m_osWrite, &dwWrite, TRUE ) )
            {
				TRACE1(_T("GetOverlappedResult fail in Write, ErrCode is %d \r\n"), GetLastError());
                m_Log.LogEx(_T("GetOverlappedResult fail in Write, ErrCode is %d !!\r\n"), GetLastError());
            }
            dwWritedBytes += dwWrite;
        }
		else
		{
			TRACE1(_T("Write COM fail %d \r\n"), GetLastError());
            m_Log.LogEx(_T("Write COM fail %d \r\n"), GetLastError());
		}
    }

	if( pWritedBytes != NULL )
		*pWritedBytes = dwWritedBytes;

    if( dwBytesToWrite )
    {
        static _TCHAR szWriteFlag[] = _T("======== Write ========\r\n");
#ifndef _UNICODE        
        m_Log.Log( szWriteFlag, _tcslen(szWriteFlag) * sizeof(_TCHAR) );
#else
		m_Log.LogEx(szWriteFlag);
#endif
        m_Log.Log( pBuf, dwWritedBytes );
        m_Log.LogEx(_T("\r\n"));
    }

    return dwWritedBytes == dwBytesToWrite;
}

void CCOMPort::Close(void)
{
    if(m_hFile != INVALID_HANDLE_VALUE)
    {
		SetDTR(FALSE);

        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }

    for( int i = 0; i < MAX_OVL_COUNT; ++i )
    {
        if( m_arrOL[i]->hEvent != NULL )
        {
            CloseHandle( m_arrOL[i]->hEvent );
            m_arrOL[i]->hEvent = NULL;
        }
    }
}

LPCTSTR CCOMPort::GetErrMsg(void)
{
    memset(m_szErrMsg, 0, MAX_ERRMSG_LEN);

    ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                     NULL,
                     GetLastError(),
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                     (LPTSTR)m_szErrMsg,
                     MAX_ERRMSG_LEN,
                     NULL 
                    );

    return m_szErrMsg;
}

void CCOMPort::Release(void)
{
	if( IsOpened() )
    {
		Close();
    }

	delete this;
}
