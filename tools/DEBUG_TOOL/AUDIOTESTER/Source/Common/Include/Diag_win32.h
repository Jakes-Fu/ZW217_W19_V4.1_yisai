// Diagnostic.h: interface for the CDiag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIAGNOSTIC_H__26414C6A_DD4F_40C3_B5C7_43110542FDE9__INCLUDED_)
#define AFX_DIAGNOSTIC_H__26414C6A_DD4F_40C3_B5C7_43110542FDE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DIAG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DIAG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef DIAG_EXPORTS
#define DIAG_API __declspec(dllexport)
#else
#define DIAG_API __declspec(dllimport)
#endif

#include "CommSock.h"
#include "comm_def.h"

#define DIAG_WAIT_TIME      3000

// 用于标示回调时的发送方式，如果没有注册消息，可以不设定
typedef union
{
    HWND   hClientWnd;    // Used to inform the client window
    DWORD  dwThreadID;    // Used to inform the client thread ID
} CLIENT_ID_T;

// 客户端的信息
typedef struct 
{
    CALLBACKFLAG  CallBackFlag;   // Indicate the receive function is a thread or a window
                                  // procedure. WINDOW_CALLBACK,THREAD_CALLBACK
    CLIENT_ID_T   ClientID;       // Indicate the call back addr pointer.
                                  // 如果不接收匿名消息，可以填空
} CLIENT_INFO_T, *pCLIENT_INFO;

// 服务器端的信息
typedef struct 
{
    char    IPAddr[16];      // IP address
    DWORD   Port;            // Port Number
} SERVER_INFO_T, *pSERVER_INFO;

typedef struct
{
    WORD   seq_num;       // 序列号 ，不用填写
    WORD   dummy;         // 用于补齐，为以后预留
    WORD   wSize;         // 整个帧的长度 
    BYTE   bType;         // 命令类型
    BYTE   bSubtype;      // 子命令类型
} DIAG_MSG_HEAD_T, *pDIAG_MSG_HEAD;

typedef struct
{
    BYTE  bType;          // 返回消息的类型
    BYTE  bSubtype;       // 返回消息的子类型
    WORD  dwSize;         // 返回消息的长度
    void  *lpData;        // 返回消息的指针
} DIAG_DATA, *pDIAG_DATA;

#define WM_DIAG_SEND_DATA   WM_SCM_SENDDATA
#define WM_DIAG_CLOSE       WM_SCM_CLOSE

typedef enum
{
    DIAG_SUCCESS,
    DIAG_TIMEOUT,
    DIAG_SOCKET_ERROR,
    DIAG_NOT_CONNECTED,
    DIAG_MCU_MODE_ERR,  // 访问方式错，或该地址不可以用此方式访问
    DIAG_MCU_ADDR_ERR,  // 地址对齐错，以WORD方式访问，但地址没有以双字节对齐
                        //             以DWORD方式访问，但地址没有以四字节对齐
    DIAG_MCU_SIZE_ERR,  // 长度太大，超过DIAG_ACCESS_MCU_MAX_LENGTH（1000）字节
    DIAG_MCU_RANGE_ERR  // 起始地址和长度都正确，但结束地址不可以用此方式访问
} DIAG_RETURN_VALUE_E;

enum
{
    SOFTKEY_UP_AND_DOWN = 0,
    SOFTKEY_DOWN,
    SOFTKEY_UP
};

#define DIAG_CREATE_MSG(msg_ptr, MSG_TYPE, type, subtype) \
msg_ptr           = (pDIAG_MSG_HEAD) malloc(sizeof(DIAG_MSG_HEAD_T) + sizeof(COMM_##MSG_TYPE##_T));\
msg_ptr->wSize    = sizeof(DIAG_MSG_HEAD_T) + sizeof(COMM_##MSG_TYPE##_T); \
msg_ptr->bType    = type;\
msg_ptr->bSubtype = subtype; \
((COMM_##MSG_TYPE##_PTR) (((pDIAG_MSG_HEAD )msg_ptr) + 1))->SignalCode = MSG_TYPE;\
((COMM_##MSG_TYPE##_PTR) (((pDIAG_MSG_HEAD )msg_ptr) + 1))->Size       = sizeof(COMM_##MSG_TYPE##_T);

#define DIAG_FREE_MSG(msg_ptr) \
free(msg_ptr);

template<class T> class CWaitableQueue;

class DIAG_API CDiag : public CCommSock
{
public:

    CDiag();
    virtual ~CDiag();        
    
    // Use this function to create a Diagnostic DLL.
    BOOL  Open(
        pCLIENT_INFO lpClientInfo,  // 客户端的信息，如果不接收匿名消息，可以置空
        pSERVER_INFO lpServerInfo   // 服务器端的信息，包括IP地址，端口号
        );
    void Close();
    
    void  DestroyData(
        pDIAG_DATA lpData           // 释放从DLL中分配的消息
        );  
    
    BOOL  RegMsg(
        unsigned char chType        // 要注册的匿名消息的类型
        );

    BOOL  UnregMsg(
        unsigned char chType        // 要取消注册的匿名消息的类型
        );
    
    DWORD CDiag::Send(
        unsigned char Type,                // 命令类型
        unsigned char Subtype,             // 子命令类型
        void          *lpBuffer = NULL,    // 消息体
        DWORD         dwSize = 0,          // 消息体长度，单位字节 
        pDIAG_DATA    *lpDestData = NULL,  // 返回的响应消息，为空则没有
		DWORD         timeOut = DIAG_WAIT_TIME,
		BOOL          isBLocked = FALSE
        );
    
    // Below is Direct chip test operation.
    DWORD CDiag::Write(
        DWORD  dwWriteMode,     // BYTE, WORD, DWORD
        DWORD  Addr,            // Address of the dest pointer
        void   *lpBuffer,       // Buffer to write 
		DWORD  dwSizeToWrite,   // In BYTE mode, means number of bytes
	    						// In WORD mode, means number of words
		    					// In DWORD mode, means number of dwords
		DWORD  timeOut   = DIAG_WAIT_TIME,
		BOOL   isBLocked = FALSE
        );
    DWORD CDiag::Read(
        DWORD dwReadMode,       // BYTE, WORD, DWORD
        DWORD Addr,             // Address of the dest pointer
        void *lpBuffer,         // Buffer to read
    	DWORD dwSizeToRead,		// In BYTE mode, means number of bytes
	    						// In WORD mode, means number of words
		    					// In DWORD mode, means number of dwords
		DWORD *lpdwSizeRead,    // In BYTE mode, means number of bytes
	    						// In WORD mode, means number of words
		    					// In DWORD mode, means number of dwords
		DWORD  timeOut   = DIAG_WAIT_TIME,
		BOOL   isBLocked = FALSE
        );
        
    // 让MS开机
    DWORD PowerOnMS();                 
    // 让MS关机
    DWORD PowerOffMS();                
    // 让MS按一个键
    DWORD PressMSKey(
        unsigned long key,
        unsigned int key_type = SOFTKEY_UP_AND_DOWN  // Up or down
        );  
    // 发送一条AT命令
    DWORD SendAtCmd(
        const char  *lpszAtCmd,        // 消息体
        pDIAG_DATA  *lpResponse = NULL // 返回的响应消息，为空则没有
        );

    // 发送一条SIM命令
    DWORD SendSimCmd(
        const void  *lpszSimCmd,       // 消息体
        DWORD       dwSize,            // 消息大小
        pDIAG_DATA  *lpResponse = NULL // 返回的响应消息，为空则没有
        );

    void operator()( PCOMMONDATA & data )
    {
        DestroyData( &data );
    }

private:
    CLIENT_INFO_T ClientInfo;       // 客户端的信息
 //   HANDLE        m_SocketEvent;    // 网络事件
    DWORD         m_ThreadID;       // 接收的线程的ID
    HANDLE        m_hThread;        // 线程句柄
//    PCOMMONDATA   m_pData;          // 接收到的数据结构
    HANDLE        m_InternalEvent;  // 用于确保线程能先进入的事件
    BOOL          m_IsInitOK;
    CWaitableQueue<PCOMMONDATA> * m_pRespQ;    

	FILE * m_fpLog;

private:
    // CMM thread body
   	DWORD         DiagThreadFunc(
        WPARAM wParam,
        LPARAM lParam
        );
	static DWORD  DiagThreadFuncEntry(
        LPVOID lpParam
        );
    DWORD         DiagWaitSocketEvent(DWORD timeOut);  
	DWORD         DiagWaitSingleSocketEvent(DWORD timeOut);
    // Can not be called from CDaig
    BOOL Init(
        PCALLBACKPARA pCallbackPara
        );
    BOOL Open(
        POPENPARA pOpenPara,
        BOOL bUsedTools = TRUE
        );
    BOOL Reset();
    BOOL Send(
        void   *lpData, 
        ULONG  DataSize,
        int    ClientID = 0, 
        int    MsgType = 1000
        );
    BOOL RegMsgType(
        UINT  iClientID,
        int   MsgType
        );
    BOOL UnRegMsgType(
        UINT  iClientID,
        int   MsgType
        );
    BOOL GetClientInfo(
        PWSHOSTINFO  pWsHostInfo, 
        int          *piHostNum
        );
    void DestroyData(
        PCOMMONDATA *ppData
        );
};

#endif // !defined(AFX_DIAGNOSTIC_H__26414C6A_DD4F_40C3_B5C7_43110542FDE9__INCLUDED_)
