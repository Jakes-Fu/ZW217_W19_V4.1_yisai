/*
和盈互联智能穿戴通讯协议2.0兼容GSM网.pdf
*/
#include "zdt_app.h"
#include "zdt_hyhl_func.h"

#ifdef FEATURE_WIFI_SUPPORT
#include "wifi_sio.h"
#endif

#ifdef ZDT_PLAT_HYHL_SUPPORT
//CRC-ITU 查表算法 C 语言代码片段
static const uint16 crctab16[] ={
0X0000, 0X1189, 0X2312, 0X329B, 0X4624, 0X57AD, 0X6536, 0X74BF,
0X8C48, 0X9DC1, 0XAF5A, 0XBED3, 0XCA6C, 0XDBE5, 0XE97E, 0XF8F7,
0X1081, 0X0108, 0X3393, 0X221A, 0X56A5, 0X472C, 0X75B7, 0X643E,
0X9CC9, 0X8D40, 0XBFDB, 0XAE52, 0XDAED, 0XCB64, 0XF9FF, 0XE876,
0X2102, 0X308B, 0X0210, 0X1399, 0X6726, 0X76AF, 0X4434, 0X55BD,
0XAD4A, 0XBCC3, 0X8E58, 0X9FD1, 0XEB6E, 0XFAE7, 0XC87C, 0XD9F5,
0X3183, 0X200A, 0X1291, 0X0318, 0X77A7, 0X662E, 0X54B5, 0X453C,
0XBDCB, 0XAC42, 0X9ED9, 0X8F50, 0XFBEF, 0XEA66, 0XD8FD, 0XC974,
0X4204, 0X538D, 0X6116, 0X709F, 0X0420, 0X15A9, 0X2732, 0X36BB,
0XCE4C, 0XDFC5, 0XED5E, 0XFCD7, 0X8868, 0X99E1, 0XAB7A, 0XBAF3,
0X5285, 0X430C, 0X7197, 0X601E, 0X14A1, 0X0528, 0X37B3, 0X263A,
0XDECD, 0XCF44, 0XFDDF, 0XEC56, 0X98E9, 0X8960, 0XBBFB, 0XAA72,
0X6306, 0X728F, 0X4014, 0X519D, 0X2522, 0X34AB, 0X0630, 0X17B9,
0XEF4E, 0XFEC7, 0XCC5C, 0XDDD5, 0XA96A, 0XB8E3, 0X8A78, 0X9BF1,
0X7387, 0X620E, 0X5095, 0X411C, 0X35A3, 0X242A, 0X16B1, 0X0738,
0XFFCF, 0XEE46, 0XDCDD, 0XCD54, 0XB9EB, 0XA862, 0X9AF9, 0X8B70,
0X8408, 0X9581, 0XA71A, 0XB693, 0XC22C, 0XD3A5, 0XE13E, 0XF0B7,
0X0840, 0X19C9, 0X2B52, 0X3ADB, 0X4E64, 0X5FED, 0X6D76, 0X7CFF,
0X9489, 0X8500, 0XB79B, 0XA612, 0XD2AD, 0XC324, 0XF1BF, 0XE036,
0X18C1, 0X0948, 0X3BD3, 0X2A5A, 0X5EE5, 0X4F6C, 0X7DF7, 0X6C7E,
0XA50A, 0XB483, 0X8618, 0X9791, 0XE32E, 0XF2A7, 0XC03C, 0XD1B5,
0X2942, 0X38CB, 0X0A50, 0X1BD9, 0X6F66, 0X7EEF, 0X4C74, 0X5DFD,
0XB58B, 0XA402, 0X9699, 0X8710, 0XF3AF, 0XE226, 0XD0BD, 0XC134,
0X39C3, 0X284A, 0X1AD1, 0X0B58, 0X7FE7, 0X6E6E, 0X5CF5, 0X4D7C,
0XC60C, 0XD785, 0XE51E, 0XF497, 0X8028, 0X91A1, 0XA33A, 0XB2B3,
0X4A44, 0X5BCD, 0X6956, 0X78DF, 0X0C60, 0X1DE9, 0X2F72, 0X3EFB,
0XD68D, 0XC704, 0XF59F, 0XE416, 0X90A9, 0X8120, 0XB3BB, 0XA232,
0X5AC5, 0X4B4C, 0X79D7, 0X685E, 0X1CE1, 0X0D68, 0X3FF3, 0X2E7A,
0XE70E, 0XF687, 0XC41C, 0XD595, 0XA12A, 0XB0A3, 0X8238, 0X93B1,
0X6B46, 0X7ACF, 0X4854, 0X59DD, 0X2D62, 0X3CEB, 0X0E70, 0X1FF9,
0XF78F, 0XE606, 0XD49D, 0XC514, 0XB1AB, 0XA022, 0X92B9, 0X8330,
0X7BC7, 0X6A4E, 0X58D5, 0X495C, 0X3DE3, 0X2C6A, 0X1EF1, 0X0F78,
};

// 计算给定长度数据的 16 位 CRC。
uint16 HYHL_Func_GetCrc16(const uint8* pData, int nLength)
{
    uint16 fcs = 0xffff; // 初始化
    while(nLength>0)
    {
        fcs = (fcs >> 8) ^ crctab16[(fcs ^ *pData) & 0xff];
        nLength--;
        pData++;
    }
    return ~fcs; // 取反
}

uint16 HYHL_Func_GetGPSDate(const uint8* pData, int nLength)
{
    uint16 fcs = 0xffff; // 初始化
    while(nLength>0)
    {
        fcs = (fcs >> 8) ^ crctab16[(fcs ^ *pData) & 0xff];
        nLength--;
        pData++;
    }
    return ~fcs; // 取反
}

int  HYHL_Func_SOSGetNextPara(uint8** pCmd, uint16* pLen, uint8* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen -1); i++)
    {
        if( (*(*pCmd + i) == '&')
            && (*(*pCmd + i+1) == '&'))    // Find next comma, that is, delimiter
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
    
    if(i == (*pLen -1))
    {
        i = *pLen;
    }
    
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 30;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 2);
            *pLen -= (EndIndex + 2);
        }
    }
    return(bRtn);
}

int  HYHL_Func_LinkManGetNextPara(char mark,uint8** pCmd, uint16* pLen, uint8* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == mark)
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 200;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  HYHL_Func_ClockGetNextPara(uint8** pCmd, uint16* pLen, uint8* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == '|')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  HYHL_Func_HiddenGetNextPara(uint8** pCmd, uint16* pLen, uint8* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == '|')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}
uint8 HYHL_Func_GetTime(uint8 * pData)
{
/*
    格式 年 月 日 时 分 秒
    长度(Byte) 1 1 1 1 1 1
*/
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    pData[0] = date.year%2000;
    pData[1] = date.mon;
    pData[2] = date.mday;
    pData[3] = time.hour;
    pData[4] = time.min;
    pData[5] = time.sec;
    return 6;
}

uint8 HYHL_Func_GetPhoneInfo(uint8 * pData)
{
/*
    字节位
    是否在充电 1 是/0 否 7
    GPS 是否己定位 1 是/0 否 6
    预留拓展   5
                            4
                            3
                            2
                            1
                            0
*/
    BOOLEAN is_charger = ZDT_GetIsCharge();
    BOOLEAN is_gps = 0;
    uint8 ret = 0;
    if(is_charger)
    {
        ret = ret | 0x80;
    }
    else
    {
        ret = ret & 0x7F;
    }
    
    if(is_gps)
    {
        ret = ret | 0x40;
    }
    else
    {
        ret = ret & 0xBF;
    }
    pData[0] = ret;
    return 1;
}

uint8 HYHL_Func_GetBatLevel(uint8 * pData)
{
    uint8 lev = MMIAPIPHONE_GetBatCapacity();
        
    pData[0] = lev;
    return 1;
}

uint8 HYHL_Func_GetBatPercent(uint8 * pData)
{
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    uint8 percent = 0;
    if(p_chgmng_info != PNULL)
    {
        percent = p_chgmng_info->bat_remain_cap;
    }    
    
    pData[0] = percent;
    return 1;
}

uint8 HYHL_Func_GetRssiLevel(uint8 * pData)
{
    uint8	i = 5;
    uint16 rssi = g_zdt_s_station_data.rssiLevel;

    //设置信强度,根据参考手机sagem得到的数据，将idle界面下的信号强度划分成5级
    // 1：1-7； 2：8-11； 3：12-15； 4：16-19； 5：20-63
    if (20 <= rssi)
    {
        i = 5;
    }
    else if (16 <= rssi)
    {
        i = 4;
    }
    else if (12 <= rssi)
    {
        i = 3;
    }
    else if (8 <= rssi)
    {
        i = 2;
    }
    else if (1 <= rssi)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }

    pData[0] = i;
    return 1;
}

uint8 HYHL_Func_GetJPNum(uint8 * pData)
{
    uint32 jp_sum = 0;
#ifdef ZDT_GSENSOR_SUPPORT
    jp_sum = ZDT_GSensor_GetStep();
#endif
    pData[0] = (jp_sum & 0x00FF0000) >> 16;
    pData[1] = (jp_sum & 0x0000FF00) >> 8;
    pData[2] = jp_sum & 0x000000FF;
    return 3;
}

uint8 HYHL_Func_GetJPStatus(uint8 * pData)
{
    pData[0] = 1;
    return 1;
}

uint16 HYHL_Func_GetWIFIInfo(uint8 * pData,uint8 loc_ok)
{
/*
    WIFI 信息 WIFI 数量 1
                    AP 地址 6
                    信号 1
    
*/
    uint8 cnt = 0;
    uint16 ret = 0;
    if((loc_ok&0x02) == 0)
    {
        pData[0] = 0;
        ret = 1;
    }
    else
    {
#ifdef FEATURE_WIFI_SUPPORT
        cnt = WIFI_Get_DataNum();
        if(cnt > 0)
        {
            WIFI_DATA_T  wifi_data = {0};
            uint8 i = 0;
            pData[ret++] = cnt;
            for(i = 0; i < cnt; i++)
            {
                WIFI_Get_Data(i,&wifi_data);
                SCI_MEMCPY(&pData[ret],wifi_data.bssid,6);
                ret += 6;
                pData[ret++] = wifi_data.rssi;
            }
        }
        else
#endif
        {
            pData[0] = 0;
            ret = 1;
        }
    }
    
    return ret;
}

uint8 HYHL_Func_GetGPSLat(uint8 * pData,uint32 Latitude)
{
    pData[0] = (Latitude & 0xFF000000) >> 24;
    pData[1] = (Latitude & 0x00FF0000) >> 16;
    pData[2] = (Latitude & 0x0000FF00) >> 8;
    pData[3] = Latitude & 0x000000FF;
    return 4;
}

uint8 HYHL_Func_GetGPSLong(uint8 * pData,uint32 Longitude)
{
    
    pData[0] = (Longitude & 0xFF000000) >> 24;
    pData[1] = (Longitude & 0x00FF0000) >> 16;
    pData[2] = (Longitude & 0x0000FF00) >> 8;
    pData[3] = Longitude & 0x000000FF;
    return 4;
}

uint8 HYHL_Func_GetGPSSateNum(uint8 * pData,uint8 sate_num)
{
    pData[0] = (sate_num & 0x0F)+0xC0;
    return 1;
}

uint8 HYHL_Func_GetGPSSpeed(uint8 * pData,uint32 speed)
{
    uint8 sp = (uint8)(speed/1000);
    pData[0] = sp;
    return 1;
}

uint8 HYHL_Func_GetGPSDir(uint8 * pData,uint8 is_ok,uint8 Lat_Dir,uint8 long_Dir,uint16 cog)
{
/*
    BYTE_1 Bit7 0
    Bit6 0
    Bit5
    GPS 实时/差分定位
    Bit4
    GPS 定位已否
    Bit3
    东经、西经
    Bit2
    南纬、北纬
    Bit1
    航向
*/
    uint8 status = 0;
    uint16 cur_cog = cog/10;
    uint8 high = cur_cog>>8;
    uint8 low = cur_cog&0x00FF;
    status = high & 0x03;
    if(is_ok)
    {
        status = status | 0x10;
    }
    
    if(Lat_Dir == 'N') //N-S 北纬
    {
        status = status | 0x04;
    }
    
    if(long_Dir == 'W') //W-E 西经
    {
        status = status | 0x08;
    }
    
    pData[0] = status;
    pData[1] = low;
    return 2;
}



uint16 HYHL_Func_GetSSInfo(uint8 * pData)
{
/*
    LBS 信息 LBS 数量 1
    LBS 信号强度 1
    PN(MCC) 2
    SID(MNC) 2
    NID(LAC) 2
    BID(CELLID) 3
    纬度 4
    经度 4
*/
    MMIZDT_Station_Update();
    ZDT_LOG("HYHL_Func_GetSSInfo rssiLevel=%d, mcc=%d,,mnc=%d,lac=%d,cell_id=%d",\
    g_zdt_station_data.s_cell.rssiLevel,g_zdt_station_data.s_cell.mcc,g_zdt_station_data.s_cell.mnc,g_zdt_station_data.s_cell.lac,g_zdt_station_data.s_cell.cell_id);
    pData[0] = 1;
    pData[1] = g_zdt_station_data.s_cell.rssiLevel;
    pData[2] = g_zdt_station_data.s_cell.mcc >> 8;
    pData[3] = g_zdt_station_data.s_cell.mcc & 0x00FF;
    pData[4] = g_zdt_station_data.s_cell.mnc >> 8;
    pData[5] = g_zdt_station_data.s_cell.mnc & 0x00FF;
    pData[6] = g_zdt_station_data.s_cell.lac >> 8;
    pData[7] = g_zdt_station_data.s_cell.lac & 0x00FF;
    pData[8] = 0;
    pData[9] =  g_zdt_station_data.s_cell.cell_id >> 8;
    pData[10] = g_zdt_station_data.s_cell.cell_id & 0x00FF;
    pData[11] = 0;
    pData[12] = 0;
    pData[13] = 0;
    pData[14] = 0;
    pData[15] = 0;
    pData[16] = 0;
    pData[17] = 0;
    pData[18] = 0;
    return 19;
}

#endif

