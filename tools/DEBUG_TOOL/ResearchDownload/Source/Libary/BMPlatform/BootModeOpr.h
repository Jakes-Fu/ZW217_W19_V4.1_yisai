/******************************************************************************
** File Name:      BootModeOpr.h                                            *
** Author:         Apple Gao                                                 *
** DATE:           07/08/2004                                                *
** Copyright:      Spreatrum, Incoporated. All Rights Reserved.              *
** Description:    This files contains dll interface.                        *
******************************************************************************

  ******************************************************************************
  **                        Edit History                                       *
  ** ------------------------------------------------------------------------- *
  ** DATE               NAME                DESCRIPTION                       *
  ** 07/08/2004         Apple Gao           Create.                           *
******************************************************************************/

#ifndef _BOOTMODEOPR_H
#define _BOOTMODEOPR_H

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include <atlbase.h>
#ifdef NOT_USE_DIAGCHAN
#include "ICommChannel.h"
#include "BMPackage.h"
#else
#include "DiagChan.h"
#endif

//#define CHAN_SYNC
#define HIGH_PRECIOUS_TIME

#include "Global.h"
#include "HostProtocol.h"
#include "DaInfo.h"

#include "tr\Tr.h"

#define MAX_RECV_LEN  (0x8000)   //32K Bytes

class  CBootModeOpr: public CTr
{
public:
    CBootModeOpr();
    virtual ~CBootModeOpr();
public:
    BOOL   CheckBaud( DWORD dwTimeout = 1000 );
    BOOL   Connect( DWORD dwTimeout = 1000 );
    BOOL   Excute( DWORD dwTimeout = 1000 );
    BOOL   Reset( LPBYTE lpRefData, DWORD dwLength, DWORD dwTimeout = 1000 );
    BOOL   SetFirstMode( LPBYTE lpRefData, DWORD dwLength, DWORD dwTimeout  = 1000 );
    BOOL   SetRandomData( LPBYTE lpRefData, DWORD dwLength, DWORD dwTimeout  = 1000 );
    BOOL   HostConnect( DWORD dwTimeout = 1000 );
    BOOL   SetFdlFileInfo( const BMFileInfo* pBMFileInfo );
    int    HostCountPackets() { return m_sHostProtocol.CountPackets();}
    BOOL   HostDownloadStart( DWORD dwTimeout = 1000 );
    BOOL   HostDownloadData( int nSeq, DWORD dwTimeout = 1000 );
    BOOL   HostDownloadEnd( DWORD dwTimeout = 1000 );
    BOOL   HostExcute( DWORD dwTimeout = 1000 );
    
    BOOL   ReadFlash( DWORD dwBase, DWORD dwLength, DWORD dwTimeout = 1000 );
    BOOL   ReadPartitionFlash( DWORD dwBase, DWORD dwLength,
                               DWORD dwOffset, DWORD dwTimeout = 1000 );
    BOOL   StartData( unsigned __int64 llBase, unsigned __int64 llLength,
                      LPBYTE lpDownLoadData, DWORD dwCheckSum = 0, DWORD dwTimeout = 1000, BOOL bIs64Bit = FALSE );
    BOOL   StartData( LPBYTE pID, DWORD nIDLen, unsigned __int64 llLength,
                      LPBYTE lpDownLoadData, DWORD dwCheckSum = 0, DWORD dwTimeout = 1000, BOOL bIs64Bit = FALSE );
    BOOL   EndData( DWORD dwTimeout  = 1000 );
    BOOL   MidstData( DWORD dwLength, LPBYTE lpDownLoadData,
                      DWORD dwTimeout = 1000, DWORD dwTotal = 0 );
    BOOL   ReadChipType( DWORD dwTimeout = 1000 );
    BOOL   ReadNVItem( DWORD dwStartAddr, DWORD dwEndAddr,
                       unsigned short uiItemID, DWORD dwTimeout = 1000 );
    BOOL   ChangeBaud( DWORD dwTimeout = 1000 );
    BOOL   EraseFlash( DWORD dwBase, DWORD dwLength, DWORD dwTimeout = 1000 );
    BOOL   EraseFlash( LPBYTE pID, DWORD nIDLen, DWORD dwLength,
                       DWORD dwTimeout = 1000 );
    BOOL   Repartition( DWORD dwTimeout = 1000 );
    BOOL   Repartition( LPBYTE pPartitionInfo, DWORD dwSize, DWORD dwTimeout = 1000 );
    BOOL   SendExtTable( LPBYTE pExtTable, DWORD dwSize, DWORD dwTimeout = 1000 );
    BOOL   AllocRecvBuffer( DWORD dwSize );
    LPBYTE GetRecvBuffer( );
    DWORD  GetRecvBufferSize();
    void   FreeRecvBuffer();
    BOOL   Initialize( LPCTSTR lpszChannelProgID, LPBYTE lpOpenParam,
                       BOOL bBigEndian, DWORD dwCookie );
    void   Uninitialize();
    DWORD  GetLastErrorCode();
    DWORD  GetSupportRawData();
    DWORD  GetFlushSize();
    void   SetLastErrorCode( DWORD dwErrorCode );
    DWORD  GetSupportRandomNum();
    BYTE*  GetRandomData();
    void   GetLastErrorDescription( DWORD dwErrorCode, LPTSTR lpszErrorDescription, int nSize );
    void   SetIsCheckCrc( BOOL bCheckCrc );
    void   StopOpr();
    BOOL   SupportNotTransCode();
    BOOL   SupportRawData();
	BOOL   SupportUbootLogDump();
    ICommChannel*  GetChannelPtr( );
    void   SetChannel( ICommChannel* pChannel );
    BOOL   ReadFlashType( DWORD dwTimeout = 1000 );
    BOOL   ReadFlashInfo( DWORD dwTimeout = 1000 );
    BOOL   ReadFlashUID( DWORD dwTimeout = 1000 );
    BOOL   ReadChipUID( DWORD dwTimeout = 1000 );
    BOOL   ReadSoftSimEID( DWORD dwTimeout = 1000 );
    void   SetStartDataInfo( BOOL bHasCheckSum ); // @hongliang.xin 2009-3-17 add checksum at "StartData"
    BOOL   ReadSectorSize( DWORD dwTimeout = 1000 );
    BOOL   DoBeforeCheckBaud();
    BOOL   DoAfterChackBaud();
    BOOL   KeepCharge( DWORD dwTimeout = 1000 );
    BOOL   DdrCheck( DWORD dwTimeout = 1000 );
    BOOL   SelfRefresh( DWORD dwTimeout = 1000 );
    BOOL   EnableRawData( DWORD dwTimeout = 1000 );
    BOOL   MidstRawStart( DWORD dwLength, LPBYTE lpSendData, DWORD dwTimeout = 1000 );
    BOOL   MidstRawStart2( DWORD dwTimeout = 1000 );
    BOOL   FlushData( DWORD dwLength, LPBYTE lpSendData, DWORD dwTimeout = 1000 );
    BOOL   ReadNandBlockInfo( DWORD dwTimeout = 1000 );
    BOOL   StartRead( LPBYTE pID, DWORD nIDLen, unsigned __int64 llLength, DWORD dwTimeout = 1000, BOOL bIs64Bit = FALSE );
    BOOL   EndRead( DWORD dwTimeout  = 1000 );
    BOOL   MidstRead( DWORD dwLength, unsigned __int64 llOffset, DWORD dwTimeout = 1000, BOOL bIs64Bit = FALSE );
    BOOL   MidstDataFast( DWORD dwLength, LPBYTE lpSendData,
                          DWORD dwTimeout = 1000, unsigned __int64 llTotal = 0 );
    BOOL   PowerOff( DWORD dwTimeout  = 1000 );
    BOOL   CheckRoot( DWORD dwTimeout = 1000 );
    BOOL   EnableFlash( DWORD dwTimeout  = 1000 );
    BOOL   EnableSecureBoot( DWORD dwTimeout = 1000 );
    BOOL   ReadRefInfo( LPBYTE lpRefData, DWORD dwLength, DWORD dwTimeout = 1000 );
    BOOL   WriteAPRInfo( LPBYTE lpRefData, DWORD dwLength, DWORD dwTimeout = 1000 );
    BOOL   ReadTransceiverType( DWORD dwTimeout = 1000 );
    BOOL   EnableDebugMode( LPBYTE lpData, DWORD dwLength, DWORD dwTimeout = 1000 );
    BOOL   DisableTransCode( DWORD dwTimeout  = 1000 );
    BOOL   EnableUbootLog( DWORD dwTimeout = 1000 );
    BOOL   EndProcess( DWORD dwTimeout  = 1000 );
    BOOL   SetCheckSumCfg( LPCTSTR lpszFileId, int nCheckSum );
    DWORD  GetCheckSumCfg( LPCTSTR lpszFileId, LPCTSTR lpszCrcFile );
    BOOL   DumpUbootLog( DWORD dwTimeout = 1000 );
    BOOL   ChangeSelinux( LPBYTE lpData, DWORD dwLength, DWORD dwTimeout = 1000);
    
#ifdef NOT_USE_DIAGCHAN
    BOOL   EnableCheckBaudCrc( BOOL bEnable );
    int    GetCheckBaudCrcType();
    int    GetCrcType();
#endif
    BOOL   EnableSupportZLP( BOOL bEnable ); //
    BOOL   IsNeed2ndEnumPort();
    DWORD  DoNVCheckSum( LPBYTE pDataBuf, DWORD dwSize );
    BOOL   SetTimeStamp( DWORD dwTimeout  = 1000 );
    BOOL   ReadPartitionInfo( DWORD dwTimeout = 1000 );
    void   OpenLog(long dwOprCookie );
    void   LogInfo( LPCTSTR lpszLog );
    void   LogData( LPCTSTR lpszLog );
    void   LogError( LPCTSTR lpszLog );
    void   LogError( LPCTSTR lpszLog, BOOL bSuccess );
    void   LogError( BOOL bSuccess, LPCTSTR lpszFmt, ... );
    void   CloseLog( DWORD dwError , LPCTSTR lpStep);
  
    FILE*  m_pSerialLogFile;
    FILE*  m_pDDRLogFile;
    
protected:
#ifdef NOT_USE_DIAGCHAN
    BOOL   SendData( BM_PACKAGE* lpPkg, DWORD dwTimeout );
    BOOL   SendData( LPBYTE lpData, int iDataSize, BOOL bCRC = TRUE, DWORD dwTimeout = 1000 );
    BOOL   ProcessData( LPBYTE lpData, int iDataLen );
    BOOL   SendHostData( LPBYTE lpData, int iDataSize, DWORD dwTimeout = 1000, BOOL bWaiting = TRUE );
    
#else
    BOOL   SendData( BM_PACKAGE* lpPkg, DWORD dwTimeout = 1000 );
#endif
    BOOL   SendCommandData( pkt_type_s nCmdType, DWORD dwTimeout = 1000 );
    BOOL   SendPacketData( pkt_type_s nCmdType,
                           const int nSendDataLen,
                           LPBYTE lpPacketData,
                           UINT uiDataLen,
                           DWORD dwTimeout = 1000 );
                           
private:
#ifdef NOT_USE_DIAGCHAN
    BOOL CreateRecvThread();
    void DestroyRecvThread();
#endif
    
    static DWORD WINAPI GetRcvDataThread( LPVOID lpParam );
    DWORD  RcvDataThreadProc();
    BOOL   DisconnectChannel( );
    BOOL   ConnectChannel( LPBYTE lpOpenParam );
    //void   InitLogFileSuffix( DWORD dwCookie );
    
    void   InitChannelLog();

    static DWORD WINAPI LogData( LPVOID lpPara, LPBYTE lpLog );
    
    void   CloseDevBeforeRemove();

	void   RunCmdline(LPCTSTR lpCmd);
	
private:
#ifdef NOT_USE_DIAGCHAN
    ICommChannel*       m_pChannel;
    HANDLE              m_hOprEvent;
    CBMPkg              m_bmPkg;
    LPBYTE              m_pOrgData;
    LPBYTE              m_pOutData;
    BOOL                m_bSupportZLP;
    BOOL                m_bWorking;
#ifdef  CHAN_SYNC
    BYTE                m_RecvData[ MAX_RECV_LEN ];
    //ULONG               m_ulRecvDataLen;
#endif // CHAN_SYNC
#else
    IProtocolChannel*   m_pChannel;
#endif
    
    BOOL                m_bBigEndian;
    
    LPBYTE              m_lpReceiveBuffer;
    DWORD               m_dwReceiveLen;
    DWORD               m_dwBufferSize;
    pkt_type_s          m_iLastSentPktType;
    DWORD               m_dwLastPktSize;
    BOOL                m_bCheckCrc;
    DWORD               m_dwLastErrorCode;
    BOOL                m_bOperationSuccess;
    DWORD               m_dwRecvThreadID;
    HANDLE              m_hRecvThreadState;
    HANDLE              m_hRecvThread;
    //FILE                *m_pLogFile;
    FILE*                m_pCrcFile;
    CString             m_strCrcFileName;
    CString             m_strLogSuffix;
    DWORD               m_dwCookie;
    CRITICAL_SECTION    m_csRecvBbuffer;
    CRITICAL_SECTION    m_csLastError;
    CRITICAL_SECTION    m_csClosePort;
    
    UINT                m_uiReadOffset;
    BOOL                m_bHasCheckSum; // @hongliang.xin 2009-3-17 for "StartData"
    DWORD               m_dwBaud;
    
    BOOL                m_bGotVerAck;  // If check baud has got ver ack, set this true
    BOOL                m_bOpened;
    
    BOOL                m_bHostProtocol;
    CHostProtocol       m_sHostProtocol;
    CDaInfo             m_tNewDaInfo;
    
    CString             m_strLogPath;

	static TCHAR		s_czDefNamePre[32];		
};

#endif // _BOOTMODEOPR_H