// BootModeOpr.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "BMPlatform.h"
#include "BootModeOpr.h"
#include <process.h>
#include "OptionHelpper.h"

extern "C"
{
#include "bmpacket.h"
#include "crc16.h"
}

#define  WM_RCV_CHANNEL_DATA    WM_APP + 301 // channel data received
#define  WM_CHANNEL_CLOSE       WM_RCV_CHANNEL_DATA + 1

#define LOG_FILENAME_PREFIX     _T("BootModeOpr_")
#define CRC_FILENAME_PREFIX     _T("CrcCfg_")
#define MAX_LOG_LEN             256

#define LOG_OPR		 if(NULL != m_pLogFile)LogOpr
#define LOG_OPR_FMT  if(NULL != m_pLogFile)LogOprFmt

#ifdef HIGH_PRECIOUS_TIME

#include <Windows.h>

inline double __fastcall usTimeCounter(bool bStart)
{
	double dResult = 0.0f;
	static LARGE_INTEGER tFrep;
	static LARGE_INTEGER tBegin;
	static bool bInit = false;
	LARGE_INTEGER tEnd;
	if(!bInit)
	{
		QueryPerformanceFrequency(&tFrep);
		bInit = true;
	}

	if(bStart)
	{
		QueryPerformanceCounter(&tBegin);
		return dResult;
	}
	else
	{
		QueryPerformanceCounter(&tEnd);
	}

	dResult = ((double)(tEnd.QuadPart - tBegin.QuadPart))/(double)tFrep.QuadPart;

	return dResult;
}
#endif	


extern COptionHelpper ohObject;
/////////////////////////////////////////////////////////////////////////////
// CBootModeOpr
CBootModeOpr::CBootModeOpr()
{
	m_bHostProtocol = FALSE;
#ifdef NOT_USE_DIAGCHAN
	m_hOprEvent = NULL;
	m_pOrgData = NULL;
	m_pOutData = NULL;
	m_pOrgData = new BYTE[MAX_BM_PKG_SIZE];
	m_pOutData = new BYTE[MAX_BM_PKG_SIZE*2+4];
	m_bSupportZLP = ohObject.IsSupportZroPkg();
	m_bWorking = FALSE;
#ifdef  CHAN_SYNC
	ZeroMemory( m_RecvData, MAX_RECV_LEN );
	//m_ulRecvDataLen = 0;
#endif // CHAN_SYNC

#endif
    m_bCheckCrc  = TRUE;
    m_bBigEndian = TRUE;


    m_iLastSentPktType = BSL_PKT_TYPE_MAX; 
    m_dwLastErrorCode = 0; 
    m_bOperationSuccess = FALSE;
    m_pLogFile = NULL;
    m_pCrcFile = NULL;
    m_pChannel = NULL;

    m_dwRecvThreadID = 0;
    m_hRecvThread = NULL;
    m_hRecvThreadState = NULL;


    m_lpReceiveBuffer = NULL;
    m_dwReceiveLen = 0;
    m_dwBufferSize = 0;
    m_dwLastPktSize = 0;
    m_uiReadOffset = 0;
	m_dwCookie = 0;

	m_bHasCheckSum  = FALSE;
	m_dwBaud = DEFAULT_BAUDRATE;

	m_bGotVerAck = FALSE;
    m_bOpened    = FALSE;

    ::InitializeCriticalSection( &m_csRecvBbuffer );
	::InitializeCriticalSection( &m_csLastError );
    ::InitializeCriticalSection(&m_csClosePort);
}

CBootModeOpr::~CBootModeOpr()
{
    FreeRecvBuffer();
    ::DeleteCriticalSection( &m_csRecvBbuffer );
	::DeleteCriticalSection( &m_csLastError );
    ::DeleteCriticalSection(&m_csClosePort);

#ifdef NOT_USE_DIAGCHAN
	SAFE_DELETE_ARRAY(m_pOrgData);
	SAFE_DELETE_ARRAY(m_pOutData);
#endif

}

BOOL CBootModeOpr::Initialize( LPCTSTR lpszChannelProgID, LPBYTE lpOpenParam,
                              BOOL bBigEndian, DWORD dwCookie  ) 
{
	USES_CONVERSION;
    m_bOpened = FALSE;
	InitLogFileSuffix(dwCookie);
	OpenLogFile( dwCookie );
    OpenCrcFile( dwCookie );
	CString strLog;
   
    CString strChannel = lpszChannelProgID;
	if(strChannel.CompareNoCase(_T("Channel.Uart"))!=0)
    {
        strLog.Format( _T("Unknown channel type.[%s]"),lpszChannelProgID);
        Log( strLog );
        return FALSE;
    }

#ifdef NOT_USE_DIAGCHAN
	if(!g_theApp.m_pfCreateChannel)
	{
		strLog.Format( _T("Create channel function is emtpy"));
		Log( strLog );
		return FALSE;
	}
    if( !g_theApp.m_pfCreateChannel((ICommChannel **)&m_pChannel,CHANNEL_TYPE_COM) )
    {
		strLog.Format( _T("Create channel object fail.[0x%08X]"), GetLastErrorCode() );
        Log( strLog );
        return FALSE;
    }
#ifdef  CHAN_SYNC
	ZeroMemory( m_RecvData, MAX_RECV_LEN );
	//m_ulRecvDataLen = 0;
#endif // CHAN_SYNC

#else
    if( !CreateBmChannel((IProtocolChannel **)&m_pChannel) )
    {
		strLog.Format( _T("Create channel object fail.[0x%08X]"), GetLastErrorCode() );
        Log( strLog );
        return FALSE;
    }
	m_pChannel->SetProperty( 0,PPI_Endian,(void*)m_bBigEndian );
#endif
   
    m_bBigEndian = bBigEndian;
    m_iLastSentPktType = BSL_PKT_TYPE_MAX; 
    m_dwLastErrorCode = 0; 
    m_bOperationSuccess = FALSE;
	m_bGotVerAck = FALSE;
	m_bHostProtocol = FALSE;


#ifdef NOT_USE_DIAGCHAN
#ifdef  CHAN_SYNC
	m_bWorking = TRUE;
#else	
	if( FALSE == CreateRecvThread() )
    {
		Log( _T("Create receive thread fail."));
		return FALSE;
    }
#endif //CHAN_SYNC
#endif //NOT_USE_DIAGCHAN
	InitComLog();
    m_bOpened = ConnectChannel( lpOpenParam );
    return m_bOpened;  
}

void CBootModeOpr::Uninitialize()
{
    DisconnectChannel();     
#ifdef NOT_USE_DIAGCHAN
#ifdef  CHAN_SYNC
	m_bWorking = FALSE;
	ZeroMemory( m_RecvData, MAX_RECV_LEN );
	//m_ulRecvDataLen = 0;    
#endif // CHAN_SYNC	
	DestroyRecvThread();	
#endif
    m_bCheckCrc  = TRUE;
	m_bGotVerAck = FALSE;

    m_iLastSentPktType = BSL_PKT_TYPE_MAX; 
    CloseLogFile();
	
	if (m_pChannel)
	{
#ifdef NOT_USE_DIAGCHAN
		if(g_theApp.m_pfReleaseChannel)
			g_theApp.m_pfReleaseChannel(m_pChannel);
#else
		ReleaseBmChannel(m_pChannel);
#endif
		m_pChannel = NULL;
	}
	
//  CoUninitialize();    
}

BOOL CBootModeOpr::CheckBaud( DWORD dwTimeout /* = 1000  */)
{
    //BYTE btSendData[ PACHET_HDR_LEN*2 ];    
    //memset( btSendData, 0x7E, PACHET_HDR_LEN*2);
	if( m_bGotVerAck )
	{
		m_bGotVerAck = FALSE;
		return TRUE;
	}
	
	int nSize = ohObject.Get7ENumOnce();
	if(nSize<0)
	{
		nSize = 1;
	}
	if(nSize > PACHET_HDR_LEN*2 )
	{
		nSize = PACHET_HDR_LEN*2;
	}

#ifdef NOT_USE_DIAGCHAN
	BOOL bSuccess = SendPacketData( BSL_CMD_CHECK_BAUD,nSize,NULL,nSize,dwTimeout );
#else
	//Clear received data, maybe there exist dirty data.
	m_pChannel->SetProperty(0,PPI_CLEAR_KEEP_PACKAGE,(LPCVOID)true);
	m_pChannel->Clear();
	m_pChannel->SetProperty(0,PPI_CLEAR_KEEP_PACKAGE,(LPCVOID)false);

    BOOL bSuccess = SendPacketData( BSL_CMD_CHECK_BAUD,nSize,NULL,nSize,dwTimeout );
#endif	
	
	if( m_dwLastErrorCode == BSL_UART_SEND_ERROR)
	{
		Sleep( 500 );
	}

	if(m_bGotVerAck)
	{
		bSuccess = TRUE;
	}

    LOG_OPR( _T("Check Baudrate"), bSuccess );

	if(bSuccess)
	{
		m_bGotVerAck = FALSE; // reset to false
	}

    return bSuccess;
}

BOOL CBootModeOpr::SetFdlFileInfo(const BMFileInfo* pBMFileInfo)
{
    return m_sHostProtocol.SetFdlFileInfo(pBMFileInfo);
}

BOOL CBootModeOpr::HostConnect( DWORD dwTimeout)
{
	m_bHostProtocol = TRUE;
	LPBYTE lpPkgBuf = NULL;
	int nPkgSize = 0;
	if (!m_sHostProtocol.Assemble_Packet_Connect(lpPkgBuf, nPkgSize))
	{
		Log(_T("Failed to assemble connect packet."));
		return FALSE;
	}
	return SendHostData(lpPkgBuf,nPkgSize, dwTimeout);
}


BOOL CBootModeOpr::HostDownloadStart( DWORD dwTimeout )
{

	LPBYTE lpPkgBuf = NULL;
	int nPkgSize = 0;
	if (!m_sHostProtocol.Assemble_Packet_Start(lpPkgBuf, nPkgSize))
	{
		Log(_T("Failed to assemble start packet."));
		return FALSE;
	}

	return SendHostData(lpPkgBuf,nPkgSize,dwTimeout);

}

BOOL CBootModeOpr::HostDownloadData( int nSeq,DWORD dwTimeout )
{
	LPBYTE lpPkgBuf = NULL;
	int nPkgSize = 0;
	if (!m_sHostProtocol.Assemble_Packet_Data(nSeq,lpPkgBuf, nPkgSize))
	{
		Log(_T("Failed to assemble data packet."));
		return FALSE;
	}
	return SendHostData(lpPkgBuf,nPkgSize,dwTimeout);;
}

BOOL CBootModeOpr::HostDownloadEnd( DWORD dwTimeout )
{

	LPBYTE lpPkgBuf = NULL;
	int nPkgSize = 0;
	if (!m_sHostProtocol.Assemble_Packet_End(lpPkgBuf, nPkgSize))
	{
		Log(_T("Failed to assemble end packet."));
		return FALSE;
	}
    return SendHostData(lpPkgBuf,nPkgSize, dwTimeout);

}

BOOL CBootModeOpr::HostExcute( DWORD dwTimeout)
{
	LPBYTE lpPkgBuf = NULL;
	int nPkgSize = 0;
	if (!m_sHostProtocol.Assemble_Packet_Execute(lpPkgBuf, nPkgSize))
	{
		Log(_T("Failed to assemble end packet."));
		return FALSE;
	}

	m_dwLastErrorCode = BSL_CMD_EXEC_DATA;
	SendHostData(lpPkgBuf,nPkgSize, dwTimeout,FALSE);  //不需要判断结果

	Log( _T("Excute downloaded file") );
	m_bHostProtocol = FALSE;
	return TRUE;

}


BOOL CBootModeOpr::Connect( DWORD dwTimeout /* = 1000  */)
{
   BOOL bSuccess = SendCommandData( BSL_CMD_CONNECT, dwTimeout  );
   LOG_OPR( _T("Connect to Module"), bSuccess );
   
   return bSuccess;
}

BOOL CBootModeOpr::Excute( DWORD dwTimeout /* = 1000  */ )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_EXEC_DATA , dwTimeout );
    LOG_OPR( _T("Excute  downloaded file"), bSuccess );
    
    return bSuccess;
}

BOOL CBootModeOpr::Reset(LPBYTE lpRefData, DWORD dwLength, DWORD dwTimeout /* = 1000  */ )
{
    UINT uiDataLen =  MAX_REF_INFO_LEN ;
    LPBYTE lpPackData = m_pOrgData;

    ZeroMemory(lpPackData,uiDataLen);
    memcpy(lpPackData,lpRefData,dwLength> MAX_REF_INFO_LEN? MAX_REF_INFO_LEN : dwLength); 

    BM_PACKAGE bp;
    bp.header.type = (unsigned short)BSL_CMD_NORMAL_RESET;
    bp.header.len = (unsigned short)dwLength;
    bp.data = lpPackData;

	DWORD dwCloseDevAfterResset = ohObject.CloseDevBeforeRemove();
	BOOL bSuccess =  SendData( &bp,dwCloseDevAfterResset? 0: dwTimeout);
    LOG_OPR( _T("Reset"), bSuccess );
	if (dwCloseDevAfterResset && !ohObject.IsUartDownload())
	{
		Log(_T("Auto close DUT after reset."));
		CloseDevBeforeRemove();
	}

    return bSuccess;
}

BOOL CBootModeOpr::SetFirstMode(LPBYTE lpRefData, DWORD dwLength, DWORD dwTimeout /* = 1000  */ )
{
    UINT uiDataLen =  MAX_REF_INFO_LEN ;
    LPBYTE lpPackData = m_pOrgData;

    ZeroMemory(lpPackData,uiDataLen);
    memcpy(lpPackData,lpRefData,dwLength> MAX_REF_INFO_LEN? MAX_REF_INFO_LEN : dwLength); 

    BM_PACKAGE bp;
    bp.header.type = (unsigned short)BSL_CMD_SET_FIRST_MODE;
    bp.header.len = (unsigned short)dwLength;
    bp.data = lpPackData;

    BOOL bSuccess =  SendData( &bp, dwTimeout);
    LOG_OPR( _T("SetFirstMode"), bSuccess );

    return bSuccess;
}

BOOL CBootModeOpr::PowerOff( DWORD dwTimeout /* = 1000*/ )
{
	DWORD dwCloseDev = ohObject.CloseDevBeforeRemove();
	BOOL bSuccess = SendCommandData( BSL_CMD_POWER_OFF , (dwCloseDev ==2 )? 0 : dwTimeout );
	LOG_OPR( _T("Power Off"), bSuccess );
	if (dwCloseDev ==2 && !ohObject.IsUartDownload())
	{
		Log(_T("Auto close DUT after poweroff."));
		CloseDevBeforeRemove();
	}
	return bSuccess;
}

BOOL CBootModeOpr::CheckRoot( DWORD dwTimeout /* = 1000*/ )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_CHECK_ROOT , dwTimeout );
    LOG_OPR( _T("CheckRoot"), bSuccess );
    return bSuccess;
}

BOOL CBootModeOpr::EnableFlash( DWORD dwTimeout /* = 1000*/ )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_ENABLE_WRITE_FLASH , dwTimeout );
    LOG_OPR( _T("Enable Flash Done"), bSuccess );
    return bSuccess;
}

BOOL CBootModeOpr::EnableDebugMode(LPBYTE lpData, DWORD dwLength, DWORD dwTimeout/* = 1000*/)
{
	BOOL bSuccess = FALSE;
	
	if(dwLength == 0 || NULL ==lpData)
	{
		bSuccess = SendCommandData( BSL_CMD_ENABLE_DEBUG_MODE , dwTimeout );
	}
	else
	{
#ifdef NOT_USE_DIAGCHAN
		LPBYTE lpPackData = m_pOrgData;
#else
		LPBYTE lpPackData = new BYTE[dwLength];
		if( lpPackData == NULL )
			return FALSE;    
#endif
		ZeroMemory(lpPackData,dwLength);
		memcpy(lpPackData,lpData,dwLength); 

		BM_PACKAGE bp;
		bp.header.type = (unsigned short)BSL_CMD_ENABLE_DEBUG_MODE;
		bp.header.len = (unsigned short)dwLength;
		bp.data = lpPackData;

		bSuccess =  SendData( &bp, dwTimeout );
	}
	LOG_OPR( _T("Enable Debug Mode Done"), bSuccess );

	return bSuccess;
}

BOOL   CBootModeOpr::EndProcess( DWORD dwTimeout /* = 1000*/)
{
    BOOL bSuccess = SendCommandData( BSL_CMD_END_PROCESS , dwTimeout );
    LOG_OPR( _T("End Process Done"), bSuccess );
    return bSuccess;
}
BOOL CBootModeOpr::ReadChipType( DWORD dwTimeout /* = 1000  */ )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_READ_CHIP_TYPE , dwTimeout );
    LOG_OPR( _T("Read Chip Type"), bSuccess);

    return bSuccess;
}

BOOL CBootModeOpr::ReadFlashType( DWORD dwTimeout /* = 1000 */ )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_READ_FLASH_TYPE, dwTimeout );
    LOG_OPR( _T("Read Flash Type"),bSuccess );
    return bSuccess;
}

BOOL CBootModeOpr::ReadFlashInfo( DWORD dwTimeout/* = 1000*/ )
{
	BOOL bSuccess = SendCommandData( BSL_CMD_READ_FLASH_INFO, dwTimeout );
    LOG_OPR( _T("Read Flash Info"),bSuccess );
    return bSuccess;
}

BOOL   CBootModeOpr::ReadFlashUID( DWORD dwTimeout/* = 1000 */)
{
	BOOL bSuccess = SendCommandData( BSL_CMD_READ_FLASH_UID, dwTimeout );
    LOG_OPR( _T("Read Flash UID"),bSuccess );
    return bSuccess;
}

BOOL   CBootModeOpr::ReadChipUID( DWORD dwTimeout/* = 1000 */)
{
    BOOL bSuccess = SendCommandData( BSL_CMD_READ_CHIP_UID, dwTimeout );
    LOG_OPR( _T("Read Chip UID"),bSuccess );
    return bSuccess;
}


BOOL   CBootModeOpr::ReadSoftSimEID( DWORD dwTimeout/* = 1000 */)
{
	BOOL bSuccess = SendCommandData( BSL_CMD_READ_SOFTSIM_EID, dwTimeout );
    LOG_OPR( _T("Read SoftSIM EID"),bSuccess );
    return bSuccess;
}
BOOL   CBootModeOpr::ReadRefInfo( LPBYTE lpRefData, DWORD dwLength ,DWORD dwTimeout/* = 1000*/)
{
	UINT uiDataLen =  MAX_REF_INFO_LEN ;

#ifdef NOT_USE_DIAGCHAN
	LPBYTE lpPackData = m_pOrgData;
#else
	LPBYTE lpPackData = new BYTE[uiDataLen];
	if( lpPackData == NULL )
		return FALSE;    
#endif
	ZeroMemory(lpPackData,uiDataLen);
	memcpy(lpPackData,lpRefData,dwLength> MAX_REF_INFO_LEN? MAX_REF_INFO_LEN : dwLength); 

	BM_PACKAGE bp;
	bp.header.type = (unsigned short)BSL_CMD_READ_REFINFO;
	bp.header.len = (unsigned short)uiDataLen;
	bp.data = lpPackData;

	BOOL bSuccess =  SendData( &bp, dwTimeout );
	LOG_OPR( _T("Read reference machine info"),bSuccess );

	return bSuccess;
}

BOOL   CBootModeOpr::WriteAPRInfo( LPBYTE lpRefData, DWORD dwLength ,DWORD dwTimeout/* = 1000*/)
{
	UINT uiDataLen =  dwLength ;

#ifdef NOT_USE_DIAGCHAN
	LPBYTE lpPackData = m_pOrgData;
#else
	LPBYTE lpPackData = new BYTE[uiDataLen];
	if( lpPackData == NULL )
		return FALSE;    
#endif
	ZeroMemory(lpPackData,uiDataLen);
	memcpy(lpPackData,lpRefData,dwLength); 

	BM_PACKAGE bp;
	bp.header.type = (unsigned short)BSL_CMD_WRITE_APR_INFO;
	bp.header.len = (unsigned short)uiDataLen;
	bp.data = lpPackData;

	BOOL bSuccess =  SendData( &bp, dwTimeout );
	LOG_OPR( _T("Write APR Info"),bSuccess );

	return bSuccess;
}

BOOL   CBootModeOpr::ReadTransceiverType(DWORD dwTimeout /*= 1000*/)
{
	BOOL bSuccess = SendCommandData( BSL_CMD_READ_RF_TRANSCEIVER_TYPE, dwTimeout );
	LOG_OPR( _T("Read Transceiver Type"),bSuccess );
	if (bSuccess)
	{	
		LPBYTE lpReadBuffer = GetRecvBuffer();
		DWORD dwSize = GetRecvBufferSize();
		if( lpReadBuffer && dwSize == sizeof( DWORD) )
		{
			TCHAR szInfo[MAX_PATH] = {0};
			_stprintf(szInfo,_T("RF Chip ID : 0x%04X"),*(DWORD*)lpReadBuffer);
			Log(szInfo);
		}
		
	}
	return bSuccess;
}

BOOL CBootModeOpr::EndData( DWORD dwTimeout /* = 1000  */ )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_END_DATA, dwTimeout );
    if( !bSuccess &&  m_dwLastErrorCode == BSL_REP_DOWN_DEST_ERROR )
    {
        Log( _T("End Data:Doownload error in previous command") );
    }
    else
    {
        LOG_OPR( _T("End Data"), bSuccess );
    }
    
    return bSuccess;
}

BOOL CBootModeOpr::ChangeBaud( DWORD dwTimeout /* = 1000  */ )
{
	DWORD dwBaud = 0;
	ICommChannel *pPhyChannel = NULL;
#ifdef NOT_USE_DIAGCHAN
	BOOL bOK  = m_pChannel->GetProperty( 0, CH_PROP_BAUD, &dwBaud );
	pPhyChannel  = m_pChannel;
#else	
	m_pChannel->GetLowerChannel(&pPhyChannel,FALSE);
	if(pPhyChannel== NULL)
	{
		return FALSE;
	}
	BOOL bOK  = pPhyChannel->GetProperty( 0, CH_PROP_BAUD, &dwBaud );
#endif   
    
    if( !bOK )
    {
        return TRUE;
    }   
	
    if( dwBaud == CHANGE_BAUD_NOT_AVAILABLE || dwBaud == m_dwBaud )
    {
        return TRUE;
    }

    UINT uiDataLen = sizeof( DWORD );

#ifdef NOT_USE_DIAGCHAN
	LPBYTE lpPackData = m_pOrgData;
#else
	LPBYTE lpPackData = new BYTE[uiDataLen];
	if( lpPackData == NULL )
		return FALSE;    
#endif 
    
    *(DWORD *)&lpPackData[ 0 ] = m_dwBaud;    
    
    BOOL bSuccess = SendPacketData( BSL_CMD_CHANGE_BAUD, 
                        uiDataLen  + PACHET_HDR_LEN , 
                        lpPackData, uiDataLen, dwTimeout  );

#ifndef NOT_USE_DIAGCHAN
    delete []lpPackData;
#endif

	LOG_OPR_FMT(bSuccess,_T("FDL:Change Baudrate( %d )"), m_dwBaud );
    
    if( bSuccess )
    {
        bSuccess = pPhyChannel->SetProperty( 0, CH_PROP_BAUD, (LPCVOID)&m_dwBaud );
        Sleep( 500 );
    }

	LOG_OPR_FMT(bSuccess,_T("BMPlatform:Change Baudrate( %d )"), m_dwBaud );
    
    return bSuccess;
}

BOOL CBootModeOpr::ReadFlash( DWORD dwBase,DWORD dwLength,
                             DWORD dwTimeout /* = 1000  */  )
{
    UINT uiDataLen =  START_BSL_PKT_LEN - PACHET_HDR_LEN;
#ifdef NOT_USE_DIAGCHAN
	LPBYTE lpPackData = m_pOrgData;
#else
	LPBYTE lpPackData = new BYTE[uiDataLen];
	if( lpPackData == NULL )
		return FALSE;    
#endif 
    
    m_uiReadOffset = dwBase % 4;
    dwLength += m_uiReadOffset;    
	
	BOOL bRetried = FALSE;

ReadFlash_Retry:

    *(DWORD *)&lpPackData[ 0 ] = dwBase;
    *(DWORD *)&lpPackData[ sizeof(DWORD) ] = dwLength;
    m_dwLastPktSize = dwLength;

    BOOL bSuccess = SendPacketData( BSL_CMD_READ_FLASH, START_BSL_PKT_LEN, 
        lpPackData, uiDataLen , dwTimeout  );

	if( !bSuccess && !bRetried )
	{
		// log the current failed information
		LOG_OPR_FMT(bSuccess,_T("%s( Base:0x%08X, Size:0x%08X )"), _T("Read Flash"), dwBase, dwLength );

		bRetried = TRUE;
		// Clear channel dirty data and prepare for repeat reading.
		Log( _T("Clear channel buffer and retry ReadFlash."));
		m_pChannel->Clear();
		goto ReadFlash_Retry;
	}
	
#ifndef NOT_USE_DIAGCHAN
	delete []lpPackData;
#endif

	LOG_OPR_FMT(bSuccess,_T("%s( Base:0x%08X, Size:0x%08X )"), _T("Read Flash"), dwBase, dwLength );	
    
    return bSuccess;
}

// dwBase gives partition id,dwOffset gives offset in this partition
BOOL CBootModeOpr::ReadPartitionFlash( DWORD dwBase,DWORD dwLength,DWORD dwOffset,DWORD dwTimeout /* = 1000 */)
{
    UINT uiDataLen =  START_BSL_PKT_LEN - PACHET_HDR_LEN + sizeof( DWORD );
#ifdef NOT_USE_DIAGCHAN
	LPBYTE lpPackData = m_pOrgData;
#else
	LPBYTE lpPackData = new BYTE[uiDataLen];
	if( lpPackData == NULL )
		return FALSE;    
#endif
    
    m_uiReadOffset = 0;
	BOOL bRetried = FALSE;

ReadPartitionFlash_Retry:
    
    *(DWORD *)&lpPackData[ 0 ] = dwBase;
    *(DWORD *)&lpPackData[ sizeof(DWORD) ] = dwLength;
    *(DWORD *)&lpPackData[ sizeof( DWORD ) * 2 ] = dwOffset;
    m_dwLastPktSize = dwLength;
    
    BOOL bSuccess = SendPacketData( BSL_CMD_READ_FLASH, START_BSL_PKT_LEN + sizeof( DWORD ), 
        lpPackData, uiDataLen , dwTimeout  );

	if( !bSuccess && !bRetried )
	{
		// log the current failed information
		LOG_OPR_FMT(bSuccess,_T("%s( Base:0x%08X, Size:0x%08X, Offset:0x%08X )"), _T("Read Flash"), dwBase, dwLength,dwOffset );

		bRetried = TRUE;
		// Clear channel dirty data and prepare for repeat reading.
		Log( _T("Clear channel buffer and retry ReadPartitionFlash."));
		m_pChannel->Clear();
		goto ReadPartitionFlash_Retry;
	}
	
#ifndef NOT_USE_DIAGCHAN
	delete []lpPackData;
#endif  

    LOG_OPR_FMT(bSuccess,_T("%s( Base:0x%08X, Size:0x%08X, Offset:0x%08X )"), _T("Read Flash"), dwBase, dwLength,dwOffset );
    
    return bSuccess;
}

BOOL CBootModeOpr::ReadNVItem( DWORD dwStartAddr, DWORD dwEndAddr, 
                              unsigned short uiItemID, DWORD dwTimeout /* = 1000  */ )
{
    BYTE lpSendData[ READ_NVITEM_PKT_LEN] = {0};
	DWORD dwDataStartPos = 0;

	if( m_bBigEndian )
    {
        DWORD dwSoruceValue, dwDestValue;
        dwSoruceValue =  dwStartAddr;    
        dwDestValue   = 0;
        CONVERT_INT( dwSoruceValue, dwDestValue);
        *(DWORD *)&lpSendData[dwDataStartPos] = dwDestValue;

        dwSoruceValue = dwEndAddr;    
        dwDestValue   = 0;
        CONVERT_INT( dwSoruceValue, dwDestValue);
        *(DWORD *)&lpSendData[dwDataStartPos + sizeof( DWORD) ] = dwDestValue;

        unsigned short uiSourceValue, uiDestValue;        
        uiSourceValue =  uiItemID;    
        uiDestValue   = 0;
        CONVERT_SHORT( uiSourceValue, uiDestValue);
        *(unsigned short *)&lpSendData[ dwDataStartPos + 2 * sizeof( DWORD) ] = uiDestValue;
    }
    else
    {
        *(DWORD *)&lpSendData[ dwDataStartPos ] = dwStartAddr;
        *(DWORD *)&lpSendData[ dwDataStartPos + sizeof( DWORD)  ] = dwEndAddr;
        *(unsigned short *)&lpSendData[ dwDataStartPos + 2 * sizeof( DWORD)  ] = uiItemID;        
    }    
    

	BM_PACKAGE bp;
	bp.header.type = BSL_CMD_READ_NVITEM;
	bp.header.len = READ_NVITEM_PKT_LEN;
	bp.data = lpSendData;

    BOOL bSuccess = SendData( &bp,dwTimeout );  

   
    LOG_OPR_FMT(bSuccess,_T("%s( Start:0x%08X, End:0x%08X, Item ID:%d )"), _T("Read NV Item"),dwStartAddr, dwEndAddr, uiItemID );
    
    return bSuccess;
}

BOOL CBootModeOpr::EraseFlash( DWORD dwBase,DWORD dwLength , DWORD dwTimeout /* = 1000  */ )
{
    DWORD dwSize = dwLength;
    if( (dwSize != 0xFFFFFFFF) && (dwSize % 2)  )
    {
        dwSize++;
    }
    
    UINT uiDataLen =  2 * sizeof(DWORD) ;
#ifdef NOT_USE_DIAGCHAN
	LPBYTE lpPackData = m_pOrgData;
#else
	LPBYTE lpPackData = new BYTE[uiDataLen];
	if( lpPackData == NULL )
		return FALSE;    
#endif  
    
    *(DWORD *)&lpPackData[ 0 ] = dwBase;
    *(DWORD *)&lpPackData[ sizeof(DWORD) ] = dwSize;
        
    BOOL bSuccess = SendPacketData( BSL_CMD_ERASE_FLASH,
        ERASE_FLASH_PKT_LEN, lpPackData, 
        uiDataLen , dwTimeout );

#ifndef NOT_USE_DIAGCHAN
	delete []lpPackData;
#endif  

    LOG_OPR_FMT(bSuccess,_T("%s( Base:0x%08X, Size:0x%08X)"), _T("Erase Flash"), dwBase, dwLength );
    
    return bSuccess;    
}

BOOL CBootModeOpr::EraseFlash(  LPBYTE pID, DWORD nIDLen,DWORD dwLength , DWORD dwTimeout /* = 1000  */ )
{
    DWORD dwSize = dwLength;
    if( (dwSize != 0xFFFFFFFF) && (dwSize % 2)  )
    {
        dwSize++;
    }
    
    UINT uiDataLen =  nIDLen + sizeof(DWORD) ;
#ifdef NOT_USE_DIAGCHAN
	LPBYTE lpPackData = m_pOrgData;
#else
	LPBYTE lpPackData = new BYTE[uiDataLen];
	if( lpPackData == NULL )
		return FALSE;    
#endif 
    
    memcpy(lpPackData,pID,nIDLen);
    *(DWORD *)&lpPackData[ nIDLen ] = dwSize;
	

	BM_PACKAGE bp;
	bp.header.type = (unsigned short)BSL_CMD_ERASE_FLASH;
	bp.header.len = (unsigned short)uiDataLen;
	bp.data = lpPackData;
    
    BOOL bSuccess =  SendData( &bp, dwTimeout );

#ifndef NOT_USE_DIAGCHAN
	delete []lpPackData;
#endif

    LOG_OPR_FMT(bSuccess,_T("%s( Partion:\"%s\", Size:0x%08X)"), _T("Erase Flash"), (*pID) == 0?_T("_ALL_"):(WCHAR*)pID, dwLength );
    
    return bSuccess;    
}

BOOL CBootModeOpr::StartData( unsigned __int64 llBase,
                              unsigned __int64 llLength , 
                              LPBYTE lpDownLoadData,
                              DWORD dwCheckSum/* = 0*/,
                              DWORD dwTimeout /* = 1000  */,
                              BOOL bIs64Bit/* = FALSE*/ )
{
    UNUSED_ALWAYS( lpDownLoadData ); 
    unsigned __int64 llSize = llLength;
    if( llSize % 2 )
    {
        llSize++;
    }
	
	if(llSize == 0)
	{
		m_dwLastErrorCode = BSL_REP_SIZE_ZERO;
		LOG_OPR_FMT(FALSE,_T("Start Data: ( Base:0x%08I64X ) size is zero."),llBase);
		return FALSE;
	}
	
    UINT uiDataLen =  bIs64Bit ? ( 2 * sizeof(__int64) ) : (2 * sizeof(DWORD)) ;
	if(0 != dwCheckSum)
	{
		uiDataLen += sizeof(DWORD);
	}	
	int nSendDataLen = PACHET_HDR_LEN + uiDataLen;
	
#ifdef NOT_USE_DIAGCHAN
	LPBYTE lpPackData = m_pOrgData;
#else
	LPBYTE lpPackData = new BYTE[uiDataLen];
	if( lpPackData == NULL )
		return FALSE;    
#endif
    ZeroMemory(lpPackData,uiDataLen);
    if (bIs64Bit)// addr(8) + Len(8) + [CS(4)]
    {
        *(unsigned __int64 *)&lpPackData[ 0 ] = llBase;
        *(unsigned __int64 *)&lpPackData[ sizeof(__int64) ] = llSize;
        if(0 != dwCheckSum)
        {
            *(DWORD *)&lpPackData[ sizeof(__int64)*2 ] = dwCheckSum;
        }
    }
    else    // addr(4) + Len(4) + [CS(4)]
    {
        *(DWORD *)&lpPackData[ 0 ] = (DWORD)llBase;
        *(DWORD *)&lpPackData[ sizeof(DWORD) ] = (DWORD)llSize;
        if(0 != dwCheckSum)
        {
            *(DWORD *)&lpPackData[ sizeof(DWORD)*2 ] = dwCheckSum;
        }
    }
     
    BOOL bSuccess = SendPacketData( BSL_CMD_START_DATA,
                        nSendDataLen, lpPackData, 
                        uiDataLen , dwTimeout );
#ifndef NOT_USE_DIAGCHAN
	delete []lpPackData;
#endif 
    
	if(0 == dwCheckSum)
	{
		LOG_OPR_FMT(bSuccess,_T("%s( Base:0x%08I64X, Size:0x%08I64X)"), _T("Start Data"), llBase, llLength );
	}
	else
	{
		LOG_OPR_FMT(bSuccess,_T("%s( Base:0x%08I64X, Size:0x%08I64X, CheckSum:0x%08X)"), _T("Start Data"),
			       llBase, llLength, dwCheckSum);
	}
    
    return bSuccess;
}

BOOL CBootModeOpr::StartData( LPBYTE pID, DWORD nIDLen,
                             unsigned __int64 llLength ,
                             LPBYTE lpDownLoadData, 
                             DWORD dwCheckSum/* = 0*/,
                             DWORD dwTimeout/* = 1000*/,
                             BOOL bIs64Bit/* = FALSE*/ )
{
    UNUSED_ALWAYS( lpDownLoadData ); 
    unsigned __int64 llSize = llLength;
    if( llSize % 2 )
    {
        llSize++;
    }

    if(llSize == 0)
	{
		m_dwLastErrorCode = BSL_REP_SIZE_ZERO;
		LOG_OPR_FMT(FALSE,_T("Start Data: ( Partition:\"%s\" ) size is zero."),(WCHAR*)pID);
		return FALSE;
	}
	
    UINT uiDataLen =  bIs64Bit ? nIDLen + 2* sizeof(__int64): nIDLen + sizeof(DWORD) ;
	if(0 != dwCheckSum)
	{
		uiDataLen += sizeof(DWORD);
	}	

#ifdef NOT_USE_DIAGCHAN
	LPBYTE lpPackData = m_pOrgData;
#else
    LPBYTE lpPackData = new BYTE[uiDataLen];
    if( lpPackData == NULL )
        return FALSE;    
#endif
    ZeroMemory(lpPackData,uiDataLen);
	memcpy(lpPackData,pID,nIDLen); 
    if (bIs64Bit)// ID(72) + Len(8) + Rev(8) + [CS(4)]
    {
        *(unsigned __int64 *)&lpPackData[ nIDLen ] = llSize;
        if(0 != dwCheckSum)
        {
            *(DWORD *)&lpPackData[ nIDLen+ 2*sizeof(__int64) ] = dwCheckSum;
        }
    }
    else    // ID(72) + Len(4)+ [CS(4)]
    {
        *(DWORD *)&lpPackData[ nIDLen ] = (DWORD)llSize;

        if(0 != dwCheckSum)
        {
            *(DWORD *)&lpPackData[ nIDLen+sizeof(DWORD) ] = dwCheckSum;
        }	
    }
	
    BM_PACKAGE bp;
	bp.header.type = (unsigned short)BSL_CMD_START_DATA;
	bp.header.len = (unsigned short)uiDataLen;
	bp.data = lpPackData;
    
    BOOL bSuccess =  SendData( &bp, dwTimeout );

#ifndef NOT_USE_DIAGCHAN
	delete []lpPackData;
#endif 
    
	if(0 == dwCheckSum)
	{
		LOG_OPR_FMT(bSuccess,_T("%s( Partition:\"%s\", Size:0x%08I64X)"), _T("Start Data"), (WCHAR*)pID, llLength );
	}
	else
	{
		LOG_OPR_FMT(bSuccess,_T("%s( Partition:\"%s\", Size:0x%08I64X, CheckSum:0x%08X)"), _T("Start Data"),(WCHAR*)pID, llLength, dwCheckSum);
	}

    return bSuccess;
}

BOOL CBootModeOpr::MidstData( DWORD dwLength, 
							 LPBYTE lpDownLoadData,
                             DWORD dwTimeout /* = 1000  */, 
							 DWORD dwTotal /*= 0*/)
{
	BM_PACKAGE bp;
	bp.header.type = BSL_CMD_MIDST_DATA;
	bp.header.len = (unsigned short)dwLength;
	bp.data = lpDownLoadData;

	BOOL bRetried = FALSE;

	Log( _T("Download Start"));

MidstData_Retry:
    //m_pChannel->Clear();
    BOOL bState = SendData( &bp,dwTimeout );  

	if( !bState && !bRetried && m_dwLastErrorCode == BSL_REP_VERIFY_ERROR )
	{
		// log the current failed information	
		LOG_OPR_FMT(bState,_T("Download (0x%08X)"),dwTotal);   

		bRetried = TRUE;
		// Clear channel dirty data and prepare for repeat reading.
		Log( _T("Clear channel buffer and retry MidstData."));
		m_pChannel->Clear();
		goto MidstData_Retry;
	}     

    if( m_dwLastErrorCode == BSL_REP_DOWN_DEST_ERROR )
    {
        Log( _T("Download:Download error in previous command") );
    }
    else
    {	
		LOG_OPR_FMT(bState,_T("Download (0x%08X)"),dwTotal);  
    }
    
    return bState;
}

BOOL CBootModeOpr::MidstDataFast( DWORD dwLength, LPBYTE lpSendData,
					 DWORD dwTimeout/* = 1000*/, unsigned __int64 llTotal /*= 0*/  )
{
	if( m_dwLastErrorCode == BSL_USER_CANCEL || m_pChannel == NULL )
	{
		return FALSE;
	}

	//Save the last send packet type
	m_iLastSentPktType = BSL_CMD_MIDST_DATA;   
	
	if( m_dwLastErrorCode == BSL_REP_DOWN_DEST_ERROR || m_dwLastErrorCode == BSL_USER_CANCEL)
	{
		return FALSE;
	}

	BOOL bRetried = FALSE;

MidstData_Retry:
	
	BOOL bState  = SendData(lpSendData,dwLength,TRUE,dwTimeout);

	if( !bState && !bRetried && m_dwLastErrorCode == BSL_REP_VERIFY_ERROR )
	{
		// log the current failed information	
		LOG_OPR_FMT(bState,_T("Download (0x%08I64X)"),llTotal);   

		bRetried = TRUE;
		// Clear channel dirty data and prepare for repeat reading.
		Log( _T("Clear channel buffer and retry MidstData."));
		m_pChannel->Clear();
		goto MidstData_Retry;
	}     

	if( m_dwLastErrorCode == BSL_REP_DOWN_DEST_ERROR )
	{
		Log( _T("Download:Download error in previous command") );
	}
	else
	{	
		LOG_OPR_FMT(bState,_T("Download (0x%08I64X)"),llTotal);  
	}

	return bState;
}

BOOL CBootModeOpr::Repartition( DWORD dwTimeout )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_REPARTITION , dwTimeout );
    LOG_OPR( _T("Repartition"), bSuccess );

    return bSuccess;
}
BOOL   CBootModeOpr::SendExtTable( LPBYTE pExtTable, DWORD dwSize, DWORD dwTimeout)
{
	BM_PACKAGE bp;
	bp.header.type = (unsigned short)BSL_CMD_EXTTABLE;
	bp.header.len = (unsigned short)dwSize;
	bp.data = pExtTable;
    
    BOOL bSuccess =  SendData( &bp, dwTimeout );
	LOG_OPR( _T("SendExtTable"), bSuccess );
    
    return bSuccess;
}
BOOL CBootModeOpr::Repartition( LPBYTE pPartitionInfo, DWORD dwSize, DWORD dwTimeout )
{
	BM_PACKAGE bp;
	bp.header.type = (unsigned short)BSL_CMD_REPARTITION;
	bp.header.len = (unsigned short)dwSize;
	bp.data = pPartitionInfo;
    
    BOOL bSuccess =  SendData( &bp, dwTimeout );
	LOG_OPR( _T("Repartition"), bSuccess );
    
    return bSuccess;
}
BOOL   CBootModeOpr::EnableSecureBoot( DWORD dwTimeout /*= 1000*/ )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_ENABLE_SECUREBOOT , dwTimeout );
    LOG_OPR( _T("EnableSecureBoot"), bSuccess );
    return bSuccess;
}
#ifdef NOT_USE_DIAGCHAN
BOOL CBootModeOpr::EnableCheckBaudCrc(BOOL bEnable)
{
	m_bmPkg.EnableCheckBaudCrc(bEnable);
	return TRUE;
}

int CBootModeOpr::GetCrcType()
{
	return m_bmPkg.GetCheckBaudCrcType();
}
 BOOL   CBootModeOpr::IsNeed2ndEnumPort()
 {
    return m_bmPkg.IsNeed2ndEnumPort();
 }
BOOL CBootModeOpr::GetCheckBaudCrcType()
{
	int  nCRC =	m_bmPkg.GetCheckBaudCrcType();
	::EnterCriticalSection( &m_csRecvBbuffer );
	if( m_lpReceiveBuffer != NULL )
	{
		if( m_dwBufferSize >= 4)
		{
			*((DWORD*)m_lpReceiveBuffer) = (DWORD)nCRC;
			m_dwReceiveLen = 4;
		}
	}
	::LeaveCriticalSection( &m_csRecvBbuffer );

	return TRUE;

}

BOOL CBootModeOpr::SendData( BM_PACKAGE* lpPkg,DWORD dwTimeout )
{
	if( NULL == lpPkg || m_dwLastErrorCode == BSL_USER_CANCEL || m_pChannel == NULL )
	{
		return FALSE;
	}

	//Save the last send packet type
	m_iLastSentPktType = (pkt_type_s)lpPkg->header.type;   
	if( m_iLastSentPktType == BSL_CMD_MIDST_DATA || m_iLastSentPktType == BSL_CMD_END_DATA )
	{
		if( m_dwLastErrorCode == BSL_REP_DOWN_DEST_ERROR )
		{
			return FALSE;
		}
	}

	if(m_dwLastErrorCode == BSL_USER_CANCEL)
	{
		return FALSE;
	}

	int nSize = 0;
	m_bmPkg.Package(lpPkg,m_pOutData,&nSize,!m_tDAInfo.bDisableTransCode);

	return SendData(m_pOutData,nSize,TRUE,dwTimeout);

}

BOOL CBootModeOpr::SendHostData( LPBYTE lpData, int iDataSize, DWORD dwTimeout /* = 1000  */, BOOL bWaiting /*= TRUE*/ )
{
	if( m_dwLastErrorCode == BSL_USER_CANCEL || m_pChannel == NULL )
	{
		return FALSE;
	}
#ifndef CHAN_SYNC
	ResetEvent( m_hOprEvent );
#endif
	m_dwLastErrorCode = 0;
	m_bOperationSuccess = FALSE;
	Log( _T("Start to write host data.") );
	int n1 = 0, n2 = 0, n3 = 0;
	if (iDataSize>8)
	{
		n1 = (int)(m_pChannel->Write( lpData, 8 ));
		if(iDataSize > 20)
		{
			n2 = (int)(m_pChannel->Write( lpData + 8, 12 ));
			n3 = (int)(m_pChannel->Write( lpData + 20, iDataSize - 20 ));
		}
		else
		{
			n2 = (int)(m_pChannel->Write( lpData + 8, iDataSize - 8 ));
		}
	}
	else
	{
		n1 = (int)(m_pChannel->Write( lpData, iDataSize ));
	}

	if( (n1 + n2 + n3) != iDataSize )
	{
		m_dwLastErrorCode = BSL_UART_SEND_ERROR;
		Log( _T("Write channel fail.") );
		return FALSE;
	} 

	Log( _T("Write host data end.") );

	if (bWaiting)
	{
		WaitForSingleObject( m_hOprEvent, dwTimeout );   
	}
	else
	{
		Sleep(dwTimeout);
	}

 	
	if( m_bOperationSuccess && m_dwLastErrorCode == 0 )
	{
		m_dwLastErrorCode = BSL_PHONE_WAIT_INPUT_TIMEOUT;        
	}

	Log( _T("Read host ack end.") );

	return m_bOperationSuccess;
}


BOOL CBootModeOpr::SendData( LPBYTE lpData, 
                            int iDataSize, 
                            BOOL bCRC /* = TRUE */,
                            DWORD dwTimeout /* = 1000  */ )
{
	UNUSED_ALWAYS( bCRC );
	m_dwLastErrorCode = 0;
	m_bOperationSuccess = FALSE;
#ifndef CHAN_SYNC
    ResetEvent( m_hOprEvent );
#endif 

	BOOL bRetried = FALSE;

SendData_Retry:
	if( m_iLastSentPktType != BSL_CMD_CHECK_BAUD )
	{
		m_bmPkg.Clear();
	}

	Log( _T("Write start.") );
    if (SupportNotTransCode())
    {
        if( iDataSize != (int)(m_pChannel->Write( lpData, iDataSize )) )
        {
            m_dwLastErrorCode = BSL_UART_SEND_ERROR;
            Log( _T("Write channel fail.") );
            return FALSE;
        } 
    }
    else if( m_bSupportZLP && (iDataSize%64) == 0)  
	{	
		int nPart1 = (int)(m_pChannel->Write( lpData, iDataSize-1 ));
		int nPart2 = (int)(m_pChannel->Write( lpData+iDataSize-1,1));
		if( (nPart1 + nPart2) != iDataSize )
		{
			m_dwLastErrorCode = BSL_UART_SEND_ERROR;
			Log( _T("Write channel fail.") );
			return FALSE;
		} 
	}
	else
	{
		if( iDataSize != (int)(m_pChannel->Write( lpData, iDataSize )) )
		{
			m_dwLastErrorCode = BSL_UART_SEND_ERROR;
			Log( _T("Write channel fail.") );
			return FALSE;
		} 
	}
	Log( _T("Write end.") );


#ifdef CHAN_SYNC
	DWORD dwBegin = GetTickCount();	
	DWORD dwCur = GetTickCount();
	do 
	{   
		dwCur = GetTickCount();
		DWORD dwRead = m_pChannel->Read(m_RecvData,MAX_RECV_LEN,dwTimeout);

		m_bOperationSuccess = ProcessData( m_RecvData , (int)dwRead );

		if( m_bOperationSuccess || m_dwLastErrorCode != BSL_REP_INCOMPLETE_DATA )
		{
			break;
		}
	} while((dwCur-dwBegin)<dwTimeout && m_bWorking);
#else
	if (0 == dwTimeout)
	{
		return TRUE;
	}
    WaitForSingleObject( m_hOprEvent, dwTimeout );   
#endif
	Log( _T("Read end.") );
    
    if( m_iLastSentPktType == BSL_CMD_READ_FLASH )
    {
        if( !m_bOperationSuccess  && !bRetried )
        {
            ResetEvent( m_hOprEvent );
            m_dwLastErrorCode = 0;
            m_bOperationSuccess = FALSE;   

			bRetried  = TRUE;            
			goto SendData_Retry;

        }  
    }
    
    if( m_dwLastErrorCode == 0 )
    {
        m_dwLastErrorCode = BSL_PHONE_WAIT_INPUT_TIMEOUT;        
    }
    
    return m_bOperationSuccess;
}

BOOL CBootModeOpr::ProcessData( LPBYTE lpData, int iDataLen )
{
    //if( m_dwLastErrorCode != 0  && m_dwLastErrorCode != BSL_REP_INCOMPLETE_DATA)//have error occured.
    //     return FALSE;
	BOOL bReturnCode = FALSE;

    if(m_bHostProtocol)
    {
	    if(m_sHostProtocol.Decode_Response(lpData,iDataLen ) ) 
	    {
		    bReturnCode = TRUE;
	    }
#ifndef CHAN_SYNC
		m_pChannel->FreeMem( lpData );
#endif
        return bReturnCode;
    }

 	int nPacket = m_bmPkg.Append(lpData,iDataLen);
#ifndef CHAN_SYNC
	m_pChannel->FreeMem( lpData );
#endif

	PACKAGE_INFO &pi = m_bmPkg.GetError();

	if(m_iLastSentPktType == BSL_CMD_CHECK_BAUD && m_pLogFile != NULL)
	{
		CString strLog;
		strLog.Format(_T("Checkbaud append result: bCrcError(%d),bNoTail(%d),bInvalidLen(%d)"),pi.bCrcError,pi.bNoTail,pi.bInvalidLen);
		Log(strLog);
	}

    if( nPacket == 0 )
    {
        if(pi.bCrcError || pi.bInvalidLen)
			m_dwLastErrorCode = BSL_REP_DECODE_ERROR;
		else// if(pi.bNoTail)
			m_dwLastErrorCode = BSL_REP_INCOMPLETE_DATA;

        return FALSE;
    }

	LPBYTE lpDecodeData = (LPBYTE)m_bmPkg.GetPackages();  
    BM_HEADER * pHdr = (BM_HEADER *)lpDecodeData;
	int iDecodeDataLen = pHdr->len + sizeof(BM_HEADER);
    
    
    m_dwLastErrorCode = pHdr->type;
    
    //According the Response message type implement operation
    if(pHdr->type == BSL_REP_ACK)
    {
        bReturnCode = TRUE;
    }
    else if(pHdr->type == BSL_REP_VER)
    {
		m_bGotVerAck = TRUE;
        bReturnCode = TRUE;
    }    
	else if ( BSL_REP_INCOMPATIBLE_PARTITION == pHdr->type)
	{
		if(  NULL != lpDecodeData  )
		{
            if( iDecodeDataLen - PACHET_HDR_LEN  >= 4)
            {
                DWORD dwVersion = 0;
                memcpy( &dwVersion ,lpDecodeData + PACHET_HDR_LEN, 4 );
                if(dwVersion == 2)
                {
                    if( iDecodeDataLen - PACHET_HDR_LEN  <= sizeof(m_tDAInfo) )
                    {
                        memcpy( &m_tDAInfo ,lpDecodeData + PACHET_HDR_LEN, iDecodeDataLen - PACHET_HDR_LEN );
                        if(ohObject.IsNeedCheckOldMemory())
                        {
                            switch (m_tDAInfo.bIsOldMemory & ohObject.GetCheckOldMemoryKind())
                            {
                            case 1:
                                m_dwLastErrorCode = BSL_REP_OLD_MEMORY_REGISTER_CHANGE_ERROR;
                                break;
                            case 2:
                                m_dwLastErrorCode = BSL_REP_OLD_MEMORY_EMMC_HAS_FLAG_ERROR;
                                break;
                            case 3:
                                m_dwLastErrorCode = BSL_REP_OLD_MEMORY_REGISTER_EMMC_ERROR;
                                break;
                            case 4:
                                m_dwLastErrorCode = BSL_REP_OLD_MEMORY_RPMB_WRITTEN_ERROR;
                                break;
                            case 5:
                                m_dwLastErrorCode = BSL_REP_OLD_MEMORY_REGISTER_RPMB_ERROR;
                                break;
                            case 6:
                                m_dwLastErrorCode = BSL_REP_OLD_MEMORY_EMMC_RPMB_ERROR;
                                break;
                            case 7:
                                m_dwLastErrorCode = BSL_REP_OLD_MEMORY_REGISTER_EMMC_RPMB_ERROR;
                                break;
                            default:
                                break;
                            }
                        }  
                    }
                }
                else if(dwVersion == 1)
                {
                    if(ohObject.IsNeedCheckOldMemory())
                    {
                        m_dwLastErrorCode = BSL_REP_OLD_MEMORY_UNSUPPORT_ERROR;
                        return FALSE;
                    }
                    if( iDecodeDataLen - PACHET_HDR_LEN  <= sizeof(m_tDAInfo) )
                    {
                        memcpy( &m_tDAInfo ,lpDecodeData + PACHET_HDR_LEN, 8);
                    }
                }
            }
			if (!ohObject.SupportUnTransCode())
			{
				m_tDAInfo.bDisableTransCode = FALSE;
			}
			m_bmPkg.DisableTransCode(m_tDAInfo.bDisableTransCode);
		}
	}
    else if(pHdr->type == BSL_REP_READ_FLASH)
    {
        bReturnCode = TRUE;
        if( NULL != lpDecodeData  &&  
			(m_iLastSentPktType == BSL_CMD_READ_FLASH || 
			m_iLastSentPktType == BSL_CMD_READ_MIDST) )
        {
            // The buffer is read from module
            if( iDecodeDataLen <= PACHET_HDR_LEN  ||
                m_dwLastPktSize != iDecodeDataLen - PACHET_HDR_LEN)
            {
                // Data read error
                m_dwLastErrorCode = BSL_REP_OPERATION_FAILED;
                return FALSE;
            }
            ::EnterCriticalSection( &m_csRecvBbuffer );
            if( m_lpReceiveBuffer != NULL )
            {
                if( m_dwReceiveLen + iDecodeDataLen - PACHET_HDR_LEN - m_uiReadOffset <= m_dwBufferSize  )
                {
                    memcpy( m_lpReceiveBuffer + m_dwReceiveLen,
                        lpDecodeData + PACHET_HDR_LEN + m_uiReadOffset,
                        iDecodeDataLen - PACHET_HDR_LEN - m_uiReadOffset );
                    m_dwReceiveLen += ( iDecodeDataLen - PACHET_HDR_LEN - m_uiReadOffset );
                    m_uiReadOffset = 0;
                }
            }
            ::LeaveCriticalSection( &m_csRecvBbuffer );
        }		

	}
    else if (pHdr->type == BSL_REP_DOWN_SIZE_ERROR)
	{
        if(  NULL != lpDecodeData  )
        {
            ::EnterCriticalSection( &m_csRecvBbuffer );
            if( m_lpReceiveBuffer != NULL )
            {
                m_dwReceiveLen = 0; 
                if( iDecodeDataLen - PACHET_HDR_LEN  <= m_dwBufferSize )
                {
                    memcpy( m_lpReceiveBuffer ,lpDecodeData + PACHET_HDR_LEN, m_dwBufferSize );
                    m_dwReceiveLen = iDecodeDataLen - PACHET_HDR_LEN;
                }
            }
            ::LeaveCriticalSection( &m_csRecvBbuffer );
        }
	}
    else if( 	pHdr->type == BSL_REP_READ_CHIP_TYPE	|| 
				pHdr->type == BSL_REP_READ_NVITEM		||
				pHdr->type == BSL_REP_READ_FLASH_TYPE	||
				pHdr->type == BSL_REP_READ_SECTOR_SIZE	||
				pHdr->type == BSL_CHIPID_NOT_MATCH		||
				pHdr->type == BSL_REP_READ_FLASH_INFO	||
				pHdr->type == BSL_REP_READ_FLASH_UID	||
				pHdr->type == BSL_REP_READ_SOFTSIM_EID	||
                pHdr->type == BSL_REP_READ_CHIP_UID		||
				pHdr->type == BSL_REP_READ_REFINFO		||
				pHdr->type == BSL_REP_RF_TRANSCEIVER_TYPE||
                pHdr->type == BSL_REP_READ_NAND_BLOCK_INFO
             )
    {       
		if(pHdr->type == BSL_CHIPID_NOT_MATCH)
		{
			bReturnCode = FALSE; 
		}
		else
		{
			bReturnCode = TRUE; 
		}

        if(  NULL != lpDecodeData  )
        {
            ::EnterCriticalSection( &m_csRecvBbuffer );
            if( m_lpReceiveBuffer != NULL )
            {
                m_dwReceiveLen = 0; 
                if( iDecodeDataLen - PACHET_HDR_LEN  <= m_dwBufferSize )
                {
                    memcpy( m_lpReceiveBuffer ,lpDecodeData + PACHET_HDR_LEN, m_dwBufferSize );
                    m_dwReceiveLen = iDecodeDataLen - PACHET_HDR_LEN;
                }
            }
            ::LeaveCriticalSection( &m_csRecvBbuffer );
        }
    }
   
    return bReturnCode;    
}
#else 
BOOL CBootModeOpr::SendData( BM_PACKAGE* lpPkg,DWORD dwTimeout )
{
	if( NULL == lpPkg || m_dwLastErrorCode == BSL_USER_CANCEL || m_pChannel == NULL )
	{
		return FALSE;
	}

    //Save the last send packet type
    m_iLastSentPktType = (pkt_type_s)lpPkg->header.type;   
    if( m_iLastSentPktType == BSL_CMD_MIDST_DATA || m_iLastSentPktType == BSL_CMD_END_DATA )
    {
        if( m_dwLastErrorCode == BSL_REP_DOWN_DEST_ERROR )
        {
            return FALSE;
        }
    }
    
	PRT_WRITE_T pwt;
	pwt.action = PRT_WRITE_normal;
	pwt.nCond = -1;
	pwt.lpProtocolData = lpPkg;

	if(lpPkg->header.type == BSL_CMD_CHECK_BAUD)
	{
		pwt.action = PRT_WRITE_no_respond;
	}

	//::EnterCriticalSection( &m_csLastError);
	if(m_dwLastErrorCode == BSL_USER_CANCEL)
	{
		//::LeaveCriticalSection( &m_csLastError);
		return FALSE;
	}

	if( !m_pChannel->Write( &pwt,1 ) )
	{		
		m_dwLastErrorCode = BSL_UART_SEND_ERROR;
        Log( _T("Write channel fail.") );
		//::LeaveCriticalSection( &m_csLastError);
        return FALSE;
	}

	PRT_READ_T prt;
	prt.lpCond = NULL;
	prt.nCond = pwt.nCond;
	PRT_BUFF* lpBuff;
	prt.lppBuff = &lpBuff;
	if( !m_pChannel->Read( &prt,1,dwTimeout ) )
	{
		PACKAGE_INFO info;
		m_pChannel->GetProperty( 0,PPI_GetError,&info );
		if( info.bCrcError || info.bInvalidLen )
		{
			m_dwLastErrorCode = BSL_REP_DECODE_ERROR;
		}
		else if( info.bNoTail )
		{
			m_dwLastErrorCode = BSL_REP_INCOMPLETE_DATA;
		}
		else
		{
			m_dwLastErrorCode = BSL_PHONE_WAIT_INPUT_TIMEOUT;
		}			
		//::LeaveCriticalSection( &m_csLastError);
		return FALSE;
	}
	
	P_Packet_Header tmpPktHeader = (P_Packet_Header)lpBuff->lpData;    
    m_dwLastErrorCode = tmpPktHeader->PacketType;

	//::LeaveCriticalSection( &m_csLastError);

	if( m_dwLastErrorCode == BSL_USER_CANCEL )
	{
		return FALSE;
	}

    m_bOperationSuccess = FALSE;
    //According the Response message type implement operation
    if(tmpPktHeader->PacketType == BSL_REP_ACK)
    {
        m_bOperationSuccess = TRUE;
    }
    else if(tmpPktHeader->PacketType == BSL_REP_VER)
    {
        m_bOperationSuccess = TRUE;
    }    
    else if(tmpPktHeader->PacketType == BSL_REP_READ_FLASH)
    {
        m_bOperationSuccess = TRUE;
        if( NULL != lpBuff->lpData  &&  
			(m_iLastSentPktType == BSL_CMD_READ_FLASH || 
			 m_iLastSentPktType == BSL_CMD_READ_MIDST) )
        {
            // The buffer is read from module
            if( lpBuff->size <= PACHET_HDR_LEN  ||
                m_dwLastPktSize != lpBuff->size - PACHET_HDR_LEN)
            {
                // Data read error
				lpBuff->free_prt( lpBuff );
                m_dwLastErrorCode = BSL_REP_OPERATION_FAILED;
                return FALSE;
            }
            if( m_lpReceiveBuffer != NULL )
            {
                if( (m_dwReceiveLen + lpBuff->size - PACHET_HDR_LEN - m_uiReadOffset) <= m_dwBufferSize  )
                {
                    memcpy( m_lpReceiveBuffer + m_dwReceiveLen,
                        lpBuff->lpData + PACHET_HDR_LEN + m_uiReadOffset,
                        lpBuff->size - PACHET_HDR_LEN - m_uiReadOffset );
                    m_dwReceiveLen += ( lpBuff->size - PACHET_HDR_LEN - m_uiReadOffset );
                    m_uiReadOffset = 0;
                }
            }
        }

    } else if( 
		tmpPktHeader->PacketType == BSL_REP_READ_CHIP_TYPE		|| 
        tmpPktHeader->PacketType == BSL_REP_READ_NVITEM			||
        tmpPktHeader->PacketType == BSL_REP_READ_FLASH_TYPE		||
		tmpPktHeader->PacketType == BSL_REP_READ_SECTOR_SIZE	||
		tmpPktHeader->PacketType == BSL_CHIPID_NOT_MATCH		||
		tmpPktHeader->PacketType == BSL_REP_READ_FLASH_INFO		||
		tmpPktHeader->PacketType == BSL_REP_READ_FLASH_UID		||
		tmpPktHeader->PacketType == BSL_REP_READ_SOFTSIM_EID    ||
        tmpPktHeader->PacketType == BSL_REP_READ_CHIP_UID		||
		tmpPktHeader->PacketType == BSL_REP_READ_REFINFO		||
		tmpPktHeader->PacketType == BSL_REP_RF_TRANSCEIVER_TYPE
		)
    { 
		if(tmpPktHeader->PacketType == BSL_CHIPID_NOT_MATCH)
		{
			m_bOperationSuccess = FALSE; 
		}
		else
		{
			m_bOperationSuccess = TRUE; 
		}

        if(  NULL != lpBuff->lpData  )
        {
            ::EnterCriticalSection( &m_csRecvBbuffer );
            if( m_lpReceiveBuffer != NULL )
            {
                m_dwReceiveLen = 0; 
                if( lpBuff->size - PACHET_HDR_LEN  == m_dwBufferSize )
                {
                    memcpy( m_lpReceiveBuffer ,lpBuff->lpData + PACHET_HDR_LEN, m_dwBufferSize );
                    m_dwReceiveLen = m_dwBufferSize;
                }
            }
            ::LeaveCriticalSection( &m_csRecvBbuffer );
        }
    }

    lpBuff->free_prt( lpBuff );   	
   
    return m_bOperationSuccess;
}
#endif

BOOL CBootModeOpr::SendCommandData( pkt_type_s nCmdType, DWORD dwTimeout /* = 1000  */ )
{
    /*
	BYTE btSendData[PACHET_HDR_LEN];
    
    memset(&btSendData, 0, PACHET_HDR_LEN);    
    *(short *)&btSendData[PKT_TYPE_POS] = (short)nCmdType;
	return SendData( btSendData, PACHET_HDR_LEN , TRUE, dwTimeout );    
	*/
	BM_PACKAGE bp;
	bp.header.type = (unsigned short)nCmdType;
	bp.header.len = 0;
	bp.data = NULL;

	return SendData( &bp,dwTimeout );
}

BOOL CBootModeOpr::SendPacketData( pkt_type_s nCmdType, 
                                  const int /*nSendDataLen*/,
                                  LPBYTE lpPacketData,
                                  UINT uiDataLen,
                                  DWORD dwTimeout /* = 1000  */ )
{
	if( m_bBigEndian && NULL != lpPacketData )
	{
		for( UINT i = 0; i < uiDataLen / sizeof( DWORD ); i++ )
		{
            DWORD dwSoruceValue, dwDestValue;
			
            dwSoruceValue =  *(DWORD *)&lpPacketData[ i * sizeof( DWORD) ];    
            dwDestValue   = 0;
            CONVERT_INT( dwSoruceValue, dwDestValue);
            *(DWORD *)&lpPacketData[ i * sizeof( DWORD) ] = dwDestValue;
        }
    }

	BM_PACKAGE bp;
	bp.header.type = (unsigned short)nCmdType;
	bp.header.len = (unsigned short)uiDataLen;
	bp.data = lpPacketData;
    
    return SendData( &bp, dwTimeout );
}

BOOL CBootModeOpr::AllocRecvBuffer( DWORD dwSize )
{
    ::EnterCriticalSection( &m_csRecvBbuffer );
    if( m_lpReceiveBuffer != NULL )
    {
        if( m_dwBufferSize == dwSize )
        {
            ZeroMemory( m_lpReceiveBuffer, m_dwBufferSize );
            m_dwReceiveLen = 0;
        }
        else
        {
            delete[] m_lpReceiveBuffer;
            m_lpReceiveBuffer = NULL;
            m_dwReceiveLen = 0;
            m_dwBufferSize = 0;            
        }
    }
    if( m_lpReceiveBuffer == NULL )
    {
        m_lpReceiveBuffer = new BYTE [ dwSize ];
        if( m_lpReceiveBuffer == NULL )
            return FALSE;
        m_dwReceiveLen = 0;
        m_dwBufferSize = dwSize;
    }
    
    ::LeaveCriticalSection( &m_csRecvBbuffer );
    
    return TRUE;
}

LPBYTE CBootModeOpr::GetRecvBuffer()
{
    return m_lpReceiveBuffer;
}

DWORD CBootModeOpr::GetRecvBufferSize()
{
    return m_dwReceiveLen;
}

void CBootModeOpr::FreeRecvBuffer()
{
    ::EnterCriticalSection( &m_csRecvBbuffer );
    if( m_lpReceiveBuffer != NULL )
    {
        delete[] m_lpReceiveBuffer;
        m_lpReceiveBuffer = NULL;
        m_dwReceiveLen = 0;    
        m_dwBufferSize = 0;
    }
    ::LeaveCriticalSection( &m_csRecvBbuffer );

}

void CBootModeOpr::InitComLog()
{
	CString strLogPath = ohObject.GetLogPath();
	DWORD dwLevel = ohObject.GetChannelLogLevel();
	if(strLogPath.IsEmpty() || 0 == dwLevel)
	{
		return;
	}
	_TCHAR szLogFileName[ MAX_PATH ] = {0};
	CreateDeepDirectory(strLogPath.operator LPCTSTR());

	SYSTEMTIME  currentTime ;
	GetLocalTime( &currentTime);
	_stprintf( szLogFileName, _T("%s\\%s%s"), strLogPath.operator LPCTSTR(), _T("UartComm_"), m_strLogSuffix.operator LPCTSTR());
	DWORD dwType = ohObject.GetChannelLogType();
	m_pChannel->InitLog(szLogFileName,dwType,dwLevel);
}

BOOL CBootModeOpr::ConnectChannel( LPBYTE lpOpenParam )
{
#ifdef NOT_USE_DIAGCHAN
#ifndef CHAN_SYNC
    m_pChannel->SetReceiver( WM_RCV_CHANNEL_DATA, TRUE, (LPVOID)m_dwRecvThreadID );
#endif
#endif
	DWORD dwPort = *((DWORD*)lpOpenParam);
	DWORD dwBaud = *((DWORD*)(lpOpenParam+sizeof(DWORD)));
	m_dwBaud = dwBaud;

	CHANNEL_ATTRIBUTE ca;
	ca.ChannelType = CHANNEL_TYPE_COM;
	ca.Com.dwPortNum = dwPort;
	ca.Com.dwBaudRate = ohObject.GetRomCodeBaudRate();//ohObject.GetDefaultBaudrate();
         
    Log( _T("Connect Channel +++"));
    BOOL bOK  = m_pChannel->Open( &ca );
    Log( _T("Connect Channel ---"));
    if( !bOK )
    {
        Log( _T("Open Channel fail.") );
        return FALSE;
    }    
    return TRUE;    
}

void  CBootModeOpr::CloseDevBeforeRemove()
{
	if (!ohObject.CloseDevBeforeRemove())
	{
		return;
	}
	Uninitialize();
}

BOOL CBootModeOpr::DisconnectChannel()
{
	::EnterCriticalSection( &m_csClosePort );
    if( m_pChannel == NULL )
	{
		::LeaveCriticalSection( &m_csClosePort );
        return FALSE;
	}
		   
    if (m_bOpened)
    {
        m_bOpened = FALSE;
        Log( _T("Disconnect Channel +++"));
        m_pChannel->Close();
        Log( _T("Disconnect Channel ---"));
    }
    ::LeaveCriticalSection( &m_csClosePort );
    return TRUE;
}

#ifdef NOT_USE_DIAGCHAN

BOOL CBootModeOpr::CreateRecvThread()
{
	m_tDAInfo.Clear();
	m_bmPkg.DisableTransCode(FALSE);
	m_bmPkg.Clear();
	
    m_hRecvThreadState = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hOprEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
    Log( _T("Create receive thread"));
    m_hRecvThread = (HANDLE)_beginthreadex(NULL,
								0,
                                (PBEGINTHREADEX_THREADFUNC)GetRcvDataThread,
                                this,
                                NULL,
                                (PBEGINTHREADEX_THREADID)&m_dwRecvThreadID);
    if(m_hRecvThread == NULL)
    {
        return FALSE;
    }
    
    //Wait For UARTCM Thread be Running
    DWORD bWaitCode = WaitForSingleObject(m_hRecvThreadState,5000);
    if( bWaitCode != WAIT_OBJECT_0)
    {			
        return FALSE;
    }
    else
    {
        //Reset the UARTCM thread state event, the next signal
        //is thread ending
        ResetEvent(m_hRecvThreadState);
    }    
    return TRUE;
}

void CBootModeOpr::DestroyRecvThread() 
{
    if( m_hRecvThread == NULL )
        return;
    
    Log( _T("Destroy receive thread"));
    
    ::PostThreadMessage (m_dwRecvThreadID, WM_QUIT, 0, 0);

    WaitForSingleObject( m_hRecvThreadState, INFINITE );
    ResetEvent(m_hRecvThreadState);

    if (m_hRecvThread)
    {
        CloseHandle(m_hRecvThread);
        m_hRecvThread = NULL;
    }
    
    if (m_hRecvThreadState)
    {
        CloseHandle(m_hRecvThreadState);
        m_hRecvThreadState = NULL;
    }   

    if (m_hOprEvent)
    {
        CloseHandle( m_hOprEvent );
        m_hOprEvent = NULL;
    }

    m_dwRecvThreadID = 0;    
}


DWORD  WINAPI CBootModeOpr::GetRcvDataThread( LPVOID lpParam )
{
    CBootModeOpr * pThis = ( CBootModeOpr * )lpParam;
    return pThis->RcvDataThreadProc();
}

DWORD CBootModeOpr::RcvDataThreadProc()
{
    MSG msg;    
    // Set up the message queue
    PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

	SetEvent(m_hRecvThreadState);

	Log(_T("Create receive thread success."));
    
    // Wait for the UART data
    while ( GetMessage(&msg, NULL, 0, 0) )
    {
        switch (msg.message )
        {
        case WM_RCV_CHANNEL_DATA:			
            m_bOperationSuccess = ProcessData( ( LPBYTE )msg.wParam , (int)msg.lParam );
            if( m_bOperationSuccess || m_dwLastErrorCode != BSL_REP_INCOMPLETE_DATA )
            {
                SetEvent( m_hOprEvent );
            }
            break;
        case WM_CHANNEL_CLOSE:
            m_bOperationSuccess = FALSE;
            break;
        default:
            break;
       }
    }
    
    SetEvent(m_hRecvThreadState);
    
    return 0;
}
#endif //NOT_USE_DIAGCHAN

DWORD CBootModeOpr::GetLastErrorCode()
{
    return m_dwLastErrorCode;
}

void CBootModeOpr::SetLastErrorCode(DWORD dwErrorCode)
{
    m_dwLastErrorCode = dwErrorCode;
}

void CBootModeOpr::GetLastErrorDescription(
                                           DWORD dwErrorCode,
                                           LPTSTR lpszErrorDescription, 
                                           int nSize )
{
    if( lpszErrorDescription == NULL || nSize == 0 )
        return;
    
    _TCHAR szErrorIniFile[MAX_PATH];
    
    GetModuleFilePath( NULL, szErrorIniFile );
    _tcscat( szErrorIniFile,  _T("\\BMError.ini") );
    
    _TCHAR szKey[ 10 ];
    _itot( dwErrorCode, szKey, 10 );
    
    GetPrivateProfileString(  _T("ErrorDescription"), szKey,
        _T("Unknown Error"), lpszErrorDescription, 
        nSize, szErrorIniFile );  
    
    return;
}

void CBootModeOpr::InitLogFileSuffix( DWORD dwCookie )
{
	SYSTEMTIME  currentTime ;
	GetLocalTime( &currentTime);
	DWORD dwPID = GetCurrentProcessId();
	//COM4_28164_2019-01-16_13-19-33-590.log
	m_strLogSuffix.Format(_T("COM%d_%d_%d-%02d-%02d_%02d-%02d-%02d-%03d.log"),
		dwCookie,
		dwPID,
		currentTime.wYear,
		currentTime.wMonth,
		currentTime.wDay,
		currentTime.wHour,
		currentTime.wMinute,
		currentTime.wSecond,
		currentTime.wMilliseconds
		);
}

BOOL CBootModeOpr::OpenLogFile( DWORD dwCookie )
{
    CloseLogFile();

	int nLogFlag = ohObject.GetLogFlag();
	if(0 == nLogFlag)
	{
		m_pLogFile = NULL;
		return TRUE;
	}

#ifdef HIGH_PRECIOUS_TIME
	usTimeCounter(true);
#endif
	
    m_dwCookie = dwCookie;
	_TCHAR szLogFileName[ MAX_PATH ] = {0};
	CString strLogPath = ohObject.GetLogPath();
	SYSTEMTIME  currentTime ;
	if (strLogPath.IsEmpty())
	{
		GetModuleFilePath( NULL, szLogFileName );
		_tcscat( szLogFileName, _T("\\Log"));
		CreateDirectory(szLogFileName,NULL);
		GetLocalTime( &currentTime);
		_stprintf( szLogFileName, _T("%s\\%sCOM_%d_%04d-%02d-%02d.log"), 
			szLogFileName, LOG_FILENAME_PREFIX, 
			dwCookie,
			currentTime.wYear,
			currentTime.wMonth,
			currentTime.wDay );
	}
	else
	{
		CreateDeepDirectory(strLogPath.operator LPCTSTR());
		GetLocalTime( &currentTime);
		_stprintf( szLogFileName, _T("%s\\%s%s"), strLogPath.operator LPCTSTR(), LOG_FILENAME_PREFIX, m_strLogSuffix.operator LPCTSTR());
	}

    m_pLogFile = _tfopen( szLogFileName, _T("a+") );  
    if( m_pLogFile != NULL )
    {
        GetModuleFileName( g_theApp.m_hInstance, 
                        szLogFileName, MAX_PATH );
        _ftprintf( m_pLogFile, _T("===============================%s\n"),             
            szLogFileName );   
        return TRUE;
    }
    return FALSE;
}

BOOL CBootModeOpr::OpenCrcFile( DWORD dwCookie )
{
    CloseCrcFile();

    if(!ohObject.IsGenCrcCfgFile())
    {
        m_pCrcFile = NULL;
        return TRUE;
    }

#ifdef HIGH_PRECIOUS_TIME
    usTimeCounter(true);
#endif

    m_dwCookie = dwCookie;
    _TCHAR szCrcFileName[ MAX_PATH ];
    GetModuleFilePath( NULL, szCrcFileName );

    _tcscat( szCrcFileName, _T("\\CrcFile"));
    CreateDirectory(szCrcFileName, NULL);

    SYSTEMTIME  currentTime ;
    GetLocalTime( &currentTime);
    _stprintf( szCrcFileName, _T("%s\\%sCOM_%d_%04d-%02d-%02d.ini"), 
        szCrcFileName, CRC_FILENAME_PREFIX, 
        dwCookie,
        currentTime.wYear,
        currentTime.wMonth,
        currentTime.wDay );

    m_pCrcFile = _tfopen( szCrcFileName, _T("a+") );  
    if( m_pCrcFile != NULL )
    {  
        m_strCrcFileName = szCrcFileName;
        CloseCrcFile();
        return TRUE;
    }
    return FALSE;
}

void CBootModeOpr::CloseLogFile()
{
    if( m_pLogFile != NULL )
    {
        fflush( m_pLogFile);
        fclose( m_pLogFile );
        m_pLogFile = NULL;
    }    
}

void CBootModeOpr::CloseCrcFile()
{
    if( m_pCrcFile != NULL )
    {
        fflush( m_pCrcFile);
        fclose( m_pCrcFile );
        m_pCrcFile = NULL;
    }    
}

void CBootModeOpr::Log( LPCTSTR lpszLog )
{
    if( m_pLogFile != NULL )
    {
        SYSTEMTIME  currentTime;
        GetLocalTime( &currentTime);
#ifdef HIGH_PRECIOUS_TIME
        _ftprintf( m_pLogFile, _T("%02d:%02d:%02d:%03d(%03.6f)  %s\n"),  
                            currentTime.wHour,
                            currentTime.wMinute,
                            currentTime.wSecond,
							currentTime.wMilliseconds,
							usTimeCounter(false),
                            lpszLog );
#else
		_ftprintf( m_pLogFile, _T("%02d:%02d:%02d:%03d  %s\n"),  
			currentTime.wHour,
			currentTime.wMinute,
			currentTime.wSecond,
			currentTime.wMilliseconds,
			lpszLog );
#endif  
        fflush(m_pLogFile);
    }
}

void CBootModeOpr::LogOpr( LPCTSTR lpszOperation, BOOL bSuccess )
{
    if( m_pLogFile != NULL)
    {
        _TCHAR szLog[ MAX_LOG_LEN ];
        _tcscpy( szLog, lpszOperation );
        if( bSuccess )
        {
            _tcscat( szLog, _T(": Success.") );
            Log( szLog );
        }
        else
        {
            _TCHAR szDescription[ MAX_LOG_LEN ];
            DWORD dwErrorCode = m_dwLastErrorCode;
            GetLastErrorDescription( m_dwLastErrorCode, szDescription, MAX_LOG_LEN );
            _stprintf( szLog, _T("%s: Fail. [%d:%s]"), szLog, dwErrorCode, szDescription );
            Log( szLog );
        }
    }    
}

void CBootModeOpr::LogOprFmt( BOOL bSuccess,LPCTSTR lpszFmt, ... )
{
	if( m_pLogFile != NULL)
	{
		_TCHAR szLog[ MAX_LOG_LEN ];

		va_list  args;
		va_start(args, lpszFmt);
		_vsntprintf(szLog, sizeof(szLog), lpszFmt, args);
		va_end(args);

		LogOpr( szLog, bSuccess );
	}
}

BOOL CBootModeOpr::SetCheckSumCfg(LPCTSTR lpszFileId, int nCheckSum)
{
    CString strTemp;
    strTemp.Format( _T("0x%x"), nCheckSum);
    return WritePrivateProfileString(_T("Crc"), lpszFileId, strTemp.GetBuffer(0), m_strCrcFileName.GetBuffer(0)); 
}

DWORD CBootModeOpr::GetCheckSumCfg(LPCTSTR lpszFileId, LPCTSTR lpszCrcFile)
{
    return (DWORD)GetPrivateProfileInt(_T("Crc"), lpszFileId, 0, lpszCrcFile); 
}

void CBootModeOpr::SetIsCheckCrc( BOOL bCheckCrc  )
{
    m_bCheckCrc = bCheckCrc;
#ifdef NOT_USE_DIAGCHAN
	m_bmPkg.SetCrc(bCheckCrc);
#else
	m_pChannel->SetProperty( 0,PPI_BM_CRC_Type,(void*)m_bCheckCrc );
#endif
}

void CBootModeOpr::StopOpr()
{
    DisconnectChannel();
	::EnterCriticalSection(&m_csLastError);
    m_dwLastErrorCode = BSL_USER_CANCEL;
	::LeaveCriticalSection(&m_csLastError);
#ifdef NOT_USE_DIAGCHAN
    SetEvent( m_hOprEvent );
#endif

	m_bGotVerAck = FALSE;
	m_bHostProtocol = FALSE;
}

ICommChannel* CBootModeOpr::GetChannelPtr()
{
#ifdef NOT_USE_DIAGCHAN
	return m_pChannel;
#else
	ICommChannel* lpChannel;
    m_pChannel->GetLowerChannel( &lpChannel,false );
	return lpChannel;
#endif
}

void CBootModeOpr::SetChannel( ICommChannel* pChannel )
{
#ifdef NOT_USE_DIAGCHAN
	m_pChannel = (ICommChannel*)pChannel;
#else
	m_pChannel->SetLowerChannel((ICommChannel*)pChannel,false);
#endif    
}

void  CBootModeOpr::SetStartDataInfo(BOOL bHasCheckSum)
{
	m_bHasCheckSum  = bHasCheckSum;
}

DWORD CBootModeOpr::DoNVCheckSum(LPBYTE pDataBuf,DWORD dwSize)
{
	//////////////////////////////////////////////////////////////////////////
	//make crc and add to the first two byte of data
	//notice: pDataBuf is bigendian, and do CRC not including the first WORD
	WORD wUpdateCrc = crc16(0,pDataBuf+sizeof(WORD),dwSize-sizeof(WORD));
	*pDataBuf = HIBYTE(wUpdateCrc);
	*(pDataBuf+1) = LOBYTE(wUpdateCrc);
					
	
	DWORD dwSum = 0;
	for(UINT i= 0; i< dwSize; i++)
	{
		dwSum += (BYTE)(*(pDataBuf+i));
	}

	return dwSum;
}

BOOL CBootModeOpr::ReadSectorSize( DWORD dwTimeout/* = 1000*/ )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_READ_SECTOR_SIZE, dwTimeout );
    LOG_OPR( _T("Read sector size"),bSuccess );
    return bSuccess;
}

BOOL CBootModeOpr::DoBeforeCheckBaud()
{
#ifdef NOT_USE_DIAGCHAN
#else
	int nPacCap = 1;
	m_pChannel->SetProperty(0,PPI_INTERNAL_PACKAGE_CAPACITY,(LPCVOID)nPacCap);
#endif
	return TRUE;

}

BOOL CBootModeOpr::DoAfterChackBaud()
{
#ifdef NOT_USE_DIAGCHAN
#else
	int nPacCap = 0;
	m_pChannel->SetProperty(0,PPI_INTERNAL_PACKAGE_CAPACITY,(LPCVOID)nPacCap);
#endif
	return TRUE;
}

BOOL CBootModeOpr::KeepCharge( DWORD dwTimeout /*= 1000*/ )
{
	BOOL bSuccess = SendCommandData( BSL_CMD_KEEP_CHARGE, dwTimeout );
    LOG_OPR( _T("Keep charge"),bSuccess );
    return bSuccess;
}

BOOL CBootModeOpr::DdrCheck( DWORD dwTimeout /*= 1000*/ )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_DDR_CHECK, dwTimeout );
    LOG_OPR( _T("DDR Check"),bSuccess );
    return bSuccess;
}

BOOL CBootModeOpr::SelfRefresh( DWORD dwTimeout /*= 1000*/ )
{
    DWORD dwSelfRefreshTime = dwTimeout/1000;
    BM_PACKAGE bp;
    bp.header.type = (unsigned short)BSL_CMD_SELF_REFRESH;
    bp.header.len = 4;
    bp.data = &dwSelfRefreshTime;

    BOOL bSuccess =  SendData( &bp, dwTimeout+5000 );
    LOG_OPR( _T("Self Refresh"), bSuccess );

    return bSuccess;
}

BOOL CBootModeOpr::ReadNandBlockInfo( DWORD dwTimeout /*= 1000*/ )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_READ_NAND_BLOCK_INFO, dwTimeout );
    LOG_OPR( _T("Read Nand Block Info"),bSuccess );
    return bSuccess;
}

BOOL CBootModeOpr::StartRead( LPBYTE pID, DWORD nIDLen, unsigned __int64 llLength,DWORD dwTimeout /*= 1000 */, BOOL bIs64Bit/* = FALSE*/)
{
	if(llLength == 0)
	{
		m_dwLastErrorCode = BSL_REP_SIZE_ZERO;
		LOG_OPR_FMT( FALSE,_T("Start read: ( Partition:\"%s\" ) read length is zero."),(WCHAR*)pID);
		return FALSE;
	}
	
    UINT uiDataLen =  bIs64Bit? nIDLen + 2*sizeof(__int64) : nIDLen + sizeof(DWORD);

#ifdef NOT_USE_DIAGCHAN
	LPBYTE lpPackData = m_pOrgData;
#else
	LPBYTE lpPackData = new BYTE[uiDataLen];
	if( lpPackData == NULL )
		return FALSE; 
#endif    
    ZeroMemory(lpPackData,uiDataLen);
	memcpy(lpPackData,pID,nIDLen); 
    if(bIs64Bit)	// ID(72) + Len(8) + Rev(8)
    {
        *(unsigned __int64 *)&lpPackData[ nIDLen ] = llLength;	
    }
    else	// ID(72) + Len(4)
    {
        *(DWORD *)&lpPackData[ nIDLen ] = (DWORD)llLength;	
    }
    
    BM_PACKAGE bp;
	bp.header.type = (unsigned short)BSL_CMD_READ_START;
	bp.header.len = (unsigned short)uiDataLen;
	bp.data = lpPackData;
    
    BOOL bSuccess =  SendData( &bp, dwTimeout );
	
#ifndef NOT_USE_DIAGCHAN
	delete []lpPackData;
#endif 
    
	LOG_OPR_FMT( bSuccess,_T("%s( Partition:\"%s\", Size:0x%08I64X)"), _T("Start Read"), (WCHAR*)pID, llLength ); 
    
    return bSuccess;
}

BOOL CBootModeOpr::EndRead( DWORD dwTimeout  /*= 1000*/ )
{
    BOOL bSuccess = SendCommandData( BSL_CMD_READ_END, dwTimeout );
    LOG_OPR( _T("End Read"), bSuccess );       
    return bSuccess;
}

BOOL CBootModeOpr::MidstRead( DWORD dwLength, unsigned __int64 llOffset,DWORD dwTimeout/* = 1000*/, BOOL bIs64Bit /*= FALSE*/)
{
    UINT uiDataLen =  bIs64Bit ? sizeof(DWORD)+sizeof(__int64): 2*sizeof( DWORD );

#ifdef NOT_USE_DIAGCHAN
	LPBYTE lpPackData = m_pOrgData;
#else
	LPBYTE lpPackData = new BYTE[uiDataLen];
	if( lpPackData == NULL )
	   return FALSE; 
#endif

	m_uiReadOffset = 0;
	BOOL bRetried = FALSE;

ReadPartitionFlash_Retry:
    *(DWORD *)&lpPackData[ 0 ] = dwLength;
    if (bIs64Bit)   //Len(4) + offset(8)
    {
        *(unsigned __int64 *)&lpPackData[ sizeof(DWORD) ] = llOffset;
    }
    else    //Len(4) + offset(4)
    {
        *(DWORD *)&lpPackData[ sizeof(DWORD) ] = (DWORD)llOffset;
    }
	m_dwLastPktSize = dwLength;


	BM_PACKAGE bp;
	bp.header.type = (unsigned short)BSL_CMD_READ_MIDST;
	bp.header.len = (unsigned short)uiDataLen;
	bp.data = lpPackData;

	BOOL bSuccess =  SendData( &bp, dwTimeout );

	if( !bSuccess && !bRetried )
	{
	   // log the current failed information
	   LOG_OPR_FMT( bSuccess,_T("%s(Size:0x%08X, Offset:0x%08I64X )"), _T("Read Flash"),dwLength,llOffset );
   
	   bRetried = TRUE;
	   // Clear channel dirty data and prepare for repeat reading.
	   Log( _T("Clear channel buffer and retry MidstRead."));
	   m_pChannel->Clear();
#ifdef NOT_USE_DIAGCHAN
	   m_bmPkg.Clear();
#endif
	   goto ReadPartitionFlash_Retry;
	}

#ifndef NOT_USE_DIAGCHAN
	delete []lpPackData;
#endif

	LOG_OPR_FMT( bSuccess,_T("%s(Size:0x%08X, Offset:0x%08I64X )"), _T("Read Flash"), dwLength,llOffset );	   
    return bSuccess;
}

BOOL   CBootModeOpr::SupportNotTransCode()
{
	return m_tDAInfo.bDisableTransCode;
}

BOOL   CBootModeOpr::DisableTransCode(DWORD dwTimeout/*  = 1000*/)
{
	BOOL bSuccess = SendCommandData( BSL_CMD_DISABLE_TRANSCODE , dwTimeout );
	LOG_OPR( _T("Disable TransCode"), bSuccess );
	return bSuccess;
}