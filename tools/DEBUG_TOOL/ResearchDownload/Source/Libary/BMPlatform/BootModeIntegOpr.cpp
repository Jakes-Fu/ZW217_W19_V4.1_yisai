// BootModeIntegOpr.cpp : implementation file
//

#include "stdafx.h"
#include "BMPlatform.h"
#include "BootModeIntegOpr.h"
#include "OptionHelpper.h"
#include "BMAGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_CHECK_BAUD_TIMES      3
#define DEFAULT_TIME_OUT              1000
#define MAX_ERASEFLASH_TIMEOUT        5000
#define SECTION_SIZE                  0x10000
#define MAX_PARTITION_NUM             128

// Do repartition always
#define REPAR_STRATEGY_DO_ALWAYS        0   
// Stop actions and report error when incompatible partition error occured
#define REPAR_STRATEGY_STOP             1
// Ignore incompatible partition error
#define REPAR_STRATEGY_IGNORE           2
// Do repartion action when imcompatible partition error occured
#define REPAR_STRATEGY_DO               3

extern COptionHelpper ohObject;


/////////////////////////////////////////////////////////////////////////////
// CBootModeIntegOpr
CBootModeIntegOpr::CBootModeIntegOpr()
{
    m_bCheckBaudRate    = FALSE;
    m_bStopOpr          = FALSE;
    m_dwOprCookie       = 0;
    m_bRcvThread        = TRUE;
    m_dwRcvThreadID     = 0;
    m_hRcvWindow        = NULL;
	m_bNeedRepartion    = TRUE; 
    m_bRepartionDone    = FALSE;
    m_pFileBuf          = NULL;
    m_pFileBuf          = new BYTE[MAX_MAP_SIZE];
    m_dwFlushSize       = 0;
}

CBootModeIntegOpr::~CBootModeIntegOpr()
{
	m_hRcvWindow = NULL;
    SAFE_DELETE_ARRAY(m_pFileBuf);
}

BEGIN_DISP_MAP(CBootModeIntegOpr)
	DISP_FUNC(CBootModeIntegOpr, "CheckBaud", CheckBaud)
	DISP_FUNC(CBootModeIntegOpr, "Connect", Connect)
	DISP_FUNC(CBootModeIntegOpr, "Download", Download)
    DISP_FUNC(CBootModeIntegOpr, "DownloadEx", DownloadEx)
	DISP_FUNC(CBootModeIntegOpr, "DownloadByID", DownloadByID)
    DISP_FUNC(CBootModeIntegOpr, "DownloadByIDEx", DownloadByIDEx)
	DISP_FUNC(CBootModeIntegOpr, "EraseFlash", EraseFlash)
	DISP_FUNC(CBootModeIntegOpr, "ReadFlash", ReadFlash)
	DISP_FUNC(CBootModeIntegOpr, "ReadFlashByID", ReadFlashByID)	
	DISP_FUNC(CBootModeIntegOpr, "Excute", Excute)
	DISP_FUNC(CBootModeIntegOpr, "Reset", Reset)
    DISP_FUNC(CBootModeIntegOpr, "SetFirstMode", SetFirstMode)
    DISP_FUNC(CBootModeIntegOpr, "ReadChipType", ReadChipType)
    DISP_FUNC(CBootModeIntegOpr, "ReadNVItem", ReadNVItem)
    DISP_FUNC(CBootModeIntegOpr, "ChangeBaud", ChangeBaud)
    DISP_FUNC(CBootModeIntegOpr, "EraseFlash2", EraseFlash2)
	DISP_FUNC(CBootModeIntegOpr, "EraseFlashByID", EraseFlashByID)
	DISP_FUNC(CBootModeIntegOpr, "Repartition", Repartition)
	DISP_FUNC(CBootModeIntegOpr, "ForceRepartition", ForceRepartition)
	DISP_FUNC(CBootModeIntegOpr, "RepartitionByID", RepartitionByID)
	DISP_FUNC(CBootModeIntegOpr, "ForceRepartitionByID", ForceRepartitionByID)
	DISP_FUNC(CBootModeIntegOpr, "ExecNandInit", ExecNandInit)
    DISP_FUNC(CBootModeIntegOpr, "ReadFlashType",ReadFlashType)
	DISP_FUNC(CBootModeIntegOpr, "ReadSectorSize", ReadSectorSize)
	DISP_FUNC(CBootModeIntegOpr, "ReadFlashAndSave", ReadFlashAndSave)
	DISP_FUNC(CBootModeIntegOpr, "ReadFlashAndSaveByID", ReadFlashAndSaveByID)
	DISP_FUNC(CBootModeIntegOpr, "ReadFlashAndDirectSave", ReadFlashAndDirectSave)
	DISP_FUNC(CBootModeIntegOpr, "ReadFlashAndDirectSaveByID", ReadFlashAndDirectSaveByID)
    DISP_FUNC(CBootModeIntegOpr, "ReadFlashAndSaveByIDEx", ReadFlashAndSaveByIDEx)
	DISP_FUNC(CBootModeIntegOpr, "DoNothing", DoNothing)
	DISP_FUNC(CBootModeIntegOpr, "SetCRC", SetCRC)
	DISP_FUNC(CBootModeIntegOpr, "ResetCRC", ResetCRC)
	DISP_FUNC(CBootModeIntegOpr, "CheckBaudRom", CheckBaudRom)
	DISP_FUNC(CBootModeIntegOpr, "ConnectRom", ConnectRom)
	DISP_FUNC(CBootModeIntegOpr, "KeepCharge", KeepCharge)
    DISP_FUNC(CBootModeIntegOpr, "DDRCheck", DdrCheck)
    DISP_FUNC(CBootModeIntegOpr, "SelfRefresh", SelfRefresh)
    DISP_FUNC(CBootModeIntegOpr, "ReadNandBlockInfo", ReadNandBlockInfo)
	DISP_FUNC(CBootModeIntegOpr, "ReadFlashInfo",ReadFlashInfo)
	DISP_FUNC(CBootModeIntegOpr, "SendExtTable", SendExtTable)
	DISP_FUNC(CBootModeIntegOpr, "ReadFlashUID", ReadFlashUID)
	DISP_FUNC(CBootModeIntegOpr, "ReadSoftSimEID", ReadSoftSimEID)
	DISP_FUNC(CBootModeIntegOpr, "GetCheckBaudCrcType",GetCheckBaudCrcType)
	DISP_FUNC(CBootModeIntegOpr, "Connect2",Connect2)
	DISP_FUNC(CBootModeIntegOpr, "CheckDUT",CheckDUT)
	DISP_FUNC(CBootModeIntegOpr, "PowerOff",PowerOff)
	DISP_FUNC(CBootModeIntegOpr, "CheckRoot",CheckRoot)
    DISP_FUNC(CBootModeIntegOpr, "ReadChipUID", ReadChipUID)
    DISP_FUNC(CBootModeIntegOpr, "EnableFlash", EnableFlash)    
    DISP_FUNC(CBootModeIntegOpr, "EnableSecureBoot", EnableSecureBoot)
	DISP_FUNC(CBootModeIntegOpr, "ReadRefInfo", ReadRefInfo)
	DISP_FUNC(CBootModeIntegOpr, "WriteAPRInfo", WriteAPRInfo)
	DISP_FUNC(CBootModeIntegOpr, "ReadTransceiverType", ReadTransceiverType)
	DISP_FUNC(CBootModeIntegOpr, "EnableDebugMode", EnableDebugMode)
	DISP_FUNC(CBootModeIntegOpr, "HostConnect", HostConnect)
	DISP_FUNC(CBootModeIntegOpr, "HostDownload", HostDownload)
	DISP_FUNC(CBootModeIntegOpr, "HostExcute", HostExcute)
    DISP_FUNC(CBootModeIntegOpr, "EndProcess", EndProcess)
    DISP_FUNC(CBootModeIntegOpr, "SetTimeStamp", SetTimeStamp)
    DISP_FUNC(CBootModeIntegOpr, "ReadPartitionInfo", ReadPartitionInfo)
	DISP_FUNC(CBootModeIntegOpr, "DumpUbootLog", DumpUbootLog)
	DISP_FUNC(CBootModeIntegOpr, "ChangeSelinux", ChangeSelinux)
END_DISP_MAP(CBootModeIntegOpr)

/////////////////////////////////////////////////////////////////////////////
// CBootModeIntegOpr message handlers

BOOL CBootModeIntegOpr::_CheckBaud(void * pFileInfo,BOOL bRom)
{
    BOOL bRet = FALSE;
    int nTimes = 0;
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
    int   nMaxTimes = ohObject.GetCheckBaudTimes( pBMFileInfo->szFileType );
	DWORD dwTimeout = ohObject.GetTimeout( bRom ? _T("CheckBaudRom"): _T("Check Baud"));
	
    m_bmOpr.DoBeforeCheckBaud();
    while( !m_bStopOpr )
    {
        if( nMaxTimes != 0 )
        {
            if( nTimes > nMaxTimes )
            {
                m_bCheckBaudRate = FALSE;
                bRet = FALSE;
                break;
            }
        }
		
        nTimes++;
        
        if(!m_bCheckBaudRate)
        {
            PostMessageToUplevel( BM_CHECK_BAUDRATE, 
                (WPARAM)m_dwOprCookie, (LPARAM)0 );
            m_bCheckBaudRate = TRUE;
        } 		
        
        if(   m_bCheckBaudRate 
            && !m_bmOpr.CheckBaud( dwTimeout ) ) // Failed
        {           
			if(bRom)
			{
				m_bCheckBaudRate = TRUE;
				continue;
			}
			else
			{
				if(m_bmOpr.GetLastErrorCode() == BSL_PHONE_WAIT_INPUT_TIMEOUT)
				{
					m_bCheckBaudRate = TRUE;
					continue;
				}
				else
				{
					bRet = FALSE;
					m_bCheckBaudRate = FALSE;
					break;
				}
			}
        } 
        
        // Success
        bRet = TRUE;		
        m_bCheckBaudRate = FALSE;        
        break;
    }

	m_bmOpr.DoAfterChackBaud();
    
    return bRet;
}
BOOL CBootModeIntegOpr::CheckBaud(void * pFileInfo) 
{
	return _CheckBaud(pFileInfo,FALSE);
}

BOOL CBootModeIntegOpr::CheckBaudRom(void * pFileInfo) 
{
	return _CheckBaud(pFileInfo,TRUE);
}

BOOL CBootModeIntegOpr::_Connect(void * pFileInfo,BOOL bRom) 
{    
    if( !_CheckBaud( pFileInfo,bRom ) )
    {
        return FALSE;
    }
    
    PostMessageToUplevel( BM_CONNECT, 
        (WPARAM)m_dwOprCookie, (LPARAM)0 );

    int nTimeout = ohObject.GetTimeout( _T("Connect") );
    
    if( !m_bmOpr.Connect( nTimeout ) && bRom)
    {
		if(bRom)
		{
			// Boot code in module has a 
			// bug,it possibly make connect
			// failed at first time,so we
			// try once more.
			if( !m_bmOpr.Connect( nTimeout ) )
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
    }
	
    return TRUE;
}

BOOL CBootModeIntegOpr::Connect(void * pFileInfo) 
{
	return _Connect(pFileInfo,FALSE);
}

BOOL CBootModeIntegOpr::ConnectRom(void * pFileInfo) 
{
	return _Connect(pFileInfo,TRUE);
}

BOOL CBootModeIntegOpr::HostConnect(void* pFileInfo)           //< LaoTan Add
{
	UNUSED_ALWAYS( pFileInfo );
	BOOL bRet = FALSE;
	int nTimes = 0;
	int nMaxTimes = ohObject.GetCheckBaudTimes( _T("HostConnect") );
	int nTimeout = ohObject.GetTimeout( _T("HostConnect") );
	PostMessageToUplevel( BM_CONNECT, (WPARAM)m_dwOprCookie, (LPARAM)0 );

	while( !m_bStopOpr )
	{
		if( nMaxTimes != 0 )
		{
			if( nTimes > nMaxTimes )
			{
				bRet = FALSE;
				break;
			}
		}
		if (m_bmOpr.HostConnect( nTimeout ))
		{
			bRet = TRUE;
			break;
		}

		nTimes++;
	}
	return bRet;
}

BOOL CBootModeIntegOpr::HostDownload(void* pFileInfo)
{
	BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
	DWORD dwTimeout = ohObject.GetTimeout( _T("HostDownload") );
	CString strError;
	BOOL bOK = FALSE;
	do 
	{
		if (0 == pBMFileInfo->llCodeSize)
		{
			strError.Format(_T("[%s] file size is zero."),pBMFileInfo->szFileID);
			break;
		}
		if (!m_bmOpr.SetFdlFileInfo(pBMFileInfo))
		{
			strError = _T("Failed to set fdl file info.");	
			break;
		}
		int nPackets = m_bmOpr.HostCountPackets();
		if (0 == nPackets)
		{
			strError = _T("Invalid fdl packets size.");	
		}
		PostMessageToUplevel( BM_DOWNLOAD, (WPARAM)m_dwOprCookie, (LPARAM)100 );

		if(!m_bmOpr.HostDownloadStart(dwTimeout))
		{
			strError = _T("Failed to HostDownloadStart.");
			break;
		}

		BOOL bMidstOK = TRUE;
		for(int i = 0; i < nPackets; ++i)
		{
			if(!m_bmOpr.HostDownloadData(i,dwTimeout))
			{
				strError.Format(_T("Failed to HostDownloadData %d packets."),i+1);
				bMidstOK = FALSE;
				break;
			}
			PostMessageToUplevel( BM_DOWNLOAD_PROCESS, (WPARAM)m_dwOprCookie, (LPARAM)( (i + 1) * 100 / nPackets));
		}
		if (!bMidstOK)
		{
			break;
		}

		if(!m_bmOpr.HostDownloadEnd(dwTimeout))
		{
			strError = _T("Failed to HostDownloadEnd.");
			break;
		}
		bOK = TRUE;

	} while (0);

	if (!strError.IsEmpty())
	{
		LogError(strError.operator LPCTSTR());
	}
	return bOK;
}

BOOL CBootModeIntegOpr::HostExcute(void* pFileInfo)
{
	UNUSED_ALWAYS( pFileInfo );

	DWORD dwTimeout = ohObject.GetTimeout( _T("HostExcute") ) ;

	return m_bmOpr.HostExcute( dwTimeout );

}

BOOL CBootModeIntegOpr::CheckDUT(void* pFileInfo)
{
	UNUSED_ALWAYS( pFileInfo );
	int nMaxTimes = ohObject.GetCheckDUTTimes();
	int nTimeout = ohObject.GetTimeout( _T("CheckDUT") );
	int nTimes = 0;
	while( !m_bStopOpr )
	{
		if( nMaxTimes != 0 )
		{
			if( nTimes > nMaxTimes )
			{
				break;
			}
		}
		if (!m_bmOpr.Connect( nTimeout ))
		{
			break;
		}

		nTimes++;
		Sleep(nTimeout);
	}

	return TRUE;
}

BOOL CBootModeIntegOpr::Connect2(void* pFileInfo)
{
	UNUSED_ALWAYS( pFileInfo );
	PostMessageToUplevel( BM_CONNECT, 
		(WPARAM)m_dwOprCookie, (LPARAM)0 );

	int nTimeout = ohObject.GetTimeout( _T("Connect") );

	if( !m_bmOpr.Connect( nTimeout ))
	{		
		// Boot code in module has a 
		// bug,it possibly make connect
		// failed at first time,so we
		// try once more.
		if( !m_bmOpr.Connect( nTimeout ) )
		{
			return FALSE;
		}		
	}

	return TRUE;
}

BOOL CBootModeIntegOpr::Download(void * pFileInfo) 
{
	return _Download(pFileInfo,FALSE,FALSE);
}

BOOL CBootModeIntegOpr::DownloadEx(void* pFileInfo)
{
    return _Download(pFileInfo,FALSE,TRUE);
}

BOOL CBootModeIntegOpr::DownloadByID(void * pFileInfo) 
{

    BOOL bOK = FALSE;
    if (m_bRepartionDone)
    {
        bOK = _Download(pFileInfo,TRUE,FALSE);
    }
    else
    {
        m_bmOpr.SetLastErrorCode(BSL_REP_INCOMPATIBLE_PARTITION);      
    }   
    return bOK;  	
}

BOOL CBootModeIntegOpr::DownloadByIDEx(void * pFileInfo) 
{
    BOOL bOK = FALSE;
    if (m_bRepartionDone)
    {
        bOK = _Download(pFileInfo,TRUE,TRUE);
    }
    else
    {
        m_bmOpr.SetLastErrorCode(BSL_REP_INCOMPATIBLE_PARTITION);
    }
    return bOK;  	
}

BOOL CBootModeIntegOpr::EraseFlash(void * pFileInfo) 
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
    
    PostMessageToUplevel( BM_ERASE_FLASH, (WPARAM)m_dwOprCookie, (LPARAM)0 );
    DWORD dwTimeout = ohObject.GetTimeout( _T("Erase Flash") );
    if( dwTimeout <= MAX_ERASEFLASH_TIMEOUT )
    {
        dwTimeout = (DWORD)(pBMFileInfo->llOprSize / SECTION_SIZE * dwTimeout + dwTimeout);
    }
    
    if( !m_bmOpr.StartData( 
        pBMFileInfo->llBase, 
        pBMFileInfo->llOprSize, 
        NULL,
        0,
        dwTimeout  )  )
    {
        return FALSE;
    }
    if( !m_bmOpr.EndData() )
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CBootModeIntegOpr::EraseFlash2( void * pFileInfo  )
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;

    PostMessageToUplevel( BM_ERASE_FLASH, (WPARAM)m_dwOprCookie, (LPARAM)0 );
    DWORD dwTimeout = ohObject.GetTimeout( _T("Erase Flash") );
    if( dwTimeout <= MAX_ERASEFLASH_TIMEOUT )
    {
        dwTimeout = (DWORD)(pBMFileInfo->llOprSize / SECTION_SIZE * dwTimeout + dwTimeout);
    }
    
    if( !m_bmOpr.EraseFlash( 
        (DWORD)(pBMFileInfo->llBase), 
        (DWORD)(pBMFileInfo->llOprSize), dwTimeout )  )
    {
        return FALSE;
    }

    return TRUE;    
}

BOOL CBootModeIntegOpr::EraseFlashByID( void * pFileInfo  )
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
	
    PostMessageToUplevel( BM_ERASE_FLASH, (WPARAM)m_dwOprCookie, (LPARAM)0 );
    DWORD dwTimeout = ohObject.GetTimeout( _T("Erase Flash") );
    if( dwTimeout <= MAX_ERASEFLASH_TIMEOUT )
    {
        dwTimeout = (DWORD)(pBMFileInfo->llOprSize / SECTION_SIZE * dwTimeout + dwTimeout);
    }
    
    if( !m_bmOpr.EraseFlash( 
        (LPBYTE)pBMFileInfo->szRepID,
		MAX_REP_ID_LEN*2,
        (DWORD)(pBMFileInfo->llOprSize), dwTimeout  )  )
    {
        return FALSE;
    }
	
    return TRUE;    
}

BOOL CBootModeIntegOpr::ReadFlash(void * pFileInfo) 
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;

    // Read NV_LENGTH byte from module
    m_bmOpr.AllocRecvBuffer( (DWORD)(pBMFileInfo->llOprSize) );

    DWORD dwBase = (DWORD)(pBMFileInfo->llBase);
    DWORD dwLeft = (DWORD)(pBMFileInfo->llOprSize);
    DWORD dwMaxLength = pBMFileInfo->dwMaxLength;    
    DWORD dwSize = dwMaxLength;   
	DWORD dwReadFlashTimeout = ohObject.GetTimeout( _T("Read Flash") );

    PostMessageToUplevel( BM_READ_FLASH, (WPARAM)m_dwOprCookie, (LPARAM)100 );
    
    if( dwBase & 0x80000000 )
    {
       DWORD dwOffset = 0;
       while(dwLeft > 0 && !m_bStopOpr )
       {
            if(dwLeft > dwMaxLength )
            {
                dwSize = dwMaxLength ;                
            }
            else
            {
                dwSize = dwLeft;
            }
        
            if( !m_bmOpr.ReadPartitionFlash( dwBase, dwSize, dwOffset, dwReadFlashTimeout ) )
            {
                m_bmOpr.FreeRecvBuffer();
                return FALSE;
            }
            dwOffset += dwSize;
            dwLeft -= dwSize;
            PostMessageToUplevel( BM_READ_FLASH_PROCESS, 
                (WPARAM)m_dwOprCookie, 
                (LPARAM)((pBMFileInfo->llOprSize - dwLeft)*100/pBMFileInfo->llOprSize) );        
        }
    }
    else
    {
        while(dwLeft > 0 && !m_bStopOpr )
        {
            if(dwLeft > dwMaxLength )
            {
                dwSize = dwMaxLength ;                
            }
            else
            {
                dwSize = dwLeft;
            }
        
            if( !m_bmOpr.ReadFlash( dwBase, dwSize, dwReadFlashTimeout ) )
            {
                m_bmOpr.FreeRecvBuffer();
                return FALSE;
            }
            dwBase += dwSize;
            dwLeft -= dwSize;
            PostMessageToUplevel( BM_READ_FLASH_PROCESS, 
                (WPARAM)m_dwOprCookie, 
                (LPARAM)((pBMFileInfo->llOprSize - dwLeft)*100/pBMFileInfo->llOprSize) );        
        }
    }

    if( m_bStopOpr )
        return FALSE;
    return TRUE;
}

BOOL CBootModeIntegOpr::ReadFlashByID(void * pFileInfo) 
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;

	DWORD dwReadStartTimeout = ohObject.GetTimeout( _T("StartRead") );

	if(!m_bmOpr.StartRead((LPBYTE)pBMFileInfo->szRepID,MAX_REP_ID_LEN*2,pBMFileInfo->llOprSize,dwReadStartTimeout))
	{
		return FALSE;
	}
	
    // Read NV_LENGTH byte from module
    m_bmOpr.AllocRecvBuffer( (DWORD)(pBMFileInfo->llOprSize));
	
    //DWORD dwBase = pBMFileInfo->dwBase;
    unsigned __int64 llLeft = pBMFileInfo->llOprSize;
    DWORD dwMaxLength = pBMFileInfo->dwMaxLength;    
    DWORD dwSize = dwMaxLength;   
	
    PostMessageToUplevel( BM_READ_FLASH, (WPARAM)m_dwOprCookie, (LPARAM)100 );
    
	DWORD dwReadMidstTimeout = ohObject.GetTimeout( _T("MidstRead") );
 
    unsigned __int64 llOffset = 0;
	while(llLeft > 0 && !m_bStopOpr )
	{
        if(llLeft > dwMaxLength )
        {
            dwSize = dwMaxLength ;                
        }
        else
        {
            dwSize = (DWORD)llLeft;
        }
		
        if( !m_bmOpr.MidstRead( dwSize, llOffset, dwReadMidstTimeout ) )
        {
            m_bmOpr.FreeRecvBuffer();
            return FALSE;
        }
        llOffset += dwSize;
        llLeft -= dwSize;
        PostMessageToUplevel( BM_READ_FLASH_PROCESS, 
            (WPARAM)m_dwOprCookie, 
            (LPARAM)((pBMFileInfo->llOprSize - llLeft)*100/pBMFileInfo->llOprSize) );        
    }
	
    if( m_bStopOpr )
        return FALSE;

	DWORD dwReadEndTimeout = ohObject.GetTimeout( _T("EndRead") );
	m_bmOpr.EndRead( dwReadEndTimeout );	
    return TRUE;
}

BOOL CBootModeIntegOpr::Excute(void * pFileInfo) 
{
    UNUSED_ALWAYS( pFileInfo );

    return m_bmOpr.Excute( ohObject.GetTimeout( _T("Excute") ) );
}

BOOL CBootModeIntegOpr::ChangeBaud( void * pFileInfo )
{
    UNUSED_ALWAYS( pFileInfo );

    PostMessageToUplevel( BM_CHANGE_BAUD,  (WPARAM)m_dwOprCookie, (LPARAM)0 );     
    
    return m_bmOpr.ChangeBaud( ohObject.GetTimeout( _T("ChangeBaud") ) );
}

BOOL CBootModeIntegOpr::Reset(void * pFileInfo) 
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
    PostMessageToUplevel( BM_RESET, 
        (WPARAM)m_dwOprCookie, 
        (LPARAM)0 );      
	return m_bmOpr.Reset((LPBYTE)(pBMFileInfo->lpCode), (DWORD)(pBMFileInfo->llCodeSize), ohObject.GetTimeout( _T("Reset")) );
}

BOOL CBootModeIntegOpr::SetFirstMode(void * pFileInfo) 
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
    PostMessageToUplevel( BM_SET_FIRST_MODE, 
        (WPARAM)m_dwOprCookie, 
        (LPARAM)0 );      
    m_bmOpr.SetFirstMode((LPBYTE)(pBMFileInfo->lpCode), (DWORD)(pBMFileInfo->llCodeSize), ohObject.GetTimeout( _T("SetFirstMode")) );
	return TRUE;
}

BOOL CBootModeIntegOpr::PowerOff(void* pFileInfo)
{
	UNUSED_ALWAYS( pFileInfo );  
	return m_bmOpr.PowerOff( ohObject.GetTimeout( _T("PowerOff")) );
}

BOOL CBootModeIntegOpr::CheckRoot(void* pFileInfo)
{
    UNUSED_ALWAYS( pFileInfo );  
    return m_bmOpr.CheckRoot();
}
BOOL CBootModeIntegOpr::ReadChipType( void * pFileInfo )
{
    UNUSED_ALWAYS( pFileInfo );
    m_bmOpr.AllocRecvBuffer( sizeof(DWORD) );
    
    PostMessageToUplevel( BM_READ_CHIPTYPE,  (WPARAM)m_dwOprCookie, (LPARAM)0 );      
    
     if( !m_bmOpr.ReadChipType( 
            ohObject.GetTimeout( _T("Read ChipType") ) ) )
     {
         m_bmOpr.FreeRecvBuffer();
         return FALSE;
     }
     return TRUE;
}

BOOL CBootModeIntegOpr::ReadNVItem( void * pFileInfo  )
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;

    m_bmOpr.AllocRecvBuffer( READ_NVITEM_REP_DATA_LEN );   

    PostMessageToUplevel( BM_READ_NVITEM,  (WPARAM)m_dwOprCookie, (LPARAM)0 );      
    
    if( !m_bmOpr.ReadNVItem( (DWORD)pBMFileInfo->llBase, 
            (DWORD)(pBMFileInfo->llBase + pBMFileInfo->llOprSize), 
            (unsigned short)(ohObject.GetNVItemID()),
            ohObject.GetTimeout( _T("Read NVItem") ) ) )
    {
        m_bmOpr.FreeRecvBuffer();
        return FALSE;
    }
    LPBYTE lpRecvBuffer = m_bmOpr.GetRecvBuffer();
    DWORD dwRecvSize = m_bmOpr.GetRecvBufferSize();
    if( dwRecvSize < READ_NVITEM_REP_DATA_LEN )
    {
        m_bmOpr.FreeRecvBuffer(); 
        return FALSE;
    }

    DWORD dwSoruceValue, dwDestValue;
    dwSoruceValue =  *(DWORD*)&lpRecvBuffer[0];    
    dwDestValue   = 0;
    CONVERT_INT( dwSoruceValue, dwDestValue);
    DWORD dwReadStart = dwDestValue;
        
    unsigned short uiSourceValue, uiDestValue;        
    uiSourceValue =  *(unsigned short*)&lpRecvBuffer[ sizeof( DWORD )];
    uiDestValue   = 0;
    CONVERT_SHORT( uiSourceValue, uiDestValue);
    DWORD dwReadLength = uiDestValue;

    if( dwReadStart == -1 || dwReadLength == -1 )
    {
        m_bmOpr.FreeRecvBuffer();
        LogError( _T("Read NV Item: NOT find the NVITEM ID.") );
        return FALSE;
    }
/*

    if( dwReadStart % 4 == 2 )
    {
        dwReadStart = dwReadStart  -2 ;
        dwReadLength += 2;   
    }
    else
    {
        dwReadLength += 1;
    }
*/    
    BMFileInfo fileinfo;
    memset( &fileinfo, 0, sizeof( BMFileInfo) );
    fileinfo.llBase = dwReadStart;
    fileinfo.llOprSize = dwReadLength;
    fileinfo.dwMaxLength = 0x1000;
    return ReadFlash( (void*)&fileinfo );    
}

BOOL CBootModeIntegOpr::Repartition( void * pFileInfo  )
{
    UNUSED_ALWAYS( pFileInfo );
    
	if(m_bNeedRepartion)
	{
		m_bRepartionDone = m_bmOpr.Repartition( ohObject.GetTimeout( _T("Repartition") ) ) ;
	}
	else
	{
		m_bNeedRepartion = TRUE; // recover to initial value
        m_bRepartionDone = TRUE;		
	}

    return m_bRepartionDone;
}

BOOL CBootModeIntegOpr::ForceRepartition( void * pFileInfo  )
{
    UNUSED_ALWAYS( pFileInfo );
    m_bRepartionDone = m_bmOpr.Repartition( ohObject.GetTimeout( _T("Repartition") ) );
	return m_bRepartionDone ;
}

BOOL CBootModeIntegOpr::RepartitionByID( void * pFileInfo )
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;

	if(m_bNeedRepartion)
	{
		m_bRepartionDone =  m_bmOpr.Repartition((LPBYTE)pBMFileInfo->lpCode,(DWORD)pBMFileInfo->llCodeSize, ohObject.GetTimeout( _T("Repartition") ) ) ;
	}
	else
	{
		m_bNeedRepartion = TRUE; // recover to initial value
		m_bRepartionDone = TRUE;
	}
    return m_bRepartionDone;
}

BOOL CBootModeIntegOpr::ForceRepartitionByID( void * pFileInfo )
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
    m_bRepartionDone = m_bmOpr.Repartition((LPBYTE)pBMFileInfo->lpCode,(DWORD)pBMFileInfo->llCodeSize, ohObject.GetTimeout( _T("Repartition") ) ) ;
	return m_bRepartionDone;
}

BOOL CBootModeIntegOpr::ReadFlashType( void * pFileInfo )
{
    UNUSED_ALWAYS( pFileInfo );
    m_bmOpr.AllocRecvBuffer( sizeof(DWORD)*4 );
    if( !m_bmOpr.ReadFlashType( ohObject.GetTimeout( _T("ReadFlashType") ) ) )
    {
        m_bmOpr.FreeRecvBuffer();
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL CBootModeIntegOpr::ReadFlashInfo(void * pFileInfo)
{
    UNUSED_ALWAYS( pFileInfo );
    m_bmOpr.AllocRecvBuffer( sizeof(DWORD)*3 );
    if( !m_bmOpr.ReadFlashInfo( ohObject.GetTimeout( _T("ReadFlashInfo") ) ) )
    {
        m_bmOpr.FreeRecvBuffer();
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
BOOL CBootModeIntegOpr::ReadFlashUID(void * pFileInfo)
{
    UNUSED_ALWAYS( pFileInfo );
    m_bmOpr.AllocRecvBuffer( 8 );
    if( !m_bmOpr.ReadFlashUID( ohObject.GetTimeout( _T("ReadFlashUID") ) ) )
    {
        m_bmOpr.FreeRecvBuffer();
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL CBootModeIntegOpr::ReadChipUID(void * pFileInfo)
{
    UNUSED_ALWAYS( pFileInfo );
    m_bmOpr.AllocRecvBuffer( 128 );
    if( !m_bmOpr.ReadChipUID( ohObject.GetTimeout( _T("ReadChipUID") ) ) )
    {
        m_bmOpr.FreeRecvBuffer();
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
BOOL CBootModeIntegOpr::EnableSecureBoot(void * pFileInfo)
{
    UNUSED_ALWAYS( pFileInfo );
    return m_bmOpr.EnableSecureBoot(ohObject.GetTimeout( _T("EnableSecureBoot") ));
}

BOOL CBootModeIntegOpr::EndProcess(void* pFileInfo)
{
    UNUSED_ALWAYS( pFileInfo );
    return m_bmOpr.EndProcess(ohObject.GetTimeout( _T("EndProcess") ));
}
BOOL CBootModeIntegOpr::EnableFlash(void * pFileInfo)
{
    UNUSED_ALWAYS( pFileInfo );
    return m_bmOpr.EnableFlash(ohObject.GetTimeout( _T("EnableFlash")));
}

BOOL CBootModeIntegOpr::EnableDebugMode(void * pFileInfo)
{
	BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
	return m_bmOpr.EnableDebugMode((LPBYTE)(pBMFileInfo->lpCode),(DWORD)(pBMFileInfo->llCodeSize),ohObject.GetTimeout(_T("EnableDebugMode")));
}

BOOL CBootModeIntegOpr::ReadSoftSimEID(void * pFileInfo)
{
	UNUSED_ALWAYS( pFileInfo );
    m_bmOpr.AllocRecvBuffer( 10 );
    if( !m_bmOpr.ReadSoftSimEID( ohObject.GetTimeout( _T("ReadSoftSimEID") ) ) )
    {
        m_bmOpr.FreeRecvBuffer();
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL CBootModeIntegOpr::ReadRefInfo(void * pFileInfo)
{

	BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;

	if (m_bmOpr.AllocRecvBuffer( MAX_REF_INFO_LEN ) && pBMFileInfo && pBMFileInfo->lpCode)
	{
		m_bmOpr.ReadRefInfo((LPBYTE)(pBMFileInfo->lpCode),(DWORD)(pBMFileInfo->llCodeSize),ohObject.GetTimeout(_T("ReadRefInfo")));
	}
	return TRUE;
	
}

BOOL CBootModeIntegOpr::WriteAPRInfo(void * pFileInfo)
{
	BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;

	m_bmOpr.WriteAPRInfo((LPBYTE)(pBMFileInfo->lpCode),(DWORD)(pBMFileInfo->llCodeSize),ohObject.GetTimeout(_T("WriteAPRInfo")));
	
	return TRUE;
}

BOOL CBootModeIntegOpr::ReadTransceiverType(void * pFileInfo)
{
	UNUSED_ALWAYS( pFileInfo );
	m_bmOpr.AllocRecvBuffer( 4 );
	if( !m_bmOpr.ReadTransceiverType( ohObject.GetTimeout( _T("ReadTransceiverType") ) ) )
	{
		m_bmOpr.FreeRecvBuffer();
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CBootModeIntegOpr::GetCheckBaudCrcType(void * pFileInfo)
{
	UNUSED_ALWAYS( pFileInfo );
	m_bmOpr.AllocRecvBuffer( 4 );
	if( !m_bmOpr.GetCheckBaudCrcType() )
	{
		m_bmOpr.FreeRecvBuffer();
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CBootModeIntegOpr::SendExtTable(void * pFileInfo)
{
	BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
	return m_bmOpr.SendExtTable((LPBYTE)pBMFileInfo->lpCode,(DWORD)pBMFileInfo->llCodeSize, ohObject.GetTimeout( _T("SendExtTable") ) ) ;
}

BOOL CBootModeIntegOpr::SetCompatibleItem(DWORD& dwError)
{
    BOOL bOK = FALSE;
    /// random data
    DWORD dwSupportRandomNum = m_bmOpr.GetSupportRandomNum();
    LPBYTE pRandomData = m_bmOpr.GetRandomData();
    BYTE bRandomDataOut[256]={0};
    if (dwSupportRandomNum > 0 && dwSupportRandomNum <=2)
    { 
        if (dwSupportRandomNum == ALG_AES)
        {
            if(g_theApp.m_pfDecryptData)
            {
                g_theApp.m_pfDecryptData(pRandomData, 32, bRandomDataOut, ohObject.GetKey(ALG_AES), ALG_AES);
            }

            bOK = m_bmOpr.SetRandomData((LPBYTE)(bRandomDataOut), 32, ohObject.GetTimeout( _T("SetRandomData")) );
        }    
        else
        {
            if(g_theApp.m_pfDecryptData)
            {
                g_theApp.m_pfDecryptData(pRandomData, 256, bRandomDataOut,ohObject.GetKey(ALG_RSA2048), ALG_RSA2048);
            }
            bOK = m_bmOpr.SetRandomData(((LPBYTE)(bRandomDataOut))+(256-32), 32, ohObject.GetTimeout( _T("SetRandomData")) );
        }

        if(!bOK)
        {
            return FALSE;
        }
    }

    /// disable transcode
    m_bNeedRepartion = TRUE;
    if (m_bmOpr.SupportNotTransCode())
    {
        m_bmOpr.DisableTransCode(ohObject.GetTimeout( _T("DisableTransCode") ));
        LogInfo(_T("NotTransCode Software Version"));
        PostMessageToUplevel( BM_UN_TRANSCODE,  (WPARAM)m_dwOprCookie, (LPARAM)0 ); 
    }

    if(ohObject.GetSerialDDRLogFlag())
    {
        // 不关心命令的返回值，因为工具只是通知uboot现在的工具是支持0xFF类型LogInfo的
        m_bmOpr.EnableUbootLog(ohObject.GetTimeout(_T("EnableUbootLog")));
    }

    /// set raw data
    m_dwFlushSize = m_bmOpr.GetFlushSize();
    if(m_bmOpr.SupportRawData())
    {
        bOK = m_bmOpr.EnableRawData(ohObject.GetTimeout( _T("EnableRawData")));
        if (!bOK)
        {
            return FALSE;
        }
    } 

    m_bmOpr.SetLastErrorCode(dwError);

    return TRUE;
}


BOOL CBootModeIntegOpr::ExecNandInit( void * pFileInfo )
{
	UNUSED_ALWAYS(pFileInfo);
    m_bRepartionDone = FALSE;
	m_bmOpr.AllocRecvBuffer( sizeof(DWORD) );
    BOOL bExcRet = m_bmOpr.Excute( ohObject.GetTimeout( _T("ExecNandInit") ) );
    
	if( bExcRet )
	{
		m_bmOpr.FreeRecvBuffer();	
		m_bNeedRepartion = FALSE;
        m_bRepartionDone = TRUE;
		return bExcRet;
	}
    m_bNeedRepartion = FALSE;
	DWORD dwError = m_bmOpr.GetLastErrorCode(); 

	m_bmOpr.FreeRecvBuffer();

    if(BSL_CHIPID_NOT_MATCH == dwError)
    {
        return FALSE; 
    }
    else if (BSL_REP_INCOMPATIBLE_PARTITION == dwError)
    {
        return SetCompatibleItem(dwError);  
    }
    else if (BSL_REP_96FMT_ERROR == dwError)
    {
        return FALSE;
    }
    /// check second hand memory
    else if (BSL_REP_OLD_MEMORY_REGISTER_CHANGE_ERROR == dwError || 
        BSL_REP_OLD_MEMORY_EMMC_HAS_FLAG_ERROR == dwError || 
        BSL_REP_OLD_MEMORY_RPMB_WRITTEN_ERROR == dwError || 
        BSL_REP_OLD_MEMORY_REGISTER_EMMC_ERROR == dwError || 
        BSL_REP_OLD_MEMORY_REGISTER_RPMB_ERROR == dwError || 
        BSL_REP_OLD_MEMORY_EMMC_RPMB_ERROR == dwError || 
        BSL_REP_OLD_MEMORY_REGISTER_EMMC_RPMB_ERROR == dwError)
    {
        if (!SetCompatibleItem(dwError))
        {
            return FALSE;
        }
        return FALSE;
    }
	else
	{
		return FALSE;
	}
    
	return TRUE;
	
/*
    
    DWORD dwStrategy = ohObject.GetRepartitionFlag();	
    DWORD dwReadFlashFlag = ohObject.GetReadFlashBefRepFlag();

	m_bRepartion = TRUE;

    switch( dwStrategy )
    {
        case REPAR_STRATEGY_DO_ALWAYS:     
        {
			bExcRet = TRUE;
			// Do repartition always
			if( dwReadFlashFlag == 0)
			{
				bExcRet = Repartition( pFileInfo );
			}
			
            break;
        }
        case REPAR_STRATEGY_STOP:
        {
            if( BSL_REP_INCOMPATIBLE_PARTITION == dwError )
            {
                // Report error
                bExcRet = FALSE;
            }
            break;
        }
        case REPAR_STRATEGY_IGNORE:
        {
            if( BSL_REP_INCOMPATIBLE_PARTITION == dwError )
            {
				m_bRepartion = FALSE;
                // Ignore this error
                bExcRet = TRUE;
				
            }
            break;
        }
        case REPAR_STRATEGY_DO:
        {
            if( BSL_REP_INCOMPATIBLE_PARTITION == dwError )
            {
				m_bRepartion = TRUE;
				bExcRet = TRUE;

                // Repartition
				if(dwReadFlashFlag == 0)
				{
					bExcRet = Repartition( pFileInfo );
				} 				
            }

            break;
        }
        default:
        {
            // Unknown settings
            ASSERT( FALSE );
        }
    }
  	
    return bExcRet; */
}

void CBootModeIntegOpr::SetMessageReceiver( BOOL bRcvThread, 
                                      LPCVOID pReceiver,
                                      DWORD dwCookie )
{
    m_dwOprCookie = dwCookie;
    m_bRcvThread = bRcvThread;
    
    if ( bRcvThread )
    {
        m_dwRcvThreadID = * (DWORD *) pReceiver;
    }
    else
    {
        m_hRcvWindow = *(HWND*)pReceiver;
    }
}    

void CBootModeIntegOpr::PostMessageToUplevel( opr_status_msg msgID, 
                                        WPARAM wParam, LPARAM lParam  )
{
    //AFX_MANAGE_STATE( AfxGetStaticModuleState() )

    if( (m_hRcvWindow == NULL) && ( m_dwRcvThreadID == 0) )
    {
        return;
    }
    
    if ( m_bRcvThread )  
    {
        if( m_dwRcvThreadID != 0 )
        {
            PostThreadMessage( m_dwRcvThreadID,  msgID, wParam , lParam );   
        }
    }
    else 
    {
        if( m_hRcvWindow != NULL )
        {
            PostMessage( m_hRcvWindow, msgID, wParam , lParam );            
        }
    }
}

BOOL CBootModeIntegOpr::Initialize(LPCTSTR lpszProgID, LPVOID pOpenArgument, 
                                   BOOL bBigEndian, long dwOprCookie, 
                                   BOOL bRcvThread, LPCVOID pReceiver) 
{
    m_bCheckBaudRate = FALSE;
    m_bStopOpr = FALSE;
	m_bNeedRepartion = TRUE;
	m_bRepartionDone = FALSE;

    SetMessageReceiver( bRcvThread, pReceiver, dwOprCookie );

    return m_bmOpr.Initialize( lpszProgID, (LPBYTE)pOpenArgument,
                                bBigEndian, dwOprCookie );
}

void CBootModeIntegOpr::Uninitialize() 
{
	m_bmOpr.Uninitialize();
	m_bCheckBaudRate   = TRUE;
}

void CBootModeIntegOpr::LogInfo(LPCTSTR lpszLog) 
{
	m_bmOpr.LogInfo( lpszLog );
}

void CBootModeIntegOpr::LogError(LPCTSTR lpszLog) 
{
	m_bmOpr.LogError( lpszLog );
}

long CBootModeIntegOpr::GetRecvBuffer() 
{
    return (long)m_bmOpr.GetRecvBuffer();	
}

void CBootModeIntegOpr::FreeRecvBuffer() 
{
    m_bmOpr.FreeRecvBuffer();
}

void CBootModeIntegOpr::StopOpr() 
{
    if (!m_bStopOpr)
    {
        m_bStopOpr = TRUE;
        m_bmOpr.StopOpr();
    }
    
}

long CBootModeIntegOpr::GetRecvBufferSize() 
{
	return m_bmOpr.GetRecvBufferSize();
}

long CBootModeIntegOpr::GetChannelPtr() 
{
    return (long)m_bmOpr.GetChannelPtr();
}

void CBootModeIntegOpr::SetChannel(long pChannel) 
{
    m_bmOpr.SetChannel( (ICommChannel*)pChannel );
}

long CBootModeIntegOpr::GetIDsOfNames(LPCTSTR lpszFunName)
{
	for(int i=0;i<(m_oprCount-1);i++)
	{
		if(_tcsicmp(m_oprEntries[i].lpszName,lpszFunName)==0)
		{
			return i;
		}
	}

	return -1;
}

BOOL CBootModeIntegOpr::Invoke(long id,void * pFileInfo)
{
	if(id<0 || id>=m_oprCount)
		return FALSE;
	CString strLog = "";
	strLog.Format(_T("%s +++"), m_oprEntries[id].lpszName);
	LogInfo(strLog.operator LPCTSTR());

	BOOL bOper = (this->*(m_oprEntries[id].pFun))(pFileInfo);

	strLog.Format(_T("%s ---"), m_oprEntries[id].lpszName);
	LogInfo(strLog.operator LPCTSTR());

	return bOper;
}

BOOL CBootModeIntegOpr::ReadSectorSize( void * pFileInfo )
{
	UNUSED_ALWAYS( pFileInfo );
    m_bmOpr.AllocRecvBuffer( sizeof(DWORD) );
    if( !m_bmOpr.ReadSectorSize( ohObject.GetTimeout( _T("ReadSectorSize") ) ) )
    {
        m_bmOpr.FreeRecvBuffer();
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL CBootModeIntegOpr::ReadFlashAndSave( void * pFileInfo )
{
	return _ReadFlashAndSave(pFileInfo,FALSE,FALSE);
}

BOOL CBootModeIntegOpr::ReadFlashAndSaveByID( void * pFileInfo )
{
	return _ReadFlashAndSave(pFileInfo,TRUE,FALSE);
}

BOOL CBootModeIntegOpr::ReadFlashAndDirectSave( void * pFileInfo )
{
	return _ReadFlashAndSave(pFileInfo,FALSE,FALSE,TRUE);
}

BOOL CBootModeIntegOpr::ReadFlashAndDirectSaveByID( void * pFileInfo )
{
	return _ReadFlashAndSave(pFileInfo,TRUE,FALSE,TRUE);
}

BOOL CBootModeIntegOpr::ReadFlashAndSaveByIDEx( void* pFileInfo )
{
    return _ReadFlashAndSave(pFileInfo,TRUE,TRUE);
}

DWORD CBootModeIntegOpr::GetOprLastErrorCode()
{
	return m_bmOpr.GetLastErrorCode();
}

BOOL CBootModeIntegOpr::ReadFlashToFile(void * pFileInfo, HANDLE hFile)
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
    

    DWORD dwBase = (DWORD)(pBMFileInfo->llBase);
    DWORD dwLeft = (DWORD)(pBMFileInfo->llOprSize);
    DWORD dwMaxLength = pBMFileInfo->dwMaxLength;    
    DWORD dwSize = dwMaxLength;    
	DWORD dwReadFlashTimeout = ohObject.GetTimeout( _T("Read Flash") );

    PostMessageToUplevel( BM_READ_FLASH, (WPARAM)m_dwOprCookie, (LPARAM)100 );
    
    if( dwBase & 0x80000000 )
    {
       DWORD dwOffset = 0;
       while(dwLeft > 0 && !m_bStopOpr )
       {		    
            if(dwLeft > dwMaxLength )
            {
                dwSize = dwMaxLength ;                
            }
            else
            {
                dwSize = dwLeft;
            }

			m_bmOpr.AllocRecvBuffer( dwSize );
        
            if( !m_bmOpr.ReadPartitionFlash( dwBase, dwSize, dwOffset, dwReadFlashTimeout) )
            {
                m_bmOpr.FreeRecvBuffer();
                return FALSE;
            }
            dwOffset += dwSize;
            dwLeft -= dwSize;

			//write to file
			DWORD dwWrite = 0;
			BOOL bOK = WriteFile(hFile,m_bmOpr.GetRecvBuffer(),m_bmOpr.GetRecvBufferSize(),&dwWrite,NULL);
			m_bmOpr.FreeRecvBuffer();
			if(!bOK)
			{
				_TCHAR szTmp[MAX_PATH]={0};
				_stprintf(szTmp,_T("Read flash data to file failed, [ErrorCode:0x%X]."),::GetLastError());
				LogError(szTmp);	
				return FALSE;
			}

            PostMessageToUplevel( BM_READ_FLASH_PROCESS, 
                (WPARAM)m_dwOprCookie, 
                (LPARAM)((pBMFileInfo->llOprSize - dwLeft)*100/pBMFileInfo->llOprSize) );        
        }
    }
    else
    {
        while(dwLeft > 0 && !m_bStopOpr )
        {
            if(dwLeft > dwMaxLength )
            {
                dwSize = dwMaxLength ;                
            }
            else
            {
                dwSize = dwLeft;
            }
        
			m_bmOpr.AllocRecvBuffer( dwSize );

            if( !m_bmOpr.ReadFlash( dwBase, dwSize, dwReadFlashTimeout ) )
            {
                m_bmOpr.FreeRecvBuffer();
                return FALSE;
            }
            dwBase += dwSize;
            dwLeft -= dwSize;

			//write to file
			DWORD dwWrite   =  0;
			DWORD dwBufSize  =  m_bmOpr.GetRecvBufferSize();
			BOOL bOK = WriteFile(hFile,m_bmOpr.GetRecvBuffer(),dwBufSize,&dwWrite,NULL);
			m_bmOpr.FreeRecvBuffer();
			if(!bOK || dwWrite != dwBufSize)
			{
				_TCHAR szTmp[MAX_PATH]={0};
				_stprintf(szTmp,_T("Write flash data to file failed, [ErrorCode:0x%X]."),::GetLastError());
				LogError(szTmp);	
				return FALSE;
			}


            PostMessageToUplevel( BM_READ_FLASH_PROCESS, 
                (WPARAM)m_dwOprCookie, 
                (LPARAM)((pBMFileInfo->llOprSize - dwLeft)*100/pBMFileInfo->llOprSize) );        
        }
    }

    if( m_bStopOpr )
        return FALSE;
    return TRUE;
}

BOOL CBootModeIntegOpr::ReadFlashToFileByID(void * pFileInfo, HANDLE hFile, BOOL bIs64Bit/* = FALSE*/)
{
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
    
	DWORD dwReadStartTimeout = ohObject.GetTimeout( _T("StartRead") );
	

	if (0 == pBMFileInfo->llOprSize)
	{
		CString strError ;
		strError.Format(_T("[%s] read size is zero."),pBMFileInfo->szFileID);
		LogError(strError.operator LPCTSTR());	
		return FALSE;
	}

    m_bmOpr.AllocRecvBuffer( 8 );
    unsigned __int64 llLeft = pBMFileInfo->llOprSize;
	

	if(!m_bmOpr.StartRead((LPBYTE)pBMFileInfo->szRepID,MAX_REP_ID_LEN*2,pBMFileInfo->llOprSize,dwReadStartTimeout,bIs64Bit))
	{
        if(m_bmOpr.GetLastErrorCode() == BSL_REP_DOWN_SIZE_ERROR && m_bmOpr.GetRecvBufferSize() == 8 && NULL != m_bmOpr.GetRecvBuffer())
        {
			llLeft = *(unsigned __int64*) m_bmOpr.GetRecvBuffer();
        }
        else
        {
			m_bmOpr.FreeRecvBuffer();
            return FALSE;
        }	
	}
    m_bmOpr.FreeRecvBuffer();
	unsigned __int64 llTotal = llLeft;
   
    DWORD dwMaxLength = pBMFileInfo->dwMaxLength;    
    DWORD dwSize = dwMaxLength;    

    PostMessageToUplevel( BM_READ_FLASH, (WPARAM)m_dwOprCookie, (LPARAM)100 );
    
    DWORD dwMidstReadTimeout = ohObject.GetTimeout( _T("MidstRead") );
  
   unsigned __int64 llOffset = 0;
   while(llLeft > 0 && !m_bStopOpr )
   {		    
        if(llLeft > dwMaxLength )
        {
            dwSize = dwMaxLength ;                
        }
        else
        {
            dwSize = (DWORD)llLeft;
        }

		m_bmOpr.AllocRecvBuffer( dwSize );
    
        if( !m_bmOpr.MidstRead( dwSize,llOffset, 
             dwMidstReadTimeout,bIs64Bit) )
        {
            m_bmOpr.FreeRecvBuffer();
            return FALSE;
        }
        llOffset += dwSize;
        llLeft -= dwSize;

		//write to file
		DWORD dwWrite = 0;
		BOOL bOK = WriteFile(hFile,m_bmOpr.GetRecvBuffer(),m_bmOpr.GetRecvBufferSize(),&dwWrite,NULL);
		m_bmOpr.FreeRecvBuffer();
		if(!bOK)
		{
			_TCHAR szTmp[MAX_PATH]={0};
			_stprintf(szTmp,_T("Write flash data to file failed, [ErrorCode:0x%X]."),::GetLastError());
			LogError(szTmp);	
			return FALSE;
		}

        PostMessageToUplevel( BM_READ_FLASH_PROCESS, 
            (WPARAM)m_dwOprCookie, 
            (LPARAM)((llTotal - llLeft)*100/llTotal) );        
    }
    
    if( m_bStopOpr )
        return FALSE;

	DWORD dwEndReadTimeout = ohObject.GetTimeout( _T("EndRead") );
	m_bmOpr.EndRead( dwEndReadTimeout);

    return TRUE;
}

BOOL CBootModeIntegOpr::DoNothing(void * pFileInfo)
{
	UNUSED_ALWAYS(pFileInfo);
	return TRUE;
}

BOOL CBootModeIntegOpr::SetCRC(void * pFileInfo) 
{
	UNUSED_ALWAYS(pFileInfo);
	m_bmOpr.SetIsCheckCrc( TRUE );
	m_PacketThread.m_pkg.SetCrc(TRUE);
	return TRUE;    
}

BOOL CBootModeIntegOpr::ResetCRC(void * pFileInfo) 
{
	UNUSED_ALWAYS(pFileInfo);
	m_bmOpr.SetIsCheckCrc( FALSE );
	m_PacketThread.m_pkg.SetCrc(FALSE);
	return TRUE;    
}

void  CBootModeIntegOpr::OnBegin()
{
	PostMessageToUplevel( BM_BEGIN,  (WPARAM)m_dwOprCookie, (LPARAM)0 );
}

void  CBootModeIntegOpr::OnEnd()
{
	PostMessageToUplevel( BM_END,  (WPARAM)m_dwOprCookie, (LPARAM)0 );
}

BOOL CBootModeIntegOpr::KeepCharge(void * pFileInfo)
{
	UNUSED_ALWAYS( pFileInfo );    	
	DWORD dwTimeout = ohObject.GetTimeout( _T("KeepCharge") );
	return m_bmOpr.KeepCharge( dwTimeout );
}

BOOL CBootModeIntegOpr::DdrCheck(void * pFileInfo)
{
    UNUSED_ALWAYS( pFileInfo );    	
    DWORD dwTimeout = ohObject.GetTimeout( _T("DDRCheck") );
    return m_bmOpr.DdrCheck( dwTimeout );
}

BOOL CBootModeIntegOpr::SelfRefresh(void * pFileInfo)
{
    UNUSED_ALWAYS( pFileInfo );    	
    DWORD dwTimeout = ohObject.GetTimeout( _T("SelfRefresh") );
    return m_bmOpr.SelfRefresh( dwTimeout );
}

BOOL CBootModeIntegOpr::ReadNandBlockInfo(void * pFileInfo)
{
    UNUSED_ALWAYS( pFileInfo );
    m_bmOpr.AllocRecvBuffer( sizeof(DWORD)*3 );
    if( !m_bmOpr.ReadNandBlockInfo( ohObject.GetTimeout( _T("ReadNandBlockInfo") ) ) )
    {
        m_bmOpr.FreeRecvBuffer();
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

unsigned int CBootModeIntegOpr::_CheckSum32(unsigned int nChkSum, unsigned char const *pDataBuf, unsigned int nLen)
{
    for(unsigned int i= 0; i< nLen; ++i)
    {
        nChkSum += (BYTE)(*(pDataBuf+i));
    }
    return nChkSum;
}

BOOL CBootModeIntegOpr::_DoCheckSum(PBMFileInfo pBMFileInfo)
{
    
    DWORD dwCheckSum    = 0;
    BOOL bOK            = TRUE;  
    //NV data
    if(_tcsnicmp(pBMFileInfo->szFileID,_T("NV"),2)==0 || 
        _tcsnicmp(pBMFileInfo->szFileID,_T("_CHECK_NV"),9)==0 )
    {        
        pBMFileInfo->dwCheckSum =m_bmOpr.DoNVCheckSum((LPBYTE)pBMFileInfo->lpCode,(DWORD)(pBMFileInfo->llCodeSize));
        bOK = pBMFileInfo->dwCheckSum==0 ? FALSE :TRUE;
        return bOK;
    }
    if( !ohObject.IsNeedDoChkSum() ||
        _tcsnicmp(pBMFileInfo->szFileType,_T("FDL"),3)== 0 ||
        _tcsicmp(pBMFileInfo->szFileType,_T("NAND_FDL"))== 0
       )
    {
        pBMFileInfo->dwCheckSum = 0;
        return TRUE;
    }

    
    unsigned __int64 llLeft = pBMFileInfo->llCodeSize;
    BOOL bNeedRemap         = FALSE;

    if( 
        !pBMFileInfo->bChangeCode && 
        pBMFileInfo->dwFirstMapSize < pBMFileInfo->llCodeSize &&
        pBMFileInfo->hFDLCode != NULL
      )
    {
        if (0 == pBMFileInfo->dwCheckSum)
        { 
			LARGE_INTEGER liOffset;
			bNeedRemap = TRUE;
			liOffset.QuadPart = pBMFileInfo->llCodeOffset;
			if (::SetFilePointer(pBMFileInfo->hFDLCode,liOffset.LowPart, &liOffset.HighPart,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			{
				TCHAR szError[MAX_PATH] = {0};
				_stprintf(szError,_T("SetFilePointer failed in _DoCheckSum, ErrorCode=0x%x\n"),GetLastError());
				LogError(szError);	
				return FALSE;
			}
        }
        else
        {
            return TRUE;
        }      
    }
    else
    {
        pBMFileInfo->dwCheckSum = 0;
        pBMFileInfo->dwCheckSum = _CheckSum32(pBMFileInfo->dwCheckSum,(LPBYTE)pBMFileInfo->lpCode,(DWORD)(pBMFileInfo->llCodeSize));
        if(!_CRCCheck(pBMFileInfo))
        {
            return FALSE;
        }
    }

    if(bNeedRemap)
    {
        if (NULL == m_pFileBuf)
        {
            LogError(_T("CBootModeIntegOpr::_DoCheckSum fail on NULL == m_pFileBuf."));	
            return FALSE;
        }
        while(llLeft != 0 && !m_bStopOpr)
        {
            DWORD dwReadSize = llLeft > MAX_MAP_SIZE ? MAX_MAP_SIZE : (DWORD)llLeft;
            DWORD dwReaded = 0;
            memset(m_pFileBuf,0,MAX_MAP_SIZE);
            if(!::ReadFile(pBMFileInfo->hFDLCode,m_pFileBuf,dwReadSize,&dwReaded,NULL) || dwReaded != dwReadSize)
            {
                LogError(_T("CBootModeIntegOpr::_DoCheckSum fail on ReadFile."));	
                bOK = FALSE;
                break;
            }
            dwCheckSum  = _CheckSum32(dwCheckSum,m_pFileBuf,dwReadSize);  
            llLeft	-= dwReadSize;
        }
        if (0 == llLeft)
        {
            pBMFileInfo->dwCheckSum = dwCheckSum;
            if(!_CRCCheck(pBMFileInfo))
            {
                return FALSE;
            }
        }
    }

    return bOK;
}

BOOL CBootModeIntegOpr::_CRCCheck(PBMFileInfo pBMFileInfo)
{
    if(_tcsicmp(pBMFileInfo->szFileID,_T("prodnv")) != 0 &&
        _tcsicmp(pBMFileInfo->szFileID,_T("phasecheck")) != 0)
    {
		if(ohObject.IsGenCrcCfgFile())
		{
			m_bmOpr.SetCheckSumCfg(pBMFileInfo->szFileID, pBMFileInfo->dwCheckSum);
		}

		if(ohObject.IsEnableCrcCfgCheck())
		{
			CString strCrcFile = ohObject.GetCrcFile();
			if(pBMFileInfo->dwCheckSum != m_bmOpr.GetCheckSumCfg(pBMFileInfo->szFileID, strCrcFile.GetBuffer(0)))
			{
				CString strError ;
				strError.Format(_T("Crc Check Failed at [%s]."),pBMFileInfo->szFileID);
				LogError(strError.operator LPCTSTR());
				m_bmOpr.SetLastErrorCode(BSL_ERROR_CRC_CHECK);
				return FALSE;
			}
		}
    }
    return TRUE;
}

BOOL CBootModeIntegOpr::_Download(void * pFileInfo,BOOL bByID, BOOL bIs64Bit/* = FALSE*/)
{	
	/* 
	 * need not send this message, because it will be mis-understand.
    PostMessageToUplevel( BM_ERASE_FLASH, 
        (WPARAM)m_dwOprCookie, (LPARAM)0 );
	*/

    m_bmOpr.FreeRecvBuffer();
    BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
//     DWORD dwTimeout = ohObject.GetTimeout( _T("Erase Flash") );
//     if( dwTimeout <= MAX_ERASEFLASH_TIMEOUT )
//     {
//         dwTimeout = (DWORD)(pBMFileInfo->llCodeSize / SECTION_SIZE * dwTimeout + dwTimeout);
//     }
	DWORD dwTimeout = ohObject.GetTimeout( _T("StartData") );
    PostMessageToUplevel( BM_DOWNLOAD, (WPARAM)m_dwOprCookie, (LPARAM)100 );
    if (0 == pBMFileInfo->llCodeSize)
    {
        CString strError ;
        strError.Format(_T("[%s] file size is zero."),pBMFileInfo->szFileID);
        LogError(strError.operator LPCTSTR());	
        return FALSE;
    }

    if (!_DoCheckSum(pBMFileInfo))
    {
        CString strError ;
        strError.Format(_T("[%s] _DoCheckSum fail."),pBMFileInfo->szFileID);
        LogError(strError.operator LPCTSTR());	
        return FALSE;
    }
	unsigned long long lCodeSize = pBMFileInfo->llCodeSize;

	if(!bByID)
	{
		if(!m_bmOpr.StartData( 
			pBMFileInfo->llBase, 
			pBMFileInfo->llCodeSize, 
			(LPBYTE)pBMFileInfo->lpCode,
            pBMFileInfo->dwCheckSum,
			dwTimeout,
            bIs64Bit) )
		{
			return FALSE;
		}
	}
	else
	{		
		if(!m_bmOpr.StartData( 
			(LPBYTE)pBMFileInfo->szRepID, 
			MAX_REP_ID_LEN*2,
			lCodeSize, 
			(LPBYTE)pBMFileInfo->lpCode,
            pBMFileInfo->dwCheckSum,
			dwTimeout,
            bIs64Bit) )
		{
			return FALSE;
		}
	}	

	DWORD dwDownloadTimeout = ohObject.GetTimeout( _T("Download") );	

#if 1
	unsigned __int64 llTotal = lCodeSize;
	unsigned __int64 llRealTotal = 0;
	int nCrc = m_bmOpr.GetCrcType();
	if(nCrc != -1)
	{
		m_PacketThread.m_pkg.SetCrc(nCrc==1?TRUE:FALSE);
	}	
    if(m_bmOpr.SupportNotTransCode())
    {
        pBMFileInfo->dwMaxLength = ohObject.GetPacketLength( _T("Download") );
    }

    if(!m_bmOpr.SupportRawData())
    {
        m_PacketThread.SetFileInfo(pBMFileInfo);
        m_PacketThread.Start(!m_bmOpr.SupportNotTransCode());
        ESCAPE_PACKET_PTR pPacket = NULL;

	    while(m_PacketThread.GetOnePacket(&pPacket) && !m_bStopOpr)
	    {
		    if( !m_bmOpr.MidstDataFast( pPacket->dwSize, pPacket->pBuf, dwDownloadTimeout, pPacket->llTotal) )
		    {
			    m_PacketThread.Stop();
			    return FALSE;
		    }	

		    llRealTotal = pPacket->llTotal;
		    PostMessageToUplevel( BM_DOWNLOAD_PROCESS, 
			    (WPARAM)m_dwOprCookie, 
			    (LPARAM)(pPacket->llTotal*100/lCodeSize));

		    if(pPacket->llTotal == llTotal)
			    break;
	    }
	    if(!m_bStopOpr && llRealTotal != llTotal)
	    {		
		    CString strErr;
		    strErr.Format(_T("Read file fail, [ERROR:0x%X]."),GetLastError());
		    LogError(strErr.operator LPCTSTR());
		    m_PacketThread.Stop();
		    return FALSE;
	    }
	    m_PacketThread.Stop();
    }
    else
    {
        DWORD dwRawDataVersion = m_bmOpr.GetSupportRawData();

        if (dwRawDataVersion == 2)
        {
            if (!m_bmOpr.MidstRawStart2(ohObject.GetTimeout( _T("MidstRawStart"))))
            {
                return FALSE;
            }
        }

        unsigned __int64 nTempRawDataSize = 0;
        //int nFlushSize = m_dwFlushSize;
        //sha256_context ctx;
        MIDST_RAW_DATA midstRawData;

        m_PacketThread.SetFileInfo(pBMFileInfo);
        m_PacketThread.Start(!m_bmOpr.SupportNotTransCode(), m_bmOpr.SupportRawData(), m_dwFlushSize);
        ESCAPE_PACKET_PTR pPacket = NULL;

        while(m_PacketThread.GetOnePacket(&pPacket) && !m_bStopOpr)
        {
            if (1 == dwRawDataVersion)
            {
                midstRawData.llOffset = nTempRawDataSize;
                midstRawData.nLength = pPacket->dwSize;
                if(!m_bmOpr.MidstRawStart( sizeof(MIDST_RAW_DATA), (LPBYTE)&midstRawData, ohObject.GetTimeout( _T("MidstRawStart"))))
                {
                    m_PacketThread.Stop();
                    return FALSE;
                }
            }           

            //sha256_starts( &ctx );

            if( !m_bmOpr.MidstDataFast( pPacket->dwSize, pPacket->pBuf, dwDownloadTimeout, pPacket->dwSize) )
            {
                m_PacketThread.Stop();
                return FALSE;
            }
            else
            {
                nTempRawDataSize += pPacket->dwSize;
                //sha256_update( &ctx, pFakeBuffer, nFlushSize);
            }

            //uint8 digest[32] = {0};
            //sha256_finish( &ctx, digest);
            //m_bmOpr.FlushData(32, digest, ohObject.GetTimeout( _T("FlushData")));

            llRealTotal = pPacket->llTotal;
            PostMessageToUplevel( BM_DOWNLOAD_PROCESS, 
                (WPARAM)m_dwOprCookie, 
                (LPARAM)(nTempRawDataSize*100/lCodeSize));

            if(pPacket->llTotal == llTotal)
                break;
        }

        if(!m_bStopOpr && llRealTotal != llTotal)
        {		
            CString strErr;
            strErr.Format(_T("Read file fail, [ERROR:0x%X]."),GetLastError());
            LogError(strErr.operator LPCTSTR());
            m_PacketThread.Stop();
            return FALSE;
        }
        m_PacketThread.Stop();

    }
#else


    DWORD dwLeft = pBMFileInfo->dwCodeSize;
    DWORD dwMaxLength = pBMFileInfo->dwMaxLength;
    DWORD dwSize = dwMaxLength;	

	BOOL bNeedRemap = FALSE;
	LPBYTE pCurData = (LPBYTE)pBMFileInfo->lpCode;
	DWORD  dwLeft2  = pBMFileInfo->dwFirstMapSize;
	DWORD  dwCurOffset = 0;
	DWORD  dwCurMapSize = pBMFileInfo->dwFirstMapSize;
	LPBYTE pMapBase = NULL;

	if( !pBMFileInfo->bChangeCode && 
		pBMFileInfo->dwFirstMapSize < pBMFileInfo->dwCodeSize &&
		pBMFileInfo->hFDLCode != NULL &&
		pBMFileInfo->hFDLCodeMapView != NULL)
	{
		bNeedRemap = TRUE;
	}
	else
	{
		dwLeft2 = dwLeft;  // no file map
	}

	do 
	{		
		while( dwLeft2 > 0 && !m_bStopOpr )
		{
			if(dwLeft2 >= dwMaxLength )
			{
				dwSize = dwMaxLength ;    
			}
			else
			{
				dwSize = dwLeft2;
				if(bNeedRemap && (( dwCurMapSize + dwCurOffset ) < pBMFileInfo->dwCodeSize))
				{
					break;
				}
			}	
		
			if( !m_bmOpr.MidstData( dwSize, pCurData, dwDownloadTimeout, pBMFileInfo->dwCodeSize - dwLeft + dwSize) )
			{
				SAFE_UNMAP_FILE(pMapBase);
				return FALSE;
			}			
	
			dwLeft2 -= dwSize;
			dwLeft  -= dwSize;
			pCurData += dwSize;
			
			PostMessageToUplevel( BM_DOWNLOAD_PROCESS, 
				(WPARAM)m_dwOprCookie, 
				(LPARAM)(pPacket->llTotal*100/pBMFileInfo->llCodeSize) );
		
		}

		SAFE_UNMAP_FILE(pMapBase);				
		if(bNeedRemap && dwLeft != 0)
		{
			dwCurOffset += dwCurMapSize;
			//DWORD dwLeft2Tail = (((dwLeft2 + 0x10000 - 1) / 0x10000)*0x10000);
			DWORD dwLeft2Tail = dwLeft2 > 0 ? 0x10000 : 0;
			if(dwLeft2 != 0)
			{
				dwCurOffset -=  dwLeft2Tail;
			}

			if((dwLeft + dwLeft2) < MAX_MAP_SIZE)
			{
				dwCurMapSize =  dwLeft + dwLeft2Tail;				
			}
			else
			{
				dwCurMapSize = MAX_MAP_SIZE;
			}

			if( (dwCurOffset + dwCurMapSize) > pBMFileInfo->dwCodeSize )
			{
				dwCurMapSize =  pBMFileInfo->dwCodeSize - dwCurOffset;
			}

			
			pCurData = NULL;			

			pMapBase = (LPBYTE)::MapViewOfFile(pBMFileInfo->hFDLCodeMapView,FILE_MAP_READ,0,dwCurOffset,dwCurMapSize);
			if(pMapBase == NULL)
			{
				CString strErr;
				strErr.Format(_T("MapViewOfFile failed, [ERROR:0x%X]."),GetLastError());
				m_bmOpr.LogError(strErr);
				return FALSE;
			}
			else
			{
				DWORD dwDlt = 0;
				pCurData = pMapBase;
				if(dwLeft2 != 0)
				{
					dwDlt = dwLeft2Tail - dwLeft2;
					pCurData += dwDlt;					 
				}
				dwLeft2 = dwCurMapSize - dwDlt;								
			}
		}

	}while(dwLeft != 0);
	SAFE_UNMAP_FILE(pMapBase);

#endif
    if( m_bStopOpr )
    {
        return FALSE;
    }
    if( !m_bmOpr.EndData( ohObject.GetTimeout( _T("EndData") ) ) )
        return FALSE;
    return TRUE;
}

BOOL CBootModeIntegOpr::_ReadFlashAndSave( void * pFileInfo , BOOL bByID, BOOL bIs64Bit/* = FALSE*/,BOOL bDirectSave/* = FALSE*/)
{
	BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
	
	if(_tcslen(pBMFileInfo->szFileName) == 0)
	{
		LogError(_T("File name to save falsh data is empty."));
		return FALSE;
	}
	
	_TCHAR szFileName[MAX_PATH]={0};
	_TCHAR szFileNameTmp[MAX_PATH]={0};
	if (bDirectSave)
	{
		_tcscpy(szFileName,pBMFileInfo->szFileName);
	}
	else
	{
		_tcscpy(szFileNameTmp,pBMFileInfo->szFileName);

		_TCHAR *pFind = _tcsrchr(szFileNameTmp,_T('\\'));
		if(pFind != NULL)
		{
			_TCHAR *pFindDot = _tcsrchr(szFileNameTmp,_T('.'));
			if(pFindDot == NULL || pFindDot <= pFind )
			{
				_stprintf(szFileName,_T("%s_COM%d"),szFileNameTmp,m_dwOprCookie);
			}
			else// if(pFindDot > pFind)
			{
				*pFindDot = _T('\0');
				_stprintf( szFileName,_T("%s_COM%d.%s"), szFileNameTmp, m_dwOprCookie, pFindDot + 1);
			}
		}
		else
		{
			_stprintf(szFileName,_T("%s_COM%d"),szFileNameTmp,m_dwOprCookie);
		}	
	}
	
	HANDLE hFile = CreateFile(szFileName,
		GENERIC_WRITE,
		FILE_SHARE_READ,           //Exclusive Open
		NULL,                      //Can't Be inherited
		CREATE_ALWAYS,             //Create always
		FILE_ATTRIBUTE_NORMAL,     
		NULL);
	if( hFile == INVALID_HANDLE_VALUE )
	{
		
		_TCHAR szTmp[MAX_PATH]={0};
		_stprintf(szTmp,_T("Create file [%s] failed, [ErrorCode:0x%X]."),szFileName,::GetLastError());
		LogError(szTmp);
		m_bmOpr.SetLastErrorCode(BSL_REP_CREATE_READBACK_FILE_ERROR);
		return FALSE;
	}	
	
	BOOL bOK = FALSE;
	
	if(!bByID)
	{
		bOK = ReadFlashToFile(pFileInfo,hFile);
	}
	else
	{
		bOK = ReadFlashToFileByID(pFileInfo,hFile,bIs64Bit);
	}
	
	CloseHandle(hFile);	
	hFile = INVALID_HANDLE_VALUE;

	if(bOK)
	{		
		LogInfo(_T("Save readed flash data success."));
		return TRUE;
	}
	else
	{
		//::DeleteFile(szFileName);
		LogError(_T("Save readed flash data failed."));
		return FALSE;
	}	
}

void CBootModeIntegOpr::EnablePortSecondEnum(BOOL bEnable)
{
	m_bmOpr.EnableCheckBaudCrc(bEnable);
}

BOOL CBootModeIntegOpr::IsNeed2ndEnumPort()
{
    return m_bmOpr.IsNeed2ndEnumPort();
}

BOOL CBootModeIntegOpr::SetTimeStamp(void* pFileInfo) 
{  
    UNUSED_ALWAYS( pFileInfo );
    LogInfo(_T("Set time stamp"));
    return m_bmOpr.SetTimeStamp(ohObject.GetTimeout( _T("SetTimeStamp")) );
}

BOOL CBootModeIntegOpr::ReadPartitionInfo(void* pFileInfo) 
{  
    UNUSED_ALWAYS( pFileInfo );
    LogInfo(_T("Read partition from phone"));
    m_bmOpr.AllocRecvBuffer( sizeof(PARTITION_T_UINT32) * MAX_PARTITION_NUM );
    return m_bmOpr.ReadPartitionInfo(ohObject.GetTimeout( _T("ReadPartitionInfo")) );
}

void CBootModeIntegOpr::OpenLog(long dwOprCookie )
{
	return m_bmOpr.OpenLog(dwOprCookie );
}

void CBootModeIntegOpr::CloseLog( DWORD dwError, LPCTSTR lpStep )
{
	return m_bmOpr.CloseLog( dwError, lpStep );
}

BOOL CBootModeIntegOpr::DumpUbootLog( void* pFileInfo )
{
	if (!m_bmOpr.SupportUbootLogDump())
	{
		LogInfo(_T("Dump Uboot log has not been supported yet.."));
		return TRUE;
	}
	UNUSED_ALWAYS( pFileInfo );
	LogInfo(_T("Dump Uboot log"));
	m_bmOpr.AllocRecvBuffer( MAX_BM_PKG_SIZE );
	return m_bmOpr.DumpUbootLog(ohObject.GetTimeout( _T("DumpUbootLog")) );
}

BOOL CBootModeIntegOpr::ChangeSelinux(void * pFileInfo)
{
	BMFileInfo* pBMFileInfo = (BMFileInfo*)pFileInfo;
	return m_bmOpr.ChangeSelinux((LPBYTE)(pBMFileInfo->lpCode),(DWORD)(pBMFileInfo->llCodeSize),ohObject.GetTimeout(_T("ChangeSelinux")));
}
