
#include "zdt_app.h"

#ifdef ZDT_PLAT_HYHL_SUPPORT
#include "zdt_hyhl.h"

#define HYHL_CMD_START                                            0x55                              
#define HYHL_CMD_START2                                               0x55
#define HYHL_CMD_END                                            0x0D
#define HYHL_CMD_END2                                               0x0A
#define HYHL_HEAD_LEN 5
#define HYHL_DATA_MAX_LEN 2048

static uint8 hyhl_cmd_line_buffer[HYHL_DATA_MAX_LEN];     /*  buffer for AT cmd line  */
static uint16 hyhl_cmd_line_len = 0;
static uint16 hyhl_data_need_len = 0;
static uint8 *hyhl_cmd_ptr;                     /* next loc in buffer       */
static uint8 hyhl_cmd_status = 0;
BOOLEAN  HYHL_CMD_ProcessOneChar(uint8 cc)
{
    BOOLEAN res = FALSE;
    switch ( hyhl_cmd_status )
    {
        case 0:
            /*-----------------------------------------------------------------
              In the HUNT state, the characters 'A' or 'a' are looked for.  
              Everything else is ignored. If 'A' or 'a' is detected, the   
              machine goes into the FOUND_A state.                              
            -----------------------------------------------------------------*/
            if (cc == HYHL_CMD_START)
            {
                hyhl_cmd_ptr = &hyhl_cmd_line_buffer[0];
                *hyhl_cmd_ptr++ = cc;
                hyhl_cmd_line_len = 1;
                hyhl_cmd_status = 1;
                hyhl_data_need_len = 0;
            }
            break;

        case 1: 
            {
                //HYHL
                if ( cc == HYHL_CMD_START2 )
                {
                    *hyhl_cmd_ptr++ = cc;
                    hyhl_cmd_line_len++;
                    hyhl_cmd_status = 2;
                    hyhl_data_need_len = HYHL_HEAD_LEN;
                }
                else
                {
                    hyhl_cmd_status = 0;        
                }
            }
            break;
            
        case 2:
            {
                if ( hyhl_cmd_ptr >= (hyhl_cmd_line_buffer + HYHL_DATA_MAX_LEN - 2) )
                {
                    hyhl_cmd_status = 0;
                    hyhl_cmd_ptr[0] = '\0';
                    break;
                }
                /*------------------------------------------------------------
                  If no overflow, fill buffer.  
                ------------------------------------------------------------*/
                else
                {
                    *hyhl_cmd_ptr++ = cc;
                    hyhl_cmd_line_len++;
                }
                
                if(hyhl_cmd_line_len == HYHL_HEAD_LEN)
                {                                
                    uint16 casic_data_len = 0;
                    casic_data_len = hyhl_cmd_line_buffer[2];
                    casic_data_len = casic_data_len << 8;
                    casic_data_len += hyhl_cmd_line_buffer[3];
                    //A2 AGPS消息是个例外
                    if(hyhl_cmd_line_buffer[4] == 0xA2)
                    {
                        casic_data_len +=2;
                    }
                    hyhl_data_need_len = casic_data_len + 6;
                    if ( hyhl_data_need_len > HYHL_DATA_MAX_LEN )
                    {
                        hyhl_cmd_status = 0;
                        hyhl_cmd_ptr[0] = '\0';
                        break;
                    }
                }
                else
                {
                    if(hyhl_cmd_line_len >= hyhl_data_need_len)
                    {
                        if(hyhl_cmd_line_buffer[hyhl_cmd_line_len-2] == HYHL_CMD_END && hyhl_cmd_line_buffer[hyhl_cmd_line_len-1] == HYHL_CMD_END2)
                        {
                            res = TRUE;
                        }
                        else
                        {
                            //TCP_LOG("TCP HYHL CMD(%d) ERR End 0x%x,0x%x",hyhl_data_need_len,hyhl_cmd_line_buffer[hyhl_cmd_line_len-2],hyhl_cmd_line_buffer[hyhl_cmd_line_len-1]);
                        }
                        hyhl_cmd_status = 0;
                    }
                }
            }

            break;
    }  /* end switch ( at_cmd_prep_state ) */

    return res;
}

static void HYHL_CMD_Reset(void)
{
    hyhl_cmd_status = 0;
    return;
}

static uint16 HYHL_CMD_Check(uint8 * pData, uint16 Data_len)
{
    uint16 ContentLen = 0;
    
    if(pData == NULL || Data_len < 11)
    {
        return 0;
    }
    
    if(pData[0] != HYHL_CMD_START || pData[1] != HYHL_CMD_START2)
    {
        return 0;
    }

    ContentLen = pData[2];
    ContentLen = ContentLen<<8;
    ContentLen += pData[3];
    
    //A2 AGPS消息是个例外
    if(pData[4] == 0xA2)
    {
        ContentLen += 2;
    }

    if(ContentLen < 5 || (ContentLen+6) > Data_len)
    {
        return 0;
    }
    
    
    if(HYHL_CMD_END != pData[ContentLen+4] || HYHL_CMD_END2 != pData[ContentLen+5])
    {
        return 0;
    }
        
    return ContentLen+6;
}

static uint16 HYHL_CMD_CheckAll(uint8 * pData, int Data_len,uint8 **ppCmdBuf, uint16 * cmd_len)
{
    uint16 end_len = 0;
    uint16 i = 0;
    BOOLEAN res = FALSE;
    if(pData == PNULL || Data_len <= 0)
    {
        return 0;
    }
    end_len = HYHL_CMD_Check(pData,Data_len);
    if(end_len > 0)
    {
         HYHL_CMD_Reset();
        *ppCmdBuf = pData;
        *cmd_len = end_len;
        return end_len;
    }
    else
    {
        for(i = 0; i < Data_len; i++)
        {
            if(HYHL_CMD_ProcessOneChar(pData[i]))
            {
                *ppCmdBuf = &hyhl_cmd_line_buffer[0];
                *cmd_len = hyhl_cmd_line_len;
                end_len = i+1;
                break;
            }
        }
    }
    return end_len;
}

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
    uint32 altitude;    //海拔高度
    uint8 alt_Dir;          //海拔方向
} GPS_DATA_T;
#endif

#ifndef ONE_DAY_SECONDS
#define ONE_DAY_SECONDS		86400
#endif

int HYHL_Net_Reg(void);
static int HYHL_Net_Land(uint32 ip_addr,uint16 port);

uint16 g_hyhl_net_msg_index = 0;
BOOLEAN g_is_hyhl_test_server = FALSE;
BOOLEAN g_is_hyhl_server_fixed = FALSE;
BOOLEAN g_is_hyhl_czsb_is_playing = FALSE;
BOOLEAN g_is_hyhl_power_off_ing = FALSE;

int HYHL_Net_GetContentLen(uint8 * pData,uint32 Data_len)
{
    uint8 protocol_id;
    uint16 MsgLen = 0;
    
    if(pData == NULL || Data_len < 11)
    {
        ZDT_LOG("HYHL_Net_GetContentLen Err NULL");
        return -1;
    }
    
    if(pData[0] != HYHL_NET_MSG_START_1 || pData[1] != HYHL_NET_MSG_START_2)
    {
        ZDT_LOG("HYHL_Net_GetContentLen Err Start");
        return -1;
    }

    MsgLen = pData[2];
    MsgLen = MsgLen<<8;
    MsgLen += pData[3];
    
    if(MsgLen < 5 || (MsgLen+6) > Data_len)
    {
        ZDT_LOG("HYHL_Net_GetContentLen Err Len %d>%d",(MsgLen+6),Data_len);
        return -1;
   }
    
    protocol_id = pData[4];
        
    return (int)(MsgLen-5);
}

int HYHL_Net_GetProtocol(uint8 * pData,uint32 Data_len)
{
    uint8 protocol_id;
    uint16 MsgLen = 0;
    
    if(pData == NULL || Data_len < 11)
    {
        ZDT_LOG("HYHL_Net_GetProtocol Err NULL");
        return -1;
    }
    
    if(pData[0] != HYHL_NET_MSG_START_1 || pData[1] != HYHL_NET_MSG_START_2)
    {
        ZDT_LOG("HYHL_Net_GetProtocol Err Start");
        return -1;
    }

    MsgLen = pData[2];
    MsgLen = MsgLen<<8;
    MsgLen += pData[3];
    
    if(MsgLen < 5 || (MsgLen+6) > Data_len)
    {
        ZDT_LOG("HYHL_Net_GetProtocol Err Len %d>%d",(MsgLen+6),Data_len);
        return -1;
   }
    
    protocol_id = pData[4];
    
    return (int)protocol_id;
}

extern uint32 HYHL_Net_TCPSend(ZDT_TCP_INTERFACE_T *pMe,uint8 protocol_id,uint8 * pContent,uint16 ContentLen,uint8 priority,uint8 repeat_num,uint32 timeout,TCPRCVHANDLER rcv_handle)
{
/*
    格式 长度(uint8)
    起始位 2    (0x55 0x55)
    包长度 2
    协议号 1
    内容: N     终端 ID 15
    信息序列号 2
    错误校验码 2
    结束位 2
*/

    ZDT_TCP_LINK_DATA_T * cur_data = NULL;
    uint16 len = 0;
    uint16 msg_len = 0;
    uint16 crc = 0;
    
    if(ContentLen == 0 || pContent == NULL )
    {
        ZDT_LOG("HYHL_Net_TCPSend: ERR NULL");
        return 0;
    }

    cur_data=(ZDT_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(ZDT_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        ZDT_LOG("HYHL_Net_TCPSend: ERR Malloc");
        return 0;
    }

    cur_data->str= (uint8 *)SCI_ALLOC_APPZ(ContentLen +12);
    if(cur_data->str == NULL)
    {
        ZDT_LOG("HYHL_Net_TCPSend: ERR Malloc len=%d",ContentLen+12);
        SCI_FREE(cur_data);
        return 0;
    }
    
    SCI_MEMSET(cur_data->str,0,ContentLen+12);
    //起始位
    cur_data->str[0] = HYHL_NET_MSG_START_1;
    cur_data->str[1] = HYHL_NET_MSG_START_2;
    
    //包长度
    msg_len = ContentLen + 5;
    cur_data->str[2] = msg_len>>8;
    cur_data->str[3] = msg_len & 0x00FF;
    
    //协议号
    cur_data->str[4] = protocol_id;
    
    //数据
    SCI_MEMCPY(cur_data->str+5,pContent,ContentLen);
    len = ContentLen + 5;
    
    //信息序列号
    g_hyhl_net_msg_index++;
    if(g_hyhl_net_msg_index >= 0xFFFF)
    {
        g_hyhl_net_msg_index = 1;
    }
    cur_data->str[len++] = g_hyhl_net_msg_index >> 8;
    cur_data->str[len++] = g_hyhl_net_msg_index & 0x00FF;
    
    //校验码
    crc = HYHL_Func_GetCrc16(cur_data->str+4,msg_len-2);
    cur_data->str[len++] = crc>>8;
    cur_data->str[len++] = crc&0x00FF;
    
    //结束位
    cur_data->str[len++] = HYHL_NET_MSG_END_1;
    cur_data->str[len++] = HYHL_NET_MSG_END_2;

    cur_data->pMe = pMe;
    cur_data->len = len;
    cur_data->times = repeat_num;
    cur_data->timeout = timeout;
    cur_data->priority = priority;
    cur_data->rcv_handle = rcv_handle;
    ZDT_TCP_PostEvtTo_APP(ZDT_APP_SIGNAL_TCP_WRITE_ADD,cur_data,len);
    
    ZDT_LOG("HYHL_Net_TCPSend Handle=0x%x",(uint32)cur_data);
    return (uint32)cur_data;
}

extern uint32 HYHL_Net_TCPSendNow(ZDT_TCP_INTERFACE_T* pMe,uint8 protocol_id,uint8 * pContent,uint16 ContentLen,uint8 priority,uint8 repeat_num,uint32 timeout,TCPRCVHANDLER rcv_handle)
{
/*
    格式 长度(uint8)
    起始位 2    (0x55 0x55)
    包长度 2
    协议号 1
    内容: N     终端 ID 15
    信息序列号 2
    错误校验码 2
    结束位 2
*/
    ZDT_TCP_LINK_DATA_T * cur_data = NULL;
    uint16 len = 0;
    uint16 msg_len = 0;
    uint16 crc = 0;
    
    if(ContentLen == 0 || pContent == NULL )
    {
        ZDT_LOG("HYHL_Net_TCPSendNow: ERR NULL");
        return 0;
    }

    cur_data=(ZDT_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(ZDT_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        ZDT_LOG("HYHL_Net_TCPSendNow: ERR Malloc");
        return 0;
    }

    cur_data->str= (uint8 *)SCI_ALLOC_APPZ(ContentLen +12);
    if(cur_data->str == NULL)
    {
        ZDT_LOG("HYHL_Net_TCPSendNow: ERR Malloc len=%d",ContentLen+12);
        SCI_FREE(cur_data);
        return 0;
    }
    
    SCI_MEMSET(cur_data->str,0,ContentLen+12);
    //起始位
    cur_data->str[0] = HYHL_NET_MSG_START_1;
    cur_data->str[1] = HYHL_NET_MSG_START_2;
    
    //包长度
    msg_len = ContentLen + 5;
    cur_data->str[2] = msg_len>>8;
    cur_data->str[3] = msg_len & 0x00FF;
    
    //协议号
    cur_data->str[4] = protocol_id;
    
    //数据
    SCI_MEMCPY(cur_data->str+5,pContent,ContentLen);
    len = ContentLen + 5;
    
    //信息序列号
    g_hyhl_net_msg_index++;
    if(g_hyhl_net_msg_index >= 0xFFFF)
    {
        g_hyhl_net_msg_index = 1;
    }
    cur_data->str[len++] = g_hyhl_net_msg_index >> 8;
    cur_data->str[len++] = g_hyhl_net_msg_index & 0x00FF;
    
    //校验码
    crc = HYHL_Func_GetCrc16(cur_data->str+4,msg_len-2);
    cur_data->str[len++] = crc>>8;
    cur_data->str[len++] = crc&0x00FF;
    
    //结束位
    cur_data->str[len++] = HYHL_NET_MSG_END_1;
    cur_data->str[len++] = HYHL_NET_MSG_END_2;
    
    cur_data->len = len;
    cur_data->times = repeat_num;
    cur_data->timeout = timeout;
    cur_data->priority = priority;
    cur_data->rcv_handle = rcv_handle;
    ZDT_API_TCP_SendDataNow(pMe,cur_data);
    return (uint32)cur_data;
}

static int HYHL_Net_1A_SendCB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
/*
    服务器地址 4
    端口 2
*/
    HYHL_APP_T *pMe = &g_hyhl_app;
    uint8 * pContent;
    int ContentLen = HYHL_Net_GetContentLen(pRcv,Rcv_len);
    int protocol_id = HYHL_Net_GetProtocol(pRcv,Rcv_len);
    if(protocol_id == 0x1A && ContentLen == 6)
    {
        pContent = pRcv+5;
        pMe->m_hyhl_reg_ip = pContent[3];
        pMe->m_hyhl_reg_ip = pMe->m_hyhl_reg_ip << 8;
        pMe->m_hyhl_reg_ip += pContent[2];
        pMe->m_hyhl_reg_ip = pMe->m_hyhl_reg_ip << 8;
        pMe->m_hyhl_reg_ip += pContent[1];
        pMe->m_hyhl_reg_ip = pMe->m_hyhl_reg_ip << 8;
        pMe->m_hyhl_reg_ip += pContent[0];
        
        pMe->m_hyhl_reg_port = pContent[4] << 8;
        pMe->m_hyhl_reg_port += pContent[5];
        pMe->m_hyhl_is_reg = TRUE;
        ZDT_LOG("HYHL_Net_1A_SendCB  ip=0x%x,%d.%d.%d.%d, Port=%d",pMe->m_hyhl_reg_port,pContent[0],pContent[1],pContent[2],pContent[3],pMe->m_hyhl_reg_port);
        return ZDT_TCP_RET_OK;
    }
    ZDT_LOG("HYHL_Net_1A_SendCB  ERR");
    return ZDT_TCP_RET_ERR;
}

uint32 HYHL_Net_1A_Send(HYHL_APP_T *pMe,uint8 repeat_num,uint32 timeout)
{
/* 注册
    终端 ID 15
*/
    uint32 send_hande = 0;
    send_hande = HYHL_Net_TCPSend(pMe->m_hyhl_tcp_reg_interface,0x1A,(uint8*)g_zdt_phone_imei,ZDT_IMEI_LEN,200,repeat_num,timeout,HYHL_Net_1A_SendCB);
    ZDT_LOG("HYHL_Net_1A_Send Handle=0x%x",send_hande);
    return send_hande;
}

static int HYHL_Net_A0_SendCB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    /*
        空
    */
    uint8 * pContent;
    int ContentLen = HYHL_Net_GetContentLen(pRcv,Rcv_len);
    int protocol_id = HYHL_Net_GetProtocol(pRcv,Rcv_len);
    
    if(protocol_id == 0xA0 && ContentLen >= 0)
    {
        ZDT_LOG("HYHL_Net_A0_SendCB Success");
        return ZDT_TCP_RET_OK;
    }
    ZDT_LOG("HYHL_Net_A0_SendCB  ERR");
    return ZDT_TCP_RET_ERR;
}

uint32 HYHL_Net_A0_Send(HYHL_APP_T *pMe,uint8 repeat_num,uint32 timeout)
{
/* 登陆
    终端 ID 15
    时区语言 2
    设备版本号 15
*/
    uint8 send_buf[51] = {0};
    uint32 send_hande = 0;
    uint16 send_len = 0;
    SCI_MEMCPY(send_buf,g_zdt_phone_imei,ZDT_IMEI_LEN);
    send_len = ZDT_IMEI_LEN;
    //时区语言东八区
    send_buf[send_len++] = 0x32;
    send_buf[send_len++] = 0x00;
    SCI_MEMCPY(send_buf+send_len, "V1.1.1.20151014",15);
    send_len += 15;
    send_hande = HYHL_Net_TCPSend(pMe->m_hyhl_tcp_land_interface,0xA0,send_buf,send_len,200,repeat_num,timeout,HYHL_Net_A0_SendCB);
    ZDT_LOG("HYHL_Net_A0_Send Handle=0x%x",send_hande);
    return send_hande;
}

static int HYHL_Net_A3_SendCB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    /*
        空
    */
    uint8 * pContent;
    int ContentLen = HYHL_Net_GetContentLen(pRcv,Rcv_len);
    int protocol_id = HYHL_Net_GetProtocol(pRcv,Rcv_len);

    if(protocol_id == 0xA3 && ContentLen >= 0)
    {
        ZDT_LOG("HYHL_Net_A3_SendCB Success");
        return ZDT_TCP_RET_OK;
    }
    ZDT_LOG("HYHL_Net_A3_SendCB  ERR");
    return ZDT_TCP_RET_ERR;
}

uint32 HYHL_Net_A3_Send(HYHL_APP_T* pMe,uint8 loc_ok,uint16 type)
{
/* 报警包
    终端 ID 15
    日期时间 6
    报警类型 2
    GPS 信息包 gps 信息卫星数 1
    纬度 4
    经度 4
    速度 1
    航向，状态 2
    状态信息包 终端信息内容 1
    电压等级 1
    信号强度等级 1
    WIFI 信息 WIFI 数量 1
    AP 地址 6
    信号 1
    LBS 信息 LBS 数量 1
    LBS 信号强度 1
    PN(MCC) 2
    SID(MNC) 2
    NID(LAC) 2
    BID(CELLID) 3
    纬度 4
    经度 4
*/    

    uint8 send_buf[200] = {0};
    uint16 send_len = 0;
    uint32 send_hande = 0;
    GPS_DATA_T last_gps = {0};
    
    if(pMe->m_hyhl_is_land == FALSE)
    {
        return 0;
    }

#ifdef ZDT_GPS_SUPPORT
    GPS_API_GetLastPos(&last_gps);
    if((loc_ok&0x01) == 0)
    {
        last_gps.is_valid = 0;
    }
#endif
    SCI_MEMCPY(send_buf,g_zdt_phone_imei,ZDT_IMEI_LEN);
    send_len = ZDT_IMEI_LEN;
    send_len += HYHL_Func_GetTime(send_buf+send_len);
    send_buf[send_len++] = type>>8;
    send_buf[send_len++] = type&0x00FF; //01: SOS     02: lose
    send_len += HYHL_Func_GetGPSSateNum(send_buf+send_len,last_gps.sate_num);
    send_len += HYHL_Func_GetGPSLat(send_buf+send_len,last_gps.Latitude);
    send_len += HYHL_Func_GetGPSLong(send_buf+send_len,last_gps.Longitude);
    send_len += HYHL_Func_GetGPSSpeed(send_buf+send_len,last_gps.speed);
    send_len += HYHL_Func_GetGPSDir(send_buf+send_len,last_gps.is_valid,last_gps.Lat_Dir,last_gps.Long_Dir,last_gps.cog);
    send_len += HYHL_Func_GetPhoneInfo(send_buf+send_len);
    send_len += HYHL_Func_GetBatLevel(send_buf+send_len);
    send_len += HYHL_Func_GetRssiLevel(send_buf+send_len);
    send_len += HYHL_Func_GetWIFIInfo(send_buf+send_len,loc_ok);
    send_len += HYHL_Func_GetSSInfo(send_buf+send_len);
    send_len += HYHL_Func_GetJPStatus(send_buf+send_len);
    send_len += HYHL_Func_GetJPNum(send_buf+send_len);
    send_hande = HYHL_Net_TCPSend(pMe->m_hyhl_tcp_land_interface,0xA3,send_buf,send_len,100,10,30000,HYHL_Net_A3_SendCB);
    ZDT_LOG("HYHL_Net_A3_Send Handle=0x%x,type=0x%x",send_hande,type);
    return send_hande;
}

static void HYHL_Heart_TimerHandle(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_HYHL_HEART,NULL,0);
    return;
}

static void HYHL_Heart_TimeStart(HYHL_APP_T *pMe,uint16 time_s)
{
    if(0 != pMe->m_hyhl_heart_timer_id)
    {
        MMK_StopTimer(pMe->m_hyhl_heart_timer_id);
        pMe->m_hyhl_heart_timer_id = 0;
    }
    
    pMe->m_hyhl_heart_timer_id = MMK_CreateTimerCallback(time_s*900, 
                                                                        HYHL_Heart_TimerHandle, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    return;
}

static void HYHL_Heart_TimeStop(HYHL_APP_T *pMe)
{
    if(0 != pMe->m_hyhl_heart_timer_id)
    {
        MMK_StopTimer(pMe->m_hyhl_heart_timer_id);
        pMe->m_hyhl_heart_timer_id = 0;
    }
}

static int HYHL_Net_A4_SendCB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
/*
心跳包间隔设置 2
Wifi 扫描间隔 1
*/
    HYHL_APP_T *pMe = &g_hyhl_app;
    uint8 * pContent;
    int ContentLen = HYHL_Net_GetContentLen(pRcv,Rcv_len);
    int protocol_id = HYHL_Net_GetProtocol(pRcv,Rcv_len);
    if(protocol_id == 0xA4 && ContentLen >= 2)
    {
        uint16 heart_time = 0;
        uint8 wifi_time = 0;
        pContent = pRcv+5;
        heart_time = pContent[0] << 8;
        heart_time += pContent[1];
        if(ContentLen > 2)
        {
            wifi_time = pContent[2];
        }
        HYHL_Heart_TimeStart(pMe,heart_time);
                
        ZDT_LOG("HYHL_Net_A4_SendCB  heart_time=%d,wifi_time=%d",heart_time,wifi_time);
        return ZDT_TCP_RET_OK;
    }
    ZDT_LOG("HYHL_Net_A4_SendCB  ERR");
    return ZDT_TCP_RET_ERR;
}

uint32 HYHL_Net_A4_Send(HYHL_APP_T* pMe)
{
/* 心跳包
    终端 ID 15
    日期时间 6
    状态信息包 终端信息内容 1
    电压等级 1
    信号强度等级 1
    电量 1
    计步器 步数 3
    WIFI 信息 WIFI 数量 1
                    AP 地址 6
                    信号 1
    LBS 信息 LBS 数量 1
    LBS 信号强度 1
    PN(MCC) 2
    SID(MNC) 2
    NID(LAC) 2
    BID(CELLID) 3
    纬度 4
    经度 4
*/

    uint8 send_buf[200] = {0};
    uint16 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_hyhl_is_land == FALSE)
    {
        return 0;
    }
    SCI_MEMCPY(send_buf,g_zdt_phone_imei,ZDT_IMEI_LEN);
    send_len = ZDT_IMEI_LEN;
    send_len += HYHL_Func_GetTime(send_buf+send_len);
    send_len += HYHL_Func_GetPhoneInfo(send_buf+send_len);
    send_len += HYHL_Func_GetBatLevel(send_buf+send_len);
    send_len += HYHL_Func_GetRssiLevel(send_buf+send_len);
    send_len += HYHL_Func_GetBatPercent(send_buf+send_len);
    send_len += HYHL_Func_GetJPNum(send_buf+send_len);
    send_len += HYHL_Func_GetWIFIInfo(send_buf+send_len,0);
    send_len += HYHL_Func_GetSSInfo(send_buf+send_len);
    send_len += HYHL_Func_GetJPStatus(send_buf+send_len);

    send_hande = HYHL_Net_TCPSend(pMe->m_hyhl_tcp_land_interface,0xA4,send_buf,send_len,100,1,5000,HYHL_Net_A4_SendCB);
    
    ZDT_LOG("HYHL_Net_A4_Send Handle=0x%x",send_hande);
    return send_hande;
}

uint32 HYHL_Net_A6_Send(HYHL_APP_T *pMe,uint8 * number)
{
/* 平台校验辅助包
终端 ID 15
手机号码 11
*/
    uint8 send_buf[100] = {0};
    uint32 send_hande = 0;
    uint16 send_len = 0;
    if(number == NULL)
    {
        return 0;
    }
    if(pMe->m_hyhl_is_land == FALSE)
    {
        return 0;
    }

    SCI_MEMCPY(send_buf,g_zdt_phone_imei,ZDT_IMEI_LEN);
    send_len = ZDT_IMEI_LEN;
    SCI_MEMCPY(send_buf+send_len,number,11);
    send_len += 11;
    send_hande = HYHL_Net_TCPSend(pMe->m_hyhl_tcp_land_interface,0xA6,send_buf,send_len,0,0,0,NULL);
    ZDT_LOG("HYHL_Net_A6_Send Handle=0x%x",send_hande);
    return send_hande;
}

static int HYHL_Net_A8_SendCB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    /*
    信息包 定位地址 N
    */
    ZDT_TCP_INTERFACE_T *pTcp = (ZDT_TCP_INTERFACE_T *)pUser;
    HYHL_APP_T *pMe = &g_hyhl_app;
    uint8 * pContent;
    int ContentLen = HYHL_Net_GetContentLen(pRcv,Rcv_len);
    int protocol_id = HYHL_Net_GetProtocol(pRcv,Rcv_len);
    ZDT_TCP_LINK_DATA_T * GetData = pTcp->m_tcp_cur_data;

    if(protocol_id == 0xA8 && ContentLen >= 0)
    {
        char cur_num[12] = {0};
        pContent = pRcv+5;
        #ifdef HYHL_DB_NEED_SOS
        if(GetData != NULL && GetData->len > 31 && GetData->str != NULL)
        {
            SCI_MEMCPY(cur_num,&GetData->str[20],11);
        }
        if(strlen(cur_num) > 0)
        {
            //ZDT_API_SMS_SendUtf8(pMe->m_zdtsms_client_id,cur_num,pContent,ContentLen);
        }
        #endif
        ZDT_LOG("HYHL_Net_A8_SendCB Success cur_num=%s,ContentLen=%d",cur_num,ContentLen);
        return ZDT_TCP_RET_OK;
    }
    ZDT_LOG("HYHL_Net_A8_SendCB  ERR");
    return ZDT_TCP_RET_ERR;
}

uint32 HYHL_Net_A8_Send(HYHL_APP_T* pMe,uint8 * number,uint8 loc_ok)
{
/* 定位包
    终端 ID 15
    手机号码 11
    日期时间 6
    Gps 信息包 gps 信息卫星数 1
    纬度 4
    经度 4
    速度 1
    航向、状态 2
    WIFI 信息 WIFI 数量 1
    AP 地址 6
    信号 1
    LBS 信息包 LBS 数量 1
    LBS 信号强度 1
    PN(MCC) 2
    SID(MNC) 2
    NID(LAC) 2
    BID(CELLID) 3
    纬度 4
    经度 4
*/    

    uint8 send_buf[200] = {0};
    uint16 send_len = 0;
    uint32 send_hande = 0;
    uint16 num_len = 0;
    GPS_DATA_T last_gps = {0};
    if(pMe->m_hyhl_is_land == FALSE)
    {
        return 0;
    }

#ifdef ZDT_GPS_SUPPORT
    GPS_API_GetLastPos(&last_gps);
    if((loc_ok&0x01) == 0)
    {
        last_gps.is_valid = 0;
    }
#endif
    SCI_MEMCPY(send_buf,g_zdt_phone_imei,ZDT_IMEI_LEN);
    send_len = ZDT_IMEI_LEN;
    if(number != NULL)
    {
        num_len = strlen((char *)number);
        if(num_len < 11)
        {
            SCI_MEMCPY(send_buf+send_len,number,num_len);
            send_len += num_len;
            SCI_MEMCPY(send_buf+send_len,"000000000000",11-num_len);
        }
        else
        {
            SCI_MEMCPY(send_buf+send_len,number,11);
            send_len += 11;
        }
    }
    else
    {
        SCI_MEMCPY(send_buf+send_len,g_zdt_sim_imsi,11);
        send_len += 11;
    }
    send_len += HYHL_Func_GetTime(send_buf+send_len);
    send_len += HYHL_Func_GetGPSSateNum(send_buf+send_len,last_gps.sate_num);
    send_len += HYHL_Func_GetGPSLat(send_buf+send_len,last_gps.Latitude);
    send_len += HYHL_Func_GetGPSLong(send_buf+send_len,last_gps.Longitude);
    send_len += HYHL_Func_GetGPSSpeed(send_buf+send_len,last_gps.speed);
    send_len += HYHL_Func_GetGPSDir(send_buf+send_len,last_gps.is_valid,last_gps.Lat_Dir,last_gps.Long_Dir,last_gps.cog);
    send_len += HYHL_Func_GetWIFIInfo(send_buf+send_len,loc_ok);
    send_len += HYHL_Func_GetSSInfo(send_buf+send_len);
    send_len += HYHL_Func_GetJPStatus(send_buf+send_len);
    send_len += HYHL_Func_GetJPNum(send_buf+send_len);
    send_hande = HYHL_Net_TCPSend(pMe->m_hyhl_tcp_land_interface,0xA8,send_buf,send_len,0,1,2000,HYHL_Net_A8_SendCB);
    ZDT_LOG("HYHL_Net_A8_Send Handle=0x%x,loc_ok=%d",send_hande,loc_ok);
    return send_hande;
}
uint32 HYHL_Net_A9_Respond(HYHL_APP_T *pMe,uint8 * ide,uint8 loc_ok)
{
/* 
    终端 ID 15
    服务器标识位 4
    日期时间 6
    Gps 信息包 gps 信息卫星数 1
    纬度 4
    经度 4
    速度 1
    航向、状态 2
    WIFI 信息 WIFI 数量 1
    AP 地址 6
    信号 1
    Lbs 信息包 LBS 数量 1
    LBS 信号强度 1
    PN(MCC) 2
    SID(MNC) 2
    NID(LAC) 2
    BID(CELLID) 3
    纬度 4
    经度 4
*/    
    
    uint8 send_buf[200] = {0};
    uint16 send_len = 0;
    uint32 send_hande = 0;
    GPS_DATA_T last_gps = {0};
#ifdef ZDT_GPS_SUPPORT
    GPS_API_GetLastPos(&last_gps);
    if((loc_ok&0x01) == 0)
    {
        last_gps.is_valid = 0;
    }
#endif
    SCI_MEMCPY(send_buf,g_zdt_phone_imei,ZDT_IMEI_LEN);
    send_len = ZDT_IMEI_LEN;
    SCI_MEMCPY(send_buf+send_len,ide,4);
    send_len += 4;
    send_len += HYHL_Func_GetTime(send_buf+send_len);
    send_len += HYHL_Func_GetGPSSateNum(send_buf+send_len,last_gps.sate_num);
    send_len += HYHL_Func_GetGPSLat(send_buf+send_len,last_gps.Latitude);
    send_len += HYHL_Func_GetGPSLong(send_buf+send_len,last_gps.Longitude);
    send_len += HYHL_Func_GetGPSSpeed(send_buf+send_len,last_gps.speed);
    send_len += HYHL_Func_GetGPSDir(send_buf+send_len,last_gps.is_valid,last_gps.Lat_Dir,last_gps.Long_Dir,last_gps.cog);
    send_len += HYHL_Func_GetWIFIInfo(send_buf+send_len,loc_ok);
    send_len += HYHL_Func_GetSSInfo(send_buf+send_len);
    send_len += HYHL_Func_GetJPStatus(send_buf+send_len);
    send_len += HYHL_Func_GetJPNum(send_buf+send_len);
    send_hande = HYHL_Net_TCPSend(pMe->m_hyhl_tcp_land_interface,0xA9,send_buf,send_len,150,0,0,NULL);
    ZDT_LOG("HYHL_Net_A9_Respond Handle=0x%x,loc_ok=%d",send_hande,loc_ok);
    return send_hande;
}

int HYHL_Net_A9_Receive(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
/*
    信息包 指令长度 1
    服务器标志位 4
    指令内容 N
*/
    if(ContentLen > 5)
    {
        uint8 * cmd_buf = pContent+ 5;
        uint8 cmd_len = pContent[0];
        ZDT_LOG("HYHL_Net_A9_Receive cmd_len = %d,%c%c",cmd_len,cmd_buf[0],cmd_buf[1]);
        if(cmd_len == 6)
        {
            if((cmd_buf[0] == 'D' || cmd_buf[0] == 'd')
                && (cmd_buf[1] == 'W' || cmd_buf[1] == 'w'))
            {
#ifdef ZDT_GPS_SUPPORT
                //HYHL_GPS_Start(pMe,HYHL_GPS_TYPE_NET,pContent+1,4);
                ZDT_GPS_PowerOn();
                HYHL_Net_A9_Respond(pMe,pContent+1,0);
#else
                HYHL_Net_A9_Respond(pMe,pContent+1,0);
#endif
            }
        }
    }
    return 0;
}

uint32 HYHL_Net_AC_Respond(HYHL_APP_T *pMe,uint8 *mark,char * rsp_cont,uint16 cont_len)
{
/* 
终端 ID 15
指令长度 1
服务器标识 4
指令结果 N
*/    
    uint8 send_buf[200] = {0};
    uint16 send_len = 0;
    uint32 send_hande = 0;
    SCI_MEMCPY(send_buf,g_zdt_phone_imei,ZDT_IMEI_LEN);
    send_len = ZDT_IMEI_LEN;
    send_buf[send_len++] = cont_len+4;
    SCI_MEMCPY(send_buf+send_len,mark,4);
    send_len += 4;
    SCI_MEMCPY(send_buf+send_len,rsp_cont,cont_len);
    send_len += cont_len;
    send_hande = HYHL_Net_TCPSend(pMe->m_hyhl_tcp_land_interface,0xAC,send_buf,send_len,150,0,0,NULL);
    ZDT_LOG("HYHL_Net_AC_Respond Handle=0x%x",send_hande);
    return send_hande;
}

#ifdef HYHL_DB_NEED_TIMESET

/* 省电模式设置
指令示例:
KQSD|1,21301030}  //1,HMHM
//55550016AC10001EC84F4B5153447C012C0B140C1E7DC84F73A10D0A
*/
int HYHL_Net_AC_KQSD_Handle(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
    int ret = 0;
    uint16 i = 0;
    uint8 * str = NULL;
    uint16 len = 0;
    uint32 timer_start = 0;
    uint32 timer_end = 0;
    uint8 timer_set = 0;

    if(ContentLen < 17)
    {
        HYHL_Net_AC_Respond(pMe,pContent+1,"KQSD=Fail!",10);
        return -1;
    }
    
    str = pContent+10;
    len = ContentLen-11;
    
    ZDT_LOG("HYHL_Net_AC_KQSD Rcv len=%d,on=%d,time=%d:%d--%d:%d",len,str[0],str[2],str[3],str[4],str[5]);
    timer_set = str[0];
    
    timer_start = (str[2] * 3600) + (str[3] * 60);
    timer_end = (str[4] * 3600) + (str[5] * 60);
    if(timer_set)
    {
        HYHL_DB_TIMESET_ListModify(timer_start,timer_end);
    }
    else
    {
        HYHL_DB_TIMESET_ListDel();
    }
    
    HYHL_Net_AC_Respond(pMe,pContent+1,"KQSD=Success!",13);
    return 0;
}
#endif

#ifdef HYHL_DB_NEED_SOS
/*
FN&&familynumber1&&13790774051&&zhangsan&&13790774051&&lisi&&1379077
4051&&wangwu&&13790774051# 添加四个亲情号码
*/
int HYHL_Net_AC_FN_Handle(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
    int ret = 0;
    uint16 i = 0;
    uint8 * str = NULL;
    uint16 len = 0;
    uint8 sos_name[HYHL_NET_SOS_MAX_NAME_SIZE+1] = {0};
    uint8 sos_num[HYHL_NET_SOS_MAX_NUM_SIZE+1] = {0};
    
    if(ContentLen < 11)
    {
        HYHL_Net_AC_Respond(pMe,pContent+1,"FN=Fail!",8);
        return -1;
    }
    
    str = pContent+9;
    len = ContentLen-10;

    ZDT_LOG("HYHL_Net_AC_FN_Rcv len = %d",len);
    HYHL_DB_SOS_ListDelAllBuf();
    for(i = 0; i < HYHL_DB_SOS_MAX_SUM; i++)
    {
        SCI_MEMSET(sos_name,0,HYHL_NET_SOS_MAX_NAME_SIZE+1);
        SCI_MEMSET(sos_num,0,HYHL_NET_SOS_MAX_NUM_SIZE+1);
        ret = HYHL_Func_SOSGetNextPara(&str, &len,sos_name,HYHL_NET_SOS_MAX_NAME_SIZE);
        if(ret < 0)
        {
            break;
        }
        ret = HYHL_Func_SOSGetNextPara(&str, &len,sos_num,HYHL_NET_SOS_MAX_NUM_SIZE);
        if(ret > 0)
        {
            ZDT_LOG("HYHL_Net_AC_FN_Rcv[%d] sos_num = %s",i,sos_num);
            HYHL_DB_SOS_ListModifyBuf(i,(char *)sos_num,strlen((char *)sos_num),(char *)sos_name,strlen((char *)sos_name));
        }
    }
    HYHL_DB_SOS_ListSaveBuf();
    ZDT_LOG("HYHL_Net_SOS Set End");
    HYHL_Net_AC_Respond(pMe,pContent+1,"FN=Success!",11);
    return 0;
}
#endif

#ifdef HYHL_DB_NEED_WHITE
/*
WN&&familynumber1&&13790774051&&zhangsan&&13790774051&&lisi&&1379077
4051&&wangwu&&13790774051# 添加50个白名单号码
*/
int HYHL_Net_AC_WN_Handle(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
    int ret = 0;
    uint16 i = 0;
    uint8 * str = NULL;
    uint16 len = 0;
    uint8 white_name[HYHL_NET_WHITE_MAX_NAME_SIZE+1] = {0};
    uint8 white_num[HYHL_NET_WHITE_MAX_NUM_SIZE+1] = {0};
    
    if(ContentLen < 11)
    {
        HYHL_Net_AC_Respond(pMe,pContent+1,"WN=Fail!",8);
        return -1;
    }
    
    str = pContent+9;
    len = ContentLen-10;

    ZDT_LOG("HYHL_Net_AC_WN_Rcv len = %d",len);
    HYHL_DB_WHITE_ListDelAllBuf();
    for(i = 0; i < HYHL_DB_WHITE_MAX_SUM; i++)
    {
        SCI_MEMSET(white_name,0,HYHL_NET_WHITE_MAX_NAME_SIZE+1);
        SCI_MEMSET(white_num,0,HYHL_NET_WHITE_MAX_NUM_SIZE+1);
        ret = HYHL_Func_SOSGetNextPara(&str, &len,white_name,HYHL_NET_WHITE_MAX_NAME_SIZE);
        if(ret < 0)
        {
            break;
        }
        ret = HYHL_Func_SOSGetNextPara(&str, &len,white_num,HYHL_NET_WHITE_MAX_NUM_SIZE);
        if(ret > 0)
        {
            ZDT_LOG("HYHL_Net_AC_WN_Rcv[%d] sos_num = %s",i,white_num);
            HYHL_DB_WHITE_ListModifyBuf(i,(char *)white_num,strlen((char *)white_num),(char *)white_name,strlen((char *)white_name));
        }
    }
    HYHL_DB_WHITE_ListSaveBuf();
    ZDT_LOG("HYHL_Net_WHITE Set End");
    HYHL_Net_AC_Respond(pMe,pContent+1,"WN=Success!",11);
    return 0;
}
#endif

#ifdef HYHL_DB_NEED_HIDDEN

/*隐身时段
指令示例:
TIME| S1,k|S2,k|S3,k|S4,k|S5,k|S6,k|S7,k|S8,k }
S1: HMHM
K1
第 0 位
1：开
0：关
第 1 位
1：星期一启用
0：星期一不启用
.....
*/

int HYHL_Net_AC_HIDDEN_Handle(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
    int ret = 0;
    uint16 i = 0;
    uint8 * str = NULL;
    uint16 len = 0;
    uint16 con_len = 0;
    uint8 timer_type = 0;
    uint32 timer_start = 0;
    uint32 timer_end = 0;
    uint8 tmp_str[201] = {0};
    
    if(ContentLen < 11)
    {
        HYHL_Net_AC_Respond(pMe,pContent+1,"TIME=Fail!",10);
        return -1;
    }
    
    str = pContent+10;
    len = ContentLen-11;
    
    HYHL_DB_HIDDEN_ListDelAll();
    for(i = 0; i < HYHL_DB_HIDDEN_MAX_SUM; i++)
    {
        SCI_MEMSET(tmp_str,0,201);
        ret = HYHL_Func_HiddenGetNextPara(&str, &len,tmp_str,200);
        if(ret < 0)
        {
            break;
        }
        
        if(ret > 5)
        {
            timer_start = (tmp_str[0] * 3600) + (tmp_str[1] * 60);
            timer_end = (tmp_str[2] * 3600) + (tmp_str[3] * 60);
            timer_type = tmp_str[5];
            ZDT_LOG("HYHL_Net_AC_HIDDEN Rcv idx=%d,timer=%ld-%ld,type=0x%x",i,timer_start,timer_end,timer_type);
            HYHL_DB_HIDDEN_ListModify(i,timer_type,timer_start,timer_end);
        }
    }
    HYHL_DB_HIDDEN_ListSave();
        
    ZDT_LOG("HYHL_Net_AC_HIDDEN Set End size=%d",sizeof(HYHL_DB_HIDDEN_Type));
    HYHL_Net_AC_Respond(pMe,pContent+1,"TIME=Success!",13);
    return 0;
}
#endif
/* 发送文本信息
 FSWB|文本内容
指令示例:
FSWB|大家好,我是小小亮}
*/
int HYHL_Net_AC_FSWB_Handle(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
    uint8 * str = NULL;
    uint16 len = 0;
    
    if(ContentLen < 11)
    {
        HYHL_Net_AC_Respond(pMe,pContent+1,"FSWB=Fail!",10);
        return -1;
    }
    
    str = pContent+10;
    len = ContentLen-11;

    ZDT_LOG("HYHL_Net_AC_FSWB Rcv len = %d,time=%d:%d--%d:%d,%d",len);

    if(str != NULL && len  != 0)
    {
        //Zdt_AlerCore(pMe,str,len);
        HYHL_Net_AC_Respond(pMe,pContent+1,"FSWB=Success!",13);
    }
    else
    {
        HYHL_Net_AC_Respond(pMe,pContent+1,"FSWB=Fail!",10);
    }
    
    return 0;
}

/* 关机
 GJ
*/
void HYHL_Net_AC_GJ_TimeOut(void)
{
    //keypad_simulate_poweroff_key();
    return;
}

int HYHL_Net_AC_GJ_Handle(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
    if(g_is_hyhl_power_off_ing == TRUE)
    {
        return 0;
    }
    HYHL_Net_AC_Respond(pMe,pContent+1,"GJ=Success!",11);
    g_is_hyhl_power_off_ing = TRUE;
    //ISHELL_SetTimer(pMe->a.m_pIShell, 3000, (PFNNOTIFY)HYHL_Net_AC_GJ_TimeOut, NULL);
    return 0;
}


int HYHL_Net_AC_CZSB_Handle(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
    g_is_hyhl_czsb_is_playing = TRUE;
    //RingSetting_PlayRingStart(RING_MISSEDCALL, NULL);
    HYHL_Net_AC_Respond(pMe,pContent+1,"CZSB=Success!",13);
    return 0;
}


int HYHL_Net_AC_KQGSENSOR_Handle(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
    #ifdef ZDT_GSENSOR_SUPPORT
        //gsensor_Start(WORK_MODE_JP);
    #endif
    HYHL_Net_AC_Respond(pMe,pContent+1,"KQGSENSOR=Success!",18);
    return 0;
}

int HYHL_Net_AC_GBGSENSOR_Handle(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
    #ifdef ZDT_GSENSOR_SUPPORT
        //gsensor_Stop(WORK_MODE_JP);
    #endif
    HYHL_Net_AC_Respond(pMe,pContent+1,"GBGSENSOR=Success!",18);
    return 0;
}

int HYHL_Net_AC_HFCC_Handle(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
    HYHL_Net_AC_Respond(pMe,pContent+1,"CZSB=Success!",13);	
    HYHL_DB_WHITE_ListDelAll();
    HYHL_DB_SOS_ListDelAll();    	
    return 0;
}

int HYHL_Net_AC_Receive(HYHL_APP_T *pMe,uint8 * pContent,uint16 ContentLen)
{
/*
    指令长度 1
    服务器标识 4
    指令内容 N
*/
    if(ContentLen > 5)
    {
        uint8 * cmd_buf = pContent+ 5;
        int16 cmd_len = ContentLen-5;
        ZDT_LOG("HYHL_Net_AC_Receive cmd_len = %d",cmd_len);
        if(cmd_len > 4 &&  strncmp( (char *)cmd_buf, "FN&&", 4 ) == 0)
        {
            //亲情号码设置
            #ifdef HYHL_DB_NEED_SOS
            HYHL_Net_AC_FN_Handle(pMe,pContent,ContentLen);
            #endif
        }
        else if(cmd_len > 4 &&  strncmp( (char *)cmd_buf, "WN&&", 4 ) == 0)
        {
            //白名单设置
            #ifdef HYHL_DB_NEED_WHITE
            HYHL_Net_AC_WN_Handle(pMe,pContent,ContentLen);
            #endif
        }
       else if(cmd_len > 5 &&  strncmp( (char *)cmd_buf, "KQSD|", 5 ) == 0)
        {
            //省电模式
            #ifdef HYHL_DB_NEED_TIMESET
                HYHL_Net_AC_KQSD_Handle(pMe,pContent,ContentLen);
            #endif
        }
        else if(cmd_len > 5 &&  strncmp( (char *)cmd_buf, "FSWB|", 5 ) == 0)
        {
            //发送文本信息
            HYHL_Net_AC_FSWB_Handle(pMe,pContent,ContentLen);
        }
        else if(cmd_len > 5 &&  strncmp( (char *)cmd_buf, "TIME|", 5) == 0)
        {
            //隐身时段
            #ifdef HYHL_DB_NEED_HIDDEN
            HYHL_Net_AC_HIDDEN_Handle(pMe,pContent,ContentLen);
            #endif
        }
        else if(cmd_len > 1 &&  strncmp( (char *)cmd_buf, "GJ", 2 ) == 0)
        {
            //关机
            HYHL_Net_AC_GJ_Handle(pMe,pContent,ContentLen);
        }
        else if(cmd_len > 3 &&  strncmp( (char *)cmd_buf, "CZSB", 4 ) == 0)
        {
            //查找手表
            HYHL_Net_AC_CZSB_Handle(pMe,pContent,ContentLen);
        }
        else if(cmd_len > 8 &&  strncmp( (char *)cmd_buf, "KQGSENSOR", 9 ) == 0)
        {
            //开启GSensor
            HYHL_Net_AC_KQGSENSOR_Handle(pMe,pContent,ContentLen);
        }
        else if(cmd_len > 8 &&  strncmp( (char *)cmd_buf, "GBGSENSOR", 9 ) == 0)
        {
            //关闭GSensor
            HYHL_Net_AC_GBGSENSOR_Handle(pMe,pContent,ContentLen);
        }
        else if(cmd_len > 3 &&  strncmp( (char *)cmd_buf, "HFCC", 4 ) == 0)
        {
            //恢复出厂设置
            HYHL_Net_AC_HFCC_Handle(pMe,pContent,ContentLen);
        }	
    }
    return 0;
}

static int HYHL_Net_ReceiveHandle(HYHL_APP_T *pMe,uint8 protocol_id,uint8 * pData,uint16 DataLen,uint16 Msg_len)
{
    int ret = 0;
    switch(protocol_id)
    {   
        case 0xA9:
                ret = HYHL_Net_A9_Receive(pMe,pData+5,Msg_len-5);
            break;
        case 0xAC:
                ret = HYHL_Net_AC_Receive(pMe,pData+5,Msg_len-5);
            break;
            
         default:
                break;
    }
    return ret;
}

static int HYHL_Net_Receive(HYHL_APP_T *pMe,uint8 * pData,uint16 Data_len)
{
    uint8 protocol_id;
    uint16 MsgLen = 0;
    uint16 crc = 0;
    uint8 crc_high = 0;
    uint8 crc_low = 0;
    uint16 len = 0;
    
    if(pData == NULL || Data_len < 11)
    {
        ZDT_LOG("HYHL_Net_Receive Err NULL");
        return 0;
    }
    
    if(pData[0] != HYHL_NET_MSG_START_1 || pData[1] != HYHL_NET_MSG_START_2)
    {
        ZDT_LOG("HYHL_Net_Receive Err Start");
        return 0;
    }

    MsgLen = pData[2];
    MsgLen = MsgLen<<8;
    MsgLen += pData[3];

    //A2 AGPS消息是个例外
    if(pData[4] == 0xA2)
    {
        MsgLen +=2;
    }
    
    if(MsgLen < 5 || (MsgLen+6) > Data_len)
    {
        ZDT_LOG("HYHL_Net_Receive Err Len %d>%d",(MsgLen+6),Data_len);
        return 0;
    }
    
    protocol_id = pData[4];
    
    crc = HYHL_Func_GetCrc16(&pData[4],MsgLen-2);
    crc_high= crc>>8;
    crc_low = crc&0x00FF;
    
    if(crc_high != pData[MsgLen+2] || crc_low != pData[MsgLen+3])
    {
        ZDT_LOG("HYHL_Net_Receive Err CRC 0x%02x,0x%02x -- 0x%02x,0x%02x",crc_high,crc_low,pData[MsgLen+2] ,pData[MsgLen+3]);
    }
    
    if(HYHL_NET_MSG_END_1 != pData[MsgLen+4] || HYHL_NET_MSG_END_2 != pData[MsgLen+5])
    {
        ZDT_LOG("HYHL_Net_Receive Err END 0x%02x,0x%02x",pData[MsgLen+4] ,pData[MsgLen+5]);
    }
    
    return HYHL_Net_ReceiveHandle(pMe,protocol_id,pData,Data_len,MsgLen);
}

static int HYHL_Net_HandleSendSuccess(HYHL_APP_T *pMe,uint8 * pData,uint32 Data_len)
{
    int ret = 0;
    int protocol_id = 0;
    protocol_id = HYHL_Net_GetProtocol(pData,Data_len);
    switch(protocol_id)
    {
        case 0xA0:
                pMe->m_hyhl_rsp_err_num = 0;
                if(pMe->m_hyhl_is_land == FALSE)
                {
                    pMe->m_hyhl_is_land = TRUE;
                }
                
                HYHL_Net_A4_Send(pMe);
            break;
                        
     default:
            break;
    }
    return ret;
}

static int HYHL_Net_HandleSendFail(HYHL_APP_T *pMe,uint8 * pData,uint32 Data_len,uint16 err_id)
{
    int ret = 0;
    int protocol_id = 0;
    
    protocol_id = HYHL_Net_GetProtocol(pData,Data_len);
    switch(protocol_id)
    {
        case 0xA0:
            if(err_id != TCP_ERR_CLEAR)
            {
                HYHL_Net_Reset(pMe);
            }
            break;
         case 0xA4: //心跳包发送失败重新注册
            if(err_id != TCP_ERR_CLEAR)
            {
                if(pMe->m_hyhl_is_land)
                {
                    ZDT_LOG("HYHL_Net ReLand Heart--------------");
                    pMe->m_hyhl_is_land = FALSE;
                    HYHL_Net_A0_Send(pMe,1,10000);
                }
            }
            break;
            
        default:
            break;
    }
    
    if(TCP_ERR_NET != err_id && TCP_ERR_CLEAR != err_id && TCP_ERR_READ != err_id && TCP_ERR_INCALL != err_id)
    {
        pMe->m_hyhl_rsp_err_num++;
        if(pMe->m_hyhl_rsp_err_num >= 5)
        {
            ZDT_LOG("HYHL_Net_HandleSendFail RESET");
            if(protocol_id != 0xA0)
            {
                HYHL_Net_Reset(pMe);
            }
            pMe->m_hyhl_rsp_err_num = 0;
        }
    }
    return ret;
}

static void HYHL_Land_TimerHandle(
                                uint8 timer_id,
                                uint32 param
                                )
{
    HYHL_APP_T *pMe = (HYHL_APP_T *)param;
    HYHL_Net_Land(pMe->m_hyhl_reg_ip,pMe->m_hyhl_reg_port);
    return;
}

static void HYHL_Land_TimeStart(HYHL_APP_T *pMe)
{
    if(0 != pMe->m_hyhl_land_timer_id)
    {
        MMK_StopTimer(pMe->m_hyhl_land_timer_id);
        pMe->m_hyhl_land_timer_id = 0;
    }
    
    pMe->m_hyhl_land_timer_id = MMK_CreateTimerCallback(3000, 
                                                                        HYHL_Land_TimerHandle, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    return;
}

static void HYHL_Land_TimeStop(HYHL_APP_T *pMe)
{
    if(0 != pMe->m_hyhl_land_timer_id)
    {
        MMK_StopTimer(pMe->m_hyhl_land_timer_id);
        pMe->m_hyhl_land_timer_id = 0;
    }
}

static int HYHL_Net_Land_CallBack(void * pUser,TCP_CB_TYPE_E cb_type,uint16 wParam, uint32 dwParam)
{
    int res = 0;
    ZDT_TCP_INTERFACE_T *pTcp = (ZDT_TCP_INTERFACE_T *)pUser;
    HYHL_APP_T *pMe = &g_hyhl_app;
    ZDT_LOG("HYHL_Net_Land_CallBack: cb_type=%d,Result=%d,Data=0x%x",cb_type,wParam,dwParam);
    switch(cb_type)
    {
        case TCP_CB_DISCONNECTED:
                ZDT_LOG("HYHL_Net TCP_CB_DISCONNECTED m_hyhl_is_land=%d",pMe->m_hyhl_is_land);
                if(pMe->m_hyhl_is_land)
                {
                    ZDT_LOG("HYHL_Net ReLand NET_DISCONNECTED--------------");
                    pMe->m_hyhl_is_land = FALSE;
                    HYHL_Net_A0_Send(pMe,1,10000);
                }
            break;
            
        case TCP_CB_REG:
            {
                uint16 ret = wParam;
                uint32 err_id =  dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //注册成功
                    ZDT_LOG("HYHL_Net_Land TCPReg OK --------------");
                    pMe->m_hyhl_rsp_err_num = 0;
                    pMe->m_hyhl_is_land = FALSE;
                    HYHL_Land_TimeStop(pMe);
                    HYHL_Net_A0_Send(pMe,10,10000);
                }
                else
                {
                    //注册失败
                    ZDT_LOG("HYHL_Net_Land TCPReg ERR --------------");
                    pMe->m_hyhl_is_land = FALSE;
                    if(err_id == TCP_ERR_REGEXIST)
                    {
                        HYHL_Land_TimeStart(pMe);
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
                    ZDT_LOG("HYHL_Net_Land TCPUNReg OK --------------");
                }
                else
                {
                    //注销失败
                    ZDT_LOG("HYHL_Net_Land TCPUNReg ERR --------------");
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
                    ZDT_LOG("HYHL_Net_TCPSend Result Success Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        HYHL_Net_HandleSendSuccess(pMe,SendData->str,SendData->len);                        
                    }
                }
                else
                {
                    //发送失败
                    ZDT_LOG("HYHL_Net_TCPSend Result FAIL Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        HYHL_Net_HandleSendFail(pMe,SendData->str,SendData->len,ret);
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
                        res = HYHL_Net_Receive(pMe,RcvData->pRcv,RcvData->len);
                    }
                    ZDT_LOG("HYHL_Net_Receive Over");
                }
                else
                {
                    //接收失败
                    ZDT_LOG("HYHL_Net_Receive FAIL");
                }
            }
            break;
                        
        default:
            break;
    }
    return res;
}

static int HYHL_Net_Land(uint32 ip_addr,uint16 port)
{
    char * ipstr = PNULL;
    ZDT_API_TCP_UnReg(g_hyhl_app.m_hyhl_tcp_reg_interface);
    ipstr = inet_ntoa(ip_addr);
    if(ipstr == PNULL)
    {
        ZDT_LOG("HYHL_Net_Land Err IP 0x%x:%d",ip_addr,port);
        return 1;
    }
    ZDT_LOG("HYHL_Net_Land TCPReg  ip=0x%x,%s,port=%d",ip_addr,ipstr,port);
    ZDT_API_TCP_UnReg(g_hyhl_app.m_hyhl_tcp_land_interface);
    ZDT_API_TCP_Reg(g_hyhl_app.m_hyhl_tcp_land_interface,ipstr,port,HYHL_Net_Land_CallBack);
    return 0;
}

static void HYHL_1A_TimerHandle(
                                uint8 timer_id,
                                uint32 param
                                )
{
    HYHL_APP_T *pMe = (HYHL_APP_T *)param;
    HYHL_Net_1A_Send(pMe,0,0);
    return;
}

static void HYHL_1A_TimeStart(HYHL_APP_T *pMe,uint16 time_s)
{
    if(0 != pMe->m_hyhl_1a_timer_id)
    {
        MMK_StopTimer(pMe->m_hyhl_1a_timer_id);
        pMe->m_hyhl_1a_timer_id = 0;
    }
    
    pMe->m_hyhl_1a_timer_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        HYHL_1A_TimerHandle, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    return;
}

static void HYHL_1A_TimeStop(HYHL_APP_T *pMe)
{
    if(0 != pMe->m_hyhl_1a_timer_id)
    {
        MMK_StopTimer(pMe->m_hyhl_1a_timer_id);
        pMe->m_hyhl_1a_timer_id = 0;
    }
}

static void HYHL_Reg_TimerHandle(
                                uint8 timer_id,
                                uint32 param
                                )
{
    HYHL_Net_Reg();
    return;
}

static void HYHL_Reg_TimeStart(HYHL_APP_T *pMe)
{
    if(0 != pMe->m_hyhl_reg_timer_id)
    {
        MMK_StopTimer(pMe->m_hyhl_reg_timer_id);
        pMe->m_hyhl_reg_timer_id = 0;
    }
    
    pMe->m_hyhl_reg_timer_id = MMK_CreateTimerCallback(3000, 
                                                                        HYHL_Reg_TimerHandle, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    return;
}

static void HYHL_Reg_TimeStop(HYHL_APP_T *pMe)
{
    if(0 != pMe->m_hyhl_reg_timer_id)
    {
        MMK_StopTimer(pMe->m_hyhl_reg_timer_id);
        pMe->m_hyhl_reg_timer_id = 0;
    }
}

static int HYHL_Net_Reg_CallBack(void * pUser,TCP_CB_TYPE_E cb_type,uint16 wParam, uint32 dwParam)
{
    ZDT_TCP_INTERFACE_T *pTcp = (ZDT_TCP_INTERFACE_T *)pUser;
    HYHL_APP_T *pMe = &g_hyhl_app;
    ZDT_LOG("HYHL_Net_Reg_CallBack: cb_type=%d,Result=%d,Data=0x%x",cb_type,wParam,dwParam);

    switch(cb_type)
    {
        case TCP_CB_REG:
            {
                uint16 ret = wParam;
                uint32 err_id =  dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //注册成功
                    ZDT_LOG("HYHL_Net_Reg TCPReg OK --------------");
                    pMe->m_hyhl_is_reg = FALSE;
                    pMe->m_hyhl_is_land = FALSE;
                    HYHL_Reg_TimeStop(pMe);
                    HYHL_1A_TimeStop(pMe);
                    if(pMe->m_hyhl_is_first_reg == FALSE)
                    {
                        pMe->m_hyhl_allow_rereg = TRUE;
                        HYHL_Net_1A_Send(pMe,10,5000);
                        pMe->m_hyhl_is_first_reg = TRUE;
                    }
                    else
                    {
                        HYHL_Net_1A_Send(pMe,0,0);
                    }
                }
                else
                {
                    //注册失败
                    ZDT_LOG("HYHL_Net_Reg TCPReg ERR --------------");
                    if(err_id == TCP_ERR_REGEXIST)
                    {
                        HYHL_Reg_TimeStart(pMe);
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
                    ZDT_LOG("HYHL_Net_Reg TCPUNReg OK --------------");
                    HYHL_1A_TimeStop(pMe);
                }
                else
                {
                    //注销失败
                    ZDT_LOG("HYHL_Net_Reg TCPUNReg ERR --------------");
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
                    ZDT_LOG("HYHL_Net_TCPSend Result Success Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        int protocol_id = HYHL_Net_GetProtocol(SendData->str,SendData->len);
                        if(protocol_id == 0x1A)
                        {
                            pMe->m_hyhl_reg_err_num = 3;
                            HYHL_1A_TimeStop(pMe);
                            pMe->m_hyhl_is_land = FALSE;
                            HYHL_Net_Land(pMe->m_hyhl_reg_ip,pMe->m_hyhl_reg_port);
                        }
                    }
                }
                else
                {
                    //发送失败
                    ZDT_LOG("HYHL_Net_TCPSend Result FAIL Handle=0x%x",SendData);
                    if(ret != TCP_ERR_CLEAR)
                    {
                        if(TCP_ERR_IP == ret)
                        {
                            g_is_hyhl_server_fixed = TRUE;
                            pMe->m_hyhl_is_reg = FALSE;
                            pMe->m_hyhl_is_land = FALSE;
                            HYHL_Net_Reg();
                        }
                        else
                        {
                            if(SendData != NULL && SendData->str != NULL)
                            {
                                int protocol_id = HYHL_Net_GetProtocol(SendData->str,SendData->len);
                                if(protocol_id == 0x1A)
                                {
                                    if(ret != TCP_ERR_INCALL)
                                    {
                                        pMe->m_hyhl_reg_err_num ++;
                                        if(pMe->m_hyhl_reg_err_num > 3)
                                        {
                                            if(pMe->m_hyhl_net_is_init)
                                            {
                                                //ZDT_System_Reset(pMe);
                                            }
                                        }
                                    }
                                    if(pMe->m_hyhl_reg_err_num > 200)
                                    {
                                        pMe->m_hyhl_reg_err_num = 0;
                                    }
                                    if(pMe->m_hyhl_reg_err_num < 3)
                                    {
                                        HYHL_1A_TimeStart(pMe,2);
                                    }
                                    else
                                    {
                                        HYHL_1A_TimeStart(pMe,120);
                                    }
                                }
                            }
                        }
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
                    ZDT_LOG("HYHL_Net_Receive Over");
                }
                else
                {
                    //接收失败
                    ZDT_LOG("HYHL_Net_Receive FAIL");
                }
            }
            break;
                        
        default:
            break;
    }
    return 0;
}

int HYHL_Net_Reg(void)
{
    ZDT_LOG("HYHL_Net_Reg TCPReg is_test=%d",g_is_hyhl_test_server);
    if(FALSE == HYHL_Net_IsInit())
    {
        return 1;
    }

    ZDT_API_TCP_UnReg(g_hyhl_app.m_hyhl_tcp_reg_interface);
    ZDT_API_TCP_UnReg(g_hyhl_app.m_hyhl_tcp_land_interface);
    
    if(g_is_hyhl_test_server)
    {
        //测试服务器
        if(g_is_hyhl_server_fixed)
        {
            ZDT_API_TCP_Reg(g_hyhl_app.m_hyhl_tcp_reg_interface,"183.56.154.34",8801,HYHL_Net_Reg_CallBack);
        }
        else
        {
            ZDT_API_TCP_Reg(g_hyhl_app.m_hyhl_tcp_reg_interface,"auth.hoinnet.com",8801,HYHL_Net_Reg_CallBack);
        }
    }
    else
    {
        //商用服务器
        if(g_is_hyhl_server_fixed)
        {
            ZDT_API_TCP_Reg(g_hyhl_app.m_hyhl_tcp_reg_interface,"14.152.50.88",8802,HYHL_Net_Reg_CallBack);
        }
        else
        {
            ZDT_API_TCP_Reg(g_hyhl_app.m_hyhl_tcp_reg_interface,"conn-tcp.hoinnet.com",8802,HYHL_Net_Reg_CallBack);
        }
    }
    return 0;
}

extern void HYHL_Net_CloseAllTimer(HYHL_APP_T *pMe)
{
    HYHL_Reg_TimeStop(pMe);
    HYHL_Land_TimeStop(pMe);
    HYHL_Heart_TimeStop(pMe);
    HYHL_1A_TimeStop(pMe);
    return;
}

extern BOOLEAN HYHL_Net_IsAllow(HYHL_APP_T* pMe)
{
    if(pMe->m_hyhl_net_is_init)
    {
        return TRUE;
    }
    return FALSE;
}

extern BOOLEAN HYHL_Net_IsOpen(HYHL_APP_T* pMe)
{
    return pMe->m_hyhl_net_is_open;
}

extern int HYHL_Net_Open(HYHL_APP_T* pMe)
{
    ZDT_LOG("HYHL_Net_Open");
    pMe->m_hyhl_is_reg = FALSE;
    pMe->m_hyhl_is_land = FALSE;
    HYHL_Net_Reg();
    pMe->m_hyhl_net_is_open = TRUE;
    return 0;
}

extern int HYHL_Net_Close(HYHL_APP_T* pMe)
{
    ZDT_LOG("HYHL_Net_Close");
    pMe->m_hyhl_is_reg = FALSE;
    pMe->m_hyhl_is_land = FALSE;
    if(HYHL_Net_IsInit())
    {
        ZDT_API_TCP_UnReg(g_hyhl_app.m_hyhl_tcp_reg_interface);
        ZDT_API_TCP_UnReg(g_hyhl_app.m_hyhl_tcp_land_interface);
    }
    pMe->m_hyhl_net_is_open = FALSE;
   return 0;
}

extern int HYHL_Net_Init(void)
{
    HYHL_APP_T * pMe = &g_hyhl_app;
    ZDT_LOG("HYHL_Net_Init");
    if(ZdtTCP_NetOpen(&(g_hyhl_app.m_hyhl_tcp_reg_interface),HYHL_CMD_CheckAll,10240))
    {
        ZDT_LOG("HYHL_Net_Init Reg OK ");
    }
    if(ZdtTCP_NetOpen(&(g_hyhl_app.m_hyhl_tcp_land_interface),HYHL_CMD_CheckAll,10240))
    {
        ZDT_LOG("HYHL_Net_Init Land OK ");
    }
    pMe->m_hyhl_net_is_init = TRUE;
    #if 0
    if(HYHL_Net_IsAllow(pMe))
    {
        HYHL_Net_Open(pMe);
    }
    #endif
    return 0;
}

BOOLEAN HYHL_Net_IsInit(void)
{
    return g_hyhl_app.m_hyhl_net_is_init;
}

extern int HYHL_Net_Reset(HYHL_APP_T* pMe)
{
    ZDT_LOG("HYHL_Net_Reset");
    if(FALSE == HYHL_Net_IsInit())
    {
        return 1;
    }
    if(HYHL_Net_IsAllow(pMe))
    {
        HYHL_Net_Open(pMe);
    }
    else
    {
        HYHL_Net_Close(pMe);
    }
    return 0;
}

extern int HYHL_API_Net_Reset(void)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_HYHL_NET_RESET,NULL,0);
    return 0;
}

extern int HYHL_API_CZSB_Stop(void)
{
    if(g_is_hyhl_czsb_is_playing)
    {
        //RingSet_PlayStop();
        g_is_hyhl_czsb_is_playing = FALSE;
    }
    return 0;
}

#endif
