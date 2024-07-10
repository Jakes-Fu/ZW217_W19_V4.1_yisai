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
#include "atc_plus_gprs.h"
#include "atc_basic_cmd.h"
#include "atc.h"
#include "sig_code.h"
#include "atc_tcp_if.h"

#ifdef _MNGPS_SUPPORT_
#include "atc_gps.h"
#endif

#ifdef _SCM21_ATC_DEV
//#include "mngprs_module.h"
#endif /* _SCM21_ATC_DEV */

#include "atc_gprs_modem.h"

#define MAX_AT_PARA_STR_LEN  128                 // for str parameter len
#define GPRS_MTU             1516
#define MAX_AT_GPRSDATA_SEND_LEN   (GPRS_MTU*10)    //10 is SNDCP queue's size

static BOOLEAN s_atc_second_auth_flag = FALSE;//HW AT+CGAUTH command required.
static  ATC_CGAUTH_CONF_NV_T   s_atc_auth_param;

static SCI_TIMER_PTR s_atc_ats7_timer[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {PNULL};//ATS7

extern RSP_RESULT_STR      g_rsp_str;
extern const ATC_INFO_T    g_atc_info_table[];
extern BOOLEAN  g_is_dns_act_pdp;
extern BOOLEAN  g_is_dns_pdp_actived;

extern BOOLEAN g_atc_ip_detach;

extern ATC_CONFIG_T         s_config_info;

#ifdef _MUX_ENABLE_
extern TCPIP_PACKET_TYPE_E  g_packet_type[MN_GPRS_MAX_PDP_CONTEXT_COUNT+1];
#endif
/*lint -save -e752 */
extern MN_DUAL_SYS_E       g_current_card_id;//lint !e752

#ifndef ATC_SAT_ENABLE
MN_GPRS_TE_RESPONSE_TYPE_E  g_atc_mt_response_type[MN_SYS_NUMBER] =
{
#if defined MULTI_SIM_SYS_DUAL
    MN_GPRS_TE_REJECT_ACTIVE, 
#elif defined MULTI_SIM_SYS_TRI
    MN_GPRS_TE_REJECT_ACTIVE, 
    MN_GPRS_TE_REJECT_ACTIVE, 
#elif defined MULTI_SIM_SYS_QUAD
    MN_GPRS_TE_REJECT_ACTIVE, 
    MN_GPRS_TE_REJECT_ACTIVE, 
    MN_GPRS_TE_REJECT_ACTIVE, 
#endif
    MN_GPRS_TE_REJECT_ACTIVE
};// value explication see 0707-10.1.10 +CGANS

uint32   g_atc_te_pdp_context[MN_SYS_NUMBER][MN_GPRS_MAX_PDP_CONTEXT_COUNT] = 
{
#if defined MULTI_SIM_SYS_DUAL
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
#elif defined MULTI_SIM_SYS_TRI
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
#elif defined MULTI_SIM_SYS_QUAD
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
#endif
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

static BOOLEAN    g_atc_is_network_request[MN_SYS_NUMBER] = 
{
#if defined MULTI_SIM_SYS_DUAL
    0, 
#elif defined MULTI_SIM_SYS_TRI
    0, 
    0, 
#elif defined MULTI_SIM_SYS_QUAD
    0, 
    0, 
    0, 
#endif
    0
};

static BOOLEAN    g_atc_is_all_pdp[MN_SYS_NUMBER] =
{
#if defined MULTI_SIM_SYS_DUAL
    0, 
#elif defined MULTI_SIM_SYS_TRI
    0, 
    0, 
#elif defined MULTI_SIM_SYS_QUAD
    0, 
    0, 
    0, 
#endif
    0
};
#endif

ATC_GPRS_EV_REPORT_DETAIL_T g_atc_mt_event_report_mode[MN_SYS_NUMBER] = 
{
#if defined MULTI_SIM_SYS_DUAL
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0},
#elif defined MULTI_SIM_SYS_TRI
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0},
#elif defined MULTI_SIM_SYS_QUAD
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0}, 
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0},
#else
    {{MN_GPRS_EVENT_REPORT_MODE_2,MN_GPRS_EVENT_REPORT_BUFFER_2}, {0}, 0},
#endif
};   


extern MN_SMS_SERVICE_TYPE_E       g_sms_ser_type[MN_SYS_NUMBER];
#ifdef _MUX_ENABLE_
MN_DUAL_SYS_E   g_atc_modem_dial_sys = MN_DUAL_SYS_1;
extern  ATC_MUX_MOD_E g_atc_mux_mode;
#endif

#ifndef ATC_SAT_ENABLE
#ifdef _SUPPORT_GPRS_
typedef struct mngprs_dummy_data {
    uint32 data_size;
    uint32 gprs_mtu;
    uint16 nsapi;
} ATC_DUMMY_DATA_STRUCT;
static ATC_DUMMY_DATA_STRUCT s_atc_dummy_data = {0};
#endif
#endif

#ifdef _MNGPS_SUPPORT_
extern BOOLEAN s_is_gps_act_pdp_req;
#endif

extern uint32    s_pid_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT];
//extern void IPOPEN_ProcessActPdpContextCnf(ATC_CONFIG_T *atc_config_ptr, APP_MN_GPRS_T *sig_ptr);
extern void ATC_ProcessPDPDeactivatedByNetwork(ATC_CONFIG_T  *atc_config_ptr);

extern int   stricmp(char *s1, const char *s2);

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
//  Description : set QoS profile
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS SetQosProfile_R98(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info,   //IN:
    BOOLEAN             is_minqos        //IN:
);

/******************************************************************************/
//  DESCRIPTION: read QOS profile
//  Global resource dependence :
//  AUTHOR: Tracy Zhang
//  Note :
/******************************************************************************/
LOCAL ATC_STATUS ReadQosProfile_R98(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T   *atc_config_ptr, //OUT:
    BOOLEAN        is_minqos        //IN:
);

/*****************************************************************************/
//  Description : set 3G QoS profile
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS SetQosProfile_R99(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info,   //IN:
    BOOLEAN             is_minqos        //IN:
);

#ifdef _GPRS_FULL_FUNCTION_
/*****************************************************************************/
//  Description : read 3G QoS profile
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL void ConvertTotalQos2ExtQos(
    MNGPRS_3G_EXTEND_QOS_SET_T *ext_qos_ptr,   //OUT:
    const MN_GPRS_TOTAL_QOS_T  *total_qos_ptr  //IN:
);

/*****************************************************************************/
//  Description : judge the cid is valid when reading 3G QoS profile
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValidPidInReadQos(
    MN_DUAL_SYS_E dual_sys,
    uint32                    cur_cid,        //IN:
    BOOLEAN                   is_negotiated,  //IN:
    ATC_NEGOTIATED_QOS_REQ_T  *neg_req_ptr    //IN:
);

/*****************************************************************************/
//  Description : read 3G QoS profile
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ReadQosProfile_R99(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T                *atc_config_ptr, //OUT:
    BOOLEAN                     is_minqos,       //IN:
    BOOLEAN                     is_negotiated,   //IN:
    ATC_NEGOTIATED_QOS_REQ_T    *neg_req_ptr     //IN:
);
#endif

#ifndef ATC_SAT_ENABLE
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
    ATC_CONFIG_T *atc_config_ptr,
    MN_DUAL_SYS_E dual_sys,
    MN_GPRS_TE_RESPONSE_TYPE_E res_type,
    BOOLEAN is_all,
    int32 pdp_id
);

#endif
#endif

#ifdef _SUPPORT_GPRS_
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
#endif

#ifdef _SUPPORT_GPRS_
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
#endif
#ifndef ATC_SAT_ENABLE
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
#endif
/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CGDCONT, set pdp context
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
/******************************************************************************/
ATC_STATUS ATC_ProcessCGDCONT( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    uint32   i;
    ATC_SET_PDP_CONTEXT_REQ_T  mn_info;

    ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;    
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessCGDCONT");

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

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
            //copy the at param indicator
            for(i = 0; i < ARR_SIZE(mn_info.param_used); i++)
            {
                mn_info.param_used[i] = cur_cmd_info->param_used[i];
            }

            //param 1 --- cid
            if(mn_info.param_used[0])
            {
                mn_info.pdp_id = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            //param 2 --- PDP_type
            if(mn_info.param_used[1])
            {
                ATC_CUR_PARAM_STRING_T *type_ptr = PNULL;

                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                type_ptr = PARA2.str_ptr;

                for(i = 0; i < type_ptr->str_len; i++)
                {
                    /*lint -e718*/
                    /*lint -e746*/
                    /*lint -e628*/
                    if(isalpha(*(type_ptr->str_ptr + i)))/*lint !e48*/
                    {
                        *(type_ptr->str_ptr + i) = toupper(*(type_ptr->str_ptr + i));
                    }

                    /*lint +e746*/
                    /*lint +e718*/
                }

                // support IP/IPV6/IPV4V6
                if (stricmp((char *)type_ptr->str_ptr, "IP") && 
                    stricmp((char *)type_ptr->str_ptr, "IPV6") && 
                    stricmp((char *)type_ptr->str_ptr, "IPV4V6"))
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCGDCONT: only support IP (not %s).", type_ptr->str_ptr);
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                strcpy((char *)mn_info.pdp_type, "IP");
            }

            //param 3 --- APN
            if(mn_info.param_used[2])
            {
                ATC_CUR_PARAM_STRING_T  *apn_ptr        = PNULL;

                if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                apn_ptr = PARA3.str_ptr;
                strncpy((char *)(mn_info.apn), (char *)(apn_ptr->str_ptr), apn_ptr->str_len);

                mn_info.apn[apn_ptr->str_len] = '\0';
            }

            //param 4 --- PDP_address
            if(mn_info.param_used[3])
            {
                char    *token = PNULL;
				ATC_CUR_PARAM_STRING_T *type_ptr = PNULL;

                mn_info.pdp_address.length = 0;

                if(PARAM4_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCGDCONT, param4 is not string!");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                token = (char *)(PARA4.str_ptr->str_ptr);
				type_ptr = PARA2.str_ptr;

                if(PNULL != token)
                {
	                if (0 == stricmp((char *)type_ptr->str_ptr, "IP"))
	                {
	                    token = strtok(token, ".");

	                    while(PNULL != token)
	                    {
	                        ATC_TRACE_LOW("%s", token);
	                        mn_info.pdp_address.value_arr[mn_info.pdp_address.length] = atoi(token);

	                        if(mn_info.pdp_address.value_arr[mn_info.pdp_address.length] == 0)
	                        {
	                            if(*token != '0')
	                            {
	                                //parse token error!
	                                return ERR_OPERATION_NOT_SUPPORTED;
	                            }
	                        }

	                        token = strtok(PNULL, ".");
	                        mn_info.pdp_address.length++;
	                    }
					}
					else if(0 == stricmp((char *)type_ptr->str_ptr, "IPV6"))
	                {
	                    uint8  temp[2] = {0};
	                    char temp_token[4] = {0};
	                    token = strtok(token,":");

	                    temp[0] = 0;
	                    temp[1] = 0;

	                    if (token == NULL)
	                    {
	                        SCI_TRACE_LOW("pdp address can not be null.");
	                    }
	                    else if ( 1 == strlen(token))
	                    {
	                        temp_token[0] = temp_token[1] = temp_token[2] = '0';
	                        temp_token[3] = *token;
	                    }
	                    else if ( 2 == strlen(token))
	                    {
	                        temp_token[0] = temp_token[1] = '0';
	                        temp_token[2] = token[0];
	                        temp_token[3] = token[1];
	                    }
	                    else if ( 3 == strlen(token))
	                    {
	                        temp_token[0] = '0';
	                        temp_token[1] = token[0];
	                        temp_token[2] = token[1];
	                        temp_token[3] = token[2];
	                    }
	                    else if ( 4 == strlen(token))
	                    {
	                        temp_token[0] = token[0];
	                        temp_token[1] = token[1];
	                        temp_token[2] = token[2];
	                        temp_token[3] = token[3];
	                    }

	                    ConvertHexToBin((uint8 *)temp_token, 4,(uint8 *)temp);

	                    while(token != NULL)
	                    {
	                        temp[0] = 0;
	                        temp[1] = 0;

	                        if ( 1 == strlen(token))
	                        {
	                            temp_token[0] = temp_token[1] = temp_token[2] = '0';
	                            temp_token[3] = *token;
	                        }
	                        else if ( 2 == strlen(token))
	                        {
	                            temp_token[0] = temp_token[1] = '0';
	                            temp_token[2] = token[0];
	                            temp_token[3] = token[1];
	                        }
	                        else if ( 3 == strlen(token))
	                        {
	                            temp_token[0] = '0';
	                            temp_token[1] = token[0];
	                            temp_token[2] = token[1];
	                            temp_token[3] = token[2];
	                        }
	                        else if ( 4 == strlen(token))
	                        {
	                            temp_token[0] = token[0];
	                            temp_token[1] = token[1];
	                            temp_token[2] = token[2];
	                            temp_token[3] = token[3];
	                        }

	                        ConvertHexToBin((uint8 *)temp_token, 4, (uint8 *)temp);

	                        mn_info.pdp_address.value_arr[mn_info.pdp_address.length++] = temp[0];
	                        mn_info.pdp_address.value_arr[mn_info.pdp_address.length] = temp[1];

	                        if(mn_info.pdp_address.value_arr[mn_info.pdp_address.length] == 0)
	                        {
	                            if(*token != '0')
	                            {
	                                return ERR_OPERATION_NOT_SUPPORTED;
	                            }
	                        }

	                        mn_info.pdp_address.length++;

	                        token = strtok(NULL,":");

	                    }
	                }
                }
            }

            //param 5 --- d_comp
            if(mn_info.param_used[4])
            {
                if((0 <= PARA5.num) && (PARA5.num <= 3))
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
            if(mn_info.param_used[5])
            {
                if((0 <= PARA6.num) && (PARA6.num <= 4))
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
            if(mn_info.param_used[6])
            {
                ATC_CUR_PARAM_STRING_T *pco_ptr = PARA7.str_ptr;

                if(PNULL != pco_ptr)
                {
                    strncpy((char *)mn_info.pco, (char *)pco_ptr->str_ptr, pco_ptr->str_len);
                    mn_info.pco[pco_ptr->str_len] = '\0';
                }
            }

            //+CGDCONT= <cid>, causes the values for context number <cid> to become undefined
            if((mn_info.param_used[1] == FALSE) &&
            (mn_info.param_used[2] == FALSE) &&
            (mn_info.param_used[3] == FALSE) &&
            (mn_info.param_used[4] == FALSE) &&
            (mn_info.param_used[5] == FALSE) &&
            (mn_info.param_used[6] == FALSE))
            {
                ret = MNGPRS_DeletePdpContextEx(dual_sys, (BOOLEAN)(!(mn_info.param_used[0])), mn_info.pdp_id);

                if(ERR_MNGPRS_NO_ERR != ret)
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

                if(ERR_MNGPRS_NO_ERR != ret)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
        }
            break;

        case ATC_CMD_TYPE_READ:
            {
//#ifdef _GPRS_FULL_FUNCTION_
#ifndef ATC_SAT_ENABLE
                MN_GPRS_PDP_CON_T   *pdp_con_arr = PNULL;
                uint32              pdp_con_num = 0;
                uint8                j = 0;

                ret = MNGPRS_ReadPdpContextEx(dual_sys, &pdp_con_arr, &pdp_con_num);

                ATC_TRACE_LOW("ATC : CONTEXT NUM %d", pdp_con_num);

                if(ret != ERR_MNGPRS_NO_ERR)
                {
                    if(pdp_con_arr != PNULL)
                    {
                        SCI_FREE(pdp_con_arr);
                    }

                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                ATC_TRACE_LOW("ATC: ReadPDPCON pdp_con_num: %d", pdp_con_num);

                if(pdp_con_num == 0)
                {
                    // There isn't PDP context.
                    sprintf((char *)g_rsp_str, "+CGDCONT:");

                }

                for(i = 0; i < pdp_con_num; i++)
                {
                    if(i == 0)
                    {
                        sprintf((char *)g_rsp_str, "+CGDCONT: %ld,%s,\"%s\",\"",
                        pdp_con_arr[i].pdp_id,
                        pdp_con_arr[i].pdp_addr_type,
                        pdp_con_arr[i].apn);
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "\r\n+CGDCONT: %ld,%s,\"%s\",\"",
                        pdp_con_arr[i].pdp_id,
                        pdp_con_arr[i].pdp_addr_type,
                        pdp_con_arr[i].apn);
                    }

                    for(j = 0; j < pdp_con_arr[i].pdp_addr_str.length - 1; j++)
                    {
                        sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d.", pdp_con_arr[i].pdp_addr_str.value_arr[j]);
                    }

                    ATC_TRACE_LOW("ATC: ReadPDPCON pdp_con_arr[i].pdp_addr_str.length: %d", pdp_con_arr[i].pdp_addr_str.length);

                    if(j == 0)
                    {
                        sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "0.0.0.0\"");
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d\"", pdp_con_arr[i].pdp_addr_str.value_arr[j]);
                    }

                    sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), ",%d", pdp_con_arr[i].d_comp);
                    sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), ",%d", pdp_con_arr[i].h_comp);
                }

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                if(pdp_con_arr != PNULL)
                {
                    SCI_FREE(pdp_con_arr);
                }

#endif
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
//  DESCRIPTION:This function handles command AT+CGPCO, set pco
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Shijun.cui
//  Note : None
/******************************************************************************/
ATC_STATUS ATC_ProcessCGPCO( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    uint32 result   = 0;
    uint32 pdp_id   = 0;
    uint8  type     = 0;
    uint8  auth     = 0;
    uint8 auth_type = 0;
    uint8  user[MAX_PAP_USER_LEN + 1]={0};
    uint8  passwd[MAX_PAP_PASSWD_LEN + 1]={0};
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

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
            if(PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG && PARAM4_FLAG)
            {
                type   = PARA1.num;
                pdp_id = PARA4.num;
                if(PARAM5_FLAG)
                {
                    auth   = PARA5.num;
                }
                else
                {
                    auth   = 1; //dafault PAP
                }

                if(type) //HEX type
                {
                    if((PARA2.str_ptr->str_len <= MAX_PAP_USER_LEN * 2) || (PARA3.str_ptr->str_len <= MAX_PAP_PASSWD_LEN * 2))
                    {
                        ConvertHexToBin(PARA2.str_ptr->str_ptr, PARA2.str_ptr->str_len, user);
                        ConvertHexToBin(PARA3.str_ptr->str_ptr, PARA3.str_ptr->str_len, passwd);
                    }
                    else
                    {
                        ATC_TRACE_LOW("ATC: pco user or passwd too long.");
                        return ERR_INVALID_INDEX;
                    }
                }
                else // ASCII type
                {
                    if((PARA2.str_ptr->str_len <= MAX_PAP_USER_LEN) && (PARA3.str_ptr->str_len <= MAX_PAP_PASSWD_LEN))
                    {
                        strncpy((char *)user, (char *)(PARA2.str_ptr->str_ptr), PARA2.str_ptr->str_len);
                        user[PARA2.str_ptr->str_len] = '\0';
                        strncpy((char *)passwd, (char *)(PARA3.str_ptr->str_ptr), PARA3.str_ptr->str_len);
                        passwd[PARA3.str_ptr->str_len] = '\0';
                    }
                    else
                    {
                        ATC_TRACE_LOW("ATC: pco user or passwd too long.");
                        return ERR_INVALID_INDEX;
                    }
                }

                if(auth == 2)
                {
                    auth_type = MN_CHAP;
                }
                else //auth == 0,1 or 3
                {
                    auth_type = MN_PAP;
                }

                result = MNGPRS_SetPdpContextPcoCipherEx(dual_sys, pdp_id, user, passwd,(MN_PCO_AUTH_TYPE_E)auth_type);

                if(result != MN_GPRS_ERR_SUCCESS)
                {
                    ATC_TRACE_LOW("ATC: set pco error, result = %d", result);
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
		  //HW:clear auth type set in the at+cgauth	
                if(pdp_id == s_atc_auth_param.cgauth_cid)
                {
                    s_atc_auth_param.cgauth_type = MN_AUTH_NONE;                
                }

            }
            else
            {
                ATC_TRACE_LOW("ATC: set pco error parameter");
                return ERR_OPERATION_NOT_SUPPORTED;
            }
}
            break;

        case ATC_CMD_TYPE_READ:
            {
#ifdef _GPRS_FULL_FUNCTION_
                MN_GPRS_PCO_T   *pdp_pco_ptr    = PNULL;
                uint32          pdp_context_num = 0;
                uint32          i               = 0;

                ATC_TRACE_LOW("ATC:ATC_ProcessCGPCO,READ.");

                result = MNGPRS_ReadPdpContextPcoEx(dual_sys, &pdp_pco_ptr, &pdp_context_num);

                ATC_TRACE_LOW("ATC:ATC_ProcessCGPCO,READ.");

                if(result != MN_GPRS_ERR_SUCCESS)
                {
                    if(pdp_pco_ptr != PNULL)
                    {
                        SCI_FREE(pdp_pco_ptr);
                    }

                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                if(pdp_context_num == 0)
                {
                    sprintf((char *)g_rsp_str, "+CGPCO: ");
                }

                for(i = 0; i < pdp_context_num; i++)
                {
                    if(i == 0)
                    {
                        if(pdp_pco_ptr[i].is_valid)
                        {

                            sprintf((char *)g_rsp_str, "+CGPCO: 0,\"%s\",\"%s\",%ld",
                            pdp_pco_ptr[i].user, pdp_pco_ptr[i].passwd, pdp_pco_ptr[i].pdp_id);
                        }
                        else
                        {
                            sprintf((char *)g_rsp_str, "+CGPCO: 0,,,0,%ld",
                            pdp_pco_ptr[i].pdp_id);
                        }
                    }
                    else
                    {
                        if(pdp_pco_ptr[i].is_valid)
                        {

                            sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "\r\n+CGPCO: 0,\"%s\",\"%s\",%ld",
                            pdp_pco_ptr[i].user, pdp_pco_ptr[i].passwd, pdp_pco_ptr[i].pdp_id);
                        }
                        else
                        {
                            sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "\r\n+CGPCO: 0,,,0,%ld",
                            pdp_pco_ptr[i].pdp_id);
                        }
                    }
                }

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                //Free pdp_pco_ptr memory
                if(pdp_pco_ptr != PNULL)
                {
                    SCI_FREE(pdp_pco_ptr);
                }

#endif
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
//  DESCRIPTION:This function handles command AT+CSTT, set pco
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: 
//  Note : None
/******************************************************************************/
ATC_STATUS ATC_ProcessCSTT( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#if 0
#ifdef _SCM21_ATC_DEV
#define MAX_STRING_LEN 50
    uint32 result   = 0;
    uint32 pdp_id   = 0;

  	ATC_SET_PDP_CONTEXT_REQ_T  mn_info;
    MNGPRS_PDP_CONTEXT_T *pdp_context  = NULL;
  	uint8  apn[MAX_STRING_LEN + 1] ={0};
    uint8  user[MAX_STRING_LEN + 1]={0};
    uint8  passwd[MAX_STRING_LEN + 1]={0};
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

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
            if(PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG)
            {

                if((PARA1.str_ptr->str_len <= MAX_STRING_LEN)&&(PARA2.str_ptr->str_len <= MAX_STRING_LEN) && (PARA3.str_ptr->str_len <= MAX_STRING_LEN))
                {
                    strncpy((char *)apn, (char *)(PARA1.str_ptr->str_ptr), PARA1.str_ptr->str_len);
                    apn[PARA1.str_ptr->str_len] = '\0';
                    strncpy((char *)user, (char *)(PARA2.str_ptr->str_ptr), PARA2.str_ptr->str_len);
                    user[PARA2.str_ptr->str_len] = '\0';
                    strncpy((char *)passwd, (char *)(PARA3.str_ptr->str_ptr), PARA3.str_ptr->str_len);
                    passwd[PARA3.str_ptr->str_len] = '\0';
                }
                else
                {
                    ATC_TRACE_LOW("ATC: cstt apn or user or passwd too long.");
                    return ERR_INVALID_INDEX;
                }

				//only validate pdp_id & apn
				mn_info.param_used[0] = TRUE;   //pdp_id MASK bit
				mn_info.param_used[1] = FALSE;	//pdp_type MASK bit
				mn_info.param_used[2] = TRUE;	//apn MASK bit
				mn_info.param_used[3] = FALSE;	//pdp_addr MASK bit
				mn_info.param_used[4] = FALSE;	//d_comp MASK bit
				mn_info.param_used[5] = FALSE;	//h_comp MASK bit
				mn_info.param_used[6] = FALSE;	//pco_str MASK bit
				
				for(pdp_id = 1;pdp_id <= 11;pdp_id++)
				{
				    pdp_context = GetPdpContextFromId(dual_sys,pdp_id);
				        
				    if (pdp_context != NULL)
				    {
						result = MNGPRS_SetPdpContextEx(dual_sys, mn_info.param_used,
		                pdp_id,
		                mn_info.pdp_type,
		                apn,
		                mn_info.pdp_address,
		                mn_info.d_comp,
		                mn_info.h_comp,
		                mn_info.pco);
						if(result != MN_GPRS_ERR_SUCCESS)
		                {
		                    ATC_TRACE_LOW("ATC: set apn error, result = %d", result);
		                    return ERR_OPERATION_NOT_SUPPORTED;
		                }
						//set  username & password
		                result = MNGPRS_SetPdpContextPcoCipherEx(dual_sys, pdp_id, user, passwd,(MN_PCO_AUTH_TYPE_E)MN_PAP);

		                if(result != MN_GPRS_ERR_SUCCESS)
		                {
		                    ATC_TRACE_LOW("ATC: set pco error, result = %d", result);
		                    return ERR_OPERATION_NOT_SUPPORTED;
		                }
				    }	
				}
				SetGPRSDataConfStatus(TRUE, 0, ATC_MODEM_IP_START);
            }
            else
            {
                ATC_TRACE_LOW("ATC: set apn or pco error parameter");
                return ERR_OPERATION_NOT_SUPPORTED;
            }
		}
            break;

        case ATC_CMD_TYPE_READ:
        {
#ifdef _GPRS_FULL_FUNCTION_
			MN_GPRS_PDP_CON_T   *pdp_con_arr = PNULL;
            MN_GPRS_PCO_T   *pdp_pco_ptr    = PNULL;
            uint32          pdp_context_num = 0;
            uint32          i               = 0;
			//read apn
            result = MNGPRS_ReadPdpContextEx(dual_sys, &pdp_con_arr, &pdp_context_num);

            ATC_TRACE_LOW("ATC : CONTEXT NUM %d", pdp_context_num);

            if(result != ERR_MNGPRS_NO_ERR)
            {
                if(pdp_con_arr != PNULL)
                {
                    SCI_FREE(pdp_con_arr);
                }

                return ERR_OPERATION_NOT_SUPPORTED;
            }

            ATC_TRACE_LOW("ATC: ReadPDPCON pdp_con_num: %d", pdp_context_num);

            if(pdp_context_num == 0)
            {
                // There isn't PDP context.
                sprintf((char *)g_rsp_str, "+CSTT: \"\",");

            }
			//echo only first cid apn
            for(i = 0; i < pdp_context_num; i++)
            {
            	ATC_TRACE_LOW("ATC: Read apn: %x", pdp_con_arr[i].apn);
                if(pdp_con_arr[i].apn != NULL)
                {
                    sprintf((char *)g_rsp_str, "+CSTT: \"%s\",", pdp_con_arr[i].apn);
					ATC_TRACE_LOW("ATC: Read apn valid! g_rsp_str= %s", g_rsp_str);
					break;
                }
            }
            ATC_TRACE_LOW("ATC:ATC_ProcessCSTT,READ apn end.");
			//set to 0
			pdp_context_num = 0;
            result = MNGPRS_ReadPdpContextPcoEx(dual_sys, &pdp_pco_ptr, &pdp_context_num);

            ATC_TRACE_LOW("ATC:ATC_ProcessCSTT,READ username & password");

            if(result != MN_GPRS_ERR_SUCCESS)
            {
                if(pdp_pco_ptr != PNULL)
                {
                    SCI_FREE(pdp_pco_ptr);
                }

                return ERR_OPERATION_NOT_SUPPORTED;
            }

            if(pdp_context_num == 0)
            {
                sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "\"\",\"\"");
            }
			//echo only first cid pco
            for(i = 0; i < pdp_context_num; i++)
            {

                if(pdp_pco_ptr[i].is_valid)
                {

                    sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "\"%s\",\"%s\"",
                    pdp_pco_ptr[i].user, pdp_pco_ptr[i].passwd);
					break;
                }
                else
                {
                    sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "\"\",\"\"");
					break;
                }
            }
			//Free pdp_con_arr memory
            if(pdp_con_arr != PNULL)
            {
                SCI_FREE(pdp_con_arr);
            }
            //Free pdp_pco_ptr memory
            if(pdp_pco_ptr != PNULL)
            {
                SCI_FREE(pdp_pco_ptr);
            }
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

#endif
            break;
        }
		case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "+CSTT:\"APN\",\"USR\",\"PWD\"");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
		case ATC_CMD_TYPE_EXECUTE:
			break;
        default:

            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SCM21_ATC_DEV */
#endif /* #if 0*/
    return S_ATC_FAIL;
}

/*****************************************************************************/
//  Description : This function initializes the nv static variables for auth param
//  Global resource dependence :
//  Author:       
//  Note:
/*****************************************************************************/
void ATC_InitAUTHConfig(void)
{	
    if(NVERR_NONE !=EFS_NvitemRead(NV_CGAUTH_CONFIG_ID, sizeof(ATC_CGAUTH_CONF_NV_T), (uint8 *)&s_atc_auth_param))
    {
        SCI_TRACE_LOW("ATC: Read param from NV fail");
        s_atc_auth_param.cgauth_cid = 1;
        s_atc_auth_param.cgauth_type = MN_PAP;
        s_atc_auth_param.cgauth_user[MAX_PAP_USER_LEN] = 0;
        s_atc_auth_param.cgauth_passwd[MAX_PAP_PASSWD_LEN] =0;
          
        //write the NV.
        if(NVERR_NONE != EFS_NvitemWrite(NV_CGAUTH_CONFIG_ID, sizeof(ATC_CGAUTH_CONF_NV_T), (uint8 *)&s_atc_auth_param, FALSE))
        {
            SCI_TRACE_LOW("ATC: write NV_ATC_CONFIG_ID fail");
        }
    }
}
/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CGAUTH, set pco
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: vicky.wang
//  Note : None
/******************************************************************************/
ATC_STATUS ATC_ProcessCGAUTH( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    uint32 result   = 0;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

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
            if(PARAM1_FLAG && PARAM3_FLAG && PARAM4_FLAG )
            {
                s_atc_auth_param.cgauth_cid = (uint8)PARA1.num;
        		
                if(PARAM2_FLAG)
                {
                    if(0 ==  PARA2.num)
                    {
                        s_atc_auth_param.cgauth_type = MN_AUTH_NONE;
                    }
                    else
                    {
                        s_atc_auth_param.cgauth_type = PARA2.num - 1;                  
                    }
                }
                else
                {
                    s_atc_auth_param.cgauth_type = MN_PAP;  //default PAP
                }
        		
                // input user and passwd
                if((PARA3.str_ptr->str_len <= MAX_PAP_USER_LEN) && (PARA4.str_ptr->str_len <= MAX_PAP_PASSWD_LEN))
                {
                    strncpy((char *)s_atc_auth_param.cgauth_user, (char *)(PARA3.str_ptr->str_ptr), PARA3.str_ptr->str_len);
                                  s_atc_auth_param.cgauth_user[PARA3.str_ptr->str_len] = '\0';
                    strncpy((char *)s_atc_auth_param.cgauth_passwd, (char *)(PARA4.str_ptr->str_ptr), PARA4.str_ptr->str_len);
                                   s_atc_auth_param.cgauth_passwd[PARA4.str_ptr->str_len] = '\0';
                }
                else
                {
                    ATC_TRACE_LOW("ATC: auth param user or passwd too long.");
                    return ERR_INVALID_INDEX;
                }
        		
                s_atc_second_auth_flag = FALSE;
				
                //save the nv
                if (NVERR_NONE != EFS_NvitemWrite(NV_CGAUTH_CONFIG_ID,
                                        sizeof(ATC_CGAUTH_CONF_NV_T),
                                        (uint8 *)&s_atc_auth_param,FALSE))
                {
                    SCI_TRACE_LOW("ATC_CGAUTH:EFS_NvitemWrite error");
                    return S_ATC_FAIL;  
                } 
				
                switch(s_atc_auth_param.cgauth_type)
                {
                    case MN_AUTH_NONE:
                    case MN_PAP:
                    case MN_CHAP:
        		result = MNGPRS_SetPdpContextPcoCipherEx(dual_sys, s_atc_auth_param.cgauth_cid,
        			(uint8 *)s_atc_auth_param.cgauth_user, (uint8 *)s_atc_auth_param.cgauth_passwd,s_atc_auth_param.cgauth_type); 
        	    break;
        
                    case MN_PAP_CHAP:
        	    result = MNGPRS_SetPdpContextPcoCipherEx(dual_sys, s_atc_auth_param.cgauth_cid, 
        			(uint8 *)s_atc_auth_param.cgauth_user, (uint8 *)s_atc_auth_param.cgauth_passwd,MN_PAP);
        	    break;
        
        	default:
        	    break;
                }
        		 
                if(result != MN_GPRS_ERR_SUCCESS)
                {
                    ATC_TRACE_LOW("ATC: set auth param error, result = %d", result);
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else
            {
                ATC_TRACE_LOW("ATC: set auth  error parameter");
                return ERR_OPERATION_NOT_SUPPORTED;
            }
        }
            break;

        case ATC_CMD_TYPE_READ:
        {
#ifdef _GPRS_FULL_FUNCTION_
                uint32 auth_type = 0;

                if(MN_AUTH_NONE == s_atc_auth_param.cgauth_type)
                {
                    auth_type = 0; //0: NONE
                }
                else
                {
                    auth_type = s_atc_auth_param.cgauth_type + 1;
                }

                ATC_TRACE_LOW("ATC:ATC_ProcessCGAUTH,READ.");
				
                sprintf((char *)g_rsp_str, "+CGAUTH: %ld, %d,\"%s\",\"%s\"",
                    s_atc_auth_param.cgauth_cid,
                    auth_type,
                    (uint8 *)s_atc_auth_param.cgauth_user, 
                    (uint8 *)s_atc_auth_param.cgauth_passwd);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#endif
                break;
        }
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "+CGAUTH: (1-3),(0-3),,,");
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
//  Description : set QoS profile
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS SetQosProfile_R98(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info,   //IN:
    BOOLEAN             is_minqos        //IN:
)
{
    MN_GPRS_TOTAL_QOS_T qos;
    uint32              pdp_id      = 0;
    ATC_STATUS          at_result   = S_ATC_DEFAULT_HANDLE;
    ERR_MNGPRS_CODE_E   mn_ret      = ERR_MNGPRS_NO_ERR;
    uint16              event_id    = 0;
    ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;    

    // init the qos
    SCI_MEMSET(&qos, 0, sizeof(qos));

    //param1--pid
    if(cur_cmd_info->param_used[0])
    {
        pdp_id = PARA1.num;
    }
    else
    {
        return ERR_INVALID_INDEX;
    }

    //param2--precedence
    if(cur_cmd_info->param_used[1])
    {
        qos.precedence = (MN_GPRS_PRECEDENCE_CLASS_E)(PARA2.num);
    }

    //param3--delay class
    if(cur_cmd_info->param_used[2])
    {
        qos.delay_class = (MN_GPRS_DELAY_CLASS_E)(PARA3.num);
    }

    //param4--reliability
    if(cur_cmd_info->param_used[3])
    {
        qos.reliability = (MN_GPRS_RELIABILITY_TYPE_E)(PARA4.num);
    }

    //param5--peak throughput
    if(cur_cmd_info->param_used[4])
    {
        qos.peak_thrput = (MN_GPRS_PEAK_THROUGHPUT_TYPE_E)(PARA5.num);
    }

    //param6--mean throughput
    if(cur_cmd_info->param_used[5])
    {
        qos.mean_thrput = (MN_GPRS_MEAN_THROUGHPUT_TYPE_E)(PARA6.num);
    }

    //+CGQREQ= <cid> causes the requested profile for context number <cid> to become undefined.
    if((cur_cmd_info->param_used[1] == FALSE) &&
    (cur_cmd_info->param_used[2] == FALSE) &&
    (cur_cmd_info->param_used[3] == FALSE) &&
    (cur_cmd_info->param_used[4] == FALSE) &&
    (cur_cmd_info->param_used[5] == FALSE))
    {
        if(is_minqos)
        {
            ret = MNGPRS_DeleteMinQosProfileEx(dual_sys, (BOOLEAN)(!(cur_cmd_info->param_used[0])), pdp_id, qos);
        }
        else
        {
            ret = MNGPRS_DeleteQosProfileEx(dual_sys, (BOOLEAN)(!(cur_cmd_info->param_used[0])), pdp_id, qos);
        }
        
        if(ERR_MNGPRS_NO_ERR != ret)
        {
            return ERR_OPERATION_NOT_SUPPORTED;
        }
            at_result = S_ATC_SUCCESS;
    }
    else
    {
        qos.is_r99  = FALSE;

        if(is_minqos)
        {
            mn_ret      = MNGPRS_SetTotalMinQosProfileEx(dual_sys, pdp_id, qos);
            event_id    = APP_MN_SET_MIN_QOS_CNF;
        }
        else
        {
            mn_ret      = MNGPRS_SetTotalQosProfileEx(dual_sys, pdp_id, qos);
            event_id    = APP_MN_SET_QOS_CNF;
        }

        if(ERR_MNGPRS_NO_ERR == mn_ret)
        {
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, event_id, pdp_id, ATC_DOMAIN_PS, dual_sys);
        }
        else
        {
            ATC_TRACE_LOW("ATC: SetQosProfile_R98 MN return = %d.", mn_ret);
            at_result = ERR_OPERATION_NOT_SUPPORTED;
        }
    }

    return (at_result);
}

/******************************************************************************/
//  DESCRIPTION: read QOS profile
//  Global resource dependence :
//  AUTHOR: Tracy Zhang
//  Note :
/******************************************************************************/
LOCAL ATC_STATUS ReadQosProfile_R98(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T   *atc_config_ptr, //OUT:
    BOOLEAN        is_minqos        //IN:
)
{
    ATC_STATUS          at_status    = S_ATC_SUCCESS;
    ERR_MNGPRS_CODE_E   mn_result    = ERR_MNGPRS_NO_ERR;
    int32               str_len      = 0;
    MN_GPRS_OUT_QOS_T   *cur_qos_arr = PNULL;
    uint32              pdp_con_num  = 0;
    uint32              i            = 0;

    if(is_minqos)
    {
        mn_result   = MNGPRS_ReadMinQosProfileEx(dual_sys, &cur_qos_arr, &pdp_con_num);
        str_len     = sprintf((char *)g_rsp_str, "+CGQMIN: ");
    }
    else
    {
        mn_result   = MNGPRS_ReadCurrQosProfileEx(dual_sys, &cur_qos_arr, &pdp_con_num);
        str_len     = sprintf((char *)g_rsp_str, "+CGQREQ: ");
    }

    if(ERR_MNGPRS_NO_ERR == mn_result)
    {
        for(i = 0; i < pdp_con_num; i++)
        {
            str_len += sprintf((char *)g_rsp_str + str_len, "%ld,%d,%d,%d,%d,%d",
            cur_qos_arr[i].pdp_id,
            cur_qos_arr[i].qos.precedence,
            cur_qos_arr[i].qos.delay_class,
            cur_qos_arr[i].qos.reliability,
            cur_qos_arr[i].qos.peak_thrput,
            cur_qos_arr[i].qos.mean_thrput);

            if(i < pdp_con_num - 1)
            {
                if(is_minqos)
                {
                    str_len += sprintf((char *)g_rsp_str + str_len, "\r\n+CGQMIN: ");
                }
                else
                {
                    str_len += sprintf((char *)g_rsp_str + str_len, "\r\n+CGQREQ: ");
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

    if(cur_qos_arr != PNULL)
    {
        SCI_FREE(cur_qos_arr);
    }

    return (at_status);
}

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGQREQ, set Qos
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note :
/******************************************************************************/
ATC_STATUS ATC_ProcessCGQREQ( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    ATC_STATUS  at_status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessCGQREQ");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            at_status = SetQosProfile_R98(dual_sys, atc_config_ptr, cur_cmd_info, FALSE);
            break;

        case ATC_CMD_TYPE_READ:
#ifdef _GPRS_FULL_FUNCTION_
            at_status = ReadQosProfile_R98(dual_sys, atc_config_ptr, FALSE);
#endif
            break;

        default:
            at_status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return (at_status);
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */
}

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGQMIN, set min Qos
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
/******************************************************************************/
ATC_STATUS ATC_ProcessCGQMIN( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    ATC_STATUS  at_status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessCGQMIN");

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            at_status = SetQosProfile_R98(dual_sys, atc_config_ptr, cur_cmd_info, TRUE);
            break;

        case ATC_CMD_TYPE_READ:
#ifdef _GPRS_FULL_FUNCTION_
            at_status = ReadQosProfile_R98(dual_sys, atc_config_ptr, TRUE);
#endif
            break;

        default:
            at_status = ERR_OPERATION_NOT_SUPPORTED;
    }

    return (at_status);
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */
}

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CGATT, attach or detach GPRS service
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
//  History:
//            2003.12.02 xiaogang.xu   Fix CR-MS00005916
/******************************************************************************/
ATC_STATUS ATC_ProcessCGATT( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    int32  state    = 0;

    ATC_TRACE_LOW("ATC: ATC_ProcessCGATT");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            //param1--state
            if(cur_cmd_info->param_used[0])
            {
                state = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            if(1 == state)
            {
                if(atc_config_ptr->atc_config_nv.spauto_flag & 0x04)
                {
                    return S_ATC_SUCCESS;
                }
                MNPHONE_GprsAttachEx(dual_sys, MN_PHONE_GPRS_ATTACH);
            }
            else if(0 == state)
            {
                if(ERR_MNPHONE_NO_ERR != MNPHONE_GprsDetachEx(dual_sys, MN_PHONE_GPRS_DETACH))
                {
                    ATC_TRACE_LOW("ATC: MNPHONE_GprsDetach failure.");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else
            {
                //Other values will result in an ERROR
                return ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:
            {
                MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;
                plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

                if((PLMN_NORMAL_GPRS_ONLY == plmn_info.plmn_status) ||
                   (PLMN_NORMAL_GSM_GPRS_BOTH == plmn_info.plmn_status))
                {
                    state = 1;
                }
                else
                {
                    state = 0;
                }

                sprintf((char *)g_rsp_str, "+CGATT: %ld", state);

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
//  DESCRIPTION:This function handles command AT+CGACT, activate or deactivate pdp context
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
/******************************************************************************/
ATC_STATUS ATC_ProcessCGACT( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    ERR_MNGPRS_CODE_E   ret     = ERR_MNGPRS_NO_ERR;
    int32               state   = 0;
    uint32              i       = 0;

    ATC_PDP_ACT_REQ_T          mn_info;
    ATC_CUR_PARAM_INFO_LIST_T  *param_ptr;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessCGACT");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    SCI_MEMSET(&mn_info, 0, sizeof(mn_info));


    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            //param1--state
            if(PARAM1_FLAG)
            {
                state = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            mn_info.is_all  = TRUE;
            param_ptr       = cur_cmd_info->cur_param_list_ptr->next;

            //no <cid>s are specified,activates all defined contexts.
            for(i = 0; (param_ptr != PNULL) && (i < MN_GPRS_MAX_PDP_CONTEXT_COUNT); i++)
            {
                mn_info.pdp_id[i] = 0;

                if(cur_cmd_info->param_used[i+1])
                {
                    mn_info.pdp_id[i] = param_ptr->param_info.num;
                }

                param_ptr = param_ptr->next;
            }

            if((i != 0) && (i < MN_GPRS_MAX_PDP_CONTEXT_COUNT))
            {
                /*<cid>s are specified*/
                mn_info.is_all    = FALSE;
                mn_info.pdp_id[i] = 0;
            }
            else if(1 == state)
            {
                /*no <cid>s specified in activate state, it will activate cid==1*/
                mn_info.is_all    = FALSE;
                mn_info.pdp_id[0] = 1; //default is for pdp1 not pdp2
                mn_info.pdp_id[1] = MN_GPRS_PDP_ID_INVALID; //0
            }

            if(1 == state)
            {
#ifdef _MUX_ENABLE_

                /*if l4 has cleared pdp context but ATC task has not handled APP_MN_DEACTIVATE_PDP_CONTEXT_CNF yet,
                  It is not allowed to activate pdp context with the same cid.*/
                if(ATC_Is_Expected_Event_Present(APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, (uint16)mn_info.pdp_id[0], ATC_DOMAIN_PS, dual_sys)  ||
                    ATC_Is_Expected_Event_Present(APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, ATC_CHECKID_FOR_DEACTIVATE_ALL_PDP, ATC_DOMAIN_PS, dual_sys))
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCGACT, this pdp id is being deactivated.");
                    return ERR_GPRS_OPERATION_FAILURE;
                }

#endif
                if(s_atc_auth_param.cgauth_type == MN_PAP_CHAP)
                {
                    s_atc_second_auth_flag = FALSE;
                    MNGPRS_SetPdpContextPcoCipherEx(dual_sys,mn_info.pdp_id[0],
					                                (uint8 *)s_atc_auth_param.cgauth_user,
					                                (uint8 *)s_atc_auth_param.cgauth_passwd,
					                                MN_PAP);
                }

                //ret = MNGPRS_ActivatePdpContextEx(dual_sys, mn_info.is_all, mn_info.pdp_id, MAX_PS_TYPE_E, MN_UNSPECIFIED);
                ret = ATC_ActivatePdpContextEx(atc_config_ptr, dual_sys, mn_info.is_all, mn_info.pdp_id, MAX_PS_TYPE_E, MN_UNSPECIFIED);

                if(ERR_MNGPRS_NO_ERR != ret)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

#ifdef _MUX_ENABLE_
                ATC_Link_ATHEventHandle(atc_config_ptr->current_link_id, ATC_ATH_EV_NEW_PDP_SRV, (uint8)mn_info.pdp_id[0]);
                ATC_Create_PSD_Call(mn_info.pdp_id[0], atc_config_ptr->current_link_id, FALSE);
                g_atc_modem_dial_sys = dual_sys;
                ATC_Add_Expected_Event(atc_config_ptr, APP_MN_ACTIVATE_PDP_CONTEXT_CNF,
                                 mn_info.pdp_id[0], ATC_DOMAIN_PS, dual_sys);
#else
                ATC_Link_ATHEventHandle(0, ATC_ATH_EV_NEW_PDP_SRV, (uint8)mn_info.pdp_id[0]);
                ATC_Create_PSD_Call(mn_info.pdp_id[0], 0, FALSE);
#endif
            }
            else if(0 == state)
            {
#ifdef _MUX_ENABLE_

                if(ATC_Check_Expected_Event(atc_config_ptr->current_link_id, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, (uint8)mn_info.pdp_id[0], dual_sys))
                {
                    return ERR_INVALID_INDEX;
                }
                else
#endif
                {
                    ret = MNGPRS_DeactivatePdpContextEx(dual_sys, mn_info.is_all, mn_info.pdp_id);

                    if(ERR_MNGPRS_NO_ERR != ret)
                    {
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }

                    if(((mn_info.pdp_id[0] >= 1) && (mn_info.pdp_id[0] <= MN_GPRS_MAX_PDP_CONTEXT_COUNT))
                    && (ATC_PSD_STATE_INACTIVE != atc_config_ptr->atc_psd_call_info[mn_info.pdp_id[0] - 1].state))
                    {
                        ATC_Set_PSD_Call_State(mn_info.pdp_id[0], ATC_PSD_STATE_DEACTIVATE_PENDING);
                    }

#ifdef _MUX_ENABLE_
                    if(mn_info.is_all)
                    {
                        //add expected event with cid==0xff for deactivating all PDP contexts
                        ATC_Add_Expected_Event(atc_config_ptr, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,
                                         ATC_CHECKID_FOR_DEACTIVATE_ALL_PDP, ATC_DOMAIN_PS, dual_sys);
                    }
                    else
                    {
                        //add expected event with cid==mn_info.pdp_id[0] for deactivating one PDP context
                        ATC_Add_Expected_Event(atc_config_ptr, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,
                                         mn_info.pdp_id[0], ATC_DOMAIN_PS, dual_sys);
                    }
#endif
                }
            }
            else
            {
                //Other values will result in an ERROR response
                return ERR_INVALID_INDEX;
            }

            ResetGPRSDataConf();
            //end
            return (atc_config_ptr->async_mode_flag ? S_ATC_SUCCESS : S_ATC_DEFAULT_HANDLE);

        case ATC_CMD_TYPE_READ:
            {
//#ifdef _GPRS_FULL_FUNCTION_
#ifndef ATC_SAT_ENABLE
                MN_GPRS_CON_STATE_T *pdp_con_state_arr  = PNULL;
                uint32              pdp_con_num         = 0;
                BOOLEAN             is_active           = 0;

                ret = MNGPRS_ReadPdpContextStateEx(dual_sys, &pdp_con_state_arr, &pdp_con_num);

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
                    sprintf((char *)g_rsp_str, "+CGACT: ");
                }

                for(i = 0; i < pdp_con_num; i++)
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
                        if(i == 0)
                        {
                            sprintf((char *)g_rsp_str, "+CGACT: %ld,1",
                            pdp_con_state_arr[i].pdp_id);
                        }
                        else
                        {
                            sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "\r\n+CGACT: %ld,1",
                            pdp_con_state_arr[i].pdp_id);
                        }
                    }
                    else
                    {
                        if(i == 0)
                        {
                            sprintf((char *)g_rsp_str, "+CGACT: %ld,0",
                            pdp_con_state_arr[i].pdp_id);
                        }
                        else
                        {
                            sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "\r\n+CGACT: %ld,0",
                            pdp_con_state_arr[i].pdp_id);
                        }
                    }
                }

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                if(pdp_con_state_arr != PNULL)
                {
                    SCI_FREE(pdp_con_state_arr);
                }
#endif
//#endif
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

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CIICR, activate pdp context
//  Global resource dependence : atc_config_ptr
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessCIICR( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SCM21_ATC_DEV
    ERR_MNGPRS_CODE_E   ret     = ERR_MNGPRS_NO_ERR;
    ATC_PDP_ACT_REQ_T          mn_info;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessCIICR");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    SCI_MEMSET(&mn_info, 0, sizeof(mn_info));


    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
	        /*no <cid>s specified in this AT CMD, it will activate cid==1*/
	        mn_info.is_all    = FALSE;
	        mn_info.pdp_id[0] = 1; //default is for pdp1 not pdp2
	        mn_info.pdp_id[1] = MN_GPRS_PDP_ID_INVALID; //0

#ifdef _MUX_ENABLE_

            /*if l4 has cleared pdp context but ATC task has not handled APP_MN_DEACTIVATE_PDP_CONTEXT_CNF yet,
              It is not allowed to activate pdp context with the same cid.*/
            if(ATC_Is_Expected_Event_Present(APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, (uint16)mn_info.pdp_id[0], ATC_DOMAIN_PS, dual_sys)  ||
                ATC_Is_Expected_Event_Present(APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, ATC_CHECKID_FOR_DEACTIVATE_ALL_PDP, ATC_DOMAIN_PS, dual_sys))
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCIICR, this pdp id is being deactivated.");
                return ERR_GPRS_OPERATION_FAILURE;
            }

#endif
            if(s_atc_auth_param.cgauth_type == MN_PAP_CHAP)
            {
                s_atc_second_auth_flag = FALSE;
                MNGPRS_SetPdpContextPcoCipherEx(dual_sys,mn_info.pdp_id[0],
				                                (uint8 *)s_atc_auth_param.cgauth_user,
				                                (uint8 *)s_atc_auth_param.cgauth_passwd,
				                                MN_PAP);
            }

            //ret = MNGPRS_ActivatePdpContextEx(dual_sys, mn_info.is_all, mn_info.pdp_id, MAX_PS_TYPE_E, MN_UNSPECIFIED);
            ret = ATC_ActivatePdpContextEx(atc_config_ptr, dual_sys, mn_info.is_all, mn_info.pdp_id, MAX_PS_TYPE_E, MN_UNSPECIFIED);

            if(ERR_MNGPRS_NO_ERR != ret)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }

#ifdef _MUX_ENABLE_
            ATC_Link_ATHEventHandle(atc_config_ptr->current_link_id, ATC_ATH_EV_NEW_PDP_SRV, (uint8)mn_info.pdp_id[0]);
            ATC_Create_PSD_Call(mn_info.pdp_id[0], atc_config_ptr->current_link_id, FALSE);
            g_atc_modem_dial_sys = dual_sys;
            ATC_Add_Expected_Event(atc_config_ptr, APP_MN_ACTIVATE_PDP_CONTEXT_CNF,
                             mn_info.pdp_id[0], ATC_DOMAIN_PS, dual_sys);
#else
            ATC_Link_ATHEventHandle(0, ATC_ATH_EV_NEW_PDP_SRV, (uint8)mn_info.pdp_id[0]);
            ATC_Create_PSD_Call(mn_info.pdp_id[0], 0, FALSE);
#endif
            ResetGPRSDataConf();

			SetGPRSDataConfStatus(TRUE, 0, ATC_MODEM_IP_CONFIG);
            //end
            return (atc_config_ptr->async_mode_flag ? S_ATC_SUCCESS : S_ATC_DEFAULT_HANDLE);

        case ATC_CMD_TYPE_TEST:
           // sprintf((char *)g_rsp_str, "+CIICR: OK");
           // ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
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
//  DESCRIPTION:This function handles command AT+CIPSHUT, deactivate pdp context
//  Global resource dependence : atc_config_ptr
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessCIPSHUT( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SCM21_ATC_DEV
    ERR_MNGPRS_CODE_E   ret     = ERR_MNGPRS_NO_ERR;
    ATC_STATUS  at_status = S_ATC_SUCCESS;
    ATC_PDP_ACT_REQ_T          mn_info;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessCIPSHUT");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    SCI_MEMSET(&mn_info, 0, sizeof(mn_info));


    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
	        /*no <cid>s specified in this AT CMD, it will deactivate all cids which are exist*/
	        mn_info.is_all    = TRUE;

#ifdef _MUX_ENABLE_

            if(ATC_Check_Expected_Event(atc_config_ptr->current_link_id, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, (uint8)mn_info.pdp_id[0], dual_sys))
            {
                return ERR_INVALID_INDEX;
            }
            else
#endif
            {
                ret = MNGPRS_DeactivatePdpContextEx(dual_sys, mn_info.is_all, mn_info.pdp_id);

                if(ERR_MNGPRS_NO_ERR != ret)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                if(((mn_info.pdp_id[0] >= 1) && (mn_info.pdp_id[0] <= MN_GPRS_MAX_PDP_CONTEXT_COUNT))
                && (ATC_PSD_STATE_INACTIVE != atc_config_ptr->atc_psd_call_info[mn_info.pdp_id[0] - 1].state))
                {
                    ATC_Set_PSD_Call_State(mn_info.pdp_id[0], ATC_PSD_STATE_DEACTIVATE_PENDING);
                }

#ifdef _MUX_ENABLE_
                if(mn_info.is_all)
                {
                    //add expected event with cid==0xff for deactivating all PDP contexts
                    ATC_Add_Expected_Event(atc_config_ptr, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,
                                     ATC_CHECKID_FOR_DEACTIVATE_ALL_PDP, ATC_DOMAIN_PS, dual_sys);
                }
                else
                {
                    //add expected event with cid==mn_info.pdp_id[0] for deactivating one PDP context
                    ATC_Add_Expected_Event(atc_config_ptr, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,
                                     mn_info.pdp_id[0], ATC_DOMAIN_PS, dual_sys);
                }
#endif
            }

        ResetGPRSDataConf();

        //end
        ATC_TRACE_LOW("ATC: ATC_ProcessCIPSHUT,atc_config_ptr->async_mode_flag =%d",atc_config_ptr->async_mode_flag);

		s_config_info.is_echo_shut_ok = TRUE; //after process ATC_ProcessDeactivatePDPContextCNF, echo "SHUT OK" on TE

		return (atc_config_ptr->async_mode_flag ? S_ATC_SUCCESS : S_ATC_DEFAULT_HANDLE);

		//return  S_ATC_SHUT_SUCCESS;// retrun SHUT OK

        case ATC_CMD_TYPE_TEST:

		at_status = S_ATC_SUCCESS;
		break;
        default:
            at_status =  ERR_OPERATION_NOT_SUPPORTED;
    }

    return at_status;
#else
    return S_ATC_FAIL;
#endif /* _SCM21_ATC_DEV  */
}

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CIFSR,Show PDP address
//  Global: atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : 
//  History:
/******************************************************************************/
ATC_STATUS ATC_ProcessCIFSR( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SCM21_ATC_DEV 
    ATC_SHOW_PDP_ADDR_REQ_T     mn_info;
    ERR_MNGPRS_CODE_E           ret         = ERR_MNGPRS_NO_ERR;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32                      i           = 0;

    ATC_TRACE_LOW("ATC: ATC_ProcessCIFSR");

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/
	SCI_MEMSET(&g_rsp_str, 0, MAX_ATC_RSP_LEN);
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            {
                MN_GPRS_PDP_ADDR_T  *pdp_addr_arr = PNULL;
                uint8               j             = 0;
		        /*no <cid>s specified in this AT CMD, it will get  cid==1*/
		        mn_info.is_all    = FALSE;
		        mn_info.pdp_id[0] = 1; //default is for pdp1
		        mn_info.pdp_id[1] = MN_GPRS_PDP_ID_INVALID; //0
		        
				if(MNGPRS_IsActPdpExistEx())
				{
	                ret = MNGPRS_ReadPdpContextAddrEx(dual_sys, mn_info.pdp_id, &pdp_addr_arr);

	                if(ERR_MNGPRS_NO_ERR != ret)
	                {
	                    if(pdp_addr_arr != PNULL)
	                    {
	                        SCI_FREE(pdp_addr_arr);
	                    }

	                    return ERR_OPERATION_NOT_SUPPORTED;
	                }
					if(pdp_addr_arr[0].length == -1)
	                {
	                    sprintf((char *)g_rsp_str, "id invalid");
	                }
					else if(pdp_addr_arr[0].length == 0)
	                {
	                    sprintf((char *)g_rsp_str, "\"0.0.0.0\"");
	                }
					else
	                {
		                for(j = 0; j < pdp_addr_arr[0].length - 1; j++)
		                {
		                    sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d.", pdp_addr_arr[0].value_arr[j]);
		                }
						sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d", pdp_addr_arr[i].value_arr[j]);
					}

	                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

	                if(pdp_addr_arr != PNULL)
	                {
	                    SCI_FREE(pdp_addr_arr);
	                }
					s_config_info.not_echo_ok_when_success = TRUE;

					//Set ip status to IP_STATUS
					SetGPRSDataConfStatus(TRUE, 0, ATC_MODEM_IP_STATUS);
					
	                break;

				}
				else //pdp not activated
				{
					return S_ATC_FAIL;
				}

            }
		case ATC_CMD_TYPE_TEST:
			break;
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }


    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SCM21_ATC_DEV  */
}


/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CGPADDR,
//                Show PDP address
//
//  Global: atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
//  History:
//    xu.xiaogang 2003-9-10 Add actual process
/******************************************************************************/
ATC_STATUS ATC_ProcessCGPADDR( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    ATC_SHOW_PDP_ADDR_REQ_T     mn_info;
    ATC_CUR_PARAM_INFO_LIST_T   *param_ptr  = PNULL;
    ERR_MNGPRS_CODE_E           ret         = ERR_MNGPRS_NO_ERR;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32                      i           = 0;

    ATC_TRACE_LOW("ATC: ATC_ProcessCGPADDR");

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            {
                MN_GPRS_PDP_ADDR_T  *pdp_addr_arr = PNULL;
                uint32              input_pid_num = 0;
                uint8               j             = 0;

                mn_info.is_all  = TRUE;
                param_ptr       = cur_cmd_info->cur_param_list_ptr;

                for(i = 0; (param_ptr != PNULL) && (i < MN_GPRS_MAX_PDP_CONTEXT_COUNT); i++)
                {
                    mn_info.pdp_id[i] = 0;

                    if(cur_cmd_info->param_used[i])
                    {
                        mn_info.pdp_id[i] = param_ptr->param_info.num;

                        input_pid_num++;
                    }

                    param_ptr = param_ptr->next;
                }

                if((i != 0) && (i < MN_GPRS_MAX_PDP_CONTEXT_COUNT))
                {
                    mn_info.is_all    = FALSE;
                    mn_info.pdp_id[i] = 0;
                }

                ret = MNGPRS_ReadPdpContextAddrEx(dual_sys, mn_info.pdp_id, &pdp_addr_arr);

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
                        sprintf((char *)g_rsp_str, "+CGPADDR: %ld,", mn_info.pdp_id[i]);
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "\r\n+CGPADDR: %ld,", mn_info.pdp_id[i]);
                    }

                    ATC_TRACE_LOW("ATC: NUM= %d", pdp_addr_arr[i].length);

                    if(pdp_addr_arr[i].length == -1)
                    {
                        sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "id invalid");
                        continue;
                    }

                    if(pdp_addr_arr[i].length == 0)
                    {
                        sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "\"0.0.0.0\"");
                        continue;
                    }

                    for(j = 0; j < pdp_addr_arr[i].length - 1; j++)
                    {
                        sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d.", pdp_addr_arr[i].value_arr[j]);
                    }

                    sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d", pdp_addr_arr[i].value_arr[j]);
                }

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                if(pdp_addr_arr != PNULL)
                {
                    SCI_FREE(pdp_addr_arr);
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

#ifdef _SUPPORT_GPRS_
/******************************************************************************/
//  DESCRIPTION:
//         check if gived str is supported protocol
//  Global:
//  AUTHOR     : xu.xiaogang
//  Note       : None
//  History:
/******************************************************************************/
static int Is_Support_L2P(char *l2p_str)
{
    /* should check all supported L2P, but now we only support PPP            */
    /* so just simple compare                                                 */
    if((0 == strcmp(l2p_str, "PPP"))
        || (0 == strcmp(l2p_str, "ppp")))
    {
    #ifdef _MUX_ENABLE_
        g_packet_type[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = TCPIP_PKTTYPE_PPP;
    #endif
        return 1;
    }
    else if((0 == strcmp(l2p_str, "M-IP"))
        ||(0 == strcmp(l2p_str, "m-ip")))
    {
    #ifdef _MUX_ENABLE_
        g_packet_type[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = TCPIP_PKTTYPE_IP;
    #endif
        return 1;
    }

    return 0;
}
#endif
/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CGDATA
//         The execution command causes the MT to perform whatever
//             actions are necessary to establish communication between
//             the TE and the network using one or more GPRS PDP types.
//         This may include performing a GPRS attach and one or more PDP context activations.
//
//  Global: atc_config_ptr
//  AUTHOR: Ivan.Yin
/******************************************************************************/
ATC_STATUS ATC_ProcessCGDATA( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    int32                       i           = 0;
    ATC_CUR_PARAM_INFO_LIST_T   *param_ptr  = 0;
    BOOLEAN                     is_all      = FALSE;
    char                        layer2name[MAX_AT_PARA_STR_LEN] = {0};
    ATC_ENTER_DATA_STATE_REQ_T  mn_info;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
#ifdef _MUX_ENABLE_
    int32 set_ppp_result = 0;
#endif

    ATC_TRACE_LOW("ATC: ATC_ProcessCGDATA");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            param_ptr = cur_cmd_info->cur_param_list_ptr;

            if(atc_config_ptr->atc_config_nv.spauto_flag & 0x04)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_CONNECT, 0);
            #ifdef _MUX_ENABLE_
                if(g_mux_sys_info[MUX_1].is_start!=MUX_STATUS_OFF)
                {
                    set_ppp_result = MPPP_SetPPPInterface(5, ATC_GetLinkIdFromCid(1), 1);
                }
                else
                {
                    set_ppp_result = MPPP_SetPPPInterface(5, 0, 1);
                }

                if(set_ppp_result)
                {
                    ATC_TRACE_LOW("ATC: MPPP_SetPPPInterface fail");
                }
            #else

                    TCPIP_SetPPPInterface(5);

            #endif
                return S_ATC_DEFAULT_HANDLE;
            }
            
            if(cur_cmd_info->param_used[0])
            {
                strncpy(layer2name, (char *)(PARA1.str_ptr->str_ptr), PARA1.str_ptr->str_len);

                if(PARA1.str_ptr->str_len == 0)
                {
                    mn_info.l2p = 0;
                }

                if(Is_Support_L2P(layer2name))
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
                ATC_TRACE_LOW("ATC: layer 2 Protocol default?");
                return ERR_INVALID_INDEX;
            }

            is_all    = TRUE;
            param_ptr = param_ptr->next;

            for(i = 0; (i < MN_GPRS_MAX_PDP_CONTEXT_COUNT) && (param_ptr != PNULL); i++)
            {
                mn_info.pdp_id[i] = 0;

                if(cur_cmd_info->param_used[i+1])
                {
                    mn_info.pdp_id[i] = param_ptr->param_info.num;
                }

                param_ptr = param_ptr->next;
            }

            if((i != 0) && (i < MN_GPRS_MAX_PDP_CONTEXT_COUNT))
            {
                is_all            = FALSE;
                mn_info.pdp_id[i] = MN_GPRS_PDP_ID_INVALID;
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCGDATA, not support to activate all pdps.");
                return ERR_OPERATION_NOT_SUPPORTED;
            }

#ifdef _MUX_ENABLE_
            /*if l4 has cleared pdp context but ATC task has not handled APP_MN_DEACTIVATE_PDP_CONTEXT_CNF yet,
              It is not allowed to activate pdp context with the same cid.*/
            if(ATC_Is_Expected_Event_Present(APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, (uint16)mn_info.pdp_id[0], ATC_DOMAIN_PS, dual_sys)
              ||ATC_Is_Expected_Event_Present(APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, ATC_CHECKID_FOR_DEACTIVATE_ALL_PDP, ATC_DOMAIN_PS, dual_sys)
              ||ATC_Is_Expected_Event_Present(APP_MN_ACTIVATE_PDP_CONTEXT_CNF, (uint16)mn_info.pdp_id[0], ATC_DOMAIN_PS, dual_sys))
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCGDATA, this pdp id is being deactivated.");
                return ERR_GPRS_OPERATION_FAILURE;
            }
#endif

            if(s_atc_auth_param.cgauth_type == MN_PAP_CHAP)
            {
                s_atc_second_auth_flag = FALSE;
                MNGPRS_SetPdpContextPcoCipherEx(dual_sys,mn_info.pdp_id[0],
				                                (uint8 *)s_atc_auth_param.cgauth_user,
				                                (uint8 *)s_atc_auth_param.cgauth_passwd,
				                                MN_PAP);
            }

            if(ERR_MNGPRS_NO_ERR == ATC_ActivatePdpContextEx(atc_config_ptr,dual_sys, is_all, mn_info.pdp_id, DOWNLOAD_E, MN_UNSPECIFIED))
            //if(ERR_MNGPRS_NO_ERR == MNGPRS_ActivatePdpContextEx(dual_sys, is_all, mn_info.pdp_id, DOWNLOAD_E, MN_UNSPECIFIED))
            {
                for(i = 0; (i < MN_GPRS_MAX_PDP_CONTEXT_COUNT) && (mn_info.pdp_id[i] != MN_GPRS_PDP_ID_INVALID); i++)
                {
                    if(mn_info.pdp_id[i] >= 1 && mn_info.pdp_id[i] <= MN_GPRS_MAX_PDP_CONTEXT_COUNT)
                    {
#ifdef _MUX_ENABLE_
                        ATC_Create_PSD_Call(mn_info.pdp_id[i], atc_config_ptr->current_link_id, TRUE);
#else
                        ATC_Create_PSD_Call(mn_info.pdp_id[i], 0, TRUE);
#endif
                    }
                }

#ifdef _MUX_ENABLE_
                g_atc_modem_dial_sys = dual_sys;
                g_packet_type[mn_info.pdp_id[0]] = g_packet_type[MN_GPRS_MAX_PDP_CONTEXT_COUNT];
#endif
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_ACTIVATE_PDP_CONTEXT_CNF, mn_info.pdp_id[0], ATC_DOMAIN_PS, dual_sys);
                if (atc_config_ptr->async_mode_flag)
                {
                    return S_ATC_SUCCESS;
                }
            }
            else
            {
                return ERR_GPRS_OPERATION_FAILURE;
            }

            return S_ATC_DEFAULT_HANDLE;

        default:
            return ERR_UNKNOWN;
    }

#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */
}

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CGAUTO
//           The set command disables or enables an automatic positive response
//               (auto-answer) to the receipt of a Request PDP Context Activation message
//               from the network.
//
//  Global : atc_config_ptr
/******************************************************************************/
ATC_STATUS ATC_ProcessCGAUTO( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    ATC_AUTO_RESP_NET_T mn_info;
    MN_GPRS_TE_RESPONSE_MODE_E gprs_res_mode = MN_GPRS_MODE_TURN_OFF_CGANS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessCGAUTO");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(cur_cmd_info->param_used[0])
            {
                mn_info.resp_mode = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            /* set response depend on response type          */
            MNGPRS_SetResModeEx(dual_sys, (MN_GPRS_TE_RESPONSE_MODE_E)mn_info.resp_mode);
            break;

        case ATC_CMD_TYPE_READ:
            gprs_res_mode = MNGPRS_GetResModeEx(dual_sys);
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CGAUTO].cmd_name->str_ptr,
            (uint32)gprs_res_mode);
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

#ifndef ATC_SAT_ENABLE
/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CGANS
//           The execution command requests the MT to respond to a network request
//               for GPRS PDP context activation which has been signalled to the TE
//               by the RING or +CRING: unsolicited result code.
//           The <response> parameter allows the TE to accept or reject the request.
//
//  Global : atc_config_ptr
/******************************************************************************/
ATC_STATUS ATC_ProcessCGANS( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    ATC_MANUAL_RESP_NET_T   mn_info = {0};
    char                    layer2name[MAX_AT_PARA_STR_LEN] = "";
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessCGANS");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            // response type
            if(cur_cmd_info->param_used[0])
            {
                mn_info.response = PARA1.num;
            }
            else
            {
                mn_info.response = 0;
            }

            // Layer 2 Protocol
            if(cur_cmd_info->param_used[1])
            {
                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING) // not equal string type
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCGANS, param2 is not string!");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                strncpy(layer2name, (char *)(PARA2.str_ptr->str_ptr), PARA2.str_ptr->str_len);

                ATC_TRACE_LOW("ATC: layer 2 Protocol:%s", layer2name);

                if(Is_Support_L2P(layer2name))
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
                ATC_TRACE_LOW("ATC: default Layer 2 Protocol");
                mn_info.l2p = 1;
            }

            // <cid> pdp_id
            if(cur_cmd_info->param_used[2])
            {
                ATC_TRACE_LOW("ATC: para 3 pdp id:%d", PARA3.num);
                mn_info.pdp_id = PARA3.num;
                mn_info.is_all = FALSE;
            }
            else
            {
                mn_info.is_all = TRUE;    //means for all pdp context
            }

            if(mn_info.pdp_id > MN_GPRS_MAX_PDP_CONTEXT_COUNT)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            HandleCGANS(atc_config_ptr, dual_sys, (MN_GPRS_TE_RESPONSE_TYPE_E)mn_info.response, mn_info.is_all, mn_info.pdp_id);

            break;
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */
}
#endif


#ifndef ATC_SAT_ENABLE
#ifdef _SUPPORT_GPRS_
/******************************************************************************/
//  DESCRIPTION:
//         check if gived str is supported gprs class
//         CLASS A,B,C,CG,CC
//  Global:
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
#endif
/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGCLASS
//           The set command is used to set the MT to operate according to
//               the specified GPRS mobile class.
//  Global: atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
//  History:
//    xu.xiaogang 2003-9-12 add actual process
/******************************************************************************/
ATC_STATUS ATC_ProcessCGCLASS( // return the handle result
    ATC_CONFIG_T           *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T    *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    int  class_type = 0;
    char class_str[MAX_AT_PARA_STR_LEN];
    char G_Class[5] = "";
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessCGCLASS");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(cur_cmd_info->param_used[0])
            {
                strncpy(class_str, (char *)(PARA1.str_ptr->str_ptr), PARA1.str_ptr->str_len);
                class_str[PARA1.str_ptr->str_len] = '\0';
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            ATC_TRACE_LOW("input class name class%s", class_str);

            if((class_type = IS_SUPPORT_GPRS_CLASS(class_str)) != MN_PHONE_USER_SERVICE_MAX_NUM)
            {
                MNPHONE_SetServiceTypeEx(dual_sys, class_type);
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:

            switch(MNPHONE_GetServiceTypeEx(dual_sys))
            {
                case MN_PHONE_USER_SELECT_GSM_ONLY:
                    //Class CC
                    strcpy(G_Class, "CC");
                    break;

                case MN_PHONE_USER_SELECT_GPRS_ONLY:
                case MN_PHONE_USER_SELECT_GSM_GPRS_BOTH:
                    //Class B
                    strcpy(G_Class, "B");
                    break;

                case MN_PHONE_USER_SELECT_NONE:
                default:
                    //Class NONE
                    strcpy(G_Class, "NONE");
                    break;
            }

            sprintf((char *)g_rsp_str, "%s: %s", g_atc_info_table[AT_CMD_CGCLASS].cmd_name->str_ptr, G_Class);
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
//  DESCRIPTION:This function handles command AT+CGSMS,set
//    SMS service type, GPRS SMS is not supported now
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
/******************************************************************************/
ATC_STATUS ATC_ProcessCGSMS(// return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessCGSMS");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(cur_cmd_info->param_used[0])
            {
                g_sms_ser_type[dual_sys] = (MN_SMS_SERVICE_TYPE_E)PARA1.num;

                ATC_TRACE_LOW("ATC_ProcessCGSMS: %d", PARA1.num);
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:
            /*if return value is invalid set type to default value*/
            if(g_sms_ser_type[dual_sys] == MN_SMS_NO_DOMAIN_SERVICE)
            {
                g_sms_ser_type[dual_sys] = MN_SMS_CS_DOMAIN_SERVICE;
            }
            sprintf((char *)g_rsp_str, "+CGSMS: %d", g_sms_ser_type[dual_sys]);

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
//  DESCRIPTION: This function handles command AT+SSST, set service type
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
/******************************************************************************/
ATC_STATUS ATC_ProcessSSST( // return the handle result
    ATC_CONFIG_T          *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T   *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    ERR_MNPHONE_CODE_E              ret         = ERR_MNPHONE_NO_ERR;
    MN_PHONE_USER_SERVICE_SELECT_E  srv_type    = MN_PHONE_USER_SELECT_NONE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessSSST");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            switch(PARA1.num)
            {
                case 0:
                    srv_type = MN_PHONE_USER_SELECT_NONE;
                    break;
                case 1:
                    srv_type = MN_PHONE_USER_SELECT_GSM_ONLY;
                    break;
                case 3:
                    srv_type = MN_PHONE_USER_SELECT_GSM_GPRS_BOTH;
                    break;
                default:
                    return ERR_OPERATION_NOT_SUPPORTED;
            }

            ret = MNPHONE_SetServiceTypeEx(dual_sys, srv_type);

            if(ERR_MNPHONE_NO_ERR != ret)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SSST].cmd_name->str_ptr, MNPHONE_GetServiceTypeEx(dual_sys));
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

#endif

#ifdef HUAWEI_6500_MODEM
/*****************************************************************************/
//  Description : This function handle the ATC_PPP_ACTIVE_PDP_REQ signal
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessPPPActReq(
    ATC_CONFIG_T            *atc_config_ptr,// point to the ATC_CONFIG_T struct.
    PPP_ACTIVE_PDP_CFG_T *sig_ptr        // point to the ATC_PPP_ACTIVE_PDP_REQ signal
)
{
    ERR_MNGPRS_CODE_E      result = ERR_MNGPRS_NO_ERR;
    uint8                        user[MAX_PAP_USER_LEN + 1]={0};
    uint8                        passwd[MAX_PAP_PASSWD_LEN + 1]={0};
    uint8                       challen[MAX_CHAP_CHALLEN_LEN + 1] = {0};
    uint8                        cha_name[MAX_CHAP_CHALNAME_LEN + 1] = {0};
    uint8                        auth_type  = 0;
    MN_DUAL_SYS_E      dual_sys = ATC_CURRENT_SIM_ID_MUX; 
    uint8                        challen_len = 0;
    uint8                        cha_name_len = 0;

    if((sig_ptr->challenge == NULL) && (sig_ptr->auth_type == MN_CHAP))
    {
        return ERR_GPRS_OPERATION_FAILURE;
    }


    ATC_TRACE_LOW("ATC: ATC_ProcessPPPActReq, username = %s, password = %s, user_len = %d, pw_len = %d", 
		sig_ptr->username, sig_ptr->password, sig_ptr->user_len, sig_ptr->pw_len);
    ATC_TRACE_LOW("ATC: ATC_ProcessPPPActReq, sig_ptr->auth_type = %d", sig_ptr->auth_type);
    ATC_TRACE_LOW("ATC: ATC_ProcessPPPActReq, sig_ptr->pid= %d", sig_ptr->pid);

    if((sig_ptr->user_len <= MAX_PAP_USER_LEN) && (sig_ptr->pw_len <= MAX_PAP_PASSWD_LEN)) 
    {
        strncpy((char *)user, sig_ptr->username, sig_ptr->user_len);
        user[sig_ptr->user_len] = '\0';
        SCI_MEMCPY(passwd, (uint8 *)sig_ptr->password, sig_ptr->pw_len); 
        passwd[sig_ptr->pw_len] = '\0';
    }					
    else
    {
        ATC_TRACE_LOW("ATC: PPP Active user or passwd too long.");
        return ERR_INVALID_INDEX;
    }

    if(sig_ptr->auth_type == MN_CHAP)
    {
        auth_type = MN_CHAP;
    }
    else //auth_type == 0,2 or 3
    {
        auth_type = MN_PAP;
    }

    if(MN_CHAP == auth_type)
    {
        challen_len = sig_ptr->challenge_len;
        cha_name_len = sig_ptr->chall_name_len;
	 SCI_TRACE_LOW("ATC: PPP chap challenge len is %d, name is %d", challen_len, cha_name_len);	
	 
        if((challen_len <=MAX_CHAP_CHALLEN_LEN) && (cha_name_len <= MAX_CHAP_CHALNAME_LEN)) 
        {
            SCI_MEMCPY(challen, (uint32 *)sig_ptr->challenge, challen_len); 
            challen[challen_len] = '\0';
            SCI_MEMCPY(cha_name, (uint8 *)sig_ptr->chall_name, cha_name_len); 
            cha_name[cha_name_len] = '\0';

            SCI_TRACE_LOW("ATC: ATC_ProcessPPPActReq, challen is %s, challengename is %s", challen, cha_name);
        }					
        else
        {
            ATC_TRACE_LOW("ATC: PPP Active user or passwd too long.");
            return ERR_INVALID_INDEX;
        }    
    }

	
    ATC_TRACE_LOW("ATC: ATC_ProcessPPPActReq, ->username = %s, ->password = %s", user, passwd);
    ATC_TRACE_LOW("ATC: ATC_ProcessPPPActReq, ->s_pid_arr[0] = %d", s_pid_arr[0]);
	
    result = MNGPRS_SetPdpContextPcoNewCipherEx(dual_sys, sig_ptr->pid, user, passwd,(MN_PCO_AUTH_TYPE_E)auth_type, 
		                                                                     challen, challen_len, cha_name, cha_name_len);
    
    if(result != ERR_MNGPRS_NO_ERR)
    {
        ATC_TRACE_LOW("ATC: set user or passwd error, result = %d", result);
        return ERR_OPERATION_NOT_SUPPORTED;
    }
    // PDP Active
    if(ERR_MNGPRS_NO_ERR != ATC_ActivatePdpContextEx(atc_config_ptr,dual_sys, FALSE, s_pid_arr, POC_E, MN_UNSPECIFIED))
    //if(ERR_MNGPRS_NO_ERR != MNGPRS_ActivatePdpContextEx(dual_sys, FALSE, s_pid_arr, POC_E, MN_UNSPECIFIED))
    {
        return ERR_GPRS_OPERATION_FAILURE;
    }
    
    ATC_TRACE_LOW("ATC: ATC_ProcessPPPActReq,  %d", (uint8)sig_ptr->pid);

#ifdef _MUX_ENABLE_
    ATC_Link_ATHEventHandle(atc_config_ptr->current_link_id, ATC_ATH_EV_NEW_PDP_SRV, sig_ptr->pid);
    ATC_Create_PSD_Call(sig_ptr->pid, atc_config_ptr->current_link_id, TRUE);
   g_atc_modem_dial_sys = dual_sys;
    ATC_Add_Expected_Event(atc_config_ptr, APP_MN_ACTIVATE_PDP_CONTEXT_CNF,
                     sig_ptr->pid, ATC_DOMAIN_PS, dual_sys);
#else
    ATC_Link_ATHEventHandle(0, ATC_ATH_EV_NEW_PDP_SRV, sig_ptr->pid);
    ATC_Create_PSD_Call(sig_ptr->pid, 0, FALSE);
#endif

    return S_ATC_DEFAULT_HANDLE;
}
#endif

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CGREG
//           The set command controls the presentation of an unsolicited result code
//                                  +CGREG: <stat> when <n>=1 and
//           there is a change in the MT's GPRS network registration status,
//                          or code +CGREG: <stat>[,<lac>,<ci>]
//           when <n>=2 and there is a change of the network cell.
//  Global: atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
/******************************************************************************/
ATC_STATUS ATC_ProcessCGREG(// return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
    ATC_STATUS                      status              = S_ATC_SUCCESS;
#ifdef _SUPPORT_GPRS_
    MN_PHONE_CURRENT_PLMN_INFO_T    register_status;
    OPER_STATUS_E                   gprs_oper_status    = NOT_REGISTERED_NOT_SEARCHING;
    MN_DUAL_SYS_E                   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    ATC_TRACE_LOW("ATC: CGREG original value:%d",  atc_config_ptr->cgreg);

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(cur_cmd_info->param_used[0])
            {
                atc_config_ptr->cgreg = (uint8)PARA1.num;
                ATC_TRACE_LOW("ATC: CGREG set value:%d", atc_config_ptr->cgreg);
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:
            register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
            ATC_TRACE_LOW("ATC: ATC_ProcessCGREG, plmn_status:%d,rat:%d,cell_cap:%d%d%d%d",
            register_status.plmn_status,
            register_status.rat,
            register_status.cell_capability.mbms_support,
            register_status.cell_capability.hsdpa_support,
            register_status.cell_capability.hsupa_support,
            register_status.cell_capability.edge_support);
            ATC_GetOperStatus(dual_sys, atc_config_ptr, &register_status, PNULL, &gprs_oper_status);
            ATC_UpCGREGInfo(dual_sys, atc_config_ptr, &register_status, gprs_oper_status, FALSE);
            break;

        default:
            break;
    }

#endif
    return status;
}

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGEREP
//           Set command enables or disables sending of unsolicited result codes
//  Global: atc_config_ptr
/******************************************************************************/
ATC_STATUS ATC_ProcessCGEREP( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
)
{
#ifndef ATC_SAT_ENABLE
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
#endif

#ifdef _SUPPORT_GPRS_
    ATC_TRACE_LOW("ATC: ATC_ProcessCGEREP");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(cur_cmd_info->param_used[0])
            {
                atc_config_ptr->cgerep_value.mode = (MN_GPRS_EVENT_REPORT_MODE_E)PARA1.num;
            }

            if(cur_cmd_info->param_used[1])
            {
                atc_config_ptr->cgerep_value.bfr = (MN_GPRS_EVENT_REPORT_BUFFER_E)PARA2.num;
            }
#ifndef ATC_SAT_ENABLE			
            HandleCGEREP(dual_sys,atc_config_ptr, atc_config_ptr->cgerep_value.mode, atc_config_ptr->cgerep_value.bfr);
#endif
            ATC_TRACE_LOW("ATC: ATC_ProcessCGEREP: mode=%d,bfr=%d", atc_config_ptr->cgerep_value.mode, atc_config_ptr->cgerep_value.bfr);
            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CGEREP].cmd_name->str_ptr,
            atc_config_ptr->cgerep_value.mode, atc_config_ptr->cgerep_value.bfr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

#endif /* _SUPPORT_GPRS_ */
    return S_ATC_SUCCESS;
}

/******************************************************************************/
//  DESCRIPTION: Spreadtrum AT+SATT attach or detach
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
/******************************************************************************/
ATC_STATUS ATC_ProcessSATT( // return the handle result
    ATC_CONFIG_T          *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T   *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    uint8   state       = 1;
    uint8   ret_state   = 0;
    uint8   action_type = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessSATT");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(cur_cmd_info->param_used[0])
            {
                state = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            if(cur_cmd_info->param_used[1])
            {
                action_type = PARA2.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            if(0 != state)
            {
                if(2 == action_type)
                {
                    return ERR_INVALID_INDEX;
                }

                MNPHONE_GprsAttachEx(dual_sys, (MN_PHONE_GPRS_ATTACH_TYPE_E)action_type);
            }
            else
            {
                if(ERR_MNPHONE_NO_ERR != MNPHONE_GprsDetachEx(dual_sys, (MN_PHONE_GPRS_DETACH_TYPE_E)action_type))
                {
                    ATC_TRACE_LOW("ATC: MNPHONE_GprsDetach failure.");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }

            break;

        case ATC_CMD_TYPE_READ:
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

            if((PLMN_NORMAL_GPRS_ONLY == plmn_info.plmn_status) || (PLMN_NORMAL_GSM_GPRS_BOTH == plmn_info.plmn_status))
            {
                ret_state = 1;
            }
            else
            {
                ret_state = 0;
            }

            sprintf((char *)g_rsp_str, "+SATT: %d", ret_state);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_UNKNOWN;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */
}

#ifndef ATC_SAT_ENABLE
#ifdef _SUPPORT_GPRS_
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
    SCI_ASSERT(NULL != data_ptr);/*assert verified*/
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
/*    Description: set dummy  para for AT+SGPRSDATA  */
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
//  DESCRIPTION: Spreadtrum send gprs for FTA test
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
/******************************************************************************/
ATC_STATUS ATC_ProcessSGPRSDATA( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
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

            SCI_ASSERT((5 <= nsapi) && (nsapi <= 15));/*assert verified*/

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
        
                        SCI_ASSERT(NULL != data_ptr);/*assert verified*/
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
    
                    SCI_ASSERT(NULL != data_ptr);/*assert verified*/
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
//  DESCRIPTION: Spreadtrum AT+SAUTOATT auto attach or not
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : Modified by Hyman.wu
/******************************************************************************/
ATC_STATUS ATC_ProcessSAUTOATT( // return the handle result
    ATC_CONFIG_T          *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T   *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;
    uint8 ret_state ;
    BOOLEAN             action_type = TRUE;
    ERR_MNPHONE_CODE_E  result      = ERR_MNPHONE_NO_ERR;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessSATT");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(cur_cmd_info->param_used[0])
            {
                action_type = (BOOLEAN)PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            result = MNPHONE_SetGprsAttachModeEx(dual_sys, (BOOLEAN)(!action_type));

            if(ERR_MNPHONE_NO_ERR != result)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            break;

        case ATC_CMD_TYPE_READ:
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

            if((PLMN_NORMAL_GPRS_ONLY == plmn_info.plmn_status) || (PLMN_NORMAL_GSM_GPRS_BOTH == plmn_info.plmn_status))
            {
                ret_state = 1;
            }
            else
            {
                ret_state = 0;
            }

            sprintf((char *)g_rsp_str, "+SAUTOATT: %d", ret_state);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_UNKNOWN;
    }

    return S_ATC_SUCCESS;
#else
     return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */
}

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGDSCONT, set secondary pdp context
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Wanghui
//  Note :
/******************************************************************************/
ATC_STATUS ATC_ProcessCGDSCONT( // return the handle result
    ATC_CONFIG_T          *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T   *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_

    ATC_TRACE_LOW("ATC: ATC_ProcessCGDSCONT");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        case ATC_CMD_TYPE_READ:
            return ERR_OPERATION_NOT_SUPPORTED;

            /* Possible response(s):
             * +CGDSCONT: (range of supported <cid>s),
             *      (list of <cid>s for active primary contexts), <PDP_type>,,,
             *      (list of supported <d_comp>s),(list of supported <h_comp>s)
             * [<CR><LF>+CGDSCONT: ...[...]]
             *
             * The test command returns values supported as a compound value.
             * If the MT supports several PDP types, <PDP_type>, the parameter
             * value ranges for each <PDP_type> are returned on a separate line. */
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (1-15),(1-15),\"IP\",,,(0,1),(0,1)", g_atc_info_table[AT_CMD_CGDSCONT].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_SUCCESS;
#endif
}


/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGTFT, set Traffic Flow Template
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Wanghui
//  Note :
/******************************************************************************/
ATC_STATUS ATC_ProcessCGTFT( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_

    ATC_TRACE_LOW("ATC: ATC_ProcessCGDSCONT");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        case ATC_CMD_TYPE_READ:
            return ERR_OPERATION_NOT_SUPPORTED;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

#else
    return S_ATC_SUCCESS;
#endif
}


/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGEQNEG, get negotiated QoS
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Pizer.Fan
//  Note :
/******************************************************************************/
ATC_STATUS ATC_ProcessCGEQNEG( // return the handle result
    ATC_CONFIG_T           *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T    *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    ATC_STATUS                  at_status   = S_ATC_SUCCESS;
#ifdef _GPRS_FULL_FUNCTION_
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32                      i           = 0;
    ATC_CUR_PARAM_INFO_LIST_T   *param_ptr  = PNULL;
    ATC_NEGOTIATED_QOS_REQ_T    neg_cids;
#endif

    ATC_TRACE_LOW("ATC: ATC_ProcessCGEQNEG");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

#ifdef _GPRS_FULL_FUNCTION_
    SCI_MEMSET(&neg_cids, 0, sizeof(neg_cids));
#endif

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
#ifdef _GPRS_FULL_FUNCTION_
            param_ptr   = cur_cmd_info->cur_param_list_ptr;

            for(i = 0; (param_ptr != PNULL) && (i < MN_GPRS_MAX_PDP_CONTEXT_COUNT); i++)
            {
                if(cur_cmd_info->param_used[i])
                {
                    neg_cids.pdp_id[i] = param_ptr->param_info.num;
                    neg_cids.num++;
                }
            }

            if(0 == neg_cids.num)
            {
                neg_cids.is_all = TRUE;
            }
            else
            {
                neg_cids.is_all = FALSE;
            }

            at_status = ReadQosProfile_R99(dual_sys, atc_config_ptr, FALSE, TRUE, &neg_cids);
#endif
            break;

            /* Possible Response(s):
             * +CGEQNEG: (list of <cid>s associated with active contexts) */
        case ATC_CMD_TYPE_TEST:
#ifdef _GPRS_FULL_FUNCTION_
            neg_cids.is_all = TRUE;
            at_status = ReadQosProfile_R99(dual_sys, atc_config_ptr, FALSE, TRUE, &neg_cids);
#endif
            break;

        case ATC_CMD_TYPE_READ:
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return (at_status);

#else
    return S_ATC_SUCCESS;
#endif
}


#endif

/*****************************************************************************/
//  Description : set 3G QoS profile
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS SetQosProfile_R99(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info,   //IN:
    BOOLEAN             is_minqos        //IN:
)
{

    MNGPRS_3G_EXTEND_QOS_SET_T  mn_info         = {0};
    BOOLEAN                     item_setted[13] = {0};    
    ERR_MNGPRS_CODE_E           ret             = ERR_MNGPRS_NO_ERR;
    ATC_STATUS                  at_result       = S_ATC_DEFAULT_HANDLE;
    MN_GPRS_TOTAL_QOS_T         new_qos;


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
    if ((PARAM2_FLAG == FALSE)&&
        (PARAM3_FLAG == FALSE)&&
        (PARAM4_FLAG == FALSE)&&
        (PARAM5_FLAG == FALSE)&&
        (PARAM6_FLAG == FALSE)&&
        (PARAM7_FLAG == FALSE)&&
        (PARAM8_FLAG == FALSE)&&
        (PARAM9_FLAG == FALSE)&&
        (PARAM10_FLAG == FALSE)&&
        (PARAM11_FLAG == FALSE)&&
        (PARAM12_FLAG == FALSE)&&
        (PARAM13_FLAG == FALSE))
    {
        //segam return error!so we do!
        ATC_TRACE( "ATGPRS:ATC_ProcessCGEQREQ param error!\n" );
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
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,APP_MN_SET_MIN_QOS_CNF,mn_info.pdp_id,ATC_DOMAIN_PS, dual_sys);
            }
        }
        else
        {
            ret= MNGPRS_SetExtendQosEx(dual_sys, mn_info.pdp_id, new_qos, DOWNLOAD_E);
            if (ret==ERR_MNGPRS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,APP_MN_SET_QOS_CNF,mn_info.pdp_id,ATC_DOMAIN_PS, dual_sys);   
            }
        }
        if (ret==ERR_MNGPRS_NO_ERR)
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

#if defined _GPRS_FULL_FUNCTION_ || defined _SUPPORT_IPC_
/*****************************************************************************/
//  Description : read 3G QoS profile
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL void ConvertTotalQos2ExtQos(
    MNGPRS_3G_EXTEND_QOS_SET_T *ext_qos_ptr,   //OUT:
    const MN_GPRS_TOTAL_QOS_T  *total_qos_ptr  //IN:
)
{
    switch(total_qos_ptr->traffic_class)
    {
        case MN_GPRS_TRAFFIC_CLASS_CONVERSATIONAL:
            ext_qos_ptr->traffic_class = 0;
            break;
        case MN_GPRS_TRAFFIC_CLASS_STREAMING:
            ext_qos_ptr->traffic_class = 1;
            break;
        case MN_GPRS_TRAFFIC_CLASS_INTERACTIVE:
            ext_qos_ptr->traffic_class = 2;
            break;
        case MN_GPRS_TRAFFIC_CLASS_BACKGROUD:
            ext_qos_ptr->traffic_class = 3;
            break;
        case MN_GPRS_TRAFFIC_CLASS_SUBSCRIBED:
        default:
            ext_qos_ptr->traffic_class = 4;
            break;
    }

    if(MN_GPRS_MAX_BIT_RATE_UPLINK7 == total_qos_ptr->max_bit_rate_for_uplink)
    {
        ext_qos_ptr->max_bt_ul = 0;
    }
    else if(MN_GPRS_MAX_BIT_RATE_UPLINK_SUBSCRIBED == total_qos_ptr->max_bit_rate_for_uplink)
    {
        ext_qos_ptr->max_bt_ul = 0XFFFF; //invalid
    }
    else if((MN_GPRS_MAX_BIT_RATE_UPLINK1 <= total_qos_ptr->max_bit_rate_for_uplink)
    && (total_qos_ptr->max_bit_rate_for_uplink <= MN_GPRS_MAX_BIT_RATE_UPLINK2))
    {
        ext_qos_ptr->max_bt_ul = total_qos_ptr->max_bit_rate_for_uplink;
    }
    else if(total_qos_ptr->max_bit_rate_for_uplink <= MN_GPRS_MAX_BIT_RATE_UPLINK4)
    {
        ext_qos_ptr->max_bt_ul = (total_qos_ptr->max_bit_rate_for_uplink - 0x40) * 8 + 64;
    }
    else if(total_qos_ptr->max_bit_rate_for_uplink <= MN_GPRS_MAX_BIT_RATE_UPLINK6)
    {
        ext_qos_ptr->max_bt_ul = (total_qos_ptr->max_bit_rate_for_uplink - 0x80) * 64 + 576;
    }
    else
    {
        ext_qos_ptr->max_bt_ul = 0XFFFF; //invalid
    }

    if(MN_GPRS_MAX_BIT_RATE_DOWNLINK_SUBSCRIBED == total_qos_ptr->max_bit_rate_for_downlink)
    {
        // If the parameter is set to '0' the subscribed value will be requested.
        ext_qos_ptr->max_bt_dl = 0;
    }
    else if((MN_GPRS_MAX_BIT_RATE_DOWNLINK1 <= total_qos_ptr->max_bit_rate_for_downlink)
    && (total_qos_ptr->max_bit_rate_for_downlink <= MN_GPRS_MAX_BIT_RATE_DOWNLINK2))
    {
        ext_qos_ptr->max_bt_dl = total_qos_ptr->max_bit_rate_for_downlink;
    }
    else if(total_qos_ptr->max_bit_rate_for_downlink <= MN_GPRS_MAX_BIT_RATE_DOWNLINK4)
    {
        ext_qos_ptr->max_bt_dl = (total_qos_ptr->max_bit_rate_for_downlink - 0x40) * 8 + 64;
    }
    else if(total_qos_ptr->max_bit_rate_for_downlink <= MN_GPRS_MAX_BIT_RATE_DOWNLINK6)
    {
        ext_qos_ptr->max_bt_dl = (total_qos_ptr->max_bit_rate_for_downlink - 0x80) * 64 + 576;
    }
    else
    {
        ext_qos_ptr->max_bt_dl = 0XFFFF; //invalid
    }

    if(MN_GPRS_GUARANTD_BIT_RATE_UPLINK_SUBSCRIBED == total_qos_ptr->guaranteed_bit_rate_for_uplink)
    {
        // If the parameter is set to '0' the subscribed value will be requested.
        ext_qos_ptr->guaranteed_bt_ul = 0;
    }
    else if((MN_GPRS_GUARANTD_BIT_RATE_UPLINK1 <= total_qos_ptr->guaranteed_bit_rate_for_uplink)
    && (total_qos_ptr->guaranteed_bit_rate_for_uplink <= MN_GPRS_GUARANTD_BIT_RATE_UPLINK2))
    {
        ext_qos_ptr->guaranteed_bt_ul = total_qos_ptr->guaranteed_bit_rate_for_uplink;
    }
    else if(total_qos_ptr->guaranteed_bit_rate_for_uplink <= MN_GPRS_GUARANTD_BIT_RATE_UPLINK4)
    {
        ext_qos_ptr->guaranteed_bt_ul = (total_qos_ptr->guaranteed_bit_rate_for_uplink - 0x40) * 8 + 64;
    }
    else if(total_qos_ptr->guaranteed_bit_rate_for_uplink <= MN_GPRS_GUARANTD_BIT_RATE_UPLINK6)
    {
        ext_qos_ptr->guaranteed_bt_ul = (total_qos_ptr->guaranteed_bit_rate_for_uplink - 0x80) * 64 + 576;
    }
    else
    {
        ext_qos_ptr->guaranteed_bt_ul = 0XFFFF; //invalid
    }

    if(MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK_SUBSCRIBED == total_qos_ptr->guaranteed_bit_rate_for_downlink)
    {
        // If the parameter is set to '0' the subscribed value will be requested.
        ext_qos_ptr->guaranteed_bt_dl = 0;
    }
    else if((MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK1 <= total_qos_ptr->guaranteed_bit_rate_for_downlink)
    && (total_qos_ptr->guaranteed_bit_rate_for_downlink <= MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK2))
    {
        ext_qos_ptr->guaranteed_bt_dl = total_qos_ptr->guaranteed_bit_rate_for_downlink;
    }
    else if(total_qos_ptr->guaranteed_bit_rate_for_downlink <= MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK4)
    {
        ext_qos_ptr->guaranteed_bt_dl = (total_qos_ptr->guaranteed_bit_rate_for_downlink - 0x40) * 8 + 64;
    }
    else if(total_qos_ptr->guaranteed_bit_rate_for_downlink <= MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK6)
    {
        ext_qos_ptr->guaranteed_bt_dl = (total_qos_ptr->guaranteed_bit_rate_for_downlink - 0x80) * 64 + 576;
    }
    else
    {
        ext_qos_ptr->guaranteed_bt_dl = 0XFFFF; //invalid
    }

    switch(total_qos_ptr->delivery_order)
    {
        case MN_GPRS_DELIVERY_ORDER_SUBSCRIBED:
            ext_qos_ptr->delivery_order = 2;
            break;
        case MN_GPRS_DELIVERY_ORDER_NO:
            ext_qos_ptr->delivery_order = 0;
            break;
        case MN_GPRS_DELIVERY_ORDER_YES:
            ext_qos_ptr->delivery_order = 1;
            break;
        default:
            ext_qos_ptr->delivery_order = 0XFF;
            break;
    }

    if(MN_GPRS_MAX_SDU_SIZE_SUBSCRIBED == total_qos_ptr->max_sdu_size)
    {
        ext_qos_ptr->max_sdu_size = 0;
    }
    else if(total_qos_ptr->max_sdu_size <= MN_GPRS_MAX_SDU_SIZE_1500OCTETS)
    {
        ext_qos_ptr->max_sdu_size = total_qos_ptr->max_sdu_size * 10;
    }
    else if(MN_GPRS_MAX_SDU_SIZE_1502OCTETS == total_qos_ptr->max_sdu_size)
    {
        ext_qos_ptr->max_sdu_size = 1502;
    }
    else if(MN_GPRS_MAX_SDU_SIZE_1510OCTETS == total_qos_ptr->max_sdu_size)
    {
        ext_qos_ptr->max_sdu_size = 1510;
    }
    else if(MN_GPRS_MAX_SDU_SIZE_1520OCTETS == total_qos_ptr->max_sdu_size)
    {
        ext_qos_ptr->max_sdu_size = 1520;
    }
    else
    {
        ext_qos_ptr->max_sdu_size = 0XFFFF;
    }

    switch(total_qos_ptr->sdu_err_ratio)
    {
        case MN_GPRS_SDU_ERR_RATIO_SUBSCRIBED:
            ext_qos_ptr->sdu_error_ratio[0] = 0;
            ext_qos_ptr->sdu_error_ratio[1] = 0;
            break;
        case MN_GPRS_SDU_ERR_RATIO1:
            ext_qos_ptr->sdu_error_ratio[0] = 1;
            ext_qos_ptr->sdu_error_ratio[1] = 2;
            break;
        case MN_GPRS_SDU_ERR_RATIO2:
            ext_qos_ptr->sdu_error_ratio[0] = 7;
            ext_qos_ptr->sdu_error_ratio[1] = 3;
            break;
        case MN_GPRS_SDU_ERR_RATIO3:
            ext_qos_ptr->sdu_error_ratio[0] = 1;
            ext_qos_ptr->sdu_error_ratio[1] = 3;
            break;
        case MN_GPRS_SDU_ERR_RATIO4:
            ext_qos_ptr->sdu_error_ratio[0] = 1;
            ext_qos_ptr->sdu_error_ratio[1] = 4;
            break;
        case MN_GPRS_SDU_ERR_RATIO5:
            ext_qos_ptr->sdu_error_ratio[0] = 1;
            ext_qos_ptr->sdu_error_ratio[1] = 5;
            break;
        case MN_GPRS_SDU_ERR_RATIO6:
            ext_qos_ptr->sdu_error_ratio[0] = 1;
            ext_qos_ptr->sdu_error_ratio[1] = 6;
            break;
        case MN_GPRS_SDU_ERR_RATIO7:
            ext_qos_ptr->sdu_error_ratio[0] = 1;
            ext_qos_ptr->sdu_error_ratio[1] = 7;
            break;
        default:
            ext_qos_ptr->sdu_error_ratio[0] = 0xff;
            ext_qos_ptr->sdu_error_ratio[1] = 0xff;
            break;
    }

    switch(total_qos_ptr->residual_ber)
    {
        case MN_GPRS_RESIDUAL_BER_SUBSCRIBED:
            ext_qos_ptr->residual_ber[0] = 0;
            ext_qos_ptr->residual_ber[1] = 0;
            break;
        case MN_GPRS_BIT_ERROR_RATE1:
            ext_qos_ptr->residual_ber[0] = 5;
            ext_qos_ptr->residual_ber[1] = 2;
            break;
        case MN_GPRS_BIT_ERROR_RATE2:
            ext_qos_ptr->residual_ber[0] = 1;
            ext_qos_ptr->residual_ber[1] = 2;
            break;
        case MN_GPRS_BIT_ERROR_RATE3:
            ext_qos_ptr->residual_ber[0] = 5;
            ext_qos_ptr->residual_ber[1] = 3;
            break;
        case MN_GPRS_BIT_ERROR_RATE4:
            ext_qos_ptr->residual_ber[0] = 4;
            ext_qos_ptr->residual_ber[1] = 3;
            break;
        case MN_GPRS_BIT_ERROR_RATE5:
            ext_qos_ptr->residual_ber[0] = 1;
            ext_qos_ptr->residual_ber[1] = 3;
            break;
        case MN_GPRS_BIT_ERROR_RATE6:
            ext_qos_ptr->residual_ber[0] = 1;
            ext_qos_ptr->residual_ber[1] = 4;
            break;
        case MN_GPRS_BIT_ERROR_RATE7:
            ext_qos_ptr->residual_ber[0] = 1;
            ext_qos_ptr->residual_ber[1] = 5;
            break;
        case MN_GPRS_BIT_ERROR_RATE8:
            ext_qos_ptr->residual_ber[0] = 1;
            ext_qos_ptr->residual_ber[1] = 6;
            break;
        case MN_GPRS_BIT_ERROR_RATE9:
            ext_qos_ptr->residual_ber[0] = 6;
            ext_qos_ptr->residual_ber[1] = 8;
            break;
        default:
            ext_qos_ptr->residual_ber[0] = 0xff;
            ext_qos_ptr->residual_ber[1] = 0xff;
            break;
    }

    switch(total_qos_ptr->delivery_of_err_sdu)
    {
        case MN_GPRS_DELIVERY_OF_ERR_SDU_SUBSCRIBED:
            ext_qos_ptr->delivery_of_err_sdu = 3;
            break;
        case MN_GPRS_DELIVERY_OF_ERR_SDU_NODETECT:
            ext_qos_ptr->delivery_of_err_sdu = 2;
            break;
        case MN_GPRS_DELIVERY_OF_ERR_SDU_YES:
            ext_qos_ptr->delivery_of_err_sdu = 1;
            break;
        case MN_GPRS_DELIVERY_OF_ERR_SDU_NO:
            ext_qos_ptr->delivery_of_err_sdu = 0;
            break;
        default:
            ext_qos_ptr->delivery_of_err_sdu = 0xff;
            break;
    }

    if(MN_GPRS_TRANSFER_DELAY_SUBSCRIBED == total_qos_ptr->transfer_delay)
    {
        // If the parameter is set to '0' the subscribed value will be requested
        ext_qos_ptr->transfer_delay = 0;
    }
    else if(total_qos_ptr->transfer_delay <= MN_GPRS_TRANSFER_DELAY2)
    {
        ext_qos_ptr->transfer_delay = total_qos_ptr->transfer_delay * 10;
    }
    else if(total_qos_ptr->transfer_delay <= MN_GPRS_TRANSFER_DELAY4)
    {
        ext_qos_ptr->transfer_delay = (total_qos_ptr->transfer_delay - 0x10) * 50 + 200;
    }
    else if(total_qos_ptr->transfer_delay <= MN_GPRS_TRANSFER_DELAY6)
    {
        ext_qos_ptr->transfer_delay = (total_qos_ptr->transfer_delay - 0x20) * 100 + 1000;
    }
    else
    {
        ext_qos_ptr->transfer_delay = 0xff;
    }

    ext_qos_ptr->traffic_handling_priority = total_qos_ptr->traffic_handling_prio;
}

/*****************************************************************************/
//  Description : judge the cid is valid when reading 3G QoS profile
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValidPidInReadQos(
    MN_DUAL_SYS_E dual_sys,
    uint32                    cur_cid,        //IN:
    BOOLEAN                   is_negotiated,  //IN:
    ATC_NEGOTIATED_QOS_REQ_T  *neg_req_ptr    //IN:
)
{
    uint8               i                   = 0;
    BOOLEAN             is_valid            = TRUE;
    MN_GPRS_CON_STATE_T *pdp_con_state_arr  = PNULL;
    uint32              pdp_con_num         = 0;

    if(is_negotiated)
    {
        is_valid = FALSE;

        if(neg_req_ptr->is_all)
        {
            if(ERR_MNGPRS_NO_ERR == MNGPRS_ReadPdpContextStateEx(dual_sys, &pdp_con_state_arr, &pdp_con_num))
            {
                for(i = 0; i < pdp_con_num; i++)
                {
                    if((cur_cid == pdp_con_state_arr[i].pdp_id)
                    && (MN_CONTEXT_ACTIVATED == pdp_con_state_arr[i].pdp_state))
                    {
                        is_valid = TRUE;
                        break;
                    }
                }
            }
            if(PNULL != pdp_con_state_arr)
            {
                    SCI_FREE(pdp_con_state_arr);
            }
        }
        else
        {
            for(i = 0; i < neg_req_ptr->num; i++)
            {
                if(neg_req_ptr->pdp_id[i] == cur_cid)
                {
                    is_valid = TRUE;
                    break;
                }
            }
        }
    }

    return (is_valid);
}

/*****************************************************************************/
//  Description : read 3G QoS profile
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ReadQosProfile_R99(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T                *atc_config_ptr, //OUT:
    BOOLEAN                     is_minqos,       //IN:
    BOOLEAN                     is_negotiated,   //IN:
    ATC_NEGOTIATED_QOS_REQ_T    *neg_req_ptr     //IN:
)
{
    ATC_STATUS                  at_status    = S_ATC_SUCCESS;
    ERR_MNGPRS_CODE_E           mn_result    = ERR_MNGPRS_NO_ERR;
    MNGPRS_3G_EXTEND_QOS_SET_T  ext_qos;
    MN_GPRS_OUT_QOS_T           *cur_qos_arr = PNULL;
    uint32                      pdp_con_num  = 0;
    int32                       str_len      = 0;
    uint8                       i            = 0;
    uint8                       line_count   = 0;
    char                        gprs_cmd_str[10];

    if(is_negotiated)
    {
        mn_result   = MNGPRS_ReadNegQosProfileEx(dual_sys, &cur_qos_arr, &pdp_con_num);
        sprintf(gprs_cmd_str, "+CGEQNEG:");
    }
    else
    {
        if(is_minqos)
        {
            mn_result   = MNGPRS_ReadMinQosProfileEx(dual_sys, &cur_qos_arr, &pdp_con_num);
            sprintf(gprs_cmd_str, "+CGEQMIN:");
        }
        else
        {
            mn_result   = MNGPRS_ReadCurrQosProfileEx(dual_sys, &cur_qos_arr, &pdp_con_num);
            sprintf(gprs_cmd_str, "+CGEQREQ:");
        }
    }

    if(ERR_MNGPRS_NO_ERR == mn_result)
    {
        for(i = 0; i < pdp_con_num; i++)
        {
            if(IsValidPidInReadQos(dual_sys, cur_qos_arr[i].pdp_id, is_negotiated, neg_req_ptr))
            {
                ConvertTotalQos2ExtQos(&ext_qos, &(cur_qos_arr[i].qos));
                ext_qos.pdp_id = cur_qos_arr[i].pdp_id;

                if(0 == line_count)
                {
                    str_len += sprintf((char *)g_rsp_str + str_len, "%s %d,%d,%d,%d,%d,%d,%d,%ld,\"%dE%d\",\"%dE%d\",%d,%d,%d",
                    gprs_cmd_str,
                    ext_qos.pdp_id,
                    ext_qos.traffic_class,
                    ext_qos.max_bt_ul,
                    ext_qos.max_bt_dl,
                    ext_qos.guaranteed_bt_ul,
                    ext_qos.guaranteed_bt_dl,
                    ext_qos.delivery_order,
                    ext_qos.max_sdu_size,
                    ext_qos.sdu_error_ratio[0],
                    ext_qos.sdu_error_ratio[1],
                    ext_qos.residual_ber[0],
                    ext_qos.residual_ber[1],
                    ext_qos.delivery_of_err_sdu,
                    ext_qos.transfer_delay,
                    ext_qos.traffic_handling_priority);
                }
                else
                {
                    str_len += sprintf((char *)g_rsp_str + str_len, "\r\n%s %d,%d,%d,%d,%d,%d,%d,%ld,\"%dE%d\",\"%dE%d\",%d,%d,%d",
                    gprs_cmd_str,
                    ext_qos.pdp_id,
                    ext_qos.traffic_class,
                    ext_qos.max_bt_ul,
                    ext_qos.max_bt_dl,
                    ext_qos.guaranteed_bt_ul,
                    ext_qos.guaranteed_bt_dl,
                    ext_qos.delivery_order,
                    ext_qos.max_sdu_size,
                    ext_qos.sdu_error_ratio[0],
                    ext_qos.sdu_error_ratio[1],
                    ext_qos.residual_ber[0],
                    ext_qos.residual_ber[1],
                    ext_qos.delivery_of_err_sdu,
                    ext_qos.transfer_delay,
                    ext_qos.traffic_handling_priority);
                }

                line_count++;
            }
        }

        if(0 < line_count)
        {
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }

        at_status    = S_ATC_SUCCESS;
    }
    else
    {
        at_status = ERR_OPERATION_NOT_SUPPORTED;
    }

    if(cur_qos_arr != PNULL)
    {
        SCI_FREE(cur_qos_arr);
    }

    return (at_status);
}
#endif

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGEQREQ, set 3G Qos
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Wanghui
//  Note :
/******************************************************************************/
ATC_STATUS ATC_ProcessCGEQREQ( // return the handle result
    ATC_CONFIG_T           *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T    *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    ATC_STATUS  at_status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    ATC_TRACE_LOW("ATC: ATC_ProcessCGEQREQ");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

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
            at_status = SetQosProfile_R99(dual_sys, atc_config_ptr, cur_cmd_info, FALSE);
        }
        break;

        case ATC_CMD_TYPE_READ:
#if defined _GPRS_FULL_FUNCTION_ || defined _SUPPORT_IPC_
            at_status = ReadQosProfile_R99(dual_sys, atc_config_ptr, FALSE, FALSE, PNULL);
#endif
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return (at_status);
#else
    return S_ATC_SUCCESS;
#endif /* _SUPPORT_GPRS_ */
}

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGEQMIN, set 3G min Qos
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Wanghui
//  Note :
/******************************************************************************/
ATC_STATUS ATC_ProcessCGEQMIN( // return the handle result
    ATC_CONFIG_T           *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T    *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    ATC_STATUS  at_status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessCGEQMIN");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            at_status = SetQosProfile_R99(dual_sys, atc_config_ptr, cur_cmd_info, TRUE);
            break;

        case ATC_CMD_TYPE_READ:
#ifdef _GPRS_FULL_FUNCTION_
            at_status = ReadQosProfile_R99(dual_sys, atc_config_ptr, TRUE, FALSE, PNULL);
#endif
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return (at_status);

#else
    return S_ATC_SUCCESS;
#endif
}

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+PSRABR, set Traffic Flow Template
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Wanghui
//  Note :
/******************************************************************************/
ATC_STATUS ATC_ProcessPSRABR( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
)
{
    return S_ATC_FAIL;
}
#ifdef _SUPPORT_GPRS_
/******************************************************************************/
//  DESCRIPTION: This function handles consequence event of HandleCGEREP
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Pizer.Fan
//  Note :
/******************************************************************************/
void ATC_BuildGprsEvReportRsp(
    ATC_CONFIG_T     *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    xSignalHeaderRec *pSig
)
{
    uint8      cr_val       = 0;                 // holds the atc_config_ptr->S3_reg value
    uint8      lf_val       = 0;                 // holds the atc_config_ptr->S4_reg value
    uint8      v_val        = 0;                 // holds the atc_config_ptr->V_option value
    uint8      rsp_buf[10];
    uint8      i               = 0;
	
    MN_GPRS_PDP_ADDR_T  *pdp_addr_arr = PNULL;	
    ERR_MNGPRS_CODE_E           ret         = ERR_MNGPRS_NO_ERR;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
	
#ifdef _MUX_ENABLE_
    uint8      tmp_link_id = 0xff;
#endif

    ATC_TRACE_LOW("ATC: ATC_BuildGprsEvReportRsp");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != pSig);/*assert verified: check null pointer*/

#ifdef _MUX_ENABLE_
    tmp_link_id = atc_config_ptr->current_link_id;
#endif


    // Set the config values
    cr_val = atc_config_ptr->s3_reg;
    lf_val = atc_config_ptr->s4_reg;
    v_val  = atc_config_ptr->v_value;

    if(atc_config_ptr->q_value)
    {
        return;
    }

    if((atc_config_ptr->cgerep_value.mode != MN_GPRS_EVENT_REPORT_MODE_2
	&& atc_config_ptr->cgerep_value.mode != MN_GPRS_EVENT_REPORT_MODE_3)
	&& atc_config_ptr->cgerep_value.bfr !=MN_GPRS_EVENT_REPORT_BUFFER_2) //CID:31189
    {
        return;
    }
    if(atc_config_ptr->v_value)
    {
        sprintf((char *)rsp_buf, "%c%c", cr_val, lf_val);
        ATC_SendReslutRsp(atc_config_ptr, rsp_buf, strlen((char*)rsp_buf));
    }
			
    switch(pSig->SignalCode)
    {
        case APP_MN_DEACTIVATE_PDP_CONTEXT_IND:
        {
            APP_MN_GPRS_DEACTIVATE_IND_T sig_info = {0};
            uint32  pdp_id[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
            
            sig_info.dual_sys =  ATC_CURRENT_SIM_ID_MUX;
            sig_info.result = ((APP_MN_GPRS_DEACTIVATE_IND_T *)pSig)->result;
            sig_info.pdp_num = 1;
            for(i=0;i<MN_GPRS_MAX_PDP_CONTEXT_COUNT;i++)
            {
                sig_info.pdp_id[i] = ((APP_MN_GPRS_DEACTIVATE_IND_T *)pSig)->pdp_id[i];
                pdp_id[i] = (uint32)sig_info.pdp_id;
            }
            	
            ret = MNGPRS_ReadPdpContextAddrEx(dual_sys, pdp_id, &pdp_addr_arr);
            if(ERR_MNGPRS_NO_ERR == ret)
            {
                if(pdp_addr_arr->length == 0)
                {
                    if(sig_info.result == MN_GPRS_ERR_RETRYING)
                    {
                         sprintf((char *)g_rsp_str, "+CGEV: NW REACT \"IP\",\"0.0.0.0\",%d",
                                    ((APP_MN_GPRS_DEACTIVATE_IND_T *)pSig)->pdp_id[0]);                       	
                    }
			        else if(sig_info.result == MN_GPRS_ERR_SUCCESS)
			        {
                         sprintf((char *)g_rsp_str, "+CGEV: NW DEACT \"IP\",\"0.0.0.0\",%d",
                                    ((APP_MN_GPRS_DEACTIVATE_IND_T *)pSig)->pdp_id[0]);  			
			        }
                }
                else
                {
                    if(sig_info.result == MN_GPRS_ERR_RETRYING)
                    {
                         sprintf((char *)g_rsp_str, "+CGEV: NW REACT \"IP\",\"");                    
                    }
                    else if(sig_info.result == MN_GPRS_ERR_SUCCESS)
                    {
                        sprintf((char *)g_rsp_str, "+CGEV: NW DEACT \"IP\",\"");  		      
                    }
                    
                    for(i = 0; i < pdp_addr_arr->length - 1; i++)
                    {
                         sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d.", pdp_addr_arr->value_arr[i]);
                    }
                     sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "\"%d\",%d", pdp_addr_arr->value_arr[i],sig_info.pdp_id[0]);
                }	                
            }
            else
            {
                if(pdp_addr_arr != PNULL)
                {
                    SCI_FREE(pdp_addr_arr);
                }
                return;
            }
	 }
        break;	 
			
        case APP_MN_DEACTIVATE_PDP_CONTEXT_CNF:
        {
            APP_MN_GPRS_DEACTIVATE_CNF_T    sig_info = {0};
            uint32  pdp_id[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};

            sig_info.dual_sys =  ATC_CURRENT_SIM_ID_MUX;
            sig_info.result = ((APP_MN_GPRS_DEACTIVATE_CNF_T *)pSig)->result;
            sig_info.pdp_num = 1;
            for(i=0;i<MN_GPRS_MAX_PDP_CONTEXT_COUNT;i++)
            {
                sig_info.pdp_id[i] = ((APP_MN_GPRS_DEACTIVATE_CNF_T *)pSig)->pdp_id[i];
                pdp_id[i] = (uint32)sig_info.pdp_id;
            }

           ret = MNGPRS_ReadPdpContextAddrEx(dual_sys, pdp_id, &pdp_addr_arr);
            if(ERR_MNGPRS_NO_ERR == ret)
            {
                if(pdp_addr_arr->length == 0)
                {
                    if(sig_info.result == MN_GPRS_ERR_RETRYING)
                    {
                         sprintf((char *)g_rsp_str, "+CGEV: NW REACT \"IP\",\"0.0.0.0\",%d",
                                    ((APP_MN_GPRS_DEACTIVATE_CNF_T *)pSig)->pdp_id[0]);                       	
                    }
                    else if(sig_info.result == MN_GPRS_ERR_SUCCESS)
                    {
                         sprintf((char *)g_rsp_str, "+CGEV: MT DEACT \"IP\",\"0.0.0.0\",%d",
                                         ((APP_MN_GPRS_DEACTIVATE_CNF_T *)pSig)->pdp_id[0]);  			
                    }
                }
                else
                {
                    if(sig_info.result == MN_GPRS_ERR_RETRYING)
                    {
                         sprintf((char *)g_rsp_str, "+CGEV: NW REACT \"IP\",\"");                    
                    }
                    else if(sig_info.result == MN_GPRS_ERR_SUCCESS)
                    {
                         sprintf((char *)g_rsp_str, "+CGEV: MT DEACT \"IP\",\"");  		      
                    }
        
                    for(i = 0; i < pdp_addr_arr->length - 1; i++)
                    {
                         sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d.", pdp_addr_arr->value_arr[i]);
                    }
                     sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d\",%d", pdp_addr_arr->value_arr[i],sig_info.pdp_id[0]);
                }	                
            }
            else
            {
                if(pdp_addr_arr != PNULL)
                {
                    SCI_FREE(pdp_addr_arr);
                }
                return;
            }
        }
        break;

        case APP_MN_ACTIVATE_PDP_CONTEXT_CNF:
        {
            APP_MN_GPRS_EXT_T    sig_info = {0};
            uint32  pdp_id[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
			
            sig_info.dual_sys =  ATC_CURRENT_SIM_ID_MUX;
            sig_info.result = ((APP_MN_GPRS_EXT_T *)pSig)->result;
			
            for(i=0;i<MN_GPRS_MAX_PDP_CONTEXT_COUNT;i++)
            {
                sig_info.pdp_id =  ((APP_MN_GPRS_EXT_T *)pSig)->pdp_id;
                pdp_id[i] = (uint32)sig_info.pdp_id;
            }
            
            ret = MNGPRS_ReadPdpContextAddrEx(dual_sys, pdp_id, &pdp_addr_arr);
            if(ERR_MNGPRS_NO_ERR == ret)
            {
                if(pdp_addr_arr->length == 0)
                {
                    if(sig_info.result == MN_GPRS_ERR_UNKNOWN_ERROR)
                    {
                        sprintf((char *)g_rsp_str, "+CGEV: NW REJECT \"IP\",\"0.0.0.0\"");                       	
                    }
                    else if(sig_info.result == MN_GPRS_ERR_RETRYING)
                    {
                        sprintf((char *)g_rsp_str, "+CGEV: NW REACT \"IP\",\"0.0.0.0\",%d",
                            ((APP_MN_GPRS_EXT_T *)pSig)->pdp_id);  			
                    }
                }
                else
                {
                    if(sig_info.result == MN_GPRS_ERR_UNKNOWN_ERROR)
                    {
                         sprintf((char *)g_rsp_str, "+CGEV: NW REJECT \"IP\""); 
                        for(i = 0; i < pdp_addr_arr->length - 1; i++)
                        {
                             sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d.", pdp_addr_arr->value_arr[i]);
                        }
                         sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d", pdp_addr_arr->value_arr[i]);
                    }
                    else if(sig_info.result == MN_GPRS_ERR_RETRYING)
                    {
                        sprintf((char *)g_rsp_str, "+CGEV: NW REACT\"IP\", \"");
                        for(i = 0; i < pdp_addr_arr->length - 1; i++)
                        {
                             sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d.", pdp_addr_arr->value_arr[i]);
                        }
                         sprintf((char *)g_rsp_str + strlen((char *)g_rsp_str), "%d\",%d", pdp_addr_arr->value_arr[i],sig_info.pdp_id);
                    }
                }	                
            }
            else
            {
                if(pdp_addr_arr != PNULL)
                {
                    SCI_FREE(pdp_addr_arr);
                }
                return;
            }
		 
        }
		break;
		
        case  APP_MN_GPRS_DETACH_IND:
            sprintf((char *)g_rsp_str, "+CGEV: NW DETACH ");   
        break;

        case APP_MN_GPRS_DETACH_RESULT:
            sprintf((char *)g_rsp_str, "+CGEV: MT DETACH ");
        break;
	
        case APP_MN_ACTIVATE_PDP_CONTEXT_IND:
	    if(atc_config_ptr->crc)
	    {
                sprintf((char *)g_rsp_str, "+CRING: GPRS ");  	    
	    }
	break;
			
        default:
            sprintf((char *)g_rsp_str, "+CGEV: ");
    }
    
    ATC_LINK_ID_TRACE("ATC: ATC_BuildGprsEvReportRsp, current_link_id is %d", atc_config_ptr->current_link_id);
    ATC_BuildUnsolicitedInfo(dual_sys,atc_config_ptr, g_rsp_str);
	
    if(pdp_addr_arr != PNULL )
    {
        SCI_FREE(pdp_addr_arr);
    }
	
#ifdef _MUX_ENABLE_
    atc_config_ptr->current_link_id = tmp_link_id;
#endif

}


/******************************************************************************/
//  DESCRIPTION: This function handles APP_MN_DEACTIVATE_PDP_CONTEXT_IND
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  minqian.qian
//  Note :
/******************************************************************************/
void ATC_ProcessDeactivatePDPContextIND(
    ATC_CONFIG_T                    *atc_config_ptr,            // point to the ATC_CONFIG_T struct.
    APP_MN_GPRS_DEACTIVATE_IND_T    *sig_ptr
)
{
    int8      i   = 0;
    uint8     cid = 0;

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != sig_ptr);/*assert verified: check null pointer*/

    atc_config_ptr->ceer_code[CEER_PDP_DEACTIVATE] = (uint32)sig_ptr->result;
    atc_config_ptr->ceer_last_type = CEER_PDP_DEACTIVATE;

    ResetGPRSDataConf();
	
#ifdef _SCM21_ATC_DEV
	SetGPRSDataConfStatus(TRUE, 0, ATC_MODEM_IP_GPRSDEACT);
#endif //_SCM21_ATC_DEV

#ifdef _MUX_ENABLE_

    for(i = 0; i < sig_ptr->pdp_num; i++)
    {
        TCPIPPS_ResetModemService(sig_ptr->pdp_id[i]);
    }

#else

        TCPIP_ResetPPPInterface();

#endif /* _MUX_ENABLE_ */

    ATC_TRACE_LOW("ATC: Close MODEM service!");

    for(i = 0; i < sig_ptr->pdp_num; i++)
    {
        cid = sig_ptr->pdp_id[i];

        if(cid >= 1 && cid <= MN_GPRS_MAX_PDP_CONTEXT_COUNT &&
            INIT_BY_ATC == atc_config_ptr->atc_psd_call_info[cid-1].init_mod_state)
        {
            if((ATC_INVALID_CID != atc_config_ptr->atc_psd_call_info[cid-1].cid) &&
                (TRUE == atc_config_ptr->atc_psd_call_info[cid-1].data_mode ||
                 TRUE == atc_config_ptr->atc_psd_call_info[cid-1].data_mode_req))
            {
                /* For data mode -- PPP connection, if network disconnect PPP,
                * We need protect possible ATH from AP side in PSD link id. */
            #ifdef _MUX_ENABLE_
                ATC_Link_ATHEventHandle(atc_config_ptr->atc_psd_call_info[cid - 1].link_id, ATC_ATH_EV_PDP_DEACT, cid);
            #else
                ATC_Link_ATHEventHandle(0, ATC_ATH_EV_PDP_DEACT, ATC_INVALID_CID);
            #endif
            }

            ATC_Clear_PSD_Call_Info(cid);
            #ifndef ATC_SAT_ENABLE
                ATC_FreeDummyData(cid + 4);
            #endif
        }
    }

    if(!ATC_IsExist_DataMode_PSD_Call_Info())
    {
        ATC_SetPsActivateStatus(FALSE);
    }

    /* 1)when pdp actived by IPOPEN, but responsed by APP_MN_DEACTIVATE_PDP_CONTEXT_IND,
         g_is_dns_act_pdp is true, while g_is_dns_pdp_actived is false;
       2)pdp(cid=1) for dns is actived, decactived by network, while g_is_dns_act_pdp is
         false, g_is_dns_pdp_actived is true.
    */

     if(g_is_dns_act_pdp || g_is_dns_pdp_actived)
    {
        ATC_ProcessPDPDeactivatedByNetwork(atc_config_ptr);
    }
}
#endif

/******************************************************************************/
//  DESCRIPTION: This function handles APP_MN_DEACTIVATE_PDP_CONTEXT_CNF
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  minqian.qian
//  Note :
/******************************************************************************/
void ATC_ProcessDeactivatePDPContextCNF(
    ATC_CONFIG_T                    *atc_config_ptr,            // point to the ATC_CONFIG_T struct.
    APP_MN_GPRS_DEACTIVATE_CNF_T    *sig_ptr,
    BOOLEAN                         recv_event_result
)
{
    uint8   cid = 0;
#ifdef  _MUX_ENABLE_
    int     i   = 0;
#endif

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != sig_ptr);/*assert verified: check null pointer*/

    cid = sig_ptr->pdp_id[0];

    atc_config_ptr->ceer_code[CEER_PDP_DEACTIVATE] = (uint32)sig_ptr->result;
    atc_config_ptr->ceer_last_type = CEER_PDP_DEACTIVATE;

    {
#ifdef  _MUX_ENABLE_

        for(i = 0; i < sig_ptr->pdp_num; i++)
        {
            TCPIPPS_ResetModemService(sig_ptr->pdp_id[i]);
        }

#else

        TCPIP_ResetPPPInterface();

#endif /* _MUX_ENABLE_ */
    }

    if(sig_ptr->pdp_num > 0 && cid >= 1 && cid <= MN_GPRS_MAX_PDP_CONTEXT_COUNT)
    {
        #ifndef ATC_SAT_ENABLE
            ATC_FreeDummyData(cid + 4);
        #endif

        if(recv_event_result)
        {
            if(MN_GPRS_ERR_SUCCESS == sig_ptr->result)
            {
                /* After successful deactivation, maybe ATH will be comming.
                * pdp_type_connect is TRUE, means PPP dial
                */
                if(TRUE == atc_config_ptr->atc_psd_call_info[cid-1].data_mode ||
                    TRUE == atc_config_ptr->atc_psd_call_info[cid-1].data_mode_req)
                {
                #ifdef _MUX_ENABLE_
                    ATC_Link_ATHEventHandle(atc_config_ptr->atc_psd_call_info[cid - 1].link_id,
                                            ATC_ATH_EV_PDP_DEACT,
                                            atc_config_ptr->atc_psd_call_info[cid - 1].cid);
                #else
                    ATC_Link_ATHEventHandle(0, ATC_ATH_EV_PDP_DEACT, ATC_INVALID_CID);
                #endif
                }

                ATC_Clear_PSD_Call_Info(cid);

                /*unsolicited ^CEND*/
                ATC_UpCENDInfo(atc_config_ptr, (xSignalHeaderRec *)sig_ptr);
            }

            if(!ATC_IsExist_DataMode_PSD_Call_Info())
            {
                ATC_SetPsActivateStatus(FALSE);
            }
        }
        else
        {
            if(MN_GPRS_ERR_SUCCESS == sig_ptr->result)
            {
                sprintf((char *)g_rsp_str, "NO CARRIER");
                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);

                ATC_Clear_PSD_Call_Info(cid);
                /*unsolicited ^CEND*/
                ATC_UpCENDInfo(atc_config_ptr, (xSignalHeaderRec *)sig_ptr);
            }

            return;
        }
    }

#ifdef _MUX_ENABLE_    
    if(!recv_event_result)
    {
        return;
    }
#endif

    if(MN_GPRS_ERR_SUCCESS == sig_ptr->result)
    {
        atc_config_ptr->ceer_val = CEER_ERR_181_NOMAL_DEACTIVE_PDP;
        if(!atc_config_ptr->async_mode_flag)
        {
#ifdef _SCM21_ATC_DEV
        	////after process ATC_ProcessDeactivatePDPContextCNF, echo "SHUT OK" on TE
        	if(s_config_info.is_echo_shut_ok == TRUE)
        	{
        		s_config_info.is_echo_shut_ok = FALSE;	 //reset to false
        		ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_SHUT_OK, 0);
		}
		else
#endif /* _SCM21_ATC_DEV */
		{
			ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
		}
           
        }
		
#ifdef _SCM21_ATC_DEV
		SetGPRSDataConfStatus(TRUE, 0, ATC_MODEM_IP_INITIAL);
#endif //_SCM21_ATC_DEV
    }
    else
    {
        if(!atc_config_ptr->async_mode_flag)
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, sig_ptr->result);
        }
    }
}

#ifdef _SUPPORT_GPRS_
#ifndef ATC_SAT_ENABLE
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
    ATC_CONFIG_T *atc_config_ptr,
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
    if(TRUE == is_all)
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
    switch(g_atc_mt_response_type[dual_sys])
    {
        case MN_GPRS_TE_REJECT_ACTIVE:
            result = MNGPRS_DeactivatePdpContextEx(dual_sys, g_atc_is_all_pdp[dual_sys], g_atc_te_pdp_context[dual_sys]);
            break;
        case MN_GPRS_MT_ACTIVE:
            result = ATC_ActivatePdpContextEx(atc_config_ptr,dual_sys, g_atc_is_all_pdp[dual_sys], g_atc_te_pdp_context[dual_sys], POC_E, MN_UNSPECIFIED);
            //result = MNGPRS_ActivatePdpContextEx(dual_sys, g_atc_is_all_pdp[dual_sys], g_atc_te_pdp_context[dual_sys], POC_E, MN_UNSPECIFIED);
            break;

        default:
            break;
    }

    return result;
}

/*****************************************************************************/
//  Function    : HandleCGEREP
//  Description : set event report mode and buffer control
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
            /* do nothing */
            break;

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
                    //sprintf((char*)g_rsp_str + 2, "OK");
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
#endif
#endif

/******************************************************************************/
//  DESCRIPTION: This function handles APP_MN_ACTIVATE_PDP_CONTEXT_CNF
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  minqian.qian
//  Note :
/******************************************************************************/
ATC_STATUS ATC_ProcessActivatePDPContextCNF(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_GPRS_EXT_T         *sig_ptr,
    BOOLEAN                                 recv_event_result
)
{
    uint8   pdp_id = ATC_INVALID_CID;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32  pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
	
#ifdef _MUX_ENABLE_
    int32  set_ppp_result = 0;
    TCPIPPS_MODEM_CFG_T modem_cfg;
#endif

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != sig_ptr);/*assert verified: check null pointer*/

    pdp_id = sig_ptr->pdp_id;

    ATC_TRACE_LOW("ATC: ATC_ProcessActivatePDPContextCNF, pdp_id=%d", pdp_id);

    if(SCI_IsTimerActive(s_atc_ats7_timer[pdp_id]))
    {
        SCI_DeactiveTimer(s_atc_ats7_timer[pdp_id]);
    }

    if(!recv_event_result)
    {
        if(pdp_id >= 1 && pdp_id <= ATC_MAX_ACTIVATE_PDP)
        {
            atc_config_ptr->atc_psd_call_info[pdp_id-1].init_mod_state = INIT_BY_MMI;
        }

        ATC_TRACE_LOW("ATC: pdp activate is not started by atc.");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    atc_config_ptr->ceer_last_type = CEER_PDP_ACTIVATE;

    if((sig_ptr->result >= MN_GPRS_ERR_SUCCESS) && (sig_ptr->result <= MN_GPRS_ERR_SUCCESS_SINGLE_ONLY))
    {
        atc_config_ptr->ceer_code[CEER_PDP_ACTIVATE] = 0;
        ATC_Set_PSD_Call_State(pdp_id, ATC_PSD_STATE_ACTIVE);
    }
    else
    {
        atc_config_ptr->ceer_code[CEER_PDP_ACTIVATE] = (uint32)sig_ptr->result;
		
    #ifdef HUAWEI_6500_MODEM
        PPPPS_CloseModemService(atc_config_ptr->current_link_id);
    #endif
	
        ATC_UpSPERRORInfo(sig_ptr->dual_sys, atc_config_ptr, PDP_ACTIVATION_REJECT_CLASS, (uint32)sig_ptr->result);
    }

    //  pdp acticve reject by network in auth_type 3, reactive
    if(s_atc_auth_param.cgauth_type == MN_PAP_CHAP  && !s_atc_second_auth_flag)
    {
        if(sig_ptr->result == MN_GPRS_ERR_UNKNOWN_ERROR
           ||sig_ptr->result == MN_GPRS_ERR_PERMENANT_PROBLEM
           || sig_ptr->result== MN_GPRS_ERR_RETRYING )
        {
            s_atc_second_auth_flag = TRUE;
            
            if(MN_GPRS_ERR_SUCCESS != MNGPRS_SetPdpContextPcoCipherEx(dual_sys, s_atc_auth_param.cgauth_cid, 
								(uint8 *)s_atc_auth_param.cgauth_user, (uint8 *)s_atc_auth_param.cgauth_passwd,MN_CHAP))
            {
                ATC_TRACE_LOW("ATC: reset pco error");
            }
            else
            {
                pdp_id_arr[0] = s_atc_auth_param.cgauth_cid;
                if( ERR_MNGPRS_NO_ERR == ATC_ActivatePdpContextEx(atc_config_ptr,dual_sys, FALSE, pdp_id_arr, POC_E, MN_UNSPECIFIED))
                //if( ERR_MNGPRS_NO_ERR == MNGPRS_ActivatePdpContextEx(dual_sys, FALSE, pdp_id_arr, POC_E, MN_UNSPECIFIED))
                {
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,pdp_id_arr[0],ATC_DOMAIN_PS, dual_sys);
                
                    return S_ATC_SUCCESS;
                }
                else
                {
                    ATC_TRACE_LOW("ATC: re-act pdp error");
                }
            }
        }
    }

    if((pdp_id >= 1 && pdp_id <= ATC_MAX_ACTIVATE_PDP) &&
        (TRUE == atc_config_ptr->atc_psd_call_info[pdp_id-1].data_mode_req))
    {
        BOOLEAN pdp_change_to_data_mode = FALSE;

        /* Take into "+ACT -> +CGDATA" consideration, non-data mode PDP can be changed into
        * data mode, so when PDP is activated, if:
        * 1. cause: "already activated"
        * 2. data_mode: FALSE --> data_mode_req: TRUE
        * 3. request link-id (current) is equal to call link id */
        if(sig_ptr->result == MN_GPRS_ERR_PDP_CONTEXT_ACTIVATED
            && (FALSE == atc_config_ptr->atc_psd_call_info[pdp_id-1].data_mode)
         #ifdef _MUX_ENABLE_
            && (atc_config_ptr->current_link_id == atc_config_ptr->atc_psd_call_info[pdp_id-1].link_id)
         #endif
          )
        {
            ATC_TRACE_LOW("ATC: PDP (cid:%d) change to data mode context", pdp_id);

            atc_config_ptr->atc_psd_call_info[pdp_id-1].data_mode = TRUE;
            pdp_change_to_data_mode = TRUE;
        }

        atc_config_ptr->atc_psd_call_info[pdp_id-1].data_mode_req = FALSE;

        if(((sig_ptr->result >= MN_GPRS_ERR_SUCCESS) && (sig_ptr->result <= MN_GPRS_ERR_SUCCESS_SINGLE_ONLY)) || (TRUE == pdp_change_to_data_mode))
        {
        #ifdef _MUX_ENABLE_
     
            int8 tmp_data_link = ATC_DEBUG_LINK - 1;
        #endif

            if(atc_config_ptr->cr)
            {
                sprintf((char *)g_rsp_str, "+CR: GPRS PPP");
                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
            }

            if(atc_config_ptr->crc && atc_config_ptr->async_mode_flag)
            {
                sprintf((char *)g_rsp_str, "+CRING: REL ASYNC");
                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);	         
            }
#ifndef HUAWEI_6500_MODEM		 
            if (!atc_config_ptr->async_mode_flag)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_CONNECT, 0);
            }
            else
            {
                sprintf((char *)g_rsp_str, "CONNECT");
                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
            }

            if((sig_ptr->result >= MN_GPRS_ERR_SUCCESS) && (sig_ptr->result <= MN_GPRS_ERR_SUCCESS_SINGLE_ONLY))
            {
                sprintf((char *)g_rsp_str, "^CONN: %d,%d", pdp_id, ATC_CMCC_CALLIND_CALL_TYPE_PACKET_DATA);
                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
            }
#endif

#ifdef _MUX_ENABLE_
            tmp_data_link =  atc_config_ptr->atc_psd_call_info[pdp_id-1].data_link_id;

            if(g_atc_mux_mode == ATC_MUX_FREE_MAP)
            {
                g_mux_dlc_info[tmp_data_link].work_mode = MUX_DLC_DATA_MODE;
            }
            /* +CGDATA: change PDP to data MODE. */
            if(pdp_change_to_data_mode)
            {
                ATC_Link_ATHEventHandle(atc_config_ptr->current_link_id,
                        ATC_ATH_EV_NEW_PDP_SRV, pdp_id);
            }

            modem_cfg.nsapi = sig_ptr->nsapi;
            modem_cfg.link_id = tmp_data_link;
            modem_cfg.cid = pdp_id;
            modem_cfg.pkt_type = g_packet_type[pdp_id];
            SCI_TRACE_LOW("ATC: TCPIPPS_SetModemService, cid:%d,g_packet_type:%d,tmp_data_link=%d", pdp_id, g_packet_type[pdp_id],tmp_data_link);
            set_ppp_result = TCPIPPS_SetModemService(&modem_cfg); 
            if(set_ppp_result)
            {
                ATC_TRACE_LOW("ATC:  MPPP_SetPPPInterface() fail");
            }

#else
    #ifdef _SUPPORT_GPRS_
            TCPIP_SetPPPInterface(sig_ptr->nsapi);
		
    #endif /* _SUPPORT_GPRS_ */
#endif /* _MUX_ENABLE_ */

#ifndef HUAWEI_6500_MODEM
            ATC_TRACE_LOW("ATC: Open MODEM service!");
#endif
            ATC_SetPsActivateStatus(TRUE);
        }
        else
        {
            uint8 link_id;
            ATC_TRACE_LOW("ATC: APP_MN_ACTIVATE_PDP_CONTEXT_CNF(failure), cid:%d", pdp_id);

        #ifdef _MUX_ENABLE_
            link_id = ATC_Get_Link_Id(pdp_id, ATC_DOMAIN_PS, sig_ptr->dual_sys);

            if((uint8)ATC_INVALID_LINK_ID != link_id)
            {
                /* activation failure, maybe ATH will be comming.
                 * pdp_type_connect is TRUE, means PPP dial */
                ATC_Link_ATHEventHandle(link_id, ATC_ATH_EV_PDP_ACT_REJ, pdp_id);
            }
            else
            {
                ATC_TRACE_LOW("ATC: ERROR: no found link_id of PSD!");
            }

        #else
            /* non-mux version ATC: no link_id, so use DUMMY_LINK_ID:0
             * activation failure, maybe ATH will be comming.
             * pdp_type_connect is TRUE, means PPP dial */
            link_id = 0;
            ATC_Link_ATHEventHandle(link_id, ATC_ATH_EV_PDP_ACT_REJ, pdp_id);
        #endif

            ATC_Clear_PSD_Call_Info(pdp_id);
        
            if (!atc_config_ptr->async_mode_flag)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, sig_ptr->result);
            }
        }
    }
    else if(g_is_dns_act_pdp)
    {
        IPOPEN_ProcessActPdpContextCnf(atc_config_ptr,(APP_MN_GPRS_T *)sig_ptr);
    }
#ifdef  _MNGPS_SUPPORT_   
    else if(TRUE == s_is_gps_act_pdp_req)
    {
        ATC_ProcessGpsEvent(atc_config_ptr,(xSignalHeaderRec *)sig_ptr);
    }
#endif
    else
    {
        if((sig_ptr->result >= MN_GPRS_ERR_SUCCESS) && (sig_ptr->result <= MN_GPRS_ERR_SUCCESS_SINGLE_ONLY))
        {
            if (!atc_config_ptr->async_mode_flag)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            }
        }
        else
        {
            ATC_Clear_PSD_Call_Info(pdp_id);

            if (!atc_config_ptr->async_mode_flag)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_GPRS_OPERATION_FAILURE);
            }
        }
    }
	
#ifdef _SCM21_ATC_DEV
	SetGPRSDataConfStatus(TRUE, 0, ATC_MODEM_IP_GPRSACT);
#endif //_SCM21_ATC_DEV

    return S_ATC_SUCCESS;
}

#ifndef ATC_SAT_ENABLE
/******************************************************************************/
//  DESCRIPTION:This function handles command AT+SIPCONFIG, get pdp's IP configure info
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Elvis.xu
//  Note :
/******************************************************************************/
ATC_STATUS ATC_ProcessSIPCONFIG( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint8  pdp_id[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
    ERR_MNGPRS_CODE_E   ret = ERR_MNGPRS_NO_ERR;
    ATC_CUR_PARAM_INFO_LIST_T  *param_ptr;
    BOOLEAN    first_valid_pdp_flag = TRUE;
    uint8      valid_cid_num = 0;
    uint32    pdp_address = 0;
    uint32    pri_dns = 0;
    uint32    sec_dns = 0;
    uint8      nsapi = 0;
    uint8      *tmp_str = NULL;
    uint8      i = 0;

    SCI_TRACE_LOW("ATC: ATC_ProcessSIPCONFIG");

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != cur_cmd_info);/*assert verified: check null pointer*/

    //AT+SIPCONFIG=[<cid>[,<cid>[,...]]]
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            //get all cid parameters
            param_ptr = cur_cmd_info->cur_param_list_ptr;

            for(i = 0; (param_ptr != PNULL) && (i < MN_GPRS_MAX_PDP_CONTEXT_COUNT); i++)
            {
                if(cur_cmd_info->param_used[i])
                {
                    if((MN_GPRS_MAX_PDP_CONTEXT_COUNT < param_ptr->param_info.num) ||
                    (0 == param_ptr->param_info.num) ||
                    (ATC_CMD_PARAM_TYPE_NUMERIC != param_ptr->param_type))
                    {
                        return ERR_INVALID_INDEX;
                    }

                    pdp_id[i] = (uint32)param_ptr->param_info.num;
                    valid_cid_num++;
                }

                param_ptr = param_ptr->next;
            }

            //when "AT+SIPCONFIG="   valid command, equaling to reading command
            //when "AT+SIPCONFIG=,,...," without any parameter invalid command
            if((valid_cid_num != i && valid_cid_num) ||
            ((valid_cid_num == 0) && (i > 1)))
            {
                SCI_TRACE_LOW("ATC: some parameter's type or order wrong, not according to spec rule");
                return ERR_INVALID_INDEX;
            }
            
            if(atc_config_ptr->atc_config_nv.spauto_flag & 0x04) //for android iq mode
            {
                sprintf((char *)g_rsp_str, "%s: 1,5,0x%C0C0C0C0,0x%08080808,0x%80808080",/*lint !e816 !e557*/
                            g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }

            //get all active pdp ip configure information when no cid parameter
            //in case no active pdp context  for rsp
            g_rsp_str[0] = '\0';
            tmp_str = &g_rsp_str[0];

            if(valid_cid_num == 0)
            {
                for(i = 0; i < MN_GPRS_MAX_PDP_CONTEXT_COUNT; i++)
                {
                    ret = MNGPRS_ReadPdpParametersEx(dual_sys, i + 1, &pdp_address, &pri_dns, &sec_dns, &nsapi);

                    if(ERR_MNGPRS_NO_ERR == ret)
                    {
                        if(first_valid_pdp_flag)
                        {
                            first_valid_pdp_flag = FALSE;

                            sprintf((char *)tmp_str, "%s: %d,%d,0x%08lX,0x%08lX,0x%08lX",
                            g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr,
                            i + 1, nsapi, pdp_address, pri_dns, sec_dns);
                        }
                        else
                        {
                            tmp_str += strlen((char *)tmp_str);
                            sprintf((char *)tmp_str, "\r\n%s: %d,%d,0x%08lX,0x%08lX,0x%08lX",
                            g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr,
                            i + 1, nsapi, pdp_address, pri_dns, sec_dns);
                        }
                    }
                }
            }
            //get pdp context ip configure information for cids selected
            else
            {
                for(i = 0; i < valid_cid_num; i++)
                {
                    ret = MNGPRS_ReadPdpParametersEx(dual_sys, pdp_id[i], &pdp_address, &pri_dns, &sec_dns, &nsapi);

                    if(ERR_MNGPRS_NO_ERR == ret)
                    {
                        if(first_valid_pdp_flag)
                        {
                            first_valid_pdp_flag = FALSE;

                            sprintf((char *)tmp_str, "%s: %d,%d,0x%08lX,0x%08lX,0x%08lX",
                            g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr,
                            pdp_id[i], nsapi, pdp_address, pri_dns, sec_dns);
                        }
                        else
                        {
                            tmp_str += strlen((char *)tmp_str);
                            sprintf((char *)tmp_str, "\r\n%s: %d,%d,0x%08lX,0x%08lX,0x%08lX",
                            g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr,
                            pdp_id[i], nsapi, pdp_address, pri_dns, sec_dns);
                        }
                    }
                    else
                    {
                        if(first_valid_pdp_flag)
                        {
                            first_valid_pdp_flag = FALSE;

                            sprintf((char *)tmp_str, "%s: %d,%d,0x%08lX,0x%08lX,0x%08lX",
                            g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr,
                            pdp_id[i], 0, 0, 0, 0);
                        }
                        else
                        {
                            tmp_str += strlen((char *)tmp_str);
                            sprintf((char *)tmp_str, "\r\n%s: %d,%d,0x%08lX,0x%08lX,0x%08lX",
                            g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr,
                            pdp_id[i], 0, 0, 0, 0);
                        }
                    }
                }
            }

            if(!strlen((char *)g_rsp_str))
            {
                sprintf((char *)g_rsp_str, "%s: ", g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr);
            }

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_READ:
            //in case no active pdp context  for rsp
            g_rsp_str[0] = '\0';
            tmp_str = &g_rsp_str[0];

            for(i = 0; i < MN_GPRS_MAX_PDP_CONTEXT_COUNT; i++)
            {
                ret = MNGPRS_ReadPdpParametersEx(dual_sys, i + 1, &pdp_address, &pri_dns, &sec_dns, &nsapi);

                if(ERR_MNGPRS_NO_ERR == ret)
                {
                    if(first_valid_pdp_flag)
                    {
                        first_valid_pdp_flag = FALSE;

                        sprintf((char *)tmp_str, "%s: %d,%d,0x%08lX,0x%08lX,0x%08lX",
                        g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr,
                        i + 1, nsapi, pdp_address, pri_dns, sec_dns);
                    }
                    else
                    {
                        tmp_str += strlen((char *)tmp_str);
                        sprintf((char *)tmp_str, "\r\n%s: %d,%d,0x%08lX,0x%08lX,0x%08lX",
                        g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr,
                        i + 1, nsapi, pdp_address, pri_dns, sec_dns);
                    }
                }
            }

            if(!strlen((char *)g_rsp_str))
            {
                sprintf((char *)g_rsp_str, "%s: ", g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr);
            }

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (1-11)", g_atc_info_table[AT_CMD_SIPCONFIG].cmd_name->str_ptr);
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
//  DESCRIPTION:This function handles command AT+SGFD, force grps detach
//  Global resource dependence : atc_config_ptr
//  AUTHOR: 
//  Note :
/******************************************************************************/
ATC_STATUS ATC_ProcessSGFD( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
)
{
#ifdef _SUPPORT_GPRS_
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            if(ERR_MNPHONE_NO_ERR != MNPHONE_GprsForceDetachEx(dual_sys, MN_PHONE_GPRS_DETACH))
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            break;

        default:
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */
}

#endif

/*****************************************************************************/
//  Description : This function handle the APP_MN_GPRS_ATTACH_RESULT signal
//  Global resource dependence : none
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessGprsAttachResult(
    ATC_CONFIG_T            *atc_config_ptr,// point to the ATC_CONFIG_T struct.
    APP_MN_GPRS_ATTACH_RESULT_T *sig_ptr        // point to the APP_MN_GPRS_ATTACH_RESULT signal
)
{
#ifdef _SUPPORT_GPRS_
    MN_PHONE_CURRENT_PLMN_INFO_T    register_status;
    OPER_STATUS_E    gprs_oper_status = NOT_REGISTERED_NOT_SEARCHING;
    MN_DUAL_SYS_E   dual_sys = sig_ptr->dual_sys;

    register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
    ATC_GetOperStatus(dual_sys, atc_config_ptr, &register_status, PNULL, &gprs_oper_status);
    ATC_UpCGREGInfo(dual_sys, atc_config_ptr, &register_status, gprs_oper_status, FALSE);
 
    atc_config_ptr->ceer_last_type = CEER_GPRS_ATTACH;

    if(sig_ptr->is_attach_ok)
    {
        atc_config_ptr->ceer_code[CEER_GPRS_ATTACH] = 0;
    }
    else
    {
        atc_config_ptr->ceer_code[CEER_GPRS_ATTACH] = (uint32)(sig_ptr->err_cause);
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */
}

/*****************************************************************************/
//  Description : This function handle the APP_MN_GPRS_DETACH_RESULT signal
//  Global resource dependence : none
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessGprsDetachResult(
    ATC_CONFIG_T            *atc_config_ptr,// point to the ATC_CONFIG_T struct.
    APP_MN_GPRS_DETACH_RESULT_T *sig_ptr        // point to the APP_MN_GPRS_DETACH_RESULT signal
)
{
#ifdef _SUPPORT_GPRS_
    MN_PHONE_CURRENT_PLMN_INFO_T    register_status;
    OPER_STATUS_E    gprs_oper_status = NOT_REGISTERED_NOT_SEARCHING;
    MN_DUAL_SYS_E   dual_sys = sig_ptr->dual_sys;

    register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
    ATC_GetOperStatus(dual_sys, atc_config_ptr, &register_status, PNULL, &gprs_oper_status);
    ATC_UpCGREGInfo(dual_sys, atc_config_ptr, &register_status, gprs_oper_status, FALSE);

    atc_config_ptr->ceer_last_type = CEER_GPRS_DETACH;
    
    if(sig_ptr->is_detach_ok)
    {
        atc_config_ptr->ceer_val = CEER_ERR_180_NOMAL_DETACH_GPRS;
        atc_config_ptr->ceer_code[CEER_GPRS_DETACH] = 0;
    }

    if(g_atc_ip_detach)
    {
        g_atc_ip_detach = FALSE;
        
        sprintf((char *)g_rsp_str, "IPDEACT OK");
        
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_DEFAULT_HANDLE;
    }
    
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _SUPPORT_GPRS_ */
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: wuding.yang
//  Note: 
/*****************************************************************************/
LOCAL void ATC_ATS7_TimerHandler(uint32 lparam)//lparam is card_id and pdp_id
{
	MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    APP_MN_GPRS_EXT_T *signal_ptr = PNULL;
    uint32 pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};

    pdp_id_arr[0] = lparam & 0xF;
    pdp_id_arr[1] = MN_GPRS_PDP_ID_INVALID;

    //SCI_TRACE_LOW("ATC_ATS7_TimerHandler pdp_id=%d",pdp_id_arr[0]);

    SCI_CREATE_SIGNAL(signal_ptr, APP_MN_ACTIVATE_PDP_CONTEXT_CNF,
                      sizeof(APP_MN_GPRS_EXT_T), P_ATC);

    signal_ptr->dual_sys = (MN_DUAL_SYS_E)(lparam>>4);
    signal_ptr->result = MN_GPRS_ERR_UNKNOWN_ERROR;
    signal_ptr->pdp_id = (uint8)pdp_id_arr[0];
    signal_ptr->nsapi = 0;// MN_GPRS_ERR_UNKNOWN_ERROR, the ATC layer will not check nsapi

    SCI_SEND_SIGNAL(signal_ptr, P_ATC);

    MNGPRS_DeactivatePdpContextEx(dual_sys, FALSE, pdp_id_arr);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:       wuding.yang
//  Note:
/*****************************************************************************/
ERR_MNGPRS_CODE_E ATC_ActivatePdpContextEx(//return whether success or failure
                ATC_CONFIG_T            *atc_config_ptr,
                MN_DUAL_SYS_E dual_sys,
                BOOLEAN is_all,         //if all PDP contexts need activate.
                uint32 pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT], // the identity of the PDP contexts will be activated.
                MN_PS_SERVICE_TYPE_E ps_service_type,
                MN_PS_SERVICE_RAT_E  ps_service_rat
                )
{
    uint8 i = 0;
    ERR_MNGPRS_CODE_E result;
    uint32 ats7_timer_value = 60000;//60 seconds
    uint8 cardid_and_pdpid = 0;

    SCI_TRACE_LOW("ATC_ActivatePdpContextEx s7_reg=%d",atc_config_ptr->s7_reg);
    if (0 == atc_config_ptr->s7_reg)
    {
        result = ERR_MNGPRS_CALL_FAILURE;//I think ATS7=0 means not work
    }
    else
    {
        result = MNGPRS_ActivatePdpContextEx(dual_sys,is_all,pdp_id_arr,ps_service_type,ps_service_rat);
        if (ERR_MNGPRS_NO_ERR == result)
        {
            for(i=0;i<MN_GPRS_MAX_PDP_CONTEXT_COUNT;i++)
            {
                if ((MN_GPRS_PDP_ID_INVALID != pdp_id_arr[i]) 
                    || is_all)
                {
                    if (pdp_id_arr[i] <= MN_GPRS_MAX_PDP_CONTEXT_COUNT)
                    {
                        if (!s_atc_ats7_timer[pdp_id_arr[i]])
                        {
                            cardid_and_pdpid = ((uint8)dual_sys)<<4;
                            cardid_and_pdpid |= pdp_id_arr[i];
                            s_atc_ats7_timer[pdp_id_arr[i]] = SCI_CreateTimer("ATC ATS7 Timer", ATC_ATS7_TimerHandler, cardid_and_pdpid, (uint32)(DEF_S7*1000), SCI_NO_ACTIVATE);
                        }

                        ats7_timer_value = (atc_config_ptr->s7_reg)*1000;

                        if(SCI_IsTimerActive(s_atc_ats7_timer[pdp_id_arr[i]]))
                        {
                            SCI_DeactiveTimer(s_atc_ats7_timer[pdp_id_arr[i]]);
                        }
                        SCI_ChangeTimer(s_atc_ats7_timer[pdp_id_arr[i]], ATC_ATS7_TimerHandler, ats7_timer_value);
                        SCI_ActiveTimer(s_atc_ats7_timer[pdp_id_arr[i]]);
                    }
                }
            }
        }
    }
    return result;
}

