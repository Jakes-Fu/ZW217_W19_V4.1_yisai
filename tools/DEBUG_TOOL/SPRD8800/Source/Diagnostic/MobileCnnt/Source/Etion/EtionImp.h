// EtionImp.h: interface for the CEtionImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETIONIMP_H__FB51ED92_3BAE_4D68_BD45_9C0BEC696B0A__INCLUDED_)
#define AFX_ETIONIMP_H__FB51ED92_3BAE_4D68_BD45_9C0BEC696B0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IEtion.h"
#include "IPort.h"

//#define DEFAULT_DELAY_TIME 200
//#define DEFAULT_DELAY_TIME 999999999
#define DEFAULT_DELAY_TIME 500
#define MAX_RCV_BUF 500

enum{
	MAX_BUF_LEN = 1152,
};

class CEtionImp : public IEtion
{
public:
	virtual BOOL      StartLog( LPCTSTR pszFileName );
	virtual void      StopLog( void );
	
    virtual void      SetPort( IPort * pPort, BOOL bMux );
	virtual IPort *   GetPort( void );
	
	virtual LPVOID	  SendATCmd(DWORD dwCmdId, DWORD dwCmdStep, LPVOID lpParam, DWORD dwReserve = 0);

	virtual BOOL AutoReportStart(
        HWND   hWnd,               //handle of the window to receive message
        LPVOID lpOpenArgs,         // argument for open communicatin channel
		DWORD  dwType,
        DWORD  dwMsgID = 0
        ); 

	virtual BOOL AutoReportStop();
	
    virtual DWORD     GetErrCode(void) const;
	
    virtual void      Release(void);
public:
	CEtionImp();
	virtual ~CEtionImp();

protected:
	LPCSTR GetCmdString(DWORD dwCmdId, DWORD dwCmdStep, LPVOID lpParam, DWORD dwReserve);
	int GetDelayTime(DWORD dwCmdId);

private:
	IPort * m_pPortMain;
	IPort * m_pPortSub;
	char m_szCmd[MAX_PATH];
	
	BOOL  m_bUsedMux;


	static const int   MAX_RETRY_NUM;
	static const char AT_RES_OK[];
	static const char AT_RES_ERROR[];
	static const char AT_AT[];
	static const char AT_SIGNAL[];
	static const char AT_NETWORK[];
	static const char AT_AUTO_REPORT_START[];
	static const char AT_AUTO_REPORT_STOP[];
	static const char AT_HAND_SELECT_NETWORK[];
	static const char AT_AUTO_SELECT_NETWORK[];
	static const char AT_CALL[];
	static const char AT_HAND_UP[];
	static const char AT_HOLD_ON[];
	static const char AT_POWER_OFF[];
	static const char AT_DIAL_TYPE_SET[];
	static const char AT_CONT_IN_SET[];
	static const char AT_SPEED_SET[];
	static const char AT_SMS_SEND_CHECK[];
	static const char AT_SMS_SEND_SET[];
	static const char AT_PIN_SET[];
	static const char AT_PIN_CHECK[];
	static const char AT_PUK2_REMAIN_NUM[];
	static const char AT_PIN_MODIFY[];
	static const char AT_SYSTEM_CHECK[];
	static const char AT_PIN_OR_PUK[];
	static const char AT_PIN_INPUT[];
	static const char AT_PUK_INPUT[];
	static const char AT_PIN_REMAIN_NUM[];
	static const char AT_OPEN_CMEE[];
	static const char AT_OPEN_AUTO_PROMT[];
	static const char AT_CHECK_NETWORK[];
	static const char AT_PUK2_INPUT[];
	static const char AT_PIN2_MODIFY[];
	static const char AT_FDN_SET[];
	static const char AT_FDN_CHECK[];
	static const char AT_UPLMN_CHECK[];
	static const char AT_AOC_SET[];
	static const char AT_UPLMN_SET[];
	static const char AT_PIN2_REMAIN_NUM[];
	static const char AT_PUK1_REMAIN_NUM[];
	static const char AT_SYSTEM_SERVICE_CHECK[];
	static const char AT_SYSTEM_SERVICE_AUTO[];
	static const char AT_SPID_CHECK[];
	static const char AT_CGDCONT_READ[];
	static const char AT_CGDCONT_TEST[];
	static const char AT_CGDCONT_SET[];
	static const char AT_CGSN[];
	static const char AT_SRATE_READ[];
	static const char AT_SRATE_TEST[];
	static const char AT_SRATE_SET[];
	static const char AT_SPFRQ_READ[];
	static const char AT_SPFRQ_TEST[];
	static const char AT_SPFRQ_SET[];
	static const char AT_SAUTOATT_READ[];
	static const char AT_SAUTOATT_SET[];
	static const char AT_SPTEST_READ[];
	static const char AT_SPTEST_SET[];
	static const char AT_PS_DEPATCH[];
	static const char AT_CGMR[];
	static const char AT_SYSCONFIG_READ[];
	static const char AT_SYSCONFIG_SET[];
	static const char AT_VTS_SET[];
	static const char AT_AT_CMD[];
	static const char AT_DSP_SET[];
	static const char AT_CFUN_CHECK[];
	static const char AT_CFUN_SET[];
	
	static const int  CALL_TIME;
	static const int  HOLD_ON_TIME;
	static const int  AUTO_REPORT_TIME;
	static const int  ENGENEERING_TIME;
	static const int  PIN_SET_TIME;
	static const int  PIN_INPUT_TIME;
	static const int  SELECT_NETWORK_TIME;
	static const int  CHECK_NETWORK_TIME;
	static const int  SMS_SEND_MODE_CHECK_TIME;
	static const int  UPLMN_SET_TIME;
	static const int  AT_AT_TIME;
	static const int  OPEN_CMEE_TIME;
	static const int  OPEN_AUTO_PROMT_TIME;
	static const int  AT_HSDPA_VERSION_TIME;
	static const int  AT_CFUN_CHECK_TIME;

};

#endif // !defined(AFX_ETIONIMP_H__FB51ED92_3BAE_4D68_BD45_9C0BEC696B0A__INCLUDED_)
