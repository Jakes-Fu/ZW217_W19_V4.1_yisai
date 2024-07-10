#include "stdafx.h"
#include "DaInfo.h"
#include "OptionHelpper.h"
extern "C"
{
#include "bmpacket.h"
#include "crc16.h"
}


extern COptionHelpper ohObject;

CDaInfo::CDaInfo(void)
{
    Clear();
}

CDaInfo::~CDaInfo(void)
{
}

void CDaInfo::Clear()
{
    m_bDisableTransCode = FALSE;
    m_nIsOldMemory = 0;
    m_nSupportRawData = 0;
    m_dwFlushSize = 0;
    m_bTimeStamp = FALSE;
    memset(&m_tRandomData, 0, sizeof(m_tRandomData));
    m_tTimeStamp.Clear();
}

void CDaInfo::ParseData(char* pBuf, int nLen, DWORD& dwLastErrorCode)
{
    if (NULL == pBuf || nLen < 0)
    {
        dwLastErrorCode = BSL_REP_96FMT_ERROR;
        return;
    }

    if (nLen < 4)
    {
        dwLastErrorCode = BSL_REP_96FMT_ERROR;
        return;
    }

    DWORD dwVersion = 0;
    memcpy( &dwVersion ,pBuf, 4 );
    DA_INFO_T daInfo;
    if(dwVersion >= 2 && dwVersion <= 4)
    {        
        if( nLen  <= sizeof(DA_INFO_T) )
        {
            memcpy( &daInfo ,pBuf, nLen );
            m_nIsOldMemory = daInfo.bIsOldMemory;
            if(ohObject.IsNeedCheckOldMemory())
            {
                CheckSecondHandMemory(dwLastErrorCode);
            }
            m_bDisableTransCode = daInfo.bDisableTransCode;

            if (dwVersion == 2)
            {
                m_tRandomData.dwSupportRandomNum = 0;
            }
            else if (dwVersion == 3 || dwVersion == 4)
            {
                m_nSupportRawData = daInfo.bSupportRawData;
                m_dwFlushSize = daInfo.dwFlushSize;
				
				m_tRandomData.dwSupportRandomNum = daInfo.dwSupportRandomNum;
				memcpy(m_tRandomData.nRandomData, daInfo.bRandomData, sizeof(m_tRandomData.nRandomData));

            }
        }
    }
    else if(dwVersion == 1)
    {
        if(ohObject.IsNeedCheckOldMemory())
        {
            dwLastErrorCode = BSL_REP_OLD_MEMORY_UNSUPPORT_ERROR;
            return;
        }
        if( nLen <= sizeof(DA_INFO_T) )
        {
            memcpy( &daInfo ,pBuf, 8);
            m_bDisableTransCode = daInfo.bDisableTransCode;
        }
    }
    else if (dwVersion == DA_INFO_MAGIC)
    {
        
        int nLeft = nLen-4;
        int nStart = 4;     // skip version

        while(nLeft > 4)
        {
            unsigned short type = 0;
            memcpy(&type, pBuf+nStart, 2);
            nStart += 2;
            nLeft -= 2;

            unsigned short len = 0;
            memcpy(&len, pBuf+nStart, 2);
            nStart += 2;
            nLeft -= 2;

            if (nLeft < len)
            {
                dwLastErrorCode = BSL_REP_96FMT_ERROR;
                return;
            }        

            void* pData = NULL;
            int nTargetLen = -1;

            switch(type)
            {
            case E_TYPE_DISABLE_TRANS_CODE:
                pData = &m_bDisableTransCode;
                nTargetLen = sizeof(m_bDisableTransCode);
                break;
            case E_TYPE_FLUSH_SIZE:
                pData = &m_dwFlushSize;
                nTargetLen = sizeof(m_dwFlushSize);
                break;
            case E_TYPE_IS_OLD_MEMORY:
                pData = &m_nIsOldMemory;
                nTargetLen = sizeof(m_nIsOldMemory);
                break;
            case E_TYPE_SUPPORT_RAW_DATA:
                pData = &m_nSupportRawData;
                nTargetLen = sizeof(m_nSupportRawData);
                break;
            case E_TYPE_RANDOM_DATA:
                pData = &m_tRandomData;
                nTargetLen = sizeof(m_tRandomData);
                break;
            case E_TYPE_TIME_STAMP:
                pData = &m_tTimeStamp;
                nTargetLen = sizeof(m_tTimeStamp);
				break;
			case E_TYPE_SUPPORT_DUMP_UBOOT_LOG:
				pData = &m_bSupportUbootLogDump;
				nTargetLen = sizeof(m_bSupportUbootLogDump);
				break;
            default:
                break;
            }

            if (nTargetLen != len)
            {
                dwLastErrorCode = BSL_REP_96FMT_ERROR;
                return;
            }

            memcpy(pData, pBuf+nStart, len);

            nLeft -= len;
            nStart += len;

        }
        
        if (ohObject.IsNeedCheckOldMemory())
        {
            m_bTimeStamp = TRUE;
            DA_INFO_TIME_STAMP ts = m_tTimeStamp;
            const int INIT_TIME_STAMP_VAL = 0xFFFFFFFF;
            if (ts.dwTimeStamp != INIT_TIME_STAMP_VAL)
            {
                unsigned short crc = 0;
                crc = crc16(crc, (unsigned char*)&ts, sizeof(ts));   
                if (ts.nSign != TIME_STAMP_SIGN || crc != 0)
                {
                    CheckSecondHandMemory(dwLastErrorCode);
                }
                else
                {
                    time_t cur_time;
                    time(&cur_time);
                    int time_span = ohObject.GetCheckSecondHandMemoryTime();
                    if ((cur_time - ts.dwTimeStamp) > time_span*60*60)
                    {
                        CheckSecondHandMemory(dwLastErrorCode);
                    }
                }
            }

        }
    }

    if (!ohObject.SupportUnTransCode())
    {
        m_bDisableTransCode = FALSE;
    }


    return;
}

void CDaInfo::CheckSecondHandMemory(DWORD& dwLastErrorCode)
{
    switch (m_nIsOldMemory & ohObject.GetCheckOldMemoryKind())
    {
    case 1:
        dwLastErrorCode = BSL_REP_OLD_MEMORY_REGISTER_CHANGE_ERROR;
        break;
    case 2:
        dwLastErrorCode = BSL_REP_OLD_MEMORY_EMMC_HAS_FLAG_ERROR;
        break;
    case 3:
        dwLastErrorCode = BSL_REP_OLD_MEMORY_REGISTER_EMMC_ERROR;
        break;
    case 4:
        dwLastErrorCode = BSL_REP_OLD_MEMORY_RPMB_WRITTEN_ERROR;
        break;
    case 5:
        dwLastErrorCode = BSL_REP_OLD_MEMORY_REGISTER_RPMB_ERROR;
        break;
    case 6:
        dwLastErrorCode = BSL_REP_OLD_MEMORY_EMMC_RPMB_ERROR;
        break;
    case 7:
        dwLastErrorCode = BSL_REP_OLD_MEMORY_REGISTER_EMMC_RPMB_ERROR;
        break;
    default:
        break;
    }
}