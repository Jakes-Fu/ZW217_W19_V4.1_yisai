// Memo.cpp: implementation of the CMemo class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SCIMemo.h"
#include "Memo.h"
#include "GlobalDef.h"
#include "IPort.h"


#pragma warning(push,3)
#include <algorithm>
#pragma warning(pop)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_RETRY_NUM     10
#define READ_DELAY_TIME   200

const int    CMemo::MMI_ID_LEN              = 4;
// 命令格式：AT## + 标识符 + 模块名 + 操作类型 + 操作参数 + \r\n
const char   CMemo::MMI_GET_INFO[]          = "AT##MMI40\r\n";
const char   CMemo::MMI_ADD_MEMO[]            = "AT##MMI41%s\r\n";
const char   CMemo::MMI_DELETE_MEMO[]         = "AT##MMI42%04d\r\n";
const char   CMemo::MMI_MODIFY_MEMO[]         = "AT##MMI43%s\r\n";
// %04d(两个%c%c)表示改Memo的id
const char   CMemo::MMI_GET_MEMO[]            = "AT##MMI44%04d\r\n";


//AT+CCLK?
//Return:
//+CCLK: <current date and time>
const char CMemo::AT_GET_PHONE_DATE[]    = "AT+CCLK?\r\n";
//AT+CCLK=<date and time string>
//<date and time string>: "yy/mm/dd,hh:mm:ss"，
//total length is 17，users must input data according to this format, 
//or it will return ERROR. 
const char CMemo::AT_SET_PHONE_DATE[]    = "AT+CCLK=%s\r\n";
//+CCLK: 
const char CMemo::AT_DATE_RET_PREFIX[]   = "+CCLK:";

//目前Memo 的存储位置是手机存储器MMI_MEMO_STORE_MS
const char   CMemo::MMI_MEMO_STORE_SIM        = '1';
const char   CMemo::MMI_MEMO_STORE_MS         = '0';

const char   CMemo::MMI_PREFIX[]            = "MMI";
const char   CMemo::MMI_LR[]                = "\r\n";
const char   CMemo::AT_RES_OK[]             = "OK\r\n";
const char   CMemo::AT_RES_ERROR[]          = "ERROR\r\n";
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemo::CMemo()
{
	m_pPort		  = NULL;
	m_pMemo       = NULL;
    m_pDateTime   = NULL;
	m_wStorePlace = 0x00;
	m_nUsedNum    = 0;
	m_nTotalNum   = 0;
	m_dwRead      = 0;

	m_bAT         = FALSE;
	memset(m_szInBuf,0,sizeof(m_szInBuf));
	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	ZeroMemory(m_szCmpCmd,sizeof(m_szCmpCmd));

	m_dwVersion = MMI_VERSION_1;

	//teana hu 2010.05.06
	memset(m_szCfgPathName, 0, MAX_PATH);
	m_bBigEndian = TRUE;
	::GetModuleFileName( NULL, m_szCfgPathName, _MAX_PATH );
    LPTSTR pExt = _tcsrchr(m_szCfgPathName, _T('.'));
    _tcscpy(pExt + 1, _T("ini"));

	m_bBigEndian = GetPrivateProfileInt( _T("Endian"), _T("Endian"), 1, m_szCfgPathName );
	//
	
	TCHAR szPath[_MAX_PATH] = {0};
	_tcscpy(szPath,m_szCfgPathName);
	 LPTSTR pDir = _tcsrchr(szPath, _T('\\'));
    _tcscpy(pDir + 1, _T("Timeout.ini"));

	m_nRetryTimes = GetPrivateProfileInt( _T("MMIMEMO"), _T("RetryTimes"), MAX_RETRY_NUM, szPath );
	m_nReadOneTimeout = GetPrivateProfileInt( _T("MMIMEMO"), _T("ReadOneTimeout"), READ_DELAY_TIME, szPath );
}

CMemo::~CMemo()
{
	if( NULL != m_pMemo )
    {
		delete []m_pMemo;
    }
	if( NULL != m_pDateTime )
	{
		delete []m_pDateTime;
	}
}

BOOL CMemo::StartLog( LPCTSTR pszFileName )
{ 
    _ASSERTE( pszFileName != NULL );

    return m_Log.Open(pszFileName);
}

void CMemo::StopLog( void )
{
    m_Log.Close();
}

void CMemo::SetPort( IPort * pPort )
{
	_ASSERTE( pPort != NULL );

	m_pPort = pPort;
}

BOOL CMemo::Init(void)
{
	if( NULL == m_pMemo )
	{
		m_pMemo = new MEMO;
		if( NULL == m_pMemo )
		{
			TRACE0("!! memory no enough !!\r\n");
            m_Log.LogEx(_T("!! memory no enough !!\r\n"));

			return FALSE;
		}
	}

	return TRUE;
}

BOOL CMemo::ExecuteCmd(LPCSTR lpCmd)
{
	_ASSERT(m_pPort != NULL);		
	if(!m_pPort->IsOpened())
		return FALSE;

    _ASSERTE( lpCmd != NULL && strlen(lpCmd) > 0 );

	DWORD dwReadTmp = 0;
	for(int j=0;j< m_nRetryTimes; j++)
	{
		m_pPort->Read( (LPVOID)m_szInBuf, MAX_BUF_LEN, &dwReadTmp, 1);
	}

	memset( m_szInBuf, 0, sizeof(m_szInBuf) );
	m_pPort->PurgeAll();

	int nDstSize = MAX_BUF_LEN+1;
	
	if(m_dwVersion == MMI_VERSION_1)
	{
		m_pPort->Write((LPVOID)lpCmd, strlen(lpCmd), NULL);
	}
	else
	{	
		memset(m_szCmpCmd,0,sizeof(m_szCmpCmd));
		CompressZero((char*)lpCmd,strlen(lpCmd),m_szCmpCmd,nDstSize);
		m_pPort->Write((LPVOID)m_szCmpCmd, strlen(m_szCmpCmd), NULL);
	}  
	
    if(m_pPort == NULL)
		return FALSE;
	if(!m_pPort->IsOpened())
		return FALSE;	

    LPSTR pBuf    = m_szInBuf;
    int   nBufLen = MAX_BUF_LEN;
	DWORD dwRead  = 0;
    m_dwRead      = 0;
	for( int i = 0; nBufLen > 0 && i < m_nRetryTimes && m_pPort->IsOpened(); ++i )
	{
		if( m_pPort->Read( (LPVOID)pBuf, nBufLen, &dwRead, m_nReadOneTimeout) && dwRead > 0)
		{
            --i;
			m_dwRead += dwRead;
			
			if( ( m_bAT || FindStr(m_szInBuf, m_dwRead, MMI_PREFIX, strlen(MMI_PREFIX)) != NULL) &&
				FindStr(m_szInBuf, m_dwRead, AT_RES_OK, strlen(AT_RES_OK)) != NULL  )
			{
				if(m_dwVersion != MMI_VERSION_1)
				{
					nDstSize = MAX_BUF_LEN+1;
					memcpy(m_szCmpCmd,m_szInBuf,sizeof(m_szInBuf));
					ZeroMemory(m_szInBuf,sizeof(m_szInBuf));
					UnCompressZero(m_szCmpCmd,strlen(m_szCmpCmd),m_szInBuf,nDstSize);
					m_dwRead = nDstSize;
				}
				return TRUE;
			}
			else if(FindStr(m_szInBuf, m_dwRead, AT_RES_ERROR, strlen(AT_RES_ERROR)) != NULL )
			{
				return FALSE;
			}
		}

        pBuf    += dwRead;
        nBufLen -= dwRead;
	}

	return FALSE;
}

LPSTR CMemo::FindStr(LPCSTR pSrc, int nSrcLen, LPCSTR pDes, int nDesLen)
{
	_ASSERTE( pSrc != NULL && pDes != NULL );
	_ASSERTE( nSrcLen > 0 && nDesLen > 0 );

	LPCSTR pFirst1 = pSrc;
	LPCSTR pLast1  = pSrc + nSrcLen;
	LPCSTR pFirst2 = pDes;
	LPCSTR pLast2  = pDes + nDesLen;
	
	LPCSTR pFind = std::search( pFirst1, pLast1, pFirst2, pLast2);
	if( pFind != pLast1 )
    {
		return (LPSTR)pFind;
    }

	return NULL;
}

BOOL CMemo::SelectStorePlace( WORD wStorePlace )
{
	if( wStorePlace != ID_STORE_PHONE &&  wStorePlace != ID_STORE_SIM)
	{
		return FALSE;
	}
	m_wStorePlace = wStorePlace;

	return TRUE;
}

__inline char CMemo::GetMMIUsedStorePlace(WORD wStorePlace)
{
	switch( wStorePlace )
	{
	case ID_STORE_PHONE:
		return MMI_MEMO_STORE_MS;
	case ID_STORE_SIM:
		return MMI_MEMO_STORE_SIM;
	default:
		_ASSERTE( 0 );
	}

	return 0;
}

BOOL CMemo::GetMemoNum( int * pUsedNum, int * pTotalNum )
{
	_ASSERTE( pUsedNum != NULL && pTotalNum != NULL );
	_ASSERTE( m_pPort != NULL);

	if(m_pPort == NULL || pUsedNum== NULL || pTotalNum == NULL)
	{
		return FALSE;
	}

	*pUsedNum = 0;
	*pTotalNum = 0;

	if(!m_pPort->IsOpened())
	{		
		return FALSE;
	}
	if( !Init() )
	{
		return FALSE;
	}
	

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	// command format is "AT##MMI00%c\r\n"
	sprintf( m_szOutBuf, MMI_GET_INFO);
   
    if( !ExecuteCmd(m_szOutBuf) )
	{
		TRACE1("!! GetMemoNum fail, the cmd is: %s !!\r\n", m_szOutBuf);
		return FALSE;
	}

	int nPrefixLen = strlen(MMI_PREFIX);
	//一个字节用两个ASCII字符表示
	_ASSERTE( m_dwRead >= nPrefixLen + sizeof(uint16)*2 + sizeof(MEMO_HEAD_INFO_T)*2 + strlen(AT_RES_OK) );
	
	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, MMI_PREFIX, nPrefixLen);
	if(pFind == NULL)
	{
		return FALSE;
	}

	pFind += nPrefixLen;
	
	int nLen = sizeof(uint16) + sizeof(MEMO_HEAD_INFO_T);
	LPBYTE pBContent = new BYTE[nLen];
	
	m_convtr.HexCharToBinary(pFind,nLen*2,pBContent,nLen);

	uint16 nHITLen = *(uint16*)pBContent;

	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		nHITLen= m_convtr.BigEdnToLitEdn((WORD)nHITLen);
	}
	//

	_ASSERTE( nHITLen == sizeof(MEMO_HEAD_INFO_T) );
	if(nHITLen != sizeof(MEMO_HEAD_INFO_T))
	{
		delete [] pBContent;
		pBContent = NULL;
		return FALSE;
	}

	CUIntArray * pArrID = NULL;
	uint16     * pAllID = NULL;
	uint16       u16Value = 0;
	int          nID     = 0;
	int          i       = 0;

	MEMO_HEAD_INFO_T * pMemoHIT = (MEMO_HEAD_INFO_T *)(pBContent + sizeof(uint16));
	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		*pTotalNum = m_convtr.BigEdnToLitEdn(pMemoHIT->max_record_num);
		*pUsedNum  = m_convtr.BigEdnToLitEdn(pMemoHIT->used_record_num);
	}
	else
	{
		*pTotalNum = pMemoHIT->max_record_num;
		*pUsedNum  = pMemoHIT->used_record_num;
	}
	//
	
	m_nUsedNum = *pUsedNum;
	m_nTotalNum= *pTotalNum;


	if( m_nTotalNum > MEMO_MAX_ENTRY_NUM  || m_nUsedNum > m_nTotalNum )
	{
		goto error;
	}

	// none used memo 
	if( 0 == m_nUsedNum )
	{
		delete [] pBContent;
		pBContent = NULL;
		return TRUE;
	}
	
	if( m_wStorePlace == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( m_wStorePlace == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	
	if(pArrID == NULL)
	{
		goto error;
	}

	pArrID->SetSize( m_nUsedNum );	
	pAllID = (uint16 * )pMemoHIT->all_entry_id;	
	for( i = 0; i < m_nUsedNum; ++i)
	{
		u16Value = *pAllID;
		//teana hu 2010.05.06
		if(m_bBigEndian)
		{
			nID = m_convtr.BigEdnToLitEdn((WORD)u16Value);
		}
		else
		{
			nID = u16Value;
		}
		//
		if(nID > MEMO_MAX_ENTRY_NUM)
		{
			pArrID->RemoveAll();
			goto error;
		}
		pArrID->SetAt(i, nID);
		pAllID++;
	}
	
	delete [] pBContent;
	pBContent = NULL;
	return TRUE;

error:
	m_nUsedNum = *pUsedNum = 0;
	m_nTotalNum= *pTotalNum = 0;
	delete [] pBContent;
	pBContent = NULL;
	return FALSE;
}

_MEMO * CMemo::GetMemo( int nUsedIdx)
{
	_ASSERTE( nUsedIdx >= 0 && nUsedIdx < m_nUsedNum );
	if(nUsedIdx < 0 || nUsedIdx >= m_nUsedNum )
	{
		return NULL;
	}

	CUIntArray * pArrID = NULL;
	if( m_wStorePlace == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( m_wStorePlace == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	
	if(pArrID == NULL)
	{
		return NULL;
	}
	int nID = pArrID->GetAt(nUsedIdx);
	if(nID > MEMO_MAX_ENTRY_NUM)
	{
		return NULL;
	}
	
	if(!Init())
	{
		return NULL;
	}

	memset( m_pMemo, 0, sizeof(MEMO) );
	memset( m_szOutBuf,0,sizeof(m_szOutBuf));

	sprintf( m_szOutBuf, MMI_GET_MEMO, /*GetMMIUsedStorePlace(m_wStorePlace),*/ nID );

    if( !ExecuteCmd(m_szOutBuf) )
	{
        m_Log.LogEx(_T("!! GetMemo fail, the cmd is: %s !!\r\n"), m_szOutBuf);
		return NULL;
	}

	int nPrefixLen = strlen(MMI_PREFIX);

	_ASSERTE( m_dwRead >= nPrefixLen + sizeof(uint16)*2 + sizeof(MEMO_ENTRY_T)*2 + strlen(AT_RES_OK) );
	
	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, MMI_PREFIX, nPrefixLen);
	if(pFind == NULL )
	{
		return NULL;
	}

	pFind += nPrefixLen;
	
	int nLen = sizeof(uint16) + sizeof(MEMO_ENTRY_T);
	LPBYTE pBContent = new BYTE[nLen];
	
	m_convtr.HexCharToBinary(pFind,nLen*2,pBContent,nLen);

	uint16 nHITLen = *(uint16*)pBContent;

	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		nHITLen= m_convtr.BigEdnToLitEdn((WORD)nHITLen);
	}
	//

	if(nHITLen != sizeof(MEMO_ENTRY_T))
	{
		delete [] pBContent;
		return NULL;
	}
	
	MEMO_ENTRY_T * pMemoET = (MEMO_ENTRY_T *)(pBContent + sizeof(uint16));
	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		pMemoET->entry_id = m_convtr.BigEdnToLitEdn(pMemoET->entry_id);
	}
	//	
	if( pMemoET->entry_id != nID )
	{
		delete [] pBContent;
		return NULL;
	}
	
	m_pMemo->nID    = nID;
	m_pMemo->wStore = m_wStorePlace;
	
	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		m_pMemo->memoTime.wYear   = m_convtr.BigEdnToLitEdn(pMemoET->memo_time.year);
		m_pMemo->memoStartDate.wYear   = m_convtr.BigEdnToLitEdn(pMemoET->memo_start_time.year);
		m_pMemo->memoEndDate.wYear   = m_convtr.BigEdnToLitEdn(pMemoET->memo_end_time.year);
		m_pMemo->nServiceID   = m_convtr.BigEdnToLitEdn((DWORD)pMemoET->service_id);
	}
	else
	{
		m_pMemo->memoTime.wYear   = pMemoET->memo_time.year;
		m_pMemo->memoStartDate.wYear   = pMemoET->memo_start_time.year;
		m_pMemo->memoEndDate.wYear   = pMemoET->memo_end_time.year;
		m_pMemo->nServiceID   = pMemoET->service_id;
	}
	//
	m_pMemo->memoTime.btMonth = pMemoET->memo_time.month;
	m_pMemo->memoTime.btDay   = pMemoET->memo_time.day;
	m_pMemo->memoTime.btHour  = pMemoET->memo_time.hour;
	m_pMemo->memoTime.btMinut = pMemoET->memo_time.minute;

	m_pMemo->memoStartDate.btMonth = pMemoET->memo_start_time.month;
	m_pMemo->memoStartDate.btDay   = pMemoET->memo_start_time.day;
	m_pMemo->memoStartDate.btHour  = pMemoET->memo_start_time.hour;
	m_pMemo->memoStartDate.btMinut = pMemoET->memo_start_time.minute;

	m_pMemo->memoEndDate.btMonth = pMemoET->memo_end_time.month;
	m_pMemo->memoEndDate.btDay   = pMemoET->memo_end_time.day;
	m_pMemo->memoEndDate.btHour  = pMemoET->memo_end_time.hour;
	m_pMemo->memoEndDate.btMinut = pMemoET->memo_end_time.minute;


	m_pMemo->fre = pMemoET->fre;

	//teana hu 2010.05.06
	int nTextLen = 0;
	if(m_bBigEndian)
	{
		m_pMemo->nMemoMode = m_convtr.BigEdnToLitEdn(pMemoET->memo_mode);
		nTextLen =  m_convtr.BigEdnToLitEdn(pMemoET->memo_text.text_len);
	}
	else
	{
		m_pMemo->nMemoMode = pMemoET->memo_mode;
		nTextLen =  pMemoET->memo_text.text_len;
	}
	//
		
	if(pMemoET->memo_text.text_len > 0)
	{
		if( pMemoET->memo_text.is_ucs2 )
		{
			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				m_convtr.BigEdnToLitEdn( (LPWORD)pMemoET->memo_text.text, nTextLen / 2 );
			}
			//
			//Ucs2ToAscii((LPWORD)pMemoET->memo_text.text, nTextLen,
			//	m_pMemo->szContent, sizeof(m_pMemo->szContent) );
			_tcscpy(m_pMemo->szContent,(LPTSTR)(pMemoET->memo_text.text));
		}
		else
		{
			int nNameLen = pMemoET->memo_text.text_len;
			if(nNameLen > MEMO_TEXT_MAX_LEN)
			{
				nNameLen = MEMO_TEXT_MAX_LEN;
			}
			//memset(m_pMemo->szContent, 0, sizeof(m_pMemo->szContent));
			AsciiToUcs2((LPSTR)(pMemoET->memo_text.text),nNameLen,m_pMemo->szContent,sizeof(m_pMemo->szContent)/sizeof(_TCHAR));
			//memcpy(m_pMemo->szContent, pMemoET->memo_text.text, nTextLen);
		}
	}
	delete [] pBContent;
	pBContent = NULL;
	
	return m_pMemo;
}

void CMemo::ConvertMemoToMemoET( _MEMO * pMemo, MEMO_ENTRY_T & pet)
{
	_ASSERTE( pMemo != NULL );

	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		pet.entry_id = m_convtr.LitEdnToBigEdn((WORD)pMemo->nID);
		pet.memo_time.year    = m_convtr.LitEdnToBigEdn(pMemo->memoTime.wYear);
		pet.memo_start_time.year   = m_convtr.LitEdnToBigEdn(pMemo->memoStartDate.wYear);
		pet.memo_end_time.year     = m_convtr.LitEdnToBigEdn(pMemo->memoEndDate.wYear);
		pet.memo_mode      = m_convtr.LitEdnToBigEdn(pMemo->nMemoMode);
		pet.service_id     = m_convtr.LitEdnToBigEdn(pMemo->nServiceID);
	}
	else
	{
		pet.entry_id = (uint16)pMemo->nID;
		pet.memo_time.year         = pMemo->memoTime.wYear;
		pet.memo_start_time.year   = pMemo->memoStartDate.wYear;
		pet.memo_end_time.year     = pMemo->memoEndDate.wYear;
	    pet.memo_mode      = pMemo->nMemoMode;
		pet.service_id     = pMemo->nServiceID;
	}
	//
	
	pet.memo_time.month	 = pMemo->memoTime.btMonth;
	pet.memo_time.day	    = pMemo->memoTime.btDay;
	pet.memo_time.hour    = pMemo->memoTime.btHour;
	pet.memo_time.minute  = pMemo->memoTime.btMinut;

	pet.memo_start_time.month	 = pMemo->memoStartDate.btMonth;
	pet.memo_start_time.day	    = pMemo->memoStartDate.btDay;
	pet.memo_start_time.hour    = pMemo->memoStartDate.btHour;
	pet.memo_start_time.minute  = pMemo->memoStartDate.btMinut;

	pet.memo_end_time.month	 = pMemo->memoEndDate.btMonth;
	pet.memo_end_time.day	    = pMemo->memoEndDate.btDay;
	pet.memo_end_time.hour    = pMemo->memoEndDate.btHour;
	pet.memo_end_time.minute  = pMemo->memoEndDate.btMinut;
    pet.fre = pMemo->fre;	

	//
    if(_tcslen(pMemo->szContent) >0)
	{
		if( HasUcs2Char(pMemo->szContent) )
		{
			pet.memo_text.is_ucs2   = TRUE;
			//int nLen = AsciiToUcs2(pMemo->szContent, (WCHAR *)pet.memo_text.text, MEMO_TEXT_MAX_LEN);
			int nLen = _tcslen(pMemo->szContent);
			_tcscpy((LPTSTR)pet.memo_text.text,pMemo->szContent);
			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				pet.memo_text.text_len = m_convtr.LitEdnToBigEdn((WORD)((nLen) * 2));
				m_convtr.LitEdnToBigEdn( (LPWORD)(pet.memo_text.text), nLen );
			}
			else
			{
				pet.memo_text.text_len = (uint16)((nLen) * 2);
			}
		}
		else
		{
			pet.memo_text.is_ucs2   = FALSE;
			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				pet.memo_text.text_len = m_convtr.LitEdnToBigEdn((WORD)_tcslen(pMemo->szContent));
			}
			else
			{
				pet.memo_text.text_len = (uint16)_tcslen(pMemo->szContent);
			}
			//
			//strcpy( (LPSTR)pet.memo_text.text, pMemo->szContent );
			Ucs2ToAscii(pMemo->szContent, _tcslen(pMemo->szContent),
				(LPSTR)(pet.memo_text.text),sizeof(pet.memo_text.text));
		}
	}
}

BOOL CMemo::AddMemo( _MEMO * pMemo )
{
	_ASSERTE( pMemo != NULL );
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );

    pMemo->nID = 0;//GetUnusedID(pMemo->wStore);

	MEMO_ENTRY_T pet = { 0 };

	ConvertMemoToMemoET(pMemo, pet);	

	int  nDesSize = MAX_BUF_LEN;
	char szHexStr[ MAX_BUF_LEN ] = { 0 };

	m_convtr.BinaryToHexChar((LPBYTE)&pet, sizeof(pet), szHexStr, nDesSize);

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	sprintf( m_szOutBuf, MMI_ADD_MEMO, /*GetMMIUsedStorePlace(pMemo->wStore),*/ szHexStr );
	if( !ExecuteCmd(m_szOutBuf) )
	{		
        m_Log.LogEx(_T("!! AddMemo fail !!\r\n"));
		return FALSE;
	}
	int nPrefixLen = strlen(MMI_PREFIX);
	_ASSERTE( m_dwRead >= nPrefixLen + sizeof(uint16)*2 + strlen(AT_RES_OK) );
	
	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, MMI_PREFIX, nPrefixLen);
	if(pFind == NULL)
	{
		return FALSE;
	}
	pFind += nPrefixLen;	

	int nLen = sizeof(uint16);
	LPBYTE pBContent = new BYTE[nLen];	
	m_convtr.HexCharToBinary(pFind,nLen*2,pBContent,nLen);
	uint16 nID = *(uint16*)pBContent;
	delete [] pBContent;
	pBContent = NULL;

	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		nID= m_convtr.BigEdnToLitEdn((WORD)nID);
	}
	//
	pMemo->nID = nID;

	CUIntArray * pArrID = NULL;
	if( pMemo->wStore == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( pMemo->wStore == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	
	if(pArrID == NULL)
	{
		return FALSE;
	}

	pArrID->Add(pMemo->nID);

	return TRUE;
}

BOOL CMemo::ModifyMemo( _MEMO * pMemo )
{
	_ASSERTE( m_pMemo != NULL );
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );
	if(pMemo == NULL || m_pPort == NULL || !m_pPort->IsOpened())
	{
		return FALSE;
	}

	if(pMemo->nID > MEMO_MAX_ENTRY_NUM)
	{
		return FALSE;
	}

	MEMO_ENTRY_T pet = { 0 };
   
	ConvertMemoToMemoET(pMemo, pet);	

	int  nDesSize = MAX_BUF_LEN;
	char szHexStr[ MAX_BUF_LEN ] = { 0 };

	m_convtr.BinaryToHexChar((LPBYTE)&pet, sizeof(pet), szHexStr, nDesSize);

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	sprintf(m_szOutBuf, MMI_MODIFY_MEMO,/* GetMMIUsedStorePlace(pMemo->wStore),*/ szHexStr);
	if( !ExecuteCmd(m_szOutBuf) )
	{	
        m_Log.LogEx(_T("!! ModifyMemo fail !!\r\n"));
		return FALSE;
	}

	return TRUE;
}

BOOL CMemo::DeleteMemo( _MEMO * pMemo )
{
	_ASSERTE( m_pMemo != NULL );
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );
	if(pMemo == NULL || m_pPort == NULL || !m_pPort->IsOpened())
	{
		return FALSE;
	}

	if(pMemo->nID > MEMO_MAX_ENTRY_NUM)
	{
		return FALSE;
	}

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));
	sprintf( m_szOutBuf, MMI_DELETE_MEMO,/* GetMMIUsedStorePlace(pMemo->wStore),*/ pMemo->nID );
	if( !ExecuteCmd(m_szOutBuf) )
	{
        m_Log.LogEx(_T("!! ModifyMemo fail !!\r\n"));
		return FALSE;
	}

	CUIntArray * pArrID = NULL;
	if( pMemo->wStore == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( pMemo->wStore == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	
	_ASSERTE( pArrID != NULL );
	int nCount = pArrID->GetSize();
	for( int i = 0; i < nCount; ++i )
	{
		if( pArrID->GetAt(i) == (unsigned int)(pMemo->nID) )
		{
			pArrID->RemoveAt(i);
			break;
		}
	}

	return TRUE;
}

__inline int CMemo::GetUnusedID(WORD wStore)
{
	CUIntArray * pArrID = NULL;
	if( wStore == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( wStore == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	
	_ASSERTE( pArrID != NULL );

	int nUnusedID = 1;
	int nCount = pArrID->GetSize();
	for( int i = 0; i < nCount; ++i )
	{
		if( (unsigned int)nUnusedID == pArrID->GetAt(i) )
		{
			nUnusedID++;
			continue;
		}
	}

	return nUnusedID;
}

__inline int CMemo::AsciiToUcs2( LPCSTR lpszSrc, int nSrcLen, WCHAR * lpszDes, int nDesSize )
{
	_ASSERTE( lpszSrc != NULL && lpszDes != NULL );
    _ASSERTE( nDesSize >= MultiByteToWideChar( CP_ACP, 0, lpszSrc, nSrcLen, NULL, 0) );

	return MultiByteToWideChar( CP_ACP, 0, lpszSrc, nSrcLen, lpszDes, nDesSize);
}

__inline int CMemo::Ucs2ToAscii( LPCWSTR lpszSrc, int nSrcLen, LPSTR lpszDes, int nDesSize )
{
	_ASSERTE( nDesSize >= nSrcLen );
	_ASSERTE( lpszSrc != NULL && lpszDes != NULL );

	//nSrcLen /= 2;
	UNUSED_ALWAYS(nSrcLen);

    _ASSERTE( nDesSize >= WideCharToMultiByte( CP_ACP, 0, lpszSrc, nSrcLen, NULL, 0, NULL, NULL) );

	return WideCharToMultiByte( CP_ACP, 0, lpszSrc, nSrcLen, lpszDes, nDesSize, NULL, NULL );
}

BOOL CMemo::HasUcs2Char(LPCSTR pStr) const
{
	_ASSERTE( pStr != NULL );

	LPCSTR pNext = NULL;
	while( *pStr )
	{
		pNext = CharNextExA(CP_ACP,pStr,0);
		if( pNext - pStr > 1 )
			return TRUE;

		pStr = pNext;
	}

	return FALSE;
}

BOOL CMemo::HasUcs2Char(LPCTSTR pStr) const
{
	_ASSERTE( pStr != NULL );
    char *pChar = (char *)pStr;
	int nLen = _tcslen(pStr);
	int i=0;
	for(i=0; i<nLen*2; i=i+2)
	{
		if(!(pChar[i] < (UCHAR)0x80 && pChar[i+1] == 0)) 
			break;
	}
	if(i>=nLen*2)
	{
		return FALSE;
	}
	
	return TRUE;
}

_DATE_TIME_T * CMemo::GetPhoneDate(void)
{
   _ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );
   m_bAT = TRUE;

   memset(m_szOutBuf,0,sizeof(m_szOutBuf));

   // command format is "AT+CCLK?\r\n"
   sprintf( m_szOutBuf, AT_GET_PHONE_DATE);
   
   if( !ExecuteCmd(m_szOutBuf) )
   {
	   TRACE1("!! Get Phone Date fail, the cmd is: %s !!\r\n", m_szOutBuf);
	   m_bAT = FALSE;
	   return NULL;
   }
   m_bAT = FALSE;
   int nPrefixLen = strlen(AT_DATE_RET_PREFIX);
   //<date and time string>: "yy/mm/dd,hh:mm:ss"，total length is 17
   _ASSERTE( m_dwRead >= (nPrefixLen + 17 + strlen(AT_RES_OK)) );
   
   LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, AT_DATE_RET_PREFIX, nPrefixLen);
   _ASSERTE( pFind != NULL );
   
   pFind += nPrefixLen;
   pFind = FindStr(pFind,strlen(pFind),"\"",1);
   pFind += 1;
   _ASSERTE( pFind != NULL );

   if(NULL == m_pDateTime)
	   m_pDateTime = new _DATE_TIME_T;
   memset(m_pDateTime,0,sizeof(_DATE_TIME_T));

   *(pFind+2)='\0';
   m_pDateTime->nYear = atoi(pFind);
   pFind +=3;

   *(pFind+2)='\0';
   m_pDateTime->nMonth = atoi(pFind);
   pFind +=3;

   *(pFind+2)='\0';
   m_pDateTime->nDay = atoi(pFind);
   pFind +=3;

   *(pFind+2)='\0';
   m_pDateTime->nHour = atoi(pFind);
   pFind +=3;

   *(pFind+2)='\0';
   m_pDateTime->nMinut = atoi(pFind);
   pFind +=3;

   *(pFind+2)='\0';
   m_pDateTime->nSecond = atoi(pFind);
   pFind +=3;

   return m_pDateTime;
}
BOOL CMemo::SetPhoneDate(_DATE_TIME_T * pDateTime)
{
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	char szDateTime[100]={0};
	sprintf(szDateTime,"%02d/%02d/%02d,%02d:%02d:%02d",
						pDateTime->nYear,
						pDateTime->nMonth,
						pDateTime->nDay,
						pDateTime->nHour,
						pDateTime->nMinut,
						pDateTime->nSecond);
	// command format is "AT+CCLK=<date and time string>\r\n"
	sprintf( m_szOutBuf, AT_SET_PHONE_DATE,szDateTime);
	m_bAT = TRUE;
	if( !ExecuteCmd(m_szOutBuf) )
	{
		m_Log.LogEx(_T("!! Set Phone Date fail, the cmd is: %s !!\r\n"), m_szOutBuf);
		m_bAT = FALSE;
		return FALSE;
	}
	m_bAT = FALSE;
	return TRUE;
}

void CMemo::Release(void)
{
	delete this;
}


void CMemo::CompressZero(char *pSrc, int nSrcLen,char * pDstBuf, int &nDstSize)
{
	_ASSERTE(pSrc != NULL && pDstBuf != NULL);
	
	char * pZeroStart=NULL;
	
	char * pDstCur = pDstBuf;
	char * pSrcCur = pSrc;
	
	for(int i=0; i<nSrcLen; i++)
	{
		if(*pSrcCur != '0' || i==(nSrcLen-1))
		{			
			if(pZeroStart != NULL)
			{
				if(pSrcCur-pZeroStart > 6)
				{
					sprintf(pDstCur,"xx%04x",pSrcCur-pZeroStart);
					pDstCur+=6;						
				}
				else
				{
					for(char *pCur = pZeroStart; pCur < pSrcCur; pCur++)
					{
						*(pDstCur++) = *pCur;
					}
				}
				pZeroStart = NULL;
			}
			
			*(pDstCur++) = *(pSrcCur++);
			
		}
		else
		{
			if(pZeroStart == NULL)
				pZeroStart = pSrcCur;
			pSrcCur++;
		}
		
	}

	nDstSize = pDstCur - pDstBuf;
	
}

void CMemo::UnCompressZero(char *pSrc, int nSrcLen, char * pDstBuf, int &nDstSize)
{
	_ASSERTE(pSrc != NULL && pDstBuf != NULL);
	
//	char * pZeroStart=NULL;
	
	char * pDstCur = pDstBuf;
	char * pSrcCur = pSrc;
	
	char szZeroNum[5]={0};
	
	for(int i=0; i<nSrcLen; i++)
	{
		if(i+1< nSrcLen && *pSrcCur== 'x' && *(pSrcCur+1) == 'x')
		{			
			memcpy(szZeroNum,pSrcCur+2,4);
			int nZeroNum =0;
			sscanf(szZeroNum,"%04x",&nZeroNum);
			memset(pDstCur,'0',nZeroNum);
			
			pSrcCur+=6;
			pDstCur+=nZeroNum;
		}
		else
		{
			*(pDstCur++) = *(pSrcCur++);
		}
		
	}

	nDstSize = pDstCur - pDstBuf;
}

BOOL CMemo::SetMemoParam(DWORD dwFlag,DWORD *pParam )
{
	if(dwFlag == MEMO_PARAM_VERSION)
	{
		m_dwVersion = *pParam;
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CMemo::GetMemoParam(DWORD dwFlag,DWORD **ppParam )
{
	UNUSED_ALWAYS(dwFlag);
	UNUSED_ALWAYS(ppParam);
	return FALSE;
}
