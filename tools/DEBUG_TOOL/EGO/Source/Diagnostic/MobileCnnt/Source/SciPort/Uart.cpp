// Uart.cpp: implementation of the CUart class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Uart.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUart::CUart()
{
	m_pChannel = NULL;
	CreateChannel(&m_pChannel,CHANNEL_TYPE_COM);

	//teana hu 2009.11.02
	memset(m_szRcvBuf, 0, MAX_BUF_LENGTH);
	m_dwRcvLen = 0;

	m_dwProDataLen = 0;
	memset(m_szProData, 0, MAX_RCV_BUF_1);
	memset(m_szPostData, 0, MAX_RCV_BUF_1);

	//for auto report
	m_dwThreadId = 0;
	m_dwGuiMsgId = 0;
	m_dwMsgId = 0;
	m_hGuiWnd = NULL;
	m_hThread = NULL;
	m_hEventWaitResult = NULL;
	m_hEventFlag = NULL;
	
	m_bCMT = FALSE;
	memset(m_szCMT, 0, MAX_RCV_BUF_1);
	
	m_bCDS = FALSE;
	memset(m_szCDS, 0, MAX_RCV_BUF_1);

	m_bRING = FALSE;
	memset(m_szRING, 0, MAX_RCV_BUF_1);

	m_bSyn = FALSE;
	m_bPostAll = FALSE;
	m_dwReadTotal= 0;
	m_bOpen = FALSE;

	m_bLoadSMS = FALSE;
}

CUart::~CUart()
{
	StopAutoReceive();
	if(m_pChannel != NULL)
	{
		ReleaseChannel(m_pChannel);
		m_pChannel = NULL;
	}

	m_hGuiWnd = NULL;
	m_hThread = NULL;
	m_hEventWaitResult = NULL;
	m_hEventFlag = NULL;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BOOL  CUart::StartLog( LPCTSTR pszFileName )
{
	if(m_pChannel == NULL)
		return FALSE;

	return m_pChannel->InitLog(pszFileName,ISpLog::typeText,(UINT)-1,NULL);	
}

void  CUart::StopLog( void )
{
	return;
}

int  CUart::GetUseableCount(void)
{
	_ASSERTE(0);
	return 0;
}

int  CUart::GetPortCount(void)
{
	_ASSERTE(0);
	return 0;
}

LPCTSTR CUart::GetName(int /*nIdx*/)
{
	_ASSERTE(0);
	return NULL;
}

BOOL CUart::Open(PCPORT_ATTRIBUTE pAttr)
{
	if(pAttr == NULL)
	{
		m_bOpen = FALSE;
		return FALSE;
	}
	
	memcpy(&m_PortAttr,pAttr,sizeof(PORT_ATTRIBUTE));
	CHANNEL_ATTRIBUTE ca;
	ca.ChannelType = CHANNEL_TYPE_COM;
	ca.Com.dwBaudRate = pAttr->Com.dwBaudRate;

	_TCHAR* pCom = _tcsrchr(pAttr->Com.szName,_T('M'));
	if(pCom == NULL)
	{
		m_bOpen = FALSE;
		return FALSE;
	}
    
	ca.Com.dwPortNum = _ttoi(pCom+1);
	m_bOpen = m_pChannel->Open(&ca);

	return m_bOpen;
}

BOOL CUart::GetAttribute(PPORT_ATTRIBUTE pAttr)
{
	if(pAttr == NULL)
		return FALSE;

	memcpy(pAttr,&m_PortAttr,sizeof(PORT_ATTRIBUTE));
	return TRUE;
}

BOOL CUart::SetDTR( BOOL bSignal )
{
	UNREFERENCED_PARAMETER(bSignal);
	_ASSERTE(0);
	return FALSE;
}

BOOL CUart::GetDSR( BOOL *pSignal )
{
	UNREFERENCED_PARAMETER(pSignal);
	_ASSERTE(0);
	return FALSE;
}

BOOL CUart::PurgeAll(void)
{
	if(m_pChannel == NULL)
		return FALSE;

	return m_pChannel->Clear();
}

BOOL CUart::PurgeInput(void)
{
	_ASSERTE(0);
	return FALSE;
}

BOOL CUart::PurgeOutput(void)
{
	_ASSERTE(0);
	return FALSE;
}

BOOL CUart::IsOpened(void) const
{
	//teana hu 2009.12.10
	return m_bOpen;
}

BOOL CUart::Read( LPVOID pBuf,  DWORD dwBytesToRead,  
           LPDWORD pReadedBytes /*= NULL*/, DWORD dwTimeOut /*= 200*/ )
{
	if(pBuf == NULL || pReadedBytes == NULL || dwBytesToRead == 0)
	{
		return FALSE;
	}
	DWORD dwSize = m_dwRcvLen - m_dwReadTotal;
	if(dwSize == 0)
	{
		ResetEvent( m_hEventWaitResult );
		DWORD dwResult = WaitForSingleObject( m_hEventWaitResult , dwTimeOut );
		UNREFERENCED_PARAMETER(dwResult);
		dwSize = m_dwRcvLen - m_dwReadTotal;
	}
	DWORD dwReadBytes = (dwSize > dwBytesToRead) ? dwBytesToRead : dwSize;

	*pReadedBytes = dwReadBytes;
	if(dwReadBytes > 0)
	{
		memcpy((char *)pBuf, m_szRcvBuf + m_dwReadTotal, dwReadBytes);
	}
	
	m_dwReadTotal += dwReadBytes;
	
	return TRUE;
}

BOOL CUart::QuickRead( LPVOID  /*pBuf*/,  
                DWORD   /*dwBytesToRead*/,  
                LPDWORD /*pReadedBytes*/  /*= NULL*/
              )
{
	_ASSERTE(0);
	return FALSE;
}

void CUart::CancelRead(void)
{
	_ASSERTE(0);
	return;
}

BOOL CUart::Write( LPVOID pBuf, DWORD dwBytesToWrite, 
            LPDWORD pWritedBytes /*= NULL */)
{
	if(!IsOpened())
	{
		return FALSE;
	}
	memset(m_szRcvBuf,0,MAX_BUF_LENGTH);
	m_dwReadTotal = 0;
	m_dwRcvLen = 0;
 	ResetEvent( m_hEventWaitResult );
	DWORD dwWrite  = m_pChannel->Write(pBuf,dwBytesToWrite);

	if(pWritedBytes != NULL)
	{
		*pWritedBytes = dwWrite;
	}
	if(dwWrite == 0)
	{
		return FALSE;
	}
	return TRUE;
}

LPVOID	CUart::WriteEx(LPCSTR lpCmd, DWORD dwTimeout, BOOL bPostAll)
{
	memset(m_szRcvBuf,0,MAX_BUF_LENGTH);

	if(lpCmd == NULL)
	{
		return m_szRcvBuf;
	}

	m_dwRcvLen = 0;
	m_bPostAll = bPostAll;

	PurgeAll();
	m_bSyn = FALSE;
	if(dwTimeout > 0)
	{
		m_bSyn = TRUE;
		ResetEvent( m_hEventWaitResult ); 
	}

    if(!Write( (LPVOID)lpCmd, strlen(lpCmd), NULL ))
	{
		m_bSyn = FALSE;
		return m_szRcvBuf;
	}  
	
	if(dwTimeout > 0)
	{
		for(int i = 0; i < MAX_RETRY_NUM; i++)
		{
			DWORD dwResult = WaitForSingleObject( m_hEventWaitResult , dwTimeout );
			if(WAIT_OBJECT_0 == dwResult)
			{
				if(strstr(m_szRcvBuf, "OK") != NULL || strstr(m_szRcvBuf, "ERROR") != NULL)
				{
					break;
				}
			}
		}
	}

	m_bSyn = FALSE;

	return m_szRcvBuf;
}

void CUart::SetReceiver( DWORD dwMsgID, DWORD dwReceiver, BOOL bIsThread /*= TRUE */)
{
	if(m_pChannel != NULL)
	{
		m_pChannel->SetReceiver(dwMsgID,bIsThread,(LPCVOID)dwReceiver);
	}

}

BOOL CUart::StartAutoReceive(HWND hWnd, LPVOID lpOpenArgs, DWORD  dwType, DWORD  dwMsgID)
{
	UNREFERENCED_PARAMETER(lpOpenArgs);
	UNREFERENCED_PARAMETER(dwType);
	
	if(m_hEventWaitResult == NULL)
	{
		m_hEventWaitResult = CreateEvent( NULL, FALSE, FALSE ,NULL );
	}

	if(m_hEventFlag == NULL)
	{
		m_hEventFlag = CreateEvent( NULL, FALSE, FALSE ,NULL );
	}
	else
	{
		ResetEvent(m_hEventFlag);
	}

    m_dwGuiMsgId = dwMsgID;

    if(!CreateScene(hWnd))
	{
        return FALSE;
    }

	//set uart message receiver
	SetReceiver( m_dwMsgId, m_dwThreadId, TRUE );

	return TRUE;
}

void CUart::StopAutoReceive(void)
{
	if(m_hEventWaitResult != NULL)
	{
		CloseHandle( m_hEventWaitResult );
		m_hEventWaitResult = NULL;
	}
	if(m_hEventFlag != NULL)
	{
		CloseHandle( m_hEventFlag );
		m_hEventFlag = NULL;
	}
    DestroyScene();
}

void CUart::FreeMem( LPBYTE pBuf )
{
	m_pChannel->FreeMem((LPVOID)pBuf);
}

LPCTSTR CUart::GetErrMsg(void)
{
	_ASSERTE(0);
	return NULL;
}

void CUart::Close(void)
{
	if(IsOpened())
	{
		m_pChannel->Close();
	}
	m_bOpen = FALSE;
}

void CUart::Release(void)
{
	if( IsOpened() )
    {
		Close();
    }

	delete this;
}
