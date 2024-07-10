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

// ���ڱ�ʾ�ص�ʱ�ķ��ͷ�ʽ�����û��ע����Ϣ�����Բ��趨
typedef union
{
    HWND   hClientWnd;    // Used to inform the client window
    DWORD  dwThreadID;    // Used to inform the client thread ID
} CLIENT_ID_T;

// �ͻ��˵���Ϣ
typedef struct 
{
    CALLBACKFLAG  CallBackFlag;   // Indicate the receive function is a thread or a window
                                  // procedure. WINDOW_CALLBACK,THREAD_CALLBACK
    CLIENT_ID_T   ClientID;       // Indicate the call back addr pointer.
                                  // ���������������Ϣ���������
} CLIENT_INFO_T, *pCLIENT_INFO;

// �������˵���Ϣ
typedef struct 
{
    char    IPAddr[16];      // IP address
    DWORD   Port;            // Port Number
} SERVER_INFO_T, *pSERVER_INFO;

typedef struct
{
    WORD   seq_num;       // ���к� ��������д
    WORD   dummy;         // ���ڲ��룬Ϊ�Ժ�Ԥ��
    WORD   wSize;         // ����֡�ĳ��� 
    BYTE   bType;         // ��������
    BYTE   bSubtype;      // ����������
} DIAG_MSG_HEAD_T, *pDIAG_MSG_HEAD;

typedef struct
{
    BYTE  bType;          // ������Ϣ������
    BYTE  bSubtype;       // ������Ϣ��������
    WORD  dwSize;         // ������Ϣ�ĳ���
    void  *lpData;        // ������Ϣ��ָ��
} DIAG_DATA, *pDIAG_DATA;

#define WM_DIAG_SEND_DATA   WM_SCM_SENDDATA
#define WM_DIAG_CLOSE       WM_SCM_CLOSE

typedef enum
{
    DIAG_SUCCESS,
    DIAG_TIMEOUT,
    DIAG_SOCKET_ERROR,
    DIAG_NOT_CONNECTED,
    DIAG_MCU_MODE_ERR,  // ���ʷ�ʽ����õ�ַ�������ô˷�ʽ����
    DIAG_MCU_ADDR_ERR,  // ��ַ�������WORD��ʽ���ʣ�����ַû����˫�ֽڶ���
                        //             ��DWORD��ʽ���ʣ�����ַû�������ֽڶ���
    DIAG_MCU_SIZE_ERR,  // ����̫�󣬳���DIAG_ACCESS_MCU_MAX_LENGTH��1000���ֽ�
    DIAG_MCU_RANGE_ERR  // ��ʼ��ַ�ͳ��ȶ���ȷ����������ַ�������ô˷�ʽ����
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
        pCLIENT_INFO lpClientInfo,  // �ͻ��˵���Ϣ�����������������Ϣ�������ÿ�
        pSERVER_INFO lpServerInfo   // �������˵���Ϣ������IP��ַ���˿ں�
        );
    void Close();
    
    void  DestroyData(
        pDIAG_DATA lpData           // �ͷŴ�DLL�з������Ϣ
        );  
    
    BOOL  RegMsg(
        unsigned char chType        // Ҫע���������Ϣ������
        );

    BOOL  UnregMsg(
        unsigned char chType        // Ҫȡ��ע���������Ϣ������
        );
    
    DWORD CDiag::Send(
        unsigned char Type,                // ��������
        unsigned char Subtype,             // ����������
        void          *lpBuffer = NULL,    // ��Ϣ��
        DWORD         dwSize = 0,          // ��Ϣ�峤�ȣ���λ�ֽ� 
        pDIAG_DATA    *lpDestData = NULL,  // ���ص���Ӧ��Ϣ��Ϊ����û��
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
        
    // ��MS����
    DWORD PowerOnMS();                 
    // ��MS�ػ�
    DWORD PowerOffMS();                
    // ��MS��һ����
    DWORD PressMSKey(
        unsigned long key,
        unsigned int key_type = SOFTKEY_UP_AND_DOWN  // Up or down
        );  
    // ����һ��AT����
    DWORD SendAtCmd(
        const char  *lpszAtCmd,        // ��Ϣ��
        pDIAG_DATA  *lpResponse = NULL // ���ص���Ӧ��Ϣ��Ϊ����û��
        );

    // ����һ��SIM����
    DWORD SendSimCmd(
        const void  *lpszSimCmd,       // ��Ϣ��
        DWORD       dwSize,            // ��Ϣ��С
        pDIAG_DATA  *lpResponse = NULL // ���ص���Ӧ��Ϣ��Ϊ����û��
        );

    void operator()( PCOMMONDATA & data )
    {
        DestroyData( &data );
    }

private:
    CLIENT_INFO_T ClientInfo;       // �ͻ��˵���Ϣ
 //   HANDLE        m_SocketEvent;    // �����¼�
    DWORD         m_ThreadID;       // ���յ��̵߳�ID
    HANDLE        m_hThread;        // �߳̾��
//    PCOMMONDATA   m_pData;          // ���յ������ݽṹ
    HANDLE        m_InternalEvent;  // ����ȷ���߳����Ƚ�����¼�
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
