// Phone.cpp: implementation of the CPhone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IPort.h"
#include "Phone.h"
#include "GlobalDef.h"
#include "..\MobileCnnt.h"


const int  CPhone::MAX_RETRY_COUNT         = 5;
const int  CPhone::READ_DELAY_TIME         = 200; // ms

const char CPhone::MMI_READY[]            = "AT##MMIREADY?\r\n";
const char CPhone::MMI_READY_OK[]         = "MMI\r\nOK\r\n";
const char CPhone::MMI_ERROR[]			  = "ERROR\r\n";

const char CPhone::AT_AT[]                = "AT\r\n";
const char CPhone::AT_RES_OK[]            = "OK\r\n";
const char CPhone::AT_ERROR[]             = "+CME ERROR:";
// 
const char CPhone::AT_NOPROMPT[]          = "AT+SIND=0\r\n";
const char CPhone::AT_NOECHO[]            = "ATE0\r\n";

// MMI接口命令格式：AT## + 标识符 + 模块名 + 操作类型 + 操作参数 + \r\n

// %c%c%c分别为：MT/MO标识，存储位置(sim卡或手机)，删除的记录id
const char CPhone::MMI_DEL_IDXSMS[]       = "AT##MMI12%c%c%04d\r\n";
const char CPhone::MMI_SMS_MT             = '0';
const char CPhone::MMI_SMS_MO             = '1';
const char CPhone::MMI_SMS_SIM            = '0';
const char CPhone::MMI_SMS_MS             = '1';

const char CPhone::MMI2_SMS_MS        = '0';
const char CPhone::MMI2_SMS_SIM1      = '1';
const char CPhone::MMI2_SMS_SIM2      = '2';
const char CPhone::MMI2_SMS_SIM3      = '3';
const char CPhone::MMI2_SMS_SIM4      = '4';

const char CPhone::AT_REG_INMSG_INDEX[]      = "AT+CNMI=3,3,0,1\r\n";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhone::CPhone()
{
    m_pPort = NULL;
	m_nSIMState		= SIM_PLUGED_SINGLE;
	m_nMMIVersion   = MMI_VERSION_1;
	m_nSIMCount     = 1;
}

CPhone::~CPhone()
{
    m_pPort = NULL;
}

void CPhone::SetPort( IPort * pIntfc )
{
    _ASSERTE( pIntfc != NULL );

    m_pPort = pIntfc;
}

BOOL CPhone::IsConnected( void )
{
    _ASSERTE( m_pPort != NULL );

    if( !m_pPort->IsOpened() )
    {
        return FALSE;
    }
#ifdef SPRD8800
	return TRUE;
#else
	return ExecuteCmd(MMI_READY);
#endif

}

BOOL CPhone::InitPhone( void )
{
	static const int nCmdNum    = 2;
	static const int nMaxCmdLen = 31;
	
	char arrCmd[nCmdNum][nMaxCmdLen + 1] = { 0 };

	strcpy(arrCmd[0], AT_NOPROMPT);
	strcpy(arrCmd[1], AT_NOECHO);

	for( int i = 0; i < nCmdNum; ++i )
	{
		_ASSERTE( strlen(arrCmd[i]) <= nMaxCmdLen );
		if( !ExecuteCmd(arrCmd[i]) )
		{
			TRACE1("!! InitPhone Fail, the cmd is %s !!\r\n", arrCmd[i]);
			return FALSE;
		}
	}

	return IsConnected();
}

BOOL CPhone::DeleteSMS( _SMS * psms )
{
    _ASSERTE( psms != NULL );

    char cM = MMI_SMS_MT;
	if( psms->wStatus == SMS_STATUS_SEND || psms->wStatus == SMS_STATUS_UNSEND )
		cM = MMI_SMS_MO;

	char cS = MMI_SMS_SIM;
#ifdef SPRD8800 
		if(theApp.m_nMMIVer == MMI_VERSION_1)
#else
			if(theApp.m_nMMIVer == MMI_VERSION_1 || theApp.m_nMMIVer == MMI_VERSION_1_1)
#endif
	
	{
		if( psms->wStore == ID_STORE_PHONE )
			cS = MMI_SMS_MS;
	}
	else
	{
		cS = MMI2_SMS_MS;
		if(psms->wStore == ID_STORE_SIM)
			cS = MMI2_SMS_SIM1;
		else if(psms->wStore == ID_STORE_SIM2)
			cS = MMI2_SMS_SIM2;
		else if(psms->wStore == ID_STORE_SIM3)
			cS = MMI2_SMS_SIM3;
		else if(psms->wStore == ID_STORE_SIM4)
			cS = MMI2_SMS_SIM4;
	}
	

    char szCmd[128] = { 0 };
#ifdef SPRD8800
	//teana
	if(theApp.m_pSMSFactory != NULL && theApp.m_bConnected)
	{
		return theApp.m_pSMSFactory->DeleteSMS(psms->nIdx);
	}
#else
	 //in mmi the index begin at 0, in at cmd the index begin at 1
		sprintf(szCmd, MMI_DEL_IDXSMS, cM, cS, psms->nIdx - 1);
		if( !ExecuteCmd(szCmd) )
		{
			TRACE0("!! delete sms by mmi cmd fail !!\r\n");
			return FALSE;
		}
#endif
	
    
	return TRUE;
}

BOOL CPhone::ExecuteCmd( LPCSTR pCmd )
{
    _ASSERTE( m_pPort != NULL );
	_ASSERTE( pCmd != NULL );
	if(!m_pPort->IsOpened())
		return FALSE;

	static const int nResLen = 127;
	char szResult[ nResLen + 1 ] = { 0 };

    LPSTR pRcv   = szResult;
    DWORD dwRcv  = nResLen;
    DWORD dwRead = 0;
	
    m_pPort->PurgeAll();

    m_pPort->Write( (LPVOID)pCmd, strlen(pCmd), NULL );

    for( int i = 0; dwRcv > 0 && i < MAX_RETRY_COUNT; ++i )
    {
        if( m_pPort->Read(pRcv, dwRcv, &dwRead, READ_DELAY_TIME) && dwRead > 0)
        {
            if( strstr(szResult, MMI_READY_OK) != NULL)                
            {
				char * pVer = strstr(szResult,"ver");
				if(pVer != NULL)
				{
					pVer = pVer + 3; // [ver]1.00
					*(pVer + 1) = '\0';
					int nData = 0;
					nData = atoi(pVer);
					if(nData != 1 && nData !=2)
						return FALSE;
					else
						m_nMMIVersion = nData;

					pVer = pVer + 2; // [ver1.]00
					*(pVer + 2) = '\0';
					nData = 0;
					nData = atoi(pVer);
					if(nData<0 || nData> SIM_PLUGED_STATE_MAX)
					{
						return FALSE;
					}
					else
					{
						m_nSIMState = nData;					
#ifndef SPRD8800
						if(m_nMMIVersion == MMI_VERSION_1 && nData == 1) // 1.01
						{
							m_nMMIVersion = MMI_VERSION_1_1;
							m_nSIMState = SIM_PLUGED_SINGLE;
						}
#endif
						//calc the SIM card count
						if(m_nSIMState == SIM_PLUGED_SINGLE)
						{
							m_nSIMCount = 1;
						}
						else
						{
							BYTE bTemp = (BYTE)m_nSIMState;
							int nCount = 0;
							BYTE bMask = SIM_PLUGED_STATE_MAX;
							while(bMask != 0 )
							{
								if((bTemp&1) == 1)
								{
									nCount++;
								}
								bMask = (BYTE)(bMask >> 1);
								bTemp = (BYTE)(bTemp >> 1);
							}
							_ASSERTE(nCount != 0);

							if(nCount == 0)
								nCount = 1;
							
							m_nSIMCount = nCount;

						}
					}					
				}

				return TRUE;	               
            }
			else if(strstr(szResult, AT_RES_OK) != NULL)
			{
				return TRUE;
			}
			else if(strstr(szResult, "ERROR") != NULL || strstr(szResult, "Error") != NULL)
			{
				return FALSE;
			}
            pRcv  += dwRead;
            dwRcv -= dwRead;
        }	
    }

//    TRACE2("!! CPhone::ExecuteCmd fail! Send: %s Receive is:%s !!\r\n", pCmd, szResult);

	return FALSE;
}


int  CPhone::GetSIMState( void )
{
	return m_nSIMState;
}

int  CPhone::GetMMIVersion( void )
{
	return m_nMMIVersion;
}

int  CPhone::GetSIMCount( void )
{
	return m_nSIMCount;
}