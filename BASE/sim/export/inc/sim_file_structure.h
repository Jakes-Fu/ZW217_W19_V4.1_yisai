/******************************************************************************
 ** File Name:      sim_file_structure.h                                      *
 ** Author:         Raislin.Kong                                              *
 ** Date:           2002.02                                                   *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the header file about the sim file structure.ref  *
 **				    to GSM 11.11 section 10 Contents of the Elementary Files  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2002.02        Raislin.Kong      create                                   *
 ******************************************************************************/

#ifndef _FILE_STRUCTURE_H_
#define _FILE_STRUCTURE_H_
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
#include "gsm_gprs.h"               //need by PLMN_T


#define MODE(P1,P2)					((P1) - ((P1)/(P2)) * (P2))

#define SIM_MAX_RECORD_LEN          255
#define SIM_MAX_RECORD_NUM		510//255*2
//the max file structure length in the sim file
#define SIM_MAX_FILE_STRUCTURE_LEN  256

#define SIM_MAX_ECC_NUM				15
//the below macro define ref to the corresponding EF file content 
//of 3GP TS V8.1.0 Section 10 "Contents of Eelement Files"
//--------------------------EF_ICCID macro define-----------------------
#define SIM_EF_ICCID_ID_NUM_LEN		10

//--------------------------EF_ONSTRING macro define-----------------------
#define SIM_EF_MAX_ONSTRING_LEN     60  //need further consideration, no spec support

//--------------------------EF_ONSHORT macro define-----------------------
#define SIM_EF_MAX_ONSHORTFORM_LEN  10

//--------------------------EF_IMSI macro define-----------------------
#define SIM_EF_IMSI_NUM				15

//--------------------------EF_SPN macro define-----------------------
#define SIM_EF_SPN_SPN_LEN			16

//--------------------------EF_BCCH macro define-----------------------
#define SIM_EF_BCCH_BCCH_LEN		16

//--------------------------EF_ACC macro define-----------------------
#define SIM_EF_ACC_ACC_TYPE_NUM		15

//--------------------------EF_FPLMN macro define-----------------------
#define	SIM_EF_FPLMN_FPLMN_NUM		4

//--------------------------EF_VGCS macro define-----------------------
#define SIM_EF_VGCS_GID_SIZE			4
#define SIM_EF_VGCS_VGI_NUM			50

//--------------------------EF_VGCSS macro define-----------------------
#define SIM_EF_VGCSS_SIZE				7

//--------------------------EF_VBS macro define-----------------------
#define SIM_EF_VBS_GID_SIZE			4
#define SIM_EF_VBS_VBI_NUM				50

//--------------------------EF_VBSS macro define-----------------------
#define SIM_EF_VBSS_SIZE				7

//--------------------------EF_NIA macro define-----------------------
//ref to GSM 2.7 section B.1.27	Network's indication of alerting in the MS
#define SIM_EF_NIA_MAX_INFORMATIVE_TEXT_LEN	25
#define SIM_EF_NIA_CATEGORY_LEN				1

//--------------------------EF_KC macro define------------------------
#define SIM_EF_KC_KC_LEN					8


//--------------------------EF_AD macro define------------------------
#define SIM_EF_AD_OFM_ENABLE_MARK	0X01

//--------------------------EF_LOCI macro define-----------------------
#define SIM_EF_LOCI_UPDATE_STATUS_MARK	0x07
#define SIM_EF_LOCI_GPRS_UPDATE_STATUS_MARK	0x07

//--------------------------EF_PUCT macro define-----------------------
#define SIM_EF_PUCT_EPPU_MARK			0x0f
#define SIM_EF_PUCT_EX_SIGN_MARK		0x10
#define SIM_EF_PUCT_EX_MARK			0xe0

//--------------------------EF_CBMI macro define-----------------------
#define SIM_EF_CBMI_CB_SIZE		2

//--------------------------EF_CBMID macro define-----------------------
#define SIM_EF_CBMID_CB_SIZE		2

//--------------------------EF_CBMIR macro define-----------------------
#define SIM_EF_CBMIR_CB_SIZE		4

//--------------------------EF_CNL macro define-----------------------
#define SIM_EF_CNL_ELEMENT_SIZE			6

//--------------------------EF_SUME macro define-----------------------
#define SIM_EF_SUME_MAX_ALPHA_ID_LEN		255
#define SIM_EF_SUME_ICONID_SIZE			4
#define SIM_EF_SUME_ICONID_REST_LEN		2

//--------------------------EF_SMS macro define-----------------------
//the record len of EFsms
#define SIM_EF_SMS_RECORD_LEN		176
#define SIM_EF_SMS_FREE             0x00
#define SIM_EF_SMS_MT_READED        0x01
#define SIM_EF_SMS_MT_TO_BE_READ    0x03
#define SIM_EF_SMS_MO_TO_BE_SENT    0x07
#define SIM_EF_SMS_MO_NO_SR         0x05
#define SIM_EF_SMS_MO_SR_NOT_RECV   0x0D
#define SIM_EF_SMS_MO_SR_NOT_SAVE   0x15
#define SIM_EF_SMS_MO_SR_SAVED      0x1D

//--------------------------EF_PNN macro define-----------------------
//the mini length of PNN record
#define SIM_EF_PNN_MIN_RECORD_LEN		3

//--------------------------EF_OPL macro define-----------------------
//the mini length of OPL record
#define SIM_EF_OPL_MIN_RECORD_LEN		8

//--------------------------EF_SMSP macro define-----------------------
//the mini length of SMSP record
#define SIM_EF_SMSP_MIN_RECORD_LEN		28
//the length of the address object length in the EFsmsp
#define SIM_EF_SMSP_ADD_OBJECT_LEN      12
//the value of the paramter indicator that there are no paremeter present
#define SIM_EF_SMSP_EMPTY_INDICATOR		0xe0
//the TP-Destination Address indeicator
#define SIM_EF_SMSP_TP_DA_INDICATOR		0x01
//the TS-Service Centre Address indicator
#define SIM_EF_SMSP_TP_SC_INDICATOR		0x02
//the TP-Protocal Indentifier
#define SIM_EF_SMSP_TP_PI_INDICATOR		0x04
//the TP-Data Coding Scheme
#define SIM_EF_SMSP_TP_DCS_INDICATOR	0x08
//the TP-Validity Period
#define SIM_EF_SMSP_TP_VP_INDICATOR		0x10

//--------------------------EF_SMSP macro define-----------------------
//the min length of the EF_SMS file
#define SIM_EF_SMSS_MIN_FILE_SIZE		2
#define SIM_EF_SMSS_MEM_EXCEED_MARK		0x01

//--------------------------EF_SMSP macro define-----------------------
#define SIM_EF_SMSR_RECORD_LEN			30

//--------------------------EF_EXTx macro define-----------------------
#define SIM_EF_EXT_DATA_LEN             11
#define SIM_EF_EXT_ADDITIONAL_DATA_LEN  10              //CR26616,add 
#define SIM_EF_EXT_RECORD_LEN           13

//--------------------------EF_CCP macro define-----------------------
#define SIM_EF_CCP_BC_INFO_LEN          10
#define SIM_EF_CCP_RESERVED_BYTE_LEN    4
#define SIM_EF_CCP_RECORD_LEN           14


//--------------------------EF_DN   macro define------------------------
//the mini record length of the Dialling number 
#define SIM_EF_DN_MIN_RECORD_LEN        14
//the TON mark of the TON and NPI byte      
#define SIM_EF_DN_TON_MARK              0x70
//the NPI mark of the TON and NPI byte    
#define SIM_EF_DN_NPI_MARK              0x0f
//the DN Numger length in the DN EF file    
#define SIM_EF_DN_DN_LEN                10

//--------------------------EF_AAEM macro define--------------------------
#define SIM_EF_AAEM_FILE_LEN            1
#define SIM_B_PRIORITY_LEVEL_A          0x01
#define SIM_B_PRIORITY_LEVEL_B          0x02
#define SIM_B_PRIORITY_LEVEL_0          0x04
#define SIM_B_PRIORITY_LEVEL_1          0x08
#define SIM_B_PRIORITY_LEVEL_2          0x10
#define SIM_B_PRIORITY_LEVEL_3          0x20
#define SIM_B_PRIORITY_LEVEL_4          0x40

//--------------------------EF_eMLPP    macro define----------------------
#define SIM_EF_eMLPP_FILE_LEN           2

//see the imsi_m file structure of the china Unicom CDMA UIM card technical specification
#define SIM_EF_CDMA_IMSI_M_LEN 10 
//--------------------------EF_SAI    macro define----------------------
#define SIM_EF_SAI_LSA_INDICATOR_LEN	1
#define SIM_EF_SAI_LSA_TEXT_MAX_LEN	254
#define SIM_EF_SAI_LSA_ACTIVE_MASK	0x1

//--------------------------EF_SLL    macro define----------------------
#define SIM_EF_SLL_LSA_NAME_MAX_LEN			245
#define SIM_EF_SLL_MANDATORY_LEN				10
#define SIM_EF_SLL_ICONID_MASK					0x3
#define SIM_EF_SLL_IDLE_SUPPORT_MASK			0x4
#define SIM_EF_SLL_LSA_IDLE_SUPPORT_MASK		0x8

//--------------------------EF_IMG    macro define----------------------
#define SIM_EF_IMG_INSTANCE_NUM	25
#define SIM_EF_IMG_INSTANCE_SIZE	9

//--------------------------EF_ECCP    macro define----------------------
#define SIM_EF_ECCP_MANDATORY_LEN	15
#define SIM_EF_ECCP_MAX_LEN			255

//--------------------------EF_ECCP    macro define----------------------
#define SIM_EF_CMI_CMI_LEN			1

//--------------------------EF_PLMNWACT    macro define----------------------
#define SIM_EF_PLMNWACT_ARR_NUM         80
#define SIM_EF_PLMNWACT_ARR_SIZE        5

//--------------------------EF_OPLMNWACT    macro define----------------------
#if defined (_SUPPORT_IPC_) || defined (GSM_CUSTOMER_AFP_SUPPORT)
#define SIM_EF_OPLMNWACT_ARR_NUM        200
#else
#define SIM_EF_OPLMNWACT_ARR_NUM        80
#endif
#define SIM_EF_OPLMNWACT_ARR_SIZE       5

//--------------------------EF_HPLMNACT    macro define----------------------
#define SIM_EF_HPLMNWACT_ARR_NUM		2
#define SIM_EF_HPLMNWACT_ARR_SIZE		5

//--------------------------EF_CPBCCH    macro define----------------------
#define SIM_EF_CPBCCH_ARR_NUM		100
#define SIM_EF_CPBCCH_ELEMENT_SIZE	2

//--------------------------EF_ORPK    macro define----------------------
#define SIM_EF_ORPK_KEYID_MAX_LEN		100
#define SIM_EF_ORPK_MANDATORY_LEN	10

//--------------------------EF_ARPK    macro define----------------------
#define SIM_EF_ARPK_KEYID_MAX_LEN		100
#define SIM_EF_ARPK_MANDATORY_LEN	10

//--------------------------EF_TPRPK    macro define----------------------
#define SIM_EF_TPRPK_KEYID_MAX_LEN	100
#define SIM_EF_TPRPK_CERID_MAX_LEN	100
#define SIM_EF_TPRPK_MANDATORY_LEN	11

//----------------------EF_PLMNsel   macro define--------------------
#define SIM_EF_PLMNSEL_ARR_NUM	200
#define SIM_EF_PLMNSEL_ARR_SIZE	3

//----------------------EF_DIR   macro define--------------------
#define SIM_EF_DIR_SIZE	54

//----------------------EF_EHPLMN   macro define--------------------
#ifdef GSM_CUSTOMER_AFP_SUPPORT
#define SIM_EF_EHPLMN_ARR_NUM   85
#else
#define SIM_EF_EHPLMN_ARR_NUM   200
#endif
#define SIM_EF_EHPLMN_ARR_SIZE  3

//----------------------EF_MBI   macro define--------------------
#define SIM_EF_MBI_ARR_NUM      5
#define SIM_EF_MBI_ARR_SIZE     4

//----------------------EF_MBDN   macro define--------------------
#define SIM_EF_MBDN_ARR_NUM   5

//----------------------EF_NETPAR   macro define--------------------
#define SIM_MAX_ARFCN_NUM    32

//the BCD code byte
typedef	uint8	bcd;
typedef uint8	SIM_LANGUAGE_CODE;
//the Language code
typedef struct
{
	uint8 code[2];
}SIM_EXT_LANGUAGE_CODE_T;


typedef struct
{
	bcd id_num[SIM_EF_ICCID_ID_NUM_LEN];
}SIM_EF_MAPPING_ICCID_T;

typedef SIM_EXT_LANGUAGE_CODE_T SIM_EXT_LANGUAGE_CODE_ARR[SIM_MAX_VALIDATE_LANGUAGE_NUM];

typedef struct
{
    uint8 max_num;
	uint8 valid_num;	//the validate language code num 
	SIM_EXT_LANGUAGE_CODE_ARR lang_codes;//the language code num
}SIM_EF_MAPPING_ELP_T;

typedef SIM_LANGUAGE_CODE SIM_LANGUAGE_CODE_ARR[SIM_MAX_VALIDATE_LANGUAGE_NUM];

typedef struct
{
    uint8 max_num;
	uint8 valid_num;	//the validate language code num
	SIM_LANGUAGE_CODE_ARR lang_codes;//the language code num
}SIM_EF_MAPPING_LP_T;

typedef struct
{
    BOOLEAN is_1st_valid;
    uint8 elp_1st_max_num;
	uint8 elp_1st_valid_num;
    SIM_EXT_LANGUAGE_CODE_ARR elp_1st_lang_codes;
    BOOLEAN is_lp_valid;
    uint8 lp_1st_max_num;
	uint8 lp_1st_valid_num;
	SIM_LANGUAGE_CODE_ARR lp_1st_lang_codes;
    BOOLEAN is_2nd_valid;
    uint8 elp_2nd_max_num;
	uint8 elp_2nd_valid_num;
    SIM_EXT_LANGUAGE_CODE_ARR elp_2nd_lang_codes;
}SIM_EF_MAPPING_LP_UNION_T;

#define MAX_MSIN_BYTE_NUM   5
typedef struct
{
    PLMN_T      hplmn;                      //the home plmn of th mobile subscribe
    uint16      mcc;                        //the Mobile Country Code   
    uint16      mnc;                        //the Mobile Network Code
    uint8       msin_digit_num;             //
    uint8       msin[MAX_MSIN_BYTE_NUM];    //The Mobile Subscriber Identification Number  
    IMSI_T      imsi;                       //
} SIM_EF_MAPPING_IMSI_T;
//need include gsm_gprs.h

typedef struct
{
	uint32 acm_max_value;
}SIM_EF_MAPPING_ACMMAX_T;

typedef struct
{
	BOOLEAN	allocated;
	BOOLEAN	activated;
}SIM_SERVICE_INFO_T;

typedef struct
{
    uint8   bc_info[SIM_EF_CCP_BC_INFO_LEN];
    uint8   revers_bytes[SIM_EF_CCP_RESERVED_BYTE_LEN];
}SIM_EF_MAPPING_CCP_T;

typedef struct
{
	uint8   len;
	uint8 	onstring[SIM_EF_MAX_ONSTRING_LEN];
}SIM_EF_MAPPING_ONSTRING_T;

typedef struct
{
	uint8	onshort[SIM_EF_MAX_ONSHORTFORM_LEN];
}SIM_EF_MAPPING_ONSHORT_T;

typedef struct CPHS_SERVICE_TABLE_s
{
    uint8   csp;
    uint8   sst;
    uint8   mailboxnum;
    uint8   opnameshort;
    uint8   infonum;
} CPHS_SERVICE_TABLE;

typedef uint8 SIM_CPHS_PHASE_E;
#define PHASE1  1
#define PHASE2  2

typedef struct
{
    CPHS_SERVICE_TABLE    cphs_service_table;
    SIM_CPHS_PHASE_E   cphs_phase;
}SIM_EF_MAPPING_CPHSINFO_T;

typedef struct
{
    BOOLEAN is_datafax_flag_present;
	uint8	line1_callfrd_uncond;
    uint8	line2_callfrd_uncond;
    uint8	fax_callfrd_uncond;
    uint8	data_callfrd_uncond;
}SIM_EF_MAPPING_CFFLAG_T;


typedef struct
{
	BOOLEAN  is_cfu;  
    BOOLEAN  is_cfb;
    BOOLEAN  is_cfnry;
    BOOLEAN  is_cfnrc;
    BOOLEAN  is_ct;
}CALL_OFFERING_T;



typedef struct
{
	BOOLEAN  is_boac;  
    BOOLEAN  is_boic;
    BOOLEAN  is_boic_exhc;
    BOOLEAN  is_baic;
    BOOLEAN  is_bic_roam;
}CALL_RESTRICTION_T;


typedef struct
{
	BOOLEAN  is_mpty;  
    BOOLEAN  is_cug;
    BOOLEAN  is_aoc;
    BOOLEAN  is_pref_cug;
    BOOLEAN  is_cug_oa;
}OTHER_SUPP_SERV_T;


typedef struct
{
	BOOLEAN  is_hold;  
    BOOLEAN  is_cw;
    BOOLEAN  is_ccbs;
    BOOLEAN  is_u2u_signalling;
}CALL_COMPLETION_T;


typedef struct
{
	BOOLEAN  is_sm_mt;  
    BOOLEAN  is_sm_mo;
    BOOLEAN  is_sm_cb;
    BOOLEAN  is_reply_path;
    BOOLEAN  is_del_conf;
    BOOLEAN  is_protocol_id;
    BOOLEAN  is_validity_period;
}TELESERVICES_T;


typedef struct
{
	BOOLEAN  is_als;  
} CPHS_TELESERV_T;

typedef struct
{
	BOOLEAN  is_reserved;  
}CPHS_FEATURES_T;


typedef struct
{
	BOOLEAN  is_clip;  
    BOOLEAN  is_colr;
    BOOLEAN  is_colp;
    BOOLEAN  is_mci;
    BOOLEAN  is_cli_send;
    BOOLEAN  is_cli_block;
}NUMBER_IDENTIF_T;


typedef struct
{
	BOOLEAN  is_gprs;  
    BOOLEAN  is_hscsd;
    BOOLEAN  is_vgc;
    BOOLEAN  is_vbs;
    BOOLEAN  is_msp;
    BOOLEAN  is_multi_band;
}PHASE_SERV_T;

typedef struct
{
    BOOLEAN  is_plmn_mode;  
    BOOLEAN  is_vps;
    BOOLEAN  is_sm_mo_paging;
    BOOLEAN  is_sm_mo_email;
    BOOLEAN  is_fax;
    BOOLEAN  is_data;
    BOOLEAN  reserved;
    BOOLEAN  is_language;
}VALUE_ADDED_SERV_T;

typedef struct
{
    BOOLEAN is_call_offering_present;
    CALL_OFFERING_T  call_offering;
    BOOLEAN is_call_restriction_present;
    CALL_RESTRICTION_T  call_restriction;
    OTHER_SUPP_SERV_T  other_supp_serv;
    CALL_COMPLETION_T  call_completion;
    TELESERVICES_T  teleservices;
    BOOLEAN is_cphs_teleserv_present;
    CPHS_TELESERV_T  cphs_teleserv;
    CPHS_FEATURES_T  cphs_feature;
    NUMBER_IDENTIF_T  number_identif;
    PHASE_SERV_T  phase_service;
    BOOLEAN  is_value_added_serv_present;
    BOOLEAN  is_info_numbers_present;
    VALUE_ADDED_SERV_T value_added_service;
    uint8  info_numbers;
}SIM_EF_MAPPING_CSP_T;

typedef struct
{
    BOOLEAN is_datafax_flag_present;
	uint8	line1_status;
    uint8	line2_status;
    uint8	fax_status;
    uint8	data_status;
}SIM_EF_MAPPING_VMWFLAG_T;

typedef struct
{
    uint8 alpha_id_len;		
    uint8 mailbox_number_len;	            
    uint8 ccp_id;  
    BOOLEAN is_ext_exist;   //the Extension exist or not
    uint8   ext_id;         //the Extension1 record identifier                      
    BOOLEAN is_ccp_exist;       
    BOOLEAN is_ton_npi_exist;   
    uint8 alpha_id[SIM_MAX_DN_ALPHA_IDENTIFY_LEN];		
    uint8 mailbox_number[SIM_EF_DN_DN_LEN];	  
    NUMBER_TYPE_E   ton;     
    NUMBER_PLAN_E   npi;      
    //SIM_EF_MAPPING_CCP_T ccp;	
}SIM_EF_MAPPING_MBNUM_T;

typedef struct
{
    uint8 alpha_id_len;	
    uint8 number_len;
    uint8 information_digits_number_len;
    uint8 entry_status;
    BOOLEAN is_ton_npi_exist;   
    BOOLEAN is_ext_exist;   //the Extension exist or not
    uint8   ext_id;         //the Extension1 record identifier                      
    uint8 alpha_id[SIM_MAX_DN_ALPHA_IDENTIFY_LEN];		
    uint8 information_digits_number[SIM_MAX_DN_ALPHA_IDENTIFY_LEN];	  
    NUMBER_TYPE_E   ton;     
    NUMBER_PLAN_E   npi;      
}SIM_EF_MAPPING_INFONUM_T;

typedef enum
{
    SIM_CALL_OFFERING = 0x01,
    SIM_CALL_RESTRICTION,
    SIM_OTHER_SERVICES,
    SIM_CALL_COMPLETION,
    SIM_TELESERVICES,
    SIM_CPHS_TELE,
    SIM_CPHS_FEATURES,
    SIM_NUM_ID,
    SIM_PHASE2_SERVICE,
    SIM_VALUEADDED_SERV = 0xC0,
    SIM_SPID = 0xC1,
    SIM_INFO_NUM = 0xD5,
    SIM_CSP_TYPE_NUM
}SIM_CSP_TYPE_E;

//the phase 2 service type,used to index the corresponding service
//information in the EF SST mapping.
typedef enum
{
	SIMSER_PH2_CHV1_DISABLE,
	SIMSER_PH2_ADN,
	SIMSER_PH2_FDN,
	SIMSER_PH2_SMS,
	SIMSER_PH2_AOC,
	SIMSER_PH2_CCP,
	SIMSER_PH2_PLMN_SEL,
	SIMSER_PH2_RFU1,
	SIMSER_PH2_MSISDN,
	SIMSER_PH2_EXT1,
	SIMSER_PH2_EXT2,
	SIMSER_PH2_SMSP,
	SIMSER_PH2_LND,
	SIMSER_PH2_CBMI,    // Cell Broadcast Message Identifier
	SIMSER_PH2_GID1,
	SIMSER_PH2_GID2,
	SIMSER_PH2_SPN,
	SIMSER_PH2_SDN,
	SIMSER_PH2_EXT3,
	SIMSER_PH2_RFU2,
	SIMSER_PH2_VGCS,
	SIMSER_PH2_VBS,
	SIMSER_PH2_EMLPP,
	SIMSER_PH2_AAEM,
	SIMSER_PH2_SMS_CB_DL,
	SIMSER_PH2_SMS_PP_DL,
	SIMSER_PH2_MENU_SEL,
	SIMSER_PH2_CALL_CON,
	SIMSER_PH2_PROACTIVE_SIM,
	SIMSER_PH2_CBMIR,
	SIMSER_PH2_BDN,
	SIMSER_PH2_EXT4,
	SIMSER_PH2_DCK,
	SIMSER_PH2_CO_NL,
	SIMSER_PH2_SMS_STATUS_REPORT,
	SIMSER_PH2_NIA,
	SIMSER_PH2_SIM_CON_MO_SMS,
	SIMSER_PH2_GPRS,
	SIMSER_PH2_IMAGE,
	SIMSER_PH2_SOLSA,
	SIMSER_PH2_USSD_IN_CC,
	SIMSER_PH2_RUN_AT_COMMAND,
	SIMSER_PH2_PLMN_WACT,
	SIMSER_PH2_OPLMN_WACT,
	SIMSER_PH2_HPLMN_WACT,
	SIMSER_PH2_CPBCCH,
	SIMSER_PH2_INV_SCAN,
	SIMSER_PH2_EXT_CCP,
	SIMSER_PH2_MEXE,
	SIMSER_PH2_RESERVED,
	SIMSER_PH2_PNN,
	SIMSER_PH2_OPL,
	SIMSER_PH2_MDN,
	SIMSER_PH2_MWIS,
	SIMSER_PH2_CFIS,
	SIMSER_PH2_SPDI,
	SIMSER_PH2_MMS,
	SIMSER_PH2_EXT8,
	SIMSER_PH2_MMSUCP,
	SIMSER_PH2_2G_SST_END_NUM,
	SIMSER_PH2_LPB,//USIM Local PB
	SIMSER_PH2_OCI,//USIM Outgoing Call Information (OCI and OCT)
	SIMSER_PH2_ICI,//USIM Incoming Call Information (ICI and ICT)
	SIMSER_PH2_CCP2,//USIM Capability Configuration Parameters 2 (CCP2)
	SIMSER_PH2_GSM_ACCESS,//USIM GSM Access
	SIMSER_PH2_EST,//USIM Enabled Services Table
	SIMSER_PH2_ACL,//USIM APN Control List (ACL)
	SIMSER_PH2_GSM_SEC_CONTEXT,//USIM GSM security context
	SIMSER_PH2_EXT5,//USIM Extension 5
	SIMSER_PH2_CC_GPRS_BY_USIM,//USIM Call control on GPRS by USIM
	SIMSER_PH2_PSEUDONYM,//USIM  Pseudonym
	SIMSER_PH2_UPLMN_WLAN,//USIM User Controlled PLMN selector for I-WLAN access
	SIMSER_PH2_OPLMN_WLAN,//USIM Operator Controlled PLMN selector for I-WLAN access
	SIMSER_PH2_UC_WSID,//User controlled WSID list
	SIMSER_PH2_OC_WSID,//Operator controlled WSID list
	SIMSER_PH2_VGCS_SEC,//USIM VGCS security
	SIMSER_PH2_VBS_SEC,//USIM VBS security
	SIMSER_PH2_WLAN_REAUTH_ID,//USIM WLAN Reauthentication Identity
	SIMSER_PH2_MMS_STORAGE,//USIM Multimedia Messages Storage
	SIMSER_PH2_GBA,//USIM Generic Bootstrapping Architecture (GBA)
	SIMSER_PH2_MBMS_SEC,//USIM MBMS security
	SIMSER_PH2_DL_USSD,//USIM Data download via USSD and USSD application mode
	SIMSER_PH2_EHPLMN,//USIM Equivalent HPLMN
	SIMSER_PH2_ADD_TERMINAL_PROFILE,//USIM Additional TERMINAL PROFILE after UICC activation
	SIMSER_PH2_EHPLMNPI,//USIM Equivalent HPLMN Presentation Indication
	SIMSER_PH2_LAST_RPLMN_SI,//USIM Last RPLMN Selection Indication
	SIMSER_PH2_OMA_BCAST_PROFILE,//USIM OMA BCAST Smart Card Profile
	SIMSER_PH2_GBA_LOCAL_KEY,//USIM GBA-based Local Key Establishment Mechanism
	SIMSER_PH2_TERMINAL_APP,//USIM Terminal Applications
    SIMSER_PH2_SPN_ICON,    //USIM Service Provider Name Icon
    SIMSER_PH2_PNN_ICON,    //USIM PLMN Network Name Icon
	SIMSER_PH2_CP_USIM_IP,//USIM Connectivity Parameters for USIM IP connections
	SIMSER_PH2_HIWLAN_ID_LIST,//USIM Home I-WLAN Specific Identifier List
	SIMSER_PH2_IWLAN_EHPLMN,//USIM I-WLAN Equivalent HPLMN Presentation Indication
	SIMSER_PH2_IWLAN_HPLMN_PRIORITY,//USIM I-WLAN HPLMN Priority Indication
	SIMSER_PH2_IWLAN_LAST_RPLMN,//USIM I-WLAN Last Registered PLMN
	SIMSER_PH2_EPS_MM,//USIM EPS Mobility Management Information
	SIMSER_PH2_CSG_INDICATION,//USIM Allowed CSG Lists and corresponding indications
	SIMSER_PH2_CC_EPS_BY_USIM,//USIM Call control on EPS PDN connection by USIM
	SIMSER_PH2_HPLMN_DA,//USIM HPLMN Direct Access
	SIMSER_PH2_ECALL_DATA,//USIM eCall Data
	SIMSER_PH2_INFONUM,     //CPHS Information Numbers
	SIMSER_PH2_NUM
}SIM_SERVICE_TYPE_PH_2_E;

//the phase 1 service index of service information table
typedef enum
{
	SIMSER_PH1_CHV1_DISABLE,
	SIMSER_PH1_ADN,
	SIMSER_PH1_FDN,
	SIMSER_PH1_SMS,
	SIMSER_PH1_AOC,
	SIMSER_PH1_CCP,
	SIMSER_PH1_PLMN_SEL,
	SIMSER_PH1_CPS,
	SIMSER_PH1_NUM
}SIM_SERVICE_TYPE_PH_1_E;

typedef SIM_SERVICE_INFO_T SIM_SERVICE_INFO_ARR[SIMSER_PH2_NUM];
typedef struct
{
	uint8 service_num;
	SIM_SERVICE_INFO_ARR	service_table;
}SIM_EF_MAPPING_SST_T;

typedef struct
{
	uint32 acm_value;
}SIM_EF_MAPPING_ACM_T;

typedef struct
{
    uint8   len;             //the really length of the gid
	uint8	gid[SIM_MAX_GID_LEN];
}SIM_EF_MAPPING_GID_T;

typedef struct SIM_EF_MAPPING_SPN_T_s
{
    uint8   display_condition;
    uint8   spn[SIM_EF_SPN_SPN_LEN];
} SIM_EF_MAPPING_SPN_T;


typedef struct
{
	uint8	cur_code[3];
}SIM_CURRENCY_CODE_T;

typedef struct
{
	BOOLEAN ex_sign;		//the sign of ex,TRUE is positive
							//FALSE is negative
	uint8	ex;
	uint16	eppu;
}SIM_EPPU_T;

typedef struct
{
	SIM_CURRENCY_CODE_T cur_code;
	SIM_EPPU_T eppu;
}SIM_EF_MAPPING_PUCT_T;

typedef struct
{
	uint8 id[2];
}SIM_CBMI_T;

typedef SIM_CBMI_T SIM_CBMI_ARR[SIM_MAX_VALIDATE_CBMI_NUM];
typedef struct
{
	uint8 validate_num;//wuding.yang cr99770. 2007.11.13
	SIM_CBMI_ARR cbmis;
}SIM_EF_MAPPING_CBMI_T;


typedef struct
{
	TMSI_T	tmsi;
	LAI_T	lai;
	T3212_VALUE	tmsi_time;
	MM_UPDATE_STATUS_E update_status;
}SIM_EF_MAPPING_LOCI_T;

typedef enum
{
	SIM_MS_NORMAL_OPERATION = 0x00,
	SIM_MS_TYPE_APPROVAL_OPERATION	 = 0x80,
	SIM_MS_NORMAL_SPECIFIC_FACILITIES = 0x01,
	SIM_MS_TYPE_APPROVAL_SPECIFIC_FACILITIES = 0x81,
	SIM_MS_MAINTENANCE	= 0x02,
	SIM_CELL_TEST	= 0x04,
	SIM_MS_MODE_RFU = 0xFF
}SIM_MS_OPERATION_MODE_E;

typedef struct
{
    BOOLEAN is_ad_exist;
    SIM_MS_OPERATION_MODE_E operation_mode;
    BOOLEAN                 ofm_actived;    //TRUE      OFM to be activated by the ME
                                            //FALSE     OFM to be disabled by the ME 
    BOOLEAN                 mnc_len_exist;  //TRUE      "length of MNC in the IMSI" exist
    uint8                   mnc_digit_num;  //default value is 2 digits
} SIM_EF_MAPPING_AD_T;


typedef struct
{
	SIM_PHASE_E phase;
}SIM_EF_MAPPING_PHASE_T;


typedef struct
{
	uint8 vgi_id[SIM_EF_VGCS_GID_SIZE];
}SIM_VGI_T;

typedef SIM_VGI_T   SIM_VGI_ARR[SIM_EF_VGCS_VGI_NUM];
typedef struct
{
	uint8 validate_num;
	SIM_VGI_ARR	bgis;
}SIM_EF_MAPPING_VGCS_T;

typedef struct
{
	BOOLEAN	actived[SIM_EF_VGCS_VGI_NUM];
}SIM_EF_MAPPING_VGCSS_T;

typedef struct
{
	uint8 vbi_id[SIM_EF_VBS_GID_SIZE];
}SIM_VBI_T;

typedef SIM_VBI_T   SIM_VBI_ARR[SIM_EF_VBS_VBI_NUM];
typedef struct
{
	uint8 validate_num;
	SIM_VBI_ARR vbis;
}SIM_EF_MAPPING_VBS_T;

typedef struct
{
	BOOLEAN avtived[SIM_EF_VBS_VBI_NUM];
}SIM_EF_MAPPING_VBSS_T;

typedef struct
{
	BOOLEAN	priority_l_A;
	BOOLEAN	priority_l_B;
	BOOLEAN	priority_l_0;
	BOOLEAN	priority_l_1;
	BOOLEAN	priority_l_2;
	BOOLEAN	priority_l_3;
	BOOLEAN	priority_l_4;
}SIM_PRIORITY_LEVEL_T;

typedef struct
{
	SIM_PRIORITY_LEVEL_T	priority_level;
	SIM_PRIORITY_LEVEL_T	fsc_condition;
}SIM_EF_MAPPING_EMLPP_T;

typedef struct
{
	SIM_PRIORITY_LEVEL_T aa_priority_level;
}SIM_EF_MAPPING_AAEM_T;

typedef struct
{
	uint8 validate_num;
	SIM_CBMI_ARR	cbmi_for_dowload;
}SIM_EF_MAPPING_CBMID_T;

typedef struct
{
	uint8 ecc_code[3];
	BOOLEAN alpha_id_flag;
	uint8 alpha_id_len;
	uint8 alpha_id[SIM_MAX_ECC_ALPHA_ID_LEN];
	BOOLEAN ecc_category_flag;
	uint8 ecc_category;
}SIM_ECC_T;

typedef SIM_ECC_T   SIM_ECC_ARR[SIM_MAX_ECC_NUM];
typedef struct
{
	uint8 validate_num;
	SIM_ECC_ARR call_codes;
}SIM_EF_MAPPING_ECC_T;

typedef struct
{
	SIM_CBMI_T	low_cbmi;
	SIM_CBMI_T	up_cbmi;
}SIM_CBMI_T_RANGE_T;

typedef SIM_CBMI_T_RANGE_T  SIM_CBMI_T_RANGE_ARR[SIM_MAX_CBMIR_RANGE_NUM];
typedef struct
{
	uint8 validate_num;
	SIM_CBMI_T_RANGE_ARR	cbmi_ranges;
}SIM_EF_MAPPING_CBMIR_T;

typedef struct
{
	BOOLEAN b_network_dck;
	BOOLEAN b_network_sub_dck;
	BOOLEAN b_sp_dck;
	BOOLEAN b_corperate_dck;
	uint8 network_dck[4];
	uint8 network_sub_dck[4];
	uint8 sp_dck[4];
	uint8 corperate_dck[4];
}SIM_EF_MAPPING_DCK_T;

typedef struct
{
	bcd list[6];
}SIM_NET_LIST_T;

typedef SIM_NET_LIST_T  SIM_NET_LIST_ARR[SIM_MAX_CO_OPERATIVE_NET_LIST_NUM];
typedef struct
{
	uint8 validate_num;
	SIM_NET_LIST_ARR net_lists;
}SIM_EF_MAPPING_CNL_T;

// add by fancier temporily ???
#define SMS_SIM_STATUS_REPORT_LEN		29
typedef uint8 SMS_SIM_STATUS_REPORT_ARR_T[SMS_SIM_STATUS_REPORT_LEN];

typedef struct
{
	uint8 sm_record_id;						//the record id of short message that the status report blong to in EF_SMS
	SMS_SIM_STATUS_REPORT_ARR_T status_report;
}SIM_EF_MAPPING_SMSR_T;


typedef enum
{
    SIM_EXT_RECORD_UNKNOW = 0x0,          //the unknow Ext record type
    SIM_EXT_RECORD_CPS    = 0x01,         //the called party subaddress
    SIM_EXT_RECORD_ADDTIONAL_DATA = 0x02  //the Addtional data
}SIM_EXT_RECORD_TYPE_E;

typedef struct
{
    SIM_EXT_RECORD_TYPE_E record_type;
	uint8   extension_data_len;                 //CR26616,add
    uint8   extension_data[SIM_EF_EXT_DATA_LEN];
    BOOLEAN more_data;
    uint8   next_record_id;
}SIM_EF_MAPPING_EXT_T;

typedef struct
{
	CKSN_T	cksn;	//the Ciphering Key Sequenece number
	KC_T	kc;		//the Ciphering key kc
}SIM_EF_MAPPING_KC_T;

typedef struct
{
	uint8 alerting_cat;
	uint8 iformative_text_len;
	uint8 iformative_text[SIM_EF_NIA_MAX_INFORMATIVE_TEXT_LEN];
}SIM_EF_MAPPING_NIA_T;

typedef struct
{
	GMM_CKSN_E cksn;		//Ciphering key sequence number
	GPRS_KC_ARR_T kc;	//Ciphering Key Kc
}SIM_EF_MAPPING_KC_GPRS_T;

typedef struct
{
	uint8 tp_mr;
	BOOLEAN	mem_exeed;
}SIM_EF_MAPPING_SMSS_T;

typedef enum
{
	SIM_R_UPDATED			= 0x00,
	SIM_R_NOT_UPDATED		= 0x01,
	SIM_R_PLMN_NOT_ALLOWED	= 0x02,
	SIM_R_ROUTING_AREA_NOT_ALLOWED	= 0x03,
	SIM_R_RESERVED = 0x07
}SIM_R_UPDATE_STATUS_E;

typedef struct
{
	P_TMSI_T	p_tmsi;
	P_TMSI_SIGNATURE_T	p_tmsi_signature;
	RAI_T	rai;
	SIM_R_UPDATE_STATUS_E update_status;
}SIM_EF_MAPPING_LOCI_GPRS_T;

typedef struct
{
	uint8 alpha_id_tag;
	uint8 alpha_id_len;
	uint8 alpha_id[SIM_EF_SUME_MAX_ALPHA_ID_LEN];
	BOOLEAN icon_flag;
	uint8 icon_id_tag;
	uint8 icon_id_len;
	uint8 icon_qualifier;
	uint8 icon_id;
}SIM_EF_MAPPING_SUME_T;

typedef struct
{
	NETWORK_NAME_T full_nt_name;
	BOOLEAN is_short_nt_name_flag;
	NETWORK_NAME_T short_nt_name;
}SIM_EF_MAPPING_PNN_T;
#define SIM_FULL_NETWORK_NAME_IEI	0x43
#define SIM_SHORT_NETWORK_NAME_IEI	0x45

typedef struct
{
	PLMN_T plmn;
	LAC_T lac_1;
	LAC_T lac_2;
	uint8 pnn_index;
}SIM_EF_MAPPING_OPL_T;

#define SIM_SP_DISPLAY_TAG		0xA3
#define SIM_SP_PLMN_LIST_TAG	0x80
typedef struct SPDI_PLMN_T_s {
	BOOLEAN present;
	PLMN_T plmn;
} SPDI_PLMN_T;

// It shall be possible to associate at least 10 PLMN Identifications (MCC+MNC combination)
// with the same SP Name.
#define SIM_SPDI_PLMN_MAX_NUM   10
typedef struct
{
    uint8       num;
    SPDI_PLMN_T plmn_arr[SIM_SPDI_PLMN_MAX_NUM];
}SIM_EF_MAPPING_SPDI_T;


typedef struct
{
	BOOLEAN lsa_only_access;
	uint8 lsa_lsa_only_access_len;
	uint8 lsa_only_access_text[SIM_EF_SAI_LSA_TEXT_MAX_LEN];
}SIM_EF_MAPPING_SAI_T;

typedef struct
{
	uint8 	lsa_name_len;
	uint8		lsa_name[SIM_EF_SLL_LSA_NAME_MAX_LEN];
	uint8		icon_qualifier;
	BOOLEAN 	idle_mode_support;
	BOOLEAN	lsa_indication_idle_mode;
	uint8		rfu;
	uint8		icon_identifier;
	uint8		priority;
	PLMN_T	plmn;
	uint16	lsa_file_id;
	uint8		lsa_rec_number;
}SIM_EF_MAPPING_SLL_T;

typedef struct
{
	SIM_EF_MAPPING_SLL_T sll_mapping;
}SIM_DF_MAPPING_SOLSA_T;

typedef struct
{
	uint8 width;
	uint8 height;
	uint8 coding_scheme;
	uint16 fileid_of_data;
	uint16 offset_of_data;
	uint16 len_of_data;
}SIM_IMG_INSTANCE;

typedef SIM_IMG_INSTANCE SIM_IMG_INSTANCE_ARR[SIM_EF_IMG_INSTANCE_NUM];

typedef struct
{
	uint8 num;
	SIM_IMG_INSTANCE_ARR descriptor;
	BOOLEAN rfu_flag;
	uint8 rfu;
}SIM_EF_MAPPING_IMG_T;

//the mapping structure of dialling number
typedef struct
{
    uint8   alpha_id_len;   //the alpha id length of the dialling number
    uint8   dn_len;         //the dialling number length
    BOOLEAN is_ton_npi_exist;   //is the TON and NPI exist
    BOOLEAN is_ccp_exist;   //the Capability/Configuration exist or not
    uint8   ccp_id;         //the Capability/Configuration Identifier
    BOOLEAN is_ext_exist;   //the Extension exist or not
    uint8   ext_id;         //the Extension1 record identifier
    uint8   alpha_id[SIM_MAX_DN_ALPHA_IDENTIFY_LEN];    //the alpha id of the dialling number
    uint8   dialling_num[SIM_EF_DN_DN_LEN];    //the dialling number
    NUMBER_TYPE_E   ton;    //the type of the number(TON)
    NUMBER_PLAN_E   npi;    //the numbering plan identification
}SIM_EF_MAPPING_DN_T;

typedef struct
{
	uint8	bc_info_len;
	uint8	bc_info[SIM_EF_ECCP_MAX_LEN];
}SIM_EF_MAPPING_ECCP_T;

typedef struct
{
	uint8 alpha_id_len;
	uint8 alpha_id[SIM_MAX_DN_ALPHA_IDENTIFY_LEN];
	uint8 cmi;
}SIM_EF_MAPPING_CMI_T;

typedef struct
{
	PLMN_T plmn;
	BOOLEAN	 e_utran_flag;
	BOOLEAN utran_flag;
	BOOLEAN	 cdma2000_1xrtt_flag;
	BOOLEAN cdma2000_hrpd_flag;
	BOOLEAN	gsm_flag;
	BOOLEAN compact_gsm_flag;
}SIM_PLMN_ACCESS_TECH_T;

typedef SIM_PLMN_ACCESS_TECH_T  SIM_PLMN_ACCESS_ARR[SIM_EF_PLMNWACT_ARR_NUM];
typedef struct
{
    uint8               max_num;
    uint8               validate_num;
    SIM_PLMN_ACCESS_ARR plmn;
} SIM_EF_MAPPING_PLMNWACT_T;

typedef SIM_PLMN_ACCESS_TECH_T  SIM_OPLMN_ACCESS_ARR[SIM_EF_OPLMNWACT_ARR_NUM];
typedef struct
{
    uint8                max_num;
    uint8                validate_num;
    SIM_OPLMN_ACCESS_ARR plmn;
}SIM_EF_MAPPING_OPLMNWACT_T;

typedef SIM_PLMN_ACCESS_TECH_T  SIM_HPLMN_ACCESS_ARR[SIM_EF_HPLMNWACT_ARR_NUM];
typedef struct
{
	uint8 validate_num;
	SIM_HPLMN_ACCESS_ARR plmn;
}SIM_EF_MAPPING_HPLMNWACT_T;

typedef struct
{
	uint8 cpbcch[SIM_EF_CPBCCH_ELEMENT_SIZE];
}SIM_ELEMENT_CPBCCH_T;

typedef SIM_ELEMENT_CPBCCH_T  SIM_CPBCCH_ARR[SIM_EF_CPBCCH_ARR_NUM];
typedef struct
{
	uint8 validate_num;
	SIM_CPBCCH_ARR cpbcch;
}SIM_EF_MAPPING_CPBCCH_T;

typedef struct
{
	uint8 invscan_flag;
}SIM_EF_MAPPING_INVSCAN_T;

typedef enum
{
	SIMSER_MEXE_ORPK,
	SIMSER_MEXE_ARPK,
	SIMSER_MEXE_TPRPK,
	SIMSER_MEXE_RFU,
	SIMSER_MEXE_NUM
}SIM_MEXE_SERVICE_TYPE_PH_2_E;
typedef SIM_SERVICE_INFO_T SIM_MEXE_SERVICE_INFO_ARR[SIMSER_MEXE_NUM];
typedef struct
{
	uint8 service_num;
	SIM_MEXE_SERVICE_INFO_ARR	service_table;
}SIM_EF_MAPPING_MEXEST_T;

typedef struct
{
	uint8	para_indicator;
	uint8	flags;
	uint8	type;
	uint16	key_file_id;
	uint16	offset_into_file;
	uint16	data_len;
	uint8	key_len;
	uint8      key_id[SIM_EF_ORPK_KEYID_MAX_LEN];
}SIM_EF_MAPPING_ORPK_T;

typedef struct
{
	uint8	para_indicator;
	uint8	flags;
	uint8	type;
	uint16	key_file_id;
	uint16	offset_into_file;
	uint16	data_len;
	uint8	key_len;
	uint8      key_id[SIM_EF_ARPK_KEYID_MAX_LEN];
}SIM_EF_MAPPING_ARPK_T;

typedef struct
{
	uint8	para_indicator;
	uint8	flags;
	uint8	type;
	uint16	key_file_id;
	uint16	offset_into_file;
	uint16	data_len;
	uint8	key_len;
	uint8      key_id[SIM_EF_TPRPK_KEYID_MAX_LEN];
	uint8	cer_len;
	uint8	cer_id[SIM_EF_TPRPK_CERID_MAX_LEN];
}SIM_EF_MAPPING_TPRPK_T;

typedef PLMN_T SIM_PLMN_SEL_ARR[SIM_EF_PLMNSEL_ARR_NUM];
typedef struct
{
	uint8 validate_num;
	uint8 max_num;
	SIM_PLMN_SEL_ARR plmn_arr;
}SIM_EF_MAPPING_PLMNSEL_T;

typedef struct
{
    uint8 imsi_m[SIM_EF_CDMA_IMSI_M_LEN];
}SIM_EF_MAPPING_CDMA_IMSI_M_T;

typedef struct
{
	uint8 adf_tag;
	uint8 adf_len;
	uint8 aid_tag;
	uint8 aid_len;
	uint8 aid_value[16];
	//donot decode the Optional
}SIM_EF_MAPPING_DIR_T;

typedef struct
{
	uint8 value[255];
}SIM_EF_MAPPING_ARR_T;

typedef struct
{
	BOOLEAN is_FDN_enable;
	BOOLEAN is_BDN_enable;
}SIM_EF_MAPPING_EST_T;

typedef struct SIM_EF_MAPPING_EHPLMN_T_s
{
    uint8   validate_num;
    uint8   max_num;
    PLMN_T  plmn_arr[SIM_EF_EHPLMN_ARR_NUM];
} SIM_EF_MAPPING_EHPLMN_T;

typedef struct SIM_EF_MAPPING_EHPLMNPI_T_s
{
    uint8   pi_value;
} SIM_EF_MAPPING_EHPLMNPI_T;

//Rel-8
#define SIM_ICON_MAX_LEN     250
typedef union SIM_ICON_U_s
{
    uint8      uri_arr[SIM_ICON_MAX_LEN];
    uint8   img_id_arr[SIM_ICON_MAX_LEN];
} SIM_ICON_U;

typedef struct SIM_ICON_LINK_s
{
    uint8       len;
    SIM_ICON_U  icon;
} SIM_ICON_LINK_T;

typedef struct SIM_ICON_INFO_T_s
{
    uint8           icon_tag;   //0x80 means URI; 0x81 means img
    uint8           qualifier;
    SIM_ICON_LINK_T icon_link;
} SIM_ICON_INFO_T, SIM_EF_MAPPING_SPNI_T, SIM_EF_MAPPING_PNNI_T;

typedef struct
{
    uint16 adn_file_id;
    uint16 ext1_file_id;
    uint16 iap_file_id;
    uint16 pbc_file_id;
    uint16 grp_file_id;
    uint16 aas_file_id;
    uint16 gas_file_id;
    uint16 anra_file_id;
    uint16 anrb_file_id;
    uint16 anrc_file_id;
    uint16 sne_file_id;
    uint16 ccp1_file_id;
    uint16 uid_file_id;
    uint16 emaila_file_id;
    uint16 emailb_file_id;
}SIM_EF_MAPPING_PBR_T;

typedef struct
{
    BOOLEAN is_als_locked;
}SIM_EF_MAPPING_ORANGE_DYN2_FLAG_T;

typedef struct
{
    BOOLEAN is_call_offering_present;
    CALL_OFFERING_T  call_offering;
    BOOLEAN is_call_restriction_present;
    CALL_RESTRICTION_T  call_restriction;
    OTHER_SUPP_SERV_T  other_supp_serv;
    CALL_COMPLETION_T  call_completion;
    TELESERVICES_T  teleservices;
    CPHS_TELESERV_T  rfu;
    CPHS_FEATURES_T  cphs_feature;
    NUMBER_IDENTIF_T  number_identif;
    PHASE_SERV_T  phase_service;
    BOOLEAN  is_value_added_serv_present;
    BOOLEAN  is_info_numbers_present;
    VALUE_ADDED_SERV_T value_added_service;
    uint8  info_numbers;
    BOOLEAN is_spid_present;
    uint8 spid;
}SIM_EF_MAPPING_ORANGE_CSP_LINE2_T;

typedef struct
{
    uint16 mcc;
    uint16 mnc;
    uint8 imsi_m_data[10];
}SIM_EF_MAPPING_IMSI_M_T;

typedef struct
{
    uint8 selected_line;
}SIM_EF_MAPPING_ORANGE_DYN_FLAG_T;

typedef struct
{
    uint8 voicemail_index;
    uint8 fax_index;
    uint8 email_index;
    uint8 other_index;
}SIM_EF_MAPPING_MBI_T;

typedef struct
{
    uint8 msp_id;
    BOOLEAN voice_callfrd_uncond;
    BOOLEAN fax_callfrd_uncond;
    BOOLEAN data_callfrd_uncond;
}SIM_EF_MAPPING_CFIS_T;

typedef struct
{
    uint8 vm_waiting_count;
    uint8 fax_waiting_count;
    uint8 data_waiting_count;
    uint8 other_waiting_count;
}SIM_EF_MAPPING_MWIS_T;

typedef struct
{
    BOOLEAN is_datafax_flag_present;
    uint8 line1_waiting_count;
    uint8 line2_waiting_count;
    uint8 fax_waiting_count;
    uint8 data_waiting_count;
    uint8 other_waiting_count;
}SIM_VMW_IND_FLAG_T;

typedef struct
{
    uint8 ksi;
    uint8 ck[16];
    uint8 ik[16];
}SIM_EF_MAPPING_KEYS_T;

typedef struct
{
    uint8 ksips;
    uint8 ckps[16];
    uint8 ikps[16];
}SIM_EF_MAPPING_KEYS_PS_T;

typedef struct
{
    ARFCN_T         camponed_arfcn;
    uint8           arfcn_num;
    uint16          arfcn[SIM_MAX_ARFCN_NUM]; //used in 3G sim read cnf;
}USIM_NETPAR_T;

typedef struct
{
    BOOLEAN         is_usim;
    BA_LIST_ARR_T   ba_list;
    USIM_NETPAR_T   usim_netpar;
}SIM_EF_MAPPING_BCCH_T;

#define SIM_MAX_APN_TLV_LEN 200
typedef struct
{
	uint8 apn_num;
	uint8 apn_tlvs_len;
	uint8 apn_tlvs[SIM_MAX_APN_TLV_LEN];
}SIM_EF_MAPPING_ACL_T;

//the union structure that can storage ALL EF file structure
//and used in read and update EF file signal
typedef union SIM_FILE_STRUCTURE_BUF_U
{
	uint8					uint8_value;
	uint16					uint16_value;
	uint32					uint32_value;
	SIM_EF_MAPPING_AAEM_T		aaem;
	SIM_EF_MAPPING_ACM_T		acm;
	SIM_EF_MAPPING_ACMMAX_T	 acm_max;
	SIM_EF_MAPPING_AD_T		    ad;
	SIM_EF_MAPPING_CBMI_T		cbmi;
	SIM_EF_MAPPING_CBMID_T	    cbmid;
	SIM_EF_MAPPING_CBMIR_T	cbmir;
	SIM_EF_MAPPING_CNL_T		cnl;
	SIM_EF_MAPPING_DCK_T		dck;
	SIM_EF_MAPPING_ECC_T		ecc;
	SIM_EF_MAPPING_EMLPP_T	    emlpp;
	SIM_EF_MAPPING_GID_T		gid;
	SIM_EF_MAPPING_ICCID_T	    iccid;
	SIM_EF_MAPPING_IMSI_T		imsi;
	SIM_EF_MAPPING_KC_T		    kc;
	SIM_EF_MAPPING_KC_GPRS_T	kc_gprs;
	SIM_EF_MAPPING_LOCI_T		loci;
	SIM_EF_MAPPING_LOCI_GPRS_T	loci_gprs;
	SIM_EF_MAPPING_ELP_T        elp;
	SIM_EF_MAPPING_LP_T        lp;
	SIM_EF_MAPPING_LP_UNION_T   language;
	SIM_EF_MAPPING_NIA_T		nia;
	SIM_EF_MAPPING_PHASE_T	    phase;
	SIM_EF_MAPPING_PUCT_T		puct;
	SIM_EF_MAPPING_PNN_T		pnn;
	SIM_EF_MAPPING_OPL_T		opl;
	SIM_EF_MAPPING_SPDI_T		spdi;
	SIM_EF_MAPPING_SAI_T		sai;
	SIM_EF_MAPPING_SLL_T		sll;
	SIM_EF_MAPPING_SPN_T		spn;
	SIM_EF_MAPPING_SST_T		sst;
	SIM_EF_MAPPING_SUME_T		sume;
	SIM_EF_MAPPING_VBS_T		vbs;
	SIM_EF_MAPPING_VBSS_T		vbss;
	SIM_EF_MAPPING_VGCS_T		vgcs;
	SIM_EF_MAPPING_VGCSS_T	    vgcss;
	SIM_EF_MAPPING_CPHSINFO_T	cphsinfo;
	SIM_EF_MAPPING_ONSTRING_T	onstring;
	SIM_EF_MAPPING_ONSHORT_T	onshort;
	SIM_EF_MAPPING_VMWFLAG_T	vmwflag;
	SIM_EF_MAPPING_CFFLAG_T	    cfflag;
	SIM_EF_MAPPING_CSP_T		csp;
	SIM_EF_MAPPING_MBNUM_T	    mbnum;
	/*the next item is new added by king.li for CPHS extended funtions*/
       SIM_EF_MAPPING_INFONUM_T     infonum;
	FPLMN_LIST_T			    fplmn;
	SIM_EF_MAPPING_PLMNSEL_T		plmn_sel;
	SIM_EF_MAPPING_BCCH_T			bcch;
	ACC_CONTROL_CLASS_T		    acc;
	HPLMN_TIMER_VALUE_T		    hplmn;
	SIM_EF_MAPPING_DN_T			fdn;
//SIM_RELEASE99_UPDATE
	SIM_EF_MAPPING_CMI_T		cmi;
	SIM_EF_MAPPING_PLMNWACT_T	plmnwact;
	SIM_EF_MAPPING_OPLMNWACT_T	oplmnwact;
	SIM_EF_MAPPING_HPLMNWACT_T	hplmnwact;
	SIM_EF_MAPPING_CPBCCH_T	cpbcch;
	SIM_EF_MAPPING_INVSCAN_T	invscan;
	SIM_EF_MAPPING_MEXEST_T	mexest;
	SIM_EF_MAPPING_ORPK_T		orpk;
	SIM_EF_MAPPING_ARPK_T		arpk;
	SIM_EF_MAPPING_TPRPK_T		tprpk;
//SIM_RELEASE99_UPDATE
	SIM_EF_MAPPING_DIR_T		dir;//
	SIM_EF_MAPPING_ARR_T mf_arr;
	SIM_EF_MAPPING_EST_T est;
	SIM_EF_MAPPING_PBR_T pbr;
	SIM_EF_MAPPING_CDMA_IMSI_M_T cdmaimsi;
	// Rel-7
    SIM_EF_MAPPING_EHPLMN_T     ehplmn;
    SIM_EF_MAPPING_EHPLMNPI_T   ehplmnpi;

    // Rel-8
    SIM_EF_MAPPING_SPNI_T       spni;
    SIM_EF_MAPPING_PNNI_T       pnni;

    SIM_EF_MAPPING_ORANGE_DYN2_FLAG_T dyn2flag;
    SIM_EF_MAPPING_ORANGE_CSP_LINE2_T csp_line2;
    SIM_EF_MAPPING_ORANGE_DYN_FLAG_T dynflag;
    SIM_EF_MAPPING_ACM_T    acm_line2;
    SIM_EF_MAPPING_IMSI_M_T cdma_imsi_m;
    SIM_EF_MAPPING_CFIS_T cfis;
    SIM_EF_MAPPING_MWIS_T mwis;
    SIM_VMW_IND_FLAG_T vmw_ind_flag;
    SIM_EF_MAPPING_KEYS_T  keys;
    SIM_EF_MAPPING_KEYS_PS_T keys_ps; 
	SIM_EF_MAPPING_ACL_T acl;
	 
	uint8					    buf[SIM_MAX_FILE_STRUCTURE_LEN];		//the max data len buf.
}SIM_FILE_STRUCTURE_BUF_U;

#define SIM_MAX_IMG_DATA_BODY_NUM 253

typedef struct
{
    uint8 img_width;
    uint8 img_height;
    uint8 img_cs;
    uint16 img_len;                  //for basic image,the len= pure image data len,for color image,len=pure image data len + 3*num_of_clut_entry
    uint8 bits_per_img_point; //only for color image
    uint16 num_of_clut_entry; //only for color image
    uint8 location_clut;          //only for color image
    uint8  img_body[SIM_MAX_IMG_DATA_BODY_NUM]; //for basic image,pure image data in it;for color image,include pure image data and CLUT
}SIM_EF_MAPPING_IIDF_T;
    
    
typedef struct
{
    uint8 img_instance_num;
    SIM_EF_MAPPING_IIDF_T iidf;
}SIM_IMG_INFO_T;

#ifdef   __cplusplus
    }
#endif


#endif	//_FILE_STRUCTURE_H_
