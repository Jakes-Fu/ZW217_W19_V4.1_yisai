// PB.cpp: implementation of the CPB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCIMMIPB.h"
#include "PB.h"
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
#define READ_DELAY_TIME   500

#define PB_INFO_HEAD_PART_SIZE  8


const int    CPB::MMI_ID_LEN              = 4;
// 命令格式：AT## + 标识符 + 模块名 + 操作类型 + 操作参数 + \r\n
const char   CPB::MMI_GET_INFO[]          = "AT##MMI00%c\r\n";
const char   CPB::MMI_ADD_PB[]            = "AT##MMI01%c%s\r\n";
const char   CPB::MMI_DELETE_PB[]         = "AT##MMI02%c%04d\r\n";
const char   CPB::MMI_MODIFY_PB[]         = "AT##MMI03%c%s\r\n";
// 第一个%c表示存储器位置，后两个%c%c表示改pb的id
const char   CPB::MMI_GET_PB[]            = "AT##MMI04%c%04d\r\n";


const char   CPB::MMI_PB_STORE_MS         = '0';
const char   CPB::MMI_PB_STORE_SIM        = '1';
const char   CPB::MMI_PB_STORE_SIM2       = '2';
const char   CPB::MMI_PB_STORE_SIM3       = '3';
const char   CPB::MMI_PB_STORE_SIM4       = '4';

const char   CPB::MMI_PREFIX[]            = "MMI";
const char   CPB::MMI_LR[]                = "\r\n";
const char   CPB::AT_RES_OK[]             = "OK\r\n";
const char   CPB::AT_RES_ERROR[]          = "ERROR\r\n";
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////




CPB::CPB()
{
	m_pPort       = NULL;
	m_pPB         = NULL;

	m_wStorePlace = 0x00;
	m_nUsedNum    = 0;

	m_dwRead      = 0;

	m_dwVersion   = MMI_VERSION_1;

	ZeroMemory(m_szTempA,sizeof(m_szTempA));
	ZeroMemory(m_szTempW,sizeof(m_szTempW));
	ZeroMemory(m_szInBuf,sizeof(m_szInBuf));
	ZeroMemory(m_szOutBuf,sizeof(m_szOutBuf));
	ZeroMemory(m_szCmpCmd,sizeof(m_szCmpCmd));

	m_arrMSID.RemoveAll();
	m_arrSIMID.RemoveAll();
	m_arrSIMID2.RemoveAll();
	m_arrSIMID3.RemoveAll();
	m_arrSIMID4.RemoveAll();

	//teana hu 2010.05.06
	memset(m_szCfgPathName, 0, MAX_PATH);
	m_bBigEndian = TRUE;
	::GetModuleFileName( NULL, m_szCfgPathName, _MAX_PATH );
    LPTSTR pExt = _tcsrchr(m_szCfgPathName, _T('.'));
    _tcscpy(pExt + 1, _T("ini"));

	m_bBigEndian = GetPrivateProfileInt( _T("Endian"), _T("Endian"), 1, m_szCfgPathName );
	//
	
	_TCHAR szPath[_MAX_PATH] = {0};
	_tcscpy(szPath,m_szCfgPathName);
	 LPTSTR pDir = _tcsrchr(szPath, _T('\\'));
    _tcscpy(pDir + 1, _T("Timeout.ini"));

	m_nRetryTimes = GetPrivateProfileInt( _T("MMIPB"), _T("RetryTimes"), MAX_RETRY_NUM, szPath );
	m_nReadOneTimeout = GetPrivateProfileInt( _T("MMIPB"), _T("ReadOneTimeout"), READ_DELAY_TIME, szPath );

}

CPB::~CPB()
{
	if( NULL != m_pPB )
    {
		delete m_pPB;
    }
}

BOOL CPB::StartLog( LPCTSTR pszFileName )
{ 
    _ASSERTE( pszFileName != NULL );

    if(m_Log.Open(pszFileName))
	{
		m_Log.LogEx(_T("=================start pb log================\r\n"));
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CPB::StopLog( void )
{
    m_Log.Close();
}

void CPB::SetPort( IPort * pPort )
{
	_ASSERTE( pPort != NULL );

	m_pPort = pPort;
}

BOOL CPB::Init(void)
{
	if( NULL == m_pPB )
	{
		m_pPB = new PB;
		if( NULL == m_pPB )
		{			
            m_Log.LogEx(_T("Init: memory no enough.\r\n"));
			return FALSE;
		}
	}
	

	return TRUE;
}

BOOL CPB::ExecuteCmd(LPCSTR lpCmd)
{
	if(!(m_pPort != NULL && m_pPort->IsOpened()) )
	{
		m_Log.LogEx(_T("ExecuteCmd: channel is not open.\r\n"));
		return FALSE;
	}
    _ASSERTE( lpCmd != NULL && strlen(lpCmd) > 0 );

	if(lpCmd == NULL || strlen(lpCmd) == 0)
	{
		m_Log.LogEx(_T("ExecuteCmd: param is invalid.\r\n"));
		return FALSE;
	}

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
	
	if(!(m_pPort != NULL && m_pPort->IsOpened()) )
	{
		m_Log.LogEx(_T("ExecuteCmd: channel is not open.\r\n"));
		return FALSE;
	}

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
			if( FindStr(m_szInBuf, m_dwRead, MMI_PREFIX, strlen(MMI_PREFIX)) != NULL &&
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

__inline LPSTR CPB::FindStr(LPCSTR pSrc, int nSrcLen, LPCSTR pDes, int nDesLen)
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

BOOL CPB::SelectStorePlace( WORD wStorePlace )
{
	if( wStorePlace != ID_STORE_PHONE &&
		wStorePlace != ID_STORE_SIM &&
		wStorePlace != ID_STORE_SIM2 &&
		wStorePlace != ID_STORE_SIM3 &&
		wStorePlace != ID_STORE_SIM4)
	{
		m_Log.LogEx(_T("SelectStorePlace: invalid store place[%d].\r\n"),wStorePlace);
		return FALSE;
	}

	m_wStorePlace = wStorePlace;

	return TRUE;
}

__inline char CPB::GetMMIUsedStorePlace(WORD wStorePlace)
{
	switch( wStorePlace )
	{
	case ID_STORE_PHONE:
		return MMI_PB_STORE_MS;
	case ID_STORE_SIM:
		return MMI_PB_STORE_SIM;
	case ID_STORE_SIM2:
		return MMI_PB_STORE_SIM2;
	case ID_STORE_SIM3:
		return MMI_PB_STORE_SIM3;
	case ID_STORE_SIM4:
		return MMI_PB_STORE_SIM4;
	default:
		_ASSERTE( 0 );
	}
	m_Log.LogEx(_T("GetMMIUsedStorePlace: invalid store place[%d].\r\n"),wStorePlace);
	return 0;
}

BOOL CPB::GetPBNum( int * pUsedNum, int * pTotalNum, BOOL bClear /*= FALSE*/ )
{
	UNUSED_ALWAYS(bClear);

	_ASSERTE( pUsedNum != NULL && pTotalNum != NULL );
	_ASSERTE( m_pPort != NULL);
	if(!m_pPort->IsOpened())
	{
		m_Log.LogEx(_T("GetPBNum: port is not open.\r\n"));
		*pUsedNum = 0;
		*pTotalNum = 0;
		return FALSE;
	}

	if( !Init() )
		return FALSE;

	char cStore = GetMMIUsedStorePlace(m_wStorePlace);
	if(cStore == 0)
	{
		return FALSE;
	}

	*pUsedNum = *pTotalNum = 0;
	
	memset( m_szOutBuf, 0, sizeof(m_szOutBuf) );

	// command format is "AT##MMI00%c\r\n"
	sprintf( m_szOutBuf, MMI_GET_INFO, cStore );
   
    if( !ExecuteCmd(m_szOutBuf) )
	{
		m_Log.LogEx(_T("GetPBNum fail, the cmd is: {%s}\r\n"), m_szOutBuf);
		return FALSE;
	}

	int nPrefixLen = strlen(MMI_PREFIX);
	_ASSERTE( m_dwRead >= nPrefixLen + /*sizeof(uint16)*2 + sizeof(PB_HEAD_INFO_T)*2 +*/ strlen(AT_RES_OK) );
	if(m_dwRead < nPrefixLen + strlen(AT_RES_OK))
	{
		m_Log.LogEx(_T("GetPBNum fail, the ack string length is too small: {{\r\n%s\r\n}}\r\n"),m_szInBuf);
		return FALSE;
	}
	
	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, MMI_PREFIX, nPrefixLen);
	_ASSERTE( pFind != NULL );
	if(pFind == NULL)
	{
		m_Log.LogEx(_T("GetPBNum fail, not find the MMI prefix in the ack string: {{\r\n%s\r\n}}\r\n"),m_szInBuf);
		return FALSE;
	}

	pFind += nPrefixLen;

	uint16 nHITLen = 0;
	int    nLen = sizeof(uint16);
	m_convtr.HexCharToBinary(pFind,nLen*2,(LPBYTE)&nHITLen,nLen);
	if(m_bBigEndian)
	{
		nHITLen= m_convtr.BigEdnToLitEdn((WORD)nHITLen);
	}

	if( nHITLen <= PB_INFO_HEAD_PART_SIZE)
	{
		m_Log.LogEx(_T("GetPBNum fail, head length [%d] is invalid, the right len[%d].\r\n"),nHITLen,sizeof(PB_HEAD_INFO_T));
		return FALSE;
	}

	nLen = nHITLen;
	LPBYTE pBContent = new BYTE[nLen];
	memset(pBContent,0,nLen);
	
	m_convtr.HexCharToBinary(pFind+4,nLen*2,pBContent,nLen);

	
	PB_HEAD_INFO_T * pPBHIT = (PB_HEAD_INFO_T *)(pBContent);
	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		*pTotalNum = m_convtr.BigEdnToLitEdn(pPBHIT->max_record_num);
		*pUsedNum  = m_convtr.BigEdnToLitEdn(pPBHIT->used_record_num);
	}
	else
	{
		*pTotalNum = pPBHIT->max_record_num;
		*pUsedNum  = pPBHIT->used_record_num;
	}
	//
	m_nUsedNum = *pUsedNum;

	// none used pb 
	if( 0 == m_nUsedNum || *pTotalNum == 0)
	{
		delete [] pBContent;
		pBContent = NULL;
		return TRUE;
	}


	CUIntArray * pArrID = GetIDArrayPtr(m_wStorePlace);
	
	_ASSERTE( pArrID != NULL );
	pArrID->SetSize( m_nUsedNum );

	//_ASSERTE( *pTotalNum <= MMI_SYNC_PB_MAX_ENTRY_NUM );
	uint8 * pAllID = (uint8 * )pPBHIT->all_entry_id;

	uint8 u8Value = 0;
	int   nIdx    = 0;
	int   nID     = 1;
	int   nTotal  = *pTotalNum;
	//int   nPbArrayCount  = MMI_SYNC_PB_MAX_ENTRY_NUM / 8; //数组长度
	int   nPbArrayCount  = (nTotal + 7) / 8; //数组长度
	if(nPbArrayCount > (nHITLen - PB_INFO_HEAD_PART_SIZE))
	{
		nPbArrayCount = (nHITLen - PB_INFO_HEAD_PART_SIZE);
	}
	for( int i = 0; i < nPbArrayCount; ++i )
	{
		for( int j = 0; j < 8; ++j, ++nID )
		{
			u8Value = *pAllID;
			if(u8Value == 0)
			{
				nID +=8;
				break;
			}
			else
			{
				u8Value >>= j;
				if( u8Value & 0x01 )
				{
					pArrID->SetAt(nIdx, nID);
					nIdx++;
				}

				if( nIdx == m_nUsedNum )
				{
					i = nPbArrayCount;
					break;
				}
			}
		}

		pAllID++;
	}
	
	delete [] pBContent;
	pBContent = NULL;

	//_ASSERTE( nIdx == m_nUsedNum );
	if(nIdx != m_nUsedNum)
	{	
		m_Log.LogEx(_T("GetPBNum warning,the index number[%d] not match used number[%d]!"),nIdx,m_nUsedNum);

		pArrID->RemoveAt(nIdx,m_nUsedNum-nIdx);
		
		//TRACE(_T("!!Get PB Info Fail,the index number[%d] not match used number[%d]!"),nIdx,m_nUsedNum);
		//return FALSE;
		m_nUsedNum = nIdx;
		*pUsedNum = nIdx;	
		
		
	}

	return TRUE;
}

BOOL CPB::GetPBParam(DWORD dwFlag,DWORD *ppParam )
{
	UNREFERENCED_PARAMETER(dwFlag);

	_ASSERTE( m_pPort != NULL);
	_ASSERTE( dwFlag == PB_PARAM_NAME_LEN && ppParam != NULL);

	if(!m_pPort->IsOpened())
	{
		m_Log.LogEx(_T("GetPBNum: port is not open.\r\n"));
		return FALSE;		
	}

	if( !Init() )
		return FALSE;
	
	memset( m_szInBuf, 0, sizeof(m_szOutBuf) );

	// command format is "AT##MMI00%c\r\n"
	sprintf( m_szOutBuf, MMI_GET_INFO, MMI_PB_STORE_MS );
   
    if( !ExecuteCmd(m_szOutBuf) )
	{
		m_Log.LogEx(_T("GetPBParam fail, the cmd is: {%s}.\r\n"),m_szOutBuf);
		return FALSE;
	}

	int nPrefixLen = strlen(MMI_PREFIX);
	_ASSERTE( m_dwRead >= nPrefixLen + /*sizeof(uint16)*2 + sizeof(PB_HEAD_INFO_T)*2 +*/ strlen(AT_RES_OK) );
	if(m_dwRead < (nPrefixLen + strlen(AT_RES_OK)))
	{
		m_Log.LogEx(_T("GetPBParam fail, the ack string length is too small: {{\r\n%s\r\n}}\r\n"),m_szInBuf);
		return FALSE;
	}
	
	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, MMI_PREFIX, nPrefixLen);
	_ASSERTE( pFind != NULL );

	if(pFind == NULL)
	{
		m_Log.LogEx(_T("GetPBParam fail, not find the MMI prefix in the ack string: {{\r\n%s\r\n}}\r\n"),m_szInBuf);
		return FALSE;
	}

	pFind += nPrefixLen;

	
	int nLen = sizeof(uint16) + PB_INFO_HEAD_PART_SIZE;
	LPBYTE pBContent = new BYTE[nLen];
	
	m_convtr.HexCharToBinary(pFind,nLen*2,pBContent,nLen);

	uint16 nHITLen = *(uint16*)pBContent;

	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		nHITLen= m_convtr.BigEdnToLitEdn((WORD)nHITLen);
	}

	//_ASSERTE( nHITLen == sizeof(PB_HEAD_INFO_T) );
	//if(nHITLen != sizeof(PB_HEAD_INFO_T))
	if( nHITLen <= PB_INFO_HEAD_PART_SIZE )
	{
		delete [] pBContent;
		m_Log.LogEx(_T("GetPBParam fail, head length [%d] is invalid, the right len[%d].\r\n"),nHITLen,sizeof(PB_HEAD_INFO_T));
		return FALSE;
	}
	
	PB_HEAD_INFO_T * pPBHIT = (PB_HEAD_INFO_T *)(pBContent + sizeof(uint16));
	(ppParam)[0] = pPBHIT->max_alpha_len;
	(ppParam)[1] = pPBHIT->max_email_len;
	(ppParam)[2] = pPBHIT->max_num_len;
	
	delete [] pBContent;

	return TRUE;
}


_PB * CPB::GetPB( int nUsedIdx)
{
	if(m_dwVersion == MMI_VERSION_1)
		return _GetPBV1(nUsedIdx);
	else if(m_dwVersion == MMI_VERSION_2 || m_dwVersion == MMI_VERSION_1_1)
		return _GetPBV2(nUsedIdx);  // name length 241
	else
		return NULL;
}

void CPB::ConvertPBToPBET( _PB * pPB, MMI_PB_V1::PB_PHONEBOOK_ENTRY_T & pet)
{
	_ASSERTE( pPB != NULL );

	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		pet.entry_id = m_convtr.LitEdnToBigEdn((WORD)pPB->nID);
	}
	else
	{
		pet.entry_id = (uint16)pPB->nID;
	}
	
	pet.group_id = (BYTE)(pPB->nGroupIndex);
    if(_tcslen(pPB->szName)>0)
	{
		if( HasUcs2Char(pPB->szName) )
		{
			pet.entry_name.is_ucs2   = TRUE;
			//int nLen = AsciiToUcs2(pPB->szName, (WCHAR *)pet.entry_name.alpha, MMI_SYNC_PB_NAME_MAX_LEN);
			int nLen = _tcslen(pPB->szName) +1; //inlcude the null-terminated
			_tcscpy((LPTSTR)pet.entry_name.alpha,pPB->szName);
			pet.entry_name.alpha_len = (BYTE)((nLen - 1) * 2);
			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				m_convtr.LitEdnToBigEdn( (LPWORD)pet.entry_name.alpha, nLen );
			}
			//
		}
		else
		{
			pet.entry_name.is_ucs2   = FALSE;
			pet.entry_name.alpha_len = (BYTE)(_tcslen(pPB->szName));
			//strcpy( (LPSTR)pet.entry_name.alpha, pPB->szName );			
			Ucs2ToAscii(pPB->szName,_tcslen(pPB->szName),(LPSTR)(pet.entry_name.alpha),sizeof(pet.entry_name.alpha));
		}
	}

	pet.number_t[0].number_len = (BYTE)(_tcslen(pPB->szPhone));
	//strcpy( (LPSTR)pet.number_t[0].number, pPB->szPhone);
	if(pet.number_t[0].number_len > 0)
		Ucs2ToAscii(pPB->szPhone,_tcslen(pPB->szPhone),(LPSTR)(pet.number_t[0].number),sizeof(pet.number_t[0].number));
	
	pet.number_t[1].number_len = (BYTE)(_tcslen(pPB->szHomePhone));
	//strcpy( (LPSTR)pet.number_t[1].number, pPB->szHomePhone);
	if(pet.number_t[1].number_len > 0)
		Ucs2ToAscii(pPB->szHomePhone,_tcslen(pPB->szPhone),(LPSTR)(pet.number_t[1].number),sizeof(pet.number_t[1].number));
	

	pet.number_t[2].number_len = (BYTE)(_tcslen(pPB->szOfficePhone));
	//strcpy( (LPSTR)pet.number_t[2].number, pPB->szOfficePhone);
	if(pet.number_t[2].number_len > 0)
		Ucs2ToAscii(pPB->szOfficePhone,_tcslen(pPB->szPhone),(LPSTR)(pet.number_t[2].number),sizeof(pet.number_t[2].number));

	
	if(_tcslen(pPB->szEmail)>0)
	{
		if( HasUcs2Char(pPB->szEmail) )
		{
			pet.mail.is_ucs2   = TRUE;
			//int nLen = AsciiToUcs2(pPB->szEmail, (WCHAR *)pet.mail.mail, MMI_SYNC_PB_MAX_MAIL_LEN);
			int nLen = _tcslen(pPB->szName) +1; //inlcude the null-terminated
			_tcscpy((LPTSTR)pet.mail.mail,pPB->szEmail);		
			pet.mail.mail_len = (BYTE)((nLen - 1) * 2);
			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				m_convtr.LitEdnToBigEdn( (LPWORD)pet.mail.mail, nLen );
			}
			//
		}
		else
		{
			pet.mail.is_ucs2   = FALSE;
			pet.mail.mail_len = (BYTE)(_tcslen(pPB->szEmail));
			//strcpy( (LPSTR)pet.mail.mail, pPB->szEmail );			
			Ucs2ToAscii(pPB->szEmail,_tcslen(pPB->szEmail),(LPSTR)(pet.mail.mail),sizeof(pet.mail.mail));
		}
	}
}

BOOL CPB::AddPB( _PB * pPB )
{
	_ASSERTE( pPB != NULL );
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );

    pPB->nID = GetUnusedID(pPB->wStore);

	if(pPB->nID == 0 )
	{
		m_Log.LogEx(_T("AddPB fail, GetUnusedID is zero.\r\n"));
		return FALSE;
	}

	char cStore = GetMMIUsedStorePlace(pPB->wStore);
	if(cStore == 0)
	{
		return FALSE;
	}

	int  nDesSize = MAX_BUF_LEN;
	char szHexStr[ MAX_BUF_LEN ] = { 0 };

	MMI_PB_V1::PB_PHONEBOOK_ENTRY_T pet = { 0 };

	ConvertPBToPBET(pPB, pet);
	
	if(m_dwVersion == MMI_VERSION_2 || m_dwVersion == MMI_VERSION_1_1)  // name length 241
	{		
		m_convtr.BinaryToHexChar((LPBYTE)&pet, sizeof(pet), szHexStr, nDesSize);
	}
	else
	{
		PB_PHONEBOOK_ENTRY_T pet0 = {0};
		ConvertPBETV1ToV0(&pet,pet0);
		m_convtr.BinaryToHexChar((LPBYTE)&pet0, sizeof(pet0), szHexStr, nDesSize);
	}

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	sprintf( m_szOutBuf, MMI_ADD_PB, cStore, szHexStr );
	if( !ExecuteCmd(m_szOutBuf) )
	{		
		if(m_pPort->IsOpened())
		{
			int nUsed = 0;
			int nTotal = 0;

			if(GetPBNum(&nUsed,&nTotal,FALSE))
			{
				CUIntArray * pArrID = GetIDArrayPtr(pPB->wStore);	
				_ASSERTE( pArrID != NULL );
				
				int nCount = pArrID->GetSize();
				for( int i = 0; i < nCount; ++i )
				{
					if( pArrID->GetAt(i) == (unsigned int)pPB->nID )
					{						
						break;
					}
				}
				if(i>=nCount)
				{
					m_Log.LogEx(_T("AddPb fail.\r\n"));
					return FALSE;
				}
				else
				{
					return TRUE; // ID already stored
				}
			}
		}
		else
		{
			m_Log.LogEx(_T("AddPb fail.\r\n"));
			return FALSE;
		}        
	}

	CUIntArray * pArrID = GetIDArrayPtr(pPB->wStore);	
	_ASSERTE( pArrID != NULL );

	//pArrID->Add(pPB->nID);
	AddID(pArrID,pPB->nID);

	return TRUE;
}

BOOL CPB::ModifyPB( _PB * pPB )
{
	_ASSERTE( m_pPB != NULL );
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );
	if(pPB == NULL || m_pPort == NULL || !m_pPort->IsOpened())
	{
		m_Log.LogEx(_T("ModifyPB fail, invalid param.\r\n"));
		return FALSE;
	}

	int  nDesSize = MAX_BUF_LEN;
	char szHexStr[ MAX_BUF_LEN ] = { 0 };

	char cStore = GetMMIUsedStorePlace(pPB->wStore);
	if(cStore == 0)
	{
		return FALSE;
	}

	MMI_PB_V1::PB_PHONEBOOK_ENTRY_T pet = { 0 };

	ConvertPBToPBET(pPB, pet);

	if( pet.entry_id == 0 )
	{
		m_Log.LogEx(_T("ModifyPB fail, entry_id is zero, [OrgPB id:%d].\r\n"),pPB->nID);
		return FALSE;
	}
	
	if(m_dwVersion == MMI_VERSION_2 || m_dwVersion == MMI_VERSION_1_1)// name length 241
	{		
		m_convtr.BinaryToHexChar((LPBYTE)&pet, sizeof(pet), szHexStr, nDesSize);		
	}
	else
	{
		PB_PHONEBOOK_ENTRY_T pet0 = {0};
		ConvertPBETV1ToV0(&pet,pet0);
		m_convtr.BinaryToHexChar((LPBYTE)&pet0, sizeof(pet0), szHexStr, nDesSize);
	}

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	sprintf(m_szOutBuf, MMI_MODIFY_PB, cStore, szHexStr);
	if( !ExecuteCmd(m_szOutBuf) )
	{
        m_Log.LogEx(_T("ModifyPB fail.\r\n"));
		return FALSE;
	}

	return TRUE;
}

BOOL CPB::DeletePB( _PB * pPB )
{
	_ASSERTE( pPB != NULL );
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );

	if( pPB == NULL || pPB->nID == 0 )
	{
		m_Log.LogEx(_T("DeletePB: param is invalid, pb id is zero.\r\n"));
		return FALSE;
	}
	char cStore = GetMMIUsedStorePlace(pPB->wStore);
	if(cStore == 0)
	{
		return FALSE;
	}

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	sprintf( m_szOutBuf, MMI_DELETE_PB, cStore, pPB->nID );
	if( !ExecuteCmd(m_szOutBuf) )
	{
        m_Log.LogEx(_T("DeletePB fail.\r\n"));
		return FALSE;
	}

	CUIntArray * pArrID = GetIDArrayPtr(pPB->wStore);	
	_ASSERTE( pArrID != NULL );

	int nCount = pArrID->GetSize();
	for( int i = 0; i < nCount; ++i )
	{
		if( pArrID->GetAt(i) == (unsigned int)pPB->nID )
		{
			pArrID->RemoveAt(i);
			break;
		}
	}

	return TRUE;
}

__inline UINT CPB::GetUnusedID(WORD wStore)
{
	CUIntArray * pArrID = GetIDArrayPtr(wStore);	
	_ASSERTE( pArrID != NULL );

	UINT nUnusedID = 1;
	int nCount = pArrID->GetSize();
	for( int i = 0; i < nCount; ++i )
	{
		if( nUnusedID == pArrID->GetAt(i) )
		{
			nUnusedID++;
		}
		else
		{
			break;
		}
	}

	return nUnusedID;
}

__inline int CPB::AsciiToUcs2( LPCSTR lpszSrc, int nSrcLen, WCHAR * lpszDes, int nDesSize )
{
	_ASSERTE( lpszSrc != NULL && lpszDes != NULL );
    _ASSERTE( nDesSize >= MultiByteToWideChar( CP_ACP, 0, lpszSrc, nSrcLen, NULL, 0) );

	return MultiByteToWideChar( CP_ACP, 0, lpszSrc, nSrcLen, lpszDes, nDesSize);
}

__inline int CPB::Ucs2ToAscii( LPCWSTR lpszSrc, int nSrcLen, LPSTR lpszDes, int nDesSize )
{
	_ASSERTE( nDesSize >= nSrcLen );
	_ASSERTE( lpszSrc != NULL && lpszDes != NULL );

	UNUSED_ALWAYS(nSrcLen);
	//nSrcLen /= 2;
	
    _ASSERTE( nDesSize >= WideCharToMultiByte( CP_ACP, 0, lpszSrc, nSrcLen, NULL, 0, NULL, NULL) );
	
	return WideCharToMultiByte( CP_ACP, 0, lpszSrc, nSrcLen, lpszDes, nDesSize, NULL, NULL );
}

BOOL CPB::HasUcs2Char(LPCSTR pStr) const
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

BOOL CPB::HasUcs2Char(LPCTSTR pStr) const
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

void CPB::Release(void)
{
	delete this;
}

void CPB::ConvertPBETV1ToV0( MMI_PB_V1::PB_PHONEBOOK_ENTRY_T * pPET1, PB_PHONEBOOK_ENTRY_T & pet0)
{
	pet0.entry_id = pPET1->entry_id;
	pet0.group_id = pPET1->group_id;
	pet0.entry_name.alpha_len = pPET1->entry_name.alpha_len;
	pet0.entry_name.is_ucs2 = pPET1->entry_name.is_ucs2;
	memcpy(pet0.entry_name.alpha,pPET1->entry_name.alpha,sizeof(pet0.entry_name.alpha));

	memcpy(&(pet0.mail),&(pPET1->mail),sizeof(pet0.mail));
	memcpy(pet0.number_t,pPET1->number_t,sizeof(pet0.number_t));
}

BOOL CPB::SetPBParam(DWORD dwFlag,DWORD *pParam )
{
	if(dwFlag == PB_PARAM_VERSION)
	{
		m_dwVersion = *pParam;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

_PB * CPB::_GetPBV1(int nUsedIdx)
{
	_ASSERTE( m_pPB != NULL );
	_ASSERTE( nUsedIdx >= 0 && nUsedIdx < m_nUsedNum );
	
	CUIntArray * pArrID = GetIDArrayPtr(m_wStorePlace);	
	if( pArrID == NULL )
	{
		return NULL;
	}
	UINT nID = pArrID->GetAt(nUsedIdx);
	if(nID == 0)
	{
		m_Log.LogEx(_T("_GetPBV1, param invalid: pb id is zero.\r\n"));
		return NULL;
	}
	
	memset( m_pPB, 0, sizeof(PB) );	

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));
	
	sprintf( m_szOutBuf, MMI_GET_PB, GetMMIUsedStorePlace(m_wStorePlace), nID );
	
    if( !ExecuteCmd(m_szOutBuf) )
	{		
        m_Log.LogEx(_T("_GetPBV1 fail, the cmd is: {%s}.\r\n"), m_szOutBuf);
		return NULL;
	}

	int nPrefixLen = strlen(MMI_PREFIX);
	_ASSERTE( m_dwRead >= nPrefixLen + /*sizeof(uint16)*2 + sizeof(PB_PHONEBOOK_ENTRY_T)*2 +*/ strlen(AT_RES_OK) );
	
	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, MMI_PREFIX, nPrefixLen);
	//_ASSERTE( pFind != NULL );
	if(pFind == NULL)
	{
		return NULL;
	}
	
	pFind += nPrefixLen;
	
	int nLen = sizeof(uint16) + sizeof(PB_PHONEBOOK_ENTRY_T);
	LPBYTE pBContent = new BYTE[nLen];
	memset(pBContent,0,nLen);
	m_convtr.HexCharToBinary(pFind,nLen*2,pBContent,nLen);
	
	uint16 nHITLen = *(uint16*)pBContent;
	
	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		nHITLen= m_convtr.BigEdnToLitEdn((WORD)nHITLen);
	}
	//
	
	//	_ASSERTE( nHITLen == sizeof(PB_PHONEBOOK_ENTRY_T) );
	if(nHITLen != sizeof(PB_PHONEBOOK_ENTRY_T) )
	{
		m_Log.LogEx(_T("_GetPBV1 fail, head length [%d] is invalid, the right len[%d].\r\n"),nHITLen,sizeof(PB_PHONEBOOK_ENTRY_T));
		delete [] pBContent;
		return NULL;
	}
	
	PB_PHONEBOOK_ENTRY_T * pPBET = (PB_PHONEBOOK_ENTRY_T *)(pBContent + sizeof(uint16));
	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		pPBET->entry_id = m_convtr.BigEdnToLitEdn(pPBET->entry_id);
	}

	if(pPBET->entry_name.alpha_len > 0)
	{
		if( pPBET->entry_name.is_ucs2 )
		{
			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				m_convtr.BigEdnToLitEdn( (LPWORD)pPBET->entry_name.alpha, 
				pPBET->entry_name.alpha_len / 2 );
			}
			//
			
			memcpy(m_pPB->szName,pPBET->entry_name.alpha,sizeof(pPBET->entry_name.alpha));
		}
		else
		{					
			AsciiToUcs2((LPSTR)(pPBET->entry_name.alpha),pPBET->entry_name.alpha_len,m_pPB->szName,sizeof(m_pPB->szName)/sizeof(_TCHAR));			
		}
	}
	
	
	if(pPBET->number_t[0].number_len > 0)
		AsciiToUcs2((LPSTR)(pPBET->number_t[0].number),pPBET->number_t[0].number_len,m_pPB->szPhone,sizeof(m_pPB->szPhone)/sizeof(_TCHAR));
	
	
    if(pPBET->number_t[1].number_len > 0)
		AsciiToUcs2((LPSTR)(pPBET->number_t[1].number),pPBET->number_t[1].number_len,m_pPB->szHomePhone,sizeof(m_pPB->szHomePhone)/sizeof(_TCHAR));
	
	
    if(pPBET->number_t[2].number_len > 0)
		AsciiToUcs2((LPSTR)(pPBET->number_t[2].number),pPBET->number_t[2].number_len,m_pPB->szOfficePhone,sizeof(m_pPB->szOfficePhone)/sizeof(_TCHAR));
	
	if(pPBET->mail.mail_len>0)
	{
		if( pPBET->mail.is_ucs2 )
		{
			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				m_convtr.BigEdnToLitEdn( (LPWORD)pPBET->mail.mail, pPBET->mail.mail_len / 2 );
			}
			//
			memcpy(m_pPB->szEmail,pPBET->mail.mail,pPBET->mail.mail_len);
		}
		else
		{						
			AsciiToUcs2((LPSTR)(pPBET->mail.mail),pPBET->mail.mail_len,m_pPB->szEmail,sizeof(m_pPB->szEmail)/sizeof(_TCHAR));
		}
	}
	
	_ASSERTE( nID == pPBET->entry_id );

	BOOL bOK = TRUE;

	if(nID != pPBET->entry_id)
	{
		m_Log.LogEx(_T("_GetPBV1: id not match, [requst id:%d] [return id:%d]\r\n"),nID,pPBET->entry_id);
		bOK = FALSE;
	}
	else
	{
		if(nID == 0)
		{
			m_Log.LogEx(_T("_GetPBV1: id zero, [requst id:%d] [return id:%d]\r\n"),nID,pPBET->entry_id);
			bOK = FALSE;
		}
	}
	
	m_pPB->nID    = nID;
	m_pPB->wStore = m_wStorePlace;
	m_pPB->nGroupIndex = pPBET->group_id;
	
	delete []pBContent;
	pBContent = NULL;

	//CTime tEnd = CTime::GetCurrentTime();
	//CTimeSpan ts = tEnd-tBegin;

	//clock_t cEnd = clock();

	//TRACE(_T("Get PB[%d] spend time [%d]ms\r\n"),nID,/*ts.GetTotalSeconds()*/(cEnd - cBegin));
	if(bOK)
	{
		return m_pPB;
	}
	else
	{
		return NULL;
	}
}


_PB * CPB::_GetPBV2(int nUsedIdx)
{
//	TRACE(_T("m_nUsedNum = 0x%x\r\n"),m_nUsedNum);
	_ASSERTE( m_pPB != NULL );
	_ASSERTE( nUsedIdx >= 0 && nUsedIdx < m_nUsedNum );
	
	CUIntArray * pArrID = GetIDArrayPtr(m_wStorePlace);	
	if( pArrID == NULL )
	{
		return NULL;
	}
	
	int nID = pArrID->GetAt(nUsedIdx);
	if(nID == 0)
	{
		m_Log.LogEx(_T("_GetPBV2, param invalid: pb id is zero.\r\n"));
		return NULL;
	}
	
	memset( m_pPB, 0, sizeof(PB) );
	memset(m_szOutBuf,0,sizeof(m_szOutBuf));
	
	sprintf( m_szOutBuf, MMI_GET_PB, GetMMIUsedStorePlace(m_wStorePlace), nID );
	
    if( !ExecuteCmd(m_szOutBuf) )
	{		
        m_Log.LogEx(_T("GetPB fail, the cmd is: {%s}.\r\n"), m_szOutBuf);
		return NULL;
	}
	
	int nPrefixLen = strlen(MMI_PREFIX);
	_ASSERTE( m_dwRead >= nPrefixLen + /*sizeof(uint16)*2 + sizeof(PB_PHONEBOOK_ENTRY_T)*2 + */strlen(AT_RES_OK) );
	
	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, MMI_PREFIX, nPrefixLen);
	//_ASSERTE( pFind != NULL );
	if(pFind == NULL)
	{
		m_Log.LogEx(_T("_GetPBV2 fail, not find the MMI prefix in the ack string: {{\r\n%s\r\n}}\r\n"),m_szInBuf);
		return NULL;
	}
	
	pFind += nPrefixLen;
	
	int nLen = sizeof(uint16) + sizeof(MMI_PB_V1::PB_PHONEBOOK_ENTRY_T);
	LPBYTE pBContent = new BYTE[nLen];
	memset(pBContent,0,nLen);
	m_convtr.HexCharToBinary(pFind,nLen*2,pBContent,nLen);
	
	uint16 nHITLen = *(uint16*)pBContent;
	
	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		nHITLen= m_convtr.BigEdnToLitEdn((WORD)nHITLen);
	}
	//
	
	//_ASSERTE( nHITLen == sizeof(MMI_PB_V1::PB_PHONEBOOK_ENTRY_T) );
	if(nHITLen != sizeof(MMI_PB_V1::PB_PHONEBOOK_ENTRY_T) )
	{
		m_Log.LogEx(_T("_GetPBV2 fail, head length [%d] is invalid, the right len[%d].\r\n"),nHITLen,sizeof(MMI_PB_V1::PB_PHONEBOOK_ENTRY_T));
		delete [] pBContent;
		return NULL;
	}
	
	MMI_PB_V1::PB_PHONEBOOK_ENTRY_T * pPBET = (MMI_PB_V1::PB_PHONEBOOK_ENTRY_T *)(pBContent + sizeof(uint16));
	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		pPBET->entry_id = m_convtr.BigEdnToLitEdn(pPBET->entry_id);
	}
	//
	_ASSERTE( pPBET->entry_id == pArrID->GetAt(nUsedIdx) );
	if(pPBET->entry_name.alpha_len > 0)
	{
		if( pPBET->entry_name.is_ucs2 )
		{
			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				m_convtr.BigEdnToLitEdn( (LPWORD)pPBET->entry_name.alpha, 
				pPBET->entry_name.alpha_len / 2 );
			}
			//
			memcpy(m_pPB->szName,pPBET->entry_name.alpha,sizeof(pPBET->entry_name.alpha));
		}
		else
		{					
			AsciiToUcs2((LPSTR)(pPBET->entry_name.alpha),pPBET->entry_name.alpha_len,m_pPB->szName,sizeof(m_pPB->szName)/sizeof(_TCHAR));			
		}
	}
	
	
	if(pPBET->number_t[0].number_len > 0)
		AsciiToUcs2((LPSTR)(pPBET->number_t[0].number),pPBET->number_t[0].number_len,m_pPB->szPhone,sizeof(m_pPB->szPhone)/sizeof(_TCHAR));
	
	
    if(pPBET->number_t[1].number_len > 0)
		AsciiToUcs2((LPSTR)(pPBET->number_t[1].number),pPBET->number_t[1].number_len,m_pPB->szHomePhone,sizeof(m_pPB->szHomePhone)/sizeof(_TCHAR));
	
	
    if(pPBET->number_t[2].number_len > 0)
		AsciiToUcs2((LPSTR)(pPBET->number_t[2].number),pPBET->number_t[2].number_len,m_pPB->szOfficePhone,sizeof(m_pPB->szOfficePhone)/sizeof(_TCHAR));
	
	if(pPBET->mail.mail_len>0)
	{
		if( pPBET->mail.is_ucs2 )
		{
			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				m_convtr.BigEdnToLitEdn( (LPWORD)pPBET->mail.mail, pPBET->mail.mail_len / 2 );
			}
			//
			memcpy(m_pPB->szEmail,pPBET->mail.mail,pPBET->mail.mail_len);
		}
		else
		{						
			AsciiToUcs2((LPSTR)(pPBET->mail.mail),pPBET->mail.mail_len,m_pPB->szEmail,sizeof(m_pPB->szEmail)/sizeof(_TCHAR));
		}
	}
	
	_ASSERTE( nID == pPBET->entry_id );

	BOOL bOK = TRUE;

	if(nID != pPBET->entry_id)
	{
		m_Log.LogEx(_T("_GetPBV1: id not match, [requst id:%d] [return id:%d]\r\n"),nID,pPBET->entry_id);
		bOK = FALSE;
	}
	else
	{
		if(nID == 0)
		{
			m_Log.LogEx(_T("_GetPBV1: id is zero, [requst id:%d] [return id:%d]\r\n"),nID,pPBET->entry_id);
			bOK = FALSE;
		}
	}
	
	m_pPB->nID    = nID;
	m_pPB->wStore = m_wStorePlace;
	m_pPB->nGroupIndex = pPBET->group_id;
	
	delete []pBContent;
	pBContent = NULL;

	if(bOK)
	{
		return m_pPB;
	}
	else
	{
		return NULL;
	}
}

void CPB::CompressZero(char *pSrc, int nSrcLen,char * pDstBuf, int &nDstSize)
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

void CPB::UnCompressZero(char *pSrc, int nSrcLen, char * pDstBuf, int &nDstSize)
{
	_ASSERTE(pSrc != NULL && pDstBuf != NULL);

//	char * pZeroStart=NULL;
	
	char * pDstCur = pDstBuf;
	char * pSrcCur = pSrc;

	char szZeroNum[5]={0};
	
	for(int i=0; i<nSrcLen; i++)
	{
		if( (i+1)< nSrcLen && *pSrcCur== 'x' && *(pSrcCur+1) == 'x')
		{			
			memcpy(szZeroNum,pSrcCur+2,4);
			int nZeroNum =0;
			sscanf(szZeroNum,"%04x",&nZeroNum);
			memset(pDstCur,'0',nZeroNum);

			pSrcCur+=6;
			pDstCur+=nZeroNum;
			i+=5;
		}
		else
		{
			*(pDstCur++) = *(pSrcCur++);
		}		
	}

	nDstSize = pDstCur - pDstBuf;
}

BOOL   CPB::ReadAllPB(int &nCount)
{
	UNUSED_ALWAYS(nCount);
	return FALSE;
}	
LPCSTR CPB::GetPBType()
{
	return NULL;
}
BOOL   CPB::SetPBType(DWORD dwFlag)
{
	UNUSED_ALWAYS(dwFlag);
	return FALSE;
}

CUIntArray* CPB::GetIDArrayPtr(WORD wStore)
{
	CUIntArray * pArrID = NULL;
	if( wStore == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( wStore == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	else if( wStore == ID_STORE_SIM2 )
		pArrID = &m_arrSIMID2;
	else if( wStore == ID_STORE_SIM3 )
		pArrID = &m_arrSIMID3;
	else if( wStore == ID_STORE_SIM4 )
		pArrID = &m_arrSIMID4;

	if(pArrID == NULL)
	{
		m_Log.LogEx(_T("GetIDArrayPtr fail, wStore: [%d].\r\n"),wStore);
	}

	return pArrID;
}

void  CPB::AddID(CUIntArray* pIDArray, int nID)
{
	if(NULL == pIDArray)
	{
		return;
	}
	for(int i=0; i< pIDArray->GetSize(); i++)
	{
		if(pIDArray->GetAt(i) > (UINT)nID)
		{
			pIDArray->InsertAt(i,nID);
			return;
		}
	}

	pIDArray->Add(nID);
}