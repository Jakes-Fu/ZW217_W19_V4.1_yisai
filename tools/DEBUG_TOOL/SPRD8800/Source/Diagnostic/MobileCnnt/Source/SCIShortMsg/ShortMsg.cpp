// File Name:   ShortMsg.cpp 
// Author:      anli.wei
// Date:        2005-04-28
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.
// Description: 手机的短消息接口的实现
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCIShortMsg.h"
#include "ShortMsg.h"
#include "IPort.h"
#include "TCHAR.h"
#include <memory>
#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////

#define MAX_RETRY_NUM           10

#ifdef _EGO
#define SEND_DELAY_TIME         5000  // 发送短信的最大延时(ms)
#define READ_DELAY_TIME         5000  // 读取一条短信的延时(ms)
#define READ_ALL_DELAY_TIME     50000  // 读取所有短信的延时(ms)
#define DEL_ALL_DELAY_TIME      500   // 删除所有短信的最大延时(ms)
#define DEL_DELAY_TIME          500   // 删除一条短信的延时(ms)
#else
#define SEND_DELAY_TIME         15000   // 发送短信的最大延时(ms)
#define READ_DELAY_TIME         15000   // 读取一条短信的延时(ms)
#define READ_ALL_DELAY_TIME     15000  // 读取所有短信的延时(ms)
#define DEL_ALL_DELAY_TIME      15000   // 删除所有短信的最大延时(ms)
#define DEL_DELAY_TIME          15000   // 删除一条短信的延时(ms)
#endif

#define GET_INCM_DELAY_TIME     200   // 获取自动通知短信的延时(ms)

const char CShortMsg::AT_END_FLAG[]             = "\r\n";
const char CShortMsg::AT_AT[]                   = "AT\r\n";
const char CShortMsg::AT_DISABLE_ECHO[]         = "ATE0\r\n";
const char CShortMsg::AT_RES_OK[]               = "OK\r\n";
const char CShortMsg::AT_RES_ERROR[]            = "+CME ERROR";
const char CShortMsg::AT_SMS_INPUT_PROMPT[]     = ">";
const char CShortMsg::AT_SMS_INPUT_END_FLAG[]   = { 0x1A };  // ctrl+z

// 设置短信的模式, 0: PDU, 1: text
const char CShortMsg::AT_SMS_MODE[]             = "AT+CMGF=%d\r\n";

// 网络有关信息
//
// 禁止自动汇报网络状态
const char CShortMsg::AT_DIS_AUTOREP_NETSTS[]   = "AT+CREG=0\r\n";
// 查询网络状态，返回 +CREG:<mode>,<status>，如:+CREG:0,1
const char CShortMsg::AT_QUERY_NETSTS[]         = "AT+CREG?\r\n";
// 网络状态前缀
const char CShortMsg::AT_RES_QUERY_NETSTS_PRE[] = "+CREG:";

// 短信
// 中心号码
const char CShortMsg::AT_GET_SMS_CENTER[]       = "AT+CSCA?\r\n";
const char CShortMsg::AT_SET_SMS_CENTER[]       = "AT+CSCA=\"%s\"\r\n";
const char CShortMsg::AT_SAVE_SMSCENTER[]       = "AT+CSAS\r\n";
const char CShortMsg::AT_RESTORE_SMSCENTER[]    = "AT+CRES\r\n";
const char CShortMsg::AT_RES_SMS_CENTER[]       = "+CSCA:";

// 短信息自动提示功能
// 手机收到短信后自动通知DTE，返回的内容格式为 +CMTI:<mem>,<index>\r\n
const char CShortMsg::AT_REG_INMSG_INDEX[]      = "AT+CNMI=3,3,0,1\r\n";
const char CShortMsg::AT_RES_INMSG_PREFIX[]     = "+CMTI:";

// 存贮位置及数量
const char CShortMsg::AT_SMS_STORE_SIM[]        = "SM";
const char CShortMsg::AT_SMS_STORE_MS[]         = "BM";
const char CShortMsg::AT_SEL_SMS_SIM[]          = "AT+CPMS=\"SM\",\"SM\"\r\n";
const char CShortMsg::AT_SEL_SMS_MS[]           = "AT+CPMS=\"BM\",\"BM\"\r\n";
const char CShortMsg::AT_SEL_SMS_MS2[]          = "AT+CPMS=\"ME\",\"ME\"\r\n";
const char CShortMsg::AT_GET_SMS_NUM[]          = "AT+CPMS?\r\n";
const char CShortMsg::AT_RES_SMS_PRE[]          = "+CPMS:";
const char CShortMsg::AT_SEL_STORE[]			= "AT+CPMS=\"%s\",\"%s\"\r\n";
const char CShortMsg::AT_GET_STORE[]            = "AT+CPMS=?\r\n";

// 所有短信息内容
// 返回的格式为
// +CMGL: <index>,<编码>,<"状态">,<"号码">,<"日期"><CR><LF><内容><CR><LF>
// +CMGL: <index>,<编码>,<"状态">,<号码>,<日期><CR><LF><内容><CR><LF>
//  ...
// <CR><LF>OK<CR><LF>
const char CShortMsg::AT_GET_ALLSMS_TXT[]       = "AT+CMGL=\"ALL\"\r\n";
const char CShortMsg::AT_GET_ALLSMS_PDU[]       = "AT+CMGL=4\r\n";
const char CShortMsg::AT_RES_GET_ALLSMS_PRE[]   = "+CMGL:";
// 指定的短信息
// 返回的格式为
// +CMGR: <状态>,<编码>,<号码>,,<日期><CR><LF><内容><CR><LF>
// <CR><LF>OK<CR><LF>
const char CShortMsg::AT_GET_IDXSMS[]           = "AT+CMGR=%d\r\n";
const char CShortMsg::AT_RES_GET_IDXSMS_PRE[]   = "+CMGR:";

// 发送
// 发送模式：参数%d表示有效期限，第2个%d表示编码方式0,ascii;1,8bit;2unicode
const char CShortMsg::AT_SMS_SENDMODE[]         = "AT+CSMP=19,%d,0,%d\r\n";
//发送模式带状态报告
const char CShortMsg::AT_SMS_SENDMODE_SRR[]     = "AT+CSMP=51,%d,0,%d\r\n";
const int  CShortMsg::AT_SMS_UCS2               = 2;
const int  CShortMsg::AT_SMS_ASCII              = 0;
// 文本方式：参数%s为目的号码，参数%d表示是否释放链路
const char CShortMsg::AT_SMS_SEND_TXT[]         = "AT+CMGS=\"%s\",%d\r\n";
// PDU方式：%s为TPDU的长度,参数%d表示是否释放链路
const char CShortMsg::AT_SMS_SEND_PDU[]         = "AT+CMGS=%d,%d\r\n";
// 参数：%d为存贮在手机或SIM卡的索引号，%s为目的号码
const char CShortMsg::AT_SMS_SEND_IDX[]         = "AT+CMSS=%d,\"%s\"\r\n";

// 删除短信息
const char CShortMsg::AT_DEL_ALLSMS[]           = "AT+CMGD=1,4\r\n";
const char CShortMsg::AT_DEL_IDXSMS[]           = "AT+CMGD=%d,0\r\n";

// set active sim
const char CShortMsg::AT_SET_ACTIVE_SIM[]		= "AT+SSYS=%d\r\n";
const char CShortMsg::AT_QUERY_ACTIVE_SIM[]		= "AT+SSYS?\r\n";
const char CShortMsg::AT_RES_ACTIVE_SIM[]		= "+SSYS:";

//teana hu 2009.05.13
const char CShortMsg::AT_LONG_SMS_PRE_SET[]	    = "AT+CMMS=%s\r\n";


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShortMsg::CShortMsg()
{
    m_hProgressWnd = NULL;
    m_pPort        = NULL;
    m_dwErrCode    = 0;

    m_eStorePlace  = SMS_SP_UNKOWN;
	m_dwRcvSize    = 0;
	m_nUsedCount   = 0;
	m_pSMSInfo     = NULL;

    m_eSMSMode     = SMS_MODE_PDU;
	m_bSRR         = FALSE;
	
	m_pUIThread    = NULL;

    ::InitializeCriticalSection( &m_CriticalSection );

	memset(m_szSendBuf,0,sizeof(m_szSendBuf));	
	memset(m_szRcvBuf,0,sizeof(m_szRcvBuf));	
	memset(m_szTempW,0,sizeof(m_szTempW));	
	memset(m_szTempA,0,sizeof(m_szTempA));	
	memset(m_arrRefNum,0,sizeof(m_arrRefNum));	
	
	_TCHAR szPath[_MAX_PATH] = {0};
	::GetModuleFileName( NULL, szPath, _MAX_PATH );
	 LPTSTR pDir = _tcsrchr(szPath, _T('\\'));
    _tcscpy(pDir + 1, _T("Timeout.ini"));

	m_nRetryTimes		= GetPrivateProfileInt( _T("SMS"), _T("RetryTimes"), MAX_RETRY_NUM, szPath );
	m_nReadOneTimeout	= GetPrivateProfileInt( _T("SMS"), _T("ReadOneTimeout"), READ_DELAY_TIME, szPath );
	m_nReadAllTimeout	= GetPrivateProfileInt( _T("SMS"), _T("ReadAllTimeout"), READ_ALL_DELAY_TIME, szPath );
    m_nDelOneTimeout	= GetPrivateProfileInt( _T("SMS"), _T("DeleteOneTimeout"), DEL_DELAY_TIME, szPath );
    m_nDelAllTimeout	= GetPrivateProfileInt( _T("SMS"), _T("DeleteAllTimeout"), DEL_ALL_DELAY_TIME, szPath );
    m_nSendTimeout		= GetPrivateProfileInt( _T("SMS"), _T("SendTimeout"), SEND_DELAY_TIME, szPath );
    m_nGetINCMTimeout	= GetPrivateProfileInt( _T("SMS"), _T("GetINCMTimeout"), GET_INCM_DELAY_TIME, szPath );
}

CShortMsg::~CShortMsg()
{
    ::DeleteCriticalSection( &m_CriticalSection );
	delete []m_pSMSInfo;
}

// 设置端口连接通道，如果需要不同的连接，可以更改此接口，并更改本库中
// 用到的发送及接收的接口
void CShortMsg::SetPort( IPort* pPort )
{
	_ASSERTE( pPort != NULL );

	m_pPort = pPort;
}

// 检查是否连接手机
BOOL CShortMsg::CheckConnection(void)
{
    CCritialSection crs( m_CriticalSection );

    if( !ExecuteCmd(AT_AT, m_nReadOneTimeout) )    
	{        
        m_Log.LogEx(_T("!! CheckConnection Fail !!\r\n"));
        //m_pszErrMsg = GetPrompt(IDS_MLP_CONNECTION_FAIL);   
		m_dwErrCode = SMS_E_CONNECTION_FAIL;
        return FALSE;
    }
    return TRUE;
}

// 禁止命令的回应
BOOL CShortMsg::DisableEcho(void)
{
    CCritialSection crs( m_CriticalSection );

    if( !ExecuteCmd(AT_DISABLE_ECHO, m_nGetINCMTimeout) )
    {        
        m_Log.LogEx(_T("!! DisableEcho Fail !!\r\n"));
        m_dwErrCode = SMS_E_DISABLEECHO_FAIL;        
        return FALSE;
    }
    return TRUE;
}

// 查询网络状态
NET_STATUS_E CShortMsg::QueryNetStatus(void)
{
    NET_STATUS_E eNetStat = NET_UNKNOWN;

    CCritialSection crs( m_CriticalSection );

    if( ExecuteCmd(AT_DIS_AUTOREP_NETSTS, m_nReadOneTimeout) && 
        ExecuteCmd(AT_QUERY_NETSTS, m_nReadOneTimeout) )
    {
        // +CREG:<mode>,<status>
        LPSTR pStat = strstr(m_szRcvBuf, AT_RES_QUERY_NETSTS_PRE);
        _ASSERTE( pStat != NULL );
		if(pStat == NULL)
		{			
			m_Log.LogEx(_T("!! QueryNetStatus Fail !!\r\n"));
			m_dwErrCode = SMS_E_QUERYNETSTATUS_FAIL;
			return eNetStat; 
		}


        pStat += strlen(AT_RES_QUERY_NETSTS_PRE);
        pStat = strchr(pStat, ',');
		if(pStat == NULL)
		{		
			m_Log.LogEx(_T("!! QueryNetStatus Fail !!\r\n"));
			m_dwErrCode = SMS_E_QUERYNETSTATUS_FAIL;
			return eNetStat; 
		}

        pStat++;

        LPSTR pEnd = strstr(pStat, AT_RES_OK);
		if(pStat == NULL)
		{			
			m_Log.LogEx(_T("!! QueryNetStatus Fail !!\r\n"));
			m_dwErrCode = SMS_E_QUERYNETSTATUS_FAIL;
			return eNetStat; 
		}

        *pEnd = 0;
        eNetStat = (NET_STATUS_E)atoi(pStat);        
    }
    else
    {        
        m_Log.LogEx(_T("!! QueryNetStatus Fail !!\r\n"));
        m_dwErrCode = SMS_E_QUERYNETSTATUS_FAIL;
    }

    return eNetStat;
}

// 查询短信中心号码
LPCTSTR CShortMsg::QueryServiceCenter(void)
{
    CCritialSection crs( m_CriticalSection );

	if( ExecuteCmd(AT_RESTORE_SMSCENTER, m_nReadOneTimeout) )
	{
		if( ExecuteCmd(AT_GET_SMS_CENTER, m_nReadOneTimeout) )
		{
			// 返回格式格式为：+CSCA: "+8613800230500",xxx
			LPSTR pFind = strstr(m_szRcvBuf, AT_RES_SMS_CENTER);
			if( pFind != NULL )
			{
				pFind += strlen(AT_RES_SMS_CENTER);
				pFind = strchr(pFind, '"');
				//teana hu 2009.03.02
				_ASSERTE( pFind != NULL );
				if(pFind == NULL)
				{
					return NULL;
				}
				
				LPSTR pStart = pFind + 1;
				
				pFind = strchr(pStart, '"');
				//teana hu 2009.03.01
				//_ASSERTE( pFind != NULL );
				if(pFind == NULL)
				{
					return NULL;
				}
				*pFind = '\0';
#ifdef _UNICODE
				int nLen = MultiByteToWideChar(CP_ACP,0,pStart,-1,NULL,0);	
				MultiByteToWideChar(CP_ACP,0,pStart,-1,m_szTempW,nLen);
#else
				memcpy(m_szTempW, pStart, strlen(pStart));

#endif				
				
				return m_szTempW;
			}
		}
	}	
    m_Log.LogEx(_T("!! QueryServiceCenter Fail !!\r\n"));
    m_dwErrCode = SMS_E_QUERY_CENTER;
    
	return NULL;
}

// 设置短信中心号码
// 短消息中心号码，{0-9, *, #, +, a, b, c}，最大长度为40(请参考AT 用户手册)
BOOL CShortMsg::SetServiceCenter( LPCTSTR pszCenter )
{
	//teana hu 2009.03.02
    _ASSERTE( pszCenter != NULL && _tcslen(pszCenter) <= 40 );
	if(pszCenter == NULL || _tcslen(pszCenter) > 40)
	{
		return FALSE;
	}

    CCritialSection crs( m_CriticalSection );

    memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	int nALen = WideCharToMultiByte(CP_ACP,0,pszCenter,-1,NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,pszCenter,-1,m_szTempA,nALen,NULL,NULL);
#else
	memcpy(m_szTempA, pszCenter, strlen(pszCenter));
#endif
	
	
	sprintf(m_szSendBuf, AT_SET_SMS_CENTER, m_szTempA);
	
	if( !ExecuteCmd(m_szSendBuf, m_nReadOneTimeout) )
	{		
        m_Log.LogEx(_T("!! SetServiceCenter Fail !!\r\n"));
        m_dwErrCode = SMS_E_SET_CENTER;        
		return FALSE;
	}

	// 短信中心号码设置后需要保存，否则再次开机后失效
	sprintf(m_szSendBuf, AT_SAVE_SMSCENTER);
	if( ExecuteCmd(m_szSendBuf, m_nReadOneTimeout) )
	{
		return TRUE;
	}
	else
	{		
        m_Log.LogEx(_T("!! SaveServiceCenter Fail !!\r\n"));
	}	
    m_Log.LogEx(_T("!! SetServiceCenter Fail !!\r\n"));
    m_dwErrCode = SMS_E_SET_CENTER;
	return FALSE;
}

// 设置短信的模式(Text或PDU)
BOOL CShortMsg::SetSMSMode( SMS_MODE_E eSMSMode /* = SMS_MODE_TEXT */)
{
	//teana hu 
    //_ASSERTE( eSMSMode == 0 || eSMSMode == 1 );
	if(eSMSMode != 0 && eSMSMode != 1 )
	{
		return FALSE;
	}

    CCritialSection crs( m_CriticalSection );

    sprintf( m_szSendBuf, AT_SMS_MODE, eSMSMode );
    if( !ExecuteCmd(m_szSendBuf) )
    {        
        m_Log.LogEx(_T("!! SetSMSMode %d Fail !!\r\n"), eSMSMode);
        m_dwErrCode = SMS_E_SETMODE_FAIL;
        return FALSE;
    }

    m_eSMSMode = eSMSMode;

    return TRUE;
}

// 注册MS收到短信后自动通知DTE功能
BOOL CShortMsg::RegisterIncomingSMS(void)
{
    CCritialSection crs( m_CriticalSection );

    if( !ExecuteCmd(AT_REG_INMSG_INDEX) )
    {        
        m_Log.LogEx(_T("!! RegisterIncomingSMS Fail !!\r\n"));
        m_dwErrCode = SMS_E_REG_INCSMS;
        return FALSE;
    }

    return TRUE;
}

// 选择短信的存储位置(手机存储器或SIM卡)
BOOL CShortMsg::SelectStorePlace( SMS_STORE_PLACE_E eStorePlace )
{
	//teana hu 2009.03.02
    _ASSERTE( eStorePlace == SMS_SP_MS   || eStorePlace == SMS_SP_SIM ||
		      eStorePlace == SMS_SP_SIM2 || eStorePlace == SMS_SP_SIM3||
			  eStorePlace == SMS_SP_SIM4);
	if( eStorePlace != SMS_SP_MS  && 
		eStorePlace != SMS_SP_SIM && 
		eStorePlace != SMS_SP_SIM2&& 
		eStorePlace != SMS_SP_SIM3&&
		eStorePlace != SMS_SP_SIM4)
	{
		return FALSE;
	}

    CCritialSection crs( m_CriticalSection );

    if( m_eStorePlace == eStorePlace )
    {
        return TRUE;
    }

	BOOL bBM = IsBM();

    switch( eStorePlace )
	{
	case SMS_SP_MS:	
		strcpy(m_szSendBuf, bBM ? AT_SEL_SMS_MS : AT_SEL_SMS_MS2);
		break;
	case SMS_SP_SIM:		
	case SMS_SP_SIM2:
	case SMS_SP_SIM3:
	case SMS_SP_SIM4:
		sprintf(m_szSendBuf, AT_SEL_SMS_SIM);
		break;
	default:
		_ASSERTE(0);
        break;
	}

	if( !ExecuteCmd(m_szSendBuf, m_nReadOneTimeout) )
	{		
        m_Log.LogEx(_T("!! SelectStorePlace Fail !!\r\n"));
        m_dwErrCode = SMS_E_SELECT_SP;
        return FALSE;
	}

    _ASSERTE( strstr(m_szRcvBuf, AT_RES_SMS_PRE) != NULL );

    m_eStorePlace = eStorePlace;

    return TRUE;
}

BOOL CShortMsg::SelectStorePlaceAll( SMS_STORE_PLACE_E eStorePlaceRD, SMS_STORE_PLACE_E eStorePlaceWS )
{

    CCritialSection crs( m_CriticalSection );
	char szRD[MAX_BUF_LEN] = {0};
	char szWS[MAX_BUF_LEN] = {0};

    switch( eStorePlaceRD )
	{
	case SMS_SP_MS:
		sprintf(szRD, "BM");
		break;
	case SMS_SP_SIM:		
	case SMS_SP_SIM2:
	case SMS_SP_SIM3:
	case SMS_SP_SIM4:
		sprintf(szRD, "SM");
		break;
	default:
		_ASSERTE(0);
        break;
	}

	switch( eStorePlaceWS )
	{
	case SMS_SP_MS:
		sprintf(szWS, "BM");
		break;
	case SMS_SP_SIM:		
	case SMS_SP_SIM2:
	case SMS_SP_SIM3:
	case SMS_SP_SIM4:
		sprintf(szWS, "SM");
		break;
	default:
		_ASSERTE(0);
        break;
	}

	sprintf(m_szSendBuf, AT_SEL_STORE, szRD, szWS);

	if( !ExecuteCmd(m_szSendBuf, m_nReadOneTimeout) )
	{		
        m_Log.LogEx(_T("!! SelectStorePlace Fail !!\r\n"));
        m_dwErrCode = SMS_E_SELECT_SP;
        return FALSE;
	}

    _ASSERTE( strstr(m_szRcvBuf, AT_RES_SMS_PRE) != NULL );

    return TRUE;
}

BOOL CShortMsg::SetActiveSim(SMS_STORE_PLACE_E eStorePlace)
{
	//teana hu 2009.03.02
    _ASSERTE( eStorePlace == SMS_SP_SIM  || eStorePlace == SMS_SP_SIM2 || 
		      eStorePlace == SMS_SP_SIM3 || eStorePlace == SMS_SP_SIM4 );
	if( eStorePlace != SMS_SP_SIM  && 
		eStorePlace != SMS_SP_SIM2 && 
		eStorePlace != SMS_SP_SIM3 &&
		eStorePlace != SMS_SP_SIM4)
	{
		return FALSE;
	}
	
    CCritialSection crs( m_CriticalSection );
	
    switch( eStorePlace )
	{
	case SMS_SP_SIM:
		sprintf(m_szSendBuf, AT_SET_ACTIVE_SIM,0);
		break;
	case SMS_SP_SIM2:
		sprintf(m_szSendBuf, AT_SET_ACTIVE_SIM,1);
		break;
	case SMS_SP_SIM3:
		sprintf(m_szSendBuf, AT_SET_ACTIVE_SIM,2);
		break;
	case SMS_SP_SIM4:
		sprintf(m_szSendBuf, AT_SET_ACTIVE_SIM,3);
		break;
	default:
		_ASSERTE(0);
        break;
	}
	
	if( !ExecuteCmd(m_szSendBuf, m_nReadOneTimeout) )
	{		
        m_Log.LogEx(_T("!!Set Active SIM Fail !!\r\n"));
        m_dwErrCode = SMS_E_SET_ACTIVE_SIM;
        return FALSE;
	}  
	
    return TRUE;

}

// 获取短信的容量(总数，已使用个数)
BOOL CShortMsg::QueryCapacity( int &nTotal, int &nUsed )
{
    CCritialSection crs( m_CriticalSection );

    nTotal = nUsed = 0;

	if( ExecuteCmd(AT_GET_SMS_NUM, m_nReadOneTimeout) )
	{
		// 格式为:+CPMS:"selPlace(SM or BM)",selectedUsed,selectedTotal,"SIM",simUsed,simTotal
		LPSTR pFind = strstr(m_szRcvBuf, AT_RES_SMS_PRE);
		if( pFind != NULL )
		{
			pFind += strlen(AT_RES_SMS_PRE);
			LPSTR pStart = strchr(pFind, ',');
			//teana hu 2009.03.02
			_ASSERTE(pStart != NULL );
			if(pStart == NULL)
			{
				return FALSE;
			}
			pStart++;

			pFind = strchr(pStart, ',');
			//teana hu 2009.03.02
			_ASSERTE( pFind != NULL );
			if(pFind == NULL)
			{
				return FALSE;
			}
			*pFind = '\0';
			//teana hu 2009.03.02
			_ASSERTE( strlen(pStart) > 0 );
			if(strlen(pStart) == 0 )
			{
				return FALSE;
			}
			nUsed = atoi(pStart);
			m_nUsedCount = nUsed;

			pStart = pFind + 1;
			pFind = strchr(pStart, ',');
			//teana hu 2009.03.02
			_ASSERTE( pFind != NULL );
			if(pFind == NULL)
			{
				return FALSE;
			}
			*pFind = '\0';
			//teana hu 2009.03.02
			_ASSERTE( strlen(pStart) > 0 );
			if(strlen(pStart) == 0)
			{
				return FALSE;
			}
			nTotal  = atoi(pStart);
	
			return TRUE;
		}
	}	
    m_Log.LogEx(_T("!! QueryCapacity Fail !!\r\n"));

    m_dwErrCode = SMS_E_QUERYCAPACITY;

	return FALSE;
}

LPCSTR CShortMsg::QueryStoreCapacity( )
{
	CCritialSection crs( m_CriticalSection );

	if( ExecuteCmd(AT_GET_SMS_NUM, m_nReadOneTimeout) )
	{
		// 格式为:+CPMS:"selPlace(SM or BM)",selectedUsed,selectedTotal,"SIM",simUsed,simTotal
		LPSTR pFind = strstr(m_szRcvBuf, AT_RES_SMS_PRE);
		if( pFind != NULL )
		{
			return m_szRcvBuf;
		}
	}	

	return NULL;
}

BOOL CShortMsg::ParseIncomingIndex( LPCSTR pszBuf, int nLen )
{
    // pBuf的格式为+CMTI:"mem",<index>\r\n
    int   nPrefixLen = strlen(AT_RES_INMSG_PREFIX);
	LPSTR pFind      = NULL;
	while( (pFind = FindStr(pszBuf, nLen, AT_RES_INMSG_PREFIX, nPrefixLen) ) != NULL )
	{
		pFind += nPrefixLen;
		pFind = strchr( pFind, ',' );
		if( NULL == pFind )
            return FALSE;

		char * pValue = pFind + 1;
		pFind = strstr( pValue, AT_END_FLAG );
		if( NULL == pFind )
            return FALSE;

        char cEnd = *pFind;
		*pFind = '\0';
        int nIdx = atoi(pValue);
		*pFind = cEnd;

        nLen -= (pFind - pszBuf);
        pszBuf = pFind + strlen(AT_END_FLAG); // \r\n

        m_deqIncomingIdx.push_back(nIdx);
	}

    return TRUE;
}

void CShortMsg::ClearAllIncomingIndex(void)
{
    m_deqIncomingIdx.clear();
}

// 获取刚刚收到或最近收到的短信息的ID(此函数可供循环调用)
BOOL CShortMsg::GetIncomingIndex( int &nIndex )
{
    CCritialSection crs( m_CriticalSection );

    nIndex = 0;
	
	DWORD dwReadTmp = 0;	
	for(int j=0;j< m_nRetryTimes; j++)
	{
		m_pPort->Read( (LPVOID)m_szRcvBuf, MAX_BUF_LEN, &dwReadTmp, 1);
	}

    memset( m_szRcvBuf, 0, sizeof(m_szRcvBuf) );
    DWORD dwRead   = 0;
    DWORD dwBufLen = MAX_BUF_LEN;
    LPSTR pRcvBuf  = m_szRcvBuf;
    int   nRcvSize = 0;
    for( int i = 0; dwBufLen > 0 && i < m_nRetryTimes; ++i )
    {
        if( m_pPort->Read( (LPVOID)pRcvBuf, dwBufLen, &dwRead, m_nGetINCMTimeout) && dwRead > 0)
        {
            --i;
            nRcvSize += dwRead;  
			
			if( ParseIncomingIndex(m_szRcvBuf, nRcvSize) )
			{                
				break;
			}
        }

        dwBufLen -= dwRead;
        pRcvBuf  += dwRead;
    }

    if( !m_deqIncomingIdx.empty() )
    {
        std::deque<int>::iterator it = m_deqIncomingIdx.begin();

        nIndex = *it;
        
        m_deqIncomingIdx.pop_front();

        return TRUE;
    }

    return FALSE;
}

// 读取所有短信息
BOOL CShortMsg::ReadAllSMS(int &nCount)
{
	USES_CONVERSION;
    CCritialSection crs( m_CriticalSection );

    int nTotal = 0;
    if( !QueryCapacity(nTotal, nCount) )
    {
        nCount = 0;
        m_dwErrCode = SMS_E_READALLSMS;
        return FALSE;
    }

    if( nCount == 0 )
    {
        return TRUE;
    }

    int    nSMSCount = m_nUsedCount + 20; // 读的过程中间可能会继续收到短信
    nSMSCount = nSMSCount > nTotal ? nTotal : nSMSCount;

	// 注意：现在接受到的sms为hex string格式加额外信息；对于PDU模式来说，额外信息会比较长
    // 我们这里设置最大值为80(请PDU短信格式的说明)
	const int nRcvBufSize = nSMSCount * (sizeof(SMS_INFO) + SMS_CONTENT_MAX_LEN + 80);
    std::auto_ptr<char>RcvBuf( new char[nRcvBufSize + 1] );
	LPSTR pRcvBuf = RcvBuf.get();
	if( NULL == pRcvBuf )
	{		
        m_Log.LogEx(_T("!! 1 ReadAllSMS Fail(memory no enough), used is: %d !!\r\n"), m_nUsedCount );

		return FALSE;
	}
	DWORD dwReadTmp = 0;
	for(int j=0;j< m_nRetryTimes; j++)
	{
		m_pPort->Read( (LPVOID)pRcvBuf, nRcvBufSize, &dwReadTmp, 1);
	}

    memset(pRcvBuf, 0, nRcvBufSize + 1);

	delete []m_pSMSInfo;
	m_pSMSInfo = (PSMS_INFO)new SMS_INFO[nSMSCount];
	if( NULL == m_pSMSInfo )
	{		
        m_Log.LogEx(_T("!! 2 ReadAllSMS Fail(memory no enough), used is: %d !!\r\n"), m_nUsedCount );
		return FALSE;
	}
    memset( m_pSMSInfo, 0, sizeof(SMS_INFO) * nSMSCount );

	m_pPort->PurgeAll();

    if( SMS_MODE_TEXT == m_eSMSMode )
    {
        strcpy(m_szSendBuf, AT_GET_ALLSMS_TXT);
    }
    else if( SMS_MODE_PDU == m_eSMSMode )
    {
        strcpy(m_szSendBuf, AT_GET_ALLSMS_PDU);
    }
    else
    {
        _ASSERTE( 0 );
    }

    if( m_hProgressWnd != NULL )
    {
        PostMessage( m_hProgressWnd, WM_PROGRESS, PROG_BEGIN, nRcvBufSize );
    }	
	else if(m_pUIThread != NULL)//UI线程显示进度条
	{
		m_pUIThread->PostThreadMessage(WM_PROGRESS,PROG_BEGIN,nRcvBufSize);
	}

    m_pPort->Write((LPVOID)m_szSendBuf, strlen(m_szSendBuf), NULL);

    // 读取所有短信时：对于文本方式为 AT+CMGL="ALL"，PDU方式为 AT+CMGL=4
    // 返回格式分别如下:
    // 文本方式：
    // +CMGL: <index>,<编码>,<"状态">,<"号码">,<"日期"><CR><LF><内容><CR><LF>
    // +CMGL: <index>,<编码>,<"状态">,<号码>,<日期><CR><LF><内容><CR><LF>
    //  ...
    // <CR><LF>OK<CR><LF>
	// +CMGL: <index>, <stat>, <st> (for SMS-ATATUS-REPORT)
    // PDU格式：
    // +CMGL: <index>,<状态>, ,长度<CR><LF><CR><LF>内容<CR><LF><CR><LF>
    // +CMGL: <index>,<状态>, ,长度<CR><LF><CR><LF>内容<CR><LF><CR><LF>
    //  ...
    // <CR><LF>OK<CR><LF>
    //

	LPSTR pInBuf    = pRcvBuf;
    DWORD  dwTotal   = 0;
	DWORD  dwRead    = 0;
	int    nRcvSize  = nRcvBufSize;
	BOOL   bRet      = FALSE;

	MSG msg;

	for( int i = 0; nRcvSize > 0 && i < m_nRetryTimes; ++i )
	{
		if( m_pPort->IsOpened() )
		{
			//m_nReadOneTimeout
			if(m_pPort->Read(pInBuf, nRcvSize, &dwRead, m_nReadAllTimeout ) )
			{
				if(dwRead > 0)
				{
					--i;
					
					dwTotal += dwRead;
					TRACE(_T("++++++++++++++read buffer:%d\n"),dwRead);
					
					if(dwTotal > 0)
					{
						//m_Log.LogEx(A2T(pInBuf));
						while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
						if( m_hProgressWnd != NULL )
						{
							PostMessage( m_hProgressWnd, WM_PROGRESS, PROG_PROCEED, dwTotal );
						}
						else if(m_pUIThread != NULL)//UI线程显示进度条
						{
							m_pUIThread->PostThreadMessage(WM_PROGRESS,PROG_PROCEED,dwTotal);
						}
						
						if( FindStr(pRcvBuf, dwTotal, AT_RES_OK, strlen(AT_RES_OK)) != NULL )
						{
							bRet      = TRUE;
							nRcvSize -= dwRead;
							break;
						}
						else if(FindStr(pRcvBuf, dwTotal, "ERROR", strlen("ERROR")) != NULL ||
							FindStr(pRcvBuf, dwTotal, "Error", strlen("Error")) != NULL )
						{
							m_Log.LogEx(_T("!! ReadAllSMS [ERROR] Failed !!\r\n"));
							bRet    = FALSE;
							break;
						}
						TRACE(_T("++++++++++++++[buf:%d]read buffer:%d\n"),nRcvBufSize,dwTotal);
						
						pInBuf   += dwRead;
						nRcvSize -= dwRead;
					}
				}
			}
			else
			{
				m_Log.LogEx(_T("!! ReadAllSMS [read com failed] Failed !!\r\n"));
				bRet = FALSE;
				break;
			}
		}
		else
		{
			m_Log.LogEx(_T("!! ReadAllSMS [port is closed] Failed !!\r\n"));
			bRet = FALSE;
			break;
		}
		
	}
	
	//m_Log.LogEx(_T("\r\n"));

	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
    if( m_hProgressWnd != NULL )
    {
        PostMessage( m_hProgressWnd, WM_PROGRESS, PROG_END, nRcvBufSize );
    }
	else if(m_pUIThread != NULL)//UI线程显示进度条
	{
		m_pUIThread->PostThreadMessage(WM_PROGRESS,PROG_END,nRcvBufSize);
	}

	if( bRet )
	{
#if defined(DEBUG) | defined(_DEBUG)
        //FILE * pf = fopen("c:\\sms.txt", "w");
        //fwrite(pRcvBuf, nRcvBufSize - nRcvSize, 1, pf);
        //fclose(pf);
#endif

		_ASSERTE( nRcvSize >= 0 );
        if( SMS_MODE_TEXT == m_eSMSMode )
        {
            if(!ParseAllSMS_TXT(pRcvBuf, nRcvBufSize - nRcvSize))
			{
			    m_Log.LogEx(_T("!! ParseAllSMS_TXT Failed !!\r\n"));
				return FALSE;
			}
        }
        else if( SMS_MODE_PDU == m_eSMSMode )
        {
            if(!ParseAllSMS_PDU(pRcvBuf, nRcvBufSize - nRcvSize))
			{
				m_Log.LogEx(_T("!! ParseAllSMS_PDU Failed !!\r\n"));
				return FALSE;
			}
        }
        
        nCount = m_nUsedCount;
	}
	else
	{		
        m_Log.LogEx(_T("!! ReadAllSMS Failed !!\r\n"));
        m_dwErrCode = SMS_E_READALLSMS;
	}

	return bRet;
}

// 获取读取后的短信息
PSMS_INFO CShortMsg::GetSMS( int nIndex )
{
    _ASSERTE( nIndex >= 0 && nIndex < m_nUsedCount );
    _ASSERTE( m_pSMSInfo != NULL );

    return &(m_pSMSInfo[nIndex]);
}

// 读取指定的短信息
BOOL CShortMsg::ReadSMS( PSMS_INFO pSMSInfo, int nIndex )
{
    _ASSERTE( pSMSInfo != NULL );
    _ASSERTE( nIndex >= 0 );

	//teana hu 2009.03.02
	if(pSMSInfo == NULL || nIndex == 0)
	{
		return FALSE;
	}

    CCritialSection crs( m_CriticalSection );

    // the sms end with \r\nOK\r\n
    sprintf( m_szSendBuf, AT_GET_IDXSMS, nIndex );
    if( !ExecuteCmd(m_szSendBuf, m_nReadOneTimeout) )
    {        
        m_Log.LogEx(_T("!! ReadSMS by index %d Fail !!\r\n"), nIndex);
        m_dwErrCode = SMS_E_READSMS;
        return FALSE;
    }

    if( m_eSMSMode == SMS_MODE_TEXT )
        ParseSMS_TXT(m_szRcvBuf, m_dwRcvSize, pSMSInfo);
    else if( m_eSMSMode == SMS_MODE_PDU )
        ParseSMS_PDU(m_szRcvBuf, m_dwRcvSize, pSMSInfo);
    else
        _ASSERTE( 0 );

    return TRUE;
}

BOOL CShortMsg::ParseSMSContent(LPSTR pBuf, PSMS_INFO pSMSInfo)
{
	_ASSERTE( pSMSInfo != NULL );
	_ASSERTE( pBuf != NULL );

	//teana hu 2009.03.02
	if(pSMSInfo == NULL || pBuf == NULL)
	{
		return FALSE;
	}

    CCritialSection crs( m_CriticalSection );

	memset(pSMSInfo, 0, sizeof(SMS_INFO));
	int nBufSize = strlen(pBuf);

	LPSTR  pPreFlag    = "+CMT:";
	int    nPreFlagLen = strlen(pPreFlag);

	LPSTR pFind    = FindStr(pBuf, nBufSize, pPreFlag, nPreFlagLen);
	_ASSERTE( NULL != pFind );
	//teana hu 2009.03.02
	if(pFind == NULL)
	{
		return FALSE;
	}
		
	pBuf = pFind + nPreFlagLen;

    
    if( m_eSMSMode == SMS_MODE_TEXT )
	{
		// 返回的格式为
		// +CMT:<od>,<sc>,<time stamp>,<length>,<CR><text><CR>
		
		int nCodec = ParseCodec(pBuf, pSMSInfo);      
		ParseNum(pBuf, pSMSInfo);
		ParseDate(pBuf, pSMSInfo);

		pBuf -= 2;

		pFind = strchr(pBuf, ',');
		_ASSERTE( NULL != pFind );
		//teana hu 2009.03.02
		if(pFind == NULL)
		{
			return FALSE;
		}
		*pFind = '\0';
//		int nBodyLen = atoi(pBuf);
		pBuf = pFind + 3;
		
		pFind = strstr(pBuf, "\r\n");
		_ASSERTE( NULL != pFind );
		//teana hu 2009.03.02
		if(pFind == NULL)
		{
			return FALSE;
		}
		
		//nBufSize -= (pBuf - m_szRcvBuf);
		ParseContent(pBuf, pFind - pBuf, pSMSInfo, nCodec);
	}
    else if( m_eSMSMode == SMS_MODE_PDU )
	{
		
		// 返回的格式为
		// +CMT: ,长度<CR><内容><CR>  		

		pFind = strchr(pBuf, ',');
		_ASSERTE( NULL != pFind );
		//teana hu 2009.03.02
		if(pFind == NULL)
		{
			return FALSE;
		}
		*pFind = '\0';
		pBuf = pFind + 1;
		pFind = strstr(pBuf, "\r\n");
		_ASSERTE( NULL != pFind );
		//teana hu 2009.03.02
		if(pFind == NULL)
		{
			return FALSE;
		}
		int nBodyLen = atoi(pBuf);
		pBuf = pFind + 2;

		pFind = strstr(pBuf, "\r\n");
		_ASSERTE( NULL != pFind );
		//teana hu 2009.03.02
		if(pFind == NULL)
		{
			return FALSE;
		}
		
		ParsePDU(pBuf, pFind - pBuf, nBodyLen, pSMSInfo);
	}
    else
	{
		_ASSERTE( 0 );
	}

    return TRUE;
}

BOOL CShortMsg::ParseSMSStatus(LPSTR pBuf,PSMS_INFO pSMSInfo)
{
	_ASSERTE( pSMSInfo != NULL );
	_ASSERTE( pBuf != NULL );

	if(pSMSInfo == NULL || pBuf == NULL)
	{
		return FALSE;
	}

    CCritialSection crs( m_CriticalSection );

	memset(pSMSInfo, 0, sizeof(SMS_INFO));
	int nBufSize = strlen(pBuf);

	LPSTR  pPreFlag    = "+CDS:";
	int    nPreFlagLen = strlen(pPreFlag);

	LPSTR pFind    = FindStr(pBuf, nBufSize, pPreFlag, nPreFlagLen);
	_ASSERTE( NULL != pFind );
	if(pFind == NULL)
	{
		return FALSE;
	}
		
	pBuf = pFind + nPreFlagLen;

    
    if( m_eSMSMode == SMS_MODE_TEXT )
	{
	}
    else if( m_eSMSMode == SMS_MODE_PDU )
	{
		
		// 返回的格式为
		// +CDS: <len>\r\n<Content>\r\n 
		//eg:	07683108200105F006EB0B813165649423F2908040415543239080404155042300
		//07683108200105F006EB0B81 length:24
		//(86)3165649423F2  length:12
		//90804041554323  length:14
		//90804041550423  length:14
		//00

		pFind = strstr(pBuf, "\r\n");
		_ASSERTE( NULL != pFind );
		if(pFind == NULL)
		{
			return FALSE;
		}
		int nBodyLen = atoi(pBuf);
		pBuf = pFind + 2;

		pFind = strstr(pBuf, "\r\n");
		_ASSERTE( NULL != pFind );
		if(pFind == NULL)
		{
			return FALSE;
		}

		pBuf += 24;
		//teana.hu 2010.04.30
		int nNumberLen = 0;
		if(*pBuf == '9' && *(pBuf + 1) == '1')
		{
			pBuf += 2;
			nNumberLen = 12 + 2 * (nBodyLen - 33) - 2;
		}
		else
		{
			nNumberLen = 12 + 2 * (nBodyLen - 33);
		}
		//
		char szNumber [MAX_BUF_LEN + 1] = {0};  //Get phone number
//		char sztemp[1] = {0};
		
//		int nNumberLen = 12 + 2 * (nBodyLen - 33);
		memcpy(szNumber, pBuf, nNumberLen);

		PDUConvert(szNumber);
		int nLen = strlen(szNumber);
		szNumber[nLen - 1] = '\0';
		
		pBuf += nNumberLen;
		pBuf += 14;

		char szDate[MAX_BUF_LEN + 1] = {0}; //Get respond date
		sprintf(szDate, "%c%c-%c%c-%c%c(%c%c:%c%c:%c%c)", *(pBuf+1),*(pBuf),*(pBuf+3),
			*(pBuf+2),*(pBuf+5),*(pBuf+4),*(pBuf+7),*(pBuf+6),
			*(pBuf+9),*(pBuf+8),*(pBuf+11),*(pBuf+10));

		pBuf += 14;
		

		char szContent[MAX_BUF_LEN + 1] = {0};

		memcpy(szContent, pBuf, 2);

#ifdef _UNICODE
		MultiByteToWideChar(CP_ACP,0,szNumber,-1,m_szTempW,strlen(szNumber)+1);
		_tcscpy(pSMSInfo->szNumber, m_szTempW);
		MultiByteToWideChar(CP_ACP,0,szDate,-1,m_szTempW,strlen(szDate)+1);
		_tcscpy(pSMSInfo->szDate, m_szTempW);
		MultiByteToWideChar(CP_ACP,0,szContent,-1,m_szTempW,strlen(szContent)+1);
		_tcscpy(pSMSInfo->szContent, m_szTempW);
#else
		strcpy(pSMSInfo->szNumber, szNumber);
		strcpy(pSMSInfo->szDate, szDate);
		strcpy(pSMSInfo->szContent, szContent);
#endif
		//ParsePDU(pBuf, pFind - pBuf, nBodyLen, pSMSInfo);
	}
    else
	{
		_ASSERTE( 0 );
	}

    return TRUE;
}

// 发送短信息
BOOL CShortMsg::SendSMS( PSMS_INFO pSMSInfo, BOOL bMore /* = FALSE */ )
{
    _ASSERTE( pSMSInfo != NULL );
	//teana hu 2009.03.02
	if(pSMSInfo == NULL)
	{
		return FALSE;
	}
	
    CCritialSection crs( m_CriticalSection );

	if( SMS_MODE_TEXT == m_eSMSMode )
        return SendSMS_TXT(pSMSInfo, bMore);
    else if( SMS_MODE_PDU == m_eSMSMode )
        return SendSMS_PDU( pSMSInfo, bMore );
    else
        _ASSERTE( 0 );

    return FALSE;
}

// 删除短信息
BOOL CShortMsg::DeleteSMS( int nIndex )
{
    CCritialSection crs( m_CriticalSection );
    sprintf(m_szSendBuf, AT_DEL_IDXSMS, nIndex);
	if( !ExecuteCmd(m_szSendBuf, m_nDelOneTimeout) )
	{		
        m_Log.LogEx(_T("!! DeleteSMS Fail, Index is %d !!\r\n"), nIndex);
        m_dwErrCode = SMS_E_DELETESMS;
		return FALSE;
	}
    return TRUE;
}

// 删除全部短信息
BOOL CShortMsg::DeleteAllSMS(void)
{
    CCritialSection crs( m_CriticalSection );

    if( !ExecuteCmd(AT_DEL_ALLSMS, m_nDelAllTimeout) )
    {        
        m_Log.LogEx(_T("!! Delete All SMS Fail !!\r\n"));
        m_dwErrCode = SMS_E_DELETEALLSMS;
		return FALSE;
    }
    return TRUE;
}

void CShortMsg::Release(void)
{
	delete this;
}
void CShortMsg::SetSRR(BOOL bSRR /*=FALSE*/)
{
	m_bSRR = bSRR;
}
int  CShortMsg::GetUnusedRefNum(void)
{
	for(int i=0;i<=MAX_REF_NUM;i++)
	{
		if(m_arrRefNum[i]!=1)
			return i;
	}
	return MAX_REF_NUM;
}

BOOL CShortMsg::IsBM()
{
	memset(m_szSendBuf,0,sizeof(m_szSendBuf));
	strcpy(m_szSendBuf,AT_GET_STORE);
	if( !ExecuteCmd(m_szSendBuf, m_nReadOneTimeout) )
	{		       
        return TRUE;
	}
	
	if(NULL != strstr(m_szRcvBuf,"BM"))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}