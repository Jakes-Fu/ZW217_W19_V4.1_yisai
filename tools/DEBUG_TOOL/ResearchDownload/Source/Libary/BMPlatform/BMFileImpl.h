// BMFileImpl.h: interface for the CBMFileImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMFILEIMPL_H__7A9980FC_26C6_4F89_B537_89D64B8C7DD9__INCLUDED_)
#define AFX_BMFILEIMPL_H__7A9980FC_26C6_4F89_B537_89D64B8C7DD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BMFile.h"
#include "Global.h"

class CBMFileImpl : public IBMFile  
{
public:
	CBMFileImpl();
	virtual ~CBMFileImpl();

//IBMFile Implemention
public:
    virtual DWORD GetCurCodeBase();
    virtual unsigned __int64 GetCurCodeBaseEx();

    virtual void SetCurCodeBase( DWORD dwCodeBase);
    virtual void SetCurCodeBaseEx( unsigned __int64 llCodeBase);

    virtual DWORD GetCurOprSize();
    virtual unsigned __int64 GetCurOprSizeEx();

    virtual void SetCurOprSize( DWORD dwOprSize );
    virtual void SetCurOprSizeEx( unsigned __int64 llOprSize );

    virtual DWORD GetCurCodeSize();
    virtual unsigned __int64 GetCurCodeSizeEx();

    virtual DWORD GetCurMaxLength();
    virtual void SetCurMaxLength( DWORD dwMaxLength );
    virtual const _TCHAR* GetCurFileType();
    virtual BOOL GetCurIsChangeCode();
    virtual BOOL GetCurIsLoadFromFile();    
    virtual const LPVOID GetCurCode();
    virtual BOOL SetCurCode( const LPVOID lpCode, DWORD dwCodeSize, 
							DWORD   dwFirstMapSize = 0,
							HANDLE  hFile = INVALID_HANDLE_VALUE,
							HANDLE  hFileMapView = INVALID_HANDLE_VALUE);

    virtual const _TCHAR* GetCurFileName();
    virtual BOOL SetCurFileName(const _TCHAR* lpszFileName, unsigned __int64 llSize = 0, unsigned __int64 llOffset = 0);
	virtual void SetCurFileType(const _TCHAR* lpszFileType);
    
public:
    BOOL InitBMFiles( PBMFileInfo lpBMFileInfo, UINT uFileCount );    
    void ClearUpBMFiles();
    BMFileInfo* GetCurrentBMFileInfo();

    UINT GetCurFileIndex();
    
    void MovePrev();
    void MoveNext();
    void MoveFirst();
    void MoveLast();
    BOOL IsEOF();
	
	BOOL SetCurBMFileInfo(LPCTSTR lpszFileID);

	LPTSTR GetLastErrMsg();

// Operations
private:
    BOOL LoadBMFileInfo( PBMFileInfo lpBMFileInfo );
    void UnloadBMFileInfo( PBMFileInfo lpBMFileInfo  );

    void SetCurBMFileInfo();
    void ClearCurBMFileInfo();
    
// Attributes
private:
    BMFileInfo*         m_pBMFile;
    UINT                m_uFileCount;
    UINT                m_uCurFile;
    BMFileInfo          m_curBMFileInfo;
	_TCHAR              m_szErrMsg[1024];
};

#endif // !defined(AFX_BMFILEIMPL_H__7A9980FC_26C6_4F89_B537_89D64B8C7DD9__INCLUDED_)
