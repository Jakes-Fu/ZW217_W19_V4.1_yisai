/******************************************************************************
 ** File Name:      atc.h                                                     *
 ** Author:         Xueli.lv                                                  *
 ** DATE:           07/03/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains types and MICROS that are of use to all*
 **                 other ATC modules.                                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/03/2002     Ivan.Yin         Created.                                  *
 ******************************************************************************/

#ifndef _ATC_H_
#define _ATC_H_
/*******************************************************************************
 **                        Common Header Files                                 *
 ******************************************************************************/
#include "sci_types.h"
#ifdef _SUPPORT_GPRS_
#include "atc_def.h"
#else
#include "atc_gsmonly_def.h"
#endif
#include <stdio.h>
#include "nvitem.h"
#ifdef _RTOS
#include "sci_api.h" /*@tony.yao  replase sci_mem.h with sci_api.h*/
#else
#include "tasks_id.h"
#endif
#include "mn_api.h"
#include "mn_events.h"
#include "mn_type.h"
#include "nv_item_id.h"
#include "sio.h"
#ifdef _MUX_ENABLE_
#include "mux_api.h"
#include "sig_code.h"
#endif

/*******************************************************************************
 **                        AT NV item id define                               *
 ******************************************************************************/
#define NV_ATC_CB_MSG_ID    MN_ATNV_USER_BASE
#define NV_ATC_CB_LANG_ID   (MN_ATNV_USER_BASE+1)
#define NV_ATC_MIDI_SIZE_ID (MN_ATNV_USER_BASE+2)
#define NV_ATC_CONFIG_ID    (MN_ATNV_USER_BASE+3)
#define NV_SMS_CONFIG_ID    (MN_ATNV_USER_BASE+4)

/*******************************************************************************
 **                        AT commands class type                              *
 ******************************************************************************/
#define ATC_CMD_TYPE_BASIC      (0x10)
#define ATC_CMD_TYPE_EXTEND     (0x20)

#define PARA1     cur_cmd_info->cur_param_list_ptr->param_info
#define PARA2     cur_cmd_info->cur_param_list_ptr->next->param_info
#define PARA3     cur_cmd_info->cur_param_list_ptr->next->next->param_info
#define PARA4     cur_cmd_info->cur_param_list_ptr->next->next->next->param_info
#define PARA5     cur_cmd_info->cur_param_list_ptr->next->next->next->next->param_info
#define PARA6     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->param_info
#define PARA7     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->param_info
#define PARA8     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->param_info
#define PARA9     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->param_info
#define PARA10     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->param_info
#define PARA11     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->param_info
#define PARA12     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->next->param_info
#define PARA13     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->next->next->param_info

#define PARAM1_TYPE cur_cmd_info->cur_param_list_ptr->param_type
#define PARAM2_TYPE cur_cmd_info->cur_param_list_ptr->next->param_type
#define PARAM3_TYPE cur_cmd_info->cur_param_list_ptr->next->next->param_type
#define PARAM4_TYPE cur_cmd_info->cur_param_list_ptr->next->next->next->param_type
#define PARAM5_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->next->next->param_type
#define PARAM6_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->param_type
#define PARAM7_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->param_type
#define PARAM8_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->param_type
#define PARAM9_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->param_type
#define PARAM10_TYPE cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->param_type
#define PARAM11_TYPE cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->param_type
#define PARAM12_TYPE cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->next->param_type
#define PARAM13_TYPE cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->next->next->param_type
#define PARAM14_TYPE cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->next->next->next->param_type

#ifdef AUDIO_PARAM_ARM
#define MAX_ATC_RSP_LEN         4096
#else
#define MAX_ATC_RSP_LEN         1024
#endif

#define param_str_ptr_1     cur_cmd_info->cur_param_list_ptr->param_info.str_ptr->str_ptr
#define param_str_len_1     cur_cmd_info->cur_param_list_ptr->param_info.str_ptr->str_len
#define param_num_1         cur_cmd_info->cur_param_list_ptr->param_info.num

#define PARAM1_FLAG cur_cmd_info->param_used[0]
#define PARAM2_FLAG cur_cmd_info->param_used[1]
#define PARAM3_FLAG cur_cmd_info->param_used[2]
#define PARAM4_FLAG cur_cmd_info->param_used[3]
#define PARAM5_FLAG cur_cmd_info->param_used[4]
#define PARAM6_FLAG cur_cmd_info->param_used[5]
#define PARAM7_FLAG cur_cmd_info->param_used[6]
#define PARAM8_FLAG cur_cmd_info->param_used[7]
#define PARAM9_FLAG cur_cmd_info->param_used[8]
#define PARAM10_FLAG cur_cmd_info->param_used[9]
#define PARAM11_FLAG cur_cmd_info->param_used[10]
#define PARAM12_FLAG cur_cmd_info->param_used[11]
#define PARAM13_FLAG cur_cmd_info->param_used[12]

/*******************************************************************************
 **                        AT commands processing indication                   *
 ******************************************************************************/
#define ATC_CMD_TYPE_PENDING    (0x40)  // Cmd was submitted to other unit 
                                        // and was not completed yet.
#define ATC_CMD_TYPE_ERROR      (0x80)  // @Steven

/*******************************************************************************
 **                        AT commands action type                             *
 ******************************************************************************/
#define ATC_CMD_TYPE_EXECUTE    (1<<0)
#define ATC_CMD_TYPE_SET        (1<<1)
#define ATC_CMD_TYPE_READ       (1<<2)
#define ATC_CMD_TYPE_TEST       (1<<3)

#define ATC_GET_CMD_TYPE      (cur_cmd_info->cmd_type & 0X0F)

/*******************************************************************************
 **                        Common NUM  define                                  *
 ******************************************************************************/
#define ATC_MAX_PARAM_NUM       20
#define ATC_MAX_MIDI_INDEX      50

/*******************************************************************************
 **                        UCS2 Flag                                           *
 ******************************************************************************/
#define ATC_UCS2_80_CODE        0x80
#define ATC_UCS2_81_CODE        0x81
#define ATC_UCS2_82_CODE        0x82

/*******************************************************************************
 **                        AT commands parameter type                          *
 ******************************************************************************/
#define ATC_CMD_PARAM_TYPE_NUMERIC    1
#define ATC_CMD_PARAM_TYPE_STRING     2
#define ATC_CMD_PARAM_TYPE_DEFAULT    3

/*******************************************************************************
 **                        NV default value define                          *
 ******************************************************************************/
#define ATC_STKTIME_DEFAULT_VAL    3
#define ATC_SIND_DEFAULT_VAL       0//2047
#define ATC_SIDETONE_DEFAULT_VAL   0
#define ATC_SPEAKER_DEFAULT_VAL    3
#define ATC_MICRO_DEFAULT_VAL      3

/*******************************************************************************
 **                        AT command line buf length                          *
 ******************************************************************************/
#ifdef AUDIO_PARAM_ARM
#define ATC_MAX_CMD_LINE_LENGTH       4096
#else
#define ATC_MAX_CMD_LINE_LENGTH       2100
#endif
#define ATC_MIN_CMD_LINE_LENGTH       8
#define ATC_MAX_CMD_RESPONSE_LENGTH   256

#define SIMAT_MAX_RPDU_DATA_LEN 256

//#ifdef _MUX_ENABLE_
#define ATC_BACKSPACE_CHAR  0x08

#define ATC_MAX_LINK_ID    18 
#define ATC_DEBUG_LINK  ATC_MAX_LINK_ID-1
#define ATC_MAX_EXP_RESULT   30
#define ATC_INVALID_LINK_ID   0xff
#define ATC_INVALID_CID  0xff
#define ATC_INVALID_NSAPI  0xff
#define ATC_MIN_NSAPI  5
#define ATC_INVALID_CALL_ID  0xff
#define ATC_NO_EVENT   0
#define ATC_PDP_NUM_MAX 2
//#endif

/*******************************************************************************
 **                       default  ATC config                                  *
 ******************************************************************************/
#define DEF_S0  0       /* default value for S0 */
#define DEF_S2  43      /* default value for S2 */
#define DEF_S3  13      /* default (ASCII 'CR') value for S3 */
#define DEF_S4  10      /* default (ASCII 'LF') value for S4 */
#define DEF_S5  8       /* default (ASCII 'BS' for S5 */
#define DEF_S6  2       /* default value for S6 */
#define DEF_S7  45      /* default value for S7 */
#define DEF_S8  2       /* default value for S8 */
#define DEF_S10 14      /* default value for S10 */

#define DEF_CR  0       /* disable the report */
#define DEF_CRC 0       /* disable the extend +CRING */

#define DEF_CREG 0      /* Disable unsolicited +CREG */
#define DEF_CGREG  0    /* Disable unsolicited +CGREG */

#define DEF_E  1        /* Echo ON */
#define DEF_L  0        /* -14db min speaker volume*/
#define DEF_M  0        /* off during the entire call*/
#define DEF_X  0        /* busy and dial tone off during a call*/
#define DEF_P  0        /* ?*/
#define DEF_Q  0        /* Quiet OFF */
#define DEF_V  1        /* diaplay verbose result code*/

#define DEF_CRSL_VALUE   6
#define DEF_CMOD  0      /* single mode*/

#define DEF_CBST_SPEED  7 /* 9.6k */
#define DEF_CBST_NAME  0  /* data circuit asynchronous */
#define DEF_CBST_CE  0    /* Transparent mode */

#define DEF_COPS_MODE  0   /* Automatic selection */
#define DEF_COPS_FORMAT  0 /* Long Format */

/*******************************************************************************
 **                        ATC status define                                   *
 ******************************************************************************/
//#define P_ATC       1000    /* ATC Task ID */
#define ATC_STATUS  uint    /* Return Status Type */

#define S_ATC_MODULE            1000

#define S_ATC_DISCARD           2
#define S_ATC_SUCCESS           1
#define S_ATC_FAIL              0

// for AOC feature
#define ATC_AOC_QUERY_MODE      0
#define ATC_AOC_DEACTIVE_MODE   1
#define ATC_AOC_ACTIVE_MODE     2

#define  CTRL_Z                0x1A   // ^z
#define  ESC                   0x1B   // Esc

#define S_ATC_DEFAULT_HANDLE        ( (S_ATC_MODULE<<16) + 7 )
#define S_ATC_READ_CMD_COMPLETE     ( (S_ATC_MODULE<<16) + 8 )
#define S_ATC_PROCESS_PENDING       ( (S_ATC_MODULE<<16) + 9 )
#define S_ATC_LAST_ERROR            ( (S_ATC_MODULE<<16) + 10 )  // @Steven

#define S_ATC_RESULT_CODE_BASE      ( (S_ATC_MODULE<<16) + 10 )
#define S_ATC_RESULT_CODE_OK        ( S_ATC_RESULT_CODE_BASE + 0 )
#define S_ATC_RESULT_CODE_CONNECT   ( S_ATC_RESULT_CODE_BASE + 1 )
#define S_ATC_RESULT_CODE_RING      ( S_ATC_RESULT_CODE_BASE + 2 )
#define S_ATC_RESULT_CODE_NO_CARRIER ( S_ATC_RESULT_CODE_BASE + 3 )
#define S_ATC_RESULT_CODE_ERROR     ( S_ATC_RESULT_CODE_BASE + 4 ) 
#define S_ATC_RESULT_CODE_NO_DIALTONE ( S_ATC_RESULT_CODE_BASE + 6 )
#define S_ATC_RESULT_CODE_BUSY      ( S_ATC_RESULT_CODE_BASE + 7 )
#define S_ATC_RESULT_CODE_NO_ANSWER ( S_ATC_RESULT_CODE_BASE + 8 )

/*******************************************************************************
 **                        ERROR index define                                  *
 ******************************************************************************/
#define ERR_PHONE_FAILURE                             0
#define ERR_NO_CONNECT_TO_PHONE                       1
#define ERR_PHONE_ADAPTOR_LINK_RESERVED               2
#define ERR_OPERATION_NOT_ALLOWED                     3
#define ERR_OPERATION_NOT_SUPPORTED                   4
#define ERR_PH_SIM_PIN_REQUIRED                       5
#define ERR_PH_FSIM_PIN_REQUIRED                      6
#define ERR_PH_FSIM_PUK_REQUIRED                      7

#define ERR_SIM_NOT_INSERTED                                10
#define ERR_SIM_PIN_REQUIRED                                11
#define ERR_SIM_PUK_REQUIRED                                12
#define ERR_SIM_FAILURE                                     13
#define ERR_SIM_BUSY                                        14
#define ERR_SIM_WRONG                                       15
#define ERR_INCORRECT_PASSWORD                              16
#define ERR_SIM_PIN2_REQUIRED                               17
#define ERR_SIM_PUK2_REQUIRED                               18

#define ERR_MEMORY_FULL                                     20
#define ERR_INVALID_INDEX                                   21
#define ERR_NOT_FOUND                                       22
#define ERR_MEMORY_FAILURE                                  23
#define ERR_TEXT_STRING_TOO_LONG                            24
#define ERR_INVALID_CHARACTERS_IN_TEXT_STRING               25
#define ERR_DIAL_STRING_TOO_LONG                            26
#define ERR_INVALID_CHARACTERS_IN_DIAL_STRING               27
#define ERR_GPRS_OPERATION_FAILURE                          28
#define ERR_GPRS_SEND_DATA_FAILURE                          29

#define ERR_NO_NETWORK_SERVICE                              30
#define ERR_NETWORK_TIMEOUT                                 31
#define ERR_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY        32

#define ERR_NETWORK_PERSONALISATION_PIN_REQUIRED            40
#define ERR_NETWORK_PERSONALISATION_PUK_REQUIRED            41
#define ERR_NETWORK_SUBSET_PERSONALISATION_PIN_REQUIRED     42
#define ERR_NETWORK_SUBSET_PERSONALISATION_PUK_REQUIRED     43
#define ERR_SERVICE_PROVIDER_PERSONALISATION_PIN_REQUIRED   44
#define ERR_SERVICE_PROVIDER_PERSONALISATION_PUK_REQUIRED   45
#define ERR_CORPORATE_PERSONALISATION_PIN_REQUIRED          46
#define ERR_CORPORATE_PERSONALISATION_PUK_REQUIRED          47

//shijun.cui add error code 2005.01.13
#define ERR_AT_COMMAND_DISCARDED                            60

//shijun.cui add sim error code 2005.02.03
#define ERR_SIM_CARD_REJ_BY_NET                             62
#define ERR_SIM_CARD_SERVICE_NOT_AVAILABLE                  63
#define ERR_SIM_CARD_PIN_UNINITIALIZED                      64
#define ERR_SIM_CARD_PIN_BLOCKED                            65
#define ERR_SIM_CARD_PUK_BLOCKED                            66

#define ERR_UNKNOWN                                         100
#define ERR_ILLEGAL_MS                                      103
#define ERR_ILLEGAL_ME                                      106
#define ERR_GPRS_SERVICES_NOT_ALLOWED                       107
#define ERR_PLMN_NOT_ALLOWED                                111
#define ERR_LOCATION_AREA_NOT_ALLOWED                       112
#define ERR_ROAMING_NOT_ALLOWED_IN_THIS_LOCATION_AREA       113

/* activate error code 2003.9.27 xu.xiaogang */
#define ERR_SERVICE_OPTION_NOT_SUPPORTED                    132
#define ERR_REQUEST_SERVICE_OPTION_NOT_SUBSCRIBED           133
#define ERR_SERVICE_OPTION_OUTOF_ORDER                      134
#define ERR_PDP_AUTHENTICATION_FAILED                       149
#define ERR_MISSING_OR_UNKOWN_APN                           533

//@Shijun.cui add for DIAG test 2005-03-07
#define MN_CAUSE_NO_NETWORK_SERVICE                         171
#define MN_CALL_ERR_EMERGENCY_CALL_ONLY                     172        // the MS only support emergency call
#define MN_CALL_ERR_NORMAL_DISCONNET                        173        // the normal clear procedure 
#define MN_CALL_ERR_REMOTE_USER_DISCONNECT                  174        // the remote user disconnect the connection
#define MN_CALL_ERR_LOW_FAILURE                             175       // the low failure(e.g RR connection fail)
#define MN_CALL_ERR_NETWORK_REJECT                          176       // the network reject 
#define MN_CALL_ERR_NO_CELL                                 177       // the MS do not camp on a cell

#define ERR_INVALID_INDEX             21
#define ERR_UNKNOWN                   100


#define ATC_CPAS_STATUS_READY          0X00
#define ATC_CPAS_STATUS_RING           0X03
#define ATC_CPAS_STATUS_PROGRESS       0X04

#define ATC_CPIN_CODE_READY            0X00
#define ATC_CPIN_CODE_PIN1             0X01
#define ATC_CPIN_CODE_PIN2             0X02
#define ATC_CPIN_CODE_PUK1             0X03
#define ATC_CPIN_CODE_PUK2             0X04
#define ATC_CPIN_CODE_BLOCK            0X05
#define ATC_CPIN_CODE_MAX              0X06

#define  AT_INVALID_CMD_ID             0XFFFF
// define the CMEE code
#define NO_RESULT_CODE_MODE            0x00
#define NUMERIC_ERR_VALUE_MODE         0x01
#define VERBOSE_ERR_VALUE_MODE         0x02
/*******************************************************************************
 **                        SDTT acrom define                                   *
 ******************************************************************************/
#define AT_SDTT_DELETE_OPER        0
#define AT_SDTT_START_OPER         1
#define AT_SDTT_TRANSFER_OPER      2
#define AT_SDTT_STOP_OPER          3
#define AT_SDTT_NULL_OPER          0xFF

#define ATC_SHARP_LEN              512

#define ATC_MAX_ACTIVATE_PDP  MN_GPRS_MAX_PDP_CONTEXT_COUNT

/*******************************************************************************
 **                        Check Current Sim Card ID                                   *
 ******************************************************************************/
#if !defined(_MUX_ENABLE_) && (defined(MULTI_SIM_SYS_DUAL) || defined(MULTI_SIM_SYS_TRI) || defined(MULTI_SIM_SYS_QUAD))
#define COND_CURRENT_CARD_ID(SIG_STRUCT) \
    if(g_current_card_id != ((SIG_STRUCT *)sig_ptr)->dual_sys)

#define CHECK_CURRENT_CARD_ID_BRK(SIG_STRUCT) \
    COND_CURRENT_CARD_ID(SIG_STRUCT)\
        break;
#define CHECK_CURRENT_CARD_ID_RTN(SIG_STRUCT) \
    COND_CURRENT_CARD_ID(SIG_STRUCT)\
        return S_ATC_DEFAULT_HANDLE;
#else
#define COND_CURRENT_CARD_ID(SIG_STRUCT)
#define CHECK_CURRENT_CARD_ID_BRK(SIG_STRUCT) 
#define CHECK_CURRENT_CARD_ID_RTN(SIG_STRUCT) 
#endif /* MULTI_SIM_SYS_DUAL || MULTI_SIM_SYS_TRI  || MULTI_SIM_SYS_QUAD*/

#ifdef _MUX_ENABLE_
#define ATC_CURRENT_SIM_ID_MUX  ATC_GetSimIdFromLinkId(atc_config_ptr->current_link_id)
#else
#define ATC_CURRENT_SIM_ID_MUX  g_current_card_id
#endif

#if !defined(_MUX_ENABLE_) || defined(_MUX_REDUCED_MODE_)
#define ATC_ADD_EXPECTED_EVENT(atc_config_ptr,expected_event,id,domain_id)
#define ATC_RECEIVE_EXPECTED_EVENT(atc_config_ptr,expected_event,id,domain_id)
#define ATC_CLEAR_EXP_RESULT(atc_exp_result_info)
#define ATC_CLEAR_ALL_EXP_RESULT(id,domain_id)
#else
#define ATC_ADD_EXPECTED_EVENT(atc_config_ptr,expected_event,id,domain_id) \
            ATC_Add_Expected_Event((atc_config_ptr),(expected_event),(id),(domain_id))
#define ATC_RECEIVE_EXPECTED_EVENT(atc_config_ptr,expected_event,id,domain_id)  \
            ATC_Receive_Expected_Event((atc_config_ptr),(expected_event),(id),(domain_id))
#define ATC_CLEAR_EXP_RESULT(atc_exp_result_info) ATC_Clear_Exp_Result((atc_exp_result_info))
#define ATC_CLEAR_ALL_EXP_RESULT(id,domain_id) ATC_Clear_All_Exp_Result((id), (domain_id))
#endif

#ifdef _MUX_ENABLE_
#define ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,link_id) ATC_Set_Current_Link_id((atc_config_ptr), (link_id))
#ifdef _MUX_REDUCED_MODE_
#define ATC_SET_CURRENT_LINK_ID(atc_config_ptr,link_id) ATC_Set_Current_Link_id((atc_config_ptr), (link_id))
#else
#define ATC_SET_CURRENT_LINK_ID(atc_config_ptr,link_id)
#endif
#else
#define ATC_SET_CURRENT_LINK_ID(atc_config_ptr,link_id)
#define ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,link_id)
#endif

#ifdef ATC_LESSEN_TRACE
#define SCI_TRACE_SLIGHT(...)
#else
#define SCI_TRACE_SLIGHT SCI_TRACE_LOW
#endif

/*******************************************************************************
 **                              Type declaration                              *
 ******************************************************************************/
typedef enum
{
    ATC_REL_HELD_CALLS_OR_SET_UDUB,          // Releases all held calls, or sets User-Determined User Busy for a waiting call
    ATC_REL_ACTIVE_CALLS_AND_ACCEPT_OTHERS,  // Releases all active calls and accepts the other (waiting or held) call
    ATC_HOLD_CALLS_AND_ACCEPT_OTHERS,        // Places all active calls on hold and accepts the other (held or waiting) call
    ATC_ADD_HELD_TO_CONVERSATION,            // Adds a held call to the conversation
    ATC_ECT           = 4,       // ref:3gpp 02.30
    ATC_REL_SPECIFIC_X_CALL_BASE = 10,       // Releases the specific active call X
    ATC_HOLD_CALLS_EXCEPT_X_BASE = 20,       // Places all active calls, except call X, on hold
    ATC_INVALID_OPERATE
} ATC_CHLD_OPERATION_E;

typedef enum
{
    ANS_OFF_AUTO_GPRS_MODE,
    ANS_ON_AUTO_GPRS_MODE,
    ANS_MODEM_GPRS_MODE,
    ANS_MODEM_GPRS_GSM_MODE
}ATC_ANS_MODE_T;

typedef enum
{
    CSSU_CODE_0 = 0,   // This is a forwarded call
    CSSU_CODE_2 = 2,   // A call has been put on hold (during voice call)
    CSSU_CODE_3 = 3,   // A call has been retrieved (during voice call)
    CSSU_CODE_5 = 5    // The call on hold has been released (during voice call)
} ATC_CSSU_CODE_E;

typedef enum
{
    CSSI_CODE_0 = 0,  // Unconditional call forwarding is active
    CSSI_CODE_1 = 1,  // Some of the conditional call forwardings are active
    CSSI_CODE_2 = 2,  // A call has been forwarded
    CSSI_CODE_3 = 3,  // A call is waiting
    CSSI_CODE_5 = 5,  // Outgoing calls are barred
    CSSI_CODE_6 = 6   // Incoming calls are barred
} ATC_CSSI_CODE_E;
typedef enum
{
    ATC_CTRLZ_DOWNFILE,
    ATC_CTRLZ_DOWNMIDI,  
    ATC_CTRLZ_SMS,
    ATC_CTRLZ_SRWD,
    ATC_CTRLZ_STK,
    ATC_CTRLZ_SSEA,
    ATC_CTRLZ_GPRS,
    ATC_CTRLZ_DOWNFILE_CHK,
    ATC_CTRLZ_SNVM,
    ATC_CTRLZ_SADM,
    ATC_CTRLZ_SADMNV,
    ATC_CTRLZ_SADM_ARM,
    ATC_CTRLZ_SADMNV_ARM,
    ATC_CTRLZ_SADMDSP,
    ATC_CTRLZ_SADMNV_DSP,
    ATC_CTRLZ_ENHA,
    ATC_CTRLZ_ENHA_TUN,
    ATC_CTRLZ_ENHANV,
    ATC_CTRLZ_ENHANV_TUN,
    ATC_CTRLZ_STKSMS,
    /* +add for lvve function */
    ATC_CTRLZ_LVVE,
    ATC_CTRLZ_LVVENV,
    /* -add for lvve function */

    ATC_CTRLZ_MAX
} ATC_CTRLZ_STATUS_E;

typedef struct
{
    const char *str;
} ATC_RSP_STR_T;

//GPRS Attach mode
typedef enum 
{
    ATC_GPRS_DETACH,
    ATC_GPRS_ALWAYS,
    ATC_GPRS_SETTINGS_MAX
}ATC_GPRS_SETTINGS_E;

/*******************************************************************************
 **                         Structure declaration                              *
 ******************************************************************************/
typedef struct atc_param_numeric_tag
{
    const int32     val_min;  /* Minimum value of numeric parameter */
    const int32     val_max;  /* Maximum value of numeric parameter */
}ATC_PARAM_NUMERIC_T;

typedef struct atc_param_string_tag
{
    const uint     str_len;

    const uint8 *    str_ptr;
}ATC_PARAM_STRING_T;

typedef struct atc_param_info_tag
{
    const uint16     param_type; /* Parameter type: 1 - numeric
                                                    2 - string   */
    const uint16     param_range_num;

    union 
    {
        const ATC_PARAM_STRING_T  * str_array_ptr;  /* Point to an array of
                                                       string parameter
                                                       value */
        const ATC_PARAM_NUMERIC_T * num_array_ptr;  /* Point to an array of 
                                                       numeric parameter 
                                                       value range */
    }param_info;
}ATC_PARAM_INFO_T;

typedef struct atc_info_tag
{
    const uint16     cmd_index;  /* index of command */
    const uint16     cmd_type;   /* command type: 1 -- execute
                                                  2 -- set
                                                  4 -- read
                                                  8 -- test    */

    const ATC_PARAM_STRING_T * cmd_name;   /* command name */
    const uint               param_num;  /* number of parameters */
    const ATC_PARAM_INFO_T * param_ptr;  /* point to an array of parameter 
                                            infomation */
}ATC_INFO_T;

typedef struct atc_cur_param_string_tag
{
    uint       str_len;

    uint8 *    str_ptr;
}ATC_CUR_PARAM_STRING_T;

typedef struct atc_cur_param_info_list_tag 
{
    uint16     param_type; /* Parameter type: 1 - numeric
                                              2 - string
                                              3 - default  */
    uint16     padding;

    union 
    {
        int32                num;  /* numeric parameter value */
        ATC_CUR_PARAM_STRING_T * str_ptr;  /* point to the address of the 
                                          parameter string */
    }param_info;

    struct atc_cur_param_info_list_tag * next;
}ATC_CUR_PARAM_INFO_LIST_T;

typedef struct atc_cur_info_list_tag
{
    uint    cmd_index;  /* index of the current command */
    uint    cmd_type;   /* command type: 1 -- execute
                                         2 -- set
                                         4 -- read
                                         8 -- test      */
    uint    cur_param_num;

    BOOLEAN param_used[ATC_MAX_PARAM_NUM];

    ATC_CUR_PARAM_INFO_LIST_T *   cur_param_list_ptr;

    struct atc_cur_info_list_tag * next;
}ATC_CUR_INFO_LIST_T;

typedef enum
{
    AUTO_MODE              = 0,     // automatic mode <oper> field is ignored
    MANUAL_MODE            = 1,     // manual operator selection <oper> field is present
    MANUAL_DEREGISTER      = 2,     // manual deregister from network and remain unregistered until mode 0,1,4 is selected
    SET_ONLY_FORMAT        = 3,     // set only <format> (for read command +COPS) 
    MANUAL_MATO_MODE       = 4      // if manual selection is fails,automatic mode is entered

} PLMN_SELECTION_MODE_E;

typedef enum
{
    NOT_REGISTERED_NOT_SEARCHING,   // Not registered The ME is currently not searching for a new operator to register to
    REGISTERED_HOME_PLMN,           // Registered; home network
    NOT_REGISTERED_AND_SEARCHING,   // Not registered The ME is currently searching for a new operator to register to
    REGISTRATION_DENIED,            // Registration denied
    UNKNOWN,                        // Unknown
    REGISTERED_ROAMING              // Registered; roaming
} OPER_STATUS_E;

typedef enum
{
    LONG_FORMAT_STR         = 0,    // long format alphanumeric <oper>
    SHORT_FORMAT_STR        = 1,    // short format alphanumeric <oper>
    NUMERIC                 = 2     // numeric <oper>

} OPER_FORMAT_E;


typedef struct
{
  PLMN_SELECTION_MODE_E  mode;
  OPER_FORMAT_E          format;
} ATC_COPS_T;

typedef struct
{
    uint8   speed;
    uint8   name;   // sync or unsync / circuit or packet
    uint8   ce;     // connection element, transparent mode or none transparent
}ATC_CBST_T;

typedef enum
{
    ATC_PIN_NO_OPERATE,
    ATC_PIN_RESET_ACM_UNIT,
    ATC_PIN_SET_ACM_MAX,
    ATC_PIN_SET_PPU,
    ATC_PIN_GET_PPU
} ATC_PIN_OPERATE_E;
typedef enum
{
    ATC_CCED_SCELL = 1,
    ATC_CCED_NCELL = 2,
    ATC_CCED_TIME_ADV = 4,
    ATC_CCED_INVALID,
    ATC_CCED_CSQ = 8
} ATC_CCED_TYPE_E;


typedef struct atc_config_nv_tag
{
/*----------------------------------------------------------------------------*/
// Below are ATC parameter which need to save in the nv.If need new one,it is 
// neccessary to add new item in the following.
/*----------------------------------------------------------------------------*/
    BOOLEAN     clip_flag;       /* TRUE:  Enable presentation of the CLI at the TE is enabled
                                   ** FALSE: Disable presentation of the CLI at the TE is enabled
                                 */
    BOOLEAN     clir_flag;        /* TRUE: CLIR invocation
                                         ** FALSE: CLIR suppression
                                         */
    BOOLEAN     colp_flag;        /* TRUE:  Enable showing the result code presentation status in the TA
                                  ** FALSE: Disable showing the result code presentation status in the TA
                                 */
    BOOLEAN     cmgf_flag;        /* TRUE: TEXT mode
                                 ** FALSE: PDU mode
                                 */
    BOOLEAN     mux_mode;

    BOOLEAN     battery_indicator; /* disable or enable the battery charge indication
                                          **
                                          */

    uint8       e_value; /* Command echo : TRUE  -- echo characters
                                            FALSE -- does not echo */
    uint8       creg;    /*network state report*/
    uint8       sidet_flag;
    uint8       call_ring_index;  /* 0: no melody (default)
                                 ** 1-8
                                 */
    uint8       cmee;
    uint8       stk_actflag;
    uint8       stk_time;

    uint16      wind_ind_level;
#ifndef MS_OAPI_SUPPORT //nv_type.nvm use 09a
    uint8       spauto_flg;
#endif
}ATC_CONFIG_NV_T;


typedef enum
{
    ATC_PSD_STATE_INACTIVE = 0x00,
    ATC_PSD_STATE_ACTIVATE_PENDING,
    ATC_PSD_STATE_ACTIVE,
    ATC_PSD_STATE_MODIFY_PENDING,
    ATC_PSD_STATE_DEACTIVATE_PENDING
} ATC_PSD_STATE_E;

/* CVHU voice hangup control mode type */
typedef enum
{
    ATC_CS_IGNORE_DTR = 0,
    ATC_CS_IGNORE_DTR_AND_ATH_BOTH,
    ATPP_CS_DTR_AND_ATH_ACT_AUTO
} ATC_CVHU_MODE_ENUM;

typedef struct atc_psd_call_info_tag
{
    uint8            link_id;       /* link id PSD call worked */
    uint8            data_link_id; /* link id PSD call worked */
    uint8            cid;           /* cid */
    BOOLEAN          data_mode_req; /* ppp dial request or not */
    BOOLEAN          data_mode;     /* ppp dial or not */
    uint8            nsapi;         /* nsapi of psd call */
    ATC_PSD_STATE_E  state;         /* ONLY apply as reference!!! */
}ATC_PSD_CALL_INFO_T;

#ifdef _MUX_ENABLE_
#define MUX_LINK_AT_1 1
#define MUX_LINK_AT_2 2
#define MUX_LINK_PPP 3
typedef struct atc_link_config_tag
{
    BOOLEAN          is_ctrlz_end;
    uint8                ctrlz_end_status;
}ATC_LINK_CONFIG_T;

typedef enum
{
    ATC_DOMAIN_CS = 0,
    ATC_DOMAIN_PS,
    ATC_NO_DOMAIN
}ATC_EVENT_DOMAIN_T;


typedef struct
{
   uint8          link_id;
   uint8          call_id;
   uint8          cid;
   uint8          nsapi;
   uint16        exp_event;
   uint16        ext_exp_event;
}ATC_EXP_RESULT_INFO_T;

typedef struct
{
   ATC_EXP_RESULT_INFO_T  atc_exp_result_info[ATC_MAX_EXP_RESULT];
   uint16  start_point; 
   uint16  end_point;
}ATC_EXP_LINK_INFO_T;

typedef struct atc_config_tag
{
/*----------------------------------------------------------------------------*/
// Below are ATC Options, these options affect the behavior of the MT on 
// handling AT Commands and responsing to TE.
/*----------------------------------------------------------------------------*/
    uint8       s0_reg;       // Nb rings before auto answer 0 -> disable
    uint8       s2_reg;       
    uint8       s3_reg;       /* Command line termination character */
    uint8       s4_reg;       /* Response formatting character */
    uint8       s5_reg;       /* Command line editing character */
    uint8       s6_reg;       // Pause duration before blind dialling
    uint8       s7_reg;       // Wait time for call completion
    uint8       s8_reg;       // Pause duration in secs when dialling (when , is detected)
    uint8       s10_reg;      // Automatic Disconnect Delay Control ( Dummy)    

    uint8       and_c;
    uint8       and_d;
    uint8       q_value;      /* Result code suppression */
    uint8       v_value;      /* DCE response format */
    uint8       m_value;      //Monitor Speaker Mode
    uint8       x_value;      /* Result code selection and call progress 
                                 monitoring control */
    uint8       l_value;      //Monitor Speaker Loudness
    uint8       p_value;      //Select Pulse Dialling
    
    uint8       cr;           //display the intermediate bearer capability report
    uint8       crc;          //extended +CRING

    uint8       cgreg;        //an unsolicited result code +CGREG(GPRS)

    uint8       cmod;         //call mode
    uint8       vtd_value;    //during time of DTMF tone
 

    BOOLEAN            cssi_flag;        /* TRUE: Enable the +CSSI result code 
                                         ** FALSE: Disable the +CSSU result code
                                         */
    BOOLEAN            cssu_flag;        /* TRUE: Enable the +CSSU result code
                                         ** FALSE: Disable the +CSSU result code
                                         */
    BOOLEAN            ccwa_flag;        /* TRUE: Enable the unsolicited result code +CCWA
                                         ** FALSE: Disable the unsolicited result code +CCWA
                                         */
    BOOLEAN            cmut_flag;        /* TRUE: mute on
                                         ** FALSE; mute off
                                         */
    BOOLEAN            sdmut_flag;        /* TRUE: mute on in downlink
                                         ** FALSE; mute off in downlink
                                         */

    BOOLEAN            crsl_value;

    BOOLEAN            cpin_flag[MN_DUAL_SYS_MAX];
    BOOLEAN            pin1_enable_flag[MN_DUAL_SYS_MAX];
    uint8              cfun_flag;
    uint8              cscs[10];         /*
                                         ** selected character set
                                         */
    uint8              aoc_mode;         /* 0: Query CCM value.
                                         ** 1: Deactivate the unsolicited reporting of CCM value
                                         ** 2: Activate the unsolicited reporting of CCM value
                                         */
    uint32             ccm_val;
    uint8              cicb_val;         /* AT+CICB=<mode> 0:data call 
                                         ** 1: fax call 2: speech call
                                         */

    uint8              ath_flag;
    BOOLEAN            sim_flag[MN_DUAL_SYS_MAX];         /* This flag indicates if the sim is ok or not
                                         **
                                         */
    BOOLEAN            smscb_flag[MN_DUAL_SYS_MAX];       
                                         
    BOOLEAN            audio_flag;    
    BOOLEAN            call_read_flag[MN_DUAL_SYS_MAX];

    BOOLEAN            csq_flag;         // auto indicate the rssi
    ATC_CCED_TYPE_E    cced_type;        // scell ncell timeadvance
    BOOLEAN            cmer_flag[MN_SYS_NUMBER];
    uint8                           rel_former[MN_SYS_NUMBER];

    uint32             ceer_val;         //error type 

    ATC_CBST_T      cbst;
    
    ATC_COPS_T      cops;               //net registersation  mode
    uint8           cpas;               //state of DCE
    uint8           cpin[MN_DUAL_SYS_MAX];                 /*
                                         **
                                         */
    uint8           midi_oper;
    uint8           midi_index;
    uint16          midi_block;
    
    uint8           audio_mode;
    uint8           speaker_volume; // speaker volume
    uint8           ssea_para;
    uint16          ssea_vol_up;
    uint16          ssea_vol_down;

    /*
    **  The following for AOC feature
    */
    ATC_PIN_OPERATE_E    operate;    
    uint32               acm_max;
    MNSIM_PUCT_INFO_T    ppc;        //the price per unit.
        BOOLEAN              is_ctrlz_end;
    uint8                ctrlz_end_status;
    uint8                ms_class;  //the MS class

    ATC_CHLD_OPERATION_E  ss_operate;
    BOOLEAN               plmn_search_flag;

    uint32      midi_size_arr[ATC_MAX_MIDI_INDEX];
    
    //This is about the item which store in NV. 
    ATC_CONFIG_NV_T    atc_config_nv;
       /*+CR99654*/
    BOOLEAN   is_fdn_enable;
    /*-CR99654*/
    uint8    c_activate_pdp;
    uint8    activate_pdp_id[ATC_MAX_ACTIVATE_PDP];
    uint8    current_link_id;

    ATC_EXP_LINK_INFO_T     atc_exp_link_info;

    ATC_LINK_CONFIG_T  atc_link_config[ATC_MAX_LINK_ID];
    uint8       trace_value;    //AT+TRACE
    ATC_PSD_CALL_INFO_T atc_psd_call_info[ATC_MAX_ACTIVATE_PDP];

    //identify PDP id activated by ATD*99#
    BOOLEAN pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT];
    
    /* VOICE hangup control mode, effects ATH */
    ATC_CVHU_MODE_ENUM          voice_hangup_mode;
                 
}ATC_CONFIG_T;

#else

typedef struct atc_config_tag
{
/*----------------------------------------------------------------------------*/
// Below are ATC Options, these options affect the behavior of the MT on 
// handling AT Commands and responsing to TE.
/*----------------------------------------------------------------------------*/
    uint8       s0_reg;       // Nb rings before auto answer 0 -> disable
    uint8       s2_reg;       
    uint8       s3_reg;       /* Command line termination character */
    uint8       s4_reg;       /* Response formatting character */
    uint8       s5_reg;       /* Command line editing character */
    uint8       s6_reg;       // Pause duration before blind dialling
    uint8       s7_reg;       // Wait time for call completion
    uint8       s8_reg;       // Pause duration in secs when dialling (when , is detected)
    uint8       s10_reg;      // Automatic Disconnect Delay Control ( Dummy)    

    uint8       and_c;
    uint8       and_d;
    uint8       q_value;      /* Result code suppression */
    uint8       v_value;      /* DCE response format */
    uint8       m_value;      //Monitor Speaker Mode
    uint8       x_value;      /* Result code selection and call progress 
                                 monitoring control */
    uint8       l_value;      //Monitor Speaker Loudness
    uint8       p_value;      //Select Pulse Dialling

    uint8       cr;           //display the intermediate bearer capability report
    uint8       crc;          //extended +CRING

    uint8       cgreg;        //an unsolicited result code +CGREG(GPRS)

    uint8       cmod;         //call mode
    uint8       vtd_value;    //during time of DTMF tone


    BOOLEAN            cssi_flag;        /* TRUE: Enable the +CSSI result code 
                                     ** FALSE: Disable the +CSSU result code
                                     */
    BOOLEAN            cssu_flag;        /* TRUE: Enable the +CSSU result code
                                     ** FALSE: Disable the +CSSU result code
                                     */
    BOOLEAN            ccwa_flag;        /* TRUE: Enable the unsolicited result code +CCWA
                                     ** FALSE: Disable the unsolicited result code +CCWA
                                     */
    BOOLEAN            cmut_flag;        /* TRUE: mute on
                                     ** FALSE; mute off
                                     */
    BOOLEAN            sdmut_flag;        /* TRUE: mute on in downlink
                                     ** FALSE; mute off in downlink
                                     */

    BOOLEAN            crsl_value;

    BOOLEAN            cpin_flag[MN_SYS_NUMBER];
    BOOLEAN            pin1_enable_flag[MN_SYS_NUMBER];
    BOOLEAN            cfun_flag;
    uint8              cscs[10];         /*
                                     ** selected character set
                                     */
    uint8              aoc_mode;         /* 0: Query CCM value.
                                     ** 1: Deactivate the unsolicited reporting of CCM value
                                     ** 2: Activate the unsolicited reporting of CCM value
                                       */
    uint8              cicb_val;         /* AT+CICB=<mode> 0:data call 
                                     ** 1: fax call 2: speech call
                                     */

    uint8              ath_flag;
    BOOLEAN            sim_flag[MN_SYS_NUMBER];         /* This flag indicates if the sim is ok or not*/                                 
                                     
    BOOLEAN            smscb_flag[MN_SYS_NUMBER];      
                                       
    BOOLEAN            audio_flag;    
    BOOLEAN            call_read_flag[MN_SYS_NUMBER];

    BOOLEAN            csq_flag;         // auto indicate the rssi
    ATC_CCED_TYPE_E    cced_type;        // scell ncell timeadvance
    BOOLEAN            cmer_flag[MN_SYS_NUMBER];
    uint8                           rel_former[MN_SYS_NUMBER];
   
    uint32             ceer_val;         //error type 

    ATC_CBST_T      cbst;

    ATC_COPS_T      cops;               //net registersation  mode
    uint8           cpas;               //state of DCE
    uint8           cpin[MN_SYS_NUMBER];                /*
                                     **
                                     */
    uint8           midi_oper;
    //uint8           midi_index;
    //uint16          midi_block;

    uint8           audio_mode;
    uint8           speaker_volume; // speaker volume
    uint8           ssea_para;
    uint16          ssea_vol_up;
    uint16          ssea_vol_down;

    /*
    **  The following for AOC feature
    */
    ATC_PIN_OPERATE_E    operate;    
    uint32               acm_max;
    MNSIM_PUCT_INFO_T    ppc;        //the price per unit.
    BOOLEAN              is_ctrlz_end;
    uint8                ctrlz_end_status;
    uint8                ms_class;  //the MS class
 
    ATC_CHLD_OPERATION_E  ss_operate;
    BOOLEAN               plmn_search_flag;

    uint32      midi_size_arr[ATC_MAX_MIDI_INDEX];

    //This is about the item which store in NV. 
    ATC_CONFIG_NV_T    atc_config_nv;

    //identify PDP id activated by ATD*99#
    BOOLEAN pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT];

    /* VOICE hangup control mode, effects ATH */
    ATC_CVHU_MODE_ENUM          voice_hangup_mode;
}ATC_CONFIG_T;

#endif

typedef enum
{
       UNKNOWN_PLAN_UNKNOWN_TYPE         = 128, // Unknown numbering plan  unknown 
       ISDN_TELE_PLAN_UNKNOWN_TYPE       = 129, // ISDN/telephony number plan, //unknow number
       ISDN_TELE_PLAN_INTERNATIONAL_TYPE = 145, // ISDN/telephony number plan, // international number
       ISDN_TELE_PLAN_NATIONAL_TYPE      = 161 // ISDN/telephony number plan // national number
} ATC_TELE_TYPE_E;

typedef enum
{
    CICB_DATA_CALL,
    CICB_FAX_CALL,
    CICB_SPEECH_CALL
} ATC_CICB_E;       //incoming call type

typedef ATC_STATUS (* ATC_Func_Handle_F)( 
        ATC_CONFIG_T * , 
        ATC_CUR_INFO_LIST_T * );
        
#ifdef _MUX_ENABLE_

typedef uint8      ATC_REC_BUFFER[ATC_REC_BUF_SIZE];  

/*
    Software structures
*/
typedef struct 
{
     uint32  size;              // Total Size of the buffer.
     volatile int  start_point;       // First data in the buffer.
        volatile int  end_point;         // Last data in the buffer.
        volatile uint32  status;            // Indicate the buffer status.
        uint32  lost_num;          // 
        uint8   *atc_buf_ptr;      // atc buffer address.
} ATC_BUF_T;

typedef struct atc_link_info_tag
{
    uint8       cmd_line_buf_long[ATC_MAX_CMD_LINE_LENGTH];
    uint8       cmd_line_buf_short[ATC_MIN_CMD_LINE_LENGTH+ 2];

    uint8       * cur_cmd_line_buf_ptr;
    uint8       * last_cmd_line_buf_ptr;

    ATC_CUR_INFO_LIST_T   * atc_info_ptr; // store the infos of AT cmds we
                                          // want to handle.
}ATC_LINK_INFO_T;

typedef struct atc_global_info_tag
{
/*----------------------------------------------------------------------------*/
// We define two AT Cmd line buf : 
//     the long buf is used to store normal ATC line 
//     the short buf is used to store ATC line which begins with "A/"
/*----------------------------------------------------------------------------*/
    uint16      buffered_at_cmd_count; //@Ivan.Yin for avoiding task queue full 2004-03-22

    uint8       c_atc_link_id;
    uint8       atc_link_id[ATC_MAX_LINK_ID];
    ATC_LINK_INFO_T     atc_link_info[ATC_MAX_LINK_ID];
//    const uint8          ** atc_result_code_text;

    const ATC_INFO_T      * atc_info_table;     //command info table
    ATC_Func_Handle_F     * atc_handle_table;   //command process ptr table

    ATC_CONFIG_T          * atc_config_ptr;

}ATC_GLOBAL_INFO_T;

#else/*_MUX_ENABLE_*/

typedef struct atc_global_info_tag
{
/*----------------------------------------------------------------------------*/
// We define two AT Cmd line buf : 
//     the long buf is used to store normal ATC line 
//     the short buf is used to store ATC line which begins with "A/"
/*----------------------------------------------------------------------------*/
    uint8       cmd_line_buf_long[ATC_MAX_CMD_LINE_LENGTH];
    uint8       cmd_line_buf_short[ATC_MIN_CMD_LINE_LENGTH + 2];
    uint16      buffered_at_cmd_count; //@Ivan.Yin for avoiding task queue full 2004-03-22

    uint8       * cur_cmd_line_buf_ptr;
    uint8       * last_cmd_line_buf_ptr;

/*----------------------------------------------------------------------------*/
// The response of AT Commands are first store in cmd_response_buf, 
// then we send it to SIO.
/*----------------------------------------------------------------------------*/

//    const uint8          ** atc_result_code_text;

    const ATC_INFO_T      * atc_info_table;     //command info table
    ATC_Func_Handle_F     * atc_handle_table;   //command process ptr table

    ATC_CUR_INFO_LIST_T   * atc_info_ptr; // store the infos of AT cmds we
                                          // want to handle.

    ATC_CONFIG_T          * atc_config_ptr;
}ATC_GLOBAL_INFO_T;

#ifdef BLUETOOTH_SUPPORT
typedef struct
{
    unsigned short      signal_code;
    unsigned short      signal_size;
    struct MS_HEAD_tag  *pre;
    struct MS_HEAD_tag  *succ;
    unsigned long       sender;
    void *data;
}BT_RECV_NEW_AT_T;
#endif

#endif

#define AT_CMD_FUNC(_F_)    \
 ATC_STATUS _F_(ATC_CONFIG_T        *atc_config_ptr, \
                            ATC_CUR_INFO_LIST_T *cur_cmd_info)
// add by Xueli Lv
typedef uint8      RSP_RESULT_STR[MAX_ATC_RSP_LEN];  
   
#ifndef _RTOS
#define SIGNAL_VARS 
#endif

#ifdef _MUX_ENABLE_
typedef struct
{
    SIGNAL_VARS
    uint32         len; 
    uint8           link_id;
    uint8           align[3];
}ATC_MUX_RECV_NEW_AT_T ;
#else/*_MUX_ENABLE_*/
typedef struct
{
    SIGNAL_VARS
    uint32         len; 
} SIO_RECV_NEW_AT_T ;
#endif

typedef struct
{
    SIGNAL_VARS
    uint32         len; 
    uint8 *        str;
} AT_WRITE_RSP_T ;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN is_rst;
} AT_FACTORST_T;

typedef struct
{
    SIGNAL_VARS
} AT_FCMPTEST_T;

enum
{
    ATC_SMS_ERR_300 ,
    ATC_SMS_ERR_301 ,
    ATC_SMS_ERR_302 ,
    ATC_SMS_ERR_303 ,
    ATC_SMS_ERR_304 ,
    ATC_SMS_ERR_305 ,
    ATC_SMS_ERR_310 ,
    ATC_SMS_ERR_311 ,
    ATC_SMS_ERR_312 ,
    ATC_SMS_ERR_313 ,
    ATC_SMS_ERR_315 ,
    ATC_SMS_ERR_316 ,
    ATC_SMS_ERR_317 ,
    ATC_SMS_ERR_318 ,
    ATC_SMS_ERR_320 ,
    ATC_SMS_ERR_321 ,
    ATC_SMS_ERR_322 ,
    ATC_SMS_ERR_330 ,
    ATC_SMS_ERR_331 ,
    ATC_SMS_ERR_332 ,
    ATC_SMS_ERR_340 ,
    ATC_SMS_ERR_500 ,
    ATC_SMS_ERR_512
};

enum
{
    ATC_CCED_ONE_SHOT,
    ATC_CCED_AUTO_SHOT,
    ATC_CCED_STOP_AUTO
};
typedef struct
{
    uint16       error_code;
    char*        error_str;
} ATC_SMS_RSP_STR_T;
typedef struct
{
    uint16          lev;
    uint16          val;
} AT_DATA_CONTEXT_T;
typedef struct
{
    uint32          reg_val;
    uint32          atc_val;
} AT_BAUTRATE_CONTEXT_T;

#ifndef _RTOS
        #define SCI_ASSERT(_bool)  assert(_bool)
#endif

typedef enum
{
    ATC_HOLD_IDLE = 0,
    ATC_CALL_HOLD
} ATC_HOLD_STATE_E;

typedef enum
{
    ATC_MPTY_IDLE = 0,
    ATC_CALL_MPTY
} ATC_MPTY_STATE_E;

typedef enum
{
    ATC_CALL_IDLE_STATE = 0,            // idle state
    ATC_CALL_OUTGOING_STATE,            // the user has originated a MO call
    ATC_CALL_ALERTING_STATE,            // Alerting indication of MO call    
    ATC_CALL_INCOMING_STATE,            // incoming a MT call
    ATC_CALL_ACTIVE_STATE              // the call entity has finish the setup procedure
} ATC_CALL_ENTITY_STATE_E;

#define ATC_CALL_IN_ACTIVE  0
#define ATC_CALL_IN_HELD    1
#define ATC_CALL_IN_DIALING 2
#define ATC_CALL_IN_ALERTNG 3
#define ATC_CALL_IN_INCOMING 4
#define ATC_CALL_IN_WAITING 5

typedef struct
{
    BOOLEAN                   direction;
    uint8                     call_id;
    ATC_CALL_ENTITY_STATE_E   call_entity_state;
    ATC_HOLD_STATE_E          hold_state;               // current call entity's HOLD auxiliary state
    ATC_MPTY_STATE_E          mpty_state;               // current call entity's MPTY auxiliary state
    uint8                     party_number[MN_MAX_ADDR_BCD_LEN*2+2];
} ATC_CALL_CONTEXT_T;

typedef struct
{
    uint8                    call_amount;
    ATC_CALL_CONTEXT_T       call_context[MN_CALL_MAX_CALL_NUM];
} ATC_CALLS_STATE_T;

#define ATC_INVALID_CALLID   0xFF

typedef enum
{
    ATC_IDLE ,
    ATC_TRAFFIC,
    ATC_BOTUM
} ATC_RRCELL_STATUS;

typedef enum
{
    LOOPTEST_MODE_OFF                       = 0,
    LOOPTEST_MODE_MIC_RECEIVER             = 2,
    LOOPTEST_MODE_MIC_LOUD_SPEAKER    = 3,
    LOOPTEST_MODE_MIC2_LOUD_SPEAKER  = 5,
    LOOPTEST_MODE_MIC_EAR_SPEAKER       = 7,
    LOOPTEST_MODE_INVALID = 0xff
}ATC_LOOPTEST_MODE;

typedef enum
{
    LOOPTEST_STATUS_ON  = 0,
    LOOPTEST_STATUS_OFF = 1,
    LOOPTEST_STATUS_INVALID = 0xff
}ATC_LOOPTEST_STATUS;
/*******************************************************************************
 **                        Functon  declaration                                *
 ******************************************************************************/
#ifndef _ULTRA_LOW_CODE_
/******************************************************************************/
// Description : write the all item which need in ATC
// Global resource dependence : None
// Author : hanjun.liu
// Note :  
/******************************************************************************/     
extern BOOLEAN ATC_WriteNVItem(
                ATC_CONFIG_T        *atc_config_ptr
                );
#endif

/******************************************************************************/
// Description : This function constructs the AT command response.
// Global resource dependence : None
// Author : Ivan.Yin
// Note : None
/******************************************************************************/
extern void ATC_BuildInfoRsp(  // Return S_ATC_SUCCESS if success,
                                      // otherwise return error code.
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                uint8 * rsp_str_ptr // Point the response string
                );

/******************************************************************************/
// Description : This function contructs the result code response 
// Global resource dependence : 
// Author : Ivan.Yin
// Note : Modify by Xueli
/******************************************************************************/
extern void ATC_BuildResultCodeRsp( 
                ATC_CONFIG_T   *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                int32          result_code_idx, // result code index
                int32          err_code_idx     //error code index    
                );


void ATC_InitConfig(
                 ATC_CONFIG_T        *atc_config_ptr  
                 );
                 
#ifdef _MUX_ENABLE_
/******************************************************************************/
// Description : This function called by CMUX to ind ATC that a new char received
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_SendNewATInd(uint8 link_id, uint32 data_length, uint8 *data_ptr);
void ATC_Debug_SendATInd(uint32 len);
#else/*_MUX_ENABLE_*/
/******************************************************************************/
// Description : This function called by SIO to ind ATC that a new AT received
// Global resource dependence : 
// Author : Ivan Ren
// Note : 
/******************************************************************************/
void SendNewATInd(uint32 len);
#endif
#ifdef SYNCTOOL_SUPPORT
//This function is provided by MMI
extern uint32 ATC_ExtCmd(uint8 *cmd, uint32 len);
#endif
ATC_STATUS atcparse(
                uint8 * atcbuf, 
                ATC_CUR_INFO_LIST_T ** info_ptr_ptr
                );
        
#ifndef _ULTRA_LOW_CODE_
/******************************************************************************/
// Description : This function contructs the result code response for SMS commands
// Global resource dependence : 
// Author : Ivan.Yin
// Note : Modified by Xueli 29/09/2002
/******************************************************************************/
extern void ATC_BuildSmsCodeRsp( 
        ATC_CONFIG_T       *atc_config_ptr,  // point to the ATC_CONFIG_T struct.
        int32              err_code_idx      //error code index    
        );      
#endif

#ifdef _ATC_GPRS_MODEM_
/******************************************************************************/ 
//    DESCRIPTION:
//           reset gprs s_dataconf structure function 
//    Global:  
//    AUTHOR: Shijun.cui 2005-08-03
//    Note :  this function is used while active/deactive pdp context operation 
//    History:
/******************************************************************************/ 
extern void ResetGPRSDataConf( 
        void
        );
#endif

/******************************************************************************/
// Description : This function is used to reset lex state while do some unexpect
//               parase.
// Global resource dependence : None
// Author : Shijun.cui
// Note : add for reset lex state during at command parase.
/******************************************************************************/
extern void zerolex(
            void
            );

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function get call state use in CLCC. 
//  Global resource dependence : none
//  Author:       William Qian
//  Note:
/*****************************************************************************/
extern uint8 ATC_GetCallState(                  // return call state
                    MN_DUAL_SYS_E dual_sys,
                    uint8   call_id             // in call_id
                    );
#endif

#ifdef _MUX_ENABLE_
/******************************************************************************/
// Description : This function setup link for one ATC entity
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Mux_Setup_Link(uint8 link_id);
/******************************************************************************/
// Description : This function create link for one ATC entity
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Mux_Release_Link(uint8 link_id, BOOLEAN free_flg);

/******************************************************************************/
// Description : This function set the current link id 
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Set_Current_Link_id(ATC_CONFIG_T *atc_config_ptr,uint8 link_id);

#ifndef _MUX_REDUCED_MODE_
/******************************************************************************/
// Description : This function clear expected result info
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Clear_Exp_Result(ATC_EXP_RESULT_INFO_T  *atc_exp_result_info);
/******************************************************************************/
// Description : This function handle the received the expected event and set the related link id
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
BOOLEAN ATC_Receive_Expected_Event(ATC_CONFIG_T *atc_config_ptr,uint16 expected_event, uint8 id,ATC_EVENT_DOMAIN_T domain_id);
/******************************************************************************/
// Description : This function add the expected event when requested the expected event
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Add_Expected_Event(ATC_CONFIG_T *atc_config_ptr,uint16 expected_event, uint8 id,ATC_EVENT_DOMAIN_T domain_id);

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void ATC_Clear_All_Exp_Result()                                       *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     clear all expected events of call-id(cid)                             *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_Clear_All_Exp_Result(uint8 id, ATC_EVENT_DOMAIN_T domain_id);

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void ATC_List_Expected_Event()                                        *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     list all the expected events                                          *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_List_Expected_Event( void );

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void ATC_Get_Link_Id()                                                *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     get link id on which call-id(cid) is                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     link-id                                                               *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
uint8 ATC_Get_Link_Id(uint8 id, ATC_EVENT_DOMAIN_T domain_id);
#endif

/******************************************************************************/
// Description : This function called by ATC to clear AT buffer
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_ClrCmdLine(uint8 link_id);
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     VOID ATC_ReadCmdLine                                              *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Read AT command from ATC BUFFER                                              *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_ReadCmdLine(
        uint8 link_id,
        uint8 * data_ptr,      // Output argument, point to the buf which is alloced
                               // by caller 
        uint length,           // The length of cmd line which should be read from
                               // sio
        uint * read_length_ptr // The actual read length
        );
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void ATC_SetEcho()                                                *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Tell CMUX to echo the char it receives                                 *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_SetEcho(BOOLEAN IsEcho);
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void ATC_SetCmdLineTerminateChar( )                               *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Set terminate char of of AT command                                   *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_SetCmdLineTerminateChar( 
        uint8 link_id,
        uint8 terminate_char1,   // Default terminate char is '\n' 
        uint8 terminate_char2    // Default terminate char is '\0'
        );
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void ATC_SetCmdLineBackSpaceChar()                                *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Set backspace char of the AT command                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_SetCmdLineBackSpaceChar( 
        uint8 link_id,
        uint8 back_space_char  // Default backspace char value is 8
        );

extern uint8 ATC_GetDualSimMuxLinkId(MN_DUAL_SYS_E sim_id);

/******************************************************************************/
// Description : This function get sim id from link id. 
// Global resource dependence : 
// Author : shaohua xia
// Note :
/******************************************************************************/
extern MN_DUAL_SYS_E ATC_GetSimIdFromLinkId(uint8 link_id);

/******************************************************************************/
// Description : This function get cid from link id. 
// Global resource dependence : 
// Author : shaohua xia
// Note :
/******************************************************************************/
extern uint8 ATC_GetCidFromLinkId(uint8 link_id);

/******************************************************************************/
// Description : This function get link from cid. 
// Global resource dependence : 
// Author : shaohua xia
// Note :
/******************************************************************************/
extern uint8 ATC_GetLinkIdFromCid(uint8 cid);

#endif
/******************************************************************************/
// Description : This function is used to ignore data before 'at'
// Global resource dependence : None
// Author : Pizer.Fan
// Note : ignore data before 'at'
/******************************************************************************/
extern const uint8 *atcparse_at(const uint8 *atcbuf, uint32 len);

extern ATC_GPRS_SETTINGS_E MMIAPICONNECTION_GetGPRSStatus(void);
#endif
