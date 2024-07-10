/****************************************************************************
** File Name:      mmicdma_app.c                                           *
** Author:          jianshengqi                                                       *
** Date:           03/22/2006                                              *
** Copyright:      2006 tlt, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshq         Create
** 
****************************************************************************/
#ifndef  _MMI_BEACON_APP_C_
#define _MMI_BEACON_APP_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "beacon_app.h"
#include "cJSON.h"
#include "zdt_app.h"

MMI_APPLICATION_T   		g_beacon_app;
BEACON_DATA_T g_cur_beacon_ts_data = {0};
uint8 g_ib_need_get_57_data = 0;
uint8 g_ib_need_set_01_data = 0;
uint16 g_ib_connect_check_num = 0;
uint32 g_ib_is_timer_disconnect_sec = 0;
#ifdef ZDT_SUPPORT_BEACON_SUNRI
uint8 g_ib_need_send_passwd = 0;
#endif

static BOOLEAN s_beacon_app_init = FALSE;
static uint8 ib_disconnect_wait_timer_id = 0;
static uint8 ib_disconnect_check_timer_id = 0;

extern BOOLEAN g_ib_is_timer_connect;
extern void BEACON_Trace(const char *pszFmt, ...);

void MMIIB_StopDisconnectCheckTimer();

#ifndef abs
#define abs(x) (((x) >= 0) ? (x) : (-(x)))
#endif 
BOOLEAN  BEACON_HTTP_GetInfo(void);
BOOLEAN  BEACON_HTTP_PostInfo(char * name_str,char * mac_str, char * data_str);

static uint8 IB_Nibble2HexChar(uint8 bNibble)
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

static uint32 IB_U8_To_Hex(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len)
{
    uint8 low = 0;
    uint8 high = 0;
    uint32 i = 0;
    uint32 j = 0;

    if((len*2) > dest_len)
    {
        len = dest_len/2;
    }
    for(i = 0; i < len; i++)
    {
        high = buf[i]>>4;
        dest_buf[j++] = (char)IB_Nibble2HexChar(high);
        low = buf[i] & 0x0F;
        dest_buf[j++] = (char)IB_Nibble2HexChar(low);
    }
    dest_buf[j] = 0;
    return j;
}

uint32 BEACON_U8_To_Hex(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len)
{
    uint8 low = 0;
    uint8 high = 0;
    uint32 i = 0;
    uint32 j = 0;

    if((len*2) > dest_len)
    {
        len = dest_len/2;
    }
    for(i = 0; i < len; i++)
    {
        high = buf[i]>>4;
        dest_buf[j++] = (char)IB_Nibble2HexChar(high);
        low = buf[i] & 0x0F;
        dest_buf[j++] = (char)IB_Nibble2HexChar(low);
    }
    dest_buf[j] = 0;
    return j;
}


int  BEACON_Data_GetNextPara(uint8** pCmd, uint16* pLen, char* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == ',')
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

int  BEACON_Data_GetNextParaM(uint8** pCmd, uint16* pLen, char* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == ':')
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

int  BEACON_Data_GetNextParaE(uint8** pCmd, uint16* pLen, char* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == ' ')
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

void MMIIB_StopDisconnectWaitTimer()
{
    if(ib_disconnect_wait_timer_id != 0)
    {
        MMK_StopTimer(ib_disconnect_wait_timer_id);
        ib_disconnect_wait_timer_id = 0;
    }
}

void MMIIB_DisconnectWaitTimerOut(uint8 timer_id,uint32 param)
{
    MMIIB_DisConnect_CB();
    return;
}

void MMIIB_StartDisconnectWaitTimer()
{
    MMIIB_StopConnectCheckTimer();
    MMIIB_StopDisconnectWaitTimer();
    ib_disconnect_wait_timer_id = MMK_CreateTimerCallback(5*1000, 
    											   MMIIB_DisconnectWaitTimerOut, 
    											    (uint32)PNULL,
    											     FALSE);
    return;
}
#ifdef WIN32
BLOCK_ID ZDTIB_TaskGetID(void)
{
	return 0xFFFFFFFF;
}

BOOLEAN beacon_hw_is_ok(void) 
{
    return FALSE;
}
BOOLEAN ZDT_IB_Hw_IsOn(void)
{
    return FALSE;
}

void ZDT_IB_Set_PowerNoOff(BOOLEAN no_off)
{
    return;
}

int ZDT_IB_Init(void)
{    
    return 0;
}

int ZDT_IB_PowerOn(void)
{    
    return 0;
}

int ZDT_IB_PowerOff(void)
{    
    return 0;
}

int ZDT_IB_UartSend(uint8 * send_buf, uint32 send_len)
{    
    return 0;
}

int ZDT_IB_CheckHW_Start(void)
{    
    return 0;
}

int ZDT_IB_CheckHW_End(void)
{    
    return 0;
}


int ZDT_IB_AT_Send_GetVer(void)
{    
    return 0;
}

int ZDT_IB_AT_Send_Wakeup(void)
{    
    return 0;
}

int ZDT_IB_AT_Send_Connect(char * dev_mac_str)
{    
    return 0;
}

int ZDT_IB_AT_Send_DisConnect(void)
{    
    return 0;
}

int ZDT_IB_AT_Send_EntryPassMode(void)
{    
    return 0;
}

int ZDT_IB_AT_Send_ExitPassMode(void)
{    
    return 0;
}

int ZDT_IB_AT_Send_Scan(void)
{    
    return 0;
}

int ZDT_IB_AT_Send_LinkData(uint8 * pData, uint16 DataLen)
{    
    return 0;
}

uint8 ZDT_IB_AT_GetCRC(uint8 * pData, uint16 DataLen)
{
    return 0;
}

int ZDT_IB_AT_Send_41(void)
{    
    return 0;
}

int ZDT_IB_AT_Send_57(void)
{    
    return 0;
}

int ZDT_IB_AT_Send_PWD(void)
{    
    return 0;
}

int ZDT_IB_AT_Send_GetNum(void)
{    
    return 0;
}

int ZDT_IB_AT_Send_DateTime(void)
{    
    return -1;
}

int ZDT_IB_AT_Send_58(void)
{    
    return 0;
}

int ZDT_IB_AT_Send_LinkCheck(void)
{
    return 0;
}

int ZDT_IB_AT_Send_01(void)
{    
    return 0;
}

BOOLEAN BEACON_API_IsSleep(void)
{
    return TRUE;
}

char * BEACON_API_GetBTName(void)
{
    return "TEST";
}
#endif

static void  MMIBEACON_HanldeOK(void)
{
    BEACONAPP_TRACE("%s",__func__);
    return;    
}

static void  MMIBEACON_HanldeERR(void)
{
    BEACONAPP_TRACE("%s",__func__);
    return;    
}

static void  MMIBEACON_HanldeREBOOT(void)
{
    BEACONAPP_TRACE("%s",__func__);
    MMIIB_Reboot_Connect();
    return;
}

static void  MMIBEACON_HanldeCONN(uint32 status)
{
    BEACONAPP_TRACE("%s",__func__);
    if(status == 4)
    {
        g_ib_connect_check_num = 0;
#ifdef ZDT_SUPPORT_BEACON_SUNRI
        g_ib_need_send_passwd = 1;
#else
        g_ib_need_get_57_data = 1;
        g_ib_need_set_01_data = 1;
#endif
    }
    MMIIB_UpdateConnectWin(status);
    return;
}

static void  MMIBEACON_HanldeCONN_Check(void)
{
#ifdef ZDT_SUPPORT_BEACON_SUNRI
    if(g_ib_need_send_passwd)
    {
        BEACONAPP_TRACE("%s ZDT_IB_AT_Send_PWD",__func__);
        ZDT_IB_AT_Send_PWD();
        g_ib_need_send_passwd = 0;
    }
#else
    if(g_ib_need_set_01_data)
    {
        BEACONAPP_TRACE("%s ZDT_IB_AT_Send_41",__func__);
        ZDT_IB_AT_Send_41();
        g_ib_need_set_01_data = 0;
    }
    if(g_ib_need_get_57_data)
    {
        BEACONAPP_TRACE("%s ZDT_IB_AT_Send_57",__func__);
        ZDT_IB_AT_Send_57();
        g_ib_need_get_57_data = 0;
    }
#endif
    
    g_ib_connect_check_num++;
    return;
}

static void  MMIBEACON_HanldeDISCONN(void)
{
    BEACONAPP_TRACE("%s",__func__);
    MMIIB_StopDisconnectWaitTimer();
    MMIIB_StopConnectCheckTimer();
    MMIIB_UpdateDisConnectWin();
    return;
}

static void  MMIBEACON_HanldeDISCONN_Check(void)
{
    uint32 cur_tick = SCI_GetTickCount();
    MMIDEFAULT_TurnOnBackLight();
    if(cur_tick - g_ib_is_timer_disconnect_sec >= 300000)
    {
        //5分钟
        BEACONAPP_TRACE("%s 5 Min",__func__);
        MMIIB_StopConnect();
        MMIIB_StopDisconnectCheckTimer();
    }    
    else if(cur_tick - g_ib_is_timer_disconnect_sec >= 30000)
    {
        //30秒
        BEACONAPP_TRACE("%s 30S,is_timer_connect=%d",__func__,g_ib_is_timer_connect);
        if(g_ib_is_timer_connect)
        {
            MMIIB_StopConnect();
            MMIIB_StopDisconnectCheckTimer();
        }
        else
        {
#ifdef ZDT_SUPPORT_BEACON_SUNRI
            ZDT_IB_AT_Send_GetNum();
#else
            ZDT_IB_AT_Send_LinkCheck();
#endif
        }
    }
    else
    {
        BEACONAPP_TRACE("%s 5S offset=%d",__func__,cur_tick-g_ib_is_timer_disconnect_sec);
#ifdef ZDT_SUPPORT_BEACON_SUNRI
        ZDT_IB_AT_Send_GetNum();
#else
        ZDT_IB_AT_Send_LinkCheck();
#endif
    }
    
    return;
}

static void  MMIBEACON_PostInfo(uint8 * str,uint16 str_len)
{
    char data_hex_str[42] = {0};
    char * mac_str = MMIBEACON_NVGetMAC();
    char * name_str = BEACON_API_GetBTName();
    BEACON_U8_To_Hex(str,str_len,data_hex_str,40);
    BEACON_HTTP_PostInfo(name_str,mac_str,data_hex_str);
    return;
}

/*
0,20:
302C32303A0901000400000008000000300000002E000000000D0A
*/
void MMIIB_StopDisconnectCheckTimer()
{
    if(ib_disconnect_check_timer_id != 0)
    {
        MMK_StopTimer(ib_disconnect_check_timer_id);
        ib_disconnect_check_timer_id = 0;
    }
}

void MMIIB_DisconnectCheckTimerOut(uint8 timer_id,uint32 param)
{
    MMIBEACON_SendSigTo_APP(BEACON_APP_SIGNAL_DISCONN_CHECK,PNULL,0);
    return;
}

void MMIIB_StartDisconnectCheckTimer()
{
    
    MMIIB_StopDisconnectCheckTimer();
    g_ib_is_timer_disconnect_sec = SCI_GetTickCount();
    ib_disconnect_check_timer_id = MMK_CreateTimerCallback(5*1000, 
    											   MMIIB_DisconnectCheckTimerOut, 
    											    (uint32)PNULL,
    											     TRUE);
    return;
}

static void  MMIBEACON_HanldeDATA(uint8 *pData, uint32 DataLen )
{
    int connect_id = 0;
    int data_len = 0;
    int ret = 0;
    uint8 * str = pData;
    uint16 str_len = DataLen;
    uint32 temp = 0;
    char buf[101] = {0};
    BEACON_DATA_T cur_beacon_data = {0};

    if(str_len > 2 && str[str_len-1] == 0x0A)
    {
        str_len -= 2;
    }
    MMIIB_StartDisconnectCheckTimer();
    ret = BEACON_Data_GetNextPara(&str, &str_len,buf,100);
    if (ret > 0)
    {
        cur_beacon_data.connect_id = atoi(buf);
    }
    
    ret = BEACON_Data_GetNextParaM(&str, &str_len,buf,100);
    if (ret > 0)
    {
        cur_beacon_data.data_len = atoi(buf);
    }
    BEACONAPP_TRACE("%s type=0x%02x,len=%d,alert_jp=%d",__func__,str[0],str_len,yx_DB_Set_Rec.alert_jp);
#ifdef ZDT_SUPPORT_BEACON_SUNRI
    if(str_len >= 4 && str[0] == 0x5A && str[1] == 0x05 && str[2] == 0x01 && str[3] == 0x01)
    {
        //密码正确
        if(ZDT_IB_AT_Send_DateTime() < 0)
        {
            ZDT_IB_AT_Send_GetNum();
        }
    }
    else if(str_len >= 4 && str[0] == 0x5A && str[1] == 0x05 && str[2] == 0x06 && str[3] == 0x01)
    {
        //校准时间
        //ZDT_IB_AT_Send_DateTime();
        ZDT_IB_AT_Send_GetNum();
    }
    else if(str_len >= 18 && str[0] == 0x5A && str[1] == 0x13 && str[2] == 0x07)
    {
        cur_beacon_data.cmd = str[1];
        cur_beacon_data.type = str[2];
        cur_beacon_data.cur_index = str[3];
        temp = str[12];
        temp = temp << 8;
        temp += str[13];
        cur_beacon_data.cur_sum = temp;
        if(cur_beacon_data.cur_index != 0)
        {
            MMIBEACON_PostInfo(str,str_len);
            g_cur_beacon_ts_data = cur_beacon_data;
            MMIIB_UpdateConnectWin(10);
        }
    }
#else
    if(str[0] == 0x09 && str_len >= 20)
    {
        if(str[1] == 1)
        {
            cur_beacon_data.cmd = str[0];
            cur_beacon_data.type = str[1];
            cur_beacon_data.cur_index = str[2];
            temp = str[6];
            temp = temp << 8;
            temp += str[5];
            temp = temp << 8;
            temp += str[4];
            temp = temp << 8;
            temp += str[3];
            cur_beacon_data.cur_time = temp;
            temp = str[10];
            temp = temp << 8;
            temp += str[9];
            temp = temp << 8;
            temp += str[8];
            temp = temp << 8;
            temp += str[7];
            cur_beacon_data.cur_sum = temp;
            temp = str[14];
            temp = temp << 8;
            temp += str[13];
            temp = temp << 8;
            temp += str[12];
            temp = temp << 8;
            temp += str[11];
            cur_beacon_data.day_time = temp;
            temp = str[18];
            temp = temp << 8;
            temp += str[17];
            temp = temp << 8;
            temp += str[16];
            temp = temp << 8;
            temp += str[15];
            cur_beacon_data.day_sum = temp;
            cur_beacon_data.crc  = str[19];
            if(yx_DB_Set_Rec.alert_jp)
            {
                MMIBEACON_PostInfo(str,20);
            }
            g_cur_beacon_ts_data = cur_beacon_data;
            MMIIB_UpdateConnectWin(10);
        }
        else
        {
            if(yx_DB_Set_Rec.alert_jp)
            {
                MMIBEACON_PostInfo(str,str_len);
            }
        }
    }
    else if(str[0] == 0x57 && str_len >= 14)
    {
        MMIBEACON_PostInfo(str,str_len);
        if(YX_Net_Is_Land() && g_ib_is_timer_connect == FALSE)
        {
            MMIIB_TTS_PlayPostDataOK();
        }
    }
    else if(str[0] == 0x58 && str_len >= 18)
    {
        if(yx_DB_Set_Rec.alert_jp)
        {
            MMIBEACON_PostInfo(str,str_len);
        }
    }
    else if(str[0] == 0x41 && str_len >= 14)
    {
        if(str[1] < 0x23)
        {
            ZDT_IB_AT_Send_01();
        }
    }
    else if(str[0] == 0x01 && str_len >= 14)
    {
    }
    else
    {
        if(str_len >= 16)
        {
            if(yx_DB_Set_Rec.alert_jp)
            {
                MMIBEACON_PostInfo(str,str_len);
            }
        }
    }
#endif

    return;
}

static void  MMIBEACON_HanldeLINK(void)
{
    return;    
}

static void  MMIBEACON_HanldeLINK_ID(uint8 *pData, uint32 DataLen )
{
    uint8 * str = pData;
    uint16 str_len = DataLen;
    uint32 temp = 0;
    char buf[101] = {0};
    int ret = 0;
    int val = 0;
    BEACONAPP_TRACE("%s = %s",__func__,pData);
    ret = BEACON_Data_GetNextParaE(&str, &str_len,buf,10);
    if (ret > 0)
    {
        val = atoi(buf);
    }

    ret = BEACON_Data_GetNextParaE(&str, &str_len,buf,10);
    if (ret > 0)
    {
        val = atoi(buf);
        BEACONAPP_TRACE("LINK_ID val=%d,%s",val,buf);
    }
    return;
}

static void  MMIBEACON_HanldeScan(void)
{
    BEACONAPP_TRACE("%s",__func__);
    return;
}

static void  MMIBEACON_HanldeNo(void)
{
    BEACONAPP_TRACE("%s",__func__);
    MMIIB_Scan_Over(1);
    return;
}

static MMI_RESULT_E  HandleBEACONAppMsg (PWND app_ptr, 
								 uint16 msg_id, 
								 DPARAM param)
{
    switch(msg_id)
    {                   
        case BEACON_APP_SIGNAL_OK:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeOK();
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        case BEACON_APP_SIGNAL_ERR:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeERR();
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        case BEACON_APP_SIGNAL_REBOOT:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeREBOOT();
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        case BEACON_APP_SIGNAL_CONN:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeCONN(p_getdata->len);
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        case BEACON_APP_SIGNAL_CONN_CHECK:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeCONN_Check();
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        case BEACON_APP_SIGNAL_DISCONN:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeDISCONN();
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        case BEACON_APP_SIGNAL_DISCONN_CHECK:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeDISCONN_Check();
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        case BEACON_APP_SIGNAL_DATA:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeDATA(p_getdata->str,p_getdata->len);
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        case BEACON_APP_SIGNAL_LINK:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeLINK();
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        case BEACON_APP_SIGNAL_LINK_ID:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeLINK_ID(p_getdata->str,p_getdata->len);
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        case BEACON_APP_SIGNAL_SCAN:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeScan();
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        case BEACON_APP_SIGNAL_NO:
            {
                MMI_BEACON_SIG_T * pp_getdata = (MMI_BEACON_SIG_T *)param;
                MMI_BEACON_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    MMIBEACON_HanldeNo();
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }         
            break;
            
        default:
            break;
    }
    return TRUE;
}

void MMIBEACON_AppInit(void)
{
    g_beacon_app.ProcessMsg     = HandleBEACONAppMsg;
#ifdef FILE_LOG_SUPPORT
    TraceUser_Init();
    Trace_Log_Print("MMIBEACON_AppInit");
#endif
    MMIBEACON_NVGetMAC();
#ifdef ZDT_SUPPORT_BEACON
    ZDT_IB_Init();
    ZDT_IB_CheckHW_Start();
    ZDT_IB_PowerOn();
#endif

    s_beacon_app_init = TRUE;
}

BOOLEAN MMIBEACON_AppIsInit(void)
{
    return s_beacon_app_init;
}

void MMIBEACON_InitModule(void)
{
    MMIBEACON_RegMenuGroup();
    MMIBEACON_RegNv();
    MMIBEACON_RegWinIdNameArr();
}

BOOLEAN  MMIBEACON_SendSigTo_APP(BEACON_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len)
{
    uint8 * pstr = NULL;
    MMI_BEACON_SIG_T * psig = PNULL;
    if(s_beacon_app_init == FALSE)
    {
        return FALSE;
    }
    if(data_ptr != PNULL && data_len != 0)
    {
        pstr = SCI_ALLOC_APPZ(data_len+1);//free it in AT task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("MMIBEACON_SendSigTo_APP Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(MMI_BEACON_SIG_T),SCI_IdentifyThread());
    psig->data.len = data_len;
    psig->data.str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
    
}

BOOLEAN  ZDTBEACON_SendTo_APP(BEACON_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len)
{
    uint8 * pstr = NULL;
    MMI_BEACON_SIG_T * psig = PNULL;
        
    if(data_ptr != PNULL && data_len != 0)
    {
        pstr = SCI_ALLOC_APPZ(data_len+1);//free it in AT task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("ZDTGPS_SendTo_APP Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(MMI_BEACON_SIG_T),ZDTIB_TaskGetID());
    psig->data.len = data_len;
    psig->data.str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
    
}

int BEACON_SMS_Receive_Mac(uint8 * pContent,uint16 ContentLen)
{
    if(pContent == PNULL || ContentLen == 0)
    {
        return 0;
    }
    MMIBEACON_NVSetMAC(pContent,ContentLen);
    if(ZDT_IB_Hw_IsOn())
    {
        ZDT_IB_AT_Send_DisConnect();
        MMIIB_StartDisconnectWaitTimer();
    }
    return 1;
}

#define BEACON_GET_IP_DOMAIN  "https://patriarch-tm.tanmasports.com/teacher/v1/studentcard/queryRopeSkippingByStudentCard?"
#define BEACON_DEFAULT_APP_SEC  "ILXJLGJSELJLX5S46GS4EFG4136X48GR4EE646XE"
#define BEACON_DEFAULT_APP_KEY "studentcard20221114"

uint8 BEACON_HTTP_GetInfo_Handle(cJSON *pObject)
{
    uint8 ret = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    cJSON *cur_json = PNULL;
    int code = 0;
    BEACONAPP_TRACE("BEACON BEACON_HTTP_GetInfo_Handle");

    json=pObject;
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"code");
        if(item_json)
        {
            val = cJSON_GetObjectInt(item_json);
            code = val;
            BEACONAPP_TRACE("BEACON code=%d",code);
        }
        item_json = cJSON_GetObjectItem(json,"msg");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                BEACONAPP_TRACE("BEACON msg len=%d,%s",msg_len,msg_char);
                if(msg_len > 0)
                {
                }
            }
        }
        cur_json = cJSON_GetObjectItem(json,"response");
        if(cur_json)
        {
            item_json = cJSON_GetObjectItem(cur_json,"bindingStatus");
            if(item_json)
            {
                msg_char = cJSON_GetObjectString(item_json);
                if(msg_char != PNULL)
                {
                    msg_len = strlen(msg_char);
                    BEACONAPP_TRACE("BEACON bindingStatus len=%d,%s",msg_len,msg_char);
                    if(msg_len > 0)
                    {
                    }
                }
            }
            item_json = cJSON_GetObjectItem(cur_json,"studentCardCode");
            if(item_json)
            {
                msg_char = cJSON_GetObjectString(item_json);
                if(msg_char != PNULL)
                {
                    msg_len = strlen(msg_char);
                    BEACONAPP_TRACE("BEACON studentCardCode len=%d,%s",msg_len,msg_char);
                    if(msg_len > 0)
                    {
                        MMIBEACON_NVSetMAC(msg_char,msg_len);
                    }
                }
            }
            item_json = cJSON_GetObjectItem(cur_json,"ropeSkippingMac");
            if(item_json)
            {
                msg_char = cJSON_GetObjectString(item_json);
                if(msg_char != PNULL)
                {
                    msg_len = strlen(msg_char);
                    BEACONAPP_TRACE("BEACON ropeSkippingMac len=%d,%s",msg_len,msg_char);
                    if(msg_len > 0)
                    {
                    }
                }
            }
            item_json = cJSON_GetObjectItem(cur_json,"ropeSkippingName");
            if(item_json)
            {
                msg_char = cJSON_GetObjectString(item_json);
                if(msg_char != PNULL)
                {
                    msg_len = strlen(msg_char);
                    BEACONAPP_TRACE("BEACON ropeSkippingName len=%d,%s",msg_len,msg_char);
                    if(msg_len > 0)
                    {
                    }
                }
            }
        }
    }
    return ret;
}

int BEACON_HTTP_GetInfo_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    BEACONAPP_TRACE("BEACON_HTTP_GetInfo_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(is_ok)
    {
        if(pRcv != NULL && Rcv_len > 2)
        {
            cJSON *json = PNULL;
            json=cJSON_Parse(pRcv);
            if (json) 
            {
                if(BEACON_HTTP_GetInfo_Handle(json))
                {
                }
                cJSON_Delete(json);
            }
        }
    }
    return 0;
}

BOOLEAN  BEACON_HTTP_GetInfo(void)
{
    char app_domain[201] = {0};
    char content_buf[1024] = {0};
    uint16 content_len = 0;
    
    if(FALSE == BEACON_SIM_Exsit())
    {
        return FALSE;
    }
    BEACONAPP_TRACE("BEACON_HTTP_GetInfo");
    sprintf(content_buf,"studentCard=%s&appKey=%s&sign=%s",g_zdt_phone_imei,BEACON_DEFAULT_APP_KEY,BEACON_DEFAULT_APP_SEC);
    content_len = SCI_STRLEN(content_buf);
    sprintf(app_domain,"%s",BEACON_GET_IP_DOMAIN);
    MMIZDT_HTTP_AppSend(TRUE,app_domain,(uint8 *)content_buf,content_len,1000,0,0,0,0,0,BEACON_HTTP_GetInfo_CB);

    return TRUE;
}

#define BEACON_POST_IP_DOMAIN  "https://patriarch-tm.tanmasports.com/teacher/v1/studentcard/submitRopeSkippingData"

uint8 BEACON_HTTP_PostInfo_Handle(cJSON *pObject)
{
    uint8 ret = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    int code = 0;
    BEACONAPP_TRACE("BEACON BEACON_HTTP_PostInfo_Handle");

    json=pObject;
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"code");
        if(item_json)
        {
            val = cJSON_GetObjectInt(item_json);
            code = val;
            BEACONAPP_TRACE("BEACON code=%d",code);
        }
        item_json = cJSON_GetObjectItem(json,"msg");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                BEACONAPP_TRACE("BEACON msg len=%d,%s",msg_len,msg_char);
                if(msg_len > 0)
                {
                }
            }
        }
            
    }
    return ret;
}

int BEACON_HTTP_PostInfo_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    BEACONAPP_TRACE("BEACON_HTTP_PostInfo_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(is_ok)
    {
        if(pRcv != NULL && Rcv_len > 2)
        {
            cJSON *json = PNULL;
            json=cJSON_Parse(pRcv);
            if (json) 
            {
                if(BEACON_HTTP_GetInfo_Handle(json))
                {
                }
                cJSON_Delete(json);
            }
        }
    }
    return 0;
}

#if 1
BOOLEAN  BEACON_HTTP_PostInfo(char * name_str,char * mac_str, char * data_str)
{
    char content_buf[2048] = {0};
    uint32 content_len = 0;

    if(FALSE == BEACON_SIM_Exsit())
    {
        return FALSE;
    }
    BEACONAPP_TRACE("BEACON_HTTP_PostInfo");
    sprintf(content_buf,"%s@%s",mac_str,data_str);
    content_len = strlen(content_buf);
#ifdef ZDT_PLAT_YX_SUPPORT_YS
    //YX_Net_Send_REPORT_ROPESKIPPING_DATA(content_buf,content_len);
#endif
    return TRUE;
}
#else
BOOLEAN  BEACON_HTTP_PostInfo(char * name_str,char * mac_str, char * data_str)
{
    cJSON *root = PNULL;
    cJSON *reqContent = PNULL;
    cJSON *requestItem = PNULL;
    char * out = PNULL;
    char * pContent = PNULL;
    char app_domain[201] = {0};
    uint16 len = 0;
    char content_buf[2048] = {0};
    uint16 content_len = 0;

    if(FALSE == BEACON_SIM_Exsit())
    {
        return FALSE;
    }
    BEACONAPP_TRACE("BEACON_HTTP_PostInfo");
    requestItem=cJSON_CreateObject();
    if(requestItem)
    {
        reqContent=cJSON_CreateObject();
        if(reqContent)
        {
            cJSON_AddItemToObject(reqContent, "studentCard", cJSON_CreateString(g_zdt_phone_imei));
            cJSON_AddItemToObject(reqContent, "appKey", cJSON_CreateString(BEACON_DEFAULT_APP_KEY));
            cJSON_AddItemToObject(reqContent, "sign", cJSON_CreateString(BEACON_DEFAULT_APP_SEC));
            cJSON_AddItemToObject(reqContent, "ropeSkippingData", cJSON_CreateString(data_str));
            cJSON_AddItemToObject(reqContent, "ropeSkippingMac", cJSON_CreateString(mac_str));
            cJSON_AddItemToObject(reqContent, "ropeSkippingName", cJSON_CreateString(name_str));
            cJSON_AddItemToObject(requestItem, "request", reqContent);
            pContent=cJSON_PrintUnformatted(requestItem);
            if(pContent)
            {
                content_len = strlen(pContent);
                sprintf(app_domain,"%s",BEACON_POST_IP_DOMAIN);
                MMIZDT_HTTP_AppSend(FALSE,app_domain,(uint8 *)pContent,content_len,1000,0,0,0,0,0,BEACON_HTTP_PostInfo_CB);
                SCI_FREE(pContent);
            }
            cJSON_Delete(reqContent);
        }
    }
    
    return TRUE;
}
#endif

PUBLIC void BEACON_TraceExt(const char *string, int len)
{
#if 1
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
#ifdef FILE_LOG_SUPPORT
        Trace_Log_Print("[BC]:%s",tmp_str);
#else
        SCI_TRACE_LOW("[BC]:%s",tmp_str);
#endif
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
#ifdef FILE_LOG_SUPPORT
           Trace_Log_Print("[BC]:%s",buff);
#else
           SCI_TRACE_LOW("[BC]:%s",buff);
#endif
        
          p += l;
          len -= l;
        }
    }
#endif
}

void BEACON_Trace(const char *pszFmt, ...)
{
    char buf[1024] = {0};
    int tmp_len = 0;
    va_list va_args;    
    va_start(va_args, pszFmt);
#ifdef WIN32
    tmp_len = vsprintf(buf,pszFmt, va_args);
#else
    tmp_len = vsnprintf(buf,1023,pszFmt, va_args);
#endif
    if (tmp_len >= 0)
    {
        BEACON_TraceExt(buf,tmp_len);
    }
    va_end(va_args);
}

PUBLIC void BEACON_TraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        IB_U8_To_Hex(str,len,dest_buff,128);
#ifdef FILE_LOG_SUPPORT
        Trace_Log_Print("[BC][U8]:%s",dest_buff);
#else
        SCI_TRACE_LOW("[BC][U8]:%s",dest_buff);
#endif
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           IB_U8_To_Hex(p,l,dest_buff,128);        
#ifdef FILE_LOG_SUPPORT
           Trace_Log_Print("[BC][U8]:%s",dest_buff);
#else
           SCI_TRACE_LOW("[BC][U8]:%s",dest_buff);
#endif
            p += l;
            len -= l;
        }
    }
#endif
}

#endif
