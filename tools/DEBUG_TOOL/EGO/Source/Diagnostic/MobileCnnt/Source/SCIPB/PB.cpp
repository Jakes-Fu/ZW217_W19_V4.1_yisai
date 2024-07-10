// PB.cpp: implementation of the CPB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCIPB.h"
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

#define ADD_PB 0
#define MDY_PB 1
#define DEL_PB 2

#define MAX_RETRY_NUM     10
#define READ_DELAY_TIME   450

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

const char   CPB::MMI_PREFIX[]            = "MMI";
const char   CPB::MMI_LR[]                = "\r\n";
const char   CPB::AT_RES_OK[]             = "OK\r\n";
const char   CPB::AT_RES_ERROR[]          = "ERROR";

//////////////////////////////////////////////////////////////////////
const char  CPB::AT_SET_STORE_PLACE_SM[]	= "AT+CPBS=\"SM\"\r\n";   //return "OK"
const char  CPB::AT_SET_STORE_PLACE_ME[]	= "AT+CPBS=\"ME\"\r\n";   //return "OK"
const char  CPB::AT_GET_PB_NUM[]			= "AT+CPBS?\r\n";         //return "+CPBS:<storage>,<num used>,<num available>"
const char  CPB::AT_GET_PB_INFO[]			= "AT+CPBR=?\r\n";        //return "+CPBR:<list supported <index>s>,<PB max length>,<name max length>"
const char  CPB::AT_GET_PB_NUM_RET_PR[]		= "+CPBS:";
const char  CPB::AT_GET_PB_INFO_RET_PRE[]	= "+CPBR:"; 
//first %d: start index, second %d: end index
const char CPB::AT_READ_ALL_PB[]            = "AT+CPBR=%d,%d\r\n";
//%d: PB index, 
const char CPB::AT_READ_ONE_PB[]			= "AT+CPBR=%d\r\n";
const char CPB::AT_READ_PB_RET_PRE[]	    = "+CPBR:";

const char CPB::AT_READ_FIRST[]             = "AT+CPBN=0\r\n";
const char CPB::AT_READ_NEXT[]              = "AT+CPBN=2\r\n";
const char CPB::AT_READ_NEXT_RET_PRE[]      = "+CPBN:";
//%d: PB index,%s: number, %s:name
const char CPB::AT_WRITE_PB[]				= "AT+CPBW=%d,\"%s\",129,\"%s\"\r\n";
//%d: PB index
const char CPB::AT_DELETE_PB[]				= "AT+CPBW=%d\r\n";

const char CPB::AT_SET_PB_TYPE[]			= "AT+CPBS=\"%s\"\r\n";

const char CPB::AT_GET_CARD_MODE[]          = "AT^CARDMODE\r\n";
const char CPB::AT_GET_CARD_MODE_RET_PRE[]  = "^CARDMODE:";

const char CPB::AT_READ_PB_EX[]				= "AT^SCPBR=%d\r\n";
const char CPB::AT_READ_PB_EX_RET_PRE[]		= "^SCPBR:";

const char CPB::AT_WRITE_PB_EX[]			= "AT^SCPBW=%d,\"%s\",129,\"%s\",129,\"%s\",129,\"%s\",129,\"%s\",%d,\"%s\"\r\n";

const char CPB::AT_DELETE_PB_EX[]			= "AT^SCPBW=%d\r\n";

const char CPB::AT_GET_PB_USED_ID_EX[]      = "AT^SCPBR=?\r\n";


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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPB::CPB()
{
	m_pPort       = NULL;
	m_pPB         = NULL;

	m_wStorePlace = 0x00;
	m_nUsedNum    = 0;
	m_nTotalNum   = 0;

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

	m_bUSIM = FALSE;

	_TCHAR szPath[_MAX_PATH] = {0};
	::GetModuleFileName( NULL, szPath, _MAX_PATH );
	 LPTSTR pDir = _tcsrchr(szPath, _T('\\'));
    _tcscpy(pDir + 1, _T("Timeout.ini"));

	m_nRetryTimes		= GetPrivateProfileInt( _T("PB"), _T("RetryTimes"), MAX_RETRY_NUM, szPath );
	m_nReadOneTimeout	= GetPrivateProfileInt( _T("PB"), _T("ReadOneTimeout"), READ_DELAY_TIME, szPath );
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

    return m_Log.Open(pszFileName);
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
            m_Log.LogEx(_T("!! memory no enough !!\r\n"));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CPB::ExecuteCmd(LPCSTR lpCmd)
{
	if(!(m_pPort != NULL && m_pPort->IsOpened()) )
		return FALSE;
    _ASSERTE( lpCmd != NULL && strlen(lpCmd) > 0 );
	
	//teana hu 2009.03.06
	if(lpCmd == NULL || strlen(lpCmd) == 0)
	{
		return FALSE;
	}
	
// 	DWORD dwReadTmp = 0;
// 	for(int j=0;j< m_nRetryTimes; j++)
// 	{
// 		m_pPort->Read( (LPVOID)m_szInBuf, MAX_BUF_LEN, &dwReadTmp, 1);
// 	}

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
		return FALSE;

    LPSTR pBuf    = m_szInBuf;
    int   nBufLen = MAX_BUF_LEN;
	DWORD dwRead  = 0;
    m_dwRead      = 0;
	clock_t cStart1 = clock();
	for( int i = 0; nBufLen > 0 && i < m_nRetryTimes && m_pPort->IsOpened(); ++i )
	{
		if( m_pPort->Read( (LPVOID)pBuf, nBufLen, &dwRead, m_nReadOneTimeout) && dwRead > 0)
		{
            --i;
			m_dwRead += dwRead;
			
			if( /*FindStr(m_szInBuf, m_dwRead, MMI_PREFIX, strlen(MMI_PREFIX)) != NULL &&*/
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
		//for debug
		//if(i==2)
		//return TRUE;
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
	m_wStorePlace = wStorePlace;

	
	switch( wStorePlace )
	{
	case ID_STORE_PHONE:		
		return ExecuteCmd(AT_SET_STORE_PLACE_ME);	
	case ID_STORE_SIM:
		return ExecuteCmd(AT_SET_STORE_PLACE_SM);
	case ID_STORE_SIM2:		
	default:
		_ASSERTE( 0 );
		return FALSE;
	}

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
	default:
		_ASSERTE( 0 );
	}

	return 0;
}

BOOL CPB::GetPBNum( int * pUsedNum, int * pTotalNum , BOOL bClear)
{
	_ASSERTE( pUsedNum != NULL && pTotalNum != NULL );
	_ASSERTE( m_pPort != NULL);

	//teana hu 2009.03.06
	if(pUsedNum == NULL || pTotalNum == NULL || m_pPort == NULL)
	{
		return FALSE;
	}

	if(!m_pPort->IsOpened())
	{
		*pUsedNum = 0;
		*pTotalNum = 0;
		return FALSE;
	}

	if( !Init() )
		return FALSE;

	*pUsedNum = *pTotalNum = 0;
	
	memset( m_szOutBuf, 0, sizeof(m_szOutBuf) );
	memset( m_szInBuf, 0 , sizeof(m_szInBuf));

	sprintf( m_szOutBuf, AT_GET_PB_NUM );
   
    if( !ExecuteCmd(m_szOutBuf) )
	{
		TRACE1("!! GetPBNum fail, the cmd is: %s !!\r\n", m_szOutBuf);
		return FALSE;
	}

	int nPrefixLen = strlen(AT_GET_PB_NUM_RET_PR);
	_ASSERTE( m_dwRead > nPrefixLen + strlen(AT_RES_OK) );

	//teana hu 2009.03.06
	if(m_dwRead <= nPrefixLen + strlen(AT_RES_OK))
	{
		return FALSE;
	}
	
	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, AT_GET_PB_NUM_RET_PR, nPrefixLen);
	_ASSERTE( pFind != NULL );

	//teana hu 2009.03.06
	if(pFind == NULL)
	{
		return FALSE;
	}

	pFind += nPrefixLen;

	LPSTR pFindNum = FindStr(pFind, MAX_BUF_LEN -(pFind - m_szInBuf), ",", 1);
	pFindNum+=1;

	LPSTR pFindRet = FindStr(pFindNum, MAX_BUF_LEN -(pFindNum - m_szInBuf), "\r\n", 2);

	*pFindRet = '\0';

	int nUsedNum= 0;
	//int nAvailNum= 0;

	sscanf(pFindNum,"%d,%d",&nUsedNum,&m_nTotalNum);
	

	*pTotalNum = m_nTotalNum;// + nAvailNum;
	*pUsedNum  = nUsedNum;
	m_nUsedNum = nUsedNum;
	//m_nTotalNum = nUsedNum + nAvailNum;
//debug 
//	if(m_wStorePlace == ID_STORE_SIM)
//	{
//		*pUsedNum = 4;
//		m_nUsedNum = 4;
//	}

	CUIntArray * pArrID = NULL;
	if(bClear)
	{
		switch( m_wStorePlace )
		{
		case ID_STORE_PHONE:
			m_arrMSID.RemoveAll();
			m_arrMSID.SetSize(m_nTotalNum);
			pArrID = &m_arrMSID;
			break;
		case ID_STORE_SIM:
			m_arrSIMID.RemoveAll();
			m_arrSIMID.SetSize(m_nTotalNum);
			pArrID = &m_arrSIMID;
			break;
		case ID_STORE_SIM2:
			m_arrSIMID2.RemoveAll();
			m_arrSIMID2.SetSize(m_nTotalNum);
			pArrID = &m_arrSIMID2;
			break;
		default:
			_ASSERTE( 0 );
		}

		if(pArrID != NULL)
		{
			for(int i = 0; i< m_nTotalNum;i++)
			{
				pArrID->SetAt(i,0);
			}
		}
	}
	
	if(IsUSIM())
	{
		//return GetUsedIDEx();		
	}

	return TRUE;
}

LPCSTR CPB::GetPBType()
{
	sprintf( m_szOutBuf, AT_GET_PB_NUM );
   
    if( !ExecuteCmd(m_szOutBuf) )
	{
		return NULL;
	}

	return m_szInBuf;
}

BOOL CPB::SetPBType(DWORD dwFlag)
{
	switch(dwFlag) {
	case 0:
		sprintf(m_szOutBuf, AT_SET_PB_TYPE, "SM");
		break;
	case 1:
		sprintf(m_szOutBuf, AT_SET_PB_TYPE, "FD");
		break;
	case 2:
		sprintf(m_szOutBuf, AT_SET_PB_TYPE, "LD");
		break;
	case 3:
		sprintf(m_szOutBuf, AT_SET_PB_TYPE, "MC");
		break;
	case 4:
		sprintf(m_szOutBuf, AT_SET_PB_TYPE, "RC");
		break;
	case 5:
		sprintf(m_szOutBuf, AT_SET_PB_TYPE, "ME");
		break;
	default:
		break;
	}

	if( !ExecuteCmd(m_szOutBuf) )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CPB::GetPBParam(DWORD dwFlag,DWORD *ppParam )
{
	_ASSERTE( m_pPort != NULL);
	_ASSERTE( ppParam != NULL);

	//teana hu 2009.03.06
	if(m_pPort == NULL || ppParam == NULL)
	{
		return FALSE;
	}

	if(!m_pPort->IsOpened())
	{
		return FALSE;		
	}

	if(dwFlag == PB_PARAM_IS_USIM)
	{
		BOOL bOK = IsUSIM();
		*ppParam = bOK;

		return TRUE;
	}

	if( dwFlag != PB_PARAM_NAME_LEN )
	{
		return FALSE;
	}

	if( !Init() )
		return FALSE;
	
	memset( m_szInBuf, 0, sizeof(m_szOutBuf) );

	sprintf( m_szOutBuf, AT_GET_PB_INFO);
   
    if( !ExecuteCmd(m_szOutBuf) )
	{
		TRACE1("!! GetPBParam fail, the cmd is: %s !!\r\n", m_szOutBuf);
		return FALSE;
	}

	int nPrefixLen = strlen(AT_GET_PB_INFO_RET_PRE);
	_ASSERTE( m_dwRead > nPrefixLen + strlen(AT_RES_OK) );

	//teana hu 2009.03.06
	if(m_dwRead <= nPrefixLen + strlen(AT_RES_OK))
	{
		return FALSE;
	}
	
	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, AT_GET_PB_INFO_RET_PRE, nPrefixLen);
	_ASSERTE( pFind != NULL );

	//teana hu 2009.03.06
	if(pFind == NULL)
	{
		return FALSE;
	}

	pFind += nPrefixLen;

	LPSTR pFindNum = FindStr(pFind, MAX_BUF_LEN -(pFind - m_szInBuf), ",", 1);
	pFindNum+=1;

	LPSTR pFindRet = FindStr(pFindNum, MAX_BUF_LEN -(pFindNum - m_szInBuf), "\r\n", 2);

	*pFindRet = '\0';

	int nNumLen= 0;
	int nNameLen= 0;

	sscanf(pFindNum,"%d,%d",&nNumLen,&nNameLen);
	
	(ppParam)[0] = nNameLen;
	(ppParam)[1] = MAX_EMAIL_LEN;
	(ppParam)[2] = nNumLen;
	
	return TRUE;
}


_PB * CPB::GetPB( int nUsedIdx)
{
	if(m_bUSIM)
	{
		return GetPBEx(nUsedIdx);
	}
	else
	{
		return GetPBStd(nUsedIdx);
	}
}

void CPB::ConvertPBToPBET( _PB * pPB, MMI_PB_V1::PB_PHONEBOOK_ENTRY_T & pet)
{
	_ASSERTE( pPB != NULL );

	pet.entry_id = m_convtr.LitEdnToBigEdn((WORD)pPB->nID);
	pet.group_id = (BYTE)(pPB->nGroupIndex);
    if(_tcslen(pPB->szName)>0)
	{
		if( HasUcs2Char(pPB->szName) )
		{
			pet.entry_name.is_ucs2   = TRUE;
#ifdef _UNICODE
			int nLen = _tcslen(pPB->szName) +1; //inlcude the null-terminated
			_tcscpy((LPTSTR)pet.entry_name.alpha,pPB->szName);
#else
			MultiByteToWideChar(CP_ACP,0,pPB->szName,-1,(LPWSTR)pet.entry_name.alpha,strlen(pPB->szName)+1);
			int nLen = wcslen((LPWSTR)pet.entry_name.alpha) * 2;
#endif
			//int nLen = AsciiToUcs2(pPB->szName, (WCHAR *)pet.entry_name.alpha, MMI_SYNC_PB_NAME_MAX_LEN);
			
			pet.entry_name.alpha_len = (BYTE)((nLen - 1) * 2);
			m_convtr.LitEdnToBigEdn( (LPWORD)pet.entry_name.alpha, nLen );
		}
		else
		{
			pet.entry_name.is_ucs2   = FALSE;
			pet.entry_name.alpha_len = (BYTE)(_tcslen(pPB->szName));
			//strcpy( (LPSTR)pet.entry_name.alpha, pPB->szName );
#ifdef _UNICODE
			Ucs2ToAscii(pPB->szName,_tcslen(pPB->szName),(LPSTR)(pet.entry_name.alpha),sizeof(pet.entry_name.alpha));
#else
			strcpy((LPSTR)(pet.entry_name.alpha), pPB->szName);
#endif
			
		}
	}

	pet.number_t[0].number_len = (BYTE)(_tcslen(pPB->szPhone));
	//strcpy( (LPSTR)pet.number_t[0].number, pPB->szPhone);
	if(pet.number_t[0].number_len > 0)
	{
#ifdef _UNICODE
		Ucs2ToAscii(pPB->szPhone,_tcslen(pPB->szPhone),(LPSTR)(pet.number_t[0].number),sizeof(pet.number_t[0].number));
#else
		strcpy((LPSTR)(pet.number_t[0].number), pPB->szPhone);
#endif
	}
	
	pet.number_t[1].number_len = (BYTE)(_tcslen(pPB->szHomePhone));
	//strcpy( (LPSTR)pet.number_t[1].number, pPB->szHomePhone);
	if(pet.number_t[1].number_len > 0)
	{
#ifdef _UNICODE
		Ucs2ToAscii(pPB->szHomePhone,_tcslen(pPB->szPhone),(LPSTR)(pet.number_t[1].number),sizeof(pet.number_t[1].number));
#else
		strcpy((LPSTR)(pet.number_t[1].number), pPB->szHomePhone);
#endif
	}
	

	pet.number_t[2].number_len = (BYTE)(_tcslen(pPB->szOfficePhone));
	//strcpy( (LPSTR)pet.number_t[2].number, pPB->szOfficePhone);
	if(pet.number_t[2].number_len > 0)
	{
#ifdef _UNICODE
		Ucs2ToAscii(pPB->szOfficePhone,_tcslen(pPB->szPhone),(LPSTR)(pet.number_t[2].number),sizeof(pet.number_t[2].number));
#else
		strcpy((LPSTR)(pet.number_t[2].number), pPB->szOfficePhone);
#endif
	}
	
	if(_tcslen(pPB->szEmail)>0)
	{
		if( HasUcs2Char(pPB->szEmail) )
		{
			pet.mail.is_ucs2   = TRUE;
			//int nLen = AsciiToUcs2(pPB->szEmail, (WCHAR *)pet.mail.mail, MMI_SYNC_PB_MAX_MAIL_LEN);
			int nLen = _tcslen(pPB->szName) +1; //inlcude the null-terminated
			_tcscpy((LPTSTR)pet.mail.mail,pPB->szEmail);		
			pet.mail.mail_len = (BYTE)((nLen - 1) * 2);
			m_convtr.LitEdnToBigEdn( (LPWORD)pet.mail.mail, nLen );
		}
		else
		{
			pet.mail.is_ucs2   = FALSE;
			pet.mail.mail_len = (BYTE)(_tcslen(pPB->szEmail));
			//strcpy( (LPSTR)pet.mail.mail, pPB->szEmail );	
#ifdef _UNICODE
			Ucs2ToAscii(pPB->szEmail,_tcslen(pPB->szEmail),(LPSTR)(pet.mail.mail),sizeof(pet.mail.mail));
#else
			strcpy((LPSTR)(pet.mail.mail), pPB->szEmail);
#endif
		}
	}
}

BOOL CPB::AddPB( _PB * pPB )
{
	if(m_bUSIM)
	{
		return AddPBEx(pPB);
	}
	else
	{
		return AddPBStd(pPB);
	}	
}

BOOL CPB::ModifyPB( _PB * pPB )
{
	if(m_bUSIM)
	{
		return ModifyPBEx(pPB);
	}
	else
	{
		return ModifyPBStd(pPB);
	}	
}

BOOL CPB::DeletePB( _PB * pPB )
{
	if(m_bUSIM)
	{
		return DeletePBEx(pPB);
	}
	else
	{
		return DeletePBStd(pPB);
	}	
}

__inline int CPB::GetUnusedID(CUIntArray * pArrID)
{
	int nUnusedID = 1;
	int nCount = pArrID->GetSize();
	for( int i = 0; i < nCount; ++i )
	{
		if( pArrID->GetAt(i) != (unsigned int)nUnusedID )
		{
			break;
		}
		else
		{
			nUnusedID++;
		}
	}

	return nUnusedID;
}

__inline int CPB::GetNextUsedID(CUIntArray * pArrID)
{	
	
	int nCount = pArrID->GetSize();
	int nUsedID = nCount;

	for( int i = (nCount-1); i >= 0; i-- )
	{
		if( ((unsigned int)nUsedID) == pArrID->GetAt(i) )
		{
			nUsedID++;
			break;
		}
		else
		{
			nUsedID--;
		}
	}
	if(nUsedID<=0 || nUsedID > nCount)
	{
		nUsedID =  1;
	}

	return nUsedID;

}

__inline int CPB::AsciiToUcs2( LPCSTR lpszSrc, WCHAR * lpszDes, int nDesSize )
{
	_ASSERTE( lpszSrc != NULL && lpszDes != NULL );
    _ASSERTE( nDesSize >= MultiByteToWideChar( CP_ACP, 0, lpszSrc, -1, NULL, 0) );

	return MultiByteToWideChar( CP_ACP, 0, lpszSrc, -1, lpszDes, nDesSize);
}

__inline int CPB::Ucs2ToAscii( LPCWSTR lpszSrc, int nSrcLen, LPSTR lpszDes, int nDesSize )
{
	_ASSERTE( nDesSize >= nSrcLen );
	_ASSERTE( lpszSrc != NULL && lpszDes != NULL );

	UNUSED_ALWAYS(nSrcLen);
	//nSrcLen /= 2;
	
    _ASSERTE( nDesSize >= WideCharToMultiByte( CP_ACP, 0, lpszSrc, -1, NULL, 0, NULL, NULL) );
	
	return WideCharToMultiByte( CP_ACP, 0, lpszSrc, -1, lpszDes, nDesSize, NULL, NULL );
}
#ifndef _UNICODE
BOOL CPB::HasUcs2Char(LPCSTR pStr) const
{
	_ASSERTE( pStr != NULL );

	//teana hu 2009.03.06
	if(pStr == NULL)
	{
		return FALSE;
	}

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
BOOL CPB::HasUcs2Char(LPCTSTR pStr) const
{
	_ASSERTE( pStr != NULL );
	//teana hu 2009.03.06
	if(pStr == NULL)
	{
		return FALSE;
	}
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

	char * pZeroStart=NULL;
	
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

BOOL CPB::ReadAllPB(int &nCount)
{
	UNREFERENCED_PARAMETER(nCount);

	CUIntArray * pArrID = NULL;
	if( m_wStorePlace == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( m_wStorePlace == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	else if( m_wStorePlace == ID_STORE_SIM2 )
		pArrID = &m_arrSIMID2;
	
	_ASSERTE( pArrID != NULL );
	//teana hu 2009.03.06
	if(pArrID == NULL)
	{
		return FALSE;
	}
	pArrID->SetSize( m_nUsedNum );

	/*

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
			if(m_pPort->Read(pInBuf, nRcvSize, &dwRead, m_nReadOneTimeout ) )
			{
				--i;
				
				dwTotal += dwRead;
				TRACE(_T("++++++++++++++read buffer:%d\n"),dwRead);
				
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
				
				if( FindStr(pInBuf, dwTotal, AT_RES_OK, strlen(AT_RES_OK)) != NULL )
				{
					bRet      = TRUE;
					nRcvSize -= dwRead;
					break;
				}
				else if(FindStr(pInBuf, dwTotal, "ERROR", strlen("ERROR")) != NULL ||
					FindStr(pInBuf, dwTotal, "Error", strlen("Error")) != NULL )
				{
					bRet    = FALSE;
					break;
				}
				TRACE(_T("++++++++++++++[buf:%d]read buffer:%d\n"),nRcvBufSize,dwTotal);
				
				pInBuf   += dwRead;
				nRcvSize -= dwRead;
			}
			else
			{
				bRet = FALSE;
				break;
			}
		}
		else
		{			
			bRet = FALSE;
			break;
		}
		
	}

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
            ParseAllSMS_TXT(pRcvBuf, nRcvBufSize - nRcvSize);
        }
        else if( SMS_MODE_PDU == m_eSMSMode )
        {
            ParseAllSMS_PDU(pRcvBuf, nRcvBufSize - nRcvSize);    
        }
        
        nCount = m_nUsedCount;
	}
	else
	{		
        m_Log.LogEx(_T("!! ReadAllSMS Fail !!\r\n"));
        m_dwErrCode = SMS_E_READALLSMS;
	}	*/
	return TRUE;
}

BOOL CPB::IsUSIM()
{  
 	sprintf( m_szOutBuf, AT_GET_CARD_MODE); 

    if( !ExecuteCmd(m_szOutBuf) )
 	{		
        m_Log.LogEx(_T("!! Get card mode fail, the cmd is: %s !!\r\n"), m_szOutBuf);
 		return FALSE;
 	}
 
 	int nPrefixLen = strlen(AT_GET_CARD_MODE_RET_PRE);
 
 	
 	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, AT_GET_CARD_MODE_RET_PRE, nPrefixLen);
 
 	if(pFind == NULL)
 		return FALSE;
 
 	pFind += nPrefixLen;
 
	LPSTR pFindRet = FindStr(pFind, MAX_BUF_LEN-(pFind - m_szInBuf), "\r\n", 2);
	*pFindRet = '\0';

	int nMode;
	sscanf(pFind,"%d",&nMode);
	
	if(nMode == 2)
	{
		m_bUSIM = TRUE;
		return TRUE;
	}
	else
	{
		m_bUSIM = FALSE;
		return FALSE;
	}
}

_PB *  CPB::GetPBStd( int nUsedIdx)
{
	TRACE(_T("m_nUsedNum = 0x%x\r\n"),m_nUsedNum);
 	if( nUsedIdx < 0 && nUsedIdx >= m_nUsedNum )
	{
		return NULL;
	}
	

	Init();
 
	CUIntArray * pArrID = GetIDArray(m_wStorePlace);
	//teana hu 2009.03.06
	if(pArrID == NULL)
	{
		return NULL;
	}
 
 	memset( m_pPB, 0, sizeof(PB) ); 

	if(nUsedIdx == 0) 
 	    sprintf( m_szOutBuf, AT_READ_FIRST);
	else
		sprintf( m_szOutBuf, AT_READ_NEXT);
 
     if( !ExecuteCmd(m_szOutBuf) )
 	{		
         m_Log.LogEx(_T("!! GetPB fail, the cmd is: %s !!\r\n"), m_szOutBuf);
 		return NULL;
 	}
 
 	int nPrefixLen = strlen(AT_READ_NEXT_RET_PRE);
 
 	
 	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, AT_READ_NEXT_RET_PRE, nPrefixLen);
 
 	if(pFind == NULL)
 		return NULL;
 
 	pFind += nPrefixLen;
 
	LPSTR pFindRet = FindStr(pFind, MAX_BUF_LEN-(pFind - m_szInBuf), "\r\n", 2);
	*pFindRet = '\0';

	int nIDtmp = 0;
	int nType = 129;

    pFindRet = strchr(pFind,',');
	*pFindRet = '\0';


	sscanf(pFind,"%d",&nIDtmp);

	pFind = pFindRet + 1;
	pFindRet = strchr(pFind,',');
	*pFindRet = '\0';
	*(pFindRet-1) = '\0';
#ifndef _UNICODE
	sscanf(pFind+1,"%s",m_pPB->szPhone);
#else
	sscanf(pFind+1,"%s",m_szTempA);
	MultiByteToWideChar(CP_ACP, 0, m_szTempA, -1, m_szTempW, MAX_BUF_LEN + 1);
	_tcscpy(m_pPB->szPhone, m_szTempW);
	
#endif

	pFind = pFindRet + 1;
	pFindRet = strchr(pFind,',');

	pFind = pFindRet + 2;
	pFindRet = strchr(pFind,'\"');
	*pFindRet = '\0';
#ifndef _UNICODE	
	sscanf(pFind,"%s",m_pPB->szName);	
	//teana
	memset(m_szTempA, 0, MAX_BUF_LEN + 1);

	if(_tcslen(m_pPB->szName) > 2 && m_pPB->szName[0] == '8' && m_pPB->szName[1] == '0')
	{
		_tcscpy(m_szTempA, m_pPB->szName + 2);

		int nTempLen = wcslen((LPCWSTR)m_szTempA);

		if(nTempLen != 0)
		{
			int nLen = MAX_BUF_LEN + 1;
			BYTE btContent[MAX_BUF_LEN + 1] = { 0 };
			m_converter.HexCharToBinary(m_szTempA, nTempLen*2, btContent, nLen);	
			m_converter.BigEdnToLitEdn((LPWORD)btContent, nLen/2);
			WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)btContent, -1, (char *)m_pPB->szName, MAX_PERSON_NAME_LEN + 1, NULL, NULL);
		}		
	}	

#else
	memset(m_szTempA, 0, MAX_BUF_LEN + 1);
	sscanf(pFind,"%s",m_szTempA);

	if(strlen(m_szTempA) > 2 && m_szTempA[0] == '8' && m_szTempA[1] == '0')
	{
		char szTemp [MAX_BUF_LEN + 1] = {0};
		strcpy(szTemp, m_szTempA + 2);
		memset(m_szTempA, 0, MAX_BUF_LEN + 1);
		strcpy(m_szTempA, szTemp);
		int nTempLen = wcslen((LPCWSTR)m_szTempA);

		if(nTempLen != 0)
		{
			int nLen = MAX_BUF_LEN + 1;
			BYTE btContent[MAX_BUF_LEN + 1] = { 0 };
			m_converter.HexCharToBinary(m_szTempA, nTempLen*2, btContent, nLen);	
			m_converter.BigEdnToLitEdn((LPWORD)btContent, nLen/2);
			_tcscpy(m_pPB->szName, (LPCWSTR)btContent);
		}		
	}
	else
	{
		memset(m_szTempW, 0, MAX_BUF_LEN + 1);
		MultiByteToWideChar(CP_ACP, 0, m_szTempA, -1, m_szTempW, MAX_BUF_LEN + 1);
		_tcscpy(m_pPB->szName, m_szTempW);
	}

#endif
 	
 	m_pPB->nID    = nIDtmp;
 	m_pPB->wStore = m_wStorePlace;
 	m_pPB->nGroupIndex = 0;

	pArrID->SetAt(nIDtmp-1,nIDtmp);
  
 	return m_pPB;
}

BOOL   CPB::AddPBStd( _PB * pPB )
{
	return WritePB(pPB,ADD_PB);
}

BOOL   CPB::ModifyPBStd( _PB * pPB )
{
	return WritePB(pPB,MDY_PB);
}

BOOL   CPB::DeletePBStd( _PB * pPB )
{
	return WritePB(pPB,DEL_PB);
}

_PB *  CPB::GetPBEx( int nUsedIdx)
{
	TRACE(_T("m_nUsedNum = 0x%x\r\n"),m_nUsedNum);
 	if( nUsedIdx < 0 && nUsedIdx >= m_nUsedNum )
	{
		return NULL;
	}

	Init();
 
	CUIntArray * pArrID = GetIDArray(m_wStorePlace);
	//teana hu 2009.03.06
	if(pArrID == NULL)
	{
		return NULL;
	}

	memset( m_pPB, 0, sizeof(PB) ); 

	LPSTR pFind = NULL;
	int nPrefixLen = strlen(AT_READ_PB_EX_RET_PRE);

	int nID = GetNextUsedID(pArrID);
	while(nID <= pArrID->GetSize())
	{ 	
		memset(m_szInBuf, 0, sizeof(m_szInBuf));
		sprintf( m_szOutBuf, AT_READ_PB_EX,nID); 
		if( !ExecuteCmd(m_szOutBuf) )
 		{		
			m_Log.LogEx(_T("!! Read PB fail, the cmd is: %s !!\r\n"), m_szOutBuf);
 			return NULL;
 		}

 		// skip ^SCPBR:
 		pFind = FindStr(m_szInBuf, MAX_BUF_LEN, AT_READ_PB_EX_RET_PRE, nPrefixLen); 
 		if(pFind == NULL)
		{
			nID++; 
		}
		else
		{
			break;
		}
	}

	if(pFind == NULL)
	{
		return NULL;
	}


 	pFind += nPrefixLen;

	int nIDtmp = 0;
	int nType = 129;

	//parse index 
	LPSTR pFindRet = FindStr(pFind, MAX_BUF_LEN-(pFind - m_szInBuf), "\r\n", 2);
	*pFindRet = '\0';
    pFindRet = strchr(pFind,',');
	*pFindRet = '\0';
	sscanf(pFind,"%d",&nIDtmp);

	_ASSERTE(nIDtmp == nID);

	//^SCPBR: 1,"12345688888",129,"",,"",,"",,"hu",0,""

	LPTSTR pNumber[] = {m_pPB->szPhone,m_pPB->szOfficePhone,m_pPB->szHomePhone,m_pPB->szFax};

	//parse number
	for(int i=0;i<4;i++)
	{
		memset(m_szTempA, 0, sizeof(m_szTempA));
		//parse numi "123456"
		pFind = pFindRet + 1;
		pFindRet = strchr(pFind,',');
		if(pFindRet == NULL)
			return NULL;
		*pFindRet = '\0';
		*(pFindRet-1) = '\0'; //remove right "
		pFind = pFind + 1; // skip left "		
	#ifndef _UNICODE
		sscanf(pFind,"%s",pNumber[i]);
	#else
		sscanf(pFind,"%s",m_szTempA);
		MultiByteToWideChar(CP_ACP, 0, m_szTempA, -1, m_szTempW, MAX_BUF_LEN + 1);
		_tcscpy(pNumber[i], m_szTempW);	
	#endif

		//parse numi type
		pFind = pFindRet + 1;
		pFindRet = strchr(pFind,',');
		if(pFindRet == NULL)
			return NULL;
		*pFindRet = '\0';
		sscanf(pFind,"%d",&nType);
	}

	//parse name "ABCD"
	pFind = pFindRet + 1;
	pFindRet = strchr(pFind,',');
	*pFindRet = '\0';
	*(pFindRet-1) = '\0'; //remove right "
	pFind = pFind + 1; // skip left "
	memset(m_szTempA, 0, MAX_BUF_LEN + 1);
	sscanf(pFind,"%s",m_szTempA);

#ifndef _UNICODE	
	sscanf(pFind,"%s",m_pPB->szName);	
	memset(m_szTempA, 0, MAX_BUF_LEN + 1);

	if(_tcslen(m_pPB->szName) > 2 && m_pPB->szName[0] == '8' && m_pPB->szName[1] == '0')
	{
		_tcscpy(m_szTempA, m_pPB->szName + 2);

		int nTempLen = wcslen((LPCWSTR)m_szTempA);

		if(nTempLen != 0)
		{
			int nLen = MAX_BUF_LEN + 1;
			BYTE btContent[MAX_BUF_LEN + 1] = { 0 };
			m_converter.HexCharToBinary(m_szTempA, nTempLen*2, btContent, nLen);	
			m_converter.BigEdnToLitEdn((LPWORD)btContent, nLen/2);
			WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)btContent, -1, (char *)m_pPB->szName, MAX_PERSON_NAME_LEN + 1, NULL, NULL);
		}		
	}	

#else
	memset(m_szTempA, 0, MAX_BUF_LEN + 1);
	sscanf(pFind,"%s",m_szTempA);

	if(strlen(m_szTempA) > 2 && m_szTempA[0] == '8' && m_szTempA[1] == '0')
	{
		char szTemp [MAX_BUF_LEN + 1] = {0};
		strcpy(szTemp, m_szTempA + 2);
		memset(m_szTempA, 0, MAX_BUF_LEN + 1);
		strcpy(m_szTempA, szTemp);
		int nTempLen = wcslen((LPCWSTR)m_szTempA);

		if(nTempLen != 0)
		{
			int nLen = MAX_BUF_LEN + 1;
			BYTE btContent[MAX_BUF_LEN + 1] = { 0 };
			m_converter.HexCharToBinary(m_szTempA, nTempLen*2, btContent, nLen);	
			m_converter.BigEdnToLitEdn((LPWORD)btContent, nLen/2);
			_tcscpy(m_pPB->szName, (LPCWSTR)btContent);
		}		
	}
	else
	{
		memset(m_szTempW, 0, MAX_BUF_LEN + 1);
		MultiByteToWideChar(CP_ACP, 0, m_szTempA, -1, m_szTempW, MAX_BUF_LEN + 1);
		_tcscpy(m_pPB->szName, m_szTempW);
	}

#endif

	//parse coding
	int nCode = 0;
	pFind = pFindRet + 1;
	pFindRet = strchr(pFind,',');
	*pFindRet = '\0';
	sscanf(pFind,"%d",&nCode);


	//convert name
/*	int nTempLen = strlen(m_szTempA)/2;
	if(nTempLen != 0)
	{
		BYTE btContent[MAX_BUF_LEN + 1] = { 0 };
		int nLen = MAX_BUF_LEN + 1;			
		m_converter.HexCharToBinary(m_szTempA, nTempLen*2, btContent, nLen);

		if(nCode == 1) //UCS2
		{				
			m_converter.BigEdnToLitEdn((LPWORD)btContent, nLen/2);
#ifdef _UNICODE
			_tcscpy(m_pPB->szName,(LPCTSTR)btContent);
#else
			WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)btContent, -1, (char *)m_pPB->szName, MAX_PERSON_NAME_LEN + 1, NULL, NULL);
#endif
			
		}
		else if(nCode == 0) // 7bit
		{
			memset(m_szTempA, 0, MAX_BUF_LEN + 1);
			BYTE offset_bits = 0;
			BYTE offset_octets = 0;
			Transfer7Bitsto8Bits(btContent,(BYTE)(nTempLen*2),&offset_octets,&offset_bits,(LPBYTE)m_szTempA);

#ifndef _UNICODE
			_tcscpy(m_pPB->szName,m_szTempA);
#else	
			MultiByteToWideChar(CP_ACP,0,m_szTempA,-1,m_pPB->szName,MAX_PERSON_NAME_LEN + 1);
#endif			
		}
		else
		{
			m_Log.LogEx(_T("!! Read PB fail, the cmd is: %s !!\r\n"), m_szOutBuf);
			return NULL;
		}
	}*/

	//parse email
	pFind = pFindRet + 1;
	if(*pFind != '\0')
	{
		pFind = pFind + 1; // skip left "

		pFindRet = strchr(pFind,'\"');
		if(pFindRet == NULL)
			return NULL;
		*pFindRet = '\0';//remove right "
		

#ifndef _UNICODE
		sscanf(pFind,"%s",m_pPB->szEmail);
#else
		memset(m_szTempA,0,sizeof(m_szTempA));
		sscanf(pFind,"%s",m_szTempA);
		MultiByteToWideChar(CP_ACP, 0, m_szTempA, -1, m_szTempW, MAX_BUF_LEN + 1);
		_tcscpy(m_pPB->szEmail, m_szTempW);	
#endif
	}
	
 	m_pPB->nID    = nIDtmp;
 	m_pPB->wStore = m_wStorePlace;
 	m_pPB->nGroupIndex = 0;

	pArrID->SetAt(nIDtmp-1,nIDtmp);
  
 	return m_pPB;
}

BOOL   CPB::AddPBEx( _PB * pPB )
{
	return WritePBEx(pPB,ADD_PB);
}

BOOL   CPB::ModifyPBEx( _PB * pPB )
{
	return WritePBEx(pPB,MDY_PB);
}

BOOL   CPB::DeletePBEx( _PB * pPB )
{
	return WritePBEx(pPB,DEL_PB);
}

BOOL CPB::WritePB(_PB * pPB, int nMode)
{
	_ASSERTE( pPB != NULL );
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );

	//teana hu 2009.03.06
	if(pPB == NULL || m_pPort == NULL || !m_pPort->IsOpened())
	{
		return FALSE;
	}
	CUIntArray * pArrID = GetIDArray(pPB->wStore);
	//teana hu 2009.03.06
	if(pArrID == NULL)
	{
		return FALSE;
	}

	if(!SelectStorePlace(pPB->wStore))
	{
		return FALSE;
	}

 	if(nMode == 0) // add
	{
		pPB->nID = GetUnusedID(pArrID);
	}

	if(nMode == 2)// delete
	{
		memset(m_szOutBuf,0,sizeof(m_szOutBuf));
		sprintf( m_szOutBuf, AT_DELETE_PB, pPB->nID);

		if( !ExecuteCmd(m_szOutBuf) )
		{
			m_Log.LogEx(_T("!! Delete PB fail, the cmd is %s\r\n"),m_szOutBuf);
			return FALSE;
		}

		int nCount = pArrID->GetSize();
		for( int i = 0; i < nCount; ++i )
		{
			if( pArrID->GetAt(i) == (unsigned int)pPB->nID )
			{
				pArrID->SetAt(i, 0);
				//pArrID->RemoveAt(i);
				break;
			}
		}

		return TRUE;
	}

	int  nDesSize = MAX_BUF_LEN;
	char szHexStr[ MAX_BUF_LEN ] = { 0 };

	MMI_PB_V1::PB_PHONEBOOK_ENTRY_T pet = { 0 };

	ConvertPBToPBET(pPB, pet);	

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, pPB->szName, -1, m_szTempA, MAX_BUF_LEN + 1, NULL, NULL);
#else
	strcpy(m_szTempA,pPB->szName);
#endif
	
	if(_tcslen(pPB->szName)!= 0 && HasUcs2Char(pPB->szName))
	{
		char szPhoneName[MAX_BUF_LEN + 1] = {0};
		int nDesLen = MAX_BUF_LEN + 1;
		m_converter.BinaryToHexChar(pet.entry_name.alpha, pet.entry_name.alpha_len, szPhoneName, nDesLen);
		
		sprintf( m_szTempA, "80%s",szPhoneName );
	}
#ifndef _UNICODE
	sprintf( m_szOutBuf, AT_WRITE_PB, pPB->nID, pPB->szPhone,m_szTempA );
#else
	char szTemp [MAX_BUF_LEN + 1] = {0};
	WideCharToMultiByte( CP_ACP, 0, pPB->szPhone, -1, szTemp, MAX_BUF_LEN + 1, NULL, NULL);
	sprintf( m_szOutBuf, AT_WRITE_PB, pPB->nID, szTemp,m_szTempA );
#endif
	if( !ExecuteCmd(m_szOutBuf) )
	{		
        m_Log.LogEx(_T("!! Write PB fail !!\r\n"));
		return FALSE;
	}

	if(nMode==0) // add
	{
		pArrID->SetAt(pPB->nID-1,pPB->nID);
	}

	return TRUE;
}


BOOL CPB::WritePBEx(_PB * pPB, int nMode)
{
	_ASSERTE( pPB != NULL );
	_ASSERTE( m_pPort != NULL && m_pPort->IsOpened() );

	//teana hu 2009.03.06
	if(pPB == NULL || m_pPort == NULL || !m_pPort->IsOpened())
	{
		return FALSE;
	}

	CUIntArray * pArrID = GetIDArray(pPB->wStore);
	//teana hu 2009.03.06
	if(pArrID == NULL)
	{
		return FALSE;
	}

	if(!SelectStorePlace(pPB->wStore))
	{
		return FALSE;
	}

	if(nMode == 0) // add
	{
		pPB->nID = GetUnusedID(pArrID);
	}

	if(nMode == 2)// delete
	{
		memset(m_szOutBuf,0,sizeof(m_szOutBuf));
		sprintf( m_szOutBuf, AT_DELETE_PB_EX, pPB->nID);

		if( !ExecuteCmd(m_szOutBuf) )
		{
			m_Log.LogEx(_T("!! Delete PB fail, the cmd is %s\r\n"),m_szOutBuf);
			return FALSE;
		}

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

	int  nDesSize = MAX_BUF_LEN;
	char szHexStr[ MAX_BUF_LEN +1] = { 0 };

	MMI_PB_V1::PB_PHONEBOOK_ENTRY_T pet = { 0 };

	ConvertPBToPBET(pPB, pet);	

	memset(m_szOutBuf,0,sizeof(m_szOutBuf));

	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, pPB->szName, -1, m_szTempA, MAX_BUF_LEN + 1, NULL, NULL);
#else
	strcpy(m_szTempA,pPB->szName);
#endif

	int nCode = 0; // 7Bit coding
	
	if(_tcslen(pPB->szName)!= 0 && HasUcs2Char(pPB->szName))
	{
		nCode = 1; // UCS2 coding
		m_converter.BinaryToHexChar(pet.entry_name.alpha, pet.entry_name.alpha_len, szHexStr, nDesSize);		
		sprintf( m_szTempA, "80%s",szHexStr );
	}
	else
	{
		//BYTE szPhoneName[MAX_BUF_LEN + 1] = {0};
		//int nSize = Transfer8Bitsto7Bits(m_szTempA,szPhoneName,strlen(m_szTempA));
		//m_converter.BinaryToHexChar(szPhoneName, nSize, szHexStr, nDesSize);	
		//m_converter.BinaryToHexChar(pet.entry_name.alpha, pet.entry_name.alpha_len, szHexStr, nDesSize);
		sprintf( m_szTempA, "%s",pet.entry_name.alpha );

	}
#ifndef _UNICODE
	sprintf( m_szOutBuf, AT_WRITE_PB_EX, 
		pPB->nID,
		pPB->szPhone,
		pPB->szOfficePhone,
		pPB->szHomePhone,
		pPB->szFax,	
		m_szTempA,
		nCode,
		pPB->szEmail);
#else
	char szTemp [MAX_BUF_LEN + 1] = {0};
	WideCharToMultiByte( CP_ACP, 0, pPB->szFax, -1, szTemp, MAX_BUF_LEN + 1, NULL, NULL);
	sprintf( m_szOutBuf, AT_WRITE_PB_EX,
		pPB->nID,
		pet.number_t[0].number,
		pet.number_t[1].number,
		pet.number_t[2].number,
		szTemp,	
		m_szTempA,
		nCode,
		pet.mail.mail);
#endif
	if( !ExecuteCmd(m_szOutBuf) )
	{		
        m_Log.LogEx(_T("!! Write PB fail !!\r\n"));
		return FALSE;
	}

	if(nMode == 0)
	{
		pArrID->SetAt(pPB->nID-1,pPB->nID);
	}

	return TRUE;
}

__inline CUIntArray * CPB::GetIDArray(WORD wStore)
{
	CUIntArray * pArrID = NULL;
	if( wStore == ID_STORE_PHONE )
		pArrID = &m_arrMSID;
	else if( wStore == ID_STORE_SIM )
		pArrID = &m_arrSIMID;
	else if( wStore == ID_STORE_SIM2 )
		pArrID = &m_arrSIMID2;

	return pArrID;
}

BYTE CPB::GetBits( LPBYTE byte_in,     BYTE bits_length,
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

int CPB::Transfer7Bitsto8Bits( LPBYTE tpdu_ptr, int sms_length, LPBYTE offset_octets_ptr,
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

int CPB::Transfer8Bitsto7Bits(const char* pSrc, unsigned char* pDst, int nSrcLength)
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

BYTE CPB::Map7BitCharTo8Bit(LPBYTE lpData_in)
{
	BYTE result = 0;

	result = default_to_ascii_table[*lpData_in];

	return result;
}

BOOL CPB::GetUsedIDEx()
{
	sprintf( m_szOutBuf, AT_GET_PB_USED_ID_EX); 

    if( !ExecuteCmd(m_szOutBuf) )
 	{		
        m_Log.LogEx(_T("!! Get PB used ID fail, the cmd is: %s !!\r\n"), m_szOutBuf);
 		return FALSE;
 	}
 
 	int nPrefixLen = strlen(AT_READ_PB_EX_RET_PRE);
 
 	
 	LPSTR pFind = FindStr(m_szInBuf, MAX_BUF_LEN, AT_READ_PB_EX_RET_PRE, nPrefixLen);
 
 	if(pFind == NULL)
 		return FALSE;

	CUIntArray * pArrID = GetIDArray(m_wStorePlace);
	//teana hu 2009.03.06
	if(pArrID == NULL)
	{
		return FALSE;
	}
 
 	pFind += nPrefixLen;
 
	//eg. (1-20,22,25-30),10,20,30
	LPSTR pFindRet = FindStr(pFind, MAX_BUF_LEN-(pFind - m_szInBuf), "\r\n", 2);
	*pFindRet = '\0';

	pFindRet = strchr(pFind,',');
	*pFindRet = '\0';
	*(pFindRet-1) = '\0';//remove right )
	pFind += 2; //remove left (

	LPSTR pNext = pFindRet+1;

	int nSize = pArrID->GetSize();

	int nIdx= 0;
	while(*pFind != '\0')
	{
		if(nIdx >= nSize)
				break;

		LPSTR pTmp = pFind;
		pFindRet = strchr(pFind,',');
		if(pFindRet != NULL)
		{
			*pFindRet = '\0';
			pFind = pFindRet + 1;	
			pFindRet = strchr(pTmp,'-');
			if(pFindRet != NULL)
			{
				*pFindRet = '\0';
				int nStart = atoi(pTmp);
				pTmp = pFindRet+1;
				int nEnd = atoi(pTmp);

				for(int i=nStart;i<nEnd;i++)
				{
					if(nIdx >= nSize)
						break;

					pArrID->SetAt(nIdx++,i);
				}				
			}
			else
			{
				int nID = atoi(pTmp);
				pArrID->SetAt(nIdx++,nID);
			}
		}
		else
		{
			pFindRet = strchr(pTmp,'-');
			if(pFindRet != NULL)
			{
				*pFindRet = '\0';
				int nStart = atoi(pTmp);
				pTmp = pFindRet+1;
				int nEnd = atoi(pTmp);

				for(int i=nStart;i<nEnd;i++)
				{
					if(nIdx >= nSize)
						break;
					pArrID->SetAt(nIdx++,i);
				}		
			}
			else
			{
				int nID = atoi(pTmp);
				pArrID->SetAt(nIdx++,nID);
			}
			break;
		}
	}

	pFind = pNext;

	//parse nlenght

	//parse tlenght

	//parse mlenght


	return TRUE;	

}