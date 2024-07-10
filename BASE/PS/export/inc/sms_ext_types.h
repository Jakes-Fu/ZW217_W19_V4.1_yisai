/******************************************************************************
 ** File Name:      sms_ext_types.h                                          *
 ** Author:         Andy Huang                                                *
 ** Date:           2011-12-28                                                *
 ** Copyright:      Copyright 2001-2011 by Spreadtrum Communications,Inc.     *
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
 ** 2011-12-28     Andy Huang       Create.                                   *
 ******************************************************************************/

#ifndef SMS_EXT_TYPES_H
#define SMS_EXT_TYPES_H

#include "sci_types.h"

/*------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
#define SMT_CAUSE_BASE_VALUE      (0xFF)
#define SMR_CAUSE_BASE_VALUE      (0)
#define SMC_CAUSE_BASE_VALUE      (0xFFF)
#define SMS_TPDU_MAX_LENGTH       (SMR_SMT_RPDU_MAX_LENGTH)       // the max length of TPDU from SMS_TL or to SMS_TL
#define SMR_SMT_RPDU_MAX_LENGTH   (234)                           // the max length of RPDU from SMR or SMT
#define SMS_SCA_MAX_LENGTH        (11)                            // Digits

/*------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
typedef enum
{
    /* SMT cause */
    SMT_NO_ERROR                              = SMT_CAUSE_BASE_VALUE + 0x00,
    SMT_TELEMATIC_INTERWORKING_NOT_SUPPORTED  = SMT_CAUSE_BASE_VALUE + 0x80,
    SMT_SHORT_MESSAGE_TYPE0_NOT_SUPPORTED     = SMT_CAUSE_BASE_VALUE + 0x81,  
    SMT_CANNOT_REPLACE_SHORT_MESSAGE          = SMT_CAUSE_BASE_VALUE + 0x82,
    SMT_UNSPECIFIED_TP_PID_ERROR              = SMT_CAUSE_BASE_VALUE + 0x8F, 
    SMT_DATA_CODING_SCHEME_NOT_SUPPORTED      = SMT_CAUSE_BASE_VALUE + 0x90,
    SMT_MESSAGE_CLASS_NOT_SUPPORTED           = SMT_CAUSE_BASE_VALUE + 0x91,
    SMT_UNSPECIFIED_TP_DCS_ERROR              = SMT_CAUSE_BASE_VALUE + 0x9F,
    SMT_COMMAND_CANNOT_BE_ACTIONED            = SMT_CAUSE_BASE_VALUE + 0xA0,
    SMT_COMMAND_UNSUPPORTED                   = SMT_CAUSE_BASE_VALUE + 0xA1,
    SMT_UNSPECIFIED_TP_COMMAND_ERROR          = SMT_CAUSE_BASE_VALUE + 0xAF,
    SMT_SC_BUSY                               = SMT_CAUSE_BASE_VALUE + 0xC0,
    SMT_NO_SC_SUBSCRIPTION                    = SMT_CAUSE_BASE_VALUE + 0xC1,
    SMT_SC_SYSTEM_FAILURE                     = SMT_CAUSE_BASE_VALUE + 0xC2,
    SMT_INVALID_SME_ADDRESS                   = SMT_CAUSE_BASE_VALUE + 0xC3,
    SMT_DESTINATION_SME_BARRED                = SMT_CAUSE_BASE_VALUE + 0xC4,
    SMT_SM_REJECTED_DUPLICATE_SM              = SMT_CAUSE_BASE_VALUE + 0xC5,
    SMT_TP_VPF_NOT_SUPPORTED                  = SMT_CAUSE_BASE_VALUE + 0xC6,
    SMT_TP_VP_NOT_SUPPORTED                   = SMT_CAUSE_BASE_VALUE + 0xC7,
    SMT_SIM_SMS_STORAGE_FULL                  = SMT_CAUSE_BASE_VALUE + 0xD0,
    SMT_NO_SMS_STORAGE_CAPABILITY_IN_SIM      = SMT_CAUSE_BASE_VALUE + 0xD1,
    SMT_ERROR_IN_MS                           = SMT_CAUSE_BASE_VALUE + 0xD2,
    SMT_MEMORY_CAPACITY_EXCEEDED              = SMT_CAUSE_BASE_VALUE + 0xD3,
    SMT_SIM_APPLICATION_TOOLKIT_BUSY          = SMT_CAUSE_BASE_VALUE + 0xD4,
    SMT_SIM_DATA_DOWNLOAD_ERROR               = SMT_CAUSE_BASE_VALUE + 0xD5,
    SMT_UNSPECIFIED_ERROR_CAUSE               = SMT_CAUSE_BASE_VALUE + 0xFF,

    /* SMR cause */
    SMR_CAUSE_NO_ERROR                        = SMR_CAUSE_BASE_VALUE  + 0,      //NO ERROR
    SMR_CAUSE_UNASSIGNED_NUMBER               = SMR_CAUSE_BASE_VALUE  + 1,      //Unassigned (unallocated) number
    SMR_CAUSE_DETERMINED_BARING               = SMR_CAUSE_BASE_VALUE  + 8,      //Operator determined barring
    SMR_CAUSE_CALL_BARRED                     = SMR_CAUSE_BASE_VALUE  + 10,     //Call barred
    SMR_CAUSE_RESERVED                        = SMR_CAUSE_BASE_VALUE  + 11,     //Reserved
    SMR_CAUSE_SMTR_REJECTED                   = SMR_CAUSE_BASE_VALUE  + 21,     //Short message transfer rejected
    SMR_CAUSE_MEMORY_EXCEEDED                 = SMR_CAUSE_BASE_VALUE  + 22,     //Memory Capacity Exceeded
    SMR_CAUSE_DESTADDR_ERROR                  = SMR_CAUSE_BASE_VALUE  + 27,     //Destination out of order
    SMR_CAUSE_SUBSCRIBER_UNIDENT              = SMR_CAUSE_BASE_VALUE  + 28,     //Unidentified subscriber
    SMR_CAUSE_FACILITY_REJECTED               = SMR_CAUSE_BASE_VALUE  + 29,     //Facility rejected
    SMR_CAUSE_SUBSCRIBER_UNKNOWN              = SMR_CAUSE_BASE_VALUE  + 30,     //Unknown subscriber
    SMR_CAUSE_NETWORK_ERROR                   = SMR_CAUSE_BASE_VALUE  + 38,     //Network out of order
    SMR_CAUSE_TEMP_FAILURE                    = SMR_CAUSE_BASE_VALUE  + 41,     //Temporary failure
    SMR_CAUSE_CONGESTION                      = SMR_CAUSE_BASE_VALUE  + 42,     //Congestion
    SMR_CAUSE_RES_UNAVAILABLE                 = SMR_CAUSE_BASE_VALUE  + 47,     //Resources unavailable, unspecified
    SMR_CAUSE_REQFACI_NOTSPEC                 = SMR_CAUSE_BASE_VALUE  + 50,     //Requested facility not subscribed
    SMR_CAUSE_REQFACI_NOTIMPL                 = SMR_CAUSE_BASE_VALUE  + 69,     //Requested facility not implemented
    SMR_CAUSE_SMREFVAL_INVALID                = SMR_CAUSE_BASE_VALUE  + 81,     //Invalid short message transfer reference value
    SMR_CAUSE_MESSAGE_SEMAERROR               = SMR_CAUSE_BASE_VALUE  + 95,     //Semantically incorrect message
    SMR_CAUSE_MANDATORYINFO_ERROR             = SMR_CAUSE_BASE_VALUE  + 96,     //Invalid mandatory information
    SMR_CAUSE_MESSAGETYPE_ERROR               = SMR_CAUSE_BASE_VALUE  + 97,     //Message type non existent or not implemented
    SMR_CAUSE_MESSAGE_NOTINSTATE              = SMR_CAUSE_BASE_VALUE  + 98,     //Message not compatible with short message protocol state
    SMR_CAUSE_INFOELEM_ERROR                  = SMR_CAUSE_BASE_VALUE  + 99,     //Information element non existent or not implemented
    SMR_CAUSE_PROTOCOL_ERROR                  = SMR_CAUSE_BASE_VALUE  + 111,    //Protocol error, unspecified
    SMR_CAUSE_CONNECTEDID_ERROR               = SMR_CAUSE_BASE_VALUE  + 120,    //connected id between REQ and RES in MO entity
    SMR_CAUSE_INTERWORKING_ERROR              = SMR_CAUSE_BASE_VALUE  + 127,    //Interworking, unspecified
    SMR_CAUSE_TR1M_TIMEOUT                    = SMR_CAUSE_BASE_VALUE  + 253,    //TR1M TIMEOUT
    SMR_CAUSE_TR2M_TIMEOUT                    = SMR_CAUSE_BASE_VALUE  + 254,    //TR2M TIMEOUT

    /* SMC cause */
    SMC_CAUSE_NETWORK_FAILURE                   = SMC_CAUSE_BASE_VALUE + 17,      //Network failure
    SMC_CAUSE_CONGESTION                        = SMC_CAUSE_BASE_VALUE + 22,      //Congestion
    SMC_CAUSE_INVALID_TI_VALUE                  = SMC_CAUSE_BASE_VALUE + 81,      //Invalid Transaction Identifier value
    SMC_CAUSE_SEMANTICALLY_ERROR_MSG            = SMC_CAUSE_BASE_VALUE + 95,      //Semantically incorrect message
    SMC_CAUSE_INVALID_MANDATORY_INFO            = SMC_CAUSE_BASE_VALUE + 96,      //Invalid mandatory information
    SMC_CAUSE_MT_NON_EXISTENT_NOT_IMPLEMENTED   = SMC_CAUSE_BASE_VALUE + 97,      //Message type non-existent or not implemented
    SMC_CAUSE_MSG_NOT_COMPATIBLE_SMPT           = SMC_CAUSE_BASE_VALUE + 98,      //Message not compatible with the short message protocol state
    SMC_CAUSE_IE_NON_EXISTENT_NOT_IMPLEMENTED   = SMC_CAUSE_BASE_VALUE + 99,      //Information element non-existent or not implemented
    SMC_CAUSE_PROTOCOL_ERROR_UNSPECIFIED        = SMC_CAUSE_BASE_VALUE + 111,     //Protocol error, unspecified
    SMC_CAUSE_NO_ERROR                          = SMC_CAUSE_BASE_VALUE + 255,     //No error in SMC layer
    
    SMC_CAUSE_MO_TC1M_EXPIRED                   = SMC_CAUSE_BASE_VALUE + 256,     //SMC_MO_TC1M expired, the cause send to SMR by SMC_SMR_ERROR_IND
    SMC_CAUSE_MT_TC1M_EXPIRED                   = SMC_CAUSE_BASE_VALUE + 257,     //SMC_MO_TC1M expired, the cause send to SMR by SMC_SMR_ERROR_IND  
    SMC_CAUSE_MM_UE_ESTABLISH_FAILUE            = SMC_CAUSE_BASE_VALUE + 258,     //In UE MM/GMM error or establishe failure, the cause send to SMR by SMC_SMR_ERROR_IND
    SMC_CAUSE_MM_NW_ESTABLISH_FAILUE            = SMC_CAUSE_BASE_VALUE + 259,     //In NW MM/GMM error or establishe failure, the cause send to SMR by SMC_SMR_ERROR_IND
    SMC_CAUSE_MM_NW_SERVICE_NOT_SUPPORTED       = SMC_CAUSE_BASE_VALUE + 260, 
    SMC_CAUSE_MM_NW_NETWORK_FAILURE             = SMC_CAUSE_BASE_VALUE + 261,
    SMC_CAUSE_MM_UE_SERVICE_NOT_SUPPORTED       = SMC_CAUSE_BASE_VALUE + 262,
    SMC_CAUSE_MM_UE_TIMER_EXPIRED               = SMC_CAUSE_BASE_VALUE + 263,
    SMC_CAUSE_MM_UE_NO_REGISTRATION             = SMC_CAUSE_BASE_VALUE + 264,
    SMC_CAUSE_MM_CONNECTION_NOT_EXISTED         = SMC_CAUSE_BASE_VALUE + 265,
    SMC_CAUSE_MM_OUT_OF_SERVICE                 = SMC_CAUSE_BASE_VALUE + 266,
    SMC_CAUSE_CANCEL_SMS_SUCCESSFULLY           = SMC_CAUSE_BASE_VALUE + 267
}SMS_LOCAL_CAUSE_E;

typedef enum
{
    SMS_MSGTYPE_NULL = 0,                           
    SMS_MSGTYPE_MO   = 1,
    SMS_MSGTYPE_MT   = 2,
    SMS_MSGTYPE_ALL  = 3
}SMS_MSG_DIRECTION_E;

typedef enum
{
  CN_DOMAIN_CS                   = 0x0,
  CN_DOMAIN_PS                   = 0x1,
  CN_DOMAIN_INVALID              = 0xFF
} CN_DOMAIN_E;

typedef enum
{
    RAT_GSM                     = 0x0,        /* GSM OR GPRS*/
    RAT_UMTS_FDD                = 0x1,        /* UMTS FDD  */
    RAT_GSM_UMTS_FDD            = 0x2,
    RAT_UNKNOWM                 = 0xFF    
} RAT_MODE_E;

/*------------------------------------------------------------------------------
  Structs
  -------------------------------------------------------------------------------*/
typedef enum
{
    SMR_SMT_MEMORY_AVAILABLE_NOTIFY = 0,            //used for memory available notify
    SMR_SMT_MEMORY_AVAILABLE_ABORT  = 1             //used for memory available abort
}MEM_AVAIL_TYPE_E;

typedef struct
{
    uint8   address_len;
    uint8   address_val_arr[SMS_SCA_MAX_LENGTH];
}SMR_SMT_ADDRESS_T;

/*------------------------------------------------------------------------------
  message
  -------------------------------------------------------------------------------*/
typedef struct MN_SMR_DATA_REQ_s
{
    SIGNAL_VARS
    CN_DOMAIN_E         cn_domain;                      //PS OR CS
    RAT_MODE_E          rat_mode;                       //The current RAT mode ,GSM(A/Gb) or UMTS(Iu)
    SMR_SMT_ADDRESS_T   sca_address;                    //SCA, if no exists the len is 0
    BOOLEAN             is_more_sms;                    //Concate SMS
    uint8               tpdu_len;                       //the length of TPDU
    uint8               tpdu_arr[SMS_TPDU_MAX_LENGTH];  //TPDU buf    
} MN_SMR_DATA_REQ_T;

typedef struct SMR_MN_DATA_IND_s
{
    SIGNAL_VARS
    CN_DOMAIN_E         cn_domain;                      //PS OR CS
    SMR_SMT_ADDRESS_T   sca_address;                    //SCA, if no exists the len is 0
    uint8               tpdu_len;                       //the length of TPDU
    uint8               tpdu_arr[SMS_TPDU_MAX_LENGTH];  //TPDU buf    
} SMR_MN_DATA_IND_T;

typedef struct MN_SMR_REPORT_REQ_s
{
    SIGNAL_VARS
    SMS_LOCAL_CAUSE_E    cause_value;                        //RP-Message type 0:ACK,1....n:error
    uint8                tpdu_len;
    uint8                tpdu_arr[SMR_SMT_RPDU_MAX_LENGTH];  //MT RPDU buf
}MN_SMR_REPORT_REQ_T;

typedef struct SMR_MN_REPORT_IND_s
{
    SIGNAL_VARS
    SMS_MSG_DIRECTION_E     msg_direction;                     //Indicate from SMR_MT or SMR_MO
    SMS_LOCAL_CAUSE_E       cause_value;                       //RP-Message type 0:ACK,1....n:error
    uint8                   tpdu_len;
    uint8                   tpdu_arr[SMR_SMT_RPDU_MAX_LENGTH]; //MT RPDU buf
}SMR_MN_REPORT_IND_T;

typedef struct MN_SMR_MEMAVAILABLE_REQ_s
{
    SIGNAL_VARS
    CN_DOMAIN_E             cn_domain;                          //PS OR CS
    RAT_MODE_E              rat_mode;                           // The current RAT mode ,GSM(A/Gb) or UMTS(Iu)
    uint8                   mem_avail_info;                     //used for current msg is NOTIFY or ABORT not
}MN_SMR_MEMAVAILABLE_REQ_T;

typedef struct MN_SMR_CANCEL_SMS_REQ_s
{
    SIGNAL_VARS
    uint8  sms_id;
}MN_SMR_CANCEL_SMS_REQ_T;

typedef struct MNSMC_INIT_TASKID_REQ_s
{
    SIGNAL_VARS
    BLOCK_ID  smr_pid;    //smr_pid
    BLOCK_ID  mm_pid;     //mm_pid
    BLOCK_ID  llc_pid;    //llc_pid
} MNSMC_INIT_TASKID_REQ_T;

typedef struct MNSMR_INIT_TASKID_REQ_s
{
    SIGNAL_VARS
    BLOCK_ID  mn_pid;    //mn_pid
    BLOCK_ID  smc_pid;    //smc_pid
} MNSMR_INIT_TASKID_REQ_T;

#endif