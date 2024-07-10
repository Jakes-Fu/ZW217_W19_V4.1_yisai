/******************************************************************************
 ** File Name:      atc_basic_cmd.c                                           *
 ** Author:         Xueli.lv                                                  *
 ** Date:           12/08/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 12/08/2002     Xueli.lv         Create.                                   *
 ******************************************************************************/
#include "base_trc.h"
#include "atc_basic_cmd.h"
#include "atc_gsm_sms.h"
#include "atc.h"
#include "sci_service.h"
#include "atc_common.h"
#include "mnclassmark_api.h"
#include "layer1_audio.h"
#include "nv_item_id.h"
#include "env_atc_signal.h"
#include "dal_time.h"
//#include "dal_chr.h"
#include "tasks_id.h"
#include "dal_power.h"
#include "aud_enha_exp.h"
#if 0 //CR171132 for 6530 codesize cutdown
#include "atc_gsm_stk.h"
#endif

#include "dal_power.h"
#include "version.h"
#include "efs.h"
#include "audio_api.h"
#include "custom_tone_adp.h"
#include "dtmf_tone_adp.h"

#ifndef _ULTRA_LOW_CODE_
#ifndef WIN32
#include "gpio_drv.h"
#endif
#include "layer1_engineering.h"
#include "aud_gen.h"
#include "audio_prod.h" 
#endif

#include "power.h"
#include "run_mode.h"
#include "ps_eng.h"
#if !defined(MS_OAPI_SUPPORT) || defined(CHIP_VER_6600L6) || defined(CHIP_VER_6610)
#include "prod_param.h"
#include "ref_outport.h"
#include "aud_proc_exp.h"
#endif

#ifdef AUDIO_PARAM_ARM
#include "audio_nv_arm.h"
#include "audio_nv_dsp.h"
#endif

/* +add for lvve function */
#include "lvve_ctrl_param_nv.h"
/* -add for lvve function */

#include "admm_III.h"
#include "atc_malloc.h" //add by shuyan.hu

//#include "Sc6610_reg_ahb.h"
#include "deep_sleep.h"

#include "atc_samsung_prod.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/
static HAUDIO hAtcCustomRing = INVALID_HANDLE;
static HAUDIO hAtcDtmfRing = INVALID_HANDLE;
extern MN_DUAL_SYS_E       g_current_card_id;
uint8                        g_atc_ciev_rang = 3;//default value
uint8                        g_atc_ciev_lev[4] = {102, 96, 90, 84};

static AUDIO_DEVICE_MODE_TYPE_E       s_aud_dev_mod = AUDIO_DEVICE_MODE_HANDHOLD;
#ifndef _ULTRA_LOW_CODE_
static int16                 s_dsp_ram_flag = 0;     
static int16                 s_dsp_data_length = 0;  
static int16                 s_dsp_dest_addr = 0;
static uint16                s_b_mic_aux_device = FALSE;
static uint16                s_b_spk_aux_device = FALSE;
#endif
static uint16                s_atc_eq_type = AUD_EQ_NORMAL;
static uint16                s_nv_item_id = 0;
static uint8                 s_atc_admm_index = 0;

static uint8                 s_enha_eqmode_index = 0;

//for AT+MAXPOWER or AT+READRSSI command
extern ATC_POWER_INF_T    g_atc_power_info;

#ifdef _MUX_ENABLE_
static  uint16   s_atc_rssi_link_id = 0;
#endif

#if 0 //CR171132 for 6530 codesize cutdown
#ifndef _ULTRA_LOW_CODE_
extern char *                g_stk_terminal_profile[];
#endif
#endif
extern RSP_RESULT_STR        g_rsp_str;
extern const ATC_INFO_T            g_atc_info_table[];

uint16 ring_times[MN_SYS_NUMBER]={0};
uint16 have_ringed_times[MN_SYS_NUMBER]={0};

#ifndef _ULTRA_LOW_CODE_
extern const ATC_RSP_STR_T   g_atc_cpin_rsp[];
#endif
extern BOOLEAN               g_atc_call_flag;
extern ATC_CALLS_STATE_T     g_calls_state[MN_SYS_NUMBER];
extern SCI_TMID                     T_ATC_CALL_EXP_IND;
extern BOOLEAN                      g_atc_expire_ind;
extern BOOLEAN                      g_atc_call_need_disc;
static BOOLEAN               s_received_sim_ready[MN_SYS_NUMBER] = {FALSE};
static uint32                s_sim_error_reason[MN_SYS_NUMBER];

BOOLEAN                      g_is_ps_deactivate[MN_SYS_NUMBER] = {FALSE};
BOOLEAN               g_is_ps_act[MN_SYS_NUMBER] = {FALSE};

#ifndef _ULTRA_LOW_CODE_
SCI_TMID                     T_ATC_POWEROFF_EXP_IND = PNULL;

ATC_CREG_STATE_T g_creg_state[MN_DUAL_SYS_MAX];

#define FRAME_MAX_LEN 1048
#define FRAME_HEAD    0x7E
#define FRAME_STOP    0x0D

#ifdef _MUX_REDUCED_MODE_
uint16   g_rxlev_arr[MN_SYS_NUMBER] = {0};
BOOLEAN  g_received_sim_ind[MN_DUAL_SYS_MAX] = {FALSE};
BOOLEAN  g_sent_sind4 = FALSE;
#endif

#ifdef _MUX_ENABLE_
extern uint8 g_poweroff_exp_link_id;
#endif


// for the at+ipr=<baudrate>  
static const AT_BAUTRATE_CONTEXT_T c_baudrate_data[AT_IPR_MAX] = 
{
    {BAUD_1200, AT_BAUD_1200}, {BAUD_2400, AT_BAUD_2400}, {BAUD_4800, AT_BAUD_4800},
    {BAUD_9600, AT_BAUD_9600}, {BAUD_19200, AT_BAUD_19200}, {BAUD_38400, AT_BAUD_38400},
    {BAUD_57600, AT_BAUD_57600}, {BAUD_115200, AT_BAUD_115200}, {BAUD_230400, AT_BAUD_230400},
    {BAUD_460800, AT_BAUD_460800}
};

// for at+wdtmf=<value>
static const AT_DATA_CONTEXT_T     c_wdtmf_data[] = 
{
    {AUD_DTMF_Zero,  0x30},   // '0'
    {AUD_DTMF_One,   0x31},   // '1'
    {AUD_DTMF_Two,   0x32},   // '2'
    {AUD_DTMF_Three, 0x33},   // '3'
    {AUD_DTMF_Four,  0x34},   // '4'
    {AUD_DTMF_Five,  0x35},   // '5'
    {AUD_DTMF_Six,   0x36},   // '6'
    {AUD_DTMF_Seven, 0x37},   // '7'
    {AUD_DTMF_Eight, 0x38},   // '8'
    {AUD_DTMF_Nine,  0x39},   // '9'
    {AUD_DTMF_Star,  0x2A},   // '*'
    {AUD_DTMF_Pond,  0x23},   // '#'
    {AUD_DTMF_letterA, 0x41}, // 'A'
    {AUD_DTMF_letterB, 0X42}, // 'B'
    {AUD_DTMF_letterC, 0X43}, // 'C'
    {AUD_DTMF_letterD, 0X44}  // 'D'
};
static const uint8          c_sidet_data[AT_MAX_SIDET_LEV] =
{4, 3, 2, 1, 0};

#ifdef _MUX_ENABLE_
extern uint32 MUX_Setbaud_rate(uint32 baud_rate);
extern BOOLEAN  GPIO_IsAllowedReportRssiToAP(void);
#endif

#endif

/*****************************************************************************/
//  Description : This function converts the hex to bin
//  Global resource dependence : 
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN   ConvertInt16ToBin(    // FALSE: There is invalid char
            uint8        *hex_ptr,    // in: the hexadecimal format string of address
            uint16       *bin_ptr      // out: pointer to the address
            );

#ifndef _ULTRA_LOW_CODE_

/*****************************************************************************/
//  Description : This function is used to read preferred PLMN list. 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadPreferredPlmnList(
                ATC_CONFIG_T    *atc_config_ptr
                );

/*****************************************************************************/
//  Description : This function is used to read max num of preferred PLMN list. 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 ReadPreferredPlmnMaxNum(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : This function convert the plmn string to the struct. 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ConvertPlmn(
              MN_PLMN_T                 *plmn_ptr,  //OUT:
              ATC_CUR_PARAM_STRING_T    *str_ptr    //IN:
              );

/*****************************************************************************/
//  Description : This function is used to restore the configuration profile.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void RestoreConfig(
                ATC_CONFIG_T  *atc_config_ptr
                );

/*****************************************************************************/
//  Description : This function is used to save the current configuration profile.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void SaveConfigProfile(
                ATC_CONFIG_T   *atc_config_ptr
                );

/*****************************************************************************/
//  Description : This function handle the AT command which has one parameter
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleSingleCmd(
                ATC_CUR_INFO_LIST_T  *cur_cmd_ptr,   // point to the atc cmd info
                ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                uint8                default_val,     // the default value  
                uint8                *cur_val         // the current value
                );

/*****************************************************************************/
//     Description : This function is used to chech the time string.
//    Global resource dependence : none
//  Author:       Xueli Lv
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckTimeStr(
                    uint8      length,
                    uint8      *str,
                    SCI_DATE_T   *date_ptr,
                    SCI_TIME_T   *tm_ptr
                    );


#ifdef _MUX_ENABLE_
/*****************************************************************************/
//  Description : This function is used to retrive the data between 0x7e
//                and transferred meaning operation 
//  Global resource dependence : 
//  return:       TRUE -- receive data successful
//                FALSE -- receive data failed
//  Author:       Shijun.cui
//  Note: provided by Gionee
/*****************************************************************************/
//0x7e--abc---0x7e//  == > --abc--
LOCAL uint32 FillFrameData(
                         ATC_CONFIG_T *atc_config_ptr,
                         const uint8  *buf_ptr,
                         int32        len
                         );
#endif

/*****************************************************************************/
//  Description : This function initializes the static variables 
//  Global resource dependence : 
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern void ATC_InitBasicVar(void)
{
    uint8    i = 0;
        
    for (i = MN_DUAL_SYS_1; i < MN_SYS_NUMBER; i++)
    {
        g_is_ps_deactivate[i] = FALSE;
        g_is_ps_act[i]     = FALSE;
    }
    
}
#endif

/*****************************************************************************/
//  Description : This function handle the ATQ command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessQ )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 0, &atc_config_ptr->q_value);    
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the ATV command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessV )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 1, &atc_config_ptr->v_value);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CMEE command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMEE )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 0, (uint8*)&atc_config_ptr->atc_config_nv.cmee);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handleS the AT+CPOF command. This command is used
//  to power off the module
//  AT+CPOF
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPOF )
{
#ifndef _ULTRA_LOW_CODE_
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_DEACTIVE_PS_CNF,0xff,ATC_NO_DOMAIN);
    // power off the spreadtrum module
    MNPHONE_DeactivePsEx(dual_sys);

    // start the timer to periodic show the calling number
    SCI_SET_TIMER(P_ATC, ATC_POWEROFF_INTERVAL, ATC_POWEROFF_EXP_IND, 
        &T_ATC_POWEROFF_EXP_IND, "T_ATC_POWEROFF_EXP_IND");
#ifdef _MUX_ENABLE_
    g_poweroff_exp_link_id = atc_config_ptr->current_link_id;
#endif
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description: This function handles the AT+CPOL command. 
//               This command is used to edit the SIM preferred list of networks.
//  Global resource dependence : g_rsp_str
//  Author: Tracy Zhang
//  Note: +CPOL=[<index>][,<format>[,<oper>]]
//        1 <= index <= 85 
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPOL )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS                              status  = S_ATC_DEFAULT_HANDLE;
    MN_PHONE_WRITE_PLMN_T   plmn_item;
    ATC_CUR_PARAM_STRING_T      *str_ptr = PNULL;
    uint32                                  max_num = 0;
   MN_DUAL_SYS_E                      dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // If <index> is given but <oper> is left out, entry is deleted. 
            if (PARAM1_FLAG && !PARAM3_FLAG)
            {
                plmn_item.order_of_plmn  = PARA1.num;
                plmn_item.is_delete_item = TRUE;        // delete item in the plmn list 

                if (ERR_MNPHONE_NO_ERR != MNPHONE_WritePLMNtoSimEx(dual_sys, plmn_item))
                {
                    status = S_ATC_FAIL;
                }
                break;
            }

            if ((!PARAM2_FLAG) || (PARA2.num != 2))
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }

            // If only <format> is given, the format of the <oper> in the read command is changed. 
            if (!PARAM1_FLAG && !PARAM3_FLAG)
            {
                if ((PARAM2_FLAG) && (2 == PARA2.num))
                {
                    status = S_ATC_SUCCESS;
                }
                break;
            }

            if (!PARAM1_FLAG && PARAM3_FLAG)
            {
                // If <oper> is given but <index> is left out, <oper> is put in the next free location. 
                plmn_item.order_of_plmn = 0;    //invalid order
            }
            else
            {
                plmn_item.order_of_plmn = PARA1.num;
            }
            plmn_item.is_delete_item = FALSE;   // write item in the plmn list   

            /* write PLMN with special format. */
            str_ptr  = PARA3.str_ptr;

            if (!ConvertPlmn(&(plmn_item.plmn), str_ptr))
            {
                status = ERR_OPERATION_NOT_SUPPORTED;
                break;
            }

            if (ERR_MNPHONE_NO_ERR != MNPHONE_WritePLMNtoSimEx(dual_sys, plmn_item))
            {
                status = S_ATC_FAIL;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            // read all items from the plmn list
            if (ReadPreferredPlmnList(atc_config_ptr))
            {
                status = S_ATC_SUCCESS;
            }
            else
            {
                status = S_ATC_FAIL;
            }
            break;
        }

        case ATC_CMD_TYPE_TEST:
        {
            max_num = ReadPreferredPlmnMaxNum(dual_sys);
            if (0 < max_num)
            {
                sprintf((char*)g_rsp_str, "%s: (1-%ld),(0-2)",\
                            g_atc_info_table[AT_CMD_CPOL].cmd_name->str_ptr,
                            max_num);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                status = S_ATC_SUCCESS;
            }
            break;
        }

        default:
        {
            status = ERR_OPERATION_NOT_ALLOWED;
            break;

        }
    }
    return (status);
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_PREFER_PLMN_CHANGE_CNF at the
//                 AT+CPOL 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPreferPlmnChangCnf(
                ATC_CONFIG_T                    *atc_config_ptr,
                APP_MN_PREFER_PLMN_CHANGE_CNF_T *sig_ptr
                )
{
    //SCI_TRACE_LOW:"ATC_ProcessPreferPlmnChangCnf result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_585_112_2_17_22_16_31_112,(uint8*)"d", sig_ptr->is_ok);
    if (sig_ptr->is_ok)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
    }
    return S_ATC_DEFAULT_HANDLE;
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function is used to read preferred PLMN list. 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadPreferredPlmnList(
                ATC_CONFIG_T    *atc_config_ptr
                )
{
    BOOLEAN                                         result = TRUE;
    MN_PHONE_PLMN_SEL_LIST_T    plmnsel_list ;
    uint32                                                     i = 0;
    uint32                                             offset = 0;
    uint32                                    num_per_lin = 0;
    char                                         plmn_str[8] = {0};
    MN_DUAL_SYS_E                            dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(plmn_str, 0, sizeof(plmn_str));

    if (ERR_MNPHONE_NO_ERR == MNPHONE_GetPlmnSelectListEx(dual_sys, &plmnsel_list))
    {
        for (i=0; i<plmnsel_list.plmn_validate_num; i++)
        {
            sprintf(plmn_str, "%03d", plmnsel_list.plmn_arr[i].mcc);
            
            if (2 == plmnsel_list.plmn_arr[i].mnc_digit_num)
            {
                sprintf(plmn_str+3, "%02d", plmnsel_list.plmn_arr[i].mnc);
            }
            else
            {
                sprintf(plmn_str+3, "%03d", plmnsel_list.plmn_arr[i].mnc);
            }

            num_per_lin = sprintf((char*)(g_rsp_str + offset), "%s: %lu, 2, \"%s\"\r\n",
                                    g_atc_info_table[AT_CMD_CPOL].cmd_name->str_ptr, i+1, plmn_str);
            offset += num_per_lin;
            if ((offset + num_per_lin > MAX_ATC_RSP_LEN)
                || (i+1 == plmnsel_list.plmn_validate_num))
            {
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                offset = 0;
            }
        }
    }
    else
    {
        result = FALSE;
    }
    return (result);
}

/*****************************************************************************/
//  Description : This function is used to read max num of preferred PLMN list. 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 ReadPreferredPlmnMaxNum(MN_DUAL_SYS_E dual_sys)
{
    uint32                                       max_num = 0;
    MN_PHONE_PLMN_SEL_LIST_T    plmnsel_list;
    
    if (ERR_MNPHONE_NO_ERR == MNPHONE_GetPlmnSelectListEx(dual_sys, &plmnsel_list))
    {
        max_num = plmnsel_list.plmn_max_num;
    }
    
    //SCI_TRACE_LOW:"ATC: ReadPreferredPlmnMaxNum max_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_663_112_2_17_22_16_31_113,(uint8*)"d", max_num);
    return (max_num);
}

/*****************************************************************************/
//  Description : This function convert the plmn string to the struct. 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ConvertPlmn(
                  MN_PLMN_T                 *plmn_ptr,  //OUT:
                  ATC_CUR_PARAM_STRING_T    *str_ptr    //IN:
                  )
{
    BOOLEAN    result = TRUE;
    uint32                i = 0;
    char    mcc_str[4] = {0};
    char    mnc_str[4] = {0};

    SCI_MEMSET(plmn_ptr, 0, sizeof (MN_PLMN_T));
    
    if ((5 == str_ptr->str_len) || (6 == str_ptr->str_len))
    {
        for (i=0; i<str_ptr->str_len; i++)
        {
            if ((str_ptr->str_ptr[i] < '0') || (str_ptr->str_ptr[i] > '9')) 
            {
                result = FALSE;
                break;
            }
        }
    }
    else
    {
        result = FALSE;
    }
    
    if (result)
    {
        strncpy(mcc_str, (char*)str_ptr->str_ptr, 3);
        strcpy(mnc_str, (char*)&(str_ptr->str_ptr[3]));

        plmn_ptr->mcc = atoi(mcc_str);
        plmn_ptr->mnc = atoi(mnc_str);
        
        if (5 == str_ptr->str_len)
        {
            plmn_ptr->mnc_digit_num = 2;
        }
        else
        {
            plmn_ptr->mnc_digit_num = 3;
        }
        //SCI_TRACE_LOW:"ConvertPlmn PLMN(%d, %d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_715_112_2_17_22_16_31_114,(uint8*)"dd", plmn_ptr->mcc, plmn_ptr->mnc);
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : This function handle the AT+CREG command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCREG )
{
#ifndef _ULTRA_LOW_CODE_
    MN_PHONE_CURRENT_PLMN_INFO_T   register_status;
    OPER_STATUS_E                  oper_status;
    ATC_STATUS                     status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                atc_config_ptr->atc_config_nv.creg = (uint8)PARA1.num;
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
                
                switch (atc_config_ptr->atc_config_nv.creg)
                {
                    case ATC_CREG_DISABLE:
                    {
                        return S_ATC_SUCCESS;
                    }

                    case ATC_CREG_ENABLE:
                    {
                        sprintf((char*)g_rsp_str,"%s: %d", g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr,oper_status);
                        break;
                    }

                    case ATC_CREG_AND_INFO:
                    {
                        if ((REGISTERED_ROAMING == oper_status)
                            || (REGISTERED_HOME_PLMN == oper_status))
                        {
                            sprintf((char*)g_rsp_str,"%s: %d,0x%04X,0x%04X", g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr,
                                oper_status, register_status.lac, register_status.cell_id);
                        }
                        else
                        {
                            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr,oper_status);
                        }
                        break;
                    }

                    default:
                    {
                        //SCI_TRACE_LOW:"ATC: Assert Failure unknow creg."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_812_112_2_17_22_16_31_115,(uint8*)"");
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = ERR_INVALID_INDEX;
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
            
            switch (atc_config_ptr->atc_config_nv.creg)
            {
                case ATC_CREG_DISABLE:
                {
                    sprintf((char*)g_rsp_str,"%s: %d,%d", g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr,
                        atc_config_ptr->atc_config_nv.creg, oper_status);
                    break;
                }

                case ATC_CREG_ENABLE:
                {
                    sprintf((char*)g_rsp_str,"%s: %d,%d",g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr,
                        atc_config_ptr->atc_config_nv.creg, oper_status);
                    break;
                }

                case ATC_CREG_AND_INFO:
                {
                    if ((REGISTERED_ROAMING == oper_status)
                        || (REGISTERED_HOME_PLMN == oper_status))
                    {
                        sprintf((char*)g_rsp_str,"%s: %d,%d,0x%04X,0x%04X", g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr,
                            atc_config_ptr->atc_config_nv.creg, oper_status, register_status.lac, register_status.cell_id);
                    }
                    else
                    {
                        sprintf((char*)g_rsp_str, "%s: %d,%d",g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr,
                            atc_config_ptr->atc_config_nv.creg, oper_status);
                    }
                    break;
                }

                default:
                {
                    //SCI_TRACE_LOW:"ATC: Assert Failure unknow creg enum."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_903_112_2_17_22_16_31_116,(uint8*)"");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            status = S_ATC_SUCCESS;
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert CREG,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_914_112_2_17_22_16_31_117,(uint8*)"");
            break;
        }       
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CSQ command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSQ )
{
    MN_PHONE_SCELL_MEAS_T cell_info; // the variable holds the cell information
    uint8                                      rssi  = AT_RXLEV_ZERO; // the received signal strength indication
    uint16                          rxqual_full  = AT_INVALID_RXQUAL;
    MN_DUAL_SYS_E               dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    
    if(ATC_CMD_TYPE_EXECUTE != ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if (!MNPHONE_IsPsPowerOnEx(dual_sys))
    {
        rssi        = AT_RXLEV_ZERO;
        rxqual_full = AT_INVALID_RXQUAL;
    }
    else
    {
        // get the RSSI information
        cell_info = MNPHONE_GetRSSIEx(dual_sys);

        rssi = cell_info.rxlev;
        if (!cell_info.is_traffic_mode)
        {
            rxqual_full = AT_INVALID_RXQUAL; 
        }
        else
        {
            rxqual_full = cell_info.rxqual_full; 
        }
    }

    sprintf((char*)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CSQ].cmd_name->str_ptr,
                   (rssi / 2), rxqual_full);
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the ATS0 command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS0 )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS    status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // check the index
            if (!PARAM1_FLAG)
                return ERR_INVALID_INDEX;

            ring_times[dual_sys] = (uint16)PARA1.num;
            break;
        }

            //Test command ATS0?
        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d",g_atc_info_table[AT_CMD_S0].cmd_name->str_ptr, 
                ring_times[dual_sys]);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert S0,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1004_112_2_17_22_16_31_118,(uint8*)"");
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CGMI command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCGMI )
{
#ifndef _ULTRA_LOW_CODE_
    char    *manufactory_str = PNULL;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            // get the manufactory name
            manufactory_str = (char*)CLASSMARK_GetManufacterName();

            sprintf((char*)g_rsp_str, "%s", manufactory_str);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_CMD_TYPE_TEST:
        {
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert CGMI,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1042_112_2_17_22_16_32_119,(uint8*)"");
            break;
        }
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CGSN command 
//  Global resource dependence : g_rsp_str
//  Author:  Xueli Lv
//  Modify:  Winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCGSN )
{
#ifndef _ULTRA_LOW_CODE_
    MN_IMEI_T           imei_arr; // the imei array
    uint8               temp_str[20];
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    // get the manufactory name
    MNNV_GetIMEIEx(dual_sys, imei_arr);        
    ConvertBcdToDigitalStr(MN_MAX_IMEI_LENGTH, imei_arr, temp_str);

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            sprintf((char*)g_rsp_str, "%s",temp_str);    
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
        
        case ATC_CMD_TYPE_TEST:
        {
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert CGSN,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1115_112_2_17_22_16_32_121,(uint8*)"");
            break;
        }
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CGMM command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCGMM )
{
#ifndef _ULTRA_LOW_CODE_
    char                *module_str = PNULL;

    if(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE)
    {
        module_str = (char*)CLASSMARK_GetModelName();

        sprintf((char*)g_rsp_str, "%s", module_str);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        
        return S_ATC_SUCCESS;
    }
    else
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CGMR command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCGMR )
{
#ifndef _ULTRA_LOW_CODE_
    if(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE)
    {
        // get the software version
#ifndef WIN32
        {
            uint8*    cur_str = PNULL;
            uint16    str_len = 0;
            
            sprintf((char*)g_rsp_str, "%s", COMMON_GetProjectVersionInfo());
            
            cur_str = g_rsp_str;
            while (*cur_str != '\0')
            {
                if (*cur_str == '\n')
                {
                    str_len = strlen((char *)g_rsp_str);
                    memmove((void *)(cur_str + 1), (void *)cur_str, strlen((char *)cur_str));
                    g_rsp_str[str_len + 1] = '\0';
                    *cur_str = '\r';
                    cur_str +=1;
                }
                cur_str++;
            }
        }
#endif
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }
    else
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CSCS command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSCS )
{
#ifndef _ULTRA_LOW_CODE_
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            //SCI_TRACE_LOW:"ATC: CSCS TYPE SET"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1207_112_2_17_22_16_32_122,(uint8*)"");
            
            if (!PARAM1_FLAG)
            {
                strcpy((char*)atc_config_ptr->cscs, "GSM");
            }
            else
            {
                if((strcmp((char*)PARA1.str_ptr->str_ptr, "IRA")==0)
                    || (strcmp((char*)PARA1.str_ptr->str_ptr, "UCS2")==0)
                    || (strcmp((char*)PARA1.str_ptr->str_ptr, "GSM")==0)) 
                {   
                    strcpy((char*)atc_config_ptr->cscs, (char*)PARA1.str_ptr->str_ptr);
                }
                else
                {
                    return S_ATC_FAIL;
                }
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str,"%s: \"%s\"",g_atc_info_table[AT_CMD_CSCS].cmd_name->str_ptr, 
                            (char*)atc_config_ptr->cscs);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
            
        case ATC_CMD_TYPE_TEST:
        {
            //SCI_TRACE_LOW:"ATC: CSCS TYPE SET"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1239_112_2_17_22_16_32_123,(uint8*)"");
            sprintf((char*)g_rsp_str,"%s: \"IRA\", \"UCS2\", \"GSM\"",g_atc_info_table[AT_CMD_CSCS].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert CSCS,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1247_112_2_17_22_16_32_124,(uint8*)"");
            break;
        }
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CPIN command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPIN )
{
#ifndef _ULTRA_LOW_CODE_
    MN_BLOCK_T            password;        // the pin
    MN_BLOCK_T            new_password;    // the new pin
    
    ATC_CPWD_PARAM_T      param[3];
    uint8                 *temp_ptr;
    uint8                 param_len;
    uint8                 cmd_type;
    ATC_STATUS            status = S_ATC_SUCCESS; 
    ERR_MNPHONE_CODE_E    err_code = ERR_MNPHONE_NO_ERR;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(param, 0, sizeof(param));
    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        //SCI_TRACE_LOW:"ATC: SPIN param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1279_112_2_17_22_16_32_125,(uint8*)"");
        return ERR_OPERATION_NOT_ALLOWED;
    }
    temp_ptr  = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    ATC_GetCmdType(temp_ptr, param_len, &cmd_type);
    switch (cmd_type)
    {
        // sends a password to ME
        case ATC_CMD_TYPE_SET:
        {
            //SCI_TRACE_LOW("ATC: cpin command is set command");
            temp_ptr++;
            param_len--;
            if (ATC_GetParams(temp_ptr, param_len, param))
            {
                if (param[0].param_len != 0)
                {
                    if (ATC_CheckPinStr(param[0].param, param[0].param_len))
                    {
                        if(param[0].param_len > MN_MAX_BLOCK_LEN)
                        {
                            //SCI_TRACE_LOW:"ATC: Assert Failure pin len error."
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1301_112_2_17_22_16_32_126,(uint8*)"");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                        SCI_MEMCPY(password.blockbuf, param[0].param, param[0].param_len);
                        password.blocklen = param[0].param_len;
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                }
                else
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }
                
                if (param[1].param_len != 0)
                {
                    if (ATC_CheckPinStr(param[1].param, param[1].param_len))
                    {
                        if(param[1].param_len > MN_MAX_BLOCK_LEN)
                        {
                            //SCI_TRACE_LOW:"ATC: Assert Failure pin length error."
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1323_112_2_17_22_16_32_127,(uint8*)"");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                        SCI_MEMCPY(new_password.blockbuf, param[1].param, param[1].param_len);
                        new_password.blocklen = param[1].param_len;
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                }
            }
            else
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            switch (atc_config_ptr->cpin[dual_sys])
            {
                // ME is not pending for any password
                case ATC_CPIN_CODE_READY:
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }
            
                // ME is waiting SIM PIN to be given
                case ATC_CPIN_CODE_PIN1:
                {
                    if (0!=param[0].param_len && 0==param[1].param_len)
                    {
                        err_code = MNPHONE_ResponsePinEx(dual_sys, MNSIM_PIN1, FALSE, &password, PNULL);
                        
                        if(ERR_MNPHONE_NO_ERR == err_code)
                        {
                            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                            status = S_ATC_DEFAULT_HANDLE;
                        }
                        else
                        {
                            //SCI_TRACE_LOW:"ATC: ATC_ProcessCPIN, PIN1,failure in MNPHONE_ResponsePin"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1363_112_2_17_22_16_32_128,(uint8*)"");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    break;
                }

                // ME is waiting SIM PIN2 to be given
                case ATC_CPIN_CODE_PIN2:
                {
                    if (0!=param[0].param_len && 0==param[1].param_len)
                    {
                        if (!atc_config_ptr->sim_flag[dual_sys])
                        {
                            err_code = MNPHONE_ResponsePinEx(dual_sys, MNSIM_PIN2, FALSE, &password, PNULL);
                            
                            if(ERR_MNPHONE_NO_ERR == err_code)
                            {
                                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                                status = S_ATC_DEFAULT_HANDLE;
                            }
                            else
                            {
                                //SCI_TRACE_LOW:"ATC: ATC_ProcessCPIN, PIN2, failure in MNPHONE_ResponsePin"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1390_112_2_17_22_16_32_129,(uint8*)"");
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else
                        {
                            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_VERIFY, MNSIM_PIN2, &password, PNULL);
                            
                            if(err_code == ERR_MNPHONE_NO_ERR)
                            {
                                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                                status = S_ATC_DEFAULT_HANDLE;
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                    break;
                }

                // ME is waiting SIM PUN1 to be given
                case ATC_CPIN_CODE_PUK1:
                {
                    if (0!=param[0].param_len && 0!=param[1].param_len)
                    {
                        if (!atc_config_ptr->sim_flag[dual_sys])
                        {
                            err_code = MNPHONE_ResponsePinEx(dual_sys, MNSIM_PIN1, TRUE, &new_password, &password);
                            
                            if(ERR_MNPHONE_NO_ERR == err_code)
                            {
                                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                                status = S_ATC_DEFAULT_HANDLE;
                            }
                            else
                            {
                                //SCI_TRACE_LOW:"ATC: ATC_ProcessCPIN, sim_flag:TRUE, PUK1, failure in MNPHONE_ResponsePin"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1432_112_2_17_22_16_32_130,(uint8*)"");
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else
                        {
                            if(!s_received_sim_ready[dual_sys])
                            {
                                err_code = MNPHONE_ResponsePinEx(dual_sys, MNSIM_PIN1, TRUE, &new_password, &password);
                                
                                if(ERR_MNPHONE_NO_ERR == err_code)
                                {
                                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                                    status = S_ATC_DEFAULT_HANDLE;
                                }
                                else
                                {
                                    //SCI_TRACE_LOW:"ATC: ATC_ProcessCPIN, sim_flag:TRUE,PUK1, failure in MNPHONE_ResponsePin"
                                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1449_112_2_17_22_16_32_131,(uint8*)"");
                                    return ERR_OPERATION_NOT_ALLOWED;
                                }           
                            }
                            else
                            {
                                err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_UNBLOCK, MNSIM_PIN1,&password, &new_password);
                                
                                if(err_code == ERR_MNPHONE_NO_ERR)
                                {
                                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                                    status = S_ATC_DEFAULT_HANDLE;
                                }
                                else
                                {
                                    return ERR_OPERATION_NOT_SUPPORTED;
                                }
                            }
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                    break;
                }

                // ME is waiting SIM PUK2 to be given
                case ATC_CPIN_CODE_PUK2:
                {
                    if (0!=param[0].param_len && 0!=param[1].param_len)
                    {
                        if (!atc_config_ptr->sim_flag[dual_sys])
                        {
                            err_code = MNPHONE_ResponsePinEx(dual_sys, MNSIM_PIN2, TRUE, &new_password, &password);
                            
                            if(ERR_MNPHONE_NO_ERR == err_code)
                            {
                                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                                status = S_ATC_DEFAULT_HANDLE;
                            }
                            else
                            {
                                //SCI_TRACE_LOW:"ATC: ATC_ProcessCPIN, PUK2, failure in MNPHONE_ResponsePin"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1492_112_2_17_22_16_32_132,(uint8*)"");
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else
                        {
                            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_UNBLOCK, MNSIM_PIN2,&new_password, &password);
                            
                            if(err_code == ERR_MNPHONE_NO_ERR)
                            {
                                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_SIM_PIN_FUNC_CNF,0xff,ATC_NO_DOMAIN);
                                status = S_ATC_DEFAULT_HANDLE;
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                    break;
                }

                // The SIM card is blocked for the user
                case ATC_CPIN_CODE_BLOCK:
                {
                    return ERR_SIM_PUK_REQUIRED;
                }

                default:
                {
                    //SCI_TRACE_LOW:"ATC: Assert Failure unknow cpin state %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1526_112_2_17_22_16_33_133,(uint8*)"d",atc_config_ptr->cpin[dual_sys]);
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            atc_config_ptr->cpin_flag[dual_sys] = TRUE;
            status = S_ATC_DEFAULT_HANDLE;
            break;
        }

        case ATC_CMD_TYPE_READ:
            //SCI_TRACE_LOW("ATC: cpin command is read command");
            if(ATC_CPIN_CODE_MAX <= atc_config_ptr->cpin[dual_sys])
            {
               //SCI_TRACE_LOW:"ATC: Assert Failure cpin state wrong"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1539_112_2_17_22_16_33_134,(uint8*)"");
               return ERR_OPERATION_NOT_ALLOWED;
            }


            if (atc_config_ptr->sim_flag[dual_sys])
            {
                sprintf((char*)g_rsp_str, g_atc_cpin_rsp[atc_config_ptr->cpin[dual_sys]].str);
            }
            else
            {
                sprintf((char*)g_rsp_str, "+CME ERROR: 10");
                status = S_ATC_DEFAULT_HANDLE;
            }
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        case ATC_CMD_TYPE_TEST:
            sprintf((char*)g_rsp_str, "%s: <PIN>", 
                g_atc_info_table[AT_CMD_CPIN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default: 
            //SCI_TRACE_LOW:"ATC: Invalid parameter value been input"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1562_112_2_17_22_16_33_135,(uint8*)"");
            return ERR_OPERATION_NOT_ALLOWED;
    }

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}


/*****************************************************************************/
//  Description : This function handle the AT+XX command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessXX )
{
#ifndef _ULTRA_LOW_CODE_
    uint8               pin_type;
    uint16              remain_times;
    ATC_STATUS          status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    
    if (PARAM1_FLAG)
    {
        if (atc_config_ptr->sim_flag[dual_sys])
        {
            pin_type = (uint8)PARA1.num;
            switch(pin_type)
            {
                case ATC_PIN1:
                {
                    remain_times = MNSIM_GetPinFalseRemainEx(dual_sys, MNSIM_PIN1);
                    break;
                }
            
                case ATC_PIN2:
                {
                    remain_times = MNSIM_GetPinFalseRemainEx(dual_sys, MNSIM_PIN2);
                    break;
                }
            
                case ATC_PUK1:
                {
                    remain_times = MNSIM_GetPinOrPukFalseRemainEx(dual_sys, MNSIM_PIN1, TRUE);
                    break;
                }

                case ATC_PUK2:
                {
                    remain_times = MNSIM_GetPinOrPukFalseRemainEx(dual_sys, MNSIM_PIN2, TRUE);
                    break;
                }

                default:
                {
                    //SCI_TRACE_LOW:"ATC: unknow pin type %d."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1618_112_2_17_22_16_33_136,(uint8*)"d",pin_type);
                    return ERR_INVALID_INDEX;
                }
            }
            sprintf((char*)g_rsp_str, "%s: %d", 
                g_atc_info_table[AT_CMD_XX].cmd_name->str_ptr,remain_times);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            status = ERR_SIM_NOT_INSERTED;
        }
    }
    else
    {
        status = ERR_INVALID_INDEX;
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+SIME command 
//  Global resource dependence : g_rsp_str
//  Author:       Feng.shi
//  Note: This is for hisilicon modem project
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSIME )
{
#ifndef _ULTRA_LOW_CODE_
    if(ATC_CMD_TYPE_READ == ATC_GET_CMD_TYPE)
    {
#ifdef MULTI_SIM_SYS_DUAL
        sprintf((char*)g_rsp_str, "%s: %d,%d", 
                g_atc_info_table[AT_CMD_SIME].cmd_name->str_ptr, atc_config_ptr->sim_flag[0], atc_config_ptr->sim_flag[1]);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
#else
        sprintf((char*)g_rsp_str, "%s: %d", 
                g_atc_info_table[AT_CMD_SIME].cmd_name->str_ptr, atc_config_ptr->sim_flag[0]);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
#endif
    }
    else
        return S_ATC_FAIL;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
/*****************************************************************************/
//  Description : This function handles the AT+CIMI command. This command is used 
//                to get the IMSI of the SIM card.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCIMI )
{
#ifndef _ULTRA_LOW_CODE_
    MN_IMSI_T          imsi_str;
    uint8              temp_str[20];
    ATC_STATUS         status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if (atc_config_ptr->sim_flag[dual_sys])
    {
        // get the IMSI identity
        imsi_str = MNSIM_GetImsiEx(dual_sys);
        
        if(MN_MAX_IMSI_ARR_LEN >= imsi_str.imsi_len)
        {
            ConvertBcdToDigitalStr(imsi_str.imsi_len, imsi_str.imsi_val, temp_str);
            sprintf((char*)g_rsp_str, "%s: %s", g_atc_info_table[AT_CMD_CIMI].cmd_name->str_ptr,temp_str);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            status = S_ATC_FAIL;
        }
    }
    else
    {
        if (s_received_sim_ready[dual_sys])
        {
            switch (s_sim_error_reason[dual_sys])
            {
                case MNSIM_ERROR_REMOVED:
                {
                    status = ERR_SIM_NOT_INSERTED;
                    break;
                }

                case MNSIM_ERROR_SERVICE_NOT_AVAILABLE:
                {
                    status = ERR_SIM_CARD_SERVICE_NOT_AVAILABLE;
                    break;
                }
                
                case MNSIM_ERROR_PIN1_UNINITIALIZED:
                case MNSIM_ERROR_PIN2_UNINITIALIZED:
                {
                    status = ERR_SIM_CARD_PIN_UNINITIALIZED;
                    break;
                }
                    
                case MNSIM_ERROR_PIN1_BLOCKED:
                case MNSIM_ERROR_PIN2_BLOCKED:
                {
                    status = ERR_SIM_CARD_PIN_BLOCKED;
                    break;
                }
                
                case MNSIM_ERROR_PIN1_UNBLOCK_BLOCKED:
                case MNSIM_ERROR_PIN2_UNBLOCK_BLOCKED:
                {
                    status = ERR_SIM_CARD_PUK_BLOCKED;
                    break;
                }
                    
                case MNSIM_ERROR_REJECT_BY_NET:
                {
                    status = ERR_SIM_CARD_REJ_BY_NET;
                    break;
                }
                
                case MNSIM_ERROR_POWER_OFF:    
                case MNSIM_ERROR_NO_SECRETCODE_INITIALIZED:    
                case MNSIM_ERROR_INVALID_STATUS_RSP:
                case MNSIM_ERROR_INSERTED_ERROR:
                case MNSIM_ERROR_INITIALISE_FAILED:    
                case MNSIM_ERROR_MEMORY_PROBLEM:
                case MNSIM_ERROR_INVALID_PARAMS:
                case MNSIM_ERROR_FILE_INVALIDATED:
                case MNSIM_ERROR_FILE_NOT_FOUND:    
                case MNSIM_ERROR_GENERAL_FAULT:
                case MNSIM_ERROR_ALLOC_ERROR:
                case MNSIM_ERROR_SM_FAULT:    
                case MNSIM_ERROR_SERIOUS:
                case MNSIM_ERROR_UNKNOWN:
                case MNSIM_ERROR_NULL:
                {
                    status = ERR_SIM_FAILURE;
                    break;
                }
                    
                default :
                {
                    status = S_ATC_FAIL;
                    break;
                }
            }
        }
        else
        {
            status = ERR_SIM_NOT_INSERTED;
        }    

    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the data of CCID
//  Global resource dependence : g_rsp_str
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
LOCAL void HandleCcidData(
                uint8                *p_ccid,        //the input data 
                uint8                *p_result  //the output data     
                )
{
    uint8    temp_val = 0;
    int32                 i = 0;
    int32                 j = 0;

    for (i=0; i<MNSIM_ICCID_ID_NUM_LEN; i++)
    {
        // get the low 4 bits
        temp_val = (p_ccid[i] & 0x0f);
        if ( 0x0A <= temp_val )
        {
            p_result[j] = (temp_val - 0x0A) + 'A';
        }
        else
        {
            p_result[j] = (temp_val ) + '0' ;
        }
        j++;

       // get the high 4 bits
        temp_val = (p_ccid[i] & 0xf0) >> 4;
        if ( temp_val >= 0x0A )
        {
            p_result[j] = (temp_val - 0x0A) + 'A';
        }
        else
        {
            p_result[j] = (temp_val ) + '0' ;
        }
        j++;
    }
}
#endif

/*****************************************************************************/
//  Description : This function handles the AT+CCID command. This command is used
//  to read the EF-CCID file from SIM card. 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCID )
{
#ifndef _ULTRA_LOW_CODE_
    MNSIM_ICCID_T      ccid;
    uint8              temp_str[2*MNSIM_ICCID_ID_NUM_LEN + 1];
    ATC_STATUS         status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(temp_str, 0 , sizeof(temp_str));
    if (atc_config_ptr->sim_flag[dual_sys])
    {
        if(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE
            ||ATC_CMD_TYPE_READ == ATC_GET_CMD_TYPE)
        {
           MNSIM_GetICCIDEx(dual_sys, &ccid);
           HandleCcidData(ccid.id_num,temp_str);
           sprintf((char*)g_rsp_str, "%s: \"%s\"", 
               g_atc_info_table[AT_CMD_CCID].cmd_name->str_ptr,temp_str);
           ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            status = ERR_OPERATION_NOT_ALLOWED;
        }
    }
    else
    {
        if (s_received_sim_ready[dual_sys])
        {
            switch (s_sim_error_reason[dual_sys])
            {
                case MNSIM_ERROR_REMOVED:
                {
                    status = ERR_SIM_NOT_INSERTED;
                    break;
                }

                case MNSIM_ERROR_SERVICE_NOT_AVAILABLE:
                {
                    status = ERR_SIM_CARD_SERVICE_NOT_AVAILABLE;
                    break;
                }
                
                case MNSIM_ERROR_PIN1_UNINITIALIZED:
                case MNSIM_ERROR_PIN2_UNINITIALIZED:
                {
                    status = ERR_SIM_CARD_PIN_UNINITIALIZED;
                    break;
                }
                    
                case MNSIM_ERROR_PIN1_BLOCKED:
                case MNSIM_ERROR_PIN2_BLOCKED:
                {
                    status = ERR_SIM_CARD_PIN_BLOCKED;
                    break;
                }
                
                case MNSIM_ERROR_PIN1_UNBLOCK_BLOCKED:
                case MNSIM_ERROR_PIN2_UNBLOCK_BLOCKED:
                {
                    status = ERR_SIM_CARD_PUK_BLOCKED;
                    break;
                }
                    
                case MNSIM_ERROR_REJECT_BY_NET:
                {
                    status = ERR_SIM_CARD_REJ_BY_NET;
                    break;
                }
                
                case MNSIM_ERROR_POWER_OFF:    
                case MNSIM_ERROR_NO_SECRETCODE_INITIALIZED:    
                case MNSIM_ERROR_INVALID_STATUS_RSP:
                case MNSIM_ERROR_INSERTED_ERROR:
                case MNSIM_ERROR_INITIALISE_FAILED:    
                case MNSIM_ERROR_MEMORY_PROBLEM:
                case MNSIM_ERROR_INVALID_PARAMS:
                case MNSIM_ERROR_FILE_INVALIDATED:
                case MNSIM_ERROR_FILE_NOT_FOUND:    
                case MNSIM_ERROR_GENERAL_FAULT:
                case MNSIM_ERROR_ALLOC_ERROR:
                case MNSIM_ERROR_SM_FAULT:    
                case MNSIM_ERROR_SERIOUS:
                case MNSIM_ERROR_UNKNOWN:
                case MNSIM_ERROR_NULL:
                {
                    status = ERR_SIM_FAILURE;
                    break;
                }
                    
                default :
                {
                    status = S_ATC_FAIL;
                    break;
                }
            }
        }
        else
        {
            status = ERR_SIM_NOT_INSERTED;
        }    
    }

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
/*****************************************************************************/
//  Description:    This function is used to set voice codec enable/disable.
//  Author:         shujing.dong
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_EnableVoiceCodec ( // If succeed, return SCI_TRUE,
    // else return error value
    BOOLEAN is_enable                   // SCI_TRUE: Enable,   SCI_FALSE: Disable
);
/*****************************************************************************/
//  Description : This function handle the APP_MN_SYNC_IND signal 
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessMnSyncInd(
                    ATC_CONFIG_T            *atc_config_ptr,
                    APP_MN_SYNC_IND_T  *sig_ptr
                    )
{
    //SCI_TRACE_LOW:"ATC: recv APP_MN_SYNC_IND signal"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1953_112_2_17_22_16_33_137,(uint8*)"");

    if(MN_CALL_SYNC_RESOURCE_ASSIGNMENT == (((APP_MN_SYNC_IND_T*)sig_ptr)->sync_cause)
        && (ATC_CALL_OUTGOING_STATE == g_calls_state[((APP_MN_SYNC_IND_T*)sig_ptr)->dual_sys].call_context[0].call_entity_state))
    {
        if (!atc_config_ptr->audio_flag && g_atc_call_flag)
        {
            AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
            AUDIO_SetVolume(atc_config_ptr->speaker_volume);
            ATC_EnableVoiceCodec(TRUE);
            atc_config_ptr->audio_flag = TRUE;
        }
    }
    return S_ATC_SUCCESS;
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handle the APP_MN_NETWORK_STATUS_IND signal and 
//                send the unsolicited result code 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessNetworkStatusInd(
                   ATC_CONFIG_T                  *atc_config_ptr,
                   APP_MN_NETWORK_STATUS_IND_T   *sig_ptr
                   )
{
    OPER_STATUS_E            oper_status;
    
    if (!sig_ptr->plmn_is_selecting)
    {
        // get the oper status
        switch (sig_ptr->plmn_status)
        {
            case PLMN_NO_SERVICE:
            {
            #ifdef _MUX_REDUCED_MODE_
                SCI_MEMSET((void *)g_rxlev_arr,0,sizeof(g_rxlev_arr));
            #endif
                oper_status = NOT_REGISTERED_NOT_SEARCHING;
                
                //Send the unsolicited +SIND: 8        
                if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT7))
                {
                    sprintf((char*)g_rsp_str, "%s: 8",
                        g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
                    ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
                }
                
                break;
            }

            case PLMN_EMERGENCY_ONLY:
            {
                oper_status = REGISTRATION_DENIED;
                
                //Send the unsolicited +SIND: 7        
                if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT6))
                {
                    sprintf((char*)g_rsp_str, "%s: 7",
                        g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
                    ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
                }
                
                g_is_ps_act[sig_ptr->dual_sys] = TRUE;
                
                break;
            }
        
            case PLMN_NORMAL_GSM_ONLY:
            case PLMN_NORMAL_GSM_GPRS_BOTH:
            {
                if (sig_ptr->plmn_is_roaming)
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
                
                //Send the unsolicited +SIND: 11        
                if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT6))
                {
                    sprintf((char*)g_rsp_str, "%s: 11",
                        g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
                    ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
                }
                
                g_is_ps_act[sig_ptr->dual_sys] = TRUE;
                break;
            }
        
            default:
            {
                oper_status = UNKNOWN;
                break;
            }
        }

#ifdef _MUX_ENABLE_
        if(atc_config_ptr->atc_config_nv.creg == ATC_CREG_ENABLE)
        {
            if(g_creg_state[sig_ptr->dual_sys].oper_status == oper_status)
                return S_ATC_SUCCESS;
        }
        else if(atc_config_ptr->atc_config_nv.creg == ATC_CREG_AND_INFO)
        {
            if((g_creg_state[sig_ptr->dual_sys].oper_status) == oper_status && 
               !GPIO_IsAllowedReportRssiToAP())
            {
                //creg_changed = FALSE;
                return S_ATC_SUCCESS;
            }
        }
#endif

        switch (atc_config_ptr->atc_config_nv.creg)
        {
            // disable the +CREG 
            case ATC_CREG_DISABLE:
            {
                break;
            }
                
            case ATC_CREG_ENABLE:
            {
                sprintf((char*)g_rsp_str, "%s: %d", 
                    g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr, oper_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
                
            case ATC_CREG_AND_INFO:
            {
                if ((REGISTERED_ROAMING == oper_status)
                    || (REGISTERED_HOME_PLMN == oper_status))
                {
                    sprintf((char*)g_rsp_str,"%s: %d,0x%04X,0x%04X",
                        g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr,
                        oper_status, sig_ptr->lac, sig_ptr->cell_id);
                }
                else
                {
                    sprintf((char*)g_rsp_str, "%s: %d", 
                        g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr, oper_status);
                } 
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
        
            default:
            {
                //SCI_TRACE_LOW:"ATC: Assert Failure unknow creg."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_2110_112_2_17_22_16_34_138,(uint8*)"");
                return S_ATC_DEFAULT_HANDLE;
            }
        }

#ifdef _MUX_ENABLE_
        if(atc_config_ptr->cgreg == ATC_CREG_ENABLE)
        {
            if(g_creg_state[sig_ptr->dual_sys].oper_status == oper_status)
                return S_ATC_SUCCESS;
        }
        else if(atc_config_ptr->cgreg == ATC_CREG_AND_INFO)
        {
            if((g_creg_state[sig_ptr->dual_sys].oper_status) == oper_status && !GPIO_IsAllowedReportRssiToAP())
            {
                return S_ATC_SUCCESS;
            }
        }
        
        g_creg_state[sig_ptr->dual_sys].oper_status = oper_status;
#endif

#ifdef _SUPPORT_GPRS_
        /* only in GPRS suport status, we can check creg flag          */
        if( sig_ptr->plmn_status & (uint8)PLMN_NORMAL_GPRS_ONLY )
        {
            switch (atc_config_ptr->cgreg)
            {
                // disable the +CGREG 
                case ATC_CREG_DISABLE:
                {
                    break;
                }
            
                case ATC_CREG_ENABLE:
                {
                    sprintf((char*)g_rsp_str, "%s: %d", 
                        g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr, oper_status);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }
                    
                case ATC_CREG_AND_INFO:
                {
                    if ((REGISTERED_ROAMING == oper_status)
                        || (REGISTERED_HOME_PLMN == oper_status))
                    {
                        sprintf((char*)g_rsp_str,"%s: %d,0x%04X,0x%04X",
                            g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr,
                            oper_status, sig_ptr->lac, sig_ptr->cell_id);
                    }
                    else
                    {
                        sprintf((char*)g_rsp_str, "%s: %d", 
                            g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr, oper_status);
                    } 
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }
                    
                default:
                {
                    //SCI_TRACE_LOW:"ATC: Assert Failure unknow plmn status."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_2171_112_2_17_22_16_34_139,(uint8*)"");
                    return S_ATC_DEFAULT_HANDLE;    
                }
            }    
        }
#endif 
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the AT command which has one parameter
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void HandleSingleCmd(
                ATC_CUR_INFO_LIST_T  *cur_cmd_ptr,   // point to the atc cmd info
                ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                uint8                default_val,     // the default value  
                uint8                *cur_val         // the current value
                )
{
    switch (cur_cmd_ptr->cmd_type & 0X0F)
    {
        // Set command AT+XXX=...
        case ATC_CMD_TYPE_SET:
        //@Ivan.Yin for ATV0 etc
        case ATC_CMD_TYPE_EXECUTE:
        {
            if (cur_cmd_ptr->param_used[0])
            {
                *cur_val = (uint8)(cur_cmd_ptr->cur_param_list_ptr->param_info.num);
            }
            else
            {
                *cur_val = default_val;
            }
            break;
        }

            // Read command AT+XXX?
        case ATC_CMD_TYPE_READ:
        {
            // Determine if the command is a extension command
            if ('+' != g_atc_info_table[cur_cmd_ptr->cmd_index].cmd_name->str_ptr[0])
            {
                sprintf((char*)g_rsp_str, "%d", *cur_val);
            }
            else
            {
                sprintf((char*)g_rsp_str, "%s: %d", 
                    g_atc_info_table[cur_cmd_ptr->cmd_index].cmd_name->str_ptr, *cur_val);
            }
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert single command,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_2228_112_2_17_22_16_34_140,(uint8*)"");
            break;
        }
    }
}
#endif

/*****************************************************************************/
//  Description : This function handle the AT command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessAT )
{
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the ATE command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessE )
{
    ATC_STATUS  status = S_ATC_SUCCESS;

    if(ATC_CMD_TYPE_EXECUTE != ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    
    if (PARAM1_FLAG)
    {
        atc_config_ptr->atc_config_nv.e_value = PARA1.num;
        if (0 == atc_config_ptr->atc_config_nv.e_value)
        {
        #ifdef _MUX_ENABLE_
            if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
            {
                SIO_ATC_SetEcho(FALSE);         
            }
            else
            {
                ATC_SetEcho(FALSE);
            }
        #else
            SIO_ATC_SetEcho(FALSE);
        #endif
        }
        else if(1 == atc_config_ptr->atc_config_nv.e_value)
        {
            atc_config_ptr->atc_config_nv.e_value = TRUE;
        #ifdef _MUX_ENABLE_
            if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
            {
                SIO_ATC_SetEcho(FALSE);
            }
            else            
            {
                ATC_SetEcho(TRUE);
            }
        #else
            SIO_ATC_SetEcho(TRUE);
        #endif
        }
        else
        {
            return ERR_OPERATION_NOT_SUPPORTED;
        }
        
    }
    else
    {
        status = ERR_INVALID_INDEX;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the ATZ command
//  ATZ is used to restore the default value configuration and release the current
//  calls  
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessZ )
{
#ifndef _ULTRA_LOW_CODE_
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    uint8         i;
    
    SCI_ASSERT(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE); /*assert verified check command type*/

    if (SCI_IS_TIMER_ACTIVE(P_ATC,ATC_CALL_EXP_IND,&T_ATC_CALL_EXP_IND))
    {
        SCI_KILL_TIMER(P_ATC,ATC_CALL_EXP_IND,
            &T_ATC_CALL_EXP_IND,"T_ATC_CALL_EXP_IND");
        
        g_atc_expire_ind = FALSE;
    }
    
    if( g_atc_call_flag )
    {
        g_atc_call_need_disc = TRUE;
    }
        
    if (g_calls_state[dual_sys].call_amount >= 1)
    {
        atc_config_ptr->ath_flag = TRUE;
        //Release all the call
        for (i=0; i<g_calls_state[dual_sys].call_amount; i++)
        {
#ifdef MS_OAPI_SUPPORT
            MNCALL_DisconnectCallEx(dual_sys, (uint8)g_calls_state[dual_sys].call_context[i].call_id, PNULL);
#else
            MNCALL_DisconnectCallEx(dual_sys, (uint8)g_calls_state[dual_sys].call_context[i].call_id, MN_CAUSE_NORMAL_CLEARING, PNULL);
#endif

#ifdef _MUX_ENABLE_
            //SCI_TRACE_LOW:"atc: mux enable"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_2347_112_2_17_22_16_34_141,(uint8*)"");
            ATC_Clear_All_Exp_Result(g_calls_state[dual_sys].call_context[i].call_id, ATC_DOMAIN_CS);
            ATC_Add_Expected_Event(atc_config_ptr, APP_MN_CALL_DISCONNECTED_IND, 
                            g_calls_state[dual_sys].call_context[i].call_id, ATC_DOMAIN_CS);
#endif

        }
    }
    else 
    {    
        if(g_atc_call_flag)
        {
            status = S_ATC_DEFAULT_HANDLE;
        }
        status = S_ATC_SUCCESS;
    }

    RestoreConfig(atc_config_ptr);

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
/*****************************************************************************/
//  Description : This function handle the AT+CCLK command 
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCLK )
{
#ifndef _ULTRA_LOW_CODE_
    SCI_DATE_T   cur_date;
    SCI_TIME_T    cur_time;
    uint32               status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    return ERR_INVALID_INDEX;
                }

                //SCI_TRACE_LOW:"ATC: CCLK input string is %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_2393_112_2_17_22_16_34_142,(uint8*)"s", PARA1.str_ptr->str_ptr);
                if (CheckTimeStr(PARA1.str_ptr->str_len, PARA1.str_ptr->str_ptr,&cur_date, &cur_time))
                {                
                    if ((ERR_TM_NONE == TM_SetSysDate(cur_date)) && (ERR_TM_NONE == TM_SetSysTime(cur_time)))
                    {
                        status = S_ATC_SUCCESS;
                    }
                    else
                    {
                        status = S_ATC_FAIL;
                    }
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }            
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            TM_GetSysDate(&cur_date);
            TM_GetSysTime(&cur_time);
            sprintf((char*)g_rsp_str, "%s: \"%02d/%02d/%02d,%02d:%02d:%02d\"", 
                g_atc_info_table[AT_CMD_CCLK].cmd_name->str_ptr,
                (cur_date.year % 100),cur_date.mon,cur_date.mday,
                cur_time.hour,cur_time.min,cur_time.sec);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert CCLK,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_2431_112_2_17_22_16_34_143,(uint8*)"");
            break;
        }
    }

    return status;    
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
/*****************************************************************************/
//  Description : This function handle the AT+VGR command 
//                 AT+VGR=<value>
//                        <value>:0 -- 255
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessVGR )
{
    ATC_STATUS  status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
             if (PARAM1_FLAG)
             {
                if(PARA1.num <= AT_AUD_VOL_SPEAKER_MAX)
                {
                    atc_config_ptr->speaker_volume = PARA1.num;
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                    break;
                }
#ifdef AUDIO_PARAM_ARM
                AUDIO_SetVolumeAndTypeEx(
                    INVALID_HANDLE, 
                    atc_config_ptr->speaker_volume, 
                    AUDIO_ARM_APP_TYPE_0);
#else
                AUDIO_SetVolume(atc_config_ptr->speaker_volume);
#endif
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
             
        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d",g_atc_info_table[AT_CMD_VGR].cmd_name->str_ptr,
                            atc_config_ptr->speaker_volume);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
             
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert VGR,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_2492_112_2_17_22_16_34_144,(uint8*)"");
            break;
        }
    }
    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SSEA command 
//                 AT+SVGR=<value>
//                        <value>:0 -- 8
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSEA )
{
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;    

    if(PARAM1_FLAG)
    {
        s_aud_dev_mod = (AUDIO_DEVICE_MODE_TYPE_E)(PARA1.num);
        // output the '>'
        sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
    #ifdef _MUX_ENABLE_
        if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
        {
            SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
            SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
            
        }
        else
        {
            ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
            MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
        }
        atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;
        atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_SSEA;
    #else
        // set the terminate char to receive PDU from the SIO
        SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
        atc_config_ptr->is_ctrlz_end = TRUE;
        atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_SSEA;
    #endif
    }
    else
    {
        status = ERR_INVALID_INDEX;
    }

    return status;

}
/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SSEA
//  Global resource dependence : g_rsp_str
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSSEAData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16            param_len
                )
{
    uint8       terminater_char;     // indicates ther terminater char
    BOOLEAN     flag = FALSE;
    uint32      i;
    AUDIO_TEST_DATA_T audio_data;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len             -= 1;
    SCI_MEMSET(&audio_data,0,sizeof(audio_data));
    //SCI_TRACE_LOW:"ATC:param_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_2565_112_2_17_22_16_35_145,(uint8*)"d",param_len);
    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
        {
            flag = TRUE;
            break;
        }

        case CTRL_Z:
        {
            //Change to bin format.
            if((sizeof(AUDIO_TEST_DATA_T) * 2) == param_len)
            {
                for(i = 0;i < 33;i++)
                {
                    ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.UL_equalizer_coeff[i])));
                    //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.UL_equalizer_coeff[%d]=%x",i,audio_data.UL_equalizer_coeff[i]);    
                    data_ptr = data_ptr + 4;
                }
                for(i = 0;i < 33;i++)
                {
                    ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.DL_equalizer_coeff[i])));
                    //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.DL_equalizer_coeff[%d]=%x",i,audio_data.DL_equalizer_coeff[i]);
                    data_ptr = data_ptr + 4;
                }
                //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.DL_equalizer_coeff=%x,%x,%x",*audio_data.DL_equalizer_coeff,
                //    *(audio_data.DL_equalizer_coeff+1),*(audio_data.DL_equalizer_coeff+2));            
                ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.UL_PGA_gain)));
                //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.UL_PGA_gain=%x",audio_data.UL_PGA_gain);            
                data_ptr = data_ptr + 4;
                ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.DL_PGA_gain)));
                //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.DL_PGA_gain=%x",audio_data.DL_PGA_gain);
                data_ptr = data_ptr + 4;
                ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.UL_digital_gain)));
                //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.UL_digital_gain=%x",audio_data.UL_digital_gain);    
                data_ptr = data_ptr + 4;
                ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.DL_digital_gain)));
                //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.UL_digital_gain=%x",audio_data.UL_digital_gain);    
                data_ptr = data_ptr + 4;
                ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.UL_digital_scale)));
                //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.UL_digital_scale=%x",audio_data.UL_digital_scale);    
                data_ptr = data_ptr + 4;
                ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.DL_digital_scale)));
                //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.DL_digital_scale=%x",audio_data.DL_digital_scale);    
                data_ptr = data_ptr + 4;
                ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.midi_PGA_gain_base)));
                //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.midi_PGA_gain_base=%x",audio_data.midi_PGA_gain_base);    
                data_ptr = data_ptr + 4;
                ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.digital_sidetone_gain)));
                //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.digital_sidetone_gain=%x",audio_data.digital_sidetone_gain);
                data_ptr = data_ptr + 4;
                for(i = 0;i < 6;i++)
                {
                    ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.DL_IIR1_coeff[i])));
                    //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.DL_IIR1_coeff[%d]=%x",i,audio_data.DL_IIR1_coeff[i]);
                    data_ptr = data_ptr + 4;
                }
                for(i = 0;i < 6;i++)
                {
                    ConvertInt16ToBin(data_ptr, (uint16 *)(&(audio_data.DL_IIR2_coeff[i])));
                    //SCI_TRACE_LOW("ATC:AT+SSEA,audio_data.DL_IIR2_coeff[%d]=%x",i,audio_data.DL_IIR2_coeff[i]);
                    data_ptr = data_ptr + 4;
                }
                flag = TRUE;        
                // call the REF api to write set the audio
                AUD_SetEngineeringParameter(s_aud_dev_mod,audio_data);
            }
            break;
        }
            
        default:
        {
            break;
        }
    }

    if (!flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);         
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);        
    }
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handle the AT+CIND command 
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCIND )
{
#ifndef _ULTRA_LOW_CODE_
    uint8                battery_lev;
    uint8                signal_quality;
    uint8                service_flag;
    uint8                sounder;
    uint8                call_flag;
    uint8                roam_flag;
    uint8                sms_full_flag;
    uint32               capacity = 0;
    BOOLEAN              message_flag;
    BOOLEAN              sim_message_flag;
    MN_SMS_RECORD_ID_T   sim_total_amount;
    MN_SMS_RECORD_ID_T   sim_used_amount;
    MN_SMS_RECORD_ID_T   me_total_amount;
    MN_SMS_RECORD_ID_T   me_used_amount;
    BOOLEAN              me_message_flag;
    MN_PHONE_CURRENT_PLMN_INFO_T register_status;
    MN_PHONE_SCELL_MEAS_T rssi;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if (g_calls_state[dual_sys].call_amount > 0)
    {
        call_flag = TRUE;
    }
    else
    {
       call_flag = FALSE;
    }

    sounder = atc_config_ptr->cmut_flag;
    register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
    
    switch (register_status.plmn_status)
    {
        case PLMN_NO_SERVICE:
        case PLMN_EMERGENCY_ONLY:
        {
            service_flag = 0;
            break;
        }
        
        default: 
        {
            service_flag = 1;
            break;
        }
    }
    roam_flag = register_status.plmn_is_roaming;
    
    //get the battery level
    capacity = CHR_GetBatCapacity(); //the return value is 0,20,40,60,80,100
    
    //SCI_TRACE_LOW:"ATC: CIND value %d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_2712_112_2_17_22_16_35_146,(uint8*)"d",capacity);
    battery_lev = capacity / 20;

    //Get the sinal quality
    rssi = MNPHONE_GetRSSIEx(dual_sys);

    signal_quality = rssi.rxqual_full;

    //Get the SMS information
    GetStorageInfo(dual_sys,MN_SMS_STORAGE_SIM, &sim_total_amount, &sim_used_amount, &sim_message_flag);    
    GetStorageInfo(dual_sys,MN_SMS_STORAGE_ME, &me_total_amount, &me_used_amount, &me_message_flag);    

    if ((sim_total_amount == sim_used_amount) && (me_total_amount == me_used_amount) && (sim_total_amount != 0) && (me_total_amount != 0))
    {
       sms_full_flag = TRUE;
    }
    else
    {
       sms_full_flag = FALSE;
    }
    message_flag = sim_message_flag || me_message_flag;

    sprintf((char*)g_rsp_str, "%s: %d,%d,%d,%d,%d,%d,%d,%d",
        g_atc_info_table[AT_CMD_CIND].cmd_name->str_ptr,battery_lev,
        signal_quality,service_flag,sounder,message_flag,
        call_flag,roam_flag,sms_full_flag);
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
/*****************************************************************************/
//  Description : This function handles the data of CCID
//  Global resource dependence : g_rsp_str
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
LOCAL void HandleCcedOneShot(
                             ATC_CONFIG_T   * atc_config_ptr
                             )
{
    MN_PHONE_CURRENT_PLMN_INFO_T   plmn_info;
    MN_PHONE_SCELL_MEAS_T          rssi;
    APP_MN_NCELL_RSSI_CNF_T        ncell;
    uint8                          rxqual;
    int32                          i;
    uint8                          *temp_str;
#ifndef MS_OAPI_SUPPORT
    MN_PHONE_CELL_MEAS_T           scell;
#endif
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

    rssi = MNPHONE_GetRSSIEx(dual_sys);

    switch (atc_config_ptr->cced_type)
    {
        case ATC_CCED_SCELL:
        {
            // check the status of the phone 
            if (PLMN_NO_SERVICE == plmn_info.plmn_status)
            {
                sprintf((char*)g_rsp_str, "%s: ", g_atc_info_table[AT_CMD_CCED].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
#ifdef MS_OAPI_SUPPORT
                if (!rssi.is_traffic_mode)
                {
                    rxqual = 99; // invalid value
                }
                else
                {
                    rxqual = rssi.rxqual_full;
                }
                sprintf((char*)g_rsp_str, "%s: %d,%02d,%d,%d,%d,%d,%d,%d",
                    g_atc_info_table[AT_CMD_CCED].cmd_name->str_ptr,plmn_info.mcc,plmn_info.mnc,
                    plmn_info.lac,plmn_info.cell_id,rssi.bsic,rssi.arfcn,(rssi.rxlev / 2), rxqual);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#else
                MNPHONE_GetMeasReportEx(dual_sys, &ncell, &scell);

                if(scell.cell_present)
                {
                    if (!rssi.is_traffic_mode)
                    {
                        rxqual = 99; // invalid value
                    }
                    else
                    {
                        rxqual = rssi.rxqual_full;
                    }
                    sprintf((char*)g_rsp_str, "%s: %d,%02d,%d,%d,%d,%d,%d,%d",
                        g_atc_info_table[AT_CMD_CCED].cmd_name->str_ptr,plmn_info.mcc,plmn_info.mnc,
                        plmn_info.lac,plmn_info.cell_id,rssi.bsic,rssi.arfcn,(rssi.rxlev / 2), rxqual);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else //sometimes MN is normal but RR is doing power sweep, so return no service at this time
                {
                    sprintf((char*)g_rsp_str, "%s: ", g_atc_info_table[AT_CMD_CCED].cmd_name->str_ptr);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
#endif
            }
            break;
        }

        case ATC_CCED_TIME_ADV:
        {
            sprintf((char*)g_rsp_str, "%s: 0", g_atc_info_table[AT_CMD_CCED].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
            
        case ATC_CCED_NCELL:
        {
#ifdef MS_OAPI_SUPPORT
            MNPHONE_GetMeasReportEx(dual_sys, &ncell);
#else
            MNPHONE_GetMeasReportEx(dual_sys, &ncell, &scell);
#endif

            sprintf((char*)g_rsp_str, "%s: ", g_atc_info_table[AT_CMD_CCED].cmd_name->str_ptr);
            temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];
            for (i=0; i< MN_MAX_NCELL_NUM; i++)
            {
                if (ncell.ncell_power[i].cell_present && ncell.ncell_power[i].identity_present)
                {
                    sprintf((char*)temp_str,"%d,%02d,%d,%d,%d,%d,%d", ncell.ncell_power[i].lai.mcc,
                        ncell.ncell_power[i].lai.mnc, ncell.ncell_power[i].lai.lac, 
                        ncell.ncell_power[i].cell_id,ncell.ncell_power[i].bsic,
                        ncell.ncell_power[i].arfcn,(ncell.ncell_power[i].rxlev / 2));
                    temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];
                    *temp_str = ',';
                    temp_str++;
                    *temp_str = '\0';
                    //SCI_TRACE_LOW("ATC: the string length is %d", strlen((char*)g_rsp_str));
                }
            }
            g_rsp_str[strlen((char*)g_rsp_str) - 1] = '\0';    
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_CCED_CSQ:
        {
            atc_config_ptr->csq_flag = FALSE;
            if (!rssi.is_traffic_mode)
            {
                rxqual = 99; // invalid value
            }
            else
            {
                rxqual = rssi.rxqual_full;
            }
            sprintf((char*)g_rsp_str, "%s: %d, %d", 
                g_atc_info_table[AT_CMD_CCED].cmd_name->str_ptr,
                (rssi.rxlev / 2), rxqual);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            break;
        }
    }
}

/*****************************************************************************/
//  Description : This function handle the AT+CCED command 
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCCED )
{
    uint16                         mode = 0;
     
    if (!PARAM1_FLAG || ATC_CMD_TYPE_SET!=ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    
    mode  = (uint16)(PARA1.num);
    switch (mode)
    {
        case ATC_CCED_ONE_SHOT:
        {
            if (PARAM2_FLAG)
            {
                atc_config_ptr->cced_type = (ATC_CCED_TYPE_E)(PARA2.num);
            }
            HandleCcedOneShot(atc_config_ptr);
            break;
        }

        case ATC_CCED_AUTO_SHOT:
        {
            if (PARAM2_FLAG)
            {
                atc_config_ptr->cced_type = (ATC_CCED_TYPE_E)(PARA2.num);
                if (ATC_CCED_CSQ == atc_config_ptr->cced_type)
                {
                    //SCI_TRACE_LOW:"ATC: Set csq true"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_2916_112_2_17_22_16_35_147,(uint8*)"");
                    atc_config_ptr->csq_flag = TRUE;
                }
            }
            break;
        }

        case ATC_CCED_STOP_AUTO:
        {
            if (PARAM2_FLAG)
            {
                if (ATC_CCED_CSQ == PARA2.num)
                {
                    atc_config_ptr->csq_flag = FALSE;
                }
                else
                {
                    atc_config_ptr->cced_type = ATC_CCED_INVALID;
                }
            }
            else
            {
                atc_config_ptr->cced_type = ATC_CCED_INVALID;
            }
            break;
        }
    
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert CCED,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_2945_112_2_17_22_16_35_148,(uint8*)"");
            break;
        }
    }

    return S_ATC_SUCCESS;
}


/*****************************************************************************/
//  Description : This function handle the AT+CMER command
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMER)
{
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    if(ATC_GET_CMD_TYPE != ATC_CMD_TYPE_SET)
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    if(!PARAM1_FLAG || !PARAM4_FLAG)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(1 != PARA1.num)
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    if(0 == PARA4.num)
    {
        atc_config_ptr->cmer_flag[dual_sys] = FALSE;
    }
    else
    {
        atc_config_ptr->cmer_flag[dual_sys] = TRUE;

        if(atc_config_ptr->rel_former[dual_sys] != 127)
        {
            sprintf((char *)g_rsp_str, "+CIEV: 1,%d", atc_config_ptr->rel_former[dual_sys]);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }

    }

    return S_ATC_SUCCESS;
}


/*****************************************************************************/
//  Description : This function handles the AT+SDTMF command 
//              AT+SDTMF is used to play the DTMF tone on the current speaker
//               AT+SDTMF=<mode>,<dtmf>,<gain>,<duartion>
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSDTMF )
{
#ifndef _ULTRA_LOW_CODE_
    uint8        dtmf_key = 0;
    uint32        duration = AUD_PLAY_FOREVER;
    int32                     i = 0;
    uint8       mode_flag = 0;

    AUD_DTMF_TONE_ID_E  dtmf_tone = AUD_DTMF_One;

    if (PARAM1_FLAG)
    {
        mode_flag = PARA1.num;
        if (!mode_flag)
        {
            // stop play the dtmf tone
            uint32 result;
    
            AUDIO_ExeCodecExtOpe(
                hAtcDtmfRing,
                ANSI2UINT16("EXT_STOP"),
                0,
                PNULL,
                &result
                );
            return S_ATC_SUCCESS;
        }
    }
    if (!PARAM2_FLAG)
    {
        return ERR_INVALID_INDEX;
    }
    dtmf_key = PARA2.str_ptr->str_ptr[0];
    // search the dtmf tone
    for (i=0; i<AUD_DTMF_MAX_ID; i++)
    {
        if (c_wdtmf_data[i].val == dtmf_key)
        {
            dtmf_tone = (AUD_DTMF_TONE_ID_E)(c_wdtmf_data[i].lev);
            break;
        }
    }

    if (PARAM3_FLAG)
    {
        duration = PARA3.num;
        if (0 == duration)
        {
            duration = AUD_PLAY_FOREVER;
        }
        else
        {
            // uint of 100 ms
            duration *= AT_SPEAKER_UNIT_DURATION; 
        }
    }
    {
        DTMF_TONE_ADP_EXT_OPE_PLAY_PARA_T tDtmfPara = {0};
        uint32 result;
        if(INVALID_HANDLE==hAtcDtmfRing)//need to create this audio handle only in the first playing.
        {
            hAtcDtmfRing     = AUDIO_CreateNormalHandle(hDtmfCodec,     SCI_NULL, hLAYER1,     SCI_NULL, AudioDummyNotifyCallback);
            
        }
        
        tDtmfPara.tone_id = dtmf_tone;
        tDtmfPara.duration = duration;
        tDtmfPara.is_uplink = FALSE;
        tDtmfPara.hAudioHandle = hAtcDtmfRing;
        tDtmfPara.volume = 4096;
        AUDIO_ExeCodecExtOpe(
            hAtcDtmfRing,
            ANSI2UINT16("EXT_PLAY"),
            DTMF_TONE_ADP_EXT_OPE_PLAY_PARA_COUNT,
            &tDtmfPara,
            &result
            );
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CRSL command.This command is used
//                to set or get the sound level of the ringer on incoming call.
//  AT+CRSL=<sound level>
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRSL )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS     status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                atc_config_ptr->crsl_value = PARA1.num;
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CRSL].cmd_name->str_ptr, atc_config_ptr->crsl_value);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert CRSL,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3079_112_2_17_22_16_36_149,(uint8*)"");
            break;
        }
    }

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CMUT command.This command is used
//                to mute the micrphone input.
//  AT+CRSL=<sound level>
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMUT )
{
#ifdef _ULTRA_LOW_CODE_
    return S_ATC_FAIL;
#else
    ATC_STATUS           status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG && g_calls_state[dual_sys].call_amount != 0)
            {
                atc_config_ptr->cmut_flag = PARA1.num;
                AUDIO_SetUpLinkMute(atc_config_ptr->cmut_flag);
                
                SCI_SLEEP(50); 
                LAYER1_SetVolume(atc_config_ptr->speaker_volume);
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CMUT].cmd_name->str_ptr, atc_config_ptr->cmut_flag);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert CMUT,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3132_112_2_17_22_16_36_150,(uint8*)"");
            break;
        }
    }

    return status;
#endif /* _ULTRA_LOW_CODE_ */
}
/*****************************************************************************/
//  Description : This function handle the AT+CFUN command 
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCFUN )
{
#ifndef _ULTRA_LOW_CODE_
    BOOLEAN       flag;
    ATC_STATUS    status = ERR_INVALID_INDEX;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if (ATC_GET_CMD_TYPE == ATC_CMD_TYPE_SET) 
    {
        //if parameter2 is 1,reset the modle
        if (PARAM2_FLAG)
        {         
            if(PARA2.num)
            {
               ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
               POWER_Reset();
               status = S_ATC_SUCCESS;  
            }
        }
        if (PARAM1_FLAG)
        {         
            flag = (BOOLEAN)PARA1.num;
            if( flag )
            {
            #ifdef _MUX_ENABLE_ 
                MNPHONE_PowerOnPsEx(dual_sys);      
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_PS_POWER_ON_CNF,0xff,ATC_NO_DOMAIN);
            #else   
            #ifdef MS_OAPI_SUPPORT
                MNPHONE_StartupPsEx(dual_sys);
            #else
                MNPHONE_StartupPsEx(dual_sys,MN_GMMREG_RAT_GPRS);
            #endif
            #endif
                g_is_ps_act[dual_sys] = TRUE;
                return S_ATC_SUCCESS;
            }
            else
            {  
            #ifdef _MUX_ENABLE_     
                MNPHONE_PowerOffPsEx(dual_sys); 
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_PS_POWER_OFF_CNF,0xff,ATC_NO_DOMAIN);
            #else                 
                MNPHONE_DeactivePsEx(dual_sys);
            #endif

                g_is_ps_act[dual_sys]   = FALSE;
                g_is_ps_deactivate[dual_sys] = TRUE;
                return  S_ATC_DEFAULT_HANDLE;
            }                              
        }
    }
    else
    {
         sprintf((char*)g_rsp_str,"%s: %d", 
             g_atc_info_table[AT_CMD_CFUN].cmd_name->str_ptr, atc_config_ptr->cfun_flag);
         ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
         status = S_ATC_SUCCESS;
        
    }

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+STONE command 
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTONE )
{
#ifndef _ULTRA_LOW_CODE_
    BOOLEAN    mode_flag = FALSE;
    uint16                freq1 = 0;
    uint32            duration = 0;

    if ( !PARAM1_FLAG )
    {
        return ERR_INVALID_INDEX;
    }
    
    mode_flag = PARA1.num;
    if (!mode_flag)
    {
        uint32 result;
            AUDIO_ExeCodecExtOpe(
                                            hAtcCustomRing,
                                            ANSI2UINT16("EXT_STOP"),
                                            0,
                                            PNULL,
                                            &result
                                        );
        return S_ATC_SUCCESS;
    }

    // get the frequence 1,2,3
    if ( PARAM2_FLAG )
    {
        freq1 = PARA2.num;
    }
    
    //set the volume

    if ( PARAM3_FLAG )
    {
        duration = PARA3.num;
    }
    
    if (0 == duration)
    {
        duration = AUD_PLAY_FOREVER;
    }
    else
    {
        duration = duration * AT_SPEAKER_UNIT_DURATION;
    }
    // play the generic tone
    {        
        CUSTOM_TONE_ADP_EXT_OPE_PLAY_PARA_T tCustomPara = {0};
        uint32 result = 0;
        if(INVALID_HANDLE==hAtcCustomRing)//need to create this audio handle only in the first playing.
        {
            hAtcCustomRing   = AUDIO_CreateNormalHandle(hCustomCodec,   SCI_NULL, hLAYER1,     SCI_NULL, AudioDummyNotifyCallback);

        }
        
        tCustomPara.freq1 = freq1;
        tCustomPara.freq2 = freq1;
        tCustomPara.freq3 = freq1;
        tCustomPara.duration = duration;
        tCustomPara.hAudioHandle = hAtcCustomRing;
        tCustomPara.volume = 4096;
        AUDIO_ExeCodecExtOpe(
                                            hAtcCustomRing,
                                            ANSI2UINT16("EXT_PLAY"),
                                            CUSTOM_TONE_ADP_EXT_OPE_PLAY_PARA_COUNT,
                                            &tCustomPara,
                                            &result
                                        );               
    }

    return     S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
/*****************************************************************************/
//  Description : This function handle the AT+SSAM command 
//  Global resource dependence : none
//  Author:  Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSAM )
{
    ATC_STATUS           status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                atc_config_ptr->audio_mode = (uint8)PARA1.num;
                #ifdef AUDIO_PARAM_ARM
                AUDIO_SetDevModeEx(INVALID_HANDLE, (AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
                #else
                AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
                #endif
            }    
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
    
        case ATC_CMD_TYPE_READ:
        {
            // get the current sound mode
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SSAM].cmd_name->str_ptr, atc_config_ptr->audio_mode);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert SSAM,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3330_112_2_17_22_16_36_151,(uint8*)"");
            break;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SSAP command 
//  Global resource dependence : none
//  Author:  Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSAP )
{
    ATC_STATUS                             status = S_ATC_SUCCESS;
    uint8                         parameter_type = 0;
    uint8                                   b_ul_flag = 0;
    uint16                                 pga_gain = 0;
    uint16                              digital_gain = 0;
    uint8                                          *ptr = NULL;
    AUDIO_TEST_DATA_T audio_test_data = {0};

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG && PARAM4_FLAG)
            {
                parameter_type = (uint8)PARA1.num;
                b_ul_flag      = (uint8)PARA2.num;
                pga_gain       = (uint16)PARA3.num;
                digital_gain   = (uint16)PARA4.num;
                //SCI_TRACE_LOW:"ATC: SSAP pga_gain= %d, digi_gain= %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3362_112_2_17_22_16_36_152,(uint8*)"dd",pga_gain,digital_gain);
            }    
            else
            {
                return ERR_INVALID_INDEX;
            }
            
            AUD_GetEngineeringParameter((AUDIO_DEVICE_MODE_TYPE_E)parameter_type, (AUDIO_TEST_DATA_T *)&audio_test_data);
            
            if( b_ul_flag ) // set UL gain
            {
                audio_test_data.UL_PGA_gain = pga_gain;
                audio_test_data.UL_digital_gain = digital_gain;
            }
            else // set DL gain
            {
                audio_test_data.DL_PGA_gain = pga_gain;
                audio_test_data.DL_digital_gain = digital_gain;
            }
            
            AUD_SetEngineeringParameter((AUDIO_DEVICE_MODE_TYPE_E)parameter_type, audio_test_data);
            
            // this is a temporary code because of the DSP, or the parameter will not go into effect immediately
            SCI_SLEEP(50); 
            LAYER1_SetVolume(atc_config_ptr->speaker_volume);
            break;
        }
    
        case ATC_CMD_TYPE_READ:
        {
            // get the current sound mode
            ptr = g_rsp_str;
            AUD_GetEngineeringParameter(AUDIO_DEVICE_MODE_HANDHOLD, (AUDIO_TEST_DATA_T *)&audio_test_data);
            sprintf((char*)ptr, "%s: %d,%d,%d,%d,%d\r\n", g_atc_info_table[AT_CMD_SSAP].cmd_name->str_ptr, AUDIO_DEVICE_MODE_HANDHOLD,
                                      audio_test_data.UL_PGA_gain, audio_test_data.UL_digital_gain, 
                                      audio_test_data.DL_PGA_gain, audio_test_data.DL_digital_gain);
            ptr = g_rsp_str + strlen((char*)g_rsp_str);
            
            AUD_GetEngineeringParameter(AUDIO_DEVICE_MODE_HANDFREE, (AUDIO_TEST_DATA_T *)&audio_test_data);
            sprintf((char*)ptr, "%s: %d,%d,%d,%d,%d\r\n", g_atc_info_table[AT_CMD_SSAP].cmd_name->str_ptr, AUDIO_DEVICE_MODE_HANDFREE,
                                      audio_test_data.UL_PGA_gain, audio_test_data.UL_digital_gain, 
                                      audio_test_data.DL_PGA_gain, audio_test_data.DL_digital_gain);
            ptr = g_rsp_str + strlen((char*)g_rsp_str);
            
            AUD_GetEngineeringParameter(AUDIO_DEVICE_MODE_EARPHONE, (AUDIO_TEST_DATA_T *)&audio_test_data);
            sprintf((char*)ptr, "%s: %d,%d,%d,%d,%d", g_atc_info_table[AT_CMD_SSAP].cmd_name->str_ptr, AUDIO_DEVICE_MODE_EARPHONE,
                                      audio_test_data.UL_PGA_gain, audio_test_data.UL_digital_gain, 
                                      audio_test_data.DL_PGA_gain, audio_test_data.DL_digital_gain);
            ptr = g_rsp_str + strlen((char*)g_rsp_str);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert SSAP,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3417_112_2_17_22_16_36_153,(uint8*)"");
            break;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SADMDSP command 
//  Global resource dependence : g_rsp_str
//  Author:  Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSADMDSP )
{
    ATC_STATUS        status = S_ATC_SUCCESS;
    ATC_AUD_MODE_OPT_TYPE_E    type = ATC_GET_AUD_MODE_NUM;
    uint32             mode_index = 0;
    uint32             dev_number = 0;
    AUDIO_NV_DSP_MODE_INFO_T*  audmod_devmod_ptr = NULL;
    #ifdef AUDIO_PARAM_ARM
    AUDIO_NV_DSP_MODE_INFO_T* audmod_dsp_devmod_ptr = PNULL;
    #endif
    uint8* str_ptr = NULL;
        
    if (ATC_GET_CMD_TYPE != ATC_CMD_TYPE_SET)
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    if(PARAM1_FLAG)
    {
        type = (ATC_AUD_MODE_OPT_TYPE_E)PARA1.num;
    }
    else
    {
        return ERR_INVALID_INDEX;
    }

    if(PARAM2_FLAG)
    {
        mode_index = (uint32)PARA2.num;
    }

    //SCI_TRACE_LOW:"ATC: ATC_ProcessSADMDSP"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3460_112_2_17_22_16_36_154,(uint8*)"");

    dev_number = AUDIONVDSP_GetModeNumber();
   
    switch(type)
    {
        case ATC_GET_AUD_MODE_NUM:
        {
            sprintf((char*)g_rsp_str, "%s: %ld", g_atc_info_table[AT_CMD_SADMDSP].cmd_name->str_ptr, dev_number);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
            
        case ATC_GET_AUD_MODE_NAME:
        {               
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)PARA2.num < dev_number))
                {
                    
                    audmod_devmod_ptr = AUDIONVDSP_GetFirstDevMode();
                    //SCI_TRACE_LOW:"ATC: mode_index=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3481_112_2_17_22_16_36_155,(uint8*)"d", mode_index);
                    while(mode_index)
                    {
                        audmod_devmod_ptr =  AUDIONVDSP_GetNextDevMode(audmod_devmod_ptr);
                        mode_index--;
                    }
                    //SCI_TRACE_LOW:"ATC: audmod_devmod_ptr=0x%x, name %s"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3487_112_2_17_22_16_36_156,(uint8*)"ds", audmod_devmod_ptr, audmod_devmod_ptr->ucModeName);
                    sprintf((char*)g_rsp_str, "%s: %d,\"%s\"", 
                            g_atc_info_table[AT_CMD_SADMDSP].cmd_name->str_ptr,    
                            PARA2.num,
                            audmod_devmod_ptr->ucModeName);
                    
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
            
        case ATC_GET_AUD_MODE_DATA:
        {
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)PARA2.num < dev_number))
                {

                    int str_rsp_len = 0;
                    char str_result_tmp[30] = {0};
                    
                    if(mode_index < dev_number)
                    {
                        audmod_devmod_ptr =  AUDIONVDSP_GetFirstDevMode();
                        
                        while(mode_index)
                        {
                            audmod_devmod_ptr =  AUDIONVDSP_GetNextDevMode(audmod_devmod_ptr);
                            
                            mode_index--;
                        }
                        
                        str_ptr = (uint8*)SCI_ALLOC_BASEZ((sizeof(AUDIO_NV_DSP_MODE_STRUCT_T))*2+1);
                        
                        ConvertBinToHex((uint8*)&audmod_devmod_ptr->tAudioNvDspModeStruct, sizeof(AUDIO_NV_DSP_MODE_STRUCT_T), str_ptr);
                        
                        sprintf(str_result_tmp, "%s: %d,\"%s\",", g_atc_info_table[AT_CMD_SADMDSP].cmd_name->str_ptr,    
                                        PARA2.num,
                                        audmod_devmod_ptr->ucModeName);

                        str_rsp_len = strlen(str_result_tmp)+strlen((char*)str_ptr);
                        //SCI_TRACE_LOW:"ATC: str_rsp_len=%d "
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3537_112_2_17_22_16_37_157,(uint8*)"d", str_rsp_len);

                        if(str_rsp_len>= MAX_ATC_RSP_LEN)
                        {
                            char * str_rsp = NULL;

                            str_rsp = (char *)SCI_ALLOC_BASEZ(str_rsp_len +1);
                            
                            sprintf(str_rsp, "%s%s", str_result_tmp, str_ptr);

                            //SCI_TRACE_LOW:"ATC: str_len_rsp=%d "
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3547_112_2_17_22_16_37_158,(uint8*)"d", strlen(str_rsp));
                            
                            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)str_rsp);
                            SCI_FREE(str_rsp);
                        }
                        else
                        {
                            sprintf((char*)g_rsp_str, "%s%s", str_result_tmp, str_ptr);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }
                        
                        SCI_FREE(str_ptr);
                    }
                    else
                    {
                        status = ERR_INVALID_INDEX;
                    }
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
            
        case ATC_SET_AUD_MODE_DATA:
        {
            // input 246 bytes
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)PARA2.num < dev_number))
                {
                    s_atc_admm_index = (uint8)PARA2.num;
                    // output the '>'
                    sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
                #ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                    {
                       // set the terminate char to receive PDU from the SIO
                       SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);                       
                       SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    }
                    else
                    {
                        ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
                        MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                        // set the terminate char to receive PDU from the SIO
                    }
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_SADMDSP;
                #else
                    // set the terminate char to receive PDU from the SIO
                    SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    atc_config_ptr->is_ctrlz_end = TRUE;
                    atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_SADMDSP;
                #endif
                    
                    status = S_ATC_DEFAULT_HANDLE;
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
        
        case ATC_SET_AUD_DEV_MODE:
        {
            // get audio control mode
            uint16 audio_ctrl_mode = 0;
            
            AUDIONVDSP_GetAudioDspCodecInfoFromRam(&audio_ctrl_mode);

            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3630_112_2_17_22_16_37_159,(uint8*)"d", audio_ctrl_mode);
             sprintf((char*)g_rsp_str, "%s: %d", 
                            g_atc_info_table[AT_CMD_SADMDSP].cmd_name->str_ptr,    
                            audio_ctrl_mode);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
        
        case ATC_SET_AUD_MODE_NV_DATA:
        {
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)PARA2.num < dev_number))
                {
                    s_atc_admm_index = (uint8)PARA2.num;
                    // output the '>'
                    sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
     
                #ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                    {
                       // set the terminate char to receive PDU from the SIO
                       SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);                       
                       SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    }
                    else
                    {
                       ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
                       MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                       // set the terminate char to receive PDU from the SIO
                    }
                    
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_SADMNV_DSP;
                #else
                    // set the terminate char to receive PDU from the SIO
                    SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    atc_config_ptr->is_ctrlz_end = TRUE;
                    atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_SADMNV_DSP;
                #endif
                    
                    status = S_ATC_DEFAULT_HANDLE;
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
       #ifdef AUDIO_PARAM_ARM     
        case ATC_GET_AUD_MODE_NV_DATA:
        {
            if ((!PARAM2_FLAG) 
                || (!(PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC && (uint32)PARA2.num < dev_number)))
            {
                status = ERR_INVALID_INDEX;
                break;
            }
            mode_index = (uint32)PARA2.num;

            audmod_dsp_devmod_ptr = SCI_ALLOC_BASEZ(sizeof(AUDIO_NV_DSP_MODE_INFO_T));
            
            audmod_devmod_ptr = AUDIONVDSP_GetFirstDevMode();
                   
            while(mode_index)
            {
                audmod_devmod_ptr =  AUDIONVDSP_GetNextDevMode(audmod_devmod_ptr);
                mode_index--;
            }
            
            //mode_name_ptr = AUDIONVDSP_GetAudioModeName(mode_index);
            if(PNULL == audmod_devmod_ptr)
            {
                //SCI_TRACE_LOW:"ATC: AUDIONVDSP_GetAudioModeName failed mode:%d."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3709_112_2_17_22_16_37_160,(uint8*)"d", mode_index);
                return ERR_NOT_FOUND;
            }       

            if (AUDMOD_NO_ERROR != AUDIONVDSP_ReadModeParamFromFlash((char *)audmod_devmod_ptr->ucModeName, audmod_dsp_devmod_ptr))
            {
                //SCI_TRACE_LOW:"ATC: AUDIONVDSP_ReadModeParamFromFlash failed."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3715_112_2_17_22_16_37_161,(uint8*)"");
                status = ERR_UNKNOWN;
                break;
            }
            str_ptr = (uint8*)SCI_ALLOC_BASEZ(sizeof(AUDIO_NV_DSP_MODE_STRUCT_T)*2+1);
            
            ConvertBinToHex((uint8*)&audmod_dsp_devmod_ptr->tAudioNvDspModeStruct, sizeof(AUDIO_NV_DSP_MODE_STRUCT_T),str_ptr);
            sprintf((char*)g_rsp_str, "%s: %ld,\"%s\",%s", 
                        g_atc_info_table[AT_CMD_SADMDSP].cmd_name->str_ptr,    
                        mode_index,
                        audmod_dsp_devmod_ptr->ucModeName,
                        str_ptr);

            SCI_FREE(str_ptr);
            SCI_FREE(audmod_dsp_devmod_ptr);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
        #endif

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert SADMDSP,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3738_112_2_17_22_16_37_162,(uint8*)"");
            status = ERR_INVALID_INDEX;
            break;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SADM command 
//  Global resource dependence : g_rsp_str
//  Author:  Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSADM )
{
    ATC_STATUS        status = S_ATC_SUCCESS;
    ATC_AUD_MODE_OPT_TYPE_E    type = ATC_GET_AUD_MODE_NUM;
    uint32             mode_index = 0;
    uint32             dev_number = 0;
    AUDMOD_DEVMOD_T*  audmod_devmod_ptr = NULL;
#ifdef AUDIO_PARAM_ARM   
    AUDIO_RESULT_E result;
#else
    AUDMOD_RESULT_E result;
#endif

#ifdef AUDIO_PARAM_ARM
    char *  mode_name_ptr = PNULL;
    AUDIO_NV_ARM_MODE_INFO_T* audmod_arm_devmod_ptr = PNULL;
#endif
    uint8* str_ptr = NULL;
        
    if (ATC_GET_CMD_TYPE != ATC_CMD_TYPE_SET)
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    if(PARAM1_FLAG)
    {
        type = (ATC_AUD_MODE_OPT_TYPE_E)PARA1.num;
    }
    else
    {
        return ERR_INVALID_INDEX;
    }
    
    if(type >= ATC_GET_AUD_MODE_NUM && type <=ATC_GET_AUD_MODE_NV_DATA)
    {
        dev_number = AUDMOD_GetDevModeNumber();
    }
#ifdef AUDIO_PARAM_ARM
    else
    {
        dev_number = AUDIONVARM_GetModeNumber();
    }
#endif/*AUDIO_PARAM_ARM*/
    
    switch(type)
    {
        case ATC_GET_AUD_MODE_NUM:
        {
            sprintf((char*)g_rsp_str, "%s: %ld", g_atc_info_table[AT_CMD_SADM].cmd_name->str_ptr, dev_number);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
            
        case ATC_GET_AUD_MODE_NAME:
        {
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)PARA2.num < dev_number))
                {
                    mode_index = (uint32)PARA2.num;
                    
                    //SCI_TRACE_LOW:"ATC: sadm mode_index = %d, dev_number = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3812_112_2_17_22_16_37_163,(uint8*)"dd", mode_index, dev_number);

                    audmod_devmod_ptr = AUDMOD_GetFirstDevMode();
                   
                    while(mode_index)
                    {
                        audmod_devmod_ptr = AUDMOD_GetNextDevMode(audmod_devmod_ptr);
                        mode_index--;
                    }
                    
                    sprintf((char*)g_rsp_str, "%s: %ld,\"%s\"", 
                            g_atc_info_table[AT_CMD_SADM].cmd_name->str_ptr,    
                            mode_index,
                            audmod_devmod_ptr->ucModeName);
                    
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
            
        case ATC_GET_AUD_MODE_DATA:
        {
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)PARA2.num < dev_number))
                {
                    int str_rsp_len = 0;
                    char str_result_tmp[30] = {0};
                
                    mode_index = (uint32)PARA2.num;
                    
                    //SCI_TRACE_LOW:"ATC: sadm mode_index = %d, dev_number = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3852_112_2_17_22_16_37_164,(uint8*)"dd", mode_index, dev_number);
                    if(mode_index < dev_number)
                    {
                        audmod_devmod_ptr = AUDMOD_GetFirstDevMode();
                        
                        while(mode_index)
                        {
                            audmod_devmod_ptr = AUDMOD_GetNextDevMode(audmod_devmod_ptr);
                            
                            mode_index--;
                        }
                        
                        str_ptr = (uint8*)SCI_ALLOC_BASEZ((sizeof(AUDIO_STRUCT_T)+1)*2);
                        
                        ConvertBinToHex((uint8*)&audmod_devmod_ptr->tAudioStruct, sizeof(AUDIO_STRUCT_T), str_ptr);
                        
                        sprintf(str_result_tmp, "%s: %ld,\"%s\",", g_atc_info_table[AT_CMD_SADM].cmd_name->str_ptr,    
                                        mode_index,
                                        audmod_devmod_ptr->ucModeName);

                        str_rsp_len = strlen(str_result_tmp)+strlen((char*)str_ptr);
                        //SCI_TRACE_LOW:"atc: str_rsp_len=%d "
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3873_112_2_17_22_16_37_165,(uint8*)"d", str_rsp_len);

                        if(str_rsp_len>= MAX_ATC_RSP_LEN)
                        {
                            char * str_rsp = NULL;

                            str_rsp = (char *)SCI_ALLOC_BASEZ(str_rsp_len +1);
                            
                            sprintf(str_rsp, "%s%s", str_result_tmp, str_ptr);

                            //SCI_TRACE_LOW:"atc: str_len_rsp=%d "
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3883_112_2_17_22_16_37_166,(uint8*)"d", strlen(str_rsp));
                            
                            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)str_rsp);
                            SCI_FREE(str_rsp);
                        }
                        else
                        {
                            sprintf((char*)g_rsp_str, "%s%s", str_result_tmp, str_ptr);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }
                        
                        SCI_FREE(str_ptr);
                    }
                    else
                    {
                        status = ERR_INVALID_INDEX;
                    }
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
            
        case ATC_SET_AUD_MODE_DATA:
        {
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)PARA2.num < dev_number))
                {
                    s_atc_admm_index = (uint8)PARA2.num;
                    // output the '>'
                    sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
                #ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                    {
                       // set the terminate char to receive PDU from the SIO
                       SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);                       
                       SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    }
                    else
                    {
                       ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
                       MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                       // set the terminate char to receive PDU from the SIO
                    }
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_SADM;
                #else
                    // set the terminate char to receive PDU from the SIO
                    SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    atc_config_ptr->is_ctrlz_end = TRUE;
                    atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_SADM;
                #endif
                    
                    status = S_ATC_DEFAULT_HANDLE;
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
        
        case ATC_SET_AUD_DEV_MODE:
        {
            if(PARAM2_FLAG)
            {
                if (ATC_CMD_PARAM_TYPE_STRING==PARAM2_TYPE)
                {
                #ifdef AUDIO_PARAM_ARM                
                    AUDIO_DEVICE_MODE_TYPE_E eType = AUDMOD_GetAudioDevModeFromName((char *)(PARA2.str_ptr->str_ptr));

                    if(AUDIO_DEVICE_MODE_MAX == eType)
                    {
                        //SCI_TRACE_LOW:"ATC: set dev mode, can't get valid mode type, mode name:%s."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3970_112_2_17_22_16_37_167,(uint8*)"s", PARA2.str_ptr->str_ptr);
                        status = ERR_INVALID_INDEX;
                    }
                    else
                    {
                        result = AUDIO_SetDevModeEx(INVALID_HANDLE, eType);
                        if (AUDIO_NO_ERROR != result)
                        {
                            //SCI_TRACE_LOW:"ATC: set dev mode result = %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3978_112_2_17_22_16_37_168,(uint8*)"d",result);
                            status = ERR_INVALID_INDEX;
                        }
                    }
                #else
                    result = AUDMOD_SetDevMode(PARA2.str_ptr->str_ptr, PNULL);
                    if (AUDMOD_NO_ERROR != result)
                    {
                        //SCI_TRACE_LOW:"ATC: set dev mode result = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3986_112_2_17_22_16_37_169,(uint8*)"d",result);
                        status = ERR_INVALID_INDEX;
                    }                
                #endif  
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC: set aud dev param error."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_3993_112_2_17_22_16_37_170,(uint8*)"");
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }            
            break;
        }
        
        case ATC_SET_AUD_MODE_NV_DATA:
        {
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)PARA2.num < dev_number))
                {
                    s_atc_admm_index = (uint8)PARA2.num;
                    // output the '>'
                    sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
                #ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                    {
                       // set the terminate char to receive PDU from the SIO
                       SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);                       
                       SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    }
                    else
                    {
                       ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
                       MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                       // set the terminate char to receive PDU from the SIO
                    }
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_SADMNV;
                #else
                    // set the terminate char to receive PDU from the SIO
                    SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    atc_config_ptr->is_ctrlz_end = TRUE;
                    atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_SADMNV;
                #endif
                    
                    status = S_ATC_DEFAULT_HANDLE;
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
            
        case ATC_GET_AUD_MODE_NV_DATA:
        {
#if !defined(MS_OAPI_SUPPORT) || defined(CHIP_VER_6600L6) || defined(CHIP_VER_6610)
            if ((!PARAM2_FLAG) || 
                (!(PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC && (uint32)PARA2.num < dev_number)))
            {
                status = ERR_INVALID_INDEX;
                break;
            }
            mode_index = (uint32)PARA2.num;
            //SCI_TRACE_LOW:"ATC: sadm mode_index = %d, dev_number = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4060_112_2_17_22_16_38_171,(uint8*)"dd", mode_index, dev_number);

            {
                AUDMOD_DEVMOD_T *temp_aud_ptr = NULL;
                
                audmod_devmod_ptr = AUDMOD_GetFirstDevMode();
               

                while(mode_index)
                {
                    audmod_devmod_ptr = AUDMOD_GetNextDevMode(audmod_devmod_ptr);
                    
                    mode_index--;
                }           

                temp_aud_ptr = (AUDMOD_DEVMOD_T*)SCI_ALLOC_BASEZ(sizeof(AUDMOD_DEVMOD_T));

                str_ptr = (uint8*)SCI_ALLOC_BASEZ((sizeof(AUDIO_STRUCT_T)+1)*2);
                
        #ifndef WIN32
                if (AUDMOD_NO_ERROR != PROD_ReadAudioParamFromFlash(audmod_devmod_ptr->ucModeName,temp_aud_ptr))
                {
                    //SCI_TRACE_LOW:"ATC: PROD_ReadAudioParamFromFlash failed."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4082_112_2_17_22_16_38_172,(uint8*)"");
                    status = ERR_UNKNOWN;
                    break;
                }
        #endif

                ConvertBinToHex((uint8*)&temp_aud_ptr->tAudioStruct, sizeof(AUDIO_STRUCT_T),str_ptr);

                sprintf((char*)g_rsp_str, "%s: %ld,\"%s\",%s", 
                    g_atc_info_table[AT_CMD_SADM].cmd_name->str_ptr,    
                    mode_index,
                    temp_aud_ptr->ucModeName,
                    str_ptr);

                SCI_FREE(str_ptr);
                SCI_FREE(temp_aud_ptr);

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
#else
            status = ERR_OPERATION_NOT_SUPPORTED;
#endif
            break;
        }

#ifdef AUDIO_PARAM_ARM
        case ATC_GET_AUD_ARM_MODE_NUM:
        {
            sprintf((char*)g_rsp_str, "%s: %ld", g_atc_info_table[AT_CMD_SADM].cmd_name->str_ptr, dev_number);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_GET_AUD_ARM_MODE_NAME:
        {
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)PARA2.num < dev_number))
                {
                    mode_index = (uint32)PARA2.num;
                    //SCI_TRACE_LOW:"ATC: sadm mode_index = %d, dev_number = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4122_112_2_17_22_16_38_173,(uint8*)"dd", mode_index, dev_number);
                    mode_name_ptr = AUDIONVARM_GetAudioModeName(mode_index);
                    
                    sprintf((char*)g_rsp_str, "%s: %ld,\"%s\"", 
                        g_atc_info_table[AT_CMD_SADM].cmd_name->str_ptr,    
                        mode_index,
                        mode_name_ptr);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_GET_AUD_ARM_MODE_DATA:
        {
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)PARA2.num < dev_number))
                {
                    mode_index = (uint32)PARA2.num;
                    
                    //SCI_TRACE_LOW:"ATC: sadm mode_index = %d, dev_number = %d, %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4151_112_2_17_22_16_38_174,(uint8*)"ddd", mode_index, dev_number, sizeof(AUDIO_NV_ARM_MODE_INFO_T));
                    //SCI_TRACE_LOW:"ATC:atc_config_ptr->q_value=%d, 0x%x "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4152_112_2_17_22_16_38_175,(uint8*)"dd",atc_config_ptr->q_value, atc_config_ptr);
                    if(mode_index < dev_number)
                    {
                        audmod_arm_devmod_ptr = (AUDIO_NV_ARM_MODE_INFO_T*)SCI_ALLOC_BASEZ(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
                        mode_name_ptr = AUDIONVARM_GetAudioModeName(mode_index);
                        if(PNULL == mode_name_ptr)
                        {
                            //SCI_TRACE_LOW:"AGC_Get_Param failed to get mode name, mode:%d."
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4159_112_2_17_22_16_38_176,(uint8*)"d", mode_index);
                            return ERR_NOT_FOUND;
                        }
                        else
                        {
                            //SCI_TRACE_LOW:"ATC:mode_name_ptr: %s, 0x%x, 0x%x"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4164_112_2_17_22_16_38_177,(uint8*)"sdd",mode_name_ptr,audmod_arm_devmod_ptr, atc_config_ptr);
                            if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam((const char *) mode_name_ptr, (AUDIO_NV_ARM_MODE_INFO_T *)(audmod_arm_devmod_ptr)))
                            {    
                                //SCI_TRACE_LOW:"AGC_Get_Param failed to get mode para."
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4167_112_2_17_22_16_38_178,(uint8*)"");
                                return ERR_NOT_FOUND;
                            }
                            str_ptr = (uint8*)SCI_ALLOC_BASEZ((sizeof(AUDIO_NV_ARM_MODE_STRUCT_T)+1)*2);
                            
                            //SCI_TRACE_LOW:"ATC:atc_config_ptr->q_value=%d, 0x%x, 0x%x, 0x%x"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4172_112_2_17_22_16_38_179,(uint8*)"dddd",atc_config_ptr->q_value, audmod_arm_devmod_ptr, atc_config_ptr, str_ptr);
                            ConvertBinToHex((uint8*)&(audmod_arm_devmod_ptr->tAudioNvArmModeStruct), sizeof(AUDIO_NV_ARM_MODE_STRUCT_T), str_ptr);

                            //SCI_TRACE_LOW:"ATC:atc_config_ptr->q_value=%d, 0x%x, 0x%x, 0x%x"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4175_112_2_17_22_16_38_180,(uint8*)"dddd",atc_config_ptr->q_value, audmod_arm_devmod_ptr, atc_config_ptr, str_ptr);
                            sprintf((char*)g_rsp_str, "%s: %ld,\"%s\",%s", 
                                    g_atc_info_table[AT_CMD_SADM].cmd_name->str_ptr,    
                                    mode_index,
                                    audmod_arm_devmod_ptr->ucModeName,
                                    str_ptr);

                            SCI_FREE(audmod_arm_devmod_ptr);
                            SCI_FREE(str_ptr);

                            //SCI_TRACE_LOW:"ATC:atc_config_ptr->q_value=%d, "
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4185_112_2_17_22_16_38_181,(uint8*)"d",atc_config_ptr->q_value);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }
                    }
                    else
                    {
                        status = ERR_INVALID_INDEX;
                    }
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_SET_AUD_ARM_MODE_DATA:
        {
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)PARA2.num < dev_number))
                {
                    s_atc_admm_index = (uint8)PARA2.num;
                    // output the '>'
                    sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
                    
                #ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                    {
                       // set the terminate char to receive PDU from the SIO
                       SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);          
                       SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    }
                    else
                    {
                       ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
                       MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                       // set the terminate char to receive PDU from the SIO
                    }
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_SADM_ARM;
                #else
                    // set the terminate char to receive PDU from the SIO
                    SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    atc_config_ptr->is_ctrlz_end = TRUE;
                    atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_SADM_ARM;
                #endif
                
                    status = S_ATC_DEFAULT_HANDLE;
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_GET_AUD_ARM_MODE_NV_DATA:
        {
            if ((!PARAM2_FLAG) 
                || (!(PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC && (uint32)PARA2.num < dev_number)))
            {
                status = ERR_INVALID_INDEX;
                break;
            }
            mode_index = (uint32)PARA2.num;
            //SCI_TRACE_LOW:"ATC: sadm mode_index = %d, dev_number = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4262_112_2_17_22_16_38_182,(uint8*)"dd", mode_index, dev_number);

            audmod_arm_devmod_ptr = SCI_ALLOC_BASEZ(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
            
            mode_name_ptr = AUDIONVARM_GetAudioModeName(mode_index);
            if(PNULL == mode_name_ptr)
            {
                //SCI_TRACE_LOW:"AGC_Get_Param failed to get mode name, mode:%d."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4269_112_2_17_22_16_38_183,(uint8*)"d", mode_index);
                return ERR_NOT_FOUND;
            }       
            //SCI_TRACE_LOW:"AGC_Get_Param to get mode name, mode:%d, %s."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4272_112_2_17_22_16_38_184,(uint8*)"ds", mode_index, mode_name_ptr);
            if (AUDMOD_NO_ERROR != AUDIONVARM_ReadModeParamFromFlash(mode_name_ptr, audmod_arm_devmod_ptr))
            {
                //SCI_TRACE_LOW:"ATC: AUDIONVARM_ReadModeParamFromFlash failed."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4275_112_2_17_22_16_38_185,(uint8*)"");
                status = ERR_UNKNOWN;
                break;
            }
            str_ptr = (uint8*)SCI_ALLOC_BASEZ((sizeof(AUDIO_NV_ARM_MODE_STRUCT_T)+1)*2);
            
            ConvertBinToHex((uint8*)&audmod_arm_devmod_ptr->tAudioNvArmModeStruct, sizeof(AUDIO_NV_ARM_MODE_STRUCT_T),str_ptr);
            sprintf((char*)g_rsp_str, "%s: %ld,\"%s\",%s", 
                        g_atc_info_table[AT_CMD_SADM].cmd_name->str_ptr,    
                        mode_index,
                        audmod_arm_devmod_ptr->ucModeName,
                        str_ptr);

            SCI_FREE(str_ptr);
            SCI_FREE(audmod_arm_devmod_ptr);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_SET_AUD_ARM_MODE_NV_DATA:
        {
            if(PARAM2_FLAG)
            {
                if ((ATC_CMD_PARAM_TYPE_NUMERIC==PARAM2_TYPE) && ((uint32)(PARA2.num) < dev_number))
                {
                    s_atc_admm_index = (uint8)PARA2.num;
                    // output the '>'
                    sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
                #ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                    {
                       // set the terminate char to receive PDU from the SIO
                       SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);                       
                       SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    }
                    else
                    {
                       ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
                       MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                       // set the terminate char to receive PDU from the SIO
                    }
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_SADMNV_ARM;
                #else
                    // set the terminate char to receive PDU from the SIO
                    SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    atc_config_ptr->is_ctrlz_end = TRUE;
                    atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_SADMNV_ARM;
                #endif
                
                    status = S_ATC_DEFAULT_HANDLE;
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }        
            break;
        }
#else
        case ATC_GET_AUD_ARM_MODE_NUM:
        case ATC_GET_AUD_ARM_MODE_NAME:
        case ATC_GET_AUD_ARM_MODE_DATA:
        case ATC_SET_AUD_ARM_MODE_DATA:
        case ATC_SET_AUD_ARM_MODE_NV_DATA:
        case ATC_GET_AUD_ARM_MODE_NV_DATA:
        {
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
        }
#endif/*AUDIO_PARAM_ARM*/

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert SADM,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4355_112_2_17_22_16_38_186,(uint8*)"");
            break;
        }
    }

    return status;
}

/*****************************************************************************/
// Description : This function handle the AT+SPADLVVE command
// Global resource dependence : g_rsp_str
// Author:  chong.liu
// Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPADLVVE )
{
    ATC_STATUS        status = S_ATC_SUCCESS;
#ifndef WIN32    
    uint8             type = 0;
    uint32            mode_index = 0;
    uint32            dev_number = 0;
    LVVE_CTRL_PARAM_NV_T *lvve_ctrl_param_nv_ptr = PNULL;
    char *  mode_name_ptr = PNULL;
    uint8   *str_ptr = PNULL;

    //SCI_TRACE_LOW:"ATC: ATC_ProcessSPADLVVE"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4379_112_2_17_22_16_38_187,(uint8*)"");

    if (ATC_GET_CMD_TYPE != ATC_CMD_TYPE_SET)
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    if((PARAM1_FLAG ) &&(PARAM1_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
    {
        type = (uint8)PARA1.num;
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: param_1 is wrong"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4392_112_2_17_22_16_38_188,(uint8*)"");
        return ERR_INVALID_INDEX;
    }

    dev_number =LVVE_CTRL_PARAM_GetDevModeCount(); 
    if(type >= ATC_GET_LEVE_MODE_NAME)
    {
        if((PARAM2_FLAG) &&(PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
        {
            mode_index = (uint32)PARA2.num;
            if(mode_index >= dev_number)
            {
                //SCI_TRACE_LOW:"ATC: param_2 is wrong"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4404_112_2_17_22_16_38_189,(uint8*)"");
                return ERR_INVALID_INDEX;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"ATC: param_2 is wrong"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4410_112_2_17_22_16_38_190,(uint8*)"");
            return ERR_INVALID_INDEX;
        }  
    }

    switch(type)
    {
        case ATC_GET_LEVE_MODE_NUM:
        {
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SPADLVVE].cmd_name->str_ptr, dev_number);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_GET_LEVE_MODE_NAME:
        {
            //SCI_TRACE_LOW:"ATC: SADLVVE type = %d,mode_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4426_112_2_17_22_16_38_191,(uint8*)"dd", type, mode_index);

            mode_name_ptr = LVVE_CTRL_PARAM_GetDevModeName(mode_index);
            if(PNULL == mode_name_ptr)
            {
                //SCI_TRACE_LOW:"ATC: LVVE_CTRL_PARAM_GetDevModeName failed to get mode name, mode:%d."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4431_112_2_17_22_16_38_192,(uint8*)"d", mode_index);
                return ERR_NOT_FOUND;
            }
            
            sprintf((char*)g_rsp_str, "%s: %d,\"%s\"",
                        g_atc_info_table[AT_CMD_SPADLVVE].cmd_name->str_ptr,
                        mode_index,
                        mode_name_ptr);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_GET_LEVE_MODE_DATA:
        {
            //SCI_TRACE_LOW:"ATC: SADLVVE type = %d,mode_index = %d, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4446_112_2_17_22_16_38_193,(uint8*)"ddd", type, mode_index,  sizeof(LVVE_CTRL_PARAM_NV_T));

            lvve_ctrl_param_nv_ptr = (LVVE_CTRL_PARAM_NV_T*)SCI_ALLOC(sizeof(LVVE_CTRL_PARAM_NV_T));
            SCI_ASSERT(lvve_ctrl_param_nv_ptr != PNULL);/*assert verified check NULL pointer*/
            SCI_MEMSET(lvve_ctrl_param_nv_ptr, 0, sizeof(LVVE_CTRL_PARAM_NV_T));

            mode_name_ptr = LVVE_CTRL_PARAM_GetDevModeName(mode_index);
            if(PNULL == mode_name_ptr)
            {
                //SCI_TRACE_LOW:"LVVE_CTRL_PARAM_GetDevModeName failed to get mode name, mode:%d."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4455_112_2_17_22_16_38_194,(uint8*)"d", mode_index);
                SCI_FREE(lvve_ctrl_param_nv_ptr);
                return ERR_NOT_FOUND;
            }
            else
            {
                //SCI_TRACE_LOW:"ATC:mode_name_ptr: %s, 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4461_112_2_17_22_16_38_195,(uint8*)"sd", mode_name_ptr, lvve_ctrl_param_nv_ptr);
                if(AUDIO_NV_ARM_NO_ERROR != LVVE_CTRL_PARAM_GetDevModeParam((uint8 *) mode_name_ptr, (LVVE_CTRL_PARAM_NV_T *)(lvve_ctrl_param_nv_ptr)))
                {
                    //SCI_TRACE_LOW:"LVVE_CTRL_PARAM_GetDevModeParam failed to get mode para."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4464_112_2_17_22_16_38_196,(uint8*)"");
                    SCI_FREE(lvve_ctrl_param_nv_ptr);
                    return ERR_NOT_FOUND;
                }
                
                str_ptr = (uint8*)SCI_ALLOC((sizeof(LVVE_CTRL_PARAM_NV_T)+1)*2);
                SCI_ASSERT(str_ptr != NULL);/*assert verified check NULL pointer*/
                SCI_MEMSET(str_ptr, 0, ((sizeof(LVVE_CTRL_PARAM_NV_T)+1)*2));
                ConvertBinToHex((uint8*)lvve_ctrl_param_nv_ptr, sizeof(LVVE_CTRL_PARAM_NV_T), str_ptr);

                sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%s",
                            g_atc_info_table[AT_CMD_SPADLVVE].cmd_name->str_ptr,
                            mode_index,
                            mode_name_ptr,
                            str_ptr);

                SCI_FREE(lvve_ctrl_param_nv_ptr);
                SCI_FREE(str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            break;
        }
        
        case ATC_SET_LEVE_MODE_DATA:
        {
            s_atc_admm_index = (uint8)PARA2.num;
            // output the '>'
            sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
#ifdef _MUX_ENABLE_
            if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
            {
                // set the terminate char to receive PDU from the SIO
                SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
            }
            else
            {
                ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
                MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                // set the terminate char to receive PDU from the SIO
            }
            
            atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;
            atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_LVVE;
#else
            // set the terminate char to receive PDU from the SIO
            SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
            SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
            atc_config_ptr->is_ctrlz_end = TRUE;
            atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_LVVE;
#endif
            status = S_ATC_DEFAULT_HANDLE;

            break;
        }

        case ATC_GET_LEVE_MODE_NV_DATA:
        {
            //SCI_TRACE_LOW:"ATC: LVVE type = %d, mode_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4522_112_2_17_22_16_39_197,(uint8*)"dd", type, mode_index);

            lvve_ctrl_param_nv_ptr = SCI_ALLOC(sizeof(LVVE_CTRL_PARAM_NV_T));
            SCI_ASSERT(lvve_ctrl_param_nv_ptr != NULL );/*assert verified check NULL pointer*/
            SCI_MEMSET(lvve_ctrl_param_nv_ptr, 0x0, sizeof(LVVE_CTRL_PARAM_NV_T));

            mode_name_ptr = LVVE_CTRL_PARAM_GetDevModeName(mode_index);
            if(PNULL == mode_name_ptr)
            {
                //SCI_TRACE_LOW:"ATC: LVVE_CTRL_PARAM_GetDevModeName failed to get mode name, mode:%d."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4531_112_2_17_22_16_39_198,(uint8*)"d", mode_index);
                SCI_FREE(lvve_ctrl_param_nv_ptr);
                return ERR_NOT_FOUND;
            }
            
            //SCI_TRACE_LOW:"ATC: AGC_Get_Param to get mode name, mode:%d, %s."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4536_112_2_17_22_16_39_199,(uint8*)"ds", mode_index, mode_name_ptr);
            if (AUDMOD_NO_ERROR != LVVE_CTRL_PARAM_ReadModeParamFromFlash((uint8 *)mode_name_ptr, lvve_ctrl_param_nv_ptr))
            {
                //SCI_TRACE_LOW:"ATC: LVVE_CTRL_PARAM_ReadModeParamFromFlash failed."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4539_112_2_17_22_16_39_200,(uint8*)"");
                SCI_FREE(lvve_ctrl_param_nv_ptr);
                status = ERR_UNKNOWN;
                break;
            }
            str_ptr = (uint8*)SCI_ALLOC((sizeof(LVVE_CTRL_PARAM_NV_T)+1)*2);
            SCI_ASSERT(str_ptr != NULL);/*assert verified check NULL pointer*/
            SCI_MEMSET(str_ptr, 0, ((sizeof(LVVE_CTRL_PARAM_NV_T)+1)*2));
            ConvertBinToHex((uint8*)lvve_ctrl_param_nv_ptr, sizeof(LVVE_CTRL_PARAM_NV_T),str_ptr);
            sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%s",
                        g_atc_info_table[AT_CMD_SPADLVVE].cmd_name->str_ptr,
                        mode_index,
                        mode_name_ptr,
                        str_ptr);

            SCI_FREE(str_ptr);
            SCI_FREE(lvve_ctrl_param_nv_ptr);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_SET_LEVE_MODE_NV_DATA:
        {
            s_atc_admm_index = (uint8)PARA2.num;
            // output the '>'
            sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
#ifdef _MUX_ENABLE_
            if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
            {
                // set the terminate char to receive PDU from the SIO
                SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
            }
            else
            {
                ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
                MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                // set the terminate char to receive PDU from the SIO
            }
            atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;
            atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_LVVENV;
#else
            // set the terminate char to receive PDU from the SIO
            SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
            SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
            atc_config_ptr->is_ctrlz_end = TRUE;
            atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_LVVENV;

#endif
            status = S_ATC_DEFAULT_HANDLE;
            break;
        }
        
        default:
        {
            SCI_ASSERT(FALSE);/*assert verified check NULL pointer*/
            break;
        }
    }
#endif
    return status;
}



/*****************************************************************************/
//  Description : This function handle the AT+SPLO command£¬Preferred operator
//  list 
//  Global resource dependence : g_rsp_str
//  Author:  Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPLO )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS        status = S_ATC_SUCCESS;
    uint8             index = 0;
    MN_PHONE_FPLMN_LIST_T    fplmn_list;
    MN_PLMN_T         plmn;
    uint8*               ptr;
    uint32                  i = 0;

    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    
    SCI_MEMSET(&plmn, 0, sizeof(plmn));
    SCI_MEMSET(&fplmn_list, 0, sizeof(fplmn_list));
    
    if( MNPHONE_GetFplmnListEx(dual_sys,&fplmn_list) != ERR_MNPHONE_NO_ERR)
    {
        //SCI_TRACE_LOW:"ATC: SPLO get fplmn fail!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4627_112_2_17_22_16_39_201,(uint8*)"");
            
        return ERR_UNKNOWN;
    }
    
    //SCI_TRACE_LOW:"ATC: SPLO get fplmn success!\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4632_112_2_17_22_16_39_202,(uint8*)"");
    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if( PARAM1_FLAG )
            {
                index = (uint8)PARA1.num;
            }
            else
            {
                index = 0;
            }
            
            if(MN_PHONE_FPLMN_LIST_NUM < index)//<index> is not the right value, show error
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            
            //SCI_TRACE_LOW:"ATC: SPLO index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4652_112_2_17_22_16_39_203,(uint8*)"d",index);
            
            if(PARAM3_FLAG)
            {
               
                if(ATC_CMD_PARAM_TYPE_NUMERIC == PARAM3_TYPE)
                {
                    //SCI_TRACE_LOW:"ATC: SPLO plmn = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4659_112_2_17_22_16_39_204,(uint8*)"d",PARA3.num);
                    
                    plmn.mnc_digit_num = (uint16)(((uint16)PARA3.num) % 10);
                    plmn.mnc = (uint16)(((uint16)(PARA3.num / 10)) % 1000);
                    plmn.mcc = (uint16)(PARA3.num / 10000);
                    
                    if((plmn.mnc_digit_num != 0x02)&&(plmn.mnc_digit_num != 0x03))
                    {
                        //SCI_TRACE_LOW:"+CMS ERROR: mnc_digit_num can only be 2 or 3\n"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4667_112_2_17_22_16_39_205,(uint8*)"");
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                }
                else//(ATC_CMD_PARAM_TYPE_STRING == PARAM3_TYPE)
                {
                    //SCI_TRACE_LOW:"+CMS ERROR: cannot input string in para3\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4673_112_2_17_22_16_39_206,(uint8*)"");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                if(0 == index)//If<oper> is given but <index> is left out, <oper> is put in the next free location.
                {
                    if(fplmn_list.plmn_num < MN_PHONE_FPLMN_LIST_NUM)
                    {
                        fplmn_list.plmn_arr[fplmn_list.plmn_num].mcc = plmn.mcc;
                        fplmn_list.plmn_arr[fplmn_list.plmn_num].mnc = plmn.mnc;
                        fplmn_list.plmn_arr[fplmn_list.plmn_num].mnc_digit_num = plmn.mnc_digit_num;
                        fplmn_list.plmn_num ++;
                    
                    }
                    else if(MN_PHONE_FPLMN_LIST_NUM == fplmn_list.plmn_num)
                    {
                        for(i=0;i<MN_PHONE_FPLMN_LIST_NUM - 1;i++)
                        {
                            fplmn_list.plmn_arr[i].mcc = fplmn_list.plmn_arr[i + 1].mcc;
                            fplmn_list.plmn_arr[i].mnc = fplmn_list.plmn_arr[i + 1].mnc;
                            fplmn_list.plmn_arr[i].mnc_digit_num = fplmn_list.plmn_arr[i + 1].mnc_digit_num;
                        }
                        
                        fplmn_list.plmn_arr[MN_PHONE_FPLMN_LIST_NUM - 1].mcc = plmn.mcc;
                        fplmn_list.plmn_arr[MN_PHONE_FPLMN_LIST_NUM - 1].mnc = plmn.mnc;
                        fplmn_list.plmn_arr[MN_PHONE_FPLMN_LIST_NUM - 1].mnc_digit_num = plmn.mnc_digit_num;
                    }
                    
                    if( MNPHONE_SetFplmnListEx(dual_sys,&fplmn_list) != ERR_MNPHONE_NO_ERR)
                    {
                        //SCI_TRACE_LOW:"+CMS ERROR: set FPLMN list fail\n"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4703_112_2_17_22_16_39_207,(uint8*)"");
                        return ERR_UNKNOWN;
                    }
                
                }
                else if((index >= 1)&&(index <= fplmn_list.plmn_num))//update the <oper> in the <index> position
                {
                    fplmn_list.plmn_arr[index - 1].mcc = plmn.mcc;
                    fplmn_list.plmn_arr[index - 1].mnc = plmn.mnc;
                    fplmn_list.plmn_arr[index - 1].mnc_digit_num = plmn.mnc_digit_num;
                    
                    if( MNPHONE_SetFplmnListEx(dual_sys,&fplmn_list) != ERR_MNPHONE_NO_ERR)
                    {
                        //SCI_TRACE_LOW:"+CMS ERROR: set FPLMN list fail\n"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4716_112_2_17_22_16_39_208,(uint8*)"");
                        return ERR_UNKNOWN;
                    }
                }
                else
                {
                    return ERR_UNKNOWN;
                }
            }
            else
            {
                if((index >= 1)&&(index <= fplmn_list.plmn_num))//If <index> is given but <oper> is left out, entry is deleted
                {
                
                    fplmn_list.plmn_arr[index - 1].mcc = (uint16)0xFFFF;
                    fplmn_list.plmn_arr[index - 1].mnc = (uint16)0xFFFF;
                    fplmn_list.plmn_num--;
                    
                    for(i=index - 1;i<MN_PHONE_FPLMN_LIST_NUM - 1;i++)
                    {
                    
                        fplmn_list.plmn_arr[i].mcc = fplmn_list.plmn_arr[i + 1].mcc;
                        fplmn_list.plmn_arr[i].mnc = fplmn_list.plmn_arr[i + 1].mnc;
                        fplmn_list.plmn_arr[i].mnc_digit_num = fplmn_list.plmn_arr[i + 1].mnc_digit_num;
                    }
                    
                    if( MNPHONE_SetFplmnListEx(dual_sys,&fplmn_list) != ERR_MNPHONE_NO_ERR)
                    {
                        //SCI_TRACE_LOW:"+CMS ERROR: set FPLMN list fail\n"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4744_112_2_17_22_16_39_209,(uint8*)"");
                        return ERR_UNKNOWN;
                    }

                }
                else//error <index> and error <oper>, show error
                {
                    return ERR_UNKNOWN;
                }
            }
            break;
        }
    
        case ATC_CMD_TYPE_READ:
        {
            // list the current fplmn list
            ptr = g_rsp_str;
            
            for(i=0; i< fplmn_list.plmn_num; i++)
            {
                    sprintf((char*)ptr, "%s: index %d, mcc is %d, mnc is %d, mnc_digit_num is %d\r\n",g_atc_info_table[AT_CMD_SPLO].cmd_name->str_ptr,i+1,fplmn_list.plmn_arr[i].mcc,fplmn_list.plmn_arr[i].mnc,fplmn_list.plmn_arr[i].mnc_digit_num);
                    ptr = g_rsp_str + strlen((char*)g_rsp_str);
            }
            
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert SPLO,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4774_112_2_17_22_16_39_210,(uint8*)"");
            break;
        }
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*******************************************************************
 DESCRIPTION       : This function handles the validated AT+CVHU
                     command,and if required, updates the CS bearer
                     database parameters.
 ******************************************************************/
AT_CMD_FUNC( ATC_ProcessCVHU)
{
    SCI_TRACE_LOW("ATC: ATC_ProcessCVHU");

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if( PARAM1_FLAG )
            {
                atc_config_ptr->voice_hangup_mode = (ATC_CVHU_MODE_ENUM)PARA1.num;
            }
            else
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d", "+CVHU", atc_config_ptr->voice_hangup_mode);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
            break;
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SADM
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSADMData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                )
{
    uint8             terminater_char;     // indicates ther terminater char
    BOOLEAN           flag = FALSE;
    uint32            dev_number = 0;
    AUDMOD_DEVMOD_T   *audio_data_ptr;
    AUDMOD_DEVMOD_T*  audmod_devmod_ptr = NULL;
    AUDMOD_RESULT_E   result;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len             -= 1;
    
    audio_data_ptr = (AUDMOD_DEVMOD_T *)SCI_ALLOC_BASEZ(sizeof(AUDMOD_DEVMOD_T)+1);
    
    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
        {
            flag = TRUE;
            break;
        }

        case CTRL_Z:
        {
            if(param_len == sizeof(AUDIO_STRUCT_T)*2)
            {
                dev_number = AUDMOD_GetDevModeNumber();
                if(s_atc_admm_index < dev_number)
                {
                    flag = TRUE;
                    audmod_devmod_ptr = AUDMOD_GetFirstDevMode();
                    
                    while(s_atc_admm_index)
                    {
                        audmod_devmod_ptr = AUDMOD_GetNextDevMode(audmod_devmod_ptr);
                        
                        s_atc_admm_index--;
                    }
                    
                    ConvertHexToBin(data_ptr, param_len, (uint8*)&audio_data_ptr->tAudioStruct);
                #ifdef AUDIO_PARAM_ARM
                    SCI_MEMCPY(audio_data_ptr->ucModeName, audmod_devmod_ptr->ucModeName, AUDMOD_NAME_MAX_LEN);
                #else
                    SCI_MEMCPY(audio_data_ptr->ucModeName, audmod_devmod_ptr->ucModeName, NAME_MAX_LEN);
                #endif
                    
                    result = AUDMOD_SetModeParam(audmod_devmod_ptr->ucModeName, audio_data_ptr);
                    
                    if(result != AUDMOD_NO_ERROR)
                    {
                        //SCI_TRACE_LOW:"ATC: siosadm setmode param result = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4848_112_2_17_22_16_39_211,(uint8*)"d",result);
                        flag = FALSE;
                    }
                }
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: siosadm input data length error."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4855_112_2_17_22_16_39_212,(uint8*)"");
            }
            break;
        }
    
        default:
        {
            break;
        }
    }
    
    SCI_FREE(audio_data_ptr);
    
    if (flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);             
    }
    
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SADM
//  Global resource dependence : g_rsp_str
//  Author:king.li
//  Note: new added for the function to read&write audio parameters into NV
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSADMNVData(               
                      ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                )
{
    uint8        terminater_char;     // indicates ther terminater char
    uint16       data_length;
#ifdef MS_OAPI_SUPPORT
    NVITEM_ERROR_E  nv_err = NVERR_NONE;
    uint16        nv_item_id, nv_item_length;
    uint8         *buf_ptr;
    uint32        dev_number = 0;
#endif
    BOOLEAN           flag = FALSE;
#ifndef MS_OAPI_SUPPORT
    uint32            mode_index = 0;
#endif
    AUDMOD_DEVMOD_T   *audio_data_ptr = NULL;
#ifndef MS_OAPI_SUPPORT
    AUDMOD_DEVMOD_T   *audio_temp_ptr = NULL;
    AUDMOD_RESULT_E   result;
#endif
    
    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != data_ptr); /*assert verified*/    
    
    param_len             -= 1;
    data_length = strlen((char*)data_ptr);
    terminater_char = data_ptr[data_length - 1];
#ifdef MS_OAPI_SUPPORT
    nv_item_id =  (uint16)NV_AUDIO;
    nv_item_length = NVITEM_GetLength(nv_item_id);
       
    buf_ptr = (uint8*)SCI_ALLOC_BASEZ(nv_item_length*2 + 1);
    
    nv_err = EFS_NvitemRead(nv_item_id, nv_item_length, (uint8*)buf_ptr);
    if(NVERR_NONE == nv_err)
    {
        //SCI_TRACE_LOW("ATC: snvm audio data successful");
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: snvm audio data fail, err = %d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4927_112_2_17_22_16_39_213,(uint8*)"d",nv_err);
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);            
    }
    
    audio_data_ptr = (AUDMOD_DEVMOD_T *)SCI_ALLOC_BASEZ(sizeof(AUDMOD_DEVMOD_T)+1);
    
#else
    mode_index = s_atc_admm_index;
    //SCI_TRACE_LOW:"ATC:SioSADMNVData,length:%d,terminater_char:%d,mode index:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4936_112_2_17_22_16_39_214,(uint8*)"ddd",param_len,terminater_char,mode_index);
#endif  
    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
        {
            flag = TRUE;
            //SCI_TRACE_LOW:"ATC:cancel the snvm audio data!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4944_112_2_17_22_16_39_215,(uint8*)"");
            break;
        }

        case CTRL_Z:
        {
            if(param_len == sizeof(AUDIO_STRUCT_T)*2)
            {
#ifdef MS_OAPI_SUPPORT
                ConvertHexToBin(data_ptr, param_len, (uint8*)&audio_data_ptr->tAudioStruct);
                
                dev_number = AUDMOD_GetDevModeNumber();
                if(s_atc_admm_index < dev_number)
                {
                    flag = TRUE;
                    #ifdef AUDIO_PARAM_ARM
                    SCI_MEMCPY(buf_ptr+s_atc_admm_index*sizeof(AUDMOD_DEVMOD_T)+AUDMOD_NAME_MAX_LEN, (uint8*)&(audio_data_ptr->tAudioStruct), sizeof(AUDIO_STRUCT_T));
                    #else
                    SCI_MEMCPY(buf_ptr+s_atc_admm_index*sizeof(AUDMOD_DEVMOD_T)+NAME_MAX_LEN, (uint8*)&(audio_data_ptr->tAudioStruct), sizeof(AUDIO_STRUCT_T));
                    #endif
                    flag = NVITEM_UpdateNvParam_Protect(nv_item_id, nv_item_length, buf_ptr);
                    if(flag)
                    {
                        //SCI_TRACE_LOW:"ATC: write snvm audio data successful"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4967_112_2_17_22_16_39_216,(uint8*)"");
                        //ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"ATC: write snvm audio data fail, err = %d."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4972_112_2_17_22_16_39_217,(uint8*)"d",nv_err);
                        //ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
                    }
                }
#else
                /*get audio name according to audio mode index*/
                audio_temp_ptr = AUDMOD_GetFirstDevMode();
                
                while(mode_index)
                {
                    audio_temp_ptr = AUDMOD_GetNextDevMode(audio_temp_ptr);

                    mode_index--;
                }
                /*get audio data*/
                audio_data_ptr = (AUDMOD_DEVMOD_T *)SCI_ALLOC_BASEZ(sizeof(AUDMOD_DEVMOD_T)+1);
                
                //SCI_TRACE_LOW("ATC:process the command!");
                if (!ConvertHexToBin(data_ptr, param_len, (uint8*)&audio_data_ptr->tAudioStruct))
                {
                    //SCI_TRACE_LOW:"ATC:ConvertHexToBin failed."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4992_112_2_17_22_16_40_218,(uint8*)"");
                    break;
                }
            #ifdef AUDIO_PARAM_ARM
                SCI_MEMCPY(audio_data_ptr->ucModeName,audio_temp_ptr->ucModeName,AUDMOD_NAME_MAX_LEN);
            #else
                SCI_MEMCPY(audio_data_ptr->ucModeName,audio_temp_ptr->ucModeName,NAME_MAX_LEN);
            #endif
                /*write audio param data to flash*/
            #if  !defined(WIN32)
                result = PROD_WriteAudioParamToFlash(audio_temp_ptr->ucModeName,audio_data_ptr);
                if (AUDMOD_NO_ERROR != result)
                {
                    //SCI_TRACE_LOW:"ATC:PROD_WriteAudioParamToFlash failed. result:%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5005_112_2_17_22_16_40_219,(uint8*)"d",result);
                    break;
                }
            #else
                result = AUDMOD_NO_ERROR;
            #endif
                flag = TRUE;                    
#endif
            }
            else
            {
                //SCI_TRACE_LOW:"ATC:SioSADMNVData len error.length:%d != %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5016_112_2_17_22_16_40_220,(uint8*)"dd", param_len,sizeof(AUDIO_STRUCT_T)*2);
            }
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: snvm audio data format is error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5023_112_2_17_22_16_40_221,(uint8*)"");
            break;
        }
    }
    
#ifdef MS_OAPI_SUPPORT
    SCI_FREE(audio_data_ptr);
    SCI_FREE(buf_ptr);
#else    
    if (NULL != audio_data_ptr)
    {
        SCI_FREE(audio_data_ptr);
    }
#endif

    if (flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);             
    }
    
    return S_ATC_DEFAULT_HANDLE;
}


/*****************************************************************************/
//Description : This function handles the data send from the SIO when AT+SPADLVVE
//Global resource dependence : g_rsp_str
//  Author: chong.liu
//Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSADLVVEData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                )
{
#ifndef WIN32
    uint8             terminater_char;     // indicates ther terminater char
    BOOLEAN           flag = FALSE;
    AUDMOD_RESULT_E   result;

    LVVE_CTRL_PARAM_NV_T *lvve_ctrl_param_nv_ptr = PNULL;
    uint8 *name_ptr = PNULL;

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified check NULL pointer*/
    SCI_ASSERT(PNULL != data_ptr);/*assert verified check NULL pointer*/

    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len             -= 1;

    lvve_ctrl_param_nv_ptr = (LVVE_CTRL_PARAM_NV_T *)SCI_ALLOC(sizeof(LVVE_CTRL_PARAM_NV_T)+1);
    SCI_ASSERT(NULL != lvve_ctrl_param_nv_ptr);/*assert verified check NULL pointer*/
    SCI_MEMSET((uint8*)lvve_ctrl_param_nv_ptr, 0, (sizeof(LVVE_CTRL_PARAM_NV_T)+1));

    //SCI_TRACE_LOW:"ATC: siosadm for arm, param_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5081_112_2_17_22_16_40_222,(uint8*)"d",param_len);

    switch (terminater_char)
    {
        case ESC:
        {
            flag = TRUE;
            break;
        }

        case CTRL_Z:
        {
            uint8 i = 0;
            //SCI_TRACE_LOW:"atc:ctrl_z sizeof(LVVE_CTRL_PARAM_NV_T)*2=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5094_112_2_17_22_16_40_223,(uint8*)"d", sizeof(LVVE_CTRL_PARAM_NV_T)*2);
            if(param_len == sizeof(LVVE_CTRL_PARAM_NV_T)*2)
            {
                name_ptr =(uint8 *)LVVE_CTRL_PARAM_GetDevModeName((AUDIO_DEVICE_MODE_TYPE_E)s_atc_admm_index);
                //SCI_TRACE_LOW:"ATC:ATC_ProcessSioSADLVVEData %s, %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5098_112_2_17_22_16_40_224,(uint8*)"sd", name_ptr, strlen((char *)name_ptr));
                
                SCI_ASSERT(name_ptr != NULL);/*assert verified check NULL pointer*/

                ConvertHexToBin(data_ptr, param_len, (uint8*)lvve_ctrl_param_nv_ptr);

                lvve_ctrl_param_nv_ptr->ucModeName[strlen((char *)name_ptr)] = '\0';
                
                //SCI_TRACE_LOW:"atc: lvve_ctrl_param_nv_ptr->name len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5106_112_2_17_22_16_40_225,(uint8*)"d",strlen((char *)(lvve_ctrl_param_nv_ptr->ucModeName)));
                for(i=0; i<strlen((char *)(lvve_ctrl_param_nv_ptr->ucModeName)); i++)
                {
                    //SCI_TRACE_LOW:"atc: ->name [%d]=%d "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5109_112_2_17_22_16_40_226,(uint8*)"dd", i,  (lvve_ctrl_param_nv_ptr->ucModeName[i]));
                }

                
                result = LVVE_CTRL_PARAM_SetDevModeParam(name_ptr, lvve_ctrl_param_nv_ptr);

                if(result != AUDIO_NV_ARM_NO_ERROR)
                {
                    //SCI_TRACE_LOW:"ATC: LVVE_CTRL_PARAM_SetDevModeParam , result = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5117_112_2_17_22_16_40_227,(uint8*)"d",result);
                    flag = FALSE;
                }
                else
                {
                    flag = TRUE;
                }
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: ATC_ProcessSioSADLVVEData, input data length error."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5127_112_2_17_22_16_40_228,(uint8*)"");
            }
            
            break;
        }
        
        default:
        {
            //SCI_TRACE_LOW:"Lost the escape character is %s is line %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5135_112_2_17_22_16_40_229,(uint8*)"sd",__FILE__,__LINE__);
            break;
        }
    }

    /*if(PNULL != lvve_ctrl_param_nv_ptr)*/
    /*{*/
    SCI_FREE(lvve_ctrl_param_nv_ptr);
    /*}*/

    if (flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
    }
#endif
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//Description : This function handles the data send from the SIO when AT+SPADLVVE
//Global resource dependence : g_rsp_str
//  Author: chong.liu
//Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSADLVVENVData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                )
{
#ifndef WIN32
    uint8             terminater_char;     // indicates ther terminater char
    BOOLEAN           flag = FALSE;
    AUDMOD_RESULT_E   result;

    LVVE_CTRL_PARAM_NV_T *lvve_ctrl_param_nv_ptr = PNULL;
    uint8 *name_ptr = PNULL;

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified check NULL pointer*/
    SCI_ASSERT(PNULL != data_ptr);/*assert verified check NULL pointer*/

    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len  = param_len - 1;

    lvve_ctrl_param_nv_ptr = (LVVE_CTRL_PARAM_NV_T *)SCI_ALLOC(sizeof(LVVE_CTRL_PARAM_NV_T)+1);
    SCI_ASSERT(PNULL != lvve_ctrl_param_nv_ptr);/*assert verified check NULL pointer*/
    SCI_MEMSET((uint8*)lvve_ctrl_param_nv_ptr, 0, (sizeof(LVVE_CTRL_PARAM_NV_T)+1));

    //SCI_TRACE_LOW:"ATC: siosadm for arm, param_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5187_112_2_17_22_16_40_230,(uint8*)"d",param_len);

    switch (terminater_char)
    {
        case ESC:
        flag = TRUE;
        break;

        case CTRL_Z:
        {
            //SCI_TRACE_LOW:"atc: ctrl_z, sizeof(LVVE_CTRL_PARAM_NV_T)*2=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5197_112_2_17_22_16_40_231,(uint8*)"d",  sizeof(LVVE_CTRL_PARAM_NV_T)*2);
            if(param_len == sizeof(LVVE_CTRL_PARAM_NV_T)*2)
            {
                name_ptr = (uint8*)LVVE_CTRL_PARAM_GetDevModeName((AUDIO_DEVICE_MODE_TYPE_E)s_atc_admm_index);

                SCI_ASSERT(name_ptr != NULL);/*assert verified check NULL pointer*/

                ConvertHexToBin(data_ptr, param_len, (uint8*)lvve_ctrl_param_nv_ptr);

                //SCI_TRACE_LOW:"ATC:ATC_ProcessSioSADLVVENVData %s, %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5206_112_2_17_22_16_40_232,(uint8*)"sd", name_ptr, strlen((char *)name_ptr));

                lvve_ctrl_param_nv_ptr->ucModeName[strlen((char *)name_ptr)] = '\0';

                result = LVVE_CTRL_PARAM_WriteModeParamToFlash(name_ptr, lvve_ctrl_param_nv_ptr);
                if(result != AUDIO_NV_ARM_NO_ERROR)
                {
                    //SCI_TRACE_LOW:"ATC: ATC_ProcessSioSADLVVENVData , result = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5213_112_2_17_22_16_40_233,(uint8*)"d",result);
                    flag = FALSE;
                }
                else
                {
                    flag = TRUE;
                }
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: ATC_ProcessSioSADLVVEData, input data length error."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5223_112_2_17_22_16_40_234,(uint8*)"");
            }

            break;
        }
    
        default:
        {
            //SCI_TRACE_LOW:"Lost the escape character is %s is line %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5231_112_2_17_22_16_40_235,(uint8*)"sd",__FILE__,__LINE__);
            break;
        }
    }

    /*if(PNULL != lvve_ctrl_param_nv_ptr)*/
    /*{*/
    SCI_FREE(lvve_ctrl_param_nv_ptr);
    /*}*/

    if (flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
    }
#endif
    return S_ATC_DEFAULT_HANDLE;
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_GET_PIN_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPinInd(
                        ATC_CONFIG_T                   *atc_config_ptr,
                        APP_MN_SIM_GET_PIN_IND_T       *sig_ptr)
{
    MN_DUAL_SYS_E     dual_sys = sig_ptr->dual_sys;
    
#ifdef _MUX_REDUCED_MODE_
    uint32 i = 0;
    BOOLEAN is_received_sim_ind = TRUE;
    
    g_received_sim_ind[dual_sys] = TRUE;
    
    for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
    {
        if(FALSE == g_received_sim_ind[i])
        {
            is_received_sim_ind = FALSE;
            break;
        }
    }
    
    if(is_received_sim_ind && (FALSE == g_sent_sind4))
    {
        g_sent_sind4 = TRUE;
        if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT3))
        {
            sprintf((char*)g_rsp_str, "%s: 4",g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
        }
    }
#endif
    
    atc_config_ptr->sim_flag[dual_sys] = TRUE;
    switch (sig_ptr->pin_num)
    {
    case MNSIM_PIN1:
        if (sig_ptr->pin_is_blocked)
        {
            atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_PUK1;
            if (atc_config_ptr->cpin_flag[dual_sys])
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_INCORRECT_PASSWORD);
                atc_config_ptr->cpin_flag[dual_sys] = FALSE;
            }
        }
        else
        {
            atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_PIN1;
            if (atc_config_ptr->cpin_flag[dual_sys])
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_INCORRECT_PASSWORD);
                atc_config_ptr->cpin_flag[dual_sys] = FALSE;
            }
        }
        break;
    case MNSIM_PIN2:
        if (sig_ptr->pin_is_blocked)
        {
            atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_PUK2;
            if (atc_config_ptr->cpin_flag[dual_sys])
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_INCORRECT_PASSWORD);
                atc_config_ptr->cpin_flag[dual_sys] = FALSE;
            }
        }
        else
        {
            atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_PIN2;
            if (atc_config_ptr->cpin_flag[dual_sys])
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_INCORRECT_PASSWORD);
                atc_config_ptr->cpin_flag[dual_sys] = FALSE;
            }
        }
        break;

    default:
        break;
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_AOC_INCREASE_ACM_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessIncreaseAcmInd(
                      ATC_CONFIG_T                   *atc_config_ptr,
                      APP_MN_AOC_MODIFIED_IND_T      *sig_ptr
                      )
{
    uint32            ccm_val;

    ccm_val = sig_ptr->acm_unit;
    
    switch (atc_config_ptr->aoc_mode)
    {
        case ATC_AOC_QUERY_MODE:
        case ATC_AOC_DEACTIVE_MODE:
        {
            break;
        }

        case ATC_AOC_ACTIVE_MODE:
        {
            sprintf((char*)g_rsp_str, "+CCCM: %lx", ccm_val);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
        
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert ACM,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5370_112_2_17_22_16_40_236,(uint8*)"");
            break;
        }
    }
    return S_ATC_SUCCESS;
}
#endif
/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_NOT_READY_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessNotReadyInd(
                       ATC_CONFIG_T                    *atc_config_ptr,
                       APP_MN_SIM_NOT_READY_IND_T      *sig_ptr
                       )
{
    MN_DUAL_SYS_E     dual_sys = sig_ptr->dual_sys;
    //SCI_TRACE_LOW:"ATC:No sim reason = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5387_112_2_17_22_16_40_237,(uint8*)"d",sig_ptr->reason);
    s_sim_error_reason[dual_sys] = sig_ptr->reason;
#ifdef _ATC_ONLY

#ifdef _MUX_REDUCED_MODE_
    {
        uint32                                  i = 0;
        BOOLEAN is_received_sim_ind = TRUE;
        
        g_received_sim_ind[dual_sys] = TRUE;
        
        for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
        {
            if(FALSE == g_received_sim_ind[i])
            {
                is_received_sim_ind = FALSE;
                break;
            }
        }

        if(is_received_sim_ind && (FALSE == g_sent_sind4))
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

    if (s_received_sim_ready[dual_sys])
    {   
        if (sig_ptr->reason == MNSIM_ERROR_REMOVED)
        {
            
        }    
    }
#endif    

    if (atc_config_ptr->cpin_flag[dual_sys])
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_SIM_FAILURE);
        atc_config_ptr->cpin_flag[dual_sys] = FALSE;
    }
    
    //modified by alex li on 2005 10 24 cr: MS00033299
    switch (s_sim_error_reason[dual_sys])
    {
        case MNSIM_ERROR_REJECT_BY_NET:
        {
            atc_config_ptr->sim_flag[dual_sys] = TRUE;
            break;
        }
        
        case MNSIM_ERROR_PIN1_BLOCKED:
        {
            atc_config_ptr->cpin[dual_sys]     = ATC_CPIN_CODE_PUK1;
            atc_config_ptr->sim_flag[dual_sys] = TRUE;
            break;
        }
        
        case MNSIM_ERROR_PIN2_BLOCKED:
        {
            atc_config_ptr->cpin[dual_sys]     = ATC_CPIN_CODE_PUK2;
            atc_config_ptr->sim_flag[dual_sys] = TRUE;
            break;
        }
        
        case MNSIM_ERROR_PIN1_UNBLOCK_BLOCKED:
        {
            atc_config_ptr->cpin[dual_sys] = ATC_CPIN_CODE_BLOCK;
            atc_config_ptr->sim_flag[dual_sys] = TRUE;
            break;
        }
    
        case MNSIM_ERROR_PIN2_UNBLOCK_BLOCKED:
        {
            atc_config_ptr->sim_flag[dual_sys] = TRUE;
            break;
        }
            
        case MNSIM_ERROR_REMOVED:
        case MNSIM_ERROR_SERVICE_NOT_AVAILABLE:

        case MNSIM_ERROR_PIN1_UNINITIALIZED:
        case MNSIM_ERROR_PIN2_UNINITIALIZED:
           
        case MNSIM_ERROR_POWER_OFF:    
        case MNSIM_ERROR_NO_SECRETCODE_INITIALIZED:    
        case MNSIM_ERROR_INVALID_STATUS_RSP:
        case MNSIM_ERROR_INSERTED_ERROR:
        case MNSIM_ERROR_INITIALISE_FAILED:    
        case MNSIM_ERROR_MEMORY_PROBLEM:
        case MNSIM_ERROR_INVALID_PARAMS:
        case MNSIM_ERROR_FILE_INVALIDATED:
        case MNSIM_ERROR_FILE_NOT_FOUND:    
        case MNSIM_ERROR_GENERAL_FAULT:
        case MNSIM_ERROR_ALLOC_ERROR:
        case MNSIM_ERROR_SM_FAULT:    
        case MNSIM_ERROR_SERIOUS:
        case MNSIM_ERROR_UNKNOWN:
        case MNSIM_ERROR_NULL:
        {
            atc_config_ptr->sim_flag[dual_sys] = FALSE;
            break;
        }
            
        default :
        {
            atc_config_ptr->sim_flag[dual_sys] = FALSE;
            break;
        }
    }
    
    //Send the unsolicited +SIND: 1            
    if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT0))
    {
        if(FALSE == atc_config_ptr->sim_flag[dual_sys])
        {
            sprintf((char*)g_rsp_str, "%s: 0",g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            //SCI_TRACE_LOW:"ATC:+WIND: 1    "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5510_112_2_17_22_16_41_238,(uint8*)"");
        }
    }
    //Send the unsolicited +SIND: 10            
    if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT9))
    {
        sprintf((char*)g_rsp_str, "%s: 10,\"SM\",0,\"FD\",0,\"LD\",0,\"MC\",0,\"RC\",0,\"ME\",0",
                    g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_READY_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessReadyInd(
                          ATC_CONFIG_T                  *atc_config_ptr,
                          APP_MN_SIM_READY_IND_T        *sig_ptr)
{
    MN_DUAL_SYS_E     dual_sys = sig_ptr->dual_sys;
    
#ifdef _MUX_REDUCED_MODE_
    uint32 i = 0;
    BOOLEAN is_received_sim_ind = TRUE;
    g_received_sim_ind[dual_sys] = TRUE;

    
    for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
    {
        if(FALSE == g_received_sim_ind[i])
        {
            is_received_sim_ind = FALSE;
            break;
        }
    }

    if(is_received_sim_ind && (FALSE == g_sent_sind4))
    {
        g_sent_sind4 = TRUE;
        if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT3))
        {
            sprintf((char*)g_rsp_str, "%s: 4",g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
        }
    }
#endif
    
    s_received_sim_ready[dual_sys] = TRUE;

    if (atc_config_ptr->cpin_flag[dual_sys])
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
        atc_config_ptr->cpin_flag[dual_sys] = FALSE;
    }
    
    if (sig_ptr->pin1_is_enabled)
    {
        atc_config_ptr->pin1_enable_flag[dual_sys] = TRUE;
    }
    atc_config_ptr->sim_flag[dual_sys] = TRUE;
    atc_config_ptr->cpin[dual_sys]     = ATC_CPIN_CODE_READY;
    
    //Send the unsolicited +SIND: 1            
    if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT0))
    {
        sprintf((char*)g_rsp_str, "%s: 1",g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
        //SCI_TRACE_LOW:"ATC:+WIND: 2    "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5580_112_2_17_22_16_41_239,(uint8*)"");
    }
    
    //Send the unsolicited +SIND: 10            
    if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | ATC_WIND_BIT9))
    {
        sprintf((char*)g_rsp_str, "%s: 10,\"SM\",1,\"FD\",1,\"LD\",1,\"MC\",1,\"RC\",1,\"ME\",1",
                g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
    }
    return S_ATC_SUCCESS;
}
#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_SCELL_RSSI_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessRssiInd(
                          ATC_CONFIG_T                  *atc_config_ptr,
                          APP_MN_SCELL_RSSI_IND_T       *sig_ptr)
{
    uint16  rxlev;
    
#ifdef _MUX_REDUCED_MODE_
    rxlev = sig_ptr->rxlev;
    if(GPIO_IsAllowedReportRssiToAP())
    {
        if(110 - rxlev < g_atc_ciev_lev[3])
        {
            rxlev = 4;
        }
        else if(110 - rxlev < g_atc_ciev_lev[2])
        {
            rxlev = 3;
        }
        else if(110 - rxlev < g_atc_ciev_lev[1])
        {
            rxlev = 2;
        }
        else if(110 - rxlev < g_atc_ciev_lev[0])
        {
            rxlev = 1;
        }
        else
        {
            rxlev = 0;
        }

        if((MN_DUAL_SYS_1 <= sig_ptr->dual_sys) && (sig_ptr->dual_sys < MN_DUAL_SYS_MAX))
        {
            if(g_rxlev_arr[sig_ptr->dual_sys] != rxlev)
            {
                g_rxlev_arr[sig_ptr->dual_sys] = rxlev;
                sprintf((char*)g_rsp_str, "+CIEV: %d", rxlev);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);  
            }
        }
    }
#endif

    rxlev = sig_ptr->rxlev / 2; 
    if (atc_config_ptr->csq_flag)
    {
        sprintf((char*)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CSQ].cmd_name->str_ptr, rxlev,AT_INVALID_RXQUAL);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_DETACH_GSM_CNF signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPowerOffCnf(
                            ATC_CONFIG_T                  *atc_config_ptr,
                            APP_MN_DEACTIVE_PS_CNF_T     *sig_ptr
                            )
{
       MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

#ifdef _MUX_REDUCED_MODE_
    SCI_MEMSET((void *)g_rxlev_arr,0,sizeof(g_rxlev_arr));
#endif

    //If it deactivate the PS,dont power off
    if(TRUE == g_is_ps_deactivate[dual_sys])
    {
        g_is_ps_deactivate[dual_sys] = FALSE;
    }
#ifdef  _ATC_ONLY
    else
    {
        SCI_KILL_TIMER(P_ATC,ATC_POWEROFF_EXP_IND,&T_ATC_POWEROFF_EXP_IND,"T_ATC_POWEROFF_EXP_IND");
        POWER_PowerOff();
    }
#endif
    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
    
    g_is_ps_act[dual_sys] = FALSE;
    
    return S_ATC_DEFAULT_HANDLE;
}
/*****************************************************************************/
//  Description : This function handles the ATC_CHR_CAP_IND signal.And inform the 
//  DTE the current battery level
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessChrCapInd(
                    ATC_CONFIG_T                  *atc_config_ptr, 
                    xSignalHeaderRec              *sig_ptr
                    )
{
    uint32                 batt_vol = 0;
    uint32              batt_state = 0;

    if (atc_config_ptr->atc_config_nv.battery_indicator)
    {
        batt_vol   = CHR_GetVoltage();
        batt_state = CHR_GetBattState();
        sprintf((char*)g_rsp_str, "+SBCI: %ld,%ld",batt_state, batt_vol);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the ATC_CHR_CHARGE_START_IND signal.Inform
//  that the battery starts charge.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessChrStartInd(
                    ATC_CONFIG_T                  *atc_config_ptr, 
                    xSignalHeaderRec              *sig_ptr
                    )
{
    uint32              batt_vol = 0;
    
    if (atc_config_ptr->atc_config_nv.battery_indicator)
    {
        batt_vol   = CHR_GetVoltage();
        sprintf((char*)g_rsp_str, "+SBCI: %d,%ld",CHR_BATT_IN_CHARGE, batt_vol);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    return S_ATC_SUCCESS;
}
#endif
/*****************************************************************************/
//  Description : This function handles the ATC_ProcessPowerOnCnf signal
//  that the battery starts charge.
//  Global resource dependence : g_rsp_str
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPsPowerOnCnf(ATC_CONFIG_T *atc_config_ptr, 
                    xSignalHeaderRec              *sig_ptr)
{
#ifndef _ULTRA_LOW_CODE_
    MN_PHONE_CURRENT_PLMN_INFO_T   register_status;
    OPER_STATUS_E            oper_status;

    SCI_TRACE_LOW("atc: ATC_ProcessPsPowerOnCnf, cmd_id=%d", g_atc_power_info.cmd_id );
    if((g_atc_power_info.cmd_id != AT_CMD_MAXPOWER)
        && (g_atc_power_info.cmd_id != AT_CMD_READRSSI)
        &&(g_atc_power_info.cmd_id != AT_CMD_LOOPTEST))
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);  
    }
    else
    {
        SCI_MEMSET(&g_atc_power_info, 0, sizeof(ATC_POWER_INF_T));
    }
  
    //add this process, for user set at+cfun=1 in times
    register_status = MNPHONE_GetCurrentPLMNInfoEx(((APP_MN_PS_POWER_ON_CNF_T*)sig_ptr)->dual_sys);

    switch (register_status.plmn_status)
    {
        case PLMN_NO_SERVICE:
        {
            oper_status = NOT_REGISTERED_AND_SEARCHING;
            break;
        }

        case PLMN_EMERGENCY_ONLY:
            oper_status = REGISTRATION_DENIED;
            break;

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
    
    switch (atc_config_ptr->atc_config_nv.creg)
    {
        case ATC_CREG_DISABLE:
        {
                break;
        }
    
        case ATC_CREG_ENABLE:
        case ATC_CREG_AND_INFO:
        {
            sprintf((char*)g_rsp_str, "%s: %d", 
                        g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr, oper_status);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
            
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert Failure unknow creg."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5798_112_2_17_22_16_41_240,(uint8*)"");
            break;
        }
    }
#ifdef _SUPPORT_GPRS_    
    switch (atc_config_ptr->cgreg)
    {
        case ATC_CREG_DISABLE:
        {
                break;
        }
    
        case ATC_CREG_ENABLE:
        case ATC_CREG_AND_INFO:
        {
            sprintf((char*)g_rsp_str, "%s: %d", 
                g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr, oper_status);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert Failure unknow cgreg."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5821_112_2_17_22_16_41_241,(uint8*)"");
            break;
        }
    }
#endif
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the ATC_ProcessPowerOnCnf signal
//  that the battery starts charge.
//  Global resource dependence : g_rsp_str
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPsPowerOffCnf(ATC_CONFIG_T                  *atc_config_ptr, 
                    xSignalHeaderRec              *sig_ptr)
{
#ifdef _ULTRA_LOW_CODE_
#ifdef _MUX_REDUCED_MODE_
    SCI_MEMSET((void *)g_rxlev_arr,0,sizeof(g_rxlev_arr));
#endif
#endif
    //s_setup_menu_len[MN_DUAL_SYS_1] = 0;
    //s_setup_menu_len[MN_DUAL_SYS_2] = 0;
    //SCI_MEMSET(&s_setup_menu_data[MN_DUAL_SYS_1][0], 0, SIMAT_MAX_RPDU_DATA_LEN);
    //SCI_MEMSET(&s_setup_menu_data[MN_DUAL_SYS_2][0], 0, SIMAT_MAX_RPDU_DATA_LEN);
    //atc_config_ptr->cpas          = ATC_CPAS_STATUS_READY;
    SCI_TRACE_LOW("atc: ATC_ProcessPsPowerOffCnf %d",g_atc_power_info.cmd_id);

    if (g_atc_power_info.cmd_id == AT_CMD_MAXPOWER)
    {
        ATC_StartGsmTxPwrTest(atc_config_ptr);
        return S_ATC_SUCCESS;
    }
#if !defined(_ULTRA_LOW_CODE_) || defined(_ATC_KIWI)
    else if(g_atc_power_info.cmd_id == AT_CMD_READRSSI)
    {
        ATC_StartGsmRxPwrTest(atc_config_ptr);
        return S_ATC_SUCCESS;
    }
    else if(g_atc_power_info.cmd_id == AT_CMD_LOOPTEST)
    {
        ATC_LoopTestPsOffCnf(atc_config_ptr);
        SCI_TRACE_LOW("ATC: return here");
        return S_ATC_SUCCESS;
    }
    else
    {
        #ifndef _ULTRA_LOW_CODE_
        g_creg_state[((APP_MN_PS_POWER_OFF_CNF_T *)sig_ptr)->dual_sys].oper_status = NOT_REGISTERED_NOT_SEARCHING;
        
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
        #endif
    }

    #ifndef _ULTRA_LOW_CODE_
    switch (atc_config_ptr->atc_config_nv.creg)
    {
        case ATC_CREG_DISABLE:
        {
            break;
        }
            
        case ATC_CREG_ENABLE:
        case ATC_CREG_AND_INFO:
        {
            sprintf((char*)g_rsp_str, "%s: %d", 
                g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr, NOT_REGISTERED_NOT_SEARCHING);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
            
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert Failure unknow creg."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5871_112_2_17_22_16_41_242,(uint8*)"");
            return S_ATC_DEFAULT_HANDLE;
        }
    }
#ifdef _SUPPORT_GPRS_
    switch (atc_config_ptr->cgreg)
    {
        case ATC_CREG_DISABLE:
        {
                break;
        }
    
        case ATC_CREG_ENABLE:
        case ATC_CREG_AND_INFO:
        {
            sprintf((char*)g_rsp_str, "%s: %d", 
                g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr, NOT_REGISTERED_NOT_SEARCHING);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
    
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert Failure unknow cgreg."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_5894_112_2_17_22_16_41_243,(uint8*)"");
            break;
        }
    } 
#endif
    #ifdef _MUX_REDUCED_MODE_
    sprintf((char*)g_rsp_str, "+CIEV: 0");
    #endif
       ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);  

    return S_ATC_SUCCESS;    
    #endif

#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the ATC_CHR_CHARGE_END_IND signal.Inform
//  that the battery starts charge.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessChrEndInd(
                    ATC_CONFIG_T                  *atc_config_ptr, 
                    xSignalHeaderRec              *sig_ptr
                    )
{
    uint32              batt_vol;
    
    if (atc_config_ptr->atc_config_nv.battery_indicator)
    {
        batt_vol   = CHR_GetVoltage();
        sprintf((char*)g_rsp_str, "+SBCI: %d,%ld",CHR_BATT_OUT_OF_CHARGE, batt_vol);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    return S_ATC_SUCCESS;
}
#endif

/*****************************************************************************/
//  Description : This function handle the AT## command 
//  This command is called by Public phone module
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSHARP_SHARP )
{
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_NETWORK_STATUS_IND at the
//                 AT+SBAND 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSetBandCnf(
                ATC_CONFIG_T                *atc_config_ptr,
                APP_MN_SET_BAND_CNF_T       *sig_ptr
                )
{
    //SCI_TRACE_LOW("ATC:enter ATC_ProcessSetBandCnf");
    if(sig_ptr->is_ok)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,0);
    }
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the AT+SBAND command, This command is used
//  to select the band in EGSM900/DCS1800/PCS1900.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSBAND )
{
    ATC_STATUS     status = S_ATC_SUCCESS;
    int32          band = 0;

    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                band = PARA1.num;
                switch(band)
                {
                    case 0:
                    {
                    band = MN_PHONE_BAND_GSM;
                    break;
                    }
                    
                    case 1:
                    {
                        band = MN_PHONE_BAND_DCS;
                        break;
                    }
                    
                    case 2:
                    {
                        band = MN_PHONE_BAND_PCS;
                        break;
                    }
                    
                    case 3:
                    {
                        band = MN_PHONE_BAND_GSM850;
                        break;
                    }
                    
                    case 4:
                    {
                        band = MN_PHONE_BAND_GSM_DCS;
                        break;
                    }
                    
                    case 5:
                    {
                        band = MN_PHONE_BAND_GSM850_GSM;
                        break;
                    }
                    
                    case 6:
                    {
                        band = MN_PHONE_BAND_GSM850_DCS;
                        break;
                    }
                    
                    case 7:
                    {
                        band = MN_PHONE_BAND_GSM850_PCS;
                        break;
                    }
                    
                    case 8:
                    {
                        band = MN_PHONE_BAND_GSM_PCS;
                        break;
                    }
                    
                    case 9:
                    {
                        band = MN_PHONE_BAND_GSM850_GSM_DCS;
                        break;
                    }
                    
                    case 10:
                    {
                        band = MN_PHONE_BAND_GSM850_GSM_PCS;
                        break;
                    }
                    
                    case 11:
                    {
                        status = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }
                
                    case 12:
                    {
                        status = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }
                    
                    case 13:
                    {
                        band = MN_PHONE_BAND_GSM_DCS_PCS;
                        break;
                    }
                
                    case 14:
                    {
                        band = MN_PHONE_BAND_GSM850_GSM_DCS_PCS;
                        break;
                    }
                     default:
                         break;
                 }
                     
                 if(status != S_ATC_SUCCESS)
                 {
                     return status;
                 }

                 if(ERR_MNPHONE_NO_ERR != MNPHONE_SelectBandEx(dual_sys, band))
                 {
                     //SCI_TRACE_LOW("ATC:error MNPHONE_SelectBandEx");
                     status = S_ATC_FAIL;
                     return status;
                 }
                     
                 ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_SET_BAND_CNF,0xff,ATC_NO_DOMAIN);

                  status = S_ATC_DEFAULT_HANDLE;
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
             
        case ATC_CMD_TYPE_READ:
        {
            //read band from the NV.
            band = MNPHONE_GetBandEx(dual_sys);
            switch(band)
            {
                case MN_PHONE_BAND_GSM:
                {
                    band = 0;
                    break;
                }
                
                case MN_PHONE_BAND_DCS:
                {
                    band = 1;
                    break;
                }
                
                case MN_PHONE_BAND_PCS:
                {
                    band = 2;
                    break;
                }
                
                case MN_PHONE_BAND_GSM850:
                {
                    band = 3;
                    break;
                }
                
                case MN_PHONE_BAND_GSM_DCS:
                {
                    band = 4;
                    break;
                }
                
                case MN_PHONE_BAND_GSM850_GSM:
                {
                    band = 5;
                    break;
                }
                
                case MN_PHONE_BAND_GSM850_DCS:
                {
                    band = 6;
                    break;
                }
                
                case MN_PHONE_BAND_GSM850_PCS:
                {
                    band = 7;
                    break;
                }
                
                case MN_PHONE_BAND_GSM_PCS:
                {
                    band = 8;
                    break;
                }
                
                case MN_PHONE_BAND_GSM850_GSM_DCS:
                {
                    band = 9;
                    break;
                }
                
                case MN_PHONE_BAND_GSM850_GSM_PCS:
                {
                    band = 10;
                    break;
                }
                
                case MN_PHONE_BAND_DCS_PCS:
                {
                    //band = 11;
                    //PS not support this combin band
                    //SCI_TRACE_LOW:"ATC: assert case dcs pcs."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6176_112_2_17_22_16_42_244,(uint8*)"");
                    break;
                }
                
                case MN_PHONE_BAND_GSM850_DCS_PCS:
                {
                    //band = 12;
                    //PS not support this combin band
                    //SCI_TRACE_LOW:"ATC: assert case dcs pcs."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6184_112_2_17_22_16_42_245,(uint8*)"");
                    break;
                }
                
                case MN_PHONE_BAND_GSM_DCS_PCS:
                {
                    band = 13;
                    break;
                }
                
                case MN_PHONE_BAND_GSM850_GSM_DCS_PCS:
                {
                    band = 14;
                    break;
                }
                
                default:
                {
                    //SCI_TRACE_LOW:"ATC: unknow band %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6202_112_2_17_22_16_42_246,(uint8*)"d",band);
                    break;
                }
            }
            sprintf((char*)g_rsp_str, "%s: %ld", 
                        g_atc_info_table[AT_CMD_SBAND].cmd_name->str_ptr, band);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
             
        default:
        {
            break;
        }
    }

    return status;

}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function convert the string of IMEI to BCD code.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
LOCAL BOOLEAN Atc_ConImeiStrToBin(    // FALSE: There is invalid char
                        uint8        *hex_ptr,     // in: the hexadecimal format string
                        uint16       length,       // in: the length of hexadecimal string 
                        uint8        *bin_ptr      // out: pointer to the binary format string
                        )
{
    uint8        *dest_ptr = bin_ptr;
    int32                     i = 0;
    uint8                   ch = 0;

    SCI_ASSERT(PNULL != hex_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != bin_ptr); /*assert verified*/

    //Handle the first value.
    ch = hex_ptr[0];
    // digital 0 - 9
    if (ch >= '0' && ch <= '9')
        *dest_ptr =(uint8)((ch - '0') << 4);
    // a - f
    else if (ch >= 'a' && ch <= 'f')
        *dest_ptr = (uint8)((ch - 'a' + 10) << 4);
    // A - F
    else if (ch >= 'A' && ch <= 'F')
        *dest_ptr = (uint8)((ch -'A' + 10) << 4);
    else
    {
        return FALSE;
    }
    *dest_ptr+=0x0A;

    dest_ptr++;

    for (i=1; i<length; i+=2)
    {
        // the bit 8,7,6,5
        ch = hex_ptr[i];
        // digital 0 - 9
        if (ch >= '0' && ch <= '9')
            *dest_ptr =(uint8)(ch - '0');
        // a - f
        else if (ch >= 'a' && ch <= 'f')
            *dest_ptr = (uint8)(ch - 'a' + 10);
        // A - F
        else if (ch >= 'A' && ch <= 'F')
            *dest_ptr = (uint8)(ch -'A' + 10) ;
        else
        {
            return FALSE;
        }
        
        // the bit 1,2,3,4
        ch = hex_ptr[i+1];
        // digtial 0 - 9 
        if (ch >= '0' && ch <= '9')
            *dest_ptr |= (uint8)((ch - '0')<< 4);
        // a - f
        else if (ch >= 'a' && ch <= 'f')
            *dest_ptr |= (uint8)((ch - 'a' + 10)<< 4);
        // A - F
        else if (ch >= 'A' && ch <= 'F')
            *dest_ptr |= (uint8)((ch -'A' + 10)<< 4); 
        else
        {
            return FALSE;
        }

        dest_ptr++;
    }
    *dest_ptr = '\0';
    return TRUE;
}
#endif

#ifdef _ATC_ONLY
/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SNVM
//  Global resource dependence : 
//  Author:       SHIJUN
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSNVMData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr  // pointer to the data
                )
{
    uint16          data_length;
    uint8           terminator;        // holds the terminal charactor
    NVITEM_ERROR_E  nv_err = NVERR_NONE;
    
    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    
    data_length = strlen((char*)data_ptr);
    terminator = data_ptr[data_length - 1];
    
    switch (terminator)
    {
        case CTRL_Z:
        {
            ConvertHexToBin(data_ptr, data_length, data_ptr);
            EFS_NvitemDelete(s_nv_item_id, s_nv_item_id);
            nv_err = EFS_NvitemWrite(s_nv_item_id, data_length/2, data_ptr, FALSE);
            if(NVERR_NONE == nv_err)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
            }
            break;
        }
            
        case ESC:
        {
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
            break;
        }
            
        default :
        {
            //SCI_TRACE_LOW:"ATC: snvm data format is error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6345_112_2_17_22_16_42_247,(uint8*)"");
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
            break;
        }
    }
    return S_ATC_DEFAULT_HANDLE;
}
#endif

/*****************************************************************************/
//  Description : This function converts the hex to bin
//  Global resource dependence : 
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN   ConvertInt16ToBin(    // FALSE: There is invalid char
                        uint8        *hex_ptr,    // in: the hexadecimal format string of address
                        uint16       *bin_ptr      // out: pointer to the address
                        )
{
    uint16      *dest_ptr = bin_ptr;
    int32                     i = 0;
    uint8                   ch = 0 ;

    SCI_ASSERT(PNULL != hex_ptr);/*assert verified*/
    SCI_ASSERT(PNULL != bin_ptr);/*assert verified*/

    *bin_ptr = 0;
    for (i=0; i<4; i++)
    {
        ch = hex_ptr[i];        
        (*dest_ptr) = (*dest_ptr)*16;
        // digital 0 - 9
        if (ch >= '0' && ch <= '9')
            *dest_ptr +=(uint8)(ch - '0');
        // a - f
        else if (ch >= 'a' && ch <= 'f')
            *dest_ptr += (uint8)(ch - 'a' + 10) ;
        // A - F
        else if (ch >= 'A' && ch <= 'F')
            *dest_ptr += (uint8)(ch -'A' + 10);
        else
        {
            return FALSE;
        }
        //SCI_TRACE_LOW("ATC:dest_Addr=%x,ch=%x",*dest_ptr,ch);
    }
    return TRUE;
}
#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the ATCMD_READW_CNF signal and send the 
//                result code to SIO
//  Global resource dependence : 
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
PUBLIC ATC_STATUS ATC_ProcessReadDSPCnf(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_READ_DSP_MEM_CNF_T    *sig_ptr
                )
{
    #ifdef _GSM_ONLY_  //CR171132 for 6530 codesize cutdown
    uint8 length = sig_ptr->length;   
    if(0 == length)
    {
        //Return the result
        sprintf((char*)g_rsp_str, "%s: 0x%x", g_atc_info_table[AT_CMD_SRWD].cmd_name->str_ptr, sig_ptr->result);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
    }
    else
    #endif
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SRWD
//  Global resource dependence : g_rsp_str
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSRWDData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr  // pointer to the data
                )
{
    uint8       terminater_char;     // indicates ther terminater char
    uint16      data_len;          // holds the gived pdu length
    BOOLEAN     flag = FALSE;
    uint32      i;
    int16       *value_or_source_address; // If the length is 0,the value will be put into the destine address directly.
                                         // Otherwise a memcpy is performed.        

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    data_len              = (uint16)strlen((char*)data_ptr);
    terminater_char      = data_ptr[data_len - 1];
    data_ptr[data_len - 1] = '\0';
    data_len             -= 1;
    
    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
        {
            flag = TRUE;
            break;
        }

        case CTRL_Z:
        {
            if(0 == s_dsp_data_length)
            {
                //allocate one word memory
                value_or_source_address  = SCI_ALLOC_BASEZ(2);
                if(!ConvertInt16ToBin(data_ptr,(uint16 *)value_or_source_address))
                {
                    //SCI_TRACE_LOW("ATC:AT+SRWD,ConvertHexToBin=FALSE");
                    break;
                }
                //SCI_TRACE_LOW:"ATC:+SRWD,source_address[0,1,2]=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6465_112_2_17_22_16_42_248,(uint8*)"d",*value_or_source_address);

            }
            else
            {
                //uint16 *temp_ptr = PNULL;
                value_or_source_address  = SCI_ALLOC_BASEZ(data_len/2);
                //Change to bin format.
                for(i=0;i<data_len/4;i++)
                {
                    ConvertInt16ToBin((data_ptr+i*4), (uint16 *)(value_or_source_address+i));
                }
                //SCI_TRACE_LOW:"ATC:+SRWD,source_address[0,1,2]=%x,%x,%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6478_112_2_17_22_16_43_249,(uint8*)"ddd",*value_or_source_address,*(value_or_source_address+1),*(value_or_source_address+1));
            }
            flag = TRUE;
            // call the process to write DSP
            L1API_WriteDspMemory(s_dsp_ram_flag,s_dsp_dest_addr,s_dsp_data_length,value_or_source_address);
            break;
        }
            
        default:
            break;
    }

    if (!flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);         
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);        
    }
    return S_ATC_DEFAULT_HANDLE;
}
#endif    

#ifdef _GSM_ONLY_ 
/*****************************************************************************/
//  Description : This function handles the AT+SRWD command, This command is used
//  to read or write the DSP.
//  Global resource dependence : g_rsp_str
//  Author:       hanjun.liu
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSRWD )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS    status = S_ATC_SUCCESS;
    int16         temp_address;    
    //check the validity of parameter
    if ((!PARAM1_FLAG)||(!PARAM2_FLAG)||(!PARAM3_FLAG)||(!PARAM4_FLAG))
    {
        //SCI_TRACE_LOW:"ATC:+SRWD,error in params"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6515_112_2_17_22_16_43_250,(uint8*)"");
        status = S_ATC_SUCCESS;
        return status;
    }

    //handle the parameter
    s_dsp_ram_flag = PARA2.num;
    s_dsp_data_length   = PARA3.num;
    if(4 != PARA4.str_ptr->str_len)
    {
        //SCI_TRACE_LOW:"ATC:Length of string is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6525_112_2_17_22_16_43_251,(uint8*)"");
        status = ERR_INVALID_INDEX;
        return status;
    }

    //Contert the ascci format of address to bin.
    temp_address = 0;
    if(!ConvertInt16ToBin(PARA4.str_ptr->str_ptr,(uint16 *)&temp_address))
    {
        //SCI_TRACE_LOW("ATC:+SRWD,ConvertInt16ToBin=FALSE");
        status = ERR_INVALID_INDEX;
        return status;
    }

    //write the DSP
    if (PARA1.num)
    {
        s_dsp_dest_addr = temp_address;
        // output the '>'
        sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
    #ifdef _MUX_ENABLE_
        if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
        {
            // set the terminate char to receive PDU from the SIO
            SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);            
            SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
        }
        else
        {
            ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
            MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
            // set the terminate char to receive PDU from the SIO
        }
        atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;
        atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_SRWD;
    #else
        // set the terminate char to receive PDU from the SIO
        SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
        atc_config_ptr->is_ctrlz_end = TRUE;
        atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_SRWD;
    #endif
        return S_ATC_DEFAULT_HANDLE;
    }
    //read the DSP
    else
    {
        //call the function to read data from DSP
        L1API_ReadDspMemory(s_dsp_ram_flag,temp_address,s_dsp_data_length,P_ATC);
        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_READ_DSP_MEM_CNF,0xff,ATC_NO_DOMAIN);
        return S_ATC_DEFAULT_HANDLE;
    }
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
#endif

/*****************************************************************************/
//  Description : This function handles the AT+SIND command 
//  Global resource dependence : g_rsp_str
//  Author:       hanjun.liu
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSIND )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS    status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        //set command AT+SIND=IndLevel
        case ATC_CMD_TYPE_SET:
        {
            // check the index
            if (!PARAM1_FLAG)
                return ERR_INVALID_INDEX;
            atc_config_ptr->atc_config_nv.wind_ind_level = (uint16)PARA1.num;
            //save in the NV.
            if(!ATC_WriteNVItem(atc_config_ptr))
                return S_ATC_FAIL;
            break;
        }

        //Test command AT+SIND?
        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d",g_atc_info_table[AT_CMD_SIND].cmd_name->str_ptr, 
                        atc_config_ptr->atc_config_nv.wind_ind_level);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert SIND,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6617_112_2_17_22_16_43_252,(uint8*)"");
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+SUPS command.This command is used
//  to enable the log output.
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSUPS )
{
    ATC_STATUS    status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // check the type
            if (PARAM1_FLAG)
            {
                //PARA1.num define the uart switch
                if (PARA1.num)
                {
                    SIO_ATC_SetMixMode(TRUE);
                }
                else
                {
                    SIO_ATC_SetMixMode(FALSE);
                }
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }
            
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert SUPS,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6661_112_2_17_22_16_43_253,(uint8*)"");
            break;
        }
    }
    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+PSORGMOD command.This command is used
//  to set audio mode. This is used for AP+BB scenario. If AP connect only with BB's 
//  receiver, and AP want to use this physical channel as handon/handfree/BT earphone,
//  it will use this function to set audio param to DSP without change physical channel.
//  handon/handfree/BT should have the same device name in the NV item.
//  Global resource dependence : g_rsp_str
//  Author:       
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPSORGMOD)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS           status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                atc_config_ptr->audio_mode = (uint8)PARA1.num;
                AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)atc_config_ptr->audio_mode);
            }   
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
    
        case ATC_CMD_TYPE_READ:
        {
            // get the current sound mode
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_PSORGMOD].cmd_name->str_ptr, atc_config_ptr->audio_mode);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            break;
        }
    }

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}


/*****************************************************************************/
//  Description : This function handles the APP_MN_CRSM_CNF signal in 
//  deleting only one item.
//  Global resource dependence : 
//  Author:       WangHui
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessCrsmCnf(
                ATC_CONFIG_T         *atc_config_ptr,
                APP_MN_CRSM_CNF_T    *sig_ptr
                )
{
#ifndef _ULTRA_LOW_CODE_
   uint8 data_response[256] = "\0";

    //SCI_TRACE_LOW:"ATC:ATC_ProcessCrsmCnf"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6730_112_2_17_22_16_43_254,(uint8*)"");
    if (sig_ptr->rsp_data.response == TRUE)
    {
        //SCI_TRACE_LOW:"ATC:ATC_ProcessCrsmCnf,true"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6733_112_2_17_22_16_43_255,(uint8*)"");
        if (0 < sig_ptr->rsp_data.length)
        {
            ATC_TranUint8IntoChar(sig_ptr->rsp_data.data,sig_ptr->rsp_data.length,data_response);
            sprintf((char*)g_rsp_str, "+CRSM: %d,%d,\"%s\"", sig_ptr->rsp_data.sw1,sig_ptr->rsp_data.sw2,data_response);
        }
        else
        {
            sprintf((char*)g_rsp_str, "+CRSM: %d,%d", sig_ptr->rsp_data.sw1,sig_ptr->rsp_data.sw2);
        }
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);        
    }
    else
    {
        //SCI_TRACE_LOW:"ATC:ATC_ProcessCrsmCnf,FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6748_112_2_17_22_16_43_256,(uint8*)"");
        sprintf((char*)g_rsp_str, "+CRSM: %d,%d", sig_ptr->rsp_data.sw1,sig_ptr->rsp_data.sw2);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);  
    }
#endif /* _ULTRA_LOW_CODE_ */

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the AT+SCPFRE command.This command is used
//  to set or get frequency value.
//  Global resource dependence : g_rsp_str
//  Author:       king.li
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCPFRE )
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    uint8        band;
    uint16        frequency, freq; // 0 - set gpio value, 1 - get gpio value
    BOOLEAN   ismode = FALSE;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // check the type
            if (PARAM1_FLAG && PARAM2_FLAG)
            {
                //PARA1.num define the band
                band = (uint8)PARA1.num;
                //PARA2.num define the frequency
                frequency = (uint16)PARA2.num;
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }
            /*in CALIBRATION_POST_MODE, should set the frequency parameter to global variable
               msg_head->seq_num higher 2 bytes carry the frequency information; big endian
               frequency information includes frequency(11bit) and band(4bit);
               bit    0   1   2   3   4   5   6   7    8   9   10  11  12  13  14  15 
                       |                                |                                         |
                               seq_num[0]                       seq_num[1]            
                       |                                     |   |    |    |    |    |    |   |  
                       |                                       frequency(low 8 bit)   
                       |   |   |        |   |   |   |
             frequency(high 3 bit)      band(4 bits)                   
                */
            freq = (frequency&0xff)|((frequency&0x700)<<5)|((band&0xf)<<8);
            POWER_SetCalibrationPostModeFreq(freq);

            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
            sprintf((char*)g_rsp_str,"%s: 0x%x", g_atc_info_table[AT_CMD_SCPFRE].cmd_name->str_ptr,freq);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
        
        case ATC_CMD_TYPE_READ:
        {
#ifndef WIN32
            ismode = RM_GetCalibrationPostMode();
#endif
            if(ismode)
            {
                  freq = POWER_GetCalibrationPostModeFreq();
                 frequency = (freq&0xff)|((freq&0xe000)>>5);
                 band = (freq&0xf00)>>8;
                  sprintf((char*)g_rsp_str,"%s: %d,%d",g_atc_info_table[AT_CMD_SCPFRE].cmd_name->str_ptr,
                     frequency, band);
                 ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }
             break;
        }
    
        default:
        {
            break;
        }
    }
    return  status;
}


/*****************************************************************************/
//  Description : This function handles the AT+SSYS command.This command is used
//  to set or get the operated sim id .
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSYS )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS    status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // check the type            
            if (PARAM1_FLAG)
            {
#ifdef _MUX_ENABLE_
                if(g_mux_sys_info[MUX_1].is_start!=MUX_STATUS_OFF)
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    break;
                }
#endif
                g_current_card_id = (MN_DUAL_SYS_E)(PARA1.num);
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;    
            }
            break;
        }
    
        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d",g_atc_info_table[AT_CMD_SSYS].cmd_name->str_ptr, 
                g_current_card_id);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }
    
        default:
        {
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+SCINF command.This command is used
//                      to get some information about the serving and neighbour cells .
//  Global resource dependence : g_rsp_str
//  Author:       elvis.xu
/*****************************************************************************/
#define RR_CELL_INFO_SIZE 6
AT_CMD_FUNC( ATC_ProcessSCINF )
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    RR_CELLS_INFO_T *rr_cells_info_bsic_ptr = PNULL;
    RR_CELLS_INFO_TRAFFIC_T *rr_cells_info_bsic_traffic_ptr = PNULL;
    RR_CELLS_INFO_EX_T *rr_cells_info_ex_ptr = PNULL;
    BOOLEAN is_traffic = FALSE;
    BOOLEAN is_present = FALSE;
    uint8 * rsp_buf_ptr = PNULL;
    int16 i, ncell_num = 0;
    
    RR_CELL_INFO_TRAFFIC_T * t_sc_ptr = PNULL;
    RR_CELL_INFO_T  * t_ncs_ptr = PNULL;
    
    RR_CELL_INFO_T * i_sc_ptr = PNULL;
    RR_CELL_INFO_T * i_ncs_ptr = PNULL;
    
    RR_CELL_INFO_EX_T * i_sc_e_ptr = PNULL;
    RR_CELL_INFO_EX_T * i_ncs_e_ptr = PNULL;

    //SCI_TRACE_LOW:"ATC:ATC_ProcessSCINF"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6914_112_2_17_22_16_43_257,(uint8*)"");

    switch(ATC_GET_CMD_TYPE)
    {
        //case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_EXECUTE:
        {
            rr_cells_info_bsic_ptr = (RR_CELLS_INFO_T *)SCI_ALLOC_BASEZ(sizeof(RR_CELLS_INFO_T));
            
            rr_cells_info_bsic_traffic_ptr = (RR_CELLS_INFO_TRAFFIC_T *)SCI_ALLOC_BASEZ(sizeof(RR_CELLS_INFO_TRAFFIC_T));
            
            rr_cells_info_ex_ptr = (RR_CELLS_INFO_EX_T *)SCI_ALLOC_BASEZ(sizeof(RR_CELLS_INFO_EX_T));
                    
            //to get cell parameters and information from RR
            PS_GetRRCellsInfoBsicAndEx(dual_sys,
                                                              rr_cells_info_bsic_ptr,
                                                              rr_cells_info_bsic_traffic_ptr,
                                                              rr_cells_info_ex_ptr,
                                                              &is_traffic);

            //to check cell params and info
            rsp_buf_ptr = &g_rsp_str[0];
            if(is_traffic)
            {
                is_present = rr_cells_info_bsic_traffic_ptr->scell.present;
                if(is_present)
                {
                    sprintf((char *)rsp_buf_ptr,"+SCINF: %d,", ATC_TRAFFIC);
                    rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                    
                    //to send serving cell params
                    t_sc_ptr = &(rr_cells_info_bsic_traffic_ptr->scell);
                    sprintf((char *)rsp_buf_ptr, "%d,%d,%d,",
                                  t_sc_ptr->arfcn, t_sc_ptr->bsic, t_sc_ptr->rxlev_full);
                    rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                    
                    sprintf((char *)rsp_buf_ptr, "%d,%d,%d;",
                                  t_sc_ptr->rxlev_sub, t_sc_ptr->rxqual_full,t_sc_ptr->rxqual_sub);
                    rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                    
                    //to count the number of neighbour cells
                    t_ncs_ptr = rr_cells_info_bsic_traffic_ptr->ncells;
                    for (i = 0; i < RR_CELL_INFO_SIZE; i++)
                    {
                        if(FALSE == t_ncs_ptr[i].present)
                        {
                            continue;
                        }
                        ncell_num++;
                    }
                    
                    //to send neighbour cells' info             
                    for (i = 0; i < RR_CELL_INFO_SIZE; i++)
                    {
                        if(FALSE == t_ncs_ptr[i].present)
                        {
                            continue;
                        }
                        sprintf((char *)rsp_buf_ptr, "%d,%d,%d,%d;",
                                      ncell_num--, t_ncs_ptr[i].arfcn, t_ncs_ptr[i].bsic, t_ncs_ptr[i].rxlev);
                        rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                    }

                    ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);                                        
                    status = S_ATC_SUCCESS;
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC: no network"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_6982_112_2_17_22_16_44_258,(uint8*)"");
                    status = ERR_NO_NETWORK_SERVICE;
                }
            }
            else
            {
                is_present = rr_cells_info_bsic_ptr->scell.present;
                if(is_present)
                {
                    sprintf((char *)rsp_buf_ptr,"+SCINF: %d,", ATC_IDLE);
                    rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                    
                    //to send serving cell params
                    i_sc_ptr = &(rr_cells_info_bsic_ptr->scell);
                    sprintf((char *)rsp_buf_ptr, "%d,%d,%d,",
                                 i_sc_ptr->arfcn, i_sc_ptr->bsic, i_sc_ptr->rxlev);
                    rsp_buf_ptr += strlen((char *)rsp_buf_ptr);

                    sprintf((char *)rsp_buf_ptr, "%d,%d,",
                                 i_sc_ptr->C1, i_sc_ptr->C2);
                    rsp_buf_ptr += strlen((char *)rsp_buf_ptr);

                    i_sc_e_ptr = &(rr_cells_info_ex_ptr->scell);
                    sprintf((char *)rsp_buf_ptr, "%d,%d,%d,",
                                  i_sc_e_ptr->ms_txpwr_max_ccch, i_sc_e_ptr->rxlev_access_min, i_sc_e_ptr->CBA);
                    rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                    
                    sprintf((char *)rsp_buf_ptr, "%d,%d,%d,",
                                  i_sc_e_ptr->CBQ, i_sc_e_ptr->CRO, i_sc_e_ptr->TempOffset);
                    rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                    
                    sprintf((char *)rsp_buf_ptr, "%d,%d;",
                                  i_sc_e_ptr->PT, i_sc_e_ptr->CRH);
                    rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                    
                    //to count the number of neighbour cells
                    i_ncs_ptr = &(rr_cells_info_bsic_ptr->ncells[0]);
                    i_ncs_e_ptr = &(rr_cells_info_ex_ptr->ncells[0]);
                    
                    for (i = 0; i < RR_CELL_INFO_SIZE; i++)
                    {
                        if(FALSE == i_ncs_ptr[i].present)
                        {
                            continue;
                        }
                        ncell_num++;
                    }
                    
                    //to send neighbour cells' info
                    for (i = 0; i < RR_CELL_INFO_SIZE; i++)
                    {
                        if(FALSE == i_ncs_ptr[i].present)
                        {
                            continue;
                        }
                        
                        sprintf((char *)rsp_buf_ptr, "%d,%d,%d,",
                                      ncell_num--, i_ncs_ptr[i].arfcn, i_ncs_ptr[i].bsic);
                        rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                        
                        sprintf((char *)rsp_buf_ptr, "%d,%d,%d,",
                                      i_ncs_ptr[i].rxlev, i_ncs_ptr[i].C1, i_ncs_ptr[i].C2);
                        rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                        
                        i_ncs_e_ptr = rr_cells_info_ex_ptr->ncells;
                        sprintf((char *)rsp_buf_ptr, "%d,%d,%d,",
                                      i_ncs_e_ptr[i].ms_txpwr_max_ccch, i_ncs_e_ptr[i].rxlev_access_min, i_ncs_e_ptr[i].CBA);
                        rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                        
                        sprintf((char *)rsp_buf_ptr, "%d,%d,%d,",
                                      i_ncs_e_ptr[i].CBQ, i_ncs_e_ptr[i].CRO, i_ncs_e_ptr[i].TempOffset);
                        
                        sprintf((char *)rsp_buf_ptr, "%d,%d;",
                                      i_ncs_e_ptr[i].PT, i_ncs_e_ptr[i].CRH);
                        rsp_buf_ptr += strlen((char *)rsp_buf_ptr);
                    }
                                    
                    ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
                    status = S_ATC_SUCCESS;
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC: no network"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7064_112_2_17_22_16_44_259,(uint8*)"");
                    status = ERR_NO_NETWORK_SERVICE;
                }
            
            }

            SCI_FREE(rr_cells_info_bsic_ptr);
            SCI_FREE(rr_cells_info_bsic_traffic_ptr);
            SCI_FREE(rr_cells_info_ex_ptr);
            break;
        }
                    
        case ATC_CMD_TYPE_TEST:
        {
            //SCI_TRACE_LOW:"ATC: Test Scinf CMD"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7078_112_2_17_22_16_44_260,(uint8*)"");
            status = S_ATC_SUCCESS;
            break;
        }
                    
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert SCINF,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7085_112_2_17_22_16_44_261,(uint8*)"");
            break;
        }
     }
     
     return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPEQPARA command  
//                      Get(Set) eq para from(to) RAM or FALSH
//  Global resource dependence : 
//  Author:     elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPEQPARA) 
{
#if !defined(MS_OAPI_SUPPORT) || defined(CHIP_VER_6600L6) || defined(CHIP_VER_6610)
    AUDIO_EQ_STRUCT_T   *eq_para_ptr = NULL;
    uint8       *buffer_ptr = NULL;
    AUDIO_RESULT_E      set_result;

    if (ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE)
    {
        if (!PARAM1_FLAG)
        {
            //SCI_TRACE_LOW:"ATC: ATC_ProcessSPEQPARA: <n> is invalid."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7109_112_2_17_22_16_44_262,(uint8*)"");
            return ERR_INVALID_INDEX;
        }
        
        /*PARA1.num is <n>*/
        switch (PARA1.num)
        {
            case 0:
            {
#if !defined(WIN32)
                //get the maximum count of eq para type
                sprintf((char *)g_rsp_str, "%s: %lu", 
                        g_atc_info_table[AT_CMD_SPEQPARA].cmd_name->str_ptr,
                        AUDPROC_EXP_GetEqParaSetNum());
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
#endif
                break;
            }
        
            case 1:   //get eq para structure info
            {
                if ((!PARAM2_FLAG) || (!PARAM3_FLAG) || PARAM4_FLAG)
                {
                    //SCI_TRACE_LOW:"ATC: ATC_ProcessSPEQPARA: <type> or <dest> is invalid."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7132_112_2_17_22_16_44_263,(uint8*)"");
                    return ERR_INVALID_INDEX;
                }

                /*PARA2.num is <type>; 
                   PARA3.num is <dest>*/
                #if !defined(WIN32)
                if (0 == PARA3.num)
                {
                    /*get eq info from ram*/
                    eq_para_ptr = AUDPROC_EXP_GetEqSetFromRam((AUD_PROC_EQPARA_SET_E)PARA2.num);
                }
                else
                {
                    /*get eq info from flash*/
                    eq_para_ptr = AUDPROC_EXP_GetEqSetFromFlash((AUD_PROC_EQPARA_SET_E)PARA2.num);
                }
            #else
            eq_para_ptr = NULL;
            #endif
                if (NULL == eq_para_ptr)
                {
                    //SCI_TRACE_LOW:"ATC: ATC_ProcessSPEQPARA: Get EQ PARA failed."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7154_112_2_17_22_16_44_264,(uint8*)"");
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                buffer_ptr = (uint8*)SCI_ALLOC_BASEZ((sizeof(AUDIO_EQ_STRUCT_T)+1)*2);

                /*convert eq structure data to hex string*/
                ConvertBinToHex((uint8*)eq_para_ptr, sizeof(AUDIO_EQ_STRUCT_T), buffer_ptr);

                sprintf((char*)g_rsp_str, "%s: %s", 
                              g_atc_info_table[AT_CMD_SPEQPARA].cmd_name->str_ptr,  
                              buffer_ptr);

                SCI_FREE(buffer_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
        
            case 2:     //set eq para info
            {
                if ((!PARAM2_FLAG) || (!PARAM3_FLAG) || (!PARAM4_FLAG))
                {
                    //SCI_TRACE_LOW:"ATC: ATC_ProcessSPEQPARA: <type> or <dest> or <data> is invalid."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7176_112_2_17_22_16_44_265,(uint8*)"");
                    return ERR_INVALID_INDEX;
                }

                /*<data> type must be string*/
                if (ATC_CMD_PARAM_TYPE_STRING != PARAM4_TYPE)
                {
                    //SCI_TRACE_LOW:"ATC: ATC_ProcessSPEQPARA: <data> type is not string."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7183_112_2_17_22_16_44_266,(uint8*)"");
                    return ERR_INVALID_INDEX;
                }
                /*check <data> string length, <data> must be hex string*/
                if (PARA4.str_ptr->str_len != (sizeof(AUDIO_EQ_STRUCT_T)*2))
                {
                    //SCI_TRACE_LOW:"ATC: ATC_ProcessSPEQPARA: <data> length:%d != %d*2."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7189_112_2_17_22_16_44_267,(uint8*)"dd", PARA4.str_ptr->str_len, sizeof(AUDIO_EQ_STRUCT_T));
                    return ERR_INVALID_INDEX;
                }
                eq_para_ptr = (AUDIO_EQ_STRUCT_T*)SCI_ALLOC_BASEZ(sizeof(AUDIO_EQ_STRUCT_T));

                /*convert hex string to bin*/
                if (!ConvertHexToBin(PARA4.str_ptr->str_ptr,(uint16)PARA4.str_ptr->str_len, (uint8*)eq_para_ptr))
                {
                    //SCI_TRACE_LOW:"ATC: ATC_ProcessSPEQPARA: ConvertHexToBin failed."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7197_112_2_17_22_16_44_268,(uint8*)"");
                    SCI_FREE(eq_para_ptr);
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                /*PARA2.num is <type>; 
                   PARA3.num is <dest>*/
                #if !defined(WIN32)
                if (0 == PARA3.num)
                {
                    /*set eq info to ram*/
                    set_result = AUDPROC_EXP_SetEqSetToRam((AUD_PROC_EQPARA_SET_E)PARA2.num, eq_para_ptr);
                }
                else
                {
                    /*set eq info to flash*/
                    set_result = AUDPROC_EXP_SetEqSetToFlash((AUD_PROC_EQPARA_SET_E)PARA2.num, eq_para_ptr);
                }
            #else
            set_result = AUDIO_NO_ERROR;
                #endif
                SCI_FREE(eq_para_ptr);
                if (AUDIO_NO_ERROR != set_result)
                {
                    //SCI_TRACE_LOW:"ATC: ATC_ProcessSPEQPARA: Set EQ PARA failed."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7221_112_2_17_22_16_44_269,(uint8*)"");
                    return ERR_OPERATION_NOT_ALLOWED;
                }
                break;
            }
        
            default:     
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
        }
    }
    else
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }
#endif // MS_OAPI_SUPPORT
    return S_ATC_SUCCESS;
}


/*****************************************************************************/
//  Description : This function handles the AT+SFTIN command.
//                          to set at+sftin  paraments
//  Global resource dependence : none
//  Author:       Shuyan.hu
//    modify: longting.zhao 2011.9.6 check PARAM1_TYPE before using PARA1
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSFTIN )
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    uint8             *tem_ptr = NULL;
    uint16             param_len = 0;
    uint8             *ftinput_ptr= NULL;
    uint8             *fstr_ptr = NULL;
    uint8              fstr_len = 0;

    //SCI_TRACE_LOW:"ATC:ATC_ProcessFTIN!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7256_112_2_17_22_16_44_270,(uint8*)"");
    
    if (!PARAM1_FLAG )
    {
        return ERR_INVALID_INDEX;
    }

    if (ATC_CMD_PARAM_TYPE_STRING != PARAM1_TYPE)
    {
            //SCI_TRACE_LOW:"ATC: ATC_ProcessSFTIN not string."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7265_112_2_17_22_16_44_271,(uint8*)"");
            return ERR_INVALID_INDEX;
    }
        
    //get the at cmd parameters 
    ftinput_ptr = (uint8*)SCI_ALLOC_BASEZ(ATC_SHARP_LEN + 1);
    
    param_len = PARA1.str_ptr->str_len;
    tem_ptr  = PARA1.str_ptr->str_ptr;
    
    switch (ATC_GET_CMD_TYPE)
    {
        //handle AT +SFTIN cmd set 
        case ATC_CMD_TYPE_SET:
        {
            fstr_ptr = &ftinput_ptr[0];

            //add message header
            sprintf((char*)fstr_ptr, "%s", "atestf");
            fstr_len = strlen((char *)fstr_ptr);
            fstr_ptr += fstr_len;
            
            if(ATC_SHARP_LEN < (fstr_len + param_len))
            {

                status = ERR_OPERATION_NOT_SUPPORTED;
                break;
            }  
            strncpy((char*)fstr_ptr, (char*)tem_ptr, param_len);
            fstr_len += param_len;
            ftinput_ptr[fstr_len] = '\0'; 

     #ifndef _ATC_ONLY
        //at to mmi handle         
         #ifdef SYNCTOOL_SUPPORT
            ATC_ExtCmd(ftinput_ptr, fstr_len);
         #endif
     #endif

            status = S_ATC_DEFAULT_HANDLE;
            break;
        }
    
        //test AT+SFTIN cmd
        case ATC_CMD_TYPE_TEST:
        {
            status = S_ATC_SUCCESS;
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC_ProcessSFTIN: the operation type error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7317_112_2_17_22_16_44_272,(uint8*)"");
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
        }
    }
    SCI_FREE(ftinput_ptr);
    return status;
    
}


/*****************************************************************************/
//  Description : This function handles the AT+SYTIN command.
//                          to set at+sytin paraments
//  Global resource dependence : 
//  Author:       Shuyan.hu
//    modify: longting.zhao 2011.9.6 check PARAM1_TYPE before using PARA1
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSYTIN )
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    uint8             *tem_ptr = NULL;
    uint16             param_len = 0;
    uint8             *ytinput_ptr= NULL;
    uint8             *ystr_ptr = NULL;
    uint8              ystr_len = 0;

    //SCI_TRACE_LOW:"ATC:ATC_ProcessSYTIN!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7342_112_2_17_22_16_44_273,(uint8*)"");
    
    if (!PARAM1_FLAG )
    {
        return ERR_INVALID_INDEX;
    }

  if (ATC_CMD_PARAM_TYPE_STRING != PARAM1_TYPE)
    {
            //SCI_TRACE_LOW:"ATC: ATC_ProcessSYTIN not string."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7351_112_2_17_22_16_44_274,(uint8*)"");
            return ERR_INVALID_INDEX;
    }
    //get the at cmd parameters 
    ytinput_ptr = (uint8*)SCI_ALLOC_BASEZ(ATC_SHARP_LEN + 1);
    
    param_len = PARA1.str_ptr->str_len;
    tem_ptr  = PARA1.str_ptr->str_ptr;
    switch (ATC_GET_CMD_TYPE)
    {
        //handle AT +SYTIN cmd set 
        case ATC_CMD_TYPE_SET:
        {
            ystr_ptr = &ytinput_ptr[0];

            //add message header
            sprintf((char*)ystr_ptr, "%s", "atests");
            ystr_len = strlen((char *)ystr_ptr);
            ystr_ptr += ystr_len;
            
            if(ATC_SHARP_LEN < (ystr_len + param_len))
            {
                status = ERR_OPERATION_NOT_SUPPORTED;
                break;
            } 
                    
            strncpy((char*)ystr_ptr, (char*)tem_ptr, param_len);
            ystr_len += param_len;
            ytinput_ptr[ystr_len] = '\0'; 

     #ifndef _ATC_ONLY
            //at to mmi handle
        #ifdef SYNCTOOL_SUPPORT
            ATC_ExtCmd(ytinput_ptr, ystr_len);
        #endif
     #endif

            status = S_ATC_DEFAULT_HANDLE;
            break;
        }
    
        //test AT+SYTIN cmd
        case ATC_CMD_TYPE_TEST:
        {
            status = S_ATC_SUCCESS;
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC_ProcessSYTIN: the operation type error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7399_112_2_17_22_16_44_275,(uint8*)"");
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
        }
    }
    
    SCI_FREE(ytinput_ptr);
    return status;
}



/*****************************************************************************/
//  Description : This function handle the AT+SPREF command 
//                 Automatic downloader
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPREF)
{   
#ifndef MS_OAPI_SUPPORT
    if(ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
 
    if(PARAM1_FLAG && (PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING))
    {   
        if(!REF_HandleATCommand(PARA1.str_ptr->str_ptr))
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
    }
    else
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
#endif
    return S_ATC_SUCCESS;        
}
/*****************************************************************************/
//  Description : This function handle the AT+SPENHA command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPENHA )
{
    ATC_STATUS        status = S_ATC_SUCCESS;
 #ifndef WIN32
    uint8             type = 0;
    uint8             enha_type = 0;
    uint32            mode_index = 0;
    uint32            dev_number = 0;
    AUDIO_ENHA_EQ_STRUCT_T*  audenha_eq_ptr = NULL;

    AUDIO_ENHA_TUNABLE_EQ_STRUCT_T* audenha_tun_eq_ptr = PNULL;
    uint8   *str_ptr = PNULL;

    if (ATC_GET_CMD_TYPE != ATC_CMD_TYPE_SET)
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    if(PARAM1_FLAG)
    {
        type = (uint8)PARA1.num;
    }
    else
    {
        return ERR_INVALID_INDEX;
    }

    if(PARAM2_FLAG)
    {
        enha_type = (uint8)PARA2.num;
    }
    else
    {
        return ERR_INVALID_INDEX;
    }

    dev_number = AUDENHA_EXP_GetEqParaSetNum((BOOLEAN)type);

    switch(enha_type)
    {
        case ATC_GET_ENHA_MODE_NUM:
        {
            sprintf((char*)g_rsp_str, "%s: %ld", g_atc_info_table[AT_CMD_SPENHA].cmd_name->str_ptr, dev_number);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
        
        case ATC_GET_ENHA_MODE_DATA:
        {
            if(PARAM3_FLAG)
            {
                if((PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) && ((uint32)PARA3.num <= dev_number) && ((uint32)PARA3.num > 0))
                {
                    mode_index = (uint32)PARA3.num;

                    if(0 == type)
                    {
                        //SCI_TRACE_LOW:"ATC: spenha eq mode_index = %d, dev_number = %d, %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7499_112_2_17_22_16_45_276,(uint8*)"ddd", mode_index, dev_number, sizeof(AUDIO_ENHA_EQ_STRUCT_T));
                        if(mode_index <= dev_number)
                        {
                            audenha_eq_ptr = (AUDIO_ENHA_EQ_STRUCT_T*)SCI_ALLOC_BASEZ(sizeof(AUDIO_ENHA_EQ_STRUCT_T));
                            SCI_ASSERT(audenha_eq_ptr != PNULL); /*assert verified*/
                            str_ptr = (uint8*)SCI_ALLOC_BASEZ((sizeof(AUDIO_ENHA_EQ_STRUCT_T)+1)*2);
                            SCI_ASSERT(str_ptr != PNULL); /*assert verified*/

                            if(AUDIO_NO_ERROR != AUDENHA_EXP_GetEqSetFromRam(mode_index, audenha_eq_ptr))
                            {
                                //SCI_TRACE_LOW:"AGC_Get_Param failed to get mode_index, mode_index :%d."
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7509_112_2_17_22_16_45_277,(uint8*)"d", mode_index);
                                SCI_FREE(audenha_eq_ptr);
                                SCI_FREE(str_ptr);
                                return ERR_NOT_FOUND;
                            }

                            ConvertBinToHex((uint8*)(audenha_eq_ptr), sizeof(AUDIO_ENHA_EQ_STRUCT_T), str_ptr);

                            sprintf((char*)g_rsp_str, "%s: %ld,%s",
                                        g_atc_info_table[AT_CMD_SPENHA].cmd_name->str_ptr,
                                        mode_index,
                                        str_ptr);
                            SCI_FREE(audenha_eq_ptr);
                            SCI_FREE(str_ptr);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }
                    }
                    else if(mode_index <= dev_number)
                    {
                        //SCI_TRACE_LOW:"ATC: spenha tun_eq mode_index = %d, dev_number = %d, %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7528_112_2_17_22_16_45_278,(uint8*)"ddd", mode_index, dev_number, sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T));
                        
                        audenha_tun_eq_ptr = (AUDIO_ENHA_TUNABLE_EQ_STRUCT_T*)SCI_ALLOC_BASEZ(sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T));
                        SCI_ASSERT(audenha_tun_eq_ptr != PNULL); /*assert verified*/

                        str_ptr = (uint8*)SCI_ALLOC_BASEZ((sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T)+1)*2);
                        SCI_ASSERT(str_ptr != PNULL); /*assert verified*/

                        if(AUDIO_NO_ERROR != AUDENHA_EXP_GetTunableEqSetFromRam(mode_index, audenha_tun_eq_ptr))
                        {
                            //SCI_TRACE_LOW:"AGC_Get_Param failed to get mode_index, mode_index :%d."
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7538_112_2_17_22_16_45_279,(uint8*)"d", mode_index);
                            SCI_FREE(audenha_tun_eq_ptr);
                            SCI_FREE(str_ptr);
                            return ERR_NOT_FOUND;
                        }

                        ConvertBinToHex((uint8*)(audenha_tun_eq_ptr), sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T), str_ptr);

                        sprintf((char*)g_rsp_str, "%s: %ld,%s",
                                g_atc_info_table[AT_CMD_SPENHA].cmd_name->str_ptr,
                                mode_index,
                                str_ptr);
                        SCI_FREE(audenha_tun_eq_ptr);
                        SCI_FREE(str_ptr);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
        
        case ATC_SET_ENHA_MODE_DATA:
        {
            if(PARAM3_FLAG && PARAM4_FLAG)
            {
                if (((PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) && ((uint32)PARA3.num <= dev_number)&& ((uint32)PARA3.num > 0))
                      &&
                    ((PARAM4_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) && ((uint32)PARA4.num <= AUD_ENHA_EQMODE_SEL_SOFTROCK) && ((uint32)PARA4.num > 0))
                    )
                {
                    s_atc_admm_index = (uint8)PARA3.num;
                    s_enha_eqmode_index = (uint8)PARA4.num;
                    /* output the '>' */
                    sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
#ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                    {
                        /* set the terminate char to receive PDU from the SIO */
                        SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    }
                    else
                    {
                        ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
                        MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                        /* set the terminate char to receive PDU from the SIO */
                    }
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;

                    if(0 == type)
                    {
                        atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_ENHA;
                    }
                    else
                    {
                        atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_ENHA_TUN;
                    }
#else
                    /* set the terminate char to receive PDU from the SIO */
                    SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    atc_config_ptr->is_ctrlz_end = TRUE;

                    if(0 == type)
                    {
                        atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_ENHA;
                    }
                    else
                    {
                        atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_ENHA_TUN;
                    }
#endif
                    status = S_ATC_DEFAULT_HANDLE;
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_GET_ENHA_MODE_NV_DATA:
        {
            if ((!PARAM3_FLAG) ||
                (!(PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC && (uint32)PARA3.num <= dev_number && (uint32)PARA3.num > 0)))
            {
                status = ERR_INVALID_INDEX;
                break;
            }
            mode_index = (uint32)PARA3.num;

            if(0 == type)
            {
                //SCI_TRACE_LOW:"ATC: spenha eq mode_index = %d, dev_number = %d, %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7644_112_2_17_22_16_45_280,(uint8*)"ddd", mode_index, dev_number, sizeof(AUDIO_ENHA_EQ_STRUCT_T));

                audenha_eq_ptr = (AUDIO_ENHA_EQ_STRUCT_T*)SCI_ALLOC_BASEZ(sizeof(AUDIO_ENHA_EQ_STRUCT_T));
                SCI_ASSERT(audenha_eq_ptr != PNULL); /*assert verified*/
                str_ptr = (uint8*)SCI_ALLOC_BASEZ((sizeof(AUDIO_ENHA_EQ_STRUCT_T)+1)*2);
                SCI_ASSERT(str_ptr != PNULL); /*assert verified*/

                if(AUDIO_NO_ERROR != AUDENHA_EXP_GetEqSetFromFlash(mode_index, audenha_eq_ptr))
                {
                    //SCI_TRACE_LOW:"AGC_Get_Param failed to get mode_index, mode_index:%d."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7653_112_2_17_22_16_45_281,(uint8*)"d", mode_index);
                    SCI_FREE(audenha_eq_ptr);
                    SCI_FREE(str_ptr);
                    return ERR_NOT_FOUND;
                }

                ConvertBinToHex((uint8*)(audenha_eq_ptr), sizeof(AUDIO_ENHA_EQ_STRUCT_T), str_ptr);

                sprintf((char*)g_rsp_str, "%s: %ld,%s",
                        g_atc_info_table[AT_CMD_SPENHA].cmd_name->str_ptr,
                        mode_index,
                        str_ptr);
                SCI_FREE(audenha_eq_ptr);
                SCI_FREE(str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: spenha tun_eq mode_index = %d, dev_number = %d, %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7671_112_2_17_22_16_45_282,(uint8*)"ddd", mode_index, dev_number, sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T));

                audenha_tun_eq_ptr = (AUDIO_ENHA_TUNABLE_EQ_STRUCT_T*)SCI_ALLOC_BASEZ(sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T));
                SCI_ASSERT(audenha_tun_eq_ptr != PNULL); /*assert verified*/

                str_ptr = (uint8*)SCI_ALLOC_BASEZ((sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T)+1)*2);
                SCI_ASSERT(str_ptr != PNULL); /*assert verified*/

                if(AUDIO_NO_ERROR != AUDENHA_EXP_GetTunableEqSetFromFlash(mode_index, audenha_tun_eq_ptr))
                {
                    //SCI_TRACE_LOW:"AGC_Get_Param failed to get mode_index, mode_index:%d."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7681_112_2_17_22_16_45_283,(uint8*)"d", mode_index);
                    SCI_FREE(audenha_tun_eq_ptr);
                    SCI_FREE(str_ptr);
                    return ERR_NOT_FOUND;
                }

                ConvertBinToHex((uint8*)(audenha_tun_eq_ptr), sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T), str_ptr);

                sprintf((char*)g_rsp_str, "%s: %ld,%s",
                        g_atc_info_table[AT_CMD_SPENHA].cmd_name->str_ptr,
                        mode_index,
                        str_ptr);
                SCI_FREE(audenha_tun_eq_ptr);
                SCI_FREE(str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            break;
        }
        
        case ATC_SET_ENHA_MODE_NV_DATA:
        {
            if(PARAM3_FLAG)
            {
                if ((PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) && ((uint32)(PARA3.num) <= dev_number) && ((uint32)(PARA3.num) > 0))
                {
                    s_atc_admm_index = (uint8)PARA3.num;
                    /* output the '>'*/
                    sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
#ifdef _MUX_ENABLE_
                    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                    {
                        /* set the terminate char to receive PDU from the SIO*/
                        SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    }
                    else
                    {
                        ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
                        MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                        /* set the terminate char to receive PDU from the SIO */
                    }
                    atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;

                    if(0 == type)
                    {
                        atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_ENHANV;
                    }
                    else
                    {
                        atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status = ATC_CTRLZ_ENHANV_TUN;
                    }
#else
                    /* set the terminate char to receive PDU from the SIO */
                    SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
                    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                    atc_config_ptr->is_ctrlz_end = TRUE;

                    if(0 == type)
                    {
                        atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_ENHANV;
                    }
                    else
                    {
                        atc_config_ptr->ctrlz_end_status = ATC_CTRLZ_ENHANV_TUN;
                    }
#endif
                    status = S_ATC_DEFAULT_HANDLE;
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }
        
        default:
        {
            SCI_ASSERT(FALSE); /*assert verified*/
            break;
        }
    }
#endif
    return status;
}

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPENHA
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioENHA_NVData(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *data_ptr,  // pointer to the data
    uint16          param_len
)
{
#ifndef WIN32
    uint8             terminater_char;     // indicates ther terminater char
    BOOLEAN           flag = FALSE;
    uint32            dev_number = 0;
    uint32            mode_index = 0;
    
    AUDIO_RESULT_E   result;
    AUDIO_ENHA_EQ_STRUCT_T*  audenha_eq_ptr = PNULL;
    
    
    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    
    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len             -= 1;


    audenha_eq_ptr = (AUDIO_ENHA_EQ_STRUCT_T *)SCI_ALLOC_BASEZ(sizeof(AUDIO_ENHA_EQ_STRUCT_T)+1);
    SCI_ASSERT(NULL != audenha_eq_ptr); /*assert verified*/

    //SCI_TRACE_LOW:"ATC: siospenha for enha, param_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7802_112_2_17_22_16_45_284,(uint8*)"d",param_len);

    switch (terminater_char)
    {
    case ESC:
        flag = TRUE;
        break;

    case CTRL_Z:
        if(param_len == sizeof(AUDIO_ENHA_EQ_STRUCT_T)*2)
        {
            dev_number = AUDENHA_EXP_GetEqParaSetNum(FALSE);
            if(s_atc_admm_index <= dev_number)
            {
                ConvertHexToBin(data_ptr, param_len, (uint8*)audenha_eq_ptr);

                mode_index = s_atc_admm_index;

                result = AUDENHA_EXP_SetEqSetToFlash(mode_index, audenha_eq_ptr);

                if(result != AUDIO_NO_ERROR)
                {
                    //SCI_TRACE_LOW:"ATC: siospenha for enha, set result = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7824_112_2_17_22_16_45_285,(uint8*)"d",result);
                    flag = FALSE;
                }
                else
                {
                    flag = TRUE;
                }
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: siospenha for enha, input data index error."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7834_112_2_17_22_16_45_286,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"ATC: siospenha for enha, input data length error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7839_112_2_17_22_16_45_287,(uint8*)"");
        }

        break;
    default:
           //SCI_TRACE_LOW:"Lost the escape character is %s is line %d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7844_112_2_17_22_16_45_288,(uint8*)"sd",__FILE__,__LINE__);
        break;
    }

    SCI_FREE(audenha_eq_ptr);

    if(flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
    }
#endif
    return S_ATC_DEFAULT_HANDLE;

}

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPENHA
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioENHA_Tun_NVData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                )
{
#ifndef WIN32
    uint8             terminater_char;     // indicates ther terminater char
    BOOLEAN           flag = FALSE;
    uint32            dev_number = 0;
    uint32            mode_index = 0;
    AUDIO_RESULT_E   result;
    
    AUDIO_ENHA_TUNABLE_EQ_STRUCT_T* audenha_tun_eq_ptr = PNULL;


    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len             -= 1;


    audenha_tun_eq_ptr = (AUDIO_ENHA_TUNABLE_EQ_STRUCT_T *)SCI_ALLOC_BASEZ(sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T)+1);
    SCI_ASSERT(NULL != audenha_tun_eq_ptr); /*assert verified*/

    //SCI_TRACE_LOW:"ATC: siospenha for enha tun, param_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7894_112_2_17_22_16_45_289,(uint8*)"d",param_len);

    switch (terminater_char)
    {
    case ESC:
        flag = TRUE;
        break;

    case CTRL_Z:
        if(param_len == sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T)*2)
        {
            dev_number = AUDENHA_EXP_GetEqParaSetNum(TRUE);
            if(s_atc_admm_index <= dev_number)
            {
                ConvertHexToBin(data_ptr, param_len, (uint8*)audenha_tun_eq_ptr);

                mode_index = s_atc_admm_index;

                result = AUDENHA_EXP_SetTunableEqSetToFlash(mode_index, audenha_tun_eq_ptr);

                if(result != AUDIO_NO_ERROR)
                {
                    //SCI_TRACE_LOW:"ATC: siospenha for enha tun, set result = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7916_112_2_17_22_16_45_290,(uint8*)"d",result);
                    flag = FALSE;
                }
                else
                {
                    flag = TRUE;
                }

            }
            else
            {
                //SCI_TRACE_LOW:"ATC: siospenha for enha tun, input data index error."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7927_112_2_17_22_16_45_291,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"ATC: siospenha for enha tun, input data length error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7932_112_2_17_22_16_45_292,(uint8*)"");
        }

        break;
    default:
           //SCI_TRACE_LOW:"Lost the escape character is %s is line %d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7937_112_2_17_22_16_45_293,(uint8*)"sd",__FILE__,__LINE__);
        break;
    }

    SCI_FREE(audenha_tun_eq_ptr);

    if(flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
    }
#endif
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPENHA
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioENHA_Data(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                )
{
#ifndef WIN32
    uint8             terminater_char;     // indicates ther terminater char
    BOOLEAN           flag = FALSE;
    uint32            dev_number = 0;
    AUDIO_RESULT_E   result;
    AUDIO_ENHA_EQ_STRUCT_T*  audenha_eq_ptr = NULL;
    uint32            enha_mode_index = 0;
    uint32            mode_index = 0;


    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len             -= 1;


    audenha_eq_ptr = (AUDIO_ENHA_EQ_STRUCT_T *)SCI_ALLOC_BASEZ(sizeof(AUDIO_ENHA_EQ_STRUCT_T)+1);
    SCI_ASSERT(NULL != audenha_eq_ptr); /*assert verified*/

    //SCI_TRACE_LOW:"ATC: siospenha for enha, param_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7986_112_2_17_22_16_46_294,(uint8*)"d",param_len);

    switch (terminater_char)
    {
    case ESC:
        flag = TRUE;
        break;

    case CTRL_Z:
        //SCI_TRACE_LOW:"ATC: sizeof(AUDIO_ENHA_EQ_STRUCT_T)=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_7995_112_2_17_22_16_46_295,(uint8*)"d", sizeof(AUDIO_ENHA_EQ_STRUCT_T));
        if(param_len == sizeof(AUDIO_ENHA_EQ_STRUCT_T)*2)
        {
            dev_number = AUDENHA_EXP_GetEqParaSetNum(FALSE);
            if(s_atc_admm_index <= dev_number)
            {
                ConvertHexToBin(data_ptr, param_len, (uint8*)audenha_eq_ptr);

                mode_index = s_atc_admm_index;
                enha_mode_index = s_enha_eqmode_index;

                result = AUDENHA_EXP_SetEqSetToRam(mode_index, audenha_eq_ptr, enha_mode_index);

                if(result != AUDIO_NO_ERROR)
                {
                    //SCI_TRACE_LOW:"ATC: siospenha for enha, set result = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8010_112_2_17_22_16_46_296,(uint8*)"d",result);
                    flag = FALSE;
                }
                else
                {
                    flag = TRUE;
                }
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: siospenha for enha, input data index error."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8020_112_2_17_22_16_46_297,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"ATC: siospenha for enha, input data length error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8025_112_2_17_22_16_46_298,(uint8*)"");
        }

        break;
    default:
           //SCI_TRACE_LOW:"Lost the escape character is %s is line %d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8030_112_2_17_22_16_46_299,(uint8*)"sd",__FILE__,__LINE__);
        break;
    }

    SCI_FREE(audenha_eq_ptr);

    if (flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
    }
#endif
    return S_ATC_DEFAULT_HANDLE;

}

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPENHA
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioENHA_Tun_Data(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                )
{
#ifndef WIN32
    uint8             terminater_char;     // indicates ther terminater char
    BOOLEAN           flag = FALSE;
    uint32            dev_number = 0;
    uint32            mode_index = 0;
    uint32            enha_mode_index = 0;
    
    AUDIO_RESULT_E   result;

    AUDIO_ENHA_TUNABLE_EQ_STRUCT_T* audenha_tun_eq_ptr = PNULL;


    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len             -= 1;


    audenha_tun_eq_ptr = (AUDIO_ENHA_TUNABLE_EQ_STRUCT_T *)SCI_ALLOC_BASEZ(sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T)+1);
    SCI_ASSERT(NULL != audenha_tun_eq_ptr); /*assert verified*/

    //SCI_TRACE_LOW:"ATC: siospenha for enha tun, param_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8082_112_2_17_22_16_46_300,(uint8*)"d",param_len);

    switch (terminater_char)
    {
    case ESC:
        flag = TRUE;
        break;

    case CTRL_Z:
        //SCI_TRACE_LOW:"ATC: sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T)=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8091_112_2_17_22_16_46_301,(uint8*)"d", sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T));
        if(param_len == sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T)*2)
        {
            dev_number = AUDENHA_EXP_GetEqParaSetNum(TRUE);
            if(s_atc_admm_index <= dev_number)
            {
                ConvertHexToBin(data_ptr, param_len, (uint8*)audenha_tun_eq_ptr);

                mode_index = s_atc_admm_index;
                enha_mode_index = s_enha_eqmode_index;

                result = AUDENHA_EXP_SetTunableEqSetToRam(mode_index, audenha_tun_eq_ptr, enha_mode_index);

                if(result != AUDIO_NO_ERROR)
                {
                    //SCI_TRACE_LOW:"ATC: siospenha for enha tun, set result = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8106_112_2_17_22_16_46_302,(uint8*)"d",result);
                    flag = FALSE;
                }
                else
                {
                    flag = TRUE;
                }

            }
            else
            {
                //SCI_TRACE_LOW:"ATC: siospenha for enha tun, input data index error."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8117_112_2_17_22_16_46_303,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"ATC: siospenha for enha tun, input data length error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8122_112_2_17_22_16_46_304,(uint8*)"");
        }

        break;
    default:
           //SCI_TRACE_LOW:"Lost the escape character is %s is line %d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8127_112_2_17_22_16_46_305,(uint8*)"sd",__FILE__,__LINE__);
        break;
    }

    SCI_FREE(audenha_tun_eq_ptr);

    if(flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
    }
#endif
    return S_ATC_DEFAULT_HANDLE;

}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function is used to chech the time string.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckTimeStr(
                    uint8      length,
                    uint8      *str,
                    SCI_DATE_T   *date_ptr,
                    SCI_TIME_T   *tm_ptr
                    )
{
    int32            i;
      
    if (AT_CCLK_STR_LEN !=length)
    {
        //SCI_TRACE_LOW:"ATC: timer string len error %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8232_112_2_17_22_16_46_306,(uint8*)"d", length);
        return FALSE;
    }

    // YY/MM/DD,HH:MM:SS
    if(str[2]  != '/'
        || str[5]  != '/'
        || str[8]  != ','
        || str[11] != ':' 
        || str[14] != ':')
    {
        //SCI_TRACE_LOW:"ATC: timer string format error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8243_112_2_17_22_16_46_307,(uint8*)"");
        return FALSE;
    }

    // Check year string
    for (i=0; i<2; i++)
    {
        if (*str > '9' || *str < '0')
        {
            //SCI_TRACE_LOW:"ATC: value of year error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8252_112_2_17_22_16_46_308,(uint8*)"");
            return FALSE;
        }
    }
    date_ptr->year = 2000 + (str[0] - '0') * 10 + (str[1] - '0');

    // check month string

    if ((str[3] > '1' || str[3] < '0') &&
        (str[4] > '9' || str[4] < '0'))
    {
        //SCI_TRACE_LOW:"ATC: month digital error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8263_112_2_17_22_16_46_309,(uint8*)"");
        return FALSE;
    }
    date_ptr->mon = (str[3] - '0') * 10 + (str[4] - '0');
    if (date_ptr->mon > AT_MAX_MONTH)
    {
        //SCI_TRACE_LOW:"ATC: month value error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8269_112_2_17_22_16_46_310,(uint8*)"");
        return FALSE;
    }

    // Check day string
    date_ptr->mday = (str[6] - '0') * 10 + (str[7] - '0');
    if (date_ptr->mday > AT_MAX_DAY )
    {
        //SCI_TRACE_LOW:"ATC: day value error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8277_112_2_17_22_16_46_311,(uint8*)"");
        return FALSE;
    }

    // check hour string
    tm_ptr->hour = (str[9] - '0') * 10 + (str[10] - '0');
    if (tm_ptr->hour > AT_MAX_HOUR)
    {
        //SCI_TRACE_LOW:"ATC: hour value error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8285_112_2_17_22_16_46_312,(uint8*)"");
        return FALSE;
    }

    // check minute string
    tm_ptr->min = (str[12] - '0') * 10 + (str[13] - '0');
    if (tm_ptr->min > AT_MAX_MINUTE )
    {
        //SCI_TRACE_LOW:"ATC: minute value error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8293_112_2_17_22_16_46_313,(uint8*)"");
        return FALSE;
    }

    // check second string
    // check minute string
    tm_ptr->sec = (str[15] - '0') * 10 + (str[16] - '0');
    if (tm_ptr->sec > AT_MAX_MINUTE )
    {
        //SCI_TRACE_LOW:"ATC: second value error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8302_112_2_17_22_16_46_314,(uint8*)"");
        return FALSE;
    }

    return TRUE;    
}


/*****************************************************************************/
//  Description : This function is used to restore the configuration profile.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void RestoreConfig(
                ATC_CONFIG_T   *atc_config_ptr
                )
{
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    // set the following parameter to the defualt value
    atc_config_ptr->atc_config_nv.creg       = DEF_CREG;
    atc_config_ptr->atc_config_nv.e_value    = FALSE;
    atc_config_ptr->atc_config_nv.clip_flag  = TRUE;
    atc_config_ptr->atc_config_nv.clir_flag  = FALSE;
    atc_config_ptr->atc_config_nv.colp_flag  = FALSE;
    atc_config_ptr->atc_config_nv.cmee       = NUMERIC_ERR_VALUE_MODE;
    atc_config_ptr->atc_config_nv.cmgf_flag  = FALSE;
    atc_config_ptr->atc_config_nv.sidet_flag = FALSE;
    atc_config_ptr->atc_config_nv.wind_ind_level            = ATC_SIND_DEFAULT_VAL;
    //The following is about STK
    atc_config_ptr->atc_config_nv.stk_actflag   = TRUE;
    atc_config_ptr->atc_config_nv.stk_time      = ATC_STKTIME_DEFAULT_VAL;
#if 0
#ifndef _ULTRA_LOW_CODE_
    ATC_CheckAndHandleConfig(atc_config_ptr,g_stk_terminal_profile[AT_STK_TRP_MAX_VAL]);
#endif /* _ULTRA_LOW_CODE_ */
#endif
    strcpy((char*)atc_config_ptr->cscs, "GSM");
    ATC_RestoreSmsConfig(dual_sys);
    // Save to the NV ram.
    ATC_WriteNVItem(atc_config_ptr);
#ifdef _MUX_ENABLE_
    ATC_SetEcho(atc_config_ptr->atc_config_nv.e_value);
    SIO_ATC_SetEcho(atc_config_ptr->atc_config_nv.e_value);
#else
    SIO_ATC_SetEcho(atc_config_ptr->atc_config_nv.e_value);
#endif
}

/*****************************************************************************/
//  Description : This function is used to save the current configuration profile.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void SaveConfigProfile(
                ATC_CONFIG_T   *atc_config_ptr
                )
{
    //SCI_TRACE_LOW:"ATC: save ate value: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8357_112_2_17_22_16_46_315,(uint8*)"d",atc_config_ptr->atc_config_nv.e_value);
    // Save to the NV ram.
    ATC_WriteNVItem(atc_config_ptr);
}
#endif /* _ULTRA_LOW_CODE_ */


#ifdef AUDIO_PARAM_ARM
/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SADM
//  Global resource dependence : g_rsp_str
//  Author: chong.liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioARMSADMData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                )
{
    uint8             terminater_char;     // indicates ther terminater char
    BOOLEAN           flag = FALSE;
    uint32            dev_number = 0;

    AUDIO_NV_ARM_RESULT_E   result = AUDIO_NV_ARM_NO_ERROR;

    #ifdef AUDIO_PARAM_ARM   
    AUDIO_RESULT_E set_devmode_result;
    #endif

    AUDIO_NV_ARM_MODE_INFO_T* audmod_arm_devmod_ptr = PNULL;
    char *name_ptr = PNULL;
    uint8  name_len = 0;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/

    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len             -= 1;
    
    audmod_arm_devmod_ptr = (AUDIO_NV_ARM_MODE_INFO_T *)SCI_ALLOC_BASEZ(sizeof(AUDIO_NV_ARM_MODE_INFO_T)+1);

    //SCI_TRACE_LOW:"ATC: siosadm for arm, param_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8397_112_2_17_22_16_46_316,(uint8*)"d",param_len);

    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
            flag = TRUE;
            break;

        case CTRL_Z:
        {
            if(param_len == sizeof(AUDIO_NV_ARM_MODE_STRUCT_T)*2)
            {
                dev_number = AUDIONVARM_GetModeNumber();
                if(s_atc_admm_index < dev_number)
                {
                    name_ptr = AUDIONVARM_GetAudioModeName((AUDIO_DEVICE_MODE_TYPE_E)s_atc_admm_index);

                    SCI_ASSERT(name_ptr != NULL);/*assert verified check NULL pointer*/
                    ConvertHexToBin(data_ptr, param_len, (uint8*)&audmod_arm_devmod_ptr->tAudioNvArmModeStruct);

                    //SCI_TRACE_LOW:"ATC:ATC_ProcessSioARMSADMData %s, %s, %d."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8418_112_2_17_22_16_46_317,(uint8*)"ssd",audmod_arm_devmod_ptr->ucModeName, name_ptr, strlen(name_ptr));

                    name_len = strlen(name_ptr);
                    if (name_len < AUDIO_NV_ARM_MODE_NAME_MAX_LEN)
                    {
                        SCI_MEMCPY(audmod_arm_devmod_ptr->ucModeName, name_ptr, name_len);
                        audmod_arm_devmod_ptr->ucModeName[name_len] = '\0';
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"ATC: Audio Mode Name too long: %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8428_112_2_17_22_16_46_318,(uint8*)"d", name_len);
                        flag = FALSE;
                        break;
                    }

                    //SCI_TRACE_LOW:"ATC:ATC_ProcessSioARMSADMData 2 %s, %s, %d."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8434_112_2_17_22_16_46_319,(uint8*)"ssd",audmod_arm_devmod_ptr->ucModeName, name_ptr, strlen(name_ptr));

                    result = AUDIONVARM_SetModeParam(name_ptr, audmod_arm_devmod_ptr);
                    if(result != AUDIO_NV_ARM_NO_ERROR)
                    {
                        //SCI_TRACE_LOW:"ATC: siosadm for arm, setmode param result = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8439_112_2_17_22_16_46_320,(uint8*)"d",result);
                        flag = FALSE;
                    }
                    else
                    {
                        AUDIO_DEVICE_MODE_TYPE_E eMode = AUDIONVARM_GetAudioModeType(name_ptr);
                        AUDIO_DEVICE_MODE_TYPE_E eOldCurMode = AUDIO_DEVICE_MODE_HANDHOLD;
                        AUDIO_GetDevMode(&eOldCurMode);

                        #ifdef AUDIO_PARAM_ARM   
                            set_devmode_result = AUDIO_SetDevModeEx(INVALID_HANDLE, eMode);
                        #else
                            AUDIO_SetDevMode(eMode);
                        #endif
                        
                        //SCI_TRACE_LOW:"ATC:ATC_ProcessSioARMSADMData: setModeInfo, Mode:0x%x, pucModeName:%s, mode:%d, oldMode:%d."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8455_112_2_17_22_16_46_321,(uint8*)"dsdd", audmod_arm_devmod_ptr, name_ptr, eMode, eOldCurMode);
                        flag = TRUE;
                    }
                    
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC: siosadm for arm , input data index error."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8462_112_2_17_22_16_47_322,(uint8*)"");
                }
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: siosadm for arm , input data length error."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8467_112_2_17_22_16_47_323,(uint8*)"");
            }

            break;
        }
    
        default:
        {
            //SCI_TRACE_LOW:"Lost the escape character is %s is line %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8475_112_2_17_22_16_47_324,(uint8*)"sd",__FILE__,__LINE__);
            break;
        }
    }

    SCI_FREE(audmod_arm_devmod_ptr);    

    if ( flag )
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);             
    }

    return S_ATC_DEFAULT_HANDLE;
}


/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SADM
//  Global resource dependence : g_rsp_str
//  Author:  chong.liu
//  Note: new added for the function to read&write audio parameters into NV
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioARMSADMNVData(               
                      ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                )
{
    uint8        terminater_char;     // indicates ther terminater char
    uint16       data_length;
    BOOLEAN           flag = FALSE;
    uint32            mode_index = 0;
    uint32            dev_number = 0;
    AUDIO_NV_ARM_RESULT_E   result = AUDIO_NV_ARM_NO_ERROR;
    AUDIO_NV_ARM_MODE_INFO_T* audmod_arm_devmod_ptr = PNULL;
    char *name_ptr = PNULL;
    int  name_len = 0;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    
    param_len  -= 1;
    data_length = strlen((char*)data_ptr);
    terminater_char = data_ptr[data_length - 1];
    mode_index = s_atc_admm_index;
    //SCI_TRACE_LOW:"ATC:SioARMSADMNVData:length:%d,terminater_char:%d,mode index::%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8522_112_2_17_22_16_47_325,(uint8*)"ddd",param_len,terminater_char,mode_index);

    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
        {
            flag = TRUE;
            //SCI_TRACE_LOW:"ATC:cancel the snvm audio data!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8530_112_2_17_22_16_47_326,(uint8*)"");
            break;
        }

        case CTRL_Z:
        {
            if(param_len == sizeof(AUDIO_NV_ARM_MODE_STRUCT_T)*2)
            {
                dev_number = AUDIONVARM_GetModeNumber();
                if(s_atc_admm_index < dev_number)
                {
                    name_ptr = AUDIONVARM_GetAudioModeName((AUDIO_DEVICE_MODE_TYPE_E )s_atc_admm_index);
                    
                    audmod_arm_devmod_ptr = (AUDIO_NV_ARM_MODE_INFO_T *)SCI_ALLOC_BASEZ(sizeof(AUDIO_NV_ARM_MODE_INFO_T)+1);

                    //SCI_TRACE_LOW:"ATC:SioARMSADMNVData %s, %s, %d."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8546_112_2_17_22_16_47_327,(uint8*)"ssd",audmod_arm_devmod_ptr->ucModeName, name_ptr, strlen(name_ptr));
                    SCI_ASSERT(name_ptr != NULL);/*assert verified check NULL pointer*/
                    name_len = strlen(name_ptr);
                    if (name_len <= AUDIO_NV_ARM_MODE_NAME_MAX_LEN)
                    {
                        SCI_MEMCPY(audmod_arm_devmod_ptr->ucModeName, name_ptr, name_len);
                        audmod_arm_devmod_ptr->ucModeName[name_len] = '\0';
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"ATC: Audio Mode Name too long: %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8556_112_2_17_22_16_47_328,(uint8*)"d", name_len);
                        flag = FALSE;
                        break;
                    }

                    //SCI_TRACE_LOW:"ATC:SioARMSADMNVData 2 %s, %s, %d."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8562_112_2_17_22_16_47_329,(uint8*)"ssd",audmod_arm_devmod_ptr->ucModeName, name_ptr, strlen(name_ptr));

                    if (!ConvertHexToBin(data_ptr, param_len, (uint8*)&audmod_arm_devmod_ptr->tAudioNvArmModeStruct))
                    {
                        //SCI_TRACE_LOW:"ATC:ConvertHexToBin failed."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8566_112_2_17_22_16_47_330,(uint8*)"");
                        break;
                    }            

                    result = AUDIONVARM_WriteModeParamToFlash((char *)audmod_arm_devmod_ptr->ucModeName, audmod_arm_devmod_ptr);
                    if (AUDMOD_NO_ERROR != result)
                    {
                        //SCI_TRACE_LOW:"ATC:SioARMSADMNVData failed. result:%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8573_112_2_17_22_16_47_331,(uint8*)"d",result);
                        break;
                    }
                    flag = TRUE;  
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC: siosadm for arm , input data index error."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8580_112_2_17_22_16_47_332,(uint8*)"");
                }
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: siosadmnv for arm, input audio data length error.length:%d != %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8586_112_2_17_22_16_47_333,(uint8*)"dd",param_len,sizeof(AUDIO_NV_ARM_MODE_STRUCT_T)*2);
            }
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: snvm audio data format is error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8593_112_2_17_22_16_47_334,(uint8*)"");
            break;
        }
    }

    if (PNULL != audmod_arm_devmod_ptr)
    {
        SCI_FREE(audmod_arm_devmod_ptr);
    }

    if (flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);             
    }

    return S_ATC_DEFAULT_HANDLE;

}

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SADM
//  Global resource dependence : g_rsp_str
//  Author: longting.zhao
//  Note: new added for the function to read&write audio parameters into NV
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioDSPSADMData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                )
{
    uint8             terminater_char;     // indicates ther terminater char
    BOOLEAN           flag = FALSE;
    uint32            dev_number = 0;
    uint32            mode_index = s_atc_admm_index;

    AUDIO_NV_DSP_RESULT_E   result = AUDIO_NV_DSP_NO_ERROR;

    AUDIO_NV_DSP_MODE_INFO_T* audmod_dsp_devmod_ptr = PNULL;
    AUDIO_NV_DSP_MODE_INFO_T * audmod_devmod_ptr = NULL;
    uint8  name_len = 0;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/

    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len             -= 1;
    
    audmod_dsp_devmod_ptr = (AUDIO_NV_DSP_MODE_INFO_T *)SCI_ALLOC_BASEZ(sizeof(AUDIO_NV_DSP_MODE_INFO_T)+1);

    //SCI_TRACE_LOW:"ATC: siosadm for dsp, param_len = %d,terminater_char=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8645_112_2_17_22_16_47_335,(uint8*)"dd",param_len, terminater_char);

    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
            flag = TRUE;
            break;

        case CTRL_Z:
        {
            //SCI_TRACE_LOW:"atc: sizeof(AUDIO_NV_DSP_MODE_STRUCT_T)=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8656_112_2_17_22_16_47_336,(uint8*)"d", sizeof(AUDIO_NV_DSP_MODE_STRUCT_T));
            if(param_len == sizeof(AUDIO_NV_DSP_MODE_STRUCT_T)*2)
            {
                dev_number = AUDIONVDSP_GetModeNumber();
                if(s_atc_admm_index < dev_number)
                {
                    audmod_devmod_ptr = AUDIONVDSP_GetFirstDevMode();
                   
                    while(mode_index)
                    {
                        audmod_devmod_ptr =  AUDIONVDSP_GetNextDevMode(audmod_devmod_ptr);
                        mode_index--;
                    }

                    ConvertHexToBin(data_ptr, param_len, (uint8*)&audmod_dsp_devmod_ptr->tAudioNvDspModeStruct);

                    //SCI_TRACE_LOW:"ATC:ATC_ProcessSioDSPSADMData %s, %d."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8673_112_2_17_22_16_47_337,(uint8*)"sd",audmod_dsp_devmod_ptr->ucModeName, strlen((char *)audmod_dsp_devmod_ptr->ucModeName));

                    name_len = strlen((char *)audmod_devmod_ptr->ucModeName);
                    if (name_len < AUDIO_NV_DSP_MODE_NAME_MAX_LEN)
                    {
                        SCI_MEMCPY(audmod_dsp_devmod_ptr->ucModeName, audmod_devmod_ptr->ucModeName, name_len);
                        audmod_dsp_devmod_ptr->ucModeName[name_len] = '\0';
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"ATC: Audio Mode Name too long: %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8683_112_2_17_22_16_47_338,(uint8*)"d", name_len);
                        flag = FALSE;
                        break;
                    }

                    //SCI_TRACE_LOW:"ATC:ATC_ProcessSioDSPSADMData 2, %s, %d."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8689_112_2_17_22_16_47_339,(uint8*)"sd",audmod_dsp_devmod_ptr->ucModeName, strlen((char *)audmod_dsp_devmod_ptr->ucModeName));

                    result = AUDIONVDSP_SetModeParam((char *)audmod_dsp_devmod_ptr->ucModeName, audmod_dsp_devmod_ptr);
                    if(result != AUDIO_NV_DSP_NO_ERROR)
                    {
                        //SCI_TRACE_LOW:"ATC: siosadm for dsp, setmode param result = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8694_112_2_17_22_16_47_340,(uint8*)"d",result);
                        flag = FALSE;
                    }
                    else
                    {
                        AUDIO_DEVICE_MODE_TYPE_E eMode = AUDIONVDSP_GetAudioModeType((char *)audmod_dsp_devmod_ptr->ucModeName);
                        AUDIO_DEVICE_MODE_TYPE_E eOldCurMode = AUDIO_DEVICE_MODE_HANDHOLD;
                        AUDIO_GetDevMode(&eOldCurMode);
                        AUDIO_SetDevModeEx(INVALID_HANDLE, eMode);
                        //SCI_TRACE_LOW:"ATC:ATC_ProcessSioDSPSADMData: setModeInfo, Mode:0x%x, pucModeName:%s, mode:%d, oldMode:%d."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8704_112_2_17_22_16_47_341,(uint8*)"dsdd", audmod_dsp_devmod_ptr, audmod_dsp_devmod_ptr->ucModeName, eMode, eOldCurMode);
                        flag = TRUE;
                    }
                    
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC: siosadm for dsp , input data index error."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8711_112_2_17_22_16_47_342,(uint8*)"");
                }
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: siosadm for dsp , input data length error."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8716_112_2_17_22_16_47_343,(uint8*)"");
            }

            break;
        }
    
        default:
        {
            //SCI_TRACE_LOW:"Lost the escape character is %s is line %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8724_112_2_17_22_16_47_344,(uint8*)"sd",__FILE__,__LINE__);
            break;
        }
    }

    SCI_FREE(audmod_dsp_devmod_ptr);    

    if ( flag )
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);             
    }

    return S_ATC_DEFAULT_HANDLE;
}


/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SADM
//  Global resource dependence : g_rsp_str
//  Author: longting.zhao
//  Note: new added for the function to read&write audio parameters into NV
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioDSPSADMNVData(               
                      ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr,  // pointer to the data
                uint16          param_len
                )
{
    uint8        terminater_char;     // indicates ther terminater char
    uint16       data_length;
    BOOLEAN           flag = FALSE;
    uint32            mode_index = 0;
    uint32            dev_number = 0;
    AUDIO_NV_DSP_RESULT_E   result = AUDIO_NV_DSP_NO_ERROR;
    AUDIO_NV_DSP_MODE_INFO_T* audmod_dsp_devmod_ptr = PNULL;
    AUDIO_NV_DSP_MODE_INFO_T * audmod_devmod_ptr = NULL;
    int name_len = 0;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    
    param_len  -= 1;
    data_length = strlen((char*)data_ptr);
    terminater_char = data_ptr[data_length - 1];
    mode_index = s_atc_admm_index;
    //SCI_TRACE_LOW:"ATC:SioDSPSADMNVData:length:%d,terminater_char:%d,mode index::%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8771_112_2_17_22_16_47_345,(uint8*)"ddd",param_len,terminater_char,mode_index);

    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
        {
            flag = TRUE;
            //SCI_TRACE_LOW:"ATC:cancel the snvm audio data!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8779_112_2_17_22_16_47_346,(uint8*)"");
            break;
        }

        case CTRL_Z:
        {
            if(param_len == sizeof(AUDIO_NV_DSP_MODE_STRUCT_T)*2)
            {
                dev_number = AUDIONVDSP_GetModeNumber();
                if(s_atc_admm_index < dev_number)
                {
                    audmod_devmod_ptr = AUDIONVDSP_GetFirstDevMode();
                   
                    while(mode_index)
                    {
                        SCI_ASSERT(audmod_devmod_ptr != PNULL);/*assert verified check NULL pointer*/
                        audmod_devmod_ptr =  AUDIONVDSP_GetNextDevMode(audmod_devmod_ptr);
                        mode_index--;
                    }

                    audmod_dsp_devmod_ptr = (AUDIO_NV_DSP_MODE_INFO_T *)SCI_ALLOC_BASEZ(sizeof(AUDIO_NV_DSP_MODE_INFO_T)+1);

                    //SCI_TRACE_LOW:"ATC:SioDSPSADMNVData %s, %d."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8801_112_2_17_22_16_47_347,(uint8*)"sd",audmod_devmod_ptr->ucModeName, strlen((char *)audmod_devmod_ptr->ucModeName));

                    name_len = strlen((char *)audmod_devmod_ptr->ucModeName);
                    if (name_len <= AUDIO_NV_DSP_MODE_NAME_MAX_LEN)
                    {
                        SCI_MEMCPY(audmod_dsp_devmod_ptr->ucModeName, audmod_devmod_ptr->ucModeName, name_len);
                        audmod_devmod_ptr->ucModeName[name_len] = '\0';
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"ATC: Audio Mode Name too long: %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8811_112_2_17_22_16_47_348,(uint8*)"d", name_len);
                        flag = FALSE;
                        break;
                    }

                    if (!ConvertHexToBin(data_ptr, param_len, (uint8*)&audmod_dsp_devmod_ptr->tAudioNvDspModeStruct))
                    {
                        //SCI_TRACE_LOW:"ATC:ConvertHexToBin failed."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8818_112_2_17_22_16_47_349,(uint8*)"");
                        break;
                    }            

                    result = AUDIONVDSP_WriteModeParamToFlash((char *)audmod_devmod_ptr->ucModeName, audmod_dsp_devmod_ptr);
                    if (AUDMOD_NO_ERROR != result)
                    {
                        //SCI_TRACE_LOW:"ATC:SioDSPSADMNVData failed. result:%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8825_112_2_17_22_16_47_350,(uint8*)"d",result);
                        break;
                    }
                    flag = TRUE;  
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC: siosadm for dsp , input data index error."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8832_112_2_17_22_16_47_351,(uint8*)"");
                }
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: siosadmnv for dsp, input audio data length error.length:%d != %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8838_112_2_17_22_16_47_352,(uint8*)"dd",param_len,sizeof(AUDIO_NV_DSP_MODE_STRUCT_T)*2);
            }
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: snvm audio data format is error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8845_112_2_17_22_16_47_353,(uint8*)"");
            break;
        }
    }

    if (PNULL != audmod_dsp_devmod_ptr)
    {
        SCI_FREE(audmod_dsp_devmod_ptr);
    }

    if (flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);             
    }

    return S_ATC_DEFAULT_HANDLE;

}
#endif/*AUDIO_PARAM_ARM*/

#ifdef _MUX_ENABLE_
/*****************************************************************************/
//  Description : This function handles the midi pdu file   
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessDownFile(
                ATC_CONFIG_T     *atc_config_ptr,
                uint8            *param_ptr,
                uint16            param_len
                )
{
    uint8                 terminator;        // holds the terminal charactor
    ATC_STATUS            status = S_ATC_DEFAULT_HANDLE; 

    //SCI_TRACE_LOW:"ATC: SIO string len=%d,Data %x,%x,%x,%x,%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8884_112_2_17_22_16_47_354,(uint8*)"dddddd",param_len,param_ptr[0],param_ptr[1],param_ptr[2],param_ptr[3],param_ptr[4]);
    terminator = param_ptr[param_len - 1];
        
    switch (terminator)
    {
        case CTRL_Z:
        {
            break;
        }

        case ESC:
        {
            //SCI_TRACE_LOW:"ATC: cancel the given midi files"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8896_112_2_17_22_16_47_355,(uint8*)"");
            status = ERR_INVALID_INDEX;
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: the midi file format is error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8903_112_2_17_22_16_47_356,(uint8*)"");
            break;
        }
    }
    return status;
}

/*****************************************************************************/
//  Description : This function handles the midi pdu file   
//  Global resource dependence : none
//  Author:       shijun.cui
//  Note: add for Gionee
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessDownFileWithChk(
                ATC_CONFIG_T     *atc_config_ptr,
                uint8            *param_ptr,
                uint16            param_len
                )
{

#ifndef _ULTRA_LOW_CODE_
    uint8                 terminator;        // holds the terminal charactor
    uint8                 frame_head = FRAME_HEAD;

    terminator = param_ptr[param_len - 1];
    
    /*
    //SCI_TRACE_LOW:"ATC: smgv SIO string lenght=%d,s_pre_block_chk_num=%d,Data1=%x,Data2=%x,Data3=%x,Data4=%x,Data5=%x, terminator = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8929_112_2_17_22_16_47_357,(uint8*)"dddddddd",param_len,s_pre_block_chk_num,param_ptr[0],param_ptr[1],param_ptr[2],param_ptr[3],param_ptr[4],terminator);
    */
        
    switch (terminator)
    {
        case FRAME_HEAD:
        {
            //@Shijun.cui add for Gionee        
            if(1 == param_len ) //only one FRAME_HEAD received, do nothing
            {
                //SCI_TRACE_LOW:"ATC: smgv receive a frame head."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8939_112_2_17_22_16_48_358,(uint8*)"");
                break;
            }
            
            FillFrameData(atc_config_ptr, (uint8*)&frame_head, 1); // fill a frame head
            FillFrameData(atc_config_ptr, param_ptr, param_len);   // transfer received data to analyse
            
            break;
        }

        case FRAME_STOP:
        {
            //SCI_TRACE_LOW("ATC: smgv receive a frame stop, switch to AT command mode.");
            //SCI_TRACE_LOW("ATC: smgv ctrlz_end set to false.");
        #ifdef _MUX_ENABLE_     
            atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = FALSE;
            if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                SIO_ATC_SetCmdLineTerminateChar(atc_config_ptr->s3_reg, '\0');      
            else
                ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,atc_config_ptr->s3_reg, '\0');
        #else
            atc_config_ptr->is_ctrlz_end = FALSE;
            SIO_ATC_SetCmdLineTerminateChar(atc_config_ptr->s3_reg, '\0');
        #endif
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
            break;
        }
    
        default:
        {
            //SCI_TRACE_LOW:"ATC:midi file format error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_8969_112_2_17_22_16_48_359,(uint8*)"");
            break;
        }
    }
    
#endif /* _ULTRA_LOW_CODE_ */   
    return S_ATC_DEFAULT_HANDLE;
}

#ifndef _ULTRA_LOW_CODE_

/*****************************************************************************/
//  Description : This function is used to retrive the data between 0x7e
//                and transferred meaning operation 
//  Global resource dependence : 
//  return:       TRUE -- receive data successful
//                FALSE -- receive data failed
//  Author:       Shijun.cui
//  Note: provided by Gionee
/*****************************************************************************/
//0x7e--abc---0x7e//  == > --abc--
//0x7e <-- 0x7d 0x5e
//0x08 <-- 0x7d 0x88
//0x0d <-- 0x7d 0x8d
//0x7d <-- 0x7d 0x5d
uint32 FillFrameData(
                ATC_CONFIG_T *atc_config_ptr,
                const uint8 *buf_ptr,
                int32 len
                )
{
    static uint8                             gFlag_7D   =0;
    static uint8  sFrameBuf[FRAME_MAX_LEN]  = {0};
    uint32                                  sFrameLen  = 0;
    uint8                                                 ch  = 0;
    int32                                                   i   = 0;

    for(i=0; i<len; i++)
    {
        ch=buf_ptr[i];
        switch( ch )
        {
            case 0x7D:
            {
                gFlag_7D = 0x7d;
                break;
            }

            case 0x5E:
            {
                if(0x7d == gFlag_7D)
                {
                    ch=0x7e;
                    gFlag_7D =0;
                }
                sFrameBuf[sFrameLen++]=ch;
                break;
            }

            case 0x5D:
            {
                if(0x7d == gFlag_7D)
                {
                    ch=0x7d;
                    gFlag_7D =0;
                }
                sFrameBuf[sFrameLen++]=ch;
                break;
            }
                
            case 0x88:
            {
                if(0x7d == gFlag_7D)
                {
                    ch=0x08;
                    gFlag_7D =0;
                }
                sFrameBuf[sFrameLen++]=ch;
                break;
            }
        
            case 0x8D:
            {
                if(0x7d == gFlag_7D)
                {
                    ch=0x0d;
                    gFlag_7D =0;
                };
                sFrameBuf[sFrameLen++]=ch;
                break;
            }

            default:
            {
                sFrameBuf[sFrameLen++]=ch;
                break;
            }
        };
    }

    return(0);
}

#endif
#endif

/*****************************************************************************/
//  Description : This function handles the AT+SLOG command.This command is used
//  to enable the log output.
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSLOG )
{
    ATC_STATUS    status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        //set command AT+SLOG=type
        case ATC_CMD_TYPE_SET:
        {
            // check the type
            if (PARAM1_FLAG)
            {
                //PARA1.num define the log switch
                if (PARA1.num)
                {
                    SCI_InitLogSwitch(SCI_TRUE);
                }
                else
                {
                    SCI_InitLogSwitch(SCI_FALSE);
                }
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            
            break;
        }
            
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert SLOG,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10043_112_2_17_22_16_51_369,(uint8*)"");
            break;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+CPAS command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPAS )
{
    if (ATC_GET_CMD_TYPE == ATC_CMD_TYPE_EXECUTE)
    {   
        sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CPAS].cmd_name->str_ptr,
                    atc_config_ptr->cpas);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        
        return S_ATC_SUCCESS;
    }
    else
    {
        return S_ATC_FAIL;
    }
}

/*****************************************************************************/
//  Description : This function handle the AT+SPAUDVOLTYPE command
//  Global resource dependence : g_rsp_str
//  Author:  chong.liu
//  Note: 
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPAUDVOLTYPE)
{
//#ifdef _AUDIO_ENHANCE_
    ATC_STATUS        status = S_ATC_SUCCESS;
    uint32            audio_type = 0;
    uint32            audio_volume = 0;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if((PARAM1_FLAG && (PARAM1_TYPE==ATC_CMD_PARAM_TYPE_NUMERIC))
            &&(PARAM2_FLAG && (PARAM2_TYPE==ATC_CMD_PARAM_TYPE_NUMERIC)))
            {
                audio_volume = PARA1.num;
                audio_type = PARA2.num;

            #ifdef AUDIO_PARAM_ARM
                if( 0 != AUDIO_SetVolumeAndTypeEx(INVALID_HANDLE, audio_volume, audio_type))
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }
            #else
                if(0 != AUDIO_SetVolume(audio_volume))
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }
            #endif
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

    return status;
//#else
//    return S_ATC_SUCCESS;
//#endif /* _AUDIO_ENHANCE_*/
}

/*****************************************************************************/
//  Description : This function handle the AT&F command 
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_F )
{
#ifndef _ULTRA_LOW_CODE_
    //SCI_ASSERT(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE); /*assert verified check command type*/

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT&D command 
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_D)
{
#ifndef _ULTRA_LOW_CODE_
    //SCI_ASSERT(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE); /*assert verified check command type*/
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT&C command 
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_C )
{
#ifndef _ULTRA_LOW_CODE_
    //SCI_ASSERT(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE); /*assert verified check command type*/

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

#if 0
/*****************************************************************************/
//  Description : This function handle the ATW command 
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessW )
{
#ifndef _ULTRA_LOW_CODE_
    SCI_ASSERT(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE);   /*assert verified check command type*/ 

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT&K command 
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_K )
{
#ifndef _ULTRA_LOW_CODE_
    SCI_ASSERT(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE);   /*assert verified check command type*/  
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the ATS95 command 
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS95 )
{
#ifndef _ULTRA_LOW_CODE_
    SCI_ASSERT(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE); /*assert verified check command type*/
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handleS the AT&W command 
//              AT&W is used to save the current configuration to the nv
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_W )
{
#ifndef _ULTRA_LOW_CODE_
    SaveConfigProfile(atc_config_ptr);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT%%% command 
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessTHREE_PERCENT )
{
#ifndef _ULTRA_LOW_CODE_
    SCI_ASSERT(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE); /*assert verified check command type*/
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CBC command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCBC )
{
#ifndef _ULTRA_LOW_CODE_
    uint32        bcs = 0;
    uint32        bcl = 0;

    // call the api for the battery
    bcs = CHR_CheckBatteryStaus();
    switch ( bcs )
    {
        case CHR_BATTERY_NONE_S:
        {
             bcs = AT_CBC_NO;
             break;
        }

        case CHR_BATTERY_NORMAL_S:
        {
             bcs = AT_CBC_NO_CHARGE;
             break;
        }

        case CHR_BATTERY_CHARGING_S:
        {
             bcs = AT_CBC_CHARGE;
             break;
        }

        default:
        {
             //SCI_TRACE_LOW:"ATC: Assert CBC,invalid type."
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_9247_112_2_17_22_16_48_360,(uint8*)"");
             break;
        }
    }
    bcl = CHR_GetBatCapacity();

    sprintf((char*)g_rsp_str, "%s: %ld,%ld", g_atc_info_table[AT_CMD_CBC].cmd_name->str_ptr, bcs, bcl);
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CRC command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRC )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 0, &atc_config_ptr->crc);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handleS the AT+SBCM command. This command is used
//  to manage battery charge.
//  AT+SBCM=<mode>,<chargeind>,<battLevMax>,<battLevMin>,<TPulseInCharge>,<TPulseOutCharge>,
//  <BattIntRes>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSBCM )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS       status = S_ATC_SUCCESS;
    uint32           mode;     // holds the charge mode
    uint32           batt_vol;
    uint32           batt_state;
    uint32           batt_lev_min;
    uint32           batt_lev_max;
    uint32           time_pluse_in_charge;
    uint32           time_pluse_out_charge;
    uint32           batt_int_res;

    if (PARAM1_FLAG)
    {
        mode = PARA1.num;
        switch (mode)
        {
            case AT_STOP_CHR:
            {
                // stop battery charge
                CHR_StopCharge();
                break;
            }

            case AT_START_CHR:
            {
                // start the battery charge
                CHR_StartCharge();
                break;
            }

            case AT_GET_BATT_VOL:
            {
                // get the battery volate
                batt_vol   = CHR_GetVoltage();
                batt_state = CHR_GetBattState();
                sprintf((char*)g_rsp_str, "+SBCI: %ld, %ld", batt_state, batt_vol);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }

            case AT_SET_BATT_PARM:
            {
                if ( PARAM3_FLAG )
                {
                    batt_lev_max = PARA3.num;
                    CHR_SetBattLevelMax(batt_lev_max);
                }
                
                if ( PARAM4_FLAG )
                {
                    batt_lev_min = PARA4.num;
                    CHR_SetBattLevelMin(batt_lev_min);
                }
                
                if ( PARAM5_FLAG )
                {
                    time_pluse_in_charge = PARA5.num;
                    CHR_SetTPulseInCharge(time_pluse_in_charge);
                }
                
                if ( PARAM6_FLAG )
                {
                    time_pluse_out_charge = PARA6.num;
                    CHR_SetTPulseOutCharge(time_pluse_out_charge);                
                }
                
                if ( PARAM7_FLAG )
                {
                    batt_int_res = PARA7.num;
                    CHR_SetBattIntRes(batt_int_res);
                }
                break;
            }
                
           default:
                break;
        }
        
        // if parameter 2 is present then set the battery charge indicator
        if ( PARAM2_FLAG )
        {
            atc_config_ptr->atc_config_nv.battery_indicator = (uint8)PARA2.num;
            // Save to the NV ram.
            ATC_WriteNVItem(atc_config_ptr);
        }
    }
    else
    {
        status = ERR_OPERATION_NOT_ALLOWED;
    }

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the ATS2 command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS2 )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS       status = S_ATC_SUCCESS; 

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                atc_config_ptr->s2_reg = (uint8)PARA1.num;
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert S2,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_9408_112_2_17_22_16_49_361,(uint8*)"");
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the ATS3 command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS3 )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 13, &atc_config_ptr->s3_reg);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the ATS4 command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS4 )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 10, &atc_config_ptr->s4_reg);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the ATS5 command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS5 )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 8, &atc_config_ptr->s5_reg);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the ATS6 command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS6 )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 2, &atc_config_ptr->s6_reg);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the ATS7 command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS7 )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 50, &atc_config_ptr->s7_reg);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the ATS8 command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS8 )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 2, &atc_config_ptr->s8_reg);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the ATS10 command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS10 )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 2, &atc_config_ptr->s7_reg);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+VGT command 
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessVGT )
{
#ifndef _ULTRA_LOW_CODE_
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+SPEAKER command 
//                 AT+SVGT=<value>,<value>
//                        <value>:0 -- 1
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPEAKER )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS  status = S_ATC_SUCCESS;
    uint32      result;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                s_b_mic_aux_device = PARA1.num;           
            }
            
            if(PARAM2_FLAG)
            {
                s_b_spk_aux_device = PARA2.num;
            }
            
            if(!PARAM1_FLAG && !PARAM2_FLAG)
            {
                status = ERR_INVALID_INDEX;
            }
            
            // audio should provide setting mic/aux device and speaker pri/aux interfaces!
            result = LAYER1_SwitchSoundDevice(s_b_mic_aux_device, s_b_spk_aux_device, (AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
            if (result != SCI_SUCCESS)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            break;
        }
         
         case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d,%d",g_atc_info_table[AT_CMD_SPEAKER].cmd_name->str_ptr,
                s_b_mic_aux_device, s_b_spk_aux_device);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
         
         default:
        {
            break;
        }
     }
     
     return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+ECHO command 
//  Global resource dependence : none
//  Author:     Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessECHO )
{
#ifndef _ULTRA_LOW_CODE_
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            //call the Audio function
            break;
        }
        
        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char*)g_rsp_str, "%s: 0,1,0,3,10,7", 
                g_atc_info_table[AT_CMD_ECHO].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            return S_ATC_SUCCESS;
        }

        case ATC_CMD_TYPE_READ:    
        {
            break; 
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert ECHO,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_9619_112_2_17_22_16_49_362,(uint8*)"");
            break;
        }
    }

    return S_ATC_FAIL;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CICB command 
//  this command is used to set the type of incoming calls
//               AT+CICB=<mode>
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCICB )
{
#ifndef _ULTRA_LOW_CODE_
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                atc_config_ptr->cicb_val = PARA1.num;
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CICB].cmd_name->str_ptr,
                atc_config_ptr->cicb_val);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert CICB,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_9664_112_2_17_22_16_50_363,(uint8*)"");
            break;
        }
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+IPR command, this command is used 
//  to set or read the bandrate of the uart.
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessIPR )
{
#ifndef _ULTRA_LOW_CODE_
    uint32        baud_rate = 0;
    uint32              status = ERR_OPERATION_NOT_ALLOWED;
    int32                        i = 0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                baud_rate = PARA1.num;
                for (i=0; i<AT_IPR_MAX; i++)
                {
                    if (c_baudrate_data[i].atc_val == baud_rate)
                    {
                        baud_rate = c_baudrate_data[i].reg_val;
                        
                        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                        SCI_SLEEP(100);
                    
                    #ifdef _MUX_ENABLE_
                        MUX_Setbaud_rate(baud_rate);
                    #else     
                        SIO_SetBaudRate(COM_USER, baud_rate);
                    #endif
                        status = S_ATC_DEFAULT_HANDLE;
                        break;
                    }
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
        #ifdef _MUX_ENABLE_
            baud_rate = MUX_Getbaud_rate();
        #else
            baud_rate = SIO_GetBaudRate(COM_USER);
        #endif
            for (i=0; i<AT_IPR_MAX; i++)
            {
                if (baud_rate == c_baudrate_data[i].reg_val)
                {
                    baud_rate = c_baudrate_data[i].atc_val;
                    sprintf((char*)g_rsp_str, "%s: %ld", g_atc_info_table[AT_CMD_IPR].cmd_name->str_ptr,    baud_rate);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    status = S_ATC_SUCCESS;
                    break;
                }
            }
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert IPR,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_9742_112_2_17_22_16_50_364,(uint8*)"");
            break;
        }
    }

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CRMP command AT+CRMP is used to 
//                play melody
//  AT+CRMP=<call type>,<volume>,<type>,<index>
//  Global resource dependence : none
//  Author:       Xueli.lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRMP )
{
#ifndef _ULTRA_LOW_CODE_
    uint8       call_type = 0;
    uint8            index = 0;
    uint32           times = AUD_PLAY_FOREVER;   
    ATC_STATUS  status = S_ATC_SUCCESS;

    //SCI_TRACE_LOW:"ATC: CRMP"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_9767_112_2_17_22_16_50_365,(uint8*)"");
    // if the call type or index are not present then return error
    if (!PARAM1_FLAG || !PARAM3_FLAG)
    {
        return ERR_INVALID_INDEX;        
    }
    
    if (PARAM2_FLAG)
    {
        times = PARA2.num;        
        if (0 == times)
        {
           times = AUD_PLAY_FOREVER;
        }
    }
   
    call_type = PARA1.num;
    index = PARA3.num;
    //if index==0,stop ring
    if (index == 0)
    {
        return S_ATC_SUCCESS;
    }

    switch (call_type)
    {
        // play the voice call melody
        case AT_CRMP_VOICE_CALL:
        case AT_CRMP_DATA_CALL:
        case AT_CRMP_FAX_CALL:
        {
            /*delete functions unused, return invalid status*/
            //status = Atc_PlayMidi(atc_config_ptr,index,times);
            //SCI_TRACE_LOW:"VOIC, DATA and FAX CALLS are not supported now"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_9800_112_2_17_22_16_50_366,(uint8*)"");
            status = ERR_OPERATION_NOT_SUPPORTED;    
            break;
        }

            // play the sms melody
        case AT_CRMP_SMS:        
        {
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert CRMP,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_9813_112_2_17_22_16_50_367,(uint8*)"");
            break;
        }
    }

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CR command 
//                set if its GPRS type before show connect
//  Global resource dependence : none
//  Author: 
//  Note:
//  History:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCR )
{
#ifndef _ULTRA_LOW_CODE_
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 0, &atc_config_ptr->cr);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+SNVM command, This command is used
//  to manage the NV items.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSNVM )
{
#ifndef _ULTRA_LOW_CODE_
#ifdef _ATC_ONLY

    ATC_STATUS      status = S_ATC_SUCCESS;
    NVITEM_ERROR_E  nv_err = NVERR_NONE;
    uint16          nv_item_id;
    uint16          nv_item_id_end;
    uint16          mode;
    uint16          nv_item_length;
    uint8           *buf_ptr;
    uint8           *temp_ptr;
    
    // Set the nv item valuve
    //SCI_TRACE_LOW("ATC:ATC_ProcessSNVM");
    if (PARAM1_FLAG && PARAM2_FLAG)
    {
        mode = (uint16)PARA1.num;
        nv_item_id = (uint16)PARA2.num + MN_NV_USER_BASE; // id from 0 - 1199
        
        // set the related NV value
        switch (mode)
        {
            case ATC_SNVM_READ:
            {
                nv_item_length = NVITEM_GetLength(nv_item_id);
                
                if(nv_item_length > MAX_ATC_RSP_LEN/2 - 12)// exclude +SNVM: <length>,
                {
                    //SCI_TRACE_LOW("ATC: nv item length = %d",nv_item_length);
                    status =  ERR_INVALID_INDEX;
                    break;
                }
                
                buf_ptr = (uint8*)SCI_ALLOC_BASEZ(nv_item_length*2 + 1);
                
                nv_err = EFS_NvitemRead(nv_item_id, nv_item_length, (uint8*)buf_ptr);
                if (nv_err != NVERR_NONE)
                {
                    //SCI_TRACE_LOW("ATC: SNVM read nv item id = %d fail, err = %d", nv_item_id, nv_err);
                    SCI_FREE(buf_ptr);
                    status = ERR_INVALID_INDEX;
                    break;
                }
                sprintf((char*)g_rsp_str,"%s: %d,",g_atc_info_table[AT_CMD_SNVM].cmd_name->str_ptr,nv_item_length);
                temp_ptr = g_rsp_str + strlen((char*)g_rsp_str);
                
                ConvertBinToHex(buf_ptr, nv_item_length, temp_ptr);
                
                //qing.xi cr53411
                temp_ptr[nv_item_length * 2] = '\0';
                //qing.xi

                
                SCI_FREE(buf_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
        
            case ATC_SNVM_ADD:
            case ATC_SNVM_REPLACE:
            {
                s_nv_item_id = nv_item_id;
            #ifdef _MUX_ENABLE_
                atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].is_ctrlz_end = TRUE;
                atc_config_ptr->atc_link_config[atc_config_ptr->current_link_id].ctrlz_end_status  = ATC_CTRLZ_SNVM;
            #else    
                atc_config_ptr->is_ctrlz_end = TRUE;
                atc_config_ptr->ctrlz_end_status  = ATC_CTRLZ_SNVM;
            #endif
                // output the '>'
                sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
            #ifdef _MUX_ENABLE_
                if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
                {
                   SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                   // set the terminate char to receive PDU from the SIO
                   SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);            
                }
                else
                {
                   ATC_SetCmdLineTerminateChar(atc_config_ptr->current_link_id,CTRL_Z, ESC);
                   MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_config_ptr->current_link_id);
                   // set the terminate char to receive PDU from the SIO
                }
            #else
                SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
                // set the terminate char to receive PDU from the SIO
                SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
            #endif
                status = S_ATC_DEFAULT_HANDLE;
                break;
            }
        
            case ATC_SNVM_GETSIZE:
            {
                nv_item_length = NVITEM_GetLength(nv_item_id);
                sprintf((char*)g_rsp_str,"%s: %d",g_atc_info_table[AT_CMD_SNVM].cmd_name->str_ptr,nv_item_length);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
        
            case ATC_SNVM_DELETE:
            {
                if(PARAM3_FLAG)
                {
                    nv_item_id_end = (uint16)PARA3.num + MN_NV_USER_BASE;
                    
                    if(nv_item_id_end < nv_item_id)
                    {
                        status = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }
                }
                else
                {
                    nv_item_id_end = nv_item_id;
                }
                nv_err = EFS_NvitemDelete(nv_item_id, nv_item_id_end);
                if (nv_err != NVERR_NONE)
                {
                    //SCI_TRACE_LOW:"ATC: SNVM read nv item id = %d fail, err = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_9966_112_2_17_22_16_50_368,(uint8*)"dd", nv_item_id, nv_err);
                    status = ERR_INVALID_INDEX;
                }
                break;
            }
        
            default :
            {
                break;
            }
        }
    }
    else
    {
        status = ERR_OPERATION_NOT_SUPPORTED;
    }
    
    return status;
#else
    return S_ATC_SUCCESS;
#endif // END _ATC_ONLY
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+SBOOT command.This command is used
//  to set the spreadtrum module to BOOT mode.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli.lv
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSBOOT )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
    POWER_BootReset();
    return S_ATC_DEFAULT_HANDLE; 
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+SSGF command.This command is used
//  to set or get GPIO function.
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSGF )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS    status = S_ATC_SUCCESS;
    uint32           gpio_id = 0;
    uint32               type = 0; // 0 - set direction, 1 - get Direction
    BOOLEAN          value = FALSE;
    BOOLEAN         result = FALSE;
    uint32        gpio_num = 0;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // check the type            
            if (PARAM1_FLAG && PARAM2_FLAG)
            {
                //PARA1.num define the GPIO id
                gpio_id = PARA1.num;
                //PARA2.num define the operation type, 0 - set gpio direction, 1 - get gpio direction
                type = PARA2.num;
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }
            
#ifndef WIN32
            gpio_num = GPIO_GetMaxNumber();
            
            if (gpio_id >= gpio_num)
            {
                status = ERR_INVALID_INDEX;
                break;
            }
#endif
            
            if ((type == AT_GPIO_SET_DIRECTION) && !PARAM3_FLAG)
            {
                //SCI_TRACE_LOW:"ATC: need gpio direction parameter!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10095_112_2_17_22_16_51_370,(uint8*)"");
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }
            
            if (PARAM3_FLAG)
            {
               value = PARA3.num;
            }
           
            switch(type)
            {
                case AT_GPIO_SET_DIRECTION:
                {
#ifndef WIN32
                    GPIO_SetDirection(gpio_id, value); // set input direction
#endif
                    break;
                }
            
                case AT_GPIO_GET_DIRECTION:
                {
#ifndef WIN32
                    result = GPIO_GetDirection(gpio_id); // get direction
#endif
                    sprintf((char*)g_rsp_str, "%s: %d",g_atc_info_table[AT_CMD_SSGF].cmd_name->str_ptr, result);
                    ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
                    break;
                }

                default :
                {
                    //SCI_TRACE_LOW:"ATC: Assert Failure receive gpio type = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10127_112_2_17_22_16_51_371,(uint8*)"d",type);
                    break;
                }
            }
            break;
        }
            
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert SSGF,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10136_112_2_17_22_16_51_372,(uint8*)"");
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+SSGS command.This command is used
//  to set or get GPIO value.
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSGS )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS    status = S_ATC_SUCCESS;
    uint32        gpio_id;
    uint32        type; // 0 - set gpio value, 1 - get gpio value
    BOOLEAN       value = FALSE;
    BOOLEAN       result = FALSE;
    uint32        gpio_num;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // check the type
            if (PARAM1_FLAG && PARAM2_FLAG)
            {
                //PARA1.num define the GPIO id
                gpio_id = PARA1.num;
                //PARA2.num define the operation type, 0 - set gpio value, 1 - get gpio value
                type = PARA2.num;
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }
            
#ifndef WIN32
            gpio_num = GPIO_GetMaxNumber();
            if (gpio_id >= gpio_num)
            {
                status = ERR_INVALID_INDEX;
                break;
            }
#endif
            
            if ((AT_GPIO_SET_VALUE == type) && !PARAM3_FLAG)
            {
                //SCI_TRACE_LOW:"ATC: need gpio set parameter!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10189_112_2_17_22_16_51_373,(uint8*)"");
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }
            
            if (PARAM3_FLAG)
            {
                value = PARA3.num;
            }

            switch(type)
            {
                case AT_GPIO_SET_VALUE:
                {
#ifndef WIN32
                    result = GPIO_GetDirection(gpio_id); // get direction
#endif
                    if (!result)
                    {
                        //SCI_TRACE_LOW("ATC: gpio %d set to input directiong, set value not permit.", gpio_id);
                        status = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }
#ifndef WIN32
                    GPIO_SetValue(gpio_id, value); // set value
#endif
                    break;
                }
            
                case AT_GPIO_GET_VALUE:
                {
#ifndef WIN32
                    result = GPIO_GetDirection(gpio_id); // get direction
#endif
                    if (!result)
                    {
                        //SCI_TRACE_LOW("ATC: gpio %d set to input directiong, get value not permit.", gpio_id);
                        status = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }
#ifndef WIN32
                    result = GPIO_GetValue(gpio_id); // get value
#endif
                    sprintf((char*)g_rsp_str, "%s: %d",g_atc_info_table[AT_CMD_SSGS].cmd_name->str_ptr, result);
                    ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
                    break;
                }
            
                default :
                {
                    //SCI_TRACE_LOW:"ATC: Assert Failure receive gpio type = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10239_112_2_17_22_16_51_374,(uint8*)"d",type);
                    break;
                }
            }
            break;
        }
            
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert SSGS,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10248_112_2_17_22_16_51_375,(uint8*)"");
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CRSM command.This command is used
//  to set or get GPIO value.
//  Global resource dependence : g_rsp_str
//  Author:       
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRSM)
{
#ifndef _ULTRA_LOW_CODE_
    uint32   status = S_ATC_DEFAULT_HANDLE;
    MNSIM_ERROR_E           error_code   = MNSIM_ERROR_POWER_OFF;
    MN_CRSM_COMMAND_TYPE_E  atc_sim_cmd  = MN_CRSM_COMMAND_TYPE_NUM;
    uint16  file_id = 0;
    uint8    P1 = 0;
    uint8    P2 = 0;
    uint8    P3 = 0;
    uint8    data[255];

   MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    //SCI_TRACE_LOW:"ATC:ATC_ProcessCRSM"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10276_112_2_17_22_16_51_376,(uint8*)"");
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                atc_sim_cmd = (MN_CRSM_COMMAND_TYPE_E)(PARA1.num);
            }
            
            switch (atc_sim_cmd)
            {
                case MN_CRSM_UPDATE_BINARY:
                case MN_CRSM_UPDATE_RECORD:
                {
                    if (PARAM2_FLAG && PARAM3_FLAG && PARAM4_FLAG && PARAM5_FLAG && PARAM6_FLAG)
                    {
                        file_id = (uint16)(PARA2.num);
                        P1      = (uint8)(PARA3.num);
                        P2      = (uint8)(PARA4.num);
                        P3      = (uint8)(PARA5.num);

                        SCI_MEMSET(data, 0, sizeof(data));
                        ConvertHexToBin(PARA6.str_ptr->str_ptr, PARA6.str_ptr->str_len, data);
                    }
                    else
                    {
                        return ERR_INVALID_INDEX;
                    }
                    break;
                }

                case MN_CRSM_READ_BINARY:
                case MN_CRSM_READ_RECORD:
                {
                    if (PARAM2_FLAG && PARAM3_FLAG && PARAM4_FLAG && PARAM5_FLAG)
                    {
                        file_id = (uint16)(PARA2.num);
                        P1      = (uint8)(PARA3.num);
                        P2      = (uint8)(PARA4.num);
                        P3      = (uint8)(PARA5.num);
                    }
                    else
                    {
                        return ERR_INVALID_INDEX;
                    }   
                    break;
                }

                case MN_CRSM_GET_RESPONSE:
                {
                    if (PARAM2_FLAG)
                    {
                        file_id = (uint16)(PARA2.num);
                    }
                    else
                    {
                        return ERR_INVALID_INDEX;
                    }   
                    break;
                }

                case MN_CRSM_GET_STATUS:
                {
                    break;
                }

                default:
                {
                    return ERR_INVALID_INDEX;
                }
            }
            
            MNSIM_GetSimStatusEx(dual_sys, &error_code);
            if (MNSIM_ERROR_REMOVED == error_code)
            {
                status = ERR_SIM_NOT_INSERTED;
            }
            else
            {
                if (ERR_MNDATAMAG_NO_ERR != MNSIM_CrsmEx(dual_sys, atc_sim_cmd, P1, P2, P3, file_id, data))
                {
                    status = S_ATC_FAIL;
                }
            }
            break;
        }

        case ATC_CMD_TYPE_TEST:
        {
            status =  S_ATC_SUCCESS;
            break;
        }
            
        case ATC_CMD_TYPE_READ:
        default:
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
    }

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+SCREG command 
//  Global resource dependence : g_rsp_str
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCREG )
{
#ifdef _ULTRA_LOW_CODE_
    return S_ATC_FAIL;
#else
    MN_PHONE_CURRENT_PLMN_INFO_T   register_status;
    ATC_STATUS                     status = S_ATC_SUCCESS;
       MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
       
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
        {
        // call the MN API functions
        #ifdef _USE_OLD_L4_API
            register_status = MNPHONE_GetCurrentPLMNInfo();
        #else
            register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
        #endif

            if (register_status.plmn_status != PLMN_NO_SERVICE)
            {
                sprintf((char*)g_rsp_str,"%s: %d,%d,%04X,%04X", g_atc_info_table[AT_CMD_SCREG].cmd_name->str_ptr,
                    register_status.mcc, register_status.mnc, register_status.lac, register_status.cell_id);
            }
            else
            {
                sprintf((char*)g_rsp_str, "%s: 0,0,0,0", g_atc_info_table[AT_CMD_SCREG].cmd_name->str_ptr);
            }

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            status = S_ATC_SUCCESS;
            break;
        }

        default:        
        {
            break;
        }
    }
    return status;
#endif
}

/*****************************************************************************/
//  Description : This function handle the AT+CIEV command 
//  Global resource dependence : none
//  Author:      
//  Note: this command set the rssi report level
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCIEV )
{
#ifdef _ULTRA_LOW_CODE_
    return S_ATC_FAIL;
#else
    ATC_STATUS    status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(!PARAM1_FLAG || !PARAM2_FLAG || !PARAM3_FLAG || !PARAM4_FLAG || !PARAM5_FLAG)
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }
            if(PARA2.num < PARA3.num || PARA3.num < PARA4.num || PARA4.num < PARA5.num)
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }
            
            g_atc_ciev_rang   = PARA1.num;
            g_atc_ciev_lev[0] = PARA2.num;
            g_atc_ciev_lev[1] = PARA3.num;
            g_atc_ciev_lev[2] = PARA4.num;
            g_atc_ciev_lev[3] = PARA5.num;
            
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d, %d, %d, %d, %d", 
                g_atc_info_table[AT_CMD_CIEV].cmd_name->str_ptr,
                g_atc_ciev_rang,
                g_atc_ciev_lev[0],
                g_atc_ciev_lev[1],
                g_atc_ciev_lev[2],
                g_atc_ciev_lev[3]);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            SCI_ASSERT(FALSE); /*assert verified check default case*/
            break;
        }
    }
    return status;
#endif /* _ULTRA_LOW_CODE_ */
}

#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
