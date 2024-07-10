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
  ** DATE				NAME				DESCRIPTION                       *
  ** 07/08/2004			Apple Gao			Create.							  * 
******************************************************************************/

#ifndef _BOOTMODEOPR_H
#define	_BOOTMODEOPR_H

//#define ATL_MODE2
/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/

#ifdef ATL_MODE2
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <comdef.h>
#include <initguid.h>
#include "Channelitf.h"
#include "Channelguid.h"
#pragma  comment( lib, "ole32" )
#else
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <stdio.h>
#include <stdlib.h>
#include "UartComm.h"
#pragma  comment( lib, "UartComm")
#endif

#include "Global.h"

#define MAX_RECV_LEN  (0x8000)   //32K Bytes

#ifndef BOOTMODEOPR_API
#ifdef  BOOTMODEOPR_EXPORTS
#define BOOTMODEOPR_API __declspec(dllexport)
#else
#define BOOTMODEOPR_API __declspec(dllimport)
#endif  
#endif  

class BOOTMODEOPR_API CBootModeOpr
{
public:
    CBootModeOpr();
    virtual ~CBootModeOpr();
public:
    BOOL CheckBaud( DWORD dwTimeout = 1000 );
    BOOL Connect( DWORD dwTimeout = 1000 );
    BOOL Excute( DWORD dwTimeout = 1000 );
    BOOL Reset( DWORD dwTimeout = 1000 );
    BOOL ReadFlash(  DWORD dwBase,DWORD dwLength, DWORD dwTimeout = 1000  );
    BOOL StartData( DWORD dwBase,DWORD dwLength ,
        LPBYTE lpDownLoadData, DWORD dwTimeout = 1000 );
    BOOL EndData( DWORD dwTimeout  = 1000 );
    BOOL MidstData( DWORD dwBase,DWORD dwLength, DWORD dwTimeout = 1000  );
        
    BOOL AllocRecvBuffer( DWORD dwSize );
    LPBYTE GetRecvBuffer( ); 
    DWORD  GetRecvBufferSize();
    void FreeRecvBuffer();
    
    BOOL Initialize( LPCTSTR lpszChannelProgID, LPBYTE lpOpenParam,
        BOOL bBigEndian, DWORD dwCookie );
    
    void Uninitialize();
    
    DWORD GetLastErrorCode();
        
    void Log( LPCTSTR lpszLog );
    
    void GetLastErrorDescription( DWORD dwErrorCode,LPTSTR lpszErrorDescription, int nSize );
    
    void SetIsCheckCrc( BOOL bCheckCrc );

    void StopOpr();
    
    DWORD GetChannelPtr( );

    void SetChannel( DWORD pChannel );
    
protected:
    BOOL SendData( LPBYTE lpData, int iDataSize, 
                BOOL bCRC = TRUE, DWORD dwTimeout = 1000 );
    BOOL ProcessData( LPBYTE lpData, int iDataLen );
    
    BOOL SendCommandData( pkt_type_s nCmdType , DWORD dwTimeout = 1000 );
    BOOL SendPacketData( pkt_type_s nCmdType, 
        const int nSendDataLen,
        LPBYTE lpPacketData,
        UINT uiDataLen,
        DWORD dwTimeout = 1000 );
    
private:
    BOOL CreateRecvThread();
    void DestroyRecvThread();

    static DWORD WINAPI GetRcvDataThread( LPVOID lpParam );
    DWORD  RcvDataThreadProc();
    
    BOOL DisconnectChannel( );
    BOOL ConnectChannel(  LPBYTE lpOpenParam );

    BOOL OpenLogFile( DWORD dwCookie );
    
    void CloseLogFile();

    void LogOpr( LPCTSTR lpszOperation, BOOL bSuccess );
    
    void GetAppPath( char* szAppPath );

private:
    CComPtr<ICommunicationChannel>      m_pChannel;
/*
#ifdef ATL_MODE2
    CComPtr<ICommunicationChannel>      m_pChannel;
#else
    CUartComm*                          m_pChannel;    
#endif    
*/
    BOOL                                m_bBigEndian;
    LPBYTE                              m_lpCurrRead;    
    LPBYTE                              m_lpReceiveBuffer;
    DWORD                               m_dwReceiveLen;
    DWORD                               m_dwBufferSize;
    pkt_type_s                          m_iLastSentPktType;
    DWORD                               m_dwLastPktSize;
    BOOL                                m_bCheckCrc;

    DWORD                               m_dwLastErrorCode;
    BOOL                                m_bOperationSuccess;
    
    BYTE                                m_RecvData[ MAX_RECV_LEN ];
    ULONG                               m_ulRecvDataLen;
    
    DWORD                               m_dwRecvThreadID;
    HANDLE                              m_hRecvThreadState;
    HANDLE                              m_hRecvThread;    

    FILE                                *m_pLogFile;
    BOOL                                m_bExitOpr;

    CRITICAL_SECTION                    m_csRecvBbuffer;    
};



#endif // _BOOTMODEOPR_H