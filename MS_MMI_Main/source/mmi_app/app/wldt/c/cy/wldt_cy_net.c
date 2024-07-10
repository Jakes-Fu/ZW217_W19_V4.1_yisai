
#include "wldt_app.h"

#ifdef WLDT_PLAT_CY_SUPPORT

#define CY_TCP_LK_TYPE 1
#define CY_TCP_LK_TIME_OUT 300
#define CY_TCP_LK_TIME_OUT_PWON 30

#ifndef ONE_DAY_SECONDS
#define ONE_DAY_SECONDS		86400
#endif

int CY_Net_Reg(void);

static BOOLEAN s_cy_is_first_land = FALSE;
BOOLEAN         g_cy_is_land = FALSE;
uint8         g_wldt_cy_imei[9] = {0};
uint8         g_wldt_cy_imei_len = 8;

#if 1
#define CY_RCV_DATA_MAX_LEN 2048

static uint8 cy_cmd_line_buffer[CY_RCV_DATA_MAX_LEN];     /*  buffer for AT cmd line  */
static uint32 cy_cmd_line_len = 0;
static uint32 cy_data_need_len = 0;
static uint8 *cy_cmd_ptr;                     /* next loc in buffer       */
static uint8 cy_cmd_status = 0;
uint8 * cy_tcp_rx_data_buf = NULL;

uint8  CY_RCV_CMD_ProcessOneChar(uint8 cc)
{
    uint8 res = 0;
    uint16 head_len = 5;
    switch ( cy_cmd_status )
    {
        case 0:
            /*-----------------------------------------------------------------
              In the HUNT state, the characters 'A' or 'a' are looked for.  
              Everything else is ignored. If 'A' or 'a' is detected, the   
              machine goes into the FOUND_A state.                              
            -----------------------------------------------------------------*/
            if (cc == CY_NET_MSG_START)
            {
                cy_cmd_ptr = &cy_cmd_line_buffer[0];
                *cy_cmd_ptr++ = cc;
                cy_cmd_line_len = 1;
                cy_cmd_status = 1;
                cy_data_need_len = 0;
            }
            break;
            
        case 1:
            {
                if ( cy_cmd_ptr >= (cy_cmd_line_buffer + CY_RCV_DATA_MAX_LEN - 2) )
                {
                    cy_cmd_status = 0;
                    cy_cmd_ptr[0] = '\0';
                    break;
                }
                /*------------------------------------------------------------
                  If no overflow, fill buffer.  
                ------------------------------------------------------------*/
                else
                {
                    *cy_cmd_ptr++ = cc;
                    cy_cmd_line_len++;
                }
                
                if(cy_cmd_line_buffer[cy_cmd_line_len-1] == CY_NET_MSG_END)
                {
                    res = 1;
                }
                else
                {
                    WLDT_TCP_LOG("TCP CY CMD(%d) ERR End 0x%x",cy_data_need_len,cy_cmd_line_buffer[cy_cmd_line_len-1]);
                }
                cy_cmd_status = 0;
            }
            break;
    }  /* end switch ( at_cmd_prep_state ) */

    return res;
}

static void CY_RCV_CMD_Reset(void)
{
    cy_cmd_status = 0;
    return;
}

static uint32 CY_RCV_CMD_Check(uint8 * pData, uint32 Data_len)
{
    uint32 ContentLen = 0;
    uint32 head_len = 5;
    if(pData == NULL || Data_len < head_len)
    {
        return 0;
    }
    
    if(pData[0] != 0x01)
    {
        return 0;
    }
    ContentLen = Data_len-5;
        
    return Data_len;
}

static uint32 CY_CMD_CheckAll(uint8 * pData, int Data_len,uint8 **ppCmdBuf, uint32 * cmd_len)
{
    uint32 end_len = 0;
    uint32 i = 0;
    uint8 res = 0;
    uint8 * pTmp = PNULL;
    if(pData == PNULL || Data_len <= 0)
    {
        return 0;
    }
    end_len = CY_RCV_CMD_Check(pData,Data_len);
    if(end_len > 0)
    {
        CY_RCV_CMD_Reset();
        pTmp = (uint8 *)SCI_ALLOC_APPZ(end_len);
        if(pTmp == NULL)
        {
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
            res = CY_RCV_CMD_ProcessOneChar(pData[i]);
            if(res > 0)
            {
                if(res == 2)
                {
                    *ppCmdBuf = cy_tcp_rx_data_buf;
                    *cmd_len = cy_cmd_line_len;
                     cy_tcp_rx_data_buf = NULL;
                }
                else
                {
                    pTmp = (uint8 *)SCI_ALLOC_APPZ(cy_cmd_line_len);
                    if(pTmp == NULL)
                    {
                        return 0;
                    }
                    SCI_MEMCPY(pTmp,cy_cmd_line_buffer,cy_cmd_line_len);
                    *ppCmdBuf = pTmp;
                    *cmd_len = cy_cmd_line_len;
                }
                end_len = i+1;
                break;
            }
        }
    }
    return end_len;
}
#endif

static int CY_Net_GetContentLen_Send(CY_APP_T *pMe,uint8 * pData,uint32 Data_len)
{
    uint16 MsgLen = 0;
    uint16 head_len = 5;
    if(pData == NULL || Data_len < head_len)
    {
        WLDT_LOG("CY_Net_GetContentLen_Send Err NULL");
        return -1;
    }
    if(pData[0] != CY_NET_MSG_START)
    {
        WLDT_LOG("CY_Net_GetContentLen_Send Err Start");
        return -1;
    }

    MsgLen = Data_len - head_len;
            
    return (int)MsgLen;
}

int CY_Net_GetContentLen_RCV(CY_APP_T *pMe,uint8 * pData,uint32 Data_len)
{
    uint16 MsgLen = 0;
    uint16 head_len = 5;
    
    if(pData == NULL || Data_len < head_len)
    {
        WLDT_LOG("CY_Net_GetContentLen_RCV Err NULL");
        return -1;
    }
    
    if(pData[0] != CY_NET_MSG_START)
    {
        WLDT_LOG("CY_Net_GetContentLen_RCV Err Start");
        return -1;
    }

    MsgLen = Data_len - head_len;
    
    return (int)MsgLen;
}


BOOLEAN CY_Net_Is_Land(void)
{
    return  g_cy_is_land;
}

void CY_Net_Set_Land(BOOLEAN is_land)
{
    if(g_cy_is_land != is_land)
    {
        g_cy_is_land = is_land;
    }
    return;
}

extern uint32 CY_Net_TCPSend(uint8 type,uint8 * pContent,uint16 ContentLen,uint8 priority,uint8 repeat_num,uint32 timeout,WLDTTCPRCVHANDLER rcv_handle)
{
    WLDT_TCP_LINK_DATA_T * cur_data = NULL;
    uint16 len = 0;
    uint16 msg_len = 0;
    if(ContentLen == 0 || pContent == NULL )
    {
        WLDT_LOG("CY_Net_TCPSend: ERR NULL");
        return 0;
    }

    cur_data=(WLDT_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(WLDT_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        WLDT_LOG("CY_Net_TCPSend: ERR Malloc");
        return 0;
    }
    msg_len = ContentLen +7;
    cur_data->str= (uint8 *)SCI_ALLOC_APPZ(msg_len);
    if(cur_data->str == NULL)
    {
        WLDT_LOG("CY_Net_TCPSend: ERR Malloc len=%d",msg_len);
        SCI_FREE(cur_data);
        return 0;
    }
    
    cur_data->str[0] = CY_NET_MSG_START;
    cur_data->str[1] = type;
    cur_data->str[2] = 0xFF;
    cur_data->str[3] = 0x22;
    cur_data->str[4] = 0x01;
    len = 5;
    
    //数据
    SCI_MEMCPY(cur_data->str+len,pContent,ContentLen);
    len += ContentLen;

    //结束]
    cur_data->str[len++] = CY_NET_MSG_END;
    cur_data->str[len++] = CY_NET_MSG_END_2;
    
    cur_data->pMe = g_cy_app.m_cy_tcp_reg_interface;
    cur_data->len = len;
    cur_data->times = repeat_num;
    cur_data->timeout = timeout;
    cur_data->priority = priority;
    cur_data->rcv_handle = rcv_handle;
    WLDT_TCP_PostEvtTo_APP(WLDT_APP_SIGNAL_TCP_WRITE_ADD,cur_data,len);
    
    WLDT_LOG("CY_Net_TCPSend Handle=0x%x",(uint32)cur_data);
    return (uint32)cur_data;
}

extern uint32 CY_Net_TCPRespond(uint8 type,char * pContent,uint16 ContentLen)
{
    WLDT_TCP_LINK_DATA_T * cur_data = NULL;
    uint16 len = 0;
    uint16 msg_len = 0;
    
    if(ContentLen == 0 || pContent == NULL )
    {
        WLDT_LOG("CY_Net_TCPRespond: ERR NULL");
        return 0;
    }
    
    cur_data=(WLDT_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(WLDT_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        WLDT_LOG("CY_Net_TCPRespond: ERR Malloc");
        return 0;
    }
    msg_len = ContentLen +7;
    cur_data->str= (uint8 *)SCI_ALLOC_APPZ(msg_len);
    if(cur_data->str == NULL)
    {
        WLDT_LOG("CY_Net_TCPRespond: ERR Malloc len=%d",msg_len);
        SCI_FREE(cur_data);
        return 0;
    }
    
    cur_data->str[0] = CY_NET_MSG_START;
    cur_data->str[1] = type;
    cur_data->str[2] = 0xFF;
    cur_data->str[3] = 0x22;
    cur_data->str[4] = 0x01;
    len = 5;
    
    //数据
    SCI_MEMCPY(cur_data->str+len,pContent,ContentLen);
    len += ContentLen;

    //结束]
    cur_data->str[len++] = CY_NET_MSG_END;
    cur_data->str[len++] = CY_NET_MSG_END_2;
    
    cur_data->pMe = g_cy_app.m_cy_tcp_reg_interface;
    cur_data->len = len;
    cur_data->times = 0;
    cur_data->timeout = 0;
    cur_data->priority = 150;
    cur_data->rcv_handle = NULL;
    WLDT_TCP_PostEvtTo_APP(WLDT_APP_SIGNAL_TCP_WRITE_ADD,cur_data,len);
    
    WLDT_LOG("CY_Net_TCPRespond Handle=0x%x",(uint32)cur_data);
    return (uint32)cur_data;
}

static void CY_Heart_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_CY_HEART,NULL,0);
    return;
}

void CY_Heart_TimeStart(CY_APP_T *pMe,uint16 time_s)
{
    if(0 != pMe->m_cy_heart_timer_id)
    {
        MMK_StopTimer(pMe->m_cy_heart_timer_id);
        pMe->m_cy_heart_timer_id = 0;
    }
    
    pMe->m_cy_heart_timer_id = MMK_CreateTimerCallback(time_s*999, 
                                                                        CY_Heart_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    return;
}

void CY_Heart_TimeStop(CY_APP_T *pMe)
{
    if(0 != pMe->m_cy_heart_timer_id)
    {
        MMK_StopTimer(pMe->m_cy_heart_timer_id);
        pMe->m_cy_heart_timer_id = 0;
    }
}

static int CY_Net_Send_LK_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    if(Rcv_len >= 2  &&  pRcv[1] == 0x12)
    {
        WLDT_LOG("CY_Net_Send_LK_CB Success");
        return WLDT_TCP_RET_OK;
    }

    WLDT_LOG("CY_Net_Send_LK_CB  ERR Rcv_len=%d",Rcv_len);
    return WLDT_TCP_RET_ERR;
}

uint32 CY_Net_Send_LK(CY_APP_T *pMe,uint8 repeat_num,uint32 timeout)
{
    uint8 send_buf[30] = {0};
    uint16 send_len = 0;
    uint32 send_hande = 0;
    if(pMe->m_cy_is_reg == FALSE)
    {
        WLDT_LOG("CY_Net_Send_LK Err NoReg");
        return 0;
    }
    SCI_MEMCPY(send_buf,g_wldt_cy_imei,g_wldt_cy_imei_len);
    send_len = g_wldt_cy_imei_len;
    send_hande = CY_Net_TCPSend(0x11,send_buf,send_len,200,repeat_num,timeout,CY_Net_Send_LK_CB);
    pMe->m_cy_lk_send_time = WLDT_Get_TimeSecond();
    WLDT_LOG("CY_Net_Send_LK Handle=0x%x,%s",send_hande,send_buf);
    return send_hande;
}

static int CY_Net_ReceiveHandle(CY_APP_T *pMe,uint8 * pData,uint16 DataLen,uint16 Content_len)
{
    uint8 * pContent;
    uint32 cont_len = Content_len;
    uint16 head_len = 5;
    char buf[30] = {0};
    uint8 type = 0;
    int ret = 0;
    BOOLEAN is_talk= FALSE;

    type = pData[1];
    WLDT_LOG("CY_Net_ReceiveHandle: DataLen=%d, Content_len=%d,type=0x%02x",DataLen, Content_len,type);
    pContent = pData+head_len;
    if(type == 0x12)
    {
        //注册成功
    }

    return 0;
}

static int CY_Net_Receive(CY_APP_T *pMe,uint8 * pData,uint16 Data_len)
{
    uint16 MsgLen = 0;
    uint16 head_len = 5;
    if(pData == NULL || Data_len < head_len)
    {
        WLDT_LOG("CY_Net_Receive Err NULL");
        return 0;
    }
    WLDT_LOG("CY_Net_Receive Start Data_len = %d",Data_len);

    if(pData[0] != CY_NET_MSG_START)
    {
        WLDT_LOG("CY_Net_Receive Err Start");
        return 0;
    }

    MsgLen = Data_len-head_len;
        
    return CY_Net_ReceiveHandle(pMe,pData,Data_len,MsgLen);
}

static int CY_Net_HandleSendSuccess(CY_APP_T *pMe,uint8 * pData,uint32 Data_len)
{
    int ret = 0;
    int cont_len = 0;
    uint8 type = 0;
    uint8 * pContent = NULL;
    uint32 content_len = 0;
    char buf[101] = {0};
    
    cont_len = CY_Net_GetContentLen_Send(pMe,pData,Data_len);
    if(cont_len > 0)
    {
        pContent = pData+5;
        content_len = cont_len;
    }
    type = pData[1];
    
    if(type == 0x12)
    {
        WLDT_LOG("CY_Net_HandleSendSuccess--LK--  is_land=%d",pMe->m_cy_is_land);
        pMe->m_cy_reg_err_num = 0;
        if(pMe->m_cy_is_land == FALSE)
        {
            CY_Net_Set_Land(TRUE);
            pMe->m_cy_is_land = TRUE;
            if(s_cy_is_first_land == FALSE)
            {
                s_cy_is_first_land = TRUE;
            }
        }
        pMe->m_cy_allow_rereg = TRUE;
        CY_Heart_TimeStart(pMe,500);
    }
    return ret;
}

static int CY_Net_HandleSendFail(CY_APP_T *pMe,uint8 * pData,uint32 Data_len,uint16 err_id)
{
    int ret = 0;
    int cont_len = 0;
    uint8 type = 0;
    uint8 * pContent = NULL;
    uint32 content_len = 0;
    char buf[101] = {0};
    BOOLEAN is_land_fail = FALSE;
    
    cont_len = CY_Net_GetContentLen_Send(pMe,pData,Data_len);
    if(cont_len > 0)
    {
        pContent = pData+5;
        content_len = cont_len;
    }
    type = pData[1];
    
    if(ret == 0x12)
    {
        WLDT_LOG("CY_Net_HandleSendFail(%d)--LK--  is_land=%d,m_cy_reg_err_num=%d",err_id,pMe->m_cy_is_land,pMe->m_cy_reg_err_num);
        if(err_id != WLDT_TCP_ERR_CLEAR && err_id != WLDT_TCP_ERR_INCALL)
        {
            if(pMe->m_cy_is_land)
            {
                pMe->m_cy_is_land = FALSE;
                CY_Net_Set_Land(FALSE);
                //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_ROAM,ANNUN_STATE_OFF);
            }
            is_land_fail = TRUE;
            pMe->m_cy_reg_err_num++;
            if(pMe->m_cy_reg_err_num > 1)
            {
                if(pMe->m_cy_net_is_init)
                {
                    //WLDT_System_Reset(pMe);
                }
            }
            if(pMe->m_cy_reg_err_num > 4)
            {
                if(pMe->m_cy_net_is_init && s_cy_is_first_land)
                {
                    CY_Net_Reset(pMe);
                }
            }
            
            if(pMe->m_cy_reg_err_num > 4)
            {
                if((pMe->m_cy_reg_err_num%2) == 0)
                {
                    CY_Heart_TimeStart(pMe,500);
                }
                else
                {
                    CY_Net_Reg();
                }
            }
            else
            {
                CY_Net_Reg();
            }
            
        }
        else if(err_id == WLDT_TCP_ERR_INCALL)
        {
            CY_Heart_TimeStart(pMe,500);
        }
        pMe->m_cy_allow_rereg = TRUE;
    }
    
    if(WLDT_TCP_ERR_NET != err_id && WLDT_TCP_ERR_CLEAR != err_id && WLDT_TCP_ERR_READ != err_id && WLDT_TCP_ERR_INCALL != err_id)
    {
        pMe->m_cy_rsp_err_num++;
        if(pMe->m_cy_rsp_err_num >= 5)
        {
            WLDT_LOG("CY_Net_HandleSendFail RESET");
            if(is_land_fail == FALSE)
            {
                CY_Net_Reset(pMe);
            }
            pMe->m_cy_rsp_err_num = 0;
        }
    }
    return ret;
}

static void CY_Land_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    CY_Net_Reg();
    return;
}

static void CY_Land_TimeStart(CY_APP_T *pMe)
{
    if(0 != pMe->m_cy_land_timer_id)
    {
        MMK_StopTimer(pMe->m_cy_land_timer_id);
        pMe->m_cy_land_timer_id = 0;
    }
    
    pMe->m_cy_land_timer_id = MMK_CreateTimerCallback(5000, 
                                                                        CY_Land_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    return;
}

static void CY_Land_TimeStop(CY_APP_T *pMe)
{
    if(0 != pMe->m_cy_land_timer_id)
    {
        MMK_StopTimer(pMe->m_cy_land_timer_id);
        pMe->m_cy_land_timer_id = 0;
    }
    
    return;
}

static int CY_Net_Land_CallBack(void * pUser,WLDT_TCP_CB_TYPE_E cb_type,uint16 wParam, uint32 dwParam)
{
    int res = 0;
    WLDT_TCP_INTERFACE_T *pTcp = (WLDT_TCP_INTERFACE_T *)pUser;
    CY_APP_T *pMe = &g_cy_app;
    WLDT_LOG("CY_Net_Land_CallBack: pTcp=0x%xcb_type=%d,Result=%d,Data=0x%x",pTcp,cb_type,wParam,dwParam);
    
    switch(cb_type)
    {
        case WLDT_TCP_CB_DISCONNECTED:
                WLDT_LOG("CY_Net WLDT_TCP_CB_DISCONNECTED m_cy_is_land=%d",pMe->m_cy_is_land);
                if(pMe->m_cy_is_land)
                {
                    WLDT_LOG("CY_Net ReLand NET_DISCONNECTED--------------");
                    pMe->m_cy_is_land = FALSE;
                    CY_Net_Set_Land(FALSE);
  		       //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_ROAM,ANNUN_STATE_OFF);		
                    CY_Net_Reset(pMe);
                }
            break;
            
        case WLDT_TCP_CB_REG:
            {
                uint16 ret = wParam;
                uint32 err_id =  dwParam;
                if(ret == WLDT_TCP_SUCCESS)
                {
                    //注册成功
                    WLDT_LOG("CY_Net_Land TCPReg OK --------------");
                    pMe->m_cy_is_reg = TRUE;
                    pMe->m_cy_is_land = FALSE;
                    CY_Net_Set_Land(FALSE);
                    //pMe->m_cy_reg_err_num = 0;
                    pMe->m_cy_rsp_err_num = 0;
                    CY_Heart_TimeStop(pMe);
                    if(pMe->m_cy_net_first_reg == FALSE)
                    {
                        CY_Net_Send_LK(pMe,5,10000);
                        pMe->m_cy_net_first_reg = TRUE;
                    }
                    else
                    {
                        CY_Net_Send_LK(pMe,5,10000);
                    }
                    CY_Land_TimeStop(pMe);
                    
                }
                else
                {
                    //注册失败
                    WLDT_LOG("CY_Net_Land TCPReg ERR --------------");
                    CY_Heart_TimeStop(pMe);
                    if(err_id == WLDT_TCP_ERR_REGEXIST)
                    {
                        #if 0
                        if(pMe->m_wldt_is_in_call > 0 && CallUtil_Get_Calldraw_Status() == CALLDRAW_STATUS_NONE )
                        {
                            WldtCALL_API_End();
                        }
                        #endif
                        CY_Land_TimeStart(pMe);
                    }
                    else
                    {
                        pMe->m_cy_is_reg = FALSE;
                        pMe->m_cy_is_land = FALSE;
                        CY_Net_Set_Land(FALSE);
                    }
                }
            }
            break;
            
        case WLDT_TCP_CB_UNREG:
            {
                uint16 ret = wParam;
                uint32 err_id =  dwParam;
                if(ret == WLDT_TCP_SUCCESS)
                {
                    //注销成功
                    WLDT_LOG("CY_Net_Land TCPUNReg OK --------------");
                }
                else
                {
                    //注销失败
                    WLDT_LOG("CY_Net_Land TCPUNReg ERR --------------");
                }
            }
            break;
            
        case WLDT_TCP_CB_SEND:
            {
                uint16 ret = wParam;
                WLDT_TCP_LINK_DATA_T * SendData = (WLDT_TCP_LINK_DATA_T*)dwParam;
                if(ret == WLDT_TCP_SUCCESS)
                {
                    //发送成功
                    WLDT_LOG("CY_Net_TCPSend Result Success Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        CY_Net_HandleSendSuccess(pMe,SendData->str,SendData->len);                        
                    }
                }
                else
                {
                    //发送失败
                    WLDT_LOG("CY_Net_TCPSend Result FAIL Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                        CY_Net_HandleSendFail(pMe,SendData->str,SendData->len,ret);
                    }
                    
                }
            }
            break;
            
        case WLDT_TCP_CB_RCV:
            {
                uint16 ret = wParam;
                WLDT_TCP_RCV_DATA_T * RcvData = (WLDT_TCP_RCV_DATA_T *) dwParam;
                if(ret == WLDT_TCP_SUCCESS)
                {
                    //接收成功
                    if(RcvData != NULL && RcvData->pRcv!= NULL)
                    {
                        res = CY_Net_Receive(pMe,RcvData->pRcv,RcvData->len);
                    }
                    WLDT_LOG("CY_Net_Receive Over");
                }
                else
                {
                    //接收失败
                    WLDT_LOG("CY_Net_Receive FAIL");
                }
            }
            break;
                        
        default:
            break;
    }
    return res;
}

int CY_Net_Rereg(CY_APP_T* pMe)
{
    if(FALSE == CY_Net_IsInit())
    {
        return 2;
    }

    if(pMe->m_cy_is_land == FALSE && pMe->m_cy_allow_rereg)
    {
        WLDT_LOG("CY_Net_Rereg %s:%d",cy_DB_Set_Rec.ip_domain,cy_DB_Set_Rec.ip_port);
        pMe->m_cy_allow_rereg = FALSE;
        CY_Net_Reset(pMe);
    }
    return 0;
}

int CY_Net_Reg(void)
{
    WLDT_LOG("CY_Net_Reg %s:%d",cy_DB_Set_Rec.ip_domain,cy_DB_Set_Rec.ip_port);
    if(FALSE == CY_Net_IsInit())
    {
        return WLDT_TCP_RET_ERR;
    }

    WLDT_API_TCP_UnReg(g_cy_app.m_cy_tcp_reg_interface);
    WLDT_API_TCP_Reg(g_cy_app.m_cy_tcp_reg_interface,(char *)cy_DB_Set_Rec.ip_domain,cy_DB_Set_Rec.ip_port,CY_Net_Land_CallBack);
    return 0;
}

extern void CY_Net_CloseAllTimer(CY_APP_T *pMe)
{
    CY_Land_TimeStop(pMe);
    CY_Heart_TimeStop(pMe);
    return;
}

extern BOOLEAN CY_Net_IsAllow(CY_APP_T* pMe)
{
    return FALSE;
    if(cy_DB_Set_Rec.net_open &&  pMe->m_cy_net_is_init)
    {
        return TRUE;
    }
    WLDT_LOG("CY_Net_IsAllow FALSE");
    return FALSE;
}

extern BOOLEAN CY_Net_IsOpen(CY_APP_T* pMe)
{
    return pMe->m_cy_net_is_open;
}

extern int CY_Net_Open(CY_APP_T* pMe)
{
    WLDT_LOG("CY_Net_Open");
    pMe->m_cy_is_reg = FALSE;
    pMe->m_cy_is_land = FALSE;
    CY_Net_Set_Land(FALSE);
    CY_Net_Reg();
    pMe->m_cy_net_is_open = TRUE;
    return 0;
}

extern int CY_Net_Close(CY_APP_T* pMe)
{
    WLDT_LOG("CY_Net_Close");
    pMe->m_cy_is_reg = FALSE;
    pMe->m_cy_is_land = FALSE;
    CY_Net_Set_Land(FALSE);
    WLDT_API_TCP_UnReg(g_cy_app.m_cy_tcp_reg_interface);
    pMe->m_cy_net_is_open = FALSE;
   return 0;
}

uint16  CT_Net_GetIMEI(MN_DUAL_SYS_E dual_sys,char * imei_ptr)
{
	MN_IMEI_T imei;
	uint8 tmp_buf1[20] = {0};
	uint8 i=0, j=0;

	MNNV_GetIMEIEx(dual_sys, imei);		

	for(i = 0, j = 0; i < 8; i++)
	{
		tmp_buf1[j++] = (imei[i] & 0x0F);
		tmp_buf1[j++] = ((imei[i] >> 4) & 0x0F);
	}
      tmp_buf1[16] = 0xF;

	for( i=0; i<8; i++ )
	{
		imei_ptr[i] = (tmp_buf1[i*2+1] << 4)+tmp_buf1[i*2+2];
	}
    
	imei_ptr[9] = 0x00;

	return 8;
}

extern int CY_Net_Init(void)
{
    CY_APP_T * pMe = &g_cy_app;
    WLDT_LOG("CY_Net_Init");
    CT_Net_GetIMEI(MN_DUAL_SYS_1,g_wldt_cy_imei);
    if(WldtTCP_NetOpen(&(g_cy_app.m_cy_tcp_reg_interface),CY_CMD_CheckAll,10240))
    {
        WLDT_LOG("CY_Net_Init Reg OK pMe=0x%x",g_cy_app.m_cy_tcp_reg_interface);
    }
    pMe->m_cy_net_is_init = TRUE;
    #if 0
    if(CY_Net_IsAllow(pMe))
    {
        CY_Net_Open(pMe);
    }
    #endif
    return 0;
}

BOOLEAN CY_Net_IsInit(void)
{
    return g_cy_app.m_cy_net_is_init;
}

extern int CY_Net_Reset(CY_APP_T* pMe)
{
    WLDT_LOG("CY_Net_Reset");
    if(FALSE == CY_Net_IsInit())
    {
        return 1;
    }
    if(CY_Net_IsAllow(pMe))
    {
        CY_Net_Open(pMe);
    }
    else
    {
        CY_Net_Close(pMe);
    }
    return 0;
}

extern int CY_Net_TimerCheck(SCI_DATE_T dt, SCI_TIME_T tm)
{
    #if 0
    CY_APP_T* pMe = &g_cy_app;
    uint32 cur_time = WLDT_Get_TimeSecond();
    uint32 last_time = pMe->m_cy_lk_send_time;
        
    //判断网络连接是否出错当大于20分钟没有发送心跳包时网络重启
    if(WLDT_SIM_Exsit() && CY_Net_IsAllow(pMe) && last_time != 0 && (cur_time - last_time) > 1200)
    {
        WLDT_LOG("CY_Net_TimerCheck Err IN  LK");
        pMe->m_cy_lk_send_time = last_time;
        CY_Net_Reset(pMe);
        return 1;
    }
    #endif
    return 0;
}
extern int CY_API_Net_Reset(void)
{
    MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_CY_NET_RESET,NULL,0);
    return 0;
}

static int CY_API_SeNetOpen(void)
{
    if(cy_DB_Set_Rec.net_open == 0)
    {
        cy_DB_Set_Rec.net_open = 1;
        CY_DB_SET_ListModify();
        MMIWLDT_Net_Reset(TRUE);
        return 1;
    }
    return 0;
}

static int CY_API_SeNetClose(void)
{
    if(cy_DB_Set_Rec.net_open > 0)
    {
        cy_DB_Set_Rec.net_open = 0;
        CY_DB_SET_ListModify();
        MMIWLDT_Net_Reset(TRUE);
        return 1;
    }
    return 0;
}

BOOLEAN WLDT_CY_SetNetOpen(void)
{
    CY_API_SeNetOpen();
    return 0;
}

BOOLEAN WLDT_CY_SetNetClose(void)
{
   CY_API_SeNetClose();
    return 0;
}


#endif
