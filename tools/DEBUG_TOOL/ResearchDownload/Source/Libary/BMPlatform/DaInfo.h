#pragma once
#include "Global.h"

typedef struct _DA_INFO_TIME_STAMP 
{
    DWORD   dwTimeStamp;
    SHORT   nSign;
    SHORT   nTimStampCrc;
    _DA_INFO_TIME_STAMP()
    {
        Clear();
    }
    void Clear()
    {
        memset(this, 0, sizeof(_DA_INFO_TIME_STAMP));
    }
}DA_INFO_TIME_STAMP;

typedef struct _RANDOM_DATA
{
    DWORD   dwSupportRandomNum;  // 0: Not Support; 1: AES; 2: RSA2038
    BYTE    nRandomData[256];
    _RANDOM_DATA()
    {
        memset(this, 0, sizeof(_RANDOM_DATA));
    }
}RANDOM_DATA;

class CDaInfo
{
public:
    CDaInfo(void);
    ~CDaInfo(void);

    void    ParseData(char* pBuf, int nLen, DWORD& dwLastErrorCode);
    void    CheckSecondHandMemory(DWORD& dwLastErrorCode);
    void    Clear();

    BOOL    m_bDisableTransCode; 
    BYTE    m_nIsOldMemory;        // 前3个bit分别对应：1. 疑似寄存器改过; 2.疑似EMMC分区有标记值; 3.疑似RPMB 写过。
    BYTE    m_nSupportRawData;	
    DWORD   m_dwFlushSize;	//unit is M

    RANDOM_DATA m_tRandomData;
    DA_INFO_TIME_STAMP  m_tTimeStamp;
    BOOL    m_bTimeStamp;
	BOOL	m_bSupportUbootLogDump;
};
