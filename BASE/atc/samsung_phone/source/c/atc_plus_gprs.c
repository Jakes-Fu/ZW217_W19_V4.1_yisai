/******************************************************************************
 ** File Name:      atc_plus_gprs.c                                           *
 ** Author:         Ivan.Yin                                                  *
 ** Date:           06/10/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains functions definitions about GPRS ATC.  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 06/10/2002     Ivan.Yin         Create.                                   *
 ******************************************************************************/

#include "base_trc.h"
#include "atc_plus_gprs.h"
#include "atc_basic_cmd.h"
#include "atc.h"
#include "atc_tcp_if.h"
#include "sio.h"
#ifdef _SUPPORT_GPRS_
#include <ctype.h>
#endif

#define MAX_AT_PARA_STR_LEN             128                 // for str parameter len xgxu
#define GPRS_MTU                                  1368    //the min size of SN-PDU is 140, a NPDU with length less than 1368 can be separated into 10 SN-PDUs at most.
                                                                          //sndcp send all SN-PDU belongs to a NPDU one time, If the number of SN-PDU is more than 10, it is 
                                                                          //maybe discard data in rlc.
 
#define MAX_AT_GPRSDATA_SEND_LEN   (GPRS_MTU*12)    //10 is SNDCP queue's size

#ifdef _SUPPORT_GPRS_
extern RSP_RESULT_STR      g_rsp_str;
extern const ATC_INFO_T    g_atc_info_table[];
//extern void IPOPEN_ProcessActPdpContextCnf(ATC_CONFIG_T *atc_config_ptr, APP_MN_GPRS_T *sig_ptr);
extern void ATC_ProcessPDPDeactivatedByNetwork(ATC_CONFIG_T  *atc_config_ptr);
extern BOOLEAN g_is_pdp_activated;
#endif
 
#ifdef _MUX_ENABLE_
extern BOOLEAN             ppp_dialing_in_progress;
extern BOOLEAN g_atc_ignore_one_detach_cnf;
#endif

#ifndef _MUX_ENABLE_
extern  MN_DUAL_SYS_E g_current_card_id;
#endif

typedef struct _ATC_GPRS_EV_REPORT_DETAIL_T
{
    MN_GPRS_EVENT_REPROT_INFO_T info;
    uint8   buf[MAX_ATC_RSP_LEN];
    uint32  len;
} ATC_GPRS_EV_REPORT_DETAIL_T;

#ifdef _SUPPORT_GPRS_

#ifdef MULTI_SIM_SYS_QUAD
MN_GPRS_TE_RESPONSE_MODE_E  g_atc_mt_gprs_res_mode[MN_SYS_NUMBER] = {MN_GPRS_MODE_GPRS_CS_S0,MN_GPRS_MODE_GPRS_CS_S0,MN_GPRS_MODE_GPRS_CS_S0,MN_GPRS_MODE_GPRS_CS_S0};
ATC_GPRS_EV_REPORT_DETAIL_T g_atc_mt_event_report_mode[MN_SYS_NUMBER] = {
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}
};    

MN_GPRS_TE_RESPONSE_TYPE_E  g_atc_mt_response_type[MN_SYS_NUMBER];         // value explication see 0707-10.1.10 +CGANS
#ifdef _GPRS_FULL_FUNCTION_
BOOLEAN                     g_atc_is_all_pdp[MN_SYS_NUMBER] = {0,0,0,0};
BOOLEAN                     g_atc_is_network_request[MN_SYS_NUMBER] = {0,0,0,0};
#endif
uint32                      g_atc_te_pdp_context[MN_SYS_NUMBER][MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}};
#elif defined MULTI_SIM_SYS_TRI
static MN_GPRS_TE_RESPONSE_MODE_E  g_atc_mt_gprs_res_mode[MN_SYS_NUMBER] = {MN_GPRS_MODE_GPRS_CS_S0,MN_GPRS_MODE_GPRS_CS_S0,MN_GPRS_MODE_GPRS_CS_S0};

ATC_GPRS_EV_REPORT_DETAIL_T g_atc_mt_event_report_mode[MN_SYS_NUMBER] = {
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}
};    

static MN_GPRS_TE_RESPONSE_TYPE_E  g_atc_mt_response_type[MN_SYS_NUMBER];         // value explication see 0707-10.1.10 +CGANS

#ifdef _GPRS_FULL_FUNCTION_
static BOOLEAN                     g_atc_is_all_pdp[MN_SYS_NUMBER] = {0,0,0};
static BOOLEAN                     g_atc_is_network_request[MN_SYS_NUMBER] = {0,0,0};
#endif
static uint32                      g_atc_te_pdp_context[MN_SYS_NUMBER][MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}};
#elif defined MULTI_SIM_SYS_DUAL
static MN_GPRS_TE_RESPONSE_MODE_E  g_atc_mt_gprs_res_mode[MN_SYS_NUMBER] = {MN_GPRS_MODE_GPRS_CS_S0,MN_GPRS_MODE_GPRS_CS_S0};
ATC_GPRS_EV_REPORT_DETAIL_T g_atc_mt_event_report_mode[MN_SYS_NUMBER] = {
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}
};    
static MN_GPRS_TE_RESPONSE_TYPE_E  g_atc_mt_response_type[MN_SYS_NUMBER];         // value explication see 0707-10.1.10 +CGANS

#ifdef _GPRS_FULL_FUNCTION_
static BOOLEAN                     g_atc_is_all_pdp[MN_SYS_NUMBER] = {0,0};
static BOOLEAN                     g_atc_is_network_request[MN_SYS_NUMBER] = {0,0};
#endif

static uint32                      g_atc_te_pdp_context[MN_SYS_NUMBER][MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}};
#else 
static MN_GPRS_TE_RESPONSE_MODE_E  g_atc_mt_gprs_res_mode[MN_SYS_NUMBER] = {MN_GPRS_MODE_GPRS_CS_S0};
ATC_GPRS_EV_REPORT_DETAIL_T g_atc_mt_event_report_mode[MN_SYS_NUMBER] = {
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}
};    
static MN_GPRS_TE_RESPONSE_TYPE_E  g_atc_mt_response_type[MN_SYS_NUMBER];         // value explication see 0707-10.1.10 +CGANS
#ifdef _GPRS_FULL_FUNCTION_
static BOOLEAN                     g_atc_is_all_pdp[MN_SYS_NUMBER] = {0};
static BOOLEAN                     g_atc_is_network_request[MN_SYS_NUMBER] = {0};
#endif
static uint32                      g_atc_te_pdp_context[MN_SYS_NUMBER][MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0,0,0,0,0,0,0,0,0,0,0};
#endif

#endif

#ifdef _SUPPORT_GPRS_
typedef struct mngprs_dummy_data {
    uint32 data_size;
    uint32 gprs_mtu;
    uint16 nsapi;
} ATC_DUMMY_DATA_STRUCT;
static ATC_DUMMY_DATA_STRUCT s_atc_dummy_data = {0};
#endif


#ifdef _SUPPORT_GPRS_
/******************************************************************************/  
//    DESCRIPTION: Set QOS for R99 element
//    Global resource dependence :  
//    AUTHOR: Tracy Zhang
//    Note : 
/******************************************************************************/ 
LOCAL ATC_STATUS SetQos_R99(
                            ATC_CONFIG_T        *atc_config_ptr, 
                            ATC_CUR_INFO_LIST_T *cur_cmd_info,   //IN:
                            BOOLEAN             is_minqos        //IN:
                            );
                            

#ifdef _GPRS_FULL_FUNCTION_
/******************************************************************************/  
//    DESCRIPTION: read QOS for R99 element
//    Global resource dependence :  
//    AUTHOR: Tracy Zhang
//    Note : 
/******************************************************************************/ 
LOCAL ATC_STATUS ReadQos_R99(
                             ATC_CONFIG_T   *atc_config_ptr, //OUT:
                             BOOLEAN        is_minqos        //IN:
                             );
#if 0
/*****************************************************************************/
//  Function    : HandleCGANS
//  Description : 
//                set CGANS var
//  Return      : 
//  Global      : none
//  Note        :
//  History     :
/*****************************************************************************/
LOCAL ERR_MNGPRS_CODE_E HandleCGANS(
                MN_DUAL_SYS_E dual_sys, 
                MN_GPRS_TE_RESPONSE_TYPE_E res_type,
                BOOLEAN is_all, 
                int32 pdp_id
                );
#endif
/*****************************************************************************/
//  Function    : HandleCGEREP
//  Description : 
//                set event report mode and buffer control
//  Return      : 
//  Note        :
/*****************************************************************************/
LOCAL ERR_MNGPRS_CODE_E HandleCGEREP(
                MN_DUAL_SYS_E                dual_sys,        //IN:
                ATC_CONFIG_T                 *atc_config_ptr, //IN
                MN_GPRS_EVENT_REPORT_MODE_E  mode,            //IN:
                MN_GPRS_EVENT_REPORT_BUFFER_E buffer          //IN:
                );

/*****************************************************************************/
//  Description : 1. turn on or off: Manual response to a network request for PDP context activation 
//                2. Based on S0
//  Global      : none
//  Author      : 
//  Note        :
//  History     :
/*****************************************************************************/
LOCAL void SetResMode( //return whether success or failure
                MN_DUAL_SYS_E dual_sys,
                MN_GPRS_TE_RESPONSE_MODE_E res_mode //the requested response mode
                );

/*****************************************************************************/
//  Description : show reponse mode set by GetResMode
//  Global      : none
//  Author      : 
//  Note        :
//  History     :
/*****************************************************************************/
LOCAL MN_GPRS_TE_RESPONSE_MODE_E  GetResMode( //return whether success or failure
                MN_DUAL_SYS_E dual_sys
                );
#endif /*_GPRS_FULL_FUNCTION_*/
#endif /*_SUPPORT_GPRS_*/
 
/******************************************************************************/ 
//    DESCRIPTION:This function handles command AT+CGDCONT, set pdp context
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : Modified by Hyman.wu
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGDCONT( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                       )
{
#ifdef _SUPPORT_GPRS_
    uint32   i;
    ATC_SET_PDP_CONTEXT_REQ_T  mn_info;

    ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;    

    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //ATC_TRACE:"ATGPRS:ATC_ProcessCGDCONT\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_207_112_2_17_22_17_48_772,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
    
        //copy the at param indicator
        for (i = 0;i < sizeof(mn_info.param_used)/sizeof(mn_info.param_used[0]); i++)
        {
            mn_info.param_used[i] = cur_cmd_info->param_used[i];
        }

        //param 1 --- cid
        if (mn_info.param_used[0])
        {
            mn_info.pdp_id = PARA1.num;
        }
        else
        {
            return ERR_INVALID_INDEX;
        }
        
        //param 2 --- PDP_type
        if (mn_info.param_used[1])
        {
            ATC_CUR_PARAM_STRING_T *type_ptr = PNULL;
            
        
            if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)      
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }    
            
            type_ptr = PARA2.str_ptr;
            for (i = 0; i < type_ptr->str_len; i++)
            {
                /*lint -e718*/
                /*lint -e746*/
                /*lint -e628*/
                if (isalpha(*(type_ptr->str_ptr+ i)))/*lint !e48*/
                {
                    *(type_ptr->str_ptr + i) = toupper(*(type_ptr->str_ptr + i));
                }
                /*lint +e746*/
                /*lint +e718*/
            }

            // only supporting IP pdp type currently
            if (strcmp((char *)type_ptr->str_ptr, "IP"))
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
                      
           
           SCI_MEMCPY(mn_info.pdp_type, type_ptr->str_ptr, type_ptr->str_len);
                       
           mn_info.pdp_type[type_ptr->str_len] = '\0';
           
        }
        //param 3 --- APN
        if (mn_info.param_used[2])
        {
            ATC_CUR_PARAM_STRING_T *apn_ptr  =  PNULL;
            
            if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }      
            apn_ptr = PARA3.str_ptr;
            
            SCI_MEMCPY(mn_info.apn, apn_ptr->str_ptr, apn_ptr->str_len);

            mn_info.apn[apn_ptr->str_len] = '\0';
        }
        //param 4 --- PDP_address
        if (mn_info.param_used[3])
        {
            char    *token  =  PNULL;
            
            mn_info.pdp_address.length = 0;
                        
            //if <pdp_address> set to 0,atc will lead to exception                         
            if(PARA4.str_ptr == NULL)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            
            if(PARAM4_TYPE != ATC_CMD_PARAM_TYPE_STRING)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            
            token = (char *)PARA4.str_ptr->str_ptr;
            if (token != NULL)
            {
                token = strtok(token,".");
                while(token != NULL)
                {
                    //SCI_TRACE_LOW("%s",token);
                    mn_info.pdp_address.value_arr[mn_info.pdp_address.length] = atoi(token);
                    if(mn_info.pdp_address.value_arr[mn_info.pdp_address.length] == 0)
                    {
                        if(*token != '0')
                        {
                            //parse token error!
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }
                    } 
                    token = strtok(NULL,".");
                    mn_info.pdp_address.length++;
                }
            }
            
        }
        //param 5 --- d_comp
        if (mn_info.param_used[4])
        {
            if ((0 <= PARA5.num) && (PARA5.num <= 3))
            {
                mn_info.d_comp = PARA5.num;
            }
            else
            {
                // Other values are reserved.
                mn_info.param_used[4] = FALSE;
            }
        }
        
        //param 6 --- h_comp
        if (mn_info.param_used[5])
        {
            if ((0 <= PARA6.num) && (PARA6.num <= 4))
            {
                mn_info.h_comp = PARA6.num;
            }
            else
            {
                // Other values are reserved.
                mn_info.param_used[5] = FALSE;
            }
        }
        
        //param 7 --- <pd1>, бн <pdN>
        if (mn_info.param_used[6])
        {
            ATC_CUR_PARAM_STRING_T *pco_ptr;
            
            pco_ptr = PARA7.str_ptr;
            
            if(pco_ptr != NULL)
            {
                strncpy((char *)mn_info.pco, (char*)pco_ptr->str_ptr, pco_ptr->str_len);
                mn_info.pco[pco_ptr->str_len] = '\0';
            }    
        }
        
        //+CGDCONT= <cid>, causes the values for context number <cid> to become undefined
        if ((mn_info.param_used[1] == FALSE)&&
            (mn_info.param_used[2] == FALSE)&&
            (mn_info.param_used[3] == FALSE)&&
            (mn_info.param_used[4] == FALSE)&&
            (mn_info.param_used[5] == FALSE)&&
            (mn_info.param_used[6] == FALSE) )
        {            
            ret = MNGPRS_DeletePdpContextEx(dual_sys, !mn_info.param_used[0],mn_info.pdp_id);/*lint !e730*/
            
            if (ERR_MNGPRS_NO_ERR != ret)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
        }
        else
        {
            ret = MNGPRS_SetPdpContextEx(dual_sys, mn_info.param_used,
                                       mn_info.pdp_id,
                                       mn_info.pdp_type,
                                       mn_info.apn,
                                       mn_info.pdp_address,
                                       mn_info.d_comp,
                                       mn_info.h_comp,
                                       mn_info.pco);
            
            if (ERR_MNGPRS_NO_ERR != ret)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
        }

        break;
        
    case ATC_CMD_TYPE_READ:
    {
#ifdef _GPRS_FULL_FUNCTION_
        MN_GPRS_PDP_CON_T    *pdp_con_arr =  PNULL;
        uint32   pdp_con_num = 0;
        uint8                        j = 0;
            
        ret = MNGPRS_ReadPdpContextEx(dual_sys, &pdp_con_arr,&pdp_con_num);
        if(ret != ERR_MNGPRS_NO_ERR)
        {
            if(pdp_con_arr != PNULL)
            {
                SCI_FREE(pdp_con_arr);
            }
            
            return ERR_OPERATION_NOT_SUPPORTED;
        }
       
        if (0 == pdp_con_num) 
        {
            // There isn't PDP context.
            sprintf((char*)g_rsp_str,"+CGDCONT: "); 
        }
        
        for(i = 0; i <pdp_con_num;i++)
        {
            if (0 == i)
            {
                sprintf((char*)g_rsp_str,"+CGDCONT: %ld,%s,\"%s\",\"",
                                            pdp_con_arr[i].pdp_id,
                                            pdp_con_arr[i].pdp_addr_type,
                                            pdp_con_arr[i].apn);
            }
            else
            {
                sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str),"\r\n+CGDCONT: %ld,%s,\"%s\",\"",
                                               pdp_con_arr[i].pdp_id,
                                               pdp_con_arr[i].pdp_addr_type,
                                               pdp_con_arr[i].apn);
            }
             
            for( j=0; j<pdp_con_arr[i].pdp_addr_str.length-1; j++ )
            {
                sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str),"%d.",pdp_con_arr[i].pdp_addr_str.value_arr[j]);
             }

            if(0 == j)
            {
                sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str),"0.0.0.0\"");
            }
            else
            {
                sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str),"%d\"",pdp_con_arr[i].pdp_addr_str.value_arr[j]);
            }
            
            sprintf( (char*)g_rsp_str+strlen((char*)g_rsp_str),",%ld",pdp_con_arr[i].d_comp);
            sprintf( (char*)g_rsp_str+strlen((char*)g_rsp_str),",%ld",pdp_con_arr[i].h_comp);
        }

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        
        if(PNULL != pdp_con_arr)
        {
            SCI_FREE(pdp_con_arr);
        }
#endif/*_GPRS_FULL_FUNCTION_*/
        break;
    }    
    
    case ATC_CMD_TYPE_TEST:
                sprintf((char *)g_rsp_str, "+CGDCONT: (1-11),(\"IP\",\"ip\"),(0,1),(0,1)");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
                
    default:        
        return ERR_OPERATION_NOT_SUPPORTED;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/ 
//    DESCRIPTION:This function handles command AT+CGPCO, set pco
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Shijun.cui
//    Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGPCO( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                )
{
#ifdef _SUPPORT_GPRS_
    uint32                                          pdp_id = 0;
    uint8                                               type = 0;
    uint8           user[MAX_PAP_USER_LEN + 1] ={0};
    uint8  passwd[MAX_PAP_PASSWD_LEN + 1] ={0};
    uint32                                            result = 0;
    MN_DUAL_SYS_E                         dual_sys = ATC_CURRENT_SIM_ID_MUX;
  
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG && PARAM4_FLAG)
            {
                type = PARA1.num;
                pdp_id = PARA4.num;
                
                if(type)//HEX type
                {
                    if(PARA2.str_ptr->str_len <= MAX_PAP_USER_LEN * 2 || PARA3.str_ptr->str_len <= MAX_PAP_PASSWD_LEN * 2)
                    {
                        ConvertHexToBin(PARA2.str_ptr->str_ptr,PARA2.str_ptr->str_len, user);
                        ConvertHexToBin(PARA3.str_ptr->str_ptr,PARA3.str_ptr->str_len, passwd);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"ATC: pco user or passwd too long."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_518_112_2_17_22_17_49_773,(uint8*)"");
                        return ERR_INVALID_INDEX;
                    }
                }
                else // ASCII type
                {
                    if(PARA2.str_ptr->str_len <= MAX_PAP_USER_LEN && PARA3.str_ptr->str_len <= MAX_PAP_PASSWD_LEN)
                    {
                        SCI_MEMCPY(user, PARA2.str_ptr->str_ptr, PARA2.str_ptr->str_len);
                        SCI_MEMCPY(passwd, PARA3.str_ptr->str_ptr, PARA3.str_ptr->str_len);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"ATC: pco user or passwd too long."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_531_112_2_17_22_17_49_774,(uint8*)"");
                        return ERR_INVALID_INDEX;
                    }
                }
                
                result = MNGPRS_SetPdpContextPcoEx(dual_sys, pdp_id, user, passwd);
                if(result != MN_GPRS_ERR_SUCCESS)
                {
                    //SCI_TRACE_LOW:"ATC: set pco error, result = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_539_112_2_17_22_17_49_775,(uint8*)"d", result);
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: set pco error parameter"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_545_112_2_17_22_17_49_776,(uint8*)"");
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            break;
        }
            
        case ATC_CMD_TYPE_READ:
        {
#ifdef _GPRS_FULL_FUNCTION_
            MN_GPRS_PCO_T    *pdp_pco_ptr = PNULL;
            uint32             pdp_context_num = 0;
            uint32                                       i = 0;  
                    
            result = MNGPRS_ReadPdpContextPcoEx(dual_sys, &pdp_pco_ptr,&pdp_context_num);
            if(result != MN_GPRS_ERR_SUCCESS)
            {
                if(pdp_pco_ptr != NULL)
                {
                    SCI_FREE(pdp_pco_ptr);
                }
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            
            if(pdp_context_num == 0)
            {
                sprintf((char*)g_rsp_str,"+CGPCO: ");
            }
            
            for(i = 0; i < pdp_context_num; i++)
            {
                if( i==0 )
                {
                    if(pdp_pco_ptr[i].is_valid)
                    {
                    
                        sprintf((char*)g_rsp_str,"+CGPCO: 0,\"%s\",\"%s\",%ld",
                                        pdp_pco_ptr[i].user, pdp_pco_ptr[i].passwd, pdp_pco_ptr[i].pdp_id);
                    }
                    else
                    {
                        sprintf((char*)g_rsp_str,"+CGPCO: 0,,,%ld", pdp_pco_ptr[i].pdp_id);
                    }
                }
                else
                {
                    if(pdp_pco_ptr[i].is_valid)
                    {
                        sprintf((char*)g_rsp_str + strlen((char*)g_rsp_str),"\r\n+CGPCO: 0,\"%s\",\"%s\",%ld",
                                        pdp_pco_ptr[i].user, pdp_pco_ptr[i].passwd, pdp_pco_ptr[i].pdp_id);
                    }
                    else
                    {
                        sprintf((char*)g_rsp_str + strlen((char*)g_rsp_str),"\r\n+CGPCO: 0,,,%ld",
                                         pdp_pco_ptr[i].pdp_id);
                    }
                }
            }

            ATC_BuildInfoRsp(atc_config_ptr,g_rsp_str);
            
            //Free pdp_pco_ptr memory
            if(pdp_pco_ptr != PNULL)
            {
                SCI_FREE(pdp_pco_ptr);
            }
#endif/*_GPRS_FULL_FUNCTION_*/
            break;
        }
        
        default:
        {
            return ERR_OPERATION_NOT_SUPPORTED;
        }
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/ 
//    DESCRIPTION: This function handles command AT+CGQREQ, set Qos
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : 
//          
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGQREQ( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                             )
{
#ifdef _SUPPORT_GPRS_
    uint32  i;
    ATC_SET_QOS_REQ_T mn_info;
    ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;
    MN_GPRS_TOTAL_QOS_T qos;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //ATC_TRACE:"ATGPRS:ATC_ProcessCGQREQ\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_642_112_2_17_22_17_49_777,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/
    
    // init the qos
    SCI_MEMSET(&qos, 0, sizeof(qos));
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

        {
            for (i = 0;i < sizeof(mn_info.param_used)/sizeof(mn_info.param_used[0]);i++)
            {
                mn_info.param_used[i] = cur_cmd_info->param_used[i];
            }
            //init the qos to subscribed
            SCI_MEMSET(&qos, 0, sizeof(qos));
            
            //param1--pid
            if (mn_info.param_used[0])
            {
                mn_info.pdp_id = PARA1.num;
                //mngprs_param_used[0] = TRUE;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            
            //param2--precedence
            if (mn_info.param_used[1])
            {
                qos.precedence = (MN_GPRS_PRECEDENCE_CLASS_E)PARA2.num;
            }

            //param3--delay class
            if (mn_info.param_used[2])
            {
                qos.delay_class = (MN_GPRS_DELAY_CLASS_E)PARA3.num;
            }
            
            //param4--reliability
            if (mn_info.param_used[3])
            {
                qos.reliability = (MN_GPRS_RELIABILITY_TYPE_E)PARA4.num;
            }
            
            //param5--peak throughput
            if (mn_info.param_used[4])
            {
                qos.peak_thrput = (MN_GPRS_PEAK_THROUGHPUT_TYPE_E)PARA5.num;
            }
            
            //param6--mean throughput
            if (mn_info.param_used[5])
            {
                qos.mean_thrput = (MN_GPRS_MEAN_THROUGHPUT_TYPE_E)PARA6.num;
            }

            //+CGQREQ= <cid> causes the requested profile for context number <cid> to become undefined.
            if ((FALSE==mn_info.param_used[1] )
                && (FALSE==mn_info.param_used[2])
                && (FALSE==mn_info.param_used[3])
                && (FALSE==mn_info.param_used[4])
                && (FALSE==mn_info.param_used[5]) )
            {
                //segam return error!so we do!
                //ATC_TRACE:"ATGPRS:ATC_ProcessCGQREQ param error!\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_710_112_2_17_22_17_49_778,(uint8*)"" );
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            else
            {        
                qos.is_r99 = FALSE;
                
                ret = MNGPRS_SetTotalQosProfileEx(dual_sys, mn_info.pdp_id, qos);
                
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SET_QOS_CNF,mn_info.pdp_id,ATC_DOMAIN_PS);
                                           
                if (ERR_MNGPRS_NO_ERR != ret)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }

            return S_ATC_DEFAULT_HANDLE;
        }
            
        case ATC_CMD_TYPE_READ:
        {
#ifdef _GPRS_FULL_FUNCTION_
            MN_GPRS_OUT_QOS_T     *cur_qos_arr = PNULL;            
            uint32   pdp_con_num;
                   
            ret = MNGPRS_ReadCurrQosProfileEx(dual_sys, &cur_qos_arr,&pdp_con_num);
            
            if(ret != ERR_MNGPRS_NO_ERR)
            {
                if(cur_qos_arr != PNULL)
                {
                    SCI_FREE(cur_qos_arr);
                }
                
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            
            if(pdp_con_num == 0)
            {
                sprintf((char*)g_rsp_str,"+CGQREQ: ");
            }
            
            for(i=0; i < pdp_con_num; i++)
            {
                if (i==0)
                {
                     sprintf((char*)g_rsp_str,"+CGQREQ: %ld,%d,%d,%d,%d,%d",
                                cur_qos_arr[i].pdp_id,
                                cur_qos_arr[i].qos.reliability,
                                cur_qos_arr[i].qos.peak_thrput,
                                cur_qos_arr[i].qos.delay_class,
                                cur_qos_arr[i].qos.precedence,
                                cur_qos_arr[i].qos.mean_thrput);
                 }
                 else
                 {
                     sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str),"\r\n+CGQREQ: %ld,%d,%d,%d,%d,%d",
                               cur_qos_arr[i].pdp_id,
                               cur_qos_arr[i].qos.reliability,
                               cur_qos_arr[i].qos.peak_thrput,
                               cur_qos_arr[i].qos.delay_class,
                               cur_qos_arr[i].qos.precedence,
                               cur_qos_arr[i].qos.mean_thrput);
                 }
            }
                    
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            
            if(cur_qos_arr != PNULL)
            {
                SCI_FREE(cur_qos_arr);
            }        
#endif/*_GPRS_FULL_FUNCTION_*/
            break;
        }
            
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/  
//    DESCRIPTION: This function handles command AT+CGQMIN, set min Qos
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : Modified by Hyman.wu
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGQMIN( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                             )
{
#ifdef _SUPPORT_GPRS_
    uint32  i;
    ATC_SET_QOS_REQ_T mn_info;
    ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;
    MN_GPRS_TOTAL_QOS_T qos;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //ATC_TRACE:"ATGPRS:ATC_ProcessCGQMIN\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_812_112_2_17_22_17_49_779,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {        
            for (i = 0;i < sizeof(mn_info.param_used)/sizeof(mn_info.param_used[0]);i++)
            {
                mn_info.param_used[i] = cur_cmd_info->param_used[i];
            }
            
            SCI_MEMSET(&qos, 0, sizeof(qos));
            
            //param1--pid
            if (mn_info.param_used[0])
            {
                mn_info.pdp_id = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            
            //param2--precedence
            if (mn_info.param_used[1])
            {
                qos.precedence = (MN_GPRS_PRECEDENCE_CLASS_E)PARA2.num;
            }

            //param3--delay class
            if (mn_info.param_used[2])
            {
                qos.delay_class = (MN_GPRS_DELAY_CLASS_E)PARA3.num;
            }
            
            //param4--reliablity
            if (mn_info.param_used[3])
            {
                qos.reliability = (MN_GPRS_RELIABILITY_TYPE_E)PARA4.num;
            }
            
            //param5--peak throughput
            if (mn_info.param_used[4])
            {
                qos.peak_thrput = (MN_GPRS_PEAK_THROUGHPUT_TYPE_E)PARA5.num;
            }
            
            //param6--mean throughput
            if (mn_info.param_used[5])
            {
                qos.mean_thrput = (MN_GPRS_MEAN_THROUGHPUT_TYPE_E)PARA6.num;
            }

            //+CGQMIN= <cid> causes the minimum acceptable profile for context number <cid> to become undefined.
            if ((mn_info.param_used[1] == FALSE)&&
                (mn_info.param_used[2] == FALSE)&&
                (mn_info.param_used[3] == FALSE)&&
                (mn_info.param_used[4] == FALSE)&&
                (mn_info.param_used[5] == FALSE) )
            {
                //segam return error!so we do!
                //ATC_TRACE:"ATGPRS:ATC_ProcessCGQREQ param error!\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_876_112_2_17_22_17_49_780,(uint8*)"" );
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            else
            {            
                ret = MNGPRS_SetTotalMinQosProfileEx(dual_sys, mn_info.pdp_id, qos);
                
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SET_MIN_QOS_CNF,mn_info.pdp_id,ATC_DOMAIN_PS);
                            
                if (ERR_MNGPRS_NO_ERR != ret)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }

            return S_ATC_DEFAULT_HANDLE;
        }
            
        case ATC_CMD_TYPE_READ:
        {
#ifdef _GPRS_FULL_FUNCTION_
            MN_GPRS_OUT_QOS_T     *min_qos_arr = PNULL;
            uint32   pdp_con_num;

            ret = MNGPRS_ReadMinQosProfileEx(dual_sys, &min_qos_arr,&pdp_con_num);
            if(ERR_MNGPRS_NO_ERR != ret)
            {
                if(min_qos_arr != PNULL)
                {
                    SCI_FREE(min_qos_arr);
                }
                
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            
            if(0 == pdp_con_num)
            {
                sprintf((char*)g_rsp_str,"+CGQMIN: ");
            }
            
            for(i=0; i < pdp_con_num; i++)
            {
                if (i == 0)
                {
                    sprintf((char*)g_rsp_str,"+CGQMIN: %ld,%d,%d,%d,%d,%d",
                                min_qos_arr[i].pdp_id,
                                min_qos_arr[i].qos.reliability,
                                min_qos_arr[i].qos.peak_thrput,
                                min_qos_arr[i].qos.delay_class,
                                min_qos_arr[i].qos.precedence,
                                min_qos_arr[i].qos.mean_thrput);
                }
                else
                {
                    sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str),"\r\n+CGQMIN: %ld,%d,%d,%d,%d,%d",
                               min_qos_arr[i].pdp_id,
                                min_qos_arr[i].qos.reliability,
                                min_qos_arr[i].qos.peak_thrput,
                                min_qos_arr[i].qos.delay_class,
                                min_qos_arr[i].qos.precedence,
                                min_qos_arr[i].qos.mean_thrput);
                }
            }
                    
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            
            if(min_qos_arr != PNULL)
            {
                SCI_FREE(min_qos_arr);
            }
#endif/*_GPRS_FULL_FUNCTION_*/
            break;
        }
            
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/  
//    DESCRIPTION:This function handles command AT+CGATT, attach or dettach GPRS service
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : Modified by Hyman.wu
//    History:
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGATT( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            )
{
#ifdef _SUPPORT_GPRS_
    int32  state;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    //ATC_TRACE:"ATGPRS:ATC_ProcessCGATT\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_973_112_2_17_22_17_50_781,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/
#ifdef _MUX_ENABLE_
    g_atc_ignore_one_detach_cnf = FALSE;
#endif

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            //param1--state
            if (cur_cmd_info->param_used[0])
            {
                state = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            if( 1 == state )
            {
                MNPHONE_GprsAttachEx(dual_sys, MN_PHONE_GPRS_ATTACH);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_GPRS_ATTACH_RESULT,0xff,ATC_NO_DOMAIN);
            }
            else if( 0 == state )
            {
                #ifndef WIN32
                #ifdef _DUAL_SIM
                #ifdef _MUX_ENABLE_
                if((RRA_CheckTrafficState(MN_DUAL_SYS_1) || RRA_CheckTrafficState(MN_DUAL_SYS_2))&& RRA_IsAnswerToPaging())
                {
                    ppp_dialing_in_progress1 = FALSE;
                    SCI_TRACE_LOW("ATC: force detach sim %d",g_current_card_id);
                    MNPHONE_GprsForceDetachEx(dual_sys, MN_PHONE_GPRS_DETACH);
                    g_atc_ignore_one_detach_cnf = TRUE;
                    return S_ATC_SUCCESS;
                }
                #endif
                #endif
                #endif
                MNPHONE_GprsDetachEx(dual_sys, MN_PHONE_GPRS_DETACH);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_GPRS_DETACH_RESULT,0xff,ATC_NO_DOMAIN);
                #ifdef _MUX_ENABLE_
                    ppp_dialing_in_progress = FALSE;
                    g_atc_ignore_one_detach_cnf = TRUE;
                #endif
                return S_ATC_SUCCESS;
            //#ifdef _USE_OLD_L4_API
            //  MNPHONE_GprsDetach(MN_PHONE_GPRS_DETACH);
            //#else
            //  MNPHONE_GprsDetachEx(dual_sys, MN_PHONE_GPRS_DETACH);
            //#endif
            //#ifdef _MUX_ENABLE_
            //    ATC_Add_Expected_Event(atc_config_ptr,APP_MN_GPRS_DETACH_RESULT,0xff,ATC_NO_DOMAIN);
            //#endif
            }
            else
            {
                //Other values will result in an ERROR
                return ERR_INVALID_INDEX;
            }

            return S_ATC_DEFAULT_HANDLE;
        }
            
        case ATC_CMD_TYPE_READ:
        {
            MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;
            
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
            if ((PLMN_NORMAL_GPRS_ONLY == plmn_info.plmn_status)
                || (PLMN_NORMAL_GSM_GPRS_BOTH == plmn_info.plmn_status))
            {
                state = 1;    
            }
            else
            {
                state = 0;
            }
        
            sprintf((char*)g_rsp_str, "+CGATT: %ld", state);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        
            break;
        }
        
        default:
            return ERR_UNKNOWN;
    }    
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/  
//    DESCRIPTION:This function handles command AT+CGACT, activate or deactivate pdp context
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : Modified by Hyman.wu
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGACT( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            )
{
#ifdef _SUPPORT_GPRS_
    int32   state;
    uint32  i;
    ATC_PDP_ACT_REQ_T          mn_info;
    ATC_CUR_PARAM_INFO_LIST_T  *param_ptr;
    ERR_MNGPRS_CODE_E  ret = ERR_MNGPRS_NO_ERR;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //ATC_TRACE:"ATGPRS:ATC_ProcessCGACT\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_1073_112_2_17_22_17_50_782,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    SCI_MEMSET(&mn_info, 0, sizeof (mn_info));

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            //param1--state
            if (cur_cmd_info->param_used[0])
            {
                state = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            
            mn_info.is_all = TRUE;
            param_ptr = cur_cmd_info->cur_param_list_ptr->next;
            
            //no <cid>s are specified,activates all defined contexts.
            for(i = 0; param_ptr != NULL && i < MN_GPRS_MAX_PDP_CONTEXT_COUNT; i++)
            {
                mn_info.pdp_id[i] = 0;
                
                if(cur_cmd_info->param_used[i+1])
                {
                    mn_info.pdp_id[i] = param_ptr->param_info.num;
                }    
                
                param_ptr = param_ptr->next;
            }
        
            if (i > 0)
            {
                mn_info.is_all    = FALSE;
                mn_info.pdp_id[i] = MN_GPRS_PDP_ID_INVALID;
            }

            if(1 == state)
            { 
                ret = MNGPRS_ActivatePdpContextEx(dual_sys, mn_info.is_all,mn_info.pdp_id,POC_E,MN_UNSPECIFIED);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,mn_info.pdp_id[0],ATC_DOMAIN_PS);
                
                if (ERR_MNGPRS_NO_ERR != ret)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else if(0 == state)
            {    
                ret = MNGPRS_DeactivatePdpContextEx(dual_sys, mn_info.is_all,mn_info.pdp_id);
           
                if (ERR_MNGPRS_NO_ERR != ret)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
                            
                if(mn_info.is_all)
                {
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,0xff,ATC_DOMAIN_PS);
                }
                else
                {
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,mn_info.pdp_id[0],ATC_DOMAIN_PS);
                }
            }
            else
            {
                //Other values will result in an ERROR response
                return ERR_INVALID_INDEX;
            }
#ifdef _ATC_GPRS_MODEM_        
            ResetGPRSDataConf();
#endif

            return S_ATC_DEFAULT_HANDLE;
        }
            
        case ATC_CMD_TYPE_READ:
        {
#ifdef _GPRS_FULL_FUNCTION_
            MN_GPRS_CON_STATE_T    *pdp_con_state_arr = PNULL;            
            uint32   pdp_con_num;                
            BOOLEAN  is_active;
                
            ret = MNGPRS_ReadPdpContextStateEx(dual_sys, &pdp_con_state_arr,&pdp_con_num);
            
            if(ret != ERR_MNGPRS_NO_ERR)
            {
                if(pdp_con_state_arr != PNULL)
                {
                    SCI_FREE(pdp_con_state_arr);
                }
                
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            
            if(pdp_con_num == 0)
            {
                sprintf((char*)g_rsp_str,"+CGACT: ");
            }
            
            for(i=0; i < pdp_con_num; i++)
            {
                if(pdp_con_state_arr[i].pdp_state == MN_CONTEXT_DEACTIVATED)
                {
                    is_active = FALSE;
                }
                else
                {
                    is_active = TRUE;
                }
                if(is_active)
                {
                     if (i == 0)
                        {
                            sprintf((char*)g_rsp_str,"+CGACT: %ld,1", pdp_con_state_arr[i].pdp_id);
                        }
                        else
                        {
                            sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str),"\r\n+CGACT: %ld,1",
                                       pdp_con_state_arr[i].pdp_id);
                        }
                }
                else
                {
                     if (i == 0)
                        {
                            sprintf((char*)g_rsp_str,"+CGACT: %ld,0",
                                        pdp_con_state_arr[i].pdp_id);
                        }
                        else
                        {
                            sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str),"\r\n+CGACT: %ld,0",
                                        pdp_con_state_arr[i].pdp_id);
                        }
                }
            }
            
            ATC_BuildInfoRsp(atc_config_ptr,g_rsp_str);
            
            if(pdp_con_state_arr != PNULL)
            {
                SCI_FREE(pdp_con_state_arr);    
            }
#endif/*_GPRS_FULL_FUNCTION_*/
            break;
        }
        
        case ATC_CMD_TYPE_TEST:
             sprintf((char *)g_rsp_str, "+CGACT: 0-1");
             ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
             break;
            
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/*****************************************************************************/
//  Description : This function used to extract parameters for AT+CGCMOD
//  Author: winnie.zhu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_GetCGCMODParams(uint8 *in_ptr, uint8 in_len, ATC_CGCMOD_PARAM_T *param_ptr, uint8 *param_num)
{
    int32            i    = 0;
    int32            j = 0;
    uint16           k = 0;
    BOOLEAN          flag = TRUE;
    if(in_len == 0)
    {
        *param_num = 0;
        return TRUE;
    }
    for (i=0; i<in_len; i++)
    {
        if (in_ptr[i] == ',')
        {
            k++;
            j = 0;
            
            if (11 < k)
            {
                flag = FALSE;
                break;
            }
        }
        else
        {
            if (j < ATC_CID_MAX_LEN)
            {
                if (in_ptr[i] != '"')
                {
                    param_ptr[k].param[j++] = in_ptr[i];
                    param_ptr[k].param_len += 1;
                }
            }
            else
            {
                flag = FALSE;
                break;
            }
        }       
    }
    if(in_ptr[in_len-1] != ',')
    {
        *param_num = k+1;
    }
    else if((in_ptr[in_len-1] == ',') && (in_len > 1))
    {
        *param_num = k;
    }
    else //AT+CGCMOD=,
    {
        *param_num = 0;
    }

    if(flag && (*param_num > 0))
    {
        for(i=0; i<*param_num; i++)
        {
            if((1 == param_ptr[i].param_len) && ((param_ptr[i].param[0])>='0') && ((param_ptr[i].param[0]<= '9')))
            {
                param_ptr[i].param[0] = param_ptr[i].param[0] - '0';
            }
            else if((2 == param_ptr[i].param_len) && ((param_ptr[i].param[0])>='0') && ((param_ptr[i].param[0])<= '9')
                && ((param_ptr[i].param[1]>='0')) && ((param_ptr[i].param[1])<='9'))
            {
                param_ptr[i].param[0] = 10*(param_ptr[i].param[0] - '0') + (param_ptr[i].param[1] - '0');
            }
            else
            {
                flag = FALSE;
                break;
            }
        }
    }

    return flag;
}
/******************************************************************************/  
//    DESCRIPTION:This function handles command AT+CGCMOD, activate or deactivate pdp context
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: winnie.zhu
//    Note : 
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGCMOD( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            )
{
#ifdef _SUPPORT_GPRS_
    uint32  i=0,j=0;
    ATC_PDP_ACT_REQ_T          mn_info;
    //ATC_CUR_PARAM_INFO_LIST_T  *param_ptr;
    ERR_MNGPRS_CODE_E  ret = ERR_MNGPRS_NO_ERR;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    MN_GPRS_ERR_E           result = MN_GPRS_ERR_SUCCESS;

    MN_GPRS_CON_STATE_T    *pdp_con_state_arr = PNULL;            
    uint32   pdp_con_num; 
    ATC_CGCMOD_PARAM_T      params[MN_GPRS_MAX_PDP_CONTEXT_COUNT];
    uint8        param_num = 0;
    uint8                 *temp_ptr;
    uint8                 param_len;
    uint8                 cmd_type;

    SCI_TRACE_LOW("ATGPRS:ATC_ProcessCGCMOD\n");

    SCI_MEMSET(params, 0, (MN_GPRS_MAX_PDP_CONTEXT_COUNT * sizeof(ATC_CGCMOD_PARAM_T)));

	//get the operation type.
	param_len = PARA1.str_ptr->str_len;
	temp_ptr  = PARA1.str_ptr->str_ptr;
	ATC_GetCmdType(temp_ptr, (uint16)param_len, &cmd_type);		/*CR168007 Additional Modification*/
    
   
    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    SCI_MEMSET(&mn_info, 0, sizeof (mn_info));
        
    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        {
            temp_ptr++;
		    param_len--;
            if(FALSE == ATC_GetCGCMODParams(temp_ptr, param_len, params,&param_num))
            {
                SCI_TRACE_LOW("ATC: invalid parameter");
                result = ERR_INVALID_INDEX;
                break;
            }
            SCI_TRACE_LOW("ATC: param_num = %d",param_num);

            if(0 == param_num)
            {
                SCI_TRACE_LOW("ATC: no cid specified");
                mn_info.is_all = TRUE;
            }

            ret = MNGPRS_ReadPdpContextStateEx(dual_sys, &pdp_con_state_arr,&pdp_con_num);
            if(ret != ERR_MNGPRS_NO_ERR)
            {
                result = ERR_OPERATION_NOT_SUPPORTED;
                break;
            }

            if(pdp_con_num == 0) //no cid exist
            {
                result = ERR_OPERATION_NOT_SUPPORTED; 
                break;
            }
            
            for(i = 0; i < param_num; i++)
            {
                mn_info.pdp_id[i] = 0;
                
                if(params[i].param_len != 0)
                {
                    for(j=0; j < pdp_con_num; j++)
                    {
                        if((params[i].param[0] == pdp_con_state_arr[j].pdp_id)&&
                            (pdp_con_state_arr[j].pdp_state == MN_CONTEXT_ACTIVATED))
                        {
                            mn_info.pdp_id[i] = params[i].param[0]; 
                            SCI_TRACE_LOW("ATC: mn_info.pdp_id[%d] = %d",i,mn_info.pdp_id[i]);
                            break;
                        }
                    }
                }
                if(j >= pdp_con_num)
                {
                    break; //no cid activate match
                }
            }   
            if((j >= pdp_con_num) && (i < param_num))
            {
                SCI_TRACE_LOW("ATC: invalid parameter");
                result = ERR_INVALID_INDEX; 
                break;
            }
            if (i > 0)
            {
                mn_info.is_all    = FALSE;
                mn_info.pdp_id[i] = MN_GPRS_PDP_ID_INVALID;
            }

            if(TRUE == mn_info.is_all) //just record activate cids
            {
                for(i=0,j=0; j < pdp_con_num; j++)
                {
                    if(pdp_con_state_arr[j].pdp_state == MN_CONTEXT_ACTIVATED)
                    {
                        mn_info.pdp_id[i++] = pdp_con_state_arr[j].pdp_id; 
                        SCI_TRACE_LOW("ATC: mn_info.pdp_id[%d] = %d",i-1,mn_info.pdp_id[i-1]);

                    }
                }
                if(i ==0) //no activate cid exist
                {
                    SCI_TRACE_LOW("ATC: no activate cid exist");
                    result = ERR_INVALID_INDEX; 
                    break;
                } 
            }

            ret = MNGPRS_ModifyPdpContextEx(dual_sys, mn_info.pdp_id);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_MODIFY_PDP_CONTEXT_CNF,mn_info.pdp_id[0],ATC_DOMAIN_PS);

            //return S_ATC_DEFAULT_HANDLE;
            result = S_ATC_SUCCESS;
            break;
        }
            
        case ATC_CMD_TYPE_TEST:
        {
            BOOLEAN  is_active;
            ret = MNGPRS_ReadPdpContextStateEx(dual_sys, &pdp_con_state_arr,&pdp_con_num);
            if(ret != ERR_MNGPRS_NO_ERR)
            {
                result = ERR_OPERATION_NOT_SUPPORTED;
                break;
            }
                                 
            sprintf((char*)g_rsp_str,"+CGCMOD: ");
            for(i=0; i < pdp_con_num; i++)
            {
                SCI_TRACE_LOW("ATC: pdp_con_state_arr[%d].pdp_state=%d,pid=%d",i,pdp_con_state_arr[i].pdp_state,pdp_con_state_arr[i].pdp_id);
                if(pdp_con_state_arr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                {
                    is_active = TRUE;
                }
                else
                {
                    is_active = FALSE;
                }
                if(is_active)
                {
                     if (i == 0)
                        {
                            sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str),"%ld", pdp_con_state_arr[i].pdp_id);
                        }
                        else
                        {
                            sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str),",%ld",
                                       pdp_con_state_arr[i].pdp_id);
                        }
                }

            }
            
            ATC_BuildInfoRsp(atc_config_ptr,g_rsp_str);
            
            if(pdp_con_state_arr != PNULL)
            {
                SCI_FREE(pdp_con_state_arr);    
            }
            result = S_ATC_SUCCESS;
            break;
        }
                 
        default:
            result = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }
    if(pdp_con_state_arr != PNULL)
    {
        SCI_FREE(pdp_con_state_arr);    
    }
    
    return result;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

#ifdef _SUPPORT_GPRS_
/******************************************************************************/ 
//    DESCRIPTION:
//         check if gived str is supported protocol
//    Global:
//    AUTHOR     : 
//    Note       : None
//    History:
/******************************************************************************/ 
static int Is_Support_L2P(char *l2p_str)
{

    char *Support_L2P[2]={"PPP","ppp"};

    /* should check all supported L2P, but now we only support PPP            */
    /* so just simple compare                                                 */
    if ((!strcmp(l2p_str,Support_L2P[0]))||(!strcmp(l2p_str,Support_L2P[1])))
    {
        return 1;
    }

    return 0;
}
#endif
/******************************************************************************/ 
//    DESCRIPTION:This function handles command AT+CGDATA
//         The execution command causes the MT to perform whatever 
//             actions are necessary to establish communication between 
//             the TE and the network using one or more GPRS PDP types. 
//         This may include performing a GPRS attach and one or more PDP context activations. 
//
//    Global: atc_config_ptr 
//    AUTHOR: Ivan.Yin
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGDATA( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                             )
{
#ifdef _SUPPORT_GPRS_
    uint32 i = 0;
    ATC_ENTER_DATA_STATE_REQ_T mn_info;
    ATC_CUR_PARAM_INFO_LIST_T *param_ptr;
    BOOLEAN is_all;
    char layer2name[MAX_AT_PARA_STR_LEN];
    uint32 pid = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //ATC_TRACE:"ATGPRS:ATC_ProcessCGDATA\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_1285_112_2_17_22_17_50_783,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            param_ptr = cur_cmd_info->cur_param_list_ptr;
            if (cur_cmd_info->param_used[0])
            {
                SCI_MEMCPY(layer2name, PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len);
                layer2name[PARA1.str_ptr->str_len]='\0';
                
                if( PARA1.str_ptr->str_len == 0 )
                {
                    mn_info.l2p = 0;
                }
                
                if( Is_Support_L2P(layer2name) )
                {
                    mn_info.l2p = 1;
                }
                else
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else
            {
                //ATC_TRACE:"ATGPRS:layer 2 Protocol default?"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_1316_112_2_17_22_17_50_784,(uint8*)"");
                return ERR_INVALID_INDEX;
            }

            param_ptr = param_ptr->next;
            for(i = 0; i<MN_GPRS_MAX_PDP_CONTEXT_COUNT && param_ptr!=NULL; i++)
            {
                mn_info.pdp_id[i] = 0;
                if (cur_cmd_info->param_used[i+1])
                {
                    pid = param_ptr->param_info.num;
                    mn_info.pdp_id[i] = pid;
                }
                
                param_ptr = param_ptr->next;
            }
            
            if( i != 0) //for param cid2,cid3...
            {
                is_all = FALSE;
                mn_info.pdp_id[i] = MN_GPRS_PDP_ID_INVALID;
            }
            else //no param cid
            {
                /*MNGPRS_ReadPdpContextStateEx(dual_sys, &ret_state_ptr,&pdp_context_num);
                for (i=0; i<pdp_context_num; i++)
                {
                    if (MN_CONTEXT_DEACTIVATED == ret_state_ptr[i].pdp_state)
                    {
                        pid = ret_state_ptr[i].pdp_id;
                        break;
                    }
                }
                is_all = FALSE;

                if(ret_state_ptr != PNULL)
                {
                    SCI_FREE(ret_state_ptr);
                    ret_state_ptr = PNULL;
                }*/
                pid = 2;
                is_all = FALSE;
            }
            //SCI_TRACE_LOW:"ATC: AT+CGDATA RequestPdpId pdp_id:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_1359_112_2_17_22_17_50_785,(uint8*)"d", pid);
            if(pid == 0 || pid > MN_GPRS_MAX_PDP_CONTEXT_COUNT)
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }
            mn_info.pdp_id[0] = pid;
            mn_info.pdp_id[1] = MN_GPRS_PDP_ID_INVALID;
            /* GPRS attach                                    */
            //MNPHONE_GprsAttach(MN_PHONE_GPRS_ATTACH);
            /* activate pdp context                           */        
            if (ERR_MNGPRS_NO_ERR == MNGPRS_ActivatePdpContextEx(dual_sys, is_all,mn_info.pdp_id,POC_E,MN_UNSPECIFIED))
            {
                atc_config_ptr->pdp_id_arr[pid-1] = TRUE;
        
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,mn_info.pdp_id[0],ATC_DOMAIN_PS);
            }
            else
            {
                return ERR_GPRS_OPERATION_FAILURE;
            }
            
            return S_ATC_DEFAULT_HANDLE;
        }

        default:
            return ERR_UNKNOWN;
    }
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

#ifdef _SUPPORT_GPRS_
/******************************************************************************/ 
//    DESCRIPTION:
//         check if gived str is supported gprs class 
//         CLASS A,B,C,CG,CC
//    Global:
/******************************************************************************/
static int IS_SUPPORT_GPRS_CLASS(char *class_str)
{
    if ( strcmp(class_str,"A") == NULL )
    {
        return MN_PHONE_USER_SERVICE_MAX_NUM;
    }
    if ( strcmp(class_str,"B") == NULL )
    {
        return MN_PHONE_USER_SELECT_GSM_GPRS_BOTH;
    }
    if ( strcmp(class_str,"C") == NULL )
    {
        return MN_PHONE_USER_SERVICE_MAX_NUM;
    }
    if ( strcmp(class_str,"CG") == NULL )
    {
        return MN_PHONE_USER_SERVICE_MAX_NUM;
    }
    if ( strcmp(class_str,"CC") == NULL )
    {
        return MN_PHONE_USER_SELECT_GSM_ONLY;
    }

    return MN_PHONE_USER_SERVICE_MAX_NUM;
}

/*************************************************************************/
/*    Description: fill dummy data  */
/*    Global resource dependence : */
/*    Author: chun.jiang*/
/*    Note:  Modified by Elvis.xu*/
/*************************************************************************/
void ATC_FillDummyData(uint8 *data_ptr, uint32 data_size)
{
    while ((data_size > 0) && (NULL != data_ptr))
    {
        *data_ptr = (uint8)(0x00FF & data_size);
        data_size--;
        data_ptr++;
    }
}

/*************************************************************************/
/*    Description: send one packet dummy data*/
/*    Global resource dependence : */
/*    Author: chun.jiang*/
/*    Note:  Modified by Elvis.xu*/
/*************************************************************************/
ERR_MNGPRS_CODE_E ATC_SendOnePacketDummyData(
    uint32 data_size,
    uint16 nsapi
)
{
    uint8* data_ptr = NULL; 
    ERR_MNGPRS_CODE_E ret_value = ERR_MNGPRS_NO_ERR;

    data_ptr = SCI_ALLOC_BASE(data_size + 1);
    SCI_ASSERT(NULL != data_ptr);/*assert verified check NULL pointer*/
    SCI_MEMSET(data_ptr, 0, (data_size + 1));

    ATC_FillDummyData(data_ptr, data_size);  
    
    ret_value = MNGPRS_SendDataReq(data_ptr, data_size, nsapi);

    if (ERR_MNGPRS_NO_ERR != ret_value)
    {
        SCI_FREE(data_ptr);
    }

    return ret_value;
}


/*************************************************************************/
/*    Description: set  and send dummy  para for AT+SGPRSDATA  */
/*    Global resource dependence : */
/*    Author: chun.jiang*/
/*    Note:  Modified by Elvis.xu*/
/*************************************************************************/
void ATC_SendDummyDataPara(
    uint32 data_size,
    uint16 nsapi,
    uint32 gprs_mtu
)
{
    ERR_MNGPRS_CODE_E ret_val = ERR_MNGPRS_NO_ERR;
    uint32  packet_num = 0;

    SCI_TRACE_LOW("ATC_SendDummyDataPara: nsapi=%d, data_size=%d", 
        nsapi,
        data_size);

    if (0 == data_size)
    {
        return;
    }
    
    s_atc_dummy_data.nsapi     = nsapi;
    s_atc_dummy_data.data_size = data_size; 
    s_atc_dummy_data.gprs_mtu  = gprs_mtu;


    while (!MNGPRS_IsFlowCtrlOpen(nsapi) && s_atc_dummy_data.data_size)
    {
        packet_num++;
        if (s_atc_dummy_data.data_size >= gprs_mtu)
        {
            ret_val = ATC_SendOnePacketDummyData(gprs_mtu, nsapi);
            s_atc_dummy_data.data_size -= gprs_mtu;
        } 
        else
        {
            ret_val = ATC_SendOnePacketDummyData(s_atc_dummy_data.data_size, nsapi);            
            s_atc_dummy_data.data_size = 0;
            s_atc_dummy_data.nsapi = 0;
        }
        
        SCI_TRACE_LOW( "ATC: GPRSDATA packet_No:%d, ret_val:%d, remain_size:%d",
            packet_num,
            ret_val,
            s_atc_dummy_data.data_size 
            );
    }

    return;
}

/*************************************************************************/
/*    Description: send dummy data for AT+SGPRSDATA  */
/*    Global resource dependence : */
/*    Author: chun jiang*/
/*    Note:  Modified by Elvis.xu*/
/*************************************************************************/
void ATC_SendDummyDataReqEx(uint16 nsapi)
{
    ERR_MNGPRS_CODE_E ret_val = ERR_MNGPRS_NO_ERR;
    uint32  packet_num = 0;
    uint32 gprs_mtu = 0;

    if ((nsapi != s_atc_dummy_data.nsapi) ||(nsapi <= 4) ||(nsapi > MN_GPRS_MAX_PDP_CONTEXT_COUNT + 4))
    {
       return;
    }

    gprs_mtu = s_atc_dummy_data.gprs_mtu;

    while (!MNGPRS_IsFlowCtrlOpen(nsapi) && s_atc_dummy_data.data_size)
    {
        packet_num++;
        
        if (s_atc_dummy_data.data_size >= gprs_mtu)
        {
            ret_val = ATC_SendOnePacketDummyData(gprs_mtu, nsapi);
            
            s_atc_dummy_data.data_size -= gprs_mtu;
        }
        else
        {       
            ret_val = ATC_SendOnePacketDummyData(s_atc_dummy_data.data_size, nsapi);
            
            s_atc_dummy_data.data_size = 0;
            s_atc_dummy_data.nsapi = 0;
        }

        SCI_TRACE_LOW( "ATC: GPRSDATA packet_No:%d, ret_val:%d, remain_size:%d",
            packet_num,
            ret_val,
            s_atc_dummy_data.data_size 
            );
    }

    return;
}

/*************************************************************************/
/*    Description: send dummy data for AT+SGPRSDATA  */
/*    Global resource dependence : */
/*    Author: Elvis.xu*/
/*    Note:  */
/*************************************************************************/
void ATC_FreeDummyData(uint16 nsapi)
{
    if ((nsapi != s_atc_dummy_data.nsapi) ||(nsapi <= 4) ||(nsapi > MN_GPRS_MAX_PDP_CONTEXT_COUNT + 4))
    {
       return;
    }

    s_atc_dummy_data.data_size = 0;
    s_atc_dummy_data.nsapi = 0;
}
#endif

/******************************************************************************/ 
//    DESCRIPTION:This function handles command AT+CGSMS,set
//    SMS service type, GPRS SMS is not supported now
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : Modified by Hyman.wu
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGSMS(// return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            )
{
#ifdef _SUPPORT_GPRS_
    ATC_SELECT_SMS_SERVICE_T mn_info;

    //ATC_TRACE:"ATGPRS: ATC_ProcessCGSMS\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_1436_112_2_17_22_17_51_786,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (cur_cmd_info->param_used[0])
            {
                mn_info.service_type = PARA1.num;
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
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/ 
//    DESCRIPTION:This function handles command AT+CGREG
//           The set command controls the presentation of an unsolicited result code 
//                                  +CGREG: <stat> when <n>=1 and 
//           there is a change in the MT's GPRS network registration status, 
//                          or code +CGREG: <stat>[,<lac>,<ci>] 
//           when <n>=2 and there is a change of the network cell.
//    Global: atc_config_ptr 
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGREG(// return the handle result
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
                )
{
#ifdef _SUPPORT_GPRS_
    MN_PHONE_CURRENT_PLMN_INFO_T   register_status;
    OPER_STATUS_E                  oper_status;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != cur_cmd_info); /*assert verified*/

    //ATC_TRACE:"ATGPRS: CREG value:%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_1486_112_2_17_22_17_51_787,(uint8*)"d", atc_config_ptr->cgreg);

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (cur_cmd_info->param_used[0])
            {
                atc_config_ptr->cgreg = (uint8)PARA1.num;
                //ATC_TRACE:"ATGPRS: CREG set value:%d\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_1495_112_2_17_22_17_51_788,(uint8*)"d",atc_config_ptr->cgreg );
                
                // call the MN API functions
                register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
                switch (register_status.plmn_status)
                {
                    case PLMN_NO_SERVICE:
                    {
                        oper_status = NOT_REGISTERED_AND_SEARCHING;
                        break;
                    }
                
                    case PLMN_EMERGENCY_ONLY:
                    {
                        oper_status = REGISTRATION_DENIED;
                        break;
                    }
                
                    case PLMN_NORMAL_GSM_ONLY:
                    case PLMN_NORMAL_GSM_GPRS_BOTH:
                    {
                        if (register_status.plmn_is_roaming)
                        {
                        #ifdef _MUX_REDUCED_MODE_
                            oper_status = REGISTERED_HOME_PLMN;
                        #else
                            oper_status = REGISTERED_ROAMING;
                        #endif
                        }
                        else
                        {
                            oper_status = REGISTERED_HOME_PLMN;
                        }
                        break;
                    }
                
                    default:
                    {
                        oper_status = UNKNOWN;
                        break;
                    }
                }
                // response a "OK"
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                switch (atc_config_ptr->cgreg)
                {
                    case ATC_CREG_DISABLE:
                    {
                        return S_ATC_DEFAULT_HANDLE;
                    }
                
                    case ATC_CREG_ENABLE:
                    {
                        sprintf((char*)g_rsp_str,"%s: %d", g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr,oper_status);
                        break;
                    }
                
                    case ATC_CREG_AND_INFO:
                    {
                        if ((REGISTERED_ROAMING == oper_status)
                            || (REGISTERED_HOME_PLMN == oper_status))
                        {
                            sprintf((char*)g_rsp_str,"%s: %d,0x%04X,0x%04X", g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr,
                                    oper_status, register_status.lac, register_status.cell_id);
                        }
                        else
                        {
                            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr,oper_status);
                        }
                        break;
                    }
                
                    default:
                        return ERR_INVALID_INDEX;
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            // call the MN API functions
            register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
            switch (register_status.plmn_status)
            {
                case PLMN_NO_SERVICE:
                {
                    oper_status = NOT_REGISTERED_AND_SEARCHING;
                    break;
                }

                case PLMN_EMERGENCY_ONLY:
                {
                    oper_status = REGISTRATION_DENIED;
                    break;
                }

                case PLMN_NORMAL_GSM_ONLY:
                {
                    oper_status = NOT_REGISTERED_NOT_SEARCHING;
                    break;
                }

                case PLMN_NORMAL_GPRS_ONLY:
                case PLMN_NORMAL_GSM_GPRS_BOTH:
                {
                    if (register_status.plmn_is_roaming)
                    {
                    #ifdef _MUX_REDUCED_MODE_
                        oper_status = REGISTERED_HOME_PLMN;
                    #else
                        oper_status = REGISTERED_ROAMING;
                    #endif
                    }
                    else
                    {
                        oper_status = REGISTERED_HOME_PLMN;
                    }
                    break;
                }

                default:
                {
                    oper_status = UNKNOWN;
                    break;
                }
            }
            switch (atc_config_ptr->cgreg)
            {
                case ATC_CREG_DISABLE:
                {
                    sprintf((char*)g_rsp_str,"%s: %d,%d", g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr,atc_config_ptr->cgreg, oper_status);
                    break;
                }

                case ATC_CREG_ENABLE:
                {
                    sprintf((char*)g_rsp_str,"%s: %d,%d",g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr,atc_config_ptr->cgreg, oper_status);
                    break;
                }

                case ATC_CREG_AND_INFO:
                {
                    if ((REGISTERED_ROAMING == oper_status)
                        || (REGISTERED_HOME_PLMN == oper_status))
                    {
                        sprintf((char*)g_rsp_str,"%s: %d,%d,0x%04X,0x%04X", 
                                        g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr,atc_config_ptr->cgreg, oper_status, register_status.lac, register_status.cell_id);
                    }
                    else
                    {
                        sprintf((char*)g_rsp_str, "%s: %d,%d",g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr,atc_config_ptr->cgreg, oper_status);
                    }
                    break;
                }
                    
                default:
                    return ERR_INVALID_INDEX;                
            }
            
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            return S_ATC_SUCCESS;
        }

        default:
            break;
    }
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/ 
//    DESCRIPTION: This function handles command AT+CGEREP
//           Set command enables or disables sending of unsolicited result codes
//    Global: atc_config_ptr 
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGEREP( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                             )
{
#ifdef _SUPPORT_GPRS_
#ifdef _GPRS_FULL_FUNCTION_
    ATC_SET_EVENT_REPORT_T mn_info;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //ATC_TRACE:"ATGPRS: ATC_ProcessCGEREP\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_1686_112_2_17_22_17_51_789,(uint8*)"" );
    
    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (cur_cmd_info->param_used[0])
            {
                mn_info.mode = PARA1.num;
            }
            else 
            {
                return ERR_INVALID_INDEX;
            }

            if (cur_cmd_info->param_used[1])
            {
                mn_info.buffer = PARA2.num;
            }
            else
            {
                mn_info.buffer = -1;
            }

            HandleCGEREP(dual_sys, atc_config_ptr, (MN_GPRS_EVENT_REPORT_MODE_E)mn_info.mode,(MN_GPRS_EVENT_REPORT_BUFFER_E)mn_info.buffer);
            break;
        }
    
        case ATC_CMD_TYPE_READ: 
        {
            sprintf( (char*)g_rsp_str, "%s: %d, %d",
                g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr,
                g_atc_mt_event_report_mode[dual_sys].info.mt_event_report_mode,
                g_atc_mt_event_report_mode[dual_sys].info.mt_event_report_buf);

#ifdef _MUX_ENABLE_ 
            if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
            {
                SIO_ATC_WriteCmdRes((uint8*)g_rsp_str, strlen((char*)g_rsp_str));
            }
            else
            {
                MUX_Write(g_rsp_str, strlen((char*)g_rsp_str), atc_config_ptr->current_link_id);
            }
#else
            SIO_ATC_WriteCmdRes((uint8*)g_rsp_str, strlen((char*)g_rsp_str));
#endif
            break;
        }

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }
#endif /*_GPRS_FULL_FUNCTION_*/
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}


/******************************************************************************/ 
//    DESCRIPTION: This function handles command AT+SSST, set service type
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : Modified by Hyman.wu
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessSSST( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                             )
{
#ifdef _SUPPORT_GPRS_
    ERR_MNPHONE_CODE_E ret = ERR_MNPHONE_NO_ERR;
    MN_PHONE_USER_SERVICE_SELECT_E service_type = MN_PHONE_USER_SERVICE_MAX_NUM;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    //ATC_TRACE:"ATGPRS: ATC_ProcessSelectServiceType\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_1764_112_2_17_22_17_51_790,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            switch (PARA1.num)
            {
                case 0:
                {
                    service_type = MN_PHONE_USER_SELECT_NONE;
                    break;
                }
            
                case 1:
                {
                    service_type = MN_PHONE_USER_SELECT_GSM_ONLY;
                    break;
                }
            
                case 3:
                {
                    service_type = MN_PHONE_USER_SELECT_GSM_GPRS_BOTH;
                    break;
                }
            
                default:
                    return ERR_OPERATION_NOT_SUPPORTED;
            }
            ret = MNPHONE_SetServiceTypeEx(dual_sys, service_type);
            if (ERR_MNPHONE_NO_ERR != ret)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            
            break;
        }
    
        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d",g_atc_info_table[AT_CMD_SSST].cmd_name->str_ptr,MNPHONE_GetServiceTypeEx(dual_sys));
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
    
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }
    return S_ATC_SUCCESS;  
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/  
//    DESCRIPTION: Spreadtrum attach or detach
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : Modified by Hyman.wu
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessSATT( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            )
{
#ifdef _SUPPORT_GPRS_
    int32  state = 1;
    MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;
    uint8 ret_state ;
    uint8 action_type;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //ATC_TRACE:"ATGPRS: ATC_ProcessSATT\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_1837_112_2_17_22_17_51_791,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (cur_cmd_info->param_used[0])
            {
                state = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            
            if (cur_cmd_info->param_used[1])
            {
                 action_type = PARA2.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            if( 0 != state )
            {
                if (2 == action_type)
                {
                    return ERR_INVALID_INDEX;
                }
                MNPHONE_GprsAttachEx(dual_sys, (MN_PHONE_GPRS_ATTACH_TYPE_E)action_type);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_GPRS_ATTACH_RESULT,0xff,ATC_NO_DOMAIN);
            }
            else
            {
                MNPHONE_GprsDetachEx(dual_sys, (MN_PHONE_GPRS_DETACH_TYPE_E)action_type);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_GPRS_DETACH_RESULT,0xff,ATC_NO_DOMAIN);
            }

            return S_ATC_DEFAULT_HANDLE;
        }
    
        case ATC_CMD_TYPE_READ:
        {
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
            if ((PLMN_NORMAL_GPRS_ONLY == plmn_info.plmn_status) || (PLMN_NORMAL_GSM_GPRS_BOTH == plmn_info.plmn_status))
            {
                ret_state = 1;
            }
            else
            {
                ret_state = 0;
            }
            
            sprintf((char*)g_rsp_str, "+SATT: %d", ret_state);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
    
        default:
            return ERR_UNKNOWN;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/  
//    DESCRIPTION: Spreadtrum auto attach or not
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : Modified by Hyman.wu
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessSAUTOATT( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            )
{
#ifdef _SUPPORT_GPRS_
    MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;
    uint8 ret_state ;
    BOOLEAN action_type;
    ERR_MNPHONE_CODE_E result;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //ATC_TRACE:"ATGPRS: ATC_ProcessSATT\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_1924_112_2_17_22_17_52_792,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (cur_cmd_info->param_used[0])
            {
                action_type = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }        
                        
            result = MNPHONE_SetGprsAttachModeEx(dual_sys, !action_type);/*lint !e730*/

            if (ERR_MNPHONE_NO_ERR != result)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            
            break;
        }
    
        case ATC_CMD_TYPE_READ:
        {
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
            if ((PLMN_NORMAL_GPRS_ONLY == plmn_info.plmn_status) || (PLMN_NORMAL_GSM_GPRS_BOTH == plmn_info.plmn_status))
            {
                ret_state = 1;
            }
            else
            {
                ret_state = 0;
            }
            
            sprintf((char*)g_rsp_str, "+SAUTOATT: %d", ret_state);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
    
        default:
            return ERR_UNKNOWN;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/  
//    DESCRIPTION: Spreadtrum send gprs for FTA test, AT+SGPRSDATA
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : Modified by Hyman.wu
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessSGPRSDATA( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            )
{
#ifndef _SUPPORT_GPRS_
    return S_ATC_FAIL;
#else
    uint32 data_len = 0;
    uint8 *data_ptr = PNULL;
    int  value = 0;
    uint8 num = 0;
    uint32 i,j;
    uint32 remain_len = 0;
    uint8  nsapi = 5; //default nsapi=5
    MN_GPRS_CON_STATE_T    *pdp_con_state_arr = PNULL;
    uint32   pdp_con_num = 0;


    SCI_ASSERT(NULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                data_len = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            if(PARAM2_FLAG)
            {
                nsapi = PARA2.num + 4; //sndcp nsapi from 5 to 15
            }
            else
            {
                int     active_pdp_count = 0;
    
                /*get the current activate cid*/
                if (ERR_MNGPRS_NO_ERR != MNGPRS_ReadPdpContextStateEx(g_current_card_id, &pdp_con_state_arr,&pdp_con_num) ||
                        PNULL == pdp_con_state_arr)
                {
                    SCI_TRACE_LOW( "ATC:+SGPRSDATA,ReadPdpContextStateEx fail" );
    
                    if (PNULL != pdp_con_state_arr)
                    {
                        SCI_FREE(pdp_con_state_arr);
                    }
    
                    return ERR_OPERATION_NOT_ALLOWED;
                }
    
                for(i = 0; i < pdp_con_num; i++)
                {
                    if (pdp_con_state_arr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                    {
                        active_pdp_count++;
                        nsapi = pdp_con_state_arr[i].pdp_id + 4;    //sndcp nsapi from 5 to 15
                    }
                }
    
                SCI_FREE(pdp_con_state_arr);
    
                /*only one activate cid can be allowed.*/
                if (1 != active_pdp_count)
                {
                    SCI_TRACE_LOW( "ATC:+SGPRSDATA,invalid active pdp count=%d",active_pdp_count);
                    nsapi = 0;
                    return ERR_OPERATION_NOT_ALLOWED;
                }
            }

            if (PARAM3_FLAG)    //data
            {
                if (PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    return ERR_INVALID_INDEX;
                }
    
                if (PARA3.str_ptr->str_len >= GPRS_MTU)   // at cmd is not support too long characters, so limit string length input
                {
                    return ERR_INVALID_INDEX;
                }

                if (PARA3.str_ptr->str_len > data_len)
                {
                    data_len = PARA3.str_ptr->str_len;
                }
                
            }

            SCI_ASSERT((5 <= nsapi) && (nsapi <= 15));/*assert verified check NULL pointer*/

            if (!MNGPRS_IsFlowCtrlOpen(nsapi))
            {
                if (data_len >= MAX_AT_GPRSDATA_SEND_LEN)
                {
                    ATC_SendDummyDataPara(data_len, nsapi, GPRS_MTU);
                    return S_ATC_SUCCESS;
                } 
    
                num = data_len/GPRS_MTU;
                remain_len = data_len - num*GPRS_MTU;
            
                if (num > 0)
                {
                    for(i=0; i<num; i++)
                    {
                        data_ptr = SCI_ALLOC_BASE(GPRS_MTU+1);
        
                        SCI_ASSERT(NULL != data_ptr);/*assert verified check NULL pointer*/
                        SCI_MEMSET(data_ptr, 0, (GPRS_MTU+1));
        
                        if (PARAM3_FLAG)
                        {
                            if (PARA3.str_ptr->str_len > 0)
                            {
                                SCI_MEMCPY(data_ptr,PARA3.str_ptr->str_ptr,PARA3.str_ptr->str_len);
                            }
                        }
                        else
                        {
                            for (j = 0; j < GPRS_MTU; j++)
                            {
                                data_ptr[j] = (uint8)j;
                            }
                        }
    
                        value = MNGPRS_SendDataReq(data_ptr, GPRS_MTU, nsapi);
    
                        if (ERR_MNGPRS_NO_ERR != value)
                        {
                            SCI_TRACE_LOW( "ATC:+SGPRSDATA,MNGPRS_SendDataReq fail" );
                            SCI_FREE(data_ptr);
                        }
                    }
    
                }
    
                if (remain_len > 0)
                {
                    data_ptr = SCI_ALLOC_BASE(remain_len+1);
    
                    SCI_ASSERT(NULL != data_ptr);/*assert verified check NULL pointer*/
                    SCI_MEMSET(data_ptr, 0, (remain_len+1));
    
                    if (PARAM3_FLAG)
                    {
                        /*use the data input*/
                        if (PARA3.str_ptr->str_len > 0)
                        {
                            SCI_MEMCPY(data_ptr,PARA3.str_ptr->str_ptr,
                                (PARA3.str_ptr->str_len >= remain_len) ? remain_len : PARA3.str_ptr->str_len);
                        }
                    }
                    else
                    {
                        for (i = 0; i < remain_len; i++)
                        {
                            data_ptr[i] = (uint8)i;
                        }
                    }
    
                    value = MNGPRS_SendDataReq(data_ptr, remain_len, nsapi);
                    if (ERR_MNGPRS_NO_ERR != value)
                    {
                        SCI_TRACE_LOW( "ATC:+SGPRSDATA,MNGPRS_SendDataReq fail" );
                        SCI_FREE(data_ptr);
                    }
                }
            }
            else
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }
            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "support +SGPRSDATA (MAX Send GPRSDATA Len is %d!)", MAX_AT_GPRSDATA_SEND_LEN);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        default:
            return ERR_UNKNOWN;
    }

    return S_ATC_SUCCESS;
#endif /* _SUPPORT_GPRS_ */
}

/******************************************************************************/  
//    DESCRIPTION: This function handles command AT+CGEQREQ, set 3G Qos
//    Global resource dependence : atc_config_ptr 
//    AUTHOR:  king.li
//    Note : 
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGEQREQ( // return the handle result
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
                )
{
#ifdef _SUPPORT_GPRS_
    ATC_STATUS  at_status = S_ATC_SUCCESS;

    //ATC_TRACE:"ATGPRS:ATC_ProcessCGEQREQ\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_2152_112_2_17_22_17_52_801,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            at_status = SetQos_R99(atc_config_ptr, cur_cmd_info, FALSE);
            break;
        }
            
        case ATC_CMD_TYPE_READ:
        {
#ifdef _GPRS_FULL_FUNCTION_
            at_status = ReadQos_R99(atc_config_ptr, FALSE);
#endif
            break;
        }
            
        default:
        {
            at_status = ERR_OPERATION_NOT_SUPPORTED;
            break;
        }
    }
    return (at_status);
#else
    return S_ATC_SUCCESS;
#endif /* _SUPPORT_GPRS_ */    
}

#ifdef _SUPPORT_GPRS_
/******************************************************************************/  
//    DESCRIPTION: Set QOS for R99 element
//    Global resource dependence :  
//    AUTHOR: Tracy Zhang
//    Note : 
/******************************************************************************/ 
LOCAL ATC_STATUS SetQos_R99(
                            ATC_CONFIG_T        *atc_config_ptr, 
                            ATC_CUR_INFO_LIST_T *cur_cmd_info,   //IN:
                            BOOLEAN             is_minqos        //IN:
                            )
{
    MNGPRS_3G_EXTEND_QOS_SET_T  mn_info         = {0};
    BOOLEAN                     item_setted[13] = {0};    
    ERR_MNGPRS_CODE_E           ret             = ERR_MNGPRS_NO_ERR;
    ATC_STATUS                  at_result       = S_ATC_DEFAULT_HANDLE;
    MN_GPRS_TOTAL_QOS_T         new_qos;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET((void *)&new_qos, 0, sizeof(MN_GPRS_TOTAL_QOS_T));
    
    //param1--pid        
    if (PARAM1_FLAG)
    {
        item_setted[0] = TRUE;
        mn_info.pdp_id = PARA1.num;
    }
    else
    {
        return ERR_INVALID_INDEX;
    }
    //param2--traffic class
    if (PARAM2_FLAG)    
    {
        item_setted[1] = TRUE;
        mn_info.traffic_class = (MN_GPRS_TRAFFIC_CLASS_E)(PARA2.num);
    }
    //param3--Maximum bitrate UL
    if (PARAM3_FLAG)
    {
        item_setted[2] = TRUE;
        mn_info.max_bt_ul = (uint16)PARA3.num;
    }
    //param4--Maximum bitrate DL
    if (PARAM4_FLAG)
    {
        item_setted[3] = TRUE;
        mn_info.max_bt_dl = (uint16)PARA4.num;
    }
    //param5--Guaranteed bitrate UL
    if (PARAM5_FLAG)
    {
        item_setted[4] = TRUE;
        mn_info.guaranteed_bt_ul = (uint16)PARA5.num;        
    }
    //param6--Guaranteed bitrate DL
    if (PARAM6_FLAG)
    {
        item_setted[5] = TRUE;
        mn_info.guaranteed_bt_dl = (uint16)PARA6.num;    
    }
    //param7--Delivery order
    if (PARAM7_FLAG)
    {
        item_setted[6] = TRUE;
        mn_info.delivery_order = (uint16)PARA7.num;
    }
    //param8--Maximum SDU size
    if (PARAM8_FLAG)
    {
        item_setted[7] = TRUE;
        mn_info.max_sdu_size = (uint32)PARA8.num;
    }
    //param9--SDU error ratio
    if (PARAM9_FLAG)
    {
        if (PARAM9_TYPE != ATC_CMD_PARAM_TYPE_STRING)
        {
            return ERR_INVALID_INDEX;
        }
        item_setted[8] = TRUE;
        if (!ConvertmEeToBin(PARA9.str_ptr->str_ptr,PARA9.str_ptr->str_len, mn_info.sdu_error_ratio))
        {
            return ERR_INVALID_INDEX;
        }
    }
    
    //param10--Residual bit error ratio
    if (PARAM10_FLAG)
    {
        if (PARAM10_TYPE != ATC_CMD_PARAM_TYPE_STRING)
        {
            return ERR_INVALID_INDEX;
        }
        item_setted[9] = TRUE;
        if (!ConvertmEeToBin(PARA10.str_ptr->str_ptr,PARA10.str_ptr->str_len,mn_info.residual_ber))
        {
            return ERR_INVALID_INDEX;
        }
    }
    
    //param11--Delivery of erroneous SDUs
    if (PARAM11_FLAG)
    {
        item_setted[10] = TRUE;
        mn_info.delivery_of_err_sdu = (uint8)PARA11.num;
    }
    //param12--Transfer delay
    if (PARAM12_FLAG)
    {
        item_setted[11] = TRUE;
        mn_info.transfer_delay =  (uint16)PARA12.num;
    }
    //param13--Traffic handling priority
    if (PARAM13_FLAG)
    {
        item_setted[12] = TRUE;
        mn_info.traffic_handling_priority = (uint8)PARA13.num;
    }
    //param14 and param15 not support yet
    //param14--Source Statistics Descriptor
          //param15--Signalling Indication
    //+CGQREQ= <cid> causes the requested profile for context number <cid> to become undefined.
    if ((PARAM2_FLAG == FALSE)
        && (FALSE == PARAM3_FLAG)
        && (FALSE == PARAM4_FLAG)
        && (FALSE == PARAM5_FLAG)
        && (FALSE == PARAM6_FLAG)
        && (FALSE == PARAM7_FLAG)
        && (FALSE == PARAM8_FLAG)
        && (FALSE == PARAM9_FLAG)
        && (FALSE == PARAM10_FLAG)
        && (FALSE == PARAM11_FLAG)
        && (FALSE == PARAM12_FLAG)
        && (FALSE == PARAM13_FLAG))
    {
        //segam return error!so we do!
        //ATC_TRACE:"ATGPRS:ATC_ProcessCGEQREQ param error!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_2321_112_2_17_22_17_52_802,(uint8*)"" );
        at_result = ERR_OPERATION_NOT_SUPPORTED;
    }
    else
    {
        ConvertAtQosToL4Qos(&mn_info);

        //check the input QOS parameter is correct or not                           
        if (TRUE == item_setted[0])
        {
            if (mn_info.pdp_id == 0 || mn_info.pdp_id > MN_GPRS_MAX_PDP_CONTEXT_COUNT)
            {   
                return ERR_MNGPRS_INVALID_PDP_ID;
            }
        }
       
        if (TRUE == item_setted[6])
        {
            if (mn_info.delivery_order >= MN_GPRS_DELIVERY_ORDER_RESERVED)
            {
                return ERR_MNGPRS_NO_VALID_PARAM;
            }
         }
      
        if (TRUE == item_setted[10])
        {
            if (mn_info.delivery_of_err_sdu > MN_GPRS_DELIVERY_OF_ERR_SDU_NO)
            {
                return ERR_MNGPRS_NO_VALID_PARAM;
            }
        }
        
        if (TRUE == item_setted[12])
        {
            if (mn_info.traffic_handling_priority > MN_GPRS_TRAFFIC_HNDL_PRIORITY3)
            {
                return ERR_MNGPRS_NO_VALID_PARAM;
            }
        }

        if (TRUE == item_setted[1])
        {
            if (mn_info.traffic_class > MN_GPRS_TRAFFIC_CLASS_BACKGROUD)
            {
                return ERR_MNGPRS_NO_VALID_PARAM;
            }
        }

        if (TRUE == item_setted[8])
        {
            if (mn_info.sdu_error_ratio[0] > MN_GPRS_SDU_ERR_RATIO_100k &&
                 mn_info.sdu_error_ratio[0] != MN_GPRS_SDU_ERR_RATIO_RESERVED)
            {
                return ERR_MNGPRS_NO_VALID_PARAM;
            }        
        }
        
        if (TRUE == item_setted[9])
        {
            if (mn_info.residual_ber[0] > MN_GPRS_RESIDUAL_BER_6)
            {
                return ERR_MNGPRS_NO_VALID_PARAM;
            }
        }
        
        //the following transfers the QOS struct  into new qos struct ...start
        new_qos.is_r99              = TRUE;
        new_qos.delivery_of_err_sdu = (MN_GPRS_DELIVERY_OF_ERR_SDU_E)mn_info.delivery_of_err_sdu;
        new_qos.delivery_order      = (MN_GPRS_DELIVERY_ORDER_E)mn_info.delivery_order;
        new_qos.traffic_class       = mn_info.traffic_class;
        //...max_sdu_size
        if (mn_info.max_sdu_size <= 1500)
        {
            new_qos.max_sdu_size = (MN_GPRS_MAX_SDU_SIZE_E)(mn_info.max_sdu_size/10);
        }
        else if (mn_info.max_sdu_size <= 1502)
        {
            new_qos.max_sdu_size = MN_GPRS_MAX_SDU_SIZE_1502OCTETS;
        }
        else if (mn_info.max_sdu_size <= 1510)
        {
            new_qos.max_sdu_size = (MN_GPRS_MAX_SDU_SIZE_E)(MN_GPRS_MAX_SDU_SIZE_1500OCTETS+2);
        }
        else if (mn_info.max_sdu_size <= 1520)
        {
            new_qos.max_sdu_size = (MN_GPRS_MAX_SDU_SIZE_E)(MN_GPRS_MAX_SDU_SIZE_1500OCTETS+3);
        }
        else 
        {
            new_qos.max_sdu_size = SM_GPRS_MAX_SDU_SIZE_RESERVED;
            return ERR_MNGPRS_NO_VALID_PARAM;
        }
        //...max_bt_ul
        if (mn_info.max_bt_ul <= 63)
        {
            new_qos.max_bit_rate_for_uplink = (MN_GPRS_MAX_BIT_RATE_UPLINK_E)mn_info.max_bt_ul;
        }
        else if (mn_info.max_bt_ul <= 575)
        {
            new_qos.max_bit_rate_for_uplink = (MN_GPRS_MAX_BIT_RATE_UPLINK_E)(MN_GPRS_MAX_BIT_RATE_UPLINK3+(mn_info.max_bt_ul-MN_GPRS_MAX_BIT_RATE_UPLINK3)/8);
        }
        else if (mn_info.max_bt_ul <= 8640)
        {
            new_qos.max_bit_rate_for_uplink = (MN_GPRS_MAX_BIT_RATE_UPLINK_E)(MN_GPRS_MAX_BIT_RATE_UPLINK5+(mn_info.max_bt_ul-576)/64);
        }
        else
        {
            new_qos.max_bit_rate_for_uplink = MN_GPRS_NW2MS_MAX_BIT_RATE_UPLINK_RESERVED;
        }
        //...max_bt_dl
        if (mn_info.max_bt_dl <= 63)
        {
            new_qos.max_bit_rate_for_downlink = (MN_GPRS_MAX_BIT_RATE_DOWNLINK_E)mn_info.max_bt_dl;
        }
        else if (mn_info.max_bt_dl <= 575)
        {
            new_qos.max_bit_rate_for_downlink = (MN_GPRS_MAX_BIT_RATE_DOWNLINK_E)(MN_GPRS_MAX_BIT_RATE_DOWNLINK3+(mn_info.max_bt_dl-MN_GPRS_MAX_BIT_RATE_DOWNLINK3)/8);
        }
        else if (mn_info.max_bt_dl <= 8640)
        {
            new_qos.max_bit_rate_for_downlink = (MN_GPRS_MAX_BIT_RATE_DOWNLINK_E)(MN_GPRS_MAX_BIT_RATE_DOWNLINK5+(mn_info.max_bt_dl-576)/64);
        }
        else
        {
            new_qos.max_bit_rate_for_downlink = MN_GPRS_NW2MS_MAX_BIT_RATE_DOWNLINK_RESERVED;
        }
        //...residual_ber
        new_qos.residual_ber = (MN_GPRS_RESIDUAL_BER_E)mn_info.residual_ber[0];
        //...sdu_err_ratio
        new_qos.sdu_err_ratio = (MN_GPRS_SDU_ERR_RATIO_E)mn_info.sdu_error_ratio[0];
        //...traffic_handling_prio
        new_qos.traffic_handling_prio = (MN_GPRS_TRAFFIC_HNDL_PRIORITY_E)mn_info.traffic_handling_priority;
        //...transfer_delay
        if (mn_info.transfer_delay <= 150)
        {
            new_qos.transfer_delay = (MN_GPRS_TRANSFER_DELAY_E)(mn_info.transfer_delay/10);
        }
        else if ((199 < mn_info.transfer_delay)&&(mn_info.transfer_delay <= 950))
        {
            new_qos.transfer_delay = (MN_GPRS_TRANSFER_DELAY_E)((mn_info.transfer_delay-200)/50+16);
        }
        else if ((999 < mn_info.transfer_delay)&&(mn_info.transfer_delay <=4100))
        {
            new_qos.transfer_delay = (MN_GPRS_TRANSFER_DELAY_E)((mn_info.transfer_delay-1000)/100+32);
        }
        else
        {
            new_qos.transfer_delay = MN_GPRS_TRANSFER_DELAY_RESERVED;
            return ERR_MNGPRS_NO_VALID_PARAM;
        }
        //...guaranteed_bit_rate_for_uplink
        if (mn_info.guaranteed_bt_ul <= 63)
        {
            new_qos.guaranteed_bit_rate_for_uplink = (MN_GPRS_GUARANTD_BIT_RATE_UPLINK_E)mn_info.guaranteed_bt_ul;
        }
        else if (mn_info.guaranteed_bt_ul <= 575)
        {
            new_qos.guaranteed_bit_rate_for_uplink = (MN_GPRS_GUARANTD_BIT_RATE_UPLINK_E)(MN_GPRS_GUARANTD_BIT_RATE_UPLINK3+(mn_info.guaranteed_bt_ul-MN_GPRS_GUARANTD_BIT_RATE_UPLINK3)/8);
        }
        else if (mn_info.guaranteed_bt_ul <= 8640)
        {
            new_qos.guaranteed_bit_rate_for_uplink = (MN_GPRS_GUARANTD_BIT_RATE_UPLINK_E)(MN_GPRS_GUARANTD_BIT_RATE_UPLINK5+(mn_info.guaranteed_bt_ul-576)/64);
        }
        else
        {
            new_qos.guaranteed_bit_rate_for_uplink = MN_GPRS_NW2MS_GUARANTD_BIT_RATE_UPLINK_RESERVED;
        }
        //...guaranteed_bit_rate_for_downlink
        if (mn_info.guaranteed_bt_dl <= 63)
        {
            new_qos.guaranteed_bit_rate_for_downlink = (MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK_E)mn_info.guaranteed_bt_dl;
        }
        else if (mn_info.guaranteed_bt_dl <= 575)
        {
            new_qos.guaranteed_bit_rate_for_downlink = (MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK_E)(MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK3+(mn_info.guaranteed_bt_dl-MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK3)/8);
        }
        else if (mn_info.guaranteed_bt_dl <= 8640)
        {
            new_qos.guaranteed_bit_rate_for_downlink = (MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK_E)(MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK5+(mn_info.guaranteed_bt_dl-576)/64);
        }
        else
        {
            new_qos.guaranteed_bit_rate_for_downlink = MN_GPRS_NW2MS_GUARANTD_BIT_RATE_DOWNLINK_RESERVED;
        }
        //the above transfers the QOS struct  into new qos struct ...end
        
        //para DOWNLOAD_E is invalid just adapt to interface
        if (is_minqos)
        {
            ret= MNGPRS_SetExtendMinQosEx(dual_sys, mn_info.pdp_id, new_qos);
            if (ret==ERR_MNGPRS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SET_MIN_QOS_CNF,mn_info.pdp_id,ATC_DOMAIN_PS);
            }
        }
        else
        {
            ret= MNGPRS_SetExtendQosEx(dual_sys, mn_info.pdp_id, new_qos, DOWNLOAD_E);
            if (ret==ERR_MNGPRS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SET_QOS_CNF,mn_info.pdp_id,ATC_DOMAIN_PS); 
            }
        }
        if (ERR_MNGPRS_NO_ERR == ret)
        {
            at_result = S_ATC_DEFAULT_HANDLE;
        }
        else
        {
            at_result = ERR_OPERATION_NOT_SUPPORTED;
        }                
    }
    return (at_result);
}

#ifdef _GPRS_FULL_FUNCTION_
/******************************************************************************/  
//    DESCRIPTION: read QOS for R99 element
//    Global resource dependence :  
//    AUTHOR: Tracy Zhang
//    Note : 
/******************************************************************************/ 
LOCAL ATC_STATUS ReadQos_R99(
                             ATC_CONFIG_T   *atc_config_ptr, //OUT:
                             BOOLEAN        is_minqos        //IN:
                             )
{
    ATC_STATUS                  at_status    = S_ATC_SUCCESS;
    ERR_MNGPRS_CODE_E           mn_result    = ERR_MNGPRS_NO_ERR;
    MNGPRS_3G_EXTEND_QOS_SET_T  *cur_qos_arr = PNULL;
    uint32                      pdp_con_num  = 0;
    uint32                      i            = 0;
    int32                       str_len      = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if (is_minqos)
    {
        mn_result = MNGPRS_ReadExtendMinQosEx(dual_sys, &cur_qos_arr, &pdp_con_num);
        str_len = sprintf((char*)g_rsp_str, "+CGEQMIN: ");
    }
    else
    {
        mn_result = MNGPRS_ReadExtendQosEx(dual_sys, &cur_qos_arr, &pdp_con_num);
        str_len = sprintf((char *)g_rsp_str, "+CGEQREQ: ");
    }
    
    if (ERR_MNGPRS_NO_ERR == mn_result)
    {
        for (i=0; i<pdp_con_num; i++)
        {
            ConvertL4QosToAtQos(&cur_qos_arr[i]);
            str_len += sprintf((char *)g_rsp_str + str_len, "%d,%d,%d,%d,%d,%d,%d,%ld,\"%dE%d\",\"%dE%d\",%d,%d,%d",
                                cur_qos_arr[i].pdp_id,
                                cur_qos_arr[i].traffic_class,
                                cur_qos_arr[i].max_bt_ul,
                                cur_qos_arr[i].max_bt_dl,                          
                                cur_qos_arr[i].guaranteed_bt_ul,
                                cur_qos_arr[i].guaranteed_bt_dl,
                                cur_qos_arr[i].delivery_order,
                                cur_qos_arr[i].max_sdu_size,
                                cur_qos_arr[i].sdu_error_ratio[0],
                                cur_qos_arr[i].sdu_error_ratio[1],
                                cur_qos_arr[i].residual_ber[0],
                                cur_qos_arr[i].residual_ber[1],
                                cur_qos_arr[i].delivery_of_err_sdu,
                                cur_qos_arr[i].transfer_delay,
                                cur_qos_arr[i].traffic_handling_priority);
            if (i < pdp_con_num-1)
            {
                if (is_minqos)
                {
                    str_len += sprintf((char *)g_rsp_str + str_len, "\r\n+CGEQMIN: ");
                }
                else
                {
                    str_len += sprintf((char *)g_rsp_str + str_len, "\r\n+CGEQREQ: ");
                }
            }
        }
                
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        at_status    = S_ATC_SUCCESS;
    }
    else
    {
        at_status = ERR_OPERATION_NOT_SUPPORTED;
    }
    
    if (cur_qos_arr != PNULL)
    {
        SCI_FREE(cur_qos_arr);
    }        
    return (at_status);
}    
#endif
#endif

/******************************************************************************/  
//    DESCRIPTION: This function handles command AT+CGEQMIN, set 3G min Qos
//    Global resource dependence : atc_config_ptr 
//    AUTHOR:  King.Li
//    Note : 
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGEQMIN( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            )
{
#ifdef _SUPPORT_GPRS_
    ATC_STATUS  at_status = S_ATC_SUCCESS;

    //ATC_TRACE:"ATGPRS:ATC_ProcessCGEQMIN\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_2628_112_2_17_22_17_53_803,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            at_status = SetQos_R99(atc_config_ptr,cur_cmd_info, TRUE);
            break;
        }
            
        case ATC_CMD_TYPE_READ:
        {
#ifdef _GPRS_FULL_FUNCTION_
            at_status = ReadQos_R99(atc_config_ptr, TRUE);
#endif
            break;
        }
            
        default:
        {
            at_status = ERR_OPERATION_NOT_SUPPORTED;
            break;
        }
    }
    return (at_status);
    
#else
    return S_ATC_SUCCESS;
#endif
}

#ifdef _SUPPORT_GPRS_    
/******************************************************************************/  
//    DESCRIPTION: This function handle signal APP_MN_DEACTIVATE_PDP_CONTEXT_IND
//    Global resource dependence : atc_config_ptr 
//    AUTHOR:
//    Note : 
/******************************************************************************/ 
ATC_STATUS ATC_ProcessDeactPdpContextInd(
                        ATC_CONFIG_T        *atc_config_ptr,
                        APP_MN_GPRS_T       *sig_ptr,
                        ATC_GLOBAL_INFO_T   *atc_global_info_ptr
                        )
{
    uint32 i = 0;
    uint32 pid = 0;
    BOOLEAN is_pdp_belong_to_modem = FALSE;

    ATC_SET_CURRENT_LINK_ID(atc_config_ptr, ATC_GetDualSimMuxLinkId(((APP_MN_GPRS_DEACTIVATE_IND_T*)sig_ptr)->dual_sys));
    //ATC_Clear_All_Exp_Result(((APP_MN_GPRS_DEACTIVATE_IND_T *)sig_ptr)->pdp_id[0],ATC_DOMAIN_PS);
        
    for( i = 0; i < ((APP_MN_GPRS_DEACTIVATE_IND_T*)sig_ptr)->pdp_num; i++)
    {
        pid = ((APP_MN_GPRS_DEACTIVATE_IND_T*)sig_ptr)->pdp_id[i];
        ATC_FreeDummyData(pid+4);
        if(atc_global_info_ptr->atc_config_ptr->pdp_id_arr[pid-1])
        {
            atc_global_info_ptr->atc_config_ptr->pdp_id_arr[pid-1] = FALSE;
            //SCI_TRACE_LOW:"ATC_ProcessDeactPdpContextInd,is deactivate AT pid,pid:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_2686_112_2_17_22_17_53_804,(uint8*)"d", pid);
            is_pdp_belong_to_modem = TRUE;
            
        #ifdef _MUX_ENABLE_
            MPPP_ResetPPPInterface(pid);
        #else
#ifdef TCPIP_SUPPORT
            TCPIP_ResetPPPInterface();
#endif
            break;
        #endif
        }
    }
    
    if(is_pdp_belong_to_modem)
    {
    #ifdef _MUX_ENABLE_
        ppp_dialing_in_progress = FALSE;
    #endif
        
        ATC_SendDetachReq(((APP_MN_GPRS_DEACTIVATE_IND_T*)sig_ptr)->dual_sys);
        
    #ifdef _ATC_GPRS_MODEM_        
        ResetGPRSDataConf();
    #endif
    }

    if(g_is_pdp_activated)
    {
        ATC_ProcessPDPDeactivatedByNetwork(atc_config_ptr);
    }

    return S_ATC_DEFAULT_HANDLE;
}

/******************************************************************************/  
//    DESCRIPTION: This function handle signal APP_MN_ACTIVATE_PDP_CONTEXT_CNF
//    Global resource dependence : atc_config_ptr 
//    AUTHOR:
//    Note : 
/******************************************************************************/ 
ATC_STATUS ATC_ProcessActPdpContextCnf(
                        ATC_CONFIG_T        *atc_config_ptr,
                        APP_MN_GPRS_T       *sig_ptr,
                        ATC_GLOBAL_INFO_T   *atc_global_info_ptr
                        )
{
#ifdef PCLINK_ENABLE
    MN_GPRS_PCLINK_CFG_T pclink_cfg;
#endif

    uint8 pid = 0;

    CHECK_CURRENT_CARD_ID_RTN(APP_MN_GPRS_EXT_T);


    //ATC_Receive_Expected_Event(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,((APP_MN_GPRS_EXT_T *)sig_ptr)->pdp_id,ATC_DOMAIN_PS);
    ATC_SET_CURRENT_LINK_ID(atc_config_ptr, ATC_GetDualSimMuxLinkId(((APP_MN_GPRS_EXT_T*)sig_ptr)->dual_sys));


#ifdef PCLINK_ENABLE
    /* in PCLink mode, directly switch to DATA mode */
    MNGPRS_GetPclinkCfg(&pclink_cfg);
    if( pclink_cfg.is_pclink_open )
    {
        if(((APP_MN_GPRS_EXT_T*)sig_ptr)->result == MN_GPRS_ERR_SUCCESS)

        {
            SIO_ATC_SetDataMode(TRUE);
            //SCI_TRACE_LOW:"ATC: Switch to DATA MODE for PCLink!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_2751_112_2_17_22_17_53_805,(uint8*)"");
        }
        else
        {
            //SCI_TRACE_LOW:"ATC: Activate result is NOT Success in PCLink Mode!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_2755_112_2_17_22_17_53_806,(uint8*)"");
        }
    }
    else
#endif /* PCLINK_ENABLE */
    {
        pid = ((APP_MN_GPRS_EXT_T*)sig_ptr)->pdp_id;
        if (TRUE == atc_global_info_ptr->atc_config_ptr->pdp_id_arr[pid-1])
        {
            if( ((APP_MN_GPRS_EXT_T*)sig_ptr)->result == MN_GPRS_ERR_SUCCESS)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_CONNECT,0);
                
                //SCI_TRACE_LOW:"ATC:APP_MN_ACTIVATE_PDP_CONTEXT_CNF pid:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_2768_112_2_17_22_17_53_807,(uint8*)"d",pid);
            #ifndef WIN32
                SIO_ATC_ClrCmdLine();
            #endif
                SIO_ATC_SetCmdLineTerminateChar(atc_global_info_ptr->atc_config_ptr->s3_reg, '\0');
                
            #ifdef _MUX_ENABLE_ 
                if(g_mux_sys_info[MUX_1].is_start==MUX_STATUS_OFF)
                    MPPP_SetPPPInterface(pid+4,0,pid);
                else
                    MPPP_SetPPPInterface(pid+4,ATC_GetLinkIdFromCid(pid),pid);
            #else
#ifdef TCPIP_SUPPORT
                TCPIP_SetPPPInterface(((APP_MN_GPRS_EXT_T *)sig_ptr)->nsapi);
#endif
            #endif
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,ERR_GPRS_OPERATION_FAILURE);
            }
        #ifdef _MUX_ENABLE_        
            ppp_dialing_in_progress = FALSE;
     #endif
        }
       /*else if(!g_is_pdp_activated )
       {
           IPOPEN_ProcessActPdpContextCnf(atc_config_ptr,sig_ptr);
       }*/
        else
        {
            //SCI_TRACE_LOW:"ATC_ProcessActPdpContextCnf,pid is not AT activated,pid:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_2797_112_2_17_22_17_53_808,(uint8*)"d", pid);
            if( ((APP_MN_GPRS_EXT_T*)sig_ptr)->result == MN_GPRS_ERR_SUCCESS) 
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,ERR_GPRS_OPERATION_FAILURE);
            }
        }
    }
    return S_ATC_DEFAULT_HANDLE;
}

#ifdef _GPRS_FULL_FUNCTION_
/*****************************************************************************/
//  Function    : HandleCGEREP
//  Description : 
//                set event report mode and buffer control
//  Return      : 
//  Note        :
/*****************************************************************************/
LOCAL ERR_MNGPRS_CODE_E HandleCGEREP(
                MN_DUAL_SYS_E                dual_sys,        //IN:
                ATC_CONFIG_T                 *atc_config_ptr, //IN
                MN_GPRS_EVENT_REPORT_MODE_E  mode,            //IN:
                MN_GPRS_EVENT_REPORT_BUFFER_E buffer          //IN:
                )
{
    ATC_GPRS_EV_REPORT_DETAIL_T *ev_report_ptr = &(g_atc_mt_event_report_mode[dual_sys]);
    
    ev_report_ptr->info.mt_event_report_mode = mode;
    ev_report_ptr->info.mt_event_report_buf  = buffer;


    switch (mode)
    {
        case MN_GPRS_EVENT_REPORT_MODE_1:
        {
            /* do nothing */
            break;
        }
    
        case MN_GPRS_EVENT_REPORT_MODE_2:
        case MN_GPRS_EVENT_REPORT_MODE_3:
        {
            /* in at command it is same for case 1 and 2         */
            /* handle the response content in buffer             */
            switch(buffer)
            {
                case MN_GPRS_EVENT_REPORT_BUFFER_1:
                {
                    /* clear buffer                              */
                    SCI_MEMSET(ev_report_ptr->buf, 0, sizeof(ev_report_ptr->buf));
                    ev_report_ptr->len = 0;
                    break;
                }
                
                case MN_GPRS_EVENT_REPORT_BUFFER_2:
                {
                    /* flushed to the TE                         */
                    g_rsp_str[0] = atc_config_ptr->s3_reg;//cr
                    g_rsp_str[1] = atc_config_ptr->s4_reg;//lf
                    sprintf((char*)g_rsp_str + 2, "OK");
                #ifdef _MUX_ENABLE_ 
                     if(ATC_DEBUG_LINK == atc_config_ptr->current_link_id)
                    {
                        SIO_ATC_WriteCmdRes((uint8*)g_rsp_str, strlen((char*)g_rsp_str) );
                        SIO_ATC_WriteCmdRes(ev_report_ptr->buf, ev_report_ptr->len);
                    }
                    else
                    {
                        MUX_Write(g_rsp_str, strlen((char*)g_rsp_str), atc_config_ptr->current_link_id);
                        MUX_Write(ev_report_ptr->buf, ev_report_ptr->len, atc_config_ptr->current_link_id);
                    }
               #else
                    SIO_ATC_WriteCmdRes((uint8*)g_rsp_str, strlen((char*)g_rsp_str) );
                    SIO_ATC_WriteCmdRes(ev_report_ptr->buf, ev_report_ptr->len);
                #endif
               
                    SCI_MEMSET(ev_report_ptr->buf, 0, sizeof(ev_report_ptr->buf));
                    ev_report_ptr->len = 0;
                    break;
                }

                default:
                    break;
            }
            break;
        }
    
        default:
            break;
    }

    return ERR_MNGPRS_NO_ERR;
}

/*****************************************************************************/
//  Description : 1. turn on or off: Manual response to a network request for PDP context activation 
//                2. Based on S0
//  Global      : none
//  Author      : 
//  Note        :
//  History     :
/*****************************************************************************/
LOCAL void SetResMode( //return whether success or failure
                MN_DUAL_SYS_E dual_sys,
                MN_GPRS_TE_RESPONSE_MODE_E res_mode //the requested response mode
                )
{
    g_atc_mt_gprs_res_mode[dual_sys] = res_mode;
}

/*****************************************************************************/
//  Description : show reponse mode set by GetResMode
//  Global      : none
//  Author      : 
//  Note        :
//  History     :
/*****************************************************************************/
LOCAL MN_GPRS_TE_RESPONSE_MODE_E  GetResMode( //return whether success or failure
                MN_DUAL_SYS_E dual_sys
                )
{
    return g_atc_mt_gprs_res_mode[dual_sys];
}
#if 0
/*****************************************************************************/
//  Function    : HandleCGANS
//  Description : 
//                set CGANS var
//  Return      : 
//  Global      : none
//  Note        :
//  History     :
/*****************************************************************************/
LOCAL ERR_MNGPRS_CODE_E HandleCGANS(
                MN_DUAL_SYS_E dual_sys, 
                MN_GPRS_TE_RESPONSE_TYPE_E res_type,
                BOOLEAN is_all, 
                int32 pdp_id
                )
{
    ERR_MNGPRS_CODE_E result = ERR_MNGPRS_NO_ERR;
    
    g_atc_mt_response_type[dual_sys] = res_type;

    g_atc_te_pdp_context[dual_sys][0] = pdp_id;

    /* set is_all flag                                           */
    if (TRUE == is_all)
    {
        g_atc_is_all_pdp[dual_sys] = TRUE;
    }
    else
    {
        g_atc_is_all_pdp[dual_sys] = FALSE;
    }

    if(!g_atc_is_network_request[dual_sys])
    {
        /* if no network request just set response type          */
        return result;
    }

    /* Deactivate or Activate pdp context based on response:xgxu */
    /* means for all pdp context; should modify add new parameter*/ 
    /* in entry of function                                      */
    switch( g_atc_mt_response_type[dual_sys] )
    {
        case MN_GPRS_TE_REJECT_ACTIVE:
        {
            result = MNGPRS_DeactivatePdpContextEx(dual_sys, g_atc_is_all_pdp[dual_sys],g_atc_te_pdp_context[dual_sys]);
            break;
        }
    
        case MN_GPRS_MT_ACTIVE:
        {
            result = MNGPRS_ActivatePdpContextEx(dual_sys, g_atc_is_all_pdp[dual_sys],g_atc_te_pdp_context[dual_sys],POC_E,MN_UNSPECIFIED);
            break;
        }

        default:
            break;
    }
    
    return result;
}
#endif
#endif /*_GPRS_FULL_FUNCTION_*/
#endif /*_SUPPORT_GPRS_*/

/******************************************************************************/ 
//    DESCRIPTION:This function handles command AT+CGPADDR, 
//                Show PDP address
//
//    Global: atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : Modified by Hyman.wu
//    History:
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGPADDR( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                              )
{
#ifdef _SUPPORT_GPRS_
//#ifdef _GPRS_FULL_FUNCTION_
    uint32 i;
    ATC_SHOW_PDP_ADDR_REQ_T mn_info;
    ATC_CUR_PARAM_INFO_LIST_T *param_ptr;
    ERR_MNGPRS_CODE_E ret;    
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_TRACE_LOW("ATGPRS:ATC_ProcessCGPADDR\n");
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_3002_112_2_17_22_17_54_809,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {    
            MN_GPRS_PDP_ADDR_T   *pdp_addr_arr;
            uint32   input_pid_num = 0;
            uint8   j;
            
            mn_info.is_all = TRUE;
            param_ptr = cur_cmd_info->cur_param_list_ptr;
            for(i = 0; param_ptr!=NULL && i<MN_GPRS_MAX_PDP_CONTEXT_COUNT; i++)
            {
                mn_info.pdp_id[i] = 0;
                
                if (cur_cmd_info->param_used[i])
                {
                    mn_info.pdp_id[i] = param_ptr->param_info.num;
                    input_pid_num++;
                }
                        
                param_ptr = param_ptr->next;
            }
            
            if( i!= 0 && i<MN_GPRS_MAX_PDP_CONTEXT_COUNT)
            {
                mn_info.is_all = FALSE;
                mn_info.pdp_id[i] = 0;
            }            
            
            ret = MNGPRS_ReadPdpContextAddrEx(dual_sys, mn_info.pdp_id,&pdp_addr_arr);
            
            //ATC_TRACE:"ATGPRS:ATC_ProcessCGPADDR\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_3038_112_2_17_22_17_54_810,(uint8*)"" );
            
            if(ERR_MNGPRS_NO_ERR != ret)
            {
                if(pdp_addr_arr != PNULL)
                {
                    SCI_FREE(pdp_addr_arr);
                }
                return ERR_OPERATION_NOT_SUPPORTED;
        
            }
            
            for(i = 0; i < input_pid_num; i++)
            {
                if(i == 0)
                {
                    sprintf((char*)g_rsp_str,"+CGPADDR: %ld,", mn_info.pdp_id[i]);
                }
                else
                {
                    sprintf((char*)g_rsp_str + strlen((char*)g_rsp_str),"\r\n+CGPADDR: %ld,", mn_info.pdp_id[i]);
                }
                
                //SCI_TRACE_LOW("ATC: NUM= %d",pdp_addr_arr[i].length);
                
                if(pdp_addr_arr[i].length == -1)
                {
                    sprintf((char*)g_rsp_str + strlen((char*)g_rsp_str),"id invalid");
                    continue;
                }
                
                if(pdp_addr_arr[i].length == 0)
                {
                    sprintf((char*)g_rsp_str + strlen((char*)g_rsp_str),"\"0.0.0.0\"");
                    continue;
                }
                
                for(j = 0; j < pdp_addr_arr[i].length - 1; j++)
                {
                    sprintf((char*)g_rsp_str + strlen((char*)g_rsp_str),"%d.",pdp_addr_arr[i].value_arr[j]);
                    
                }
                sprintf((char*)g_rsp_str + strlen((char*)g_rsp_str),"%d",pdp_addr_arr[i].value_arr[j]);   
    
            }
            
            ATC_BuildInfoRsp(atc_config_ptr,g_rsp_str);
            
            if(pdp_addr_arr != PNULL)
            {
                SCI_FREE(pdp_addr_arr);
            }
            
            break;
        }
        
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }
//#endif /* _GPRS_FULL_FUNCTION_ */ 
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */   
}

#if 0
/******************************************************************************/ 
//    DESCRIPTION:This function handles command AT+CGAUTO
//           The set command disables or enables an automatic positive response 
//               (auto-answer) to the receipt of a Request PDP Context Activation message 
//               from the network.
//
//    Global : atc_config_ptr 
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGAUTO( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                             )
{
#ifdef _SUPPORT_GPRS_
#ifdef _GPRS_FULL_FUNCTION_
    ATC_AUTO_RESP_NET_T mn_info;
    MN_GPRS_TE_RESPONSE_MODE_E gprs_res_mode = MN_GPRS_MODE_TURN_OFF_CGANS;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (cur_cmd_info->param_used[0])
            {
                mn_info.resp_mode = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }        

            SetResMode(dual_sys, (MN_GPRS_TE_RESPONSE_MODE_E)(mn_info.resp_mode));
            break;
        }
    
        case ATC_CMD_TYPE_READ:
        {
            gprs_res_mode = GetResMode(dual_sys);
            sprintf((char*)g_rsp_str, "%s: %ld", g_atc_info_table[AT_CMD_CGAUTO].cmd_name->str_ptr,
                        (uint32)gprs_res_mode);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

            break;
        }
    
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }
#endif /* _GPRS_FULL_FUNCTION_ */
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/ 
//    DESCRIPTION:This function handles command AT+CGANS
//           The execution command requests the MT to respond to a network request 
//               for GPRS PDP context activation which has been signalled to the TE 
//               by the RING or +CRING: unsolicited result code. 
//           The <response> parameter allows the TE to accept or reject the request.
//
//    Global : atc_config_ptr 
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGANS( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            )
{
#ifdef _SUPPORT_GPRS_
#ifdef _GPRS_FULL_FUNCTION_
    ATC_MANUAL_RESP_NET_T mn_info;

    char layer2name[MAX_AT_PARA_STR_LEN];
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    //ATC_TRACE:"ATGPRS:ATC_ProcessCGANS\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_3180_112_2_17_22_17_54_811,(uint8*)"" );

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // response type
            if (cur_cmd_info->param_used[0])
            {
                mn_info.response = PARA1.num;
            }
            else
            {
                mn_info.response = 0;
            }

            // Layer 2 Protocol
            if (cur_cmd_info->param_used[1])
            {
                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)// not equal string type
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
                    
                SCI_MEMCPY(layer2name,PARA2.str_ptr->str_ptr,PARA2.str_ptr->str_len);
                layer2name[PARA2.str_ptr->str_len]='\0';
                //ATC_TRACE:"layer 2 Protocol:%s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_3209_112_2_17_22_17_54_812,(uint8*)"s",layer2name);
                
                if( Is_Support_L2P(layer2name) )
                {
                    mn_info.l2p = 1;
                }
                else
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else
            {
                //ATC_TRACE:"default Layer 2 Protocol"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_3222_112_2_17_22_17_54_813,(uint8*)"");
            }
            
            // <cid> pdp_id
            if (cur_cmd_info->param_used[2])
            {
                //ATC_TRACE:"para 3 pdp id:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_3228_112_2_17_22_17_54_814,(uint8*)"d", PARA3.num);
                mn_info.pdp_id = PARA3.num;
                mn_info.is_all = FALSE;
            }
            else
            {
                mn_info.is_all = TRUE;    //means for all pdp context
            }
            
            if (mn_info.pdp_id > MN_GPRS_MAX_PDP_ID_NUM)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            
            HandleCGANS(dual_sys, (MN_GPRS_TE_RESPONSE_TYPE_E)mn_info.response, mn_info.is_all, mn_info.pdp_id);
            break;
        }
    
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }
#endif /* _GPRS_FULL_FUNCTION_ */
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}

/******************************************************************************/ 
//    DESCRIPTION: This function handles command AT+CGCLASS 
//           The set command is used to set the MT to operate according to 
//               the specified GPRS mobile class.
//    Global: atc_config_ptr 
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGCLASS( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                              )
{
#ifdef _SUPPORT_GPRS_
    int  class_type=0;
    char class_str[MAX_AT_PARA_STR_LEN];
    char G_Class[5] = "";
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;       

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (cur_cmd_info->param_used[0])
            {
                SCI_MEMCPY(class_str,PARA1.str_ptr->str_ptr,PARA1.str_ptr->str_len);
                class_str[PARA1.str_ptr->str_len] = '\0';
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            //ATC_TRACE:"input class name class%s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PLUS_GPRS_3288_112_2_17_22_17_54_815,(uint8*)"s",class_str);

            if( (class_type = IS_SUPPORT_GPRS_CLASS(class_str) )!= MN_PHONE_USER_SERVICE_MAX_NUM )
            {
                MNPHONE_SetServiceTypeEx(dual_sys, class_type);
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            break;
        }
            
        case ATC_CMD_TYPE_READ:
        {
            switch(MNPHONE_GetServiceTypeEx(dual_sys))
            {
                case MN_PHONE_USER_SELECT_GSM_ONLY:
                {
                    //Class CC
                    strcpy(G_Class, "CC");
                    break;
                }
                
                case MN_PHONE_USER_SELECT_GPRS_ONLY:
                case MN_PHONE_USER_SELECT_GSM_GPRS_BOTH:
                {
                    //Class B
                    strcpy(G_Class, "B");
                    break;
                }
                
                case MN_PHONE_USER_SELECT_NONE:
                default:
                {
                    //Class NONE
                    strcpy(G_Class, "NONE");
                    break;
                }
            }
            sprintf((char*)g_rsp_str, "%s: %s", g_atc_info_table[AT_CMD_CGCLASS].cmd_name->str_ptr, G_Class);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

            break;
        }

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */    
}
#endif
