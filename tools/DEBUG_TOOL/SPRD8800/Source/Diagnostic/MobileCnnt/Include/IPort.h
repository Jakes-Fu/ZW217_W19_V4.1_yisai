// File Name:   IPort.h 
// Author:      anli.wei
// Date:        2004-09-20
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.
// Description: 定义连接手机的接口，这些接口可能是"串口、红外或USB"，目前
//              只实现串口。其中有些函数是针对串口的，在红外或USB的实现中
//              将这些接口实现为空即可。
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

// 端口的属性(为了将来可能的扩充，保持接口的稳定性)
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

// 实现手机同pc的接口，操作串口、usb和红外的类需要实现此接口
class IPort 
{
public:

    // 启动log功能，辅助调试，记录失败及读写信息
    virtual BOOL    StartLog( LPCTSTR pszFileName ) = 0;
    virtual void    StopLog( void )                 = 0;

    // 获取可用端口的数量
	virtual int     GetUseableCount(void) = 0;
    virtual int     GetPortCount(void)    = 0;
    // 获取指定索引的端口名称
    virtual LPCTSTR GetName(int nIdx)     = 0;

	virtual BOOL Open(PCPORT_ATTRIBUTE pAttr)        = 0;
    virtual BOOL GetAttribute(PPORT_ATTRIBUTE pAttr) = 0;

    // [[ special fun for comport
    // 设置或清除DTR
    // bSignal : TRUE 设置DTR, FALSE 清除DTR
	virtual BOOL SetDTR( BOOL bSignal )           = 0;
    // 获取DSR的状态
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

    // 从接收缓冲区中读取数据
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

    // 设置自动接受数据的信息
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
