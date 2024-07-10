// File Name:   IPort.h 
// Author:      anli.wei
// Date:        2004-09-20
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.
// Description: ���������ֻ��Ľӿڣ���Щ�ӿڿ�����"���ڡ������USB"��Ŀǰ
//              ֻʵ�ִ��ڡ�������Щ��������Դ��ڵģ��ں����USB��ʵ����
//              ����Щ�ӿ�ʵ��Ϊ�ռ��ɡ�
#if !defined(__IPORT_H__)
#define __IPORT_H__

#pragma once

#ifndef _TCHAR

#ifdef _UNICODE
#define _TCHAR wchar_t //lint !e652
#else
#define _TCHAR char
#endif
#endif // _TCHAR

typedef enum PORT_TYPE
{
    PORT_TYPE_NONE     = 0,
	PORT_TYPE_COM      = 1,
	PORT_TYPE_USB      = 2,
	PORT_TYPE_INFRARED = 3,
};

const int PORT_NAME_MAX_LEN = 128;

// �˿ڵ�����(Ϊ�˽������ܵ����䣬���ֽӿڵ��ȶ���)
typedef struct _PORT_ATTRIBUTE
{
    PORT_TYPE PortType;

    union {
        // ComPort
        struct {
            _TCHAR szName[PORT_NAME_MAX_LEN];
            DWORD  dwBaudRate;
        } Com;

        // USB
        struct {
        } Usb;

        // Infrared
        struct {
        } Infrared;
    };

} PORT_ATTRIBUTE, * PPORT_ATTRIBUTE;
typedef const PPORT_ATTRIBUTE PCPORT_ATTRIBUTE;

// ʵ���ֻ�ͬpc�Ľӿڣ��������ڡ�usb�ͺ��������Ҫʵ�ִ˽ӿ�
class IPort 
{
public:

    // ����log���ܣ��������ԣ���¼ʧ�ܼ���д��Ϣ
    virtual BOOL    StartLog( LPCTSTR pszFileName ) = 0;
    virtual void    StopLog( void )                 = 0;

    // ��ȡ���ö˿ڵ�����
	virtual int     GetUseableCount(void) = 0;
    virtual int     GetPortCount(void)    = 0;
    // ��ȡָ�������Ķ˿�����
    virtual LPCTSTR GetName(int nIdx)     = 0;

	virtual BOOL Open(PCPORT_ATTRIBUTE pAttr)        = 0;
    virtual BOOL GetAttribute(PPORT_ATTRIBUTE pAttr) = 0;

    // [[ special fun for comport
    // ���û����DTR
    // bSignal : TRUE ����DTR, FALSE ���DTR
	virtual BOOL SetDTR( BOOL bSignal )           = 0;
    // ��ȡDSR��״̬
	virtual BOOL GetDSR( BOOL *pSignal )          = 0;

    // purge input or output
	virtual BOOL PurgeAll(void)                   = 0;
	virtual BOOL PurgeInput(void)                 = 0;
	virtual BOOL PurgeOutput(void)                = 0;
    // ]]

    virtual BOOL IsOpened(void) const             = 0;
    
	// if dwTimeOut is INFINITE means read operate never return until 
	// read something therefor i give a CancelRead function to end read oper.
	virtual BOOL Read( LPVOID  pBuf,  
                       DWORD   dwBytesToRead,  
		               LPDWORD pReadedBytes  = NULL, 
                       DWORD   dwTimeOut     = 200
                     ) = 0;

    // �ӽ��ջ������ж�ȡ����
    virtual BOOL QuickRead( LPVOID  pBuf,  
                            DWORD   dwBytesToRead,  
		                    LPDWORD pReadedBytes  = NULL
                          ) = 0;
	
    virtual void CancelRead() = 0;

    virtual BOOL Write( LPVOID  pBuf, 
                        DWORD   dwBytesToWrite, 
		                LPDWORD pWritedBytes   = NULL 
                      ) = 0;

	virtual LPVOID	WriteEx(LPCSTR lpCmd, DWORD dwTimeout, BOOL bPostAll) = 0;

    // �����Զ��������ݵ���Ϣ
    virtual void SetReceiver( DWORD dwMsgID, DWORD dwReceiver, BOOL bIsThread = TRUE ) = 0;
    virtual BOOL StartAutoReceive(HWND hWnd, LPVOID lpOpenArgs, DWORD  dwType, DWORD  dwMsgID) = 0;
    virtual void StopAutoReceive(void)  = 0;
    virtual void FreeMem( LPBYTE pBuf ) = 0;

    virtual void    Close(void)     = 0;

	virtual LPCTSTR GetErrMsg(void) = 0;

	virtual void Release(void)      = 0;

public:
    IPort() {};
    virtual ~IPort() {};
};

#ifdef  IPORT_EXPORTS
	#define IPORT_API extern "C" __declspec(dllexport)
#else
	#define IPORT_API extern "C" __declspec(dllimport)
#endif

IPORT_API BOOL CreatePort( IPort ** pPort, PORT_TYPE PortType );

#endif // __IPORT_H__
