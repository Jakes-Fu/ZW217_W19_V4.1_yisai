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
    virtual void SetCurCodeBase( DWORD dwCodeBase);
    virtual DWORD GetCurOprSize();
    virtual void SetCurOprSize( DWORD dwOprSize );
    virtual DWORD GetCurMaxLength();
    virtual void SetCurMaxLength( DWORD dwMaxLength );
    virtual const char* GetCurFileType();
    virtual BOOL GetCurIsChangeCode();
    virtual BOOL GetCurIsLoadFromFile();    
    virtual DWORD GetCurCodeSize();
    virtual const LPVOID GetCurCode();
    virtual void SetCurCode( const LPVOID lpCode, DWORD dwCodeSize );
    virtual const char* GetCurFileName();
    virtual void SetCurFileName(const char* lpszFileName, int nFileNameLen );
    
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
};

#endif // !defined(AFX_BMFILEIMPL_H__7A9980FC_26C6_4F89_B537_89D64B8C7DD9__INCLUDED_)
