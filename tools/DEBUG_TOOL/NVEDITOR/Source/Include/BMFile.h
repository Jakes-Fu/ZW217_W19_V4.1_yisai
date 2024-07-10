#ifndef  _BMFILE_H__
#define _BMFILE_H__

class IBMFile
{
public:
	virtual ~IBMFile()=0;
    virtual DWORD GetCurCodeBase() = 0;
    virtual void SetCurCodeBase( DWORD dwCodeBase) = 0;
    virtual DWORD GetCurOprSize() = 0;
    virtual void SetCurOprSize( DWORD dwOprSize ) = 0;
    virtual DWORD GetCurMaxLength() = 0;
    virtual void SetCurMaxLength( DWORD dwMaxLength ) = 0;
    virtual const _TCHAR* GetCurFileType() = 0;
    virtual BOOL GetCurIsChangeCode() = 0;
    virtual BOOL GetCurIsLoadFromFile() = 0;
    virtual DWORD GetCurCodeSize() = 0;
    virtual const LPVOID GetCurCode() = 0;	
    virtual BOOL SetCurCode( const LPVOID lpCode, DWORD dwCodeSize, 
							DWORD   dwFirstMapSize = 0,
							HANDLE  hFile = INVALID_HANDLE_VALUE,
							HANDLE  hFileMapView = INVALID_HANDLE_VALUE ) = 0;
    virtual const _TCHAR* GetCurFileName() = 0;
    virtual BOOL SetCurFileName(const _TCHAR* lpszFileName, int nFileNameLen  ) = 0;
	virtual void SetCurFileType(const _TCHAR* lpszFileType) = 0;
};

#endif //_BMFILE_H__