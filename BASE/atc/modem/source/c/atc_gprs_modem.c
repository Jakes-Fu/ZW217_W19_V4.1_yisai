/******************************************************************************
 ** File Name:      atc_gprs_modem.c                                          *
 ** Author:         Hyman.wu                                                  *
 ** Date:           14/10/2004                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains atc functions about GPRS data transfer *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 14/10/2004     Hyman.wu         Create.                                   *
 ******************************************************************************/
/*******************************************************************************
 **                        ATC Header Files                                    *
 ******************************************************************************/
#include "atc_plus_gprs.h"
#include "atc_gprs_modem.h"
#include "atc_tcp_if.h"
#ifdef _SUPPORT_GPRS_
#include "app_tcp_if.h"
#endif

#ifdef _SCM21_ATC_DEV
#define MAX_CONNECT_NUM  		6
#define DEFAULT_CONNECT_ID  	0
#endif //_SCM21_ATC_DEV

#ifdef ATC_SAT_ENABLE
uint16                         g_auto_send_len = 0;
#endif
BOOLEAN                        g_is_dns_act_pdp = FALSE;  //action: activate dns pdp by IPOPEN command
BOOLEAN                        g_is_dns_pdp_actived = FALSE;  //status: dns pdp is actived or not
BOOLEAN                        g_atc_ip_detach = FALSE; // identify if PDP deactivate is triggered by AT+IPDEACT
ATC_TX_BUF_T                   atc_tx_buf;
char                           atc_rx_buf[MAX_RX_BUF_LEN];
int                            socket_num = 0;
static uint8                   s_nsapi   = 0;  // record nsapi when PDP activated
static struct at_socket_info   s_dataconf[MAX_DATACONF_NUM+1] = {0};
static uint8                   s_sock_index_activate_pdp = 0;

ATC_CONFIG_T                  *g_gprs_atc_config_ptr;
extern RSP_RESULT_STR          g_rsp_str;
extern  MN_DUAL_SYS_E          g_current_card_id;

/******************************************************************************
 **                    Local Function Delcarations                            *
 ******************************************************************************/
LOCAL int32 ATC_CreateAndConnectTCPSocket(ATC_CONFIG_T *atc_config_ptr, unsigned long ip_addr, uint8 sock_index_id);
LOCAL int32 ATC_CreateUDPSocket(unsigned long ip_addr, uint8 sock_index_id);
LOCAL void ATC_CloseSocket(ATC_CONFIG_T *atc_config_ptr, uint32 id);
void ConvertIntToIPAddrStr(uint32 ip_addr, char *ip_str);


#ifdef _SUPPORT_GPRS_
/******************************************************************************/
//  DESCRIPTION:
//           reset gprs s_dataconf structure function
//  Global:
//  AUTHOR: Shijun.cui 2005-08-03
//  Note :  this function is used while active/deactive pdp context operation
//  History:
/******************************************************************************/
void ResetGPRSDataConf(void)
{
    int8 i;
	//@Shijun.cui only need clear state and socket
    for(i = 1; i <= MAX_DATACONF_NUM; i++)
    {
        if(s_dataconf[i].state == ATC_MODEM_ACTIVE)
        {
            s_dataconf[i].socket = NULL;
            s_dataconf[i].state = ATC_MODEM_SET;
        }
    }
}

#ifdef _SCM21_ATC_DEV
/******************************************************************************/
//  DESCRIPTION:
//           set gprs s_dataconf state.
//  Global:
//  AUTHOR: gang
//  Note :  this function is used while active/deactive pdp context operation and so on.
//  History:
/******************************************************************************/
void SetGPRSDataConfStatus(BOOLEAN is_all,uint8 index, uint32 state)
{
	uint8 i = 0;
	
	if(is_all == TRUE)
	{
	    for(i = 0; i < MAX_DATACONF_NUM; i++)
	    {
	            //s_dataconf[i].socket = NULL;
	            s_dataconf[i].modem_state = state;
	    }
	}
	else //specified socket_id
	{
	    //add by gang.li for SCM21 modem
	    //s_dataconf[index].socket = NULL;
	    s_dataconf[index].modem_state = state;
	}
	SCI_TRACE_LOW("ATC: SetGPRSDataConfStatus to %d, is_all=%d", state, is_all);
}
#endif //_SCM21_ATC_DEV

#endif

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


    SCI_TRACE_LOW("ATC: ATC_ProcessNETAPN");
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

            if(ERR_MNGPRS_NO_ERR != (err=MNGPRS_SetPdpContextEx(dual_sys,context_param,default_cid,(uint8 *)"IP",(uint8 *)mn_info.apn, pdp_addr,0,0,0)))
            {
                SCI_TRACE_LOW("ATC: MNGPRS_SetPdpContextEx FAIL,%d",err);
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
                        SCI_TRACE_LOW("ATC: set username or password error, result = %d", result);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                }
                else
                {
                    SCI_TRACE_LOW("ATC: username or password too long.");
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
    char *udp_str = "UDP";
    char *udp_str1 = "udp";
    char *tcp_str = "TCP";
    char *tcp_str1 = "tcp";
    uint8  id        = 0;
    
    SCI_TRACE_LOW("ATC: ProcessSOCKET");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {   
            //param1---ID
            if(PARAM1_FLAG)
            {
                if(PARA1.num <= MAX_DATACONF_NUM-1)
                {
                    id = PARA1.num;

                    // if the socket is still alive, not allowed to change its parameters.
                    // u must firstly do AT+IPCLOSE=ID to close this socket
                    //if(NULL != s_dataconf[id].socket)
                    if(0 != s_dataconf[id].socket)
                    {
                        SCI_TRACE_LOW("ATC: socket %d is still alive, not allowed to change it", id);
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
            if(PARAM2_FLAG)
            {
                SCI_TRACE_LOW("ATC: connection type=%s", PARA2.str_ptr->str_ptr);

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
            if(PARAM3_FLAG)      
            { 
                if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    SCI_TRACE_LOW("ATC: ProcessSOCKET, param3 is not string!");
                    return ERR_OPERATION_NOT_SUPPORTED; 
                }

                if (PARA3.str_ptr->str_len >= ATC_MAX_NETWORK_ADDR_LEN)
                {
                    SCI_TRACE_LOW("ATC: ProcessSOCKET, param3 string is too long!");
                    return ERR_OPERATION_NOT_SUPPORTED; 
                }

                if(NULL == s_dataconf[id].addr_str)
                {
                    s_dataconf[id].addr_str = (char *)SCI_ALLOC_BASEZ(ATC_MAX_NETWORK_ADDR_LEN); 
                    if(NULL==s_dataconf[id].addr_str)
                    {
                        SCI_TRACE_LOW("ATC: malloc for sock %d IP address fail", id);
                        return ERR_INVALID_INDEX;
                    }

                    SCI_TRACE_LOW("ATC: sock %d IP address malloc here", id);
                }
                else
                {
                    SCI_TRACE_LOW("ATC: sock %d IP address has been malloc, only memset it", id);
                    SCI_MEMSET(s_dataconf[id].addr_str, 0x00, ATC_MAX_NETWORK_ADDR_LEN);
                }

                memcpy(s_dataconf[id].addr_str, PARA3.str_ptr->str_ptr, PARA3.str_ptr->str_len); 
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            //param 4 --- PORT
            if(PARAM4_FLAG)
            {
                uint16 tmp_num = 0;
                if(PARA4.num <= MAX_PORT_NUM)
                {
                    tmp_num = (uint16)PARA4.num;
                    s_dataconf[id].fport = tmp_num;
                    s_dataconf[id].lport = 0;   //temporarily set, unknow parameter
                    SCI_TRACE_LOW("ATC: socket index %d port = %d",id, s_dataconf[id].fport);
                }
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            s_dataconf[id].state = ATC_MODEM_SET; 
            break;
        }
        
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
      }
     return S_ATC_SUCCESS;
}


/******************************************************************************/
//  DESCRIPTION:This function handles command AT+IPOPEN, activate PDP firstly if it is 
//                       not  activated, if PDP is activated already, DNS directly    
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPOPEN(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{ 
    MN_DUAL_SYS_E       dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32  pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
    uint32              id = 0;
    
    SCI_TRACE_LOW("ATC: ProcessIPOPEN\n");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                if(PARA1.num <= MAX_DATACONF_NUM - 1)
                {
                    id = PARA1.num;                   

                    // in case id=0
                    if(s_dataconf[id].sock_index_id != id/* || ATC_MODEM_IDLE == s_dataconf[id].state*/)
                    {
                        SCI_TRACE_LOW("ATC: socket %d is set previously!", id);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }

                    // if the socket is still alive, not allowed to reopen it.
                    // u must firstly do AT+IPCLOSE=ID to close this socket
                     //if (NULL != s_dataconf[id].socket)
                     if(0 != s_dataconf[id].socket)
                    {
                        SCI_TRACE_LOW("ATC: socket %d is still alive, not allowed to reopen it", id);
                        sprintf((char *)g_rsp_str, "+IPOPEN: %d", id);  
                        SCI_TRACE_LOW("ATC: sock %d still alive, TCP +IPOPEN=%d", id, id);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;
                    }
                }
                else 
                {  
                    SCI_TRACE_LOW("ATC: connect socket id error, not (0--9)");
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
                    
                    SCI_TRACE_LOW("ATC:PDP not activated, id = %x is set",id);
                        
                    pdp_id_arr[0]=1;  //active NO.1 pdp id
                    ret = ATC_ActivatePdpContextEx(atc_config_ptr,dual_sys, FALSE, pdp_id_arr, POC_E, MN_UNSPECIFIED);
                    //ret= MNGPRS_ActivatePdpContextEx(dual_sys, FALSE, pdp_id_arr, POC_E, MN_UNSPECIFIED);
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,pdp_id_arr[0],ATC_DOMAIN_PS, dual_sys);
                    
                    if(ERR_MNGPRS_NO_ERR != ret)
                    {
                        SCI_TRACE_LOW("ATC: active pdp fail ret %d", ret);
                        return ERR_GPRS_OPERATION_FAILURE;
                    }

                    s_sock_index_activate_pdp = id;  // record id to activate PDP
                    g_is_dns_act_pdp = TRUE;
                }
                // PDP is already activated, directly DNS 
                else
                {
                    TCPIP_HOST_HANDLE ret = 0;
                    
                    ret = sci_async_gethostbyname(s_dataconf[id].addr_str, SCI_IdentifyThread(),20000, s_nsapi);
                    SCI_TRACE_LOW("ATC:requested IP= %s, request id=%d", s_dataconf[id].addr_str, ret);

                    if(0 == ret)
                    {
                        SCI_TRACE_LOW("ATC: sock index %d DNS fail", id);
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
    
    return S_ATC_SUCCESS;
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
    uint32 id = 0;

    SCI_TRACE_LOW("ATC: ProcessIPSEND\n");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                if(PARA1.num <= MAX_DATACONF_NUM-1)
                {
                    id = PARA1.num;
                    // in case after at+ipclose=id, do at+ipsend
                    if(s_dataconf[id].sock_index_id != id
                       || ATC_MODEM_IDLE == s_dataconf[id].state)
                    {
                        SCI_TRACE_LOW("ATC: socket %d is set previously!", id);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                }
                else
                {
                    SCI_TRACE_LOW("ATC: input ID outside of connect ID range!");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
                
            if(TCP_DATACONF == s_dataconf[id].type 
               && s_dataconf[id].state != ATC_MODEM_ACTIVE)
            {
                SCI_TRACE_LOW("ATC: data state is not avtive");
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            if(PARAM2_FLAG)
            {
                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
                {
                    SCI_TRACE_LOW("ATC: ProcessIPSEND, param2 is not numeral!");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                if(1 <= PARA2.num && PARA2.num <= MAX_SEND_DATA_LENGTH)
                {
                #ifdef ATC_SAT_ENABLE
                    g_auto_send_len = (uint16)PARA2.num; 
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

            //output the '>'
            ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_GPRS);
            s_dataconf[id].state = ATC_MODEM_SEND;

            SCI_TRACE_LOW("ATC:set ctrl Z gprs end char");
                             
            break;
        }
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_DEFAULT_HANDLE;
}

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+IPCLOSE, close socKet and
//               deactivate PDP context 
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessIPCLOSE (   // return the handle result
            ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
            ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
            )
{   
    uint32 id = 0;

    SCI_TRACE_LOW("ATC: ProcessIPCLOSE");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {   
            char* ptr;
            ptr = (char*)g_rsp_str;

            //param1 --- connnection ID
            if(PARAM1_FLAG)
            {
                if(PARA1.num <= MAX_DATACONF_NUM - 1)
                {
                    id = PARA1.num;
                    if(s_dataconf[id].sock_index_id != id)
                    {
                        SCI_TRACE_LOW("ATC: the ID has not been set previously!");
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                }
                else
                {
                    SCI_TRACE_LOW("ATC: ProcessIPCLOSE, param ID out of its range!");
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
                SCI_TRACE_LOW("ATC: +IPCLOSE: %d", id);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                SCI_TRACE_LOW("ATC: socket do not exist");
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            break;
        }

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_DEFAULT_HANDLE;                                                     
}


/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CIPMUX, set multi ip connection
//  Global resource dependence : atc_config_ptr
//  AUTHOR:
//  Note :AT+CIPMUX:setting nums of allowed ip connection. 0--single ip connect; 1-- multi ip connection
/******************************************************************************/
ATC_STATUS ATC_ProcessCIPMUX( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SCM21_ATC_DEV

    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint8 i		= 0;
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
#ifdef _LENOVO_6500_
	if (!atc_config_ptr->sim_flag[dual_sys])
	{
		    return ERR_OPERATION_NOT_SUPPORTED;
        }
        else
#endif
        {
            if(PARAM1_FLAG)
            {
                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
				//0--single ip connect; 1-- multi ip connection
				if((0 == atc_config_ptr->multi_ip_connect) &&(PARA1.num == 1))
				{
					if(s_dataconf[DEFAULT_CONNECT_ID].modem_state == ATC_MODEM_IP_INITIAL)
					{
						atc_config_ptr->multi_ip_connect = PARA1.num;
					}
					else
					{
						return ERR_OPERATION_NOT_SUPPORTED;
					}
				}
				else if((1 == atc_config_ptr->multi_ip_connect) &&(PARA1.num == 0))
				{
					for(i = 0; i < MAX_CONNECT_NUM; i++)
					{
							if(s_dataconf[i].socket != NULL)
							{
								socket_num++;
							}
					}
					//modem_state for all the sockets are the same, so just check the default ID (0) modem_state;
					if((socket_num == 0) && (s_dataconf[DEFAULT_CONNECT_ID].modem_state >= ATC_MODEM_IP_INITIAL) 
						&& (s_dataconf[DEFAULT_CONNECT_ID].modem_state <= ATC_MODEM_IP_CONFIG))
					{
						atc_config_ptr->multi_ip_connect = PARA1.num;
					}
					else
					{
						return ERR_OPERATION_NOT_SUPPORTED;
					}
				}								
            }
            else
            {
                ATC_TRACE_LOW("ATC: +CIPMUX pararam1 don't exist, invalid parameter.");
                return ERR_OPERATION_NOT_SUPPORTED;
            }
		}
            break;

        case ATC_CMD_TYPE_READ:
        {			
			//read atc_config_ptr->multi_ip_connect
            sprintf((char *)g_rsp_str, "+CIPMUX: %d", atc_config_ptr->multi_ip_connect);

            ATC_TRACE_LOW("ATC:ATC_ProcessCIPMUX,READ multi_ip_connect.");
         
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

            break;
        }
		case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "+CIPMUX:(0,1)");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:

            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SCM21_ATC_DEV */
}

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CIPSTART,  
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessCIPSTART(
        ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct
        ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{ 
#ifdef _SCM21_ATC_DEV
    char *udp_str = "UDP";
    char *tcp_str = "TCP";
    uint8  id        = 0;
    
    SCI_TRACE_LOW("ATC: ProcessCIPSTART");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {   
			//AT+CIPMUX=1, 1--multi ip connect need parameter-- connection ID
			if(atc_config_ptr->multi_ip_connect == 1)
			{
				//param1---ID
	            if(PARAM1_FLAG)
	            {
	                if(PARA1.num <= MAX_CONNECT_NUM-1)
	                {
	                    id = PARA1.num;

	                    // if the socket is still alive, not allowed to change its parameters.
	                    // u must firstly do AT+IPCLOSE=ID to close this socket
	                    //if(NULL != s_dataconf[id].socket)
	                    if(0 != s_dataconf[id].socket)
	                    {
	                        SCI_TRACE_LOW("ATC: socket %d is still alive, not allowed to change it", id);
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

	            //param 2 --- type:TCP or UDP
	            if(PARAM2_FLAG)
	            {
	                SCI_TRACE_LOW("ATC: connection type=%s", PARA2.str_ptr->str_ptr);

	                if(0 == strcmp((char *)(PARA2.str_ptr->str_ptr), udp_str) )
	                {
	                    s_dataconf[id].type = UDP_DATACONF; 
	                }
	                else if(0==strcmp((char *)(PARA2.str_ptr->str_ptr), tcp_str) )
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
	            if(PARAM3_FLAG)      
	            { 
	                if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
	                {
	                    SCI_TRACE_LOW("ATC: ProcessCIPSTART, param3 is not string!");
	                    return ERR_OPERATION_NOT_SUPPORTED; 
	                }

	                if (PARA3.str_ptr->str_len >= ATC_MAX_NETWORK_ADDR_LEN)
	                {
	                    SCI_TRACE_LOW("ATC: ProcessCIPSTART, param3 string is too long!");
	                    return ERR_OPERATION_NOT_SUPPORTED; 
	                }

	                if(NULL == s_dataconf[id].addr_str)
	                {
	                    s_dataconf[id].addr_str = (char *)SCI_ALLOC_BASEZ(ATC_MAX_NETWORK_ADDR_LEN); 
	                    if(NULL==s_dataconf[id].addr_str)
	                    {
	                        SCI_TRACE_LOW("ATC: malloc for sock %d IP address fail", id);
	                        return ERR_INVALID_INDEX;
	                    }

	                    SCI_TRACE_LOW("ATC: sock %d IP address malloc here", id);
	                }
	                else
	                {
	                    SCI_TRACE_LOW("ATC: sock %d IP address has been malloc, only memset it", id);
	                    SCI_MEMSET(s_dataconf[id].addr_str, 0x00, ATC_MAX_NETWORK_ADDR_LEN);
	                }

	                memcpy(s_dataconf[id].addr_str, PARA3.str_ptr->str_ptr, PARA3.str_ptr->str_len); 
	            }
	            else
	            {
	                return ERR_INVALID_INDEX;
	            }

	            //param 4 --- PORT
	            if(PARAM4_FLAG)
	            {
	                uint16 tmp_num = 0;
	                if(PARA4.num <= MAX_PORT_NUM)
	                {
	                    tmp_num = (uint16)PARA4.num;
	                    s_dataconf[id].fport = tmp_num;
	                    s_dataconf[id].lport = 0;   //temporarily set, unknow parameter
	                    SCI_TRACE_LOW("ATC: socket index %d port = %d",id, s_dataconf[id].fport);
	                }
	            }
	            else
	            {
	                return ERR_INVALID_INDEX;
	            }
				
				s_dataconf[id].state = ATC_MODEM_SET; 
				
			}	
			else  //atc_config_ptr->multi_ip_connect == 0,  single ip connection, use default connect ID =0
			{

	            //param 1 --- MODE:TCP or UDP
	            if(PARAM1_FLAG)
	            {
	            	if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
	                {
	                    SCI_TRACE_LOW("ATC: ProcessCIPSTART, param1 is not string!");
	                    return ERR_OPERATION_NOT_SUPPORTED; 
	                }
	                SCI_TRACE_LOW("ATC: connection type=%s", PARA1.str_ptr->str_ptr);

	                if(0 == strcmp((char *)(PARA1.str_ptr->str_ptr), udp_str) )
	                {
	                    s_dataconf[DEFAULT_CONNECT_ID].type = UDP_DATACONF; 
	                }
	                else if(0==strcmp((char *)(PARA1.str_ptr->str_ptr), tcp_str) )
	                {
	                    s_dataconf[DEFAULT_CONNECT_ID].type = TCP_DATACONF;  
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

	            //param 2 --- ADDR
	            if(PARAM2_FLAG)      
	            { 
	                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
	                {
	                    SCI_TRACE_LOW("ATC: ProcessCIPSTART, param2 is not string!");
	                    return ERR_OPERATION_NOT_SUPPORTED; 
	                }

	                if (PARA2.str_ptr->str_len >= ATC_MAX_NETWORK_ADDR_LEN)
	                {
	                    SCI_TRACE_LOW("ATC: ProcessCIPSTART, param3 string is too long!");
	                    return ERR_OPERATION_NOT_SUPPORTED; 
	                }

	                if(NULL == s_dataconf[DEFAULT_CONNECT_ID].addr_str)
	                {
	                    s_dataconf[DEFAULT_CONNECT_ID].addr_str = (char *)SCI_ALLOC_BASEZ(ATC_MAX_NETWORK_ADDR_LEN); 
	                    if(NULL==s_dataconf[DEFAULT_CONNECT_ID].addr_str)
	                    {
	                        SCI_TRACE_LOW("ATC: malloc for sock %d IP address fail", DEFAULT_CONNECT_ID);
	                        return ERR_INVALID_INDEX;
	                    }

	                    SCI_TRACE_LOW("ATC: sock %d IP address malloc here", DEFAULT_CONNECT_ID);
	                }
	                else
	                {
	                    SCI_TRACE_LOW("ATC: sock %d IP address has been malloc, only memset it", DEFAULT_CONNECT_ID);
	                    SCI_MEMSET(s_dataconf[DEFAULT_CONNECT_ID].addr_str, 0x00, ATC_MAX_NETWORK_ADDR_LEN);
	                }

	                memcpy(s_dataconf[DEFAULT_CONNECT_ID].addr_str, PARA2.str_ptr->str_ptr, PARA2.str_ptr->str_len); 
	            }
	            else
	            {
	                return ERR_INVALID_INDEX;
	            }

	            //param 3 --- PORT
	            if(PARAM3_FLAG)
	            {
	                uint16 tmp_num = 0;
	                if(PARA3.num <= MAX_PORT_NUM)
	                {
	                    tmp_num = (uint16)PARA3.num;
	                    s_dataconf[DEFAULT_CONNECT_ID].fport = tmp_num;
	                    s_dataconf[DEFAULT_CONNECT_ID].lport = 0;   //temporarily set, unknow parameter
	                    SCI_TRACE_LOW("ATC: socket index %d port = %d",DEFAULT_CONNECT_ID, s_dataconf[DEFAULT_CONNECT_ID].fport);
	                }
	            }
	            else
	            {
	                return ERR_INVALID_INDEX;
	            }

				s_dataconf[DEFAULT_CONNECT_ID].state = ATC_MODEM_SET; 
			}

            break;
        }
		case ATC_CMD_TYPE_TEST:
			if(atc_config_ptr->multi_ip_connect == 0)
			{
				sprintf((char *)g_rsp_str, "+CIPSTART:(\"TCP\",\"UDP\"),,(0-65535)");
			}
			else if(atc_config_ptr->multi_ip_connect == 1)
			{
				sprintf((char *)g_rsp_str, "+CIPSTART:(0-5),(\"TCP\",\"UDP\"),,(0-65535)");
			}	
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			
			break;
			
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
      }
     return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SCM21_ATC_DEV */
}


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

    SCI_TRACE_LOW("ATC: _socketSend data_len=%d, unsend_len=%d",  atc_tx_buf.data_len, atc_tx_buf.unsend_len);
    
     if((atc_tx_buf.data_len > 0) && (atc_tx_buf.unsend_len > 0))
     {
        send_len = sci_sock_send(sock_id,atc_tx_buf.buf+(atc_tx_buf.data_len - atc_tx_buf.unsend_len),atc_tx_buf.unsend_len,0);
        SCI_TRACE_LOW("ATC:send data =%d",send_len);
        ATC_SET_CURRENT_LINK_ID_MUX(s_dataconf[sock_index].link_id);

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
            SCI_TRACE_LOW("ATC: ATC_SocketSend sock_error_code=%d ", sock_error_code);

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
LOCAL void ATC_SocketRecv(ATC_CONFIG_T *atc_config_ptr, uint32 socket, uint8 sock_index)
{   
    char  atc_rcv_hex_buff[MAX_RX_BUF_LEN*2+1] = {0};
    
    int32 rx_len = sci_sock_recv(socket,atc_rx_buf,MAX_RX_BUF_LEN,0);

    SCI_TRACE_LOW("ATC:ATC_SocketRecv rx_len =%d, buf=%s", rx_len, atc_rx_buf);

    SCI_MEMSET(g_rsp_str, 0, sizeof(g_rsp_str));
    
    while(rx_len > 0)
    {   
        uint32 len    = 0;
        uint32 hex_buff_len    = 0;

        sprintf((char *)g_rsp_str, "+IPDATA: %d,%s,%d,%d\n", sock_index, s_dataconf[sock_index].ip_addr_str, s_dataconf[sock_index].fport, rx_len);

        len = strlen((char*)g_rsp_str);

        SCI_TRACE_LOW("ATC:g_rsp_str len=%d, rx_len=%d", len, rx_len);

        ConvertBinToHex((uint8 *)atc_rx_buf,(uint16)rx_len, (uint8 *)atc_rcv_hex_buff);

        hex_buff_len = strlen(atc_rcv_hex_buff);
          
        SCI_TRACE_LOW("ATC: rcv_hex_buff =%s, hex len=%d", atc_rcv_hex_buff, hex_buff_len);

        // add '\n' to the end of received data
        if(strlen((char *)g_rsp_str) + hex_buff_len < MAX_ATC_RSP_LEN)
        {
            SCI_MEMCPY((char *)g_rsp_str+len, atc_rcv_hex_buff, hex_buff_len);
            sprintf((char *)g_rsp_str+len+hex_buff_len, "\n");
        }
        
        ATC_SET_CURRENT_LINK_ID_MUX(s_dataconf[sock_index].link_id);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

        SCI_MEMSET(atc_rx_buf, 0, sizeof(atc_rx_buf));
        SCI_MEMSET(atc_rcv_hex_buff, 0, sizeof(atc_rcv_hex_buff));

        rx_len = sci_sock_recv(socket, atc_rx_buf, MAX_RX_BUF_LEN,0); 
    }
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
    uint8       terminater_char   = 0;     // indicates ther terminater char
    BOOLEAN                  flag    = FALSE;
    uint8                             i    = 0;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != pdu_ptr); /*assert verified*/

    terminater_char              = pdu_ptr[param_len - 1];
    pdu_ptr[param_len - 1] = '\0';
    param_len                   -= 1;

    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
        {
            atc_tx_buf.data_len = 0;
            atc_tx_buf.unsend_len = 0;


            for(i = 0; i < MAX_DATACONF_NUM; i++)
            {
                if(ATC_MODEM_SEND==s_dataconf[i].state)
                {
                    s_dataconf[i].state = ATC_MODEM_ACTIVE;  // u can continue use at+ipsend command
                    SCI_TRACE_LOW("ATC: receive ESC, sock index = %d ", i);
                    break;
                }
            }

            flag = TRUE;
            break;
        }
        case CTRL_Z:
        {
            uint8 sock_index = 0;

            //copy send data to data_send buffer
            atc_tx_buf.data_len = param_len/2;
            atc_tx_buf.unsend_len = atc_tx_buf.data_len;

            for(i = 0; i < MAX_DATACONF_NUM; i++)
            {
                if(ATC_MODEM_SEND == s_dataconf[i].state)
                {
                    s_dataconf[i].state = ATC_MODEM_ACTIVE;  // u can continue use at+ipsend command
                    sock_index = i;
                    SCI_TRACE_LOW("ATC: sock index %d has process at+ipsend", i);
                    break;
                }
            }

            SCI_TRACE_LOW("ATC: atc_tx_buf.data_len=%d", atc_tx_buf.data_len);

            if(atc_tx_buf.data_len > MAX_TX_BUF_LEN)
            {
                atc_tx_buf.data_len = 0;
                atc_tx_buf.unsend_len = 0;
                return S_ATC_FAIL;
            }

            if(!ConvertHexToBin(pdu_ptr, param_len, (uint8 *)atc_tx_buf.buf))
            {
                SCI_TRACE_LOW("ATC: ConvertHexToBin fail ");
                //atc_tx_buf.data_len = 0;
                // atc_tx_buf.unsend_len = 0;
                SCI_MEMSET( atc_tx_buf.buf, 0x00, sizeof( atc_tx_buf.buf));

                return S_ATC_FAIL;
            }

            SCI_TRACE_LOW("ATC: sock %d, socket id = %d", sock_index, s_dataconf[sock_index].socket);
            ATC_SocketSend(atc_config_ptr, s_dataconf[sock_index].socket, sock_index);

            SCI_MEMSET( atc_tx_buf.buf, 0x00, sizeof( atc_tx_buf.buf));

            break;
        }

        default:
            break;  
    }

    return S_ATC_DEFAULT_HANDLE;
}

/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
void IPOPEN_ProcessActPdpContextCnf(
                        ATC_CONFIG_T      *atc_config_ptr,
                        APP_MN_GPRS_T     *sig_ptr
)
{
    TCPIP_HOST_HANDLE ret = 0;
#ifdef _MUX_ENABLE_    
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
#endif
    
    g_is_dns_act_pdp = FALSE;

    if((((APP_MN_GPRS_EXT_T*)sig_ptr)->result >= MN_GPRS_ERR_SUCCESS) && (((APP_MN_GPRS_EXT_T*)sig_ptr)->result <= MN_GPRS_ERR_SUCCESS_SINGLE_ONLY))
    {
        g_is_dns_pdp_actived = TRUE;
        s_nsapi = ((APP_MN_GPRS_EXT_T*)sig_ptr)->nsapi;
        SCI_TRACE_LOW("ATC: PDP activate success s_nsapi=%d", s_nsapi);

        ret = sci_async_gethostbyname(s_dataconf[s_sock_index_activate_pdp].addr_str,SCI_IdentifyThread(),20000,((APP_MN_GPRS_EXT_T*)sig_ptr)->nsapi);
        SCI_TRACE_LOW("ATC:reqeuested sock %d IP= %s, ret=%d", s_sock_index_activate_pdp, s_dataconf[s_sock_index_activate_pdp].addr_str, ret);

        if(0 == ret)
        {
            SCI_TRACE_LOW("ATC: sock index %d dns fail", s_sock_index_activate_pdp);
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,ERR_GPRS_OPERATION_FAILURE);
        }
        else
        {                               
            s_dataconf[s_sock_index_activate_pdp].sock_request_id = ret;
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,SOCKET_ASYNC_GETHOSTBYNAME_CNF, 0xff, ATC_DOMAIN_PS, dual_sys);
        }
    }
    else
    {
        g_is_dns_pdp_actived = FALSE;
        SCI_TRACE_LOW("ATC: sock index result unsuccess");
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
    uint8              i    = 0;
    TCPIP_SOCKET_T     sock_id    = 0;
       
    switch(sig_ptr ->SignalCode)
    {
        case SOCKET_ASYNC_GETHOSTBYNAME_CNF:
        {
            uint32 ip_addr       = 0;
            uint32 request_id    = 0;
            int32  ret           = 0;
            //char ip_str[ATC_MAX_IP_STR_LEN+1] = {0};

            SCI_TRACE_LOW("ATC: _GETHOSTBYNAME_CNF, error_code=%d",(((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr)->error_code));

            if (0 == (((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr)->error_code))
            {
                SCI_MEMCPY((uint8 *)&ip_addr,((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr)->hostent.h_addr_list[0],4);
                SCI_TRACE_LOW("ATC: ip from network = %s", inet_ntoa(ip_addr));
                
                // check request id 
                request_id = ((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr)->request_id;

                for(i = 0; i < MAX_DATACONF_NUM; i++)
                {
                    if(s_dataconf[i].sock_request_id == request_id)
                    {
                        SCI_TRACE_LOW("ATC: request_id=%d, i=%d,sock index=%d", request_id, i, s_dataconf[i].sock_index_id);

                        ConvertIntToIPAddrStr(ip_addr, s_dataconf[i].ip_addr_str);
                        SCI_TRACE_LOW("ATC: ip_addr_str=%s", s_dataconf[i].ip_addr_str);

                        if(TCP_DATACONF == s_dataconf[i].type)
                        {
                            ret = ATC_CreateAndConnectTCPSocket(atc_config_ptr, ip_addr, s_dataconf[i].sock_index_id);
                        }
                        else if(UDP_DATACONF == s_dataconf[i].type)
                        {
                            ret = ATC_CreateUDPSocket(ip_addr, s_dataconf[i].sock_index_id);
                            
                            sprintf((char *)g_rsp_str, "+IPOPEN: %d", i);  
                            SCI_TRACE_LOW("ATC: UDP +IPOPEN=%d", i);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }
                        
                        break;
                    }
                }

                if(0 != ret)
                {        
                    SCI_TRACE_LOW("ATC:  Create And Connect TCP/UDP Socket fail, ret=%d", ret);
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,ERR_GPRS_OPERATION_FAILURE);
                }
            }

            break;
        }

        case SOCKET_CONNECT_EVENT_IND:
        {
            sock_id = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)sig_ptr)->socket_id;

            SCI_TRACE_LOW("ATC: SOCKET_CONNECT_EVENT_IND");

            for(i = 0; i < MAX_DATACONF_NUM; i++)
            {
                //if((TCPIP_SOCKET_T)s_dataconf[i].socket == sock_id)
                if(s_dataconf[i].socket == sock_id)
                {
                    SCI_TRACE_LOW("ATC: sock_index %d, socket_id=%d,  i=%d", s_dataconf[i].sock_index_id, sock_id, i);
                    s_dataconf[i].state = ATC_MODEM_ACTIVE;  
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
                    SCI_TRACE_LOW("ATC: TCP +IPOPEN=%d", i);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                    SCI_TRACE_LOW("ATC: connect error, error_code =%d", (((SOCKET_CONNECT_EVENT_IND_SIG_T*)sig_ptr)->error_code));
                    sprintf((char *)g_rsp_str, "+CME:ERROR:connect error");              
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }

            break;
        }

        case SOCKET_WRITE_EVENT_IND:
        {
            SCI_TRACE_LOW("ATC: SOCKET_WRITE_EVENT_IND");

            sock_id = ((SOCKET_WRITE_EVENT_IND_SIG_T*)sig_ptr)->socket_id;

            for(i = 0; i < MAX_DATACONF_NUM; i++)
            {
                //if((TCPIP_SOCKET_T)s_dataconf[i].socket == sock_id)
                if(s_dataconf[i].socket == sock_id)
                {
                    SCI_TRACE_LOW("ATC: socket_id=%d, i=%d,sock_index=%d", sock_id, i, s_dataconf[i].sock_index_id);
                    ATC_SocketSend(atc_config_ptr, sock_id, i); 
                    break;
                }                        
            }
            break;
        }

        case SOCKET_READ_EVENT_IND:
        {
            SCI_TRACE_LOW("ATC: SOCKET_READ_EVENT_IND");
            sock_id = ((SOCKET_READ_EVENT_IND_SIG_T*)sig_ptr)->socket_id;

            for(i = 0; i < MAX_DATACONF_NUM; i++)
            {
                //if((TCPIP_SOCKET_T)s_dataconf[i].socket == sock_id)
                if(s_dataconf[i].socket == sock_id)
                {
                    SCI_TRACE_LOW("ATC: socket_id=%d, i=%d,sock_index=%d", sock_id, i, s_dataconf[i].sock_index_id);
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

            sprintf((char *)g_rsp_str, "+IPIND:%d, 0", i);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

            ATC_CloseSocket(atc_config_ptr,i);

            break;
        }

        default:
            SCI_TRACE_LOW("ATC: event type error");
            break;
    }    
}


/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : 
//  AUTHOR: 
//  Note : 
/******************************************************************************/
LOCAL int32 ATC_CreateAndConnectTCPSocket(ATC_CONFIG_T *atc_config_ptr, unsigned long ip_addr, uint8 sock_index_id)
{ 
     int   result = 0;
     struct sci_sockaddr    sa;
     int    ret      = 0;
     TCPIP_SOCKET_T     socket_id = 0;
#ifdef _MUX_ENABLE_     
     MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
#endif
     SCI_TRACE_LOW("ATC: _CreateAndConnectTCPSocket");
   
    // create socket
    socket_id = sci_sock_socket(AF_INET, SOCK_STREAM, 0, default_netid); //netid = 5, when cid = 1
    if(-1 == socket_id)
    {
        SCI_TRACE_LOW("ATC: create socket fail!");
        return -1;
    }

    ++socket_num;
    
    //s_dataconf[sock_index_id].socket = (void *)socket_id;
    s_dataconf[sock_index_id].socket = socket_id;

    SCI_TRACE_LOW("ATC: sock index %d, socket id= %d, current sock num is %d",  sock_index_id, socket_id, socket_num);
      
    // to watch for the Connect Read Wrtie Close event
    result = sci_sock_asyncselect(socket_id, SCI_IdentifyThread(), AS_CONNECT|AS_READ|AS_WRITE|AS_CLOSE); 
    if(0 != result)
    {
        SCI_TRACE_LOW("ATC: TCP SOCKET ERROR !");
        return -1;
    }
                
    sa.family  = AF_INET;
    sa.ip_addr = ip_addr;
    sa.port    = htons(s_dataconf[sock_index_id].fport); 
    SCI_TRACE_LOW("ATC:IP=%d, port=%d",sa.ip_addr, sa.port );

    // socket connect
    result = sci_sock_connect(socket_id, &sa, sizeof(sa));/*lint !e64 */
    if(-1 == result)
    {
        int sock_error_code = 0;
        
        sock_error_code = sci_sock_errno(socket_id);
        SCI_TRACE_LOW("ATC: socket connect sock_error_code=%d ", sock_error_code);
        if(EWOULDBLOCK == sock_error_code ||EINPROGRESS==sock_error_code || EISCONN==sock_error_code)
        {
            ret = 0;
        }
        else
        {
            SCI_TRACE_LOW("ATC: socket connect to server fail, ret=%d", result);
            return -1;
        }
    }
    
    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,SOCKET_CONNECT_EVENT_IND ,0xff,ATC_DOMAIN_PS, dual_sys);

    return ret;
}


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

     SCI_TRACE_LOW("ATC: Create UDP Socket");

     socket_id = sci_sock_socket(AF_INET, SOCK_DGRAM, 0, default_netid); //netid = 5, when cid = 1
     if(-1 == socket_id)
    {
        SCI_TRACE_LOW("ATC: create UDP socket fail!");
        return -1;
    }

    ++socket_num;
    
    s_dataconf[sock_index_id].socket = socket_id;

    SCI_TRACE_LOW("ATC: Create UDP Socket socket id= %d, current sock num is %d",  socket_id, socket_num);
      
    // to watch for the  Read Wrtie  event
    result = sci_sock_asyncselect(socket_id, SCI_IdentifyThread(), AS_READ|AS_WRITE); 
    if(0 != result)
    {
        SCI_TRACE_LOW("ATC: UDP SOCKET ERROR !");
        return -1;
    }
    
    sa.family  = AF_INET;
    sa.ip_addr = ip_addr;
    sa.port    = htons(s_dataconf[sock_index_id].fport); 
    SCI_TRACE_LOW("ATC: UDP IP=%d, port=%d",sa.ip_addr, sa.port );
    result = sci_sock_connect(socket_id, &sa, sizeof(sa));/*lint !e64 */
    if(-1 == result)
    {
        int sock_error_code = 0;
        sock_error_code = sci_sock_errno(socket_id);
        SCI_TRACE_LOW("ATC: UDP socket connect sock_error_code=%d ", sock_error_code);
        return -1;
    }

    return 0;
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
    ERR_MNPHONE_CODE_E ret = ERR_MNPHONE_NO_ERR;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_TRACE_LOW("ATC: ATC_ProcessIPDACT");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            ret = MNPHONE_GprsDetachEx(dual_sys, MN_PHONE_GPRS_DETACH);
            if (ERR_MNGPRS_NO_ERR != ret)
            {
                SCI_TRACE_LOW("ATC: Detach failed, error code = 0x%x", ret);
                return  ERR_INVALID_INDEX;
            }

            g_atc_ip_detach = TRUE;            
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_GPRS_DETACH_RESULT, 0xff, ATC_NO_DOMAIN, dual_sys);
            break;
        }
        
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_DEFAULT_HANDLE;
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
    
    SCI_TRACE_LOW("ATC: PDP deactivated  by network");

    g_is_dns_act_pdp = FALSE;
    g_is_dns_pdp_actived = FALSE;
    socket_num = 0;

    for(id = 0; id < MAX_DATACONF_NUM; id++)
    {
        if(s_dataconf[id].socket)
        {
            sci_sock_socketclose(s_dataconf[id].socket);
            
            SCI_TRACE_LOW("ATC: sock_id %d addr_str free here", id);
            SCI_FREE(s_dataconf[id].addr_str);
            SCI_MEMSET(&s_dataconf[id], 0x00, sizeof(s_dataconf[id]));
        }
    }

    // not AT+IPDEACT trigger the deactivate PDP    
    if (!g_atc_ip_detach)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_INVALID_INDEX);
    }
}

void ConvertIntToIPAddrStr(uint32 ip_addr, char *ip_str)
{
    char tmp_ip_str[16] = {0};
    int i = 0;
    int tmp_ip_str_len = 0;

    for (i=3; i>=0; i--)
    {
        sprintf(tmp_ip_str+tmp_ip_str_len, "%d", (ip_addr>>(i*8))& 0xff);
        tmp_ip_str_len = strlen(tmp_ip_str);

        if(i > 0)
        {
            tmp_ip_str[tmp_ip_str_len] = '.';
            tmp_ip_str_len += 1;
        }
    }

    strncpy(ip_str, tmp_ip_str, ATC_MAX_IP_STR_LEN);
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
            
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, pdp_id_arr[0], ATC_DOMAIN_PS, dual_sys);

            g_is_dns_pdp_actived = FALSE;
            SCI_TRACE_LOW("ATC: deactivate PDP success");
        }

        SCI_MEMSET(&s_dataconf[id], 0x00, sizeof(s_dataconf[id]));
    }
}
