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
#include "atc_gsm_ss.h"
#include "atc_gsm.h"
#include "atc_common.h"
#include "atc_basic_cmd.h"
#include "atc_stk.h"
#include "record.h"
#include "env_atc_signal.h"
#include "sig_code.h"
#include "sci_codec.h"
#include "mn_api.h"//MNSIM_GetFDNStatus_Req
//#include "audio_prod.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/
#define PROVISIONED_BIT      0x04   //bit2 is provision bit
static ATC_SS_CONTEXT_T      s_ss_context[MN_SYS_NUMBER];
static uint8                 s_clck_psw[AT_CLCK_PSW_LEN];
static uint8                 s_old_password[AT_CLCK_PSW_LEN];    // the old pin
static uint8                 s_new_password[AT_CLCK_PSW_LEN];    // the new pin
static uint8                 s_new_password_again[AT_CLCK_PSW_LEN];// the new pin again

static uint8                 s_cusd_presentation[MN_SYS_NUMBER] = {0}; //AT_CUSD_DISABLE
static BOOLEAN               s_cusd_isresponse[MN_SYS_NUMBER] = {0}; //FALSE

static HAUDIO hAtcVoiceRing = INVALID_HANDLE;

/*lint -save -e752 */
extern MN_DUAL_SYS_E         g_current_card_id;//lint !e752
extern RSP_RESULT_STR        g_rsp_str;
extern uint8                 g_tel_str[2*MN_MAX_ADDR_BCD_LEN + 1];
extern const ATC_INFO_T      g_atc_info_table[];
#ifndef ATC_SAT_ENABLE
extern BOOLEAN               g_is_stk_call[MN_SYS_NUMBER];
#endif
extern BOOLEAN               g_atc_call_flag;
extern BOOLEAN               g_atc_call_need_disc;
extern BOOLEAN               g_atc_expire_ind;
extern ATC_CALLS_STATE_T     g_calls_state[MN_SYS_NUMBER];
extern SCI_TMID              T_ATC_CALL_EXP_IND;
extern BOOLEAN               g_atc_pcm_flag;
extern uint8                 s_mt_need_open_codec_flag;
/*lint -save -e552 */
extern uint8                 g_atc_expire_call_id;//lint !e552
extern ATC_LAST_CALLED_NUM_T g_last_mo_call;

#ifdef _MUX_ENABLE_
extern ATC_MUX_MOD_E         g_atc_mux_mode;
extern uint8                       atc_urc_link_id;
#endif

/*****************************************************************************/
//  Description : This function handles release all of the held call or release
//  release the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldUdub(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
);

/*****************************************************************************/
//  Description : This function handles release all of the held call or release
//  release the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelAcceptOthers(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
);

/*****************************************************************************/
//  Description : This function hold the active calls and accept the held or waiting calls
//  release the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldHoldAcceptOthers(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
);

/*****************************************************************************/
//  Description : This function adds the held call to the conversation calls
//  release the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldMultCall(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E  dual_sys
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
//  Description : This function handles release active call and accept held call
//  Global resource dependence : none
//  Author:       Elvis Xu
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelAcceptHeld(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
);

/*****************************************************************************/
//  Description : This function handles releases all existing calls
//  Global resource dependence : none
//  Author:       longting.zhao
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelAllCalls(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
);

/*****************************************************************************/
//  Description : This function handles AT+CHLD=1X release an active call 
//  Global resource dependence : none
//  Author:       longting.zhao
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelActiveCall(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys,
     int32         operation_code
);

/*****************************************************************************/
//  Description : This function handles AT+CHLD=2X split call from mulitiparty call
//  Global resource dependence : none
//  Author:       longting.zhao
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldSplitCallFromMpty(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys,
     int32         operation_code
);

/*****************************************************************************/
//  Description : This function handles AT+CHLD=7X release specific call
//  Global resource dependence : none
//  Author:       longting.zhao
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelSpecificCall(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys,
     int32         operation_code
);

/*****************************************************************************/
//  Description : This function handles hold active call only
//  Global resource dependence : none
//  Author:       longting.zhao
//  Note:
/*****************************************************************************/
LOCAL void HandleChldHoldActiveCall(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
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
//  Description : This function convert the ATC call bar reason code
//                to the MNSS's code
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:
/*****************************************************************************/
LOCAL void ConvertCallBarCode(
    uint8                 fac_type,   // the call forwarded reason
    MN_SS_CODE_E          *ss_code_ptr  // the SS code
);

/*****************************************************************************/
//  Description : This function check the SS CNF signal
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:        modify by longting.zhao 2011.8.26 add parameter to notify app error code
/*****************************************************************************/
LOCAL BOOLEAN ATC_ProcessSSCnf(
   ATC_CONFIG_T              *atc_config_ptr,
   APP_MN_INTERROGATE_CNF_T   *sig_ptr,
   BOOLEAN     op_type 
);

/*****************************************************************************/
//  Description : This function handle APP_MN_INTERROGATE_CNF signal
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_CheckInterrogateCnf(
    ATC_CONFIG_T              *atc_config_ptr,
    APP_MN_INTERROGATE_CNF_T *sig_ptr,
    BOOLEAN              *is_service_active
);

/*****************************************************************************/
//  Description : This function handle the SS service in AT+CLCK.
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
#ifdef _MUX_ENABLE_
LOCAL ATC_STATUS HandleClckSSReq(
       MN_DUAL_SYS_E   dual_sys,
       ATC_CONFIG_T   *atc_config_ptr,
       MN_SS_CODE_E    ss_code,
       uint8           operate_type,
       uint8           class_type
);
#else
LOCAL ATC_STATUS HandleClckSSReq(
       MN_DUAL_SYS_E   dual_sys,
       MN_SS_CODE_E    ss_code,
       uint8           operate_type,
       uint8           class_type
);
#endif

#ifdef _SUPPORT_CCBS_
/*****************************************************************************/
//  Description : This function handle APP_MN_CCBS_ERASE_CNF message
//  Global resource dependence :
//  Author:     Haoming Lu
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_HandleCCBSInd(
                                            ATC_CONFIG_T* atc_config_ptr,
                                            MN_DUAL_SYS_E   dual_sys,
                                            BOOLEAN is_ccbs_accept);

/*****************************************************************************/
//  Description : This function handle at cmd about recall ind
//  Global resource dependence :
//  Author:     Haoming Lu
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_HandleRecallIndCMD(
                                            ATC_CONFIG_T* atc_config_ptr,
                                            MN_DUAL_SYS_E   dual_sys,
                                            BOOLEAN is_recall_accept);

#endif // #ifdef _SUPPORT_CCBS_


/*****************************************************************************/
//  Description : This function initializes the static variables for SS module
//  Global resource dependence :
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
void ATC_InitSsVar(void)
{
    uint8 i;

    for (i = 0; i < MN_SYS_NUMBER; i++)
    {
        s_cusd_presentation[i] = AT_CUSD_DISABLE;
        s_cusd_isresponse[i]   = FALSE;
        SCI_MEMSET(&s_ss_context[i], 0, sizeof(ATC_SS_CONTEXT_T));
        s_ss_context[i].cmd_id = (int16)AT_INVALID_CMD_ID;
        s_ss_context[i].ss_code = (MN_SS_CODE_E)0;
    }

}

/*****************************************************************************/
//  Description : Send stand ss request to MN
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL ATC_STATUS ATC_SendReqToSs(
    ATC_CONFIG_T              *atc_config_ptr,
    MN_PARSE_INFO_U           *ss_info_ptr,
    MN_DUAL_SYS_E   dual_sys
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

/*****************************************************************************/
//  Description : get service class from MN_SS_INTERROGATE_INFO_T(from MN)
//  Global resource dependence : none
//  Author:
//  Note: NA
/*****************************************************************************/
LOCAL uint16 ATC_getclass_from_interrogate_cnf(
    MN_SS_INTERROGATE_INFO_T *interrogate_info,
    int list_num
);

/*****************************************************************************/
//  Description : get service class from MN_SS_INTERROGATE_INFO_T(from MN)
//                the status of the service is not necessarily active. 
//  Global resource dependence : none
//  Author: Haoming.Lu
//  Note: NA
/*****************************************************************************/
LOCAL uint16 ATC_getclass_from_info_list(
    MN_SS_INTERROGATE_INFO_T *interrogate_info
);

LOCAL BOOLEAN ATC_class_to_basicservice(
    uint8 *service_type,
    MN_SS_BASIC_SERVICE_T *basic_service,
    BOOLEAN is_query_mode
);

/*****************************************************************************/
//  Description : Audio dummy notify callback function.
//  Global resource dependence : none
//  Author: shujing.dong
//  Note: 
/*****************************************************************************/
static void AudioDummyNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //do nothing
}

#ifndef ATC_SAT_ENABLE
static uint32* atc_bt_call_buffer=PNULL;
static uint32* atc_bt_call_buffer_up=PNULL;
static HAUDIO atchBluePlay=0;
static HAUDIO atchBlueRec=0;
static uint32 bt_mem_flag = 0;
#define ATC_BLUERING_BUF_SIZE (480)

/*****************************************************************************/
//  Description:    This function is used to set bt voice codec enable/disable.
//  Author:        
//  Note:
/*****************************************************************************/
AUDIO_RESULT_E ATC_EnableBtVoiceCodec( BOOLEAN is_enable )
{
    AUDIO_RESULT_E  ret = SCI_SUCCESS;

#ifdef _MUX_ENABLE_
    SCI_TRACE_LOW("ATC_EnableBtVoiceCodec is_enable=%d", is_enable);


    if(is_enable)
    {
        if(bt_mem_flag == 0)
        {
            atc_bt_call_buffer = (uint32 *)SCI_ALLOC_BASE(ATC_BLUERING_BUF_SIZE*4);
            SCI_ASSERT(PNULL!=atc_bt_call_buffer);/*assert verified*/
            atc_bt_call_buffer_up = (uint32 *)SCI_ALLOC_BASE(ATC_BLUERING_BUF_SIZE*4);
            SCI_ASSERT(PNULL!=atc_bt_call_buffer_up);/*assert verified*/
            bt_mem_flag = 1;
            
            if(PNULL == atc_bt_call_buffer || PNULL ==atc_bt_call_buffer_up)
            {
                SCI_TRACE_LOW("ATC: atc_bt_call_buffer==0x%X, atc_bt_call_buffer_up==0x%X\r\n", atc_bt_call_buffer,atc_bt_call_buffer_up);
                ret = AUDIO_NO_ENOUGH_MEMORY;
                return ret;
            }
        }

        if(INVALID_HANDLE==atchBluePlay)
        {
            atchBluePlay = AUDIO_CreateBufferHandle(hDownlinkCodec, SCI_NULL, hPCMDev, SCI_NULL, SCI_NULL, 0, atc_bt_call_buffer, ATC_BLUERING_BUF_SIZE, AudioDummyNotifyCallback);
            SCI_TRACE_LOW("atchBluePlay = %d",atchBluePlay);
        }

        if(INVALID_HANDLE==atchBlueRec)
        {
            atchBlueRec = AUDIO_CreateRecordBufferHandle(hUplinkCodec, SCI_NULL, hPCMDev, SCI_NULL,(const uint8*)atc_bt_call_buffer_up, 0,  atc_bt_call_buffer_up, 0, AudioDummyNotifyCallback); //lint !e605
            SCI_TRACE_LOW("atchBluePlay = %d",atchBlueRec);
        } 

        if(INVALID_HANDLE == atchBluePlay || INVALID_HANDLE == atchBlueRec)
        {
            SCI_TRACE_LOW("ATC: atchBluePlay==0x%X, atchBlueRec==0x%X\r\n", atchBluePlay,atchBlueRec);
            ret=AUDIO_HANDLE_IS_INVALID;
            return ret;
        }

        ret = AUDIO_Play(atchBluePlay, 0); 
        SCI_TRACE_LOW("ATC: AUDIO_Play result=0x%X\r\n", ret);
        
        ret = AUDIO_Play(atchBlueRec, 0);    
        SCI_TRACE_LOW("ATC: AUDIO_Play Rec result=0x%X\r\n", ret);
    }
    else
    {
        if(INVALID_HANDLE != atchBluePlay)
        {
            ret = AUDIO_Stop(atchBluePlay);
            SCI_TRACE_LOW("ATC: AUDIO_Stop result=0x%X\r\n", ret);
            AUDIO_CloseHandle(atchBluePlay);
            atchBluePlay = NULL;
        }
        if(INVALID_HANDLE != atchBlueRec)
        {
            ret = AUDIO_Stop(atchBlueRec);
            SCI_TRACE_LOW("ATC: AUDIO_Stop Rec result=0x%X\r\n", ret);

            AUDIO_CloseHandle(atchBlueRec);
            atchBlueRec = NULL;
        }

        if((atc_bt_call_buffer!=NULL)&&(atc_bt_call_buffer_up != NULL))
        {
            SCI_FREE(atc_bt_call_buffer);
            SCI_FREE(atc_bt_call_buffer_up);
            atc_bt_call_buffer = NULL;
            atc_bt_call_buffer_up = NULL;
            bt_mem_flag = 0;
        }
    }

#endif
    return ret;
}

/*****************************************************************************/
//  Description : This function handle the AT+COLP command
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCOLP)
{
#ifndef _ULTRA_LOW_CODE_
    MN_SS_BASIC_SERVICE_T      service_info;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ERR_MNSS_CODE_E err_code;

    service_info.service_type = MN_SS_ALL_SERVICE;
    service_info.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
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

        case ATC_CMD_TYPE_READ:
            // interrogate the COLP service status
            err_code = MNSS_InterrogateServiceEx(dual_sys, MN_SS_COLP, service_info);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_INTERROGATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = AT_CMD_COLP;
                s_ss_context[dual_sys].ss_code = MN_SS_COLP;
                return S_ATC_DEFAULT_HANDLE;
            }
            else if (err_code == ERR_MNSS_FDN_FAILED)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCOLP, FDN check failed");
                return ERR_SIM_FDN_FAILED;
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCOLP, MNSS_InterrogateService, failure:%d", err_code);
                return ERR_OPERATION_NOT_ALLOWED;
            }
            
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
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
AT_CMD_FUNC(ATC_ProcessCOLR)
{
#ifndef _ULTRA_LOW_CODE_
    MN_SS_BASIC_SERVICE_T      service_info;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ERR_MNSS_CODE_E err_code;

    service_info.service_type = MN_SS_ALL_SERVICE;
    service_info.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
            // interrogate the COLR service status
            err_code = MNSS_InterrogateServiceEx(dual_sys, MN_SS_COLR, service_info);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_INTERROGATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = AT_CMD_COLR;
                s_ss_context[dual_sys].ss_code = MN_SS_COLR;
            }
            else if (err_code == ERR_MNSS_FDN_FAILED)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCOLR, FDN check failed");
                return ERR_SIM_FDN_FAILED;
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCOLR, MNSS_InterrogateService, failure:%d", err_code);
                return ERR_OPERATION_NOT_ALLOWED;
            }
            
            break;
            
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CLIP command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCLIP)
{
#ifndef _ULTRA_LOW_CODE_
    MN_SS_BASIC_SERVICE_T      service_info;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ERR_MNSS_CODE_E err_code;

    service_info.service_type = MN_SS_ALL_SERVICE;
    service_info.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
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

        case ATC_CMD_TYPE_READ:
            err_code = MNSS_InterrogateServiceEx(dual_sys, MN_SS_CLIP, service_info);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                      APP_MN_INTERROGATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = AT_CMD_CLIP;
                s_ss_context[dual_sys].ss_code = MN_SS_CLIP;
                return S_ATC_DEFAULT_HANDLE;
            }
            else if (err_code == ERR_MNSS_FDN_FAILED)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCLIP, FDN check failed");
                return ERR_SIM_FDN_FAILED;
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCLIP, MNSS_InterrogateService, failure:%d", err_code);
                return ERR_OPERATION_NOT_ALLOWED;
            }

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CLIR command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCLIR)
{
#ifndef _ULTRA_LOW_CODE_
    MN_SS_BASIC_SERVICE_T      service_info;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ERR_MNSS_CODE_E err_code;

    service_info.service_type = MN_SS_ALL_SERVICE;
    service_info.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                atc_config_ptr->atc_config_nv.clir_flag = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:
            err_code = MNSS_InterrogateServiceEx(dual_sys, MN_SS_CLIR, service_info);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_INTERROGATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = AT_CMD_CLIR;
                s_ss_context[dual_sys].ss_code = MN_SS_CLIR;
                return S_ATC_DEFAULT_HANDLE;
            }
            else if (err_code == ERR_MNSS_FDN_FAILED)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCLIR, FDN check failed");
                return ERR_SIM_FDN_FAILED;
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCLIR, MNSS_InterrogateService, failure:%d", err_code);
                return ERR_OPERATION_NOT_ALLOWED;
            }
            
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handle the AT+CAOC command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCAOC)
{
#ifndef _ULTRA_LOW_CODE_
    switch(ATC_GET_CMD_TYPE)
    {
            // AT+CAOC
        case ATC_CMD_TYPE_EXECUTE:
            sprintf((char *)g_rsp_str, "%s: \"%06lx\"",
                    g_atc_info_table[AT_CMD_CAOC].cmd_name->str_ptr,
                    atc_config_ptr->ccm_val);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

            // AT+CAOC?
        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d",
                    g_atc_info_table[AT_CMD_CAOC].cmd_name->str_ptr,
                    atc_config_ptr->aoc_mode);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

            // AT+CAOC=
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                atc_config_ptr->aoc_mode = (uint8)PARA1.num;
                SCI_TRACE_LOW("ATC: user set the AOC mode");

                /* 0: query CCM value */
                if(0 == atc_config_ptr->aoc_mode)
                {
                    sprintf((char *)g_rsp_str, "%s: \"%06lx\"",
                            g_atc_info_table[AT_CMD_CAOC].cmd_name->str_ptr,
                            atc_config_ptr->ccm_val);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handle the AT+CCWE command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCCWE)
{
#ifndef _ULTRA_LOW_CODE_

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(!PARAM1_FLAG)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCWE, no param1");
                return ERR_INVALID_INDEX;
            }

            // set the values
            atc_config_ptr->ccwe_flag = (uint8)PARA1.num;
            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d",
                    g_atc_info_table[AT_CMD_CCWE].cmd_name->str_ptr,
                    atc_config_ptr->ccwe_flag);
            // the read command response
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

#endif /* _ULTRA_LOW_CODE_ */

    return S_ATC_SUCCESS;
}
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
#ifdef _MUX_ENABLE_
    SCI_TRACE_LOW ("ATC_EnableVoiceCodec: is_enable = %d", is_enable);

    if(INVALID_HANDLE==hAtcVoiceRing)
    {
        hAtcVoiceRing = AUDIO_CreateNormalHandle(hVoiceCodec, SCI_NULL, hLAYER1, SCI_NULL, AudioDummyNotifyCallback);
        SCI_ASSERT(INVALID_HANDLE != hAtcVoiceRing); /*assert verified*/
    }
    if (is_enable)
    {        
        AUDIO_Play(hAtcVoiceRing, 0);
        
    #ifndef ATC_SAT_ENABLE
        if(g_atc_pcm_flag)
        {
            ATC_EnableBtVoiceCodec(TRUE);
        }
    #endif
    }
    else
    {
        AUDIO_Stop(hAtcVoiceRing);
        
    #ifndef ATC_SAT_ENABLE
        if(INVALID_HANDLE!=atchBluePlay || INVALID_HANDLE!=atchBlueRec)
        {
            SCI_TRACE_LOW("ATC_EnableVoiceCodec bt close");
            ATC_EnableBtVoiceCodec(FALSE);
        }
    #endif
    }
#endif
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
AT_CMD_FUNC(ATC_ProcessCCFC)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_OPERATION_MODE_E       operation_mode; // holds the operation mode
    ATC_CCFC_REASON_E          ccfc_reason;
    uint8                    service_class = 255;
    MN_SS_CODE_E               ss_code;
    MN_SS_BASIC_SERVICE_T      basic_service;
    BOOLEAN                    temp_flag;
    ATC_STATUS                 re_value = S_ATC_DEFAULT_HANDLE;
    ERR_MNSS_CODE_E         err_code;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(ATC_CMD_TYPE_TEST == ATC_GET_CMD_TYPE)
    {
        sprintf((char *)g_rsp_str, "%s: (0-5)", g_atc_info_table[AT_CMD_CCFC].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
        return S_ATC_SUCCESS;
    }

    SCI_ASSERT(ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE);/*assert verified: check valid value*/

    // check if the parameter1 and parameter2 are present or not.
    if(!PARAM1_FLAG || !PARAM2_FLAG)
    {
        return ERR_INVALID_INDEX;
    }

    // get the call forwarded operation mode
    operation_mode = (ATC_OPERATION_MODE_E)PARA2.num;

    if((PARAM4_FLAG && PARA4.num != 128 && PARA4.num != 129 && PARA4.num != 145 && PARA4.num != 161) ||
            (PARAM7_FLAG && PARA7.num != 0 && PARA7.num != 128 && PARA7.num != 136 && PARA7.num != 160 && PARA7.num != 168))
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessCCFC,  param 4 or 7 wrong values!");
        return ERR_INVALID_INDEX;
    }

    // get the parameter call forwarded reason
    ccfc_reason    = (ATC_CCFC_REASON_E)PARA1.num;
    // get the MNSS ss_code
    ConvertCfCode(ccfc_reason, &ss_code);

    if(PARAM5_FLAG)
    {
        service_class = PARA5.num;
    }
    else
    {
        service_class = 7;
    }

    s_ss_context[dual_sys].input_class = service_class;

    switch(operation_mode)
    {
            // deactivate the Call forwarded service
        case ATC_DISABLE:

            if(!ATC_class_to_basicservice(&service_class, &basic_service, FALSE))
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, Class not allowed");
                return ERR_OPERATION_NOT_ALLOWED;
            }

            err_code = MNSS_DeactivateSSEx(dual_sys, ss_code, basic_service);

            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_DEACTIVATE_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = AT_CMD_CCFC;
                s_ss_context[dual_sys].ss_code = ss_code;
                s_ss_context[dual_sys].service_class = service_class;
            }
            else if (err_code == ERR_MNSS_FDN_FAILED)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, FDN check failed");
                return ERR_SIM_FDN_FAILED;
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, MNSS_DeactivateSS, failure:%d", err_code);
                return ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_ENABLE:

            if(!ATC_class_to_basicservice(&service_class, &basic_service, FALSE))
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, Class not allowed");
                return ERR_OPERATION_NOT_ALLOWED;
            }

            err_code = MNSS_ActivateSSEx(dual_sys, ss_code, basic_service);

            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_ACTIVATE_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = AT_CMD_CCFC;
                s_ss_context[dual_sys].ss_code = ss_code;
                s_ss_context[dual_sys].service_class = service_class;
            }
            else if (err_code == ERR_MNSS_FDN_FAILED)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, FDN check failed");
                return ERR_SIM_FDN_FAILED;
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, MNSS_ActivateSS, failure:%d", err_code);
                return ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_QUERY_STATUS:

            if(ATC_class_to_basicservice(&service_class, &basic_service, TRUE) != TRUE)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, Class not allowed");
                return ERR_OPERATION_NOT_ALLOWED;
            }

            err_code = MNSS_InterrogateServiceEx(dual_sys, ss_code, basic_service);

            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_INTERROGATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = AT_CMD_CCFC;
                s_ss_context[dual_sys].ss_code = ss_code;
                s_ss_context[dual_sys].service_class = service_class;
            }
            else if (err_code == ERR_MNSS_FDN_FAILED)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, FDN check failed");
                return ERR_SIM_FDN_FAILED;
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, MNSS_InterrogateService, failure:%d", err_code);
                return ERR_OPERATION_NOT_ALLOWED;
            }

            break;

            // registrer the CF(call forwarded service)
        case ATC_REGISTRATION:
            {
                MN_SS_REGISTER_PARAM_U  register_param = {0};
                uint8                   str_len = 0;

                if(!PARAM3_FLAG)
                {
                    ATC_TRACE_LOW("ATC: reg fail1");
                    re_value = ERR_INVALID_INDEX;
                    break;
                }

                if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, param3 is not string!");
                    re_value = ERR_INVALID_INDEX;
                    break;
                }

                if(((char)*PARA3.str_ptr->str_ptr) == '+')
                {
                    str_len = PARA3.str_ptr->str_len - 1;
                }
                else
                {
                    str_len = PARA3.str_ptr->str_len;
                }

                if(str_len > 2 * MN_MAX_ADDR_BCD_LEN)
                {
                    ATC_TRACE_LOW("ATC: reg fail2");
                    re_value = ERR_INVALID_INDEX;
                    break;
                }

                temp_flag = ATC_ConvertTeleStrToStruct(
                                &(register_param.cfnry_param.forward_num),
                                PARA3.str_ptr->str_ptr,
                                PARA3.str_ptr->str_len);

                if(!temp_flag)
                {
                    ATC_TRACE_LOW("ATC: reg fail3");
                    re_value = ERR_INVALID_INDEX;
                    break;
                }

                if(PARAM6_FLAG)
                {
                    if(ATC_CMD_PARAM_TYPE_STRING != PARAM6_TYPE)
                    {
                        ATC_TRACE_LOW("ATC: Subaddr is not string");
                        re_value = ERR_INVALID_INDEX;
                        break;
                    }
                }

                if(PARAM7_FLAG && 0 == PARA7.num)
                {
                    ATC_TRACE_LOW("ATC: reg fail4");
                    re_value = ERR_INVALID_INDEX;
                    break;
                }

                if(ATC_NO_REPLY == ccfc_reason)
                {
                    if(!PARAM8_FLAG)
                    {
                        register_param.cfnry_param.no_reply_time = 20;
                    }
                    else
                    {
                        register_param.cfnry_param.no_reply_time = (PARA8.num < 5) ? 5 : ((uint8)PARA8.num);
                    }
                }

                if(ATC_class_to_basicservice(&service_class, &basic_service, FALSE) != TRUE)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, Class not allowed");
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                err_code = MNSS_RegisterSSInfoEx(dual_sys, ss_code, basic_service, register_param);

                if(err_code == ERR_MNSS_NO_ERR)
                {
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                      APP_MN_REGISTER_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                    s_ss_context[dual_sys].cmd_id = AT_CMD_CCFC;
                    s_ss_context[dual_sys].ss_code = ss_code;
                    s_ss_context[dual_sys].service_class = service_class;
                }
                else if (err_code == ERR_MNSS_FDN_FAILED)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, FDN check failed");
                    return ERR_SIM_FDN_FAILED;
                }
                else
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, MNSS_RegisterSSInfo, failure:%d", err_code);
                    return ERR_OPERATION_NOT_ALLOWED;
                }
            }
            break;
            // erase the CF service
        case ATC_ERASURE:

            if(ATC_class_to_basicservice(&service_class, &basic_service, FALSE) != TRUE)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, Class not allowed");
                return ERR_OPERATION_NOT_ALLOWED;
            }

            err_code = MNSS_EraseSSInfoEx(dual_sys, ss_code, basic_service);

            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_ERASE_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = AT_CMD_CCFC;
                s_ss_context[dual_sys].ss_code = ss_code;
                s_ss_context[dual_sys].service_class = service_class;
            }
            else if (err_code == ERR_MNSS_FDN_FAILED)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, FDN check failed");
                return ERR_SIM_FDN_FAILED;
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCCFC, MNSS_EraseSSInfo, failure:%d", err_code);
                return ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    ATC_TRACE_LOW("ATC: re_valuse=%x", re_value);

    return(re_value);
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_SETUP_COMPLETE_IND,
//  APP_MN_SETUP_COMPLETE_CNF signal to response the function
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSetupCompleteSignal(
    ATC_CONFIG_T        *atc_config_ptr,
    xSignalHeaderRec    *sig_ptr
)
{
    #ifndef ATC_SAT_ENABLE
    ATC_TELE_TYPE_E     tel_num_type;
    uint8               tel_str[MN_MAX_ADDR_BCD_LEN*2 + 1] = {0};
    #endif
    uint8               call_index = ATC_INVALID_CALLID;
    uint8               call_id = ATC_INVALID_CALLID;
    MN_DUAL_SYS_E       dual_sys = MN_DUAL_SYS_1;


    ATC_CMCC_CALLIND_CALL_TYPE_E cmcc_call_type = ATC_CMCC_CALLIND_CALL_TYPE_INVALID;

    ATC_TRACE_LOW("ATC:ATC_ProcessSetupCompleteSignal");

    switch(sig_ptr->SignalCode)
    {
        case APP_MN_SETUP_COMPLETE_CNF:
            dual_sys    = ((APP_MN_SETUP_COMPLETE_CNF_T *)sig_ptr)->dual_sys;
            call_index  = GetIndexOfCallContext(((APP_MN_SETUP_COMPLETE_CNF_T *)sig_ptr)->call_id, dual_sys);
            call_id     = ((APP_MN_SETUP_COMPLETE_CNF_T *)sig_ptr)->call_id;
            break;

        case APP_MN_SETUP_COMPLETE_IND:
            dual_sys    = ((APP_MN_SETUP_COMPLETE_IND_T *)sig_ptr)->dual_sys;
            call_index  = GetIndexOfCallContext(((APP_MN_SETUP_COMPLETE_IND_T *)sig_ptr)->call_id, dual_sys);
            call_id     = ((APP_MN_SETUP_COMPLETE_IND_T *)sig_ptr)->call_id;
            break;

        default:
            SCI_ASSERT(FALSE);/*assert verified: check valid value*/
            break;
    }

    if(ATC_INVALID_CALLID == call_index)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessSetupCompleteSignal, invalid call index.");
        return S_ATC_FAIL;
    }

    g_calls_state[dual_sys].call_context[call_index].call_entity_state = ATC_CALL_ACTIVE_STATE;

    if(!CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
    {
        ATC_TRACE_LOW("ATC: setup complete expire timer check.");

        // no wait call, check & kill call exp timer
        if(SCI_IS_TIMER_ACTIVE(P_ATC, ATC_CALL_EXP_IND, &T_ATC_CALL_EXP_IND))
        {
            SCI_KILL_TIMER(P_ATC, ATC_CALL_EXP_IND,
                           &T_ATC_CALL_EXP_IND, "T_ATC_CALL_EXP_IND");
            g_atc_expire_call_id = 0xff;
            g_atc_expire_ind = FALSE;
        }
    }

    // handle the at+chld commands
    if(ATC_INVALID_OPERATE != atc_config_ptr->ss_operate[dual_sys])
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
        return S_ATC_DEFAULT_HANDLE;
    }

    if(!atc_config_ptr->audio_flag && g_atc_call_flag)
    {
        if(ATC_VOICE_MODE == g_calls_state[dual_sys].call_context[0].call_mode)
        {
            AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
            AUDIO_SetVolume(atc_config_ptr->speaker_volume);
            ATC_EnableUplinkMute(FALSE);
            ATC_EnableRemoteMute(FALSE); 
        
            ATC_EnableVoiceCodec(TRUE);
            s_mt_need_open_codec_flag = 0;
        }

        atc_config_ptr->audio_flag = TRUE;
    }
    if( g_atc_call_flag &&
        (ATC_VOICE_MODE == g_calls_state[dual_sys].call_context[0].call_mode) &&
        (APP_MN_SETUP_COMPLETE_CNF == sig_ptr->SignalCode))
    {
        ATC_HandleMoMuteAndLocalRingState(dual_sys, sig_ptr->SignalCode, call_index);
    }

    ATC_UpECINDInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, 0, 0, g_calls_state[dual_sys].call_context[call_index].call_id + 1, 0, 0);

    ATC_UpDSCIInfo(dual_sys, atc_config_ptr, sig_ptr, call_index, 0);


    if(g_calls_state[dual_sys].call_context[call_index].call_mode == ATC_VOICE_MODE)
    {
        cmcc_call_type = ATC_CMCC_CALLIND_CALL_TYPE_CIRCUIT_VOICE;
    }
    else if(g_calls_state[dual_sys].call_context[call_index].call_mode == ATC_DATA_MODE)
    {
        cmcc_call_type = ATC_CMCC_CALLIND_CALL_TYPE_CIRCUIT_DATA;
    }

    sprintf((char *)g_rsp_str, "^CONN: %d,%d", g_calls_state[dual_sys].call_context[call_index].call_id + 1, cmcc_call_type);
    ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);

    //Send the unsolicited +WIND: 9
    ATC_UpSINDInfo(dual_sys,
                   atc_config_ptr,
                   PNULL,
                   ATC_WIND_BIT8,
                   9,
                   (BOOLEAN)(0 != atc_config_ptr->atc_config_nv.creg));

    //if call is set up by STK
#ifndef ATC_SAT_ENABLE
    if(TRUE == g_is_stk_call[dual_sys])
    {
        ATC_StkProcessCallSignals(TRUE, dual_sys,call_id,ATC_STK_CALL_SUCC,MN_CAUSE_ERROR_NONE);
    }
#endif

    atc_config_ptr->cpas = ATC_CPAS_STATUS_PROGRESS;

    //if COLP  is enable ,show the connected phone number.
#ifndef ATC_SAT_ENABLE
    if(atc_config_ptr->atc_config_nv.colp_flag &&
            APP_MN_SETUP_COMPLETE_CNF == sig_ptr->SignalCode)
    {
        if(((APP_MN_SETUP_COMPLETE_CNF_T *)sig_ptr)->connected_num_present)
        {
            ATC_ConvertCallingNumToStr(&(((APP_MN_SETUP_COMPLETE_CNF_T *)sig_ptr)->connected_num), &tel_num_type, tel_str);
        }
        else
        {
            SCI_MEMCPY(tel_str, g_tel_str, 2 * MN_MAX_ADDR_BCD_LEN + 1);
        }

        sprintf((char *)g_rsp_str, "%s: \"%s\",129 ",
                g_atc_info_table[AT_CMD_COLP].cmd_name->str_ptr, tel_str);
        ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
    }
#endif
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+CCWA command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCCWA)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_OPERATION_MODE_E       operation_code; // holds the CALL WAITING SS operation code
    MN_SS_BASIC_SERVICE_T      basic_service;
    uint8                      service_class = 255;
    ATC_STATUS                 status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E              dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ERR_MNSS_CODE_E            err_code ;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(!PARAM1_FLAG)
            {
                if(PARAM2_FLAG || PARAM3_FLAG)
                {
                    return ERR_INVALID_INDEX;
                }

                break;
            }

            atc_config_ptr->ccwa_flag[dual_sys] = (uint8)PARA1.num;

            if(!PARAM2_FLAG)
            {
                if(PARAM3_FLAG)
                {
                    status =  ERR_INVALID_INDEX;
                    break;
                }

                // return the result OK
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                return S_ATC_DEFAULT_HANDLE;
            }
            else
            {
                if(PARAM3_FLAG)  /* we only support voice class now */
                {
                    service_class = (uint8) PARA3.num;
                }
                else
                {
                    service_class = 7;
                }

                s_ss_context[dual_sys].input_class = service_class;

                operation_code = (ATC_OPERATION_MODE_E)PARA2.num;

                if(!ATC_class_to_basicservice(&service_class, &basic_service, (BOOLEAN)(operation_code == ATC_QUERY_STATUS)))
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_OPERATION_NOT_SUPPORTED);
                    return S_ATC_DEFAULT_HANDLE;
                }

                switch(operation_code)
                {
                        // deactivate the Call forwarded service
                    case ATC_DISABLE:
                        err_code = MNSS_DeactivateSSEx(dual_sys, MN_SS_CW, basic_service);
                        if(ERR_MNSS_NO_ERR == err_code)
                        {
                            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                          APP_MN_DEACTIVATE_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                        }
                        else if (err_code == ERR_MNSS_FDN_FAILED)
                        {
                            ATC_TRACE_LOW("ATC: ATC_ProcessCCWA, FDN check failed");
                            return ERR_SIM_FDN_FAILED;
                        }
                        else 
                        {
                            ATC_TRACE_LOW("ATC: ATC_ProcessCCWA, MNSS_DeactivateSS, failure:%d", err_code);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                        break;

                    case ATC_ENABLE:
                        err_code = MNSS_ActivateSSEx(dual_sys, MN_SS_CW, basic_service);
                        if(ERR_MNSS_NO_ERR == err_code)
                        {
                            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                          APP_MN_ACTIVATE_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                        }
                        else if (err_code == ERR_MNSS_FDN_FAILED)
                        {
                            ATC_TRACE_LOW("ATC: ATC_ProcessCCWA, FDN check failed");
                            return ERR_SIM_FDN_FAILED;
                        }
                        else 
                        {
                            ATC_TRACE_LOW("ATC: ATC_ProcessCCWA, MNSS_ActivateSS, failure:%d", err_code);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                        break;

                    case ATC_QUERY_STATUS:
                        basic_service.service_type = MN_SS_ALL_SERVICE;
                        err_code = MNSS_InterrogateServiceEx(dual_sys, MN_SS_CW, basic_service);
                        if(err_code == ERR_MNSS_NO_ERR)
                        {
                            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                              APP_MN_INTERROGATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                            s_ss_context[dual_sys].cmd_id = AT_CMD_CCWA;
                            s_ss_context[dual_sys].ss_code = MN_SS_CW;
                            s_ss_context[dual_sys].service_class = service_class;
                        }
                        else if (err_code == ERR_MNSS_FDN_FAILED)
                        {
                            ATC_TRACE_LOW("ATC: ATC_ProcessCCWA, FDN check failed");
                            return ERR_SIM_FDN_FAILED;
                        }
                        else
                        {
                            ATC_TRACE_LOW("ATC: ATC_ProcessCCWA, MNSS_InterrogateService, failure:%d", err_code);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                        break;

                    default:
                        SCI_ASSERT(FALSE);/*assert verified: check valid value*/
                        break;
                }
            }

            status = S_ATC_DEFAULT_HANDLE;
            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CCWA].cmd_name->str_ptr, atc_config_ptr->ccwa_flag[dual_sys]);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (0-1)", g_atc_info_table[AT_CMD_CCWA].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return status;
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
AT_CMD_FUNC(ATC_ProcessCACM)
{
#ifndef _ULTRA_LOW_CODE_
    MNSIM_AOC_T              aoc_context;
    MN_BLOCK_T               chv2;
    MNSIM_PIN_STATUS_E       chv2_status;
    ERR_MNPHONE_CODE_E  err_code;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
            // AT+CACM=?
        case ATC_CMD_TYPE_TEST:
            break;

            // AT+CACM?
        case ATC_CMD_TYPE_READ:
            aoc_context = MNSIM_GetAocEx(dual_sys);
            sprintf((char *)g_rsp_str, "%s: \"%06lx\"", g_atc_info_table[AT_CMD_CACM].cmd_name->str_ptr,
                    aoc_context.acm_unit);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

            // AT+CACM=[<passward>]
        case ATC_CMD_TYPE_SET:

            // to verify the CHV2
            if(PARAM1_FLAG)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCACM 1");

                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    return ERR_INCORRECT_PASSWORD;
                }

                if(PARA1.str_ptr->str_len > MNSIM_PIN_MAX_LEN)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCACM 1-1");
                    return ERR_INVALID_INDEX;
                }
                else
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCACM 1-2");
                    SCI_MEMCPY(chv2.blockbuf, PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len);
                    chv2.blocklen = PARA1.str_ptr->str_len;
                    err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_VERIFY, MNSIM_PIN2, &chv2, PNULL);

                    if(err_code == ERR_MNPHONE_NO_ERR)
                    {
                        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                          APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }

                    atc_config_ptr->operate[dual_sys] = ATC_PIN_RESET_ACM_UNIT;
                    return S_ATC_DEFAULT_HANDLE;
                }
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCACM 2(1)");
                chv2_status = MNSIM_GetPinStatusEx(dual_sys, MNSIM_PIN2);
                ATC_TRACE_LOW("ATC: ATC_ProcessCACM 2(2)");

                if(MNSIM_PIN_READY == chv2_status)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCACM 2-1");
                    MNSIM_InitAcmUnitEx(dual_sys);
                }
                else
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCACM 2-2");
                    return ERR_SIM_PIN2_REQUIRED;
                }
            }

            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
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
AT_CMD_FUNC(ATC_ProcessCAMM)
{
#ifndef _ULTRA_LOW_CODE_
    MN_BLOCK_T               chv2;
    MNSIM_PIN_STATUS_E       chv2_status;
    MNSIM_AOC_T              aoc_context;
    ERR_MNPHONE_CODE_E  err_code;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
            // AT+CAMM=?
        case ATC_CMD_TYPE_TEST:
            //sprintf((char*)g_rsp_str, "%s: (\"000000-FFFFFF\")", g_atc_info_table[AT_CMD_CAMM].cmd_name->str_ptr);
            //ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

            // AT+CAMM?
        case ATC_CMD_TYPE_READ:
            aoc_context = MNSIM_GetAocEx(dual_sys);
            sprintf((char *)g_rsp_str, "%s: \"%06lx\"", g_atc_info_table[AT_CMD_CAMM].cmd_name->str_ptr,
                    (aoc_context.acm_max & 0xFFFFFF));
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_SET:

            // check the parameter 1
            if(PARAM1_FLAG)
            {
                uint8  hex[6+1] = {'0', '0', '0', '0', '0', '0', 0};
                uint8  bin[3] = {0};
                uint32 bin_value = 0;

                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCAMM, param1 is not string!");
                    return ERR_INVALID_INDEX;
                }

                if(PARA1.str_ptr->str_len == 0)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCAMM, param1 length is 0!");
                    return ERR_INVALID_INDEX;
                }

                if(PARA1.str_ptr->str_len > 6)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCAMM, param1 length is too long >6 !");
                    return ERR_INVALID_INDEX;
                }

                sprintf((char *)(hex + 6 - PARA1.str_ptr->str_len), "%s", PARA1.str_ptr->str_ptr);

                if(FALSE == ConvertHexToBin(hex, 6, bin))
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCAMM, param1 format error!");
                    return ERR_INVALID_INDEX;
                }

                bin_value = (((uint32)bin[0] << 16) | ((uint32)bin[1] << 8) | ((uint32)bin[2]));
                atc_config_ptr->acm_max[dual_sys] = bin_value;
                //atc_config_ptr->acm_max[dual_sys] = PARA1.num;
            }
            else
            {
                atc_config_ptr->acm_max[dual_sys] = 0;
            }

            // check the parameter 2
            if(PARAM2_FLAG)
            {
                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCAMM, param2 is not string!");
                    return ERR_INVALID_INDEX;
                }

                if(PARA2.str_ptr->str_len > MNSIM_PIN_MAX_LEN)
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
                        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                          APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }

                    atc_config_ptr->operate[dual_sys] = ATC_PIN_SET_ACM_MAX;
                    return S_ATC_DEFAULT_HANDLE;
                }
            }
            else
            {
                // get the chv2 status
                chv2_status = MNSIM_GetPinStatusEx(dual_sys, MNSIM_PIN2);

                if(MNSIM_PIN_READY == chv2_status)
                {
                    MNSIM_UpdateAcmMaxEx(dual_sys, atc_config_ptr->acm_max[dual_sys]);
                }
                else
                {
                    return ERR_SIM_PIN2_REQUIRED;
                }
            }

            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

BOOLEAN ATC_DecStrToE(int str_len, char *str_ptr, BOOLEAN *ex_sign, uint8 *ex, uint16 *e)
{
    int     i, digital_cnt = 0;
    char   *byte_ptr;
    BOOLEAN ex_sign_tmp = FALSE;
    uint8   ex_tmp = 0;
    uint16  e_tmp = 0;

    if((str_len <= 0) || (!str_ptr))
    {
        ATC_TRACE_LOW("ATC: ATC_DecStrToE, no valid string data!");
        return FALSE;
    }

    for(i = 0, byte_ptr = str_ptr;
            i < str_len;
            i++, byte_ptr ++)
    {
        if(*byte_ptr == '.')
        {
            if(TRUE == ex_sign_tmp)
            {
                ATC_TRACE_LOW("ATC: ATC_DecStrToE, more '.'");
                return TRUE;
            }

            /* negative sign */
            ex_sign_tmp = TRUE;
        }
        else if((*byte_ptr < '0') || (*byte_ptr > '9'))
        {
            ATC_TRACE_LOW("ATC: ATC_DecStrToE, NOT '0' ~ '9'!");
            return FALSE;
        }
        else
        {
            /* e is uint16!
             * so "65535", "655.35" */
            if(5 == digital_cnt)
            {
                ATC_TRACE_LOW("ATC: ATC_DecStrToE, too many digits!");
                return FALSE;
            }
            else if((4 == digital_cnt) && (e_tmp == 6553) && (*byte_ptr > '5'))
            {
                ATC_TRACE_LOW("ATC: ATC_DecStrToE, too big value!");
                return FALSE;
            }
            else
            {
                digital_cnt++;

                if(TRUE == ex_sign_tmp)
                {
                    ex_tmp++;
                }

                e_tmp *= 10;
                e_tmp += ((*byte_ptr) - '0');
            }
        }
    }

    *ex_sign = ex_sign_tmp;
    *ex = ex_tmp;
    *e = e_tmp;
    return TRUE;
}

/*****************************************************************************/
//  Description : This function handles the AT+CPUC command.The set command sets
//  the parameters for Advice of Charge related to price per unit and the currency
//  table in SIM file EFPUCT.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPUC)
{
#ifndef _ULTRA_LOW_CODE_
    MN_BLOCK_T               chv2;
    MNSIM_PIN_STATUS_E       chv2_status;
    ERR_MNPHONE_CODE_E  err_code;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
            // AT+CPUC=?
        case ATC_CMD_TYPE_TEST:
            break;

            // AT+CPUC?The return result will be handle in ATC_ProcessPuctCnf()
        case ATC_CMD_TYPE_READ:
            atc_config_ptr->operate[dual_sys] = ATC_PIN_GET_PPU;
            MNSIM_GetPuctEx(dual_sys);
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_PUCT_INFO_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            return S_ATC_DEFAULT_HANDLE;

            // AT+CACM=<Curr>,<PPC>,<passward>
        case ATC_CMD_TYPE_SET:

            //check the parameter
            if(!PARAM1_FLAG || !PARAM2_FLAG)
            {
                return ERR_INVALID_INDEX;
            }

            if(PARAM1_FLAG)
            {
                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCPUC, param1 is not string!");
                    return ERR_INVALID_INDEX;
                }

                if(PARA1.str_ptr->str_len > MN_MAX_CURRENCY_CODE_NUM)
                {
                    return ERR_INVALID_INDEX;
                }
                else
                {
                    //Get and assign the curr value.
                    SCI_MEMCPY(atc_config_ptr->ppc[dual_sys].currency_code, PARA1.str_ptr->str_ptr, MN_MAX_CURRENCY_CODE_NUM);
                }
            }

            if(PARAM2_FLAG)
            {
                BOOLEAN ex_sign = FALSE;                // the sign of ex,TRUE positive,FALSE is negative
                uint8    ex = 0;
                uint16    eppu = 0;                    // price per unit,eppu*10^ex

                //Get the curr value.
                //atc_config_ptr->ppc[dual_sys].eppu=PARA2.num;
                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCPUC, param2 is not string!");
                    return ERR_INVALID_INDEX;
                }

                if(PARA2.str_ptr->str_len == 0)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCPUC, param2 len is 0");
                    return ERR_INVALID_INDEX;
                }

                if(FALSE == ATC_DecStrToE((int)PARA2.str_ptr->str_len, (char *)PARA2.str_ptr->str_ptr,
                                          &ex_sign, &ex, &eppu))
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCPUC, ATC_DecStrToE error!");
                    return ERR_INVALID_INDEX;
                }
                else
                {
                    atc_config_ptr->ppc[dual_sys].ex_sign = ex_sign;
                    atc_config_ptr->ppc[dual_sys].ex = ex;
                    atc_config_ptr->ppc[dual_sys].eppu = eppu;
                }
            }

            //atc_config_ptr->ppc[dual_sys].ex_sign=FALSE;
            // to verify the CHV2 and update the PPU
            if(PARAM3_FLAG)
            {
                if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCPUC, param3 is not string!");
                    return ERR_INVALID_INDEX;
                }

                if(PARA3.str_ptr->str_len > MNSIM_PIN_MAX_LEN)
                {
                    return ERR_INVALID_INDEX;
                }
                else
                {
                    SCI_MEMCPY(chv2.blockbuf, PARA3.str_ptr->str_ptr, PARA3.str_ptr->str_len);
                    chv2.blocklen = PARA3.str_ptr->str_len;
                    err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_VERIFY, MNSIM_PIN2, &chv2, PNULL);

                    if(err_code == ERR_MNPHONE_NO_ERR)
                    {
                        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                          APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                          APP_MN_PUCT_INFO_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }

                    atc_config_ptr->operate[dual_sys] = ATC_PIN_SET_PPU;
                    return S_ATC_DEFAULT_HANDLE;
                }
            }
            else
            {
                chv2_status = MNSIM_GetPinStatusEx(dual_sys, MNSIM_PIN2);

                if(MNSIM_PIN_READY == chv2_status)
                {
                    MNSIM_UpdatePuctEx(dual_sys, atc_config_ptr->ppc[dual_sys]);
                }
                else
                {
                    return ERR_SIM_PIN2_REQUIRED;
                }
            }

            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}
/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_CHV_FUNC_CNF signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
ATC_STATUS ATC_ProcessChvCnf(
    ATC_CONFIG_T                   *atc_config_ptr,
    APP_MN_SIM_PIN_FUNC_CNF_T      *sig_ptr
)
{
    MNSIM_PIN_REQ_RESULT_E      chv_status;
    MN_DUAL_SYS_E     dual_sys = sig_ptr->dual_sys;

    chv_status = sig_ptr->pin_operate_result;
    ATC_TRACE_LOW("ATC: ATC_PROCESSCHVCNF, chv_status=%d, pin_function_type=%d", chv_status, sig_ptr->pin_function_type);

    switch(chv_status)
    {
        case MNSIM_PIN_REQ_OK:
            {
                switch(sig_ptr->pin_function_type)
                {
                    case MNSIM_PIN_DISABLE:
                        if (sig_ptr->pin_num == MNSIM_PIN1)
                        {
                            atc_config_ptr->pin1_enable_flag[dual_sys] = FALSE;
                            atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_READY;
                        }
                        break;

                    case MNSIM_PIN_ENABLE:
                        if (sig_ptr->pin_num == MNSIM_PIN1)
                        {
                            atc_config_ptr->pin1_enable_flag[dual_sys] = TRUE;
                            atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_READY;
                        }
                        break;
                        //handle the cnf signal of AT+CACM,AT+CAMM,AT+CPUC
                    case MNSIM_PIN_VERIFY:

                        switch(atc_config_ptr->operate[dual_sys])
                        {
                            case ATC_PIN_RESET_ACM_UNIT:
                                ATC_TRACE_LOW("ATC: ATC_PROCESSCHVCNF, ATC_PIN_RESET_ACM_UNIT");
                                MNSIM_InitAcmUnitEx(dual_sys);
                                break;

                            case ATC_PIN_SET_ACM_MAX:
                                MNSIM_UpdateAcmMaxEx(dual_sys, atc_config_ptr->acm_max[dual_sys]);
                                break;

                            case ATC_PIN_SET_PPU:
                                //The result will be shown in ATC_ProcessPuctCnf()
                                MNSIM_UpdatePuctEx(dual_sys, atc_config_ptr->ppc[dual_sys]);
                                return S_ATC_SUCCESS;

                            case ATC_PIN_ENABLE_FDN:
                                ATC_TRACE_LOW("ATC: ATC_PROCESSCHVCNF, ATC_PIN_ENABLE_FDN");
                                if(PHONEBOOK_SetFDNServiceEx(dual_sys, TRUE, P_ATC) == ERR_MNDATAMAG_NO_ERR)
                                {
                                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_FDN_SERVICE_IND, 0xff, ATC_NO_DOMAIN, dual_sys);
                                    return S_ATC_DEFAULT_HANDLE;
                                }
                                break;

                            case ATC_PIN_DISABLE_FDN:
                                ATC_TRACE_LOW("ATC: ATC_PROCESSCHVCNF, ATC_PIN_DISABLE_FDN");
                                if(PHONEBOOK_SetFDNServiceEx(dual_sys, FALSE, P_ATC) == ERR_MNDATAMAG_NO_ERR)
                                {
                                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_FDN_SERVICE_IND, 0xff, ATC_NO_DOMAIN, dual_sys);
                                    return S_ATC_DEFAULT_HANDLE;
                                }
                                break;

                            default:
                                break;
                        }

                        atc_config_ptr->operate[dual_sys] = ATC_PIN_NO_OPERATE;
                        break;

                    case MNSIM_PIN_UNBLOCK:
                        if (sig_ptr->pin_num == MNSIM_PIN1)
                        {
                            atc_config_ptr->pin1_enable_flag[dual_sys] = TRUE;
                            atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_READY;
                        }
                        else
                        {
                            atc_config_ptr->is_fdn_enable[dual_sys] = TRUE;
                            atc_config_ptr->cpin2[dual_sys] = ATC_CPIN_CODE_READY;
                        }
                        break;

                    case MNSIM_PIN_CHANGE:
                        ATC_TRACE_LOW("ATC: ATC_PROCESSCHVCNF, unnecessary to change pin status");
                        break;

                    default:
                        ATC_TRACE_LOW("ATC: ATC_PROCESSCHVCNF, wrong pin_function_type %d", sig_ptr->pin_function_type);
                        break;
                }

                atc_config_ptr->cpin_flag[dual_sys] = FALSE;
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            }
            break;

        case MNSIM_PIN_ERROR_WITH_BLOCKED:
            if(sig_ptr->pin_num == MNSIM_PIN1)
            {
                if (sig_ptr->pin_status.pin_blocked)
                {
                    atc_config_ptr->sim_ready[dual_sys] = FALSE;
                    if (sig_ptr->unblock_pin_status.pin_blocked)//Both PIN1 and PUK1 are blocked, that means blocked.
                    {
                        atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_BLOCK;
                        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR , ERR_SIM_CARD_PUK_BLOCKED);
                    }
                    else//Only PIN1 blocked, PUK1 is not blocked.
                    {
                        atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_PUK1;
                        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR , ERR_SIM_PUK_REQUIRED);
                    }
                }
                else
                {
                    //PIN1 is not blocked. PUK1 maybe blocked, maybe not. This is OK!
                    atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_READY;
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_INCORRECT_PASSWORD);
                }
            }
            else
            {
                if (sig_ptr->unblock_pin_status.false_remain > 0)
                {
                    atc_config_ptr->cpin2[dual_sys] = ATC_CPIN_CODE_PUK2;
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR , ERR_SIM_PUK2_REQUIRED);
                }
                else
                {
                    atc_config_ptr->cpin2[dual_sys] = ATC_CPIN_CODE_BLOCK;
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR , ERR_SIM_PUK2_BLOCKED);
                }
                
            }
            break;

        default:
            if (sig_ptr->pin_num == MNSIM_PIN1)
            {
                if (!sig_ptr->pin_status.pin_blocked)
                {
                    atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_READY;
                }
                else
                {
                    if (sig_ptr->unblock_pin_status.false_remain > 0)
                    {
                        atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_PUK1;
                    }
                    else
                    {
                        atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_BLOCK;
                    }
                }
            }
            else
            {
                if (!sig_ptr->pin_status.pin_blocked)
                {
                    atc_config_ptr->cpin2[dual_sys] = ATC_CPIN_CODE_READY;
                }
                else
                {
                    if (sig_ptr->unblock_pin_status.false_remain > 0)
                    {
                        atc_config_ptr->cpin2[dual_sys] = ATC_CPIN_CODE_PUK2;
                    }
                    else
                    {
                        atc_config_ptr->cpin2[dual_sys] = ATC_CPIN_CODE_BLOCK;
                    }
                }
            }

            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_INCORRECT_PASSWORD);
            break;
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_PUCT_INFO_CNF signal
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/

ATC_STATUS ATC_ProcessPuctCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_PUCT_INFO_CNF_T *sig_ptr
)
{
#ifndef _ULTRA_LOW_CODE_

    if(TRUE == sig_ptr->is_ok)
    {
        switch(atc_config_ptr->operate[sig_ptr->dual_sys])
        {
                //if get ppu(price per unit),show it in pc.
            case ATC_PIN_GET_PPU:

                if(0 == sig_ptr->puct_info.ex)
                {
                    sprintf((char *)g_rsp_str, "%s: \"%s\", \"%d\"", g_atc_info_table[AT_CMD_CPUC].cmd_name->str_ptr
                            , sig_ptr->puct_info.currency_code, sig_ptr->puct_info.eppu);
                }
                else
                {
                    uint16 ex_tmp, ex_value = 1;

                    ex_tmp = sig_ptr->puct_info.ex;

                    while(0 != ex_tmp)
                    {
                        ex_value *= 10;
                        ex_tmp --;
                    }

                    ATC_TRACE_LOW("ATC: ATC_ProcessPuctCnf, Read: eppu:%d,ex_sign:%d,ex:%d,ex_value:%d",
                                  sig_ptr->puct_info.eppu,
                                  sig_ptr->puct_info.ex_sign,
                                  sig_ptr->puct_info.ex,
                                  ex_value);

                    if(TRUE == sig_ptr->puct_info.ex_sign)
                    {
                        sprintf((char *)g_rsp_str, "%s: \"%s\", \"%d.%d\"", g_atc_info_table[AT_CMD_CPUC].cmd_name->str_ptr
                                , sig_ptr->puct_info.currency_code
                                , sig_ptr->puct_info.eppu / ex_value
                                , sig_ptr->puct_info.eppu % ex_value);
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "%s: \"%s\", \"%d\"", g_atc_info_table[AT_CMD_CPUC].cmd_name->str_ptr
                                , sig_ptr->puct_info.currency_code
                                , sig_ptr->puct_info.eppu * ex_value);
                    }
                }

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
                break;
                //if read ppu(price per unit),return ok.
            case ATC_PIN_SET_PPU:
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
                break;
            default:
                break;
        }
    }
    else
    {
        sprintf((char *)g_rsp_str, "+CME ERROR: 16");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_SUCCESS;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CHLD command. This command is used
//                to request the call-related supplement service.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCHLD)
{
#ifndef _ULTRA_LOW_CODE_
    int32         operation_code;
    ATC_STATUS    status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT(ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE);/*assert verified: check valid value*/

    if(!PARAM1_FLAG)
    {
        return ERR_INVALID_INDEX;
    }

    operation_code = PARA1.num;

    if(ATC_REL_SPECIFIC_X_CALL_BASE > operation_code)
    {
        switch(operation_code)
        {
                // release all of the held calls or set the UDUB
            case ATC_REL_HELD_CALLS_OR_SET_UDUB:
                status = HandleChldUdub(atc_config_ptr, dual_sys);
                break;

                // release all active calls and accept waiting call or held calls
            case ATC_REL_ACTIVE_CALLS_AND_ACCEPT_OTHERS:
                status = HandleChldRelAcceptOthers(atc_config_ptr, dual_sys);
                break;

                // Place all active calls on hold and accept waiting call or held call
            case ATC_HOLD_CALLS_AND_ACCEPT_OTHERS:
                status = HandleChldHoldAcceptOthers(atc_config_ptr, dual_sys);
                break;

            case ATC_ADD_HELD_TO_CONVERSATION:
                status = HandleChldMultCall(atc_config_ptr,  dual_sys);
                break;

            case ATC_ECT:
                status = HandleEctCall(atc_config_ptr, dual_sys);
            break;


#ifdef _SUPPORT_CCBS_
            case ATC_CCBS_REGISTER:
           {
                uint8 mode = 0;
                
                if (PARAM2_FLAG)
                {
                    mode = PARA2.num;
                }
                else
                {
                    mode = 0;
                }
                
                switch(mode)
                {
                    case 0://Disconnect the call and accept the ccbs function 
                        status = ATC_HandleCCBSInd(atc_config_ptr, dual_sys, TRUE);
                        break;
                       
                    case 1://Disconnect the call, and the user has reject the ccbs ind    
                        status = ATC_HandleCCBSInd(atc_config_ptr, dual_sys, FALSE);
                        break;
                        
                    case 2://Disconnect the call, the user has reject the new recall message
                        status = ATC_HandleRecallIndCMD(atc_config_ptr, dual_sys, FALSE);
                        break;

                    default:
                        status = ERR_INVALID_INDEX;
                        break;                        
                }
                break;       
           }
#endif // #ifdef _SUPPORT_CCBS_

            // release all existing calls
            case ATC_REL_EXISTING_CALLS:
            {
                HandleChldRelAllCalls(atc_config_ptr,  dual_sys);
                break;
            }

            // hold active call only, not care waiting call
            case ATC_HOLD_ACTIVE_CALL_ONLY:
            {
                HandleChldHoldActiveCall(atc_config_ptr,  dual_sys);
                break;
            }

            case ATC_REL_ACTIVE_CALLS_AND_ACCEPT_HELD_CALL:
                status = HandleChldRelAcceptHeld(atc_config_ptr,  dual_sys);
                break;

            default:
                status = ERR_INVALID_INDEX;
                break;
        }
    }
#ifndef ATC_SAT_ENABLE
    // the operation code is 1X
    else if((ATC_REL_SPECIFIC_X_CALL_BASE < operation_code)
            && (ATC_HOLD_CALLS_EXCEPT_X_BASE > operation_code))
    {
        status = HandleChldRelActiveCall(atc_config_ptr,  dual_sys, operation_code);
    }
    // case 2X, split call from mulitiparty call
    else if((20 < operation_code)
            && (27 >= operation_code))
    {
        status = HandleChldSplitCallFromMpty(atc_config_ptr,  dual_sys, operation_code);
    }
    // the operation code is 7X
    else if((ATC_REL_SPECIFIC_EXISTING_X_CALL_BASE < operation_code)
        && (80 > operation_code))
    {
        status = HandleChldRelSpecificCall(atc_config_ptr,  dual_sys, operation_code);
    }
#endif
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
AT_CMD_FUNC(ATC_ProcessCLCC)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS          status = ERR_INVALID_INDEX;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    int32               i;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:

            //List the information of existing calls
            for(i = 0; i < g_calls_state[dual_sys].call_amount; i++)
            {
                /*+CLCC: <id1>,<dir>,<stat>,<mode>,<mpty>[,<number>,<type>*/
                sprintf((char *)g_rsp_str, "%s: %d,%d,%d,%d,%d,\"%s\",%d",
                        g_atc_info_table[AT_CMD_CLCC].cmd_name->str_ptr,
                        (g_calls_state[dual_sys].call_context[i].call_id + 1),
                        g_calls_state[dual_sys].call_context[i].direction,
                        ATC_GetCallState(dual_sys, (uint8)i),
                        g_calls_state[dual_sys].call_context[i].call_mode,
                        g_calls_state[dual_sys].call_context[i].mpty_state,
                        &g_calls_state[dual_sys].call_context[i].party_number[0],
                        g_calls_state[dual_sys].call_context[i].number_type);
                ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            }

            status = S_ATC_SUCCESS;
            break;

        case ATC_CMD_TYPE_TEST:
            status = S_ATC_SUCCESS;
            break;

        default:
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
    }

    return status;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handle the AT+CSSN command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSSN)
{
#ifndef _ULTRA_LOW_CODE_
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                // set the values
                atc_config_ptr->cssi_flag[dual_sys] = (uint8)PARA1.num;
            }

            if(PARAM2_FLAG)
            {
                atc_config_ptr->cssu_flag[dual_sys] = (uint8)PARA2.num;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CSSN].cmd_name->str_ptr,
                    atc_config_ptr->cssi_flag[dual_sys], atc_config_ptr->cssu_flag[dual_sys]);
            // the read command response
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
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
AT_CMD_FUNC(ATC_ProcessSAC)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS    status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:

            if(MNSS_GetCurStateEx(dual_sys))
            {
                s_cusd_isresponse[dual_sys] = FALSE;
                // abort the ss service
                MNSS_RelActivingServiceEx(dual_sys);
            }

            if(MNPHONE_IsPLMNSearchEx(dual_sys))
            {
                // abort the PLMN searching
                MNPHONE_AbortPLMNSearchEx(dual_sys);
            }

            break;

        case ATC_CMD_TYPE_READ:

            // check the current state of SS service and if the MS is searching plmn or selecting plmn to register
            if(MNSS_GetCurStateEx(dual_sys) || MNPHONE_IsPLMNSearchEx(dual_sys))
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        default:
            break;
    }

    return status;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handles the CUSD request
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleUssdReq(
    MN_DUAL_SYS_E   dual_sys,
    BOOLEAN                param1_flag,
    int32                  para1_num,
    BOOLEAN                param2_flag,
    ATC_CUR_PARAM_STRING_T *ptr_para2_str,
    BOOLEAN                param3_flag,
    int32                  para3_num
)
{
    ATC_STATUS  status     = S_ATC_SUCCESS;

    switch(para1_num)
    {
        case AT_CUSD_DISABLE:
        case AT_CUSD_ENABLE:
            s_cusd_presentation[dual_sys] = para1_num;

            if(param2_flag)
            {
                SCI_ASSERT(NULL != ptr_para2_str && NULL != ptr_para2_str->str_ptr);/*assert verified: check null pointer*/

                if(FALSE == s_cusd_isresponse[dual_sys])
                {
                    //Send the original request to MN.
                    ATC_TRACE_LOW("ATC: MNSS_UserOriginateUSSD");

                    if(ERR_MNSS_NO_ERR != MNSS_UserOriginateUSSDEx(dual_sys, FALSE, (MN_SS_DCS_E)para3_num,
                                                                   ptr_para2_str->str_len,
                                                                   ptr_para2_str->str_ptr))
                    {
                        return S_ATC_FAIL;
                    }
                    else
                    {
                        s_cusd_isresponse[dual_sys] = TRUE;
                        status = S_ATC_DEFAULT_HANDLE;
                    }
                }
                else
                {
                    //Send the request to MN.
                    ATC_TRACE_LOW("ATC: MNSS_ResponseUSSD");

                    if(ERR_MNSS_NO_ERR != MNSS_ResponseUSSDEx(dual_sys, (MN_SS_DCS_E)para3_num,
                                                              ptr_para2_str->str_len,
                                                              ptr_para2_str->str_ptr))
                    {
                        return S_ATC_FAIL;
                    }
                    else
                    {
                        status = S_ATC_DEFAULT_HANDLE;
                    }
                }
            }

            break;

        case AT_CUSD_CANCEL:
            s_cusd_isresponse[dual_sys] = FALSE;
            MNSS_RelActivingServiceEx(dual_sys);
            break;

        default:
            return ERR_INVALID_INDEX;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+CUSD command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCUSD)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_CUR_PARAM_STRING_T  *ptr_para2_str  = PNULL;
    ATC_STATUS              status          = S_ATC_SUCCESS;
    uint32                  para1_num       = AT_CUSD_DISABLE;
    uint32                  para3_num       = MN_SS_DEFAULT_DCS_VALUE;
    uint8                   *ussd_str_ptr   = NULL;
    uint16                  ussd_str_len    = 0;
    uint16                  max_str_len     = 0;
    MN_DUAL_SYS_E           dual_sys        = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr, s_cusd_presentation[dual_sys]);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
            /*+CUSD: (list of supported <n>s)*/
            sprintf((char *)g_rsp_str, "%s: (0-2)", g_atc_info_table[AT_CMD_CUSD].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;

        case ATC_CMD_TYPE_SET:

            // AT+CUSD=<n>,<str>,<dsc>
            if(PARAM1_FLAG)
            {
                para1_num = PARA1.num;
            }

            if(PARAM3_FLAG)
            {
                if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCUSD, param3 is not num!");
                    status = ERR_INVALID_INDEX;
                    break;
                }

                switch(PARA3.num)
                {
                    case AT_CUSD_SMSALPHABET:
                        para3_num = MN_SS_DEFAULT_DCS_VALUE;
                        break;

                    case AT_CUSD_UCS2ALPHABET:
                        para3_num = MN_SS_USC2_NO_CLASS_DCS;
                        break;

                    case AT_CUSD_DEFAULTALPHABET:
                        para3_num = MN_SS_DEFAULT_DCS_VALUE;
                        break;

                    default:
                        return ERR_OPERATION_NOT_ALLOWED;
                }
            }

            if(PARAM2_FLAG)
            {
                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCUSD, param2 is not string!");
                    status = ERR_INVALID_INDEX;
                    break;
                }

                if(0 == PARA2.str_ptr->str_len)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCUSD, the string is empty");
                    return ERR_INVALID_INDEX;
                }

                ATC_TRACE_LOW("ATC: ATC_ProcessCUSD target <dcs> is %d.", para3_num);

                switch (atc_config_ptr->cscs[dual_sys])
                {
                    case ATC_CHSET_IRA:
                        max_str_len = MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE;
                        break;

                    case ATC_CHSET_GSM:
                        max_str_len = MN_SS_MAX_USSD_STRING_LENGTH_AFTER_ENCODE;
                        break;

                    case ATC_CHSET_HEX:
                        max_str_len = MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE * 2;
                        break;

                    case ATC_CHSET_UCS2:
                        max_str_len = MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE * 4;
                        break;

                    default:
                        ATC_TRACE_LOW("ATC: current <dcs> is %d.", atc_config_ptr->cscs[dual_sys]);
                        return ERR_OPERATION_NOT_ALLOWED;
                }

                if(PARA2.str_ptr->str_len > max_str_len)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCUSD, the string is too long");
                    return ERR_INVALID_INDEX;
                }

                ptr_para2_str = PARA2.str_ptr;
                ussd_str_ptr = (uint8 *)SCI_ALLOC_BASEZ(ATC_USSD_MAX_STRING_LENGTH * sizeof(uint8));
                if (ussd_str_ptr == PNULL)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCUSD no memory available");
                    return ERR_MEMORY_FULL;
                }

                if(!ATC_ChsetTransformation((ATC_CHARACTER_SET_TYPE_E)atc_config_ptr->cscs[dual_sys],
                                            ptr_para2_str->str_ptr,
                                            (uint16)ptr_para2_str->str_len,
                                            ATC_CHSET_IRA,
                                            ATC_USSD_MAX_STRING_LENGTH,
                                            ussd_str_ptr,
                                            &ussd_str_len))
                {
                    SCI_FREE(ussd_str_ptr);
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                ptr_para2_str->str_ptr = ussd_str_ptr;
                ptr_para2_str->str_len = ussd_str_len;
            }

            status = HandleUssdReq(dual_sys, PARAM1_FLAG, para1_num, PARAM2_FLAG, ptr_para2_str, PARAM3_FLAG, para3_num);
            if(status == S_ATC_DEFAULT_HANDLE)
            {
                status = S_ATC_SUCCESS;     //return OK to APP immediately, invoid waiting for response msg a long time
            }
            SCI_FREE(ussd_str_ptr);
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return status;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_USSD_IND
//                       signal to response the function
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessUssdInd(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_USSD_IND_T *sig_ptr
)
{
#ifndef _ULTRA_LOW_CODE_

    ATC_TRACE_LOW("ATC: ATC_ProcessUssdInd, s_cusd_presentation:%d,s_cusd_isresponse:%d",
                  s_cusd_presentation[sig_ptr->dual_sys], s_cusd_isresponse[sig_ptr->dual_sys]);

    if(AT_CUSD_ENABLE == s_cusd_presentation[sig_ptr->dual_sys])
    {
        ATC_UpCUSDInfo(sig_ptr->dual_sys, atc_config_ptr, &sig_ptr->ussd_data, sig_ptr->ussd_action);
    }

    if(s_cusd_isresponse[sig_ptr->dual_sys] == FALSE)
    {
        s_cusd_isresponse[sig_ptr->dual_sys] = TRUE;
    }

#endif /* _ULTRA_LOW_CODE_ */
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_USSD_NOTIFY_IND  signal
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessUssdNotifyInd(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_USSD_NOTIFY_IND_T *sig_ptr
)
{

    ATC_TRACE_LOW("ATC: ATC_ProcessUssdNotifyInd, s_cusd_isresponse:%d,s_cusd_presentation:%d",
                  s_cusd_isresponse[sig_ptr->dual_sys], s_cusd_presentation[sig_ptr->dual_sys]);

    if(AT_CUSD_ENABLE == s_cusd_presentation[sig_ptr->dual_sys])
    {
        ATC_UpCUSDInfo(sig_ptr->dual_sys, atc_config_ptr, &sig_ptr->ussd_data, sig_ptr->ussd_action);
    }

#ifndef _SUPPORT_IPC_
    MNSS_ResponseUSSDNotifyEx(sig_ptr->dual_sys);
#endif/*_SUPPORT_IPC_*/

    return S_ATC_SUCCESS;
}
/*****************************************************************************/
//  Description : This function handles the APP_MN_USSD_SERVICE_CNF signal
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessUssdServiceCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_USSD_SERVICE_CNF_T *sig_ptr
)
{
    ATC_STATUS              status = S_ATC_SUCCESS;

    ATC_TRACE_LOW("ATC: ATC_ProcessUssdServiceCnf, SIM %d, s_cusd_isresponse:%d, s_cusd_presentation:%d",
                  sig_ptr->dual_sys, s_cusd_isresponse[sig_ptr->dual_sys], s_cusd_presentation[sig_ptr->dual_sys]);

    if (MN_SS_OPERATION_SUCC != sig_ptr->result_type)
    {
        ATC_UpSPERRORInfo(sig_ptr->dual_sys, atc_config_ptr, SS_SERVICE_REJECT_CLASS, (uint32)sig_ptr->err_code);
    }

    switch(sig_ptr->result_type)
    {
        case MN_SS_OPERATION_SUCC:  // operation success
            ATC_TRACE_LOW("ATC: MN_SS_OPERATION_SUCC");
            status = S_ATC_SUCCESS;
            break;

        case MN_SS_OPERATION_ERROR:  // operation error
            ATC_TRACE_LOW("ATC: MN_SS_OPERATION_ERROR,error code:%d", sig_ptr->err_code);
            status = ERR_INVALID_INDEX;
            break;

        case MN_SS_OPERATION_REJECT:  // operation reject by network
            ATC_TRACE_LOW("ATC: MN_SS_OPERATION_REJECT,error code:%d", sig_ptr->err_code);
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;

        default:
            status = ERR_UNKNOWN;
            break;
    }


    if (S_ATC_SUCCESS == status)
    {
        if(AT_CUSD_ENABLE == s_cusd_presentation[sig_ptr->dual_sys])
        {
            ATC_UpCUSDInfo(sig_ptr->dual_sys, atc_config_ptr, &sig_ptr->ussd_data, sig_ptr->ussd_action);
        }
    }
    else
    {
        ATC_UpCUSDInfo(sig_ptr->dual_sys, atc_config_ptr, &sig_ptr->ussd_data, sig_ptr->ussd_action); //return error information to APP
    }

    s_cusd_isresponse[sig_ptr->dual_sys] = FALSE;
    return status;
}

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
    switch(ccfc_reason)
    {
            // call forwarded unconditional
        case ATC_UNCONDITIONAL:
            *ss_code_ptr = MN_SS_CFU;
            break;
            // call forwared when the mobile phone is busy
        case ATC_MOBILE_BUSY:
            *ss_code_ptr = MN_SS_CFB;
            break;
            // call forwarded when on reply
        case ATC_NO_REPLY:
            *ss_code_ptr = MN_SS_CFNRY;
            break;
            // call forwarded when the mobile subscriber not reachable
        case ATC_NOT_REACHABLE:
            *ss_code_ptr = MN_SS_CFNRC;
            break;
            // all call forwarded
        case ATC_ALL_CALL_FORWARDED:
            *ss_code_ptr = MN_SS_ALL_FORWARD;
            break;
            // all conditional forwarded
        case ATC_ALL_CONDITIONAL_FORWARDED:
            *ss_code_ptr = MN_SS_ALL_COND_FORWARD;
            break;
        default:
            SCI_ASSERT(FALSE);/*assert verified: check valid value*/
            break;
    }
}

/*****************************************************************************/
//  Description : This function convert the ATC call bar reason code
//                to the MNSS's code
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:
/*****************************************************************************/
LOCAL void ConvertCallBarCode(
    uint8                 fac_type,   // the call forwarded reason
    MN_SS_CODE_E          *ss_code_ptr  // the SS code
)
{
    switch(fac_type)
    {
        case AT_PWD_AO:        //BAOC (Barr All Outgoing Calls)
            *ss_code_ptr = MN_SS_BAOC;
            break;
        case AT_PWD_OI:        // BOIC (Barr Outgoing International Calls)
            *ss_code_ptr = MN_SS_BOIC;
            break;
        case AT_PWD_OX:        // BOIC-exHC (Barr Outgoing. International Calls except to Home Country)
            *ss_code_ptr = MN_SS_BOIC_EXHC;
            break;
        case AT_PWD_AI:        //BAIC (Barr All Incoming Calls)
            *ss_code_ptr = MN_SS_BAIC;
            break;
        case AT_PWD_IR:        //BIC-Roam (Barr Inc.  When Roaming outside Home Country)
            *ss_code_ptr = MN_SS_BICROAM;
            break;
        case AT_PWD_AB:        // All Barring services
            *ss_code_ptr = MN_SS_ALL_BAR_SERVICE;
            break;
        case AT_PWD_AG:        //All outGoing barring services
            *ss_code_ptr = MN_SS_BAR_OUTGOING;
            break;
        
        case AT_PWD_AC:        //All inComing barring services
            *ss_code_ptr = MN_SS_BAR_INCOMING;
            break;
        default:
            SCI_ASSERT(FALSE);/*assert verified: check valid value*/
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
ATC_STATUS ATC_ProcessForwardInd(
    ATC_CONFIG_T              *atc_config_ptr,
    APP_MN_CALL_FORWARD_IND_T *sig_ptr
)
{
    MN_DUAL_SYS_E    dual_sys = ((APP_MN_CALL_FORWARD_IND_T *)sig_ptr)->dual_sys;
 
    switch(sig_ptr->forward_type)
    {
        case MN_SS_INCOMING_IS_FORWARED:

            if(atc_config_ptr->cssu_flag[dual_sys])
            {
                sprintf((char *)g_rsp_str, "+CSSU:0");
                ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }

            break;

        case MN_SS_OUTGOING_FORWARED_TO_C:

            if(atc_config_ptr->cssi_flag[dual_sys])
            {
                // the result code is 2:A call has been forwarded
                sprintf((char *)g_rsp_str, "+CSSI:2");
                ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }

            break;

        case MN_SS_INCOMING_FORWARED_TO_C:

            if(atc_config_ptr->cssu_flag[dual_sys])
            {
                sprintf((char *)g_rsp_str, "+CSSU:10");
                ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }

            break;

        default:
            SCI_ASSERT(FALSE);/*assert verified: check valid value*/
            break;
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_WAITING_IND signal and
//                sends the +CSSI: unsolicited result code
//  Global resource dependence : none
//  Author:       Yi Jin
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessWaitingInd(
    ATC_CONFIG_T              *atc_config_ptr,
    APP_MN_CALL_WAIT_IND_T *sig_ptr
)
{
    if (atc_config_ptr->cssi_flag[sig_ptr->dual_sys])
    {
        // the result code is 3:A call has been waiting
        sprintf((char *)g_rsp_str, "+CSSI:3");
        ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_CLIR_SUPP_REJ_IND signal and
//                sends the +CSSI: unsolicited result code
//  Global resource dependence : none
//  Author:       Yi Jin
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessClirSuppRejInd(
    ATC_CONFIG_T              *atc_config_ptr,
    APP_MN_CLIR_SUPP_REJ_IND_T *sig_ptr
)
{
    if(atc_config_ptr->cssi_flag[sig_ptr->dual_sys])
    {
        // the result code is 7:A call clir suppression has been rejected
        sprintf((char *)g_rsp_str, "+CSSI:7");
        ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_DEFLECT_IND signal and
//                sends the +CSSI: unsolicited result code
//  Global resource dependence : none
//  Author:       Yi Jin
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessCallDeflectInd(
    ATC_CONFIG_T              *atc_config_ptr,
    APP_MN_CALL_DEFLECT_IND_T *sig_ptr
)
{
    uint8  call_index = GetIndexOfCallContext(sig_ptr->call_id, sig_ptr->dual_sys);

    if(ATC_INVALID_CALLID != call_index)
    {
        if(g_calls_state[sig_ptr->dual_sys].call_context[call_index].call_entity_state != ATC_CALL_OUTGOING_STATE &&
                ATC_CALL_ALERTING_STATE != g_calls_state[sig_ptr->dual_sys].call_context[call_index].call_entity_state)
        {
            if(atc_config_ptr->cssu_flag[sig_ptr->dual_sys])
            {
                sprintf((char *)g_rsp_str, "+CSSU:9");
                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }
        }
        else if(atc_config_ptr->cssi_flag[sig_ptr->dual_sys])
        {
            // the result code is 8:A call has been deflected
            sprintf((char *)g_rsp_str, "+CSSI:8");
            ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
            return S_ATC_SUCCESS;
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
ATC_STATUS ATC_ProcessBarredInd(
    ATC_CONFIG_T              *atc_config_ptr,
    APP_MN_CALL_BARRED_IND_T  *sig_ptr
)
{
    // the enable the +CSSI unsolicited result code
    if(atc_config_ptr->cssi_flag[sig_ptr->dual_sys])
    {
        switch(sig_ptr->barred_operation)
        {
            case MN_SS_BAOC:
            case MN_SS_BOIC:
            case MN_SS_BOIC_EXHC:
            case MN_SS_BAR_OUTGOING:
                // the unsolicited code is 5:Outgoing calls are barred
                sprintf((char *)g_rsp_str, "+CSSI:5");
                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
                break;
            case MN_SS_BAIC:
            case MN_SS_BICROAM:
            case MN_SS_BAR_INCOMING:
                // the unsolicited code is 6:Incoming calls are barred
                sprintf((char *)g_rsp_str, "+CSSI:6");
                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
                break;
            case MN_SS_ALL_BAR_SERVICE:
                // the unsolicited code is 5:Outgoing calls are barred
                sprintf((char *)g_rsp_str, "+CSSI:5");
                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
                // the unsolicited code is 6:Incoming calls are barred
                sprintf((char *)g_rsp_str, "+CSSI:6");
                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
                break;
            default:
                ATC_TRACE_LOW("ATC: ATC_ProcessBarredInd, unknown barred_operation:%d", sig_ptr->barred_operation);
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
ATC_STATUS ATC_ProcessForwardStatusInd(
    ATC_CONFIG_T                   *atc_config_ptr,
    APP_MN_FORWARD_STATUS_IND_T    *sig_ptr
)
{
    // enable the unsolicited result code: +CSSI
    if(atc_config_ptr->cssi_flag[sig_ptr->dual_sys])
    {
        switch(sig_ptr->forward_operation)
        {
            case MN_SS_ALL_FORWARD:
            case MN_SS_CFU:

                // Unconditional call forwarding is active
                if(MN_SS_PROVITION == sig_ptr->forward_status ||
                        MN_SS_ACTIVE_OPERATIVE == sig_ptr->forward_status)
                {
                    sprintf((char *)g_rsp_str, "+CSSI:0");
                    ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
                    return S_ATC_SUCCESS;
                }
                else
                {
                    return S_ATC_DEFAULT_HANDLE;
                }

            case MN_SS_ALL_COND_FORWARD:
            case MN_SS_CFB:
            case MN_SS_CFNRY:
            case MN_SS_CFNRC:

                // Some of the conditional call forwardings are active
                if(MN_SS_PROVITION == sig_ptr->forward_status ||
                        MN_SS_ACTIVE_OPERATIVE == sig_ptr->forward_status)
                {
                    sprintf((char *)g_rsp_str, "+CSSI:1");
                    ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
                    return S_ATC_SUCCESS;
                }
                else
                {
                    return S_ATC_DEFAULT_HANDLE;
                }

            default:
                SCI_ASSERT(FALSE);/*assert verified: check valid value*/
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
ATC_STATUS ATC_ProcessRegisterCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_REGISTER_SS_CNF_T *sig_ptr
)
{
    if (MN_SS_OPERATION_SUCC != sig_ptr->result_type)
    {
        ATC_UpSPERRORInfo(sig_ptr->dual_sys, atc_config_ptr, SS_SERVICE_REJECT_CLASS, (uint32)sig_ptr->err_code);
    }

    ATC_ProcessSSCnf(atc_config_ptr, (APP_MN_INTERROGATE_CNF_T *)sig_ptr, TRUE);
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_INTERROGATE_CNF signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
ATC_STATUS ATC_ProcessInterCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_INTERROGATE_CNF_T *sig_ptr
)
{
    BOOLEAN   is_service_active;
    uint16              service_class = 0;

    #ifndef ATC_SAT_ENABLE
    uint16              service_status = 0;
    #endif

    if (MN_SS_OPERATION_SUCC != sig_ptr->result_type)
    {
        ATC_UpSPERRORInfo(sig_ptr->dual_sys, atc_config_ptr, SS_SERVICE_REJECT_CLASS, (uint32)sig_ptr->err_code);
    }
    
    if(ATC_CheckInterrogateCnf(atc_config_ptr, sig_ptr, &is_service_active))
    {
        switch(s_ss_context[sig_ptr->dual_sys].cmd_id)
        {
        #ifndef ATC_SAT_ENABLE
            case AT_CMD_CLIP:
                sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CLIP].cmd_name->str_ptr,
                        atc_config_ptr->atc_config_nv.clip_flag, is_service_active);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
                break;

            case AT_CMD_CLIR:

                if(is_service_active)
                {
                    switch(sig_ptr->info_list[0].clir_info)
                    {
                        case MN_SS_CLIR_PERMANENT:
                            service_status = 1;
                            break;

                        case MN_SS_CLIR_TMP_DEFAULT_ALLOWED:
                            service_status = 4;
                            break;

                        case MN_SS_CLIR_TMP_DEFAULT_RESTRICTED:
                            service_status = 3;
                            break;

                        default:
                            service_status = 2;
                            break;
                    }
                }

                sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CLIR].cmd_name->str_ptr,
                        atc_config_ptr->atc_config_nv.clir_flag, service_status);

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
                break;

            case AT_CMD_COLP:
                sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_COLP].cmd_name->str_ptr,
                        atc_config_ptr->atc_config_nv.colp_flag, is_service_active);

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
                break;

            case AT_CMD_COLR:
                sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_COLR].cmd_name->str_ptr,
                        0, is_service_active);

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
                break;
        #endif
        
            case AT_CMD_CLCK:
                service_class = ATC_getclass_from_interrogate_cnf(sig_ptr->info_list, sig_ptr->list_num);
                is_service_active = (s_ss_context[sig_ptr->dual_sys].service_class & service_class) ? 1 : 0;

                if(is_service_active)
                {
                    int i;

                    for(i = 0; i < sig_ptr->list_num; i++)
                    {
                        service_class = ATC_getclass_from_interrogate_cnf(sig_ptr->info_list + i, 1);
                        service_class = service_class & s_ss_context[sig_ptr->dual_sys].service_class;

                        if(!service_class)
                        {
                            continue;
                        }

                        sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CLCK].cmd_name->str_ptr,
                                is_service_active, service_class);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                }
                else
                {
                    sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CLCK].cmd_name->str_ptr,
                            is_service_active, s_ss_context[sig_ptr->dual_sys].input_class);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }

                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
                break;

            case AT_CMD_CCWA:
                service_class = ATC_getclass_from_interrogate_cnf(sig_ptr->info_list, sig_ptr->list_num);
                is_service_active = (s_ss_context[sig_ptr->dual_sys].service_class & service_class) ? 1 : 0;

                if(is_service_active)
                {
                    int i;

                    for(i = 0; i < sig_ptr->list_num; i++)
                    {
                        service_class = ATC_getclass_from_interrogate_cnf(sig_ptr->info_list + i, 1);
                        service_class &= s_ss_context[sig_ptr->dual_sys].service_class;

                        if(!service_class)
                        {
                            continue;
                        }

                        sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CCWA].cmd_name->str_ptr,
                                is_service_active, service_class);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                }
                else
                {
                    sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CCWA].cmd_name->str_ptr,
                            is_service_active, s_ss_context[sig_ptr->dual_sys].input_class);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }

                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                break;

#ifdef _SUPPORT_CCBS_
            case AT_CMD_SCCBS:
            {
                service_class = ATC_getclass_from_interrogate_cnf(sig_ptr->info_list, sig_ptr->list_num);
                is_service_active = service_class ? TRUE : FALSE;
                if(is_service_active)
                {
                    uint8              tel_number[MN_MAX_ADDR_BCD_LEN*2] = {0};
                    ATC_TELE_TYPE_E    tel_type;
                    int i = 0;
                    
                    for(i = 0; i < sig_ptr->list_num; i++)
                    {
                        SCI_MEMSET(&tel_number[0], 0 , (MN_MAX_ADDR_BCD_LEN * 2));
                        service_class = ATC_getclass_from_interrogate_cnf(sig_ptr->info_list + i, 1);
                        
                        if(!service_class)
                        {
                            continue;
                        }

                        if((0 != sig_ptr->info_list[i].ccbs_index) && (sig_ptr->info_list[i].forward_num_present))
                        {
                            ATC_ConvertCalledNumToStr(&(sig_ptr->info_list[i].forward_num), &tel_type, tel_number, NULL);
                            sprintf((char *)g_rsp_str, "%s: %d,%d,%d,\"%s\",%d", g_atc_info_table[AT_CMD_SCCBS].cmd_name->str_ptr,
                                        AT_STATUS_ACTIVE, sig_ptr->info_list[i].ccbs_index, service_class, tel_number, tel_type);

                        }
                        else if(0 != sig_ptr->info_list[i].ccbs_index)
                        {
                            tel_number[0] = '\0';
                            sprintf((char *)g_rsp_str, "%s: %d,%d,%d,\"%s\",128", g_atc_info_table[AT_CMD_SCCBS].cmd_name->str_ptr,
                                    AT_STATUS_ACTIVE, sig_ptr->info_list[i].ccbs_index, service_class, tel_number);
                        }
                        else if(0 == i)
                        {
                            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SCCBS].cmd_name->str_ptr,
                            AT_STATUS_ACTIVE);
                        }

                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                }
                else
                {
                    sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SCCBS].cmd_name->str_ptr,
                    AT_STATUS_NOT_ACTIVE);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }

                s_ss_context[sig_ptr->dual_sys].cmd_id = (int16)AT_INVALID_CMD_ID;
                s_ss_context[sig_ptr->dual_sys].ss_code = (MN_SS_CODE_E)0;        /*CR165167*/
                break;
            }
#endif //end of _SUPPORT_CCBS_


            case AT_CMD_CCFC:
                service_class = ATC_getclass_from_interrogate_cnf(sig_ptr->info_list, sig_ptr->list_num);
                is_service_active = (s_ss_context[sig_ptr->dual_sys].service_class & service_class) ? 1 : 0;

                if(is_service_active || (sig_ptr->list_num > 0))
                {
                    uint8              tel_number[MN_MAX_ADDR_BCD_LEN*2] = {0};
                    ATC_TELE_TYPE_E    tel_type;
                    int i;

                    for(i = 0; i < sig_ptr->list_num; i++)
                    {
                        BOOLEAN service_group_status = FALSE;  //for every service group
                        SCI_MEMSET(&tel_number[0], 0 , (MN_MAX_ADDR_BCD_LEN * 2));
                        service_class = ATC_getclass_from_info_list(sig_ptr->info_list + i);
                        service_class = service_class & s_ss_context[sig_ptr->dual_sys].service_class;

                        if(sig_ptr->info_list[i].ss_status_present)
                        {
                            if((sig_ptr->info_list[i].ss_status == MN_SS_PROVITION) ||
                                (sig_ptr->info_list[i].ss_status == MN_SS_ACTIVE_OPERATIVE) ||
                                (sig_ptr->info_list[i].ss_status == MN_SS_ACITVE_QUIESCENT))
                            {
                                service_group_status = TRUE;
                            }
                        }
                        
                        if(!service_class)
                        {
                            continue;
                        }

                        if(sig_ptr->info_list[i].forward_num_present)
                        {
                            ATC_ConvertCalledNumToStr(&(sig_ptr->info_list[i].forward_num), &tel_type, tel_number, NULL);

                            if(sig_ptr->info_list[i].no_reply_time_present)
                            {
                                sprintf((char *)g_rsp_str, "%s: %d,%d,\"%s\",%d,,,%d", g_atc_info_table[AT_CMD_CCFC].cmd_name->str_ptr,
                                        service_group_status? AT_STATUS_ACTIVE : AT_STATUS_NOT_ACTIVE, service_class, tel_number, tel_type, sig_ptr->info_list[i].no_reply_time);
                            }
                            else
                            {
                                sprintf((char *)g_rsp_str, "%s: %d,%d,\"%s\",%d", g_atc_info_table[AT_CMD_CCFC].cmd_name->str_ptr,
                                        service_group_status? AT_STATUS_ACTIVE : AT_STATUS_NOT_ACTIVE, service_class, tel_number, tel_type);
                            }
                        }
                        else
                        {
                            tel_number[0] = '\0';
                            sprintf((char *)g_rsp_str, "%s: %d,%d,\"%s\",145", g_atc_info_table[AT_CMD_CCFC].cmd_name->str_ptr,
                                    service_group_status? AT_STATUS_ACTIVE : AT_STATUS_NOT_ACTIVE, service_class, tel_number);
                        }

                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                }
                else
                {
                    sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CCFC].cmd_name->str_ptr,
                            AT_STATUS_NOT_ACTIVE, s_ss_context[sig_ptr->dual_sys].input_class);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }

                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);

                /* Synchronize Service */
                switch(s_ss_context[sig_ptr->dual_sys].ss_code)
                {
                    case MN_SS_ALL_FORWARD:
                        atc_config_ptr->atc_sync_srv.cfall = is_service_active;
                        break;

                    case MN_SS_CFU:
                        atc_config_ptr->atc_sync_srv.cfu = is_service_active;
                        break;

                    case MN_SS_ALL_COND_FORWARD:
                        atc_config_ptr->atc_sync_srv.cfcond = is_service_active;
                        break;

                    case MN_SS_CFB:
                        atc_config_ptr->atc_sync_srv.cfb = is_service_active;
                        break;

                    case MN_SS_CFNRY:
                        atc_config_ptr->atc_sync_srv.cfnry = is_service_active;
                        break;

                    case MN_SS_CFNRC:
                        atc_config_ptr->atc_sync_srv.cfnrc = is_service_active;
                        break;

                    default:
                        break;
                }

                ATC_TRACE_LOW("ATC: ATC_ProcessInterCnf,cfall=%d,cfu=%d,cfcond=%d,cfb=%d,cfnry=%d,cfnrc=%d",
                              atc_config_ptr->atc_sync_srv.cfall,
                              atc_config_ptr->atc_sync_srv.cfu,
                              atc_config_ptr->atc_sync_srv.cfcond,
                              atc_config_ptr->atc_sync_srv.cfb,
                              atc_config_ptr->atc_sync_srv.cfnry,
                              atc_config_ptr->atc_sync_srv.cfnrc);

                /* clear s_ss_context before synchronizing service */
                s_ss_context[sig_ptr->dual_sys].cmd_id = (int16)AT_INVALID_CMD_ID;
                s_ss_context[sig_ptr->dual_sys].ss_code = (MN_SS_CODE_E)0;        /*CR165167*/

                /* continue synchronizing service */
                if(TRUE == ATC_SynchronizeService(sig_ptr->dual_sys))
                {
                    /* if in synchronizing service, NO ATC_BuildInfoRsp &
                    * ATC_BuildResultCodeRsp, return directly.
                    * NOTE: s_ss_context may be set, anyway return! */
                    return S_ATC_DEFAULT_HANDLE;
                }

                break;

            default:
                ATC_TRACE_LOW("ATC: ATC_ProcessInterCnf, unknown s_ss_context[%d].cmd_id: %d", sig_ptr->dual_sys, s_ss_context[sig_ptr->dual_sys].cmd_id);
                return S_ATC_DEFAULT_HANDLE;
        }

    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
    }

    s_ss_context[sig_ptr->dual_sys].cmd_id = (int16)AT_INVALID_CMD_ID;
    s_ss_context[sig_ptr->dual_sys].ss_code = (MN_SS_CODE_E)0;

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_DEACTIVATE_SS_CNF signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:        modify by longting.zhao 2011.8.26 notify app error code
/*****************************************************************************/
ATC_STATUS ATC_ProcessDeactCnf(
    ATC_CONFIG_T               *atc_config_ptr,
    APP_MN_DEACTIVATE_SS_CNF_T *sig_ptr
)
{
    if (MN_SS_OPERATION_SUCC != sig_ptr->result_type)
    {
        ATC_UpSPERRORInfo(sig_ptr->dual_sys, atc_config_ptr, SS_SERVICE_REJECT_CLASS, (uint32)sig_ptr->err_code);
    }

    ATC_ProcessSSCnf(atc_config_ptr, (APP_MN_INTERROGATE_CNF_T *)sig_ptr, FALSE);
    return S_ATC_DEFAULT_HANDLE;
}


/*****************************************************************************/
//  Description : This function handles the APP_MN_ACTIVATE_SS_CNF signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:        modify by longting.zhao 2011.8.26 notify app error code
/*****************************************************************************/
ATC_STATUS ATC_ProcessActCnf(
    ATC_CONFIG_T                *atc_config_ptr,
    APP_MN_ACTIVATE_SS_CNF_T    *sig_ptr
)
{
    if (MN_SS_OPERATION_SUCC != sig_ptr->result_type)
    {
        ATC_UpSPERRORInfo(sig_ptr->dual_sys, atc_config_ptr, SS_SERVICE_REJECT_CLASS, (uint32)sig_ptr->err_code);
    }

    ATC_ProcessSSCnf(atc_config_ptr, (APP_MN_INTERROGATE_CNF_T *)sig_ptr, TRUE);
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_ERASE_SS_CNF_T signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessEraseCnf(
    ATC_CONFIG_T              *atc_config_ptr,
    APP_MN_ERASE_SS_CNF_T     *sig_ptr
)
{
    if (MN_SS_OPERATION_SUCC != sig_ptr->result_type)
    {
        ATC_UpSPERRORInfo(sig_ptr->dual_sys, atc_config_ptr, SS_SERVICE_REJECT_CLASS, (uint32)sig_ptr->err_code);
    }

    ATC_ProcessSSCnf(atc_config_ptr, (APP_MN_INTERROGATE_CNF_T *)sig_ptr, FALSE);
    return S_ATC_DEFAULT_HANDLE;
}
/*****************************************************************************/
//  Description : This function handle the AT+CLCK command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCLCK)
{
#ifndef _ULTRA_LOW_CODE_
    MN_BLOCK_T        pin_block = {0};
    uint8             cmd_type;
    uint8             *temp_ptr;
    uint8             param_len;
    uint8             fac_type;
    ATC_CPWD_PARAM_T  param[4];
    uint8             operate_type = ATC_CLCK_DISABLE;
    uint8             class_type; //only support voice now.
    ERR_MNPHONE_CODE_E  err_code;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_SS_CODE_E    ss_code;

    SCI_MEMSET(param, 0, sizeof(param));
    SCI_MEMSET(s_clck_psw, 0, sizeof(s_clck_psw));

    //check the index.
    if(!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    param_len = PARA1.str_ptr->str_len;
    temp_ptr  = PARA1.str_ptr->str_ptr;
    ATC_GetCmdType(temp_ptr, (uint16)param_len, &cmd_type);

    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
            temp_ptr++;
            param_len--;

            if(!ATC_GetParams(temp_ptr, param_len, param, 4))
            {
                ATC_TRACE_LOW("ATC: ATC_GetParams=FALSE");
                return ERR_OPERATION_NOT_ALLOWED;
            }

            /*<fac>, <mode> param must be used*/
            if((AT_CPWD_FAC_LEN != param[0].param_len) || (1 != param[1].param_len)
                    /*||((0 != param[2].param_len) && (ATC_CLCK_QUERY_STATUS == operate_type))*/)
            {
                return ERR_INVALID_INDEX;
            }

            // get the operate code
            operate_type = param[1].param[0];
            operate_type -= '0';

            /*<mode>:0~2*/
            if(operate_type > ATC_CLCK_QUERY_STATUS)
            {
                return ERR_INVALID_INDEX;
            }

            // check the password

            if(ATC_CLCK_QUERY_STATUS != operate_type)
            {
                if(ATC_CheckPinStr(param[2].param, param[2].param_len))
                {
                    if(param[2].param_len < AT_PSW_MIN_LEN || param[2].param_len > AT_PSW_MAX_LEN)
                    {
                        ATC_TRACE_LOW("ATC: WARNING, the length of passward is wrong!");
                        return ERR_OPERATION_NOT_ALLOWED;
                    }

                    SCI_MEMCPY(pin_block.blockbuf, param[2].param, param[2].param_len);
                    pin_block.blocklen = param[2].param_len;
                    SCI_MEMCPY(s_clck_psw, param[2].param, AT_CLCK_PSW_LEN);
                }
                else
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }
            }

            // get the class
            if(0 != param[3].param_len)
            {
                uint32 temp;
                param[3].param[param[3].param_len] = 0;
                temp = atoi((const char *)param[3].param);
                class_type = temp & 0xFF;
            }
            else
            {
                class_type = 7;
            }

            if(class_type < 1)
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            s_ss_context[dual_sys].input_class = class_type;

            // check the fac type
            if(!ATC_GetClckFacType(param[0].param, param[0].param_len, &fac_type))
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            switch(fac_type)
            {
                    //handle the pin
                case AT_CLCK_SC:

                    if(!atc_config_ptr->sim_flag[dual_sys])
                    {
                        return ERR_SIM_NOT_INSERTED;
                    }

                    switch(operate_type)
                    {
                        case ATC_CLCK_DISABLE:
                            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_DISABLE, MNSIM_PIN1, &pin_block, PNULL);

                            if(err_code == ERR_MNPHONE_NO_ERR)
                            {
                                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                                  APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }

                            break;
                        case ATC_CKCK_ENABLE:
                            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_ENABLE, MNSIM_PIN1, &pin_block, PNULL);

                            if(err_code == ERR_MNPHONE_NO_ERR)
                            {
                                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                                  APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }

                            break;
                        case ATC_CLCK_QUERY_STATUS:
                            sprintf((char *)g_rsp_str, "+CLCK: %d", atc_config_ptr->pin1_enable_flag[dual_sys]);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                            return S_ATC_DEFAULT_HANDLE;
                        default:
                            return ERR_OPERATION_NOT_ALLOWED;
                    }

                    break;

                //The following is about SS.
                case AT_PWD_AO:        //BAOC (Barr All Outgoing Calls)
                case AT_PWD_OI:        // BOIC (Barr Outgoing International Calls)
                case AT_PWD_OX:        // BOIC-exHC (Barr Outgoing. International Calls except to Home Country)
                case AT_PWD_AI:        //BAIC (Barr All Incoming Calls)
                case AT_PWD_IR:        //BIC-Roam (Barr Inc.  When Roaming outside Home Country)
                case AT_PWD_AB:        // All Barring services
                case AT_PWD_AG:        //All outGoing barring services
                case AT_PWD_AC:        //All inComing barring services
                    {
                        uint8 status = S_ATC_SUCCESS;
                        if(!atc_config_ptr->sim_flag[dual_sys])
                        {
                            return ERR_SIM_NOT_INSERTED;
                        }

                        /* check password is valid */
                        if(operate_type != ATC_CLCK_QUERY_STATUS)
                        {
                            if(pin_block.blocklen > AT_CLCK_PSW_LEN)
                            {
                                ATC_TRACE_LOW("ATC: clck password length is larger than 4");
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }

                        ConvertCallBarCode(fac_type, &ss_code);
#ifdef _MUX_ENABLE_
                        status = HandleClckSSReq(dual_sys, atc_config_ptr, ss_code, operate_type, class_type);
#else
                        status = HandleClckSSReq(dual_sys, ss_code, operate_type, class_type);
#endif
                        if (S_ATC_SUCCESS != status)
                        {
                            return status;
                        }
                    }
                    break;
        #ifndef ATC_SAT_ENABLE
                case AT_PWD_FD:

                    //handle FDN
                    if(!atc_config_ptr->sim_flag[dual_sys])
                    {
                        return ERR_INVALID_INDEX;
                    }

                    switch(operate_type)
                    {
                        case ATC_CLCK_DISABLE:
                            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_VERIFY, MNSIM_PIN2, &pin_block, PNULL);

                            if(err_code == ERR_MNPHONE_NO_ERR)
                            {
                                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                                  APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }

                            atc_config_ptr->operate[dual_sys] = ATC_PIN_DISABLE_FDN;
                            break;
                        case ATC_CKCK_ENABLE:
                            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_VERIFY, MNSIM_PIN2, &pin_block, PNULL);

                            if(err_code == ERR_MNPHONE_NO_ERR)
                            {
                                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                                  APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }

                            atc_config_ptr->operate[dual_sys] = ATC_PIN_ENABLE_FDN;
                            break;
                        case ATC_CLCK_QUERY_STATUS:
                            if (atc_config_ptr->is_fdn_ready[dual_sys])
                            {
                                ATC_TRACE_LOW("ATC: ATC_ProcessCLCK sim id=%d, query mode, is_fdn_enable=%d",dual_sys,atc_config_ptr->is_fdn_enable[dual_sys]);
                                sprintf((char *)g_rsp_str, "+CLCK: %d", atc_config_ptr->is_fdn_enable[dual_sys]);
                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                                return S_ATC_DEFAULT_HANDLE;
                            }
                            else
                            {
                                //Send Signal to L4-->SIM to get the FDN status
                                ATC_TRACE_LOW("ATC: ATC_ProcessCLCK sim id=%d, query mode,  FDN",dual_sys);
                                MNSIM_GetFDNStatus_Req(dual_sys,(uint32)P_ATC);
                                atc_config_ptr->is_fdn_init[dual_sys] = TRUE;
                                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_FDN_SERVICE_IND, 0xff, ATC_NO_DOMAIN, dual_sys);
                            }
                            break;

                        default:
                            return ERR_OPERATION_NOT_ALLOWED;
                    }

                    break;
            #endif /*ATC_SAT_ENABLE*/
                default:
                    return ERR_OPERATION_NOT_ALLOWED;
            }

            s_ss_context[dual_sys].cmd_id = AT_CMD_CLCK;
            s_ss_context[dual_sys].ss_code = (MN_SS_CODE_E)0;
            break;

        case ATC_CMD_TYPE_READ:
            atc_config_ptr->pin1_enable_flag[dual_sys] = MNSIM_IsPin1EnableEx(dual_sys);
            ATC_TRACE_LOW("ATC: CPIN pin1 enable flag: %d", atc_config_ptr->pin1_enable_flag);
            ATC_FormatClckReadRsp(g_rsp_str, atc_config_ptr->pin1_enable_flag[dual_sys]);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            return S_ATC_SUCCESS;

        case ATC_CMD_TYPE_TEST:
            ATC_FormatClckTestRsp(g_rsp_str);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            return S_ATC_SUCCESS;


        default:
            return ERR_OPERATION_NOT_ALLOWED;
    }

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
/*****************************************************************************/
//  Description : This function handle the SS service in AT+CLCK.
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
#ifdef _MUX_ENABLE_
LOCAL ATC_STATUS HandleClckSSReq(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T   *atc_config_ptr,
    MN_SS_CODE_E    ss_code,
    uint8           operate_type,
    uint8           class_type
)
#else
LOCAL ATC_STATUS HandleClckSSReq(
    MN_DUAL_SYS_E   dual_sys,
    MN_SS_CODE_E    ss_code,
    uint8           operate_type,
    uint8           class_type
)
#endif
{
    MN_SS_BASIC_SERVICE_T      basic_service;
    ERR_MNSS_CODE_E err_code;


    if(!ATC_class_to_basicservice(&class_type, &basic_service, (BOOLEAN)(operate_type == ATC_CLCK_QUERY_STATUS)))
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    // get the MNSS service code
    //ConvertServiceCode(service_type, &(basic_service.service_code)); /* +cr143414 -cr143414*/
    ATC_TRACE_LOW("ATC: basic_service.service_code=%d", basic_service.service_code);


    switch(operate_type)
    {
        case ATC_CLCK_DISABLE:
            err_code = MNSS_DeactivateSSEx(dual_sys, ss_code, basic_service);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_DEACTIVATE_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            }
            else if (err_code == ERR_MNSS_FDN_FAILED)
            {
                ATC_TRACE_LOW("ATC: HandleClckSSReq, FDN check failed");
                return ERR_SIM_FDN_FAILED;
            }
            else
            {
                ATC_TRACE_LOW("ATC: HandleClckSSReq, MNSS_DeactivateSS, failure:%d", err_code);
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            break;

        case ATC_CKCK_ENABLE:
            err_code = MNSS_ActivateSSEx(dual_sys, ss_code, basic_service);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_ACTIVATE_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            }
            else if (err_code == ERR_MNSS_FDN_FAILED)
            {
                ATC_TRACE_LOW("ATC: HandleClckSSReq, FDN check failed");
                return ERR_SIM_FDN_FAILED;
            }
            else
            {
                ATC_TRACE_LOW("ATC: HandleClckSSReq, MNSS_ActivateSS, failure:%d", err_code);
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            break;

        case ATC_CLCK_QUERY_STATUS:
            err_code = MNSS_InterrogateServiceEx(dual_sys, ss_code, basic_service);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                s_ss_context[dual_sys].service_class = class_type;
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_INTERROGATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            }
            else if (err_code == ERR_MNSS_FDN_FAILED)
            {
                ATC_TRACE_LOW("ATC: HandleClckSSReq, FDN check failed");
                return ERR_SIM_FDN_FAILED;
            }
            else
            {
                ATC_TRACE_LOW("ATC: HandleClckSSReq, MNSS_InterrogateService, failure:%d", err_code);
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            break;

        default:
            return S_ATC_FAIL;
    }

    return S_ATC_SUCCESS;
}
/*****************************************************************************/
//  Description : This function handle the AT+CPWD command
//  This command is used to change the password
//            AT+CPWD=<fac>,<oldpin>,<newpin>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPWD)
{
#ifndef _ULTRA_LOW_CODE_
    uint8             cmd_type;
    uint8             *temp_ptr;
    uint8             param_len;
    uint8             fac_type;
    ATC_CPWD_PARAM_T  param[3];
    MN_BLOCK_T        password;        // the old pin
    MN_BLOCK_T        new_password;    // the new pin
    uint32  err_code;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(param, 0, sizeof(param));
    SCI_MEMSET(s_old_password, 0, sizeof(s_old_password));
    SCI_MEMSET(s_new_password, 0, sizeof(s_new_password));

    if(!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        ATC_TRACE_LOW("ATC: cpwd parameter is not correct");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    //get the operation type.
    param_len = PARA1.str_ptr->str_len;
    temp_ptr  = PARA1.str_ptr->str_ptr;
    ATC_GetCmdType(temp_ptr, (uint16)param_len, &cmd_type);

    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
            ATC_TRACE_LOW("ATC: cpwd command type is set");
            temp_ptr++;
            param_len--;

            if(!ATC_GetParams(temp_ptr, param_len, param, 3))
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            //handle the pin
            if(AT_CPWD_FAC_LEN != param[0].param_len || 0 == param[1].param_len
                    || 0 == param[2].param_len)
            {
                return ERR_INVALID_INDEX;
            }

            // check the old password
            if(ATC_CheckPinStr(param[1].param, param[1].param_len))
            {

                if(param[1].param_len < AT_PSW_MIN_LEN || param[1].param_len > AT_PSW_MAX_LEN)
                {
                    ATC_TRACE_LOW("ATC: WARNING, the length of passward is wrong!");
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                SCI_MEMCPY(password.blockbuf, param[1].param, param[1].param_len);
                password.blocklen = param[1].param_len;
                SCI_MEMCPY(s_old_password, param[1].param, AT_CLCK_PSW_LEN);
            }
            else
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            // check the new password
            if(ATC_CheckPinStr(param[2].param, param[2].param_len))
            {
                if(param[2].param_len < AT_PSW_MIN_LEN || param[2].param_len > AT_PSW_MAX_LEN)
                {
                    ATC_TRACE_LOW("ATC: WARNING, the length of passward is wrong!");
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                SCI_MEMCPY(new_password.blockbuf, param[2].param, param[2].param_len);
                new_password.blocklen = param[2].param_len;
                SCI_MEMCPY(s_new_password, param[2].param, AT_CLCK_PSW_LEN);
            }
            else
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            // check the fac type
            if(!ATC_GetFacType(param[0].param, param[0].param_len, &fac_type))
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            switch(fac_type)
            {
                case AT_PWD_SC:

                    if(!atc_config_ptr->sim_flag[dual_sys])
                    {
                        ATC_TRACE_LOW("ATC: cpwd sim is not ready");
                        return ERR_OPERATION_NOT_ALLOWED;
                    }

                    err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_CHANGE, MNSIM_PIN1, &password, &new_password);

                    if(err_code == ERR_MNPHONE_NO_ERR)
                    {
                        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                          APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }

                    break;
                    
        #ifndef ATC_SAT_ENABLE
                case AT_PWD_FD:
                case AT_PWD_P2:
                    if(!atc_config_ptr->sim_flag[dual_sys])
                    {
                        ATC_TRACE_LOW("ATC: cpwd sim is not ready");
                        return ERR_OPERATION_NOT_ALLOWED;
                    }

                    err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_CHANGE, MNSIM_PIN2, &password, &new_password);

                    if(err_code == ERR_MNPHONE_NO_ERR)
                    {
                        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                          APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                    break;
            #endif
                    //The following is about SS.
                case AT_PWD_AO:
                case AT_PWD_OI:
                case AT_PWD_OX:
                case AT_PWD_AI:
                case AT_PWD_IR:
                case AT_PWD_AB:
                case AT_PWD_AG:
                case AT_PWD_AC:
                    if(!atc_config_ptr->sim_flag[dual_sys])
                    {
                        return ERR_SIM_NOT_INSERTED;
                    }

                    if(password.blocklen > 4 || new_password.blocklen > 4)
                    {
                        ATC_TRACE_LOW("ATC: cpwd password length is large");
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    
                    // All Barring services
                    err_code = MNSS_RegisterPasswordEx(dual_sys, MN_SS_ALL_BAR_SERVICE);
                    if(err_code == ERR_MNSS_NO_ERR)
                    {
                        SCI_TRACE_MID("ATC:AT_PWD_AB");
                        SCI_MEMCPY(s_new_password_again, s_new_password, AT_CLCK_PSW_LEN);
                        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                          APP_MN_REGISTER_PASSWORD_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                    }
                    else if (err_code == ERR_MNSS_FDN_FAILED)
                    {
                        ATC_TRACE_LOW("ATC: ATC_ProcessCPWD, FDN check failed");
                        return ERR_SIM_FDN_FAILED;
                    }
                    else
                    {
                        ATC_TRACE_LOW("ATC: ATC_ProcessCPWD, MNSS_RegisterPasswordEx, failure:%d", err_code);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }

                    break;

                default:
                    return ERR_OPERATION_NOT_ALLOWED;
            }

            s_ss_context[dual_sys].cmd_id = AT_CMD_CPWD;
            s_ss_context[dual_sys].ss_code = (MN_SS_CODE_E)0;
            break;

        case ATC_CMD_TYPE_READ:
            ATC_TRACE_LOW("ATC: cpwd command type is read");
            return ERR_OPERATION_NOT_ALLOWED;

        case ATC_CMD_TYPE_TEST:
            ATC_TRACE_LOW("ATC: cpwd command type is test");
            ATC_FormatCpwdStr(g_rsp_str);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            return S_ATC_SUCCESS;

        default:
            return ERR_OPERATION_NOT_ALLOWED;
    }

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_GET_PASSWORD_IND signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
ATC_STATUS ATC_ProcessGetPswInd(
    ATC_CONFIG_T                   *atc_config_ptr,
    APP_MN_GET_PASSWORD_IND_T      *sig_ptr
)
{
#ifndef _ULTRA_LOW_CODE_
    SCI_TRACE_MID("ATC:ATC_ProcessGetPswInd");
    
    switch(s_ss_context[sig_ptr->dual_sys].cmd_id)
    {
        case AT_CMD_CPWD:
            SCI_TRACE_MID("ATC:AT_CMD_CPWD");

            //Send the old password
            if(MN_SS_ENTER_PW == sig_ptr->guidance_info)
            {
                SCI_TRACE_MID("ATC:s_old_password");
                MNSS_SendUserPasswordEx(sig_ptr->dual_sys, s_old_password);
            }
            //Send the new password
            else if(MN_SS_ENTER_NEW_PW == sig_ptr->guidance_info)
            {
                SCI_TRACE_MID("ATC:s_new_password");
                MNSS_SendUserPasswordEx(sig_ptr->dual_sys, s_new_password);
            }
            else if(MN_SS_ENTER_NEW_PW_AGAIN == sig_ptr->guidance_info)
            {
                SCI_TRACE_MID("ATC:s_new_password again");
                MNSS_SendUserPasswordEx(sig_ptr->dual_sys, s_new_password_again);
            }
            else
            {
                ATC_TRACE_LOW("ATC: received error singal in get password indicate!!!");
            }

            break;
        case AT_CMD_CLCK:
            SCI_TRACE_MID("ATC:s_clck_psw");
            MNSS_SendUserPasswordEx(sig_ptr->dual_sys, s_clck_psw);
            break;
        default:
            break;
    }
#endif /* _ULTRA_LOW_CODE_ */

    return S_ATC_DEFAULT_HANDLE;
}
/*****************************************************************************/
//  Description : This function handles the APP_MN_REGISTER_PASSWORD_CNF signal
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:         modify by longting.zhao 2011.8.26 notify app incorrect passward
/*****************************************************************************/
ATC_STATUS ATC_ProcessPwdCnf(
    ATC_CONFIG_T                   *atc_config_ptr,
    APP_MN_REGISTER_PASSWORD_CNF_T      *sig_ptr
)
{
#ifndef _ULTRA_LOW_CODE_
    
    SCI_TRACE_LOW("ATC: _processPwdCnf, sig_ptr->err_code= %d, result_type=%d", sig_ptr->err_code,sig_ptr->result_type);

    if(MN_SS_OPERATION_SUCC == sig_ptr->result_type)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else if(MN_SS_ERROR_NEGATIVE_PW_CHECK == sig_ptr->err_code)
    {
        SCI_TRACE_LOW("ATC: _processPwdCnf, incorrect passward");
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_INCORRECT_PASSWORD);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
    }

    s_ss_context[sig_ptr->dual_sys].cmd_id = (int16)AT_INVALID_CMD_ID;
    s_ss_context[sig_ptr->dual_sys].ss_code = (MN_SS_CODE_E)0; 
#endif /* _ULTRA_LOW_CODE_ */

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_RELEASE_SS_IND signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessRelCnf(
    ATC_CONFIG_T               *atc_config_ptr,
    APP_MN_RELEASE_SS_IND_T    *sig_ptr
)
{
    ATC_TRACE_LOW("ATC: APP_MN_RELEASE_SS_IND");
    //handle for the USSD.
    if(TRUE == s_cusd_isresponse[sig_ptr->dual_sys])
    {
        ATC_TRACE_LOW("ATC: APP_MN_RELEASE_SS_IND");
        s_cusd_isresponse[sig_ptr->dual_sys] = FALSE;

        //if enable to present result,show it.
        if(AT_CUSD_ENABLE == s_cusd_presentation[sig_ptr->dual_sys])
        {
            ATC_UpCUSDInfo(sig_ptr->dual_sys, atc_config_ptr, NULL, 2);
        }
    }
    else // handle for the other ss command. MNSS doesn't send APP_MN_RELEASE_IND for ussd notify event
    {
        // atc receives APP_MN_SS_RELEASE_IND instead of APP_MN_SS_INTERROGATE_CNF/
        // APP_MN_SS_REGISTER_CNF/APP_MN_ERASE_SS_CNF/APP_MN_ACTIVATE_SS_CNF/APP_MN_DEACTIVATE_SS_CNF,
        // atc should send response to TE
        if((AT_CMD_CCFC == s_ss_context[sig_ptr->dual_sys].cmd_id) ||  
            (AT_CMD_CCWA == s_ss_context[sig_ptr->dual_sys].cmd_id) || 
            (AT_CMD_CLCK == s_ss_context[sig_ptr->dual_sys].cmd_id) || 
    #ifndef ATC_SAT_ENABLE
            (AT_CMD_CLIP == s_ss_context[sig_ptr->dual_sys].cmd_id) || 
            (AT_CMD_CLIR == s_ss_context[sig_ptr->dual_sys].cmd_id) ||
            (AT_CMD_COLP == s_ss_context[sig_ptr->dual_sys].cmd_id) ||
            (AT_CMD_COLR == s_ss_context[sig_ptr->dual_sys].cmd_id) ||
    #endif
            (AT_CMD_CPWD == s_ss_context[sig_ptr->dual_sys].cmd_id))
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
            s_ss_context[sig_ptr->dual_sys].cmd_id = (int16)AT_INVALID_CMD_ID;
            s_ss_context[sig_ptr->dual_sys].ss_code = (MN_SS_CODE_E)0;   
        }
    }

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
    int32           i;
    BOOLEAN    flag = FALSE;

    for(i = 0; i < call_state_ptr->call_amount; i++)
    {
        if((ATC_CALL_HOLD == call_state_ptr->call_context[i].hold_state) &&
                (ATC_CALL_ACTIVE_STATE == call_state_ptr->call_context[i].call_entity_state))
        {
            *call_id = call_state_ptr->call_context[i].call_id;
            flag = TRUE;
            break;
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
    int32            i;
    BOOLEAN          flag = FALSE;

    for(i = 0; i < call_state_ptr->call_amount; i++)
    {
        if((ATC_CALL_ACTIVE_STATE == call_state_ptr->call_context[i].call_entity_state) &&
                (ATC_HOLD_IDLE == call_state_ptr->call_context[i].hold_state))
        {
            *call_id = call_state_ptr->call_context[i].call_id;
            flag = TRUE;
            break;
        }
    }

    return flag;
}

/*****************************************************************************/
//  Description : This function check if there is a waiting call
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN CheckPresentWaitCall(
    ATC_CALLS_STATE_T   *call_state_ptr,
    uint8               *call_id
)
{
    int32           i;
    BOOLEAN     flag = FALSE;

    // check all of the present calls
    for(i = 0; i < call_state_ptr->call_amount; i++)
    {
        if(ATC_CALL_INCOMING_STATE == call_state_ptr->call_context[i].call_entity_state)
        {
            *call_id = call_state_ptr->call_context[i].call_id;
            flag = TRUE;
            break;
        }
    }

    return flag;
}

/*****************************************************************************/
//  Description : This function check if there is an outgoing call
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
BOOLEAN CheckPresentOutgoingCall(
    ATC_CALLS_STATE_T   *call_state_ptr,
    uint8               *call_id
)
{
    int32           i;
    BOOLEAN         flag = FALSE;

    if(PNULL == call_state_ptr)
    {
        return flag;
    }

    // check all of the present calls
    for(i = 0; i < call_state_ptr->call_amount; i++)
    {
        if(ATC_CALL_OUTGOING_STATE == call_state_ptr->call_context[i].call_entity_state ||
                ATC_CALL_ALERTING_STATE == call_state_ptr->call_context[i].call_entity_state)
        {
            if(NULL != call_id)
            {
                *call_id = call_state_ptr->call_context[i].call_id;
            }

            flag = TRUE;
            break;
        }
    }

    return flag;
}

/*****************************************************************************/
//  Description : This function check the SS CNF signal
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:        modify by longting.zhao 2011.8.26 notify app incorrect passward
/*****************************************************************************/
LOCAL BOOLEAN ATC_ProcessSSCnf(
   ATC_CONFIG_T              *atc_config_ptr,
   APP_MN_INTERROGATE_CNF_T   *sig_ptr,
   BOOLEAN     op_type 
)
{
/*
    BOOLEAN op_type == TRUE
    {
        APP_MN_INTERROGATE_CNF,
        APP_MN_REGISTER_SS_CNF,
        APP_MN_ACTIVATE_SS_CNF
    }
    BOOLEAN  op_type == FALSE
    {
        APP_MN_ERASE_SS_CNF,
        APP_MN_DEACTIVATE_SS_CNF
    }
*/
    BOOLEAN is_op_succ = FALSE;
    int32 i;

    ATC_TRACE_LOW("ATC: ATC_ProcessSSCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d,cmd_id=%d,ss_code=%d",
                  sig_ptr->result_type, sig_ptr->list_num, sig_ptr->info_list[0].ss_status_present, sig_ptr->info_list[0].ss_status,
                  s_ss_context[sig_ptr->dual_sys].cmd_id, s_ss_context[sig_ptr->dual_sys].ss_code);

    //IF success,check further.
    if(MN_SS_OPERATION_SUCC == sig_ptr->result_type)
    {
        if(0 < sig_ptr->list_num)
        {
            for(i = 0; i < sig_ptr->list_num; i++)
            {
                if(sig_ptr->info_list[i].ss_status_present)
                {
                    if(op_type)
                    {
                        if((sig_ptr->info_list[i].ss_status == MN_SS_PROVITION) ||
                                (sig_ptr->info_list[i].ss_status == MN_SS_ACTIVE_OPERATIVE) ||
                                (sig_ptr->info_list[i].ss_status == MN_SS_ACITVE_QUIESCENT))
                        {
                            is_op_succ = TRUE;
                            break;
                        }
                    }
                    else
                    {
                        if((sig_ptr->info_list[i].ss_status == MN_SS_NOT_PROVITION) ||
                                (sig_ptr->info_list[i].ss_status == MN_SS_NOT_REGISTERED) ||
                                (sig_ptr->info_list[i].ss_status == MN_SS_NOT_ACTIVE))
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

        if((TRUE == is_op_succ) && (AT_CMD_CCFC == s_ss_context[sig_ptr->dual_sys].cmd_id))
        {
            BOOLEAN  is_service_active = op_type;

            /* Synchronize Service */
            switch(s_ss_context[sig_ptr->dual_sys].ss_code)
            {
                case MN_SS_ALL_FORWARD:
                    atc_config_ptr->atc_sync_srv.cfu    = is_service_active;
                    atc_config_ptr->atc_sync_srv.cfb    = is_service_active;
                    atc_config_ptr->atc_sync_srv.cfnry  = is_service_active;
                    atc_config_ptr->atc_sync_srv.cfnrc  = is_service_active;

                    atc_config_ptr->atc_sync_srv.cfcond = is_service_active;
                    atc_config_ptr->atc_sync_srv.cfall  = is_service_active;
                    break;

                case MN_SS_CFU:
                    atc_config_ptr->atc_sync_srv.cfu = is_service_active;
                    break;

                case MN_SS_ALL_COND_FORWARD:
                    atc_config_ptr->atc_sync_srv.cfb    = is_service_active;
                    atc_config_ptr->atc_sync_srv.cfnry  = is_service_active;
                    atc_config_ptr->atc_sync_srv.cfnrc  = is_service_active;

                    atc_config_ptr->atc_sync_srv.cfcond = is_service_active;
                    break;

                case MN_SS_CFB:
                    atc_config_ptr->atc_sync_srv.cfb = is_service_active;
                    break;

                case MN_SS_CFNRY:
                    atc_config_ptr->atc_sync_srv.cfnry = is_service_active;
                    break;

                case MN_SS_CFNRC:
                    atc_config_ptr->atc_sync_srv.cfnrc = is_service_active;
                    break;

                default:
                    break;
            }

            ATC_TRACE_LOW("ATC: ATC_ProcessSSCnf,cfall=%d,cfu=%d,cfcond=%d,cfb=%d,cfnry=%d,cfnrc=%d",
                          atc_config_ptr->atc_sync_srv.cfall,
                          atc_config_ptr->atc_sync_srv.cfu,
                          atc_config_ptr->atc_sync_srv.cfcond,
                          atc_config_ptr->atc_sync_srv.cfb,
                          atc_config_ptr->atc_sync_srv.cfnry,
                          atc_config_ptr->atc_sync_srv.cfnrc);
        }
    }

    //Return the indication
    if(is_op_succ)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
    }
    else if(MN_SS_ERROR_NEGATIVE_PW_CHECK == sig_ptr->err_code)
    {
        SCI_TRACE_LOW("ATC: _processSSCnf, incorrect passward");
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_INCORRECT_PASSWORD);    
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
    }

    s_ss_context[sig_ptr->dual_sys].cmd_id = (int16)AT_INVALID_CMD_ID;
    s_ss_context[sig_ptr->dual_sys].ss_code = (MN_SS_CODE_E)0; 
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
    APP_MN_INTERROGATE_CNF_T *sig_ptr,
    BOOLEAN              *is_service_active
)
{
    int32 i = 0;
    BOOLEAN is_inter_succ = FALSE;

    ATC_TRACE_LOW("ATC: ATC_CheckInterrogateCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d,service_type=%d,service_code=%d",
                  sig_ptr->result_type, sig_ptr->list_num, sig_ptr->info_list[0].ss_status_present, sig_ptr->info_list[0].ss_status,
                  sig_ptr->info_list[0].basic_service.service_type, sig_ptr->info_list[0].basic_service.service_code);
    *is_service_active = FALSE;

    //IF success,check further.
    if(MN_SS_OPERATION_SUCC == sig_ptr->result_type)
    {
        if(0 < sig_ptr->list_num)
        {
            is_inter_succ = TRUE;

            for(i = 0; i < sig_ptr->list_num; i++)
            {
                if(MN_SS_ALL_SERVICE == sig_ptr->info_list[i].basic_service.service_type ||
                        (MN_SS_BEARER_SERVICE == sig_ptr->info_list[i].basic_service.service_type) ||
                        (MN_SS_TELE_SERVICE == sig_ptr->info_list[i].basic_service.service_type
                         && ((MN_SS_ALL_SPEECH_TRANS_SERVICES == sig_ptr->info_list[i].basic_service.service_code)
                             || (MN_SS_ALL_TELESERVICES == sig_ptr->info_list[i].basic_service.service_code))))
                {
                    if(sig_ptr->info_list[i].ss_status_present)
                    {
                    #ifndef ATC_SAT_ENABLE
                        if( (AT_CMD_CLIP == s_ss_context[sig_ptr->dual_sys].cmd_id) || 
                            (AT_CMD_CLIR == s_ss_context[sig_ptr->dual_sys].cmd_id) ||
                            (AT_CMD_COLP == s_ss_context[sig_ptr->dual_sys].cmd_id) ||
                            (AT_CMD_COLR == s_ss_context[sig_ptr->dual_sys].cmd_id))
                        {
                            if(sig_ptr->info_list[i].ss_status & PROVISIONED_BIT)
                            {
                                *is_service_active = TRUE;
                                break;
                            }
                        }
                        else
                    #endif
                        {
                            if((sig_ptr->info_list[i].ss_status == MN_SS_PROVITION) ||
                                (sig_ptr->info_list[i].ss_status == MN_SS_ACTIVE_OPERATIVE) ||
                                (sig_ptr->info_list[i].ss_status == MN_SS_ACITVE_QUIESCENT))
                            {
                                *is_service_active = TRUE;
                                break;
                            }
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
//  release the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldUdub(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
    ATC_STATUS          status = S_ATC_DEFAULT_HANDLE;
    uint8                      call_id = 0xff;
    uint8                      i;

    atc_config_ptr->ss_operate[dual_sys] = ATC_REL_HELD_CALLS_OR_SET_UDUB;

    // check if there is a waiting call
    if(CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
    {
        MNCALL_DisconnectCallEx(dual_sys, (uint8)call_id, MN_CAUSE_USER_BUSY, PNULL);

        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
                            APP_MN_CALL_DISCONNECTED_IND, call_id, ATC_DOMAIN_CS, dual_sys);
    }
    else
    {
        for(i = 0; i < g_calls_state[dual_sys].call_amount; i++)
        {
            if((ATC_CALL_HOLD == g_calls_state[dual_sys].call_context[i].hold_state) &&
                    (ATC_CALL_ACTIVE_STATE == g_calls_state[dual_sys].call_context[i].call_entity_state))
            {
                call_id = g_calls_state[dual_sys].call_context[i].call_id;

                MNCALL_DisconnectCallEx(dual_sys, (uint8)call_id, MN_CAUSE_NORMAL_CLEARING, PNULL);

                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
                                   APP_MN_CALL_DISCONNECTED_IND, call_id, ATC_DOMAIN_CS, dual_sys);
            }
        }

        if(call_id != 0xff)
        {
            return status;
        }
        else
        {
            atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
            status = S_ATC_SUCCESS;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles release active call and accept other call
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelAcceptOthers(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
    ATC_STATUS       status = S_ATC_DEFAULT_HANDLE;
    uint8                   call_id = 0xff;
    uint8                    i;

    atc_config_ptr->ss_operate[dual_sys] = ATC_REL_ACTIVE_CALLS_AND_ACCEPT_OTHERS;

    for(i = 0; i < g_calls_state[dual_sys].call_amount; i++)
    {
        if((ATC_HOLD_IDLE == g_calls_state[dual_sys].call_context[i].hold_state) &&
                (ATC_CALL_ACTIVE_STATE == g_calls_state[dual_sys].call_context[i].call_entity_state))
        {
            call_id = g_calls_state[dual_sys].call_context[i].call_id;

            MNCALL_DisconnectCallEx(dual_sys, (uint8)call_id, MN_CAUSE_NORMAL_CLEARING, PNULL);

            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_CALL_DISCONNECTED_IND, call_id, ATC_DOMAIN_CS, dual_sys);
        }
    }

    if(call_id == 0xff)
    {
        // if present waiting calls then accpet it
        if(CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
        {
            MNCALL_ConnectCallEx(dual_sys, call_id, PNULL);
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
                              (uint16)APP_MN_SETUP_COMPLETE_IND, call_id, ATC_DOMAIN_CS, dual_sys);
        }
        else
        {
            // if present held calls then retrieve then
            if(CheckPresentHeldCall(&g_calls_state[dual_sys], &call_id))
            {
                MNCALL_RetrieveCallEx(dual_sys, call_id);
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_CALL_RETRIEVED_CNF, call_id, ATC_DOMAIN_CS, dual_sys);
            }
            else
            {
                ATC_TRACE_LOW("ATC: HandleChldRelAcceptOthers no targeted call(s)");
                status = S_ATC_SUCCESS; //error permitted for APP call disconnection
                atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
            }
        }
    }

    return status;
}
/*****************************************************************************/
//  Description : This function hold the active calls and accept the held or waiting calls
//  release the waiting call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldHoldAcceptOthers(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
    uint8                   active_id;
    uint8                   waiting_id;
    uint8                   held_id;
    ATC_STATUS              status = S_ATC_DEFAULT_HANDLE;

    atc_config_ptr->ss_operate[dual_sys] = ATC_HOLD_CALLS_AND_ACCEPT_OTHERS;

    // check if there are active calls
    if(CheckPresentActiveCall(&g_calls_state[dual_sys], &active_id))
    {
        if(CheckPresentHeldCall(&g_calls_state[dual_sys], &held_id))
        {
            MNCALL_SwapCallsEx(dual_sys, active_id, held_id);
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_CALL_RETRIEVED_CNF, held_id, ATC_DOMAIN_CS, dual_sys);
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_CALL_HELD_CNF, active_id, ATC_DOMAIN_CS, dual_sys);
        }
        else
        {
            MNCALL_HoldCallEx(dual_sys, active_id);
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_CALL_HELD_CNF, active_id, ATC_DOMAIN_CS, dual_sys);
        }
    }
    else
    {
        // check if there is waiting calls
        if(CheckPresentWaitCall(&g_calls_state[dual_sys], &waiting_id))
        {
            MNCALL_ConnectCallEx(dual_sys, waiting_id, PNULL);
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_SETUP_COMPLETE_IND, waiting_id, ATC_DOMAIN_CS, dual_sys);
        }
        else
        {
            // check if there are held calls
            if(CheckPresentHeldCall(&g_calls_state[dual_sys], &held_id))
            {
                MNCALL_RetrieveCallEx(dual_sys, held_id);
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_CALL_RETRIEVED_CNF, held_id, ATC_DOMAIN_CS, dual_sys);
            }
            else
            {
                ATC_TRACE_LOW("ATC: HandleChldHoldAcceptOthers no targeted call(s)");
                status = S_ATC_SUCCESS; //error permitted for APP call disconnection
                atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
            }
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function adds the held call to the conversation calls
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldMultCall(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E  dual_sys
)
{
    uint8                   active_id;
    uint8                   held_id;
    ATC_STATUS              status = S_ATC_DEFAULT_HANDLE;

    if(CheckPresentHeldCall(&g_calls_state[dual_sys], &held_id) &&
            CheckPresentActiveCall(&g_calls_state[dual_sys], &active_id))
    {
        MNCALL_BuildMPTYEx(dual_sys, held_id, active_id);
        atc_config_ptr->ss_operate[dual_sys] = ATC_ADD_HELD_TO_CONVERSATION;
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                          APP_MN_BUILD_MPTY_CNF, active_id, ATC_DOMAIN_CS, dual_sys);
    }
    else
    {
        ATC_TRACE_LOW("ATC: HandleChldMultCall no targeted calls");
        status = S_ATC_SUCCESS; //error permitted for APP call disconnection
        atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
    }

    return status;
}

/*****************************************************************************/
//  Description : check ect condition:
//        a) First Call (Active, Held), Second Call (Active, Idle);
//        b) First Call (Active, Held), Second Call (Call Delivered, Idle).
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
        atc_config_ptr->ss_operate[dual_sys] = ATC_ECT;
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                          APP_MN_ECT_INVOKE_CNF, held_id, ATC_DOMAIN_CS, dual_sys);
    }
    else
    {
        ATC_TRACE_LOW("ATC: HandleEctCall no targeted calls");
        status = S_ATC_SUCCESS; //error permitted for APP call disconnection
        atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles release active call and accept held call
//  Global resource dependence : none
//  Author:       Elvis Xu
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelAcceptHeld(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
    ATC_STATUS       status = S_ATC_DEFAULT_HANDLE;
    uint8                   call_id = 0xff;
    uint8                    i;

    atc_config_ptr->ss_operate[dual_sys] = ATC_REL_ACTIVE_CALLS_AND_ACCEPT_HELD_CALL;

    for(i = 0; i < g_calls_state[dual_sys].call_amount; i++)
    {
        if((ATC_HOLD_IDLE == g_calls_state[dual_sys].call_context[i].hold_state) &&
                (ATC_CALL_ACTIVE_STATE == g_calls_state[dual_sys].call_context[i].call_entity_state))
        {
            call_id = g_calls_state[dual_sys].call_context[i].call_id;

            MNCALL_DisconnectCallEx(dual_sys, (uint8)call_id, MN_CAUSE_NORMAL_CLEARING, PNULL);

            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_CALL_DISCONNECTED_IND, call_id, ATC_DOMAIN_CS, dual_sys);
        }
    }

    if(call_id == 0xff)
    {
        // if present held calls then retrieve then
        if(CheckPresentHeldCall(&g_calls_state[dual_sys], &call_id))
        {
            MNCALL_RetrieveCallEx(dual_sys, call_id);
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_CALL_RETRIEVED_CNF, call_id, ATC_DOMAIN_CS, dual_sys);
        }
        else
        {
            status = S_ATC_SUCCESS; //error permitted for APP call disconnection
            atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles releases all existing calls
//  Global resource dependence : none
//  Author:       longting.zhao
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelAllCalls(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
    uint8               xcall_id  = 0;
    uint8            call_index = 0;
    ATC_STATUS       status = ERR_OPERATION_NOT_ALLOWED;

    for(call_index = 0; call_index < g_calls_state[dual_sys].call_amount; call_index++)
    {
        if(ATC_CALL_IDLE_STATE != g_calls_state[dual_sys].call_context[call_index].call_entity_state)
        {
            xcall_id = g_calls_state[dual_sys].call_context[call_index].call_id;

            MNCALL_DisconnectCallEx(dual_sys, (uint8)xcall_id, MN_CAUSE_NORMAL_CLEARING, PNULL);

            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
                            (uint16)APP_MN_CALL_DISCONNECTED_IND, xcall_id, ATC_DOMAIN_CS, dual_sys);

            atc_config_ptr->ss_operate[dual_sys] = ATC_REL_EXISTING_CALLS;
            status = S_ATC_DEFAULT_HANDLE;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles AT+CHLD=1X release an active call 
//  Global resource dependence : none
//  Author:       longting.zhao
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelActiveCall(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys,
     int32         operation_code
)
{
    uint8               xcall_id  = 0;
    uint8            call_index = 0;
    ATC_STATUS       status = S_ATC_DEFAULT_HANDLE;

    // check the call id it can not exceed MN_CALL_MAX_CALL_NUM
    if((operation_code - ATC_REL_SPECIFIC_X_CALL_BASE) <= MN_CALL_MAX_CALL_NUM)
    {
        // get the specific call id
        xcall_id = (uint8)(operation_code - ATC_REL_SPECIFIC_X_CALL_BASE - 1);
        call_index = GetIndexOfCallContext(xcall_id, dual_sys);

        if((ATC_INVALID_CALLID != call_index)
            &&(ATC_CALL_ACTIVE_STATE == g_calls_state[dual_sys].call_context[call_index].call_entity_state))
        {

            MNCALL_DisconnectCallEx(dual_sys, (uint8)xcall_id, MN_CAUSE_NORMAL_CLEARING, PNULL);

            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
                            (uint16)APP_MN_CALL_DISCONNECTED_IND, xcall_id, ATC_DOMAIN_CS, dual_sys);
            atc_config_ptr->ss_operate[dual_sys] = ATC_REL_SPECIFIC_X_CALL_BASE;
        }
        else
        {
            status = ERR_INVALID_INDEX;
        }
    }
    else
    {
        status = ERR_OPERATION_NOT_ALLOWED;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles AT+CHLD=2X split call from mulitiparty call
//  Global resource dependence : none
//  Author:       longting.zhao
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldSplitCallFromMpty(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys,
    int32         operation_code
)
{
    uint8 temp_call_id = 0;
    uint8               xcall_id  = 0;
    uint8            call_index = 0;
    ATC_STATUS       status = S_ATC_DEFAULT_HANDLE;

    xcall_id = (uint8)(operation_code - ATC_HOLD_CALLS_EXCEPT_X_BASE - 1);
    call_index = GetIndexOfCallContext(xcall_id, dual_sys);

    if(TRUE == CheckPresentHeldCall(&g_calls_state[dual_sys], &temp_call_id))
    {
        status = ERR_OPERATION_NOT_ALLOWED;
    }
    else if((ATC_INVALID_CALLID != call_index)
            && (ATC_CALL_ACTIVE_STATE == g_calls_state[dual_sys].call_context[call_index].call_entity_state))
    {
        MNCALL_SplitMPTYEx(dual_sys, xcall_id);
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
        APP_MN_SPLIT_MPTY_CNF, xcall_id, ATC_DOMAIN_CS, dual_sys);
        atc_config_ptr->ss_operate[dual_sys] = ATC_REL_SPECIFIC_X_CALL_BASE;
    }
    else
    {
        status = ERR_OPERATION_NOT_ALLOWED;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles AT+CHLD=7X release specific call
//  Global resource dependence : none
//  Author:       longting.zhao
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS HandleChldRelSpecificCall(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys,
     int32         operation_code
)
{
    uint8               xcall_id  = 0;
    uint8            call_index = 0;
    ATC_STATUS       status = S_ATC_DEFAULT_HANDLE;

    // check the call id it can not exceed MN_CALL_MAX_CALL_NUM
    if((operation_code - ATC_REL_SPECIFIC_EXISTING_X_CALL_BASE) <= MN_CALL_MAX_CALL_NUM)
    {
        // get the specific call id
        xcall_id = (uint8)(operation_code - ATC_REL_SPECIFIC_EXISTING_X_CALL_BASE - 1);
        call_index = GetIndexOfCallContext(xcall_id, dual_sys);

        if((ATC_INVALID_CALLID != call_index) &&
        (ATC_CALL_IDLE_STATE != g_calls_state[dual_sys].call_context[call_index].call_entity_state))
        {

            MNCALL_DisconnectCallEx(dual_sys, (uint8)xcall_id, MN_CAUSE_NORMAL_CLEARING, PNULL);

            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
                            (uint16)APP_MN_CALL_DISCONNECTED_IND, xcall_id, ATC_DOMAIN_CS, dual_sys);
            atc_config_ptr->ss_operate[dual_sys] = ATC_REL_SPECIFIC_EXISTING_X_CALL_BASE;
        }
        else
        {
            status = ERR_INVALID_INDEX;
        }
    }
    else
    {
        status = ERR_OPERATION_NOT_ALLOWED;
    }

    return status;
}


/*****************************************************************************/
//  Description : This function handles hold active call only
//  Global resource dependence : none
//  Author:       longting.zhao
//  Note:
/*****************************************************************************/
LOCAL void HandleChldHoldActiveCall(
    ATC_CONFIG_T        *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
    uint8                   active_id = 0;

    // check if there are active calls
    if(CheckPresentActiveCall(&g_calls_state[dual_sys], &active_id))
    {
        atc_config_ptr->ss_operate[dual_sys] = ATC_HOLD_ACTIVE_CALL_ONLY;

        MNCALL_HoldCallEx(dual_sys, active_id);
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                        APP_MN_CALL_HELD_CNF, active_id, ATC_DOMAIN_CS, dual_sys);
    }
    else
    {
        atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
    }
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_BUILD_MPTY_CNF signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessBuildMptyCnf(
    ATC_CONFIG_T              *atc_config_ptr,
    APP_MN_BUILD_MPTY_CNF_T   *sig_ptr
)
{
    uint8   call_index;
    int32   i;

    if(sig_ptr->req_is_accepted)
    {
        call_index = GetIndexOfCallContext(sig_ptr->call_id, sig_ptr->dual_sys);

        SCI_ASSERT(ATC_INVALID_CALLID != call_index);/*assert verified: check valid value*/
        for(i = 0; i < g_calls_state[sig_ptr->dual_sys].call_amount; i++)
        {
            if(ATC_CALL_ACTIVE_STATE == g_calls_state[sig_ptr->dual_sys].call_context[i].call_entity_state)
            {
                g_calls_state[sig_ptr->dual_sys].call_context[i].mpty_state = ATC_CALL_MPTY;
                g_calls_state[sig_ptr->dual_sys].call_context[i].hold_state = ATC_HOLD_IDLE;
                    
                ATC_UpDSCIInfo(sig_ptr->dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, i, 0);
            }
        }


        ATC_TRACE_LOW("ATC: build MPTY call successfully");
        // reponse the at+chld=3
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_TRACE_LOW("ATC: build MPTY call failed");
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
    }

    atc_config_ptr->ss_operate[sig_ptr->dual_sys] = ATC_INVALID_OPERATE;
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
        ATC_TRACE_LOW("ATC: ect invoke successfully");
        // reponse the at+chld=4
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_TRACE_LOW("ATC: ect invoke failed");
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
    }

    atc_config_ptr->ss_operate[sig_ptr->dual_sys] = ATC_INVALID_OPERATE;
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_RETRIEVED_CNF signal.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessCallRetrievCnf(
    ATC_CONFIG_T                  *atc_config_ptr,
    APP_MN_CALL_RETRIEVED_CNF_T   *sig_ptr
)
{
    uint8                   call_index;
    uint8                   active_id = 0xff;
    uint8                   held_id = 0xff;
    BOOLEAN                 is_swap = FALSE;
    MN_DUAL_SYS_E    dual_sys = sig_ptr->dual_sys;
    int32   i;

    ATC_TRACE_LOW("ATC: receive the APP_MN_CALL_RETRIEVED_CNF signal");

    if(sig_ptr->req_is_accepted)
    {
        call_index = GetIndexOfCallContext(sig_ptr->call_id, sig_ptr->dual_sys);

        SCI_ASSERT(ATC_INVALID_CALLID != call_index);/*assert verified: check valid value*/

        if(CheckPresentActiveCall(&g_calls_state[dual_sys], &active_id) &&
                CheckPresentHeldCall(&g_calls_state[dual_sys], &held_id))
        {
            is_swap = TRUE;
        }

        if(ATC_CALL_MPTY == g_calls_state[dual_sys].call_context[call_index].mpty_state) //lint !e661 !e662
        {
            for(i = 0; i < g_calls_state[dual_sys].call_amount; i++)
            {
                if(ATC_CALL_MPTY == g_calls_state[dual_sys].call_context[i].mpty_state)
                {
                    g_calls_state[dual_sys].call_context[i].hold_state = ATC_HOLD_IDLE;
                }
            }
        }
        else
        {
            g_calls_state[dual_sys].call_context[call_index].hold_state = ATC_HOLD_IDLE; //lint !e661 !e662
        }

        ATC_UpECINDInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, 0, 0, sig_ptr->call_id + 1, 0, 0);
        ATC_UpDSCIInfo(sig_ptr->dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr,  call_index, 0);

        if (is_swap)
        {
            ATC_TRACE_LOW("ATC: multi party calls swapping and waiting for APP_MN_CALL_HELD_CNF msg");
            //ss_operate should stay in current state, APP_MN_CALL_HELD_CNF will clear it
        }
        else
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
        }
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
        atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_HELD_IND signal and
//                sends the +CSSU: unsolicited result code
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessHeldInd(
    ATC_CONFIG_T              *atc_config_ptr,
    APP_MN_CALL_HELD_IND_T    *sig_ptr
)
{
    SCI_ASSERT((PNULL != atc_config_ptr) && (PNULL != sig_ptr));/*assert verified: check null pointer*/

    // determine if the +CSSU is enable
    if(atc_config_ptr->cssu_flag[sig_ptr->dual_sys])
    {
        // A call has been put on hold (during voice call)
        sprintf((char *)g_rsp_str, "+CSSU:2");
        ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
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
ATC_STATUS  ATC_ProcessHeldCnf(
    ATC_CONFIG_T            *atc_config_ptr,
    APP_MN_CALL_HELD_CNF_T  *sig_ptr
)
{
    uint8                   call_id;
    uint8                   call_index;
    uint8                   active_id = 0xff;
    uint8                   held_id = 0xff;
    BOOLEAN                 is_swap = FALSE;
    MN_DUAL_SYS_E    dual_sys = sig_ptr->dual_sys;
    int32                   i;
#ifdef _MUX_ENABLE_
    uint8    temp_link_id = atc_config_ptr->current_link_id;
#endif

    ATC_TRACE_LOW("ATC: receive the APP_MN_CALL_HELD_CNF signal");

    if(sig_ptr->req_is_accepted)
    {
        call_index = GetIndexOfCallContext(sig_ptr->call_id, dual_sys);

        SCI_ASSERT(ATC_INVALID_CALLID != call_index);/*assert verified: check valid value*/

        if(ATC_HOLD_ACTIVE_CALL_ONLY != atc_config_ptr->ss_operate[dual_sys])
        {
            if(CheckPresentActiveCall(&g_calls_state[dual_sys], &active_id) &&
                    CheckPresentHeldCall(&g_calls_state[dual_sys], &held_id))
            {
                is_swap = TRUE;
            }
        }

        if(ATC_CALL_MPTY == g_calls_state[dual_sys].call_context[call_index].mpty_state) //lint !e661 !e662
        {
            for(i = 0; i < g_calls_state[dual_sys].call_amount; i++)
            {
                if(ATC_CALL_MPTY == g_calls_state[dual_sys].call_context[i].mpty_state)
                {
                    g_calls_state[dual_sys].call_context[i].hold_state = ATC_CALL_HOLD;
                }
            }
        }
        else
        {
            g_calls_state[dual_sys].call_context[call_index].hold_state = ATC_CALL_HOLD; //lint !e661 !e662
        }

        ATC_UpECINDInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, 0, 1, sig_ptr->call_id + 1, 0, 0);
        ATC_UpDSCIInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, call_index, 1);

        if(ATC_HOLD_CALLS_AND_ACCEPT_OTHERS == atc_config_ptr->ss_operate[dual_sys])
        {
            if(is_swap)
            {
                ATC_TRACE_LOW("ATC:is_swap");
            }
            else if(CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
            {
                MNCALL_ConnectCallEx(dual_sys, call_id, PNULL);
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_SETUP_COMPLETE_IND, call_id, ATC_DOMAIN_CS, dual_sys);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
            }
        }
        else if(ATC_HOLD_ACTIVE_CALL_ONLY == atc_config_ptr->ss_operate[dual_sys])
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
        }
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
        atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
    }

    //continue to dial the call stored in buffer previously
    if ((atc_config_ptr->ss_operate[dual_sys] == ATC_INVALID_OPERATE)
         && (g_last_mo_call.is_buffer_call))
    {
        g_last_mo_call.is_buffer_call = FALSE;

        if(g_last_mo_call.called_num_present)
        {
            if(MN_RETURN_SUCCESS == MNCALL_StartCallEx(dual_sys, g_last_mo_call.call_type, &g_last_mo_call.called_num, PNULL, g_last_mo_call.call_clir, PNULL))
            {
                g_atc_call_flag = TRUE;
                g_atc_call_need_disc = FALSE;    //reset the neee_disc flag
                atc_config_ptr->cpas = ATC_CPAS_STATUS_PROGRESS;
                ATC_SET_CURRENT_LINK_ID_MUX(g_last_mo_call.buffer_link_id);
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, (uint16)APP_MN_CALL_START_IND, 0xff, ATC_DOMAIN_CS, dual_sys);
                ATC_SET_CURRENT_LINK_ID_MUX(temp_link_id);

                switch (g_last_mo_call.call_type)
                {
                    case MN_CALL_TYPE_NORMAL:
                    case MN_CALL_TYPE_EMERGENCY:
                    case MN_CALL_TYPE_SPEECH_PREFER:
                        return S_ATC_SUCCESS;
                    
                    default :
                        return S_ATC_DEFAULT_HANDLE;
                }
            }
            else
            {
                ATC_TRACE_LOW("ATC: failure in MNCALL_StartCall");
                return ERR_INVALID_INDEX;
            }
        }
        else
        {
            ATC_TRACE_LOW("ATC: no call number recently.");
            return ERR_INVALID_INDEX;
        }
    }

    return S_ATC_SUCCESS;
}
/*****************************************************************************/
//  Description : This function handle the APP_MN_SPLIT_MPTY_CNF signal
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
ATC_STATUS  ATC_ProcessSplitMptyCnf(
    ATC_CONFIG_T            *atc_config_ptr,
    APP_MN_SPLIT_MPTY_CNF_T  *sig_ptr
)
{
    uint8   call_index;
    int32   i;

    ATC_TRACE_LOW("ATC: receive the APP_MN_SPLIT_MPTY_CNF signal");

    if(sig_ptr->req_is_accepted)
    {
        call_index = GetIndexOfCallContext(sig_ptr->call_id, sig_ptr->dual_sys);

        SCI_ASSERT(ATC_INVALID_CALLID != call_index);/*assert verified: check valid value*/

        g_calls_state[sig_ptr->dual_sys].call_context[call_index].mpty_state = ATC_MPTY_IDLE; //lint !e661 !e662
        g_calls_state[sig_ptr->dual_sys].call_context[call_index].hold_state = ATC_HOLD_IDLE; //lint !e661 !e662

        for(i = 0; i < g_calls_state[sig_ptr->dual_sys].call_amount; i++)
        {
            if(ATC_CALL_MPTY == g_calls_state[sig_ptr->dual_sys].call_context[i].mpty_state)
            {
                g_calls_state[sig_ptr->dual_sys].call_context[i].hold_state = ATC_CALL_HOLD;
            }
        }

        ATC_UpDSCIInfo(sig_ptr->dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr,  call_index, 1);

        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
    }

    atc_config_ptr->ss_operate[sig_ptr->dual_sys] = ATC_INVALID_OPERATE;
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_RETRIEVED_IND signal and
//                sends the +CSSU: unsolicited result code
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessRetrievedInd(
    ATC_CONFIG_T                *atc_config_ptr,
    APP_MN_CALL_RETRIEVED_IND_T *sig_ptr
)
{
    SCI_ASSERT((PNULL != atc_config_ptr) && (PNULL != sig_ptr));/*assert verified: check null pointer*/

    if(atc_config_ptr->cssu_flag[sig_ptr->dual_sys])
    {
        sprintf((char *)g_rsp_str, "+CSSU:3");
        ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
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
ATC_STATUS ATC_ProcessBuildMptyInd(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_BUILD_MPTY_IND_T  *sig_ptr
)
{
    SCI_ASSERT((PNULL != atc_config_ptr) && (PNULL != sig_ptr));/*assert verified: check null pointer*/

    if(atc_config_ptr->cssu_flag[sig_ptr->dual_sys])
    {
        sprintf((char *)g_rsp_str, "+CSSU:4");
        ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);

        return S_ATC_SUCCESS;
    }

    return S_ATC_DEFAULT_HANDLE;
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handle the SS request through ATD command
//  Global resource dependence : none
//  Author:       shijun.cui
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_CheckSSStr(
    MN_DUAL_SYS_E   dual_sys,
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

    SCI_ASSERT(str_ptr != NULL);/*assert verified: check null pointer*/

    ATC_TRACE_LOW("ATC: enter ATC_CheckSSStr!");
    ATC_TRACE_LOW("ATC: length = %d, str_ptr = %s", length, str_ptr);

    SCI_MEMSET(&ss_info, 0, sizeof(ss_info));

    operation = MNPHONE_ParseBStringEx(dual_sys, str_ptr, length, &ss_info);

    ATC_TRACE_LOW("ATC: MNPHONE_ParseBString operation = %d!", operation);

    if(MN_SS_OPERATION == operation)
    {
        old_passwd_ptr = ss_info.ss_parse_info.ss_info_u.ss_pwd_info.old_pwd;
        new_passwd_ptr = ss_info.ss_parse_info.ss_info_u.ss_pwd_info.new_pwd;
        new_passwd_again_ptr = ss_info.ss_parse_info.ss_info_u.ss_pwd_info.new_pwd_two;

        SCI_MEMCPY(s_old_password, old_passwd_ptr, AT_CLCK_PSW_LEN);
        SCI_MEMCPY(s_new_password, new_passwd_ptr, AT_CLCK_PSW_LEN);
        SCI_MEMCPY(s_new_password_again, new_passwd_again_ptr, AT_CLCK_PSW_LEN);

        SCI_MEMCPY(s_clck_psw, old_passwd_ptr, AT_CLCK_PSW_LEN);

        return ATC_SendReqToSs(atc_config_ptr, &ss_info, dual_sys);
    }
    else if(MN_USSD_OPERATION == operation)
    {
        //Send the original request to MN.
        ATC_TRACE_LOW("ATC: begain MNSS_UserOriginateUSSD");

        if(FALSE == s_cusd_isresponse[dual_sys])
        {
            result = MNSS_UserOriginateUSSDEx(dual_sys, FALSE, MN_SS_DEFAULT_DCS_VALUE, length, str_ptr);

            if(ERR_MNSS_NO_ERR != result)
            {
                ATC_TRACE_LOW("ATC: MNSS_UserOriginateUSSD result = %d", result);
                return ERR_PHONE_FAILURE;
            }
            else
            {
                s_cusd_isresponse[dual_sys] = TRUE;
                s_cusd_presentation[dual_sys] = AT_CUSD_ENABLE;
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            }
        }
        else
        {
            //Send the response to MN.
            ATC_TRACE_LOW("ATC: begain MNSS_ResponseUSSD");
            result = MNSS_ResponseUSSDEx(dual_sys, MN_SS_DEFAULT_DCS_VALUE, length, str_ptr);

            if(ERR_MNSS_NO_ERR != result)
            {
                ATC_TRACE_LOW("ATC: MNSS_ResponseUSSD result = %d", result);
                return ERR_PHONE_FAILURE;
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            }
        }

        return S_ATC_SUCCESS;
    }
    else if(MN_UNKONOWN_OPERATION == operation)
    {
        ATC_TRACE_LOW("ATC: operation = %d, unknown opertaion, return error to AP!", operation);
        return ERR_OPERATION_NOT_SUPPORTED;
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
    int16 cmd_id = 0;

    switch(ss_code)
    {
    #ifndef ATC_SAT_ENABLE
        case MN_SS_CLIP:
            cmd_id = AT_CMD_CLIP;
            break;

        case MN_SS_CLIR:
            cmd_id = AT_CMD_CLIR;
            break;

        case MN_SS_COLP:
            cmd_id = AT_CMD_COLP;
            break;

        case MN_SS_COLR:
            cmd_id = AT_CMD_COLR;
            break;
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
            cmd_id = AT_CMD_CCFC;
            break;

        case MN_SS_CW:
            cmd_id = AT_CMD_CCWA;
            break;
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
            cmd_id = AT_CMD_CLCK;
            break;
        default:
            ATC_TRACE_LOW("ATC: unknow ss code = %d", ss_code);
            break;
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
    MN_PARSE_INFO_U           *ss_info_ptr,
    MN_DUAL_SYS_E   dual_sys
)
{
    ERR_MNSS_CODE_E         err_code = ERR_MNSS_NO_ERR;
    MN_SS_CODE_E            ss_code;
    MN_SS_OPERATION_CODE_E  ss_oper = MN_SS_REGISTERSS;
    MN_SS_BASIC_SERVICE_T   basic_service;
    MN_SS_REGISTER_PARAM_U  register_param;
    ATC_STATUS              status = S_ATC_SUCCESS;

    ATC_TRACE_LOW("ATC: enter ATC_SendReqToSs", ss_oper);

    ss_oper = ss_info_ptr->ss_parse_info.ss_oper;
    ss_code = ss_info_ptr->ss_parse_info.ss_code;
    basic_service = ss_info_ptr->ss_parse_info.ss_info_u.ss_basic_info.basic_service;
    register_param = ss_info_ptr->ss_parse_info.ss_info_u.ss_basic_info.register_param;

    ATC_TRACE_LOW("ATC: ss_oper = %d, ss_code = %d, basic_service_code = %d, basic_service_type = %d, register_param_Nbr_user = %d",
                  ss_oper, ss_code, basic_service.service_code, basic_service.service_type, register_param.Nbr_user);

    switch(ss_oper)
    {
        case MN_SS_REGISTERSS:
            //register call forwarding
            err_code = MNSS_RegisterSSInfoEx(dual_sys, ss_code, basic_service, register_param);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_REGISTER_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = RetrieveCmdId(ss_code);
                s_ss_context[dual_sys].ss_code = ss_code;
            }
            break;

        case MN_SS_ERASESS:
            //disable call forwarding
            err_code = MNSS_EraseSSInfoEx(dual_sys, ss_code, basic_service);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_ERASE_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = RetrieveCmdId(ss_code);
                s_ss_context[dual_sys].ss_code = ss_code;
            }
            break;

        case MN_SS_ACTIVATESS:
            //activate call waiting and bar
            err_code = MNSS_ActivateSSEx(dual_sys, ss_code, basic_service);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_ACTIVATE_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = RetrieveCmdId(ss_code);
                s_ss_context[dual_sys].ss_code = ss_code;
            }
            break;

        case MN_SS_DEACTIVATESS:
            //deactivate call waiting and bar
            err_code = MNSS_DeactivateSSEx(dual_sys, ss_code, basic_service);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_DEACTIVATE_SS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = RetrieveCmdId(ss_code);
                s_ss_context[dual_sys].ss_code = ss_code;
            }
            break;

        case MN_SS_INTERROGATESS:
            //interrogate status
            err_code = MNSS_InterrogateServiceEx(dual_sys, ss_code, basic_service);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_INTERROGATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = RetrieveCmdId(ss_code);
                s_ss_context[dual_sys].ss_code = ss_code;
            }
            break;

        case MN_SS_REGISTERPASSWORD:
            ss_code = (MN_SS_CODE_E)ss_info_ptr->ss_parse_info.ss_info_u.ss_pwd_info.code;
            ATC_TRACE_LOW("ATC: registerpassword ss_code = %d", ss_code);
            err_code = MNSS_RegisterPasswordEx(dual_sys, ss_code);
            if(err_code == ERR_MNSS_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_REGISTER_PASSWORD_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                s_ss_context[dual_sys].cmd_id = AT_CMD_CPWD;
                s_ss_context[dual_sys].ss_code = ss_code;
            }
            break;

        default:
            ATC_TRACE_LOW("ATC: SendReqToSs in default!");
            status = ERR_PHONE_FAILURE;
            break;
    }

    if (err_code == ERR_MNSS_FDN_FAILED)
    {
        ATC_TRACE_LOW("ATC: ATC_SendReqToSs, FDN check failed");
        status = ERR_SIM_FDN_FAILED;
    }
    else
    {
        if (err_code != ERR_MNSS_NO_ERR)
        {
            ATC_TRACE_LOW("ATC: ATC_SendReqToSs, failure, ss_oper = %d,  err_code= %d", ss_oper, err_code);
            status = ERR_OPERATION_NOT_ALLOWED;
        }
    }       

    return status;
}
#endif /* _ULTRA_LOW_CODE_ */

/*****************************************************************************/
//  Description : get service class from MN_SS_INTERROGATE_INFO_T(from MN)
//  Global resource dependence : none
//  Author:
//  Note: NA
/*****************************************************************************/
LOCAL uint16 ATC_getclass_from_interrogate_cnf(
    MN_SS_INTERROGATE_INFO_T *interrogate_info,
    int list_num
)
{
    uint16              service_class = 0;
    int i;

    for(i = 0; i < list_num; i++)
    {
        if(interrogate_info[i].ss_status_present)
        {
            if((interrogate_info[i].ss_status != MN_SS_PROVITION) &&
                    (interrogate_info[i].ss_status != MN_SS_ACTIVE_OPERATIVE) &&
                    (interrogate_info[i].ss_status != MN_SS_ACITVE_QUIESCENT))
            {
                continue;
            }
        }

        if(MN_SS_ALL_SERVICE == interrogate_info[i].basic_service.service_type)
        {
            service_class |= 253;
        }

        if(MN_SS_TELE_SERVICE == interrogate_info[i].basic_service.service_type)
        {
            if(MN_SS_ALL_TELESERVICES == interrogate_info[i].basic_service.service_code)
            {
                service_class |= (1 + 4 + 8);
            }
            else if(MN_SS_ALL_SPEECH_TRANS_SERVICES == interrogate_info[i].basic_service.service_code ||
                    MN_SS_TELEPHONE == interrogate_info[i].basic_service.service_code)
            {
                service_class |= 1;
            }
            else if(MN_SS_ALL_SHORT_MSG_SERVICE == interrogate_info[i].basic_service.service_code ||
                    MN_SS_SHORT_MSG_MTPP == interrogate_info[i].basic_service.service_code)
            {
                service_class |= 8;
            }
            else if(MN_SS_ALL_FACSIMIL_TRANS_SERVICES == interrogate_info[i].basic_service.service_code ||
                    MN_SS_AUTO_FACSIMIL3 == interrogate_info[i].basic_service.service_code   ||
                    MN_SS_FACSIMIL4 == interrogate_info[i].basic_service.service_code)
            {
                service_class |= 4;
            }
        }
        else if(MN_SS_BEARER_SERVICE == interrogate_info[i].basic_service.service_type)
        {
            if(MN_SS_ALL_BEARER_SERVICES == interrogate_info[i].basic_service.service_code)
            {
                service_class |= (16 + 32 + 64 + 128);
            }
            else if(MN_SS_ALL_DATADCS_SERVICES == interrogate_info[i].basic_service.service_code ||
                    MN_SS_ALL_DATA_CIRCUIT_SYNC == interrogate_info[i].basic_service.service_code)
            {
                service_class |= 16;
            }
            else if(MN_SS_ALL_DATADCA_SERVICES == interrogate_info[i].basic_service.service_code ||
                    MN_SS_ALL_DATA_CIRCUIT_ANSY == interrogate_info[i].basic_service.service_code)
            {
                service_class |= 32;
            }
            else if(MN_SS_ALL_PADACCESSCA_SERVICES == interrogate_info[i].basic_service.service_code)
            {
                service_class |= 128;
            }
            else if(MN_SS_ALL_DATAPDS_SERVICES == interrogate_info[i].basic_service.service_code)
            {
                service_class |= 64;
            }

        }
    }

    ATC_TRACE_LOW("ATC_getclass_from_interrogate_cnf:service_class=%d", service_class);
    return service_class;
}

/*****************************************************************************/
//  Description : get service class from MN_SS_INTERROGATE_INFO_T(from MN)
//                the status of the service is not necessarily active. 
//  Global resource dependence : none
//  Author: Haoming.Lu
//  Note: NA
/*****************************************************************************/
LOCAL uint16 ATC_getclass_from_info_list(
    MN_SS_INTERROGATE_INFO_T *interrogate_info
)
{
    uint16              service_class = 0;
    if(interrogate_info == NULL)
        return service_class;

    if(MN_SS_ALL_SERVICE == interrogate_info->basic_service.service_type)
    {
        service_class |= 253;
    }

    if(MN_SS_TELE_SERVICE == interrogate_info->basic_service.service_type)
    {
        if(MN_SS_ALL_TELESERVICES == interrogate_info->basic_service.service_code)
        {
            service_class |= (1 + 4 + 8);
        }
        else if(MN_SS_ALL_SPEECH_TRANS_SERVICES == interrogate_info->basic_service.service_code ||
                MN_SS_TELEPHONE == interrogate_info->basic_service.service_code)
        {
            service_class |= 1;
        }
        else if(MN_SS_ALL_SHORT_MSG_SERVICE == interrogate_info->basic_service.service_code ||
                MN_SS_SHORT_MSG_MTPP == interrogate_info->basic_service.service_code)
        {
            service_class |= 8;
        }
        else if(MN_SS_ALL_FACSIMIL_TRANS_SERVICES == interrogate_info->basic_service.service_code ||
                MN_SS_AUTO_FACSIMIL3 == interrogate_info->basic_service.service_code   ||
                MN_SS_FACSIMIL4 == interrogate_info->basic_service.service_code)
        {
            service_class |= 4;
        }
    }
    else if(MN_SS_BEARER_SERVICE == interrogate_info->basic_service.service_type)
    {
        if(MN_SS_ALL_BEARER_SERVICES == interrogate_info->basic_service.service_code)
        {
            service_class |= (16 + 32 + 64 + 128);
        }
        else if(MN_SS_ALL_DATADCS_SERVICES == interrogate_info->basic_service.service_code ||
                MN_SS_ALL_DATA_CIRCUIT_SYNC == interrogate_info->basic_service.service_code)
        {
            service_class |= 16;
        }
        else if(MN_SS_ALL_DATADCA_SERVICES == interrogate_info->basic_service.service_code ||
                MN_SS_ALL_DATA_CIRCUIT_ANSY == interrogate_info->basic_service.service_code)
        {
            service_class |= 32;
        }
        else if(MN_SS_ALL_PADACCESSCA_SERVICES == interrogate_info->basic_service.service_code)
        {
            service_class |= 128;
        }
        else if(MN_SS_ALL_DATAPDS_SERVICES == interrogate_info->basic_service.service_code)
        {
            service_class |= 64;
        }

    }    


    ATC_TRACE_LOW("ATC_getclass_from_interrogate_cnf:service_class=%d", service_class);
    return service_class;
}

/*****************************************************************************/
//  Description : Convert service class to MN basic service
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_class_to_basicservice(
    uint8 *service_class,
    MN_SS_BASIC_SERVICE_T *basic_service,
    BOOLEAN is_query_mode
)
{
    uint8 temp;

    uint16 tele_service_code_tbl[] =
    {
        /*1*/       MN_SS_ALL_SPEECH_TRANS_SERVICES,
        /*2*/       0,
        /*3*/       0,
        /*4*/       MN_SS_ALL_FACSIMIL_TRANS_SERVICES,
        /*5*/       MN_SS_TELESERVICES_EXP_SMS,
        /*6*/       0,
        /*7*/       0,
        /*8*/       MN_SS_ALL_SHORT_MSG_SERVICE,
        /*9*/       MN_SS_ALL_TELESERVICES,
        /*10*/  0,
        /*11*/  0,
        /*12*/  MN_SS_ALL_DATA_TELESERVICES,
        /*13*/  MN_SS_ALL_TELESERVICES
    };

    uint16 tele_service_code_flag = 1 + 8 + 16 + 128 + 0x1000;
    uint16 bearer_service_code_tbl[] =
    {
        /*1*/       MN_SS_ALL_DATA_CIRCUIT_SYNC,
        /*2*/       MN_SS_ALL_DATA_CIRCUIT_ANSY,
        /*3*/       MN_SS_ALL_BEARER_SERVICES,
        /*4*/       MN_SS_ALL_DATAPDS_SERVICES,
        /*5*/       MN_SS_ALL_SYNC_SERVICES,
        /*6*/       MN_SS_ALL_BEARER_SERVICES,
        /*7*/       MN_SS_ALL_BEARER_SERVICES,
        /*8*/       MN_SS_ALL_PADACCESSCA_SERVICES,
        /*9*/   MN_SS_ALL_BEARER_SERVICES,
        /*10*/  MN_SS_ALL_ANSY_SERVICES,
        /*11*/  MN_SS_ALL_BEARER_SERVICES,
        /*12*/  MN_SS_ALL_BEARER_SERVICES,
        /*13*/  MN_SS_ALL_BEARER_SERVICES,
        /*14*/  MN_SS_ALL_BEARER_SERVICES,
        /*15*/  MN_SS_ALL_BEARER_SERVICES
    };

    uint16 bearer_service_code_flag = 1 + 2 + 8 + 16 + 128 + 512 + 0x4000;

    if(service_class == NULL || basic_service == NULL)
    {
        return FALSE;
    }

    temp = *service_class;

    if(0 == temp)
    {
        return FALSE;
    }
    
    if(temp & 0x02)
    {
        temp |= 0xF0;
        temp &= 0xFD;
    }

    if(!is_query_mode)
    {
        if(temp == 253 || temp == 245)
        {
            basic_service->service_type = MN_SS_ALL_SERVICE;
            basic_service->service_code = 0;
        }
        else if(((temp & 0xF0) > 0) && ((temp & 0x0F) > 0))
        {
            return FALSE;
        }
        else if((temp & 0x0F) > 0)
        {
            temp &= 0x0F;
            temp --;

            if(!(tele_service_code_flag & (1 << temp)))
            {
                return FALSE;
            }

            basic_service->service_type = MN_SS_TELE_SERVICE;
            basic_service->service_code = tele_service_code_tbl[temp];
        }
        else
        {
            temp = (temp >> 4) & 0x0F;
            temp --;

            if(!(bearer_service_code_flag & (1 << temp)))
            {
                return FALSE;
            }

            basic_service->service_type = MN_SS_BEARER_SERVICE;
            basic_service->service_code = bearer_service_code_tbl[temp];
        }

    }
    else
    {
        *service_class = temp;

        if(((temp & 0xF0) > 0) && ((temp & 0x0F) > 0))
        {
            basic_service->service_type = MN_SS_ALL_SERVICE;
            basic_service->service_code = 0;
        }
        else if((temp & 0x0F) > 0)
        {
            temp &= 0x0F;
            temp --;
            basic_service->service_type = MN_SS_TELE_SERVICE;
            basic_service->service_code = tele_service_code_tbl[temp];
        }
        else
        {
            temp = (temp >> 4) & 0x0F;
            temp --;
            
            basic_service->service_type = MN_SS_BEARER_SERVICE;
            basic_service->service_code = bearer_service_code_tbl[temp];
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : interrogate call forward service
//  Global resource dependence : none
//  Author: Pizer.Fan
//  Note: NA
/*****************************************************************************/
BOOLEAN ATC_InterrogateCFService(
    MN_SS_CODE_E ss_code,
    MN_DUAL_SYS_E   dual_sys
)
{
    MN_SS_BASIC_SERVICE_T basic_service;
    ERR_MNSS_CODE_E err_code = ERR_MNSS_NO_ERR;

    if((MN_SS_ALL_FORWARD != ss_code) &&
            (MN_SS_CFU != ss_code) &&
            (MN_SS_ALL_COND_FORWARD != ss_code) &&
            (MN_SS_CFB != ss_code) &&
            (MN_SS_CFNRY != ss_code) &&
            (MN_SS_CFNRC != ss_code))
    {
        ATC_TRACE_LOW("ATC: ATC_InterrogateCFService, NOT Call Forward Code (%d)!", ss_code);
        return FALSE;
    }

    basic_service.service_type = MN_SS_TELE_SERVICE;
    basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;

    err_code = MNSS_InterrogateServiceEx(dual_sys, MN_SS_CFU, basic_service);
    if(ERR_MNSS_NO_ERR != err_code)
    {
        ATC_TRACE_LOW("ATC: ATC_InterrogateCFService, failure in MN (ss_code:%d)", ss_code);
       if (err_code == ERR_MNSS_FDN_FAILED)
       {
           ATC_TRACE_LOW("ATC: ATC_InterrogateCFService, FDN check failed");
       }

        return FALSE;
    }

    if((int16)AT_INVALID_CMD_ID != s_ss_context[dual_sys].cmd_id)
    {
        ATC_TRACE_LOW("ATC: ATC_InterrogateCFService, WARNING! s_ss_context(%d,%d) is already existed.",
                      s_ss_context[dual_sys].cmd_id, s_ss_context[dual_sys].ss_code);
    }

    s_ss_context[dual_sys].cmd_id = AT_CMD_CCFC;
    s_ss_context[dual_sys].ss_code = ss_code;

    return TRUE;
}

/*****************************************************************************/
//  Description : This function restore the gsm ss configs
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
void ATC_RestoreGsmSSConfig(MN_DUAL_SYS_E  dual_sys)
{
    //set +CUSD(<n>) default value when AT&F
    s_cusd_presentation[dual_sys] = AT_CUSD_DISABLE;
}

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function handles the APP_MN_FDN_SERVICE_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Elvis Xu
//  Note
/*****************************************************************************/
ATC_STATUS ATC_ProcessFDNServiceInd(
    ATC_CONFIG_T                   *atc_config_ptr,
    APP_MN_FDN_SERVICE_IND_T      *sig_ptr
)
{
    MN_DUAL_SYS_E   dual_sys = sig_ptr->dual_sys;
    
    ATC_TRACE_LOW("ATC: arrive at ATC_TASK APP_MN_FDN_SERVICE_IND is_fdn_enable=%d", sig_ptr->is_fdn_enable);
    atc_config_ptr->is_fdn_enable[dual_sys] = sig_ptr->is_fdn_enable;

    if(atc_config_ptr->is_fdn_init[dual_sys])
    {
        sprintf((char *)g_rsp_str, "+CLCK: %d", sig_ptr->is_fdn_enable);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        atc_config_ptr->is_fdn_init[dual_sys] = FALSE;
    }

    if ((atc_config_ptr->operate[dual_sys] == ATC_PIN_ENABLE_FDN) || (atc_config_ptr->operate[dual_sys] == ATC_PIN_DISABLE_FDN))
    {
        atc_config_ptr->is_fdn_enable[dual_sys] = sig_ptr->is_fdn_enable;
        if (!sig_ptr->is_fdn_exist)
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_SIM_FDN_FAILED);
        }
        else
        {
            if (((atc_config_ptr->operate[dual_sys] == ATC_PIN_ENABLE_FDN) && (atc_config_ptr->is_fdn_enable[dual_sys])) ||
                 ((atc_config_ptr->operate[dual_sys] == ATC_PIN_DISABLE_FDN) && (!atc_config_ptr->is_fdn_enable[dual_sys])))
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_UNKNOWN);
            }
        }

        atc_config_ptr->cpin2[dual_sys] = ATC_CPIN_CODE_READY;
        atc_config_ptr->operate[dual_sys] = ATC_PIN_NO_OPERATE;
    }

    return S_ATC_SUCCESS;
}
#endif

/*****************************************************************************/
//  Description : This function handle the AT+SCCBS command. This command is to 
//                  interrogate or deactivate the ccbs service
//  command format:
//                AT+SCCBS=<operate mode>[,<ccbs index>]
//  Global resource dependence : none
//  Author:       Haoming Lu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSCCBS)
{
#ifdef _SUPPORT_CCBS_
    uint8       operation_mode; // holds the operation mode
    uint8 ccbs_index;
    ERR_MNSS_CODE_E err_mnss_code = ERR_MNSS_NO_ERR;
    MN_SS_CODE_E               ss_code = MN_SS_CCBS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    uint8                    service_class = 255;
    MN_SS_BASIC_SERVICE_T      basic_service;

    if(ATC_CMD_TYPE_TEST == ATC_GET_CMD_TYPE)
    {
        sprintf((char *)g_rsp_str, "%s: (0-1)", g_atc_info_table[AT_CMD_CCFC].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
        return S_ATC_SUCCESS;
    }

    SCI_ASSERT(ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE);/*assert verified: check valid value*/

    // check if the parameter1 are present or not.
    if(!PARAM1_FLAG)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessSCCBS,  param 1 does not exist!");
        return ERR_INVALID_INDEX;
    }
    else
    {
        // get the CCBS operation mode
        operation_mode = PARA1.num;
        if(0 == operation_mode)
        {
            operation_mode = ATC_DISABLE;
        }
        else if(1 == operation_mode)
        {
            operation_mode = ATC_QUERY_STATUS;
        }
        else
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessSCCBS,  param 1 wrong values!");
            return ERR_OPERATION_NOT_SUPPORTED;
        }
    }

    switch(operation_mode)
    {
        case ATC_DISABLE:
            {
                if(!PARAM2_FLAG)//erase all the ccbs activated
                {
                    err_mnss_code = MNSS_EraseCcbsEx(dual_sys, TRUE, NULL);
                }
                else  //erase one ccbs activated
                {
                    ccbs_index = (uint8)PARA2.num;
                    if(ccbs_index > 5 || 0 == ccbs_index)
                    {
                        ATC_TRACE_LOW("ATC: ATC_ProcessSCCBS, ccbs_index is out of range (1-5)!");
                        return ERR_INVALID_INDEX;
                    }
                    else
                    {
                        err_mnss_code = MNSS_EraseCcbsEx(dual_sys, FALSE, ccbs_index);
                    }
                }
                
                if(err_mnss_code != ERR_MNSS_NO_ERR)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSCCBS, MNSS_EraseCcbsEx() returns err!");
                    return ERR_UNKNOWN;
                }
                else
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSCCBS, MNSS_EraseCcbsEx() SUCCEED!");
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_CCBS_ERASE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                    s_ss_context[dual_sys].cmd_id = AT_CMD_SCCBS;
                    s_ss_context[dual_sys].ss_code = ss_code;
                }
                break;
            }
        case ATC_QUERY_STATUS:
            {
                //here we ignore whether param2 exists
                service_class = 7;
                if(!ATC_class_to_basicservice(&service_class, &basic_service, TRUE))
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSCCBS, Class not allowed");
                    return ERR_OPERATION_NOT_ALLOWED;
                }
                err_mnss_code = MNSS_InterrogateServiceEx(dual_sys, MN_SS_CCBS, basic_service);
                if(err_mnss_code != ERR_MNSS_NO_ERR)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSCCBS, MNSS_InterrogateServiceEx() returns err!");
                    return ERR_UNKNOWN;
                }
                else
                {
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                      APP_MN_INTERROGATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                    s_ss_context[dual_sys].cmd_id = AT_CMD_SCCBS;
                    s_ss_context[dual_sys].ss_code = ss_code;
                }
                break;
            }
        default:
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessSCCBS,  param 1 wrong values!");
                return ERR_OPERATION_NOT_SUPPORTED;
            }
    }
#endif // #ifdef _SUPPORT_CCBS_    
    return(S_ATC_SUCCESS);

}

#ifdef _SUPPORT_CCBS_
/*****************************************************************************/
//  Description : This function handle APP_MN_CCBS_ACTIVATE_IND message
//  Global resource dependence :
//  Author:     Haoming Lu
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessCCBSActInd(
    ATC_CONFIG_T * atc_config_ptr,
    APP_MN_CCBS_ACTIVATE_IND_T *sig_ptr
)
{
    if (MN_SS_OPERATION_SUCC == sig_ptr->result_type)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessCCBSActInd, act ccbs failed!");
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle APP_MN_CCBS_RECALL_IND message
//  Global resource dependence :
//  Author:     Elivs Xu
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessCCBSRecallInd(
    ATC_CONFIG_T * atc_config_ptr,
    APP_MN_CCBS_RECALL_IND_T *sig_ptr
)
{
    atc_config_ptr->ccbs_info[sig_ptr->dual_sys].cur_call_id = sig_ptr->call_id;
    atc_config_ptr->ccbs_info[sig_ptr->dual_sys].cur_ccbs_index = sig_ptr->ccbs_index;
    g_atc_call_flag = TRUE;

    if (sig_ptr->remote_num_present)
    {
        ATC_TELE_TYPE_E tel_num_type = UNKNOWN_PLAN_UNKNOWN_TYPE;
        uint8 tel_num_str[2*MN_MAX_ADDR_BCD_LEN + 1] = {0};
        uint16 tel_num_len = 0;

        ATC_ConvertCalledNumToStr(&sig_ptr->remote_num, &tel_num_type, tel_num_str, &tel_num_len);
        tel_num_str[tel_num_len] = '\0';
        sprintf((char *)g_rsp_str, "+CCBS: 2,%d,%d,%s,%d", sig_ptr->call_id, sig_ptr->ccbs_index, tel_num_str, tel_num_type);
    }
    else
    {
        sprintf((char *)g_rsp_str, "+CCBS: 2,%d,%d,\"\",129", sig_ptr->call_id, sig_ptr->ccbs_index);
    }

    ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle APP_MN_CCBS_ERASE_CNF message
//  Global resource dependence :
//  Author:     Haoming Lu
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessCCBSEraseCnf(
    ATC_CONFIG_T * atc_config_ptr,
    APP_MN_CCBS_ERASE_CNF_T *sig_ptr
)
{
    if(MN_SS_OPERATION_SUCC == sig_ptr->result_type)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
    }
    else
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessCCBSEraseCnf, erase ccbs failed!");
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);    
    }
 
    s_ss_context[sig_ptr->dual_sys].cmd_id = (int16)AT_INVALID_CMD_ID;
    s_ss_context[sig_ptr->dual_sys].ss_code = (MN_SS_CODE_E)0; 

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle at cmd about ccbs ind to accept or reject the ccbs
//  Global resource dependence :
//  Author:     Haoming Lu
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_HandleCCBSInd(
                                            ATC_CONFIG_T* atc_config_ptr,
                                            MN_DUAL_SYS_E   dual_sys,
                                            BOOLEAN is_ccbs_accept)
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    uint8 call_index;
    
    uint8 call_id = atc_config_ptr->ccbs_info[dual_sys].cur_call_id;
                                
    call_index = GetIndexOfCallContext(call_id, dual_sys);
    if (ATC_INVALID_CALLID == call_index)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessCHLD invalid call index!");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(is_ccbs_accept)
    {
        if(MN_RETURN_SUCCESS == MNCALL_DisconnectCallWithCCBSEx(dual_sys, call_id, MN_CAUSE_NORMAL_CLEARING, TRUE, PNULL))
        {
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
            (uint16)APP_MN_CCBS_ACTIVATE_IND, 0xff, ATC_NO_DOMAIN, dual_sys);
        }
        else
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessCHLD set ccbs wrong!");
            status = ERR_UNKNOWN;
        }
    }
    else
    {
        if(MN_RETURN_SUCCESS == MNCALL_DisconnectCallWithCCBSEx(dual_sys, call_id, MN_CAUSE_NORMAL_CLEARING, FALSE, PNULL))
        {
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
            (uint16)APP_MN_CALL_DISCONNECTED_IND, 0xff, ATC_DOMAIN_CS, dual_sys);
        }
        else
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessCHLD set ccbs wrong!");
            status = ERR_UNKNOWN;
        }
    }

    return status;
}



/*****************************************************************************/
//  Description : This function handle at cmd about recall ind
//  Global resource dependence :
//  Author:     Haoming Lu
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_HandleRecallIndCMD(
                                            ATC_CONFIG_T* atc_config_ptr,
                                            MN_DUAL_SYS_E   dual_sys,
                                            BOOLEAN is_recall_accept)
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    
    if((atc_config_ptr->ccbs_info[dual_sys].cur_ccbs_index < ATC_MAX_CCBS_NUM + 1)
        &&(atc_config_ptr->ccbs_info[dual_sys].cur_ccbs_index != 0)
            &&(ATC_INVALID_CALLID != atc_config_ptr->ccbs_info[dual_sys].cur_call_id))
    {
        uint8    call_id;
        uint8 call_index;
        uint8 ccbs_index = ATC_MAX_CCBS_NUM;
        
        ccbs_index = atc_config_ptr->ccbs_info[dual_sys].cur_ccbs_index;
        call_id = atc_config_ptr->ccbs_info[dual_sys].cur_call_id;
//reset ccbs info
        atc_config_ptr->ccbs_info[dual_sys].cur_ccbs_index = 0xff;
        atc_config_ptr->ccbs_info[dual_sys].cur_call_id = 0xff;

        if(!is_recall_accept)
        {
            if(MN_RETURN_SUCCESS == MNCALL_RecallRspEx(dual_sys, call_id, FALSE, NULL))
            {
                call_index = GetIndexOfCallContext(call_id, dual_sys);

                ATC_TRACE_LOW("ATC: ATC_HandleRecallIndCMD, call_id:%d%s",
                      g_calls_state[dual_sys].call_context[call_index].call_id,
                      (g_calls_state[dual_sys].call_context[call_index].call_id == 0 ? " WARNING! not 0" : ""));
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_HandleRecallIndCMD, MNCALL_RecallRspEx failure!");
                status =  ERR_UNKNOWN;
            }
        }
        else
        {
             ATC_TRACE_LOW("ATC: ATC_HandleRecallIndCMD, ERROR AT CMD!");
             status =  ERR_UNKNOWN;
        }
    }
    else
    {
        ATC_TRACE_LOW("ATC: ATC_HandleRecallIndCMD, NO VALID CCBS call!");
        status =  ERR_UNKNOWN;
    }
    return status;
   
}

#endif // #ifdef _SUPPORT_CCBS_



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
