// BMFileImpl.cpp: implementation of the CBMFileImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BMFileImpl.h"

static BOOL GetAbsolutePath(CString &strAbsoluteFilePath,LPCTSTR lpszFilePath )
{
    _TCHAR szFileName[_MAX_FNAME];    
    _TCHAR szDir[_MAX_DIR];
    _TCHAR szDirve[_MAX_DRIVE];	
    
    _tsplitpath( lpszFilePath,szDirve,NULL,NULL,NULL);	
	if(szDirve[0] != _T('\0') || (_tcslen(lpszFilePath)>2 && lpszFilePath[0] == _T('\\') && lpszFilePath[1] == _T('\\')))
	{
		strAbsoluteFilePath = lpszFilePath;
	}
	else
	{
		//do it if strHelpTopic is ralatively 
        GetModuleFileName( AfxGetApp()->m_hInstance, szFileName,_MAX_FNAME);
        _tsplitpath( szFileName , szDirve , szDir , NULL , NULL );	
        strAbsoluteFilePath = szDirve;
        strAbsoluteFilePath += szDir;
        if( lpszFilePath[0] == _T('\\') || lpszFilePath[0] == _T('/') )
            lpszFilePath++;
        
        strAbsoluteFilePath += lpszFilePath;		
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBMFileImpl::CBMFileImpl()
{
    m_pBMFile = NULL;
    m_uFileCount = 0;
    m_uCurFile = 0;
	memset(m_szErrMsg,0,sizeof(m_szErrMsg));

}

CBMFileImpl::~CBMFileImpl()
{
	if(NULL != m_pBMFile)
	{
		delete []m_pBMFile;
		m_pBMFile = NULL;
	}
}

DWORD CBMFileImpl::GetCurCodeBase()
{
    return (DWORD)m_curBMFileInfo.llBase;
}

unsigned __int64 CBMFileImpl::GetCurCodeBaseEx()
{
    return m_curBMFileInfo.llBase;
}

void CBMFileImpl::SetCurCodeBase( DWORD dwCodeBase)
{
    m_curBMFileInfo.llBase = dwCodeBase;
}

void CBMFileImpl::SetCurCodeBaseEx( unsigned __int64 llCodeBase)
{
    m_curBMFileInfo.llBase = llCodeBase;
}

DWORD CBMFileImpl::GetCurOprSize()
{
    return (DWORD)m_curBMFileInfo.llOprSize;
}
unsigned __int64 CBMFileImpl::GetCurOprSizeEx()
{
    return m_curBMFileInfo.llOprSize;
}
void CBMFileImpl::SetCurOprSize( DWORD dwOprSize )
{
    m_curBMFileInfo.llOprSize = dwOprSize;
}

void CBMFileImpl::SetCurOprSizeEx( unsigned __int64 llOprSize )
{
    m_curBMFileInfo.llOprSize = llOprSize;
}

DWORD CBMFileImpl::GetCurMaxLength()
{
    return m_curBMFileInfo.dwMaxLength;
}

void CBMFileImpl::SetCurMaxLength( DWORD dwMaxLength )
{
    m_curBMFileInfo.dwMaxLength = dwMaxLength;
}

const _TCHAR * CBMFileImpl::GetCurFileType()
{
    return m_curBMFileInfo.szFileType;
}

void CBMFileImpl::SetCurFileType(const _TCHAR* lpszFileType)
{
	if(lpszFileType == NULL)
		return;

	_tcscpy(m_curBMFileInfo.szFileType,lpszFileType);
}

BOOL CBMFileImpl::GetCurIsChangeCode()
{
    return m_curBMFileInfo.bChangeCode;
}

BOOL CBMFileImpl::GetCurIsLoadFromFile()
{
    return m_curBMFileInfo.bLoadCodeFromFile;
}

DWORD CBMFileImpl::GetCurCodeSize()
{
    return (DWORD)m_curBMFileInfo.llCodeSize;
}
unsigned __int64 CBMFileImpl::GetCurCodeSizeEx()
{
    return m_curBMFileInfo.llCodeSize;
}
const LPVOID CBMFileImpl::GetCurCode()
{
    return m_curBMFileInfo.lpCode;
}

BOOL CBMFileImpl::SetCurCode( const LPVOID lpCode, DWORD dwCodeSize,
							 DWORD   dwFirstMapSize /*= 0*/,
							 HANDLE  hFile /*= INVALID_HANDLE_VALUE*/,
							 HANDLE  hFileMapView /*= INVALID_HANDLE_VALUE*/)
{
    if( NULL == lpCode )
    {
        // Invalid parameter
        return FALSE;
    }

    if( m_curBMFileInfo.bChangeCode )
    {
        if( m_curBMFileInfo.lpCode != NULL && dwCodeSize != m_curBMFileInfo.llCodeSize )
        { 
			SAFE_DELETE_ARRAY(m_curBMFileInfo.lpCode);
        }
        if( m_curBMFileInfo.lpCode == NULL )
        {
            m_curBMFileInfo.lpCode = new BYTE[ dwCodeSize ];
        }
        if( m_curBMFileInfo.lpCode == NULL )
        {
            return FALSE;
        }
        m_curBMFileInfo.llCodeSize = dwCodeSize;
        memcpy( m_curBMFileInfo.lpCode, lpCode, dwCodeSize );
    }
    else
    {
        m_curBMFileInfo.lpCode = lpCode;
        m_curBMFileInfo.llCodeSize = dwCodeSize;
		if(dwFirstMapSize == 0)
		{
			m_curBMFileInfo.dwFirstMapSize = dwCodeSize;
		}
		else
		{
			m_curBMFileInfo.dwFirstMapSize = dwFirstMapSize;
		}

		if(hFile != INVALID_HANDLE_VALUE)
		{
			m_curBMFileInfo.hFDLCode = hFile;
		}

		if(hFileMapView != INVALID_HANDLE_VALUE)
		{
			m_curBMFileInfo.hFDLCodeMapView = hFileMapView;
		}
		
    }
    return TRUE;
}

const TCHAR* CBMFileImpl::GetCurFileName()
{
    return m_curBMFileInfo.szFileName;
}

BOOL CBMFileImpl::SetCurFileName( const _TCHAR* lpszFileName, unsigned __int64 llSize /*= 0*/, unsigned __int64 llOffset/* = 0*/ )
{
    if( m_curBMFileInfo.bLoadCodeFromFile )
    {
        UnloadBMFileInfo( &m_curBMFileInfo );
    }
    m_curBMFileInfo.bLoadCodeFromFile = TRUE;
    _tcscpy(m_curBMFileInfo.szFileName, lpszFileName);
	m_curBMFileInfo.llCodeSize		= llSize;
	m_curBMFileInfo.llCodeOffset	= llOffset; 
    m_curBMFileInfo.bNeedCloseFile	= LoadBMFileInfo( &m_curBMFileInfo );
    return m_curBMFileInfo.bNeedCloseFile;
}

BOOL CBMFileImpl::InitBMFiles( PBMFileInfo lpBMFileInfo, UINT uFileCount  )
{
	memset(m_szErrMsg,0,sizeof(m_szErrMsg));
    m_uFileCount = uFileCount;
    m_pBMFile = new BMFileInfo[uFileCount];
    if( NULL == m_pBMFile  )
    {
		_stprintf(m_szErrMsg,_T("InitBMFiles: memory is not enough."));
        return FALSE;
    }
    memset(m_pBMFile,0,uFileCount * sizeof( BMFileInfo ));
    memcpy( m_pBMFile, lpBMFileInfo, uFileCount * sizeof( BMFileInfo ) );
    
    // Open file mapping objects
    for(UINT i=0; i < m_uFileCount; i++)
    {
        if( !LoadBMFileInfo( &m_pBMFile[i] ) )
        {
            ClearUpBMFiles();
            return FALSE;
        }
    }
    
    return TRUE;
}

void CBMFileImpl::ClearUpBMFiles()
{
    if( m_pBMFile == NULL )
        return;
    
    for(UINT i = 0; i < m_uFileCount ; i++ )
    {
        UnloadBMFileInfo( &m_pBMFile[i] );
    }
    m_uFileCount = 0;
    SAFE_DELETE_ARRAY(m_pBMFile);

    ClearCurBMFileInfo();
}

void CBMFileImpl::MoveFirst()
{
    m_uCurFile = 0;
    SetCurBMFileInfo();
}

void CBMFileImpl::MoveLast()
{
    m_uCurFile = m_uFileCount - 1;
    SetCurBMFileInfo();
}

void CBMFileImpl::MovePrev()
{
    --m_uCurFile;
    SetCurBMFileInfo();
}

void CBMFileImpl::MoveNext()
{
    ++m_uCurFile;
    SetCurBMFileInfo();
}

BOOL CBMFileImpl::IsEOF()
{
    return m_uCurFile == m_uFileCount;
}

UINT CBMFileImpl::GetCurFileIndex()
{
    return m_uCurFile;
}

BMFileInfo* CBMFileImpl::GetCurrentBMFileInfo()
{
    return &m_curBMFileInfo;
}

void CBMFileImpl::SetCurBMFileInfo()
{
    if( IsEOF() )
    {
        return;
    }
    ClearCurBMFileInfo();
    
    memcpy( &m_curBMFileInfo, &m_pBMFile[m_uCurFile], sizeof( BMFileInfo ));

    if( m_curBMFileInfo.bChangeCode && m_pBMFile[m_uCurFile].lpCode != NULL )
    {
        DWORD dwCodeSize = (DWORD)m_curBMFileInfo.llCodeSize;
        m_curBMFileInfo.lpCode = new BYTE[ dwCodeSize ];
        if( m_curBMFileInfo.lpCode == NULL )
        {
            m_curBMFileInfo.llCodeSize = 0;
        }
        else
        {
            memcpy( m_curBMFileInfo.lpCode , m_pBMFile[m_uCurFile].lpCode, dwCodeSize );
        }
    }
}

void CBMFileImpl::ClearCurBMFileInfo()
{
    if( m_curBMFileInfo.bChangeCode && m_curBMFileInfo.lpCode != NULL )
    {
        SAFE_DELETE_ARRAY(m_curBMFileInfo.lpCode);
    }
    if (m_curBMFileInfo.bNeedCloseFile)
    {
        SAFE_CLOSE_HANDLE(m_curBMFileInfo.hFDLCode);
		m_curBMFileInfo.bNeedCloseFile = FALSE;
    }

    memset( &m_curBMFileInfo, 0, sizeof( BMFileInfo ));
    
}

BOOL CBMFileImpl::LoadBMFileInfo( PBMFileInfo lpBMFileInfo )
{
	if( lpBMFileInfo == NULL )
		return FALSE;

	if( FALSE == lpBMFileInfo->bLoadCodeFromFile )
	{
		return TRUE;
	}

	BOOL bRet		= FALSE;
	CString strFDLFileName;
	HANDLE hFDLCode = NULL;

	do 
	{
		GetAbsolutePath( strFDLFileName,(LPCTSTR)lpBMFileInfo->szFileName );
		if( strFDLFileName.IsEmpty() )
		{
			_stprintf(m_szErrMsg,_T("LoadBMFileInfo: BMFile [%s] is empty."),lpBMFileInfo->szFileID);
			break;
		}

		hFDLCode = CreateFile(strFDLFileName.operator LPCTSTR (),
			GENERIC_READ,
			FILE_SHARE_READ,           //Exclusive Open
			NULL,                      //Can't Be inherited
			OPEN_EXISTING,             //If not existing then failed
			FILE_ATTRIBUTE_READONLY,   //Read Only
			NULL);
		if( hFDLCode == INVALID_HANDLE_VALUE)
		{   
			hFDLCode = NULL;
			_stprintf(m_szErrMsg,_T("LoadBMFileInfo: BMFile [%s] open failed, [ERR:0x%X]."),
				lpBMFileInfo->szFileName, GetLastError());
			break;
		}

        unsigned __int64 llCodeSize	= 0;
        LARGE_INTEGER liFileSize;
        liFileSize.HighPart = liFileSize.LowPart = 0;

		if( !GetFileSizeEx(hFDLCode,&liFileSize))
		{
			_stprintf(m_szErrMsg,_T("LoadBMFileInfo: GetFileSize [%s] failed, [ERR:0x%X]."),
				lpBMFileInfo->szFileName, GetLastError());
			break;
		}
        llCodeSize = liFileSize.HighPart ;
        llCodeSize = llCodeSize<<32;
        llCodeSize |= liFileSize.LowPart;
		if ( 0 == lpBMFileInfo->llCodeSize)
		{
			lpBMFileInfo->llCodeSize = llCodeSize;
		}
		else
		{
			llCodeSize = lpBMFileInfo->llCodeSize;
		}
		
		if( lpBMFileInfo->bChangeCode )
		{
			lpBMFileInfo->lpCode = new BYTE[ (unsigned int)llCodeSize ];	
			if(lpBMFileInfo->lpCode == NULL)
			{
				_tcscpy(m_szErrMsg,_T("LoadBMFileInfo: memory is not enough."));
				break;
			}
			if (lpBMFileInfo->llCodeOffset)
			{
				LARGE_INTEGER liOffset;
				liOffset.QuadPart = lpBMFileInfo->llCodeOffset;
				if (::SetFilePointer(hFDLCode,liOffset.LowPart, &liOffset.HighPart,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
				{
					_stprintf(m_szErrMsg,_T("SetFilePointer failed in LoadBMFileInfo, ErrorCode=0x%x\n"),GetLastError());	
					break;
				}
			}
			DWORD dwReaded = 0;
			if(!::ReadFile(hFDLCode,lpBMFileInfo->lpCode,(DWORD)llCodeSize,&dwReaded,NULL) || dwReaded != llCodeSize)
			{
				break;
			}
			lpBMFileInfo->dwFirstMapSize	= (DWORD)llCodeSize;
			SAFE_CLOSE_HANDLE(hFDLCode);
		}
		else
		{
			lpBMFileInfo->dwFirstMapSize	= 0;
			lpBMFileInfo->hFDLCode			= hFDLCode;
		}

		bRet = TRUE;

	} while (0);

	if (!bRet)
	{

		SAFE_DELETE_ARRAY(lpBMFileInfo->lpCode);
		SAFE_CLOSE_HANDLE(hFDLCode);
	}

	return bRet;    
}
void CBMFileImpl::UnloadBMFileInfo( PBMFileInfo lpBMFileInfo )
{
    if( lpBMFileInfo == NULL )
        return;
    if( lpBMFileInfo->bLoadCodeFromFile )
    {
        SAFE_DELETE_ARRAY(lpBMFileInfo->lpCode);
        SAFE_CLOSE_HANDLE(lpBMFileInfo->hFDLCode);
    }   
}

BOOL CBMFileImpl::SetCurBMFileInfo(LPCTSTR lpszFileID)
{
	UINT i=0;
	for(i = 0; i<m_uFileCount; i++)
	{
		CString strID = m_pBMFile[i].szFileID;
		if(strID.CompareNoCase(lpszFileID) == 0)
		{
			break;
		}
	}
	if(i>=m_uFileCount)
	{
		return FALSE;
	}
	m_uCurFile = i;

    ClearCurBMFileInfo();
    memcpy( &m_curBMFileInfo, &m_pBMFile[m_uCurFile], sizeof( BMFileInfo ));

    if( m_curBMFileInfo.bChangeCode && m_pBMFile[m_uCurFile].lpCode != NULL )
    {
        DWORD dwCodeSize = (DWORD)m_curBMFileInfo.llCodeSize;
        m_curBMFileInfo.lpCode = new BYTE[ dwCodeSize ];
        if( m_curBMFileInfo.lpCode == NULL )
        {
            m_curBMFileInfo.llCodeSize = 0;
        }
        else
        {
            memcpy( m_curBMFileInfo.lpCode , m_pBMFile[m_uCurFile].lpCode, dwCodeSize );
        }
    }
	return TRUE;
}

LPTSTR CBMFileImpl::GetLastErrMsg()
{
	return m_szErrMsg;
}