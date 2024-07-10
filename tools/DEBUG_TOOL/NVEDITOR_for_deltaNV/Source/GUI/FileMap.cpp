// FileMap.cpp: implementation of the CFileMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define LOCAL_EXCEPTION			13

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileMap::CFileMap( LPCTSTR lpszFilePath )
{
	ASSERT( NULL != lpszFilePath );

	m_hFile      = INVALID_HANDLE_VALUE;
	m_hFileMap   = NULL;
	m_pvFile     = NULL;
	m_cbFileSize = 0;

	::_tcscpy( m_szFilePath, lpszFilePath );
}

CFileMap::~CFileMap()
{
	ClearUp();	
}//lint !e1740 !e1551

void CFileMap::ClearUp( void )
{
	try
	{
		if ( NULL != m_pvFile )
		{
			VERIFY( ::UnmapViewOfFile( m_pvFile ) );
			m_pvFile = NULL;
		}

		if ( NULL != m_hFileMap )
		{
			VERIFY( ::CloseHandle( m_hFileMap ) );
			m_hFileMap = NULL;
		}

		if ( INVALID_HANDLE_VALUE != m_hFile )
		{
			VERIFY( ::CloseHandle( m_hFile ) );
			m_hFile = INVALID_HANDLE_VALUE;
		}

		m_cbFileSize = 0;
	}
	catch (...)
	{
		
	}	
}


CFileMap::operator const void * ( void )
{
	if ( NULL == m_pvFile )
		CreateMap();

	return m_pvFile;
}

		
void CFileMap::CreateMap( void )
{
    ASSERT( NULL == m_pvFile );

	try
	{
		m_hFile = ::CreateFile( m_szFilePath           ,
		                        GENERIC_READ           ,
								FILE_SHARE_READ        ,
								NULL                   ,
								OPEN_EXISTING          ,
								FILE_ATTRIBUTE_NORMAL  ,
								NULL                   );

		if ( INVALID_HANDLE_VALUE == m_hFile )
		{
			throw LOCAL_EXCEPTION;
		}
		
		m_cbFileSize = ::GetFileSize( m_hFile, NULL );

		m_hFileMap = ::CreateFileMapping( m_hFile,
										  NULL,
									      PAGE_READONLY,
									      0,
									      m_cbFileSize,
									      NULL );

		if ( NULL == m_hFileMap )
		{
			throw LOCAL_EXCEPTION;
		}

		m_pvFile = ::MapViewOfFile( m_hFileMap,
								    FILE_MAP_READ,
									0,
									0,
									0 );

		if ( NULL == m_pvFile )
		{
			throw LOCAL_EXCEPTION;
		}
	}
	catch ( int iReason )
	{
		if ( LOCAL_EXCEPTION == iReason )
		{
			ClearUp();
		}
		else
		{
			throw;
		}
	}
}

DWORD CFileMap::GetSize( void )
{
    if ( NULL == m_pvFile )
        CreateMap();

    return m_cbFileSize;
}