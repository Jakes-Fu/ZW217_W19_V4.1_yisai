// ControlCenter.cpp: implementation of the CControlCenter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ControlCenter.h"
#include <winsock2.h>
#include <ATLCONV.H>
#include "Resource.h"
#include "MainFrm.h"


#define MSG_TYPE_SYSTEM		5
#define MSG_TYPE_REG		99
#define MSG_TYPE_UNREG		100
#define MSG_SUBTYPE_ASSERT  4
#define MSG_ANONY_START     0     //150
#define MSG_TOOL_REQ		209
#define MSG_TYPE_ASSERT		255
#define	MSG_TIME_OUT		211

// Tool intercomunication relative definitions
#define ITC_REQ_TYPE	    209
#define ITC_REQ_SUBTYPE     100
#define ITC_REP_SUBTYPE     101

#define ITC_TOOL_NAME_LEN 32
typedef struct tag_ITC_Req_T
{
	DWORD dwReqType;	// Request type
	char szToolName[ITC_TOOL_NAME_LEN];	// target tool name
}ITC_Req_T;

typedef struct tag_ITC_Rep_T
{
	DWORD dwReqType;	// Request type
	DWORD dwRetCode;	// Error code
}ITC_Rep_T;

enum ITC_Req_Type_E
{
	Reg_Tool,
	Unreg_Tool,
	Query_Tool,
	Logel_ClearLog,
	Logel_SaveLog,
	Logel_Rec_keynote,
	Script_Resume,
	Query_Endian,
    Reset_Channelserver,
    Reg_Tool_Care_Type,     // Logel工具向ChannelServer注册工具关心的数据类型
};

enum Care_Type
{
    eUnkown                 = -1,
    eEVENT_DEVICE_REMOVED,
    eEVENT_DEVICE_PLUGIN
};


#define SIZE_OF_WSHOSTINFO 52
//
// typedef struct WSHOSTINFO
// {
//    char   HostName[32];
//    char   HostAddr[16];
//    int    HostID;
// }WSHOSTINFO,*PWSHOSTINFO;
//

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlCenter::CControlCenter()
{
	m_nSequenceNumber = 1;
	m_bAssert = FALSE;
	m_hWnd = NULL;
	m_pWnd = NULL;
	m_nTimeout = 3;
	memset( m_RegedMsg,0,2*ANONY_MSG_COUNT*MAX_REG_TOOL );
    memset( m_clientData,0, sizeof(WORD) * ANONY_MSG_COUNT * MAX_REG_TOOL);

	//m_pConvertor = NULL;
	//m_nOldToolCount = 0;
	m_bLog = FALSE;
	m_nEndian = ENDIAN_LITTLE;
	m_nChannelType = 0;
	m_dwDownData = 0;
	m_dwUpData = 0;
	m_nTypeDefCount = 0;

	LoadTypeDef();
	InitializeCriticalSection( &m_csClient );
//	InitializeCriticalSection( &m_csMessage );
	//InitializeCriticalSection( &m_csConvert );
/*  // [[ for self debug
	m_pFileSmp = NULL;
	m_pFileOrg = NULL;
	CreateProtoPackage(&m_pSMPPacket,PT_SMP);
	// ]]
*/
}

CControlCenter::~CControlCenter()
{
	DeleteCriticalSection( &m_csClient );
//	DeleteCriticalSection( &m_csMessage );
	//DeleteCriticalSection( &m_csConvert );
	StopLog();//lint !e1551	
/*  // [[ for self debug
	ReleaseProtoPackage(m_pSMPPacket);
	// ]]
*/
}

int CControlCenter::OnHiLvlData( LPVOID lpData, ULONG ulDataSize )
{
	if( 0 == ulDataSize )
	{
		if( NULL == lpData )
		{
			return 0;
		}
		else
		{
			// Socket event
			DWORD* lpEventData = (DWORD*)lpData;
			return OnHiLvlEvent( *(lpEventData+1),(void*)*lpEventData );			
		}
	}

	DWORD dwClientId = *(DWORD*)((BYTE*)lpData + ulDataSize);
	
	EnterCriticalSection( &m_csClient );
	Client_Info* lpci = NULL;
	if( m_Clients.Lookup( (WORD)dwClientId,lpci ) )
	{
		int nPkg = 0;
		BOOL bModify = FALSE;
		if( lpci->bFirstReceive )
		{
			lpci->bFirstReceive = FALSE;
			// Compatible with commsock
			if( ulDataSize >= SIZE_OF_WSHOSTINFO )  
			{
				if( IsCommSockData( (BYTE*)lpData ) )
				{
					// Host info sent by CommSock,ignore it
					if( ulDataSize == SIZE_OF_WSHOSTINFO )
					{
						m_lpHighChan->FreeMem( lpData );
						LeaveCriticalSection( &m_csClient );
						return 0;
					}
					else
					{
						bModify = TRUE;
					}
				}
			}
		}
		
		if( bModify )
		{
			nPkg = lpci->lpPkg->Append( (BYTE*)lpData+SIZE_OF_WSHOSTINFO,ulDataSize-SIZE_OF_WSHOSTINFO );
		}
		else
		{
			nPkg = lpci->lpPkg->Append( lpData,ulDataSize );
		}

		if( nPkg > 0 )
		{
			PRT_BUFF** lppBuff = new PRT_BUFF*[nPkg]; //lint !e737
			lpci->lpPkg->GetPackages( lppBuff,nPkg );
			LeaveCriticalSection( &m_csClient );
			for( int i=0;i<nPkg;i++ )
			{
				ProcessData( dwClientId,lppBuff[i]);
			}
			delete[] lppBuff;
		}
		else
		{
			LeaveCriticalSection( &m_csClient );
		}
	}
	else
	{
		LeaveCriticalSection( &m_csClient );
	}

	m_lpHighChan->FreeMem( lpData );

	return 0;
}

int CControlCenter::OnHiLvlEvent( unsigned int event,void* lpEventData )
{
	if( SC_CLIENT_CONNECT == event )
	{
		// A client connect to it
		AddClient( (DWORD)lpEventData );
	}
	else if( SC_CLIENT_CLOSED == event )
	{
		// A client has disconnect
		RemoveClient( (DWORD)lpEventData );
	}

	return 0;
}

int CControlCenter::OnChannelData( LPVOID lpData, ULONG /*ulDataSize*/ )
{
	if( NULL == lpData )
	{
		return 0;
	}

	EnterCriticalSection( &m_csClient );

	CString strLog;
	CMainFrame *pMF = (CMainFrame *) m_pWnd;
	strLog.Format(_T("CControlCenter::OnChannelData +++"));
	pMF->Log(strLog);

	PRT_BUFF* lpBuff = (PRT_BUFF*)lpData;
	DIAG_PACKAGE* lpPkg = (DIAG_PACKAGE*)lpBuff->lpData;
	m_dwUpData += (DWORD)lpBuff->size;	//lint !e737

/*  // [[ for self debug
	if(m_pFileSmp && m_pFileOrg)
	{
		int nSize = 0;
		void * pData = NULL;
		SMP_PACKAGE smp = {0};
		smp.data = lpPkg;
		smp.header.len = (unsigned short)(lpPkg->header.len+SMP_HDR_LEN);
		if(m_pSMPPacket->Package(&smp,sizeof(smp),&pData,&nSize) == 0 )
		{
			fwrite(pData,nSize,1,m_pFileSmp);
			m_pSMPPacket->FreeMem(pData);
			fwrite(lpPkg,lpPkg->header.len,1,m_pFileOrg);
		}
	}
	// ]]
*/
//	static BYTE buff[65536];
//	PRT_BUFF pb;
//	pb.reserved = 0;
//	pb.lpData = buff;	

// 	if( lpPkg->header.type < MSG_ANONY_START || lpPkg->header.type == 0xF9 )
// 	{
// 		m_bGotOneRightPacket = TRUE;
// 		if( m_bLog )
// 		{
// 			m_RequestLog.LogRawStrA( SPLOGLV_INFO,"Received from channel" );
// 			m_RequestLog.LogBufData( SPLOGLV_INFO,lpBuff->lpData,lpBuff->size,LOG_READ );
// 		}
// 	
// 		// This is a response for previous request
// 		EnterCriticalSection( &m_csMessage );
// 		PRT_BUFF* lpReq;
// 		if( m_Messages.Lookup( lpPkg->header.sn,lpReq ) )
// 		{
// 			LeaveCriticalSection( &m_csMessage );
// 			DIAG_HEADER* lpReqHead = (DIAG_HEADER*)lpReq->lpData;
// 
// 			// Save the original sn back
// 			//DWORD dwSn = lpPkg->header.sn;
// 			lpPkg->header.sn = lpReqHead->sn;
// 			EnterCriticalSection( &m_csClient );
// 	
// 			{
// 				LeaveCriticalSection( &m_csClient );
// 				SendUp(  lpReqHead->len,lpBuff,TRUE );
// 			}
// 			/*
// 			if( IsLastPackage( &lpPkg->header ) )
// 			{
// 				EnterCriticalSection( &m_csMessage );
// 				m_Messages.RemoveKey( dwSn );
// 				LeaveCriticalSection( &m_csMessage );
// 				lpReq->free_prt( lpReq );
// 			}
// 			else
// 			{
// 				// Restore timeout
// 				lpReq->reserved = m_nTimeout;
// 			}
// 			*/
// 		}
// 		else
// 		{
// 			LeaveCriticalSection( &m_csMessage );
// 		}
// 	}
// 	else
	{
		// Anonymous and assert message
		if( m_bLog )
		{
			m_ChannelLog.LogHexData( lpBuff->lpData,lpBuff->size );
		}
		BOOL bSend = FALSE;
		if( lpPkg->header.type == MSG_TYPE_ASSERT )
		{
			if( !m_bAssert )
			{
				m_bAssert = TRUE;
				//PostMessage( m_hWnd,WM_NOTIFY_ASSERT,m_bAssert,0 );
				if(m_bGotOneRightPacket && pMF != NULL && pMF->IsNeedResetPort())
				{
					strLog.Format(_T("CControlCenter::OnChannelData: assert and need reset"));
					pMF->Log(strLog);

					PostMessage( m_hWnd,WM_PORT_RESET,0,0 );
					lpBuff->free_prt( lpBuff );	

					strLog.Format(_T("CControlCenter::OnChannelData ---"));
					pMF->Log(strLog);
					
					LeaveCriticalSection( &m_csClient );
					return 0;
				}
				else
				{
					strLog.Format(_T("CControlCenter::OnChannelData: assert1"));
					pMF->Log(strLog);

					PostMessage( m_hWnd,WM_NOTIFY_ASSERT,m_bAssert,0 );
				}
			}
		}
		else
		{
			m_bGotOneRightPacket = TRUE;
			if( m_bAssert )
			{
				strLog.Format(_T("CControlCenter::OnChannelData: assert2"));
				pMF->Log(strLog);

				m_bAssert = FALSE;
				PostMessage( m_hWnd,WM_NOTIFY_ASSERT,m_bAssert,0 );
			}
		}

		BYTE cType = (BYTE)(lpPkg->header.type - MSG_ANONY_START);
		if( m_RegedMsg[cType][0] > 0 )
		{	
			for( int i=0;i<m_RegedMsg[cType][0];i++ )
			{
				Client_Info* lpci;
				if( m_Clients.Lookup( m_RegedMsg[cType][i+1],lpci ) )
				{					
					// This tool is new,use original buffer
					bSend |= SendUp( m_RegedMsg[cType][i+1],lpBuff ); //lint !e737					
				}
			}			
		}
		if( m_bLog && bSend )
		{
			// The package maybe send to more than one client,
			// We do not log the package only if it is failed to 
			// be send to all the clients
			m_ToolLog.LogHexData( lpBuff->lpData,lpBuff->size,LOG_WRITE );
		}
	}

	lpBuff->free_prt( lpBuff );

	strLog.Format(_T("CControlCenter::OnChannelData ---"));
	pMF->Log(strLog);

	LeaveCriticalSection( &m_csClient );
	return 0;
}

int CControlCenter::OnChannelEvent( unsigned int event,void* lpEventData )
{
	EnterCriticalSection( &m_csClient );
	CMainFrame * pMF = (CMainFrame *)m_pWnd; 
	CString strLog;
	strLog.Format(_T("CControlCenter::OnChannelEvent: %d"), event);
	pMF->Log(strLog);
	
	if( PP_EVENT_CHANNLE_DISCON == event )
	{
		// Send to UI		
		strLog.Format(_T("CControlCenter::OnChannelEvent: post disconnect msg"));
		pMF->Log(strLog);
		PostMessage( m_pWnd->m_hWnd,WM_COMMAND,ID_DISCONNECT_MSSIM,0 );	
	}
/*
	else if(PP_EVENT_FILE_READ_OVER == event)
	{
		/// hongliang: 此处不能使用PP_EVENT_FILE_READ_OVER，因为收到这个事件是，底层的Channel
		/// 虽然已经读到文件结尾，但不意味着DiagChan已经将数据传输完毕，如果此时强制断开连接
		/// 会存在文件结尾数据丢失的风险.
		/// 此处只是为了UI回放多个文件时，通知UI回放下一个文件。
		if(pWnd) 
		{
			PostMessage( pWnd->m_hWnd,WM_FILE_READ_OVER,0,0 ); 
		}

	}
*/
/*	else if( PP_EVENT_BIG_ENDIAN == event )
	{
		m_nEndian = ENDIAN_BIG;
		// Tell UI
		int nRetry = 3;
		while( nRetry )
		{
			nRetry -= 1;
			if( NULL == m_pWnd)
			{
				Sleep( 100 );	
			}
			else
			{
				PostMessage( m_pWnd->m_hWnd,WM_EVENT_ENDIAN,m_nEndian,0 );
				break;
			}			
		}
		// Tell all clients the endian is decided
		TellClientsEndian();
	}
	else if( PP_EVENT_LITTLE_ENDIAN == event )
	{
		m_nEndian = 0;
		// Tell UI
		int nRetry = 3;
		while( nRetry )
		{
			nRetry -= 1;
			if( NULL == m_pWnd )
			{
				Sleep( 100 );	
			}
			else
			{
				PostMessage( m_pWnd->m_hWnd,WM_EVENT_ENDIAN,m_nEndian,0 );
				break;
			}			
		}
		// Tell all clients the endian is decided
		TellClientsEndian();
	}
*/
	if( NULL != lpEventData )
	{
		((PRT_BUFF*)lpEventData)->free_prt( lpEventData );
	}

	LeaveCriticalSection( &m_csClient );
	return 0;
}

void CControlCenter::ProcessData( DWORD dwClientId,PRT_BUFF* lpBuff)
{
	if( m_bLog )
	{
		m_RequestLog.LogRawStrA( SPLOGLV_INFO,"Received from client" );
		m_RequestLog.LogBufData( SPLOGLV_INFO,lpBuff->lpData,lpBuff->size,LOG_READ );
	}

	DIAG_HEADER* lpPkg = (DIAG_HEADER*)(lpBuff->lpData);

	if( MSG_TYPE_REG == lpPkg->type )
	{
		BOOL bRet = Reg( dwClientId,lpPkg->subtype );
		SendUp( (WORD)dwClientId,lpPkg->sn,(BYTE)MSG_TYPE_REG,(BYTE)bRet,TRUE );
		lpBuff->free_prt( lpBuff );
	}
	else if( MSG_TYPE_UNREG == lpPkg->type )
	{
		Unreg( dwClientId,lpPkg->subtype );
		SendUp( (WORD)dwClientId,lpPkg->sn,(BYTE)MSG_TYPE_UNREG,(BYTE)0,TRUE );
		lpBuff->free_prt( lpBuff );
	}
	else if( MSG_TOOL_REQ == lpPkg->type )
	{
		OnItcReq( dwClientId,lpBuff );
	}
	else
	{
		Reg( dwClientId, lpPkg->type ); //为了兼容，先注册type
		ProcessRequest( dwClientId,lpBuff );
	}
}

BOOL CControlCenter::Reg( DWORD dwClientId,BYTE cType )
{
	if( cType < MSG_ANONY_START )
	{
		// this message do not need reg
		return FALSE;
	}

	cType -= MSG_ANONY_START;

	EnterCriticalSection( &m_csClient );
	WORD nCount = m_RegedMsg[cType][0];
	if( nCount == MAX_REG_TOOL - 1 )
	{
		// There is too many tool register this message
		LeaveCriticalSection( &m_csClient );
		return FALSE;
	}
	
	BOOL bReg = FALSE;
	
	for( int i=1;i<nCount+1;i++ )
	{
		if( m_RegedMsg[cType][i] == dwClientId )
		{
			// This client has registered the message
			bReg = TRUE;
			break;
		}
	}
	if( !bReg )
	{
		nCount++;
		m_RegedMsg[cType][0] = nCount;
		m_RegedMsg[cType][nCount] = (WORD)dwClientId;
	}
	LeaveCriticalSection( &m_csClient );

	return TRUE;
}

void CControlCenter::Unreg( DWORD dwClientId,BYTE cType ,BOOL bClear/*=FALSE*/)
{
/*
	if( cType == 0 )
	{
		// Remove all registered messages for this client
		for( int i=0;i<ANONY_MSG_COUNT;i++ )
		{
			Unreg( dwClientId, (BYTE)(i + MSG_ANONY_START) );
		}
	}
 	else if( cType < MSG_ANONY_START )
 	{
 		// This message need not unreg
 		return;
	}
*/
	if(bClear)
	{
		for( int i=0;i<ANONY_MSG_COUNT;i++ )
		{
			Unreg( dwClientId, (BYTE)(i + MSG_ANONY_START) );
		}
	}
	else
	{
		cType -= MSG_ANONY_START;
		EnterCriticalSection( & m_csClient );
		WORD nCount = m_RegedMsg[cType][0];
		int i=0;
		for( i=1;i<nCount+1;i++ )
		{
			if( m_RegedMsg[cType][i] == (WORD)dwClientId )
			{
				break;
			}
		}
		
		if( i == nCount )
		{
			// The client is the last one to register the type,just decrease the count
			m_RegedMsg[cType][0] = --nCount;
		}
		else if( i < nCount )
		{
			memcpy( &m_RegedMsg[cType][i],&m_RegedMsg[cType][i+1],( nCount - i ) * 2 );
			m_RegedMsg[cType][0] = --nCount;
		}
		LeaveCriticalSection( &m_csClient );
	}
}

void CControlCenter::ProcessRequest( DWORD dwClientId,PRT_BUFF* lpBuff )
{
	UNUSED_ALWAYS(dwClientId);
	if( lpBuff )
	{
		DIAG_HEADER* lpPkg = (DIAG_HEADER*)(lpBuff->lpData);
		// Because maybe there are more than one tool connect to 
		// channelserver at the same time,we use a internal sn to 
		// replace the sn passed by tool,then store them back later
		//unsigned int sn = lpPkg->sn;
		lpPkg->sn = m_nSequenceNumber++;

		if( SendDown( lpBuff ) )
		{
			//简化处理流程，客户端发过来的命令，工具默认先注册
			/*
			// Add to request list and wait for response
			// Assert response is processed as asynchronous message,so 
			// we do not add assert request to the list			
			if( MSG_TYPE_ASSERT != lpPkg->type || 
				( MSG_TYPE_SYSTEM != lpPkg->type && MSG_SUBTYPE_ASSERT == lpPkg->subtype )
				)
			{
				lpPkg->len = (WORD)dwClientId;	// Use length field to store client id
				lpBuff->reserved = m_nTimeout;	// Use reserved value to store timeout
				unsigned int key = lpPkg->sn;   // Use the internal sn as a key
				lpPkg->sn = sn;					// Save original sn back
				InterlockedIncrement( (long*)&lpBuff->ref_count );
				EnterCriticalSection( &m_csMessage );
				m_Messages.SetAt( key,lpBuff );
				LeaveCriticalSection( &m_csMessage );
			}
			*/
			
		}
		
		lpBuff->free_prt( lpBuff );
	}
	else
	{
		//简化处理流程，客户端发过来的命令，工具默认先注册
		/*
		// Remove all pending request sent by this client
		EnterCriticalSection( &m_csMessage );
		POSITION pos = m_Messages.GetStartPosition();
		DWORD dwKey;
		PRT_BUFF* lpBuff1;
		while( pos )
		{
			m_Messages.GetNextAssoc( pos,dwKey,lpBuff1 );
			DIAG_HEADER* lpPkg = (DIAG_HEADER*)lpBuff1->lpData;
			if( dwClientId == lpPkg->len )
			{
				m_Messages.RemoveKey( dwKey );
				lpBuff1->free_prt( lpBuff1 );
			}
		}
		LeaveCriticalSection( &m_csMessage );
		*/
	}
}

void CControlCenter::AddClient( DWORD /*dwClientId*/ )
{
	RefreshClientInfo();
}

void CControlCenter::RemoveClient( DWORD /*dwClientId*/ )
{
	RefreshClientInfo();
}

DWORD CControlCenter::SendDown( PRT_BUFF* lpBuff )
{
	if( NULL == m_lpLowChan || NULL == lpBuff )
	{
		return 0;
	}

	DWORD dwRet = 0;
	PRT_WRITE_T pwt;
	pwt.action = PRT_WRITE_no_respond;
	DIAG_PACKAGE dp;
	memcpy( &dp,lpBuff->lpData,sizeof( DIAG_HEADER ) );
	dp.header.len -= sizeof( DIAG_HEADER );
	dp.data = lpBuff->lpData + sizeof( DIAG_HEADER );
	pwt.lpProtocolData = &dp;
	dwRet = m_lpLowChan->Write( &pwt,1 );
	m_dwDownData += dwRet;
	
	if( m_bLog )
	{
		if( dwRet )
		{
			m_RequestLog.LogRawStrA( SPLOGLV_INFO,"Send to ms" );
		}
		else
		{
			m_RequestLog.LogRawStrA( SPLOGLV_INFO,"Send to ms failed" );
		}
		m_RequestLog.LogBufData( SPLOGLV_INFO,lpBuff->lpData,lpBuff->size );
	}
	return dwRet;
}

DWORD CControlCenter::SendUp( WORD wClientId,PRT_BUFF* lpBuff,BOOL bResponse )
{
	if( NULL == m_lpHighChan || lpBuff == NULL )
	{
		return 0;
	}

	DWORD dwRet = SendUp( wClientId,lpBuff->lpData,lpBuff->size );

	// Anonymous message will be logged by caller,we only log response message here
	if( m_bLog && bResponse )
	{
		if( dwRet == (DWORD)(lpBuff->size) ) //lint !e737
		{
			m_RequestLog.LogRawStrA( SPLOGLV_INFO,"Send to client" );
		}
		else
		{
			m_RequestLog.LogFmtStrA( SPLOGLV_ERROR,"Send to client failed,error code is %d",GetLastError() );
		}
		m_RequestLog.LogBufData( SPLOGLV_INFO,lpBuff->lpData,lpBuff->size );
	}
	return dwRet;
}

DWORD CControlCenter::SendUp( WORD wClientId,DWORD sn,BYTE type,BYTE subtype,BOOL bResponse )
{
	DWORD dwRet = 0;
	DWORD dwSize = sizeof( DIAG_HEADER);
	LPBYTE pBuf = new BYTE[dwSize];
	if(pBuf == NULL)
		return 0;

	memset(pBuf,0,dwSize);
	DIAG_HEADER *pdp = (DIAG_HEADER *)pBuf;
	pdp->len = (WORD)dwSize;

	if( NULL != m_lpHighChan)
	{
		pdp->sn = sn;
		pdp->type = type;
		pdp->subtype = subtype;
		
		dwRet = SendUp( wClientId,(BYTE*)pdp,pdp->len );
	}
	
	// Anonymous message will be logged by caller,we only log response message here
	if( m_bLog && bResponse )
	{
		if( dwRet == pdp->len )
		{
			m_RequestLog.LogRawStrA( SPLOGLV_INFO,"Send to client" );
		}
		else
		{
			m_RequestLog.LogFmtStrA( SPLOGLV_ERROR,"Send to client failed,error code is %d",GetLastError() );
		}
		m_RequestLog.LogBufData( SPLOGLV_INFO,(BYTE*)pdp,pdp->len );
	}

	delete [] pBuf;

	return dwRet;
}

DWORD CControlCenter::SendUp( WORD wClientId,BYTE* lpBuff,DWORD dwSize )
{
	if( NULL == m_lpHighChan || NULL == lpBuff )
	{
		return 0;
	}

	DWORD dwRet = 0;
	DWORD dwRetry = 5;
	DWORD dwRemain = dwSize;
	BYTE* lpEnd = lpBuff+dwSize;

	while( dwRetry > 0 && lpBuff < lpEnd)
	{
		dwRet = m_lpHighChan->Write( lpBuff,dwRemain,wClientId );		
		if( dwRet == dwRemain )
		{
			dwRemain = 0;
			break;
		}
		lpBuff += dwRet;
		dwRemain -= dwRet;
		dwRetry--;
		TRACE( _T("Send up failed,Retry number is %d\n"),dwRetry );
		Sleep( 10 );
	}

	return ( dwSize - dwRemain );
}

bool CControlCenter::IsLastPackage( DIAG_HEADER* lpHead )
{
	if( NULL == lpHead )
	{
		return true;
	}

	bool bLast = true;

	for( unsigned int i=0;i<m_nTypeDefCount;i++ )
	{
		if( lpHead->type == m_TypeDef[i*2] )
		{
			if( lpHead->subtype == m_TypeDef[i*2+1] )
			{
				return true;
			}
			else
			{
				bLast = false;
			}
		}
	}	
	return bLast;
}

void CControlCenter::LoadTypeDef()
{
	wchar_t szFileName[MAX_PATH];
	if( !GetModuleFileNameW( NULL,szFileName,MAX_PATH-20 ) )
	{
		return;
	}

	wchar_t* lpPath = wcsrchr( szFileName,L'\\' );
	if( lpPath )
	{
		lpPath++;
		*lpPath = 0;
	}

	wcscat( szFileName,L"type_def.def" );
	
	FILE* fp = _wfopen( szFileName,L"r" );
	if( NULL == fp )
	{
		return;
	}

#define MCOL 255
	char buff[MCOL];
	fgets( buff,MCOL,fp );
	fgets( buff,MCOL,fp );
	for( unsigned int i=0;i<TYPE_DEF_COUNT;i++ )
	{
		int type,subtype;
		if( 2 == fscanf( fp,"type=%d,subtype=%d",&type,&subtype ) )
		{
			m_TypeDef[i*2] = (unsigned char)type;
			m_TypeDef[i*2+1] = (unsigned char)subtype;
			m_nTypeDefCount++;
		}
		else
		{
			break;
		}
	}

	fclose( fp );
}

void CControlCenter::OnTimerInterval()
{
	// To see if there are some request timeout

	static unsigned int nTime = 0;

// 	EnterCriticalSection( &m_csMessage );
// 	POSITION pos = m_Messages.GetStartPosition();
// 	DWORD dwKey;
// 	PRT_BUFF* lpBuff;
// 	BOOL bError = FALSE;
// 	while( pos )
// 	{
// 		m_Messages.GetNextAssoc( pos,dwKey,lpBuff );
// 		lpBuff->reserved--;
// 		if( lpBuff->reserved == 0 )
// 		{
// 			// This request is timeout,send timeout message to client
// 			// and remove it from the request list
// 			m_Messages.RemoveKey( dwKey );
// 			DIAG_HEADER* lpHead = (DIAG_HEADER*)lpBuff->lpData;
// 			if( 0 == SendUp( lpHead->len,dwKey,MSG_TIME_OUT,0,TRUE ) )
// 			{
// 				// Error occurs when sending data via socket
// 				bError = TRUE;
// 			}
// 			lpBuff->free_prt( lpBuff );
// 		}
// 	}
// 
// 	LeaveCriticalSection( &m_csMessage );

	if( ((++nTime)% 5) != 0 )
	{
		return; //each 5s to sendup heart-beat packet
	}

	BOOL bError = FALSE;
	WORD dwID = 0;
	Client_Info * lpInfo;
	EnterCriticalSection( &m_csClient );
	POSITION pos = m_Clients.GetStartPosition();
	while( pos != NULL )
	{
		m_Clients.GetNextAssoc( pos,dwID,lpInfo );
		if( 0 == SendUp( dwID,0,MSG_TIME_OUT,0,TRUE ) )
		{
			// Error occurs when sending data via socket
			bError = TRUE;
		}
		
	}
	LeaveCriticalSection( &m_csClient );


	if( bError )
	{
		RefreshClientInfo();
	}

}
/*
BOOL CControlCenter::ConvertEndian( PRT_BUFF* lpBuff,BOOL bToMs )
{
	if( NULL == m_pConvertor )
	{
		return FALSE;
	}

	BOOL bRet = TRUE;
	EnterCriticalSection( &m_csConvert );
	try
	{
		if( bToMs )
		{
			if( m_nEndian )
			{
				m_pConvertor->ConvertToBigEndian( lpBuff->lpData,lpBuff->size );
			}
		}
		else
		{
			if( m_nEndian )
			{
				m_pConvertor->ConvertToLittleEndian( lpBuff->lpData,lpBuff->size );
			}
		}
	}
    catch (CCDFException *e) //lint !e1752
	{
		e->Delete();
		bRet = FALSE;
	}

	LeaveCriticalSection( &m_csConvert );
	return bRet;
}*/

BOOL CControlCenter::OnItcReq(DWORD dwClientId,PRT_BUFF* lpBuff )
{
	USES_CONVERSION;
	BOOL bRet = FALSE;
	DIAG_HEADER* lpPkg = (DIAG_HEADER*)(lpBuff->lpData);
	
	if( 0 == lpPkg->subtype )
	{		
		BYTE buf[12]={0};
		lpPkg->len = 12;
		memcpy( buf,lpPkg,sizeof( DIAG_HEADER ) );
		*(DWORD*)(buf+sizeof(DIAG_HEADER) ) = 2000;
		PRT_BUFF pb;
		pb.size = 12;
		pb.lpData = buf;
		SendUp( (WORD)dwClientId,&pb,TRUE );
		bRet = TRUE;
	}
	else
	{
		if( (lpBuff->size-sizeof( DIAG_HEADER )) != sizeof(ITC_Req_T) || lpPkg->subtype != ITC_REQ_SUBTYPE)
		{
			lpBuff->free_prt( lpBuff );
			return FALSE;
		}

		ITC_Req_T* lpReq = (ITC_Req_T*)( lpBuff->lpData + sizeof( DIAG_HEADER ) );
		
		BYTE RepBuf[sizeof( DIAG_HEADER ) + sizeof( ITC_Rep_T )];
		DIAG_HEADER* pRecv_Pkt_Header = (DIAG_HEADER*)RepBuf;
		ITC_Rep_T* lpRep = (ITC_Rep_T*)( RepBuf + sizeof( DIAG_HEADER ) );
		
		pRecv_Pkt_Header->sn = 0;
		pRecv_Pkt_Header->len = sizeof( DIAG_HEADER ) + sizeof( ITC_Rep_T );
		pRecv_Pkt_Header->type = ITC_REQ_TYPE;
		pRecv_Pkt_Header->subtype = ITC_REP_SUBTYPE;
		lpRep->dwReqType = lpReq->dwReqType;
		
		POSITION pos = NULL;
		Client_Info* lpInfo = NULL;
		DWORD dwTarget = 0xFFFFFFFF;
		PRT_BUFF pb;
		
		switch( lpReq->dwReqType )
		{
		case Reg_Tool:
			// Set client name
			EnterCriticalSection( &m_csClient );
			if( m_Clients.Lookup( (WORD)dwClientId,lpInfo ) )
			{
				_tcsncpy( lpInfo->szToolName,A2T(lpReq->szToolName), MAX_TOOL_NAME-1);
			}
			LeaveCriticalSection( &m_csClient );
			bRet = TRUE;
			break;
			
		case Unreg_Tool:
			bRet = TRUE;
			break;
			
		case Query_Tool:
			EnterCriticalSection( &m_csClient );
			pos = m_Clients.GetStartPosition();
			while( pos != NULL )
			{
				WORD dwID;
				m_Clients.GetNextAssoc( pos,dwID,lpInfo );
				if( dwID == dwClientId )
				{
					continue;
				}
				if(_tcsicmp(lpInfo->szToolName,A2T(lpReq->szToolName)) == 0)
				{
					// Found,send response to the client
					lpRep->dwRetCode = 0;
					break;
				}
			}
			LeaveCriticalSection( &m_csClient );
			
			if( pos == NULL )
			{
				// Not found
				lpRep->dwRetCode = 1;
			}
			
			pb.size = pRecv_Pkt_Header->len;
			pb.lpData = RepBuf;
			SendUp( (WORD)dwClientId,&pb,TRUE );
			
			bRet = TRUE;
			break;
			
		case Logel_ClearLog:
		case Logel_SaveLog:
		case Logel_Rec_keynote:
		case Script_Resume:
			EnterCriticalSection( &m_csClient );
			pos = m_Clients.GetStartPosition();
			while( pos != NULL )
			{
				WORD dwID;
				m_Clients.GetNextAssoc( pos,dwID,lpInfo );
				if( dwID == dwClientId )
				{
					continue;
				}
				if(_tcsicmp(lpInfo->szToolName,A2T(lpReq->szToolName)) == 0)
				{
					// Found,send response to the client
					dwTarget = dwID;
					break;
				}
			}
			LeaveCriticalSection( &m_csClient );
			
			if( dwTarget == 0xFFFFFFFF )
			{
				lpRep->dwReqType = 0;
			}
			else
			{
				lpRep->dwRetCode = 1;
				SendUp( (WORD)dwTarget,lpBuff,TRUE );
			}
			
			pb.size = pRecv_Pkt_Header->len;
			pb.lpData = RepBuf;
			SendUp( (WORD)dwClientId,&pb,TRUE );
			bRet = TRUE;
			break;
			
		case Query_Endian:
			if( m_nEndian < ENDIAN_SELFADAPT )
			{
				// Endian is known,just send response
				lpRep->dwRetCode = ( m_nChannelType << 8 ) | m_nEndian;
				pb.size = pRecv_Pkt_Header->len;
				pb.lpData = RepBuf;
				SendUp( (WORD)dwClientId,&pb,TRUE );
				bRet = TRUE;
			}
			else
			{
				// Endian is unknown,send a query endian command down
				// In this function,we don't send a response to the client,
				// it will be send when the query endian is returned and a 
				// endian event is received by OnChannelEvent
				DIAG_HEADER dh;
				dh.sn = SPECIAL_SN;
				dh.len = 8;
				dh.type = 0;
				dh.subtype = 0;
				PRT_BUFF prt;
				prt.lpData = (BYTE*)&dh;
				SendDown( &prt );
				bRet = TRUE;
			}
			break;
/*        case Reg_Tool_Care_Type:
            {
                bool bFindClient = false;

                EnterCriticalSection( &m_csClient );
                pos = m_Clients.GetStartPosition();
                while( pos != NULL )
                {
                    WORD dwID;
                    m_Clients.GetNextAssoc( pos,dwID,lpInfo );
                    if( dwID == dwClientId )
                    {
                        bFindClient = true;
                        break;
                    }
                }
			    LeaveCriticalSection( &m_csClient );

                if (bFindClient)
                {
                    Care_Type cType = (Care_Type)_ttoi(A2T(lpReq->szToolName));

					if(cType < ANONY_MSG_COUNT)
					{
						m_clientData[cType][dwClientId] = 1;
					}
                }
                
                break;
            }
*/
		default:
			break;
		}
	}

	lpBuff->free_prt( lpBuff );
	return bRet;
}

UINT CControlCenter::GetClientInfo( Client_Info** lppInfo )
{
	if( NULL == lppInfo )
	{
		return 0;
	}

	EnterCriticalSection( &m_csClient );
	UINT nCount = m_Clients.GetCount();
	if( nCount > 0 )
	{
		Client_Info* lpClient = new Client_Info[nCount];
		if( NULL == lpClient )
		{
			nCount = 0;
		}
		else
		{
			*lppInfo = lpClient;
			POSITION pos = m_Clients.GetStartPosition();
			while( pos != NULL )
			{
				WORD dwID;
				Client_Info* lpInfo;
				m_Clients.GetNextAssoc( pos,dwID,lpInfo );
				memcpy( lpClient,lpInfo,sizeof( Client_Info ) );
				lpClient++;
			}
		}
	}
	LeaveCriticalSection( &m_csClient );
	return nCount;
}

void CControlCenter::StartLog()
{
	m_ChannelLog.Open( _T("MS_TO_CS"),ISpLog::typeBinary | ISpLog::modeBinRead );
	m_ToolLog.Open( _T("CS_TO_TOOL"),ISpLog::typeBinary | ISpLog::modeBinWrite );
	m_RequestLog.Open( _T("Request"),ISpLog::typeText );
	m_bLog = TRUE;
/*  // [[ for self debug
	m_pFileSmp = _tfopen(_T("C:\\smp.log"),_T("w+b"));
	m_pFileOrg = _tfopen(_T("C:\\org.log"),_T("w+b"));
	// ]]
*/
}

void CControlCenter::StopLog()
{
	m_bLog = FALSE;
	m_ChannelLog.Close();
	m_ToolLog.Close();
	m_RequestLog.Close();
/*  // [[ for self debug
	if(m_pFileSmp)
	{
		fclose(m_pFileSmp);
		m_pFileSmp = NULL;
	}
	if(m_pFileOrg)
	{
		fclose(m_pFileOrg);
		m_pFileOrg = NULL;
	}
	// ]]
*/
}

BOOL CControlCenter::IsCommSockData( BYTE* lpBuff )
{
	// If the client use a commsock library to connect to ChannelServer,
	// it will send a WSHOSTINFO structure immediately after the connection
	// is established. The HOSTINFO is useless for us now, So we must distinguish
	// and remove it
	// below is the definition of the WSHOSTINFO struct
	//
	// typedef struct WSHOSTINFO
	// {
	//    char   HostName[32];
	//    char   HostAddr[16];
    //    int    HostID;
	// }WSHOSTINFO,*PWSHOSTINFO;
	//
	// We will check the HostAddr field,If it contains a valid ip address,we will consider
	// the whole buffer contains a WSHOSTINFO structure
	char* lpAddr = (char*)lpBuff + 32;
	DWORD dwAddr1=0,dwAddr2=0,dwAddr3=0,dwAddr4=0;
	int n = sscanf( lpAddr,"%d.%d.%d.%d",&dwAddr1,&dwAddr2,&dwAddr3,&dwAddr4 );
	if( n < 4 )
	{
		return FALSE;
	}
	if( dwAddr1 <= 255 && dwAddr2 <= 255 && dwAddr3 <= 255 && dwAddr4 <= 255 )
	{
		return TRUE;
	}
	return FALSE;
}

void CControlCenter::TellClientsEndian()
{
	BYTE RepBuf[sizeof( DIAG_HEADER ) + sizeof( ITC_Rep_T )];
	DIAG_HEADER* pRecv_Pkt_Header = (DIAG_HEADER*)RepBuf;
	ITC_Rep_T* lpRep = (ITC_Rep_T*)( RepBuf + sizeof( DIAG_HEADER ) );
	
	pRecv_Pkt_Header->sn = 0;
	pRecv_Pkt_Header->len = sizeof( DIAG_HEADER ) + sizeof( ITC_Rep_T );
	pRecv_Pkt_Header->type = ITC_REQ_TYPE;
	pRecv_Pkt_Header->subtype = ITC_REP_SUBTYPE;
	lpRep->dwReqType = Query_Endian;
	lpRep->dwRetCode = ( m_nChannelType << 8 ) | m_nEndian;

	PRT_BUFF pb;
	pb.size = pRecv_Pkt_Header->len;
	pb.lpData = RepBuf;

	SendUp( 0,&pb,TRUE );
}

void CControlCenter::RefreshClientInfo()
{
	USES_CONVERSION;
	CMainFrame *pMF = (CMainFrame *)m_pWnd;
	CString strLog;
	strLog.Format(_T("CControlCenter::RefreshClientInfo +++"));
	pMF->Log(strLog);
	// Query client ip address and host name
	DWORD dwCount = 0;
	m_lpHighChan->GetProperty( 0,CH_PROP_SOCKET_CLIENTS_COUNT,&dwCount );
	PCHANNEL_ATTRIBUTE pAttr = NULL;
	if( dwCount > 0 )
	{
		pAttr = new CHANNEL_ATTRIBUTE[dwCount];
		if( !m_lpHighChan->GetProperty( 0,CH_PROP_SOCKET_CLIENTS_INFO,pAttr ) )
		{
			// Can not get Client info
			dwCount = 0;
			if( m_bLog )
			{
				m_RequestLog.LogRawStrA( SPLOGLV_ERROR,"Can not get client info from socket channel!" );
			}
		}
	}
	
	EnterCriticalSection( &m_csClient );
	POSITION pos = m_Clients.GetStartPosition();
	while( pos )
	{
		WORD wClientId;
		Client_Info* pInfo = NULL;
		m_Clients.GetNextAssoc( pos,wClientId,pInfo );
		
		bool bFind = false;
		for( UINT i=0;i<dwCount;i++ )
		{
			if( wClientId == pAttr[i].Socket.dwFlag )
			{
				pAttr[i].Socket.dwFlag = 0xFFFFFFFF;
				bFind = true;
				break;
			}
		}
		if( !bFind )
		{
			// This client is not exist in socket channel,remove it 
			// Unreg all messages
			Unreg( wClientId,0 ,TRUE);
			
			// Remove all pending request
			ProcessRequest( wClientId,NULL);
			ReleaseDiagPackage( pInfo->lpPkg );
			delete pInfo;			
			m_Clients.RemoveKey( wClientId );
		}
	}
	
	for( UINT i=0;i<dwCount;i++ )
	{
		if( pAttr[i].Socket.dwFlag != 0xFFFFFFFF )
		{
			// This client is exist in socket channel, but not exist
			// in our client list. Add it
			Client_Info* lpInfo = new Client_Info;
			memset( lpInfo,0,sizeof( Client_Info ) );
			CreateDiagPackage( &lpInfo->lpPkg );
			lpInfo->lpPkg->SetProperty( 0,PPI_DIAG_Type,(void*)DIAG_TYPE_NO_HEADER );
			lpInfo->lpPkg->SetProperty( 0,PPI_Endian,(void*)( PP_LITTLE_ENDIAN << 8 | PP_LITTLE_ENDIAN ) );
			lpInfo->bFirstReceive = TRUE;
			time(&lpInfo->time);

			DWORD dwIP;
			CONVERT_INT( pAttr[i].Socket.dwIP,dwIP );
			HOSTENT* lpHost = gethostbyaddr( (const char*)&dwIP,4,AF_INET );
			TCHAR szIP[MAX_IP_ADDR_LEN] = {0};
			for( int j=0;j<4;j++ )
			{
				BYTE c = (BYTE)(dwIP & 0xFF);
				TCHAR szTemp[4];
				_itot( c,szTemp,10 );
				_tcscat( szIP,szTemp );
				if( j < 3 )
				{
					_tcscat( szIP,_T(".") );
				}
				dwIP = dwIP >> 8;
			}
			_tcscpy( lpInfo->szIP,szIP );
			if( lpHost )
			{
				_tcsncpy( lpInfo->szHostName,A2T(lpHost->h_name),MAX_HOST_NAME_LEN - 1 );
			}
			m_Clients.SetAt( (WORD)(pAttr[i].Socket.dwFlag),lpInfo );
		}
	}

	LeaveCriticalSection( &m_csClient );

	
	if(pAttr != NULL)
	{
		delete[] pAttr;
		pAttr = NULL;
	}

	strLog.Format(_T("CControlCenter::RefreshClientInfo ---"));
	pMF->Log(strLog);
}

void CControlCenter::TellClientsConnectState(BOOL bConnect)
{
       
	BYTE RepBuf[sizeof( DIAG_HEADER ) + sizeof( ITC_Rep_T )] = {0};
    DIAG_HEADER* pRecv_Pkt_Header = (DIAG_HEADER*)RepBuf;
    ITC_Rep_T* lpRep = (ITC_Rep_T*)( RepBuf + sizeof( DIAG_HEADER ) );
    
    pRecv_Pkt_Header->sn = 0;
    pRecv_Pkt_Header->len = sizeof( DIAG_HEADER ) + sizeof( ITC_Rep_T );
    pRecv_Pkt_Header->type = ITC_REQ_TYPE;
    pRecv_Pkt_Header->subtype = ITC_REP_SUBTYPE;
    lpRep->dwReqType = Reg_Tool_Care_Type;
	lpRep->dwRetCode = bConnect?eEVENT_DEVICE_PLUGIN:eEVENT_DEVICE_REMOVED;
    
    PRT_BUFF pb;
    pb.size = pRecv_Pkt_Header->len;
    pb.lpData = RepBuf;
    
    SendUp(0, &pb, TRUE); //Broadcast
}