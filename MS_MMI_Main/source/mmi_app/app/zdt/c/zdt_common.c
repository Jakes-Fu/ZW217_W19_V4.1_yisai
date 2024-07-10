
#include "zdt_common.h"
#include "zdt_app.h"
#include "mmiphone_charge.h"

#define ZDT_POWER10or16_GRADE  8
#define ZDT_SYSTEM_START_YEAR    (1970)//TENCENT系统的起始年份

typedef struct 
{
    unsigned short 	wYear;
    unsigned short	nMonth;
    unsigned short	nDay;
    unsigned short	nHour;
    unsigned short	nMin;
    unsigned short	nSec;
}ZDTTime;

BOOLEAN           g_zdt_is_charge = FALSE;
uint8                 g_zdt_is_bat_low = 0;
uint8                 g_zdt_is_bat_full = 0;
BOOLEAN           g_zdt_is_auto_answer = FALSE;
BOOLEAN           g_zdt_bt_is_on = FALSE;
uint8                g_zdt_lowbat_time_id = 0;
uint8                g_zdt_lowbat_play_times  = 0;
BOOLEAN           g_zdt_need_lowbat_send = FALSE;

const uint32 g_dwaZdtPower10[ZDT_POWER10or16_GRADE] = 
{
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000
};

// Power of 16
const uint32 g_dwaZdtPower16[ZDT_POWER10or16_GRADE] = 
{
    1, 16, 256, 4096, 65536, 1048576, 16777216, 268435456
};

PUBLIC BOOLEAN is_all_number(char* data, uint16 len)
{
	char * temp=data;
	uint16 index = 0;
	while ((*temp != 0) && (index <len))
	{
		index++;
		switch(*temp)
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				temp++;
				break;
			default:
				return FALSE;
		}
	}
	return TRUE;
}

//MEID十进制转换用
static uint16 MEID_Hex2Dec_2(uint8* ch)
{
	uint16 dec =0;
	uint16 dec1=0;
	uint16 dec2=0;
	uint16 dec3=0;
	uint16 dec4=0;
	uint8   tmp[2]={0};
	
	if ((ch[0]>='0')&&(ch[0]<='9'))
	{
		dec1 = (ch[0]-0x30);
	}
	else return 0;

	if ((ch[1]>='0')&&(ch[1]<='9'))
	{
		dec2 = (ch[1]-0x30)*2;
	}
	else return 0;

       dec3 = dec2/10;
       dec4 = dec2%10;
       	
	dec = dec1 +dec3 + dec4;
	return dec;
}

//MEID十六进制转换用
static uint16 MEID_Hex2Dec(uint8* ch)
{
	uint16 dec =0;
	uint16 dec1=0;
	uint16 dec2=0;
	uint16 dec3=0;
	uint16 dec4=0;
	uint8   tmp[2]={0};
	
	if ((ch[0]>='0')&&(ch[0]<='9'))
	{
		dec1 = (ch[0]-0x30);
	}
	else if((ch[0]>='A')&&(ch[0]<='F'))
	{
		dec1 = (ch[0]-'A'+10);
	}
	else if((ch[0]>='a')&&(ch[0]<='f'))
	{
		dec1 = (ch[0]-'a'+10);
	}
	else return 0;

	if ((ch[1]>='0')&&(ch[1]<='9'))
	{
		dec2 = (ch[1]-0x30)*2;
	}
	else if((ch[1]>='A')&&(ch[1]<='F'))
	{
		dec2 = (ch[1]-'A'+10)*2;
	}
	else if((ch[1]>='a')&&(ch[1]<='f'))
	{
		dec2 = (ch[1]-'a'+10)*2;
	}
	else return 0;

       dec3 = dec2/16;
       dec4 = dec2%16;
       	
	dec = dec1 +dec3 + dec4;
	return dec;
}

uint8 ZDT_Nibble2HexChar(uint8 bNibble)
{
    uint8  bRtn = '0';

    if(bNibble <= 9)
        bRtn = bNibble + '0';
    else if(bNibble <= 0x0F)
    {
        switch(bNibble)
        {
            case 10:
                bRtn = 'A';
                break;
            case 11:
                bRtn = 'B';
                break;
            case 12:
                bRtn = 'C';
                break;
            case 13:
                bRtn = 'D';
                break;
            case 14:
                bRtn = 'E';
                break;
            case 15:
                bRtn = 'F';
                break;
        }
    }
    return(bRtn);
}

uint8 ZDT_HexChar2Nibble(uint8 bHexChar)
{
    uint8  bRtn = 0;

    if(bHexChar >= '0' && bHexChar <= '9')
        bRtn = bHexChar - '0';
    else
    {
        switch(bHexChar)
        {
            case 'A':
            case 'a':
                bRtn = 10;
                break;
            case 'B':
            case 'b':
                bRtn = 11;
                break;
            case 'C':
            case 'c':
                bRtn = 12;
                break;
            case 'D':
            case 'd':
                bRtn = 13;
                break;
            case 'E':
            case 'e':
                bRtn = 14;
                break;
            case 'F':
            case 'f':
                bRtn = 15;
                break;
        }
    }
    return(bRtn);
}

uint8 ZDT_GetNextPara(T_ATP_GET_PARA_TYPE tType, uint8** pCmd, uint16* pLen, uint32* pNum, uint8* pStr, uint16 wMaxStrLen)
{
    uint8  bRtn = 1, c, bHaveNextComma = FALSE, bDoubleQuotes = 0, MaxNumLen = ZDT_POWER10or16_GRADE;
    int    i, L = 0, EndIndex = 0;

    for(i=0; i < *pLen; i++)
    {
        if(tType == ATP_GET_STR_WITH_DQ)
        {
            if(bDoubleQuotes < 2)
            {
                if(*(*pCmd + i) == '"')
                    bDoubleQuotes ++;
                continue;
            }
        }
        if( *(*pCmd + i) == ',')    // Find next comma, that is, delimiter
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
    
    EndIndex = i;    // maybe EndIndex = *pLen, that is, end of command string and no comma any more

    if(EndIndex)
    {
        if(tType == ATP_GET_NUM_DEC)  // Get decimal number
        {
            *pNum = 0;
            for(i=EndIndex-1; i>=0; i--)  // Scan from the last digit
            {
                c = *(*pCmd + i);
                if( c >= '0' && c <= '9')
                {
                    if(MaxNumLen)
                    {
                        *pNum += ( (c - '0') * g_dwaZdtPower10[L++] );
                        MaxNumLen --;
                    }
                }
                else
                {   // Invalid digit
                    bRtn = 0;
                    break;
                }
            }
        }
        else if(tType == ATP_GET_NUM_HEX)  // Get hexadecimal number
        {
            *pNum = 0;
            for(i=EndIndex-1; i>=0; i--)  // Scan from the last digit
            {
                c = *(*pCmd + i);
                if( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
                {
                    if(MaxNumLen)
                    {
                        *pNum += ( ZDT_HexChar2Nibble(c) * g_dwaZdtPower16[L++] );
                        MaxNumLen --;
                    }
                }
                else
                {   // Invalid digit
                    bRtn = 0;
                    break;
                }
            }
        }
        else    // Get string
        {
            bDoubleQuotes = 0;
            if(!wMaxStrLen)  // No limit if NULL
                wMaxStrLen = 512;
            for(i=0; i<EndIndex; i++)
            {
                if( i == 0 || i == EndIndex-1 )    // Check double quotes at 1st and last one
                {
                    if(tType==ATP_GET_STR_WITH_DQ && *(*pCmd + i)=='"')
                    {
                        bDoubleQuotes ++;
                        continue;
                    }
                }
                if(wMaxStrLen)
                {
                    *(pStr + L) = *(*pCmd + i);
                    L ++;
                    wMaxStrLen --;
                }
            }
            if(tType==ATP_GET_STR_WITH_DQ && bDoubleQuotes!=2)
            {   // Without a pair of double quotes
                bRtn = 0;
            }
        }
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

uint16  ZDT_IsAllNumber(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len //[IN]  源String的长度，byte数
                             )
{
    uint16 i = 0;
    for(i = 0; i < src_len; i++)
    {
        if( src_ptr[i] < '0' || src_ptr[i] > '9')
        {
            return i;
        }
    }
    return i;
}

uint16 ZDT_Str16_to_Int(
                           uint8*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           uint8*  dest_ptr //[OUT] String的目标指针
                           )
{
    uint16  i,  j;
    uint8 high = 0;
    uint8 low = 0;
    uint16 len = 0;
    if(src_len == 0)
    {
        return 0;
    }
    
    // 转换成ucs2
    for(i = 0; i < src_len; i++)
    {
        j = i%2;
        if(j == 0)
        {
            if(src_ptr[i] >= 0x30 &&  src_ptr[i] < 0x3a)
            {
                high = (src_ptr[i] - 0x30) << 4;
            }
            else if(src_ptr[i] >= 0x41 && src_ptr[i] <= 0x46)
            {
                high = (src_ptr[i] - 0x41 + 10) << 4;
            }
            else if(src_ptr[i] >= 0x61 && src_ptr[i] <= 0x66)
            {
                high = (src_ptr[i] - 0x61 + 10) << 4;
            }
        }
        else
        {
            if(src_ptr[i] >= 0x30 &&  src_ptr[i] < 0x3a)
            {
                low = src_ptr[i] - 0x30;
            }
            else if(src_ptr[i] >= 0x41 && src_ptr[i] <= 0x46)
            {
                low = src_ptr[i] - 0x41 + 10;
            }
            else if(src_ptr[i] >= 0x61 && src_ptr[i] <= 0x66)
            {
                low = src_ptr[i] - 0x61 + 10;
            }
            dest_ptr[i/2] = high + low;
        }
        
    }
    
    return src_len/2;
}

uint16 ZDT_UCS_Str16_to_uint16(
                           uint8*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           uint16*  dest_ptr, //[OUT] String的目标指针
                           uint16 dest_max_len
                           )
{
    uint16 i = 0;
    uint16 len = 0;
    uint8 * tmp_buf = NULL;
    uint16 tmp;
    tmp_buf = SCI_ALLOC_APPZ(src_len);
    SCI_MEMSET(tmp_buf,0,src_len);
    len = ZDT_Str16_to_Int(src_ptr,src_len,tmp_buf);
    len = len/2;
    for(i = 0; i < len; i++)
    {
        if(i < dest_max_len)
        {
            tmp = tmp_buf[i*2];
            tmp = tmp << 8;
            tmp += tmp_buf[i*2+1];
            dest_ptr[i] = tmp;
        }
        else
        {
            break;
        }
    }
    SCI_FREE(tmp_buf);
    return i;
}

uint32  ZDT_Get_DayTimeSecond(void)
{
    uint32 daysecond;
    SCI_TIME_T          time;
    TM_GetSysTime(&time);
    daysecond = (time.hour* 3600) + (time.min* 60) + time.sec;
    return daysecond;
}

uint32  ZDT_Get_TimeSecond(void)
{
    SCI_TM_T sys_time = {0};
    uint32   second = 0;

    TM_GetTime(&sys_time);
    second = MMIAPICOM_Tm2Second(sys_time.tm_sec, sys_time.tm_min, sys_time.tm_hour, sys_time.tm_mday, sys_time.tm_mon, sys_time.tm_year);

    return second;
}

//解决支付宝闰年的天数计算错误问题 added by bao 20240102
static BOOLEAN  ZDT_IsLeapYear(uint32 year)
{
	BOOLEAN ret_val = FALSE;

	ret_val = (BOOLEAN)((!(year % 4) && (year % 100)) || !(year % 400));

	return ret_val;
}


static uint32 ZDTTimeAllDay(ZDTTime a)   
{     
    uint32   notrun[12] = {31,28,31,30,31,30,31,31,30,31,30,31};   
    uint32   x = 0, i = 0;   

    //解决支付宝闰年的天数计算错误问题 Modified by bao 20240102
    //x = (a.wYear)*365 + a.wYear/4 + a.wYear/400 - a.wYear/100;
    for(i = 0; i < a.wYear; i++)
    {
        x += (365 + ZDT_IsLeapYear(i));
    }
    
    if (ZDT_IsLeapYear(a.wYear))
    {   
        notrun[1] = 29;
    }
    
    for (i = 0; i < a.nMonth; i++)
    {   
        if (i > 0)
        {   
            x += notrun[i-1];   
        }   
    }   
    
    x += a.nDay;
    
    return  x;   
}

static uint32 ZDTTimeInterval(ZDTTime a, ZDTTime b)   
{   
    uint32   x_days = 0, y_days = 0;   
    uint32   sum = 0;   

    //SCI_TRACE_LOW("[QQ]ZDTTimeInterval");

    x_days = ZDTTimeAllDay(a);   
    y_days = ZDTTimeAllDay(b);   
    
    sum = (x_days-y_days)*24 + (a.nHour -b.nHour) - 8;//hours 差   
    return  (sum*3600 + (a.nMin-b.nMin)*60 + (a.nSec - b.nSec));   
}

static uint32 ZDTTimeIntervalBJ(ZDTTime a, ZDTTime b)   
{   
    uint32   x_days = 0, y_days = 0;   
    uint32   sum = 0;   

    //SCI_TRACE_LOW("[QQ]ZDTTimeInterval");

    x_days = ZDTTimeAllDay(a);   
    y_days = ZDTTimeAllDay(b);   
    
    sum = (x_days-y_days)*24 + (a.nHour -b.nHour) - 8;//hours 差   
    return  (sum*3600 + (a.nMin-b.nMin)*60 + (a.nSec - b.nSec));   
}

static void ZDTGetDateTime(ZDTTime* time_ptr)
{
    SCI_DATE_T date = {0};
    SCI_TIME_T time = {0};
    
    //SCI_TRACE_LOW("[QQ]ZDTGetDateTime");

    if (PNULL == time_ptr)
    {
        return;
    }

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);

    time_ptr->wYear = date.year;
    time_ptr->nMonth = date.mon;
    time_ptr->nDay = date.mday;
    time_ptr->nHour = time.hour;
    time_ptr->nMin = time.min; 
    time_ptr->nSec = time.sec;
}

extern uint32 ZDTGetUTCTime(void)
{
    ZDTTime time = {0};
    ZDTTime startTime = {0};
    ZDTTime time1 = {0};

    ZDTGetDateTime(&time1);

    startTime.wYear = ZDT_SYSTEM_START_YEAR;
    startTime.nMonth = 1;
    startTime.nDay = 1;
    startTime.nHour = 0;
    startTime.nMin = 0;
    startTime.nSec = 0;

    time.wYear =  time1.wYear;
    time.nMonth = time1.nMonth;
    time.nDay = time1.nDay;
    time.nHour = time1.nHour;
    time.nMin = time1.nMin;
    time.nSec = time1.nSec;

    return ZDTTimeInterval(time, startTime);
}

extern uint32 ZDTGetBJTime(void)
{
    ZDTTime time = {0};
    ZDTTime startTime = {0};
    ZDTTime time1 = {0};

    ZDTGetDateTime(&time1);

    startTime.wYear = ZDT_SYSTEM_START_YEAR;
    startTime.nMonth = 1;
    startTime.nDay = 1;
    startTime.nHour = 0;
    startTime.nMin = 0;
    startTime.nSec = 0;

    time.wYear =  time1.wYear;
    time.nMonth = time1.nMonth;
    time.nDay = time1.nDay;
    time.nHour = time1.nHour;
    time.nMin = time1.nMin;
    time.nSec = time1.nSec;

    return ZDTTimeIntervalBJ(time, startTime);
}

extern uint64 ZDTGetSystemTimeMS(void)
{
    static uint64 s_last_utc_ms = 0;
    static uint32 s_last_tick_count = 0;
    static uint32 s_last_cur_tick = 0;
    uint64 ret = 0;
    uint64 temp = 0;
    uint32 cur_tick;
    uint32 cur_sec;


    cur_tick = SCI_GetTickCount();
        
    if(s_last_utc_ms > 0 && s_last_tick_count > 0 && cur_tick > s_last_tick_count && cur_tick >= s_last_cur_tick)
    {
        cur_tick = cur_tick - s_last_tick_count;
        temp = cur_tick;
        ret = s_last_utc_ms + temp;
    }
    else
    {
        s_last_tick_count = cur_tick;
        cur_tick = cur_tick%1000;
        cur_sec = ZDTGetUTCTime();
        temp = cur_sec;
        s_last_utc_ms = temp*1000;
        temp = cur_tick;
        s_last_utc_ms += temp;
        ret = s_last_utc_ms;
    }
    s_last_cur_tick = cur_tick;
    return ret;
}
BOOLEAN ZDT_GetIsCharge(void)
{
    return g_zdt_is_charge;
}

void ZDT_SetIsCharge(BOOLEAN is_charge)
{
    if(g_zdt_is_charge != is_charge)
    {
        g_zdt_is_charge = is_charge;
#ifdef ZDT_LED_SUPPORT
        if(is_charge)
        {
            ZDT_Led_Start_Charge();
        }
        else
        {
            ZDT_Led_Start_ChargeOver();
        }
#endif
        if(is_charge)
        {
#ifdef ZDT_PLAT_YX_SUPPORT_LZ
            YX_Net_Send_ALARM_POWER(&g_yx_app,5);
#endif
#ifdef ZDT_LCD_NOT_SUPPORT
#ifdef ZDT_PLAY_RING_CUSTOM
             MMIAPISET_PlayCustomRing(MMISET_CUST_CHARGING,1);
#endif
#endif
        }
        else
        {
#ifdef ZDT_PLAT_YX_SUPPORT_LZ
            YX_Net_Send_ALARM_POWER(&g_yx_app,6);
#endif
        }
    }
	return;
}

uint8 ZDT_GetIsBatLow(void)
{
    return g_zdt_is_bat_low;
}

BOOLEAN ZDT_Get_SIM_Number(MN_DUAL_SYS_E dual_sys,char * sim_number)
{
    if(sim_number == PNULL)
    {
        return FALSE;
    }

    if (MMIAPIPHONE_GetSimExistedStatus(dual_sys))
    {
        PHONEBOOK_ENTRY_T sim_contact = {0};

        sim_contact.entry_id = 1;

        PHONEBOOK_ReadPhonebookEntryEx(dual_sys, PHONEBOOK_MSISDN_STORAGE, &sim_contact);

        if(sim_contact.number_len > 0)
        {
            MMIAPICOM_GenDispNumber(MMIPB_GetNumberTypeFromUint8(sim_contact.ton),
                sim_contact.number_len, sim_contact.number, sim_number, 127);
            SCI_TraceLow("ZDT_Get_SIM_Number sim 1 number = %s", sim_number);
            return TRUE;
        }
    }
    return FALSE;
}

static void ZDT_LowBat_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    g_zdt_lowbat_play_times++;
    if(g_zdt_lowbat_play_times < 3)
    {
#if defined(ZDT_PLAT_YX_SUPPORT_LZ) || defined(ZDT_PLAT_YX_SUPPORT_CY)
        if(FALSE == YX_API_Mute_IsValid())
#endif
#ifdef ZDT_PLAT_SHB_SUPPORT
        if(FALSE == SHB_DB_TIMER_IsValid_Slient())
#endif
        {
#ifdef ZDT_LCD_NOT_SUPPORT
#ifdef ZDT_PLAY_RING_CUSTOM
            MMIAPISET_PlayCustomRing(MMISET_CUST_LOW_BAT,1);
#endif
#endif
        }
    }
    return;
}

void ZDT_LowBat_TimeStart(uint16 time_s)
{
    if(0 != g_zdt_lowbat_time_id)
    {
        MMK_StopTimer(g_zdt_lowbat_time_id);
        g_zdt_lowbat_time_id = 0;
    }
    
    g_zdt_lowbat_time_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        ZDT_LowBat_TimeOut, 
                                                                        (uint32)0, 
                                                                        TRUE);
    return;
}

void ZDT_LowBat_TimeStop(void)
{
    if(0 != g_zdt_lowbat_time_id)
    {
        MMK_StopTimer(g_zdt_lowbat_time_id);
        g_zdt_lowbat_time_id = 0;
    }
}

void ZDT_SetIsBatLow(uint8 is_low)
{
    if(g_zdt_is_bat_low != is_low)
    {
        g_zdt_is_bat_low = is_low;
        if(is_low)
        {
#if defined(ZDT_PLAT_YX_SUPPORT_LZ) || defined(ZDT_PLAT_YX_SUPPORT_CY)
                if(YX_Net_Is_Land())
                {
                    g_zdt_need_lowbat_send = TRUE;
			YX_Net_Send_ALARM_POWER(&g_yx_app,1);
                }
#endif
#ifdef ZDT_PLAT_SHB_SUPPORT
            SHB_HTTP_ZTE1001_Send(0);
#endif
#if defined(ZDT_PLAT_YX_SUPPORT_LZ) || defined(ZDT_PLAT_YX_SUPPORT_CY)
            if(YX_API_Mute_IsValid())
            {
                ZDT_LowBat_TimeStop();
            }
#endif
#ifdef ZDT_PLAT_SHB_SUPPORT
            if(SHB_DB_TIMER_IsValid_Slient())
            {
                ZDT_LowBat_TimeStop();
            }
            else
#endif
            {
                g_zdt_lowbat_play_times = 0;
#ifdef ZDT_LCD_NOT_SUPPORT
#ifdef ZDT_PLAY_RING_CUSTOM
                MMIAPISET_PlayCustomRing(MMISET_CUST_LOW_BAT,1);
                ZDT_LowBat_TimeStart(600); //300
#endif
#endif
            }
        }
        else
        {
            g_zdt_need_lowbat_send = FALSE;
#ifdef ZDT_PLAT_SHB_SUPPORT
            //SHB_HTTP_ZTE1001_Send(0);
#endif
            ZDT_LowBat_TimeStop();
        }
#ifdef ZDT_PLAT_YX_SUPPORT
        YX_API_BatStatus_Post(1,10);
#endif
    }
    else
    {
        if(is_low > 0 && g_zdt_need_lowbat_send)
        {
#if defined(ZDT_PLAT_YX_SUPPORT_LZ) || defined(ZDT_PLAT_YX_SUPPORT_CY)
            if(YX_Net_Is_Land())
            {
                g_zdt_need_lowbat_send = FALSE;
                YX_Net_Send_ALARM_POWER(&g_yx_app,1);
            }
#endif
        }
    }
    return;
}

uint8 ZDT_GetIsBatFull(void)
{
    return g_zdt_is_bat_full;
}

void ZDT_SetIsBatFull(uint8 is_full)
{
    if(g_zdt_is_bat_full != is_full)
    {
        g_zdt_is_bat_full = is_full;
        if(is_full)
        {
#ifdef ZDT_PLAT_YX_SUPPORT_LZ
            YX_Net_Send_ALARM_POWER(&g_yx_app,7);
#endif
            if(TRUE == g_zdt_is_charge 
#if defined(ZDT_PLAT_YX_SUPPORT_LZ) || defined(ZDT_PLAT_YX_SUPPORT_CY)
                && FALSE == YX_API_Mute_IsValid()
#endif
#ifdef ZDT_PLAT_SHB_SUPPORT
                && FALSE == SHB_DB_TIMER_IsValid_Slient()
#endif
                )
            {
#ifdef ZDT_LCD_NOT_SUPPORT
#ifdef ZDT_PLAY_RING_CUSTOM
                 MMIAPISET_PlayCustomRing(MMISET_CUST_CHARGFULL,1);
#endif
#endif
            }
        }
#ifdef ZDT_LED_SUPPORT
        if(is_full)
        {
            ZDT_Led_Start_ChargeFull();
        }
#endif
    }
    return;
}

PUBLIC uint8 ZDT_GetBatteryPercent()
{
	
	CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
	//ZDT_LOG("adp_type=%d,bat_cur_vol=%d,bat_remain_cap=%d,bat_statistic_vol=%d,charging_current=%d,charging_stop_reason=%d,chgmng_state=%d,chg_vol=%d,",p_chgmng_info->adp_type,p_chgmng_info->bat_cur_vol,p_chgmng_info->bat_remain_cap,p_chgmng_info->bat_statistic_vol,
		//p_chgmng_info->charging_current,p_chgmng_info->charging_stop_reason,p_chgmng_info->chgmng_state,p_chgmng_info->chg_vol);
	return p_chgmng_info->bat_remain_cap>(BATTERY_CHARGE_FULL_CAP-1)?100:p_chgmng_info->bat_remain_cap;
}

PUBLIC BOOLEAN ZDT_IsChargeDone()
{
    BOOLEAN ret = FALSE;
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    //ZDT_LOG("adp_type=%d,bat_cur_vol=%d,bat_remain_cap=%d,bat_statistic_vol=%d,charging_current=%d,charging_stop_reason=%d,chgmng_state=%d,chg_vol=%d,",p_chgmng_info->adp_type,p_chgmng_info->bat_cur_vol,p_chgmng_info->bat_remain_cap,p_chgmng_info->bat_statistic_vol,
    //	p_chgmng_info->charging_current,p_chgmng_info->charging_stop_reason,p_chgmng_info->chgmng_state,p_chgmng_info->chg_vol);
    //if(p_chgmng_info->bat_remain_cap >= 98 && p_chgmng_info->charging_stop_reason == CHGMNG_CHARGEDONE && p_chgmng_info->charging_current == 0 )
    if(p_chgmng_info != PNULL)
    {
		if(p_chgmng_info->charging_stop_reason == CHGMNG_CHARGEDONE)
        {
            ret = TRUE;
        }
    }
    return ret;
}

PUBLIC BOOLEAN ZDT_IsRealCharingVol()
{
	BOOLEAN ret = TRUE;
	CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
	
    if(p_chgmng_info->chg_vol <= 3300)
	{
		ret = FALSE;
	}

	return ret;
}


PUBLIC BOOLEAN ZDT_IsInCharging()
{
	BOOLEAN ret = FALSE;
	CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
	
	if(p_chgmng_info->chgmng_state == CHGMNG_CHARGING || p_chgmng_info->chgmng_state == CHGMNG_STARTING)
	{
		ret = TRUE;
	}

	return ret;
}

BOOLEAN ZDT_GetIsAutoAnswer(void)
{
    return g_zdt_is_auto_answer;
}

void ZDT_SetIsAutoAnswer(BOOLEAN is_auto)
{
    g_zdt_is_auto_answer = is_auto;
    return;
}

BOOLEAN ZDT_GetIsBTOn(void)
{
    return g_zdt_bt_is_on;
}

void ZDT_SetIsBTOn(BOOLEAN is_on)
{
    g_zdt_bt_is_on = is_on;
    return;
}

BOOLEAN ZDT_SIM_Exsit(void)
{
    uint32 sim_ok_num = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;

    sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);
    if(sim_ok_num ==0)
    {
        return FALSE;
    }
    return TRUE;
}

static uint16  ZDT_DeleteStrHeadArea(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len, //[IN]  源String的长度，byte数
                             uint8*  out_str
                             )
{
    uint16 i = 0;
    uint16 head_len = 0;
    if(src_ptr != PNULL && src_len > 5)
    {
        if(strncmp(src_ptr, "0086", 4) == 0)
        {
            head_len = 4;
        }
        else if(strncmp(src_ptr, "+86", 3) == 0)
        {
            head_len = 3;
        }
        else if(strncmp(src_ptr, "00852", 5) == 0 || strncmp(src_ptr, "00853", 5) == 0 || strncmp(src_ptr, "00886", 5) == 0)
        {
            head_len = 5;
        }
        else if(strncmp(src_ptr, "+852", 4) == 0 || strncmp(src_ptr, "+853", 4) == 0 || strncmp(src_ptr, "+886", 4) == 0)
        {
            head_len = 4;
        }
        SCI_MEMCPY(out_str,src_ptr+head_len,src_len-head_len);
        i = src_len-head_len;
    }
    else
    {
        SCI_MEMCPY(out_str,src_ptr,src_len);
        i = src_len;
    }
    return i;
}

static uint16  ZDT_DeleteStrHeadZero(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len, //[IN]  源String的长度，byte数
                             uint8*  out_str
                             )
{
    uint16 i = 0;
    uint16 j = 0;
    for(i = 0; i < src_len; i++)
    {
        if( src_ptr[i] == '0')
        {
            j = i+1;
        }
        else
        {
            break;
        }
    }
    
    SCI_MEMCPY(out_str,src_ptr+j,src_len-j);
    
    return src_len-j;
}

uint16  ZDT_DeleteStrHead(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len, //[IN]  源String的长度，byte数
                             uint8*  out_str
                             )
{
    uint8  head_str[101] = {0};
    uint16 head_len = 0;
    uint16 out_len = 0;
    
    head_len = ZDT_DeleteStrHeadArea(src_ptr,src_len,head_str);
    
    out_len = ZDT_DeleteStrHeadZero(head_str,head_len,out_str);
    
    return out_len;
}
#if 1
BOOLEAN  ZDT_DBFile_Write(char * path,char * filename, uint8 * data_ptr, uint32  data_len)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    BOOLEAN ret = FALSE;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;
     
     if(MMIFILE_IsFileExist(full_path,full_path_len))
     {
         MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
     }
        
	file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
	if(SFS_INVALID_HANDLE != file_handle)
	{
		file_ret = MMIFILE_WriteFile(
			file_handle, 
			data_ptr, 
			data_len, 
			&transbyte, 
			NULL
			);
        
		if(SFS_ERROR_NONE == file_ret)
		{
			ret = TRUE;
		}
		MMIFILE_CloseFile(file_handle);
	}

    return ret;
	
}

BOOLEAN  ZDT_DBFile_Exist(char * path, char * filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;
     
     return MMIFILE_IsFileExist(full_path,full_path_len);	
}

uint32  ZDT_DBFile_GetSize(char * path, char * filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    MMIFILE_HANDLE file_handle = 0;
    uint32   file_size          = 0;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;

     file_handle = MMIAPIFMM_CreateFile(full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != file_handle) 
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        MMIAPIFMM_CloseFile(file_handle);
    }
    return file_size;	
}

BOOLEAN  ZDT_DBFile_Read(char * path, char * filename, void *buffer, uint32 bytes_to_read, uint32 *bytes_read)
{
    BOOLEAN ret = FALSE;
    FFS_ERROR_E error = FFS_NO_ERROR;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
	  
    for(i = 0; i < tmp_len; i++)
    {
        full_path[full_path_len++] = file_dev_ptr[i];
    }
        
    full_path[full_path_len++] = ':';
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
    full_path_len += tmp_len;
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
    full_path_len += tmp_len;
        
    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_OPEN_EXISTING |SFS_MODE_READ, NULL, NULL);
    }
    else
    {
        return FALSE;
    }

    if(SFS_INVALID_HANDLE != file_handle)
    {
    	file_ret = MMIFILE_ReadFile(
                              file_handle, 
                              buffer, 
                              bytes_to_read, 
                              bytes_read, 
                              NULL
                             );
        
        if(SFS_ERROR_NONE != file_ret)
        {
            error = FFS_ERROR_INVALID_PARAM;
        }
        else
        {
            ret = TRUE;
        }
        MMIFILE_CloseFile(file_handle);
    }

    return ret;
}

BOOLEAN  ZDT_DBFile_RemoveFile(char * path, char * filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;
     if(MMIFILE_IsFileExist(full_path,full_path_len))
     {
        MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
     }
     return TRUE;	
}

BOOLEAN  ZDT_DBFile_RemoveDir(char * path	)
{
    BOOLEAN ret = FALSE;
    SFS_HANDLE	hFind    = PNULL;
    SFS_FIND_DATA_T sfsfind             = {0};
    SFS_ERROR_E		result              = SFS_NO_ERROR;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;

    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
	  
    for(i = 0; i < tmp_len; i++)
    {
        full_path[full_path_len++] = file_dev_ptr[i];
    }

    full_path[full_path_len++] = ':';
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
    full_path_len += tmp_len;        
    if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
    {
        full_path[full_path_len++] = 0x5c;
    }

    full_path[full_path_len] = 0x2a;
    full_path[full_path_len+1] = 0;

    hFind  = SFS_FindFirstFile(full_path,&sfsfind );

    if( hFind == PNULL)
    {
        return TRUE;
    }

    do
    {
        uint32 uNameLen = 0;
        uNameLen = MMIAPICOM_Wstrlen( (wchar *)sfsfind.name );
        SCI_MEMCPY(&full_path[full_path_len], sfsfind.name, MIN(uNameLen*2+2, MMIFILE_FULL_PATH_MAX_LEN));
        full_path[full_path_len+uNameLen] = '\0';

        // 是目录
        if( sfsfind.attr & SFS_ATTR_DIR )
        {
        }
        else
        {
            //删除文件
            result = MMIFILE_DeleteFile(full_path, PNULL);

            if( SFS_NO_ERROR != result )
            {
                SFS_FindClose(hFind);
                return FALSE;
            }
        }

        result = SFS_FindNextFile(hFind, &sfsfind);

    }while(SFS_NO_ERROR == result);

    SFS_FindClose(hFind);

    return TRUE;
}
#endif

BOOLEAN ZDT_File_MemoryExsit(BOOLEAN is_tf)
{
    MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_UDISK;

    if(is_tf)
    {
       sd_type = MMI_DEVICE_SDCARD;
    }
    
    if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
    {
        return TRUE;
    }
    return FALSE;
}

BOOLEAN ZDT_File_MemFull(BOOLEAN is_tf)
{
    uint32          free_space_high     = 0;
    uint32          free_space_low      = 102400;
    uint32          cur_free_space_high = 0;
    uint32          cur_free_space_low  = 0;
    MMIFILE_ERROR_E file_ret            = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_UDISK;

    if(is_tf)
    {
       sd_type = MMI_DEVICE_SDCARD;
    }
    
    if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
    {
        file_ret = SFS_GetDeviceFreeSpace ( (uint16 *) MMIAPIFMM_GetDevicePath (sd_type),
                                            &cur_free_space_high,
                                            &cur_free_space_low);

        if (file_ret == SFS_ERROR_NONE)
        {
            if (cur_free_space_high > free_space_high //高位大于肯定大于
                    || ( (cur_free_space_high == free_space_high) //高位相等，低位大于
                         && (cur_free_space_low > free_space_low))
               )
            {
                return FALSE;
            }
        }
    }
        
    return TRUE;
}

uint32 ZDT_File_GetMemFree(BOOLEAN is_tf)
{
    uint32          cur_free_space_high = 0;
    uint32          cur_free_space_low  = 0;
    MMIFILE_ERROR_E file_ret            = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_UDISK;

    if(is_tf)
    {
       sd_type = MMI_DEVICE_SDCARD;
    }
    
    if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
    {
        file_ret = SFS_GetDeviceFreeSpace ( (uint16 *) MMIAPIFMM_GetDevicePath (sd_type),
                                            &cur_free_space_high,
                                            &cur_free_space_low);
    }
    return cur_free_space_low/1024;
}

BOOLEAN ZDT_TFCard_Exist(void)
{
    MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_SDCARD;
    
    if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
    {
        return TRUE;
    }
        
    return FALSE;
}

BOOLEAN ZDT_File_Save(const char * file_name,uint8  * data_ptr, uint32 data_len,BOOLEAN is_add)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    BOOLEAN ret = FALSE;


    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    if(is_add)
    {
        if(MMIFILE_IsFileExist(full_path, full_path_len))
        {
            file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_APPEND, NULL, NULL);
        }
        else
        {
            file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
        }
    }
    else
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    }
    
    if(SFS_INVALID_HANDLE != file_handle)
    {
        file_ret = MMIFILE_WriteFile(
        	file_handle, 
        	data_ptr, 
        	data_len, 
        	&transbyte, 
        	NULL
        	);

        if(SFS_ERROR_NONE == file_ret)
        {
        	ret = TRUE;
        }
        MMIFILE_CloseFile(file_handle);
    }

    return ret;
	
}

BOOLEAN ZDT_File_Remove(const char * file_name)
{	
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;
       
    if(MMIFILE_IsFileExist(full_path,full_path_len))
    {
        MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
        return TRUE;
    }

    return FALSE;
}

BOOLEAN ZDT_File_Read(const char * file_name,uint8  * data_ptr, uint32  data_len, uint32 * read_len)
{
    BOOLEAN ret = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    
    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;
    
    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_OPEN_EXISTING |SFS_MODE_READ, NULL, NULL);
    }
    else
    {
        return FALSE;
    }

    if(SFS_INVALID_HANDLE != file_handle)
    {
    	file_ret = MMIFILE_ReadFile(
                              file_handle, 
                              data_ptr, 
                              data_len, 
                              read_len, 
                              PNULL
                             );
        
    	if(SFS_ERROR_NONE == file_ret)
    	{
    		ret = TRUE;
    	}
    	MMIFILE_CloseFile(file_handle);
    }

    return ret;
}

BOOLEAN ZDT_File_Exsit(const char * file_name)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    
    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
        return TRUE;
    }
    
    return FALSE;
}

BOOLEAN ZDT_Dir_Exsit(const char * dir_path)
{
    SFS_HANDLE	hFind    = PNULL;
    SFS_FIND_DATA_T sfsfind             = {0};
    BOOLEAN		result              = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    uint16 i = 0;
    BOOLEAN ret = FALSE;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)dir_path, SCI_STRLEN(dir_path));
    full_path_len = tmp_len;        
    if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
    {
        full_path[full_path_len++] = 0x5c;
    }

    full_path[full_path_len] = 0x2a;
    full_path[full_path_len+1] = 0;

    hFind  = SFS_FindFirstFile(full_path,&sfsfind );

    if( hFind == PNULL)
    {
        return FALSE;
    }

    SFS_FindClose(hFind);

    return TRUE;
}

BOOLEAN ZDT_Dir_Remove(const char * dir_path)
{
    MMIFILE_ERROR_E	result    = PNULL;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)dir_path, SCI_STRLEN(dir_path));
    full_path_len = tmp_len;        
    if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
    {
        full_path[full_path_len++] = 0x5c;
    }

    full_path[full_path_len] = 0x2a;
    full_path[full_path_len+1] = 0;

    result  = MMIFILE_DeleteDirectory(full_path);

    if( result == SFS_ERROR_NONE)
    {
        return TRUE;
    }

    return FALSE;
}

BOOLEAN ZDT_File_RemoveDirFile(const char * dir_path)
{
    SFS_HANDLE	hFind    = PNULL;
    SFS_FIND_DATA_T sfsfind             = {0};
    BOOLEAN		result              = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    uint16 i = 0;
    BOOLEAN ret = FALSE;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)dir_path, SCI_STRLEN(dir_path));
    full_path_len = tmp_len;        
    if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
    {
        full_path[full_path_len++] = 0x5c;
    }

    full_path[full_path_len] = 0x2a;
    full_path[full_path_len+1] = 0;

    hFind  = SFS_FindFirstFile(full_path,&sfsfind );

    if( hFind == PNULL)
    {
        return TRUE;
    }

    do
    {
        uint32 uNameLen = 0;
        uNameLen = MMIAPICOM_Wstrlen( (wchar *)sfsfind.name );
        SCI_MEMCPY(&full_path[full_path_len], sfsfind.name, MIN(uNameLen*2+2, MMIFILE_FULL_PATH_MAX_LEN));
        full_path[full_path_len+uNameLen] = '\0';

        // 是目录
        if( sfsfind.attr & SFS_ATTR_DIR )
        {
        }
        else
        {
            //删除文件
            result = MMIFILE_DeleteFile(full_path, PNULL);

            if( SFS_NO_ERROR != result )
            {
                SFS_FindClose(hFind);
                return FALSE;
            }
        }

        result = SFS_FindNextFile(hFind, &sfsfind);

    }while(SFS_NO_ERROR == result);

    SFS_FindClose(hFind);

    return TRUE;
}

MMIFILE_HANDLE  ZDT_File_Open(const char * file_name,BOOLEAN is_read)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    MMIFILE_HANDLE file_handle = 0;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    if(is_read)
    {
        if(MMIFILE_IsFileExist(full_path, full_path_len))
        {
            file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_OPEN_EXISTING |SFS_MODE_READ, NULL, NULL);
        }
        else
        {
            return SFS_INVALID_HANDLE;
        }
    }
    else
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    }

    return file_handle;
	
}

uint32  ZDT_File_Write(MMIFILE_HANDLE file_handle,uint8  * data_ptr, uint32 data_len)
{
    uint16 j = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    uint32 transbyte = 0;
    uint32 all_transbyte = 0;
    BOOLEAN ret = TRUE;
    if(file_handle == SFS_INVALID_HANDLE)
    {
        return 0;
    }

    do
    {
        transbyte = 0;
        file_ret = MMIFILE_WriteFile(
        	file_handle, 
        	data_ptr, 
        	data_len, 
        	&transbyte, 
        	NULL
        	);
        
        if(SFS_ERROR_NONE != file_ret)
        {
            ret = FALSE;
            break;
        }
        all_transbyte += transbyte;
    }while(transbyte != 0 && all_transbyte < data_len);
    

    return all_transbyte;
	
}

void  ZDT_File_Close(MMIFILE_HANDLE file_handle)
{
    if(file_handle != SFS_INVALID_HANDLE)
    {
        MMIFILE_CloseFile(file_handle);
    }
    return;
	
}

uint32  ZDT_File_GetSize(const char * file_name)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    MMIFILE_HANDLE file_handle = 0;
    uint32   file_size          = 0;
    
     tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
     full_path_len = tmp_len;

     file_handle = MMIAPIFMM_CreateFile(full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != file_handle) 
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        MMIAPIFMM_CloseFile(file_handle);
    }
    return file_size;
}

BOOLEAN  ZDT_File_Rename(const char * file_name, const char * new_filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar   new_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  new_full_path_len = 0;
    uint16   tmp_len = 0;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    tmp_len = GUI_GBToWstr(new_full_path, (const uint8*)new_filename, SCI_STRLEN(new_filename));
    new_full_path_len = tmp_len;

    return MMIAPIFMM_RenameFile(full_path,full_path_len,new_full_path,new_full_path_len);
}

//yangyu add begin
BOOLEAN ZDT_IsStartWith(char* full_str, char* start_str)
{
    if(full_str == NULL || start_str == NULL || SCI_STRLEN(start_str) > SCI_STRLEN(full_str))
    {
        return FALSE;
    }
    while(*start_str != '\0')
    {
        if(*full_str++ != *start_str++)
        {
            return FALSE;
        }

    }
    return TRUE;

}

#define BASE_YEAR  1970
#define TIME_ZONE_SEC  (8*60*60)  //beijing

typedef struct
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;

} date_time_t;



void getMonthAndRemainDay(int year ,int year_days, int *month, int *remain_days);

int getMonthDay(int year, int month);






#if 0
int main()
{
	printf("yu \n");
	int i;
	timestampToDate(1497597457);
	timestampToDate(1598665411);
	timestampToDate(1583199811);


}
#endif



void timestampToDateTime(long time_second, SCI_DATE_T  *sys_date, SCI_TIME_T  *sys_time)
{

	date_time_t date_time = {0};
	long remain_days = 0;
	long remain_sec = 0;
	//printf("yu2 \n");
	int i = 0, j = 0, k = 0;

	long total_sec = 0;
	long last_year_sec = 0;

	int get_year_flag = 0;

	time_second = time_second + TIME_ZONE_SEC; //time zone

	while(!get_year_flag)
	{
		if(MMIAPICOM_IsLeapYear(i + BASE_YEAR))
		{
			total_sec += 366*24*60*60;

		}
		else
		{
			total_sec += 365*24*60*60;
		}
		if(total_sec < time_second)
		{
			i++;
			last_year_sec = total_sec;
		}
		else
		{
			get_year_flag = 1;
		}

	}

	date_time.year = BASE_YEAR + i;
	remain_days = (time_second - last_year_sec)/(24*60*60);
	remain_sec = (time_second - last_year_sec)%(24*60*60);


	getMonthAndRemainDay(date_time.year, remain_days, &date_time.month, &date_time.day );
	//printf("result : %d-%d-%d" , date_time.year ,date_time.month, date_time.day);

	date_time.hour = remain_sec/(60*60);
	date_time.minute = (remain_sec%(60*60))/60;
	date_time.second = (remain_sec%(60*60))%60;

	SCI_TRACE_LOW("timestampToDateTime %ld => %d-%d-%d %02d:%02d:%02d \n" ,time_second, date_time.year ,date_time.month, date_time.day, date_time.hour, date_time.minute,date_time.second);
    sys_date->year = date_time.year;
    sys_date->mon= date_time.month;
    sys_date->mday = date_time.day;


    sys_time->hour = date_time.hour;
    sys_time->min= date_time.minute;
    sys_time->sec= date_time.second;




}

int getMonthDay(int year, int month)
{
	int is_leap_year = MMIAPICOM_IsLeapYear(year);
	int day = 31;
	switch(month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			day = 31;
			break;

		case 4:
		case 6:
		case 9:
		case 11:
			day = 30;
			break;
		case 2:
			day = is_leap_year?29:28;
			break;
	}
	return day;

}

void getMonthAndRemainDay(int year ,int year_days, int *month, int *remain_days)
{
	int i = 0;
	int temp_sum_day = 0;
	//SCI_TRACE_LOW("year : %d, year_days = %d \n" ,year, year_days);
	for(i = 1; i <= 12 ; i++)
	{

		if(year_days >= temp_sum_day && year_days < getMonthDay(year, i) + temp_sum_day)
		{
			*month = i;
			*remain_days = year_days - temp_sum_day + 1; //need plus one ,no have 0 day
			break;
		}
		else
		{
			temp_sum_day += getMonthDay(year, i);
		}
	}

}





PUBLIC void API_WstrAppendStr(wchar *wstr, uint32 w_len, uint8 *str, uint32 s_len)
{
    uint32 i = 0;
    uint32 already_len = 0;
    if(wstr == NULL || str == NULL || w_len == 0 || s_len == 0 || w_len < s_len)
    {
        SCI_TRACE_LOW("API_WstrAppendStr error");
        return;
    }
    already_len = MMIAPICOM_Wstrlen(wstr);
    for(i = 0; i < s_len; i++)
    {
        wstr[already_len+i] = str[i];
    }
}

PUBLIC void API_WstrAppendInt(wchar *wstr, uint32 w_len, int a)
{
    uint8 c_buf[65] = {0};
    
    if(wstr == NULL || w_len == 0 )
    {
        SCI_TRACE_LOW("API_WstrAppendInt error");
        return;
    }
    
    sprintf(c_buf,"%ld", a);
    API_WstrAppendStr(wstr, w_len, c_buf, strlen(c_buf));
}

