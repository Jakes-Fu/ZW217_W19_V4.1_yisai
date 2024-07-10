/******************************************************************************
** File Name:      CommChannel.h                                            *
** Author:         Apple Gao                                                 *
** DATE:           20/07/2007                                                *
** Copyright:      Spreatrum, Incoporated. All Rights Reserved.              *
** Description:    This files contains dll interface.                        *
******************************************************************************

  ******************************************************************************
  **                        Edit History                                       *
  ** ------------------------------------------------------------------------- *
  ** DATE				NAME				DESCRIPTION                       *
  ** 20/07/2007			Apple Gao			Create.							  * 
******************************************************************************/

#ifndef _COMMCHANNEL_H
#define	_COMMCHANNEL_H

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/

#include "Channelitf.h"
#include "adlogelitf.h"
#include "logelerr.h"
#include "LogelProperty.h"


class CCommChannel : public ICommunicationChannel 
{
public:
	CCommChannel();
	virtual ~CCommChannel();
public:
    
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj);
    STDMETHOD_(ULONG,AddRef) (THIS);
    STDMETHOD_(ULONG,Release) (THIS);
    
    STDMETHOD( SetReceiver )( THIS_ DWORD  dwMsgId, BOOL bRcvThread,
        const LPVOID pReceiver );
    
    STDMETHOD( GetReceiver )( THIS_ DWORD  &dwMsgId, BOOL &bRcvThread,
        LPVOID &pReceiver );    
    
    STDMETHOD( Open )( THIS_ LPBYTE pOpenArgument );
    
    STDMETHOD( Close )( THIS ) ;
    
    STDMETHOD( Clear )( THIS );
    
    STDMETHOD_(ULONG, Read )( THIS_ LPVOID lpData, ULONG ulDataSize ) ;
    
    STDMETHOD_(ULONG, Write )( THIS_ LPVOID lpData,ULONG ulDataSize ) ;
    
    STDMETHOD( FreeMem )(THIS_ LPVOID pMemBlock );
    
    //STDMETHOD( GetChannelHandle )( THIS );

    STDMETHOD( GetProperty )( THIS_ LONG  lFlags,  
                const BSTR cbstrName,  VARIANT *  pvarValue );
    STDMETHOD( PutProperty )( THIS_ LONG lFlags,  
         const BSTR cbstrName,  const VARIANT * pcvarValue );

protected:
    BOOL _SetReceiver(  DWORD  dwMsgId, BOOL bRcvThread,
        const LPVOID pReceiver );
    
    void _GetReceiver( DWORD  &dwMsgId, BOOL &bRcvThread, LPVOID &pReceiver ); 
    
    virtual BOOL _Open( LPBYTE pOpenArgument );
    
    virtual void _Close( ) ;
    
    virtual BOOL _Clear( );
    
    virtual DWORD _Read(  LPVOID lpData, ULONG ulDataSize ) ;
    
    virtual DWORD _Write( LPVOID lpData,ULONG ulDataSize ) ;
    
    void _FreeMem(  LPVOID pMemBlock );

    //HANDLE _GetChannelHandle();

    virtual HRESULT _GetProperty( LONG lFlags, const BSTR cbstrName, VARIANT* pvarValue );
    
    virtual HRESULT _PutProperty( LONG lFlags, const BSTR cbstrName, const VARIANT* pvarValue );

protected:
    
    // -----------------------------------------------------------------------------
    // Name:
    //      SendDataToUplevel
    //
    // Function:
    //      Forward the data to the registered up-level application
    // 
    // Description:
    //      The registered up-level receiver is specified via SetReceiver()
    // -----------------------------------------------------------------------------
    void SendDataToUplevel(
        const void * pData, // Data buffer
        DWORD        dwSize // Data size
        );

	static DWORD WINAPI PlaybackThreadProc(
        LPVOID lpParam
        );
	DWORD  PlaybackFunc();

protected:        
    // Has connected with the Channel?
    volatile BOOL    m_bConnected;    
    
    // Message ID for the up-level application to receive the UART data
    DWORD   m_dwMsgId;
    // Indicates the up-level receiver is a thread or a window
    BOOL    m_bRcvThread;
    // ID of the up-level receiving thread
    DWORD   m_dwRcvThreadID;
    // Handle of the up-level receiving window
    HWND    m_hRcvWindow; 

	BOOL								m_bSaveDataFile;
	BOOL								m_bSendDataToUpLevel;
	BOOL								m_bDebugMode;
	BOOL								m_bPlayback;

	HANDLE								m_hPlaybackThread;

protected:
    void Log( LPCTSTR lpszLog );
    BOOL OpenFiles( DWORD dwCookie );    
    void CloseFiles();
    void GetAppPath( char* szAppPath );
    FILE                *m_pLogFile; 
	FILE				*m_pOrgDataFile;
    DWORD               m_dwCookie;    
    ULONG m_ulRef;
};





#endif // _COMMCHANNEL_H