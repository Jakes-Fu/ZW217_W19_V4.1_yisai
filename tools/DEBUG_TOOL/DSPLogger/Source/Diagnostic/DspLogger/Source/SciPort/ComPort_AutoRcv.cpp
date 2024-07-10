// File Name:   ComPort_AutoRcv.cpp 
// Author:      anli.wei
// Date:        2006-02-14
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.

/************************************************************************/
/*  实现从串口自动读取数据的功能: 在使用自动接受时，使用了缓冲池结构，第*/
/* 一次时调用new来分配固定大小的内存块,上层应用使用完后应调用FreeMem来释*/
/* 放获取的内存指针，但是在本dll中并不一定实际调用delete，而是判断一下缓*/
/* 存数量，然后采用放入缓存或调用delete的操作。                         */
/************************************************************************/
#include "stdafx.h"
#include "COMPort.h"
#include "Debug.h"
#include "shlwapi.h"
#pragma warning( disable : 4127 4100 )

const int CCOMPort::MAX_SEND_TRYS    = 5;
// 缓存的最大数量，如果超出此数量，将不再分配内存
const int CCOMPort::MAX_BUF_COUNT    = 8192;//8M
// 保留的缓存数量，如果超出此数量，多余的将调用delete释放掉
const int CCOMPort::NORMAL_BUF_COUNT = 2048;//2M
//init memory pool count 
const int CCOMPort::ALLOC_BUF_COUNT  = 100;
// size of buffer
const int CCOMPort::BUF_SIZE         = 1024;//1k

void CCOMPort::SetReceiver( DWORD dwMsgID, DWORD dwReceiver, BOOL bIsThread /* = TRUE */)
{
    _ASSERTE( dwMsgID > WM_USER );

    m_bRcvIsThread = bIsThread;
    m_dwMsgID      = dwMsgID;

    if( bIsThread )
    {
        m_dwRcvThread = dwReceiver;
    }
    else
    {
        m_hRcvWnd = (HWND)dwReceiver;
    }
}

BOOL CCOMPort::StartAutoReceive(void)
{
    _ASSERTE( m_dwMsgID != INVALID_MSG_ID );
    _ASSERTE( m_hFile   != INVALID_HANDLE_VALUE );
    m_dwAllocCount = 0;
    m_bReceive     = TRUE;

// teana hu 2008-02-18
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);	
	_TCHAR szExePath[MAX_PATH] = {0};
	_TCHAR szName[MAX_PATH] = {0};
	GetModuleFileName(NULL, szExePath, MAX_PATH);
	PathRemoveFileSpec(szExePath);
	_stprintf(szName, _T("%s\\DSP_%d_%d_%d_%d_%d_%d_%d.org"), szExePath, sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute,sysTime.wSecond, sysTime.wMilliseconds);

	m_fpOrg = _tfopen(szName, _T("wb"));

    m_hRcvProc = CreateThread( 
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)RcvThread,
        this,
        0,
        NULL);
    
    if( m_hRcvProc == NULL ) return FALSE;

	//init a critical section object
	InitializeCriticalSection( &m_csForBuf );	

	//alloc init memory pool @andy add 2006-12-29
	LPBYTE pBuf = NULL;
	while (m_dqBuf.size() < ALLOC_BUF_COUNT)
	{
        pBuf = new BYTE[BUF_SIZE];
		if( pBuf != NULL )
		{
			++m_dwAllocCount;
			m_dqBuf.push_back(pBuf);
		}
	}		
	return TRUE;
}

void CCOMPort::StopAutoReceive(void)
{
    m_dwMsgID = INVALID_MSG_ID;

    if( m_bReceive && m_hFile != INVALID_HANDLE_VALUE )
    {
        _ASSERTE( m_hRcvProc != NULL );
        
        m_bReceive = FALSE;

        DWORD dwMask = 0;
        GetCommMask(m_hFile, &dwMask);
        SetCommMask(m_hFile, dwMask );

        if( WAIT_TIMEOUT == WaitForSingleObject(m_hRcvProc, 2000) )
        {
            TerminateThread(m_hRcvProc, (ULONG)-1);
        }

        CloseHandle(m_hRcvProc);
        m_hRcvProc = NULL;

        // 如果此assert发生，说明上层在收到数据处理后，没有调用FreeMem
        _ASSERTE( m_dqBuf.size() == m_dwAllocCount );

//		TRACE2(_T(" free-->alloc memory count: %d --- deque size:%d\n "),m_dwAllocCount,m_dqBuf.size());

        std::deque<LPBYTE>::iterator itB = m_dqBuf.begin();
        std::deque<LPBYTE>::iterator itE = m_dqBuf.end();

        std::deque<LPBYTE>::iterator it;
        for( it = itB; it < itE; ++it )
        {
            delete []*it;
        }
        m_dqBuf.clear();
        m_dwAllocCount = 0;

        ::DeleteCriticalSection( &m_csForBuf );
		//teana hu 2008-02-18
		if(m_fpOrg != NULL)
		{
			fclose(m_fpOrg);
			m_fpOrg = NULL;
		}
		CloseOrgFile();
    }
}

void CCOMPort::OpenOrgFile(LPCTSTR lpFileName)
{
	if(m_fpTPOrg != NULL)
	{
		CloseOrgFile();
	}
	
	DeleteOrgFile();

	if(lpFileName == NULL)
	{
		SYSTEMTIME sysTime;
		GetSystemTime(&sysTime);	
		_TCHAR szExePath[MAX_PATH] = {0};
		GetModuleFileName(NULL, szExePath, MAX_PATH);
		PathRemoveFileSpec(szExePath);
		_stprintf(m_szOrgFile, _T("%s\\Temp.org"), szExePath);
	}
	else
	{
		_tcscpy(m_szOrgFile, lpFileName);
	}

	m_fpTPOrg = _tfopen(m_szOrgFile, _T("wb"));
	if(m_fpTPOrg == NULL)
	{
		TRACE0(_T("Create *.org file failed!"));
	}
}

void CCOMPort::CloseOrgFile(void)
{
	if(m_fpTPOrg != NULL)
	{
		fclose(m_fpTPOrg);
		m_fpTPOrg = NULL;
	}
}

void CCOMPort::RenameOrgFile(LPCTSTR lpFileName)
{
	if(lpFileName != NULL)
	{
		MoveFile(m_szOrgFile, lpFileName);
	}
	memset(m_szOrgFile, 0, MAX_PATH);
}

void CCOMPort::DeleteOrgFile()
{
	if(m_szOrgFile[0] != '\0')
	{
		DeleteFile(m_szOrgFile);
		memset(m_szOrgFile, 0, MAX_PATH);
	}
}

DWORD WINAPI CCOMPort::RcvThread(LPVOID pParam )
{
    CCOMPort * pThis = (CCOMPort *)pParam;
    return pThis->AutoRcvProc();
}

DWORD CCOMPort::AutoRcvProc(void)
{
    _ASSERTE( m_hFile != INVALID_HANDLE_VALUE );

    DWORD   dwEvtMask = 0;
    DWORD   dwSize  = 0;
    DWORD   dwRead  = 0;
    DWORD   dwErr   = 0;
    LPBYTE  pRcvBuf = NULL;
    COMSTAT comStat = { 0 };

    pRcvBuf = NULL;
    while( m_bReceive )
    {
        comStat.cbInQue = 0;
        if( !ClearCommError( m_hFile, &dwErr, &comStat ) )
        {
            if( ERROR_ACCESS_DENIED == GetLastError() )
            {
                TRACE0(_T(" auto receive exit \n"));
                break;
            }
        }

        if( comStat.cbInQue ) 
        {
            pRcvBuf = ( pRcvBuf != NULL ? pRcvBuf : AllocMem() );
            if( NULL == pRcvBuf )
            {
                break;
            }

            dwSize = comStat.cbInQue > BUF_SIZE ? BUF_SIZE : comStat.cbInQue;
            dwRead = 0;

            TRACE1(_T("---->COMSTAT->cbInQue size:%d\n"),comStat.cbInQue);

            if( !ReadFile(m_hFile, pRcvBuf, dwSize, &dwRead, &m_osRead) )
            {
                ClearCommError( m_hFile, &dwErr, &comStat );
            }

			fwrite(pRcvBuf, sizeof(BYTE), dwRead, m_fpOrg);
			fflush(m_fpOrg);
			if(m_fpTPOrg != NULL)
			{
				fwrite(pRcvBuf, sizeof(BYTE), dwRead, m_fpTPOrg);
				fflush(m_fpTPOrg);
			}
            TRACE1(_T("<----Read size:%d\n"),dwRead);

            if( dwRead && SendDataToReciver(pRcvBuf, dwRead) )
            {
                //Sleep(2); //important !!! (it can improve performance 5%)
                pRcvBuf = NULL;
            }
			
			// Because ReadUart() may not read all the cbInQue data,
            // and if not, the rest data won't generate the EV_RXCHAR event,
            // so continue to ClearCommError() to ensure to read all the data.
			
			continue;
        }

        dwEvtMask = 0;
        if( !::WaitCommEvent(m_hFile, &dwEvtMask, &m_osWait) )
        {			
            DWORD dwError = GetLastError();
            if( ERROR_IO_PENDING == dwError ) 
            {
                DWORD dwTrans = 0;
                GetOverlappedResult( m_hFile, &m_osWait, &dwTrans, TRUE );
            }
        }
        ::ResetEvent(m_osWait.hEvent);        
    }

    if( pRcvBuf != NULL )
    {
        FreeMem( pRcvBuf );
    }

    return 0L;
}

void CCOMPort::FreeMem( LPBYTE pBuf )
{
    _ASSERTE( pBuf != NULL );
    
    EnterCriticalSection( &m_csForBuf );

    if( m_dqBuf.size() >= NORMAL_BUF_COUNT )
    {
        delete []pBuf;
        --m_dwAllocCount;
    }
    else
    {
        m_dqBuf.push_back(pBuf);
    }

    LeaveCriticalSection( &m_csForBuf );
}

__inline LPBYTE CCOMPort::AllocMem(void)
{
    LPBYTE pBuf		= NULL;
	int	   nCount	= 0;
    EnterCriticalSection( &m_csForBuf );
    
	DWORD dwDQSize = m_dqBuf.size();
    if( dwDQSize )
    {
        pBuf = *(m_dqBuf.begin());
        m_dqBuf.pop_front();
    }
    else if( m_dwAllocCount < MAX_BUF_COUNT )
    {
		//alloc ALLOC_BUF_COUNT memory blocks
		while (nCount++ < ALLOC_BUF_COUNT)
		{
			pBuf = new BYTE[BUF_SIZE];
			if( pBuf != NULL )
			{
				++m_dwAllocCount;
				m_dqBuf.push_back(pBuf);
			}
		}

		//get buffer 
		pBuf = *(m_dqBuf.begin());
        m_dqBuf.pop_front();	        
    }
    else
    {
        _ASSERTE( 0 );
    }
    
//	TRACE2(_T("--->alloc memory count: %d ---memory deque count: %d<----\n"), m_dwAllocCount, m_dqBuf.size());

    LeaveCriticalSection( &m_csForBuf );

    return pBuf;
}

__inline BOOL CCOMPort::SendDataToReciver( LPCVOID pData, DWORD dwSize )
{
    _ASSERTE( pData != NULL && dwSize > 0 );
    if (m_dwMsgID == INVALID_MSG_ID) return FALSE;

	BOOL bPost = FALSE;
    if( m_bRcvIsThread )
    {
        for( int i = 0; i < MAX_SEND_TRYS; ++i )
        {
            bPost = PostThreadMessage(m_dwRcvThread, m_dwMsgID, (WPARAM)pData, dwSize);
            if( bPost || (!bPost && ERROR_INVALID_THREAD_ID == GetLastError()))
            {	
                return bPost;
            }
        }        
    }
    else
    {
        bPost = PostMessage( m_hRcvWnd, m_dwMsgID, (WPARAM)pData, dwSize );
    }

    return bPost;
}