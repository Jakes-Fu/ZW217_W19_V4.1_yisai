/******************************************************************************
 ** File Name:      ps_mn_signals_def.h                                       *
 ** Author:         Tracy Zhang                                               *
 ** Date:           2012-08-22                                                *
 ** Copyright:      Copyright 2001-2012 by Spreadtrum Communications,Inc.     *
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
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2012-08-22     Tracy Zhang      Create.                                   *
 ** 2012-08-23     Andy Huang       add macros, enums
 ******************************************************************************/

#ifndef _PS_MN_SIGNALS_DEF
#define _PS_MN_SIGNALS_DEF

#include "sci_types.h"
#include "sci_api.h"

/**************************************************************
*                           MACRO                             *
***************************************************************/
#define BEARER_CAPABILITY_LEN (16)
#define ADDR_BCD_MAX_LEN (20)
#define SUB_ADDR_MAX_LEN (20)
#define SMALL_USER_INFO_MAX_LEN (35)
#define USER_INFO_MAX_LEN (35)
#define LLC_MSG_LEN (16)
#define HLC_MSG_LEN (3)
#define BACKUP_BEARER_CAPABILITY_LEN (15)
#define CC_CAPABILITY_LEN (2)
#define NETWORK_NAME_ARR_LEN (40)
#define MAX_EMG_NUM_LIST_SIZE (10)
#define APN_LENGTH (100)
#define MAX_NSAPI_NUMBER (11)
#define WILDCARD_TI (128)
#define WILDCARD_PD (128)
#define NUMBER_STRING_MAX_LEN            (80)

#if defined (_SUPPORT_IPC_) || defined (GSM_CUSTOMER_AFP_SUPPORT)
#define LONG_PLMN_SIZE        200
#else
#define LONG_PLMN_SIZE        85
#endif

/**************************************************************
*                           ENUM                              *
***************************************************************/
typedef enum
{
    LENGTH_EVEN,
    LENGTH_ODD
}LENGTH_TYPE_E;

typedef enum
{
    CLIR_SUPPRESSION,
    CLIR_INVOCATION,
    CLIR_NETWORK_DECIDE
}CLIR_E;

typedef uint16 CALL_STATE_E;
#define    PSCC_NULL_STATE    0
#define    CALL_INITIATED_STATE    1
#define    MM_CONN_PENDING_STATE    2
#define    MO_CALL_PROCEEDING_STATE    3
#define    CALL_DELIVERED_STATE    4
#define    CALL_PRESENT_STATE    6
#define    CALL_RECEIVED_STATE    7
#define    CONN_REQ_STATE    8
#define    MT_CALL_CNF_STATE    9
#define    ACTIVE_STATE    10
#define    DISC_REQ_STATE    11
#define    DISC_IND_STATE    12
#define    REL_REQ_STATE    19
#define    MO_MODIFY_STATE    26
#define    MT_MODIFY_STATE    27
#define    CC_PROM_PRESENT_STATE    34
#define    WAIT_FOR_NETWORK_INFO_STATE    35
#define    CC_EST_PRESENT_STATE    36
#define    CC_EST_CNF_STATE    37
#define    RECALL_PRESENT_STATE    38
#define    CALL_REEST_STATE    80

typedef uint16 RE_INDICATION_E;
#define    REPEAT_CIRCULAR_SUCC_SEL    1
#define    REPEAT_SUPPORT_FALLBACK    2
#define    REPEAT_RESERVED    3
#define    REPEAT_SERVICE_CHAG_FALLBACK    4

typedef enum
{
    PRESENT_ALLOW,
    PRESENT_RESTRICT,
    PRESENT_NUM_NO_AVAILABLE
}PRESENT_IND_E;

typedef enum
{
    NOT_SCREEN,
    SCREEN_VRF_PASS,
    SCREEN_VRF_FAIL,
    SCREEN_NETWORK_PVD
}SCREEN_IND_E;

typedef enum
{
    CODE_ITU_STD,
    CODE_INTERNATIONAL_STD,
    CODE_NATION_STD,
    CODE_GSM_STD
}CODE_STD_E;

typedef uint16 LOCATION_E;
#define    LOCATION_USER    0
#define    PRIV_SER_LOC    1
#define    PUB_SER_LOC    2
#define    PUB_SER_REMOTE    4
#define    PRIV_SER_REMOTE    5
#define    NET_BYN_INTER    10

typedef uint16 PROG_DES_E;
#define    PROG_NOT_END_TO_END    1
#define    PROG_DEST_NO_PLMN    2
#define    PROG_ORIG_NO_PLMN    3
#define    PROG_RTN_PLMN    4
#define    PROG_IN_BAND_INFO    8
#define    PROG_END_TO_END    32
#define    PROG_QUEUEING    64

typedef enum
{
    CALL_NO_PRIOR = 0,
    CALL_PRIOR_LEVER4 = 1,
    CALL_PRIOR_LEVER3 = 2,
    CALL_PRIOR_LEVER2 = 3,
    CALL_PRIOR_LEVER1 = 4,
    CALL_PRIOR_LEVER0 = 5,
    CALL_PRIOR_LEVERB = 6,
    CALL_PRIOR_LEVERA = 7
}CALL_PRIOR_E;

typedef uint16 ALERT_PATTERN_E;
#define    ALERT_PATTERN1    0
#define    ALERT_PATTERN2    1
#define    ALERT_PATTERN3    2
#define    ALERT_PATTERN5    4
#define    ALERT_PATTERN6    5
#define    ALERT_PATTERN7    6
#define    ALERT_PATTERN8    7
#define    ALERT_PATTERN9    8

typedef uint16 SIGNAL_VALUE_E;
#define    SIGNAL_DIAL    0
#define    SIGNAL_RING_BACK    1
#define    SIGNAL_INTERCEPT    2
#define    SIGNAL_CONGESTION    3
#define    SIGNAL_BUSY    4
#define    SIGNAL_CONFIRM    5
#define    SIGNAL_ANSWER    6
#define    SIGNAL_WAITING    7
#define    SIGNAL_OFF_HOOK    8
#define    SIGNAL_TONES_OFF    63
#define    SIGNAL_ALERTING_OFF    79

typedef enum
{
    CLIR_UNAVAILABLE,
    CLIR_REJECT_BY_USER,
    CLIR_INTERACT_OTHER,
    CLIR_PAYPHONE
}NO_CLIR_CAUSE_E;

typedef uint16 GSM_CAUSE_T;
#define    CAUSE_UNASSIGNED_NO    1
#define    CAUSE_NO_ROUTE_TO_DEST    3
#define    CAUSE_CHAN_UNACCEPTABLE    6
#define    CAUSE_OPER_DETERM_BARRING    8
#define    CAUSE_NORMAL_CLEARING    16
#define    CAUSE_USER_BUSY    17
#define    CAUSE_NO_USER_RESPONDING    18
#define    CAUSE_ALERTING_NO_ANSWER    19
#define    CAUSE_CALL_REJECTED    21
#define    CAUSE_NUMBER_CHANGED    22
#define    CAUSE_NONSEL_USER_CLRING    26
#define    CAUSE_DEST_OUT_OF_ORDER    27
#define    CAUSE_INVALID_NO_FORMAT    28
#define    CAUSE_FACILITY_REJECTED    29
#define    CAUSE_RSP_TO_STATUS_ENQ    30
#define    CAUSE_NORMAL_UNSPECIFIED    31
#define    CAUSE_NO_CIRC_CHAN_AV    34
#define    CAUSE_NET_OUT_OF_ORDER    38
#define    CAUSE_TEMP_FAILURE    41
#define    CAUSE_SWITCH_CONGESTION    42
#define    CAUSE_ACC_INFO_DISCARDED    43
#define    CAUSE_REQ_CIRC_CHAN_UNAV    44
#define    CAUSE_RESOURCES_UNAV    47
#define    CAUSE_QOS_UNAV    49
#define    CAUSE_REQ_FAC_NOT_SUBSCR    50
#define    CAUSE_CUG_INCOMING_BARRED    55
#define    CAUSE_BEAR_CAP_NOT_AUTH    57
#define    CAUSE_BEAR_CAP_UNAV    58
#define    CAUSE_SERV_OPT_UNAV    63
#define    CAUSE_BEAR_SVC_NOT_IMPL    65
#define    CAUSE_ACM_EQ_OR_GT_ACMMAX    68
#define    CAUSE_REQ_FACIL_NOT_IMPL    69
#define    CAUSE_ONLY_RESTRIC_DIG_AV    70
#define    CAUSE_SVC_OPT_NOT_IMPL    79
#define    CAUSE_INVALID_TI    81
#define    CAUSE_USER_NOT_IN_CUG    87
#define    CAUSE_INCOMPAT_DEST    88
#define    CAUSE_INVALID_TRANSIT_NET    91
#define    CAUSE_INVALID_MSG_SEMANTIC    95
#define    CAUSE_MAND_IE_ERROR    96
#define    CAUSE_MSG_NONEXISTENT    97
#define    CAUSE_MSG_GEN_ERROR    98
#define    CAUSE_IE_NONEXISTENT    99
#define    CAUSE_INVALID_CONDITION_IE    100
#define    CAUSE_MSG_INCOMPAT_STATE    101
#define    CAUSE_RECOV_ON_TIMER_EXP    102
#define    CAUSE_PROTOCOL_ERROR    111
#define    CAUSE_INTERWORKING    127
#define    CAUSE_AUTHENTICATION_REJ    150
#define    CAUSE_EMERGENCY_CALL_ONLY    151
#define    CAUSE_IMSI_DETACH    152
#define    CAUSE_T3230_EXPIRY    153
#define    CAUSE_RR_CONNECTION_ERROR    154
#define    CAUSE_SMS_CANCEL_SUCCEEDED    155
#define    CAUSE_SMS_CANCEL_FAILED     156

typedef uint16 BEARER_CAPABILITY_TYPE_E;
#define    BEARER_CAPABILITY_LINE_1    0
#define    BEARER_CAPABILITY_LINE_2    1
#define    BEARER_CAPABILITY_LINE_MAX    2

typedef enum
{
    NOTIFY_SUSPEND,
    NOTIFY_RESUME,
    NOTIFY_BEARER_CHANGE
}NOTIFY_E;

typedef enum
{
    HOLD_IDLE,
    HOLD_REQUEST,
    CALL_HOLD,
    RETRIEVE_REQUEST
}HOLD_AUXILIARY_STATE_E;

typedef enum
{
    MPTY_IDLE,
    MPTY_REQUEST,
    CALL_IN_MPTY,
    SPLIT_REQUEST
}MPTY_AUXILIARY_STATE_E;

typedef uint16 CONGESTION_LEVEL_E;
#define    LEVEL_RECEIVER_READY    0
#define    LEVEL_RECEIVER_NOT_READY    15

typedef uint16 SS_LOW_LAYER_ERROR_E;
#define    SS_LOW_LAYER_ERROR_NONE    0
#define    SS_LOW_LAYER_ERROR_NO_ROUTER    1
#define    SS_LOW_LAYER_ERROR_NO_CONN    2
#define    SS_LOW_LAYER_ERROR_CONN_REL    3

typedef enum
{
    GSM_ONLY,
    GSM_AND_GPRS,
    GPRS_ONLY
}MS_ATTACH_TYPE_E;

typedef enum
{
    MS_NORMAL_SERVICE,
    MS_EMERGENCY_SERVICE_ONLY,
    MS_NO_CELL,
    MS_NULL
}HANDSET_STATE_E;

typedef enum
{
    MS_POWER_OFF = 0,
    MS_SIM_REMOVE = 1,
    MS_SIM_INVALID = 2,
    MS_GSM_DETACH = 3,
    MS_DEACT_TO_CALI = 4
}MN_MM_NREG_CAUSE_E;

typedef enum
{
    NREG_IND_ABNORMAL_CELL = 0,
    NREG_IND_BAD_SIM = 1,
    NREG_IND_IMSI_UNKNOW_IN_HLR = 2,
    NREG_IND_PLMN_NOT_ALLOW = 3,
    NREG_IND_LAI_NOT_ALLOW_R = 4,
    NREG_IND_LAI_NOT_ALLOW_S = 5,
    NREG_IND_ACC_NOT_ALLOW = 6,
    NREG_IND_ACC_NETWORK_BUSY = 7,
    NREG_IND_ATTEMP_FAILURE = 8,
    NREG_IND_SIM_AUTH_FAIL = 9,
}MN_MM_NREG_IND_CAUSE_E;

typedef enum
{
    MS_POWER_ON_WITH_SIM = 0,
    MS_POWER_ON_NO_SIM = 1,
    MS_SIM_INSERT = 2,
    MS_PLMN_RESELECTION = 3,
    MS_USER_REQ_MANUAL_PLMN_SELECTION = 4
}MN_MM_REG_CAUSE_E;

typedef enum
{
    RR_ACT_NORMAL,
    RR_ACT_EMERGENCY_ONLY,
    RR_ACT_NO_SERVICE
}RR_ACT_TYPE_E;

typedef enum
{
    MM_UPDATED = 0,
    MM_NOT_UPDATED = 1,
    MM_PLMN_NOT_ALLOWED = 2,
    MM_LAI_NOT_ALLOWED = 3,
    MM_REVERSE_STATE = 4
}MM_UPDATE_STATUS_E;

typedef enum
{
    SM_PPP = 0,
    SM_IPv4 = 1,
    SM_IPv6 = 2,
    SM_IPv4v6 = 3,
    PDP_EMPTY = 4
}PDP_ADDR_TYPE_E;

typedef int16 SM_CAUSE_T;
#define    LLC_OR_SNDCP_FAILURE    25
#define    INSUFFICIENT_RESOURCES    26
#define    MISSING_OR_UNKNOWN_APN    27
#define    UNKNOWN_PDP_ADDRESS_OR_TYPE    28
#define    USER_AUTHENTICATION_FAILED    29
#define    ACTIVATION_REJECTED_BY_GGSN    30
#define    ACTIVATION_REJECTED_UNSPECIFIED    31
#define    SERVICE_OPTION_NOT_SUPPORTED    32
#define    REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED    33
#define    SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER    34
#define    NSAPI_ALREADY_USED    35
#define    REGULAR_DEACTIVATION    36
#define    QOS_NOT_ACCEPTED    37
#define    NETWORK_FAILURE    38
#define    REACTIVATION_REQUIRED    39
#define    SEMANTIC_ERROR_IN_THE_TFT_OPERATION    41
#define    SYNTACTICAL_ERROR_IN_THE_TFT_OPERATION    42
#define    UNKNOWN_PDP_CONTEXT    43
#define    SEMANTIC_ERRORS_IN_PACKET_FILTER    44
#define    SYNTACTICAL_ERRORS_IN_PACKET_FILTER    45
#define    PDP_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATE    46
#define    PDP_TYPE_IPV4_ONLY_ALLOWED         50
#define    PDP_TYPE_IPV6_ONLY_ALLOWED         51
#define    SINGLE_ADDR_BEARERS_ONLY_ALLOWED   52
#define    INVALID_TRANSACTION_IDENTIFIER_VALUE    81
#define    INVALID_MANDATORY_INFORMATION    96
#define    MESSAGE_TYPE_NONEXISTENT_OR_NOT_IMPLEMENTED    97
#define    MESSAGE_TYPE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE    98

typedef enum
{
    NO_OP_TFT_2G = 0,
    CREATE_NEW_TFT_2G = 1,
    DELETE_EXISTING_TFT_2G = 2,
    ADD_PKT_FILTER_TO_TFT_2G = 3,
    REPLACE_PKT_FILTER_IN_TFT_2G = 4,
    DELETE_PKT_FILTER_FROM_TFT_2G = 5
}TFT_OPCODE_E;

typedef enum
{
    MANUAL,
    AUTO
}RESPONSE_MODE_E;

typedef int32 GMM_ATTACH_TYPE_T;
#define GPRS_ATTACH 1
#define GPRS_ATTACH_WHILE_IMSI 2
#define GPRS_COMBINED_ATTACH 3

typedef int32 GMM_MN_ATTCH_RESULT_T;
#define    GMM_MN_ATTACH_PARA_WRONG    0
#define    GMM_MN_ATTACH_BOTH_SUCCESS    1
#define    GMM_MN_ATTACH_GPRS_SUCCESS    2
#define    GMM_MN_ATTACH_FAIL    3
#define    GMM_MN_ATTACH_FAIL_WITH_GPRS_NOT_ALLOW    4
#define    GMM_MN_ATTACH_FAIL_WITH_IMSI_ATTACH    5
#define    GMM_MN_ATTACH_FAIL_WITH_ATTEMPT_MAX    6
#define    GMM_MN_ATTACH_FAIL_WITH_ACCESS_BARRED    7
#define    GMM_MN_ATTACH_FAIL_WITH_PLMN_NOT_ALLOW    8
#define    GMM_MN_ATTACH_GSM_FAIL_WITH_ATTEMPT_MAX    9

typedef int32 GMM_DETACH_TYPE_E;
#define    GPRS_DETACH    1
#define    GPRS_IMSI_DETACH    2
#define    GPRS_COMBINED_DETACH    3
#define    GPRS_DETACH_SWITCH_TO_CALI    4

typedef int32 GMM_MN_DETACH_RESULT_T;
#define    GMM_MN_DETACH_PARA_WRONG    0
#define    GMM_MN_DETACH_GPRS    1
#define    GMM_MN_DETACH_GSM    2
#define    GMM_MN_DETACH_BOTH    3
#define    GMM_MN_DETACH_GPRS_WITH_IMSI_ATTACH    4
#define    GMM_MN_DETACH_GPRS_WITH_GPRS_NOT_ALLOW    5
#define    GMM_MN_DETACH_BOTH_FAIL_WITH_RAUEMPT_MAX    6
#define    GMM_MN_DETACH_FAIL_WITH_GMM_WRONG    7
#define    GMM_MN_DETACH_FAIL_WITH_PLMN_NOT_ALLOW    8
#define    GMM_MN_DETACH_GSM_FAIL_WITH_ATTEMPT_MAX    9
#define    APP_CAUSE_USER    400
#define    CAUSE_ACCESS_NOT_ALLOW    401

typedef enum
{
    ATC_PC_OFF = 0,
    ATC_PC_ON = 1,
    ATC_PC_RFC1144 = 2,
    ATC_PC_RFC2507 = 3
}ATC_PC_TYPE_E;

typedef enum
{
    ATC_DC_OFF = 0,
    ATC_DC_ON = 1,
    ATC_DC_V42BIS = 2
}ATC_DC_TYPE_E;

typedef uint16 USER_USER_PD_E;
#define    UTU_USER_SPEC    0
#define    UTU_OSI_HIGH_L    1
#define    UTU_X244    2
#define    UTU_IA5_CHAR    4
#define    UTU_CALL_CONTROL_MSG    8

typedef uint16 SUBADDRESS_TYPE_E;
#define    SUBADDR_NSAP_TYPE    0
#define    SUBADDR_USER_SPEC_TYPE    2

typedef uint16 NUMBER_TYPE_E;
#define    NUM_TYPE_UNKNOW    0
#define    NUM_TYPE_INTERNATIONAL    1
#define    NUM_TYPE_NATIONAL    2
#define    NUM_TYPE_NETWORK    3
#define    NUM_TYPE_DEDICATECODE_SHORTCODE    4
#define    NUM_TYPE_ALPHANUMERIC    5
#define    NUM_TYPE_ABBREVIATE    6

typedef uint16 NUMBER_PLAN_E;
#define    NUM_PLAN_UNKNOW    0
#define    NUM_PLAN_ISDN_TELE    1
#define    NUM_PLAN_DATA    3
#define    NUM_PLAN_TELEX    4
#define    NUM_PLAN_NATIONAL    8
#define    NUM_PLAN_PRIVATE    9
#define    NUM_PLAN_ERMES    10


/**************************************************************
*                           STRUCTURE                         *
***************************************************************/

/**********************MN<->CC related types **********************/
typedef struct {
    int16  pd;
    int16  ti;
} PD_TI_REF_T;

typedef uint8 BEARER_CAPABILITY_BUF_T[BEARER_CAPABILITY_LEN];

typedef struct {
    uint16  bc_len;
    BEARER_CAPABILITY_BUF_T  bc_buf;
} BEARER_CAPABILITY_T;

typedef struct {
    BOOLEAN  is_police_support;
    BOOLEAN  is_ambulance_support;
    BOOLEAN  is_fire_brigade_support;
    BOOLEAN  is_marine_guard_support;
    BOOLEAN  is_mountain_rescue_support;
    BOOLEAN  is_man_init_ecall_support;
    BOOLEAN  is_auto_init_ecall_support;
    BOOLEAN  reserve;
} EMG_SERV_CATG_VAL_T;

typedef uint8 CALL_ADDR_ARR_T[ADDR_BCD_MAX_LEN];

typedef struct {
    NUMBER_TYPE_E  number_type;
    NUMBER_PLAN_E  number_plan;
    uint16  num_len;
    CALL_ADDR_ARR_T  party_num;
} CALLED_NUMBER_T;

typedef uint8 CALL_SUB_ADDR_ARR_T[SUB_ADDR_MAX_LEN];

typedef struct {
    SUBADDRESS_TYPE_E  sub_addr_type;
    LENGTH_TYPE_E  odd_even_type;
    uint16  addr_len;
    CALL_SUB_ADDR_ARR_T  sub_num;
} SUBADDR_T;

typedef uint8 SMALL_USER_ARR_T[SMALL_USER_INFO_MAX_LEN];

typedef struct {
    USER_USER_PD_E  user_protocol;
    uint16  info_len;
    SMALL_USER_ARR_T  info;
} SMALL_USER_USER_T;

typedef struct {
    BEARER_CAPABILITY_T  bc;
    BOOLEAN  catg_val_present;
    EMG_SERV_CATG_VAL_T  catg_val;
    CALLED_NUMBER_T  called_num;
    BOOLEAN  called_sub_addr_present;
    SUBADDR_T  called_sub_addr;
    BOOLEAN  user_to_user_present;
    SMALL_USER_USER_T  user_to_user;
    CLIR_E  clir_type;
} SETUP_REQ_VALUE_T;

typedef uint8 USER_ARR_T[USER_INFO_MAX_LEN];

typedef struct {
    USER_USER_PD_E  user_protocol;
    uint16  info_len;
    USER_ARR_T  info;
} USER_USER_T;

typedef struct {
    BOOLEAN  conn_sub_addr_present;
    BOOLEAN  user_to_user_present;
    SUBADDR_T  conn_subaddr;
    USER_USER_T  user_to_user;
} SETUP_RSP_VALUE_T;

typedef struct {
    NUMBER_TYPE_E  number_type;
    NUMBER_PLAN_E  number_plan;
    PRESENT_IND_E  present_ind;
    SCREEN_IND_E  screen_ind;
    uint16  num_len;
    CALL_ADDR_ARR_T  party_num;
    uint8            calling_uri[NUMBER_STRING_MAX_LEN + 1];
} CALLING_NUMBER_T;

typedef uint8 LLC_MSG_ARR_T[LLC_MSG_LEN];

typedef struct {
    uint16  msg_len;
    LLC_MSG_ARR_T  llc_msg;
} LLC_MSG_T;

typedef uint8 HLC_MSG_ARR_T[HLC_MSG_LEN];

typedef struct {
    uint16  msg_len;
    HLC_MSG_ARR_T  hlc_msg;
} HLC_MSG_T;

typedef struct {
    CODE_STD_E  code_std;
    LOCATION_E  local;
    PROG_DES_E  descrip;
} PROGRESS_IND_T;

typedef struct {
    SIGNAL_VALUE_E  value;
} SIGNAL_T;

typedef uint8 BACKUP_BEARER_CAPABILITY_BUF_T[BACKUP_BEARER_CAPABILITY_LEN];

typedef struct {
    uint16  bc_len;
    BACKUP_BEARER_CAPABILITY_BUF_T  bc_buf;
} BACKUP_BEARER_CAPABILITY_T;

typedef struct {
    BOOLEAN  bc1_present;
    BOOLEAN  bc2_present;
    BOOLEAN  calling_subaddr_present;
    BOOLEAN  called_subaddr_present;
    BOOLEAN  redirect_subaddr_present;
    BOOLEAN  calling_num_present;
    BOOLEAN  called_num_present;
    BOOLEAN  redirect_num_present;
    BOOLEAN  llc_mode1_present;
    BOOLEAN  llc_mode2_present;
    BOOLEAN  hlc_mode1_present;
    BOOLEAN  hlc_mode2_present;
    BOOLEAN  user_to_user_present;
    BOOLEAN  ss_version_present;
    BOOLEAN  progress_present;
    BOOLEAN  prior_present;
    BOOLEAN  alert_pattern_present;
    BOOLEAN  signal_info_present;
    BOOLEAN  multicall_is_supported;
    BOOLEAN  no_clir_present;
    BOOLEAN  backup_bc_present;
    RE_INDICATION_E  repeat_ind;
    BEARER_CAPABILITY_T  bc1;
    BEARER_CAPABILITY_T  bc2;
    SUBADDR_T  calling_subaddr;
    SUBADDR_T  called_subaddr;
    CALLING_NUMBER_T  calling_num;
    CALLED_NUMBER_T  called_num;
    CALLING_NUMBER_T  redirect_num;
    SUBADDR_T  redirect_subaddr;
    LLC_MSG_T  llc_mode1;
    HLC_MSG_T  hlc_mode1;
    LLC_MSG_T  llc_mode2;
    HLC_MSG_T  hlc_mode2;
    SMALL_USER_USER_T  user_to_user;
    uint8  ss_version;
    CLIR_E  clir;
    PROGRESS_IND_T  progress;
    CALL_PRIOR_E  prior;
    ALERT_PATTERN_E  alert_pattern;
    SIGNAL_T  signal_info;
    NO_CLIR_CAUSE_E  no_clir_cause;
    BACKUP_BEARER_CAPABILITY_T  backup_bc;
} SETUP_IND_VALUE_T;

typedef struct {
    BOOLEAN  progress_present;
    BOOLEAN  connect_num_present;
    BOOLEAN  conn_sub_addr_present;
    BOOLEAN  user_to_user_present;
    PROGRESS_IND_T  progress;
    CALLING_NUMBER_T  conn_addr;
    SUBADDR_T  conn_sub_addr;
    USER_USER_T  user_to_user;
} SETUP_CNF_VALUE_T;

typedef struct {
    LOCATION_E  local;
    GSM_CAUSE_T  cause_v;
} CAUSE_T;

typedef struct {
    CAUSE_T  cause;
    BOOLEAN  user_to_user_present;
    USER_USER_T  user_to_user;
} REJ_REQ_VALUE_T;

typedef struct {
    CAUSE_T  cause;
} REJ_IND_VALUE_T;

typedef struct {
    BEARER_CAPABILITY_TYPE_E  bc_type;
    BLOCK_PTR_T  classmark_ptr;
} BEARER_CAPABILITY_PTR_T;

typedef struct {
    BOOLEAN  bc_present;
    BEARER_CAPABILITY_PTR_T  bc;
    BOOLEAN  bc2_present;
    BEARER_CAPABILITY_PTR_T  bc2;
    RE_INDICATION_E  bc_repeat_mode;
    BOOLEAN  cause_present;
    CAUSE_T  cause;
} CALL_CONF_REQ_VALUE_T;

typedef struct {
    BOOLEAN  bc1_present;
    BOOLEAN  bc2_present;
    BOOLEAN  progress_present;
    BOOLEAN  prior_present;
    BOOLEAN  multicall_is_supported;
    RE_INDICATION_E  repeat_ind;
    BEARER_CAPABILITY_T  bc1;
    BEARER_CAPABILITY_T  bc2;
    PROGRESS_IND_T  progress;
    CALL_PRIOR_E  prior;
} CALL_PROCEEDING_IND_VALUE_T;

typedef struct {
    PROGRESS_IND_T  progress;
    BOOLEAN  user_to_user_present;
    USER_USER_T  user_to_user;
} CALL_PROGRESS_IND_VALUE_T;

typedef struct {
    BOOLEAN  user_to_user_present;
    USER_USER_T  user_to_user;
} ALERT_REQ_VALUE_T;

typedef struct {
    BOOLEAN  progress_present;
    BOOLEAN  user_to_user_present;
    PROGRESS_IND_T  progress;
    USER_USER_T  user_to_user;
} ALERT_IND_VALUE_T;

typedef NOTIFY_E  NOTIFY_IND_VALUE_T;

typedef struct {
    BOOLEAN  user_to_user_present;
    USER_USER_T  user_to_user;
    CAUSE_T  cause;
} DISC_REQ_VALUE_T;

typedef struct {
    CAUSE_T  cause;
    BOOLEAN  progress_present;
    BOOLEAN  user_to_user_present;
    PROGRESS_IND_T  progress;
    USER_USER_T  user_to_user;
    BOOLEAN  allowed_action_present;
    BOOLEAN  active_ccbs_possible;
} DISC_IND_VALUE_T;

typedef uint8 L3_MESSAGE_ARR_T[MAX_L3_MESSAGE_SIZE];

typedef struct {
    BOOLEAN  time_is_expired;
    BOOLEAN  user_to_user_present;
    USER_USER_T  user_to_user;
    BOOLEAN  facility_present;
    L3_MSG_UNIT_T  facility;
    BOOLEAN  ss_version_present;
    uint8  ss_version;
    CAUSE_T  cause;
} REL_REQ_VALUE_T;

typedef struct {
    BOOLEAN  user_to_user_present;
    CAUSE_T  cause1;
    CAUSE_T  cause2;
    USER_USER_T  user_to_user;
} REL_IND_VALUE_T;

typedef L3_MSG_UNIT_T  FACILITY_T;

typedef struct {
    FACILITY_T  facility;
    BLOCK_PTR_T  classmark_ptr;
    BOOLEAN  ss_version_present;
} FACILITY_REQ_VALUE_T;

typedef struct {
    BOOLEAN  dtmf_present;
    uint8  dtmf_char;
    CAUSE_T  cause;
} START_DTMF_CNF_VALUE_T;

typedef struct {
    BEARER_CAPABILITY_PTR_T  bc;
} MODIFY_REQ_VALUE_T;

typedef struct {
    BEARER_CAPABILITY_T  bc;
    BOOLEAN  cause_present;
    CAUSE_T  cause;
    BOOLEAN  llc_present;
    LLC_MSG_T  llc;
    BOOLEAN  hlc_present;
    HLC_MSG_T  hlc;
    BOOLEAN  rev_callsetup_dir;
    BOOLEAN  immediate_mod_ind;
} MODIFY_CNF_VALUE_T;
typedef struct {
    BEARER_CAPABILITY_T  bc;
    BOOLEAN  llc_present;
    BOOLEAN  hlc_present;
    LLC_MSG_T  llc;
    HLC_MSG_T  hlc;
    BOOLEAN  rev_callsetup_dir;
    BOOLEAN  immediate_mod_ind;
} MODIFY_IND_VALUE_T;

typedef struct {
    BEARER_CAPABILITY_PTR_T  bc;
    BOOLEAN  cause_present;
    CAUSE_T  cause;
} MODIFY_RSP_VALUE_T;

typedef struct {
    BOOLEAN  is_success;
    CAUSE_T  cause;
} HOLD_OR_RETRIEVE_CNF_VALUE_E;

typedef struct {
    HOLD_AUXILIARY_STATE_E  hold_st;
    MPTY_AUXILIARY_STATE_E  mpty_st;
} AUX_STATE_T;

typedef struct {
    BOOLEAN  axu_state_present;
    AUX_STATE_T  axu_state;
} ENQ_STATUS_RSP_VALUE_T;

typedef struct {
    USER_USER_T  user_to_user;
    BOOLEAN  is_more_data;
} USER_INFO_VALUE_T;

typedef struct {
    CONGESTION_LEVEL_E  congestion_level;
    CAUSE_T  cause;
} CONGESTION_CONT_VALUE_T;

typedef uint8 CC_CAPABILITY_BUF_T[CC_CAPABILITY_LEN];

typedef struct {
    uint16  cc_len;
    CC_CAPABILITY_BUF_T  cc_buf;
} CC_CAPABILITY_T;

typedef struct {
    BOOLEAN  bc1_present;
    BOOLEAN  bc2_present;
    BOOLEAN  simple_facility_present;
    BOOLEAN  calling_subaddr_present;
    BOOLEAN  called_num_present;
    BOOLEAN  called_subaddr_present;
    BOOLEAN  llc_mode1_present;
    BOOLEAN  llc_mode2_present;
    BOOLEAN  hlc_mode1_present;
    BOOLEAN  hlc_mode2_present;
    BOOLEAN  user_to_user_present;
    BOOLEAN  ss_version_present;
    BOOLEAN  clir_present;
    BOOLEAN  cc_capacility_present;
    BOOLEAN  advance_facility_present;
    BOOLEAN  stream_id_present;
    BOOLEAN  redial_present;
    RE_INDICATION_E  repeat_ind;
    BEARER_CAPABILITY_T  network_bc1;
    BEARER_CAPABILITY_T  network_bc2;
    BEARER_CAPABILITY_PTR_T  ms_bc1;
    BEARER_CAPABILITY_PTR_T  ms_bc2;
    L3_MSG_UNIT_T  simple_facility;
    SUBADDR_T  calling_subaddr;
    CALLED_NUMBER_T  called_num;
    SUBADDR_T  called_subaddr;
    LLC_MSG_T  llc_mode1;
    HLC_MSG_T  hlc_mode1;
    LLC_MSG_T  llc_mode2;
    HLC_MSG_T  hlc_mode2;
    SMALL_USER_USER_T  user_to_user;
    uint8  ss_version;
    CLIR_E  clir;
    CC_CAPABILITY_T  cc_capability;
    L3_MSG_UNIT_T  advance_facility;
    uint8  stream_id;
} CCBS_SETUP_CONTAINER_VALUE_T;

typedef struct {
    IS_REJECT_RECALL_T  is_reject_recall;
    BOOLEAN  cause_present;
    CAUSE_T  cause;
    CCBS_SETUP_CONTAINER_VALUE_T  ccbs_setup_container;
} CC_ESTABLISHMENT_RSP_VALUE_T;

typedef uint16  BCCH_TC_MASK_T;

typedef struct {
    PLMN_T  plmn;
    BOOLEAN  select_any_plmn;
    BA_LIST_ARR_T  ba_undecoded;
    BA_LIST_ARR_T  bis_ba_undecoded;
    BOOLEAN        is_usim;         //if is usim, then set to TRUE, else set to FALSE
    ARFCN_LIST_T arfcn_list;         //ARFCN
    BOOLEAN  ignore_forbid_plmn_list;
    MS_BAND_E  ms_band;
    BOOLEAN  gprs_requested;
    BOOLEAN  manual_select_plmn;
    BOOLEAN  sel_hplmn;
    BAND_FILTER_T  band_filter;
    uint8  paging_tbf_mask;
} SEL_PARAM_T;

typedef struct {
    SIGNAL_VARS
    uint32  mn_pid;
    uint32  mm_pid;
} yPDef_MNCC_INIT_TASKID_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    GSM_EST_TYPE_E  est_type;
    SETUP_REQ_VALUE_T  setup_req;
} yPDef_MNCC_SETUP_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    SETUP_RSP_VALUE_T  setup_rsp;
} yPDef_MNCC_SETUP_RSP;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    SETUP_IND_VALUE_T  setup_ind;
} yPDef_MNCC_SETUP_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    SETUP_CNF_VALUE_T  setup_cnf;
} yPDef_MNCC_SETUP_CNF;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
} yPDef_MNCC_SETUP_COMPLETE_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    REJ_REQ_VALUE_T  rej_req;
} yPDef_MNCC_REJ_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    REJ_IND_VALUE_T  rej_ind;
} yPDef_MNCC_REJ_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_CONF_REQ_VALUE_T  call_cnf_req;
} yPDef_MNCC_CALL_CONF_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    CALL_PROCEEDING_IND_VALUE_T  call_proceeding_ind;
} yPDef_MNCC_CALL_PROCEEDING_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    CALL_PROGRESS_IND_VALUE_T  call_progress_ind;
} yPDef_MNCC_CALL_PROGRESS_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    ALERT_REQ_VALUE_T  alert_req;
} yPDef_MNCC_ALERT_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    ALERT_IND_VALUE_T  alert_ind;
} yPDef_MNCC_ALERT_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    NOTIFY_E  notify;
} yPDef_MNCC_NOTIFY_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    NOTIFY_IND_VALUE_T  notify_ind;
} yPDef_MNCC_NOTIFY_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    DISC_REQ_VALUE_T  disc_req;
} yPDef_MNCC_DISC_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    DISC_IND_VALUE_T  disc_ind;
} yPDef_MNCC_DISC_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    REL_REQ_VALUE_T  rel_req;
} yPDef_MNCC_REL_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    REL_IND_VALUE_T  rel_ind;
} yPDef_MNCC_REL_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    GSM_CAUSE_T  gsm_cause;
} yPDef_MNCC_ERR_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    FACILITY_REQ_VALUE_T  facility_req;
} yPDef_MNCC_FACILITY_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    L3_MSG_UNIT_T  facility_ind;
} yPDef_MNCC_FACILITY_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    uint8  dtmf_val;
} yPDef_MNCC_START_DTMF_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    START_DTMF_CNF_VALUE_T  start_dtmf_cnf;
} yPDef_MNCC_START_DTMF_CNF;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
} yPDef_MNCC_STOP_DTMF_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
} yPDef_MNCC_STOP_DTMF_CNF;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    MODIFY_REQ_VALUE_T  modify_req;
} yPDef_MNCC_MODIFY_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    MODIFY_CNF_VALUE_T  notify_cnf;
} yPDef_MNCC_MODIFY_CNF;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    MODIFY_IND_VALUE_T  modify_ind;
} yPDef_MNCC_MODIFY_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    MODIFY_RSP_VALUE_T  modify_rsp;
} yPDef_MNCC_MODIFY_RSP;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    SYNC_CAUSE_E  sync_cause;
    CHANNEL_MODE_E  channel_mode;
} yPDef_MNCC_SYNC_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
} yPDef_MNCC_HOLD_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
} yPDef_MNCC_RETRIEVE_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    HOLD_OR_RETRIEVE_CNF_VALUE_E  hold_cnf;
} yPDef_MNCC_HOLD_CNF;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    HOLD_OR_RETRIEVE_CNF_VALUE_E  retrieve_cnf;
} yPDef_MNCC_RETRIEVE_CNF;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
} yPDef_MNCC_ENQ_STATUS_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    ENQ_STATUS_RSP_VALUE_T  enq_status_rsp;
} yPDef_MNCC_ENQ_STATUS_RSP;

typedef struct {
    SIGNAL_VARS
    uint32  Param1;
    uint32  Param2;
    uint32  Param3;
    uint32  Param4;
} yPDef_MNCC_INIT_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    USER_INFO_VALUE_T  user_info;
} yPDef_MNCC_USER_INFO_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    USER_INFO_VALUE_T  user_info;
} yPDef_MNCC_USER_INFO_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CONGESTION_CONT_VALUE_T  congestion_cont;
} yPDef_MNCC_CONGESTION_CONT_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    IS_REJECT_RECALL_T  is_rej_recall;
} yPDef_MNCC_PROMPT_RSP;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    CCBS_SETUP_CONTAINER_VALUE_T  ccbs_setup_container;
} yPDef_MNCC_CC_ESTABLISHMENT_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CC_ESTABLISHMENT_RSP_VALUE_T  cc_est_rsp;
} yPDef_MNCC_CC_ESTABLISHMENT_RSP;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    CALL_STATE_E  call_state;
    L3_MSG_UNIT_T  recall_ind;
} yPDef_MNCC_RECALL_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    IS_REJECT_RECALL_T  is_rej_recall;
    CAUSE_T  rsp_cause;
} yPDef_MNCC_RECALL_RSP;

/**********************MN<->SS related types **********************/

typedef struct {
    SIGNAL_VARS
    uint32  mn_pid;
    uint32  mm_pid;
} yPDef_MNSS_INIT_TASKID_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    FACILITY_T  facility;
    SS_VERSION_PRESENT_T  is_version_present;
    SS_VERSION_VALUE_T  version_value;
} yPDef_MNSS_BEGIN_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    FACILITY_T  facility;
} yPDef_MNSS_BEGIN_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    FACILITY_T  facility;
} yPDef_MNSS_FACILITY_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    FACILITY_T  facility;
} yPDef_MNSS_FACILITY_IND;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    BOOLEAN  is_cause_present;
    CAUSE_T  cause;
    BOOLEAN  is_facility_present;
    FACILITY_T  facility;
} yPDef_MNSS_END_REQ;

typedef struct {
    SIGNAL_VARS
    PD_TI_REF_T  pd_ti;
    BOOLEAN  is_cause_present;
    CAUSE_T  cause;
    BOOLEAN  is_facility_present;
    FACILITY_T  facility;
    SS_LOW_LAYER_ERROR_E  low_layer_err;
} yPDef_MNSS_END_IND;


/********************** MN<->REG related types **********************/
typedef uint8 BA_LIST_ARR_T[16];

typedef struct {
    PLMN_T  rplmn;
    BA_LIST_ARR_T  balist;
    BOOLEAN  sel_any_plmn;
    MS_BAND_E  ms_band;
    BA_LIST_ARR_T  bis_ba_undecoded;
    BOOLEAN  sel_hplmn;
    BAND_FILTER_T  band_filter;
} MN_REG_PAR;

typedef BOOLEAN  IS_MS_SUPPORT_CS_T;

typedef BOOLEAN  IS_MS_SUPPORT_PS_T;

typedef BOOLEAN  IS_AUTO_PLMN_SEL_T;

typedef BOOLEAN  IS_FORBID_ROAMING_T;

typedef BOOLEAN  IS_PHONE_CTRL_PLMN_T;

typedef BOOLEAN  BA_IS_VALID_T;

typedef struct {
    NETWORK_MODE_E  network_mode;
    PLMN_T  plmn;
    BOOLEAN  gprs_support;
    RAI_T  rai;
    CELL_ID_T  cellid;
    BA_LIST_ARR_T  bis_ba_undecoded;
    BA_IS_VALID_T  bis_ba_valid;
} CURRENT_CELL_INFO_T;

typedef PLMN_T PLMN_LONG_ARR_T[LONG_PLMN_SIZE];

typedef struct {
    uint8  length;
    PLMN_LONG_ARR_T  plmn_arr;
} PLMN_LONG_LIST_T;

typedef uint8  T3212_VALUE;

typedef uint8  HPLMN_TIMER_VALUE_T;

typedef struct {
    SIGNAL_VARS
    CKSN_T  cksn;
    KC_T  kc;
} yPDef_MN_MM_UPDATE_KC_REQ;

typedef struct {
    SIGNAL_VARS
    MS_ATTACH_TYPE_E  attach_type;
    PLMN_T  plmn;
} yPDef_MN_MM_REG_REQ;

typedef struct {
    SIGNAL_VARS
    PLMN_T  plmn;
    HANDSET_STATE_E  handset_state;
} yPDef_MN_MM_REG_CNF;

typedef struct {
    SIGNAL_VARS
    HANDSET_STATE_E  handset_state;
    PLMN_T  plmn;
} yPDef_MN_MM_REG_IND;

typedef struct {
    SIGNAL_VARS
    MN_MM_NREG_CAUSE_E  nreg_cause;
} yPDef_MN_MM_NREG_REQ;

typedef struct {
    SIGNAL_VARS
    HANDSET_STATE_E  handset_state;
} yPDef_MN_MM_NREG_CNF;

typedef struct {
    SIGNAL_VARS
    MN_MM_NREG_IND_CAUSE_E  nreg_ind_cause;
    HANDSET_STATE_E  handset_state;
    PLMN_T  plmn;
    uint8   network_rej_cause;
} yPDef_MN_MM_NREG_IND;

typedef struct {
    SIGNAL_VARS
    AVAILABLE_PLMN_LA_LIST_T  plmn_la_list;
    BOOLEAN  is_report;
} yPDef_MN_MM_SCAN_CNF;

typedef struct {
    SIGNAL_VARS
    MN_MM_REG_CAUSE_E  reg_cause;
    MN_REG_PAR  reg_param;
    IS_MS_SUPPORT_CS_T  is_support_cs;
    IS_MS_SUPPORT_PS_T  is_support_ps;
    IS_AUTO_PLMN_SEL_T  is_auto_sel_mode;
    IS_FORBID_ROAMING_T  is_forbid_roaming;
    IS_PHONE_CTRL_PLMN_T  is_phone_ctrl_plmn;
    PLMN_T  plmn;
    MS_MODE_T  ms_mode;
    uint8  campon_ref;
    uint8  paging_tbf_mask;
    BOOLEAN is_afc_adjust_flag;
} yPDef_MN_MM_CAMPON_REQ;

typedef struct {
    SIGNAL_VARS
    RR_ACT_TYPE_E  rr_act_type;
    CURRENT_CELL_INFO_T  curr_cell_info;
    uint8  campon_ref;
} yPDef_MN_MM_CAMPON_CNF;

typedef struct {
    SIGNAL_VARS
    BLOCK_PTR_T  classmark_ptr;
    IMEI_T  imei;
    IMEISV_T  imeisv;
} yPDef_MN_MM_CLASSMARK_REQ;

typedef struct {
    SIGNAL_VARS
    IMSI_T  imsi;
    PLMN_T  plmn;
    PLMN_LIST_T  plmn_lst;
    BOOLEAN  is_ehplmn_present;
    PLMN_LONG_LIST_T  ehplmn_list;
    TMSI_T  tmsi;
    LAI_T  lai;
    MM_UPDATE_STATUS_E  mm_update_status;
    T3212_VALUE  t3212_val;
    HPLMN_TIMER_VALUE_T  hplmn_servch_timer;
    ACC_CONTROL_CLASS_T  acc_ctrl_class;
    BOOLEAN  is_net_lock_present;
    NET_LOCK_PLMN_LIST_T net_lock_plmn_list;
    PHONE_SAVE_POWER_LOCK_ARFCN_T save_power_lock_arfcn;
} yPDef_MN_MM_INFO_REQ;

typedef struct {
    SIGNAL_VARS
    FPLMN_LONG_LIST_T  fplmn_lst;
} yPDef_MN_MM_UPDATE_FPLMN_LIST_REQ;


/********************** MN<->MM related types **********************/
typedef uint8 NETWORK_NAME_ARR_T[NETWORK_NAME_ARR_LEN];

typedef struct {
    CODE_SCHEME_E  code_scheme;
    BOOLEAN  is_add_ci;
    uint16  length;
    NETWORK_NAME_ARR_T  name;
} NETWORK_NAME_T;

typedef struct {
    uint8  year;
    uint8  month;
    uint8  day;
    uint8  hour;
    uint8  minute;
    uint8  second;
    int32  time_zone;
} UT_LT_ZONE_T;

typedef struct {
    uint8  value;
} DAYLIGHT_SAVING_TIME_T;

typedef struct {
    BOOLEAN  full_name_for_network_flag;
    BOOLEAN  short_name_for_network_flag;
    BOOLEAN  local_time_zone_flag;
    BOOLEAN  ut_lt_zone_flag;
    BOOLEAN  lsa_identity_flag;
    BOOLEAN  daylight_saving_time_flag;
    NETWORK_NAME_T  full_network_name;
    NETWORK_NAME_T  short_network_name;
    LOCAL_TIME_ZONE_T  local_time_zone;
    UT_LT_ZONE_T  ut_lt_zone;
    LSA_IDENTITY_T  lsa_identity;
    DAYLIGHT_SAVING_TIME_T  daylight_saving_time;
} MMGMM_INFORMATION_T;

typedef struct {
    EMG_SERV_CATG_VAL_T  emg_serv_catg_val;
    uint16  emg_num_len;
    CALL_ADDR_ARR_T  emg_num;
} EMG_NUM_T;

typedef EMG_NUM_T EMG_NUM_ARR_T[MAX_EMG_NUM_LIST_SIZE];

typedef struct {
    int32  emg_num_list_num;
    EMG_NUM_ARR_T  emg_num_arr;
} EMG_NUM_LIST_T;

typedef struct {
    SIGNAL_VARS
    uint32  mn_pid;
    uint32  sim_pid;
    uint32  rr_pid;
    uint32  cc_pid;
    uint32  smc_pid;
    uint32  ss_pid;
    uint32  gmm_pid;
    uint32  mm_pid;
} yPDef_MN_MM_INIT_REQ;

typedef struct {
    SIGNAL_VARS
    PLMN_T  plmn;
    MMGMM_INFORMATION_T  mmgmm_info;
} yPDef_MN_MM_INFORMATION_IND;

typedef struct {
    SIGNAL_VARS
    MCC_T  mcc;
    EMG_NUM_LIST_T  emg_num_lst;
} yPDef_MN_MM_EMG_NUM_LIST_IND;

typedef struct {
    SIGNAL_VARS
    PLMN_LIST_T  eplmn_lst;
} yPDef_MN_MM_EPLMN_LIST_IND;

typedef struct {
    SIGNAL_VARS
    uint16  arfcn;
    BOOLEAN bar_current_cell; 
    uint16 bar_time; 
} yPDef_MN_MM_SWITCH_ARFCN_REQ;

typedef struct {
    SIGNAL_VARS
    uint16  arfcn;
    BOOLEAN is_ok;
} yPDef_MN_MM_SWITCH_ARFCN_CNF;

/********************** MN<->SM related types **********************/

typedef struct {
    RELIABILITY_CLASS_T  reliability;
    PEAK_THROUGHPUT_CLASS_T  peak_thrput;
    DELAY_CLASS_T  delay_class;
    PRECEDENCE_CLASS_T  precedence;
    MEAN_THROUGHPUT_CLASS_T  mean_thrput;
    BOOLEAN  is_r99;
    DELIVERY_OF_ERR_SDU_T  delivery_of_err_sdu;
    DELIVERY_ORDER_T  delivery_order;
    TRAFFIC_CLASS_T  traffic_class;
    MAX_SDU_SIZE_T  max_sdu_size;
    MAX_BIT_RATE_FOR_UPLINK_T  max_bit_rate_for_uplink;
    MAX_BIT_RATE_FOR_DOWNLINK_T  max_bit_rate_for_downlink;
    RESIDUAL_BER_T  residual_ber;
    SDU_ERR_RATIO_T  sdu_err_ratio;
    TRAFFIC_HANDLING_PRIO_T  traffic_handling_prio;
    TRAFFIC_HANDING_PRIO_T  transfer_delay;
    GUARANTEED_BIT_RATE_FOR_UPLINK_T  guaranteed_bit_rate_for_uplink;
    GUARANTEED_BIT_RATE_FOR_DOWNLINK_T  guaranteed_bit_rate_for_downlink;
} TOTAL_QOS_T;

#define OCTET_ARR20_LENGTH 20

typedef uint8 OCTET_ARR20_T[OCTET_ARR20_LENGTH];

typedef struct {
    PDP_ADDR_TYPE_E  pdp_addr_type;
    OCTET_ARR20_T  pdp_addr_value;
    uint16  pdp_addr_length;
} PDP_ADDRESS_T;

typedef uint8 OCTET_ARR_APN_T[APN_LENGTH];

typedef struct {
    OCTET_ARR_APN_T  apn_value;
    uint16  apn_length;
} APN_T;

typedef uint8 OCTET_ARR251_T[251];

typedef struct {
    OCTET_ARR251_T  pco_value;
    uint16  pco_length;
} PCO_T;

typedef NSAPI_NUM_T NSAPI_ARR_T[MAX_NSAPI_NUMBER];

typedef struct {
    int16  length;
    NSAPI_ARR_T  nsapi_arr;
} NSAPI_LIST_T;

typedef BOOLEAN  TEAR_DOWN_ID_T;

typedef uint8 OCTET_ARR254_T[254];

typedef struct {
    OCTET_ARR254_T  packet_filter_list;
    uint8  tft_length;
    uint8  num_packet_filter;
    TFT_OPCODE_E  tft_opcode;
} TRAFFIC_FLOW_TEMPLATE_T;

typedef struct {
    OCTET_PTR_T  pdu_ptr;
    uint16  pdu_size;
} PDU_STRUCT_T;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    TOTAL_QOS_T  qos;
    PDP_ADDRESS_T  pdp_addr;
    APN_T  apn;
    PCO_T  pco;
} yPDef_SMREG_PDP_ACTIVATE_REQ;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    TOTAL_QOS_T  qos;
    PDP_ADDRESS_T  pdp_addr;
    PCO_T  pco;
    SM_CAUSE_T     sm_cause_2;  
} yPDef_SMREG_PDP_ACTIVATE_CNF;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    SM_CAUSE_T  sm_cause;
    PCO_T  pco;
} yPDef_SMREG_PDP_ACTIVATE_REJ;

typedef struct {
    SIGNAL_VARS
    PDP_ADDRESS_T  pdp_addr;
    APN_T  apn;
    NSAPI_NUM_T  nsapi;
    PCO_T  pco;
} yPDef_SMREG_PDP_ACTIVATE_IND;

typedef struct {
    SIGNAL_VARS
    SM_CAUSE_T  sm_cause;
    PDP_ADDRESS_T  pdp_addr;
    APN_T  apn;
} yPDef_SMREG_PDP_ACTIVATE_REJ_RSP;

typedef struct {
    SIGNAL_VARS
    SM_CAUSE_T  sm_cause;
    NSAPI_LIST_T  nsapi_lst;
    TEAR_DOWN_ID_T  is_tear_down;
} yPDef_SMREG_PDP_DEACTIVATE_REQ;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
} yPDef_SMREG_PDP_DEACTIVATE_CNF;

typedef struct {
    SIGNAL_VARS
    SM_CAUSE_T  sm_cause;
    NSAPI_NUM_T  nsapi;
    TEAR_DOWN_ID_T  is_tear_down;
} yPDef_SMREG_PDP_DEACTIVATE_IND;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    TOTAL_QOS_T  qos;
    PDP_ADDRESS_T  pdp_addr;
} yPDef_SMREG_PDP_MODIFY_IND;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    TOTAL_QOS_T  qos;
    TRAFFIC_FLOW_TEMPLATE_T  tft;
} yPDef_SMREG_PDP_MODIFY_REQ;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    TOTAL_QOS_T  qos;
} yPDef_SMREG_PDP_MODIFY_CNF;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    SM_CAUSE_T  sm_cause;
} yPDef_SMREG_PDP_MODIFY_REJ;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  primary_nsapi;
    NSAPI_NUM_T  nsapi;
    TOTAL_QOS_T  qos;
    TRAFFIC_FLOW_TEMPLATE_T  tft;
    TOTAL_QOS_T  min_qos;
    BOOLEAN  min_qos_flag;
} yPDef_SMREG_PDP_ACTIVATE_SEC_REQ;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    TOTAL_QOS_T  qos;
    uint32 pcid;
} yPDef_SMREG_PDP_ACTIVATE_SEC_CNF;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    SM_CAUSE_T  sm_cause;
} yPDef_SMREG_PDP_ACTIVATE_SEC_REJ;

typedef struct {
    SIGNAL_VARS
    RESPONSE_MODE_E  rsp_mode;
} yPDef_SMREG_PDP_INFO_REQ;

typedef struct {
    SIGNAL_VARS
    PDP_ADDRESS_T  pdp_addr;
    APN_T  apn;
} yPDef_SMREG_PDP_MTACT_IND;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    TOTAL_QOS_T  qos;
    PDP_ADDRESS_T  pdp_addr;
    APN_T  apn;
    PCO_T  pco;
    BOOLEAN  is_accept;
} yPDef_SMREG_PDP_MTACT_RES;

typedef struct {
    SIGNAL_VARS
    BLOCK_PTR_T  data_ind_ptr;
    int32  len;
} yPDef_SNDCREG_PUSH_DATA_IND;

typedef struct {
    SIGNAL_VARS
    uint32  mn_pid;
    uint32  gmm_pid;
} yPDef_SMREG_INIT_REQ;

typedef struct {
    SIGNAL_VARS
    int16  dual_sys;
    NSAPI_NUM_T  nsapi;
} yPDef_SMREG_PDP_FORCE_DEACTIVATE_REQ;


/**********************MN<->GMM related types **********************/
typedef struct {
    SIGNAL_VARS
    GMM_ATTACH_TYPE_T  mn_attach_type;
    MS_BAND_E  ms_band;
} yPDef_APP_GMM_ATTACH_REQ;

typedef struct {
    SIGNAL_VARS
    GMM_MN_ATTCH_RESULT_T  attach_result;
} yPDef_GMM_APP_ATTACH_CNF;

typedef struct {
    SIGNAL_VARS
    GMM_MN_ATTCH_RESULT_T  attach_result;
} yPDef_GMM_APP_ATTACH_REJ;

typedef struct {
    SIGNAL_VARS
    GMM_DETACH_TYPE_E  detach_type;
    BOOLEAN  is_power_off;
} yPDef_APP_GMM_DETACH_REQ;

typedef struct {
    SIGNAL_VARS
    GMM_DETACH_TYPE_E  detach_type;
    BOOLEAN  is_power_off;
} yPDef_APP_GMM_FORCE_DETACH_REQ;

typedef struct {
    SIGNAL_VARS
    GMM_MN_DETACH_RESULT_T  detach_result;
} yPDef_GMM_APP_DETACH_CNF;

typedef struct {
    SIGNAL_VARS
    GMM_MN_DETACH_RESULT_T  detach_result;
} yPDef_GMM_APP_DETACH_IND;

typedef struct {
    SIGNAL_VARS
    BOOLEAN  is_plmn_change;
    NETWORK_MODE_E  network_mode;
    RAI_T  current_rai;
    CELL_ID_T  cell_id;
} yPDef_APP_GMM_CELL_INFO_NOTIFY;

typedef struct {
    SIGNAL_VARS
    PLMN_T  cur_plmn;
    MMGMM_INFORMATION_T  mmgmm_info;
} yPDef_GMM_APP_INFORMATION_IND;

typedef struct {
    SIGNAL_VARS
    BOOLEAN  is_sim_exist;
} yPDef_APP_GMM_IMSI_INFO_NOTIFY;

typedef struct {
    SIGNAL_VARS
    PLMN_T  plmn;
} yPDef_APP_GMM_UPDATE_FPLMN_LIST_REQ;

typedef struct {
    SIGNAL_VARS
    uint32  mn_pid;
    uint32  mm_pid;
    uint32  sim_pid;
    uint32  rr_pid;
    uint32  sms_pid;
    uint32  Param6;
} yPDef_APP_GMM_INIT_REQ;

typedef struct {
    SIGNAL_VARS
    MCC_T  mcc;
    EMG_NUM_LIST_T  emg_num_list;
} yPDef_GMM_APP_EMG_NUM_LIST_IND;

typedef struct {
    SIGNAL_VARS
    PLMN_LIST_T  plmn_list;
} yPDef_GMM_APP_EPLMN_LIST_IND;


/**********************MN<->LLC related types **********************/
typedef struct {
    BOOLEAN  version_flag;
    BOOLEAN  iov_i_flag;
    BOOLEAN  t200_flag;
    BOOLEAN  n200_flag;
    BOOLEAN  n201_u_flag;
    BOOLEAN  n201_i_flag;
    BOOLEAN  md_flag;
    BOOLEAN  mu_flag;
    BOOLEAN  kd_flag;
    BOOLEAN  ku_flag;
    uint32  iov_i;
    uint16  t200;
    uint16  n201_u;
    uint16  n201_i;
    uint16  md;
    uint16  mu;
    uint8  version;
    uint8  kd;
    uint8  ku;
    uint8  n200;
} LLC_XID_PARAM_T;

typedef struct {
    SIGNAL_VARS
    SAPI_NUM_T  sapi;
    LLC_XID_PARAM_T  llc_xid_param;
} yPDef_LLC_SET_XID_PARAM_REQ;


/**********************MN<->SNDCP related types **********************/
typedef struct {
    ATC_PC_TYPE_E  h_comp;
    ATC_DC_TYPE_E  d_comp;
} COMPRESS_INFO_T;

typedef struct {
    SIGNAL_VARS
    NPDU_PTR_T  data_ptr;
    NPDU_SIZE_T  data_size;
    NSAPI_NUM_T  nsapi;
} yPDef_SN_DATA_REQ;

typedef struct {
    SIGNAL_VARS
    NPDU_PTR_T  data_ptr;
    NPDU_SIZE_T  data_size;
    NSAPI_NUM_T  nsapi;
} yPDef_SN_DATA_IND;

typedef struct {
    SIGNAL_VARS
    NPDU_PTR_T  data_ptr;
    NPDU_SIZE_T  data_size;
    NSAPI_NUM_T  nsapi;
} yPDef_SN_UNITDATA_REQ;

typedef struct {
    SIGNAL_VARS
    NPDU_PTR_T  data_ptr;
    NPDU_SIZE_T  data_size;
    NSAPI_NUM_T  nsapi;
} yPDef_SN_UNITDATA_IND;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    COMPRESS_INFO_T  compress_info;
} yPDef_SN_XID_REQ;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    XID_BLOCK_PTR_T  data_ptr;
    XID_BLOCK_SIZE_T  data_size;
} yPDef_SN_XID_IND;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    XID_BLOCK_PTR_T  data_ptr;
    XID_BLOCK_SIZE_T  data_size;
} yPDef_SN_XID_RES;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
    XID_BLOCK_PTR_T  data_ptr;
    XID_BLOCK_SIZE_T  data_size;
} yPDef_SN_XID_CNF;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
} yPDef_SN_SUSPENDSENDDATA;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
} yPDef_SN_RESUMESENDDATA;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
} yPDef_SN_SUSPENDRECEIVEDATA;

typedef struct {
    SIGNAL_VARS
    NSAPI_NUM_T  nsapi;
} yPDef_SN_RESUMERECEIVEDATA;


/**********************MN<->SMS related types **********************/
typedef struct {
    SMS_CAUSE_TYPE_T  sor;
    GSM_CAUSE_T  cs;
    L3_MSG_UNIT_T  tl_rl_msg_t;
} SMS_TL_RL_REPORT_T;

/********************** MN<->RR related types **********************/
typedef struct {
    SIGNAL_VARS
    RX_EX_T  rx_ex_req;
} yPDef_MN_RR_RX_EX_REQ;

typedef struct {
    SIGNAL_VARS
    BLOCK_ID  mn_pid;
    BLOCK_ID  mm_pid;
    BLOCK_ID  gmm_pid;
} yPDef_MN_RR_INIT_REQ;

typedef struct {
    SIGNAL_VARS
    NCELL_NUM_T  ncell_num;
    NCELL_MEAS_REPORT_T  ncell_meas_rep;
} yPDef_RR_MN_NCELL_RSSI_CNF;

typedef struct {
    SIGNAL_VARS
    CELL_MEAS_T  scell_meas_rep;
    BOOLEAN is_traffic;
    uint8 rx_qual;
	uint32 cdac; 
    BOOLEAN is_paging_trigger;
} yPDef_RR_MN_SCELL_RSSI_IND;

/**********************MN<->RRA related types **********************/
typedef struct {
    SIGNAL_VARS
    CARD_MASK_T  req_rr;
} yPDef_RRA_MAC_GPRS_PARAM_REQ;

typedef struct {
    SIGNAL_VARS
    BOOLEAN sim1_present;
    BOOLEAN sim2_present;
    BOOLEAN sim3_present;
    BOOLEAN sim4_present;
} yPDef_RRA_SIM_NUM_SET_REQ;

typedef struct {
    SIGNAL_VARS
    CARD_MASK_T  card_mask;
} yPDef_RRA_SMSCB_START_REQ;

typedef struct {
    SIGNAL_VARS
    CARD_MASK_T  card_mask;
} yPDef_RRA_SMSCB_STOP_REQ;

typedef struct {
    SIGNAL_VARS
    CARD_MASK_T  card_mask;
    SMSCB_SCHEDULE_T smscb_schedule;
} yPDef_RRA_SMSCB_SCHEDULE_REQ;

typedef struct {
    SIGNAL_VARS
    CARD_MASK_T  card_mask;
} yPDef_RRA_SMSCB_USELESS_MSG_REQ;

typedef struct {
    SIGNAL_VARS
    SMSCB_BLOCK_ARR_T smscb_data;
    CARD_MASK_T  card_mask;
} yPDef_RRA_SMSCB_DATA_IND;

/**********************MN<->SCT related types **********************/
typedef struct {
    SIGNAL_VARS
    BOOLEAN      loop_on_off;
    LOOP_LIST_E  loop;
} SCT_MN_TCH_LOOP_IND_MSG;
#endif
