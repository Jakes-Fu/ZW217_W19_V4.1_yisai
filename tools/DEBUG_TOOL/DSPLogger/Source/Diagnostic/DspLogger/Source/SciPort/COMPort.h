// File Name:   ComPort.h 
// Author:      anli.wei
// Date:        2004-09-20
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.
// Description: 定义连接手机的接口之串口方式。在打开串口时采用了重叠操作，
//              但目前的读写是采用的阻塞方式，这里采用重叠操作是为了方便
//              将来可能需要的扩充.
#if !defined(AFX_COM_H__F4C264E1_99B5_4CD5_9D06_570C98F10B3C__INCLUDED_)
#define AFX_COM_H__F4C264E1_99B5_4CD5_9D06_570C98F10B3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPort.h"
#include "Log.h"

#include <deque>

class CCOMPort : public IPort
{
public:

    virtual BOOL  StartLog( LPCTSTR pszFileName );
    virtual void  StopLog( void );

    // 获取可用端口的数量
	virtual int     GetUseableCount(void);
    virtual int     GetPortCount(void);

    virtual LPCTSTR GetName(int nIdx);

	virtual BOOL Open(PCPORT_ATTRIBUTE pAttr);
	virtual BOOL GetAttribute(PPORT_ATTRIBUTE pAttr);

	virtual BOOL SetDTR( BOOL bSignal );
	virtual BOOL GetDSR( BOOL *pSignal );

	virtual BOOL PurgeAll(void);
	virtual BOOL PurgeInput(void);
	virtual BOOL PurgeOutput(void);

    virtual BOOL IsOpened(void) const { return m_hFile != INVALID_HANDLE_VALUE; }

	virtual BOOL Read( LPVOID pBuf,  DWORD dwBytesToRead,  
		               LPDWORD pReadedBytes = NULL, DWORD dwTimeOut = 200 );

    virtual BOOL QuickRead( LPVOID  pBuf,  
                            DWORD   dwBytesToRead,  
		                    LPDWORD pReadedBytes  = NULL
                          );

	virtual void CancelRead(void);

	virtual BOOL Write( LPVOID pBuf, DWORD dwBytesToWrite, 
		                LPDWORD pWritedBytes = NULL );

    virtual void SetReceiver( DWORD dwMsgID, DWORD dwReceiver, BOOL bIsThread = TRUE );
    virtual BOOL StartAutoReceive(void);
    virtual void StopAutoReceive(void);
    virtual void FreeMem( LPBYTE pBuf );

	virtual LPCTSTR GetErrMsg(void);

	virtual void Close(void);
	
	virtual void Release(void);

	virtual void OpenOrgFile(LPCTSTR lpFileName);

	virtual void CloseOrgFile(void);

	virtual void RenameOrgFile(LPCTSTR lpFileName);

	virtual void DeleteOrgFile();

	virtual void SetProperty(DWORD dwProperty);


	CCOMPort();
	virtual ~CCOMPort();

protected:
	BOOL Open( LPCTSTR pszName, DWORD dwBaudrate );

    int  GetPortCount(BOOL bUseable = TRUE);

    static DWORD WINAPI RcvThread(LPVOID pParam );
    DWORD  AutoRcvProc(void);

    __inline LPBYTE AllocMem(void);
    __inline BOOL	SendDataToReciver( LPCVOID pData, DWORD dwSize );

private:

    enum { 
        INVALID_MSG_ID       = 0,
        MAX_OVL_COUNT        = 3,
        MAX_ERRMSG_LEN       = 512, 
        MAX_COMPORT_NUM      = 128,
        MAX_COMPORT_NAME_LEN = 32,
    };

    HANDLE         m_hFile;
    OVERLAPPED     m_osWrite;
	OVERLAPPED     m_osRead;
    OVERLAPPED     m_osWait;
    OVERLAPPED *   m_arrOL[MAX_OVL_COUNT];

    int            m_nCount;
    _TCHAR         m_arrName[ MAX_COMPORT_NUM ][MAX_COMPORT_NAME_LEN];

    CLog           m_Log;

    PORT_ATTRIBUTE m_PortAttr; 

    volatile BOOL  m_bReceive;
    BOOL           m_bRcvIsThread;
    DWORD          m_dwMsgID;
    DWORD          m_dwRcvThread;
    HWND           m_hRcvWnd;
    HANDLE         m_hRcvProc;

    DWORD          m_dwAllocCount;
    std::deque<LPBYTE>m_dqBuf;

    CRITICAL_SECTION  m_csForBuf;

    static const int COM_INPUT_BUF_SIZE;
    static const int COM_OUTPUT_BUF_SIZE;
    static const int MAX_SEND_TRYS;

    static const int BUF_SIZE;
    static const int MAX_BUF_COUNT;
	static const int ALLOC_BUF_COUNT;
    static const int NORMAL_BUF_COUNT;

    static _TCHAR m_szErrMsg[MAX_ERRMSG_LEN];
	FILE* m_fpOrg;
	FILE* m_fpTPOrg;
	TCHAR m_szOrgFile[MAX_PATH];
	BOOL m_bGenerateOrgFile;
    
};

#endif // !defined(AFX_COM_H__F4C264E1_99B5_4CD5_9D06_570C98F10B3C__INCLUDED_)
