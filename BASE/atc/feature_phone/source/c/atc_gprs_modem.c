/******************************************************************************
 ** File Name:  atc_gprs_modem.c                                              *
 ** Author:                                                                   *
 ** DATE:       04/07/2011                                                    *
 ** Description:This file contains new functions definitions about GPRS ATC.  *
 ******************************************************************************/
#ifdef ATC_SAT_ENABLE
#include "base_trc.h"
#include  "atc_gprs_modem.h"
#include  "socket_api.h"

//BOOLEAN g_is_pdp_activated = FALSE; //remove
ATC_TX_BUF_T atc_tx_buf;
char atc_rx_buf[MAX_RX_BUF_LEN];
int socket_num         = 0;

static uint8 s_nsapi   = 0;  // record nsapi when PDP activated
static uint8 s_sock_index_activate_pdp = 0;
//static BOOLEAN s_is_pdp_active_flag = FALSE; //remove
static struct at_socket_info s_dataconf[MAX_DATACONF_NUM]={0};

extern  MN_DUAL_SYS_E     g_current_card_id;
extern RSP_RESULT_STR     g_rsp_str; 

BOOLEAN                        g_is_dns_act_pdp = FALSE;  //action: activate dns pdp by IPOPEN command
BOOLEAN                        g_is_dns_pdp_actived = FALSE;  //status: dns pdp is actived or not
BOOLEAN                        g_atc_ip_detach = FALSE; // identify if PDP deactivate is triggered by AT+IPDEACT

#ifdef TCPIP_SUPPORT
LOCAL int32 ATC_CreateAndConnectTCPSocket(ATC_CONFIG_T *atc_config_ptr, unsigned long ip_addr, uint8 sock_index_id);
LOCAL int32 ATC_CreateUDPSocket(unsigned long ip_addr, uint8 sock_index_id);
LOCAL void ATC_SocketSend(ATC_CONFIG_T *atc_config_ptr,uint32 sock_id, uint8 sock_index);
LOCAL void ATC_CloseSocket(ATC_CONFIG_T *atc_config_ptr, uint32 id);
#endif

static BOOLEAN s_sock_receive_close = FALSE;

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+NETAPN, set net access
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessNETAPN(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifndef _SUPPORT_GPRS_
    return S_ATC_FAIL;
#else
    uint32                      i       = 0;
    ATC_SET_PDP_CONTEXT_REQ_T   mn_info;
    uint32 result                       = 0;
    uint8  user[MAX_PAP_USER_LEN + 1];
    uint8  passwd[MAX_PAP_PASSWD_LEN + 1];
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    BOOLEAN                 context_param[7] = {TRUE,TRUE,TRUE,FALSE,FALSE,FALSE,FALSE};
    MN_GPRS_PDP_ADDR_T      pdp_addr = {0}; 
    ERR_MNGPRS_CODE_E       err = ERR_MNGPRS_NO_ERR;


    //SCI_TRACE_LOW:"ATC: ATC_ProcessNETAPN"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_50_112_2_17_22_16_57_417,(uint8*)"");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/  
    
    SCI_MEMSET(&mn_info, 0x00, sizeof(mn_info));
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET: 
        {
            //copy the AT param indicator
            for(i = 0; i < ARR_SIZE(mn_info.param_used); i++)
            {
                mn_info.param_used[i] = cur_cmd_info->param_used[i];
            } 

            //param 1 --- APN
            if(mn_info.param_used[0])
            {
                ATC_CUR_PARAM_STRING_T  *apn_ptr = PNULL;

                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                apn_ptr = PARA1.str_ptr;
                strncpy((char *)(mn_info.apn), (char *)(apn_ptr->str_ptr), apn_ptr->str_len);

                mn_info.apn[apn_ptr->str_len] = '\0';
            }      
            else  
            {   
                //default apn setting and save
                strncpy((char *)(mn_info.apn),default_apn,strlen(default_apn));
                mn_info.apn[strlen(default_apn)] = '\0';
            }

            if(ERR_MNGPRS_NO_ERR!=(err=MNGPRS_SetPdpContextEx(dual_sys,context_param,default_cid,(uint8 *)"IP",(uint8 *)mn_info.apn, pdp_addr,0,0,0)))
            {
                //SCI_TRACE_LOW:"ATC: MNGPRS_SetPdpContextEx FAIL,%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_91_112_2_17_22_16_57_418,(uint8*)"d",err);
                return FALSE;
            }

            //param 2,3 --- USERNAME and password
            if(mn_info.param_used[1]&&mn_info.param_used[2])
            {
                if((PARA2.str_ptr->str_len <= MAX_PAP_USER_LEN) && (PARA3.str_ptr->str_len <= MAX_PAP_PASSWD_LEN))
                {
                    strncpy((char *)user, (char *)(PARA2.str_ptr->str_ptr), PARA2.str_ptr->str_len);
                    user[PARA2.str_ptr->str_len] = '\0';

                    strncpy((char *)passwd, (char *)(PARA3.str_ptr->str_ptr), PARA3.str_ptr->str_len);
                    passwd[PARA3.str_ptr->str_len] = '\0';

                    result = MNGPRS_SetPdpContextPcoEx(dual_sys,default_cid, user, passwd);   //default cid=1

                    if(result != MN_GPRS_ERR_SUCCESS)
                    {
                        //SCI_TRACE_LOW:"ATC: set username or password error, result = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_110_112_2_17_22_16_57_419,(uint8*)"d", result);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC: username or password too long."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_116_112_2_17_22_16_57_420,(uint8*)"");
                    return ERR_INVALID_INDEX;
                }
            }
            break;
        }
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }    
       return S_ATC_SUCCESS;
#endif
}

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+SOCKET,  
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessSOCKET(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifndef _SUPPORT_GPRS_
    return S_ATC_FAIL;
#else
    char *udp_str = "UDP";
    char *udp_str1 = "udp";
    char *tcp_str = "TCP";
    char *tcp_str1 = "tcp";
    uint8  id        = 0;
    
    //SCI_TRACE_LOW:"atc: ProcessSOCKET"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_142_112_2_17_22_16_57_421,(uint8*)"");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/  
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {   
            //param1---ID
            if(cur_cmd_info->param_used[0])
            {
                if(0 <= PARA1.num && PARA1.num <= MAX_DATACONF_NUM-1)
                {
                    id = PARA1.num;

                    // if the socket is still alive, not allowed to change its parameters.
                    // u must firstly do AT+IPCLOSE=ID to close this socket
                    //if(NULL != s_dataconf[id].socket)
                    if(0 != s_dataconf[id].socket)
                    {
                        //SCI_TRACE_LOW:"atc: socket %d is still alive, not allowed to change it"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_163_112_2_17_22_16_57_422,(uint8*)"d", id);
                        return ERR_INVALID_INDEX;
                    }
                    
                    s_dataconf[id].sock_index_id = id;
#ifdef _MUX_ENABLE_
                    s_dataconf[id].link_id = atc_config_ptr->current_link_id;
#endif
                }
                else
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            //param 2 --- TYPE
            if(cur_cmd_info->param_used[1])
            {
                //SCI_TRACE_LOW:"atc: connection type=%s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_185_112_2_17_22_16_57_423,(uint8*)"s", PARA2.str_ptr->str_ptr);

                if((0 == strcmp((char *)(PARA2.str_ptr->str_ptr), udp_str)) ||
                   (0 == strcmp((char *)(PARA2.str_ptr->str_ptr), udp_str1)))
                {
                    s_dataconf[id].type = UDP_DATACONF; 
                }
                else if((0==strcmp((char *)(PARA2.str_ptr->str_ptr), tcp_str))||
                        (0==strcmp((char *)(PARA2.str_ptr->str_ptr), tcp_str1)))
                {
                    s_dataconf[id].type = TCP_DATACONF;  
                }
                else
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }   
            else
            {
                return ERR_INVALID_INDEX;
            }   

            //param 3 --- ADDR
            if(cur_cmd_info->param_used[2])      
            { 
                if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    //SCI_TRACE_LOW:"atc: ProcessSOCKET, param3 is not string!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_210_112_2_17_22_16_58_424,(uint8*)"");
                    return ERR_OPERATION_NOT_SUPPORTED; 
                }

                if(NULL == s_dataconf[id].addr_str)
                {
                    s_dataconf[id].addr_str = (char *)SCI_ALLOC_BASEZ(PARA3.str_ptr->str_len*sizeof(char)+1); 
                    if(NULL==s_dataconf[id].addr_str)
                    {
                        //SCI_TRACE_LOW:"atc: malloc for sock %d IP address fail"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_219_112_2_17_22_16_58_425,(uint8*)"d", id);
                        return ERR_INVALID_INDEX;
                    }

                    //SCI_TRACE_LOW:"atc: sock %d IP address malloc here"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_223_112_2_17_22_16_58_426,(uint8*)"d", id);
                }
                else
                {
                    //SCI_TRACE_LOW:"atc: sock %d IP address has been malloc, only memset it"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_227_112_2_17_22_16_58_427,(uint8*)"d", id);
                    SCI_MEMSET(s_dataconf[id].addr_str, 0x00, sizeof(s_dataconf[id].addr_str));
                }

                memcpy(s_dataconf[id].addr_str, PARA3.str_ptr->str_ptr, PARA3.str_ptr->str_len); 
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            //param 4 --- PORT
            if(cur_cmd_info->param_used[3])
            {
                if(0 <= PARA4.num && PARA4.num <= MAX_PORT_NUM)
                {
                    s_dataconf[id].fport = PARA4.num;   
                    //SCI_TRACE_LOW:"atc: socket index %d port = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_244_112_2_17_22_16_58_428,(uint8*)"dd",id, s_dataconf[id].fport);
                }
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            s_dataconf[id].state = SET; 
            break;
        }
        
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
      }
     return S_ATC_SUCCESS;
#endif     
}


/******************************************************************************/
//  DESCRIPTION:This function handles command AT+IPOPEN, activate PDP firstly if it is 
//                         not  activated, if PDP is activated already, DNS directly    
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPOPEN(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifndef _SUPPORT_GPRS_
    return S_ATC_FAIL;
#else
#ifdef TCPIP_SUPPORT
    uint32                                                                       id = 0;
    MN_DUAL_SYS_E                                              dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32  pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
    
    //SCI_TRACE_LOW:"atc: ProcessIPOPEN\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_277_112_2_17_22_16_58_429,(uint8*)"");

    if (!atc_config_ptr)
    {
        //SCI_TRACE_LOW:"ATC: ProcessIPOPEN atc_config_ptr pointer is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_281_112_2_17_22_16_58_430,(uint8*)"");
        return S_ATC_FAIL;
    }

    if (!cur_cmd_info)
    {
        //SCI_TRACE_LOW:"ATC: ProcessIPOPEN cur_cmd_info pointer is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_287_112_2_17_22_16_58_431,(uint8*)"");
        return S_ATC_FAIL;
    }

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(cur_cmd_info->param_used[0] )
            {
                if(0 <= PARA1.num && PARA1.num <= MAX_DATACONF_NUM-1)
                {
                    id = PARA1.num;                   

                    // in case id=0
                    if(s_dataconf[id].sock_index_id != id/* || IDLE==s_dataconf[id].state*/)
                    {
                        //SCI_TRACE_LOW:"atc: socket %d is set previously!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_304_112_2_17_22_16_58_432,(uint8*)"d", id);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }

                    // if the socket is still alive, not allowed to reopen it.
                    // u must firstly do AT+IPCLOSE=ID to close this socket
                     //if (NULL != s_dataconf[id].socket)
                     if(0 != s_dataconf[id].socket)
                    {
                        //SCI_TRACE_LOW:"atc: socket %d is still alive, not allowed to reopen it"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_313_112_2_17_22_16_58_433,(uint8*)"d", id);
                        sprintf((char *)g_rsp_str, "+IPOPEN: %d", id);  
                        //SCI_TRACE_LOW:"atc: sock %d still alive, TCP +IPOPEN=%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_315_112_2_17_22_16_58_434,(uint8*)"dd", id, id);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_DEFAULT_HANDLE;
                    }
                }
                else 
                {  
                    //SCI_TRACE_LOW:"ATC: connect socket id error, not (0--9)"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_322_112_2_17_22_16_58_435,(uint8*)"");
                    return ERR_INVALID_INDEX;
                }

#ifndef ATC_SAT_ENABLE
                if(!g_is_dns_pdp_actived)
                {
                    MN_GPRS_CON_STATE_T *pdp_con_state_arr  = PNULL;
                    ERR_MNGPRS_CODE_E   ret     = ERR_MNGPRS_NO_ERR;
                    uint32    pdp_con_num = 0;
                    uint32    i;

                    ret = MNGPRS_ReadPdpContextStateEx(dual_sys, &pdp_con_state_arr, &pdp_con_num);
                    if(ret != ERR_MNGPRS_NO_ERR)
                    {
                        if(pdp_con_state_arr != PNULL)
                        {
                            SCI_FREE(pdp_con_state_arr);
                            pdp_con_state_arr = PNULL;
                        }
                    }
                    else
                    {

                        for(i = 0; i < pdp_con_num; i++)
                        {
                            if((pdp_con_state_arr[i].pdp_id == 1) && (pdp_con_state_arr[i].pdp_state == MN_CONTEXT_ACTIVATED))
                            {
                                g_is_dns_pdp_actived = TRUE;
                                break;
                            }
                        }

                        if(pdp_con_state_arr != PNULL)
                        {
                            SCI_FREE(pdp_con_state_arr);
                            pdp_con_state_arr = PNULL;
                        }
                    }

                }
#endif /*ATC_SAT_ENABLE*/ 
                
                SCI_TRACE_LOW("ATC: g_is_dns_act_pdp %d, g_is_dns_pdp_actived %d", g_is_dns_act_pdp, g_is_dns_pdp_actived);
        
                // PDP not activated, activate PDP firstly
                if(!g_is_dns_pdp_actived)
                {
                    ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;
                    
                    //SCI_TRACE_LOW:"atc:PDP not activated, id = %x is set"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_331_112_2_17_22_16_58_436,(uint8*)"d",id);
                        
                    pdp_id_arr[0]=1;  //active NO.1 pdp id
                    ret= MNGPRS_ActivatePdpContextEx(dual_sys, FALSE, pdp_id_arr, POC_E, MN_UNSPECIFIED);
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,pdp_id_arr[0],ATC_DOMAIN_PS);
                    //SCI_TRACE_LOW:"atc: active pdp ret =%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_336_112_2_17_22_16_58_437,(uint8*)"d", ret);
                    
                    if(ERR_MNGPRS_NO_ERR != ret)
                    {
                        //SCI_TRACE_LOW:"atc: active pdp fail"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_340_112_2_17_22_16_58_438,(uint8*)"");
                        return ERR_GPRS_OPERATION_FAILURE;
                    }

                    s_sock_index_activate_pdp = id;  // record id to activate PDP
                    g_is_dns_act_pdp = TRUE;
                    //SCI_TRACE_LOW:"atc: s_is_pdp_active_flag = TRUE"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_346_112_2_17_22_16_58_439,(uint8*)"");
                }
                // PDP is already activated, directly DNS 
                else
                {
                    TCPIP_HOST_HANDLE ret = 0;
                    
                    ret = sci_async_gethostbyname(s_dataconf[id].addr_str, SCI_IdentifyThread(),20000, s_nsapi);
                    //SCI_TRACE_LOW:"atc:requested IP= %s, request id=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_354_112_2_17_22_16_58_440,(uint8*)"sd", s_dataconf[id].addr_str, ret);

                    if(0 == ret)
                    {
                        //SCI_TRACE_LOW:"atc: sock index %d DNS fail"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_358_112_2_17_22_16_58_441,(uint8*)"d", id);
                        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,ERR_GPRS_OPERATION_FAILURE);
                        return S_ATC_DEFAULT_HANDLE;
                    }
                    else
                    {                               
                        s_dataconf[id].sock_request_id = ret;
                    }
                }
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            break;
        }

         default:
                return ERR_OPERATION_NOT_SUPPORTED;
        }
#endif
     return S_ATC_SUCCESS;
#endif
}

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+IPSEND, send HEX data 
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPSEND( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
)
{
#ifndef _SUPPORT_GPRS_
    return S_ATC_FAIL;
#else
    uint32 id = 0;

    //SCI_TRACE_LOW:"atc: ProcessIPSEND\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_399_112_2_17_22_16_58_443,(uint8*)"");

    if(!atc_config_ptr)
    {
        //SCI_TRACE_LOW:"atc: ProcessIPSEND atc_config_ptr pointer is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_403_112_2_17_22_16_58_444,(uint8*)"");
        return S_ATC_FAIL;
    }

    if(!cur_cmd_info)
    {
        //SCI_TRACE_LOW:"atc: ProcessIPSEND cur_cmd_info pointer is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_409_112_2_17_22_16_58_445,(uint8*)"");
        return S_ATC_FAIL;
    }

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(cur_cmd_info->param_used[0] )
            {
                if(0 <= PARA1.num && PARA1.num <= MAX_DATACONF_NUM-1)
                {
                    id = PARA1.num;
                    // in case after at+ipclose=id, do at+ipsend
                    if(s_dataconf[id].sock_index_id != id
                       || IDLE==s_dataconf[id].state)
                    {
                        //SCI_TRACE_LOW:"atc: socket %d is set previously!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_426_112_2_17_22_16_58_446,(uint8*)"d", id);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"atc: input ID outside of connect ID range!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_432_112_2_17_22_16_58_447,(uint8*)"");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
                
            //if(s_dataconf.state != ACTIVE)
            if(TCP_DATACONF == s_dataconf[id].type 
               &&s_dataconf[id].state != ACTIVE)
            {
                //SCI_TRACE_LOW:"atc: data state is not avtive"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_445_112_2_17_22_16_58_448,(uint8*)"");
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            if(cur_cmd_info->param_used[1])
            {
                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
                {
                    //SCI_TRACE_LOW:"ATC: ProcessIPSEND, param2 is not numeral!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_453_112_2_17_22_16_58_449,(uint8*)"");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                if(1<=PARA2.num && PARA2.num <= MAX_SEND_DATA_LENGTH)
                {
                    //s_ready_dataconf[PARA1.num].len = PARA2.num; 
                }
                else
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            //output the '>'
            ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_GPRS);

            s_dataconf[id].state = SEND;

            //SCI_TRACE_LOW:"atc:set ctrl Z gprs end char"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_502_112_2_17_22_16_58_451,(uint8*)"");
                             
            break;
        }
        default:
               return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_DEFAULT_HANDLE;
#endif
}

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+IPCLOSE, close socKet and
//                 deactivate PDP context 
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPCLOSE (   // return the handle result
            ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
            ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
            )
{
#ifndef _SUPPORT_GPRS_
    return S_ATC_FAIL;
#else
    uint32 id = 0;

    //SCI_TRACE_LOW:"ATC: ProcessIPCLOSE"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_528_112_2_17_22_16_58_452,(uint8*)"");
    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {   
            char* ptr;
            ptr = (char*)g_rsp_str;

            //param1 --- connnection ID
            if(cur_cmd_info->param_used[0])
            {
                if( 0 <= PARA1.num && PARA1.num <= MAX_DATACONF_NUM-1)
                {
                    id = PARA1.num;
                    //if(s_ready_dataconf[id].id  !=  id)
                    if(s_dataconf[id].sock_index_id != id)
                    {
                        //SCI_TRACE_LOW:"ATC: the ID has not been set previously!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_548_112_2_17_22_16_58_453,(uint8*)"");
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC: ProcessIPCLOSE, param ID out of its range!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_554_112_2_17_22_16_58_454,(uint8*)"");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else
            {  
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            if(0 != s_dataconf[id].socket)
            {
                ATC_CloseSocket(atc_config_ptr,id);

                    sprintf((char *)g_rsp_str, "+IPCLOSE:%d", id);
                    //SCI_TRACE_LOW:"atc: +IPCLOSE: %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_599_112_2_17_22_16_58_460,(uint8*)"d", id);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                if(s_sock_receive_close)
                {
                    s_sock_receive_close = FALSE;
                    sprintf((char *)g_rsp_str, "+SOCKETSTATUS:%d, CLOSE", id);
                    SCI_TRACE_LOW("atc: +SOCKETSTATUS: %d, CLOSE", id);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                    //SCI_TRACE_LOW:"atc: socket do not exist"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_605_112_2_17_22_16_58_461,(uint8*)"");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            break;
        }

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_DEFAULT_HANDLE;   
 #endif   
}


/******************************************************************************/
//    DESCRIPTION:This function handles command AT+IPDEACT, execute deattachment
//    Global resource dependence : 
//    AUTHOR: 
//    Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPDEACT( // return the handle result
            ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
            ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
            )
{
#ifndef _SUPPORT_GPRS_
    return S_ATC_FAIL;
#else
    //SCI_TRACE_LOW:"ATC: ATC_ProcessIPDACT"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1128_112_2_17_22_16_59_507,(uint8*)"");
    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            if(MNPHONE_GprsDetachEx(g_current_card_id, MN_PHONE_GPRS_DETACH) != ERR_MNGPRS_NO_ERR)
            {
                //SCI_TRACE_LOW:"Detach failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1138_112_2_17_22_16_59_508,(uint8*)"");
                return  ERR_INVALID_INDEX;
            }

            g_atc_ip_detach = TRUE;            
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr, APP_MN_GPRS_DETACH_RESULT, 0xff, ATC_NO_DOMAIN);
            break;
        }
        
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_DEFAULT_HANDLE;
#endif    
}

/*****************************************************************************/
//  Description : This function handles Gprs data from the SIO. 
//  Global resource dependence : g_rsp_str
//  Author:      
//  Note: modify by longting.zhao 2011.9.2 add param_len,indicating pdu len
/*****************************************************************************/
uint32 ATC_ProcessSioData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *pdu_ptr,  // pointer to the given pdu buffer
                uint16          param_len
                )
{
#ifndef _SUPPORT_GPRS_
    return S_ATC_FAIL;
#else
    uint8       terminater_char   = 0;     // indicates ther terminater char
    BOOLEAN                  flag    = FALSE;
    uint8                             i    = 0;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != pdu_ptr); /*assert verified*/

    terminater_char              = pdu_ptr[param_len - 1];
    pdu_ptr[param_len - 1] = '\0';
    param_len                   -= 1;

    //SCI_TRACE_LOW:"atc:  _ProcessSioData pdu data =%s, terminater_char=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_732_112_2_17_22_16_59_469,(uint8*)"sd", pdu_ptr, terminater_char);

    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
        {
            //  s_ready_dataconf.id = 0;
            atc_tx_buf.data_len = 0;
            atc_tx_buf.unsend_len = 0;


            for(i=0; i<MAX_DATACONF_NUM; i++)
            {
                if(SEND==s_dataconf[i].state)
                {
                    s_dataconf[i].state = ACTIVE;  // u can continue use at+ipsend command
                    //SCI_TRACE_LOW:"atc: receive ESC, sock index = %d "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_749_112_2_17_22_16_59_470,(uint8*)"d", i);
                    break;
                }
            }

            flag = TRUE;
            break;
        }

#ifdef TCPIP_SUPPORT
        case CTRL_Z:
        {
            uint8 sock_index = 0;

            //copy send data to data_send buffer
            atc_tx_buf.data_len = param_len/2;
            atc_tx_buf.unsend_len  =  atc_tx_buf.data_len;

            for(i=0; i<MAX_DATACONF_NUM; i++)
            {
                if(SEND==s_dataconf[i].state)
                {
                    s_dataconf[i].state = ACTIVE;  // u can continue use at+ipsend command
                    sock_index = i;
                    //SCI_TRACE_LOW:"atc: sock index %d has process at+ipsend"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_771_112_2_17_22_16_59_471,(uint8*)"d", i);
                    break;
                }
            }

            //SCI_TRACE_LOW:"atc: atc_tx_buf.data_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_776_112_2_17_22_16_59_472,(uint8*)"d",atc_tx_buf.data_len);

            if(atc_tx_buf.data_len > MAX_TX_BUF_LEN)
            {
                atc_tx_buf.data_len = 0;
                atc_tx_buf.unsend_len = 0;
                return S_ATC_FAIL;
            }

            if(!ConvertHexToBin(pdu_ptr, param_len, (uint8 *)atc_tx_buf.buf))
            {
                //SCI_TRACE_LOW:"atc: ConvertHexToBin fail "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_787_112_2_17_22_16_59_473,(uint8*)"");
                //atc_tx_buf.data_len = 0;
                // atc_tx_buf.unsend_len = 0;
                SCI_MEMSET( atc_tx_buf.buf, 0x00, sizeof( atc_tx_buf.buf));

                return S_ATC_FAIL;
            }

            //SCI_TRACE_LOW:"atc: sock %d, socket id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_795_112_2_17_22_16_59_474,(uint8*)"dd", sock_index, s_dataconf[sock_index].socket);
            ATC_SocketSend(atc_config_ptr, s_dataconf[sock_index].socket, sock_index);

            SCI_MEMSET( atc_tx_buf.buf, 0x00, sizeof( atc_tx_buf.buf));

            break;
        }
#endif

        default:
            break;  
    }

    return S_ATC_DEFAULT_HANDLE;
#endif    
}

#ifdef TCPIP_SUPPORT
/*****************************************************************************/
//  Description : This function handle asynchronous socket data sending
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
LOCAL void ATC_SocketSend(ATC_CONFIG_T *atc_config_ptr,uint32 sock_id, uint8 sock_index)
{
     int32            send_len     = 0;
     int32 sock_error_code    = 0;

    //SCI_TRACE_LOW:"atc: _socketSend data_len=%d, unsend_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_627_112_2_17_22_16_58_462,(uint8*)"dd",  atc_tx_buf.data_len, atc_tx_buf.unsend_len);
    
     if((atc_tx_buf.data_len > 0) && (atc_tx_buf.unsend_len > 0))
     {
        send_len = sci_sock_send(sock_id,atc_tx_buf.buf+(atc_tx_buf.data_len - atc_tx_buf.unsend_len),atc_tx_buf.unsend_len,0);
        //SCI_TRACE_LOW:"ATC:send data =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_632_112_2_17_22_16_58_463,(uint8*)"d",send_len);
        ATC_SET_CURRENT_LINK_ID(atc_config_ptr, s_dataconf[sock_index].link_id); 

        if(send_len > 0)
        {
            atc_tx_buf.unsend_len -= send_len;
            if(0 == atc_tx_buf.unsend_len)
            {
                // report message: +IPSEND: ID,LEN                 
                sprintf((char *)g_rsp_str, "+IPSEND: %d,%d", sock_index, atc_tx_buf.data_len);                             
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
               
                atc_tx_buf.data_len = 0;
                atc_tx_buf.unsend_len = 0;
             }
        }
        else
        {
            sock_error_code = sci_sock_errno(sock_id);
            //SCI_TRACE_LOW:"ATC: ATC_SocketSend sock_error_code=%d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_651_112_2_17_22_16_58_464,(uint8*)"d", sock_error_code);

            if(EPIPE==sock_error_code || ENOBUFS == sock_error_code || EWOULDBLOCK == sock_error_code)
            {
                sprintf((char *)g_rsp_str, "+CME ERROR: %d", ERR_INVALID_INDEX);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);              
            }
        }
    }    
}

/*****************************************************************************/
//  Description : This function handle asynchronous socket data receiving
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
LOCAL void ATC_SocketRecv(ATC_CONFIG_T *atc_config_ptr,uint32 socket, uint8 sock_index)
{   
    char  atc_rcv_hex_buff[MAX_RX_BUF_LEN*2+1] = {0};
    
    int32 rx_len = sci_sock_recv(socket,atc_rx_buf,MAX_RX_BUF_LEN,0);

    //SCI_TRACE_LOW:"ATC:ATC_SocketRecv rx_len =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_672_112_2_17_22_16_58_465,(uint8*)"d", rx_len);
    SCI_MEMSET(g_rsp_str, 0, sizeof(g_rsp_str));
    
    while(rx_len > 0)
    {   
        uint32                len    = 0;
        uint32 hex_buff_len    = 0;

        //SCI_TRACE_LOW:"ATC: g_rsp_str len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_680_112_2_17_22_16_59_466,(uint8*)"d", strlen((char*)g_rsp_str));
        sprintf((char *)g_rsp_str, "+IPDATA: %d,%s,%d,%d\n", sock_index, s_dataconf[sock_index].addr_str, s_dataconf[sock_index].fport, rx_len);

        len = strlen((char*)g_rsp_str);

        //SCI_TRACE_LOW:"ATC:g_rsp_str len=%d, rx_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_685_112_2_17_22_16_59_467,(uint8*)"dd", len, rx_len);

        ConvertBinToHex((uint8 *)atc_rx_buf,(uint16)rx_len, (uint8 *)atc_rcv_hex_buff);

        hex_buff_len = strlen(atc_rcv_hex_buff);
          
        //SCI_TRACE_LOW:"atc: hex len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_691_112_2_17_22_16_59_468,(uint8*)"d",hex_buff_len);

        // add '\n' to the end of received data
        if(strlen((char *)g_rsp_str)+hex_buff_len < MAX_ATC_RSP_LEN)
        {
            SCI_MEMCPY((char *)g_rsp_str+len, atc_rcv_hex_buff, hex_buff_len);
            sprintf((char *)g_rsp_str+len+hex_buff_len, "\n");
        }
        
        ATC_SET_CURRENT_LINK_ID(atc_config_ptr, s_dataconf[sock_index].link_id);  
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

        SCI_MEMSET(atc_rx_buf, 0, sizeof(atc_rx_buf));
        SCI_MEMSET(atc_rcv_hex_buff, 0, sizeof(atc_rcv_hex_buff));

        rx_len = sci_sock_recv(socket,atc_rx_buf,MAX_RX_BUF_LEN,0); 
    }
}

/******************************************************************************/
//    DESCRIPTION:
//    Global resource dependence : 
//    AUTHOR: 
//    Note : 
/******************************************************************************/
void IPOPEN_ProcessActPdpContextCnf(
                        ATC_CONFIG_T      *atc_config_ptr,
                        APP_MN_GPRS_T     *sig_ptr
)
{
    TCPIP_HOST_HANDLE ret = 0;
    
    //SCI_TRACE_LOW:"atc: ActPdpContextCnf"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_821_112_2_17_22_16_59_475,(uint8*)"");
    g_is_dns_act_pdp = FALSE;
	
    if((((APP_MN_GPRS_EXT_T*)sig_ptr)->result >= MN_GPRS_ERR_SUCCESS) && (((APP_MN_GPRS_EXT_T*)sig_ptr)->result <= MN_GPRS_ERR_SUCCESS_SINGLE_ONLY))
    {
        g_is_dns_pdp_actived = TRUE;
        s_nsapi = ((APP_MN_GPRS_EXT_T*)sig_ptr)->nsapi;
        //SCI_TRACE_LOW:"atc: PDP activate success s_nsapi=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_827_112_2_17_22_16_59_476,(uint8*)"d", s_nsapi);

        ret = sci_async_gethostbyname(s_dataconf[s_sock_index_activate_pdp].addr_str,SCI_IdentifyThread(),20000,((APP_MN_GPRS_EXT_T*)sig_ptr)->nsapi);
        //SCI_TRACE_LOW:"atc:reqeuested sock %d IP= %s, ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_830_112_2_17_22_16_59_477,(uint8*)"dsd", s_sock_index_activate_pdp, s_dataconf[s_sock_index_activate_pdp].addr_str, ret);

        if(0 == ret)
        {
            //SCI_TRACE_LOW:"atc: sock index %d dns fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_834_112_2_17_22_16_59_478,(uint8*)"d", s_sock_index_activate_pdp);
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,ERR_GPRS_OPERATION_FAILURE);
        }
        else
        {                               
            s_dataconf[s_sock_index_activate_pdp].sock_request_id = ret;
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,SOCKET_ASYNC_GETHOSTBYNAME_CNF, 0xff,ATC_DOMAIN_PS);
        }
    }
    else
    {
        g_is_dns_pdp_actived = FALSE;
        //SCI_TRACE_LOW:"atc: sock index result unsuccess"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_849_112_2_17_22_16_59_479,(uint8*)"");
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,ERR_GPRS_OPERATION_FAILURE);
    }
}

/*****************************************************************************/
//  Description : This function handle asynchronous socket after receive
//                asynchronous events
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
void ATC_ProcessSocketEvent(ATC_CONFIG_T *atc_config_ptr,xSignalHeaderRec *sig_ptr)
{
    uint8                                i    = 0;
    TCPIP_SOCKET_T     sock_id    = 0;
     
    //SCI_TRACE_LOW:"atc: _ProcessSocketEvent, sig code =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_864_112_2_17_22_16_59_480,(uint8*)"d", sig_ptr ->SignalCode);
    
    switch(sig_ptr ->SignalCode)
    {
        case SOCKET_ASYNC_GETHOSTBYNAME_CNF:
        {
            unsigned long ip_addr    = 0;
            int32                  ret      = 0;
            uint32       request_id    = 0;

            //SCI_TRACE_LOW:"atc: _GETHOSTBYNAME_CNF, error_code=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_874_112_2_17_22_16_59_481,(uint8*)"d",(((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr)->error_code));

            if (0 == (((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr)->error_code))
            {
                SCI_MEMCPY((uint8 *)&ip_addr,((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr)->hostent.h_addr_list[0],4);

                // check request id 
                request_id = ((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr)->request_id;

                for(i=0; i<MAX_DATACONF_NUM; i++)
                {
                    if(s_dataconf[i].sock_request_id == request_id)
                    {
                        //SCI_TRACE_LOW:"atc: request_id=%d, i=%d,sock index=%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_887_112_2_17_22_16_59_482,(uint8*)"ddd", request_id, i, s_dataconf[i].sock_index_id);

                        if(TCP_DATACONF == s_dataconf[i].type)
                        {
                            ret = ATC_CreateAndConnectTCPSocket(atc_config_ptr, ip_addr, s_dataconf[i].sock_index_id);
                        }
                        else if(UDP_DATACONF == s_dataconf[i].type)
                        {
                            ret = ATC_CreateUDPSocket(ip_addr, s_dataconf[i].sock_index_id);
                            
                            sprintf((char *)g_rsp_str, "+IPOPEN: %d", i);  
                            //SCI_TRACE_LOW:"atc: UDP +IPOPEN=%d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_898_112_2_17_22_16_59_483,(uint8*)"d", i);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }
                        
                        break;
                    }
                }

                if(0 != ret)
                {        
                    //SCI_TRACE_LOW:"atc:  Create And Connect TCP/UDP Socket fail, ret=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_908_112_2_17_22_16_59_484,(uint8*)"d", ret);
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,ERR_GPRS_OPERATION_FAILURE);
                }
            }

            break;
        }

        case SOCKET_CONNECT_EVENT_IND:
        {
            sock_id = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)sig_ptr)->socket_id;

            //SCI_TRACE_LOW:"atc: SOCKET_CONNECT_EVENT_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_920_112_2_17_22_16_59_485,(uint8*)"");

            for(i=0; i<MAX_DATACONF_NUM; i++)
            {
                //if((TCPIP_SOCKET_T)s_dataconf[i].socket == sock_id)
                if(s_dataconf[i].socket == sock_id)
                {
                    //SCI_TRACE_LOW:"atc: sock_index %d, socket_id=%d,  i=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_927_112_2_17_22_16_59_486,(uint8*)"ddd", s_dataconf[i].sock_index_id, sock_id, i);
                    s_dataconf[i].state = ACTIVE;  
                    break;
                }                        
            }            

            if(i < MAX_DATACONF_NUM) //&& is_connecting[i])
            {
                if (0 == (((SOCKET_CONNECT_EVENT_IND_SIG_T*)sig_ptr)->error_code))
                {
                    // to watch for the Read Wrtie Close event
                    sci_sock_asyncselect(sock_id, SCI_IdentifyThread(), AS_READ|AS_WRITE|AS_CLOSE); 

                    sprintf((char *)g_rsp_str, "+IPOPEN: %d", i);  
                    //SCI_TRACE_LOW:"atc: TCP +IPOPEN=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_941_112_2_17_22_16_59_487,(uint8*)"d", i);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    s_sock_receive_close = FALSE;
                }
                else
                {
                    //SCI_TRACE_LOW:"atc: connect error, error_code =%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_946_112_2_17_22_16_59_488,(uint8*)"d", (((SOCKET_CONNECT_EVENT_IND_SIG_T*)sig_ptr)->error_code));
                    sprintf((char *)g_rsp_str, "+CME:ERROR:connect error");              
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }

            break;
        }

        case SOCKET_WRITE_EVENT_IND:
        {
            //SCI_TRACE_LOW:"atc: SOCKET_WRITE_EVENT_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_957_112_2_17_22_16_59_489,(uint8*)"");

            sock_id = ((SOCKET_WRITE_EVENT_IND_SIG_T*)sig_ptr)->socket_id;

            for(i=0; i<MAX_DATACONF_NUM; i++)
            {
                //if((TCPIP_SOCKET_T)s_dataconf[i].socket == sock_id)
                if(s_dataconf[i].socket == sock_id)
                {
                    //SCI_TRACE_LOW:"atc: socket_id=%d, i=%d,sock_index=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_966_112_2_17_22_16_59_490,(uint8*)"ddd", sock_id, i, s_dataconf[i].sock_index_id);
                    ATC_SocketSend(atc_config_ptr, sock_id, i); 
                    break;
                }                        
            }
            break;
        }

        case SOCKET_READ_EVENT_IND:
        {
            //SCI_TRACE_LOW:"atc: SOCKET_READ_EVENT_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_976_112_2_17_22_16_59_491,(uint8*)"");
            sock_id = ((SOCKET_READ_EVENT_IND_SIG_T*)sig_ptr)->socket_id;

            for(i=0; i<MAX_DATACONF_NUM; i++)
            {
                //if((TCPIP_SOCKET_T)s_dataconf[i].socket == sock_id)
                if(s_dataconf[i].socket == sock_id)
                {
                    //SCI_TRACE_LOW:"atc: socket_id=%d, i=%d,sock_index=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_984_112_2_17_22_16_59_492,(uint8*)"ddd", sock_id, i, s_dataconf[i].sock_index_id);
                    ATC_SocketRecv(atc_config_ptr, sock_id, i);
                    break;           
                }
            }

            break;           
        }

        case SOCKET_CONNECTION_CLOSE_EVENT_IND:
        {            
            SCI_TRACE_LOW("ATC: SOCKET_CONNECTION_CLOSE_EVENT_IND");
            
            sock_id = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)sig_ptr)->socket_id;

            for(i = 0; i < MAX_DATACONF_NUM; i++)
            {
                if(s_dataconf[i].socket == sock_id)
                {
                    SCI_TRACE_LOW("ATC: sock_index %d, i=%d", s_dataconf[i].sock_index_id, i);
                    break;
                }                        
            }

            sprintf((char *)g_rsp_str, "+IP SOCKET_CONNECTION_CLOSE:%d, 0", i);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			
			s_sock_receive_close = TRUE;

            if( (s_dataconf[i].state == SEND) && (atc_config_ptr->ctrlz_end_status == ATC_CTRLZ_GPRS))
            {
                SCI_TRACE_LOW("ATC: sock_index %d, i=%d, is_ctrlz_end = FALSE", s_dataconf[i].sock_index_id, i);

                atc_config_ptr->is_ctrlz_end = FALSE;
		#ifdef PPP_USB
                SIO_ATC_SetCmdLineTerminateChar(atc_config_ptr->s3_reg, '\0',ATC_GetSioPort());
		#else
                SIO_ATC_SetCmdLineTerminateChar(atc_config_ptr->s3_reg, '\0');
		#endif				
            }

            ATC_CloseSocket(atc_config_ptr,i);

            break;
        }

        default:
            //SCI_TRACE_LOW:"ATC: event type error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_997_112_2_17_22_16_59_493,(uint8*)"");
            break;
    }    
}

/******************************************************************************/
//    DESCRIPTION:
//    Global resource dependence : 
//    AUTHOR: 
//    Note : 
/******************************************************************************/
LOCAL int32 ATC_CreateAndConnectTCPSocket(ATC_CONFIG_T *atc_config_ptr, unsigned long ip_addr, uint8 sock_index_id)
{ 
     int   result = 0;
     struct sci_sockaddr    sa;
     int    ret      = 0;
     TCPIP_SOCKET_T     socket_id = 0;

     //SCI_TRACE_LOW:"ATC: _CreateAndConnectTCPSocket"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1014_112_2_17_22_16_59_494,(uint8*)"");
   
    // create socket
    socket_id = sci_sock_socket(AF_INET, SOCK_STREAM, 0, default_netid); //netid = 5, when cid = 1
    if(-1 == socket_id)
    {
        //SCI_TRACE_LOW:"atc: create socket fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1020_112_2_17_22_16_59_495,(uint8*)"");
        return -1;
    }

    ++socket_num;
    
    //s_dataconf[sock_index_id].socket = (void *)socket_id;
    s_dataconf[sock_index_id].socket = socket_id;

    //SCI_TRACE_LOW:"ATC: sock index %d, socket id= %d, current sock num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1029_112_2_17_22_16_59_496,(uint8*)"ddd",  sock_index_id, socket_id, socket_num);
      
    // to watch for the Connect Read Wrtie Close event
    result = sci_sock_asyncselect(socket_id, SCI_IdentifyThread(), AS_CONNECT|AS_READ|AS_WRITE|AS_CLOSE); 
    if(0 != result)
    {
        //SCI_TRACE_LOW:"atc: TCP SOCKET ERROR !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1035_112_2_17_22_16_59_497,(uint8*)"");
        return -1;
    }
                
    sa.family  = AF_INET;
    sa.ip_addr = ip_addr;
    sa.port    = htons(s_dataconf[sock_index_id].fport); 
    //SCI_TRACE_LOW:"ATC:IP=%d, port=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1042_112_2_17_22_16_59_498,(uint8*)"dd",sa.ip_addr, sa.port );

    // socket connect
    result = sci_sock_connect(socket_id, &sa, sizeof(sa));
    if(-1 == result)
    {
        int sock_error_code = 0;
        
        sock_error_code = sci_sock_errno(socket_id);
        //SCI_TRACE_LOW:"ATC: socket connect sock_error_code=%d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1051_112_2_17_22_16_59_499,(uint8*)"d", sock_error_code);
        if(EWOULDBLOCK == sock_error_code ||EINPROGRESS==sock_error_code || EISCONN==sock_error_code)
        {
            ret = 0;
        }
        else
        {
            //SCI_TRACE_LOW:"atc: socket connect to server fail, ret=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1058_112_2_17_22_16_59_500,(uint8*)"d", result);
            return -1;
        }
    }
    
    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,SOCKET_CONNECT_EVENT_IND ,0xff,ATC_DOMAIN_PS);

    return ret;
}
#endif

/******************************************************************************/
//  DESCRIPTION:This function Create UDP Socket
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
LOCAL int32 ATC_CreateUDPSocket(unsigned long ip_addr, uint8 sock_index_id)
{
    int                               result = 0;
    TCPIP_SOCKET_T     socket_id = 0;
    struct sci_sockaddr             sa ;

     //SCI_TRACE_LOW:"atc: Create UDP Socket"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1079_112_2_17_22_16_59_501,(uint8*)"");

     socket_id = sci_sock_socket(AF_INET, SOCK_DGRAM, 0, default_netid); //netid = 5, when cid = 1
     if(-1 == socket_id)
    {
        //SCI_TRACE_LOW:"atc: create UDP socket fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1084_112_2_17_22_16_59_502,(uint8*)"");
        return -1;
    }

    ++socket_num;
    
    s_dataconf[sock_index_id].socket = socket_id;

    //SCI_TRACE_LOW:"ATC: Create UDP Socket socket id= %d, current sock num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1092_112_2_17_22_16_59_503,(uint8*)"dd",  socket_id, socket_num);
      
    // to watch for the  Read Wrtie  event
    result = sci_sock_asyncselect(socket_id, SCI_IdentifyThread(), AS_READ|AS_WRITE); 
    if(0 != result)
    {
        //SCI_TRACE_LOW:"atc: UDP SOCKET ERROR !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1098_112_2_17_22_16_59_504,(uint8*)"");
        return -1;
    }
    
    sa.family  = AF_INET;
    sa.ip_addr = ip_addr;
    sa.port    = htons(s_dataconf[sock_index_id].fport); 
    //SCI_TRACE_LOW:"ATC: UDP IP=%d, port=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1105_112_2_17_22_16_59_505,(uint8*)"dd",sa.ip_addr, sa.port );
    result = sci_sock_connect(socket_id, &sa, sizeof(sa));
    if(-1 == result)
    {
        int sock_error_code = 0;
        sock_error_code = sci_sock_errno(socket_id);
        //SCI_TRACE_LOW:"ATC: UDP socket connect sock_error_code=%d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1111_112_2_17_22_16_59_506,(uint8*)"d", sock_error_code);
        return -1;
    }

    return 0;
}

/******************************************************************************/
//  DESCRIPTION:This function handles PDP deactivated by network
//  Global resource dependence : 
//  AUTHOR: longting.zhao 9.29.2011
//  Note : normally, if no data activity for 2 hours, PDP will be deactivated by network
/******************************************************************************/
void ATC_ProcessPDPDeactivatedByNetwork(ATC_CONFIG_T  *atc_config_ptr)
{
    int id = 0;
    
    //SCI_TRACE_LOW:"atc: PDP deactivated  by network"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1163_112_2_17_22_17_0_509,(uint8*)"");

    g_is_dns_act_pdp = FALSE;
    g_is_dns_pdp_actived = FALSE;
	s_sock_receive_close = FALSE;
    socket_num = 0;

    for(id=0; id<MAX_DATACONF_NUM; id++)
    {
        if(0 != s_dataconf[id].socket)
        {
            sci_sock_socketclose(s_dataconf[id].socket);
            SCI_FREE(s_dataconf[id].addr_str);
            //SCI_TRACE_LOW:"atc: sock_id %d addr_str free here"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GPRS_MODEM_1176_112_2_17_22_17_0_510,(uint8*)"d", id);

            SCI_MEMSET(&s_dataconf[id], 0x00, sizeof(s_dataconf[id]));
        }
    }

    // not AT+IPDEACT trigger the deactivate PDP    
    if (!g_atc_ip_detach)
    {
        sprintf((char *)g_rsp_str, "+CME ERROR: %d", ERR_INVALID_INDEX);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);   
    }  
}

LOCAL void ATC_CloseSocket(ATC_CONFIG_T *atc_config_ptr, uint32 id)
{
    int32 result = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32  pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};

    SCI_TRACE_LOW("ATC: ATC_CloseSocket");
    
    result = sci_sock_socketclose(s_dataconf[id].socket);
    if(0 != result)
    {
        SCI_TRACE_LOW("ATC: socket close failure !result=%d ", result);
        sprintf((char *)g_rsp_str, "+CME ERROR: %d", ERR_INVALID_INDEX);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return; 
    }
    else
    {   
        SCI_FREE(s_dataconf[id].addr_str);

        SCI_TRACE_LOW("ATC: sock_id %d addr_str free here", id);

        socket_num = socket_num - 1;
        SCI_TRACE_LOW("ATC: left socket num is %d", socket_num);

        // deactivate PDP context
        if(0 == socket_num)
        {
            pdp_id_arr[0] = 1; 
			
            if( MNGPRS_DeactivatePdpContextEx(dual_sys, FALSE, pdp_id_arr) != ERR_MNGPRS_NO_ERR)
            {
                SCI_TRACE_LOW("ATC: deactive pdp context fail");
                sprintf((char *)g_rsp_str, "+CME ERROR: %d", ERR_INVALID_INDEX);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return;
            }

            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF ,pdp_id_arr[0],ATC_DOMAIN_PS);

            g_is_dns_pdp_actived = FALSE;
            SCI_TRACE_LOW("ATC: deactivate PDP success");
        }

        SCI_MEMSET(&s_dataconf[id], 0x00, sizeof(s_dataconf[id]));
    }
}

#endif /*ATC_SAT_ENABLE*/
