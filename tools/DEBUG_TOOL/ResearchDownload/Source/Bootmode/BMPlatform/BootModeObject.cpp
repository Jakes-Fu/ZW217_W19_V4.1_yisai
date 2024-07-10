// BootModeObject.cpp : Implementation of CBootModeObject
#include "stdafx.h"
#include "BMPlatform.h"
#include "BootModeObject.h"
#include "OptionHelpper.h"
//#include "BootModeGuid.h"

/////////////////////////////////////////////////////////////////////////////
// CBootModeObject

HRESULT CBootModeObject::StartBootModeOperation(     LPVOID lpBMFileInfo, 
                                                     UINT uFileCount, 
                                                     LPVOID pOpenArgument, 
                                                     BOOL bBigEndian, 
                                                     DWORD dwOprCookie,
                                                     BOOL bRcvThread, 
                                                     LPCVOID pReceiver,
                                                     LPCWSTR lpbstrProgID)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
        
    return m_BootModeObj.StartBootModeOperation( lpBMFileInfo, 
        uFileCount, pOpenArgument, bBigEndian, 
        dwOprCookie, bRcvThread, pReceiver, lpbstrProgID );
    
}

HRESULT CBootModeObject::StopBootModeOperation()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
        
    return m_BootModeObj.StopBootModeOperation();
}

HRESULT CBootModeObject::SetWaitTimeForNextChip(DWORD dwWaitTime)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
        
    return m_BootModeObj.SetWaitTimeForNextChip( dwWaitTime );
}

HRESULT CBootModeObject::SetCommunicateChannelPtr( LPVOID pCommunicateChannel )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
        
    return m_BootModeObj.SetCommunicateChannelPtr( pCommunicateChannel );    
}

HRESULT CBootModeObject::GetCommunicateChannelPtr( LPVOID* ppCommunicateChannel )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
        
    return m_BootModeObj.GetCommunicateChannelPtr( ppCommunicateChannel );
}

HRESULT CBootModeObject::SubscribeOperationObserver(IBMOprObserver* pSink, 
                                                         ULONG uFlags,                                                       
                                                         LPDWORD lpdwCookie )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
        
    return m_BootModeObj.SubscribeOperationObserver( pSink, uFlags, lpdwCookie );
}

HRESULT CBootModeObject::UnsubscribeOperationObserver( DWORD dwCookie )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
        
    return m_BootModeObj.UnsubscribeOperationObserver( dwCookie );
}

const LPBYTE CBootModeObject::GetReadBuffer(  )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    return m_BootModeObj.GetReadBuffer(  );
}

DWORD CBootModeObject::GetReadBufferSize(  )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
        
    return m_BootModeObj.GetReadBufferSize(  );
}

HRESULT CBootModeObject::SetCheckBaudTimes(  DWORD dwTimes )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	extern COptionHelpper ohObject;
    ohObject.SetCheckBaudTimes( dwTimes );

	return S_OK;
}

HRESULT CBootModeObject::SetRepartitionFlag(  DWORD dwFlag )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
        
    extern COptionHelpper ohObject;
    ohObject.SetRepartitionFlag( dwFlag );
    
    return S_OK;
}

HRESULT CBootModeObject::SetReadFlashBefRepFlag(  DWORD dwFlag )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
        
    extern COptionHelpper ohObject;
    ohObject.SetReadFlashBefRepFlag( dwFlag );
    
    return S_OK;
}

DWORD CBootModeObject::GetPacketLength(LPCWSTR bstrFileType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    USES_CONVERSION;
    
    extern COptionHelpper ohObject;
	
	return ohObject.GetPacketLength(W2T((LPWSTR)bstrFileType));
}

HRESULT CBootModeObject::GetProperty( LONG  lFlags, LPCWSTR cbstrName,  LPVOID*  pvarValue )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

    extern COptionHelpper ohObject;	

    if(ohObject.GetProperty(lFlags,W2T((LPWSTR)cbstrName),pvarValue))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}    
}

HRESULT CBootModeObject::SetProperty( LONG lFlags, LPCWSTR cbstrName,  LPCVOID pcvarValue )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;

	if(_tcsicmp(W2T((LPWSTR)cbstrName),_T("EnablePortSecondEnum")) == 0)
	{
		BOOL bEnable = *(BOOL*)pcvarValue;
		m_BootModeObj.EnablePortSecondEnum(bEnable);
	}
        
    extern COptionHelpper ohObject;
    if(ohObject.SetProperty(lFlags,W2T((LPWSTR)cbstrName),pcvarValue))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}  
}
