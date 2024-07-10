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

#include "BootModeitf.h"
#include "Global.h"
#include <afxtempl.h>
#include <afxmt.h>
#include "BMFileImpl.h"
#include "ICommChannel.h"
#include "BootModeIntegOpr.h"

typedef CMap< DWORD, DWORD, IBMOprObserver*, IBMOprObserver*> MAP_OBSERVER;

class CBootModeObj 
{
public:
    CBootModeObj();
    virtual ~CBootModeObj();

public:
    // IBootModeHandler    
    HRESULT SetWaitTimeForNextChip(/*[in]*/ DWORD dwWaitTime = CHANGE_CHIP_TIMEOUT );
    
    HRESULT StopBootModeOperation();
    
    HRESULT StartBootModeOperation(
        /*[in]*/ LPVOID lpBMFileInfo, 
        /*[in]*/ UINT uFileCount, 
        /*[in]*/ LPVOID pOpenArgument, 
        /*[in]*/ BOOL bBigEndian, 
        /*[in]*/ DWORD dwOprCookie,
        /*[in]*/ BOOL bRcvThread,  
        /*[in]*/ LPCVOID pReceiver,
        /*[in]*/ LPCWSTR lpbstrProgID );
        
    HRESULT SetCommunicateChannelPtr( /*[in]*/LPVOID pCommunicateChannel );    
    HRESULT GetCommunicateChannelPtr( /*[out]*/LPVOID* ppCommunicateChannel );    
    //IBMOprSubscriber
    HRESULT SubscribeOperationObserver(IBMOprObserver* pSink,
										   ULONG uFlags,
                                           LPDWORD lpdwCookie );    
    HRESULT UnsubscribeOperationObserver(DWORD dwCookie);    
    //IBMOprBuffer
    const LPBYTE GetReadBuffer() ;
    DWORD   GetReadBufferSize ( );

	void EnablePortSecondEnum(BOOL bEnable);
    
protected:    
    static DWORD WINAPI GetBMThreadFunc(LPVOID lpParam);
    DWORD BMThreadFunc();   
    
    BOOL CreateBMThread();
    void DestroyBMThread();
            
    BOOL GenerateStartOprNotification( DWORD dwOprCookie, LPCTSTR lpszFileId, 
		                               LPCTSTR lpszFileType,LPCTSTR lpszOperationType );
    BOOL GenerateEndOprNotification( DWORD dwOprCookie,LPCTSTR lpszFileId, 
		                             LPCTSTR lpszFileType,LPCTSTR lpszOperationType, DWORD dwResult  );
    BOOL GenerateStartFileOprNotification( DWORD dwOprCookie, LPCTSTR lpszFileId, LPCTSTR lpszFileType );    
    BOOL GenerateEndFileOprNotification( DWORD dwOprCookie, LPCTSTR lpszFileId, 
		                                 LPCTSTR lpszFileType, DWORD dwResult );    
    BOOL GenerateStartNotification( DWORD dwOprCookie , DWORD dwResult );    
    BOOL GenerateEndNotification( DWORD dwOprCookie , DWORD dwResult );

    void EndProc( BOOL bEndSuccess );    
    void Log( LPCTSTR lpszLog );    
    int  EnumKeys(char* pSection,CStringArray* pKeys);
    
private:
//  COleDispatchDriver  m_OprDriver;
	CBootModeIntegOpr   m_OprDriver;    
    HANDLE              m_hBMThread;     
    BOOL                m_bExitThread;
    CBMFileImpl         m_BMFileImpl;        
    DWORD               m_dwOprCookie;    
    DWORD               m_dwWaitTime;    
    MAP_OBSERVER        m_mapObserverRegs;
    DWORD               m_dwNextObserverCookie;
    CCriticalSection    m_CS;

	BOOL				m_bBigEndian;
	BYTE                m_byteOpenArgument[8];
	TCHAR               m_szWProgID[MAX_PATH];
	BOOL				m_bRcvThread;
	DWORD				m_dwReceiver;
};
#endif // _BOOTMODEOBJ_H
