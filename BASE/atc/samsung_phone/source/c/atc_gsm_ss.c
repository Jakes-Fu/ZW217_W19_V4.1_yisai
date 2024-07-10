/******************************************************************************
 ** File Name:      atc_gsm_ss.c                                              *
 ** Author:         Xueli.lv                                                  *
 ** Date:           10/09/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains the functions that handle the AT       * 
 **                 command regarding SS(supplement service)                  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 10/09/2002     Xueli.lv         Create.                                   *
 ** 15/07/2003     Liuhan           Modify.                                   *
 ******************************************************************************/
#include "base_trc.h"
#include "atc_gsm_ss.h"
#include "atc_gsm.h"
#include "atc_common.h"
#if 0  //CR171132 for 6530 codesize cutdown
#include "atc_gsm_stk.h"
#endif
#include "env_atc_signal.h"
#include "sig_code.h"
#include "sci_codec.h"
#include "audio_api.h"
#include "audio_prod.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/

#ifndef _ULTRA_LOW_CODE_
static ATC_SS_CONTEXT_T      s_ss_context;
static uint8                 s_clck_psw[AT_CLCK_PSW_LEN];
static uint8                 s_old_password[AT_CLCK_PSW_LEN];    // the old pin
static uint8                 s_new_password[AT_CLCK_PSW_LEN];    // the new pin
static uint8                 s_new_password_again[AT_CLCK_PSW_LEN];// the new pin again
static uint8                 s_cusd_presentation = AT_CUSD_DISABLE;
static BOOLEAN               s_cusd_isresponse   = FALSE;
static BOOLEAN               s_cusd_notification = TRUE; // For cingular,unsolicited ussd message from net
#endif
static HAUDIO hAtcVoiceRing = INVALID_HANDLE;
extern RSP_RESULT_STR        g_rsp_str;
#ifdef _GSM_ONLY_  //CR171132 for 6530 codesize cutdown
extern uint8                 g_tel_str[2*MN_MAX_ADDR_BCD_LEN + 1];
#endif
extern const  ATC_INFO_T     g_atc_info_table[];
#if 0
#ifndef _ULTRA_LOW_CODE_
extern BOOLEAN               g_is_stk_call[MN_SYS_NUMBER];
#endif
#endif
extern BOOLEAN               g_atc_call_flag;
extern BOOLEAN               g_atc_expire_ind;
extern ATC_CALLS_STATE_T     g_calls_state[MN_SYS_NUMBER];
extern SCI_TMID              T_ATC_CALL_EXP_IND;
/*lint +e552*/
extern uint8                 g_atc_expire_call_id;
/*lint -e552*/
#ifdef _MUX_REDUCED_MODE_
extern BOOLEAN               g_received_sim_ind[MN_DUAL_SYS_MAX];
extern BOOLEAN               g_sent_sind4;
#endif


#ifndef _MUX_ENABLE_
extern  MN_DUAL_SYS_E g_current_card_id;
#endif

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function convert the ATC CF(call forwarded) reason code
//                to the MNSS's code
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void ConvertServiceCode(
            ATC_CLASS_E     service_type,      // the service type
            uint16          *service_code_ptr  // the SS code
            );

/*****************************************************************************/
//  Description : This function handles release all of the held call or release 
//  release the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldUdub(
                        ATC_CONFIG_T        *atc_config_ptr
                        );

/*****************************************************************************/
//  Description : This function handles release all of the held call or release 
//  release the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelAcceptOthers(
                        ATC_CONFIG_T        *atc_config_ptr
                        );

/*****************************************************************************/
//  Description : This function hold the active calls and accept the held or waiting
//  calls 
//  release the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldHoldAcceptOthers(
                        ATC_CONFIG_T        *atc_config_ptr
                        );

/*****************************************************************************/
//  Description : This function adds the held call to the conversation
//  calls 
//  release the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldMultCall(
                   ATC_CONFIG_T        *atc_config_ptr
                   );

/*****************************************************************************/
//  Description : This function handle ect operation
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleEctCall(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E  dual_sys
);

/*****************************************************************************/
//  Description : This function convert the ATC CF(call forwarded) reason code
//                to the MNSS's code
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void ConvertCfCode(
            ATC_CCFC_REASON_E     ccfc_reason,   // the call forwarded reason
            MN_SS_CODE_E          *ss_code_ptr   // the SS code
            );

/*****************************************************************************/
//  Description : This function check the SS CNF signal 
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_ProcessSSCnf(
            ATC_CONFIG_T              *atc_config_ptr,
            MN_SS_OPERATE_RESULT_E    result_type,
            uint8                     list_num,
            MN_SS_INTERROGATE_INFO_T  info_list[],
            BOOLEAN                   op_type  //APP_MN_INTERROGATE_CNF,APP_MN_REGISTER_SS_CNF,
                                                //APP_MN_ACTIVATE_SS_CNF is TRUE
                                               //   APP_MN_ERASE_SS_CNF,APP_MN_DEACTIVATE_SS_CNF
                                               //is FALSE
                                );

/*****************************************************************************/
//  Description : This function handle APP_MN_INTERROGATE_CNF signal 
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_CheckInterrogateCnf(
            ATC_CONFIG_T              *atc_config_ptr,
            MN_SS_OPERATE_RESULT_E    result_type,
            uint8                     list_num,
            MN_SS_INTERROGATE_INFO_T  info_list[],
            BOOLEAN              *is_service_active
                                );

/*****************************************************************************/
//  Description : This function handle the SS service in AT+CLCK.
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
#ifdef _MUX_ENABLE_
LOCAL BOOLEAN HandleClckSSReq(
                    ATC_CONFIG_T   *atc_config_ptr,
                    MN_SS_CODE_E    ss_code,
                    uint8           operate_type,
                    uint8           class_type
                    );
#else
LOCAL BOOLEAN HandleClckSSReq(
                    MN_SS_CODE_E    ss_code,
                    uint8           operate_type,
                    uint8           class_type
                    );
#endif

/*****************************************************************************/
//  Description : This function initializes the static variables for SS module
//  Global resource dependence : 
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern void ATC_InitSsVar(void)
{
    s_cusd_presentation = AT_CUSD_DISABLE;
    s_cusd_isresponse   = FALSE;
    SCI_MEMSET(&s_ss_context, 0, sizeof(s_ss_context));
}

/*****************************************************************************/
//  Description : Send stand ss request to MN
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL ATC_STATUS ATC_SendReqToSs(
                          ATC_CONFIG_T              *atc_config_ptr,                          
                          MN_PARSE_INFO_U           *ss_info_ptr
                          );

/*****************************************************************************/
//  Description : retrieve the cmd id from ss code
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL int16 RetrieveCmdId(
                   MN_SS_CODE_E ss_code
                   );
#endif

/*****************************************************************************/
//  Description:    This function is used to set voice codec enable/disable.
//  Author:         shujing.dong
//  Note:
/*****************************************************************************/
BOOLEAN ATC_EnableVoiceCodec ( // If succeed, return SCI_TRUE, else return error value
    BOOLEAN is_enable          // SCI_TRUE: Enable,   SCI_FALSE: Disable
)
{
    //SCI_TRACE_LOW:"ATC_EnableVoiceCodec: is_enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_230_112_2_17_22_17_17_627,(uint8*)"d", is_enable);

    if(INVALID_HANDLE==hAtcVoiceRing)
    {
        hAtcVoiceRing    = AUDIO_CreateNormalHandle(hVoiceCodec,    SCI_NULL, hLAYER1,     SCI_NULL, AudioDummyNotifyCallback);
        SCI_ASSERT(INVALID_HANDLE != hAtcVoiceRing); /*assert verified*/
    }
    if (is_enable)
    {        
        AUDIO_Play (hAtcVoiceRing, 0);
    }
    else
    {
        AUDIO_Stop (hAtcVoiceRing);
    }

    return SCI_TRUE;
}
/*****************************************************************************/
//  Description : This function handle the AT+CCFC command 
//  command format:
//                 AT+CCFC=<reason>, <operate mode>, <forwarded number>,<number type>
//                         ,<no reply time>
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCFC )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_OPERATION_MODE_E       operation_mode; // holds the operation mode
    ATC_CCFC_REASON_E          ccfc_reason;
    ATC_CLASS_E                service_type;
    MN_SS_CODE_E               ss_code;
    MN_SS_BASIC_SERVICE_T      basic_service;
    BOOLEAN                    temp_flag;
    ATC_STATUS                 re_value = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    if(ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }
    // check if the parameter1 and parameter2 are present or not.
    if (!PARAM1_FLAG || !PARAM2_FLAG)
        return ERR_INVALID_INDEX;
    // get the call forwarded operation mode
    operation_mode = PARA2.num;
    // get the parameter call forwarded reason
    ccfc_reason    = PARA1.num;
    // get the MNSS ss_code
    ConvertCfCode(ccfc_reason, &ss_code);

    service_type = ATC_VOICE;
    // get the MNSS service code
    ConvertServiceCode(service_type, &(basic_service.service_code));
    basic_service.service_type = MN_SS_TELE_SERVICE;
    
    switch (operation_mode)
    {
        // deactivate the Call forwarded service
        case ATC_DISABLE:
        {
            MNSS_DeactivateSSEx(dual_sys, ss_code, basic_service);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_DEACTIVATE_SS_CNF,0xff,ATC_NO_DOMAIN);
            break;
        }

        case ATC_ENABLE:
        {
            MNSS_ActivateSSEx(dual_sys, ss_code, basic_service);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ACTIVATE_SS_CNF,0xff,ATC_NO_DOMAIN);
            break;
        }

        case ATC_QUERY_STATUS:
        {
            basic_service.service_type = MN_SS_ALL_SERVICE; 
            s_ss_context.cmd_id = AT_CMD_CCFC;      
            MNSS_InterrogateServiceEx(dual_sys, ss_code, basic_service);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_INTERROGATE_CNF,0xff,ATC_NO_DOMAIN);
            break;
        }

        // registrer the CF(call forwarded service)
        case ATC_REGISTRATION:
        {
            MN_SS_REGISTER_PARAM_U  register_param;
            uint8                   str_len = 0;
            
            if (!PARAM3_FLAG)
            {
                //SCI_TRACE_LOW:"ATC:CCFC reg fail1"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_320_112_2_17_22_17_17_628,(uint8*)"");
                re_value = ERR_INVALID_INDEX;
                break;
            }
            
            if(ATC_CMD_PARAM_TYPE_STRING != PARAM3_TYPE)
            {
                //SCI_TRACE_LOW:"ATC command's parameter type wrong"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_327_112_2_17_22_17_17_629,(uint8*)"");
                re_value = ERR_INVALID_INDEX;
                break;
            }
                        
            if (((char)*PARA3.str_ptr->str_ptr) == '+')
            {
                str_len = PARA3.str_ptr->str_len - 1;
            }
            else
            {
                str_len = PARA3.str_ptr->str_len;
            }
            
            if(str_len > 2*MN_MAX_ADDR_BCD_LEN)
            {
                //SCI_TRACE_LOW:"ATC:CCFC reg fail2"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_343_112_2_17_22_17_17_630,(uint8*)"");
                re_value = ERR_INVALID_INDEX;
                break;
            }

            if ((ATC_NO_REPLY == ccfc_reason) && !PARAM5_FLAG)
            {
                re_value =  ERR_INVALID_INDEX;
                break;
            }

            temp_flag = ATC_ConvertTeleStrToStruct(
                            &(register_param.cfnry_param.forward_num),
                            PARA3.str_ptr->str_ptr,
                            PARA3.str_ptr->str_len);
            if (!temp_flag)
            {
                //SCI_TRACE_LOW:"ATC:CCFC reg fail3"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_360_112_2_17_22_17_17_631,(uint8*)"");
                re_value = ERR_INVALID_INDEX;
                break;
            }

            if (ATC_NO_REPLY == ccfc_reason )
            {           
                register_param.cfnry_param.no_reply_time = (uint8)PARA5.num;
            }
                                    
            MNSS_RegisterSSInfoEx(dual_sys, ss_code, basic_service, register_param);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_REGISTER_SS_CNF,0xff,ATC_NO_DOMAIN);
            break;
        }
    
            // erase the CF service
        case ATC_ERASURE:       
        {
            MNSS_EraseSSInfoEx(dual_sys, ss_code, basic_service);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ERASE_SS_CNF,0xff,ATC_NO_DOMAIN);
            break;
        }

        default:
            break;
    }

    return(re_value);
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
}

//#ifdef _GSM_ONLY_
/*****************************************************************************/
//  Description : This function handle the AT+CLIP command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCLIP )
{
#ifndef _ULTRA_LOW_CODE_
    MN_SS_BASIC_SERVICE_T      service_info; 
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
       
    service_info.service_type = MN_SS_ALL_SERVICE;
    service_info.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                if((BOOLEAN)PARA1.num)
                {
                    atc_config_ptr->atc_config_nv.clip_flag = TRUE;
                }
                else
                {
                    atc_config_ptr->atc_config_nv.clip_flag = FALSE;
                }           
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            MNSS_InterrogateServiceEx(dual_sys, MN_SS_CLIP, service_info);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_INTERROGATE_CNF,0xff,ATC_NO_DOMAIN);
            s_ss_context.cmd_id = AT_CMD_CLIP;
            return S_ATC_DEFAULT_HANDLE;
        }

        default:
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

#ifdef _GSM_ONLY_
/*****************************************************************************/
//  Description : This function handle the AT+COLP command 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCOLP )
{
#ifndef _ULTRA_LOW_CODE_
    MN_SS_BASIC_SERVICE_T      service_info; 
       MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    service_info.service_type = MN_SS_ALL_SERVICE;
    service_info.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                if((BOOLEAN)PARA1.num)
                {
                    atc_config_ptr->atc_config_nv.colp_flag = TRUE;
                }
                else
                {
                    atc_config_ptr->atc_config_nv.colp_flag = FALSE;
                }
            }
            else
            {
                return ERR_INVALID_INDEX;
            }       
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            // interrogate the CLIP service status      
            MNSS_InterrogateServiceEx(dual_sys, MN_SS_COLP, service_info);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_INTERROGATE_CNF,0xff,ATC_NO_DOMAIN);
            s_ss_context.cmd_id = AT_CMD_COLP;
            return S_ATC_DEFAULT_HANDLE;
        }

        default:
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
#endif

/*****************************************************************************/
//  Description : This function handles the APP_MN_SETUP_COMPLETE_IND,
//  APP_MN_SETUP_COMPLETE_CNF signal to response the function 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSetupCompleteSignal(
                        ATC_CONFIG_T             *atc_config_ptr,
                        xSignalHeaderRec *sig_ptr
                        )
{
#ifdef _GSM_ONLY_  //CR171132 for 6530 codesize cutdown
    ATC_TELE_TYPE_E     tel_num_type;
    uint8               tel_str[MN_MAX_ADDR_BCD_LEN*2 + 1] = {0};
#endif
    uint8               call_index = ATC_INVALID_CALLID;
    MN_DUAL_SYS_E       dual_sys = MN_DUAL_SYS_1;       

    switch(sig_ptr->SignalCode)
    {
        case APP_MN_SETUP_COMPLETE_CNF:
        {
            dual_sys = ((APP_MN_SETUP_COMPLETE_CNF_T*)sig_ptr)->dual_sys;
            call_index = GetIndexOfCallContext(((APP_MN_SETUP_COMPLETE_CNF_T*)sig_ptr)->call_id,dual_sys);     
            break;
        }
            
        case APP_MN_SETUP_COMPLETE_IND:
        {
            dual_sys = ((APP_MN_SETUP_COMPLETE_IND_T*)sig_ptr)->dual_sys;
            call_index = GetIndexOfCallContext(((APP_MN_SETUP_COMPLETE_IND_T*)sig_ptr)->call_id,dual_sys);
            break;
        }
            
        default:
            break;
    }

    SCI_ASSERT(ATC_INVALID_CALLID!=call_index); /*assert verified*/

    g_calls_state[dual_sys].call_context[call_index].call_entity_state = ATC_CALL_ACTIVE_STATE;
    
    if(!CheckPresentWaitCall(&g_calls_state[dual_sys], &call_index))
    {
        // no wait call, check & kill call exp timer
        if (SCI_IS_TIMER_ACTIVE(P_ATC,ATC_CALL_EXP_IND,&T_ATC_CALL_EXP_IND))
        {
            SCI_KILL_TIMER(P_ATC,ATC_CALL_EXP_IND,
                &T_ATC_CALL_EXP_IND,"T_ATC_CALL_EXP_IND");
            g_atc_expire_call_id = 0xff;           
            g_atc_expire_ind = FALSE;
        }
    }
    
    atc_config_ptr->cpas = ATC_CPAS_STATUS_PROGRESS;
    
    // handle the at+chld commands
    if (ATC_INVALID_OPERATE != atc_config_ptr->ss_operate)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
        atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
        return S_ATC_DEFAULT_HANDLE;
    }

    if (!atc_config_ptr->audio_flag && g_atc_call_flag)
    {
        AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
        AUDIO_SetVolume(atc_config_ptr->speaker_volume);
        ATC_EnableVoiceCodec(TRUE);
        atc_config_ptr->audio_flag = TRUE;
    }
    
    //Send the unsolicited +WIND: 9         
    if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT8))
    {
        sprintf((char*)g_rsp_str, "%s: 9",g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
    }
#if 0
#ifndef _ULTRA_LOW_CODE_
    //if call is set up by STK
    if(TRUE == g_is_stk_call[dual_sys]) 
    {
        ATC_StkProcessCallSignals(dual_sys,TRUE);
    }
#endif
#endif
#ifdef _GSM_ONLY_
    //if COLP  is enable ,show the connected phone number.
    if (atc_config_ptr->atc_config_nv.colp_flag
        && APP_MN_SETUP_COMPLETE_CNF == sig_ptr->SignalCode)
    {   
        if (((APP_MN_SETUP_COMPLETE_CNF_T*)sig_ptr)->connected_num_present)
        {
            ATC_ConvertCallingNumToStr(&(((APP_MN_SETUP_COMPLETE_CNF_T*)sig_ptr)->connected_num), &tel_num_type, tel_str);
        }
        else
        {
            tel_str[0] = 129;  //0x81,tele number type
            SCI_MEMCPY(&(tel_str[1]),g_tel_str, 2*MN_MAX_ADDR_BCD_LEN);
        }                   
        sprintf((char*)g_rsp_str, "%s: \"%s\",%d ",
        g_atc_info_table[AT_CMD_COLP].cmd_name->str_ptr, &(tel_str[1]),tel_str[0]);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }   
#endif
#ifndef _MUX_REDUCED_MODE_      
    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK,0);
#endif
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+CCWA command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCWA )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_OPERATION_MODE_E       operation_code; // holds the CALL WAITING SS operation code
    MN_SS_BASIC_SERVICE_T      basic_service;
    ATC_STATUS                 status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (!PARAM1_FLAG)
            {
                status =  ERR_INVALID_INDEX;
                break;
            }
        
            atc_config_ptr->ccwa_flag = (uint8)PARA1.num;
            if (!PARAM2_FLAG)
            {
                // return the result OK 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            }
            else
            {
                operation_code = PARA2.num;
                basic_service.service_type = MN_SS_TELE_SERVICE;
                basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES; 
                
                switch (operation_code)
                {
                    // deactivate the Call forwarded service
                    case ATC_DISABLE:
                    {
                        MNSS_DeactivateSSEx(dual_sys, MN_SS_CW, basic_service);
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_DEACTIVATE_SS_CNF,0xff,ATC_NO_DOMAIN);
                        break;
                    }

                    case ATC_ENABLE:
                    {
                        MNSS_ActivateSSEx(dual_sys, MN_SS_CW, basic_service);
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ACTIVATE_SS_CNF,0xff,ATC_NO_DOMAIN);
                        break;
                    }

                    case ATC_QUERY_STATUS:
                    {
                        basic_service.service_type = MN_SS_ALL_SERVICE;             
                        MNSS_InterrogateServiceEx(dual_sys, MN_SS_CW, basic_service);
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_INTERROGATE_CNF,0xff,ATC_NO_DOMAIN);
                        s_ss_context.cmd_id = AT_CMD_CCWA;
                        break;
                    }

                    default:
                      return ERR_INVALID_INDEX;
                }
            }
            status = S_ATC_DEFAULT_HANDLE;
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str,"%s: %d",g_atc_info_table[AT_CMD_CCWA].cmd_name->str_ptr,atc_config_ptr->ccwa_flag);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char*)g_rsp_str, "%s: (0-1),(0-2),(1,2,4,128)",g_atc_info_table[AT_CMD_CCWA].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
            break;
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
} 

/*****************************************************************************/
//  Description : This function handle the AT+CAOC command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCAOC )
{
#ifndef _ULTRA_LOW_CODE_
    switch (ATC_GET_CMD_TYPE)
    {
        // AT+CAOC
        case ATC_CMD_TYPE_EXECUTE:
        {
            sprintf((char*)g_rsp_str, "%s: %06x", g_atc_info_table[AT_CMD_CAOC].cmd_name->str_ptr,
                atc_config_ptr->aoc_mode);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

            // AT+CAOC?
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                atc_config_ptr->aoc_mode = (uint8)PARA1.num;
                //SCI_TraceLow:"ATC: user set the AOC mode"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_727_112_2_17_22_17_18_632,(uint8*)"");
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            break;
        }

        default:
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_CHV_FUNC_CNF signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessChvCnf(
                  ATC_CONFIG_T                   *atc_config_ptr,
                  APP_MN_SIM_PIN_FUNC_CNF_T      *sig_ptr
                  )
{
    MNSIM_PIN_REQ_RESULT_E      chv_status;
    MN_DUAL_SYS_E     dual_sys = sig_ptr->dual_sys;
    
#ifdef _MUX_REDUCED_MODE_
    uint32 i = 0;
    BOOLEAN is_all_sim_received_ind = TRUE;
    
    g_received_sim_ind[dual_sys] = TRUE;
    if(FALSE == g_sent_sind4)
    {
        for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
        {
            if(FALSE == g_received_sim_ind[i])
            {
                is_all_sim_received_ind = FALSE;
                break;
            }
        }
        
        if(is_all_sim_received_ind)
        {
            g_sent_sind4 = TRUE;
            if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT3))
            {
                sprintf((char*)g_rsp_str, "%s: 4",g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            }
        }
    }
#endif

    chv_status = sig_ptr->pin_operate_result;

    switch (chv_status)
    {
        case MNSIM_PIN_REQ_OK:
        {
            switch (sig_ptr->pin_function_type)
            {
                case MNSIM_PIN_DISABLE:
                {
                    atc_config_ptr->pin1_enable_flag[dual_sys] = FALSE;
                    break;
                }

                case MNSIM_PIN_ENABLE:
                {
                    atc_config_ptr->pin1_enable_flag[dual_sys] = TRUE;
                    break;
                }
            
                    //handle the cnf signal of AT+CACM,AT+CAMM,AT+CPUC
                case MNSIM_PIN_VERIFY:
                {
                    switch(atc_config_ptr->operate)
                    {
                        case ATC_PIN_RESET_ACM_UNIT:
                        {
                            MNSIM_InitAcmUnitEx(dual_sys);
                            break;
                        }
                    
                        case ATC_PIN_SET_ACM_MAX:
                        {
                            MNSIM_UpdateAcmMaxEx(dual_sys, atc_config_ptr->acm_max);
                            break;
                        }
                    
                        case ATC_PIN_SET_PPU:
                        {
                            //The result will be shown in ATC_ProcessPuctCnf()
                            MNSIM_UpdatePuctEx(dual_sys, atc_config_ptr->ppc);
                            return S_ATC_SUCCESS;
                        }
                    
                        default:
                            break;
                    }
                    
                    break;
                }
                
            default:
                break;
            }
            
        atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_READY;
        atc_config_ptr->cpin_flag[dual_sys] = FALSE;
        
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
        break;
    }

        case MNSIM_PIN_ERROR_WITH_BLOCKED:
        {
            if(sig_ptr->pin_num == MNSIM_PIN1)
            {
                atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_PUK1;
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_INCORRECT_PASSWORD);
            }
            else
            {
                atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_PUK2;
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_INCORRECT_PASSWORD);
            }
       
            break;
        }
        
        default:
        {
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,ERR_INCORRECT_PASSWORD);
            break;
        }
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_PUCT_INFO_CNF signal 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/

extern ATC_STATUS ATC_ProcessPuctCnf(
                        ATC_CONFIG_T             *atc_config_ptr,
                        APP_MN_PUCT_INFO_CNF_T *sig_ptr
                        )
{
#if 0 //CR171132 for 6530 codesize cutdown
    uint8 temp_str[MN_MAX_CURRENCY_CODE_NUM*4+1]={0};
#endif    
    if ( sig_ptr->is_ok )
    {
        switch(atc_config_ptr->operate)
        {
           #if 0
            //if get ppu(price per unit),show it in pc.
            case ATC_PIN_GET_PPU:
            {
                if(strcmp((char*)atc_config_ptr->cscs, "UCS2") == 0)
                {
                    SCI_MEMSET(&temp_str[0], 0, MN_MAX_CURRENCY_CODE_NUM*4+1);
                    ConvertStr2Hex(sig_ptr->puct_info.currency_code,3, &temp_str[0]);
                    sprintf((char*)g_rsp_str, "%s: \"%s\", \"0x%x\"",g_atc_info_table[AT_CMD_CPUC].cmd_name->str_ptr
                    ,&temp_str[0],sig_ptr->puct_info.eppu);
                }
                else
                {
                    sprintf((char*)g_rsp_str, "%s: \"%s\", \"0x%x\"",g_atc_info_table[AT_CMD_CPUC].cmd_name->str_ptr
                        ,sig_ptr->puct_info.currency_code,sig_ptr->puct_info.eppu);
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            #endif
            //if read ppu(price per unit),return ok.
            case ATC_PIN_SET_PPU:
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK ,0);
                break;
            }
        
            default:
                break;
        }
    }
    else
    {
        sprintf((char*)g_rsp_str, "+CME ERROR: 16");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    return S_ATC_DEFAULT_HANDLE;
}
#endif /* _ULTRA_LOW_CODE_ */

/*****************************************************************************/
//  Description : This function handle the AT+CHLD command. This command is used 
//                to request the call-related supplement service. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCHLD )
{
#ifndef _ULTRA_LOW_CODE_
    int32         operation_code;
    uint8         xcall_id;
    MN_CALL_DISCONNECT_CALL_CAUSE_E  cause;

    ATC_STATUS    status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    
    if(!PARAM1_FLAG)
    {
        return ERR_INVALID_INDEX;
    }

    operation_code = PARA1.num;

    if (ATC_REL_SPECIFIC_X_CALL_BASE > operation_code)
    {
        switch (operation_code)
        {
            // release all of the held calls or set the UDUB
            case ATC_REL_HELD_CALLS_OR_SET_UDUB:
            {
                status = HandleChldUdub(atc_config_ptr);
                if(status == S_ATC_DEFAULT_HANDLE)
                {
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_CALL_DISCONNECTED_IND, 0xff,ATC_DOMAIN_CS);
                }
                break;
            }

                // release all active calls and accept waiting call or held calls
            case ATC_REL_ACTIVE_CALLS_AND_ACCEPT_OTHERS:
            {
                status = HandleChldRelAcceptOthers(atc_config_ptr);
                break;
            }
        
                // Place all active calls on hold and accept waiting call or held call
            case ATC_HOLD_CALLS_AND_ACCEPT_OTHERS:
            {
                status = HandleChldHoldAcceptOthers(atc_config_ptr);
                break;
            }

            case ATC_ADD_HELD_TO_CONVERSATION:
            {
                status = HandleChldMultCall(atc_config_ptr);
                if(status == S_ATC_DEFAULT_HANDLE)
                {
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_BUILD_MPTY_CNF,0xff,ATC_DOMAIN_CS);
                }
                break;
            }

            case ATC_ECT:
            {
                status = HandleEctCall(atc_config_ptr, dual_sys);
                break;
            }
            
            default:
            {
                status = ERR_INVALID_INDEX;
                break;
            }
        }
    }
    // the operation code is 1X
    else if ((ATC_REL_SPECIFIC_X_CALL_BASE < operation_code) &&
             (ATC_HOLD_CALLS_EXCEPT_X_BASE > operation_code))
    {
        // check the call id it can not exceed MN_CALL_MAX_CALL_NUM 
        if ((operation_code - ATC_REL_SPECIFIC_X_CALL_BASE) < MN_CALL_MAX_CALL_NUM)
        {
            // get the specific call id
            xcall_id = (uint8)(operation_code -ATC_REL_SPECIFIC_X_CALL_BASE - 1);
            //atc will move call_context when mpty call is deleted, but layer4 doesn't move. then the call id is right to layer4. according to 22.030
            {
                if(g_calls_state[dual_sys].call_context[xcall_id].call_entity_state == ATC_CALL_INCOMING_STATE)
                {
                    cause = MN_CAUSE_USER_BUSY;
                }
                else
                {
                    cause = MN_CAUSE_NORMAL_CLEARING;
                }
#ifdef MS_OAPI_SUPPORT
                MNCALL_DisconnectCallEx(dual_sys, (uint8)xcall_id, PNULL);
#else
                MNCALL_DisconnectCallEx(dual_sys, (uint8)xcall_id, cause, PNULL);
#endif
                atc_config_ptr->ss_operate = ATC_REL_SPECIFIC_X_CALL_BASE;
            }
        }
        else
        {
            status = ERR_OPERATION_NOT_ALLOWED;
        }
    }
    else if ((20 < operation_code) && (operation_code <= 27))
    {
        xcall_id = (uint8)(operation_code - ATC_HOLD_CALLS_EXCEPT_X_BASE - 1);
        if (ATC_CALL_ACTIVE_STATE == g_calls_state[dual_sys].call_context[xcall_id].call_entity_state)
        {           
            MNCALL_SplitMPTYEx(dual_sys, xcall_id); 
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SPLIT_MPTY_CNF,0xff,ATC_DOMAIN_CS);
            atc_config_ptr->ss_operate = ATC_REL_SPECIFIC_X_CALL_BASE;
        }
        else
        {
            status = ERR_OPERATION_NOT_ALLOWED;
        }
    }
    else
    {
        status = ERR_OPERATION_NOT_ALLOWED;
    }
    return status;
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handle the AT+CLCC command. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCLCC )
{
#ifndef _ULTRA_LOW_CODE_
    int32               i;
    ATC_STATUS          status = ERR_INVALID_INDEX; 
    uint16              offset = 0;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE)
    {
        //If there is no call,return +CLCC:
        if (0==g_calls_state[dual_sys].call_amount)
        {
            sprintf((char*)g_rsp_str,"%s: ", g_atc_info_table[AT_CMD_CLCC].cmd_name->str_ptr);
        }
        //List the information of existing calls
        for (i=0; i<g_calls_state[dual_sys].call_amount; i++)
        {
            uint8      call_id  = (uint8)(g_calls_state[dual_sys].call_context[i].call_id + 1);
            BOOLEAN    dir_flag = g_calls_state[dual_sys].call_context[i].direction;
            uint8      state    = ATC_GetCallState(dual_sys,(uint8)i);
            //uint8      mode     = (uint8)call_context.call_entity_context[i].call_mode;
            uint8      mpty     = (uint8)g_calls_state[dual_sys].call_context[i].mpty_state;
            uint8      cr_val;                 // holds the atc_config_ptr->S3_reg value
            uint8      lf_val;                 // holds the atc_config_ptr->S4_reg value
            
            cr_val = atc_config_ptr->s3_reg;
            lf_val = atc_config_ptr->s4_reg;
            if(i==(g_calls_state[dual_sys].call_amount-1))
            {
                sprintf((char*)&g_rsp_str[offset],"%s: %d,%d,%d,%d,%d,\"%s\",%d",
                    g_atc_info_table[AT_CMD_CLCC].cmd_name->str_ptr,
                    call_id,dir_flag,state,0,mpty,&(g_calls_state[dual_sys].call_context[i].party_number[1]),g_calls_state[dual_sys].call_context[i].party_number[0]);
            }
            else
            {
                offset += sprintf((char*)&g_rsp_str[offset],"%s: %d,%d,%d,%d,%d,\"%s\",%d%c%c",
                    g_atc_info_table[AT_CMD_CLCC].cmd_name->str_ptr,
                    call_id,dir_flag,state,0,mpty,&(g_calls_state[dual_sys].call_context[i].party_number[1]),
                    g_calls_state[dual_sys].call_context[i].party_number[0],cr_val,lf_val);
                //SCI_TRACE_LOW:"ATC: OFFSET %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1103_112_2_17_22_17_18_633,(uint8*)"d", offset);
            }
            //temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];  
        }
        ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);        
        status = S_ATC_SUCCESS;
    }
    else
    {
        status = ERR_OPERATION_NOT_ALLOWED;
    }
    return status;
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the CUSD request 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleUssdReq(
                                                    MN_DUAL_SYS_E dual_sys,
                               BOOLEAN                param1_flag,
                               int32                  para1_num,
                               BOOLEAN                param2_flag,
                               ATC_CUR_PARAM_STRING_T *ptr_para2_str,
                               BOOLEAN                param3_flag,
                               int32                  para3_num
                               )
{
    MN_SS_DCS_E data_code_scheme = MN_SS_DEFAULT_DCS_VALUE;
    ATC_STATUS  status           = S_ATC_SUCCESS;

    if (!param1_flag)
    {
        //SCI_TRACE_LOW:"ATC:USSD,PARAM1_FLAG = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1140_112_2_17_22_17_18_634,(uint8*)"");
        return ERR_OPERATION_NOT_ALLOWED;
    }
    
    if (param3_flag)
    {
        switch(para3_num)
        {
            case MN_SS_DEFAULT_DCS_VALUE:
            {
                data_code_scheme = MN_SS_DEFAULT_DCS_VALUE;
                break;
            }
       
            case MN_SS_USC2_NO_CLASS_DCS:
            {
                data_code_scheme = MN_SS_USC2_NO_CLASS_DCS;
                break;
            }
        
            case MN_SS_8BIT_NO_CLASS_DCS:
            {
                data_code_scheme = MN_SS_8BIT_NO_CLASS_DCS;
                break;
            }
        
            default:
            {
                data_code_scheme = para3_num;
                break;
            }
        }                   
    }
    
    switch(para1_num)
    {
        case AT_CUSD_DISABLE:
        {
            s_cusd_presentation = AT_CUSD_DISABLE;
            if (param2_flag)
            {
                if (ptr_para2_str->str_len > MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE)
                {
                    return ERR_INVALID_INDEX;
                }

                if(FALSE == s_cusd_isresponse)
                {
                    //Send the original requst to MN.
                    if(ERR_MNSS_NO_ERR != MNSS_UserOriginateUSSDEx(dual_sys,FALSE, data_code_scheme,
                        ptr_para2_str->str_len,ptr_para2_str->str_ptr))
                    {
                        return S_ATC_FAIL;
                    }
                    else
                    {
                        s_cusd_isresponse = TRUE;
                    }                                   
                }
                else
                {
                    //Send the requst to MN.
                    if(ERR_MNSS_NO_ERR!=MNSS_ResponseUSSDEx(dual_sys, data_code_scheme,
                        ptr_para2_str->str_len, ptr_para2_str->str_ptr))
                        return S_ATC_FAIL;
                }
            }
            break;
        }
    
        case AT_CUSD_ENABLE:
        {
            s_cusd_presentation = AT_CUSD_ENABLE;

            if (param2_flag)
            {
                if (ptr_para2_str->str_len > MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE)
                {
                    return ERR_INVALID_INDEX;
                }

                if(FALSE == s_cusd_isresponse)
                {
                    //Send the original requst to MN.
                    //SCI_TRACE_LOW("ATC:MNSS_UserOriginateUSSDEx");
                    if(ERR_MNSS_NO_ERR != MNSS_UserOriginateUSSDEx(dual_sys, FALSE,data_code_scheme,
                        ptr_para2_str->str_len,ptr_para2_str->str_ptr))
                        return S_ATC_FAIL;
                    
                    s_cusd_isresponse = TRUE;                   
                }
                else
                {
                    //Send the requst to MN.
                    //SCI_TRACE_LOW:"ATC:MNSS_UserOriginateUSSDEx"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1234_112_2_17_22_17_19_635,(uint8*)"");
                    if(ERR_MNSS_NO_ERR!=MNSS_ResponseUSSDEx(dual_sys, data_code_scheme,
                        ptr_para2_str->str_len, ptr_para2_str->str_ptr))
                        return S_ATC_FAIL;
                }
            }
            break;
        }

        case AT_CUSD_CANCEL:
        {
            s_cusd_isresponse = FALSE;
            MNSS_RelActivingServiceEx(dual_sys);
            break;
        }

        default:
            return ERR_INVALID_INDEX;
    }
    return status;
}
#endif/*_ULTRA_LOW_CODE_*/

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the CUSD request 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleUssdInd(
                                ATC_CONFIG_T                *atc_config_ptr,
                                MN_SS_DCS_E                 dcs,
                                uint8                       *input_ussd_str,
                                uint16                      input_ussd_len,
                                uint8                       *out_ussd_str
                                )
{
    uint8  temp_str[MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE + 1] = {0};
    uint16 temp_len = 0;
    
    if(input_ussd_len  > MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE)
    {
        input_ussd_len = MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE;
    }
    
    //SCI_TRACE_LOW:"ATC: unknown  cscs %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1278_112_2_17_22_17_19_636,(uint8*)"s", (char*)atc_config_ptr->cscs);
    
    switch( dcs )
    {
        //default sms aphabet.
        case MN_SS_DEFAULT_DCS_VALUE:
        {
            temp_len = ATC_Default2Ascii(input_ussd_str, temp_str, input_ussd_len);
            
            if(strcmp((char*)atc_config_ptr->cscs, "UCS2") == 0)
            {   
                if(!ATC_ConvertAsciiToUcs2Hex(temp_str,temp_len, out_ussd_str))
                {
                    //SCI_TRACE_LOW:"ATC: ATC_ConvertAsciiToUcs2Hex return false"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1291_112_2_17_22_17_19_637,(uint8*)"");
                    return S_ATC_FAIL;
                }
            }
            else if(strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
            {
                ConvertBinToHex(temp_str,temp_len, out_ussd_str);
            }
            else
            {
                SCI_ASSERT(temp_len <= sizeof(temp_str));/*assert verified check NULL pointer*/
                SCI_MEMCPY(out_ussd_str,(char*)temp_str,temp_len);
            }
        
            break;
        }
    
        case MN_SS_8BIT_NO_CLASS_DCS:
        {
            temp_len = input_ussd_len;
            SCI_MEMCPY(temp_str,input_ussd_str,input_ussd_len);
            
            if(strcmp((char*)atc_config_ptr->cscs, "UCS2") == 0)
            {   
                if(!ATC_ConvertAsciiToUcs2Hex(temp_str,temp_len, out_ussd_str))
                {
                    return S_ATC_FAIL;
                }
            }
            else if(strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
            {
                ConvertBinToHex(temp_str,temp_len, out_ussd_str);
            }
            else
            {
                SCI_MEMCPY(out_ussd_str,(char*)temp_str,temp_len);
            }
        
            break;
        }
    
        //USC2 aphabet
        case MN_SS_USC2_NO_CLASS_DCS:
        {
        //change to GB string,and return it.
    #ifdef GB_STRING
            uint8   *pStr = temp_str;
            uint16  i = 0;
                
            while(i < input_ussd_len)
            {
                uint16 ucs2 = ((input_ussd_str[i] & 0x00FF) << 8) 
                        + (input_ussd_str[i + 1] & 0x00FF);
                pStr = SCI_TransUCS2ToGB2312(ucs2, pStr);
                i = i + 2;
            }   
    #else
            //Return USC2 string
            SCI_MEMCPY(temp_str,input_ussd_str,input_ussd_len);
    #endif          
            
            temp_len = input_ussd_len;
            
            if((strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
               ||(strcmp((char*)atc_config_ptr->cscs, "UCS2") == 0))
            {
                ConvertBinToHex(temp_str,temp_len, out_ussd_str);
            }
            else
            {
                SCI_MEMCPY(out_ussd_str,(char*)temp_str,temp_len);
            }
            break;
        }
            
        default:
            break;
    }
    return S_ATC_SUCCESS;
}
#endif/*_ULTRA_LOW_CODE_*/

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_USSD_IND 
//                       signal to response the function 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessUssdInd(
                        ATC_CONFIG_T             *atc_config_ptr,
                        APP_MN_USSD_IND_T *sig_ptr
                        )
{
    uint8  *ussd_str = PNULL;
    
    sig_ptr->ussd_data.ussd_str[sig_ptr->ussd_data.str_len] = '\0';
    //SCI_TRACE_LOW:"ATC: ussd ind ussd len is %d, str is %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1386_112_2_17_22_17_19_638,(uint8*)"ds",sig_ptr->ussd_data.str_len,sig_ptr->ussd_data.ussd_str);
    
    //if disable to present result,return. 
    if(AT_CUSD_DISABLE == s_cusd_presentation)
        return S_ATC_FAIL;
        
    ussd_str = SCI_ALLOC_BASE(sig_ptr->ussd_data.str_len*4 + 1);
    
    if(ussd_str != PNULL)   
    {
        SCI_MEMSET(ussd_str,'\0',(sig_ptr->ussd_data.str_len*4 + 1));
        
        if(S_ATC_SUCCESS == HandleUssdInd(atc_config_ptr,
                    sig_ptr->ussd_data.data_code_scheme,
                    sig_ptr->ussd_data.ussd_str,
                    sig_ptr->ussd_data.str_len,
                    ussd_str))
        {
            switch (sig_ptr->ussd_data.data_code_scheme)
            {
                #if 0
                //default sms aphabet.
                case MN_SS_DEFAULT_DCS_VALUE:
                case MN_SS_8BIT_NO_CLASS_DCS:
                {
                    sprintf((char*)g_rsp_str, "%s: 1,\"%s\",%d",
                            g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr,ussd_str,sig_ptr->ussd_data.data_code_scheme);
                        break;
                }

                //USC2 aphabet
                case MN_SS_USC2_NO_CLASS_DCS:
                {
                #ifdef GB_STRING
                    sprintf((char*)g_rsp_str, "%s: 1,\"%s\",%d",
                        g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr,ussd_str,sig_ptr->ussd_data.data_code_scheme);
                #else
                    sprintf((char*)g_rsp_str, "%s: 1,\"%s\",%d",
                        g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr,ussd_str,sig_ptr->ussd_data.data_code_scheme);
                #endif
                    break;
                }
                #endif
                default:
                    break;
            }
            
            //SCI_TRACE_LOW:"ATC: ussd_str is %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1433_112_2_17_22_17_19_639,(uint8*)"s",ussd_str);
        }
        else
        {
            SCI_FREE(ussd_str);
            return S_ATC_FAIL;
        }
        
        SCI_FREE(ussd_str);
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: ussd_str is pnull"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1445_112_2_17_22_17_19_640,(uint8*)"",ussd_str);
        return S_ATC_FAIL;
    }
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_USSD_NOTIFY_IND  signal   
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessUssdNotifyInd(
                        ATC_CONFIG_T             *atc_config_ptr,
                        APP_MN_USSD_NOTIFY_IND_T *sig_ptr
                        )
{
    uint8  *ussd_str = PNULL;
 
    sig_ptr->ussd_data.ussd_str[sig_ptr->ussd_data.str_len] = '\0';
    //SCI_TRACE_LOW:"ATC: ussd notify ussd len is %d, str is %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1466_112_2_17_22_17_19_641,(uint8*)"ds",sig_ptr->ussd_data.str_len,sig_ptr->ussd_data.ussd_str);

    //if disable to present result,return. 
    if(AT_CUSD_DISABLE == s_cusd_presentation)
    {
        s_cusd_notification = FALSE; //Set the flag for receive RelInd later
        return S_ATC_FAIL;
    }
    
    ussd_str = SCI_ALLOC_BASE(sig_ptr->ussd_data.str_len*4 + 1);
    
    if(ussd_str != PNULL)
    {
        SCI_MEMSET(ussd_str,'\0',(sig_ptr->ussd_data.str_len*4 + 1));

        if(S_ATC_SUCCESS == HandleUssdInd(atc_config_ptr,
                    sig_ptr->ussd_data.data_code_scheme,
                    sig_ptr->ussd_data.ussd_str,
                    sig_ptr->ussd_data.str_len,
                    ussd_str))
        {
            switch (sig_ptr->ussd_data.data_code_scheme)
            {
            #if 0
                //default sms aphabet.
                case MN_SS_DEFAULT_DCS_VALUE:   
                case MN_SS_8BIT_NO_CLASS_DCS:
                {
                    sprintf((char*)g_rsp_str, "%s: 1,\"%s\",%d",
                        g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr,ussd_str,sig_ptr->ussd_data.data_code_scheme);
                    break;
                }

                //USC2 aphabet
                case MN_SS_USC2_NO_CLASS_DCS:
                {
                #ifdef GB_STRING
                    sprintf((char*)g_rsp_str, "%s: 1,\"%s\",%d",
                        g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr,ussd_str,sig_ptr->ussd_data.data_code_scheme);
                #else
                    sprintf((char*)g_rsp_str, "%s: 1,\"%s\",%d",
                        g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr,ussd_str,sig_ptr->ussd_data.data_code_scheme);
                #endif
                    break;
                }
             #endif
                default:
                    break;
            }
            //SCI_TRACE_LOW:"ATC: ussd_str is %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1515_112_2_17_22_17_19_642,(uint8*)"s",ussd_str);
        }
        else
        {
            SCI_FREE(ussd_str);
            return S_ATC_FAIL;
        }
        
        SCI_FREE(ussd_str);
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: ussd_str is pnull"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1527_112_2_17_22_17_19_643,(uint8*)"",ussd_str);
        return S_ATC_FAIL;
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_USSD_SERVICE_CNF signal 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessUssdServiceCnf(
                        ATC_CONFIG_T             *atc_config_ptr,
                        APP_MN_USSD_SERVICE_CNF_T *sig_ptr
                        )
{
    uint8  *ussd_str = PNULL;
    uint8 m_value = AT_CUSD_NOSUPPORT;
    
    sig_ptr->ussd_data.ussd_str[sig_ptr->ussd_data.str_len] = '\0';
    //SCI_TRACE_LOW:"ATC: ussd service cnf ussd len is %d, str is %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1550_112_2_17_22_17_19_644,(uint8*)"ds",sig_ptr->ussd_data.str_len,sig_ptr->ussd_data.ussd_str);

    s_cusd_isresponse = FALSE;
    //if disable to present result,return. 
    if(AT_CUSD_DISABLE == s_cusd_presentation)
    {
        return S_ATC_FAIL;
    }
    
    switch(sig_ptr->result_type)
    {
        case MN_SS_OPERATION_SUCC:  // operation success
        {
            m_value = AT_CUSD_NOSTEPMORE;
            break;
        }
        case MN_SS_OPERATION_ERROR:  // operation error
        case MN_SS_OPERATION_REJECT:  // operation reject by network
        default:
        {
            sprintf((char*)g_rsp_str, "%s: 2",g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);                
            return S_ATC_DEFAULT_HANDLE;
        }
    }
    if(0 == sig_ptr->ussd_data.str_len)
    {
        sprintf((char*)g_rsp_str, "%s: 2",g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);                
        return S_ATC_DEFAULT_HANDLE;
    }

    ussd_str = SCI_ALLOC_BASE(sig_ptr->ussd_data.str_len*4 + 1);
    
    if(ussd_str != PNULL)
    {
        SCI_MEMSET(ussd_str,'\0',(sig_ptr->ussd_data.str_len*4 + 1));

        if(S_ATC_SUCCESS == HandleUssdInd(atc_config_ptr,
                    sig_ptr->ussd_data.data_code_scheme,
                    sig_ptr->ussd_data.ussd_str,
                    sig_ptr->ussd_data.str_len,
                    ussd_str))
        {
            switch (sig_ptr->ussd_data.data_code_scheme)
            {
                //default sms aphabet.
                case MN_SS_DEFAULT_DCS_VALUE:   
                case MN_SS_8BIT_NO_CLASS_DCS:
                {
                    sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%d",g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr,
                        m_value,ussd_str,sig_ptr->ussd_data.data_code_scheme);
                    break;
                }

                //USC2 aphabet
                case MN_SS_USC2_NO_CLASS_DCS:
                {
                #ifdef GB_STRING
                    sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%d",g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr,
                        m_value,ussd_str,sig_ptr->ussd_data.data_code_scheme);
                #else
                    sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%d",g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr,
                        m_value,ussd_str,sig_ptr->ussd_data.data_code_scheme);
                #endif
                    
                    break;
                }
                default:
                    break;
            }
            //SCI_TRACE_LOW:"ATC: ussd_str is %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1621_112_2_17_22_17_19_645,(uint8*)"s",ussd_str);
        }
        else
        {
            SCI_FREE(ussd_str);
            sprintf((char*)g_rsp_str, "%s: 2",g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);                
            return S_ATC_DEFAULT_HANDLE;
        }
        
        SCI_FREE(ussd_str);
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: ussd_str is pnull"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_1635_112_2_17_22_17_20_646,(uint8*)"",ussd_str);
        sprintf((char*)g_rsp_str, "%s: 2",g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);                
        return S_ATC_DEFAULT_HANDLE;
    }
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}
#endif /* _ULTRA_LOW_CODE_ */

/*****************************************************************************/
//  Description : This function handles the AT+CCUG command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:MN havent support the interface!!
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCUG )
{
    return S_ATC_FAIL;
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function convert the ATC CF(call forwarded) reason code
//                to the MNSS's code
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void ConvertCfCode(
            ATC_CCFC_REASON_E     ccfc_reason,   // the call forwarded reason
            MN_SS_CODE_E          *ss_code_ptr  // the SS code
            )
{

    switch (ccfc_reason)
    {
        // call forwarded unconditional
        case ATC_UNCONDITIONAL:
        {
            *ss_code_ptr = MN_SS_CFU;
            break;
        }
        
        // call forwared when the mobile phone is busy
        case ATC_MOBILE_BUSY:
        {
            *ss_code_ptr = MN_SS_CFB;
            break;
        }
    
        // call forwarded when on reply
        case ATC_NO_REPLY:
        {
            *ss_code_ptr = MN_SS_CFNRY;
            break;
        }
    
        // call forwarded when the mobile subscriber not reachable
        case ATC_NOT_REACHABLE:
        {
            *ss_code_ptr = MN_SS_CFNRC;
            break;
        }
    
        // all call forwarded
        case ATC_ALL_CALL_FORWARDED:
        {
            *ss_code_ptr = MN_SS_ALL_FORWARD;
            break;
        }
    
        // all conditional forwarded
        case ATC_ALL_CONDITIONAL_FORWARDED:
        {
            *ss_code_ptr = MN_SS_ALL_COND_FORWARD;
            break;
        }
    
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : This function convert the ATC CF(call forwarded) reason code
//                to the MNSS's code
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void ConvertServiceCode(
            ATC_CLASS_E     service_type,   // the service type
            uint16          *service_code_ptr  // the SS code
            )
{

    switch (service_type)
    {
        case ATC_VOICE:
        {
            *service_code_ptr = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            break;
        }
    
        case ATC_DATA:
        {
            *service_code_ptr = MN_SS_ALL_DATA_TELESERVICES;
            break;
        }
    
        case ATC_FAX:
        {
            *service_code_ptr = MN_SS_ALL_FACSIMIL_TRANS_SERVICES;
            break;
        }
    
        case ATC_ALL_CLASSES:
        {
            *service_code_ptr = MN_SS_TELESERVICES_EXP_SMS;
            break;
        }
    
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_FORWARD_IND signal and 
//                sends the +CSSI: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessForwardInd(
            ATC_CONFIG_T              *atc_config_ptr,
            APP_MN_CALL_FORWARD_IND_T *sig_ptr
            )
{
    if (atc_config_ptr->cssi_flag)
    {
        switch (sig_ptr->forward_type)
        {
            case MN_SS_INCOMING_IS_FORWARED:
            {
                sprintf((char*)g_rsp_str, "+CSSU: 0");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }

            case MN_SS_OUTGOING_FORWARED_TO_C:
            case MN_SS_INCOMING_FORWARED_TO_C:
            {
                // the result code is 2:A call has been forwarded
                sprintf((char*)g_rsp_str, "+CSSI: 2");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }

            default:
                break;
        }
        
    }
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_BARRED_IND signal and 
//                sends the +CSSI: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessBarredInd(
                ATC_CONFIG_T              *atc_config_ptr,
                APP_MN_CALL_BARRED_IND_T  *sig_ptr
                )
{
    // the enable the +CSSI unsolicited result code
    if (atc_config_ptr->cssi_flag)
    {
        switch (sig_ptr->barred_operation)
        {
            case MN_SS_BAOC:
            case MN_SS_BOIC:
            case MN_SS_BOIC_EXHC:
            {
                // the unsolicited code is 5:Outgoing calls are barred
                sprintf((char*)g_rsp_str, "+CSSI: 5");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
        
            case MN_SS_BAIC:
            case MN_SS_BICROAM:
            {
                // the unsolicited code is 6:Incoming calls are barred
                sprintf((char*)g_rsp_str,"+CSSI: 6");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
        
            default:
                break;
        }
        return S_ATC_SUCCESS;
    }
    // diabale the +CSSI unsolicited result code
    else
    {
        return S_ATC_DEFAULT_HANDLE;
    }
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_FORWARD_STATUS_IND signal and 
//                sends the +CSSI: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessForwardStatusInd(
                        ATC_CONFIG_T                   *atc_config_ptr,
                        APP_MN_FORWARD_STATUS_IND_T    *sig_ptr
                        )
{
    // enable the unsolicited result code: +CSSI
    if (atc_config_ptr->cssi_flag)
    {
        switch (sig_ptr->forward_operation)
        {
            case MN_SS_ALL_FORWARD:
            {            
                // Unconditional call forwarding is active
                if (MN_SS_NOT_ACTIVE == sig_ptr->forward_status)
                {
                    sprintf((char*)g_rsp_str, "+CSSI: 0");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_SUCCESS;
                }
                else
                {
                    return S_ATC_DEFAULT_HANDLE;
                }
            }
                
            case MN_SS_CFU:
            case MN_SS_ALL_COND_FORWARD:
            case MN_SS_CFB:
            case MN_SS_CFNRY:
            case MN_SS_CFNRC:
            {
                // Some of the conditional call forwardings are active
                if (MN_SS_NOT_ACTIVE == sig_ptr->forward_status)
                {
                    sprintf((char*)g_rsp_str, "+CSSI: 1");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_SUCCESS;
                }
                else
                {
                    return S_ATC_DEFAULT_HANDLE;
                }
            }
                
            default:
                break;
        }
    }
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_REGISTER_SS_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessRegisterCnf(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_REGISTER_SS_CNF_T *sig_ptr
                )
{
    ATC_ProcessSSCnf(atc_config_ptr,sig_ptr->result_type,
                sig_ptr->list_num,sig_ptr->info_list,TRUE);
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_INTERROGATE_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessInterCnf(
                        ATC_CONFIG_T             *atc_config_ptr,
                        APP_MN_INTERROGATE_CNF_T *sig_ptr
                        )
{
    BOOLEAN   is_service_active;

    if(ATC_CheckInterrogateCnf(atc_config_ptr,sig_ptr->result_type,
        sig_ptr->list_num,sig_ptr->info_list,&is_service_active))
    {
        switch ( s_ss_context.cmd_id )
        {
            //#ifdef _GSM_ONLY_
            case AT_CMD_CLIP:
            {
                sprintf((char*)g_rsp_str, "%s: %d,%d",g_atc_info_table[AT_CMD_CLIP].cmd_name->str_ptr,
                    atc_config_ptr->atc_config_nv.clip_flag, is_service_active);
                break;
            }
            //#endif

            case AT_CMD_CLIR:
            {
                sprintf((char*)g_rsp_str, "%s: %d,%d",g_atc_info_table[AT_CMD_CLIR].cmd_name->str_ptr,
                    atc_config_ptr->atc_config_nv.clir_flag,is_service_active);
                break;
            }

            #ifdef _GSM_ONLY_
            case AT_CMD_COLP:
            {
                sprintf((char*)g_rsp_str, "%s: %d,%d",g_atc_info_table[AT_CMD_COLP].cmd_name->str_ptr,
                    atc_config_ptr->atc_config_nv.colp_flag, is_service_active);
                break;
            }
            #endif
            #if 0
            case AT_CMD_COLR:
            {
                sprintf((char*)g_rsp_str, "%s: %d,%d",g_atc_info_table[AT_CMD_COLR].cmd_name->str_ptr,
                    0, is_service_active);
                break;
            }
            #endif
            case AT_CMD_CLCK:
            {
                sprintf((char*)g_rsp_str, "%s: %d,%d",g_atc_info_table[AT_CMD_CLCK].cmd_name->str_ptr,
                    is_service_active, ATC_VOICE);
                break;
            }

            case AT_CMD_CCWA:
            {
                sprintf((char*)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CCWA].cmd_name->str_ptr,
                    is_service_active, ATC_VOICE);
                break;
            }

            case AT_CMD_CCFC:
            {
                if (is_service_active)
                {
                    uint8              tel_number[MN_MAX_ADDR_BCD_LEN*2] = {0};
                    ATC_TELE_TYPE_E    tel_type;
                    if (sig_ptr->info_list[0].forward_num_present)
                    {
                        ATC_ConvertCalledNumToStr(&(sig_ptr->info_list[0].forward_num),&tel_type, tel_number);
                        if (sig_ptr->info_list[0].no_reply_time_present)
                        {
                            sprintf((char*)g_rsp_str, "%s: %d,%d,\"%s\",%d,%d",g_atc_info_table[AT_CMD_CCFC].cmd_name->str_ptr,
                                AT_STATUS_ACTIVE, ATC_VOICE, &(tel_number[1]),tel_number[0], sig_ptr->info_list[0].no_reply_time);
                        }
                        else
                        {
                            sprintf((char*)g_rsp_str, "%s: %d,%d,\"%s\",%d",g_atc_info_table[AT_CMD_CCFC].cmd_name->str_ptr,
                                AT_STATUS_ACTIVE, ATC_VOICE,  &(tel_number[1]),tel_number[0]);
                        }
                    }
                    else
                    {
                        tel_number[0] = '\0';
                        sprintf((char*)g_rsp_str, "%s: %d,%d,\"%s\",,",g_atc_info_table[AT_CMD_CCFC].cmd_name->str_ptr,
                                AT_STATUS_ACTIVE, ATC_VOICE, tel_number);
                    }
                }
                else
                {
                    sprintf((char*)g_rsp_str, "%s: %d,%d",g_atc_info_table[AT_CMD_CCFC].cmd_name->str_ptr,
                        AT_STATUS_NOT_ACTIVE, ATC_VOICE);
                }
                break;
            }

            default:
                return S_ATC_DEFAULT_HANDLE;    
        }
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK ,0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE); 
    }
    s_ss_context.cmd_id = (int16)AT_INVALID_CMD_ID;
    
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_DEACTIVATE_SS_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessDeactCnf(
                       ATC_CONFIG_T               *atc_config_ptr,
                       APP_MN_DEACTIVATE_SS_CNF_T *sig_ptr
                       )
{
    BOOLEAN result = FALSE;

    result = ATC_ProcessSSCnf(atc_config_ptr,sig_ptr->result_type,
                sig_ptr->list_num,sig_ptr->info_list,FALSE);
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_ACTIVATE_SS_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessActCnf(
                        ATC_CONFIG_T                *atc_config_ptr,
                        APP_MN_ACTIVATE_SS_CNF_T    *sig_ptr
                        )
{
    BOOLEAN result = FALSE;

    result = ATC_ProcessSSCnf(atc_config_ptr,sig_ptr->result_type,
                sig_ptr->list_num,sig_ptr->info_list,TRUE);
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_ERASE_SS_CNF_T signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessEraseCnf(
                        ATC_CONFIG_T              *atc_config_ptr,
                        APP_MN_ERASE_SS_CNF_T     *sig_ptr
                        )
{
    ATC_ProcessSSCnf(atc_config_ptr,sig_ptr->result_type,
                sig_ptr->list_num,sig_ptr->info_list,FALSE);
    return S_ATC_DEFAULT_HANDLE;
}
#endif /* _ULTRA_LOW_CODE_ */

/*****************************************************************************/
//  Description : This function handle the AT+CLCK command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCLCK )
{
#ifndef _ULTRA_LOW_CODE_
    MN_BLOCK_T        pin_block;
    uint8             cmd_type;
    uint8             *temp_ptr;
    uint8             param_len;
    uint8             fac_type;
    ATC_CPWD_PARAM_T  param[4];
    uint8             operate_type = ATC_CLCK_DISABLE;
    uint8             class_type; //only support voice now.
    ERR_MNPHONE_CODE_E  err_code;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(param, 0, sizeof(param));
    SCI_MEMSET(s_clck_psw, 0, sizeof(s_clck_psw));  

    //check the index.
    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    param_len = PARA1.str_ptr->str_len;
    temp_ptr  = PARA1.str_ptr->str_ptr;
    ATC_GetCmdType(temp_ptr, param_len, &cmd_type);
    switch (cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        {
            temp_ptr++;
            param_len--;
            if (!ATC_GetParams(temp_ptr, param_len, param))
            {
                //SCI_TRACE_LOW:"ATC:ATC_GetParams=FALSE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2111_112_2_17_22_17_20_647,(uint8*)"");
                return ERR_OPERATION_NOT_ALLOWED;
            }
            //check the index 
            if ((AT_CPWD_FAC_LEN != param[0].param_len) || (1 != param[1].param_len))
            {
                return ERR_INVALID_INDEX;
            }

            // get the operate code
            operate_type = param[1].param[0]; 
            operate_type -= '0'; 

            // check the password
            if (ATC_CheckPinStr(param[2].param, param[2].param_len))
            {
                SCI_MEMCPY(pin_block.blockbuf, param[2].param, param[2].param_len);
                pin_block.blocklen = param[2].param_len;
                SCI_MEMCPY(s_clck_psw, param[2].param, AT_CLCK_PSW_LEN);
            }
            else
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }
            
            // get the class
            if(0 != param[3].param_len)
            {
                class_type = param[3].param[0]; 
                class_type -= '0'; 
            }
            else
            {
                class_type = AT_CLCK_VOICE;
            }

            // check the fac type
            if (!ATC_GetClckFacType(param[0].param, param[0].param_len, &fac_type))
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            switch (fac_type)
            {
                //handle the pin
                case AT_CLCK_SC:
                {
                    if (!atc_config_ptr->sim_flag[dual_sys])
                    {
                        return ERR_INVALID_INDEX;
                    }
                    
                    switch (operate_type)
                    {
                        case ATC_CLCK_DISABLE:
                        {
                            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_DISABLE, MNSIM_PIN1, &pin_block, PNULL);
                            if(err_code == ERR_MNPHONE_NO_ERR)
                            {
                                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }
                            
                            break;
                        }
                    
                        case ATC_CKCK_ENABLE:
                        {
                            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_ENABLE, MNSIM_PIN1, &pin_block, PNULL);
                            if(err_code == ERR_MNPHONE_NO_ERR)
                            {
                                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }
                            break;
                        }
                    
                        case ATC_CLCK_QUERY_STATUS:
                        {
                            sprintf((char*)g_rsp_str, "+CLCK: %d", atc_config_ptr->pin1_enable_flag[dual_sys]);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK,0);
                            break;
                        }
                            
                        default:
                            return ERR_OPERATION_NOT_ALLOWED;
                    }                   
                    break;
                }
                
                //The following is about SS.
                case AT_PWD_AO:
                {
                    //BAOC (Barr All Outgoing Calls)
                #ifdef _MUX_ENABLE_         
                    if(!HandleClckSSReq(atc_config_ptr,MN_SS_BAOC,operate_type,class_type))
                #else
                    if(!HandleClckSSReq(MN_SS_BAOC,operate_type,class_type))
                #endif
                    {
                        return S_ATC_FAIL;
                    }
                    break;
                }
            
                case AT_PWD_OI:
                {
                    // BOIC (Barr Outgoing International Calls) 
                #ifdef _MUX_ENABLE_         
                    if(!HandleClckSSReq(atc_config_ptr,MN_SS_BOIC,operate_type,class_type))
                #else 
                    if(!HandleClckSSReq(MN_SS_BOIC,operate_type,class_type))
                #endif
                    {
                        return S_ATC_FAIL;
                    }
                    break;
                }
            
                case AT_PWD_OX:
                {
                    // BOIC-exHC (Barr Outgoing. International Calls except to Home Country)
                #ifdef _MUX_ENABLE_         
                    if(!HandleClckSSReq(atc_config_ptr,MN_SS_BOIC_EXHC,operate_type,class_type))
                #else
                    if(!HandleClckSSReq(MN_SS_BOIC_EXHC,operate_type,class_type))
                #endif
                    {
                        return S_ATC_FAIL;
                    }
                    break;
                }
            
                case AT_PWD_AI:
                {
                    //BAIC (Barr All Incoming Calls) 
                #ifdef _MUX_ENABLE_         
                    if(!HandleClckSSReq(atc_config_ptr,MN_SS_BAIC,operate_type,class_type))
                #else
                    if(!HandleClckSSReq(MN_SS_BAIC,operate_type,class_type))
                #endif
                    {
                        return S_ATC_FAIL;
                    }
                
                    break;
                }
            
                case AT_PWD_IR:
                {
                    //BIC-Roam (Barr Inc.  When Roaming outside Home Country)
                #ifdef _MUX_ENABLE_         
                    if(!HandleClckSSReq(atc_config_ptr,MN_SS_BICROAM,operate_type,class_type))
                #else
                    if(!HandleClckSSReq(MN_SS_BICROAM,operate_type,class_type))
                #endif
                    {
                        return S_ATC_FAIL;
                    }
                    break;
                }
                    
                case AT_PWD_AB:
                {
                    // All Barring services 
                #ifdef _MUX_ENABLE_         
                    if(!HandleClckSSReq(atc_config_ptr,MN_SS_ALL_BAR_SERVICE,operate_type,class_type))
                #else 
                    if(!HandleClckSSReq(MN_SS_ALL_BAR_SERVICE,operate_type,class_type))
                #endif
                    {
                        return S_ATC_FAIL;
                    }
                    break;
                }
            
                case AT_PWD_AG:
                {
                    //All outGoing barring services
                #ifdef _MUX_ENABLE_         
                    if(!HandleClckSSReq(atc_config_ptr,MN_SS_BAR_OUTGOING,operate_type,class_type))
                #else
                    if(!HandleClckSSReq(MN_SS_BAR_OUTGOING,operate_type,class_type))
                #endif
                    {
                        return S_ATC_FAIL;
                    }
                    break;
                }
            
                case AT_PWD_AC:
                {
                    //All inComing barring services  
                #ifdef _MUX_ENABLE_         
                    if(!HandleClckSSReq(atc_config_ptr,MN_SS_BAR_INCOMING,operate_type,class_type))
                #else
                    if(!HandleClckSSReq(MN_SS_BAR_INCOMING,operate_type,class_type))
                #endif
                    {
                        return S_ATC_FAIL;
                    }
                    break;
                }
            
                default:
                    return ERR_OPERATION_NOT_ALLOWED;
            }
            s_ss_context.cmd_id = AT_CMD_CLCK;
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            atc_config_ptr->pin1_enable_flag[dual_sys] = MNSIM_IsPin1EnableEx(dual_sys);
            //SCI_TRACE_LOW:"ATC: CPIN pin1 enable flag: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2332_112_2_17_22_17_21_648,(uint8*)"d", atc_config_ptr->pin1_enable_flag[dual_sys]);
            ATC_FormatClckReadRsp(g_rsp_str, atc_config_ptr->pin1_enable_flag[dual_sys]);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            return S_ATC_SUCCESS;
        }

        case ATC_CMD_TYPE_TEST:
        {
            ATC_FormatClckTestRsp(g_rsp_str);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            return S_ATC_SUCCESS;
        }

        default:
            break;
    }
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
} 

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handle the SS service in AT+CLCK.
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
#ifdef _MUX_ENABLE_
LOCAL BOOLEAN HandleClckSSReq(
                                   ATC_CONFIG_T   *atc_config_ptr,
                    MN_SS_CODE_E    ss_code,
                    uint8           operate_type,
                    uint8           class_type
                    )
#else
LOCAL BOOLEAN HandleClckSSReq(
                    MN_SS_CODE_E    ss_code,
                    uint8           operate_type,
                    uint8           class_type
                    )
#endif
{
    ATC_CLASS_E                service_type;
    MN_SS_BASIC_SERVICE_T      basic_service;

       MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //Only support the voice now.
    switch(class_type)
    {
        case AT_CLCK_VOICE:
        {
            service_type = ATC_VOICE;
            break;
        }
    
        default:
            return FALSE;
    }
    // get the MNSS service code
    ConvertServiceCode(service_type, &(basic_service.service_code));
    //SCI_TRACE_LOW("ATC:basic_service.service_code=%d",basic_service.service_code);
    basic_service.service_type = MN_SS_TELE_SERVICE;

    switch (operate_type)
    {
        case ATC_CLCK_DISABLE:
        {
            MNSS_DeactivateSSEx(dual_sys, ss_code, basic_service);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_DEACTIVATE_SS_CNF,0xff,ATC_NO_DOMAIN);
            break;
        }
    
        case ATC_CKCK_ENABLE:
        {
            MNSS_ActivateSSEx(dual_sys, ss_code, basic_service);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ACTIVATE_SS_CNF,0xff,ATC_NO_DOMAIN);
            break;
        }
    
        case ATC_CLCK_QUERY_STATUS:
        {
            basic_service.service_type = MN_SS_ALL_SERVICE;     
            MNSS_InterrogateServiceEx(dual_sys, ss_code, basic_service);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_INTERROGATE_CNF,0xff,ATC_NO_DOMAIN);
            break;
        }
            
        default:
            return FALSE;
    }
    return TRUE;
}
#endif/*_ULTRA_LOW_CODE_*/

/*****************************************************************************/
//  Description : This function handle the AT+CPWD command
//  This command is used to change the password
//            AT+CPWD=<fac>,<oldpin>,<newpin> 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPWD )
{
#ifndef _ULTRA_LOW_CODE_
    uint8             cmd_type;
    uint8             *temp_ptr;
    uint8             param_len;
    uint8             fac_type;
    ATC_CPWD_PARAM_T  param[3];
    MN_BLOCK_T        password;        // the old pin
    MN_BLOCK_T        new_password;    // the new pin
    ERR_MNPHONE_CODE_E  err_code;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(param, 0, sizeof(param));
    SCI_MEMSET(s_old_password, 0, sizeof(s_old_password));
    SCI_MEMSET(s_new_password, 0, sizeof(s_new_password));

    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        //SCI_TRACE_LOW:"ATC: cpwd parameter is not right"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2452_112_2_17_22_17_21_649,(uint8*)"");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    //get the operation type.
    param_len = PARA1.str_ptr->str_len;
    temp_ptr  = PARA1.str_ptr->str_ptr;
    ATC_GetCmdType(temp_ptr, param_len, &cmd_type);
    switch (cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        {
            temp_ptr++;
            param_len--;
            
            if (!ATC_GetParams(temp_ptr, param_len, param))
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }
            
            //handle the pin
            if (AT_CPWD_FAC_LEN != param[0].param_len ||0 == param[1].param_len 
                || 0 == param[2].param_len)
                return ERR_INVALID_INDEX;

            // check the old pin
            if (ATC_CheckPinStr(param[1].param, param[1].param_len))
            {
                SCI_MEMCPY(password.blockbuf, param[1].param, param[1].param_len);
                password.blocklen = param[1].param_len;
                SCI_MEMCPY(s_old_password, param[1].param, AT_CLCK_PSW_LEN);
            }
            else
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }
            
            // check the new pin
            if (ATC_CheckPinStr(param[2].param, param[2].param_len))
            {
                if (param[2].param_len < AT_PSW_MIN_LEN || param[2].param_len > AT_PSW_MAX_LEN)
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }
                
                SCI_MEMCPY(new_password.blockbuf, param[2].param, param[2].param_len);
                new_password.blocklen = param[2].param_len;
                SCI_MEMCPY(s_new_password, param[2].param, AT_CLCK_PSW_LEN);
                SCI_MEMCPY(s_new_password_again, param[2].param, AT_CLCK_PSW_LEN);
            }
            else
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }
            
            // check the fac type
            if (!ATC_GetFacType(param[0].param, param[0].param_len, &fac_type))
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }
            
            switch ( fac_type )
            {       
                case AT_PWD_SC:
                {
                    if (!atc_config_ptr->sim_flag[dual_sys])
                    {
                        //SCI_TRACE_LOW:"ATC: cpwd sim is not ready"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2519_112_2_17_22_17_21_650,(uint8*)"");
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    
                    err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_CHANGE, MNSIM_PIN1, &password, &new_password);
                    if(err_code == ERR_MNPHONE_NO_ERR)
                    {
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                    }
                    break;
                }
            
                case AT_PWD_P2:
                {
                    if (!atc_config_ptr->sim_flag[dual_sys])
                    {
                        //SCI_TRACE_LOW:"ATC: cpwd sim is not ready"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2535_112_2_17_22_17_21_651,(uint8*)"");
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    
                    err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_CHANGE, MNSIM_PIN2, &password, &new_password);
                    if(err_code == ERR_MNPHONE_NO_ERR)
                    {
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                    }
                    break;
                }
            
                //The following is about SS.
                case AT_PWD_AO:
                case AT_PWD_OI:
                case AT_PWD_OX:
                case AT_PWD_AI:
                case AT_PWD_IR:
                case AT_PWD_AB:
                case AT_PWD_AG:
                case AT_PWD_AC:
                {
                    // All Barring services 
                    if(ERR_MNSS_NO_ERR != MNSS_RegisterPasswordEx(dual_sys, MN_SS_ALL_BAR_SERVICE))
                    {
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_REGISTER_PASSWORD_CNF,0xff,ATC_NO_DOMAIN);
                        return S_ATC_FAIL;
                    }
                    //SCI_TRACE_MID:"ATC:AT_PWD_AB"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2563_112_2_17_22_17_21_652,(uint8*)"");
                    break;
                }
            
                default:
                    return ERR_OPERATION_NOT_ALLOWED;
            }
            s_ss_context.cmd_id = AT_CMD_CPWD;
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            //SCI_TRACE_LOW("ATC: cpwd command type is read");
            return ERR_OPERATION_NOT_ALLOWED;
        }

        case ATC_CMD_TYPE_TEST:
        {
            ATC_FormatCpwdStr(g_rsp_str);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            return S_ATC_SUCCESS;
        }

        default:
            break;
    }
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_GET_PASSWORD_IND signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessGetPswInd(
                      ATC_CONFIG_T                   *atc_config_ptr,
                      APP_MN_GET_PASSWORD_IND_T      *sig_ptr
                      )
{
    MN_DUAL_SYS_E dual_sys;
    
    dual_sys = sig_ptr->dual_sys;
    
    //SCI_TRACE_MID:"ATC:ATC_ProcessGetPswInd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2610_112_2_17_22_17_21_653,(uint8*)"");
    switch( s_ss_context.cmd_id )
    {
        case AT_CMD_CPWD:
        {
            //SCI_TRACE_MID:"ATC:AT_CMD_CPWD"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2615_112_2_17_22_17_21_654,(uint8*)"");
            //Send the old password
            if(MN_SS_ENTER_PW == sig_ptr->guidance_info)
            {
                //SCI_TRACE_MID:"ATC:s_old_password"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2619_112_2_17_22_17_21_655,(uint8*)"");
                MNSS_SendUserPasswordEx(dual_sys, s_old_password);
            }
            //Send the new password
            else if(MN_SS_ENTER_NEW_PW == sig_ptr->guidance_info)
            {
                //SCI_TRACE_MID:"ATC:s_new_password"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2625_112_2_17_22_17_21_656,(uint8*)"");
                MNSS_SendUserPasswordEx(dual_sys, s_new_password);
            }
            else if(MN_SS_ENTER_NEW_PW_AGAIN == sig_ptr->guidance_info)
            {
                //SCI_TRACE_MID:"ATC:s_new_password again"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2630_112_2_17_22_17_22_657,(uint8*)"");
                MNSS_SendUserPasswordEx(dual_sys, s_new_password_again);
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: received error singal in get password indicate!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2635_112_2_17_22_17_22_658,(uint8*)"");
            }
            break;
        }
    
        case AT_CMD_CLCK:
        {
            MNSS_SendUserPasswordEx(dual_sys, s_clck_psw);
            break;
        }
    
        default:
            break;
    }
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_REGISTER_PASSWORD_CNF signal 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:         
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPwdCnf(
                      ATC_CONFIG_T                   *atc_config_ptr,
                      APP_MN_REGISTER_PASSWORD_CNF_T      *sig_ptr
                      )
{
    if(MN_SS_OPERATION_SUCC == sig_ptr->result_type)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK,0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
    }
    s_ss_context.cmd_id = (int16)AT_INVALID_CMD_ID;
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_RELEASE_SS_IND signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessRelCnf(
                    ATC_CONFIG_T               *atc_config_ptr,
                    APP_MN_RELEASE_SS_IND_T    *sig_ptr)
{
    //handle for the USSD.
    if(TRUE == s_cusd_isresponse)
    {
        s_cusd_isresponse = FALSE;
        #if 0
        //if enable to present result,show it.
        if(AT_CUSD_ENABLE == s_cusd_presentation)
        {
            sprintf((char*)g_rsp_str, "%s: 2",g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
        }
        #endif
    }
    else
    {
        if (s_cusd_notification)
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
        }
        else
        {
            s_cusd_notification = TRUE;
        }
    }

    s_ss_context.cmd_id = (int16)AT_INVALID_CMD_ID;

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function checks if there is held calls
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN   CheckPresentHeldCall(
                ATC_CALLS_STATE_T   *call_state_ptr,
                uint8               *call_id
                )
{
    BOOLEAN    flag = FALSE;
    uint8    i = 0;
    
    //SCI_TRACE_LOW:"enter into CheckPresentHeldCall"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2723_112_2_17_22_17_22_659,(uint8*)"");
    for (i=0; i < call_state_ptr->call_amount; i++)
    {
        if ((ATC_CALL_HOLD == call_state_ptr->call_context[i].hold_state) 
            && (ATC_CALL_ACTIVE_STATE == call_state_ptr->call_context[i].call_entity_state))
        {
            flag = TRUE;
            *call_id = call_state_ptr->call_context[i].call_id;
            //SCI_TRACE_LOW:"found a held call %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2731_112_2_17_22_17_22_660,(uint8*)"d", *call_id);
            break;
        }
    }
    return flag;
}

/*****************************************************************************/
//  Description : This function release all held calls present
//  Global resource dependence : none
//    Author:       Elvis Xu
//  Note:
/*****************************************************************************/
BOOLEAN   RelPresentHeldCalls(
                            MN_DUAL_SYS_E dual_sys,
                ATC_CALLS_STATE_T   *call_state_ptr
                )
{
    BOOLEAN    flag = FALSE;
    uint8    held_id = 0xff;
    uint8    i = 0;
    MN_CALL_DISCONNECT_CALL_CAUSE_E  cause;
    
    //SCI_TRACE_LOW:"enter into RelPresentHeldCalls"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2752_112_2_17_22_17_22_661,(uint8*)"");
    
    for (i=0; i < call_state_ptr->call_amount; i++)
    {
        if ((ATC_CALL_HOLD == call_state_ptr->call_context[i].hold_state) &&
            (ATC_CALL_ACTIVE_STATE == call_state_ptr->call_context[i].call_entity_state))
        {
            flag = TRUE;
            held_id = call_state_ptr->call_context[i].call_id;
            //SCI_TRACE_LOW:"held_id: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2761_112_2_17_22_17_22_662,(uint8*)"d", held_id);
            
            if(call_state_ptr->call_context[i].call_entity_state == ATC_CALL_INCOMING_STATE)
            {
                cause = MN_CAUSE_USER_BUSY;
            }
            else
            {
                cause = MN_CAUSE_NORMAL_CLEARING;
            }
            
#ifdef MS_OAPI_SUPPORT
            MNCALL_DisconnectCallEx(dual_sys, held_id, PNULL);
#else
            MNCALL_DisconnectCallEx(dual_sys, held_id, cause, PNULL);
#endif
        }
    }
    
    return flag;
}

/*****************************************************************************/
//  Description : This function check if there is an active call
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN CheckPresentActiveCall(
                    ATC_CALLS_STATE_T   *call_state_ptr,
                    uint8               *call_id
                    )
{
    BOOLEAN    flag = FALSE;
    uint8    i = 0;
    
    //SCI_TRACE_LOW:"enter into CheckPresentActiveCall"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2795_112_2_17_22_17_22_663,(uint8*)"");
    for (i=0; i < call_state_ptr->call_amount; i++)
    {
        if ((ATC_CALL_ACTIVE_STATE == call_state_ptr->call_context[i].call_entity_state) &&
            (ATC_HOLD_IDLE == call_state_ptr->call_context[i].hold_state))
        {
            flag = TRUE;
            *call_id = call_state_ptr->call_context[i].call_id;
            //SCI_TRACE_LOW:"found an active call %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2803_112_2_17_22_17_22_664,(uint8*)"d", *call_id);
            break;
        }
    }
    return flag;
}

/*****************************************************************************/
//  Description : This function release all active calls present
//  Global resource dependence : none
//    Author:       Elvis Xu
//  Note:
/*****************************************************************************/
BOOLEAN RelPresentActiveCalls(
                                   MN_DUAL_SYS_E dual_sys,
                    ATC_CALLS_STATE_T   *call_state_ptr
                    )
{
    BOOLEAN    flag = FALSE;
    uint8    active_id = 0xff;
    uint8    i = 0;
    
    //SCI_TRACE_LOW:"enter into RelPresentActiveCalls"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2823_112_2_17_22_17_22_665,(uint8*)"");
    
    for (i = 0; i < call_state_ptr->call_amount; i++)
    {
        if ((ATC_CALL_ACTIVE_STATE == call_state_ptr->call_context[i].call_entity_state) &&
            (ATC_HOLD_IDLE == call_state_ptr->call_context[i].hold_state))
        {
            flag = TRUE;
            active_id = call_state_ptr->call_context[i].call_id;
            //SCI_TRACE_LOW:"call %d released"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2832_112_2_17_22_17_22_666,(uint8*)"d", active_id);
#ifdef MS_OAPI_SUPPORT
            MNCALL_DisconnectCallEx(dual_sys, active_id, PNULL);
#else
            MNCALL_DisconnectCallEx(dual_sys, active_id, MN_CAUSE_NORMAL_CLEARING, PNULL);
#endif
        }
    }
    return flag;
}
#endif /* _ULTRA_LOW_CODE_ */
 
/*****************************************************************************/
//  Description : This function check if there is a waiting call
//  Global resource dependence : none
//    Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN CheckPresentWaitCall(
                    ATC_CALLS_STATE_T   *call_state_ptr,
                    uint8     *call_id
                    )
{
    BOOLEAN    flag = FALSE;
    uint8    i = 0;
    
    //SCI_TRACE_LOW:"enter into CheckPresentWaitCall"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2856_112_2_17_22_17_22_667,(uint8*)"");
    
    // check all of the present calls
    for (i=0; i < call_state_ptr->call_amount; i++)
    {
        if (ATC_CALL_INCOMING_STATE == call_state_ptr->call_context[i].call_entity_state)
        {
            flag = TRUE;
            *call_id = call_state_ptr->call_context[i].call_id;
            //SCI_TRACE_LOW:"found a waiting call %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2865_112_2_17_22_17_22_668,(uint8*)"d", *call_id);
            break;
        }
    }
    return flag;
}

/*****************************************************************************/
//  Description : This function set user_determined user busy for a waiting call
//  Global resource dependence : none
//    Author:       Elvis Xu
//  Note:
/*****************************************************************************/
BOOLEAN SetUserBusyForWaitCall(
                                   MN_DUAL_SYS_E dual_sys,
                    ATC_CALLS_STATE_T   *call_state_ptr
                    )
{
    BOOLEAN     flag = FALSE;
    uint8    wait_id = 0xff;
    
    //SCI_TRACE_LOW:"enter into SetUserBusyForWaitCall"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2884_112_2_17_22_17_22_669,(uint8*)"");
    flag = CheckPresentWaitCall(call_state_ptr, &wait_id);
    if (flag)
    {
        //SCI_TRACE_LOW:"waiting call %d set UDUB"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2888_112_2_17_22_17_22_670,(uint8*)"d", wait_id);
#ifdef MS_OAPI_SUPPORT
        MNCALL_DisconnectCallEx(dual_sys, wait_id, PNULL);
#else
        MNCALL_DisconnectCallEx(dual_sys, wait_id, MN_CAUSE_USER_BUSY, PNULL);
#endif
    }
    
    return flag;
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function check the SS CNF signal 
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_ProcessSSCnf(
            ATC_CONFIG_T              *atc_config_ptr,
            MN_SS_OPERATE_RESULT_E    result_type,
            uint8                     list_num,
            MN_SS_INTERROGATE_INFO_T  info_list[],
            BOOLEAN                   op_type  //APP_MN_INTERROGATE_CNF,APP_MN_REGISTER_SS_CNF,
                                                //APP_MN_ACTIVATE_SS_CNF is TRUE
                                               //   APP_MN_ERASE_SS_CNF,APP_MN_DEACTIVATE_SS_CNF is FALSE
                                )
{
    int32                      i = 0;
    BOOLEAN is_op_succ = FALSE;
    
    /*
    //SCI_TRACE_LOW:"ATC:ATC_ProcessSSCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2919_112_2_17_22_17_22_671,(uint8*)"dddd",result_type,list_num,info_list[0].ss_status_present,info_list[0].ss_status);
    */  
    //IF success,check further.
    if (MN_SS_OPERATION_SUCC == result_type)
    {
        if(0 < list_num)
        {
            for (i = 0;i < list_num;i++)
            {
                if (info_list[i].ss_status_present)
                {
                    if(op_type)
                    {
                        if ((info_list[i].ss_status == MN_SS_PROVITION)||
                            (info_list[i].ss_status == MN_SS_ACTIVE_OPERATIVE)||
                            (info_list[i].ss_status == MN_SS_ACITVE_QUIESCENT))
                        {
                            is_op_succ = TRUE;
                            break;
                        }
                    }
                    else
                    {
                        if ((info_list[i].ss_status == MN_SS_NOT_PROVITION)||
                            (info_list[i].ss_status == MN_SS_NOT_REGISTERED)||
                            (info_list[i].ss_status == MN_SS_NOT_ACTIVE))
                        {
                            is_op_succ = TRUE;
                            break;
                        }
                    }
                }
                else
                {
                    is_op_succ = TRUE;
                    break;
                }
            }
        }
        else
        {
            is_op_succ = TRUE;
        }
    }
    
    //Return the indication
    if(is_op_succ)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK ,0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE); 
    }
    return is_op_succ;
}

/*****************************************************************************/
//  Description : This function handle APP_MN_INTERROGATE_CNF signal 
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_CheckInterrogateCnf(
            ATC_CONFIG_T              *atc_config_ptr,
            MN_SS_OPERATE_RESULT_E    result_type,
            uint8                     list_num,
            MN_SS_INTERROGATE_INFO_T  info_list[],
            BOOLEAN              *is_service_active
                                )
{
    int32 i = 0;
    BOOLEAN is_inter_succ = FALSE;
    
    /*
    //SCI_TRACE_LOW:"ATC:ATC_CheckInterrogateCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d,service_type=%d,service_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_2994_112_2_17_22_17_22_672,(uint8*)"dddddd",result_type,list_num,info_list[0].ss_status_present,info_list[0].ss_status,info_list[0].basic_service.service_type,info_list[0].basic_service.service_code);
    */  
    *is_service_active = FALSE;
    //IF success,check further.
    if (MN_SS_OPERATION_SUCC == result_type)
    {
        if(0 < list_num)
        {
            is_inter_succ = TRUE;
            for (i = 0;i < list_num;i++)
            {
                if(MN_SS_ALL_SERVICE == info_list[i].basic_service.service_type ||
                    (MN_SS_TELE_SERVICE == info_list[i].basic_service.service_type
                    &&MN_SS_ALL_SPEECH_TRANS_SERVICES == info_list[i].basic_service.service_code))
                {
                    if (info_list[i].ss_status_present)
                    {
                        if ((info_list[i].ss_status == MN_SS_PROVITION)||
                            (info_list[i].ss_status == MN_SS_ACTIVE_OPERATIVE)||
                            (info_list[i].ss_status == MN_SS_ACITVE_QUIESCENT))
                        {                           
                            *is_service_active = TRUE;
                            break;
                        }
                    }
                    else
                    {
                        *is_service_active = TRUE;
                        break;
                    }
                }
            }
        }
    }
    return is_inter_succ;
}

/*****************************************************************************/
//  Description : This function handles release all of the held call or release 
//    the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldUdub(
                        ATC_CONFIG_T        *atc_config_ptr
                        )
{
    ATC_CALLS_STATE_T * call_state_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    //SCI_TRACE_LOW:"enter into HandleChldUdub"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3043_112_2_17_22_17_22_673,(uint8*)"");
    atc_config_ptr->ss_operate = ATC_REL_HELD_CALLS_OR_SET_UDUB;
    call_state_ptr = &g_calls_state[dual_sys];

    // if present held calls then release all of the held calls
    if (RelPresentHeldCalls(dual_sys,call_state_ptr))
    {
        return S_ATC_DEFAULT_HANDLE;
    }
    // check if there is a waiting call
    else if (SetUserBusyForWaitCall(dual_sys,call_state_ptr))
    {
        return S_ATC_DEFAULT_HANDLE;
    }
    else
    {
        atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
        return S_ATC_SUCCESS;
    }

}

/*****************************************************************************/
//  Description : This function handles release active call and accept other call
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelAcceptOthers(
                        ATC_CONFIG_T        *atc_config_ptr
                        )
{
    ATC_STATUS    status = S_ATC_DEFAULT_HANDLE;
    ATC_CALLS_STATE_T * call_state_ptr = PNULL;
    uint8 call_id = 0xff;
    uint8 flag = TRUE;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
     
    //SCI_TRACE_LOW:"enter into HandleChldRelAcceptOthers"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3079_112_2_17_22_17_22_674,(uint8*)"");

    
    atc_config_ptr->ss_operate = ATC_REL_ACTIVE_CALLS_AND_ACCEPT_OTHERS;
    call_state_ptr = &g_calls_state[dual_sys];
    
    // if present active calls then release them
    if (CheckPresentActiveCall(call_state_ptr, &call_id))
    {
        flag = RelPresentActiveCalls(dual_sys,call_state_ptr);
    }
    else
    {
        // if present waiting calls then accpet it
        if (CheckPresentWaitCall(call_state_ptr, &call_id))
        {           
            MNCALL_ConnectCallEx(dual_sys, call_id, PNULL);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                        (uint16)APP_MN_SETUP_COMPLETE_IND,
                        call_id,
                        ATC_DOMAIN_CS);
            
            if (!atc_config_ptr->audio_flag)
            {
                AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
                AUDIO_SetVolume(atc_config_ptr->speaker_volume);
                ATC_EnableVoiceCodec(TRUE);
                atc_config_ptr->audio_flag = TRUE;
                g_atc_call_flag = TRUE;
            }
        }
        // if present held calls then retrieve then
        else if (CheckPresentHeldCall(call_state_ptr, &call_id))
        {
            MNCALL_RetrieveCallEx(dual_sys, call_id);   
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_CALL_RETRIEVED_CNF,call_id,ATC_DOMAIN_CS);
        }
        else
        {
            flag = FALSE;
        }
    }   
    
    if (!flag)
    {
        status = ERR_OPERATION_NOT_ALLOWED;
        atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
    }

    return status;  
}

/*****************************************************************************/
//    Description : This function hold the active calls and accept the held or waiting
//    calls 
//    release the waiting call.
//    Global resource dependence : none
//    Author:       Xueli Lv
//    Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldHoldAcceptOthers(
                        ATC_CONFIG_T        *atc_config_ptr
                        )
{
    ATC_STATUS    status = S_ATC_DEFAULT_HANDLE;
    ATC_CALLS_STATE_T * call_state_ptr = PNULL;
    uint8    active_id = 0xff;
    uint8    waiting_id = 0xff;
    uint8    held_id = 0xff;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    //SCI_TRACE_LOW:"enter into HandleChldHoldAcceptOthers"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3148_112_2_17_22_17_23_675,(uint8*)"");
    atc_config_ptr->ss_operate = ATC_HOLD_CALLS_AND_ACCEPT_OTHERS;
    call_state_ptr = &g_calls_state[dual_sys];

    if (CheckPresentActiveCall(call_state_ptr, &active_id))
    {
        if (CheckPresentHeldCall(call_state_ptr, &held_id))
        {
            MNCALL_SwapCallsEx(dual_sys, active_id, held_id);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_CALL_RETRIEVED_CNF,held_id,ATC_DOMAIN_CS);
        }
        else
        {
            MNCALL_HoldCallEx(dual_sys, active_id);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_CALL_HELD_CNF,active_id,ATC_DOMAIN_CS);
        }
    }
    else
    {
        // check if there is waiting calls
        if (CheckPresentWaitCall(call_state_ptr, &waiting_id))
        {           
            MNCALL_ConnectCallEx(dual_sys, waiting_id, PNULL);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SETUP_COMPLETE_IND,waiting_id,ATC_DOMAIN_CS);
            if (!atc_config_ptr->audio_flag)
            {
                AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
                AUDIO_SetVolume(atc_config_ptr->speaker_volume);
                ATC_EnableVoiceCodec(TRUE);
                atc_config_ptr->audio_flag = TRUE;
                g_atc_call_flag = TRUE;
            }
        }
        // check if there are held calls
        else if (CheckPresentHeldCall(call_state_ptr, &held_id))
        {
            MNCALL_RetrieveCallEx(dual_sys, held_id);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_CALL_RETRIEVED_CNF,held_id,ATC_DOMAIN_CS);
        }
        else
        {
            status = ERR_OPERATION_NOT_ALLOWED;
            atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function adds the held call to the conversation
//  calls 
//  release the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldMultCall(
                   ATC_CONFIG_T        *atc_config_ptr
                   )
{
    ATC_STATUS    status = S_ATC_DEFAULT_HANDLE;
    ATC_CALLS_STATE_T * call_state_ptr = PNULL;
    uint8    active_id = 0xff;
    uint8    held_id = 0xff;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    //SCI_TRACE_LOW:"enter into HandleChldMultCall"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3213_112_2_17_22_17_23_676,(uint8*)"");
    call_state_ptr = &g_calls_state[dual_sys];
    
    if (CheckPresentHeldCall(call_state_ptr, &held_id) &&
        CheckPresentActiveCall(call_state_ptr, &active_id))
    {
        //SCI_TRACE_LOW:"held_id = %d, active_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3219_112_2_17_22_17_23_677,(uint8*)"dd", held_id, active_id);
        MNCALL_BuildMPTYEx(dual_sys, held_id, active_id);
        atc_config_ptr->ss_operate = ATC_ADD_HELD_TO_CONVERSATION;
    }
    else
    {
        status = ERR_OPERATION_NOT_ALLOWED;
        atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
    }
    
    return status;
}

/*****************************************************************************/
//  Description : check ect condition:
//        a)	First Call (Active, Held), Second Call (Active, Idle);
//        b)	First Call (Active, Held), Second Call (Call Delivered, Idle).

//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEctConditionSatisfied(
    MN_DUAL_SYS_E  dual_sys, 
    uint8          *held_id
    )
{
    uint32 i;
    uint8  flag = 0;    
    
    if (g_calls_state[dual_sys].call_amount != 2)
    {
        return FALSE;
    }
    for(i = 0; i < g_calls_state[dual_sys].call_amount; i++)
    {
        if ((ATC_CALL_ACTIVE_STATE == g_calls_state[dual_sys].call_context[i].call_entity_state) &&  
            (ATC_CALL_HOLD == g_calls_state[dual_sys].call_context[i].hold_state) &&
            (ATC_MPTY_IDLE == g_calls_state[dual_sys].call_context[i].mpty_state))
        {
            *held_id = g_calls_state[dual_sys].call_context[i].call_id;
            flag++;
        }
        else if (((ATC_CALL_ACTIVE_STATE == g_calls_state[dual_sys].call_context[i].call_entity_state) || 
            (ATC_CALL_ALERTING_STATE == g_calls_state[dual_sys].call_context[i].call_entity_state)) &&
            (ATC_HOLD_IDLE == g_calls_state[dual_sys].call_context[i].hold_state) &&
            (ATC_MPTY_IDLE == g_calls_state[dual_sys].call_context[i].mpty_state))
        {
            flag++;
        }
    }

    if (flag == 2)
    {
        return TRUE;
    }
    return FALSE;
}
/*****************************************************************************/
//  Description : This function handle ect operation
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleEctCall(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E       dual_sys
)
{
    uint8                   held_id;
    ATC_STATUS              status = S_ATC_DEFAULT_HANDLE;

    if(IsEctConditionSatisfied(dual_sys, &held_id))
    {
        MNCALL_ECTInvokeEx(dual_sys,  held_id);
        atc_config_ptr->ss_operate= ATC_ECT;
        ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                          APP_MN_ECT_INVOKE_CNF, held_id, ATC_DOMAIN_CS);
    }
    else
    {
        SCI_TRACE_LOW("ATC: HandleEctCall no targeted calls");
        status = S_ATC_SUCCESS; //error permitted for APP call disconnection
        atc_config_ptr->ss_operate= ATC_INVALID_OPERATE;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_BUILD_MPTY_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessBuildMptyCnf(
                        ATC_CONFIG_T              *atc_config_ptr,
                        APP_MN_BUILD_MPTY_CNF_T   *sig_ptr
                        )
{
    ATC_CALLS_STATE_T * call_state_ptr = PNULL;
    uint8    call_index = MN_CALL_MAX_CALL_NUM;
    uint8    i;
    MN_DUAL_SYS_E dual_sys;
    
    dual_sys = sig_ptr->dual_sys;
    
    call_state_ptr = &g_calls_state[dual_sys];

    if (sig_ptr->req_is_accepted)
    {
        call_index = GetIndexOfCallContext(sig_ptr->call_id,dual_sys);

        SCI_ASSERT(ATC_INVALID_CALLID!=call_index); /*assert verified*/
         for(i=0; i < call_state_ptr->call_amount; i++)
         {
             if(ATC_CALL_ACTIVE_STATE == call_state_ptr->call_context[i].call_entity_state)
             {
                 call_state_ptr->call_context[i].mpty_state = ATC_CALL_MPTY;
                 call_state_ptr->call_context[i].hold_state = ATC_HOLD_IDLE;
             }
         }
         
         // reponse the at+chld=3
         ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);

    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,0);
    }
    atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
    return S_ATC_DEFAULT_HANDLE;
}


/*****************************************************************************/
//  Description : This function handles the APP_MN_ECT_INVOKE_CNF signal
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessEctInvokeCnf(
    ATC_CONFIG_T              *atc_config_ptr,
    APP_MN_ECT_INVOKE_CNF_T   *sig_ptr
)
{
    if(sig_ptr->req_is_accepted)
    {
        SCI_TRACE_LOW("ATC: ect invoke successfully");
        // reponse the at+chld=4
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        SCI_TRACE_LOW("ATC: ect invoke failed");
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
    }

    atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
    return S_ATC_DEFAULT_HANDLE;
}


/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_RETRIEVED_CNF signal.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCallRetrievCnf(
                         ATC_CONFIG_T                  *atc_config_ptr,
                         APP_MN_CALL_RETRIEVED_CNF_T   *sig_ptr
                         )
{
    uint8   call_index;
    int32   i;
    MN_DUAL_SYS_E           dual_sys;
    
    dual_sys = sig_ptr->dual_sys;

    if (sig_ptr->req_is_accepted)
    {
        call_index = GetIndexOfCallContext(sig_ptr->call_id,dual_sys);
        
        SCI_ASSERT(ATC_INVALID_CALLID!=call_index); /*assert verified check call index*/

        if(ATC_CALL_MPTY==g_calls_state[dual_sys].call_context[call_index].mpty_state)
        {
            for(i=0; i<g_calls_state[dual_sys].call_amount; i++)
            {
                if(ATC_CALL_MPTY==g_calls_state[dual_sys].call_context[i].mpty_state)
                {
                    g_calls_state[dual_sys].call_context[i].hold_state = ATC_HOLD_IDLE;
                }
            }
        }
        else
        {
            g_calls_state[dual_sys].call_context[call_index].hold_state = ATC_HOLD_IDLE;
        }
        // reponse the at+chld=3
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
    }
    atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_HELD_IND signal and 
//                sends the +CSSU: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessHeldInd(
                  ATC_CONFIG_T              *atc_config_ptr,
                  APP_MN_CALL_HELD_IND_T    *sig_ptr
                  )
{
    SCI_ASSERT((PNULL != atc_config_ptr) && (PNULL != sig_ptr)); /*assert verified*/

    // determine if the +CSSU is enable
    if (atc_config_ptr->cssu_flag)
    {
        // A call has been put on hold (during voice call)
        sprintf((char*)g_rsp_str, "+CSSU: 2");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }
    return S_ATC_DEFAULT_HANDLE;    
}

/*****************************************************************************/
//  Description : This function handle the APP_MN_CALL_HELD_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessHeldCnf(
                    ATC_CONFIG_T            *atc_config_ptr,
                    APP_MN_CALL_HELD_CNF_T  *sig_ptr
                    )
{
    uint8                   call_id, active_id, held_id;
    uint8                   call_index;
    int32                   i;
    
    BOOLEAN                 is_swap_call = FALSE;
    MN_DUAL_SYS_E           dual_sys;
    
    dual_sys = sig_ptr->dual_sys;

    if(ATC_HOLD_CALLS_AND_ACCEPT_OTHERS == atc_config_ptr->ss_operate)
    {
        if (CheckPresentActiveCall(&g_calls_state[dual_sys], &active_id))
        {
            if (CheckPresentHeldCall(&g_calls_state[dual_sys], &held_id))
                is_swap_call = TRUE;
        }
    }

    if (sig_ptr->req_is_accepted)
    {
        call_index = GetIndexOfCallContext(sig_ptr->call_id,dual_sys);
        
        SCI_ASSERT(ATC_INVALID_CALLID!=call_index); /*assert verified check call index*/

        if(ATC_CALL_MPTY==g_calls_state[dual_sys].call_context[call_index].mpty_state)
        {
            for(i=0; i<g_calls_state[dual_sys].call_amount; i++)
            {
                if(ATC_CALL_MPTY==g_calls_state[dual_sys].call_context[i].mpty_state)
                {
                    g_calls_state[dual_sys].call_context[i].hold_state = ATC_CALL_HOLD;
                }
            }
        }
        else
        {
            g_calls_state[dual_sys].call_context[call_index].hold_state = ATC_CALL_HOLD;
        }

        if(ATC_HOLD_CALLS_AND_ACCEPT_OTHERS == atc_config_ptr->ss_operate)
        {
            if (CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
            {
                MNCALL_ConnectCallEx(dual_sys, call_id, PNULL);
                if (!atc_config_ptr->audio_flag)
                {
                    AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
                    AUDIO_SetVolume(atc_config_ptr->speaker_volume);
                    ATC_EnableVoiceCodec(TRUE);
                         atc_config_ptr->audio_flag = TRUE;
                    g_atc_call_flag = TRUE;
                }
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SETUP_COMPLETE_IND,call_id,ATC_DOMAIN_CS);
            }
            else
            {
                if(FALSE == is_swap_call)
                // reponse the at+chld=3
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
            }
        }
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
        atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the APP_MN_SPLIT_MPTY_CNF signal 
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessSplitMptyCnf(
                    ATC_CONFIG_T            *atc_config_ptr,
                    APP_MN_SPLIT_MPTY_CNF_T  *sig_ptr
                    )
{
    uint8   call_index;
    int32   i;    
    MN_DUAL_SYS_E dual_sys;
    
    dual_sys = sig_ptr->dual_sys;

    if (sig_ptr->req_is_accepted)
    {
        call_index = GetIndexOfCallContext(sig_ptr->call_id,dual_sys);
        
        SCI_ASSERT(ATC_INVALID_CALLID!=call_index); /*assert verified check call index*/

        g_calls_state[dual_sys].call_context[call_index].mpty_state = ATC_MPTY_IDLE;
        g_calls_state[dual_sys].call_context[call_index].hold_state = ATC_HOLD_IDLE;
        for(i=0; i<g_calls_state[dual_sys].call_amount; i++)
        {
            if(ATC_CALL_MPTY==g_calls_state[dual_sys].call_context[i].mpty_state)
            {
                g_calls_state[dual_sys].call_context[i].hold_state = ATC_CALL_HOLD;
            }
        }

        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
    }
    atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_RETRIEVED_IND signal and 
//                sends the +CSSU: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessRetrievedInd(
                    ATC_CONFIG_T                *atc_config_ptr,
                    APP_MN_CALL_RETRIEVED_IND_T *sig_ptr
                    )
{
    SCI_ASSERT((PNULL != atc_config_ptr) && (PNULL != sig_ptr));/*assert verified*/

    if (atc_config_ptr->cssu_flag)
    {
        sprintf((char*)g_rsp_str, "+CSSU: 3");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }
    return S_ATC_DEFAULT_HANDLE;    
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_BUILD_MPTY_IND signal and 
//                sends the +CSSU: unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessBuildMptyInd(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_BUILD_MPTY_IND_T  *sig_ptr
                )
{
    SCI_ASSERT((PNULL != atc_config_ptr) && (PNULL != sig_ptr));/*assert verified*/

    if (atc_config_ptr->cssu_flag)
    {
        sprintf((char*)g_rsp_str, "+CSSU: 4");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

        return S_ATC_SUCCESS;
    }
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handle the SS request through ATD command  
//  Global resource dependence : none
//  Author:       shijun.cui
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_CheckSSStr(
            ATC_CONFIG_T   *atc_config_ptr,
            uint8          *str_ptr,
            uint8          length
            )
{
    MN_OPERATION_E          operation;
    ERR_MNSS_CODE_E         result;

    MN_PARSE_INFO_U         ss_info;
    uint8                   *old_passwd_ptr;
    uint8                   *new_passwd_ptr;
    uint8                   *new_passwd_again_ptr;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    SCI_ASSERT(str_ptr != NULL);/*assert verified*/
    
    //SCI_TRACE_LOW("ATC: length = %d, str_ptr = %s",length, str_ptr);
    
    SCI_MEMSET(&ss_info, 0, sizeof(ss_info));
    
    operation = MNPHONE_ParseBStringEx(dual_sys, str_ptr, length, &ss_info);
    
    //SCI_TRACE_LOW("ATC: MNPHONE_ParseBString operation = %d!",operation);
    
    if (MN_SS_OPERATION == operation)
    {
        old_passwd_ptr = ss_info.ss_parse_info.ss_info_u.ss_pwd_info.old_pwd;
        new_passwd_ptr = ss_info.ss_parse_info.ss_info_u.ss_pwd_info.new_pwd;
        new_passwd_again_ptr = ss_info.ss_parse_info.ss_info_u.ss_pwd_info.new_pwd_two;
        
        SCI_MEMCPY(s_old_password, old_passwd_ptr, AT_CLCK_PSW_LEN);
        SCI_MEMCPY(s_new_password, new_passwd_ptr, AT_CLCK_PSW_LEN);
        SCI_MEMCPY(s_new_password_again, new_passwd_again_ptr, AT_CLCK_PSW_LEN);
        
        return ATC_SendReqToSs(atc_config_ptr,&ss_info);
    }
    else if (MN_USSD_OPERATION == operation || MN_UNKONOWN_OPERATION == operation)
    {
        //Send the original requst to MN.
        //SCI_TRACE_LOW("ATC: begain MNSS_UserOriginateUSSD");
        if(FALSE == s_cusd_isresponse)
        {           
            result = MNSS_UserOriginateUSSDEx(dual_sys, FALSE,MN_SS_DEFAULT_DCS_VALUE, length, str_ptr);
            if(ERR_MNSS_NO_ERR != result)
            {
                //SCI_TRACE_LOW:"ATC: MNSS_UserOriginateUSSD result = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3573_112_2_17_22_17_23_678,(uint8*)"d",result);
                return ERR_PHONE_FAILURE;
            }
            else
            {
                s_cusd_isresponse = TRUE;
                s_cusd_presentation = AT_CUSD_ENABLE;
            }
        }
        else
        {
            //Send the response to MN.
            //SCI_TRACE_LOW("ATC: begain MNSS_ResponseUSSD");           
            result = MNSS_ResponseUSSDEx(dual_sys, MN_SS_DEFAULT_DCS_VALUE, length, str_ptr);
            if(ERR_MNSS_NO_ERR != result)
            {
                //SCI_TRACE_LOW:"ATC: MNSS_ResponseUSSD result = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3589_112_2_17_22_17_23_679,(uint8*)"d",result);
                return ERR_PHONE_FAILURE;
            }
        }
        return S_ATC_SUCCESS;
    }

    // neither ss or ussd service
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : retrieve the cmd id from ss code
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL int16 RetrieveCmdId(
                   MN_SS_CODE_E ss_code
                   )
{
    int16 cmd_id=0;
    
    switch(ss_code)
    {
        #ifdef _GSM_ONLY_ 
        case MN_SS_CLIP:
        {
            cmd_id = AT_CMD_CLIP;
            break;
        }
        #endif
        #if 0
        case MN_SS_CLIR:
        {
            cmd_id = AT_CMD_CLIR;
            break;
        }
        #endif
        #ifdef _GSM_ONLY_    
        case MN_SS_COLP:
        {
            cmd_id = AT_CMD_COLP;
            break;
        }
        #endif
        #if 0    
        case MN_SS_COLR:
        {
            cmd_id = AT_CMD_COLR;
            break;
        }
        #endif
        /* PS do not support MN_SS_CNAP query operation now*/
        /*    
        case MN_SS_CNAP:
            break;
        */
        
        case MN_SS_ALL_FORWARD:
        case MN_SS_CFU:
        case MN_SS_ALL_COND_FORWARD:
        case MN_SS_CFB:
        case MN_SS_CFNRY:
        case MN_SS_CFNRC:
        {
            cmd_id = AT_CMD_CCFC;
            break;
        }
            
        case MN_SS_CW:
        {
            cmd_id = AT_CMD_CCWA;
            break;
        }
    
        /* PS do not support these query operations now*/
        /*
        case MN_SS_HOLD:
            break;
        case MN_SS_MC:
            break;
        case MN_SS_MULTIPTY:
            break;
        case MN_SS_CUG:
            break;
        case MN_SS_AOCI:
            break;
        case MN_SS_AOCC:
            break;
        */

        case MN_SS_ALL_BAR_SERVICE:
        case MN_SS_BAR_OUTGOING:
        case MN_SS_BAOC:
        case MN_SS_BOIC:
        case MN_SS_BOIC_EXHC:
        case MN_SS_BAR_INCOMING:
        case MN_SS_BAIC:
        case MN_SS_BICROAM:
        {
            cmd_id = AT_CMD_CLCK;
            break;
        }
    
        default:
        {
            //SCI_TRACE_LOW:"ATC: unknow ss code = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3694_112_2_17_22_17_24_680,(uint8*)"d", ss_code);
            break;
        }
    }
    
    return cmd_id;
}

/*****************************************************************************/
//  Description : Send stand ss request to MN
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL ATC_STATUS ATC_SendReqToSs(
                          ATC_CONFIG_T              *atc_config_ptr,                          
                          MN_PARSE_INFO_U           *ss_info_ptr
                          )
{
    ERR_MNSS_CODE_E         err_code;
    MN_SS_CODE_E            ss_code;
    MN_SS_OPERATION_CODE_E  ss_oper;
    MN_SS_BASIC_SERVICE_T   basic_service;
    MN_SS_REGISTER_PARAM_U  register_param;
    ATC_STATUS              status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    ss_oper = ss_info_ptr->ss_parse_info.ss_oper;
    ss_code = ss_info_ptr->ss_parse_info.ss_code;    
    basic_service = ss_info_ptr->ss_parse_info.ss_info_u.ss_basic_info.basic_service;
    register_param = ss_info_ptr->ss_parse_info.ss_info_u.ss_basic_info.register_param;
    
    switch (ss_oper)
    {
        case MN_SS_REGISTERSS:
        {
            //register call forwarding        
            err_code = MNSS_RegisterSSInfoEx(dual_sys, ss_code,basic_service,register_param);
            if (err_code != ERR_MNSS_NO_ERR)
            {
                //SCI_TRACE_LOW:"ATC: mn register ss error code = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3732_112_2_17_22_17_24_681,(uint8*)"d",err_code);
                status = ERR_PHONE_FAILURE;
            }
            else
            {
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_REGISTER_SS_CNF,0xff,ATC_NO_DOMAIN);
            }
            break;
        }

        case MN_SS_ERASESS:
        {
            //disable call forwarding        
            err_code = MNSS_EraseSSInfoEx(dual_sys, ss_code,basic_service);
            if (err_code != ERR_MNSS_NO_ERR)
            {
                //SCI_TRACE_LOW:"ATC: mn erase ss info error code = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3748_112_2_17_22_17_24_682,(uint8*)"d",err_code);
                status = ERR_PHONE_FAILURE;
            }
            else
            {
                 ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ERASE_SS_CNF,0xff,ATC_NO_DOMAIN);
            }
            break;
        }

        case MN_SS_ACTIVATESS:
        {
            //activate call waiting and bar        
            err_code = MNSS_ActivateSSEx(dual_sys, ss_code,basic_service);
            if (err_code != ERR_MNSS_NO_ERR)
            {
                //SCI_TRACE_LOW:"ATC: mn active ss error code %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3764_112_2_17_22_17_24_683,(uint8*)"d",err_code);
                status = ERR_PHONE_FAILURE;
            }
            else
            {
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ACTIVATE_SS_CNF,0xff,ATC_NO_DOMAIN);
            }
            break;
        }

        case MN_SS_DEACTIVATESS:
        {
            //deactivate call waiting and bar
            err_code = MNSS_DeactivateSSEx(dual_sys, ss_code,basic_service);
            if (err_code != ERR_MNSS_NO_ERR)
            {
                //SCI_TRACE_LOW:"ATC: mn deactive ss error code = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3780_112_2_17_22_17_24_684,(uint8*)"d",err_code);
                status = ERR_PHONE_FAILURE;
            }
            else
            {
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_DEACTIVATE_SS_CNF,0xff,ATC_NO_DOMAIN);
            }
            break;
        }

        case MN_SS_INTERROGATESS:
        {
            //interrogate status
            s_ss_context.cmd_id = RetrieveCmdId(ss_code);
            err_code = MNSS_InterrogateServiceEx(dual_sys, ss_code,basic_service);
            if (err_code != ERR_MNSS_NO_ERR)
            {
                //SCI_TRACE_LOW:"ATC: mn interrogate service error code = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3797_112_2_17_22_17_24_685,(uint8*)"d",err_code);
                status = ERR_PHONE_FAILURE;
            }
            else
            {
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_INTERROGATE_CNF,0xff,ATC_NO_DOMAIN);
            }
            break;
        }
            
        case MN_SS_REGISTERPASSWORD:
        {
            ss_code = (MN_SS_CODE_E)(ss_info_ptr->ss_parse_info.ss_info_u.ss_pwd_info.code);
            //SCI_TRACE_LOW("ATC: registerpassword ss_code = %d",ss_code);        
            err_code = MNSS_RegisterPasswordEx(dual_sys, ss_code);
            if (err_code != ERR_MNSS_NO_ERR)
            {
                //SCI_TRACE_LOW:"ATC: mn register ss password error code = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3814_112_2_17_22_17_24_686,(uint8*)"d",err_code);
                status = ERR_PHONE_FAILURE;
            }
            else
            {
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_REGISTER_PASSWORD_CNF,0xff,ATC_NO_DOMAIN);
            }
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: SendReqToSs in default!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_3826_112_2_17_22_17_24_687,(uint8*)"");
            status = ERR_PHONE_FAILURE;
            break;
        }
    }
    
    return status;
}
#endif /* _ULTRA_LOW_CODE_ */

/*****************************************************************************/
//  Description : This function handle the AT+CLIR command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCLIR )
{
#ifndef _ULTRA_LOW_CODE_
    MN_SS_BASIC_SERVICE_T      service_info; 
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    service_info.service_type = MN_SS_ALL_SERVICE;
    service_info.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                if((BOOLEAN)PARA1.num)
                {
                    atc_config_ptr->atc_config_nv.clir_flag = TRUE;
                }
                else
                {
                    atc_config_ptr->atc_config_nv.clir_flag = FALSE;
                }
            }
            else
            {
                return ERR_INVALID_INDEX;
            }       
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            MNSS_InterrogateServiceEx(dual_sys, MN_SS_CLIR, service_info);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_INTERROGATE_CNF,0xff,ATC_NO_DOMAIN);
            
            s_ss_context.cmd_id = AT_CMD_CLIR;
            return S_ATC_DEFAULT_HANDLE;
        }

        default:
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CUSD command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCUSD )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_CUR_PARAM_STRING_T *ptr_para2_str         = PNULL;
    ATC_STATUS              status                = S_ATC_SUCCESS;
    uint8                   data_code_scheme      = MN_SS_DEFAULT_DCS_VALUE;
    int32                   para3_num             = 0;
    ATC_CUR_PARAM_STRING_T  ptr_para_str;
    uint8                   ussd_str[MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE+1] = {0};
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        // AT+CUSD?
        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d,String,%d",g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr,
                s_cusd_presentation,data_code_scheme);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }

        // AT+CUSD=?
        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char*)g_rsp_str, "%s: (0-2),,",g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }

        // AT+CUSD=<Index>,<str>,<dsc>
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                if(PARAM3_FLAG)
                {
                    para3_num = PARA3.num;
                }
                
                if(PARAM2_FLAG)
                {
                    if(0 == strcmp((char*)(atc_config_ptr->cscs), "UCS2"))
                    {
                        //SCI_TRACE_LOW:"ATC: PARA2.str_ptr->str_len is %d, str is %s"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_4133_112_2_17_22_17_25_688,(uint8*)"ds",PARA2.str_ptr->str_len,PARA2.str_ptr->str_ptr);
                        ATC_ConvertUcs2HexToAscii(PARA2.str_ptr->str_ptr,PARA2.str_ptr->str_len, ussd_str);
                        ptr_para_str.str_len = PARA2.str_ptr->str_len / 4;
                        ussd_str[ptr_para_str.str_len] = '\0';
                        ptr_para_str.str_ptr = ussd_str;
                        ptr_para2_str = &ptr_para_str;
                        //SCI_TRACE_LOW:"ATC: ptr_para2_str str_len is %d, str is %s"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_4139_112_2_17_22_17_25_689,(uint8*)"ds",ptr_para2_str->str_len,ptr_para2_str->str_ptr);
                    }
                    else if(0 == strcmp((char*)(atc_config_ptr->cscs), "IRA"))
                    {
                        //SCI_TRACE_LOW:"ATC: PARA2.str_ptr->str_len is %d, str is %s"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_4143_112_2_17_22_17_25_690,(uint8*)"ds",PARA2.str_ptr->str_len,PARA2.str_ptr->str_ptr);
                        ConvertHexToBin(PARA2.str_ptr->str_ptr,PARA2.str_ptr->str_len, ussd_str);
                        ptr_para_str.str_len = PARA2.str_ptr->str_len / 2;
                        ussd_str[ptr_para_str.str_len] = '\0';
                        ptr_para_str.str_ptr = ussd_str;
                        ptr_para2_str = &ptr_para_str;
                        //SCI_TRACE_LOW:"ATC: ptr_para2_str str_len is %d, str is %s"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SS_4149_112_2_17_22_17_25_691,(uint8*)"ds",ptr_para2_str->str_len,ptr_para2_str->str_ptr);
                    }
                    else
                    {
                        ptr_para2_str = PARA2.str_ptr;
                    }
                }

                status = HandleUssdReq(dual_sys,PARAM1_FLAG,PARA1.num,PARAM2_FLAG,ptr_para2_str,PARAM3_FLAG,para3_num);
                if(status == S_ATC_DEFAULT_HANDLE)
                {
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_USSD_SERVICE_CNF,0xff,ATC_NO_DOMAIN);
                }
                ptr_para2_str = PNULL;
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }

        default:
            break;
    }
    return status;
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
}

#if 0
/*****************************************************************************/
//  Description : This function handle the AT+CSSN command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSSN )
{
#ifndef _ULTRA_LOW_CODE_
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                // set the values
                atc_config_ptr->cssi_flag = (uint8)PARA1.num;
            }
            if (PARAM2_FLAG)
            {
                atc_config_ptr->cssu_flag = (uint8)PARA2.num;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str,"%s: %d,%d", g_atc_info_table[AT_CMD_CSSN].cmd_name->str_ptr,
                atc_config_ptr->cssi_flag,atc_config_ptr->cssu_flag);
            // the read command response
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handle the AT+COLR command 
//  Global resource dependence : none
//  Author:       Shijun.cui
//  Note:         for CR31660
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCOLR )
{
#ifndef _ULTRA_LOW_CODE_
    MN_SS_BASIC_SERVICE_T      service_info; 
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    service_info.service_type = MN_SS_ALL_SERVICE;
    service_info.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
        {
            // interrogate the COLR service status
            MNSS_InterrogateServiceEx(dual_sys, MN_SS_COLR, service_info);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_INTERROGATE_CNF,0xff,ATC_NO_DOMAIN);
            s_ss_context.cmd_id = AT_CMD_COLR;
            break;
        }
    
        default:
            break;
    }
    
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CPUC command.The set command sets 
//the parameters for Advice of Charge related to price per unit and the currency
// table in SIM file EFPUCT.  
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPUC )
{
#ifndef _ULTRA_LOW_CODE_
    MN_BLOCK_T               chv2;
    MNSIM_PIN_STATUS_E       chv2_status; 
    ERR_MNPHONE_CODE_E       err_code;
    uint8 temp_str[MN_MAX_CURRENCY_CODE_NUM*2+1];
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    switch (ATC_GET_CMD_TYPE)
    {
        // AT+CPUC?The return result will be handle in ATC_ProcessPuctCnf()
        case ATC_CMD_TYPE_READ:
        {
            atc_config_ptr->operate = ATC_PIN_GET_PPU;      
            MNSIM_GetPuctEx(dual_sys);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_PUCT_INFO_CNF,0xff,ATC_NO_DOMAIN);
            break;
        }

        // AT+CACM=<Curr>,<PPC>,<passward>
        case ATC_CMD_TYPE_SET:
        {
            //check the parameter
            if( !PARAM1_FLAG || !PARAM2_FLAG )
            {
                return ERR_INVALID_INDEX;
            }

            if (PARAM1_FLAG)
            {
                if (PARA1.str_ptr->str_len > MN_MAX_CURRENCY_CODE_NUM)
                {
                    return ERR_INVALID_INDEX;
                }
                else
                {
                    //Get and assign the curr value.
                    if(strcmp((char*)atc_config_ptr->cscs, "UCS2") == 0)
                    {
                        SCI_MEMSET(&temp_str[0], 0, MN_MAX_CURRENCY_CODE_NUM*2+1);
                        if(strlen((char *)(PARA1.str_ptr->str_ptr))!=MN_MAX_CURRENCY_CODE_NUM*4)
                        {
                            return ERR_UNKNOWN;
                        }
                        
                        ConvertAscUcs2IntoBin((char *)&temp_str[0], PARA1.str_ptr->str_ptr, MN_MAX_CURRENCY_CODE_NUM);
                        temp_str[0]=temp_str[1];
                        temp_str[1]=temp_str[3];
                        temp_str[2]=temp_str[5];
                        temp_str[3]=0;
                        
                        SCI_MEMCPY(atc_config_ptr->ppc.currency_code, &temp_str[0], MN_MAX_CURRENCY_CODE_NUM);
                    }
                    else
                    {
                        SCI_MEMCPY(atc_config_ptr->ppc.currency_code, PARA1.str_ptr->str_ptr, MN_MAX_CURRENCY_CODE_NUM);
                    }
                }
            }

            if (PARAM2_FLAG)
            {
                //Get the curr value.
                atc_config_ptr->ppc.eppu=PARA2.num;
            }

            atc_config_ptr->ppc.ex_sign=FALSE;
            // to verify the CHV2 and update the PPU
            if (PARAM3_FLAG)
            {
                if (PARA3.str_ptr->str_len > MNSIM_PIN_MAX_LEN)
                {
                    return ERR_INVALID_INDEX;
                }
                else
                {
                    SCI_MEMCPY(chv2.blockbuf, PARA3.str_ptr->str_ptr, PARA3.str_ptr->str_len);
                    chv2.blocklen = PARA3.str_ptr->str_len;             
                    err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_VERIFY, MNSIM_PIN2, &chv2, PNULL);
                    if(ERR_MNPHONE_NO_ERR == err_code)
                    {
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                    }
                    atc_config_ptr->operate = ATC_PIN_SET_PPU;
                    return S_ATC_DEFAULT_HANDLE;
                }
            }
            else
            {           
                chv2_status = MNSIM_GetPinStatusEx(dual_sys, MNSIM_PIN2);
                
                if (MNSIM_PIN_READY == chv2_status)
                {
                    MNSIM_UpdatePuctEx(dual_sys, atc_config_ptr->ppc);
                }
                else
                {
                    return ERR_SIM_PIN2_REQUIRED;
                }
            }
            break;
        }

        default: 
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handle the AT+CACM command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCACM )
{
#ifndef _ULTRA_LOW_CODE_
    MNSIM_AOC_T              aoc_context;
    MN_BLOCK_T               chv2;
    MNSIM_PIN_STATUS_E       chv2_status; 
    ERR_MNPHONE_CODE_E       err_code; 
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        // AT+CACM?
        case ATC_CMD_TYPE_READ:
        {
            aoc_context = MNSIM_GetAocEx(dual_sys);
            sprintf((char*)g_rsp_str, "%s: %lx", g_atc_info_table[AT_CMD_CACM].cmd_name->str_ptr,
                aoc_context.acm_unit);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

            // AT+CACM=[<passward>]
        case ATC_CMD_TYPE_SET:
        {
            // to verify the CHV2
            if (PARAM1_FLAG)
            {
                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    return ERR_INCORRECT_PASSWORD;
                }
                
                if (PARA1.str_ptr->str_len > MNSIM_PIN_MAX_LEN)
                {
                    return ERR_INVALID_INDEX;
                }
                else
                {
                    SCI_MEMCPY(chv2.blockbuf, PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len);
                    chv2.blocklen = PARA1.str_ptr->str_len;             
                    err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_VERIFY, MNSIM_PIN2, &chv2, PNULL);
                    if(err_code == ERR_MNPHONE_NO_ERR)
                    {
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                    }
                    
                    atc_config_ptr->operate = ATC_PIN_RESET_ACM_UNIT;
                    return S_ATC_DEFAULT_HANDLE;
                }
            }
            else
            {           
                chv2_status = MNSIM_GetPinStatusEx(dual_sys, MNSIM_PIN2);
                if (MNSIM_PIN_READY == chv2_status)
                {               
                    MNSIM_InitAcmUnitEx(dual_sys);
                }
                else
                {
                    return ERR_SIM_PIN2_REQUIRED;
                }
            }
            break;
        }

        default:
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handle the AT+CAMM command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCAMM )
{
#ifndef _ULTRA_LOW_CODE_
    MN_BLOCK_T               chv2;
    MNSIM_PIN_STATUS_E       chv2_status; 
    MNSIM_AOC_T              aoc_context;
    ERR_MNPHONE_CODE_E       err_code;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        // AT+CAMM?
        case ATC_CMD_TYPE_READ:
        {
            aoc_context = MNSIM_GetAocEx(dual_sys);
            sprintf((char*)g_rsp_str, "%s: %lx", g_atc_info_table[AT_CMD_CAMM].cmd_name->str_ptr, 
                aoc_context.acm_max);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_CMD_TYPE_SET:
        {
            // check the parameter 1
            if (PARAM1_FLAG)
            {
                atc_config_ptr->acm_max = PARA1.num;
            }
            else
            {
                atc_config_ptr->acm_max = 0;
            }
            // check the parameter 2
            if (PARAM2_FLAG)
            {
                if (PARA2.str_ptr->str_len > MNSIM_PIN_MAX_LEN)
                {
                    return ERR_INVALID_INDEX;
                }
                else
                {
                    SCI_MEMCPY(chv2.blockbuf, PARA2.str_ptr->str_ptr, PARA2.str_ptr->str_len);
                    chv2.blocklen = PARA2.str_ptr->str_len;             
                    err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_VERIFY, MNSIM_PIN2, &chv2, PNULL);
                    if(err_code == ERR_MNPHONE_NO_ERR)
                    {
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                    }
                    atc_config_ptr->operate = ATC_PIN_SET_ACM_MAX;
                    return S_ATC_DEFAULT_HANDLE;
                }           
            }
            else
            {
                // get the chv2 status          
                chv2_status = MNSIM_GetPinStatusEx(dual_sys, MNSIM_PIN2);
                if (MNSIM_PIN_READY == chv2_status)
                {               
                    MNSIM_UpdateAcmMaxEx(dual_sys, atc_config_ptr->acm_max);
                }
                else
                {
                    return ERR_SIM_PIN2_REQUIRED;
                }
            }
            break;
        }

        default:
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handles the AT+SAC command, This command is used
//  to abort the current SS service of PLMN searching.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSAC )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS    status = S_ATC_SUCCESS;
       MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            if (MNSS_GetCurStateEx(dual_sys))
            {
                s_cusd_isresponse = FALSE;
                // abort the ss service         
                MNSS_RelActivingServiceEx(dual_sys);
            }
            
            if (MNPHONE_IsPLMNSearchEx(dual_sys))
            {
                // abort the PLMN searching         
                MNPHONE_AbortPLMNSearchEx(dual_sys);
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            // check the current state of SS service and ifthe MS is searching PLMN
            if (MNSS_GetCurStateEx(dual_sys) || MNPHONE_IsPLMNSearchEx(dual_sys))
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }

        default:
            break;
    }
    return status;
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
