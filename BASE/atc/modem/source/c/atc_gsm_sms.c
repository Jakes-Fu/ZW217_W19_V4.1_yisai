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
#include "atc_gsm_sms.h"
#include "atc.h"
#include "atc_common.h"
#include "sci_array.h"

#include "tasks_id.h"

/**---------------------------------------------------------------------------*
**                         Declare Variable                                  *
**---------------------------------------------------------------------------*/
/*lint -save -e752 */
extern MN_DUAL_SYS_E         g_current_card_id;//lint !e752
static ATC_SMS_CONFIG_T      s_sms_config[MN_SYS_NUMBER] = {0};
static ATC_SMS_RECORD_T      s_sms_record[MN_SYS_NUMBER] = {0};
static ATC_SMS_NV_CONFIG_T   s_sms_nv_config;
BOOLEAN                      s_sms_exclusive_flag = FALSE;
ATC_SMS_CNMA_T               s_sms_cnma[MN_SYS_NUMBER] = {0};
extern RSP_RESULT_STR        g_rsp_str;
extern const ATC_INFO_T      g_atc_info_table[];

MN_CALLED_NUMBER_T           g_sc_addr[MN_SYS_NUMBER] = {0};
MN_SMS_SERVICE_TYPE_E        g_sms_ser_type[MN_SYS_NUMBER];
extern BOOLEAN               g_atc_ps_not_save_sms[MN_SYS_NUMBER];

const char *s_sms_mem_str[] =
{"ME", "SM", "SR"};

const char *s_sms_state_str[] =
{
    "REC UNREAD",
    "REC READ",
    "STO UNSENT",
    "STO SENT",
    "ALL",
    "INVALID"
};

int   stricmp(char *s1, const char *s2);

/*****************************************************************************/
//  Description : This function get the specific sms information
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  ReadSpecSms(
    MN_DUAL_SYS_E   dual_sys,
    MN_SMS_STORAGE_E        storage,
    MN_SMS_RECORD_ID_T      *index_ptr,
    ATC_SMS_STATE_E         sms_state
);

LOCAL BOOLEAN DeleteSpecSms(
    MN_DUAL_SYS_E   dual_sys,
    MN_SMS_STORAGE_E        storage,
    MN_SMS_RECORD_ID_T      *index_ptr,
    ATC_SMS_DEL_STATE_E     del_state
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
//  Description : This function compare the input string and the sm storage
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN   CompareSmStorage(
    char   *mem_str,          // in: pointer to the storage
    uint8  *storage_id_ptr   // out: identify the storage id
);

/*****************************************************************************/
//  Description : This function is used to handle the response of the at+cmss
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleCmssCmd(
    APP_MN_READ_SMS_CNF_T    *sig_ptr,
    ATC_CONFIG_T             *atc_config_ptr
);

/*****************************************************************************/
//  Description : This function handles the binary format pdu string
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
#ifdef _MUX_ENABLE_
LOCAL BOOLEAN HandlePduModeSms(
    MN_DUAL_SYS_E         dual_sys,
    ATC_CONFIG_T    *atc_config_ptr,
    uint8         *pdu_ptr,       // point to the pdu buffer
    uint16         pdu_len
);
#else
LOCAL BOOLEAN HandlePduModeSms(
    MN_DUAL_SYS_E         dual_sys,
    uint8         *pdu_ptr,       // pointer to the pdu buffer
    uint16         pdu_len
);
#endif
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
//  Description : This function handles the text mode SMS
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTextModeSms(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T    *atc_config_ptr,
    uint8         *pdu_ptr,       // point to the pdu buffer
    uint16         pdu_len
);

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
//  Description : This function converts the absolute period to the relative period
//  Global resource dependence :
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL uint8 GetValidityPeriod(uint32 periodsecond);

/*****************************************************************************/
//  Description : This function is used to read the next sms by AT+CMGL
//  Global resource dependence :
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
LOCAL void ReadNextSMS(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T          *atc_config_ptr
);

/*****************************************************************************/
//  Description : This function is used to update the sms status by AT+CMGL, AT+CMGR
//  Global resource dependence :
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateSMSStatus(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T          *atc_config_ptr,
    MN_SMS_STATUS_E     sms_status,
    MN_SMS_RECORD_ID_T  record_id
);

/*****************************************************************************/
//  Description : This function converts TE character set sms address
//    (such as <da> <oa> <ra>..)to the ME character set string(BCD format) in text mode
//      Global resource dependence :
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_DecodeSMSAddrChsetFromTE(
    ATC_CUR_PARAM_STRING_T *sms_addr_ptr,  //in, sms address info
    int32         addr_type,              //in, address type value
    uint8         TE_chset_type,          //in, TE character set type
    MN_CALLED_NUMBER_T     *called_number_ptr  //out, call number structure
);
/*****************************************************************************/
//  Description : This function converts sms address(such as <da> <oa> <ra>..)
//                 to the TE character set string in text mode
//      Global resource dependence :
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_EncodeCalledNumToTE(
    MN_CALLED_NUMBER_T     *called_number_ptr,  //input, call number structure
    uint8                   dest_chset,       //in, character set of telephone number
    ATC_TELE_TYPE_E        *tel_type_ptr,       //output,telephone number type
    uint8                  *tel_str_ptr,        //output,telephone number string
    uint16                 *tel_str_len_ptr     //input, max length of tel_str_ptr buffer
    //output, telephone number length
);
/*****************************************************************************/
//  Description : This function converts sms data
//                 to the TE character set string in text mode
//      Global resource dependence :
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_EncodeSMSDataToTE(
    uint8        *data_str,        //in, sms data
    uint16        data_len,        //in, sms data length
    MN_SMS_ALPHABET_TYPE_E      dcs,   //in, the Data Coding Scheme of sms data
    uint8         dest_chset,       //in, character set of sms <data>
    uint8        *data_to_TE_ptr,     //out, sms data(reported to TE)
    uint16       *data_to_TE_len_ptr  //out, sms data(reported to TE) length
);

/*****************************************************************************/
//  Description : This function delete duplicated items(mid/dcs) for AT+CSCB
//  Global resource dependence :
//  Author:       Elvis xu
//  Note:
/*****************************************************************************/
BOOLEAN ATC_SmsCBDelDupItem(uint16 *digit_array, uint8 *digit_num);

/*****************************************************************************/
//  Description : This function used to add new msg id or language id Item for AT+CSCB
//  Global resource dependence :
//  Author:       Elvis xu
//  Note:
/*****************************************************************************/
BOOLEAN ATC_SmsCBAddItem(
    uint16 *dst_set_ptr,
    uint8 *dst_num_ptr,
    uint16 *src_set_ptr,
    uint8 src_num,
    uint8 max
);

/*****************************************************************************/
//  Description : This function delete mid or lang_id items from the setting mid set for AT+CSCB
//  Global resource dependence :
//  Author:       Elvis xu
//  Note:
/*****************************************************************************/
BOOLEAN ATC_SmsCBDelItem(
    uint16 *dst_set_ptr,
    uint8 *dst_num_ptr,
    uint16 *del_set_ptr,
    uint8 del_num
);

/*****************************************************************************/
//  Description : If ME does not get acknowledgement within required time (network timeout),
//     ME should respond as specified in 3GPP TS 24.011 [6] to the network.  ME/TA shall
//     automatically disable routing to TE by setting both <mt> and <ds> values of +CNMI to zero
//  Global resource dependence : none
//  Author: george.yuan
//  Modify:
//  Note:
/*****************************************************************************/
void AtcCNMATimerExpired(
    uint32 lparam   //the function's input parameter
)
{
    MN_L3_MSG_UNIT_T  tpdu = {0};
    
    /* cnma is not on time, so set to false*/
    s_sms_cnma[lparam].is_waiting_for_cnma = FALSE;
    MNSMS_SendATSmsDlvrRptEx(lparam,s_sms_cnma[lparam].cnma_index,MN_SMS_NACK_DELIVER_REPORT,&tpdu);
}

/**********************************************************************
//    Description:
//      the funciton to restart atc CNMA timer
//    Global resource dependence :
//    Author:george.yuan
//    Note:
***********************************************************************/
static void AtcReStartCNMATimer(
    MN_DUAL_SYS_E       dual_sys,
    uint32  timer_value
)
{
    SCI_ASSERT(0 < timer_value);/*assert verified: check null pointer*/

    if(SCI_IsTimerActive(s_sms_cnma[dual_sys].cnma_timer))
    {
        SCI_DeactiveTimer(s_sms_cnma[dual_sys].cnma_timer);
    }

    SCI_ChangeTimer(s_sms_cnma[dual_sys].cnma_timer, AtcCNMATimerExpired, timer_value);
    SCI_ActiveTimer(s_sms_cnma[dual_sys].cnma_timer);
}

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
AT_CMD_FUNC(ATC_ProcessCMGL)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_SMS_STATE_E       atc_status = ATC_SMS_MAX_STATUS;
    MN_SMS_RECORD_ID_T    start_index = 0;
    ATC_STATUS            status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

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

    /* +CMGL[=<stat>] */
    if(!PARAM1_FLAG)
    {
        // return the result code "operate not allowed"
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
        return status;
    }

    if(!atc_config_ptr->sms_is_ready[dual_sys] && MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem1)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return status;
    }

    // check the sms mode: TRUE: text mode; FALSE: pdu mode
    switch(PARAM1_TYPE)
    {
            // the parameter 1 is a string
        case ATC_CMD_PARAM_TYPE_STRING:

            if(!atc_config_ptr->atc_config_nv.cmgf_flag)
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                return status;
            }

            PARA1.str_ptr->str_ptr[PARA1.str_ptr->str_len] = '\0';

            // get the sms state
            if(!GetSmsState(PARA1.str_ptr->str_ptr, &atc_status))
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                return status;
            }

            break;

        case ATC_CMD_PARAM_TYPE_NUMERIC:

            if(atc_config_ptr->atc_config_nv.cmgf_flag)
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                return status;
            }

            atc_status = PARA1.num;

            if(atc_status >= ATC_SMS_MAX_STATUS)
            {
                return ERR_INVALID_INDEX;
            }

            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    s_sms_record[dual_sys].cmgl_idex = 0;

    // Check sms statue and read them
    switch(atc_status)
    {
        case ATC_SMS_UNREAD:
        case ATC_SMS_READ:
        case ATC_SMS_UNSENT:
        case ATC_SMS_SENT:
        case ATC_SMS_ALL:

            if(ReadSpecSms(dual_sys, s_sms_config[dual_sys].mem1, &start_index, atc_status))
            {
                MNSMS_ReadSmsEx(dual_sys, s_sms_config[dual_sys].mem1, start_index);
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  (uint16)APP_MN_READ_SMS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            }
            else
            {
                status = S_ATC_SUCCESS;
            }

            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    // set the static variables
    s_sms_record[dual_sys].cmd_id    = AT_CMD_CMGL;
    s_sms_record[dual_sys].state     = atc_status;

    return status;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handles the AT+CSMS command.this command is used
//                to set message service
//                AT+CSMS=<service>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSMS)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS     status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                s_sms_config[dual_sys].service_flag = PARA1.num;
                if(ERR_MNSMS_NONE != MNSMS_SetCSMSEx(dual_sys, PARA1.num))
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                }
                else
                {
                    sprintf((char *)g_rsp_str, "%s: %d,%d,%d", 
                    g_atc_info_table[AT_CMD_CSMS].cmd_name->str_ptr, 
                    s_sms_config[dual_sys].mo_flag,
                    s_sms_config[dual_sys].mt_flag, 
                    s_sms_config[dual_sys].bm_flag);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d,%d,%d,%d",
                                    g_atc_info_table[AT_CMD_CSMS].cmd_name->str_ptr,
                                    s_sms_config[dual_sys].service_flag,
                                    s_sms_config[dual_sys].mo_flag,
                                    s_sms_config[dual_sys].mt_flag,
                                    s_sms_config[dual_sys].bm_flag);
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
//  Description : This function handles the AT+CSDH command.this command is used
//                to give additional information on text mode result code.
//                AT+CSDH=<value>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSDH)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS     status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                s_sms_config[dual_sys].csdh_flag = PARA1.num;
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CSDH].cmd_name->str_ptr,
                    s_sms_config[dual_sys].csdh_flag);
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
//  Description : This function handles the AT+CRES command.this command is used
//                to restore the parameters specified in the at+csca & at+csmp
//                AT+CRES
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCRES)
{
#ifndef _ULTRA_LOW_CODE_
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //check the SIM card
    if(!atc_config_ptr->sms_is_ready[dual_sys])
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }

    //Set SC from SIM card
    if(ERR_MNSMS_NONE  != MNSMS_GetScAddrEx(dual_sys, &g_sc_addr[dual_sys]))
    {
        SCI_TRACE_LOW("ATC: Get sc failure");
    }

    //Set the s_sms_nv_config from NV.
    if(NVERR_NONE != EFS_NvitemRead(NV_SMS_CONFIG_ID,
                                    sizeof(ATC_SMS_NV_CONFIG_T), (uint8 *)&s_sms_nv_config))
    {
        SCI_TRACE_LOW("ATC: Read s_sms_nv_config failure");
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handles the AT+CSAS command.this command is used
//                to save the SMS configuration profile
//                AT+CSAS
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSAS)
{
#ifndef _ULTRA_LOW_CODE_
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(!atc_config_ptr->sms_is_ready[dual_sys])
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }

    // call the mnsms api to set the SC
    MNSMS_SetScAddrEx(dual_sys, &g_sc_addr[dual_sys]);

    //write para in AT+CSMP the NV.
    if(NVERR_NONE != EFS_NvitemWrite(NV_SMS_CONFIG_ID,
                                     sizeof(ATC_SMS_NV_CONFIG_T), (uint8 *)&s_sms_nv_config, FALSE))
    {
        SCI_TRACE_LOW("ATC: write NV_ATC_CONFIG_ID fail");
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handles the AT+CPMS command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPMS)
{
#ifndef _ULTRA_LOW_CODE_
    uint8                 storage_id;
    MN_SMS_RECORD_ID_T    total_amount1;
    MN_SMS_RECORD_ID_T    used_amount1;
    MN_SMS_STORAGE_E    storage_type1;
    MN_SMS_RECORD_ID_T    total_amount2;
    MN_SMS_RECORD_ID_T    used_amount2;
    MN_SMS_STORAGE_E    storage_type2;
    MN_SMS_RECORD_ID_T    total_amount3;
    MN_SMS_RECORD_ID_T    used_amount3;
    MN_SMS_STORAGE_E    storage_type3;
    uint8                 temp_str[20] = {0};
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG || PARAM2_FLAG || PARAM3_FLAG)
            {
                storage_type1 = s_sms_config[dual_sys].mem1;
                storage_type2 = s_sms_config[dual_sys].mem2;
                storage_type3 = s_sms_config[dual_sys].mem3;

                if(PARAM1_FLAG)
                {
                    if(CompareSmStorage((char *)PARA1.str_ptr->str_ptr, &storage_id))
                    {
                        // for compatible with wavecom spec
                        switch(storage_id)
                        {
                            case AT_SMS_STORAGE_SM:
                                storage_type1 = MN_SMS_STORAGE_SIM;
                                break;
                            case AT_SMS_STORAGE_ME:
                                storage_type1 = MN_SMS_STORAGE_ME;
                                break;
                            default:
                                SCI_TRACE_LOW("ATC:AT+CPMS,error in para1");
                                return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                }

                if(PARAM2_FLAG)
                {
                    if(CompareSmStorage((char *)PARA2.str_ptr->str_ptr, &storage_id))
                    {
                        // for compatible with wavecom spec
                        switch(storage_id)
                        {
                            case AT_SMS_STORAGE_SM:
                                storage_type2 = MN_SMS_STORAGE_SIM;
                                break;
                            case AT_SMS_STORAGE_ME:
                                storage_type2 = MN_SMS_STORAGE_ME;
                                break;
                            default:
                                SCI_TRACE_LOW("ATC:AT+CPMS,error in para2");
                                return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                }

                if(PARAM3_FLAG)
                {
                    if(CompareSmStorage((char *)PARA3.str_ptr->str_ptr, &storage_id))
                    {
                        // for compatible with wavecom spec
                        switch(storage_id)
                        {
                            case AT_SMS_STORAGE_SM:
                                storage_type3 = MN_SMS_STORAGE_SIM;
                                break;

                            case AT_SMS_STORAGE_ME:
                                storage_type3 = MN_SMS_STORAGE_ME;
                                break;

                            default:
                                SCI_TRACE_LOW("ATC:AT+CPMS,error in para2");
                                return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                }

                if(((MN_SMS_STORAGE_SIM == storage_type1) || (MN_SMS_STORAGE_SIM == storage_type2) || (MN_SMS_STORAGE_SIM == storage_type3))
                        && (!atc_config_ptr->sms_is_ready[dual_sys]))
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessCPMS: sms is not ready");
                    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
                    return S_ATC_DEFAULT_HANDLE;
                }

                if(ERR_MNSMS_NONE == MNSMS_GetSmsNumEx(dual_sys, storage_type1, &total_amount1, &used_amount1) &&
                        ERR_MNSMS_NONE == MNSMS_GetSmsNumEx(dual_sys, storage_type2, &total_amount2, &used_amount2) &&
                        ERR_MNSMS_NONE == MNSMS_GetSmsNumEx(dual_sys, storage_type3, &total_amount3, &used_amount3))
                {
                    s_sms_config[dual_sys].mem1 = storage_type1;
                    s_sms_config[dual_sys].mem2 = storage_type2;
                    s_sms_config[dual_sys].mem3 = storage_type3;

                    if(MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem3)
                    {
                        MNSMS_SetStoragePriorityEx(dual_sys, TRUE);
                    }
                    else
                    {
                        MNSMS_SetStoragePriorityEx(dual_sys, FALSE);
                    }

                    sprintf((char *)g_rsp_str, "%s: %d,%d,%d,%d,%d,%d", g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
                            used_amount1, total_amount1, used_amount2, total_amount2, used_amount3, total_amount3);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                    return ERR_PHONE_FAILURE;
                }
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:

            if(((MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem1) || (MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem2) || (MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem3))
                    && (!atc_config_ptr->sms_is_ready[dual_sys]))
            {
                SCI_TRACE_LOW("ATC: ATC_ProcessCPMS: sms is not ready");
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
                return S_ATC_DEFAULT_HANDLE;
            }

            if(ERR_MNSMS_NONE != MNSMS_GetSmsNumEx(dual_sys, s_sms_config[dual_sys].mem1, &total_amount1, &used_amount1))
            {
                return S_ATC_FAIL;
            }

            if(MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem1)
            {
                sprintf((char *)g_rsp_str, "%s: \"SM\",%d,%d", g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
                        used_amount1, total_amount1);
            }
            else
            {
                sprintf((char *)g_rsp_str, "%s: \"ME\",%d,%d", g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
                        used_amount1, total_amount1);
            }

            if(ERR_MNSMS_NONE != MNSMS_GetSmsNumEx(dual_sys, s_sms_config[dual_sys].mem2, &total_amount2, &used_amount2))
            {
                return S_ATC_FAIL;
            }

            if(MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem2)
            {
                sprintf((char *)temp_str, ",\"SM\",%d,%d", used_amount2, total_amount2);
            }
            else
            {
                sprintf((char *)temp_str, ",\"ME\",%d,%d", used_amount2, total_amount2);
            }

            strcat((char *)g_rsp_str, (char *)temp_str);

            if(ERR_MNSMS_NONE != MNSMS_GetSmsNumEx(dual_sys, s_sms_config[dual_sys].mem3, &total_amount3, &used_amount3))
            {
                return S_ATC_FAIL;
            }

            SCI_MEMSET(temp_str, 0, sizeof(temp_str));

            if(MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem3)
            {
                sprintf((char *)temp_str, ",\"SM\",%d,%d", used_amount3, total_amount3);
            }
            else
            {
                sprintf((char *)temp_str, ",\"ME\",%d,%d", used_amount3, total_amount3);
            }

            strcat((char *)g_rsp_str, (char *)temp_str);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: ((\"%s\",\"%s\"),(\"%s\",\"%s\"),(\"%s\",\"%s\"))",
                    g_atc_info_table[AT_CMD_CPMS].cmd_name->str_ptr,
                    s_sms_mem_str[AT_SMS_STORAGE_ME], s_sms_mem_str[AT_SMS_STORAGE_SM],
                    s_sms_mem_str[AT_SMS_STORAGE_ME], s_sms_mem_str[AT_SMS_STORAGE_SM],
                    s_sms_mem_str[AT_SMS_STORAGE_ME], s_sms_mem_str[AT_SMS_STORAGE_SM]);

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
//  Description : This function handles the AT+CPMS command This command is used
//                to set or read the parameters of sending SMS
//                AT+CSMP=<fo>,<pid>,<vp>,<dcs>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSMP)
{
 #ifndef _ULTRA_LOW_CODE_
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                s_sms_nv_config.fo = (uint8)PARA1.num;
            }

            if(PARAM2_FLAG)
            {
                s_sms_nv_config.vp = (uint8)PARA2.num;
            }

            if(PARAM3_FLAG)
            {
                s_sms_nv_config.pid = (uint8)PARA3.num;
            }

            if(PARAM4_FLAG)
            {
                s_sms_nv_config.dcs = (uint8)PARA4.num;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d,%d,%d,%d", g_atc_info_table[AT_CMD_CSMP].cmd_name->str_ptr,
                    s_sms_nv_config.fo, s_sms_nv_config.vp, s_sms_nv_config.pid, s_sms_nv_config.dcs);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
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
//  Description : This function handles the AT+CMGF command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGF)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS     status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                atc_config_ptr->atc_config_nv.cmgf_flag = PARA1.num;
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CMGF].cmd_name->str_ptr, atc_config_ptr->atc_config_nv.cmgf_flag);
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
//  Description : This function handles the AT+CMGR command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGR)
{
    MN_SMS_RECORD_ID_T    rec_id, index;
    MN_SMS_RECORD_ID_T    total_amount;
    MN_SMS_RECORD_ID_T    mn_sms_num;
    MN_SMS_STATUS_E       *status_ptr = PNULL;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(!atc_config_ptr->sms_is_ready[dual_sys] && MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem1)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }

    if(ATC_CMD_TYPE_TEST == ATC_GET_CMD_TYPE)
    {
        if(ERR_MNSMS_NONE != MNSMS_GetSmsNumEx(dual_sys, s_sms_config[dual_sys].mem1, &total_amount, &mn_sms_num))
        {
            return S_ATC_FAIL;
        }

        sprintf((char *)g_rsp_str, "%s: (1-%d)",
                g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr,
                total_amount);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }
    else if(ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE)
    {
        if(!PARAM1_FLAG)
        {
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
            return S_ATC_DEFAULT_HANDLE;
        }

        if(MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem1)
        {
            rec_id = (MN_SMS_RECORD_ID_T)PARA1.num ;
            index = rec_id;

            if(0xFFFF == index)
            {
                return ERR_INVALID_INDEX;
            }

            index--;
        }
        else
        {
            index = (MN_SMS_RECORD_ID_T)PARA1.num - 1;
        }

        MNSMS_GetSmsNumEx(dual_sys, s_sms_config[dual_sys].mem1, &total_amount, &mn_sms_num);

        if(0 == total_amount)
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }

        status_ptr = SCI_ALLOC_BASE(total_amount * sizeof(MN_SMS_STATUS_E));

        if(NULL == status_ptr)
        {
            return ERR_MEMORY_FULL;
        }

        if(ERR_MNSMS_NONE != MNSMS_GetSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem1, &mn_sms_num, status_ptr)
                || index >= total_amount ||
                MN_SMS_FREE_SPACE == status_ptr[index])
        {
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
        }
        else
        {
            s_sms_record[dual_sys].cmd_id    = AT_CMD_CMGR;
            MNSMS_ReadSmsEx(dual_sys, s_sms_config[dual_sys].mem1, index);

            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_READ_SMS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
        }

        SCI_FREE(status_ptr);
    }
    else
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_303);
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the AT+CSCA command to set the short
//                message service centre.
//  AT+CSCA=<scaddress>[,<tosc>]
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSCA)
{
    ATC_TELE_TYPE_E        tosca = ISDN_TELE_PLAN_UNKNOWN_TYPE;
    uint8                  tel_str_for_TE[ATC_MAX_SMS_ADDRESS_LEN_FOR_TE] = {0} ;
    uint16                 tel_str_for_TE_len = 0;
    ERR_MNSMS_CODE_E       result;
    MN_DUAL_SYS_E          dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(!atc_config_ptr->sms_is_ready[dual_sys])
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessCSCA, sms is not ready!!!");
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }

    switch(ATC_GET_CMD_TYPE)
    {
            // AT+CSCA=<sca>[,<tosa>]
        case ATC_CMD_TYPE_SET:

            // check the parameter 1
            if(!PARAM1_FLAG || ATC_CMD_PARAM_TYPE_STRING != PARAM1_TYPE)
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                return S_ATC_DEFAULT_HANDLE;
            }

            /* <tosca> */
            if(PARAM2_FLAG)
            {
                tosca = PARA2.num;
            }

            SCI_TRACE_LOW("ATC: CSCA len = %d",PARA1.str_ptr->str_len);
            if(PARA1.str_ptr->str_len != 0)
            {
                if(!ATC_DecodeSMSAddrChsetFromTE(PARA1.str_ptr, (uint8)tosca,
                                                 atc_config_ptr->cscs[dual_sys],
                                                 &g_sc_addr[dual_sys]))
                {
                    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                    return S_ATC_DEFAULT_HANDLE;
                }
            }
            else
            {
                SCI_MEMSET(&g_sc_addr[dual_sys], 0, sizeof(MN_CALLED_NUMBER_T));
            }

            MNSMS_SetScAddrEx(dual_sys, &g_sc_addr[dual_sys]);
            break;

            // AT+CSCA?
        case ATC_CMD_TYPE_READ:

            result = MNSMS_GetScAddrEx(dual_sys, &g_sc_addr[dual_sys]);
            
            SCI_TRACE_LOW("ATC: Get sc result = %d", result);
            if(result == ERR_MNSMS_NO_SC_ADDRESS)
            {
                sprintf((char *)g_rsp_str, "%s: ",
                    g_atc_info_table[AT_CMD_CSCA].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }
            else if(result != ERR_MNSMS_NONE)
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_330);
                return S_ATC_DEFAULT_HANDLE;
            }
            
            if(g_sc_addr[dual_sys].num_len == 0)
            {
                sprintf((char *)g_rsp_str, "%s: ",
                    g_atc_info_table[AT_CMD_CSCA].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }

            if(!ATC_EncodeCalledNumToTE(&g_sc_addr[dual_sys], atc_config_ptr->cscs[dual_sys], &tosca,
                                        tel_str_for_TE, &tel_str_for_TE_len))
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_330);
                return S_ATC_DEFAULT_HANDLE;
            }

            sprintf((char *)g_rsp_str, "%s: \"%s\",%d",
                    g_atc_info_table[AT_CMD_CSCA].cmd_name->str_ptr, tel_str_for_TE, tosca);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the AT+CNMI command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCNMI)
{
#ifndef _ULTRA_LOW_CODE_
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
            // AT+CNMI=<mode>,<mt>,<bm>,<ds>
        case ATC_CMD_TYPE_SET:

            // <mt>
            if(PARAM2_FLAG)
            {
                s_sms_config[dual_sys].mt_ind = (uint8)PARA2.num;
                MNSMS_SetNewSmsIndModeEx(dual_sys,s_sms_config[dual_sys].mt_ind);
            }

            // <bm>
            if(PARAM3_FLAG)
            {
                s_sms_config[dual_sys].bm_ind = (uint8)PARA3.num;
            }

            // <ds>
            if(PARAM4_FLAG)
            {
                s_sms_config[dual_sys].ds_ind = (uint8)PARA4.num;
                if(s_sms_config[dual_sys].ds_ind == 1)
                {
                    MNSMS_SetStautsRptSaveModeEx(dual_sys,FALSE);
                }
                else
                {
                    MNSMS_SetStautsRptSaveModeEx(dual_sys,TRUE);
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

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handles the AT+CMGD command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGD)
{
    MN_SMS_RECORD_ID_T    index;
    MN_SMS_RECORD_ID_T    total_amount;
    MN_SMS_RECORD_ID_T    mn_sms_num;
    MN_SMS_STATUS_E       *status_ptr = PNULL;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(!atc_config_ptr->sms_is_ready[dual_sys] && MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem1)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }

    if(ATC_CMD_TYPE_TEST == ATC_GET_CMD_TYPE)
    {
        if(ERR_MNSMS_NONE != MNSMS_GetSmsNumEx(dual_sys, s_sms_config[dual_sys].mem1, &total_amount, &mn_sms_num))
        {
            return S_ATC_FAIL;
        }

        sprintf((char *)g_rsp_str, "%s: (1-%d),(0-4)",
                g_atc_info_table[AT_CMD_CMGD].cmd_name->str_ptr,
                total_amount);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }
    else if(ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE)
    {
        if((!PARAM1_FLAG) ||
                (PARAM1_FLAG && PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC) ||
                (PARAM2_FLAG && PARAM2_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC))
        {
            //invalid params
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
            return S_ATC_DEFAULT_HANDLE;
        }

        //Delete the SMS by condition
        if(PARAM2_FLAG && ATC_DEL_INDEX != PARA2.num)
        {
            index = 0;//while not delete by index,ignore the index
            s_sms_record[dual_sys].cmgl_idex = index;
            s_sms_record[dual_sys].cmgd_state = (ATC_SMS_DEL_STATE_E)PARA2.num;

            if(DeleteSpecSms(dual_sys, s_sms_config[dual_sys].mem1, &index, s_sms_record[dual_sys].cmgd_state))
            {
                MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem1, index, MN_SMS_FREE_SPACE);
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_UPDATE_SMS_STATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
                                  APP_MN_DELETE_ALL_SMS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            }
            else
            {
                //none coincidental sms to be delete
                return S_ATC_SUCCESS;
            }
        }
        //delete the SMS by index.
        else
        {
            // get the parameter of index
            index = (MN_SMS_RECORD_ID_T)PARA1.num;

            if(MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem1)
            {
                if(0xFFFF == index)
                {
                    return ERR_INVALID_INDEX;
                }
            }

            index--;

            s_sms_record[dual_sys].cmgd_state = ATC_DEL_INDEX;

            MNSMS_GetSmsNumEx(dual_sys, s_sms_config[dual_sys].mem1, &total_amount, &mn_sms_num);

            if(0 == total_amount)
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            status_ptr = SCI_ALLOC_BASE(total_amount * sizeof(MN_SMS_STATUS_E));

            if(NULL == status_ptr)
            {
                return ERR_MEMORY_FULL;
            }

            if(ERR_MNSMS_NONE != MNSMS_GetSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem1, &mn_sms_num, status_ptr)
                    || index >= total_amount ||
                    MN_SMS_FREE_SPACE == status_ptr[index])
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
            }
            else
            {
                // delete the SM identified by index
                MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem1, index, MN_SMS_FREE_SPACE);
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_UPDATE_SMS_STATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            }

            SCI_FREE(status_ptr);
        }

        s_sms_record[dual_sys].cmd_id = AT_CMD_CMGD;
    }
    else
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_303);
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the AT+CMGS command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGS)
{
#ifndef _ULTRA_LOW_CODE_
    uint8             sm_octet_len; // indicate the number of octets coded in the TP layer data unit to be given
    uint8             toda = 0;       // the type of the destination address
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //+CMGS=? only return ok!
    if(ATC_GET_CMD_TYPE == ATC_CMD_TYPE_TEST)
    {
        return S_ATC_SUCCESS;
    }

    if(!atc_config_ptr->sms_is_ready[dual_sys])
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }

    // check the parameter
    if(!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_SET)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_303);
        return S_ATC_DEFAULT_HANDLE;
    }

    /*first sms sending exclusive from another one during its excution period.*/
    if (s_sms_exclusive_flag)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessCMGS A previous sms is sending!");
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    s_sms_record[dual_sys].more_sms_flag = FALSE;

    switch(s_sms_config[dual_sys].cmms_value)
    {
        case ATC_CMMS_DISABLE:
            /* disable */
            s_sms_record[dual_sys].more_sms_flag = FALSE;
            break;

        case ATC_CMMS_KEEP_ENABLED:
            /* keep enabled until the time between the response of
            * the latest message send command (+CMGS, +CMSS, etc.)
            * and the next send command exceeds 1-5 seconds (the exact
            * value is up to ME implementation), then ME shall close
                * the link and TA switches <n> automatically back to 0 */
            s_sms_record[dual_sys].more_sms_flag = s_sms_config[dual_sys].cmms_flag;
            break;

        case ATC_CMMS_ENABLED:
            /* enable (if the time between the response of the latest
            * message send command and the next send command exceeds
            * 1-5 seconds (the exact value is up to ME implementation),
            * ME shall close the link but TA shall not switch automatically
                * back to <n>=0) */
            s_sms_record[dual_sys].more_sms_flag = TRUE;
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    if(TRUE == s_sms_record[dual_sys].more_sms_flag)
    {
        /* stop CMMS  timer before send one of multiple messages and
        *acive it when has received the response of forward one
            *(+CMGS, +CMSS, etc.)*/
        SCI_TRACE_LOW("ATC:ATC_ProcessCMGS,Deactive CMMS timer");
        ATC_ActiveCMMSTimer(FALSE);
    }

    switch(PARAM1_TYPE)
    {
            // the parameter 1 is the numeric and the message mode is PDU
        case ATC_CMD_PARAM_TYPE_NUMERIC:
            sm_octet_len = (uint8)PARA1.num;

            if(!atc_config_ptr->atc_config_nv.cmgf_flag &&
                ((ATC_MIN_SM_LEN <= sm_octet_len) &&
                (sm_octet_len <= ATC_MAX_SM_LEN)))
            {
                SCI_TRACE_LOW("ATC:s_sms_record.length=%d", sm_octet_len);
                s_sms_record[dual_sys].length = sm_octet_len;
            }
            else
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_303);
                return S_ATC_DEFAULT_HANDLE;
            }

            break;

        case ATC_CMD_PARAM_TYPE_STRING:

            if(!atc_config_ptr->atc_config_nv.cmgf_flag)
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_303);
                return S_ATC_DEFAULT_HANDLE;
            }

            /* <toda> */
            if(PARAM2_FLAG)
            {
                toda = PARA2.num;
            }
            else
            {
                toda = ISDN_TELE_PLAN_UNKNOWN_TYPE;
            }

            if(!ATC_DecodeSMSAddrChsetFromTE(PARA1.str_ptr, toda,
                                             atc_config_ptr->cscs[dual_sys],
                                             &s_sms_record[dual_sys].dest_num))
            {
                ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                return S_ATC_DEFAULT_HANDLE;
            }

            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    s_sms_record[dual_sys].cmd_id = AT_CMD_CMGS;
    s_sms_record[dual_sys].state  = ATC_SMS_UNSENT;
    s_sms_exclusive_flag = TRUE;
    // output the '>'
    ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SMS);
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
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
AT_CMD_FUNC(ATC_ProcessCMGW)
{
    ATC_STATUS          status = S_ATC_SUCCESS;
    uint8               cmgf = atc_config_ptr->atc_config_nv.cmgf_flag;
    uint8               length = 0;
    MN_CALLED_NUMBER_T  oa;
    uint8               tooa = UNKNOWN_PLAN_UNKNOWN_TYPE;
    ATC_SMS_STATE_E     stat = ATC_SMS_UNSENT;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(&oa, 0, sizeof(MN_CALLED_NUMBER_T));

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            /* if text mode (+CMGF=1):
            * +CMGW[=<oa/da>[,<tooa/toda>[,<stat>]]]<CR>
            * text is entered<ctrl-Z/ESC>
            * if PDU mode (+CMGF=0):
            * +CMGW[=<length>[,<stat>]<CR>
            * PDU is given<ctrl-Z/ESC>
                */
            if(!PARAM1_FLAG)
            {
                /* NO <oa/da> in text mode or NO <length> in PDU mode. */
                SCI_TRACE_LOW("ATC: ATC_ProcessCMGW, no PARAM1");
                return ERR_OPERATION_NOT_ALLOWED;
            }

            /* if text mode (+CMGF=1):
            * +CMGW[=<oa/da>[,<tooa/toda>[,<stat>]]]<CR>
            * text is entered<ctrl-Z/ESC> */
            if(cmgf)
            {
                if((PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                        || (PARAM2_FLAG && (PARAM2_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC))
                        || (PARAM3_FLAG && (PARAM3_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)))
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessCMGW, text mode, invalid parameters");
                    return ERR_INVALID_INDEX;
                }

                if(PARAM2_FLAG)
                {
                    tooa = (uint8)PARA2.num;
                }

                if(!ATC_DecodeSMSAddrChsetFromTE(PARA1.str_ptr, tooa,
                                                 atc_config_ptr->cscs[dual_sys],
                                                 &s_sms_record[dual_sys].dest_num))
                {
                    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                    return S_ATC_DEFAULT_HANDLE;
                }

                if(PARAM3_FLAG)
                {
                    if(PARA3.num >= ATC_SMS_ALL)
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessCMGW, text mode, stat need less than ATC_SMS_ALL(%d)", ATC_SMS_ALL);
                        return ERR_INVALID_INDEX;
                    }

                    stat = (ATC_SMS_STATE_E)PARA3.num;
                }

                s_sms_record[dual_sys].state  = stat;
                s_sms_record[dual_sys].cmd_id = AT_CMD_CMGW;
                SCI_TRACE_LOW("ATC: ATC_ProcessCMGW, text mode, oa=%s,tooa=%d,stat=%d",
                              (PARA1.str_ptr->str_ptr), tooa, stat);
            }
            /* if PDU mode (+CMGF=0):
            * +CMGW[=<length>[,<stat>]<CR>
            * PDU is given<ctrl-Z/ESC> */
            else
            {
                if((PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
                        || (PARAM2_FLAG && (PARAM2_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC))
                        || (PARAM3_FLAG))
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessCMGW, PDU mode, invalid parameters");
                    return ERR_INVALID_INDEX;
                }

                if((ATC_MIN_SM_LEN <= PARA1.num) &&
                        (ATC_MAX_SM_LEN >= PARA1.num))
                {
                    length = (uint8)PARA1.num;
                }
                else
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessCMGW, PDU mode, length: NOT [%d, %d]", ATC_MIN_SM_LEN, ATC_MAX_SM_LEN);
                    return ERR_INVALID_INDEX;
                }

                if(PARAM2_FLAG)
                {
                    if(PARA2.num >= ATC_SMS_ALL)
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessCMGW, PDU mode, stat need less than ATC_SMS_ALL(%d)", ATC_SMS_ALL);
                        return ERR_INVALID_INDEX;
                    }

                    stat = (ATC_SMS_STATE_E)PARA2.num;
                }

                s_sms_record[dual_sys].length = length;
                s_sms_record[dual_sys].state  = stat;
                s_sms_record[dual_sys].cmd_id = AT_CMD_CMGW;
                SCI_TRACE_LOW("ATC: ATC_ProcessCMGW, PDU mode, length=%d,stat=%d", length, stat);
            }

            // output the character '>'
            ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SMS);

            return S_ATC_DEFAULT_HANDLE;

        case ATC_CMD_TYPE_TEST:
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+CMGC command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMGC)
{
#ifndef _ULTRA_LOW_CODE_
    uint8          sm_octet_len;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE || !PARAM1_FLAG)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
    {
        return ERR_INVALID_INDEX;
    }

    sm_octet_len = (uint8)PARA1.num;

    s_sms_record[dual_sys].cmd_id = AT_CMD_CMGC;
    s_sms_record[dual_sys].state  = ATC_SMS_UNSENT;
    s_sms_record[dual_sys].length = sm_octet_len;

    ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SMS);

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}

/*****************************************************************************/
//  Description : This function handles the AT+CMSS command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMSS)
{
    MN_SMS_RECORD_ID_T    total_num;
    MN_SMS_RECORD_ID_T    mn_sms_num;
    MN_SMS_STATUS_E       *status_ptr;
    uint8                  toda;
    ATC_STATUS             status = S_ATC_SUCCESS;
    uint16                 index;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(!atc_config_ptr->sms_is_ready[dual_sys])
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }

    /*first sms sending exclusive from another one during its excution period.*/
    if (s_sms_exclusive_flag)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessCMGS A previous sms is sending!");
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            {
                s_sms_record[dual_sys].dest_num_flag = FALSE;

                if(!PARAM1_FLAG)
                {
                    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
                    return S_ATC_DEFAULT_HANDLE;
                }

                if(MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem2)
                {
                    index = PARA1.num;

                    if(0xFFFF == index)
                    {
                        return ERR_INVALID_INDEX;
                    }

                    index--;
                }
                else
                {
                    index = PARA1.num - 1;
                }

                if(PARAM2_FLAG)
                {
                    if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessCMSS,ERROR, param2 is not string!");
                        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                        return S_ATC_DEFAULT_HANDLE;
                    }

                    if(PARAM3_FLAG)
                    {
                        toda = PARA3.num;
                    }
                    else
                    {
                        toda = ISDN_TELE_PLAN_UNKNOWN_TYPE;
                    }

                    if(!ATC_DecodeSMSAddrChsetFromTE(PARA2.str_ptr, toda,
                                                     atc_config_ptr->cscs[dual_sys],
                                                     &s_sms_record[dual_sys].dest_num))
                    {
                        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                        return S_ATC_DEFAULT_HANDLE;
                    }

                    s_sms_record[dual_sys].dest_num_flag = TRUE;
                }

                MNSMS_GetSmsNumEx(dual_sys, s_sms_config[dual_sys].mem2, &total_num, &mn_sms_num);

                if(0 == total_num)
                {
                    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                    return S_ATC_DEFAULT_HANDLE;
                }

                status_ptr = SCI_ALLOC_BASE(total_num * sizeof(MN_SMS_STATUS_E));

                if(NULL == status_ptr)
                {
                    return ERR_MEMORY_FULL;
                }

                //Get the SMS status
                if(ERR_MNSMS_NONE != MNSMS_GetSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem2, &mn_sms_num, status_ptr))
                {
                    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_301);
                    SCI_FREE(status_ptr);
                    return S_ATC_DEFAULT_HANDLE;
                }

                //Read the SMS.
                if(total_num >= PARA1.num ) //&& MN_SMS_MO_TO_BE_SEND == status_ptr[PARA1.num - 1])
                {
                    s_sms_record[dual_sys].index = (MN_SMS_RECORD_ID_T)index;
                    s_sms_record[dual_sys].cmd_id = AT_CMD_CMSS;
                    MNSMS_ReadSmsEx(dual_sys, s_sms_config[dual_sys].mem2, s_sms_record[dual_sys].index);
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_READ_SMS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                    s_sms_exclusive_flag = TRUE;    //first sms sending exclusive from another one during its excution period.
                }
                else
                {
                    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
                }

                SCI_FREE(status_ptr);
                // When receiving APP_MN_READ_SMS_CNF,
                //ATC_ProcessReadSmsCnf will call the api to send short message.
                status =  S_ATC_DEFAULT_HANDLE;
            }
            break;

        case ATC_CMD_TYPE_TEST:
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;

    }

    return status;
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
AT_CMD_FUNC(ATC_ProcessSMSC)
{
    MN_SMS_RECORD_ID_T    index;
    MN_SMS_RECORD_ID_T    total_amount;
    MN_SMS_RECORD_ID_T    mn_sms_num = 0;
    ATC_STATUS            status = S_ATC_DEFAULT_HANDLE;
    MN_SMS_STATUS_E       *status_ptr = PNULL;
    ATC_SMS_STATE_E       at_sms_status;
    MN_SMS_STATUS_E       sms_status;
    BOOLEAN               status_flag;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT(ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE || ATC_CMD_TYPE_TEST == ATC_GET_CMD_TYPE);/*assert verified: check valid value*/
    SCI_TRACE_LOW("ATC:enter ATC_ProcessSMSC");

    //check the SIM card
    if(!atc_config_ptr->sms_is_ready[dual_sys] && MN_SMS_STORAGE_SIM == s_sms_config[dual_sys].mem2)
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessSMSC: sms is not ready");
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG && PARAM2_FLAG)
            {
                index = (MN_SMS_RECORD_ID_T)(PARA1.num - 1);

                MNSMS_GetSmsNumEx(dual_sys, s_sms_config[dual_sys].mem2, &total_amount, &mn_sms_num);

                if(0 == total_amount)
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                status_ptr = (MN_SMS_STATUS_E *)SCI_ALLOC_BASE(total_amount * sizeof(MN_SMS_STATUS_E));

                if(NULL == status_ptr)
                {
                    return ERR_MEMORY_FULL;
                }

                if(ERR_MNSMS_NONE != MNSMS_GetSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem2, &mn_sms_num, status_ptr)
                        || index >= total_amount ||
                        MN_SMS_FREE_SPACE == status_ptr[index])
                {
                    ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
                    SCI_FREE(status_ptr);
                    return S_ATC_DEFAULT_HANDLE;
                }

                if((PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) &&
                        !atc_config_ptr->atc_config_nv.cmgf_flag)
                {
                    at_sms_status = (ATC_SMS_STATE_E)PARA2.num;

                    if(ChangeSmsStatus(status_ptr[index], &sms_status, at_sms_status, &status_flag))
                    {
                        if(status_flag)
                        {
                            SCI_TRACE_LOW("ATC: ATC_ProcessSMSC, status_flag = TRUE");
                            MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem2, index, sms_status);
                            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                              APP_MN_UPDATE_SMS_STATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                            s_sms_record[dual_sys].cmd_id = AT_CMD_SMSC;
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: ATC_ProcessSMSC, status_flag = FALSE");
                            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_302);
                        }
                    }
                    else
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessSMSC, ChangeSmsStatus is FALSE");
                        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
                    }

                    SCI_TRACE_LOW("ATC:cmgf:0,status_ptr=%d, sms_status=%d,at_sms_status=%d,status_flag=%d",
                                  status_ptr[index], sms_status, at_sms_status, status_flag);
                }
                // the parameter 2 is string type and the message format is text mode
                else if((PARAM2_TYPE == ATC_CMD_PARAM_TYPE_STRING) &&
                        atc_config_ptr->atc_config_nv.cmgf_flag)
                {
                    PARA2.str_ptr->str_ptr[PARA2.str_ptr->str_len] = '\0';

                    if(ConvertStrStatusToNumStatus(PARA2.str_ptr->str_ptr, &at_sms_status))
                    {
                        if(ChangeSmsStatus(status_ptr[index], &sms_status, at_sms_status, &status_flag))
                        {
                            if(status_flag)
                            {
                                MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem2, index, sms_status);
                                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                                  APP_MN_UPDATE_SMS_STATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
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

                        SCI_TRACE_LOW("ATC:cmgf:1,status_ptr=%d, sms_status=%d,at_sms_status=%d,status_flag=%d",
                                      status_ptr[index], sms_status, at_sms_status, status_flag);
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
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_CMD_TYPE_TEST:
            {
                if(ERR_MNSMS_NONE != MNSMS_GetSmsNumEx(dual_sys, s_sms_config[dual_sys].mem2, &total_amount, &mn_sms_num))
                {
                    return S_ATC_FAIL;
                }

                if(0 == atc_config_ptr->atc_config_nv.cmgf_flag)
                {
                    sprintf((char *)g_rsp_str, "%s: (1-%d),(0-3)",
                            g_atc_info_table[AT_CMD_SMSC].cmd_name->str_ptr,
                            total_amount);
                }
                else
                {
                    sprintf((char *)g_rsp_str, "%s: (1-%d),(\"REC UNREAD\",\"REC READ\",\"STO UNSENT\",\"STO SENT\") ",
                            g_atc_info_table[AT_CMD_SMSC].cmd_name->str_ptr,
                            total_amount);
                }

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                status = S_ATC_SUCCESS;
            }
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+SMSC command. This command is used
//                to set unchange the sms status
//  AT+SUSS=<mode>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSUSS)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS            status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                s_sms_config[dual_sys].suss_flag = (uint8)PARA1.num;
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SUSS].cmd_name->str_ptr, s_sms_config[dual_sys].suss_flag);
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
//  Description : This function handles the AT+CNMA command.
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCNMA)
{
    ATC_STATUS            status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_L3_MSG_UNIT_T  tpdu;
    
#ifndef _ULTRA_LOW_CODE_
    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_EXECUTE:
        if(s_sms_config[dual_sys].service_flag == 1)
        {
            tpdu.length = 0;
            if( s_sms_cnma[dual_sys].is_waiting_for_cnma )
            {
                s_sms_cnma[dual_sys].is_waiting_for_cnma = FALSE;
                SCI_DeactiveTimer( s_sms_cnma[dual_sys].cnma_timer );
                MNSMS_SendATSmsDlvrRptEx(dual_sys,s_sms_cnma[dual_sys].cnma_index,MN_SMS_ACK_DELIVER_REPORT,&tpdu);
                status = S_ATC_SUCCESS;
            }
            else
            {
                status = S_ATC_SUCCESS; 
            }
        }
        else
        {
            SCI_TRACE_LOW("ATC: CSMS = 0 ,CNMA not support");
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_340);
        }
        break;
        
        
    case ATC_CMD_TYPE_SET:
        if(s_sms_config[dual_sys].service_flag == 1)
        {
            //pdu mode
            if(atc_config_ptr->atc_config_nv.cmgf_flag == 0)
            {
                if(PARAM1_FLAG && !PARAM2_FLAG)
                {
                    if( s_sms_cnma[dual_sys].is_waiting_for_cnma )
                    {
                        s_sms_cnma[dual_sys].is_waiting_for_cnma = FALSE;
                        SCI_DeactiveTimer( s_sms_cnma[dual_sys].cnma_timer );
                    }
                    
                    tpdu.length = 0;
                    if(PARA1.num == 0 || PARA1.num == 1)
                    {
                        MNSMS_SendATSmsDlvrRptEx(dual_sys,s_sms_cnma[dual_sys].cnma_index,MN_SMS_ACK_DELIVER_REPORT,&tpdu);
                        status = S_ATC_SUCCESS;         
                    }
                    else if(PARA1.num == 2)
                    {
                        MNSMS_SendATSmsDlvrRptEx(dual_sys,s_sms_cnma[dual_sys].cnma_index,MN_SMS_NACK_DELIVER_REPORT,&tpdu);
                        status = S_ATC_SUCCESS;
                    }
                    else
                    {
                        status = ERR_OPERATION_NOT_ALLOWED;
                    }
                }
                else if(PARAM1_FLAG && PARAM2_FLAG)
                {
                    s_sms_cnma[dual_sys].cnma_n = PARA1.num;
                    s_sms_cnma[dual_sys].cnma_length = PARA2.num;
                    // output the '>'
                    ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SMS);
                }
            }
            else
            {
                //text mode
                if( s_sms_cnma[dual_sys].is_waiting_for_cnma )
                {
                    s_sms_cnma[dual_sys].is_waiting_for_cnma = FALSE;
                    SCI_DeactiveTimer( s_sms_cnma[dual_sys].cnma_timer );
                }
                tpdu.length = 0;
                MNSMS_SendATSmsDlvrRptEx(dual_sys,s_sms_cnma[dual_sys].cnma_index,MN_SMS_ACK_DELIVER_REPORT,&tpdu);
                status = S_ATC_SUCCESS;
            }
        }
        else
        {
            SCI_TRACE_LOW("ATC: CSMS = 0 ,CNMA not support");
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_340);
        }
        break;
        
    case ATC_CMD_TYPE_TEST:
        if(0 == atc_config_ptr->atc_config_nv.cmgf_flag)
        {
            sprintf((char*)g_rsp_str, "+CNMA: (0-2)");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            return S_ATC_SUCCESS;
        }
        break;
    default:
        return  ERR_OPERATION_NOT_ALLOWED;
    }
#endif /* _ULTRA_LOW_CODE_ */
    
    return status;  
}

/*****************************************************************************/
//  Description : This function handles the PDU send from the SIO.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSmsPdu(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *pdu_ptr  // pointer to the given pdu buffer
)
{
    uint8       terminater_char;     // indicates ther terminater char
    uint16      pdu_len;          // holds the gived pdu length
    BOOLEAN     flag = FALSE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/

    pdu_len              = (uint16)strlen((char *)pdu_ptr);
    SCI_ASSERT(0 < pdu_len);/*assert verified: check valid value*/

    terminater_char      = pdu_ptr[pdu_len - 1];
    pdu_ptr[pdu_len - 1] = '\0';
    pdu_len             -= 1;

    SCI_TRACE_LOW("ATC: The record length is %d,pdu_len:%d",
                  s_sms_record[dual_sys].length, pdu_len);

    switch(terminater_char)
    {
            // cancel the operation
        case ESC:
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            /*cancel the restriction of sms sending exclusive for another sms.*/
            if (s_sms_exclusive_flag && (AT_CMD_CMGS == s_sms_record[dual_sys].cmd_id))
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessSmsPdu permit another sms sending again!");
                s_sms_exclusive_flag = FALSE;
            }

            SCI_MEMSET(&s_sms_record[dual_sys], 0, sizeof(s_sms_record[dual_sys]));
            flag = TRUE;
            break;

        case CTRL_Z:
            //@Shijun.cui add CR29191
            //while mem full, PS will report write cnf and full ind message
            // in any way, AT report memory full indication to user

            // text mode SMS
            if(atc_config_ptr->atc_config_nv.cmgf_flag)
            {
                flag = HandleTextModeSms(dual_sys, atc_config_ptr, pdu_ptr, pdu_len);
            }
            // pdu mode SMS
            else if(s_sms_record[dual_sys].length <= (pdu_len / 2))
            {
                SCI_TRACE_LOW("ATC:Call HandlePduModeSms()");
#ifdef _MUX_ENABLE_
                flag = HandlePduModeSms(dual_sys, atc_config_ptr, pdu_ptr, pdu_len);
#else
                flag = HandlePduModeSms(dual_sys, pdu_ptr, pdu_len);
#endif
            }
            break;

        default:
            break;
    }

    SCI_TRACE_LOW("ATC:flag = %d", flag);

    //return the status
    if(!flag)
    {
        s_sms_exclusive_flag = FALSE;
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_304);
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the PDU send from the SIO.
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSmsStatusPdu(
                                          ATC_CONFIG_T    *atc_config_ptr,
                                          uint8           *pdu_ptr  // pointer to the given pdu buffer
                                          )
{
    uint8       terminater_char;     // indicates ther terminater char
    uint8       tpdu_len;          // holds the gived pdu length
    MN_L3_MSG_UNIT_T  tpdu;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    SCI_ASSERT(PNULL != atc_config_ptr);/* assert verified: null pointer */
    SCI_ASSERT(PNULL != pdu_ptr);/* assert verified: null pointer */
    
    tpdu_len              = (uint8)strlen((char*)pdu_ptr);
    terminater_char      = pdu_ptr[tpdu_len - 1];
    tpdu_len             -= 1;
    
    
    switch (terminater_char)
    {
        // cancel the operation
    case ESC:
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        break;
        
    case CTRL_Z:
        if(tpdu_len/2 != s_sms_cnma[dual_sys].cnma_length)
        {
            SCI_TRACE_LOW("ATC: The cnma input length is error %d != %d",s_sms_cnma[dual_sys].cnma_length,tpdu_len);
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_304); //cr166450
            return S_ATC_DEFAULT_HANDLE;
        }

        //deactive timer
        if( s_sms_cnma[dual_sys].is_waiting_for_cnma )
        {
            s_sms_cnma[dual_sys].is_waiting_for_cnma = FALSE;
            SCI_DeactiveTimer( s_sms_cnma[dual_sys].cnma_timer );
        }

        if(!ConvertHexToBin(pdu_ptr, tpdu_len, tpdu.l3_msg))
        {
            return FALSE;
        }

        tpdu.length = tpdu_len / 2;

        if(s_sms_cnma[dual_sys].cnma_n == 0 || s_sms_cnma[dual_sys].cnma_n == 1)
        {
            MNSMS_SendATSmsDlvrRptEx(dual_sys,s_sms_cnma[dual_sys].cnma_index,MN_SMS_ACK_DELIVER_REPORT,&tpdu);
        }
        else if(s_sms_cnma[dual_sys].cnma_n == 2)
        {
            MNSMS_SendATSmsDlvrRptEx(dual_sys,s_sms_cnma[dual_sys].cnma_index,MN_SMS_NACK_DELIVER_REPORT,&tpdu);
        }
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        break;
        
    default:
        break;
    }
    return S_ATC_DEFAULT_HANDLE;
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

                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_SET_SMS_RECEIVE_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
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

#ifdef ATC_SAT_ENABLE
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
    flag = HandlePduModeSms(dual_sys, atc_config_ptr,pdu_ptr, pdu_len);
#else
    flag = HandlePduModeSms(dual_sys, pdu_ptr, pdu_len);
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
    flag = HandlePduModeSms(dual_sys, atc_config_ptr,pdu_ptr, pdu_len);
    #else
    flag = HandlePduModeSms(dual_sys, pdu_ptr, pdu_len);
    #endif
    
    return S_ATC_DEFAULT_HANDLE;
#endif
}

#else
/*****************************************************************************/
//  Description : This function is used parse the cb parameters for AT+CSCB
//  Global resource dependence :
//  Author:       Yi.Jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_ParseCSCBParams(
    uint8          *str_ptr,         // in : the input string type param
    uint8           str_len,         // the input string length
    uint8           *digit_num,   // Out: return the obtained number of digits
    uint16          *digit_array,   // Out: return the obtained array of digits
    uint8            max
);


/*****************************************************************************/
//  Description : This function delete duplicated items(mid/dcs) for AT+CSCB
//  Global resource dependence :
//  Author:       Elvis xu
//  Note:
/*****************************************************************************/
BOOLEAN ATC_SmsCBDelDupItem(uint16 *digit_array, uint8 *digit_num)
{
    BOOLEAN    is_new = TRUE;
    BOOLEAN    is_dup = FALSE;
    uint16      *tmp_array = PNULL;
    uint8         num = 0;
    uint8 i, j;

    if(!digit_array || !digit_num)
    {
        return FALSE;
    }
    else if(*digit_num < 2)
    {
        return TRUE;
    }

    tmp_array = (uint16 *)SCI_ALLOC_BASE(*digit_num * sizeof(uint16));

    if(!tmp_array)
    {
        return FALSE;
    }

    tmp_array[num++] = digit_array[0];

    for(i = 1; i < *digit_num; i++)
    {
        is_new = TRUE;

        for(j = 0; j < num; j++)
        {
            if(digit_array[i] == tmp_array[j])
            {
                is_new = FALSE;
                is_dup = TRUE;
                break;
            }
        }

        if(is_new)
        {
            tmp_array[num++] = digit_array[i];
        }
    }

    if(is_dup)
    {
        *digit_num = num;

        for(i = 0; i < num; i++)
        {
            digit_array[i] = tmp_array[i];
        }
    }

    SCI_FREE(tmp_array);

    return TRUE;
}

/*****************************************************************************/
//  Description : This function used to add new msg id or language id Item for AT+CSCB
//  Global resource dependence :
//  Author:       Elvis xu
//  Note:
/*****************************************************************************/
BOOLEAN ATC_SmsCBAddItem(
    uint16 *dst_set_ptr,
    uint8 *dst_num_ptr,
    uint16 *src_set_ptr,
    uint8 src_num,
    uint8 max
)
{
    BOOLEAN    status = FALSE;

    if (src_num == 0)
    {
        status = TRUE;
    }
    else if (*dst_num_ptr == 0)
    {
        memcpy((void *)dst_set_ptr, (void *)src_set_ptr, src_num * sizeof(uint16));
        *dst_num_ptr = src_num;
        status = TRUE;
    }
    else
    {
        uint16        *tmp_set = PNULL;
        uint8          tmp_num = 0;

        tmp_set = (uint16 *)SCI_ALLOC_BASE(2 * max * sizeof(uint16));
        if (PNULL == tmp_set)
        {
            return FALSE;
        }

        memcpy((void *)&tmp_set[0], (void *)dst_set_ptr, *dst_num_ptr * sizeof(uint16));
        memcpy((void *)&tmp_set[*dst_num_ptr], (void *)src_set_ptr, src_num * sizeof(uint16));
        tmp_num = *dst_num_ptr + src_num;

        if (!ATC_SmsCBDelDupItem(tmp_set, &tmp_num))
        {
            SCI_FREE(tmp_set);
            return FALSE;
        }

        if (tmp_num > max)
        {
            status = FALSE;
        }
        else
        {
            SCI_MEMSET((void *)dst_set_ptr, 0, max * sizeof(uint16));
            memcpy((void *)dst_set_ptr, (void *)&tmp_set[0], tmp_num * sizeof(uint16));
            *dst_num_ptr = tmp_num;
            status = TRUE;
        }

        SCI_FREE(tmp_set);
    }
  
    return status;
}


/*****************************************************************************/
//  Description : This function delete mid or lang_id items from the setting mid set for AT+CSCB
//  Global resource dependence :
//  Author:       Elvis xu
//  Note:
/*****************************************************************************/
BOOLEAN ATC_SmsCBDelItem(
    uint16 *dst_set_ptr,
    uint8 *dst_num_ptr,
    uint16 *del_set_ptr,
    uint8 del_num
)
{
    BOOLEAN    is_found = FALSE;
    BOOLEAN    status = FALSE;
    uint8        i, j;

    if(!dst_set_ptr || !del_set_ptr || !dst_num_ptr)
    {
        return FALSE;
    }

    if(del_num == 0)
    {
        return TRUE;
    }
    else if(*dst_num_ptr == 0)
    {
        return FALSE;
    }

    for(i = 0; i < del_num; i++)
    {
        is_found = FALSE;

        for(j = 0; j < *dst_num_ptr; j++)
        {
            if(is_found)
            {
                if(j < *dst_num_ptr - 1)
                {
                    dst_set_ptr[j] = dst_set_ptr[j + 1];
                }
                else
                {
                    dst_set_ptr[j] = 0;
                    (*dst_num_ptr) --;
                }
            }
            else if(dst_set_ptr[j] == del_set_ptr[i])
            {
                is_found = TRUE;
                status = TRUE;

                if(j < *dst_num_ptr - 1)
                {
                    dst_set_ptr[j] = dst_set_ptr[j + 1];
                }
                else
                {
                    dst_set_ptr[j] = 0;
                    (*dst_num_ptr) --;
                }
            }
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+CSCB command. This command is used
//                to select which type CBM to be received by the ME.
//  AT+CSCB=[<mode>[,<mids>[,<dcss>]]]
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSCB)
{
    ATC_STATUS     status = S_ATC_SUCCESS;
#ifndef _ULTRA_LOW_CODE_
    ATC_CB_MSG_ID_MUSTER_T            msg_arr[MN_SYS_NUMBER] = {0};
    MN_CB_LANGUAGE_ID_MUSTER_T        lang_arr[MN_SYS_NUMBER] = {0};
    MN_DUAL_SYS_E                     dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint8  *temp_ptr = PNULL;
    int32    i;


    // get the cell broadcast parameters
    if(NVERR_NONE != EFS_NvitemRead(NV_ATC_CB_MSG_ID, 
                    MN_SYS_NUMBER * sizeof(ATC_CB_MSG_ID_MUSTER_T), (uint8 *)msg_arr)) 
    {
        SCI_TRACE_LOW("ATC: Read msg from NV fail");
        SCI_MEMSET((void *)msg_arr, 0, MN_SYS_NUMBER * sizeof(ATC_CB_MSG_ID_MUSTER_T));
    }

    if(NVERR_NONE != EFS_NvitemRead(NV_ATC_CB_LANG_ID, 
                    MN_SYS_NUMBER * sizeof(MN_CB_LANGUAGE_ID_MUSTER_T), (uint8 *)lang_arr))
    {
        SCI_TRACE_LOW("ATC: Read language from NV fail");
        SCI_MEMSET((void *)lang_arr, 0, MN_SYS_NUMBER * sizeof(MN_CB_LANGUAGE_ID_MUSTER_T));
    }

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (0,1)", g_atc_info_table[AT_CMD_CSCB].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;

        case ATC_CMD_TYPE_SET:
            if(PARAM1_FLAG)
            {
                // 0: msgs and dcss are accepted; 1: otherwise
                atc_config_ptr->smscb_mode[dual_sys] = (BOOLEAN)PARA1.num;

                // check parameter 2: the msg identity
                if(PARAM2_FLAG)
                {
                    uint16    mid_array[MN_CB_MSG_ID_MAX_NUM] = {0}; // to get parameter msg id array
                    uint8      mid_num = 0;

                    if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                    {
                        SCI_TRACE_LOW("ATC: CSCB param2 is not string type.");
                        return ERR_INVALID_INDEX;
                    }

                    if(!ATC_ParseCSCBParams(PARA2.str_ptr->str_ptr,
                                            PARA2.str_ptr->str_len, &mid_num, mid_array, MN_CB_MSG_ID_MAX_NUM))
                    {
                        SCI_TRACE_LOW("ATC: CSCB param2 is not correct values.");
                        return ERR_INVALID_INDEX;
                    }

                    //delete duplicated items
                    if(!ATC_SmsCBDelDupItem(mid_array, &mid_num))
                    {
                        return ERR_INVALID_INDEX;
                    }

                    for(i = 0; i < mid_num; i++)
                    {
                        SCI_TRACE_LOW("ATC: new msg_id[%d] = %d", i, mid_array[i]);
                        if(mid_array[i] > 999)
                        {
                            return ERR_INVALID_INDEX;
                        }
                    }

                    if(atc_config_ptr->smscb_mode[dual_sys])
                    {
                        if(!ATC_SmsCBDelItem(msg_arr[dual_sys].cb_msg_id.msg_id_arr, &(msg_arr[dual_sys].cb_msg_id.num), mid_array, mid_num))
                        {
                            SCI_TRACE_LOW("ATC: fail to delete msg id set targeted");
                            return ERR_NOT_FOUND;
                        }
                    }
                    else
                    {
                        if (!ATC_SmsCBAddItem(msg_arr[dual_sys].cb_msg_id.msg_id_arr, &(msg_arr[dual_sys].cb_msg_id.num), mid_array, mid_num, MN_CB_MSG_ID_MAX_NUM))
                        {
                            SCI_TRACE_LOW("ATC: fail to add new msg id set");
                            return ERR_INVALID_INDEX;
                        }
                    }

                    // check parameter 3: the dcs
                    if(PARAM3_FLAG)
                    {
                        uint16    dcs_array[MN_CB_LANGUAGE_ID_MAX_NUM] = {0}; // to get parameter dcs id array
                        uint8      dcs_num = 0;
                        uint16    lan_array[MN_CB_LANGUAGE_ID_MAX_NUM] = {0};
                        uint8      lan_num = 0;

                        if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                        {
                            SCI_TRACE_LOW("ATC:  CSCB param3 is not string type.");
                            return ERR_INVALID_INDEX;
                        }

                        if(!ATC_ParseCSCBParams(PARA3.str_ptr->str_ptr,
                                                PARA3.str_ptr->str_len, &dcs_num, dcs_array, MN_CB_LANGUAGE_ID_MAX_NUM))
                        {
                            SCI_TRACE_LOW("ATC: CSCB param3 is not correct values.");
                            return ERR_INVALID_INDEX;
                        }

                        //delete duplicated items
                        if(!ATC_SmsCBDelDupItem(dcs_array, &dcs_num))
                        {
                            return ERR_INVALID_INDEX;
                        }

                        for(i = 0; i < dcs_num; i++)
                        {
                            SCI_TRACE_LOW("ATC: new language_id[%d] = %d", i, dcs_array[i]);
                            switch(dcs_array[i])
                            {
                                case MN_SMSCB_GERMAN_ID:
                                case MN_SMSCB_ENGLISH_ID:
                                case MN_SMSCB_ITALIAN_ID:
                                case MN_SMSCB_FRENCH_ID:
                                case MN_SMSCB_SPANISH_ID:
                                case MN_SMSCB_DUTCH_ID:
                                case MN_SMSCB_SWEDISH_ID:
                                case MN_SMSCB_DANISH_ID:
                                case MN_SMSCB_PORTUGUESE_ID:
                                case MN_SMSCB_FINNISH_ID:
                                case MN_SMSCB_NORWEGIAN_ID:
                                case MN_SMSCB_GREEK_ID:
                                case MN_SMSCB_TURKISH_ID:
                                case MN_SMSCB_HUNGARIAN_ID:
                                case MN_SMSCB_POLISH_ID:
                                case MN_SMSCB_LANGUAGE_UNSPECIFIED:
                                case MN_SMSCB_CZECH_ID:
                                case MN_SMSCB_HEBREW_ID:
                                case MN_SMSCB_ARABIC_ID:
                                case MN_SMSCB_RUSSIAN_ID:
                                case MN_SMSCB_ICELANDIC_ID:
                                case MN_SMSCB_CHINESE_ID:
                                    break;

                                case MN_SMSCB_LANGUAGE_NUM:
                                default:
                                    SCI_TRACE_LOW("ATC: new language_id[%d] out of range", i);
                                    return ERR_INVALID_INDEX;
                            }
                        }

                        for (i = 0; i < lang_arr[dual_sys].num; i++)
                        {
                            lan_array[i] = (uint16)(lang_arr[dual_sys].language_id_arr[i]);
                        }
                        lan_num = lang_arr[dual_sys].num;

                        if(atc_config_ptr->smscb_mode[dual_sys])
                        {
                            if(!ATC_SmsCBDelItem(lan_array, &lan_num, dcs_array, dcs_num))
                            {
                                SCI_TRACE_LOW("ATC:  fail to delete language id set targeted");
                                return ERR_NOT_FOUND;
                            }
                        }
                        else
                        {
                            if (!ATC_SmsCBAddItem(lan_array, &lan_num, dcs_array, dcs_num, MN_CB_LANGUAGE_ID_MAX_NUM))
                            {
                                SCI_TRACE_LOW("ATC: fail to add new language id set targeted");
                                return ERR_INVALID_INDEX;
                            }
                        }

                        //update language id
                        for (i = 0; i < lan_num; i++)
                        {
                            lang_arr[dual_sys].language_id_arr[i] = (MN_SMSCB_LANGUAGE_ID_E)lan_array[i];
                        }
                        lang_arr[dual_sys].num = lan_num;
                    }

                    if(!atc_config_ptr->smscb_flag[dual_sys])
                    {
                        msg_arr[dual_sys].is_enable = TRUE;
                        MNSMSCB_StartCbMsgEx(dual_sys, &(msg_arr[dual_sys].cb_msg_id), &lang_arr[dual_sys]);
                        atc_config_ptr->smscb_flag[dual_sys] = TRUE;
                    }
                    else
                    {
                        MNSMSCB_ChangeCbParamEx(dual_sys, &(msg_arr[dual_sys].cb_msg_id), &lang_arr[dual_sys]);
                    }
                }
                else
                {
                    if(PARAM3_FLAG)
                    {
                        status = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }

                    SCI_MEMSET(&(msg_arr[dual_sys].cb_msg_id), 0, sizeof(MN_CB_MSG_ID_MUSTER_T));
                    SCI_MEMSET(&lang_arr[dual_sys], 0, sizeof(MN_CB_LANGUAGE_ID_MUSTER_T));

                    if (atc_config_ptr->smscb_mode[dual_sys])
                    {
                        msg_arr[dual_sys].is_enable = TRUE;
                        MNSMSCB_StartCbMsgEx(dual_sys, &(msg_arr[dual_sys].cb_msg_id), &lang_arr[dual_sys]);
                        atc_config_ptr->smscb_flag[dual_sys] = TRUE;
                    }
                    else
                    {
                        msg_arr[dual_sys].is_enable = FALSE;
                        MNSMSCB_StopCbMsgEx(dual_sys);
                        atc_config_ptr->smscb_flag[dual_sys] = FALSE;
                    }
                }

                // save msg_arr to NV
                if(NVERR_NONE != EFS_NvitemWrite(NV_ATC_CB_MSG_ID,
                                MN_SYS_NUMBER * sizeof(ATC_CB_MSG_ID_MUSTER_T), (uint8 *)msg_arr, FALSE))
                {
                    SCI_TRACE_LOW("ATC: Write NV_ATC_CB_MSG_ID failed");
                    return ERR_MEMORY_FAILURE;
                }
                                
                // save lang_arr to NV
                if (NVERR_NONE != EFS_NvitemWrite(NV_ATC_CB_LANG_ID,
                                 MN_SYS_NUMBER * sizeof(MN_CB_LANGUAGE_ID_MUSTER_T), (uint8 *)lang_arr, FALSE))
                {
                    SCI_TRACE_LOW("ATC: Write NV_ATC_CB_LANG_ID failed");
                    return ERR_MEMORY_FAILURE;
                }

            }
            else
            {
                if(PARAM2_FLAG || PARAM3_FLAG)
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                }
                else
                {
                    status = S_ATC_SUCCESS;
                }
            }

            break;

        case ATC_CMD_TYPE_READ:
            temp_ptr = g_rsp_str;
            sprintf((char *)temp_ptr, "%s: %d,\"", g_atc_info_table[AT_CMD_CSCB].cmd_name->str_ptr,
                    atc_config_ptr->smscb_mode[dual_sys]);
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];

            // print the message id
            for(i = 0; i < msg_arr[dual_sys].cb_msg_id.num; i++)
            {
                sprintf((char *)temp_ptr, "%d", msg_arr[dual_sys].cb_msg_id.msg_id_arr[i]);
                temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];

                if((msg_arr[dual_sys].cb_msg_id.num - 1) != i)
                {
                    sprintf((char *)temp_ptr, ",");
                    temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];
                }
            }

            // add the character'"'
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];
            *temp_ptr++ = '"';
            *temp_ptr++ = ',';
            *temp_ptr++ = '"';
            *temp_ptr = '\0';

            // print the language id
            for(i = 0; i < lang_arr[dual_sys].num; i++)
            {
                sprintf((char *)temp_ptr, "%d", lang_arr[dual_sys].language_id_arr[i]);
                temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];

                if((lang_arr[dual_sys].num - 1) != i)
                {
                    sprintf((char *)temp_ptr, ",");
                    temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];
                }
            }

            // add the character'"'
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];
            *temp_ptr++ = '"';
            *temp_ptr = '\0';
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

#endif /* _ULTRA_LOW_CODE_ */

    return status;
}

#endif

/*****************************************************************************/
//  Description : This function initializes the static variables for SMS module
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
void ATC_InitSmsConfig(ATC_CONFIG_T *atc_config_ptr)
{
    uint8 i = 0;

    // Set the default value
    for(i = MN_DUAL_SYS_1; i < MN_SYS_NUMBER; i++)
    {
        g_sms_ser_type[i]           = MN_SMS_CS_DOMAIN_SERVICE;

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
        s_sms_config[i].suss_flag  = TRUE;
        s_sms_config[i].cmms_value = ATC_CMMS_ENABLED;
        s_sms_config[i].cmms_flag  = FALSE;
        atc_config_ptr->sms_is_ready[i] = FALSE;
        s_sms_config[i].cmms_timer = SCI_CreateTimer("ATC CMMS Timer", ATC_ProcessCMMSTimeout, 0, ATC_CMMS_TIMER_VALUE, SCI_NO_ACTIVATE);

        SCI_MEMSET(&s_sms_record[i], 0, sizeof(s_sms_record[i]));
    }

    if(NVERR_NONE != EFS_NvitemRead(NV_SMS_CONFIG_ID, sizeof(ATC_SMS_NV_CONFIG_T), (uint8 *)&s_sms_nv_config))
    {
        s_sms_nv_config.fo         = ATC_SMS_FO_DEFAULT_VAL;
        s_sms_nv_config.dcs        = MN_SMS_DEFAULT_ALPHABET;
        s_sms_nv_config.vp         = ATC_SMS_VP_DEFAULT_VAL;
        s_sms_nv_config.pid        = ATC_SMS_PID_DEFAULT_VAL;

        //write the NV.
        if(NVERR_NONE != EFS_NvitemWrite(NV_SMS_CONFIG_ID, sizeof(ATC_SMS_NV_CONFIG_T), (uint8 *)&s_sms_nv_config, FALSE))
        {
            SCI_TRACE_LOW("ATC: write NV_ATC_CONFIG_ID fail");
        }
    }
}

/*****************************************************************************/
//  Description : This function restore the s_sms_config.service_flag
//  Global resource dependence :
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
void ATC_RestoreSmsConfig(MN_DUAL_SYS_E  dual_sys, ATC_CONFIG_T *atc_config_ptr)
{
    // Set the default value
    s_sms_config[dual_sys].service_flag = TRUE;

    //set +CNMI default value (+CNMI=[<mode>[,<mt>[,<bm>[,<ds>[,<bfr>]]]]])
    s_sms_config[dual_sys].mt_ind      = MT_IND_DEFAULT_VAL;
    s_sms_config[dual_sys].bm_ind      = BM_IND_DEFAULT_VAL;
    s_sms_config[dual_sys].ds_ind      = DS_IND_DEFAULT_VAL;
    s_sms_config[dual_sys].bfr_ind     = BFR_IND_DEFAULT_VAL;
    //set +CPMS default value (+CPMS=<mem1>[,<mem2>[,<mem3>]])
    s_sms_config[dual_sys].mem1        = MN_SMS_STORAGE_SIM;
    s_sms_config[dual_sys].mem2        = MN_SMS_STORAGE_SIM;
    s_sms_config[dual_sys].mem3        = MN_SMS_STORAGE_SIM;

    //set +csca default value (+CSCA=<sca>[,<tosca>])
    if(atc_config_ptr->sms_is_ready[dual_sys])
    {
        //get sc from SIM card
        if(ERR_MNSMS_NONE  != MNSMS_GetScAddrEx(dual_sys, &g_sc_addr[dual_sys]))
        {
            SCI_TRACE_LOW("ATC: ATC_RestoreSmsConfig, SIM %d Get sc failure", dual_sys);
        }
    }
    else
    {
        SCI_TRACE_LOW("ATC: ATC_RestoreSmsConfig, SIM %d not ready, cannot get sc", dual_sys);
    }
}



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
    int32          i;

    for(i = 0; i < ATC_STORAGE_MAX; i++)
    {
        //if (!strcmp(mem_str, s_sms_mem_str[i]))
        if(!stricmp(mem_str, s_sms_mem_str[i]))
        {
            *storage_id_ptr = (uint8)i;
            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : This function handles the binary format pdu string
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
#ifdef _MUX_ENABLE_
LOCAL BOOLEAN HandlePduModeSms(
    MN_DUAL_SYS_E         dual_sys,
    ATC_CONFIG_T    *atc_config_ptr,
    uint8         *pdu_ptr,       // point to the pdu buffer
    uint16         pdu_len
)
#else
LOCAL BOOLEAN HandlePduModeSms(
    MN_DUAL_SYS_E         dual_sys,
    uint8         *pdu_ptr,       // point to the pdu buffer
    uint16         pdu_len
)
#endif
{
    uint32                i;
    uint8                 cmd_id;

    MN_L3_MSG_UNIT_T      bin_pdu = {0};
    MN_SMS_ADDR_T         sc_addr = {0};        /* holds the Short Message Service Center code */
    MN_SMS_STATUS_E       status;
    BOOLEAN               flag = TRUE;
    ERR_MNSMS_CODE_E      err_code = 0;

    SCI_TRACE_LOW("ATC:enter HandlePduModeSms");

    SCI_ASSERT(PNULL != pdu_ptr);/*assert verified: check null pointer*/

    if((ATC_MESSAGE_SIZE < (pdu_len / 2))
            || !ConvertHexToBin(pdu_ptr, pdu_len, bin_pdu.l3_msg))
    {
        SCI_TRACE_LOW("ATC:error ConvertHexToBin");
        return FALSE;
    }

    bin_pdu.length = (uint16)(pdu_len / 2);
    cmd_id = (uint8)(s_sms_record[dual_sys].cmd_id);

    // get the SC address
    if(0 == (int16)(bin_pdu.l3_msg[0]))
    {
        SCI_TRACE_LOW("ATC:no SC address");
        
        if(g_sc_addr[dual_sys].num_len == 0)
        {
            SCI_TRACE_LOW("ATC:please set the SC address.");

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
            SCI_TRACE_LOW("ATC: HandlePduModeSms sc_addr.length = %d",
                          sc_addr.length);
            sc_addr.length = MN_SMS_ADDR_MAX_LEN;
        }

            SCI_MEMCPY(((char *)sc_addr.addr_arr_t + 1),
                       (char *)g_sc_addr[dual_sys].party_num,
                       (sc_addr.length - 1));
        }
        
        //copy the data
        bin_pdu.length --;

        for(i = 0; i < bin_pdu.length; i++)
        {
            bin_pdu.l3_msg[i] = bin_pdu.l3_msg[i + 1];
        }
    }
    else
    {
        SCI_TRACE_LOW("ATC:have SC address");
        sc_addr.length = (uint16)(bin_pdu.l3_msg[0]);
        if ((sc_addr.length < 1) || (sc_addr.length > MN_SMS_ADDR_MAX_LEN))
        {
            SCI_TRACE_LOW("ATC:SC address length %d out range(1-%d)", sc_addr.length, MN_SMS_ADDR_MAX_LEN);
            return FALSE;
        }

        SCI_MEMCPY(sc_addr.addr_arr_t, &bin_pdu.l3_msg[1], sc_addr.length);
        //copy the data
        bin_pdu.length = bin_pdu.length -((uint16)(bin_pdu.l3_msg[0]) + 1);
        for(i = 0; i < bin_pdu.length; i++)
        {
            bin_pdu.l3_msg[i] = bin_pdu.l3_msg[sc_addr.length + 1 + i];
        }
    }


    switch(cmd_id)
    {
            // AT+CMGS=<length>,<cr>,<pdu> <ctrl+z>
        case AT_CMD_CMGS:
            err_code = MNSMS_SendATSmsPDUEx(dual_sys, &sc_addr, &bin_pdu, s_sms_record[dual_sys].more_sms_flag);

            if( ERR_MNSMS_NONE == err_code)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_SEND_SMS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            }
            else
            {
                SCI_TRACE_LOW("ATC: call MNSMS_SendATSmsPDUEx failed, err_code: %d", err_code);

                s_sms_exclusive_flag = FALSE;
                flag = FALSE;
            }
            break;

            // AT+CMGC=<length>, <cr>, <pdu is given>,<ctrl+z>
        case AT_CMD_CMGC:
            MNSMS_AtcSendCommandEx(dual_sys, &sc_addr, &bin_pdu);
            break;

            // AT+CMGW=<length>,<cr>,<pdu is given>,<ctrl+z>
        case AT_CMD_CMGW:

            switch(s_sms_record[dual_sys].state)
            {
                    // the MT short message
                case ATC_SMS_UNREAD:
                    status = MN_SMS_MT_TO_BE_READ;
                    break;

                case ATC_SMS_READ:
                    status = MN_SMS_MT_READED;
                    break;

                    // the MO short message
                case ATC_SMS_UNSENT:
                    status = MN_SMS_MO_TO_BE_SEND;
                    break;

                case ATC_SMS_SENT:
                    status = MN_SMS_MO_SR_NOT_REQUEST;
                    break;

                default:
                    status = MN_SMS_MT_TO_BE_READ;
                    break;
            }

            if(ERR_MNSMS_NONE != MNSMS_AtcWriteSmsEx(dual_sys, s_sms_config[dual_sys].mem2, status, &sc_addr, &bin_pdu))
            {
                flag = FALSE;
                SCI_TRACE_LOW("ATC: failure in MNSMS_AtcWriteSms");
            }

#ifdef _MUX_ENABLE_
            else
            {
                ATC_Add_Expected_Event(atc_config_ptr, APP_MN_WRITE_SMS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            }

#endif
            break;

        default:
            flag = FALSE;
            break;
    }

    return flag;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_IND signal and send the
//                result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSmsInd(
    ATC_CONFIG_T       *atc_config_ptr,
    APP_MN_SMS_IND_T   *sig_ptr
)
{
    MN_SMS_RECORD_ID_T   index = 0;
    MN_DUAL_SYS_E   dual_sys = sig_ptr->dual_sys;
    MN_L3_MSG_UNIT_T  tpdu = {0};
    
    if(atc_config_ptr->urc_level !=ATC_URC_ALL_STATUS_ON
        &&  atc_config_ptr->urc_level  != ATC_URC_CALL_SMS_STATUS_ON)    
    {
        MNSMS_SendATSmsDlvrRptEx(dual_sys, sig_ptr->record_id, MN_SMS_NACK_DELIVER_REPORT, &tpdu);
        return S_ATC_DEFAULT_HANDLE;
    }

    if(g_atc_ps_not_save_sms[dual_sys]) 
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessSmsInd,ps not save sms.");
        s_sms_cnma[dual_sys].cnma_index = sig_ptr->record_id;

        if(atc_config_ptr->atc_config_nv.cmgf_flag)
        {
            HandleTextModeSmsInd(dual_sys, atc_config_ptr, &(sig_ptr->sms_t), index);
        }
        else
        {
            HandlePduModeSmsInd(dual_sys, atc_config_ptr, &(sig_ptr->sms_t), index);
        }

        return S_ATC_SUCCESS;
    }

    switch(s_sms_config[dual_sys].mt_ind)
    {
        case ATC_MT_NO_DELIVER:
            return S_ATC_DEFAULT_HANDLE;

            // +CMTI: <index>
        case ATC_MT_CMTI:
            index = (sig_ptr->record_id + 1);

            if(MN_SMS_STORAGE_SIM == sig_ptr->storage)
            {
                sprintf((char *)g_rsp_str, "+CMTI: \"SM\",%d", index);
            }
            else
            {
                sprintf((char *)g_rsp_str, "+CMTI: \"ME\",%d", index);
            }

            ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
            break;

        case ATC_MT_CMT_2:

            if(sig_ptr->sms_t.dcs.sms_class != MN_SMS_CLASS_2)
            {
                s_sms_cnma[dual_sys].cnma_index = sig_ptr->record_id;

                if(atc_config_ptr->atc_config_nv.cmgf_flag)
                {
                    HandleTextModeSmsInd(dual_sys, atc_config_ptr, &(sig_ptr->sms_t), index);
                }
                else
                {
                    HandlePduModeSmsInd(dual_sys, atc_config_ptr, &(sig_ptr->sms_t), index);
                }
            }
            else
            {
                index = (sig_ptr->record_id + 1);

                if(MN_SMS_STORAGE_SIM == sig_ptr->storage)
                {
                    sprintf((char *)g_rsp_str, "+CMTI: \"SM\",%d", index);
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+CMTI: \"ME\",%d", index);
                }

                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
            }

            break;

        case ATC_MT_CMT_3:

            if(sig_ptr->sms_t.dcs.sms_class == MN_SMS_CLASS_3)
            {
                s_sms_cnma[dual_sys].cnma_index = sig_ptr->record_id;

                if(atc_config_ptr->atc_config_nv.cmgf_flag)
                {
                    HandleTextModeSmsInd(dual_sys, atc_config_ptr, &(sig_ptr->sms_t), index);
                }
                else
                {
                    HandlePduModeSmsInd(dual_sys, atc_config_ptr, &(sig_ptr->sms_t), index);
                }
            }
            else
            {
                index = (sig_ptr->record_id + 1);

                if(MN_SMS_STORAGE_SIM == sig_ptr->storage)
                {
                    sprintf((char *)g_rsp_str, "+CMTI: \"SM\",%d", index);
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+CMTI: \"ME\",%d", index);
                }

                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
            }

            break;

            // +CMT:<length> CR LF <pdu>
        default:
            return S_ATC_DEFAULT_HANDLE;
    }

    return S_ATC_SUCCESS;

}

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
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and send the
//                result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessReadSmsCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_READ_SMS_CNF_T    *sig_ptr
)
{
    SCI_TRACE_LOW("ATC: received the APP_MN_READ_SMS_CNF signal");

    // check if the operation is successful
    if(MN_SMS_OPERATE_SUCCESS != sig_ptr->cause)
    {
        // return the error code "SIM fail"
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
        return S_ATC_DEFAULT_HANDLE;
    }

    if(s_sms_record[sig_ptr->dual_sys].cmd_id == AT_CMD_CMSS)
    {
        HandleCmssCmd(sig_ptr, atc_config_ptr);
        return S_ATC_DEFAULT_HANDLE;
    }

    if(atc_config_ptr->atc_config_nv.cmgf_flag)
    {
        HandleTextModeSmsReadCnf(sig_ptr->dual_sys, atc_config_ptr, &(sig_ptr->sms_t_u), sig_ptr->status, sig_ptr->record_id );
    }
    else
    {
        HandlePduModeSmsReadCnf(sig_ptr->dual_sys, atc_config_ptr, &(sig_ptr->sms_t_u), sig_ptr->status, sig_ptr->record_id);
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and send the
//                result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSendSmsCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_SEND_SMS_CNF_T    *sig_ptr
)
{
    SCI_TRACE_LOW("ATC:ATC_ProcessSendSmsCnf");
        
    /*cancel the restriction of sms sending exclusive for another sms.*/
    if (s_sms_exclusive_flag)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessSendSmsCnf permit another sms sending again!");
        s_sms_exclusive_flag = FALSE;
    }

    switch(sig_ptr->cause)
    {
        case MN_SMS_OPERATE_SUCCESS:

            switch(s_sms_record[sig_ptr->dual_sys].cmd_id)
            {
                case AT_CMD_CMSS:
                    sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CMSS].cmd_name->str_ptr, sig_ptr->tp_mr);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                    MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys, s_sms_config[sig_ptr->dual_sys].mem2, s_sms_record[sig_ptr->dual_sys].index, MN_SMS_MO_SR_NOT_REQUEST);
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                      APP_MN_UPDATE_SMS_STATE_CNF, 0xff, ATC_NO_DOMAIN, sig_ptr->dual_sys);
                    break;
                case AT_CMD_CMGS:
                    sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CMGS].cmd_name->str_ptr, sig_ptr->tp_mr);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                default:
                    return S_ATC_DEFAULT_HANDLE;
            }

            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            if((s_sms_record[sig_ptr->dual_sys].cmd_id == AT_CMD_CMSS || s_sms_record[sig_ptr->dual_sys].cmd_id == AT_CMD_CMGS) &&
                    TRUE == s_sms_config[sig_ptr->dual_sys].cmms_flag)
            {
                /* more messages are waiting to send, and the response of the latest message send command (+CMGS, +CMSS, etc.)
                * has been received,  invoke ATC_ActiveCMMSTimer to make sure the next send command will be done in timer duration
                    *(+CMGS, +CMSS, etc.)*/
                SCI_TRACE_LOW("ATC:ATC_ProcessSendSmsCnf, Active CMMS timer");
                ATC_ActiveCMMSTimer(TRUE);
            }

            break;

        default:
            if((0x11==sig_ptr->rl_cause)||(0x16==sig_ptr->rl_cause)||(MN_SMS_CAUSE_PROTOCOL_ERROR==sig_ptr->rl_cause))
            {
                ATC_UpSPERRORInfo(sig_ptr->dual_sys, atc_config_ptr, SMS_REJECT_CP_CLASS, sig_ptr->rl_cause);
            }
            else if((MN_SMS_CAUSE_DEST_OUT_OF_ORDER==sig_ptr->rl_cause)||(MN_SMS_CAUSE_NET_OUT_OF_ORDER==sig_ptr->rl_cause)
              ||(MN_SMS_CAUSE_TEMP_FAILURE==sig_ptr->rl_cause)||(MN_SMS_CAUSE_CONGESTION==sig_ptr->rl_cause)
              ||(MN_SMS_CAUSE_RESOURCES_UNAV==sig_ptr->rl_cause)||(MN_SMS_CAUSE_INTERWORKING==sig_ptr->rl_cause))
            {
                ATC_UpSPERRORInfo(sig_ptr->dual_sys, atc_config_ptr, SMS_REJECT_RP_CLASS, sig_ptr->rl_cause);
            }

            SCI_TRACE_LOW("ATC: Receive the error status: %d", sig_ptr->cause);
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
            break;
    }

    SCI_MEMSET(&s_sms_record[sig_ptr->dual_sys], 0, sizeof(ATC_SMS_RECORD_T));
    s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_UPDATE_SMS_STATUS_CNF signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessUpdateSmsCnf(
    ATC_CONFIG_T                    *atc_config_ptr,
    APP_MN_UPDATE_SMS_STATUS_CNF_T    *sig_ptr
)
{
    MN_SMS_RECORD_ID_T     del_index;

    switch(sig_ptr->cause)
    {
        case MN_SMS_OPERATE_SUCCESS:

            switch(s_sms_record[sig_ptr->dual_sys].cmd_id)
            {
                case AT_CMD_CMGR:
                    s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                    break;

                case AT_CMD_CMGL:
                    //finish reading one sms and read the next one
                    ReadNextSMS(sig_ptr->dual_sys, atc_config_ptr);
                    break;

                case AT_CMD_CMGD:

                    if(ATC_DEL_INDEX == s_sms_record[sig_ptr->dual_sys].cmgd_state)
                    {
                        if(MN_SMS_OPERATE_SUCCESS == sig_ptr->cause)
                        {
                            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                        }
                        else
                        {
                            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_301);
                        }

                        s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
                        break;
                    }
                    else
                    {
                        if(DeleteSpecSms(sig_ptr->dual_sys, s_sms_config[sig_ptr->dual_sys].mem1, &del_index, s_sms_record[sig_ptr->dual_sys].cmgd_state))
                        {
                            SCI_TRACE_LOW("ATC: Received APP_MN_UPDATE_SMS_STATE_CNF, sleep 30ms");
                            SCI_SLEEP(30); //wait a moment for APP handling APP_MN_UPDATE_SMS_STATE_CNF
                            // delete the SM identified by index
                            MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys, s_sms_config[sig_ptr->dual_sys].mem1, del_index, MN_SMS_FREE_SPACE);
                            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                              APP_MN_UPDATE_SMS_STATE_CNF, 0xff, ATC_NO_DOMAIN, sig_ptr->dual_sys);
                        }
                        else
                        {
                            /* here OK hould not be sent, just send APP_MN_DELETE_ALL_SMS_CNF*/
                            /*
                            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                            */
                            uint16           sig_size = 0;
                            uint16           sig_code = 0;
                            xSignalHeaderRec *dispatch_sig_ptr = PNULL;
                            uint32           task_id = SCI_IdentifyThread();

                            sig_code = APP_MN_DELETE_ALL_SMS_CNF;
                            sig_size = sizeof(APP_MN_DELETE_ALL_SMS_CNF_T);
                            SCI_CREATE_SIGNAL(
                                dispatch_sig_ptr ,
                                sig_code ,
                                sig_size ,
                                P_ATC
                            ) ;

                            /* Send the signal to the dedicated task's queue */
                            SCI_SEND_SIGNAL((xSignalHeaderRec *) dispatch_sig_ptr , task_id) ;
                            s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
                        }
                    }

                    break;

                case AT_CMD_SMSC:
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                    s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
                    break;

                default:
                    break;
            }

            break;

        default:
            //return the indication string
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_DELETE_ALL_SMS_CNF signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       george.yuan 2009-06-21
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessDeleteAllSmsCnf(
    ATC_CONFIG_T                     *atc_config_ptr,
    APP_MN_DELETE_ALL_SMS_CNF_T    *sig_ptr
)
{
    SCI_TRACE_LOW("ATC:  ATC_ProcessDeleteAllSmsCnf ");
    sprintf((char *)g_rsp_str, "OK");
    ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function get the specific storage's total amount and used
//                amount.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN GetStorageInfo(
    MN_DUAL_SYS_E   dual_sys,
    MN_SMS_STORAGE_E storage,     // in--get the where store sms's status, ME or SIM
    MN_SMS_RECORD_ID_T *total_ptr,    // out--the max number of sms can saved in ME or SIM
    MN_SMS_RECORD_ID_T *used_ptr,
    BOOLEAN            *unread_flag
)
{
    int32                 i;
    MN_SMS_STATUS_E       *status_ptr = PNULL;
    MN_SMS_RECORD_ID_T    mn_sms_num = 0;
    MN_SMS_RECORD_ID_T    total_count = 0;
    MN_SMS_RECORD_ID_T   used_count = 0;
    BOOLEAN                unread = FALSE;


    MNSMS_GetSmsNumEx(dual_sys, storage, &mn_sms_num, &used_count);

    if(0 == mn_sms_num)
    {
        SCI_TRACE_LOW("ATC: GetStorageInfo: WARNING, SmsNum is 0");
        return FALSE;
    }

    status_ptr = SCI_ALLOC_BASE(mn_sms_num * sizeof(MN_SMS_STATUS_E));

    if(NULL == status_ptr)
    {
        SCI_TRACE_LOW("ATC: GetStorageInfo: no mem, SmsNum is %d", mn_sms_num);
        return FALSE;
    }

    //Get SMS status
    if(ERR_MNSMS_NONE != MNSMS_GetSmsStatusEx(dual_sys, storage, &total_count, status_ptr))
    {
        SCI_FREE(status_ptr);
        return FALSE;
    }

    //Get the information about SMS
    for(i = 0; i < total_count; i++)
    {
        if(MN_SMS_MT_TO_BE_READ == status_ptr[i])
        {
            unread = TRUE;
        }
    }

    SCI_FREE(status_ptr);

    SCI_TRACE_LOW("ATC: total amout: %d, used amount:%d, unread flag:%d", total_count, used_count, unread);

    if(total_ptr)
    {
        *total_ptr = total_count;
    }

    if(used_ptr)
    {
        *used_ptr = used_count;
    }

    if(unread_flag)
    {
        *unread_flag = unread;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : This function get the specific sms information
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  ReadSpecSms(
    MN_DUAL_SYS_E   dual_sys,
    MN_SMS_STORAGE_E        storage,
    MN_SMS_RECORD_ID_T      *index_ptr,
    ATC_SMS_STATE_E         sms_state
)
{
    int32                 i;
    MN_SMS_RECORD_ID_T    total_num;
    MN_SMS_RECORD_ID_T    mn_sms_num;

    MN_SMS_STATUS_E       *status_ptr;


    if(ERR_MNSMS_NONE != MNSMS_GetSmsNumEx(dual_sys, storage, &total_num, &mn_sms_num))
    {
        return FALSE;
    }

    status_ptr = SCI_ALLOC_BASE(total_num * sizeof(MN_SMS_STATUS_E));

    if(NULL == status_ptr)
    {
        return FALSE;
    }

    //Get SMS status
    if(ERR_MNSMS_NONE != MNSMS_GetSmsStatusEx(dual_sys, storage, &total_num, status_ptr))
    {
        SCI_FREE(status_ptr);
        return FALSE;
    }

    //Get the defined SMS and change the index
    for(i = s_sms_record[dual_sys].cmgl_idex; i < total_num; i++)
    {
        switch(sms_state)
        {
            case ATC_SMS_UNREAD:

                if(MN_SMS_MT_TO_BE_READ == status_ptr[i] || MN_SMS_MT_SR_TO_BE_READ == status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }

                break;
            case ATC_SMS_READ:

                if(MN_SMS_MT_READED == status_ptr[i] || MN_SMS_MT_SR_READED == status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }

                break;
            case ATC_SMS_UNSENT:

                if(MN_SMS_MO_TO_BE_SEND == status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }

                break;
            case ATC_SMS_SENT:

                if((MN_SMS_MO_SR_NOT_REQUEST            == status_ptr[i]) ||
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
            case ATC_SMS_ALL:

                if(MN_SMS_FREE_SPACE != status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }

                break;
            default:
                SCI_ASSERT(FALSE);/*assert verified: check valid value*/
                break;

        }
    }

    SCI_FREE(status_ptr);
    s_sms_record[dual_sys].cmgl_idex = i + 1;
    SCI_TRACE_LOW("ATC: the current cmgl index: %d", s_sms_record[dual_sys].cmgl_idex);
    return FALSE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_UPDATE_SMS_STATUS_CNF signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessWriteSmsCnf(
    ATC_CONFIG_T                     *atc_config_ptr,
    APP_MN_WRITE_SMS_CNF_T            *sig_ptr
)
{
    if(s_sms_record[sig_ptr->dual_sys].cmd_id != AT_INVALID_CMD_ID)
    {
        //If success,return the indication string
        if(sig_ptr->cause == MN_SMS_OPERATE_SUCCESS)
        {
            uint16 rec_id;

            if(MN_SMS_STORAGE_SIM == sig_ptr->storage)
            {
                rec_id = sig_ptr->record_id;

                if(0xFFFF == rec_id)
                {
                    SCI_TRACE_LOW("ATC:WRITE error!");
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
                    s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
                    return S_ATC_DEFAULT_HANDLE;
                }

                /* rec_id--; george comment it */
            }
            else
            {
                rec_id = sig_ptr->record_id;
            }

            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CMGW].cmd_name->str_ptr, (rec_id + 1));
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        }
        //If not sucess,return the error.
        else
        {
            SCI_TRACE_LOW("ATC:WRITE error!");
            ATC_BuildSmsCodeRsp(atc_config_ptr, (int32)ATC_ConvMnSmsCause2AtcSmsCause(sig_ptr->cause));
        }

        s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
        SCI_SLEEP(30); //wait a moment for APP handling APP_MN_WRITE_SMS_CNF;
    }

    return S_ATC_DEFAULT_HANDLE;
}
/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_MEM_FULL_IND signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSmsFullInd(
    ATC_CONFIG_T                      *atc_config_ptr,
    APP_MN_SMS_MEM_FULL_IND_T            *sig_ptr
)
{
    SCI_TRACE_LOW("ATC:ATC_ProcessSmsFullInd, mem_status = %d", sig_ptr->mem_status);

    //If success,return the indication string
    if(MN_SMS_ALL_FULL == sig_ptr->mem_status)
    {
        /*
        sprintf((char*)g_rsp_str, "%s: SIM & ME are full", g_atc_info_table[AT_CMD_CMGW].cmd_name->str_ptr);
        */
    }
    else if((MN_SMS_SIM_FULL == sig_ptr->mem_status))
    {
        /*
        sprintf((char*)g_rsp_str, "%s: SIM is full", g_atc_info_table[AT_CMD_CMGW].cmd_name->str_ptr);
        */
    }
    else if((MN_SMS_ME_FULL == sig_ptr->mem_status))
    {
        sprintf((char *)g_rsp_str, "%s: ME is full", g_atc_info_table[AT_CMD_CMGW].cmd_name->str_ptr);
        ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
    }

    ATC_UpECINDInfo(sig_ptr->dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, 2, 0, 0, 0, 0);
    SCI_SLEEP(30); //avoid APP queue full during synchronization operation;


    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_MEM_FULL_IND signal
//                and send the result code to SIO
//  Global resource dependence :
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSmsReadyInd(
    ATC_CONFIG_T                     *atc_config_ptr,
    APP_MN_SMS_READY_IND_T            *sig_ptr
)
{
    MN_SMS_RECORD_ID_T    total_num;
    MN_SMS_RECORD_ID_T    used_num;
    MN_SMS_MEM_FULL_STATUS_E    mem_status = MN_SMS_MEM_AVAIL;
    MN_DUAL_SYS_E              dual_sys   = sig_ptr->dual_sys;

    /* When SMS is ready, send +SIND URC code to AP
        * +SIND: 12 */
    if(atc_config_ptr->urc_level == ATC_URC_ALL_STATUS_ON)
    {
        sprintf((char *)g_rsp_str, "%s: 12", g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
        ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
    }

    atc_config_ptr->sms_is_ready[sig_ptr->dual_sys] = TRUE;

    // Check if sms is full,up +ECIND: 2,1
    if(ERR_MNSMS_NONE == MNSMS_GetSmsNumEx(sig_ptr->dual_sys, s_sms_config[sig_ptr->dual_sys].mem1, &total_num, &used_num))
    {
        if(MN_SMS_STORAGE_SIM == s_sms_config[sig_ptr->dual_sys].mem1)
        {
            if(used_num >= total_num)
            {
                mem_status = MN_SMS_SIM_FULL;
                ((APP_MN_SMS_MEM_FULL_IND_T *)sig_ptr)->mem_status = mem_status;
            }
        }
        else if(MN_SMS_STORAGE_ALL == s_sms_config[sig_ptr->dual_sys].mem1)
        {
             if(used_num >= total_num)
            {
                mem_status = MN_SMS_ALL_FULL;
                ((APP_MN_SMS_MEM_FULL_IND_T *)sig_ptr)->mem_status = mem_status;
            }
        }

        if(MN_SMS_SIM_FULL ==mem_status||MN_SMS_ALL_FULL == mem_status)
        {
             ATC_UpECINDInfo(sig_ptr->dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, 2, 0, 0, 0, 0);
        }
    }
    else
    {
        SCI_TRACE_LOW("ATC: Get Sms Num fail");
    }

    //Read the SC from SIM
    if(ERR_MNSMS_NONE  != MNSMS_GetScAddrEx(dual_sys, &g_sc_addr[dual_sys]))
    {
        SCI_TRACE_LOW("ATC: Get sc failure");
    }

    if(1 == atc_config_ptr->spreadyflag)
    {
        sprintf((char *)g_rsp_str, "*SPREADY: 1");
        ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
    }

    return S_ATC_SUCCESS;
}
/*****************************************************************************/
//  Description : This function handles the APP_MN_SMS_STATUS_REPORT_IND signal.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSmsStatusReportInd(
    ATC_CONFIG_T                      *atc_config_ptr,
    APP_MN_SMS_STATUS_REPORT_IND_T    *sig_ptr
)
{
    MN_CALLED_NUMBER_T      mn_called_num;
    uint8                   first_octet;
    uint8                   called_num_arr[ATC_MAX_SMS_ADDRESS_LEN_FOR_TE] = {0};
    uint16                  called_num_len = 0;
    ATC_TELE_TYPE_E         tel_type;
    uint8                   *temp_ptr = PNULL;

    if(s_sms_config[sig_ptr->dual_sys].ds_ind == 1)
    {
        // handle the pdu mode status report
        if(!atc_config_ptr->atc_config_nv.cmgf_flag)
        {
            if(ERR_MNSMS_NONE == MNSMS_GetScAddrEx(sig_ptr->dual_sys, &mn_called_num))
            {
                MNSMS_AddScAddrEx(sig_ptr->dual_sys, &mn_called_num, &(sig_ptr->msg_data));
            }

            sprintf((char *)g_rsp_str, "+CDS: %d", sig_ptr->msg_data.length);
            ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
            ConvertBinToHex(sig_ptr->msg_data.l3_msg, (uint8)sig_ptr->msg_data.length, g_rsp_str);
            g_rsp_str[(uint8)sig_ptr->msg_data.length * 2] = '\0';

            ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
        }
        // handle the text mode status report
        else
        {
            /*+CDS: <fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>*/
            first_octet = sig_ptr->msg_data.l3_msg[0];
            ATC_EncodeCalledNumToTE(&(sig_ptr->status_report.dest_addr_t), atc_config_ptr->cscs[sig_ptr->dual_sys],
                                    &tel_type, called_num_arr, &called_num_len);

            sprintf((char *)g_rsp_str, "+CDS: %d,%d,\"%s\",%d,", first_octet, sig_ptr->status_report.tp_mr,
                    called_num_arr, tel_type);
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];
            FormatTimeStampStr(temp_ptr, &(sig_ptr->status_report.tp_scts));
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];
            sprintf((char *)temp_ptr, ",");
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];
            FormatTimeStampStr(temp_ptr, &(sig_ptr->status_report.tp_dt));
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];
            sprintf((char *)temp_ptr, ",%d", sig_ptr->status_report.tp_status);
            ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
        }
    }
    else if(s_sms_config[sig_ptr->dual_sys].ds_ind == 2)
    {
        /*CR Modify note:
        <use character "n" represents max num available of the current storage>
        the range of sms record id       is from 0 to (n-1)
        the range of sms storage index is from 1 to n       when in the unsolicited sms AT
        */
        if(MN_SMS_STORAGE_SIM == sig_ptr->storage)
        {
            sprintf((char *)g_rsp_str, "+CDSI: \"SM\",%d", (sig_ptr->record_id + 1));
        }
        else
        {
            sprintf((char *)g_rsp_str, "+CDSI: \"ME\",%d", (sig_ptr->record_id + 1));
        }

        ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
    }
    else
    {
        SCI_TRACE_LOW("ATC: disable the status report");
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the text mode SMS
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTextModeSms(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T   *atc_config_ptr,
    uint8         *pdu_ptr,       // point to the pdu buffer
    uint16         pdu_len
)
{
    /* init the struct by 0*/
    MN_SMS_MO_SMS_T    mo_sms = {0};
    MN_SMS_MT_SMS_T    mt_sms = {0};
    MN_SMS_USER_DATA_HEAD_T  user_data_head_t = {0}; // only for interface change
    MN_SMS_USER_VALID_DATA_T user_valid_data_t = {0};
    uint16             data_len = 0;
    BOOLEAN            temp_flag = TRUE;
    MN_SMS_STATUS_E    status;
    MN_SMS_PATH_E      sms_path = MN_SMS_GSM_PATH;

    if ((MN_SMS_PS_DOMAIN_SERVICE == g_sms_ser_type[dual_sys]) ||
            (MN_SMS_PS_DOMAIN_SERVICE_PREF == g_sms_ser_type[dual_sys]))
    {
        sms_path = MN_SMS_GPRS_PATH;
    }

    switch(s_sms_nv_config.dcs)
    {
            // 7 bit encoded
        case MN_SMS_DEFAULT_ALPHABET:

            if(ATC_SMS_7BIT_LEN < pdu_len)
            {
                return FALSE;
            }

            /*first, convert to IRA, then IRA string will be converted to GSM by
            calling MNSMS_EncodeUserData as below source code*/
            if(!ATC_ChsetTransformation((ATC_CHARACTER_SET_TYPE_E)atc_config_ptr->cscs[dual_sys],
                                        pdu_ptr, pdu_len,
                                        ATC_CHSET_IRA,
                                        MN_SMS_MAX_USER_VALID_DATA_LENGTH,
                                        user_valid_data_t.user_valid_data_arr,
                                        &data_len))
            {
                return FALSE;
            }

            user_valid_data_t.length = (uint8)data_len; //data_len must be less than MN_SMS_MAX_USER_VALID_DATA_LENGTH
            break;

            // unicode encoded
        case MN_SMS_UCS2_ALPHABET:
        case MN_SMS_8_BIT_ALPHBET:

            if(ATC_SMS_UCS2_LEN < pdu_len ||
                    0 != pdu_len % 2)
            {
                SCI_TRACE_LOW("ATC: invalid <text> length:%d.", pdu_len);
                return FALSE;
            }

            if(!ConvertHexToBin(pdu_ptr, pdu_len, user_valid_data_t.user_valid_data_arr))
            {
                return FALSE;
            }

            user_valid_data_t.length = pdu_len / 2;
            break;

        default:
            SCI_ASSERT(FALSE);/*assert verified: check valid value*/
            break;
    }

    SCI_TRACE_LOW("ATC: text sms length:%d,dcs:%d.",  user_valid_data_t.length, s_sms_nv_config.dcs);

    if(ATC_SMS_UNSENT == s_sms_record[dual_sys].state
            || ATC_SMS_SENT == s_sms_record[dual_sys].state)
    {
        mo_sms.reply_path_is_request     = s_sms_nv_config.fo & 0x80;
        mo_sms.status_report_is_request  = s_sms_nv_config.fo & 0x20; // get the bit 5
        mo_sms.time_format_e = MN_SMS_TP_VPF_RELATIVE_FORMAT; // get the bit 4

        if(s_sms_nv_config.fo & 0x10)
        {
            mo_sms.time_format_e = MN_SMS_TP_VPF_RELATIVE_FORMAT;
        }
        else
        {
            mo_sms.time_format_e = MN_SMS_TP_VPF_NOT_PRESENT;
        }

        mo_sms.time_valid_period.time_second = GetValidPeriodSeconds(s_sms_nv_config.vp);
        mo_sms.dest_addr_present         = TRUE;
        mo_sms.dest_addr_t               = s_sms_record[dual_sys].dest_num;
        mo_sms.sc_addr_present           = TRUE;
        SCI_MEMCPY(&mo_sms.sc_addr_t, &g_sc_addr[dual_sys], sizeof(MN_CALLED_NUMBER_T));
        mo_sms.pid_present               = TRUE;
        mo_sms.pid_e                     = (MN_SMS_PID_E)s_sms_nv_config.pid;
        mo_sms.dcs.alphabet_type         = (MN_SMS_ALPHABET_TYPE_E)s_sms_nv_config.dcs;
        mo_sms.dcs.class_is_present      = FALSE;
        mo_sms.dcs.msg_is_waiting        = FALSE;

        if(ERR_MNSMS_NONE != MNSMS_EncodeUserDataEx(dual_sys,
                                                    FALSE,
                                                    (MN_SMS_ALPHABET_TYPE_E)(s_sms_nv_config.dcs),
                                                    &user_data_head_t, // at should build this value when support long SMS
                                                    &user_valid_data_t,
                                                    &(mo_sms.user_data_t)
                                                   ))
        {
            SCI_TRACE_LOW("ATC: MNSMS_EncodeUserData failed.");
            return FALSE;
        }
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

        SCI_MEMCPY(&mt_sms.sc_addr_t, &g_sc_addr[dual_sys], sizeof(MN_CALLED_NUMBER_T));

        if(ERR_MNSMS_NONE != MNSMS_EncodeUserDataEx(dual_sys,
                                                    FALSE, // AT do not support long SMS now
                                                    (MN_SMS_ALPHABET_TYPE_E)(s_sms_nv_config.dcs),
                                                    &user_data_head_t, // at should build this value when support long SMS
                                                    &user_valid_data_t,
                                                    &(mt_sms.user_data_t)
                                                   ))
        {
            SCI_TRACE_LOW("ATC: MNSMS_EncodeUserData failed.");
            return FALSE;
        }
    }

    switch(s_sms_record[dual_sys].cmd_id)
    {
        case AT_CMD_CMGS:
            SCI_TRACE_LOW("ATC:status_report_is_request=%d", mo_sms.status_report_is_request);

            if(ERR_MNSMS_NONE != MNSMS_AppSendSmsEx(dual_sys, &mo_sms, MN_SMS_NO_STORED,
                                                    sms_path, s_sms_record[dual_sys].more_sms_flag))
            {
                temp_flag = FALSE;
            }
            else
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_SEND_SMS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            }

            break;
        case AT_CMD_CMGW:

            switch(s_sms_record[dual_sys].state)
            {
                    // the MT short message
                case ATC_SMS_UNREAD:
                    status = MN_SMS_MT_TO_BE_READ;
                    break;

                case ATC_SMS_READ:
                    status = MN_SMS_MT_READED;
                    break;

                    // the MO short message
                case ATC_SMS_UNSENT:
                    status = MN_SMS_MO_TO_BE_SEND;
                    break;

                case ATC_SMS_SENT:
                    status = MN_SMS_MO_SR_NOT_REQUEST;
                    break;

                default:
                    status = MN_SMS_MT_TO_BE_READ;
                    break;
            }

            if(MN_SMS_MT_TO_BE_READ == status || MN_SMS_MT_READED == status)
            {
                SCI_TRACE_LOW("ATC: call MNSMS_WriteMtSms");

                //Write the mt sms
                if(ERR_MNSMS_NONE != MNSMS_WriteMtSmsEx(dual_sys, s_sms_config[dual_sys].mem2, status, &mt_sms))
                {
                    SCI_TRACE_LOW("ATC: failure in MNSMS_WriteMtSms");
                    temp_flag = FALSE;
                }

#ifdef _MUX_ENABLE_
                else
                {
                    ATC_Add_Expected_Event(atc_config_ptr, APP_MN_WRITE_SMS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                }
#endif
            }
            else
            {
                //Wrtie the mo sms
                if(ERR_MNSMS_NONE != MNSMS_WriteMoSmsEx(dual_sys, s_sms_config[dual_sys].mem2, status, &mo_sms))
                {
                    SCI_TRACE_LOW("ATC: failure in MNSMS_WriteMoSms");
                    temp_flag = FALSE;
                }

#ifdef _MUX_ENABLE_
                else
                {
                    ATC_Add_Expected_Event(atc_config_ptr, APP_MN_WRITE_SMS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                }
#endif
            }

            break;
        default:
            break;
    }

    return temp_flag;
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

    if(vp <= ATC_SMS_MINUTES_VAL)
    {
        // up to 12 hours
        seconds_val = (vp + 1) * 5 * 60;
    }
    else if(vp <= ATC_SMS_HOURS_VAL)
    {
        // 12hours + (vp-143)*30minutes
        seconds_val = 12 * 60 * 60 + (vp - ATC_SMS_MINUTES_VAL) * 30 * 60;
    }
    else if(vp <= ATC_SMS_DAYS_VAL)
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
    ATC_TELE_TYPE_E             toda = UNKNOWN_PLAN_UNKNOWN_TYPE;       // the type of the address
    ATC_TELE_TYPE_E             tosca = UNKNOWN_PLAN_UNKNOWN_TYPE;
    uint8                           pid = 0;
    uint8                           fo = 0;
    uint8                       vp = 0;
    MN_SMS_ALPHABET_TYPE_E      dcs = MN_SMS_RESERVED_ALPHABET;
    uint8                     *temp_str = PNULL;
    uint8                     data_len = 0;
    uint8                     *data_str = PNULL;
    BOOLEAN                   mo_flag = TRUE;
    MN_SMS_TIME_STAMP_T       *time_stamp_ptr = PNULL;
    MN_SMS_USER_DATA_HEAD_T   sms_user_data_head;
    MN_SMS_USER_VALID_DATA_T  sms_user_valid_data;

    uint8                     da_to_TE[ATC_MAX_SMS_ADDRESS_LEN_FOR_TE] = {0};
    uint16                    da_to_TE_len = 0;
    uint8                     sca_to_TE[ATC_MAX_SMS_ADDRESS_LEN_FOR_TE] = {0};
    uint16                    sca_to_TE_len = 0;
    uint8                     data_to_TE[ATC_MAX_SMS_DATA_LEN_FOR_TE] = {0};
    uint16                    data_to_TE_len = 0;

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != sms_ptr);/*assert verified: check null pointer*/

    temp_str = g_rsp_str;
    SCI_TRACE_LOW("ATC: HandleTextModeSmsReadCnf, sms_status:%d,record_id:%d", sms_status, record_id);

    /*1. decode sms parameters*/
    SCI_MEMSET(da_to_TE, 0, ATC_MAX_SMS_ADDRESS_LEN_FOR_TE);
    SCI_MEMSET(sca_to_TE, 0, ATC_MAX_SMS_ADDRESS_LEN_FOR_TE);

    switch(sms_status)
    {
        case MN_SMS_FREE_SPACE:
            SCI_TRACE_LOW("ATC: SMS FREE SPACE!");
            break;
            // " REC READ"
        case MN_SMS_MT_READED:
            sms_state = ATC_SMS_READ;
            ATC_EncodeCalledNumToTE(&sms_ptr->mt_sms_t.origin_addr_t, atc_config_ptr->cscs[dual_sys], &toda, da_to_TE, &da_to_TE_len);
            ATC_EncodeCalledNumToTE(&sms_ptr->mt_sms_t.sc_addr_t, atc_config_ptr->cscs[dual_sys], &tosca, sca_to_TE, &sca_to_TE_len);

            MNSMS_DecodeUserDataEx(dual_sys,
                                   sms_ptr->mt_sms_t.user_head_is_exist,
                                   &(sms_ptr->mt_sms_t.dcs),
                                   &(sms_ptr->mt_sms_t.user_data_t),
                                   &sms_user_data_head,
                                   &sms_user_valid_data
                                  );

            data_len = sms_user_valid_data.length; //sms_ptr->mt_sms_t.user_data_t.length;
            data_str = sms_user_valid_data.user_valid_data_arr; //sms_ptr->mt_sms_t.user_data_t.user_data_arr;
            data_str[data_len] = '\0';
            mo_flag  = FALSE;
            time_stamp_ptr = &(sms_ptr->mt_sms_t.time_stamp_t);
            pid = (uint8)sms_ptr->mt_sms_t.pid_e;
            fo = (uint8)sms_ptr->mt_sms_t.user_data_t.user_data_arr[0];
            dcs = sms_ptr->mt_sms_t.dcs.alphabet_type;
            break;

            // "REC UNREAD"
        case MN_SMS_MT_TO_BE_READ:
            sms_state = ATC_SMS_UNREAD;
            ATC_EncodeCalledNumToTE(&sms_ptr->mt_sms_t.origin_addr_t, atc_config_ptr->cscs[dual_sys], &toda, da_to_TE, &da_to_TE_len);
            ATC_EncodeCalledNumToTE(&sms_ptr->mt_sms_t.sc_addr_t, atc_config_ptr->cscs[dual_sys], &tosca, sca_to_TE, &sca_to_TE_len);

            MNSMS_DecodeUserDataEx(dual_sys,
                                   sms_ptr->mt_sms_t.user_head_is_exist,
                                   &(sms_ptr->mt_sms_t.dcs),
                                   &(sms_ptr->mt_sms_t.user_data_t),
                                   &sms_user_data_head,
                                   &sms_user_valid_data
                                  );
            data_len = sms_user_valid_data.length; //sms_ptr->mt_sms_t.user_data_t.length;
            data_str = sms_user_valid_data.user_valid_data_arr; //sms_ptr->mt_sms_t.user_data_t.user_data_arr;
            data_str[data_len] = '\0';
            mo_flag  = FALSE;
            time_stamp_ptr = &(sms_ptr->mt_sms_t.time_stamp_t);

            pid = (uint8)sms_ptr->mt_sms_t.pid_e;
            fo = (uint8)sms_ptr->mt_sms_t.user_data_t.user_data_arr[0];
            dcs = sms_ptr->mt_sms_t.dcs.alphabet_type;
            break;

            // "STO UNSENT"
        case MN_SMS_MO_TO_BE_SEND:
            sms_state = ATC_SMS_UNSENT;

            if(sms_ptr->mo_sms_t.sc_addr_present)
            {
                ATC_EncodeCalledNumToTE(&sms_ptr->mo_sms_t.sc_addr_t, atc_config_ptr->cscs[dual_sys], &tosca, sca_to_TE, &sca_to_TE_len);
            }

            if(sms_ptr->mo_sms_t.dest_addr_present)
            {
                ATC_EncodeCalledNumToTE(&sms_ptr->mo_sms_t.dest_addr_t, atc_config_ptr->cscs[dual_sys], &toda, da_to_TE, &da_to_TE_len);
            }

            MNSMS_DecodeUserDataEx(dual_sys,
                                   sms_ptr->mo_sms_t.user_head_is_exist,
                                   &(sms_ptr->mo_sms_t.dcs),
                                   &(sms_ptr->mo_sms_t.user_data_t),
                                   &sms_user_data_head,
                                   &sms_user_valid_data
                                  );
            data_len     = sms_user_valid_data.length;
            data_str     = sms_user_valid_data.user_valid_data_arr;
            data_str[data_len] = '\0';

            pid = (uint8)sms_ptr->mo_sms_t.pid_e;
            fo = (uint8)sms_ptr->mo_sms_t.user_data_t.user_data_arr[0];

            if(MN_SMS_TP_VPF_RELATIVE_FORMAT == sms_ptr->mo_sms_t.time_format_e)
            {
                vp = GetValidityPeriod(sms_ptr->mo_sms_t.time_valid_period.time_second);
            }

            dcs = sms_ptr->mo_sms_t.dcs.alphabet_type;

            break;
            // "STO SENT"
        case MN_SMS_MO_SR_NOT_REQUEST:
        case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
        case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
        case MN_SMS_MO_SR_RECEIVED_AND_STORED:
            sms_state = ATC_SMS_SENT;

            if(sms_ptr->mo_sms_t.sc_addr_present)
            {
                ATC_EncodeCalledNumToTE(&sms_ptr->mo_sms_t.sc_addr_t, atc_config_ptr->cscs[dual_sys], &tosca, sca_to_TE, &sca_to_TE_len);
            }

            if(sms_ptr->mo_sms_t.dest_addr_present)
            {
                ATC_EncodeCalledNumToTE(&sms_ptr->mo_sms_t.dest_addr_t, atc_config_ptr->cscs[dual_sys], &toda, da_to_TE, &da_to_TE_len);
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
            data_str[data_len] = '\0';

            pid = (uint8)sms_ptr->mo_sms_t.pid_e;
            fo = (uint8)sms_ptr->mo_sms_t.user_data_t.user_data_arr[0];

            if(MN_SMS_TP_VPF_RELATIVE_FORMAT == sms_ptr->mo_sms_t.time_format_e)
            {
                vp = GetValidityPeriod(sms_ptr->mo_sms_t.time_valid_period.time_second);
            }

            dcs = sms_ptr->mo_sms_t.dcs.alphabet_type;
            break;
            //status report
        case MN_SMS_MT_SR_READED:
            sms_state = ATC_SMS_READ;
            ATC_EncodeCalledNumToTE(&sms_ptr->report_sms_t.dest_addr_t, atc_config_ptr->cscs[dual_sys], &toda, da_to_TE, &da_to_TE_len);
            break;
        case MN_SMS_MT_SR_TO_BE_READ:
            sms_state = ATC_SMS_UNREAD;
            ATC_EncodeCalledNumToTE(&sms_ptr->report_sms_t.dest_addr_t, atc_config_ptr->cscs[dual_sys], &toda, da_to_TE, &da_to_TE_len);
            break;

        default:
            SCI_ASSERT(FALSE);/*assert verified: check valid value*/
            break;
    }

    /*2. transform some parameters to TE character set */
    // convert <data>
    ATC_EncodeSMSDataToTE(data_str, (uint16)data_len, dcs, atc_config_ptr->cscs[dual_sys], data_to_TE, &data_to_TE_len);

    /*3. report AT info to TE*/
    SCI_MEMSET(g_rsp_str, 0, MAX_ATC_RSP_LEN);

    switch(s_sms_record[dual_sys].cmd_id)
    {
        case AT_CMD_CMGL:

            //Handle the status report
            if(MN_SMS_MT_SR_READED == sms_status || MN_SMS_MT_SR_TO_BE_READ == sms_status)
            {
                /*SMS-STATUS-REPORT:
                  +CMGR: <index>,<stat>,<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>*/

                //add  +CMGR: <index>,<stat>,<fo>,<mr>,[<ra>],[<tora>],
                sprintf((char *)temp_str, "%s: %d,\"%s\",%d,%d,\"%s\",%d,",
                        g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                        s_sms_record[dual_sys].cmgl_idex,
                        s_sms_state_str[sms_state], s_sms_nv_config.fo,
                        sms_ptr->report_sms_t.tp_mr, da_to_TE, toda);

                //add <scts>
                temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                FormatTimeStampStr(temp_str, &(sms_ptr->report_sms_t.tp_scts));

                //add <dt>
                temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                sprintf((char *)temp_str, ",");
                temp_str++;
                FormatTimeStampStr(temp_str, &(sms_ptr->report_sms_t.tp_dt));

                //add <st>
                temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                sprintf((char *)temp_str, ",%d", sms_ptr->report_sms_t.tp_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else if(MN_SMS_FREE_SPACE == sms_status)
            {
                sprintf((char *)temp_str, "%s: \"%s\"",
                        g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr, s_sms_state_str[sms_state]);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
            }
            else
            {
                /*SMS-SUBMITs and/or SMS-DELIVERs:
                +CMGL: <index>,<stat>,<oa/da>,[<alpha>],[<scts>][,<tooa/toda>,
                  <length>]<CR><LF><data>[<CR><LF>*/

                //add +CMGL: <index>,<stat>,<oa/da>,[<alpha>],
                sprintf((char *)temp_str, "%s: %d,\"%s\",\"%s\", ,",
                        g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                        s_sms_record[dual_sys].cmgl_idex, s_sms_state_str[sms_state], da_to_TE);

                //add [<scts>]
                temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];

                if(mo_flag)
                {
                    sprintf((char *)temp_str, " ");
                }
                else
                {
                    FormatTimeStampStr(temp_str, time_stamp_ptr);
                }

                //add ,<tooa/toda>,<length>
                if(s_sms_config[dual_sys].csdh_flag)
                {
                    temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                    sprintf((char *)temp_str, ",%d,%d", toda, data_to_TE_len);
                }

                //add  <CR><LF>
                temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                sprintf((char *)temp_str, "%c%c", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);

                //add <data>
                temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                sprintf((char *)temp_str, "%s", data_to_TE);
                //temp_str[data_to_TE_len] = '\0';
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                if (PNULL != data_str)
                {
                    if( 0 == strcmp((char *)atc_config_ptr->cscs, "GSM") )
                    {
                        SCI_MEMCPY(temp_str, data_str, data_len);
                        
                        temp_str[data_len] = '\0'; 
                    }
                    else
                    {
                        ConvertBinToHex(data_str, data_len, temp_str);
    
                        temp_str[data_len * 2] = '\0'; 
                    }
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }

            if(FALSE == UpdateSMSStatus(dual_sys, atc_config_ptr, sms_status, record_id))
            {
                //AT+CMGL has finished reading one sms and starts to read the next one
                ReadNextSMS(dual_sys, atc_config_ptr);
            }

            break;

        case AT_CMD_CMGR:

            //Handle the status report
            if(MN_SMS_MT_SR_READED == sms_status || MN_SMS_MT_SR_TO_BE_READ == sms_status)
            {
                /*SMS-STATUS-REPORT:
                    +CMGR: <stat>,<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>*/

                //add  +CMGR: <stat>,<fo>,<mr>,[<ra>],[<tora>],
                sprintf((char *)temp_str, "%s: \"%s\",%d,%d,\"%s\",%d,",
                        g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr,
                        s_sms_state_str[sms_state], s_sms_nv_config.fo,
                        sms_ptr->report_sms_t.tp_mr, da_to_TE, toda);

                //add <scts>
                temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                FormatTimeStampStr(temp_str, &(sms_ptr->report_sms_t.tp_scts));

                //add <dt>
                temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                sprintf((char *)temp_str, ",");
                temp_str++;
                FormatTimeStampStr(temp_str, &(sms_ptr->report_sms_t.tp_dt));

                //add <st>
                temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                sprintf((char *)temp_str, ",%d", sms_ptr->report_sms_t.tp_status);

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else if(MN_SMS_FREE_SPACE == sms_status)
            {
                sprintf((char *)temp_str, "%s: \"%s\"",
                        g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr, s_sms_state_str[sms_state]);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
            }
            else
            {

                if(mo_flag)
                {
                    /*SMS-SUBMIT:
                    +CMGR: <stat>,<da>,[<alpha>][,<toda>,<fo>,<pid>,<dcs>,[<vp>],
                        <sca>,<tosca>,<length>]<CR><LF><data>*/

                    //add +CMGR: <stat>,<da>,[<alpha>]
                    sprintf((char *)temp_str, "%s: \"%s\",\"%s\", ",
                            g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr,
                            s_sms_state_str[sms_state], da_to_TE);

                    if(s_sms_config[dual_sys].csdh_flag)
                    {
                        //add  ,<toda>,<fo>,<pid>,<dcs>,[<vp>],<sca>,<tosca>,<length>
                        temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                        sprintf((char *)temp_str, ",%d,%d,%d,%d,%d,\"%s\",%d,%d",
                                toda, fo, pid, dcs, vp, sca_to_TE, tosca, data_to_TE_len);
                    }
                }
                else
                {
                    /*SMS-DELIVER:
                    +CMGR: <stat>,<oa>,[<alpha>],<scts>[,<tooa>,<fo>,<pid>,<dcs>,
                        <sca>,<tosca>,<length>]<CR><LF><data>*/

                    //add +CMGR: <stat>,<oa>,[<alpha>]
                    sprintf((char *)temp_str, "%s: \"%s\",\"%s\", ,",
                            g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr,
                            s_sms_state_str[sms_state], da_to_TE);

                    //add <scts>
                    temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                    FormatTimeStampStr(temp_str, time_stamp_ptr);

                    if(s_sms_config[dual_sys].csdh_flag)
                    {
                        //add  ,<tooa>,<fo>,<pid>,<dcs>,<sca>,<tosca>,<length>
                        temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                        sprintf((char *)temp_str, ",%d,%d,%d,%d,\"%s\",%d,%d",
                                toda, fo, pid, dcs, sca_to_TE, tosca, data_to_TE_len);
                    }
                }

                // add <CR><LF>
                temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                sprintf((char *)temp_str, "%c%c", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);

                //add <data>
                temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
                sprintf((char *)temp_str, "%s", data_to_TE);
                //temp_str[data_to_TE_len] = '\0';
                //ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                if (PNULL != data_str)
                {
                    if( 0 == strcmp((char *)atc_config_ptr->cscs, "GSM") )
                    {
                        SCI_MEMCPY(temp_str, data_str, data_len);
                        
                        temp_str[data_len] = '\0'; 
                    }
                    else
                    {
                        ConvertBinToHex(data_str, data_len, temp_str);
    
                        temp_str[data_len * 2] = '\0'; 
                    }
    
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }

            if(FALSE == UpdateSMSStatus(dual_sys, atc_config_ptr, sms_status, record_id))
            {
                //AT+CMGR is finished, return OK
                s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
            }

            break;

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
    uint8                   temp_str[20+4] = {0};//add 4 octets for pre and post fixes of RSP
#ifndef _ATC_ONLY
    uint8                   tp_status = 0;
#endif

    SCI_TRACE_LOW("ATC: HandlePduModeSmsReadCnf, sms_status:%d,record_id:%d", sms_status, record_id);

    switch(sms_status)
    {
        case MN_SMS_FREE_SPACE:
            SCI_TRACE_LOW("ATC: Read a free SMS space.");
            break;

        case MN_SMS_MT_READED:
            sms_state = ATC_SMS_READ;
            MNSMS_GetHexSmsMtEx(dual_sys, &(sms_ptr->mt_sms_t), &tpdu_len, g_rsp_str, MAX_ATC_RSP_LEN);
            break;

        case MN_SMS_MT_TO_BE_READ:
            sms_state = ATC_SMS_UNREAD;
            MNSMS_GetHexSmsMtEx(dual_sys, &(sms_ptr->mt_sms_t), &tpdu_len, g_rsp_str, MAX_ATC_RSP_LEN);
            break;

        case MN_SMS_MO_TO_BE_SEND:
            sms_state = ATC_SMS_UNSENT;
            MNSMS_GetHexSmsMoEx(dual_sys, &(sms_ptr->mo_sms_t), &tpdu_len, g_rsp_str, MAX_ATC_RSP_LEN);
            break;
        case MN_SMS_MO_SR_NOT_REQUEST:
        case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
        case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
        case MN_SMS_MO_SR_RECEIVED_AND_STORED:
            sms_state = ATC_SMS_SENT;
            MNSMS_GetHexSmsMoEx(dual_sys, &(sms_ptr->mo_sms_t), &tpdu_len, g_rsp_str, MAX_ATC_RSP_LEN);
            break;
            //status report
        case MN_SMS_MT_SR_READED:
        case MN_SMS_MT_SR_TO_BE_READ:

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

        default:
            SCI_ASSERT(FALSE);/*assert verified: check valid value*/
            break;
    }

    switch(s_sms_record[dual_sys].cmd_id)
    {
        case AT_CMD_CMGL:

            //Handle the status report
            if(MN_SMS_MT_SR_READED == sms_status || MN_SMS_MT_SR_TO_BE_READ == sms_status)
            {
                sprintf((char *)temp_str, "%s: %d,%d, ,%d", g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                        s_sms_record[dual_sys].cmgl_idex, sms_state, tpdu_len);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else if(MN_SMS_FREE_SPACE == sms_status)
            {
                sprintf((char *)temp_str, "%s: %d,%d, ,%d", g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                        s_sms_record[dual_sys].cmgl_idex, 0/*sms_state*/, 0/*tpdu_len*/);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
            }
            else
            {
                sprintf((char *)temp_str, "%s: %d,%d, ,%d", g_atc_info_table[AT_CMD_CMGL].cmd_name->str_ptr,
                        s_sms_record[dual_sys].cmgl_idex, sms_state, tpdu_len);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }

            if(FALSE == UpdateSMSStatus(dual_sys, atc_config_ptr, sms_status, record_id))
            {
                //AT+CMGL has finished reading one sms and starts to read the next one
                ReadNextSMS(dual_sys, atc_config_ptr);
            }

            break;

        case AT_CMD_CMGR:

            //Handle the status report
            if(MN_SMS_MT_SR_READED == sms_status || MN_SMS_MT_SR_TO_BE_READ == sms_status)
            {
                sprintf((char *)temp_str, "%s: %d, ,%d", g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr,
                        sms_state, tpdu_len);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else if(MN_SMS_FREE_SPACE == sms_status)
            {
                sprintf((char *)temp_str, "%s: %d, ,%d", g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr,
                        0/*sms_state*/, 0/*tpdu_len*/);

                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
            }
            else
            {
                sprintf((char *)temp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CMGR].cmd_name->str_ptr, sms_state, tpdu_len);
                ATC_BuildInfoRsp(atc_config_ptr, temp_str);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }


            if(FALSE == UpdateSMSStatus(dual_sys, atc_config_ptr, sms_status, record_id))
            {
                //AT+CMGR is finished, return OK
                s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
            }

            break;

        default:
            break;
    }
}

/*****************************************************************************/
//  Description : This function handles the signal of APP_MN_SMS_IND in the
//                PDU mode
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandlePduModeSmsInd(
    MN_DUAL_SYS_E       dual_sys,
    ATC_CONFIG_T          *atc_config_ptr,
    MN_SMS_MT_SMS_T       *mt_sms_ptr,
    MN_SMS_RECORD_ID_T    record_id
)
{
    uint8                 pdu_len;
    uint8                 *temp_str;
    uint8                 temp_str1[MAX_SMS_PDU_LEN] = {0};     /*CR166630*/

    SCI_TRACE_LOW("ATC: enter function: HandlePduModeSmsInd()");
    //wh modify begin

    MNSMS_GetHexSmsMtEx(dual_sys, mt_sms_ptr, &pdu_len, temp_str1, MAX_ATC_RSP_LEN);

    temp_str = g_rsp_str;
    /*+CMT: [alpha],<length><CR><LF><data>*/
    sprintf((char *)temp_str, "+CMT: ,%d%c%c%s", pdu_len,
            atc_config_ptr->s3_reg, atc_config_ptr->s4_reg, temp_str1);

    ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);

    AtcReStartCNMATimer(dual_sys,ATC_CNMA_TIMER_VALUE);
    s_sms_cnma[dual_sys].is_waiting_for_cnma = TRUE;
}

/*****************************************************************************/
//  Description : This function handles the signal of APP_MN_SMS_IND in the
//                Text mode
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleTextModeSmsInd(
    MN_DUAL_SYS_E       dual_sys,
    ATC_CONFIG_T          *atc_config_ptr,
    MN_SMS_MT_SMS_T       *mt_sms_ptr,
    MN_SMS_RECORD_ID_T    record_id
)
{
    uint8                     dest_addr_arr[ATC_MAX_SMS_ADDRESS_LEN_FOR_TE] = {0};
    uint8                     sca[ATC_MAX_SMS_ADDRESS_LEN_FOR_TE] = {0};
    uint8                     *temp_str = PNULL;
    uint8                     data_len;
    uint8                     *data_str = PNULL;
    MN_SMS_USER_DATA_HEAD_T   user_data_head;
    MN_SMS_USER_VALID_DATA_T  user_valid_data;
    ATC_TELE_TYPE_E     tooa = UNKNOWN_PLAN_UNKNOWN_TYPE;
    ATC_TELE_TYPE_E     tosca = UNKNOWN_PLAN_UNKNOWN_TYPE;
    uint8                     data_to_TE[ATC_MAX_SMS_DATA_LEN_FOR_TE] = {0};
    uint16                    data_to_TE_len = 0;

    temp_str = g_rsp_str;
    SCI_MEMSET(g_rsp_str, 0, MAX_ATC_RSP_LEN);

    ATC_EncodeCalledNumToTE(&(mt_sms_ptr->origin_addr_t), atc_config_ptr->cscs[dual_sys], &tooa, dest_addr_arr, NULL);
    ATC_EncodeCalledNumToTE(&(mt_sms_ptr->sc_addr_t), atc_config_ptr->cscs[dual_sys], &tosca, sca, NULL);

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

    ATC_EncodeSMSDataToTE(data_str, (uint16)data_len, mt_sms_ptr->dcs.alphabet_type,
                          atc_config_ptr->cscs[dual_sys], data_to_TE, &data_to_TE_len);

    /*
    +CMT: <oa>, [<alpha>],<scts>
    [,<tooa>,<fo>,<pid>,<dcs>,<sca>,<tosca>, <length>]<CR><LF>
    <data>
      */
    /* <oa> */
    sprintf((char *)temp_str, "+CMT: \"%s\"", dest_addr_arr);

    /* <alpha> */
    temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
    sprintf((char *)temp_str, ", ,");

    /* <scts> */
    temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
    FormatTimeStampStr(temp_str, &(mt_sms_ptr->time_stamp_t));

    if(s_sms_config[dual_sys].csdh_flag)
    {
        //add  ,<tooa>,<fo>,<pid>,<dcs>,<sca>,<tosca>,<length>
        temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
        sprintf((char *)temp_str, ",%d,%d,%d,%d,\"%s\",%d,%d",
                tooa, mt_sms_ptr->user_data_t.user_data_arr[0], mt_sms_ptr->pid_e,
                mt_sms_ptr->dcs.alphabet_type, sca, tosca, data_to_TE_len);
    }

    /*<CR><LF>*/
    temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
    sprintf((char *)temp_str, "%c%c", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);

    /*<data> */
    temp_str = &g_rsp_str[strlen((char *)g_rsp_str)];
    sprintf((char *)temp_str, "%s", data_to_TE);
    // temp_str[data_to_TE_len] = '\0';
    ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);

    AtcReStartCNMATimer(dual_sys,ATC_CNMA_TIMER_VALUE);
    s_sms_cnma[dual_sys].is_waiting_for_cnma = TRUE;
}

/*****************************************************************************/
//  Description : This function is used to read the next sms by AT+CMGL
//  Global resource dependence :
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
LOCAL void ReadNextSMS(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T          *atc_config_ptr
)
{
    MN_SMS_RECORD_ID_T      start_index = 0;

    //Read the next item
    if(ReadSpecSms(dual_sys, s_sms_config[dual_sys].mem1, &start_index, s_sms_record[dual_sys].state))
    {
        SCI_TRACE_LOW("ATC: ReadNextSMS, sleep 30ms");
        SCI_SLEEP(30);//wait a moment for APP handling APP_MN_READ_SMS_CNF
        MNSMS_ReadSmsEx(dual_sys, s_sms_config[dual_sys].mem1, start_index);
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                          APP_MN_READ_SMS_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
    }
    else
    {
        SCI_MEMSET(&s_sms_record[dual_sys], 0, sizeof(ATC_SMS_RECORD_T));
        s_sms_record[dual_sys].cmd_id = AT_INVALID_CMD_ID;
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK , 0);
    }

}

/*****************************************************************************/
//  Description : This function is used to update the sms status by AT+CMGL, AT+CMGR
//  Global resource dependence :
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateSMSStatus(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T          *atc_config_ptr,
    MN_SMS_STATUS_E     sms_status,
    MN_SMS_RECORD_ID_T  record_id
)
{
    if(MN_SMS_MT_TO_BE_READ == sms_status && s_sms_config[dual_sys].suss_flag)
    {
        //update unread MT sms status to readed
        MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem1, record_id, MN_SMS_MT_READED);
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                          APP_MN_UPDATE_SMS_STATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
    }
    else if(MN_SMS_MT_SR_TO_BE_READ == sms_status && s_sms_config[dual_sys].suss_flag)
    {
        //update unread status-report sms status to readed
        MNSMS_UpdateSmsStatusEx(dual_sys, s_sms_config[dual_sys].mem3, record_id, MN_SMS_MT_SR_READED);
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                          APP_MN_UPDATE_SMS_STATE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

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

    SCI_ASSERT(PNULL != state_str);/*assert verified: check null pointer*/

    // search the SMS state string
    for(i = ATC_SMS_UNREAD; i < ATC_SMS_MAX_STATUS; i++)
    {
        //check if it is one SMS state
        if(0 == strcmp((char *)state_str, s_sms_state_str[i]))
        {
            *sms_state_ptr  = i;
            result          = TRUE;
            break;
        }
    }

    return (result);
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
    SCI_ASSERT(PNULL != ts_str);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != time_stamp_ptr);/*assert verified: check null pointer*/

    sprintf((char *)ts_str, "\"%02d/%02d/%02d,%02d:%02d:%02d+%02d\"", time_stamp_ptr->year,
            time_stamp_ptr->month, time_stamp_ptr->day, time_stamp_ptr->hour, time_stamp_ptr->minute,
            time_stamp_ptr->second, time_stamp_ptr->timezone);
}

/*****************************************************************************/
//  Description : This function is used to delete the specific SMS according to
//                the SMS state.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteSpecSms(
    MN_DUAL_SYS_E   dual_sys,
    MN_SMS_STORAGE_E        storage,
    MN_SMS_RECORD_ID_T      *index_ptr,
    ATC_SMS_DEL_STATE_E     del_state
)
{
    int32                 i;
    MN_SMS_RECORD_ID_T    total_num;
    MN_SMS_RECORD_ID_T    mn_sms_num;
    MN_SMS_STATUS_E       *status_ptr;


    if(ERR_MNSMS_NONE != MNSMS_GetSmsNumEx(dual_sys, storage, &total_num, &mn_sms_num))
    {
        return FALSE;
    }

    status_ptr = SCI_ALLOC_BASE(total_num * sizeof(MN_SMS_STATUS_E));

    if(NULL == status_ptr)
    {
        SCI_TRACE_LOW("ATC: NO MEM!!!in DeleteSpecSms ");
        return FALSE;
    }

    if(ERR_MNSMS_NONE != MNSMS_GetSmsStatusEx(dual_sys, storage, &total_num, status_ptr))
    {
        SCI_FREE(status_ptr);
        return FALSE;
    }


    for(i = s_sms_record[dual_sys].cmgl_idex; i < total_num; i++)
    {
        switch(del_state)
        {
            case ATC_DEL_INDEX:
                SCI_FREE(status_ptr);
                return FALSE;

            case ATC_DEL_ALL_READ:

                if(MN_SMS_MT_READED == status_ptr[i] || MN_SMS_MT_SR_READED == status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }

                break;

            case ATC_DEL_ALL_READ_SENT:

                if((MN_SMS_MT_READED                    == status_ptr[i]) ||
                        (MN_SMS_MT_SR_READED                  == status_ptr[i]) ||
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

            case ATC_DEL_ALL_READ_SENT_UNSENT:

                if((!((MN_SMS_MT_TO_BE_READ == status_ptr[i]) || (MN_SMS_MT_SR_TO_BE_READ == status_ptr[i]))) &&
                        MN_SMS_FREE_SPACE     != status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }

                break;


            case ATC_DEL_ALL:

                if(MN_SMS_FREE_SPACE != status_ptr[i])
                {
                    *index_ptr = (MN_SMS_RECORD_ID_T)i;
                    s_sms_record[dual_sys].cmgl_idex = i + 1;
                    SCI_FREE(status_ptr);
                    return TRUE;
                }

                break;
            default:
                SCI_ASSERT(FALSE);/*assert verified: check valid value*/
                break;

        }
    }

    SCI_FREE(status_ptr);

    s_sms_record[dual_sys].cmgl_idex = i + 1;
    SCI_TRACE_LOW("ATC:the current cmgl index: %d", s_sms_record[dual_sys].cmgl_idex);
    return FALSE;
}

/*****************************************************************************/
//  Description : This function is used to handle the response of the at+cmss
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleCmssCmd(
    APP_MN_READ_SMS_CNF_T    *sig_ptr,
    ATC_CONFIG_T             *atc_config_ptr
)
{
    MN_SMS_PATH_E      sms_path = MN_SMS_GSM_PATH;

    if ((MN_SMS_PS_DOMAIN_SERVICE == g_sms_ser_type[sig_ptr->dual_sys]) || 
        (MN_SMS_PS_DOMAIN_SERVICE_PREF == g_sms_ser_type[sig_ptr->dual_sys]))
    {
        sms_path = MN_SMS_GPRS_PATH;
    }

    // the state of the specfic sms is right
    if(s_sms_record[sig_ptr->dual_sys].dest_num_flag)
    {
        sig_ptr->sms_t_u.mo_sms_t.dest_addr_present = TRUE;
        sig_ptr->sms_t_u.mo_sms_t.dest_addr_t = s_sms_record[sig_ptr->dual_sys].dest_num;
    }
    else if(!sig_ptr->sms_t_u.mo_sms_t.dest_addr_present)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_321);
        s_sms_record[sig_ptr->dual_sys].cmd_id = AT_INVALID_CMD_ID;
    }

    MNSMS_AppSendSmsEx(sig_ptr->dual_sys, &sig_ptr->sms_t_u.mo_sms_t, MN_SMS_NO_STORED,
                       sms_path, FALSE);

    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                      APP_MN_SEND_SMS_CNF, 0xff, ATC_NO_DOMAIN, sig_ptr->dual_sys);


}

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

    switch(sms_status)
    {
        case MN_SMS_FREE_SPACE:
            return_flag = FALSE;
            break;

        case MN_SMS_MT_READED:

            if(at_sms_status != ATC_SMS_UNREAD)
            {
                *status_ptr = FALSE;
            }
            else
            {
                *changed_status_ptr = MN_SMS_MT_TO_BE_READ;
            }

            break;

        case MN_SMS_MT_TO_BE_READ:

            if(at_sms_status != ATC_SMS_READ)
            {
                *status_ptr = FALSE;
            }
            else
            {
                *changed_status_ptr = MN_SMS_MT_READED;
            }

            break;

        case MN_SMS_MO_TO_BE_SEND:

            if(ATC_SMS_SENT != at_sms_status)
            {
                *status_ptr = FALSE;
            }
            else
            {
                *changed_status_ptr = MN_SMS_MO_SR_NOT_REQUEST;
            }

            break;

        default:

            if(ATC_SMS_UNSENT != at_sms_status)
            {
                *status_ptr = FALSE;
            }
            else
            {
                *changed_status_ptr = MN_SMS_MO_TO_BE_SEND;
            }

            break;
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

    for(i = 0; i < ATC_SMS_ALL; i++)
    {
        if(!strcmp((char *)status_ptr, s_sms_state_str[i]))
        {
            *sms_status_ptr = (ATC_SMS_STATE_E)i;
            return_flag = TRUE;
            break;
        }
    }

    return return_flag;
}

/*****************************************************************************/
//  Description : report SMSCB in text mode
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSmscbMsgPagesIndInText(
    ATC_CONFIG_T                 *atc_config_ptr,    //IN:
    APP_MN_SMSCB_MSG_PAGES_IND_T *sig_ptr            //IN:
)
{
    BOOLEAN result  = TRUE;
#ifndef ATC_SAT_ENABLE
    MN_SMSCB_DCS_T  cb_dcs_t;
    uint16          data_to_TE_len  = 0;

    uint16  sn      = 0;
    uint16  mid     = 0;
    uint8   dcs     = 0;
    uint8   page    = 0;
    uint8   i       = 0;
    int16   msg_len = 0;
    uint8   msg_arr[MN_CB_MSG_LEN+1] = {0};

    int32   str_len      = 0;

    for(i = 0; i < sig_ptr->total_pages; i++)
    {
        if(6 <= sig_ptr->buf_len[i])
        {
            // Octet Number(s)  Field
            // 1-2              Serial Number
            // 3-4              Message Identifier
            // 5                Data Coding Scheme
            // 6                Page Parameter
            // 7-88             Content of Message

            sn      = (sig_ptr->page_buf[i][0] << 8) + sig_ptr->page_buf[i][1];
            mid     = (sig_ptr->page_buf[i][2] << 8) + sig_ptr->page_buf[i][3];
            dcs     = sig_ptr->page_buf[i][4];
            page    = (sig_ptr->page_buf[i][5] & 0xF0) >> 4;

            // +CBM: <sn>,<mid>,<dcs>,<page>,<pages><CR><LF><data> (text mode enabled)
            str_len = sprintf((char *)g_rsp_str, "+CBM: %d,%d,%d,%d,%d%c%c",
                              sn, mid, dcs, page, sig_ptr->total_pages, atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
//#ifdef _ATC_WAIT_
            /* add <data> */
            if(MN_RETURN_SUCCESS == MNSMSCB_ConvertPdu2TextEx(sig_ptr->dual_sys,
                                                              msg_arr, &msg_len, &cb_dcs_t, sig_ptr->page_buf[i], sig_ptr->buf_len[i]))
            {
                ATC_EncodeSMSDataToTE(msg_arr, msg_len, cb_dcs_t.alphabet_type,
                                      atc_config_ptr->cscs[sig_ptr->dual_sys], &(g_rsp_str[str_len]), &data_to_TE_len);
                str_len += data_to_TE_len;
                g_rsp_str[str_len] = '\0';

                ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
            }
            else
//#endif/*_ATC_WAIT_*/
            {
                result = FALSE;
                break;
            }
        }
        else
        {
            result = FALSE;
            break;
        }
    }
#endif
    return (result);
}

/*****************************************************************************/
//  Description : report SMSCB in PDU mode
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSmscbMsgPagesIndInPDU(
    ATC_CONFIG_T                 *atc_config_ptr,    //IN:
    APP_MN_SMSCB_MSG_PAGES_IND_T *sig_ptr            //IN:
)
{
    BOOLEAN result  = TRUE;
#ifndef ATC_SAT_ENABLE
    uint8   hex_arr[SMSCB_PAGE_LENGTH * 2 + 1] = {0};
    int32   str_len = 0;
    uint8   i       = 0;

    for(i = 0; i < sig_ptr->total_pages; i++)
    {
        // +CBM: <length><CR><LF><pdu> (PDU mode enabled)
        str_len = sprintf((char *)g_rsp_str, "+CBM: %d%c%c",
                          sig_ptr->buf_len[i], atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);

        if(sig_ptr->buf_len[i] <= SMSCB_PAGE_LENGTH)
        {
            ConvertBinToHex(sig_ptr->page_buf[i], sig_ptr->buf_len[i], hex_arr);

            str_len += sprintf((char *)(g_rsp_str + str_len), "%s", hex_arr);
            g_rsp_str[str_len] = '\0';
            ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
        }
        else
        {
            result = FALSE;
        }
    }
#endif
    return (result);
}

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function handle the APP_MN_SMSCB_MSG_PAGES_IND signal
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSmscbMsgPagesInd(
    ATC_CONFIG_T                 *atc_config_ptr,    //IN:
    APP_MN_SMSCB_MSG_PAGES_IND_T *sig_ptr    //IN:
)
{
    ATC_STATUS      at_status = S_ATC_SUCCESS;

    SCI_ASSERT(NULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != sig_ptr);/*assert verified: check null pointer*/

    if(s_sms_config[sig_ptr->dual_sys].bm_ind && (atc_config_ptr->urc_level != ATC_URC_ALL_STATUS_OFF))
    {
        if(atc_config_ptr->atc_config_nv.cmgf_flag)
        {
            // report in text mode
            if(!HandleSmscbMsgPagesIndInText(atc_config_ptr, sig_ptr))
            {
                at_status = S_ATC_FAIL;
            }
        }
        else
        {
            //report in pdu mode
            if(!HandleSmscbMsgPagesIndInPDU(atc_config_ptr, sig_ptr))
            {
                at_status = S_ATC_FAIL;
            }
        }
    }

    return (at_status);
}

/*****************************************************************************/
//  Description : This function handles the AT+CMMS command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMMS)
{
    ATC_STATUS status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(!PARAM1_FLAG)
            {
                SCI_TRACE_LOW("ATC: ATC_ProcessCMMS, no PARAM1");
                return ERR_INVALID_INDEX;
            }

            /* NOTE: Maybe timer is active
            * anyway, stop timer */
            ATC_ActiveCMMSTimer(FALSE);

            s_sms_config[dual_sys].cmms_value = (ATC_CMMS_VALUE)PARA1.num;

            switch(s_sms_config[dual_sys].cmms_value)
            {
                case ATC_CMMS_DISABLE:
                    /* disable */
                    s_sms_config[dual_sys].cmms_flag = FALSE;
                    break;

                case ATC_CMMS_KEEP_ENABLED:
                    /* keep enabled until the time between the response of
                    * the latest message send command (+CMGS, +CMSS, etc.)
                    * and the next send command exceeds 1-5 seconds (the exact
                    * value is up to ME implementation), then ME shall close
                        * the link and TA switches <n> automatically back to 0 */
                    s_sms_config[dual_sys].cmms_flag = TRUE;
                    break;

                case ATC_CMMS_ENABLED:
                    /* enable (if the time between the response of the latest
                    * message send command and the next send command exceeds
                    * 1-5 seconds (the exact value is up to ME implementation),
                    * ME shall close the link but TA shall not switch automatically
                        * back to <n>=0) */
                    s_sms_config[dual_sys].cmms_flag = TRUE;
                    break;

                default:
                    SCI_ASSERT(FALSE);/*assert verified: check valid value*/
                    break;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d",
                    g_atc_info_table[AT_CMD_CMMS].cmd_name->str_ptr,
                    s_sms_config[dual_sys].cmms_value);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return status;
}
#endif

/*****************************************************************************/
//  Description : This function is used parse the cb parameters for AT+CSCB
//  Global resource dependence :
//  Author:       Yi.Jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_ParseCSCBParams(
    uint8          *str_ptr,         // in : the input string type param
    uint8           str_len,         // the input string length
    uint8           *digit_num,    // Out: return the obtained number of digits
    uint16          *digit_array,   // Out: return the obtained array of digits
    uint8            max
)
{
    uint8 i = 0;
    uint8 array_index = 0;
    uint16 curr_digit   = 0;
    uint16 pre_digit    = 0xffff;
    uint8 curr_pos      = 0;
    uint32 valid_digit = 0;

    // check if invalid character exists or not
    for(i = 0; i < str_len; i++)
    {
        if(str_ptr[i] > '9' || str_ptr[i] < '0')
        {
            if((i > 0) && ((str_ptr[i] == '-') || (str_ptr[i] == ',')))
            {
                if((str_ptr[i] == ',') && (str_ptr[i + 1] > '9' || str_ptr[i + 1] < '0'))
                {
                    return FALSE;
                }
            }
            else
            {
                return FALSE;
            }
        }
    }

    i = 0;

    *digit_num = 0;

    while(curr_pos < str_len)
    {
        while(str_ptr[curr_pos + i] <= '9' && str_ptr[curr_pos + i] >= '0')
        {
            valid_digit = curr_digit * 10;
            valid_digit += str_ptr[curr_pos + i] - '0' ;

            if(valid_digit <= 0xffff)
            {
                curr_digit = (uint16)valid_digit;
            }
            else
            {
                return FALSE;
            }

            if(!(str_ptr[curr_pos + i + 1] >= '0' && str_ptr[curr_pos + i + 1] <= '9'))
            {
                if(str_ptr[curr_pos + i + 1] == '-')
                {
                    if(pre_digit != 0xffff)
                    {
                        return FALSE;
                    }

                    pre_digit = curr_digit;
                    curr_digit = 0;
                    // continue the loop till curr_digit is filled
                    curr_pos += i + 2;
                    i = 0;

                    if(!(str_ptr[curr_pos] >= '0' && str_ptr[curr_pos] <= '9'))
                    {
                        return FALSE;
                    }

                    continue;
                }
                else
                {
                    if(str_ptr[curr_pos + i + 1] == ',')
                    {
                    }
                    else if(curr_pos + i + 1 != str_len)
                    {
                        return FALSE;
                    }

                    if(pre_digit != 0xffff)   // add a number range
                    {
                        if(curr_digit - pre_digit + (*digit_num) > max)
                        {
                            return FALSE;
                        }

                        while(curr_digit >= pre_digit)
                        {
                            digit_array[array_index++] = pre_digit;
                            (*digit_num) ++;
                            pre_digit ++;
                        }
                    }
                    else  // add  one number
                    {
                        digit_array[array_index++] = curr_digit;
                        (*digit_num) ++;

                        if((*digit_num) > max)
                        {
                            return FALSE;
                        }
                    }

                    curr_digit = 0 ;
                    pre_digit = 0xffff;
                }

                break;
            }

            i++; // move on
        }

        curr_pos += i + 1;
        i = 0;
    }


    return TRUE;
}


/*****************************************************************************/
//  Description : This function handles the +CMMS timer expire
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
void ATC_ProcessCMMSTimeout(uint32 lparam)
{
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(s_sms_config[dual_sys].cmms_value)
    {
        case ATC_CMMS_DISABLE:
            SCI_TRACE_LOW("ATC: CMMSTimeout: WARNING! DISABLE == cmms_value");
            s_sms_config[dual_sys].cmms_flag  = FALSE;
            break;

        case ATC_CMMS_KEEP_ENABLED:
            SCI_TRACE_LOW("ATC: CMMSTimeout: cmms_value: KEEP_ENABLED->DISABLE");
            s_sms_config[dual_sys].cmms_value = ATC_CMMS_DISABLE;
            s_sms_config[dual_sys].cmms_flag  = FALSE;
            break;

        case ATC_CMMS_ENABLED:
            SCI_TRACE_LOW("ATC: CMMSTimeout: cmms_value: ENABLED");
            s_sms_config[dual_sys].cmms_flag  = TRUE;
            break;

        default:
            SCI_TRACE_LOW("ATC: CMMSTimeout: WARNING! unknown cmms_value:%d", s_sms_config[dual_sys].cmms_value);
            s_sms_config[dual_sys].cmms_flag  = FALSE;
            break;
    }
}

/**********************************************************************
// Description:
//      the funciton to start CMMS timer
// Global resource dependence :
// Author: Pizer.Fan
// Note:
***********************************************************************/
void ATC_ActiveCMMSTimer(BOOLEAN act_flg)
{
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    /* Anyway, deactivate timer firstly. */
    if(SCI_IsTimerActive(s_sms_config[dual_sys].cmms_timer))
    {
        SCI_DeactiveTimer(s_sms_config[dual_sys].cmms_timer);
    }

    /* Activate timer */
    if(act_flg)
    {
        SCI_TRACE_LOW("ATC: ATC_ActiveCMMSTimer, active timer(cmms_value=%d,cmms_flag=%d)",
                      s_sms_config[dual_sys].cmms_value, s_sms_config[dual_sys].cmms_flag);
        SCI_ChangeTimer(s_sms_config[dual_sys].cmms_timer, ATC_ProcessCMMSTimeout, ATC_CMMS_TIMER_VALUE);
        SCI_ActiveTimer(s_sms_config[dual_sys].cmms_timer);
    }
    /* Deactivate timer */
    else
    {
        SCI_TRACE_LOW("ATC: ATC_ActiveCMMSTimer, deactive timer(cmms_value=%d,cmms_flag=%d)",
                      s_sms_config[dual_sys].cmms_value, s_sms_config[dual_sys].cmms_flag);
    }
}

/*****************************************************************************/
//  Description : This function converts the absolute period to the relative period
//  Global resource dependence :
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL uint8 GetValidityPeriod(uint32 periodsecond)
{
    uint8        vp = 0;

    if(periodsecond > ((ATC_SMS_DAYS_VAL - ATC_SMS_HOURS_VAL + 1) * 24 * 60 * 60))
    {
        // 197 to 255
        vp = (periodsecond / (7 * 24 * 60 * 60)) - 4 + ATC_SMS_DAYS_VAL;
    }
    else if(periodsecond > (12 * 60 * 60 + (ATC_SMS_HOURS_VAL - ATC_SMS_MINUTES_VAL) * 30 * 60))
    {
        // 168 to 196
        vp = (periodsecond / (24 * 60 * 60)) - 1 + ATC_SMS_HOURS_VAL;
    }
    else if(periodsecond > ((ATC_SMS_MINUTES_VAL + 1) * 5 * 60))
    {
        //144 to 167
        vp = ((periodsecond - 12 * 60 * 60) / (30 * 60)) + ATC_SMS_MINUTES_VAL;
    }
    else if(periodsecond == 0)
    {
        SCI_TRACE_LOW("ATC: GetValidityPeriod, warning, invalid absolute period");
    }
    else
    {
        //0 to 143
        vp = (periodsecond / (5 * 60)) - 1;
    }

    return vp;
}

/*****************************************************************************/
//  Description : This function converts TE character set sms address
//  (such as <da> <oa> <ra>..)to the ME character set string(BCD format) in text mode
//  Global resource dependence :
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_DecodeSMSAddrChsetFromTE(
    ATC_CUR_PARAM_STRING_T *sms_addr_ptr,  //in, sms address info
    int32         addr_type,              //in, address type value
    uint8         TE_chset_type,          //in, TE character set type
    MN_CALLED_NUMBER_T     *called_number_ptr  //out, call number structure
)
{
    ATC_TELE_TYPE_E        to_addr = ISDN_TELE_PLAN_UNKNOWN_TYPE;
    uint8                  *addr_str_ptr = NULL;
    uint16                  addr_str_len = 0;
    uint8                  tel_str[(MN_MAX_ADDR_BCD_LEN<<1)+1] = {0};
    uint16                 tel_str_len = 0;

    uint8                  bcd_str[MN_MAX_ADDR_BCD_LEN+1] = {0};
    uint8                  bcd_len = 0;


    SCI_ASSERT(NULL != sms_addr_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != called_number_ptr);/*assert verified: check null pointer*/

    /*get address type*/
    if(!(addr_type == UNKNOWN_PLAN_UNKNOWN_TYPE ||
            addr_type == ISDN_TELE_PLAN_UNKNOWN_TYPE ||
            addr_type == ISDN_TELE_PLAN_INTERNATIONAL_TYPE ||
            addr_type == ISDN_TELE_PLAN_NATIONAL_TYPE))
    {
        // Operation not supported
        return FALSE;
    }

    to_addr = (ATC_TELE_TYPE_E)addr_type;

    /*addr_str_ptr points to the TE character set string which is input.*/
    addr_str_ptr = sms_addr_ptr->str_ptr;
    addr_str_ptr[sms_addr_ptr->str_len] = '\0';
    addr_str_len = sms_addr_ptr->str_len;

    /*convert string from TE character set to MT character set*/
    if(!ATC_ChsetTransformation((ATC_CHARACTER_SET_TYPE_E)TE_chset_type,
                                addr_str_ptr, addr_str_len,
                                ATC_CHSET_IRA,
                                (uint16)((MN_MAX_ADDR_BCD_LEN << 1) + 1), //max IRA string length
                                tel_str, &tel_str_len))
    {
        return FALSE;
    }
    SCI_TRACE_LOW("ATC: tel_str:%s", tel_str);

    /*addr_str_ptr points to the TA character set string.*/
    addr_str_ptr = tel_str;

    if(*addr_str_ptr == '+')
    {
        /* Type of number set to ISDN_TELE_PLAN_INTERNATIONAL_TYPE */
        to_addr = ISDN_TELE_PLAN_INTERNATIONAL_TYPE;
        addr_str_ptr++;
        tel_str_len--;
    }

    if(!TransfromStrToBcd(bcd_str, &bcd_len, addr_str_ptr, tel_str_len))
    {
        SCI_TRACE_LOW("ATC: TransfromStrToBcd failed.");
        return FALSE;
    }
    
    SCI_TRACE_LOW("ATC: bcd_len:%d", bcd_len);

    if(bcd_len > (MN_MAX_ADDR_BCD_LEN/2) || 0 == bcd_len)
    {
        SCI_TRACE_LOW("ATC: invalid bcd length:%d", bcd_len);
        return FALSE;
    }

    /*decode address info into ME format*/
    SCI_MEMSET(called_number_ptr->party_num, 0, MN_MAX_ADDR_BCD_LEN);
    SCI_MEMCPY(called_number_ptr->party_num, bcd_str, bcd_len);
    called_number_ptr->num_len = (uint16)bcd_len;
    called_number_ptr->number_plan = (MN_NUMBER_PLAN_E)(to_addr & 0x0F);
    called_number_ptr->number_type = (MN_NUMBER_TYPE_E)((to_addr & 0x70) >> 4);

    return TRUE;
}

/*****************************************************************************/
//  Description : This function converts sms address(such as <da> <oa> <ra>..)
//                 to the TE character set string in text mode
//  Global resource dependence :
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_EncodeCalledNumToTE(
    MN_CALLED_NUMBER_T     *called_number_ptr,  //input, call number structure
    uint8                   dest_chset,       //in, character set of telephone number
    ATC_TELE_TYPE_E        *tel_type_ptr,       //output,telephone number type
    uint8                  *tel_str_ptr,        //output,telephone number string buffer
    //the max length must be ATC_MAX_SMS_ADDRESS_LEN_FOR_TE
    uint16                 *tel_str_len_ptr     //output, telephone number length
)
{
    uint8     tel_num_arr[MN_MAX_ADDR_BCD_LEN*2+1] = {0};
    uint16    tel_num_len = 0;

    if(NULL == called_number_ptr || NULL == tel_str_ptr)
    {
        SCI_TRACE_LOW("ATC: ATC_EncodeCalledNumToTE, invalid parameters.");
        return FALSE;
    }

    /*first, convert bcd number to ira string*/
    ATC_ConvertCalledNumToStr(called_number_ptr, tel_type_ptr, tel_num_arr, &tel_num_len);

    /*then, convert ira string to specific character set*/
    if(!ATC_TransformIRAToOtherChset(tel_num_arr, tel_num_len,
                                     (ATC_CHARACTER_SET_TYPE_E)dest_chset,
                                     ATC_MAX_SMS_ADDRESS_LEN_FOR_TE,
                                     tel_str_ptr, tel_str_len_ptr))
    {
        SCI_MEMSET(tel_str_ptr, 0, ATC_MAX_SMS_ADDRESS_LEN_FOR_TE);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
/*****************************************************************************/
//  Description : This function converts sms data
//                 to the TE character set string in text mode
//  Global resource dependence :
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_EncodeSMSDataToTE(
    uint8        *data_str,        //in, sms data
    uint16        data_len,        //in, sms data length
    MN_SMS_ALPHABET_TYPE_E      dcs,   //in, the Data Coding Scheme of sms data
    uint8         dest_chset,       //in, character set of sms <data>
    uint8        *data_to_TE_ptr,     //out, sms data(reported to TE)
    uint16       *data_to_TE_len_ptr  //out, sms data(reported to TE) length
)
{
    if(NULL == data_str || 0 == data_len || NULL == data_to_TE_ptr)
    {
        SCI_TRACE_LOW("ATC: ATC_EncodeSMSDataToTE, invalid parameters.");
        return FALSE;
    }

    if(MN_SMS_DEFAULT_ALPHABET == dcs)
    {
        /*sms data is decoded first(from GSM to IRA) by calling MNSMS_DecodeUserData*/
        /*ME/TA converts IRA to current TE character set*/
        return ATC_TransformIRAToOtherChset(data_str, data_len,
                                            (ATC_CHARACTER_SET_TYPE_E)dest_chset,
                                            ATC_MAX_SMS_DATA_LEN_FOR_TE,
                                            data_to_TE_ptr, data_to_TE_len_ptr);
    }
    else
    {
        /*ME/TA converts each 8-bit octet into two IRA character long hexadecimal number*/
        ConvertBinToHex(data_str, data_len, data_to_TE_ptr);

        if(NULL != data_to_TE_len_ptr)
        {
            *data_to_TE_len_ptr = data_len * 2;
        }

        return TRUE;
    }
}

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function used to initialize sms cell broadcast parameters
//  Global resource dependence :
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
void ATC_InitSmsCellBroadcastPara(ATC_CONFIG_T *atc_config_ptr, BOOLEAN is_all_init, MN_DUAL_SYS_E dual_sys)
{
    ATC_CB_MSG_ID_MUSTER_T                msg_arr[MN_SYS_NUMBER] = {0};
    MN_CB_LANGUAGE_ID_MUSTER_T        lang_arr[MN_SYS_NUMBER] = {0};
    uint8 i;

    // get the cell broadcast parameters
    if(NVERR_NONE != EFS_NvitemRead(NV_ATC_CB_MSG_ID, 
                    MN_SYS_NUMBER * sizeof(ATC_CB_MSG_ID_MUSTER_T), (uint8 *)msg_arr)) 
    {
        SCI_TRACE_LOW("ATC: Read msg from NV fail");
        SCI_MEMSET((void *)msg_arr, 0, MN_SYS_NUMBER * sizeof(ATC_CB_MSG_ID_MUSTER_T));
        // save msg_arr to NV
        if(NVERR_NONE != EFS_NvitemWrite(NV_ATC_CB_MSG_ID,
                        MN_SYS_NUMBER * sizeof(ATC_CB_MSG_ID_MUSTER_T), (uint8 *)msg_arr, FALSE))
        {
            SCI_TRACE_LOW("ATC: Write NV_ATC_CB_MSG_ID failed");
            return;
        }
    }
                                

    if(NVERR_NONE != EFS_NvitemRead(NV_ATC_CB_LANG_ID, 
                    MN_SYS_NUMBER * sizeof(MN_CB_LANGUAGE_ID_MUSTER_T), (uint8 *)lang_arr))
    {
        SCI_TRACE_LOW("ATC: Read language from NV fail");
        SCI_MEMSET((void *)lang_arr, 0, MN_SYS_NUMBER * sizeof(MN_CB_LANGUAGE_ID_MUSTER_T));
          // save lang_arr to NV
          if (NVERR_NONE != EFS_NvitemWrite(NV_ATC_CB_LANG_ID,
                           MN_SYS_NUMBER * sizeof(MN_CB_LANGUAGE_ID_MUSTER_T), (uint8 *)lang_arr, FALSE))
          {
              SCI_TRACE_LOW("ATC: Write NV_ATC_CB_LANG_ID failed");
              return;
          }
    }

    if (is_all_init)
    {
        for (i = 0; i < MN_SYS_NUMBER; i++)
        {
            if (msg_arr[i].is_enable)
            {
                atc_config_ptr->smscb_mode[i] = FALSE;
                MNSMSCB_StartCbMsgEx((MN_DUAL_SYS_E)i, &(msg_arr[i].cb_msg_id), &lang_arr[i]);
                atc_config_ptr->smscb_flag[i] = TRUE;
            }
            else
            {
                atc_config_ptr->smscb_mode[i] = FALSE;
                atc_config_ptr->smscb_flag[i] = FALSE;
            }
        }
    }
    else if (dual_sys < MN_SYS_NUMBER)
    {
        if (msg_arr[dual_sys].is_enable)
        {
            atc_config_ptr->smscb_mode[dual_sys] = FALSE;
            MNSMSCB_StartCbMsgEx(dual_sys, &(msg_arr[dual_sys].cb_msg_id), &lang_arr[dual_sys]);
            atc_config_ptr->smscb_flag[dual_sys] = TRUE;
        }
        else
        {
            atc_config_ptr->smscb_mode[dual_sys] = FALSE;
            atc_config_ptr->smscb_flag[dual_sys] = FALSE;
        }
    }
    else
    {
         SCI_TRACE_LOW("ATC: ATC_InitSmsCellBroadcastPara  wrong sim id %d > MN_SYS_NUMBER %d", dual_sys, MN_SYS_NUMBER);
         return;
    }

}
#endif

/*****************************************************************************/
//  Description : This function converts MN_SMS_CAUSE_E to ATC_SMS_ERR_CODE
//  Global resource dependence :
//  Author:       andy.huang
//  Note:
/*****************************************************************************/
ATC_SMS_ERR_CODE_E ATC_ConvMnSmsCause2AtcSmsCause(   //return: ATC_SMS_ERR_CODE_E, refer to 3GPP TS 27.005/3.2.5
    MN_SMS_CAUSE_E input_mn_sms_err                  //IN: mn sms error code
)
{
    ATC_SMS_ERR_CODE_E atc_sms_err = ATC_SMS_ERR_500;

    SMS_ERR_CONV_ITEM_T err_conv_table[] = 
    {
        {MN_SMS_UNSPEC_ERROR,           ATC_SMS_ERR_302},
        {MN_SMS_WRITE_NV_FAILED,        ATC_SMS_ERR_320},
        {MN_SMS_ME_MEM_FULL,            ATC_SMS_ERR_322},
        {MN_SMS_ME_NOT_SUPPORT,         ATC_SMS_ERR_303},
        {MN_SMS_ME_DELETE_FAILURE,      ATC_SMS_ERR_302},
        {MN_SMS_SIM_NOT_READY,          ATC_SMS_ERR_310},
        {MN_SMS_SIM_MEM_FULL,           ATC_SMS_ERR_322},
        {MN_SMS_SIM_PATTERN_NOT_FIND,   ATC_SMS_ERR_313},
        {MN_SMS_SIM_SEEK_FAILED,        ATC_SMS_ERR_313},
        {MN_SMS_RECORD_OUT_OF_RANGE,    ATC_SMS_ERR_321},
        {MN_SMS_NOT_RIGHT_STORAGE,      ATC_SMS_ERR_320},
        {MN_SMS_SIM_UNKNOW_FAILED,      ATC_SMS_ERR_500}
    };

    uint32 i = 0;

    for(i=0; i < (sizeof(err_conv_table)/sizeof(err_conv_table[0])); i++)
    {
        if( input_mn_sms_err == err_conv_table[i].mn_sms_err )
        {
            atc_sms_err = err_conv_table[i].atc_sms_err;
            break;
        }
    }

    return atc_sms_err;
}

/*****************************************************************************/
//  Description : This function handle sms module when receive sim error 
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
void ATC_SmsSimRemovedInd(
        MN_DUAL_SYS_E dual_sys,
        ATC_CONFIG_T     *atc_config_ptr
        )
{
    atc_config_ptr->sms_is_ready[dual_sys] = FALSE;
}
/*****************************************************************************/
//  Description : This function reset sms sending status 
//  Global resource dependence :
//  Author:Haoming.Lu
//  Note:
/*****************************************************************************/
void ATC_ResetSmsSendingStatus(void)
{
    s_sms_exclusive_flag = FALSE;
}

int strnicmp(char * s1, char * s2, int len)
{
    char  c1,   c2;
    
    while (len--)
    {
        c1 = (char)((*s1++) & ~0x20);
        c2 = (char)((*s2++) & ~0x20);
        if (c1 == c2)
        {
            if (c1 == 0)   /* end of both strings */
                return 0;   /* they match */
            continue;   /* check next char */
        }
        else if(c1 < c2)
            return -1;
        else  /* c1 > c2 */
            return 1;
    }
    return 0;
}

int stricmp(char * s1, const char * s2)
{
    int   l1,   l2;
    
    l1 = strlen(s1);  /* get both lengths */
    l2 = strlen(s2);
    
    /* call strnicmp with maximum length */
    return( strnicmp(s1, s2, ( l1 > l2 )?l1:l2 ));/*lint !e605 */
}

