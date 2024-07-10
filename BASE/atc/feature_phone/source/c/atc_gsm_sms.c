/******************************************************************************
 ** File Name:      atc_gsm_sms.c                                             *
 ** Author:         Xueli.lv                                                  *
 ** Date:           24/09/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains the functions that handle the AT       *
 **                 command for SMS(short message service)                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 24/09/2002     Xueli.lv         Create.                                   *
 ******************************************************************************/
#include "base_trc.h"
#include "atc_gsm_sms.h"
#include "atc.h"
#include "atc_common.h"
#include "sci_array.h"
#include "sig_code.h"

/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/
#ifdef _MUX_ENABLE_
BOOLEAN               g_atc_sms_flag = FALSE;
BOOLEAN               g_ignore_sms_result = FALSE;
#else
extern  MN_DUAL_SYS_E g_current_card_id;
#endif

#if !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_SYNC_TOOL_) ||defined(_ATC_UIX8910_ENABLE_)
static ATC_SMS_CONFIG_T      s_sms_config[MN_SYS_NUMBER] = {0};
static ATC_SMS_RECORD_T      s_sms_record[MN_SYS_NUMBER] = {0};
static ATC_SMS_NV_CONFIG_T   s_sms_nv_config;

#ifdef ATC_ORANGE_ENABLE
    static const char* s_sms_mem_str[] = {"ME", "SM","SR"};
#else
    static const char* s_sms_mem_str[] = {"BM", "SM","SR"};
#endif

static const char* s_sms_state_str[] =
            {"REC UNREAD",
             "REC READ",
             "STO UNSENT",
             "STO SENT",
             "ALL",
             "MAX STATUS"
            };

MN_CALLED_NUMBER_T           g_sc_addr[MN_SYS_NUMBER] = {0};

extern RSP_RESULT_STR        g_rsp_str;
extern const ATC_INFO_T      g_atc_info_table[];
#endif

static uint8 gCMMS = 0;

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
//used to serve for AT+SFSMSM
typedef struct _SMS_FILL_MEM_T{
    MN_L3_MSG_UNIT_T * bin_pdu_ptr;
    MN_SMS_MT_SMS_T * mt_sms_ptr;
    MN_SMS_MO_SMS_T * mo_sms_ptr;
    MN_SMS_ADDR_T *  src_addr_ptr;
    MN_SMS_STATUS_E  sms_status;
    MN_SMS_MEM_FULL_STATUS_E me_mem_sts;    //to record ME memory full or not
    MN_SMS_MEM_FULL_STATUS_E *sim_mem_sts;  //to record each SIM's memory full or not
}SMS_FILL_MEM_T;

static MN_SMS_MEM_FULL_STATUS_E s_sim_mem_sts[MN_SYS_NUMBER]=
{
#if defined MULTI_SIM_SYS_DUAL
    MN_SMS_SIM_FULL,
#elif defined MULTI_SIM_SYS_TRI
    MN_SMS_SIM_FULL,
    MN_SMS_SIM_FULL,
#elif defined MULTI_SIM_SYS_QUAD
    MN_SMS_SIM_FULL,
    MN_SMS_SIM_FULL,
    MN_SMS_SIM_FULL,
#endif
    MN_SMS_SIM_FULL
};

static SMS_FILL_MEM_T s_sms_fill_mem = {PNULL, PNULL, PNULL, PNULL, MN_SMS_FREE_SPACE, MN_SMS_ME_FULL, s_sim_mem_sts};

/*****************************************************************************/
//  Description : This function is used handle the cb parameters
//                sms status.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleCbParam(
                BOOLEAN                        mode,         // indicate accept or reject the below parameters
                BOOLEAN                        msg_present,
                uint16                         msg_id,       // specific message identity
                BOOLEAN                        lang_present,
                MN_SMSCB_LANGUAGE_ID_E         lang_id,      // specific language identity
                MN_CB_MSG_ID_MUSTER_T          *msg_ptr,
                MN_CB_LANGUAGE_ID_MUSTER_T     *lang_ptr
                );

/*****************************************************************************/
//  Description : This function is used change the specific sms status.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  ChangeSmsStatus(
                MN_SMS_STATUS_E      sms_status,
                MN_SMS_STATUS_E      *changed_status_ptr,
                ATC_SMS_STATE_E      at_sms_status,
                BOOLEAN              *sms_present_ptr
                );

/*****************************************************************************/
//  Description : This function is used change the string type status to numberic
//                sms status.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ConvertStrStatusToNumStatus(
                    uint8            *status_ptr,
                    ATC_SMS_STATE_E  *sms_status_ptr
                    );


/*****************************************************************************/
//  Description : This function sends msg to fill sms storage for AT+SFSMSM
//  Global resource dependence :
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Atc_FillMtMoSmsEx(
                               ATC_CONFIG_T  * atc_config_ptr,
                               MN_SMS_STORAGE_E       storage,
                               MN_DUAL_SYS_E            dual_sys);

/*****************************************************************************/
//  Description : This function used to free dynamic memory for AT+SFSMSM
//  Global resource dependence :
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
LOCAL void Atc_FreeMemForSfsmsm(SMS_FILL_MEM_T * sfm_ptr);

/*****************************************************************************/
//  Description : This function used to update SMS memories' status for AT+SFSMSM
//                      and find its free or not
//  Global resource dependence :
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
LOCAL MN_SMS_MEM_FULL_STATUS_E Atc_UpdateSmsMemSts(
                ATC_CONFIG_T         *atc_config_ptr,
                SMS_FILL_MEM_T            *sfm_ptr);

/*****************************************************************************/
//  Description : This function handles the signal of APP_MN_SMS_IND in the
//                Text mode
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleTextModeSmsInd(
                MN_DUAL_SYS_E         dual_sys,
                ATC_CONFIG_T          *atc_config_ptr,
                MN_SMS_MT_SMS_T       *mt_sms_ptr,
                MN_SMS_RECORD_ID_T    record_id
                );

/*****************************************************************************/
//  Description : This function handles the signal of APP_MN_SMS_IND in the
//                PDU mode
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandlePduModeSmsInd(
                MN_DUAL_SYS_E         dual_sys,
                ATC_CONFIG_T          *atc_config_ptr,
                MN_SMS_MT_SMS_T       *mt_sms_ptr,
                MN_SMS_RECORD_ID_T    record_id
                );
#endif

#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_)
/*****************************************************************************/
//  Description : This function handles the text mode SMS
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
#if defined( _MUX_ENABLE_)||defined(_ATC_UIX8910_ENABLE_)
LOCAL BOOLEAN HandleTextModeSms(
             ATC_CONFIG_T  *atc_config_ptr,
             uint8         *pdu_ptr,       // point to the pdu buffer
             uint16         pdu_len
             );
/*****************************************************************************/
//  Description : This function handles the binary format pdu string
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandlePduModeSms(
             ATC_CONFIG_T  *atc_config_ptr,
             uint8         *pdu_ptr,       // point to the pdu buffer
             uint16         pdu_len
             );
#else
/*****************************************************************************/
//  Description : This function handles the text mode SMS
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTextModeSms(
             uint8         *pdu_ptr,       // point to the pdu buffer
             uint16         pdu_len
             );

/*****************************************************************************/
//  Description : This function handles the binary format pdu string
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandlePduModeSms(
             uint8         *pdu_ptr,       // pointer to the pdu buffer
             uint16         pdu_len
             );
#endif/*_MUX_ENABLE_*/

/*****************************************************************************/
//  Description : This function handles the signal of APP_MN_SMS_READ_CNF in the
//                pdu mode
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandlePduModeSmsReadCnf(
                    MN_DUAL_SYS_E       dual_sys,
                    ATC_CONFIG_T        *atc_config_ptr,
                    MN_SMS_U_T          *sms_ptr,
                    MN_SMS_STATUS_E     sms_status,
                    MN_SMS_RECORD_ID_T  record_id
                    );

/*****************************************************************************/
//  Description : This function handles the signal of APP_MN_SMS_READ_CNF in the
//                text mode
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleTextModeSmsReadCnf(
                    MN_DUAL_SYS_E       dual_sys,
                    ATC_CONFIG_T        *atc_config_ptr,
                    MN_SMS_U_T          *sms_ptr,
                    MN_SMS_STATUS_E     sms_status,
                    MN_SMS_RECORD_ID_T  record_id
                    );

/*****************************************************************************/
//  Description : This function get the specific sms information
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  ReadSpecSms(
                MN_DUAL_SYS_E           dual_sys,
                MN_SMS_STORAGE_E        storage,
                MN_SMS_RECORD_ID_T      *index_ptr,
                ATC_SMS_STATE_E         sms_state
                );
#endif
/*****************************************************************************/
//  Description : This function compare the input string and the sm storage
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN   CompareSmStorage(
                        char   *mem_str,          // in: pointer to the storage
                        uint8  *storage_id_ptr   // out: identify the storage id
                        );
#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_)
/*****************************************************************************/
//  Description : This function is used to get the SMS state of string type
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetSmsState(
               uint8                 *state_str,
               ATC_SMS_STATE_E       *sms_state_ptr
               );

/*****************************************************************************/
//  Description : This function is used to handle the response of the at+cmss
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleCmssCmd(
                MN_DUAL_SYS_E            dual_sys,
                APP_MN_READ_SMS_CNF_T    *sig_ptr,
                ATC_CONFIG_T             *atc_config_ptr
                );

/*****************************************************************************/
//  Description : This function is used to format the time stamp string
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void FormatTimeStampStr(
                    uint8                 *ts_str,
                    MN_SMS_TIME_STAMP_T   *time_stamp_ptr
                    );

/*****************************************************************************/
//  Description : get the SMS status
//  Global resource dependence :
//  Author:  Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL ERR_MNSMS_CODE_E GetSmsStatusArr(                                     //RETURN:
                                       uint16            *total_num_ptr,    //OUT:
                                       MN_SMS_STATUS_E   **status_ptr,      //OUT:
                                       MN_DUAL_SYS_E     dual_sys,
                                       MN_SMS_STORAGE_E  storage
                                       );

LOCAL BOOLEAN DeleteSpecSms(
                MN_DUAL_SYS_E           dual_sys,
                MN_SMS_STORAGE_E        storage,
                MN_SMS_RECORD_ID_T      *index_ptr,
                ATC_SMS_DEL_STATE_E     del_state
                );

/*****************************************************************************/
//  Description : This function converts the relative period to the absolute period
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL uint32   GetValidPeriodSeconds(
                uint8      vp
                );

/*****************************************************************************/
//  Description : This function handles the cause from the sms signal
//  Global resource dependence :
//  Author:       Sunsome Ju
//  Note:
/*****************************************************************************/
LOCAL void HandleSmsCause(
                    ATC_CONFIG_T       *atc_config_ptr,
                    MN_SMS_CAUSE_E     cause,
                    MN_SMS_MO_FAIL_CAUSE_E rl_cause
                    );

#endif

/*****************************************************************************/
//  Description : This function handles the AT+CMGL command
//                AT+CMGL=<type>
//             <type>       0         rec unread
//                          1         rec read
//                          2         sto unread
//                          3         sto read
//                          4         all message
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGL )
{
//reduce atc code Nemge
#if defined(_ATC_SYNC_TOOL_) && !defined(_ULTRA_LOW_CODE_) || defined(_ATC_UIX8910_ENABLE_)
    ATC_SMS_STATE_E       atc_status = ATC_SMS_MAX_STATUS;
    MN_SMS_RECORD_ID_T    start_index = 0;
    ATC_STATUS            status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    /* +CMGL=? */
    if(ATC_CMD_TYPE_TEST == ATC_GET_CMD_TYPE)
    {
        if(0 == atc_config_ptr->atc_config_nv.cmgf_flag)
        {
            sprintf((char *)g_rsp_str, "%s: (0-%d)",
                    g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                    (ATC_SMS_MAX_STATUS - 1));
        }
        else
        {
            sprintf((char *)g_rsp_str, "%s: (\"REC UNREAD\",\"REC READ\",\"STO UNSENT\",\"STO SENT\",\"ALL\") ",
                    g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr);
        }

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }

    if (!PARAM1_FLAG)
    {
        // return the result code "operate not allowed"
        goto sms_err_302;
    }

    if((!s_sms_config[dual_sys].sms_is_ready) && (MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem1))
    {
        SCI_TRACE_LOW("ATC: SIM not inserted or ready!");
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }


    // check the sms mode: TRUE: text mode; FALSE: pdu mode
    switch (PARAM1_TYPE)
    {
        // the parameter 1 is a string
        case ATC_CMD_PARAM_TYPE_STRING:
        {
            if (!atc_config_ptr->atc_config_nv.cmgf_flag)
            {
                goto sms_err_302;
            }
            PARA1.str_ptr->str_ptr[PARA1.str_ptr->str_len] = '\0';
            // get the sms state
            if (!GetSmsState(PARA1.str_ptr->str_ptr, &atc_status))
            {
                goto sms_err_302;
            }
            break;
        }

        case ATC_CMD_PARAM_TYPE_NUMERIC:
        {
            if (atc_config_ptr->atc_config_nv.cmgf_flag)
            {
                goto sms_err_302;
            }
            atc_status = PARA1.num;
            if(atc_status >= ATC_SMS_MAX_STATUS)
                return ERR_INVALID_INDEX;
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC:CMGL wrong type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_411_112_2_17_22_17_7_559,(uint8*)"");
            goto sms_err_302;
        }
    }
    s_sms_record[dual_sys].cmgl_idex = 0;

    // Check sms statue and read them
    switch (atc_status)
    {
        case ATC_SMS_UNREAD:
        case ATC_SMS_READ:
        case ATC_SMS_UNSENT:
        case ATC_SMS_SENT:
        case ATC_SMS_ALL:
        {
            if (ReadSpecSms(dual_sys,s_sms_config[dual_sys].mem1, &start_index,atc_status))
            {
                MNSMS_ReadSmsEx(dual_sys, s_sms_config[dual_sys].mem1, start_index);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_READ_SMS_CNF,0xff,ATC_NO_DOMAIN);
            }
            else
            {
                status = S_ATC_SUCCESS;
            }
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC:CMGL wrong type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_440_112_2_17_22_17_7_560,(uint8*)"");
            goto sms_err_302;
        }
    }

    // set the static variables
    s_sms_record[dual_sys].cmd_id    = AT_CMD_CMGL;
    s_sms_record[dual_sys].state     = atc_status;
    return status;

sms_err_302:
    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CSMS command.this command is used
//                to set message service
//                AT+CSMS=<service>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSMS )
{

#if (!defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)) && defined(_ATC_SYNC_TOOL_) || defined(_ATC_UIX8910_ENABLE_)
    ATC_STATUS     status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    SCI_TRACE_LOW("ATC:ATC_ProcessCSMS ATC_GET_CMD_TYPE = %d\n");

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                s_sms_config[dual_sys].service_flag = PARA1.num;
                sprintf((char*)g_rsp_str, "%s: %d,%d,%d",g_atc_info_table[AT_CMD_CSMS].cmd_name->str_ptr, s_sms_config[dual_sys].mo_flag,
                s_sms_config[dual_sys].mt_flag, s_sms_config[dual_sys].bm_flag);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d,%d,%d,%d", g_atc_info_table[AT_CMD_CSMS].cmd_name->str_ptr,
                s_sms_config[dual_sys].service_flag, s_sms_config[dual_sys].mo_flag, s_sms_config[dual_sys].mt_flag, s_sms_config[dual_sys].bm_flag);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC:CSMS wrong type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_498_112_2_17_22_17_7_561,(uint8*)"");
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CSDH command.this command is used
//                to give additional information on text mode result code.
//                AT+CSDH=<value>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSDH )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS     status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                s_sms_config[dual_sys].csdh_flag = PARA1.num;
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CSDH].cmd_name->str_ptr,
                s_sms_config[dual_sys].csdh_flag);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC:CSDH wrong type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_545_112_2_17_22_17_7_562,(uint8*)"");
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CRES command.this command is used
//                to restore the parameters specified in the at+csca & at+csmp
//                AT+CRES
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRES )
{
#ifdef _ATC_SYNC_TOOL_
       MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    //check the SIM card
    if (!s_sms_config[dual_sys].sms_is_ready)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }
    //Set SC from SIM card
    if (ERR_MNSMS_NONE  != MNSMS_GetScAddrEx(dual_sys, &g_sc_addr[dual_sys]))
    {
        //SCI_TRACE_LOW:"ATC: Get sc failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_575_112_2_17_22_17_7_563,(uint8*)"");
    }

    //Set the s_sms_nv_config from NV.
    if (NVERR_NONE != EFS_NvitemRead(NV_SMS_CONFIG_ID,
        sizeof(ATC_SMS_NV_CONFIG_T), (uint8 *)&s_sms_nv_config))
    {
        //SCI_TRACE_LOW:"ATC: Read s_sms_nv_config failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_582_112_2_17_22_17_7_564,(uint8*)"");
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CSAS command.this command is used
//                to save the SMS configuration profile
//                AT+CSAS
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSAS )
{
#ifdef _ATC_SYNC_TOOL_
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if (!s_sms_config[dual_sys].sms_is_ready)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }
    // call the mnsms api to set the SC
    MNSMS_SetScAddrEx(dual_sys, &g_sc_addr[dual_sys]);

    //write para in AT+CSMP the NV.
    if (NVERR_NONE != EFS_NvitemWrite(NV_SMS_CONFIG_ID,
        sizeof(ATC_SMS_NV_CONFIG_T), (uint8 *)&s_sms_nv_config, FALSE))
    {
        //SCI_TRACE_LOW:"ATC: write NV_ATC_CONFIG_ID fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_614_112_2_17_22_17_7_565,(uint8*)"");
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CPMS command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPMS )
{
#ifdef _ATC_SYNC_TOOL_
    uint8                 *mem_str;
    uint8                 storage_id;
    MN_SMS_RECORD_ID_T    total_amount1;
    MN_SMS_RECORD_ID_T    used_amount1;
    BOOLEAN               unread_flag1;
    MN_SMS_RECORD_ID_T    total_amount2;
    MN_SMS_RECORD_ID_T    used_amount2;
    BOOLEAN               unread_flag2;
    uint8                 temp_str[20];
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        // Set command AT+CPMS=<mem1>[,<mem2>]
        case ATC_CMD_TYPE_SET:
        {
            // determine the parameter 1
        #ifdef ATC_ORANGE_ENABLE
            if(PARAM1_FLAG)
        #else
            if (PARAM1_FLAG && PARAM2_FLAG)
        #endif
            {
                mem_str = PARA1.str_ptr->str_ptr;
                if (CompareSmStorage((char*)mem_str, &storage_id))
                {
                    SCI_TRACE_LOW("ATC_ProcessCPMS storage_id = %d\n", storage_id);

                    // for compatible with wavecom spec
                    switch (storage_id)
                    {
                        case AT_SMS_STORAGE_SM:
                        {
                            s_sms_config[dual_sys].mem1 = MN_SMS_STORAGE_SIM;
                            break;
                        }

                    #ifdef ATC_ORANGE_ENABLE
                        case AT_SMS_STORAGE_ME:
                    #else
                        case AT_SMS_STORAGE_BM:
                    #endif
                        {
                            s_sms_config[dual_sys].mem1 = MN_SMS_STORAGE_ME;
                            break;
                        }

                        default:
                        {
                            //SCI_TRACE_LOW:"ATC:+CPMS,error in para1"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_668_112_2_17_22_17_7_566,(uint8*)"");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                }
                else
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }
            #ifndef ATC_ORANGE_ENABLE
                mem_str = PARA2.str_ptr->str_ptr;
                if (CompareSmStorage((char*)mem_str, &storage_id))
                {
                    // for compatible with wavecom spec
                    switch (storage_id)
                    {
                        case AT_SMS_STORAGE_SM:
                        {
                            s_sms_config[dual_sys].mem2 = MN_SMS_STORAGE_SIM;
                            break;
                        }

                        case AT_SMS_STORAGE_BM:
                        {
                            s_sms_config[dual_sys].mem2 = MN_SMS_STORAGE_ME;
                            break;
                        }

                        default:
                        {
                            //SCI_TRACE_LOW:"ATC:+CPMS,error in para2"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_697_112_2_17_22_17_7_567,(uint8*)"");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }

                    GetStorageInfo(dual_sys,s_sms_config[dual_sys].mem1, &total_amount1, &used_amount1,&unread_flag1);
                    GetStorageInfo(dual_sys,s_sms_config[dual_sys].mem2, &total_amount2, &used_amount2,&unread_flag2);
                    sprintf((char*)g_rsp_str, "%s: %d,%d,%d,%d", g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
                        used_amount1, total_amount1, used_amount2, total_amount2);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }
            #endif
            }
        #ifndef ATC_ORANGE_ENABLE
            else
            {
                return ERR_INVALID_INDEX;
            }
        #endif

            break;
        }

            // read command AT+CPMS?
        case ATC_CMD_TYPE_READ:
        {
            // get the mem1 info
            if (TRUE != GetStorageInfo(dual_sys,s_sms_config[dual_sys].mem1, &total_amount1, &used_amount1,&unread_flag1))
            {
                return ERR_SIM_BUSY;
            }
            if(MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem1)
            {
                sprintf((char*)g_rsp_str, "%s: \"SM\",%d,%d",g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
                    used_amount1,total_amount1);
            }
            else
            {
            #ifdef ATC_ORANGE_ENABLE
                sprintf((char*)g_rsp_str, "%s: \"ME\",%d,%d",g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
                    used_amount1,total_amount1);
            #else
                sprintf((char*)g_rsp_str, "%s: \"BM\",%d,%d",g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
                    used_amount1,total_amount1);
            #endif
            }
            // get the mem2 info
            if (TRUE != GetStorageInfo(dual_sys,s_sms_config[dual_sys].mem2, &total_amount2, &used_amount2,&unread_flag2))
            {
                return ERR_SIM_BUSY;
            }
            if(MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem2)
            {
                sprintf((char*)temp_str, ",\"SM\",%d,%d", used_amount2,total_amount2);
            }
            else
            {
            #ifdef ATC_ORANGE_ENABLE
                sprintf((char*)temp_str, ",\"ME\",%d,%d", used_amount2,total_amount2);
            #else
                sprintf((char*)temp_str, ",\"BM\",%d,%d", used_amount2,total_amount2);
            #endif
            }

            GetStorageInfo(dual_sys,s_sms_config[dual_sys].mem2,&total_amount2, &used_amount2,&unread_flag2);
            strcat((char*)g_rsp_str, (char*)temp_str);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_CMD_TYPE_TEST:
        {
        #ifdef ATC_ORANGE_ENABLE
            // sprintf((char*)g_rsp_str, "%s: ((\"%s\",\"%s\",\"%s\"),(\"%s\",\"%s\",\"%s\"))",g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
            sprintf((char*)g_rsp_str, "%s: ((\"%s\",\"%s\"),(\"%s\",\"%s\"))",g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
                s_sms_mem_str[AT_SMS_STORAGE_ME],s_sms_mem_str[AT_SMS_STORAGE_SM],/*s_sms_mem_str[AT_SMS_STORAGE_SR],*/
                s_sms_mem_str[AT_SMS_STORAGE_ME],s_sms_mem_str[AT_SMS_STORAGE_SM]/*,s_sms_mem_str[AT_SMS_STORAGE_SR]*/);
        #else
            // sprintf((char*)g_rsp_str, "%s: ((\"%s\",\"%s\",\"%s\"),(\"%s\",\"%s\",\"%s\"))",g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
            sprintf((char*)g_rsp_str, "%s: ((\"%s\",\"%s\"),(\"%s\",\"%s\"))",g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
                s_sms_mem_str[AT_SMS_STORAGE_BM],s_sms_mem_str[AT_SMS_STORAGE_SM],/*s_sms_mem_str[AT_SMS_STORAGE_SR],*/
                s_sms_mem_str[AT_SMS_STORAGE_BM],s_sms_mem_str[AT_SMS_STORAGE_SM]/*,s_sms_mem_str[AT_SMS_STORAGE_SR]*/);
        #endif
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC:+CPMS,error in para"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_770_112_2_17_22_17_8_568,(uint8*)"");
            break;
        }
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CPMS command This command is used
//                to set or read the parameters of sending SMS
//                AT+CSMP=<fo>,<pid>,<vp>,<dcs>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSMP )
{
 #if defined _ATC_SYNC_TOOL_ ||defined(_ATC_UIX8910_ENABLE_)
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                s_sms_nv_config.fo = (uint8)PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                s_sms_nv_config.vp = (uint8)PARA2.num;
            }

            if (PARAM3_FLAG)
            {
                s_sms_nv_config.pid = (uint8)PARA3.num;
            }

            if (PARAM4_FLAG)
            {
                s_sms_nv_config.dcs = (uint8)PARA4.num;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d,%d,%d,%d", g_atc_info_table[AT_CMD_CSMP].cmd_name->str_ptr,
                s_sms_nv_config.fo, s_sms_nv_config.vp,s_sms_nv_config.pid,s_sms_nv_config.dcs);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CMGF command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGF )
{
#if defined(_ATC_SYNC_TOOL_) && !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_UIX8910_ENABLE_) //reduce atc code  Nemge
    ATC_STATUS     status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                 atc_config_ptr->atc_config_nv.cmgf_flag = PARA1.num;
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CMGF].cmd_name->str_ptr,atc_config_ptr->atc_config_nv.cmgf_flag);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CMGR command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGR )
{
#if defined(_ATC_SYNC_TOOL_) && !defined(_ULTRA_LOW_CODE_) //reduce atc code Nemge
    ERR_MNSMS_CODE_E    mn_status   = ERR_MNSMS_NONE;
    MN_SMS_STATUS_E     *status_ptr   = PNULL;
    uint16                         total_num   = 0;
    uint16                                index   = 0;
    MN_DUAL_SYS_E             dual_sys   =  ATC_CURRENT_SIM_ID_MUX;

    if(!atc_config_ptr->sim_flag[dual_sys])
    {
        return ERR_SIM_NOT_INSERTED;
    }
    else
    {
        switch(atc_config_ptr->cpin[dual_sys])
        {
            case ATC_CPIN_CODE_READY:
                break;
            case ATC_CPIN_CODE_PIN1:
                return ERR_SIM_PIN_REQUIRED;
            case ATC_CPIN_CODE_PIN2:
                return ERR_SIM_PIN2_REQUIRED;
            case ATC_CPIN_CODE_PUK1:
                return ERR_SIM_PUK_REQUIRED;
            case ATC_CPIN_CODE_PUK2:
                return ERR_SIM_PUK2_REQUIRED;
            case ATC_CPIN_CODE_BLOCK:
            default:
                return ERR_SIM_FAILURE;
        }
    }

    // check the command type and the parameter1
    if (ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE || !PARAM1_FLAG)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_303);
        return S_ATC_DEFAULT_HANDLE;
    }
    if (!s_sms_config[dual_sys].sms_is_ready)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }

    SCI_ASSERT(PARA1.num >= 1);/*assert verified*/
    index = PARA1.num - 1;
    mn_status = GetSmsStatusArr(&total_num, &status_ptr, dual_sys, s_sms_config[dual_sys].mem1);

    if (ERR_MNSMS_PTR_IS_NULL == mn_status)
    {
        return (ERR_MEMORY_FAILURE);
    }
    else
    {
        if ((ERR_MNSMS_NONE != mn_status)
            || (index >= total_num)
            || (MN_SMS_FREE_SPACE == status_ptr[index]))
        {
            // response the error code "invalid index"
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
            SCI_FREE(status_ptr);
            return S_ATC_DEFAULT_HANDLE;
        }
    }

    s_sms_record[dual_sys].cmd_id    = AT_CMD_CMGR;
    MNSMS_ReadSmsEx(dual_sys, s_sms_config[dual_sys].mem1, index);

    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_READ_SMS_CNF,0xff,ATC_NO_DOMAIN);

    SCI_FREE(status_ptr);
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CSCA command to set the short
//                message service centre.
//                AT+CSCA=<scaddress>[,<tosc>]
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSCA)
{
#if defined(_ATC_SYNC_TOOL_) && !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_UIX8910_ENABLE_)//reduce atc code  Nemge
    uint8                  *addr_str;
    uint8                  tosa;       // the type of the service centre address
    uint8                  tel_str[(MN_MAX_ADDR_BCD_LEN<<1)+1] = {0};
    uint8                  tel_str_hex[(MN_MAX_ADDR_BCD_LEN<<2)+1] = {0};
    uint8                  bcd_len;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if (!s_sms_config[dual_sys].sms_is_ready)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }

    switch (ATC_GET_CMD_TYPE)
    {
        // AT+CSCA=<sca>[,<tosa>]
        case ATC_CMD_TYPE_SET:
        {
            // check the parameter 1
           if (!PARAM1_FLAG
            || ATC_CMD_PARAM_TYPE_STRING != PARAM1_TYPE)
           {
               ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
               return S_ATC_DEFAULT_HANDLE;
           }

           //check the length, sc length should less than or equeal 20
           if(PARA1.str_ptr->str_len > (MN_MAX_ADDR_BCD_LEN))
           {
               return ERR_INVALID_INDEX;
           }
            addr_str = PARA1.str_ptr->str_ptr;
            addr_str[PARA1.str_ptr->str_len] = '\0';

            if (*addr_str == '+')
            {
                addr_str = &PARA1.str_ptr->str_ptr[1];
                PARA1.str_ptr->str_len -= 1;
            }

            if(!TransfromStrToBcd(tel_str, &bcd_len,
                                  addr_str, PARA1.str_ptr->str_len))
            {
                return ERR_INVALID_INDEX;
            }
            SCI_ASSERT(bcd_len <= MN_MAX_ADDR_BCD_LEN);/*assert verified*/
            SCI_MEMCPY(g_sc_addr[dual_sys].party_num,tel_str,bcd_len);
            g_sc_addr[dual_sys].num_len = bcd_len;

            g_sc_addr[dual_sys].number_plan = MN_NUM_PLAN_ISDN_TELE;
            g_sc_addr[dual_sys].number_type = MN_NUM_TYPE_INTERNATIONAL;

            MNSMS_SetScAddrEx(dual_sys, &g_sc_addr[dual_sys]);
            break;
        }

        // AT+CSCA?
        case ATC_CMD_TYPE_READ:
        {
            if(ERR_MNSMS_NONE  != MNSMS_GetScAddrEx(dual_sys, &g_sc_addr[dual_sys]))
            {
                //SCI_TRACE_LOW:"ATC: Get sc failure"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_1049_112_2_17_22_17_8_569,(uint8*)"");
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_330);
                return S_ATC_DEFAULT_HANDLE;
            }

            addr_str = tel_str;
            SCI_MEMSET(tel_str,0, sizeof(tel_str));
            if (MN_NUM_TYPE_INTERNATIONAL == g_sc_addr[dual_sys].number_type)
            {
                tel_str[0] = '+';
                addr_str   = &tel_str[1];
            }
            TransformBcdToStr(g_sc_addr[dual_sys].party_num, g_sc_addr[dual_sys].num_len, addr_str);
            /* bit  8  7  6  5  4  3  2  1
            **     ext| number |  number  |
            **        | type   |   plan   |
            */
            tosa = (uint8)(g_sc_addr[dual_sys].number_plan + (g_sc_addr[dual_sys].number_type << 4) + 0x80);
            if(strcmp((char*)atc_config_ptr->cscs, "UCS2") == 0)
            {
                ConvertStr2Hex(addr_str, g_sc_addr[dual_sys].num_len * 2, tel_str_hex);
                sprintf((char*)g_rsp_str, "%s: \"%s\",%d",g_atc_info_table[AT_CMD_CSCA].cmd_name->str_ptr, tel_str_hex,tosa);
            }
            else
            {
                sprintf((char*)g_rsp_str, "%s: \"%s\",%d",g_atc_info_table[AT_CMD_CSCA].cmd_name->str_ptr, tel_str,tosa);
            }
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
            //SCI_ASSERT(FALSE);
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CNMI command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCNMI )
{
//#if defined(_ATC_SYNC_TOOL_) || defined(ATC_ORANGE_ENABLE)
#if defined(_ATC_SYNC_TOOL_)
       MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        // AT+CNMI=<mode>,<mt>,<bm>,<ds>
        case ATC_CMD_TYPE_SET:
        {
            // <mt>
            if (PARAM2_FLAG)
            {
                s_sms_config[dual_sys].mt_ind = (uint8)PARA2.num;
            }
            // <bm>
            if (PARAM3_FLAG)
            {
                s_sms_config[dual_sys].bm_ind = (uint8)PARA3.num;
            }
            // <ds>
            if (PARAM4_FLAG)
            {
                s_sms_config[dual_sys].ds_ind = (uint8)PARA4.num;
                if(s_sms_config[dual_sys].ds_ind == 1)
                {
                    //MNSMS_SetStautsRptSaveModeEx(dual_sys,FALSE);
                }
                else
                {
                    //MNSMS_SetStautsRptSaveModeEx(dual_sys,TRUE);
                }
            }

            // <bfr>
            if(PARAM5_FLAG)
            {
                s_sms_config[dual_sys].bfr_ind = (uint8)PARA5.num;
                SCI_TRACE_LOW("ATC: ATC_ProcessCNMI, bfr:%d", s_sms_config[dual_sys].bfr_ind);
            }

            break;

            // read command AT+CNMI?
        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d,%d,%d,%d,%d", g_atc_info_table[AT_CMD_CNMI].cmd_name->str_ptr,
                    s_sms_config[dual_sys].mode, s_sms_config[dual_sys].mt_ind, s_sms_config[dual_sys].bm_ind,
                    s_sms_config[dual_sys].ds_ind, s_sms_config[dual_sys].bfr_ind);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CMGD command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGD )
{
#if defined(_ATC_SYNC_TOOL_)&& !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_UIX8910_ENABLE_)//reduce atc code  Nemge
    ERR_MNSMS_CODE_E    mn_status   = ERR_MNSMS_NONE;
    MN_SMS_STATUS_E     *status_ptr = PNULL;
    uint16              total_num   = 0;
    uint16              index       = 0;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if((!s_sms_config[dual_sys].sms_is_ready) && (MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem1))
    {
        SCI_TRACE_LOW("ATC: SIM not inserted or ready!");
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }
    // check the command type and the parameter1
    if (ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE || (!PARAM1_FLAG))
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    s_sms_record[dual_sys].cmd_id = AT_CMD_CMGD;
    if (PARAM2_FLAG)
    {
        s_sms_record[dual_sys].cmgd_state = PARA2.num;
    }
    else
    {
        s_sms_record[dual_sys].cmgd_state = ATC_DEL_INDEX;
    }

    SCI_ASSERT(PARA1.num >= 1);/*assert verified*/
    index = PARA1.num - 1;
    mn_status = GetSmsStatusArr(&total_num, &status_ptr, dual_sys, s_sms_config[dual_sys].mem1);

    if (ERR_MNSMS_PTR_IS_NULL == mn_status)
    {
        return (ERR_MEMORY_FAILURE);
    }
    else
    {
        if (((ERR_MNSMS_NONE != mn_status)
            || (index >= total_num)
            || (MN_SMS_FREE_SPACE == status_ptr[index]))
            && (ATC_DEL_INDEX == s_sms_record[dual_sys].cmgd_state))
        {
            // response the error code "invalid index"
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
            SCI_FREE(status_ptr);
            return S_ATC_DEFAULT_HANDLE;
        }
    }

    //Delete the SMS by condition
    if (PARAM2_FLAG && ATC_DEL_INDEX != s_sms_record[dual_sys].cmgd_state)
    {
        s_sms_record[dual_sys].cmgl_idex = index;
        if (DeleteSpecSms(dual_sys,s_sms_config[dual_sys].mem1, &index,s_sms_record[dual_sys].cmgd_state))
        {
            MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem1, index, MN_SMS_FREE_SPACE);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_UPDATE_SMS_STATE_CNF,0xff,ATC_NO_DOMAIN);
        }
        else
        {
            SCI_FREE(status_ptr);
            return S_ATC_SUCCESS;
        }
    }
    //delete the SMS by index.
    else
    {
        // delete the SM identified by index
        MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem1, index, MN_SMS_FREE_SPACE);
        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_UPDATE_SMS_STATE_CNF,0xff,ATC_NO_DOMAIN);
    }
    SCI_FREE(status_ptr);
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CMGS command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGS )
{
#if defined(_ATC_SYNC_TOOL_) && !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_UIX8910_ENABLE_) //reduce atc code  Nemge
    uint8  sm_octet_len; // indicate the number of octets coded in the TP layer data unit to be given
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if(!atc_config_ptr->sim_flag[dual_sys])
    {
        return ERR_SIM_NOT_INSERTED;
    }
    else
    {
        switch(atc_config_ptr->cpin[dual_sys])
        {
            case ATC_CPIN_CODE_READY:
                break;
            case ATC_CPIN_CODE_PIN1:
                return ERR_SIM_PIN_REQUIRED;
            case ATC_CPIN_CODE_PIN2:
                return ERR_SIM_PIN2_REQUIRED;
            case ATC_CPIN_CODE_PUK1:
                return ERR_SIM_PUK_REQUIRED;
            case ATC_CPIN_CODE_PUK2:
                return ERR_SIM_PUK2_REQUIRED;
            case ATC_CPIN_CODE_BLOCK:
            default:
                return ERR_SIM_FAILURE;
        }
    }

    // check the parameter
    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_SET)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
        return S_ATC_DEFAULT_HANDLE;
    }
    if (PARAM2_FLAG)
    {
        s_sms_record[dual_sys].more_sms_flag = PARA2.num;
    }
    else
    {
        s_sms_record[dual_sys].more_sms_flag = FALSE;
    }

    switch (PARAM1_TYPE)
    {
        // the parameter 1 is the numeric and the message mode is PDU
        case ATC_CMD_PARAM_TYPE_NUMERIC:
        {
            sm_octet_len = (uint8)PARA1.num;
            if (!atc_config_ptr->atc_config_nv.cmgf_flag &&
                (ATC_MIN_SM_LEN < sm_octet_len ||
                ATC_MAX_SM_LEN > sm_octet_len))
            {
                s_sms_record[dual_sys].length = sm_octet_len;
            }
            else
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                return S_ATC_DEFAULT_HANDLE;
            }
            break;
        }

        case ATC_CMD_PARAM_TYPE_STRING:
        {
             //Check the length of input number
             if(2*MN_MAX_ADDR_BCD_LEN < PARA1.str_ptr->str_len)
             {
                 return ERR_INVALID_INDEX;
             }
            if (!atc_config_ptr->atc_config_nv.cmgf_flag ||
                !ATC_ConvertTeleStrToStruct(&s_sms_record[dual_sys].dest_num,
                                           PARA1.str_ptr->str_ptr,
                                           PARA1.str_ptr->str_len))
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                return S_ATC_DEFAULT_HANDLE;
            }
            break;
        }

        default:
        {
            SCI_ASSERT(FALSE);/*assert verified*/
            break;
        }
    }

    /*update SC Address*/
    if (ERR_MNSMS_NONE  != MNSMS_GetScAddrEx(dual_sys, &g_sc_addr[dual_sys]))
    {
        //SCI_TRACE_LOW:"ATC: Get sc failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_1323_112_2_17_22_17_9_570,(uint8*)"");
    }

    /*Set the s_sms_nv_config from NV*/
    if (NVERR_NONE != EFS_NvitemRead(NV_SMS_CONFIG_ID,
        sizeof(ATC_SMS_NV_CONFIG_T), (uint8 *)&s_sms_nv_config))
    {
        //SCI_TRACE_LOW:"ATC: Read s_sms_nv_config failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_1330_112_2_17_22_17_9_571,(uint8*)"");
    }

    s_sms_record[dual_sys].cmd_id = AT_CMD_CMGS;
    s_sms_record[dual_sys].state  = ATC_SMS_UNSENT;

    // output the '>'
    ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SMS);

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CMGW command
//                AT+CMGW=<length/destination address>,<state>
//                state: 0   "REC UNREAD"
//                       1   "REC READ"
//                       2   "STO UNSEND"
//                       3   "STO SENT"
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGW )
{
#if (!defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)) && defined(_ATC_SYNC_TOOL_) || defined(_ATC_UIX8910_ENABLE_)
    uint8              sm_octet_len;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if((!s_sms_config[dual_sys].sms_is_ready) && (MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem2))
    {
        SCI_TRACE_LOW("ATC: SIM not inserted or ready!");
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }


    // check the parameter1 and the command type
    if (ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE || !PARAM1_FLAG)
        return ERR_OPERATION_NOT_ALLOWED;

    switch (PARAM1_TYPE)
    {
        case ATC_CMD_PARAM_TYPE_STRING:
        {
            if (!atc_config_ptr->atc_config_nv.cmgf_flag ||
                !ATC_ConvertTeleStrToStruct(&s_sms_record[dual_sys].dest_num,
                                          PARA1.str_ptr->str_ptr,
                                          PARA1.str_ptr->str_len))
            {
                return ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_CMD_PARAM_TYPE_NUMERIC:
        {
            sm_octet_len = (uint8)PARA1.num;
            if (!atc_config_ptr->atc_config_nv.cmgf_flag
                && (ATC_MIN_SM_LEN < sm_octet_len || ATC_MAX_SM_LEN > sm_octet_len))
            {
                s_sms_record[dual_sys].length = sm_octet_len;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            break;
        }

        default:
            return ERR_INVALID_INDEX;
    }

    // check the parameter of <state>
    if (PARAM2_FLAG)
    {
        if(PARA2.num < ATC_SMS_ALL)
        {
            s_sms_record[dual_sys].state = PARA2.num;
        }
        else
        {
            return ERR_INVALID_INDEX;
        }
    }
    else
    {
        s_sms_record[dual_sys].state = ATC_SMS_UNSENT;
    }
    s_sms_record[dual_sys].cmd_id = AT_CMD_CMGW;
    // output the character '>'
    ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SMS);

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CMGC command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMGC )
{
#ifndef _ULTRA_LOW_CODE_
    uint8          sm_octet_len;
       MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if (ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE || !PARAM1_FLAG)
        return ERR_OPERATION_NOT_ALLOWED;

    sm_octet_len = (uint8)PARA1.num;

    s_sms_record[dual_sys].cmd_id = AT_CMD_CMGC;
    s_sms_record[dual_sys].state  = ATC_SMS_UNSENT;
    s_sms_record[dual_sys].length = sm_octet_len;
    // output the character '>'
    ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SMS);

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CMSS command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMSS )
{
#if defined(_ATC_SYNC_TOOL_) && !defined(_ULTRA_LOW_CODE_) || defined(_ATC_UIX8910_ENABLE_) //reduce atc code  Nemge
    ERR_MNSMS_CODE_E    mn_status   = ERR_MNSMS_NONE;
    MN_SMS_STATUS_E     *status_ptr = PNULL;
    uint16              total_num   = 0;
    uint16              index       = 0;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;


    SCI_ASSERT(PARA1.num >= 1);/*assert verified*/

    if(ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    s_sms_record[dual_sys].dest_num_flag = FALSE;
    // Get the parameter 1
    if (PARAM1_FLAG)
    {
        s_sms_record[dual_sys].index = (MN_SMS_RECORD_ID_T)(PARA1.num - 1);

    }
    else
    {
        // the length is invalid
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
        return S_ATC_DEFAULT_HANDLE;
    }
    // get the da and the toda
    if (PARAM1_FLAG && PARAM2_FLAG)
    {
        if (ATC_MAX_DIGITAL < PARA2.str_ptr->str_len)
        {
            // the destination address is invalid
            sprintf((char*)g_rsp_str, "+CMS ERROR: invalid PDU mode parameter");
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
            return S_ATC_DEFAULT_HANDLE;
        }
        else
        {
            if (!ATC_ConvertTeleStrToStruct(&s_sms_record[dual_sys].dest_num,
                                      PARA2.str_ptr->str_ptr,
                                      PARA2.str_ptr->str_len))
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                return S_ATC_DEFAULT_HANDLE;
            }
            s_sms_record[dual_sys].dest_num_flag = TRUE;
        }

    }

    /*update SC Address*/
    if (ERR_MNSMS_NONE  != MNSMS_GetScAddrEx(dual_sys, &g_sc_addr[dual_sys]))
    {
        //SCI_TRACE_LOW:"ATC: Get sc failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_1593_112_2_17_22_17_9_572,(uint8*)"");
    }

    /*Set the s_sms_nv_config from NV*/
    if (NVERR_NONE != EFS_NvitemRead(NV_SMS_CONFIG_ID,
        sizeof(ATC_SMS_NV_CONFIG_T), (uint8 *)&s_sms_nv_config))
    {
        //SCI_TRACE_LOW:"ATC: Read s_sms_nv_config failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_1600_112_2_17_22_17_9_573,(uint8*)"");
    }

    s_sms_record[dual_sys].cmd_id = AT_CMD_CMSS;

    index = PARA1.num - 1;
    mn_status = GetSmsStatusArr(&total_num, &status_ptr, dual_sys, s_sms_config[dual_sys].mem2);

    if (ERR_MNSMS_PTR_IS_NULL == mn_status)
    {

        return (ERR_MEMORY_FAILURE);
    }
    else
    {
        if (ERR_MNSMS_NONE != mn_status)
        {
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_301);
            SCI_FREE(status_ptr);

            return S_ATC_DEFAULT_HANDLE;
        }
    }

    //Read the SMS.
    if ((total_num >= PARA1.num)
        && (MN_SMS_MO_TO_BE_SEND == status_ptr[index]))
    {
        MNSMS_ReadSmsEx(dual_sys, s_sms_config[dual_sys].mem2, s_sms_record[dual_sys].index);
        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_READ_SMS_CNF,0xff,ATC_NO_DOMAIN);
    }
    else
    {

        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
    }

    SCI_FREE(status_ptr);
    // When receiving APP_MN_READ_SMS_CNF,
    //ATC_ProcessReadSmsCnf will call the api to send short message.
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CSCB command. This command is used
//                to select which type CBM to be received by the ME.
//  AT+CSCB=<mode>,<mids>,<dcss>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSCB )
{
#ifndef _ULTRA_LOW_CODE_
    MN_CB_MSG_ID_MUSTER_T             msg_arr;
    MN_CB_LANGUAGE_ID_MUSTER_T        lang_arr;
    ATC_STATUS                        status = S_ATC_SUCCESS;
    uint16                            msg_id = 0;
    MN_SMSCB_LANGUAGE_ID_E            lang_id = MN_SMSCB_LANGUAGE_UNSPECIFIED;
    BOOLEAN                           mode = FALSE;            // 0: msgs and dcss are accepted; 1: otherwise
    int32                             i;
    uint8                             *temp_ptr =PNULL;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    // get the cellb roadcast parameters
    if ((NVERR_NONE != EFS_NvitemRead(NV_ATC_CB_MSG_ID,
                       sizeof(MN_CB_MSG_ID_MUSTER_T), (uint8*)&msg_arr)) ||
        (NVERR_NONE != EFS_NvitemRead(NV_ATC_CB_LANG_ID,
                       sizeof(MN_CB_LANGUAGE_ID_MUSTER_T), (uint8*)&lang_arr)))
    {
        //SCI_TRACE_LOW:"ATC: Read msg and language from NV fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_1668_112_2_17_22_17_9_574,(uint8*)"");
        SCI_MEMSET(&msg_arr, 0, sizeof(msg_arr));
        SCI_MEMSET(&lang_arr, 0, sizeof(lang_arr));
    }

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                mode = PARA1.num;
                // check parameter 2: the msg identity
                if (PARAM2_FLAG)
                {
                    msg_id = (uint16)PARA2.num;
                }
                // check parameter 3: the language identity
                if (PARAM3_FLAG)
                {
                    lang_id = (MN_SMSCB_LANGUAGE_ID_E)PARA3.num;
                }
                // Add or delete the parameters
                HandleCbParam(mode, PARAM2_FLAG, msg_id, (BOOLEAN)PARAM3_FLAG, lang_id, &msg_arr, &lang_arr);
                if (!atc_config_ptr->smscb_flag[dual_sys])
                {
                    MNSMSCB_StartCbMsgEx(dual_sys, &msg_arr, &lang_arr);
                    atc_config_ptr->smscb_flag[dual_sys] = TRUE;
                }
                else
                {
                    MNSMSCB_ChangeCbParamEx(dual_sys, &msg_arr, &lang_arr);
                }
            }
            else
            {
                // the parameter 1 must be present
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            temp_ptr = g_rsp_str;
            sprintf((char*)temp_ptr, "%s: %d,\"", g_atc_info_table[AT_CMD_CSCB].cmd_name->str_ptr,mode);
            temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];
            // print the message id
            for (i=0; i<msg_arr.num; i++)
            {
                sprintf((char*)temp_ptr, "%d", msg_arr.msg_id_arr[i]);
                temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];
                if ((msg_arr.num - 1) != i)
                {
                    sprintf((char*)temp_ptr, ",");
                    temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];
                }
            }
            // add the character'"'
            temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];
            *temp_ptr++ = '"';
            *temp_ptr++ = ',';
            *temp_ptr++ = '"';
            *temp_ptr   = '\0';

            // print the language id
            for (i=0; i<lang_arr.num; i++)
            {
                sprintf((char*)temp_ptr, "%d", lang_arr.language_id_arr[i]);
                temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];
                if ((lang_arr.num - 1) != i)
                {
                    sprintf((char*)temp_ptr, ",");
                    temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];
                }
            }
            // add the character'"'
            temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];
            *temp_ptr++ = '"';
            *temp_ptr = '\0';
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
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
//  Description : This function handles the AT+SMSC command. This command is used
//                to change the specific SMS status: from READ to unread; for SENT
//                to UNSENT.
//  AT+SMSC=<loc>,<status>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSMSC )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS            status = S_ATC_DEFAULT_HANDLE;
    ATC_SMS_STATE_E       at_sms_status;
    MN_SMS_STATUS_E       sms_status;
    BOOLEAN               status_flag;

    ERR_MNSMS_CODE_E    mn_status   = ERR_MNSMS_NONE;
    MN_SMS_STATUS_E     *status_ptr = PNULL;
    uint16              total_num   = 0;
    uint16              index       = 0;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT(PARA1.num >= 1);/*assert verified*/

    if(ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    index = PARA1.num - 1;
    mn_status = GetSmsStatusArr(&total_num, &status_ptr, dual_sys, s_sms_config[dual_sys].mem2);

    if (ERR_MNSMS_PTR_IS_NULL == mn_status)
    {
        return (ERR_MEMORY_FAILURE);
    }
    else
    {
        if (ERR_MNSMS_NONE != mn_status)
        {
            //SCI_TRACE_LOW:"ATC:MNSMS_GetSmsStatus is error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_1800_112_2_17_22_17_10_575,(uint8*)"");
            SCI_FREE(status_ptr);
            return ERR_OPERATION_NOT_ALLOWED;
        }
    }

    if (PARAM1_FLAG && PARAM2_FLAG)
    {
        if ((PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC)
            && !atc_config_ptr->atc_config_nv.cmgf_flag)
        {
            at_sms_status = PARA2.num;
            if (ChangeSmsStatus(status_ptr[index], &sms_status, at_sms_status, &status_flag))
            {
                if (status_flag)
                {
                    //SCI_TRACE_LOW("ATC:status_flag = TRUE");
                    MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem2, index, sms_status);
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_UPDATE_SMS_STATE_CNF,0xff,ATC_NO_DOMAIN);
                    s_sms_record[dual_sys].cmd_id = AT_CMD_SMSC;
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC:status_flag = FALSE"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_1823_112_2_17_22_17_10_576,(uint8*)"");
                    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                }
            }
            else
            {
                //SCI_TRACE_LOW:"ATC:ChangeSmsStatus is FALSE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_1829_112_2_17_22_17_10_577,(uint8*)"");
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
            }
            //SCI_TRACE_LOW("ATC:status_ptr=%d, sms_status=%d,at_sms_status=%d,status_flag=%d",
            //  status_ptr[index], sms_status, at_sms_status, status_flag);
        }
        // the parameter 2 is string type and the message format is text mode
        else if ((PARAM2_TYPE == ATC_CMD_PARAM_TYPE_STRING)
            && atc_config_ptr->atc_config_nv.cmgf_flag)
        {
            PARA2.str_ptr->str_ptr[PARA2.str_ptr->str_len] = '\0';
            if (ConvertStrStatusToNumStatus(PARA2.str_ptr->str_ptr, &at_sms_status))
            {
                if (ChangeSmsStatus(status_ptr[index], &sms_status, at_sms_status, &status_flag))
                {
                    if (status_flag)
                    {
                        MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem2, index, sms_status);
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_UPDATE_SMS_STATE_CNF,0xff,ATC_NO_DOMAIN);
                        s_sms_record[dual_sys].cmd_id = AT_CMD_SMSC;
                    }
                    else
                    {
                        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                    }
                }
                else
                {
                    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
                }
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
    }
    else
    {
        status = ERR_OPERATION_NOT_ALLOWED;
    }
    SCI_FREE(status_ptr);
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+SFSMSM command
//                AT+SFSMSM=<length/destination address>,<state>
//                state: 0   "REC UNREAD"
//                       1   "REC READ"
//                       2   "STO UNSEND"
//                       3   "STO SENT"
//  Global resource dependence : g_rsp_str
//  Author:       Elvis Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSFSMSM )
{
#ifndef _ULTRA_LOW_CODE_
    uint8    sm_octet_len = 0;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    //SCI_TRACE_LOW:"ATC: ATC_ProcessSFSMSM is called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_1896_112_2_17_22_17_10_578,(uint8*)"");
    if(!atc_config_ptr->sim_flag[dual_sys])
    {
        return ERR_SIM_NOT_INSERTED;
    }
    else
    {
        switch(atc_config_ptr->cpin[dual_sys])
        {
            case ATC_CPIN_CODE_READY:
                break;
            case ATC_CPIN_CODE_PIN1:
                return ERR_SIM_PIN_REQUIRED;
            case ATC_CPIN_CODE_PIN2:
                return ERR_SIM_PIN2_REQUIRED;
            case ATC_CPIN_CODE_PUK1:
                return ERR_SIM_PUK_REQUIRED;
            case ATC_CPIN_CODE_PUK2:
                return ERR_SIM_PUK2_REQUIRED;
            case ATC_CPIN_CODE_BLOCK:
            default:
                return ERR_SIM_FAILURE;
        }
    }

    // to update sms storage information
    Atc_FreeMemForSfsmsm(&s_sms_fill_mem);
    if (MN_SMS_ALL_FULL == Atc_UpdateSmsMemSts(atc_config_ptr, &s_sms_fill_mem))
    {
        if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT10))
        {
            sprintf((char*)g_rsp_str, "%s: 1024,2", g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        return ERR_MEMORY_FULL;
    }

    // check the parameter1 and the command type
    if (ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE || !PARAM1_FLAG)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    switch (PARAM1_TYPE)
    {
        case ATC_CMD_PARAM_TYPE_STRING:
        {
            if (!atc_config_ptr->atc_config_nv.cmgf_flag ||
                !ATC_ConvertTeleStrToStruct(&s_sms_record[dual_sys].dest_num,
                                          PARA1.str_ptr->str_ptr,
                                          PARA1.str_ptr->str_len))
            {
                return ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_CMD_PARAM_TYPE_NUMERIC:
        {
            sm_octet_len = (uint8)PARA1.num;
            if (!atc_config_ptr->atc_config_nv.cmgf_flag &&
                (ATC_MIN_SM_LEN < sm_octet_len ||
                ATC_MAX_SM_LEN > sm_octet_len))
            {
                s_sms_record[dual_sys].length = sm_octet_len;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            break;
        }

        default:
            //SCI_TRACE_LOW:"ATC:SFSMSM assert wrong para"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_1970_112_2_17_22_17_10_579,(uint8*)"");
            break;
    }

    // check the parameter of <state>
    if (PARAM2_FLAG)
    {
        if(PARA2.num < ATC_SMS_ALL)
        {
            s_sms_record[dual_sys].state = PARA2.num;
        }
        else
        {
            return ERR_INVALID_INDEX;
        }
    }
    else
    {
        s_sms_record[dual_sys].state = ATC_SMS_UNSENT;
    }
    s_sms_record[dual_sys].cmd_id = AT_CMD_SFSMSM;
    // output the character '>'
    ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SMS);

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the SMS memory indication from AP.
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPSMSFULL)
{
    ATC_STATUS            status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                if(0x01 == PARA1.num)
                {
                    if(ERR_MNSMS_NONE != MNSMS_SetSmsReceiveFuncEx(dual_sys, FALSE))
                    {
                        status = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }
                }
                else if(0x00 == PARA1.num)
                {
                    if(ERR_MNSMS_NONE != MNSMS_SetSmsReceiveFuncEx(dual_sys, TRUE))
                    {
                        status = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }
                }
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,  APP_MN_SET_SMS_RECEIVE_FUNC_CNF, 0xff, ATC_NO_DOMAIN);
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:
            status =  S_ATC_SUCCESS;
            break;

        case ATC_CMD_TYPE_TEST:
            status = S_ATC_SUCCESS;
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return status;
}


#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_) || defined(_ATC_UIX8910_ENABLE_)
/*****************************************************************************/
//  Description : This function handles the PDU send from the SIO.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSmsPdu(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *pdu_ptr  // pointer to the given pdu buffer
                )
{
    uint8       terminater_char;     // indicates ther terminater char
    uint16      pdu_len;          // holds the gived pdu length
    BOOLEAN     flag = FALSE;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/

    pdu_len              = (uint16)strlen((char*)pdu_ptr);
    terminater_char      = pdu_ptr[pdu_len - 1];
    pdu_ptr[pdu_len - 1] = '\0';
    pdu_len             -= 1;

    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            SCI_MEMSET(&s_sms_record[dual_sys], 0, sizeof(s_sms_record[dual_sys]));
            flag = TRUE;
            break;
        }

        case CTRL_Z:
        {
            // text mode SMS
            if (atc_config_ptr->atc_config_nv.cmgf_flag)
            {
            #if defined( _MUX_ENABLE_)||defined(_ATC_UIX8910_ENABLE_)
                flag = HandleTextModeSms(atc_config_ptr,pdu_ptr, pdu_len);
            #else
                flag = HandleTextModeSms(pdu_ptr, pdu_len);
            #endif
            }
            // pdu mode SMS
            else if (s_sms_record[dual_sys].length <= (pdu_len / 2))
            {
            #if defined( _MUX_ENABLE_)||defined(_ATC_UIX8910_ENABLE_)
                flag = HandlePduModeSms(atc_config_ptr,pdu_ptr, pdu_len);
            #else
                //SCI_TRACE_LOW("ATC:Call HandlePduModeSms()");
                flag = HandlePduModeSms(pdu_ptr, pdu_len);
            #endif
            }
            break;
        }

        default:
            break;
    }
    //SCI_TRACE_LOW("ATC:flag = %d",flag);
    //return the status
    if (!flag)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_304);
        s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
    }
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the text mode SMS
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
#if defined( _MUX_ENABLE_)||defined(_ATC_UIX8910_ENABLE_)
LOCAL BOOLEAN HandleTextModeSms(
             ATC_CONFIG_T   *atc_config_ptr,
             uint8          *pdu_ptr,       // point to the pdu buffer
             uint16         pdu_len
             )
#else
LOCAL BOOLEAN HandleTextModeSms(
             uint8         *pdu_ptr,       // point to the pdu buffer
             uint16         pdu_len
             )
#endif
{
    MN_SMS_MO_SMS_T    mo_sms;
    MN_SMS_MT_SMS_T    mt_sms;
    MN_SMS_USER_DATA_HEAD_T  user_data_head_t; // only for interface change
    MN_SMS_USER_VALID_DATA_T user_valid_data_t;
    BOOLEAN            temp_flag = TRUE;
#if !defined(_ULTRA_LOW_CODE_) || defined(_ATC_UIX8910_ENABLE_)
    ERR_MNSMS_CODE_E   send_result;
#endif
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
//    #ifndef _ULTRA_LOW_CODE_
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE) || defined(_ATC_UIX8910_ENABLE_)
    MN_SMS_STATUS_E    status;
    MN_SMS_MT_SMS_T * mt_sms_ptr = PNULL;
    MN_SMS_MO_SMS_T * mo_sms_ptr = PNULL;
#endif

    SCI_MEMSET(&mo_sms, 0, sizeof(mo_sms));

    switch (s_sms_nv_config.dcs)
    {
        // 7 bit encoded
        case MN_SMS_DEFAULT_ALPHABET:
        {
            if (ATC_SMS_7BIT_LEN < pdu_len)
            {
                temp_flag = FALSE;
            }
            break;
        }

        // unicode encoded
        case MN_SMS_UCS2_ALPHABET:
        {
            if (ATC_SMS_UCS2_LEN < pdu_len)
            {
                temp_flag = FALSE;
            }
            else
            {
                //change ascii format of UINCODE into bin format.
                ConvertAscUcs2IntoBin((char*)pdu_ptr,
                              user_valid_data_t.user_valid_data_arr,
                              pdu_len/2);
            }
            break;
        }

        // 8 bit encoded
        case MN_SMS_8_BIT_ALPHBET:
        {
            if (ATC_SMS_8BIT_LEN < pdu_len)
            {
                temp_flag = FALSE;
            }
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC:assert wrong dcs type!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3122_112_2_17_22_17_12_603,(uint8*)"");
            break;
        }
    }


    // Check the dcs and input string type
    if (temp_flag)
    {
        if(ATC_SMS_UNSENT == s_sms_record[dual_sys].state||ATC_SMS_SENT == s_sms_record[dual_sys].state)
        {
            mo_sms.reply_path_is_request     = s_sms_nv_config.fo & 0x80;
            mo_sms.status_report_is_request  = s_sms_nv_config.fo & 0x20; // get the bit 5
            mo_sms.time_format_e = MN_SMS_TP_VPF_RELATIVE_FORMAT; // get the bit 4
            if(s_sms_nv_config.fo & 0x10)
            {
                mo_sms.time_format_e         = MN_SMS_TP_VPF_RELATIVE_FORMAT;
            }
            else
            {
                mo_sms.time_format_e         = MN_SMS_TP_VPF_NOT_PRESENT;
            }
            mo_sms.time_valid_period.time_second = GetValidPeriodSeconds(s_sms_nv_config.vp);
            mo_sms.dest_addr_present         = TRUE;
            mo_sms.dest_addr_t               = s_sms_record[dual_sys].dest_num;
            mo_sms.sc_addr_present           = TRUE;
            SCI_MEMCPY(&mo_sms.sc_addr_t, &g_sc_addr[dual_sys],sizeof(MN_CALLED_NUMBER_T));
            mo_sms.pid_present               = TRUE;
            mo_sms.pid_e                     = (MN_SMS_PID_E)s_sms_nv_config.pid;
            mo_sms.dcs.alphabet_type         = (MN_SMS_ALPHABET_TYPE_E)s_sms_nv_config.dcs;
            mo_sms.dcs.class_is_present      = FALSE;
            mo_sms.dcs.msg_is_waiting        = FALSE;

            if(MN_SMS_UCS2_ALPHABET != s_sms_nv_config.dcs)
            {
                if(MN_SMS_USER_DATA_LENGTH <= pdu_len)
                {
                    return FALSE;
                }

                user_valid_data_t.length = pdu_len;
                SCI_MEMCPY(user_valid_data_t.user_valid_data_arr, pdu_ptr, pdu_len);
            }
            else
            {
                user_valid_data_t.length = pdu_len/2;
            }

            MNSMS_EncodeUserDataEx(dual_sys,
                                FALSE,
                                (MN_SMS_ALPHABET_TYPE_E)(s_sms_nv_config.dcs),
                                &user_data_head_t, // at should build this value when support long SMS
                                &user_valid_data_t,
                                &(mo_sms.user_data_t)
                                );
        }
        else
        {
            mt_sms.reply_path_is_exist          = s_sms_nv_config.fo & 0x80;
            mt_sms.sme_status_report_is_request = s_sms_nv_config.fo & 0x20; // get the bit 5
            mt_sms.pid_e                        = (MN_SMS_PID_E)(s_sms_nv_config.pid);
            mt_sms.dcs.alphabet_type            = (MN_SMS_ALPHABET_TYPE_E)s_sms_nv_config.dcs;
            mt_sms.dcs.class_is_present         = FALSE;
            mt_sms.dcs.msg_is_waiting           = FALSE;
            mt_sms.origin_addr_t                = s_sms_record[dual_sys].dest_num;

            SCI_MEMCPY(&mt_sms.sc_addr_t,&g_sc_addr[dual_sys],sizeof(g_sc_addr[dual_sys]));

            if(MN_SMS_UCS2_ALPHABET != s_sms_nv_config.dcs)
            {
                user_valid_data_t.length = pdu_len;
                SCI_MEMCPY(user_valid_data_t.user_valid_data_arr, pdu_ptr, pdu_len);
            }
            else
            {
                user_valid_data_t.length = pdu_len/2;
            }
            MNSMS_EncodeUserDataEx(dual_sys,
                                 FALSE, // AT do not support long SMS now
                                 (MN_SMS_ALPHABET_TYPE_E)(s_sms_nv_config.dcs),
                                 &user_data_head_t, // at should build this value when support long SMS
                                 &user_valid_data_t,
                                 &(mt_sms.user_data_t)
                                 );

        }

        switch (s_sms_record[dual_sys].cmd_id)
        {
        #if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_UIX8910_ENABLE_)// reduce atc code Nemge
            case AT_CMD_CMGS:
            {
                //SCI_TRACE_LOW:"ATC:status_report_is_request=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3213_112_2_17_22_17_12_604,(uint8*)"d",mo_sms.status_report_is_request);
                send_result = MNSMS_AppSendSmsEx(dual_sys, &mo_sms, MN_SMS_NO_STORED,
                                                 MN_SMS_GSM_PATH, s_sms_record[dual_sys].more_sms_flag);
                if(ERR_MNSMS_NONE != send_result)
                {
                #ifdef _MUX_ENABLE_
                    if(ERR_MNSMS_NO_SC_ADDRESS == send_result)
                    {
                        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_330);
                    }
                    else
                    {
                        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                    }
                #endif
                    temp_flag = FALSE;
                }
                else
                {
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                              APP_MN_SEND_SMS_CNF, 0xff, ATC_NO_DOMAIN);
                }

                break;
            }
         #endif

        #ifndef _ULTRA_LOW_CODE_
            case AT_CMD_SFSMSM:
        #endif

        #if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
            case AT_CMD_CMGW:
            {
                switch (s_sms_record[dual_sys].state)
                {
                    // the MT short message
                    case ATC_SMS_UNREAD:
                    {
                        status = MN_SMS_MT_TO_BE_READ;
                        break;
                    }

                    case ATC_SMS_READ:
                    {
                        status = MN_SMS_MT_READED;
                        break;
                    }

                    // the MO short message
                    case ATC_SMS_UNSENT:
                    {
                        status = MN_SMS_MO_TO_BE_SEND;
                        break;
                    }

                    case ATC_SMS_SENT:
                    {
                        status = MN_SMS_MO_SR_NOT_REQUEST;
                        break;
                    }

                    default:
                    {
                        status = MN_SMS_MT_TO_BE_READ;
                        break;
                    }
                }

                if (MN_SMS_MT_TO_BE_READ == status||MN_SMS_MT_READED == status)
                {
                    //Write the mt sms
                    if (ERR_MNSMS_NONE != MNSMS_WriteMtSmsEx(dual_sys, s_sms_config[dual_sys].mem2, status, &mt_sms))
                    {
                        temp_flag = FALSE;
                    }
                    else
                    {
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_WRITE_SMS_CNF,0xff,ATC_NO_DOMAIN);
                    }

                    //to save the filling data for  AT+SFSMSM
                    if ((AT_CMD_SFSMSM == s_sms_record[dual_sys].cmd_id) &&  temp_flag)
                    {
                        mt_sms_ptr = (MN_SMS_MT_SMS_T*)SCI_ALLOC_BASE(sizeof(MN_SMS_MT_SMS_T));

                        SCI_MEMCPY((void *)mt_sms_ptr, (void *)&mt_sms, sizeof(MN_SMS_MT_SMS_T));

                        s_sms_fill_mem.mt_sms_ptr = mt_sms_ptr;
                        s_sms_fill_mem.sms_status= status;
                    }
                }
                else
                {
                    //Wrtie the mo sms
                    if (ERR_MNSMS_NONE != MNSMS_WriteMoSmsEx(dual_sys, s_sms_config[dual_sys].mem2, status, &mo_sms))
                    {
                        temp_flag = FALSE;
                    }
                    else
                    {
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_WRITE_SMS_CNF,0xff,ATC_NO_DOMAIN);
                    }

                    //to save the filling data for  AT+SFSMSM
                    if ((AT_CMD_SFSMSM == s_sms_record[dual_sys].cmd_id) && temp_flag)
                    {
                        mo_sms_ptr = (MN_SMS_MO_SMS_T*)SCI_ALLOC_BASE(sizeof(MN_SMS_MO_SMS_T));

                        SCI_MEMCPY((void *)mo_sms_ptr, (void *)&mo_sms, sizeof(MN_SMS_MO_SMS_T));

                        s_sms_fill_mem.mo_sms_ptr = mo_sms_ptr;
                        s_sms_fill_mem.sms_status= status;
                    }
                }
                break;
            }
        #endif/*_ULTRA_LOW_CODE_*/
            default:
                break;
        }/*lint !e764 */
    }
    return temp_flag;
}

/*****************************************************************************/
//  Description : This function handles the binary format pdu string
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
#if defined( _MUX_ENABLE_)||defined(_ATC_UIX8910_ENABLE_)
LOCAL BOOLEAN HandlePduModeSms(
             ATC_CONFIG_T  *atc_config_ptr,
             uint8         *pdu_ptr,       // point to the pdu buffer
             uint16         pdu_len
             )
#else
LOCAL BOOLEAN HandlePduModeSms(
             uint8         *pdu_ptr,       // point to the pdu buffer
             uint16         pdu_len
             )
#endif
{
    uint32                i;
    uint8                 cmd_id;
    MN_L3_MSG_UNIT_T      bin_pdu;
    MN_SMS_ADDR_T         sc_addr;        // holds the Short Message Service Center code
    BOOLEAN               flag = TRUE;

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE) ||defined(_ATC_UIX8910_ENABLE_)
    MN_SMS_STATUS_E       status;
#endif

#ifndef _ULTRA_LOW_CODE_
    MN_L3_MSG_UNIT_T * bin_pdu_ptr = PNULL;
    MN_SMS_ADDR_T * src_addr_ptr = PNULL;
#endif
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //SCI_TRACE_LOW:"ATC:enter HandlePduModeSms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2199_112_2_17_22_17_10_581,(uint8*)"");

    SCI_ASSERT(PNULL != pdu_ptr); /*assert verified*/

    if ((MN_MAX_L3_MESSAGE_SIZE < (pdu_len / 2))
        ||!ConvertHexToBin(pdu_ptr, pdu_len, bin_pdu.l3_msg))
    {
        //SCI_TRACE_LOW:"ATC:error ConvertHexToBin"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2206_112_2_17_22_17_10_582,(uint8*)"");
        return FALSE;
    }

    bin_pdu.length = (uint16)(pdu_len / 2);
    cmd_id         = (uint8)(s_sms_record[dual_sys].cmd_id);

    // get the SC address
    if(0 == (int16)(bin_pdu.l3_msg[0]))
    {
        //SCI_TRACE_LOW:"ATC:no SC address"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2216_112_2_17_22_17_10_583,(uint8*)"");

        if(g_sc_addr[dual_sys].num_len == 0)
        {
            //SCI_TRACE_LOW:"ATC:please set the SC address."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2220_112_2_17_22_17_10_584,(uint8*)"");

            sc_addr.length = 0;
            sc_addr.addr_arr_t [0] = 0;
        }
        else
        {
            sc_addr.length = g_sc_addr[dual_sys].num_len;

            sc_addr.addr_arr_t [0] = g_sc_addr[dual_sys].number_plan +
                                     (g_sc_addr[dual_sys].number_type << 4) + 0x80;
            sc_addr.length += 1;

            if(sc_addr.length < 1)
            {
                SCI_PASSERT(FALSE, ("ATC: HandlePduModeSms sc_addr.length is 0"));/*assert verified: check valid value*/
            }

            if(sc_addr.length > MN_SMS_ADDR_MAX_LEN)
            {
                //SCI_TRACE_LOW:"ATC: HandlePduModeSms sc_addr.length = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2241_112_2_17_22_17_10_585,(uint8*)"d",sc_addr.length);
                sc_addr.length = MN_SMS_ADDR_MAX_LEN;
            }

            SCI_MEMCPY(((char *)sc_addr.addr_arr_t + 1),
                       (char *)g_sc_addr[dual_sys].party_num,
                       (sc_addr.length - 1));
        }

        //copy the data
        bin_pdu.length = bin_pdu.length - 1;
        for (i=0; i<bin_pdu.length; i++)
        {
            bin_pdu.l3_msg[i] = bin_pdu.l3_msg[i+1];
        }
    }
    else
    {
        //SCI_TRACE_LOW:"ATC:have SC address"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2259_112_2_17_22_17_10_586,(uint8*)"");
        sc_addr.length = (uint16)(bin_pdu.l3_msg[0]);
        if ((sc_addr.length < 1) || (sc_addr.length > MN_SMS_ADDR_MAX_LEN))
        {
            //SCI_TRACE_LOW:"ATC:SC address length %d out range(1-%d)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2263_112_2_17_22_17_10_587,(uint8*)"dd", sc_addr.length, MN_SMS_ADDR_MAX_LEN);
            return FALSE;
        }

        SCI_MEMCPY(sc_addr.addr_arr_t, &bin_pdu.l3_msg[1], sc_addr.length);
        //copy the data
        bin_pdu.length = bin_pdu.length -((uint16)(bin_pdu.l3_msg[0]) + 1);
        for (i=0; i<bin_pdu.length; i++)
        {
            bin_pdu.l3_msg[i] = bin_pdu.l3_msg[sc_addr.length + 1 + i];
        }
    }

    if (MN_SMS_SIM_SMS_LEN < bin_pdu.length)
    {
        SCI_TRACE_LOW("ATC: ERROR SMS tpdu length %d is longer than permited %d", bin_pdu.length, MN_SMS_SIM_SMS_LEN);
        return FALSE;
    }
    switch (cmd_id)
    {
        // AT+CMGS=<length>,<cr>,<pdu> <ctrl+z>
#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_UIX8910_ENABLE_)
  // reduce atc code Nemge
        case AT_CMD_CMGS:
    #if 0
        case AT_CMD_SCMGS:
    #endif
        {
            MNSMS_SendATSmsPDUEx(dual_sys, &sc_addr, &bin_pdu, s_sms_record[dual_sys].more_sms_flag);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_SEND_SMS_CNF,0xff,ATC_DOMAIN_CS);
            break;
        }

        // AT+CMGC=<length>, <cr>, <pdu is given>,<ctrl+z>
        case AT_CMD_CMGC:
        {
            MNSMS_AtcSendCommandEx(dual_sys, &sc_addr, &bin_pdu);
            break;
        }
#endif
#ifndef _ULTRA_LOW_CODE_
        case AT_CMD_SFSMSM:
#endif


#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE) ||defined(_ATC_UIX8910_ENABLE_)
        // AT+CMGW=<length>,<cr>,<pdu is given>,<ctrl+z>
        case AT_CMD_CMGW:

    #if 0
        case AT_CMD_SCMGW:
    #endif


        {
            switch (s_sms_record[dual_sys].state)
            {
                // the MT short message
                case ATC_SMS_UNREAD:
                {
                    status = MN_SMS_MT_TO_BE_READ;
                    break;
                }

                case ATC_SMS_READ:
                {
                    status = MN_SMS_MT_READED;
                    break;
                }

                // the MO short message
                case ATC_SMS_UNSENT:
                {
                    status = MN_SMS_MO_TO_BE_SEND;
                    break;
                }

                case ATC_SMS_SENT:
                {
                    status = MN_SMS_MO_SR_NOT_REQUEST;
                    break;
                }

                default:
                {
                    status = MN_SMS_MT_TO_BE_READ;
                    break;
                }
            }

            MNSMS_AtcWriteSmsEx(dual_sys, s_sms_config[dual_sys].mem2, status, &sc_addr, &bin_pdu);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_WRITE_SMS_CNF,0xff,ATC_NO_DOMAIN);

            #ifndef _ULTRA_LOW_CODE_
            //to save temp filling data for  AT+SFSMSM
            if(AT_CMD_SFSMSM == cmd_id)
            {
                bin_pdu_ptr = (MN_L3_MSG_UNIT_T *)SCI_ALLOC_BASE(sizeof(MN_L3_MSG_UNIT_T));

                SCI_MEMCPY((void *)bin_pdu_ptr, (void *)&bin_pdu, sizeof(MN_L3_MSG_UNIT_T));

                src_addr_ptr = (MN_SMS_ADDR_T *)SCI_ALLOC_BASE(sizeof(MN_SMS_ADDR_T));

                SCI_MEMCPY((void *)src_addr_ptr, (void *)&sc_addr, sizeof(MN_SMS_ADDR_T));

                s_sms_fill_mem.bin_pdu_ptr = bin_pdu_ptr;
                s_sms_fill_mem.src_addr_ptr = src_addr_ptr;
                s_sms_fill_mem.sms_status= status;
            }
            #endif
            break;
        }
    #endif

        default:
        {
            flag = FALSE;
            break;
        }
    }/*lint !e764 */
    return flag;
}

/*****************************************************************************/
//  Description : This function initializes the static variables for SMS module
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern void ATC_InitSmsConfig( void )
{
    uint8 i = 0;

    // Set the default value
    for (i = MN_DUAL_SYS_1; i<MN_SYS_NUMBER; i++)
    {
        s_sms_config[i].mem1        = MN_SMS_STORAGE_SIM;
        s_sms_config[i].mem2        = MN_SMS_STORAGE_SIM;
        s_sms_config[i].mem3        = MN_SMS_STORAGE_SIM;
        s_sms_config[i].mode            = AT_RETURN_SESULT_CODE;
        s_sms_config[i].mt_ind      = MT_IND_DEFAULT_VAL;
        s_sms_config[i].bm_ind      = BM_IND_DEFAULT_VAL;
        s_sms_config[i].ds_ind      = DS_IND_DEFAULT_VAL;
        s_sms_config[i].bfr_ind = BFR_IND_DEFAULT_VAL;
        s_sms_config[i].mo_flag = TRUE;
        s_sms_config[i].mt_flag = TRUE;
        s_sms_config[i].bm_flag = TRUE;
        s_sms_config[i].service_flag = TRUE;
        s_sms_config[i].csdh_flag     = FALSE;
        s_sms_config[i].sm_len        = 0;
        s_sms_config[i].sms_is_ready = FALSE;
        s_sms_config[i].suss_flag  = TRUE;

        SCI_MEMSET(&s_sms_record[i], 0, sizeof(s_sms_record[i]));
    }

    if (NVERR_NONE != EFS_NvitemRead(NV_SMS_CONFIG_ID,
        sizeof(ATC_SMS_NV_CONFIG_T), (uint8 *)&s_sms_nv_config))
    {
        s_sms_nv_config.fo         = ATC_SMS_FO_DEFAULT_VAL;
        s_sms_nv_config.dcs        = MN_SMS_DEFAULT_ALPHABET;
        s_sms_nv_config.vp         = ATC_SMS_VP_DEFAULT_VAL;
        s_sms_nv_config.pid        = ATC_SMS_PID_DEFAULT_VAL;
        //write the NV.
        if (NVERR_NONE != EFS_NvitemWrite(NV_SMS_CONFIG_ID,
            sizeof(ATC_SMS_NV_CONFIG_T), (uint8 *)&s_sms_nv_config, FALSE))
        {
            //SCI_TRACE_LOW:"ATC: write NV_ATC_CONFIG_ID fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2133_112_2_17_22_17_10_580,(uint8*)"");
        }

    }
}

#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_UIX8910_ENABLE_)
/*****************************************************************************/
//  Description : This function handles the APP_MN_UPDATE_SMS_STATUS_CNF signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessUpdateSmsCnf(
                ATC_CONFIG_T                      *atc_config_ptr,
                APP_MN_UPDATE_SMS_STATUS_CNF_T    *sig_ptr
                )
{
    MN_SMS_RECORD_ID_T     del_index = 0;

    MN_DUAL_SYS_E dual_sys;

    dual_sys = sig_ptr->dual_sys;

    switch (sig_ptr->cause)
    {
        case MN_SMS_OPERATE_SUCCESS:
        {
            switch (s_sms_record[dual_sys].cmd_id )
            {
                case AT_CMD_CMGR:
                {
                    s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
                    break;
                }

                case AT_CMD_CMGD:
                {
                    if (DeleteSpecSms(dual_sys,s_sms_config[dual_sys].mem1, &del_index,s_sms_record[dual_sys].cmgd_state))
                    {
                        //SCI_TRACE_LOW:"ATC: Received APP_MN_UPDATE_SMS_STATE_CNF, sleep 30ms"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2569_112_2_17_22_17_11_590,(uint8*)"");
                        SCI_SLEEP(30); //wait a moment for APP handling APP_MN_UPDATE_SMS_STATE_CNF
                        // delete the SM identified by index
                        MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem1, del_index, MN_SMS_FREE_SPACE);
                    }
                    else
                    {
                        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                        s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
                    }

                    break;
                }
#ifndef _ULTRA_LOW_CODE_
                case AT_CMD_SMSC:
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                    s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
                    break;
                }
#endif
                default:
                    break;
            }

            break;
        }

        default:
        {
            //return the indication string
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
    }
    return S_ATC_DEFAULT_HANDLE;
}
//#endif
/*****************************************************************************/
//  Description : This function handles the signal of APP_MN_SMS_READ_CNF in the
//                text mode
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleTextModeSmsReadCnf(
                    MN_DUAL_SYS_E       dual_sys,
                    ATC_CONFIG_T        *atc_config_ptr,
                    MN_SMS_U_T          *sms_ptr,
                    MN_SMS_STATUS_E     sms_status,
                    MN_SMS_RECORD_ID_T  record_id
                    )
{
    ATC_SMS_STATE_E           sms_state = ATC_SMS_MAX_STATUS;
    uint8                     dest_addr_arr[MN_MAX_ADDR_BCD_LEN*2 + 2]={0};
    uint8                     sc_addr[MN_MAX_ADDR_BCD_LEN*2 + 1]={0};
    uint8                     *dest_addr = PNULL;
    BOOLEAN                   sc_flag = FALSE;
    MN_SMS_RECORD_ID_T        start_index = 0;
    uint8                     *temp_str = PNULL;
    uint8                     data_len = 0;
    uint8                     *data_str = PNULL;
    uint8                     tp_status = 0;
    BOOLEAN                   mo_flag = TRUE;
    uint32                    valid_period = 0;
    MN_SMS_TIME_STAMP_T       *time_stamp_ptr = PNULL;
    MN_SMS_ALPHABET_TYPE_E    alpha_type = MN_SMS_RESERVED_ALPHABET;
    MN_SMS_USER_DATA_HEAD_T   sms_user_data_head;
    MN_SMS_USER_VALID_DATA_T  sms_user_valid_data;

    dest_addr = dest_addr_arr;

    temp_str = g_rsp_str;
    //ATC_TRACE:"ATC: Enter the HandleTextModeSmsReadCnf routine"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3400_112_2_17_22_17_13_605,(uint8*)"");


    if (sms_status == MN_SMS_MT_READED ||
        sms_status == MN_SMS_MT_TO_BE_READ ||
        sms_status == MN_SMS_MT_SR_READED ||
        sms_status == MN_SMS_MT_SR_TO_BE_READ) //MT sms
    {
        if (sms_ptr->mt_sms_t.origin_addr_t.number_type == MN_NUM_TYPE_INTERNATIONAL)
        {
            dest_addr_arr[0] = '+';
            dest_addr = &dest_addr_arr[1];
        }
    }
    else //MO sms
    {
        if (sms_ptr->mo_sms_t.dest_addr_t.number_type == MN_NUM_TYPE_INTERNATIONAL)
        {
            dest_addr_arr[0] = '+';
            dest_addr = &dest_addr_arr[1];
        }
    }

    switch (sms_status)
    {
        case MN_SMS_FREE_SPACE:
        {
            // return the result code "sim fail"
            //ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
            //return;
            //SCI_TRACE_LOW:"ATC: SMS FREE SPACE!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3430_112_2_17_22_17_13_606,(uint8*)"");
            break;
        }

        // " REC READ"
        case MN_SMS_MT_READED:
        {
            sms_state = ATC_SMS_READ;
            TransformBcdToStr(sms_ptr->mt_sms_t.sc_addr_t.party_num,
                sms_ptr->mt_sms_t.sc_addr_t.num_len, sc_addr);
            TransformBcdToStr(sms_ptr->mt_sms_t.origin_addr_t.party_num,
                sms_ptr->mt_sms_t.origin_addr_t.num_len, dest_addr);
            MNSMS_DecodeUserDataEx(dual_sys,
                                sms_ptr->mt_sms_t.user_head_is_exist,
                                &(sms_ptr->mt_sms_t.dcs),
                                &(sms_ptr->mt_sms_t.user_data_t),
                                &sms_user_data_head,
                                &sms_user_valid_data
                                );

            data_len = sms_user_valid_data.length; //sms_ptr->mt_sms_t.user_data_t.length;
            data_str = sms_user_valid_data.user_valid_data_arr; //sms_ptr->mt_sms_t.user_data_t.user_data_arr;

            mo_flag  = FALSE;
            time_stamp_ptr = &(sms_ptr->mt_sms_t.time_stamp_t);
            alpha_type     = sms_ptr->mt_sms_t.dcs.alphabet_type;
            data_str[data_len] = '\0';
            break;
        }

            // "REC UNREAD"
        case MN_SMS_MT_TO_BE_READ:
        {
            sms_state = ATC_SMS_UNREAD;
            TransformBcdToStr(sms_ptr->mt_sms_t.sc_addr_t.party_num,
                sms_ptr->mt_sms_t.sc_addr_t.num_len, sc_addr);
            TransformBcdToStr(sms_ptr->mt_sms_t.origin_addr_t.party_num,
                sms_ptr->mt_sms_t.origin_addr_t.num_len, dest_addr);
            MNSMS_DecodeUserDataEx(dual_sys,
                                sms_ptr->mt_sms_t.user_head_is_exist,
                                &(sms_ptr->mt_sms_t.dcs),
                                &(sms_ptr->mt_sms_t.user_data_t),
                                &sms_user_data_head,
                                &sms_user_valid_data
                                );

            data_len = sms_user_valid_data.length; //sms_ptr->mt_sms_t.user_data_t.length;
            data_str = sms_user_valid_data.user_valid_data_arr; //sms_ptr->mt_sms_t.user_data_t.user_data_arr;

            mo_flag  = FALSE;
            time_stamp_ptr = &(sms_ptr->mt_sms_t.time_stamp_t);
            alpha_type     = sms_ptr->mt_sms_t.dcs.alphabet_type;
            if ((AT_CMD_CMGR == s_sms_record[dual_sys].cmd_id ||
                 AT_CMD_CMGL == s_sms_record[dual_sys].cmd_id) &&
                 s_sms_config[dual_sys].suss_flag)
            {
                MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem1, record_id, MN_SMS_MT_READED);
            }
            data_str[data_len] = '\0';
            break;
        }

            // "STO UNSENT"
        case MN_SMS_MO_TO_BE_SEND:
        {
            sms_state = ATC_SMS_UNSENT;
            sc_flag   = sms_ptr->mo_sms_t.sc_addr_present;
            if (sc_flag)
            {
                TransformBcdToStr(sms_ptr->mo_sms_t.sc_addr_t.party_num,
                    sms_ptr->mo_sms_t.sc_addr_t.num_len, sc_addr);
            }
            if (sms_ptr->mo_sms_t.dest_addr_present)
            {
                TransformBcdToStr(sms_ptr->mo_sms_t.dest_addr_t.party_num,
                    sms_ptr->mo_sms_t.dest_addr_t.num_len, dest_addr);
            }
            MNSMS_DecodeUserDataEx(dual_sys,
                                sms_ptr->mo_sms_t.user_head_is_exist,
                                &(sms_ptr->mo_sms_t.dcs),
                                &(sms_ptr->mo_sms_t.user_data_t),
                                &sms_user_data_head,
                                &sms_user_valid_data
                                );

            data_len     = sms_user_valid_data.length; //sms_ptr->mo_sms_t.user_data_t.length;
            data_str     = sms_user_valid_data.user_valid_data_arr; //sms_ptr->mo_sms_t.user_data_t.user_data_arr;

            if(MN_SMS_TP_VPF_RELATIVE_FORMAT == sms_ptr->mo_sms_t.time_format_e)
            {
                valid_period = sms_ptr->mo_sms_t.time_valid_period.time_second;
            }
            alpha_type   = sms_ptr->mo_sms_t.dcs.alphabet_type;
            data_str[data_len] = '\0';
            break;
        }

            // "STO SENT"
        case MN_SMS_MO_SR_NOT_REQUEST:
        case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
        case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
        case MN_SMS_MO_SR_RECEIVED_AND_STORED:
        {
            sms_state = ATC_SMS_SENT;
            sc_flag   = sms_ptr->mo_sms_t.sc_addr_present;
            if (sc_flag)
            {
                TransformBcdToStr(sms_ptr->mo_sms_t.sc_addr_t.party_num,
                    sms_ptr->mo_sms_t.sc_addr_t.num_len, sc_addr);
            }
            if (sms_ptr->mo_sms_t.dest_addr_present)
            {
                TransformBcdToStr(sms_ptr->mo_sms_t.dest_addr_t.party_num,
                    sms_ptr->mo_sms_t.dest_addr_t.num_len, dest_addr);
            }
            MNSMS_DecodeUserDataEx(dual_sys,
                                sms_ptr->mo_sms_t.user_head_is_exist,
                                &(sms_ptr->mo_sms_t.dcs),
                                &(sms_ptr->mo_sms_t.user_data_t),
                                &sms_user_data_head,
                                &sms_user_valid_data
                                );

            data_len     = sms_user_valid_data.length; //sms_ptr->mo_sms_t.user_data_t.length;
            data_str     = sms_user_valid_data.user_valid_data_arr; //sms_ptr->mo_sms_t.user_data_t.user_data_arr;

            if(MN_SMS_TP_VPF_RELATIVE_FORMAT == sms_ptr->mo_sms_t.time_format_e)
            {
                valid_period = sms_ptr->mo_sms_t.time_valid_period.time_second;
            }
            alpha_type   = sms_ptr->mo_sms_t.dcs.alphabet_type;
            data_str[data_len] = '\0';
            break;
        }

        //status report
        case MN_SMS_MT_SR_READED:
        case MN_SMS_MT_SR_TO_BE_READ:
        {
            if(MN_SMS_MT_SR_READED == sms_status)
            {
                sms_state = ATC_SMS_READ;
            }
            else
            {
                sms_state = ATC_SMS_UNREAD;
            }
            tp_status = sms_ptr->report_sms_t.tp_status;
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC:assert wrong sms_status!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3583_112_2_17_22_17_13_607,(uint8*)"");
            break;
        }
    }

    dest_addr = &dest_addr_arr[0];

    //ATC_TRACE:"ATC: The SMS data len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3590_112_2_17_22_17_13_608,(uint8*)"d", data_len);
    switch (s_sms_record[dual_sys].cmd_id)
    {
        case AT_CMD_CMGL:
        {
            //Handle the status report
            if(MN_SMS_MT_SR_READED == sms_status||MN_SMS_MT_SR_TO_BE_READ == sms_status)
            {
                sprintf((char*)temp_str,"%s: %d,\"%s\",%d",g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                    s_sms_record[dual_sys].cmgl_idex, s_sms_state_str[sms_state], tp_status);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
            }
            else if (MN_SMS_FREE_SPACE == sms_status)
            {
                sprintf((char*)temp_str,"%s: %d,%d,\"\",\"\",", g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                    s_sms_record[dual_sys].cmgl_idex, 0/*alpha_type*/);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
            }
            else
            {
                // The result string is "+CMGL: <index>,<dcs>, <state string>, <oa/da>,[valid period]"
                sprintf((char*)temp_str,"%s: %d,%d,\"%s\",\"%s\",", g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                    s_sms_record[dual_sys].cmgl_idex, alpha_type,s_sms_state_str[sms_state], dest_addr);
                temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];
                if (mo_flag)
                {
                    sprintf((char*)temp_str, "%ld", valid_period);
                }
                else
                {
                    FormatTimeStampStr(temp_str, time_stamp_ptr);
                }
                temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];
                // add the cr and cl to the tail of string
                sprintf((char*)temp_str,"%c%c",atc_config_ptr->s3_reg,atc_config_ptr->s4_reg);
                temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];
                if (alpha_type == MN_SMS_UCS2_ALPHABET)
                {
                    ConvertBinToHex(data_str, data_len, temp_str);

                    temp_str[data_len * 2] = '\0';

                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                    uint8    temp_len = (uint8)strlen((char*)g_rsp_str);

                    if ((data_len > 0) && (PNULL != data_str))
                    {
                        SCI_MEMCPY(temp_str, data_str, data_len);
                    }
                #ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                       SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + data_len));
                    else
                       MUX_Write(g_rsp_str, (temp_len + data_len),atc_config_ptr->current_link_id);
                #else
			#ifdef PPP_USB
                    SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + data_len),ATC_GetSioPort());
			#else
                    SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + data_len));
			#endif
                #endif
                    sprintf((char*)g_rsp_str, "%c%c",atc_config_ptr->s3_reg,atc_config_ptr->s4_reg);
                #ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    else
                        MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                #else

			#ifdef PPP_USB
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str),ATC_GetSioPort());
			#else
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
			#endif
                #endif
                }
            }

            //Read the next item
            if (ReadSpecSms(dual_sys,s_sms_config[dual_sys].mem1, &start_index,s_sms_record[dual_sys].state))
            {
                //SCI_TRACE_LOW:"ATC: Received  APP_MN_READ_SMS_CNF, text mode, sleep 30ms"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3665_112_2_17_22_17_13_609,(uint8*)"");
                SCI_SLEEP(30);//wait a moment for APP handling APP_MN_READ_SMS_CNF
                MNSMS_ReadSmsEx(dual_sys, s_sms_config[dual_sys].mem1, start_index);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_READ_SMS_CNF,0xff,ATC_NO_DOMAIN);
            }
            else
            {
                SCI_MEMSET(&s_sms_record[dual_sys], 0, sizeof(s_sms_record[dual_sys]));
                s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK ,0);
            }
            break;
        }

        case AT_CMD_CMGR:
        {
            //Handle the status report
            if(MN_SMS_MT_SR_READED == sms_status||MN_SMS_MT_SR_TO_BE_READ == sms_status)
            {
                sprintf((char*)temp_str,"%s: %d,%d,%d",g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr,
                 s_sms_record[dual_sys].cmgl_idex, sms_state, tp_status);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
            }
            else
            {

                if (mo_flag)
                {
                    // return +CMGR=<state>,<dcs>,<sc address>,<vp>
                    sprintf((char*)temp_str,"%s: \"%s\",%d,\"%s\",%ld",g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr,
                        s_sms_state_str[sms_state],alpha_type,dest_addr, valid_period);
                }
                else
                {
                    // return +CMGR=<state>,<dcs>,<sc address>,<ts>
                    sprintf((char*)temp_str,"%s: \"%s\",%d,\"%s\",", g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr,
                        s_sms_state_str[sms_state], alpha_type,dest_addr);
                    temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];
                    FormatTimeStampStr(temp_str, time_stamp_ptr);
                }
                temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];
                // add the cr and cl to the tail of string
                sprintf((char*)temp_str,"%c%c",atc_config_ptr->s3_reg,atc_config_ptr->s4_reg);

                temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];
                //ATC_TRACE:"ATC: the SMS len is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3710_112_2_17_22_17_13_610,(uint8*)"d", data_len);
                if (alpha_type == MN_SMS_UCS2_ALPHABET)
                {
                    ConvertBinToHex(data_str, data_len, temp_str);

                    temp_str[data_len * 2] = '\0';


                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                    uint8    temp_len = (uint8)strlen((char*)g_rsp_str);

                    if ((data_len > 0) && (PNULL != data_str))
                    {
                        SCI_MEMCPY(temp_str, data_str, data_len);
                    }
                #ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                        SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + data_len));
                    else
                        MUX_Write(g_rsp_str, (temp_len + data_len),atc_config_ptr->current_link_id);
                #else
			#ifdef PPP_USB
                    SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + data_len),ATC_GetSioPort());
			#else
                    SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + data_len));
			#endif

                #endif
                    sprintf((char*)g_rsp_str, "%c%c",atc_config_ptr->s3_reg,atc_config_ptr->s4_reg);
                #ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    else
                        MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                #else

			#ifdef PPP_USB
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str),ATC_GetSioPort());
			#else
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
			#endif
                #endif
                }
            }
            s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;

            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK ,0);
            break;
        }

        default:
            break;
    }
}

/*****************************************************************************/
//  Description : This function handles the signal of APP_MN_SMS_READ_CNF in the
//                PDU mode
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandlePduModeSmsReadCnf(
                    MN_DUAL_SYS_E       dual_sys,
                    ATC_CONFIG_T        *atc_config_ptr,
                    MN_SMS_U_T          *sms_ptr,
                    MN_SMS_STATUS_E     sms_status,
                    MN_SMS_RECORD_ID_T  record_id
                    )
{
    ATC_SMS_STATE_E         sms_state = ATC_SMS_MAX_STATUS;
    uint8                   tpdu_len = 0;
    uint8                   temp_str[20]={0};
    MN_SMS_RECORD_ID_T      start_index = 0;
#ifndef _ATC_ONLY
    uint8                   tp_status = 0;
#endif

#ifdef _MUX_ENABLE_
    //SCI_TRACE_LOW:"ATC: pdu read sms cnf, mux link id %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3780_112_2_17_22_17_14_611,(uint8*)"d", atc_config_ptr->current_link_id);
#endif

    switch (sms_status)
    {
        case MN_SMS_FREE_SPACE:
        {
            //ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
            //return ;
            //SCI_TRACE_LOW:"ATC: Read a free SMS space."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3789_112_2_17_22_17_14_612,(uint8*)"");
            break;
        }

        case MN_SMS_MT_READED:
        {
            sms_state = ATC_SMS_READ;
            MNSMS_GetHexSmsMtEx(dual_sys,&(sms_ptr->mt_sms_t), &tpdu_len, g_rsp_str, MAX_ATC_RSP_LEN);
            break;
        }

        case MN_SMS_MT_TO_BE_READ:
        {
            sms_state = ATC_SMS_UNREAD;
            MNSMS_GetHexSmsMtEx(dual_sys,&(sms_ptr->mt_sms_t), &tpdu_len, g_rsp_str, MAX_ATC_RSP_LEN);

            if ((AT_CMD_CMGR == s_sms_record[dual_sys].cmd_id ||
                 AT_CMD_CMGL == s_sms_record[dual_sys].cmd_id) &&
                 s_sms_config[dual_sys].suss_flag)
            {
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_READ_SMS_CNF,0xff,ATC_NO_DOMAIN);
                MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem1, record_id, MN_SMS_MT_READED);
            }
            break;
        }

        case MN_SMS_MO_TO_BE_SEND:
        {
            sms_state = ATC_SMS_UNSENT;
            MNSMS_GetHexSmsMoEx(dual_sys, &(sms_ptr->mo_sms_t), &tpdu_len, g_rsp_str, MAX_ATC_RSP_LEN);
            break;
        }

        case MN_SMS_MO_SR_NOT_REQUEST:
        case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
        case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
        case MN_SMS_MO_SR_RECEIVED_AND_STORED:
        {
            sms_state = ATC_SMS_SENT;
            MNSMS_GetHexSmsMoEx(dual_sys, &(sms_ptr->mo_sms_t), &tpdu_len, g_rsp_str, MAX_ATC_RSP_LEN);

            break;
        }

        //status report
        case MN_SMS_MT_SR_READED:
        case MN_SMS_MT_SR_TO_BE_READ:
        {
            if(MN_SMS_MT_SR_READED == sms_status)
            {
                sms_state = ATC_SMS_READ;
            }
            else
            {
                sms_state = ATC_SMS_UNREAD;
            }
        #ifdef _ATC_ONLY
            tpdu_len = sms_ptr->report_sms_t.msg_data.length;
            MNSMS_AddScAddrEx(dual_sys,&sms_ptr->report_sms_t.sc_addr_t, &(sms_ptr->report_sms_t.msg_data));
            ConvertBinToHex(sms_ptr->report_sms_t.msg_data.l3_msg, (uint8)sms_ptr->report_sms_t.msg_data.length, g_rsp_str);
            g_rsp_str[sms_ptr->report_sms_t.msg_data.length*2] = '\0';
        #else
            tp_status = sms_ptr->report_sms_t.tp_status;
        #endif
            break;
        }

        default:
        {
            SCI_ASSERT(FALSE); /*assert verified check FALSE case*/
            break;
        }
    }

    switch (s_sms_record[dual_sys].cmd_id)
    {
        case AT_CMD_CMGL:
        {
            //Handle the status report
            /*
            if(MN_SMS_MT_SR_READED == sms_status||MN_SMS_MT_SR_TO_BE_READ == sms_status)
            {
                sprintf((char*)temp_str,"%s: %d,%d,%d",g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                    s_sms_record[dual_sys].cmgl_idex, sms_state, tp_status);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
            }
            else*/
            if (MN_SMS_FREE_SPACE == sms_status)
            {
                sprintf((char*)temp_str,"%s: %d, %d,\"\",%d",g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                    s_sms_record[dual_sys].cmgl_idex, 0/*sms_state*/, 0/*tpdu_len*/);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
            }
            else
            {
                sprintf((char*)temp_str,"%s: %d, %d,\"\",%d",g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                    s_sms_record[dual_sys].cmgl_idex, sms_state, tpdu_len);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            //Read the next item
            if (ReadSpecSms(dual_sys,s_sms_config[dual_sys].mem1, &start_index,s_sms_record[dual_sys].state))
            {
                //SCI_TRACE_LOW:"ATC: Received  APP_MN_READ_SMS_CNF, pdu mode, sleep 30ms"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3892_112_2_17_22_17_14_613,(uint8*)"");
                SCI_SLEEP(30);//wait a moment for APP handling APP_MN_READ_SMS_CNF
                MNSMS_ReadSmsEx(dual_sys, s_sms_config[dual_sys].mem1, start_index);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_READ_SMS_CNF,0xff,ATC_NO_DOMAIN);
            }
            else
            {
                SCI_MEMSET(&s_sms_record[dual_sys], 0, sizeof(s_sms_record[dual_sys]));
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK ,0);
                s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
            }
            break;
        }

        case AT_CMD_CMGR:
        {
            //Handle the status report
            /*
            if(MN_SMS_MT_SR_READED == sms_status||MN_SMS_MT_SR_TO_BE_READ == sms_status)
            {
                sprintf((char*)temp_str,"%s: %d,%d,%d",g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr,
                    s_sms_record[dual_sys].cmgl_idex, sms_state, tp_status);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
            }
            else*/
            if (MN_SMS_FREE_SPACE == sms_status)
            {
                sprintf((char*)temp_str,"%s: %d,\"\",%d",g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr,
                    0/*sms_state*/, 0/*tpdu_len*/);

                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
            }
            else
            {
                   uint32 str_len = 0;
                   sprintf((char*)temp_str,"%s: %d,\"\",%d\r\n", g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr, sms_state, tpdu_len);
                         str_len = strlen((char*)temp_str);
                memmove((void*)(((char*)g_rsp_str) + str_len),(void*)g_rsp_str,(strlen((char*)g_rsp_str)+1));
                SCI_MEMCPY((void*)g_rsp_str,temp_str,str_len);
                //sprintf((char*)temp_rsp_str,"%s: %d,\"\",%d\r\n%s", g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr, sms_state, tpdu_len, g_rsp_str);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK ,0);
            break;
        }

        default:
            break;
    }
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and send the
//                result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessReadSmsCnf(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_READ_SMS_CNF_T    *sig_ptr
                )
{
    MN_DUAL_SYS_E dual_sys;

    dual_sys = sig_ptr->dual_sys;

    SCI_TRACE_LOW("ATC: received the APP_MN_READ_SMS_CNF signal");
    // check if the operation is successful
    if (MN_SMS_OPERATE_SUCCESS != sig_ptr->cause)
    {
        // return the error code "SIM fail"
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }
#if !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_UIX8910_ENABLE_)// reduce atc code Nemge
    if (s_sms_record[dual_sys].cmd_id == AT_CMD_CMSS)
    {
        HandleCmssCmd(dual_sys,sig_ptr, atc_config_ptr);
        return S_ATC_DEFAULT_HANDLE;
    }
#endif
    if (atc_config_ptr->atc_config_nv.cmgf_flag)
    {
        HandleTextModeSmsReadCnf(dual_sys,atc_config_ptr, &(sig_ptr->sms_t_u), sig_ptr->status, sig_ptr->record_id);
    }
    else
    {
        HandlePduModeSmsReadCnf(dual_sys,atc_config_ptr, &(sig_ptr->sms_t_u), sig_ptr->status, sig_ptr->record_id);
    }
    return S_ATC_DEFAULT_HANDLE;
}
#endif
/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and send the
//                result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSendSmsCnf(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_SEND_SMS_CNF_T    *sig_ptr
                )
{
    MN_DUAL_SYS_E dual_sys;

    dual_sys = sig_ptr->dual_sys;

#ifdef _MUX_ENABLE_
    //SCI_TRACE_LOW:"ATC:ATC_ProcessSendSmsCnf ignore result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2478_112_2_17_22_17_11_588,(uint8*)"d", g_ignore_sms_result);
    g_atc_sms_flag = FALSE;
    if(g_ignore_sms_result)
    {
        g_ignore_sms_result = FALSE;
        return S_ATC_DEFAULT_HANDLE;
    }
#endif
    switch (sig_ptr->cause)
    {
        case MN_SMS_OPERATE_SUCCESS:
        {
            switch (s_sms_record[dual_sys].cmd_id)
            {
            #if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_UIX8910_ENABLE_)// reduce atc code Nemge
                case AT_CMD_CMSS:
                {
                    sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CMSS].cmd_name->str_ptr, sig_ptr->tp_mr);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem2, s_sms_record[dual_sys].index, MN_SMS_MO_SR_NOT_REQUEST);
                    break;
                }

                case AT_CMD_CMGS:
                {
                    sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CMGS].cmd_name->str_ptr, sig_ptr->tp_mr);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }
                #if 0
                case AT_CMD_SCMGS:
                {
                    sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SCMGS].cmd_name->str_ptr, sig_ptr->tp_mr);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }
                #endif
		#endif
                default:
                    return S_ATC_DEFAULT_HANDLE;
            }/*lint !e764 */

            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0); /*lint !e527 */
            break;
        }

    default:
        //SCI_TRACE_LOW:"ATC: Receive the error status: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2523_112_2_17_22_17_11_589,(uint8*)"d", sig_ptr->cause);
    #ifdef _ATC_ONLY
        HandleSmsCause(atc_config_ptr, sig_ptr->cause, sig_ptr->rl_cause);
    #else
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
    #endif
        break;
    }
    SCI_MEMSET(&s_sms_record[dual_sys], 0, sizeof(s_sms_record[dual_sys]));
    s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function get the specific storage's total amount and used amount.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN GetStorageInfo(
                                    MN_DUAL_SYS_E dual_sys,
                     MN_SMS_STORAGE_E        storage,   // in--get the where store sms's status, ME or SIM
                     MN_SMS_RECORD_ID_T *total_ptr, // out--the max number of sms can saved in ME or SIM
                     MN_SMS_RECORD_ID_T *used_ptr,
                     BOOLEAN                   *unread_flag
                     )
{
#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_) || defined(_ATC_UIX8910_ENABLE_)
    ERR_MNSMS_CODE_E    mn_status   = ERR_MNSMS_NONE;
    MN_SMS_STATUS_E     *status_ptr = PNULL;
    int32   i = 0;

    SCI_ASSERT(PNULL != total_ptr); /*assert verified*/

    *used_ptr    = 0;
    *unread_flag = FALSE;

    mn_status = GetSmsStatusArr(total_ptr, &status_ptr, dual_sys, storage);

    if (ERR_MNSMS_PTR_IS_NULL == mn_status)
    {
        return (ERR_MEMORY_FAILURE);
    }
    else
    {
        if (ERR_MNSMS_NONE != mn_status)
        {
            SCI_FREE(status_ptr);
            return FALSE;
        }
    }

    //Get the information about SMS
    for (i=0; i<*total_ptr; i++)
    {
        if (MN_SMS_FREE_SPACE != status_ptr[i])
        {
            *used_ptr = *used_ptr + 1;
        }
        if (MN_SMS_MT_TO_BE_READ == status_ptr[i])
        {
            *unread_flag = TRUE;
        }
    }
    SCI_FREE(status_ptr);
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : This function get the specific sms information
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  ReadSpecSms(
                MN_DUAL_SYS_E           dual_sys,
                MN_SMS_STORAGE_E        storage,
                MN_SMS_RECORD_ID_T      *index_ptr,
                ATC_SMS_STATE_E         sms_state
                )
{
    int32               i           = 0;
    uint16              total_num   = 0;
    ERR_MNSMS_CODE_E    mn_status   = ERR_MNSMS_NONE;
    MN_SMS_STATUS_E     *status_ptr = PNULL;

    mn_status = GetSmsStatusArr(&total_num, &status_ptr, dual_sys, storage);

    if (ERR_MNSMS_PTR_IS_NULL == mn_status)
    {
        return (ERR_MEMORY_FAILURE);
    }
    else
    {
        if (ERR_MNSMS_NONE != mn_status)
        {
            SCI_FREE(status_ptr);
            return FALSE;
        }
    }

    //Get the defined SMS and change the index
    for (i = s_sms_record[dual_sys].cmgl_idex; i < total_num; i++)
    {
        switch (sms_state)
        {
            case ATC_SMS_UNREAD:
            {
                if (MN_SMS_MT_TO_BE_READ == status_ptr[i]||MN_SMS_MT_SR_TO_BE_READ == status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }
                break;
            }

            case ATC_SMS_READ:
            {
                if (MN_SMS_MT_READED == status_ptr[i]||MN_SMS_MT_SR_READED == status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }
                break;
            }

            case ATC_SMS_UNSENT:
            {
                if (MN_SMS_MO_TO_BE_SEND == status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }
                break;
            }

            case ATC_SMS_SENT:
            {
                if ((MN_SMS_MO_SR_NOT_REQUEST            == status_ptr[i]) ||
                    (MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED == status_ptr[i]) ||
                    (MN_SMS_MO_SR_RECEIVED_NOT_STORE     == status_ptr[i]) ||
                    (MN_SMS_MO_SR_RECEIVED_AND_STORED    == status_ptr[i])
                    )
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }
                break;
            }

            case ATC_SMS_ALL:
            {
                if (MN_SMS_FREE_SPACE != status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }

                break;
            }

            default:
            {
                //SCI_TRACE_LOW:"ATC: the ASSERT sms_state"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2764_112_2_17_22_17_11_591,(uint8*)"");
                break;
            }
        }
    }

    SCI_FREE(status_ptr);
    s_sms_record[dual_sys].cmgl_idex = i + 1;
    //SCI_TRACE_LOW:"ATC: the current cmgl index: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2772_112_2_17_22_17_11_592,(uint8*)"d",s_sms_record[dual_sys].cmgl_idex);
    return FALSE;
}
#endif   // nemge
/*****************************************************************************/
//  Description : This function compare the input string and the sm storage
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN   CompareSmStorage(
                        char   *mem_str,          // in: pointer to the storage
                        uint8  *storage_id_ptr   // out: identify the storage id
                        )
{
#ifdef _ATC_SYNC_TOOL_
    int32          i;

    for (i=0; i<MN_SMS_STORAGE_ALL; i++)
    {
        if (!strcmp(mem_str, s_sms_mem_str[i]))
        {
            *storage_id_ptr = (uint8)i;
            return TRUE;
        }
    }
    return FALSE;
#else
    return FALSE;
#endif
 }
#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_) || defined(_ATC_UIX8910_ENABLE_) // nemge
/*****************************************************************************/
//  Description : This function is used to get the SMS state of string type
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetSmsState(
                          uint8             *state_str,     //IN:
                          ATC_SMS_STATE_E   *sms_state_ptr  //OUT:
                          )
{
    ATC_SMS_STATE_E     i       = ATC_SMS_UNREAD;
    BOOLEAN             result  = FALSE;

    SCI_ASSERT(PNULL != state_str); /*assert verified*/


    // search the SMS state string
    for (i=ATC_SMS_UNREAD; i<ATC_SMS_MAX_STATUS; i++)
    {
        //check if it is one SMS state
        if (0 == strcmp((char*)state_str, s_sms_state_str[i]))
        {
            *sms_state_ptr  = i;
            result          = TRUE;
            break;
        }
    }
    return (result);
}

/*****************************************************************************/
//  Description : This function is used to handle the response of the at+cmss
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleCmssCmd(
                MN_DUAL_SYS_E            dual_sys,
                APP_MN_READ_SMS_CNF_T    *sig_ptr,
                ATC_CONFIG_T             *atc_config_ptr
                )
{
    // the state of the specfic sms is right
    if (MN_SMS_MO_TO_BE_SEND == sig_ptr->status)
    {
        if (s_sms_record[dual_sys].dest_num_flag)
        {
            sig_ptr->sms_t_u.mo_sms_t.dest_addr_present =TRUE;
            sig_ptr->sms_t_u.mo_sms_t.dest_addr_t = s_sms_record[dual_sys].dest_num;
        }
        else if (!sig_ptr->sms_t_u.mo_sms_t.dest_addr_present)
        {
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
            s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
        }
        MNSMS_AppSendSmsEx(dual_sys, &sig_ptr->sms_t_u.mo_sms_t, MN_SMS_NO_STORED, MN_SMS_GSM_PATH, FALSE);
    }
    else
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
        s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
    }
}

/*****************************************************************************/
//  Description : This function is used to format the time stamp string
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void FormatTimeStampStr(
                    uint8                 *ts_str,
                    MN_SMS_TIME_STAMP_T   *time_stamp_ptr
                    )
{
    SCI_ASSERT(PNULL != ts_str); /*assert verified*/
    SCI_ASSERT(PNULL != time_stamp_ptr); /*assert verified*/

    sprintf((char*)ts_str, "\"%02d/%02d/%02d,%02d:%02d:%02d+%02d\"", time_stamp_ptr->year,
            time_stamp_ptr->month,time_stamp_ptr->day,time_stamp_ptr->hour,time_stamp_ptr->minute,
            time_stamp_ptr->second,time_stamp_ptr->timezone);
}

/*****************************************************************************/
//  Description : get the SMS status
//  Global resource dependence :
//  Author:  Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL ERR_MNSMS_CODE_E GetSmsStatusArr(                                     //RETURN:
                                       uint16            *total_num_ptr,    //OUT:
                                       MN_SMS_STATUS_E   **status_ptr,      //OUT:
                                       MN_DUAL_SYS_E     dual_sys,
                                       MN_SMS_STORAGE_E  storage
                                       )
{
    ERR_MNSMS_CODE_E    mn_status = ERR_MNSMS_NONE;
    uint16              mn_sms_num   = 0;
    uint16              used_num     = 0;

    SCI_ASSERT(PNULL != total_num_ptr);/*assert verified*/

    *total_num_ptr = 0;
    MNSMS_GetSmsNumEx(dual_sys, storage, &mn_sms_num, &used_num);

    *status_ptr = SCI_ALLOC_BASE(mn_sms_num * sizeof (MN_SMS_STATUS_E));

    mn_status = MNSMS_GetSmsStatusEx(dual_sys, storage, total_num_ptr, *status_ptr);

    return (mn_status);
}

/*****************************************************************************/
//  Description : This function is used to delete the specific SMS according to
//                the SMS state.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteSpecSms(
                MN_DUAL_SYS_E           dual_sys,
                MN_SMS_STORAGE_E        storage,
                MN_SMS_RECORD_ID_T      *index_ptr,
                ATC_SMS_DEL_STATE_E     del_state
                )
{
    int32               i = 0;
    uint16              total_num   = 0;
    ERR_MNSMS_CODE_E    mn_status   = ERR_MNSMS_NONE;
    MN_SMS_STATUS_E     *status_ptr = PNULL;

    mn_status = GetSmsStatusArr(&total_num, &status_ptr, dual_sys, storage);

    if (ERR_MNSMS_PTR_IS_NULL == mn_status)
    {
        return (ERR_MEMORY_FAILURE);
    }
    else
    {
        if (ERR_MNSMS_NONE != mn_status)
        {
            SCI_FREE(status_ptr);
            return FALSE;
        }
    }

    for (i=s_sms_record[dual_sys].cmgl_idex; i<total_num; i++)
    {
        switch (del_state)
        {
            case ATC_DEL_INDEX:
            {
                SCI_FREE(status_ptr);
                return FALSE;
            }

            case ATC_DEL_ALL_READ:
            {
                if (MN_SMS_MT_READED == status_ptr[i]||MN_SMS_MT_SR_READED == status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }
                break;
            }

            case ATC_DEL_ALL_READ_SENT:
            {
                if ((MN_SMS_MT_READED                    == status_ptr[i]) ||
                    (MN_SMS_MT_SR_READED                  == status_ptr[i])||
                    (MN_SMS_MO_SR_NOT_REQUEST            == status_ptr[i]) ||
                    (MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED == status_ptr[i]) ||
                    (MN_SMS_MO_SR_RECEIVED_NOT_STORE     == status_ptr[i]) ||
                    (MN_SMS_MO_SR_RECEIVED_AND_STORED    == status_ptr[i]))
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }
                break;
            }

            case ATC_DEL_ALL_READ_SENT_UNSENT:
            {
                if ((!((MN_SMS_MT_TO_BE_READ == status_ptr[i])||(MN_SMS_MT_SR_TO_BE_READ == status_ptr[i]))) &&
                    MN_SMS_FREE_SPACE     != status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }
                break;
            }

            case ATC_DEL_ALL:
            {
                if (MN_SMS_FREE_SPACE != status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }
                break;
            }

            default:
            {
                //SCI_TRACE_LOW:"ATC:assert wrong PduModeSmsReadCnf!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4243_112_2_17_22_17_14_616,(uint8*)"");
                break;
            }

        }
    }
    SCI_FREE(status_ptr);

    s_sms_record[dual_sys].cmgl_idex = i + 1;
    //SCI_TRACE_LOW:"ATC:the current cmgl index: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4252_112_2_17_22_17_14_617,(uint8*)"d",s_sms_record[dual_sys].cmgl_idex);
    return FALSE;
}

/*****************************************************************************/
//  Description : This function converts the relative period to the absolute period
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL uint32   GetValidPeriodSeconds(
                            uint8      vp
                            )
{
    uint32        seconds_val;

    if (vp <= ATC_SMS_MINUTES_VAL)
    {
        // up to 12 hours
        seconds_val = (vp + 1 ) * 5 * 60;
    }
    else if (vp <= ATC_SMS_HOURS_VAL)
    {
        // 12hours + (vp-143)*30minutes
        seconds_val = 12 * 60 * 60 +(vp - ATC_SMS_MINUTES_VAL) * 30 * 60;
    }
    else if (vp <= ATC_SMS_DAYS_VAL)
    {
        // (vp -166) * 1day
        seconds_val = (vp - ATC_SMS_HOURS_VAL + 1) * 24 * 60 * 60;
    }
    else
    {
        // (vp - 192) * 1week
        seconds_val = (vp - ATC_SMS_DAYS_VAL + 4) * 7 * 24 * 60 * 60;
    }
    return seconds_val;
}

/*****************************************************************************/
//  Description : This function handles the cause from the sms signal
//  Global resource dependence :
//  Author:       Sunsome Ju
//  Note:
/*****************************************************************************/
LOCAL void HandleSmsCause(
                    ATC_CONFIG_T       *atc_config_ptr,
                    MN_SMS_CAUSE_E     cause,
                    MN_SMS_MO_FAIL_CAUSE_E rl_cause
                    )
{
    uint8       *rsp_buf_ptr  = g_rsp_str;
    uint8       cr_val;
    uint8       lf_val;

    cr_val = atc_config_ptr->s3_reg;
    lf_val = atc_config_ptr->s4_reg;

    g_rsp_str[0] = cr_val;
    g_rsp_str[1] = lf_val;
    rsp_buf_ptr = &g_rsp_str[2];

    switch (cause)
    {
        case MN_SMS_OPERATE_SUCCESS:
        {
            //not show any error
            break;
        }

        case MN_SMS_SEND_FAILURE:   // send fail
            {
                if(MN_SMS_CAUSE_PS_ERROR == rl_cause)
                {
                    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_500);
                }
                else
                {
                    sprintf((char*)rsp_buf_ptr, "%s%d%c%c", "+CMS ERROR: ",
                        (uint8)rl_cause, cr_val, lf_val);

                    //SCI_TRACE_LOW:"ATC:BuildSmsCodeRsp,rl_cause is %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4541_112_2_17_22_17_15_620,(uint8*)"d", (uint8)rl_cause);
                    //SCI_TRACE_LOW:"ATC:BuildSmsCodeRsp,The string=%s"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4542_112_2_17_22_17_15_621,(uint8*)"s", g_rsp_str);
                #ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id ==ATC_DEBUG_LINK)
                    {
                        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    }
                    else
                    {
                        MUX_Write(g_rsp_str, strlen((char*)g_rsp_str), atc_config_ptr->current_link_id);
                    }
                #else
			#ifdef PPP_USB
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str),ATC_GetSioPort());
			#else
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
			#endif
                #endif
                }
            break;
        }

        case MN_SMS_MN_BUSY:   // the MN is busy
        case MN_SMS_SIM_UNKNOW_FAILED:
        case MN_SMS_UNSPEC_ERROR:   // the unspecific error
        case MN_SMS_DEFAULT_PARAM_ERROR:   // the default param which uplayer want to use not set
        default:
        {
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_500);
            break;
        }
    }
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_MEM_FULL_IND signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSmsReadyInd(
                ATC_CONFIG_T                      *atc_config_ptr,
                APP_MN_SMS_READY_IND_T            *sig_ptr
                )
{
    s_sms_config[sig_ptr->dual_sys].sms_is_ready = TRUE;
    //Read the SC from SIM
    if (ERR_MNSMS_NONE  != MNSMS_GetScAddrEx(sig_ptr->dual_sys, &g_sc_addr[sig_ptr->dual_sys]))
    {
        //SCI_TRACE_LOW:"ATC: Get sc failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2958_112_2_17_22_17_12_600,(uint8*)"");
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the signal of APP_MN_SMS_IND in the
//                PDU mode
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandlePduModeSmsInd(
                MN_DUAL_SYS_E         dual_sys,
                ATC_CONFIG_T          *atc_config_ptr,
                MN_SMS_MT_SMS_T       *mt_sms_ptr,
                MN_SMS_RECORD_ID_T    record_id
                )
{
#if defined(_ATC_SYNC_TOOL_) ||defined(_ATC_UIX8910_ENABLE_)
    uint8       pdu_len     = 0;
    uint16      copy_len    = 0;
    uint8       *pdu        = PNULL;
    uint8       *temp_str   = PNULL;

    SCI_MEMSET((uint8 *)g_rsp_str, 0, sizeof(g_rsp_str));

    /* Since the pdu length is not determined yet, the Auther used the global string g_rsp_str for temoprary use,
             * it is large enough (1K) and not used in this time.  But it is not easily comprehensive. Ryan 20071029 */
    temp_str = g_rsp_str;

    MNSMS_GetHexSmsMtEx(dual_sys,mt_sms_ptr, &pdu_len, temp_str, MAX_ATC_RSP_LEN);

    /* Here temp_str actually holds the content of PDU, but it needs to be copied out.  Ryan 20071029 */
    pdu = SCI_ALLOC_BASEZ(strlen((char *)g_rsp_str)+1);

    if (pdu == PNULL)// alloc memory fail.
    {
        //SCI_TRACE_LOW:"ATC:PDU allocation fail,exit +CMT!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3974_112_2_17_22_17_14_614,(uint8*)"");
        return;
    }

    copy_len = strlen((char *)g_rsp_str) + 1;
    SCI_MEMCPY(pdu, temp_str, copy_len);

    SCI_MEMSET((uint8 *)g_rsp_str, 0, sizeof(g_rsp_str));

#ifdef _MUX_REDUCED_MODE_
    sprintf((char*)g_rsp_str, "+CMT: %d,,%d%c%c%s", record_id, pdu_len,
             atc_config_ptr->s3_reg, atc_config_ptr->s4_reg, pdu);
#else
    sprintf((char*)g_rsp_str, "+CMT:,%d%c%c%s", pdu_len,
             atc_config_ptr->s3_reg, atc_config_ptr->s4_reg, pdu);
#endif

    SCI_FREE(pdu);

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#endif
}

/*****************************************************************************/
//  Description : This function handles the signal of APP_MN_SMS_IND in the
//                Text mode
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleTextModeSmsInd(
                MN_DUAL_SYS_E         dual_sys,
                ATC_CONFIG_T          *atc_config_ptr,
                MN_SMS_MT_SMS_T       *mt_sms_ptr,
                MN_SMS_RECORD_ID_T    record_id
                )
{
#if defined(_ATC_SYNC_TOOL_) ||defined(_ATC_UIX8910_ENABLE_)
    uint8                     dest_addr_arr[MN_MAX_ADDR_BCD_LEN*2 + 2] = {0};
    uint8                     sc_addr_arr[MN_MAX_ADDR_BCD_LEN*2 + 2] = {0};
    uint8                     *dest_addr = PNULL;
    uint8                     *sc_addr = PNULL;
    uint8                     *temp_str = PNULL;
    uint8                     data_len;
    uint8                     *data_str;
    MN_SMS_USER_DATA_HEAD_T   user_data_head;
    MN_SMS_USER_VALID_DATA_T  user_valid_data;

    temp_str = g_rsp_str;

    sc_addr = sc_addr_arr;
    dest_addr = dest_addr_arr;

    if (mt_sms_ptr->sc_addr_t.number_type == MN_NUM_TYPE_INTERNATIONAL)
    {
        sc_addr_arr[0] = '+';
        sc_addr = &sc_addr_arr[1];
    }
    if (mt_sms_ptr->origin_addr_t.number_type == MN_NUM_TYPE_INTERNATIONAL)
    {
        dest_addr_arr[0] = '+';
        dest_addr = &dest_addr_arr[1];
    }


    // get the origin address and the sc address
    TransformBcdToStr(mt_sms_ptr->sc_addr_t.party_num, mt_sms_ptr->sc_addr_t.num_len, sc_addr);
    TransformBcdToStr(mt_sms_ptr->origin_addr_t.party_num, mt_sms_ptr->origin_addr_t.num_len, dest_addr);

    MNSMS_DecodeUserDataEx(dual_sys,
                        mt_sms_ptr->user_head_is_exist,
                        &(mt_sms_ptr->dcs),
                        &(mt_sms_ptr->user_data_t),
                        &user_data_head,
                        &user_valid_data
                        );


    // get the data content
    data_len = user_valid_data.length;          //mt_sms_ptr->user_data_t.length;
    data_str = user_valid_data.user_valid_data_arr;   //mt_sms_ptr->user_data_t.user_data_arr;
    data_str[data_len] = '\0';

    sprintf((char*)temp_str,"+CMT: \"%s\",\"%s\",", dest_addr_arr, sc_addr_arr);

    temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];

    sprintf((char*)temp_str, "\"%02d/%02d/%02d,%02d:%02d:%02d+%02d\",%d%c%c", mt_sms_ptr->time_stamp_t.year,
            mt_sms_ptr->time_stamp_t.month,mt_sms_ptr->time_stamp_t.day,mt_sms_ptr->time_stamp_t.hour,mt_sms_ptr->time_stamp_t.minute,
            mt_sms_ptr->time_stamp_t.second, mt_sms_ptr->time_stamp_t.timezone,data_len,
            atc_config_ptr->s3_reg,atc_config_ptr->s4_reg);
    temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];
    //Handle when MT is UCS2 alphabet
    if (MN_SMS_UCS2_ALPHABET == mt_sms_ptr->dcs.alphabet_type)
    {
        ConvertBinToHex(data_str, data_len, temp_str);

        temp_str[data_len * 2] = '\0';


        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    //handle when MT is SMS alphabet
    else
    {
        uint16    temp_len = (uint16)strlen((char*)g_rsp_str);
        //SCI_TRACE_LOW:"ATC: The temp_len is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4076_112_2_17_22_17_14_615,(uint8*)"d", temp_len);

        SCI_MEMCPY(temp_str, data_str, data_len);
    #ifdef _MUX_ENABLE_
        if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
            SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + data_len));
        else
            MUX_Write(g_rsp_str, (temp_len + data_len),atc_config_ptr->current_link_id);
    #else
	#ifdef PPP_USB
        SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + data_len),ATC_GetSioPort());
	#else
        SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + data_len));
	#endif

    #endif
        sprintf((char*)g_rsp_str, "%c%c",atc_config_ptr->s3_reg,atc_config_ptr->s4_reg);
    #ifdef _MUX_ENABLE_
        if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
            SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
        else
            MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
    #else
	#ifdef PPP_USB
        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str),ATC_GetSioPort());
	#else
        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
	#endif
    #endif
    }
#endif
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_IND signal and send the
//                result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSmsInd(
                    ATC_CONFIG_T       *atc_config_ptr,
                    APP_MN_SMS_IND_T   *sig_ptr
                    )
{
#if defined(_ATC_SYNC_TOOL_) ||defined(_ATC_UIX8910_ENABLE_)

    MN_SMS_RECORD_ID_T   index = 0;

    MN_DUAL_SYS_E dual_sys;

    dual_sys = sig_ptr->dual_sys;
    SCI_TRACE_LOW("ATC: ATC_ProcessSmsInd s_sms_config[dual_sys].mt_ind = %d\n", s_sms_config[dual_sys].mt_ind);

    switch (s_sms_config[dual_sys].mt_ind)
    {
        case ATC_MT_NO_DELIVER:
        {
            return S_ATC_DEFAULT_HANDLE;
        }

        // +CMTI: <index>
        case ATC_MT_CMTI:
        {
            index = (sig_ptr->record_id + 1);
            if(MN_SMS_STORAGE_SIM == sig_ptr->storage)
            {
                sprintf((char*)g_rsp_str, "+CMTI: \"SM\",%d", index);
            }
            else
            {
                sprintf((char*)g_rsp_str, "+CMTI: \"ME\",%d", index);
            }
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        // +CMT:<length> CR LF <pdu>
        default:
        {
            if (atc_config_ptr->atc_config_nv.cmgf_flag)
            {
                HandleTextModeSmsInd(dual_sys,atc_config_ptr, &(sig_ptr->sms_t), sig_ptr->record_id+1);
            }
            else
            {
                HandlePduModeSmsInd(dual_sys,atc_config_ptr, &(sig_ptr->sms_t), sig_ptr->record_id+1);
            }
            break;
        }
    }
#endif
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_UPDATE_SMS_STATUS_CNF signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessWriteSmsCnf(
                ATC_CONFIG_T                      *atc_config_ptr,
                APP_MN_WRITE_SMS_CNF_T            *sig_ptr
                )
{
#if !defined(_ATC_SYNC_TOOL_)

    uint16 cmd_id = s_sms_record[sig_ptr->dual_sys].cmd_id;


    //SCI_TRACE_LOW:"ATC: enter into ATC_ProcessWriteSmsCnf"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2789_112_2_17_22_17_12_593,(uint8*)"");

    switch(cmd_id)
    {
        case AT_CMD_CMGW:
        #if 0
        case AT_CMD_SCMGW:
        #endif
        {
            //If success,return the indication string
            if (sig_ptr->cause == MN_SMS_OPERATE_SUCCESS)
            {
                sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[cmd_id].cmd_name->str_ptr, (sig_ptr->record_id + 1));
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
            }
            //If not sucess,return the error.
            else
            {
                //SCI_TRACE_LOW:"ATC:WRITE error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2807_112_2_17_22_17_12_594,(uint8*)"");
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,0);
            }

            s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
            break;
        }

    #ifndef ATC_ORANGE_ENABLE
        case AT_CMD_SFSMSM:
        {
            BOOLEAN fill_flag = TRUE;

            switch(sig_ptr->cause)
            {
                case MN_SMS_OPERATE_SUCCESS:
                {
                    if (MN_SMS_STORAGE_ME == sig_ptr->storage)
                    {
                        fill_flag = Atc_FillMtMoSmsEx(atc_config_ptr, MN_SMS_STORAGE_ME, sig_ptr->dual_sys);
                    }
                    else if(MN_SMS_STORAGE_SIM== sig_ptr->storage)
                    {
                        fill_flag = Atc_FillMtMoSmsEx(atc_config_ptr, MN_SMS_STORAGE_SIM, sig_ptr->dual_sys);
                    }
                    else
                    {
                        fill_flag = FALSE;
                    }
                    break;
                }

                case MN_SMS_ME_MEM_FULL:
                {
                    // to update sms storage information
                    (void)Atc_UpdateSmsMemSts(atc_config_ptr, &s_sms_fill_mem);
                    if ((MN_SMS_MEM_AVAIL == s_sms_fill_mem.sim_mem_sts[sig_ptr->dual_sys])
                        && (atc_config_ptr->sim_flag[sig_ptr->dual_sys]))
                    {
                        fill_flag = Atc_FillMtMoSmsEx(atc_config_ptr, MN_SMS_STORAGE_SIM, sig_ptr->dual_sys);
                    }
                    break;
                }

                case MN_SMS_SIM_MEM_FULL:
                {
                    // to update sms storage information
                    (void)Atc_UpdateSmsMemSts(atc_config_ptr, &s_sms_fill_mem);
                    if (MN_SMS_MEM_AVAIL == s_sms_fill_mem.me_mem_sts)
                    {
                        fill_flag = Atc_FillMtMoSmsEx(atc_config_ptr, MN_SMS_STORAGE_ME, sig_ptr->dual_sys);
                    }
                    break;
                }

                default :
                {
                    fill_flag = FALSE;
                    break;
                }
            }

            if (!fill_flag)
            {
                //SCI_TRACE_LOW:"ATC:WRITE error, Stopping Filling SMS Storage!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2868_112_2_17_22_17_12_595,(uint8*)"");
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,0);

                Atc_FreeMemForSfsmsm(&s_sms_fill_mem);
                s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
            }
            break;
        }
    #endif

        default:
            break;
    }

    //SCI_SLEEP(30); //wait a moment for APP handling APP_MN_WRITE_SMS_CNF;
#elif defined(_ATC_UIX8910_ENABLE_)
    uint16 cmd_id = s_sms_record[sig_ptr->dual_sys].cmd_id;


    //SCI_TRACE_LOW:"ATC: enter into ATC_ProcessWriteSmsCnf"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2789_112_2_17_22_17_12_593,(uint8*)"");

    switch(cmd_id)
    {
        case AT_CMD_CMGW:
        {
            if (sig_ptr->cause == MN_SMS_OPERATE_SUCCESS)
            {
                sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[cmd_id].cmd_name->str_ptr, (sig_ptr->record_id + 1));
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
            }
            else
            {
                //SCI_TRACE_LOW:"ATC:WRITE error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2807_112_2_17_22_17_12_594,(uint8*)"");
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,0);
            }

            s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
            break;
        }

        default:
            break;
    }
#endif
    return S_ATC_DEFAULT_HANDLE;
}

#endif

/*****************************************************************************/
//  Description : This function handles the  APP_MN_SET_SMS_RECEIVE_FUNC_CNF signal and
//                     send the result code to SIO
//  Global resource dependence :
//  Author:       Bruce Jiang
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSetSmsRcvFunCnf(
    ATC_CONFIG_T       *atc_config_ptr,
    APP_MN_SET_SMS_RECEIVE_FUNC_CNF_T   *sig_ptr
)
{
#if !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_SYNC_TOOL_)
    SCI_TRACE_LOW("ATC: received the APP_MN_SET_SMS_RECEIVE_FUNC_CNF_T signal");

    if(MN_SMS_OPERATE_SUCCESS != sig_ptr->cause)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
        return S_ATC_DEFAULT_HANDLE;
    }

    if(sig_ptr->is_receive_sms)
    {
        sprintf((char *)g_rsp_str, "+SPSMSFULL:NOT FULL");
        ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
    }
    else
    {
        sprintf((char *)g_rsp_str, "+SPSMSFULL:FULL");
        ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
    }

    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
#endif

   return S_ATC_DEFAULT_HANDLE;

}


#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
/*****************************************************************************/
//  Description : This function restore the s_sms_config[].service_flag
//  Global resource dependence :
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern void ATC_RestoreSmsConfig(MN_DUAL_SYS_E dual_sys)
{
    // Set the default value
#ifdef _ATC_SYNC_TOOL_
    s_sms_config[dual_sys].service_flag = TRUE;
#endif
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_MEM_FULL_IND signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSmsFullInd(
                ATC_CONFIG_T                      *atc_config_ptr,
                APP_MN_SMS_MEM_FULL_IND_T            *sig_ptr
                )
{
#ifdef _ATC_SYNC_TOOL_
    //SCI_TRACE_LOW:"ATC: ATC_ProcessSmsFullInd is called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2896_112_2_17_22_17_12_596,(uint8*)"");
#ifdef _MUX_REDUCED_MODE_
    if(MN_SMS_ALL_FULL == sig_ptr->mem_status)
    {
        if (AT_CMD_SFSMSM == s_sms_record[sig_ptr->dual_sys].cmd_id)
        {
            //SCI_TRACE_LOW:"ATC:SFSMSM Filled Successfully!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2902_112_2_17_22_17_12_597,(uint8*)"");
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
            Atc_FreeMemForSfsmsm(&s_sms_fill_mem);
            s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
        }
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_MEMORY_FULL);
    }
    //SCI_TRACE_LOW:"ATC: mem full %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2909_112_2_17_22_17_12_598,(uint8*)"d", sig_ptr->mem_status);
#else
    if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT10))
    {
        if(MN_SMS_ALL_FULL == sig_ptr->mem_status)
        {
            sprintf((char*)g_rsp_str, "%s: 1024,2", g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else if(MN_SMS_SIM_FULL == sig_ptr->mem_status)
        {
            sprintf((char*)g_rsp_str, "%s: 1024,0", g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else if(MN_SMS_ME_FULL == sig_ptr->mem_status)
        {
            sprintf((char*)g_rsp_str, "%s: 1024,1", g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
    }

    if((AT_CMD_SFSMSM == s_sms_record[sig_ptr->dual_sys].cmd_id) &&
        (MN_SMS_ALL_FULL == sig_ptr->mem_status))
    {
        //SCI_TRACE_LOW:"ATC:SFSMSM Filled Successfully!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_2933_112_2_17_22_17_12_599,(uint8*)"");
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
        Atc_FreeMemForSfsmsm(&s_sms_fill_mem);
        s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
    }
#endif
    SCI_SLEEP(30); //avoid APP queue full during synchronization operation;
#endif
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_STATUS_REPORT_IND signal.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSmsStatusReportInd(
                ATC_CONFIG_T                      *atc_config_ptr,
                APP_MN_SMS_STATUS_REPORT_IND_T    *sig_ptr
                )
{
#ifdef _ATC_SYNC_TOOL_
    MN_CALLED_NUMBER_T      mn_called_num;
    uint8                   first_octet;
    uint8                   called_num_arr[MN_MAX_ADDR_BCD_LEN*2+1];
    ATC_TELE_TYPE_E         tel_type;
    uint8                   *temp_ptr = PNULL;
    uint8                   pdu_len = 0;
    uint8                   *pdu = PNULL;
    MN_DUAL_SYS_E           dual_sys;
    dual_sys = sig_ptr->dual_sys;


    if (s_sms_config[dual_sys].ds_ind)
    {
        MN_L3_MSG_UNIT_T *msg_data_ptr = NULL;
    #ifdef _ATC_ONLY
        msg_data_ptr = &(sig_ptr->status_report.msg_data);
    #else
        msg_data_ptr = &(sig_ptr->msg_data);
    #endif
        // handle the pdu mode status report
        if (!atc_config_ptr->atc_config_nv.cmgf_flag)
        {
            pdu_len = msg_data_ptr->length;

            if (ERR_MNSMS_NONE != MNSMS_GetScAddrEx(dual_sys, &mn_called_num))
            {
               SCI_TRACE_LOW("ATC: ATC_ProcessSmsStatusReportInd, fail to get Scaddr!");
               return S_ATC_FAIL;
            }

            MNSMS_AddScAddrEx(dual_sys,&mn_called_num, msg_data_ptr);

            temp_ptr = g_rsp_str;
            ConvertBinToHex(msg_data_ptr->l3_msg, (uint8)msg_data_ptr->length, temp_ptr);

            pdu = SCI_ALLOC_BASEZ(msg_data_ptr->length*2+1);

            if (pdu == PNULL)// alloc memory fail.
            {
                //SCI_TRACE_LOW:"ATC:PDU allocation fail,exit +CMT!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3007_112_2_17_22_17_12_601,(uint8*)"");
                return (S_ATC_FAIL);
            }

            temp_ptr[msg_data_ptr->length*2] = '\0';

            //SCI_MEMSET(pdu, 0, (msg_data_ptr->length*2+1));
            SCI_MEMCPY(pdu, temp_ptr, (msg_data_ptr->length*2+1));

            SCI_MEMSET((uint8 *)g_rsp_str, 0, sizeof(g_rsp_str));
        #ifdef _MUX_REDUCED_MODE_
            sprintf((char*)g_rsp_str,"+CDS: %d,%d%c%c%s",sig_ptr->record_id + 1, pdu_len,
                        atc_config_ptr->s3_reg, atc_config_ptr->s4_reg, pdu);
        #else
            sprintf((char*)g_rsp_str,"+CDS: %d%c%c%s",pdu_len,
                        atc_config_ptr->s3_reg, atc_config_ptr->s4_reg, pdu);
        #endif
            SCI_FREE(pdu);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        // handle the text mode status report
        else
        {
            first_octet = msg_data_ptr->l3_msg[0];
            ATC_ConvertCalledNumToStr(&(sig_ptr->status_report.dest_addr_t),&tel_type,called_num_arr);
            sprintf((char*)g_rsp_str, "+CDS: %d,%d,\"%s\",%d,",first_octet, sig_ptr->status_report.tp_mr,
                &(called_num_arr[1]), called_num_arr[0]);
            temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];
            FormatTimeStampStr(temp_ptr, &(sig_ptr->status_report.tp_scts));
            temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];
            FormatTimeStampStr(temp_ptr, &(sig_ptr->status_report.tp_dt));
            temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];
            sprintf((char*)temp_ptr, ",%d", sig_ptr->status_report.tp_status);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: disable the status report"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_3046_112_2_17_22_17_12_602,(uint8*)"");
    }
#endif
    return S_ATC_DEFAULT_HANDLE;
}

#endif

#if !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_UIX8910_ENABLE_)
/*****************************************************************************/
//  Description : This function is used change the specific sms status.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  ChangeSmsStatus(
                MN_SMS_STATUS_E      sms_status,
                MN_SMS_STATUS_E      *changed_status_ptr,
                ATC_SMS_STATE_E      at_sms_status,
                BOOLEAN              *status_ptr
                )
{
    BOOLEAN      return_flag = TRUE;

    *status_ptr = TRUE;

    switch ( sms_status )
    {
        case MN_SMS_FREE_SPACE:
        {
            return_flag = FALSE;
            break;
        }

        case MN_SMS_MT_READED:
        {
            if (at_sms_status != ATC_SMS_UNREAD)
            {
                *status_ptr = FALSE;
            }
            else
            {
                *changed_status_ptr = MN_SMS_MT_TO_BE_READ;
            }
            break;
        }

        case MN_SMS_MT_TO_BE_READ:
        {
            if (at_sms_status != ATC_SMS_READ)
            {
                *status_ptr = FALSE;
            }
            else
            {
                *changed_status_ptr = MN_SMS_MT_READED;
            }
            break;
        }

        case MN_SMS_MO_TO_BE_SEND:
        {
            if (ATC_SMS_SENT != at_sms_status)
            {
                *status_ptr = FALSE;
            }
            else
            {
                *changed_status_ptr = MN_SMS_MO_SR_NOT_REQUEST;
            }
            break;
        }

        default:
        {
            if (ATC_SMS_UNSENT != at_sms_status)
            {
                *status_ptr = FALSE;
            }
            else
            {
                *changed_status_ptr = MN_SMS_MO_TO_BE_SEND;
            }
            break;
        }
    }
    return return_flag;
}

/*****************************************************************************/
//  Description : This function is used change the string type status to numberic
//                sms status.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ConvertStrStatusToNumStatus(
                    uint8            *status_ptr,
                    ATC_SMS_STATE_E  *sms_status_ptr
                    )
{
    BOOLEAN          return_flag = FALSE;
    int32            i;

    for (i=0; i<ATC_SMS_ALL; i++)
    {
        if (!strcmp((char*)status_ptr, s_sms_state_str[i]))
        {
            *sms_status_ptr = (ATC_SMS_STATE_E)i;
            return_flag = TRUE;
            break;
        }
    }
    return return_flag;
}

/*****************************************************************************/
//  Description : This function is used handle the cb parameters
//                sms status.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleCbParam(
                BOOLEAN                        mode,         // indicate accept or reject the below parameters
                BOOLEAN                        msg_present,
                uint16                         msg_id,       // specific message identity
                BOOLEAN                        lang_present,
                MN_SMSCB_LANGUAGE_ID_E         lang_id,      // specific language identity
                MN_CB_MSG_ID_MUSTER_T          *msg_ptr,
                MN_CB_LANGUAGE_ID_MUSTER_T     *lang_ptr
                )
{
    if (!mode)
    {// add the specific parameters
        if (msg_present)
        {
            SCI_InsertArrayItem(msg_ptr->msg_id_arr, &(msg_ptr->num),
                sizeof(uint16), &msg_id,MN_CB_MSG_ID_MAX_NUM);
        }
        if (lang_present)
        {
            SCI_InsertArrayItem(lang_ptr->language_id_arr, &(lang_ptr->num),
                sizeof(MN_SMSCB_LANGUAGE_ID_E), &lang_id, MN_CB_LANGUAGE_ID_MAX_NUM);
        }
    }
    else
    {
        if (msg_present)
        {
            SCI_DeleteArrayItem(msg_ptr->msg_id_arr, &(msg_ptr->num), sizeof(uint16), &msg_id);
        }
        if (lang_present)
        {
            SCI_DeleteArrayItem(lang_ptr->language_id_arr,
                &(lang_ptr->num), sizeof(MN_SMSCB_LANGUAGE_ID_E), &lang_id);
        }
    }
    if (NVERR_NONE != EFS_NvitemWrite(NV_ATC_CB_MSG_ID,
        sizeof(MN_CB_MSG_ID_MUSTER_T), (uint8*)msg_ptr, FALSE))
    {
        //SCI_TRACE_LOW:"ATC: Write NV_ATC_CB_MSG_ID failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4434_112_2_17_22_17_15_618,(uint8*)"");
    }
    if (NVERR_NONE != EFS_NvitemWrite(NV_ATC_CB_LANG_ID,
        sizeof(MN_CB_LANGUAGE_ID_MUSTER_T), (uint8*)lang_ptr, FALSE))
    {
        //SCI_TRACE_LOW:"ATC: Write NV_ATC_CB_LANG_ID failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4439_112_2_17_22_17_15_619,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : This function handle the APP_MN_CALL_HELD_CNF signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessSmscbMsgInd(
                ATC_CONFIG_T            *atc_config_ptr,
                APP_MN_SMSCB_MSG_IND_T  *sig_ptr
                )
{
    uint8          *temp_ptr;
    MN_DUAL_SYS_E  dual_sys;

    dual_sys = sig_ptr->dual_sys;

    if (s_sms_config[dual_sys].bm_ind)
    {
        sprintf((char*)g_rsp_str,"+CBM: %d,%d\n",sig_ptr->msg_id, sig_ptr->dcs.alphabet_type);
        temp_ptr = &g_rsp_str[strlen((char*)g_rsp_str)];

        if (MN_SMS_UCS2_ALPHABET == sig_ptr->dcs.alphabet_type)
        {
            ConvertBinToHex(sig_ptr->msg_arr, sig_ptr->msg_len, temp_ptr);

            temp_ptr[sig_ptr->msg_len * 2] = '\0';


            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            uint8    temp_len = (uint8)strlen((char*)g_rsp_str);


            SCI_MEMCPY(temp_ptr, sig_ptr->msg_arr, sig_ptr->msg_len);
        #ifdef _MUX_ENABLE_
            if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + sig_ptr->msg_len));
            else
                MUX_Write(g_rsp_str, (temp_len + sig_ptr->msg_len),atc_config_ptr->current_link_id);
        #else
	#ifdef PPP_USB
            SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + sig_ptr->msg_len),ATC_GetSioPort());
	#else
            SIO_ATC_WriteCmdRes(g_rsp_str, (temp_len + sig_ptr->msg_len));
	#endif

        #endif
            sprintf((char*)g_rsp_str, "%c%c",atc_config_ptr->s3_reg,atc_config_ptr->s4_reg);
        #ifdef _MUX_ENABLE_
            if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
            else
                MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
        #else
	#ifdef PPP_USB
            SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str),ATC_GetSioPort());
	#else
            SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
	#endif

        #endif
        }
    }
    return S_ATC_SUCCESS;
}
#endif

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
/*****************************************************************************/
//  Description : This function sends msg to fill sms storage for AT+SFSMSM
//  Global resource dependence :
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Atc_FillMtMoSmsEx(
                               ATC_CONFIG_T  * atc_config_ptr,
                               MN_SMS_STORAGE_E      storage,
                               MN_DUAL_SYS_E         dual_sys)
{
    SMS_FILL_MEM_T * sfm_ptr = &s_sms_fill_mem;
    BOOLEAN fill_flag = TRUE;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(MN_SYS_NUMBER > dual_sys); /*assert verified*/
    //SCI_TRACE_LOW:"ATC: enter into Atc_FillMtMoSmsEx"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4587_112_2_17_22_17_15_622,(uint8*)"");

    if (atc_config_ptr->atc_config_nv.cmgf_flag)
    {
        if((MN_SMS_MT_TO_BE_READ == sfm_ptr->sms_status)
            ||(MN_SMS_MT_READED == sfm_ptr->sms_status))
        {
            //Write the mt sms
            if (ERR_MNSMS_NONE != MNSMS_WriteMtSmsEx(dual_sys, storage, sfm_ptr->sms_status, sfm_ptr->mt_sms_ptr))
            {
                fill_flag = FALSE;
            }
        }
        else
        {
            //Wrtie the mo sms
            if (ERR_MNSMS_NONE != MNSMS_WriteMoSmsEx(dual_sys, storage, sfm_ptr->sms_status, sfm_ptr->mo_sms_ptr))
            {
                fill_flag = FALSE;
            }
        }
    }
    else
    {
        // pdu mode SMS
        if (ERR_MNSMS_NONE != MNSMS_AtcWriteSmsEx(dual_sys, storage, sfm_ptr->sms_status, sfm_ptr->src_addr_ptr, sfm_ptr->bin_pdu_ptr))
        {
            fill_flag = FALSE;
        }
    }

    return fill_flag;
}

/*****************************************************************************/
//  Description : This function used to free dynamic memory for AT+SFSMSM
//  Global resource dependence :
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
LOCAL void Atc_FreeMemForSfsmsm(SMS_FILL_MEM_T * sfm_ptr)
{
    uint8 i = 0;

    //SCI_TRACE_LOW:"ATC: enter into Atc_FreeMemForSfsmsm"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4629_112_2_17_22_17_15_623,(uint8*)"");

    if(PNULL != sfm_ptr->bin_pdu_ptr)
    {
        SCI_FREE(sfm_ptr->bin_pdu_ptr);
        sfm_ptr->bin_pdu_ptr = PNULL;
    }

    if(PNULL != sfm_ptr->mt_sms_ptr)
    {
        SCI_FREE(sfm_ptr->mt_sms_ptr);
        sfm_ptr->mt_sms_ptr = PNULL;
    }

    if(PNULL != sfm_ptr->mo_sms_ptr)
    {
        SCI_FREE(sfm_ptr->mo_sms_ptr);
        sfm_ptr->mo_sms_ptr = PNULL;
    }

    if(PNULL != sfm_ptr->src_addr_ptr)
    {
        SCI_FREE(sfm_ptr->src_addr_ptr);
        sfm_ptr->src_addr_ptr = PNULL;
    }

    sfm_ptr->sms_status = MN_SMS_FREE_SPACE;
    sfm_ptr->me_mem_sts = MN_SMS_ME_FULL;

    for (i = 0; i < MN_SYS_NUMBER; i++)
    {
        *(sfm_ptr->sim_mem_sts + i) = MN_SMS_SIM_FULL;
    }

    return;
}

/*****************************************************************************/
//  Description : This function used to update SMS memories' status for AT+SFSMSM
//                      and find its free or not
//  Global resource dependence :
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
LOCAL MN_SMS_MEM_FULL_STATUS_E Atc_UpdateSmsMemSts(
                ATC_CONFIG_T         *atc_config_ptr,
                SMS_FILL_MEM_T            *sfm_ptr)
{
    MN_SMS_MEM_FULL_STATUS_E mem_storage = MN_SMS_MEM_AVAIL;
    MN_SMS_RECORD_ID_T total_num = 0;
    MN_SMS_RECORD_ID_T used_num = 0;
    MN_DUAL_SYS_E     dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    BOOLEAN                 unread_flag = FALSE;

    //SCI_TRACE_LOW:"ATC: Atc_UpdateSmsMemSts is called"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4681_112_2_17_22_17_15_624,(uint8*)"");

    if (TRUE == GetStorageInfo(dual_sys,MN_SMS_STORAGE_ME, &total_num, &used_num, &unread_flag))
    {
        if (used_num < total_num)
        {
            sfm_ptr->me_mem_sts = MN_SMS_MEM_AVAIL;
        }
        else
        {
            sfm_ptr->me_mem_sts = MN_SMS_ME_FULL;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"Get Me Storage inforamtion failure!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4696_112_2_17_22_17_15_625,(uint8*)"");
    }

    if ((atc_config_ptr->sim_flag[dual_sys]) &&
        (TRUE == GetStorageInfo(dual_sys,MN_SMS_STORAGE_SIM, &total_num, &used_num, &unread_flag)))
    {
        if (used_num < total_num)
        {
            sfm_ptr->sim_mem_sts[dual_sys] = MN_SMS_MEM_AVAIL;
        }
        else
        {
            sfm_ptr->sim_mem_sts[dual_sys]  = MN_SMS_SIM_FULL;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"Get SIM %d Storage inforamtion failure!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_SMS_4713_112_2_17_22_17_15_626,(uint8*)"d", dual_sys);
    }

    if ((MN_SMS_ME_FULL == sfm_ptr->me_mem_sts) &&
         (MN_SMS_SIM_FULL == sfm_ptr->sim_mem_sts[dual_sys]))
    {
        mem_storage = MN_SMS_ALL_FULL;
    }
    else
    {
        mem_storage = MN_SMS_MEM_AVAIL;
    }

    return mem_storage;
}

#endif

AT_CMD_FUNC( ATC_ProcessCMMS )
{
#ifdef _ATC_UIX8910_ENABLE_
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    ATC_STATUS     status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                if (0 == PARA1.num || 1 == PARA1.num)
                    gCMMS =  PARA1.num;
                else
                    status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char *)g_rsp_str,"CMMS: %d",gCMMS);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char *)g_rsp_str,"CMMS: (0,1,2)");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC( ATC_ProcessCGSMS )
{
#ifdef _ATC_UIX8910_ENABLE_
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    ATC_STATUS     status = S_ATC_SUCCESS;
    uint8 uService = 1;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                if ((PARA1.num >= 0) && (4 > PARA1.num))
                    CFW_GprsSetSmsSeviceMode(PARA1.num);
                else
                    status = ERR_OPERATION_NOT_ALLOWED;

            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            uService = CFW_GprsGetSmsSeviceMode();
            switch (uService)
            {
                case 0:
                    sprintf((char *)g_rsp_str, "+CGSMS:0");
                    break;
                case 1:
                    sprintf((char *)g_rsp_str, "+CGSMS:1");
                    break;
                case 2:
                    sprintf((char *)g_rsp_str, "+CGSMS:2");
                    break;
                case 3:
                    sprintf((char *)g_rsp_str, "+CGSMS:3");
                    break;
                default:
                    sprintf((char *)g_rsp_str, "+CGSMS:0");
                    break;
            }
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char *)g_rsp_str,"CGSMS: (0,1,2,3)");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif
}

#if 0
/*****************************************************************************/
//  Description : This function handles the AT+SMSC command. This command is used
//                to set unchange the sms status
//  AT+SUSS=<mode>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSUSS )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS            status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                s_sms_config[dual_sys].suss_flag = (uint8)PARA1.num;
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SUSS].cmd_name->str_ptr, s_sms_config[dual_sys].suss_flag);
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
//  Description : This function handles the AT+SCMGS command
//  Global resource dependence : g_rsp_str
//  Author:       Sunsome Ju
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCMGS )
{
 #ifdef _ULTRA_LOW_CODE_
    return S_ATC_FAIL;
 #else
    uint8             sm_octet_len; // indicate the number of octets coded in the TP layer data unit to be given
    uint8             pdu_ptr[2*ATC_MAX_SM_LEN];
    uint16            pdu_len = 0;
    BOOLEAN           flag;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if(!atc_config_ptr->sim_flag[dual_sys])
    {
        return ERR_SIM_NOT_INSERTED;
    }
    else
    {
        switch(atc_config_ptr->cpin[dual_sys])
        {
            case ATC_CPIN_CODE_READY:
                break;
            case ATC_CPIN_CODE_PIN1:
                return ERR_SIM_PIN_REQUIRED;
            case ATC_CPIN_CODE_PIN2:
                return ERR_SIM_PIN2_REQUIRED;
            case ATC_CPIN_CODE_PUK1:
                return ERR_SIM_PUK_REQUIRED;
            case ATC_CPIN_CODE_PUK2:
                return ERR_SIM_PUK2_REQUIRED;
            case ATC_CPIN_CODE_BLOCK:
            default:
                return ERR_SIM_FAILURE;
        }
    }

    s_sms_record[dual_sys].more_sms_flag = FALSE;
    // check the parameter
    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_SET || !PARAM2_FLAG)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
        return S_ATC_DEFAULT_HANDLE;
    }

    switch (PARAM1_TYPE)
    {
        // the parameter 1 is the numeric and the message mode is PDU
        case ATC_CMD_PARAM_TYPE_NUMERIC:
            sm_octet_len = (uint8)PARA1.num;
            if (!atc_config_ptr->atc_config_nv.cmgf_flag &&
                (ATC_MIN_SM_LEN < sm_octet_len ||
                ATC_MAX_SM_LEN > sm_octet_len))
            {
                s_sms_record[dual_sys].length = sm_octet_len;
            }
            else
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                return S_ATC_DEFAULT_HANDLE;
            }
            break;

        case ATC_CMD_PARAM_TYPE_STRING:
        {
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
            return S_ATC_DEFAULT_HANDLE;
        }

        default:
        {
            SCI_ASSERT(FALSE); /*assert verified check FALSE case*/
            break;
        }
    }

    switch (PARAM2_TYPE)
    {
        // the parameter 1 is the numeric and the message mode is PDU
        case ATC_CMD_PARAM_TYPE_NUMERIC:
        {
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
            return S_ATC_DEFAULT_HANDLE;
        }

        case ATC_CMD_PARAM_TYPE_STRING:
        {
             //Check the length of input number
             if(2*ATC_MAX_SM_LEN < PARA2.str_ptr->str_len)
             {
                 return ERR_INVALID_INDEX;
             }

            if (!atc_config_ptr->atc_config_nv.cmgf_flag)/* ||
                !ATC_ConvertStrToStruct(pdu_ptr,&pdu_len,
                                           PARA2.str_ptr->str_ptr,
                                           PARA2.str_ptr->str_len))*/
            {
                SCI_MEMCPY(pdu_ptr,PARA2.str_ptr->str_ptr, PARA2.str_ptr->str_len);
                pdu_len = PARA2.str_ptr->str_len;
            }
            else
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                return S_ATC_DEFAULT_HANDLE;
            }
            break;
        }

        default:
        {
            SCI_ASSERT(FALSE); /*assert verified check FALSE case*/
            break;
        }
    }

    s_sms_record[dual_sys].cmd_id = AT_CMD_SCMGS;
    s_sms_record[dual_sys].state  = ATC_SMS_UNSENT;
    if (PARAM3_FLAG)
    {
        s_sms_record[dual_sys].more_sms_flag = PARA3.num;
    }
    else
    {
        s_sms_record[dual_sys].more_sms_flag = FALSE;
    }

#ifdef _MUX_ENABLE_
    flag = HandlePduModeSms(atc_config_ptr,pdu_ptr, pdu_len);
    g_atc_sms_flag = TRUE;
#else
    flag = HandlePduModeSms(pdu_ptr, pdu_len);
#endif

    return S_ATC_DEFAULT_HANDLE;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handles the AT+SCMGW command
//                AT+CMGW=<length/destination address>,<state>,<sms string>
//                state: 0   "REC UNREAD"
//                       1   "REC READ"
//                       2   "STO UNSEND"
//                       3   "STO SENT"
//  Global resource dependence : g_rsp_str
//  Author:       Sunsome Ju
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCMGW )
{
 #ifdef _ULTRA_LOW_CODE_
    return S_ATC_FAIL;
 #else
    uint8              sm_octet_len;
    uint8              pdu_ptr[2*ATC_MAX_SM_LEN];
    uint16             pdu_len = 0;
    BOOLEAN            flag;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if(!atc_config_ptr->sim_flag[dual_sys])
    {
        return ERR_SIM_NOT_INSERTED;
    }
    else
    {
        switch(atc_config_ptr->cpin[dual_sys])
        {
            case ATC_CPIN_CODE_READY:
                break;

            case ATC_CPIN_CODE_PIN1:
                return ERR_SIM_PIN_REQUIRED;

            case ATC_CPIN_CODE_PIN2:
                return ERR_SIM_PIN2_REQUIRED;

            case ATC_CPIN_CODE_PUK1:
                return ERR_SIM_PUK_REQUIRED;

            case ATC_CPIN_CODE_PUK2:
                return ERR_SIM_PUK2_REQUIRED;

            case ATC_CPIN_CODE_BLOCK:
            default:
                return ERR_SIM_FAILURE;
        }
    }

    // check the parameter1 and the command type
    if (ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE || !PARAM1_FLAG || !PARAM3_FLAG)
        return ERR_OPERATION_NOT_ALLOWED;

    switch (PARAM1_TYPE)
    {
        case ATC_CMD_PARAM_TYPE_STRING:
        {
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_303);
            return S_ATC_DEFAULT_HANDLE;
        }

        case ATC_CMD_PARAM_TYPE_NUMERIC:
        {
            sm_octet_len = (uint8)PARA1.num;
            if (!atc_config_ptr->atc_config_nv.cmgf_flag
                && (ATC_MIN_SM_LEN < sm_octet_len || ATC_MAX_SM_LEN > sm_octet_len))
            {
                s_sms_record[dual_sys].length = sm_octet_len;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            break;
        }

        default:
        {
            SCI_ASSERT(FALSE); /*assert verified check FALSE case*/
            break;
        }
    }

    // check the parameter of <state>
    if (PARAM2_FLAG)
    {
        if(PARA2.num < ATC_SMS_ALL)
        {
            s_sms_record[dual_sys].state = PARA2.num;
        }
        else
        {
            return ERR_INVALID_INDEX;
        }
    }
    else
    {
        s_sms_record[dual_sys].state = ATC_SMS_UNSENT;
    }

    switch (PARAM3_TYPE)
    {
        // the parameter 1 is the numeric and the message mode is PDU
        case ATC_CMD_PARAM_TYPE_NUMERIC:
        {
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_303);
            return S_ATC_DEFAULT_HANDLE;
        }

        case ATC_CMD_PARAM_TYPE_STRING:
        {
             //Check the length of input number
             if(2*ATC_MAX_SM_LEN < PARA3.str_ptr->str_len)
             {
                 return ERR_INVALID_INDEX;
             }

            if (!atc_config_ptr->atc_config_nv.cmgf_flag)
            {
                SCI_MEMCPY(pdu_ptr,PARA3.str_ptr->str_ptr, PARA3.str_ptr->str_len);
                pdu_len = PARA3.str_ptr->str_len;
            }
            else
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_303);
                return S_ATC_DEFAULT_HANDLE;
            }
            break;
        }

        default:
        {
            SCI_ASSERT(FALSE); /*assert verified check FALSE case*/
            break;
        }
    }

    s_sms_record[dual_sys].cmd_id = AT_CMD_SCMGW;

    #ifdef _MUX_ENABLE_
    flag = HandlePduModeSms(atc_config_ptr,pdu_ptr, pdu_len);
    #else
    flag = HandlePduModeSms(pdu_ptr, pdu_len);
    #endif

    return S_ATC_DEFAULT_HANDLE;
#endif
}
#endif
