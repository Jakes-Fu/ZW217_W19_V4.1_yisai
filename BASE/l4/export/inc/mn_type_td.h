/******************************************************************************
 ** File Name:      mn_type_td.h                                              *
 ** Author:         Fancier fan                                               *
 ** Date:           05/28/2002                                                *
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
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 05/28/2002     Fancier fan      Create.                                   *
 ******************************************************************************/
 

#ifndef _MN_TYPE_TD_H
#define _MN_TYPE_TD_H

#include "sci_types.h"
#include "os_api.h"
#include "mn_error.h"


#define MN_SMS_DLVR_SUCCESSFUL     0
#define MN_SMS_DLVR_UNSPECIFIED_ERR     255


#define EM_MAX_NUM_FREQ_RANGE     5
#define EM_BANDTYPE_AORF          1
#define EM_BANDTYPE_AANDF         2
#define EM_BAND_A_LOWER_FREQ      0x2746
#define EM_BAND_A_UPPER_FREQ      0x2789
#define EM_BAND_F_LOWER_FREQ      0x24BC
#define EM_BAND_F_UPPER_FREQ      0x257C

#define	MN_PHONE_SERVICE_DOMAIN_CS_ONLY			0x00
#define	MN_PHONE_SERVICE_DOMAIN_PS_ONLY		0x01
#define	MN_PHONE_SERVICE_DOMAIN_CS_PS_BOTH	0x02
#define MN_PHONE_SERVICE_DOMAIN_ANY    0x03

#define MN_ATC_MAX_PLMN_NUM  20

/* +vcard dev binli */
#define MN_USIM_EMAIL_STRING_MAX_LEN         255
#define MN_USIM_AAS_MAX_NUM                    3
#define MN_MAX_USIM_ANR_NUM                       3
#define MN_PHONEBOOK_ALPHA_STRING_MAX_LEN 241
#define MN_IAP_RECORD_MAX_LEN    6
/* -vcard dev binli */
//add for supporting VCARD
#define PHONEBOOK_EMAIL_STRING_MAX_LEN 		255
#define PHONEBOOK_ALPHA_STRING_MAX_LEN		255
#define MNSIM_ANR_MAX_NUM					3
#define MNSIM_EMAIL_MAX_NUM					1
#define USIM_AAS_MAX_NUM						3
#define USIM_GAS_MAX_NUM						20
#define PHONEBOOK_IAP_MAX_NUM				5 /*ANR * 3 + EMAIL * 1 +SNE  * 1*/
#define PHONEBOOK_MAX_USAGE_STORAGE_NUM     8 /*PBR numbers in BOTH TEL and ADF*/
#define PHONEBOOK_MAX_OPERATE_NUM                 6  

#define EM_PRI_BAND 0
#define EM_SEC_BAND 1
#define EM_PRI_SEC_BAN 2

#define MN_ENG_MAX_TDD_NCELL_NUM_OF_GSM 96



typedef int32 (* MN_DUMMYMMI_RECEIVER_FPTR)(
                const uint8* str_ptr,    // point to the string
                uint32 str_len     // received string length
                );


typedef enum _FLOW_CTRL_MOD_E
{
   FLOW_CTRL_MOD_MN_AL = 0,
   FLOW_CTRL_MOD_NAS,
   FLOW_CTRL_MOD_RABM,
   FLOW_CTRL_MOD_SNDCP,
   FLOW_CTRL_MOD_RLC 
}FLOW_CTRL_MOD_E;


typedef enum _MN_APN_TYPE_E
{
	MN_APN_TYPE_SIM,
	MN_APN_TYPE_USIM,
	MN_APN_TYPE_NUM
} MN_APN_TYPE_E;

typedef enum _MN_APN_NAME_CODE_SCHEM_E
{
	MN_APN_NAME_7BIT_ALPHA,
	MN_APN_NAME_UCS2,
	MN_APN_NAME_NUM
} MN_APN_NAME_CODE_SCHEM_E;

typedef enum _MN_SPN_TYPE_E
{
	MN_SPN_TYPE_SIM,
	MN_SPN_TYPE_USIM,
	MN_SPN_TYPE_NUM
} MN_SPN_TYPE_E;

typedef enum _MN_SPN_NAME_CODE_SCHEM_E
{
	MN_SPN_NAME_7BIT_ALPHA,
	MN_SPN_NAME_UCS2,
	MN_SPN_NAME_NUM
} MN_SPN_NAME_CODE_SCHEM_E;

typedef enum _MN_PLMN_LOCK_STATUS_E
{
    MN_PLMN_AUTO,
    MN_PLMN_MANUAL,
    MN_PLMN_LOCKED
} MN_PLMN_LOCK_STATUS_E;

typedef enum _MN_SP_LOCK_STATUS_E
{
    MN_SP_UNLOCKED,
    MN_SP_LOCKED
} MN_SP_LOCK_STATUS_E;

typedef enum
{
    MN_SMS_PS_DOMAIN_SERVICE = 0,
    MN_SMS_CS_DOMAIN_SERVICE,
    MN_SMS_PS_DOMAIN_SERVICE_PREF,
    MN_SMS_CS_DOMAIN_SERVICE_PREF,
    MN_SMS_NO_DOMAIN_SERVICE
}MN_SMS_SERVICE_TYPE_E;

/*PHONEBOOK_SIM_ENTRY_T definition is unused for 2G any more,
 but this is also used by 3G,so reserv this.*/
typedef struct _PHONEBOOK_SIM_ENTRY_T
{
    uint8  alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN];  //the alpha id 
    MNBCD number[PHONEBOOK_MAX_DN_NUMBER_LEN];  //the dialling number    
    uint16 entry_id;
    uint8  alpha_len;
    uint8  number_len;
    uint8  npi_ton;         // low byte ton,high byte is npi
} PHONEBOOK_SIM_ENTRY_T;

typedef struct _CALLLOG_ENTRY_T
{
    uint8   alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN]; //the alpha id 
    MNBCD number[PHONEBOOK_MAX_DN_NUMBER_LEN];  //the dialling number    
    MN_NUMBER_TYPE_E   ton;        //type of number
    MN_NUMBER_PLAN_E   npi;        //the Number plan identifier
    uint32          time;
    uint32          dur_time;
    uint8           alpha_len;
    uint8           number_len;
    uint16          entry_id;

} CALLLOG_ENTRY_T;

typedef enum _MN_PHONE_SIM_STATE_E
{
	MN_PHONE_SIM_INVALID,
	MN_PHONE_SIM_VALID,
	MN_PHONE_SIM_UNKNOW = 255
} MN_PHONE_SIM_STATE_E;

typedef enum
{
    MN_GPRS_PDP_TYPE_X25,
    MN_GPRS_PDP_TYPE_IP,
    MN_GPRS_PDP_TYPE_OSPIH,
    MN_GPRS_PDP_TYPE_PPP
}MN_GPRS_PDP_TYPE_E;     //the type of the PDP

typedef enum _MN_SMS_INDICATION_TYPE_E
{
	dummy_sms_indication
} MN_SMS_INDICATION_TYPE_E;

/*+michael for usat dev*/
typedef enum _MN_SMS_DLVR_RPT_E
{
   MN_SMS_ACK_DELIVER_REPORT,
   MN_SMS_NACK_DELIVER_REPORT

}MN_SMS_DLVR_RPT_E;    

typedef struct
{
	SIGNAL_VARS
}APP_MN_SMS_MEM_OVER_FLOW_IND_T;

/*
    Indicate reading SMS states
*/
typedef struct
{
   SIGNAL_VARS
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_READING_SMS_STATES_IND_T;
/*
    Indicate reding SMS
*/
typedef struct
{
   SIGNAL_VARS
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_READING_SMS_IND_T;

/*
    Indicate updating SMS state
*/
typedef struct
{
   SIGNAL_VARS
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_UPDATING_SMS_STATE_IND_T;

/*
    Indicate sending SMS
*/
typedef struct
{
   SIGNAL_VARS
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_SENDING_SMS_IND_T;

/*
    Indicate saving SMS 
*/
typedef struct
{
   SIGNAL_VARS
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_SAVING_SMS_IND_T;

/*
    Indicate reading SMS param;
*/
typedef struct
{
   SIGNAL_VARS
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_READING_SMS_PARAM_IND_T;

/*
    Indicate writing SMS param
*/
typedef struct
{
   SIGNAL_VARS
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_WRITING_SMS_PARAM_IND_T;

typedef enum MN_PHONE_DM_PREFER_RAT_E
{
    MN_PHONE_DM_PREFER_AUTO = 0,
	MN_PHONE_DM_PREFER_GSM,
	MN_PHONE_DM_PREFER_TD,
	MN_PHONE_DM_PREFER_VDS_GSM = 4       //refer to the note of MN_GMMREG_RAT_VDS_GSM
}MN_PHONE_DM_PREFER_RAT_E;

typedef uint8	MN_PHONE_SERVICE_DOMAIN_E;

typedef enum _MN_PHONE_SERVICE_STATUS_E
{
	MN_PHONE_NO_SERVICE = 0,
	MN_PHONE_LIMITED_SERVICE = 0x01,
	MN_PHONE_FULL_SERVICE = 0x02
}MN_PHONE_SERVICE_STATUS_E;

typedef enum _MN_PHONE_RAT_E
{
	MN_PHONE_RAT_NONE = 0x00,
	MN_PHONE_RAT_RESERVE = 0x01,	
	MN_PHONE_RAT_GSM = 0x03,
	//MN_PHONE_RAT_TD = 0x04,
	MN_PHONE_RAT_WCDMA = 0x05,
	MN_PHONE_RAT_TD = 0x0F
}MN_PHONE_RAT_E;

#define MN_PHONE_RAT_GPRS MN_PHONE_RAT_GSM
#define MN_PHONE_RAT_WCDMA MN_PHONE_RAT_TD

 typedef enum _MN_PHONE_SYSTEM_MODE_E
{
	MN_PHONE_SYSTEM_MODE_AUTO = 2,
	MN_PHONE_SYSTEM_MODE_GSM = 13,
	MN_PHONE_SYSTEM_MODE_WCDMA = 14,
	MN_PHONE_SYSTEM_MODE_TD = 15
}MN_PHONE_SYSTEM_MODE_E;

#define MN_PHONE_SYSTEM_MODE_GPRS MN_PHONE_SYSTEM_MODE_GSM

typedef struct  _MN_PHONE_RAT_INFO_T
{
	MN_GMMREG_RAT_E rat;/*single mode for UE(gsm:0 td:1 dual mode:2 Unknow:3)*/
	MN_GMMREG_RAT_E dm_prefer_rat;/*preferred mode only when UE in dual mode
	                               (gsm prefer:0 td prefer:1 unknow:3)
	                              This item is invalid when UE in single mode.*/
} MN_PHONE_RAT_INFO_T;

typedef struct _MN_PHONE_RESELECT_PARAM_T
{
	MN_PHONE_PREF_MODE_E pref;
	MN_PHONE_BAND_TYPE_E band;
	MN_PHONE_USER_SERVICE_SELECT_E service_type;
	MN_PHONE_ROAMING_MODE_E	roam_mode;
} MN_PHONE_RESELECT_PARAM_T;

typedef enum mn_protocol_stack_sub_state_enum
{
    MN_PROTOCOL_STACK_LIMITED_SERVICE_MODE,
    MN_PROTOCOL_STACK_FULL_MODE,
    MN_PROTOCOL_STACK_NUM
}mn_protocol_stack_sub_state_enum;

typedef struct 
{
    SIGNAL_VARS
    BOOLEAN                 set_result;  
} APP_MN_PREFER_RAT_RSP_T;

typedef struct 
{
    SIGNAL_VARS
    MN_GMMREG_RAT_E         rat;
} APP_MN_CURRENT_RAT_IND_T;

typedef struct 
{
    SIGNAL_VARS
    uint8             *msg_ptr;
    uint8             *msg_type_ptr;
    uint32                  pdu_len;
    uint8                  *pdu_data_ptr;
} APP_MN_AT_TRACE_IND_T;

typedef struct 
{
    SIGNAL_VARS
    MN_PHONE_CURR_CELL_CAPABILITY_T cell_capability;
} APP_MN_CURR_CELL_CAPABILITY_IND_T;

//  struct of signal: APP_MN_PLMN_READ_CNF_T
//  this signal indicate detail of plmn read from sim
typedef struct 
{
    SIGNAL_VARS
    MN_PHONE_OPERATE_RESULT_E     is_valid_result;
    uint8		     	          plmn_num;                           // plmn_num
    MN_PLMN_T                     plmn[MN_PHONE_MAX_PLMN_INFO_LIST_NUM]; // plmn_detail
    BOOLEAN                       is_valid_plmn[MN_PHONE_MAX_PLMN_INFO_LIST_NUM];
    BOOLEAN                       is_gsm_act[MN_PHONE_MAX_PLMN_INFO_LIST_NUM];
    BOOLEAN                       is_gsm_compact_act[MN_PHONE_MAX_PLMN_INFO_LIST_NUM];
    BOOLEAN                       is_td_act[MN_PHONE_MAX_PLMN_INFO_LIST_NUM];
} APP_MN_PLMN_READ_CNF_T;

//  struct of signal: APP_MN_PLMN_WRITE_CNF_T
//  this signal indicate that PLMN write to sim complete
typedef struct 
{
    SIGNAL_VARS
    BOOLEAN         is_write_successful;
} APP_MN_PLMN_WRITE_CNF_T;

typedef struct
{
    BOOLEAN is_ok;
} APP_MN_READ_AUTH_FILE_RESULT_T;

typedef struct
{
    SIGNAL_VARS
    APP_MN_READ_AUTH_FILE_RESULT_T  result;  
    MN_DUAL_SYS_E                   dual_sys;
    
}APP_MN_READ_AUTH_FILE_CNF_T;

typedef enum MN_ATC_STK_MODE_E
{
    MN_ATC_STK_MODE_NOT_SUPPORT,
    MN_ATC_STK_MODE_OMS,
    MN_ATC_STK_MODE_SP
} MN_ATC_STK_MODE_E;

typedef struct
{
    BOOLEAN is_ok;
}APP_MN_SET_EXT_PROFILE_RESULT_T;

typedef struct
{
    SIGNAL_VARS
    APP_MN_SET_EXT_PROFILE_RESULT_T	result;  
    MN_DUAL_SYS_E                   dual_sys;
} APP_MN_SET_EXT_PROFILE_CNF_T;

typedef enum
{
    MN_SIM_SERVICE_NOT_SUPPORT,
    MN_SIM_SERVICE_ENABLED,
    MN_SIM_SERVICE_DISABLED
} MN_SIM_SERVICE_E;

typedef enum
{
   MN_SIM_ENHANCED_SEARCH_FROM_OFFSET_FORWARD = 0x04,
   MN_SIM_ENHANCED_SEARCH_FROM_OFFSET_BACKWARD = 0x05,
   MN_SIM_ENHANCED_SEARCH_FROM_OFFSET_FORWARD_FROM_NEXT_REC = 0x06,
   MN_SIM_ENHANCED_SEARCH_FROM_OFFSET_BACKWARD_FROM_NEXT_REC = 0x07,
   MN_SIM_ENHANCED_SEARCH_FROM_VALUE_FORWARD = 0x0C,
   MN_SIM_ENHANCED_SEARCH_FROM_VALUE_BACKWARD = 0x0D,
   MN_SIM_ENHANCED_SEARCH_FROM_VALUE_FORWARD_FROM_NEXT_REC = 0x0E,
   MN_SIM_ENHANCED_SEARCH_FROM_VALUE_BACKWARD_FROM_NEXT_REC = 0x0F,
   MN_SIM_ENHANCED_SEARCH_RFU = 0xFF
}MN_SIM_ENHANCED_SEARCH_IND_E;

typedef enum
{
    MN_SIM_SIMPLE_SEARCH_FORWARD = 0x04,
    MN_SIM_SIMPLE_SEARCH_BACKWARD = 0x05,
    MN_SIM_ENHANCED_SEARCH = 0x06,
    MN_SIM_PROPRIETARY_SEARCH = 0x07
}MN_SIM_SEARCH_MODE_E;

typedef struct
{
    MN_SIM_SEARCH_MODE_E search_mode;   //Search mode, For Simple search and Enhanced
    MN_SIM_ENHANCED_SEARCH_IND_E enhanced_search_ind;   //The first byte of data in enhanced search
    uint8 enhanced_search_offset;   //The second byte of data in enhanced search
    uint8 rec_num;    //The base record
    uint8 search_string_len;   //The length of the search record reference data
    uint8 *search_string_ptr;   //the search reference data
}MN_SIM_SEARCH_RECORD_T;


//  struct of signal: APP_MN_ACL_SERVICE_IND_T
//  this signal is indicationg of acl service status
typedef struct
{
    SIGNAL_VARS
    BOOLEAN         is_acl_enable;
    MN_DUAL_SYS_E   dual_sys;
}APP_MN_ACL_SERVICE_IND_T;

typedef struct
{
    uint8     *data_ptr;
    uint16    data_length;/* rec_len*num_records */
    uint8     num_records;/* rec numbers in one read or ... time*/
} MN_SIM_PARA_VALUE_T;

//  struct of signal: APP_MN_SIM_READ_CNF_T
//  this signal is cnf SIM and  EF read
typedef struct
{
    SIGNAL_VARS
    MN_SIM_FILE_READ_RESULT_E       result;		//the Read file result
    uint16	                                        file_id;
    uint8                                           rec_num;
    MN_SIM_PARA_VALUE_T                 read_data;
    MN_DUAL_SYS_E                       dual_sys;
} APP_MN_SIM_READ_CNF_T,APP_MN_RECORD_READ_CNF_T;

//  struct of signal: APP_MN_SIM_WRITE_CNF_T
//  this signal is cnf SIM EF write
typedef struct
{
    SIGNAL_VARS
    BOOLEAN                     is_ok; //indicate whether successfully
    uint16                          file_id;
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_SIM_WRITE_CNF_T,APP_MN_RECORD_UPDATE_CNF_T;

typedef enum
{
    MN_SIM_OP_ACC_ALOW,
    MN_SIM_OP_ACC_PIN1,
    MN_SIM_OP_ACC_PIN2,
    MN_SIM_OP_ACC_PIN_ADM,
    MN_SIM_OP_ACC_PIN_UNIVERSAL,
    MN_SIM_OP_ACC_NEVER,
    MN_SIM_OP_ACC_PIN_RFU
}MN_SIM_OP_ACC_PIN_E;

typedef enum
{
    MN_SIM_FILE_VALIDATED,
    MN_SIM_FILE_NOT_READABLE_UPDATABLE_WHEN_INVALIDATED,
    MN_SIM_FILE_READABLE_UPDATABLE_WHEN_INVALIDATED,
    MN_SIM_FILE_STATUS_UNKOWN
}MN_SIM_FILE_STATUS_E;

typedef enum
{
   MN_SIM_FILE_DEACTIVATED = 0,
   MN_SIM_FILE_ACTIVATED,
   MN_SIM_FILE_STATUS_UNKNOWN
}MN_SIM_LIFE_CYCLE_STATUS_E;


typedef struct
{
    MN_SIM_EF_STRUCTURE_E         file_struct;	//TRANSPARENT,LINEAR_FIXED or CYCLIC
    MN_SIM_FILE_STATUS_E            bit3_status; // file status, valid for 2G files only
    MN_SIM_LIFE_CYCLE_STATUS_E  file_life_cycle;  //Life cycle for USIM or file status for SIM
    MN_SIM_OP_ACC_PIN_E              read_acc;   //read access attibute
    MN_SIM_OP_ACC_PIN_E              write_acc;  //write access attibute
    MN_SIM_OP_ACC_PIN_E              invalidate_acc;   //invalidate access attibute
    MN_SIM_OP_ACC_PIN_E              rehabilitate_acc; // rehabilitate access attibute
    uint16                                      file_size;        // the size of file
    uint8                                       record_num;	//the record num of the record file, 1 if transparent file
} APP_MN_GET_SIM_EF_ENTIRE_INFO_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN                                             is_ok; //indicate whether successfully
    uint16	                                                file_id;	//file id of the file
    APP_MN_GET_SIM_EF_ENTIRE_INFO_T     ef_entire_info;
    MN_DUAL_SYS_E                                   dual_sys;
} APP_MN_GET_SIM_EF_ENTIRE_INFO_CNF_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN                              is_ok; //indicate whether successfully
    uint16	                             file_id;	//file id of the file
    MN_SIM_PARA_VALUE_T                  search_result;   //The record number given from SIM
    MN_DUAL_SYS_E                        dual_sys;
} APP_MN_SIM_SEARCH_RECORD_CNF_T;

typedef enum _MN_CALL_MODE_E
{
    MN_CALL_SINGLE_MODE,
    MN_CALL_ALTER_VOICE_FAX,
    MN_CALL_ALTER_VOICE_DATA,
    MN_CALL_VOICE_THEN_DATA,
    MN_CALL_MM_NO_FALLBACK           =129,    //Proprietary value. Multimedia call only, without fallback or service change
    MN_CALL_MM_FALLBACK_SPEECH       =130,    //Proprietary value. Multimedia call with fallback to speech
    MN_CALL_MM_FALLBACK_N_SERVICE    =131     //Proprietary value. Multimedia call with fallback and service change (allowed for UDI/RDI call only)
} MN_CALL_MODE_E;

typedef enum _MN_SPEECH_SERVICE_ID_E
{
    MN_SPEECH_ONLY_FULL_RATE = 0,
    MN_SPEECH_DUAL_HALF_PRE_RATE,
    MN_SPEECH_DUAL_FULL_PRE_RATE,
    //Add the data and fax service id
    MN_MAX_SERVICE_ID

} MN_SPEECH_SERVICE_ID_E;

// inform that the network has accepted or reject the SWAP request
typedef struct
{
    SIGNAL_VARS
    uint8                 hold_call_id;
    uint8                 retrieve_call_id;
    BOOLEAN               req_is_accepted; // indicate the request of swapping call is accepted or rejected
    uint8                 cuase; /* refer to 3GPP 24.008 Annex H */
    MN_DUAL_SYS_E         dual_sys;
} APP_MN_CALL_SWAP_CNF_T;

// inform that the procedure of swap is on
typedef struct
{
    SIGNAL_VARS
    uint8                 hold_call_id;
    uint8                 retrieve_call_id;
    MN_DUAL_SYS_E         dual_sys;
} APP_MN_CALL_SWAP_IND_T;

typedef enum {
    TC_CONVERSATIONAL=1,
    TC_STREAMING,
    TC_INTERACTIVE,
    TC_BACKGROUND
}TRAFFIC_CLASS_E;

typedef enum _MNSMS_SERVICE_E
{
    MNSMS_SERV_PS,
    MNSMS_SERV_CS,
    MNSMS_SERV_PS_PREFER,
    MNSMS_SERV_CS_PREFER,
    MNSMS_SERV_UNKNOWN
}MNSMS_SERVICE_E;

typedef enum{
    MN_GRPS_PRECEDENCE_CLASS_SUBSCRIBED,
    MN_GPRS_PRECEDENCE_CLASS_HIGH_PRIORITY,
    MN_GPRS_PRECEDENCE_CLASS_NORMAL_PRIORITY,
    MN_GPRS_PRECEDENCE_CLASS_LOW_PRIORITY,
    MN_GPRS_PRECEDENCE_CLASS_RESERVED
}MN_GRPS_PRECEDENCE_CLASS_TYPE_E;

//add the CS part,maybe the name should be modified
typedef struct
{
    uint8 csdata_speed;
    uint8 csdata_name;
    uint8 csdata_ce;
    uint8 csdata_reserve;
}CSDATA_BC_CONF_T;

// the callback function for csdata user when receiving data
typedef int32 (* MNCSDATA_RECEIVEDATA_FPTR)(
        uint8* buff_ptr,    // pointer of received data buff
        uint32 buff_size,       // received data length
        uint16 rlc_id
        );
typedef void (* MNCSDATA_FLOWCONTROL_FPTR)();
/* +cr254874 */
typedef void (* MNCSDATA_SENDDATA_TRIGGER_FPTR)();
/* -cr254874 */
// for cs data api
typedef struct
{
    SIGNAL_VARS
    uint8 *ptr; // pointer of received data buff
    uint32 len;   // received data length
    uint16 rlc_id;
} SN_DATA_IND_T;

typedef struct
{
    uint8   c1;
    uint8   c2;
    uint8   ncc_permitted;
    uint8   ms_txpwr_maxcch;
    uint8   rxlev_access_min;
    uint8   cbq;
    uint8   cell_resel_off;
    uint8   temporary_off;
    uint8   penalty_time;
    uint8   cell_prio;
    uint8   priority_access_thr;
} MN_PHONE_SCELL_GSM_INFO_T;

typedef struct
{
    uint8   att;
    uint8   drxParameter[3]; //utran, cs, ps
    uint8   cellIdentifier[4];  /*george change to 4*/
    uint16  mcc;
    uint16  mnc;
    uint16  lac;
    uint16  t3212Len;
    uint16  rac;
    uint16  nmo;
    uint16  t3302Len;
    uint16  t3312Len;
    uint8   ptmsi[4];   
} MN_ENG_MM_GMM_INFO_T;

typedef struct MN_FREQ_INFO_T
{
    uint32  freq;
    uint8   cell_num;
    uint8   pend;
    uint16  cell_id[3];
} MN_FREQ_INFO_T;

typedef struct MN_EM_LOCK_FREQ_CELL_T
{
    uint32  msg_id;
    uint16  length;
    uint8   freq_num;
    uint8   pend;
    MN_FREQ_INFO_T freq_info[4];
} MN_EM_LOCK_FREQ_CELL_T;

typedef struct MN_UE_ID_INFO_T
{
    uint8   imsi_v[15];
    uint8   imei_v[17];
    uint8   imeisv_v[17];
    uint8   tmsi_v[4];
    uint8   ptmsi_v[4];
} MN_UE_ID_INFO_T;

typedef enum
{   
    MN_SIM_ACCESS_TYPE_UTRAN,
    MN_SIM_ACCESS_TYPE_GSM,
    MN_SIM_ACCESS_TYPE_OTHER
}mn_sim_access_type_enum;
typedef enum _MN_SIM_FILE_TYPE_E
{
    MN_SIM_PL,
    MN_SIM_ECC,
    MN_SIM_SST,
    MN_SIM_UST,
    MN_SIM_EST,
    MN_SIM_FDN,
    MN_SIM_BDN,
    MN_SIM_ACL,
    MN_SIM_TOT
} MN_SIM_FILE_TYPE_E;
typedef struct _MN_SIM_PLMN_INFO_T
{
	uint8	aucPlmn[3];
	uint8	aucPlmnTecId[2];
} MN_SIM_PLMN_INFO_T;
typedef struct _MN_ATC_PLMN_INFO_T
{
	uint8	aucPlmn[3];   /* BYTE type ,not BCD*/
	mn_sim_access_type_enum	 access_type_en;
} MN_ATC_PLMN_INFO_T;

typedef struct _MN_ATC_UPLMN_WACT_
{
	uint8	plmn_num;
	MN_ATC_PLMN_INFO_T	plmn_info[MN_ATC_MAX_PLMN_NUM];
} MN_ATC_UPLMN_WACT_T;   

typedef struct _MN_SIM_UPLMN_WACT_T
{
	uint8	plmn_num;
	MN_SIM_PLMN_INFO_T	plmn_info[MN_ATC_MAX_PLMN_NUM];
} MN_SIM_UPLMN_WACT_T;   

typedef enum _MN_ACCESS_STRATUM_REL_IND_E
{
    MN_ACC_STR_REL4,
    MN_ACC_STR_REL5
} MN_ACCESS_STRATUM_REL_IND_E;

typedef enum _MN_SIM_CARD_NUMBER_E
{
    MN_SIM_CARD_ONE,
    MN_SIM_CARD_TWO
} MN_SIM_CARD_NUMBER_E;


typedef struct APP_MN_PROFILE_DATA_T
{
    BOOLEAN is_valid;
    uint16 ext_data_len;
    uint8  ext_data[40] ;
} APP_MN_PROFILE_DATA_T; 

typedef struct PHONEBOOK_EF_USAGE_T
{
    uint8 max_space_num; /*Total number*/
    uint8 free_space_num;  /*Unused number*/
    uint8 rec_len;  /*Length of each record*/
}PHONEBOOK_EF_USAGE_T;

typedef struct PHONEBOOK_STORAGE_USAGE_T
{
    PHONEBOOK_EF_USAGE_T adn_usage;
    PHONEBOOK_EF_USAGE_T anr_usage[MNSIM_ANR_MAX_NUM];
    PHONEBOOK_EF_USAGE_T email_usage;
    PHONEBOOK_EF_USAGE_T sne_usage;
    PHONEBOOK_EF_USAGE_T ext1_usage;
}PHONEBOOK_STORAGE_USAGE_T;

typedef struct PHONEBOOK_VCARD_STATUS_T
{
    uint8 storage_num;  /*Actual PBR number in BOTH TEL and ADF in current USIM*/
    PHONEBOOK_STORAGE_USAGE_T storage_usage[PHONEBOOK_MAX_USAGE_STORAGE_NUM];
}PHONEBOOK_VCARD_STATUS_T;

typedef enum PHONEBOOK_ADD_UPDATE_OPERATION_E
{
    OPERATE_USIM_INSERT,
    OPERATE_USIM_DELETE,
    OPERATE_USIM_MODIFY,
    OPERATE_USIM_NONE
}PHONEBOOK_ADD_UPDATE_OPERATION_E;

typedef struct PHONEBOOK_IAP_ENTRY_T
{
    uint8   iap_data[MN_IAP_RECORD_MAX_LEN];	
    uint8   iap_data_len;
} PHONEBOOK_IAP_ENTRY_T; 

typedef struct PHONEBOOK_UPDATE_INFO_ENTRY_T
{
    PHONEBOOK_ENTRY_T adn_info;
    PHONEBOOK_IAP_ENTRY_T iap_info;
		PHONEBOOK_ADD_UPDATE_OPERATION_E anr[MN_MAX_USIM_ANR_NUM];
    PHONEBOOK_ADD_UPDATE_OPERATION_E email;
    PHONEBOOK_ADD_UPDATE_OPERATION_E sne;  
} PHONEBOOK_UPDATE_INFO_ENTRY_T;

/* +vcard dev binli */
 /*****************************************************************************
 **                        VCARD SUPPORT DEFINES                                  *
 ******************************************************************************/

typedef struct PHONEBOOK_ALPHA_STRING_T_s
{
    uint8   alpha[MN_PHONEBOOK_ALPHA_STRING_MAX_LEN];	
    uint8   alpha_num;
} PHONEBOOK_ALPHA_STRING_T;

typedef struct PHONEBOOK_USER_INFO_T
{
    uint8 		path_len;
    uint8 		*path_ptr;
    uint8 		rec_id;
}PHONEBOOK_USER_INFO_T;

typedef struct PHONEBOOK_AAS_ENTRY_T
{
    uint8   alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN];
    uint8			alpha_len;
} PHONEBOOK_AAS_ENTRY_T;

typedef struct PHONEBOOK_READ_USER_INFO_ENTRY_T
{
    PHONEBOOK_USER_INFO_T 	read_file_info[PHONEBOOK_MAX_OPERATE_NUM];  /*CR179773 Eddie.Wang*/
    uint8                                      adn_rec_id;
    uint8 						file_num;
    uint8						anr_file_num;
    uint8						sne_file_num;  
    uint8						email_file_num;   
}PHONEBOOK_READ_USER_INFO_ENTRY_T;

typedef struct MN_USIM_EMAIL_STRING_T
{
    uint8   address[MN_USIM_EMAIL_STRING_MAX_LEN];	
    uint8   address_len;
} MN_USIM_EMAIL_STRING_T;

typedef struct PHONEBOOK_ANR_NUMBER_T
{
    MNBCD               number[PHONEBOOK_MAX_DN_NUMBER_LEN];
    uint8               aas_id; //EFaas中的记录号
    uint8               number_len;
    MN_NUMBER_TYPE_E    ton;
    MN_NUMBER_PLAN_E    npi;
    BOOLEAN   			adn_id_exist; /*if byte 16 and 17 exist*/
    uint8     adn_sfi; /*byte 16, sfi of ADN*/
    uint8     id_in_adn; /*byte 17, corespond of rec. id in adn*/
} PHONEBOOK_ANR_NUMBER_T;

typedef struct PHONEBOOK_ADDITIONAL_ENTRY_T
{
    PHONEBOOK_ANR_NUMBER_T          anr_number[MN_USIM_AAS_MAX_NUM];
    MN_USIM_EMAIL_STRING_T            email_addr;
    PHONEBOOK_ALPHA_STRING_T        sne_alpha;  
    uint16                                          entry_id;	
} PHONEBOOK_ADDITIONAL_ENTRY_T;

typedef struct PHONEBOOK_UPDATE_USER_INFO_ENTRY_T
{ 
    PHONEBOOK_USER_INFO_T			anr_info[MN_MAX_USIM_ANR_NUM];  
    PHONEBOOK_USER_INFO_T			adn_info;
    PHONEBOOK_USER_INFO_T 			email_info;  
    PHONEBOOK_USER_INFO_T 			sne_info;  
    PHONEBOOK_ADDITIONAL_ENTRY_T  	user_info;  
    BOOLEAN							anr_flag[MN_MAX_USIM_ANR_NUM];
    BOOLEAN							email_flag;
    BOOLEAN							sne_flag;  
    BOOLEAN							adn_flag;
    uint8                                      adn_rec_id;
}PHONEBOOK_UPDATE_USER_INFO_ENTRY_T;

typedef struct PHONEBOOK_DEL_USER_INFO_ENTRY_T
{  
    PHONEBOOK_USER_INFO_T	anr_info[MN_MAX_USIM_ANR_NUM];
    PHONEBOOK_USER_INFO_T 	adn_info;
    PHONEBOOK_USER_INFO_T 	sne_info;  
    PHONEBOOK_USER_INFO_T 	email_info;
    PHONEBOOK_USER_INFO_T	iap_info;
    BOOLEAN      			 	iap_flag;

}PHONEBOOK_DEL_USER_INFO_ENTRY_T;
/* -vcard dev binli */

typedef enum 
{
    MNPHONE_TD_MEAS_AUTOMATIC = 0,      //与协议规定一致
    MNPHONE_TD_MEAS_ENABLE       = 1,	//亮屏时Enable
    MNPHONE_TD_MEAS_DISABLE      = 2	//暗屏时Disable
} MNPHONE_TD_MEAS_STATE;

/*****************************************************************************
 **                        TD_SCDMA DEFINE                                    *
 ******************************************************************************/
typedef struct _MN_RAI_T {
    uint16  mcc;
    uint16  mnc;
    uint16  mnc_digit_num;
    uint16  lac;
    uint16  rac;
} MN_RAI_T;



typedef struct
{
    SIGNAL_VARS
    uint16          uarfcn;
    uint8           cpi;
    uint8           meas;
    MN_RAI_T        rai;
    uint8           cell_id;
    MN_DUAL_SYS_E   dual_sys;
} APP_MN_TD_NCELL_INFO_IND_T;


/* grr em info begin */
typedef struct _GSM_SCELL_ENG_PARAM
{
    uint8 scell_band;  //0 EGSM900,1 DCS1800, 3 PCS1900, 4 GSM850
    uint8 scell_bsic;
    uint8 scell_sync;   //boolean,indicate whether bsic is verified，now always set true
    uint8 scell_qual;   /*only dedicate mode use, other mode invalid  set 0xFF*/
    uint8 scell_rxlev;
    uint16 scell_arfcn;
    int16 scell_c1;
    int16 scell_c2;
}GSM_SCELL_ENG_PARAM;

typedef struct _GSM_NCELL_ENG_PARAM
{
    uint8 ncell_band_ind;  /* boolean,0 means DCS1800,1 means PCS1900 */
    uint8 ncell_bsic;
    uint8 ncell_sync;      /*boolean,indicate whether bsic is verified, now always set true*/
    uint8 ncell_qual;      /*not used set oxFF*/     
    uint8 ncell_rxlev;
    uint16 ncell_arfcn;
    int16 ncell_c1;
    int16 ncell_c2;
}GSM_NCELL_ENG_PARAM;

typedef struct _TD_CELL_ENG_PARAM
{
    uint8  cell_param_id;
    uint8  rscp;
    uint16 uarfcn; 
}TD_CELL_ENG_PARAM;

typedef struct _EM_GRR_INFO_STRUCT
{
    uint8 rrm_state;    /* 1 SEARCHING,2 IDLE, 3 DEDICATE, 4 TBF, 0xFF means invalid */
    uint8 edge_avail;  /*boolean value,0 means edge is not supported in the scell, 1 means edge is supported.*/
    uint8 chan_mode;   /*1  F_H_RATE_VER_1, 2  F_H_RATE_VER_2,  3  F_H_RATE_VER_3,   0FF means invalid*/
    uint8 chan_type;   /*0 TCH_H, 1 TCH_F, 2 SDCCH_4, 3 SDCCH_8,  0xFF invalid */
    uint8 speech_mode; /*3 AMR(version 3),2 EFR(version 2),1 FR(version 1),0 HR(version 0), 0xFF means invalid */
    uint16 td_cell_num; /* TD Neighbour Cell NUM*/
    uint16 gsm_ncell_num; /*GSM Neighbour Cell NUM*/
    GSM_SCELL_ENG_PARAM scell_param;
    GSM_NCELL_ENG_PARAM ncell_param[6];
    TD_CELL_ENG_PARAM td_cell_param[6];
}EM_GRR_INFO_STRUCT;
/* grr em info end */

typedef enum MN_PHONEBOOK_OPERATE_TYPE_E
{
    OPERATE_SUCCESS,
    OPERATE_FILE_NOT_EXIST,
    OPERATE_FILE_SIM_FAIL,
    OPERATE_FILE_RECORDS_FULL
}MN_PHONEBOOK_OPERATE_TYPE_E;

typedef struct APP_MN_UPDATE_USER_INFO_RESULT_T
{
    MN_PHONEBOOK_OPERATE_TYPE_E anr_result[MN_USIM_AAS_MAX_NUM];
    MN_PHONEBOOK_OPERATE_TYPE_E adn_result;
    MN_PHONEBOOK_OPERATE_TYPE_E email_result;
    MN_PHONEBOOK_OPERATE_TYPE_E sne_result;  
    BOOLEAN                 anr_flag[MN_USIM_AAS_MAX_NUM];
    BOOLEAN                 adn_flag;
    BOOLEAN                 email_flag;
    BOOLEAN                 sne_flag;  
    uint16                     adn_entry_id;
} APP_MN_UPDATE_USER_INFO_RESULT_T;


typedef struct APP_MN_UPDATE_USER_INFO_CNF_T
{
    SIGNAL_VARS
    APP_MN_UPDATE_USER_INFO_RESULT_T update_result;
    uint16                     adn_entry_id;	
    MN_DUAL_SYS_E               dual_sys;
} APP_MN_UPDATE_USER_INFO_CNF_T;

typedef struct APP_MN_DEL_USER_INFO_CNF_T
{
    SIGNAL_VARS
    APP_MN_UPDATE_USER_INFO_RESULT_T update_result;
    uint16                     adn_entry_id;	
    MN_DUAL_SYS_E               dual_sys;
} APP_MN_DEL_USER_INFO_CNF_T;

typedef struct APP_MN_READ_ADDITIONAL_ENTRY_T
{
    PHONEBOOK_ANR_NUMBER_T   	    anr_number[MN_MAX_USIM_ANR_NUM];
    BOOLEAN                                     anr_ret[MN_MAX_USIM_ANR_NUM];
    BOOLEAN                                     email_ret;
    BOOLEAN                                     sne_ret;  
    MN_USIM_EMAIL_STRING_T              email_addr;
    PHONEBOOK_ALPHA_STRING_T	    sne_alpha; 
    uint16                                          adn_entry_id;	
} APP_MN_READ_ADDITIONAL_ENTRY_T;


typedef struct APP_MN_READ_ADDITIONAL_ENTRY_CNF_T
{
    SIGNAL_VARS
    APP_MN_READ_ADDITIONAL_ENTRY_T add_info;
    uint16                     adn_entry_id;	
    MN_DUAL_SYS_E               dual_sys;
} APP_MN_READ_ADDITIONAL_ENTRY_CNF_T;


typedef uint32 EM_BAND_TYPE_E;

typedef struct 
{
  EM_BAND_TYPE_E           band_type;
  uint16         lower_freq;
  uint16         upper_freq;
} EM_BAND_RANGE_INFO_T;

typedef struct
{
  int8          				    no_range;  
  uint8                             pend[3];
  EM_BAND_RANGE_INFO_T             range_info[EM_MAX_NUM_FREQ_RANGE];
} EM_BAND_NVM_INFO_T;

/* nas em info begin */
typedef struct _EM_PLMN_ID_STRUCT
{
    uint            mcc1:4; /*  */
    uint            mcc2:4; /*  */
    uint            mcc3:4; /*  */
    uint            mnc1:4; /*  */
    uint            mnc2:4; /*  */
    uint            mnc3:4; /*  */
} EM_PLMN_ID_STRUCT;

/* GSM update status types */
/*
    MM_U1_UPDATED = 0,
    MM_U2_NOT_UPDATED,
    MM_U3_ROAMING_NOT_ALLOWED
*/
typedef struct _EM_NAS_GMM_INFO_STRUCT
{
    uint8         gmm_state;
    uint8         gmm_attach_state;
    uint8         gmm_update_status;
    uint8         failed_attach_counter;
  
    uint8         failed_rau_counter;
    uint8         gmm_rej_cause;
    uint8         nmo;                  /* 0: NMO-I, 1: NMO-II, 2: NMO-III */
    uint8         sgsn_version;         /* 0: R97 verson for sgsn; 1: R99 version for sgsn */
  
    uint8         auth_status;          /* 0: Success, 1: Failure */

    uint8         ptmsi[4];
    uint32        t3312_value; 
    uint32        remaining_t3312_value; 
} EM_NAS_GMM_INFO_STRUCT;

typedef struct _EM_NAS_ABNORMAL_INFO_STRUCT
{
    uint8           index;                /* 0 - 7 */
    uint8           rej_domain;           /* 0:CS, 1:PS or 2:Cm_serv_rej */
    uint8           cause_sel;            /* 0:Nw rej or 1:Local rej */
    uint8           rej_cause;   
    EM_PLMN_ID_STRUCT   rej_plmn_id;      /* include mcc and mnc */  
} EM_NAS_ABNORMAL_INFO_STRUCT;

typedef struct _EM_NAS_SYS_INFO_STRUCT
{
    EM_PLMN_ID_STRUCT      plmn_value;       
    uint8                  imsi_att;         /* whether IMSI attach is allowed. 1: allowed */  
  
    uint8                  lac[2];
    uint8                  rac;
} EM_NAS_SYS_INFO_STRUCT;

typedef struct _EM_NAS_MM_INFO_STRUCT
{
    uint8         mm_state;
    uint8         lu_status;            /* gsm_attach_state: */
    uint8         gsm_update_status;    /* gsm_update_status */
    uint8         failed_lu_counter;
    uint8         msc_version;          /* 0: R97 verson for msc; 1: R99 version for msc */
    uint8         mm_pending_proc;
    uint8         cmm_rej_cause;
  
    uint8         tmsi[4];
    uint32        t3212_value; 
    uint32        remaining_t3212_value; 
    uint32        remaining_hplmn_timer;
} EM_NAS_MM_INFO_STRUCT;

typedef struct _EM_NAS_INFO_STRUCT
{
	EM_NAS_SYS_INFO_STRUCT        sys_info;
	EM_NAS_MM_INFO_STRUCT         mm_info;
	EM_NAS_GMM_INFO_STRUCT        gmm_info;
	EM_NAS_ABNORMAL_INFO_STRUCT   abnormal_info[8];
} EM_NAS_INFO_STRUCT;
/* nas em info end */

/* bcfe em info begin */
typedef struct _EM_RRC_BCFE_SERV_CELL_INFO_STRUCT
{
   uint8   serv_param_id;/*0xff is invalid value*/
   uint8   serv_rscp;/*0 ~ 116, 117 is invalid value. it's equal to the dbm value add 116. */
   uint16  serv_uarfcn;/*0xffff is invalid value*/
   int16   s_value;/*may be positive or negative values may.-32768 is invalid value*/
   int16   r_value;/*may be positive or negative values may.-32768 is invalid value*/
} EM_RRC_BCFE_SERV_CELL_INFO_STRUCT;


typedef struct _EM_RRC_BCFE_NEIGHBOR_CELL_INFO_STRUCT
{
   uint8   param_id;
   uint8   rscp;/*0 ~ 116, 117 is invalid value. it's equal to the dbm value add 116. */
   uint16  uarfcn;
   int16   s_value;/*may be positive or negative values may*/
   int16   r_value;/*may be positive or negative values may*/
} EM_RRC_BCFE_NEIGHBOR_CELL_INFO_STRUCT;

typedef struct _EM_RRC_BCFE_INTER_RAT_NEIGHBOR_CELL_INFO_STRUCT
{
   uint8   bsic;
   uint8   band;/*0 means band 1800 1 means band 1900*/
   uint8   rssi;/*the dbm value add 111. */
   uint8   bsic_status;/*1 indicate bsic verify success. 0 indicate verify fail */
   uint16  arfcn;
   int16   s_value;/*may be positive or negative values may*/
   int16   r_value;/*may be positive or negative values may*/
} EM_RRC_BCFE_INTER_RAT_NEIGHBOR_CELL_INFO_STRUCT;

typedef struct _EM_RRC_BCFE_INFO_STRUCT
{
   EM_RRC_BCFE_SERV_CELL_INFO_STRUCT serv_cell_info;
   uint8 no_td_ncell;
   EM_RRC_BCFE_NEIGHBOR_CELL_INFO_STRUCT  td_ncell_info[64];
   uint8 no_gsm_ncell;
   EM_RRC_BCFE_INTER_RAT_NEIGHBOR_CELL_INFO_STRUCT gsm_ncell_info[32];
}EM_RRC_BCFE_INFO_STRUCT;
/* bcfe em info end */

/* dcfe em info begin */
typedef struct _EM_RRC_DCFE_INFO_STRUCT
{
    uint8 rrc_dcfe_state;
    uint8 nas_sysc_ind;
} EM_RRC_DCFE_INFO_STRUCT;
/* dcfe em info end */


typedef struct _EM_PROTOCOL_STACK_INFO_T
{
    MN_GMMREG_RAT_E  current_rat;
    EM_NAS_INFO_STRUCT em_nas_info;
    EM_RRC_BCFE_INFO_STRUCT em_rrc_bcfe_info;
    EM_RRC_DCFE_INFO_STRUCT em_rrc_dcfe_info;
    EM_GRR_INFO_STRUCT em_grr_info;
} EM_PROTOCOL_STACK_INFO_T;

#if defined PRODUCT_DM
#if (!defined MULTI_SIM_SYS_SINGLE) && (!defined MULTI_SIM_SYS_DUAL) && (!defined MULTI_SIM_SYS_TRI) && (!defined MULTI_SIM_SYS_QUAD)
typedef struct _GSM_STACK_CFG_PARAMS
{
    uint8  a_plus_b_flag;   /* A+B enable flag*/
    uint8 offset;
    uint8 penny;
    uint8 absolute_td_threshold;
    uint8  td_meas_capability; /*0:in a+b mode , meas rssi only ;      1: a+b mode , meas rscp*/
    uint8  q_offset_table_switch;/*0:relative offset, original spec;  1:absolute offset,cmcc spec*/
    uint8 power_saving_in_dark_mode;/*0:not applied power saving,1:power saving*/
    uint8 inter_rat_init_pinp_offset;  /*initial inter rat pinp offsert value,range:0-15*/
    uint8 inter_rat_pinp_offset_step_rate;  /*step rate = increase_step_value/reduce_step_value,range:0-10*/
    uint8 inter_rat_pinp_gsm_tmr_val; /*the duration in GSM active, uint:min*/
    uint8 inter_rat_pinp_td_tmr_val; /*the duration in TD active, uint:min*/
    uint8 tddo_tdd_offset;       /*tdd_offset used by TDDO, default value is -90dBm.*/
    BOOLEAN simple_fr_switch;    /*FALSE:simple fr is disable, TRUE: simple fr is enable*/
    BOOLEAN like_fr_switch;      /*FALSE:like fr is disable, TRUE: like fr is enable*/
    BOOLEAN innovation_fr_switch;   /*FALSE:innovation fr is disable, TRUE: innovation fr is enable*/
    BOOLEAN pcco_switch;   /*FALSE:pcco is disable, TRUE: pcco is enable*/
}GSM_STACK_CFG_PARAMS;

typedef struct _MN_ENG_TD_NCELL_OF_GSM_T{
    uint8  cell_param_id;
    uint8  rscp;
    uint16 arfcn;
}MN_ENG_TD_NCELL_OF_GSM_T;

typedef struct _MN_ENG_TD_NCELL_OF_GSM_LIST
{
       uint8  cell_num;
       MN_ENG_TD_NCELL_OF_GSM_T tdd_ncell_array[MN_ENG_MAX_TDD_NCELL_NUM_OF_GSM];
}MN_ENG_TD_NCELL_OF_GSM_LIST;

typedef struct _MN_ENG_GRR_PARAMS
{
uint8  Qsearch_i;
uint8  tdd_qoffset;
MN_ENG_TD_NCELL_OF_GSM_LIST tdd_ncell_list;
uint32 G2G_resel_total_amount;
uint32 G2G_resel_succ_amount;
uint32 G2G_resel_fail_amount;
uint32 G2G_CS_HO_total_amount;
uint32 G2G_CS_HO_succ_amount;
uint32 G2G_CS_HO_fail_amount;
uint32 G2G_PS_HO_total_amount;
uint32 G2G_PS_HO_succ_amount;
uint32 G2G_PS_HO_fail_amount;
uint32 G2T_resel_total_amount;
uint32 G2T_resel_succ_amount;
uint32 G2T_resel_fail_amount;
uint32 G2T_CS_HO_total_amount;
uint32 G2T_CS_HO_succ_amount;
uint32 G2T_CS_HO_fail_amount;
uint32 G2T_PCCO_total_amount;
uint32 G2T_PCCO_succ_amount;
uint32 G2T_PCCO_fail_amount;
uint32 no_cell_amount;
uint32 power_sweep_amount;
uint32 power_sweep_succ_amount;
uint32 power_sweep_fail_amount;
uint32 power_sweep_used_ba;
uint32 power_sweep_unused_ba;
uint32 T2G_resel_fail_no_revert_amount;
uint32 T2G_resel_no_revert_total_amount;     /*T2G紧急重选次数*/
uint32 power_sweep_used_ba_camp_fail_amount;  /*带BA power_sweep 失败次数*/
uint32 power_sweep_used_ba_camp_suc_amount;   /*带BA power_sweep 成功次数*/
}MN_ENG_GRR_PARAMS;

#endif  //#if defined MULTI_SIM_SYS_SINGLE 
#endif  //#if defined PRODUCT_DM

#endif
/* +cr250176 */
typedef struct MN_CB_MSG_SET_PARA_T
{
 uint8 enable;     /* 0x01:enable ,0x02:disable */
 uint8 selected_type;   /* 0x01:all CBMi ,0x02:some CBMi */
 uint8 list_count;   /* the max number of CBMi to SIM */
 MN_CB_MSG_ID_T *msg_id_list ; /* point to message id array */
}MN_CB_MSG_SET_PARA_T;

typedef struct APP_MN_CB_MSG_GET_PARA_T
{
 SIGNAL_VARS 
 uint8 enable;     /* 0x01:enable ,0x02:disable */
 uint8 selected_type;   /* 0x01:all CBMi ,0x02:some CBMi */
 uint8 max_list_count;   /* the max number of CBMi in SIM */
 uint8 list_count;   /* the used number of CBMi in SIM */
 MN_CB_MSG_ID_T *msg_id_list ; /* point to CBMi array */
}APP_MN_CB_MSG_GET_PARA_T;

typedef struct 
{
    SIGNAL_VARS 
    MN_SMS_CAUSE_E     cause;  // the operate cause
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_CB_MSG_SET_CNF_T;
/* -cr250176 */
/* +cr250176_NEW */
typedef struct
{
    SIGNAL_VARS
    MN_CB_MSG_ID_T  msg_id;
    MN_SMSCB_DCS_T  dcs;
    uint16          msg_len;
    MN_CB_MSG_ARR_T msg_arr;
    MN_DUAL_SYS_E   dual_sys;
    MN_CB_DISPLAY_MODE_E display_mode;
    uint8          geographic_scope;
    MN_MSG_CODE_T         msg_code;
    uint8          update_num;
    uint8          total_page;
    uint8          current_page;
} APP_MN_TDPS_SMSCB_MSG_IND_T;
/* -cr250176_NEW */
