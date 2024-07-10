#include "stdafx.h"
#include "SCIShortMsg.h"
#include "ShortMsg.h"
#include "IPort.h"
#include "Converter.h"

#include <algorithm>

const LPCSTR CShortMsg::m_arrSMSStatus[] =
{    "REC UNREAD", "REC READ", "STO UNSENT", "STO SENT" };

BOOL CShortMsg::StartLog( LPCTSTR pszFileName )
{
    _ASSERTE( pszFileName != NULL );
	//teana hu 2009.03.02
	if(pszFileName == NULL)
	{
		return FALSE;
	}
	

    return m_Log.Open( pszFileName );
}

void CShortMsg::StopLog( void )
{
    m_Log.Close();
}

void CShortMsg::SetWndForProgress( HWND hWnd )
{
    _ASSERTE( ::IsWindow(hWnd) );

    m_hProgressWnd = hWnd;
}

void CShortMsg::SetUIThreadForProgress(CWinThread *pUIThread )
{
    _ASSERTE( pUIThread != NULL );
	
    m_pUIThread = pUIThread;
}

// 执行发送并等待结果返回
// 此函数要求返回结果中含有"OK"或">"，否则此函数永远不会返回TRUE
// 接收的内容存放至RcvBuf中
BOOL CShortMsg::ExecuteCmd(LPCSTR pszCmd, DWORD dwDelayTime /* = 200 */ )
{
	if(!(m_pPort != NULL && m_pPort->IsOpened()) )
		return FALSE;

    _ASSERTE( pszCmd != NULL  && strlen(pszCmd) > 0 );
	//teana hu 2009.03.02
	if(pszCmd == NULL  || strlen(pszCmd) == 0)
	{
		return FALSE;
	}
	
	DWORD dwReadTmp = 0;
	for(int j=0;j< m_nRetryTimes; j++)
	{
		m_pPort->Read( (LPVOID)m_szRcvBuf, MAX_BUF_LEN, &dwReadTmp, 1);
	}

	m_pPort->PurgeAll();

    m_pPort->Write( (LPVOID)pszCmd, strlen(pszCmd), NULL );

	if(!(m_pPort != NULL && m_pPort->IsOpened()) )
		return FALSE;
	
    m_dwRcvSize = 0;
    memset( m_szRcvBuf, 0, sizeof(m_szRcvBuf) );
    DWORD dwRead   = 0;
    DWORD dwBufLen = MAX_BUF_LEN;
    LPSTR pRcvBuf  = m_szRcvBuf;
	for( int i = 0; dwBufLen > 0 && i < m_nRetryTimes && m_pPort->IsOpened(); ++i )
	{
		if( m_pPort->Read( (LPVOID)pRcvBuf, dwBufLen, &dwRead, dwDelayTime) && dwRead > 0)
		{
            --i;
            m_dwRcvSize += dwRead;  
			
			if(m_dwRcvSize > 0)
			{
				if( !ParseIncomingIndex(m_szRcvBuf, m_dwRcvSize) )
				{
					continue;
				}
				
				
				if( FindStr(m_szRcvBuf, m_dwRcvSize, AT_SMS_INPUT_PROMPT, strlen(AT_SMS_INPUT_PROMPT)) != NULL ||
					FindStr(m_szRcvBuf, m_dwRcvSize, AT_RES_OK, strlen(AT_RES_OK)) != NULL )
				{
					return TRUE;
				}
				else if(FindStr(m_szRcvBuf, m_dwRcvSize, "ERROR", strlen("ERROR")) != NULL ||
					FindStr(m_szRcvBuf, m_dwRcvSize, "Error", strlen("Error")) != NULL )
				{
					return FALSE;
				}
			}
		}     

        dwBufLen -= dwRead;
        pRcvBuf  += dwRead;
	}

	return FALSE;
}

LPSTR CShortMsg::FindStr(LPCSTR pSrc, int nSrcLen, LPCSTR pDes, int nDesLen)
{
	_ASSERTE( pSrc != NULL && pDes != NULL );
	_ASSERTE( nSrcLen > 0 && nDesLen > 0 );

	LPCSTR pFirst1 = pSrc;
	LPCSTR pLast1  = pSrc + nSrcLen;
	LPCSTR pFirst2 = pDes;
	LPCSTR pLast2  = pDes + nDesLen;
	
    std::search( pFirst1, pLast1, pFirst2, pLast2);

	LPSTR pFind = (LPSTR)std::search( pFirst1, pLast1, pFirst2, pLast2);
	if( pFind != pLast1 )
    {
		return pFind;
    }

	return NULL;
}

void CShortMsg::ProcRcvedContent(LPSTR pContent, int &nLen)
{
	static const int nMaxByteSize  = 512;
	BYTE btContent[ nMaxByteSize ] = { 0 };

	int nByteSize = nMaxByteSize;
	m_converter.HexCharToBinary(pContent, nLen, btContent, nByteSize);
	m_converter.BigEdnToLitEdn( (LPWORD)btContent, nByteSize / 2 );

	memset(pContent, 0, nLen);
	int nMBLen = ::WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)btContent, nByteSize / 2, pContent, nLen , NULL, NULL );

	pContent[nMBLen] = '\0';
}

SMS_STATUS_E CShortMsg::GetSMSStatus(LPCSTR pszState)
{
	_ASSERTE( pszState != NULL );

	static const int nStatusNum = sizeof(m_arrSMSStatus) / sizeof(m_arrSMSStatus[0]);
    
	static const SMS_STATUS_E wStatus[nStatusNum] = 
	{ SMS_STAT_UNREAD, SMS_STAT_READ, SMS_STAT_UNSEND, SMS_STAT_SEND };

	for( int i = 0; i < nStatusNum; ++i )
	{
		if( strcmp( pszState, m_arrSMSStatus[i]) == 0 )
			return wStatus[i];
	}

	_ASSERTE(0);

	return SMS_STAT_UNKNOWN;
}

void CShortMsg::ProcSendContent(PSMS_INFO psms, LPSTR pszSend, int nSMSSize)
{
	_ASSERTE( psms != NULL );
    int nALen = 0;
	int nWLen = 0;

	if(psms->bIsUcs2)
	{		

		nWLen = psms->nContentLen;
		ZeroMemory(m_szTempW,sizeof(m_szTempW));
#ifdef _UNICODE
		memcpy(m_szTempW,psms->szContent,nWLen*sizeof(_TCHAR));
		nALen = nWLen*sizeof(_TCHAR);
#else
		MultiByteToWideChar(CP_ACP,0,psms->szContent,-1,(LPWSTR)m_szTempW,strlen(psms->szContent)+1);
		nALen = wcslen((LPWSTR)m_szTempW) * 2;
#endif
		
		m_converter.LitEdnToBigEdn((LPWORD)m_szTempW, nWLen);		
		m_converter.BinaryToHexChar((LPBYTE)m_szTempW, nALen, pszSend, nSMSSize);
		pszSend[nSMSSize] = 0;
		
	}
	else
	{
		char szTemp[MAX_BUF_LEN+1] = {0};
		nALen = sizeof(szTemp);
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,psms->szContent,-1,szTemp,nALen,NULL,NULL);
#else
		strcpy(szTemp, psms->szContent);
#endif
		//teana hu 2009.05.08 8bits to 7bits

		BYTE temp7Bit [MAX_BUF_LEN + 1] = {0};

		int nLen = Transfer8Bitsto7Bits(szTemp, temp7Bit, strlen(szTemp));

		m_converter.BinaryToHexChar((LPBYTE)temp7Bit,nLen,pszSend,nSMSSize);
	}

//	static const int nUcsSize = 200;
//	WCHAR szUcs[ nUcsSize ] = { 0 };
	
//	int nUcsByteSize = MultiByteToWideChar(CP_ACP, 0, pContent, -1, szUcs, nUcsSize );
//	nUcsByteSize--;
	

//	m_converter.LitEdnToBigEdn(szUcs, nUcsByteSize);

//	m_converter.BinaryToHexChar((LPBYTE)szUcs, nUcsByteSize * 2, pSMS, nSMSSize);
//    *(pSMS + nSMSSize) = 0;
}

int CShortMsg::GetSMSValidDate(int nHours)
{
	// 这里的换算关系可参考at用户手册或sms spec.

	int nValidData = 168; // 1天
	// 我们目前只支持如下几种期限
	switch( nHours )
	{
	case 1:
	case 6:
	case 12:
		nValidData = nHours * 60 / 5 - 1;
		break;
	case 24: // 1 天
	case 72: // 3 天
		nValidData = nHours / 24 + 166;
		break;
	case 120: // 1周
	case 168:
		nValidData = 197; 
		break;
	default:
		_ASSERTE(0);
		break;
	}

	return nValidData;
}
