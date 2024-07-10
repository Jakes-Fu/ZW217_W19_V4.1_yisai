// OptionHelpper.cpp: implementation of the COptionHelpper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BMPlatform.h"
#include "OptionHelpper.h"
#include "Global.h"

COptionHelpper ohObject;

#define DEFAULT_CHECK_BAUDRATE_TIMES 3
#define DEFAULT_MAX_LENGTH 0x3000
#define INVALID_OPTION_VALUE -1
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptionHelpper::COptionHelpper()
{
    m_nCheckBaudTimes = INVALID_OPTION_VALUE;
    m_nRepartitionFlag = INVALID_OPTION_VALUE;
	m_nReadFlashBRFlag = INVALID_OPTION_VALUE;
	m_strProduct = _T("");
	m_bChangeTimeOutSetting = FALSE;
	m_bChangePacketLenSetting = FALSE;
	m_bEnableSecondEnum = FALSE;
    m_bPowerOff         = FALSE;
	m_bUartDownload		= FALSE;
	m_dwRomCodeBaudrate = DEFAULT_BAUDRATE;
	m_strLogPath.Empty();
}

COptionHelpper::~COptionHelpper()
{

}

int COptionHelpper::GetCheckBaudTimes( const _TCHAR* lpszFileType )
{
    if( INVALID_OPTION_VALUE != m_nCheckBaudTimes )
    {
        // It is changed by client,do not load from ini file
        return m_nCheckBaudTimes;
    }

    _TCHAR szConfigIniFile[MAX_PATH];
    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMTimeout.ini") );
    
    return GetPrivateProfileInt( _T("Check Baud Times") , lpszFileType, DEFAULT_CHECK_BAUDRATE_TIMES, szConfigIniFile);    
}

int COptionHelpper::GetCheckDUTTimes()
{
	_TCHAR szConfigIniFile[MAX_PATH];

	GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
	_tcscat( szConfigIniFile,  _T("\\BMTimeout.ini") );

	return GetPrivateProfileInt( _T("CheckDUT") , _T("CheckTimes"), 0, szConfigIniFile);    
}

int COptionHelpper::GetTimeout( _TCHAR* lpszOperation )
{
    _TCHAR szConfigIniFile[MAX_PATH];
    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMTimeout.ini") );

	CString strSection = _T("Timeout");
	int nTimeout = 1000;
	nTimeout = GetPrivateProfileInt( strSection , lpszOperation, 1000, szConfigIniFile);    
	if(m_bChangeTimeOutSetting && !m_strProduct.IsEmpty())
	{
		strSection = m_strProduct;
		nTimeout = GetPrivateProfileInt( strSection , lpszOperation, nTimeout, szConfigIniFile); 
	}
    
    return nTimeout;    
}
int  COptionHelpper::GetInterval( _TCHAR* lpszOperation )
{
    _TCHAR szConfigIniFile[MAX_PATH];

    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMTimeout.ini") );
    int nTimeout = 0;
    nTimeout = GetPrivateProfileInt( _T("Interval") , lpszOperation, 0, szConfigIniFile);    

    return nTimeout;    
}

int COptionHelpper::Get7ENumOnce()
{
	_TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMTimeout.ini") );

	return GetPrivateProfileInt( _T("CheckBaudSetting") , _T("7ENumPerTime"), 1, szConfigIniFile);  
}

int COptionHelpper::GetPacketLength( _TCHAR* lpszFileType )
{
    _TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );

	CString strSection = _T("Max Length");
    int nLen =  GetPrivateProfileInt( strSection, lpszFileType,DEFAULT_MAX_LENGTH, szConfigIniFile);

	
	if(m_bChangePacketLenSetting && !m_strProduct.IsEmpty())
	{
		strSection = m_strProduct;
		nLen = GetPrivateProfileInt( strSection , lpszFileType, nLen, szConfigIniFile); 
	}

	return nLen;
}

int COptionHelpper::GetRepartitionFlag()
{
    if( INVALID_OPTION_VALUE != m_nRepartitionFlag )
    {
        // It is changed by client,do not load from ini file
        return m_nRepartitionFlag;
    }    

    _TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );
    
    return GetPrivateProfileInt( _T("Repartition"), _T("strategy"),1, szConfigIniFile);
}

BOOL COptionHelpper::GetFileOperations( _TCHAR* lpszFileType,CStringArray* pOperations )
{
    _TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );
    
    _TCHAR szSection[MAX_PATH];
    GetPrivateProfileSection( lpszFileType, szSection, MAX_PATH, szConfigIniFile );
    
    
    return EnumKeys(szSection,pOperations);        
}

// Format of pSection
// "Key1 = data1\0Key2 = data2\0....Keyn = datan\0\0"
int COptionHelpper::EnumKeys(_TCHAR* pSection,CStringArray* pKeys)
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

void COptionHelpper::SetCheckBaudTimes( int nTimes )
{
    m_nCheckBaudTimes = nTimes;
}

void COptionHelpper::SetRepartitionFlag( int nFlag )
{
    m_nRepartitionFlag = nFlag;
}

void COptionHelpper::SetReadFlashBefRepFlag(int nFlag)
{
	m_nReadFlashBRFlag = nFlag;
}

int COptionHelpper::GetNVItemID()
{
    _TCHAR szConfigIniFile[MAX_PATH];
    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMTimeout.ini") );
    
    return GetPrivateProfileInt( _T("NVItem"), _T("ItemID"), 0x0, szConfigIniFile); 
}

int COptionHelpper::GetLogFlag()
{
    _TCHAR szConfigIniFile[MAX_PATH];
    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMTimeout.ini") );
    
    return GetPrivateProfileInt( _T("Log"), _T("Enable"), 0x0, szConfigIniFile); 
}

int COptionHelpper::GetReadFlashBefRepFlag()
{
    if( INVALID_OPTION_VALUE != m_nReadFlashBRFlag )
    {
        // It is changed by client,do not load from ini file
        return m_nReadFlashBRFlag;
    }    

    _TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );
    
    return GetPrivateProfileInt( _T("ReadFlashBeforeRepartition"), _T("strategy"),0, szConfigIniFile);
}

BOOL COptionHelpper::SetProperty(LONG lFlags, _TCHAR* lpszName,LPCVOID pvarValue)
{
	UNUSED_ALWAYS(lFlags);
	CString strName = lpszName;
	BOOL bOK = TRUE;
	if(strName.CompareNoCase(_T("PRODUCT"))== 0)
	{
		m_strProduct = (_TCHAR*)pvarValue;
		if(m_strProduct.Find(_T("PAC_")) == 0)
		{
			m_strProduct = m_strProduct.Right(m_strProduct.GetLength()-4);
		}

		// judge if the customer product time out is set
		_TCHAR szConfigIniFile[MAX_PATH];    
		GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
		_tcscat( szConfigIniFile,  _T("\\BMTimeout.ini") );
    
		_TCHAR szSection[MAX_PATH*10];
		DWORD dwSize = GetPrivateProfileSection( m_strProduct, szSection, MAX_PATH*10, szConfigIniFile );
		
		m_bChangeTimeOutSetting = (dwSize==0?FALSE:TRUE);	

		memset(szConfigIniFile,0,sizeof(szConfigIniFile));
		GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
		_tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );
		dwSize = GetPrivateProfileSection( m_strProduct, szSection, MAX_PATH*10, szConfigIniFile );
		m_bChangePacketLenSetting = (dwSize==0?FALSE:TRUE);

		
	}
	else if(strName.CompareNoCase(_T("EnablePortSecondEnum"))== 0)
	{
		m_bEnableSecondEnum = *((BOOL*)pvarValue);
	}
    else if (strName.CompareNoCase(_T("PowerOff"))== 0)
    {
       m_bPowerOff = *((BOOL*)pvarValue);
    }
	else if (strName.CompareNoCase(_T("UartDownload"))== 0)
	{
		m_bUartDownload = *((BOOL*)pvarValue);
	}
	else if(strName.CompareNoCase(_T("RomCodeBaudRate"))== 0)
	{
		m_dwRomCodeBaudrate = *((DWORD*)pvarValue);
	}
	else if(strName.CompareNoCase(_T("LogPath")) == 0)
	{
		m_strLogPath = (_TCHAR*)pvarValue;	
	}
    else
    {
        bOK = FALSE;
    }
	
	return bOK;
}
BOOL COptionHelpper::GetProperty(LONG lFlags, _TCHAR* lpszName,LPVOID * pvarValue)
{
	UNUSED_ALWAYS(lFlags);
	UNUSED_ALWAYS(lFlags);

	CString strName = lpszName;
	BOOL bOK = FALSE;
	if(strName.CompareNoCase(_T("CheckNVTimes"))== 0)
	{	
		_TCHAR szConfigIniFile[MAX_PATH];    
		GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
		_tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );
    
		//_TCHAR szSection[MAX_PATH];
		DWORD dwTimes = GetPrivateProfileInt( _T("DownloadNV"), _T("CheckNVTimes"), 0, szConfigIniFile );
		
		*pvarValue = (void*)dwTimes;
			
		bOK = TRUE;
	}

	return bOK;
}

UINT COptionHelpper::GetDefaultBaudrate()
{
	_TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMTimeout.ini") );
    
    return GetPrivateProfileInt( _T("Baudrate"), _T("default"),DEFAULT_BAUDRATE, szConfigIniFile);
}

BOOL COptionHelpper::IsEnablePortSecondEnum()
{
	return m_bEnableSecondEnum;
}

DWORD COptionHelpper::GetRomCodeBaudRate()
{
	return m_dwRomCodeBaudrate;
}

BOOL COptionHelpper::IsEnablePowerOff()
{
    return m_bPowerOff;
}

BOOL COptionHelpper::IsUartDownload()
{
	return m_bUartDownload;
}

BOOL COptionHelpper::IsSupportZroPkg()
{
    _TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );

    return GetPrivateProfileInt( _T("Misc") , _T("SupportZroPkg"), 1, szConfigIniFile);  
}

BOOL COptionHelpper::IsNeedDoChkSum()
{
    _TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );

    return GetPrivateProfileInt( _T("Misc") , _T("DoCheckSum"), 0, szConfigIniFile);  
}

BOOL COptionHelpper::SupportUnTransCode()
{
	_TCHAR szConfigIniFile[MAX_PATH];    
	GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
	_tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );

	return GetPrivateProfileInt( _T("Misc") , _T("SupportUnTransCode"), TRUE, szConfigIniFile);  
}

DWORD COptionHelpper::CloseDevBeforeRemove()
{
	_TCHAR szConfigIniFile[MAX_PATH];    
	GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
	_tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );
	return GetPrivateProfileInt( _T("Misc") , _T("CloseDevBeforeRemove"), 1, szConfigIniFile); 
}

BOOL COptionHelpper::IsNeedCheckOldMemory()
{
    _TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );

    return GetPrivateProfileInt( _T("Misc") , _T("NeedCheckOldMemory"), FALSE, szConfigIniFile);  
}

int COptionHelpper::GetCheckOldMemoryKind()
{
    _TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );

    int nCheckRegModify = GetPrivateProfileInt( _T("Misc") , _T("CheckRegModify"), 1, szConfigIniFile); 
    int nCheckEmmcPartition = GetPrivateProfileInt( _T("Misc") , _T("CheckEmmcPartition"), 1, szConfigIniFile); 
    int nCheckRpmbWritten = GetPrivateProfileInt( _T("Misc") , _T("CheckRpmbWritten"), 1, szConfigIniFile); 

    int nRet = nCheckRegModify + nCheckEmmcPartition*2 + nCheckRpmbWritten*4;

    return nRet;
}

BOOL COptionHelpper::IsGenCrcCfgFile()
{
    _TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );

    return GetPrivateProfileInt( _T("Misc") , _T("GenCrcCfgFile"), FALSE, szConfigIniFile);  
}

BOOL COptionHelpper::IsEnableCrcCfgCheck()
{
    _TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );

    return GetPrivateProfileInt( _T("Misc") , _T("EnableCrcCfgCheck"), FALSE, szConfigIniFile);  
}

CString COptionHelpper::GetCrcFile()
{
    _TCHAR szConfigIniFile[MAX_PATH];    
    GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
    _tcscat( szConfigIniFile,  _T("\\BMFileType.ini") );
    _TCHAR szBuf[MAX_PATH] = {0};
    GetPrivateProfileString(_T("Misc"),_T("CrcFile"),_T(""),szBuf,MAX_PATH,szConfigIniFile);

    return szBuf;  
}

DWORD COptionHelpper::GetChannelLogLevel()
{
	_TCHAR szConfigIniFile[MAX_PATH];    
	GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
	_tcscat( szConfigIniFile,  _T("\\Channel.ini") );

	return GetPrivateProfileInt( _T("Log") , _T("Level"), 0, szConfigIniFile);  
}

DWORD COptionHelpper::GetChannelLogType()
{
	_TCHAR szConfigIniFile[MAX_PATH];    
	GetModuleFilePath( g_theApp.m_hInstance, szConfigIniFile );
	_tcscat( szConfigIniFile,  _T("\\Channel.ini") );

	return GetPrivateProfileInt( _T("Log") , _T("Type"), 0x4000, szConfigIniFile); 
}
