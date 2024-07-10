#include "stdafx.h"
#include "SCIShortMsg.h"
#include "ShortMsg.h"
#include "IPort.h"
#include "Converter.h"

BOOL CShortMsg::ParseAllSMS_TXT(LPSTR pBuf, int nBufSize)
{
	_ASSERTE( pBuf != NULL && nBufSize > 0 );
    _ASSERTE( m_pSMSInfo != NULL );
	//teana hu 2009.03.02
	if(pBuf == NULL || nBufSize <= 0 || m_pSMSInfo == NULL)
	{
		return FALSE;
	}
	// 返回的格式为
	// \r\n+CMGL: <index>,<编码>,<"状态">,<"号码">,,<"日期"><CR><LF><内容><CR><LF>
	// \r\n+CMGL: <index>,<编码>,<"状态">,<"号码">,,<"日期"><CR><LF><内容><CR><LF>
	//  ...
	// <CR><LF>OK<CR><LF>
	// +CMGL: <index>, <stat>, <st> (for SMS-ATATUS-REPORT).

	PSMS_INFO psms        = NULL;
	LPSTR     pPreFlag    = (LPSTR)AT_RES_GET_ALLSMS_PRE;
	int       nPreFlagLen = strlen(pPreFlag);
	LPSTR     pFind       = NULL;
	LPCSTR    pStart      = NULL;
	for( int i = 0; ; ++i )
	{
		pStart = pBuf;

		pFind = FindStr(pBuf, nBufSize, pPreFlag, nPreFlagLen);
		if( NULL == pFind )
		{
			m_nUsedCount = i;
			break;
		}

		pBuf = pFind + nPreFlagLen;
			
		psms = &m_pSMSInfo[i];
		memset(psms, 0, sizeof(SMS_INFO));
	
		ParseIndex(pBuf, psms);
		if(*pBuf != '"')
		{
			int nCodec = ParseCodec(pBuf, psms);        
			
			ParseStatus(pBuf, psms);
			ParseNum(pBuf, psms);
			ParseDate(pBuf, psms);

			nBufSize -= (pBuf - pStart);
			pStart    = pBuf;
			ParseContent(pBuf, nBufSize, psms, nCodec);		

			nBufSize -= (pBuf - pStart);
		}
		else
		{
			ParseStatus(pBuf, psms);
			ParseSRR(pBuf, psms);
			nBufSize -= (pBuf - pStart);
			pStart    = pBuf;
		}
	}

	return TRUE;
}

BOOL CShortMsg::ParseSMS_TXT(LPSTR pBuf, int nBufSize, PSMS_INFO pSMSInfo)
{
    _ASSERTE( pBuf != NULL && nBufSize > 0 );
    _ASSERTE( pSMSInfo != NULL );

	//teana hu 2009.03.02
	if(pBuf == NULL || nBufSize <= 0 || pSMSInfo == NULL)
	{
		return FALSE;
	}

    memset(pSMSInfo, 0, sizeof(SMS_INFO));

    // 返回的格式为
	// \r\n+CMGR: <状态>,<编码>,<号码>,<日期><CR><LF><内容><CR><LF>
    // <CR><LF>OK<CR><LF>    

	LPSTR  pPreFlag    = (LPSTR)AT_RES_GET_IDXSMS_PRE;
	int    nPreFlagLen = strlen(pPreFlag);

	LPSTR pFind    = FindStr(pBuf, nBufSize, pPreFlag, nPreFlagLen);
	_ASSERTE( NULL != pFind );
	//teana hu 2009.03.02
	if(pFind == NULL)
	{
		return FALSE;
	}
	
	pBuf = pFind + nPreFlagLen;
			  
	ParseStatus(pBuf, pSMSInfo);
    int nCodec = ParseCodec(pBuf, pSMSInfo);      
	ParseNum(pBuf, pSMSInfo);
	ParseDate(pBuf, pSMSInfo);

    nBufSize -= (pBuf - m_szRcvBuf);
    ParseContent(pBuf, nBufSize, pSMSInfo, nCodec);		

    return TRUE;
}

void CShortMsg::ParseIndex( LPSTR &pBuf, PSMS_INFO psms )
{
	_ASSERTE( pBuf != NULL && psms != NULL );

	// index
	LPSTR pFind = strchr(pBuf, ',');
	//teana hu 2009.03.02
	_ASSERTE( pFind != NULL );
	if(pFind != NULL)
	{
		*pFind = '\0';
		if( strlen(pBuf) > 0 )
		{
			psms->nIndex = atoi(pBuf);
			
			pBuf = pFind + 1;
		}
	}
}

int CShortMsg::ParseCodec( LPSTR &pBuf, PSMS_INFO /* psms */ )
{
	_ASSERTE( pBuf != NULL /*&& psms != NULL */);

	// 编码
	LPSTR pFind = strchr(pBuf, ',');
	_ASSERTE( pFind != NULL );
	*pFind = '\0';     
	_ASSERTE( strlen(pBuf) > 0 );
	
	int nCodec = atoi(pBuf);

	pBuf = pFind + 1;

	return nCodec;
}

void CShortMsg::ParseStatus( LPSTR &pBuf, PSMS_INFO psms )
{
	_ASSERTE( pBuf != NULL && psms != NULL );

	// 状态
	LPSTR pFind = strchr(pBuf, '"');
	_ASSERTE( pFind != NULL );
	pBuf = pFind + 1;

	pFind = strchr(pBuf, '"');
	_ASSERTE( pFind != NULL );
	
	*pFind = '\0';     
	
	psms->eStatus = GetSMSStatus(pBuf);

	pBuf = pFind + 2;
}

void CShortMsg::ParseNum( LPSTR &pBuf, PSMS_INFO psms )
{
	_ASSERTE( pBuf != NULL && psms != NULL );
	
	// 号码
	LPSTR pFind = strchr(pBuf, '"');
	_ASSERTE( pFind != NULL );
	pBuf = pFind + 1;

	pFind = strchr(pBuf, '"');
	_ASSERTE( pFind != NULL );
	*pFind = '\0'; 
	
#ifdef _UNICODE
	MultiByteToWideChar(CP_ACP,0,pBuf,-1,psms->szNumber,strlen(pBuf)+1 );
	
#else	
	strcpy(psms->szNumber, pBuf);

#endif

	pBuf = pFind + 2;
	

	
}

void CShortMsg::ParseSRR( LPSTR &pBuf, PSMS_INFO psms )
{
	_ASSERTE( pBuf != NULL && psms != NULL );
	LPSTR pFind=pBuf + 1; //lint !e662
	*pFind='\0'; //lint !e661
    int nSRR = 0;
	nSRR = atoi(pBuf);
	//CString str;
	//GetSRRContent(nSRR,str);
    
	//_tcscpy(psms->szContent,str);
	_stprintf(psms->szContent,_T("%d"),nSRR);
	psms->bRSS = TRUE;

	pBuf = pFind + 1; //lint !e662
}

void CShortMsg::GetSRRContent( int nIndx,CString &strContent)
{
	switch(nIndx)
	{
		case SMS_RECEIVED_BY_SME:
			strContent = "状态报告:已收到";
			break;
		case SMS_NOT_CONFIRM_RECEIVED:
			strContent = "状态报告:不确定是否收到";
			break;
		case SMS_SM_REPLACED_BY_SC:
			strContent = "状态报告:被短信中心替换";
			break;
		case SMS_SEND_LATER:
			strContent = "状态报告:短暂故障，过会儿再发送";
			break;
		case SMS_NOT_SEND_AGAIN:
			strContent = "状态报告:永久故障，不再发送";
			break;
		case SMS_INVALID_STATUS_REPORT:
			strContent = "状态报告:无效状态报告";
			break;
		default:
			_ASSERT(0);
	}
}
void CShortMsg::SkipUnused( LPSTR &pBuf )
{
	_ASSERTE( pBuf != NULL );

	LPSTR pFind = strchr(pBuf, ',');
	_ASSERTE( pFind != NULL );
	pBuf = pFind + 1;
}

void CShortMsg::ParseDate( LPSTR &pBuf, PSMS_INFO psms )
{
	_ASSERTE( pBuf != NULL && psms != NULL );
	
    // 对于接受的信息，日期有效，其格式为"日期"，对于草稿信息来说，其日期无效，
    // 但解析时，应跳过
	if( psms->eStatus != SMS_STAT_SEND && psms->eStatus != SMS_STAT_UNSEND )
	{
		LPSTR pFind = strchr(pBuf, '"');
		_ASSERTE( pFind != NULL );
		pBuf = pFind + 1;

		pFind = strchr(pBuf, ':');
		_ASSERTE( NULL != pFind );
		
		pFind++;
		pFind = strchr(pFind, ':');
		_ASSERTE( pFind != NULL );
		
		*pFind = '\0';
		for( LPSTR pPos = pBuf; pPos < pFind ; ++pPos )
		{
			if( *pPos == '/' ) *pPos = '-';
			if( *pPos == ',' ) *pPos = ' ';
		}
		
		//strcpy(psms->szDate, pBuf);
#ifdef _UNICODE
		MultiByteToWideChar(CP_ACP,0,pBuf,-1,psms->szDate,strlen(pBuf)+1);
#else
		strcpy(psms->szDate, pBuf);
#endif
        
		
		pBuf  = pFind + 1;
		pFind = strchr(pBuf, '"');
		_ASSERTE( pFind != NULL );
		pBuf = pFind + 3;
	}
    else
    {
        LPSTR pFind = strstr(pBuf, "\r\n");
        _ASSERTE( pFind != NULL );

		pBuf = pFind + 2;
        
    }    
}

void CShortMsg::ParseContent( LPSTR &pBuf, int nBufSize, PSMS_INFO psms, int nCodec )
{
	_ASSERTE( pBuf != NULL && nBufSize > 0 && psms != NULL );

	BOOL bAdd2 = FALSE;
	LPSTR pFind = FindStr( pBuf, nBufSize, AT_RES_GET_ALLSMS_PRE, strlen(AT_RES_GET_ALLSMS_PRE) );
	if( pFind != NULL )
    {
        pFind -= 4; // \r\n\r\n
	    *pFind = '\0';
        bAdd2 = TRUE;
    }
    else
    {
        pFind = FindStr( pBuf, nBufSize, AT_RES_OK, strlen(AT_RES_OK) );
        _ASSERTE( pFind != NULL );

        pFind -= 2; // \r\n 
        *pFind = '\0';
    }
	
	if(pFind <= pBuf)
	{
		memset(psms->szContent, 0, sizeof(psms->szContent));
		if(bAdd2)// \r\n
			pBuf +=2; //lint !e662
		return;
	}

    int nRcvSize = pFind - pBuf;
	// Process content
	if( nCodec == 2 )
	{
		psms->bIsUcs2 = TRUE;

		ProcRcvedContent( pBuf, nRcvSize); 
        nRcvSize /= 2;
	}   

	memset(psms->szContent, 0, sizeof(psms->szContent));
	 _ASSERTE( nRcvSize < sizeof(psms->szContent) );
	 
	if(nRcvSize < sizeof(psms->szContent))
	{
		memcpy(psms->szContent, pBuf, nRcvSize);//lint !e670
		psms->nContentLen = nRcvSize;
	}
	else
	{
	    memcpy(psms->szContent, pBuf, sizeof(psms->szContent)-2);//lint !e670
		psms->nContentLen = sizeof(psms->szContent)-2;
	}
	
	psms->eStorePlace = m_eStorePlace;
	
    if( bAdd2 )
    {
		// \r\n 
		pBuf = pFind + 2; //lint !e662
	    
    }
}

BOOL CShortMsg::SendSMS_TXT( PSMS_INFO pSMSInfo, BOOL bMore )
{
    _ASSERTE( pSMSInfo != NULL );
	//teana hu 2009.03.02
	if(pSMSInfo == NULL)
	{
		return FALSE;
	}

    int nValidDate = 24; // Hours
	if( pSMSInfo->nValidDate != 0 )
		nValidDate = pSMSInfo->nValidDate;

    static const int nSendSize   = 800;
	char szSend[ nSendSize + 1 ] = { 0 };

//	if( pSMSInfo->bIsUcs2 )
//	{
//		int nSendLen = nSendSize;
//		ProcSendContent( pSMSInfo->szContent, szSend, nSendLen );
//	}
//	else
//	{
//		strcpy( szSend, pSMSInfo->szContent );
//	}
	int nSendLen = nSendSize;
	ProcSendContent( pSMSInfo, szSend, nSendLen );

    strcat(szSend, AT_SMS_INPUT_END_FLAG);

	int nCode = pSMSInfo->bIsUcs2 ? AT_SMS_UCS2 : AT_SMS_ASCII;
	if(!m_bSRR)
		sprintf(m_szSendBuf, AT_SMS_SENDMODE, GetSMSValidDate(nValidDate), nCode);
	else
		sprintf(m_szSendBuf, AT_SMS_SENDMODE_SRR, GetSMSValidDate(nValidDate), nCode);
	if( !ExecuteCmd(m_szSendBuf, m_nReadOneTimeout) )
	{		
        m_Log.LogEx(_T("!! Set Send SMS Mode Fail !!\r\n"));
        m_dwErrCode = SMS_E_SENDSMS;
		return FALSE;
	}

	sprintf(m_szSendBuf, AT_SMS_SEND_TXT, pSMSInfo->szNumber, bMore);
	if( !ExecuteCmd(m_szSendBuf, m_nReadOneTimeout) )
	{		
        m_Log.LogEx(_T("!! Begin Send SMS Fail !!\r\n"));
        m_dwErrCode = SMS_E_SENDSMS;
		return FALSE;
	}	
	if( !ExecuteCmd(szSend, m_nSendTimeout) )
	{	
        m_Log.LogEx(_T("!! Send sms fail !!\r\n"));
        m_dwErrCode = SMS_E_SENDSMS;
		return FALSE;
	}

	return TRUE;
}
