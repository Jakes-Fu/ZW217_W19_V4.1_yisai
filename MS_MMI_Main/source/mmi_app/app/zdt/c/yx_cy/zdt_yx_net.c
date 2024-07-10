
#include "zdt_app.h"
#ifdef ZDT_GSENSOR_SUPPORT
#include "zdt_gsensor.h"
#endif
#include "mmialarm_export.h"
#include "mmicl_export.h"
#include "mmicl_internal.h"
#include "cJSON.h"
#ifdef WIN32
#include "zdt_base64.h"
#endif
#define z_abs(x)  ((x) >= 0 ? (x) : (-(x)))
#ifdef ZDT_PLAT_YX_SUPPORT
#define YX_TCP_LK_TYPE 1

#define YX_TCP_LK_TIME_OUT 300

#ifndef ONE_DAY_SECONDS
#define ONE_DAY_SECONDS		86400
#endif

#ifndef ZDT_GPS_SUPPORT
typedef struct GPS_DATA_t
{
    uint8 is_valid;
    uint32 Latitude;
    uint8 Lat_Dir;
    uint32 Longitude;
    uint8 Long_Dir;
    uint32 date;
    uint32 time;
    uint32 speed;  //运行速度
    uint16 hdop;	// 水平精度
    uint16 cog;     //方向角度
    uint8 sate_num;	// 卫星颗数
    uint32 altitude;    //海拔高度
    uint8 alt_Dir;          //海拔方向
    uint32 Lat_m;          //精确到小数点后4位
} GPS_DATA_T;
#endif

int YX_Net_Reg(void);

extern uint8 s_yx_gps_loc_short_mode;

static BOOLEAN s_yx_is_first_land = FALSE;
static uint16 g_yx_loc_timer_interval = 0;
static uint16 g_yx_loc_timer_lk_num = 0;
static uint16 g_yx_lk_timers_num = 0;
static char g_yx_net_call_number[41] = {0};
static char g_yx_net_monitorl_number[41] = {0};
static BOOLEAN g_is_yx_is_locing = FALSE;
static EVENT_CURRENT_DATA_T  s_yx_current_event_info = {0, 0, {0}};
static BOOLEAN g_is_yx_first_reg = FALSE;
static uint8 g_is_yx_first_reg_times = 0;

BOOLEAN g_is_yx_power_off_ing = FALSE;
BOOLEAN g_is_yx_czsb_is_playing = FALSE;

BOOLEAN g_yx_cur_heart_change = FALSE;
uint16 g_yx_cur_heart_time = YX_TCP_LK_TIME_OUT;

uint16 g_yx_power_on_time_min = 0;

YX_WEATHER_INFO_T g_yx_wt_info = {0};
BOOLEAN g_yx_wt_recieve_ok = FALSE;
uint8 g_yx_wt_oneday_start = 0;
extern uint8 g_yx_al_repeat_num;
static uint8 g_yx_net_rcv_busino[20] = {0};
BOOLEAN         g_yx_is_land = FALSE;
static BOOLEAN         g_yx_is_first_land = FALSE;
static BOOLEAN         g_yx_is_handle_factory = FALSE;
static BOOLEAN         g_yx_is_need_reset_factory = FALSE;
static BOOLEAN         g_yx_is_need_reset = FALSE;
static uint32 s_yx_wl_check_times = 0;
static BOOLEAN s_yx_is_in_wl_time = FALSE;
uint32 g_yx_wl_interval_min = 1;

YX_HEALTH_MSG_Type yx_health_msg_Rec = {0};

YX_JOB_LINK_DATA_T yx_job_cur_data = {0};
    
#if 1
#define YX_RCV_DATA_MAX_LEN 4096


static uint8 yx_cmd_line_buffer[YX_RCV_DATA_MAX_LEN];     /*  buffer for AT cmd line  */
static uint32 yx_cmd_line_len = 0;
static uint32 yx_data_need_len = 0;
static uint8 *yx_cmd_ptr;                     /* next loc in buffer       */
static uint8 yx_cmd_status = 0;
uint8 * yx_tcp_rx_data_buf = NULL;
uint8  g_yx_imei_len = ZDT_IMEI_LEN;

uint8  YX_RCV_CMD_ProcessOneChar(uint8 cc)
{
    uint8 res = 0;
    switch ( yx_cmd_status )
    {
        case 0:
            /*-----------------------------------------------------------------
              In the HUNT state, the characters 'A' or 'a' are looked for.  
              Everything else is ignored. If 'A' or 'a' is detected, the   
              machine goes into the FOUND_A state.                              
            -----------------------------------------------------------------*/
            if (cc != YX_NET_CMD_END && cc != YX_NET_CMD_END_2)
            {
                yx_cmd_ptr = &yx_cmd_line_buffer[0];
                *yx_cmd_ptr++ = cc;
                yx_cmd_line_len = 1;
                yx_cmd_status = 1;
            }
            break;
            
        case 1:
            {
                if ( yx_cmd_ptr >= (yx_cmd_line_buffer + YX_RCV_DATA_MAX_LEN - 2) )
                {
                    yx_cmd_status = 0;
                    yx_cmd_ptr[0] = '\0';
                    break;
                }
                /*------------------------------------------------------------
                  If no overflow, fill buffer.  
                ------------------------------------------------------------*/
                else
                {
                    *yx_cmd_ptr++ = cc;
                    yx_cmd_line_len++;
                }

                if(cc == YX_NET_CMD_END_2 && yx_cmd_line_buffer[yx_cmd_line_len-2] == YX_NET_CMD_END)
                {
                    res = 1;
                    yx_cmd_status = 0;
                }
            }
            break;
    }  /* end switch ( at_cmd_prep_state ) */

    return res;
}

static void YX_RCV_CMD_Reset(void)
{
    yx_cmd_status = 0;
    return;
}

static uint32 YX_RCV_CMD_Check(uint8 * pData, uint32 Data_len)
{
    return 0;
}

static uint32 YX_CMD_CheckAll(uint8 * pData, int Data_len,uint8 **ppCmdBuf, uint32 * cmd_len)
{
    uint32 end_len = 0;
    uint32 i = 0;
    uint8 res = 0;
    uint8 * pTmp = PNULL;
    if(pData == PNULL || Data_len <= 0)
    {
        return 0;
    }
    end_len = YX_RCV_CMD_Check(pData,Data_len);
    if(end_len > 0)
    {
        YX_RCV_CMD_Reset();
        pTmp = (uint8 *)SCI_ALLOC_APPZ(end_len);
        if(pTmp == NULL)
        {
            ZDT_LOG("YX_CMD_CheckAll Err Mem 1");
            return 0;
        }
        SCI_MEMCPY(pTmp,pData,end_len);
        *ppCmdBuf = pTmp;
        *cmd_len = end_len;
        return end_len;
    }
    else
    {
        for(i = 0; i < Data_len; i++)
        {
            res = YX_RCV_CMD_ProcessOneChar(pData[i]);
            if(res > 0)
            {
                uint8 * decrypt_char;
                uint32 decrypt_len = 0;
                uint8	*temp_ptr = PNULL;
                yx_cmd_line_buffer[yx_cmd_line_len] = 0;
                temp_ptr = (uint8*)strchr((char*)yx_cmd_line_buffer, '#');
                if(temp_ptr == PNULL)
                {
                    decrypt_char = YX_Func_Decrypt(yx_cmd_line_buffer,yx_cmd_line_len,&decrypt_len);
                }
                else
                {
                    decrypt_char = YX_Func_Decrypt(yx_cmd_line_buffer,temp_ptr-yx_cmd_line_buffer,&decrypt_len);
                }
                if(decrypt_char == PNULL)
                {
                    ZDT_LOG("YX_CMD_CheckAll Err YX_Func_Decrypt");
                    return 0;
               }
                ZDT_TCPRcvTrace(decrypt_char,decrypt_len);
                *ppCmdBuf = decrypt_char;
                *cmd_len = decrypt_len;
                end_len = i+1;
                break;
            }
        }
    }
    return end_len;
}
#endif

static int YX_Net_GetContentLen_Send(YX_APP_T *pMe,uint8 * pData,uint32 Data_len,uint8 ** pOut, uint32 * pContertLen)
{
    uint32 MsgLen = 0;
    char len_str[101] = {0};
    uint8 * pStr = pData;
    uint32 len = Data_len;
    int ret = 0;
    uint8 * decrypt_char;
    uint32 decrypt_len = 0;
    uint8	*temp_ptr = PNULL;
    
    if(pData == NULL || Data_len < 2)
    {
        ZDT_LOG("YX_Net_GetContentLen_Send Err NULL");
        return -1;
    }
    
    temp_ptr = (uint8*)strchr((char*)pStr, '#');
    if(temp_ptr == PNULL)
    {
        decrypt_char = YX_Func_Decrypt(pStr,len,&decrypt_len);
    }
    else
    {
        decrypt_char = YX_Func_Decrypt(pStr,temp_ptr-pStr,&decrypt_len);
    }
    
    if(decrypt_char == PNULL)
    {
        ZDT_LOG("YX_Net_GetContentLen_Send Err YX_Func_Decrypt");
        return -1;
    }

    if(decrypt_char[0] != YX_NET_MSG_START
        || decrypt_char[decrypt_len-1] != YX_NET_MSG_END
        )
    {
        ZDT_LOG("YX_Net_GetContentLen_Send ERR End");
        SCI_FREE(decrypt_char);
        return -1;
    }
    
    pStr = decrypt_char;
    len = decrypt_len;
    ret = YX_Func_GetCmdPosPara(&pStr, &len, len_str,100,6);
    if(ret > 0)
    {
        MsgLen = atoi(len_str);
        ZDT_LOG("YX_Net_GetContentLen_Send MsgLen = %d",MsgLen);
    }
    else
    {
        ZDT_LOG("YX_Net_GetContentLen_Send ERR MsgLen");
        SCI_FREE(decrypt_char);
        return -1;
    }
    if(MsgLen >= decrypt_len)
    {
        ZDT_LOG("YX_Net_GetContentLen_Send ERR MsgLen=%d,decrypt_len=%d",MsgLen,decrypt_len);
        SCI_FREE(decrypt_char);
        return -1;
    }
    *pOut = decrypt_char;
    *pContertLen = MsgLen;
    return (int)decrypt_len;
}

int YX_Net_GetContentLen_RCV(YX_APP_T *pMe,uint8 * pData,uint32 Data_len)
{
    uint32 MsgLen = 0;
    char len_str[101] = {0};
    uint8 * pStr = pData;
    uint32 len = Data_len;
    int ret = 0;

    if(pData == NULL || Data_len < 2)
    {
        ZDT_LOG("YX_Net_GetContentLen_RCV ERR NULL");
        return -1;
    }
    
    if(pData[0] != YX_NET_MSG_START
        || pData[Data_len-1] != YX_NET_MSG_END
        )
    {
        ZDT_LOG("YX_Net_GetContentLen_RCV ERR End");
        return -1;
    }
    
    ret = YX_Func_GetCmdPosPara(&pStr, &len, len_str,100,6);
    if(ret > 0)
    {
        MsgLen = atoi(len_str);
        ZDT_LOG("YX_Net_GetContentLen_RCV MsgLen = %d",MsgLen);
    }
    else
    {
        ZDT_LOG("YX_Net_GetContentLen_RCV ERR MsgLen");
        return -1;
    }

    if(MsgLen >= Data_len)
    {
        ZDT_LOG("YX_Net_GetContentLen_RCV ERR MsgLen=%d,Data_len=%d",MsgLen,Data_len);
        return -1;
    }
    return (int)MsgLen;
}

BOOLEAN YX_Net_Is_Land(void)
{
    #ifdef WIN32
        return TRUE;
    #endif
    return  g_yx_is_land;
}

void YX_Net_SetIdleYXIcon(BOOLEAN is_disp)
{
    static BOOLEAN s_is_yx_icon_disp = FALSE;
    if(s_is_yx_icon_disp == is_disp)
    {
        return;
    }
    s_is_yx_icon_disp = is_disp;
    //MMIAPICOM_StatusAreaSetYxLandState(is_disp);
}

void YX_Net_Set_Land(BOOLEAN is_land)
{
    if(g_yx_is_land != is_land)
    {
        if(g_is_yx_power_off_ing == FALSE)
        {
            YX_Net_SetIdleYXIcon(is_land);
            if(is_land)
            {
                if(g_yx_is_first_land == FALSE)
                {
                    if(MMIAPIIDLE_IdleWinIsOpen())
                    {
#ifdef ZDT_PLAY_RING_CUSTOM
                        MMIAPISET_PlayCustomRing(MMISET_CUST_NET_OK,1);
#endif
                    }
                }
            }
            else
            {
                if(g_yx_is_first_land == FALSE)
                {
#ifdef ZDT_PLAY_RING_CUSTOM
                MMIAPISET_PlayCustomRing(MMISET_CUST_NO_NET,1);
#endif
                }
            }
        }

        if(g_yx_is_first_land == FALSE && is_land)
        {
            g_yx_is_first_land = TRUE;
        }
        g_yx_is_land = is_land;
    }
    return;
}

extern uint32 YX_Net_TCPSend(char type,char * mark_str,uint8 * pContent,uint32 ContentLen,uint8 priority,uint8 repeat_num,uint32 timeout,TCPRCVHANDLER rcv_handle)
{
    ZDT_TCP_LINK_DATA_T * cur_data = NULL;
    uint32 len = 0;
    uint32 msg_len = 0;
    uint32 content_len = 0;
    uint16 company_len = 0;
    //uint16 ps_ver_len = 0;
    uint16 fw_ver_len = 0;
    uint8 * content_str = PNULL;
    char len_str[30] = {0};
    uint8 len_str_len = 0;
    uint8 imei_len = 0;
    uint8 iccid_len = 0;
    uint8 mark_str_len = 0;
    uint8  * crypt_data = PNULL;
    uint32 crypt_len = 0;

    if(mark_str == NULL || ContentLen == 0 || pContent == NULL )
    {
        ZDT_LOG("YX_Net_TCPSend: ERR NULL");
        return 0;
    }
    
    sprintf(len_str,"%d",ContentLen);
    len_str_len = strlen(len_str);
    imei_len = g_yx_imei_len;
    iccid_len = ZDT_ICCID_LEN;
    mark_str_len = SCI_STRLEN(mark_str);

    content_len = len_str_len+imei_len+iccid_len+mark_str_len+YX_NET_MSG_HEAD_LEN+ContentLen+2;
    content_str = SCI_ALLOC_APPZ(content_len);
    if(content_str == NULL)
    {
        ZDT_LOG("YX_Net_TCPSend: ERR content_str");
        return 0;
    }
    
    cur_data=(ZDT_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(ZDT_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        ZDT_LOG("YX_Net_TCPSend: ERR Malloc TCP");
        SCI_FREE(content_str);
        return 0;
    }

    // 起始符[
    content_str[0] = YX_NET_MSG_START;
    
    // 设备号
    SCI_MEMCPY(content_str+1,g_zdt_phone_imei,imei_len);
    len = imei_len + 1;
    content_str[len++] = ',';

    //ICCID
    SCI_MEMCPY(content_str+len,g_zdt_sim_iccid,iccid_len);
    len += iccid_len;
    content_str[len++] = ',';
    
    // 交易流水号
    len += YX_Func_GetIndexWater(content_str+len);
    content_str[len++] = ',';

    // 标识
    SCI_MEMCPY(content_str+len,mark_str,mark_str_len);
    len += mark_str_len;
    content_str[len++] = ',';

    // 报文类型
    content_str[len++] = type;
    content_str[len++] = ',';

    // 时间
    len += YX_Func_GetDateTime(content_str+len);
    content_str[len++] = ',';

    // 报文长度
    SCI_MEMCPY(content_str+len,len_str,len_str_len);
    len += len_str_len;
    content_str[len++] = ',';
    
    // 数据
    SCI_MEMCPY(content_str+len,pContent,ContentLen);
    len += ContentLen;

    // 结束]
    content_str[len++] = YX_NET_MSG_END;
    
    ZDT_LOG("[TCP] Send:%s",content_str);
    
    crypt_data = YX_Func_Encrypt(content_str,len,&crypt_len);
    
    SCI_FREE(content_str);
    
    if(crypt_data == PNULL)
    {
        ZDT_LOG("YX_Net_TCPSend: ERR crypt_data");
        SCI_FREE(cur_data);
        return 0;
    }
    
    company_len = SCI_STRLEN(YX_COMPANY_STR);
    //ps_ver_len = SCI_STRLEN(YX_PS_VERSION_STR);
    fw_ver_len = SCI_STRLEN(YX_FW_VERSION_STR);

    msg_len = crypt_len +company_len+fw_ver_len+6;
    cur_data->str= (uint8 *)SCI_ALLOC_APPZ(msg_len);
    if(cur_data->str == NULL)
    {
        ZDT_LOG("YX_Net_TCPSend: ERR Malloc len=%d",msg_len);
        SCI_FREE(cur_data);
        SCI_FREE(crypt_data);
        return 0;
    }
        
    SCI_MEMCPY(cur_data->str,crypt_data,crypt_len);
    len = crypt_len;
    cur_data->str[len++] = '#';

    SCI_MEMCPY(cur_data->str+len,YX_COMPANY_STR,company_len);
    len += company_len;
    cur_data->str[len++] = '#';
    
    //SCI_MEMCPY(cur_data->str+len,YX_PS_VERSION_STR,ps_ver_len);
    //len += ps_ver_len;
    //cur_data->str[len++] = '#';
    
    SCI_MEMCPY(cur_data->str+len,YX_FW_VERSION_STR,fw_ver_len);
    len += fw_ver_len;
    cur_data->str[len++] = '\r';
    cur_data->str[len++] = '\n';

    SCI_FREE(crypt_data);
    cur_data->pMe = g_yx_app.m_yx_tcp_reg_interface;
    cur_data->len = len;
    cur_data->times = repeat_num;
    cur_data->timeout = timeout;
    cur_data->priority = priority;
    cur_data->rcv_handle = rcv_handle;
    ZDT_TCP_PostEvtTo_APP(ZDT_APP_SIGNAL_TCP_WRITE_ADD,cur_data,len);
    
    ZDT_LOG("YX_Net_TCPSend Handle=0x%x",(uint32)cur_data);
    return (uint32)cur_data;
}

extern uint32 YX_Net_TCPRespond(char type,char * water_str,char * mark_str,char * pContent,uint32 ContentLen)
{
    ZDT_TCP_LINK_DATA_T * cur_data = NULL;
    uint32 len = 0;
    uint32 msg_len = 0;
    uint32 content_len = 0;
    uint16 company_len = 0;
    //uint16 ps_ver_len = 0;
    uint16 fw_ver_len = 0;
    uint8 * content_str = PNULL;
    char len_str[30] = {0};
    uint8 len_str_len = 0;
    uint8 imei_len = 0;
    uint16 water_len = 18;
    uint8 iccid_len = 0;
    uint8 mark_str_len = 0;
    uint8  * crypt_data = PNULL;
    uint32 crypt_len = 0;

    if(mark_str == NULL || ContentLen == 0 || pContent == NULL )
    {
        ZDT_LOG("YX_Net_TCPRespond: ERR NULL");
        return 0;
    }
    
    sprintf(len_str,"%d",ContentLen);
    len_str_len = strlen(len_str);
    imei_len = g_yx_imei_len;
    iccid_len = ZDT_ICCID_LEN;
    mark_str_len = SCI_STRLEN(mark_str);

    content_len = len_str_len+imei_len+iccid_len+mark_str_len+YX_NET_MSG_HEAD_LEN+ContentLen+2;
    content_str = SCI_ALLOC_APPZ(content_len);
    if(content_str == NULL)
    {
        ZDT_LOG("YX_Net_TCPRespond: ERR content_str");
        return 0;
    }
    
    cur_data=(ZDT_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(ZDT_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        ZDT_LOG("YX_Net_TCPRespond: ERR Malloc TCP");
        SCI_FREE(content_str);
        return 0;
    }

    // 起始符[
    content_str[0] = YX_NET_MSG_START;
    
    // 设备号
    SCI_MEMCPY(content_str+1,g_zdt_phone_imei,imei_len);
    len = imei_len + 1;
    content_str[len++] = ',';

    //ICCID
    SCI_MEMCPY(content_str+len,g_zdt_sim_iccid,iccid_len);
    len += iccid_len;
    content_str[len++] = ',';
    
    // 交易流水号
    if(water_str == PNULL)
    {
        len += YX_Func_GetIndexWater(content_str+len);
    }
    else
    {
        SCI_MEMCPY(content_str+len,water_str,water_len);
        len += water_len;
    }
    content_str[len++] = ',';

    // 标识
    SCI_MEMCPY(content_str+len,mark_str,mark_str_len);
    len += mark_str_len;
    content_str[len++] = ',';

    // 报文类型
    content_str[len++] = type;
    content_str[len++] = ',';

    // 时间
    len += YX_Func_GetDateTime(content_str+len);
    content_str[len++] = ',';

    // 报文长度
    SCI_MEMCPY(content_str+len,len_str,len_str_len);
    len += len_str_len;
    content_str[len++] = ',';
    
    // 数据
    SCI_MEMCPY(content_str+len,pContent,ContentLen);
    len += ContentLen;

    // 结束]
    content_str[len++] = YX_NET_MSG_END;
    
    ZDT_LOG("[TCP] Send:%s",content_str);
    
    crypt_data = YX_Func_Encrypt(content_str,len,&crypt_len);
    
    SCI_FREE(content_str);
    
    if(crypt_data == PNULL)
    {
        ZDT_LOG("YX_Net_TCPRespond: ERR crypt_data");
        SCI_FREE(cur_data);
        return 0;
    }
    
    company_len = SCI_STRLEN(YX_COMPANY_STR);
    //ps_ver_len = SCI_STRLEN(YX_PS_VERSION_STR);
    fw_ver_len = SCI_STRLEN(YX_FW_VERSION_STR);

    msg_len = crypt_len +company_len+fw_ver_len+6;
    cur_data->str= (uint8 *)SCI_ALLOC_APPZ(msg_len);
    if(cur_data->str == NULL)
    {
        ZDT_LOG("YX_Net_TCPRespond: ERR Malloc len=%d",msg_len);
        SCI_FREE(cur_data);
        SCI_FREE(crypt_data);
        return 0;
    }
        
    SCI_MEMCPY(cur_data->str,crypt_data,crypt_len);
    len = crypt_len;
    cur_data->str[len++] = '#';

    SCI_MEMCPY(cur_data->str+len,YX_COMPANY_STR,company_len);
    len += company_len;
    cur_data->str[len++] = '#';
    
    //SCI_MEMCPY(cur_data->str+len,YX_PS_VERSION_STR,ps_ver_len);
    //len += ps_ver_len;
    //cur_data->str[len++] = '#';
    
    SCI_MEMCPY(cur_data->str+len,YX_FW_VERSION_STR,fw_ver_len);
    len += fw_ver_len;
    cur_data->str[len++] = '\r';
    cur_data->str[len++] = '\n';

    SCI_FREE(crypt_data);
    cur_data->pMe = g_yx_app.m_yx_tcp_reg_interface;
    cur_data->len = len;
    cur_data->times = 0;
    cur_data->timeout = 0;
    cur_data->priority = 100;
    cur_data->rcv_handle = PNULL;
    ZDT_TCP_PostEvtTo_APP(ZDT_APP_SIGNAL_TCP_WRITE_ADD,cur_data,len);
    
    ZDT_LOG("YX_Net_TCPRespond Handle=0x%x",(uint32)cur_data);
    return (uint32)cur_data;
}

#ifdef ZDT_CYHEALTH_SUPPORT
/*
{"nucleicAcidTime":"","userName":"鏄撹禌1","healthyCodeSource":"绂忓缓鐪佸仴搴风爜","vaccinesStatus":0,"nucleicAcid":"UNKNOWN","healthStatus":"TX","time":"2023-01-18 14:36"}
*/
int YX_Net_HTTP_Health_Handle(uint8 * pContent,uint32 ContentLen)
{
    int ret = -1;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    uint16 content_wstr[100] = {0};
    MMI_STRING_T alert_string = {0};
    uint32 ms = 1000;
    if(pContent == PNULL || ContentLen < 2)
    {
        return 0;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        SCI_MEMSET(&yx_health_msg_Rec,0,sizeof(YX_HEALTH_MSG_Type));
        yx_health_msg_Rec.rcv_time = ZDT_Get_TimeSecond();
        yx_health_msg_Rec.code_result = 1;

        //通行标识（TX：通行；JX：禁止通行）
        item_json = cJSON_GetObjectItem(json,"healthStatus");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("healthStatus=%s",msg_char);
                if(0 == strncmp(msg_char,"TX",2))
                {
                    yx_health_msg_Rec.code_status = 1;
                }
                else
                {
                    yx_health_msg_Rec.code_status = 2;
                }
            }
        }
        else
        {
            ZDT_LOG("healthStatus NONE");
        }

        //二维码图片字符串
        item_json = cJSON_GetObjectItem(json,"qrcode");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("qrcode=%s",msg_char);
                if(msg_len > HTTP_HEALTH_QRCODE_MAX_LEN)
                {
                    msg_len = HTTP_HEALTH_QRCODE_MAX_LEN;
                }
                SCI_MEMCPY(yx_health_msg_Rec.code_content,msg_char,msg_len);
            }
        }
        else
        {
            ZDT_LOG("qrcode NONE");
        }

        //亮码人姓名（脱敏）
        item_json = cJSON_GetObjectItem(json,"userName");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("userName=%s",msg_char);
                if(msg_len > HTTP_HEALTH_NAME_MAX_LEN)
                {
                    msg_len = HTTP_HEALTH_NAME_MAX_LEN;
                }
                SCI_MEMCPY(yx_health_msg_Rec.personName,msg_char,msg_len);
            }
        }
        else
        {
            ZDT_LOG("userName NONE");
        }

        //亮码精确时间
        item_json = cJSON_GetObjectItem(json,"time");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("time=%s",msg_char);
                if(msg_len > HTTP_HEALTH_MSG_MAX_LEN)
                {
                    msg_len = HTTP_HEALTH_MSG_MAX_LEN;
                }
                SCI_MEMCPY(yx_health_msg_Rec.code_time,msg_char,msg_len);
            }
        }
        else
        {
            ZDT_LOG("time NONE");
        }

        //健康码省份来源
        item_json = cJSON_GetObjectItem(json,"healthyCodeSource");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("healthyCodeSource=%s",msg_char);
                if(msg_len > HTTP_HEALTH_QRCODE_MAX_LEN)
                {
                    msg_len = HTTP_HEALTH_QRCODE_MAX_LEN;
                }
                SCI_MEMCPY(yx_health_msg_Rec.code_addr,msg_char,msg_len);
            }
        }
        else
        {
            ZDT_LOG("healthyCodeSource NONE");
        }

        //核酸状态 N:阴性  P:阳性，UNKNOWN:未查询到
        item_json = cJSON_GetObjectItem(json,"nucleicAcid");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("nucleicAcid=%s",msg_char);
                if(0 == strncmp(msg_char,"N",1))
                {
                    yx_health_msg_Rec.nucleic_feminine = 1;
                }
                else if(0 == strncmp(msg_char,"P",1))
                {
                    yx_health_msg_Rec.nucleic_feminine = 2;
                }
                else
                {
                    yx_health_msg_Rec.nucleic_feminine = 0;
                }
            }
        }
        else
        {
            ZDT_LOG("nucleicAcid NONE");
        }

        //核酸时间 格式 yyy-MM-dd HH:mm:ss
        item_json = cJSON_GetObjectItem(json,"nucleicAcidTime");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("nucleicAcidTime=%s",msg_char);
                if(msg_len > HTTP_HEALTH_MSG_MAX_LEN)
                {
                    msg_len = HTTP_HEALTH_MSG_MAX_LEN;
                }
                SCI_MEMCPY(yx_health_msg_Rec.detect_time,msg_char,msg_len);
            }
        }
        else
        {
            ZDT_LOG("nucleicAcidTime NONE");
        }

        //疫苗状态：0:未查询到接种信息,1:不适宜接种,2:未完成基础免疫,3:完成基础免疫,4:完成加强免疫
        item_json = cJSON_GetObjectItem(json,"vaccinesStatus");
        if(item_json)
        {
            val = cJSON_GetObjectInt(item_json);
            ZDT_LOG("vaccinesStatus = %d",val);
            yx_health_msg_Rec.vacc_status = val+1;
        }
        else
        {
            ZDT_LOG("vaccinesStatus NONE");
        }
        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("YX_Net_HTTP_Health_Handle ERR  cJSON_Parse");
    }
    return ret;
}

int YX_Net_HTTP_Health_Rcv_Handle(uint8 * pContent,uint32 ContentLen)
{
    int ret = -1;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    uint16 content_wstr[100] = {0};
    MMI_STRING_T alert_string = {0};
    uint32 ms = 1000;
    if(pContent == PNULL || ContentLen < 2)
    {
        return 0;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"code");
        if(item_json)
        {
            val = cJSON_GetObjectInt(item_json);
            ZDT_LOG("code = %d",val);
        }
        else
        {
            ZDT_LOG("code NONE");
        }
        item_json = cJSON_GetObjectItem(json,"msg");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("msg=%s",msg_char);
            }
        }
        else
        {
            ZDT_LOG("msg NONE");
        }
        if(val == 200)
        {
            uint8 * decrypt_char = PNULL;
            uint32 decrypt_len = 0;
            uint8	*temp_ptr = PNULL;
            item_json = cJSON_GetObjectItem(json,"data");
            if(item_json)
            {
                msg_char = cJSON_GetObjectString(item_json);
                if(msg_char != PNULL)
                {
                    msg_len = strlen(msg_char);
                    ZDT_LOG("data=%s",msg_char);
                    decrypt_char = YX_Func_Decrypt(msg_char,msg_len,&decrypt_len);
                    if(decrypt_char != PNULL)
                    {
                        YX_Net_HTTP_Health_Handle(decrypt_char,decrypt_len);
                        SCI_FREE(decrypt_char);
                        MMIZDT_UpdateHealthWin(1);
                    }
                    else
                    {
                        MMIZDT_UpdateHealthWin(0);
                    }
                }
                else
                {
                    MMIZDT_UpdateHealthWin(0);
                }
            }
            else
            {
                ZDT_LOG("data NONE");
                MMIZDT_UpdateHealthWin(0);
            }
        }
        else
        {
            if(msg_len > 0)
            {
                GUI_UTF8ToWstr(content_wstr, 100, msg_char, msg_len);
                alert_string.wstr_ptr =  content_wstr;
                alert_string.wstr_len = MMIAPICOM_Wstrlen(alert_string.wstr_ptr);
                MMIPUB_OpenAlertWinByTextPtr(&ms,&alert_string,PNULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            MMIZDT_UpdateHealthWin(0);
        }
        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("YX_Net_HTTP_Health_Rcv_Handle ERR  cJSON_Parse");
        MMIZDT_UpdateHealthWin(0);
    }
    return ret;
}

int YX_Net_HTTP_Health_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    int ret = -1;
    uint32 ms = 1000;
    ZDT_LOG("YX_Net_HTTP_Health_Send_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(pRcv != NULL && Rcv_len > 2)
    {
        ret = YX_Net_HTTP_Health_Rcv_Handle(pRcv,Rcv_len);
    }
    else
    {
        MMIZDT_UpdateHealthWin(0);
    }
    return ret;
}

BOOLEAN  YX_Net_HTTP_Health_Send(void)
{
    BOOLEAN res = FALSE;
    cJSON *reqContent = PNULL;
    char * pContent = PNULL;
    char app_domain[100] = {0};
    uint16 content_len = 0;
    uint32 ms = 1000;
    uint8  * crypt_data = PNULL;
    uint32 crypt_len = 0;

    ZDT_LOG("YX_Net_HTTP_Health_Send");
    if(FALSE == ZDT_SIM_Exsit())
    {
        ZDT_LOG("YX_Net_HTTP_Health_Send Err NO SIM");
        return FALSE;
    }
    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        ZDT_LOG("YX_Net_HTTP_Health_Send Err NO Net");
        return FALSE;
    }
    //加密
    reqContent=cJSON_CreateObject();
    if(reqContent)
    {
        crypt_data = YX_Func_Encrypt(g_zdt_phone_imei,ZDT_IMEI_LEN,&crypt_len);
        //crypt_data = YX_Func_Encrypt("862111210542936",ZDT_IMEI_LEN,&crypt_len);
        if(crypt_data != PNULL)
        {
            cJSON_AddItemToObject(reqContent, "imei", cJSON_CreateString(crypt_data));
            SCI_FREE(crypt_data);
        }
        cJSON_AddItemToObject(reqContent, "modelNo", cJSON_CreateString(YX_HTTP_MODLE));
        cJSON_AddItemToObject(reqContent, "provider", cJSON_CreateString(YX_HTTP_COMPANY));
        pContent=cJSON_PrintUnformatted(reqContent);
        if(pContent)
        {
            content_len = strlen(pContent);
            sprintf(app_domain,"http://%s/healthycode/api/healthycode/open/query/v1",yx_DB_Set_Rec.app_domain);
            res = MMIZDT_HTTP_AppSend(FALSE,app_domain,(uint8 *)pContent,content_len,1000,0,0,0,0,0,YX_Net_HTTP_Health_Send_CB);
            SCI_FREE(pContent);
        }
        cJSON_Delete(reqContent);
    }
    return res;
}

/*
{"qrcode":"FJQRJKMC1000000101000000A001eYxlYVeTjfLbT0ZrIs1dlcS1z4ewqUC+SjiB2JxjGs82sWYF2bIWQy+6efhNN2bi1674985035362462653DA1F5C04C2D4429A5E491F284A48527EB99634CBBE88A32C846178A","userName":"*******P","type":3,"validityDate":14}


*/
int YX_Net_HTTP_Health2_DATA_Handle(uint8 * pContent,uint32 ContentLen)
{
    int ret = -1;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    uint16 content_wstr[100] = {0};
    MMI_STRING_T alert_string = {0};
    uint32 ms = 1000;
    if(pContent == PNULL || ContentLen < 2)
    {
        return 0;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        //type
        item_json = cJSON_GetObjectItem(json,"type");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                val = atoi(msg_char);
                ZDT_LOG("type str=%s, val=%d",msg_char,val);
            }
            else
            {
                val = cJSON_GetObjectInt(item_json);
                ZDT_LOG("type=%d",val);
            }
            yx_health_msg_Rec.type2_type = val;
            if(val == 1 || val == 2 || val == 3)
            {
                yx_health_msg_Rec.type2_status = 1;
            }
        }
        else
        {
            ZDT_LOG("type NONE");
        }

        //BASE64图片
        item_json = cJSON_GetObjectItem(json,"qrcodeBase64");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                uint8 * pic_data = PNULL;
                uint32 pic_len = 0;
                msg_len = strlen(msg_char);
                #ifdef WIN32
                    msg_char = zdt_bas64;
                    msg_len = 65508;
                #endif
                ZDT_LOG("qrcodeBase64 len=%d",msg_len);
                pic_data = YX_Func_BASE64_Decode(msg_char,msg_len,&pic_len);
                if(pic_data != PNULL)
                {
                    ZDT_Http_Type2_Base64_Save(pic_data,pic_len);
                    SCI_FREE(pic_data);
                }
            }
        }
        else
        {
            ZDT_LOG("qrcodeBase64 NONE");
        }
        item_json = cJSON_GetObjectItem(json,"qrcode");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("qrcode=%s",msg_char);
                if(msg_len > HTTP_HEALTH_QRCODE_MAX_LEN)
                {
                    msg_len = HTTP_HEALTH_QRCODE_MAX_LEN;
                }
                SCI_MEMCPY(yx_health_msg_Rec.type2_qrcode,msg_char,msg_len);
            }
        }
        else
        {
            ZDT_LOG("qrcode NONE");
        }

        //亮码人姓名（脱敏）
        item_json = cJSON_GetObjectItem(json,"userName");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("userName=%s",msg_char);
                if(msg_len > HTTP_HEALTH_NAME_MAX_LEN)
                {
                    msg_len = HTTP_HEALTH_NAME_MAX_LEN;
                }
                SCI_MEMCPY(yx_health_msg_Rec.type2_username,msg_char,msg_len);
            }
        }
        else
        {
            ZDT_LOG("userName NONE");
        }

        //有效期
        item_json = cJSON_GetObjectItem(json,"validityDate");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                val = atoi(msg_char);
                ZDT_LOG("validityDate str=%s, val=%d",msg_char,val);
            }
            else
            {
                val = cJSON_GetObjectInt(item_json);
                ZDT_LOG("validityDate=%d",val);
            }
            yx_health_msg_Rec.type2_validityDate = val;
        }
        else
        {
            ZDT_LOG("validityDate NONE");
        }

        //二维码颜色
        yx_health_msg_Rec.type2_color = 1;
        item_json = cJSON_GetObjectItem(json,"color");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("color=%s",msg_char);
                if(msg_char[0] == 'G')
                {
                    yx_health_msg_Rec.type2_color = 1;
                }
                else if(msg_char[0] == 'Y')
                {
                    yx_health_msg_Rec.type2_color = 2;
                }
                else if(msg_char[0] == 'R')
                {
                    yx_health_msg_Rec.type2_color = 3;
                }
            }
        }
        else
        {
            ZDT_LOG("color NONE");
        }

        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("YX_Net_HTTP_Health2_DATA_Handle ERR  cJSON_Parse");
    }
    return ret;
}
int YX_Net_HTTP_Health2_Rcv_Handle(uint8 * pContent,uint32 ContentLen)
{
    int ret = -1;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    uint16 content_wstr[100] = {0};
    MMI_STRING_T alert_string = {0};
    uint32 ms = 1000;
    if(pContent == PNULL || ContentLen < 2)
    {
        return 0;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"code");
        if(item_json)
        {
            val = cJSON_GetObjectInt(item_json);
            ZDT_LOG("code = %d",val);
        }
        else
        {
            ZDT_LOG("code NONE");
        }
        item_json = cJSON_GetObjectItem(json,"msg");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("msg=%s",msg_char);
            }
        }
        else
        {
            ZDT_LOG("msg NONE");
        }
        if(val == 200)
        {
            uint8 * decrypt_char = PNULL;
            uint32 decrypt_len = 0;
            uint8	*temp_ptr = PNULL;
            item_json = cJSON_GetObjectItem(json,"data");
            if(item_json)
            {
                msg_char = cJSON_GetObjectString(item_json);
                if(msg_char != PNULL)
                {
                    msg_len = strlen(msg_char);
                    ZDT_LOG("data=%s",msg_char);
                    decrypt_char = YX_Func_Decrypt(msg_char,msg_len,&decrypt_len);
                    if(decrypt_char != PNULL)
                    {
                        YX_Net_HTTP_Health2_DATA_Handle(decrypt_char,decrypt_len);
                        SCI_FREE(decrypt_char);
                        MMIZDT_UpdateHealthWin(2);
                    }
                    else
                    {
                        MMIZDT_UpdateHealthWin(0);
                    }
                }
                else
                {
                    MMIZDT_UpdateHealthWin(0);
                }
            }
            else
            {
                ZDT_LOG("data NONE");
                MMIZDT_UpdateHealthWin(0);
            }
            
        }
        else
        {
            if(msg_len > 0)
            {
                GUI_UTF8ToWstr(content_wstr, 100, msg_char, msg_len);
                alert_string.wstr_ptr =  content_wstr;
                alert_string.wstr_len = MMIAPICOM_Wstrlen(alert_string.wstr_ptr);
                MMIPUB_OpenAlertWinByTextPtr(&ms,&alert_string,PNULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            MMIZDT_UpdateHealthWin(0);
        }
        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("YX_Net_HTTP_Health_Rcv_Handle ERR  cJSON_Parse");
        MMIZDT_UpdateHealthWin(0);
    }
    return ret;
}
int YX_Net_HTTP_Health2_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    int ret = -1;
    ZDT_LOG("YX_Net_HTTP_Health2_Send_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(pRcv != NULL && Rcv_len > 2)
    {
        ret = YX_Net_HTTP_Health2_Rcv_Handle(pRcv,Rcv_len);
    }
    else
    {
        MMIZDT_UpdateHealthWin(0);
    }
    return ret;
}

BOOLEAN  YX_Net_HTTP_Health2_Send(void)
{
    BOOLEAN res = FALSE;
    cJSON *root = PNULL;
    cJSON *reqContent = PNULL;
    char * pContent = PNULL;
    char app_domain[100] = {0};
    uint16 content_len = 0;
    uint8  * crypt_data = PNULL;
    uint32 crypt_len = 0;

    ZDT_LOG("YX_Net_HTTP_Health2_Send");
    if(FALSE == ZDT_SIM_Exsit())
    {
        ZDT_LOG("YX_Net_HTTP_Health2_Send Err NO SIM");
        return FALSE;
    }
    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        ZDT_LOG("YX_Net_HTTP_Health2_Send Err NO Net");
        return FALSE;
    }
    //加密
    root=cJSON_CreateObject();
    if(root)
    {
        reqContent=cJSON_CreateObject();
        if(reqContent)
        {
            cJSON_AddItemToObject(reqContent, "imei", cJSON_CreateString(g_zdt_phone_imei));
            cJSON_AddItemToObject(reqContent, "appCode", cJSON_CreateString("HEALTHY_UPLOAD"));
            pContent=cJSON_PrintUnformatted(reqContent);
            if(pContent)
            {
                content_len = strlen(pContent);
                crypt_data = YX_Func_Encrypt(pContent,content_len,&crypt_len);
                if(crypt_data != PNULL)
                {
                    cJSON_AddItemToObject(root, "data", cJSON_CreateString(crypt_data));
                    SCI_FREE(crypt_data);
                }
                SCI_FREE(pContent);
            }
            cJSON_Delete(reqContent);
        }
        cJSON_AddItemToObject(root, "modelNo", cJSON_CreateString(YX_HTTP_MODLE));
        cJSON_AddItemToObject(root, "provider", cJSON_CreateString(YX_HTTP_COMPANY));
        pContent=cJSON_PrintUnformatted(root);
        if(pContent)
        {
            content_len = strlen(pContent);
            sprintf(app_domain,"http://%s/healthycode/api/healthycode/get/upload/data/v1",yx_DB_Set_Rec.app_domain);
            yx_health_msg_Rec.type2_status = 0;
            res = MMIZDT_HTTP_AppSend(FALSE,app_domain,(uint8 *)pContent,content_len,1000,0,0,0,0,0,YX_Net_HTTP_Health2_Send_CB);
            SCI_FREE(pContent);
        }
        cJSON_Delete(root);
    }
    return res;
}
#endif

int YX_Net_HTTP_AppCount_Rcv_Handle(uint8 * pContent,uint32 ContentLen)
{
    int ret = -1;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    uint16 content_wstr[100] = {0};
    MMI_STRING_T alert_string = {0};
    uint32 ms = 1000;
    if(pContent == PNULL || ContentLen < 2)
    {
        return 0;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"code");
        if(item_json)
        {
            val = cJSON_GetObjectInt(item_json);
            ZDT_LOG("code = %d",val);
        }
        else
        {
            ZDT_LOG("code NONE");
        }
        item_json = cJSON_GetObjectItem(json,"msg");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("msg=%s",msg_char);
            }
        }
        else
        {
            ZDT_LOG("msg NONE");
        }
        if(val == 200)
        {
            uint8 * decrypt_char = PNULL;
            uint32 decrypt_len = 0;
            uint8	*temp_ptr = PNULL;
            item_json = cJSON_GetObjectItem(json,"data");
            if(item_json)
            {
                msg_char = cJSON_GetObjectString(item_json);
                if(msg_char != PNULL)
                {
                    msg_len = strlen(msg_char);
                    ZDT_LOG("data=%s",msg_char);
                    decrypt_char = YX_Func_Decrypt(msg_char,msg_len,&decrypt_len);
                    if(decrypt_char != PNULL)
                    {
                        SCI_FREE(decrypt_char);
                    }
                }
            }
            else
            {
                ZDT_LOG("data NONE");
            }
        }
        else
        {
            if(msg_len > 0)
            {
                GUI_UTF8ToWstr(content_wstr, 100, msg_char, msg_len);
                alert_string.wstr_ptr =  content_wstr;
                alert_string.wstr_len = MMIAPICOM_Wstrlen(alert_string.wstr_ptr);
                MMIPUB_OpenAlertWinByTextPtr(&ms,&alert_string,PNULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
        }
        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("YX_Net_HTTP_Health_Rcv_Handle ERR  cJSON_Parse");
    }
    return ret;
}

int YX_Net_HTTP_AppCount_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    int ret = -1;
    ZDT_LOG("YX_Net_HTTP_AppCount_Send_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(pRcv != NULL && Rcv_len > 2)
    {
        //YX_Net_HTTP_AppCount_Rcv_Handle(pRcv,Rcv_len);
    }
    return ret;
}

BOOLEAN  YX_Net_HTTP_AppCount_Send(char * event_code, uint32 event_val)
{
    BOOLEAN res = FALSE;
    cJSON *root = PNULL;
    cJSON *reqContent = PNULL;
    char * pContent = PNULL;
    char app_domain[100] = {0};
    uint16 content_len = 0;
    uint8  * crypt_data = PNULL;
    uint32 crypt_len = 0;
    uint32 bj_time_s = ZDTGetBJTime();
    char  bj_time_str[100] = {0};
    char  val_str[100] = {0};
    
    ZDT_LOG("YX_Net_HTTP_AppCount_Send");
    if(FALSE == ZDT_SIM_Exsit())
    {
        ZDT_LOG("YX_Net_HTTP_AppCount_Send Err NO SIM");
        return FALSE;
    }
    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        ZDT_LOG("YX_Net_HTTP_AppCount_Send Err NO Net");
        return FALSE;
    }
    //加密
    root=cJSON_CreateObject();
    if(root)
    {
        reqContent=cJSON_CreateObject();
        if(reqContent)
        {
            cJSON_AddItemToObject(reqContent, "imei", cJSON_CreateString(g_zdt_phone_imei));
            sprintf(bj_time_str,"%d000",bj_time_s);
            cJSON_AddItemToObject(reqContent, "time", cJSON_CreateString(bj_time_str));
            cJSON_AddItemToObject(reqContent, "eventCode", cJSON_CreateString(event_code));
            sprintf(val_str,"%d",event_val);
            cJSON_AddItemToObject(reqContent, "eventValue", cJSON_CreateString(val_str));
            pContent=cJSON_PrintUnformatted(reqContent);
            if(pContent)
            {
                content_len = strlen(pContent);
                crypt_data = YX_Func_Encrypt(pContent,content_len,&crypt_len);
                if(crypt_data != PNULL)
                {
                    cJSON_AddItemToObject(root, "data", cJSON_CreateString(crypt_data));
                    SCI_FREE(crypt_data);
                }
                SCI_FREE(pContent);
            }
            cJSON_Delete(reqContent);
        }
        cJSON_AddItemToObject(root, "modelNo", cJSON_CreateString(YX_HTTP_MODLE));
        cJSON_AddItemToObject(root, "provider", cJSON_CreateString(YX_HTTP_COMPANY));
        pContent=cJSON_PrintUnformatted(root);
        if(pContent)
        {
            content_len = strlen(pContent);
            sprintf(app_domain,"http://%s/analysis/api/event/device/upload/v1",yx_DB_Set_Rec.app_domain);
            res = MMIZDT_HTTP_AppSend(FALSE,app_domain,(uint8 *)pContent,content_len,1000,0,0,0,0,0,YX_Net_HTTP_AppCount_Send_CB);
            SCI_FREE(pContent);
        }
        cJSON_Delete(root);
    }
    return res;
}

BOOLEAN  YX_AppCount_Check_Send(void)
{
    BOOLEAN res = FALSE;
    res = ZDT_DB_APPCOUNT_IsChange();
    if(res)
    {
        if(zdt_DB_AppCount_Rec.ADDRESSBOOK != zdt_DB_AppCount_Bak.ADDRESSBOOK)
        {
            YX_Net_HTTP_AppCount_Send("USE_ADDRESSBOOK",zdt_DB_AppCount_Rec.ADDRESSBOOK);
        }
        if(zdt_DB_AppCount_Rec.PEDOMETER != zdt_DB_AppCount_Bak.PEDOMETER)
        {
            YX_Net_HTTP_AppCount_Send("USE_PEDOMETER",zdt_DB_AppCount_Rec.PEDOMETER);
        }
        if(zdt_DB_AppCount_Rec.ALIPAY != zdt_DB_AppCount_Bak.ALIPAY)
        {
            YX_Net_HTTP_AppCount_Send("USE_ALIPAY",zdt_DB_AppCount_Rec.ALIPAY);
        }
        if(zdt_DB_AppCount_Rec.CLOCK != zdt_DB_AppCount_Bak.CLOCK)
        {
            YX_Net_HTTP_AppCount_Send("USE_CLOCK",zdt_DB_AppCount_Rec.CLOCK);
        }
        if(zdt_DB_AppCount_Rec.CALENDAR != zdt_DB_AppCount_Bak.CALENDAR)
        {
            YX_Net_HTTP_AppCount_Send("USE_CALENDAR",zdt_DB_AppCount_Rec.CALENDAR);
        }
        if(zdt_DB_AppCount_Rec.MESSAGE != zdt_DB_AppCount_Bak.MESSAGE)
        {
            YX_Net_HTTP_AppCount_Send("USE_MESSAGE",zdt_DB_AppCount_Rec.MESSAGE);
        }
        if(zdt_DB_AppCount_Rec.CALL_RECORD != zdt_DB_AppCount_Bak.CALL_RECORD)
        {
            YX_Net_HTTP_AppCount_Send("USE_CALL_RECORD",zdt_DB_AppCount_Rec.CALL_RECORD);
        }
        if(zdt_DB_AppCount_Rec.HOMEWORK != zdt_DB_AppCount_Bak.HOMEWORK)
        {
            YX_Net_HTTP_AppCount_Send("USE_HOMEWORK",zdt_DB_AppCount_Rec.HOMEWORK);
        }
        if(zdt_DB_AppCount_Rec.ANSWERCARD != zdt_DB_AppCount_Bak.ANSWERCARD)
        {
            YX_Net_HTTP_AppCount_Send("USE_ANSWERCARD",zdt_DB_AppCount_Rec.ANSWERCARD);
        }
        if(zdt_DB_AppCount_Rec.HEALTHCODE != zdt_DB_AppCount_Bak.HEALTHCODE)
        {
            YX_Net_HTTP_AppCount_Send("USE_HEALTHCODE",zdt_DB_AppCount_Rec.HEALTHCODE);
        }
        if(zdt_DB_AppCount_Rec.IDCODE != zdt_DB_AppCount_Bak.IDCODE)
        {
            YX_Net_HTTP_AppCount_Send("USE_IDCODE",zdt_DB_AppCount_Rec.IDCODE);
        }
        ZDT_DB_APPCOUNT_ListModify();
    }
    
    return res;
}

int YX_Net_HTTP_IP_DATA_Handle(uint8 * pContent,uint32 ContentLen)
{
    int ret = -1;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    uint16 content_wstr[100] = {0};
    MMI_STRING_T alert_string = {0};
    uint32 ms = 1000;
    if(pContent == PNULL || ContentLen < 2)
    {
        return 0;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"ip");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("ip=%s",msg_char);
                if(msg_len > MAX_YX_IP_DOMAIN_SIZE)
                {
                    msg_len = MAX_YX_IP_DOMAIN_SIZE;
                }
                SCI_MEMSET(yx_DB_Set_Rec.ip_domain,0,MAX_YX_IP_DOMAIN_SIZE+1);
                SCI_MEMCPY(yx_DB_Set_Rec.ip_domain,msg_char,msg_len);
            }
        }
        else
        {
            ZDT_LOG("ip NONE");
        }

        item_json = cJSON_GetObjectItem(json,"port");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                val = atoi(msg_char);
                ZDT_LOG("port str=%s, val=%d",msg_char,val);
            }
            else
            {
                val = cJSON_GetObjectInt(item_json);
                ZDT_LOG("port=%d",val);
            }
            yx_DB_Set_Rec.ip_port = val;
            YX_DB_SET_ListModify();
            g_is_yx_first_reg = FALSE;
            ret = 1;
        }
        else
        {
            ZDT_LOG("port NONE");
        }

        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("YX_Net_HTTP_IP_DATA_Handle ERR  cJSON_Parse");
    }
    return ret;
}

int YX_Net_HTTP_IP_Rcv_Handle(uint8 * pContent,uint32 ContentLen)
{
    int ret = -1;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    uint16 content_wstr[100] = {0};
    MMI_STRING_T alert_string = {0};
    uint32 ms = 1000;
    if(pContent == PNULL || ContentLen < 2)
    {
        return 0;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"code");
        if(item_json)
        {
            val = cJSON_GetObjectInt(item_json);
            ZDT_LOG("code = %d",val);
        }
        else
        {
            ZDT_LOG("code NONE");
        }
        item_json = cJSON_GetObjectItem(json,"msg");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("msg=%s",msg_char);
            }
        }
        else
        {
            ZDT_LOG("msg NONE");
        }
        if(val == 200)
        {
            uint8 * decrypt_char = PNULL;
            uint32 decrypt_len = 0;
            uint8	*temp_ptr = PNULL;
            item_json = cJSON_GetObjectItem(json,"data");
            if(item_json)
            {
                msg_char = cJSON_GetObjectString(item_json);
                if(msg_char != PNULL)
                {
                    msg_len = strlen(msg_char);
                    ZDT_LOG("data=%s",msg_char);
                    decrypt_char = YX_Func_Decrypt(msg_char,msg_len,&decrypt_len);
                    if(decrypt_char != PNULL)
                    {
                        ret = YX_Net_HTTP_IP_DATA_Handle(decrypt_char,decrypt_len);
                        SCI_FREE(decrypt_char);
                    }
                }
            }
            else
            {
                ZDT_LOG("data NONE");
            }
        }
        else
        {
            #if 0
            if(msg_len > 0)
            {
                GUI_UTF8ToWstr(content_wstr, 100, msg_char, msg_len);
                alert_string.wstr_ptr =  content_wstr;
                alert_string.wstr_len = MMIAPICOM_Wstrlen(alert_string.wstr_ptr);
                MMIPUB_OpenAlertWinByTextPtr(&ms,&alert_string,PNULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            #endif
        }
        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("YX_Net_HTTP_Health_Rcv_Handle ERR  cJSON_Parse");
    }
    return ret;
}

int YX_Net_HTTP_IP_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    int ret = -1;
    ZDT_LOG("YX_Net_HTTP_IP_Send_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(pRcv != NULL && Rcv_len > 2)
    {
        ret = YX_Net_HTTP_IP_Rcv_Handle(pRcv,Rcv_len);
    }
    if(ret > 0)
    {
        MMIZDT_Net_Reset(TRUE);
    }
    else
    {
        MMIZDT_Net_Reset(FALSE);
    }
    return ret;
}

BOOLEAN  YX_Net_HTTP_IP_Send(void)
{
    BOOLEAN res = FALSE;
    cJSON *root = PNULL;
    cJSON *reqContent = PNULL;
    char * pContent = PNULL;
    char app_domain[100] = {0};
    uint16 content_len = 0;
    uint8  * crypt_data = PNULL;
    uint32 crypt_len = 0;
    char  val_str[100] = {0};
    
    ZDT_LOG("YX_Net_HTTP_IP_Send");
    if(FALSE == ZDT_SIM_Exsit())
    {
        ZDT_LOG("YX_Net_HTTP_IP_Send Err NO SIM");
        return FALSE;
    }
    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        ZDT_LOG("YX_Net_HTTP_IP_Send Err NO Net");
        return FALSE;
    }
    //加密
    root=cJSON_CreateObject();
    if(root)
    {
        reqContent=cJSON_CreateObject();
        if(reqContent)
        {
            cJSON_AddItemToObject(reqContent, "imei", cJSON_CreateString(g_zdt_phone_imei));
            cJSON_AddItemToObject(reqContent, "deviceProviderId", cJSON_CreateString(YX_DEV_PRODUCT_KEY_STR));
            pContent=cJSON_PrintUnformatted(reqContent);
            if(pContent)
            {
                content_len = strlen(pContent);
                crypt_data = YX_Func_Encrypt(pContent,content_len,&crypt_len);
                if(crypt_data != PNULL)
                {
                    cJSON_AddItemToObject(root, "data", cJSON_CreateString(crypt_data));
                    SCI_FREE(crypt_data);
                }
                SCI_FREE(pContent);
            }
            cJSON_Delete(reqContent);
        }
        cJSON_AddItemToObject(root, "productKey", cJSON_CreateString(YX_AES_KEY_STR));
        pContent=cJSON_PrintUnformatted(root);
        if(pContent)
        {
            content_len = strlen(pContent);
            sprintf(app_domain,"http://%s/uc/api/student/card/server/get/v1",yx_DB_Set_Rec.app_domain);
            res = MMIZDT_HTTP_AppSend(FALSE,app_domain,(uint8 *)pContent,content_len,1000,0,0,0,0,0,YX_Net_HTTP_IP_Send_CB);
            SCI_FREE(pContent);
        }
        cJSON_Delete(root);
    }
    return res;
}

static void YX_Heart_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_HEART,NULL,0);
    return;
}

void YX_Heart_TimeStart(YX_APP_T *pMe,uint16 time_s)
{
    if(0 != pMe->m_yx_heart_timer_id)
    {
        MMK_StopTimer(pMe->m_yx_heart_timer_id);
        pMe->m_yx_heart_timer_id = 0;
    }
    
    pMe->m_yx_heart_timer_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        YX_Heart_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    return;
}

void YX_Heart_TimeStop(YX_APP_T *pMe)
{
    if(0 != pMe->m_yx_heart_timer_id)
    {
        MMK_StopTimer(pMe->m_yx_heart_timer_id);
        pMe->m_yx_heart_timer_id = 0;
    }
}

BOOLEAN YX_LocReport_JudgeSend(void)
{
    BOOLEAN res = FALSE;
    res = YX_LocReport_Judge();
    if(res)
    {
        if(s_yx_gps_loc_short_mode)
        {
            ZDT_GPS_Set_PowerNoOff(TRUE);
        }
        else
        {
            ZDT_GPS_Set_PowerNoOff(FALSE);
        }
    }
    else
    {
    #ifndef WIN32
        if(ZDT_GPS_Get_PowerNoOff())
        {
            res = TRUE;
            ZDT_GPS_Set_PowerNoOff(FALSE);
        }
	#endif	
    }
    YX_LocReport_Reset();
    return res;
}

static void YX_LocReport_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    YX_APP_T *pMe = (YX_APP_T *)param;
    if(g_is_yx_is_locing)
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_LOCREPORT,NULL,0);
    }
    else
    {
        if(YX_LocReport_JudgeSend())
        {
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_LOCREPORT,NULL,0);
        }
        else
        {
            #if 0
            if(g_yx_loc_timer_interval != 0)
            {
                MMK_StartTimerCallback(pMe->m_yx_pos_timer_id,g_yx_loc_timer_interval*1000, 
                                                                        YX_LocReport_TimeOut, 
                                                                        (uint32)pMe,
                                                                        FALSE);
            }
            #endif
        }
    }
}

void YX_LocReport_TimeStart(YX_APP_T *pMe,uint16 time_s)
{
    if(0 != pMe->m_yx_pos_timer_id)
    {
        MMK_StopTimer(pMe->m_yx_pos_timer_id);
        pMe->m_yx_pos_timer_id = 0;
    }
    
    if(time_s == 0)
    {
        return;
    }
    
    pMe->m_yx_pos_timer_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        YX_LocReport_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        TRUE);
}

void YX_LocReport_TimeStop(YX_APP_T *pMe)
{
    if(0 != pMe->m_yx_pos_timer_id)
    {
        MMK_StopTimer(pMe->m_yx_pos_timer_id);
        pMe->m_yx_pos_timer_id = 0;
    }
}

void YX_LocReport_TimeSet(uint16 time_val)
{
    uint16 left = 0;
    uint16 lk_num = 0;
    
    g_yx_loc_timer_lk_num = 0;
    #if 1
    g_yx_loc_timer_interval = time_val;
    if(time_val != 0 && time_val <= YX_LOC_PRE_TIME_LONG)
    {
        s_yx_gps_loc_short_mode = 1;
    }
    else
    {
        s_yx_gps_loc_short_mode = 0;
    }
    #else
    g_yx_loc_timer_interval = 0;
    if(time_val != 0)
    {
        lk_num = time_val/g_yx_cur_heart_time;
        left = time_val%g_yx_cur_heart_time;
        if(lk_num > 0)
        {
            if(left <= 60)
            {
                g_yx_loc_timer_lk_num = lk_num;
            }
            else if( (g_yx_cur_heart_time-left) <= 60)
            {
                g_yx_loc_timer_lk_num = lk_num + 1;
            }
            else
            {
                g_yx_loc_timer_interval = time_val;
            }
        }
        else
        {
            if((g_yx_cur_heart_time-left) <= 60)
            {
                g_yx_loc_timer_lk_num = 1;
            }
            else
            {
                g_yx_loc_timer_interval = time_val;
            }
        }
    }
    #endif
}

BOOLEAN YX_LocReport_In_LK(uint16 num)
{
    uint16 cur_times = 0;
    if(g_yx_loc_timer_interval == 0 && g_yx_loc_timer_lk_num > 0)
    {
        cur_times = num%g_yx_loc_timer_lk_num;
        if(cur_times == 0)
        {
            return TRUE;

        }
    }
    return FALSE;
}

static void YX_LocCR_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    YX_APP_T *pMe = (YX_APP_T *)param;
    uint16 left = 0;
    
    YX_LocReport_TimeSet(yx_DB_Set_Rec.time_interval);
    
#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 
    YX_LOCTION_Start(pMe,YX_LOCTION_TYPE_OFF,NULL,0);
#endif
    g_is_yx_is_locing = FALSE;
    YX_LocReport_TimeStop(pMe);
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_LOCREPORT,NULL,0);
    YX_LocReport_TimeStart(pMe,yx_DB_Set_Rec.time_interval);
}

void YX_LocCR_TimeStart(YX_APP_T *pMe,uint16 time_s)
{
    if(time_s == 0)
    {
        time_s = 180;
    }
    if(0 != pMe->m_yx_cr_timer_id)
    {
        MMK_StopTimer(pMe->m_yx_cr_timer_id);
        pMe->m_yx_cr_timer_id = 0;
    }
    
    pMe->m_yx_cr_timer_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        YX_LocCR_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    
}

void YX_LocCR_TimeStop(YX_APP_T *pMe)
{
    if(0 != pMe->m_yx_cr_timer_id)
    {
        MMK_StopTimer(pMe->m_yx_cr_timer_id);
        pMe->m_yx_cr_timer_id = 0;
    }
}

static void YX_Find_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    YX_API_CZSB_Stop();
}

void YX_Find_TimeStart(YX_APP_T *pMe,uint16 time_s)
{
    if(time_s == 0)
    {
        time_s = 60;
    }
    if(0 != pMe->m_yx_find_timer_id)
    {
        MMK_StopTimer(pMe->m_yx_find_timer_id);
        pMe->m_yx_find_timer_id = 0;
    }
    
    pMe->m_yx_find_timer_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        YX_Find_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

void YX_Find_TimeStop(YX_APP_T *pMe)
{
    if(0 != pMe->m_yx_find_timer_id)
    {
        MMK_StopTimer(pMe->m_yx_find_timer_id);
        pMe->m_yx_find_timer_id = 0;
    }
}

static int YX_Net_Send_GET_SERVER_INFO_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"GET_SERVER_INFO",strlen("GET_SERVER_INFO")))
            {
                ZDT_LOG("YX_Net_Send_GET_SERVER_INFO_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_GET_SERVER_INFO_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,200);
        if(ret > 0)
        {
            SCI_MEMSET(yx_DB_Set_Rec.ip_domain,0,MAX_YX_IP_DOMAIN_SIZE+1);
            SCI_MEMCPY(yx_DB_Set_Rec.ip_domain,buf,ret);
            ZDT_LOG("ip=%s",buf);
        }
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,200);
        if(ret > 0)
        {
            yx_DB_Set_Rec.ip_port = atoi(buf);
            ZDT_LOG("port=%s, ip_port=%d",buf,yx_DB_Set_Rec.ip_port);
            YX_DB_SET_ListModify();
            g_is_yx_first_reg = FALSE;
            return ZDT_TCP_RET_OK;
        }
    }

    ZDT_LOG("YX_Net_Send_GET_SERVER_INFO_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

uint32 YX_Net_Send_GET_SERVER_INFO(YX_APP_T *pMe)
{
    // 设备发起的注册请求，必须使用固定的地址：zd.ctxy.cn:9141
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '1';
    send_len = 1;
    send_hande = YX_Net_TCPSend('3',"GET_SERVER_INFO",send_buf,send_len,200,0,0,YX_Net_Send_GET_SERVER_INFO_CB);
    ZDT_LOG("YX_Net_Send_GET_SERVER_INFO Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

static int YX_Net_Send_DEVICE_LOGIN_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    uint8 status = 0;
    char sms_num[201] = {0};
    uint8 need_send_sms = 0;
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"DEVICE_LOGIN",strlen("DEVICE_LOGIN")))
            {
                ZDT_LOG("YX_Net_Send_DEVICE_LOGIN_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_DEVICE_LOGIN_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,200);
        if(ret > 0)
        {
            status = atoi(buf);
            ZDT_LOG("status=%s,%d",buf,status);
            if(status == 1)
            {
                ZDT_LOG("YX_Net_Send_DEVICE_LOGIN_CB  ERR status");
                g_yx_is_need_reset_factory = TRUE;
                return ZDT_TCP_RET_ERR;
            }
        }
        #if 0
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,200);
        if(ret > 0)
        {
            SCI_MEMCPY(sms_num,buf,ret);
            ZDT_LOG("sms_num=%s",buf);
        }
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,200);
        if(ret > 0)
        {
            need_send_sms = atoi(buf);
            ZDT_LOG("need_sms=%s,%d",buf,need_send_sms);
            if(need_send_sms)
            {
                YX_API_SMS_SendReg(sms_num);
            }
        }
        #endif
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_DEVICE_LOGIN_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

/*
[35380100360174,898600MFSSYYGXXXXXXP,201805082000001000,DEVICE_LOGIN,3,20180528111820,12,5@1@1@0@1@21]
*/
uint32 YX_Net_Send_DEVICE_LOGIN(YX_APP_T *pMe)
{
    uint8 send_buf[100] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[send_len++] = '4';
    send_buf[send_len++] = '@';
    send_buf[send_len++] = '1';
    send_buf[send_len++] = '@';
    send_buf[send_len++] = '1';
    send_buf[send_len++] = '@';
    send_buf[send_len++] = '0';
    send_buf[send_len++] = '@';
    send_buf[send_len++] = '1';
    send_buf[send_len++] = '@';
    send_buf[send_len++] = '2';
    send_buf[send_len++] = '1';
    send_hande = YX_Net_TCPSend('3',"DEVICE_LOGIN",send_buf,send_len,150,0,0,YX_Net_Send_DEVICE_LOGIN_CB);
    ZDT_LOG("YX_Net_Send_DEVICE_LOGIN Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

uint32 YX_Net_Send_REPORT_HEARTBEAT(YX_APP_T *pMe)
{
    uint8 send_buf[101] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_len = YX_Func_GetBatPercent(send_buf);
    //send_len += YX_Func_GetJPNum(send_buf+send_len);	
    send_hande = YX_Net_TCPSend('3',"REPORT_HEARTBEAT",send_buf,send_len,100,0,0,PNULL);
    ZDT_LOG("YX_Net_Send_REPORT_HEARTBEAT Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

static int YX_Net_Send_ALARM_POWER_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    uint8 status = 0;
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"ALARM_POWER",strlen("ALARM_POWER")))
            {
                ZDT_LOG("YX_Net_Send_ALARM_POWER_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_ALARM_POWER_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,200);
        if(ret > 0)
        {
            status = atoi(buf);
            ZDT_LOG("status=%s,%d",buf,status);
            if(status != 0)
            {
                ZDT_LOG("YX_Net_Send_ALARM_POWER_CB  ERR status=%d",status);
                return ZDT_TCP_RET_ERR;
            }
        }
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_ALARM_POWER_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

/*
[35380100360174,898600MFSSYYGXXXXXXP,201805082000001000,ALARM_POWER,3,20180528111820,5,1@10%]
1=缺电报警;2=关机报警;3=自动关机报警;4=开机报警;5=设备充电;6=电源已断;7=设备电量已充满
*/
uint32 YX_Net_Send_ALARM_POWER(YX_APP_T *pMe, uint16 alert_type)
{
    uint8 send_buf[100] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_ALARM_POWER Err NoReg");
        return 0;
    }
    send_len += YX_Func_GetAlertType(send_buf,alert_type);
    send_buf[send_len++] = '@';
    send_len += YX_Func_GetBatPercent(send_buf+send_len);
    send_hande = YX_Net_TCPSend('3',"ALARM_POWER",send_buf,send_len,100,0,0,YX_Net_Send_ALARM_POWER_CB);
    ZDT_LOG("YX_Net_Send_ALARM_POWER Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

static int YX_Net_Send_REPORT_LOCATION_INFO_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    uint8 status = 0;
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"REPORT_LOCATION_INFO",strlen("REPORT_LOCATION_INFO")))
            {
                ZDT_LOG("YX_Net_Send_REPORT_LOCATION_INFO_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_REPORT_LOCATION_INFO_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,200);
        if(ret > 0)
        {
            status = atoi(buf);
            ZDT_LOG("status=%s,%d",buf,status);
            if(status != 0)
            {
                ZDT_LOG("YX_Net_Send_REPORT_LOCATION_INFO_CB  ERR status=%d",status);
                return ZDT_TCP_RET_ERR;
            }
        }
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_REPORT_LOCATION_INFO_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

uint32 YX_Net_Send_REPORT_LOCATION_INFO(YX_APP_T *pMe, uint8 loc_ok)
{
    uint8 send_buf[2048] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_REPORT_LOCATION_INFO Err NoReg");
        return 0;
    }
    ZDT_LOG("YX_Net_Send_REPORT_LOCATION_INFO loc_ok=0x%x",loc_ok);
    send_len += YX_Func_GetGPSInfo(send_buf+send_len,loc_ok);
    send_buf[send_len++] = '@';
    send_len += YX_Func_GetSSInfo(send_buf+send_len);
    send_buf[send_len++] = '@';
    send_len += YX_Func_GetWIFIInfo(send_buf+send_len,loc_ok);
    send_hande = YX_Net_TCPSend('3',"REPORT_LOCATION_INFO",send_buf,send_len,100,0,0,YX_Net_Send_REPORT_LOCATION_INFO_CB);
    ZDT_LOG("YX_Net_Send_REPORT_LOCATION_INFO Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

static int YX_Net_Send_REPORT_CALL_LOG_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    uint8 status = 0;
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"REPORT_CALL_LOG",strlen("REPORT_CALL_LOG")))
            {
                ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,200);
        if(ret > 0)
        {
            status = atoi(buf);
            ZDT_LOG("status=%s,%d",buf,status);
            if(status != 0)
            {
                ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG_CB  ERR status=%d",status);
                return ZDT_TCP_RET_ERR;
            }
        }
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

/*
[35380100360174,898600MFSSYYGXXXXXXP,201805082000001000,REPORT_CALL_LOG,3,20180528111820,44,18785192785@20180718121221!20180718121258@10@0]
*/
uint32 YX_Net_Send_REPORT_CALL_LOG(MMICL_CALL_TYPE_E call_type, MMICL_CALL_INFO_T call_info)
{
    uint8 send_buf[1024] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    uint32 cur_call_time = 0;
    uint8 call_mode = 0;
    MMI_TM_T tm = {0};
    uint32 call_duration = 0;
    uint32 call_duration_s = 0;
    uint8 time_start[50] = {0};
    uint8 time_end[50] = {0};
    uint8   tele_num[CC_MAX_TELE_NUM_LEN + 2]= {0};
    uint8	tel_num_len = 0;
    
    if(g_yx_app.m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG Err NoReg");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG Start");

 #if defined(MMI_BLACKLIST_SUPPORT)
    if(call_type != MMICL_CALL_DIALED && call_type != MMICL_CALL_RECEIVED && call_type != MMICL_CALL_MISSED && call_type != MMICL_CALL_REFUSED)
#else
    if(call_type != MMICL_CALL_DIALED && call_type != MMICL_CALL_RECEIVED && call_type != MMICL_CALL_MISSED)
#endif
    {
        ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG ERR call_type=%d",call_type);
        return 0;
    }

    if(call_type == MMICL_CALL_DIALED)
    {
        call_mode = 1;
    }
 #if defined(MMI_BLACKLIST_SUPPORT)
    else if(call_type == MMICL_CALL_REFUSED)
    {
        call_mode = 2;
    }
#endif
    else
    {
        call_mode = 0;
    }
    
    if( call_info.call_num.is_num_present )
    {        
        tel_num_len =  MMIAPICOM_GenNetDispNumber( call_info.call_num.number.number_type,
                                    (uint8)call_info.call_num.number.num_len,
                                    (uint8*)call_info.call_num.number.party_num,
                                    tele_num,
                                    (CC_MAX_TELE_NUM_LEN + 2));

    }
                
    cur_call_time = call_info.call_time.start_time;
    tm = MMIAPICOM_Second2Tm(cur_call_time);
    sprintf(time_start,"%04d%02d%02d%02d%02d%02d",tm.tm_year,tm.tm_mon, tm.tm_mday,tm.tm_hour,tm.tm_min, tm.tm_sec);
    
    call_duration = MMIAPICL_GetCallDurationTime(call_info.call_time.start_time,call_info.call_time.stop_time);
    tm = MMIAPICOM_Second2Tm(call_duration);
    call_duration_s = tm.tm_hour*3600+tm.tm_min*60+tm.tm_sec;
                
    cur_call_time += call_duration;
    tm = MMIAPICOM_Second2Tm(cur_call_time);
    sprintf(time_end,"%04d%02d%02d%02d%02d%02d",tm.tm_year,tm.tm_mon, tm.tm_mday,tm.tm_hour,tm.tm_min, tm.tm_sec);
    
    sprintf(send_buf,"%s@%s!%s@%d@%d",tele_num,time_start,time_end,call_duration_s,call_mode);
    send_len = strlen(send_buf);
    send_hande = YX_Net_TCPSend('3',"REPORT_CALL_LOG",send_buf,send_len,100,0,0,YX_Net_Send_REPORT_CALL_LOG_CB);
    ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

uint32 YX_Net_Send_REPORT_CALL_LOG_All(YX_APP_T *pMe)
{
    uint8 * send_buf = PNULL;
    uint32 send_len = 0;
    uint32 send_hande = 0;
    uint8 i = 0;
    uint16 j = 0;
    MMICL_CALL_TYPE_E call_type = MMICL_CALL_ALL;
    MMICL_CALL_ARRAY_BCDINFO_T* p_cl_arry = PNULL;
    uint16 call_times;
    
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG Err NoReg");
        return 0;
    }
    
    p_cl_arry = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(p_cl_arry == PNULL)
    {
        ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG Err NO CL BUF");
        return 0;
    }
    ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG Start");

    SCI_MEMSET(p_cl_arry, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    MMICL_GetRecordInfo(call_type, p_cl_arry);
    if(p_cl_arry->record_num == 0)
    {
        ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG ERR record_num NULL");
        SCI_FREE(p_cl_arry);
        return 0;
    }
    send_len = p_cl_arry->record_num * MMICL_MAX_CALL_TIMES * 200;
    send_buf = SCI_ALLOC_APPZ(send_len);
    if(send_buf == PNULL)
    {
        ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG ERR Mem=%d",send_len);
        SCI_FREE(p_cl_arry);
        return 0;
    }
    for (i = 0; i < p_cl_arry->record_num; i++)		// this is for example for simulator
    {
        call_times = p_cl_arry->call_bcdinfo[i].call_times;
        if(call_times > MMICL_MAX_CALL_TIMES)
        {
            call_times = MMICL_MAX_CALL_TIMES;
        }
        for(j = 0; j < call_times; j++)
        {
            uint32 cur_call_time = 0;
            uint8 call_mode = 0;
            MMI_TM_T tm = {0};
            uint32 call_duration = 0;
            uint32 call_duration_s = 0;
            uint8 item_buf[200] = {0};
            uint8 call_num[100] = {0};
            uint8 time_start[50] = {0};
            uint8 time_end[50] = {0};
           if(p_cl_arry->call_bcdinfo[i].call_type == MMICL_CALL_MISSED
                 || p_cl_arry->call_bcdinfo[i].call_type == MMICL_CALL_RECEIVED
                 || p_cl_arry->call_bcdinfo[i].call_type == MMICL_CALL_DIALED
              )
            {
                    if(p_cl_arry->call_bcdinfo[i].call_type == MMICL_CALL_DIALED)
                    {
                        call_mode = 1;
                    }
                    else
                    {
                        call_mode = 0;
                    }
                    
                    if (p_cl_arry->call_bcdinfo[i].is_num_present)//电话号码存在
                    {
                        //get number
                        MMICL_GenDispNumber(p_cl_arry->call_bcdinfo[i].call_type,
                            p_cl_arry->call_bcdinfo[i].number_type,
                            p_cl_arry->call_bcdinfo[i].number_len,
                            p_cl_arry->call_bcdinfo[i].number,
                            call_num,
                            90);
                    }
                    
#ifdef CALL_TIMES_SUPPORT
                    cur_call_time = p_cl_arry->call_bcdinfo[i].call_start_time[j];
#else
                    cur_call_time = p_cl_arry->call_bcdinfo[i].call_start_time;
#endif
                    tm = MMIAPICOM_Second2Tm(cur_call_time);
                    sprintf(time_start,"%04d%02d%02d%02d%02d%02d",tm.tm_year,tm.tm_mon, tm.tm_mday,tm.tm_hour,tm.tm_min, tm.tm_sec);

#ifdef CALL_TIMES_SUPPORT
                    call_duration = p_cl_arry->call_bcdinfo[i].call_duration_time[j];
#else
                    call_duration = p_cl_arry->call_bcdinfo[i].call_duration_time;
#endif
                    tm = MMIAPICOM_Second2Tm(call_duration);
                    call_duration_s = tm.tm_hour*3600+tm.tm_min*60+tm.tm_sec;
                    
                    cur_call_time += call_duration;
                    tm = MMIAPICOM_Second2Tm(cur_call_time);
                    sprintf(time_end,"%04d%02d%02d%02d%02d%02d",tm.tm_year,tm.tm_mon, tm.tm_mday,tm.tm_hour,tm.tm_min, tm.tm_sec);
                    sprintf(item_buf,"%s@%s!%s@%d@%d@",call_num,time_start,time_end,call_duration_s,call_mode);
                    strcat(send_buf,item_buf);
            }
        }
    }

    send_len = strlen(send_buf);
    send_hande = YX_Net_TCPSend('3',"REPORT_CALL_LOG",send_buf,send_len,100,0,0,YX_Net_Send_REPORT_CALL_LOG_CB);
    ZDT_LOG("YX_Net_Send_REPORT_CALL_LOG Handle=0x%x,%s",send_hande,send_buf);
    SCI_FREE(p_cl_arry);
    SCI_FREE(send_buf);
    return send_hande;
}

static int YX_Net_Send_REPORT_SOS_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    uint8 status = 0;
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"REPORT_SOS",strlen("REPORT_SOS")))
            {
                ZDT_LOG("YX_Net_Send_REPORT_SOS_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_REPORT_SOS_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,200);
        if(ret > 0)
        {
            status = atoi(buf);
            ZDT_LOG("status=%s,%d",buf,status);
            if(status != 0)
            {
                ZDT_LOG("YX_Net_Send_REPORT_SOS_CB  ERR status=%d",status);
                return ZDT_TCP_RET_ERR;
            }
        }
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_REPORT_SOS_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

uint32 YX_Net_Send_REPORT_SOS(void)
{
    uint8 send_buf[10] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(g_yx_app.m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_REPORT_SOS Err NoReg");
        return 0;
    }
    ZDT_LOG("YX_Net_Send_REPORT_SOS Start");
    send_buf[send_len++] = '1';
    send_hande = YX_Net_TCPSend('3',"REPORT_SOS",send_buf,send_len,100,0,0,YX_Net_Send_REPORT_SOS_CB);
    ZDT_LOG("YX_Net_Send_REPORT_SOS Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

static int YX_Net_Send_REPORT_TEMPER_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"REPORT_TEMPER",strlen("REPORT_TEMPER")))
            {
                ZDT_LOG("YX_Net_Send_REPORT_TEMPER_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_REPORT_TEMPER_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_REPORT_TEMPER_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

/*
[864002050000695,89860089271996303433,202007281740190085,REPORT_TEMPER,3,20200728174019,5,36.2]
*/
uint32 YX_Net_Send_REPORT_TEMPER(YX_APP_T *pMe, uint16 alert_type)
{
    uint8 send_buf[100] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_REPORT_TEMPER Err NoReg");
        return 0;
    }
    send_len += YX_Func_GetTemper(send_buf,alert_type);
    send_hande = YX_Net_TCPSend('3',"REPORT_TEMPER",send_buf,send_len,100,0,0,YX_Net_Send_REPORT_TEMPER_CB);
    ZDT_LOG("YX_Net_Send_REPORT_TEMPER Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

uint32 YX_Net_Send_REPORT_SMS_READ(YX_APP_T *pMe, uint16 sms_idx)
{
    uint8 send_buf[YX_DB_NETSMS_MAX_UUID_SIZE+50] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("REPORT_SMS_READ Err NoReg");
        return 0;
    }
    send_len = strlen(yx_DB_NetSms_Reclist[sms_idx].msg_id);
    ZDT_LOG("REPORT_SMS_READ,sms_idx=%d,send_len=%d",sms_idx,send_len);
    if(send_len == 0)
    {
        ZDT_LOG("REPORT_SMS_READ Err send len");
        return 0;
    }
    SCI_MEMCPY(send_buf,yx_DB_NetSms_Reclist[sms_idx].msg_id,send_len);
    send_buf[send_len++] = '@';
    send_buf[send_len++] = '4';
    send_hande = YX_Net_TCPSend('3',"REPORT_SMS_READ",send_buf,send_len,100,0,0,YX_Net_Send_REPORT_TEMPER_CB);
    ZDT_LOG("YX_Net_Send_REPORT_SMS_READ Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

static int YX_Net_Send_SYSTEM_SMS_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"SYSTEM_SMS",strlen("SYSTEM_SMS")))
            {
                ZDT_LOG("YX_Net_Send_SYSTEM_SMS_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_SYSTEM_SMS_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_SYSTEM_SMS_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

uint32 YX_Net_Send_SYSTEM_SMS(YX_APP_T *pMe, uint8* pNum,uint8 * utf8_ptr,uint16 utf8_len)
{
    uint8 send_buf[4096] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("SYSTEM_SMS Err NoReg");
        return 0;
    }
    sprintf(send_buf,"%s@",pNum);
    send_len = SCI_STRLEN(send_buf);
    send_len += YX_Func_GetDateTime(send_buf+send_len);
    send_buf[send_len++] = '@';
    send_len += YX_Func_GetSMSContent(send_buf+send_len,utf8_ptr,utf8_len);
    send_hande = YX_Net_TCPSend('3',"SYSTEM_SMS",send_buf,send_len,100,0,0,YX_Net_Send_SYSTEM_SMS_CB);
    ZDT_LOG("SYSTEM_SMS Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

uint32 YX_Net_Send_REPORT_TEMPER_Ex()
{
    YX_APP_T *pMe = &g_yx_app;
    uint8 send_buf[100] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_REPORT_TEMPER Err NoReg");
        return 0;
    }
    send_len += YX_Func_GetTemper_ex(send_buf);
    send_hande = YX_Net_TCPSend('3',"REPORT_TEMPER",send_buf,send_len,100,0,0,YX_Net_Send_REPORT_TEMPER_CB);
    ZDT_LOG("YX_Net_Send_REPORT_TEMPER Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

static int YX_Net_Send_REPORT_CROSS_BORDER_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    uint8 status = 0;
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"REPORT_CROSS_BORDER",strlen("REPORT_CROSS_BORDER")))
            {
                ZDT_LOG("YX_Net_Send_REPORT_CROSS_BORDER_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_REPORT_CROSS_BORDER_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,200);
        if(ret > 0)
        {
            status = atoi(buf);
            ZDT_LOG("status=%s,%d",buf,status);
            if(status != 0)
            {
                ZDT_LOG("YX_Net_Send_REPORT_CROSS_BORDER_CB  ERR status=%d",status);
                return ZDT_TCP_RET_ERR;
            }
        }
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_REPORT_CROSS_BORDER_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

//越界上报
/*
[864209027029083,898602E5091739001549,201807111527032,REPORT_CROS
S_BORDER,3,20180711152703,45,1@0@0E121.411783N31.178125T201807111527
03@1@3]
*/
uint32 YX_Net_Send_REPORT_CROSS_BORDER(YX_APP_T *pMe,uint8 loc_ok,uint8 is_in_fence,uint8 card_type,uint16 zone_id,double latitude,double longitude)
{
    uint8 send_buf[1024] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    uint8 type_buf[20] = {0};
    uint8 zone_buf[20] = {0};
    uint32 type_len = 0;
    uint32 zone_len = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_REPORT_CROSS_BORDER Err NoReg");
        return 0;
    }
    ZDT_LOG("YX_Net_Send_REPORT_CROSS_BORDER loc_ok=0x%x",loc_ok);
    sprintf(type_buf,"%d",card_type);
    type_len = SCI_STRLEN(type_buf);
    sprintf(zone_buf,"%d",zone_id);
    zone_len = SCI_STRLEN(zone_buf);
    
    SCI_MEMCPY(send_buf,type_buf,type_len);
    send_len += type_len;
    send_buf[send_len++] = '@';
    if(is_in_fence)
    {
        send_buf[send_len++] = '1';
    }
    else
    {
        send_buf[send_len++] = '0';
    }
    send_buf[send_len++] = '@';
    send_len += YX_Func_GetGPSInfoExt(send_buf+send_len,latitude,longitude);
    send_buf[send_len++] = '@';
    SCI_MEMCPY(send_buf+send_len,zone_buf,zone_len);
    send_len += zone_len;
    send_hande = YX_Net_TCPSend('3',"REPORT_CROSS_BORDER",send_buf,send_len,100,0,0,YX_Net_Send_REPORT_CROSS_BORDER_CB);
    ZDT_LOG("YX_Net_Send_REPORT_CROSS_BORDER Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

extern int YX_Net_Receive_SET_NORMAL_BUTTON(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen);
static int YX_Net_Send_SET_GET_NORMAL_BUTTON_Reques_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"GET_NORMAL_BUTTON",strlen("GET_NORMAL_BUTTON")))
            {
                ZDT_LOG("YX_Net_Send_SET_GET_NORMAL_BUTTON_Reques_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_SET_GET_NORMAL_BUTTON_Reques_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
	YX_Net_Receive_SET_NORMAL_BUTTON(pMe,pContent,Content_len);
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_SET_GET_NORMAL_BUTTON_Reques_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

uint32 YX_Net_Send_SET_GET_NORMAL_BUTTON_Reques(YX_APP_T *pMe)
{
	uint8 send_buf[10] = {0};
	uint32 send_len = 0;
	uint32 send_hande = 0;
	if(pMe->m_yx_is_land == FALSE)
	{
		ZDT_LOG("GET_NORMAL_BUTTON Err NoReg");
		return 0;
	}
       send_buf[0] = '1';
       send_len = 1;
	send_hande = YX_Net_TCPSend('3',"GET_NORMAL_BUTTON",send_buf,send_len,100,0,0,YX_Net_Send_SET_GET_NORMAL_BUTTON_Reques_CB);
	ZDT_LOG("GET_NORMAL_BUTTON Handle=0x%x,%s",send_hande,send_buf);
	return send_hande;
}

extern int YX_Net_Receive_SET_INCOMING_CALL(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen);
static int YX_Net_Send_SET_INCOMING_CALL_Reques_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"GET_INCOMING_CALL",strlen("GET_INCOMING_CALL")))
            {
                ZDT_LOG("YX_Net_Send_SET_INCOMING_CALL_Reques_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_SET_INCOMING_CALL_Reques_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
	YX_Net_Receive_SET_INCOMING_CALL(pMe,pContent,Content_len);
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_SET_INCOMING_CALL_Reques_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

uint32 YX_Net_Send_SET_INCOMING_CALL_Reques(YX_APP_T *pMe)
{
    uint8 send_buf[10] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_SET_INCOMING_CALL_Reques Err NoReg");
        return 0;
    }
    send_buf[0] = '1';
    send_len = 1;
    send_hande = YX_Net_TCPSend('3',"GET_INCOMING_CALL",send_buf,send_len,100,0,0,YX_Net_Send_SET_INCOMING_CALL_Reques_CB);
    ZDT_LOG("YX_Net_Send_SET_INCOMING_CALL_Reques Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}


extern int YX_Net_Receive_SET_CLASS_MODEL(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen);
static int YX_Net_Send_SET_CLASS_MODEL_Reques_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"GET_CLASS_MODEL",strlen("GET_CLASS_MODEL")))
            {
                ZDT_LOG("YX_Net_Send_SET_CLASS_MODEL_Reques_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_SET_CLASS_MODEL_Reques_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
	YX_Net_Receive_SET_CLASS_MODEL(pMe,pContent,Content_len);
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_SET_CLASS_MODEL_Reques_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

uint32 YX_Net_Send_SET_CLASS_MODEL_Reques(YX_APP_T *pMe)
{
    uint8 send_buf[10] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_SET_CLASS_MODEL_Reques Err NoReg");
        return 0;
    }
    send_buf[0] = '1';
    send_len = 1;
    send_hande = YX_Net_TCPSend('3',"GET_CLASS_MODEL",send_buf,send_len,100,0,0,YX_Net_Send_SET_CLASS_MODEL_Reques_CB);
    ZDT_LOG("YX_Net_Send_SET_CLASS_MODEL_Reques Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

static int YX_Net_Send_HDKT_REQUEST_QUESTION_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"HDKT_REQUEST_QUESTION",strlen("HDKT_REQUEST_QUESTION")))
            {
                ZDT_LOG("YX_Net_Send_HDKT_REQUEST_QUESTION_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_HDKT_REQUEST_QUESTION_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_HDKT_REQUEST_QUESTION_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

//设备请求互动课堂功能
uint32 YX_Net_Send_HDKT_REQUEST_QUESTION(YX_APP_T *pMe)
{
    uint8 send_buf[10] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("HDKT_REQUEST_QUESTION Err NoReg");
        return 0;
    }
    send_buf[send_len++] = '1';
    send_hande = YX_Net_TCPSend('3',"HDKT_REQUEST_QUESTION",send_buf,send_len,100,0,0,YX_Net_Send_HDKT_REQUEST_QUESTION_CB);
    ZDT_LOG("HDKT_REQUEST_QUESTION Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

//提交答案
uint32 YX_Net_Send_HDKT_SEND_ANSWER(YX_APP_T *pMe,uint8 job_type, uint16 job_answer)
{
    uint8 i = 0;
    uint8 send_buf[10] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    uint16 offset = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_HDKT_SEND_ANSWER Err NoReg");
        return 0;
    }
    if(job_type == 31)
    {  // 判断
        send_buf[send_len++] = '3';
        send_buf[send_len++] = '1';
        send_buf[send_len++] = '@';
        if(job_answer > 0)
        {
            send_buf[send_len++] = '0';
            send_buf[send_len++] = '1';
        }
        else
        {
            send_buf[send_len++] = '0';
            send_buf[send_len++] = '0';
        }
    }
    else if(job_type == 32)
    {  // 单选
        send_buf[send_len++] = '3';
        send_buf[send_len++] = '2';
        send_buf[send_len++] = '@';
        send_buf[send_len++] = '4';
        send_buf[send_len++] = '0'+job_answer;
    }
    else if(job_type == 33)
    {  //多选
        send_buf[send_len++] = '3';
        send_buf[send_len++] = '3';
        send_buf[send_len++] = '@';
        for(i = 0; i < 8; i++)
        {
            offset = (0x01 << i);
            if((job_answer&offset) > 0)
            {
                send_buf[send_len++] = '4';
                send_buf[send_len++] = '1'+i;
            }
        }
    }
    send_hande = YX_Net_TCPSend('3',"HDKT_SEND_ANSWER",send_buf,send_len,100,0,0,PNULL);
    ZDT_LOG("YX_Net_Send_HDKT_SEND_ANSWER Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

static BOOLEAN YX_JOB_Link_AddData(YX_APP_T *pMe,uint8 answer_type,uint8 job_type)
{
    YX_JOB_LINK_NODE_T  * p1 = NULL;
    YX_JOB_LINK_NODE_T  * p2 = NULL;
    
    p1=(YX_JOB_LINK_NODE_T *)SCI_ALLOC_APPZ(sizeof(YX_JOB_LINK_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    
    p1->data.answer_type = answer_type;
    p1->data.job_type = job_type;
    
    p1->next_ptr = NULL;

    if(pMe->m_yx_job_link_head == NULL)
    {
        pMe->m_yx_job_link_head = p1;
    }
    else
    {
        p2 = pMe->m_yx_job_link_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  YX_JOB_Link_Get(YX_APP_T *pMe,YX_JOB_LINK_DATA_T * pData)
{
    YX_JOB_LINK_NODE_T  * p1 = NULL;
    
    p1 = pMe->m_yx_job_link_head;
    
    if(p1 != NULL)
    {
      *pData = p1->data;
        pMe->m_yx_job_link_head = p1->next_ptr;
        SCI_FREE(p1);
        return TRUE;
    }
        
    return FALSE;
}

static BOOLEAN  YX_JOB_Link_TypeExsit(YX_APP_T *pMe,uint8 job_type)
{
    YX_JOB_LINK_NODE_T  * p1 = NULL;
    YX_JOB_LINK_NODE_T  * p2 = NULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = pMe->m_yx_job_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data.job_type == job_type)
        {
            res = TRUE;
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
    return res;
}

static BOOLEAN  YX_JOB_Link_GetByType(YX_APP_T *pMe,YX_JOB_LINK_DATA_T * pData,uint8 job_type)
{
    YX_JOB_LINK_NODE_T  * p1 = NULL;
    YX_JOB_LINK_NODE_T  * p2 = NULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = pMe->m_yx_job_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data.job_type == job_type)
        {
            *pData = p1->data;
            if(p1 == pMe->m_yx_job_link_head)
            {
                pMe->m_yx_job_link_head = p1->next_ptr;
            }
            else
            {
                p2->next_ptr = p1->next_ptr;
            }
            SCI_FREE(p1);
            res = TRUE;
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
        
    return res;
}

BOOLEAN  YX_JOB_Link_DelAll(YX_APP_T *pMe)
{
    YX_JOB_LINK_NODE_T  * p1 = NULL;
    
    while(pMe->m_yx_job_link_head != NULL)
    {
        p1 = pMe->m_yx_job_link_head;
        pMe->m_yx_job_link_head = p1->next_ptr;
        SCI_FREE(p1);
    }
    return TRUE;
}

extern int YX_Net_Receive_VACC(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen);
static int YX_Net_Send_VACC_Request_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"GET_VACC",strlen("GET_VACC")))
            {
                ZDT_LOG("YX_Net_Send_VACC_Request_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_VACC_Request_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
	YX_Net_Receive_VACC(pMe,pContent,Content_len);
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_VACC_Request_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

extern int YX_Net_Receive_NUCLEIC_ACID(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen);
static int YX_Net_Send_NUCLEICACID_Request_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"GET_NUCLEIC_ACID",strlen("GET_NUCLEIC_ACID")))
            {
                ZDT_LOG("YX_Net_Send_NUCLEICACID_Request_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_NUCLEICACID_Request_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
	YX_Net_Receive_NUCLEIC_ACID(pMe,pContent,Content_len);
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_NUCLEICACID_Request_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

extern int YX_Net_Receive_Travel_Card(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen);
static int YX_Net_Send_TravelCard_Request_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"GET_TRAVEL_CARD",strlen("GET_TRAVEL_CARD")))
            {
                ZDT_LOG("YX_Net_Send_TravelCard_Request_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_TravelCard_Request_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
	YX_Net_Receive_Travel_Card(pMe,pContent,Content_len);
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_TravelCard_Request_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}
extern int YX_Net_Receive_Health_Code(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen);
static int YX_Net_Send_HealthCode_Request_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    int ret = 0;
    uint8 * pStr = pRcv;
    uint32 len = Rcv_len;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            if(0 != strncmp(mark_str,"GET_HEALTH_CODE",strlen("GET_HEALTH_CODE")))
            {
                ZDT_LOG("YX_Net_Send_HealthCode_Request_CB  ERR mark_str=%s",mark_str);
                return ZDT_TCP_RET_ERR;
            }
        }
        else
        {
            ZDT_LOG("YX_Net_Send_HealthCode_Request_CB  ERR mark_str");
            return ZDT_TCP_RET_ERR;
        }
        
        pContent = pRcv+Rcv_len-cont_len-1;
        Content_len = cont_len;
	YX_Net_Receive_Health_Code(pMe,pContent,Content_len);
        return ZDT_TCP_RET_OK;
    }

    ZDT_LOG("YX_Net_Send_HealthCode_Request_CB  ERR cont_len=%d",cont_len);
    return ZDT_TCP_RET_ERR;
}

uint32 YX_Net_Send_GETVACC_Request(YX_APP_T *pMe)
{
    uint8 send_buf[1024] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_GETVACC_Request Err NoReg");
        return 0;
    }
    send_buf[0] = '1';
    send_len = 1;
    send_hande = YX_Net_TCPSend('3',"GET_VACC",send_buf,send_len,100,0,0,YX_Net_Send_VACC_Request_CB);
    ZDT_LOG("YX_Net_Send_GETVACC_Request Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

uint32 YX_Net_Send_NUCLEICACID_Request(YX_APP_T *pMe)
{
    uint8 send_buf[1024] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_NUCLEICACID_Request Err NoReg");
        return 0;
    }
    send_buf[0] = '1';
    send_len = 1;
    send_hande = YX_Net_TCPSend('3',"GET_NUCLEIC_ACID",send_buf,send_len,100,0,0,YX_Net_Send_NUCLEICACID_Request_CB);
    ZDT_LOG("YX_Net_Send_NUCLEICACID_Request Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}
uint32 YX_Net_Send_TravelCard_Request(YX_APP_T *pMe)
{
    uint8 send_buf[1024] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_TravelCard_Request Err NoReg");
        return 0;
    }
    send_buf[0] = '1';
    send_len = 1;
    send_hande = YX_Net_TCPSend('3',"GET_TRAVEL_CARD",send_buf,send_len,100,0,0,YX_Net_Send_TravelCard_Request_CB);
    ZDT_LOG("YX_Net_Send_TravelCard_Request Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}
uint32 YX_Net_Send_HealthCode_Request(YX_APP_T *pMe)
{
    uint8 send_buf[1024] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Send_HealthCode_Request Err NoReg");
        return 0;
    }
    send_buf[0] = '1';
    send_len = 1;
    send_hande = YX_Net_TCPSend('3',"GET_HEALTH_CODE",send_buf,send_len,100,0,0,YX_Net_Send_HealthCode_Request_CB);
    ZDT_LOG("YX_Net_Send_HealthCode_Request Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}


//接收
int YX_Net_Receive_VACC(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[1001] = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;
    uint8 * p1 = NULL;
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_VACC Err ContentLen");
        return 0;
    }
	
    ret = YX_Func_GetNextPara(&str, &len,buf,10);
    if(ret > 0) //新冠疫苗状态
    {
        yx_health_msg_Rec.vacc_status = atoi(buf);
        ZDT_LOG("YX_Net_Receive_VACC vacc_status=%d",yx_health_msg_Rec.vacc_status);
    }
    ZDT_LOG("YX_Net_Receive_VACC str=%s",str);
    p1=strstr(str,(char*)"#");
    p1+=1;
    SCI_MEMCPY(yx_health_msg_Rec.vacc_conut,p1,1);
    p1+=2;
    SCI_MEMCPY(yx_health_msg_Rec.vacc_newest_time,p1,10);

    ZDT_LOG("yx_health_msg_Rec.vacc_conut=%s,vacc_newest_time=%s",yx_health_msg_Rec.vacc_conut,yx_health_msg_Rec.vacc_newest_time);
    ZDT_LOG("YX_Net_Receive_VACC Len = %d,str=%s",ContentLen,str);
    
    return 0;
}

int YX_Net_Receive_NUCLEIC_ACID(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[1001] = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_NUCLEIC_ACID Err ContentLen");
        return 0;
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,10);
    if(ret > 0) //核酸状态
    {
        yx_health_msg_Rec.nucleic_status = atoi(buf);
        ZDT_LOG("nucleic_status=%d",yx_health_msg_Rec.nucleic_status);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,HTTP_HEALTH_MSG_MAX_LEN);
    if(ret > 0) //核酸结果
    {
    	ZDT_LOG("yx_health_msg_Rec.buf=%s",buf);
	if(0 == strncmp( (char *)buf, "e998b4e680a7", strlen("e998b4e680a7") ))
	{
		yx_health_msg_Rec.nucleic_feminine=1;  //阴性
	}else{
		yx_health_msg_Rec.nucleic_feminine=0;
	}
	ZDT_LOG("yx_health_msg_Rec.nucleic_feminine=%d",yx_health_msg_Rec.nucleic_feminine);
    	yx_health_msg_Rec.nucleic_report_len= ZDT_Str16_to_Int(buf,ret, yx_health_msg_Rec.nucleic_report);
	ZDT_LOG("yx_health_msg_Rec.nucleic_report_len=%d,nucleic_report=%s",yx_health_msg_Rec.nucleic_report_len,yx_health_msg_Rec.nucleic_report);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,HTTP_HEALTH_MSG_MAX_LEN);
    if(ret > 0) //检测时间
    {
        SCI_MEMCPY(yx_health_msg_Rec.detect_time,buf,ret);
	ZDT_LOG("yx_health_msg_Rec.detect_time = %s,str=%s",yx_health_msg_Rec.detect_time,buf);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,HTTP_HEALTH_QRCODE_MAX_LEN);
    if(ret > 0) //有效时间
    {
        yx_health_msg_Rec.effective_time = atoi(buf);
	ZDT_LOG("yx_health_msg_Rec.effective_time=%d,buf=%s",yx_health_msg_Rec.effective_time,buf);
    }
    ZDT_LOG("YX_Net_Receive_NUCLEIC_ACID Len = %d,str=%s",ContentLen,str);
    return 0;
}

int YX_Net_Receive_Travel_Card(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[1001] = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_Travel_Card Err ContentLen");
        return 0;
    }	
    ZDT_LOG("YX_Net_Receive_Travel_Card Len = %d,str=%s",ContentLen,str);

    ret = YX_Func_GetNextPara(&str, &len,buf,10);//行程卡状态
    if(ret > 0)
    {
        yx_health_msg_Rec.travel_status = atoi(buf);
        ZDT_LOG("travel_status=%d",yx_health_msg_Rec.travel_status);
    }
    for(i=0; i<TRAVEL_CITY_MAX_NUM;i++)
    {
    	ret = YX_Func_GetNextPara(&str, &len,buf,HTTP_HEALTH_MSG_MAX_LEN);//
	if(ret > 0) 
       	{
	      yx_health_msg_Rec.travel_city_content_len[i] = ZDT_Str16_to_Int(buf,ret,yx_health_msg_Rec.travel_city_content[i]);
	      ZDT_LOG("travel_city_content_len[%d] =%d,buf=%s",i,yx_health_msg_Rec.travel_city_content_len[i],buf);
	}else{
		break;
	}
    }
    return 0;
}

int YX_Net_Receive_Health_Code(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[1001] = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_Health_Code Err ContentLen");
        return 0;
    }
    ZDT_LOG("YX_Net_Receive_Health_Code Len = %d,str=%s",ContentLen,str);
	
    ret = YX_Func_GetNextPara(&str, &len,buf,10);//健康码状态
    if(ret > 0)
    {
        yx_health_msg_Rec.code_status = atoi(buf);
        ZDT_LOG("code_status=%d",yx_health_msg_Rec.code_status);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,HTTP_HEALTH_QRCODE_MAX_LEN);
    if(ret > 0) //    二维码地址
    {
        ZDT_LOG("yx_health_msg_Rec.code_addr");
    }
    
    ret = YX_Func_GetNextPara(&str, &len,buf,HTTP_HEALTH_QRCODE_MAX_LEN);
    if(ret > 0)  //base4 值
    {
	ZDT_LOG("yx_health_msg_Rec.code_base4");
    }
	
    ZDT_LOG("YX_Net_Receive_Health_Cod:str=%s",str);
    ret = YX_Func_GetNextPara(&str, &len,buf,HTTP_HEALTH_QRCODE_MAX_LEN);
    if(ret > 0)  //二维码内容
    {
    	SCI_MEMCPY(yx_health_msg_Rec.code_content,buf,ret);
	ZDT_LOG("YX_Net_Receive_Health_Cod:len=%d,buf=%s,code_content=%s",len,buf,yx_health_msg_Rec.code_content);
    }
    return 0;
}

uint32 YX_Net_Respond_SET_SERVER_INFO(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SET_SERVER_INFO",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SET_SERVER_INFO Handle=0x%x",send_hande);
    return send_hande;
}

static uint16  YX_SERVER_INFO_DeleteStrHeadArea(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len, //[IN]  源String的长度，byte数
                             uint8*  out_str
                             )
{
    uint16 i = 0;
    uint16 head_len = 0;
    char head_str[20] = {0};
    
    if(src_ptr != PNULL && src_len > 7)
    {
        SCI_MEMCPY(head_str,src_ptr,8);
        YX_Func_strlwr(head_str);
        if(strncmp(head_str, "http://", 7) == 0)
        {
            head_len = 7;
        }
        else if(strncmp(head_str, "https://", 8) == 0)
        {
            head_len = 8;
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

//[868560040058335,89860317947552295702,20220224190011223,SET_SERVER_INFO,1,20220224190011,20,171.220.242.225@9003]
int YX_Net_Receive_SET_SERVER_INFO(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[MAX_YX_IP_DOMAIN_SIZE+1] = {0};
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_SET_SERVER_INFO Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Receive_SET_SERVER_INFO ContentLen = %d",ContentLen);
    
    ret = YX_Func_GetNextPara(&str, &len,buf,MAX_YX_IP_DOMAIN_SIZE);
    if(ret > 0)
    {
        SCI_MEMSET(yx_DB_Set_Rec.ip_domain,0,MAX_YX_IP_DOMAIN_SIZE+1);
        YX_SERVER_INFO_DeleteStrHeadArea(buf,ret,yx_DB_Set_Rec.ip_domain);
        ZDT_LOG("ip=%s, %s",buf,yx_DB_Set_Rec.ip_domain);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,MAX_YX_IP_DOMAIN_SIZE);
    if(ret > 0)
    {
        yx_DB_Set_Rec.ip_port = atoi(buf);
        ZDT_LOG("port=%s, ip_port=%d",buf,yx_DB_Set_Rec.ip_port);
        YX_DB_SET_ListModify();
    }
    
    ZDT_LOG("YX_Net_Receive_SET_SERVER_INFO ret =%d, %s:%d",ret,yx_DB_Set_Rec.ip_domain,yx_DB_Set_Rec.ip_port);
    return 0;
}

uint32 YX_Net_Respond_SET_INCOMING_CALL(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SET_INCOMING_CALL",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SET_INCOMING_CALL Handle=0x%x",send_hande);
    return send_hande;
}

//[868560040058335,89860317947552295702,20220224190011223,SET_INCOMING_CALL,1,20220224190011,20,171.220.242.225@9003]
int YX_Net_Handle_SET_INCOMING_CALL_Del(uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char buf[1001] = {0};
    uint16 i = 0;
    uint8 * str = pData;
    uint32 len = DataLen;
    
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SET_INCOMING_CALL_Del Err ContentLen");
        return 0;
    }
    
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        ret = YX_Func_GetNextParaF(&str, &len,buf,1000);
        if(ret > 0)
        {
            ZDT_LOG("Del white_num %s,",buf);
            YX_DB_WHITE_DeleteNumber(buf);
        }
    }
    return 0;
}

int YX_Net_Handle_SET_INCOMING_CALL_Add(uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char buf[1001] = {0};
    char item_buf[501] = {0};
    char timer_str[11] = {0};
    uint16 i = 0;
    uint8 * str = pData;
    uint32 len = DataLen;
    uint8 * pItem = PNULL;
    uint32 ItemLen = 0;
    uint8 * pTime = NULL;
    uint32 time_len = 0;
    YX_DB_WHITE_Type cur_data = {0};
    uint8 start_hour = 0;
    uint8 start_min = 0;
    uint8 end_hour = 0;
    uint8 end_min = 0;
    
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SET_INCOMING_CALL_Del Err ContentLen");
        return 0;
    }

    ZDT_LOG("YX_Net_Handle_SET_INCOMING_CALL_Add %s",pData);
    
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        ret = YX_Func_GetNextParaF(&str, &len,buf,1000);
        if(ret > 0)
        {
            pItem = buf;
            ItemLen = ret;
            SCI_MEMSET(&cur_data,0,sizeof(YX_DB_WHITE_Type));
            //呢称
            ret = YX_Func_GetNextParaD(&pItem , &ItemLen,item_buf,500);
            if(ret > 0)
            {
                if(ret > YX_DB_WHITE_MAX_NAME_SIZE)
                {
                    ret = YX_DB_WHITE_MAX_NAME_SIZE;
                }
                ZDT_Str16_to_Int(item_buf,ret,cur_data.white_name);
                //SCI_MEMCPY(cur_data.white_name,item_buf,ret);
            }
            ret = YX_Func_GetNextParaD(&pItem , &ItemLen,item_buf,500);
            if(ret > 0)
            {
                if(ret > YX_DB_WHITE_MAX_NUMBER_SIZE)
                {
                    ret = YX_DB_WHITE_MAX_NUMBER_SIZE;
                }
                SCI_MEMCPY(cur_data.white_num,item_buf,ret);
            }
            ret = YX_Func_GetNextParaJ(&pItem , &ItemLen,item_buf,500);
            if(ret > 0)
            {
                start_hour = 0;
                start_min = 0;
                end_hour = 0;
                end_min = 0;
                pTime = item_buf;
                time_len = ret;
                SCI_MEMSET(timer_str,0,11);
                ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
                if(ret > 0)
                {
                    start_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
                    start_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
                    ZDT_LOG("start_hour=%d,start_min=%d",start_hour,start_min);
                }

                SCI_MEMSET(timer_str,0,11);
                ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
                if(ret > 0)
                {
                    end_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
                    end_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
                    ZDT_LOG("end_hour=%d,end_min=%d",end_hour,end_min);
                }
                
                cur_data.time1_start = (start_hour * 3600) + (start_min * 60);
                cur_data.time1_end = (end_hour * 3600) + (end_min * 60);
                ZDT_LOG("time1_start=%d,time1_end=%d",cur_data.time1_start,cur_data.time1_end);
            }
            
            ret = YX_Func_GetNextParaJ(&pItem , &ItemLen,item_buf,500);
            if(ret > 0)
            {
                start_hour = 0;
                start_min = 0;
                end_hour = 0;
                end_min = 0;
                pTime = buf;
                time_len = ret;
                SCI_MEMSET(timer_str,0,11);
                ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
                if(ret > 0)
                {
                    start_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
                    start_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
                    ZDT_LOG("start_hour=%d,start_min=%d",start_hour,start_min);
                }

                SCI_MEMSET(timer_str,0,11);
                ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
                if(ret > 0)
                {
                    end_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
                    end_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
                    ZDT_LOG("end_hour=%d,end_min=%d",end_hour,end_min);
                }
                
                cur_data.time2_start = (start_hour * 3600) + (start_min * 60);
                cur_data.time2_end = (end_hour * 3600) + (end_min * 60);
                ZDT_LOG("time2_start=%d,time2_end=%d",cur_data.time2_start,cur_data.time2_end);
            }
            
            ZDT_LOG("Add white_num %s,time1=%d-%d,time2=%d-%d",cur_data.white_num,cur_data.time1_start,cur_data.time1_end,cur_data.time2_start,cur_data.time2_end);
            YX_DB_WHITE_AddNumber(cur_data.white_num,&cur_data);
        }
        else
        {
            break;
        }
    }
    return 0;
}

int YX_Net_Handle_SET_INCOMING_CALL_Week(uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char buf[11] = {0};
    uint16 i = 0;
    uint8 week_type = 0;
    uint8 cur_type = 0;
    uint8 * str = pData;
    uint32 len = DataLen;
    
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SET_INCOMING_CALL_Week Err ContentLen");
        return 0;
    }
    
    for(i = 0; i < 7; i++)
    {
        ret = YX_Func_GetNextParaJ(&str, &len, buf, 10);
        if(ret > 0)
        {
            cur_type = atoi(buf);
            if(cur_type > 6)
            {
                cur_type = 0;
            }
            week_type = week_type | (0x01 << cur_type);
        }
    }
    yx_DB_Set_Rec.in_call_cycle = week_type;
    ZDT_LOG("week_type=0x%x",yx_DB_Set_Rec.in_call_cycle);
    return 0;
}

int YX_Net_Receive_SET_INCOMING_CALL(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[1001] = {0};
    uint16 i = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_SET_INCOMING_CALL Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Receive_SET_INCOMING_CALL ContentLen = %d",ContentLen);
    for(i = 0; i < 4; i++)
    {
        ret = YX_Func_GetNextPara(&str, &len,buf,1000);
        if(ret > 0)
        {
            if(i == 0)
            {
                // 删除号码
                if(ret == 1 && buf[0] == '0')
                {
                }
                else
                {
                    YX_Net_Handle_SET_INCOMING_CALL_Del(buf,ret);
                }
            }
            else if(i == 1)
            {
                // 添加号码和时段
                if(ret == 1 && buf[0] == '0')
                {
                }
                else
                {
                    YX_Net_Handle_SET_INCOMING_CALL_Add(buf,ret);
                }
            }
            else if(i == 2)
            {
                // 呼入限制1、无限制2、限制白名单以外的号码呼入3、限制所有号码呼入
                yx_DB_Set_Rec.in_call_mode = atoi(buf);
                ZDT_LOG("in_call_mode=%d",yx_DB_Set_Rec.in_call_mode);
            }
            else if(i == 3)
            {
                YX_Net_Handle_SET_INCOMING_CALL_Week(buf,ret);
            }
        }
    }
    
    YX_DB_SET_ListModify();
    YX_DB_WHITE_ListSaveBuf();
    ZDT_LOG("YX_Net_Receive_SET_INCOMING_CALL Over");
    return 0;
}

uint32 YX_Net_Respond_SET_NORMAL_BUTTON(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SET_NORMAL_BUTTON",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SET_NORMAL_BUTTON Handle=0x%x",send_hande);
    return send_hande;
}

int YX_Net_Handle_SET_NORMAL_BUTTON(uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char buf[101] = {0};
    char name_buf[101] = {0};
    char num_buf[101] = {0};
    uint16 i = 0;
    uint16 cur_index = 0;
    uint8 * str = pData;
    uint32 len = DataLen;
    YX_DB_SOS_Type sos_data = {0};
    YX_DB_SPD_Type spd_data = {0};
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SET_NORMAL_BUTTON Err ContentLen");
        return 0;
    }
    ret = YX_Func_GetNextParaD(&str, &len, buf, 100);
    if(ret > 0)
    {
        cur_index = atoi(buf);
    }
    ret = YX_Func_GetNextParaJ(&str, &len, num_buf, YX_DB_SOS_MAX_NUMBER_SIZE);
    if(ret > 0)
    {
        if(cur_index == 0)
        {
            SCI_MEMCPY(sos_data.sos_num,num_buf,ret);
        }
        else
        {
            SCI_MEMCPY(spd_data.spd_num,num_buf,ret);
        }
        ret = YX_Func_GetNextParaF(&str, &len, name_buf, YX_DB_SOS_MAX_NAME_SIZE);
        if(ret > 0)
        {
            if(cur_index == 0)
            {
                //SCI_MEMCPY(sos_data.sos_name,name_buf,ret);
		  ZDT_Str16_to_Int(name_buf,ret,sos_data.sos_name);		
            }
            else
            {
                //SCI_MEMCPY(spd_data.spd_name,name_buf,ret);
                ZDT_Str16_to_Int(name_buf,ret,spd_data.spd_name);
            }
        }
        
        if(cur_index == 0)
        {
            ZDT_LOG("add cur_index=%d,sos_name=%s,num=%s",cur_index,sos_data.sos_name,sos_data.sos_num);
            YX_DB_SOS_ListModifyBuf(cur_index,&sos_data);
        }
        else
        {
            ZDT_LOG("add cur_index=%d,sos_name=%s,num=%s",cur_index,spd_data.spd_name,spd_data.spd_num);
            YX_DB_SPD_ListModifyBuf(cur_index-1,&spd_data);
        }
    }
    else
    {
        ZDT_LOG("del cur_index=%d",cur_index);
        if(cur_index == 0)
        {
            YX_DB_SOS_ListDelBuf(0);
        }
        else
        {
            YX_DB_SPD_ListDelBuf(cur_index-1);
        }
    }
    return 0;
}

int YX_Net_Receive_SET_NORMAL_BUTTON(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[1001] = {0};
    uint16 i = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_SET_NORMAL_BUTTON Err ContentLen");
        return 0;
    }
    YX_DB_SOS_ListDelAllBuf();
    YX_DB_SPD_ListDelAllBuf();
    ZDT_LOG("YX_Net_Receive_SET_NORMAL_BUTTON ContentLen = %d",ContentLen);
    for(i = 0; i < 10; i++)
    {
        ret = YX_Func_GetNextParaF(&str, &len,buf,1000);
        if(ret > 0)
        {
            YX_Net_Handle_SET_NORMAL_BUTTON(buf,ret);
        }
    }
    
    YX_DB_SOS_ListSaveBuf();
    YX_DB_SPD_ListSaveBuf();
    ZDT_LOG("YX_Net_Receive_SET_NORMAL_BUTTON Over");
    return 0;
}

uint32 YX_Net_Respond_LOCATION_INFO_GET(YX_APP_T *pMe, uint8 loc_ok,char * water_str)
{
    uint8 send_buf[2048] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("YX_Net_Respond_LOCATION_INFO_GET Err NoReg");
        return 0;
    }
    ZDT_LOG("YX_Net_Respond_LOCATION_INFO_GET loc_ok=0x%x",loc_ok);
    send_len += YX_Func_GetGPSInfo(send_buf+send_len,loc_ok);
    send_buf[send_len++] = '@';
    send_len += YX_Func_GetSSInfo(send_buf+send_len);
    send_buf[send_len++] = '@';
    send_len += YX_Func_GetWIFIInfo(send_buf+send_len,loc_ok);
    send_hande = YX_Net_TCPRespond('2',water_str,"LOCATION_INFO_GET",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_LOCATION_INFO_GET Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

int YX_Net_Receive_LOCATION_INFO_GET(YX_APP_T *pMe,char * water_str)
{
    ZDT_LOG("YX_Net_Receive_LOCATION_INFO_GET Start");
#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 
    YX_LOCTION_Start(pMe,YX_LOCTION_TYPE_CR,water_str,18);    
#endif
    return 0;
}

uint32 YX_Net_Respond_FREQUENCY_LOCATION_SET(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"FREQUENCY_LOCATION_SET",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_FREQUENCY_LOCATION_SET Handle=0x%x",send_hande);
    return send_hande;
}

int YX_Net_Receive_FREQUENCY_LOCATION_SET(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char min_buf[31] = {0};
    uint8 * str = pContent;
    uint32 len = ContentLen;
    uint16 min_interval = 5;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_FREQUENCY_LOCATION_SET Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Receive_FREQUENCY_LOCATION_SET ContentLen = %d",ContentLen);
    
    ret = YX_Func_GetNextPara(&str, &len,min_buf,30);
    if(ret > 0)
    {
        min_interval = atoi(min_buf);
        ZDT_LOG("time_min=%s,%d",min_buf,min_interval);
    }
    
    if(min_interval < 3)
    {
        min_interval = 3;
    }
    
    yx_DB_Set_Rec.time_interval = min_interval*60;
    YX_DB_SET_ListModify();
    YX_LocReport_TimeStop(pMe);
    YX_LocReport_TimeSet(yx_DB_Set_Rec.time_interval);
    if(FALSE == YX_DB_TIMER_IsValid_Sleep())
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_LOCREPORT,NULL,0);
    }
    YX_LocReport_TimeStart(pMe,g_yx_loc_timer_interval);
    ZDT_LOG("YX_Net_Receive_FREQUENCY_LOCATION_SET time_interval =%d s",yx_DB_Set_Rec.time_interval);
    return 0;
}

uint32 YX_Net_Respond_REMOTE_OPERATE_TERMINAL(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"REMOTE_OPERATE_TERMINAL",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_REMOTE_OPERATE_TERMINAL Handle=0x%x",send_hande);
    return send_hande;
}

BOOLEAN YX_Net_FactoryHaveIP(YX_APP_T *pMe)
{
    BOOLEAN res = FALSE;
    char imei_str[16] = {0};
    char ip_str_bak[MAX_YX_IP_DOMAIN_SIZE+1] = {0};
    uint16 ip_port_bak = 0;
    ZDT_LOG("YX_Net_FactoryHaveIP");
    ip_port_bak = yx_DB_Set_Rec.ip_port;
    strcpy(ip_str_bak,yx_DB_Set_Rec.ip_domain);
    strcpy(imei_str,yx_DB_Set_Rec.imei);
    ZDT_DB_DelAll();
    YX_DB_DelAll(pMe);
    yx_DB_Set_Rec.ip_port = ip_port_bak;
    strcpy(yx_DB_Set_Rec.ip_domain,ip_str_bak);
    strcpy(yx_DB_Set_Rec.imei,imei_str);
    YX_DB_SET_ListModify();
    MMIAPICL_ClearCalllogData();
    YX_LocReport_TimeStop(pMe);
    YX_LocReport_TimeSet(yx_DB_Set_Rec.time_interval);
    return res;
}

int YX_Net_Receive_REMOTE_OPERATE_TERMINAL(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[31] = {0};
    uint8 * str = pContent;
    uint32 len = ContentLen;
    uint8 need_reset = 0;
    uint8 need_fatory = 0;
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_REMOTE_OPERATE_TERMINAL Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Receive_REMOTE_OPERATE_TERMINAL ContentLen = %d",ContentLen);
    
    ret = YX_Func_GetNextPara(&str, &len,buf,30);
    if(ret > 0)
    {
        need_reset = atoi(buf);
        ZDT_LOG("need_reset = %d",need_reset);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,30);
    if(ret > 0)
    {
        need_fatory = atoi(buf);
        ZDT_LOG("need_fatory = %d",need_fatory);
    }
    if(need_fatory)
    {
        MMISET_CleanUserData();//delete default nv of vol ...
        YX_Net_FactoryHaveIP(pMe);
        if(need_reset == 0)
        {
            g_yx_is_handle_factory = TRUE;
        }
    }
    ZDT_LOG("YX_Net_Receive_REMOTE_OPERATE_TERMINAL ret =%d, %s:%d",ret,yx_DB_Set_Rec.ip_domain,yx_DB_Set_Rec.ip_port);
    //if(need_reset)
    {
        g_yx_is_need_reset = TRUE;
    }
    return 0;
}

uint32 YX_Net_Respond_SET_HEARTBEAT(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SET_HEARTBEAT",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SET_HEARTBEAT Handle=0x%x",send_hande);
    return send_hande;
}

int YX_Net_Receive_SET_HEARTBEAT(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char min_buf[31] = {0};
    uint8 * str = pContent;
    uint32 len = ContentLen;
    uint16 min_interval = 5;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_SET_HEARTBEAT Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Receive_SET_HEARTBEAT ContentLen = %d",ContentLen);
    
    ret = YX_Func_GetNextPara(&str, &len,min_buf,30);
    if(ret > 0)
    {
        min_interval = atoi(min_buf);
        ZDT_LOG("time_min=%s,%d",min_buf,min_interval);
    }
    if(min_interval < 5)
    {
        min_interval = 5;
    }
    yx_DB_Set_Rec.heart_interval = min_interval*60;
    g_yx_cur_heart_time = yx_DB_Set_Rec.heart_interval;
    YX_DB_SET_ListModify();
    YX_LocReport_TimeStop(pMe);
    YX_LocReport_TimeSet(yx_DB_Set_Rec.time_interval);
    if(FALSE == YX_DB_TIMER_IsValid_Sleep())
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_LOCREPORT,NULL,0);
    }
    YX_LocReport_TimeStart(pMe,g_yx_loc_timer_interval);
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_HEART,NULL,0);
    ZDT_LOG("YX_Net_Receive_SET_HEARTBEAT heart_interval =%d s",yx_DB_Set_Rec.heart_interval);
    return 0;
}

uint32 YX_Net_Respond_SET_MODEL(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SET_MODEL",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SET_MODEL Handle=0x%x",send_hande);
    return send_hande;
}

int YX_Net_Receive_SET_MODEL(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[31] = {0};
    uint8 * str = pContent;
    uint32 len = ContentLen;
    uint16 min_interval = 5;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_SET_MODEL Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Receive_SET_MODEL ContentLen = %d",ContentLen);
    
    ret = YX_Func_GetNextPara(&str, &len,buf,30);
    if(ret > 0)
    {
        yx_DB_Set_Rec.env_mute = atoi(buf);
        ZDT_LOG("env_mute=%s,%d",buf,yx_DB_Set_Rec.env_mute);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,30);
    if(ret > 0)
    {
        yx_DB_Set_Rec.env_ring = atoi(buf);
        ZDT_LOG("env_ring=%s,%d",buf,yx_DB_Set_Rec.env_ring);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,30);
    if(ret > 0)
    {
        yx_DB_Set_Rec.mt_call_limit = atoi(buf);
        ZDT_LOG("mt_call_limit=%s,%d",buf,yx_DB_Set_Rec.mt_call_limit);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,30);
    if(ret > 0)
    {
        yx_DB_Set_Rec.mo_call_limit = atoi(buf);
        ZDT_LOG("mo_call_limit=%s,%d",buf,yx_DB_Set_Rec.mo_call_limit);
    }
    
    YX_DB_SET_ListModify();
    ZDT_LOG("YX_Net_Receive_SET_MODEL env_mute=%d,env_ring=%d,mo_call_limit=%d,mt_call_limit=%d",yx_DB_Set_Rec.env_mute,yx_DB_Set_Rec.env_ring,yx_DB_Set_Rec.mo_call_limit,yx_DB_Set_Rec.mt_call_limit);
    if(yx_DB_Set_Rec.env_mute)
    {
    	   MMIAPISET_SetCallRingType(MMISET_CALL_VIBRA, MMIAPISET_GetActiveSim());
    }else{
    	   MMIAPISET_SetCallRingType(MMISET_CALL_VIBRA_AND_RING, MMIAPISET_GetActiveSim());
    }
    return 0;
}

uint32 YX_Net_Respond_SET_CLASS_MODEL(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SET_CLASS_MODEL",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SET_CLASS_MODEL Handle=0x%x",send_hande);
    return send_hande;
}

uint8 YX_Net_Handle_SET_CLASS_MODEL_Week(uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char buf[11] = {0};
    uint16 i = 0;
    uint8 week_type = 0;
    uint8 cur_type = 0;
    uint8 * str = pData;
    uint32 len = DataLen;
    
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SET_CLASS_MODEL_Week Err ContentLen");
        return 0;
    }
    
    for(i = 0; i < 7; i++)
    {
        ret = YX_Func_GetNextParaJ(&str, &len, buf, 10);
        if(ret > 0)
        {
            cur_type = atoi(buf);
            if(cur_type > 6)
            {
                cur_type = 0;
            }
            cur_type++;
            week_type = week_type | (0x01 << cur_type);
        }
    }
    ZDT_LOG("week_type=0x%x",week_type);
    return week_type;
}

// 序号=开始时间-结束时间！周期!是否生效
int YX_Net_Handle_SET_CLASS_MODEL_Item(uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char buf[101] = {0};
    char timer_str[11] = {0};
    uint16 i = 0;
    uint16 cur_index = 0;
    uint8 * str = pData;
    uint32 len = DataLen;
    uint8 * pTime = NULL;
    uint32 time_len = 0;
    uint8 start_hour = 0;
    uint8 start_min = 0;
    uint8 end_hour = 0;
    uint8 end_min = 0;
    uint32 time_start = 0;
    uint32 time_end = 0;
    uint8 week_type = 0;
    uint8 cur_type = 0;
    uint8 is_valid = 0;
    
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SET_CLASS_MODEL_Item Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Handle_SET_CLASS_MODEL_Item %s",pData);
    
    //序号
    ret = YX_Func_GetNextParaD(&str, &len, buf, 100);
    if(ret > 0)
    {
        cur_index = atoi(buf);
        ZDT_LOG("cur_index=%d,%s",cur_index,buf);
    }
    if(cur_index > 0)
    {
        cur_index = cur_index + YX_TIME_TYPE_CLASS_1 - 1;
    }
    else
    {
        cur_index = YX_TIME_TYPE_CLASS_1;
    }

    //时间
    ret = YX_Func_GetNextParaF(&str, &len, buf, 100);
    if(ret > 0)
    {
        pTime = buf;
        time_len = ret;
        SCI_MEMSET(timer_str,0,11);
        ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
        if(ret > 0)
        {
            start_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
            start_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
            ZDT_LOG("start_hour=%d,start_min=%d",start_hour,start_min);
        }

        SCI_MEMSET(timer_str,0,11);
        ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
        if(ret > 0)
        {
            end_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
            end_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
            ZDT_LOG("end_hour=%d,end_min=%d",end_hour,end_min);
        }
        
        time_start = (start_hour * 3600) + (start_min * 60);
        time_end = (end_hour * 3600) + (end_min * 60)+59;
        ZDT_LOG("time_start=%d,time_end=%d",time_start,time_end);
    }

    // 周期
    ret = YX_Func_GetNextParaF(&str, &len, buf, 100);
    if(ret > 0)
    {
        week_type = YX_Net_Handle_SET_CLASS_MODEL_Week(buf,ret);
    }

    //是否生效
    ret = YX_Func_GetNextParaF(&str, &len, buf, 100);
    if(ret > 0)
    {
        is_valid = atoi(buf);
        if(is_valid)
        {
            week_type = week_type | 0x01;
        }
        ZDT_LOG("is_valid=%d",is_valid);
    }
    YX_DB_TIMER_ListModify(cur_index,week_type,time_start,time_end);
    return 0;
}

int YX_Net_Receive_SET_CLASS_MODEL(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[1001] = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;

    for(j = YX_TIME_TYPE_CLASS_1; j < YX_TIME_TYPE_CLASS_MAX; j++)
    {
        YX_DB_TIMER_ListDel(j);
    }
    
    if(ContentLen == 0 || ContentLen == 1 )
    {
        ZDT_LOG("YX_Net_Receive_SET_CLASS_MODEL Err ContentLen");
        YX_DB_TIMER_ListSave();
        YX_CheckClassSilentOnOff();
        return 0;
    }
    
    ZDT_LOG("YX_Net_Receive_SET_CLASS_MODEL ContentLen = %d",ContentLen);

    for(i = 0; i < (YX_DB_TIME_CLASS_MAX_SUM+3); i++)
    {
        ret = YX_Func_GetNextPara(&str, &len,buf,1000);
        if(ret > 0)
        {
            if(i == 0)
            {
                // SOS呼入标识
                yx_DB_Set_Rec.sos_mt = atoi(buf);
                ZDT_LOG("sos_mt=%d",yx_DB_Set_Rec.sos_mt);
            }
            else if(i == 1)
            {
                // SOS呼出标识
                yx_DB_Set_Rec.sos_mo = atoi(buf);
                ZDT_LOG("sos_mo=%d",yx_DB_Set_Rec.sos_mo);
            }
            else if(i == 2)
            {
                if(ret == 1 && buf[0] == '0')
                {
                    ZDT_LOG("YX_Net_Receive_SET_CLASS_MODEL ,delete");
                    for(j = YX_TIME_TYPE_CLASS_1; j < YX_TIME_TYPE_CLASS_MAX; j++)
                    {
                        YX_DB_TIMER_ListDel(j);
                    }
                }
                else
                {
                    YX_Net_Handle_SET_CLASS_MODEL_Item(buf,ret);
                }
            }
            else
            {
                YX_Net_Handle_SET_CLASS_MODEL_Item(buf,ret);
            }
        }
    }
    
    YX_DB_SET_ListModify();
    YX_DB_TIMER_ListSave();
    YX_CheckClassSilentOnOff();
    ZDT_LOG("YX_Net_Receive_SET_CLASS_MODEL Over");
    return 0;
}

uint32 YX_Net_Respond_SEND_LOCATIN_DISABLE_MODEL(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SEND_LOCATIN_DISABLE_MODEL",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SEND_LOCATIN_DISABLE_MODEL Handle=0x%x",send_hande);
    return send_hande;
}

uint8 YX_Net_Handle_SEND_LOCATIN_DISABLE_MODEL_Week(uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char buf[11] = {0};
    uint16 i = 0;
    uint8 week_type = 0;
    uint8 cur_type = 0;
    uint8 * str = pData;
    uint32 len = DataLen;
    
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SEND_LOCATIN_DISABLE_MODEL_Week Err ContentLen");
        return 0;
    }
    
    for(i = 0; i < 7; i++)
    {
        ret = YX_Func_GetNextParaJ(&str, &len, buf, 10);
        if(ret > 0)
        {
            cur_type = atoi(buf);
            if(cur_type > 6)
            {
                cur_type = 0;
            }
            cur_type++;
            week_type = week_type | (0x01 << cur_type);
        }
    }
    ZDT_LOG("week_type=0x%x",week_type);
    return week_type;
}

// 序号=开始时间-结束时间！周期!是否生效
int YX_Net_Handle_SEND_LOCATIN_DISABLE_MODEL_Item(uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char buf[101] = {0};
    char timer_str[11] = {0};
    uint16 i = 0;
    uint16 cur_index = 0;
    uint8 * str = pData;
    uint32 len = DataLen;
    uint8 * pTime = NULL;
    uint32 time_len = 0;
    uint8 start_hour = 0;
    uint8 start_min = 0;
    uint8 end_hour = 0;
    uint8 end_min = 0;
    uint32 time_start = 0;
    uint32 time_end = 0;
    uint8 week_type = 0;
    uint8 cur_type = 0;
    uint8 is_valid = 0;
    
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SEND_LOCATIN_DISABLE_MODEL_Item Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Handle_SEND_LOCATIN_DISABLE_MODEL_Item %s",pData);
    
    //序号
    ret = YX_Func_GetNextParaD(&str, &len, buf, 100);
    if(ret > 0)
    {
        cur_index = atoi(buf);
        ZDT_LOG("cur_index=%d,%s",cur_index,buf);
    }
    if(cur_index > 0)
    {
        cur_index = cur_index + YX_TIME_TYPE_SLEEP_1 - 1;
    }
    else
    {
        cur_index = YX_TIME_TYPE_SLEEP_1;
    }

    //时间
    ret = YX_Func_GetNextParaF(&str, &len, buf, 100);
    if(ret > 0)
    {
        pTime = buf;
        time_len = ret;
        SCI_MEMSET(timer_str,0,11);
        ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
        if(ret > 0)
        {
            start_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
            start_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
            ZDT_LOG("start_hour=%d,start_min=%d",start_hour,start_min);
        }

        SCI_MEMSET(timer_str,0,11);
        ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
        if(ret > 0)
        {
            end_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
            end_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
            ZDT_LOG("end_hour=%d,end_min=%d",end_hour,end_min);
        }
        
        time_start = (start_hour * 3600) + (start_min * 60);
        time_end = (end_hour * 3600) + (end_min * 60);
        ZDT_LOG("time_start=%d,time_end=%d",time_start,time_end);
    }

    // 周期
    ret = YX_Func_GetNextParaF(&str, &len, buf, 100);
    if(ret > 0)
    {
        week_type = YX_Net_Handle_SEND_LOCATIN_DISABLE_MODEL_Week(buf,ret);
    }

    //是否生效
    ret = YX_Func_GetNextParaF(&str, &len, buf, 100);
    if(ret > 0)
    {
        is_valid = atoi(buf);
        if(is_valid)
        {
            week_type = week_type | 0x01;
        }
        ZDT_LOG("is_valid=%d",is_valid);
    }
    YX_DB_TIMER_ListModify(cur_index,week_type,time_start,time_end);
    return 0;
}

int YX_Net_Receive_SEND_LOCATIN_DISABLE_MODEL(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[1001] = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_SEND_LOCATIN_DISABLE_MODEL Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Receive_SEND_LOCATIN_DISABLE_MODEL ContentLen = %d",ContentLen);
    for(i = 0; i < YX_DB_TIME_SLEEP_MAX_SUM; i++)
    {
        ret = YX_Func_GetNextPara(&str, &len,buf,1000);
        if(ret > 0)
        {
            if(ret == 1 && buf[0] == '0')
            {
                for(j = YX_TIME_TYPE_SLEEP_1; j < YX_TIME_TYPE_SLEEP_MAX; j++)
                {
                    YX_DB_TIMER_ListDel(j);
                }
            }
            else
            {
                YX_Net_Handle_SEND_LOCATIN_DISABLE_MODEL_Item(buf,ret);
            }
        }
    }
    
    YX_DB_TIMER_ListSave();
    ZDT_LOG("YX_Net_Receive_SEND_LOCATIN_DISABLE_MODEL Over");
    return 0;
}

uint32 YX_Net_GET_CLASS_MODEL_Item(uint8 *pData, uint16 index)
{
    uint8 item_buf[201] = {0};
    uint32 item_len = 0;
    uint16 cur_index = 0;
    uint8 start_hour = 0;
    uint8 start_min = 0;
    uint8 end_hour = 0;
    uint8 end_min = 0;
    uint32 time_start = 0;
    uint32 time_end = 0;
    uint8 week_type = 0;
    uint8 cur_type = 0;
    uint8 is_valid = 0;
    uint8 i = 0;
    
    cur_index = index-YX_TIME_TYPE_CLASS_1 + 1;
    time_start = yx_DB_Timer_Reclist[index].time_start;
    time_end = yx_DB_Timer_Reclist[index].time_end;
    week_type = yx_DB_Timer_Reclist[index].time_set;

    start_hour = time_start/3600;
    start_min = (time_start%3600)/60;
    end_hour = time_end/3600;
    end_min = (time_end%3600)/60;

    sprintf(item_buf,"@%d=%02d%02d-%02d%02d!",cur_index,start_hour,start_min,end_hour,end_min);
    item_len = strlen(item_buf);
    for(i = 1; i < 8; i++)
    {
        if(((0x01<<i) & week_type) > 0)
        {
            item_buf[item_len++] = '0'+i-1;
            item_buf[item_len++] = '+';
        }
    }
    if(item_buf[item_len-1] == '+')
    {
        item_len--;
    }
    item_buf[item_len++] = '!';
    
    is_valid = week_type & 0x01;
    if(is_valid)
    {
        item_buf[item_len++] = '1';
    }
    else
    {
        item_buf[item_len++] = '0';
    }
    SCI_MEMCPY(pData,item_buf,item_len);
    return item_len;
}

uint32 YX_Net_GET_INCOMING_CALL_Item(uint8 *pData, uint16 index)
{
    uint8 tmp_buf[201] = {0};
    uint8 item_buf[201] = {0};
    uint32 item_len = 0;
    uint8 start_hour = 0;
    uint8 start_min = 0;
    uint8 end_hour = 0;
    uint8 end_min = 0;
    uint8 start2_hour = 0;
    uint8 start2_min = 0;
    uint8 end2_hour = 0;
    uint8 end2_min = 0;
    uint32 time_start = 0;
    uint32 time_end = 0;
    
    time_start = yx_DB_White_Reclist[index].time1_start;
    time_end = yx_DB_White_Reclist[index].time1_end;
    start_hour = time_start/3600;
    start_min = (time_start%3600)/60;
    end_hour = time_end/3600;
    end_min = (time_end%3600)/60;

    sprintf(item_buf,"%s=%s=%02d%02d-%02d%02d+",yx_DB_White_Reclist[index].white_name,yx_DB_White_Reclist[index].white_num,start_hour,start_min,end_hour,end_min);
    if(yx_DB_White_Reclist[index].time2_start != 0 && yx_DB_White_Reclist[index].time2_end != 0)
    {
        time_start = yx_DB_White_Reclist[index].time2_start;
        time_end = yx_DB_White_Reclist[index].time2_end;
        start2_hour = time_start/3600;
        start2_min = (time_start%3600)/60;
        end2_hour = time_end/3600;
        end2_min = (time_end%3600)/60;
        sprintf(tmp_buf,"%02d%02d-%02d%02d+",start2_hour,start2_min,end2_hour,end2_min);
        strcpy(item_buf,tmp_buf);
    }
    item_len = strlen(item_buf);

    item_buf[item_len++] = '!';
    SCI_MEMCPY(pData,item_buf,item_len);
    return item_len;
}

uint32 YX_Net_Respond_REQUEST_CALL(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"REQUEST_CALL",send_buf,send_len);
    YX_Start_ParentMonitorCall();
    ZDT_LOG("YX_Net_Respond_REQUEST_CALL Handle=0x%x",send_hande);
    return send_hande;
}

//[35380100360174,898600MFSSYYGXXXXXXP,201805082000001000,REQUEST_CALL,1,20180528111820,1,1]
int YX_Net_Receive_REQUEST_CALL(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[101] = {0};
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_REQUEST_CALL Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Receive_REQUEST_CALL ContentLen = %d",ContentLen);
    ret = YX_Func_GetNextPara(&str, &len,buf,100);
    if(ret > 0)
    {
        ZDT_LOG("call=%s",buf);
    }
    ZDT_LOG("YX_Net_Receive_REQUEST_CALL ret =%d, %s:%d",ret,yx_DB_Set_Rec.ip_domain,yx_DB_Set_Rec.ip_port);
    return 0;
}

static BOOLEAN YX_Clock_SaveNV(uint16 idx,uint16 Hour,uint16 Minute,uint8 onoff,uint8 clock_week,uint8 * pConten, uint16 content_len)
{
    uint8 i = 0;
    uint8 clock_type;
    uint32 pwrofftime = 0;
    uint8 is_on = onoff;
    uint16 cur_id = idx+EVENT_ALARM_0;
    uint32          cur_seconds = 0;
    uint32          res_seconds = 0;
    SCI_TIME_T          time;
    SCI_DATE_T          date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    if(clock_week >= 0xFE)
    {
        clock_type = 2;
    }
    else
    {
        clock_type = 3;
    }
    
    if(Hour >= 24)
    {
        return FALSE;
    }
    if(Minute >= 60)
    {
        return FALSE;
    }
    if(clock_week & 0x80)
    {
        clock_week |= 0x01;
        clock_week &= 0x7F;
    }
    else
    {
        clock_week &= 0xFE;
    }
    SCI_MEMSET(&s_yx_current_event_info,0,sizeof(EVENT_CURRENT_DATA_T));
    
    s_yx_current_event_info.id = cur_id;

    MMIAPIALM_FormInteEventInfo(s_yx_current_event_info.id,&s_yx_current_event_info.event);
    
    s_yx_current_event_info.type = SMART_EVENT_ALARM;
    s_yx_current_event_info.event.event_fast_info.is_valid = TRUE;
    if(onoff)
    {
        s_yx_current_event_info.event.event_fast_info.is_on = TRUE;
    }
    else
    {
        s_yx_current_event_info.event.event_fast_info.is_on = FALSE;
    }
    s_yx_current_event_info.event.event_fast_info.snooze_type = ALM_SNOOZE_TIME_10_MIN;
    s_yx_current_event_info.event.event_fast_info.hour = Hour;
    s_yx_current_event_info.event.event_fast_info.minute = Minute;

#if 0
    cur_seconds = MMIAPICOM_Tm2Second(time.sec, time.min, time.hour, date.mday, date.mon, date.year);
    res_seconds = MMIAPICOM_Tm2Second(0, Minute, Hour, date.mday, date.mon, date.year);

    if(cur_seconds > res_seconds)
    {
        //推迟一天
        
    }
#endif

    s_yx_current_event_info.event.event_fast_info.year = date.year;
    s_yx_current_event_info.event.event_fast_info.mon = date.mon;
    s_yx_current_event_info.event.event_fast_info.day = date.mday;
    
    if(clock_type == 1)
    {
        //一次
        s_yx_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_ONCE;
        
    }
    else if(clock_type == 2)
    {
        //每天
        s_yx_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
    }
    else if(clock_type == 3)
    {
        //自定义
        s_yx_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
        s_yx_current_event_info.event.event_fast_info.fre = clock_week;
    }
    else
    {
        //一次
        s_yx_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_ONCE;
    }

    s_yx_current_event_info.event.event_content_info.ring = 1;
    s_yx_current_event_info.event.event_content_info.is_vibrate = 1;
    s_yx_current_event_info.event.event_content_info.is_add = TRUE;
    
    if(pConten != PNULL && content_len > 0)
    {
        s_yx_current_event_info.event.event_content_info.w_length = GUI_UTF8ToWstr(s_yx_current_event_info.event.event_content_info.w_content, MMIACC_EVENT_TEXT_MAX_LEN,(uint8 *)pConten, content_len);
    }
    else
    {
        s_yx_current_event_info.event.event_content_info.w_length = 3;
        s_yx_current_event_info.event.event_content_info.w_content[0] = 0x95F9;
        s_yx_current_event_info.event.event_content_info.w_content[1] = 0x949F;
        s_yx_current_event_info.event.event_content_info.w_content[2] = 0x0031+idx;
    }
     SetDelayAlarmExt(s_yx_current_event_info.id, &s_yx_current_event_info.event, FALSE);
     // 修改闹钟设置
     MMIAPIALM_SaveInteEventInfo(s_yx_current_event_info.id,&s_yx_current_event_info.event);    
    return TRUE;
}

uint32 YX_Net_Respond_SET_CLOCK_ACTION(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SET_CLOCK_ACTION",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SET_CLOCK_ACTION Handle=0x%x",send_hande);
    return send_hande;
}

//周期 周一至周六用1-6表示，周日用0表示，如1+2+3+4表示周一至周四
uint8 YX_Net_Handle_SET_CLOCK_ACTION_Week(uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char buf[11] = {0};
    uint16 i = 0;
    uint8 week_type = 0;
    uint8 cur_type = 0;
    uint8 * str = pData;
    uint32 len = DataLen;
    
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SET_CLOCK_ACTION_Week Err ContentLen");
        return 0;
    }
    
    for(i = 0; i < 7; i++)
    {
        ret = YX_Func_GetNextParaJ(&str, &len, buf, 10);
        if(ret > 0)
        {
            cur_type = atoi(buf);
            if(cur_type == 0)
            {
                cur_type = 7;
            }
            week_type = week_type | (0x01 << cur_type);
        }
    }
    
    ZDT_LOG("week_type=0x%x",week_type);
    return week_type;
}

// 标题!是否震动!是否响铃!提醒时间!周期!是否生效
//闹钟时间!周期!生效标志
int YX_Net_Handle_SET_CLOCK_ACTION_Item(uint16 cur_index,uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char time_buf[5] = {0};
    char buf[101] = {0};
    uint16 i = cur_index;
    uint8 * str = pData;
    uint32 len = DataLen;
    uint8 is_vib = 0;
    uint8 is_ring = 0;
    uint8 clock_hour = 0;
    uint8 clock_min = 0;
    uint32 clock_id= 0;
    uint8 clock_onoff= 0;
    uint8 clock_type= 0;
    uint8 clock_week = 0xFF;
    uint32 clock_time= 0;
    
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SET_CLOCK_ACTION_Item Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Handle_SET_CLOCK_ACTION_Item %s",pData);

    //提醒时间
    ret = YX_Func_GetNextParaF(&str, &len, time_buf, 5);
    if(ret > 0)
    {
        buf[0] = time_buf[0];
        buf[1] = time_buf[1];
        buf[2] = 0;
        clock_hour = atoi(buf);
        buf[0] = time_buf[3];
        buf[1] = time_buf[4];
        clock_min = atoi(buf);
        clock_time = (clock_hour * 3600) + (clock_min * 60);
        ZDT_LOG("time=%s,%02d-%02d, %d",time_buf,clock_hour,clock_min,clock_time);
    }

    // 周期
    ret = YX_Func_GetNextParaF(&str, &len, buf, 100);
    if(ret > 0)
    {
        clock_week = YX_Net_Handle_SET_CLOCK_ACTION_Week(buf,ret);
        ZDT_LOG("clock_week=0x%x",clock_week);
    }

    //是否生效
    ret = YX_Func_GetNextParaF(&str, &len, buf, 100);
    if(ret > 0)
    {
        clock_onoff = atoi(buf);
        if(clock_week)
        {
            clock_week |= 0x01;
        }
        else
        {
            clock_week &= 0xfe;
        }
        ZDT_LOG("clock_onoff=%d",clock_onoff);
    }
    YX_DB_CLOCK_ListModifyBuf(i,clock_id,clock_onoff,clock_type,clock_week,clock_time,PNULL,0);
    YX_Clock_SaveNV(i,clock_hour,clock_min,clock_onoff,clock_week,PNULL,0);
    return 0;
}

//[35380100360174,898600MFSSYYGXXXXXXP,201805082000001000,SET_CLOCK_ACTION,1,20180528111820,47,喝水!1!1!1130!1+2+3!1@睡觉!1!1!2130!1+2+3+4+5+6+0!1]
//[35380100360174,898600MFSSYYGXXXXXXP,201805082000001000,SET_CL0CK_ACT I0N,1,20180528111820,40,1@07:00!1+2+3!1@2@23:00!0+1+2+3+4+5+6!1]
int YX_Net_Receive_SET_CLOCK_ACTION(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[1001] = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_SET_CLOCK_ACTION Err ContentLen");
        return 0;
    }
    MMIAPIALM_RestoreFactorySetting();
    YX_DB_CLOCK_ListDelAllBuf();
    ZDT_LOG("YX_Net_Receive_SET_CLOCK_ACTION ContentLen = %d",ContentLen);
    for(i = 0; i < YX_DB_CLOCK_MAX_SUM; i++)
    {
        ret = YX_Func_GetNextPara(&str, &len,buf,10);
        ret = YX_Func_GetNextPara(&str, &len,buf,1000);
        if(ret > 0)
        {
            if(ret == 1 && buf[0] == '0')
            {
                //YX_DB_CLOCK_ListDelBuf(i);
            }
            else
            {
                YX_Net_Handle_SET_CLOCK_ACTION_Item(i,buf,ret);
            }
        }
    }
    
    YX_DB_CLOCK_ListSaveBuf();
    ZDT_LOG("YX_Net_Receive_SET_CLOCK_ACTION Over");
    return 0;
}

uint32 YX_Net_Respond_SEND_MSG(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SEND_MSG",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SEND_MSG Handle=0x%x",send_hande);
    return send_hande;
}

int YX_Net_Receive_SEND_MSG(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    uint8 buf[2048] = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;
    YX_DB_NETSMS_Type cur_data = {0};
    MMISET_CALL_RING_TYPE_E     call_ring_type = MMISET_CALL_RING;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_SEND_MSG Err ContentLen");
        return 0;
    }

    ZDT_LOG("YX_Net_Receive_SEND_MSG ContentLen = %d",ContentLen);
    cur_data.netsms_type = 5;
    cur_data.netsms_mode = 1;
    cur_data.netsms_playtimes = 0;
    cur_data.netsms_isring = 1;
    cur_data.netsms_isvib = 1;
    cur_data.netsms_isflash = 0;
    ret = YX_Func_GetNextPara(&str, &len,buf,10);
    ZDT_LOG("1111---ret=%d",ret);
    if(ret > 0)
    {
         
    }
   
    ret = YX_Func_GetNextPara(&str, &len,buf,1800);
    ZDT_LOG("2222---ret=%d",ret);
    if(ret > 0)
    {
        cur_data.netsms_content_len = ZDT_Str16_to_Int(buf,ret,cur_data.netsms_content);
	 ZDT_LOG("W9K_LOG:____cur_data.netsms_content=%s",cur_data.netsms_content);
        ZDT_LOG("netsms_content_len=%d",cur_data.netsms_content_len);
    }
    cur_data.netsms_encoding = 2;
    cur_data.netsms_rcv_time = ZDT_Get_TimeSecond();
    YX_DB_NetSms_Add(&cur_data);
    ZDT_LOG("YX_Net_Receive_SEND_SMS Over,netsms_rcv_time=%d",cur_data.netsms_rcv_time);
    MMIAPICOM_StatusAreaSetMsgState(TRUE);
    #ifdef ZDT_BH_W9_FEATRUE
    MMIDEFAULT_TurnOnBackLight();
    MMIAPI_New_Message_Win(cur_data.netsms_type);
    //MMIPUB_OpenAlertWarningWin(STR_MESSAGE_HAVE_NEWS);
    #endif
    call_ring_type = (MMISET_CALL_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, CALL_RING_TYPE);
	switch (call_ring_type)
	{
	 case MMISET_CALL_SILENT:
                cur_data.netsms_isvib = 0;
                cur_data.netsms_isring = 0;
		 break;
	 case MMISET_CALL_RING:
                cur_data.netsms_isvib = 0;
                cur_data.netsms_isring = 1;
		 break;
	 case MMISET_CALL_VIBRA:
                cur_data.netsms_isvib = 1;
                cur_data.netsms_isring = 0;
		 break;
	 case MMISET_CALL_VIBRA_AND_RING:
                cur_data.netsms_isvib = 1;
                cur_data.netsms_isring = 1;
		 break;
	 default:
                cur_data.netsms_isvib = 0;
                cur_data.netsms_isring = 1;
        	 break;
	}
        if(cur_data.netsms_isring)
        {
            if(!YX_API_Mute_IsValid() && !MMIALARM_IsActiveWin())
            {
#ifdef ZDT_PLAY_RING_CUSTOM
                MMIAPISET_PlayCustomRing(MMISET_CUST_VOICE_NEW,1);
#else
                MMIAPISET_PlayRingByIdEx(FALSE,1,7,1,MMISET_RING_TYPE_MSG,PNULL);
#endif
            }
        }
        if(cur_data.netsms_isvib)
        {
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,1000);
        }
    if(cur_data.netsms_isflash)
    {
#ifdef ZDT_LED_SUPPORT
        ZDT_Led_Start_Call_Incoming();
#endif
    }
    return 0;
}

uint32 YX_Net_Respond_SEND_SMS(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SEND_SMS",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SEND_SMS Handle=0x%x",send_hande);
    return send_hande;
}

//[862622051083028,89860803192070506004,202105071759545407,SEND_SMS,1,20210507175954,201,2@1957@1@0@0@0@0@E682A8E69C89E4B880E4B8AAE9A284E4B9A0E4BBBBE58AA1E380903035E69C883037E697A52028E5B08FE5ADA6E8AFADE696872920E9A284E4B9A0E4BBBBE58AA1E38091EFBC8CE8AFB7E59CA8E5AEB6E995BFE7ABAF E69FA5E79C8B]
int YX_Net_Receive_SEND_SMS(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[2048] = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;
    YX_DB_NETSMS_Type cur_data = {0};
    MMISET_CALL_RING_TYPE_E     call_ring_type = MMISET_CALL_RING;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_SEND_SMS Err ContentLen");
        return 0;
    }

    ZDT_LOG("YX_Net_Receive_SEND_SMS ContentLen = %d",ContentLen);
    ret = YX_Func_GetNextPara(&str, &len,buf,10);
    if(ret > 0)
    {
        cur_data.netsms_type = atoi(buf);
        ZDT_LOG("netsms_type=%d",cur_data.netsms_type);
    }
    ret = YX_Func_GetNextPara(&str, &len,cur_data.msg_id,YX_DB_NETSMS_MAX_UUID_SIZE);
    if(ret > 0)
    {
        ZDT_LOG("msg_id=%s",cur_data.msg_id);
    }
    
    ret = YX_Func_GetNextPara(&str, &len,buf,10);
    if(ret > 0)
    {
        cur_data.netsms_mode = atoi(buf);
        ZDT_LOG("netsms_mode=%d",cur_data.netsms_mode);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,10);
    if(ret > 0)
    {
        cur_data.netsms_playtimes= atoi(buf);
        ZDT_LOG("netsms_playtimes=%d",cur_data.netsms_playtimes);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,10);
    if(ret > 0)
    {
        cur_data.netsms_isflash = atoi(buf);
        ZDT_LOG("netsms_isflash=%d",cur_data.netsms_isflash);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,10);
    if(ret > 0)
    {
        cur_data.netsms_isvib = atoi(buf);
        ZDT_LOG("netsms_isvib=%d",cur_data.netsms_isvib);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,10);
    if(ret > 0)
    {
        cur_data.netsms_isring = atoi(buf);
        ZDT_LOG("netsms_isring=%d",cur_data.netsms_isring);
    }
    ret = YX_Func_GetNextPara(&str, &len,buf,1800);
    if(ret > 0)
    {
        cur_data.netsms_content_len = ZDT_Str16_to_Int(buf,ret,cur_data.netsms_content);
	 ZDT_LOG("W9K_LOG:____cur_data.netsms_content=%s",cur_data.netsms_content);
        ZDT_LOG("netsms_content_len=%d",cur_data.netsms_content_len);
    }
    cur_data.netsms_encoding = 2;
    cur_data.netsms_rcv_time = ZDT_Get_TimeSecond();
    YX_DB_NetSms_Add(&cur_data);
    ZDT_LOG("YX_Net_Receive_SEND_SMS Over,netsms_rcv_time=%d",cur_data.netsms_rcv_time);
    MMIAPICOM_StatusAreaSetMsgState(TRUE);
    #ifdef ZDT_BH_W9_FEATRUE
    MMIDEFAULT_TurnOnBackLight();
    MMIAPI_New_Message_Win(cur_data.netsms_type);
    //MMIPUB_OpenAlertWarningWin(STR_MESSAGE_HAVE_NEWS);
    #endif
    call_ring_type = (MMISET_CALL_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, CALL_RING_TYPE);
	switch (call_ring_type)
	{
	 case MMISET_CALL_SILENT:
                cur_data.netsms_isvib = 0;
                cur_data.netsms_isring = 0;
		 break;
	 case MMISET_CALL_RING:
                cur_data.netsms_isvib = 0;
                cur_data.netsms_isring = 1;
		 break;
	 case MMISET_CALL_VIBRA:
                cur_data.netsms_isvib = 1;
                cur_data.netsms_isring = 0;
		 break;
	 case MMISET_CALL_VIBRA_AND_RING:
                cur_data.netsms_isvib = 1;
                cur_data.netsms_isring = 1;
		 break;
	 default:
                cur_data.netsms_isvib = 0;
                cur_data.netsms_isring = 1;
        	 break;
	}
        if(cur_data.netsms_isring)
        {
            if(!YX_API_Mute_IsValid() && !MMIALARM_IsActiveWin())
            {
#ifdef ZDT_PLAY_RING_CUSTOM
                MMIAPISET_PlayCustomRing(MMISET_CUST_VOICE_NEW,1);
#else
                MMIAPISET_PlayRingByIdEx(FALSE,1,7,1,MMISET_RING_TYPE_MSG,PNULL);
#endif
            }
        }
        if(cur_data.netsms_isvib)
        {
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,1000);
        }
    if(cur_data.netsms_isflash)
    {
#ifdef ZDT_LED_SUPPORT
        ZDT_Led_Start_Call_Incoming();
#endif
    }
    return 0;
}

#if 0
const char * g_test_net_sms = "2@1957@1@3@1@1@1@E682A8E69C89E4B880E4B8AAE9A284E4B9A0E4BBBBE58AA1E380903035E69C883037E697A52028E5B08FE5ADA6E8AFADE696872920E9A284E4B9A0E4BBBBE58AA1E38091EFBC8CE8AFB7E59CA8E5AEB6E995BFE7ABAFE69FA5E79C8B";
void YX_Net_Test_NetSms(void)
{
    YX_Net_Receive_SEND_SMS(&g_yx_app,g_test_net_sms,strlen(g_test_net_sms));
}
#endif

uint32 YX_Net_Respond_SET_REGIONAL_ALARM(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SET_REGIONAL_ALARM",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SET_REGIONAL_ALARM Handle=0x%x",send_hande);
    return send_hande;
}

/*
[864521043102235,89860322247553879380,202206021921090005,SET_REGIONAL_ALARM,1,20220602192109,67,1@1@Round*2*(22.530811#113.953323)*(500)@1@0600-2358@1+2+3+4+5+6+0+]
*/
int YX_Net_Handle_SET_REGIONAL_ALARM_Item(REGIONAL_GPS_INFO_T * pRegionalGps,uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char buf[101] = {0};
    uint8 * str = pData;
    uint32 len = DataLen;    
    double dtmp = 0;
    uint32 Latitude = 0;
    uint8 Lat_Dir = 0;
    uint32 Longitude = 0;
    uint8 Long_Dir = 0;
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SET_REGIONAL_ALARM_Item Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Handle_SET_REGIONAL_ALARM_Item %s",pData);
    
    ret = YX_Func_GetNextParaHush(&str, &len, buf, 100);
    if(ret > 0)
    {
        dtmp = (double)atof(buf);
        pRegionalGps->Latitude = dtmp;
        ZDT_LOG("Latitude=%s,%lf",buf,pRegionalGps->Latitude);
    }

    ret = YX_Func_GetNextParaHush(&str, &len, buf, 100);
    if(ret > 0)
    {
        dtmp = (double)atof(buf);
        pRegionalGps->Longitude = dtmp;
        ZDT_LOG("Longitude=%s,%lf",buf,pRegionalGps->Longitude);
    }
    return 0;
}

/*
[864521043102235,89860322247553879380,202206021921090005,SET_REGIONAL_ALARM,1,20220602192109,67,1@1@Round*2*(22.530811#113.953323)*(500)@1@0600-2358@1+2+3+4+5+6+0+]
*/
int YX_Net_Handle_SET_REGIONAL_ALARM_Shape(YX_DB_REGIONAL_Type  * pRegional,uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    uint8 i = 0;
    char buf[101] = {0};
    uint8 * str = pData;
    uint32 len = DataLen;
    
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SET_REGIONAL_ALARM_Shape Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Handle_SET_REGIONAL_ALARM_Shape %s",pData);
    
    //形状
    ret = YX_Func_GetNextParaStar(&str, &len, buf, 100);
    if(ret > 0)
    {
        if(0 == strncmp(buf,"Round",5))
        {
            //圆形
            pRegional->zone_shape = 1;
        }
        else if(0 == strncmp(buf,"Rectangle",9))
        {
            //矩形
            pRegional->zone_shape = 2;
        }
        else if(0 == strncmp(buf,"Polygon",7))
        {
            //多边形
            pRegional->zone_shape = 3;
        }
        ZDT_LOG("zone_shape=%d",pRegional->zone_shape);
    }
    ret = YX_Func_GetNextParaStar(&str, &len, buf, 100);
    if(ret > 0)
    {
        pRegional->gps_sum = atoi(buf);
        ZDT_LOG("gps_sum=%d",pRegional->gps_sum);
    }
    if(pRegional->zone_shape == 1)
    {
        pRegional->gps_sum = 1;
        ret = YX_Func_GetNextParaStar(&str, &len, buf, 100);
        if(ret > 2)
        {
            YX_Net_Handle_SET_REGIONAL_ALARM_Item(&pRegional->regional_gps[0],buf+1,ret-2);
        }
        ret = YX_Func_GetNextParaStar(&str, &len, buf, 100);
        if(ret > 2)
        {
            buf[ret-1] = 0;
            pRegional->gps_radius = atoi(&buf[1]);
            ZDT_LOG("gps_radius=%d",pRegional->gps_radius);
        }
    }
    else if(pRegional->zone_shape == 2)
    {
        ret = YX_Func_GetNextParaStar(&str, &len, buf, 100);
        if(ret > 2)
        {
            YX_Net_Handle_SET_REGIONAL_ALARM_Item(&pRegional->regional_gps[0],buf+1,ret-2);
        }
        ret = YX_Func_GetNextParaStar(&str, &len, buf, 100);
        if(ret > 2)
        {
            YX_Net_Handle_SET_REGIONAL_ALARM_Item(&pRegional->regional_gps[1],buf+1,ret-2);
        }
    }
    else if(pRegional->zone_shape == 3)
    {
        for(i = 0; i < pRegional->gps_sum; i++)
        {
            if(i >= MAX_YX_REGIONAL_GPS_NUM)
            {
                break;
            }
            ret = YX_Func_GetNextParaStar(&str, &len, buf, 100);
            if(ret > 2)
            {
                YX_Net_Handle_SET_REGIONAL_ALARM_Item(&pRegional->regional_gps[i],buf+1,ret-2);
            }
        }
    }
    return 0;
}
uint8 YX_Net_Handle_SET_REGIONAL_ALARM_Week(uint8 * pData,uint32 DataLen)
{
    int ret = 0;
    char buf[11] = {0};
    uint16 i = 0;
    uint8 week_type = 0;
    uint8 cur_type = 0;
    uint8 * str = pData;
    uint32 len = DataLen;
    uint8 * pItem = PNULL;
    uint32 ItemLen = 0;
    if(DataLen == 0)
    {
        ZDT_LOG("YX_Net_Handle_SET_REGIONAL_ALARM_Week Err ContentLen");
        return 0;
    }
    
    for(i = 0; i < 7; i++)
    {
        ret = YX_Func_GetNextParaJ(&str, &len, buf, 10);
        if(ret > 0)
        {
            cur_type = atoi(buf);
            if(cur_type > 6)
            {
                cur_type = 0;
            }
            cur_type++;
            week_type = week_type | (0x01 << cur_type);
        }
    }
    ZDT_LOG("week_type=0x%x",week_type);
    return week_type;
}
/*
操作代码@请求状态@形状*元素个数*元素值 1*元素值 2*...*元素值 n@区域编号@时间段@周期字段
[864521043102235,89860322247553879380,202206021921090005,SET_REGIONAL_ALARM,1,20220602192109,67,1@1@Round*2*(22.530811#113.953323)*(500)@1@0600-2358@1+2+3+4+5+6+0+]
*/
int YX_Net_Receive_SET_REGIONAL_ALARM(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char buf[1001] = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint8 * str = pContent;
    uint32 len = ContentLen;
    uint8 * pItem = PNULL;
    uint32 ItemLen = 0;
    char timer_str[11] = {0};
    uint8 item_buf[201] = {0};
    uint8 * pTime = NULL;
    uint32 time_len = 0;
    uint8 start_hour = 0;
    uint8 start_min = 0;
    uint8 end_hour = 0;
    uint8 end_min = 0;
    YX_DB_REGIONAL_Type  cur_type = {0};
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_SET_REGIONAL_ALARM Err ContentLen");
        return 0;
    }
    ZDT_LOG("YX_Net_Receive_SET_REGIONAL_ALARM ContentLen = %d,str=%s",ContentLen,str);
    ret = YX_Func_GetNextPara(&str, &len,buf,1000);
    if(ret > 0)
    {
        cur_type.opt_type = atoi(buf);
        ZDT_LOG("opt_type=%d",cur_type.opt_type);
    }
    
    ret = YX_Func_GetNextPara(&str, &len,buf,1000);
    if(ret > 0)
    {
        cur_type.request_status = atoi(buf);
        ZDT_LOG("request_status=%d",cur_type.request_status);
    }

    ret = YX_Func_GetNextPara(&str, &len,buf,1000);
    if(ret > 0)
    {
        YX_Net_Handle_SET_REGIONAL_ALARM_Shape(&cur_type,buf,ret);
    }
    
    ret = YX_Func_GetNextPara(&str, &len,buf,1000);
    if(ret > 0)
    {
        cur_type.zone_id = atoi(buf);
        ZDT_LOG("zone_id=%d",cur_type.zone_id);
    }
    
    ret = YX_Func_GetNextPara(&str, &len,buf,1000);
    if(ret > 0)
    {
        //时间段
        pItem = buf;
        ItemLen = ret;
        ret = YX_Func_GetNextParaJ(&pItem , &ItemLen,item_buf,200);
        if(ret > 0)
        {
            start_hour = 0;
            start_min = 0;
            end_hour = 0;
            end_min = 0;
            pTime = item_buf;
            time_len = ret;
            SCI_MEMSET(timer_str,0,11);
            ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
            if(ret > 0)
            {
                start_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
                start_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
                ZDT_LOG("start_hour=%d,start_min=%d",start_hour,start_min);
            }

            SCI_MEMSET(timer_str,0,11);
            ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
            if(ret > 0)
            {
                end_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
                end_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
                ZDT_LOG("end_hour=%d,end_min=%d",end_hour,end_min);
            }
            
            cur_type.time1_start = (start_hour * 3600) + (start_min * 60);
            cur_type.time1_end = (end_hour * 3600) + (end_min * 60);
            ZDT_LOG("time1_start=%d,time1_end=%d",cur_type.time1_start,cur_type.time1_end);
        }
        
        ret = YX_Func_GetNextParaJ(&pItem , &ItemLen,item_buf,500);
        if(ret > 0)
        {
            start_hour = 0;
            start_min = 0;
            end_hour = 0;
            end_min = 0;
            pTime = buf;
            time_len = ret;
            SCI_MEMSET(timer_str,0,11);
            ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
            if(ret > 0)
            {
                start_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
                start_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
                ZDT_LOG("start_hour=%d,start_min=%d",start_hour,start_min);
            }

            SCI_MEMSET(timer_str,0,11);
            ret = YX_Func_TimerGetNextPara(&pTime,&time_len,timer_str,10);
            if(ret > 0)
            {
                end_hour = ((timer_str[0]-'0')*10 + (timer_str[1]-'0'));
                end_min = ((timer_str[2]-'0')*10 + (timer_str[3]-'0'));
                ZDT_LOG("end_hour=%d,end_min=%d",end_hour,end_min);
            }
            
            cur_type.time2_start = (start_hour * 3600) + (start_min * 60);
            cur_type.time2_end = (end_hour * 3600) + (end_min * 60);
            ZDT_LOG("time2_start=%d,time2_end=%d",cur_type.time2_start,cur_type.time2_end);
        }
    }
    
    ret = YX_Func_GetNextPara(&str, &len,buf,1000);
    if(ret > 0)
    {
        //周期时间
        cur_type.time_week = YX_Net_Handle_SET_REGIONAL_ALARM_Week(buf,ret);
        ZDT_LOG("time_week=0x%x",cur_type.time_week);
    }
    if(cur_type.opt_type == 1)
    {
        YX_DB_REGIONAL_Add(&cur_type);
    }
    else if(cur_type.opt_type == 2)
    {
        YX_DB_REGIONAL_Change(&cur_type);
    }
    else if(cur_type.opt_type == 3)
    {
        YX_DB_REGIONAL_Delete(&cur_type);
    }
    YX_DB_REGIONAL_ListSave();
    YX_API_LOCTION_WL_Start();
    ZDT_LOG("YX_Net_Receive_SET_REGIONAL_ALARM Over");
    return 0;
}

int YX_Net_Receive_SET_REGIONAL_ALARM_Test()
{
   YX_Net_Receive_SET_REGIONAL_ALARM(&g_yx_app, "1@1@Round*2*(22.533607#113.949197)*(100)@1@0600-2358@1+2+3+4+5+6+0+",67);
    return 0;
}

uint32 YX_Net_Respond_SET_MONITOR_REGIONAL(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SET_MONITOR_REGIONAL",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SET_MONITOR_REGIONAL Handle=0x%x",send_hande);
    return send_hande;
}

uint32 YX_Net_Respond_SET_MONITOR_PLACE(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SET_MONITOR_PLACE",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SET_MONITOR_PLACE Handle=0x%x",send_hande);
    return send_hande;
}

uint32 YX_Net_Respond_SET_SMS_INTERCEPT(YX_APP_T *pMe,char * water_str)
{
    uint8 send_buf[2] = {0};
    uint32 send_len = 0;
    uint32 send_hande = 0;
    send_buf[0] = '0';
    send_len = 1;
    send_hande = YX_Net_TCPRespond('2',water_str,"SET_SMS_INTERCEPT",send_buf,send_len);
    ZDT_LOG("YX_Net_Respond_SET_SMS_INTERCEPT Handle=0x%x",send_hande);
    return send_hande;
}

int YX_Net_Receive_SET_SMS_INTERCEPT(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char min_buf[31] = {0};
    uint8 * str = pContent;
    uint32 len = ContentLen;
    uint8 sms_intercept = 2;
    
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_SET_SMS_INTERCEPT Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Receive_SET_SMS_INTERCEPT ContentLen = %d",ContentLen);
    
    ret = YX_Func_GetNextPara(&str, &len,min_buf,30);
    if(ret > 0)
    {
        sms_intercept = atoi(min_buf);
        ZDT_LOG("sms_intercept=%s,%d",min_buf,sms_intercept);
    }

    yx_DB_Set_Rec.sms_intercept = sms_intercept;
    YX_DB_SET_ListModify();
    ZDT_LOG("YX_Net_Receive_SET_SMS_INTERCEPT sms_intercept =%d",yx_DB_Set_Rec.sms_intercept);
    return 0;
}

uint32 YX_Net_Receive_HDKT_SEND_QUESTION(YX_APP_T *pMe,uint8 * pContent,uint32 ContentLen)
{
    int ret = 0;
    char min_buf[31] = {0};
    uint8 * str = pContent;
    uint32 len = ContentLen;
    uint8 answer_type = 0;
    uint8 job_type = 0;
    if(ContentLen == 0)
    {
        ZDT_LOG("YX_Net_Receive_HDKT_SEND_QUESTION Err ContentLen");
        return 0;
    }
    
    ZDT_LOG("YX_Net_Receive_HDKT_SEND_QUESTION ContentLen = %d",ContentLen);
    
    ret = YX_Func_GetNextPara(&str, &len,min_buf,30);
    if(ret > 0)
    {
        answer_type = atoi(min_buf);
        ZDT_LOG("answer_type=%s,%d",min_buf,answer_type);
    }

    ret = YX_Func_GetNextPara(&str, &len,min_buf,30);
    if(ret > 0)
    {
        job_type = atoi(min_buf);
        ZDT_LOG("job_type=%s,%d",min_buf,job_type);
    }
    yx_job_cur_data.answer_type = answer_type;
    yx_job_cur_data.job_type = job_type;
    //在此处增加界面的处理
    MMIZDT_OpenWLDT_ByMode(answer_type,job_type);
    ZDT_LOG("YX_Net_Receive_HDKT_SEND_QUESTION answer_type =%d, job_type=%d",answer_type,job_type);
    return 0;
}

static int YX_Net_ReceiveHandle(YX_APP_T *pMe,char * mark_str,uint8 * pData,uint32 DataLen,uint32 Content_len)
{
    uint8 * pContent;
    uint32 cont_len = Content_len;
    char buf[101] = {0};
    char water_str[ZDT_WATER_LEN] = {0};
    int ret = 0;
    uint8 * pStr = pData;
    uint32 len = DataLen;
   
    ZDT_LOG("YX_Net_ReceiveHandle: mark_str=%s,DataLen=%d, Content_len=%d",mark_str,DataLen, Content_len);
    ZDT_LOG("ZDT__LOG YX_Net_ReceiveHandle send_buf_clip0=%s",pData);	
    ZDT_LOG("ZDT__LOG YX_Net_ReceiveHandle send_buf_clip1=%s",pData+76);
    YX_Func_GetCmdPosPara(&pStr, &len, water_str,ZDT_WATER_LEN-1,2);
    pContent = pData+DataLen-cont_len-1;
    if(0 == strncmp( (char *)mark_str, "SET_SERVER_INFO", strlen("SET_SERVER_INFO") ))
    {
        YX_Net_Receive_SET_SERVER_INFO(pMe,pContent,cont_len);
        YX_Net_Respond_SET_SERVER_INFO(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "SET_INCOMING_CALL", strlen("SET_INCOMING_CALL") ))
    {
        YX_Net_Receive_SET_INCOMING_CALL(pMe,pContent,cont_len);
	 YX_Net_Respond_SET_INCOMING_CALL(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "SET_NORMAL_BUTTON", strlen("SET_NORMAL_BUTTON") ))
    {
        YX_Net_Receive_SET_NORMAL_BUTTON(pMe,pContent,cont_len);
	YX_Net_Respond_SET_NORMAL_BUTTON(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "LOCATION_INFO_GET", strlen("LOCATION_INFO_GET") ))
    {
        YX_Net_Receive_LOCATION_INFO_GET(pMe,water_str);
        //YX_Net_Respond_LOCATION_INFO_GET(pMe,0,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "FREQUENCY_LOCATION_SET", strlen("FREQUENCY_LOCATION_SET") ))
    {
        YX_Net_Receive_FREQUENCY_LOCATION_SET(pMe,pContent,cont_len);
        YX_Net_Respond_FREQUENCY_LOCATION_SET(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "REMOTE_OPERATE_TERMINAL", strlen("REMOTE_OPERATE_TERMINAL") ))
    {
        YX_Net_Receive_REMOTE_OPERATE_TERMINAL(pMe,pContent,cont_len);
        YX_Net_Respond_REMOTE_OPERATE_TERMINAL(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "SET_HEARTBEAT", strlen("SET_HEARTBEAT") ))
    {
        YX_Net_Receive_SET_HEARTBEAT(pMe,pContent,cont_len);
        YX_Net_Respond_SET_HEARTBEAT(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "SET_MODEL", strlen("SET_MODEL") ))
    {
        YX_Net_Receive_SET_MODEL(pMe,pContent,cont_len);
         YX_Net_Respond_SET_MODEL(pMe,water_str);
   }
    else if(0 == strncmp( (char *)mark_str, "SET_CLASS_MODEL", strlen("SET_CLASS_MODEL") ))
    {
        YX_Net_Receive_SET_CLASS_MODEL(pMe,pContent,cont_len);
	 YX_Net_Respond_SET_CLASS_MODEL(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "SEND_LOCATIN_DISABLE_MODEL", strlen("SEND_LOCATIN_DISABLE_MODEL") ))
    {
        YX_Net_Receive_SEND_LOCATIN_DISABLE_MODEL(pMe,pContent,cont_len);
        YX_Net_Respond_SEND_LOCATIN_DISABLE_MODEL(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "REQUEST_CALL", strlen("REQUEST_CALL") ))
    {
        YX_Net_Receive_REQUEST_CALL(pMe,pContent,cont_len);
        YX_Net_Respond_REQUEST_CALL(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "SET_CLOCK_ACTION", strlen("SET_CLOCK_ACTION") ))
    {
        YX_Net_Receive_SET_CLOCK_ACTION(pMe,pContent,cont_len);
        YX_Net_Respond_SET_CLOCK_ACTION(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "SEND_MSG", strlen("SEND_MSG") ))
    {
        YX_Net_Receive_SEND_MSG(pMe,pContent,cont_len);
        YX_Net_Respond_SEND_MSG(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "SEND_SMS", strlen("SEND_SMS") ))
    {
        YX_Net_Receive_SEND_SMS(pMe,pContent,cont_len);
		YX_Net_Respond_SEND_SMS(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "SET_SMS_INTERCEPT", strlen("SET_SMS_INTERCEPT") ))
    {
        YX_Net_Receive_SET_SMS_INTERCEPT(pMe,pContent,cont_len);
        YX_Net_Respond_SET_SMS_INTERCEPT(pMe,water_str);

    }
    else if(0 == strncmp( (char *)mark_str, "SET_REGIONAL_ALARM", strlen("SET_REGIONAL_ALARM") ))
    {
        //设置区域报警
        YX_Net_Receive_SET_REGIONAL_ALARM(pMe,pContent,cont_len);
        YX_Net_Respond_SET_REGIONAL_ALARM(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "SET_MONITOR_REGIONAL", strlen("SET_MONITOR_REGIONAL") ))
    {
        //设置监控区域
	 YX_Net_Respond_SET_MONITOR_REGIONAL(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "SET_MONITOR_PLACE", strlen("SET_MONITOR_PLACE") ))
    {
        //设置监控场所
	 YX_Net_Respond_SET_MONITOR_PLACE(pMe,water_str);
    }
    else if(0 == strncmp( (char *)mark_str, "HDKT_SEND_QUESTION", strlen("HDKT_SEND_QUESTION") ))
    {
        //平台(教师)给学生证推送题目
	 YX_Net_Receive_HDKT_SEND_QUESTION(pMe,pContent,cont_len);
    }
    return 0;
}

static int YX_Net_Receive(YX_APP_T *pMe,uint8 * pData,uint32 Data_len)
{
    uint32 MsgLen = 0;
    char mark_str[31] = {0};
    char buf[201] = {0};
    uint8 * pStr = pData;
    uint32 len = Data_len;
    int ret = 0;
    if(pData == NULL || Data_len < 2)
    {
        ZDT_LOG("YX_Net_Receive Err NULL");
        return 0;
    }
    ZDT_LOG("YX_Net_Receive Data_len = %d,%s",Data_len,pData);
    MMIZDT_Net_Reset_Restore();
    if(pData[0] != YX_NET_MSG_START 
        || pData[Data_len-1] != YX_NET_MSG_END
        )
    {
        ZDT_LOG("YX_Net_Receive Err Start");
        return 0;
    }
    MsgLen = YX_Net_GetContentLen_RCV(pMe,pData,Data_len);
    if(MsgLen >= 0)
    {
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            return YX_Net_ReceiveHandle(pMe,mark_str,pData,Data_len,MsgLen);
        }
        
    }
}


static BOOLEAN YX_Net_Call_Start(YX_APP_T *pMe,char * number)
{
    uint8 i = 0;

    if(number == NULL)
    {
        return FALSE;
    }
    
    if(SCI_STRLEN(number) == 0)
    {
        return FALSE;
    }
    
    MMIZDT_MoCallByNum(MMIZDT_Net_GetActiveSys(),number);
    return TRUE;
}

static int YX_Net_HandleSendSuccess(YX_APP_T *pMe,uint8 * pData,uint32 Data_len)
{
    int ret = 0;
    uint8 * pOutData = NULL;
    int OutLen = 0;
    uint8 * pContent = NULL;
    uint32 content_len = 0;
    char mark_str[31] = {0};
    uint32 mark_len = 0;
    char buf[101] = {0};
    uint8 * pStr = PNULL;
    uint32 len = 0;
    uint16 alert_type = 0;
    
    OutLen = YX_Net_GetContentLen_Send(pMe,pData,Data_len,&pOutData,&content_len);
    if(OutLen > 0)
    {
        pStr = pOutData;
        len = OutLen;
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            mark_len = ret;
        }
        
        if(content_len > 0 && 0 == strncmp(mark_str,"ALARM_POWER",strlen("ALARM_POWER")))
        {
            pContent = pOutData+OutLen-content_len-1;
            ret = YX_Func_GetNextPara(&pContent, &content_len,buf,100);
            if(ret > 0)
            {
                alert_type = atoi(buf);
            }
        }
        SCI_FREE(pOutData);
    }

    ZDT_LOG("YX_Net_HandleSendSuccess %s",mark_str);
    if(0 == strncmp(mark_str,"DEVICE_LOGIN",strlen("DEVICE_LOGIN")))
    {
        ZDT_LOG("YX_Net_HandleSendSuccess--DEVICE_LOGIN--  is_land=%d",pMe->m_yx_is_land);
        YX_Net_Send_REPORT_HEARTBEAT(pMe);
        g_is_yx_first_reg_times = 0;
    }
    else if(0 == strncmp(mark_str,"SET_HEARTBEAT",strlen("SET_HEARTBEAT")))
    {
        YX_Net_Send_REPORT_HEARTBEAT(pMe);
    }
    else if(0 == strncmp(mark_str,"REPORT_HEARTBEAT",strlen("REPORT_HEARTBEAT")))
    {
        ZDT_LOG("YX_Net_HandleSendSuccess--REPORT_HEARTBEAT--  is_land=%d",pMe->m_yx_is_land);
        pMe->m_yx_lk_send_time = ZDT_Get_TimeSecond();
        pMe->m_yx_send_err_num = 0;
        g_yx_lk_timers_num++;
        if(g_yx_lk_timers_num > 0xFFF0)
        {
            g_yx_lk_timers_num = 1;
        }
        MMIZDT_Net_Reset_Restore();
        if(pMe->m_yx_is_land == FALSE)
        {
            YX_Net_Set_Land(TRUE);
            pMe->m_yx_is_land = TRUE;
            YX_LocReport_TimeStop(pMe);
            if(s_yx_is_first_land == FALSE)
            {
                s_yx_is_first_land = TRUE;
                YX_Net_Send_ALARM_POWER(pMe,4);
                g_yx_al_repeat_num = 0;	
                YX_Net_Send_SET_INCOMING_CALL_Reques(pMe);
                YX_Net_Send_SET_CLASS_MODEL_Reques(pMe);
                YX_Net_Send_SET_GET_NORMAL_BUTTON_Reques(pMe);
            }
            MMIRFID_CheckUpdate();
            if(FALSE == YX_DB_TIMER_IsValid_Sleep())
            {
                MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_LOCREPORT,NULL,0);
            }
            #if 0
            if(MMIZDT_GetNeedReset())
            {
                g_yx_is_handle_factory = FALSE;
                MMIZDT_NV_SetNeedReset(0);
                YX_Net_Respond_REMOTE_OPERATE_TERMINAL(pMe);
            }
            #endif
            YX_LocReport_TimeStart(pMe,g_yx_loc_timer_interval);
        }
        else
        {
            if(YX_LocReport_In_LK(g_yx_lk_timers_num))
            {
                if(YX_LocReport_JudgeSend())
                {
                    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_LOCREPORT,NULL,0);
                }
            }
        }
        pMe->m_yx_allow_rereg = TRUE;
        YX_AppCount_Check_Send();
        YX_Heart_TimeStart(pMe,g_yx_cur_heart_time);
    }
    else if(0 == strncmp(mark_str,"REPORT_LOCATION_INFO",strlen("REPORT_LOCATION_INFO")))
    {
        ZDT_LOG("YX_Net_HandleSendSuccess--REPORT_LOCATION_INFO-- time_interval=%d",yx_DB_Set_Rec.time_interval);
        //YX_LocReport_TimeStart(pMe,g_yx_loc_timer_interval);
    }
    else if(0 == strncmp(mark_str,"SET_SERVER_INFO",strlen("SET_SERVER_INFO")))
    {
        ZDT_LOG("YX_Net_HandleSendSuccess--SET_SERVER_INFO--  is_land=%d",pMe->m_yx_is_land);
        #ifndef WIN32
        SCI_Sleep(1000);
        #endif
        MMIZDT_Net_Reset(TRUE);
    }
    else if(0 == strncmp(mark_str,"ALARM_POWER",strlen("ALARM_POWER")))
    {
        ZDT_LOG("YX_Net_HandleSendSuccess--ALARM_POWER--  alert_type=%d",alert_type);
        if(alert_type == 3) //自动关机
        {
            //MMIAPICC_QuitForPowerOff();
            //MMIAPIPHONE_PowerOff();
        }
    }
    else if(0 == strncmp(mark_str,"FREQUENCY_LOCATION_SET",strlen("FREQUENCY_LOCATION_SET")))
    {
        ZDT_LOG("YX_Net_HandleSendSuccess--FREQUENCY_LOCATION_SET--  is_land=%d",pMe->m_yx_is_land);
        if(g_yx_is_handle_factory)
        {
            g_yx_is_handle_factory = FALSE;
            MMIZDT_Net_Reset(TRUE);
        }
    }
    else if(0 == strncmp(mark_str,"REQUEST_CALL",strlen("REQUEST_CALL")))
    {
        ZDT_LOG("YX_Net_HandleSendSuccess--REQUEST_CALL--  is_land=%d",pMe->m_yx_is_land);
        MMIZDT_Call_SOS();
    }
    else if(0 == strncmp(mark_str,"REMOTE_OPERATE_TERMINAL",strlen("REMOTE_OPERATE_TERMINAL")))
    {
        ZDT_LOG("YX_Net_HandleSendSuccess--REMOTE_OPERATE_TERMINAL--  is_land=%d",pMe->m_yx_is_land);
        if(g_yx_is_need_reset)
        {
            g_yx_is_need_reset = FALSE;
            //MMIZDT_NV_SetNeedReset(1);
            SCI_Sleep(10);
            //UserFile_RemoveDirFile("Photos");
            MMIAPIPHONE_PowerReset();
        }
    }
    return OutLen;
}

static int YX_Net_HandleSendFail(YX_APP_T *pMe,uint8 * pData,uint32 Data_len,uint16 err_id)
{
    int ret = 0;
    uint8 * pOutData = NULL;
    int OutLen = 0;
    uint8 * pContent = NULL;
    uint32 content_len = 0;
    char mark_str[31] = {0};
    uint32 mark_len = 0;
    char buf[101] = {0};
    uint8 * pStr = PNULL;
    uint32 len = 0;
    uint16 alert_type = 0;
    BOOLEAN is_land_fail = FALSE;
    
    OutLen = YX_Net_GetContentLen_Send(pMe,pData,Data_len,&pOutData,&content_len);
    if(OutLen > 0)
    {
        pStr = pOutData;
        len = OutLen;
        ret = YX_Func_GetCmdPosPara(&pStr, &len, mark_str,30,3);
        if(ret > 0)
        {
            mark_len = ret;
        }
        if(content_len > 0 && 0 == strncmp(mark_str,"ALARM_POWER",strlen("ALARM_POWER")))
        {
            pContent = pOutData+OutLen-content_len-1;
            ret = YX_Func_GetNextPara(&pContent, &content_len,buf,100);
            if(ret > 0)
            {
                alert_type = atoi(buf);
            }
        }
        SCI_FREE(pOutData);
    }
    
    ZDT_LOG("YX_Net_HandleSendFail err_id=%d, mark_str=%s",err_id,mark_str);
    
    if(0 == strncmp(mark_str,"DEVICE_LOGIN",strlen("DEVICE_LOGIN")))
    {
        if(err_id != TCP_ERR_CLEAR)
        {
            if(g_yx_is_need_reset_factory)
            {
                g_yx_is_need_reset_factory = FALSE;
                YX_Net_FactoryHaveIP(pMe);
            }
            else
            {
                g_is_yx_first_reg_times++;
                if(g_is_yx_first_reg_times >= 3)
                {
                    g_is_yx_first_reg = TRUE;
                    g_is_yx_first_reg_times = 0;
                }
            }
            MMIZDT_Net_Reset(FALSE);
        }
    }
    else if(0 == strncmp(mark_str,"REPORT_LOCATION_INFO",strlen("REPORT_LOCATION_INFO")))
    {
        ZDT_LOG("YX_Net_HandleSendSuccess--REPORT_LOCATION_INFO-- time_interval=%d",yx_DB_Set_Rec.time_interval);
        if(err_id != TCP_ERR_CLEAR)
        {
            //YX_LocReport_TimeStart(pMe,g_yx_loc_timer_interval);
        }
    }
    else if(0 == strncmp(mark_str,"REPORT_HEARTBEAT",strlen("REPORT_HEARTBEAT")))
    {
        ZDT_LOG("YX_Net_HandleSendFail(%d)--GET_SERVER_INFO--  is_land=%d,m_yx_send_err_num=%d",err_id,pMe->m_yx_is_land,pMe->m_yx_send_err_num);
        if(err_id != TCP_ERR_CLEAR && err_id != TCP_ERR_INCALL)
        {
            if(pMe->m_yx_is_land)
            {
                pMe->m_yx_is_land = FALSE;
                YX_Net_Set_Land(FALSE);
            }
            is_land_fail = TRUE;
            pMe->m_yx_send_err_num++;
            if(pMe->m_yx_send_err_num > 1)
            {
                if(pMe->m_yx_net_is_init)
                {
                    MMIZDT_Net_Reset(FALSE);
                }
            }
            YX_Heart_TimeStart(pMe,g_yx_cur_heart_time);
        }
        else if(err_id == TCP_ERR_INCALL)
        {
            YX_Heart_TimeStart(pMe,g_yx_cur_heart_time);
        }
        pMe->m_yx_allow_rereg = TRUE;
    }
    else if(0 == strncmp(mark_str,"ALARM_POWER",strlen("ALARM_POWER")))
    {
        ZDT_LOG("YX_Net_HandleSendFail--ALARM_POWER--  alert_type=%d",alert_type);
        if(err_id != TCP_ERR_CLEAR)
        {
            if(alert_type == 3) //自动关机
            {
                //MMIAPICC_QuitForPowerOff();
                //MMIAPIPHONE_PowerOff();
            }
        }
    }
    else if(0 == strncmp(mark_str,"FREQUENCY_LOCATION_SET",strlen("FREQUENCY_LOCATION_SET")))
    {
        ZDT_LOG("YX_Net_HandleSendSuccess--FREQUENCY_LOCATION_SET--  is_land=%d",pMe->m_yx_is_land);
        if(g_yx_is_handle_factory)
        {
            g_yx_is_handle_factory = FALSE;
            MMIZDT_Net_Reset(TRUE);
        }
    }
    else if(0 == strncmp(mark_str,"REMOTE_OPERATE_TERMINAL",strlen("REMOTE_OPERATE_TERMINAL")))
    {
        ZDT_LOG("YX_Net_HandleSendSuccess--REMOTE_OPERATE_TERMINAL--  is_land=%d",pMe->m_yx_is_land);
        if(g_yx_is_need_reset)
        {
            g_yx_is_need_reset = FALSE;
            //MMIZDT_NV_SetNeedReset(1);
            SCI_Sleep(10);
            //UserFile_RemoveDirFile("Photos");
            MMIAPIPHONE_PowerReset();
        }
    }
    
    if(TCP_ERR_NET != err_id && TCP_ERR_CLEAR != err_id && TCP_ERR_READ != err_id && TCP_ERR_INCALL != err_id)
    {
        pMe->m_yx_rsp_err_num++;
        if(pMe->m_yx_rsp_err_num >= 5)
        {
            ZDT_LOG("YX_Net_HandleSendFail RESET");
            if(is_land_fail == FALSE)
            {
                MMIZDT_Net_Reset(FALSE);
            }
            pMe->m_yx_rsp_err_num = 0;
        }
    }
    return OutLen;
}

static void YX_Land_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    YX_Net_Reg();
    return;
}

static void YX_Land_TimeStart(YX_APP_T *pMe)
{
    if(0 != pMe->m_yx_land_timer_id)
    {
        MMK_StopTimer(pMe->m_yx_land_timer_id);
        pMe->m_yx_land_timer_id = 0;
    }
    
    pMe->m_yx_land_timer_id = MMK_CreateTimerCallback(5000, 
                                                                        YX_Land_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    return;
}

static void YX_Land_TimeStop(YX_APP_T *pMe)
{
    if(0 != pMe->m_yx_land_timer_id)
    {
        MMK_StopTimer(pMe->m_yx_land_timer_id);
        pMe->m_yx_land_timer_id = 0;
    }
    
    return;
}

static int YX_Net_Land_CallBack(void * pUser,TCP_CB_TYPE_E cb_type,uint16 wParam, uint32 dwParam)
{
    int res = 0;
    ZDT_TCP_INTERFACE_T *pTcp = (ZDT_TCP_INTERFACE_T *)pUser;
    YX_APP_T *pMe = &g_yx_app;
    ZDT_LOG("YX_Net_Land_CallBack: pTcp=0x%xcb_type=%d,Result=%d,Data=0x%x",pTcp,cb_type,wParam,dwParam);

    
    switch(cb_type)
    {
        case TCP_CB_DISCONNECTED:
                ZDT_LOG("YX_Net TCP_CB_DISCONNECTED m_yx_is_land=%d",pMe->m_yx_is_land);
                if(pMe->m_yx_is_land)
                {
                    ZDT_LOG("YX_Net ReLand NET_DISCONNECTED--------------");
                    pMe->m_yx_is_land = FALSE;
                    YX_Net_Set_Land(FALSE);
                    MMIZDT_Net_Reset(FALSE);
                }
            break;
            
        case TCP_CB_REG:
            {
                uint16 ret = wParam;
                uint32 err_id =  dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //注册成功
                    ZDT_TCPTrace("YX_Net_Land_CallBack TCP_CB_REG OK --------------");
                    pMe->m_yx_is_reg = TRUE;
                    pMe->m_yx_is_land = FALSE;
                    YX_Net_Set_Land(FALSE);
                    //pMe->m_yx_send_err_num = 0;
                    pMe->m_yx_rsp_err_num = 0;
                    YX_Heart_TimeStop(pMe);
                    YX_LocReport_TimeStop(pMe);
                    g_yx_lk_timers_num = 0;
                    if(pMe->m_yx_net_first_reg == FALSE)
                    {
                        YX_Net_Send_DEVICE_LOGIN(pMe);
                        pMe->m_yx_net_first_reg = TRUE;
                    }
                    else
                    {
                        YX_Net_Send_DEVICE_LOGIN(pMe);
                    }
                    YX_Land_TimeStop(pMe);
                    
                }
                else
                {
                    //注册失败
                    ZDT_TCPTrace("YX_Net_Land_CallBack TCP_CB_REG ERR --------------");
                    YX_Heart_TimeStop(pMe);
                    YX_LocReport_TimeStop(pMe);
                    if(err_id == TCP_ERR_REGEXIST)
                    {
                        YX_Land_TimeStart(pMe);
                    }
                    else
                    {
                        pMe->m_yx_is_reg = FALSE;
                        pMe->m_yx_is_land = FALSE;
                        YX_Net_Set_Land(FALSE);
                    }
                }
            }
            break;
            
        case TCP_CB_UNREG:
            {
                uint16 ret = wParam;
                uint32 err_id =  dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //注销成功
                    ZDT_TCPTrace("YX_Net_Land_CallBack TCP_CB_UNREG OK --------------");
                    pMe->m_yx_is_land = FALSE;
                    YX_Net_Set_Land(FALSE);
                }
                else
                {
                    //注销失败
                    ZDT_TCPTrace("YX_Net_Land_CallBack TCP_CB_UNREG ERR --------------");
                }
            }
            break;
            
        case TCP_CB_SEND:
            {
                uint16 ret = wParam;
                ZDT_TCP_LINK_DATA_T * SendData = (ZDT_TCP_LINK_DATA_T*)dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //发送成功
                    ZDT_LOG("YX_Net_TCPSend Result Success Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        YX_Net_HandleSendSuccess(pMe,SendData->str,SendData->len);                        
                    }
                }
                else
                {
                    //发送失败
                    ZDT_LOG("YX_Net_TCPSend Result FAIL Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        YX_Net_HandleSendFail(pMe,SendData->str,SendData->len,ret);
                    }
                    
                }
            }
            break;
            
        case TCP_CB_RCV:
            {
                uint16 ret = wParam;
                ZDT_TCP_RCV_DATA_T * RcvData = (ZDT_TCP_RCV_DATA_T *) dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //接收成功
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        res = YX_Net_Receive(pMe,RcvData->pRcv,RcvData->len);
                    }
                    ZDT_LOG("YX_Net_Receive Over");
                }
                else
                {
                    //接收失败
                    ZDT_LOG("YX_Net_Receive FAIL");
                }
            }
            break;
                        
        default:
            break;
    }
    return res;
}

static int YX_Net_Reg_CallBack(void * pUser,TCP_CB_TYPE_E cb_type,uint16 wParam, uint32 dwParam)
{
    int res = 0;
    ZDT_TCP_INTERFACE_T *pTcp = (ZDT_TCP_INTERFACE_T *)pUser;
    YX_APP_T *pMe = &g_yx_app;
    ZDT_LOG("YX_Net_Reg_CallBack: pTcp=0x%xcb_type=%d,Result=%d,Data=0x%x",pTcp,cb_type,wParam,dwParam);
    
    switch(cb_type)
    {
        case TCP_CB_DISCONNECTED:
                ZDT_LOG("YX_Net_Reg_CallBack TCP_CB_DISCONNECTED");
                MMIZDT_Net_Reset(FALSE);
            break;
            
        case TCP_CB_REG:
            {
                uint16 ret = wParam;
                uint32 err_id =  dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //注册成功
                    ZDT_TCPTrace("YX_Net_Reg_CallBack TCP_CB_REG OK --------------");
                    YX_Net_Send_GET_SERVER_INFO(pMe);
                }
                else
                {
                    //注册失败
                    ZDT_TCPTrace("YX_Net_Reg_CallBack TCP_CB_REG ERR --------------");
                }
            }
            break;
            
        case TCP_CB_UNREG:
            {
                uint16 ret = wParam;
                uint32 err_id =  dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //注销成功
                    ZDT_TCPTrace("YX_Net_Reg_CallBack TCP_CB_UNREG OK --------------");
                }
                else
                {
                    //注销失败
                    ZDT_TCPTrace("YX_Net_Reg_CallBack TCP_CB_UNREG ERR --------------");
                }
            }
            break;
            
        case TCP_CB_SEND:
            {
                uint16 ret = wParam;
                ZDT_TCP_LINK_DATA_T * SendData = (ZDT_TCP_LINK_DATA_T*)dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //发送成功
                    ZDT_LOG("YX_Net_TCPSend Result Success Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                       MMIZDT_Net_Reset(TRUE);              
                    }
                }
                else
                {
                    //发送失败
                    ZDT_LOG("YX_Net_TCPSend Result FAIL Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        MMIZDT_Net_Reset(FALSE);    
                    }
                    
                }
            }
            break;
            
        case TCP_CB_RCV:
            {
                uint16 ret = wParam;
                ZDT_TCP_RCV_DATA_T * RcvData = (ZDT_TCP_RCV_DATA_T *) dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //接收成功
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        //res = YX_Net_Receive(pMe,RcvData->pRcv,RcvData->len);
                    }
                    ZDT_LOG("YX_Net_Receive Over");
                }
                else
                {
                    //接收失败
                    ZDT_LOG("YX_Net_Receive FAIL");
                }
            }
            break;
                        
        default:
            break;
    }
    return res;
}

int YX_Net_Rereg(YX_APP_T* pMe)
{
    if(FALSE == YX_Net_IsInit())
    {
        return 2;
    }

    if(pMe->m_yx_is_land == FALSE && pMe->m_yx_allow_rereg)
    {
        ZDT_LOG("YX_Net_Rereg %s:%d",yx_DB_Set_Rec.ip_domain,yx_DB_Set_Rec.ip_port);
        pMe->m_yx_allow_rereg = FALSE;
        YX_Net_Reset(pMe);
    }
    return 0;
}

int YX_Net_Reg(void)
{
    BOOLEAN res = FALSE;
    ZDT_LOG("YX_Net_Reg %s:%d",yx_DB_Set_Rec.ip_domain,yx_DB_Set_Rec.ip_port);
    if(FALSE == YX_Net_IsInit())
    {
        return ZDT_TCP_RET_ERR;
    }
    ZDT_API_TCP_UnReg(g_yx_app.m_yx_tcp_reg_interface);
    if(YX_DB_SET_HaveIP() && g_is_yx_first_reg == FALSE)
    {
        ZDT_API_TCP_Reg(g_yx_app.m_yx_tcp_reg_interface,(char *)yx_DB_Set_Rec.ip_domain,yx_DB_Set_Rec.ip_port,YX_Net_Land_CallBack);
    }
    else
    {
        if(g_is_yx_first_reg == TRUE)
        {
            g_is_yx_first_reg_times++;
            if(g_is_yx_first_reg_times >= 3)
            {
                g_is_yx_first_reg = FALSE;
                g_is_yx_first_reg_times = 0;
            }
        }
        res = YX_Net_HTTP_IP_Send();
        if(res == FALSE)
        {
            MMIZDT_Net_Reset(FALSE);
        }
    }
    return 0;
}

extern void YX_Net_CloseAllTimer(YX_APP_T *pMe)
{
    YX_Land_TimeStop(pMe);
    YX_Heart_TimeStop(pMe);
    YX_LocReport_TimeStop(pMe);
    YX_Find_TimeStop(pMe);
    return;
}

extern BOOLEAN YX_Net_IsAllow(YX_APP_T* pMe)
{
    if(yx_DB_Set_Rec.net_open &&  pMe->m_yx_net_is_init)
    {
        return TRUE;
    }
    ZDT_LOG("YX_Net_IsAllow FALSE");
    return FALSE;
}

extern BOOLEAN YX_Net_IsOpen(YX_APP_T* pMe)
{
    return pMe->m_yx_net_is_open;
}

extern int YX_Net_Open(YX_APP_T* pMe)
{
    ZDT_LOG("YX_Net_Open");
    pMe->m_yx_is_reg = FALSE;
    pMe->m_yx_is_land = FALSE;
    YX_Net_Set_Land(FALSE);
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    if(pMe->m_yx_voc_is_sending)
    {
        YX_Net_Send_TK_VocFile_End(pMe,YX_VOCSEND_ERR_UNREG);
        pMe->m_yx_voc_is_sending = FALSE;
    }
    else
    {
        YX_VOC_Send_Link_DelAll(pMe);
    }
#endif
    YX_Net_Reg();
    pMe->m_yx_net_is_open = TRUE;
    return 0;
}

extern int YX_Net_Close(YX_APP_T* pMe)
{
    ZDT_LOG("YX_Net_Close");
    pMe->m_yx_is_reg = FALSE;
    pMe->m_yx_is_land = FALSE;
    YX_Net_Set_Land(FALSE);
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    if(pMe->m_yx_voc_is_sending)
    {
        YX_Net_Send_TK_VocFile_End(pMe,YX_VOCSEND_ERR_UNREG);
        pMe->m_yx_voc_is_sending = FALSE;
    }
    else
    {
        YX_VOC_Send_Link_DelAll(pMe);
    }
#endif
    ZDT_API_TCP_UnReg(g_yx_app.m_yx_tcp_reg_interface);
    pMe->m_yx_net_is_open = FALSE;
   return 0;
}

extern int YX_Net_Init(void)
{
    YX_APP_T * pMe = &g_yx_app;
    ZDT_LOG("YX_Net_Init");
    g_yx_power_on_time_min = ZDT_Get_TimeSecond();
    g_yx_cur_heart_time = yx_DB_Set_Rec.heart_interval;
    if(ZdtTCP_NetOpen(&(g_yx_app.m_yx_tcp_reg_interface),YX_CMD_CheckAll,10240))
    {
        ZDT_LOG("YX_Net_Init Reg OK pMe=0x%x",g_yx_app.m_yx_tcp_reg_interface);
    }

    if(MMIZDT_CheckImeiChange())
    {
        ZDT_DB_DelAll();
        YX_DB_DelAll(pMe);
        MMIZDT_SaveImei();
        MMIZDT_NVClearRFID();
    }
    
    pMe->m_yx_net_is_init = TRUE;
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    //YX_Voice_FileSendAllRemove(pMe);
#endif
    YX_LocReport_TimeSet(yx_DB_Set_Rec.time_interval);
#if 0
    if(YX_Net_IsAllow(pMe))
    {
        YX_Net_Open(pMe);
    }
#endif
    return 0;
}

BOOLEAN YX_Net_IsInit(void)
{
    return g_yx_app.m_yx_net_is_init;
}

extern int YX_Net_Reset(YX_APP_T* pMe)
{
    ZDT_LOG("YX_Net_Reset");
    if(FALSE == YX_Net_IsInit())
    {
        return 1;
    }
    if(YX_Net_IsAllow(pMe))
    {
        YX_Net_Open(pMe);
    }
    else
    {
        YX_Net_Close(pMe);
    }
    return 0;
}

extern int YX_Net_TimerCheck_WT(SCI_DATE_T dt, SCI_TIME_T tm)
{
    if(g_yx_wt_oneday_start == 0)
    {
        if(tm.hour>= 6)
        {
            g_yx_wt_recieve_ok = FALSE;
            g_yx_wt_oneday_start = 1;
        }
    }
    else if(g_yx_wt_oneday_start == 1)
    {
        if(tm.hour >= 12)
        {
            g_yx_wt_recieve_ok = FALSE;
            g_yx_wt_oneday_start = 2;
        }
    }
    else if(g_yx_wt_oneday_start == 2)
    {
        if(tm.hour >= 18)
        {
            g_yx_wt_recieve_ok = FALSE;
            g_yx_wt_oneday_start = 3;
        }
    }
    return 0;
}

static int YX_Net_TimerCheck_WL(void)
{
    BOOLEAN need_send = FALSE;
   if(YX_DB_REGIONAL_IsTimerValid_All())
   {
        s_yx_wl_check_times++;
        if(s_yx_wl_check_times >= 0xFFFFFFF0)
        {
            s_yx_wl_check_times = 0;
        }
        ZDT_LOG("YX_Net_TimerCheck WL Valid time_interval=%d,s_yx_is_in_wl=%d",g_yx_wl_interval_min,s_yx_is_in_wl_time);
        if(s_yx_is_in_wl_time == FALSE)
        {
            need_send = TRUE;
            s_yx_is_in_wl_time = TRUE;
        }
        else
        {
            if(g_yx_wl_interval_min == 0)
            {
                need_send = TRUE;
            }
            else if(0 == (s_yx_wl_check_times%g_yx_wl_interval_min))
            {
                need_send = TRUE;
            }
        }
        if(need_send)
        {
            YX_API_LOCTION_WL_Start();
        }
   }
   else
   {
        if(s_yx_is_in_wl_time)
        {
            s_yx_wl_check_times = 0;
            s_yx_is_in_wl_time = FALSE;
        }
   }
    return 0;
}

extern int YX_Net_TimerCheck(SCI_DATE_T dt, SCI_TIME_T tm)
{
    static uint32 last_reset_tick = 0;
    YX_APP_T* pMe = &g_yx_app;
    uint32 cur_time = ZDT_Get_TimeSecond();
    uint32 last_time = pMe->m_yx_lk_send_time;
    uint32 cur_tick = SCI_GetTickCount();
    
    YX_Net_TimerCheck_WT(dt,tm);
    YX_Net_TimerCheck_WL();
    if(last_time == 0)
    {
        if(ZDT_SIM_Exsit() 
            && YX_Net_IsAllow(pMe) 
            && ((MMIZDT_Net_PdpStateGet() == FALSE) || ((cur_tick - last_reset_tick) > (yx_DB_Set_Rec.heart_interval*2*1000 + 120000)))
            )
        {
            ZDT_LOG("YX_Net_TimerCheck Err IN  PDP");
            last_reset_tick = cur_tick;
            MMIZDT_Net_Reset(FALSE);
        }
    }
    else
    {
        //判断网络连接是否出错当大于20分钟没有发送心跳包时网络重启
        if(ZDT_SIM_Exsit() && YX_Net_IsAllow(pMe) && last_time != 0 && (cur_time - last_time) > (yx_DB_Set_Rec.heart_interval*2 + 120))
        {
            ZDT_LOG("YX_Net_TimerCheck Err IN  LK");
            pMe->m_yx_lk_send_time = last_time;
            last_reset_tick = cur_tick;
            MMIZDT_Net_Reset(FALSE);
        }
    }
    
    return 0;
}

PUBLIC BOOLEAN YX_IsInClassSilentTime()
{
    BOOLEAN ret = FALSE;
    uint16 i = 0;
    for(i = YX_TIME_TYPE_CLASS_1; i < YX_TIME_TYPE_CLASS_MAX; i++)
    {
       ret = YX_DB_TIMER_IsValid(i);
       if(ret)
       {
            break;
       }

    }
    ZDT_LOG("YX_IsInClassSilentTime ret=%d", ret);
    return ret;
    
}

PUBLIC void YX_CheckClassSilentOnOff()
{
    static BOOLEAN s_device_is_in_class_mode = FALSE;
    ZDT_LOG("YX_CheckClassSilentOnOff");
    if(YX_IsInClassSilentTime())
    {
        POWER_RESTART_CONDITION_E   start_condition = POWER_GetRestartCondition();
        MMIAPIENVSET_ActiveMode(MMIENVSET_SILENT_MODE);
        if (!MMIAPIKL_IsPhoneLocked() && 
            (!MMIAPICC_IsInState(CC_IN_CALL_STATE)) &&
            ((RESTART_BY_ALARM != start_condition) && (RESTART_BY_CHARGE != start_condition))
#ifndef MMI_PDA_SUPPORT
            &&(!MMIAPIALM_IsAliveWinOpened())//闹铃，倒计时提示界面均不允许锁键
            &&(!MMIAPICT_IsArriveWinOpened())//
#endif
#ifdef WRE_SUPPORT
            &&(!MMIWRE_Actived())//WRE运行时不允许锁键
#endif
#ifdef MRAPP_SUPPORT
            &&(!MMIMRAPP_mrappIsActived())
#endif
#ifdef BROWSER_SUPPORT 
            &&(!MMIAPIBROWSER_IsRunning())
#endif
#ifdef JAVA_SUPPORT 
            &&(!MMIAPIJAVA_IsForegroundRuning())
#endif

            )
        {
            MMIAPIKL_LockPhone();
        }
        else
        {
            if(MMIAPIKL_IsPhoneLocked())
            {
                if (MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID))
                {
                    MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID, MSG_FULL_PAINT, PNULL,0);
                }
            }
        }
        if(s_device_is_in_class_mode == FALSE)
        {
            s_device_is_in_class_mode = TRUE;
            if(TRUE == MMIAPICC_YxCall_NeedDisconnect())
            {
                MMIAPICC_ReleaseCallByRedkey();
            }
        }
    }
    else
    {
        MMIAPIENVSET_ActiveMode(MMIENVSET_STANDARD_MODE);
        if(s_device_is_in_class_mode == TRUE)
        {
            s_device_is_in_class_mode = FALSE;
        }
    }
}


extern int YX_API_Net_Reset(void)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_NET_RESET,NULL,0);
    return 0;
}

extern int YX_API_CZSB_Stop(void)
{
    if(g_is_yx_czsb_is_playing)
    {
        ZdtWatch_Factory_Speaker_Stop();
        g_is_yx_czsb_is_playing = FALSE;
    }
    return 0;
}

extern int YX_API_SeNetOpen(void)
{
    if(yx_DB_Set_Rec.net_open == 0)
    {
        yx_DB_Set_Rec.net_open = 1;
        YX_DB_SET_ListModify();
        MMIZDT_Net_Reset(TRUE);
        return 1;
    }
    return 0;
}

extern int YX_API_SeNetClose(void)
{
    if(yx_DB_Set_Rec.net_open > 0)
    {
        yx_DB_Set_Rec.net_open = 0;
        YX_DB_SET_ListModify();
        MMIZDT_Net_Reset(TRUE);
        return 1;
    }
    return 0;
}

extern int YX_API_WT_Send(void)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_WT,NULL,0);
    return 0;
}

extern int YX_API_WT_Reset(void)
{
    g_yx_wt_oneday_start = 0;
    return 0;
}


int MMIRFID_HTTP_Rcv_Handle(uint8 * pContent,uint32 ContentLen)
{
    int ret = -1;
    char *msg_char;
    uint32 msg_len = 0;
    int val = -1;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    cJSON *data_json = PNULL;
    char rfid_str[ZDT_RFID_LEN+1] = {0};
    if(pContent == PNULL || ContentLen < 2)
    {
        return 0;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"code");
        if(item_json)
        {
            val = cJSON_GetObjectInt(item_json);
            ZDT_LOG("code = %d",val);
        }
        else
        {
            ZDT_LOG("code NONE");
        }
        
        if(val == 0)
        {
            data_json = cJSON_GetObjectItem(json,"data");
            if(data_json)
            {
                item_json = cJSON_GetObjectItem(data_json,"rfid");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("rfid=%s",msg_char);
                        strcpy(rfid_str,msg_char);
                        if(MMIZDT_IsRFIDChanged(rfid_str))
                        {
                            MMIZDT_NVSetRFID(rfid_str);
                        }
                        MMIAPISET_UpdateWatchInfoWin();
                    }
                }
                else
                {
                    ZDT_LOG("rfid NONE");
                }
            }
            else
            {
                ZDT_LOG("data NONE");
            }
        }

        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("json NONE");
    }
    return ret;
}

int MMIRFID_HTTP_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    int ret = -1;
    ZDT_LOG("MMIRFID_HTTP_Send_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(pRcv != NULL && Rcv_len > 2)
    {
        ret = MMIRFID_HTTP_Rcv_Handle(pRcv,Rcv_len);
    }
    return ret;
}

//http://112.5.176.43:19990/dzbp/api/dtDeviceInfo/getDtDeviceByImei?imei=869718069000021&version=1.0.1&forceUpdate=true&otaUpdate=true&type=ZC101
BOOLEAN  MMIRFID_HTTP_Send(void)
{
    BOOLEAN res = FALSE;
    char app_domain[100] = {0};
    char content_buf[300] = {0};
    uint16 content_len = 0;
    uint32 ms = 1000;

    ZDT_LOG("MMIRFID_HTTP_Send");
    if(FALSE == ZDT_SIM_Exsit())
    {
        ZDT_LOG("MMIRFID_HTTP_Send Err NO SIM");
        return FALSE;
    }
    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        ZDT_LOG("MMIRFID_HTTP_Send Err NO Net");
        return FALSE;
    }
    sprintf(content_buf,"imei=%s&version=1.0.1&forceUpdate=true&otaUpdate=true&type=ZC101",g_zdt_phone_imei);
    content_len = strlen(content_buf);
    sprintf(app_domain,"http://112.5.176.43:19990/dzbp/api/dtDeviceInfo/getDtDeviceByImei?");
    res = MMIZDT_HTTP_AppSend(TRUE,app_domain,(uint8 *)content_buf,content_len,2000,0,0,0,0,0,MMIRFID_HTTP_Send_CB);
    return res;
}

BOOLEAN  MMIRFID_CheckSend(void)
{
    if(ZDT_SIM_Exsit())
    {
        //MMIRFID_HTTP_Send();  //ZC100不能获取到ID
        return TRUE;
    }
    return FALSE;
}

BOOLEAN  MMIRFID_CheckUpdate(void)
{
    if(strlen(g_zdt_phone_rfid.num) == 0)
    {
        //MMIRFID_HTTP_Send();
        return TRUE;
    }
    return FALSE;
}
BOOLEAN MMIYX_SetNetOpen(void)
{
    YX_API_SeNetOpen();
    return 0;
}

BOOLEAN MMIYX_SetNetClose(void)
{
   YX_API_SeNetClose();
    return 0;
}

BOOLEAN MMIYX_UnBindDevice(void)
{
    return 0;
}

#endif
