#include "stdafx.h"
#include "SCIShortMsg.h"
#include "ShortMsg.h"
#include "IPort.h"
#include "Converter.h"

const short CShortMsg::SMS_PDU_7BIT    = 0x00;
const short CShortMsg::SMS_PDU_8BIT    = 0x04;
const short CShortMsg::SMS_PDU_UNICODE = 0x08;


const unsigned char default_to_ascii_table[128] =
{
    0x40,0x20,0x24,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0A,0x20,0x20,0x0D,0x20,0x20,
    0x20,0x5F,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x20,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x20,0x20,0x20,0x20,0x20,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x20,0x20,0x20,0x20,0x20
};

const unsigned char called_party_bcd_number[]="*#abc";

BOOL CShortMsg::ParseAllSMS_PDU(LPSTR pBuf, int nBufSize)
{
    _ASSERTE( pBuf != NULL && nBufSize > 0 );
    _ASSERTE( m_pSMSInfo != NULL );

	//teana hu 2009.03.02
	if(pBuf == NULL || nBufSize <= 0 || m_pSMSInfo == NULL )
	{
		return FALSE;
	}

	//teana hu 2008-12-29
	BOOL bRet = TRUE;

	// 返回的格式为
	// \r\n+CMGL: <index>,状态,,长度\r\n\r\n<内容>\r\n
	// \r\n+CMGL: <index>,状态,,长度\r\n\r\n<内容>\r\n
	//  ...
	// <CR><LF>OK<CR><LF>

	PSMS_INFO psms        = NULL;
	LPSTR     pPreFlag    = (LPSTR)AT_RES_GET_ALLSMS_PRE;
	int       nPreFlagLen = strlen(pPreFlag);
	LPSTR     pFind       = NULL;
	LPCSTR    pStart      = NULL;
	char	  szBuf[20] = {0};
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
		psms->eStorePlace = m_eStorePlace;
	
        // index
        pFind = strchr(pBuf, ',');
		//teana hu 2009.03.02
        _ASSERTE( pFind != NULL );
		if(pFind == NULL)
		{
			return FALSE;
		}
//        *pFind = '\0';
		memset(szBuf,0,sizeof(szBuf));
		memcpy(szBuf,pBuf,pFind-pBuf);
        psms->nIndex = atoi(szBuf);
        pBuf = pFind + 1;
		
        // status
        pFind = strchr(pBuf, ',');
		//teana hu 2009.03.02
        _ASSERTE( pFind != NULL );
		if(pFind == NULL)
		{
			return FALSE;
		}
//       *pFind = '\0';
		memset(szBuf,0,sizeof(szBuf));
		memcpy(szBuf,pBuf,pFind-pBuf);
        psms->eStatus = (SMS_STATUS_E)atoi(szBuf);
        pBuf = pFind + 1;

		//判断是否是状态报告
        pFind = strchr(pBuf, ',');
		if(pFind == NULL || (pFind != NULL && ((pFind - pBuf) >3)))
		{
			ParseSRR(pBuf, psms);
			nBufSize -= (pBuf - pStart);
			pStart    = pBuf;
			continue;
		}

        // body length
        pFind = strchr(pBuf, ',');
		//teana hu 2009.03.02
        _ASSERTE( pFind != NULL );
		if(pFind == NULL)
		{
			return FALSE;
		}
        pBuf = pFind + 1;
        pFind = strstr(pBuf, "\r\n");
		//teana hu 2009.03.02
        _ASSERTE( pFind != NULL );
		if(pFind == NULL)
		{
			return FALSE;
		}
        //*pFind = '\0';
		memset(szBuf,0,sizeof(szBuf));
		memcpy(szBuf,pBuf,pFind-pBuf);
        int nBodyLen = atoi(szBuf);

        // content
		// remove \r\n[\r\n]
		pBuf = pFind;
		if(strstr(pBuf, "\r\n") == pBuf)
		{
			pBuf+= 2;
		}
		if(strstr(pBuf, "\r\n") == pBuf)
		{
			pBuf+= 2;
		}
        //pBuf = pFind + 4; // \r\n\r\n
        pFind = strstr(pBuf, "\r\n");
		//teana hu 2009.03.02
        _ASSERTE( pFind != NULL );
		if(pFind == NULL)
		{
			return FALSE;
		}

		if( (pFind-pBuf) >= nBodyLen )
		{
			bRet = ParsePDU(pBuf, pFind - pBuf, nBodyLen, psms);
		}
		
//		TRACE(_T("-------------------Read %d\n"),i);

		pBuf = pFind + 2; // \r\n
        

		nBufSize -= (pBuf - pStart);
		pStart    = pBuf;
	}

	return bRet;
}

BOOL CShortMsg::ParseSMS_PDU( LPSTR pBuf, int /* nBufSize */, PSMS_INFO pSMSInfo )
{
    _ASSERTE( pBuf != NULL /* && nBufSize > 0 */ );
    _ASSERTE( pSMSInfo != NULL );

	//teana hu 2009.03.02
	if(pBuf == NULL || pSMSInfo == NULL)
	{
		return FALSE;
	}

    memset(pSMSInfo, 0, sizeof(SMS_INFO));

    // 返回的格式为
	// +CMGR: <状态>, ,长度\r\n\r\n<内容><CR><LF>
    // <CR><LF>OK<CR><LF>   
    
	LPSTR  pPreFlag    = (LPSTR)AT_RES_GET_IDXSMS_PRE;
	int    nPreFlagLen = strlen(pPreFlag);

	LPSTR pFind = strstr(pBuf, pPreFlag);
	_ASSERTE( NULL != pFind );
	//teana hu 2009.03.02
	if(pFind == NULL)
	{
		return FALSE;
	}
	
	pBuf = pFind + nPreFlagLen;
    pFind = strchr(pBuf, ',');
    _ASSERTE( NULL != pFind );
	//teana hu 2009.03.02
	if(pFind == NULL)
	{
		return FALSE;
	}
    *pFind = '\0';
    pSMSInfo->eStatus = (SMS_STATUS_E)atoi(pBuf);
    pBuf = pFind + 1;

    pFind = strchr(pBuf, ',');
    _ASSERTE( NULL != pFind );
	//teana hu 2009.03.02
	if(pFind == NULL)
	{
		return FALSE;
	}
    pBuf = pFind + 1;

    pFind = strstr(pBuf, "\r\n");
    _ASSERTE( NULL != pFind );
	//teana hu 2009.03.02
	if(pFind == NULL)
	{
		return FALSE;
	}
    *pFind = '\0';
    int nBodyLen = atoi(pBuf);

    pBuf = pFind + 4; // \r\n\r\n
    pFind = strstr(pBuf, "\r\n");
    _ASSERTE( NULL != pFind );
	//teana hu 2009.03.02
	if(pFind == NULL)
	{
		return FALSE;
	}

    return  ParsePDU(pBuf, pFind - pBuf, nBodyLen, pSMSInfo);
   	
   // return TRUE;
}

BOOL CShortMsg::ParsePDU( LPSTR pBuf, int nBufLen, int nBodyLen, PSMS_INFO pSMSInfo )
{
    _ASSERTE( pBuf != NULL && nBufLen > 0 );
    _ASSERTE( nBodyLen < nBufLen );
    _ASSERTE( pSMSInfo != NULL );

	//teana hu 2009.03.02
	if(pBuf == NULL || nBufLen <= 0 || nBodyLen >= nBufLen || pSMSInfo == NULL)
	{
		return FALSE;
	}

    // Message PDU Format
    //
    // sample:
    //   0891 683108200105F0 000B A1 3118966659F8 00 08 30609031248420 06 006100620063
    // 
    // structure:
    //   0891...............service center address prefix flag  4
    //   683108200105F0.....service center address              x
    //   00                                                     2
    //   0B.................destination address length          2
    //   A1.................phone number type                   2
    //   3118966659F8.......destintion address ( sender )       x
    //   00                                                     2
    //   08.................data coding scheme (unicode)        2
    //   30609031248420.....time stamp                          14
    //   06.................user data length in hex             2
    //   006100620063.......user data                           x
    //
    // NOTE:
    // nBodyLength does not include the service center address !
    //   nBodyLength = [000B A1 3118966659F8 00 08 30609031248420 06 006100620063]

	//接收PDU 串和发送PDU 串结构是不完全相同的。通过一个实例来分析，假定收到的短消
	//息其PDU 串为：
	// 08 91 68 31 08 10 00 05 F0 04 0D 91 68 31 19 10 99 91 F2 00 08 40 40 60 31 35 30
	// 23 06 60 A8 59 7D FF 01
	//
	//分段含义解释说明
	// 08                    SMSC 地址信息的长度共8 个八位字节(包括91)
	// 91                    SMSC 地址格式(TON/NPI) 用国际格式号码(在前面加‘+’)
	// 68 31 08 10 00 05 F0  SMSC 地址8613800100500，补‘F’凑成偶数个
	// 84                    基本参数(TP-MTI/MMS/RP) 接收，无更多消息，有回复地址
	// 0D                    回复地址数字个数共13 个十进制数(不包括91 和‘F’)
	// 91                    回复地址格式(TON/NPI) 国际格式
	// 68 31 19 10 99 91 F2  回复地址(TP-RA) 8613910199192，补‘F’凑成偶数个
	// 00                    协议标识(TP-PID) 是普通GSM 类型，点到点方式
	// 08                    用户信息编码方式(TP-DCS) UCS2 编码
	// 40 90 10 31 35 30 23  服务时间戳(TP-SCTS) 2004-09-01 13:53:03
	// 06                    用户信息长度(TP-UDL) 实际长度6 个字节
	// 60 A8 59 7D FF 01     用户信息(TP-UD) “您好！”

    nBodyLen *= 2;
    //const int nTmpLen = 800;
    //char szTmp[nTmpLen] = { 0 };
    ZeroMemory(m_szTempA,sizeof(m_szTempA));

    char * const pStart = pBuf;
    LPSTR pEnd = NULL;
	
    // skip the service center
	if(nBufLen < nBodyLen)
	{
#ifdef _DEBUG
		AfxMessageBox(_T("ParsePDU() Error in 1\n"));
#endif
		return FALSE;
	}
    pBuf += (nBufLen - nBodyLen); 

	if((nBufLen - nBodyLen) > 4)
	{
		pSMSInfo->bExistSC = TRUE;
	}
	else
	{
		pSMSInfo->bExistSC = FALSE;
	}

	//First octet of this SMS-DELIVER message
    m_szTempA[0] = *pBuf++; m_szTempA[1] = *pBuf++; m_szTempA[2] = 0;
    short nfo = (short)strtol(m_szTempA, NULL, 16);
	if( nfo & 0x40 )
		pSMSInfo->bUdhi = TRUE;
	else
		pSMSInfo->bUdhi = FALSE;

	if( nfo & 0x20 )
		pSMSInfo->bRSS = TRUE;
	else
		pSMSInfo->bRSS = FALSE;


	if((nfo & 0x3) == 0x1)  // SMS-SUBMIT
	{
		/* skip MR */
		pBuf+=2; 

		/* parse DA (destination address)*/

		/* DA length */
		m_szTempA[0] = *pBuf++; m_szTempA[1] = *pBuf++; m_szTempA[2] = 0;

		/*skip phone number type*/
		pBuf += 2;  
		
		int nDALen = strtol(m_szTempA, NULL, 16);
		if(nDALen != 0)
		{
			int nBoundLen = (nDALen + 1) / 2 * 2;
			
			memcpy(m_szTempA, pBuf, nBoundLen);
			m_szTempA[nBoundLen] = 0;
			PDUConvert(m_szTempA);
			m_szTempA[nDALen] = 0;
			
			//convert to unicode
			ZeroMemory(m_szTempW,sizeof(m_szTempW));
#ifdef _UNICODE
			MultiByteToWideChar(CP_ACP,0,m_szTempA,-1,m_szTempW,strlen(m_szTempA)+1);
#else
			strcpy(m_szTempW, m_szTempA);
#endif
			
			_tcscpy(pSMSInfo->szNumber, m_szTempW);
			
			pBuf += nBoundLen; // pass DA
		}
	}	
	else  // SMS-DELIVER
	{
		/*parse OA (original address)*/

		//OA-Length
		m_szTempA[0] = *pBuf++; m_szTempA[1] = *pBuf++; m_szTempA[2] = 0;
        int nNumLen = strtol(m_szTempA, NULL, 16);
		int nBoundLen = (nNumLen + 1) / 2 * 2; //may be with a trailing F
		
        //phone number type
		m_szTempA[0] = *pBuf++; m_szTempA[1] = *pBuf++; m_szTempA[2] = 0;
        int nNumType = strtol(m_szTempA, NULL, 16); 
		char szNumber[50] ={0};
		if(nNumType == 0x91)
			szNumber[0]='+';
		
        //Sender number, may be with a trailing F
		memcpy(m_szTempA, pBuf, nBoundLen);
		m_szTempA[nBoundLen] = 0;
		PDUConvert(m_szTempA);
		m_szTempA[nNumLen] = 0;
		strcat(szNumber,m_szTempA);
		//convert to unicode
		if(m_szTempW[0] != '\0')
		{
			ZeroMemory(m_szTempW,sizeof(m_szTempW));
		}
		
#ifdef _UNICODE
		MultiByteToWideChar(CP_ACP,0,szNumber,-1,m_szTempW,strlen(szNumber)+1);
#else
		strcpy(m_szTempW, szNumber);
#endif
		
		_tcscpy(pSMSInfo->szNumber, m_szTempW);
		
        // pass OA
		pBuf += nBoundLen; 
	}

	/* skip TP-PID */
    pBuf += 2;  
	
    /* DCS (data coding scheme)*/
    m_szTempA[0] = *pBuf++; m_szTempA[1] = *pBuf++;  m_szTempA[2] = 0;
    short nCodec = (short)strtol(m_szTempA, &pEnd, 16);
	
	nCodec &= 0x0C;

    /* VP/SCTS */
	if( (nfo & 0x18) == 0x10) //int
	{
		// validDate
		m_szTempA[0] = *pBuf++;
		m_szTempA[1] = *pBuf++;
		m_szTempA[2] = 0;
		pSMSInfo->nValidDate = strtol(m_szTempA, NULL, 16);
	}
	else if( (nfo & 0x18) == 0x11 || ( (nfo & 0x3) == 0x0 && (nfo & 0x18) == 0x0 ) ) // semi-octet
	{
		memcpy(m_szTempA, pBuf, 14);
		m_szTempA[14] = 0;
		PDUConvert(m_szTempA);
        char szDate[20] = {0};
		LPSTR pDate = szDate; //pSMSInfo->szDate;
		LPSTR pTmp  = m_szTempA;
		// year-month-day
		*pDate++ = *pTmp++; *pDate++ = *pTmp++; *pDate++ = '-';
		*pDate++ = *pTmp++; *pDate++ = *pTmp++; *pDate++ = '-';
		*pDate++ = *pTmp++; *pDate++ = *pTmp++; *pDate++ = ' ';
		// hour:minute:second
		*pDate++ = *pTmp++; *pDate++ = *pTmp++; *pDate++ = ':';
		*pDate++ = *pTmp++; *pDate++ = *pTmp++; *pDate++ = ':';
		*pDate++ = *pTmp++; *pDate++ = *pTmp++; *pDate++ = '\0';
        
		pBuf += 14; // skip timestamp
		
		//convert to unicode
		ZeroMemory(m_szTempW,sizeof(m_szTempW));
#ifdef _UNICODE
		MultiByteToWideChar(CP_ACP,0,szDate,-1,m_szTempW,strlen(szDate)+1);
#else
		strcpy(m_szTempW, szDate);
#endif
		
		_tcscpy(pSMSInfo->szDate, m_szTempW);
	}

    // content length
    m_szTempA[0] = *pBuf++;
    m_szTempA[1] = *pBuf++;
    m_szTempA[2] = 0;
    int nCharNum = strtol(m_szTempA, NULL, 16);

	if(nCharNum == 0)
	{
		pSMSInfo->bIsUcs2     = FALSE;
		pSMSInfo->nContentLen = 0;
		return TRUE;
	}

    int nContentLen = nBufLen - (pBuf - pStart);
	if(nContentLen < 0)
	{
#ifdef _DEBUG
		AfxMessageBox(_T("ParsePDU() Error in 2\n"));
#endif
		return FALSE;
	}
    //udhi
	if(pSMSInfo->bUdhi)
	{
		//udhi length
		m_szTempA[0] = *pBuf++; m_szTempA[1] = *pBuf++; m_szTempA[2] = 0;
		short nUdhl = (short)strtol(m_szTempA, NULL, 16);

		pBuf+=((nUdhl-3)*2);
		m_szTempA[0] = *pBuf++; m_szTempA[1] = *pBuf++; m_szTempA[2] = 0;
		pSMSInfo->nRef   =  strtol(m_szTempA, NULL, 16);
		m_arrRefNum[pSMSInfo->nRef]=1;

		m_szTempA[0] = *pBuf++; m_szTempA[1] = *pBuf++; m_szTempA[2] = 0;
		pSMSInfo->nTotal =  strtol(m_szTempA, NULL, 16);	

		m_szTempA[0] = *pBuf++; m_szTempA[1] = *pBuf++; m_szTempA[2] = 0;
		pSMSInfo->nSeq   =  strtol(m_szTempA, NULL, 16);
		nCharNum -= (nUdhl+1);
		nContentLen -= (nUdhl+1)*2;		
	}
	if(nContentLen < 0)
	{
#ifdef _DEBUG
		AfxMessageBox(_T("ParsePDU() Error in 3\n"));
#endif
		return FALSE;
	}
    memcpy(m_szTempA, pBuf, nContentLen);
    m_szTempA[nContentLen] = '\0';
    if(strlen(m_szTempA)==0)
	{
		pSMSInfo->bIsUcs2     = FALSE;
		pSMSInfo->nContentLen = 0;
		return TRUE;
	}
	
    const int nMaxLen = 200;
    switch( nCodec )
    {
    case SMS_PDU_7BIT:
		{
			int nLen = nMaxLen;
			BYTE btContent[nMaxLen] = { 0 };
			m_converter.HexCharToBinary(m_szTempA, nContentLen, btContent, nLen);
			ZeroMemory(m_szTempA,sizeof(m_szTempA));
			BYTE offset_bits = 0;
			BYTE offset_octets = 0;
			//teana hu 2009.07.21
			if(!pSMSInfo->bIsUcs2 && pSMSInfo->bUdhi)
			{
				BYTE Mend = btContent[0];
				LPBYTE lpContent = &btContent[1];
				Mend = (BYTE)(Mend >> 1);
				nLen = Transfer7Bitsto8Bits( lpContent, (BYTE)(nCharNum - 2), &offset_bits, &offset_octets, (LPBYTE)m_szTempA );

				char szTempBit [MAX_BUF_LEN + 1] = {0};
				strcpy(szTempBit, m_szTempA);
				m_szTempA[0] = Mend;
				strncpy(m_szTempA + 1, szTempBit, strlen(szTempBit));
				
				m_szTempA[nLen + 1] = 0;
			}
			else
			{
				nLen = Transfer7Bitsto8Bits( btContent, (BYTE)nCharNum, &offset_bits, &offset_octets, (LPBYTE)m_szTempA );
				m_szTempA[nLen] = 0;
			}
			//

			//pSMSInfo->szContent[nLen] = 0;
			//convert to unicode
			ZeroMemory(m_szTempW,sizeof(m_szTempW));
#ifdef _UNICODE
			MultiByteToWideChar(CP_ACP,0,m_szTempA,-1,m_szTempW,strlen(m_szTempA)+1);
#else
			strcpy(m_szTempW, m_szTempA);
#endif
			
			_tcscpy(pSMSInfo->szContent, m_szTempW);
			pSMSInfo->bIsUcs2     = FALSE;
			pSMSInfo->nContentLen = nLen;
        }
        break;
    case SMS_PDU_8BIT:
        {
			int nLen = nMaxLen;
			BYTE btContent[nMaxLen] = { 0 };
			//int nLen = sizeof(pSMSInfo->szContent);
			m_converter.HexCharToBinary(m_szTempA, nContentLen, btContent, nLen);
			btContent[nLen] = 0;
			//convert to unicode
			ZeroMemory(m_szTempW,sizeof(m_szTempW));
#ifdef _UNICODE
			MultiByteToWideChar(CP_ACP,0,(LPSTR)btContent,-1,m_szTempW,strlen((LPSTR)btContent)+1);
#else
			strcpy(m_szTempW, (LPSTR)btContent);
#endif
			
			_tcscpy(pSMSInfo->szContent, m_szTempW);			
			//pSMSInfo->szContent[nLen] = 0;
			pSMSInfo->bIsUcs2     = FALSE;
			pSMSInfo->nContentLen = nLen;
        }
        break;
    case SMS_PDU_UNICODE:
        {
			int nLen = nMaxLen;
			BYTE btContent[nMaxLen] = { 0 };
			m_converter.HexCharToBinary(m_szTempA, nContentLen, btContent, nLen);
			//@hongliang.xin 2006-10-11
			//_ASSERT(nLen%2==0);
			m_converter.BigEdnToLitEdn((LPWORD)btContent, nLen/2);
			//nLen = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)btContent, nLen / 2, pSMSInfo->szContent,
			//			   SMS_CONTENT_MAX_LEN, NULL, NULL);
#ifdef _UNICODE
			_tcscpy(pSMSInfo->szContent, (LPCWSTR)btContent);
#else
		//	_tcscpy(pSMSInfo->szContent, (LPSTR)btContent);
			WideCharToMultiByte(CP_ACP,0,(LPCWSTR)btContent,-1,pSMSInfo->szContent,SMS_CONTENT_MAX_LEN + 1,NULL,NULL);
#endif
			
			pSMSInfo->szContent[nLen] = 0;
			pSMSInfo->bIsUcs2 = TRUE;
			pSMSInfo->nContentLen = nLen;
        }
        break;
    default:
		break;
//        _ASSERTE( 0 );
// 		{
// 			int nLen = nMaxLen;
// 			BYTE btContent[nMaxLen] = { 0 };
// 			//int nLen = sizeof(pSMSInfo->szContent);
// 			m_converter.HexCharToBinary(m_szTempA, nContentLen, btContent, nLen);
// 			btContent[nLen] = 0;
// 			//convert to unicode
// 			ZeroMemory(m_szTempW,sizeof(m_szTempW));
// 			MultiByteToWideChar(CP_ACP,0,(LPSTR)btContent,-1,m_szTempW,strlen((LPSTR)btContent)+1);
// 			_tcscpy(pSMSInfo->szContent, m_szTempW);			
// 			//pSMSInfo->szContent[nLen] = 0;
// 			pSMSInfo->bIsUcs2     = FALSE;
// 			pSMSInfo->nContentLen = nLen;
//         }
    }

	//teana hu 2009.08.28

	CString strNumber = pSMSInfo->szNumber;
	
	strNumber.Replace(_T("a"), _T("*"));
	strNumber.Replace(_T("b"), _T("#"));
	strNumber.Replace(_T("c"), _T("a"));
	strNumber.Replace(_T("d"), _T("b"));
	strNumber.Replace(_T("e"), _T("c"));

	strNumber.Replace(_T("A"), _T("*"));
	strNumber.Replace(_T("B"), _T("#"));
	strNumber.Replace(_T("C"), _T("a"));
	strNumber.Replace(_T("D"), _T("b"));
	strNumber.Replace(_T("E"), _T("c"));

	_tcscpy(pSMSInfo->szNumber, strNumber);

	//

    return TRUE;
}

void CShortMsg::PDUConvert( LPSTR pszPDU )
{
    _ASSERTE( pszPDU != NULL );

    int nLen = strlen(pszPDU);
    _ASSERTE( nLen % 2 == 0 );

    nLen--;
    char cValue;
    for( int i = 0; i < nLen; i += 2 )
    {
        cValue = pszPDU[i];
        pszPDU[i] = pszPDU[i + 1];
        pszPDU[i + 1] = cValue;
    }
}

BYTE CShortMsg::GetBits( LPBYTE byte_in,     BYTE bits_length,
                         LPBYTE offset_bits, LPBYTE offset_octets )
{
    BYTE got_len = 0;
    BYTE getting_len;
    BYTE temp;
    BYTE result = 0;

    while(got_len < bits_length)
    {
        getting_len = (BYTE)__min(8 - ( *offset_bits ), bits_length - got_len);
        temp = (BYTE)(( byte_in [ *offset_octets ] >> ( ( *offset_bits ) ) )
                    & ((1 << getting_len ) - 1));
        *offset_bits = (BYTE)(*offset_bits + getting_len);
        if ((*offset_bits) >= 8)
        {
            (*offset_octets)++;
            (*offset_bits) -= 8;
        }
 
        result	= (BYTE)(result + (temp << got_len));
        got_len	= (BYTE)(got_len + getting_len);
    }
	return result;
}

int CShortMsg::Transfer7Bitsto8Bits( LPBYTE tpdu_ptr, BYTE sms_length, LPBYTE offset_octets_ptr,
                                      LPBYTE offset_bits_ptr, LPBYTE user_data_ptr ) 
{
	BYTE i;
	BYTE octets_count;

	octets_count = 0;	

	BYTE data_temp = 0;

	for(i = 0; i < sms_length; i++)
	{
		data_temp = GetBits(tpdu_ptr, 7, offset_bits_ptr, offset_octets_ptr);

		if(data_temp <= 0x20)
		{
			user_data_ptr[octets_count] = Map7BitCharTo8Bit(&data_temp);
		}
		else
		{
			user_data_ptr[octets_count] = data_temp;
		}
		  
		octets_count += 1;	
	}

    return octets_count;
}

int CShortMsg::Transfer8Bitsto7Bits(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    int nSrc;        // 源字符串的计数值
    int nDst;        // 目标编码串的计数值
    int nChar;       // 当前正在处理的组内字符字节的序号，范围是0-7
    unsigned char nLeft = 0;    // 上一字节残余的数据
    
    // 计数值初始化
    nSrc = 0;
    nDst = 0;
    
    // 将源串每8个字节分为一组，压缩成7个字节
    // 循环该处理过程，直至源串被处理完
    // 如果分组不到8字节，也能正确处理
    while(nSrc <= nSrcLength)
    {
        // 取源字符串的计数值的最低3位
        nChar = nSrc & 7;
    
        // 处理源串的每个字节
        if(nChar == 0)
        {
            // 组内第一个字节，只是保存起来，待处理下一个字节时使用
            nLeft = *pSrc;
        }
        else
        {
            // 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节
            *pDst = (unsigned char)((*pSrc << (8-nChar)) | nLeft);
    
            // 将该字节剩下的左边部分，作为残余数据保存起来
            nLeft = (unsigned char)(*pSrc >> nChar);
            // 修改目标串的指针和计数值 pDst++;
            nDst++; 
			pDst++;
        } 
        
        // 修改源串的指针和计数值
        pSrc++; 
		nSrc++;
    }
    
    // 返回目标串长度
    return nDst; 
}

BYTE CShortMsg::Map7BitCharTo8Bit(LPBYTE lpData_in)
{
	BYTE result = 0;

	result = default_to_ascii_table[*lpData_in];

	return result;
}

BOOL CShortMsg::SendSMS_PDU( PSMS_INFO pSMSInfo, BOOL bMore )
{
	UNUSED_ALWAYS(bMore);
    _ASSERTE( pSMSInfo != NULL );

	//teana hu 2009.03.02
	if(pSMSInfo == NULL)
	{
		return FALSE;
	}

    // --------------------------------------------------------------- 
    // For detailed SMS format, please see the following 2 ETSI specs:
    //     03.38:    4       SMS Data Coding Scheme                   
    //     03.40:    9.2.2.2 SMS-SUBMIT type                          
    // ---------------------------------------------------------------

    // at command:
    // [
    //    AT+CMGS=<encoded_message_length><CR>
    //    <PDU><Ctrl+Z>
    // ]

    // remarks:
    // [
    //    1 encoded_message_length:
    //      Octet number, represented in decimal format.
    //      Not including the length of service center.
    //    2 PDU:
    //      PDU is made up of 3 parts: 
    //      service_center + destination_address + message_body 
    //    3 service_center:
    //      Prefixed with 0891. For example:
    //      13010314500 -> 8613010314500 --> 0891683110304105F0
    //    4 destination_address:
    //      Prefixed with 11000B81. For example:
    //      13918202173 --> 11000B813119282071F3
    //    5 message_body:
    //      00<data_coding_scheme>08 + user_data_length + user_data
    //      <data_coding_sheme>:
    //      00[00000000]: default alphabet
    //      04[00000100]: 8-bit data
    //      08[00001000]: UCS2 (unicode format)
    //      <user_data_length>:
    //      1 byte in hexadecimal format
    //      <user_data>:
    //      Encoded into PDU format according to the <data_coding_scheme>  
    //      For example: (here we use unicode format)
    //      hello --> 0008070A00680065006C006C006F
    // ]

    //const int nTmpLen = 800;
    //char szTmp[nTmpLen + 1] = { 0 };
	ZeroMemory(m_szTempA,sizeof(m_szTempA));
	ZeroMemory(m_szTempW,sizeof(m_szTempW));
	
    // no service center;
    strcpy(m_szSendBuf, "00");

	//First octet of the SMS-SUBMIT message
	UINT nFirstOctet = 0x11;
	if(pSMSInfo->bUdhi)
		nFirstOctet |= 0x40;
	if(pSMSInfo->bRSS)
		nFirstOctet |= 0x20;
	sprintf(m_szTempA, "%02x", nFirstOctet);
	strcat(m_szSendBuf, m_szTempA);
	
	//TP-Message-Reference
	strcat(m_szSendBuf, "00");

	//Address-Length and Type-of-Address
	//if begin with '+'
	char szNumber[SMS_NUM_MAX_LEN+1];
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,pSMSInfo->szNumber,-1,szNumber,SMS_NUM_MAX_LEN+1,NULL,NULL);
#else
	strcpy(szNumber, pSMSInfo->szNumber);
#endif
	
	if(pSMSInfo->szNumber[0] == '+')
	{
		sprintf(m_szTempA, "%02x91", strlen(szNumber)-1);
        strcat(m_szSendBuf, m_szTempA);
	}
	else
	{
		sprintf(m_szTempA, "%02x81", strlen(szNumber));
		strcat(m_szSendBuf, m_szTempA);
	}

	// The phone number in semi octets
	if(szNumber[0] == '+')
		strcpy(m_szTempA, szNumber+1); // skip '+'
	else
		strcpy(m_szTempA, szNumber);

    if( strlen(m_szTempA) % 2 != 0 )
        strcat(m_szTempA, "F");
	PDUConvert(m_szTempA);
    strcat(m_szSendBuf, m_szTempA);

	//TP-PID
	strcat(m_szSendBuf, "00");
	
    //TP-DCS. Data coding scheme
    sprintf(m_szTempA, "%02x", pSMSInfo->bIsUcs2 ? SMS_PDU_UNICODE : SMS_PDU_7BIT);
    strcat(m_szSendBuf, m_szTempA);

	//TP-Validity-Period. One Octet.
	//pSMSInfo->nValidDate is by hours
	UINT nValidity = pSMSInfo->nValidDate;
	if((int)nValidity<=0)
		nValidity = 0;
	else if(nValidity > 0 && nValidity <= 12)
		nValidity = (nValidity*60)/5 -1;
	else if(nValidity >12 && nValidity <= 24)
		nValidity = (nValidity -12)*60/30 +143;
	else if(nValidity > 24 && nValidity <= 720)
		nValidity = (nValidity / 24) + 166;
	else 
		nValidity = nValidity / 98 + 192;
	if(nValidity > 255)
		nValidity = 255;
    sprintf(m_szTempA, "%02x", nValidity);
    strcat(m_szSendBuf, m_szTempA);  
 

    char szUDH[20] = { 0 };	
	sprintf(szUDH, "050003%02x%02x%02x", pSMSInfo->nRef,pSMSInfo->nTotal,pSMSInfo->nSeq);

	//teana hu 2009.06.04
	BYTE Mend = 0;
	if(!pSMSInfo->bIsUcs2 && pSMSInfo->bUdhi)
	{
//		_TCHAR szTemp [SMS_CONTENT_MAX_LEN + 1] = {0};
		memcpy(&Mend, pSMSInfo->szContent, 1);
		for(int i = 0; i < pSMSInfo->nContentLen - 1; i++)
		{
			pSMSInfo->szContent[i] = pSMSInfo->szContent[i+1];
		}
		pSMSInfo->szContent[i] = 0;
		pSMSInfo->nContentLen --;
	}

    ZeroMemory(m_szTempA, sizeof(m_szTempA));
    int nLen = MAX_BUF_LEN +1;
    if( pSMSInfo->nContentLen>0 )
    {
		 ProcSendContent(pSMSInfo, m_szTempA, nLen);
    }

	if(!pSMSInfo->bIsUcs2 && pSMSInfo->bUdhi)
	{
		pSMSInfo->nContentLen ++;
	}
	

//    else
//    {
//        m_converter.BinaryToHexChar((LPBYTE)pSMSInfo->szContent, pSMSInfo->nContentLen, szTmp, nLen);
//    }
	if(pSMSInfo->bIsUcs2)
	{
		nLen = strlen(m_szTempA);
	}
	else
	{
		nLen = pSMSInfo->nContentLen * 2;
	}
    char szLen[6];  
	if(pSMSInfo->bUdhi)
	{
		if(pSMSInfo->bIsUcs2)
		{
			sprintf(szLen, "%02x", (nLen +12)/ 2 );
		}
		else
		{
			nLen = (nLen / 2) + 7;
			sprintf(szLen, "%02x", nLen);
			Mend = (BYTE)(Mend << 1);
			char szTemp[20] = { 0 };
			strcpy(szTemp, szUDH);
			sprintf(szUDH, "%s%02x", szTemp, Mend);
		}
		
		strcat(m_szSendBuf, szLen);
		strcat(m_szSendBuf, szUDH);
		strcat(m_szSendBuf, m_szTempA);
	}
	else
	{
		sprintf(szLen, "%02x", nLen / 2 );
		strcat(m_szSendBuf, szLen);
		strcat(m_szSendBuf, m_szTempA);
	}

    // minus service center flag: 2 bytes
    nLen = strlen(m_szSendBuf) - 2; 
    nLen /= 2;
#ifdef SPRD8800
	sprintf(m_szTempA, AT_SMS_SEND_PDU, nLen, bMore);
#else
	sprintf(m_szTempA, "AT+CMGS=%d\r\n", nLen);
#endif
    
	if( !ExecuteCmd(m_szTempA, m_nReadOneTimeout) )
	{		
        m_Log.LogEx(_T("!! Begin Send SMS Fail(pdu set len) !!\r\n"));
        m_dwErrCode = SMS_E_SENDSMS;
		return FALSE;
	}
	
    strcat(m_szSendBuf, AT_SMS_INPUT_END_FLAG);
	if( !ExecuteCmd(m_szSendBuf, m_nSendTimeout) )
	{		
        m_Log.LogEx(_T("!! Send sms fail !!\r\n"));
        m_dwErrCode = SMS_E_SENDSMS;
		return FALSE;
	}
    m_arrRefNum[pSMSInfo->nRef]=1;
    return TRUE;
}

BOOL CShortMsg::SendLongSmsPreSet(LPSTR lpParam)
{
	char szCmd[MAX_BUF_LEN] = {0};
	sprintf(szCmd, AT_LONG_SMS_PRE_SET, lpParam);
	if( !ExecuteCmd(szCmd, 200) )
	{		
        m_Log.LogEx(_T("!! Set Long SMS Pre Fail!!\r\n"));
		return FALSE;
	}
	
	return TRUE;
}