/*============================================================================*
 *
 *                          Copyright (c) 2002-2003 Spreadtrum
 *                              All Rights Reserved
 *
 *-----------------------------------------------------------------------------
 *
 * File name:
 *      TPSet.h
 *
 * Description:
 *      Test points set.
 *
 *============================================================================*/

#ifndef  _TPSET_H_
#define _TPSET_H_
#pragma warning(push,3)
#include <vector>
#pragma warning(pop)
#pragma warning( disable : 4663 4018 4245 4083 4710)
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <stdio.h>


#include "DspKernelExport.h"

#define TP_SIZE                         8
#define TP_DATA_FIELD_SIZE              4
#define TP_MAX_COUNT                    5000000
#define DEFAULT_FIND_DATA_NUM			4
#define MAX_FIND_DATA_NUM				20
//critical section class
class CAutoCS
{
public:
    CAutoCS( CRITICAL_SECTION &cs ) 
    { 
        m_pCS = &cs; 
        EnterCriticalSection( m_pCS ); 
    }

    ~CAutoCS( ) { LeaveCriticalSection( m_pCS ); }
private:
    CRITICAL_SECTION * m_pCS;
};


class CMassTPSet : public ITPSet
{
public:
    CMassTPSet();
    virtual ~CMassTPSet();//lint !e1510
    
public:
    virtual void    SetLogFilePath(char* lpszPath );	
	virtual void    SetLogFileName(char* pLogName );
    virtual void    SetMaxCount( ULONG lCount ){ m_lMaxCount = lCount; }
    virtual void    SaveRecent(BOOL bOnlySaveRct ){ m_bOnlySaveRct = bOnlySaveRct;}
    virtual ULONG   GetTpCount();
	inline  ULONG   GetTp(ULONG nIndex);	
	inline  WORD    GetTPAddr(ULONG nIndex);    
	inline  WORD    GetTPData(ULONG nIndex);    
	inline  DWORD   GetTPTime(ULONG nIndex);
    virtual void    Clear();  
	virtual LPCSTR  GetTpFlagDesc( int nFlag );
    virtual ULONG	Find(PTP_FIND_DATA pFindData, DWORD nNum, DWORD* pStartPos, BOOL *pbSign);  
    inline  ULONG	GetIndexByFindIdx( ULONG lIdx );
	virtual DWORD	GetColorIndexByFindIdx(ULONG lIdx);
	virtual BOOL	GetSignedByFindIdx(ULONG lIdx);
    virtual void	ClearFind(void) ;
    virtual ULONG   Find(ULONG nStart,char* pAddr,char* pData,int bUp,LPCTSTR pDesc);
    virtual ULONG   FindNext();  
    virtual ULONG   FindPre();
	virtual void    SetCurveNum(int nCurveNum);
	virtual void	SetStartPos(DWORD dwStartPos);
	virtual void	SetEndPos(DWORD dwEndPos);
	virtual void	SetFilePreFix(LPSTR lpPreFix);
	virtual LPSTR	GetTPFileName();
	virtual void	SetTPDirectory(LPSTR lpDirectory);
	virtual LPSTR	GetTPDirectory();
	virtual void WINAPI  SetCBFunc(PFCALLBACK Func);
	virtual void	SetLogSign(LPSTR lpSign); 
	void			AddTp(BYTE* pBuf,DWORD dwTime);
	BOOL			Load(LPCTSTR lpszFileName);
	void			Close();	
	BOOL			CombineFile();	
	void			ClearPos();	
	void			SetGuiHwnd(HWND hWnd);	
	void			SetStartTime(DWORD dwTime);

public:  
	BOOL    m_bOnlySaveRct;	
private:
	void	AddFlagTime(BOOL bStart);
	DWORD   GetTPBody(ULONG nIndex);    
protected:
	void	GenerateFileName();	 
	BOOL	InitForWrite();
			 
	void	CalculateMask(char* pAddr,char* pData,LPCTSTR pDesc);
	ULONG	Search();
			 
	BOOL	CheckWritePointer();
	BOOL	CheckReadPointer(ULONG nIndex);
private:
	CRITICAL_SECTION m_CS;
	CRITICAL_SECTION m_CSRead;

    HANDLE	m_hSwapFile;
    HANDLE	m_hSwapMap;
	
    volatile ULONG	m_lTPCount;
    ULONG	m_lMaxCount;
    ULONG	m_lFindCount;

    std::vector<DWORD>	m_vecFindIdx;
	std::vector<DWORD>	m_vecColorIdx;
	std::vector<BOOL>	m_vecSigned;

    const BYTE* m_lpWrite;
    BYTE*		m_lpCurWrite;
    volatile BYTE*		m_lpRead;       

	char    m_szTPName[_MAX_PATH];// tp file name user set
    char	m_szTPFile[_MAX_PATH];// current tp file name
    char	m_szTPPath[_MAX_PATH];// dir path of tp files

	//teana hu 2009.10.26
	char	m_szTPDirectory[MAX_PATH];
	char    m_szTPDirectoryBak[MAX_PATH];


	char    m_szOverTimeTPFile[_MAX_PATH];
	char	m_szLogSign[MAX_PATH];

    // Used for search
    DWORD	m_dwSearchMask;
    DWORD	m_dwSearchData;
	TCHAR	m_szSearchDesc[MAX_PATH];
    BOOL	m_bSearchUp;
    ULONG	m_lSearchCur;
	int		m_nCurveNum;
	DWORD	m_dwStartPos;
	DWORD	m_dwEndPos;
	DWORD	m_dwStartTime;
	
	HWND	m_hGuiWnd;
	LPSTR	m_lpPreFix;

	//teana hu 2009.11.20
	PFCALLBACK m_CallBack;	

    BOOL InitForRead(LPCTSTR szFilePath);
	HANDLE	m_hRdFile;
    HANDLE	m_hRdMap;
	
};

#endif //_TPSET_H_
