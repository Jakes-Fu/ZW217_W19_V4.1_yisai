// TPSet.cpp: implementation of the CTPSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include <Windows.h>
#include <crtdbg.h>
#include "TPSet.h"

#pragma warning(disable:4127 4100)

#define WM_CLEAR_STATE 0x1027

/**************************************************************/
//
// CMassTPSet implementations
//
/**************************************************************/

CMassTPSet::CMassTPSet()
{
    m_hSwapFile    = INVALID_HANDLE_VALUE;
    m_hSwapMap     = NULL;

    m_lTPCount     = 0;
	m_nCurveNum	   = DEFAULT_FIND_DATA_NUM;
    m_lMaxCount    = TP_MAX_COUNT;
    m_lFindCount   = 0;

    m_lpRead	   = NULL;
    m_lpWrite      = NULL;
    m_lpCurWrite   = NULL;
	m_bOnlySaveRct = FALSE;
    
	memset(m_szTPName,0,_MAX_PATH);
    memset(m_szTPPath,0,_MAX_PATH);
    memset(m_szTPFile,0,_MAX_PATH);
	memset(m_szOverTimeTPFile,0,_MAX_PATH);
	memset(m_szTPDirectory, 0, MAX_PATH);
	memset(m_szTPDirectoryBak, 0, MAX_PATH);
	memset(m_szSearchDesc, 0, MAX_PATH);
	
    m_vecFindIdx.reserve(100000);
	m_vecColorIdx.reserve(100000);
	m_vecSigned.reserve(100000);

    InitializeCriticalSection(&m_CS);
	InitializeCriticalSection(&m_CSRead);

	m_dwStartPos	= 0;
	m_dwEndPos		= 0;
	//teana hu 2009.05.18
	m_lpPreFix		= NULL;
	m_CallBack		= NULL;
	m_dwStartTime	= 0;
	m_hRdFile    = INVALID_HANDLE_VALUE;
    m_hRdMap     = NULL;
	memset(m_szLogSign,0,sizeof(m_szLogSign));
	
}//lint !e1401

CMassTPSet::~CMassTPSet()
{
    Close();
    DeleteCriticalSection(&m_CS);
	DeleteCriticalSection(&m_CSRead);
}

BOOL CMassTPSet::InitForWrite()
{
    _ASSERTE( m_lMaxCount >  0 );
	CAutoCS cs( m_CS );
	CAutoCS csRead( m_CSRead );
    Close();

    GenerateFileName();

    if(m_hSwapFile == INVALID_HANDLE_VALUE)
    {
        m_hSwapFile = ::CreateFile(m_szTPFile,
            GENERIC_READ|GENERIC_WRITE,
            FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_ALWAYS,
            FILE_FLAG_SEQUENTIAL_SCAN,
            NULL);

    }

    if( m_hSwapFile != INVALID_HANDLE_VALUE )
    {
        ULARGE_INTEGER ulLen;
        ulLen.QuadPart = m_lMaxCount * TP_SIZE;

        m_hSwapMap = ::CreateFileMapping(m_hSwapFile,NULL,PAGE_READWRITE,ulLen.HighPart,ulLen.LowPart,NULL);	
    }

    if( m_hSwapMap != NULL )
    {
        m_lpWrite = (LPBYTE)::MapViewOfFile(m_hSwapMap, FILE_MAP_WRITE, 0, 0, 0);
    }

	BOOL bInitRead = FALSE;
	if(m_hRdFile == INVALID_HANDLE_VALUE)
	{
		bInitRead = InitForRead(m_szTPFile);
	}
	

    m_lpCurWrite = (LPBYTE)m_lpWrite;
   // m_lpRead     = (LPBYTE)m_lpWrite;
    m_lTPCount   = 0L;

    return (m_hSwapMap != NULL && m_lpWrite != NULL && bInitRead);
}

BOOL CMassTPSet::InitForRead(LPCTSTR szFilePath)
{

	BOOL bOK = FALSE;
	do 
	{
		m_hRdFile = ::CreateFile(szFilePath, 
			GENERIC_READ,
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL);

		if (INVALID_HANDLE_VALUE == m_hRdFile)
		{
			break;
		}

		m_hRdMap = CreateFileMapping( m_hRdFile,   //The Handle of Opened File
			NULL,          //Security
			PAGE_READONLY, //Read Only Access
			0,             //Max Size
			0,             //Min Size
			"");		   //Object Name
		if( m_hRdMap )
		{
			m_lpRead = (LPBYTE)::MapViewOfFile(m_hRdMap, FILE_MAP_READ, 0, 0, 0);
			bOK = TRUE;
		}
	} while(0);

	if (!bOK)
	{
		if(m_lpRead)
		{
			UnmapViewOfFile((void*)m_lpRead);
		}
		if (m_hRdMap)
		{
			CloseHandle(m_hRdMap);
			m_hRdMap = NULL;
		}
		
		if( INVALID_HANDLE_VALUE != m_hRdFile )
		{
			CloseHandle(m_hRdFile);
			m_hRdFile = INVALID_HANDLE_VALUE;
		}
	}
    return bOK;
}
BOOL CMassTPSet::CheckWritePointer()
{
    BOOL bRet = TRUE;
	if( 0 == m_lTPCount ) 
	{
        bRet = InitForWrite();		
	}
	
    if( m_lTPCount >= m_lMaxCount )
    {
		//delete overtime file
		if ( m_bOnlySaveRct )
		{
			if (strlen(m_szOverTimeTPFile) != 0 )
			{
				DeleteFile(m_szOverTimeTPFile);
			}
			
			// if only save recent tp set down file name
			strcpy(m_szOverTimeTPFile,m_szTPFile);	
		}
		//teana hu 2010.07.26
		::SendMessage(m_hGuiWnd, WM_SAVE_MARK, 0, 0);
		//
		//m_lpPreFix = NULL;
		
		bRet = InitForWrite();	
		if (bRet)//fixed CR 134041
		{
			AddFlagTime(TRUE);
		}
    }
	
    return bRet;
}

BOOL CMassTPSet::CheckReadPointer(ULONG nIndex)
{
    if( nIndex >= m_lTPCount || m_lpRead == NULL ||nIndex <0)
    {
        return FALSE;
    }

    return TRUE;
}

ULONG CMassTPSet::GetTp(ULONG nIndex)
{   
    CAutoCS cs( m_CSRead );
    if( !CheckReadPointer(nIndex) )
    {
        return 0;
    }
    
    ULONG dwOffset = nIndex * TP_SIZE;
    ULONG qwData = *((ULONG*)(m_lpRead + dwOffset));

    return qwData;
}

void  CMassTPSet::SetStartPos(DWORD dwStartPos)
{
	m_dwStartPos = dwStartPos;
}

void  CMassTPSet::SetEndPos(DWORD dwEndPos)
{
	m_dwEndPos = dwEndPos;
}

void  CMassTPSet::SetFilePreFix(LPSTR lpPreFix)
{
	m_lpPreFix = lpPreFix;
}

LPSTR  CMassTPSet::GetTPFileName()
{
	return m_szTPFile;
}

void   CMassTPSet::SetLogSign(LPSTR lpSign)
{
	if (lpSign)
	{
		strcpy(m_szLogSign,lpSign);
	}
}
DWORD CMassTPSet::GetTPBody(ULONG nIndex)
{
    CAutoCS cs( m_CSRead );
    if(!CheckReadPointer(nIndex))
    {
        return 0;
    }

    ULONG dwOffset = nIndex * TP_SIZE;
    DWORD dwTp = (*(m_lpRead + dwOffset) << 8) | *(m_lpRead + dwOffset + 1)
                 | (*(m_lpRead + dwOffset + 2) << 24) | (*(m_lpRead + dwOffset + 3) << 16);

    return dwTp;
}

void CMassTPSet::AddTp( BYTE* pBuf,DWORD dwTime )
{
    CAutoCS cs( m_CS );
   
    if( CheckWritePointer())
    {
        memcpy(m_lpCurWrite,pBuf,TP_DATA_FIELD_SIZE);
        m_lpCurWrite += TP_DATA_FIELD_SIZE;
        memcpy(m_lpCurWrite,&dwTime,TP_DATA_FIELD_SIZE);
        m_lpCurWrite += TP_DATA_FIELD_SIZE;

        m_lTPCount++;
    }

}
ULONG CMassTPSet::GetTpCount()
{
	CAutoCS cs(m_CSRead);
	return m_lTPCount;
}
WORD CMassTPSet::GetTPAddr(ULONG nIndex)
{    
    CAutoCS cs( m_CSRead );
    if(!CheckReadPointer(nIndex))
    {
        return 0;
    }

    ULONG lOffset = nIndex * TP_SIZE;
    WORD  wAddr   = *(m_lpRead + lOffset);
    wAddr <<= 8;
    wAddr |= *(m_lpRead + lOffset + 1);

    return wAddr;
}

WORD CMassTPSet::GetTPData(ULONG nIndex)
{      
    CAutoCS cs( m_CSRead );
    if( !CheckReadPointer(nIndex) )
    {
        return 0;
    }

    ULONG lOffset = nIndex * TP_SIZE + 2;
    WORD  wData   = *(m_lpRead + lOffset);
    wData <<= 8;
    wData |= *(m_lpRead + lOffset + 1);

    return wData;
}

DWORD CMassTPSet::GetTPTime(ULONG nIndex)
{
    CAutoCS cs( m_CSRead );
    if(!CheckReadPointer(nIndex))
    {
        return 0;
    }

    ULONG dwOffset = nIndex * TP_SIZE + 4;
    DWORD dwData = *(m_lpRead + dwOffset) + (*(m_lpRead + dwOffset + 1) << 8)
                   + (*(m_lpRead + dwOffset + 2) << 16) + (*(m_lpRead + dwOffset + 3) << 24);

    return dwData;
}



ULONG CMassTPSet::Find(ULONG nStart,char* pAddr,char* pData,int bUp,LPCTSTR pDesc)
{
    if(pAddr == NULL || pData == NULL || nStart > m_lTPCount)
    {
        return 0;
    }

    m_lSearchCur = nStart;
    m_bSearchUp = bUp;

    CalculateMask(pAddr,pData,pDesc);

    ULONG ll = Search();
    if(ll != -1)
    {
        m_lSearchCur = ll;
    }

    return ll;
}

void CMassTPSet::CalculateMask(char* pAddr,char* pData,LPCTSTR pDesc)
{
    char szAddr[TP_ITEM_SIZE + 1],szData[TP_ITEM_SIZE + 1];
    strcpy(szAddr,pAddr);
    strcpy(szData,pData);
	if (pDesc && strlen(pDesc))
	{
		strcpy(m_szSearchDesc,pDesc);
	}
	else
	{
		memset(m_szSearchDesc,0,MAX_PATH);
	}

    int nAddr = strlen(szAddr);
    int nData = strlen(szData);
    
    if(nAddr == 0)
    {
        strcpy(szAddr,"****");
        nAddr = TP_ITEM_SIZE;
    }
    
    if(nData == 0)
    {
        strcpy(szData,"****");
        nData = TP_ITEM_SIZE;
    }
    
    for(int i=0;i<TP_ITEM_SIZE - nAddr;i++)
    {
        strcat(szAddr,"*");
    }
    
    nAddr = TP_ITEM_SIZE;
    
    for(i=0;i<TP_ITEM_SIZE - nData;i++)
    {
        strcat(szData,"*");
    }
  
	
    nData = TP_ITEM_SIZE;
    
    m_dwSearchData = m_dwSearchMask = 0;
    
    for(i=0;i<nData;i+=2)
    {
        for(int j=0;j<2;j++)
        {
            m_dwSearchMask = m_dwSearchMask << 4;
            m_dwSearchData = m_dwSearchData << 4;
            
            const char c = szData[i+j];
            
            if(c >= '0' && c <= '9')
            {
                m_dwSearchMask += 0xf;
                m_dwSearchData += c - '0';
            }
            else if(c >='A' && c <= 'F')
            {
                m_dwSearchMask += 0xf;
                m_dwSearchData += (c - 'A' + 10);
            }
            else if(c >= 'a' && c <= 'f')
            {
                m_dwSearchMask += 0xf;
                m_dwSearchData += (c - 'a' + 10);
            }
        }
    }
    
    for(i=0;i<nAddr;i+=2)
    {
        for(int j=0;j<2;j++)
        {
            m_dwSearchMask = m_dwSearchMask << 4;
            m_dwSearchData = m_dwSearchData << 4;
            
            const char c = szAddr[i+j];
            if(c >= '0' && c <= '9')
            {
                m_dwSearchMask += 0xf;
                m_dwSearchData += c - '0';
            }
            else if(c >='A' && c <= 'F')
            {
                m_dwSearchMask += 0xf;
                m_dwSearchData += (c - 'A' + 10);
            }
            else if(c >= 'a' && c <= 'f')
            {
                m_dwSearchMask += 0xf;
                m_dwSearchData += (c - 'a' + 10);
            }
        }
    }
}

ULONG CMassTPSet::Search()
{
    if(m_bSearchUp)
    {	
        for(;m_lSearchCur!=0;m_lSearchCur--)
        {
            DWORD dwSrc = GetTPBody(m_lSearchCur);
            if( ! ((dwSrc & m_dwSearchMask) ^ m_dwSearchData) )
            {
				DWORD dwAddr = GetTPAddr(m_lSearchCur);
				DWORD dwData = GetTPData(m_lSearchCur);
				if(m_CallBack((int)dwAddr, (int)dwData, m_szSearchDesc, TP_LIKE))
				{
					return m_lSearchCur;
				}
                
            }
        }
    }
    else
    {
        for(;m_lSearchCur<m_lTPCount;m_lSearchCur++)
        {
            DWORD dwSrc = GetTPBody(m_lSearchCur);
			//m_szSearchDesc
            if( ! ((dwSrc & m_dwSearchMask) ^ m_dwSearchData) )
            {
                DWORD dwAddr = GetTPAddr(m_lSearchCur);
				DWORD dwData = GetTPData(m_lSearchCur);
				if(m_CallBack((int)dwAddr, (int)dwData, m_szSearchDesc, TP_LIKE))
				{
					return m_lSearchCur;
				}
            }
        }
    }

    return 0;
}

ULONG CMassTPSet::FindNext()
{
    m_bSearchUp = FALSE;
    m_lSearchCur++;
    return Search();
}

ULONG CMassTPSet::FindPre()
{
    m_bSearchUp = TRUE;
    m_lSearchCur--;
    return Search();
}

void CMassTPSet::GenerateFileName()
{
	//teana hu 2009.05.18
	char name[_MAX_FNAME];

	char szPrefix[MAX_PATH] = {0};
	if(m_lpPreFix == NULL)
	{	
		sprintf(szPrefix, "%s",GetLocalTime().c_str());		
	}
	else
	{
		sprintf(szPrefix, "%s%s",m_lpPreFix,GetLocalTime().c_str());
	}

	sprintf(name, "\\%s%s.tp", szPrefix,m_szLogSign);
	strcpy(m_szTPFile,m_szTPPath);
	//teana hu 2009.10.26
	if(m_szTPDirectory[0] != '\0')
	{
		char szTemp[MAX_PATH] = {0};
		strcpy(szTemp, m_szTPFile);
		strcat(m_szTPFile, "\\");
		strcat(m_szTPFile, m_szTPDirectory);
		ZeroMemory(m_szTPDirectoryBak, MAX_PATH);
		strcpy(m_szTPDirectoryBak, m_szTPFile);
		if(!CreateDirectory( m_szTPDirectoryBak, NULL ))
		{
			DWORD dwErr = GetLastError();
			if(dwErr != ERROR_ALREADY_EXISTS)
			{
				memset(m_szTPFile, 0, MAX_PATH);
				strcpy(m_szTPFile, szTemp);
			}
		}
	}
	strcat(m_szTPFile,name);
}

LPSTR CMassTPSet::GetTPDirectory()
{
	return m_szTPDirectoryBak;
}

void WINAPI CMassTPSet::SetCBFunc(PFCALLBACK Func)
{
	if(Func == NULL)
	{
		return;
	}
	m_CallBack = Func;
}

void CMassTPSet::SetLogFileName(char* pLogName)
{
	_ASSERTE(pLogName != NULL);

	strcpy(m_szTPName, pLogName);
	char* pPos= strchr(m_szTPName,'.');
	if (pPos != NULL)
	{
		*pPos='\0';
	}
}

void CMassTPSet::Close()
{
    CAutoCS cs( m_CS );
	CAutoCS csRead( m_CSRead );

    ClearFind();

	//release read 
	if(m_lpRead)
    {
        UnmapViewOfFile((void*)m_lpRead);
    }
	if (m_hRdMap)
	{
		CloseHandle(m_hRdMap);
		m_hRdMap = NULL;
	}
	
	if( INVALID_HANDLE_VALUE != m_hRdFile )
	{
		CloseHandle(m_hRdFile);
		m_hRdFile = INVALID_HANDLE_VALUE;
	}

	//release write

    if(NULL != m_lpWrite)
    {
        UnmapViewOfFile(m_lpWrite);
    }

    CloseHandle(m_hSwapMap);
    
    if( INVALID_HANDLE_VALUE != m_hSwapFile && m_lpWrite != NULL )
    {        
        // Change to real size
        LARGE_INTEGER qwLength;
        qwLength.QuadPart = m_lTPCount * TP_SIZE;
        SetFilePointer(m_hSwapFile, qwLength.LowPart, &qwLength.HighPart,
            FILE_BEGIN);
       
        if( !SetEndOfFile(m_hSwapFile) )
        {
            _ASSERTE( GetLastError() == 0 );
        }          
    }

    CloseHandle(m_hSwapFile);  
	ClearFind();
   
    m_lTPCount			 = 0;
    m_lpWrite			 = NULL;
    m_lpRead			 = NULL;
    m_lpCurWrite		 = NULL;
    m_hSwapFile			 = INVALID_HANDLE_VALUE;
    m_hSwapMap			 = NULL; 
	//m_lpPreFix			 = NULL;
	::PostMessage(m_hGuiWnd, WM_CLEAR_STATE, 0, 0);
}

void CMassTPSet::Clear()
{
	Close();
	
	//del mapping file when catch log 
	if (strlen(m_szTPFile) != 0)
	{
		DeleteFile(m_szTPFile);
	}
}

BOOL CMassTPSet::CombineFile()
{
	if (NULL == m_szOverTimeTPFile || 0 == strlen(m_szOverTimeTPFile))
	{
		return FALSE;
	}
	
	//waite for receive thread finished 
	Sleep(1);

	//close current tp file
	Close();	

	//
	//Combine file 
	//
	const int size=4096;
	char  buf[size]; 
    FILE* pfold = fopen( m_szOverTimeTPFile,"ab+" );
    if( NULL == pfold )
    {
        return FALSE;
    }

	FILE* pfnew = fopen( m_szTPFile,"rb" );
    if( NULL == pfnew )
    {
        return FALSE;
    }

	//append data to old file
	while( !feof( pfnew ) )
	{
		int count = fread(buf, sizeof(char), size, pfnew);
		if( ferror( pfnew ) )      
		{
			break;
		}
		fwrite(buf, sizeof(char), count, pfold);

	}
	SAFE_CLOSE_FILE(pfold);
	SAFE_CLOSE_FILE(pfnew);

	//del current tp file
	if (strlen(m_szTPFile) != 0)
	{
		DeleteFile(m_szTPFile);		
		memset(m_szTPFile, 0, _MAX_PATH);
	}

	//reload TP file
	Load(m_szOverTimeTPFile);
	memset(m_szOverTimeTPFile, 0, _MAX_PATH);	
	return TRUE;
}


BOOL CMassTPSet::Load(LPCTSTR lpszFileName)
{
    _ASSERTE( lpszFileName != NULL );
	
	//close last tp file
    Close();

    strcpy(m_szTPFile, lpszFileName);

    m_hSwapFile = ::CreateFile(m_szTPFile,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    
    if( INVALID_HANDLE_VALUE == m_hSwapFile )
    {
        return FALSE;
    }

    ULARGE_INTEGER uiLen;
    uiLen.LowPart = GetFileSize(m_hSwapFile, &uiLen.HighPart);

    m_lMaxCount = (ULONG)(uiLen.QuadPart / TP_SIZE);
    m_hSwapMap = ::CreateFileMapping(m_hSwapFile,NULL,PAGE_READONLY,uiLen.HighPart,uiLen.LowPart,NULL);	
    if( NULL == m_hSwapMap )
    {
        CloseHandle(m_hSwapFile);
        m_hSwapFile = INVALID_HANDLE_VALUE;
        return FALSE;
    }

    m_lpRead = (LPBYTE)::MapViewOfFile(m_hSwapMap, FILE_MAP_READ, 0, 0, 0);
    if( NULL == m_lpRead )
    {
        CloseHandle(m_hSwapMap);
        m_hSwapMap = NULL;
        CloseHandle(m_hSwapFile);
        m_hSwapFile = INVALID_HANDLE_VALUE;

        return FALSE;
    }

    m_lTPCount = m_lMaxCount;
        
    return TRUE;
}

void CMassTPSet::SetLogFilePath( char* lpszPath )
{
    strcpy(m_szTPPath,lpszPath);
}

void CMassTPSet::SetTPDirectory(LPSTR lpDirectory)
{
	strcpy(m_szTPDirectory, lpDirectory);
}

inline BOOL TpEqu(DWORD pv1, DWORD pv2, BOOL bSign )
{
    return (WORD)pv1 == (WORD)pv2;
}

inline BOOL TpLess(DWORD pv1, DWORD pv2, BOOL bSign )
{
	if(!bSign)
	{
		return (WORD)pv1 < (WORD)pv2;
	}
	else
	{
		return (short)pv1 < (short)pv2;
	}
}

inline BOOL TpLessEqu(DWORD pv1, DWORD pv2, BOOL bSign )
{
	if(!bSign)
	{
		return (WORD)pv1 <= (WORD)pv2;
	}
	else
	{
		return (short)pv1 <= (short)pv2;
	}
}

inline BOOL TpGreat(DWORD pv1, DWORD pv2, BOOL bSign )
{
	if(!bSign)
	{
		return (WORD)pv1 > (WORD)pv2;
	}
	else
	{
		return (short)pv1 > (short)pv2;
	}
}

inline BOOL TpGreatEqu(DWORD pv1, DWORD pv2, BOOL bSign )
{
	if(!bSign)
	{
		return (WORD)pv1 >= (WORD)pv2;
	}
	else
	{
		return (short)pv1 >= (short)pv2;
	}
}

inline BOOL TpLike(DWORD pv1, DWORD pv2, BOOL bSign)
{
    static char szV[5] = { 0 };

    char * pszv1 = szV;
    char * pszv2 = NULL;
	pszv2 = pv2? (char *)pv2 : (char *)&pv2;  //Fixed crush issue when pv2==0
	

    sprintf(szV, "%04x", (WORD)pv1);   

    if( *pszv2 == '0' && ( *(pszv2 + 1)== 'x' || *(pszv2+1) == 'X' ) )
    {
        pszv2 += 2;
    }

    if( pszv2[0] == '*' && pszv2[1] == '*' &&
        pszv2[2] == '*' && pszv2[3] == '*' )
    {
        return TRUE;
    }

    for( int i = 0; i < 4; ++i )
    {
        if( pszv2[i] >= 'A' && pszv2[i] <= 'F')
        {
            pszv2[i] += 32;
        }
    }

    for( i = 0; i < 4; ++i )
    {
        if( *(pszv2+i) != '*' && *(pszv2+i) != *(pszv1+i) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

inline BOOL TpUnLike(DWORD pv1, DWORD pv2, BOOL bSign )
{	
	char szUnlike[5] = {0};
	sprintf(szUnlike, "%04x", (WORD)pv2);

	for(int i = 0; i < 4; i++)
	{
		if(szUnlike[i] != '0')
		{
			szUnlike[i] = 'F';
		}
	}

	char * pEnd = NULL;
	DWORD dwUnlike = (WORD)strtol(szUnlike, &pEnd, 16);

	if((pv1 & dwUnlike) == pv2)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

LPCSTR CMassTPSet::GetTpFlagDesc( int nFlag )
{
    _ASSERTE( nFlag >= TP_EQU && nFlag < TP_FLAG_MAX_NUM );

    if (nFlag < TP_EQU || nFlag >= TP_FLAG_MAX_NUM ) 
                return NULL;

    static const char * szFlag[] = { 
        { "=" },
        { "<" },
        { "<=" },
        { ">" },
        { ">=" },
        { "Like" },
		{ "UnLike" },
    };

    return szFlag[nFlag];
}

ULONG CMassTPSet::Find(PTP_FIND_DATA pFindData, DWORD nNum, DWORD* pStartPos, BOOL *pbSign)
{
    _ASSERTE(NULL != pStartPos);

    const int MAX_FIND_NUM = MAX_FIND_DATA_NUM; 
	_ASSERTE( pFindData != NULL && nNum > 0  && nNum <= (DWORD)m_nCurveNum );
	ClearFind();

    typedef BOOL (*PFUN)(DWORD,DWORD,BOOL);

    DWORD dwAddr[MAX_FIND_NUM] = {0};
    DWORD dwData[MAX_FIND_NUM] = {0};
    PFUN pFunAddr[] = { TpEqu, TpLess, TpLessEqu, TpGreat, TpGreatEqu, TpLike, TpUnLike};
    PFUN pFunData[] = { TpEqu, TpLess, TpLessEqu, TpGreat, TpGreatEqu, TpLike, TpUnLike};

	//get filter addr and data
    int i=0;
	char * pEnd = NULL;
	for( i = 0; i < (int)nNum; ++i )
	{
		if(pFindData[i].dwReserve2 != TP_A_LESS_VALUE_LESS_B && pFindData[i].dwReserve2 != TP_DESCRIPTION)
		{
			if( pFindData[i].wAddrFlag == TP_LIKE || pFindData[i].wAddrFlag == TP_UNLIKE)
			{
				dwAddr[i] = (DWORD)pFindData[i].szAddrValue;
			}
			else
			{
				dwAddr[i] = (WORD)strtol(pFindData[i].szAddrValue, &pEnd, 16);
			}
			
			if( pFindData[i].wDataFlag == TP_LIKE || pFindData[i].wDataFlag == TP_UNLIKE)
			{
				dwData[i] = (DWORD)pFindData[i].szDataValue;
			}
			else
			{
				dwData[i] = (WORD)strtol(pFindData[i].szDataValue, &pEnd, 16);
			}
		}	
	}

    //TPSet reset
    if (*pStartPos >= m_lTPCount)
    {
        *pStartPos= 0;
        return 0;
    }    

	//find target test points
	ULONG lIdx= 0;
    //for(lIdx = *pStartPos; lIdx < m_lTPCount; lIdx++ )
	DWORD dwStartPos = *pStartPos;
	DWORD dwEndPos = m_lTPCount;

	if(m_dwEndPos != 0)
	{
		dwEndPos = m_dwEndPos;
	}

	if(m_dwStartPos != 0)
	{
		dwStartPos = m_dwStartPos;
	}

	for(lIdx = dwStartPos; lIdx < dwEndPos; ++lIdx )
	{	
		if  (0 == lIdx)
		{
			continue;
		}
		for( i = 0; i < (int)nNum; ++i )
		{
			if(pFindData[i].dwReserve2 == TP_DESCRIPTION)
			{
				DWORD dwAddr = GetTPAddr(lIdx);
				DWORD dwData = GetTPData(lIdx);
				
				if(m_CallBack((int)dwAddr, (int)dwData, pFindData[i].szDescription, pFindData[i].wDataFlag))
				{
					++m_lFindCount;
					m_vecFindIdx.push_back(lIdx);
					m_vecColorIdx.push_back(i);
					m_vecSigned.push_back(pbSign[i]);
				}
			}
			else if(pFindData[i].dwReserve2 != TP_A_LESS_VALUE_LESS_B && pFindData[i].wAddrFlag != TP_UNLIKE
				&& pFindData[i].wDataFlag != TP_UNLIKE)
			{
				if( pFunAddr[pFindData[i].wAddrFlag](GetTPAddr(lIdx), dwAddr[i], pbSign[i]) && 
					pFunData[pFindData[i].wDataFlag](GetTPData(lIdx), dwData[i], pbSign[i]) )
				{
					++m_lFindCount;
					m_vecFindIdx.push_back(lIdx);
					m_vecColorIdx.push_back(i);
					m_vecSigned.push_back(pbSign[i]);
				}
			}
			else if(pFindData[i].wAddrFlag == TP_UNLIKE || pFindData[i].wDataFlag == TP_UNLIKE)
			{
				DWORD dwAddrVal = 0;
				DWORD dwDataVal = 0;
				char szAddr[5] = {0};
				char szData[5] = {0};

				if(pFindData[i].szDataValue[0] == '0')
				{
					if( pFunAddr[pFindData[i].wAddrFlag](GetTPAddr(lIdx), dwAddr[i], pbSign[i]) && 
						(GetTPData(lIdx) & 0xffff) > 0x0fff)
					{
						++m_lFindCount;
						m_vecFindIdx.push_back(lIdx);
						m_vecColorIdx.push_back(i);
						m_vecSigned.push_back(pbSign[i]);
					}
					continue;
				}

				for(int k = 0; k < 4; ++k)
				{
					if(pFindData[i].szAddrValue[k] != '*')
					{
						szAddr[k] = pFindData[i].szAddrValue[k];
					}
					else
					{
						szAddr[k] = '0';
					}

					if(pFindData[i].szDataValue[k] != '*')
					{
						szData[k] = pFindData[i].szDataValue[k];
					}
					else
					{
						szData[k] = '0';
					}
				}

				dwAddrVal = (WORD)strtol(szAddr, &pEnd, 16);
				dwDataVal = (WORD)strtol(szData, &pEnd, 16);

				if( pFunAddr[pFindData[i].wAddrFlag](GetTPAddr(lIdx), dwAddrVal, pbSign[i]) && 
					pFunData[pFindData[i].wDataFlag](GetTPData(lIdx), dwDataVal, pbSign[i]))
				{
					++m_lFindCount;
					m_vecFindIdx.push_back(lIdx);
					m_vecColorIdx.push_back(i);
					m_vecSigned.push_back(pbSign[i]);
				}
			}
			else
			{

				DWORD dwLeftData = 0;
				DWORD dwRightData = 0;

				DWORD dwFlagLeft = TP_GREAT_EQU;
				DWORD dwFlagRight = TP_LESS_EQU;

				char szTmp[20] = {0};

				strcpy(szTmp, pFindData[i].szAddrValue);

				LPSTR lpTmp = szTmp;
				
				while (*lpTmp != '\0')
				{
					if(*lpTmp == '*')
					{
						*lpTmp = '0';
					}
					lpTmp++;
				}

				dwLeftData = (WORD)strtol(pFindData[i].szAddrValue, &pEnd, 16);

				strcpy(szTmp, pFindData[i].szDataValue);
				lpTmp = szTmp;
				
				while (*lpTmp != '\0')
				{
					if(*lpTmp == '*')
					{
						*lpTmp = 'F';
					}
					lpTmp++;
				}

				dwRightData = (WORD)strtol(pFindData[i].szDataValue, &pEnd, 16);

				if(dwRightData == 0)
				{
					dwRightData = 0xFFFF;
					strcpy(pFindData[i].szDataValue, "FFFF");
				}


				if(pFunAddr[TP_GREAT_EQU](GetTPAddr(lIdx), pFindData[i].wAddrFlag, pbSign[i]) &&
					pFunAddr[dwFlagLeft](GetTPData(lIdx), dwLeftData, pbSign[i]) &&
					pFunAddr[TP_LESS_EQU](GetTPAddr(lIdx), pFindData[i].wDataFlag, pbSign[i]) &&
					pFunAddr[dwFlagRight](GetTPData(lIdx), dwRightData, pbSign[i]))
				{
					++m_lFindCount;
					m_vecFindIdx.push_back(lIdx);
					m_vecColorIdx.push_back(i);
					m_vecSigned.push_back(pbSign[i]);
				}
			}
        }
    }
       
    _ASSERTE( m_lFindCount == m_vecFindIdx.size() );
    *pStartPos= lIdx;
    
    return m_lFindCount;
}

ULONG CMassTPSet::GetIndexByFindIdx( ULONG lIdx )
{
    _ASSERTE( lIdx < m_lFindCount );

    return m_vecFindIdx[lIdx];
}

DWORD CMassTPSet::GetColorIndexByFindIdx(ULONG lIdx)
{
	_ASSERTE( lIdx < m_lFindCount );

	return m_vecColorIdx[lIdx];  
}

BOOL CMassTPSet::GetSignedByFindIdx(ULONG lIdx)
{
	_ASSERTE( lIdx < m_lFindCount );
	
	return m_vecSigned[lIdx]; 
}

void   CMassTPSet::ClearFind(void) 
{
	std::vector<DWORD>	vecTempFindIdx;
	std::vector<DWORD>	vecTempColorIdxx;
	std::vector<BOOL>	vecTempSigned;
    m_lFindCount = 0; 	
	m_vecFindIdx.swap(vecTempFindIdx);
	m_vecColorIdx.swap(vecTempColorIdxx);
	m_vecSigned.swap(vecTempSigned);

	m_vecFindIdx.reserve(100000);
	m_vecColorIdx.reserve(100000);
	m_vecSigned.reserve(100000);
}

void CMassTPSet::ClearPos()
{
	m_dwStartPos = 0;
	m_dwEndPos = 0;
}

void CMassTPSet::SetCurveNum(int nCurveNum)
{
	m_nCurveNum = nCurveNum;
}

void CMassTPSet::SetGuiHwnd(HWND hWnd)
{
	m_hGuiWnd = hWnd;
}

void CMassTPSet::AddFlagTime(BOOL bStart)
{
	BYTE buf[4] = {0};
	if(!bStart)
	{
		buf[3] = 1;
	}
	
	memcpy(m_lpCurWrite,buf,TP_DATA_FIELD_SIZE);
	m_lpCurWrite += TP_DATA_FIELD_SIZE;
	memcpy(m_lpCurWrite,&m_dwStartTime,TP_DATA_FIELD_SIZE);
	m_lpCurWrite += TP_DATA_FIELD_SIZE;
	
	m_lTPCount++;
}

void CMassTPSet::SetStartTime(DWORD dwTime)
{
	m_dwStartTime = dwTime;
}