/******************************************************************************
 ** File Name:      sim_signal.h                                              *
 ** Author:         Raislin.Kong                                              *
 ** Date:           2002.2                                                    *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the header file that define the signals used in   *
 **					sim	task												  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2002.2         Raislin.Kong      create                                   *
 ******************************************************************************/

#ifndef _SIMSIGNAL_H_
#define _SIMSIGNAL_H_

#include "gsm_gprs.h"  //need by PLMN_T 
#include "sim_file_system.h" //need by read sim file interface

#ifdef WIN32
#include "sim_file_structure.h"
#endif

#ifdef   __cplusplus
    extern   "C" 
    {
#endif


//the max chv length
#define SIM_CHV_MAX_LEN				8
#define SIM_SEEK_PATTERN_MAX_LEN	16
#define SIM_INCREASE_DATA_LEN		3
#define SIM_NETWORK_RAND_LEN		16
#define	SIM_SERS_LEN				4

//for 3G authentication
#define USIM_RAND_VALUE_LEN    16
#define USIM_AUTN_VALUE_LEN    16

#define USIM_CK_LEN                   16
#define USIM_IK_LEN                    16
#define USIM_MAX_RES_LEN        16
#define USIM_AUTS_LEN               14

#define SIM_AUTN_3G_SUCCESS_TAG   0xDB  
#define SIM_AUTN_SYNC_FAIL_TAG      0xDC  

typedef struct
{
    uint8   data[3];
}SIM_INSTR_INCREASE_DATA_T;

//the data that increased
typedef struct
{
	uint32 data;
}SIM_INCREASE_DATA_T;

//the structure of increase result
typedef struct
{
	SIM_INCREASE_DATA_T  new_value;
	SIM_INCREASE_DATA_T  added_value;
}SIM_INCREASE_RESULT_T;

typedef struct
{
	uint8 secret_code[SIM_CHV_MAX_LEN];
}SIM_CHV_T;

typedef enum
{
	SIM_CHV1 = 0x01,
	SIM_CHV2 = 0x02
}SIM_CHV_INDICATION_E;


typedef struct
{
	SRES_T	sers;
	KC_T	kc;
}SIM_GSM_ALGORITHM_RESULT_T;

#ifdef SIM_SIMULATOR
#define SIMTIW_TO_WIN_DUMMY 5000
#endif

typedef struct SIM_SERVICE_STATUS_T
{
	uint32	task_id;		//the task id that request the service
	uint32	service_type;	//the service type
	uint32	service_info;	//the service info.just to record the simple service info.
							//for example.the sub service type or the file id of read file
	uint32	command_ref;	//the command ref
	BOOLEAN	in_service;		//is the SIM layer in service?the sim layer can only 
							//do one service at the same time
}SIM_SERVICE_STATUS_T;

//the power on requet,jut to request the sim task to powen on sim card
typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
}SIM_SIG_START_UP_REQ_T;

//the singal structure of SIM_READ_REQ 
typedef struct
{
    SIGNAL_VARS
    uint32	command_ref; 
    SIM_FILE_NAME_E	file_name;	//the EF file ID
}SIM_SIG_READ_T_FILE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32	command_ref; 
    SIM_FILE_NAME_E	file_name;	//the EF file ID
    uint8	record_num;			//the record num of the record file
    SIM_RECORD_ACCESS_MODE_E record_mode;	//the record access mode of record file
}SIM_SIG_READ_R_FILE_REQ_T;

//Send the Dphone SIM card timeout message to the SIM manager
typedef struct
{
    SIGNAL_VARS
}SIM_SIG_DPHONE_REFRESH_TIMEOUT_IND_T;

//to send the SIM card reponse of the command to the SIM manager
typedef struct
{
	SIGNAL_VARS
	uint8	sw1;
	uint8	sw2;
	uint16	data_len;
	uint8*	data;
}SIM_SIG_RESPONSE_IND_T;

typedef struct
{
	SIGNAL_VARS 
	uint8 card_index;      //which sim card
	BOOLEAN is_reset_ok;//is reset successfully
	BOOLEAN is_atr_volt_info;//is the atr data has the voltage info
	BOOLEAN is_atr_1_8v_only;//is the atr data show this is a 1.8V only sim card
	uint16 t_protocol;//t protocol of this this card
}SIM_SIG_1_8V_DETECT_IND_T;

//the result of reading file
typedef enum
{
    SIM_READ_OK,                   //read file succeeded
    SIM_READ_INVALIDE_FILE,        //the file is invalide in the sim card
    SIM_READ_INCONSISTENT_FILE,    //the file type or structure is not consisten to the request
    SIM_READ_NOT_EXSITE,           //the file not existe in the SIM card
    SIM_READ_CHV1_NEEDED,               //the read operation need the right CHV1
    SIM_READ_CHV2_NEEDED,               //the read operation need the right CHV2
    SIM_READ_U_CHV1_NEEDED,             //the read operation need the right UNBLOCK CHV1
    SIM_READ_U_CHV2_NEEDED,             //the read operation need the right UNBLOCK CHV2
    SIM_READ_CHV_BLOCKED,               //both the read operation relative chv and unblock chv blocked
    SIM_READ_ACCESS_NEVER,              //the read operation access will never be fufilled
    SIM_READ_FAILED_UNKNOW         //the read file is failed for unknow reason
}SIM_RESULT_READ_FILE_E;

//the signal structure of SIM_READ_REQ
typedef struct
{
    SIGNAL_VARS
    uint32	command_ref;
    SIM_FILE_NAME_E file_name;
    SIM_RESULT_READ_FILE_E result;	//the Read file result
    SIM_FILE_STRUCTURE_BUF_U	 buf;		//the Read file result buf
}SIM_SIG_READ_T_FILE_CNF_T;

//the signal structure of SIM_READ_REQ
typedef struct
{
    SIGNAL_VARS
    uint32	command_ref;
    SIM_FILE_NAME_E   file_name;
    uint8           record_id;
    SIM_RECORD_ACCESS_MODE_E record_mode;
    SIM_RESULT_READ_FILE_E result;	//the Read file result
    SIM_FILE_STRUCTURE_BUF_U	 buf;		//the Read file result buf
}SIM_SIG_READ_R_FILE_CNF_T;

//the signal structure of SIM_UPDATE_REQ
typedef struct
{
    SIGNAL_VARS
    uint32	command_ref; 
    SIM_FILE_NAME_E	file_name;	//the EF file ID
    SIM_FILE_STRUCTURE_BUF_U	buf;		//the update data
}SIM_SIG_UPDATE_T_FILE_REQ_T;

//the signal structure of SIM_UPDATE_REQ
typedef struct
{
    SIGNAL_VARS
    uint32	command_ref; 
    SIM_FILE_NAME_E	file_name;	//the EF file ID
    uint8	record_num;			//the record num of record ef when record access mode is absolute
    					               //invalide to transparent ef
    SIM_RECORD_ACCESS_MODE_E	record_mode;	//the record access mode of record ef
    								//invalide to transparent ef
    SIM_FILE_STRUCTURE_BUF_U	buf;		//the update data
}SIM_SIG_UPDATE_R_FILE_REQ_T;

//the result of a update file
typedef enum
{
    SIM_UPDATE_OK,
    SIM_UPDATE_INVALIDE_FILE,
    SIM_UPDATE_INCONSISTENT_FILE,
    SIM_UPDATE_NOT_EXISTE,
    SIM_UPDATE_CHV1_NEEDED,
    SIM_UPDATE_CHV2_NEEDED,
    SIM_UPDATE_U_CHV1_NEEDED,
    SIM_UPDATE_U_CHV2_NEEDED,
    SIM_UPDATE_CHV_BLOCKED,
    SIM_UPDATE_ACCESS_NEVER,
    SIM_UPDATE_FAILED_UNKNOW
}SIM_RESULT_UPDATE_FILE_E;

//the signal structure of SIM_UPDATE_CNF
typedef struct
{
    SIGNAL_VARS
    uint32	command_ref; 
    SIM_FILE_NAME_E	file_name;	//the EF file ID
    SIM_FILE_STRUCTURE_BUF_U	buf;
    SIM_RESULT_UPDATE_FILE_E	result;
}SIM_SIG_UPDATE_T_FILE_CNF_T;

//the signal structure of SIM_UPDATE_CNF
typedef struct
{
    SIGNAL_VARS
    uint32	command_ref;
    SIM_FILE_NAME_E	file_name;	//the EF file ID
    uint8	record_id;			//the record num of record ef when record access mode is absolute
    				                    //invalide to transparent ef
    SIM_RECORD_ACCESS_MODE_E	record_mode;	//the record access mode of record ef
    							//invalide to transparent ef
    SIM_RESULT_UPDATE_FILE_E	result;
}SIM_SIG_UPDATE_R_FILE_CNF_T;

//the signal structure of SIM_INCREASE_FILE_REQ
typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
	SIM_FILE_NAME_E file_name;	//the increased file,must be a cyclic ef
	SIM_INCREASE_DATA_T	data;	//the increase data,must be 3 byte
}SIM_SIG_INCREASE_FILE_REQ_T;

//the result of increase file
typedef enum
{
	SIM_INCREASE_FILE_OK,			//the increase operation succeed
    SIM_INCREASE_INVALIDE_FILE,		//the file name is a invalide file
    SIM_INCREASE_INCONSISTENT_FILE,	//the EF file structure is not a cylic
    SIM_INCREASE_FILE_NOT_EXISTE,	//the EF file is not exist in the SIM card	
    SIM_INCREASE_CHV1_NEEDED,       //need the right CHV1 to the increase operation
    SIM_INCREASE_CHV2_NEEDED,       //need the right CHV2 to the increase operation
    SIM_INCREASE_U_CHV1_NEEDED,     //need the right UNBLOCK CHV1 to the increase operation
    SIM_INCREASE_U_CHV2_NEEDED,     //need the right UNBLOCK CHV2 to the increase operation
    SIM_INCREASE_CHV_BLOCKED,       //all both the coresponding chv and ublock chv has be blocked
    SIM_INCREASE_ACCESS_NEVER,      //the increase will never success for no access 
    SIM_INCREASE_REACH_MAX_VLAUE,	//the value of the record have reached to max value,
    								//can not increase
    SIM_INCREASE_UNKNOW_ERROR		//increase failed for unknow error reason
}SIM_RESULT_INCREASE_FILE_E;

//the signal structure of SIM_INCREASE_FILE_CNF
typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
    SIM_FILE_NAME_E file_name;
	SIM_RESULT_INCREASE_FILE_E result;
	SIM_INCREASE_RESULT_T	increase_result;
}SIM_SIG_INCREASE_FILE_CNF_T;


//the soft power off indication from the upper layer
typedef struct
{
	SIGNAL_VARS
}SIM_SIG_POWER_OFF_REQ_T;

typedef struct
{
	SIGNAL_VARS
}SIM_SIG_PRESENT_CHECK_IND_T;

//the initialize requset signal
typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
	SIM_CHV_T chv1;			//the chv1 used to verify the chv1
	SIM_CHV_T	unblock_chv1;	//the chv1 unblock chv
	BOOLEAN use_unblock;
	//BOOLEAN me_fdn_capability;	//whether the ME FDN capability
	//BOOLEAN me_cc_capability;	//whether the ME call control capability	
}SIM_SIG_INITIALIZE_REQ_T;

typedef enum
{
    SIM_INI_OK,						//the initialization is successful
    SIM_INI_ERROR_CHV1_AND_BLOCKED,	//chv1 verify fail and chv1 been blocked
    SIM_INI_ERROR_CHV1,				//chv1 verify fail but not been blocked
    SIM_INI_ERROR_UNBLOCK_CHV1,		//unblock chv1 fail but unblock chv1 not been blocked
    SIM_INI_ERROR_UNBLOCK_CHV1_BLOCKED,	//unblock chv1 fail and unblock chv1 has been blocked
    SIM_INI_ERROR_READ_SST,			//the SIM service table request fail
    SIM_INI_ERROR_READ_IMSI,		//the IMSI request fail
    SIM_INI_ERROR_READ_ACC,			//the Access Control request fail
    SIM_INI_ERROR_READ_HPLMN,		//the HPLMN search period request fail
    SIM_INI_ERROR_READ_PLMN,		//the PLMN Selector request fail
    SIM_INI_ERROR_READ_PLMN_WACT,
    SIM_INI_ERROR_READ_OPLMN_WACT,
    SIM_INI_ERROR_READ_HPLMN_WACT,
    SIM_INI_ERROR_READ_LOCI,		//the Location Information request fail
    SIM_INI_ERROR_READ_KC,			//the Cipher Key request fail
    SIM_INI_ERROR_READ_BCCH,		//the BCCH Information request fail
    SIM_INI_ERROR_READ_FPLMN,		//the Forbidden PLMN request fail
    SIM_INI_ERROR_READ_LSA,			//the LSA Information request fail
    SIM_INI_ERROR_READ_CBMID,		//the CBMIR request fail
    SIM_INI_ERROR_READ_DCK,			//the Depersonalisation Control Keys request fail
    SIM_INI_ERROR_READ_NIA,			//the Network's Indication of alerting request fail
    SIM_INI_ERROR_READ_AOC_CONTEXT,	//Read the AOC Contxt failed
    SIM_INI_ERROR_READ_SPN,         //the Service Provider Name fail
    SIM_INI_ERROR_READ_EHPLMN,      
    SIM_INI_ERROR_READ_EHPLMNPI,      
    SIM_INI_ERROR_READ_GID1,			//Group Identifier Level 1 request fail
    SIM_INI_ERROR_READ_GID2,			//Group Identifier Level 2 request fail
    SIM_INI_ERROR_IMSI_FULL_FF,     //the IMSI is full 0xff,but other file is ok,for dphone encrypt
    SIM_INI_ERROR_TERMINAL_PROFILE,
    SIM_INI_ERROR_DPHONE,
    SIM_INI_ERROR_CARD_OUT,
    SIM_INI_ERROR_CHV_NOT_INPUT,
    SIM_INI_ERROR_UNKNOWN
}SIM_RESULT_INITIALIZE_SIM_E;

//the request signal to read imsi file when the imsi is full 0xff,
//this signal is used for dphone encrypt SIM card.
typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
}SIM_SIG_INIT_ENCRYPT_REQ_T;

typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
	SIM_EF_MAPPING_IMSI_T	imsi_mapping;	//the IMSI
	SIM_SERVICE_RESULT_E    result;
}SIM_SIG_INIT_ENCRYPT_CNF_T;

typedef struct
{
BOOLEAN chv_acm;
BOOLEAN chv_acmmax;
BOOLEAN chv_puct;
}SIM_AOC_CHV_SELECT;    /*@king.li new adds for cr90738@*/

typedef struct
{
	BOOLEAN     AOC_is_support;
	uint32      unit_ACM;
	uint32      max_ACM;
       SIM_EF_MAPPING_PUCT_T     puct;  /*king.li new adds for initial setting of AOC for cr90738@*/
       uint32      unit_ACM_line2;
} SIM_AOC_CONTEXT_T;

typedef struct
{
    BOOLEAN is_chv1_enabled;                //the chv1 enabled or not
    BOOLEAN gsm_operation_start;			//whether the GSM operation shall start
    BOOLEAN cbmid_supported;                //the CBMID service supported or not
    BOOLEAN solsa_supported;                //the SoLSA service supported or not
    BOOLEAN nia_supported;                  //the NIA service supported or not
    BOOLEAN dck_supported;                  //the DCK service supported or not
    BOOLEAN spn_supported;
    BOOLEAN gid1_supported;
    BOOLEAN gid2_supported;
	SIM_EF_MAPPING_SST_T	sst_mapping;	//the SIM Service Table 
	SIM_EF_MAPPING_IMSI_T	imsi_mapping;	//the IMSI
	ACC_CONTROL_CLASS_T		acc_mapping;	//the Access Control
	HPLMN_TIMER_VALUE_T		hplmn_mapping;	//the HPLMN Search Period
#ifndef PRODUCT_DM
	SIM_EF_MAPPING_PLMNSEL_T		plmn_mapping;	//the PLMN selector
#else
	PLMN_LIST_T				plmn_mapping;	//the PLMN selector
#endif
	SIM_EF_MAPPING_LOCI_T	loci_mapping;	//the Location Information
	SIM_EF_MAPPING_KC_T		kc_mapping;		//the Cipher Key
	SIM_EF_MAPPING_BCCH_T			bcch_mapping;	//the BCCH information
	FPLMN_LIST_T			fplmn_mapping;	//the Forbidden PLMN Information
	SIM_DF_MAPPING_SOLSA_T	solsa_mapping;	//LSA Information
	SIM_EF_MAPPING_CBMID_T	cbmid_mapping;	//CBMIR
	SIM_EF_MAPPING_DCK_T	dck_mapping;	//Depersonalisation Control Keys
	SIM_EF_MAPPING_NIA_T	nia_mapping;	//Network's indication of alerting
	SIM_EF_MAPPING_SPN_T	spn_mapping;	//Service Provider Name
	SIM_AOC_CONTEXT_T		aoc_context;	//the AOC context
    SIM_CHV_STATUS_T        chv_status;         //the status of the chv
    SIM_CHV_STATUS_T        unblock_status;     //the status of the unblock chv
    SIM_AOC_CHV_SELECT   aoc_chv_select;   // the status of the acm, acmmax and puct chv1 or chv2 select   /*@king.li new adds for cr90738@*/

    SIM_EF_MAPPING_HPLMNWACT_T  hplmn_wact_t;   // "HPLMN selector with Access Technology request"
    SIM_EF_MAPPING_PLMNWACT_T   plmn_wact_t;    // "User controlled PLMN Selector with Access Technology"
    SIM_EF_MAPPING_OPLMNWACT_T  oplmn_wact_t;   // "Operator controlled PLMN Selector with Access Technology"
    SIM_EF_MAPPING_GID_T        gid1_mapping;    //Group Identifier Level 1
    SIM_EF_MAPPING_GID_T        gid2_mapping;    //Group Identifier Level 2

    // Rel-7
    SIM_EF_MAPPING_EHPLMN_T     ehplmn_mapping;
    SIM_EF_MAPPING_EHPLMNPI_T   ehplmn_pi;

    SIM_CDMA_TYPE_E cdma_type;
    BOOLEAN is_fdn_enabled;//add for IPC. AP should know FDN very quickly.
}SIM_INI_INFO;

typedef struct
{
    SIGNAL_VARS
    uint32	command_ref;
    SIM_RESULT_INITIALIZE_SIM_E	result;		//the initialize result
    SIM_INI_INFO	ini_info;
}SIM_SIG_INITIALIZE_CNF_T;

typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
    BOOLEAN is_chv1_enabled;                //the chv1 enabled or not
    SIM_CHV_STATUS_T        chv_status;         //the status of the chv
    SIM_CHV_STATUS_T        unblock_status;     //the status of the unblock chv
}SIM_SIG_DPHONE_PIN_CHECK_IND_T;

//SIM card insert status
typedef enum
{
    SIM_INSERTED_OK,
#ifdef SIM_SIMULATOR
    SIM_INSERTED_NO_SIM,
#endif
    SIM_INSERTED_WRONG_SIM,
    SIM_INSERTED_CARD_OUT,
    SIM_INSERTED_ERROR
}SIM_INSERT_STATUS_E;

typedef struct
{
    BOOLEAN	chv1_enable;			//chv1 enable
    BOOLEAN is_usim;                //TRUE:Usim; FALSE,Sim.
	BOOLEAN ecc_exist;				//the EF ecc exist or not
	SIM_CHV_STATUS_T	chv1_status;	//the chv1 status
	SIM_CHV_STATUS_T	chv2_status;	//the chv2 status
	SIM_CHV_STATUS_T	unblock_chv1_status;	//the unblock chv1 status
	SIM_CHV_STATUS_T	unblock_chv2_status;	//the unblock chv2 status
	SIM_EF_MAPPING_ECC_T		ecc_mapping;	//the EF ecc
	SIM_EF_MAPPING_LP_UNION_T   language_mapping;//the EFlp(EFli) or EFelp(EFpl)
	SIM_EF_MAPPING_PHASE_T	phase_mapping;	//the EF phase
	SIM_EF_MAPPING_AD_T		ad_mapping;		//the EF ad
    SIM_EF_MAPPING_ICCID_T  iccid_mapping;  //the ICCID
	uint8 sms_num;
}SIM_CARD_PRESENT_INFO;

typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
	SIM_INSERT_STATUS_E	insert_status;
	SIM_CARD_PRESENT_INFO present_info;
}SIM_SIG_CARD_PRESENT_IND_T;

//the card is beening removed
typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
	uint32  bHotPlug;
}SIM_SIG_CARD_REMOVE_IND_T,
SIM_SIG_POWER_OFF_CNF_T;

//the card has no any response
typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
}SIM_SIG_CARD_UNRESPONSE_IND_T;

//the card has sent part response
typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
}SIM_SIG_CARD_PART_RESPONSE_IND_T;


typedef enum
{
	SIM_VERIFY_CHV,
	SIM_CHANGE_CHV,
	SIM_DISABLE_CHV,
	SIM_ENABLE_CHV,
	SIM_UNBLOCK_CHV
}SIM_CHV_REQ_TYPE_E;

typedef struct 
{
	SIGNAL_VARS
	uint32	command_ref;
	SIM_CHV_REQ_TYPE_E	req_type;		//the request type
	SIM_CHV_INDICATION_E	chv_indication;	//the chv indication
	SIM_CHV_T	first_chv;					//the first chv in requst
	SIM_CHV_T second_chv;					//the second chv in request
}SIM_SIG_CHV_REQ_T;

typedef enum
{
	SIM_CHV_REQ_OK,
	SIM_CHV_WRONG_REQ_TYPE,	//the chv request type is wrong
	SIM_CHV_ERROR_WITH_BLOCKED,	//the chv operation error and the chv been blocked
	SIM_CHV_ERROR_CONTRADICT_STATUS,	//the chv status is contradict to the chv operation
	SIM_CHV_ERROR_NOT_INITIALIZED,	//the chv still not been initialized
	SIM_CHV_ERROR_CHV_FAIL,	//do not fulfill the access condition of the chv operation
	SIM_CHV_ERROR_UNKNOW	//unknow chv operation error
}SIM_RESULT_CHV_E;

typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
    BOOLEAN is_chv1_enabled;                //the chv1 enabled or not
	SIM_CHV_INDICATION_E	chv_indication;	//the chv indication
    SIM_RESULT_CHV_E	result;				//the request result
	SIM_CHV_REQ_TYPE_E req_type;			//the request type
    SIM_CHV_STATUS_T    chv_status;         //the status of the chv
    SIM_CHV_STATUS_T    unblock_status;     //the status of the unblock chv
}SIM_SIG_CHV_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
}SIM_SIG_READ_ECC_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32                  command_ref;
    BOOLEAN                 ecc_exist;
    SIM_EF_MAPPING_ECC_T    ecc_mapping;
    SIM_RESULT_READ_FILE_E  result;
}SIM_SIG_READ_ECC_CNF_T;


typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_LP_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_LP_T lp_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_LP_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_LP_T lp_mapping;
}SIM_SIG_UPDATE_LP_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_RESULT_UPDATE_FILE_E result;
}SIM_SIG_UPDATE_LP_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_AD_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_AD_T ad_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_AD_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_SST_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_SST_T sst_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_SST_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_PHASE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_PHASE_T phase_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_PHASE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_ELP_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_ELP_T    elp_mapping;
    SIM_RESULT_READ_FILE_E  result;
}SIM_SIG_READ_ELP_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_ELP_T    elp_mapping;
}SIM_SIG_UPDATE_ELP_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_RESULT_UPDATE_FILE_E    result;
}SIM_SIG_UPDATE_ELP_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_IMSI_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_IMSI_T imsi_mapping;
    SIM_RESULT_READ_FILE_E  result;
}SIM_SIG_READ_IMSI_CNF_T;

//the Access control request reqest signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_ACC_REQ_T;

//the Access control request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    ACC_CONTROL_CLASS_T     acc_mapping;
    SIM_SERVICE_RESULT_E    result;
}SIM_SIG_READ_ACC_CNF_T;

//the HPLMN search period rquest request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_HPLMN_REQ_T;

//the HPLMN search period request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    HPLMN_TIMER_VALUE_T     hplmn_mapping;
    SIM_RESULT_READ_FILE_E  result;
}SIM_SIG_READ_HPLMN_CNF_T;

//Location Information request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_LOCI_REQ_T;

//Location Information request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_LOCI_T   loci_mapping;
    SIM_RESULT_READ_FILE_E  result;
}SIM_SIG_READ_LOCI_CNF_T;

//Location Information update request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_LOCI_T   loci_mapping;
}SIM_SIG_UPDATE_LOCI_REQ_T;

//Location Information update confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_RESULT_UPDATE_FILE_E    result;
}SIM_SIG_UPDATE_LOCI_CNF_T;

//Cipher Key request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_KC_REQ_T;

//Cipher Key request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_KC_T kc_mapping;
    SIM_RESULT_READ_FILE_E  result;
}SIM_SIG_READ_KC_CNF_T;

//Cipher Key update request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_KC_T kc_mapping;
}SIM_SIG_UPDATE_KC_REQ_T;

//Cipher Key udpate confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_RESULT_UPDATE_FILE_E    result;
}SIM_SIG_UPDATE_KC_CNF_T;

//BCCH information request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_BCCH_REQ_T;

//BCCH information request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_BCCH_T   bcch_mapping;
    SIM_RESULT_READ_FILE_E  result;
}SIM_SIG_READ_BCCH_CNF_T;

//BCCH information update request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_BCCH_T   bcch_mapping;
}SIM_SIG_UPDATE_BCCH_REQ_T;

//BCCH information update confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_RESULT_UPDATE_FILE_E    result;
}SIM_SIG_UPDATE_BCCH_CNF_T;

//Forbiden PLMN request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_FPLMN_REQ_T;

//Forbiden PLMN request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    FPLMN_LIST_T fplmn_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_FPLMN_CNF_T;

//Forbiden PLMN udpate request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    FPLMN_LIST_T flpmn_mapping;
}SIM_SIG_UPDATE_FPLMN_REQ_T;

//Forbiden PLMN update confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_RESULT_UPDATE_FILE_E result;
}SIM_SIG_UPDATE_FPLMN_CNF_T;

//the PLMN selector request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_PLMNSEL_REQ_T;

//the PLMN selector request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_PLMNSEL_T plmn_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_PLMNSEL_CNF_T;

//the PLMN selector update request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_PLMNSEL_T plmn_mapping;
}SIM_SIG_UPDATE_PLMNSEL_REQ_T;

//the PLMN selector update confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_RESULT_UPDATE_FILE_E result;
}SIM_SIG_UPDATE_PLMNSEL_CNF_T;

//the Cell broadcast message identifier request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_CBMI_REQ_T;

//the Cell broadcast message identifier request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_CBMI_T cbmi_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_CBMI_CNF_T;

//the Cell broadcast message identifier update request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_CBMI_T cbmi_mapping;
}SIM_SIG_UPDATE_CBMI_REQ_T;

//the Cell broadcast message identifier update confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_RESULT_UPDATE_FILE_E result;
}SIM_SIG_UPDATE_CBMI_CNF_T;

//the Group identifier level 1 request requet signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_GID_REQ_T;

//the Group identifier level 1 request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_GID_T gid_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_GID_CNF_T;

//the Cell Broadcast Message range identifier request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_CBMIR_REQ_T;

//the Cell Broadcast Message range identifier request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_CBMIR_T cbmir_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_CBMIR_CNF_T;

//the Cell Broadcast Message range identifier update request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_CBMIR_T cbmir_mapping;
}SIM_SIG_UPDATE_CBMIR_REQ_T;

//the Cell Broadcast Message range identifier update confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_RESULT_UPDATE_FILE_E result;
}SIM_SIG_UPDATE_CBMIR_CNF_T;

//Depersonalisation Contorl Keys request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_DCK_REQ_T;

//Depersonalisation Contorl Keys request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_DCK_T dck_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_DCK_CNF_T;

//Network's indication of alerting request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_NIA_REQ_T;

//Network's indication of alerting request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_NIA_T nia_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_NIA_CNF_T;

//Enhanced Multi Level Pre-emption and Priority request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_EMLPP_REQ_T;

//Enhanced Multi Level Pre-emption and Priority request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_EMLPP_T emlpp_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_EMLPP_CNF_T;

//Automatic Answer on eMLLPP service request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_AAEM_REQ_T;

//Automatic Answer on eMLLPP service request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_AAEM_T aaem_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_AAEM_CNF_T;

//Automatic Answer on eMLLPP service request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_AAEM_T aaem_mapping;
}SIM_SIG_UPDATE_AAEM_REQ_T;

//Automatic Answer on eMLLPP service request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_RESULT_UPDATE_FILE_E result;
}SIM_SIG_UPDATE_AAEM_CNF_T;

//voice Broadcase service request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_VBS_REQ_T;

//voice Broadcase service request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_VBS_T vbs_support;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_VBS_CNF_T;

//the serious error indication signal to indicate the
//upper layer that there are serious error in the SIM.
typedef struct
{
	SIGNAL_VARS
}SIM_SIG_SERIOUS_ERROR_IND_T;

//voice broadcast service status request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_VBSS_REQ_T;

//voice broadcast service status request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_VBSS_T vbss_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_VBSS_CNF_T;

//voice group call service request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_VGCS_REQ_T;

//voice group call service request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_VGCS_T vgcs_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_VGCS_CNF_T;

//voice group call service status request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_READ_VGCSS_REQ_T;

//voice group call service status request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_VGCSS_T vgcss_mapping;
    SIM_RESULT_READ_FILE_E result;
}SIM_SIG_READ_VGCSS_CNF_T;


//the Accumulated Call meter reqest request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_AOC_READ_ACM_REQ_T;

//the Accumulated Call meter reqest confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_ACM_T    acm_mapping;
    SIM_RESULT_READ_FILE_E  result;
}SIM_SIG_AOC_READ_ACM_CNF_T;

typedef enum _SIM_BEARER_CAPABILITY_TYPE_E
{
    SIM_BEARER_CAPABILITY_LINE_1     = 0,
    SIM_BEARER_CAPABILITY_LINE_2     = 1,
    SIM_BEARER_CAPABILITY_LINE_MAX
} SIM_BEARER_CAPABILITY_TYPE_E;

//the Accumulated Call meter initialize request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_BEARER_CAPABILITY_TYPE_E selected_line;
}SIM_SIG_AOC_INIT_ACM_REQ_T;

//the Accumulated Call meter initialize confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_RESULT_UPDATE_FILE_E    result;
}SIM_SIG_AOC_INIT_ACM_CNF_T;

//the Accumulated Call meter increase request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_BEARER_CAPABILITY_TYPE_E selected_line;
    SIM_INCREASE_DATA_T acm_data;
}SIM_SIG_AOC_INCREASE_ACM_REQ_T;

//the Accumulated Call meter increase confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_INCREASE_RESULT_T   increase_result;
    SIM_RESULT_INCREASE_FILE_E  result;
}SIM_SIG_AOC_INCREASE_ACM_CNF_T;

//the Accumulated call meter maximum value request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_AOC_READ_ACMMAX_REQ_T;

//the Accumulated call meter maximum value request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_ACMMAX_T acmmax_mapping;
    SIM_RESULT_READ_FILE_E  result;
}SIM_SIG_AOC_READ_ACMMAX_CNF_T;

//the Accumulated call meter maximum value update request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_ACMMAX_T acm_mapping;
}SIM_SIG_AOC_UPDATE_ACMMAX_REQ_T;

//the Accumulated call meter maximum value update confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_RESULT_UPDATE_FILE_E    result;
    SIM_EF_MAPPING_ACMMAX_T acm_mapping;
}SIM_SIG_AOC_UPDATE_ACMMAX_CNF_T;

//the Price per Unit and currency table request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_AOC_READ_PUCT_REQ_T;

//the Price per Unit and currency table request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_PUCT_T   puct_mapping;
    SIM_RESULT_READ_FILE_E  result;
}SIM_SIG_AOC_READ_PUCT_CNF_T;

//the Price per Unit and currency table request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_EF_MAPPING_PUCT_T   puct_mapping;
}SIM_SIG_AOC_UPDATE_PUCT_REQ_T;

//the Price per Unit and currency table update confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    BOOLEAN is_service_support;
	SIM_EF_MAPPING_PUCT_T   puct_mapping;
    SIM_RESULT_UPDATE_FILE_E    result;
}SIM_SIG_AOC_UPDATE_PUCT_CNF_T;


//the record EF(including EFadn,EFfdn,EFmsisdn,EFlnd,EFsmsp,EFsdn,EFbdn)
//information request request signal
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_FILE_NAME_E file_name;
}SIM_SIG_GET_RECORD_EF_INFO_REQ_T;

//the record EF(including EFadn,EFfdn,EFmsisdn,EFlnd,EFsmsp,EFsdn,EFbdn)
//information request operation result
typedef enum
{
    SIM_GET_INFO_OK,
    SIM_GET_INFO_INVALIDE_FILE,
    SIM_GET_INFO_ACCESS_FAILED,
    SIM_GET_INFO_SELECT_ERROR,
    SIM_GET_INFO_UNKNOW_ERROR
}SIM_RESULT_GET_RECORD_INFO_E;

//the record EF(including EFadn,EFfdn,EFmsisdn,EFlnd,EFsmsp,EFsdn,EFbdn)
//information request confirm signal
typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    SIM_FILE_NAME_E file_name;
    BOOLEAN is_service_support;
    SIM_RECORD_EF_INFO_T info;
    uint8 dn_ext_num;
    SIM_RESULT_GET_RECORD_INFO_E result;
}SIM_SIG_GET_RECORD_EF_INFO_CNF_T;

//the session termination service request signal
typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
	SIM_EF_MAPPING_LOCI_T   loci_mapping;	//Location Information update data
	KC_T					kc_mapping;		//Cipher Key update data
	SIM_EF_MAPPING_BCCH_T	bcch_mapping;	//BCCH Information update data
	FPLMN_LIST_T			fplmn_mapping;	//Forbidden PLMN update data
	SIM_INCREASE_DATA_T   acm_data;	//the Advice of Charge increase data
}SIM_SIG_SESSION_TERMINATION_REQ_T;

//the resut type of the session termination service request
typedef enum
{
	SIM_ST_OK,
    SIM_ST_UPDATE_LOCI_ERROR,
    SIM_ST_UPDATE_KC_ERROR,
    SIM_ST_UPDATE_BCCH_ERROR,
    SIM_ST_UPDATE_FPLMN_ERROR,
    SIM_ST_INCREASE_ACM_ERROR
}SIM_RESULT_SESSION_TERMINATION_E;


typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
	SIM_RESULT_SESSION_TERMINATION_E result;	//the result of Session Termination 
}SIM_SIG_SESSION_TERMINATION_CNF_T;

#ifdef SIM_TEST_IN_WIN32

typedef struct
{
	SIM_CHV_T chv1;
	SIM_CHV_T unblock_chv1;
	SIM_CHV_T chv2;
	SIM_CHV_T unblock_chv2;
}SIM_ALL_CHV_T;
typedef enum
{
	POWER_ON_WITH_NO_SIM,
	POWER_ON_WITH_WRONG_SIM,
	POWER_ON_WITH_GOOD_SIM,
	REJECT_SIM,
	INSERT_WRONG_SIM,
	INSERT_GOOD_SIM
}WIN_OPERATION_TYPE_E;

typedef struct
{
	SIGNAL_VARS
	WIN_OPERATION_TYPE_E operation_type;
}SIM_SIG_TIW_TO_WIN_DUMMY_T;
#endif

typedef struct
{
	SIGNAL_VARS
}SIM_SIG_SELF_CARD_IN_IND_T;

typedef struct
{
	SIGNAL_VARS
}SIM_SIG_SELF_CARD_OUT_IND_T;

typedef enum
{
	SIM_MM_DATA_OK,
	SIM_MM_DATA_IMSI_ERROR,
	SIM_MM_DATA_LOCI_ERROR,
	SIM_MM_DATA_PLMN_SEL_ERROR,
	SIM_MM_DATA_FPLMN_ERROR,
	SIM_MM_DATA_KC_ERROR,
	SIM_MM_DATA_ACC_ERROR,
	SIM_MM_DATA_HPLMN_ERROR,
	SIM_MM_DATA_KC_GPRS_ERROR,
	SIM_MM_DATA_LOCI_GPRS_ERROR
}SIM_MM_DATA_RESULT_E;

typedef enum
{
    SIM_MM_AUTH_OK,
    SIM_MM_AUTH_FAILED,
    SIM_MM_AUTH_SELECT_GSM_FAILED,
    SIM_MM_AUTH_UPDATE_KC_FAILED,
    SIM_MM_AUTH_SYNC_FAILED,
    SIM_MM_AUTH_MAC_INCORRECT
}SIM_MM_AUTH_RESULT_E;

typedef enum
{
    SIM_GMM_AUTH_OK,
	SIM_GMM_AUTH_FAILED,
    SIM_GMM_AUTH_SELECT_GSM_FAILED,
    SIM_GMM_AUTH_UPDATE_KC_FAILED,
    SIM_GMM_AUTH_SYNC_FAILED,
    SIM_GMM_AUTH_MAC_INCORRECT,
    SIM_GMM_AUTHENTICATION_UNACCEPTABLE
}SIM_GMM_AUTH_RESULT_E;



typedef struct
{
    SIGNAL_VARS
    SIM_FILE_NAME_E file_name;
    SIM_ACCESS_CONDITION_TYPE_E acc_type;
}SIM_SIG_GET_ACCESS_INFO_REQ_T;

typedef enum
{
    SIM_GAI_OK,
    SIM_GAI_CONTRADICTION_FILE_TYPE,
    SIM_GAI_FILE_NOT_EXIST,
    SIM_GAI_UNKNOW_ERROR
}SIM_GET_ACCESS_INFO_RESULT_E;

typedef struct
{
    SIGNAL_VARS
    SIM_ACCESS_CONDITION_E ai;
    SIM_GET_ACCESS_INFO_RESULT_E result;
}SIM_SIG_GET_ACCESS_INFO_CNF_T;

typedef struct
{
    SIGNAL_VARS
}SIM_SIG_GET_SPN_REQ_T;

typedef struct
{
    SIGNAL_VARS
    SIM_RESULT_READ_FILE_E result;
    BOOLEAN is_service_support;
    SIM_EF_MAPPING_SPN_T spn_mapping;
}SIM_SIG_GET_SPN_CNF_T;

//the request signal to query the SIM card status,use in the SIM card
//reader to make sure that the simc is workable
typedef struct
{
    SIGNAL_VARS
}SIM_SIG_QUERY_CARD_STATUS_REQ_T;

//the request signal to query the SIM card status,use in the SIM card
//reader to make sure that the simc is workable
typedef struct
{
    SIGNAL_VARS
    BOOLEAN     is_chv1_enable;
    BOOLEAN     is_card_present;    //is the card present in the seat
    SIM_INSERT_STATUS_E card_inserted_status;   //the SIM card inserted status
    SIM_CARD_STATE_E card_state;    //the current card state
}SIM_SIG_QUERY_CARD_STATUS_CNF_T;


//the request signal to get the file info in the SIM card.supply this service
//to the sim reader
typedef struct
{
    SIGNAL_VARS
    uint16      file_id;
    uint8       path_len;
    uint8       path_id[16];//the same size as TD IPC
}SIM_SIG_GET_FILE_INFO_REQ_T;

typedef enum
{
    SIM_GET_FILE_INFO_OK,
    SIM_GET_FILE_NOT_EXISTE,
    SIM_GET_UNKNOW_ERROR
}SIM_GET_FILE_INFO_RESULT_E;

//the request signal to get the file info in the SIM card.supply this service
//to the sim reader
typedef struct
{
    SIGNAL_VARS
    uint16 file_id;      //the file to be get information
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;  //the select data of the file
    SIM_GET_FILE_INFO_RESULT_E result;          //the get file info operatin result
}SIM_SIG_GET_FILE_INFO_CNF_T;

//the signal to change the arm clock
typedef struct
{
    SIGNAL_VARS
    uint32 clk;
}SIM_SIG_SET_ARM_CLK_REQ_T;

typedef SIM_SIG_SET_ARM_CLK_REQ_T SIM_SIG_SET_ARM_CLK_CNF_T;

/*
typedef struct
{
    SIGNAL_VARS
    uint8 pc_len;   //the length of the proactive command
}SIM_SIG_PC_EXIST_IND_T;
*/
/*
//the Proactive Command indication signal from the SIM layer
typedef struct
{
    SIGNAL_VARS
    SIM_FETCH_DATA_T pc_data;   //the proactive command data buf
}SIMAT_SIG_PC_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIM_FETCH_DATA_T pc_data;   //the proactive command data buf
}SIM_SIG_PC_EXIST_IND_T;

//the Envelope request signal
typedef struct
{
    SIGNAL_VARS
    SIM_ENVELOPE_DATA_T envelope_data;  //the data buf that enveloped in
}SIMAT_SIG_ENVELOPE_REQ_T;

typedef enum
{
    SIMAT_EN_OK,
	SIMAT_EN_OK_WITH_PC,
    SIMAT_EN_SIM_BUSY,
    SIMAT_EN_OK_WITH_DATA,
    SIMAT_EN_ERROR_WITH_DATA,
    SIMAT_EN_CARD_NOT_READY,
    SIMAT_EN_CARD_NOT_SUPPORT,
    SIMAT_EN_ERROR_UNKNOW
}SIMAT_ENVELOPE_RESULT_E;

//the envelope confirm singal
typedef struct
{
    SIGNAL_VARS
    SIM_ENVELOPE_RESPONSE_T response;    //the envelope result data come from the sim 
    SIMAT_ENVELOPE_RESULT_E result;     //the envelop result
}SIMAT_SIG_ENVELOPE_CNF_T;

//the TERMINAL CONFIRM signal
typedef struct
{
    SIGNAL_VARS
    SIM_TERMINAL_RESPONSE_DATA_T response;  //the respone send to the SIM
}SIMAT_SIG_TERMINAL_RESPONSE_REQ_T;

typedef struct
{
    SIGNAL_VARS
}SIMAT_SIG_POLLING_IND_T;

typedef struct
{
    SIGNAL_VARS
}SIMAT_SIG_TERMINAL_PROFILE_IND_T;

//the signal to notify the STK task that the current STK applciation session
//is terminated
typedef struct
{
    SIGNAL_VARS
}SIMAT_SIG_SESSION_TERMINATED_IND_T;

//the terminal profile request signal structure
typedef struct
{
    SIGNAL_VARS
    SIM_TERMINAL_PROFILE_DATA_T profile;
}SIM_SIG_TERMINAL_PROFILE_REQ_T;

//the confirm signal struction of the terminal profile request
typedef struct
{
    SIGNAL_VARS
    SIM_TERMINAL_PROFILE_RESULT_E result;
}SIM_SIG_TERMINAL_PROFILE_CNF_T;

//the power off ind to the stk layer
typedef struct
{
	SIGNAL_VARS
}SIM_SIG_SIMAT_POWER_OFF_IND_T;

//the response signal struction of the sms data download ind
#define STK_MAX_SMS_PP_ACK_LEN    128
typedef struct
{	
    uint8   len;        //the acknowledgement length
    uint8   ack_arr[STK_MAX_SMS_PP_ACK_LEN];  //the acknowledgement data
}STK_SMS_PP_ACK_T;

//the SMS-PP data download result
typedef enum
{
    STK_SMSPP_OK,     //the SMS-PP data download success
    STK_SMSPP_OK_WITH_ACK,    //the SMS-PP data download success with acknowledgement for the network
    STK_SMSPP_BUSY,   //the Application toolkit busy
    STK_SMSPP_ERROR,  //SIM data download error
    STK_SMSPP_ERROR_WITH_MESSAGE  //sim data download error with a message for the network
}STK_SMS_PP_RESULT_E;

typedef struct
{
    SIGNAL_VARS
	STK_SMS_PP_ACK_T ack;         //the SIM acknowledgement,shoud be send back to the network
    STK_SMS_PP_RESULT_E result;   //the SMS-PP data download result
}STK_SMS_PP_CNF_T;*/
#define SIM_REFRESH_MAX_FILE_NUM 40
typedef struct
{
    SIGNAL_VARS
    BOOLEAN call_state;     // true: in call, false: not in call
}SIM_SIG_SWITCH_CHECK_CARD_PRES_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32	                command_ref;
    SIM_REFRESH_TYPE_E      refresh_type;
    uint8                   file_num;                               //the file number in the file list
    SIM_FILE_NAME_E         file_arr[SIM_REFRESH_MAX_FILE_NUM];     //the file array
}SIM_SIG_REFRESH_CARD_PRESENT_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32	                command_ref;
    SIM_REFRESH_TYPE_E      refresh_type;
    SIM_FILE_NAME_E         file_arr[SIM_REFRESH_MAX_FILE_NUM];     //the file array
    uint8                   file_num;                               //the file number in the file list
    BOOLEAN                 refresh_result;
	SIM_CARD_PRESENT_INFO   present_info;
}SIM_SIG_REFRESH_CARD_PRESENT_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32	command_ref;
}SIM_SIG_REFRESH_INI_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32	command_ref;
    uint8   file_num;   //the file number in the file list
    SIM_FILE_NAME_E file_arr[SIM_REFRESH_MAX_FILE_NUM];   //the file array
}SIM_SIG_REFRESH_FILE_CHG_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32	command_ref;
    uint8   file_num;   //the file number in the file list
    SIM_FILE_NAME_E file_arr[SIM_REFRESH_MAX_FILE_NUM];   //the file array
}SIM_SIG_REFRESH_INI_FILE_CHG_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32	command_ref;
}SIM_SIG_REFRESH_INI_FULL_REQ_T;

typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
}SIM_SIG_REFRESH_RESET_REQ_T;

typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
	SIM_RESULT_INITIALIZE_SIM_E	result;		//the initialize result
	SIM_INI_INFO	ini_info;
    BOOLEAN is_bdn_enable;					//the bdn service
    BOOLEAN is_fdn_enable;					//the fdn service
    SIM_EF_MAPPING_AD_T		ad_mapping;		//the EF ad
}SIM_SIG_REFRESH_INI_CNF_T;	

typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    BOOLEAN result;
}SIM_SIG_REFRESH_RESET_CNF_T;

typedef struct
{
	SIM_FILE_NAME_E    file_name;          //the file_name
	uint8 			   record_num;         //the record number of the file
}SIM_FILE_INFO_T;

typedef struct
{
    SIGNAL_VARS
    uint32	command_ref;
    BOOLEAN result;
    uint8   file_num;   									//the file number in the file list
    SIM_FILE_INFO_T  file_info[SIM_REFRESH_MAX_FILE_NUM];   //the file info
}SIM_SIG_REFRESH_FILE_CHG_CNF_T;

typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
	SIM_RESULT_INITIALIZE_SIM_E	result;		//the initialize result
    SIM_INI_INFO	ini_info;
    BOOLEAN is_bdn_enable;					//the bdn service
    BOOLEAN is_fdn_enable;					//the fdn service
    SIM_EF_MAPPING_AD_T		ad_mapping;		//the EF ad
    BOOLEAN  get_file_info_result;
    uint8   file_num;   									//the file number in the file list
    SIM_FILE_INFO_T  file_info[SIM_REFRESH_MAX_FILE_NUM];   //the file info
}SIM_SIG_REFRESH_INI_FILE_CHG_CNF_T;	

//the singal structure of SIM_REFRESH_READ_T_FILE_REQ 
typedef struct
{
	SIGNAL_VARS
	uint32	command_ref; 
	SIM_FILE_NAME_E	file_name;	//the EF file ID
	uint8	record_id;
}SIM_SIG_REFRESH_READ_FILE_REQ_T;

//the signal structure of SIM_READ_REQ
typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
    SIM_FILE_NAME_E file_name;
	uint8	record_id;	
	SIM_RESULT_READ_FILE_E result;	//the Read file result
	SIM_FILE_STRUCTURE_BUF_U	 buf;		//the Read file result buf			
}SIM_SIG_REFRESH_READ_FILE_CNF_T;

typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
	SIM_RESULT_INITIALIZE_SIM_E	result;		//the initialize result
    SIM_INI_INFO	ini_info;
    BOOLEAN is_bdn_enable;					//the bdn service
    BOOLEAN is_fdn_enable;					//the fdn service
    SIM_EF_MAPPING_AD_T		ad_mapping;		//the EF ad
}SIM_SIG_REFRESH_INI_FULL_CNF_T;	

typedef struct
{
	SIGNAL_VARS
	uint32	command_ref;
}SIM_SIG_DPHONE_REFRESH_SKIP_CNF_T;	

typedef struct
{
    SIGNAL_VARS
}SIM_SIG_GET_CARD_TYPE_REQ_T;

typedef struct
{
    SIGNAL_VARS
}SIM_SIG_GET_CDMA_CARD_IMSI_REQ_T;

typedef enum
{
    CARD_TYPE_SIM = 0x1,    //SIM(GSM)
    CARD_TYPE_UIM,          //UIM(CDMA)
    CARD_TYPE_SIM_UIM_DUAL, //SIM&UIM(CDMA&GSM)
    CARD_TYPE_USIM,         //USIM(GSM/WCDMA)
    CARD_TYPE_USIM_UIM_DUAL,//USIM(CDMA/CDMA2000&GSM/WCDMA)
    CARD_TYPE_NUM
}CARD_TYPE_E;

typedef enum
{
    GET_RESULT_OK,
    GET_RESULT_CARD_NOT_READY,
    GET_RESULT_FAILED
}GET_CARD_TYPE_RESULT_E;

typedef struct
{
    SIGNAL_VARS
    GET_CARD_TYPE_RESULT_E result;
    BOOLEAN is_have_card;
    CARD_TYPE_E card_type;
}SIM_SIG_GET_CARD_TYPE_CNF_T;

typedef enum
{
    GET_CIMSI_RESULT_OK,
    GET_CIMSI_RESULT_NO_CARD,
    GET_CIMSI_RESULT_CARD_NOT_READY,
    GET_CIMSI_RESULT_FILE_NOT_EXIST,        
    GET_CIMSI_RESULT_FAILED
}GET_CDMA_CARD_IMSI_RESULT_E;

typedef struct
{
    SIGNAL_VARS
    GET_CDMA_CARD_IMSI_RESULT_E result;
    SIM_EF_MAPPING_CDMA_IMSI_M_T imsi_m;
}SIM_SIG_GET_CDMA_CARD_IMSI_CNF_T;

//information request request signal
#define MAX_DEEP_FILE_LEVEL 4
typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
	uint16 file_id;							//file id of the file
 	SIM_EF_STRUCTURE_E file_struct; 		//TRANSPARENT,LINEAR_FIXED or CYCLIC
	uint8  record_num;						//the record num of the record file,it is only used for LINEAR_FIXED or CYCLIC file
 	uint8  file_level;						//the file level of the file,such as EFimsi,the file path is 3F00/7F20/6F07,and the file level is 3 
 	uint16 file_path[MAX_DEEP_FILE_LEVEL]; 	//the path to access the file,the top file must be MF(3F 00),such as
 	  										//the EFimsi,the file path is 3F00/7F20/6F07

}SIM_SIG_READ_FILE_BUF_BY_PATH_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
	uint16  file_id;						//file id of the file
 	SIM_EF_STRUCTURE_E file_struct; 		//TRANSPARENT,LINEAR_FIXED or CYCLIC
 	uint8	record_num;						//the record num of the record file,it is only used for LINEAR_FIXED or CYCLIC file						
	SIM_RESULT_READ_FILE_E result;			//the Read file result
	uint16 data_len;						//the data len readed
	uint8 buf[SIM_MAX_FILE_STRUCTURE_LEN];	//the Read file result buf				
}SIM_SIG_READ_FILE_BUF_BY_PATH_CNF_T;

//Yang Wuding. Add for Dual SIM power off bug.
typedef enum
{
	SIM_NORMAL_POWER_OFF,        
	SIM_ABNORMAL_POWER_OFF
}SIM_POWER_OFF_TYPE_E;

typedef struct
{
    SIGNAL_VARS
    SIM_EF_MAPPING_LOCI_T loci;
}SIM_SIG_LOCI_UPDATE_IND_T;

typedef struct
{
	SIGNAL_VARS
}SIM_SIG_RESTORE_EXP_IND_T;

typedef struct _SIM_SIG_CRSM_REQ_T_s
{
    SIGNAL_VARS
    uint8           ins; 
    uint8           p1;
    uint8           p2;
    uint8           p3;
    uint16          file_id;
    uint8           data_ptr[SIM_SEND_DATA_SIZE];
    uint8           path_len;
    uint8           path_id[SIM_PATH_LEVEL];
} SIM_SIG_CRSM_REQ_T;

typedef struct _SIM_SIG_CSIM_REQ_T_s
{
    SIGNAL_VARS
    uint8           data_len;
    uint8           data_ptr[SIM_SEND_DATA_SIZE];
} SIM_SIG_CSIM_REQ_T;

typedef struct
{
    uint8 alpha_id_len; //the alpher id length
    uint8 number_len;   //the number length
    uint8 ccp_id;       //the CCP id in the EF_ccp,this member value must be right when
                        //upate the dialling number
    BOOLEAN is_ccp_exist;       //is CCP exist in the phone num
    BOOLEAN is_ton_npi_exist;   //the TON and NPI exist or not
    uint8 alpha_id[SIM_MAX_DN_ALPHA_IDENTIFY_LEN];  //the alpher id	
    uint8 numbers[SIM_MAX_DN_NUMBER_LEN];   //the dialling number    
    NUMBER_TYPE_E   ton;        //type of number
    NUMBER_PLAN_E   npi;        //the Number plan identifier
    SIM_EF_MAPPING_CCP_T ccp;   //the CCP
}SIM_DIALLING_NUMBER_T;

typedef struct _SIM_SIG_CRSM_CNF_T_s
{
    SIGNAL_VARS
    uint16          file_id;
    uint8           sw1;
    uint8           sw2;
    uint8           data_len;
    uint8           rsp_data[255];
    BOOLEAN     result;
    BOOLEAN     is_update;
    uint8           dn_record_id;
    SIM_DIALLING_NUMBER_T dn_mapping;
} SIM_SIG_CRSM_CNF_T;

typedef struct _SIM_SIG_CSIM_CNF_T_s
{
    SIGNAL_VARS
    BOOLEAN     result;
    uint8           data_len;
    uint8           rsp_data[255];
} SIM_SIG_CSIM_CNF_T;

typedef struct
{
    SIGNAL_VARS
}SIM_SIG_HOT_PLUG_IN_IND_T;

typedef struct
{
    SIGNAL_VARS
    uint32	                command_ref;
    uint8                   record_id;
}SIM_SIG_IMG_READ_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32	                command_ref;
    uint8                   record_id;
    SIM_RESULT_READ_FILE_E result;	//the Read file result
    BOOLEAN             is_service_support;
    SIM_IMG_INFO_T    img_info;
}SIM_SIG_IMG_READ_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32	                command_ref;
    uint32                  task_id;
    uint32                  algo_index;
    BOOLEAN              is_mbbms_auth;
    uint8                    rand[128+1]; /*LV style, first byte is len*/
    uint8                    autn[128+1]; /*LV style, first byte is len*/
}SIM_SIG_COMMON_AUTH_REQ_T;//for common Authentication

typedef struct SIM_SIG_COMMON_ALGO_CNF_T
{
    SIGNAL_VARS
    uint32 command_ref;
    uint32 task_id;
    uint32 algo_index;
    SIM_MM_AUTH_RESULT_E         ret;
    uint8 sim_or_usim;                           /*0--sim, 1--usim*/
    uint8 kc[8+1];                                   /*LV style, first byte is len*/
    uint8 sres_res_autn[USIM_MAX_RES_LEN+1];  /*LV style, first byte is len*/
    uint8 ck[USIM_CK_LEN+1];                /*LV style, first byte is len*/
    uint8 ik[USIM_IK_LEN+1];                  /*LV style, first byte is len*/
}SIM_SIG_COMMON_ALGO_CNF_T;

typedef enum SIM_MULTI_SUBSCRIBER_ID_TYPE_E
{
    SIM_MULTI_SUBSCRIBER_NULL,//for cphs datamailbox and faxmailbox
    SIM_MULTI_SUBSCRIBER_ID_1,//line1 in cphs
    SIM_MULTI_SUBSCRIBER_ID_2,//line2 in cphs
    SIM_MULTI_SUBSCRIBER_MAX
}SIM_MULTI_SUBSCRIBER_ID_TYPE_E;

typedef enum SIM_VOICE_MAIL_TYPE_E
{
    SIM_VOICE_MAIL,
    SIM_VOICE_MAIL_FAX,
    SIM_VOICE_MAIL_EMAIL,
    SIM_VOICE_MAIL_OTHER
}SIM_VOICE_MAIL_TYPE_E;

typedef struct
{
    SIGNAL_VARS
    SIM_MULTI_SUBSCRIBER_ID_TYPE_E line_id;
    SIM_VOICE_MAIL_TYPE_E vm_type;
}SIM_SIG_READ_VOICE_MAIL_REQ_T;

typedef struct SIM_SIG_READ_VOICE_MAIL_CNF_T
{
    SIGNAL_VARS
    SIM_MULTI_SUBSCRIBER_ID_TYPE_E line_id;
    SIM_VOICE_MAIL_TYPE_E vm_type;
    BOOLEAN is_voicemail_exist;
    SIM_EF_MAPPING_MBNUM_T voice_mail;
}SIM_SIG_READ_VOICE_MAIL_CNF_T;

typedef struct
{
    SIGNAL_VARS
    SIM_MULTI_SUBSCRIBER_ID_TYPE_E line_id;
    SIM_VOICE_MAIL_TYPE_E vm_type;
    SIM_EF_MAPPING_MBNUM_T vm_content;
}SIM_SIG_UPDATE_VOICE_MAIL_REQ_T;

typedef struct SIM_SIG_UPDATE_VOICE_MAIL_CNF_T
{
    SIGNAL_VARS
    SIM_MULTI_SUBSCRIBER_ID_TYPE_E line_id;
    SIM_VOICE_MAIL_TYPE_E vm_type;
    BOOLEAN is_can_be_update;
    BOOLEAN result;
}SIM_SIG_UPDATE_VOICE_MAIL_CNF_T;

//language information update request signal
typedef struct
{
    SIGNAL_VARS
    SIM_EF_MAPPING_LP_T	    lp_mapping;
    SIM_EF_MAPPING_ELP_T    elp_mapping;
}SIM_SIG_UPDATE_LANGUAGE_REQ_T;

//language information update confirm signal
typedef struct
{
    SIGNAL_VARS
    BOOLEAN    is_update_ok;
	SIM_EF_MAPPING_LP_UNION_T   language_mapping;//the EFlp(EFli) or EFelp(EFpl)
}SIM_SIG_UPDATE_LANGUAGE_CNF_T;

typedef struct
{
    uint8 elp_1st_max_num;
    uint8 lp_1st_max_num;
    uint8 elp_2nd_max_num;
}SIM_LANGUAGE_REC_LEN_INFO_T;

//IPC
typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    uint16 file_id;
    uint8 path_len;
    uint8 path_ptr[SIM_PATH_LEVEL]; //16 is IPC DM_BASE defined
}SIM_SIG_READ_RAW_T_FILE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    SIM_RESULT_READ_FILE_E result;	//the Read file result
    uint16 file_id;
    uint16 data_len;
    uint8 *data_ptr;
}SIM_READ_RAW_T_FILE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    uint16 file_id;
    uint8 path_len;
    uint8 path_ptr[SIM_PATH_LEVEL];
    uint8 data_len;
    uint8 data_ptr[255];
}SIM_SIG_UPDATE_RAW_T_FILE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    SIM_RESULT_UPDATE_FILE_E result;	//the update file result
    uint16 file_id;
}SIM_UPDATE_RAW_T_FILE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    uint16 file_id;
    uint8 path_len;
    uint8 path_id[SIM_PATH_LEVEL];
    uint8 rec_num;
    uint8 rec_len;
}SIM_SIG_READ_RAW_R_FILE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    SIM_RESULT_READ_FILE_E result;	//the Read file result
    uint16 file_id;
    uint8 rec_num;
    uint8 data_len;
    uint8 *data_ptr;
}SIM_READ_RAW_R_FILE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    uint16 file_id;
    uint8 path_len;
    uint8 path_id[SIM_PATH_LEVEL];
    uint8 rec_num;
    uint8 rec_len;
    uint8 data_ptr[255];
}SIM_SIG_UPDATE_RAW_R_FILE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    SIM_RESULT_UPDATE_FILE_E result;	//the update file result
    uint16 file_id;
    uint8 rec_num;
}SIM_UPDATE_RAW_R_FILE_CNF_T;

typedef enum
{
   IPC_MN_SIM_ENHANCED_SEARCH_FROM_OFFSET_FORWARD = 0x04,
   IPC_MN_SIM_ENHANCED_SEARCH_FROM_OFFSET_BACKWARD = 0x05,
   IPC_MN_SIM_ENHANCED_SEARCH_FROM_OFFSET_FORWARD_FROM_NEXT_REC = 0x06,
   IPC_MN_SIM_ENHANCED_SEARCH_FROM_OFFSET_BACKWARD_FROM_NEXT_REC = 0x07,
   IPC_MN_SIM_ENHANCED_SEARCH_FROM_VALUE_FORWARD = 0x0C,
   IPC_MN_SIM_ENHANCED_SEARCH_FROM_VALUE_BACKWARD = 0x0D,
   IPC_MN_SIM_ENHANCED_SEARCH_FROM_VALUE_FORWARD_FROM_NEXT_REC = 0x0E,
   IPC_MN_SIM_ENHANCED_SEARCH_FROM_VALUE_BACKWARD_FROM_NEXT_REC = 0x0F,
   IPC_MN_SIM_ENHANCED_SEARCH_RFU = 0xFF
}IPC_SIM_ENHANCED_SEARCH_IND_E;//MN_SIM_ENHANCED_SEARCH_IND_E;

typedef enum
{
    IPC_MN_SIM_SIMPLE_SEARCH_FORWARD = 0x04,
    IPC_MN_SIM_SIMPLE_SEARCH_BACKWARD = 0x05,
    IPC_MN_SIM_ENHANCED_SEARCH = 0x06,
    IPC_MN_SIM_PROPRIETARY_SEARCH = 0x07
}IPC_SIM_SEARCH_MODE_E;//MN_SIM_SEARCH_MODE_E;

typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    uint16 file_id;
    uint8 path_len;
    uint8 *path_ptr;
    IPC_SIM_SEARCH_MODE_E search_mode;   //Search mode, For Simple search and Enhanced
    IPC_SIM_ENHANCED_SEARCH_IND_E enhanced_search_ind;   //The first byte of data in enhanced search
    uint8 enhanced_search_offset;   //The second byte of data in enhanced search
    uint8 rec_num;    //The base record
    uint8 search_string_len;   //The length of the search record reference data
    uint8 *search_string_ptr;   //the search reference data
}SIM_SIG_SEARCH_R_FILE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32 command_ref;
    BOOLEAN                         is_ok; //indicate whether successfully
    uint16                              file_id;	//file id of the file
    uint16                              search_result_len;
    uint8                               *search_result_ptr;
}SIM_SEARCH_RECORD_CNF_T;
//IPC

typedef struct
{
    SIM_MM_AUTH_RESULT_E auth_result;
    uint8   sres_len;
    uint8   sres[USIM_MAX_RES_LEN];
    uint8   ck_len;
    uint8   ck[USIM_CK_LEN];
    uint8   ik_len;
    uint8   ik[USIM_IK_LEN];
    uint8   kc_len;
    uint8   kc[8];
}SIM_AUTH_RESPONSE_T;

/********************************************************************************/
//  Description : The service is allocated and activated, or not
//  Global resource dependence : 
//  Author: 
//  Note: 
/********************************************************************************/
BOOLEAN SIMEF_IsServiceEnabled(                                        //RETURN:
                               uint32                  cur_sim_task,   //IN:
                               SIM_SERVICE_TYPE_PH_2_E service_type    //IN:
                               );

#ifdef   __cplusplus
    }
#endif
#endif //_SIMSIGNAL_H_
