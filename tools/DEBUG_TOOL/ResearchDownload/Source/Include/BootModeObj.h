/******************************************************************************
** File Name:      BootModeObj.h                                            *
** Author:         Apple Gao                                                 *
** DATE:           07/22/2004                                                *
** Copyright:      Spreatrum, Incoporated. All Rights Reserved.              *
** Description:    This files contains dll interface.                        *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** ------------------------------------------------------------------------- *
** DATE				NAME				DESCRIPTION                          *
** 07/21/2004		Apple Gao			Create.							     * 
******************************************************************************/

#ifndef _BOOTMODEOBJ_H
#define	_BOOTMODEOBJ_H


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
//#include "BootModeOpr.h"
//#include <atlbase.h>
//extern CComModule _Module;
//#include <atlcom.h>
#include "BootModeitf.h"
#include "Global.h"
#include <afxtempl.h>
#include <afxmt.h>
#include "BMFileImpl.h"


#ifndef BOOTMODEOBJ_API
#ifdef  BOOTMODEOBJ_EXPORTS
#define BOOTMODEOBJ_API __declspec(dllexport)
#else
#define BOOTMODEOBJ_API __declspec(dllimport)
#endif  
#endif  

class BOOTMODEOBJ_API CBootModeObj 
{
public:
    CBootModeObj();
    virtual ~CBootModeObj();

public:
    // IBootModeHandler    
    STDMETHOD(SetWaitTimeForNextChip)(/*[in]*/ DWORD dwWaitTime = CHANGE_CHIP_TIMEOUT );
    
    STDMETHOD(StopBootModeOperation)();
    
    STDMETHOD(StartBootModeOperation)(
        /*[in]*/ DWORD lpBMFileInfo, 
        /*[in]*/ UINT uFileCount, 
        /*[in]*/ DWORD pOpenArgument, 
        /*[in]*/ BOOL bBigEndian, 
        /*[in]*/ DWORD dwOprCookie,
        /*[in]*/ BOOL bRcvThread,  
        /*[in]*/ const DWORD pReceiver,
        /*[in]*/ BSTR lpbstrProgID );
        
    STDMETHOD( SetCommunicateChannelPtr)( /*[in]*/LPVOID pCommunicateChannel );
    
    STDMETHOD( GetCommunicateChannelPtr)( /*[out]*/LPVOID* ppCommunicateChannel );
    
    //IBMOprSubscriber
    STDMETHOD(SubscribeOperationObserver) (THIS_ IBMOprObserver* pSink,
        ULONG uFlags,
        LPDWORD lpdwCookie );
    
    STDMETHOD(UnsubscribeOperationObserver) (THIS_ DWORD dwCookie);
    
    //IBMOprBuffer
    STDMETHOD_(const LPBYTE, GetReadBuffer) ( THIS ) ;

    STDMETHOD_( DWORD, GetReadBufferSize) ( THIS );
    
protected:
    
    static DWORD GetBMThreadFunc(LPVOID lpParam);
    DWORD BMThreadFunc();   
    
    BOOL CreateBMThread();
    void DestroyBMThread();
            
    BOOL GenerateStartOprNotification( DWORD dwOprCookie, LPCTSTR lpszFileType,
                            LPCTSTR lpszOperationType );

    BOOL GenerateEndOprNotification( DWORD dwOprCookie, LPCTSTR lpszFileType,
                            LPCTSTR lpszOperationType, DWORD dwResult  );

    BOOL GenerateStartFileOprNotification( DWORD dwOprCookie, LPCTSTR lpszFileType );
    
    BOOL GenerateEndFileOprNotification( DWORD dwOprCookie, LPCTSTR lpszFileType );
    
    BOOL GenerateStartNotification( DWORD dwOprCookie , DWORD dwResult );
    
    BOOL GenerateEndNotification( DWORD dwOprCookie , DWORD dwResult );

    void EndProc( BOOL bEndSuccess );
    
    void GetAppPath( char* szAppPath );

    int EnumKeys(char* pSection,CStringArray* pKeys);
    
private:
    COleDispatchDriver  m_OprDriver;
    
    HANDLE              m_hBMThread;     
    DWORD               m_dwBMThreadID;  
    BOOL                m_bExitThread;

    CBMFileImpl         m_BMFileImpl;
        
    DWORD               m_dwOprCookie;
    
    DWORD               m_dwWaitTime;
    
    CMap< DWORD, DWORD, IBMOprObserver*, IBMOprObserver*> m_mapObserverRegs;
    DWORD               m_dwNextObserverCookie;
    CCriticalSection    m_CS;
};
#endif // _BOOTMODEOBJ_H
