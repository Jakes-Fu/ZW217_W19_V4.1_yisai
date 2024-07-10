// IEtion.h: interface
//
//////////////////////////////////////////////////////////////////////
#if !defined(_IETION_H_)
#define _IETION_H_

#pragma once

typedef enum
{
	CHECK_CONNECT,
	SIGNAL,
	NETWORK,
	AUTO_REPORT_START,
	AUTO_REPORT_STOP,
	HAND_SELECT_NETWORK,
	AUTO_SELECT_NETWORK,
	CHECK_NETWORK,
	CALL,
	HANG_UP,
	HOLD_ON,
	CHECK_PIN,
	SET_PIN,
	POWER_OFF,
	DIAL_TYPE_SET,
	CONT_IN_SET,
	SPEED_SET,
	SPEED_SET_NO_RETURN,
	SMS_SEND_MODE_CHECK,
	SMS_SEND_MODE_SET,
	PIN_CHECK,
	PUK2_REMAIN_NUM,
	PIN_SET,
	PIN_MODIFY,
	SYSTEM_CHECK,
	PIN_OR_PUK,
	PIN_INPUT,
	PUK_INPUT,
	PIN_REMAIN_NUM,
	OPEN_CMEE,
	OPEN_AUTO_PROMT,
	PIN2_MODIFY,
	PUK2_INPUT,
	FDN_SET,
	FDN_CHECK,
	UPLMN_CHECK,
	AOC_SET,
	UPLMN_SET,
	PIN2_REMAIN_NUM,
	PUK1_REMAIN_NUM,
	SYSTEM_SERVICE_CHECK,
	SYSTEM_SERVICE_AUTO,
	SPID_CHECK,
	CGDCONT_READ,
	CGDCONT_TEST,
	CGDCONT_SET,
	CGSN_CHECK,
	SRATE_READ,
	SRATE_TEST,
	SRATE_SET,
	SPFRQ_READ,
	SPFRQ_TEST,
	SPFRQ_SET,
	SAUTOATT_READ,
	SAUTOATT_SET,
	SPTEST_READ,
	SPTEST_SET,
	PS_DEPATCH,
	HSDPA_VERSION,
	SYSCONFIG_READ,
	SYSCONFIG_SET,
	VTS_SET,
	AT_CMD,
	DSP_SET,
	CFUN_CHECK,
	CFUN_SET
	
};

typedef enum
{
	AUTO_SIGNAL
};

class IPort;
class IEtion
{
public:
    virtual BOOL       StartLog( LPCTSTR pszFileName ) = 0;
	virtual void         StopLog( void )                 = 0;
	
    // 设置接口
    virtual void         SetPort( IPort * pPort, BOOL bMux = FALSE) = 0;
	virtual IPort *      GetPort( void ) = 0;
	
	//发送AT命令并接收反馈结果
	virtual LPVOID		SendATCmd(DWORD dwCmdId, DWORD dwCmdStep, LPVOID lpParam, DWORD dwReserve)	=0;

	virtual BOOL AutoReportStart(
        HWND   hWnd,               //handle of the window to receive message
        LPVOID lpOpenArgs,         // argument for open communicatin channel
		DWORD  dwType,
        DWORD  dwMsgID = 0
        ) = 0; 

	virtual BOOL AutoReportStop() = 0;
	
    // 获取操作失败的信息
    virtual DWORD        GetErrCode(void) const = 0;
	
	//释放资源
    virtual void         Release(void) = 0;
	
public:
	IEtion () {};
	virtual ~ IEtion () {};
};

#ifdef  ETION_EXPORTS
	#define ETION_API extern "C" __declspec(dllexport)
#else
	#define ETION_API extern "C" __declspec(dllimport)
#endif

ETION_API BOOL CreateEtion( IEtion ** pEtion );

#endif // _IETION_H_
