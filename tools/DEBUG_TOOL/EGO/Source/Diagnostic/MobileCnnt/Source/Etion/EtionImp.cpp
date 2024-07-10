// EtionImp.cpp: implementation of the CEtionImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Etion.h"
#include "EtionImp.h"

#pragma warning(push,3)
#include <algorithm>
#pragma warning(pop)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int  CEtionImp::MAX_RETRY_NUM           = 8;

//at cmd
const char CEtionImp::AT_RES_OK[]			  = "OK\r\n";
const char CEtionImp::AT_RES_ERROR[]		  = "ERROR";
const char CEtionImp::AT_AT[]                 = "AT\r\n";
const char CEtionImp::AT_SIGNAL[]			  = "AT+CSQ\r\n";
const char CEtionImp::AT_NETWORK[]			  = "AT+COPS?\r\n";
const char CEtionImp::AT_AUTO_REPORT_START[]  = "AT+CCED=1,8\r\n";
const char CEtionImp::AT_AUTO_REPORT_STOP[]   = "AT+CCED=2,8\r\n";
const char CEtionImp::AT_HAND_SELECT_NETWORK[] = "AT+COPS=1,2,%s\r\n";
const char CEtionImp::AT_CHECK_NETWORK[]	   = "AT+COPS=?\r\n";
const char CEtionImp::AT_AUTO_SELECT_NETWORK[] ="AT+COPS=0\r\n";
const char CEtionImp::AT_CALL[]				   ="ATD%s\r\n";
const char CEtionImp::AT_HAND_UP[]			   ="ATH\r\n";
const char CEtionImp::AT_HOLD_ON[]			   ="ATA\r\n";
const char CEtionImp::AT_POWER_OFF[]		   ="AT+CPOF\r\n";
const char CEtionImp::AT_DIAL_TYPE_SET[]	   ="AT+SPTEST=6,%s\r\n";
#ifndef _EGO
const char CEtionImp::AT_CONT_IN_SET[]		   ="AT+CGDCONT=1,\"IP\",\"%s\"\r\n";
#else
const char CEtionImp::AT_CONT_IN_SET[]		   ="AT+CGDCONT=2,\"IP\",\"%s\"\r\n";
#endif
const char CEtionImp::AT_SPEED_SET[]		   ="AT+SPPSRATE=2,%s\r\n";
const char CEtionImp::AT_SMS_SEND_CHECK[]	   ="AT+CGSMS?\r\n";
const char CEtionImp::AT_SMS_SEND_SET[]		   ="AT+CGSMS=%s\r\n";
const char CEtionImp::AT_PIN_CHECK[]		   ="AT+CLCK=\"SC\",2\r\n";
const char CEtionImp::AT_PIN_SET[]		       ="AT+CLCK=\"SC\",%s\r\n";
const char CEtionImp::AT_PIN_MODIFY[]		   ="AT+CPWD=\"SC\",%s\r\n";
const char CEtionImp::AT_SYSTEM_CHECK[]		   ="AT^SYSINFO\r\n";
const char CEtionImp::AT_PIN_OR_PUK[]		   ="AT+CPIN?\r\n";
const char CEtionImp::AT_PIN_INPUT[]		   ="AT+CPIN=%s\r\n";
const char CEtionImp::AT_PUK_INPUT[]		   ="AT+CPIN=%s\r\n";
const char CEtionImp::AT_PIN_REMAIN_NUM[]	   ="AT+XX=0\r\n";
const char CEtionImp::AT_PUK2_REMAIN_NUM[]	   ="AT+XX=3\r\n";
const char CEtionImp::AT_OPEN_CMEE[]		   ="AT+CMEE=1\r\n";
const char CEtionImp::AT_OPEN_AUTO_PROMT[]	   ="AT+SIND=1023\r\n";
const char CEtionImp::AT_PUK2_INPUT[]		   ="AT+CPIN=%s\r\n";;
const char CEtionImp::AT_PIN2_MODIFY[]		   ="AT+CPWD=\"P2\",%s\r\n";
const char CEtionImp::AT_FDN_SET[]			   ="AT+CLCK=\"FD\",%s\r\n";
const char CEtionImp::AT_FDN_CHECK[]		   ="AT+CLCK=\"FD\",2\r\n";
const char CEtionImp::AT_UPLMN_CHECK[]		   ="AT+SPUPLMN?\r\n";
const char CEtionImp::AT_AOC_SET[]			   ="AT+SPTEST=3,%s\r\n";
const char CEtionImp::AT_UPLMN_SET[]		   ="AT+SPUPLMN=%s\r\n";
const char CEtionImp::AT_PIN2_REMAIN_NUM[]	   ="AT+XX=1\r\n";
const char CEtionImp::AT_PUK1_REMAIN_NUM[]     ="AT+XX=2\r\n";
const char CEtionImp::AT_SYSTEM_SERVICE_CHECK[] ="AT+CREG?\r\n";
const char CEtionImp::AT_SYSTEM_SERVICE_AUTO[]  ="AT+CREG=2\r\n";
const char CEtionImp::AT_SPID_CHECK[]           ="AT+SPID\r\n";
const char CEtionImp::AT_CGDCONT_READ[]			="AT+CGDCONT?\r\n";
const char CEtionImp::AT_CGDCONT_TEST[]			="AT+CGDCONT=?\r\n";
const char CEtionImp::AT_CGDCONT_SET[]			="AT+CGDCONT=%s\r\n";
const char CEtionImp::AT_CGSN[]					="AT+CGSN\r\n";
const char CEtionImp::AT_SRATE_READ[]			="AT+SPPSRATE?\r\n";
const char CEtionImp::AT_SRATE_TEST[]			="AT+SPPSRATE=?\r\n";
const char CEtionImp::AT_SRATE_SET[]			="AT+SPPSRATE=%s\r\n";
const char CEtionImp::AT_SPFRQ_READ[]			="AT+SPFRQ?\r\n";
const char CEtionImp::AT_SPFRQ_TEST[]			="AT+SPFRQ=?\r\n";
const char CEtionImp::AT_SPFRQ_SET[]			="AT+SPFRQ=%s\r\n";
const char CEtionImp::AT_SAUTOATT_READ[]		="AT+SAUTOATT?\r\n";
const char CEtionImp::AT_SAUTOATT_SET[]			="AT+SAUTOATT=%s\r\n";
const char CEtionImp::AT_SPTEST_READ[]			="AT+SPTEST?\r\n";
const char CEtionImp::AT_SPTEST_SET[]			="AT+SPTEST=%s\r\n";
const char CEtionImp::AT_PS_DEPATCH[]			="AT+CGACT=0,1\r\n";
const char CEtionImp::AT_CGMR[]					="AT+CGMR\r\n";
const char CEtionImp::AT_SYSCONFIG_READ[]		="AT^SYSCONFIG?\r\n";
const char CEtionImp::AT_SYSCONFIG_SET[]		="AT^SYSCONFIG=%s\r\n";
const char CEtionImp::AT_VTS_SET[]				="AT+VTS=%s\r\n";
const char CEtionImp::AT_AT_CMD[]				="%s\r\n";
const char CEtionImp::AT_DSP_SET[]	            ="AT+SPDSP=%s\r\n";
const char CEtionImp::AT_CFUN_CHECK[]			="AT+CFUN?\r\n";
const char CEtionImp::AT_CFUN_SET[]				="AT+CFUN=1\r\n";

#ifdef _EGO
const int  CEtionImp::CALL_TIME				   = 1;
const int  CEtionImp::HOLD_ON_TIME			   = 1;
#else 
const int  CEtionImp::CALL_TIME				   = 1;
const int  CEtionImp::HOLD_ON_TIME			   = 1;
#endif

const int  CEtionImp::AUTO_REPORT_TIME		   = 300;
const int  CEtionImp::PIN_SET_TIME			   = 3000;
const int  CEtionImp::PIN_INPUT_TIME		   = 3000;
const int  CEtionImp::SELECT_NETWORK_TIME	   = 15000;
const int  CEtionImp::CHECK_NETWORK_TIME	   = 1;
const int  CEtionImp::SMS_SEND_MODE_CHECK_TIME = 300;
const int  CEtionImp::UPLMN_SET_TIME		   = 600;
const int  CEtionImp::AT_HSDPA_VERSION_TIME	   = 300;
const int  CEtionImp::AT_AT_TIME			   = 0;
const int  CEtionImp::OPEN_CMEE_TIME		   = 0;
const int  CEtionImp::OPEN_AUTO_PROMT_TIME	   = 0;
const int  CEtionImp::AT_CFUN_CHECK_TIME	   = 300;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEtionImp::CEtionImp()
{

	m_pPortMain = NULL;
	m_pPortSub = NULL;

	m_bUsedMux = FALSE;

}

CEtionImp::~CEtionImp()
{
	m_pPortMain = NULL;
	m_pPortSub = NULL;
	m_bUsedMux = FALSE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BOOL      CEtionImp::StartLog( LPCTSTR pszFileName )
{
	UNREFERENCED_PARAMETER(pszFileName);
	return FALSE;
}

void      CEtionImp::StopLog( void )
{
}
	
void      CEtionImp::SetPort( IPort * pPort, BOOL bMux )
{
	_ASSERTE( pPort != NULL );

	if(bMux)
	{
		m_bUsedMux = bMux;
	}

	if(bMux)
	{
		m_pPortSub = pPort;
	}
	else
	{
		m_pPortMain = pPort;
	}

	if(!m_bUsedMux)
	{
		m_pPortSub = m_pPortMain;
	}
}

IPort *   CEtionImp::GetPort( void )
{
	return NULL;
}
	
LPVOID	  CEtionImp::SendATCmd(DWORD dwCmdId, DWORD dwCmdStep, LPVOID lpParam, DWORD dwReserve)
{
	char szCmd[MAX_BUF_LEN + 1] = {0};
	strcpy(szCmd, GetCmdString(dwCmdId, dwCmdStep, lpParam, dwReserve));
	UINT nDelayTime = GetDelayTime(dwCmdId);
	BOOL bATCmd = FALSE;

	if(dwCmdId == AT_CMD)
	{
		bATCmd = TRUE;
	}
	else
	{
		bATCmd = FALSE;
	}
	
	if(nDelayTime > 0 && (dwCmdId != CALL && dwCmdId != CHECK_NETWORK && dwCmdId != HOLD_ON && dwCmdId != HANG_UP))
	{
		return m_pPortMain->WriteEx(szCmd, nDelayTime, bATCmd);
	}
	else
	{
		return m_pPortSub->WriteEx(szCmd, nDelayTime, bATCmd);
	}
}

BOOL CEtionImp::AutoReportStart(
        HWND   hWnd, 
        LPVOID lpOpenArgs,
		DWORD  dwType,
        DWORD  dwMsgID
        )
{
	if(m_bUsedMux)
	{
		m_pPortSub->StartAutoReceive(hWnd, lpOpenArgs, dwType, dwMsgID);
	}
	m_pPortMain->StartAutoReceive(hWnd, lpOpenArgs, dwType, dwMsgID);

	return TRUE;
}

BOOL CEtionImp::AutoReportStop()
{
	if(m_bUsedMux)
	{
		if(m_pPortSub != NULL)
		{
			m_pPortSub->StopAutoReceive();
		}
	}
	if(m_pPortMain != NULL)
	{
		m_pPortMain->StopAutoReceive();
	}

	return TRUE;
}
	
DWORD     CEtionImp::GetErrCode(void) const
{
	return 0;
}
	
void      CEtionImp::Release(void)
{
	//AutoReportStop();
	delete this;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
LPCSTR CEtionImp::GetCmdString(DWORD dwCmdId, DWORD dwCmdStep, LPVOID lpParam, DWORD dwReserve)
{
	UNREFERENCED_PARAMETER(dwCmdStep);
	switch(dwCmdId)
	{
	case CHECK_CONNECT:
		return AT_AT;
	case SIGNAL:
		return AT_SIGNAL;
	case NETWORK:
		return AT_NETWORK;
	case AUTO_REPORT_START:
		return AT_AUTO_REPORT_START;
	case AUTO_REPORT_STOP:
		return AT_AUTO_REPORT_STOP;
	case HAND_SELECT_NETWORK:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_HAND_SELECT_NETWORK, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_HAND_SELECT_NETWORK, szTemp);
#endif
			return m_szCmd;
		}
	case AUTO_SELECT_NETWORK:
		return AT_AUTO_SELECT_NETWORK;
	case CALL:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_CALL, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_CALL, szTemp);
#endif
			return m_szCmd;
		}
	case HANG_UP:
		return AT_HAND_UP;
	case HOLD_ON:
		return AT_HOLD_ON;		
	case POWER_OFF:
		return AT_POWER_OFF;
	case DIAL_TYPE_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_DIAL_TYPE_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_DIAL_TYPE_SET, szTemp);
#endif
			return m_szCmd;
		}
	case CONT_IN_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_CONT_IN_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_CONT_IN_SET, szTemp);
#endif

			return m_szCmd;
		}
	case SPEED_SET_NO_RETURN:
	case SPEED_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_SPEED_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_SPEED_SET, szTemp);
#endif
			return m_szCmd;
		}	
	case SMS_SEND_MODE_CHECK:
		return AT_SMS_SEND_CHECK;
	case SMS_SEND_MODE_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_SMS_SEND_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_SMS_SEND_SET, szTemp);
#endif

			return m_szCmd;
		}
	case PIN_CHECK:
		return AT_PIN_CHECK;
	case PIN_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_PIN_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_PIN_SET, szTemp);
#endif

			return m_szCmd;
		}
	case PIN_MODIFY:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_PIN_MODIFY, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_PIN_MODIFY, szTemp);
#endif
			return m_szCmd;
		}
	case SYSTEM_CHECK:
		return AT_SYSTEM_CHECK;
	case PIN_OR_PUK:
		return AT_PIN_OR_PUK;
	case PIN_INPUT:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_PIN_INPUT, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_PIN_INPUT, szTemp);
#endif
			return m_szCmd;
		}
	case PUK_INPUT:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_PUK_INPUT, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_PUK_INPUT, szTemp);
#endif
			return m_szCmd;
		}
	case PIN_REMAIN_NUM:
		return AT_PIN_REMAIN_NUM;
	case OPEN_CMEE:
		return AT_OPEN_CMEE;
	case OPEN_AUTO_PROMT:
		return AT_OPEN_AUTO_PROMT;
	case CHECK_NETWORK:
		return AT_CHECK_NETWORK;
	case PUK2_REMAIN_NUM:
		return AT_PUK2_REMAIN_NUM;
	case PIN2_MODIFY:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_PIN2_MODIFY, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_PIN2_MODIFY, szTemp);
#endif
			return m_szCmd;
		}
	case PUK2_INPUT:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_PUK2_INPUT, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_PUK2_INPUT, szTemp);
#endif
			return m_szCmd;
		}
	case FDN_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_FDN_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_FDN_SET, szTemp);
#endif
			return m_szCmd;
		}
	case FDN_CHECK:
		return AT_FDN_CHECK;
	case UPLMN_CHECK:
		return AT_UPLMN_CHECK;
	case AOC_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_AOC_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			sprintf(m_szCmd, AT_AOC_SET, szTemp);
#endif

			return m_szCmd;
		}
	case UPLMN_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};

#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_UPLMN_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			
			sprintf(m_szCmd, AT_UPLMN_SET, szTemp);
#endif

			return m_szCmd;
		}
	case PIN2_REMAIN_NUM:
		return AT_PIN2_REMAIN_NUM;
	case PUK1_REMAIN_NUM:
		return AT_PUK1_REMAIN_NUM;
	case SYSTEM_SERVICE_CHECK:
		return AT_SYSTEM_SERVICE_CHECK;
	case SYSTEM_SERVICE_AUTO:
		return AT_SYSTEM_SERVICE_AUTO;
	case SPID_CHECK:
		return AT_SPID_CHECK;
	case CGDCONT_READ:
		return AT_CGDCONT_READ;
	case CGDCONT_TEST:
		return AT_CGDCONT_TEST;
	case CGDCONT_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};

#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_CGDCONT_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			
			sprintf(m_szCmd, AT_CGDCONT_SET, szTemp);
#endif
			return m_szCmd;
		}
	case CGSN_CHECK:
		return AT_CGSN;
	case SRATE_READ:
		return AT_SRATE_READ;
	case SRATE_TEST:
		return AT_SRATE_TEST;
	case SRATE_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};

#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_SRATE_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			
			sprintf(m_szCmd, AT_SRATE_SET, szTemp);
#endif
			return m_szCmd;
		}
	case SPFRQ_READ:
		return AT_SPFRQ_READ;
	case SPFRQ_TEST:
		return AT_SPFRQ_TEST;
	case SPFRQ_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};

#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_SPFRQ_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			
			sprintf(m_szCmd, AT_SPFRQ_SET, szTemp);
#endif
			return m_szCmd;
		}
	case SAUTOATT_READ:
		return AT_SAUTOATT_READ;
	case SAUTOATT_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};

#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_SAUTOATT_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			
			sprintf(m_szCmd, AT_SAUTOATT_SET, szTemp);
#endif
			return m_szCmd;
		}
	case SPTEST_READ:
		return AT_SPTEST_READ;
	case SPTEST_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};

#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_SPTEST_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			
			sprintf(m_szCmd, AT_SPTEST_SET, szTemp);
#endif
			return m_szCmd;
		}
	case HSDPA_VERSION:
		return AT_CGMR;
	case SYSCONFIG_READ:
		return AT_SYSCONFIG_READ;
	case SYSCONFIG_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};

#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_SYSCONFIG_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			
			sprintf(m_szCmd, AT_SYSCONFIG_SET, szTemp);
#endif
			return m_szCmd;
		}
	case VTS_SET:
				{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};

#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_VTS_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			
			sprintf(m_szCmd, AT_VTS_SET, szTemp);
#endif
			return m_szCmd;
		}
	case AT_CMD:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_AT_CMD, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			
			sprintf(m_szCmd, AT_AT_CMD, szTemp);
#endif
			return m_szCmd;
		}
	case DSP_SET:
		{
			char szTemp[MAX_PATH] = {0};
			TCHAR szWtemp[MAX_PATH] = {0};
			
#ifdef _UNICODE
			dwReserve = (dwReserve + 1) * 2;
			memcpy(szWtemp, (LPTSTR)lpParam, dwReserve);
			WideCharToMultiByte(CP_ACP,0,szWtemp,-1,szTemp,sizeof(szTemp),NULL,NULL);
			
			sprintf(m_szCmd, AT_DSP_SET, szTemp);
#else
			memcpy(szTemp, (LPTSTR)lpParam, dwReserve);
			
			sprintf(m_szCmd, AT_DSP_SET, szTemp);
#endif
			return m_szCmd;
		}
//		break;
	case PS_DEPATCH:
		return AT_PS_DEPATCH;
	case CFUN_CHECK:
		return AT_CFUN_CHECK;
	case CFUN_SET:
		return AT_CFUN_SET;
	default:
		break;
	}
	return "";
}

int CEtionImp::GetDelayTime(DWORD dwCmdId)
{
	switch(dwCmdId)
	{
	case CHECK_CONNECT:
		return AT_AT_TIME;
	case CALL:
		return CALL_TIME;
	case AUTO_REPORT_START:
		return AUTO_REPORT_TIME;
	case PIN_INPUT:
		return PIN_INPUT_TIME;
	case PIN_REMAIN_NUM:
		return 0;
	case PUK2_REMAIN_NUM:
		return 0;
	case PIN2_REMAIN_NUM:
		return 0;
	case PUK1_REMAIN_NUM:
		return 0;
	case UPLMN_CHECK:
		return 0;
	case PUK_INPUT:
		return PIN_INPUT_TIME;
	case AUTO_SELECT_NETWORK:
		return SELECT_NETWORK_TIME;
	case HAND_SELECT_NETWORK:
		return SELECT_NETWORK_TIME;
	case CHECK_NETWORK:
		return CHECK_NETWORK_TIME;
	case NETWORK:
		return 0;
	case SMS_SEND_MODE_CHECK:
		return SMS_SEND_MODE_CHECK_TIME;
	case UPLMN_SET:
		return UPLMN_SET_TIME;
	case SYSTEM_SERVICE_AUTO:
		return 0;
	case SYSTEM_SERVICE_CHECK:
		return 0;
	case PIN_CHECK:
		return 0;
	case FDN_CHECK:
		return 0;
	case SPID_CHECK:
		return 0;
	case CGDCONT_READ:
		return 0;
	case CGDCONT_TEST:
		return 0;
	case SYSTEM_CHECK:
		return 0;
	case CGSN_CHECK:
		return 0;
	case SRATE_READ:
		return 0;
	case SRATE_TEST:
		return 0;
	case SPFRQ_READ:
		return 0;
	case SPFRQ_TEST: 
		return 0;
	case SAUTOATT_READ:
		return 0;
	case SPTEST_READ:
		return 0;
	case SIGNAL:
		return 0;
	case SPEED_SET_NO_RETURN:
		return 0;
	case HOLD_ON:
		return HOLD_ON_TIME;
	case PS_DEPATCH:
		return 0;
	case OPEN_CMEE:
		return OPEN_CMEE_TIME;
	case OPEN_AUTO_PROMT:
		return OPEN_AUTO_PROMT_TIME;
	case SYSCONFIG_READ:
		return 0;
	case VTS_SET:
		return 0;
	case HSDPA_VERSION:
		return AT_HSDPA_VERSION_TIME;
	case CFUN_CHECK:
		return AT_CFUN_CHECK_TIME;
	case CFUN_SET:
		return 0;
	case AT_CMD:
		return 0;
	default:
		return DEFAULT_DELAY_TIME;
	}
}