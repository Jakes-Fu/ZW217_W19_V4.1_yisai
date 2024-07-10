// BootModeObj.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
//#include <initguid.h>
#include "BootModeObj.h"
#include <atlconv.h>
#include <process.h>

#include "OptionHelpper.h"

extern COptionHelpper ohObject;

#define DEFAULT_MAX_LENGTH          0x3000
#define MAX_UINT_SLEEP_TIME         500

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBootModeObj
CBootModeObj::CBootModeObj()
{
    m_mapObserverRegs.RemoveAll();
	m_hBMThread				= NULL;
    m_bExitThread			= FALSE;
    m_dwOprCookie			= 0; 
    
    m_dwWaitTime			= 0;
    m_dwNextObserverCookie	= 1;

    m_dwOprCookie			= 0;
	m_dwReceiver			= 0;
	m_bLogOpen				= FALSE;
}

CBootModeObj::~CBootModeObj()
{            
    m_mapObserverRegs.RemoveAll();
}

BOOL CBootModeObj::GenerateEndNotification( DWORD dwOprCookie , DWORD dwResult )
{
	DWORD dwErrorCode = 1;
	if(dwResult == 1) // success
	{
		dwErrorCode = 0;
	}
	else // get operation error code
	{
		dwErrorCode = m_OprDriver.GetOprLastErrorCode();
		if (0 == dwErrorCode)
		{
			dwErrorCode = 1;
		}
	}

    POSITION pos = m_mapObserverRegs.GetStartPosition();
    DWORD dwCookie;
    IBMOprObserver* pReg = NULL;
    while( pos ){
        pReg = NULL;
        dwCookie = 0;
        m_mapObserverRegs.GetNextAssoc( pos, dwCookie, pReg );
        if( pReg == NULL )
            continue;
        HRESULT hr = pReg->OnEnd( dwOprCookie ,dwErrorCode );
        if( FAILED(hr) )
        {
            return FALSE;
        }
		break;
    }
    return TRUE;
}

BOOL CBootModeObj::GenerateStartNotification( DWORD dwOprCookie, DWORD dwResult  )
{  
	DWORD dwErrorCode = 1; //fail
	if(dwResult == 0) // success
	{
		dwErrorCode = 0;
	}
	else
	{
		dwErrorCode = dwResult;
	}

    POSITION pos = m_mapObserverRegs.GetStartPosition();
    DWORD dwCookie;
    IBMOprObserver* pReg = NULL;
    while( pos ){
        pReg = NULL;
        dwCookie = 0;
        m_mapObserverRegs.GetNextAssoc( pos, dwCookie, pReg );
        if( pReg == NULL )
            continue;
        HRESULT hr = pReg->OnStart( dwOprCookie , dwErrorCode );
        if( FAILED(hr) )
        {
            return FALSE;
        }
		break;
    }   
    return TRUE;
}


BOOL CBootModeObj::GenerateEndFileOprNotification( DWORD dwOprCookie, 
												  LPCTSTR lpszFileId,
                                                  LPCTSTR lpszFileType,
                                                  DWORD dwResult )
{
	USES_CONVERSION;

	DWORD dwErrorCode = 1; // fail
	if(dwResult == 1) // success
	{
		dwErrorCode = 0;
	}
    
    POSITION pos = m_mapObserverRegs.GetStartPosition();
    DWORD dwCookie = 0;
    IBMOprObserver* pReg = NULL;
    while( pos ){
        dwCookie = 0;
        pReg = NULL;
        m_mapObserverRegs.GetNextAssoc( pos, dwCookie, pReg );
        if( pReg == NULL )
            continue;
        HRESULT hr = pReg->OnFileOprEnd( dwOprCookie, T2OLE((_TCHAR *)lpszFileId),T2OLE((_TCHAR *)lpszFileType), dwErrorCode );
        if( FAILED(hr) )
        {				
			return FALSE;		
        }
		break;
    }    
    return TRUE;
}

BOOL CBootModeObj::GenerateStartFileOprNotification( DWORD dwOprCookie, 
													LPCTSTR lpszFileId,
                                                    LPCTSTR lpszFileType  )
{
    USES_CONVERSION;
    
    POSITION pos = m_mapObserverRegs.GetStartPosition();
    DWORD dwCookie;
    IBMOprObserver* pReg = NULL;
    while( pos ){
        dwCookie = 0;
        pReg = NULL;
        m_mapObserverRegs.GetNextAssoc( pos, dwCookie, pReg );
        if( pReg == NULL )
            continue;
        HRESULT hr = pReg->OnFileOprStart( dwOprCookie, T2OLE((_TCHAR *)lpszFileId),
                        T2OLE((_TCHAR *)lpszFileType), (LPVOID)(&m_BMFileImpl) );
        if( FAILED(hr) )
        {
            return FALSE;
        }
		break;
    }        
    return TRUE;
}

BOOL CBootModeObj::GenerateEndOprNotification( DWORD dwOprCookie, 
											  LPCTSTR lpszFileId,
                                              LPCTSTR lpszFileType, 
                                              LPCTSTR lpszOperationType,
                                              DWORD dwResult )
{
    USES_CONVERSION;

	DWORD dwErrorCode = 1;
	if(dwResult == 1) // success
	{
		dwErrorCode = 0;
	}
	else // get operation error code
	{
		dwErrorCode = m_OprDriver.GetOprLastErrorCode();
	}
    
    POSITION pos = m_mapObserverRegs.GetStartPosition();
    DWORD dwCookie;
    IBMOprObserver* pReg = NULL;

    while( pos ){
        dwCookie = 0;
        pReg = NULL;
        m_mapObserverRegs.GetNextAssoc( pos, dwCookie, pReg );
        if( pReg == NULL )
            continue;
        HRESULT hr = pReg->OnOperationEnd( dwOprCookie,T2OLE((_TCHAR *)lpszFileId), T2OLE((_TCHAR *)lpszFileType),
                    T2OLE((_TCHAR *)lpszOperationType) , dwErrorCode , (LPVOID)(&m_BMFileImpl) );
        if( FAILED(hr) )
        {
            return FALSE;
        }
		break;
    }        
    return TRUE;
}

BOOL CBootModeObj::GenerateStartOprNotification( DWORD dwOprCookie, 
												LPCTSTR lpszFileID,
                                                LPCTSTR lpszFileType, 
                                                LPCTSTR lpszOperationType )
{
    USES_CONVERSION;
    
    POSITION pos = m_mapObserverRegs.GetStartPosition();
    DWORD dwCookie;
    IBMOprObserver* pReg = NULL;
    while( pos ){
        dwCookie = 0;
        pReg = NULL;
        m_mapObserverRegs.GetNextAssoc( pos, dwCookie, pReg );
        if( pReg == NULL )
            continue;
        HRESULT hr = pReg->OnOperationStart( dwOprCookie, T2W((_TCHAR *)lpszFileID),T2W((_TCHAR *)lpszFileType),
            T2W((_TCHAR *)lpszOperationType), (LPVOID)(&m_BMFileImpl) );
        if( FAILED(hr) )
        {
            return FALSE;
        }
		break;
    }        
    return TRUE;
}

void CBootModeObj::EndProc(  BOOL bEndSuccess  )
{
	BOOL bEndNotify = FALSE;
	DWORD dwEnd = bEndSuccess;
    DWORD dwErrorCode = m_OprDriver.GetOprLastErrorCode();
	BMFileInfo* pFileInfo = m_BMFileImpl.GetCurrentBMFileInfo();

	if (bEndSuccess)
	{
		dwErrorCode = 0;
	}

    	if (!bEndSuccess                        && 
    		BSL_USER_CANCEL != dwErrorCode      &&
   		0 != dwErrorCode)
	{
		DISPID dispid;
		CString strLog;
		dispid = m_OprDriver.GetIDsOfNames( _T("DumpUbootLog") );
		if( -1 != dispid )
		{
			BOOL bResult = m_OprDriver.Invoke( dispid, NULL ); 
			strLog.Format( _T("DumpUbootLog operation %s."),bResult ? _T("Successful") : _T("Failed") );

			if (bResult)
			{
				LogInfo( strLog );
			}
			else
			{
				LogError( strLog );
			}

		}
	}

    //When downloaded fail,RD need to power off device.
    if ( 
		!bEndSuccess                        && 
            BSL_USER_CANCEL != dwErrorCode      &&
            0 != dwErrorCode                    &&
            ohObject.IsEnablePowerOff()
       )
    {
        DISPID dispid;
        CString strLog;
        dispid = m_OprDriver.GetIDsOfNames( _T("PowerOff") );
        if( -1 != dispid )
        {
            BOOL bResult = m_OprDriver.Invoke( dispid, NULL ); 
            strLog.Format( _T("PowerOff operation %s."),bResult ? _T("Successful") : _T("Failed") );

			if (bResult)
			{
				LogInfo( strLog );
			}
			else
			{
				LogError( strLog );
			}
            
        }
        
    }

    if( m_bExitThread )
    {

		m_OprDriver.StopOpr();
		bEndNotify = GenerateEndNotification( m_dwOprCookie, dwEnd );  
		if( bEndNotify && bEndSuccess )
		{
			LogInfo( _T("Finish Successful."));
		}
		else
		{
			LogError( _T("Finish Failed."));
		}

		if (m_bLogOpen)
		{
			m_OprDriver.CloseLog( dwErrorCode, pFileInfo->szFileID );
			m_bLogOpen = FALSE;
		}
		

        return;
    }        
    
    if( m_dwWaitTime == 0 )
    {
		m_OprDriver.StopOpr();
        m_bExitThread = TRUE;
        bEndNotify = GenerateEndNotification( m_dwOprCookie, dwEnd );      
    }
    else
    {
		LogInfo( _T("Download files move to the first."));
        m_BMFileImpl.MoveFirst(); 
        ICommChannel* pChannel;
        HRESULT hr = GetCommunicateChannelPtr( (LPVOID*)&pChannel );
        if( SUCCEEDED(hr) )
        {
			DWORD dwBaud = DEFAULT_BAUDRATE;
			pChannel->GetProperty(0,CH_PROP_BAUD,&dwBaud);
			pChannel->SetProperty(0,CH_PROP_BAUD,(LPCVOID)&dwBaud);
        }
//@hongliang.xin 2010-1-25  not used 
//      dwEnd |= FACTORY_MODE;
        bEndNotify = GenerateEndNotification( m_dwOprCookie, dwEnd );  

		BOOL bRemoveDUT = FALSE;
		if (ohObject.IsUartDownload() && bEndSuccess)
		{
			DISPID dispid;
			dispid = m_OprDriver.GetIDsOfNames( _T("CheckDUT") );
			if( -1 != dispid )
			{
				bRemoveDUT = m_OprDriver.Invoke( dispid, NULL ); 
				LogInfo( _T("End of CheckDUT operation.") );
			}
		}

		if (!bRemoveDUT)
		{
			UINT uiSleepCount = m_dwWaitTime / MAX_UINT_SLEEP_TIME;
			for( unsigned int i = 0; i < uiSleepCount && !m_bExitThread; i++)
			{
				Sleep( MAX_UINT_SLEEP_TIME );        
			}
			// @hongliang.xin 2010-7-19
			if(!m_bExitThread && (m_dwWaitTime % MAX_UINT_SLEEP_TIME) != 0 )
			{
				Sleep( m_dwWaitTime % MAX_UINT_SLEEP_TIME );   
			}
		}
        
    }
    if( bEndNotify && bEndSuccess )
    {
        LogInfo( _T("Finish Successful."));
    }
    else
    {
        LogError( _T("Finish Failed."));
    }

	if (m_bLogOpen)
	{
		m_OprDriver.CloseLog( dwErrorCode, pFileInfo->szFileID );
		m_bLogOpen = FALSE;
	}

}

BOOL CBootModeObj::CreateBMThread( )
{
    LogInfo( _T("Create boot mode thread."));
    /*m_hBMThread = (HANDLE)_beginthreadex(NULL,
								0,
                                (PBEGINTHREADEX_THREADFUNC)GetBMThreadFunc,
                                this,
                                NULL,
                                NULL);*/
    m_bExitThread = FALSE;
	m_hBMThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)GetBMThreadFunc, this, 0, NULL );

    if(m_hBMThread == NULL)
    {
        return FALSE;		
    }
    

    return TRUE;
}

void CBootModeObj::DestroyBMThread()
{
    if(m_hBMThread != NULL )
    {
		m_bExitThread = TRUE;
		m_OprDriver.StopOpr();
		LogInfo( _T("End boot mode thread +++"));
        WaitForSingleObject( m_hBMThread, INFINITE ); 
        CloseHandle( m_hBMThread );
        m_hBMThread = NULL;
		LogInfo( _T("End boot mode thread ---"));
    }
}

DWORD WINAPI CBootModeObj::GetBMThreadFunc( LPVOID lpParam )
{
    CBootModeObj *pThis = (CBootModeObj *)lpParam;
    
    return pThis->BMThreadFunc();
}

DWORD CBootModeObj::BMThreadFunc( )
{
	if (!m_bLogOpen)
	{
		m_OprDriver.OpenLog( m_dwOprCookie );
		m_bLogOpen = TRUE;
	}

    m_BMFileImpl.MoveFirst();
    CString strLog;

	//add by hongliang.xin 2009-3-2
	UINT nCheckNVTimes = 0; // check nv and redownload time
	ohObject.GetProperty(0,_T("CheckNVTimes"),(LPVOID*)&nCheckNVTimes);
	UINT nReadNVCount = 0; // checked nv times, begin to check NV after downloaded all files
	BOOL bCheckNVSuccess = TRUE; // check nv success flag

	BMFileInfo* pFileInfo = m_BMFileImpl.GetCurrentBMFileInfo();
	DWORD dwBaudRate = ohObject.GetDefaultBaudrate();
	if (0 == _tcsicmp(pFileInfo->szFileType,_T("HOST_FDL")) )//RDA
	{
		dwBaudRate = DEFAULT_HOST_BAUDRATE;
	}
	ohObject.SetProperty(0,_T("RomCodeBaudRate"),(LPVOID)&dwBaudRate);

	BOOL result = m_OprDriver.Initialize(W2T((LPWSTR)m_szWProgID),
		(LPVOID)m_byteOpenArgument,
		m_bBigEndian,
		m_dwOprCookie,
		m_bRcvThread,
		(LPCVOID)&m_dwReceiver);
    
    if( !result )
    {
        GenerateStartNotification( m_dwOprCookie, BSL_REP_PORT_ERROR );
        LogError( _T("The channel occurs error. ") );
		m_bExitThread = TRUE;
		EndProc(FALSE);
        return 0;
    }

    while( !m_bExitThread )
	{
		//if( m_BMFileImpl.GetCurFileIndex() == 0)
		//	m_OprDriver.OnBegin();

		if ( m_BMFileImpl.GetCurFileIndex() == 0 )
		{
			if (!m_bLogOpen)
			{
				m_OprDriver.OpenLog( m_dwOprCookie );
				m_bLogOpen = TRUE;
			}
		}

        if( m_BMFileImpl.GetCurFileIndex() == 0 && !GenerateStartNotification( m_dwOprCookie, 0 )) 
        {
			LogError( _T("Fail at start notification.") );
			EndProc( FALSE );            
			continue;
        }

		BMFileInfo* pFileInfo = m_BMFileImpl.GetCurrentBMFileInfo();

        if( !GenerateStartFileOprNotification( m_dwOprCookie, pFileInfo->szFileID, pFileInfo->szFileType) )
        {
            GenerateEndFileOprNotification( m_dwOprCookie, pFileInfo->szFileID,pFileInfo->szFileType ,FALSE);
            strLog.Format( _T("Fail at start [ID:%s] %s  file notification"),pFileInfo->szFileID,pFileInfo->szFileType );
            LogError( strLog );
            EndProc( FALSE );           
            continue;
        }

		CStringArray aryKeyAndData;
        if( !ohObject.GetFileOperations( pFileInfo->szFileType,&aryKeyAndData ) )   
        {
            strLog.Format( _T("Can not read file operations for file type %s "), pFileInfo->szFileType );
            LogError( strLog );
            EndProc( FALSE );
            continue;
        }
		else
		{
			strLog.Format( _T("[INFO] FileID=%s, operations for file type %s "), pFileInfo->szFileID,pFileInfo->szFileType );
			LogInfo( strLog );
		}
		
        int nInterval = ohObject.GetInterval(pFileInfo->szFileType);
        if ( 0 != nInterval)
        {
            Sleep(nInterval);
        }

        DWORD dwMaxLength = ohObject.GetPacketLength( pFileInfo->szFileType );	
		m_BMFileImpl.SetCurMaxLength( dwMaxLength );		
           
		BOOL result = TRUE;  
		BOOL bIsCheckNV = FALSE;
        
		if(_tcsicmp(pFileInfo->szFileType,_T("CHECK_NV")) == 0)
		{
			bIsCheckNV = TRUE;
			nReadNVCount = 0;
			bCheckNVSuccess = TRUE;
		}

		int i=0;
		
        for( i = 0; i < aryKeyAndData.GetSize(); i+=2 )
        { 
			if(bIsCheckNV)
			{
				if(nCheckNVTimes == 0) {result = TRUE; break;}
				if(0 == i) {bCheckNVSuccess = TRUE; nReadNVCount++;}	
			}			

            CString str;
            
            //OLECHAR FAR* szName = aryKeyAndData.GetAt( i + 1).AllocSysString() ;
			CString strName = aryKeyAndData.GetAt( i + 1);
			strName.TrimLeft();
			strName.TrimRight();
            DISPID dispid;
            
            dispid = m_OprDriver.GetIDsOfNames( strName );
            if( dispid == -1 )
            {
                strLog.Format( _T("%s---%s operation is not implemented"), 
                    pFileInfo->szFileType, aryKeyAndData.GetAt( i + 1 ));
                LogError( strLog );
                continue;
            }

            if( !GenerateStartOprNotification( m_dwOprCookie,pFileInfo->szFileID,
                pFileInfo->szFileType, aryKeyAndData.GetAt( i + 1 ) ) )
            {
                GenerateEndOprNotification( m_dwOprCookie, pFileInfo->szFileID,
                    pFileInfo->szFileType, aryKeyAndData.GetAt( i + 1 ) , FALSE ) ;
                strLog.Format( _T("Fail at start [ID:%s]%s---%s operation"), 
                    pFileInfo->szFileID, pFileInfo->szFileType, aryKeyAndData.GetAt( i + 1 ) );
                LogError( strLog );
                break;
            }
                
            result = m_OprDriver.Invoke( dispid, (void*)pFileInfo );   
            
            if( !GenerateEndOprNotification( m_dwOprCookie,pFileInfo->szFileID, 
                pFileInfo->szFileType, aryKeyAndData.GetAt( i + 1 ) , result ) )
            {				
				strLog.Format( _T("Fail at end %s---%s operation"), 
					pFileInfo->szFileType, aryKeyAndData.GetAt( i + 1 ) );
				LogError( strLog ); 
				
				if(0 == i && bIsCheckNV) //read flash and check NV failed
				{			
					bCheckNVSuccess = FALSE;
					strLog.Format( _T("Check NV failed and download %s again"), pFileInfo->szFileID );
					LogInfo( strLog ); 
				}
				else
				{
					break;
				}	
            }
            else
            {
                //if (0 == strName.CompareNoCase(_T("CheckRoot")) && !result)
                {
                    result = TRUE;
                }
               
            }

            if( !result )
            {				
                strLog.Format( _T("Fail at %s---%s operation"), 
                    pFileInfo->szFileType, aryKeyAndData.GetAt( i + 1 ) );
                LogError( strLog );
			
				if( bIsCheckNV && (nReadNVCount <= nCheckNVTimes))			
				{
					// read or download NV failed
					// recheck and download again					
					strLog.Format( _T("Recheck and download %s again"), pFileInfo->szFileID );
				    LogInfo( strLog );
					if( i == 2 ) //download
					{
						i = -2; // i= i+2;  turn back
					}
				}
				else
				{
					break;   
				}				 
            }	
			else
			{
			    /**
				 * @hongliang.xin 2009-3-2
				 * "i==0  && bCheckNVSuccess" means read NV and check successs
				 * notice: maybe uplevel need not backup NV, so callback always return S_OK
				 * so that there is no check action in uplevel actually
				 */
				if(bIsCheckNV)
				{	
					if (0 == i && bCheckNVSuccess)
					{
						i=aryKeyAndData.GetSize(); // set NV BMFiles process over		   
						strLog.Format( _T("Check %s success"), pFileInfo->szFileID );
						LogInfo( strLog ); 
						break; //check success, end the rest process
					}

					if( 2 == i && (nReadNVCount <= nCheckNVTimes) )
					{
						i = -2; // i= i+2;  turn back
					}
					
				}//else bCheckNVSuccess == false, download again
			}
			
        }

        BOOL bSuccessful = TRUE;
        if( i < aryKeyAndData.GetSize())
        {
            bSuccessful = FALSE;
        }

        if( !GenerateEndFileOprNotification( m_dwOprCookie, pFileInfo->szFileID,
                    pFileInfo->szFileType, bSuccessful )  )
        {
            strLog.Format( _T("Fail at end %s file notification"), 
                pFileInfo->szFileType );
            LogError( strLog );   
            EndProc( FALSE );
            continue;
        }

        if( !bSuccessful )
        {
            EndProc( FALSE );
            continue;
        }
       
		if( ohObject.IsEnablePortSecondEnum() &&           
			_tcsicmp(pFileInfo->szFileID,_T("FDLA")) == 0 &&
			_tcsicmp(pFileInfo->szFileType,_T("FDL1")) == 0 &&
            m_OprDriver.IsNeed2ndEnumPort() )
		{
			EndProc( TRUE );
			continue;
		}
        
        m_BMFileImpl.MoveNext();

        if( m_BMFileImpl.IsEOF() )
        {
            // All file have been downloaded
            EndProc( TRUE );		
        }
        
	}	
    
    return 0L;
}

// Format of pSection
// "Key1 = data1\0Key2 = data2\0....Keyn = datan\0\0"
int CBootModeObj::EnumKeys(char* pSection,CStringArray* pKeys)
{
    ASSERT(pSection != NULL);
    ASSERT(pKeys != NULL);
    
    int number = 0;
    while(1)
    {
        CString strLine = pSection;
        int nLen = strLine.GetLength();
        if(0 == nLen)
        {
            // All keys are read
            break;
        }
        
        int nIndex = strLine.Find('=');
        if(-1 == nIndex)
        {
            // Not a valid key
            continue;
        }
        CString strKey = strLine.Left(nIndex);
        strKey.TrimLeft();
        strKey.TrimRight();
        pKeys->Add(strKey);
        
        CString strData = strLine.Right(nLen - nIndex - 1);
        strData.TrimLeft();
        strData.TrimRight();
        pKeys->Add(strData);
        
        number++;
        
        pSection += nLen + 1;        
    }
    
    return number;
}

void CBootModeObj::LogInfo(LPCTSTR lpszLog) 
{
	m_OprDriver.LogInfo( lpszLog );
}

void CBootModeObj::LogError(LPCTSTR lpszLog) 
{
	m_OprDriver.LogError( lpszLog );
}

HRESULT CBootModeObj::StartBootModeOperation(   LPVOID lpBMFileInfo, 
                                                     UINT uFileCount, 
                                                     LPVOID pOpenArgument, 
                                                     BOOL bBigEndian, 
                                                     DWORD dwOprCookie,
                                                     BOOL  bRcvThread, 
                                                     LPCVOID pReceiver,
                                                     LPCWSTR lpbstrProgID)
{
    USES_CONVERSION;

    ASSERT( lpBMFileInfo != NULL );
    ASSERT( uFileCount > 0 );    
    
    m_dwOprCookie = dwOprCookie;    
 
    /*BOOL result = m_OprDriver.Initialize(W2T((LPWSTR)lpbstrProgID),
										pOpenArgument,
										bBigEndian,
										dwOprCookie,
										bRcvThread,
										pReceiver);
    
    if( !result )
    {
        GenerateStartNotification( m_dwOprCookie, 0 );
        LogInfo( _T("The channel occurs error. ") );
        return BM_E_CHANNEL_FAILED;
    }*/
	m_bBigEndian = bBigEndian;
	memcpy(m_byteOpenArgument,(BYTE*)pOpenArgument,8);
	_tcscpy(m_szWProgID,lpbstrProgID);
	m_bRcvThread = bRcvThread;
	m_dwReceiver = *(DWORD*)pReceiver;

	if(!m_BMFileImpl.InitBMFiles( (PBMFileInfo)lpBMFileInfo,uFileCount))
    {
        GenerateStartNotification( m_dwOprCookie, 0 );
		CString str = m_BMFileImpl.GetLastErrMsg();
		LogError(str);
        LogError( _T("The using file( FDL, NV ,etc ) is error. ") );	
        return BM_E_FILEINFO_ERROR;
    } 
	else
	{
		 LogInfo( _T("load file success.") );
	}
 
    if(!CreateBMThread( ))
    {
        GenerateStartNotification( m_dwOprCookie, 0 );
        LogError( _T("Create download  thread fail. ") );   
		m_OprDriver.Uninitialize();
        m_BMFileImpl.ClearUpBMFiles();
        return BM_E_CREATETHREAD_FAILED;
    }
    
    LogInfo( _T("Start Boot Mode Operation success.") ); 
    return BM_S_OK;    
}

HRESULT CBootModeObj::StopBootModeOperation()
{    
    LogInfo( _T("Stop Boot Mode Operation +++") );    
    DestroyBMThread();

	m_OprDriver.Uninitialize();

    m_BMFileImpl.ClearUpBMFiles(); 
	LogInfo( _T("Stop Boot Mode Operation ---") ); 
    return BM_S_OK;
}

HRESULT CBootModeObj::SetWaitTimeForNextChip( DWORD dwWaitTime /* = CHANGE_CHIP_TIMEOUT  */)
{
    m_dwWaitTime = dwWaitTime ;
    
    return BM_S_OK;
}

HRESULT CBootModeObj::SetCommunicateChannelPtr( LPVOID pCommunicateChannel  )
{
	m_OprDriver.SetChannel((long)pCommunicateChannel);    
    return S_OK;
}

HRESULT CBootModeObj::GetCommunicateChannelPtr( LPVOID* ppCommunicateChannel )
{
    long result = m_OprDriver.GetChannelPtr();
    *ppCommunicateChannel = (LPVOID)result;

    if( result ==  NULL )
    {
        return BM_E_FAILED;
    }
    return BM_S_OK ;
}

HRESULT CBootModeObj::SubscribeOperationObserver( IBMOprObserver* pSink, 
                                                   ULONG uFlags,
                                                   LPDWORD lpdwCookie )
{  
	UNUSED_ALWAYS(uFlags);
    CSingleLock _lock( &m_CS, TRUE);
    
    DWORD dwCookie = m_dwNextObserverCookie;
    IBMOprObserver* p = NULL;
    do{
        p = NULL;
        if( !m_mapObserverRegs.Lookup( dwCookie, p) )
            break;
        dwCookie++;
        if( dwCookie == 0 )
            dwCookie++;
    }while( dwCookie != m_dwNextObserverCookie );
    
    if( p != NULL )
    {
        LogError( _T("Subscribe Operation Observer fail."));
        return BM_E_REG_OBSERVER_FAILED;
    }
    
    m_mapObserverRegs.SetAt( dwCookie, pSink );
    //pSink->AddRef();

    m_dwNextObserverCookie = dwCookie + 1;
    if( m_dwNextObserverCookie == 0 )
        m_dwNextObserverCookie++;
    
    *lpdwCookie = dwCookie;

    LogInfo( _T("Subscribe Operation Observer success."));
    return BM_S_OK;
}

HRESULT CBootModeObj::UnsubscribeOperationObserver( THIS_ DWORD dwCookie )
{
    CSingleLock _lock( &m_CS, TRUE);

    IBMOprObserver* pObserver;
    if( !m_mapObserverRegs.Lookup( dwCookie, pObserver ) )
        return BM_E_FAILED;
	
    //pObserver->Release();

    m_mapObserverRegs.RemoveKey( dwCookie );
	/* can not invoke log, maybe some object is not created */
    // LogInfo( _T("Unsubscribe Operation Observer."));

    return BM_S_OK;
}

const LPBYTE CBootModeObj::GetReadBuffer( THIS )
{    
    long rlt = m_OprDriver.GetRecvBuffer();    
    return (const LPBYTE)rlt;
}

DWORD CBootModeObj::GetReadBufferSize( THIS )
{
    long dwSize = m_OprDriver.GetRecvBufferSize(); 
    return dwSize;    
}
void CBootModeObj::EnablePortSecondEnum(BOOL bEnable)
{
	m_OprDriver.EnablePortSecondEnum(bEnable);
}
