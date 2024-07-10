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
#include "dal_chr.h"
#include "tasks_id.h"
#include "dal_power.h"
#include "aud_enha_exp.h"
#if defined( AUTO_TEST_SUPPORT)||defined(_ATC_UIX8910_ENABLE_)
#include "tasks_id.h"
#include "keypad.h"
#include "ref_param.h"
#include "mn_type.h"
#endif
#ifdef FM_SUPPORT
#include "fm_drv.h"
#endif

#ifdef BLUETOOTH_SUPPORT_SPRD_BT
#include "bt_abs.h"
#if defined(BLE_INCLUDED) || defined(BT_BLE_SUPPORT)
#ifndef WIN32
#include "bt_ble_abs.h"
#endif
#endif
#endif

#ifdef WIFI_SUPPORT_UNISOC_RX
#include "wifi_api.h"
#endif

#if 0 //CR171132 for 6530 codesize cutdown
#include "atc_gsm_stk.h"
#endif

#include "dal_power.h"
#if !defined( _ULTRA_LOW_CODE_ ) ||defined(_ATC_UIX8910_ENABLE_)
#include "aud_gen.h"
#include "audio_prod.h"
#include "layer1_engineering.h"
#ifndef WIN32
#include "gpio_drv.h"
#endif
#endif
#include "version.h"
#include "efs.h"
#include "audio_api.h"

#include "custom_tone_adp.h"
#include "dtmf_tone_adp.h"

#include "power.h"
#include "run_mode.h"
#include "ps_eng.h"

#include "prod_param.h"
#include "ref_outport.h"
#include "aud_proc_exp.h"


#ifdef AUDIO_PARAM_ARM
#include "audio_nv_arm.h"
#endif

#include "admm_III.h"
#include "atc_malloc.h" //add by shuyan.hu

#include "sig_code.h"
#include "tcpip_types.h"
#include "app_tcp_if.h"

#include "ps_nvitem_id.h"//NV_SIM_CFG
#ifdef DUALSIM_SIMLOCK_SUPPORT
#include "simlock_storage.h"
#endif

#include "layer1_engineering.h"
#include "diag.h"

#include "sio.h"
#include "spiflash.h"
#include "mem_prod.h"
#include "scm_api.h"
#include "CVS_frame.h"
#include "audio_api.h"
#include "os_param.h"


#ifdef PLATFORM_UWS6121E
#include "ppp_mp_api.h"
#endif

#ifdef MQTT_SUPPORT
#include "../../../../Third-party/at_op/atc_daps_api.h"
#endif

#ifdef COAP_SUPPORT
#include "coap_async_api.h"
#endif

#include "deepsleep_drvapi.h"

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

#if !defined( _ULTRA_LOW_CODE_ ) ||defined(_ATC_UIX8910_ENABLE_)
//uint8                        g_atc_ciev_rang = 3;//default value
uint8                        g_atc_ciev_lev[4] = {102, 96, 90, 84};


static int16                 s_dsp_ram_flag = 0;
static int16                 s_dsp_data_length = 0;
static int16                 s_dsp_dest_addr = 0;
#endif
static AUDIO_DEVICE_MODE_TYPE_E       s_aud_dev_mod = AUDIO_DEVICE_MODE_HANDHOLD;
#if !defined( _ULTRA_LOW_CODE_ ) ||defined(_ATC_UIX8910_ENABLE_)
static uint16                s_b_mic_aux_device = FALSE;
static uint16                s_b_spk_aux_device = FALSE;
#endif
static uint16                s_atc_eq_type = AUD_EQ_NORMAL;
static uint16                s_nv_item_id = 0;
static uint8                 s_atc_admm_index = 0;
#ifdef _NDS_EXT_CMD_
static uint8                 s_swversion[20]={"vertion1.01"};
static uint8                 s_cid[4] = {"cid"};
static uint8                 s_gmid[7] = {"gmid"};
static uint8                 s_seed[16] = {"seed"};
#endif

static uint8                 s_enha_eqmode_index = 0;
#if 0 //CR171132 for 6530 codesize cutdown
#ifndef _ULTRA_LOW_CODE_
extern char *                g_stk_terminal_profile[];
#endif
#endif
extern RSP_RESULT_STR        g_rsp_str;
extern const ATC_INFO_T            g_atc_info_table[];
extern ATC_CONFIG_T                    s_config_info;

uint16 ring_times[MN_SYS_NUMBER]={0};
uint16 have_ringed_times[MN_SYS_NUMBER]={0};


#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
extern const ATC_RSP_STR_T   g_atc_cpin_rsp[];
//extern const AUD_RING_DATA_INFO_T g_atc_message_ring_info[];
extern SCI_TMID              T_ATC_CALL_EXP_IND;
extern BOOLEAN               g_atc_expire_ind;
extern BOOLEAN               g_atc_call_need_disc;
static BOOLEAN               s_received_sim_ready[MN_SYS_NUMBER] = {FALSE};

BOOLEAN                      g_is_ps_deactivate[MN_SYS_NUMBER] = {FALSE};
SCI_TMID                     T_ATC_POWEROFF_EXP_IND = PNULL;
/*lint -save -e551 */
static BOOLEAN               s_is_ps_activate[MN_SYS_NUMBER] = {FALSE};   /*lint !e551*/

ATC_CREG_STATE_T g_creg_state[MN_DUAL_SYS_MAX];
#endif
extern BOOLEAN               g_atc_call_flag;
extern ATC_CALLS_STATE_T     g_calls_state[MN_SYS_NUMBER];
static uint32                s_sim_error_reason[MN_SYS_NUMBER];

#ifndef _MUX_ENABLE_
BOOLEAN g_is_diag_cmd = FALSE;
uint8  *g_diag_cmd_buffer_ptr = PNULL;
#endif
#ifndef BLUETOOTH_SUPPORT_NONE
static const char    *s_bt_mode_str[ATC_MAX_BT_MODE_NUM * 2] =
{
    "EUT", "CH", "TX", "TXPWR", "RX","RXPACKCOUNT", "eut", "ch", "tx", "txpwr", "rx", "rxpackcount"
};
#endif
#ifndef _ULTRA_LOW_CODE_
#define  ATC_LOOP_TEST_DELAY_TIME 100

#ifdef _MUX_REDUCED_MODE_
uint16   g_rxlev_arr[MN_SYS_NUMBER] = {0};
BOOLEAN  g_received_sim_ind[MN_DUAL_SYS_MAX] = {FALSE};
BOOLEAN  g_sent_sind4 = FALSE;
#endif

#ifdef _MUX_ENABLE_
extern uint8 g_poweroff_exp_link_id;
#endif

ATC_LOOP_TEST_INFO_T loop_test_info;
static ATC_POWER_INF_T    s_atc_power_info = {0};
#ifndef _ULTRA_LOW_CODE_
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
#endif
//static const uint8          c_sidet_data[AT_MAX_SIDET_LEV] =
//{4, 3, 2, 1, 0};



#ifdef _MUX_ENABLE_
extern uint32 MUX_Setbaud_rate(uint32 baud_rate);
extern BOOLEAN  GPIO_IsAllowedReportRssiToAP(void);
#endif
#endif

extern int32 g_is_cvs_config;            //for bringup
extern CVSI_CTRL_PARAM_NV_T *s_cvs_ctrl_param_nv_head_ptr;
uint16 s_cvs_audio_index=NV_ARM_CVS_PARA;
volatile CVSI_CTRL_PARAM_NV_T ** s_cvs_ctrl_param_head = &s_cvs_ctrl_param_nv_head_ptr;

//yeyuan added @20141016
/*****************************************************************************/
//  Description : This function handle the AT+SPADCVS command
//  Global resource dependence : g_rsp_str
//  Author:  ye.yuan
//  Note:
/*****************************************************************************/
//extern CVSI_CTRL_PARAM_NV_T *s_cvs_ctrl_param_nv_head_ptr;
extern int32 CVS_GetDevModeNum(void);
extern char* CVS_GetDevModeNameByIndex(int32 index);
extern void CVS_CreatParamList(void);
extern int32 CVS_CTRL_PARAM_SetParamToMem(CVSI_CTRL_PARAM_NV_T *cvs_ctrl_param_nv_ptr);
extern int32 CVS_CTRL_PARAM_SetParamToFlash(CVSI_CTRL_PARAM_NV_T *cvs_ctrl_param_nv_ptr);


#ifdef BT_HFU_SUPPORT
ATC_STATUS ATC_SPBTCTRL_HFU_PROCESS( ATC_CONFIG_T*atc_config_ptr,ATC_SPBTTEST_PARAM_T*param);
#endif

#ifdef BT_BLE_HOGP_SUPPORT
ATC_STATUS ATC_SPBTCTRL_HOGP_PROCESS( ATC_CONFIG_T*atc_config_ptr,ATC_SPBTTEST_PARAM_T*param);
#endif

#if defined( AUTO_TEST_SUPPORT)||defined(_ATC_UIX8910_ENABLE_)
/*Function pointer points to the function which is recveived string information from MMI.
It is used for auto test.
*/
static ATC_MMI_RECEIVER_FPTR    s_dummymmi_receiver_fptr = NULL;
static uint8    s_dummy_gsm_cell_num = 0;
static uint8    atc_3g_as_tag = 1;
static uint8    s_dummy_kpd_rsp[DUMMYMMI_RSP_MAX][MAX_DUMMY_KPD_RSP_LEN] = {0};

//extern BOOLEAN REFPARAM_GetEnableArmLogFlag(void);
extern void TRR_GetEmParam_gsm_cell_info(uint32 index,uint8 **value);
#if !defined( _ULTRA_LOW_CODE_)||defined(_ATC_UIX8910_ENABLE_)  // atc_atd_compact Nemge
/*****************************************************************************/
//  Description : convert the second parm of the AT+SPDUMMYKPD command  to key code value
//
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
LOCAL int GetVKeyCode(ATC_CUR_PARAM_INFO_LIST_T *param_ptr);
#endif
#endif

#ifndef WIN32
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
#ifdef BT_NONSIG_SUPPORT
static BOOLEAN spbttest_bt_rx_data_flag = FALSE;
static uint32 rx_data_show_pkt_cnt = 0;
static uint32 rx_data_show_pkt_err_cnt = 0;
static uint32 rx_data_show_bit_cnt = 0;
static uint32 rx_data_show_bit_err_cnt = 0;
static uint8   rx_data_show_rssi = 0;
static BT_STATUS rx_data_show_status = BT_SUCCESS;
#endif
#endif
#endif

#ifndef WIN32
#ifdef FM_SUPPORT
static uint32 s_fm_volum = 0;
static HAUDIO hAtcFmHandle = INVALID_HANDLE;
#if defined(VOLUME_LEVEL_15)
#define   FM_VOLUME_LEVEL_MAX   15
#elif defined(VOLUME_LEVEL_9)
#define FM_VOLUME_LEVEL_MAX   9
#else
#define FM_VOLUME_LEVEL_MAX   9
#endif
#endif
#endif

#if defined(TCPIP_SUPPORT) && !defined(IPVERSION_SUPPORT_V4_V6)    //avoid linker error
extern  int strnicmp (const char *a, const char *b, size_t c); /*lint !e18 !e526*/
#else
static int strnicmp(char * s1, char * s2, int len)
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
#endif

static int stricmp(char * s1, char * s2)
{
    int   l1,   l2;

    l1 = strlen(s1);  /* get both lengths */
    l2 = strlen(s2);

    /* call strnicmp with maximum length */
    return( strnicmp(s1, s2, ( l1 > l2 )?l1:l2 ));
}

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE) ||defined(_ATC_UIX8910_ENABLE_)
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
//  Description : This function convert the bcd code to digital string
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
void ConvertBcdToDigitalStr(
                uint8         length,
                uint8         *bcd_ptr,      // in: the bcd code string
                uint8         *digital_ptr   // out: the digital string
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

/*****************************************************************************/
//  Description : This function handles the data of CCID
//  Global resource dependence : g_rsp_str
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
LOCAL void HandleCcidData(
                uint8                *p_ccid,        //the input data
                uint8                *p_result  //the output data
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
//  Description : This function is used to save the current configuration profile.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void SaveConfigProfile(
                ATC_CONFIG_T   *atc_config_ptr
                );

#endif

#ifdef BLUETOOTH_SUPPORT_SPRD_BT
extern BT_TEST_MODE_T BT_GetTestMode(void);
extern void BT_SetTestMode(BT_TEST_MODE_T mode);
#endif

#ifdef FM_SUPPORT
extern uint32 FM_OPERATION_get_freq_seek_info();    //freq_seek 显示当前频率信息
extern uint32 FM_OPERATION_get_inpwr_info_rt();
extern uint32 FM_OPERATION_read_reg(uint32 reg_addr, uint32 *reg_data);  ///读寄存器
extern uint32 FM_OPERATION_write_reg(uint32 reg_addr, uint32 reg_data);   ///写寄存器reg_addr：寄存器地址，reg_data: 写入寄存器的值
extern uint32 FM_OPERATION_get_freq_offset_info();  //freq_offset 频偏信息
extern uint32 FM_OPERATION_get_agc_sts_info();  //当前agc 值
extern uint32 FM_OPERATION_IQrecover();
extern uint32 FM_OPERATION_IQmonitor();
extern uint32 FM_OPERATION_data_capture(uint32 level, uint32 i_data_num);
extern uint32 FM_OPERATION_long_ant_set();
extern uint32 FM_OPERATION_short_ant_set();
#endif

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
    return S_ATC_SUCCESS;
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
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE) || defined(_ATC_UIX8910_ENABLE_)
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
ATC_STATUS ATC_ProcessPreferPlmnChangCnf(
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

/*****************************************************************************/
//  Description : This function handle the AT+CREG command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCREG )
{
#ifdef _ATC_SYNC_TOOL_
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
#endif /* _ATC_SYNC_TOOL_ */
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
    uint8                                      rsrp  = AT_RXLEV_ZERO; // the received lte signal strength indication
    uint16                          rxqual_full  = AT_INVALID_RXQUAL;
    MN_DUAL_SYS_E               dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    MN_GMMREG_RAT_E current_rat = MN_GMMREG_RAT_UNKNOWN;

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
        current_rat = MNPHONE_GetCurrentRAT(dual_sys);
        SCI_TRACE_LOW("ATC_ProcessCSQ entry current_rat = %d\n", current_rat);

        if (MN_GMMREG_RAT_4G == current_rat)
        {
            cell_info = MNPHONE_GetRSRPEx(dual_sys);
            if (cell_info.rxlev < 0)
            {
                rsrp = 0;
            }
            else if ((0 < cell_info.rxlev) && (cell_info.rxlev < 97))
            {
                rsrp = cell_info.rxlev;
            }
            else
            {
                rsrp = 97;
            }

            SCI_TRACE_LOW("ATC_ProcessCSQ entry rsrp = %d\n", rsrp);
        }
        else
        {
            cell_info = MNPHONE_GetRSSIEx(dual_sys);

            if (cell_info.rxlev > 113)
            {
                rssi = 0;
            }
            else if ((cell_info.rxlev <= 113) && (cell_info.rxlev >= 51))
            {
                rssi = 31 - (cell_info.rxlev - 51) / 2;
            }
            else
            {
                rssi = 31;
            }

            SCI_TRACE_LOW("ATC_ProcessCSQ entry rssi = %d\n", rssi);
        }
        if (!cell_info.is_traffic_mode)
        {
            rxqual_full = AT_INVALID_RXQUAL;
        }
        else
        {
            rxqual_full = cell_info.rxqual_full;
        }
    }

    if (MN_GMMREG_RAT_4G == current_rat)
    {
        sprintf((char*)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CSQ].cmd_name->str_ptr,
                       rsrp, rxqual_full);
    }
    else
    {
        sprintf((char*)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_CSQ].cmd_name->str_ptr,
                       rssi, rxqual_full);
    }
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
#ifdef _ATC_SYNC_TOOL_
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
#endif /* _ATC_SYNC_TOOL_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CGMI command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCGMI )
{

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE) || defined(_ATC_UIX8910_ENABLE_)
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

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
    MN_IMEI_T           imei_arr; // the imei array
    uint8               temp_str[20];
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
#ifdef _NDS_EXT_CMD_
    uint8 rand_str[17];
    uint8 ndssign[17];
    int ndssign_len;
#endif
    // get the manufactory name
    if (!MNNV_GetIMEIEx(dual_sys, imei_arr))
    {
        SCI_TRACE_LOW("ATC: fail to get imei!");
        return ERR_MEMORY_FAILURE;
    }
    ConvertBcdToDigitalStr(MN_MAX_IMEI_LENGTH, imei_arr, temp_str);

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            sprintf((char*)g_rsp_str, "%s",temp_str);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

#ifdef _NDS_EXT_CMD_
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG && (16 ==PARA1.str_ptr->str_len))
            {
                SCI_MEMCPY(rand_str,PARA1.str_ptr->str_ptr,PARA1.str_ptr->str_len);
                rand_str[PARA1.str_ptr->str_len] = '\0';

                if(0 == sarft_sign_data((unsigned char *)temp_str,strlen((char *)temp_str),rand_str,strlen((char *)rand_str),
                                    s_cid,s_gmid,s_seed,ndssign,&ndssign_len))
                {
                    //SCI_TRACE_LOW:"ATC: sarft sign data fail."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_1092_112_2_17_22_16_32_120,(uint8*)"");
                    return ERR_UNKNOWN;
                }

                sprintf((char*)g_rsp_str,"+CGSN: %s,\"%s\",\"%s\"",
                temp_str,rand_str,ndssign);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

            }
            else
            {
                return ERR_INVALID_INDEX;
            }
            break;
        }
#endif
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

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
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

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
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

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE) || defined(_ATC_UIX8910_ENABLE_)
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

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
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

            SCI_TraceLow("ATC_ProcessCPIN atc_config_ptr->cpin[dual_sys]: %d", atc_config_ptr->cpin[dual_sys]);
            switch (atc_config_ptr->cpin[dual_sys])
            {
                case ATC_CPIN_CODE_READY:
                {
                    if (param[0].param_len != 0 && param[1].param_len != 0)
                    {
                        err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_UNBLOCK,
                                                   MNSIM_PIN1, &password, &new_password);
                        if(err_code == ERR_MNPHONE_NO_ERR)
                        {
                            ATC_Add_Expected_Event(atc_config_ptr,
                                              (uint16)APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN);
                            status = S_ATC_DEFAULT_HANDLE;
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }

                    break;
                }

                // ME is waiting SIM PIN to be given
                case ATC_CPIN_CODE_PIN1:
                {
                    if (0!=param[0].param_len && 0==param[1].param_len)
                    {
                        err_code = MNPHONE_ResponsePinEx(dual_sys, MNSIM_PIN1, FALSE, &password, PNULL);

                        if(ERR_MNPHONE_NO_ERR == err_code)
                        {
                        #ifdef _MUX_ENABLE_
                            if(!atc_config_ptr->sim_ready[dual_sys])
                            {
                                ATC_Add_Expected_Event(atc_config_ptr,
                                                 (uint16)APP_MN_SIM_READY_IND, 0xff, ATC_NO_DOMAIN, dual_sys);
                            }
                            else
                            {
                                ATC_Add_Expected_Event(atc_config_ptr,
                                                 (uint16)APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                            }
                        #endif
                            status = S_ATC_DEFAULT_HANDLE;
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: ATC_ProcessCPIN, PIN1,failure in MNPHONE_ResponsePin");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                        status = S_ATC_DEFAULT_HANDLE;
                    }
                    else
                    {
                        //when emergency, change pin with puk
                        if((!s_received_sim_ready[dual_sys]) &&
                            (param[0].param_len != 0) &&
                            (param[1].param_len != 0))
                        {
                            err_code = MNPHONE_ResponsePinEx(dual_sys, MNSIM_PIN1, FALSE, &new_password, &password);

                            if(ERR_MNPHONE_NO_ERR == err_code)
                            {
                                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                                                  (uint16)APP_MN_SIM_READY_IND, 0xff, ATC_NO_DOMAIN);
                                status = S_ATC_DEFAULT_HANDLE;
                            }
                            else
                            {
                                SCI_TRACE_LOW("ATC: ATC_ProcessCPIN, sim_flag:TRUE,PUK1, failure in MNPHONE_ResponsePin");
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else if (param[0].param_len != 0 && param[1].param_len != 0)
                        {
                            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_UNBLOCK,
                                                       MNSIM_PIN1, &password, &new_password);

                            if(err_code == ERR_MNPHONE_NO_ERR)
                            {
                                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                                                  (uint16)APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN);
                                status = S_ATC_DEFAULT_HANDLE;
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

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

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE) || defined(_ATC_UIX8910_ENABLE_)
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
ATC_STATUS  ATC_ProcessMnSyncInd(
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
AT_CMD_FUNC(ATC_ProcessE)
{
    ATC_STATUS         status = S_ATC_SUCCESS;

    if (ATC_CMD_TYPE_EXECUTE != ATC_GET_CMD_TYPE)
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessE Command Type not match");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(!PARAM1_FLAG)
    {
        atc_config_ptr->atc_config_nv.e_value = 0;
    }
    else
    {
        atc_config_ptr->atc_config_nv.e_value = PARA1.num;
    }

    if(0 == atc_config_ptr->atc_config_nv.e_value)
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
#ifdef PPP_USB
        SIO_ATC_SetEcho(FALSE,ATC_GetSioPort());
#else
        SIO_ATC_SetEcho(FALSE);
#endif
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
#ifdef PPP_USB
        SIO_ATC_SetEcho(TRUE,ATC_GetSioPort());
#else
        SIO_ATC_SetEcho(TRUE);
#endif
#endif
    }
    else
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    return status;
}


/*****************************************************************************/
//  Description : This function handle the ATX command
//  Global resource dependence : none
//  Author:       longting.zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessX)
{
    uint8 param_num = 0;

    if (ATC_CMD_TYPE_EXECUTE != ATC_GET_CMD_TYPE)
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessE Command Type not match");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    param_num = *(PARA1.str_ptr->str_ptr) - '0';

    SCI_TRACE_LOW("atc: para1=%d,", param_num);

    if(4 == param_num)
    {
        atc_config_ptr->connect_result_code_mode = ATC_CONNECT_WITH_BAUD;
    }

    return S_ATC_SUCCESS;
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

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
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

            MNCALL_DisconnectCallEx(dual_sys, (uint8)g_calls_state[dual_sys].call_context[i].call_id, MN_CAUSE_NORMAL_CLEARING, PNULL);

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

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE) || defined(_ATC_UIX8910_ENABLE_)
    SCI_DATE_T   cur_date = {0};
    SCI_TIME_T    cur_time = {0};
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
#if 0
#ifdef AUDIO_PARAM_ARM
                AUDIO_SetVolumeAndTypeEx(
                    INVALID_HANDLE,
                    atc_config_ptr->speaker_volume,
                    AUDIO_ARM_APP_TYPE_0);
#else
                AUDIO_SetVolume(atc_config_ptr->speaker_volume);
#endif
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
        ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SSEA);
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
 #if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE) || defined(_ATC_UIX8910_ENABLE_)
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
    MN_SMS_RECORD_ID_T   sim_total_amount = 0;
    MN_SMS_RECORD_ID_T   sim_used_amount = 0;
    MN_SMS_RECORD_ID_T   me_total_amount = 0;
    MN_SMS_RECORD_ID_T   me_used_amount = 0;
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

#if defined(_ATC_SYNC_TOOL_) || defined(_ATC_UIX8910_ENABLE_)
    //Get the SMS information
    GetStorageInfo(dual_sys,MN_SMS_STORAGE_SIM, &sim_total_amount, &sim_used_amount, &sim_message_flag);
    GetStorageInfo(dual_sys,MN_SMS_STORAGE_ME, &me_total_amount, &me_used_amount, &me_message_flag);
#endif
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

    MN_PHONE_CELL_MEAS_T           scell;

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

            MNPHONE_GetMeasReportEx(dual_sys, &ncell, &scell);

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
//    Description : This function handle the AT+CCED command
//  Global resource dependence : none
//    Author:       Xueli.lv
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

#if 0 // not used currently
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
#endif

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
#if defined( _ULTRA_LOW_CODE_ ) && !defined(ATC_ORANGE_ENABLE) && !defined(_ATC_UIX8910_ENABLE_)
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
#if 0
                AUDIO_SetUpLinkMute(atc_config_ptr->cmut_flag);

                SCI_SLEEP(50);
                LAYER1_SetVolume(atc_config_ptr->speaker_volume);
#else
                MMICC_EnableMute(atc_config_ptr->cmut_flag);
#endif
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

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)

    #ifdef ATC_ORANGE_ENABLE
        uint8       fun = 0xff;
    #else
        BOOLEAN       flag;
    #endif

    ATC_STATUS    status = ERR_INVALID_INDEX;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    SCI_TraceLow("ATC_ProcessCFUN");
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
        #ifdef ATC_ORANGE_ENABLE
            fun = PARA1.num;
        #else
            flag = (BOOLEAN)PARA1.num;
            atc_config_ptr->cfun_flag = PARA1.num;
        #endif

        #ifdef ATC_ORANGE_ENABLE
            if (1 == fun)
        #else
            if( flag )
        #endif
            {
            #ifdef _MUX_ENABLE_
                MNPHONE_PowerOnPsEx(dual_sys);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_PS_POWER_ON_CNF,0xff,ATC_NO_DOMAIN);
            #else
                //#if defined(_ATC_UIX8910_ENABLE_)
                //MNPHONE_PowerOnPsEx(dual_sys);
                //#else
                MNPHONE_StartupPsEx(dual_sys,MN_GMMREG_RAT_GPRS);
                //#endif
            #endif
                s_is_ps_activate[dual_sys] = TRUE;
                return S_ATC_SUCCESS;
            }
        #ifdef ATC_ORANGE_ENABLE
            else if (4 == fun)
        #else
            else
        #endif
            {
            #ifdef _MUX_ENABLE_
                MNPHONE_PowerOffPsEx(dual_sys);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_PS_POWER_OFF_CNF,0xff,ATC_NO_DOMAIN);
            #else
                MNPHONE_DeactivePsEx(dual_sys);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_DEACTIVE_PS_CNF,0xff,ATC_NO_DOMAIN);
            #endif
                s_is_ps_activate[dual_sys]   = FALSE;
                g_is_ps_deactivate[dual_sys] = TRUE;
                return  S_ATC_DEFAULT_HANDLE;
            }
        #ifdef ATC_ORANGE_ENABLE
            else
            {
                return  ERR_INVALID_INDEX;
            }
        #endif
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
    dev_number = CVS_GetDevModeNum();
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
                    sprintf((char*)g_rsp_str, "%s: %ld,\"%s\"",
                            g_atc_info_table[AT_CMD_SADM].cmd_name->str_ptr,
                            mode_index,
                            CVS_CTRL_PARAM_GetNameByIndex(mode_index));
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
        case ATC_SET_AUD_DEV_MODE:
        {
            status = S_ATC_SUCCESS;
            break;
        }

#ifdef AUDIO_PARAM_ARM
        case ATC_GET_AUD_ARM_MODE_NUM:
        {
            sprintf((char*)g_rsp_str, "%s: %ld", g_atc_info_table[AT_CMD_SADM].cmd_name->str_ptr, AUDIONVARM_GetModeNumber());
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
                    ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SADM_ARM);

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
                    ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SADMNV_ARM);

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
            status = ERR_INVALID_INDEX;
            //SCI_TRACE_LOW:"ATC: Assert SADM,invalid type."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4355_112_2_17_22_16_38_186,(uint8*)"");
            break;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SPLO command，Preferred operator
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

    BOOLEAN           flag = FALSE;

    uint32            mode_index = 0;

    AUDMOD_DEVMOD_T   *audio_data_ptr = NULL;

    AUDMOD_DEVMOD_T   *audio_temp_ptr = NULL;
    AUDMOD_RESULT_E   result;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    param_len             -= 1;
    data_length = strlen((char*)data_ptr);
    terminater_char = data_ptr[data_length - 1];

    mode_index = s_atc_admm_index;
    //SCI_TRACE_LOW:"ATC:SioSADMNVData,length:%d,terminater_char:%d,mode index:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_4936_112_2_17_22_16_39_214,(uint8*)"ddd",param_len,terminater_char,mode_index);

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

    if (NULL != audio_data_ptr)
    {
        SCI_FREE(audio_data_ptr);
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
//  Description : This function handles the APP_MN_SIM_NOT_READY_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessNotReadyInd(
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
            ATC_UpSINDInfo(dual_sys, atc_config_ptr, PNULL, ATC_WIND_BIT3, 4);
        }
    }
#endif

#ifndef _ULTRA_LOW_CODE_
    if (s_received_sim_ready[dual_sys])
    {
        if (sig_ptr->reason == MNSIM_ERROR_REMOVED)
        {

        }
    }
#endif
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

    //Send the unsolicited +SIND: 0 and 10
    if(FALSE == atc_config_ptr->sim_flag[dual_sys])
    {
        ATC_UpSINDInfo(dual_sys, atc_config_ptr, PNULL, ATC_WIND_BIT0, 0);
    }

    ATC_UpSINDInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, ATC_WIND_BIT9, 10);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_READY_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessReadyInd(
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
        ATC_UpSINDInfo(dual_sys, atc_config_ptr, PNULL, ATC_WIND_BIT3, 4);
    }
#endif


 #if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
    s_received_sim_ready[dual_sys] = TRUE;
#endif

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
    ATC_UpSINDInfo(dual_sys, atc_config_ptr, PNULL, ATC_WIND_BIT0, 1);

    //Send the unsolicited +SIND: 10
    ATC_UpSINDInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, ATC_WIND_BIT9, 10);

    return S_ATC_SUCCESS;
}
#if !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_UIX8910_ENABLE_)
/*****************************************************************************/
//  Description : This function handles the APP_MN_AOC_INCREASE_ACM_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessIncreaseAcmInd(
                      ATC_CONFIG_T                   *atc_config_ptr,
                      APP_MN_AOC_MODIFIED_IND_T      *sig_ptr
                      )
{
    uint32            ccm_val;
    uint8 *pData = NULL;

    ccm_val = sig_ptr->acm_unit;

    switch (atc_config_ptr->aoc_mode)
    {
        case ATC_AOC_QUERY_MODE:
        case ATC_AOC_DEACTIVE_MODE:
        {
            if (atc_config_ptr->operate == ATC_PIN_RESET_ACM_UNIT)
            {
                if (sig_ptr->is_ok)
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                }
                else
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
                }
            }
            else if (atc_config_ptr->operate == ATC_PIN_GET_ACM)
            {
                if (sig_ptr->is_ok)
                {
                    pData = (uint8 *)&(sig_ptr->acm_unit);
                    sprintf((char*)g_rsp_str, "+CACM: \"%02X%02X%02X\"", pData[0], pData[1], pData[2]);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                }
                else
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
                }
            }
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

/*****************************************************************************/
//  Description : This function handles the APP_MN_SCELL_RSSI_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessRssiInd(
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
//  Description : This function handles the ATC_CHR_CAP_IND signal.And inform the
//  DTE the current battery level
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessChrCapInd(
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
ATC_STATUS ATC_ProcessChrStartInd(
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

/*****************************************************************************/
//  Description : This function handles the ATC_CHR_CHARGE_END_IND signal.Inform
//  that the battery starts charge.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessChrEndInd(
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
    #ifndef _SUPPORT_GPRS_  //CR171132 for 6530 codesize cutdown
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

#ifndef _ULTRA_LOW_CODE_
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
#ifdef _ATC_SYNC_TOOL_
    ATC_WriteNVItem(atc_config_ptr);
#endif
}

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
//  Description : This function handles the ATC_ProcessPowerOnCnf signal
//  that the battery starts charge.
//  Global resource dependence : g_rsp_str
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessPsPowerOnCnf(ATC_CONFIG_T *atc_config_ptr,
                    xSignalHeaderRec              *sig_ptr,
                    uint8 bExpect)
{
#if !defined( _ULTRA_LOW_CODE_ )||defined(_ATC_UIX8910_ENABLE_)
    MN_PHONE_CURRENT_PLMN_INFO_T   register_status;
    OPER_STATUS_E            oper_status;

    if(bExpect)
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);

    //add this process, for user set at+cfun=1 in times
    register_status = MNPHONE_GetCurrentPLMNInfoEx(((APP_MN_PS_POWER_ON_CNF_T*)sig_ptr)->dual_sys);
    SCI_TRACE_LOW("ATC_ProcessPsPowerOnCnf");

    atc_config_ptr->cfun_flag = 1;

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
ATC_STATUS ATC_ProcessPsPowerOffCnf(ATC_CONFIG_T                  *atc_config_ptr,
                    xSignalHeaderRec              *sig_ptr,
                    uint8 bExpect)
{

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
#ifdef _MUX_REDUCED_MODE_
    SCI_MEMSET((void *)g_rxlev_arr,0,sizeof(g_rxlev_arr));
#endif
    //s_setup_menu_len[MN_DUAL_SYS_1] = 0;
    //s_setup_menu_len[MN_DUAL_SYS_2] = 0;
    //SCI_MEMSET(&s_setup_menu_data[MN_DUAL_SYS_1][0], 0, SIMAT_MAX_RPDU_DATA_LEN);
    //SCI_MEMSET(&s_setup_menu_data[MN_DUAL_SYS_2][0], 0, SIMAT_MAX_RPDU_DATA_LEN);
    atc_config_ptr->cpas          = ATC_CPAS_STATUS_READY;
    g_creg_state[((APP_MN_PS_POWER_OFF_CNF_T *)sig_ptr)->dual_sys].oper_status = NOT_REGISTERED_NOT_SEARCHING;

    if(bExpect)
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);

    SCI_TRACE_LOW("ATC_ProcessPsPowerOffCnf");
    atc_config_ptr->cfun_flag = 0;

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
    #ifdef _MUX_REDUCED_MODE_
    sprintf((char*)g_rsp_str, "+CIEV: 0");
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    #endif


    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

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
ATC_STATUS ATC_ProcessSetBandCnf(
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
        ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SRWD);

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
#ifdef _ATC_SYNC_TOOL_
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
#endif /* _ATC_SYNC_TOOL_ */
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
ATC_STATUS ATC_ProcessCrsmCnf(
                ATC_CONFIG_T         *atc_config_ptr,
                APP_MN_CRSM_CNF_T    *sig_ptr
                )
{
#if !defined(_ULTRA_LOW_CODE_) || defined(_ATC_UIX8910_ENABLE_)
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
#ifndef _ULTRA_LOW_CODE_ //reduce atc code  Nemge
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
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_  */
}

/*****************************************************************************/
//  Description : This function handles the AT+SSYS command.This command is used
//  to set or get the operated sim id .
//  Global resource dependence : g_rsp_str
//  Author:       Shijun.cui
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSYS )
{
#ifdef _ATC_SYNC_TOOL_
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
#endif /* _ATC_SYNC_TOOL_ */
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
//    Description : This function handles the AT+SFTIN command.
//                          to set at+sftin  paraments
//  Global resource dependence : none
//    Author:       Shuyan.hu
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
    typedef uint32 (*CMD_HANDLER)(osiResetMode_t);
    typedef struct
    {
        uint8*      p_cmd_str;
        CMD_HANDLER cmd_handler;
    }AT_CMD_ENTRY;
    static const AT_CMD_ENTRY s_cmd_entry[] =
    {
        {(uint8*)"AUTODLOADER", osiReset}
    };

    uint32 i;
    uint8  *temp_p0, *temp_p1, ch;
    uint8 *at_string = NULL;

    if(PARA1.str_ptr->str_ptr == NULL)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    at_string = PARA1.str_ptr->str_ptr;

    //Convert strings to upper case and delete redundant space
    temp_p0 = at_string;
    temp_p1 = at_string;
    ch = *temp_p1++;
    while(ch)
    {
        if(ch == ' ')
        {
            while(*temp_p1 == ' '){temp_p1++;};
            *temp_p0++ = ' ';
            ch = *temp_p1++;
            continue;
        }
        else if('a' <= ch && ch <= 'z')
        {
            ch -= 32;
        }
        *temp_p0++ = ch;
        ch = *temp_p1++;
    }
    *temp_p0++ = 0;

    //Search CMD table.
    for(i = 0; i < sizeof(s_cmd_entry)/sizeof(AT_CMD_ENTRY); i++)
    {
        if(strncmp((int8*)at_string, (int8*)s_cmd_entry[i].p_cmd_str,
            strlen((int8*)s_cmd_entry[i].p_cmd_str)) == 0)
        {
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
            s_cmd_entry[i].cmd_handler(OSI_RESET_FORCE_DOWNLOAD);
            return S_ATC_DEFAULT_HANDLE;
        }
    }

    return ERR_OPERATION_NOT_ALLOWED;
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
//#if 0
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

    dev_number = AUDIONVARM_GetModeNumber();

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
                    ATC_SendAsyncInfo(atc_config_ptr, ((0 == type) ? ATC_CTRLZ_ENHA : ATC_CTRLZ_ENHA_TUN));
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
                    ATC_SendAsyncInfo(atc_config_ptr, ((0 == type) ? ATC_CTRLZ_ENHANV : ATC_CTRLZ_ENHANV_TUN));
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
    return S_ATC_DEFAULT_HANDLE;
}

AT_CMD_FUNC(ATC_ProcessSPADCVS)
{
    ATC_STATUS        status = S_ATC_SUCCESS;
    uint8             type = 0;
    int32            mode_index = 0;
    uint32            dev_number = 0;
    CVSI_CTRL_PARAM_NV_T *cvs_ctrl_param_nv_ptr = PNULL;
    char *  mode_name_ptr = PNULL;
    uint8   *str_ptr = PNULL;

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
        return ERR_INVALID_INDEX;
    }

    //To do
    dev_number = (uint32)CVS_GetDevModeNum();

    if(type >= ATC_GET_CVS_MODE_NAME)
    {

        if((PARAM2_FLAG) &&(PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
        {
            mode_index = (int32)PARA2.num;
            if(mode_index >= (int32)dev_number)
            {
                return ERR_INVALID_INDEX;
            }
        }
        else
        {
            return ERR_INVALID_INDEX;
        }
    }
    switch(type)
    {
    case ATC_GET_CVS_MODE_NUM:
        sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SPADCVS].cmd_name->str_ptr, dev_number);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    case ATC_GET_CVS_MODE_NAME:
        mode_name_ptr = CVS_GetDevModeNameByIndex(mode_index);
        if(PNULL == mode_name_ptr)
        {
            return ERR_NOT_FOUND;
        }
        sprintf((char*)g_rsp_str, "%s: %d,\"%s\"",
                g_atc_info_table[AT_CMD_SPADCVS].cmd_name->str_ptr,
                mode_index,
                mode_name_ptr);

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    case ATC_GET_CVS_MODE_MEM_DATA:

        if (s_cvs_ctrl_param_nv_head_ptr == NULL)
        {
            CVS_CreatParamList();
        }
        mode_name_ptr=CVS_GetDevModeNameByIndex(mode_index);
        {
            CVSI_CTRL_PARAM_NV_T *temp;

            temp = *s_cvs_ctrl_param_head;
            if(PNULL == temp)
            {
                SCI_TRACE_LOW("ATC: +SPADCVS, temp null");
                return ERR_NOT_FOUND;
            }

            temp = (CVSI_CTRL_PARAM_NV_T *)temp+ mode_index;
            cvs_ctrl_param_nv_ptr=(CVSI_CTRL_PARAM_NV_T *)SCI_ALLOC(sizeof(CVSI_CTRL_PARAM_NV_T));
            if (PNULL == cvs_ctrl_param_nv_ptr)
            {
                return ERR_NOT_FOUND;
            }
            SCI_MEMCPY(cvs_ctrl_param_nv_ptr, temp, sizeof(CVSI_CTRL_PARAM_NV_T));
            str_ptr = (uint8*)SCI_ALLOC((sizeof(CVSI_CTRL_PARAM)+1)*2);
            if (PNULL == str_ptr)
            {
                SCI_FREE(cvs_ctrl_param_nv_ptr);
                return ERR_NOT_FOUND;
            }
            SCI_MEMSET(str_ptr, 0, ((sizeof(CVSI_CTRL_PARAM)+1)*2));
            ConvertBinToHex((uint8*)&(cvs_ctrl_param_nv_ptr->CVS_param_NV), sizeof(CVSI_CTRL_PARAM), str_ptr);

            sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%s",
                    g_atc_info_table[AT_CMD_SPADCVS].cmd_name->str_ptr,
                    mode_index,
                    mode_name_ptr,
                    str_ptr);

            temp = PNULL;
            SCI_FREE(cvs_ctrl_param_nv_ptr);
            SCI_FREE(str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        break;
    case ATC_SET_CVS_MODE_MEM_DATA:

        s_atc_admm_index = (uint8)PARA2.num;
        // output the '>'
        sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
        ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_CVS);
        break;

    case ATC_GET_CVS_MODE_NV_DATA:
        {
            CVSI_CTRL_PARAM_NV_T *cvs_param_nv_temp_t = PNULL;
            CVSI_CTRL_PARAM_NV_T *temp = PNULL;
            NVITEM_ERROR_E  nv_err = NVERR_NONE;
            cvs_param_nv_temp_t = (CVSI_CTRL_PARAM_NV_T *)SCI_ALLOC(sizeof(CVSI_CTRL_PARAM_NV_T)*dev_number);
            if (PNULL == cvs_param_nv_temp_t)
            {
                return ERR_NOT_FOUND;
            }
            nv_err = EFS_NvitemRead(s_cvs_audio_index, sizeof(CVSI_CTRL_PARAM_NV_T)*dev_number, (uint8 *)cvs_param_nv_temp_t);
            if (nv_err != NVERR_NONE)
            {
                SCI_TRACE_LOW( "%d%d", "%d\1%d", "ATC: +SPADCVS, NvitemRead_fail,nv item id=%d,err=%d", 2, ( s_cvs_audio_index, nv_err));
                SCI_FREE(cvs_param_nv_temp_t);
                return ERR_INVALID_INDEX;
            }

            cvs_ctrl_param_nv_ptr = (CVSI_CTRL_PARAM_NV_T *)SCI_ALLOC(sizeof(CVSI_CTRL_PARAM_NV_T));
            if (PNULL == cvs_ctrl_param_nv_ptr)
            {
                SCI_FREE(cvs_param_nv_temp_t);
                return ERR_NOT_FOUND;
            }
            mode_name_ptr=CVS_GetDevModeNameByIndex(mode_index);

            temp=cvs_param_nv_temp_t+mode_index;
            SCI_MEMCPY(cvs_ctrl_param_nv_ptr, temp, sizeof(CVSI_CTRL_PARAM_NV_T));
            str_ptr = (uint8*)SCI_ALLOC((sizeof(CVSI_CTRL_PARAM)+1)*2);
            if (PNULL == str_ptr)
            {
                SCI_FREE(cvs_param_nv_temp_t);
                SCI_FREE(cvs_ctrl_param_nv_ptr);
                return ERR_NOT_FOUND;
            }
            SCI_MEMSET(str_ptr, 0, ((sizeof(CVSI_CTRL_PARAM)+1)*2));
            ConvertBinToHex((uint8*)&(cvs_ctrl_param_nv_ptr->CVS_param_NV), sizeof(CVSI_CTRL_PARAM),str_ptr);
            sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%s",
                    g_atc_info_table[AT_CMD_SPADCVS].cmd_name->str_ptr,
                    mode_index,
                    mode_name_ptr,
                    str_ptr);

            SCI_FREE(cvs_param_nv_temp_t);
            SCI_FREE(cvs_ctrl_param_nv_ptr);
            SCI_FREE(str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        break;

    case ATC_SET_CVS_MODE_NV_DATA:
        s_atc_admm_index = (uint8)PARA2.num;
        // output the '>'
        sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg,atc_config_ptr->s4_reg);
        ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_CVSNV);
        break;
    default:
        status = ERR_OPERATION_NOT_SUPPORTED;
        break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPADCVS
//  Global resource dependence : g_rsp_str
//  Author: ye.yuan
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSADCVSData(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *data_ptr,  // pointer to the data
    uint16          param_len
)
{
    uint8             terminater_char;     // indicates ther terminater char
    BOOLEAN           flag = FALSE;
     /* BUG655840 REMOVED */

    CVSI_CTRL_PARAM_NV_T*cvs_ctrl_param_nv_ptr = PNULL;

    SCI_ASSERT((PNULL != atc_config_ptr) && (PNULL != data_ptr));/*assert verified*/

    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len             -= 1;

    cvs_ctrl_param_nv_ptr = (CVSI_CTRL_PARAM_NV_T *)SCI_ALLOC(sizeof(CVSI_CTRL_PARAM_NV_T));
    SCI_ASSERT(NULL != cvs_ctrl_param_nv_ptr); /*assert verified*/
    SCI_MEMSET((uint8*)cvs_ctrl_param_nv_ptr, 0, (sizeof(CVSI_CTRL_PARAM_NV_T)));


    switch (terminater_char)
    {
    case ESC:
        flag = TRUE;
        break;

    case CTRL_Z:
        if(param_len == sizeof(CVSI_CTRL_PARAM_NV_T)*2)
        {
            ConvertHexToBin(data_ptr, param_len, (uint8*)cvs_ctrl_param_nv_ptr);
            CVS_CTRL_PARAM_SetParamToMem(cvs_ctrl_param_nv_ptr);
            g_is_cvs_config = 0;
            flag = TRUE;
        }
        else
        {
            //SDI_TRACEID(ATC, ATC__SioSADLVVEData_InputDataLenErr, "ATC: SioSADLVVEData,InputDataLenErr");
            SCI_TRACE_LOW("ATC: SioSADLVVEData,InputDataLenErr");
        }
        break;

    default:
        break;
    }

    SCI_FREE(cvs_ctrl_param_nv_ptr);


    if (flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        //ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,param_len);
    }

    return S_ATC_DEFAULT_HANDLE;

}
/*****************************************************************************/
//  Description : This function handles the data send from the SIO when AT+SPADLVVE
//  Global resource dependence : g_rsp_str
//  Author: ye.yuan
//  Note:
/*****************************************************************************/
PUBLIC uint32 ATC_ProcessSioSADCVSNVData(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *data_ptr,  // pointer to the data
    uint16          param_len
)
{
    uint8             terminater_char;     // indicates ther terminater char
    BOOLEAN           flag = FALSE;
     /* BUG655840 REMOVED */
    CVSI_CTRL_PARAM_NV_T*cvs_ctrl_param_nv_ptr = PNULL;

    SCI_ASSERT((PNULL != atc_config_ptr) && (PNULL != data_ptr));/*assert verified*/

    terminater_char      = data_ptr[param_len - 1];
    data_ptr[param_len - 1] = '\0';
    param_len             -= 1;

    cvs_ctrl_param_nv_ptr = (CVSI_CTRL_PARAM_NV_T *)SCI_ALLOC(sizeof(CVSI_CTRL_PARAM_NV_T));
    SCI_ASSERT(NULL != cvs_ctrl_param_nv_ptr);/*assert verified*/
    SCI_MEMSET((uint8*)cvs_ctrl_param_nv_ptr, 0, (sizeof(CVSI_CTRL_PARAM_NV_T)));


    switch (terminater_char)
    {
    case ESC:
        flag = TRUE;
        break;

    case CTRL_Z:
        if(param_len == sizeof(CVSI_CTRL_PARAM_NV_T)*2) {
            ConvertHexToBin(data_ptr, param_len, (uint8*)cvs_ctrl_param_nv_ptr);
            CVS_CTRL_PARAM_SetParamToFlash(cvs_ctrl_param_nv_ptr);
            flag = TRUE;
        } else {
            //SDI_TRACEID(ATC, ATC__SioSADLVVEData_InputDataLenErr, "ATC: SioSADLVVEData,InputDataLenErr");
            SCI_TRACE_LOW("ATC: SioSADLVVEData,InputDataLenErr");
        }
        break;

    default:
        break;
    }

    SCI_FREE(cvs_ctrl_param_nv_ptr);

    if (flag) {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    } else {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
    }

    return S_ATC_DEFAULT_HANDLE;

}


AT_CMD_FUNC(ATC_ProcessPEINFO)
{
    ATC_STATUS status = S_ATC_SUCCESS;
    char *buffer_ptr = NULL;
    uint32 retlength = 0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            buffer_ptr = (char*)SCI_ALLOC(AUDIO_AT_PEINFO_LENGTH);
            SCI_MEMCPY(buffer_ptr,0,AUDIO_AT_PEINFO_LENGTH);
            retlength = AUDIO_Get_PhoneInfo((void *)buffer_ptr);
            if(retlength > 0) {
                sprintf((char *)g_rsp_str, "+PEINFO:");
                AUDIO_bin2ascii(g_rsp_str+strlen("+PEINFO:"),buffer_ptr,retlength);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            } else {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            SCI_FREE(buffer_ptr);
            break;

        default:
        {
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
        }
    }
    return status;
}

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
/*****************************************************************************/
//  Description : This function initializes the static variables
//  Global resource dependence :
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
 void ATC_InitBasicVar(void)
{
    uint8    i = 0;

    for (i = MN_DUAL_SYS_1; i < MN_SYS_NUMBER; i++)
    {
        g_is_ps_deactivate[i] = FALSE;
        s_is_ps_activate[i]     = FALSE;
    }

}

/*****************************************************************************/
// Description : This function convert the bcd code to digital string
// Global resource dependence : none
// Author:       Xueli Lv
// Note:
/*****************************************************************************/
void ConvertBcdToDigitalStr(
                uint8         length,
                uint8         *bcd_ptr,      // in: the bcd code string
                uint8         *digital_ptr   // out: the digital string
                )
{
    int32               i = 0;
    uint8         temp = 0;

    // get the first digital
    temp = ((*bcd_ptr >> 4) &0x0f);
    if (temp >= 0x0a)
    {
        *digital_ptr = (temp - 0x0a) + 'A';
    }
    else
    {
        *digital_ptr = temp + '0';
    }

    bcd_ptr++;
    digital_ptr++;


    for (i=0; i<(length - 1); i++)
    {
        temp = *bcd_ptr;
        // get the low 4 bits
        temp &= 0x0f;
        // A -- F
        if (temp >= 0x0a)
        {
            *digital_ptr = (temp - 0x0a) + 'A';
        }
        else
        {
            // 1 -- 9
            *digital_ptr = temp + '0';
        }
        digital_ptr++;

        temp = *bcd_ptr;
        // get the high 4 bits
        temp = (temp & 0xf0) >> 4;

        if ((temp == 0x0f) && (i == (length -1)))
        {
            *digital_ptr = '\0';
            return;
        }
        // A -- F
        else if (temp>=0x0a)
        {
            *digital_ptr = (temp - 0x0a) + 'A';
        }
        else
        {
            // 1 -- 9
            *digital_ptr = temp + '0';
        }
        digital_ptr++;
        bcd_ptr++;
    }
    *digital_ptr = '\0';
}

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
    atc_config_ptr->atc_config_nv.wind_ind_level            = ATC_SIND_DEFAULT_VAL;
    //The following is about STK
    //atc_config_ptr->atc_config_nv.stk_actflag   = TRUE;
    //atc_config_ptr->atc_config_nv.stk_time      = ATC_STKTIME_DEFAULT_VAL;
#if 0
#ifndef _ULTRA_LOW_CODE_
    ATC_CheckAndHandleConfig(atc_config_ptr,g_stk_terminal_profile[AT_STK_TRP_MAX_VAL]);
#endif /* _ULTRA_LOW_CODE_ */
#endif
    strcpy((char*)atc_config_ptr->cscs, "GSM");
    ATC_RestoreSmsConfig(dual_sys);
    #ifdef _ATC_SYNC_TOOL_
    // Save to the NV ram.
    ATC_WriteNVItem(atc_config_ptr);
    #endif

#ifdef _MUX_ENABLE_
    ATC_SetEcho(atc_config_ptr->atc_config_nv.e_value);
    SIO_ATC_SetEcho(atc_config_ptr->atc_config_nv.e_value);
#else
#ifdef PPP_USB
    SIO_ATC_SetEcho(atc_config_ptr->atc_config_nv.e_value,ATC_GetSioPort());
#else
    SIO_ATC_SetEcho(atc_config_ptr->atc_config_nv.e_value);
#endif
#endif
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


/*****************************************************************************/
//  Description : This function handles the APP_MN_SIM_GET_PIN_IND signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessPinInd(
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
        ATC_UpSINDInfo(dual_sys, atc_config_ptr, PNULL, ATC_WIND_BIT3, 4);
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
//  Description : This function handles the APP_MN_DETACH_GSM_CNF signal
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessPowerOffCnf(
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
    //ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);

    s_is_ps_activate[dual_sys] = FALSE;

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handle the APP_MN_NETWORK_STATUS_IND signal and
//                send the unsolicited result code
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessNetworkStatusInd(
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
                ATC_UpSINDInfo(sig_ptr->dual_sys, atc_config_ptr, PNULL, ATC_WIND_BIT7, 8);

                break;
            }

            case PLMN_EMERGENCY_ONLY:
            {
                oper_status = REGISTRATION_DENIED;

                //Send the unsolicited +SIND: 7
                ATC_UpSINDInfo(sig_ptr->dual_sys, atc_config_ptr, PNULL, ATC_WIND_BIT6, 7);

                s_is_ps_activate[sig_ptr->dual_sys] = TRUE;

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
                ATC_UpSINDInfo(sig_ptr->dual_sys, atc_config_ptr, PNULL, ATC_WIND_BIT6, 11);

                s_is_ps_activate[sig_ptr->dual_sys] = TRUE;
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
    }
    return S_ATC_SUCCESS;
}
#endif

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

                    name_len = strlen(name_ptr);
                    if (name_len < AUDIO_NV_ARM_MODE_NAME_MAX_LEN)
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
#endif/*AUDIO_PARAM_ARM*/

/*****************************************************************************/
//  Description : This function handle the AT&C command
//  Global resource dependence : none
//  Author:       Ivan.yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_C )
{
    SCI_TRACE_LOW("ATC: ATC_Processand_C");
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT&D command
//  Global resource dependence : none
//  Author:       Ivan.yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_D )
{
    SCI_TRACE_LOW("ATC: ATC_Processand_D");
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : register callback funtion for dummy mmi keypad response.
//                It is used by ATC.
//  Global resource dependence : none
//  Author: minqian.qian
//  Note:
/*****************************************************************************/
MN_RETURN_RESULT_E ATC_RegisterMMIKpdRspFun(
    ATC_MMI_RECEIVER_FPTR      callback_ptr
)
{
#ifdef AUTO_TEST_SUPPORT
    SCI_TRACE_LOW("Enter register mmi");
    if (NULL == callback_ptr || NULL == *callback_ptr)
    {
        SCI_TRACE_LOW("ATC_RegisterMMIKpdRspFun failed!");
        return MN_RETURN_FAILURE;
    }

    s_dummymmi_receiver_fptr = callback_ptr;
#endif
    return MN_RETURN_SUCCESS;
}
/*****************************************************************************/
//  Description : It is used by MMI to send dummy information to ATC
//  Global resource dependence : none
//  Author: minqian.qian
//  Note:
/*****************************************************************************/
MN_RETURN_RESULT_E ATC_SendRspInfoToAT(
    const uint8 *str_ptr,      //Point to the input message
    uint16       str_len       //input message length
)
{
#ifdef AUTO_TEST_SUPPORT
    SCI_TRACE_LOW("Begin to send mmi info");
    if (NULL != s_dummymmi_receiver_fptr)
    {
        (*s_dummymmi_receiver_fptr)(str_ptr,(uint32)str_len);
    }
    else
    {
        return MN_RETURN_FAILURE;
    }
#endif
    return MN_RETURN_SUCCESS;
}

#ifdef AUTO_TEST_SUPPORT
/*****************************************************************************/
/*  Description : Auto Poweron Flag */
/*  Global resource dependence : none */
/*  Author:Joe.Ni */
/*  Note: */
/*****************************************************************************/
void ATC_REFPARAM_SetAutoPowerOnFlag(uint8 mode)
{
    if(1 == mode)
    {
        REFPARAM_SetAutoPowerOnFlag(TRUE);
    }
    else
    {
        REFPARAM_SetAutoPowerOnFlag(FALSE);
    }
}
/*****************************************************************************/
/*  Description : Get Auto Poweron Flag */
/*  Global resource dependence : none */
/*  Author:Joe.Ni */
/*  Note: */
/*****************************************************************************/
uint8 ATC_REFPARAM_GetAutoPowerOnFlag(void)
{
    return ((TRUE == REFPARAM_GetAutoPowerOnFlag()) ? 1 : 0);
}
/*****************************************************************************/
//  Description : This function initializes the static variables for basic_cmd module
//  Global resource dependence :
//  Note: modify by vicky.wang
/*****************************************************************************/
void ATC_InitEngVar(void)
{
    ATC_RegisterMMIKpdRspFun(ATC_ProcessAutoTestInfoFromMMI);
}


/*****************************************************************************/
//  Description : This function is a callback function, it receives and
//                      handles the message which is sent by MMI.
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
int32  ATC_ProcessAutoTestInfoFromMMI(
    const uint8   *str_ptr,     //the string type refer to AT+SPDUMMYKPDRSP
    uint32        str_len
)
{
    if(NULL == str_ptr)
    {
        return 1;
    }

    if(str_len > 0)
    {
         uint8   index = DUMMYMMI_RSP_MAX;

        SCI_TRACE_LOW("ATC: ATC_ProcessDummyKPDRsp,data:%s",str_ptr);
        str_len = MIN((MAX_DUMMY_KPD_RSP_LEN - 1), str_len);

        if(0 == strncmp((char *)str_ptr, "MESSAGE", strlen("MESSAGE")))
        {
            /*the last message box information of MMI*/
            index = DUMMYMMI_RSP_MESSAGE;
        }
        else if(0 == strncmp((char *)str_ptr, "OPENWIN", strlen("OPENWIN")) ||
                0 == strncmp((char *)str_ptr, "CLOSEWIN", strlen("CLOSEWIN")))
        {
            /*the last win state information of MMI*/
            index = DUMMYMMI_RSP_WIN;
        }
        else if(0 == strncmp((char *)str_ptr, "BROWSERTITLE", strlen("BROWSERTITLE")))
        {
            /*the last browser title information of MMI*/
            index = DUMMYMMI_RSP_BROWSERTITLE;
        }
        else if(0 == strncmp((char *)str_ptr, "BROWSERCURSOR", strlen("BROWSERCURSOR")))
        {
            /*the last browser cursor state information of MMI*/
            index = DUMMYMMI_RSP_BROWSERCURSOR;
        }
        else if(0 == strncmp((char *)str_ptr, "SOFTKEY", strlen("SOFTKEY")))
        {
            /*the last window softkey information of MMI*/
            index = DUMMYMMI_RSP_SOFTKEY;
        }
        else if(0 == strncmp((char *)str_ptr, "IMAGEID", strlen("IMAGEID")))
        {
            /*the last image id information of MMI*/
            index = DUMMYMMI_RSP_IMAGEID;
        }
        else if(0 == strncmp((char *)str_ptr, "FOCUSWIN", strlen("FOCUSWIN")))
        {
            /*current focus window information of MMI*/
            index = DUMMYMMI_RSP_FOCUSWIN;
        }
        else if(0 == strncmp((char *)str_ptr, "WINSTRING", strlen("WINSTRING")))
        {
            /*current window context information of MMI*/
            index = DUMMYMMI_RSP_WINSTRING;
        }

        else if(0 == strncmp((char *)str_ptr, "BACKLIGHTS", strlen("BACKLIGHTS")))
        {
            /*current backlight state of MMI*/
            index = DUMMYMMI_RSP_BACKLIGHT;
        }

        if(DUMMYMMI_RSP_MAX != index)
        {
            SCI_MEMSET((void *)s_dummy_kpd_rsp[index], 0x0, MAX_DUMMY_KPD_RSP_LEN);
            SCI_MEMCPY(s_dummy_kpd_rsp[index], str_ptr, str_len);

	    SCI_TRACE_LOW("Enter the str");
        }
    }
    else
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessAutoTestInfoFromMMI,ERROR,wrong string length:%d", str_len);
    }

    return 0;
}
#endif

#if defined( AUTO_TEST_SUPPORT)|| !defined( _ULTRA_LOW_CODE_)  ||defined(_ATC_UIX8910_ENABLE_)// atc_atd_compact  Nemge
/*****************************************************************************/
//  Description : This function is used convert string data to number
//
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
BOOLEAN ConvertStringtoUint16(uint8 *str_ptr, uint16 length, uint16 *num_ptr)
{
    uint16  i;
    uint16  numeric_value = 0;
    uint16  numeric_max_value;

    if(NULL == str_ptr || length < 1)
    {
        return FALSE;
    }

    numeric_max_value = (65535 - 9) / 10;

    for(i = 0; i < length; ++i)
    {
        if(str_ptr[i] >= '0' && str_ptr[i] <= '9')
        {
            if(numeric_value > numeric_max_value)
            {
                SCI_TRACE_LOW("ATC: invalid param, numeric value overflow, return position:%d", i);
                return FALSE;
            }

            numeric_value = numeric_value * 10 + (int32)(str_ptr[i] - '0');
        }
        else
        {
            SCI_TRACE_LOW("ATC: invalid param, not numeric, return position:%d", i);
            return FALSE;
        }
    }

    SCI_TRACE_LOW("ATC: get the number: %d", numeric_value);

    if(NULL != num_ptr)
    {
        *num_ptr = numeric_value;
    }

    return TRUE;
}
/*****************************************************************************/
//  Description : convert the second parm of the AT+SPDUMMYKPD command  to key code value
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
LOCAL int GetVKeyCode(ATC_CUR_PARAM_INFO_LIST_T *param_ptr)
{
    int key_code = 0xFFFF;

    if (PNULL == param_ptr)
        return key_code;

    if (ATC_CMD_PARAM_TYPE_NUMERIC == param_ptr->param_type)
    {
        if (param_ptr->param_info.num >=0 && param_ptr->param_info.num <= 9)
        {
            /*SCI_VK_0 ... SCI_VK_9*/
            key_code = param_ptr->param_info.num + '0';
        }
    }
    else if (ATC_CMD_PARAM_TYPE_STRING == param_ptr->param_type)
    {
        SCI_ASSERT(PNULL != param_ptr->param_info.str_ptr);

        if (1 == param_ptr->param_info.str_ptr->str_len)
        {
            if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"0") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"1") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"2") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"3") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"4") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"5") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"6") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"7") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"8") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"9"))
            {
                uint16 value;

                /*SCI_VK_0 .... SCI_VK_9*/
                if (ConvertStringtoUint16(param_ptr->param_info.str_ptr->str_ptr,1,&value))
                {
                    key_code = value + '0';
                }
            }
            else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"*"))
            {
                /*SCI_VK_STAR*/
                key_code = '*';
            }
            else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"#"))
            {
                /*SCI_VK_POUND*/
                key_code = '#';
            }
            else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"@") )
            {
                /*SCI_VK_AT*/
                key_code = '@';
            }
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"call") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"CALL"))
        {
            /*SCI_VK_CALL*/
            key_code = 0x01;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"power") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"POWER") )
        {
            /*SCI_VK_POWER*/
            key_code = 0x02;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"cancel") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"CANCEL") )
        {
            /*SCI_VK_CANCEL*/
            key_code = 0x03;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"up") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"UP") )
        {
            /*SCI_VK_UP*/
            key_code = 0x04;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"down") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"DOWN") )
        {
            /*SCI_VK_DOWN*/
            key_code = 0x05;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"left") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"LEFT") )
        {
            /*SCI_VK_LEFT*/
            key_code = 0x06;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"right") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"RIGHT") )
        {
            /*SCI_VK_RIGHT*/
            key_code = 0x07;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"mselect") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"MSELECT") )
        {
            /*SCI_VK_MENU_SELECT*/
            key_code = 0x08;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"mcancel") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"MCANCEL") )
        {
            /*SCI_VK_MENU_CANCEL*/
            key_code = 0x09;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"sup") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"SUP") )
        {
            /*SCI_VK_SIDE_UP*/
            key_code = 0x0A;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"sdown") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"SDOWN") )
        {
            /*SCI_VK_SIDE_DOWN*/
            key_code = 0x0B;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"ok") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"OK") )
        {
            /*SCI_VK_WEB*/
            /*confirm key for mocor*/
            key_code = 0x0D;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"web") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"WEB") )
        {
            /*SCI_VK_WEB*/
            // key_code = 0x0D;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"camera") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"CAMERA") )
        {
            /*SCI_VK_CAMERA*/
            key_code = SCI_VK_MENU_CANCEL;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"flip") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"FLIP") )
        {
            /*SCI_VK_FLIP*/
            key_code = SCI_VK_FLIP;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"headsetbuttom") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"HEADSETBUTTOM") )
        {
            /*SCI_VK_HEADSET_BUTTOM*/
            key_code = SCI_VK_HEADSET_BUTTOM;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"headsetdetect") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"HEADSETDETECT") )
        {
            /*SCI_VK_HEADSET_DETECT*/
            key_code = SCI_VK_HEADSET_DETECT;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"sdcarddetect") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"SDCARDDETECT") )
        {
            /*SCI_VK_SDCARD_DETECT*/
            key_code = SCI_VK_SDCARD_DETECT;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"pause") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"PAUSE") )
        {
            /*SCI_VK_PAUSE*/
            key_code = SCI_VK_PAUSE;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"rewind") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"REWIND") )
        {
            /*SCI_VK_REWIND*/
            key_code = SCI_VK_REWIND;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"fastforword") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"FASTFORWORD") )
        {
            /*SCI_VK_FASTFORWARD*/
            key_code = SCI_VK_FASTFORWARD;
        }
        else if (0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"tv") ||
                 0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr,"TV") )
        {
            /*SCI_VK_TV for CMMB*/
            key_code = SCI_VK_TV;
        }

    }

    return key_code;
}
#endif

/*****************************************************************************/
//  Description : This function handles the AT+SPDUMMYKPD command
//                dummy keypad operation
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDUMMYKPD)
{
    ATC_STATUS     status = S_ATC_SUCCESS;
#if defined(AUTO_TEST_SUPPORT)||defined(_ATC_UIX8910_ENABLE_)
    xSignalHeaderRec        *key_sig_ptr    = PNULL;
    uint8               softkey_cmd;
    int                  key_code = 0;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if((!PARAM1_FLAG) || (!PARAM2_FLAG))
            {
                status = ERR_INVALID_INDEX;
            }
            else
            {
                softkey_cmd = (uint8)PARA1.num;
                key_code = (int)GetVKeyCode(cur_cmd_info->cur_param_list_ptr->next);//PARA2.num;

                if(0xFFFF == key_code)
                {
                    return  ERR_INVALID_INDEX;
                }

                SCI_MEMSET((void *)s_dummy_kpd_rsp[DUMMYMMI_RSP_MESSAGE], 0x0, MAX_DUMMY_KPD_RSP_LEN);
                SCI_MEMSET((void *)s_dummy_kpd_rsp[DUMMYMMI_RSP_WINSTRING], 0x0, MAX_DUMMY_KPD_RSP_LEN);

                SCI_TRACE_LOW("ATC: +SPDUMMYKPD, softkey_cmd:%d, key_code:%d",softkey_cmd, key_code);

                switch(softkey_cmd)
                {
                    case DIAG_KEY_MSG_DOWN_INFO_F:
                        SCI_CREATE_SIGNAL(key_sig_ptr, KPD_DOWN, sizeof(KPDSVR_SIG_T), SCI_IdentifyThread());
                        SCI_ASSERT(PNULL != key_sig_ptr);/*assert verified: check null pointer*/

                        ((KPDSVR_SIG_T *)key_sig_ptr)->key = key_code;
                        SCI_SEND_SIGNAL(key_sig_ptr, KPDSVR);
                        SCI_TRACE_LOW("ATC: SIMULATOR KPD: generate down key, key code is 0x%x", key_code);
                        break;
                    case DIAG_KEY_MSG_UP_INFO_F:
                        SCI_CREATE_SIGNAL(key_sig_ptr, KPD_UP, sizeof(KPDSVR_SIG_T), SCI_IdentifyThread());
                        SCI_ASSERT(PNULL != key_sig_ptr);/*assert verified: check null pointer*/

                        ((KPDSVR_SIG_T *)key_sig_ptr)->key = key_code;
                        SCI_SEND_SIGNAL(key_sig_ptr, KPDSVR);
                       SCI_TRACE_LOW("ATC: SIMULATOR KPD: generate up key, key code is 0x%x", key_code);
                        break;
                    case DIAG_NORMAL_KEY_MSG_INFO_F:
                        SCI_CREATE_SIGNAL(key_sig_ptr, KPD_DOWN, sizeof(KPDSVR_SIG_T), SCI_IdentifyThread());
                        SCI_ASSERT(PNULL != key_sig_ptr);/*assert verified: check null pointer*/

                        ((KPDSVR_SIG_T *)key_sig_ptr)->key = key_code;
                        SCI_SEND_SIGNAL(key_sig_ptr, KPDSVR);

                        SCI_CREATE_SIGNAL(key_sig_ptr, KPD_UP, sizeof(KPDSVR_SIG_T), SCI_IdentifyThread());
                        SCI_ASSERT(PNULL != key_sig_ptr);/*assert verified: check null pointer*/

                        ((KPDSVR_SIG_T *)key_sig_ptr)->key = key_code;
                        SCI_SEND_SIGNAL(key_sig_ptr, KPDSVR);
                        SCI_TRACE_LOW("ATC: SIMULATOR KPD: generate down and up key, key code is 0x%x", key_code);
                        break;
                    default:
                        status = ERR_OPERATION_NOT_SUPPORTED;
                        break;
                }
            }

            break;

        case ATC_CMD_TYPE_TEST:
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }
#endif
    return status;
}
/*****************************************************************************/
//  Description : This function handles the AT+SPDUMMYKPDRSP command
//                get dummy keypad operation response
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDUMMYKPDRSP)
{
#ifdef AUTO_TEST_SUPPORT
    uint8       result_type;

    if (ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE)
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessSPDUMMYKPDRSP Command Type not match");
        return S_ATC_FAIL;
    }

    if(PARAM1_FLAG)
    {
       result_type = (uint8)PARA1.num;
    }
    else
    {
       result_type = 0;
    }

    if (result_type >= DUMMYMMI_RSP_MAX)
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessSPDUMMYKPDRSP result_type %d exceeds %d(DUMMYMMI_RSP_MAX)",
                    result_type, DUMMYMMI_RSP_MAX);
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    if(0 != strlen((char *)s_dummy_kpd_rsp[result_type]))
    {
		sprintf((char *)g_rsp_str, "%s", s_dummy_kpd_rsp[result_type]);
    }
    else
    {
        sprintf((char *)g_rsp_str, "%s", "EMPTY");
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#endif
    return S_ATC_DEFAULT_HANDLE;
}
/*****************************************************************************/
//  Description : This function handles the AT+SPAUTOPOWERON command.
//                This command is used to set or get auto poweron flag.
//  Global resource dependence : g_rsp_str
//  Author:       Joe.Ni
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPAUTOPOWER )
{
    ATC_STATUS     status = S_ATC_SUCCESS;
#ifdef AUTO_TEST_SUPPORT
    uint8     auto_poweron_flag;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        ATC_REFPARAM_SetAutoPowerOnFlag(PARA1.num);
        break;
    case ATC_CMD_TYPE_READ:
        auto_poweron_flag = ATC_REFPARAM_GetAutoPowerOnFlag();
        sprintf((char *)g_rsp_str,"%s: %d",g_atc_info_table[AT_CMD_SPAUTOPOWER].cmd_name->str_ptr,auto_poweron_flag);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;
    default:
        status = ERR_OPERATION_NOT_SUPPORTED;
        break;
    }

#endif
    return status;
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
//   Description : This function handle the AT+SPAUDVOLTYPE command
//  Global resource dependence : g_rsp_str
//  Author:  chong.liu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPAUDVOLTYPE)
{
    ATC_STATUS        status = S_ATC_SUCCESS;
    uint32            audio_type = 0;
    uint32            audio_volume = 0;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if((PARAM1_FLAG && (PARAM1_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
                &&
               (PARAM2_FLAG && (PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC)))
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
}

/*****************************************************************************/
//  Description : This function handles the AT+SPDIAG command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDIAG)
{
//#ifndef _ULTRA_LOW_CODE_ //reduce atc code   Nemge
    uint32   status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if (!PARAM1_FLAG || (PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING))
        {
            SCI_TRACE_LOW("ATC: ATC_ProcessSPDIAG, no param");
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
        }

        if (PARA1.str_ptr->str_len == 0)
        {
            SCI_TRACE_LOW("ATC: ATC_ProcessSPDIAG, empty sting");
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
        }

        break;

    default:
        status = ERR_OPERATION_NOT_ALLOWED;
        break;
    }

    SCI_TRACE_LOW("ATC: ATC_ProcessSPDIAG, PARA1.str_ptr->str_ptr=%s,PARA1.str_ptr->str_len=%d", PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len);

    if (status != S_ATC_SUCCESS)
    {
        sprintf((char *)g_rsp_str, "+SPDIAG:%d", status);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return status;
    }
    else
    {
        uint8 *new_at_cmd_ptr = PNULL;
        uint32 cmd_len = 0;

        cmd_len = PARA1.str_ptr->str_len + 2; //<CR><cmd><CR>
        new_at_cmd_ptr = (uint8*)SCI_ALLOC_BASEZ(cmd_len + 1);

        if (new_at_cmd_ptr == PNULL)
        {
            SCI_TRACE_LOW("ATC: ATC_ProcessSPDIAG, memory full");
            status = ERR_MEMORY_FULL;
            sprintf((char *)g_rsp_str, "+SPDIAG:%d", status);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            new_at_cmd_ptr[0] = 0x0d;  //<CR>
            SCI_MEMCPY(&new_at_cmd_ptr[1], PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len);
            new_at_cmd_ptr[cmd_len - 1] = 0x0d;  //<CR>
            new_at_cmd_ptr[cmd_len] = '\0';
#ifdef _MUX_ENABLE_
            ATC_SendNewATInd(atc_config_ptr->current_link_id, cmd_len, new_at_cmd_ptr);
            SCI_FREE(new_at_cmd_ptr);
#else
            if (g_diag_cmd_buffer_ptr)
            {
                SCI_FREE(g_diag_cmd_buffer_ptr);
                g_diag_cmd_buffer_ptr = PNULL;
            }
            g_diag_cmd_buffer_ptr = new_at_cmd_ptr;
            g_is_diag_cmd = TRUE;

	#ifdef PPP_USB
            SendNewATInd(cmd_len,ATC_GetSioPort());
	#else
            SendNewATInd(cmd_len);
	#endif
#endif
        }

    }

    return S_ATC_DEFAULT_HANDLE;
//#else
//    return S_ATC_FAIL;
//#endif /* _ULTRA_LOW_CODE_  */
}

#ifndef _MUX_ENABLE_
/*****************************************************************************/
//  Description : This function is used to read command line from diag command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
void ATC_ReadDiagCmdLine (
    uint8 *data_ptr,       // Output argument, point to the buf which is alloced
    uint length,           // The length of cmd line which should be read from
    uint *read_length_ptr  // The actual read length
)
{
    if (g_diag_cmd_buffer_ptr)
    {
        *read_length_ptr = length;
        SCI_MEMCPY(data_ptr, g_diag_cmd_buffer_ptr, length);

        SCI_FREE(g_diag_cmd_buffer_ptr);
        g_diag_cmd_buffer_ptr = PNULL;
        g_is_diag_cmd = FALSE;
    }
    else
    {
        *read_length_ptr = 0;
        g_is_diag_cmd = FALSE;
    }
}
#endif

#ifndef _ULTRA_LOW_CODE_ //reduce atc code  Nemge
/*****************************************************************************/
//  Description : This function is used to get the parameters of diag command
//  Global resource dependence : none
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
BOOLEAN ATC_GetDiagParams(
    uint8            *in_ptr,
    uint8             in_len,
    ATC_DIAG_PARAM_T *param_ptr
)
{
    uint8            i;
    uint8            j = 0;
    BOOLEAN          flag = TRUE;

    if(NULL == in_ptr || NULL == param_ptr || in_len < 2)
    {
        return FALSE;
    }

    SCI_MEMSET((void*)param_ptr->mode, 0, ATC_MAX_DIAG_MODE_LEN);
    SCI_MEMSET((void*)param_ptr->act_value, 0, ATC_MAX_DIAG_ACT_LEN);
    param_ptr->mode_len = 0;
    param_ptr->cmd_type = ATC_CMD_TYPE_SET;


    for(i = 0; i < in_len; i++)
    {
        if(in_ptr[i] == ',')
        {
            i++;
            if ((in_ptr[i] != '"') && (i < in_len))
            {
                for (j = 0; ((i + j) < in_len) && (j < ATC_MAX_DIAG_ACT_LEN); j++ )
                {
                    param_ptr->act_value[j] = in_ptr[i + j];
                }
                param_ptr->cmd_type = ATC_CMD_TYPE_SET;
            }
            else
            {
                flag = FALSE;
            }

            break;

        }
        else if (in_ptr[i] == '?')
        {
            param_ptr->cmd_type = ATC_CMD_TYPE_READ;
            break;
        }
        else
        {
            if(j < ATC_MAX_DIAG_MODE_LEN)
            {
                if(in_ptr[i] != '"')
                {
                    param_ptr->mode[j++] = in_ptr[i];
                    param_ptr->mode_len += 1;
                }
            }
            else if(in_ptr[i] != '"')
            {
                flag = FALSE;
                break;
            }
        }
    }

    SCI_TRACE_LOW("ATC: get the diag parameters %d", flag);
    return flag;
}
#endif

#ifdef BLUETOOTH_SUPPORT_SPRD_BT
#ifdef BT_NONSIG_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : g_rsp_str
//  Author:       wuding.yang
//  Note:
/*****************************************************************************/
LOCAL void BT_SPBTTEST_GetRXDataCallback(const BT_NONSIG_DATA* data)
{
#ifndef WIN32
    rx_data_show_rssi = data->rssi;
    rx_data_show_status = data->status;
    rx_data_show_pkt_cnt = data->pkt_cnt;
    rx_data_show_pkt_err_cnt = data->pkt_err_cnt;
    rx_data_show_bit_cnt = data->bit_cnt;
    rx_data_show_bit_err_cnt =data->bit_err_cnt;

    spbttest_bt_rx_data_flag = TRUE;
#endif
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : g_rsp_str
//  Author:       wuding.yang
//  Note:
/*****************************************************************************/
BOOLEAN ATC_SPBTTEST_GetParams(
                uint8            *in_ptr,
                uint8             in_len,
                ATC_SPBTTEST_PARAM_T *param_ptr
                )
{
    int32              i    = 0;
    int32              j    = 0;
    uint16            k    = 0;
    BOOLEAN    flag    = TRUE;

    for (i=0; i<in_len; i++)
    {
        if (in_ptr[i] == ',')
        {
            k++;
            j = 0;

            if (8 < k)
            {
                flag = FALSE;
                break;
            }
        }
        else
        {
            if (j < ATC_SPBBTTEST_MAX_LEN)
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
    //SCI_TRACE_LOW("ATC: ATC_SPBTTEST_GetParams flag=%d", flag);
    return flag;

}
#endif

/*****************************************************************************/
//  Description : This function handles the AT+SPBTTEST command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPBTTEST)
{
    ATC_STATUS           status = S_ATC_SUCCESS;//S_ATC_DEFAULT_HANDLE;
#ifndef WIN32
#ifdef BLUETOOTH_SUPPORT_SPRD_BT

#if defined(BLE_INCLUDED) || defined(BT_BLE_SUPPORT)

    SCI_TRACE_LOW("ATC: ble not support AT+SPBTTEST command, return fail");
    return S_ATC_FAIL;

#else
    ATC_SPBTTEST_PARAM_T      param[4];
    uint8                 *temp_ptr;
    uint8                 param_len;
    uint8                 cmd_type;
    uint32                type_oper = 0;
    const char    *s_bttest_str[15] =
    {
        "TESTMODE","TESTADDRESS","TXCH","TXPATTERN","TXPKTTYPE","TXPKTLEN","TXPWR","TX","RXCH","RXPATTERN","RXPKTTYPE","RXGAIN","RX","RXDATA" ,"BTPTS",
    };
    int                   bttest_index;
    BOOLEAN         quiry_flag = FALSE;
#ifdef BT_NONSIG_SUPPORT
    BT_NONSIG_PARAM spbttest_bt_non_param = {0};
#endif
    static uint8       spbttest_bt_address[6] = {0};
    static uint8       spbttest_bt_tx_ch = 0;
    static uint8       spbttest_bt_tx_pattern = 1;
    static uint8       spbttest_bt_tx_pkttype = 1;
    static uint16     spbttest_bt_tx_pktlen = 0;
    static uint8       spbttest_bt_tx_pwr_type = 0;// 0---gain value; 1---power level
    static uint8       spbttest_bt_tx_pwr_value = 0;
    static uint8       spbttest_bt_tx_mode = 0;
    static uint8       spbttest_bt_tx_status = 0;
    static uint8       spbttest_bt_rx_ch = 0;
    static uint8       spbttest_bt_rx_pattern = 0;
    static uint8       spbttest_bt_rx_pktype = 1;
    static uint8       spbttest_bt_rx_gain_mode = 0;
    static uint8       spbttest_bt_rx_gain_value = 0;
    static uint8       spbttest_bt_rx_status = 0;
    static uint8       spbttest_bt_create_flag = 0;
    BOOLEAN         spbttest_bt_status = FALSE;

    SCI_MEMSET(param, 0, (3 * sizeof(ATC_SPBTTEST_PARAM_T)));
    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    temp_ptr  = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    ATC_GetCmdType(temp_ptr, (uint16)param_len, &cmd_type);

    switch (cmd_type)
    {
    case ATC_CMD_TYPE_SET:
        temp_ptr++;
        param_len--;
        if (ATC_SPBTTEST_GetParams(temp_ptr, param_len, param))
        {
            if ('?' == param[0].param[param[0].param_len-1])
            {
                quiry_flag = TRUE;
                param[0].param[param[0].param_len-1] = '\0';
            }
            else
            {
                param[0].param[param[0].param_len] = '\0';
            }

            for (bttest_index=0; bttest_index<14; bttest_index++)
            {
                if (!strcmp((char *)param[0].param, s_bttest_str[bttest_index]))
                {
                    break;
                }
            }

            SCI_TRACE_LOW("SPBTTEST: param_len is %d, 0 param_len is %d, index is %d",param_len,param[0].param_len,bttest_index);

            switch (bttest_index)
            {
            case 0:   //"TESTMODE"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get testmode");
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=", BT_GetTestMode()); 	//lint !e718  !e746
                }
                else if (param[1].param_len != 0)
                {

                     if (CALIBRATION_MODE == POWER_GetResetMode())
                     {
                         if (0 == spbttest_bt_create_flag)
                         {
                             BT_Init(NULL);//create the BT thread
                             spbttest_bt_create_flag = 1;
                             SCI_SLEEP(400);
                         }
                     }

                    type_oper = param[1].param[0] - '0';

                    if (0 == type_oper) // quit EUT mode
                    {
                        SCI_TRACE_LOW("SPBTTEST: quit EUT");
                        if (BT_TESTMODE_SIG == BT_GetTestMode())
                        {
                            BT_LeaveEutMode();
                            MNPHONE_SetDisablePowerSweep(FALSE);
                        }
                        else
                        {
                            BT_Stop();
                            SCI_SLEEP(300);//Wait BT Task running to stop the BT
                        }
#ifdef BT_NONSIG_SUPPORT
                        SCI_MEMSET(&spbttest_bt_non_param,0,sizeof(BT_NONSIG_PARAM));
#endif
                        SCI_MEMSET(spbttest_bt_address,0,6);
                        spbttest_bt_tx_ch = 0;
                        spbttest_bt_tx_pattern = 1;
                        spbttest_bt_tx_pkttype = 1;
                        spbttest_bt_tx_pktlen = 0;
                        spbttest_bt_tx_pwr_type = 0;
                        spbttest_bt_tx_pwr_value = 0;
                        spbttest_bt_tx_mode = 0;
                        spbttest_bt_tx_status = 0;
                        spbttest_bt_rx_ch = 0;
                        spbttest_bt_rx_pattern = 0;
                        spbttest_bt_rx_pktype = 1;
                        spbttest_bt_rx_gain_mode = 0;
                        spbttest_bt_rx_gain_value = 0;
                        spbttest_bt_rx_status = 0;
                        BT_SetTestMode(BT_TESTMODE_NONE); //lint !e718  !e746
                    }
                    else if (1 == type_oper) // enter EUT mode
                    {
                        SCI_TRACE_LOW("SPBTTEST: enter EUT");
                        //BTI_Init(NULL);
                        MNPHONE_SetDisablePowerSweep(TRUE);
                        BT_EnterEutMode();
                    }
                    else if (2 == type_oper) // enter nonSignal mode
                    {
                        SCI_TRACE_LOW("SPBTTEST: start Non Sig Test");
                        BT_SetTestMode(BT_TESTMODE_NONSIG);

                        BT_Start();
                        SCI_SLEEP(700);//wait BT init
                    }
                    sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                }

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 1:   //"TESTADDRESS"
            {
                if (quiry_flag)
                {
/*//wuding: the code coming from 7701, it looks strange
                    uint8 addr[12] = {0};
                    uint8 i = 0;
                    SCI_TRACE_LOW("SPBTTEST: get TESTADDRESS");
                    //here call the low layer API to get TESTADDRESS
                    //BT_TEST_Get_BdAddr(addr);
                    for(i=0; i<6;i++)
                        SCI_TRACE_LOW("get bt addr:%x", addr[i]);
                    sprintf((char *)g_rsp_str, "%s%s%s%s","+SPBTTEST:",s_bttest_str[bttest_index],"=", "11:22:33:44:55:66");
*/
                    sprintf((char *)g_rsp_str, "%s%s%s%x%x%x%x%x%x","+SPBTTEST:",s_bttest_str[bttest_index],"=",spbttest_bt_address[0],spbttest_bt_address[1],spbttest_bt_address[2],spbttest_bt_address[3],spbttest_bt_address[4],spbttest_bt_address[5]);
                }
                else if (param[1].param_len != 0)
                {
                    uint8 tmp_addr[12] = {0};
                    //uint8 addr_tmp[12] = {0};
                    int i = 0;
                    int j = 0;
                    //here call the low layer API to set TESTADDRESS
                    SCI_TRACE_LOW("testing bt addr: %s, len=%d", (param[1].param), param[1].param_len);
                    for(i=0; i<param[1].param_len; i++)
                    {
                        if(param[1].param[i] != ':')
                        {
                            if(param[1].param[i]>= 'A' && param[1].param[i]<='F')
                            {
                                tmp_addr[j] = (param[1].param[i] - '0' - 7);
                            }
                            else
                            {
                                tmp_addr[j] = (param[1].param[i] - '0');
                            }
                            //sprintf(tmp_addr+j, "%x", (param[1].param[i] - '0'));
                            SCI_TRACE_LOW("tmp_addr=%x", tmp_addr[j]);
                            ++j;
                        }
                    }

                    for(j=0; j<6; j++)
                    {
                        //sprintf(tmp_addr_2[j], "%x", (tmp_addr[j*2]<<4 | tmp_addr[j*2+1]));
                        spbttest_bt_address[j] = (tmp_addr[j*2])<<4 | (tmp_addr[j*2+1]);
                        SCI_TRACE_LOW("set bt addr: %x", spbttest_bt_address[j]);
                    }

                    //BT_TEST_Set_BdAddr(tmp_addr_2);
                    // param[1].param_len & param[1].param are the TESTADDRESS which user inputed, such as 11:22:33:44:55:66
                    sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 2:   //"TXCH"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get TXCH");
                    //here call the low layer API to get TXCH
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=",spbttest_bt_tx_ch);
                }
                else if (param[1].param_len != 0)
                {
                    if (param[1].param_len == 1)  // valid type_oper range [0~93]
                    {
                        type_oper = param[1].param[0] - '0';
                    }
                    else if (param[1].param_len == 2)
                    {
                        type_oper = param[1].param[0] - '0';
                        type_oper = type_oper * 10 + (param[1].param[1] - '0');
                    }
                    spbttest_bt_tx_ch = (uint8)type_oper;
                    //here call the low layer API to set TXCH, type_oper is the input x
                    sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 3:   //"TXPATTERN"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get TXPATTERN");
                    //here call the low layer API to get TXPATTERN
                   sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=",spbttest_bt_tx_pattern);
                }
                else if (param[1].param_len != 0)
                {
                    type_oper = param[1].param[0] - '0';

                    //here call the low layer API to set TXPATTERN, type_oper is the input x, range [0~3]
                    //wuding: according to the NPI BT AT cmd doc
                    //00000000---1
                    //11111111---2
                    //10101010---3
                    //PRBS9     ---4
                    //11110000---9
                    spbttest_bt_tx_pattern = (uint8)type_oper;
                    sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 4:   //"TXPKTTYPE"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get TXPKTTYPE");
                    //here call the low layer API to get TXPKTTYPE
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=", spbttest_bt_tx_pkttype);
                }
                else if (param[1].param_len != 0)
                {
                    //type_oper = param[1].param[0] - '0';
                    uint8 i = 0;
                    for (type_oper=0,i=0;i<param[1].param_len;i++)
                    {
                        type_oper = type_oper * 10 + (param[1].param[i] - '0');
                    }

                    //wuding: according to the NPI BT AT cmd doc
                    //0x00---NULL,             0x01---POLL,          0x02---FHS,            0x03---DM1,            0x04---DH1,
                    //0x05---HV1,              0x06---HV2,           0x07---HV3,            0x08---DV,               0x09---AUX1,
                    //0x0A---DM3,             0x0B---DH3,           0x0C---EV4,            0x0D---EV5,             0x0E---DM5,
                    //0x0F---DH5,              0x10---ID,             0x11---INVALID,      0x12 ---,                  0x13 ---,
                    //0x14---EDR_2DH1,   0x15---EDR_EV3,    0x16---EDR_2EV3,   0x17---EDR_3EV3,    0x18---EDR_3DH1,
                    //0x19---EDR_AUX1,   0x1A---EDR_2DH3,  0x1B---EDR_3DH3,  0x1C---EDR_2EV5,   0x1D---EDR_3EV5,
                    //0x1E---EDR_2DH5,   0x1F---EDR_3DH5
                    spbttest_bt_tx_pkttype = (uint8)type_oper;

                    //here call the low layer API to set TXPKTTYPE, type_oper is the input x, range [0~5]
                    sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 5:   //"TXPKTLEN"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get TXPKTLEN");
                    //here call the low layer API to get TXPKTLEN
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=", spbttest_bt_tx_pktlen);
                }
                else if (param[1].param_len != 0)
                {
                    int i=0;
                    for (type_oper=0,i=0;i<param[1].param_len;i++)
                    {
                        type_oper = type_oper * 10 + (param[1].param[i] - '0');
                    }
                    //here call the low layer API to set TXPKTLEN, type_oper is the input x
                    spbttest_bt_tx_pktlen = (uint16)type_oper;
                    sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 6:   //"TXPWR"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get TXPWR X1,X2");
                    //here call the low layer API to get TXPWR
                    //wuding: the code coming from 7701, it looks strange
                    //sprintf((char *)g_rsp_str, "%s%s%s%d%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=", 1,",",58);
                    sprintf((char *)g_rsp_str, "%s%s%s%d,%d","+SPBTTEST:",s_bttest_str[bttest_index],"=",spbttest_bt_tx_pwr_type,spbttest_bt_tx_pwr_value);
                }
                else if ((param[1].param_len != 0) && (param[2].param_len != 0))
                {
                    int x1=0,x2=0,i;
                    x1 = param[1].param[0] - '0';

                    for (x2=0,i=0;i<param[2].param_len;i++)
                    {
                        x2 = x2 * 10 + (param[2].param[i] - '0');
                    }

                    //wuding: NPI AT cmd AT+SPBTTEST=TXPWR,x1,x2
                    //x1: 0---gain value;  1---power level
                    //x2: value
                    spbttest_bt_tx_pwr_type = x1;
                    spbttest_bt_tx_pwr_value = x2;

                    //here call the low layer API to set TXPWR, x1 & x2 are the input
                    sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 7:   //"TX"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get TX status");
                    //here call the low layer API to get TX status
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=", spbttest_bt_tx_status);
                }
                else if (param[1].param_len != 0)
                {
                    int pktcnt=0,i;
                    type_oper = param[1].param[0] - '0';

                    if (0 == type_oper)
                    {
                        spbttest_bt_tx_status = 0;
                        //here call the low layer API to stop TX
                        SCI_TRACE_LOW("SPBTTEST: stop TX");
#ifdef BT_NONSIG_SUPPORT
                        spbttest_bt_status = BT_SetNonSigTxTestMode(TRUE, FALSE, &spbttest_bt_non_param);
#endif
                    }
                    else if (1 == type_oper)
                    {
                        spbttest_bt_tx_status = 1;

                        if (param[2].param_len != 0)
                        {
                            spbttest_bt_tx_mode = param[2].param[0] - '0';
#ifdef BT_NONSIG_SUPPORT
                            spbttest_bt_non_param.channel = spbttest_bt_tx_ch;
                            spbttest_bt_non_param.pattern = spbttest_bt_tx_pattern;
                            spbttest_bt_non_param.pac.pac_type = spbttest_bt_tx_pkttype;
                            spbttest_bt_non_param.pac.pac_len = spbttest_bt_tx_pktlen;
                            spbttest_bt_non_param.item.power.power_type = spbttest_bt_tx_pwr_type;
                            spbttest_bt_non_param.item.power.power_value = spbttest_bt_tx_pwr_value;
#endif
                            if (0 == spbttest_bt_tx_mode)
                            {
                                //here call the low layer API to Sart TX for continues
                                SCI_TRACE_LOW("SPBTTEST: Sart TX for continues");
#ifdef BT_NONSIG_SUPPORT
                                spbttest_bt_non_param.pac.pac_cnt = 0;
#endif
                            }
                            else
                            {
                                if (param[3].param_len != 0)
                                {
                                    for (pktcnt=0,i=0;i<param[3].param_len;i++)
                                    {
                                        pktcnt = pktcnt * 10 + (param[3].param[i] - '0');
                                    }
                                }
				    else
				    {
				        pktcnt = 0;//continues
				    }

                                //here call the low layer API to Sart TX for single, pktcnt is input for pktcnt
                                SCI_TRACE_LOW("SPBTTEST: Sart TX for single");
#ifdef BT_NONSIG_SUPPORT
                                spbttest_bt_non_param.pac.pac_cnt = pktcnt;
#endif
                            }
#ifdef BT_NONSIG_SUPPORT
                            spbttest_bt_status = BT_SetNonSigTxTestMode(TRUE,TRUE, &spbttest_bt_non_param);
#endif
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }

                    SCI_Sleep(200);

                    if (TRUE == spbttest_bt_status)//lint !e774
                    {
                        sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "%s","+SPBTTEST:ERR");
                    }
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 8:   //"RXCH"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get RXCH");
                    //here call the low layer API to get RXCH
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=", spbttest_bt_rx_ch);
                }
                else if (param[1].param_len != 0)
                {
                    if (param[1].param_len == 1)  // valid type_oper range [0~93]
                    {
                        type_oper = param[1].param[0] - '0';
                    }
                    else if (param[1].param_len == 2)
                    {
                        type_oper = param[1].param[0] - '0';
                        type_oper = type_oper * 10 + (param[1].param[1] - '0');
                    }
                    spbttest_bt_rx_ch = (uint8)type_oper;
                    //here call the low layer API to set RXCH, type_oper is the input x
                    sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 9:   //"RXPATTERN"
            {
                //wuding: there is no RXPATTERN in BT No-Signal!
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get RXPATTERN");
                    //here call the low layer API to get RXPATTERN
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=", spbttest_bt_rx_pattern);
                }
                else if (param[1].param_len != 0)
                {
                    type_oper = param[1].param[0] - '0';
                    //wuding: NPI AT cmd
                    //0000---0
                    //1111---1
                    //1010---2
                    //random---3
                    //receiver  ---7
                    //11110000---9
                    spbttest_bt_rx_pattern = (uint8)type_oper;
                    //here call the low layer API to set RXPATTERN, type_oper is the input x, range [0~3]
                    sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 10:   //"RXPKTTYPE"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get RXPKTTYPE");
                    //here call the low layer API to get RXPKTTYPE
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=", spbttest_bt_rx_pktype);
                }
                else if (param[1].param_len != 0)
                {
                    uint8 i = 0;
                    for (type_oper=0,i=0;i<param[1].param_len;i++)
                    {
                        type_oper = type_oper * 10 + (param[1].param[i] - '0');
                    }

                    //wuding: the same as TX PKT type
                    spbttest_bt_rx_pktype = (uint8)type_oper;

                    //here call the low layer API to set RXPKTTYPE, type_oper is the input x, range [0~5]
                    sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 11:   //"RXGAIN"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get RXGAIN");
                    //here call the low layer API to get RXGAIN Mode and x
                    if (0 == spbttest_bt_rx_gain_mode)
                    {
                        sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=", 0);
                    }
		      else
                    {
                        sprintf((char *)g_rsp_str, "%s%s%s%d,%d","+SPBTTEST:",s_bttest_str[bttest_index],"=", 1, spbttest_bt_rx_gain_value);
                    }
                }
                else if (param[1].param_len != 0)
                {
                    int mode=0,x=0,i;
                    mode = param[1].param[0] - '0';

                    spbttest_bt_rx_gain_mode = mode;//0 Auto; 1 Fix

                    if (0 == mode)
                    {
                        //here call the low layer API to Set RX Gain Auto mode
                        SCI_TRACE_LOW("SPBTTEST: Set RX Gain Auto mode");
                    }
                    else if (1 == mode)
                    {
                        if (param[2].param_len != 0)
                        {
                            for (x=0,i=0;i<param[2].param_len;i++)
                            {
                                x = x * 10 + (param[2].param[i] - '0');
                            }
                            //here call the low layer API to Set RX Gain Fix mode, x is input for x
                            //AT+SPBTTEST=RXGAIN,mode,[x]
                            //x: gain parameter , only available when mode is 1
                            if ((0 == x) || (1 == x) ||(2 == x) || (3 == x) || (4 == x) || (5 == x))//double check before send it to controller
                            {
                                spbttest_bt_rx_gain_value = x;
                            }
                           else
                           {
                               spbttest_bt_rx_gain_value = 0;
                           }
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }

                    sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 12:   //"RX"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBTTEST: get RX");
                    //here call the low layer API to get RX
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTTEST:",s_bttest_str[bttest_index],"=", spbttest_bt_rx_status);
                }
                else if (param[1].param_len != 0)
                {
                    BOOLEAN enable = FALSE;
                    type_oper = param[1].param[0] - '0';

                    if(type_oper)
                    {
                        spbttest_bt_rx_status = 1;
                        enable = TRUE;
#ifdef BT_NONSIG_SUPPORT
                        spbttest_bt_non_param.pattern = spbttest_bt_rx_pattern;
                        spbttest_bt_non_param.channel = spbttest_bt_rx_ch;
                        if (1 == spbttest_bt_rx_gain_mode)//Fix Mode
                        {
                            spbttest_bt_non_param.item.rx_gain = spbttest_bt_rx_gain_value;
                        }
                        else//auto mode
                        {
                            spbttest_bt_non_param.item.rx_gain = 0;
                        }
                        spbttest_bt_non_param.pac.pac_type = spbttest_bt_rx_pktype;
#endif
                    }
                    else
                    {
                        spbttest_bt_rx_status = 0;
                        enable = FALSE;
                    }
#ifdef BT_NONSIG_SUPPORT
                    spbttest_bt_status = BT_SetNonSigRxTestMode(TRUE,enable,&spbttest_bt_non_param,(BT_ADDRESS*)spbttest_bt_address);
#endif
                    //here call the low layer API to set RX, type_oper is the input x, range [0~1]
                    if (spbttest_bt_status)//lint !e774
                    {
                        sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "%s","+SPBTTEST:ERR");
                    }
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);// return ok response for AT cmd first
                //for rx test end and start next tx test
                //BT_RX_STATUS((uint8)type_oper);
                break;
            }
            case 13:   //"RXDATA"
            {
                if (quiry_flag)
                {
                    uint32 res[5] = {0};
                    uint8 loop_times = 0;

                    SCI_TRACE_LOW("SPBTTEST: get RXDATA");

#ifdef BT_NONSIG_SUPPORT
                    //here call the low layer API to get RXDATA, such as error_bits,total_bits,error_packets,total_packets,rssi
                    spbttest_bt_rx_data_flag = FALSE;
                    spbttest_bt_status = BT_GetNonSigRxData(TRUE, BT_SPBTTEST_GetRXDataCallback);

                    if (spbttest_bt_status)
                    {
                        while(!spbttest_bt_rx_data_flag)
                        {
                            loop_times++;
                            if (loop_times < 12)//wait 600ms
                            {
                                SCI_SLEEP(50);
                            }
                            else
                            {
                                break;//quit the loop
                            }
                        }

                        if (spbttest_bt_rx_data_flag && (BT_SUCCESS == rx_data_show_status))
                        {
                            res[0] = rx_data_show_bit_err_cnt;
                            res[1] = rx_data_show_bit_cnt;
                            res[2] = rx_data_show_pkt_err_cnt;
                            res[3] = rx_data_show_pkt_cnt;
                            res[4] = rx_data_show_rssi;
    			   SCI_TRACE_LOW("rxdata, %d, %d, %d, %d", res[0],res[1],res[2],res[3],res[4]);
                        }
                        else
                        {
                            SCI_TRACE_LOW("rxdata, null. %d %d",spbttest_bt_rx_data_flag,rx_data_show_status);
                        }

                        sprintf((char *)g_rsp_str, "%s%s%s%d,%d,%d,%d,0x%x","+SPBTTEST:",s_bttest_str[bttest_index],"=",
                        res[0] /*error_bits  990*/, res[1] /*total_bits 1600000*/, res[2] /*error_packets 100*/, res[3] /*total_packets 1024*/,res[4]/*rssi*/);
                    }
                    else
#endif
                    {
                        sprintf((char *)g_rsp_str, "%s","+SPBTTEST:ERR");
                    }
                }   //lint !e529
                else
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }

	     case 14:
	     {
		      uint8 oper = param[1].param[0] - '0';
			switch(oper){
				case 0:
					pbap_pullphonebook_req();
					break;
				case 1:
					pbap_pullvcardentry_req();
					break;
				case 2:
					pts_mode_enter();
					break;
				case 3:
					pts_mode_exit();
					break;
				default:
					break;
			}
			sprintf((char *)g_rsp_str, "%s","+SPBTTEST:OK");
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			break;
	     }
            default:   //Error input para
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }
            }
        }
        else
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
        break;

    case ATC_CMD_TYPE_READ:
    case ATC_CMD_TYPE_TEST:
    default:
        return ERR_OPERATION_NOT_ALLOWED;
    }
#endif
#endif
#endif
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
#if !defined( _ULTRA_LOW_CODE_ ) ||defined(_ATC_UIX8910_ENABLE_)//reduce atc code   Nemge
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
#else
    return S_ATC_FAIL;
#endif /*#ifndef _ULTRA_LOW_CODE_ //reduce atc code */
}

/*****************************************************************************/
//  Description : This function handle the AT+GMM command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessGMM )
{
#ifndef _ULTRA_LOW_CODE_ //reduce atc code  Nemge
    char*   module_str = PNULL;

    if(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE)
    {
        module_str = (char*)CLASSMARK_GetModelName();
        SCI_ASSERT(PNULL != module_str);
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
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+FCLASS command
//  Global resource dependence : g_rsp_str
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessFCLASS)
{
#ifndef _ULTRA_LOW_CODE_ //reduce atc code Nemge
    ATC_STATUS       status = S_ATC_SUCCESS;
#ifndef _GSM_ONLY_
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            break;

        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s:0,8", g_atc_info_table[AT_CMD_FCLASS].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            break;
    }
#endif
    return status;
#else
    return S_ATC_FAIL;
#endif
}


/*****************************************************************************/
//  Description : This function handles the AT_CLS command
//  Global resource dependence : g_rsp_str
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSHARP_CLS)
{
#ifndef _ULTRA_LOW_CODE_ //reduce atc code Nemge
    ATC_STATUS       status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            break;

        case ATC_CMD_TYPE_READ:
            status = S_ATC_DEFAULT_HANDLE;
            ATC_BuildResultErrCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
            break;

        case ATC_CMD_TYPE_TEST:
            status = S_ATC_DEFAULT_HANDLE;
            ATC_BuildResultErrCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
            break;

        default:
            break;
    }

    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+GCI command
//  Global resource dependence : g_rsp_str
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessGCI)
{
#ifndef _ULTRA_LOW_CODE_ //reduce atc code Nemge
    ATC_STATUS       status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            break;

        case ATC_CMD_TYPE_READ:
            status = S_ATC_DEFAULT_HANDLE;
            ATC_BuildResultErrCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
            break;

        case ATC_CMD_TYPE_TEST:
            status = S_ATC_DEFAULT_HANDLE;
            ATC_BuildResultErrCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
            break;

        default:
            break;
    }

    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+I1 command
//  Global resource dependence : g_rsp_str
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI1)
{
#ifndef _ULTRA_LOW_CODE_ //reduce atc code Nemge
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            break;

        case ATC_CMD_TYPE_READ:
            break;

        default:
            break;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+I2 command
//  Global resource dependence : g_rsp_str
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI2)
{
#ifndef _ULTRA_LOW_CODE_ //reduce atc code Nemge
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            break;

        case ATC_CMD_TYPE_READ:
            break;

        default:
            break;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+I3 command
//  Global resource dependence : g_rsp_str
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI3)
{
#ifndef _ULTRA_LOW_CODE_ //reduce atc code Nemge
    ATC_STATUS       status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            //status = S_ATC_DEFAULT_HANDLE;
            sprintf((char *)g_rsp_str, "Spreadtrum SoftModem Version 1.0");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_READ:
            break;

        default:
            break;
    }

    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+I4 command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI4)
{
#ifndef _ULTRA_LOW_CODE_ //reduce atc code Nemge
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            break;

        case ATC_CMD_TYPE_READ:
            break;

        default:
            break;
    }

    ATC_BuildResultErrCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+I52 command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI5)
{
#ifndef _ULTRA_LOW_CODE_ //reduce atc code Nemge
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            break;

        case ATC_CMD_TYPE_READ:
            break;

        default:
            break;
    }

    ATC_BuildResultErrCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+I6 command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI6)
{
#ifndef _ULTRA_LOW_CODE_ //reduce atc code Nemge
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            break;

        case ATC_CMD_TYPE_READ:
            break;

        default:
            break;
    }

    ATC_BuildResultErrCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+I7 command
//  Global resource dependence :
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI7)
{
#ifndef _ULTRA_LOW_CODE_ //reduce atc code Nemge
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            break;

        case ATC_CMD_TYPE_READ:
            break;

        default:
            break;
    }


    ATC_BuildResultErrCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handle the AT+CRC command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCRC )
{
#if !defined( _ULTRA_LOW_CODE_)||defined(_ATC_UIX8910_ENABLE_)
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 0, &atc_config_ptr->crc);
    return S_ATC_SUCCESS;
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
#if !defined( _ULTRA_LOW_CODE_)||defined(_ATC_UIX8910_ENABLE_)
    uint32   status = S_ATC_DEFAULT_HANDLE;
    MNSIM_ERROR_E           error_code   = MNSIM_ERROR_POWER_OFF;
    MN_CRSM_COMMAND_TYPE_E  atc_sim_cmd  = MN_CRSM_COMMAND_TYPE_NUM;
    uint16  file_id = 0;
    uint8   path_len = 0;
    uint8*  path_id = NULL;
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

                        #if defined(_ATC_UIX8910_ENABLE_)
                        if(PARAM7_FLAG)
                        {
				path_len = (uint8)(PARA7.str_ptr->str_len);
                            path_id = (uint8*)(PARA7.str_ptr->str_ptr);
				if(path_len != 4)
				{
					return S_ATC_FAIL;
				}
                        }
                        #endif

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
                        #if defined(_ATC_UIX8910_ENABLE_)
                        if(PARAM7_FLAG)
                        {
				path_len = (uint8)(PARA7.str_ptr->str_len);
                            path_id = (uint8*)(PARA7.str_ptr->str_ptr);
				if(path_len != 4)
				{
					return S_ATC_FAIL;
				}
                        }
                        #endif
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
                        #if defined(_ATC_UIX8910_ENABLE_)
                        if(PARAM7_FLAG)
                        {
				path_len = (uint8)(PARA7.str_ptr->str_len);
                            path_id = (uint8*)(PARA7.str_ptr->str_ptr);
				if(path_len != 4)
				{
					return S_ATC_FAIL;
				}
                        }
                        #endif
                    }
                    else
                    {
                        return ERR_INVALID_INDEX;
                    }
                    break;
                }

                case MN_CRSM_GET_STATUS:
                {
                    if (PARAM2_FLAG)
                    {
                        file_id = (uint16)(PARA2.num);
                        #if defined(_ATC_UIX8910_ENABLE_)
                        if(PARAM7_FLAG)
                        {
				path_len = (uint8)(PARA7.str_ptr->str_len);
                            path_id = (uint8*)(PARA7.str_ptr->str_ptr);
				if(path_len != 4)
				{
					return S_ATC_FAIL;
				}
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
                if (ERR_MNDATAMAG_NO_ERR != MNSIM_CrsmEx(dual_sys, atc_sim_cmd, P1, P2, P3, file_id, data, path_len, path_id))
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



typedef enum
{
	ATC_PM_DISABLE_SLEEP = 0,
	ATC_PM_ENABLE_PM0,
	ATC_PM_ENABLE_PM1,
	ATC_PM_ENABLE_PM2,
	ATC_PM_ENABLE_PM3,
	ATC_PM_SLEEP_TYPE_MAX
} ATC_PM_TYPE_E;

extern void SCI_ENGINEER_EnableDeepSleep (uint32 mode);
extern uint32 SCI_GetSleepMode_Flag (void);

/*****************************************************************************/
//  Description : This function handles the AT+CSCLK command.This command is used
//  to set or get deep sleep mode.
//  Global resource dependence : g_rsp_str
//  Author:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPMMODE)
{
#ifndef WIN32
    uint32   status = S_ATC_DEFAULT_HANDLE;
	uint32	pm_mode = 0;
	uint32	ret_pm_mode = 0;
	SCI_TRACE_LOW("ATC_ProcessDPMODE");
	switch (ATC_GET_CMD_TYPE)
	{
		case ATC_CMD_TYPE_SET:
		{
			SCI_TRACE_LOW("ATC:pm mode %d",PARA1.num);
			switch(PARA1.num)
			{
				case ATC_PM_DISABLE_SLEEP:
					SCI_ENGINEER_EnableDeepSleep(DISABLE_SLEEP);
					break;
				case ATC_PM_ENABLE_PM1:
					SCI_ENGINEER_EnableDeepSleep(ENABLE_PM1);
					break;
				case ATC_PM_ENABLE_PM2:
					SCI_ENGINEER_EnableDeepSleep(ENABLE_PM2);
					break;
				case ATC_PM_ENABLE_PM3:
					SCI_ENGINEER_EnableDeepSleep(ENABLE_PM3);
					break;
				case ATC_PM_ENABLE_PM0:
				default:
					SCI_ENGINEER_EnableDeepSleep(ENABLE_PM0);
					break;
			}
			status = S_ATC_SUCCESS;
			break;
		}

		case ATC_CMD_TYPE_READ:
		{
			pm_mode = SCI_GetSleepMode_Flag();
			SCI_TRACE_LOW("ATC:pm mode %d get",pm_mode);
			switch(pm_mode)
			{
				case DISABLE_SLEEP:
					ret_pm_mode = ATC_PM_DISABLE_SLEEP;
					break;
				case ENABLE_PM1:
					ret_pm_mode = ATC_PM_ENABLE_PM1;
					break;
				case ENABLE_PM2:
					ret_pm_mode = ATC_PM_ENABLE_PM2;
					break;
				case ENABLE_PM3:
					ret_pm_mode = ATC_PM_ENABLE_PM3;
					break;
				case ENABLE_PM0:
					ret_pm_mode = ATC_PM_ENABLE_PM0;
					break;
				default:
					ret_pm_mode = 0xFF;
					break;
			}

			sprintf((char*)g_rsp_str, "%s=%d",\
							g_atc_info_table[AT_CMD_PMMODE].cmd_name->str_ptr,
							ret_pm_mode);
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			status = S_ATC_SUCCESS;
			break;
		}

		case ATC_CMD_TYPE_TEST:
		{

			sprintf((char*)g_rsp_str, "%s: (0-4)",\
						g_atc_info_table[AT_CMD_PMMODE].cmd_name->str_ptr);
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			status = S_ATC_SUCCESS;
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



#if 0
/*****************************************************************************/
//  Description : This function handle the AT&D command
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processand_D)
{
#ifndef _ULTRA_LOW_CODE_
    SCI_ASSERT(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE); /*assert verified check command type*/
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
    SCI_ASSERT(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE); /*assert verified check command type*/

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

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
    HandleSingleCmd(cur_cmd_info, atc_config_ptr, 2, &atc_config_ptr->s10_reg);
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
//    Author:
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
                // output the '>'
                ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SNVM);
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
                    if (result)
                    {
                        //SCI_TRACE_LOW("ATC: gpio %d set to output directiong, get value not permit.", gpio_id);
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
            register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

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
/*****************************************************************************/
// Description : This function handle the AT+GSMSTATUS command
//                 Automatic downloader
// Global resource dependence : none
// Author:winnie.zhu
// Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessGSMSTATUS)
{
#ifdef _NDS_EXT_CMD_
    const char *state_str[]={"OK","INITING","SIMERR","NETERR","OTHERERR"};
    uint8 state = 4;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_PHONE_CURRENT_PLMN_INFO_T       register_status = {0};

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            if(POWER_MS_STATUS_INIT == POWER_GetMsStatus())
            {
                state = 1;
            }
            else if (!DMSIM_SimIsReady(dual_sys))
            {
                state = 2;
            }

            else
            {
                register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
                switch(register_status.plmn_status)
                {
                    case PLMN_NORMAL_GSM_ONLY:
                    case PLMN_NORMAL_GPRS_ONLY:
                    case PLMN_NORMAL_GSM_GPRS_BOTH:
                    {
                        state = 0;
                        break;
                    }

                    case PLMN_EMERGENCY_ONLY:
                    case PLMN_EMERGENCY_SIM_INVALID:
                    case PLMN_EMERGENCY_GPRS_ONLY:
                    case PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY:
                    case PLMN_NO_SERVICE:
                    {
                        state = 3;
                        break;
                    }

                    default :
                    {
                        state = 4;
                        break;
                    }
                }
            }

            sprintf((char*)g_rsp_str, "%s",state_str[state]);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
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
// Description : This function handle the AT+FIRBOOT command
//                 Automatic downloader
//  Global resource dependence : none
//  Author:winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessFIRBOOT )
{
#ifdef _NDS_EXT_CMD_
    uint8 seed_str[21];
    uint8 seed_str1[21];
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // check the index
            if (PARAM1_FLAG && (20 ==PARA1.str_ptr->str_len))
            {
                SCI_MEMCPY(seed_str,PARA1.str_ptr->str_ptr,PARA1.str_ptr->str_len);
                seed_str[PARA1.str_ptr->str_len] = '\0';

                //save in the NV.
                if (NVERR_NONE != EFS_NvitemWrite(NV_FIRBOOT_CONFIG_ID,
                                                sizeof(FIRBOOT_SEED_NV_T),
                                                PARA1.str_ptr->str_ptr,
                                                FALSE))
                {
                    //SCI_TRACE_LOW:"FIRBOOT:EFS_NvitemWrite error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10585_112_2_17_22_16_52_377,(uint8*)"");
                    return S_ATC_FAIL;
                }

                EFS_NvitemRead(NV_FIRBOOT_CONFIG_ID,sizeof(FIRBOOT_SEED_NV_T),seed_str1);
                seed_str1[PARA1.str_ptr->str_len] = '\0';
                //SCI_TRACE_LOW:"winnie:read seed:%s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10591_112_2_17_22_16_52_378,(uint8*)"s",seed_str1);
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
#endif /* _ULTRA_LOW_CODE_ */
}
#ifdef _NDS_EXT_CMD_
/*****************************************************************************/
// Description : This function get FIRBOOT seed
// lobal resource dependence :
// Author: Winnie zhu
// Note:
/*****************************************************************************/
BOOLEAN ATC_FIRBOOTGetSeed(
                        uint8        *seed_ptr,    // in: the ascii format string
                        uint16        length       // in: the length of binary string
                        )
{
    if(length < sizeof(FIRBOOT_SEED_NV_T))
    {
        return FALSE;
    }
    if(NVERR_NONE != EFS_NvitemRead(NV_FIRBOOT_CONFIG_ID,sizeof(FIRBOOT_SEED_NV_T),seed_ptr))
    {
        return FALSE;
    }
    return TRUE;
}
#endif
/*****************************************************************************/
// Description : This function handle the AT+VGSMSW command
//                 Automatic downloader
// Global resource dependence : none
// Author:winnie.zhu
// Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessVGSMSW )
{
#ifdef _NDS_EXT_CMD_
    uint8 rand_str[17];
    uint8 ndssign[17];
    int ndssign_len;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG && (16 ==PARA1.str_ptr->str_len))
            {
                SCI_MEMCPY(rand_str,PARA1.str_ptr->str_ptr,PARA1.str_ptr->str_len);
                rand_str[PARA1.str_ptr->str_len] = '\0';
                SCI_MEMSET(ndssign,0,sizeof(ndssign));
                if(!sarft_sign_data(s_swversion,strlen((char *)s_swversion),rand_str,strlen((char *)rand_str),
                                    s_cid,s_gmid,s_seed,ndssign,&ndssign_len))
                {
                    //SCI_TRACE_LOW:"ATC: sarft sign data fail."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10653_112_2_17_22_16_52_379,(uint8*)"");
                    return ERR_UNKNOWN;
                }

                sprintf((char*)g_rsp_str,"+VGSMSW: \"%s\",\"%s\",\"%s\"",rand_str,s_swversion,ndssign);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
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
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
// Description : This function handle the AT+POSI command
//                 Automatic downloader
// Global resource dependence : none
// Author:winnie.zhu
// Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPOSI )
{
#ifdef _NDS_EXT_CMD_
    MN_PHONE_CURRENT_PLMN_INFO_T   plmn_info;
    MN_PHONE_SCELL_MEAS_T          rssi;
    APP_MN_NCELL_RSSI_CNF_T        ncell = {0};
    MN_PHONE_CELL_MEAS_T    cell_power[MN_MAX_CELL_NUM * 2] = {0}; //include scell
    uint8           cell_id[MN_MAX_CELL_NUM * 2]={0};
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
#ifndef MS_OAPI_SUPPORT
    MN_PHONE_CELL_MEAS_T    scell = {0};
#endif

    uint16 mode;
    uint8 end = 0;
    uint8 rand_str[40];
    uint8 ndssign[30];
    //uint8 rand_nds_data[16];
    //uint8 ndssign_str[50];
    int ndssign_len = 30;
    //uint16 rand_nds_len = 0;
    uint8 data_str[255];
    int32 i,j;
    uint8 *temp_str;
    uint8 cell_num = 0;
    uint8 temp1;
    //int    sign_errno = 0;


    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if (!PARAM1_FLAG || !PARAM2_FLAG ||(32 !=PARA2.str_ptr->str_len))
        {
            return ERR_INVALID_INDEX;
        }
        mode  = (uint16)(PARA1.num);
        SCI_MEMCPY(rand_str,PARA2.str_ptr->str_ptr,PARA2.str_ptr->str_len);
        rand_str[PARA2.str_ptr->str_len] = '\0';

        switch (mode)
        {
            case 0:
            {
                end = 1;
                SCI_MEMSET(&plmn_info, 0, sizeof(MN_PHONE_CURRENT_PLMN_INFO_T));
                SCI_MEMSET(&rssi, 0, sizeof(MN_PHONE_SCELL_MEAS_T));
                plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
                rssi = MNPHONE_GetRSSIEx(dual_sys);
                // check the status of the phone
                if (PLMN_NO_SERVICE == plmn_info.plmn_status)
                {
                    sprintf((char*)data_str, "%d,%d", mode,end);
                    if(!sarft_sign_data(data_str,strlen((char *)data_str),rand_str,strlen((char *)rand_str),
                                        s_cid,s_gmid,s_seed,ndssign,&ndssign_len))
                    {
                        //SCI_TRACE_LOW:"ATC: sarft sign data fail."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10738_112_2_17_22_16_52_380,(uint8*)"");
                        return ERR_UNKNOWN;
                    }
                    sprintf((char*)g_rsp_str, "%s: %d,%d,\"%s\",\"%s\"",
                    g_atc_info_table[AT_CMD_POSI].cmd_name->str_ptr,mode,end,rand_str,ndssign);

                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                #ifdef MS_OAPI_SUPPORT
                    sprintf((char*)data_str, "%d,%d,%02d,%d,%d,%d,%d,%d",mode,plmn_info.mcc,plmn_info.mnc,
                        plmn_info.lac,plmn_info.cell_id,rssi.bsic,((rssi.rxlev+1) / 2),end);
                    if(!sarft_sign_data(data_str,strlen((char *)data_str),rand_str,strlen((char *)rand_str),
                                        s_cid,s_gmid,s_seed,ndssign,&ndssign_len))
                    {
                        //SCI_TRACE_LOW:"ATC: sarft sign data fail."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10754_112_2_17_22_16_52_381,(uint8*)"");
                        return ERR_UNKNOWN;
                    }
                    sprintf((char*)g_rsp_str, "%s: %d,%d,%02d,%d,%d,%d,%d,%d,\"%s\",\"%s\"",
                        g_atc_info_table[AT_CMD_POSI].cmd_name->str_ptr,mode,plmn_info.mcc,plmn_info.mnc,
                        plmn_info.lac,plmn_info.cell_id,rssi.bsic,((rssi.rxlev+1) / 2),end,rand_str,ndssign);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                #else
                    MNPHONE_GetMeasReportEx(dual_sys, &ncell, &scell);

                    if(scell.cell_present)
                    {
                        sprintf((char*)data_str, "%d,%d,%02d,%d,%d,%d,%d,%d",mode,plmn_info.mcc,plmn_info.mnc,
                        plmn_info.lac,plmn_info.cell_id,rssi.bsic,((rssi.rxlev+1) / 2),end);
                        if(!sarft_sign_data(data_str,strlen((char *)data_str),rand_str,strlen((char *)rand_str),
                                            s_cid,s_gmid,s_seed,ndssign,&ndssign_len))
                        {
                            //SCI_TRACE_LOW:"ATC: sarft sign data fail."
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10771_112_2_17_22_16_52_382,(uint8*)"");
                            return ERR_UNKNOWN;
                        }
                        sprintf((char*)g_rsp_str, "%s: %d,%d,%02d,%d,%d,%d,%d,%d,\"%s\",\"%s\"",
                            g_atc_info_table[AT_CMD_POSI].cmd_name->str_ptr,mode,plmn_info.mcc,plmn_info.mnc,
                            plmn_info.lac,plmn_info.cell_id,rssi.bsic,((rssi.rxlev+1) / 2),end,rand_str,ndssign);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                    else //sometimes MN is normal but RR is doing power sweep, so return no service at this time
                    {
                        sprintf((char*)data_str, "%d,%d", mode,end);
                        if(!sarft_sign_data(data_str,strlen((char *)data_str),rand_str,strlen((char *)rand_str),
                                        s_cid,s_gmid,s_seed,ndssign,&ndssign_len))
                        {
                            //SCI_TRACE_LOW:"ATC: sarft sign data fail."
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10785_112_2_17_22_16_52_383,(uint8*)"");
                            return ERR_UNKNOWN;
                        }
                        sprintf((char*)g_rsp_str, "%s: %d,%d,\"%s\",\"%s\"",
                           g_atc_info_table[AT_CMD_POSI].cmd_name->str_ptr,mode,end,rand_str,ndssign);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                #endif
                }
                break;
            }

            case 1:
            {
                end = 0;
                cell_num = 0;
                sprintf((char*)g_rsp_str, "%s:%d,", g_atc_info_table[AT_CMD_POSI].cmd_name->str_ptr, mode);
                temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];

                for (dual_sys = MN_DUAL_SYS_1; dual_sys < MN_DUAL_SYS_3; dual_sys++)
                {
                    if (DMSIM_SimIsReady(MN_DUAL_SYS_1) && (dual_sys == MN_DUAL_SYS_2))
                    {
                        break;
                    }
                    SCI_MEMSET(&plmn_info, 0, sizeof(MN_PHONE_CURRENT_PLMN_INFO_T));
                    SCI_MEMSET(&rssi, 0, sizeof(MN_PHONE_SCELL_MEAS_T));
                    SCI_MEMSET(&ncell, 0, sizeof(APP_MN_NCELL_RSSI_CNF_T));

                    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
                    rssi = MNPHONE_GetRSSIEx(dual_sys);
                    #ifdef MS_OAPI_SUPPORT
                        MNPHONE_GetMeasReportEx(dual_sys, &ncell);
                    #else
                        MNPHONE_GetMeasReportEx(dual_sys, &ncell, &scell);
                    #endif

                    for (i = 0; i < MN_MAX_NCELL_NUM; i++)
                    {
                        //remove the lock of mobile operator
                        if (ncell.ncell_power[i].cell_present && ncell.ncell_power[i].identity_present)
                        {
                            SCI_MEMCPY(&cell_power[cell_num], &ncell.ncell_power[i], sizeof(MN_PHONE_CELL_MEAS_T));
                            cell_id[cell_num] = cell_num;
                            //SCI_TRACE_LOW:"ATC: sim_id=%d, ncell_index=%d, arfcn=%d, bsic=%d, rxlev=%d, mcc=%d, mnc=%d, lac=%d, mnc_digit_num=%d, cell_id=%d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10833_112_2_17_22_16_52_384,(uint8*)"dddddddddd",dual_sys, cell_num, cell_power[cell_num].arfcn, cell_power[cell_num].bsic, cell_power[cell_num].rxlev, cell_power[cell_num].lai.mcc,cell_power[cell_num].lai.mnc, cell_power[cell_num].lai.lac,cell_power[cell_num].lai.mnc_digit_num, cell_power[cell_num].cell_id);
                            cell_num++;
                        }
                    }

                    //add scell
                    if(PLMN_NO_SERVICE != plmn_info.plmn_status)
                    {
                        BOOLEAN scell_present = FALSE;
                        for(i = 0; i < cell_num; i++)
                        {
                            //check ncell including scell or not
                            if(cell_power[i].cell_id == plmn_info.cell_id)
                            {
                                scell_present = TRUE;
                                break;
                            }
                        }
                        if(!scell_present)
                        {
                            cell_power[cell_num].lai.mcc = plmn_info.mcc;
                            cell_power[cell_num].lai.mnc = plmn_info.mnc;
                            cell_power[cell_num].lai.lac = plmn_info.lac;
                            cell_power[cell_num].cell_id = plmn_info.cell_id;
                            cell_power[cell_num].lai.mnc_digit_num= plmn_info.mnc_digit_num;
                            cell_power[cell_num].bsic    = rssi.bsic;
                            cell_power[cell_num].rxlev   = rssi.rxlev;
                            //SCI_TRACE_LOW:"ATC: sim_id=%d, scell_index=%d, arfcn=%d, bsic=%d, rxlev=%d, mcc=%d, mnc=%d, lac=%d, mnc_digit_num=%d, cell_id=%d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10864_112_2_17_22_16_52_385,(uint8*)"dddddddddd",dual_sys, cell_num, cell_power[cell_num].arfcn, cell_power[cell_num].bsic, cell_power[cell_num].rxlev, cell_power[cell_num].lai.mcc,cell_power[cell_num].lai.mnc, cell_power[cell_num].lai.lac,cell_power[cell_num].lai.mnc_digit_num, cell_power[cell_num].cell_id);

                            cell_id[cell_num] = cell_num;
                            cell_num++;
                        }

                        //SCI_TRACE_LOW:"ATC_ProcessPOSI sim %d, ncell_has_scell %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10870_112_2_17_22_16_52_386,(uint8*)"dd", dual_sys, scell_present);
                    }
                }


                //return OK without any cell
                if(cell_num == 0)
                {
                    //SCI_TRACE_LOW:"ATC_ProcessPOSI ncell_num = 0"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10878_112_2_17_22_16_52_387,(uint8*)"");
                    return S_ATC_SUCCESS;
                }

                for(i = 1; i < cell_num; i++)
                {
                    temp1 = cell_id[i];

                    for(j = i - 1; j >= 0 && cell_power[cell_id[j]].rxlev < cell_power[temp1].rxlev; j--)
                    {
                        cell_id[j + 1] = cell_id[j];
                    }
                    cell_id[j + 1] = temp1;
                }


                for(j = 0; j < cell_num; j++ )
                {
                    i = cell_id[j];
                    //SCI_TRACE_LOW:"ATC: cell_num=%d, cell_power[%d].rxlev=%d "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10897_112_2_17_22_16_53_388,(uint8*)"ddd",cell_num, i, cell_power[i].rxlev);
                    if(cell_power[i].lai.mcc == 0 || cell_power[i].cell_id == 0 || cell_power[i].rxlev == 0)
                    {
                        //SCI_TRACE_LOW:"ATC_ProcessPOSI invalid cell mnc %d,cell_id %x, rxlev %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10902_112_2_17_22_16_53_389,(uint8*)"ddd",cell_power[i].lai.mcc,cell_power[i].cell_id, cell_power[i].rxlev);
                        continue;
                    }

                    sprintf((char*)temp_str,"%d,%02d,%x,%x,%x,%d,%x", cell_power[i].lai.mcc,
                        cell_power[i].lai.mnc, cell_power[i].lai.lac,
                        cell_power[i].cell_id, cell_power[i].bsic,
                        cell_power[i].rxlev, end);
                    //SCI_TRACE_LOW:"ATC POSI: %s"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10910_112_2_17_22_16_53_390,(uint8*)"s", temp_str);
                    temp_str = &g_rsp_str[strlen((char*)g_rsp_str)];
                    *temp_str = ',';
                    temp_str++;
                    *temp_str = '\0';
                }
                sprintf((char *)data_str, "%d,%s,1", mode, temp_str);

                g_rsp_str[strlen((char*)g_rsp_str) - 2] = '1'; //end
                g_rsp_str[strlen((char*)g_rsp_str) - 1] = ',';
                sprintf((char *)&g_rsp_str[strlen((char*)g_rsp_str)],"\"%s\"", rand_str);
         #if 0
                rand_nds_len = ConvertStrToNDSData(rand_str, strlen(rand_str), rand_nds_data);

                sign_errno = sarft_sign_data(g_rsp_str, strlen(g_rsp_str), rand_nds_data, rand_nds_len,
                          g_cid_nds_data, g_mid_nds_data, g_seed_nds_data, ndssign, &ndssign_len);
                if(0 != sign_errno)
                {
                    //SCI_TRACE_LOW:"ATC: sarft sign data fail."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10928_112_2_17_22_16_53_391,(uint8*)"");

                    //SCI_TRACE_LOW:"ATC_ProcessPOSI sign str %s"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10930_112_2_17_22_16_53_392,(uint8*)"s", sarft_error_str(sarft_sign_errno));
                    return ERR_UNKNOWN;
                }

                ConvertNDSDataToStr(ndssign, ndssign_len, ndssign_str);
                sprintf((char *)&g_rsp_str[strlen((char*)g_rsp_str)],",\"%s\"", ndssign_str);
         #else
                if(!sarft_sign_data(data_str,strlen((char *)data_str),rand_str,strlen((char *)rand_str),
                    s_cid,s_gmid,s_seed,ndssign,&ndssign_len))
                {
                    //SCI_TRACE_LOW:"ATC: sarft sign data fail."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10940_112_2_17_22_16_53_393,(uint8*)"");
                    return ERR_UNKNOWN;
                }

                sprintf((char *)&g_rsp_str[strlen((char*)g_rsp_str)],"\"%s\"", ndssign);

         #endif
                g_rsp_str[strlen((char*)g_rsp_str)] = '\0';
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                //SCI_TRACE_LOW:"ATC_ProcessPOSI: %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10949_112_2_17_22_16_53_394,(uint8*)"s",g_rsp_str);
                //SCI_TRACE_LOW:"ATC: the string length is %d,cell_num=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_10950_112_2_17_22_16_53_395,(uint8*)"dd", strlen((char*)g_rsp_str), cell_num);
                break;
            }

            default:
                break;
        }
        break;

    default:
        break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _NDS_EXT_CMD_ */
}
#endif

#ifdef FM_SUPPORT
LOCAL void AudioDrvCallback(
                HAUDIO hAudio,
                uint32 notify_info,
                uint32 affix_info
                )
{

    SCI_TRACE_LOW("ATC: AudioDrvCallback() entry, hAudio=0x%x, notify_info=%d,affix_info=%d, END_IND=%d", hAudio, notify_info, affix_info);

 }

/*****************************************************************************/
//  Description:    This function is used to set voice codec enable/disable.
//  Author:         shujing.dong
//  Note:
/*****************************************************************************/
static BOOLEAN ATC_EnableFmVoiceCodec ( // If succeed, return SCI_TRUE, else return error value
    BOOLEAN is_enable          // SCI_TRUE: Enable,   SCI_FALSE: Disable
)
{
#ifndef WIN32
    AUDIO_RESULT_E audio_result = 0;

    SCI_TRACE_LOW("ATC: ATC_EnableVoiceCodec: is_enable = %d", is_enable);

    if (is_enable)
    {
        if(INVALID_HANDLE == hAtcFmHandle)
        {
            hAtcFmHandle = AUDIO_CreateNormalHandle(hNullCodec, PNULL, hFMDev, PNULL, AudioDrvCallback);
            SCI_ASSERT(INVALID_HANDLE != hAtcFmHandle); /*assert verified*/
        }
        audio_result = AUDIO_Play (hAtcFmHandle, 0);
        SCI_TRACE_LOW("ATC: AUDIO_Play: audio_result = %d, hAtcFmHandle=0x%x", audio_result, hAtcFmHandle);

    }
    else
    {
        audio_result = AUDIO_Stop (hAtcFmHandle);
        SCI_TRACE_LOW("ATC: AUDIO_Stop: audio_result = %d, hAtcFmHandle=0x%x", audio_result, hAtcFmHandle);
    }

    if(audio_result == AUDIO_NO_ERROR)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

#else
        return TRUE;
#endif
}
#endif

/*****************************************************************************/
//  Description : This function handles the AT+SPFMMODE command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPFMMODE)
{
#ifdef FM_SUPPORT
    ATC_STATUS     status = S_ATC_SUCCESS;
#ifndef WIN32
    uint32 fm_status = FM_SUCCESS;
    AUDIO_RESULT_E audio_result = 0;
    BOOLEAN res = TRUE;

     switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARA1.num)
            {
                fm_status = FM_Init();
                if (fm_status != FM_SUCCESS)
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMMODE fail to enter EUT mode! cause=%d", fm_status);
                    status = S_ATC_FAIL;
                    break;
                }

                fm_status = FM_Open();
                if (fm_status != FM_SUCCESS)
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMMODE fail to enter EUT mode! cause=%d", fm_status);
                    status = S_ATC_FAIL;
                    break;
                }

                audio_result =  AUDIO_GetVolume(&s_fm_volum);
                SCI_TRACE_LOW("ATC: ATC_ProcessSPFMMODE AUDIO_GetVolume audio_result=%d,volum=%d", audio_result, s_fm_volum);

                audio_result = AUDIO_SetVolume(FM_VOLUME_LEVEL_MAX);
                SCI_TRACE_LOW("ATC: ATC_ProcessSPFMMODE AUDIO_SetVolume audio_result=%d, FM_VOLUME_LEVEL_MAX=%d", audio_result, FM_VOLUME_LEVEL_MAX);

                res = ATC_EnableFmVoiceCodec(TRUE);
                if (!res)
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMMODE ATC_EnableFmVoiceCodec FAIL=%d", res);
                    status = S_ATC_FAIL;
                    break;
                }

            }
            else
            {
                AUDIO_SetVolume(s_fm_volum);
                res = ATC_EnableFmVoiceCodec(FALSE);
                if (!res)
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMMODE ATC_EnableFmVoiceCodec FAIL=%d", res);
                    //status = S_ATC_FAIL;
                    //break;
                }

                fm_status = FM_Close();
                if (fm_status != FM_SUCCESS)
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMMODE  fail to level  mode! cause=%d", fm_status);
                    status = S_ATC_FAIL;
                    break;
                }
            }
        }
        break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
        break;
    }
#endif
    return status;
#else
    return S_ATC_FAIL;
#endif
}


/*****************************************************************************/
//  Description : This function handles the AT+SPFMTUNE command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPFMTUNE)
{
#ifdef FM_SUPPORT
    ATC_STATUS     status = S_ATC_SUCCESS;
#ifndef WIN32
    uint32 fm_status = FM_SUCCESS;
    uint32 fm_freq = 0;
    uint32 rssi = 0;

     switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            fm_freq = PARA1.num * 10;
            SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTUNE TUNE mode of FM, fm_freq %d", fm_freq);
            fm_status = FM_ManualSeek(fm_freq, 0, 0, PNULL);// FM_PlayStart(fm_freq);
            if (fm_status != FM_SUCCESS)
            {
                SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTUNE fail to enter TUNE mode! cause=%d", fm_status);
                status = S_ATC_FAIL;
                break;
            }
           SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTUNE TUNE mode of FM, fm_freq %d", fm_freq);
        }
        break;

        case ATC_CMD_TYPE_READ:
        {
            SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTUNE TUNE mode of FM, fm_freq %d", fm_freq);
            if (FM_IsInit())
            {
                fm_freq = FM_OPERATION_get_freq_seek_info();//lint !e746
                rssi =FM_OPERATION_get_inpwr_info_rt();//lint !e746
            }
            sprintf((char *)g_rsp_str, "+SPFMTUNE:TUNE=%d,%d", fm_freq, rssi);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
        break;
    }
#endif
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function is used to get the parameters of diag command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN ATC_GetDiagParamsEx(
    uint8            *in_ptr,
    uint8             in_len,
    ATC_DIAG_PARAM_EX_T *param_ptr
)
{
    uint8            i;
    uint8            j = 0;
    uint8           k= 0;
    BOOLEAN          flag = TRUE;
    BOOLEAN         mode_flag = FALSE;

    if(NULL == in_ptr || NULL == param_ptr || in_len < 2)
    {
        return FALSE;
    }

    SCI_MEMSET((void*)param_ptr->mode, 0, ATC_MAX_DIAG_MODE_LEN);
    param_ptr->mode_len = 0;
    param_ptr->cmd_type = ATC_CMD_TYPE_SET;

    for(i = 0; i < in_len; i++)
    {
       if (in_ptr[i] == ',')
        {
            k++;
            j = 0;
            mode_flag = TRUE;

            if (4 < k)
            {
                flag = FALSE;
                break;
            }
        }
        else if (in_ptr[i] == '?')
        {
            param_ptr->cmd_type = ATC_CMD_TYPE_READ;
            break;
        }
        else
        {
            if(!mode_flag)
            {
                if(j < ATC_MAX_DIAG_MODE_LEN)
                {
                    if(in_ptr[i] != '"')
                    {
                        param_ptr->mode[j++] = in_ptr[i];
                        param_ptr->mode_len += 1;

                    }
                }
            }
            else
            {
                if ((j < ATC_MAX_DIAG_ACT_LEN) && (k>=1))
                {
                    if (in_ptr[i] != '"')
                    {
                        param_ptr->act_value[k-1][j++] = in_ptr[i];
                        param_ptr->act_len[k-1] += 1;
                    }
                }
             }

        }

    }

    SCI_TRACE_LOW("ATC: param_ptr->cmd_type=%d, param_ptr.mode=%s", param_ptr->cmd_type,param_ptr->mode);
    SCI_TRACE_LOW("ATC:param_ptr.act_value[0]=%d, %s", param_ptr->act_len[0], param_ptr->act_value[0]);
    SCI_TRACE_LOW("ATC:param_ptr.act_value[1]=%d, %s", param_ptr->act_len[1], param_ptr->act_value[1]);
    SCI_TRACE_LOW("ATC:param_ptr.act_value[2]=%d, %s", param_ptr->act_len[2], param_ptr->act_value[2]);
    SCI_TRACE_LOW("ATC:param_ptr.act_value[3]=%d, %s", param_ptr->act_len[3], param_ptr->act_value[3]);

    SCI_TRACE_LOW("ATC: get the diag parameters %d", flag);
    return flag;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPFMTEST command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPFMTEST)
{
    ATC_STATUS           status = S_ATC_SUCCESS;//S_ATC_DEFAULT_HANDLE;
#ifndef WIN32
#if defined (FM_SUPPORT) && defined (FM_EX_SUPPORT)
    ATC_DIAG_PARAM_EX_T      param = {0};
    ATC_FM_MODE_E         fm_mode = ATC_MAX_FM_MODE_NUM;
    uint8                 param_len;
    uint8                *temp_ptr = PNULL;

    uint32                 act_value = 0;
    uint8                 i;
    uint32 fm_status = FM_SUCCESS;
    AUDIO_RESULT_E audio_result = 0;
    BOOLEAN res = TRUE;

    static uint8    fm_testmode=0;
    static uint32  fm_volume =0;
    static uint8    fm_mute =0;
    static uint32  fm_mute_old_volume = 12;
    static uint8    fm_seek_dir =0;

    static const char    *s_fm_mode_str[ATC_MAX_FM_MODE_NUM] =
    {
        "TESTMODE", "VOLUME", "MUTE", "TUNE", "SEEK",
        "RREG", "WREG", "STATUS", "IQ", "DATACAPTURE", "SHORTANT"
     };

    if(!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        SCI_TRACE_LOW("ATC: diag the param is error");
        status = ERR_OPERATION_NOT_ALLOWED;
        sprintf((char *)g_rsp_str, "+SPFMTEST:ERR=%d", status);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return status;
    }

    temp_ptr  = PARA1.str_ptr->str_ptr + 1;
    param_len = PARA1.str_ptr->str_len - 1;

    if (!ATC_GetDiagParamsEx(temp_ptr, param_len, &param))
    {
        SCI_TRACE_LOW("ATC: fail to get param");
        status = ERR_OPERATION_NOT_ALLOWED;
        sprintf((char *)g_rsp_str, "+SPFMTEST:ERR=%d", status);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return status;
    }

    param.mode[param.mode_len] = '\0';

    switch(param.cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        case ATC_CMD_TYPE_READ:
            for (i = 0; i < ATC_MAX_FM_MODE_NUM; i++)
            {
                if(!strcmp((char *)param.mode, s_fm_mode_str[i]))
                {
                    fm_mode = (ATC_FM_MODE_E)((i < ATC_MAX_FM_MODE_NUM) ? i : (i - ATC_MAX_FM_MODE_NUM));
                    break;
                }
            }
            break;

        default:
            SCI_TRACE_LOW("ATC: invalid cmd type=%d", param.cmd_type);
            status = ERR_OPERATION_NOT_SUPPORTED;
            sprintf((char *)g_rsp_str, "+SPFMTEST:ERR=%d", status);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            return status;
    }

    switch(fm_mode)
    {
        case ATC_FM_TESTMODE:
        {

            if (param.cmd_type == ATC_CMD_TYPE_SET)
            {
                act_value = (uint8)atoi((char *)param.act_value[0]);
                SCI_TRACE_LOW("ATC: ATC_FM_TESTMODE, act_value %d", act_value);
                if (act_value)
                {
                    fm_status = FM_Init();
                    if (fm_status != FM_SUCCESS)
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST fail to enter EUT mode! cause=%d", fm_status);
                        status = S_ATC_FAIL;
                        break;
                    }

                    fm_status = FM_Open();
                    if (fm_status != FM_SUCCESS)
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST fail to enter EUT mode! cause=%d", fm_status);
                        status = S_ATC_FAIL;
                        break;
                    }

                    /*audio_result =  AUDIO_GetVolume(&s_fm_volum);
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST AUDIO_GetVolume audio_result=%d,volum=%d", audio_result, s_fm_volum);

                    audio_result = AUDIO_SetVolume(FM_VOLUME_LEVEL_MAX);
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST AUDIO_SetVolume audio_result=%d, FM_VOLUME_LEVEL_MAX=%d", audio_result, FM_VOLUME_LEVEL_MAX);
                  */
                    res = ATC_EnableFmVoiceCodec(TRUE);
                    if (!res)
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST ATC_EnableFmVoiceCodec FAIL=%d", res);
                        status = S_ATC_FAIL;
                        break;
                    }
                    fm_testmode = 1;
                }
                else
                {
                   // AUDIO_SetVolume(s_fm_volum);
                    res = ATC_EnableFmVoiceCodec(FALSE);
                    if (!res)
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST ATC_EnableFmVoiceCodec FAIL=%d", res);
                    }

                    fm_status = FM_Close();
                    if (fm_status != FM_SUCCESS)
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST  fail to level  mode! cause=%d", fm_status);
                        status = S_ATC_FAIL;
                        break;
                    }
                    fm_testmode = 0;

                }

            }
            else if (param.cmd_type == ATC_CMD_TYPE_READ)
            {
                sprintf((char *)g_rsp_str, "+SPFMTEST:%s=%d", param.mode, fm_testmode);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }

        }
        break;


        case ATC_FM_VOLUME:
        {

            if (param.cmd_type == ATC_CMD_TYPE_SET)
            {
                act_value = (uint8)atoi((char *)param.act_value[0]);
                SCI_TRACE_LOW("ATC: ATC_FM_VOLUME, act_value %d", act_value);

                audio_result = AUDIO_SetVolume(act_value);
                SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST AUDIO_SetVolume audio_result=%d, FM_VOLUME_LEVEL_MAX=%d", audio_result, FM_VOLUME_LEVEL_MAX);
                if(0 != audio_result)
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST  set volume FAIL");
                    status = S_ATC_FAIL;
                    break;
                }
                fm_volume = act_value;
            }
            else if (param.cmd_type == ATC_CMD_TYPE_READ)
            {
                audio_result =  AUDIO_GetVolume(&fm_volume);
                if(0 != audio_result)
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST  GET volume FAIL");
                    status = S_ATC_FAIL;
                    break;
                }
                sprintf((char *)g_rsp_str, "+SPFMTEST:%s=%d", param.mode, fm_volume);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }

        }
        break;

        case ATC_FM_MUTE:
        {

            if (param.cmd_type == ATC_CMD_TYPE_SET)
            {
                act_value = (uint8)atoi((char *)param.act_value[0]);
                SCI_TRACE_LOW("ATC: ATC_FM_MUTE, act_value %d", act_value);


                if(act_value)
                {
                    audio_result =  AUDIO_GetVolume(&fm_mute_old_volume);
                    if(0 != audio_result)
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST  get volume FAIL");
                        status = S_ATC_FAIL;
                        break;
                    }
                   audio_result = AUDIO_SetVolume(0);
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST set mute audio_result =%d", audio_result);
                    if(0 != audio_result)
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST  set mute FAIL");
                        status = S_ATC_FAIL;
                        break;
                    }

                }
                else
                {
                   audio_result = AUDIO_SetVolume(fm_mute_old_volume);
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST set mute audio_result =%d", audio_result);
                    if(0 != audio_result)
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST  set mute FAIL");
                        status = S_ATC_FAIL;
                        break;
                    }
                }

                fm_mute = act_value;

            }
            else if (param.cmd_type == ATC_CMD_TYPE_READ)
            {
                sprintf((char *)g_rsp_str, "+SPFMTEST:%s=%d", param.mode, fm_mute);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;

            }

        }
        break;

        case ATC_FM_TUNE:
        {

            if (param.cmd_type == ATC_CMD_TYPE_SET)
            {
                act_value = atoi((char *)param.act_value[0]);
                SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST TUNE act_value=%d", act_value);

                fm_status =FM_ManualSeek(act_value, 0, 0, PNULL);// FM_PlayStart(fm_freq);
                if (fm_status != FM_SUCCESS)
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST fail to enter TUNE mode! cause=%d", fm_status);
                    status = S_ATC_FAIL;
                    break;
                }
               SCI_TRACE_LOW("ATC: ATC_ProcessSPFMTEST TUNE mode of FM, fm_freq %d", act_value);

            }
            else if (param.cmd_type == ATC_CMD_TYPE_READ)
            {
                uint32 fm_freq = 0;
                uint32 rssi = 0;
                fm_freq = FM_OPERATION_get_freq_seek_info();

                rssi =FM_OPERATION_get_inpwr_info_rt();
                sprintf((char *)g_rsp_str, "+SPFMTEST:%s=%d, %d", param.mode, fm_freq,rssi);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }

        }
        break;

        case ATC_FM_SEEK:
        {

            if (param.cmd_type == ATC_CMD_TYPE_SET)
            {
                uint32 current_fm_freq = 0;
                FM_ALL_CHANNEL_INFO_T channel_info={0};
                FM_SCAN_DIR_E seek_dir = FM_SCAN_DOWN;
                current_fm_freq = FM_OPERATION_get_freq_seek_info();

                fm_seek_dir = (uint8)atoi((char *)param.act_value[0]);
                if(fm_seek_dir == 0)
                {
                    seek_dir = FM_SCAN_DOWN;
                }
                else if(fm_seek_dir == 1)
                {
                    seek_dir = FM_SCAN_UP;
                }
                else
                {
                    SCI_TRACE_LOW("ATC: ATC_FM_SEEK  error fm_seek_dir=%d", fm_seek_dir);
                    status = S_ATC_FAIL;
                    break;
                }

                current_fm_freq= current_fm_freq;
                SCI_TRACE_LOW("ATC: ATC_FM_SEEK, fm_seek_dir %d,current_fm_freq %d", fm_seek_dir, current_fm_freq);
                fm_testmode =FM_OPERATION_autoseek(current_fm_freq, seek_dir, 0, &channel_info);
                SCI_TRACE_LOW("ATC: ATC_FM_SEEK, fm_seek_dir %d, seek_freq %d ", fm_seek_dir, *(channel_info.auto_freq_tab_ptr));

            }
            else if (param.cmd_type == ATC_CMD_TYPE_READ)
            {
              uint32 fm_freq = 0;
                uint32 rssi = 0;
                fm_freq = FM_OPERATION_get_freq_seek_info();

                rssi =FM_OPERATION_get_inpwr_info_rt();
                sprintf((char *)g_rsp_str, "+SPFMTEST:%s=%d, %d", param.mode, fm_freq,rssi);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }

        }
        break;

        case ATC_FM_RREG:
        {

            if (param.cmd_type == ATC_CMD_TYPE_SET)
            {
#if 0
                uint8 reg_addr_len = 0;
                uint32 reg_addr = 0;
                uint32 reg_data =0;
                BOOLEAN result = TRUE;

                reg_addr_len = param.act_len[0];
                if(reg_addr_len == 0)
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    SCI_TRACE_LOW("ATC: ATC_FM_RREG  reg_addr_len =%d", reg_addr_len);
                    break;
                }

                reg_addr = strtoul((char *)param.act_value[0],0, 10);
                SCI_TRACE_LOW("ATC: ATC_FM_RREG  %s, reg_addr =%lu, 0x%x", param.act_value[0], reg_addr, reg_addr);

                if(0 != (reg_addr%4))
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    SCI_TRACE_LOW("ATC: ATC_FM_RREG  reg addr error  reg_addr%4");
                }

                result = FM_OPERATION_read_reg(reg_addr, &reg_data);
                SCI_TRACE_LOW("ATC: ATC_FM_RREG  %d, reg_addr =%lu", result, reg_data);

                if(FALSE == result)
                {
                    status = S_ATC_FAIL;
                    SCI_TRACE_LOW("ATC: ATC_FM_RREG  result =%d", result);
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+SPFMRREG: %lu,%lu", reg_addr,reg_data);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_SUCCESS;
                }

#else
                BOOLEAN result = FALSE;
                if(FALSE == result)
                {
                    status = S_ATC_FAIL;
                    SCI_TRACE_LOW("ATC: ATC_FM_RREG  result =%d", result);
                }
                else
                {
                   // sprintf((char *)g_rsp_str, "+SPFMRREG: %lu,%lu", reg_addr,reg_data);
                    //ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_SUCCESS;
                }
#endif

            }
            else
            {
                SCI_TRACE_LOW("ATC: ATC_FM_RREG  error cmd_type =%d", param.cmd_type);
                status = ERR_OPERATION_NOT_SUPPORTED;
                break;
            }

        }
        break;

        case ATC_FM_WREG:
        {

            if (param.cmd_type == ATC_CMD_TYPE_SET)
            {
            #if 0
                uint8 reg_addr_len = 0;
                uint32 reg_addr = 0;
                uint8 reg_data_len = 0;
                uint32 reg_data =0;
                BOOLEAN result = TRUE;

                reg_addr_len = param.act_len[0];
                if(reg_addr_len == 0)
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    SCI_TRACE_LOW("ATC: ATC_FM_WREG  reg_addr_len =%d", reg_addr_len);
                    break;
                }
                reg_addr = strtoul((char *)param.act_value[0], 0, 10);
                SCI_TRACE_LOW("ATC: ATC_FM_WREG  reg_addr  =%lu, 0x%x", reg_addr, reg_addr);

                if(0 != (reg_addr%4))
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    SCI_TRACE_LOW("ATC: ATC_FM_WREG  reg addr error  reg_addr%4 ");
                }

                reg_data_len = param.act_len[1];
                if(reg_data_len == 0)
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    SCI_TRACE_LOW("ATC: ATC_FM_WREG  reg_data_len =%d", reg_data_len);
                    break;
                }
                reg_data = strtoul((char *)param.act_value[1], 0, 10);

                SCI_TRACE_LOW("ATC: reg_data =%lu", reg_data);

                result =  FM_OPERATION_write_reg(reg_addr, reg_data);
                if(FALSE == result)
                {
                    status = S_ATC_FAIL;
                    SCI_TRACE_LOW("ATC: ATC_FM_WREG  result =%d", result);
                }
#else
		  BOOLEAN result = TRUE;
                if(FALSE == result)
                {
                    status = S_ATC_FAIL;
                    SCI_TRACE_LOW("ATC: ATC_FM_WREG  result =%d", result);
                }
#endif

            }
            else
            {
                SCI_TRACE_LOW("ATC: ATC_FM_WREG  error cmd_type =%d", param.cmd_type);
                status = ERR_OPERATION_NOT_SUPPORTED;
                break;
            }

        }
        break;

        case ATC_FM_STATUS:
        {

            if (param.cmd_type == ATC_CMD_TYPE_READ)
            {
                uint32 fm_freq = 0;
                uint32 rssi = 0;
                uint32 offset = 0;
                uint32 agc = 0;

                fm_freq = FM_OPERATION_get_freq_seek_info();  ////当前频率值

                rssi =FM_OPERATION_get_inpwr_info_rt();

                offset = FM_OPERATION_get_freq_offset_info();//lint !e746

                agc = FM_OPERATION_get_agc_sts_info();//lint !e746

                sprintf((char *)g_rsp_str, "+SPFMTEST:%s=fm_freq %d, rssi %d, offset %d, agc %d", param.mode, fm_freq, rssi, offset, agc);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;

            }
            else
            {
                SCI_TRACE_LOW("ATC: ATC_FM_STATUS  error cmd_type =%d", param.cmd_type);
                status = S_ATC_FAIL;
            }

        }
        break;

        case ATC_FM_IQ:
        {

            if (param.cmd_type == ATC_CMD_TYPE_SET)
            {
                uint8 iq_flag = 0;

                iq_flag = (uint8)atoi((char *)param.act_value[0]);

                if(0 == iq_flag)
                {
                    FM_OPERATION_IQrecover();//lint !e746
                }
                else if(1 == iq_flag)
                {
                    FM_OPERATION_IQmonitor();//lint !e746
                }
                else
                {
                    SCI_TRACE_LOW("ATC: ATC_FM_IQ  error iq_flag =%d", iq_flag);
                    status = S_ATC_FAIL;
                    break;
                }
            }
            else
            {
                SCI_TRACE_LOW("ATC: ATC_FM_IQ  error ");
                status = S_ATC_FAIL;
            }

        }
        break;

        case ATC_FM_DATACAPTURE:
        {
            uint32 result = 0;

            if (param.cmd_type == ATC_CMD_TYPE_SET)
            {

                uint32 level = 0;
                uint32 data_num = 0;

                level = atol((char *)param.act_value[0]);

                data_num = atol((char *)param.act_value[1]);

                SCI_TRACE_LOW("ATC: ATC_FM_DATACAPTURE  level=%d, data_num =%d", level, data_num);

                result = FM_OPERATION_data_capture(level, data_num);//Level为0-0x1f，i_data_num： 0-0x10000

                 SCI_TRACE_LOW("ATC: ATC_FM_DATACAPTURE  result=%d", result);

            }
            else
            {
                SCI_TRACE_LOW("ATC: ATC_FM_DATACAPTURE  error ");
                status = S_ATC_FAIL;
                break;
            }

        }
        break;

        case ATC_FM_SHORTANT:
        {

            if (param.cmd_type == ATC_CMD_TYPE_SET)
            {
                uint8 ant_set = 0;

                ant_set = (uint8)atoi((char *)param.act_value[0]);

                if(ant_set == 0)
                {
                     FM_OPERATION_long_ant_set();//lint !e746
                }
                else
                {
                     FM_OPERATION_short_ant_set();//lint !e746
                }

                SCI_TRACE_LOW("ATC: ATC_FM_SHORTANT  ant_set=%d", ant_set);

            }
            else
            {
                SCI_TRACE_LOW("ATC: ATC_FM_SHORTANT  error cmd_type =%d", param.cmd_type);
                status = S_ATC_FAIL;
                break;
            }

        }
        break;
        default:
            SCI_TRACE_LOW("ATC: FM mode %d is invalid", fm_mode);
            status = ERR_OPERATION_NOT_SUPPORTED;
            sprintf((char *)g_rsp_str, "+SPFMTEST:ERR=%d", status);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            return status;
    }


    if (status == S_ATC_SUCCESS)
    {
        if (param.cmd_type == ATC_CMD_TYPE_SET)
        {
            sprintf((char *)g_rsp_str, "+SPFMTEST:OK");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            sprintf((char *)g_rsp_str, "+SPFMTEST:%s=%d", param.mode, act_value);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
    }
    else
    {
        SCI_TRACE_LOW("ATC: FM mode %s test fail, cause %d", param.mode, status);
        sprintf((char *)g_rsp_str, "+SPFMTEST:ERR=%d", status);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }


#endif
#endif
    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPDSP command to read/write RF register
//  Global resource dependence :
//                      P0	P1	            P2	            P3
//  Reg read	        0	RF reg address	X               X
//  Reg write           1   RF reg address	RF reg value    X
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessRWRFREG)
{
    uint16 atc_p0 = 0;
    uint16 atc_p1 = 0;
    uint16 atc_p2 = 0;
    uint16 atc_p3 = 0;
    uint16 res = 0;
    uint16 reg_val = 0;

    ATC_STATUS     status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:

        if(PARAM1_FLAG || PARAM2_FLAG || PARAM3_FLAG || PARAM4_FLAG)
        {
            if(PARAM1_FLAG)
                atc_p0 = PARA1.num;

            if(PARAM2_FLAG)
                atc_p1 = PARA2.num;

            if(PARAM3_FLAG)
                atc_p2 = PARA3.num;

            if(PARAM4_FLAG)
                atc_p3 = PARA4.num;

            //check whether the parameter is valid
            switch(atc_p0)
			{
			    case 0:
			    case 1:
			    case 2:
			    case 3:
			    case 4:
			        //the p1 shuld not be 0
			        /*if(atc_p1 == 0)
			        {
	                    return S_ATC_FAIL;
			        }*/
                    break;
			    default:
			        return S_ATC_FAIL;
			}

            SCI_TRACE_LOW("ATC: ATC_ProcessRWRFREG p0:0x%x, p1:0x%x, p2:0x%X, p3:0x%X", atc_p0, atc_p1, atc_p2, atc_p3);

            L1API_ReadWriteRfRegister(atc_p0,atc_p1,atc_p2,atc_p3); //lint !e718  !e746

            SCI_Sleep(300);

            if(0 == atc_p0) //read rf register
            {
                res = L1API_ReadWriteRfRegisterResult(atc_p0,&reg_val); //lint !e718  !e746

                switch(res)
                {
                    case 0://read ok
                        sprintf((char *)g_rsp_str, "+RWRFREG: %d", reg_val);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        break;
                    case 1://read failed
                        sprintf((char *)g_rsp_str, "+RWRFREG: read register failed");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    case 2://waiting dsp response
                        sprintf((char *)g_rsp_str, "+RWRFREG: read register failed for waiting dsp response");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    default:
                        sprintf((char *)g_rsp_str, "+RWRFREG: read register failed for L1 return %d.",res);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                }
            }
            else if(1 == atc_p0)// write register
            {
                 res = L1API_ReadWriteRfRegisterResult(atc_p0,&reg_val);

                switch(res)
                {
                    case 0://write ok
                        status = S_ATC_SUCCESS;
                        break;
                    case 1://write failed
                        sprintf((char *)g_rsp_str, "+RWRFREG: write register failed");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    case 2://waiting dsp response
                        sprintf((char *)g_rsp_str, "+RWRFREG: write register failed for waiting dsp response");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    default:
                        sprintf((char *)g_rsp_str, "+RWRFREG: write register failed for L1 return %d.",res);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                }
            }
            else if(4 == atc_p0)// read temperature sensor
            {
                 res = L1API_ReadWriteRfRegisterResult(atc_p0,&reg_val);

                switch(res)
                {
                    case 0://read ok
                        sprintf((char *)g_rsp_str, "+RWRFREG: %d", reg_val);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        break;
                    case 1://read failed
                        sprintf((char *)g_rsp_str, "+RWRFREG: read temperature sensor failed");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    case 2://waiting dsp response
                        sprintf((char *)g_rsp_str, "+RWRFREG: read temperature sensor failed for waiting dsp response");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    default:
                        sprintf((char *)g_rsp_str, "+RWRFREG: read temperature sensor failed for L1 return %d.",res);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                }
            }
            else//other operation
            {
                 res = L1API_ReadWriteRfRegisterResult(atc_p0,&reg_val);

                switch(res)
                {
                    case 0://write ok
                        status = S_ATC_SUCCESS;
                        break;
                    case 1://write failed
                        sprintf((char *)g_rsp_str, "+RWRFREG: write register failed");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    case 2://waiting dsp response
                        sprintf((char *)g_rsp_str, "+RWRFREG: write register failed for waiting dsp response");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    default:
                        sprintf((char *)g_rsp_str, "+RWRFREG: write register failed for L1 return %d.",res);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                }
            }
        }
        else
        {
            sprintf((char *)g_rsp_str, "+RWRFREG: invalid parameter.");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            status = ERR_INVALID_INDEX;
        }

        break;

    default:
        status = ERR_OPERATION_NOT_SUPPORTED;
        break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SPUSIMTEST command
//  Global resource dependence :
//  Author:  jian.qin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPUSIMTEST)
{
    ATC_STATUS     status = S_ATC_SUCCESS;
    NVITEM_ERROR_E nv_result = NVERR_NONE;
    MULTIPS_CREATE_PARAM_T multips_param;

    nv_result = EFS_NvitemRead(NV_SIM_CFG, (uint16)sizeof(MULTIPS_CREATE_PARAM_T), (uint8*)&multips_param);//read nv maybe will last a long time

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
	     if(PARAM1_FLAG)
	     {
                if(1==(uint8)PARA1.num || 0==(uint8)PARA1.num)
                {
                     multips_param.reserve0 = (uint8)PARA1.num;
                     EFS_NvitemWrite(NV_SIM_CFG,sizeof(MULTIPS_CREATE_PARAM_T),(uint8*)&multips_param,FALSE);
		  }
		   else
		   {
		        status = ERR_OPERATION_NOT_SUPPORTED;
		   }

	     }
            break;
        }
        case ATC_CMD_TYPE_READ:
	     sprintf((char *)g_rsp_str,"%s: %d",g_atc_info_table[AT_CMD_SPUSIMTEST].cmd_name->str_ptr,multips_param.reserve0);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
	 case ATC_CMD_TYPE_TEST:
	     sprintf((char *)g_rsp_str,"%s: %s",g_atc_info_table[AT_CMD_SPUSIMTEST].cmd_name->str_ptr,"0,1");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
	 default:
	     status = ERR_OPERATION_NOT_SUPPORTED;
	     break;
    }

     return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+ETSRESET command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
//This at command used to factory reset (erase running NV) under the calibration mode.
/*****************************************************************************/
LOCAL void Reset_Delay_TimerHandler(uint32 param)
{
    POWER_Reset(); // added by bao for power reset
}

AT_CMD_FUNC(ATC_ProcessETSRESET)
{
    extern BOOLEAN EFS_mark_erase( void );

    SCI_TRACE_LOW("ATC: ATC_ProcessETSRESET");

    //factory reset, erase running NV
    EFS_mark_erase();

    SCI_CreateTimer ("Reset Delay Timer",\
                                   Reset_Delay_TimerHandler,\
                                   0, 
                                   1000,
                                   SCI_AUTO_ACTIVATE);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT^SPSIMLOCKDATAWRITE  PDU process func
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSimlockData(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *pdu_ptr  // pointer to the given pdu buffer
)
{
#ifdef DUALSIM_SIMLOCK_SUPPORT
    uint8       terminater_char;     // indicates ther terminater char
    uint32      pdu_len;          // holds the gived pdu length

    BOOLEAN result_flag = FALSE;

    SCI_TRACE_LOW("ATC: ATC_ProcessSimlockData len = %d", ( strlen((char *)pdu_ptr)));

    SCI_ASSERT((PNULL != atc_config_ptr) && (PNULL != pdu_ptr));          /*assert verified*/

    pdu_len = strlen((char *)pdu_ptr);

    terminater_char      = pdu_ptr[pdu_len - 1];
    pdu_ptr[pdu_len - 1] = '\0';
    pdu_len             -= 1;

    SCI_TRACE_LOW("ATC: ATC_ProcessSimlockData terminater_char = 0x%x", terminater_char);

    switch (terminater_char)
    {
        // cancel the operation
    case ESC:

        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        return S_ATC_DEFAULT_HANDLE;

    case CTRL_Z:

        result_flag = simlock_data_process((uint8*)pdu_ptr,pdu_len);
        break;

    default:
        break;
    }

    SCI_TRACE_LOW("ATC: ATC_ProcessSimlockData simlock_data_process result_flag = %d", result_flag);

    if (TRUE != result_flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR,0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK,0);
    }

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handle the AT^SPDATAENCRYPT  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPDATAENCRYPT)
{
#ifdef DUALSIM_SIMLOCK_SUPPORT

    ATC_STATUS status = S_ATC_SUCCESS;

    SCI_TRACE_LOW("ATC: ATC_Processxor_SPDATAENCRYPT");
    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if(PARAM1_FLAG&&PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING)
        {
            uint8* encrypt_key=NULL;
            encrypt_key = SCI_ALLOC_APPZ(2*sizeof(simlock_encrypt_keys_t)+1);
            if(NULL == encrypt_key)
            {
                SCI_TRACE_LOW("ATC: ATC_Processxor_SPDATAENCRYPT fail NULL == encrypt_key");
                status = S_ATC_FAIL;
                return;
            }

            status = simlock_encrypt_keys(PARA1.str_ptr->str_ptr,PARA1.str_ptr->str_len,(char*)encrypt_key, 2*sizeof(simlock_encrypt_keys_t)+1);

            sprintf((char*)g_rsp_str,"%s=%s", g_atc_info_table[AT_CMD_xor_SPDATAENCRYPT].cmd_name->str_ptr,encrypt_key);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

            if(encrypt_key)
            {
                SCI_FREE(encrypt_key);
            }
        }
        else
        {
            status = ERR_INVALID_INDEX;
        }
        break;
    case ATC_CMD_TYPE_READ:
        break;
    case ATC_CMD_TYPE_TEST:
        break;
    default:
        break;
    }
    return status;

#else
    return S_ATC_FAIL;
#endif
}


/*****************************************************************************/
//  Description : This function handle the AT^SPSIMLOCKDATAWRITE  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPSIMLOCKDATAWRITE )
{
#ifdef DUALSIM_SIMLOCK_SUPPORT

    ATC_STATUS status = S_ATC_SUCCESS;
    uint8             simlockdata_len = 0; // indicate the number of octets coded in the TP layer data unit to be given

    SCI_TRACE_LOW("ATC: ATC_Processxor_SPSIMLOCKDATAWRITE");

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if(PARAM1_FLAG&&PARAM1_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC)
        {

            ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_SIMLOCKDATA);
            status = S_ATC_DEFAULT_HANDLE;
        }
        else
        {
            status = ERR_INVALID_INDEX;
        }
        break;
    case ATC_CMD_TYPE_READ:
        break;
    case ATC_CMD_TYPE_TEST:
        break;
    default:
        break;
    }
    return status;

#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handle the AT+SPSIMLOCKCHECK  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPSIMLOCKCHECK)
{
#ifdef DUALSIM_SIMLOCK_SUPPORT
    ATC_STATUS  status = S_ATC_SUCCESS;
    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        status = ERR_OPERATION_NOT_ALLOWED;
        break;
    case ATC_CMD_TYPE_READ:
        sprintf((char *)g_rsp_str, "%s: %d",g_atc_info_table[AT_CMD_SPSIMLOCKCHECK].cmd_name->str_ptr,!MNPHONE_GetSimlockNvCheckResult());
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;
    default:
        status = ERR_INVALID_INDEX;
        break;
    }
    return status;

#else
    return S_ATC_FAIL;
#endif

}
/*****************************************************************************/
//  Description : This function handle the AT^SPSIMLOCKIMEI  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPSIMLOCKIMEI )
{
#ifdef DUALSIM_SIMLOCK_SUPPORT

    ATC_STATUS status = S_ATC_SUCCESS;

    SCI_TRACE_LOW("ATC: ATC_Processxor_SPSIMLOCKIMEI");

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if(PARAM1_FLAG&&PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING)
        {
            //status = simlock_imei_process((uint8*)PARA1.str_ptr->str_ptr,PARA1.str_ptr->str_len);
        }
        else
        {
            status = ERR_INVALID_INDEX;
        }
        break;
    case ATC_CMD_TYPE_READ:
        break;
    case ATC_CMD_TYPE_TEST:
        break;
    default:
        break;
    }
    return status;

#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handle the AT^SPHUK  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPHUK )
{
#ifdef DUALSIM_SIMLOCK_SUPPORT

    ATC_STATUS status = S_ATC_SUCCESS;

    SCI_TRACE_LOW("ATC: ATC_Processxor_SPHUK");

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if(PARAM1_FLAG&&PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING)
        {
            status = simlock_set_huk((uint8*)PARA1.str_ptr->str_ptr,PARA1.str_ptr->str_len);

            sprintf((char*)g_rsp_str,"%s=%d", g_atc_info_table[AT_CMD_xor_SPHUK].cmd_name->str_ptr,status);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

        }
        else
        {
            status = ERR_INVALID_INDEX;
        }
        break;
    case ATC_CMD_TYPE_READ:
        break;
    case ATC_CMD_TYPE_TEST:
        break;
    default:
        break;
    }
    return status;

#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handle the AT^SPFACAUTHPUBKEY  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPFACAUTHPUBKEY )
{
#ifdef DUALSIM_SIMLOCK_SUPPORT

    ATC_STATUS status = S_ATC_SUCCESS;

    SCI_TRACE_LOW("ATC: ATC_Processxor_SPHUK");

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if(PARAM1_FLAG&&PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING
					&&PARAM2_FLAG&&PARAM2_TYPE == ATC_CMD_PARAM_TYPE_STRING)
        {
            status = simlock_facauthpubkey((uint8*)PARA1.str_ptr->str_ptr,PARA1.str_ptr->str_len,
				    (uint8*)PARA2.str_ptr->str_ptr,PARA2.str_ptr->str_len);
            sprintf((char*)g_rsp_str,"%s=%d", g_atc_info_table[AT_CMD_xor_SPFACAUTHPUBKEY].cmd_name->str_ptr,status);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

        }
        else
        {
            status = ERR_INVALID_INDEX;
        }
        break;
    case ATC_CMD_TYPE_READ:
        break;
    case ATC_CMD_TYPE_TEST:
        break;
    default:
        break;
    }
    return status;

#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handle the AT^SPIDENTIFYSTART  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPIDENTIFYSTART )
{
#ifdef DUALSIM_SIMLOCK_SUPPORT

    ATC_STATUS status = S_ATC_SUCCESS;
    char data[SL_RSA_N_LEN+1]={0}; //must initialize to  "0"
    uint32 len = sizeof(data);

    SCI_TRACE_LOW("ATC: Processxor_SPIDENTIFYSTART");

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_EXECUTE:
        status = simlock_authstart(data,  &len);
        sprintf((char*)g_rsp_str,"%s=%s", g_atc_info_table[AT_CMD_xor_SPIDENTIFYSTART].cmd_name->str_ptr,data);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

        break;
    default:
        break;
    }
    return status;

#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handle the AT^SPIDENTIFYEND  command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_Processxor_SPIDENTIFYEND )
{
#ifdef DUALSIM_SIMLOCK_SUPPORT

    ATC_STATUS status = S_ATC_SUCCESS;

    SCI_TRACE_LOW("ATC: ATC_Processxor_SPIDENTIFYEND");

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if(PARAM1_FLAG&&PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING)
        {
            status = simlock_authend((uint8*)PARA1.str_ptr->str_ptr,PARA1.str_ptr->str_len);
            sprintf((char*)g_rsp_str,"%s=%d", g_atc_info_table[AT_CMD_xor_SPIDENTIFYEND].cmd_name->str_ptr,status);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            status = ERR_INVALID_INDEX;
        }
        break;
    case ATC_CMD_TYPE_READ:
        break;
    case ATC_CMD_TYPE_TEST:
        break;
    default:
        break;
    }
    return status;

#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description :convert the atc input string 12345678 to uint32 0x12345678
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
BOOLEAN ConvertStringtoUint32(uint8 *str_ptr, uint8 length, uint32 *num_ptr)
{
    uint8  i;
    uint32  numeric_value = 0;

    if((NULL == str_ptr) || (length < 1) || (length > 8))
    {
        return FALSE;
    }

    for(i = 0; i < length; ++i)
    {
        if(str_ptr[i] >= '0' && str_ptr[i] <= '9')
        {
            numeric_value = numeric_value * 16 + (int32)(str_ptr[i] - '0');
        }
        else if(str_ptr[i] >= 'a' && str_ptr[i] <= 'f')
        {
            numeric_value = numeric_value * 16 + (int32)(str_ptr[i] - 'a') + 0x0a;
        }
        else if(str_ptr[i] >= 'A' && str_ptr[i] <= 'F')
        {
            numeric_value = numeric_value * 16 + (int32)(str_ptr[i] - 'A') + 0x0a;
        }
        else
        {
            SCI_TRACE_LOW("ATC: ConvertStringtoUint32,invalid param, not numeric, return position:%d", i);
            return FALSE;
        }
    }

    SCI_TRACE_LOW("ATC: ConvertStringtoUint32,get the number: 0x%8x", numeric_value);

    *num_ptr = numeric_value;

    return TRUE;
}

/*****************************************************************************/
//  Description :convert the atc input bt address string to bt address
//  For example :atc input string 11:22:33:44:55:66,convert to bt address 0x112233445566
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
BOOLEAN ConvertStringtoBtAddr(uint8 *str_ptr, uint8 length, uint8 *addr)
{
    uint8  i=0,j=0;
    uint8  hex_value = 0;

    if((NULL == str_ptr) || (length != 17))
    {
        return FALSE;
    }

    for(i = 0; i < length; ++i)
    {
        hex_value = 0;
        if(str_ptr[i] >= '0' && str_ptr[i] <= '9')
        {
            hex_value = hex_value * 16 + (int32)(str_ptr[i] - '0');
        }
        else if(str_ptr[i] >= 'a' && str_ptr[i] <= 'f')
        {
            hex_value = hex_value * 16 + (int32)(str_ptr[i] - 'a') + 0x0a;
        }
        else if(str_ptr[i] >= 'A' && str_ptr[i] <= 'F')
        {
            hex_value = hex_value * 16 + (int32)(str_ptr[i] - 'A') + 0x0a;
        }
        else
        {
            SCI_TRACE_LOW("ATC: ConvertStringtoUint32,invalid param, not numeric, return position:%d", i);
            return FALSE;
        }

        i++;
        if(str_ptr[i] >= '0' && str_ptr[i] <= '9')
        {
            hex_value = hex_value * 16 + (int32)(str_ptr[i] - '0');
        }
        else if(str_ptr[i] >= 'a' && str_ptr[i] <= 'f')
        {
            hex_value = hex_value * 16 + (int32)(str_ptr[i] - 'a') + 0x0a;
        }
        else if(str_ptr[i] >= 'A' && str_ptr[i] <= 'F')
        {
            hex_value = hex_value * 16 + (int32)(str_ptr[i] - 'A') + 0x0a;
        }
        else
        {
            SCI_TRACE_LOW("ATC: ConvertStringtoUint32,invalid param, not numeric, return position:%d", i);
            return FALSE;
        }

        addr[j++] = hex_value;
        if(j==6)
        break;

        i++;
        if(str_ptr[i] != ':')
        {
            SCI_TRACE_LOW("ATC: ConvertStringtoUint32,invalid param, not numeric, return position:%d", i);
            return FALSE;
        }
    }

    SCI_TRACE_LOW("ATC: ConvertStringtoUint32,get the bt addr: 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x", addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);

    return TRUE;
}

/*****************************************************************************/
//  Description :convert the bt address to atc output string
//  For example :the bt address 0x112233445566,convert to atc output string 11:22:33:44:55:66
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
void ConvertBtAddrtoString(uint8 *addr,uint8 *str_ptr)
{
    sprintf((char *)str_ptr, "%02x%s%02x%s%02x%s%02x%s%02x%s%02x",addr[0],":",addr[1],":",addr[2],":",addr[3],":",addr[4],":",addr[5]);
}

/*****************************************************************************/
//  Description :convert the bt name double byte ascii 0030 to ascii 30
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint8* ConvertWstrToStr(
                                  const uint16* src,
                                  uint8* dst
                                  )
{
    uint8 *return_str = dst;

    if( src == PNULL || PNULL == dst )
    {
        return return_str;
    }

    while( !(*src & 0xFF00) && '\0' != (*dst++ = *src++) )
    {
        ;
    }

    return return_str;
}

/*****************************************************************************/
//  Description :convert the bt name ascii 30 to double byte ascii 0030
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16* ConvertStrToWstr(
                                  const uint8* src,
                                  uint16* dst
                                  )
{
    uint16 *return_wstr = dst;

    if( src == PNULL ||PNULL == dst)
    {
        return return_wstr;
    }

    while( '\0' != ( *dst++ = *src++ ) )    /* copy string */
    {
        ;
    }

    return return_wstr;
}

/*****************************************************************************/
//  Description :convert the bt name to string
//              bt name is encoded in UCS2,if it is double byte ascii,convert to ascii,else convert to string
//              for example: 1) UCS2 004D004F0043004F0052,convert to MOCOR
//                           2) UCS2 5F204E09,convert to string 5F204E09
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
void ConvertBtNametoString(uint16 *name,uint8 name_arr_len,uint8 *name_str_ptr,uint8 name_str_arr_len)
{
    uint8 i = 0, j = 0;
    uint8 temp_name[200];

    SCI_MEMSET(name_str_ptr,0,name_str_arr_len);

    if(name[0] < 0x0080)
    {
        //if the name is encode in 2 type ascii
        ConvertWstrToStr(name,name_str_ptr);
    }
    else
    {
        //if the name is encode in UCS2
        for(i = 0; i < name_arr_len; i++)
        {
            if(name[i] != '\0')
            {
                j++;
            }
            else
            {
                break;
            }
        }

        if(j == 0)
        {
            name_str_ptr[0] = '\0';
        }
        else
        {
            for(i = 0; i< j; i++)
			{
				temp_name[2*i] =  name[i] >> 8;
				temp_name[2*i + 1] =  name[i] & 0xff;
			}
            ConvertBinToHex(temp_name,j*2,name_str_ptr);
        }
    }
}

/*****************************************************************************/
//  Description :convert the string to bt name
//              bt name is encoded in UCS2.
//              if the atc input bt name is ascii,convert to double byte ascii(UCS2);
//              if the atc input bt name is UCS2 string,convert to UCS2
//              for example: 1) atc input bt name is MOCOR,convert to UCS2 004D004F0043004F0052
//                           2) atc input bt name is string 5F204E09,convert to UCS2 5F204E09
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
void ConvertStringtoBtName(uint8 type,uint8 *str,uint8 str_len,uint16 *name,uint8 name_arr_count)
{
    uint8 temp_name[200] = {0};
    uint8 i=0,j=0;

    SCI_MEMSET(name,0,name_arr_count*2);

    if(type == ATC_BT_NAME_ASCII)
    {
        //transfer uint8 ascii to ucs2
        ConvertStrToWstr(str,name);
    }
    else
    {
        //transfer hex string to bin
        ConvertHexToBin(str, str_len, temp_name);

        //copy uint8 to uint16
        for(i = 0; i< (name_arr_count - 1);i++)
        {
            if(i > (sizeof(temp_name)- 2))
            break;

            if((temp_name[i] == 0x00) &&
               (temp_name[i+1] == 0x00)) //lint !e661
            {
                break;
            }
            name[j] = temp_name[i++];
            name[j] = (name[j] << 8) | temp_name[i];
            j++;
        }

        name[j] = '\0';
    }
}

/*****************************************************************************/
//  Description : This function handles the AT+SPBTCTRL command
//  Global resource dependence : g_rsp_str
//  Author:       yongxia.zhang
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPBTCTRL)
{
    ATC_STATUS           status = S_ATC_SUCCESS;//S_ATC_DEFAULT_HANDLE;
#ifndef WIN32
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
    ATC_SPBTTEST_PARAM_T      param[9] ={0};
    uint8                 *temp_ptr;
    uint8                 param_len;
    uint8                 cmd_type;
    uint32                type_oper;
    const char    *s_btctrl_str[33] =
    {
        "ONOFF","VISIBLE","NAME","ADDR","INQ","INQC","PAIR","PAIRC","PIN","LISTPD","REMOVEPD","CONNECT","ADV","ADVDATA","CONPARA","LINKNUM","SLVAUTH","DEBUG","TEST","HFU","A2DPSNK","AVRCPCT","HID","SPP",
        "GATSREG","GATSS","GATSC","GATSD","GATSST","GATSIND","GATSRSP","GATSDISC","HOGP"
    };
    int                   bttest_index;
    BOOLEAN         quiry_flag = FALSE;
    BT_STATUS       ret_val = BT_SUCCESS;//lint !e529

    SCI_MEMSET(param, 0, (9 * sizeof(ATC_SPBTTEST_PARAM_T)));
    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    temp_ptr  = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    ATC_GetCmdType(temp_ptr, (uint16)param_len, &cmd_type);

    switch (cmd_type)
    {
    case ATC_CMD_TYPE_SET:
        temp_ptr++;
        param_len--;
        if (ATC_SPBTTEST_GetParams(temp_ptr, param_len, param))
        {
            if ('?' == param[0].param[param[0].param_len-1])
            {
                quiry_flag = TRUE;
                param[0].param[param[0].param_len-1] = '\0';
            }
            else
            {
                param[0].param[param[0].param_len] = '\0';
            }

            for (bttest_index=0; bttest_index<32; bttest_index++)
            {
                if (!strcmp((char *)param[0].param, s_btctrl_str[bttest_index]))
                {
                    break;
                }
            }

            SCI_TRACE_LOW("ATC: SPBTCTRL,param_len is %d, 0 param_len is %d, index is %d",param_len,param[0].param_len,bttest_index);

            switch (bttest_index)
            {
                case 0:   //"ONOFF"
                {
                    if (quiry_flag)
                    {
                        SCI_TRACE_LOW("ATC: SPBTCTRL,get bt state");
                        //+SPBTCTRL:ONOFF=<state>
                        sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTCTRL:",s_btctrl_str[bttest_index],"=", BT_GetState());
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;//OK
                    }
                    else if (param[1].param_len != 0)
                    {
                        type_oper = param[1].param[0] - '0';

                        if (0 == type_oper) // bt stop
                        {
                            SCI_TRACE_LOW("ATC: SPBTCTRL,stop bt");
                            if(BT_GetState())
                            {
                                ret_val = BT_Stop();
                                if(BT_PENDING == ret_val)
                                {
                                    return S_ATC_DEFAULT_HANDLE;
                                }
                                else
                                {
                                    return ERR_OPERATION_NOT_ALLOWED;
                                }
                            }
                        }
                        else if (1 == type_oper) // bt start
                        {
                            SCI_TRACE_LOW("ATC: SPBTCTRL,start bt");
                            if(!BT_GetState())
                            {
                                ret_val = BT_Start();
                                if(BT_PENDING == ret_val)
                                {
                                    return S_ATC_DEFAULT_HANDLE;
                                }
                                else
                                {
                                    return ERR_OPERATION_NOT_ALLOWED;
                                }
                            }
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: SPBTCTRL,ONOFF param error,param=%d",type_oper);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    break;//lint !e527
                }

                case 1:   //"VISIBLE"
                {
                    if (quiry_flag)
                    {
                        uint32      visible_info = 0;
                        BOOLEAN     is_visible = FALSE;

                        if(BT_GetState())
                        {
                            visible_info = BT_GetVisibility();
                            SCI_TRACE_LOW("ATC: SPBTCTRL,visible status is = %d",visible_info);

                            if ((BT_INQUIRY_ENABLE|BT_PAGE_ENABLE) == visible_info)
                            {
                                // Only when device is inquiry and page enable
                                is_visible = TRUE;
                            }
                            else
                            {
                                is_visible = FALSE;
                            }
                        }
                        else
                        {
                            is_visible = FALSE;
                        }

                        //+SPBTCTRL:VISIBLE=<visible>
                        sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTCTRL:",s_btctrl_str[bttest_index],"=", is_visible);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;//OK
                    }
                    else if (param[1].param_len != 0)
                    {
                        type_oper = param[1].param[0] - '0';

                        if (0 == type_oper) // set hidden
                        {
                            SCI_TRACE_LOW("ATC: SPBTCTRL,set hidden");
                            ret_val = BT_SetVisibility(BT_PAGE_ENABLE);//set blue tooth hidden
                            if ( BT_PENDING == ret_val)
                            {
                                return S_ATC_DEFAULT_HANDLE;
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else if (1 == type_oper) // set visible
                        {
                            SCI_TRACE_LOW("ATC: SPBTCTRL,set visible");
                            ret_val = BT_SetVisibility(BT_INQUIRY_ENABLE | BT_PAGE_ENABLE);//set blue tooth visible
                            if (BT_PENDING == ret_val)
                            {
                                return S_ATC_DEFAULT_HANDLE;
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: SPBTCTRL,VISIBLE param error,param=%d",type_oper);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    break;//lint !e527
                }

                case 2:   //"NAME"
                {
                    if (quiry_flag)
                    {
                        uint16  device_name[BT_DEVICE_NAME_SIZE] = {0};
                        uint8   name_string[ATC_MAX_BT_NAME_STRING_LEN] = {0};

                        SCI_TRACE_LOW("ATC: SPBTCTRL,get bt name");
                        BT_GetLocalName(device_name);
                        ConvertBtNametoString(device_name,BT_DEVICE_NAME_SIZE, name_string,sizeof(name_string));
                        //+SPBTCTRL:NAME=<name>
                        sprintf((char *)g_rsp_str, "%s%s%s%s","+SPBTCTRL:",s_btctrl_str[bttest_index],"=",name_string);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;//OK
                    }
                    else if(param[1].param_len != 0)
                    {
                        uint8 name_type = ATC_BT_NAME_ASCII;
                        uint16 local_name[BT_DEVICE_NAME_SIZE] = {0};

                        if(param[2].param_len != 0)
                        {
                            if(param[2].param[0] == '1')
                            {
                                name_type = ATC_BT_NAME_UCS2;
                            }
                        }

                        ConvertStringtoBtName(name_type,param[1].param,param[1].param_len,local_name,BT_DEVICE_NAME_SIZE);
                        ret_val = BT_SetLocalName(local_name);
                        if (BT_SUCCESS == ret_val)
                        {
                            return S_ATC_SUCCESS;//OK
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: call BT_SetLocalName ret_val = %d.",ret_val);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    break;//lint !e527
                }

                case 3:   //"ADDR"
                {
                    if (quiry_flag)
                    {
                        BT_ADDRESS addr;
                        uint8 addr_str[ATC_MAX_BT_ADDRESS_STRING_LEN] = {0};
                        BT_GetBdAddr(&addr);
                        ConvertBtAddrtoString(addr.addr,addr_str);

                        //+SPBTCTRL:ADDR=<address>
                        sprintf((char *)g_rsp_str, "%s%s%s%s","+SPBTCTRL:",s_btctrl_str[bttest_index],"=",addr_str);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;//OK
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    break;//lint !e527
                }

                case 4:   //"INQ"
                {
                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        uint32 service_type = BT_SERVICE_ALL;

                        if(param[1].param_len != 0)
                        {
                            if(!ConvertStringtoUint32(param[1].param,param[1].param_len,&service_type))
                            {
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }

                        ret_val = BT_SearchDevice(service_type);
                        if (BT_PENDING == ret_val)
                        {
                            //OK
                            //+INQS
                            sprintf((char *)g_rsp_str, "%s","OK");
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                            sprintf((char *)g_rsp_str, "%s","+INQS");
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                            return S_ATC_DEFAULT_HANDLE;
                        }
                        else
                        {
                            //ERROR
                            SCI_TRACE_LOW("ATC: call BT_SearchDevice ret_val = %d.",ret_val);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    break;//lint !e527
                }

                case 5:   //"INQC"
                {
                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        ret_val = BT_CancelSearch();

                        if (BT_PENDING == ret_val)
                        {
                            return S_ATC_DEFAULT_HANDLE;
                        }
                        else if(BT_SUCCESS == ret_val)
                        {
                            return S_ATC_SUCCESS;//OK
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: call BT_CancelSearch ret_val = %d.",ret_val);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    break;//lint !e527;
                }

                case 6:   //"PAIR"
                {
                    if (quiry_flag)
                    {
                        SCI_TRACE_LOW("ATC: call PAIR ERROR quiry_flag=%d.",quiry_flag);
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        BT_ADDRESS addr = {0};

                        if(param[1].param_len != 0)
                        {
                            if(!ConvertStringtoBtAddr(param[1].param,param[1].param_len,addr.addr))
                            {
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: call PAIR ERROR param[1].param_len== 0.");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                        ret_val = BT_PairDevice(&addr);

                        if (BT_PENDING == ret_val)
                        {
                            return S_ATC_SUCCESS;//OK
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: call BT_PairDevice ret_val = %d.",ret_val);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    break;//lint !e527
                }

                case 7:   //"PAIRC"
                {
                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        BT_ADDRESS addr = {0};

                        if(param[1].param_len != 0)
                        {
                            if(!ConvertStringtoBtAddr(param[1].param,param[1].param_len,addr.addr))
                            {
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                        ret_val = BT_CancelPair(&addr);

                        if ((BT_PENDING == ret_val) || (BT_SUCCESS == ret_val) )
                        {
                            return S_ATC_SUCCESS;//OK
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: call BT_CancelPair ret_val = %d.",ret_val);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    break;//lint !e527
                }

                case 8:   //"PIN"
                {
                    if (quiry_flag)
                    {
                        SCI_TRACE_LOW("ATC: call PIN ERROR quiry_flag=%d.",quiry_flag);
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        BT_ADDRESS addr = {0};
                        //AT+SPBTCTRL=PIN,<addr>,<pin>
                        //get addr
                        if(param[1].param_len != 0)
                        {
                            if(!ConvertStringtoBtAddr(param[1].param,param[1].param_len,addr.addr))
                            {
                                SCI_TRACE_LOW("ATC: call PIN ConvertStringtoBtAddr ERROR.");
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: call PIN param[1].param_len==0 ERROR");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                        //get pin
                        if(param[2].param_len != 0)
                        {
                            ret_val = BT_InputPairPin(&addr, param[2].param,param[2].param_len);

                            if (BT_PENDING == ret_val)
                            {
                                return S_ATC_DEFAULT_HANDLE;
                            }
                            else
                            {
                                SCI_TRACE_LOW("ATC: call BT_InputPairPin ret_val = %d.",ret_val);
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    break;//lint !e527
                }

                case 9:   //"LISTPD"
                {
                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        int   device_number =0;
                        uint8 index =0;
                        uint8 addr_string[ATC_MAX_BT_ADDRESS_STRING_LEN] = {0};
                        uint8 name_string[ATC_MAX_BT_NAME_STRING_LEN] = {0};
                        BT_DEVICE_INFO  dev = {0};

                        device_number = BT_GetPairedDeviceCount(BT_SERVICE_ALL);

                        for (index = 1; index <= device_number; index++)
                        {
                            BT_GetPairedDeviceInfo(BT_SERVICE_ALL, index, &dev);
                            //+LISTPD:11:22:33:44:55:66,devName1,00000408
                            ConvertBtAddrtoString(dev.addr.addr,addr_string);
                            ConvertBtNametoString(dev.name,BT_DEVICE_NAME_SIZE, name_string,sizeof(name_string));
                            sprintf((char *)g_rsp_str, "%s%s%s%s%s%08x","+LISTPD:",addr_string,",",name_string,",",dev.dev_class);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }

                        //+LISTPDE
                        sprintf((char *)g_rsp_str, "+LISTPDE");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;//OK
                    }
                    break;//lint !e527
                }

                case 10:   //"REMOVEPD"
                {
                    //AT+SPBTCTRL=REMOVEPD,<cmd_type>,<address>
                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        if(param[1].param_len != 0)
                        {
                            if(param[1].param[0] == '0')//clear one paired device record
                            {
                                if(param[2].param_len != 0)//address
                                {
                                    BT_ADDRESS addr = {0};

                                    ConvertStringtoBtAddr(param[2].param,param[2].param_len,addr.addr);
                                    ret_val = BT_RemovePairedDevice(&addr);

                                    if(BT_PENDING != ret_val)
                                    {
                                        SCI_TRACE_LOW("ATC: call BT_RemovePairedDevice ret_val = %d.",ret_val);
                                        return ERR_OPERATION_NOT_ALLOWED;
                                    }

                                    return S_ATC_SUCCESS;//OK
                                }
                                else
                                {
                                    return ERR_OPERATION_NOT_ALLOWED;
                                }
                            }
                            else if(param[1].param[0] == '1')//clear all the paired device records
                            {
                        		int   device_number =0;
                                uint8 index =0;
                                BT_DEVICE_INFO  dev = {0};

                                device_number = BT_GetPairedDeviceCount(BT_SERVICE_ALL);

                                for (index = 1; index <= device_number; index++)
                                {
                                    BT_GetPairedDeviceInfo(BT_SERVICE_ALL, index, &dev);
                                    BT_RemovePairedDevice(&(dev.addr));
                                }

                                return S_ATC_SUCCESS;//OK
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
                    }
                    break;//lint !e527
                }

                case 11:   //"CONNECT"
                {
                    //AT+SPBTCTRL=CONNECT,<address>,<profile>
                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        if(param[1].param_len != 0)
                        {
                            BT_ADDRESS addr = {0};
                            uint8 connect_flag = 0;

                            //get bt address
                            if(!ConvertStringtoBtAddr(param[1].param,param[1].param_len,addr.addr))
                            {
                                return ERR_OPERATION_NOT_ALLOWED;
                            }

                            //profile name
                            if(param[2].param_len != 0)
                            {
                                if (!strcmp((char *)param[2].param, "SPP"))
                                {
#ifdef BT_SPP_SUPPORT
                                    if (param[3].param_len != 0)
                                    {
                                        connect_flag = param[3].param[0] - '0';

                                        if (connect_flag)
                                        {
                                            ret_val = BT_SppConnect(&addr, SPP_PORT_SPEED_115200);
                                        }
                                        else
                                       {
                                           ret_val = BT_SppDisconnect();
                                       }

                                        if((ret_val == BT_PENDING)|| (ret_val == BT_SUCCESS))
                                        {
                                            //OK
                                            //+CONNECTING>>11:22:33:44:55:66
                                            sprintf((char *)g_rsp_str, "%s","OK");
                                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                                        sprintf((char *)g_rsp_str, "%s%s","+CONNECTING>>",param[1].param);
                                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                                            if(ret_val == BT_SUCCESS)
                                            {
                                                if (connect_flag)
						      {
                                                    sprintf((char *)g_rsp_str,"+CONNNECTED");
                                                }
                                                else
                                                {
                                                    sprintf((char *)g_rsp_str,"+DISCONNECTED");
                                                }
                                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                            }
                                            return S_ATC_DEFAULT_HANDLE;
                                        }
                                        else
                                        {
                                            SCI_TRACE_LOW("ATC: call BT_SppConnect ret_val = %d.",ret_val);
                                            return ERR_OPERATION_NOT_ALLOWED;
                                        }
                                    }
#else
                                   SCI_TRACE_LOW("ATC: call BT_SppConnect ERR_OPERATION_NOT_ALLOWED.");
                                   return ERR_OPERATION_NOT_ALLOWED;
#endif
                                }
                                else if (!strcmp((char *)param[2].param, "HID"))
                                {
#ifdef BT_HID_SUPPORT
                                    if (param[3].param_len != 0)
                                    {
                                        connect_flag = param[3].param[0] - '0';

                                        if (connect_flag)
                                        {
/**/
                                            if ((0x40 == addr.addr[0]) && (0x45 == addr.addr[1]) && (0xDA == addr.addr[2]))
                                            {
SCI_TRACE_LOW("ATC: YWD change the addr");
                                                addr.addr[0] = addr.addr[5];
                                                addr.addr[1] = addr.addr[4];
                                                addr.addr[2] = addr.addr[3];
                                                addr.addr[3] = 0xDA;
                                                addr.addr[4] = 0x45;
                                                addr.addr[5] = 0x40;
                                            }
/**/
                                            ret_val = HID_Connect(addr);
                                        }
                                        else
                                       {
                                           ret_val = HID_Disconnect();
                                       }

SCI_TRACE_LOW("BTYWD AT HID=0x%x",ret_val);

                                        if ((ret_val == BT_PENDING) || (ret_val == BT_SUCCESS))
                                        {
                                            //OK
                                            //+CONNECTING>>11:22:33:44:55:66
                                            sprintf((char *)g_rsp_str, "%s","OK");
                                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                                            sprintf((char *)g_rsp_str, "%s%s","+CONNECTING>>",param[1].param);
                                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                                            if(ret_val == BT_SUCCESS)
                                            {
                                                if (connect_flag)
                                                {
                                                    sprintf((char *)g_rsp_str,"+CONNECTED");
                                                }
                                                else
                                                {
                                                    sprintf((char *)g_rsp_str,"+DISCONNECTED");
                                                }
                                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                            }
                                            return S_ATC_DEFAULT_HANDLE;
                                        }
                                        else
                                        {
                                            SCI_TRACE_LOW("ATC: BT HID ret_val = %d.",ret_val);
                                            return ERR_OPERATION_NOT_ALLOWED;
                                        }
                                   }
#else
                                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                                }
                                else if (!strcmp((char *)param[2].param, "HFU"))
                                {
#ifdef BT_HFU_SUPPORT
                                    if (param[3].param_len != 0)
                                    {
                                        connect_flag = param[3].param[0] - '0';

                                        if (connect_flag)
                                        {
                                            ret_val = BT_HfuConnect(&addr);
                                        }
                                        else
                                       {
                                           ret_val = BT_HfuDisconnect();
                                       }

                                        if ((ret_val == BT_PENDING)|| (ret_val == BT_SUCCESS))
                                        {
                                            //OK
                                            //+CONNECTING>>11:22:33:44:55:66
                                            sprintf((char *)g_rsp_str, "%s","OK");
                                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                                            sprintf((char *)g_rsp_str, "%s%s","+CONNECTING>>",param[1].param);
                                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                                            if(ret_val == BT_SUCCESS)
                                            {
                                                if (connect_flag)
                                                {
                                                    sprintf((char *)g_rsp_str,"+CONNNECTED");
                                                }
                                                else
                                                {
                                                    sprintf((char *)g_rsp_str,"+DISCONNECTED");
                                                }
                                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                            }
                                            return S_ATC_DEFAULT_HANDLE;
                                        }
                                        else
                                        {
                                            SCI_TRACE_LOW("ATC: BT HFU ret_val = %d.",ret_val);
                                            return ERR_OPERATION_NOT_ALLOWED;
                                        }
                                    }
#else
                                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                                }
                                else if (!strcmp((char *)param[2].param, "A2DPSNK"))
                                {
#ifdef BT_AV_SNK_SUPPORT
                                    if (param[3].param_len != 0)
                                    {
                                        connect_flag = param[3].param[0] - '0';

                                        if (connect_flag)
                                        {
                                            ret_val = A2DP_Sink_Connect(addr);
                                        }
                                        else
                                       {
                                           ret_val = A2DP_Sink_Disconnect();
                                       }

                                        if ((ret_val == BT_PENDING)|| (ret_val == BT_SUCCESS))
                                        {
                                            //OK
                                            //+CONNECTING>>11:22:33:44:55:66
                                            sprintf((char *)g_rsp_str, "%s","OK");
                                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                                            sprintf((char *)g_rsp_str, "%s%s","+CONNECTING>>",param[1].param);
                                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                                            if(ret_val == BT_SUCCESS)
                                            {
                                                if (connect_flag)
                                                {
                                                    sprintf((char *)g_rsp_str,"+CONNNECTED");
                                                }
                                                else
                                                {
                                                    sprintf((char *)g_rsp_str,"+DISCONNECTED");
                                                }
                                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                            }
                                            return S_ATC_DEFAULT_HANDLE;
                                        }
                                        else
                                        {
                                            SCI_TRACE_LOW("ATC: BT A2DP SNK ret_val = %d.",ret_val);
                                            return ERR_OPERATION_NOT_ALLOWED;
                                        }
                                    }
#else
                                    return ERR_OPERATION_NOT_ALLOWED;
#endif
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
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }

                    break;//lint !e527
                }
                case 12://ADV
		  {
#if defined(BLE_INCLUDED) || defined(BT_BLE_SUPPORT)

                    BOOLEAN adv_flag = FALSE;
                    BOOLEAN adv_interval_flag = FALSE;
                    BOOLEAN adv_type_flag = FALSE;
                    BOOLEAN adv_own_addr_type_flag = FALSE;
                    BOOLEAN adv_dir_addr_type_flag = FALSE;
                    BOOLEAN adv_dir_addr_flag = FALSE;
                    BOOLEAN adv_channel_flag = FALSE;
                    BOOLEAN  adv_policy_flag = FALSE;
                    tBLE_ADV_TYPE adv_type = 0;
                    uint16 interval = 0;
                    tBLE_ADDR_TYPE local_addr_type = 0;
                    tBLE_ADDR_TYPE peer_addr_type = 0;
                    bdaddr_t peer_addr = {0};
                    tBLE_ADV_CHNL_MAP channel_map = 0;
                    uint8 filter_policy = 0;
                    //BLE_STATUS status = BLE_SUCCESS;

                    SCI_TRACE_LOW("BT ATC YWD %d %d",param[1].param_len,param[2].param_len);
                    if (!BT_GetState())
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }

                    if (quiry_flag)
                    {
                        adv_flag = ble_adv_is_enable();
                        if (adv_flag)
                        {
                            ble_get_adv_param(&adv_type,&interval,&local_addr_type,&peer_addr_type,&peer_addr);
                            channel_map = ble_get_gap_adv_channel_map();  //lint !e718  !e746
                            filter_policy = ble_get_adv_filter_policy();    //lint !e718  !e746

                            sprintf((char *)g_rsp_str, "%s%s%s%d%s%d%s%d%s%d%s%d%s%x%s%x%s%x%s%x%s%x%s%x%s%d%s%d","+SPBTCTRL:",s_btctrl_str[bttest_index],"=",\
							adv_flag,",",interval,",",adv_type,",",local_addr_type,",",peer_addr_type,",",\
							peer_addr.address[0],":",peer_addr.address[1],":",peer_addr.address[2],":",peer_addr.address[3],":",peer_addr.address[4],":",peer_addr.address[5],",",\
							channel_map,",",filter_policy);
                        }
                        else
                        {
                            sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTCTRL:",s_btctrl_str[bttest_index],"=",adv_flag);
                        }
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;
                    }
                    else
                    {
                        if(param[1].param_len != 0)
			   {
			       if (1 == (param[1].param[0]- '0'))
                            {
                                adv_flag = TRUE;
                                if(param[2].param_len != 0)
                                {
                                    adv_interval_flag = TRUE;
                                }
                                if(param[3].param_len != 0)
                                {
                                    adv_type_flag = TRUE;
                                }
                                if(param[4].param_len != 0)
                                {
                                    adv_own_addr_type_flag = TRUE;
                                }
                                if(param[5].param_len != 0)
                                {
                                    adv_dir_addr_type_flag = TRUE;
                                }
                                if(param[6].param_len != 0)
                                {
                                    adv_dir_addr_flag = TRUE;
                                }
                                if(param[7].param_len != 0)
                                {
                                    adv_channel_flag = TRUE;
                                }
                                if(param[8].param_len != 0)
                                {
                                    adv_policy_flag = TRUE;
                                }
                            }
                        }

                        if (adv_flag)
                        {
                            if (adv_interval_flag && adv_type_flag && adv_own_addr_type_flag && adv_dir_addr_type_flag && adv_dir_addr_flag)
                            {
                                //all should be TRUE here
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_ALLOWED;
                            }

                            /*lint -save -e527 -e774 */
                            if (adv_interval_flag)
                            {
                                interval = atoi((char *)param[2].param);
                            }

                            if (adv_type_flag)
                            {
                                adv_type = atoi((char *)param[3].param);
                            }

                            if (adv_own_addr_type_flag)
                            {
                                local_addr_type = atoi((char *)param[4].param);
                            }

                            if (adv_dir_addr_type_flag)
                            {
                                peer_addr_type = atoi((char *)param[5].param);
                            }

                            if (adv_dir_addr_flag)
                            {
                                uint8 tmp_addr[12] = {0};
                                int i = 0;
                                int j = 0;

                                for(i=0; i<param[6].param_len; i++)
                                {
                                    if(param[6].param[i] != ':')
                                    {
                                        if(param[6].param[i]>= 'A' && param[6].param[i]<='F')
                                        {
                                           tmp_addr[j] = (param[6].param[i] - '0' - 7);
                                        }
                                        else
                                        {
                                            tmp_addr[j] = (param[6].param[i] - '0');
                                        }

                                        //SCI_TRACE_LOW("tmp_addr=%x", tmp_addr[j]);
                                        ++j;
                                    }
                                }

                                for(j=0; j<6; j++)
                                {
                                    //sprintf(tmp_addr_2[j], "%x", (tmp_addr[j*2]<<4 | tmp_addr[j*2+1]));
                                    peer_addr.address[j] = (tmp_addr[j*2])<<4 | (tmp_addr[j*2+1]);
                                }
                            }

                            status = ble_set_adv_param(adv_type,interval,local_addr_type,peer_addr_type,&peer_addr);
                            if (BLE_SUCCESS == status)//some parameters are invalid
                            {
                                if (adv_channel_flag)
                                {
                                    channel_map = atoi((char *)param[7].param);
                                    ble_set_gap_adv_channel_map(channel_map); //lint !e718  !e746
                                }

                                if (adv_policy_flag)
                                {
                                    filter_policy = atoi((char *)param[8].param);
                                    ble_set_adv_filter_policy(filter_policy);  //lint !e718  !e746
                                }

                                ble_adv_enable(FALSE);//Disable adv before new parameters running

                                ble_adv_enable(TRUE);//Enable ADV with new parameters
                            }
                            else
                            {
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else//Disable ADV
                        {
                            ble_adv_enable(FALSE);
                        }

                        return S_ATC_SUCCESS;
                    }
                    break;//lint !e527
#else
                    SCI_TRACE_LOW("ATC: SPBTCTRL ADV ERR_OPERATION_NOT_ALLOWED.");
                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                    }

                case 13://ADVDATA
                {
#if defined(BLE_INCLUDED) || defined(BT_BLE_SUPPORT)

                    BOOLEAN adv_data_flag = FALSE;
                    uint8 data_buf[40] = {0};
                    uint8 data_len= 0;

                    if (!BT_GetState())
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }

                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        if(param[1].param_len != 0)
			   {
                             adv_data_flag = TRUE;
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                        if (adv_data_flag)  //lint !e774
                        {
                            //copy data
                            data_len = param[1].param_len;
                            if(param[1].param_len >= 40)
                            {
                                data_len = 40;
                            }
                            SCI_MEMCPY(data_buf, param[1].param, data_len);
                            SCI_TRACE_LOW("ATC: ADV data_len=%d, data_buf %s", data_len, data_buf);
                            ble_set_adv_manu_data(data_buf, data_len);  //lint !e718  !e746
                        }

                       return S_ATC_SUCCESS;
                    }
                    break;//lint !e527
#else
                    SCI_TRACE_LOW("ATC: SPBTCTRL ADVDATA ERR_OPERATION_NOT_ALLOWED.");
                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                }

                 case 14://CONPARA  AT+SPBTCTRL=CONPARA,uint16,uint16,uint16,uint16
                {
#if defined(BLE_INCLUDED) || defined(BT_BLE_SUPPORT)

                    BOOLEAN param1_flag = FALSE;
                    uint16 param1 = 0;
                    BOOLEAN param2_flag = FALSE;
                    uint16 param2 = 0;
                    BOOLEAN param3_flag = FALSE;
                    uint16 param3 = 0;
                    BOOLEAN param4_flag = FALSE;
                    uint16 param4 = 0;
                    conn_param_req_t current_para = {0};

                    if (!BT_GetState())
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }

                    if (quiry_flag)
                    {
                        ble_get_conn_param(&current_para);  //lint !e718  !e746
                        //+SPBTCTRL:CONPARA=<%d>,<%d>,<%d>,<%d>,
                        sprintf((char *)g_rsp_str, "%s%s%s%d,%d,%d","+SPBTCTRL:",s_btctrl_str[bttest_index],"=", current_para.conn_interval,current_para.conn_latency,current_para.timeout);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;//OK
                    }
                    else
                    {
                        if(param[1].param_len != 0)
			   {
			        param1_flag = TRUE;
                        }

                        if(param[2].param_len != 0)
			   {
			        param2_flag = TRUE;
                        }

                        if(param[3].param_len != 0)
			   {
			        param3_flag = TRUE;
                        }

                        if(param[4].param_len != 0)
			   {
			        param4_flag = TRUE;
                        }

                        if (param1_flag)
                        {
                            param1 = (uint16)atoi((char *)param[1].param);
                            SCI_TRACE_LOW("ATC: CONPARA param1=%d",param1);
                        }

                        if (param2_flag)
                        {
                            param2 = (uint16)atoi((char *)param[2].param);
                            SCI_TRACE_LOW("ATC: CONPARA param2=%d",param2);
                        }

                        if (param3_flag)
                        {
                            param3 = (uint16)atoi((char *)param[3].param);
                            SCI_TRACE_LOW("ATC: CONPARA param3=%d",param3);
                        }

                        if (param4_flag)
                        {
                            param4 = (uint16)atoi((char *)param[4].param);
                            SCI_TRACE_LOW("ATC: CONPARA param4=%d",param4);
                        }

                        status = S_ATC_FAIL;
                        if (param1_flag && param2_flag && param3_flag && param4_flag)
                        {
                            if (ble_l2cap_conn_param_update_req(param1,param2,param3,param4))
                            {
                                status = S_ATC_SUCCESS;
                            }
                        }
                        return status;
                    }
                    break;//lint !e527
#else
                    SCI_TRACE_LOW("ATC: SPBTCTRL CONPARA ERR_OPERATION_NOT_ALLOWED.");
                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                }

                case 15://LINKNUM
                {
#if defined(BLE_INCLUDED) || defined(BT_BLE_SUPPORT)

                    uint8 link_num = 0;
                    BOOLEAN ret = FALSE;

                    if (quiry_flag)
                    {
                        //+SPBTCTRL:LINKNUM=<%d>
                        //sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTCTRL:",s_btctrl_str[bttest_index],"=", link_num);
                        //ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        //return S_ATC_SUCCESS;//OK
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        if(param[1].param_len != 0)
                        {
                            link_num = (uint16)atoi((char *)param[1].param);
                            ret = ble_set_link_num(link_num); //lint !e718  !e746
                        }

                        if (ret)
                        {
                            return S_ATC_SUCCESS;
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }

                    break;//lint !e527
#else
                    SCI_TRACE_LOW("ATC: SPBTCTRL LINKNUM ERR_OPERATION_NOT_ALLOWED.");
                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                }

                case 16://SLVAUTH
                {
#if defined(BLE_INCLUDED) || defined(BT_BLE_SUPPORT)

                    if (quiry_flag)
                    {
                        //ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        //return S_ATC_SUCCESS;//OK
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        ble_slave_send_authen_req(); //lint !e718  !e746
                        return S_ATC_SUCCESS;
                    }

                    break;//lint !e527
#else
                    SCI_TRACE_LOW("ATC: SPBTCTRL SLVAUTH ERR_OPERATION_NOT_ALLOWED.");
                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                }

                case 17://DEBUG
                {

                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        if(param[1].param_len != 0)
			   {
			       if (1 == (param[1].param[0] - '0'))
                            {

                            }
                            sprintf((char *)g_rsp_str, "param[1]:%d", param[1].param[0] - '0');
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }


                        if(param[2].param_len != 0)
			   {
			       if (1 == (param[2].param[0] - '0'))
                            {

                            }
                            sprintf((char *)g_rsp_str, "param[2]:%d", param[2].param[0] - '0');
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }

                        if(param[3].param_len != 0)
			   {
			       if (1 == (param[3].param[0] - '0'))
                            {

                            }
                            sprintf((char *)g_rsp_str, "param[3]:%d", param[3].param[0] - '0');
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }

                        if(param[4].param_len != 0)
			   {
			       if (1 == (param[4].param[0] - '0'))
                            {

                            }
                            sprintf((char *)g_rsp_str, "param[4]:%d", param[4].param[0] - '0');
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }

                        if(param[5].param_len != 0)
			   {
			       if (1 == (param[5].param[0] - '0'))
                            {

                            }
                            sprintf((char *)g_rsp_str, "param[5]:%d", param[5].param[0] - '0');
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }

                        if(param[6].param_len != 0)
			   {
			       if (1 == (param[6].param[0]- '0'))
                            {

                            }
                            sprintf((char *)g_rsp_str, "param[6]:%d", param[6].param[0]- '0');
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }

                       return S_ATC_SUCCESS;
                    }
                    break;//lint !e527
                }

                case 18://TEST
                {
#ifdef CHIP_VER_EX_SL6210C
                    const char *s_bt_simu_str[BT_SIMU_MAX] = {"NONE","KPD","SPEAKER"};
                    BT_SIMU_MODE_E simu_mode = BT_SIMU_NONE;
                    BOOLEAN flag = FALSE;
                    if (quiry_flag)
                    {
                        sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBTCTRL:",s_btctrl_str[bttest_index],"=", BT_ATC_GetTestMode(&simu_mode));
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;//OK
                    }
                    else
                    {
                        if (param[1].param_len != 0)
                        {
                            if (1 == (param[1].param[0] - '0'))
                            {
                                flag = TRUE;
                                if (param[2].param_len != 0)
                                {
                                    for (bttest_index=0; bttest_index<BT_SIMU_MAX; bttest_index++)
                                    {
                                        if (!strcmp((char *)param[2].param, s_bt_simu_str[bttest_index]))
                                        {
                                            break;
                                        }
                                    }
                                    if (bttest_index < BT_SIMU_MAX)
                                    {
                                        simu_mode = (BT_SIMU_MODE_E)bttest_index;
                                    }
                                }
                                else
                                {
                                    simu_mode = BT_SIMU_KPD;//KPD as default, to compitile
                                }
                            }

                            BT_ATC_SetTestMode(flag,simu_mode);//We should set the simu mode before the BT ON

                            sprintf((char *)g_rsp_str, "TEST:%d", param[1].param[0] - '0');
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }
                        return S_ATC_SUCCESS;
                    }
#else
                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                    break;//lint !e527
                }

                case 19:   //"HFU"
                {
#ifdef CHIP_VER_EX_SL6210C
                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
#ifdef BT_HFU_SUPPORT
                        ATC_SPBTCTRL_HFU_PROCESS(atc_config_ptr,&param[1]);
                        return S_ATC_SUCCESS;
#else
                        return ERR_OPERATION_NOT_ALLOWED;
#endif
                    }
#else
                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                }
                break;

                case 20:   //"A2DPSNK"
                {
#ifdef CHIP_VER_EX_SL6210C
                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        if (param[1].param_len != 0)
                        {
                            type_oper = param[1].param[0] - '0';
                        }

                        switch(type_oper)
                        {
                            case 0:
                            {
                            }
                            break;

                            case 1:
                            {
                            }
                            break;

                            default:
                            break;
                        }
                        return S_ATC_SUCCESS;
                    }
#else
                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                }
                break;

                case 21:   //"AVRCP CT"
                {
#if defined(CHIP_VER_EX_SL6210C) && defined(BT_AV_CT_SUPPORT)
                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        if (param[1].param_len != 0)
                        {
                            AvrcpCtPanelOperation Op_Avrcp = {0};
                            BOOLEAN isAutoRelease = TRUE;
                            uint32 avrcpct_ctrl_time = 0;
                            /* op  0x0041	volume_up --- VOLUP
                             *	   0x0042	volume_down --- VOLDOWN
                             *	   0x0043	mute --- MUTE
                             *	   0x0044	play --- PLAY
                             *	   0x0045	stop
                             *	   0x0046	pause
                             *	   0x0047	record
                             *	   0x0048	rewind
                             *	   0x0049	fast forward
                             *	   0x004a	eject
                             *	   0x004b	forward
                             *	   0x004c	backward
                             */
                            if (('V' == param[1].param[0]) && ('O' == param[1].param[1]) && ('L' == param[1].param[2]) && ('U' == param[1].param[3]) && ('P' == param[1].param[4]))
                            {
                                type_oper = 0x41;
                            }
                            else if (('V' == param[1].param[0]) && ('O' == param[1].param[1]) && ('L' == param[1].param[2]) && ('D' == param[1].param[3]) && ('O' == param[1].param[4]) && ('W' == param[1].param[5]) && ('N' == param[1].param[6]))
                            {
                                type_oper = 0x42;
                            }
                            else if (('M' == param[1].param[0]) && ('U' == param[1].param[1]) && ('T' == param[1].param[2]) && ('E' == param[1].param[3]))
                            {
                                type_oper = 0x43;
                            }
                            else if (('P' == param[1].param[0]) && ('L' == param[1].param[1]) && ('A' == param[1].param[2]) && ('Y' == param[1].param[3]))
                            {
                                type_oper = 0x44;
                            }
                            else if (('S' == param[1].param[0]) && ('T' == param[1].param[1]) && ('O' == param[1].param[2]) && ('P' == param[1].param[3]))
                            {
                                type_oper = 0x45;
                            }
                            else if (('P' == param[1].param[0]) && ('A' == param[1].param[1]) && ('U' == param[1].param[2]) && ('S' == param[1].param[3]) && ('E' == param[1].param[4]))
                            {
                                type_oper = 0x46;
                            }
                            else if (('R' == param[1].param[0]) && ('E' == param[1].param[1]) && ('C' == param[1].param[2]) && ('O' == param[1].param[3]) && ('R' == param[1].param[4]) && ('D' == param[1].param[5]))
                            {
                                type_oper = 0x47;
                            }
                            else if (('R' == param[1].param[0]) && ('E' == param[1].param[1]) && ('W' == param[1].param[2]) && ('I' == param[1].param[3]) && ('N' == param[1].param[4]) && ('D' == param[1].param[5]))
                            {
                                type_oper = 0x48;
                            }
                            else if (('F' == param[1].param[0]) && ('A' == param[1].param[1]) && ('S' == param[1].param[2]) && ('T' == param[1].param[3]) && ('F' == param[1].param[4]) && ('W' == param[1].param[5]) && ('D' == param[1].param[6]))
                            {
                                type_oper = 0x49;
                            }
                            else if (('E' == param[1].param[0]) && ('J' == param[1].param[1]) && ('E' == param[1].param[2]) && ('C' == param[1].param[3]) && ('T' == param[1].param[4]))
                            {
                                type_oper = 0x4A;
                            }
                            else if (('F' == param[1].param[0]) && ('O' == param[1].param[1]) && ('R' == param[1].param[2]) && ('W' == param[1].param[3]) && ('A' == param[1].param[4]) && ('R' == param[1].param[5]) && ('D' == param[1].param[6]))
                            {
                                type_oper = 0x4B;
                            }
                            else if (('B' == param[1].param[0]) && ('A' == param[1].param[1]) && ('C' == param[1].param[2]) && ('K' == param[1].param[3]) && ('W' == param[1].param[4]) && ('A' == param[1].param[5]) && ('R' == param[1].param[6]) && ('D' == param[1].param[7]))
                            {
                                type_oper = 0x4C;
                            }

                            Op_Avrcp.Op = (uint16)type_oper;
                            if((0x49 == type_oper) ||(0x48 == type_oper))
                            {
                                Op_Avrcp.press = TRUE;
                                ConvertStringtoDecimal(param[2].param,strlen(param[2].param),&avrcpct_ctrl_time);
                                SCI_TRACE_LOW("yiheng.zhou_avrcpct_ctrl_time:%d",avrcpct_ctrl_time);
                                AVRCP_CT_FASTFWD_OR_REWIND_PanelKey(Op_Avrcp, avrcpct_ctrl_time);//push fastfwd/rewind

                            }
                            else
                            {
                                Op_Avrcp.press = TRUE;
                                isAutoRelease = TRUE;
                                AVRCP_CT_SetPanelKey(Op_Avrcp,isAutoRelease);
                            }

                        }

                        return S_ATC_SUCCESS;
                    }
#else
                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                }
                break;

                case 22:   //"HID"
                {
#if defined (CHIP_VER_EX_SL6210C) && defined (BT_HID_SUPPORT)
                    uint32 key_val = 0;

                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        //SCI_TRACE_LOW("BTYWD HID Key %d . 0x%x 0x%x 0x%x ... %d",param[1].param_len,param[1].param[0],param[1].param[1],param[1].param[2],param[2].param_len);
                        if (('K' == param[1].param[0]) && ('E' == param[1].param[1]) && ('Y' == param[1].param[2]))
                        {
                            type_oper = 0;
                        }

                        switch(type_oper)
                        {
                            case 0:/*input Key*/
                            {
                                if (param[2].param_len != 0)
                                {
                                    //if ((param[3].param_len != 0))
                                    {
                                        if (ConvertStringtoDecimal(param[2].param,param[2].param_len,&key_val))
                                        {
                                            //SCI_TRACE_LOW("BTYWD HID Key 222222 key value=%d",key_val);
                                            BT_HIDSendBTKey(KEY_PRESSED,key_val,TRUE);
                                            SCI_SLEEP(10);
                                            BT_HIDSendBTKey(KEY_RELEASED,key_val,TRUE);
                                        }
                                        else
                                        {
                                            return ERR_OPERATION_NOT_ALLOWED;
                                        }
                                    }

                                }
                            }
                            break;

                            default:
                            break;
                        }
                        return S_ATC_SUCCESS;
                    }
#else
                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                }
                break;

                case 23:   //"SPP"
                {
#ifdef CHIP_VER_EX_SL6210C
                    if (quiry_flag)
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    else
                    {
                        if (param[1].param_len != 0)
                        {
                            type_oper = param[1].param[0] - '0';
                        }

                        switch(type_oper)
                        {
                            case 0:
                            {

                            }
                            break;

                            case 1:
                            {

                            }
                            break;

                            default:
                            break;
                        }

                        return S_ATC_SUCCESS;
                    }
#else
                    return ERR_OPERATION_NOT_ALLOWED;
#endif
                }
                break;

                case 24://GATSREG   Register/Deregister a GATT Server
                {
                    uint8 op = 0;
                    uint8 gserv_id[32] = {0};
                    uint8 gserv_id_len = 0;
                    uint32 server_handle = 0;
                    BOOLEAN result = FALSE;

                    if (quiry_flag)
                    {

                        //AT+SPBTCTRL=GATSREG?
                        //+SPBTCTRL:GATSREG=<server_handle>,<server_uuid>,<result>
                        //OK

                        //BT function
                        app_read_register_gatt_server_id();

                        return S_ATC_DEFAULT_HANDLE;
                    }
                    else
                    {
                        //register server
                        //AT+SPBTCTRL=GATSREG,1,<server_uuid>
                        //+SPBTCTRL:GATSREG=1,<server_handle>,<server_uuid>,<result>
                        //OK

                        //degister Server
                       //AT+SPBTCTRL=GATSREG,0,<server_handle>
                        //+SPBTCTRL:GATSREG=0,<server_handle>,<server_uuid>,<result>
                        //OK

                        if(param[1].param_len != 0)
			   {
                            op = param[1].param[0] - '0';

                            if((op != 0) && (op !=1))
                            {
                                SCI_TRACE_LOW("ATC: AT+SPBTCTRL=GATSREG, op=%d param error", op);
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }

                        //BT function
                        if(1== op)
                        {
                            if(param[2].param_len != 0)
                            {
                                if(param[2].param_len >= 32)
                                {
                                    param[2].param_len = 32;
                                }
                                //string "ABCD" to 0xABCD
                                ConvertHexToBin(param[2].param, param[2].param_len, gserv_id);
                                gserv_id_len= param[2].param_len/2;
                            }

                             result = app_register_gatt_server(gserv_id, gserv_id_len);
                        }
                        else
                        {
                            if(param[2].param_len != 0)
                            {
                                server_handle = atoi((char *)param[2].param);
                            }

                            result = app_deregister_gatt_server(server_handle);
                        }

                        if(TRUE == result)
                        {
                            return S_ATC_DEFAULT_HANDLE;
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s, result=%d",s_btctrl_str[bttest_index],result);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                    }
                    break;//lint !e527
                }

                case 25://GATSS    Add/Remove a Service
                {
                    uint8 op = 0;
                    uint8 service_uuid[32] = {0};
                    uint8 service_uuid_len = 0;
                    uint16 handle_num = 0;
                    uint16 service_handle = 0;
                    uint8 is_primary = 0;
                    uint32 server_handle = 0;
                    BOOLEAN result = FALSE;

                    if (quiry_flag)
                    {

                        //AT+SPBTCTRL=GATSS?
                        //+SPBTCTRL:GATSS=<server_handle>,<service_uuid>,<service_handle>,<service_handle_number>,<result>
                        //OK

                        //BT function
                        app_read_user_service();
                        return S_ATC_DEFAULT_HANDLE;
                    }
                    else
                    {
                        //Add a service
                        //AT+SPBTCTRL=GATSS,1,<server_handle>,<service_uuid>,<service_handle_number>,<is_primary>
                        //+SPBTCTRL:GATSS=1,<server_handle>,<service_uuid>,<service_handle>,<service_handle_number>,<result>
                        //OK

                        //remove a service
                        //AT+SPBTCTRL=GATSS,0,<service_handle>
                        //+SPBTCTRL:GATSS=0,<server_handle>,<service_uuid>,<service_handle>,<result>
                        //OK

                        if(param[1].param_len != 0)
			   {
                            op = param[1].param[0] - '0';

                            if((op != 0) && (op !=1))
                            {
                                SCI_TRACE_LOW("ATC: AT+SPBTCTRL=GATSS, op=%d paamer error", op);
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                        if(1 == op)
                        {
                            if(param[2].param_len != 0)
                            {
                                server_handle = atoi((char *)param[2].param);
                            }

                            if(param[3].param_len != 0)
                            {
                                if(param[3].param_len >= 32)
                                {
                                    param[3].param_len = 32;
                                }
                                ConvertHexToBin(param[3].param, param[3].param_len, service_uuid);
                                service_uuid_len = param[3].param_len / 2;

                            }

                            if(param[4].param_len != 0)
                            {
                                handle_num = atoi((char *)param[4].param);
                            }

                            if(param[5].param_len != 0)
                            {
                                is_primary = atoi((char *)param[5].param);
                            }

                           //BT function
                             result = app_add_user_service(server_handle, service_uuid, service_uuid_len, handle_num, is_primary);

                        }
                        else
                        {

                            if(param[2].param_len != 0)
                            {
                                service_handle = atoi((char *)param[2].param);
                            }

                            //BT function
                             result = app_remove_user_service(service_handle);

                        }

                        if(TRUE == result)
                        {
                            return S_ATC_DEFAULT_HANDLE;
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s, result=%d",s_btctrl_str[bttest_index],result);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                    }
                    break;//lint !e527
                }

                case 26://GATSC   Add a Characteristic to an Existing Service
                {
                    uint16 service_handle = 0;
                    uint8 char_uuid[32] = {0};
                    uint8 char_uuid_len = 0;
                    uint32 property = 0;
                    uint8 permission = 0;
                    BOOLEAN result = FALSE;

                    if (quiry_flag)
                    {
                        SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s  not support read command", s_btctrl_str[bttest_index]);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                    else
                    {

                        //AT+SPBTCTRL=GATSC,<service_handle>,<char_uuid>,<property>,<permission>
                        //+SPBTCTRL:GATSC=1,<server_handle>,<service_handle>,<char_uuid>,<char_handle>,<result>
                        //OK

                        if(param[1].param_len != 0)
                        {
                            service_handle = atoi((char *)param[1].param);
                        }

                        if(param[2].param_len != 0)
                        {
                            if(param[2].param_len >= 32)
                            {
                                param[2].param_len = 32;
                            }
                            ConvertHexToBin(param[2].param, param[2].param_len, char_uuid);
                            char_uuid_len = param[2].param_len / 2;

                        }


                        if(param[3].param_len != 0)
                        {
                            property = atoi((char *)param[3].param);
                        }

                        if(param[4].param_len != 0)
                        {
                            permission = atoi((char *)param[4].param);
                        }

                        //BT function
                         result = app_add_user_character(service_handle, char_uuid, char_uuid_len, property, permission);

                        if(TRUE == result)
                        {
                            return S_ATC_DEFAULT_HANDLE;
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s, result=%d",s_btctrl_str[bttest_index],result);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    break;//lint !e527
                }

                case 27://GATSD  AT+SPBTCTRL=GATSD  Add a Descriptor to an Existing Service
                {
                    uint16 service_handle = 0;
                    uint8 desc_uuid[32] = {0};
                    uint8 desc_uuid_len = 0;
                    uint8 permission = 0;
                    BOOLEAN result = FALSE;

                    if (quiry_flag)
                    {
                        SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s  not support read command", s_btctrl_str[bttest_index]);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                    else
                    {

                        //AT+SPBTCTRL=GATSD,<service_handle>,<desc_uuid>,<permission>
                        //+SPBTCTRL:GATSC=1,<server_handle>,<service_handle>,<desc_uuid>,<desc_handle>,<result>
                        //OK

                        if(param[1].param_len != 0)
                        {
                            service_handle = atoi((char *)param[1].param);
                        }

                        if(param[2].param_len != 0)
                        {
                            if(param[2].param_len >= 32)
                            {
                                param[2].param_len = 32;
                            }
                            ConvertHexToBin(param[2].param, param[2].param_len, desc_uuid);
                            desc_uuid_len = param[2].param_len / 2;

                        }

                        if(param[3].param_len != 0)
                        {
                            permission = atoi((char *)param[3].param);
                        }

                        //BT function
                         result = app_add_user_discriptor(service_handle, desc_uuid, desc_uuid_len, permission);

                        if(TRUE == result)
                        {
                            return S_ATC_DEFAULT_HANDLE;
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s, result=%d",s_btctrl_str[bttest_index],result);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    break;//lint !e527
                }

                case 28://GATSST  Start/Stop a Service
                {
                    uint8 op = 0;
                    uint16 service_handle = 0;
                    uint8 transport = 0;
                    BOOLEAN result = FALSE;

                    if (quiry_flag)
                    {
                        SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s  not support read command", s_btctrl_str[bttest_index]);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                    else
                    {
                        //start a service
                        //AT+SPBTCTRL=GATSST,1,<service_handle>,<transport>
                        //+SPBTCTRL:GATSST=1,<server_handle>,<service_uuid>,<service_handle>,<result>
                        //OK

                        //stop a service
                        //AT+SPBTCTRL=GATSST,0,<service_handle>
                        //+SPBTCTRL:GATSST=0,<server_handle>,<service_uuid>,<service_handle>,<result>
                        //OK

                        if(param[1].param_len != 0)
			   {
                            op = param[1].param[0] - '0';

                            if((op != 0) && (op !=1))
                            {
                                SCI_TRACE_LOW("ATC: AT+SPBTCTRL=GATSS, op=%d paamer error", op);
                                return ERR_OPERATION_NOT_ALLOWED;
                            }
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                        if(param[2].param_len != 0)
                        {
                            service_handle = atoi((char *)param[2].param);
                        }

                        if(param[3].param_len != 0)
                        {
                            transport = atoi((char *)param[3].param);
                        }

                        //BT function
                        if(1 == op)
                        {
                            result = app_start_user_service(service_handle, transport);
                        }
                        else
                        {
                            result = app_stop_user_service(service_handle);
                        }

                        if(TRUE == result)
                        {
                            return S_ATC_DEFAULT_HANDLE;
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s, result=%d",s_btctrl_str[bttest_index],result);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    break;//lint !e527
                }

                case 29://GATSIND    Send an Indication or Notification to a Client
                {

                    uint32  conn_id = 0;
                    uint16 attr_handle = 0;
                    uint8 need_confirm = 0;
                    BOOLEAN result = FALSE;
                    uint8 ind_value[32] = {0};
                    uint8 ind_value_len = 0;

                    if (quiry_flag)
                    {
                        SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s  not support read command", s_btctrl_str[bttest_index]);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                    else
                    {

                        //AT+SPBTCTRL=GATSIND,<conn_id>,<attr_handle>,<need_confirm>,<ind_value>
                        //+SPBTCTRL:GATSIND=<attr_handle>,<conn_id>,<result>
                        //OK

                        if(param[1].param_len != 0)
                        {
                            conn_id = atoi((char *)param[1].param);
                        }

                        if(param[2].param_len != 0)
                        {
                            attr_handle = atoi((char *)param[2].param);
                        }

                        if(param[3].param_len != 0)
                        {
                            need_confirm = atoi((char *)param[3].param);
                        }

                        if(param[4].param_len != 0)
                        {
                            if(param[4].param_len >= 32)
                            {
                                param[4].param_len = 32;
                            }
                            ConvertHexToBin(param[4].param, param[4].param_len, ind_value);
                            ind_value_len = param[4].param_len / 2;
                        }

                        //BT function
                         result = app_send_indi_or_noti(conn_id, attr_handle, need_confirm, ind_value, ind_value_len);

                        if(TRUE == result)
                        {
                            return S_ATC_DEFAULT_HANDLE;
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s, result=%d",s_btctrl_str[bttest_index],result);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                    }
                    break;//lint !e527
                }

                case 30://GATSRSP    Send a Response to a Client’s Read or Write Operation
                {

                    uint32  conn_id = 0;
                    uint32 trans_id = 0;
                    uint16 attr_handle = 0;
                    uint8 rsp_value[32] = {0};
                    uint8 rsp_value_len = 0;
                    BOOLEAN result = FALSE;

                    if (quiry_flag)
                    {
                        SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s  not support read command", s_btctrl_str[bttest_index]);
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                    else
                    {
                        //AT+SPBTCTRL=GATSRSP,<conn_id>,<trans_id>,<attr_handle>,<rsp_value>
                        //+SPBTCTRL:GATSRSP=<conn_id>,<att_handle>,<result>
                        //OK

                        if(param[1].param_len != 0)
                        {
                            conn_id = atoi((char *)param[1].param);
                        }

                        if(param[2].param_len != 0)
                        {
                            trans_id = atoi((char *)param[2].param);
                        }

                        if(param[3].param_len != 0)
                        {
                            attr_handle = atoi((char *)param[3].param);
                        }

                        if(param[4].param_len != 0)
                        {
                            if(param[4].param_len >= 32)
                            {
                                param[4].param_len = 32;
                            }
                            ConvertHexToBin(param[4].param, param[4].param_len, rsp_value);
                            rsp_value_len = param[4].param_len / 2;
                        }

                        //BT function
                        result = app_send_response(conn_id, trans_id, attr_handle, rsp_value, rsp_value_len);

                       if(TRUE == result)
                        {
                            return S_ATC_DEFAULT_HANDLE;
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s, result=%d",s_btctrl_str[bttest_index],result);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                    }
                    break;//lint !e527
                }

                case 31://GATSDISC    Disconnect Server Actively
                {
                    uint32  conn_id = 0;
                    uint16 attr_handle = 0;
                    BOOLEAN result = FALSE;

                    if (quiry_flag)
                    {
                        //AT+SPBTCTRL=GATSDISC?
                        //+SPBTCTRL:GATSDISC=<conn_id>,<remote_addr>,<result>
                        //OK

                        //BT function
                        app_read_user_conn_id();
                        return S_ATC_DEFAULT_HANDLE;
                    }
                    else
                    {

                        //AT+SPBTCTRL=GATSDISC,<conn_id>
                        //+SPBTCTRL:GATSDISC=<conn_id>,<result>
                        //OK

                        if(param[1].param_len != 0)
                        {
                            conn_id = atoi((char *)param[1].param);
                        }

                        //BT function
                        result = app_disconnect_gatt_server(conn_id);

                        if(TRUE == result)
                        {
                            return S_ATC_SUCCESS;
                        }
                        else
                        {
                            SCI_TRACE_LOW("ATC: AT+SPBTCTRL=%s, result=%d",s_btctrl_str[bttest_index],result);
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                    }
                    break;//lint !e527
                }

                case 32: //Hogp
                {
#ifdef BT_BLE_HOGP_SUPPORT
                    ATC_SPBTCTRL_HOGP_PROCESS(atc_config_ptr,&param[1]);
                    break;
#endif
                }

                default:   //Error input para
                    return ERR_OPERATION_NOT_ALLOWED;
            }
        }
        else
        {
            SCI_TRACE_LOW("ATC: ATC_SPBTTEST_GetParams ERROR");
            return ERR_OPERATION_NOT_ALLOWED;
        }
        break;//lint !e527

    case ATC_CMD_TYPE_READ:
    case ATC_CMD_TYPE_TEST:
    default:
        return ERR_OPERATION_NOT_ALLOWED;
    }

#endif
#endif
    return status;//lint !e527
}//lint !e529

/*****************************************************************************/
//  Description : This function handles the AT+SPBLETEST command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPBLETEST)
{
    ATC_STATUS           status = S_ATC_SUCCESS;//S_ATC_DEFAULT_HANDLE;
#ifndef WIN32
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
//#if defined(BLE_INCLUDED) || defined(BT_BLE_SUPPORT)

    ATC_SPBTTEST_PARAM_T      param[4];
    uint8                 *temp_ptr;
    uint8                 param_len;
    uint8                 cmd_type;
    uint32                type_oper = 0;
    const char    *s_bletest_str[14] =
    {
        "TESTMODE","TESTADDRESS","TXCH","TXPATTERN","TXPKTTYPE","TXPKTLEN","TXPWR","TX","RXCH","RXPATTERN","RXPKTTYPE","RXGAIN","RX","RXDATA"
    };
    int                   bletest_index;
    BOOLEAN         quiry_flag = FALSE;
#ifdef BT_NONSIG_SUPPORT
    BT_NONSIG_PARAM spbletest_bt_non_param = {0};
#endif
    static uint8       spbletest_bt_address[6] = {0};
    static uint8       spbletest_bt_tx_ch = 0;
    static uint8       spbletest_bt_tx_pattern = 1;
    static uint8       spbletest_bt_tx_pkttype = 1;
    static uint16     spbletest_bt_tx_pktlen = 0;
    static uint8       spbletest_bt_tx_pwr_type = 0;// 0---gain value; 1---power level
    static uint8       spbletest_bt_tx_pwr_value = 0;
    static uint8       spbletest_bt_tx_mode = 0;
    static uint8       spbletest_bt_tx_status = 0;
    static uint8       spbletest_bt_rx_ch = 0;
    static uint8       spbletest_bt_rx_pattern = 0;
    static uint8       spbletest_bt_rx_pktype = 1;
    static uint8       spbletest_bt_rx_gain_mode = 0;
    static uint8       spbletest_bt_rx_gain_value = 0;
    static uint8       spbletest_bt_rx_status = 0;
    static uint8       spbletest_bt_create_flag = 0;
    BOOLEAN         spbletest_bt_status = FALSE;

    SCI_MEMSET(param, 0, (3 * sizeof(ATC_SPBTTEST_PARAM_T)));
    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    temp_ptr  = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    ATC_GetCmdType(temp_ptr, (uint16)param_len, &cmd_type);

    switch (cmd_type)
    {
    case ATC_CMD_TYPE_SET:
        temp_ptr++;
        param_len--;
        if (ATC_SPBTTEST_GetParams(temp_ptr, param_len, param))
        {
            if ('?' == param[0].param[param[0].param_len-1])
            {
                quiry_flag = TRUE;
                param[0].param[param[0].param_len-1] = '\0';
            }
            else
            {
                param[0].param[param[0].param_len] = '\0';
            }

            for (bletest_index=0; bletest_index<14; bletest_index++)
            {
                if (!strcmp((char *)param[0].param, s_bletest_str[bletest_index]))
                {
                    break;
                }
            }

            SCI_TRACE_LOW("SPBLETEST: param_len is %d, 0 param_len is %d, index is %d",param_len,param[0].param_len,bletest_index);

            switch (bletest_index)
            {
            case 0:   //"TESTMODE"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get testmode");
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=", BT_GetTestMode()); //lint !e718 !e746
                }
                else if (param[1].param_len != 0)
                {

                     if (CALIBRATION_MODE == POWER_GetResetMode())
                     {
                         if (0 == spbletest_bt_create_flag)
                         {
                             BT_Init(NULL);//create the BT thread
                             spbletest_bt_create_flag = 1;
                             SCI_SLEEP(400);
                         }
                     }

                    type_oper = param[1].param[0] - '0';

                    if (0 == type_oper) // quit EUT mode
                    {
                        SCI_TRACE_LOW("SPBLETEST: quit TEST");
                        UART_SetControllerBqbMode(FALSE);  //lint !e718  !e746
                        BT_Stop();

#ifdef BT_NONSIG_SUPPORT
                        SCI_MEMSET(&spbletest_bt_non_param,0,sizeof(BT_NONSIG_PARAM));
#endif
                        SCI_MEMSET(spbletest_bt_address,0,6);
                        spbletest_bt_tx_ch = 0;
                        spbletest_bt_tx_pattern = 1;
                        spbletest_bt_tx_pkttype = 1;
                        spbletest_bt_tx_pktlen = 0;
                        spbletest_bt_tx_pwr_type = 0;
                        spbletest_bt_tx_pwr_value = 0;
                        spbletest_bt_tx_mode = 0;
                        spbletest_bt_tx_status = 0;
                        spbletest_bt_rx_ch = 0;
                        spbletest_bt_rx_pattern = 0;
                        spbletest_bt_rx_pktype = 1;
                        spbletest_bt_rx_gain_mode = 0;
                        spbletest_bt_rx_gain_value = 0;
                        spbletest_bt_rx_status = 0;
                        BT_SetTestMode(BT_TESTMODE_NONE); //lint !e718 !e746
                    }
                    else if (1 == type_oper) // enter EUT mode
                    {
                        SCI_TRACE_LOW("SPBLETEST: enter EUT");
                        BT_Start();
                        SCI_SLEEP(1000);//wait BT init
                        //ble_adv_enable(FALSE);
                        SCI_SLEEP(100);

                        UART_SetControllerBqbMode(TRUE);

                        BT_SetTestMode(BT_TESTMODE_SIG);
                    }
                    else if (2 == type_oper) // enter nonSignal mode
                    {
                        SCI_TRACE_LOW("SPBLETEST: start Non Sig Test");
                        BT_Start();
                        SCI_SLEEP(1000);//wait BT init
                        //ble_adv_enable(FALSE);
                        SCI_SLEEP(100);

                        BT_SetTestMode(BT_TESTMODE_NONSIG);
                    }
                    sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                }

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 1:   //"TESTADDRESS"
            {
                if (quiry_flag)
                {
/*//wuding: the code coming from 7701, it looks strange
                    uint8 addr[12] = {0};
                    uint8 i = 0;
                    SCI_TRACE_LOW("SPBLETEST: get TESTADDRESS");
                    //here call the low layer API to get TESTADDRESS
                    //BT_TEST_Get_BdAddr(addr);
                    for(i=0; i<6;i++)
                        SCI_TRACE_LOW("get bt addr:%x", addr[i]);
                    sprintf((char *)g_rsp_str, "%s%s%s%s","+SPBLETEST:",s_bletest_str[bletest_index],"=", "11:22:33:44:55:66");
*/
                    sprintf((char *)g_rsp_str, "%s%s%s%x%x%x%x%x%x","+SPBLETEST:",s_bletest_str[bletest_index],"=",spbletest_bt_address[0],spbletest_bt_address[1],spbletest_bt_address[2],spbletest_bt_address[3],spbletest_bt_address[4],spbletest_bt_address[5]);
                }
                else if (param[1].param_len != 0)
                {
                    uint8 tmp_addr[12] = {0};
                    //uint8 addr_tmp[12] = {0};
                    int i = 0;
                    int j = 0;
                    //here call the low layer API to set TESTADDRESS
                    SCI_TRACE_LOW("testing bt addr: %s, len=%d", (param[1].param), param[1].param_len);
                    for(i=0; i<param[1].param_len; i++)
                    {
                        if(param[1].param[i] != ':')
                        {
                            if(param[1].param[i]>= 'A' && param[1].param[i]<='F')
                            {
                                tmp_addr[j] = (param[1].param[i] - '0' - 7);
                            }
                            else
                            {
                                tmp_addr[j] = (param[1].param[i] - '0');
                            }
                            //sprintf(tmp_addr+j, "%x", (param[1].param[i] - '0'));
                            SCI_TRACE_LOW("tmp_addr=%x", tmp_addr[j]);
                            ++j;
                        }
                    }

                    for(j=0; j<6; j++)
                    {
                        //sprintf(tmp_addr_2[j], "%x", (tmp_addr[j*2]<<4 | tmp_addr[j*2+1]));
                        spbletest_bt_address[j] = (tmp_addr[j*2])<<4 | (tmp_addr[j*2+1]);
                        SCI_TRACE_LOW("set bt addr: %x", spbletest_bt_address[j]);
                    }

                    //BT_TEST_Set_BdAddr(tmp_addr_2);
                    // param[1].param_len & param[1].param are the TESTADDRESS which user inputed, such as 11:22:33:44:55:66
                    sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 2:   //"TXCH"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get TXCH");
                    //here call the low layer API to get TXCH
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=",spbletest_bt_tx_ch);
                }
                else if (param[1].param_len != 0)
                {
                    if (param[1].param_len == 1)  // valid type_oper range [0~39]
                    {
                        type_oper = param[1].param[0] - '0';
                    }
                    else if (param[1].param_len == 2)
                    {
                        type_oper = param[1].param[0] - '0';
                        type_oper = type_oper * 10 + (param[1].param[1] - '0');
                    }
                    spbletest_bt_tx_ch = (uint8)type_oper;
                    //here call the low layer API to set TXCH, type_oper is the input x
                    sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 3:   //"TXPATTERN"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get TXPATTERN");
                    //here call the low layer API to get TXPATTERN
                   sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=",spbletest_bt_tx_pattern);
                }
                else if (param[1].param_len != 0)
                {
                    type_oper = param[1].param[0] - '0';

                    //here call the low layer API to set TXPATTERN, type_oper is the input x, range [0~7]
                    //wuding: according to the NPI BT AT cmd doc
                    //PN9        ---0
                    //11110000---1
                    //1010       ---2
                    //PN15       ---3
                    //1111       ---4
                    //0000       ---5
                    //00001111---6
                    //0101       ---7

                    spbletest_bt_tx_pattern = (uint8)type_oper;
                    sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 4:   //"TXPKTTYPE"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get TXPKTTYPE");
                    //here call the low layer API to get TXPKTTYPE
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=", spbletest_bt_tx_pkttype);
                }
                else if (param[1].param_len != 0)
                {
                    //type_oper = param[1].param[0] - '0';
                    uint8 i = 0;
                    for (type_oper=0,i=0;i<param[1].param_len;i++)
                    {
                        type_oper = type_oper * 10 + (param[1].param[i] - '0');
                    }

                    //wuding: according to the NPI BT AT cmd doc
                    //0x00---RF_PHY_Test_Ref,   0x01---ADV_NONCONN_IND,   0x02---ADV_IND,   0x03---DATA,


                    spbletest_bt_tx_pkttype = (uint8)type_oper;

                    //here call the low layer API to set TXPKTTYPE, type_oper is the input x, range [0~3]
                    sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 5:   //"TXPKTLEN"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get TXPKTLEN");
                    //here call the low layer API to get TXPKTLEN
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=", spbletest_bt_tx_pktlen);
                }
                else if (param[1].param_len != 0)
                {
                    int i=0;
                    for (type_oper=0,i=0;i<param[1].param_len;i++)
                    {
                        type_oper = type_oper * 10 + (param[1].param[i] - '0');
                    }
                    //here call the low layer API to set TXPKTLEN, type_oper is the input x
                    spbletest_bt_tx_pktlen = (uint16)type_oper;
                    sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 6:   //"TXPWR"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get TXPWR X1,X2");
                    //here call the low layer API to get TXPWR
                    //wuding: the code coming from 7701, it looks strange
                    //sprintf((char *)g_rsp_str, "%s%s%s%d%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=", 1,",",58);
                    sprintf((char *)g_rsp_str, "%s%s%s%d,%d","+SPBLETEST:",s_bletest_str[bletest_index],"=",spbletest_bt_tx_pwr_type,spbletest_bt_tx_pwr_value);
                }
                else if ((param[1].param_len != 0) && (param[2].param_len != 0))
                {
                    int x1=0,x2=0,i;
                    x1 = param[1].param[0] - '0';

                    for (x2=0,i=0;i<param[2].param_len;i++)
                    {
                        x2 = x2 * 10 + (param[2].param[i] - '0');
                    }

                    //wuding: NPI AT cmd AT+SPBLETEST=TXPWR,x1,x2
                    //x1: 0---gain value;  1---power level
                    //x2: value
                    spbletest_bt_tx_pwr_type = x1;
                    spbletest_bt_tx_pwr_value = x2;

                    //here call the low layer API to set TXPWR, x1 & x2 are the input
                    sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 7:   //"TX"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get TX status");
                    //here call the low layer API to get TX status
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=", spbletest_bt_tx_status);
                }
                else if (param[1].param_len != 0)
                {
                    int pktcnt=0,i;
                    type_oper = param[1].param[0] - '0';

                    if (0 == type_oper)
                    {
                        spbletest_bt_tx_status = 0;
                        //here call the low layer API to stop TX
                        SCI_TRACE_LOW("SPBLETEST: stop TX");
#ifdef BT_NONSIG_SUPPORT
                        spbletest_bt_status = BT_SetNonSigTxTestMode(FALSE,FALSE, &spbletest_bt_non_param);
#endif
                    }
                    else if (1 == type_oper)
                    {
                        spbletest_bt_tx_status = 1;

                        if (param[2].param_len != 0)
                        {
                            spbletest_bt_tx_mode = param[2].param[0] - '0';
#ifdef BT_NONSIG_SUPPORT
                            spbletest_bt_non_param.channel = spbletest_bt_tx_ch;
                            spbletest_bt_non_param.pattern = spbletest_bt_tx_pattern;
                            spbletest_bt_non_param.pac.pac_type = spbletest_bt_tx_pkttype;
                            spbletest_bt_non_param.pac.pac_len = spbletest_bt_tx_pktlen;
                            spbletest_bt_non_param.item.power.power_type = spbletest_bt_tx_pwr_type;
                            spbletest_bt_non_param.item.power.power_value = spbletest_bt_tx_pwr_value;
#endif
                            if (0 == spbletest_bt_tx_mode)
                            {
                                //here call the low layer API to Sart TX for continues
                                SCI_TRACE_LOW("SPBLETEST: Sart TX for continues");
#ifdef BT_NONSIG_SUPPORT
                                spbletest_bt_non_param.pac.pac_cnt = 0;
#endif
                            }
                            else
                            {
                                if (param[3].param_len != 0)
                                {
                                    for (pktcnt=0,i=0;i<param[3].param_len;i++)
                                    {
                                        pktcnt = pktcnt * 10 + (param[3].param[i] - '0');
                                    }
                                }
				    else
				    {
				        pktcnt = 0;//continues
				    }

                                //here call the low layer API to Sart TX for single, pktcnt is input for pktcnt
                                SCI_TRACE_LOW("SPBLETEST: Sart TX for single");
#ifdef BT_NONSIG_SUPPORT
                                spbletest_bt_non_param.pac.pac_cnt = pktcnt;
#endif
                            }
#ifdef BT_NONSIG_SUPPORT
                            spbletest_bt_status = BT_SetNonSigTxTestMode(FALSE, TRUE, &spbletest_bt_non_param);
#endif
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }

                    if (TRUE == spbletest_bt_status)//lint !e774
                    {
                        sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "%s","+SPBLETEST:ERR");
                    }
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 8:   //"RXCH"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get RXCH");
                    //here call the low layer API to get RXCH
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=", spbletest_bt_rx_ch);
                }
                else if (param[1].param_len != 0)
                {
                    if (param[1].param_len == 1)  // valid type_oper range [0~39]
                    {
                        type_oper = param[1].param[0] - '0';
                    }
                    else if (param[1].param_len == 2)
                    {
                        type_oper = param[1].param[0] - '0';
                        type_oper = type_oper * 10 + (param[1].param[1] - '0');
                    }
                    spbletest_bt_rx_ch = (uint8)type_oper;
                    //here call the low layer API to set RXCH, type_oper is the input x
                    sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 9:   //"RXPATTERN"
            {
                //wuding: there is no RXPATTERN in BT No-Signal!
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get RXPATTERN");
                    //here call the low layer API to get RXPATTERN
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=", spbletest_bt_rx_pattern);
                }
                else if (param[1].param_len != 0)
                {
                    type_oper = param[1].param[0] - '0';
                    //wuding: NPI AT cmd
                    //PN9         ---0
                    //11110000---1
                    //1010       ---2
                    //PN15       ---3
                    //1111       ---4
                    //0000       ---5
                    //00001111---6
                    //0101       ---7


                    spbletest_bt_rx_pattern = (uint8)type_oper;
                    //here call the low layer API to set RXPATTERN, type_oper is the input x, range [0~3]
                    sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 10:   //"RXPKTTYPE"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get RXPKTTYPE");
                    //here call the low layer API to get RXPKTTYPE
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=", spbletest_bt_rx_pktype);
                }
                else if (param[1].param_len != 0)
                {
                    uint8 i = 0;
                    for (type_oper=0,i=0;i<param[1].param_len;i++)
                    {
                        type_oper = type_oper * 10 + (param[1].param[i] - '0');
                    }
                    //rang: 0~3
                    //0x00: RF_PHY_Test_Ref   0x01: ADV_NONCONN_IND  0x02: ADV_IND  0x03: DATA


                    //wuding: the same as TX PKT type
                    spbletest_bt_rx_pktype = (uint8)type_oper;

                    //here call the low layer API to set RXPKTTYPE, type_oper is the input x, range [0~5]
                    sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 11:   //"RXGAIN"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get RXGAIN");
                    //here call the low layer API to get RXGAIN Mode and x
                    if (0 == spbletest_bt_rx_gain_mode)
                    {
                        sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=", 0);
                    }
		      else
                    {
                        sprintf((char *)g_rsp_str, "%s%s%s%d,%d","+SPBLETEST:",s_bletest_str[bletest_index],"=", 1, spbletest_bt_rx_gain_value);
                    }
                }
                else if (param[1].param_len != 0)
                {
                    int mode=0,x=0,i;
                    mode = param[1].param[0] - '0';

                    spbletest_bt_rx_gain_mode = mode;//0 Auto; 1 Fix

                    if (0 == mode)
                    {
                        //here call the low layer API to Set RX Gain Auto mode
                        SCI_TRACE_LOW("SPBLETEST: Set RX Gain Auto mode");
                    }
                    else if (1 == mode)
                    {
                        if (param[2].param_len != 0)
                        {
                            for (x=0,i=0;i<param[2].param_len;i++)
                            {
                                x = x * 10 + (param[2].param[i] - '0');
                            }
                            //here call the low layer API to Set RX Gain Fix mode, x is input for x
                            //AT+SPBLETEST=RXGAIN,mode,[x]
                            //x: gain parameter , only available when mode is 1
                            if ((0 == x) || (1 == x) ||(2 == x) || (3 == x) || (4 == x) || (5 == x))//double check before send it to controller
                            {
                                spbletest_bt_rx_gain_value = x;
                            }
                           else
                           {
                               spbletest_bt_rx_gain_value = 0;
                           }
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }

                    sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            case 12:   //"RX"
            {
                if (quiry_flag)
                {
                    SCI_TRACE_LOW("SPBLETEST: get RX");
                    //here call the low layer API to get RX
                    sprintf((char *)g_rsp_str, "%s%s%s%d","+SPBLETEST:",s_bletest_str[bletest_index],"=", spbletest_bt_rx_status);
                }
                else if (param[1].param_len != 0)
                {
                    BOOLEAN enable = FALSE;
                    type_oper = param[1].param[0] - '0';

                    if(type_oper)
                    {
                        spbletest_bt_rx_status = 1;
                        enable = TRUE;
#ifdef BT_NONSIG_SUPPORT
                        spbletest_bt_non_param.pattern = spbletest_bt_rx_pattern;
                        spbletest_bt_non_param.channel = spbletest_bt_rx_ch;
                        if (1 == spbletest_bt_rx_gain_mode)//Fix Mode
                        {
                            spbletest_bt_non_param.item.rx_gain = spbletest_bt_rx_gain_value;
                        }
                        else//auto mode
                        {
                            spbletest_bt_non_param.item.rx_gain = 0;
                        }
                        spbletest_bt_non_param.pac.pac_type = spbletest_bt_rx_pktype;
#endif
                    }
                    else
                    {
                        spbletest_bt_rx_status = 0;
                        enable = FALSE;
                    }
#ifdef BT_NONSIG_SUPPORT
                    spbletest_bt_status = BT_SetNonSigRxTestMode(FALSE,enable,&spbletest_bt_non_param,(BT_ADDRESS*)spbletest_bt_address);
#endif
                    //here call the low layer API to set RX, type_oper is the input x, range [0~1]
                    if (spbletest_bt_status)//lint !e774
                    {
                        sprintf((char *)g_rsp_str, "%s","+SPBLETEST:OK");
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "%s","+SPBLETEST:ERR");
                    }
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);// return ok response for AT cmd first
                //for rx test end and start next tx test
                //BT_RX_STATUS((uint8)type_oper);
                break;
            }
            case 13:   //"RXDATA"
            {
                if (quiry_flag)
                {
                    uint32 res[5] = {0};
                    uint8 loop_times = 0;

                    SCI_TRACE_LOW("SPBLETEST: get RXDATA");

#ifdef BT_NONSIG_SUPPORT
                    //here call the low layer API to get RXDATA, such as error_bits,total_bits,error_packets,total_packets,rssi
                    spbttest_bt_rx_data_flag = FALSE;
                    spbletest_bt_status = BT_GetNonSigRxData(FALSE,BT_SPBTTEST_GetRXDataCallback);

                    if (spbletest_bt_status)
                    {
                        while(!spbttest_bt_rx_data_flag)
                        {
                            loop_times++;
                            if (loop_times < 12)//wait 600ms
                            {
                                SCI_SLEEP(50);
                            }
                            else
                            {
                                break;//quit the loop
                            }
                        }

                        if (spbttest_bt_rx_data_flag && (BT_SUCCESS == rx_data_show_status))
                        {
                            res[0] = rx_data_show_bit_err_cnt;
                            res[1] = rx_data_show_bit_cnt;
                            res[2] = rx_data_show_pkt_err_cnt;
                            res[3] = rx_data_show_pkt_cnt;
                            res[4] = rx_data_show_rssi;
    			       SCI_TRACE_LOW("rxdata, %d, %d, %d, %d", res[0],res[1],res[2],res[3],res[4]);
                        }
                        else
                        {
                            SCI_TRACE_LOW("rxdata, null. %d %d",spbttest_bt_rx_data_flag,rx_data_show_status);
                        }

                        sprintf((char *)g_rsp_str, "%s%s%s%d,%d,%d,%d,0x%x","+SPBLETEST:",s_bletest_str[bletest_index],"=",
                        res[0] /*error_bits  990*/, res[1] /*total_bits 1600000*/, res[2] /*error_packets 100*/, res[3] /*total_packets 1024*/,res[4]/*rssi*/);
                    }
                    else
#endif
                    {
                        sprintf((char *)g_rsp_str, "%s","+SPBLETEST:ERR");
                    }
                }   //lint !e529
                else
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            default:   //Error input para
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }
            }
        }
        else
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
        break;

    case ATC_CMD_TYPE_READ:
    case ATC_CMD_TYPE_TEST:
    default:
        return ERR_OPERATION_NOT_ALLOWED;
    }

//#endif
#endif
#endif
    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+FOTA command from SSAP
//  Global resource dependence : none
//  Author:  Shijun.cui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessFOTA )
{
    ATC_STATUS                    status = S_ATC_SUCCESS;
#ifdef FOTA_SUPPORT_ADUPS
    uint8                         parameter_type = 0;
    uint8                         b_ul_flag = 0;
    uint16                        pga_gain = 0;
    uint16                        digital_gain = 0;
    uint8                         *ptr = NULL;
	char						  *pcmd = NULL;
    uint32                        cmd_len = 0;
    uint32                        param[3] = {0};

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                pcmd = (char *)(PARA1.str_ptr->str_ptr);
				cmd_len = PARA1.str_ptr->str_len;
            }
            if (PARAM2_FLAG)
                param[0] = (uint32)PARA2.num;
            if (PARAM3_FLAG)
                param[1] = (uint32)PARA3.num;
            if (PARAM4_FLAG)
                param[2] = (uint32)PARA4.num;

			adups_DebugPrint("ATC_ProcessFOTA: cmd:%s, p1:%d, p2:%d, p3:%d", pcmd, param[0], param[1], param[2]);
			adups_send_newat_msg(pcmd, param[0], param[1], param[2], 0);
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            ptr = g_rsp_str;
            ptr = g_rsp_str + strlen((char*)g_rsp_str);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            break;
        }
    }
#endif
    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+SCWTEST command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSCWTEST)
{
    static BOOLEAN      scwtest_flag = FALSE;

    ATC_STATUS    status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    SCI_TRACE_LOW("ATC: ATC_ProcessSCWTEST is called");

    if(ATC_GET_CMD_TYPE == ATC_CMD_TYPE_SET)
    {
        if (PARAM1_FLAG)
        {
            scwtest_flag = (BOOLEAN)PARA1.num;

            if(scwtest_flag)
            {
                MNPHONE_DeactivePsEx(dual_sys);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_DEACTIVE_PS_CNF,0xff,ATC_NO_DOMAIN);

#ifndef _ULTRA_LOW_CODE_
                s_is_ps_activate[dual_sys]   = FALSE;
                g_is_ps_deactivate[dual_sys] = TRUE;

                status = S_ATC_DEFAULT_HANDLE;
#else
                SCI_SLEEP(1000);

                status = S_ATC_SUCCESS;
#endif
            }
            else
            {
                MNPHONE_StartupPsEx(dual_sys, MN_GMMREG_RAT_GPRS);
                SCI_SLEEP(1000);

#ifndef _ULTRA_LOW_CODE_
                s_is_ps_activate[dual_sys] = TRUE;
#endif
                status = S_ATC_SUCCESS;
            }
        }
    }
    else
    {
         sprintf((char*)g_rsp_str,"%s: %d",
             g_atc_info_table[AT_CMD_SCWTEST].cmd_name->str_ptr, scwtest_flag);
         ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
         status = S_ATC_SUCCESS;
    }
    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT^SCFG command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
//  This at command used to CW function
/*****************************************************************************/
//AT^SCFG="MEopMode/CT"[,<cwMode>[,<cwReserved>,<cwLevel>,<cwArfcn>]]
//response
//^SCFG: "MEopMode/CT",<cwMode>
//OK
//ERROR
//+CME ERROR: <err>
//Parameter Description:
//cwMode:"1"---Activate CW Test Mode
//             "0"---Terminate theCW Test Mode wave generation. The CW Test Mode itself remains active.
//cwReserved: Parameter reserved for future use.
//cwLevel: "low","high"
//cwArfcn: 0-1023, GSM850: "128"-"251"  GSM900: "0"-"124" OR "975"-"1023"  GSM1800: "512"-"885" GSM1900:(512-810)+32768="33280"-"33578"
//EXAMPLE: AT^SCFG="MEopMode/CT","1","","high",1


//AT^SCFG="MEopMode/CR"[,<crRat>,<crChannel>[,<crBand>],<crPath>,<crLna>]
//response
//^SCFG: "MEopMode/CR",<crMeas0>[,<crMeas1>]
//In case of <crPath>=1 or <crPath>=2:
//^SCFG: "MEopMode/CR",<crMeas0>
//In case of <crPath>=3:
//^SCFG: "MEopMode/CR",<crMeas0>,<crMeas1>
//OK
//ERROR
//+CME ERROR:<err>
//Parameter Description:
//crRat:0---GSM(2G)  2--UTRAN(3G) or EUTRAN(4G)
//crChannel:"0"---"65535"
//GSM850: "128"-"251"  GSM900: "0"-"124" OR "975"-"1023"  GSM1800: "512"-"885" GSM1900:(512-810)+32768="33280"-"33578"
//crBand:"1"--GSM 900  "2"--DCS1800   "4"---GSM850  "8"---PCS1900
//crPath:"1"---Main antenna path(2G,3G and 4G)  "2"---Rx diversity antenna path(3G and 4G)  "3"---Combined antenna paths(3G and LTE)
//crLna: "0"..."5" Gain stages
//crMeas0: "-120"..."0"  Result in dBm. This parameter gives the antenna path measurement result for the received RF Rx signal.
//EXAMPLE: AT^SCFG="MEopMode/CR",0,"1023",,"1","0"

AT_CMD_FUNC(ATC_Processxor_SCFG)
{
    ATC_STATUS     status = S_ATC_SUCCESS;
    uint8 op_mode = 0;
    static uint8 cw_mode = 0;
    uint8 cw_level = 0;
    uint8 cw_pcl = 0;
    uint16 arfcn = 0;
    MN_PHONE_BAND_TYPE_E band = 0;
    BOOLEAN cw_enable = FALSE;
    uint8 cr_rat= 0;
    uint8 cr_path = 1;
    uint16 gain = 0;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
	     if(PARAM1_FLAG)
	     {
                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    SCI_TRACE_LOW("ATC: ATC_Processxor_SCFG, param1 is not string!");
                    status = ERR_INVALID_INDEX;
                    break;
                }
                SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG PARA1.str_ptr=%s",PARA1.str_ptr->str_ptr);

                if (0 == stricmp((char *)PARA1.str_ptr->str_ptr, "MEopMode/CT") )  //lint !e718 !e746
                {
                   op_mode = 0;
                }
                else if (0 == stricmp((char *)PARA1.str_ptr->str_ptr, "MEopMode/CR")) //lint !e718 !e746
                {
                    op_mode = 1;
                }
                else
                {
                    SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG MEopMode ERROR %s",PARA1.str_ptr->str_ptr);
                    status = ERR_INVALID_INDEX;
                    break;
                }

                if(PARAM2_FLAG)
                {
                    if(0 == op_mode)//MEopMode/CT
                    {
                        if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                        {
                            SCI_TRACE_LOW("ATC: ATC_Processxor_SCFG, param2 is not string!");
                            status = ERR_INVALID_INDEX;
                            break;
                        }
                        SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG PARA2.str_ptr=%d, %s",PARA2.str_ptr->str_len,PARA2.str_ptr->str_ptr);

                        cw_mode= atoi((char *)PARA2.str_ptr->str_ptr);

                        SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG cw_mode=%d",cw_mode);
                        if(cw_mode == 0)
                        {
                            cw_enable = FALSE;
                            L1API_GsmTxCWPwrTest(FALSE, arfcn, band, cw_pcl);   //lint !e718 !e746

                            arfcn = 0;
                            band = 0;

                        }
                        else if(cw_mode == 1)
                        {
                            cw_enable = TRUE;

                            if(!PARAM4_FLAG || !PARAM5_FLAG)
                            {
                                status = ERR_INVALID_INDEX;
                                SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG !PARAM4_FLAG || !PARAM5_FLAG ERROR");
                                break;
                            }

                            if(PARAM4_TYPE != ATC_CMD_PARAM_TYPE_STRING || PARAM5_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
                            {
                                status = ERR_INVALID_INDEX;
                                SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG !PARAM4_TYPE || !PARAM5_TYPE ERROR");
                                break;
                            }

                            if (0 == stricmp((char *)PARA4.str_ptr->str_ptr, "high") )
                            {
                               cw_level = 1;
                            }
                            else if (0 == stricmp((char *)PARA4.str_ptr->str_ptr, "low"))
                            {
                                cw_level = 0;
                            }
                            else
                            {
                                status = ERR_INVALID_INDEX;
                                SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG cw_level ERROR %s", PARA4.str_ptr->str_ptr);
                                break;
                            }

                            arfcn = PARA5.num;

                            if((arfcn <= 124) || ((arfcn >= 975) && (arfcn <= 1023)))
                            {
                                band= MN_PHONE_BAND_GSM;
                            }
                            else if((arfcn >= 128) && (arfcn <= 251))
                            {
                                band= MN_PHONE_BAND_GSM850;
                            }
                            else if((arfcn >= 512) && (arfcn <= 885))
                            {
                                band= MN_PHONE_BAND_DCS;
                            }
                           else if((arfcn >= (0x8000 + 512)) && (arfcn <= (0x8000 + 810)))
                            {
                                arfcn = arfcn - 0x8000;
                                band = MN_PHONE_BAND_PCS;
                            }
                            else
                            {
                                status = ERR_INVALID_INDEX;
                                SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG arfcn ERROR %s", PARA5.str_ptr->str_ptr);
                                break;
                            }

                            if(MN_PHONE_BAND_GSM == band  || MN_PHONE_BAND_GSM850 == band )
                            {
                                if(cw_level == 0)
                                {
                                    cw_pcl= 19;
                                }
                                else
                                {
                                    cw_pcl= 17;
                                }
                            }
                            else
                            {
                                if(cw_level == 0)
                                {
                                    cw_pcl= 15;
                                }
                                else
                                {
                                    cw_pcl= 10;
                                }
                            }

                            L1API_GsmTxCWPwrTest(TRUE, arfcn, band, cw_pcl); //lint !e718 !e746

                            SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG arfcn=%d, cw_band=%d,cw_pcl=%d",arfcn,band,cw_pcl);
                        }
                        else
                        {
                            status = ERR_INVALID_INDEX;
                            break;
                        }

                    }
                    else  //MEopMode/CR
                    {
                        int16 rssi = 0;
                        BOOLEAN res = FALSE;
                        uint8 times = 0;

                        if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
                        {
                            SCI_TRACE_LOW("ATC: ATC_Processxor_SCFG, param2 is not num!");
                            status = ERR_INVALID_INDEX;
                            break;
                        }

                        cr_rat = PARA2.num;
                        if(0 != cr_rat)
                        {
                            status = ERR_INVALID_INDEX;
                            SCI_TRACE_LOW("ATC:SCFG cr_rat ERROR %d",cr_rat);
                            break;
                        }

                        if(!PARAM3_FLAG || PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                        {
                            status = ERR_INVALID_INDEX;
                            SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG CR !PARAM3_FLAG || PARAM3_TYPE ! ERROR");
                            break;
                        }

                        arfcn = atoi((char *)PARA3.str_ptr->str_ptr);

                        if((arfcn <= 124) || ((arfcn >= 975) && (arfcn <= 1023)))
                        {
                            band= MN_PHONE_BAND_GSM;
                        }
                        else if((arfcn >= 128) && (arfcn <= 251))
                        {
                            band= MN_PHONE_BAND_GSM850;
                        }
                        else if((arfcn >= 512) && (arfcn <= 885))
                        {
                            band= MN_PHONE_BAND_DCS;
                        }
                        else if((arfcn >= (0x8000 + 512)) && (arfcn <= (0x8000 + 810)))
                        {
                            arfcn = arfcn - 0x8000;
                            band = MN_PHONE_BAND_PCS;
                        }
                        else
                        {
                            status = ERR_INVALID_INDEX;
                            SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG CR arfcn ERROR %s", PARA5.str_ptr->str_ptr);
                            break;
                        }

                        if(!PARAM5_FLAG || !PARAM6_FLAG)
                        {
                            status = ERR_INVALID_INDEX;
                            SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG CR !PARAM5_FLAG || !PARAM6_FLAG ERROR");
                            break;
                        }

                        if(PARAM5_TYPE != ATC_CMD_PARAM_TYPE_STRING || PARAM6_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                        {
                            status = ERR_INVALID_INDEX;
                            SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG CR !PARAM5_TYPE || !PARAM6_TYPE ERROR");
                            break;
                        }

                        cr_path = atoi((char *)PARA5.str_ptr->str_ptr);
                        if(1 != cr_path)
                        {
                            status = ERR_INVALID_INDEX;
                            SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG CR cr_path ERROR %d",cr_path);
                            break;
                        }

                        gain = atoi((char *)PARA6.str_ptr->str_ptr);

                        //layer1 interface
                        L1API_GsmRXCWPwrTest(TRUE, arfcn, band, gain); //lint !e718 !e746

                        while(!res)
                        {
                            SCI_Sleep(1000);
                            res = L1API_GetGsmRXPwrResult(arfcn,band, &rssi); //get rssi value
                            times++;

                            if((FALSE == res) && (times >=20))
                            {
                                break;
                            }
                        }
                        L1API_GsmRXCWPwrTest(FALSE, arfcn, band, gain); //lint !e718 !e746

                        SCI_TRACE_LOW("ATC:ATC_Processxor_SCFG CR arfcn %d, band %d,rxlev %d,gain %d, res %d",arfcn, band, rssi,gain, res);

                       sprintf((char *)g_rsp_str,"%s:\"MEopMode/CR\",\"%d\"",g_atc_info_table[AT_CMD_xor_SCFG].cmd_name->str_ptr,rssi);

                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }

                }
                else
                {
                    if(0 ==op_mode)
                    {
                        sprintf((char *)g_rsp_str,"%s:\"MEopMode/CT\",\"%d\"",g_atc_info_table[AT_CMD_xor_SCFG].cmd_name->str_ptr,cw_mode);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;
                    }
                    else
                    {
                        SCI_TRACE_LOW("ATC:AT^SCFG=MEopMode/CR NOT SUPPORT");
                        return S_ATC_FAIL;
                    }

                }

	     }
            else
            {
                status = ERR_INVALID_INDEX;
                break;
            }
            break;
        }
        case ATC_CMD_TYPE_READ:
	     status = ERR_OPERATION_NOT_SUPPORTED;
            break;
	 case ATC_CMD_TYPE_TEST:
            break;
	 default:
	     status = ERR_OPERATION_NOT_SUPPORTED;
	     break;
    }
    return status;
}

#ifdef BT_HFU_SUPPORT
#define BT_HFU_CMD_MAX 8
uint8 atc_hfu_reject_flag = 0;
extern BOOLEAN s_bt_audconnind_flag ;
ATC_STATUS ATC_SPBTCTRL_HFU_PROCESS( ATC_CONFIG_T*atc_config_ptr,ATC_SPBTTEST_PARAM_T*param)
{
    uint8 i = 0;
    SCI_TIMER_PTR redial_time  = 0 ;
    const char *s_bt_hfu_str[BT_HFU_CMD_MAX] = {"ANSWER","HANGUP","REJECT","REDIAL","SWITCH","VOINC","VODEC","HOLD"};

     if (param->param_len != 0)
     {
         for (i=0; i<BT_HFU_CMD_MAX; i++)
        {
            if (!strcmp((char *)param->param, s_bt_hfu_str[i]))
            {
                break;
            }
        }

       switch(i)
       {
            case 0://hfu answer dial
                SCI_TRACE_LOW("ATC_SPBTCTRL_HFU_PROCESS: BT_HfuAnswerCall");
                BT_HfuAnswerCall();
                break;
            case 1://hfu hangup dial
                SCI_TRACE_LOW("ATC_SPBTCTRL_HFU_PROCESS: BT_HfuHangup");
                BT_HfuHangup();
                break;
            case 2://reject dial
                SCI_TRACE_LOW("ATC_SPBTCTRL_HFU_PROCESS: BT_Reject");
                atc_hfu_reject_flag = 1;
                BT_HfuHangup();
                break;
            case 3:// redial
                SCI_TRACE_LOW("ATC_SPBTCTRL_HFU_PROCESS: BT_HfuRedial");
                if(s_bt_audconnind_flag == TRUE)
                {
                    SCI_TRACE_LOW("ATC_SPBTCTRL_HFU_PROCESS: s_bt_call_flag = %d",s_bt_audconnind_flag);
                    BT_HfuHangup();
                    redial_time = timed_event_in(500*1000, NULL, 0, NULL);
                    cancel_timed_event(redial_time,0,0);
                }
                BT_HfuRedial();
                break;
            case 4://switch channel
                SCI_TRACE_LOW("ATC_SPBTCTRL_HFU_PROCESS: BT_Hfu_Switch_channel");
                BT_HfuSwitchAudio(TRUE);
                break;
            case 5://increase volumn
                SCI_TRACE_LOW("ATC_SPBTCTRL_HFU_PROCESS: BT_Hfu_Increase_volumn");
                break;
            case 6://decrease volumn
                SCI_TRACE_LOW("ATC_SPBTCTRL_HFU_PROCESS: BT_Hfu_Decrease_volumn");
                break;
            case 7://hold
                SCI_TRACE_LOW("ATC_SPBTCTRL_HFU_PROCESS: BT_Hfu_Hold");
                break;
            default :
                SCI_TRACE_LOW("ATC_SPBTCTRL_HFU_PROCESS: default");
                return ERR_OPERATION_NOT_ALLOWED;
       }
        return S_ATC_SUCCESS;
      }
      else
      {
          return ERR_OPERATION_NOT_ALLOWED;
      }
}
#endif

#ifdef BT_BLE_HOGP_SUPPORT
#define BT_HOGP_CMD_MAX 1
ATC_STATUS ATC_SPBTCTRL_HOGP_PROCESS( ATC_CONFIG_T*atc_config_ptr,ATC_SPBTTEST_PARAM_T*param)
{
    uint8 i = 0;
    uint16  key_val = 0;
    const char *s_bt_hogp_str[BT_HFU_CMD_MAX] = {"KEY"};

     if (param->param_len != 0)
     {
         for (i=0; i<BT_HFU_CMD_MAX; i++)
        {
            if (!strcmp((char *)param->param, s_bt_hogp_str[i]))
            {
                break;
            }
        }

       switch(i)
       {
            case 0://hogp at input key value
            {
                SCI_TRACE_LOW("ATC_SPBTCTRL_HOGP_PROCESS: HOGP INPUT KEY VALUE");
                 if (param[1].param_len != 0)
                 {
                    if (ConvertStringtoDecimal(param[1].param,param[1].param_len,&key_val))
                    {
                        SCI_TRACE_LOW("BT zyh HOGP Key key value=%d",key_val);
                        BT_HIDSendBTKey(KEY_PRESSED,key_val,FALSE);
                        SCI_SLEEP(10);
                        BT_HIDSendBTKey(KEY_RELEASED,key_val,FALSE);
                     }
                     else
                     {
                        return ERR_OPERATION_NOT_ALLOWED;
                     }
                 }
                break;
               }

            default :
                SCI_TRACE_LOW("ATC_SPBTCTRL_HOGP_PROCESS: default");
                return ERR_OPERATION_NOT_ALLOWED;
       }
        return S_ATC_SUCCESS;
      }
      else
      {
          return ERR_OPERATION_NOT_ALLOWED;
      }

}
#endif

#ifndef DISABLE_RPC_CFW_EmodGetNvVersion
extern uint32_t CFW_EmodGetNvVersion(void);
#endif

ATC_STATUS ATC_ProcessNVGV(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                )
{
    ATC_STATUS     status = S_ATC_SUCCESS;
    uint32         v_nvVersion;
    uint8          temp_str[9] = {0};

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            v_nvVersion = CFW_EmodGetNvVersion();

            SCI_TransBinToHexStr(&v_nvVersion, 4, temp_str, 9);
            sprintf((char*)g_rsp_str, "%s: \"%s\"", g_atc_info_table[AT_CMD_NVGV].cmd_name->str_ptr, temp_str);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

	       case ATC_CMD_TYPE_TEST:
            break;

      	 default:
      	     status = ERR_OPERATION_NOT_SUPPORTED;
      	     break;
    }

    return status;
}

#ifndef DISABLE_RPC_CFW_EmodGetNvData
extern uint8_t * CFW_EmodGetNvData(uint8_t dataType, uint8_t nSim);
#endif

ATC_STATUS ATC_ProcessNVPC(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                )
{
    ATC_STATUS     status = S_ATC_SUCCESS;
    uint8          operType;
    uint8          dataType;
    uint16         dataLength;
    uint16         offset;
    uint8         *p_nv;
    MN_DUAL_SYS_E  dual_sys   =  ATC_CURRENT_SIM_ID_MUX;
    uint8         *p_temp;
    int32          ret;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if((PARAM1_FLAG && PARAM1_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) \
        	 && (PARAM2_FLAG && PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
        {
            dataType = (uint8)PARA1.num;
            operType = (uint8)PARA2.num;

            switch(operType)
            {
                // nv data read
                case 0:
                  if((PARAM3_FLAG && PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) \
                  	 && (PARAM4_FLAG && PARAM4_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
                  {
                    offset = (uint16)PARA3.num;
                    dataLength = (uint16)PARA4.num;

                    p_nv = CFW_EmodGetNvData(dataType, dual_sys);

                    p_temp = SCI_ALLOCAZ((dataLength * 2 + 1));

                    SCI_TransBinToHexStr((uint8 *)(p_nv + offset), dataLength, p_temp, (dataLength * 2 + 1));

                    sprintf((char*)g_rsp_str,"%s: \"%s\"", g_atc_info_table[AT_CMD_NVPC].cmd_name->str_ptr,p_temp);

                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                    SCI_FREE(p_temp);
                  }
                  else
                  {
                    status = ERR_INVALID_INDEX;
                  }
               	  break;

                // nv data set
                case 1:
                  if((PARAM3_FLAG && PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) \
                  	 && (PARAM4_FLAG && PARAM4_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) \
                  	 && (PARAM5_FLAG && PARAM5_TYPE == ATC_CMD_PARAM_TYPE_STRING))
                  {
                    offset = (uint16)PARA3.num;
                    dataLength = (uint16)PARA4.num;

                    if(PARA5.str_ptr->str_len <= (dataLength * 2))
                    {
                      p_temp = SCI_ALLOCAZ(dataLength);
                      ConvertHexToBin((uint8*)PARA5.str_ptr->str_ptr, PARA5.str_ptr->str_len, p_temp);

                      ret = CFW_EmodSetNvData(dataType, offset, dataLength, p_temp, dual_sys);

                      if(ret != 0)
                      {
                        status = ERR_UNKNOWN;
                      }

                      SCI_FREE(p_temp);
                    }
                    else
                    {
                      status = ERR_TEXT_STRING_TOO_LONG;
                    }

                  }
                  else
                  {
                    status = ERR_INVALID_INDEX;
                  }
                	 break;

                // nv data write to flash
                case 2:
                	 ret = CFW_EmodSaveNvData(dataType, dual_sys);

                  if(ret != 0)
                  {
                    status = ERR_UNKNOWN;
                  }
               	  break;

                // nv data special
                default:
                  if((PARAM3_FLAG && PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) \
                  	 && (PARAM4_FLAG && PARAM4_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) \
                  	 && (PARAM5_FLAG && PARAM5_TYPE == ATC_CMD_PARAM_TYPE_STRING))
                  {
                    offset = (uint16)PARA3.num;
                    dataLength = (uint16)PARA4.num;

                    if(PARA5.str_ptr->str_len <= (dataLength * 2))
                    {
                      p_temp = SCI_ALLOCAZ(dataLength);
                      ConvertHexToBin((uint8*)PARA5.str_ptr->str_ptr, PARA5.str_ptr->str_len, p_temp);

                      ret = CFW_EmodSpecialNvProcess(operType, dataType, offset, dataLength, p_temp, dual_sys);

                      if(ret != 0)
                      {
                        status = ERR_UNKNOWN;
                      }

                      SCI_FREE(p_temp);
                    }
                    else
                    {
                      status = ERR_TEXT_STRING_TOO_LONG;
                    }

                  }
                  else
                  {
                    status = ERR_INVALID_INDEX;
                  }
                	 break;

            }
        }
        else
        {
            status = ERR_INVALID_INDEX;
        }
        break;
    case ATC_CMD_TYPE_TEST:
        break;
    default:
    	   status = ERR_AT_COMMAND_DISCARDED;
        break;
    }

  return status;
}


ATC_STATUS ATC_Processxor_SSIT(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                )
{
    ATC_STATUS     status = S_ATC_SUCCESS;
    uint32         module;
    uint32         cmd;
    uint32         v1 = 0;
    uint32         v2 = 0;
    uint32         v3 = 0;
    uint32         result;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if((PARAM1_FLAG && PARAM1_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC) \
        	 && (PARAM2_FLAG && PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
        {
            module = PARA1.num;
            cmd = PARA2.num;

            if(PARAM3_FLAG && PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC)
            {
              v1 = PARA3.num;
            }

            if(PARAM4_FLAG && PARAM4_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC)
            {
              v2 = PARA4.num;
            }

            if(PARAM5_FLAG && PARAM5_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC)
            {
              v3 = PARA5.num;
            }

            result = CFW_EmodEngineerCmd(module, cmd, v1, v2, v3);

            sprintf((char*)g_rsp_str,"%s: %d", g_atc_info_table[AT_CMD_xor_SSIT].cmd_name->str_ptr, result);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            status = ERR_INVALID_INDEX;
        }
        break;
    case ATC_CMD_TYPE_TEST:
        break;
    default:
    	   status = ERR_AT_COMMAND_DISCARDED;
        break;
    }

  return status;
}

AT_CMD_FUNC(ATC_ProcessCACCP)
{
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;

    uint8 nMode = 0;
    uint8 nPath = 0;
    uint8 nCtrl = 0;
    uint8 *nParam = PNULL;
    uint16 param_len = 0;

    uint8 resultCode = 0;
    uint8 hasMsg = 0;
    uint8 resultMsg[900]   = {0};

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                nMode = PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                nPath = PARA2.num;
            }

            if (PARAM3_FLAG)
            {
                nCtrl = PARA3.num;
            }

            if (PARAM4_FLAG)
            {
                nParam = PARA4.str_ptr->str_ptr;
                param_len = PARA4.str_ptr->str_len;
            }

            CSW_SetAudioCodecCalibParam(&resultCode, &hasMsg, resultMsg, nMode, nPath, nCtrl, nParam, param_len);
            if (resultCode != 0)
            {
                sprintf((char *)g_rsp_str,"ERROR %d",resultCode);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = S_ATC_SUCCESS;
                if (hasMsg)
                {
                    sprintf((char *)g_rsp_str,"%s",resultMsg);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }
            SCI_MEMSET(resultMsg, 0x00, 900);
            break;
        }
        case ATC_CMD_TYPE_READ:
        {
            goto SUCCESS_WITH_CACCP_REPORT;
        }
	 case ATC_CMD_TYPE_TEST:
        {
            goto SUCCESS_WITH_CACCP_REPORT;
        }
	 default:
	     status = ERR_OPERATION_NOT_SUPPORTED;
	     break;
    }

    return status;
SUCCESS_WITH_CACCP_REPORT:
    sprintf((char *)g_rsp_str,"+CACCP");
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

AT_CMD_FUNC(ATC_ProcessCAVQE)
{
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;

    uint8 nMode = 0;
    uint8 nPath = 0;
    uint8 nCtrl = 0;
    uint8 *nParam = NULL;
    uint param_len = 0;

    uint8 resultCode = 0;
    uint8 hasMsg = 0;
    uint8 resultMsg[900]   = {0};

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
            goto SUCCESS_WITH_CAVQE_REPORT;
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                nMode = PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                nPath = PARA2.num;
            }

            if (PARAM3_FLAG)
            {
                nCtrl = PARA3.num;
            }

            if (PARAM4_FLAG)
            {
                nParam = PARA4.str_ptr->str_ptr;
                param_len = PARA4.str_ptr->str_len;
            }

            CSW_SetAudioZspVqeCalibParam(&resultCode, &hasMsg, resultMsg, nMode, nPath, nCtrl, nParam, param_len);
            if (resultCode != 0)
            {
                sprintf((char *)g_rsp_str,"ERROR %d",resultCode);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = S_ATC_SUCCESS;
                if (hasMsg)
                {
                    sprintf((char *)g_rsp_str,"%s",resultMsg);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }
            SCI_MEMSET(resultMsg, 0x00, 900);
            break;
        }
        default:
        {
            return S_ATC_FAIL;
        }
    }
    return status;
SUCCESS_WITH_CAVQE_REPORT:
    sprintf((char *)g_rsp_str,"+CAVQE");
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

#ifndef DISABLE_RPC_CSW_SetAudioZspPreProcessCalibParam
extern uint32_t CSW_SetAudioZspPreProcessCalibParam(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength);
#endif


AT_CMD_FUNC(ATC_ProcessCAPRE)
{
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;

    uint8 nMode = 0;
    uint8 nPath = 0;
    uint8 nCtrl = 0;
    uint8 *nParam = NULL;
    uint param_len = 0;

    uint8 resultCode = 0;
    uint8 hasMsg = 0;
    uint8 resultMsg[900]   = {0};

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
            goto SUCCESS_WITH_CAPRE_REPORT;
        case ATC_CMD_TYPE_SET:
            if (PARAM1_FLAG)
            {
                nMode = PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                nPath = PARA2.num;
            }

            if (PARAM3_FLAG)
            {
                nCtrl = PARA3.num;
            }

            if (PARAM4_FLAG)
            {
                nParam = PARA4.str_ptr->str_ptr;
                param_len = PARA4.str_ptr->str_len;
            }

            CSW_SetAudioZspPreProcessCalibParam(&resultCode, &hasMsg, resultMsg, nMode, nPath, nCtrl, nParam, param_len);
            if (resultCode != 0)
            {
                sprintf((char *)g_rsp_str,"ERROR %d",resultCode);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = S_ATC_SUCCESS;
                if (hasMsg)
                {
                    sprintf((char *)g_rsp_str,"%s",resultMsg);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }
            SCI_MEMSET(resultMsg, 0x00, 900);
            break;
        default:
            return S_ATC_FAIL;
    }
    return status;
SUCCESS_WITH_CAPRE_REPORT:
    sprintf((char *)g_rsp_str,"+CAPRE");
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

#ifndef DISABLE_RPC_CSW_SetAudioZspPostProcessCalibParam
extern uint32_t CSW_SetAudioZspPostProcessCalibParam(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength);
#endif

AT_CMD_FUNC(ATC_ProcessCAPOST)
{
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;

    uint8 nMode = 0;
    uint8 nPath = 0;
    uint8 nCtrl = 0;
    uint8 *nParam = NULL;
    uint param_len = 0;

    uint8 resultCode = 0;
    uint8 hasMsg = 0;
    uint8 resultMsg[900]   = {0};

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
            goto SUCCESS_WITH_CAPOST_REPORT;
        case ATC_CMD_TYPE_SET:
            if (PARAM1_FLAG)
            {
                nMode = PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                nPath = PARA2.num;
            }

            if (PARAM3_FLAG)
            {
                nCtrl = PARA3.num;
            }

            if (PARAM4_FLAG)
            {
                nParam = PARA4.str_ptr->str_ptr;
                param_len = PARA4.str_ptr->str_len;
            }

            CSW_SetAudioZspPostProcessCalibParam(&resultCode, &hasMsg, resultMsg, nMode, nPath, nCtrl, nParam, param_len);
            if (resultCode != 0)
            {
                sprintf((char *)g_rsp_str,"ERROR %d",resultCode);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = S_ATC_SUCCESS;
                if (hasMsg)
                {
                    sprintf((char *)g_rsp_str,"%s",resultMsg);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }
            SCI_MEMSET(resultMsg, 0x00, 900);
            break;

       default:
            return S_ATC_FAIL;
    }

    return status;
SUCCESS_WITH_CAPOST_REPORT:
    sprintf((char *)g_rsp_str,"+CAPOST");
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

#ifndef DISABLE_RPC_CSW_WriteCalibParamToFlash
extern uint32_t CSW_WriteCalibParamToFlash(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength);
#endif


AT_CMD_FUNC(ATC_ProcessCAWTF)
{
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;

    uint8 nPath = 0;
    uint8 nCtrl = 0;
    uint8 *nParam = NULL;
    uint param_len = 0;

    uint8 resultCode = 0;
    uint8 hasMsg = 0;
    uint8 resultMsg[900]   = {0};

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
            goto SUCCESS_WITH_CAWTF_REPORT;
        case ATC_CMD_TYPE_SET:
            if (PARAM1_FLAG)
            {
                nPath = PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                nCtrl = PARA2.num;
            }

            if (PARAM3_FLAG)
            {
                nParam = PARA3.str_ptr->str_ptr;
                param_len = PARA3.str_ptr->str_len;
            }

            CSW_WriteCalibParamToFlash(&resultCode, &hasMsg, resultMsg, nPath, nCtrl, nParam, param_len);
            if (resultCode != 0)
            {
                sprintf((char *)g_rsp_str,"ERROR %d",resultCode);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = S_ATC_SUCCESS;
                if (hasMsg)
                {
                    sprintf((char *)g_rsp_str,"%s",resultMsg);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }
            SCI_MEMSET(resultMsg, 0x00, 900);
            break;
        default:
            return S_ATC_FAIL;

    }
    return status;
SUCCESS_WITH_CAWTF_REPORT:
    sprintf((char *)g_rsp_str,"+CAWTF");
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

#ifndef DISABLE_RPC_CSW_ExportCalibOrImportCalibFlashParam
extern uint32_t CSW_ExportCalibOrImportCalibFlashParam(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nPath, uint8_t nCtrl, uint16_t nOffset, uint16_t nlength, uint8_t * nParam, uint16_t nParamLength);
#endif


AT_CMD_FUNC(ATC_ProcessCAIET)
{
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;

    uint8 nPath = 0;
    uint8 nCtrl = 0;
    uint16 nOffset = 0;
    uint16 nLength = 0;
    uint8 *nParam = NULL;
    uint param_len = 0;

    uint8 resultCode = 0;
    uint8 hasMsg = 0;
    uint8 resultMsg[900]   = {0};

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
            goto SUCCESS_WITH_CAIET_REPORT;
        case ATC_CMD_TYPE_SET:
            if (PARAM1_FLAG)
            {
                nPath = PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                nCtrl = PARA2.num;
            }

            if (PARAM3_FLAG)
            {
                nOffset = PARA3.num;
            }

            if (PARAM4_FLAG)
            {
                nLength = PARA4.num;
            }

            if (PARAM5_FLAG)
            {
                nParam = PARA5.str_ptr->str_ptr;
                param_len = PARA5.str_ptr->str_len;
            }

            CSW_ExportCalibOrImportCalibFlashParam(&resultCode, &hasMsg, resultMsg, nPath, nCtrl, nOffset, nLength, nParam, param_len);
            if (resultCode != 0)
            {
                sprintf((char *)g_rsp_str,"ERROR %d",resultCode);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = S_ATC_SUCCESS;
                if (hasMsg)
                {
                    sprintf((char *)g_rsp_str,"%s",resultMsg);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }
            SCI_MEMSET(resultMsg, 0x00, 900);
            break;
        default:
            return S_ATC_FAIL;
    }

    return status;
SUCCESS_WITH_CAIET_REPORT:
    sprintf((char *)g_rsp_str,"+CAIET");
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

AT_CMD_FUNC(ATC_ProcessCADTF)
{
    ATC_STATUS     status = S_ATC_FAIL;

    uint8 nPath = 0;
    uint8 nCtrl = 0;
#if 0
    uint8 *nParam = NULL;
    uint param_len;

    uint8 resultCode = 0;
    uint8 hasMsg = 0;
    uint8 resultMsg[900]   = {0};

    uint8 count;
    count = cur_cmd_info->cur_param_num;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
            goto SUCCESS_WITH_CADTF_REPORT;
        case ATC_CMD_TYPE_SET:
            if (PARAM1_FLAG)
            {
                nPath = PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                nCtrl = PARA2.num;
            }

            if (nCtrl)
            {
                DSPVB_PcmDumpEnable();
            }
            else
            {
                DSPVB_PcmDumpStop();
            }
            status = S_ATC_SUCCESS;

            if (count == 2)
            {
                if (nCtrl)
                {
                    DSPVB_PcmDumpEnable();
                }
                else
                {
                    DSPVB_PcmDumpStop();
                }
                status = S_ATC_SUCCESS;
                break;
            }

            if (PARAM3_FLAG)
            {
                nParam = PARA3.str_ptr->str_ptr;
                param_len = PARA3.str_ptr->str_len;
            }

            CSW_DumpPcmDataToTflash(&resultCode, &hasMsg, resultMsg, nPath, nCtrl, nParam, param_len);
            if (resultCode)
            {
                sprintf((char *)g_rsp_str,"ERROR %d",resultCode);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = S_ATC_SUCCESS;
                if (hasMsg)
                {
                    sprintf((char *)g_rsp_str,"%s",resultMsg);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }
            SCI_MEMSET(resultMsg, 0x00, 900);

            break;

        default:
            return S_ATC_FAIL;
    }
#endif
    return status;
SUCCESS_WITH_CADTF_REPORT:
    sprintf((char *)g_rsp_str,"+CADTF");
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

AT_CMD_FUNC(ATC_ProcessCAVCT)
{
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;

    uint8 nPath = 0;
    uint8 nCtrl = 0;
    uint8 *nParam = NULL;
    uint param_len = 0;

    uint8 resultCode = 0;
    uint8 hasMsg = 0;
    uint8 resultMsg[900]   = {0};

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
            goto SUCCESS_WITH_CAVCT_REPORT;
        case ATC_CMD_TYPE_SET:
            if (PARAM1_FLAG)
            {
                nPath = PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                nCtrl = PARA2.num;
            }

            if (PARAM3_FLAG)
            {
                nParam = PARA3.str_ptr->str_ptr;
                param_len = PARA3.str_ptr->str_len;
            }

            CSW_VersionControl(&resultCode,&hasMsg, resultMsg, nPath, nCtrl, nParam, param_len);
            if (resultCode != 0)
            {
                sprintf((char *)g_rsp_str,"ERROR %d",resultCode);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = S_ATC_SUCCESS;
                if (hasMsg)
                {
                    sprintf((char *)g_rsp_str,"%s", resultMsg);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }
            SCI_MEMSET(resultMsg, 0x00, 900);
            break;

        default:
            return S_ATC_FAIL;
    }
    return status;
SUCCESS_WITH_CAVCT_REPORT:
    sprintf((char *)g_rsp_str,"+CAVCT");
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

AT_CMD_FUNC(ATC_ProcessCAVIMS)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
	BOOLEAN volte_on = FALSE;
	SCI_TraceLow("ATC_ProcessCAVIMS");
	if (ATC_GET_CMD_TYPE == ATC_CMD_TYPE_SET)
	{
		if (PARAM1_FLAG)
		{
			volte_on = PARA1.num;
			SCI_TraceLow("volte_on:0x%x",volte_on);
			MMIAPIPHONE_SetVolteInfo(dual_sys, volte_on);
		}
		else
		{
			return S_ATC_FAIL;
		}
	}
	else if(ATC_GET_CMD_TYPE == ATC_CMD_TYPE_READ)
	{
		uint8 vlolte_info;
		vlolte_info = MMIAPIPHONE_GetVolteInfo();

		sprintf((char *)g_rsp_str, "%s:%d",
                                    g_atc_info_table[AT_CMD_CAVIMS].cmd_name->str_ptr,
                                    (vlolte_info&(1<<dual_sys))>>(dual_sys));
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	}
	else
	{
		sprintf((char *)g_rsp_str, "%s",
                                    g_atc_info_table[AT_CMD_CAVIMS].cmd_name->str_ptr);
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	}
	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessSPSELMODE)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
	if (ATC_GET_CMD_TYPE == ATC_CMD_TYPE_SET)
	{
		if (PARAM1_FLAG)
		{
			if(1 != PARA1.num)
				return S_ATC_FAIL;

			MNPHONE_SetNetMode(0, dual_sys);
			MNPHONE_ClearFPLMN(dual_sys);
		}
		else
		{
			return S_ATC_FAIL;
		}
	}
	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessRRCREL)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
	if (ATC_GET_CMD_TYPE == ATC_CMD_TYPE_SET)
	{
		if (PARAM1_FLAG)
		{
			MNPHONE_SetRRCRel(PARA1.num,dual_sys);
		}
		else
		{
			return S_ATC_FAIL;
		}
	}
	else if (ATC_GET_CMD_TYPE == ATC_CMD_TYPE_READ)
	{
		uint32 rrcRel;
		rrcRel = MNPHONE_GetRRCRel(dual_sys);

		sprintf((char *)g_rsp_str, "%s:%d",
                                    g_atc_info_table[AT_CMD_RRCREL].cmd_name->str_ptr,rrcRel);
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	}
	else
	{
		sprintf((char *)g_rsp_str, "%s",
                                    g_atc_info_table[AT_CMD_RRCREL].cmd_name->str_ptr);
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	}
	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessIMSEN)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
	BOOLEAN imsEn = FALSE;
	SCI_TraceLow("ATC_ProcessIMSEN");
	if (ATC_GET_CMD_TYPE == ATC_CMD_TYPE_SET)
	{
		if (PARAM1_FLAG)
		{
			imsEn = PARA1.num;
			SCI_TraceLow("volte_on:0x%x",imsEn);
			//CFW_ImsSetVolte(imsEn,PHONE_GetFreeUti(),dual_sys);
			CFW_ImsEnVolte(imsEn, dual_sys);
		}
		else
		{
			return S_ATC_FAIL;
		}
	}
	else if(ATC_GET_CMD_TYPE == ATC_CMD_TYPE_READ)
	{
		BOOLEAN bOn;
		bOn = MNPHONE_ImsIsSet(dual_sys);

		sprintf((char *)g_rsp_str, "%s:%d",
                                    g_atc_info_table[AT_CMD_IMSEN].cmd_name->str_ptr,
                                    bOn);
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	}
	else
	{
		sprintf((char *)g_rsp_str, "%s",
                                    g_atc_info_table[AT_CMD_IMSEN].cmd_name->str_ptr);
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	}
	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessSPIMEI)
{
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;

    uint8 nSimID = ATC_CURRENT_SIM_ID_MUX;
    uint8 pImei[16] = {0};
    uint8 *imei = NULL;
    uint8 pImeiLen = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint16         nv_id_imei = 0;
    MN_IMEI_T      imei_arr;
    MN_IMEI_T      *imei_arr_ptr;
    uint8 bcd_ptr[8] = {0};
    uint8 bcd_ptr_len = 0;
    uint8 i = 0;
    MN_IMEISVN_T    nv_imeisv;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if (PARAM1_FLAG)
            {
                nSimID = PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                imei = PARA2.str_ptr->str_ptr;
                pImeiLen = PARA2.str_ptr->str_len;
            }

            if (PNULL == imei || strlen(imei) != 15)
            {
                status = S_ATC_FAIL;
                break;
            }

        	if (nSimID == MN_DUAL_SYS_1)
        	{
        		nv_id_imei = NV_ID_IMEI;
        	}
        	else if (nSimID == MN_DUAL_SYS_2)
        	{
        		nv_id_imei = (uint16)NV_IMEI1;
        	}
            else
            {
    			status = S_ATC_FAIL;
                break;
            }

            if (!IMEITransfromStrToBcd(bcd_ptr, &bcd_ptr_len, imei, pImeiLen))
            {
    			status = S_ATC_FAIL;
                break;
            }

    		imei_arr_ptr = (MN_IMEI_T*)(bcd_ptr);
    		if (NVERR_NONE != NVITEM_UpdateCali(nv_id_imei, sizeof(MN_IMEI_T), imei_arr_ptr))
    		{
    			status = S_ATC_FAIL;
                break;
    		}

            if (!MNNV_GetIMEISVNEx(dual_sys, &nv_imeisv))
            {
                SCI_TRACE_LOW("SIMAT%d:SIMAT_GetIMEISV fail to get IMEISV.",dual_sys);
            }

            if (FALSE == nv_imeisv.svn_flag)
            {
                nv_imeisv.svn = 0x01;
                SCI_TRACE_LOW("CFW_GetIMEIFromAP get imeisv error");
            }

            SCI_MEMCPY(pImei, imei, pImeiLen);
            pImei[14] = nv_imeisv.svn/10+'0';
            pImei[15] = nv_imeisv.svn%10+'0';
            CFW_EmodSaveIMEI(pImei, 16, nSimID);

            status = S_ATC_SUCCESS;
            break;
        case ATC_CMD_TYPE_READ:
        	if (nSimID == MN_DUAL_SYS_1)
        	{
        		nv_id_imei = NV_ID_IMEI;
        	}
        	else if (nSimID == MN_DUAL_SYS_2)
        	{
        		nv_id_imei = (uint16)NV_IMEI1;
        	}
            else
            {
    			status = S_ATC_FAIL;
                break;
            }

    		if (NVERR_NONE != EFS_NvitemRead(nv_id_imei, sizeof(MN_IMEI_T), imei_arr))
    		{
    			status = S_ATC_FAIL;
                break;
    		}

            IMEITransformBcdToStr((uint8*)imei_arr, sizeof(imei_arr), pImei);
            sprintf((char *)g_rsp_str, "%s", pImei);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            status = S_ATC_SUCCESS;
            break;
        default:
            status = S_ATC_FAIL;
            break;
    }
    return status;
}

AT_CMD_FUNC(ATC_ProcessSFUN)
{
#if defined(_ATC_UIX8910_ENABLE_)
    ATC_STATUS    status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if (PARAM1_FLAG)
        {
            switch(PARA1.num)
            {
            case ATC_POWER_OFF_SIM_PS:
                if (!s_is_ps_activate[dual_sys])
                {
                    return S_ATC_SUCCESS;
                }

#ifndef ATC_SAT_ENABLE
                //Initialise the variable of STK
                //ATC_InitSimStkVar(dual_sys);
#endif

                MNPHONE_DeactivePsEx(dual_sys);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                                  (uint16)APP_MN_DEACTIVE_PS_CNF, 0xff, ATC_NO_DOMAIN);
                s_is_ps_activate[dual_sys]   = FALSE;
                g_is_ps_deactivate[dual_sys] = TRUE;
                atc_config_ptr->cfun_status[dual_sys] = ATC_FUN_DEACT_SIM_PS;
                break;
            case ATC_POWER_ON_PS:
                if (s_is_ps_activate[dual_sys])
                {
                    return S_ATC_SUCCESS;
                }

                MNPHONE_PowerOnPsEx(dual_sys);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                                  (uint16)APP_MN_PS_POWER_ON_CNF, 0xff, ATC_NO_DOMAIN);
                s_is_ps_activate[dual_sys]   = TRUE;
                atc_config_ptr->cfun_status[dual_sys] = ATC_FUN_ACT_PS;
                break;
            case ATC_POWER_OFF_PS:
                if (!s_is_ps_activate[dual_sys])
                {
                    return S_ATC_SUCCESS;
                }
                MNPHONE_PowerOffPsEx(dual_sys);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                                  (uint16)APP_MN_PS_POWER_OFF_CNF, 0xff, ATC_NO_DOMAIN);
                s_is_ps_activate[dual_sys]   = FALSE;
                atc_config_ptr->cfun_status[dual_sys] = ATC_FUN_DEACT_PS;
                break;
            case ATC_POWER_ON_SIM_ONLY:
                MNPHONE_PowerOnSimEx(dual_sys, FALSE);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                                  (uint16)APP_MN_SIM_POWER_ON_CNF, 0xff, ATC_NO_DOMAIN);
                atc_config_ptr->cfun_status[dual_sys] = ATC_FUN_ACT_SIM;
                atc_config_ptr->is_sim_init[dual_sys] = TRUE;
                break;
            case ATC_POWER_ON_SIM_NORMAL:
                MNPHONE_PowerOnSimEx(dual_sys, TRUE);// init sms and pb
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                                  (uint16)APP_MN_SIM_POWER_ON_CNF, 0xff, ATC_NO_DOMAIN);
                atc_config_ptr->cfun_status[dual_sys] = ATC_FUN_ACT_SIM;
                atc_config_ptr->is_sim_init[dual_sys] = TRUE;
                break;
            case ATC_POWER_OFF_SIM:
#if 0
                if (s_is_ps_activate[dual_sys])
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }

#ifndef ATC_SAT_ENABLE
                Initialise the variable of STK
                ATC_InitSimStkVar(dual_sys);
#endif
#endif

                MNPHONE_PowerOffSimEx(dual_sys);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                                  (uint16)APP_MN_SIM_POWER_OFF_CNF, 0xff, ATC_NO_DOMAIN);
                atc_config_ptr->cfun_status[dual_sys] = ATC_FUN_DEACT_SIM;
                break;
            default:
                status = ERR_INVALID_INDEX;
                break;
            }
        }
        else
        {
            status = ERR_INVALID_INDEX;
        }
        break;

    case ATC_CMD_TYPE_READ:
        sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SFUN].cmd_name->str_ptr,
                atc_config_ptr->cfun_status[dual_sys]);
        ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        break;

    default:
        return ERR_OPERATION_NOT_ALLOWED;
    }

    return status;

#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessSPTESTMODEM)
{
#if defined(_ATC_UIX8910_ENABLE_)
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    uint8 uMode = 0;
    uint8 nRat = 0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if (PARAM1_FLAG && PARAM2_FLAG)
            {
                uMode = PARA1.num;
                nRat = PARA2.num;
            }
            else
            {
                return S_ATC_FAIL;
            }

            switch(uMode)
            {

                case TD_LTE_ONLY_MODE:
                case LTEFDD_ONLY_MODE:
                case TD_LTEFDD_LTE_DUAL_MODE:
                    CFW_NWSetRat(4, dual_sys);
                    status = S_ATC_SUCCESS;
                    break;

                case TD_LTE_OR_TD_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE:
                case LTE_AND_GSM_MODE:
                    CFW_NWSetRat(0, dual_sys);
                    status = S_ATC_SUCCESS;
                    break;

                case GSM_ONLY_MODE:
                case GSM_ONLY_AND_MASTER_CARD:
                    CFW_NWSetRat(2, dual_sys);
                    status = S_ATC_SUCCESS;
                    break;

                case SV_LTE_TEST_MODE:
                case LTE_IDD_OR_W_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE:
                case TD_LTE_OR_W_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE:
                case TD_LTE_OR_LTE_FDD_OR_W_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE:
                case TD_LTE_OR_LTEFDD_OR_TD_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE:
                case TD_LTE_OR_LTEFDD_OR_TD_OR_W_OR_GSM_CSFB_MUL_DUAL_STANDBY_MODE:
                case W_ONLY_MODE:
                case TD_ONLY_MODE:
                case TG_DUAL_MODE:
                case WG_DUAL_MODE:
                case M_GSM_PREFER_IN_WG_DUAL_MODE:
                case WCDMA_OR_TD_ONLY_MODE:
                case WCDMA_ONLY_MASTER_CARD_MODE:
                case TD_ONLY_MASTER_CARD_MODE:
                case TG_OR_WG_MASTER_CARD_MODE:
                case LTE_AND_WCDMA_MODE:
                case NO_SIM_MODE:
                case TG_DUAL_MODE_OR_WG_DUAL_MODE:
                    return ERR_OPERATION_NOT_SUPPORTED;
            }
            break;
        default:
            status = S_ATC_FAIL;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessSPTEST)
{
#if defined(_ATC_UIX8910_ENABLE_)
    uint8 type = 0;
    uint8 flag = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T info;
    MN_PHONE_USER_SERVICE_SELECT_E user_service;
    uint8 *ptr = PNULL;
    MN_GPRS_EXT_QOS_INFO_T ext_qos = {0};
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    uint8 nFta = 0;
    uint8 protection = 0;
    uint8 uMode = 0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if (PARAM1_FLAG && PARAM2_FLAG)
            {
                type=( uint8 )PARA1.num;

                switch(type)
                {
                    case FTA_TEST:
                        nFta = (uint8)PARA2.num;
                        if (nFta > 2)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }
                        CFW_NwSetFTA(nFta, dual_sys);
                        status = S_ATC_SUCCESS;
                        break;

                    case TRAFFIC_CLASS_TEST:
#ifdef _GPRS_FULL_FUNCTION_ || _ATC_UIX8910_ENABLE_
                    {
                        ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;
                        BOOLEAN item_setted[13] = {0,0,0,0,1,0,0,0,0,0,0,0,0};

                        ext_qos.traffic_class=(MN_GPRS_TRAFFIC_CLASS_E)PARA2.num;
                        ret= MNGPRS_SetStorageExtReqQosWithItemEx(dual_sys, MN_GPRS_STORAGE_NV, DOWNLOAD_E, &ext_qos, item_setted);
                        if(ret!=ERR_MNGPRS_NO_ERR)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }
                    }
#endif
                        break;

                    case SERVICE_TYPE_TEST:
                        user_service=(MN_PHONE_USER_SERVICE_SELECT_E)PARA2.num+1;
                        MNPHONE_SetServiceTypeEx(dual_sys, user_service);
                        // uMode = (uint8) PARA2.num;
                        // MNPHONE_SetCEMode(uMode, dual_sys);
                        status = S_ATC_SUCCESS;
                        break;

                    case INTEGRALITY_TEST:
                        protection  = PARA2.num;
                        if (protection > 1)
                        {
                            status = ERR_OPERATION_NOT_SUPPORTED;
                            break;
                        }
                        CFW_NwSetnSecurityUsedFlag(protection, dual_sys);
                        status = S_ATC_SUCCESS;
                        break;

                    case RW_TEST:
                    case AOC_TEST:
                    case GSIM_SUPPORT_TEST:
                    case PS_VERSION_TEST:
                    case WAKEUP_CONDITION_TEST:
                    case DSP_LOG_TEST:
                    case FBAND_TEST:
                    case CELL_CAPABILITY_TEST:
                    case IS_VT_MT_CALL_SUPPORT_TEST:
                    case TRACE_TASK_SWITCH_TEST:
                    case TRACE_LEVEL_TEST:
                    case EMERGENCE_ACTIVATE_PS_FOR_PSIT_TEST:
                    case CLOSE_AUDIO_TEST:
                    case DISABLE_ENABLE_MAX_POWER_TEST:
                    case AGPS_TEST:
                    case PS_TASK_POINT_SWITCH_TEST:
                    case VOICE_CODEC_SWITCH_TEST:
                    case IMEI_IN_LTE:
                    case IMEI_IN_2G_OR_3G_TEST:
                    case IMEISV_IN_LTE_TEST:
                    case IMEISV_IN_2G_OR_3G_TEST:
                    case CONTROL_ENG_MODE_SWITCH_TEST:
                    case OPEN_LTE_FEATURE_IN_NON_SIM_TEST:
                    case IMS_TEST_FOR_SIP_MESSAGE_TEST:
                   case  LINK_ID_SETTING_TEST:
                        return ERR_OPERATION_NOT_SUPPORTED;

                    default:
                        break;
                }
            }
            else if  (PARAM1_FLAG && (!PARAM2_FLAG))
            {
                if((uint8 )PARA1.num == CELL_CAPABILITY_TEST)
                {
                    // get cell capability
                    info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

                    if(info.cell_capability.mbms_support)
                    {
                        flag = (1<<0);
                    }

                    if(info.cell_capability.hsdpa_support)
                    {
                        flag |= (1<<1);
                    }

                    if(info.cell_capability.hsupa_support)
                    {
                        flag |= (1<<2);
                    }

                    if(info.cell_capability.edge_support)
                    {
                        flag |= (1<<3);
                    }

                    sprintf((char *)g_rsp_str, "%s:%d,%d", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, CELL_CAPABILITY_TEST, flag);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                    return ERR_INVALID_INDEX;
                }
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:
#if 0
            //get fta flag
            ptr = g_rsp_str;

            //get INTEGRALITY flag
            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, FTA_TEST, 0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            //get INTEGRALITY flag
            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, INTEGRALITY_TEST, 0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            //get real network flag
            sprintf((char *)ptr, "%s:%d,%lu\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, RW_TEST, 0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            //em_get_aoc_nv_param(&aoc_vl);
            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, AOC_TEST, 0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            // get gsm support flag
            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, GSIM_SUPPORT_TEST, 0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            // get PS Version
            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, PS_VERSION_TEST, 0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            // get traffic class
            //ext_qos= MNGPRS_GetExtendQos(DOWNLOAD_E);
            MNGPRS_ReadStorageExtReqQosEx(dual_sys,MN_GPRS_STORAGE_NV, DOWNLOAD_E, &ext_qos);
            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, TRAFFIC_CLASS_TEST, ext_qos.traffic_class);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            // get service class
            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, SERVICE_TYPE_TEST, MNPHONE_GetServiceTypeEx(dual_sys));
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            // get cell capability
            info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

            if(info.cell_capability.mbms_support)
            {
                flag = (1<<0);
            }

            if(info.cell_capability.hsdpa_support)
            {
                flag |= (1<<1);
            }

            if(info.cell_capability.hsupa_support)
            {
                flag |= (1<<2);
            }

            if(info.cell_capability.edge_support)
            {
                flag |= (1<<3);
            }

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, CELL_CAPABILITY_TEST,flag);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            //wake up condition for dsp
            //em_get_wakeup_condition_for_dsp_stat(&s_item,0);
            sprintf((char *)ptr, "%s:%d,%lu\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, WAKEUP_CONDITION_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            //em_get_dsp_log_switch(&s_item,0);
            sprintf((char *)ptr, "%s:%d,%lu\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, DSP_LOG_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            //FBAND support flag
            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, FBAND_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, IS_VT_MT_CALL_SUPPORT_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, TRACE_TASK_SWITCH_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, TRACE_LEVEL_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, EMERGENCE_ACTIVATE_PS_FOR_PSIT_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, CLOSE_AUDIO_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, DISABLE_ENABLE_MAX_POWER_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, PS_TASK_POINT_SWITCH_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, VOICE_CODEC_SWITCH_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, IMEI_IN_2G_OR_3G_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, IMEISV_IN_LTE_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, IMEISV_IN_2G_OR_3G_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, CONTROL_ENG_MODE_SWITCH_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, OPEN_LTE_FEATURE_IN_NON_SIM_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, IMS_TEST_FOR_SIP_MESSAGE_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, LINK_ID_SETTING_TEST,0);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#endif
            status = S_ATC_SUCCESS;
            break;

        case ATC_CMD_TYPE_TEST:
            break;

        default:
            SCI_ASSERT(0);/*assert verified:Check_value */
            status = S_ATC_FAIL;
            break;
    }

    return status;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessSPDATA)
{
#if defined(_ATC_UIX8910_ENABLE_)
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    SCI_TRACE_LOW("ATC_ProcessSPDATA !!");
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            break;
        default:
            SCI_TRACE_LOW("ATC_ProcessSPDATA error!!");
            break;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessCEMODE)
{
#if defined(_ATC_UIX8910_ENABLE_)
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint8 mode;

    SCI_TRACE_LOW("ATC_ProcessCEMODE!!");

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
             if (PARAM1_FLAG)
            {
                mode = PARA1.num;
                MNPHONE_SetCEMode(mode, dual_sys);
            }
            break;
        case ATC_CMD_TYPE_READ:
            MNPHONE_GetCEMode(&mode, dual_sys);
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CEMODE].cmd_name->str_ptr, mode);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
         case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (0, 1, 2, 3)",g_atc_info_table[AT_CMD_CEMODE].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        default:
            SCI_TRACE_LOW("ATC_ProcessCEMODE error!!");
            break;
    }

    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessCANXP)
{
#if defined(_ATC_UIX8910_ENABLE_)
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;

    uint8 nMode = 0;
    uint8 nPath = 0;
    uint8 nCtrl = 0;
    uint8 *nParam = NULL;
    uint param_len = 0;

    uint8 resultCode = 0;
    uint8 hasMsg = 0;
    uint8 resultMsg[900]   = {0};

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
            goto SUCCESS_WITH_CACANXP_REPORT;
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                nMode = PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                nPath = PARA2.num;
            }

            if (PARAM3_FLAG)
            {
                nCtrl = PARA3.num;
            }

            if (PARAM4_FLAG)
            {
                nParam = PARA4.str_ptr->str_ptr;
                param_len = PARA4.str_ptr->str_len;
            }

            CSW_SetAudioZspNxpCalibParam(&resultCode, &hasMsg, resultMsg, nMode, nPath, nCtrl, nParam, param_len);
            if (resultCode != 0)
            {
                sprintf((char *)g_rsp_str,"ERROR %d",resultCode);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = S_ATC_SUCCESS;
                if (hasMsg)
                {
                    sprintf((char *)g_rsp_str,"%s",resultMsg);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
            }
            SCI_MEMSET(resultMsg, 0x00, 900);
            break;
        }
        default:
        {
            return S_ATC_FAIL;
        }
    }
    return status;
SUCCESS_WITH_CACANXP_REPORT:
    sprintf((char *)g_rsp_str,"+CANXP");
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}


AT_CMD_FUNC(ATC_Processxor_SYSCONFIG)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
	SCI_TraceLow("ATC_Processxor_SYSCONFIG");
	if (ATC_GET_CMD_TYPE == ATC_CMD_TYPE_SET)
	{
		if (PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG && PARAM4_FLAG)
		{
			uint8 nMode = PARA1.num;
			uint8 nAcqorder = PARA2.num;
			uint8 nRoam = PARA3.num;
			uint8 nSrvdomain = PARA4.num;
			uint8 nOldMode = nMode;
			uint8 nOldAcrorder = nAcqorder;
			uint8 nOldRoam = nRoam;
			uint8 nOldSrvdomain = nSrvdomain;

			MNPHONE_GetSysConfig(&nOldMode, &nOldAcrorder, &nOldRoam, &nOldSrvdomain, dual_sys);
			SCI_TraceLow("ATC_Processxor_SYSCONFIG, nMode:%d,nOldMode:%d,s_is_ps_activate is %d", nMode, nOldMode, s_is_ps_activate[dual_sys]);
			if(MNPHONE_SysConfig(nMode, nAcqorder, nRoam, nSrvdomain, dual_sys) == 0)
			{

				if(nMode != nOldMode && s_is_ps_activate[dual_sys] == TRUE)
				{
					switch(nMode)
					{
						case 2:
							if(nOldMode != 17 && nOldMode != 16)
							{
								MNPHONE_ChangeRat(0, dual_sys);
							}
							break;
						case 12:
							if(nOldMode != 16)
							{
								MNPHONE_ChangeRat(4, dual_sys);
							}
							break;
						case 13:
							if(nOldMode != 16)
							{
								MNPHONE_ChangeRat(2, dual_sys);
							}
							break;
						case 17:
							if(nOldMode != 2 && nOldMode != 16)
							{
								MNPHONE_ChangeRat(0, dual_sys);
							}
							break;
						default:
							break;
					}
				}
				return S_ATC_SUCCESS;
			}
			else
			{
				return S_ATC_FAIL;
			}
		}
		else
		{
			return S_ATC_FAIL;
		}
	}
	else if(ATC_GET_CMD_TYPE == ATC_CMD_TYPE_READ)
	{
		uint8 nMode;
		uint8 nAcqorder;
		uint8 nRoam;
		uint8 nSrvdomain;
		if(MNPHONE_GetSysConfig(&nMode, &nAcqorder, &nRoam, &nSrvdomain, dual_sys) == 0)
		{
			sprintf((char *)g_rsp_str, "^SYSCONFIG:%d,%d,%d,%d",
                                    /*g_atc_info_table[AT_CMD_xor_SYSCONFIG].cmd_name->str_ptr,*/
                                    nMode,nAcqorder,nRoam,nSrvdomain);
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			return S_ATC_SUCCESS;
		}
		else
		{
			return S_ATC_FAIL;
		}
	}
    else if(ATC_GET_CMD_TYPE == ATC_CMD_TYPE_TEST)
    {
        sprintf((char *)g_rsp_str, "^SYSCONFIG: (2,12-17),(0-6),(0-2),(0-4)");
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
		return S_ATC_SUCCESS;
    }
	else
	{
		sprintf((char *)g_rsp_str, "%s",
                                    g_atc_info_table[AT_CMD_xor_SYSCONFIG].cmd_name->str_ptr);
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	}
	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif

}
typedef struct
{
	uint8 value0;
	uint8 value1;
	uint8 value2;
	uint8 value3;
	uint8 value4;
	uint8 value5;
	uint8 value6;
	uint8 value7;
	uint8 value8;
	uint8 value9;
	uint8 value10;
	uint8 value11;
	uint8 value12;
	uint8 value13;
	uint8 value14;
	uint8 value15;
	uint8 value16;
	uint8 value17;
	uint8 value18;
	uint8 value19;
	uint8 value20;
	uint8 value21;
	uint8 value22;
	uint8 value23;
	uint8 value24;
	uint8 value27;
	uint8 value28;
	uint8 value29;
	uint8 value30;
	uint8 value31;
	uint8 value32;
	uint8 value33;
	uint8 value34;
	uint8 value35;
	uint8 value36;
	uint8 value37;
	uint8 value38;
	uint8 value39;
	uint8 rsrp_flag25;
	uint8 rsrp25;
	uint8 rsrq_flag25;
	uint8 rsrq25;
	uint8 rsrp_flag26;
	uint8 rsrp26;
	uint8 rsrq_flag26;
	uint8 rsrq26;
	uint16 band31;
	uint16 band32;
	uint8 nArfcn31;
	uint8 nArfcn32;
	uint16 arfcn31[3];
	uint16 arfcn32[3];
	uint8 nArfcn36;
	uint32 arfcn36[8];
	uint16 band38;
	uint16 arfcn38[3];

}SPLTEDUMMYPARA_T;


SPLTEDUMMYPARA_T gSpLteDummyPara= {0,};

AT_CMD_FUNC(ATC_ProcessSPLTEDUMMYPARA)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
	SCI_TraceLow("ATC_ProcessSPLTEDUMMYPARA");
	if (ATC_GET_CMD_TYPE == ATC_CMD_TYPE_SET)
	{
		if(PARAM1_FLAG && PARAM2_FLAG)
		{
			uint8 nFunId = PARA1.num;
			uint8 nValue = PARA2.num;
			uint8 rsrp_flag = 0;
			uint8 rsrp = 0;
			uint8 rsrq_flag = 0;
			uint8 rsrq = 0;
			uint16 band = 0;
			uint8 nArfcn = 0;
			uint16 arfcn[8] = {0,};
			uint32 lteArfcn[8] = {0};
			MNPHONE_SpLteDummyPara_t para;
			memset(&para, 0, sizeof(para));
			SCI_TRACE_LOW("nFunId : %d",nFunId);
			switch(nFunId)
			{
				case 0:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					gSpLteDummyPara.value0 = nValue;
					break;
				#if 0
				case 1:
					if(nValue > 4)
						return S_ATC_FAIL;
					gSpLteDummyPara.value1 = nValue;
					break;
				case 2:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					gSpLteDummyPara.value2 = nValue;
					break;
				#endif
				case 3:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					gSpLteDummyPara.value3 = nValue;
					break;
				case 4:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					gSpLteDummyPara.value4 = nValue;
					break;
				case 5:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					gSpLteDummyPara.value5 = nValue;
					break;
				case 10:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					gSpLteDummyPara.value10 = nValue;
					break;
				case 13:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					gSpLteDummyPara.value13 = nValue;
					break;
				case 18:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					gSpLteDummyPara.value18 = nValue;
					break;
				case 19:
					if(nValue < 1 || nValue > 97)
						return S_ATC_FAIL;
					gSpLteDummyPara.value19 = nValue;
					break;
				case 20:
					if(nValue < 1 || nValue > 34)
						return S_ATC_FAIL;
					gSpLteDummyPara.value20 = nValue;
					break;
				case 23:
					if(nValue < 1 || nValue > 63)
						return S_ATC_FAIL;
					gSpLteDummyPara.value23 = nValue;
					break;
				case 24:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					gSpLteDummyPara.value24 = nValue;
					break;
				case 25:
					if(PARAM3_FLAG && PARAM4_FLAG && PARAM5_FLAG)
					{
						rsrp_flag = PARA2.num;
						rsrp = PARA3.num;
						rsrq_flag = PARA4.num;
						rsrq = PARA5.num;
						if((rsrp_flag != 0 && rsrp_flag != 1)
							|| (rsrp > 114)
							|| (rsrq_flag != 0 && rsrq_flag != 1)
							|| (rsrq > 76))
						{
							SCI_TRACE_LOW("case 25 param range error, rsrp_flag is %d,rsrp is %d,rsrq_flag is %d,rsrq is %d",rsrp_flag,rsrp,rsrq_flag,rsrq);
							return S_ATC_FAIL;
						}
						gSpLteDummyPara.rsrp_flag25 = rsrp_flag;
						gSpLteDummyPara.rsrp25 = rsrp;
						gSpLteDummyPara.rsrq_flag25 = rsrq_flag;
						gSpLteDummyPara.rsrq25 = rsrq;
					}
					else
					{
						SCI_TRACE_LOW("case 25 param error");
						return S_ATC_FAIL;
					}
					break;
				case 26:
					if(PARAM3_FLAG && PARAM4_FLAG && PARAM5_FLAG)
					{
						rsrp_flag = PARA2.num;
						rsrp = PARA3.num;
						rsrq_flag = PARA4.num;
						rsrq = PARA5.num;
						if((rsrp_flag != 0 && rsrp_flag != 1)
							|| (rsrp > 114)
							|| (rsrq_flag != 0 && rsrq_flag != 1)
							|| (rsrq > 76))
						{
							return S_ATC_FAIL;
						}
						gSpLteDummyPara.rsrp_flag26 = rsrp_flag;
						gSpLteDummyPara.rsrp26 = rsrp;
						gSpLteDummyPara.rsrq_flag26 = rsrq_flag;
						gSpLteDummyPara.rsrq26 = rsrq;
					}
					else
					{
						return S_ATC_FAIL;
					}
					break;
				case 27:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					gSpLteDummyPara.value27 = nValue;
					break;
				case 31:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					if(PARAM3_FLAG && PARAM4_FLAG && PARAM5_FLAG)
					{
						uint8 i;
						band = PARA3.num;
						nArfcn = PARA4.num;
						if(nArfcn < 1 || nArfcn > 3)
							return S_ATC_FAIL;
						if(band != 1800 && band != 1900)
							return S_ATC_FAIL;

						arfcn[0] = PARA5.num;

						if(nArfcn>= 2 && PARAM6_FLAG)
							arfcn[1] = PARA6.num;
						else if(nArfcn>= 2)
							return S_ATC_FAIL;

						if(nArfcn >= 3 && PARAM7_FLAG)
							arfcn[2] = PARA7.num;
						else if(nArfcn>= 3)
							return S_ATC_FAIL;

						for(i = 0; i < nArfcn; i++)
						{
							if(arfcn[i] > 1023)
								return S_ATC_FAIL;
						}

						SCI_TRACE_LOW("band :%d,nArfcn:%d,arfcn:%d,%d,%d",band,nArfcn,arfcn[0],arfcn[1],arfcn[2]);
						gSpLteDummyPara.value31 = nValue;
						gSpLteDummyPara.band31 = band;
						if(band == 1800)
						{
							band = 0;
						}
						else if(band == 1900)
						{
							band = 1;
						}
						gSpLteDummyPara.nArfcn31 = nArfcn;
						memcpy(gSpLteDummyPara.arfcn31, arfcn, sizeof(gSpLteDummyPara.arfcn31));
					}
					else
					{
						return S_ATC_FAIL;
					}
					break;
				case 32:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					if(PARAM3_FLAG && PARAM4_FLAG && PARAM5_FLAG)
					{
						uint8 i;
						band = PARA3.num;
						nArfcn = PARA4.num;
						if(nArfcn < 1 || nArfcn > 3)
							return S_ATC_FAIL;
						if(band != 1800 && band != 1900)
							return S_ATC_FAIL;

						arfcn[0] = PARA5.num;

						if(nArfcn>= 2 && PARAM6_FLAG)
							arfcn[1] = PARA6.num;
						else if(nArfcn>= 2)
							return S_ATC_FAIL;

						if(nArfcn >= 3 && PARAM7_FLAG)
							arfcn[2] = PARA7.num;
						else if(nArfcn>= 3)
							return S_ATC_FAIL;

						for(i = 0; i < nArfcn; i++)
						{
							if(arfcn[i] > 1023)
								return S_ATC_FAIL;
						}

						SCI_TRACE_LOW("band :%d,nArfcn:%d,arfcn:%d,%d,%d",band,nArfcn,arfcn[0],arfcn[1],arfcn[2]);
						gSpLteDummyPara.value32 = nValue;
						gSpLteDummyPara.band32 = band;
						if(band == 1800)
						{
							band = 0;
						}
						else if(band == 1900)
						{
							band = 1;
						}
						gSpLteDummyPara.nArfcn32 = nArfcn;
						memcpy(gSpLteDummyPara.arfcn32, arfcn, sizeof(gSpLteDummyPara.arfcn32));
					}
					else
					{
						return S_ATC_FAIL;
					}
					break;
				case 35:
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					gSpLteDummyPara.value35 = nValue;
					break;
					break;
				case 36:
					#if 0
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					if(PARAM3_FLAG && PARAM4_FLAG)
					{
						int i;
						nArfcn = PARA3.num;
						if(nArfcn < 1 || nArfcn > 8)
							return S_ATC_FAIL;

						lteArfcn[0] = PARA4.num;
						if(nArfcn >= 2&&PARAM5_FLAG)
							lteArfcn[1] = PARA5.num;
						else if(nArfcn>= 2)
							return S_ATC_FAIL;

						if(nArfcn >= 3&&PARAM6_FLAG)
							lteArfcn[2] = PARA6.num;
						else if(nArfcn>= 3)
							return S_ATC_FAIL;

						if(nArfcn >= 4&&PARAM7_FLAG)
							lteArfcn[3] = PARA7.num;
						else if(nArfcn>= 4)
							return S_ATC_FAIL;

						if(nArfcn >= 5&&PARAM8_FLAG)
							lteArfcn[4] = PARA8.num;
						else if(nArfcn>= 5)
							return S_ATC_FAIL;

						if(nArfcn >= 6&&PARAM9_FLAG)
							lteArfcn[5] = PARA9.num;
						else if(nArfcn>= 6)
							return S_ATC_FAIL;

						if(nArfcn >= 7&&PARAM10_FLAG)
							lteArfcn[6] = PARA10.num;
						else if(nArfcn>= 7)
							return S_ATC_FAIL;

						if(nArfcn >= 8&&PARAM11_FLAG)
							lteArfcn[7] = PARA11.num;
						else if(nArfcn>= 8)
							return S_ATC_FAIL;

						for(i = 0; i < nArfcn; i++)
						{
							if(lteArfcn[i] > 262143)
								return S_ATC_FAIL;
						}

						gSpLteDummyPara.value36 = nValue;
						gSpLteDummyPara.nArfcn36 = nArfcn;
						memcpy(gSpLteDummyPara.arfcn36, lteArfcn, sizeof(gSpLteDummyPara.arfcn36));
					}
					else
					#endif
					{
						return S_ATC_FAIL;
					}
					break;
				case 38:
					#if 0
					if(nValue !=0 && nValue != 1)
						return S_ATC_FAIL;
					if(PARAM3_FLAG && PARAM4_FLAG && PARAM5_FLAG && PARAM6_FLAG)
					{
						band = PARA3.num;
						if(band != 1800 && band != 1900)
							return S_ATC_FAIL;

						arfcn[0] = PARA4.num;
						arfcn[1] = PARA5.num;
						arfcn[2] = PARA6.num;

						gSpLteDummyPara.value38 = nValue;
						gSpLteDummyPara.band38 = band;
						if(band == 1800)
						{
							band = 0;
						}
						else if(band == 1900)
						{
							band = 1;
						}
						memcpy(gSpLteDummyPara.arfcn38, arfcn, sizeof(gSpLteDummyPara.arfcn38));
					}
					else
					#endif
					{
						return S_ATC_FAIL;
					}
					break;
				case 1:
				case 2:
				case 6:
				case 7:
				case 8:
				case 9:
				case 11:
				case 12:
				case 14:
				case 15:
				case 16:
				case 17:
				case 21:
				case 22:
				case 28:
				case 29:
				case 30:
				case 33:
				case 34:
				case 37:
				case 39:
				default:
					return S_ATC_FAIL;
			}
			para.atFuncIndex = nFunId;
			para.value = nValue;
			para.gsmBand = band;
			memcpy(para.gsmArfcn, arfcn, sizeof(para.gsmArfcn));
			para.gsmNum = nArfcn;
			memcpy(para.lteArfcn, lteArfcn, sizeof(para.lteArfcn));
			para.lteNum = nArfcn;
			para.rsrp = rsrp;
			para.rsrp_flag = rsrp_flag;
			para.rsrq = rsrq;
			para.rsrq_flag =rsrq_flag;

			SCI_TRACE_LOW("para.atFuncIndex:%d",para.atFuncIndex);
			SCI_TRACE_LOW("para.value:%d",para.value);
			SCI_TRACE_LOW("para.rsrp_flag:%d",para.rsrp_flag);
			SCI_TRACE_LOW("para.rsrp:%d",para.rsrp);
			SCI_TRACE_LOW("para.rsrq_flag:%d",para.rsrq_flag);
			SCI_TRACE_LOW("para.rsrq :%d",para.rsrq );
			SCI_TRACE_LOW("para.gsmBand:%d",para.gsmBand);
			SCI_TRACE_LOW("para.gsmNum:%d",para.gsmNum);
			SCI_TRACE_LOW("para.gsmArfcn:%d,%d,%d",para.gsmArfcn[0],para.gsmArfcn[1],para.gsmArfcn[2]);
			SCI_TRACE_LOW("para.lteNum:%d",para.lteNum);
			SCI_TRACE_LOW("para.lteArfcn:%d,%d,%d,%d,%d,%d,%d,%d",para.lteArfcn[0],para.lteArfcn[1],para.lteArfcn[2],para.lteArfcn[3],para.lteArfcn[4],para.lteArfcn[5],para.lteArfcn[6],para.lteArfcn[7]);

			//SCI_TRACE_BUF("para:", &para ,sizeof(para));
			MNPHONE_SpLteDummyPara(para, dual_sys);
		}
		else if(PARAM1_FLAG)
		{
			uint8 nFunId = PARA1.num;
			uint8 nValue;
			uint8 rsrp_flag;
			uint8 rsrp;
			uint8 rsrq_flag;
			uint8 rsrq;
			uint8 band;
			uint8 nArfcn;
			uint16 arfcn[3] = {0,};
			uint32 lteArfcn[8] = {0,};

			switch(nFunId)
			{
				case 0:
					nValue = gSpLteDummyPara.value0;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				#if 0
				case 1:
					nValue = gSpLteDummyPara.value1;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 2:
					nValue = gSpLteDummyPara.value2;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				#endif
				case 3:
					nValue = gSpLteDummyPara.value3;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 4:
					nValue = gSpLteDummyPara.value4;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 5:
					nValue = gSpLteDummyPara.value5;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 10:
					nValue = gSpLteDummyPara.value10;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 13:
					nValue = gSpLteDummyPara.value13;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 18:
					nValue = gSpLteDummyPara.value18;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 19:
					nValue = gSpLteDummyPara.value19;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 20:
					nValue = gSpLteDummyPara.value20;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 23:
					nValue = gSpLteDummyPara.value23;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 24:
					nValue = gSpLteDummyPara.value24;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 25:
					rsrp_flag = gSpLteDummyPara.rsrp_flag25 ;
					rsrp = gSpLteDummyPara.rsrp25;
					rsrq_flag = gSpLteDummyPara.rsrq_flag25;
					rsrq = gSpLteDummyPara.rsrq25;
					sprintf((char *)g_rsp_str, "%s:%d,%d,%d,%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, rsrp_flag,rsrp,rsrq_flag,rsrq);
					break;

				case 26:
					rsrp_flag = gSpLteDummyPara.rsrp_flag26;
					rsrp = gSpLteDummyPara.rsrp26;
					rsrq_flag = gSpLteDummyPara.rsrq_flag26;
					rsrq = gSpLteDummyPara.rsrq26;
					sprintf((char *)g_rsp_str, "%s:%d,%d,%d,%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, rsrp_flag,rsrp,rsrq_flag,rsrq);
					break;
				case 27:
					nValue = gSpLteDummyPara.value27;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 31:
					nValue = gSpLteDummyPara.value31;
					band = gSpLteDummyPara.band31;
					nArfcn = gSpLteDummyPara.nArfcn31;
					memcpy(arfcn, gSpLteDummyPara.arfcn31, sizeof(gSpLteDummyPara.arfcn31));
					sprintf((char *)g_rsp_str, "%s:%d,%d,%d,%d,%d,%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue, band, nArfcn, arfcn[0], arfcn[1], arfcn[2]);
					break;
				case 32:
					nValue = gSpLteDummyPara.value32;
					band = gSpLteDummyPara.band32;
					nArfcn = gSpLteDummyPara.nArfcn32;
					memcpy(arfcn, gSpLteDummyPara.arfcn32, sizeof(gSpLteDummyPara.arfcn32));
					sprintf((char *)g_rsp_str, "%s:%d,%d,%d,%d,%d,%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue, band, nArfcn, arfcn[0], arfcn[1], arfcn[2]);
					break;
				case 35:
					nValue = gSpLteDummyPara.value35;
					sprintf((char *)g_rsp_str, "%s:%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue);
					break;
				case 36:
					#if 0
					nValue = gSpLteDummyPara.value36;
					nArfcn = gSpLteDummyPara.nArfcn36;
					memcpy(lteArfcn, gSpLteDummyPara.arfcn36, sizeof(gSpLteDummyPara.arfcn36));
					sprintf((char *)g_rsp_str, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue, nArfcn, lteArfcn[0], lteArfcn[1], lteArfcn[2],lteArfcn[3], lteArfcn[4], lteArfcn[5],lteArfcn[6], lteArfcn[7]);
					break;
					#else
					return S_ATC_FAIL;
					#endif
				case 38:
					#if 0
					nValue = gSpLteDummyPara.value38;
					band = gSpLteDummyPara.band38;
					memcpy(arfcn, gSpLteDummyPara.arfcn38, sizeof(gSpLteDummyPara.arfcn38));
					sprintf((char *)g_rsp_str, "%s:%d,%d,%d,%d,%d,%d,%d",
						g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr, nFunId, nValue, band, arfcn[0], arfcn[1], arfcn[2]);
					break;
					#else
					return S_ATC_FAIL;
					#endif
				case 1:
				case 2:
				case 6:
				case 7:
				case 8:
				case 9:
				case 11:
				case 12:
				case 14:
				case 15:
				case 16:
				case 17:
				case 21:
				case 22:
				case 28:
				case 29:
				case 30:
				case 33:
				case 34:
				case 37:
				case 39:
				default:
					return S_ATC_FAIL;
			}

			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
		}
		else
		{
			return S_ATC_FAIL;
		}
	}
	else
	{
		sprintf((char *)g_rsp_str, "%s",
                                    g_atc_info_table[AT_CMD_SPLTEDUMMYPARA].cmd_name->str_ptr);
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	}
	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif

}

typedef struct
{
	uint8 nEnable0;
	uint8 nEnable1;
	uint8 nEnable2;
	uint8 nEnable3;
	uint8 nEnable4;
	uint8 nEnable5;
	uint8 nEnable6;
	uint8 nEnable7;
	uint8 nEnable8;
	uint8 nEnable9;
	uint8 nEnable10;
	uint8 nEnable11;
	uint8 nEnable12;
	uint8 nEnable13;
	uint8 nEnable14;
	uint16 nValue1_2;
	uint16 nValue1_3;
	uint16 nValue2_3;
	uint16 nValue1_5;
	uint16 nValue1_6;
	uint16 nValue1_7;
	uint16 nValue1_8;
	uint16 nValue1_9;
	uint16 nValue1_10;
	uint16 nValue1_11;
	uint16 nValue1_12;
	uint16 nValue2_12;
	uint16 nValue1_13;
	uint16 nValue1_14;
}RRTPARAM_T;

RRTPARAM_T gRrtParam;

AT_CMD_FUNC(ATC_ProcessRRTMPARAM)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

	SCI_TRACE_LOW("ATC_ProcessRRTMPARAM");

	switch (ATC_GET_CMD_TYPE)
	{
		case ATC_CMD_TYPE_SET:
			if (PARAM1_FLAG && PARAM2_FLAG)
			{
				uint8 nIndex = PARA1.num;
				uint8 nEnable = PARA2.num;
				uint16 nValue1 = 0;
				uint16 nValue2 = 0;
				MNPHONE_RrtmParam_t para;
				switch(nIndex)
				{
					case 0:
						gRrtParam.nEnable0 = nEnable;
						sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						break;
					case 1:
						gRrtParam.nEnable1 = nEnable;
						sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						break;
					case 2:
						if(nEnable == 1 && PARAM3_FLAG)
						{
							nValue1 = PARA3.num;
							if(nValue1 != 0 && nValue1 != 1)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable2 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_2,nValue1);

							gRrtParam.nValue1_2 = nValue1;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable2 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					case 3:
						if(nEnable == 1 && PARAM3_FLAG && PARAM4_FLAG)
						{
							nValue1 = PARA3.num;
							nValue2 = PARA4.num;
							if(nValue2 > 511)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable3 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_2,nValue1, gRrtParam.nValue2_3, nValue2);

							gRrtParam.nValue1_3 = nValue1;
							gRrtParam.nValue2_3 = nValue2;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable3 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					case 4:
						gRrtParam.nEnable4= nEnable;
						sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						break;
					case 5:
						if(nEnable == 1 && PARAM3_FLAG)
						{
							nValue1 = PARA3.num;
							if(nValue1 > 63)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable5 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_5,nValue1);

							gRrtParam.nValue1_5 = nValue1;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable5 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					case 6:
						if(nEnable == 1 && PARAM3_FLAG)
						{
							nValue1 = PARA3.num;
							if(nValue1 > 63)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable6 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_6,nValue1);

							gRrtParam.nValue1_6 = nValue1;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable6 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					case 7:
						if(nEnable == 1 && PARAM3_FLAG)
						{
							nValue1 = PARA3.num;
							if(nValue1 > 7)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable7 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_7,nValue1);

							gRrtParam.nValue1_7 = nValue1;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable7 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					case 8:
						if(nEnable == 1 && PARAM3_FLAG)
						{
							nValue1 = PARA3.num;
							if(nValue1 > 7)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable8 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_8,nValue1);

							gRrtParam.nValue1_8 = nValue1;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable8 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					case 9:
						if(nEnable == 1 && PARAM3_FLAG)
						{
							nValue1 = PARA3.num;
							if(nValue1 > 63)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable9 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_9,nValue1);

							gRrtParam.nValue1_9 = nValue1;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable9 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					case 10:
						if(nEnable == 1 && PARAM3_FLAG)
						{
							nValue1 = PARA3.num;
							if(nValue1 > 63)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable10 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_10,nValue1);

							gRrtParam.nValue1_10 = nValue1;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable10 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					case 11:
						if(nEnable == 1 && PARAM3_FLAG)
						{
							nValue1 = PARA3.num;
							if(nValue1 > 7)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable11 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_11,nValue1);

							gRrtParam.nValue1_11 = nValue1;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable11 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					case 12:
						if(nEnable == 1 && PARAM3_FLAG && PARAM4_FLAG)
						{
							nValue1 = PARA3.num;
							nValue2 = PARA4.num;
							if(nValue2 > 511)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable12 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_12,nValue1, gRrtParam.nValue2_12, nValue2);

							gRrtParam.nValue1_12 = nValue1;
							gRrtParam.nValue2_12 = nValue2;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable12 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					case 13:
						if(nEnable == 1 && PARAM3_FLAG)
						{
							nValue1 = PARA3.num;
							if(nValue1 > 15)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable13 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_13,nValue1);

							gRrtParam.nValue1_13 = nValue1;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable13 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					case 14:
						if(nEnable == 1 && PARAM3_FLAG)
						{
							nValue1 = PARA3.num;
							if(nValue1 > 15)
							{
								return S_ATC_FAIL;
							}
							gRrtParam.nEnable14 = nEnable;

							sprintf((char *)g_rsp_str, "%s:%d,%d,%d->%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable, gRrtParam.nValue1_14,nValue1);

							gRrtParam.nValue1_14 = nValue1;
						}
						else if(nEnable == 0)
						{
							gRrtParam.nEnable14 = nEnable;
							sprintf((char *)g_rsp_str, "%s:%d,%d",
								g_atc_info_table[AT_CMD_RRTMPARAM].cmd_name->str_ptr, nIndex, nEnable);
						}
						else
						{
							return S_ATC_FAIL;
						}
						break;
					default:
						return S_ATC_FAIL;
				}

				ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

				para.enable = nEnable;
				para.paramIndex = nIndex;
				para.value1 = nValue1;
				para.value2 = nValue2;
				SCI_TRACE_BUF("para:", &para ,sizeof(para));
				MNPHONE_RrtmParam(para, dual_sys);
			}
			else
			{
				return S_ATC_FAIL;
			}
			break;
		default:
			return S_ATC_FAIL;
	}

	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessGRRLTEFREQ)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

	SCI_TRACE_LOW("ATC_ProcessGRRLTEFREQ");

	switch (ATC_GET_CMD_TYPE)
	{
		case ATC_CMD_TYPE_SET:
			if (PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG && PARAM4_FLAG)
			{
				MNPHONE_GrrLteFreq_t para;
				para.earfcn = PARA1.num;
				para.prior = PARA2.num;
				para.thresh_h = PARA3.num;
				para.thresh_l = PARA4.num;
				SCI_TRACE_BUF("para:", &para ,sizeof(para));
				MNPHONE_GrrLteFreq(para, dual_sys);
			}
			else
			{
				return S_ATC_FAIL;
			}
			break;
		default:
			return S_ATC_FAIL;
	}

	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessL1PARAM)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

	SCI_TRACE_LOW("ATC_ProcessL1PARAM");

	switch (ATC_GET_CMD_TYPE)
	{
		case ATC_CMD_TYPE_SET:
			if (PARAM1_FLAG)
			{
				MNPHONE_L1Param_t para;
				uint16 nIndex = PARA1.num;
				uint16 nValue = 0;
				if(nIndex  == 3 && !PARAM2_FLAG)
				{
					return S_ATC_FAIL;
				}
				else if(nIndex == 3)
				{
					nValue = PARA2.num;
				}
				para.index = nIndex;
				para.value = nValue;
				SCI_TRACE_BUF("para:", &para ,sizeof(para));
				MNPHONE_L1Param(para, dual_sys);
			}
			else
			{
				return S_ATC_FAIL;
			}
			break;
		default:
			return S_ATC_FAIL;
	}

	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessRRDMPARAM)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

	SCI_TRACE_LOW("ATC_ProcessRRDMPARAM");
	#if 0 // this is for 3G ,no need now
	switch (ATC_GET_CMD_TYPE)
	{
		case ATC_CMD_TYPE_SET:
			if (PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG && PARAM4_FLAG)
			{

			}
			else
			{
				return S_ATC_FAIL;
			}
			break;
		default:
			return S_ATC_FAIL;
	}
	#endif
	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessSPCLEANINFO)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

	SCI_TRACE_LOW("ATC_ProcessSPCLEANINFO");

	switch (ATC_GET_CMD_TYPE)
	{
		case ATC_CMD_TYPE_SET:
			if (PARAM1_FLAG)
			{
				MNPHONE_SpCleanInfo_t para;
				uint8 nType = PARA1.num;
				if(nType != 1 && nType != 4 && nType != 7)
				{
					return S_ATC_FAIL;
				}

				para.type = nType;
				MNPHONE_SpCleanInfo(para, dual_sys);
			}
			else
			{
				return S_ATC_FAIL;
			}
			break;
		default:
			return S_ATC_FAIL;
	}

	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}
uint8 g_XCAP[128];
uint8 g_BSF[128];
AT_CMD_FUNC(ATC_ProcessXCAPRTURI)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

	SCI_TRACE_LOW("ATC_ProcessXCAPRTURI");

	switch (ATC_GET_CMD_TYPE)
	{
		case ATC_CMD_TYPE_SET:
			if (PARAM1_FLAG)
			{
				SCI_TRACE_LOW("URI: %s, str_len: %d", PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len);
				SCI_MEMSET(g_XCAP, 0, sizeof(g_XCAP));
				strcpy(g_XCAP, PARA1.str_ptr->str_ptr);
				MNPHONE_SetXCAP(g_XCAP, PARA1.str_ptr->str_len, dual_sys);
			}
			else
			{
				return S_ATC_FAIL;
			}
			break;
		case ATC_CMD_TYPE_READ:
			sprintf((char *)g_rsp_str, "%s:%s",
								g_atc_info_table[AT_CMD_XCAPRTURI].cmd_name->str_ptr, g_XCAP);
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			break;
		case ATC_CMD_TYPE_TEST:
			sprintf((char *)g_rsp_str, "%s: <uri>",g_atc_info_table[AT_CMD_XCAPRTURI].cmd_name->str_ptr);
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			break;
		default:
			return S_ATC_FAIL;
	}

	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessBSF)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

	SCI_TRACE_LOW("ATC_ProcessBSF");

	switch (ATC_GET_CMD_TYPE)
	{
		case ATC_CMD_TYPE_SET:
			if (PARAM1_FLAG)
			{
				SCI_TRACE_LOW("BSF: %s, str_len: %d", PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len);
				SCI_MEMSET(g_BSF, 0, sizeof(g_BSF));
				strcpy(g_BSF, PARA1.str_ptr->str_ptr);
				MNPHONE_SetBSF(g_BSF, PARA1.str_ptr->str_len, dual_sys);
			}
			else
			{
				return S_ATC_FAIL;
			}
			break;
		case ATC_CMD_TYPE_READ:
			sprintf((char *)g_rsp_str, "%s:%s",
								g_atc_info_table[AT_CMD_BSF].cmd_name->str_ptr, g_BSF);
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			break;
		case ATC_CMD_TYPE_TEST:
			sprintf((char *)g_rsp_str, "%s: <bsf>",g_atc_info_table[AT_CMD_BSF].cmd_name->str_ptr);
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			break;
		default:
			return S_ATC_FAIL;
	}

	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessCISRVCC)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

	SCI_TRACE_LOW("ATC_ProcessCISRVCC");

	switch (ATC_GET_CMD_TYPE)
	{
		case ATC_CMD_TYPE_SET:
			if (PARAM1_FLAG)
			{
				SCI_TRACE_LOW("SRVCC: %d", PARA1.num);
				MNPHONE_ImsSrvccCapSet(PARA1.num, dual_sys);
			}
			else
			{
				return S_ATC_FAIL;
			}
			break;
		case ATC_CMD_TYPE_READ:
			sprintf((char *)g_rsp_str, "%s:%d",
								g_atc_info_table[AT_CMD_CISRVCC].cmd_name->str_ptr, MNPHONE_ImsGetSrvccCap(dual_sys));
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			break;
		case ATC_CMD_TYPE_TEST:
			sprintf((char *)g_rsp_str, "%s: <bsf>",g_atc_info_table[AT_CMD_CISRVCC].cmd_name->str_ptr);
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			break;
		default:
			return S_ATC_FAIL;
	}

	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}


AT_CMD_FUNC(ATC_ProcessCHANGERAT)
{
#if defined(_ATC_UIX8910_ENABLE_)
	MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

	SCI_TRACE_LOW("ATC_ProcessCHANGERAT");

	switch (ATC_GET_CMD_TYPE)
	{
		case ATC_CMD_TYPE_SET:
			if (PARAM1_FLAG)
			{
				SCI_TRACE_LOW("CHANGERAT: %d", PARA1.num);
				MNPHONE_ChangeRat(PARA1.num, dual_sys);
			}
			else
			{
				return S_ATC_FAIL;
			}
			break;
		case ATC_CMD_TYPE_READ:
			sprintf((char *)g_rsp_str, "%s",
								g_atc_info_table[AT_CMD_CHANGERAT].cmd_name->str_ptr);
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			break;
		case ATC_CMD_TYPE_TEST:
			sprintf((char *)g_rsp_str, "%s",g_atc_info_table[AT_CMD_CHANGERAT].cmd_name->str_ptr);
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			break;
		default:
			return S_ATC_FAIL;
	}

	return S_ATC_SUCCESS;
#else
	return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessSPCHKSD)
{
#if defined(_ATC_UIX8910_ENABLE_)
    uint32 portNo;
    uint8 index;

#define COM_DEBUG 1

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
#ifdef DUAL_TCARD_SUPPORT
            for (index = 0; index < 2; index++)
#else
            for (index = 0; index < 1; index++)
#endif
            {
#ifdef SDCARD_SUPPORT
                if (SCM_STATUS_NORMAL == SCM_GetSlotStatus(index))
                {
                    sprintf((char *)g_rsp_str, "+SPCHKSD: 1");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_SUCCESS;
                }
#endif
            }
            return S_ATC_FAIL;
        }

        default:
            return S_ATC_FAIL;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessEMMCDDRSIZE)
{
#if defined(_ATC_UIX8910_ENABLE_)
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            sprintf((char *)g_rsp_str, "%dMB+%dMB", MEM_GetRamTotalSize()/(1024*1024), SPIFLASH_GetFlashTotalSize()/(1024*1024));
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
             return S_ATC_FAIL;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessFACNETINFO)
{
#if defined(_ATC_UIX8910_ENABLE_)
    MN_NET_INFO_T netInfo;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                MNPHONE_FacGetNetInfo(&netInfo, PARA1.num);
                sprintf((char *)g_rsp_str, "+FACNETINFO: %d", netInfo.nwCapabilityLte.lteScell.pcid);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_SUCCESS;
            }
            else
            {
                return S_ATC_FAIL;
            }
            break;
        }

        default:
            return S_ATC_FAIL;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessFACNETINFOSTA)
{
#if defined(_ATC_UIX8910_ENABLE_)
    MN_NET_INFO_STATISTIC_T netInfoStatistic;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                MNPHONE_FacGetNetInfoStatistic(&netInfoStatistic, PARA1.num);
                return S_ATC_SUCCESS;
            }
            else
            {
                return S_ATC_FAIL;
            }
            break;
        }

        default:
            return S_ATC_FAIL;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

ATC_STATUS ATC_ProcessSimPresentInd(
                        ATC_CONFIG_T        *atc_config_ptr,
                        APP_MN_SIM_PRESENT_IND_T       *sig_ptr,
                        ATC_GLOBAL_INFO_T   *atc_global_info_ptr
                        )
{
    SCI_TRACE_LOW("ATC_ProcessSimPresentInd, is_sim_present:%d", sig_ptr->is_sim_present);
    if(RM_GetCalibrationPostMode())
        MMIAPIPHONE_SetSimExistedStatus(sig_ptr->dual_sys,sig_ptr->is_sim_present);
    return S_ATC_DEFAULT_HANDLE;
}

AT_CMD_FUNC(ATC_ProcessSPRNDIS)
{
#if defined(_ATC_UIX8910_ENABLE_)
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                MNPHONE_SwitchUSBShareNetwork(PARA1.num);
                return S_ATC_SUCCESS;
            }
            else
            {
                return S_ATC_FAIL;
            }
            break;
        }

        default:
            return S_ATC_FAIL;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessCHECKOPERAIN)
{
#if defined(_ATC_UIX8910_ENABLE_)
    unsigned short* opera_file_name = L"E:\\OperaMini\\mini4\\reksio.ini";

	char * ret=0;
	uint32 bytes_read = 0;
	unsigned char* read_buff = NULL;

	unsigned  char* Opera_accept_flag = "Eula=TRUE";

	SFS_HANDLE hHandle = 0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {

            if(MMIFILE_IsFileExist(opera_file_name, MMIAPICOM_Wstrlen(opera_file_name)))
            {


					hHandle = MMIAPIFMM_CreateFile(opera_file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);

					if( NULL  ==  hHandle )
					{

						 sprintf((char *)g_rsp_str, "+CHECKOPERAIN: creatfile error");
               				 ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
						return  S_ATC_FAIL ;
					}


					SFS_SetFilePointer(hHandle, 256, SFS_SEEK_BEGIN);

					read_buff = SCI_ALLOC_APP(256);
					if(NULL == read_buff  )
					{

						MMIAPIFMM_CloseFile(hHandle);
					 	SCI_Free((void * )read_buff);
						 sprintf((char *)g_rsp_str, "+CHECKOPERAIN: alloc error");
               				 ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
						return  S_ATC_FAIL ;
					}


					if(SFS_NO_ERROR != SFS_ReadFile(hHandle, (void*)read_buff, 240, (uint32 *)&bytes_read, NULL))
					{
						MMIAPIFMM_CloseFile(hHandle);
					 	SCI_Free((void * )read_buff);
						sprintf((char *)g_rsp_str, "+CHECKOPERAIN: SFS_ReadFile error ");
               				ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
						return  S_ATC_FAIL ;
					}

					MMIAPIFMM_CloseFile(hHandle);
					ret = strstr(read_buff, (uint8*)Opera_accept_flag);
				 	SCI_Free((void * )read_buff);

					if(ret == NULL )
					{
						sprintf((char *)g_rsp_str, "+CHECKOPERAIN: not accept ");
               				ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
						return  S_ATC_FAIL ;
					}
					else
					{

				                sprintf((char *)g_rsp_str, "+CHECKOPERAIN: 1");
				                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
				                return S_ATC_SUCCESS;
					}

            }
            return S_ATC_FAIL;
        }

        default:
            return S_ATC_FAIL;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessAUDCH)
{
#if defined(_ATC_UIX8910_ENABLE_)
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                AUDIO_SetDevMode(PARA1.num);
                return S_ATC_SUCCESS;
            }
            else
            {
                return S_ATC_FAIL;
            }
            break;
        }

        default:
            return S_ATC_FAIL;
    }
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessSPWIQ)
{
#if defined(_ATC_UIX8910_ENABLE_)
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
               ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
               POWER_Reset();
               break;
            }
            else
            {
                return S_ATC_FAIL;
            }
        }

        default:
            return S_ATC_FAIL;
    }
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC(ATC_ProcessIMSNV)
{
#if defined(_ATC_UIX8910_ENABLE_)
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM2_FLAG)
            {
                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    SCI_TRACE_LOW("ATC_ProcessIMSNV, param2 is not string!");
                    return S_ATC_FAIL;
                }
                if(MNPHONE_ImsNvParamSet(PARA1.num, PARA2.str_ptr->str_ptr, dual_sys))
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                else
                    return S_ATC_FAIL;

                break;
            }
            else if(PARAM1_FLAG)
            {
                char value[256];

                SCI_MEMSET(value, 0, sizeof(value));
                if(MNPHONE_ImsNvParamGet(PARA1.num, value, dual_sys))
                {
                    sprintf((char *)g_rsp_str, "+IMSNV: %d,%s", PARA1.num,value);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                }
                else
                {
                    return S_ATC_FAIL;
                }
                break;
            }
	     else
	     {
	          return S_ATC_FAIL;
	     }
        }
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s",g_atc_info_table[AT_CMD_IMSNV].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        default:
            return S_ATC_FAIL;
    }
    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif
}

extern int gnss_at_test(char *buf, char *rsp);
extern int gnss_at_test_v2(char *buf, char *rsp);
extern int cgnssst_at_test_parse(char *buf, char *rsp);

AT_CMD_FUNC(ATC_ProcessSPGPSTEST)
{
    ATC_STATUS status = S_ATC_SUCCESS;//S_ATC_DEFAULT_HANDLE;
#ifndef WIN32
#ifdef GPS_SUPPORT
    char *temp_ptr = PNULL;
    uint8 *buf_ptr = PNULL;
    uint8 param_len = 0;
    uint8 cmd_type = 0;
    char rsp[256] = NULL;
    int rsp_size = 0;

    SCI_TRACE_LOW("ATC_GNSS:[LIBGPS] enter ATC_ProcessSPGPSTEST");
    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        SCI_TRACE_LOW("ATC_GNSS:[LIBGPS] ATC_ProcessSPGPSTEST 111 return ERR_OPERATION_NOT_ALLOWED");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    temp_ptr  = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    ATC_GetCmdType(temp_ptr, (uint16)param_len, &cmd_type);
    buf_ptr = temp_ptr+1;
    SCI_TRACE_LOW("ATC_GNSS:[LIBGPS] temp_ptr: [%s],buf_ptr: [%s]",temp_ptr,buf_ptr);
    SCI_TRACE_LOW("ATC_GNSS:[LIBGPS] ATC_ProcessSPGPSTEST param_len: %d,cmd_type: %d",param_len,cmd_type);
    //gnss_at_test(temp_ptr,rsp);
    rsp_size = gnss_at_test_v2(buf_ptr,rsp);
    memset(g_rsp_str, 0, sizeof(g_rsp_str));
    memcpy((uint8*)g_rsp_str, (uint8*)rsp, rsp_size);
    //just for debug
    //SCI_TRACE_LOW("ATC_GNSS: [LIBGPS] sizeof(RSP_RESULT_STR):%d,rsp_size:%d, g_rsp_str: [%s],rsp: [%s]",
    //sizeof(g_rsp_str),rsp_size,g_rsp_str,rsp);

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#endif
#endif
    return status;
}

AT_CMD_FUNC(ATC_ProcessCGNSSST)
{
    ATC_STATUS status = S_ATC_SUCCESS;//S_ATC_DEFAULT_HANDLE;
#ifndef WIN32
#ifdef GPS_SUPPORT
    uint8 *temp_ptr = PNULL;
    uint8 *buf_ptr = PNULL;
    uint8 param_len = 0;
    uint8 cmd_type = 0;
    int32 satellite_type = 0;
    char rsp[60] = NULL;

    SCI_TRACE_LOW("ATC_GNSS:[LIBGPS] enter ATC_ProcessCGNSSST");
    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        SCI_TRACE_LOW("ATC_GNSS:[LIBGPS] ATC_ProcessCGNSSST 111 return ERR_OPERATION_NOT_ALLOWED");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    temp_ptr  = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    ATC_GetCmdType(temp_ptr, (uint16)param_len, &cmd_type);
    buf_ptr = temp_ptr+1;
    SCI_TRACE_LOW("ATC_GNSS:[LIBGPS] temp_ptr: [%s],buf_ptr: [%s]",temp_ptr,buf_ptr);
    SCI_TRACE_LOW("ATC_GNSS:[LIBGPS] ATC_ProcessCGNSSST enter param_len: %d,cmd_type: %d",param_len,cmd_type);

    if(cmd_type == ATC_CMD_TYPE_READ)
    {
        satellite_type = gnss_getcpmode();
        sprintf((char *)rsp, "+CGNSSST: %d",satellite_type);
        memset(g_rsp_str, 0, sizeof(g_rsp_str));
        memcpy((uint8*)g_rsp_str, (uint8*)rsp, strlen(rsp));
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else if(cmd_type == ATC_CMD_TYPE_SET)
    {
        if(buf_ptr[0] == '\0')
        {
            SCI_TRACE_LOW("ATC_GNSS:[LIBGPS] CGNSSST need to set cpmode?");
            sprintf((char *)rsp, "+CGNSSST: wating for set...empty");
        }
        else
        {
            if(!cgnssst_at_test_parse(buf_ptr,rsp))
               SCI_TRACE_LOW("ATC_GNSS:[LIBGPS] CGNSSST set SUCC");
            else
               SCI_TRACE_LOW("ATC_GNSS:[LIBGPS] CGNSSST set Fail");
        }
        memset(g_rsp_str, 0, sizeof(g_rsp_str));
        memcpy((uint8*)g_rsp_str, (uint8*)rsp, strlen(rsp));
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else if(cmd_type == ATC_CMD_TYPE_TEST)
    {
        sprintf((char *)rsp, "+CGNSSST: (1,2,3,4,5,8,9,13,16,17,18,19,20,21,24,25,29)");
        memset(g_rsp_str, 0, sizeof(g_rsp_str));
        memcpy((uint8*)g_rsp_str, (uint8*)rsp, strlen(rsp));
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }

#endif
#endif
    return status;
}

BOOLEAN ATC_SPWIFI_GetParams(
                uint8            *in_ptr,
                uint8             in_len,
                ATC_SPWIFI_PARAM_T *param_ptr
                )
{
    int32              i    = 0;
    int32              j    = 0;
    uint16            k    = 0;
    BOOLEAN    flag    = TRUE;

    for (i=0; i<in_len; i++)
    {
        if (in_ptr[i] == ',')
        {
            k++;
            j = 0;

            if (8 < k)
            {
                flag = FALSE;
                break;
            }
        }
        else
        {
            if (j < ATC_SPWIFI_MAX_LEN)
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

    return flag;
}

AT_CMD_FUNC(ATC_ProcessWIFI)
{
    ATC_STATUS status = S_ATC_SUCCESS;//S_ATC_DEFAULT_HANDLE;
#ifndef WIN32
#ifdef WIFI_SUPPORT_UNISOC_RX
    ATC_SPWIFI_PARAM_T param[4] = {0};
    uint8 *temp_ptr = PNULL;
    uint8 param_len = 0;
    uint8 cmd_type = 0;
    uint32 type_oper = 0;
    BOOLEAN quiry_flag = FALSE;
    int wifi_index = 0;
	WIFISCAN_REQ scan_req = {0};
    struct wifi_scan_res *scan_res = PNULL;
    const char *s_wifi_str[3] =
    {
        "OPEN","SCAN","CLOSE"
    };
    uint32 i = 0;
    char ssid[32] = {0};
    C_F_W_WIFISENTEST_PARAM wifi_test_para = {0};
    C_F_W_WIFISENTEST_RES wifi_test_res = {0};

    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
		SCI_TRACE_LOW("ATC_ProcessWIFI 111 return ERR_OPERATION_NOT_ALLOWED");

        return ERR_OPERATION_NOT_ALLOWED;
    }

    temp_ptr  = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    ATC_GetCmdType(temp_ptr, (uint16)param_len, &cmd_type);

    SCI_TRACE_LOW("ATC_ProcessWIFI enter. %d %d",param_len,cmd_type);

    switch (cmd_type)
    {
    case ATC_CMD_TYPE_SET:
        temp_ptr++;
        param_len--;
        if (ATC_SPWIFI_GetParams(temp_ptr, param_len, param))
        {
            if ('?' == param[0].param[param[0].param_len-1])
            {
                quiry_flag = TRUE;
                param[0].param[param[0].param_len-1] = '\0';
            }
            else
            {
                param[0].param[param[0].param_len] = '\0';
            }

            for (wifi_index=0; wifi_index<3; wifi_index++)
            {
                if (!strcmp((char *)param[0].param, s_wifi_str[wifi_index]))
                {
                    break;
                }
            }

            SCI_TRACE_LOW("WIFI AT: param_len is %d, 0 param_len is %d, index is %d",param_len,param[0].param_len,wifi_index);

            switch (wifi_index)
            {
                case 0://OPEN
                {
                    if (quiry_flag)
                    {
                        sprintf((char *)g_rsp_str, "%s%s%s%d\n\r","+WIFI:",s_wifi_str[wifi_index],"=", wifi_sta_is_open());
                    }
                    else//if (param[1].param_len != 0)
                    {
						wifi_sta_test_init();

						wifi_sta_open();

                        sprintf((char *)g_rsp_str, "%s\n\r","+WIFI:OK");
                    }

                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }

                case 1://SCAN
                {
                    if (quiry_flag)
                    {
						scan_res = wifi_sta_scan_results();
						if (scan_res && scan_res->nr_bss)
						{
						    for(i=0;i<scan_res->nr_bss;i++)
						    {
								SCI_MEMCPY(ssid,scan_res->bsses[i].ssid,32);
							    sprintf((char *)g_rsp_str, "%s%d%s%x%s%x%s%x%s%x%s%x%s%x%s%d%s%s\n\r","+WIFI CHN:",scan_res->bsses[i].channel," MAC ",scan_res->bsses[i].bssid[0],":",scan_res->bsses[i].bssid[1],":",scan_res->bsses[i].bssid[2],":",scan_res->bsses[i].bssid[3],":",scan_res->bsses[i].bssid[4],":",scan_res->bsses[i].bssid[5],\
									" RSSI:",scan_res->bsses[i].rssi," SSID:",ssid);
                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
						    }
							return status;
						}
                        else
                        {
                            sprintf((char *)g_rsp_str, "%s\n\r","+WIFI:SCAN No Result");
                        }
                    }
                    else if (param[1].param_len != 0)
                    {
                        type_oper = param[1].param[0] - '0';
                        if (0 == type_oper)
                        {
                            wifi_sta_scan_stop();
							sprintf((char *)g_rsp_str, "%s\n\r","+WIFI:OK");
                        }
                        else
                        {
                            if (wifi_sta_scan_is_lte_busy())
                           	{
                                sprintf((char *)g_rsp_str, "%s\n\r","+WIFI: LTE Busy");
                           	}
							else
							{
                                wifi_sta_scan_all_chn_start();
    							sprintf((char *)g_rsp_str, "%s\n\r","+WIFI:OK");
							}
                        }
                    }
                    else
                    {
						sprintf((char *)g_rsp_str, "%s\n\r","+WIFI:ERROR CMD");
                    }

                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }

                case 2://STOP
                {
                    if (quiry_flag)
                    {
                        sprintf((char *)g_rsp_str, "%s%s%s%d\n\r","+WIFI:",s_wifi_str[wifi_index],"=",wifi_sta_is_open());
                    }
                    else //if (param[1].param_len != 0)
                    {
                        wifi_sta_close();

                        sprintf((char *)g_rsp_str, "%s\n\r","+WIFI:OK");
                    }

                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }

                default: //Error input para
                {
                    SCI_TRACE_LOW("ATC_ProcessWIFI default return ERR_OPERATION_NOT_ALLOWED");
                    return ERR_OPERATION_NOT_ALLOWED;
                }
            }
        }
        else
        {
			SCI_TRACE_LOW("ATC_ProcessWIFI else return ERR_OPERATION_NOT_ALLOWED");
            return ERR_OPERATION_NOT_ALLOWED;
        }
        break;

    case ATC_CMD_TYPE_READ:
    case ATC_CMD_TYPE_TEST:
        {
            SCI_TRACE_LOW("ATC_ProcessWIFI 0x%x return ERR_OPERATION_NOT_ALLOWED",cmd_type);
            return ERR_OPERATION_NOT_ALLOWED;
        }
        break;

    case 0xFF:
        {
            for(i=0;i<param_len;i++)
            {
                SCI_TRACE_LOW("AT WIFISENTEST temp_ptr[%d]=0x%x",i,temp_ptr[i]);//Debug log
            }

			if ((param_len > 5)
				&& ('S' == temp_ptr[0]) && ('C' == temp_ptr[1]) && ('A' == temp_ptr[2]) && ('N' == temp_ptr[3]) && ('=' == temp_ptr[4]))
			{
                if (',' == temp_ptr[7])
                {
					type_oper = temp_ptr[5] - '0';
					cmd_type = type_oper * 10 + (temp_ptr[6] - '0');
                }
				else//if (',' == temp_ptr[6])
				{
                    cmd_type = temp_ptr[5] - '0';
				}

				wifi_sta_test_init();
				wifi_sta_open();
				scan_req.wswitch = 1;
				scan_req.nChNum = 1;
				scan_req.nMaxNodeNum = 10;
				if (cmd_type > 13)
				{
				    cmd_type = 13;
				}
				scan_req.nCh[0] = cmd_type;
				wifi_sta_scan_start(scan_req);

				SCI_Sleep(1000);

				scan_res = wifi_sta_scan_results();
				if (scan_res && scan_res->nr_bss)
				{
                    sprintf((char *)g_rsp_str, "%s%d%s\n\r","+WIFISCAN:wifiApNum:",cmd_type,",maxApNum:10");
					for(i=0;i<scan_res->nr_bss;i++)
					{
					    SCI_MEMCPY(ssid,scan_res->bsses[i].ssid,32);
						sprintf((char *)g_rsp_str, "%s%d%s%x%s%x%s%x%s%x%s%x%s%x%s%d%s%s\n\r","+WIFISCAN: CH",cmd_type,",MAC ",\
							scan_res->bsses[i].bssid[0],":",scan_res->bsses[i].bssid[1],":",scan_res->bsses[i].bssid[2],":",scan_res->bsses[i].bssid[3],":",scan_res->bsses[i].bssid[4],":",scan_res->bsses[i].bssid[5],\
							" RSSI:",scan_res->bsses[i].rssi,",SSID:",ssid);
						ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
					}
					return status;
				}
				else
				{
					sprintf((char *)g_rsp_str, "%s\n\r","+WIFISCAN No Result");
				}
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
			}
			else if ((9 == param_len)
				&& ('S' == temp_ptr[0]) && ('E' == temp_ptr[1]) && ('N' == temp_ptr[2]) && ('T' == temp_ptr[3]) && ('E' == temp_ptr[4]) && ('S' == temp_ptr[5]) && ('T' == temp_ptr[6]) && ('O' == temp_ptr[7]) && ('N' == temp_ptr[8]))
			{
                wifi_sensetivity_test_on();
			}
			else if ((10 == param_len)
				&& ('S' == temp_ptr[0]) && ('E' == temp_ptr[1]) && ('N' == temp_ptr[2]) && ('T' == temp_ptr[3]) && ('E' == temp_ptr[4]) && ('S' == temp_ptr[5]) && ('T' == temp_ptr[6]) && ('O' == temp_ptr[7]) && ('F' == temp_ptr[8]) && ('F' == temp_ptr[9]))
			{
                wifi_sensetivity_test_off();
			}
			else if ((param_len > 8)
				&& ('S' == temp_ptr[0]) && ('E' == temp_ptr[1]) && ('N' == temp_ptr[2]) && ('T' == temp_ptr[3]) && ('E' == temp_ptr[4]) && ('S' == temp_ptr[5]) && ('T' == temp_ptr[6]) && ('=' == temp_ptr[7]))
			{
			    if ('0' == temp_ptr[8])//Single
			    {
                    if (',' != temp_ptr[9])
                    {
                        SCI_TRACE_LOW("ATC_ProcessWIFI SENTEST AAA");
                        return ERR_OPERATION_NOT_ALLOWED;
                    }

			        if ('1' == temp_ptr[10])//start
			        {
                        if (',' != temp_ptr[11])
                        {
                            SCI_TRACE_LOW("ATC_ProcessWIFI SENTEST BBB");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                        wifi_test_para.ch_id = temp_ptr[12] - '0';
                        if (14 == param_len)
						{
						    type_oper = wifi_test_para.ch_id;
                            wifi_test_para.ch_id = type_oper * 10 + (temp_ptr[13] - '0');
						}
						wifi_test_para.mode = 0;

						wifi_sensetivity_test(wifi_test_para);
			        }
					else if ('0' == temp_ptr[10])//stop and get result
					{
                        wifi_sensetivity_get_result();
                        SCI_Sleep(200);
                        wifi_test_res = wifi_sensetivity_read_result();

                        sprintf((char *)g_rsp_str, "%s%d\n\r","+WIFISENTEST:0->",wifi_test_res.result[0]);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
					}
					else
					{
                        SCI_TRACE_LOW("ATC_ProcessWIFI SENTEST parse mode <0,1>");
                        return ERR_OPERATION_NOT_ALLOWED;
					}
			    }
				else if ('1' == temp_ptr[8])//continuous
				{
				    wifi_test_para.mode = 1;

                    if (',' == temp_ptr[11])
                    {
                        wifi_test_para.ch_id = temp_ptr[10] - '0';

						if (',' == temp_ptr[13])
						{
							type_oper = temp_ptr[12] - '0';
                            wifi_test_para.loop_time = type_oper*1000;

                            wifi_test_para.max_loop = temp_ptr[14] - '0';
							if (16 == param_len)
							{
							    type_oper = wifi_test_para.max_loop;
								wifi_test_para.max_loop = type_oper * 10 + temp_ptr[15] - '0';
							}
							wifi_sensetivity_test(wifi_test_para);

                            type_oper = wifi_test_para.loop_time*wifi_test_para.max_loop;
							if (type_oper > 13000)
							{
							    type_oper = 13000;//max wait time
							}
							SCI_TRACE_LOW("ATC_ProcessWIFI SENTEST wait %d ms",type_oper);
                            SCI_Sleep(type_oper);

							wifi_sensetivity_get_result();
							SCI_Sleep(200);

							wifi_test_res = wifi_sensetivity_read_result();
                            if (wifi_test_para.max_loop > AT_WIFI_SENTEST_MAX_CNT)
                            {
                                wifi_test_para.max_loop = AT_WIFI_SENTEST_MAX_CNT;
                            }

                            type_oper = 0;
                            for (i=0;i<wifi_test_para.max_loop;i++)
                            {
							    sprintf((char *)g_rsp_str, "%s%d%s%d%s\n\r","+WIFISENTEST:1->",wifi_test_res.result[i],"(",i,")");
								ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                                type_oper += wifi_test_res.result[i];
								if ((wifi_test_para.max_loop-1) == i)//last time
								{
									sprintf((char *)g_rsp_str, "%s%d%s\n\r","+WIFISENTEST:1->",type_oper,"(total)");
									ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
								}
                            }
						}
						else
						{
							SCI_TRACE_LOW("ATC_ProcessWIFI SENTEST parse wrong seconds 0x%x",temp_ptr[12]);
							return ERR_OPERATION_NOT_ALLOWED;
						}
                    }
					else if (',' == temp_ptr[12])
					{
						type_oper = temp_ptr[10] - '0';
						wifi_test_para.ch_id = type_oper * 10 + (temp_ptr[11] - '0');

                        if (',' == temp_ptr[14])
                        {
                            type_oper = temp_ptr[13] - '0';
							wifi_test_para.loop_time = type_oper*1000;

                            wifi_test_para.max_loop = temp_ptr[15] - '0';
							if (17 == param_len)
							{
							    type_oper = wifi_test_para.max_loop;
								wifi_test_para.max_loop = type_oper * 10 + temp_ptr[16] - '0';
							}
							wifi_sensetivity_test(wifi_test_para);

                            type_oper = wifi_test_para.loop_time*wifi_test_para.max_loop;
                            if (type_oper > 13000)
                            {
                            	type_oper = 13000;//max wait time
                            }
                            SCI_TRACE_LOW("ATC_ProcessWIFI SENTEST wait %d ms",type_oper);
                            SCI_Sleep(type_oper);

                            wifi_sensetivity_get_result();
                            SCI_Sleep(200);

                            wifi_test_res = wifi_sensetivity_read_result();
                            if (wifi_test_para.max_loop > AT_WIFI_SENTEST_MAX_CNT)
                            {
                            	wifi_test_para.max_loop = AT_WIFI_SENTEST_MAX_CNT;
                            }

                            type_oper = 0;
                            for (i=0;i<wifi_test_para.max_loop;i++)
                            {
                            	sprintf((char *)g_rsp_str, "%s%d%s%d%s\n\r","+WIFISENTEST:1->",wifi_test_res.result[i],"(",i,")");
                            	ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                            	type_oper += wifi_test_res.result[i];
                            	if ((wifi_test_para.max_loop-1) == i)//last time
                            	{
                            		sprintf((char *)g_rsp_str, "%s%d%s\n\r","+WIFISENTEST:1->",type_oper,"(total)");
                            		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            	}
                            }

                        }
						else
						{
							SCI_TRACE_LOW("ATC_ProcessWIFI SENTEST parse wrong seconds 0x%x",temp_ptr[12]);
							return ERR_OPERATION_NOT_ALLOWED;
						}

					}
					else
					{
                        SCI_TRACE_LOW("ATC_ProcessWIFI SENTEST parse chn 0x%x 0x%x",temp_ptr[11],temp_ptr[12]);
                        return ERR_OPERATION_NOT_ALLOWED;
					}
				}
				else
				{
					SCI_TRACE_LOW("ATC_ProcessWIFI SENTEST parse mode <0,1>");
					return ERR_OPERATION_NOT_ALLOWED;
				}
			}
			else
			{
                SCI_TRACE_LOW("ATC_ProcessWIFI SENTEST parse fail");
                return ERR_OPERATION_NOT_ALLOWED;
			}
		}
		break;

    default:
        SCI_TRACE_LOW("ATC_ProcessWIFI default 0x%x return ERR_OPERATION_NOT_ALLOWED",cmd_type);
        return ERR_OPERATION_NOT_ALLOWED;
    }
#endif
#endif

    return status;
}

#if defined PLATFORM_UWS6121E
uint8 getATCommandParams(ATC_CUR_INFO_LIST_T *cur_cmd_info, uint8 *cmd_type, ATC_CPWD_PARAM_T *param, uint8 size)
{
    uint8                 *temp_ptr;
    uint8                 param_len;

    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
//        SDI_VTRACEID_INTEGER(ATC, ATC_getATCommandParams_1_failed____d_, "%d", "%d", "ATC:getATCommandParams 1 failed = %d.", 1, ( ERR_OPERATION_NOT_ALLOWED));
        return ERR_OPERATION_NOT_ALLOWED;
    }
    SCI_MEMSET(param, 0, (size * sizeof(ATC_CPWD_PARAM_T)));
    temp_ptr  = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    if(temp_ptr == NULL)
    {
//        SDI_VTRACEID_INTEGER(ATC, ATC_getATCommandParams_2_failed____d_, "%d", "%d", "ATC:getATCommandParams 2 failed = %d.", 1, ( ERR_OPERATION_NOT_ALLOWED));
        return ERR_OPERATION_NOT_ALLOWED;
    }

    ATC_GetCmdType(temp_ptr, (uint16)param_len, cmd_type);

    if(ATC_GetParams(temp_ptr, param_len, param, size))
    {
//       SDI_VTRACEID(ATC, ATC_getATCommandParams_param_0_____s__param_1_____s__param_2_____s__cmd_type____d, "%s%s%s%d", "%s\1%s\1%s\1%d", "ATC:getATCommandParams param[0] = %s, param[1] = %s, param[2] = %s, cmd_type = %d", 4, ( param[0].param, param[1].param, param[2].param, cmd_type));
    }
    else
    {
//        SDI_VTRACEID_INTEGER(ATC, ATC_getATCommandParams_3_failed____d_, "%d", "%d", "ATC:getATCommandParams 3 failed = %d.", 1, ( ERR_OPERATION_NOT_ALLOWED));
        return ERR_OPERATION_NOT_ALLOWED;
    }
    return S_ATC_SUCCESS;
}
#endif
#ifdef PLATFORM_UWS6121E
extern PUBLIC void TCPIPDEBUG_GetAllNetifInfo(uint8* buf, uint32 len);
#endif

/*****************************************************************************/
//  Description: This function handles the AT+NETIF command
//               used for query all open net infomation.
//  Example:     AT+NETIF=net info
//  Author:      bangyu.liu
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessNETIF)
{
#ifdef PLATFORM_UWS6121E
    uint8  *temp_ptr = NULL;

    //SDI_VTRACEID_INTEGER(ATC, ATC_ProcessNETINFO_enter__MAX_ATC_RSP_LEN__d_n, "%d", "%d", "ATC_ProcessNETIF enter! MAX_ATC_RSP_LEN=%d\n", 1, ( MAX_ATC_RSP_LEN));
    temp_ptr  = PARA1.str_ptr->str_ptr;

    //SDI_VTRACEID(ATC, NETINFO__temp_ptr__s__n, "%s", "%s", "NETIF  temp_ptr=%s!\n", 1, ( temp_ptr));
	if (strcmp(&temp_ptr[1],"net info") == 0)
    {
        TCPIPDEBUG_GetAllNetifInfo(g_rsp_str, MAX_ATC_RSP_LEN);
    }
    else
    {
        sprintf((char *)g_rsp_str, "AT CMD error, use: AT+NETIF=net info\n");
    }
    //SDI_VTRACEID(ATC, _s__n, "%s", "%s", "%s \n", 1, ( (char *)g_rsp_str));
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    //SDI_TRACEID_INTEGER(ATC, ATC_ProcessNETINFO_Exit__n, "ATC_ProcessNETIF Exit!\n");
#endif
    return S_ATC_SUCCESS;
}

#ifdef PLATFORM_UWS6121E
extern PUBLIC int sci_set_mtu(TCPIP_NETID_T net_id, int type, int mtu);
extern PUBLIC int sci_get_mtu(TCPIP_NETID_T net_id, int type, int *mtu);
extern TCPIP_NETID_T sci_get_netidbyname(char* str);
extern nd_PMTU_minval;
#endif
BOOLEAN AT_SetMTU(char* ipver, TCPIP_NETID_T netid, uint32 mtu, char* resp)
{
    BOOLEAN ret = 0;
    uint32 ipver_num = 0;
    if (ipver != NULL)
    {
        if (strncasecmp(ipver, "V4", 2) == 0)
        {
            ipver_num = 4;
        }
        else if (strncasecmp(ipver, "V6", 2) == 0)
        {
            ipver_num = 6;
        }
        else
        {
            sprintf(resp, "Error input, format: AT+MTU=V4/V6,net name,mtu value");
            return 1;
        }
        SCI_TRACE_LOW("[%s %d] version:%s\n", __FUNCTION__, __LINE__, ipver);
    }
    else
    {
        sprintf(resp, " Error, format: AT+MTU=V4/V6,net name,mtu value");
        return 1;
    }

    if (ipver_num == 4)
        {
            ret = sci_set_mtu(netid, AF_INET, mtu);
            if (ret == SETMTU_INVALID_MTU)
            {
                sprintf(resp, "The ipv4 mtu value should between [%d~%d]\r\n", 576, 1500);
                return 1;
            }
            else if (ret == SETMTU_INVALID_NETIF)
            {
                sprintf(resp, "The net interface is not exist!\r\n");
                return 1;
            }
        }
    else if(ipver_num == 6)
        {
            ret = sci_set_mtu(netid, AF_INET6, mtu);
            if (ret == SETMTU_INVALID_MTU)
            {
                sprintf(resp, "The ipv6 mtu value should between [%d~%d]\r\n", 1280, nd_PMTU_minval);
                return 1;
            }
            else if (ret == SETMTU_INVALID_NETIF)
            {
                sprintf(resp, "The net interface is not exist!\r\n");
                return 1;
            }
        }

    if (ret == 0)
    {
        sci_get_mtu(netid, AF_INET, &mtu);
        sci_get_mtu(netid, AF_INET6, &mtu);
    }

    return ret;
}

/*****************************************************************************/
//  Description: This function handles the AT+MTU command .
//  Example:     AT+MTU=?  (can view example);
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMTU)
{
#ifdef PLATFORM_UWS6121E
    ATC_CPWD_PARAM_T      param[3] = {0};
    uint8                               cmd_type = 0;
    ATC_CPWD_PARAM_T *      p_param;
    char *ipver = NULL;
    char *netname = NULL;
    char *mtu_str = NULL;
    TCPIP_NETID_T netid = 0;
    uint32 mtu = 0;
    uint32 ret = 0;

    if (getATCommandParams(cur_cmd_info, &cmd_type, param, 3) != S_ATC_SUCCESS)
    {
        sprintf((char *)g_rsp_str, "+MTU:Error Param");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }

    switch (cmd_type)
    {
        case ATC_CMD_TYPE_SET:
            p_param = param;
            if(*param[0].param=='=')
            {
                if(param[0].param_len>1 && param[1].param_len>0 && param[2].param_len>0)
                {

                    if((strcasecmp(param[0].param+1,"V4") == 0) || (strcasecmp(param[0].param+1,"V6") == 0))
                    {
                        ipver = param[0].param+1;
                        SCI_TRACE_LOW("ipver is %s\n", ipver);
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "+MTU:Error, format: AT+MTU=V4/V6,net name,mtu value\n ");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }
                    netname = param[1].param;
                    SCI_TRACE_LOW("netname is %s\n", netname);
                    netid = sci_get_netidbyname(netname);
                    if(netid == -1)
                    {
                        sprintf(g_rsp_str, "%s\r\n", "+MTU:The net interface is not exist!");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }
                    SCI_TRACE_LOW("mtu_str is %s\n", mtu_str);
                    mtu_str = param[2].param;
                    mtu = atoi(mtu_str);
                    SCI_TRACE_LOW("mtu is %d\n", mtu);
                    ret = AT_SetMTU(ipver, netid, mtu, g_rsp_str);
                    if (ret == 0)
                    {
                        sprintf((char *)g_rsp_str, "Set MTU OK!");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;
                    }
                    else
                    {
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+MTU:Error Param");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_FAIL;
                }
            }
            break;
        case ATC_CMD_TYPE_READ:
            break;
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "AT+MTU=V4/V6,net name,mtu value");
            break;
        default:
            sprintf((char *)g_rsp_str, "+MTU:Error Param");
            break;
    }
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#endif
    return S_ATC_SUCCESS;
}

#ifdef PLATFORM_UWS6121E
extern PUBLIC int query_get_traffic(void);
extern PUBLIC int query_clear_traffic(void);
extern PUBLIC int query_traffic_get_result(char *at_rsp);
#endif
/*****************************************************************************/
//  Description: This function handles the AT+TRAFFIC command
//               used for query traffic.
//  Example:     AT+TRAFFIC="sim"--query traffic
//                    AT+TRAFFIC="clear sim"--clear last traffic
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessTRAFFIC)
{
#ifdef PLATFORM_UWS6121E
    ATC_CPWD_PARAM_T      param[1] = {0};
    uint8                               cmd_type = 0;
    ATC_CPWD_PARAM_T *      p_param;
    char at_rsp[100] = {0};

    if (getATCommandParams(cur_cmd_info, &cmd_type, param, 1) != S_ATC_SUCCESS)
    {
        sprintf((char *)g_rsp_str, "+TRAFFIC:Error Param");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }

    switch (cmd_type)
    {
        case ATC_CMD_TYPE_SET:
            p_param = param;
            if(*param[0].param=='=')
            {
                if(param[0].param_len>1)
                {
                    if(strcasecmp(param[0].param+1,"sim") == 0)
                    {
                        query_get_traffic();
                        query_traffic_get_result(at_rsp);
                        sprintf((char *)g_rsp_str, "%s", at_rsp);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;
                    }
                    else if(strcasecmp(param[0].param+1,"clear sim") == 0)
                    {
                        query_clear_traffic();
                        query_traffic_get_result(at_rsp);
                        sprintf((char *)g_rsp_str, "%s", at_rsp);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "+TRAFFIC:Error, format: AT+TRAFFIC=sim/clear sim\n ");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+TRAFFIC:Error Param");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_FAIL;
                }
            }
            break;
        case ATC_CMD_TYPE_READ:
            break;
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "AT+TRAFFIC=sim/clear sim");
            break;
        default:
            sprintf((char *)g_rsp_str, "+TRAFFIC:Error Param");
            break;
    }
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#endif
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description: This function handles the AT+DAPSIPERF command .
//  Example:     AT+DAPSIPERF=?  (can view example);
//  Author:       Xirui.Zhang
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessDAPSIPERF)
{
#ifdef PLATFORM_UWS6121E
    ATC_CPWD_PARAM_T      param[9] = {0};
    uint8                               cmd_type = 0;
    uint32                              netid = 0;
    uint8                               ndis_mode;
    ATC_CPWD_PARAM_T *      p_param;
    uint8                               flag=0;
    uint32                             netid_check_ppp;

    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T *pdp_context_arr_ptr = 0;
    uint32              pdp_context_num = 0;
    BOOLEAN             is_pdpactived = FALSE;
    uint8 nsapi=0;
    int8 i = 0;

    if (getATCommandParams(cur_cmd_info, &cmd_type, param, 9) != S_ATC_SUCCESS)
    {
        sprintf((char *)g_rsp_str, "DAPSIPERF:Error Param");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }

    switch (cmd_type)
    {
        case ATC_CMD_TYPE_SET:
//            SDI_TRACEID_INTEGER(ATC, AT_Cmd_DAPSIPERF_enter, "AT_Cmd DAPSIPERF enter");
            p_param = param;
            if(*param[0].param=='=')
            {
                if(param[0].param_len>1 && param[1].param_len>0 && param[2].param_len>0)
                {
                    //set netid  module:ppp,usb,wan  mifi:usb,wan,wifi
                    //if ppp or usb tethering mode, set wan netid. if not, return error mode
                    if(strcasecmp(param[0].param+1,"wan") == 0)
                    {
                        if(ERR_MNGPRS_NO_ERR ==
                            MNGPRS_ReadPdpContextStateEx(dual_sys,&pdp_context_arr_ptr,&pdp_context_num))
                        {
                            if(pdp_context_arr_ptr != 0)
                            {
                                for(i = 0; i < pdp_context_num; i++)
                                {
                                    if(pdp_context_arr_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                                    {
                                        nsapi = MNGPRS_GetNsapiByCidEx(dual_sys, pdp_context_arr_ptr[i].pdp_id);
                                        is_pdpactived = TRUE;
                                        break;
                                    }
                                }
                                SCI_FREE(pdp_context_arr_ptr);
                            }
                            else
                            {
                                return S_ATC_FAIL;
                            }
                        }
                        else
                        {
                             return S_ATC_FAIL;
                        }

#if 1
                        if (!is_pdpactived)
                        {
                            sprintf((char *)g_rsp_str, "+DAPSIPERF:Please activate wan net interface first");
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            return S_ATC_SUCCESS;
                        }
#endif
                        netid = nsapi;
                        p_param++;
                    }
#if 0
                    else if(strcasecmp(param[0].param+1,"ppp") == 0)
                    {
//                        SDI_TRACEID_INTEGER(ATC, DAPSIPERF__enter_ppp, "DAPSIPERF: enter ppp");
                        netid = PPP_GetNetId();
//                        SDI_VTRACEID_INTEGER(ATC, DAPSIPERF__ppp_netid_____d, "%d", "%d", "DAPSIPERF: ppp netid--->%d", 1, (netid));
                        if(netid)
                        {
                            if(PPP_GetPPPMode(&ndis_mode) == PPP_E_SUCCESS && ndis_mode == TRUE)
                            {
                                sprintf((char *)g_rsp_str, "+DAPSIPERF:DAPSIPERF only support Tethering mode, please restart dialing! ");
                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                return S_ATC_SUCCESS;
                            }
                        }
                        else
                        {
                            sprintf((char *)g_rsp_str, "+DAPSIPERF:Please start PPP mode first");
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	                     return S_ATC_SUCCESS;
                        }
                        p_param++;
                    }
#endif
                    else
                    {
                        sprintf((char *)g_rsp_str, "+DAPSIPERF:please input the first parameters to set net interface, like wan ");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_SUCCESS;
                    }

                    if(iperf_func((void *)p_param, netid, atc_config_ptr->current_link_id))
                    {
                        sprintf((char *)g_rsp_str, "+DAPSIPERF: Failed !!!");
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "+DAPSIPERF: Successful !!!");
                    }
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+DAPSIPERF:wrong param");
                }
            }
            break;
        case ATC_CMD_TYPE_READ:
//            SDI_TRACEID_INTEGER(ATC, AT_Cmd_DAPSIPERF_enter_read, "AT_Cmd DAPSIPERF enter read");
            sprintf((char *)g_rsp_str, "+DAPSIPERF:this is read mode");
            break;
        case ATC_CMD_TYPE_TEST:
//            SDI_TRACEID_INTEGER(ATC, AT_Cmd_DAPSIPERF_enter_test_, "AT_Cmd DAPSIPERF enter test ");
            sprintf((char *)g_rsp_str, "+DAPSIPERF:this is template\r\n"
                                                    "tcp server: \r\nAT+DAPSIPERF=wan,s,tcp,port,parallel(0-10),interval(0/1)\r\n"
                                                    "tcp client: \r\nAT+DAPSIPERF=wan,c,tcp,ip,port,time,parallel(0-10),interval(0/1)\r\n"
                                                    "udp server: \r\nAT+DAPSIPERF=wan,s,udp,port,interval(0/1)\r\n"
                                                    "udp client: \r\nAT+DAPSIPERF=wan,c,udp,ip,port,time,bw,parallel(0-10),interval(0/1)\r\n");
            break;
        default:
            sprintf((char *)g_rsp_str, "+DAPSIPERF:Error Param");
            break;
    }
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#endif
    return S_ATC_SUCCESS;

}

/*****************************************************************************/
// Description:  Check if the input string contains only numbers.
// Global resource : none
// Note : PARAM1: const char *str (IN): the input string.
//        RETURN: TRUE - only numbers;
//                FALSE - include other char.
/*****************************************************************************/
BOOLEAN at_isNum(const char *str)
{
    int i;

    if ( str == NULL )
        return FALSE;

    for ( i = 0; i < strlen ( str ); ++i )
    {
        if ( str[i] < '0' || str[i] > '9' )
        {
            return FALSE;
        }
    }

    return TRUE;
}

int at_optind = 1;
int at_optopt;
char *at_optarg;
int at_getopt(int argc, char *const argv[], const char *optstring)
{
	static int optchr = 1;
	char *cp;
	if (optchr == 1) {
		if (at_optind >= argc) {
			/* all arguments processed */
			return EOF;
		}

		if (argv[at_optind][0] != '-' || argv[at_optind][1] == '\0') {
			/* no option characters */
			return EOF;
		}
	}

	if (strcmp(argv[at_optind], "--") == 0) {
		/* no more options */
		at_optind++;
		return EOF;
	}

	at_optopt = argv[at_optind][optchr];
	cp = (char *)strchr(optstring, at_optopt);
	if (cp == NULL || at_optopt == ':') {
		if (argv[at_optind][++optchr] == '\0') {
			optchr = 1;
			at_optind++;
		}
		return '?';
	}

	if (cp[1] == ':') {
		/* Argument required */
		optchr = 1;
		if (argv[at_optind][optchr + 1]) {
			/* No space between option and argument */
			at_optarg = &argv[at_optind++][optchr + 1];
		} else if (++at_optind >= argc) {
			/* option requires an argument */
			return '?';
		} else {
			/* Argument in the next argv */
			at_optarg = argv[at_optind++];
		}
	} else {
		/* No argument */
		if (argv[at_optind][++optchr] == '\0') {
			optchr = 1;
			at_optind++;
		}
		at_optarg = NULL;
	}
	return *cp;
}

/*****************************************************************************/
//  Description: This function handles the AT+PING command
//               used for ping test.
//  Example:     AT+PING=V4,www.baidu.com
//               AT+PING=V6,ipv6.ustb.edu.cn
//  Author:      qin.he
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessPING)
{
#ifdef PLATFORM_UWS6121E
    ATC_CPWD_PARAM_T      param[4] = {0};
    uint8                               cmd_type = 0;
    ATC_CPWD_PARAM_T *      p_param;
    uint32 datalen = 0;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T *pdp_context_arr_ptr = 0;
    uint32              pdp_context_num = 0;
    BOOLEAN             is_pdpactived = FALSE;
    uint8 nsapi = 5;
    int8 i = 0;
    int32 res;
    char* ipver = NULL;
    char* host_name = NULL;
    char* datalen_str = NULL;
    uint32 host_name_size;
    int opt = 0;
    TCPIP_PING_OPTION_PARAM_T optvalue = {0};

    SCI_TRACE_LOW("ATC_ProcessPING");
    if (getATCommandParams(cur_cmd_info, &cmd_type, param, 4) != S_ATC_SUCCESS)
    {
        sprintf((char *)g_rsp_str, "+PING:Error Param");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_FAIL;
    }
    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
            p_param = param;
            if(*param[0].param=='=')
            {
                if(ERR_MNGPRS_NO_ERR ==
                    MNGPRS_ReadPdpContextStateEx(dual_sys,&pdp_context_arr_ptr,&pdp_context_num))
                {
                    if(pdp_context_arr_ptr != 0)
                    {
                        for(i = 0; i < pdp_context_num; i++)
                        {
                            if(pdp_context_arr_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                            {
                                nsapi = MNGPRS_GetNsapiByCidEx(dual_sys, pdp_context_arr_ptr[i].pdp_id);
                                is_pdpactived = TRUE;
                                break;
                            }
                        }
                        SCI_FREE(pdp_context_arr_ptr);
                        if(is_pdpactived == FALSE)
                        {
                            sprintf((char *)g_rsp_str, "Please pdp activate first\n");
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            return S_ATC_FAIL;
                        }
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "pdp error\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }
                }

                if(param[0].param_len>1 && param[1].param_len>0)
                {
                    if((strcasecmp(param[0].param+1,"V4") == 0) || (strcasecmp(param[0].param+1,"V6") == 0))
                    {
                        ipver = param[0].param+1;
                        SCI_TRACE_LOW("ipver is %s\n", ipver);
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "+PING:Error Param ");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }
                    host_name_size = param[1].param_len;
                    host_name = param[1].param;
                    SCI_TRACE_LOW("host_name is %s\n", host_name);
                    if(param[2].param_len>0 || param[3].param_len>0 )
                    {
                        int i = 0;
                        char* argv[2] = {0};
                        //array to 2d array
                        if( param[2].param_len>0 )
                        {
                            if( param[2].param[0] == '-' )
                            {
                                argv[i] = param[2].param;
                                SCI_TRACE_LOW("param[2].param is %s, argv[%d]=%s\n", param[2].param, i, argv[i]);
                                i++;
                            }
                            else
                            {
                                sprintf((char *)g_rsp_str, "+PING:Error Param ");
                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                return S_ATC_FAIL;
                            }
                        }
                        if( param[3].param_len>0 )
                        {
                            if( param[2].param[0] == '-' )
                            {
                                argv[i] = param[3].param;
                                SCI_TRACE_LOW("param[3].param is %s, argv[%d]=%s\n",param[3].param, i, argv[i]);
                                i++;
                            }
                            else
                            {
                                sprintf((char *)g_rsp_str, "+PING:Error Param ");
                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                return S_ATC_FAIL;
                            }
                        }

                        //analyzsis -t and -l
                        at_optind = 0;
                        while( (opt = at_getopt( i, argv, "t:l:")) != -1)
                        {
                            SCI_TRACE_LOW("opt is %d %s\n", opt, opt);
                            switch( opt )
                            {
                                case 't':
                                    SCI_TRACE_LOW("optarg is %s\n", at_optarg);
                                    if(at_optarg[0]==' ' && at_isNum(++at_optarg))
                                    {
                                        optvalue.spend_time = atoi(at_optarg);
                                        SCI_TRACE_LOW("expect spend_time is %d\n", optvalue.spend_time);
                                    }
                                    else
                                    {
                                        sprintf((char *)g_rsp_str, "+PING:Error Param ");
                                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                        return S_ATC_FAIL;
                                    }
                                    break;

                                case 'l':
                                    SCI_TRACE_LOW("optarg is %s\n", at_optarg);
                                    if(at_optarg[0]==' ' && at_isNum(++at_optarg))
                                    {
                                        optvalue.data_len = atoi(at_optarg);
                                        if( optvalue.data_len > 65500 )
                                        {
                                            sprintf((char *)g_rsp_str, "+PING:'-l' value must be 0 to 65500");
                                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                            return S_ATC_FAIL;
                                        }
                                        SCI_TRACE_LOW("datalen is %d\n", optvalue.data_len);
                                    }
                                    else
                                    {
                                        sprintf((char *)g_rsp_str, "+PING:Error Param ");
                                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                        return S_ATC_FAIL;
                                    }
                                    break;

                                case '?':
                                    if( at_optopt == 't' || at_optopt == 'l'  )
                                    {
                                        SCI_TRACE_LOW("please input value of '-t' or '-l' optopt = %s\n", at_optopt);
                                        sprintf((char *)g_rsp_str, "+PING:please input value of '-t' or '-l' ");
                                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                    }
                                    else
                                    {
                                        sprintf((char *)g_rsp_str, "+PING:Error Param ");
                                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                    }
                                    return S_ATC_FAIL;

                                default:
                                    SCI_TRACE_LOW(" other\n");
                            }
                        }
                    }
                    else
                    {
                        optvalue.data_len = 0;
                        optvalue.spend_time = 0;
                        SCI_TRACE_LOW("datalen is %d  spend_time is %d\n", optvalue.data_len, optvalue.spend_time);
                    }
                    sprintf((char *)g_rsp_str, "Please wait\n");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    AT_PingStart(ipver, host_name, optvalue, host_name_size, 0, nsapi, 3);
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+PING:Error Param ");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_FAIL;
                }
            }
            break;

        case ATC_CMD_TYPE_READ:
            break;
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "AT+PING=V4/V6,host_name,-l datalen,-t sec ");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        default:
            sprintf((char *)g_rsp_str, "+PING:Error Param");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    }
#endif
    return S_ATC_SUCCESS;
}

#ifdef PLATFORM_UWS6121E
extern PUBLIC int speed_test_exit(void);
extern PUBLIC void speed_test_main(uint8 is_async, uint32 netid, uint32 timeout);
#endif
/*****************************************************************************/
//  Description: This function handles the AT+SACK command
//               used for ping test.
//  Example:     AT+SACK=1, 100 //1:sync start test; 100: test timeout: 100s
//                     AT+SACK=2, 100 //2:Async start test; 100: test timeout: 100s
//                     AT+SACK=0  // stop test
//  Author:      qin.he
/*****************************************************************************/
ATC_STATUS ATC_ProcessSACK(
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                )
{
    ATC_STATUS  at_status = S_ATC_SUCCESS;
#ifdef PLATFORM_UWS6121E
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T *pdp_context_arr_ptr = 0;
    uint32              pdp_context_num = 0;
    BOOLEAN             is_pdpactived = FALSE;
    uint8 nsapi=5;
    int8 i = 0;
    uint32 timeout = 0;

    SCI_TraceLow("ATC_ProcessSACK");

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(ERR_MNGPRS_NO_ERR ==
                MNGPRS_ReadPdpContextStateEx(dual_sys,&pdp_context_arr_ptr,&pdp_context_num))
            {
                if(pdp_context_arr_ptr != 0)
                {
                    for(i = 0; i < pdp_context_num; i++)
                    {
                        if(pdp_context_arr_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                        {
                            nsapi = MNGPRS_GetNsapiByCidEx(dual_sys, pdp_context_arr_ptr[i].pdp_id);
                            is_pdpactived = TRUE;
                            break;
                        }
                    }
                    SCI_FREE(pdp_context_arr_ptr);
                }
                else
                {
                    return S_ATC_FAIL;
                }
            }
            else
            {
                 return S_ATC_FAIL;
            }

            if (!is_pdpactived)
            {
                 sprintf((char *)g_rsp_str, "no pdp actived");
                 ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                 return S_ATC_FAIL;
            }


            if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            timeout = PARA2.num;

            if(PARA1.num == 1)
            {
                speed_test_main(FALSE, nsapi, timeout);
            }
            else if(PARA1.num == 2)
            {
                speed_test_main(TRUE, nsapi, timeout);
            }
            else if(PARA1.num == 0)
            {
                speed_test_exit();
            }
            else
            {
                 return ERR_OPERATION_NOT_SUPPORTED;
            }
            break;
        }
        case ATC_CMD_TYPE_TEST:
        {
            break;
        }
        default:
        {
            at_status = ERR_OPERATION_NOT_SUPPORTED;
            break;
        }
    }
#endif
    return (at_status);
}

#ifdef PLATFORM_UWS6121E
extern PUBLIC int socket_test_exit(void);
extern PUBLIC void socket_test_main(uint8 is_async, uint32 netid, uint32 interval);
#endif
/*****************************************************************************/
//  Description: This function handles the AT+SOCKET command
//               used for ping test.
//  Example:     AT+SOCKET=1, "loopback", 5 //1:sync start test; loopback/gprs: use loopback/gprs netif; 5: send interval is 5s
//                     AT+SOCKET=2, "loopback", 5 //1:Async start test; loopback/gprs: use loopback/gprs netif; 5: send interval is 5s
//                     AT+SOCKET=0  // stop test
//  Author:      qin.he
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSOCKET)
{
#ifdef PLATFORM_UWS6121E
    ATC_CPWD_PARAM_T      param[3] = {0};
    uint8                               cmd_type = 0;
    ATC_CPWD_PARAM_T *      p_param;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T *pdp_context_arr_ptr = 0;
    uint32              pdp_context_num = 0;
    BOOLEAN             is_pdpactived = FALSE;
    uint8 nsapi=5;
    uint32 netid = 0xffffffff;
    int8 i = 0;
    uint32 interval = 0;
    char* interval_ch = NULL;
    char* netname = NULL;

    if (getATCommandParams(cur_cmd_info, &cmd_type, param, 3) != S_ATC_SUCCESS)
    {
        sprintf((char *)g_rsp_str, "+SOCKET:Error");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }

    switch (cmd_type)
    {
        case ATC_CMD_TYPE_SET:
            p_param = param;
            if(*param[0].param == '=')
            {
                if(param[0].param_len>1 && param[1].param_len>1 && param[2].param_len>1)
                {
                    netname = param[1].param;
                    SCI_TRACE_LOW("netname is %s\n", netname);
                    if((strcasecmp("gprs", netname) == 0) || (strcasecmp("loopback", netname) == 0))
                    {
                        if(strcasecmp("gprs", netname) == 0)
                        {
                            if(ERR_MNGPRS_NO_ERR ==
                                MNGPRS_ReadPdpContextStateEx(dual_sys,&pdp_context_arr_ptr,&pdp_context_num))
                            {
                                if(pdp_context_arr_ptr != 0)
                                {
                                    for(i = 0; i < pdp_context_num; i++)
                                    {
                                        if(pdp_context_arr_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                                        {
                                            nsapi = MNGPRS_GetNsapiByCidEx(dual_sys, pdp_context_arr_ptr[i].pdp_id);
                                            is_pdpactived = TRUE;
                                            break;
                                        }
                                    }
                                    SCI_FREE(pdp_context_arr_ptr);
                                }
                                else
                                {
                                    sprintf((char *)g_rsp_str, "pdp_context_arr_ptr = 0,fail");
                                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                    return S_ATC_FAIL;
                                }
                            }
                            else
                            {
                                sprintf((char *)g_rsp_str, "Read PdpContext State fail");
                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                return S_ATC_FAIL;
                            }
                            if (!is_pdpactived)
                            {
                                 sprintf((char *)g_rsp_str, "no pdp actived");
                                 ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                 return S_ATC_FAIL;
                            }
                            netid = nsapi;
                        }
                    }
                    else
                    {
                         sprintf((char *)g_rsp_str, "+SOCKET:Error Netname");
                         ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                         return S_ATC_FAIL;
                    }

                    interval_ch = param[2].param;
                    interval = atoi(interval_ch);
                    SCI_TRACE_LOW("interval is %d\n", interval);
                    if(interval < 0 || interval > 65535)
                    {
                        sprintf((char *)g_rsp_str, "+SOCKET:interval time should be 0~65535");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }

                    if((strcasecmp(param[0].param+1,"1") == 0) || (strcasecmp(param[0].param+1,"2") == 0))
                    {
                        if(strcasecmp(param[0].param+1,"1") == 0)
                        {
                            SCI_TRACE_LOW("+SOCKET:sync start test");
                            socket_test_main(FALSE, netid, interval);
                            sprintf((char *)g_rsp_str, "+SOCKET:SUCCESS");
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            return S_ATC_SUCCESS;
                        }
                        else if(strcasecmp(param[0].param+1,"2") == 0)
                        {
                            SCI_TRACE_LOW("+SOCKET:async start test");
                            socket_test_main(FALSE, netid, interval);
                            sprintf((char *)g_rsp_str, "+SOCKET:SUCCESS");
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            return S_ATC_SUCCESS;
                        }
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "+SOCKET:Error");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }
                }
                else if(param[0].param_len > 1 && param[1].param_len <1 && param[2].param_len <1 && strcasecmp(param[0].param+1,"0") == 0)
                {
                    socket_test_exit();
                    sprintf((char *)g_rsp_str, "+SOCKET:EXIT SUCCESS");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_SUCCESS;
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+SOCKET:Error");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_FAIL;
                }
            }
            break;
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "AT+SOCKET=0/1/2,gprs,interval time");
            break;
        default:
            sprintf((char *)g_rsp_str, "+SOCKET:Error Param");
            break;
    }
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#endif
    return S_ATC_SUCCESS;
}

#ifdef PLATFORM_UWS6121E
extern PUBLIC void http_test_setnetid(uint32 netid);
extern PUBLIC int http_test_exit(void);
extern PUBLIC int http_test_main(uint8 method,uint8 *param,uint32 break_start, uint32 break_end);
#endif
/*****************************************************************************/
//  Description: This function handles the AT+HTTP command
//               used for ping test.
//  Example:     AT+HTTP="https://www.baidu.com"
//  Author:      qin.he
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessHTTP)
{
    ATC_STATUS  at_status = S_ATC_SUCCESS;
#ifdef PLATFORM_UWS6121E
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T *pdp_context_arr_ptr = 0;
    uint32              pdp_context_num = 0;
    BOOLEAN             is_pdpactived = FALSE;
    uint8 nsapi=5;
    int8 i = 0;
    uint8 *temp_ptr;
    uint8 param_len;
    char at_param[201] = {0};
    uint8 method = 0XFF;
    uint32 breakstart = 0;
    uint32 breakend = 0;
    SCI_TraceLow("ATC_ProcessHTTP");

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(NULL != cur_cmd_info); /*assert verified*/

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(ERR_MNGPRS_NO_ERR ==
                MNGPRS_ReadPdpContextStateEx(dual_sys,&pdp_context_arr_ptr,&pdp_context_num))
            {
                if(pdp_context_arr_ptr != 0)
                {
                    for(i = 0; i < pdp_context_num; i++)
                    {
                        if(pdp_context_arr_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                        {
                            nsapi = MNGPRS_GetNsapiByCidEx(dual_sys, pdp_context_arr_ptr[i].pdp_id);
                            is_pdpactived = TRUE;
                            break;
                        }
                    }
                    SCI_FREE(pdp_context_arr_ptr);
                }
                else
                {
                    return S_ATC_FAIL;
                }
            }
            else
            {
                 return S_ATC_FAIL;
            }

            if (!is_pdpactived)
            {
                 sprintf((char *)g_rsp_str, "no pdp actived");
                 ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                 return S_ATC_FAIL;
            }


            if(!PARAM1_FLAG || (PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING))
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            if(!PARAM2_FLAG || (PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING))
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            temp_ptr = PARA1.str_ptr->str_ptr;
            param_len = PARA1.str_ptr->str_len;
            SCI_TraceLow("ATC_ProcessHTTP, str_len: %d, str: %s",param_len,temp_ptr);
            if(!memcmp(temp_ptr,"GET",param_len))
            {
                method = 0;
                SCI_TraceLow("ATC_ProcessHTTP, PARAM3_TYPE: %d\n",PARAM3_TYPE);
                if(PARAM3_FLAG && (PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
                {
                     breakstart = (uint32)PARA3.num;
                }
                if(PARAM4_FLAG && (PARAM4_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
                {
                    breakend = (uint32)PARA4.num;
                }
                if(breakstart>0 && breakend == 0)
                {
                    sprintf((char *)g_rsp_str, "range set error");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else if(!memcmp(temp_ptr,"DELETE",param_len))
            {
                 method = 1;
            }
            else if(!memcmp(temp_ptr,"HEAD",param_len))
            {
                 method = 2;
            }
            else
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            SCI_TraceLow("ATC_ProcessHTTP, method %d\n",method);
            param_len =PARA2.str_ptr->str_len;
            if(param_len > 200)
            {
                param_len = 200;
            }
            memcpy((uint8 *)at_param, PARA2.str_ptr->str_ptr, param_len);
            http_test_setnetid(nsapi);
            http_test_exit();
            http_test_main(method,at_param,breakstart,breakend);
            return S_ATC_DEFAULT_HANDLE;
        }
        case ATC_CMD_TYPE_TEST:
        {
            break;
        }
        default:
        {
            at_status = ERR_OPERATION_NOT_SUPPORTED;
            break;
        }
    }
#endif
    return (at_status);
}

#ifdef MQTT_SUPPORT
char * g_host =NULL;
extern  struct MqttOpts g_stMqttOpts;
extern MQTTAsync g_stMqttClient;
extern mqtt_netid;
#endif
/*****************************************************************************/
// Description: This function handles the AT+MQTTCONN command
//                      used for create MQTT connection
// Example:     AT+MQTTCONN=<host>,<port>, <clientid>, <keepalive>, <cleansession>, [username], [password]
//		host--host name of MQTT server
//             port--port of MQTT server
//             clientid--client id of MQTT client
//             keepalive--interval time of send heardbeat packet to MQTT server
//             cleansession--wheather clean session
// Author:      hongqun.cheng
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMQTTCONN)
{
    int32  status = S_ATC_SUCCESS;
#if defined MQTT_SUPPORT
    int rc = 0;
    char *pcHost = NULL;
    int iPort = MQTT_PORT_DEF;
    char cPort[6] = {'\0'};
    char cUrl[MQTT_HOSTNAME_LEN_MAX + 6] = {'\0'};
    char *pcClientId = NULL;
    int iKeepLive = MQTT_KEEPLIVE_DEF;
    int iCleanSession = MQTT_CLEANSESSION_DEF;
    char *pcUserName = NULL;
    char *pcPassword = NULL;

    //netid
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T * ret_state_ptr = 0;
    uint32 pdp_context_num = 0;
    uint8 nsapi = 5;
    int8 i=0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if((5 == cur_cmd_info->cur_param_num) || (7 == cur_cmd_info->cur_param_num))
            {
                if((PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA1.str_ptr->str_ptr) > 0) && (strlen(PARA1.str_ptr->str_ptr) < MQTT_HOSTNAME_LEN_MAX - 1 ))
                && (PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC )
                &&(PARAM3_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA3.str_ptr->str_ptr) > 0) && (strlen(PARA3.str_ptr->str_ptr) < MQTT_CLIENTID_LEN_MAX - 1 ))
                && (PARAM4_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC )&&(PARA4.num >= 30 && PARA4.num <= 1800)
                && (PARAM5_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC ))
                {
                    if((7 == cur_cmd_info->cur_param_num))
                    {
                        if((PARAM6_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                        && ((strlen(PARA6.str_ptr->str_ptr) > 0) && (strlen(PARA6.str_ptr->str_ptr) < MQTT_USERNAME_LEN_MAX - 1 ))
                        && (PARAM7_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                        && ((strlen(PARA7.str_ptr->str_ptr) > 0) && (strlen(PARA7.str_ptr->str_ptr) < MQTT_PASSWORD_LEN_MAX - 1 )))
                        {
                            pcUserName = PARA6.str_ptr->str_ptr;
                            pcPassword = PARA7.str_ptr->str_ptr;
                        }
                        else
                        {
                            status = ERR_OPERATION_NOT_ALLOWED;
                            SCI_TRACE_LOW(" [ MQTT ]  +MQTTCONN: parameter error  [%s %d]  \n", __FUNCTION__,__LINE__);
                            sprintf((char *)g_rsp_str, "\r\n+MQTTCONN: Parameter Error\r\n");
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            break;
                        }
                    }

                    //get netid
                    if(ERR_MNGPRS_NO_ERR == MNGPRS_ReadPdpContextStateEx(dual_sys,  &ret_state_ptr, &pdp_context_num))
                    {
                        if(ret_state_ptr != 0)
                        {
                            for(i=0;i<pdp_context_num;i++)
                            {
                                if (ret_state_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                                {
                                    nsapi = MNGPRS_GetNsapiByCidEx(dual_sys,ret_state_ptr[i].pdp_id);
                                    SCI_TRACE_LOW(" MQTT  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                                    break;
                                }
                            }
                        }
                    }

                    if (nsapi != 0)
                    {
                        SCI_TRACE_LOW(" MQTT  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                        mqtt_netid = nsapi;
                    }

                    //mqtt_netid = MQTT_MNGPRS_GetPdpContextNsapiEx();
                    socket_SetNetId(mqtt_netid);


                    if(0 == mqtt_netid)
                    {
                        sprintf((char *)g_rsp_str, "+MQTTCONN: Please activate net interface first");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        //return S_ATC_FAIL;
                    }


                    if (CONNECTING == g_stMqttOpts.m_eState || CONNECTED == g_stMqttOpts.m_eState)
                    {
                        SCI_TRACE_LOW("  [ MQTT ] Mqtt connection state is error.  [%s %d]  \n",__FUNCTION__,__LINE__);
                        sprintf((char *)g_rsp_str, "\r\n +MQTTCONN: Network Connect Fail\r\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }

                    MqttOptsInit();
                    g_stMqttOpts.m_eState = CONNECTING;

                    pcHost = PARA1.str_ptr->str_ptr;
                    iPort = PARA2.num;
                    cPort[0] = '\0';
                    sprintf(cPort, "%d", iPort);
                    pcClientId = PARA3.str_ptr->str_ptr;
                    iKeepLive = PARA4.num;
                    iCleanSession = PARA5.num;

                    g_stMqttOpts.m_eMQTTVersion = MQTTVERSION_3_1_1;
                    g_stMqttOpts.m_pcHost = pcHost;
                    g_stMqttOpts.m_pcPort = cPort;
                    g_stMqttOpts.m_iCleanSession = iCleanSession;
                    g_stMqttOpts.m_pcUsername = pcUserName;
                    g_stMqttOpts.m_pcPassword = pcPassword;
                    g_stMqttOpts.m_pcClientid = pcClientId;
                    g_stMqttOpts.m_iKeepalive = iKeepLive;
                    g_stMqttOpts.m_pcConnection = pcHost;
                    if(strstr(pcHost,"ssl://") != NULL)
                    {
                        g_host = strchr(pcHost,'/')+2;
                    }
                    else
                    {
                        g_host = pcHost;
                    }

                    sprintf(cUrl, "%s:%s", g_stMqttOpts.m_pcHost, g_stMqttOpts.m_pcPort);
                    SCI_TRACE_LOW(" [ MQTT ]  pcHost is: %s,  pcPort is: %s  g_host  : %s [%s %d]  \n", g_stMqttOpts.m_pcHost, g_stMqttOpts.m_pcPort,g_host, __FUNCTION__,__LINE__);

                    if (NULL != g_stMqttOpts.m_pcUsername && NULL != g_stMqttOpts.m_pcPassword)
                    {
                        SCI_TRACE_LOW(" [ MQTT ]  pcUsername is: %s,  pcPassword is: %s  [%s %d]  \n", g_stMqttOpts.m_pcUsername, g_stMqttOpts.m_pcPassword, __FUNCTION__,__LINE__);
                    }

                    MQTTAsync_init();
                    MQTTAsync_create(&g_stMqttClient, cUrl, g_stMqttOpts.m_pcClientid, 1, NULL);
                    MQTTAsync_setCallbacks(g_stMqttClient, g_stMqttClient, mqtt_ConnectionLostCb, mqtt_MessageArrivedCb, NULL);
                    rc = AT_MQTT_MessageHandleTask(atc_config_ptr->current_link_id);

                    if(rc == -1)
                    {
                        sprintf((char *)g_rsp_str, "\r\n+MQTTCONN: Message Task Error\r\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }

                    MqttConnect(g_stMqttClient);

                }
                else
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    SCI_DEBUG("+MQTTCONN: parameter error");
                    sprintf((char *)g_rsp_str, "\r\n+MQTTCONN: Parameter Error\r\n");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                SCI_DEBUG("+MQTTCONN: the number of parameters error");
                sprintf((char *)g_rsp_str, "\r\n+MQTTCONN: The Number of Parameters Error\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            break;
        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            SCI_DEBUG( "AT_CmdFunc_MQTTCONN, command type:%d not allowed",  ATC_GET_CMD_TYPE);
            sprintf((char *)g_rsp_str, "\r\n+MQTTCONN: Error Command Type\r\n");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    }
#endif
    return status;
}

/*****************************************************************************/
// Description: This function handles the AT+MQTTPUB command
//                      used for publish a MQTT message on topic
// Example:     AT+MQTTPUB=<topic>,<message>, <qos>, <duplicate>, <retain>
//             topic--topic name
//             message--message of topic
//             qos--quality of mqtt service. Range of [0-2].
//             duplicate--not used, duplicate of MQTT. Range of [0-1].
//             retain--retain of MQTT. Range of [0-1].
// Author:      hongqun.cheng
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMQTTPUB)
{
    int32  status = S_ATC_SUCCESS;
#if defined MQTT_SUPPORT
    char *pcTopic = NULL;
    char *pcMessage = NULL;
    int iQos = MQTT_QOS_DEF;
    int iDup = MQTT_DUP_DEF;
    int iRetain = MQTT_RETAIN_DEF;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(cur_cmd_info->cur_param_num == 5 && (PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING )
            && ((strlen(PARA1.str_ptr->str_ptr) > 0) && (strlen(PARA1.str_ptr->str_ptr) < MQTT_TOPIC_LEN_MAX - 1 ))
            && (PARAM2_TYPE == ATC_CMD_PARAM_TYPE_STRING )
            && ((strlen(PARA2.str_ptr->str_ptr) >= 0) && (strlen(PARA2.str_ptr->str_ptr) < MQTT_MESSAGE_LEN_MAX - 1 ))
            && (PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC )
            &&(PARAM4_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC ) && (PARAM5_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC ))
            {
                if(0 == mqtt_netid)
                {
                    sprintf((char *)g_rsp_str, "+MQTTPUB: Net is error.");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    //return S_ATC_FAIL;
                }

                if (CONNECTING == g_stMqttOpts.m_eState || DISCONNECTED == g_stMqttOpts.m_eState)
                {
                    SCI_TRACE_LOW("  [ MQTT ] Mqtt connection state is error.  [%s %d]  \n",__FUNCTION__,__LINE__);
                    sprintf((char *)g_rsp_str, "\r\n +MQTTPUB: Network Connection State Error\r\n");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_FAIL;
                }

                if (1 != MQTTAsync_isConnected(g_stMqttClient))
                {
                    SCI_TRACE_LOW("  [ MQTT ] Mqtt connection state is error.  [%s %d]  \n",__FUNCTION__,__LINE__);
                    sprintf((char *)g_rsp_str, "\r\n +MQTTPUB: MQTT Connection State Error\r\n");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_FAIL;
                }

                pcTopic = PARA1.str_ptr->str_ptr;
                pcMessage = PARA2.str_ptr->str_ptr;
                iQos = PARA3.num;
                iDup= PARA4.num;
                iRetain = PARA5.num;

                g_stMqttOpts.m_pcTopic = pcTopic;
                g_stMqttOpts.m_pcMessage = pcMessage;
                g_stMqttOpts.m_iQos = iQos;
                g_stMqttOpts.m_iRetained = iRetain;

                MqttPublish(g_stMqttClient, strlen(g_stMqttOpts.m_pcMessage), g_stMqttOpts.m_pcMessage);
                sprintf((char *)g_rsp_str, "\r\n+MQTTPUB: MQTT Publish Success\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                SCI_DEBUG("+MQTTPUB: Error Param");
                sprintf((char *)g_rsp_str, "\r\n+MQTTPUB: Error Param\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            break;
        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            SCI_DEBUG( "AT_CmdFunc_MQTTPUB, command type:%d not allowed",  ATC_GET_CMD_TYPE);
            sprintf((char *)g_rsp_str, "\r\n+MQTTPUB: Error Command Type\r\n");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    }
#endif
    return status;

}

/*****************************************************************************/
// Description: This function handles the AT+MQTTSUBUNSUB command
//                      used for subscribe or unsubscribe a MQTT topic
// Example:     AT+MQTTSUBUNSUB=<topic>,<sub flag>, [qos]
//             topic--topic name
//             sub flag--Range of (0,1). 1-subscribe topic, 0-unsubscribe topic.
//             qos--quality of mqtt service. Be default, when flag is 0. Range of [0-2].
// Author:      hongqun.cheng
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMQTTSUBUNSUB)
{
    int32  status = S_ATC_SUCCESS;
#if defined MQTT_SUPPORT
    char *pcTopic = NULL;
    unsigned char ucSubFlag = MQTT_SUB_FLAG_DEF;
    int iQos = MQTT_QOS_DEF;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(0== mqtt_netid)
            {
                sprintf((char *)g_rsp_str, "+MQTTSUBUNSUB: Net is error.");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                //return S_ATC_FAIL;
            }

            if (CONNECTING == g_stMqttOpts.m_eState || DISCONNECTED == g_stMqttOpts.m_eState)
            {
                SCI_TRACE_LOW("  [ MQTT ] Mqtt connection state is error.  [%s %d]  \n",__FUNCTION__,__LINE__);
                sprintf((char *)g_rsp_str, "\r\n +MQTTSUBUNSUB: Network Connection State Error\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_FAIL;
            }

            if (1 != MQTTAsync_isConnected(g_stMqttClient))
            {
                SCI_TRACE_LOW("  [ MQTT ] Mqtt connection state is error.  [%s %d]  \n",__FUNCTION__,__LINE__);
                sprintf((char *)g_rsp_str, "\r\n +MQTTSUBUNSUB: MQTT Connection State Error\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_FAIL;
            }

            if(cur_cmd_info->cur_param_num == 2 && (PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA1.str_ptr->str_ptr) > 0) && (strlen(PARA1.str_ptr->str_ptr) < MQTT_TOPIC_LEN_MAX - 1 ))
                && (PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC ) && (PARA2.num == MQTT_UNSUB_FLAG))
            {
                pcTopic = PARA1.str_ptr->str_ptr;
                g_stMqttOpts.m_pcTopic = pcTopic;

                SCI_TRACE_LOW("  [ MQTT ]  MQTT Unsubscribe: topic is %s  [%s %d]  \n", g_stMqttOpts.m_pcTopic, __FUNCTION__,__LINE__);

                MqttUnsubscribe(g_stMqttClient);

                sprintf((char *)g_rsp_str, "\r\n+MQTTSUBUNSUB: MQTT Unsubscribe Success\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else if (cur_cmd_info->cur_param_num == 3 && (PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA1.str_ptr->str_ptr) > 0) && (strlen(PARA1.str_ptr->str_ptr) < MQTT_TOPIC_LEN_MAX - 1 ))
                && (PARAM2_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC ) && (PARA2.num == MQTT_SUB_FLAG) && (PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC ))
            {
                pcTopic = PARA1.str_ptr->str_ptr;
                iQos = PARA3.num;

                g_stMqttOpts.m_pcTopic = pcTopic;
                g_stMqttOpts.m_iQos = iQos;
                MqttSubscribe(g_stMqttClient);

                sprintf((char *)g_rsp_str, "\r\n+MQTTSUBUNSUB: MQTT Subscribe Success\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                SCI_DEBUG("+MQTTSUBUNSUB: Error Param");
                sprintf((char *)g_rsp_str, "\r\n+MQTTSUBUNSUB: Error Param\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            break;
        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            SCI_DEBUG( "AT_CmdFunc_MQTTSUBUNSUB, command type:%d not allowed",  ATC_GET_CMD_TYPE);
            sprintf((char *)g_rsp_str, "\r\n+MQTTSUBUNSUB: Error Command Type\r\n");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    }
#endif
    return status;

}

/*****************************************************************************/
// Description: This function handles the AT+MQTTDISCONN command
//                      used for disconnect the MQTT connection
// Example:     AT+MQTTDISCONN
// Author:      hongqun.cheng
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMQTTDISCONN)
{
    int32  status = S_ATC_SUCCESS;
#if defined MQTT_SUPPORT

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
             if(0== mqtt_netid)
            {
                sprintf((char *)g_rsp_str, "+MQTTDISCONN: Net is error.");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                //return S_ATC_FAIL;
            }

            if (CONNECTING == g_stMqttOpts.m_eState || DISCONNECTING == g_stMqttOpts.m_eState || DISCONNECTED == g_stMqttOpts.m_eState)
            {
                SCI_TRACE_LOW("  [ MQTT ] Mqtt connection state is error.  [%s %d]  \n",__FUNCTION__,__LINE__);
                sprintf((char *)g_rsp_str, "\r\n +MQTTDISCONN: Network Connection State Error\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_FAIL;
            }

            g_stMqttOpts.m_eState = DISCONNECTING;
            MqttDisconnect(g_stMqttClient);
            //sprintf((char *)g_rsp_str, "\r\n+MQTTDISCONN: MQTT Disconnect  Success\r\n");
            //ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            SCI_DEBUG( "AT_CmdFunc_MQTTDISCONN, command type:%d not allowed",  ATC_GET_CMD_TYPE);
            sprintf((char *)g_rsp_str, "\r\n+MQTTDISCONN: Error Command Type\r\n");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    }
#endif
    return status;

}

#ifdef COAP_SUPPORT

extern coap_netid;
extern g_coap_client;
#endif
/*****************************************************************************/
// Description: This function handles the COAP_GET command
//                      used for create coap client and handle GET request
// Example:     AT+COAP_GET=<url>,<cmdline>, [timer]
//		url--hostname  of coap server and resource patch
//             cmdline--paratemer string
//             timer--the timeout of handling request
// Author:      xiameng.yang
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCOAP_GET)
{
    int32  status = S_ATC_SUCCESS;
#if defined COAP_SUPPORT
    int rc = 0;
    uint8_t def_seconds = DATA_TIMEOUT_DEF;

    const char *url ;
    const char *cmdline;

    uint8_t wait_seconds;
    uint8_t timeout_param[10] = {0};
    coap_client_t *client;
    bool paramok = true;

    //netid
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T * ret_state_ptr = 0;
    uint32 pdp_context_num = 0;
    uint8 nsapi = 5;
    int8 i=0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(cur_cmd_info->cur_param_num >=2)
            {
                if((PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA1.str_ptr->str_ptr) > 0) && (strlen(PARA1.str_ptr->str_ptr) < COAP_HOSTNAME_LEN_MAX - 1 ))
                && (PARAM2_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA2.str_ptr->str_ptr) > 0) && (strlen(PARA2.str_ptr->str_ptr) < COAP_CMDLINE_LEN_MAX - 1 ))
                )
                {

                    //get netid
                    if(ERR_MNGPRS_NO_ERR == MNGPRS_ReadPdpContextStateEx(dual_sys,  &ret_state_ptr, &pdp_context_num))
                    {
                        if(ret_state_ptr != 0)
                        {
                            for(i=0;i<pdp_context_num;i++)
                            {
                                if (ret_state_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                                {
                                    nsapi = MNGPRS_GetNsapiByCidEx(dual_sys,ret_state_ptr[i].pdp_id);
                                    SCI_TRACE_LOW(" COAP  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                                    break;
                                }
                            }
                        }
                    }

                    if (nsapi != 0)
                    {
                        SCI_TRACE_LOW(" COAP  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                        coap_netid = nsapi;
                    }

                    socket_SetNetId(coap_netid);

                    if(0 == coap_netid)
                    {
                        sprintf((char *)g_rsp_str, "+COAP_GET: Please activate net interface first");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        //return S_ATC_FAIL;
                    }

                    url = PARA1.str_ptr->str_ptr;

                    if (strlen(url) > AT_URL_LENG_MAX)
                    {
                        SCI_TRACE_LOW("the LENGTH of URL must be less than 60!\n");
                        sprintf(g_rsp_str, "%s\r\n", "the LENGTH of URL must be less than 60!\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }

                    cmdline = PARA2.str_ptr->str_ptr;

                    SCI_TRACE_LOW(" COAP  cmdline  : %s    [  %s  %d]\n",cmdline,__FUNCTION__,__LINE__);

                    def_seconds = DATA_TIMEOUT_DEF;

                    if(cur_cmd_info->cur_param_num ==3)
                    {
                        if(PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC )
                        {
                            wait_seconds = PARA3.num;
                            SCI_TRACE_LOW("the time is : %d\n",wait_seconds);
                            if (wait_seconds != 0)
                            {
                                def_seconds = wait_seconds;
                                sprintf((char *)timeout_param, "-B %d", def_seconds);
                            }
                        }
                    }
                    client =coap_getCoapClient();

                    if (client != NULL)
                    {
                        coap_client_setParams(client, timeout_param);
                        coap_client_setParams(client, (uint8_t *)cmdline);
                        SCI_TRACE_LOW(" COAP  client ->cmdline  : %s    [  %s  %d]\n",client->cmdline,__FUNCTION__,__LINE__);
                        coap_client_setIndataCbk(client, (coap_incoming_data_cb_t)coapProcessReturnData);
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);

                        paramok = coap_async_get(client, (uint8_t *)url);
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);
                    }
                     else
                    {
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);
                        paramok = false;
                    }

                     if (paramok)
                    {
                        status = S_ATC_SUCCESS;
                        SCI_TRACE_LOW("+coap_get: parameter ok");
                        sprintf((char *)g_rsp_str, "\r\n+coap_get: parameter ok\r\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        break;
                    }
                    else
                    {
                        status = S_ATC_FAIL;
                        SCI_TRACE_LOW("+coap_get: COAP TASK IS RUNNING ,WAIT ! error");
                        sprintf((char *)g_rsp_str, "\r\n+coap_get:  COAP TASK IS RUNNING ,WAIT  \r\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        break;
                    }

                }
                else
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    SCI_TRACE_LOW("+coap: parameter error");
                    sprintf((char *)g_rsp_str, "\r\n+coap: Parameter Error\r\n");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                SCI_TRACE_LOW("+COAP_GET: the number of parameters error");
                sprintf((char *)g_rsp_str, "\r\n+COAP_GET: The Number of Parameters Error\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            break;
        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            SCI_DEBUG( "AT_CmdFunc_COAP_GET, command type:%d not allowed",  ATC_GET_CMD_TYPE);
            sprintf((char *)g_rsp_str, "\r\n+COAP_GET: Error Command Type\r\n");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    }
#endif
    return status;
}

/*****************************************************************************/
// Description: This function handles the COAP_POST command
//                      used for create coap client and handle POST request
// Example:     AT+COAP_POST=<url>,<cmdline>,<data> ,[timer]
//		url--hostname  of coap server and resource patch
//             cmdline--paratemer string
//             data -- user data to post
//             timer--the timeout of handling request
// Author:      xiameng.yang
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCOAP_POST)
{
    int32  status = S_ATC_SUCCESS;
#if defined COAP_SUPPORT
    int rc = 0;
    uint8_t def_seconds = DATA_TIMEOUT_DEF;

    const char *url ;
    const char *cmdline;
    const char *data = NULL;

    uint8_t wait_seconds;
    uint8_t timeout_param[10] = {0};
    coap_client_t *client;
    bool paramok = true;

    //netid
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T * ret_state_ptr = 0;
    uint32 pdp_context_num = 0;
    uint8 nsapi = 5;
    int8 i=0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(cur_cmd_info->cur_param_num >=2)
            {
                if((PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA1.str_ptr->str_ptr) > 0) && (strlen(PARA1.str_ptr->str_ptr) < COAP_HOSTNAME_LEN_MAX - 1 ))
                && (PARAM2_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA2.str_ptr->str_ptr) > 0) && (strlen(PARA2.str_ptr->str_ptr) < COAP_CMDLINE_LEN_MAX - 1 ))
                )
                {

                    //get netid
                    if(ERR_MNGPRS_NO_ERR == MNGPRS_ReadPdpContextStateEx(dual_sys,  &ret_state_ptr, &pdp_context_num))
                    {
                        if(ret_state_ptr != 0)
                        {
                            for(i=0;i<pdp_context_num;i++)
                            {
                                if (ret_state_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                                {
                                    nsapi = MNGPRS_GetNsapiByCidEx(dual_sys,ret_state_ptr[i].pdp_id);
                                    SCI_TRACE_LOW(" COAP  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                                    break;
                                }
                            }
                        }
                    }

                    if (nsapi != 0)
                    {
                        SCI_TRACE_LOW(" COAP  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                        coap_netid = nsapi;
                    }

                    socket_SetNetId(coap_netid);

                    if(0 == coap_netid)
                    {
                        sprintf((char *)g_rsp_str, "+COAP_POST: Please activate net interface first");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        //return S_ATC_FAIL;
                    }

                    url = PARA1.str_ptr->str_ptr;

                    if (strlen(url) > AT_URL_LENG_MAX)
                    {
                        SCI_TRACE_LOW("the LENGTH of URL must be less than 60!\n");
                        sprintf(g_rsp_str, "%s\r\n", "the LENGTH of URL must be less than 60!\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }

                    cmdline = PARA2.str_ptr->str_ptr;

                    SCI_TRACE_LOW(" COAP  cmdline  : %s    [  %s  %d]\n",cmdline,__FUNCTION__,__LINE__);

                    def_seconds = DATA_TIMEOUT_DEF;

                    if(cur_cmd_info->cur_param_num >=3)
                    {
                        if(PARAM3_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                        {
                            data = PARA3.str_ptr->str_ptr;
                            SCI_TRACE_LOW(" COAP  data  : %s    [  %s  %d]\n",data,__FUNCTION__,__LINE__);

                            if(cur_cmd_info->cur_param_num ==4)
                            {
                                if(PARAM4_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC )
                                {
                                    wait_seconds = PARA4.num;
                                    SCI_TRACE_LOW("the time is : %d\n",wait_seconds);
                                    if (wait_seconds != 0)
                                    {
                                        def_seconds = wait_seconds;
                                        sprintf((char *)timeout_param, "-B %d", def_seconds);
                                    }
                                }
                            }
                        }
                    }
                    client =coap_getCoapClient();

                    if (client != NULL)
                    {
                        coap_client_setParams(client, timeout_param);
                        coap_client_setParams(client, (uint8_t *)cmdline);
                        SCI_TRACE_LOW(" COAP  client ->cmdline  : %s    [  %s  %d]\n",client->cmdline,__FUNCTION__,__LINE__);
                        coap_client_setIndataCbk(client, (coap_incoming_data_cb_t)coapProcessReturnData);
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);

                        paramok = coap_async_post(client, (uint8_t *)url,(uint8_t *)data);
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);
                    }
                     else
                    {
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);
                        paramok = false;
                    }

                     if (paramok)
                    {
                        status = S_ATC_SUCCESS;
                        SCI_TRACE_LOW("+coap_post: parameter ok");
                        sprintf((char *)g_rsp_str, "\r\n+coap_post: parameter ok\r\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        break;
                    }
                    else
                    {
                        status = S_ATC_FAIL;
                        SCI_TRACE_LOW("+coap_post: COAP TASK IS RUNNING ,WAIT ! error");
                        sprintf((char *)g_rsp_str, "\r\n+coap_post:  COAP TASK IS RUNNING ,WAIT  \r\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        break;
                    }

                }
                else
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    SCI_TRACE_LOW("+coap: parameter error");
                    sprintf((char *)g_rsp_str, "\r\n+coap: Parameter Error\r\n");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                SCI_TRACE_LOW("+coap_post: the number of parameters error");
                sprintf((char *)g_rsp_str, "\r\n+coap_post: The Number of Parameters Error\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            break;
        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            SCI_DEBUG( "coap_post, command type:%d not allowed",  ATC_GET_CMD_TYPE);
            sprintf((char *)g_rsp_str, "\r\n+coap_post: Error Command Type\r\n");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    }
#endif
    return status;
}

/*****************************************************************************/
// Description: This function handles the COAP_PUT command
//                      used for create coap client and handle PUT request
// Example:     AT+COAP_PUT=<url>,<cmdline>,<data> ,[timer]
//		url--hostname  of coap server and resource patch
//             cmdline--paratemer string
//             data -- user data to put
//             timer--the timeout of handling request
// Author:      xiameng.yang
/*****************************************************************************/

AT_CMD_FUNC(ATC_ProcessCOAP_PUT)
{
    int32  status = S_ATC_SUCCESS;
#if defined COAP_SUPPORT
    int rc = 0;
    uint8_t def_seconds = DATA_TIMEOUT_DEF;

    const char *url ;
    const char *cmdline;
    const char *data = NULL;

    uint8_t wait_seconds;
    uint8_t timeout_param[10] = {0};
    coap_client_t *client;
    bool paramok = true;

    //netid
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T * ret_state_ptr = 0;
    uint32 pdp_context_num = 0;
    uint8 nsapi = 5;
    int8 i=0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(cur_cmd_info->cur_param_num >=2)
            {
                if((PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA1.str_ptr->str_ptr) > 0) && (strlen(PARA1.str_ptr->str_ptr) < COAP_HOSTNAME_LEN_MAX - 1 ))
                && (PARAM2_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA2.str_ptr->str_ptr) > 0) && (strlen(PARA2.str_ptr->str_ptr) < COAP_CMDLINE_LEN_MAX - 1 ))
                )
                {

                    //get netid
                    if(ERR_MNGPRS_NO_ERR == MNGPRS_ReadPdpContextStateEx(dual_sys,  &ret_state_ptr, &pdp_context_num))
                    {
                        if(ret_state_ptr != 0)
                        {
                            for(i=0;i<pdp_context_num;i++)
                            {
                                if (ret_state_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                                {
                                    nsapi = MNGPRS_GetNsapiByCidEx(dual_sys,ret_state_ptr[i].pdp_id);
                                    SCI_TRACE_LOW(" COAP  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                                    break;
                                }
                            }
                        }
                    }

                    if (nsapi != 0)
                    {
                        SCI_TRACE_LOW(" COAP  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                        coap_netid = nsapi;
                    }

                    socket_SetNetId(coap_netid);

                    if(0 == coap_netid)
                    {
                        sprintf((char *)g_rsp_str, "+COAP_PUT: Please activate net interface first");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        //return S_ATC_FAIL;
                    }

                    url = PARA1.str_ptr->str_ptr;

                    if (strlen(url) > AT_URL_LENG_MAX)
                    {
                        SCI_TRACE_LOW("the LENGTH of URL must be less than 60!\n");
                        sprintf(g_rsp_str, "%s\r\n", "the LENGTH of URL must be less than 60!\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }

                    cmdline = PARA2.str_ptr->str_ptr;

                    SCI_TRACE_LOW(" COAP  cmdline  : %s    [  %s  %d]\n",cmdline,__FUNCTION__,__LINE__);

                    def_seconds = DATA_TIMEOUT_DEF;

                    if(cur_cmd_info->cur_param_num >=3)
                    {
                        if(PARAM3_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                        {
                            data = PARA3.str_ptr->str_ptr;
                            SCI_TRACE_LOW(" COAP  data  : %s    [  %s  %d]\n",data,__FUNCTION__,__LINE__);

                            if(cur_cmd_info->cur_param_num ==4)
                            {
                                if(PARAM4_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC )
                                {
                                    wait_seconds = PARA4.num;
                                    SCI_TRACE_LOW("the time is : %d\n",wait_seconds);
                                    if (wait_seconds != 0)
                                    {
                                        def_seconds = wait_seconds;
                                        sprintf((char *)timeout_param, "-B %d", def_seconds);
                                    }
                                }
                            }
                        }
                    }
                    client =coap_getCoapClient();

                    if (client != NULL)
                    {
                        coap_client_setParams(client, timeout_param);
                        coap_client_setParams(client, (uint8_t *)cmdline);
                        SCI_TRACE_LOW(" COAP  client ->cmdline  : %s    [  %s  %d]\n",client->cmdline,__FUNCTION__,__LINE__);
                        coap_client_setIndataCbk(client, (coap_incoming_data_cb_t)coapProcessReturnData);
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);

                        paramok = coap_async_put(client, (uint8_t *)url,(uint8_t *)data);
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);
                    }
                     else
                    {
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);
                        paramok = false;
                    }

                     if (paramok)
                    {
                        status = S_ATC_SUCCESS;
                        SCI_TRACE_LOW("+COAP_PUT: parameter ok");
                        sprintf((char *)g_rsp_str, "\r\n+COAP_PUT: parameter ok\r\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        break;
                    }
                    else
                    {
                        status = S_ATC_FAIL;
                        SCI_TRACE_LOW("+COAP_PUT: COAP TASK IS RUNNING ,WAIT ! error");
                        sprintf((char *)g_rsp_str, "\r\n+COAP_PUT:  COAP TASK IS RUNNING ,WAIT  \r\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        break;
                    }

                }
                else
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    SCI_TRACE_LOW("+coap: parameter error");
                    sprintf((char *)g_rsp_str, "\r\n+coap: Parameter Error\r\n");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                SCI_TRACE_LOW("+COAP_PUT: the number of parameters error");
                sprintf((char *)g_rsp_str, "\r\n+COAP_PUT: The Number of Parameters Error\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            break;
        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            SCI_DEBUG( "COAP_PUT, command type:%d not allowed",  ATC_GET_CMD_TYPE);
            sprintf((char *)g_rsp_str, "\r\n+COAP_PUT: Error Command Type\r\n");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    }
#endif
    return status;
}



/*****************************************************************************/
// Description: This function handles the COAP_DELETE command
//                      used for create coap client and handle DELETE request
// Example:     AT+COAP_DELETE=<url>,<cmdline>, [timer]
//		url--hostname  of coap server and resource patch
//             cmdline--paratemer string
//             timer--the timeout of handling request
// Author:      xiameng.yang
/*****************************************************************************/

AT_CMD_FUNC(ATC_ProcessCOAP_DELETE)
{
    int32  status = S_ATC_SUCCESS;
#if defined COAP_SUPPORT
    int rc = 0;
    uint8_t def_seconds = DATA_TIMEOUT_DEF;

    const char *url ;
    const char *cmdline;

    uint8_t wait_seconds;
    uint8_t timeout_param[10] = {0};
    coap_client_t *client;
    bool paramok = true;

    //netid
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T * ret_state_ptr = 0;
    uint32 pdp_context_num = 0;
    uint8 nsapi = 5;
    int8 i=0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(cur_cmd_info->cur_param_num >=2)
            {
                if((PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA1.str_ptr->str_ptr) > 0) && (strlen(PARA1.str_ptr->str_ptr) < COAP_HOSTNAME_LEN_MAX - 1 ))
                && (PARAM2_TYPE == ATC_CMD_PARAM_TYPE_STRING )
                && ((strlen(PARA2.str_ptr->str_ptr) > 0) && (strlen(PARA2.str_ptr->str_ptr) < COAP_CMDLINE_LEN_MAX - 1 ))
                )
                {

                    //get netid
                    if(ERR_MNGPRS_NO_ERR == MNGPRS_ReadPdpContextStateEx(dual_sys,  &ret_state_ptr, &pdp_context_num))
                    {
                        if(ret_state_ptr != 0)
                        {
                            for(i=0;i<pdp_context_num;i++)
                            {
                                if (ret_state_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                                {
                                    nsapi = MNGPRS_GetNsapiByCidEx(dual_sys,ret_state_ptr[i].pdp_id);
                                    SCI_TRACE_LOW(" COAP  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                                    break;
                                }
                            }
                        }
                    }

                    if (nsapi != 0)
                    {
                        SCI_TRACE_LOW(" COAP  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                        coap_netid = nsapi;
                    }

                    socket_SetNetId(coap_netid);

                    if(0 == coap_netid)
                    {
                        sprintf((char *)g_rsp_str, "+COAP_DELETE: Please activate net interface first");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        //return S_ATC_FAIL;
                    }

                    url = PARA1.str_ptr->str_ptr;

                    if (strlen(url) > AT_URL_LENG_MAX)
                    {
                        SCI_TRACE_LOW("the LENGTH of URL must be less than 60!\n");
                        sprintf(g_rsp_str, "%s\r\n", "the LENGTH of URL must be less than 60!\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        return S_ATC_FAIL;
                    }

                    cmdline = PARA2.str_ptr->str_ptr;

                    SCI_TRACE_LOW(" COAP  cmdline  : %s    [  %s  %d]\n",cmdline,__FUNCTION__,__LINE__);

                    def_seconds = DATA_TIMEOUT_DEF;

                    if(cur_cmd_info->cur_param_num ==3)
                    {
                        if(PARAM3_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC )
                        {
                            wait_seconds = PARA3.num;
                            SCI_TRACE_LOW("the time is : %d\n",wait_seconds);
                            if (wait_seconds != 0)
                            {
                                def_seconds = wait_seconds;
                                sprintf((char *)timeout_param, "-B %d", def_seconds);
                            }
                        }
                    }
                    client =coap_getCoapClient();

                    if (client != NULL)
                    {
                        coap_client_setParams(client, timeout_param);
                        coap_client_setParams(client, (uint8_t *)cmdline);
                        SCI_TRACE_LOW(" COAP  client ->cmdline  : %s    [  %s  %d]\n",client->cmdline,__FUNCTION__,__LINE__);
                        coap_client_setIndataCbk(client, (coap_incoming_data_cb_t)coapProcessReturnData);
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);

                        paramok = coap_async_delete(client, (uint8_t *)url);
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);
                    }
                     else
                    {
                        SCI_TRACE_LOW(" COAPDEBUG    [  %s  %d]\n",__FUNCTION__,__LINE__);
                        paramok = false;
                    }

                     if (paramok)
                    {
                        status = S_ATC_SUCCESS;
                        SCI_TRACE_LOW("+COAP_DELETE: parameter ok");
                        sprintf((char *)g_rsp_str, "\r\n+COAP_DELETE: parameter ok\r\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        break;
                    }
                    else
                    {
                        status = S_ATC_FAIL;
                        SCI_TRACE_LOW("+COAP_DELETE: COAP TASK IS RUNNING ,WAIT ! error");
                        sprintf((char *)g_rsp_str, "\r\n+COAP_DELETE:  COAP TASK IS RUNNING ,WAIT  \r\n");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        break;
                    }

                }
                else
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    SCI_TRACE_LOW("+coap: parameter error");
                    sprintf((char *)g_rsp_str, "\r\n+coap: Parameter Error\r\n");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                SCI_TRACE_LOW("+COAP_DELETE: the number of parameters error");
                sprintf((char *)g_rsp_str, "\r\n+COAP_DELETE: The Number of Parameters Error\r\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            }
            break;
        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            SCI_DEBUG( "COAP_DELETE, command type:%d not allowed",  ATC_GET_CMD_TYPE);
            sprintf((char *)g_rsp_str, "\r\n+COAP_DELETE: Error Command Type\r\n");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    }
#endif
    return status;
}

static BOOLEAN atc_armlog_enable = TRUE;

/*****************************************************************************/
//  Description : This function handles the AT+ARMLOG command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessARMLOG)
{
    uint32   status = S_ATC_SUCCESS;

#ifndef WIN32

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                atc_armlog_enable = PARA1.num;
                SCI_SetArmLogFlag(PARA1.num);
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:

            if(REFPARAM_GetEnableArmLogFlag() != atc_armlog_enable)
            {
                atc_armlog_enable = REFPARAM_GetEnableArmLogFlag();
            }

            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_ARMLOG].cmd_name->str_ptr, atc_armlog_enable);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_ALLOWED;
    }

#endif
    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SDRMOD command
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSDRMOD)
{
    ATC_STATUS  status = S_ATC_SUCCESS;
    uint8       debug_mode = 0;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if(PARAM1_FLAG)
        {
            if(0 == PARA1.num)
            {
                OSPARAM_SetAssertMode(SCI_ASSERT_DEBUG_MODE);
            }
            else
            {
                OSPARAM_SetAssertMode(SCI_ASSERT_RELEASE_MODE);
            }
        }
        else
        {
            status = ERR_OPERATION_NOT_SUPPORTED;
        }
        break;
    case ATC_CMD_TYPE_READ:
        OSPARAM_GetAssertMode((uint8*)&debug_mode);

        sprintf((char*)g_rsp_str, "%s: %d",
                g_atc_info_table[AT_CMD_SDRMOD].cmd_name->str_ptr,
                (debug_mode == SCI_ASSERT_RELEASE_MODE) ? 1 : 0);

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;
    default:
        break;
    }
    return status;
}

#ifndef DISABLE_RPC_CFW_EmodGetLteCalibInfo
extern uint32_t CFW_EmodGetLteCalibInfo(void);
#endif


static uint8_t GetBitCalibInfo(uint32_t param32, int nBit)
{
    if (nBit < 0 || nBit > 31)
        return -1;
    else
    {
        if (param32 & (1 << nBit))
            return 1;
        else
            return 0;
    }
}

AT_CMD_FUNC(ATC_ProcessCALIBINFO)
{

    ATC_STATUS  status = S_ATC_SUCCESS;
    uint32_t gsmCal, lteCal;
    char RspGsm[200] = {0}, RspLte[200] = {0};
    uint8_t gsmBit[16], lteBit[16];
    int nBit=0;


    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_READ:

        lteCal = CFW_EmodGetLteCalibInfo();

        for (nBit = 0; nBit < 16; nBit++)
        {

            lteBit[nBit] = GetBitCalibInfo(lteCal, nBit);
        }

        sprintf(g_rsp_str, "LTE_CALIBINFO: LTE_afc:%d, LTE_TDD_agc:%d, LTE_TDD_apc:%d, LTE_FDD_agc:%d, LTE_FDD_apc:%d, FINAL_LTE:%d, ANT_LTE:%d",
                lteBit[0], lteBit[1], lteBit[2], lteBit[4], lteBit[5], lteBit[8], lteBit[9]);

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;
    default:
        return ERR_OPERATION_NOT_ALLOWED;
    }
    return status;

}

#if defined LIBLWM2M_SUPPORT
extern uint32  lwm2m_netid;
#endif
//just for test !!!
AT_CMD_FUNC(ATC_ProcessTESTLWM2M)
{
    #if defined(_ATC_UIX8910_ENABLE_)
    #if defined LIBLWM2M_SUPPORT
    ATC_STATUS status = S_ATC_SUCCESS;
    //netid
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_GPRS_CON_STATE_T * ret_state_ptr = 0;
    uint32 pdp_context_num = 0;
    uint8 nsapi = 5;
    int8 i=0;

   int num = 0;
   int num2 = 0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            //check whether the number of input parameter is 5
            if(PARAM1_FLAG == 0)
            {
                SCI_TRACE_LOW("the number of arguments must be 1!\n");
                sprintf(g_rsp_str, "%s\r\n", "^SPDHCPCFG:The number of arguments must be 1!\n");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_FAIL;
            }
            if ((PARA1.num< 0) || (PARA1.num > 65535) || (PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC )) //7->0.0.0.0
            {
                SCI_TRACE_LOW("PAREMETER ERROR!\n");
                sprintf(g_rsp_str, "%s\r\n", "^PAREMETER ERROR");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                return S_ATC_FAIL;
            }
            num = PARA1.num;


            if((PARAM1_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC )&&PARA2.num >= 1 &&PARA2.num <= 65535  )
            {
                num2 = PARA2.num;
            }
            SCI_TRACE_LOW(" testlwm2m num2 :%d [%s  %d]\n",num2,__FUNCTION__,__LINE__);

            switch (num)
            {
                case 1:
                    SCI_TRACE_LOW("testlwm2m test : %d , [%s  %d]\n",num,__FUNCTION__,__LINE__);
                    if(LWM2M_TEST_create(num2) !=-1)
                    {
                        sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m   success test : ",num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_SUCCESS;
                        break;
                    }
                    else
                    {
                        sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    }

                case 2:
                    SCI_TRACE_LOW("testlwm2m test : %d , [%s  %d]\n",num,__FUNCTION__,__LINE__);
                    if( LWM2M_TEST_register_bootstrap() !=-1)
                    {
                        sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_SUCCESS;
                        break;
                    }
                     else
                    {
                        sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    }

                case 3:
                    if(LWM2M_TEST_close(0) != -1)
                    {
                        sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_SUCCESS;
                        break;
                    }
                     else
                    {
                        sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    }

                case 4:
                    SCI_TRACE_LOW("testlwm2m test : %d , [%s  %d]\n",num,__FUNCTION__,__LINE__);
                    if(LWM2M_TEST_add_object(0,num2) != -1)
                    {
                        sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_SUCCESS;
                        break;
                    }
                     else
                    {
                        sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    }
                case 5:
                    SCI_TRACE_LOW("testlwm2m test : %d , [%s  %d]\n",num,__FUNCTION__,__LINE__);

                    if(LWM2M_TEST_del_object(0,num2)!= -1)
                    {
                        sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_SUCCESS;
                        break;
                    }
                     else
                    {
                        sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        status = S_ATC_FAIL;
                        break;
                    }
                     case 6:
                        SCI_TRACE_LOW("testlwm2m test : %d , [%s  %d]\n",num,__FUNCTION__,__LINE__);
                        if(LWM2M_TEST_observersp(0) != -1)
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_SUCCESS;
                            break;
                        }
                         else
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_FAIL;
                            break;
                        }
                     case 7:
                            if(LWM2M_TEST_discoverrsp(0)!=-1)
                            {
                                sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                status = S_ATC_SUCCESS;
                                break;
                            }
                             else
                            {
                                sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                status = S_ATC_FAIL;
                                break;
                            }
                       case 8:
                        if(LWM2M_TEST_readrsp(0)!=-1)
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_SUCCESS;
                            break;
                        }
                         else
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_FAIL;
                            break;
                        }
                         case 9:
                        if(LWM2M_TEST_writersp(0)!=-1)
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_SUCCESS;
                            break;
                        }
                         else
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_FAIL;
                            break;
                        }

                         case 10:
                        if(LWM2M_TEST_notify(0)!=-1)
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_SUCCESS;
                            break;
                        }
                         else
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_FAIL;
                            break;
                        }
                         case 11:
                        if(LWM2M_TEST_NOTIFY_POSITIVE(0)!=-1)
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_SUCCESS;
                            break;
                        }
                         else
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_FAIL;
                            break;
                        }

                        case 12:
                        if(LWM2M_TEST_executersp(0)!=-1)
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_SUCCESS;
                            break;
                        }
                         else
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_FAIL;
                            break;
                        }
                    case 21:
                        if(LWM2M_TEST_CTM2MINIT(num2)!=-1)
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_SUCCESS;
                            break;
                        }
                         else
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_FAIL;
                            break;
                        }
                         case 22:
                        if(LWM2M_TEST_CTM2MREG()!=-1)
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_SUCCESS;
                            break;
                        }
                         else
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_FAIL;
                            break;
                        }
                        case 23:
                        if(LWM2M_TEST_CTM2MDEREG()!=-1)
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_SUCCESS;
                            break;
                        }
                         else
                        {
                            sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m  failed test : ",num);
                            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            status = S_ATC_FAIL;
                            break;
                        }
                    case 100 :
                        //get netid
                    if(ERR_MNGPRS_NO_ERR == MNGPRS_ReadPdpContextStateEx(dual_sys,  &ret_state_ptr, &pdp_context_num))
                    {
                        if(ret_state_ptr != 0)
                        {
                            for(i=0;i<pdp_context_num;i++)
                            {
                                if (ret_state_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
                                {
                                    nsapi = MNGPRS_GetNsapiByCidEx(dual_sys,ret_state_ptr[i].pdp_id);
                                    SCI_TRACE_LOW(" LWM2MDEBUG  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                                    break;
                                }
                            }
                        }
                    }

                    if (nsapi != 0)
                    {
                        SCI_TRACE_LOW(" LWM2MDEBUG  nsapi :%d   [  %s  %d]  \r\n",nsapi,__FUNCTION__,__LINE__);
                        lwm2m_netid = nsapi;
                    }

                    socket_SetNetId(lwm2m_netid);

                    if(0 == lwm2m_netid)
                    {
                        sprintf((char *)g_rsp_str, "+lwm2m: Please activate net interface first");
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        //return S_ATC_FAIL;
                    }

                    SCI_TRACE_LOW("testlwm2m pdp ok , [%s  %d]\n",num,__FUNCTION__,__LINE__);
                    sprintf(g_rsp_str, "%s  %d\r\n", "^testlwm2m test : ",num);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    status = S_ATC_SUCCESS;
                    break;
                default:
                    sprintf((char *)g_rsp_str, "   num range Error");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;

            }

        break;
        case ATC_CMD_TYPE_READ:

        break;
        default:
            sprintf((char *)g_rsp_str, "Error");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;
    }
    return status;
#else
    return status;
#endif
#endif
}



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
