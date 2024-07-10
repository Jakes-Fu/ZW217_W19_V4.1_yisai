// DiagObj.cpp: implementation of the CDiagObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "nveditor.h"
#include "DiagObj.h"
#include <Winsock2.h>
#include <Shlwapi.h>
#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDiagObj::CDiagObj(void)
{
	m_pChannel = NULL;
	m_bBigEndian = FALSE;
	m_bLog = FALSE;
	m_nObserverID = -1;
	m_bOpen =  FALSE;

	memset(m_LastErrorMsg,0,sizeof(m_LastErrorMsg));
	_stprintf(m_LastErrorMsg,_T("No error."));
	m_nRetries  = 3;


	m_dwNVItemSize = 0;
	m_hNVItemEvent = NULL;
	m_dwReadNVItemTimeout = DIAG_WAIT_TIME;
	m_dwWriteNVItemTimeout = DIAG_WAIT_TIME;


}

CDiagObj::~CDiagObj(void)
{
	Close();
}

int CDiagObj::OnChannelEvent( unsigned int event,void* lpEventData )
{
	if( PP_EVENT_CHANNLE_DISCON == event )
	{
		PRT_BUFF *pBuf = (PRT_BUFF *)lpEventData;
		pBuf->free_prt(pBuf);
		m_Log.LogRawStr(SPLOGLV_INFO,_T("Receive close event."));
		m_bOpen=FALSE;		
	}
    
	return 0;
}
int CDiagObj::OnChannelData(LPVOID lpData, ULONG ulDataSize )
{
	UNUSED_ALWAYS(ulDataSize);
	if( NULL == lpData )
	{
		return 0;
	}

	PRT_BUFF* lpBuff = (PRT_BUFF*)lpData;
	DIAG_PACKAGE* lpPkg = (DIAG_PACKAGE*)(lpBuff->lpData);

	if(lpPkg->header.type == DIAG_NVITEM_F)
	{
		if(lpPkg->header.subtype==0/*NVERR_NONE*/)
		{
			m_vNVItemBuf.push_back(lpBuff);
			m_dwNVItemSize += (lpPkg->header.len - sizeof(lpPkg->header));	
		}
		else
		{
			lpBuff->free_prt(lpBuff);
			m_dwNVItemSize = 0;
		}
		SetEvent(m_hNVItemEvent);
	
	}
	else
	{
		lpBuff->free_prt(lpBuff);
	}
	return 0;
}


DWORD	CDiagObj::Open(LPCTSTR lpszConfig)
{
	USES_CONVERSION;	
	_stprintf(m_LastErrorMsg,_T("No error."));

	if(lpszConfig==NULL)
	{
		_stprintf(m_LastErrorMsg,_T("CDiagObj::Open: lpszConfig is NULL."));
		return DIAG_E_INVALIDPARAM;
	}

	if(!PathFileExists(lpszConfig))
	{
		_stprintf(m_LastErrorMsg,_T("CDiagObj::Open: lpszConfig [%s] not exist."),lpszConfig);
		return DIAG_E_FAIL;
	}

	if (m_bOpen)
	{
		Close();
	}	

	m_dwReadNVItemTimeout = GetPrivateProfileInt(_T("Timeouts"),_T("ReadNVItem"),DIAG_WAIT_TIME,lpszConfig);
	m_dwWriteNVItemTimeout = GetPrivateProfileInt(_T("Timeouts"),_T("WriteNVItem"),DIAG_WAIT_TIME,lpszConfig);

	int nChannelType = GetPrivateProfileInt(_T("Settings"),_T("ChannelType"),0,lpszConfig);
	if(nChannelType != 0 && nChannelType != 1)
	{	
		m_Log.LogFmtStr(SPLOGLV_INFO,_T("channel type [%d] is invalid."),nChannelType);

		_stprintf(m_LastErrorMsg,_T("CDiagObj::Open: channel type [%d] is invalid"),nChannelType);

		return DIAG_E_INVALIDPARAM;
	}
	CHANNEL_ATTRIBUTE ca;
	_TCHAR szIP[50]={0};
	if(CHANNEL_TYPE_COM == nChannelType)
	{
		ca.ChannelType = CHANNEL_TYPE_COM;
		ca.Com.dwPortNum  = GetPrivateProfileInt(_T("SerialPort"),_T("Port"),1,lpszConfig);
		ca.Com.dwBaudRate = GetPrivateProfileInt(_T("SerialPort"),_T("Baudrate"),115200,lpszConfig);		
	}
	else
	{
		ca.ChannelType = CHANNEL_TYPE_SOCKET;		
		GetPrivateProfileString(_T("Socket"),_T("ServerIP"),_T("127.0.0.1"),szIP,50,lpszConfig);
		DWORD dwIP = inet_addr(W2A(szIP));
		if(dwIP == INADDR_NONE)
		{		
			_stprintf(m_LastErrorMsg,_T("CDiagObj::Open: ServerIP [%s] is invalid."),szIP);
			return DIAG_E_FAIL;
		}
		CONVERT_INT(inet_addr(W2A(szIP)),ca.Socket.dwIP);
		ca.Socket.dwPort = GetPrivateProfileInt(_T("Socket"),_T("ServerPort"),1,lpszConfig);
		ca.Socket.dwFlag = SC_TYPE_CLIENT;		
	}

	

	m_bLog = GetPrivateProfileInt(_T("Settings"),_T("Log"),0,lpszConfig);	
	if(m_bLog)
	{	

		_TCHAR szPath[_MAX_PATH] = {0};
		if(CHANNEL_TYPE_COM == nChannelType)
		{		
			_stprintf(szPath,_T("nveditor_com%d"),ca.Com.dwPortNum);				
		
			m_Log.Open(szPath,ISpLog::modeCreate|ISpLog::modeTimeSuffix|ISpLog::typeText,SPLOGLV_INFO);
			m_Log.LogRawStr(SPLOGLV_INFO,_T("-------------------------------"));
			m_Log.LogFmtStr(SPLOGLV_INFO,_T("Open channel COM[%d] BAUDRATE[%d]."),ca.Com.dwPortNum,ca.Com.dwBaudRate);
		}
		else
		{			
			_stprintf(szPath,_T("nveditor_port%d"),ca.Socket.dwPort);	
			m_Log.Open(szPath,ISpLog::modeCreate|ISpLog::modeTimeSuffix|ISpLog::typeText,SPLOGLV_INFO);
			m_Log.LogRawStr(SPLOGLV_INFO,_T("-------------------------------"));
			m_Log.LogFmtStr(SPLOGLV_INFO,_T("Open channel IP[%s] PORT[%d]."),szIP,ca.Socket.dwPort);
		}
		
	}

	m_hNVItemEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_hNVItemEvent == NULL)
	{
		m_Log.LogFmtStr(SPLOGLV_INFO,_T("CreateEvent m_hNVItemEvent failed."));
		return DIAG_E_FAIL;
	}

	if(m_pChannel == NULL)
	{
		if(!CreateDiagChannel(&m_pChannel))
		{
			m_Log.LogFmtStr(SPLOGLV_INFO,_T("CreateDiagChannel failed."));
			_stprintf(m_LastErrorMsg,_T("CDiagObj::Open: CreateDiagChannel failed."));
			//LeaveCriticalSection( &m_csChannel );
			return DIAG_E_CREATE_CHANNEL;
		}
	}

	m_nRetries   = GetPrivateProfileInt(_T("Settings"),_T("Retries"),3,lpszConfig);	
	m_bBigEndian = GetPrivateProfileInt(_T("Settings"),_T("BigEndian"),0,lpszConfig);	
	

	if(ca.ChannelType == CHANNEL_TYPE_SOCKET)
	{
		m_pChannel->SetProperty( 0,PPI_DIAG_Type,(void*)DIAG_TYPE_NO_HEADER);
		m_pChannel->SetProperty( 0,PPI_Endian,(void*)FALSE );
	}
	else
	{
		m_pChannel->SetProperty( 0,PPI_Endian,(void*)m_bBigEndian );
		m_pChannel->SetProperty( 0,PPI_DIAG_Type,(void*)DIAG_TYPE_NORMAL);
	}

	m_nObserverID = m_pChannel->AddObserver(this);
	if(!m_pChannel->Open(&ca))
	{
		m_pChannel->Close();
		m_Log.LogFmtStr(SPLOGLV_INFO,_T("Open channel failed."));
		int nRetries = 0;
		while(nRetries< m_nRetries)
		{
			Sleep(1000);
			if(!m_pChannel->Open(&ca))
			{
				m_pChannel->Close();
				m_Log.LogFmtStr(SPLOGLV_INFO,_T("Retry to open channel failed."));		
				nRetries++;
			}	
			else
			{
				break;
			}
		}
		if(nRetries>=m_nRetries)
		{
			_stprintf(m_LastErrorMsg,_T("CDiagObj::Open: Retry to open channel failed."));
			//LeaveCriticalSection( &m_csChannel );
			return DIAG_E_FAIL;
		}
	}	

	memcpy(&m_tChannelAttr,&ca,sizeof(CHANNEL_ATTRIBUTE));	
	m_bOpen = TRUE;

	if(CHANNEL_TYPE_SOCKET== ca.ChannelType)
	{
		if(!RegMsg(DIAG_NVITEM_F))
		{
			m_Log.LogFmtStr(SPLOGLV_INFO,_T("RegMsg DIAG_NVITEM_F failed."));	
		}

		PRT_WRITE_T pwt;
		pwt.action = PRT_WRITE_no_respond;
		pwt.nCond = -1;
		DIAG_PACKAGE dp = {0};
		dp.header.len = 0;
		dp.header.type = 209;
		dp.header.subtype = 0;
		dp.data = NULL;
		pwt.lpProtocolData = &dp;
		m_pChannel->Write( &pwt,1 );
	}	

	m_Log.LogFmtStr(SPLOGLV_INFO,_T("Open channel success."));


	return DIAG_SUCCESS;
}

void	CDiagObj::Close()
{
	_stprintf(m_LastErrorMsg,_T("No error."));

	m_Log.LogFmtStr(SPLOGLV_INFO,_T("Close channel."));

	if(m_hNVItemEvent)
	{
		SetEvent(m_hNVItemEvent);
	}
	//EnterCriticalSection( &m_csChannel );
	if(m_pChannel != NULL)
	{
		if(CHANNEL_TYPE_SOCKET== m_tChannelAttr.ChannelType)
		{
			UnregMsg(DIAG_NVITEM_F);
		}
		m_bOpen = FALSE;
		m_pChannel->Close();
		if(m_nObserverID != -1)
		{
			m_pChannel->RemoveObserver(m_nObserverID);
			m_nObserverID = -1;
		}
		//ReleaseDiagChannel(m_pChannel);
		m_pChannel = NULL;
	}
	m_bOpen = FALSE;
	//LeaveCriticalSection(&m_csChannel );

	ClearNVItemBuf();

	if(m_hNVItemEvent)
	{
		CloseHandle(m_hNVItemEvent);
		m_hNVItemEvent = NULL;
	}
	
	m_Log.Close();
}
void	CDiagObj::DestroyData(PRT_BUFF* lpData )
{
	if(lpData)
	{
		lpData->free_prt((LPVOID)lpData);
	}
}

BOOL	CDiagObj::RegMsg( unsigned char chType )
{
	_stprintf(m_LastErrorMsg,_T("No error."));

	if (m_pChannel == NULL || !m_bOpen)
	{
		_stprintf(m_LastErrorMsg,_T("RegMsg: not connected."));
		return FALSE;
	}

	if( m_tChannelAttr.ChannelType == CHANNEL_TYPE_COM )
		return FALSE;
	
	DWORD  dwRet = 0;
	
	PRT_WRITE_T pwt;
	pwt.action = PRT_WRITE_no_respond;
	pwt.nCond = -1;
	DIAG_PACKAGE dp = {0};
	dp.header.len = 0;
	dp.header.type = MSG_REG;
	dp.header.subtype = chType;
	dp.data = NULL;
	pwt.lpProtocolData = &dp;

	dwRet = m_pChannel->Write( &pwt,1 );
	if( dwRet == 0 )
	{
		m_Log.LogRawStr(SPLOGLV_INFO,_T("RegMsg: Write channel failed."));
		int nRetries = 0;
		while(nRetries< m_nRetries)
		{
			Sleep(10);
			dwRet = m_pChannel->Write( &pwt,1 );
			if( dwRet == 0)
			{		
				m_Log.LogRawStr(SPLOGLV_INFO,_T("RegMsg: Retry to write channel failed."));		
				nRetries++;
			}	
			else
			{
				m_Log.LogRawStr(SPLOGLV_INFO,_T("RegMsg: Retry to write channel success."));	
				break;
			}
		}	

		if(nRetries>=m_nRetries)
		{
			_stprintf(m_LastErrorMsg,_T("CDiagObj::RegMsg: Retry to write channel failed."));
			// must call Close to release COM resource,
			// otherwise it will effect open it again in another instance.
			//if(m_bFailToClose)
			//{
			//	Close();
			//}
			
			return FALSE;
		}
	}

	return TRUE;
}

BOOL	CDiagObj::UnregMsg( unsigned char chType )
{	
	_stprintf(m_LastErrorMsg,_T("No error."));

	if (m_pChannel == NULL || !m_bOpen)
	{
		_stprintf(m_LastErrorMsg,_T("UnregMsg: not connected."));
		return FALSE;
	}
	
	if( m_tChannelAttr.ChannelType == CHANNEL_TYPE_COM )
		return FALSE;
	
	DWORD  dwRet = 0;	
	PRT_WRITE_T pwt;
	pwt.action = PRT_WRITE_no_respond;
	pwt.nCond = -1;
	DIAG_PACKAGE dp = {0};
	dp.header.len = 0;
	dp.header.type = MSG_UNREG;
	dp.header.subtype = chType;
	dp.data = NULL;
	pwt.lpProtocolData = &dp;

	dwRet = m_pChannel->Write( &pwt,1 );
	if( dwRet == 0 )
	{
		m_Log.LogRawStr(SPLOGLV_INFO,_T("UnregMsg: Write channel failed."));
		int nRetries = 0;
		while(nRetries< m_nRetries)
		{
			Sleep(10);
			dwRet = m_pChannel->Write( &pwt,1 );
			if( dwRet == 0)
			{		
				m_Log.LogRawStr(SPLOGLV_INFO,_T("UnregMsg: Retry to write channel failed."));		
				nRetries++;
			}	
			else
			{
				m_Log.LogRawStr(SPLOGLV_INFO,_T("UnregMsg: Retry to write channel success."));	
				break;
			}
		}
		//LeaveCriticalSection(&m_csChannel );

		if(nRetries>=m_nRetries)
		{
			_stprintf(m_LastErrorMsg,_T("CDiagObj::UnregMsg: Retry to write channel failed."));
			// must call Close to release COM resource,
			// otherwise it will effect open it again in another instance.
			//if(m_bFailToClose)
			//{
			//	Close();
			//}
			
			return FALSE;
		}
	}

	return TRUE;
}



DWORD  CDiagObj::ReadNVItem(WORD nNVItemID,BYTE **lpRcvData,DWORD &dwSize)
{
	*lpRcvData = NULL;
	dwSize = 0;

	_stprintf(m_LastErrorMsg,_T("No error."));
	if (m_pChannel == NULL || !m_bOpen)
	{
		m_Log.LogRawStr(SPLOGLV_INFO,_T("ReadNVItem: not connnected."));
		_stprintf(m_LastErrorMsg,_T("ReadNVItem: not connected."));
		return DIAG_E_NOT_CONNECTED;
	}
	
	DWORD  dwRet = 0;

	ClearNVItemBuf();

	ResetEvent(m_hNVItemEvent);


	WORD nID = nNVItemID;
	if(m_bBigEndian)
	{
		CONVERT_SHORT(nID,nID);
	}
	
	PRT_WRITE_T pwt = {0};
	pwt.action = PRT_WRITE_no_respond;
	pwt.nCond  = 0;
	DIAG_PACKAGE dp = {0};
	dp.header.type    = (BYTE)DIAG_NVITEM_F;
	dp.header.subtype = (BYTE)NVITEM_READ;
	dp.data = &nID;
	dp.header.len = sizeof(WORD);
	pwt.lpProtocolData = &dp;

	m_Log.LogFmtStr(SPLOGLV_INFO,_T("ReadNVItem: ID, %d [0x%x]."),nNVItemID,nNVItemID);


	dwRet = m_pChannel->Write( &pwt,1 );
	if( dwRet == 0 )
	{
		m_Log.LogRawStr(SPLOGLV_INFO,_T("CDiagObj::ReadNVItem: write channel failed."));
		int nRetries = 0;
		while(nRetries < m_nRetries)
		{
			Sleep(10);
			dwRet = m_pChannel->Write( &pwt,1 );
			if( dwRet == 0)
			{		
				m_Log.LogRawStr(SPLOGLV_INFO,_T("CDiagObj::ReadNVItem: Retry to write channel failed."));		
				nRetries++;
			}	
			else
			{
				m_Log.LogRawStr(SPLOGLV_INFO,_T("CDiagObj::GrabImage: Retry to write channel success."));		
				break;
			}
		}

		if(nRetries>=m_nRetries)
		{
			_stprintf(m_LastErrorMsg,_T("CDiagObj::ReadNVItem: Retry to write channel failed."));
		
			return DIAG_E_CHANNEL_ERROR;
		}
	}


	dwRet = WaitForSingleObject(m_hNVItemEvent,m_dwReadNVItemTimeout);	
	if(dwRet == WAIT_OBJECT_0 ) //WAIT_TIMEOUT
	{
		if(m_dwNVItemSize !=0)
		{
			LPBYTE pImage = new BYTE[m_dwNVItemSize+2];
			memset(pImage,0,m_dwNVItemSize+2);		
			int nItemSize = 0;
			if(m_vNVItemBuf.size()>0)
			{					
				PRT_BUFF * lpBuff = m_vNVItemBuf[0];
				DIAG_PACKAGE *pdp = (DIAG_PACKAGE*)(lpBuff->lpData);				
				WORD wLen = (WORD)(pdp->header.len-(WORD)sizeof(pdp->header));
				memcpy(pImage,&wLen,sizeof(WORD));
				memcpy(pImage+2,&pdp->data,wLen);
				nItemSize = wLen+2;	
				*lpRcvData = pImage;
				dwSize = nItemSize;
			}	
			else
			{
				delete [] pImage;
				return DIAG_E_FAIL;
			}			

			return DIAG_SUCCESS;
		}
		else
		{
			m_Log.LogRawStr(SPLOGLV_INFO,_T("ReadNVItem: size is zero."));
			_stprintf(m_LastErrorMsg,_T("ReadNVItem: size is zero."));
			//return DIAG_E_FAIL;
		}
	}
	else
	{
		ClearNVItemBuf();
		m_Log.LogRawStr(SPLOGLV_INFO,_T("ReadNVItem timeout!"));
		_stprintf(m_LastErrorMsg,_T("ReadNVItem timeout!"));
	}

	return DIAG_E_FAIL;
}

DWORD  CDiagObj::WriteNVItem(WORD nNVItemID,BYTE *lpSendData,DWORD dwSize)
{
	_stprintf(m_LastErrorMsg,_T("No error."));
	if (m_pChannel == NULL || !m_bOpen)
	{
		m_Log.LogRawStr(SPLOGLV_INFO,_T("WriteNVItem: not connnected."));
		_stprintf(m_LastErrorMsg,_T("WriteNVItem: not connected."));
		return DIAG_E_NOT_CONNECTED;
	}
	
	DWORD  dwRet = 0;

	ClearNVItemBuf();

	ResetEvent(m_hNVItemEvent);


	WORD nID = nNVItemID;
	if(m_bBigEndian)
	{
		CONVERT_SHORT(nID,nID);
	}
	
	PRT_WRITE_T pwt = {0};
	pwt.action = PRT_WRITE_no_respond;
	pwt.nCond  = 0;
	DIAG_PACKAGE dp = {0};
	dp.header.type    = (BYTE)DIAG_NVITEM_F;
	dp.header.subtype = (BYTE)NVITEM_WRITE;
	dp.data = lpSendData;
	dp.header.len = (WORD)dwSize;
	pwt.lpProtocolData = &dp;


	dwRet = m_pChannel->Write( &pwt,1 );
	if( dwRet == 0 )
	{
		m_Log.LogRawStr(SPLOGLV_INFO,_T("CDiagObj::WriteNVItem: write channel failed."));
		int nRetries = 0;
		while(nRetries < m_nRetries)
		{
			Sleep(10);
			dwRet = m_pChannel->Write( &pwt,1 );
			if( dwRet == 0)
			{		
				m_Log.LogRawStr(SPLOGLV_INFO,_T("CDiagObj::WriteNVItem: Retry to write channel failed."));		
				nRetries++;
			}	
			else
			{
				m_Log.LogRawStr(SPLOGLV_INFO,_T("CDiagObj::WriteNVItem: Retry to write channel success."));		
				break;
			}
		}

		if(nRetries>=m_nRetries)
		{
			_stprintf(m_LastErrorMsg,_T("CDiagObj::WriteNVItem: Retry to write channel failed."));
		
			return DIAG_E_CHANNEL_ERROR;
		}
	}

//	return DIAG_SUCCESS;

	dwRet = WaitForSingleObject(m_hNVItemEvent,m_dwWriteNVItemTimeout);	
	if(dwRet == WAIT_OBJECT_0)
	{
		return DIAG_SUCCESS;
	}
	else
	{
		ClearNVItemBuf();
		m_Log.LogRawStr(SPLOGLV_INFO,_T("WriteNVItem timeout!"));
		_stprintf(m_LastErrorMsg,_T("WriteNVItem timeout!"));
	}

	return DIAG_E_FAIL;

}

void CDiagObj::ClearNVItemBuf()
{
	UINT nSize = m_vNVItemBuf.size();
	for(UINT i = 0; i< nSize ; i++)
	{
		PRT_BUFF* lpBuff = m_vNVItemBuf[i];
		lpBuff->free_prt((void*)lpBuff);
	}
	m_vNVItemBuf.clear();
	m_dwNVItemSize = 0;
}


void CDiagObj::Log(LPCTSTR pszLog)
{
	if(pszLog != NULL)
	{
		m_Log.LogRawStr(SPLOGLV_INFO,pszLog);	
	}
}
BOOL CDiagObj::IsConnected()
{
	if(m_pChannel != NULL && m_bOpen)
	{
		return TRUE;
	}
	else
	{
		Close();
		return FALSE;
	}
}
