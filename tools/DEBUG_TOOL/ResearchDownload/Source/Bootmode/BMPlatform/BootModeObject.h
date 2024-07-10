// BootModeObject.h : Declaration of the CBootModeObject

#ifndef __BOOTMODEOBJECT_H_
#define __BOOTMODEOBJECT_H_

#include "resource.h"       // main symbols
#include "BootModeObj.h"


/////////////////////////////////////////////////////////////////////////////
// CBootModeObject
class CBootModeObject: public IBootModeHandler
{
public:
    CBootModeObject()
    {	
    }
	virtual ~CBootModeObject()
	{

	}

public: 
    virtual HRESULT StartBootModeOperation( /*[in]*/ LPVOID  lpDLFileInfo, 
									        /*[in]*/ UINT    uFileCount, 
									        /*[in]*/ LPVOID  pOpenArgument, 
									        /*[in]*/ BOOL    bBigEndian, 
									        /*[in]*/ DWORD   dwOprCookie,
		                                    /*[in]*/ BOOL    bRcvThread,  
		                                    /*[in]*/ LPCVOID pReceiver ,
				                            /*[in]*/ LPCWSTR lpbstrProgID );			  
    virtual HRESULT StopBootModeOperation();	
    virtual HRESULT SetWaitTimeForNextChip(/*[in]*/ DWORD dwWaitTime);			  
    virtual HRESULT SetCommunicateChannelPtr( /*[in]*/LPVOID pCommunicateChannel );				  
    virtual HRESULT GetCommunicateChannelPtr( /*[out]*/LPVOID* ppCommunicateChannel );
    virtual HRESULT SubscribeOperationObserver ( IBMOprObserver* pSink,
					                             ULONG uFlags,
					                             LPDWORD lpdwCookie );   
    virtual HRESULT UnsubscribeOperationObserver ( DWORD dwCookie);   
    virtual const LPBYTE GetReadBuffer();
    virtual DWORD GetReadBufferSize();    
	
    // IBMSettings
    virtual HRESULT SetCheckBaudTimes( DWORD dwTimes );    
    virtual HRESULT SetRepartitionFlag( DWORD dwFlag );   	
    virtual HRESULT SetReadFlashBefRepFlag( DWORD dwFlag ); 	
    virtual DWORD   GetPacketLength( LPCWSTR bstrFileType );	
	virtual HRESULT GetProperty( LONG  lFlags, LPCWSTR cbstrName, LPVOID*  pvarValue );  
    virtual HRESULT SetProperty( LONG lFlags, LPCWSTR cbstrName,  LPCVOID pcvarValue );
private:
    CBootModeObj     m_BootModeObj;
};

#endif //__BOOTMODEOBJECT_H_
