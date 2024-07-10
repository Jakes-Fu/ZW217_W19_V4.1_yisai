/****************************************************************************
 ** File Name:      caftapi.h                                               *
 ** Author:         wei.zhou                                                *
 ** Date:           02/06/2009                                              *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is used to describe the application modual.   *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                             *
 ** 02/2009        wei.zhou           Create
 ** 
 ****************************************************************************/

#ifndef  _CAFTAPI_H_
#define  _CAFTAPI_H_

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                    */
/*----------------------------------------------------------------------------*/
#include "cafhelp.h"

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define ITAPI_SUCCESS          (0)    //successful operation
#define ITAPI_ERROR            (-1)   //unsuccessful operation
#define ITAPI_EALREADY         (-2)   // Operation is already in progress

#define ITAPI_MAX_ADDR_BCD_LEN          20    // max dial number len,modify by sunsome cr10312
#define ITAPI_SMS_USER_DATA_LENGTH      140      // the SMS user data length before decode or after encode
#define ITAPI_SMS_ADDR_MAX_LEN          12
#define ITAPI_MAX_L3_MESSAGE_SIZE       251
#define ITAPI_SMS_MAX_USER_VALID_DATA_LENGTH    160      // the SMS user data length before decode or after encode
#define ITAPI_SMS_SPLIT_MAX_NUM         4
#define ITAPI_IMEI_LEN                  17
#define ITAPI_MAX_IMSI_ARR_LEN          20

typedef enum
{
    MSG_SMS_SERVICE_START = (MSG_KERNEL_START + 0x200),
    MSG_SMS_READ_SMS_CNF = MSG_SMS_SERVICE_START,
    MSG_SMS_UPDATE_SMS_STATE_CNF,
    MSG_SMS_SEND_SMS_CNF,
    MSG_SMS_WRITE_SMS_CNF,
    MSG_SMS_WRITE_SMS_PARAM_CNF,
    MSG_SMS_SET_SMS_RECEIVE_FUNC_CNF,
    MSG_SMS_MEM_FULL_IND,
    MSG_SMS_STATUS_REPORT_IND,
    MSG_SMS_RECEIVE_IND,
    MSG_SMS_SERVICE_END
}ITAPI_SMS_MSG_E;

typedef enum
{
    MSG_CALL_SERVICE_START = (MSG_SMS_SERVICE_START + 0x100),
    MSG_CALL_START_IND = MSG_CALL_SERVICE_START,
    MSG_CALL_SETUP_COMPLETE_IND,
    MSG_CALL_SETUP_COMPLETE_CNF,
    MSG_CALL_DISCONNECTED_IND,
    MSG_CALL_SETUP_IND,
    MSG_CALL_ERR_IND,
    MSG_CALL_SERVICE_END
}ITAPI_CALL_MSG_E;

typedef enum
{
    MSG_PHONE_SERVICE_START = (MSG_CALL_SERVICE_START + 0x100),
    MSG_PHONE_SERVICE_END
}ITAPI_PHONE_MSG_E;

typedef enum
{
    MSG_SS_SERVICE_START = (MSG_PHONE_SERVICE_START + 0x100),
    MSG_SS_SERVICE_END
}ITAPI_SS_MSG_E;

typedef enum
{
    MSG_SIM_SERVICE_START = (MSG_SS_SERVICE_START + 0x100),
    MSG_SIM_SERVICE_END
}ITAPI_SIM_MSG_E;

typedef enum
{
    ITAPI_STATE_INIT,
    ITAPI_STATE_ACTIVED,
    ITAPI_STATE_DEACTIVED,
    ITAPI_STATE_SIMRDY,
    ITAPI_STATE_NOSIM,
    ITAPI_STATE_BUSY
}ITAPI_STATE_E;

#ifdef MMI_MULTI_SIM_SYS_SINGLE
typedef enum
{
    ITAPI_DUAL1 = 0,
    ITAPI_DUAL_MAX
}ITAPI_DUAL_E;

#elif defined(MMI_MULTI_SIM_SYS_DUAL)
typedef enum
{
    ITAPI_DUAL1 = 0,
    ITAPI_DUAL2 = 1,
    ITAPI_DUAL_MAX
}ITAPI_DUAL_E;
#elif defined(MMI_MULTI_SIM_SYS_TRI)
typedef enum
{
    ITAPI_DUAL1 = 0,
    ITAPI_DUAL2 = 1,
    ITAPI_DUAL3 = 2,
    ITAPI_DUAL_MAX
}ITAPI_DUAL_E;
#elif defined(MMI_MULTI_SIM_SYS_QUAD)
typedef enum
{
    ITAPI_DUAL1 = 0,
    ITAPI_DUAL2 = 1,
    ITAPI_DUAL3 = 2,
    ITAPI_DUAL4 = 3,
    ITAPI_DUAL_MAX
}ITAPI_DUAL_E;
#else
    #error need ITAPI_DUAL define
#endif

typedef enum _ITAPI_SMS_STORAGE_E  
{
    ITAPI_SMS_NO_STORED,
    ITAPI_SMS_STORAGE_ME,
    ITAPI_SMS_STORAGE_SIM,
    ITAPI_SMS_STORAGE_ALL
}ITAPI_SMS_STORAGE_E;

/*
 the SMS status in SIM
*/
typedef enum _ITAPI_SMS_STATUS_E
{
    ITAPI_SMS_FREE_SPACE ,                   // free space
    ITAPI_SMS_MT_READED ,                    // MT SMS , message read
    ITAPI_SMS_MT_TO_BE_READ ,                // MT SMS , message to be read
    ITAPI_SMS_MO_TO_BE_SEND ,                // MO SMS , message to be send
    ITAPI_SMS_MO_SR_NOT_REQUEST ,            // MO SMS , status report not requested 
    ITAPI_SMS_MO_SR_REQUESTED_NOT_RECEIVED , // MO SMS , status report request but not received
    ITAPI_SMS_MO_SR_RECEIVED_NOT_STORE ,     // MO SMS , status report request,received but not store in SIM card
    ITAPI_SMS_MO_SR_RECEIVED_AND_STORED ,    // MO SMS , status report request,received and stored in SIM card
    ITAPI_SMS_MT_SR_READED ,                 // status report received and read
    ITAPI_SMS_MT_SR_TO_BE_READ               // status report received to be read
}ITAPI_SMS_STATUS_E;

typedef enum _ITAPI_NUMBER_TYPE_E
{
    ITAPI_NUM_TYPE_UNKNOW          = 0,    /*0000,an unknown number*/
    ITAPI_NUM_TYPE_INTERNATIONAL,          /*0001,international number*/
    ITAPI_NUM_TYPE_NATIONAL,               /*0010,national number*/
    ITAPI_NUM_TYPE_NETWORK,                /*0011,network specific number*/
    ITAPI_NUM_TYPE_DEDICATECODE_SHORTCODE, /*0100,dedicated code or short code*/
    ITAPI_NUM_TYPE_ALPHANUMERIC,           /*0101,alphanumeric*/
    ITAPI_NUM_TYPE_ABBREVIATE,             /*0110,abbreviate number*/
    ITAPI_NUM_TYPE_RESERVED                /*0111,reserved for extent */
}ITAPI_NUMBER_TYPE_E;

typedef enum _ITAPI_NUMBER_PLAN_E
{
    ITAPI_NUM_PLAN_UNKNOW          = 0,    /*0000,an unknow number*/
    ITAPI_NUM_PLAN_ISDN_TELE,              /*0001,ISDN/telephony numbering plan*/
    ITAPI_NUM_PLAN_DATA            = 3,    /*0011,data numbering plan*/
    ITAPI_NUM_PLAN_TELEX,                  /*0100,telex numbering plan*/
    ITAPI_NUM_PLAN_NATIONAL        = 8,    /*1000,national numbering plan*/
    ITAPI_NUM_PLAN_PRIVATE ,               /*1001,private numbering plan*/
    ITAPI_NUM_PLAN_ERMES,                  /*1001,ermes numbering plan*/
    ITAPI_NUM_PLAN_RESERVED        = 15,   /*1111,reserved for extention*/
    ITAPI_NUM_PLAN_CANNOTUSE      
}ITAPI_NUMBER_PLAN_E;

/*
 the alphabet type being used
*/
typedef enum _ITAPI_SMS_ALPHABET_TYPE_E
{
    ITAPI_SMS_DEFAULT_ALPHABET = 0 ,        // default alphabet
    ITAPI_SMS_8_BIT_ALPHBET        ,        // 8 bit data
    ITAPI_SMS_UCS2_ALPHABET        ,        // UCS2(16bit) 
    ITAPI_SMS_RESERVED_ALPHABET             // reserved
}ITAPI_SMS_ALPHABET_TYPE_E;

/*
the SMS path enum
*/
typedef enum _ITAPI_SMS_PATH_E
{
    ITAPI_SMS_GSM_PATH,              // The GSM path
    ITAPI_SMS_GPRS_PATH              // The GPRS path
}ITAPI_SMS_PATH_E;

/*
 the SMS operate cause
*/
typedef enum _ITAPI_SMS_CAUSE_E
{
    ITAPI_SMS_OPERATE_SUCCESS      ,   // operate right
    ITAPI_SMS_UNSPEC_ERROR         ,   // the unspecific error 
    ITAPI_SMS_SEND_FAILURE         ,   // send fail
    ITAPI_SMS_DEFAULT_PARAM_ERROR  ,   // the default param which uplayer want to use not set
    ITAPI_SMS_ME_MEM_FULL          ,   // the me memory is full
    ITAPI_SMS_ME_NOT_SUPPORT       ,   // save in me not support
    ITAPI_SMS_ME_DELETE_FAILURE    ,   // delete SMS in ME failed
    ITAPI_SMS_SIM_NOT_READY        ,   // the SIM card is not ready
    ITAPI_SMS_SIM_MEM_FULL         ,   // the SIM memory is full
    ITAPI_SMS_SIM_ACCESS_FAILED    ,   // do not have right to do access the SIM card file
    ITAPI_SMS_SIM_READ_FAILED      ,   // read SIM file error
    ITAPI_SMS_SIM_UPDATE_FAILED    ,   // update SIM file error
    ITAPI_SMS_SIM_DELETE_FAILURE   ,   // delete SMS in SIM failed
    ITAPI_SMS_RECORD_OUT_OF_RANGE  ,   // record id out of range
    ITAPI_SMS_NOT_RIGHT_STORAGE    ,   // the storage is not right
    ITAPI_SMS_SIM_SAVE_STATUS_FREE ,   // save long SMS in SIM, return save ok, but read status free space
    ITAPI_SMS_UNKNOW_FAILED
}ITAPI_SMS_CAUSE_E;

/*
 the tp status enum
*/
typedef enum _ITAPI_SMS_TP_STATUS_E
{
    ITAPI_SMS_RECEIVED_BY_SME,         // the SMS receive by SME successfully
    ITAPI_SMS_NOT_CONFIRM_RECEIVED,    // the short message is forwarded by the SC to the SME but the SC is unable to confirm delivery
    ITAPI_SMS_SM_REPLACED_BY_SC,       // short message replaced by the SC
    ITAPI_SMS_SEND_LATER,              // temporary error happened while forward short message, transfer it again later
    ITAPI_SMS_NOT_SEND_AGAIN,          // permanent error happened while forward short message, SC is not making any more transfer attempts
    ITAPI_SMS_INVALID_STATUS_REPORT    // the invalid status report 
}ITAPI_SMS_TP_STATUS_E;

/*
the SMS mem full status
*/
typedef enum _ITAPI_SMS_MEM_FULL_STATUS_E
{
    ITAPI_SMS_ME_FULL,              // me memory full
    ITAPI_SMS_SIM_FULL,             // SIM memory full
    ITAPI_SMS_ALL_FULL,             // all memory full
    ITAPI_SMS_MEM_AVAIL             // memory available
}ITAPI_SMS_MEM_FULL_STATUS_E;

/*
the long SMS limit condition
*/
typedef enum _ITAPI_SMS_LEN_LIMIT_E
{
    ITAPI_LEN_LIMIT_DEFAULT,        // default limit
    ITAPI_LEN_LIMIT_JAVA,           // for java support limit
    ITAPI_LEN_LIMIT_DM,             // for DM support limit
    ITAPI_LEN_LIMIT_VCARD           // for VCARD limit
}ITAPI_SMS_LEN_LIMIT_E;


// plmn status
typedef enum _ITAPI_PLMN_STATUS_E
{
    ITAPI_PLMN_NO_SERVICE,                        /* no service */
    ITAPI_PLMN_NORMAL_GSM_ONLY,                   /* voice service available */
    ITAPI_PLMN_NORMAL_GPRS_ONLY,                  /* data service available */
    ITAPI_PLMN_NORMAL_GSM_GPRS_BOTH,              /* voice and data service available */
    ITAPI_PLMN_EMERGENCY_ONLY,                    /* emergency service available */
    ITAPI_PLMN_EMERGENCY_SIM_INVALID,             /* emergency; MM in limited service state and no further PLMN access allowed 
                                                     until power off or new SIM inserted */
    ITAPI_PLMN_EMERGENCY_GPRS_ONLY,               /* data service availabe but emergency; MM in limited service state */
    ITAPI_PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY,   /* data service availabe but emergency; MM in limited service state and
                                                     no further PLMN access allowed until power off or new SIM inserted */
    ITAPI_PLMN_REGISTER_SERVICE,                  /* attaching after camping on */
    ITAPI_PLMN_REGISTER_GPRS_ONLY                 /* data service availabe but attaching; */
}ITAPI_PHONE_PLMN_STATUS_E;


typedef enum _ITAPI_CALL_TYPE_E
{
    ITAPI_CALL_TYPE_NORMAL,             /* gsm speech normal call  */
    ITAPI_CALL_TYPE_EMERGENCY,          /* emergency call  */
    ITAPI_CALL_TYPE_NONE                /* default value   */
}ITAPI_CALL_TYPE_E;

typedef enum _ITAPI_CALL_CLIR_E
{
    ITAPI_CALL_CLIR_SUPPRESSION,
    ITAPI_CALL_CLIR_INVOCATION,
    ITAPI_CALL_CLIR_NETWORK_DECIDE
}ITAPI_CALL_CLIR_E;

typedef enum _ITAPI_CALL_DISCONNECT_CAUSE_E
{
    ITAPI_CAUSE_UNASSIGNED_NO,
    ITAPI_CAUSE_NORMAL_CLEARING,
    ITAPI_CAUSE_USER_BUSY,
    ITAPI_CAUSE_NO_USER_RESPONDING,
    ITAPI_CAUSE_ALERTING_NO_ANSWER,
    ITAPI_CAUSE_CALL_REJECTED,
    ITAPI_CAUSE_NUMBER_CHANGED,
    ITAPI_CAUSE_EMERGENCY_CALL_ONLY,
    ITAPI_CAUSE_CALL_HAS_DISCONNECTED,
    ITAPI_CAUSE_REMOTE_USER_DISCONNECT,
    ITAPI_CAUSE_NETWORK_REJECT,
    ITAPI_CAUSE_NO_CELL,
    ITAPI_CAUSE_SUPPLEMENT_NOT_PROVIDE,
    ITAPI_CAUSE_RESOURCES_UNAV,
    ITAPI_CAUSE_UNKNOW
}ITAPI_CALL_DISCONNECT_CAUSE_E;

/* 
 the time stamp struct 
*/
typedef struct _ITAPI_SMS_TIME_STAMP_T
{
    uint8 year    ;
    uint8 month   ;
    uint8 day     ;
    uint8 hour    ;
    uint8 minute  ;
    uint8 second  ;
    uint8 timezone;
}ITAPI_SMS_TIME_STAMP_T , *ITAPI_SMS_TIME_STAMP_PTR_T;

typedef struct _ITAPI_CALLED_NUMBER_T 
{
    ITAPI_NUMBER_TYPE_E  number_type;
    uint16               num_len;
    uint8                party_num[ITAPI_MAX_ADDR_BCD_LEN];
}ITAPI_CALLED_NUMBER_T;

typedef struct _ITAPI_SMS_USER_DATA_T
{
    uint16  length;                                          // the length
    wchar_t user_data_arr [ ITAPI_SMS_USER_DATA_LENGTH + 1 ]; // the data buf 
}ITAPI_SMS_USER_DATA_T;

/*
 the user data head structure
*/
typedef struct _ITAPI_SMS_USER_DATA_HEAD_T
{
    uint8 length;                                          // the length
    uint8 user_header_arr [ ITAPI_SMS_USER_DATA_LENGTH ]; // the data buf 
} ITAPI_SMS_USER_DATA_HEAD_T;

/*
 the user valid data structure
*/
typedef struct _ITAPI_SMS_USER_VALID_DATA_T
{    
    uint16  length;                                                 // the length
    wchar_t *user_data_arr;
} ITAPI_SMS_USER_VALID_DATA_T;

/*
 the MO SMS structure
*/
typedef struct _ITAPI_SMS_MO_SMS_T
{
    BOOLEAN                      status_report_is_request;   // whether request the status report 
    ITAPI_CALLED_NUMBER_T        dest_addr_t;                // the destination address structure
    ITAPI_SMS_TIME_STAMP_T       time_stamp_t;               // the time when user edit the SMS 
    ITAPI_SMS_USER_VALID_DATA_T  user_data_t;
}ITAPI_SMS_MO_SMS_T;

typedef struct _ITAPI_SMS_ADDR_T {
    uint16 length;
    uint8  addr_arr_t[ITAPI_SMS_ADDR_MAX_LEN];
}ITAPI_SMS_ADDR_T;

typedef struct _ITAPI_L3_MSG_UNIT_T {
    uint16  length;
    uint8   l3_msg[ITAPI_MAX_L3_MESSAGE_SIZE];
}ITAPI_L3_MSG_UNIT_T;

typedef struct _ITAPI_SMS_T
{
    BOOLEAN                      status_report_is_request;   // whether request the status report 
    ITAPI_SMS_ALPHABET_TYPE_E    alphabet_type;              // the alphabet type
    ITAPI_SMS_TP_STATUS_E        tp_status;      // the status of the previously sent MO short message
    ITAPI_SMS_STATUS_E           status;            // the SMS status in SIM
    ITAPI_CALLED_NUMBER_T        dest_addr_t;       // the destination address structure
    ITAPI_SMS_USER_DATA_T        user_data_t;       // the user data structure
    ITAPI_SMS_TIME_STAMP_T       time_stamp_t;      // the time when user edit the SMS 
}ITAPI_SMS_T;

typedef struct
{
    ITAPI_SMS_STORAGE_E         storage;        /*!< where the short message is saved */
    uint16                      record_id;      /*!< the sms position */
    ITAPI_SMS_T                 sms_t;          /*!< the sms umion , mt or mo */
    ITAPI_DUAL_E                item_sys;       /*!< dual sim sys */
    BOOLEAN                     is_mms;
}ITAPI_READSMS_CNF_T;

typedef struct
{
    ITAPI_SMS_CAUSE_E           cause;          /*!< the operate cause */
    uint16                      record_id;      /*!< the sms position */
    ITAPI_SMS_STORAGE_E         storage;        /*!< where the short message is saved */
    ITAPI_DUAL_E                dual_sys;
}ITAPI_UPDATESMS_CNF_T;

typedef struct
{
    ITAPI_SMS_CAUSE_E           cause;          /*!< the operate cause */
    uint8                       tp_mr;
    ITAPI_DUAL_E                dual_sys;
}ITAPI_SENDSMS_CNF_T;

typedef struct
{
    ITAPI_SMS_CAUSE_E           cause;
    uint32                      save_num;
    uint16                      record_id_arr[ITAPI_SMS_SPLIT_MAX_NUM];
    ITAPI_SMS_STORAGE_E         storage;
    ITAPI_DUAL_E                dual_sys;
}ITAPI_WRITESMS_CNF_T;

typedef struct
{
    ITAPI_SMS_CAUSE_E           cause;          /*!< the operate cause */
    uint16                      record_id;      /*!< the parameter position */
    ITAPI_DUAL_E                dual_sys;
}ITAPI_WRITESMSPARAM_CNF_T;

typedef struct 
{
    ITAPI_SMS_CAUSE_E           cause;
    BOOLEAN                     is_receive_sms;
    ITAPI_DUAL_E                dual_sys;
}ITAPI_SETSMSRECEIVEFUN_CNF_T;

typedef struct
{
    ITAPI_SMS_MEM_FULL_STATUS_E mem_status;
    ITAPI_DUAL_E                dual_sys;
}ITAPI_MEMFULLSMS_IND_T;

typedef struct
{
    ITAPI_SMS_STORAGE_E         storage;            /*!< where the sms is saved */
    uint16                      record_id;          /*!< the relating sms position */
    ITAPI_SMS_T                 sms_t;              /*!< whether the short message is received by SME */
    ITAPI_L3_MSG_UNIT_T         msg_data;           /*!< Only used for ATC module, Not decoded message body*/
    ITAPI_DUAL_E                dual_sys;
}ITAPI_SMSREPORT_IND_T;

typedef struct
{
    ITAPI_SMS_CAUSE_E           cause;              /*!< the operate cause */
    uint16                      record_id;          /*!< the sms position */
    ITAPI_SMS_STORAGE_E         storage;            /*!< message class */
    BOOLEAN                     is_new_mms;
    ITAPI_SMS_T                 sms_t;              /*!< the mt sms structure */
    ITAPI_DUAL_E                dual_sys;
}ITAPI_SMSMT_IND_T;

typedef struct
{
    ITAPI_PHONE_PLMN_STATUS_E   plmn_status;        /* public lands mobile network(PLMN) status */
    BOOLEAN                     plmn_is_roaming;    /* if PLMN is roaming status */
    uint16                      mcc;                /* mobile country code */
    uint16                      mnc;                /* mobile network code */
    uint16                      mnc_digit_num;      /* number of mnc */
    uint16                      cell_id;            /* cell identity */
    uint16                      lac;                /* location area code */
}ITAPI_PLMN_INFO_T;

typedef struct _ITAPI_PHONE_SCELL_MEAS_T
{
    uint16          arfcn;              /* absolute radio frequency channel number */
    uint8           rxlev;              /* received signal level, scope 0 -- 31 */
    uint16          rxqual;             /* received signal quality, only call state, scope 0 -- 7, no call state invalid 99 */
    uint16          cell_id;            /* cell identity */
    uint16          lac;                /* location area code */
}ITAPI_PHONE_SCELL_MEAS_T;

typedef struct _ITAPI_IMSI_T
{
    uint8 imsi_len;
    uint8 imsi_val[ITAPI_MAX_IMSI_ARR_LEN];
}ITAPI_IMSI_T;

typedef struct
{
    uint8                       call_id;
    ITAPI_CALLED_NUMBER_T       called_num;
    ITAPI_DUAL_E                dual_sys;
}ITAPI_CALL_START_IND_T;

typedef struct
{
    uint8                       call_id;
    ITAPI_DUAL_E                dual_sys;
}ITAPI_CALL_SETUP_COMPLETE_IND_T;

typedef struct
{
    uint8                           call_id;
    ITAPI_CALL_DISCONNECT_CAUSE_E   disconnected_cause;  
    ITAPI_DUAL_E                    dual_sys;
}ITAPI_CALL_DISCONNECTED_IND_T;

typedef struct
{
    uint8                           call_id;
    ITAPI_CALL_DISCONNECT_CAUSE_E   err_cause;
    ITAPI_DUAL_E                    dual_sys;
}ITAPI_CALL_ERR_IND_T;

typedef struct
{
    uint8                           call_id;
    BOOLEAN                         calling_num_present; 
    ITAPI_CALLED_NUMBER_T           called_num;
    ITAPI_DUAL_E                    dual_sys;
}ITAPI_CALL_SETUP_IND_T;

typedef struct
{
    uint8                           call_id;
    BOOLEAN                         connected_num_present; 
    ITAPI_CALLED_NUMBER_T           connected_num;
    ITAPI_DUAL_E                    dual_sys;
}ITAPI_CALL_SETUP_COMPLETE_CNF_T;

/**--------------------------------------------------------------------------*
**                         STURCT DEFINITION                                *
**--------------------------------------------------------------------------*/
typedef struct ITAPI_T ITAPI_T;

CAF_INTERFACE(ITAPI_T)
{
    INHERIT_IUNKNOWN(ITAPI_T);
    int32     (*MakeVoiceCall)(ITAPI_T * tapi_ptr, const char * pszNumber, void *param_ptr, char *user_name_ptr, char *psw_ptr);
    int32     (*IsDataServiceReady)(ITAPI_T * tapi_ptr);
    int32     (*RegisterMsg)(ITAPI_T * tapi_ptr, CAF_HANDLE_T dest_handle, uint16 start_msg, uint16 end_msg);
    int32     (*EnableMsgPostWork)(ITAPI_T * tapi_ptr, BOOLEAN  need_posttransfer);
    int32     (*SmsGetSmsNum)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 *max_sms_num_ptr, uint16 *used_sms_num_ptr);
    int32     (*SmsGetSmsStatus)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 *max_sms_num_ptr, ITAPI_SMS_STATUS_E *status_arr);
    int32     (*SetStoragePriority)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN is_sim_first);
    int32     (*GetStoragePriority)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN *is_sim_first_ptr);
    int32     (*ReadSms)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 record_id);
    int32     (*UpdateSms)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 record_id, ITAPI_SMS_STATUS_E status);
    int32     (*SendSms)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_MO_SMS_T *sms_ptr, ITAPI_SMS_STORAGE_E sms_write, ITAPI_SMS_PATH_E sms_path, BOOLEAN is_more);
    int32     (*SendPduSms)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_ADDR_T *addr_ptr, ITAPI_L3_MSG_UNIT_T  *tpdu_ptr, BOOLEAN is_more);
    int32     (*WriteSms)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, ITAPI_SMS_STATUS_E status, ITAPI_SMS_MO_SMS_T *sms_ptr);
    int32     (*SetSmsCenterAddr)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALLED_NUMBER_T *tp_sc_addr);
    int32     (*GetSmsCenterAddr)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALLED_NUMBER_T *tp_sc_addr);
    int32     (*GetSmsErrCode)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys);
    int32     (*SetSmsReceiveFunc)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN  is_receive_sms);
    int32     (*GetPlmnInfo)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_PLMN_INFO_T *tapi_plmn_info_ptr);
    int32     (*GetRssi)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_PHONE_SCELL_MEAS_T *tapi_cell_info_ptr);
    int32     (*GetImei)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 *imei_ptr, uint16 *imei_len_ptr, uint16 imei_max_len);
    int32     (*GetDualSimStatus)(ITAPI_T * tapi_ptr, BOOLEAN *is_sim1_ok_ptr, BOOLEAN *is_sim2_ok_ptr);
    int32     (*GetImsi)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_IMSI_T *imsi_ptr);
    int32     (*StartCall)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALL_TYPE_E call_type, ITAPI_CALLED_NUMBER_T *called_num_ptr, ITAPI_CALL_CLIR_E clir_type);
    int32     (*ConnectCall)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 call_id);
    int32     (*DisConnectCall)(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 call_id);
};

// Define pointer to poit the virtual table of the control base class
struct ITAPI_T
{
    CAF_VTBL_T(ITAPI_T)  *vtbl_ptr;
};

/*****************************************************************************/
//  Description : tapi query interface
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
static __inline CAF_RESULT_E ITAPI_QueryInterface( ITAPI_T *tapi_ptr, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->QueryInterface( tapi_ptr, guid, object_pptr );
}

/*****************************************************************************/
//  Description : tapi add ref
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
static __inline uint32 ITAPI_AddRef( ITAPI_T *tapi_ptr ) 
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->AddRef( tapi_ptr );
}

/*****************************************************************************/
//  Description : tapi release
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
static __inline uint32 ITAPI_Release( ITAPI_T *tapi_ptr ) 
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->Release( tapi_ptr );
}

/*****************************************************************************/
//  Description : data service built
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
static __inline int32 ITAPI_MakeVoiceCall(ITAPI_T * tapi_ptr, const char * pszNumber, void *param_ptr, char *user_name_ptr, char *psw_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->MakeVoiceCall( tapi_ptr, pszNumber, param_ptr, user_name_ptr, psw_ptr);
}

/*****************************************************************************/
//  Description : data service ready?
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
static __inline int32 ITAPI_IsDataServiceReady(ITAPI_T * tapi_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->IsDataServiceReady( tapi_ptr );
}

/*****************************************************************************/
//  Description : register the message to handle by start and end message.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
static __inline int32 ITAPI_RegisterMsg(ITAPI_T * tapi_ptr, CAF_HANDLE_T dest_handle, uint16 start_msg, uint16 end_msg)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->RegisterMsg( tapi_ptr, dest_handle, start_msg, end_msg );
}

/*****************************************************************************/
//  Description : enable the msg transfer to the post process or not.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
static __inline int32 ITAPI_EnableMsgPostWork(ITAPI_T * tapi_ptr, BOOLEAN  need_posttransfer)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->EnableMsgPostWork( tapi_ptr, need_posttransfer );
}

/*****************************************************************************/
//  Description : Get the SMS max support number and used number in the selected 
//                system and storage
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_SMS_GetSmsNum(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 *max_sms_num_ptr, uint16 *used_sms_num_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->SmsGetSmsNum( tapi_ptr, dual_sys, storage, max_sms_num_ptr, used_sms_num_ptr );
}

/*****************************************************************************/
//  Description : Get the SMS state and the max support number in the 
//                destination storage
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
static __inline int32 ITAPI_SMS_GetSmsStatus(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 *max_sms_num_ptr, ITAPI_SMS_STATUS_E *status_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->SmsGetSmsStatus( tapi_ptr, dual_sys, storage, max_sms_num_ptr, status_ptr );
}

/*****************************************************************************/
//  Description : set storage priority, SIM first or ME first
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
static __inline int32 ITAPI_SMS_SetStoragePriority(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN is_sim_first)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->SetStoragePriority( tapi_ptr, dual_sys, is_sim_first );
}

/*****************************************************************************/
//  Description : get storage priority, sim first or me first
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
static __inline int32 ITAPI_SMS_GetStoragePriority(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN *is_sim_first_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->GetStoragePriority( tapi_ptr, dual_sys, is_sim_first_ptr );
}

/*****************************************************************************/
//  Description : Read the the SMS according to the record_id 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
static __inline int32 ITAPI_SMS_ReadSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 record_id)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->ReadSms( tapi_ptr, dual_sys, storage, record_id );
}

/*****************************************************************************/
//  Description : update the SMS status in destination storage, the operation 
//                can delete an SMS or change the state from read to be read.
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
static __inline int32 ITAPI_SMS_UpdateSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 record_id, ITAPI_SMS_STATUS_E status)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->UpdateSms( tapi_ptr, dual_sys, storage, record_id, status );
}

/*****************************************************************************/
//  Description : APP originate an MO short message, send out SMS 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:   
/*****************************************************************************/
static __inline int32 ITAPI_SMS_SendSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_MO_SMS_T *sms_ptr, ITAPI_SMS_STORAGE_E sms_write, ITAPI_SMS_PATH_E sms_path, BOOLEAN is_more)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->SendSms( tapi_ptr, dual_sys, sms_ptr, sms_write, sms_path, is_more );
}

/*****************************************************************************/
//  Description : originate an PDU MO short message, send out SMS 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:   
/*****************************************************************************/
static __inline int32 ITAPI_SMS_SendPduSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_ADDR_T *addr_ptr, ITAPI_L3_MSG_UNIT_T  *tpdu_ptr, BOOLEAN is_more)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->SendPduSms( tapi_ptr, dual_sys, addr_ptr, tpdu_ptr, is_more );
}

/*****************************************************************************/
//  Description : save the MO SMS to the destination storage 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_SMS_WriteSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, ITAPI_SMS_STATUS_E status, ITAPI_SMS_MO_SMS_T *sms_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->WriteSms( tapi_ptr, dual_sys, storage, status, sms_ptr );
}

/*****************************************************************************/
//  Description : Set SC Address in SIM card
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
static __inline int32 ITAPI_SMS_SetSmsCenterAddr(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALLED_NUMBER_T *tp_sc_addr_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->SetSmsCenterAddr( tapi_ptr, dual_sys, tp_sc_addr_ptr );
}

/*****************************************************************************/
//  Description : Read SC Address which stored in SIM card
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_SMS_GetSmsCenterAddr(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALLED_NUMBER_T *tp_sc_addr_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->GetSmsCenterAddr( tapi_ptr, dual_sys, tp_sc_addr_ptr );
}

/*****************************************************************************/
//  Description : set the default index of parameters which save in SIM
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_SMS_GetErrCode(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->GetSmsErrCode( tapi_ptr, dual_sys );
}

/*****************************************************************************/
//  Description : judge whether receive sms
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_SMS_SetSmsReceiveFunc(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN  is_receive_sms)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->SetSmsReceiveFunc( tapi_ptr, dual_sys, is_receive_sms );
}

/*****************************************************************************/
//  Description : Get PLMN information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_PHONE_GetPlmnInfo(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_PLMN_INFO_T *tapi_plmn_info_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->GetPlmnInfo( tapi_ptr, dual_sys, tapi_plmn_info_ptr );
}

/*****************************************************************************/
//  Description : Get RSSI information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_PHONE_GetRssi(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_PHONE_SCELL_MEAS_T *tapi_cell_info_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->GetRssi( tapi_ptr, dual_sys, tapi_cell_info_ptr );
}

/*****************************************************************************/
//  Description : Get IMEI information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_PHONE_GetImei(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 *imei_ptr, uint16 *imei_len_ptr, uint16 imei_max_len)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->GetImei( tapi_ptr, dual_sys, imei_ptr, imei_len_ptr, imei_max_len );
}

/*****************************************************************************/
//  Description : Get dual sim status
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_SIM_GetDualSimStatus(ITAPI_T * tapi_ptr, BOOLEAN *is_sim1_ok_ptr, BOOLEAN *is_sim2_ok_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->GetDualSimStatus( tapi_ptr, is_sim1_ok_ptr, is_sim2_ok_ptr );
}

/*****************************************************************************/
//  Description : Get IMSI information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_SIM_GetImsi(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_IMSI_T *imsi_ptr)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->GetImsi( tapi_ptr, dual_sys, imsi_ptr );
}

/*****************************************************************************/
//  Description : APP task start a MO call.only one call is established at one 
//                time. 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_CALL_StartCall(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALL_TYPE_E call_type, ITAPI_CALLED_NUMBER_T *called_num_ptr, ITAPI_CALL_CLIR_E clir_type)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->StartCall( tapi_ptr, dual_sys, call_type, called_num_ptr, clir_type);
}

/*****************************************************************************/
//  Description : APP task accept the incoming call
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_CALL_ConnectCall(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 call_id)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->ConnectCall( tapi_ptr, dual_sys, call_id);
}

/*****************************************************************************/
//  Description :  APP task disconnect the call indicated by call_id
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
static __inline int32 ITAPI_CALL_DisConnectCall(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 call_id)
{
    CAF_ASSERT( PNULL != tapi_ptr );
    return CAF_GET_VTBLPTR( tapi_ptr, ITAPI_T )->DisConnectCall( tapi_ptr, dual_sys, call_id);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif //_CAFTAPI_H_
