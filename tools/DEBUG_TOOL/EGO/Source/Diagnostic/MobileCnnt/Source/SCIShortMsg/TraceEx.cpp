
#include "stdafx.h"
#include "TraceEx.h"


HANDLE CTraceEx::m_hFile  = INVALID_HANDLE_VALUE;
int    CTraceEx::m_nCount = 0;

//////////////////////////////////////////////////////////////////////////////
CTraceEx::CTraceEx()
{
}

CTraceEx::~CTraceEx()
{
}

BOOL CTraceEx::Init( LPCTSTR pTraceFileName )
{
#ifndef TRACE_INFO_TO_FILE
		return TRUE;
#else
		if( INVALID_HANDLE_VALUE == m_hFile )
		{
			_ASSERTE( pTraceFileName != NULL );
			//teana hu 2009.03.02
			if(pTraceFileName == NULL)
			{
				return FALSE;
			}

			m_hFile = CreateFile( pTraceFileName, 
								  GENERIC_WRITE,
							      0,
							      NULL,
							      CREATE_ALWAYS,
							      FILE_ATTRIBUTE_NORMAL,
							      NULL );
		}
		BOOL bCreate = (m_hFile != INVALID_HANDLE_VALUE);
		if( bCreate )
		{
			_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE);
			_CrtSetReportFile( _CRT_WARN, m_hFile );
		}
        else
		{
			_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
			_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
		}

		m_nCount++;

		return bCreate;
#endif // TRACE_INFO_TO_FILE
}

void CTraceEx::Release(void)
{
	m_nCount--;
	if( INVALID_HANDLE_VALUE != m_hFile && 0 == m_nCount )
	{
		CloseHandle( m_hFile );

        m_hFile = INVALID_HANDLE_VALUE;
	}
}

LPCTSTR CTraceEx::GetTraceFileDir(void)
{
	static _TCHAR szFileName[_MAX_PATH];

	::GetModuleFileName(NULL, szFileName, _MAX_PATH);

	LPTSTR pExt = _tcsrchr(szFileName, _T('\\'));
	if( pExt != NULL )
	{
		*(pExt + 1) = 0;
	}
	else
	{
		_tcscpy( szFileName, _T("c:\\") );
	}

	return szFileName;
}