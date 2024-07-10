// Uart.h: interface for the CUart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UART_H__DCC6631D_D760_453F_A62B_A63538F695D0__INCLUDED_)
#define AFX_UART_H__DCC6631D_D760_453F_A62B_A63538F695D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPort.h"
#include "ICommChannel.h"


static LPCSTR AsynCmd[] = 
{
		"+CSQ:",
		"+CLIP:",
		"+CMTI:",
		"+CGDCONT:",
		"+SPPSRATE:",
		"+SPFRQ:",
		"+SAUTOATT:",
		"+SPTEST:",
		"+SPID:",
		"+SPUPLMN:",
		"TMSI:",
		"IMEISV:",
		"+COPS:",
		"+XX:",
		"+CREG:",
		"+SIND:",
		"+CLCK:",
		"^SYSINFO:",
		"RING",
		"^MODE",
		"+CDS:",
		"NO CARRIER",
		"^SYSCONFIG",
		"+CMT:",
		"+ECEER:",
		//"+CMGL:",
#ifdef _EGO
		"OK",
#endif
};

#define MAX_BUF_LENGTH  0x800000
#define MAX_RCV_BUF_1 1024

class CUart : public IPort  
{
public:
	CUart();
	virtual ~CUart();
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

    virtual BOOL IsOpened(void) const;

	virtual BOOL Read( LPVOID pBuf,  DWORD dwBytesToRead,  
		               LPDWORD pReadedBytes = NULL, DWORD dwTimeOut = 200 );

    virtual BOOL QuickRead( LPVOID  pBuf,  
                            DWORD   dwBytesToRead,  
		                    LPDWORD pReadedBytes  = NULL
                          );

	virtual void CancelRead(void);

	virtual BOOL Write( LPVOID pBuf, DWORD dwBytesToWrite, 
		                LPDWORD pWritedBytes = NULL );
	virtual LPVOID	WriteEx(LPCSTR lpCmd, DWORD dwTimeout, BOOL bPostAll);

    virtual void SetReceiver( DWORD dwMsgID, DWORD dwReceiver, BOOL bIsThread = TRUE );
    virtual BOOL StartAutoReceive(HWND hWnd, LPVOID lpOpenArgs, DWORD  dwType, DWORD  dwMsgID);
    virtual void StopAutoReceive(void);
    virtual void FreeMem( LPBYTE pBuf );

	virtual LPCTSTR GetErrMsg(void);

	virtual void Close(void);
	
	virtual void Release(void);

private:
	ICommChannel * m_pChannel;
	PORT_ATTRIBUTE m_PortAttr; 
	BOOL m_bOpen;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
public:
	UINT Receive(
		void* pData,    //data pointer
		DWORD dwSize    //data block size
		);


protected:
	void ProcessData(void* pData, DWORD dwSize);
	BOOL IsIMEI(LPCSTR lpString);

	HANDLE CreateMsgThread();

    BOOL CreateScene(
        HWND hWnd      //handle of the gui window to receive the message
                    );
    BOOL DestroyScene();

	BOOL IsAsynRespond(LPCSTR lpRespond);
public:
	UINT m_dwMsgId;
	HANDLE m_hEventFlag;
private:
	char m_szRcvBuf[MAX_BUF_LENGTH];

	//for auto report
	UINT m_dwThreadId;

	UINT m_dwGuiMsgId;

    HWND m_hGuiWnd;

    HANDLE m_hThread;

	char m_szProData[MAX_RCV_BUF_1];
	char m_szPostData[MAX_RCV_BUF_1];
	DWORD m_dwProDataLen;
	
	BOOL m_bCMT;
	char m_szCMT[MAX_RCV_BUF_1];

	BOOL m_bRING;
	char m_szRING[MAX_RCV_BUF_1];
	
	//teana hu 2009.08.04
	BOOL m_bCDS;
	char m_szCDS[MAX_RCV_BUF_1];
	
	BOOL m_bLoadSMS;

	BOOL m_bSyn;
	HANDLE m_hEventWaitResult;
	DWORD m_dwReadTotal;
	DWORD m_dwRcvLen;

	BOOL m_bPostAll;

	

	static const int   MAX_RETRY_NUM;

};

#endif // !defined(AFX_UART_H__DCC6631D_D760_453F_A62B_A63538F695D0__INCLUDED_)
