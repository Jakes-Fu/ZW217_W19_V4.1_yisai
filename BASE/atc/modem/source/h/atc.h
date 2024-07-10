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
#ifdef ATC_SAT_ENABLE
#include "atc_sat_def.h"
#else
#include "atc_def.h"
#endif
#include <stdio.h>
#include "nvitem.h"
#ifdef _RTOS
#include "sci_api.h" /*@tony.yao  replase sci_mem.h with sci_api.h*/
#else
#include "tasks_id.h"
#endif
#include "mn_api.h"
#include "simat_api.h"
#include "mn_events.h"
#include "mn_type.h"
#include "nv_item_id.h"
#include "sio.h"
#ifdef _MUX_ENABLE_
#include "mux_api.h"
#endif

#include "sig_code.h"
#include "env_atc_signal.h"

/*******************************************************************************
 **                        AT NV item id define                               *
 ******************************************************************************/
#define NV_ATC_CB_MSG_ID    MN_ATNV_USER_BASE
#define NV_ATC_CB_LANG_ID   (MN_ATNV_USER_BASE+1)
//#define NV_ATC_MIDI_SIZE_ID (MN_ATNV_USER_BASE+2)
#define NV_ATC_CONFIG_ID    (MN_ATNV_USER_BASE+3)
#define NV_SMS_CONFIG_ID    (MN_ATNV_USER_BASE+4)
#define NV_FIRBOOT_CONFIG_ID    (MN_ATNV_USER_BASE+5)
#define NV_CGAUTH_CONFIG_ID    (MN_ATNV_USER_BASE+6)

#define MAX_PAP_USER_LEN 64
#define MAX_PAP_PASSWD_LEN 64
#define MAX_CHAP_CHALLEN_LEN 64
#define MAX_CHAP_CHALNAME_LEN 128

/*******************************************************************************
 **                        AT commands class type                              *
 ******************************************************************************/
#define ATC_CMD_TYPE_BASIC      (0x10)
#define ATC_CMD_TYPE_EXTEND     (0x20)

#define PARA1      cur_cmd_info->cur_param_list_ptr->param_info
#define PARA2      cur_cmd_info->cur_param_list_ptr->next->param_info
#define PARA3      cur_cmd_info->cur_param_list_ptr->next->next->param_info
#define PARA4      cur_cmd_info->cur_param_list_ptr->next->next->next->param_info
#define PARA5      cur_cmd_info->cur_param_list_ptr->next->next->next->next->param_info
#define PARA6      cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->param_info
#define PARA7      cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->param_info
#define PARA8      cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->param_info
#define PARA9      cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->param_info
#define PARA10     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->param_info
#define PARA11     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->param_info
#define PARA12     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->next->param_info
#define PARA13     cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->next->next->param_info


#define PARAM1_TYPE  cur_cmd_info->cur_param_list_ptr->param_type
#define PARAM2_TYPE  cur_cmd_info->cur_param_list_ptr->next->param_type
#define PARAM3_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->param_type
#define PARAM4_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->next->param_type
#define PARAM5_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->next->next->param_type
#define PARAM6_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->param_type
#define PARAM7_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->param_type
#define PARAM8_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->param_type
#define PARAM9_TYPE  cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->param_type
#define PARAM10_TYPE cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->param_type
#define PARAM11_TYPE cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->param_type
#define PARAM12_TYPE cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->next->param_type
#define PARAM13_TYPE cur_cmd_info->cur_param_list_ptr->next->next->next->next->next->next->next->next->next->next->next->next->param_type


#define MAX_ATC_RSP_LEN         2048    //1024  //@MS00149293

#define MAX_SMS_PDU_LEN         360

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
#define ATC_MAX_ACTIVATE_PDP  MN_GPRS_MAX_PDP_CONTEXT_COUNT

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
#define ATC_STKTIME_DEFAULT_VAL       3
#define ATC_SIND_DEFAULT_VAL          1023
#define ATC_SIDETONE_DEFAULT_VAL      0
#define ATC_SPEAKER_DEFAULT_VAL       3
#define ATC_MICRO_DEFAULT_VAL         3

/*******************************************************************************
 **                        AT command line buf length                          *
 ******************************************************************************/
#define ATC_MAX_CMD_LINE_LENGTH       1976//1100    //@MS00150370
#define ATC_MIN_CMD_LINE_LENGTH       8
#define ATC_MAX_CMD_RESPONSE_LENGTH   256
#define ATC_BACKSPACE_CHAR            0x08
#define ATC_MAX_LINK_ID               18 //oms phone require mux link in com_data1 equal 16. so atc link equal 18.
#define ATC_DEBUG_LINK                ATC_MAX_LINK_ID-1
//#define ATC_REC_BUF_SIZE            2600 //already defined in sio.h
#define ATC_MAX_EXP_RESULT            30
#define ATC_INVALID_LINK_ID           0xff
#define ATC_INVALID_CID               0xff
#define ATC_INVALID_NSAPI             0xff
#define ATC_MIN_NSAPI                 5
#define ATC_INVALID_CALL_ID           0xff
#define ATC_NO_EVENT                  0

#ifdef _MUX_ENABLE_
#define ATC_MAX_BUF_NUM         2 //these buffers are created when ATC task initialization, and will not be free
#endif
/*******************************************************************************
 **                       default  ATC config                                  *
 ******************************************************************************/
#define DEF_S0  0       /* default value for S0 */
#define DEF_S2  43      /* default value for S2 */
#define DEF_S3  13      /* default (ASCII 'CR') value for S3 */
#define DEF_S4  10      /* default (ASCII 'LF') value for S4 */
#define DEF_S5  8       /* default (ASCII 'BS' for S5 */
#define DEF_S6  2       /* default value for S6 */
#define DEF_S7  60      /* default value for S7 */
#define DEF_S8  2       /* default value for S8 */
#define DEF_S10 14      /* default value for S10 */

#define DEF_CR  0       /* disable the report */
#define DEF_CRC 0       /* disable the extend +CRING */

#define DEF_CREG   2    /* Disable unsolicited +CREG */
#define DEF_CGREG  2    /* Disable unsolicited +CGREG */
#define DEF_E      1    /* Echo ON */
#define DEF_L      0    /* -14db min speaker volume*/
#define DEF_M      0    /* off during the entire call*/
#define DEF_X      0    /* busy and dial tone off during a call*/
#define DEF_P      0    /* ?*/
#define DEF_Q      0    /* Quiet OFF */
#define DEF_V      1    /* diaplay verbose result code*/

#define DEF_CRSL_VALUE   6
#define DEF_CMOD         0 /* single mode*/

#define DEF_CBST_SPEED   7 /* 9.6k */
#define DEF_CBST_NAME    0 /* data circuit asynchronous */
#define DEF_CBST_CE      0 /* Transparent mode */

#define DEF_COPS_MODE    0 /* Automatic selection */
#define DEF_COPS_FORMAT  2 /* Numeric Format */

#define DEF_CFUN         1 /* Start PS */
#define DEF_TRACE        0 /* AT+TRACE default value */
#define DEF_SPAUTO       0 /* AT+SPAUTO default value */
#define DEF_EARPHONE_VOL 5 /*AT+CLVL default value*/

/*******************************************************************************
 **                        ATC status define                                   *
 ******************************************************************************/
//#define P_ATC       1000    /* ATC Task ID */
#define ATC_STATUS  uint    /* Return Status Type */

#define S_ATC_MODULE            1000

#define S_ATC_SUCCESS           1
#define S_ATC_FAIL              0
#ifdef _SCM21_ATC_DEV
#define S_ATC_SHUT_SUCCESS      2   //for AT+CIPSHUT ,if success,return SHUT OK
#endif 
#define S_ATC_SUCCESS           1
// for AOC feature
#define ATC_AOC_QUERY_MODE      0
#define ATC_AOC_DEACTIVE_MODE   1
#define ATC_AOC_ACTIVE_MODE     2

#define  CTRL_Z                 0x1A   // ^z
#define  ESC                    0x1B   // Esc

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
#ifdef _SCM21_ATC_DEV
#define S_ATC_RESULT_CODE_SHUT_OK 	( S_ATC_RESULT_CODE_BASE + 9 )
#endif
/*******************************************************************************
 **                        ERROR index define                                  *
 ******************************************************************************/
#define ERR_PHONE_FAILURE                                   0
#define ERR_NO_CONNECT_TO_PHONE                             1
#define ERR_PHONE_ADAPTOR_LINK_RESERVED                     2
#define ERR_OPERATION_NOT_ALLOWED                           3
#define ERR_OPERATION_NOT_SUPPORTED                         4
#define ERR_PH_SIM_PIN_REQUIRED                             5
#define ERR_PH_FSIM_PIN_REQUIRED                            6
#define ERR_PH_FSIM_PUK_REQUIRED                            7

#define ERR_SIM_NOT_INSERTED                                10
#define ERR_SIM_PIN_REQUIRED                                11
#define ERR_SIM_PUK_REQUIRED                                12
#define ERR_SIM_FAILURE                                     13
#define ERR_SIM_BUSY                                        14
#define ERR_SIM_WRONG                                       15
#define ERR_INCORRECT_PASSWORD                              16
#define ERR_SIM_PIN2_REQUIRED                               17
#define ERR_SIM_PUK2_REQUIRED                               18
#define ERR_SIM_PUK2_BLOCKED                                19

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
#define ERR_HIDDEN_KEY_REQUIRED                             48 /* NOTE: This key is required
                                                                 when accessing hidden phonebook entries. */

//shijun.cui add error code 2005.01.13
#define ERR_AT_COMMAND_DISCARDED                            60

//shijun.cui add sim error code 2005.02.03
#define ERR_SIM_CARD_REJ_BY_NET                             62
#define ERR_SIM_CARD_SERVICE_NOT_AVAILABLE                  63
#define ERR_SIM_CARD_PIN_UNINITIALIZED                      64
#define ERR_SIM_CARD_PIN_BLOCKED                            65
#define ERR_SIM_CARD_PUK_BLOCKED                            66
#define ERR_SIM_FDN_FAILED                                  70

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
#define ERR_UNSPECIFIED_GPRS_ERROR                          148
#define ERR_PDP_AUTHENTICATION_FAILED                       149
#define ERR_INVALID_MOBILE_CLASS                            150

#define ERR_LEX_LEN_ZERO                                    200       //command line len zero
#define ERR_LEX_NO_AT_PREX                                  201       //wrong command, not start with AT characters
#define ERR_LEX_INVALID_CTRL_Z_COMMAND                      202       //wrong CTRL_Z command

#define ERR_BT_ENTER_EUT_MODE_FAILURE                       210
#define ERR_BT_LEAVE_EUT_MODE_FAILURE                       211

#define ERR_MISSING_OR_UNKOWN_APN                           533


#define ERR_GPS_INVALID_PARAMETER           7101
#define ERR_GPS_NOT_SUPPORT                       7102
#define ERR_GPS_OPERATION_FAILED              7103

//@Shijun.cui add for DIAG test 2005-03-07
#define MN_CAUSE_NO_NETWORK_SERVICE                         171
#define MN_CALL_ERR_EMERGENCY_CALL_ONLY                     172       // the MS only support emergency call
#define MN_CALL_ERR_NORMAL_DISCONNET                        173       // the normal clear procedure 
#define MN_CALL_ERR_REMOTE_USER_DISCONNECT                  174       // the remote user disconnect the connection
#define MN_CALL_ERR_LOW_FAILURE                             175       // the low failure(e.g RR connection fail)
#define MN_CALL_ERR_NETWORK_REJECT                          176       // the network reject 
#define MN_CALL_ERR_NO_CELL                                 177       // the MS do not camp on a cell



#define ATC_CPAS_STATUS_READY          0X00
#define ATC_CPAS_STATUS_RING           0X03
#define ATC_CPAS_STATUS_PROGRESS       0X04

#define ATC_URC_ALL_STATUS_OFF          0
#define ATC_URC_CALL_SMS_STATUS_ON      1
#define ATC_URC_ALL_STATUS_ON           2

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

#define ATC_ESQOPT_TIMER_VALUE     100         //the value of the timer to wait for the response 
#define ATC_ESQOPT_TIMER_UNIT      100         //unit of the timer (100ms)

#define MAX_DUMMY_KPD_RSP_LEN      256       //max rsp info length

/*******************************************************************************
 **                        SET CURRENT LINK ID         RECEIVE  EXPECTED  EVENT                          *
 ******************************************************************************/
#ifdef _MUX_ENABLE_
#define ATC_SET_CURRENT_LINK_ID_MUX(CURRENT_LINK_ID) \
    ATC_Set_Current_Link_id(ATC_GetConfigInfoPtr(), CURRENT_LINK_ID);

#define ATC_RECEIVE_EXPECTED_EVENT_MUX(RESULT, EXP_EVENT, ID, DOMAIN_ID, SIM_ID)\
    RESULT = ATC_Receive_Expected_Event(S_CONFIG_INFO_PTR, EXP_EVENT, ID, DOMAIN_ID, SIM_ID);

#define ATC_ADD_EXPECTED_EVENT_MUX(CONFIG_INFO_PTR, EXP_EVENT, ID, DOMAIN_ID, SIM_ID)\
    ATC_Add_Expected_Event(CONFIG_INFO_PTR, EXP_EVENT, ID, DOMAIN_ID, SIM_ID);

#define ATC_SET_CURRENT_CARD_ID_MUX(MSG_TYPE)\
    if(g_mux_sys_info[MUX_1].is_start !=MUX_STATUS_OFF)\
    {\
        g_current_card_id = ATC_GetSimIdFromLinkId(((MSG_TYPE *)sig_ptr)->link_id);\
    }

#define ATC_GET_MUX_LINK_ID(LINK_ID, DUAL_SYS)\
    LINK_ID = ATC_GetMuxLinkIdFromSimId(DUAL_SYS);

#define ATC_CURRENT_SIM_ID_MUX  ATC_GetSimIdFromLinkId((ATC_GetConfigInfoPtr())->current_link_id)

#define ATC_LINK_ID_TRACE(F_STRING, LINK_ID)   ATC_TRACE_LOW(F_STRING, LINK_ID)
#else
#define ATC_SET_CURRENT_LINK_ID_MUX(CURRENT_LINK_ID)
#define ATC_RECEIVE_EXPECTED_EVENT_MUX(RESULT, EXP_EVENT, ID, DOMAIN_ID, SIM_ID)
#define ATC_ADD_EXPECTED_EVENT_MUX(CONFIG_INFO_PTR, EXP_EVENT, ID, DOMAIN_ID, SIM_ID)
#define ATC_SET_CURRENT_CARD_ID_MUX(MSG_TYPE)
#define ATC_GET_MUX_LINK_ID(LINK_ID, DUAL_SYS)
#define ATC_CURRENT_SIM_ID_MUX  g_current_card_id
#define ATC_LINK_ID_TRACE(F_STRING, LINK_ID)
#endif

/*******************************************************************************
 **                                                              CCBS                                                                                *
 ******************************************************************************/
#define ATC_MAX_CCBS_NUM       5

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
    ATC_CCBS_REGISTER = 5,       //The user has accept the ccbs ind
    ATC_REL_EXISTING_CALLS = 6,            // Releases all existing calls
    ATC_HOLD_ACTIVE_CALL_ONLY = 7,    // hold active call only
    ATC_REL_ACTIVE_CALLS_AND_ACCEPT_HELD_CALL = 8,    //Releases all active calls and accepts the held calls without caring waiting call
    ATC_REL_SPECIFIC_X_CALL_BASE = 10,       // Releases the specific active call X
    ATC_HOLD_CALLS_EXCEPT_X_BASE = 20,       // Places all active calls, except call X, on hold
    ATC_REL_SPECIFIC_EXISTING_X_CALL_BASE = 70,       // Releases the specific existing call X
    ATC_INVALID_OPERATE
} ATC_CHLD_OPERATION_E;

typedef enum
{
    ANS_OFF_AUTO_GPRS_MODE,
    ANS_ON_AUTO_GPRS_MODE,
    ANS_MODEM_GPRS_MODE,
    ANS_MODEM_GPRS_GSM_MODE
} ATC_ANS_MODE_T;

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
    ATC_CTRLZ_SMS,
    ATC_CTRLZ_SRWD,
    ATC_CTRLZ_STK,
    ATC_CTRLZ_SSEA,
    ATC_CTRLZ_GPRS,
    ATC_CTRLZ_SNVM,
    ATC_CTRLZ_SADM,
    ATC_CTRLZ_SADMNV,
    ATC_CTRLZ_SMS_STATUS,
    ATC_CTRLZ_SADM_ARM,
    ATC_CTRLZ_SADMNV_ARM,
    ATC_CTRLZ_SADMDSP,
    ATC_CTRLZ_SADMNV_DSP,
#ifndef ATC_SAT_ENABLE    
    ATC_CTRLZ_ENHA,
    ATC_CTRLZ_ENHA_TUN,
    ATC_CTRLZ_ENHANV,
    ATC_CTRLZ_ENHANV_TUN,
#endif
    ATC_CTRLZ_LVVE,
    ATC_CTRLZ_LVVENV,
    ATC_CTRLZ_MAX
} ATC_CTRLZ_STATUS_E;

typedef struct
{
    const char *str;
} ATC_RSP_STR_T;

typedef enum
{
    CELL_STATE_NULL = 0,
    CELL_STATE_GSM,
    CELL_STATE_GPRS,
    CELL_STATE_EDGE,
    CELL_STATE_WCDMA,
    CELL_STATE_HSDPA,
    CELL_STATE_HSUPA,
    CELL_STATE_HS_PA,
    CELL_STATE_TD,

    CELL_STATE_MAX
} CELL_State_E;

/*the type for AT+SPCEER command*/
typedef enum
{
    CEER_CALL_DISCONNECT = 0,               /*0:the last call disconnect*/
    CEER_CALL_MODIFICATION,                 /*1:the last call modification*/
    CEER_PDP_ACTIVATE,                           /*2:the last PDP activate*/
    CEER_PDP_DEACTIVATE,                       /*3:the last PDP deactivate*/
    CEER_GPRS_ATTACH,                            /*4:the last GPRS attach*/
    CEER_GPRS_DETACH,                           /*5:the last GPRS detach*/
    CEER_WAP_ATTACH,                            /*6:the last wap attach*/
    CEER_MAX_NUM
} AT_SPCEER_TYPE_E;

typedef enum
{
    ATC_CHSET_IRA = 0,    /*0: International reference alphabet*/
    ATC_CHSET_GSM,        /*1: GSM 7 bit default alphabet */
    ATC_CHSET_HEX,        /*2: Character strings consist only of hexadecimal numbers from 00 to FF*/
    ATC_CHSET_UCS2,       /*3: 16-bit universal multiple-octet coded character set*/
    ATC_CHSET_MAX_NUM
} ATC_CHARACTER_SET_TYPE_E;


typedef enum
{
    ATC_CARD_TYPE_SIM  = 0,
    ATC_CARD_TYPE_USIM = 1, //Dual sim card treat as usim
    ATC_CARD_TYPE_UIM  = 2, //not used now
    ATC_CARD_TYPE_MAX
} ATC_SIM_TYPE_E;


typedef enum
{
    ATC_AUDIO_REC_STOP,
    ATC_AUDIO_REC_START,
    ATC_AUDIO_BT_REC_START    
} ATC_AUDIO_REC_E;

typedef enum
{
    LOCATION_UPDATE_FAIL_CLASS = 0,
    GPRS_ATTACH_FAIL_CLASS     = 1,
    RAU_COMRAU_FAIL_CLASS      = 2,
    PDP_ACTIVATION_REJECT_CLASS= 3,
    SS_SERVICE_REJECT_CLASS    = 5,
    RADIO_LINK_FAIL_CLASS      = 6,
    MM_AUTH_FAIL_CLASS         = 7,
    PS_SECURITY_FAIL_CLASS     = 8,
    NW_CS_AUTH_REJECT_CLASS    = 9,
    NW_PS_AUTH_REJECT_CLASS    = 10,
    SMS_REJECT_CLASS           = 11,
    SMS_REJECT_CP_CLASS        = 12,
    SMS_REJECT_RP_CLASS        = 13,
}ATC_ERROR_ITEM_CLASS;

/*******************************************************************************
 **                         Structure declaration                              *
 ******************************************************************************/
typedef struct atc_param_numeric_tag
{
    const int32     val_min;  /* Minimum value of numeric parameter */
    const int32     val_max;  /* Maximum value of numeric parameter */
} ATC_PARAM_NUMERIC_T;

typedef struct atc_param_string_tag
{
    const uint     str_len;

    const uint8     *str_ptr;
} ATC_PARAM_STRING_T;

typedef struct atc_param_info_tag
{
    const uint16     param_type; /* Parameter type: 1 - numeric
                                                    2 - string   */
    const uint16     param_range_num;

    union
    {
        const ATC_PARAM_STRING_T   *str_array_ptr;  /* Point to an array of
                                                       string parameter
                                                       value */
        const ATC_PARAM_NUMERIC_T *num_array_ptr;  /* Point to an array of
                                                       numeric parameter
                                                       value range */
    } param_info;
} ATC_PARAM_INFO_T;

typedef struct atc_info_tag
{
    const uint16     cmd_index;  /* index of command */
    const uint16     cmd_type;   /* command type: 1 -- execute
                                                  2 -- set
                                                  4 -- read
                                                  8 -- test    */

    const ATC_PARAM_STRING_T *cmd_name;    /* command name */
    const uint               param_num;  /* number of parameters */
    const ATC_PARAM_INFO_T *param_ptr;  /* point to an array of parameter
                                            infomation */
} ATC_INFO_T;

typedef struct atc_cur_param_string_tag
{
    uint       str_len;

    uint8     *str_ptr;
} ATC_CUR_PARAM_STRING_T;

typedef struct atc_cur_param_info_list_tag
{
    uint16     param_type; /* Parameter type: 1 - numeric
                                              2 - string
                                              3 - default  */
    uint16     padding;

    union
    {
        int32                num;  /* numeric parameter value */
        ATC_CUR_PARAM_STRING_T *str_ptr;  /* point to the address of the
                                          parameter string */
    } param_info;

    struct atc_cur_param_info_list_tag *next;
} ATC_CUR_PARAM_INFO_LIST_T;

typedef struct atc_cur_info_list_tag
{
    uint    cmd_index;  /* index of the current command */
    uint    cmd_type;   /* command type: 1 -- execute
                                         2 -- set
                                         4 -- read
                                         8 -- test      */
    uint    cur_param_num;

    BOOLEAN param_used[ATC_MAX_PARAM_NUM];

    ATC_CUR_PARAM_INFO_LIST_T    *cur_param_list_ptr;

    struct atc_cur_info_list_tag *next;
} ATC_CUR_INFO_LIST_T;

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
    USER_CONTROLLED_PLMN           = 0,     // User controlled PLMN selector with Access Technology EFPLMNwAcT
    OPERATOR_CONTROLLED_PLMN       = 1,     // Operator controlled PLMN selector with Access Technology EFOPLMNwAcT
    HPLMN                          = 2      // HPLMN selector with Access Technology EFHPLMNwAcT
} PLMN_LIST_TYPE_E;

typedef enum
{
    NOT_REGISTERED_NOT_SEARCHING,   // Not registered The ME is currently not searching for a new operator to register to
    REGISTERED_HOME_PLMN,           // Registered; home network
    NOT_REGISTERED_AND_SEARCHING,   // Not registered The ME is currently searching for a new operator to register to
    REGISTRATION_DENIED,            // Registration denied
    UNKNOWN,                        // Unknown
    REGISTERED_ROAMING,              // Registered; roaming
    REGISTERED_EMERGENCE = 8         //emergence according to 3GPP Release 10
} OPER_STATUS_E;

typedef enum
{
    PLMN_FORMAT_FULL_NAME = 0x00,
    PLMN_FORMAT_SHORT_NAME,
    PLMN_FORMAT_NUMBERIC
} ATC_PLMN_FORMAT_E;

typedef struct
{
    PLMN_SELECTION_MODE_E  mode;
    ATC_PLMN_FORMAT_E    format;
} ATC_COPS_T;

typedef enum
{
    ATC_ESQOPT_TIMER_INTERVAL       = 0,    // indicate according to timer interval (100ms unit)<option>
    ATC_ESQOPT_RSSI_THRESHOLD       = 1,    // indicate according to RSSI threshold (1dB unit)<option>
    ATC_ESQOPT_STOP_INDICATION      = 2     // stop indication <option>
} ATC_ESQOPT_OPTION_TYPE_E;

typedef struct
{
    ATC_ESQOPT_OPTION_TYPE_E   option;/*0:timer interval 1:RSSI threshold 2:stop*/
    uint32                     thres_value;
    uint32                     timer_value;
} ATC_ESQOPT_T;

typedef struct
{
    uint8   speed;
    uint8   name;   // sync or unsync / circuit or packet
    uint8   ce;     // connection element, transparent mode or none transparent
} ATC_CBST_T;

typedef enum
{
    ATC_PIN_NO_OPERATE,
    ATC_PIN_RESET_ACM_UNIT,
    ATC_PIN_SET_ACM_MAX,
    ATC_PIN_SET_PPU,
    ATC_PIN_GET_PPU,
    ATC_PIN_ENABLE_FDN,
    ATC_PIN_DISABLE_FDN
} ATC_PIN_OPERATE_E;
typedef enum
{
    ATC_CCED_SCELL = 1,
    ATC_CCED_NCELL = 2,
    ATC_CCED_TIME_ADV = 4,
    ATC_CCED_INVALID,
    ATC_CCED_CSQ = 8
} ATC_CCED_TYPE_E;

typedef enum
{
    ATC_SYNCHRONIZESERVICE_STAT_NULL = 0x00,
    ATC_SYNCHRONIZESERVICE_STAT_CFU,
    ATC_SYNCHRONIZESERVICE_STAT_CFB,
    ATC_SYNCHRONIZESERVICE_STAT_CFNRY,
    ATC_SYNCHRONIZESERVICE_STAT_CFNRC,
    ATC_SYNCHRONIZESERVICE_STAT_FINISHED
} ATC_SYNCHRONIZESERVICE_STAT_E;

typedef struct
{
    ATC_SYNCHRONIZESERVICE_STAT_E status;

    uint8                         cfu;
    uint8                         cfb;
    uint8                         cfnry;
    uint8                         cfnrc;
    uint8                         cfcond;
    uint8                         cfall;

} ATC_SYNCHRONIZESERVICE_CNTXT_T;

typedef struct
{
    MN_GPRS_EVENT_REPORT_MODE_E   mode;
    MN_GPRS_EVENT_REPORT_BUFFER_E bfr;
} ATC_CGEREP_SETTING_T;

typedef enum _ATC_PHONE_SERVICE_STATUS_E
{
    ATC_PHONE_NO_SERVICE = 0,
    ATC_PHONE_LIMITED_SERVICE = 0x01,
    ATC_PHONE_FULL_SERVICE = 0x02
}ATC_PHONE_SERVICE_STATUS_E;

typedef enum _ATC_PHONE_RAT_E
{
    ATC_PHONE_RAT_NONE = 0x00,
    ATC_PHONE_RAT_RESERVE = 0x01,	
    ATC_PHONE_RAT_GSM = 0x03,
    //ATC_PHONE_RAT_TD = 0x04,
    //ATC_PHONE_RAT_WCDMA = 0x05,
    //ATC_PHONE_RAT_TD = 0x0F
}ATC_PHONE_RAT_E;

typedef enum _ATC_PHONE_SIM_STATE_E
{
    ATC_PHONE_SIM_INVALID,
    ATC_PHONE_SIM_VALID,
    ATC_PHONE_SIM_UNKNOW = 255
}ATC_PHONE_SIM_STATE_E;




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
    uint8       spauto_flag; //bit1: auto answer, bit2: sim1 gprs auto attach, bit3:android iq mode, bit4: sim1 auto power on
    uint8       earphone_vol;

} ATC_CONFIG_NV_T;

typedef enum
{
    ATC_PSD_STATE_INACTIVE = 0x00,
    ATC_PSD_STATE_ACTIVATE_PENDING,
    ATC_PSD_STATE_ACTIVE,
    ATC_PSD_STATE_MODIFY_PENDING,
    ATC_PSD_STATE_DEACTIVATE_PENDING
} ATC_PSD_STATE_E;

/*+CR162235 Additional Modification*/
typedef enum
{
    INIT_UNKNOW = 0,
    INIT_BY_ATC,
    INIT_BY_MMI
} ATC_INIT_MOD_STATE_E;

typedef struct atc_psd_call_info_tag
{
    uint8            link_id;       /* link id PSD call worked */
    uint8            data_link_id; /* link id PSD call worked */
    uint8            cid;           /* cid */
    BOOLEAN          data_mode_req; /* ppp dial request or not */
    BOOLEAN          data_mode;     /* ppp dial or not */
    uint8            nsapi;         /* nsapi of psd call */
    ATC_PSD_STATE_E  state;         /* ONLY apply as reference!!! */
    ATC_INIT_MOD_STATE_E    init_mod_state; /*init by which module*/
} ATC_PSD_CALL_INFO_T;

typedef struct
{
    uint8 cur_call_id;
    uint8 cur_ccbs_index;
}ATC_CCBS_INFO_T;

#ifdef _MUX_ENABLE_
#define ATC_PDP_NUM_MAX 3   // 2 increase to 3
#define ATC_LINK_NUM_PER_SIM  3

typedef enum
{
ATC_MUX_SINGLE_MAP = 0,        //µ¥¿¨Ó³Éä
ATC_MUX_MULTI_MAP,           // ¶à¿¨Ó³Éä
ATC_MUX_FREE_MAP         // ×ÔÓÉÓ³Éä
}ATC_MUX_MOD_E;

typedef struct atc_link_config_tag
{
    BOOLEAN          is_ctrlz_end;
    uint8                ctrlz_end_status;
} ATC_LINK_CONFIG_T;

typedef enum
{
    ATC_DOMAIN_CS = 0,
    ATC_DOMAIN_PS,
    ATC_DOMAIN_PHONEBOOK,
    ATC_NO_DOMAIN
} ATC_EVENT_DOMAIN_T;

typedef struct
{
    uint8          link_id;
    uint8          cid;
    uint8          nsapi;
    uint16        call_id; //verify identity for call,phonebook,sms... cnf message
    uint16        exp_event;
    uint16        ext_exp_event;
    MN_DUAL_SYS_E        dual_sys;
} ATC_EXP_RESULT_INFO_T;

typedef struct
{
    ATC_EXP_RESULT_INFO_T  atc_exp_result_info[ATC_MAX_EXP_RESULT];
    uint16  start_point;
    uint16  end_point;
} ATC_EXP_LINK_INFO_T;

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

    uint8       cgreg;          //an unsolicited result code +CGREG(GPRS)

    uint8       cmod;         //call mode
    uint8       vtd_value;    //during time of DTMF tone


    BOOLEAN            cssi_flag[MN_SYS_NUMBER];        /* TRUE: Enable the +CSSI result code
                                         ** FALSE: Disable the +CSSU result code
                                         */
    BOOLEAN            cssu_flag[MN_SYS_NUMBER];        /* TRUE: Enable the +CSSU result code
                                         ** FALSE: Disable the +CSSU result code
                                         */
    BOOLEAN            ccwa_flag[MN_SYS_NUMBER];        /* TRUE: Enable the unsolicited result code +CCWA
                                         ** FALSE: Disable the unsolicited result code +CCWA
                                         */
    BOOLEAN            cmut_flag;        /* TRUE: mute on
                                         ** FALSE; mute off
                                         */
    BOOLEAN            sdmut_flag;        /* TRUE: mute on in downlink
                                         ** FALSE; mute off in downlink
                                         */

    BOOLEAN            crsl_value;

    MN_CARD_TYPE_E     sim_type[MN_SYS_NUMBER];  // 1:SIM 2:USIM 0xff:unkonw

    BOOLEAN            cpin_flag[MN_SYS_NUMBER];
    BOOLEAN            pin1_enable_flag[MN_SYS_NUMBER];
    uint8              cfun_flag[MN_SYS_NUMBER];
    uint8              cscs[MN_SYS_NUMBER];         //selected character set
    uint8              aoc_mode;         /* 0: Query CCM value.
                                         ** 1: Deactivate the unsolicited reporting of CCM value
                                         ** 2: Activate the unsolicited reporting of CCM value
                                         */
    uint32             ccm_val;
    uint8              cicb_val;         /* AT+CICB=<mode> 0:data call
                                         ** 1: fax call 2: speech call
                                         */

    uint8              ath_flag;
    BOOLEAN            sim_flag[MN_SYS_NUMBER];         //This flag indicates if the sim is ok or not
    BOOLEAN            sim_ready[MN_SYS_NUMBER];
    MNSIM_ERROR_E      sim_err_reason[MN_SYS_NUMBER];
    BOOLEAN           is_sim_init[MN_SYS_NUMBER];//Is sim in init procedure? From sfun=2 to SIM_READY_IND/NOT_READY_IND, this value is TRUE.

    BOOLEAN            smscb_flag[MN_SYS_NUMBER];
    BOOLEAN            smscb_mode[MN_SYS_NUMBER];

    BOOLEAN            audio_flag;
    BOOLEAN            call_read_flag[MN_SYS_NUMBER];
    uint8              idle_rxlev[MN_SYS_NUMBER];

    BOOLEAN            csq_flag[MN_SYS_NUMBER];         // auto indicate the rssi
    ATC_CCED_TYPE_E    cced_type[MN_SYS_NUMBER];        // scell ncell timeadvance
    BOOLEAN            cmer_flag[MN_SYS_NUMBER];
    uint32             ceer_val;         //error type
    ATC_CBST_T         cbst;
    ATC_COPS_T         cops[MN_SYS_NUMBER];               //net registersation  mode
    uint8              cpas;               //state of DCE
    uint8              cpin[MN_SYS_NUMBER];
    uint8              cpin2[MN_SYS_NUMBER];
    uint8              midi_oper;
    uint16             midi_block;
    uint8              audio_mode;
    uint8              speaker_volume;    // speaker volume
    uint8              ssea_para;
    uint16             ssea_vol_up;
    uint16             ssea_vol_down;
    BOOLEAN            dsci_flag;
    /*
    **  The following for AOC feature
    */
    ATC_PIN_OPERATE_E     operate[MN_SYS_NUMBER];
    uint32                acm_max[MN_SYS_NUMBER];
    MNSIM_PUCT_INFO_T     ppc[MN_SYS_NUMBER];        //the price per unit.
    uint8                 ms_class;  //the MS class
    BOOLEAN               pdp_type_connect;
    ATC_CHLD_OPERATION_E  ss_operate[MN_SYS_NUMBER];
    BOOLEAN               plmn_search_flag;

    //This is about the item which store in NV.
    ATC_CONFIG_NV_T                 atc_config_nv;
    BOOLEAN                         is_fdn_enable[MN_SYS_NUMBER];
    BOOLEAN                         is_fdn_init[MN_SYS_NUMBER];
    BOOLEAN                         is_fdn_ready[MN_SYS_NUMBER];
    uint8                           c_activate_pdp;
    uint8                           activate_pdp_id[ATC_MAX_ACTIVATE_PDP];
    uint8                           current_link_id;
    ATC_EXP_LINK_INFO_T             atc_exp_link_info;
    ATC_LINK_CONFIG_T               atc_link_config[ATC_MAX_LINK_ID];
    uint8                           trace_value;    //AT+TRACE
    ATC_PSD_CALL_INFO_T             atc_psd_call_info[ATC_MAX_ACTIVATE_PDP];
    BOOLEAN                         reset_flag[MN_SYS_NUMBER];        /* TRUE: AT+RESET flag */
    ATC_SYNCHRONIZESERVICE_CNTXT_T  atc_sync_srv;
    ATC_CGEREP_SETTING_T            cgerep_value;
    ATC_ESQOPT_T                    esqopt_param;
    SCI_TIMER_PTR                   esqopt_timer;
    BOOLEAN                         ccwe_flag;        /* TRUE: AT+CCWE flag */
    uint8                           call_link_id[MN_CALL_MAX_CALL_NUM];
    BOOLEAN                         psntrg_flag;   //for AT*PSNTRG
    uint8                           rel_former[MN_SYS_NUMBER];
    uint32                          ceer_code[CEER_MAX_NUM];     /*error cause array*/
    uint8                           ceer_last_type;   /*the last error cause type*/
    BOOLEAN                         pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT];
    BOOLEAN                         tz_report_flag;
    BOOLEAN                         tz_update_flag;
    uint8                           cfun_status[MN_SYS_NUMBER];
    uint8                           urc_level;
    BOOLEAN                         usb_flag;
    ATC_CCBS_INFO_T                 ccbs_info[MN_SYS_NUMBER];
    uint8                           audio_rec;
    BOOLEAN                         edge_support; //indicator for mobile itself supporting edge
    BOOLEAN                         async_mode_flag;
    uint8                           spreadyflag;
    BOOLEAN                         sms_is_ready[MN_SYS_NUMBER];
#ifdef _SCM21_ATC_DEV
	BOOLEAN							is_echo_shut_ok;
	BOOLEAN							not_echo_ok_when_success;
	/*used for AT+CIPMUX:setting nums of allowed ip connection. 0--single ip connect; 1-- multi ip connection */
	uint8							multi_ip_connect;
#endif //_SCM21_ATC_DEV
} ATC_CONFIG_T;
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
    uint8       x_value;      /* Result code selection and call progress monitoring control */
    uint8       l_value;      //Monitor Speaker Loudness
    uint8       p_value;      //Select Pulse Dialling

    uint8       cr;           //display the intermediate bearer capability report
    uint8       crc;          //extended +CRING

    uint8       cgreg;          //an unsolicited result code +CGREG(GPRS)

    uint8       cmod;         //call mode
    uint8       vtd_value;    //during time of DTMF tone

    BOOLEAN            cssi_flag[MN_SYS_NUMBER];   /* TRUE: Enable the +CSSI result code
                                                   ** FALSE: Disable the +CSSU result code
                                                   */
    BOOLEAN            cssu_flag[MN_SYS_NUMBER];   /* TRUE: Enable the +CSSU result code
                                                   ** FALSE: Disable the +CSSU result code
                                                   */
    BOOLEAN            ccwa_flag[MN_SYS_NUMBER];   /* TRUE: Enable the unsolicited result code +CCWA
                                                   ** FALSE: Disable the unsolicited result code +CCWA
                                                   */
    BOOLEAN            cmut_flag;                  /* TRUE: mute on
                                                   ** FALSE; mute off
                                                   */
    BOOLEAN            sdmut_flag;                 /* TRUE: mute on in downlink
                                                   ** FALSE; mute off in downlink
                                                   */
    BOOLEAN            crsl_value;
    MN_CARD_TYPE_E     sim_type[MN_SYS_NUMBER];  // 1:SIM 2:USIM 0xff:unkonw
    BOOLEAN            cpin_flag[MN_SYS_NUMBER];
    BOOLEAN            pin1_enable_flag[MN_SYS_NUMBER];
    uint8              cfun_flag[MN_SYS_NUMBER];
    uint8              cscs[MN_SYS_NUMBER];         //selected character set
    uint8              aoc_mode;         /* 0: Query CCM value.
                                            ** 1: Deactivate the unsolicited reporting of CCM value
                                            ** 2: Activate the unsolicited reporting of CCM value
                                            */
    uint32             ccm_val;
    uint8              cicb_val;         /* AT+CICB=<mode> 0:data call
                                            ** 1: fax call 2: speech call
                                            */
    uint8              ath_flag;
    BOOLEAN            sim_flag[MN_SYS_NUMBER];         /* This flag indicates if the sim is ok or not*/
    BOOLEAN            sim_ready[MN_SYS_NUMBER];
    MNSIM_ERROR_E      sim_err_reason[MN_SYS_NUMBER];
    BOOLEAN           is_sim_init[MN_SYS_NUMBER]; //Is sim in init procedure? From sfun=2 to SIM_READY_IND/NOT_READY_IND, this value is TRUE.
    BOOLEAN            smscb_flag[MN_SYS_NUMBER];
    BOOLEAN            smscb_mode[MN_SYS_NUMBER];
    BOOLEAN            audio_flag;
    BOOLEAN            call_read_flag[MN_SYS_NUMBER];
    uint8              idle_rxlev[MN_SYS_NUMBER];
    BOOLEAN            csq_flag[MN_SYS_NUMBER];         // auto indicate the rssi
    ATC_CCED_TYPE_E    cced_type[MN_SYS_NUMBER];        // scell ncell timeadvance
    BOOLEAN            cmer_flag[MN_SYS_NUMBER];
    uint32             ceer_val;         //error type
    ATC_CBST_T         cbst;
    ATC_COPS_T         cops[MN_SYS_NUMBER];   //net registersation  mode
    uint8              cpas;                  //state of DCE
    uint8              cpin[MN_SYS_NUMBER];
    uint8              cpin2[MN_SYS_NUMBER];
    uint8              midi_oper;
    uint16             midi_block;
    uint8              audio_mode;
    uint8              speaker_volume;    // speaker volume
    uint8              ssea_para;
    uint16             ssea_vol_up;
    uint16             ssea_vol_down;
    BOOLEAN            dsci_flag;
    /*
    **  The following for AOC feature
    */
    ATC_PIN_OPERATE_E     operate[MN_SYS_NUMBER];
    uint32                acm_max[MN_SYS_NUMBER];
    MNSIM_PUCT_INFO_T     ppc[MN_SYS_NUMBER];        //the price per unit.
    BOOLEAN               is_ctrlz_end;
    uint8                 ctrlz_end_status;
    uint8                 ms_class;  //the MS class
    BOOLEAN               pdp_type_connect;
    ATC_CHLD_OPERATION_E  ss_operate[MN_SYS_NUMBER];
    BOOLEAN               plmn_search_flag;

    //This is about the item which store in NV.
    ATC_CONFIG_NV_T                 atc_config_nv;
    BOOLEAN                         is_fdn_enable[MN_SYS_NUMBER];
    BOOLEAN                         is_fdn_init[MN_SYS_NUMBER];
    BOOLEAN                         is_fdn_ready[MN_SYS_NUMBER];
    uint8                           trace_value;    //AT+TRACE
    ATC_PSD_CALL_INFO_T             atc_psd_call_info[ATC_MAX_ACTIVATE_PDP];
    BOOLEAN                         reset_flag[MN_SYS_NUMBER];        /* TRUE: AT+RESET flag */
    ATC_SYNCHRONIZESERVICE_CNTXT_T  atc_sync_srv;
    ATC_CGEREP_SETTING_T            cgerep_value;
    ATC_ESQOPT_T                    esqopt_param;
    SCI_TIMER_PTR                   esqopt_timer;
    BOOLEAN                         ccwe_flag;        /* TRUE: AT+CCWE flag */
    BOOLEAN                         psntrg_flag;
    uint8                           rel_former[MN_SYS_NUMBER];
    uint32                          ceer_code[CEER_MAX_NUM];     /*error cause array*/
    uint8                           ceer_last_type;           /*the last error cause type*/
    BOOLEAN                         pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT];
    BOOLEAN                         tz_report_flag;
    BOOLEAN                         tz_update_flag;
    uint8                           cfun_status[MN_SYS_NUMBER];
    uint8                           urc_level;
    BOOLEAN                         usb_flag;
    ATC_CCBS_INFO_T                 ccbs_info[MN_SYS_NUMBER];
    uint8                           audio_rec;
    BOOLEAN                         edge_support; //indicator for mobile itself supporting edge
    BOOLEAN                         async_mode_flag;
    uint8                           spreadyflag;
    BOOLEAN                         sms_is_ready[MN_SYS_NUMBER];
#ifdef _SCM21_ATC_DEV
	BOOLEAN							is_echo_shut_ok;
	BOOLEAN							not_echo_ok_when_success;
	uint8							multi_ip_connect;
#endif //_SCM21_ATC_DEV
} ATC_CONFIG_T;
#endif
typedef enum
{
    UNKNOWN_PLAN_UNKNOWN_TYPE         = 128, // Unknown numbering plan  unknown
    ISDN_TELE_PLAN_UNKNOWN_TYPE       = 129, // ISDN/telephony number plan, //unknow number
    ISDN_TELE_PLAN_INTERNATIONAL_TYPE = 145, // ISDN/telephony number plan, // international number
    ISDN_TELE_PLAN_NATIONAL_TYPE      = 161 // ISDN/telephony number plan // national number
} ATC_TELE_TYPE_E;

typedef struct ATC_EMG_NUM_LIST_IND_T_s
{
    uint16          mcc;
    uint8           list_num;
    MN_EMG_NUM_T    emg_num_list[MN_EMG_NUM_LIST_MAX_NUM];
} ATC_EMG_NUM_LIST_IND_T;

typedef enum
{
    CICB_DATA_CALL,
    CICB_FAX_CALL,
    CICB_SPEECH_CALL
} ATC_CICB_E;       //incoming call type

typedef ATC_STATUS(* ATC_Func_Handle_F)(
    ATC_CONFIG_T * ,
    ATC_CUR_INFO_LIST_T *);

#ifdef _MUX_ENABLE_
/*
    Software structures
*/
typedef struct
{
    uint32        size;              // Total Size of the buffer.
    volatile int  start_point;       // First data in the buffer.
    volatile int  end_point;         // Last data in the buffer.
    BOOLEAN       is_free;           // indicate the buffer occupied or not
    uint8        *atc_buf_ptr;       // atc base buffer address.
} ATC_BUF_T;

typedef struct
{
    uint8        buf_index;     //base buffer index when it complete, the dynamic buffer should be released
    BOOLEAN      is_dyn_buf;    //if base buffer not available, then allocate a new dynamic buffer
    ATC_BUF_T   *link_buf_ptr;  //point to a free base buffer
} ATC_LINK_BUF_T;

#ifndef HUAWEI_6500_MODEM
typedef struct atc_link_info_tag
{
    uint8       cmd_line_buf_long[ATC_MAX_CMD_LINE_LENGTH];
    uint8       cmd_line_buf_short[ATC_MIN_CMD_LINE_LENGTH+ 2];

    uint8        *cur_cmd_line_buf_ptr;
    uint8        *last_cmd_line_buf_ptr;

    ATC_CUR_INFO_LIST_T    *atc_info_ptr; // store the infos of AT cmds we
    // want to handle.
} ATC_LINK_INFO_T;
#else
typedef struct atc_link_info_tag
{
    uint8       cmd_line_buf_long[ATC_MAX_CMD_LINE_LENGTH];
    uint8       cmd_line_buf_short[ATC_MIN_CMD_LINE_LENGTH+ 12];

    uint8        *cur_cmd_line_buf_ptr;
    uint8        *last_cmd_line_buf_ptr;

    ATC_CUR_INFO_LIST_T    *atc_info_ptr; // store the infos of AT cmds we
    // want to handle.
} ATC_LINK_INFO_T;
#endif

typedef struct atc_global_info_tag
{
    /*----------------------------------------------------------------------------*/
    // We define two AT Cmd line buf :
    //     the long buf is used to store normal ATC line
    //     the short buf is used to store ATC line which begins with "A/"
    /*----------------------------------------------------------------------------*/
    uint16      buffered_at_cmd_count; //@Ivan.Yin for avoiding task queue full 2004-03-22

    ATC_LINK_INFO_T     atc_link_info;

    const ATC_INFO_T       *atc_info_table;     //command info table
    ATC_Func_Handle_F      *atc_handle_table;   //command process ptr table

    ATC_CONFIG_T           *atc_config_ptr;

} ATC_GLOBAL_INFO_T;
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

    uint8        *cur_cmd_line_buf_ptr;
    uint8        *last_cmd_line_buf_ptr;

    /*----------------------------------------------------------------------------*/
    // The response of AT Commands are first store in cmd_response_buf,
    // then we send it to SIO.
    /*----------------------------------------------------------------------------*/

    const ATC_INFO_T       *atc_info_table;     //command info table
    ATC_Func_Handle_F      *atc_handle_table;   //command process ptr table

    ATC_CUR_INFO_LIST_T    *atc_info_ptr; // store the infos of AT cmds we
    // want to handle.

    ATC_CONFIG_T           *atc_config_ptr;
} ATC_GLOBAL_INFO_T;
#endif

#define AT_CMD_FUNC(_F_)    \
    ATC_STATUS _F_(ATC_CONFIG_T        *atc_config_ptr, \
                   ATC_CUR_INFO_LIST_T *cur_cmd_info)
// add by Xueli Lv
typedef uint8      RSP_RESULT_STR[MAX_ATC_RSP_LEN];

typedef enum
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
}ATC_SMS_ERR_CODE_E;

typedef struct
{
    MN_SMS_CAUSE_E   mn_sms_err;
    ATC_SMS_ERR_CODE_E atc_sms_err;
}SMS_ERR_CONV_ITEM_T;

enum
{
    ATC_CCED_ONE_SHOT,
    ATC_CCED_AUTO_SHOT,
    ATC_CCED_STOP_AUTO
};

typedef enum
{
    ATC_CMCC_CALLIND_CALL_TYPE_CIRCUIT_VOICE = 0,
    ATC_CMCC_CALLIND_CALL_TYPE_CIRCUIT_DATA = 1,
    ATC_CMCC_CALLIND_CALL_TYPE_PACKET_DATA = 2,
    ATC_CMCC_CALLIND_CALL_TYPE_EMERGENCY = 9,
    ATC_CMCC_CALLIND_CALL_TYPE_INVALID
} ATC_CMCC_CALLIND_CALL_TYPE_E;

typedef enum
{
    ATC_CM_CALL_END_OFFLINE = 0,
    ATC_CM_CALL_END_NO_SRV = 21,
    ATC_CM_CALL_END_CLIENT_END = 29,
    ATC_CM_CALL_END_CONF_FAILED = 101,
    ATC_CM_CALL_END_NETWORK_END = 104
} ATC_CMCC_CM_CALL_END_STATUS_E;

typedef struct
{
    uint16       error_code;
    char        *error_str;
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
    ATC_VOICE_MODE = 0,
    ATC_DATA_MODE = 1,
    ATC_FAX_MODE = 2,
    ATC_UNKNOWN_CALLMODE = 9
} ATC_CALL_MODE_E;

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
    uint8                          call_id;
    ATC_CALL_ENTITY_STATE_E   call_entity_state;
    ATC_HOLD_STATE_E          hold_state;               // current call entity's HOLD auxiliary state
    ATC_MPTY_STATE_E          mpty_state;               // current call entity's MPTY auxiliary state
    uint8                          party_number[MN_MAX_ADDR_BCD_LEN*2+1];
    ATC_TELE_TYPE_E     number_type;
    ATC_CALL_MODE_E   call_mode;
    uint8 is_emergency_call ;
} ATC_CALL_CONTEXT_T;

typedef struct
{
    uint8                    call_amount;
    ATC_CALL_CONTEXT_T       call_context[MN_CALL_MAX_CALL_NUM];
} ATC_CALLS_STATE_T;

#define ATC_INVALID_CALLID   0xFF

typedef enum
{
    ATC_ATH_NULL = 0x00,
    ATC_ATH_LISTENING,
    ATC_ATH_WAITING,
    ATC_ATH_AVOID_REPETITION
} ATC_ATH_STATE_E;

typedef enum
{
    ATC_ATH_EV_NEW_PDP_SRV = 0x00,
    ATC_ATH_EV_NEW_PPP_SRV,
    ATC_ATH_EV_NEW_CS_SRV,
    ATC_ATH_EV_RECV_ATH,
    ATC_ATH_EV_PDP_DEACT,
    ATC_ATH_EV_PDP_ACT_REJ,
    ATC_ATH_EV_WAIT_TIMEOUT,
    ATC_ATH_EV_AVOID_REP_TIMEOUT
} ATC_ATH_EVENT_E;

typedef struct
{
    uint8   link_id;

    /* PSD call fields */
    uint8           cid;                    /* cid run on this link id, first cid value if more cids */
    uint8           ath_cid;                /* PPP cid */
    ATC_ATH_STATE_E ath_stat;               /* PPP ath state */
    SCI_TIMER_PTR   ath_wait_timer;         /* PPP ath wait timer */
    SCI_TIMER_PTR   ath_avoid_rep_timer;    /* PPP ath avoid_rep timer */

    /* VOICE call fields */

    /* VT call fields */

    /* Other fields */
} ATC_LINK_INST_T;

/* if disconnect modem is received and ath received at almost the same time,
 * then the ath command will be dropped. */
extern ATC_LINK_INST_T atc_link_inst[ATC_MAX_LINK_ID];

typedef enum
{
    CEER_ERR_0_NO_ERROR = 0,
    CEER_ERR_1_UNASSIGNED_NUMBER = 1,
    CEER_ERR_3_NO_ROUTE_TO_DESTINATION = 3,
    CEER_ERR_6_UNACCEPTABLE_CHANNEL = 6,
    CEER_ERR_8_OPERATOR_DETERMINATE_BARRING = 8,
    CEER_ERR_16_NORMAL_CLEARING = 16,
    CEER_ERR_17_USER_BUSY = 17,
    CEER_ERR_18_NO_USER_RESPONDING = 18,
    CEER_ERR_19_ALERTING_NO_ANSWER = 19,
    CEER_ERR_21_CALL_REJECTED = 21,
    CEER_ERR_22_NUMBER_CHANGED = 22,
    CEER_ERR_26_NONSELECT_USER_CLEARING = 26,
    CEER_ERR_27_DESTINATION_OUT_OF_ORDER = 27,
    CEER_ERR_28_INVALID_NUMBER_FORMAT = 28,
    CEER_ERR_29_FACILITY_REJECTED = 29,
    CEER_ERR_30_RESPONSE_TO_STATUS_QUERY = 30,
    CEER_ERR_31_NORMAL_UNSPECIFIED = 31,
    CEER_ERR_34_NO_CIRCUIT_CHANNEL_AVAILABLE = 34,
    CEER_ERR_38_NET_OUT_OF_ORDER = 38,
    CEER_ERR_41_TEMPORARY_FAILURE = 41,
    CEER_ERR_42_SWITCH_CONGESTION = 42,
    CEER_ERR_43_ACCESS_INFORMATION_DISCARDED = 43,
    CEER_ERR_44_REQUEST_CIRCUIT_CHANNEL_UNAVAILABLE = 44,
    CEER_ERR_47_RESOURCES_UNAVAILABLE = 47,
    CEER_ERR_49_QOS_UNAVAILABLE = 49,
    CEER_ERR_50_REQUEST_FACILITY_NOT_SUBSCRIBE = 50,
    CEER_ERR_55_CUG_INCOMING_BARRED = 55,
    CEER_ERR_57_BEAR_CAPABILITY_NOT_AUTHORIZATION = 57,
    CEER_ERR_58_BEAR_CAPABILITY_UNAVAILABLE = 58,
    CEER_ERR_63_SERVICE_UNAVAILABLE = 63,
    CEER_ERR_65_BEAR_SERVICE_NOT_IMPLEMENT = 65,
    CEER_ERR_68_ACM_EQUAL_OR_GREAT_ACMMAX = 68,
    CEER_ERR_69_REQUEST_FACILITY_NOT_IMPLEMENT = 69,
    CEER_ERR_70_ONLY_RESTRICT_DIGITAL_AVAILABLE = 70,
    CEER_ERR_79_SERVICE_OPTION_NOT_IMPLEMENT = 79,
    CEER_ERR_81_INVALID_TI = 81,
    CEER_ERR_87_USER_NOT_IN_CUG = 87,
    CEER_ERR_88_INCOMPATIBILITY_DESTINATION = 88,
    CEER_ERR_91_INVALID_TRANSIT_NET = 91,
    CEER_ERR_95_INVALID_MESSAGE_SEMANTIC = 95,
    CEER_ERR_96_MANDATORY_IE_ERROR = 96,
    CEER_ERR_97_MESSAGE_NONEXISTENT = 97,
    CEER_ERR_98_MESSAGE_UNCOMPATBILITY_ERROR = 98,
    CEER_ERR_99_IE_NONEXISTENT = 99,
    CEER_ERR_100_INVALID_CONDITION_IE = 100,
    CEER_ERR_101_MESSAGE_INCOMPATIBILITY_STATE = 101,
    CEER_ERR_102_RECOVER_ON_TIMER = 102,
    CEER_ERR_111_PROTOCOL_ERROR = 111,
    CEER_ERR_127_INTERWORKING = 127,
    CEER_ERR_150_AUTHENTICATION_REJECTED = 150,
    CEER_ERR_151_EMERGENCY_CALL_ONLY = 151,
    CEER_ERR_152_IMSI_DETACH = 152,
    CEER_ERR_153_T3230_EXPIRY = 153,
    CEER_ERR_154_RR_CONNECTION_ERROR = 154,
    CEER_ERR_171_NO_NETWORK_SERVICE = 171,
    CEER_ERR_172_EMERGENCY_CALL_ONLY = 172,
    CEER_ERR_173_NORMAL_DISCONNECT = 173,
    CEER_ERR_174_REMOTE_DISCONNECT = 174,
    CEER_ERR_175_LOW_FAILURE = 175,
    CEER_ERR_176_NETWORK_REJECT = 176,
    CEER_ERR_177_NO_CELL = 177,
    CEER_ERR_180_NOMAL_DETACH_GPRS = 180,
    CEER_ERR_181_NOMAL_DEACTIVE_PDP = 181,
    CEER_ERR_202_SUPPLEMENT_NOT_PROVIDE = 202,
    CEER_ERR_241_FDN_RESTRICTION = 241
} ATC_CEER_ERROR_E;

typedef struct
{
    uint16       err_code;
    char        *err_verbose;
} ATC_CEER_ERROR_STR_T;

typedef struct
{    
    BOOLEAN    is_mute;             // uplink is mute flag
    BOOLEAN    is_remote_mute;      // downlink is mute flag
} ATC_AUDIO_STATUS_T;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E  dual_sys;
    uint8         call_id;
} ATC_DTMF_TIMER_EXP_IND_T;

#ifdef FOTA_SUPPORT_ADUPS
typedef struct
{
    SIGNAL_VARS
    uint16 dual_sys;
    char *cmd;
    uint32 param0;
    uint32 param1;
    uint32 param2;
}AT_ADUPS_MSG_T;
#endif

#ifdef HUAWEI_6500_MODEM
typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E  dual_sys;
} ATC_CELL_TIMER_EXP_IND_T;
#endif

/* access technology of the registered network */
typedef enum
{
    NW_ACT_0_GSM = 0,
    NW_ACT_1_GSM_Compact = 1,
    NW_ACT_2_UTRAN = 2,
    NW_ACT_3_GSM_w_EGPRS = 3,
    NW_ACT_4_UTRAN_w_HSDPA = 4,
    NW_ACT_5_UTRAN_w_HSUPA = 5,
    NW_ACT_6_UTRAN_w_HSDPA_and_HSUPA = 6
} ATC_REG_NETWORK_ACT_E;

//ss service cnf signal type,will be used by atc.c and atc_gsm_oms.c
typedef enum
{
    ATC_SS_SERVICE_CNF_INTERROGATE,
    ATC_SS_SERVICE_CNF_DEACTIVATE,
    ATC_SS_SERVICE_CNF_ACTIVATE,
    ATC_SS_SERVICE_CNF_ERASE,
    ATC_SS_SERVICE_CNF_REGISTER,
    ATC_SS_SERVICE_CNF_RELEASE
} ATC_SS_SERVICE_CNF_TYPE_E;

#define CC_TCH_RESOURCE_ASSIGNED    0x01    // tch has been assigned
#define CC_MT_CALL_CONNECTING   0x02    // user has answered the incoming call

/*******************************************************************************
 **                        Functon  declaration                                *
 ******************************************************************************/
/******************************************************************************/
// Description : write the all item which need in ATC
// Global resource dependence : None
// Author : hanjun.liu
// Note :
/******************************************************************************/
extern BOOLEAN ATC_WriteNVItem(
    ATC_CONFIG_T        *atc_config_ptr,
    uint16 ItemID
);

/******************************************************************************/
// Description : This function constructs the AT command response.
// Global resource dependence : None
// Author : Ivan.Yin
// Note : None
/******************************************************************************/
extern void ATC_BuildInfoRsp(  // Return S_ATC_SUCCESS if success,
    // otherwise return error code.
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    uint8 *rsp_str_ptr  // Point the response string
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
/******************************************************************************/
// Description : This function contructs the result code response
// Global resource dependence :
// Author : wanghui
// Note :
// History:
/******************************************************************************/
extern void ATC_BuildResultErrCodeRsp(
    ATC_CONFIG_T       *atc_config_ptr,  // point to the ATC_CONFIG_T struct.
    int32              result_code_idx,  // result code index
    int32              err_code_idx      //error code index
);

/******************************************************************************/
//    DESCRIPTION: This function handles consequence event of HandleCGEREP
//    Global resource dependence : atc_config_ptr
//    AUTHOR:  Pizer.Fan
//    Note :
/******************************************************************************/
extern void ATC_BuildGprsEvReportRsp(
    ATC_CONFIG_T     *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    xSignalHeaderRec *pSig
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
//This function is provided by MMI
#ifndef _ATC_ONLY
extern uint32 ATC_ExtCmd(uint8 *cmd, uint32 len);
#endif
ATC_STATUS atcparse(
    uint8 *atcbuf,  /* cr209452 clean pclint warnnings by zhangjun */
    ATC_CUR_INFO_LIST_T **info_ptr_ptr
);

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

/******************************************************************************/
// Description : This function is used to ignore data before 'at'
// Global resource dependence : None
// Author : Pizer.Fan
// Note : ignore data before 'at'
/******************************************************************************/
extern const uint8 *atcparse_at(const uint8 *atcbuf, uint32 len);

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
void ATC_Mux_Release_Link(uint8 link_id);

/******************************************************************************/
// Description : This function set the current link id
// Global resource dependence :
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Set_Current_Link_id(ATC_CONFIG_T *atc_config_ptr, uint8 link_id);
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
BOOLEAN ATC_Receive_Expected_Event(
    ATC_CONFIG_T         *atc_config_ptr,
    uint16               expected_event,
    uint16               id,
    ATC_EVENT_DOMAIN_T   domain_id,
    MN_DUAL_SYS_E    dual_sys
);

/******************************************************************************/
// Description : This function get the link id of the expected event if existed 
// Global resource dependence :
// Author :
// Note : 
/******************************************************************************/
BOOLEAN ATC_Get_Clean_Expected_Event(
    MN_DUAL_SYS_E   dual_sys,
    uint16 expected_event,
    ATC_EVENT_DOMAIN_T domain_id,
    uint16 *checkid,
    uint8 *link_id
    );

/******************************************************************************/
// Description : This function add the expected event when requested the expected event
// Global resource dependence :
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Add_Expected_Event(
    ATC_CONFIG_T *atc_config_ptr,
    uint16 expected_event,
    uint16 id,
    ATC_EVENT_DOMAIN_T domain_id,
    MN_DUAL_SYS_E    dual_sys
);

/******************************************************************************/
// Description : This function check the expected event is existed or not
// Global resource dependence :
// Author : Pizer Fan
// Note :
/******************************************************************************/
BOOLEAN ATC_Check_Expected_Event(uint8 link_id, uint16 expected_event, uint8 cid, MN_DUAL_SYS_E dual_sys);

/******************************************************************************/
// Description : This function check the expected event is existed or not
// Global resource dependence :
// Author : minqian.qian
// Note :
/******************************************************************************/
BOOLEAN ATC_Is_Expected_Event_Present(
    uint16 expected_event,
    uint16 checkid,
    ATC_EVENT_DOMAIN_T domain_id,
    MN_DUAL_SYS_E   dual_sys
);

/******************************************************************************/
// Description : This function shrink the expected event buffer
// Global resource dependence :
// Author : minqian.qian
// Note :
/******************************************************************************/
void ATC_Shrink_Expected_Event_Buffer(ATC_CONFIG_T *atc_config_ptr);

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
    uint8 *data_ptr,       // Output argument, point to the buf which is alloced
    // by caller
    uint length,           // The length of cmd line which should be read from
    // sio
    uint *read_length_ptr  // The actual read length
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
uint8 ATC_Get_Link_Id(uint8 id, ATC_EVENT_DOMAIN_T domain_id, MN_DUAL_SYS_E dual_sys);

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
void ATC_Clear_All_Exp_Result(uint8 id, ATC_EVENT_DOMAIN_T domain_id, MN_DUAL_SYS_E dual_sys);

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
void ATC_List_Expected_Event(void);

/******************************************************************************/
// Description : This function get link id from sim_id
// Global resource dependence : 
// Author : Elvis Xu
// Note :
/******************************************************************************/
extern uint8 ATC_GetMuxLinkIdFromSimId(MN_DUAL_SYS_E sim_id);

/******************************************************************************/
// Description : This function get sim_id id from link id
// Global resource dependence : 
// Author : Elvis Xu
// Note :
/******************************************************************************/
MN_DUAL_SYS_E ATC_GetSimIdFromLinkId(uint8 link_id);

/******************************************************************************/
// Description : This function get data cid from link id
// Global resource dependence : 
// Author : Elvis Xu
// Note :
/******************************************************************************/
uint8 ATC_GetCidFromLinkId(uint8 link_id);

/******************************************************************************/
// Description : This function get data link id id from cid
// Global resource dependence : 
// Author : Elvis Xu
// Note :
/******************************************************************************/
uint8 ATC_GetLinkIdFromCid(uint8 cid);

#endif

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     ATC_Link_ATHEventHandle                                               *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     handle PS PDP ATH related event.                                      *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
BOOLEAN ATC_Link_ATHEventHandle(uint8 link_id, ATC_ATH_EVENT_E ath_ev, uint8 cid);

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     ATC_Link_Property                                                     *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Get property of the specific link.                                    *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     const ATC_LINK_INST_T *                                               *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
const ATC_LINK_INST_T *ATC_Link_Property(uint8 link_id);

#ifndef ATC_SAT_ENABLE
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     ATC_Link_Is_Existed_Data_PSD                                          *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Judge if there is data PSD on the specific link or not                *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     BOOLEAN                                                               *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
BOOLEAN ATC_Link_Is_Existed_Data_PSD(uint8 link_id);
#endif

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     ATC_Get_Data_Link_Id                                                  *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Get data link id                                                      *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     uint8                                                                 *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
uint8 ATC_Get_Data_Link_Id(uint8 link_id, uint8 cid);

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     ATC_Create_PSD_Call                                                   *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Create PSD call info                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
BOOLEAN ATC_Create_PSD_Call(uint8 cid, uint8 link_id, BOOLEAN data_mode_req);

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void ATC_Set_PSD_Call_State()                                         *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Set PSD call state                                                    *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_Set_PSD_Call_State(uint8 cid, ATC_PSD_STATE_E state);

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void ATC_Clear_PSD_Call_Info()                                        *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     clear PSD call info                                                   *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_Clear_PSD_Call_Info(uint8 cid);

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void ATC_List_PSD_Call_Info()                                         *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     List PSD call info                                                    *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_List_PSD_Call_Info(uint8 cid);

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     ATC_GetCeerErrVerbose                                                 *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     get the error string of +CEER                                         *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     BOOLEAN                                                               *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
BOOLEAN ATC_GetCeerErrVerbose(
    uint16 err_code,
    char  *err_verbose
);

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     ATC_SynchronizeService                                                *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     synchronize services                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     BOOLEAN                                                               *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
BOOLEAN ATC_SynchronizeService(MN_DUAL_SYS_E  dual_sys);
/*****************************************************************************/
// Description : This function handles +Esqopt Time out
// Global resource dependence : g_rsp_str
// Author:      sunny.shen
// Note:
/*****************************************************************************/
extern void ATC_ProcessEsqoptTimeout(
    uint32 lparam   //the function's input parameter
);

/******************************************************************************/
// Description : This function do some reset work for ATC
// Global resource dependence : g_atc_info_table
//                              g_atc_handle_table
//                              g_atc_result_code_text
// Author : Yijin
// Note : This function should be called when ps reset confirm received
/******************************************************************************/
int ATC_Reset(MN_DUAL_SYS_E dual_sys);

/******************************************************************************/
// Description : This function find pending event from event list
// Global resource dependence :
// Author : Martin Jiang
// Note :
/******************************************************************************/
BOOLEAN ATC_GetPending_Event(
    ATC_CONFIG_T *atc_config_ptr,
    uint16 expected_event,
    uint8 link_id,
    MN_DUAL_SYS_E dual_sys
);

/******************************************************************************/
// Description : This function find number of expected events from event list
// Global resource dependence :
// Author : minqian Qian
// Note :
/******************************************************************************/
uint8 ATC_Find_Expected_Event(
    ATC_CONFIG_T *atc_config_ptr,
    uint16 expected_event,
    uint8 link_id,
    MN_DUAL_SYS_E dual_sys
);

/*****************************************************************************/
//  Description : return this flag for ath in atc_gsm.c
//  Global resource dependence : none
//  Author:       bo.chen
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_IsPsActivate(
    void
);

/******************************************************************************/
// Description : This function up DSCI info to TE.
// Global resource dependence : None
// Author : minqian.qian
// Note : None
/******************************************************************************/
extern void ATC_UpDSCIInfo(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    xSignalHeaderRec        *sig_ptr,
    uint8       call_index,         //the index of call state context
    uint8       callstat               //the state of cal     0 ~ 6
);

/******************************************************************************/
// Description : This function up +SIND info
// Global resource dependence : None
// Author : minqian.qian
// Note : None
/******************************************************************************/
extern void ATC_UpSINDInfo(MN_DUAL_SYS_E  dual_sys,
                           ATC_CONFIG_T *atc_config_ptr,
                           xSignalHeaderRec        *sig_ptr,
                           uint16 atc_wind_bit_flag,       //which the wind bit to check with
                           uint16  upValue,            //+sind up value
                           BOOLEAN  autoUpFlag         //is auto up or not
                          );

/******************************************************************************/
// Description : This function up ^SYSINFO info
// Global resource dependence : None
// Author : minqian.qian
// Note : None
/******************************************************************************/
extern void ATC_UpSYSINFOInfo(MN_DUAL_SYS_E dual_sys,
                       ATC_CONFIG_T *atc_config_ptr,
                       BOOLEAN       autoUpFlag
                       );

/******************************************************************************/
// Description : This function up +ECIND info to TE.
// Global resource dependence : None
// Author : minqian.qian
// Note : None
/******************************************************************************/
extern void ATC_UpECINDInfo(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    xSignalHeaderRec        *sig_ptr,
    uint16           type,      //ME status(+ECIND <type>)
    uint16 value1,   uint16 value2, uint16 value3, uint16    value4);

/******************************************************************************/
// Description : This function up +CREG info to TE.
// Global resource dependence : None
// Author : minqian.qian
// Note : None
/******************************************************************************/
void ATC_UpCREGInfo(MN_DUAL_SYS_E                   dual_sys,
                    ATC_CONFIG_T                   *atc_config_ptr,
                    MN_PHONE_CURRENT_PLMN_INFO_T   *plmn_info_ptr,  //plmn info
                    OPER_STATUS_E                   oper_status,   //oper status
                    BOOLEAN                         autoUpFlag    //unsolicited or not
                    );

/******************************************************************************/
// Description : This function up +CGREG info to TE.
// Global resource dependence : None
// Author : minqian.qian
// Note : None
/******************************************************************************/
void ATC_UpCGREGInfo(MN_DUAL_SYS_E                   dual_sys,
                     ATC_CONFIG_T                   *atc_config_ptr,
                     MN_PHONE_CURRENT_PLMN_INFO_T   *plmn_info_ptr,  //plmn info
                     OPER_STATUS_E                   gprs_oper_status,  //GPRS oper status
                     BOOLEAN                         autoUpFlag     //unsolicited or not
                     );

/******************************************************************************/
// Description : This function up ^CEND info to TE.
// Global resource dependence : None
// Author : minqian.qian
// Note : None
/******************************************************************************/
extern void ATC_UpCENDInfo(ATC_CONFIG_T        *atc_config_ptr,
                           xSignalHeaderRec *sig_ptr
                          );

/******************************************************************************/
// Description : This function judge that psd call info list has datamode item or not
// Global resource dependence :
// Author : minqian.qian
// Note :
/******************************************************************************/
extern BOOLEAN ATC_IsExist_DataMode_PSD_Call_Info(void);

/******************************************************************************/
// Description : This function set ps activate status
// Global resource dependence :
// Author : minqian.qian
// Note :
/******************************************************************************/
void ATC_SetPsActivateStatus(
    BOOLEAN is_activate
);

/******************************************************************************/
// Description : This function is used to unsolicited report +CUSD info to TE.
// Global resource dependence : None
// Author : minqian.qian
// Note : None
/******************************************************************************/
extern void ATC_UpCUSDInfo(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T       *atc_config_ptr,
    MN_SS_USSD_DATA_T *ussd_ptr,   //ussd data info
    uint8             ussd_action  //corresponding to <m> parameter in unsolicited +CUSD
);

/******************************************************************************/
// Description : This function is used to unsolicited report +SPERROR info to TE.
// Global resource dependence : None
// Author : 
// Note : None
/******************************************************************************/
extern void ATC_UpSPERRORInfo(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T    *atc_config_ptr,
    ATC_ERROR_ITEM_CLASS item_class, 
    uint32 cause
);

/*****************************************************************************/
//  Description:    return the atc version
//                  Global resource dependence:
//                      s_atc_version_info
//  Author:         Fancier.Fan
//                  Note: 2005 8 24
/*****************************************************************************/
extern const char *SCI_GetATVersionInfo(void);

/******************************************************************************/
// Description : This function get s_config_info variable pointer
// Global resource dependence :
// Author : Elvis Xu
// Note :
/******************************************************************************/
ATC_CONFIG_T * ATC_GetConfigInfoPtr(void);

/******************************************************************************/
// Description : This function constructs the AT command response.
// Global resource dependence : None
// Author : Ivan
// Note :
/******************************************************************************/
extern void ATC_BuildUnsolicitedInfo(
        MN_DUAL_SYS_E dual_sys,
        ATC_CONFIG_T   *atc_config_ptr,  // point to the ATC_CONFIG_T struct.
        uint8          *rsp_str_ptr      // Point the response string
        );

/******************************************************************************/
// Description : This function send the AT command response.
// Global resource dependence : None
// Author : Elvis.xu
// Note :
/******************************************************************************/
extern void ATC_SendReslutRsp(
                 ATC_CONFIG_T   *atc_config_ptr,  // point to the ATC_CONFIG_T struct.
                 uint8  *data_ptr,                // point to the response string
                 uint32 data_length               // length of response string
                 );

/******************************************************************************/
// Description : This function send the AT command response for free map.
// Global resource dependence : None
// Author : Vicky.wang
// Note :
/******************************************************************************/
extern void ATC_SendBroadCastRsp(
                 ATC_CONFIG_T   *atc_config_ptr,  // point to the ATC_CONFIG_T struct.
                 uint8  *data_ptr,                // point to the response string
                 uint32 data_length               // length of response string
                 );

/******************************************************************************/
// Description : This function send the asynchronous information.
// Global resource dependence : None
// Author : Elvis.xu
// Note :
/******************************************************************************/
extern void ATC_SendAsyncInfo(
                 ATC_CONFIG_T   *atc_config_ptr,  // point to the ATC_CONFIG_T struct.
                 uint8 ctlz_end_status
                 );

#ifdef HUAWEI_6500_MODEM
/*****************************************************************************/
// Description : active the pdp by username and password 
// Global resource : 
// Author : fen.xie
// Note :   PARAM1: link_id (IN) -- MUX link ID
/*****************************************************************************/
extern void PPP_ActivePdpByAtc(char *username,
                        char *passwd,
                        int  pwd_len,
                        int  user_len,
                        uint32 pid,
                        MN_PCO_AUTH_TYPE_E auth_type,
                        char *challenge,
                        uint8 challenge_len,
                        char *chall_name,
                        uint8 chall_name_len
			);
#endif

#ifdef _NDS_EXT_CMD_
/** 
 *function: sign data with recieved randnum 
 *called by: gprs module application software.
 *implemented by:this lib implement this interface.
 * @data[in]: the data need to be sign.
 * @data_len[in]: the size of data to be signed in byte.
 * @randnum[in]:  the randnum recieved from stb
 * @randnum_len[in]: the length of randnum
 * @cid[in]:stb id, that recieved from stb when initialize boot
 * @gmid[in]:gprs module id
 * @seed[in]:key seed that recieved from stb when initialize boot
 * @signres[in]:is the address that save the sign result. 
 * @sign_len[in,out]: [in] is the size in byte that can save the sign result.
 *                    [out] is the actually size of the sign result in byte.
 * @return:if successfully return 0, otherwise return nonzero. When return 
 *         nonzero, the caller may call sarft_error_str function with 
 *         sarft_sign_errno to error description.
**/
extern int sarft_sign_data(unsigned char *data, int data_len, unsigned char *randnum, 
        int randnum_len, const unsigned char *cid, const unsigned char *gmid, 
        unsigned char *seed, unsigned char *signres, int *sign_len);
#endif

/******************************************************************************/ 
//    DESCRIPTION:
//           control wifi action.
//    Global: atc_config_ptr 
//    AUTHOR: wuding.yang
//    Note : 
//    History:
/******************************************************************************/
ATC_STATUS ATC_ProcessWIFICTRL( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
				ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
							 );

/******************************************************************************/ 
//    DESCRIPTION:
//           process the signal from BT task
//    Global: atc_config_ptr 
//    AUTHOR: yongxia.zhang
//    Note : 
//    History:
/******************************************************************************/
ATC_STATUS ATC_ProcessBtSignal(ATC_CONFIG_T *atc_config_ptr, xSignalHeaderRec *sig_ptr);

#ifdef _SCM21_ATC_DEV
/******************************************************************************/
//  DESCRIPTION:
//           set gprs s_dataconf state.
//  Global:
//  AUTHOR: gang
//  Note :  this function is used while active/deactive pdp context operation and so on.
//  History:
/******************************************************************************/
void SetGPRSDataConfStatus(BOOLEAN is_all,uint8 index, uint32 state);

#endif //_SCM21_ATC_DEV

#endif
