// BMAFImp.cpp: implementation of the CBMAFImp class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <atlconv.h>
#include <initguid.h>
#include <dbt.h>

#pragma warning(push,3)
#include <vector>
#include <algorithm>
#pragma warning(pop)

#include "BMAFImp.h"
#include "BMAFrame.h"
#include "XmlConfigParse.h"
//#include "BootModeguid.h"
#include "BMFile.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

//#define  NEW_TD_DLOADER

#ifdef NEW_TD_DLOADER
#define ADD_DOWNLOAD_LENGTH       512
#else
#define ADD_DOWNLOAD_LENGTH       8
#endif

//#define MAX_RET_FILE_NUM 5

static BOOL GetAbsolutePath(CString &strAbsoluteFilePath,LPCTSTR lpszFilePath )
{
    TCHAR szFileName[_MAX_FNAME];    
    TCHAR szDir[_MAX_DIR];
    TCHAR szDirve[_MAX_DRIVE];	
    
    _tsplitpath( lpszFilePath,szDirve,NULL,NULL,NULL);	
    if(szDirve[0] != _T('\0') || (_tcslen(lpszFilePath)>2 && lpszFilePath[0] == _T('\\') && lpszFilePath[1] == _T('\\')))
	{		
		strAbsoluteFilePath = lpszFilePath;
    }
	else
	{
		//do it if strHelpTopic is ralatively 
		GetModuleFileName( AfxGetApp()->m_hInstance, szFileName,_MAX_FNAME);
		_tsplitpath( szFileName , szDirve , szDir , NULL , NULL );	
		strAbsoluteFilePath = szDirve;
		strAbsoluteFilePath += szDir;
		if( lpszFilePath[0] == _T('\\') || lpszFilePath[0] == _T('/') )
			lpszFilePath++;

		strAbsoluteFilePath += lpszFilePath;
        
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBMAFImp::CBMAFImp()
{
	m_arrFile.RemoveAll();
	m_strProductName.Empty();
	m_arrFileInfo.clear();
	
	m_nFileCount = 0 ;
	m_nBMFileCount = 0;
	m_arrBMFileInfo.clear();
	m_mapObserverRegs.RemoveAll();
	m_bInitBMFiles = FALSE;

	m_dwWaitTime = 0;
	m_dwRepartitionFlag = 1;
    m_bLog = HasLog();
	m_strSpecConfig = _T("");

	m_dwReadFlashBRFlag = 0;

    m_strFileType = _T("");

	m_dwPacketLength = 0x1000;

	m_arrBMFileInfoEx.clear();
	m_bEnablePortSecondEnum = FALSE;
    m_bPowerOff             = FALSE;
	m_bUartDownload			= FALSE;
	m_strLogPath.Empty();
	if(m_bLog)
	{
		m_bLog = StartLog();
	}
}

CBMAFImp::~CBMAFImp()
{
	BMAF_StopAllWork();
}
/**
 * stop one port work
 *
 * @param lpszProductName: the product name; 
 * @param ppszFileList: the download files; 
 * @param dwFileCount: the number of download files; 
 * @param pOpenArgument: the download channel(port) param;
 * @param bBigEndian: if bigendian; 
 * @param dwOprCookie: the port identify;
 * @param bRcvThread: 
 * @param pReceiver: 
 * @param lpbstrProgID: 
 *
 * @return Returns S_OK if successful,Otherwise returns error code;
 */ 
HRESULT CBMAFImp::BMAF_StartOneWork(	LPCTSTR lpszProductName, 
										LPCTSTR *ppszFileList,
										DWORD   dwFileCount,
										LPVOID  pOpenArgument,
										BOOL    bBigEndian, 
										DWORD   dwOprCookie,
										BOOL    bRcvThread, 
										LPCVOID pReceiver,
										LPCWSTR lpbstrProgID )
{
	if(m_bLog && !m_log.IsOpenLog())
	{
		if(!StartLog())
			return BMAF_E_STRART_LOG_FAIL;
	}

//     if( FAILED( CoInitialize(NULL) ) )
//     {
// 		if(m_bLog && m_log.IsOpenLog())
// 			m_log.XLog(_T("COM Initialize Failed"));
// 		return BMAF_E_COM_INIT_FAIL;
// 	}

	if(!RegBootModeObj(dwOprCookie))
		return BMAF_E_REG_BMOBJ_FAIL;

	if(!RegBMPObserver(dwOprCookie))
		return BMAF_E_REG_BMPOBSERVER_FAIL;

    if(!m_bInitBMFiles)
	{
		m_strProductName = lpszProductName;
		m_arrFile.RemoveAll();
		
		for(UINT i= 0;i< dwFileCount;i++)
			m_arrFile.Add(ppszFileList[i]);
		
		m_nFileCount = (UINT) dwFileCount;	

		if(!InitBMFiles(dwOprCookie))
		  return BMAF_E_INIT_BMFILES_FAIL;
		m_bInitBMFiles = TRUE;
	}

	if(m_nBMFileCount == 0)
		return S_OK;

	if(!InitBootFileInfo(dwOprCookie))
		  return BMAF_E_INIT_BMFILES_FAIL;

	return StartOneWork(bBigEndian, 
						pOpenArgument,
						dwOprCookie,
						bRcvThread, 
						pReceiver,
						lpbstrProgID );
	
	

}

/**
 * stop all port work
 *
 * @return Returns S_OK if successful,Otherwise returns S_FAIL;
 */ 
HRESULT CBMAFImp::BMAF_StopAllWork()
{
    POSITION pos = m_mapBootModeObj.GetStartPosition();
    DWORD dwCookie = 0;
	
    PBOOTMODEOBJ_T  pStruct = NULL;
    while( pos )
    {
        m_mapBootModeObj.GetNextAssoc( pos, dwCookie, pStruct );
		if(pStruct->pSnapin)
		{
			pStruct->pSnapin->StopBootModeOperation();    
			pStruct->pSnapin->UnsubscribeOperationObserver( pStruct->dwCookie );

			if(pStruct->pSnapin)
			{
				if(g_theApp.m_pfReleaseBMObject)
				{
					g_theApp.m_pfReleaseBMObject(pStruct->pSnapin);
				}		
				pStruct->pSnapin = NULL;
			}
		}
		if ((pStruct->BootFileInfo).pBMFileInfo)
		{
			UINT  nCount = (pStruct->BootFileInfo).dwBMFileCount;
			for(UINT i = 0; i < nCount ; i++ )
			{
				UnloadBMFileInfo( (pStruct->BootFileInfo).pBMFileInfo+i,m_arrBMFileFlag[i] );	
			}
			SAFE_DELETE_ARRAY((pStruct->BootFileInfo).pBMFileInfo);
			(pStruct->BootFileInfo).bInitBMFiles = FALSE;
		}
        delete pStruct;
    }
	
    m_mapBootModeObj.RemoveAll();
	ClearBMFiles();	

	m_arrFile.RemoveAll();
	m_nFileCount =0;
    
	int nCount = m_arrFileInfo.size();
	for(int i=0;i<nCount;i++)
	{
		delete [] m_arrFileInfo[i];
	}
	m_arrFileInfo.clear();
	m_bInitBMFiles = FALSE;	
	StopLog();
	return S_OK;
}

/**
 * stop one port work
 *
 * @param dwOprCookie: the port identify; 
 *
 * @return Returns S_OK if successful,Otherwise returns S_FAIL;
 */ 
HRESULT CBMAFImp::BMAF_StopOneWork( DWORD dwOprCookie )
{
	PBOOTMODEOBJ_T pBMO = NULL;
	HRESULT hr;
	hr = BMAF_GetBootModeObjInfo(dwOprCookie,(LPVOID*)&pBMO);
	if(FAILED(hr))
		return hr;

	if( !pBMO->bStop )
    {        
        pBMO->bStop = TRUE;
        pBMO->pSnapin->StopBootModeOperation();       
        pBMO->pSnapin->UnsubscribeOperationObserver( pBMO->dwCookie );          
        
    }
	return S_OK;
}

/**
 * Get struct BOOTMODEOBJ_T information
 *
 * @param dwOprCookie: the port identify; 
 * @param ppBootModeObjT: the pointer of struct BOOTMODEOBJ_T
 *
 * @return Returns S_OK if successful,Otherwise returns S_FAIL;
 */   
HRESULT CBMAFImp::BMAF_GetBootModeObjInfo(DWORD dwOprCookie, LPVOID *ppBootModeObjT)
{
	PBOOTMODEOBJ_T pBMO = NULL;
	if(m_mapBootModeObj.Lookup(dwOprCookie,pBMO))
		*ppBootModeObjT = (LPVOID)pBMO;
	else
		*ppBootModeObjT = 0;

	return S_OK;
}

/**
 * Register BootmodePlatform DLL
 *
 * @param lpszDllName: DLL name; 
 *
 * @return Returns S_OK if successful,Otherwise returns error code;
 */   
HRESULT CBMAFImp::BMAF_RegisterBMPDll(LPCTSTR lpszDllName)
{
	UNREFERENCED_PARAMETER(lpszDllName);
/*	CString strErrMsg;
	if(m_bLog && !m_log.IsOpenLog())
		StartLog();
	
	BOOL bRegSuccess = TRUE;
    HRESULT hr = OleInitialize( NULL );
    if( SUCCEEDED(hr) )
    {
        HINSTANCE hLib = LoadLibrary( lpszDllName );
		
        if ( hLib >= (HINSTANCE)HINSTANCE_ERROR)
        {
            FARPROC lpDllEntryPoint = NULL;
            // Find the entry point.
            (FARPROC&)lpDllEntryPoint = GetProcAddress( hLib, "DllRegisterServer");
            if(lpDllEntryPoint != NULL)
            {
                hr = (*lpDllEntryPoint)();
                if( FAILED( hr ) )
                {
					if(m_bLog && m_log.IsOpenLog())
					{
						strErrMsg.Format( _T("Call DllRegisterServer fail (%s).[ErrorCode:0x%08X]"), lpszDllName,hr );
						m_log.XLog( strErrMsg );
					}
					bRegSuccess = FALSE;
                }
            }
            else
            {
				 if(m_bLog && m_log.IsOpenLog())
				 {
					 strErrMsg.Format(_T("Get process address  fail %s."),lpszDllName);
					 m_log.XLog( strErrMsg );
				 }
				 bRegSuccess = FALSE;
            }            
        }
        else
        {
			if(m_bLog && m_log.IsOpenLog())
			{
				strErrMsg.Format(_T("Load %s fail."),lpszDllName);
				m_log.XLog( strErrMsg );
			}
			bRegSuccess = FALSE;
        }
		
        OleUninitialize( );	 
		if(!bRegSuccess)
			return BMAF_E_REG_DLL_FAIL;
    }
    else
    {
		if(m_bLog && m_log.IsOpenLog())
		{
			strErrMsg = _T("Call OleInitialize() fail.");
			m_log.XLog( strErrMsg );
		}
        return BMAF_E_COM_INIT_FAIL;
    }*/
	return S_OK;
}
/**
 * Unregister BootmodePlatform DLL,not implement
 *
 * @param lpszDllName: DLL name; 
 *
 * @return Returns S_OK if successful,Otherwise returns error code;
 */   
HRESULT CBMAFImp::BMAF_UnregisterBMPDll(LPCTSTR lpszDllName)
{
	UNUSED_ALWAYS(lpszDllName);
	return S_OK;
}

/**
 * Subscribe Callback observer
 *
 * @param dwOprCookie: the port identify; 
 * @param pSink: pointer of IBMOprObserver; 
 * @param uFlags: not used for extent future; 
 *
 * @return Returns S_OK ;
 */  
HRESULT CBMAFImp::BMAF_SubscribeObserver( DWORD dwOprCookie,
										  IBMOprObserver* pSink, 
									      ULONG uFlags)

{
	UNUSED_ALWAYS(uFlags);

    CSingleLock _lock( &m_CS, TRUE);
	IBMOprObserver* pBMOprObs = NULL;
	if( m_mapObserverRegs.Lookup( dwOprCookie, pBMOprObs) )
	{
        return S_OK;   // already subscribe 
    }

	m_mapObserverRegs.SetAt( dwOprCookie, pSink );
    //pSink->AddRef();


    if(m_bLog && !m_log.IsOpenLog())
	    StartLog();
	if(m_bLog && m_log.IsOpenLog())
	{
		CString strMsg;
		strMsg.Format(_T("[PORT:%d] Subscribe Operation Observer succeed."),dwOprCookie);
		m_log.XLog( strMsg );
	}
	return S_OK;
}
/**
 * Unsubscribe Callback observer
 *
 * @param dwOprCookie: the port identify; 
 *
 * @return Returns S_OK;
 */  
HRESULT CBMAFImp::BMAF_UnsubscribeObserver(DWORD dwOprCookie)
{
	CSingleLock _lock( &m_CS, TRUE);
	
    IBMOprObserver* pObserver;
    if( !m_mapObserverRegs.Lookup( dwOprCookie, pObserver ) )
        return BM_E_FAILED;
	
    //pObserver->Release();
    m_mapObserverRegs.RemoveKey( dwOprCookie );
    //Log( "Unsubscribe Operation Observer.");
	return S_OK;
}
/**
 * Set property for download
 *
 * @param dwPropertyID: the property identify;
 * @param dwPropertyValue: the property value; 
 *
 * @return Returns S_OK if successful,Otherwise returns S_FALSE;
 */  
HRESULT CBMAFImp::BMAF_SetProperty(DWORD dwPropertyID, LPCVOID pPropertyValue)
{
	switch(dwPropertyID) {
	case BMAF_TIME_WAIT_FOR_NEXT_CHIP:
		m_dwWaitTime = *(DWORD*)pPropertyValue;
		break;
	case BMAF_NAND_REPARTION_FLAG:
		m_dwRepartitionFlag = *(DWORD*)pPropertyValue;
		break;
	case BMAF_SPECIAL_CONFIG_FILE:
		m_strSpecConfig = (LPCTSTR)pPropertyValue;
		break;
	case BMAF_READ_FLASH_BEFORE_REPARTITION:
		m_dwReadFlashBRFlag = *(DWORD*)pPropertyValue;
		break;
	case BMAF_SPEC_FILE_TYPE:
		m_strFileType  = (LPCTSTR)pPropertyValue;
		break;
	case BMAF_ENABLE_PORT_SECOND_ENUM:
		m_bEnablePortSecondEnum = *(BOOL*)pPropertyValue;
        break;
    case BMAF_POWER_OFF_DEVICE:
        m_bPowerOff = *(BOOL*)pPropertyValue;
        break;
	case BMAF_PROP_LOG_PATH:
		m_strLogPath  = (LPCTSTR)pPropertyValue;
	case BMAF_UART_MODE_DOWNLOAD:
		m_bUartDownload = *(BOOL*)pPropertyValue;
		break;
	default:
		return S_FALSE;
		break;
	}	

	return S_OK;
}
/**
 * Get property for download
 *
 * @param dwPropertyID: the property identify;
 * @param dwPropertyValue: the property value to store; 
 *
 * @return Returns S_OK if successful,Otherwise returns S_FALSE;
 */ 
HRESULT CBMAFImp::BMAF_GetProperty(DWORD dwPropertyID, LPVOID* lpPropertyValue)
{
	switch(dwPropertyID) {
	case BMAF_TIME_WAIT_FOR_NEXT_CHIP:
		*lpPropertyValue = (void*)&m_dwWaitTime;
		break;
	case BMAF_NAND_REPARTION_FLAG:
		*lpPropertyValue =  (void*)&m_dwRepartitionFlag;
		break;
	case BMAF_SPECIAL_CONFIG_FILE:
		*lpPropertyValue = (void*)(LPCTSTR)m_strSpecConfig;
		break;
	case BMAF_READ_FLASH_BEFORE_REPARTITION:
		*lpPropertyValue = (void*)&m_dwReadFlashBRFlag;
		break;
	case BMAF_SPEC_PACKET_LENGTH:
        *lpPropertyValue = (void*)&m_dwPacketLength;
		break;
	default:
		return S_FALSE;
		break;
	}	
	
	return S_OK;
}
/**
 * Set Communicate channel pointer
 *
 * @param dwOprCookie: the port identify;
 * @param pCommunicateChannel: Communicate channel pointer; 
 *
 * @return Returns S_OK if successful,Otherwise returns S_FALSE;
 */ 
HRESULT CBMAFImp::BMAF_SetCommunicateChannelPtr(DWORD dwOprCookie,
												LPVOID pCommunicateChannel )
{
    PBOOTMODEOBJ_T pBMO = NULL;
	BMAF_GetBootModeObjInfo(dwOprCookie,(LPVOID*)&pBMO);
	if(NULL == pBMO)
		return S_FALSE;
	return pBMO->pSnapin->SetCommunicateChannelPtr(pCommunicateChannel);
}
/**
 * Get Communicate channel pointer
 *
 * @param dwOprCookie: the port identify;
 * @param pCommunicateChannel: Communicate channel pointer; 
 *
 * @return Returns S_OK if successful,Otherwise returns S_FALSE;
 */ 
HRESULT CBMAFImp::BMAF_GetCommunicateChannelPtr(DWORD dwOprCookie, 
												/*[out]*/LPVOID* ppCommunicateChannel )
{
    PBOOTMODEOBJ_T pBMO = NULL;
	BMAF_GetBootModeObjInfo(dwOprCookie,(LPVOID*)&pBMO);
	if(NULL == pBMO)
		return S_FALSE;
	return pBMO->pSnapin->GetCommunicateChannelPtr(ppCommunicateChannel);
}
/**
 * Release this
 *
 * @return Returns S_OK;
 */ 
HRESULT CBMAFImp::BMAF_Release()
{
	delete this;
	return S_OK;
}
/////////////////////////////////////////////////////

HRESULT CBMAFImp::StartOneWork(	BOOL bBigEndian, 
							   LPVOID pPortArgument,
							   DWORD dwOprCookie,
							   BOOL bRcvThread, 
							   LPCVOID pReceiver,
					           LPCWSTR lpbstrProgID )
{
    HRESULT hr = 0;
	IBootModeHandler* pBootModehandler = m_pBootModeObj->pSnapin;
    IBMSettings * pSettings = (IBMSettings *)m_pBootModeObj->pSnapin;

    // Set repartition flags,only used by nand flash
    pSettings->SetRepartitionFlag( m_dwRepartitionFlag );    
	pSettings->SetReadFlashBefRepFlag( m_dwReadFlashBRFlag );
	pSettings->SetProperty(0,_T("PRODUCT"), (LPVOID)(LPTSTR)(LPCTSTR)m_strProductName);
	pSettings->SetProperty(0,_T("EnablePortSecondEnum"), (LPVOID)&m_bEnablePortSecondEnum);
    pSettings->SetProperty(0,_T("PowerOff"), (LPVOID)&m_bPowerOff);
	pSettings->SetProperty(0,_T("UartDownload"), (LPVOID)&m_bUartDownload);
	
	pSettings->SetProperty(0,_T("LogPath"),(LPVOID)(LPCTSTR)m_strLogPath);

	if(!m_strFileType.IsEmpty())
	{
		m_dwPacketLength = pSettings->GetPacketLength( (LPTSTR)(LPCTSTR)m_strFileType );
	}
	else
	{
		m_dwPacketLength = 0x1000;
	}

	BMFileInfo *pBMFileInfo = new BMFileInfo[m_nBMFileCount];
	if(pBMFileInfo == NULL)
		return BMAF_E_OUTOFMEMORY;
	memset(pBMFileInfo,0,m_nBMFileCount*sizeof(BMFileInfo));
// 	for(UINT i=0;i<m_nBMFileCount;i++)
// 	{
// 		memcpy(pBMFileInfo+i,m_arrBMFileInfo[i],sizeof(BMFileInfo));
// 	}
	if( !m_mapBootModeObj.Lookup(dwOprCookie, m_pBootModeObj) || NULL == m_pBootModeObj)
	{
		return BMAF_E_REG_BMOBJ_FAIL;
	}
	
	for(UINT i=0;i<m_nBMFileCount;i++)
	{
		 memcpy(pBMFileInfo+i,(m_pBootModeObj->BootFileInfo).pBMFileInfo+i,sizeof(BMFileInfo));
 	}
    hr = pBootModehandler->StartBootModeOperation( (LPVOID)pBMFileInfo, 
											(DWORD)m_nBMFileCount,
											pPortArgument, 
											bBigEndian, 
											dwOprCookie,
											bRcvThread, 
											pReceiver,
											lpbstrProgID );

	if(SUCCEEDED( hr ))
	{
		pBootModehandler->SetWaitTimeForNextChip( m_dwWaitTime );
	}
	else
	{
		UnregBMPObserver(dwOprCookie);
	}

	delete []pBMFileInfo;

	if(m_pBootModeObj != NULL)
		m_pBootModeObj->bStop = FALSE;
    
	return hr;
}


//////////////////////////////////////////////
BOOL CBMAFImp::InitBMFiles(DWORD dwOprCookie)
{
    USES_CONVERSION;
	
	if(m_nFileCount == 0)
		return TRUE;		

	CXmlConfigParse xcp;

	_TCHAR szConfigFile[MAX_PATH];
	GetModuleFilePath( NULL, szConfigFile );
	int nLen  = _tcslen(szConfigFile);
	_tcscat( szConfigFile,  _T("\\config\\BMAConfig.xml") );	
	CFileFind finder;
	if(!finder.FindFile(szConfigFile))
	{
		szConfigFile[nLen] = '\0';
		_tcscat( szConfigFile,  _T("\\BMAConfig.xml") );
	}

	if(finder.FindFile(szConfigFile))
	{
		if( !xcp.Init(szConfigFile))
		{
			if(m_bLog && m_log.IsOpenLog())
			{
				m_log.XLog(_T("Read \"BMAConfig.xml\" file failed!"));
			}
			return FALSE;
		}
	}

	if(!m_strSpecConfig.IsEmpty())
	{
		if( !xcp.Init(m_strSpecConfig,1))
		{
			if(m_bLog && m_log.IsOpenLog())
			{
				m_log.XLog(_T("Read xml configure file failed!"));
			}	
			return FALSE;
		}
	}

	UINT nCount =0;
	UINT nRealCount = 0;
	UINT i=0;

	PFILE_INFO_T pFileInfo = NULL;
	PPRODUCT_INFO_T pProd = NULL;
	pProd = xcp.GetProdInfo(m_strProductName);
	if(pProd == NULL)
	{
		if(m_bLog && m_log.IsOpenLog())
		{
			m_log.XLog(_T("Not found product configure!"));
		}
	
		return FALSE;
	}	

	BOOL bMultiFiles = FALSE;
	if(pProd->tChips.bEnable && pProd->tChips.dwCount != 0)
	{
		bMultiFiles = TRUE;
	}

	nCount = pProd->dwFileCount;
	for(i=0;i<nCount;i++)
	{
		pFileInfo = new FILE_INFO_T;
		memcpy(pFileInfo,pProd->pFileInfoArr + i,sizeof(FILE_INFO_T));
		if(pFileInfo->dwFlag != FILE_OMIT_FALG)
		{
			m_arrFileInfo.push_back(pFileInfo);
			++nRealCount;
		}
		else
		{
			delete pFileInfo;
			pFileInfo = NULL;
		}
	}

	m_nBMFileCount = nRealCount;

	//输入的文件个数应该与配置文件的个数减去忽略的文件后的个数相同	
	if(m_nFileCount != m_nBMFileCount )
	{
		if(m_bLog && m_log.IsOpenLog())
			m_log.XLog(_T("The number of input files not match the number of files in xml config minusing the omitting files "));
		return FALSE;
	}
	
	nCount =0;
	i=0;
	
	IBMOprObserver *pObserver;	

	
    pFileInfo = NULL;
	BMFileInfo * pBMFileInfoArr = new BMFileInfo[MAX_RET_FILE_NUM];
	memset(pBMFileInfoArr,0,MAX_RET_FILE_NUM * sizeof(BMFileInfo));
	BMFileInfo * pBMFileInfo = NULL;
	DWORD dwBMFileInfoCount = 0;
	DWORD dwFlag = 0;

	BOOL bOK = TRUE;

	for( i = 0; i<m_nFileCount;i++)
	{
		pFileInfo = m_arrFileInfo[i];
        pBMFileInfo = NULL;
		memset(pBMFileInfoArr,0,MAX_RET_FILE_NUM * sizeof(BMFileInfo));
		dwBMFileInfoCount = 0;
		dwFlag = 0;
		//从GUI上忽略的文件
		if(m_arrFile[i].CompareNoCase(FILE_OMIT) == 0)
		{
			continue;
		}
		
		if( m_mapObserverRegs.Lookup(dwOprCookie,pObserver))
		{
			HRESULT hr = pObserver->OnFilePrepare(dwOprCookie,
				T2W((LPTSTR)(LPCTSTR)m_strProductName),
				T2W((LPTSTR)(LPCTSTR)m_arrFile[i]),
				(LPVOID)pFileInfo,
				(LPVOID)pBMFileInfoArr,
				&dwBMFileInfoCount,
				&dwFlag);
			if(FAILED(hr))
			{
				if(m_bLog && m_log.IsOpenLog())
					m_log.XLog(_T("OnFilePrepare: Failed\n "));
				bOK = FALSE;
				break;				
			}
			if(dwFlag)  //回调已经处理，dwBMFileInfoCount 反映处理的情况
			{
				for(UINT j = 0; j<dwBMFileInfoCount; j++ )
				{
					pBMFileInfo = new BMFileInfo;
					memcpy(pBMFileInfo,pBMFileInfoArr+j,sizeof(BMFileInfo));
					m_arrBMFileInfo.push_back(pBMFileInfo);
					nCount++;
					m_arrBMFileFlag.push_back(pBMFileInfo->bLoadCodeFromFile);
				}
			}
		}
		
		if(dwFlag == 0)
		{
			pBMFileInfo = new BMFileInfo;
			InitBMFileDefault(pBMFileInfo,i,bMultiFiles);
			m_arrBMFileInfo.push_back(pBMFileInfo);
			nCount++;
			m_arrBMFileFlag.push_back(pBMFileInfo->bLoadCodeFromFile);
		}		
	}
   
	if(!bOK)
	{
		m_nBMFileCount = nCount;
		delete [] pBMFileInfoArr;
		ClearBMFiles();
		return bOK;
	}

	// do extend files
	{
		bOK = TRUE;
		pBMFileInfo = NULL;
		memset(pBMFileInfoArr,0,MAX_RET_FILE_NUM * sizeof(BMFileInfo));
		dwBMFileInfoCount = 0;
		dwFlag = 0;			
		if( m_mapObserverRegs.Lookup(dwOprCookie,pObserver))
		{
			HRESULT hr = pObserver->OnFilePrepare(dwOprCookie,
				T2W((LPTSTR)(LPCTSTR)m_strProductName),
				NULL,
				0,
				(LPVOID)pBMFileInfoArr,
				&dwBMFileInfoCount,
				&dwFlag);
			if(FAILED(hr))
			{
				if(m_bLog && m_log.IsOpenLog())
					m_log.XLog(_T("OnFilePrepare: Failed\n "));
				bOK = FALSE;					
			}
			if(bOK && dwFlag)  //回调已经处理，dwBMFileInfoCount 反映处理的情况
			{
				for(UINT j = 0; j<dwBMFileInfoCount; j++ )
				{
					pBMFileInfo = new BMFileInfo;
					memcpy(pBMFileInfo,pBMFileInfoArr+j,sizeof(BMFileInfo));
					m_arrBMFileInfo.push_back(pBMFileInfo);
					nCount++;
					m_arrBMFileFlag.push_back(pBMFileInfo->bLoadCodeFromFile);
				}				
			}
		}		
	}

	m_nBMFileCount = nCount;
	delete [] pBMFileInfoArr;

	if(!bOK)
	{
		ClearBMFiles();
		return bOK;
	}

#if 0 //polo deleted 
    // Open file mapping objects
    for( i=0; i < m_nBMFileCount; i++)
    {
		pBMFileInfo = m_arrBMFileInfo[i];

        if( pBMFileInfo->bLoadCodeFromFile /*&& !pBMFileInfo->bChangeCode*/)			
        {
			if(!LoadBMFileInfo( pBMFileInfo ) )
			{
				CString strErr;
				strErr.Format(_T("Load BMFile[%s] failed"),pBMFileInfo->szFileID);
				m_log.XLog(strErr);

				ClearBMFiles();
				
				return FALSE;
			}

			pBMFileInfo->bLoadCodeFromFile = FALSE;	
        }
	
    }
#endif 
	return TRUE;
	
}

BOOL CBMAFImp::InitBootFileInfo(DWORD dwOprCookie)
{
	BOOL bOK					= FALSE;
	BMFileInfo* pBMFileInfoArr	= NULL;
	UINT i						=0;

	do 
	{
		if (0 == m_nBMFileCount)
		{
			break;
		}
		if( !m_mapBootModeObj.Lookup(dwOprCookie, m_pBootModeObj) || NULL == m_pBootModeObj)
		{
			break;
		}
		if ((m_pBootModeObj->BootFileInfo).bInitBMFiles)
		{
			return TRUE;
		}
		pBMFileInfoArr = new BMFileInfo[m_nBMFileCount];
		if (NULL == pBMFileInfoArr)
		{
			break;
		}
		memset(pBMFileInfoArr,0,m_nBMFileCount * sizeof(BMFileInfo));
		for(i=0;i<m_nBMFileCount;++i)
		{
			memcpy(pBMFileInfoArr+i,m_arrBMFileInfo[i],sizeof(BMFileInfo));
		}
		bOK = TRUE;
	} while(0);
	
	if (!bOK)
	{
		return FALSE;
	}
	
	for( i=0; i < m_nBMFileCount; ++i)
	{
		BMFileInfo*	pBMFileInfo = pBMFileInfoArr+i;
		
		if( pBMFileInfo->bLoadCodeFromFile)			
		{
			if(!LoadBMFileInfo( pBMFileInfo ) )
			{
				CString strErr;
				strErr.Format(_T("Load BMFile[%s] failed"),pBMFileInfo->szFileID);
				m_log.XLog(strErr);
				
				
				for(UINT j = 0; j < i ; ++j )
				{
					UnloadBMFileInfo( pBMFileInfoArr+j,m_arrBMFileFlag[j] );	
				}
				SAFE_DELETE_ARRAY(pBMFileInfoArr);
				
				return FALSE;
			}
			
			pBMFileInfo->bLoadCodeFromFile = FALSE;	
		}	
	}

	(m_pBootModeObj->BootFileInfo).pBMFileInfo = pBMFileInfoArr;
	(m_pBootModeObj->BootFileInfo).dwBMFileCount = m_nBMFileCount;
	(m_pBootModeObj->BootFileInfo).bInitBMFiles  = TRUE;


	return TRUE;
}

void CBMAFImp::InitBMFileDefault(PBMFileInfo lpBMFileInfo, int nFileIndex, BOOL bMultiFiles /*= FALSE*/)
{
	ASSERT(lpBMFileInfo != NULL);

    FILE_INFO_T * pFileInfo = NULL;
	pFileInfo = m_arrFileInfo[nFileIndex];

	memset(lpBMFileInfo,0,sizeof(BMFileInfo));
	_tcscpy(lpBMFileInfo->szFileID,pFileInfo->szID);
	_tcscpy(lpBMFileInfo->szFileType,pFileInfo->szType);
	_tcscpy(lpBMFileInfo->szFileName,m_arrFile[nFileIndex]);
	_tcscpy(lpBMFileInfo->szRepID,pFileInfo->arrBlock[0].szRepID);
	lpBMFileInfo->llBase = pFileInfo->arrBlock[0].llBase;
	
	if(pFileInfo->dwFlag == 0)
	{
		lpBMFileInfo->bLoadCodeFromFile = FALSE;
		lpBMFileInfo->llOprSize = pFileInfo->arrBlock[0].llSize;
	}
	else
	{
		lpBMFileInfo->bLoadCodeFromFile = TRUE;
		lpBMFileInfo->llCodeSize = pFileInfo->arrBlock[0].llSize;

		if(_tcsicmp(pFileInfo->szID,_T("FDL")) != 0  && _tcsicmp(pFileInfo->szID,_T("FDL2")) != 0)
		{
			if(bMultiFiles)
			{
				lpBMFileInfo->bChangeCode = TRUE;
			}
		}		
		if( _tcsnicmp(pFileInfo->szID,_T("NV"),2) == 0 ||
			_tcsnicmp(pFileInfo->szID,_T("SoftSIM"),7) == 0)
		{
			lpBMFileInfo->bChangeCode = TRUE;

		}

		if( _tcsnicmp(pFileInfo->szID,_T("SoftSIM"),7) == 0)
		{
			lpBMFileInfo->bLoadCodeFromFile = FALSE;	
		}
		
	}	
}

BOOL CBMAFImp::RegBootModeObj(DWORD dwOprCookie)
{	
    if( m_mapBootModeObj.Lookup(dwOprCookie, m_pBootModeObj) )
    {		
        m_pBootModeObj->bStop         = TRUE;
        m_pBootModeObj->bFirstStart   = FALSE;
    }
    else
    {
		m_pBootModeObj = new BOOTMODEOBJ_T;
		memset(m_pBootModeObj,0,sizeof(BOOTMODEOBJ_T));     
		IBootModeHandler * pBootModeHandler  = NULL;
		if(!g_theApp.m_pfCreateBMObject)
		{
			if(m_bLog)
				m_log.XLog(_T("Create BMObject function is empty."));
			return FALSE; //BMA_ERR_NOTREG
		}

		if(!g_theApp.m_pfCreateBMObject(&pBootModeHandler))
		{
			if(m_bLog)
			    m_log.XLog(_T("Create BMObject failed."));
            return FALSE; //BMA_ERR_NOTREG
		}
		
		//m_pBootModeObj->clsid = clsidHandler;
		m_pBootModeObj->pSnapin = pBootModeHandler;
		m_pBootModeObj->bStop = FALSE;
		m_pBootModeObj->dwCookie = 0; //没有注册Observer
		m_pBootModeObj->bFirstStart = TRUE;

		m_mapBootModeObj.SetAt(dwOprCookie,m_pBootModeObj);
    }
	if(m_bLog && m_log.IsOpenLog())
	{
		CString str;
		str.Format(_T("[PORT:%d] Register BootModeObj succeed."),dwOprCookie);
		m_log.XLog(str);
	}
	return TRUE;

}

BOOL CBMAFImp::UnregBootModeObj(DWORD dwOprCookie)
{	
	if(!m_mapBootModeObj.Lookup(dwOprCookie,m_pBootModeObj))
	{
		return FALSE;
	}
	if(m_pBootModeObj->pSnapin)
	{
		if(g_theApp.m_pfReleaseBMObject)
		{
			g_theApp.m_pfReleaseBMObject(m_pBootModeObj->pSnapin);
		}		
		m_pBootModeObj->pSnapin = NULL;
	}
	delete m_pBootModeObj;
	m_pBootModeObj = NULL;
	m_mapBootModeObj.RemoveKey(dwOprCookie);
	return TRUE;
}
BOOL CBMAFImp::RegBMPObserver(DWORD dwOprCookie)
{
	IBMOprObserver* pObserver=NULL;
	if(!m_mapObserverRegs.Lookup(dwOprCookie,pObserver))
		return TRUE;
	DWORD dwCookie = dwOprCookie;
	IBootModeHandler * pbmh = m_pBootModeObj->pSnapin;
    
	HRESULT hr = pbmh->SubscribeOperationObserver( pObserver, NULL, &dwCookie );
	if(FAILED( hr ) )
	{
		if(m_bLog)
		{
			CString str;
			str.Format(_T("Subscribe operation observer failed.[ErrorCode:0x%08X]"),hr);
			m_log.XLog(str);
		}
		return FALSE;
	}
	PBOOTMODEOBJ_T pBootModeObj = NULL;
	m_mapBootModeObj.Lookup(dwOprCookie,pBootModeObj);
	ASSERT(pBootModeObj !=NULL);
	pBootModeObj->dwCookie = dwCookie; //表明注册
	return TRUE;

}
BOOL CBMAFImp::UnregBMPObserver(DWORD dwOprCookie)
{
	PBOOTMODEOBJ_T pBMO = NULL;
	if(FAILED(BMAF_GetBootModeObjInfo(dwOprCookie,(LPVOID*)&pBMO)))
		return FALSE;	

	if( pBMO->pSnapin)
	{
		pBMO->pSnapin->UnsubscribeOperationObserver( pBMO->dwCookie );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CBMAFImp::StartLog( void )
{	
    return m_log.OpenLog();
}

void CBMAFImp::StopLog( void )
{
    m_log.CloseLog();
}

BOOL CBMAFImp::LoadBMFileInfo(PBMFileInfo lpBMFileInfo)
{
	if( lpBMFileInfo == NULL )
		return FALSE;

	if( !lpBMFileInfo->bLoadCodeFromFile )
	{
		return TRUE;
	}

	CString strFDLFileName;
	CString strError;
	BOOL bRet			= FALSE;
	HANDLE hCode		= NULL;
    unsigned __int64 llCodeSize	= 0;
    LARGE_INTEGER liFileSize;
    liFileSize.HighPart = liFileSize.LowPart = 0;
	
	do 
	{
		GetAbsolutePath( strFDLFileName,(LPCTSTR)lpBMFileInfo->szFileName );
		if( strFDLFileName.IsEmpty() )
		{
			break;
		}
		hCode = CreateFile(strFDLFileName.operator LPCTSTR (),
			GENERIC_READ,
			FILE_SHARE_READ,                         //Exclusive Open
			NULL,                      //Can't Be inherited
			OPEN_EXISTING,             //If not existing then failed
			FILE_ATTRIBUTE_READONLY,   //Read Only
			NULL);
		if( hCode == INVALID_HANDLE_VALUE)
		{   
			hCode = NULL;
			break;
		}
        
		if( !GetFileSizeEx(hCode,&liFileSize))
		{
			break;
		}
        llCodeSize = liFileSize.HighPart ;
        llCodeSize = llCodeSize<<32;
        llCodeSize |= liFileSize.LowPart;
		if ( 0 == lpBMFileInfo->llCodeOffset )
		{
			lpBMFileInfo->llCodeSize = llCodeSize;
		}
		else
		{
			llCodeSize = lpBMFileInfo->llCodeSize;
		}

		if ( lpBMFileInfo->llCodeSize >= 0x100000000 && NULL == _tcsstr (lpBMFileInfo->szFileType,_T("_64")) )
		{
			_tcscat(lpBMFileInfo->szFileType,_T("_64"));
		}
		
		if( lpBMFileInfo->bChangeCode )
		{
			lpBMFileInfo->lpCode = new BYTE[ (unsigned int)llCodeSize ];	
			if(lpBMFileInfo->lpCode == NULL)
			{
				strError.Format(_T("LoadBMFileInfo: memory is not enough."));
				break;
			}
			if (lpBMFileInfo->llCodeOffset)
			{
				LARGE_INTEGER liOffset;
				liOffset.QuadPart = lpBMFileInfo->llCodeOffset;
				if (::SetFilePointer(hCode,liOffset.LowPart, &liOffset.HighPart,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
				{
					strError.Format(_T("SetFilePointer failed in LoadBMFileInfo, ErrorCode=0x%x\n"),GetLastError());	
					break;
				}
			}
			DWORD dwReaded = 0;
			if(!::ReadFile(hCode,lpBMFileInfo->lpCode,(DWORD)llCodeSize,&dwReaded,NULL) || dwReaded != llCodeSize)
			{
				break;
			}
			lpBMFileInfo->dwFirstMapSize	= (DWORD)llCodeSize;
			SAFE_CLOSE_HANDLE(hCode);
		}
		else
		{
			lpBMFileInfo->dwFirstMapSize	= 0;
			lpBMFileInfo->hFDLCode			= hCode;
		}

		bRet = TRUE;
	} while (0);
	
	if (!bRet)
	{
		
		SAFE_DELETE_ARRAY(lpBMFileInfo->lpCode);
		SAFE_CLOSE_HANDLE(hCode);
	}
	else
	{
		lpBMFileInfo->bLoadCodeFromFile = FALSE;
	}
	
	return bRet; 
}
void CBMAFImp::UnloadBMFileInfo( PBMFileInfo lpBMFileInfo, BOOL bLoadByFrame )
{
    if( lpBMFileInfo == NULL )
        return;
    if( bLoadByFrame )
    {
        SAFE_DELETE_ARRAY(lpBMFileInfo->lpCode);
        SAFE_CLOSE_HANDLE(lpBMFileInfo->hFDLCode);
    }
}

void CBMAFImp::ClearBMFiles()
{
	UINT  nCount = m_arrBMFileInfo.size();
    for(UINT i = 0; i < nCount ; i++ )
    {
        UnloadBMFileInfo( m_arrBMFileInfo[i],m_arrBMFileFlag[i] );
        SAFE_DELETE_ARRAY(m_arrBMFileInfo[i]);
    }
    
    m_arrBMFileInfo.clear();
    m_nBMFileCount = 0;

	m_arrBMFileFlag.clear();
}
BOOL CBMAFImp::HasLog( void )
{
	_TCHAR szConfigFile[MAX_PATH];
    
    GetModuleFilePath( NULL, szConfigFile );
	_tcscat( szConfigFile,  _T("\\BMTimeout.ini") );
	return GetPrivateProfileInt( _T("Log"), _T("Enable"), 0, szConfigFile); 

}

