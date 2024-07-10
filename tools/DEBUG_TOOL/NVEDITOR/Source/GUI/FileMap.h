// FileMap.h: interface for the CFileMap class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FILEMAP_H
#define _FILEMAP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileMap  
{
public:
	CFileMap( LPCTSTR lpszFilePath );
	virtual ~CFileMap();

	operator const void * ( void );
	DWORD GetSize( void );

protected:
	void ClearUp( void );
    void CreateMap( void );

private:

	TCHAR  m_szFilePath[ MAX_PATH ];
	HANDLE m_hFile;
	HANDLE m_hFileMap;
	LPVOID m_pvFile;
	DWORD  m_cbFileSize;

};

#endif // _FILEMAP_H
