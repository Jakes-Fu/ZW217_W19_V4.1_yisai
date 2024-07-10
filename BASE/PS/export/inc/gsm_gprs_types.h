/******************************************************************************
 ** File Name:      gsm_gprs_types.h                                       *
 ** Author:         Feng.shi                                                  *
 ** Date:           2012-10-04                                                *
 ** Copyright:      Copyright 2001-2012 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2012-10-04     Feng.shi      Create.                                      *
 ******************************************************************************/
#ifndef _GSM_GPRS_TYPES_H
#define _GSM_GPRS_TYPES_H

/**************************************************************
*                           MACRO                             *
***************************************************************/ 
#define ALL_NSAPI    0
#define EGPRS_CLASS2 0
#define EGPRS_CLASS1 1
#define SAPI_0 0
#define SAPI_1 1
#define SAPI_3 3
#define SAPI_5 5
#define SAPI_7 7
#define SAPI_9 9
#define SAPI_11 11

#define DYNAMIC_ALLOCATION 0
#define EXTENDED_DYNAMIC 1
#define SINGLE_BLOCK_ALLOCATION 2
#define SINGLE_BLOCK_WITHOUT_TBF 3
#define MULTI_BLOCK_ALLOCATION 4

#define TLLI_ALL1 0xFFFFFFFF
#define LLC_DATA 0
#define LLC_UNITDATA 1
#define QOS_RELIABILITY_CLASS_SUBSCRIBED 0
#define QOS_RELIABILITY_ACK_GTP_LLC 1
#define QOS_RELIABILITY_UNACK_GTP_ACK_LLC 2
#define QOS_RELIABILITY_UNACK_LLC_ACK_RLC 3
#define QOS_RELIABILITY_UNACK_RLC_PROTEST 4
#define QOS_RELIABILITY_UNACK_RLC_UNPROTEST 5
#define QOS_RELIABILITY_SIX 6
#define QOS_RELIABILITY_CLASS_RESERVED 7
#define QOS_PEAK_THROUGHPUT_SUBSCRIBED 0
#define QOS_PEAK_THROUGHPUT_RESERVED 15
#define QOS_MEAN_THROUGHPUT_SUBSCRIBED 0
#define QOS_MEAN_THROUGHPUT_RESERVED 30
#define QOS_MEAN_THROUGHPUT_BESTEFFORT 31
#define QOS_DELAY_CLASS_SUBSCRIBED 0
#define QOS_DELAY_CLASS_RESERVED 7
#define QOS_PRECEDENCE_CLASS_SUBSCRIBED 0
#define QOS_PRECEDENCE_CLASS_RESERVED 7
#define QOS_DELIVERY_OF_ERR_SDU_SUBSCRIBED 0
#define QOS_DELIVERY_OF_ERR_SDU_RESERVED 7
#define QOS_DELIVERY_ORDER_SUBSCRIBED 0
#define QOS_DELIVERY_ORDER_RESERVED 3
#define QOS_TRAFFIC_CLASS_SUBSCRIBED 0
#define QOS_TRAFFIC_CLASS_RESERVED 7
#define QOS_MAX_SDU_SIZE_SUBSCRIBED 0
#define QOS_MAX_SDU_SIZE_RESERVED 255
#define QOS_MAX_BIT_RATE_FOR_UPLINK_SUBSCRIBED 0
#define QOS_MAX_BIT_RATE_FOR_DOWNLINK_SUBSCRIBED 0
#define QOS_RESIDUAL_BER_SUBSCRIBED 0
#define QOS_RESIDUAL_BER_RESERVED 15
#define QOS_SDU_ERR_RATIO_SUBSCRIBED 0
#define QOS_SDU_ERR_RATIO_RESERVED 15
#define QOS_TRAFFIC_HANDLING_PRIO_SUBSCRIBED 0
#define QOS_TRANSFER_DELAY_SUBSCRIBED 0
#define QOS_TRANSFER_DELAY_RESERVED 63
#define QOS_GUARANTEED_BIT_RATE_FOR_UPLINK_SUBSCRIBED 0
#define QOS_GUARANTEED_BIT_RATE_FOR_DOWNLINK_SUBSCRIBED 0
#define SUPPORT_SHORT_L2_HEADER FALSE
#define MAX_L3_MESSAGE_SIZE 251
#define MAX_N201_U 1520
#define MIN_N201_U 140
#define MAX_N201_I 1520
#define MIN_N201_I 140
#define GMM_CIPHER_NONE 0
#define GMM_CIPHER_NEED 1
#define GMM_CIPHER_START 2
#define GMM_CIPHER_ENDED 3

#define DECODE_SUCCESS 0
#define INVALID_MESSAGE_TYPE 1
#define MESSAGE_TOO_SHORT 2
#define INVALID_TRANSACTION_IDENTIFIER 3
#define SKIP_INDICATOR_NOT_ZERO 4
#define IE_SEQUENCE_ERROR 5
#define DUPLICATED_IE 6
#define UNRECOGNISED_IE_COMP_REQ 7
#define UNRECOGNISED_IE 8
#define OPTIONAL_IE_CONTENT_ERROR 9
#define MANDATORY_IE_CONTENT_ERROR 10
#define MANDATORY_IE_MISSING 11
#define INVALID_PROTOCOL_DISCRIMINATOR 12
#define CONDITIONAL_IE_ERROR 13
#define INVALID_PARAMETER 14
#define INVALID_MANDATORY_IE 15
#define MESSAGE_LEN_ERROR 16
#define CAUSE_NONE_VALUE 0
#define CAUSE_UNALLOCATED_TMSI 1
#define CAUSE_IMSI_UNKNOWN_IN_HLR 2
#define CAUSE_ILLEGAL_MS 3
#define CAUSE_IMSI_UNKNOWN_IN_VLR 4
#define CAUSE_IMEI_NOT_ACCEPTED 5
#define CAUSE_ILLEGAL_ME 6
#define CAUSE_PLMN_NOT_ALLOWED 11
#define CAUSE_LA_NOT_ALLOWED 12
#define CAUSE_ROAMING_NOT_ALLOWED 13
#define CAUSE_PLMN_GPRS_NOT_ALLOW 14
#define CAUSE_LA_NO_SUITABLE_CELL 15
#define CAUSE_NETWORK_FAILURE 17
#define CAUSE_MAC_FAILURE 20
#define CAUSE_SYNCH_FAILURE 21
#define CAUSE_CONGESTION 22
#define CAUSE_GSM_AUTH_UNACCEPT 23
#define CAUSE_SERV_OPT_NOT_SUPPORTED 32
#define CAUSE_SERV_OPT_NOT_SUBSCRIBED 33
#define CAUSE_SERV_OPT_TEMP_OUT_OF_ORDER 34
#define CAUSE_CALL_CANNOT_BE_IDENTIFIED 38
#define CAUSE_CALL_BARRED 10
#define CAUSE_RESERVED 11
#define CAUSE_MEMORY_EXCEEDED 22
#define CAUSE_UNIDENTIFIED_SUBSCRIBER 28
#define CAUSE_UNKNOWN_SUBSCRIBER 30
#define CAUSE_INVALID_SM_TRANSFER_REF 81
#define CAUSE_INVALID_IE_CONTENTS 4096
#define CAUSE_MM_AUTH_FAILURE 9
#define CAUSE_MM_CONNECTION_FAIL 10
#define CAUSE_MAX_NUM_CM_CONNS_EXCEEDED 11
#define CAUSE_INCOMPATIBLE_MM_STATE 12
#define RRC_MSG_NO_ERROR 20
#define RRC_INVALID_MSG_TYPE 21
#define RRC_MSG_TOO_SHORT 22
#define RRC_INVALID_TI 23
#define RRC_SKIP_INDICATOR_NOT_ZERO 24
#define RRC_IE_SEQUENCE_ERROR 25
#define RRC_DUPLICATED_IE 26
#define RRC_UNRECOGNISED_IE_COMP_REQ 27
#define RRC_UNRECOGNISED_IE 28
#define RRC_OPTIONAL_IE_CONTENT_ERROR 29
#define RRC_MANDATORY_IE_CONTENT_ERROR 30
#define RRC_MANDATORY_IE_MISSING 31
#define RRC_INVALID_PROTOCOL_DISCRIMINATOR 32
#define RRC_COND_IE_ERROR 33


#define RRC_NORMAL_EVENT 0
#define RRC_ABNORMAL_REL_UNSPECIFIED 1
#define RRC_ABNORMAL_REL_CHAN_UNACCEPT 2
#define RRC_ABNORMAL_REL_TIMER_EXPIRY 3
#define RRC_ABNORMAL_REL_NO_RF_ACTIVITY 4
#define RRC_PREEMPTIVE_RELEASE 5
#define RRC_TIMING_ADVANCE_OUT_OF_RANGE 8
#define RRC_CHAN_MODE_UNACCEPTABLE 9
#define RRC_FREQUENCEY_NOT_IMPLEMENTED 10
#define RRC_CALL_ALREADY_CLEARED 65
#define RRC_SEMANTICALLY_INCORRECT_MSG 95
#define RRC_MANDATORY_IE_ERROR 96
#define RRC_MSG_NOT_IMPLEMENTED 97
#define RRC_MSG_NOT_COMPAT_WITH_STATE 98
#define RRC_CONDITIONAL_IE_ERROR 100
#define RRC_NO_CA_AVAILABLE 101
#define RRC_PROTOCOL_ERROR_UNSPECIFIED 111
#define CAUSE_CC_NO_CAUSE 13
#define CAUSE_INSUFFICIENT_TASKS 14
#define CAUSE_CP_ACK_NOT_RECEIVED 15
#define CAUSE_CP_NO_ERROR 16
#define CAUSE_RP_TIMEOUT_ERROR 17
#define CAUSE_RP_NO_ERROR 18
#define CAUSE_NOT_APPLICABLE 19


#define ONE_PHASE_ACCESS 0
#define SHORT_ACCESS 1
#define TWO_PHASE_ACCESS 2
#define PAGE_RESPONSE 3
#define CELL_UPDATE 4
#define MM_PROCEDURE 5
#define SINGLE_BLOCK 6
#define CELL_UPDATE_WITH_NOTIFICATION 7
#define MM_PROC_ATT 8
#define MM_PROC_RAU 9
#define MM_PROC_OTHER 10
#define SIGNALLING 11
#define NOT_MM_CAUSE 100

#define MM_PD 5
#define CC_PD 3
#define SS_PD 11
#define SMS_PD 9
#define RR_PD 6
#define SCT_PD 15

#define NOT_SACK_OR_ACK 1
#define SACK_OR_ACK 0

#define MAX_FPLMN_SIZE 128
#define MAX_PLMN_SIZE 8
#define LONG_FPLMN_SIZE 128
#define SELECT_PLMN_LIST_SIZE 16
#define NET_LOCK_PLMN_LIST_SIZE 32//should be == MN_NET_LOCK_PLMN_LIST_SIZE
#define LAC_LIST_SIZE 8
#define MAX_FLAI_SIZE 10
#ifdef MODEM_PLATFORM
#define SCANED_CELLS_NUM      40
#endif
#define SMS_NORMAL_EVENT 0

#define SMS_ADDR_MAX_LEN 12

#define ARFCN_POWERLEVEL_LEN 454

#define DEFAULT_DCS_VALUE 0
#define UCS_DCS_VALUE 1

#define SMS_SOR_NORMAL_EVENT 0
#define SMS_SOR_CP_ERROR 1
#define SMS_SOR_RP_ERROR 2
#define SMS_SOR_LOWLAYER_ERROR 3
#define SMS_SOR_CANCEL_BY_USER 4
#define SMS_SOR_TC1M_EXPIRED 5
#define SMS_SOR_TR1M_EXPIRED 6
#define SMS_SOR_TR2M_EXPIRED 7

#define MAC_MAX_NR_OF_RFL_NUMBER  16
#define MAC_MAX_NR_OF_ARFCN_INDEX 64

#define MAX_INSTANCE_NUM          16

/**************************************************************
*                           ENUM                              *
***************************************************************/
typedef int16 NETWORK_MODE_E;
#define    GPRS_MNO_NONE   0
#define    GPRS_MNO_ONE    1
#define    GPRS_MNO_TWO    2
#define    GPRS_MNO_THREE  3

typedef int16  CODE_SCHEME_E;

typedef enum
{
    CIPHERING,
    RESOURCE_ASSIGNMENT,
    CHANNEL_MODE_MODIFICATION,
    NO_CIPHERING
}SYNC_CAUSE_E;

typedef enum
{
    MM_LOCATION_UPDATE,
    MM_IMSI_DETACH,
    CC_NORMAL_CALL,
    CC_EMERGENCY_CALL,
    CC_REESTABLISHMENT,
    SS_SERVICE,
    SMS_SERVICE,
    RESPONSE_PAGING,
    ONE_PHASE_PACKET_ACCESS,
    SINGLE_BLOCK_PACKET_ACCESS
}GSM_EST_TYPE_E;

typedef enum
{
    STATE_INITVAL,
    STATE_LL_EST_WAIT,
    STATE_LL_EST_START,
    STATE_LL_EST_RESTART,
    STATE_LL_EST_END,
    STATE_LL_REL_WAIT,
    STATE_LL_REL_START
}LLC_LINK_STATE_E;

typedef enum
{
    FLOW_CTRL_UNACK_MODE,
    FLOW_CTRL_ACK_MODE,
    FLOW_CTRL_BOTH_MODE
}FLOW_CTRL_MODE_E;

typedef enum
{
    GPRS_FLOW_CTRL_MOD_L4,
    GPRS_FLOW_CTRL_MOD_SNDCP,
    GPRS_FLOW_CTRL_MOD_LLC,
    GPRS_FLOW_CTRL_MOD_RLC,
    GPRS_FLOW_CTRL_MOD_INVALID
}GPRS_FLOW_CTRL_MOD_E;

typedef enum
{
    REEST_NOT_ALLOWED,
    REEST_ALLOWED
}REEST_ALLOWED_OR_NOT_E;

typedef enum
{
    BCCH_NORM,
    BCCH_EXT
}BCCH_POSITION_E;

typedef enum
{
	NORM_SEARCH,
	FIND_PLMN_SEARCH,
	LIMITED_SERVICE_SEARCH,
	SHOW_PLMN_LIST_SEARCH
}PLMN_STATE_E;

typedef enum
{
	NC0,
	NC1,
	NC2,
	NC_SPARE
}NC_ORDER_E;

typedef enum
{ 
    SMS_GSM_PATH,
    SMS_GPRS_PATH
}SMS_PATH_E;

typedef enum
{ 
    SMS_NO_AVIAL_TI,
    SMS_INVALID_SMI,
    SMS_UNACCEPT_MEM_REQ,
    SMS_LOW_LAYER_FAILURE
}SMS_PS_ERROR_E;

typedef enum
{
    PCH1_NC,
    PCH1_C,
    PCH2_NC,
    PCH3_NC,
    PCH4_NC
}CCCH_CONFIG_E;

typedef enum
{
    CELL_BAR_QUA2_INACT,
    RESERVED,
    NORMAL_PRIO,
    LOW_PRIO
}CBQ2_E;

typedef enum
{
    NO_CELL,
    ACCEPTABLE_CELL_IN_FORBID_LIST,
    ACCEPTABLE_CELL,
    LOW_PRIO_CELL,
    NORMAL_PRIO_CELL
}CELL_PRIO_E;

typedef enum
{
    ACCESS_BURSTS_T,
    CONTROL_BLOCK_T
} CONTROL_ACK_E;

typedef enum
{
    PSI2,
    SYS_INFO13,
    SYS_INFO1,
    ASSIGN_MSG
} FREQ_PARAM_TYPE_E;

typedef enum
{
    ACCESS_REJ,
    ACCESS_FORBID,
    T3146_EXPIRED,
    DCCH_TO_PDCH_FAIL,
    IN_CALL,
    CELL_RESELECT
} RACH_REJ_CAUSE_T;

typedef enum
{
    LLC_SNDCP_CAUSE_NO_ERROR = 0,
    RELEASE_CAUSE_DM_RECEIVED = 1,
    RELEASE_CAUSE_INVALID_XID_RESPONSE = 2,
    RELEASE_CAUSE_NO_PEER_RESPONSE = 3,
    RELEASE_CAUSE_NORMAL_RELEASE = 4,
    SNDCP_CAUSE_INVALID_PRIMITIVE = 5,
    SNDCP_CAUSE_NOT_IMPLEMENTED = 6,
    SNDCP_CAUSE_INVALID_XID_COMMAND = 7,
    SNDCP_CAUSE_INVALID_XID_RESPONSE = 8,
    SNDCP_CAUSE_LLC_ERROR = 9
} SNDCP_CAUSE_T;

typedef enum
{
    SIM_CNF_SUC = 0,
    SIM_CNF_TEMP_FAIL = 1,
    GMM_IMSI_UNKNOWN = 2,
    GMM_ILLEGAL_MS = 3,
    GMM_ILLEGAL_ME = 6,
    GMM_GPRS_NOT_ALLOWED = 7,
    GMM_BOTH_NOT_ALLOW = 8,
    GMM_CANNOT_DERIVED_MS_IDENTITY = 9,
    GMM_IMPLICITLY_DETACHED = 10,
    GMM_PLMN_NOT_ALLOW = 11,
    GMM_LA_NOT_ALLOW = 12,
    GMM_ROAM_NOT_ALLOW = 13,
    GMM_PLMN_GPRS_NOT_ALLOW = 14,
    GMM_LA_NO_SUITABLE_CELL = 15,
    GMM_MSC_UNREACHABLE = 16,
    GMM_NETWORK_FAILURE = 17,
    GMM_CONGESTION = 22,
    GMM_RETRY_NEW_CELL = 48,
    GMM_SEMANTICALLY_INCORECT_MSG = 95,
    GMM_INVALID_MANDATORY_INFORM = 96,
    GMM_MSG_NOT_EXISTENT = 97,
    GMM_IE_NOT_EXISTENT = 99,
    GMM_PERMENANT_NOT_ALLOWED = 104,
    GMM_TEMPORARILY_NOT_ALLOWED = 105,
    GMM_ACCESS_BARRED = 106,
    GMM_SUSPENDED = 107,
    GMM_DETACHING = 108,
    GMM_RAU_PROCESSING = 109,
    GMM_UNKNOWN_ERROR = 110,
    GMM_PROTOCOL_ERROR = 111,
    GMM_CAUSE_NOTDEFINE = 255,
    GMM_MM_CAUSE_START = 256,
    GMM_ATTEMPT_COUNTER_EQUAL_5 = 257,
    GMM_ATTEMPT_COUNTER_SMALLER_5 = 258,
    GMM_TIMER_EXPIRY = 259,
    GMM_COMBINED_ATT_ABORT = 260,
    GMM_FORCE_DETACHED = 261,
    RAU_FAIL_T3330_TIME_OUT = 262,
    RAU_FAIL_LOWER_LAYER_FAIL = 263,
    RAU_FAIL_RETRY_NEW_CELL = 264,
    RAU_FAIL_NOT_RETRY_NEW_CELL = 265,
    GMM_ACCESS_CLASS_BARRED = 266,
    RAU_FAIL_OTHER = 267
}GPRS_CAUSE_E;

typedef enum 
{
    GMM_CAUSE_NOT_INCLUDE = 1,
    GMM_CAUSE_MAC_FAILURE = 0x14,
    GMM_CAUSE_SYNCH_FAILURE = 0x15,
    GMM_CAUSE_AUTHENTICATION_UNACCEPTABLE = 0x17,
    GMM_CAUSE_PROTOCOL_ERROR = 111,
    GMM_CAUSE_IMPLICITLY_DETACHED = 257,
    GMM_DETACH_REQ_RECEIVED,
    RECEIVED_IMSII_PAGE,
    RECEIVED_NO_CELL,
    RECEIVED_NO_SIM,
    RECEIVED_LIMITED_CELL,
    GMM_REATTACH_REQ
}GMM_CAUSE_T; /* 0~ 256 refer to GPRS_CAUSE_E */

/**************************************************************
*                           STRUCT                            *
***************************************************************/
typedef uint32  TLLI_T;
typedef int16  NSAPI_NUM_T;
typedef int16  RELIABILITY_CLASS_T;

typedef int16  DELAY_CLASS_T;

typedef int16  PEAK_THROUGHPUT_CLASS_T;
typedef int16  MEAN_THROUGHPUT_CLASS_T;
typedef int16  PRECEDENCE_CLASS_T;
typedef int16  DELIVERY_OF_ERR_SDU_T;
typedef int16  DELIVERY_ORDER_T;
typedef int16  TRAFFIC_CLASS_T;
typedef int16  MAX_SDU_SIZE_T;
typedef int16  MAX_BIT_RATE_FOR_UPLINK_T;
typedef int16  MAX_BIT_RATE_FOR_DOWNLINK_T;
typedef int16  RESIDUAL_BER_T;
typedef int16  SDU_ERR_RATIO_T;
typedef int16  TRAFFIC_HANDLING_PRIO_T;
typedef int16  TRAFFIC_HANDING_PRIO_T;
typedef int16  GUARANTEED_BIT_RATE_FOR_UPLINK_T;
typedef int16  GUARANTEED_BIT_RATE_FOR_DOWNLINK_T;
typedef int32  L3_SIZE_T;
typedef uint32 XID_BLOCK_SIZE_T;
typedef int16  N201_U_T;
typedef int16  N201_I_T;
typedef uint32 PDU_SIZE_T;
typedef int16  NSAPI_NUM_T;
typedef int16  CIPHER_T;
typedef int16  RSSI_T;
typedef int16  RACH_ACCESS_T;
typedef int16  RELIABILITY_CLASS_T;
typedef int16  FRAME_SIZE_T;
typedef int16  LLC_DATA_TYPE_T;
typedef uint32 CELL_ID_T;
typedef uint16 MNC_T;
typedef uint16 MCC_T;
typedef uint16 LAC_T;
typedef uint8  RAC_T;
typedef uint8  CKSN_T;
typedef uint16 ACC_CONTROL_CLASS_T;
typedef uint32 NPDU_SIZE_T;
typedef uint16 SMI_T;
typedef uint16 SMS_CAUSE_TYPE_T;
typedef uint8  LOCAL_TIME_ZONE_T;
typedef uint32 LSA_IDENTITY_T;
typedef uint8  SS_VERSION_VALUE_T;
typedef uint8  MS_MODE_T;

typedef BOOLEAN  IS_MORE_SMS_T;
typedef BOOLEAN  SS_VERSION_PRESENT_T;
typedef BOOLEAN  IS_SGSN_R99_T;
typedef BOOLEAN  IS_REJECT_RECALL_T;
typedef BOOLEAN  IS_CAUSE_PRESENT_T;


typedef uint8 *OCTET_PTR_T;
typedef uint8 MAC_MSG_BLOCK_ARR_T[23];
typedef uint8 L3_MESSAGE_ARR_T[MAX_L3_MESSAGE_SIZE];

typedef uint8 FOUR_OCTET_ARR_T[4];
typedef uint8 EIGHT_OCTET_ARR_T[8];
typedef uint8 NINE_OCTET_ARR_T[9];

typedef uint8 RAND_T[16];
typedef uint8 SRES_T[4];
typedef uint8 KC_T[8];
typedef uint8 SRES_EXT_T[12];
typedef uint8 AUTS_T[14];
typedef uint8 AUTN_T[16];


typedef uint8 SMS_ADDR_ARR_T[SMS_ADDR_MAX_LEN];

typedef uint8 CA_REF_NUM_ARR_T[4];

typedef uint8 MSG_HEADER_ARR_T[2];

typedef L2_MESSAGE_ARR_T *L2_INFO_PTR_T;
typedef L3_MESSAGE_ARR_T *L3_INFO_PTR_T;

typedef OCTET_PTR_T  XID_BLOCK_PTR_T;
typedef OCTET_PTR_T  PDU_PTR_T;
typedef OCTET_PTR_T  FRAME_PTR_T;
typedef OCTET_PTR_T  NPDU_PTR_T;
typedef OCTET_PTR_T  BLOCK_PTR_T;

typedef struct{
    uint8  pd;
    uint8  ti_si;
    uint16  msg_type;
} MSG_HEADER_T;

typedef struct{
    FRAME_PTR_T  frame_ptr;
    FRAME_SIZE_T  frame_size;
} LLC_FRAME_T;

typedef struct{
    TLLI_T  tlli;
    PEAK_THROUGHPUT_CLASS_T  peak_throughput;
    RADIO_PRIORITY_T  radio_priority;
    RACH_ACCESS_T  access_type;
    LLC_DATA_TYPE_T  data_type;
    LLC_PDU_TYPE_T  pdu_type;
    LLC_FRAME_T  llc_frame;
} LLC_TRANS_Q_NODE_T;

typedef struct LLC_TRANS_Q_NODE_T_s *LLC_TRANS_Q_PTR_T;
typedef struct LLC_FRAME_T_s *LLC_FRAME_PTR_T;


typedef struct{
    MCC_T  mcc;
    MNC_T  mnc;
    int16  mnc_digit_num;
    uint8  rat;
    uint8 nStatus;
} PLMN_T;


typedef struct{
    MCC_T  mcc;
    MNC_T  mnc;
    int16  mnc_digit_num;
    LAC_T  lac;
} LAI_T;

typedef struct{
    MCC_T  mcc;
    MNC_T  mnc;
    int16  mnc_digit_num;
    LAC_T  lac;
    RAC_T  rac;
    CELL_ID_T  cell_id;
} CGI_T;


typedef struct{
    MCC_T  mcc;
    MNC_T  mnc;
    int16  mnc_digit_num;
    LAC_T  lac;
    RAC_T  rac;
} RAI_T;


typedef struct{
    BOOLEAN  valid_flag;
    FOUR_OCTET_ARR_T  tmsi_value;
} TMSI_T;


typedef struct{
    BOOLEAN  valid_flag;
    FOUR_OCTET_ARR_T  value;
} P_TMSI_T;

typedef struct{
    BOOLEAN  valid_flag;
    uint8  length;
    EIGHT_OCTET_ARR_T  imsi_value;
} IMSI_T;


typedef struct{
    EIGHT_OCTET_ARR_T  imei_value;
} IMEI_T;


typedef struct{
    NINE_OCTET_ARR_T  imeisv_value;
} IMEISV_T;


typedef struct{
    BOOLEAN  present;
    PLMN_T  fplmn;
} FPLMN_SIM_T;

typedef PLMN_T PLMN_ARR_T[MAX_PLMN_SIZE];

typedef struct{
    uint8  length;
    PLMN_ARR_T  plmn_arr;
} PLMN_LIST_T;

typedef FPLMN_SIM_T FPLMN_LONG_ARR_T[LONG_FPLMN_SIZE];

typedef struct{
    uint8  length;
    FPLMN_LONG_ARR_T  fplmn_arr;
} FPLMN_LONG_LIST_T;

typedef PLMN_T SELECT_PLMN_ARR_T[SELECT_PLMN_LIST_SIZE];

typedef struct{
    uint8  length;
    SELECT_PLMN_ARR_T  plmn_arr;
} SELECT_PLMN_LIST_T;

typedef PLMN_T NET_LOCK_PLMN_ARR_T[NET_LOCK_PLMN_LIST_SIZE];

typedef struct{
    uint8  length;
    NET_LOCK_PLMN_ARR_T  plmn_arr;
    BOOLEAN is_only_check_mcc;
} NET_LOCK_PLMN_LIST_T;

typedef LAC_T LAC_ARR_T[LAC_LIST_SIZE];

typedef struct{
    uint8  length;
    LAC_ARR_T  lac_arr;
} LAC_LIST_T;

typedef struct{
    PLMN_T  plmn;
    LAC_LIST_T  lac_list;
    RSSI_T  plmn_max_rssi;
} PLMN_LAC_T;

typedef PLMN_LAC_T AVAILABLE_PLMN_LA_ARR_T[SELECT_PLMN_LIST_SIZE];

typedef struct{
    uint8  length;
    AVAILABLE_PLMN_LA_ARR_T  plmn_la_arr;
} AVAILABLE_PLMN_LA_LIST_T;


typedef LAI_T FLAI_ARR_T[MAX_FLAI_SIZE];


typedef struct{
    uint8  length;
    FLAI_ARR_T  flai_arr;
} FLAI_LIST_T;


typedef struct{
    uint16  length;
    L3_MESSAGE_ARR_T  l3_msg;
#ifdef __MN_CALL_ADP_CFW__
    BOOLEAN  result;
    uint8  operation_code;
    uint8  component_tag;
    uint8  cssi;
    uint8  cssu;
#endif
} L3_MSG_UNIT_T;

struct NPDU_BUFFER_UNIT_T{
    int32  NPDU_num;
    NPDU_SIZE_T  NPDU_size;
    NPDU_PTR_T  NPDU_ptr;
    int32  SNPDU_count;
    struct NPDU_BUFFER_UNIT_T  *NPDU_next_ptr;
};

typedef struct NPDU_BUFFER_UNIT_T *NPDU_BUFFER_UNIT_PTR_T;


typedef NPDU_BUFFER_UNIT_PTR_T  REFERENCE_T;

typedef struct{
    BOOLEAN  supported;
    uint8  egsm_thr;
    uint8  egsm_val;
    uint8  dcs_thr;
    uint8  dcs_val;
    uint8  pcs_thr;
    uint8  pcs_val;
    uint8  gsm850_thr;
    uint8  gsm850_val;
    MS_BAND_E  ms_band;
} RX_EX_T;





typedef struct{
    uint16  length;
    SMS_ADDR_ARR_T  addr_arr_t;
} SMS_ADDR_T;


typedef struct{
    uint8  max_retrans;
    uint8  tx_integer;
    uint8  cba;
    REEST_ALLOWED_OR_NOT_E  re_est;
    uint16  access_class;
} RACH_CONTROL_PARAM_T;

typedef struct{
    BOOLEAN  mscr;
    BOOLEAN  att;
    uint8  bs_ag;
    CCCH_CONFIG_E  ccch_config;
    CBQ2_E  cbq2;
    uint8  bs_pa;
    uint8  t3212_value;
    int16  ccch_group;
    int16  paging_group;
} CCCH_T;

typedef struct{
    uint8  crh;
    uint8  ms_txpwr_maxcch;
    uint8  rxlev_access_min;
    uint8  si3_acs;
    uint8  si4_acs;
    BOOLEAN  neci;
    uint8  ms_txpwr_dbm;
    uint8  p;
} CELL_SELECTION_PARAM_T;

typedef struct{
    BOOLEAN  pwrc;
    BOOLEAN  dtx_allowed;
    DTX_ALLOWED_E  dtx_allowed_sacch;
    uint8  rlt;
} CELL_OPTION_T;

typedef struct{
    BOOLEAN  sel_param_present;
    uint8  cbq;
    uint8  cell_resel_off;
    uint8  temporary_off;
    uint8  penalty_time;
    uint8  sel_param_pos;
} OPTION_SEL_PARAM_T;

typedef struct{
    BOOLEAN  power_offset_present;
    uint8  power_offset;
} OPTION_POWER_OFFSET_T;


typedef struct{
    BOOLEAN  gprs_ind_present;
    uint8  ra_colour;
    BCCH_POSITION_E  si13_position;
} GPRS_IND_T;

typedef struct{
    BOOLEAN  bs_pcc_rel;
    uint8  bs_pbcch_blks;
    uint8  bs_pag_blks_res;
    uint8  bs_prach_blks;
} PCCCH_ORG_PARAM_T;

typedef struct{
    uint8  tsc;
    uint8  tn;
    BOOLEAN  hopping_ind;
    ARFCN_T  arfcn;
    uint8  maio;
    uint8  ma_number;
} RR_PCCCH_DESC_T;

typedef RR_PCCCH_DESC_T RR_PCCCH_DESC_ARR_T[16];

typedef struct{
    BOOLEAN  hopping_ind;
    ARFCN_T  arfcn;
    uint8  maio;
    uint8  ma_num;
    uint8  ts_alloc;
} MEAS_CHAN_T;

typedef MEAS_CHAN_T MEAS_CHAN_ARR_T[32];

typedef struct{
    BOOLEAN  ie_present;
    BOOLEAN  si13_position_present;
    BCCH_POSITION_E  si13_position;
    uint8  pbcch_tn;
    uint8  psi1_repeat_period;
} RR_SI13_PBCCH_LOCATION_T;

typedef struct {
    BOOLEAN  ie_present;
    uint8  gprs_priority_class;
    uint8  gprs_hsc_thr;
} HCS_PARS_T;

typedef struct{
    ARFCN_T  arfcn;
    uint8  bsic;
    uint8  cba2;
    BOOLEAN  exc_acc;
    BOOLEAN  same_ra_as_serv_cell;
    BOOLEAN  gprs_rxlev_acc_min_present;
    uint8  gprs_rxlev_acc_min;
    uint8  gprs_ms_txpwr_max_cch;
    BOOLEAN  gprs_temp_offset_present;
    uint8  gprs_temp_offset;
    uint8  gprs_penalty_time;
    BOOLEAN  gprs_resel_offset_present;
    uint8  gprs_resel_offset;
    HCS_PARS_T  hcs_pars;
    RR_SI13_PBCCH_LOCATION_T  si13_pbcch_location;
} RR_NCELL_PARS_T;

typedef RR_NCELL_PARS_T RR_NCELL_PARS_ARR_T[32];

typedef struct{
    uint8  ext_meas_order;
    BOOLEAN  ext_rep_type_present;
    uint8  ext_rep_type;
    uint8  ncc_permitted;
    BOOLEAN  int_freq_present;
    uint8  int_freq;
    BOOLEAN  ext_rep_period_present;
    uint8  ext_rep_period;
    ARFCN_LIST_T  ext_freq_list;
} CELL_EXT_MEAS_PARS_T;

typedef struct{
    BOOLEAN  r_cell_present;
    MS_BAND_E  band;
    ARFCN_T  arfcn;
    BSIC_T  bsic;
    BOOLEAN  bsic_busy;
    RSSI_T  rssi;
    BCCH_TC_MASK_T  tc_mask;
    uint32  search_start_time;
    uint8  tc2_norm_bad_times;
    uint8  tc3_norm_bad_times;
    uint8  tc6_norm_bad_times;
    uint8  tc7_norm_bad_times;
    BOOLEAN  si3_present;
    BOOLEAN  si4_present;
} REDUCED_CELL_INFO_T;

typedef struct{
    RSSI_T  rxlev_full;
    RSSI_T  rxlev_sub;
} RXLEV_T;

typedef struct{
    uint8  rxqual_full;
    uint8  rxqual_sub;
} RXQUAL_T;

typedef struct{
    BOOLEAN  sapi0_active;
    BOOLEAN  sapi3_active;
    CHANNEL_TYPE_E  sapi0_chan;
    CHANNEL_TYPE_E  sapi3_chan;
    CHANNEL_DESCRIPTION_T  chan_desc;
    MOBILE_ALLOCATION_T  ma_list;
    CHANNEL_MODE_E  chan_mode;
    BOOLEAN  cipher_set;
    CIPHER_PARAM_T  cipher_param;
    BOOLEAN  multislot_supported;
    MULTI_SLOTS_T  multislots_param;
    RXLEV_T  rxlev;
    RXQUAL_T  rxqual;
    uint8  ta;
    uint8  txpwr;
    uint8  ms_tx_cur;
    BOOLEAN  dtx_used;
    uint8  rlt_cur;
} RES_PARAM_T;

typedef struct{
    ARFCN_T  arfcn;
    RSSI_T  rssi;
    MS_BAND_E  band;
} ARFCN_POWERLEVEL_T;

typedef ARFCN_POWERLEVEL_T ARFCN_POWERLEVEL_ARR_T[ARFCN_POWERLEVEL_LEN];


typedef struct{
    SMS_CAUSE_TYPE_T  sms_cause_type;
    uint16  sms_cause_value;
} SMS_CAUSE_T;

typedef struct{
    BOOLEAN  band_filter_on;
    ARFCN_T  start_arfcn;
    ARFCN_T  end_arfcn;
} BAND_FILTER_T;

typedef struct{
    PLMN_T  plmn;
    ARFCN_ROUND_LIST_T  arfcn_list;
} PLMN_ARFCN_T;

typedef PLMN_ARFCN_T PLMN_ARFCN_ARR_T[SELECT_PLMN_LIST_SIZE];

typedef struct{
    uint8  length;
    PLMN_ARFCN_ARR_T  plmn_arfcn_arr;
} PLMN_ARFCN_LIST_T;

typedef struct{
    PEAK_THROUGHPUT_CLASS_T  peak;
    RADIO_PRIORITY_T  prior;
    RLC_MODE_T  mode;
    LLC_PDU_TYPE_T  pdu_type;
    int16  octet_count;
} CHAN_DESCR_T;

typedef uint8 BA_LIST_ARR_T[16];

typedef uint8 MA_BITMAP_ARR_T[8];

typedef struct {
    uint8  length;
    MA_BITMAP_ARR_T  bitmap;
} MA_BITMAP_T;

typedef uint8 RFL_NUMBER_ARRAY_T[MAC_MAX_NR_OF_RFL_NUMBER];

typedef struct {
    uint8  rfl_nbr_count;
    RFL_NUMBER_ARRAY_T  rfl_nbr;
} RFL_NUMBER_LIST_T;


typedef uint8 ARFCN_INDEX_ARRAY_T[MAC_MAX_NR_OF_ARFCN_INDEX];

typedef struct {
    uint8  arfcn_index_count;
    ARFCN_INDEX_ARRAY_T  arfcn_index;
} ARFCN_INDEX_LIST_T;

typedef uint8 MAC_MS_PAR_IMEI_ARR_T[8];

typedef uint8 MAC_MA_BITMAP_T[8];

typedef struct {
    BOOLEAN  ie_present;
    uint8  hsn;
    BOOLEAN  rfl_nbr_list_present;
    RFL_NUMBER_LIST_T  rfl_nbr_list;
    BOOLEAN  ma_bitmap_present;
    uint8  ma_length;
    MAC_MA_BITMAP_T  ma_bitmap;
    ARFCN_INDEX_LIST_T  arfcn_index_list;
} GPRS_MOBILE_ALLOCATION_T;

typedef struct{
    NETWORK_MODE_E  nmo;
    uint32  t3168;
    uint32  t3192;
    uint8  drx_timer_max;
    AB_FORMAT_E  access_burst_type;
    CONTROL_ACK_E  ctrl_ack_type;
    uint8  bs_cv_max;
    BOOLEAN  pan_present;
    uint8  pan_dec;
    uint8  pan_inc;
    uint8  pan_max;
    BOOLEAN  extention_present;
    uint8  extention_len;
    BOOLEAN  supp_egprs;
    BOOLEAN  supp_egprs_pkt_ch_req;
    uint8  bep_period;
    BOOLEAN  pfc_feature_mod;
    BOOLEAN  support_dtm;
    BOOLEAN  bss_pag_coord;
    BOOLEAN  ccn_active;
    BOOLEAN  nw_ext_utbf;
} GPRS_CELL_OPTIONS_IE_T;

typedef uint8 MAX_RETRANS_T[4];

typedef uint8 PERSIST_LEVEL_T[4];

typedef struct {
    uint16  acc_ctrl_class;
    uint8  s;
    uint8  tx_init;
    MAX_RETRANS_T  max_retrans;
    BOOLEAN  presist_level_present;
    PERSIST_LEVEL_T  presist_level;
} PRACH_CTRL_PARAMETERS_T;

typedef struct {
    ARFCN_LIST_T  arfcn_list;
    uint8  rfl_number;
} REF_FREQ_LIST_T;

typedef REF_FREQ_LIST_T REF_FREQ_LIST_ARR_T[4];

typedef struct {
    uint8  ma_number;
    GPRS_MOBILE_ALLOCATION_T  mobile_allocation;
} RR_GPRS_MA_T;

typedef RR_GPRS_MA_T GPRS_MOBILE_ALLOC_ARR_T[6];

typedef struct {
    uint8  cell_bar_access_2;
    BOOLEAN  exc_acc;
    uint8  gprs_rxlev_acc_min;
    uint8  gprs_ms_txpwr_max_cch;
    HCS_PARS_T  hcs_pars;
    uint8  multiband_reporting;
} SERVING_CELL_PARS_T;

typedef struct {
    uint8  gprs_cell_resel_hyst;
    BOOLEAN  gprs_cell_resel_hyst_present;
    uint8  c31_hyst;
    uint8  c32_qual;
    BOOLEAN  rand_acc_retry;
    BOOLEAN  t_resel_present;
    uint8  t_resel;
    BOOLEAN  ra_reselect_hyst_present;
    uint8  ra_reselect_hyst;
} GENERAL_CELL_SEL_PARS_T;

typedef struct PSI2_FREQ_PARAM_T_s {
    uint8  change_mark;
    uint8  ref_freq_list_num;
    REF_FREQ_LIST_ARR_T  ref_freq_list_arr;
    ARFCN_LIST_T  ca_list;
    uint8  gprs_ma_list_num;
    GPRS_MOBILE_ALLOC_ARR_T  gprs_ma_list_arr;
} PSI2_FREQ_PARAM_T;

typedef struct PSI2_FREQ_PARAM_T_s *PSI2_FREQ_PARAM_PTR_T;

typedef struct {
    uint8  change_mark;
    GPRS_MOBILE_ALLOCATION_T  gprs_ma_ie;
} SI13_FREQ_PARAM_T;

typedef struct {
    ARFCN_LIST_T  ca_list;
} SI1_FREQ_PARAM_T;

typedef union {
    PSI2_FREQ_PARAM_PTR_T  psi2_freq_param_ptr;
    SI13_FREQ_PARAM_T  si13_freq_param;
    SI1_FREQ_PARAM_T  si1_freq_param;
} FREQ_PARAM_T;

typedef struct {
    FREQ_PARAM_TYPE_E  freq_param_type;
    FREQ_PARAM_T  freq_param_value;
} FREQ_UPDATE_T;

typedef uint8  RACH_REJ_TIME_T;
typedef uint8  TA_T;
typedef uint8  CHANNEL_CODING_REQUESTED_T;
typedef uint8 CHANNEL_REQUEST_DESCRIPTION_T[5];
typedef uint8 PACKET_CHANNEL_DESC_IE_T[3];
typedef uint8 MA_BITMAP_ARR_T[8];
typedef int16  MAC_GRR_MSG_TYPE_T;
typedef BOOLEAN  IS_PCCCH_T;
typedef uint32  T3168_VALUE_T;
typedef uint32  T3192_VALUE_T;
typedef BOOLEAN  NECI_VALUE_T;
typedef BOOLEAN  IS_CELL_SUPPORT_EGPRS_T;
typedef BOOLEAN  IS_CELL_SUPPORT_ERACH_T; 
typedef uint8 PACKET_CONSTRUCT_ARR_T[11];

typedef struct PACKET_CONSTRUCT_T_s {
    uint8  length;
    PACKET_CONSTRUCT_ARR_T  packet_construct_arr;
} PACKET_CONSTRUCT_T;

typedef struct GPRS_MEAS_RESULT_T_s {
    uint8  c_value;
    uint8  rxqual;
    uint8  sign_var;
} GPRS_MEAS_RESULT_T;

typedef struct PACCH_DATA_T_s {
    MAC_GRR_MSG_TYPE_T  msg_type;
    uint16  msg_length;
    MAC_MSG_BLOCK_ARR_T  msg_value;
    ABSOLUTE_FRAME_NUMBER_T  frame_number;
} PACCH_DATA_T;

typedef struct{
    BOOLEAN is_valid;
    uint8 highest_instance;
    BOOLEAN is_enh_report;
    uint8 serving_band_report;
}SI2Q_MEAS_PARAM_T;

typedef struct{
    BOOLEAN is_valid;
    uint8 start_index;  
    uint8 end_index;
}BSIC_INFO_T;

typedef BSIC_INFO_T BSIC_INFO_ARR_T[MAX_INSTANCE_NUM];

typedef struct{
    ARFCN_T arfcn;
    uint8 ba_index;
    BSIC_T bsic;
}ARFCN_INDEX_BSIC_T;

typedef ARFCN_INDEX_BSIC_T ARFCN_INDEX_BSIC_ARR_T[96];

typedef struct{
    BOOLEAN is_valid;
    uint8 length;
    ARFCN_INDEX_BSIC_ARR_T arfcn_index_bsic_arr;
}ENH_BA_LIST_T;

typedef struct{
	uint8 multiband_reporting;
	uint8 serving_band_reporting;
	uint8 scale_ord;
	uint8 reporting_offset_900;
	uint8 reporting_threshold_900;
	uint8 reporting_offset_1800;
	uint8 reporting_threshold_1800;
	uint8 reporting_offset_850;
	uint8 reporting_threshold_850;
	uint8 reporting_offset_1900;
	uint8 reporting_threshold_1900;
}GPRS_MEAS_PARAM_T;

typedef struct{
    BOOLEAN is_valid;
    BOOLEAN highest_instance;
    BOOLEAN is_enh_report;
	uint8 reporting_rate;
    BOOLEAN invalid_bsic_report	;
    GPRS_MEAS_PARAM_T gprs_meas_param;
}SI2Q_GPRS_MEAS_PARAM_T;

typedef uint8 CCN_SUP_ARR_T[128];

typedef struct{
    uint16 ccn_cell_num;
    CCN_SUP_ARR_T ccn_sup_arr;
}SI2Q_CCN_SUPPORT_T;

typedef struct{
    BOOLEAN	ccn_sup_param_present;
    SI2Q_CCN_SUPPORT_T ccn_sup_param;
}SIQ_EXT_INFO_T;

typedef BOOLEAN INSTANCE_INFO_ARR_T[16];

typedef BSIC_INFO_T GPRS_BSIC_INFO_ARR_T[16];

typedef struct{
    uint8 ba_ind; 
    uint8 ba_ind_3g; 
    uint8 mp_change_mark;
    uint8 si2q_count;
    INSTANCE_INFO_ARR_T instance_info;
    BOOLEAN gprs_enh_ba_valid;
    SI2Q_MEAS_PARAM_T meas_param;
	BOOLEAN  is_bsic_present;
    GPRS_BSIC_INFO_ARR_T gprs_bsic_info;
    ENH_BA_LIST_T gprs_enh_ba;
	SI2Q_GPRS_MEAS_PARAM_T gprs_meas_param;
    BOOLEAN si2q_ext_info_present;
    SIQ_EXT_INFO_T si2q_ext_info;
}SI2Q_PARAM_T;

#ifdef _ENH_MEAS_SUPPORT_
typedef struct MI_MEAS_PARAM_T_s {
    BOOLEAN  is_valid;
    uint8  highest_instance;
    uint8  multiband_reporting;
    uint8  serving_band_reporting;
    uint8  scale_ord;
    uint8  reporting_offset_900;
    uint8  reporting_threshold_900;
    uint8  reporting_offset_1800;
    uint8  reporting_threshold_1800;
    uint8  reporting_offset_850;
    uint8  reporting_threshold_850;
    uint8  reporting_offset_1900;
    uint8  reporting_threshold_1900;
} MI_MEAS_PARAM_T;

typedef struct MI_PARAM_T_s {
    uint8  ba_ind;
    uint8  ba_ind_3g;
    uint8  mp_change_mark;
    uint8  mi_count;
    INSTANCE_INFO_ARR_T  instance_info;
    BOOLEAN  is_enh_report;
    BOOLEAN  invalid_bsic_report;
    BOOLEAN  enh_ba_valid;
    BOOLEAN  is_bsic_present;
    BSIC_INFO_ARR_T  bsic_info;
    ENH_BA_LIST_T  enh_ba;
    MI_MEAS_PARAM_T  meas_param;
} MI_PARAM_T;
#endif
#ifdef MODEM_PLATFORM
typedef struct {
    MCC_T  mcc;
    MNC_T  mnc;
    int16  mnc_digit_num;
    LAC_T  lac;
    CELL_ID_T cell_id;
    ARFCN_T arfcn;
    MS_BAND_E  band;
    BSIC_T bsic;
    RSSI_T rxlev;
}SCANED_CELL_INFO_T;


typedef struct
{
    uint8 cell_num;
    SCANED_CELL_INFO_T  scaned_cell_info[SCANED_CELLS_NUM];
}SCANED_CELL_INFO_ARR_T;
#endif // MODEM_PLATFORM

typedef struct {
    BOOLEAN  cell_present;
    BOOLEAN  si1_present;
    BOOLEAN  si2_present;
    BOOLEAN  si2bis_present;
    BOOLEAN  si2ter_present;
    BOOLEAN  si2quater_present;
    BOOLEAN  si3_present;
    BOOLEAN  si4_present;
    BOOLEAN  si5_present;
    BOOLEAN  si5bis_present;
    BOOLEAN  si5ter_present;
    BOOLEAN  si6_present;
    BOOLEAN  si7_present;
    BOOLEAN  si8_present;
    BOOLEAN  si7_not_exist;
    BOOLEAN  si8_not_exist;
    BOOLEAN  si13_present;

    BOOLEAN  pcs_band_ind;
    MS_BAND_E  cell_band;
    MS_BAND_E  plmn_band;
    
    ARFCN_T  arfcn;
    RSSI_T  rssi;
    BSIC_T  bsic;
    BOOLEAN  bsic_exist;
    
    RACH_CONTROL_PARAM_T  rach_ctrl_param;
    ARFCN_LIST_T  ca_list;
    ARFCN_LIST_T  ba_list;
    ARFCN_LIST_T  whole_ba_list;
    uint8  ba_ind;
    uint8  ba_ind_3g;
    uint8  ba_2_ext_ind;
    uint8  ba_5_ext_ind;
    BA_LIST_ARR_T  ba_undecoded;
    BA_LIST_ARR_T  ba_undecoded_bis;
    BA_LIST_ARR_T  ba_undecoded_ter;
    uint8  ncc_permitted;
    CELL_ID_T  cell_id;
    LAI_T  lai;
    uint8  multiband_report;
    CCCH_T  ctrl_chan_desc;
    CELL_OPTION_T  cell_options;
    CELL_SELECTION_PARAM_T  cell_sel_param;
    OPTION_SEL_PARAM_T  option_sel_param;
    OPTION_POWER_OFFSET_T  option_power_offset;
    uint8  si_2ter_ind;
    uint8  si2quater_ind;
    BCCH_POSITION_E si2quater_position;
    BOOLEAN  early_classmark_send;
    
    BOOLEAN  cbch_present;
    CHANNEL_DESCRIPTION_T  cbch_chan_desc;
    MA_BITMAP_T  cbch_ma_bitmap;
    
    GPRS_IND_T  gprs_ind;    
    BOOLEAN pbcch_present;
    uint8  bcch_change_mark;
    uint8  si_change_field;
    
    /* Parameters for cell selection or reselection */
    CELL_PRIO_E  cell_prio;
    int16  c1;
    int16  c2;
    int16  c31;
    int16  c32;
    uint32  is_ncell_time;
    uint32  rx_si3_time;
    uint32  rx_si4_time;
    uint32  rx_si7_time;
    uint32  rx_si8_time;
    uint32  rx_psi1_time;
    uint32  search_start_time;
    uint32  ps_rej_penalty_start_time;
    BOOLEAN ps_rej_penalty;
    BCCH_TC_MASK_T  tc_mask;
    uint8  tc0_norm_bad_times;
    uint8  tc1_norm_bad_times;
    uint8  tc2_norm_bad_times;
    uint8  tc3_norm_bad_times;
    uint8  tc4_norm_bad_times;
    uint8  tc5_norm_bad_times;
    uint8  tc6_norm_bad_times;
    uint8  tc7_norm_bad_times;
    uint8  tc0_ext_bad_times;
    uint8  tc3_ext_bad_times;
    uint8  tc5_ext_bad_times;
    uint8  tc7_ext_bad_times;
    uint32  last_req_si3_time;
    RSSI_T  last_req_si_rssi;
    uint8  candidate_times;
    
    PAGING_MODE_E  page_mode;
    uint32  delta_fn;
    int16  otd;
    uint8  dsc_cur;
    BOOLEAN  bad_flag;
    int16  bsic_priority;
    BOOLEAN  allow_emergency_call;

#ifdef _SUPPORT_GPRS_    
    /* gprs param */
    BOOLEAN si13_gprs_ma_present;
	uint8 si13_change_mark;
	GPRS_MOBILE_ALLOCATION_T si13_gprs_ma;
	uint8 rac;
	BOOLEAN spgc_ccch_sup;
	uint8 priority_access_thr;
	NC_ORDER_E nc_order;
	GPRS_CELL_OPTIONS_IE_T gprs_cell_options;
	GLOBAL_PWRC_PARAM_T global_pwrc_param;	
	
	BOOLEAN  psi13_present;
	BOOLEAN si_status_support;
                
    SERVING_CELL_PARS_T serv_cell_param;
    GENERAL_CELL_SEL_PARS_T gen_sel_param;
    uint8 ncell_param_count;
    RR_NCELL_PARS_ARR_T ncell_param;

    BOOLEAN meas_par_present;
    BOOLEAN nc_period_present;
    uint8 nc_non_drx_period;
    uint8 nc_reporting_period_i;
    uint8 nc_reporting_period_t;
    BOOLEAN ext_meas_par_present;
    CELL_EXT_MEAS_PARS_T ext_meas_param;

    BOOLEAN nc_report;
    BOOLEAN ext_nc_report;    
    
    IS_SGSN_R99_T sgsnr;
    SI2Q_PARAM_T si2q_param;

    /* Parameters decoded from PBCCH */
#ifdef _SUPPORT_PBCCH_    
    BOOLEAN pbcch_camping;
    BOOLEAN psi1_present;
	BOOLEAN psi2_present;
	BOOLEAN psi3_present;
	BOOLEAN psi3bis_present;
	BOOLEAN psi4_present;
	BOOLEAN psi5_present;
	BOOLEAN psi8_present;    
    BOOLEAN psi_status_support;
    BOOLEAN psi4_available;
	BOOLEAN psi5_available;

    uint8 psi1_repeat_period;
    uint8 psi_change_field;
	uint8 psi_count_low_rate;
	uint8 psi_count_high_rate;	
	uint8 pbcch_change_mark;
	uint8 psi2_change_mark;
	uint8 psi3_change_mark;
	uint8 psi4_change_mark;
	uint8 psi5_change_mark;
	uint8 psi8_change_mark;	
	uint8 psi3_bis_count;

    PBCCH_DESCRIPTION_T pbcch_desc;
    PRACH_CTRL_PARAMETERS_T prach_ctrl_param;
	PCCCH_ORG_PARAM_T pccch_org_param;	
    uint8 pccch_desc_count;
    RR_PCCCH_DESC_ARR_T pccch_desc;

    uint8 rfl_count;
    REF_FREQ_LIST_ARR_T ref_freq_list;      
    uint8 ca_ref_count;
    CA_REF_NUM_ARR_T ca_ref_num;
    ARFCN_LIST_T gprs_ca_list;
    ARFCN_LIST_T gprs_ba_list;
    uint8 gprs_ma_count;
    GPRS_MOBILE_ALLOC_ARR_T gprs_ma;
    uint8 meas_chan_count;
    MEAS_CHAN_ARR_T meas_chan_list;
#endif //_SUPPORT_PBCCH_
    
#endif //_SUPPORT_GPRS_ 
#ifdef _ENH_MEAS_SUPPORT_
    BOOLEAN  mi_present;
    MI_PARAM_T  mi_param;
#endif
    BOOLEAN  fake_cell_flag;

} CELL_INFO_T;

typedef struct
{
    RELIABILITY_CLASS_T  reliability;
    PEAK_THROUGHPUT_CLASS_T  peak_throughput;
} QOS_PARAM_T;

typedef struct{
    BOOLEAN  valid_flag;
    uint8    save_power_band;
    uint16   save_power_arfcn;
} PHONE_SAVE_POWER_LOCK_ARFCN_T;

typedef struct { 
   ARFCN_T arfcn; 
   BOOLEAN bar_current_cell; 
   uint16 bar_time; 
} MANUAL_SWITCH_ARFCN_T;
#endif
