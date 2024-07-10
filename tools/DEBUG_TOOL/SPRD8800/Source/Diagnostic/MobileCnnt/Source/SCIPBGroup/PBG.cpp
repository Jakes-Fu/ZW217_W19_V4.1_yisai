// PBG.cpp: implementation of the CPBG class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SCIPBGroup.h"
#include "PBG.h"
#include "GlobalDef.h"
#include "IPort.h"

#pragma  warning(push,3)
#include <algorithm>
#pragma  warning(pop)


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_RETRY_NUM           10
#define READ_DELAY_TIME         1000

const int    CPBG::MMI_ID_LEN              = 4;
// 命令格式：AT## + 标识符 + 模块名 + 操作类型 + 操作参数 + \r\n
const char   CPBG::MMI_GET_INFO[]           = "AT##MMI60\r\n";
const char   CPBG::MMI_ADD_PBG[]            = "AT##MMI61%s\r\n";
const char   CPBG::MMI_DELETE_PBG[]         = "AT##MMI62%04d\r\n";
const char   CPBG::MMI_MODIFY_PBG[]         = "AT##MMI63%s\r\n";
// 两个%c%c表示改pbg的id
const char   CPBG::MMI_GET_PBG[]            = "AT##MMI64%04d\r\n";

const char   CPBG::MMI_PBG_STORE_SIM        = '1';
const char   CPBG::MMI_PBG_STORE_MS         = '0';

const char   CPBG::MMI_PREFIX[]            = "MMI";
const char   CPBG::MMI_LR[]                = "\r\n";
const char   CPBG::AT_RES_OK[]             = "OK\r\n";
const char   CPBG::AT_RES_ERROR[]          = "ERROR\r\n";
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPBG::CPBG()
{
	m_pPort        = NULL;
	m_pPBG         = NULL;

	m_wStorePlace  = 0x00;
	m_nUsedNum     = 0;

	m_dwRead       = 0;
	memset(m_szInBuf,0,sizeof(m_szInBuf));	
	memset(m_szOutBuf,0,sizeof(m_szOutBuf));		
	memset(m_szTempA,0,sizeof(m_szTempA));		
	memset(m_szTempW,0,sizeof(m_szTempW));	

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

	m_nRetryTimes     = GetPrivateProfileInt( _T("MMIPBG"), _T("RetryTimes"), MAX_RETRY_NUM, szPath );
	m_nReadOneTimeout = GetPrivateProfileInt( _T("MMIPBG"), _T("ReadOneTimeout"), READ_DELAY_TIME, szPath );
}

CPBG::~CPBG()
{
	if( NULL != m_pPBG )
    {
		delete [] m_pPBG;
    }
}

BOOL CPBG::StartLog( LPCTSTR pszFileName )
{ 
    _ASSERTE( pszFileName != NULL );

    return m_Log.Open(pszFileName);
}

void CPBG::StopLog( void )
{
    m_Log.Close();
}

void CPBG::SetPort( IPort * pPort )
{
	_ASSERTE( pPort != NULL );

	m_pPort = pPort;
}

BOOL CPBG::Init(void)
{
	if( NULL == m_pPBG )
	{
		m_pPBG = new PBGROUP;
		if( NULL == m_pPBG )
		{
			TRACE0("!! memory no enough !!\r\n");
            m_Log.LogEx(_T("!! memory no enough !!\r\n"));

			return FALSE;
		}
	}

	return TRUE;
}

BOOL CPBG::ExecuteCmd(LPCSTR lpCmd)
{
	if(!(m_pPort != NULL && m_pPort->IsOpened()) )
		return FALSE;
    _ASSERTE( lpCmd != NULL && strlen(lpCmd) > 0 );

	DWORD dwReadTmp = 0;
	for(int j=0;j< m_nRetryTimes; j++)
	{
		m_pPort->Read( (LPVOID)m_szInBuf, MAX_BUF_LEN, &dwReadTmp, 1);
	}

	memset( m_szInBuf, 0, sizeof(m_szInBuf) );
	m_pPort->PurgeAll();	

	int nDstSize = MAX_BUF_LEN +1 ;
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

__inline LPSTR CPBG::FindStr(LPCSTR pSrc, int nSrcLen, LPCSTR pDes, int nDesLen)
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

BOOL CPBG::SelectStorePlace( WORD wStorePlace )
{
	m_wStorePlace = wStorePlace;

	return TRUE;
}

__inline char CPBG::GetMMIUsedStorePlace(WORD wStorePlace)
{
	switch( wStorePlace )
	{
	case ID_STORE_PHONE:
		return MMI_PBG_STORE_MS;
	case ID_STORE_SIM:
		return MMI_PBG_STORE_SIM;
	default:
		_ASSERTE( 0 );
	}

	return 0;
}

BOOL CPBG::GetPBGNum( int * pUsedNum, int * pTotalNum )
{
	_ASSERTE( m_pPort != NULL);
	if(!m_pPort->IsOpened())
	{
		*pUsedNum = 0;
		*pTotalNum = 0;
		return FALSE;
	}

	_ASSERTE( pUsedNum != NULL && pTotalNum != NULL );

	if( !Init() )
		return FALSE;

	*pUsedNum = *pTotalNum = 0;

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	// command format is "AT##MMI60\r\n"
	sprintf( m_szOutBuf, MMI_GET_INFO/*, GetMMIUsedStorePlace(m_wStorePlace)*/ );
   
    if( !ExecuteCmd(m_szOutBuf) )
	{
		TRACE1("!! GetPBGNum fail, the cmd is: %s !!\r\n", m_szOutBuf);
		return FALSE;
	}

	int nPrefixLen = strlen(MMI_PREFIX);
	_ASSERTE( m_dwRead >= nPrefixLen + sizeof(uint16)*2 + sizeof(GROUP_HEAD_INFO_T)*2 + strlen(AT_RES_OK) );
	
	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, MMI_PREFIX, nPrefixLen);
	_ASSERTE( pFind != NULL );

	pFind += nPrefixLen;
	
	int nLen = sizeof(uint16) + sizeof(GROUP_HEAD_INFO_T);
	LPBYTE pBContent = new BYTE[nLen];
	
	m_convtr.HexCharToBinary(pFind,nLen*2,pBContent,nLen);

	uint16 nHITLen = *(uint16*)pBContent;

	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		nHITLen= m_convtr.BigEdnToLitEdn((WORD)nHITLen);
	}

	if( nHITLen != sizeof(GROUP_HEAD_INFO_T) )
	{
		delete []pBContent;
		return FALSE;
	}
	
	GROUP_HEAD_INFO_T * pGHIT = (GROUP_HEAD_INFO_T *)(pBContent+sizeof(uint16));
	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		*pTotalNum = m_convtr.BigEdnToLitEdn(pGHIT->max_group_num);
		*pUsedNum  = m_convtr.BigEdnToLitEdn(pGHIT->used_num);
	}
	else
	{
		*pTotalNum = pGHIT->max_group_num;
		*pUsedNum  = pGHIT->used_num;
	}
	//
	m_nUsedNum = *pUsedNum;

	// none used pbg 
	if( 0 == m_nUsedNum )
	{
		delete []pBContent;
		pBContent = NULL;
		return TRUE;
	}

	CUIntArray * pArrID = NULL;
	if( m_wStorePlace == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( m_wStorePlace == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	
	_ASSERTE( pArrID != NULL );
	pArrID->SetSize( m_nUsedNum );

	_ASSERTE( *pTotalNum <= MMI_SYNC_GROUP_MAX_NUM );
	uint16 * pAllID = (uint16 * )pGHIT->all_entry_id;

	uint16 u16Value = 0;
	int   nIdx    = 0;
//	int   nID     = 0;
	for( int i = 0; i < MMI_SYNC_GROUP_MAX_NUM; ++i)
	{
		u16Value = *pAllID;
		//teana hu 2010.05.06
		if(m_bBigEndian)
		{
			u16Value = m_convtr.BigEdnToLitEdn((WORD)u16Value);
		}
		//
//		if(u16Value == nID)
//		{
			pArrID->SetAt(nIdx, u16Value);
			nIdx++;			
//		}
		if( nIdx == m_nUsedNum )
		{
//			i = MMI_SYNC_GROUP_MAX_NUM;
			break;
		}
		pAllID++;
	}
   	
	delete []pBContent;
	pBContent = NULL;

	_ASSERTE( nIdx == m_nUsedNum );

	return TRUE;
}

_PBGROUP * CPBG::GetPBG( int nUsedIdx)
{
	_ASSERTE( m_pPBG != NULL );
	_ASSERTE( nUsedIdx >= 0 && nUsedIdx < m_nUsedNum );

	CUIntArray * pArrID = NULL;
	if( m_wStorePlace == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( m_wStorePlace == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	
	_ASSERTE( pArrID != NULL );

	memset( m_pPBG, 0, sizeof(PBGROUP) );
	
	int nID = pArrID->GetAt(nUsedIdx);

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	sprintf( m_szOutBuf, MMI_GET_PBG, /*GetMMIUsedStorePlace(m_wStorePlace),*/ nID );

    if( !ExecuteCmd(m_szOutBuf) )
	{
        m_Log.LogEx(_T("!! GetPBG fail, the cmd is: %s !!\r\n"), m_szOutBuf);
		return NULL;
	}

	int nPrefixLen = strlen(MMI_PREFIX);
	_ASSERTE( m_dwRead >= nPrefixLen + sizeof(uint16)*2 + sizeof(GROUP_ENTRY_T)*2 + strlen(AT_RES_OK) );
	
	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, MMI_PREFIX, nPrefixLen);
	if(pFind == NULL)
	{
		return NULL;
	}

	pFind += nPrefixLen;

	int nLen = sizeof(uint16) + sizeof(GROUP_ENTRY_T);
	LPBYTE pBContent = new BYTE[nLen];
	
	m_convtr.HexCharToBinary(pFind,nLen*2,pBContent,nLen);

	uint16 nHITLen = *(uint16*)pBContent;

	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		nHITLen= m_convtr.BigEdnToLitEdn((WORD)nHITLen);
	}
	//

	if(nHITLen != sizeof(GROUP_ENTRY_T) )
	{
		delete [] pBContent;
		return NULL;
	}
	
	GROUP_ENTRY_T * pGET = (GROUP_ENTRY_T *)(pBContent + sizeof(uint16));
	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		pGET->entry_id = m_convtr.BigEdnToLitEdn(pGET->entry_id);
	}
	//
	_ASSERTE( pGET->entry_id == nID );

	m_pPBG->bFixedGroup = pGET->is_fixed_group;
	//teana hu 2010.05.06
	int nNameLen = 0;
	if(m_bBigEndian)
	{
		m_pPBG->nMemberNum = m_convtr.BigEdnToLitEdn(pGET->member_num);
		nNameLen = m_convtr.BigEdnToLitEdn(pGET->name_len);
	}
	else
	{
		m_pPBG->nMemberNum = pGET->member_num;
		nNameLen = pGET->name_len;
	}
	//
	
	if(nNameLen > MMI_SYNC_GROUP_NAME_MAX_LEN)
	{
		nNameLen = MMI_SYNC_GROUP_NAME_MAX_LEN;
	}

	if(pGET->name_len >0)
	{
		if( pGET->is_ucs2 )
		{
			m_pPBG->bUcs2 = TRUE;
			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				m_convtr.BigEdnToLitEdn( (LPWORD)pGET->group_name, 
								 nNameLen/ 2 );
			}
			//
			//Ucs2ToAscii((LPWORD)pGET->group_name, nNameLen,
			//	m_pPBG->szName, sizeof(m_pPBG->szName) );
			memcpy(m_pPBG->szName,pGET->group_name,sizeof(pGET->group_name));
		}
		else
		{
			m_pPBG->bUcs2 = FALSE;
			memset(m_pPBG->szName, 0, sizeof(m_pPBG->szName));
			//ZeroMemory(m_szTempW,sizeof(m_szTempW));
#ifdef _UNICODE
			AsciiToUcs2((LPSTR)pGET->group_name,nNameLen,m_pPBG->szName,sizeof(m_pPBG->szName)/sizeof(_TCHAR));
#else
			strcpy(m_pPBG->szName, (LPSTR)pGET->group_name);
#endif
			
			//memcpy(m_pPBG->szName, pGET->group_name, sizeof(pGET->group_name));
		}
	}
	
	_ASSERTE( nID == pGET->entry_id );
	
	m_pPBG->nID    = nID;
	m_pPBG->wStore = m_wStorePlace;

	delete [] pBContent;
	pBContent = NULL;

	return m_pPBG;
}

void CPBG::ConvertPBGroupToGET( _PBGROUP * pPBGroup, GROUP_ENTRY_T & pet)
{
	_ASSERTE( pPBGroup != NULL );

	//teana hu 2010.05.06
	if(m_bBigEndian)
	{
		pet.entry_id = m_convtr.LitEdnToBigEdn((WORD)pPBGroup->nID);
		pet.member_num = m_convtr.LitEdnToBigEdn((WORD)pPBGroup->nMemberNum);
	}
	else
	{
		pet.entry_id = (uint16)pPBGroup->nID;
		pet.member_num = (uint16)pPBGroup->nMemberNum;
	}
   
	pet.is_fixed_group = (BYTE)(pPBGroup->bFixedGroup);
    if(_tcslen(pPBGroup->szName)>0)
	{
		if( HasUcs2Char(pPBGroup->szName) )
		{
			pet.is_ucs2   = TRUE;
			//int nLen = AsciiToUcs2(pPBGroup->szName, (WCHAR *)pet.group_name, MMI_SYNC_GROUP_NAME_MAX_LEN);
			int nLen = _tcslen(pPBGroup->szName) +1; //inlcude the null-terminated
			_tcscpy((LPTSTR)pet.group_name,pPBGroup->szName);
			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				pet.name_len = m_convtr.LitEdnToBigEdn((WORD)((nLen - 1) * 2));
				m_convtr.LitEdnToBigEdn( (LPWORD)pet.group_name, nLen );
			}
			else
			{
				pet.name_len = (uint16)((nLen - 1) * 2);
			}
			//
		}
		else
		{
			pet.is_ucs2   = FALSE;
			ZeroMemory(m_szTempA,sizeof(m_szTempA));
#ifdef _UNICODE
			Ucs2ToAscii(pPBGroup->szName,_tcslen(pPBGroup->szName),m_szTempA,MAX_BUF_LEN + 1);
#else
			strcpy(m_szTempA, pPBGroup->szName);
#endif

			//teana hu 2010.05.06
			if(m_bBigEndian)
			{
				pet.name_len = m_convtr.LitEdnToBigEdn((WORD)strlen(m_szTempA));
			}
			else
			{
				pet.name_len = (uint16)strlen(m_szTempA);
			}
			//
			strcpy( (LPSTR)pet.group_name, m_szTempA );
		}
	}

}

BOOL CPBG::AddPBG( _PBGROUP * pPBGroup )
{
	_ASSERTE( pPBGroup != NULL );
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );

    pPBGroup->nID = GetUnusedID(pPBGroup->wStore);

	GROUP_ENTRY_T pet = { 0 };

	ConvertPBGroupToGET(pPBGroup, pet);	

	int  nDesSize = MAX_BUF_LEN;
	char szHexStr[ MAX_BUF_LEN ] = { 0 };

	m_convtr.BinaryToHexChar((LPBYTE)&pet, sizeof(pet), szHexStr, nDesSize);

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	sprintf( m_szOutBuf, MMI_ADD_PBG, /*GetMMIUsedStorePlace(pPBGroup->wStore),*/ szHexStr );
	if( !ExecuteCmd(m_szOutBuf) )
	{
        m_Log.LogEx(_T("!! AddPb fail !!\r\n"));
		return FALSE;
	}

	CUIntArray * pArrID = NULL;
	if( pPBGroup->wStore == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( pPBGroup->wStore == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	
	_ASSERTE( pArrID != NULL );

	pArrID->Add(pPBGroup->nID);

	return TRUE;
}

BOOL CPBG::ModifyPBG( _PBGROUP * pPBGroup )
{
	_ASSERTE( pPBGroup != NULL );
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );

	GROUP_ENTRY_T pet = { 0 };
   
	ConvertPBGroupToGET(pPBGroup, pet);	

	int  nDesSize = MAX_BUF_LEN;
	char szHexStr[ MAX_BUF_LEN ] = { 0 };

	m_convtr.BinaryToHexChar((LPBYTE)&pet, sizeof(pet), szHexStr, nDesSize);

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	sprintf(m_szOutBuf, MMI_MODIFY_PBG, /*GetMMIUsedStorePlace(pPB->wStore),*/ szHexStr);
	if( !ExecuteCmd(m_szOutBuf) )
	{
        m_Log.LogEx(_T("!! ModifyPB fail !!\r\n"));
		return FALSE;
	}

	return TRUE;
}

BOOL CPBG::DeletePBG( _PBGROUP * pPBGroup )
{
	_ASSERTE( pPBGroup != NULL );
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	sprintf( m_szOutBuf, MMI_DELETE_PBG,/* GetMMIUsedStorePlace(pPB->wStore),*/ pPBGroup->nID );
	if( !ExecuteCmd(m_szOutBuf) )
	{
        m_Log.LogEx(_T("!! ModifyPB fail !!\r\n"));
		return FALSE;
	}

	CUIntArray * pArrID = NULL;
	if( pPBGroup->wStore == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( pPBGroup->wStore == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	
	_ASSERTE( pArrID != NULL );
	int nCount = pArrID->GetSize();
	for( int i = 0; i < nCount; ++i )
	{
		if( pArrID->GetAt(i) == (unsigned int)(pPBGroup->nID) )
		{
			pArrID->RemoveAt(i);
			break;
		}
	}

	return TRUE;
}

__inline int CPBG::GetUnusedID(WORD wStore)
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

__inline int CPBG::AsciiToUcs2( LPCSTR lpszSrc,  int nSrcLen, WCHAR * lpszDes, int nDesSize )
{
	_ASSERTE( lpszSrc != NULL && lpszDes != NULL );
    _ASSERTE( nDesSize >= MultiByteToWideChar( CP_ACP, 0, lpszSrc, nSrcLen, NULL, 0) );

	return MultiByteToWideChar( CP_ACP, 0, lpszSrc, nSrcLen, lpszDes, nDesSize);
}

__inline int CPBG::Ucs2ToAscii( LPCWSTR lpszSrc, int nSrcLen, LPSTR lpszDes, int nDesSize )
{
	_ASSERTE( nDesSize >= nSrcLen);
	_ASSERTE( lpszSrc != NULL && lpszDes != NULL );
    
	UNUSED_ALWAYS(nSrcLen);
	
	//nSrcLen /= 2;
	
    _ASSERTE( nDesSize >= WideCharToMultiByte( CP_ACP, 0, lpszSrc, nSrcLen, NULL, 0, NULL, NULL) );
	
	return WideCharToMultiByte( CP_ACP, 0, lpszSrc, nSrcLen, lpszDes, nDesSize, NULL, NULL );
}
#ifndef _UNICODE
BOOL CPBG::HasUcs2Char(LPCSTR pStr) const
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
#else
BOOL CPBG::HasUcs2Char(LPCWSTR pStr) const
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
#endif
void CPBG::Release(void)
{
	delete this;
}


void CPBG::CompressZero(char *pSrc, int nSrcLen,char * pDstBuf, int &nDstSize)
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

void CPBG::UnCompressZero(char *pSrc, int nSrcLen, char * pDstBuf, int &nDstSize)
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


BOOL CPBG::SetPBGParam(DWORD dwFlag,DWORD *pParam )
{
	if(dwFlag == PBG_PARAM_VERSION)
	{
		m_dwVersion = *pParam;
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CPBG::GetPBGParam(DWORD dwFlag,DWORD **ppParam )
{
	UNUSED_ALWAYS(dwFlag);
	UNUSED_ALWAYS(ppParam);
	return FALSE;
}
