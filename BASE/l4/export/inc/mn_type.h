/******************************************************************************
 ** File Name:      mn_type.h                                                 *
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
#if !defined (_MN_TYPE_H)
#define _MN_TYPE_H

#include "sci_types.h"
#include "os_api.h"
#include "mn_error.h"

 /*****************************************************************************
 **                        COMMON DEFINE                                      *
 ******************************************************************************/
#define MN_MBBMS_ALGO_REQ_LEN   128
#define MN_COMMON_ALGO_REQ_LEN  128
#define MN_MAX_IMSI_INFO_ARR_LEN    16
#define MN_MAX_MCC_MNC_ARR_LEN  3
#define APP_MN_KC_CK_LEN        20
#define APP_MN_IK_LEN           20
#define APP_MN_SRES_AUTN_LEN    20

#define MN_MAX_TMSI_ARR_LEN             4
#define MN_MAX_IMSI_ARR_LEN             8
#define MN_MAX_PTMSI_ARR_LEN            4
#define MN_MAX_KC_ARR_LEN               8
#define MN_MAX_ADDR_BCD_LEN             20    // max dial number len,modify by sunsome cr10312
#define MN_MAX_SUBADDR_LEN              20
#define MN_MAX_STRING_LEN               100
#define MN_MAX_BLOCK_LEN                100
#define MN_MAX_NCELL_NUM                6
#define MN_MAX_CELL_NUM                 7
#define MN_MAX_MOBILE_ID_LENGTH         9
#define MN_MAX_IMEI_LENGTH              (MN_MAX_MOBILE_ID_LENGTH-1)
#define MN_MAX_CURRENCY_CODE_NUM        3
#define MN_SMS_ADDR_MAX_LEN             12

#define MN_SMS_DLVR_SUCCESSFUL          0
#define MN_SMS_DLVR_UNSPECIFIED_ERR     255

#define MN_SS_MAX_NUM_OF_ADD_DATA       255


#define MN_MAX_L3_MESSAGE_SIZE          251
#define MN_SMS_SIM_SMS_LEN              175
#define MN_MAX_PARSE_STR_LEN            41
#define MN_MAX_OPER_NAME_LEN            60 // Refer to SIM
#define MN_MAX_POWER_SWEEP_LEN          40
#define MN_MAX_SIMAT_SS_STR_LEN         255
#define MN_MAX_UIM_IMSI_LEN             10

#define MN_SIM_MAX_VALIDATE_LANGUAGE_NUM 8

#define MN_NET_LOCK_PLMN_LIST_SIZE		32

#define MN_MAX_NUMBER_STRING_LEN            80

#ifdef _LESSEN_TRACE_SUPPORT
#define SCI_TRACE_SLIGHT 
#else
#define SCI_TRACE_SLIGHT SCI_TRACE_LOW
#endif

typedef uint8 MN_IMEI_T[MN_MAX_IMEI_LENGTH];
typedef uint8 MN_IMEISV_T[MN_MAX_MOBILE_ID_LENGTH];
typedef uint8 MNBCD;

typedef enum
{
    MN_RETURN_SUCCESS,       // success
    MN_RETURN_FAILURE        // unknown  
} MN_RETURN_RESULT_E;

typedef enum {
    MN_UNSPECIFIED = 0,
    MN_GSM_PREFER,
    MN_TD_PREFER
}MN_PS_SERVICE_RAT_E;

typedef enum _MN_CRSM_COMMAND_TYPE_E
{
    MN_CRSM_READ_BINARY     = 176,
    MN_CRSM_READ_RECORD     = 178,
    MN_CRSM_GET_RESPONSE    = 192,
    MN_CRSM_RETRIEVE_DATA   = 203,
    MN_CRSM_UPDATE_BINARY   = 214,
    MN_CRSM_SET_DATA        = 219,
    MN_CRSM_UPDATE_RECORD   = 220,
    MN_CRSM_GET_STATUS      = 242,
    MN_CRSM_COMMAND_TYPE_NUM
} MN_CRSM_COMMAND_TYPE_E;

typedef struct mn_sim_restrict_access_struct {
    uint8           sw1;        /* response type integer format */
    uint8           sw2;        /* response in integer type */
    uint8           length;     /* length of the data/response */
    uint8           data[255];  /* actual data */
    BOOLEAN         response;   /* response true is sim operate successfully  */
} mn_sim_restrict_access_struct;

typedef enum _MN_NUMBER_TYPE_E
{
    MN_NUM_TYPE_UNKNOW          = 0,    /*0000,an unknow number*/
    MN_NUM_TYPE_INTERNATIONAL,          /*0001,international number*/
    MN_NUM_TYPE_NATIONAL,               /*0010,national number*/
    MN_NUM_TYPE_NETWORK,                /*0011,network specific number*/
    MN_NUM_TYPE_DEDICATECODE_SHORTCODE, /*0100,dedicated code or short code*/
    MN_NUM_TYPE_ALPHANUMERIC,           /*0101,alphanumeric*/
    MN_NUM_TYPE_ABBREVIATE,             /*0110,abbreviate number*/
    MN_NUM_TYPE_RESERVED                /*0111,reserved for extention*/
} MN_NUMBER_TYPE_E;

typedef enum _MN_NUMBER_PLAN_E
{
    MN_NUM_PLAN_UNKNOW          = 0,    /*0000,an unknow number*/
    MN_NUM_PLAN_ISDN_TELE,              /*0001,ISDN/telephony numbering plan*/
    MN_NUM_PLAN_DATA            = 3,    /*0011,data numbering plan*/
    MN_NUM_PLAN_TELEX,                  /*0100,telex numbering plan*/
    MN_NUM_PLAN_NATIONAL        = 8,    /*1000,national numbering plan*/
    MN_NUM_PLAN_PRIVATE ,               /*1001,private numbering plan*/
    MN_NUM_PLAN_ERMES,                  /*1001,ermes numbering plan*/
    MN_NUM_PLAN_RESERVED        = 15,   /*1111,reserved for extention*/
    MN_NUM_PLAN_CANNOTUSE      
} MN_NUMBER_PLAN_E;

typedef enum _MN_SUBADDR_TYPE_E
{
    MN_SUBADDR_NSAP_TYPE        = 0,    /*000,X.213/ISO 8348AD2*/
    MN_SUBADDR_USER_SPEC_TYPE   = 2     /*010,user specified*/
} MN_SUBADDR_TYPE_E;

typedef enum _MN_OPER_TYPE_E
{
    MN_OPER_NAME,
    MN_OPER_NAME_SHORTFORM
} MN_OPER_TYPE_E;

typedef enum _MN_LOCI_STATUS_E
{
    MN_LOCI_UPDATED,
    MN_LOCI_NOT_UPDATED,
    MN_LOCI_PLMN_NOT_ALLOWED,
    MN_LOCI_LAI_NOT_ALLOWED,
    MN_LOCI_REVERSE_STATE
} MN_LOCI_STATUS_E;

typedef enum _MN_LENGTH_TYPE_E
{
    MN_LENGTH_EVEN,
    MN_LENGTH_ODD
} MN_LENGTH_TYPE_E;

typedef enum {
	BROWSER_E,
	MMS_E,
	DOWNLOAD_E,
	STREAMING_E,
	IM_E,
	POC_E,
	MAX_PS_TYPE_E
}MN_PS_SERVICE_TYPE_E;

typedef enum
{
    MN_GPRS_STORAGE_SIM = 0x00,
    MN_GPRS_STORAGE_NV,
    MN_GPRS_STORAGE_ALL
} MN_GPRS_STORAGE_E;

typedef struct _MN_LAI_T
{
    uint16  mcc;
    uint16  mnc;
    uint16  mnc_digit_num;
    uint16  lac;
} MN_LAI_T;


typedef struct _MN_IMSI_T
{
    uint8 imsi_len;
    uint8 imsi_val[MN_MAX_IMSI_ARR_LEN];
} MN_IMSI_T;

typedef struct _MN_IMEISVN_T
{
	MN_IMEI_T 	imei;
	BOOLEAN  	svn_flag; //true--svn exit;false--svn not exit
	uint8  		svn;
}MN_IMEISVN_T;

typedef struct _MN_CPHS_INFO_T
{
    BOOLEAN is_valid;
    uint8   phase;
    BOOLEAN opName_shform_active;
    BOOLEAN mailbox_active;
    BOOLEAN SST_active;
    BOOLEAN CSP_active;
    BOOLEAN infoNum_active;
} MN_CPHS_INFO_T;

typedef struct _MN_CFF_T
{
    BOOLEAN cff_line_1;
    BOOLEAN cff_line_2;
} MN_CFF_T;

typedef struct _MN_OPER_NAME_T
{
    MN_OPER_TYPE_E type;
    uint8          length;
    uint8          buf[MN_MAX_OPER_NAME_LEN];
} MN_OPER_NAME_T;

typedef struct _MN_TMSI_T 
{
    BOOLEAN  valid_flag;
    uint8    tmsi_value[MN_MAX_TMSI_ARR_LEN];
} MN_TMSI_T;

typedef struct _MN_SUBADDR_T 
{
    MN_SUBADDR_TYPE_E  sub_addr_type;
    MN_LENGTH_TYPE_E   odd_even_type;
    uint16             addr_len;
    uint8              sub_num[MN_MAX_SUBADDR_LEN];
} MN_SUBADDR_T;

typedef struct _MN_PLMN_T
{
    uint16  mcc;
    uint16  mnc;
    uint16  mnc_digit_num;
} MN_PLMN_T;

typedef struct _MN_STRING_T
{
    uint16  strlen;
    uint8   strbuf[MN_MAX_STRING_LEN];
} MN_STRING_T;

typedef struct _MN_BLOCK_T
{
    uint16  blocklen;
    uint8   blockbuf[MN_MAX_BLOCK_LEN];
} MN_BLOCK_T;

typedef struct _MN_CALLED_NUMBER_T 
{
    MN_NUMBER_TYPE_E  number_type;
    MN_NUMBER_PLAN_E  number_plan;
    uint16            num_len;
    uint8             party_num[MN_MAX_ADDR_BCD_LEN];
} MN_CALLED_NUMBER_T;

typedef enum _MN_PRESENT_IND_E
{
    MN_PRESENT_ALLOW,
    MN_PRESENT_RESTRICT,
    MN_PRESENT_NO_AVAILABLE
} MN_PRESENT_IND_E;

typedef enum _MN_SCREEN_IND_E
{
    MN_NOT_SCREEN,
    MN_SCREEN_VRF_PASS,
    MN_SCREEN_VRF_FAIL,
    MN_SCREEN_NETWORK_PVD
} MN_SCREEN_IND_E;

typedef enum
{
    MN_SMS_PS_DOMAIN_SERVICE = 0,
    MN_SMS_CS_DOMAIN_SERVICE,
    MN_SMS_PS_DOMAIN_SERVICE_PREF,
    MN_SMS_CS_DOMAIN_SERVICE_PREF,
    MN_SMS_NO_DOMAIN_SERVICE
}MN_SMS_SERVICE_TYPE_E;

typedef enum mn_protocol_stack_sub_state_enum
{
    MN_PROTOCOL_STACK_LIMITED_SERVICE_MODE,
    MN_PROTOCOL_STACK_FULL_MODE,
    MN_PROTOCOL_STACK_NUM
}mn_protocol_stack_sub_state_enum;

typedef struct _MN_CALLING_NUMBER_T 
{
    MN_NUMBER_TYPE_E  number_type;
    MN_NUMBER_PLAN_E  number_plan;
    MN_PRESENT_IND_E  present_ind;
    MN_SCREEN_IND_E   screen_ind;
    uint16            num_len;
    uint16            party_num[MN_MAX_ADDR_BCD_LEN];
    uint8              calling_uri[MN_MAX_NUMBER_STRING_LEN+1];
} MN_CALLING_NUMBER_T;


typedef struct _MN_SMS_ADDR_T {
    uint16 length;
    uint8  addr_arr_t[MN_SMS_ADDR_MAX_LEN];
} MN_SMS_ADDR_T;

typedef struct _MN_L3_MSG_UNIT_T {
    uint16  length;
    uint8   l3_msg[MN_MAX_L3_MESSAGE_SIZE];
} MN_L3_MSG_UNIT_T;

#if defined MULTI_SIM_SYS_SINGLE 
    typedef enum
    {
        MN_DUAL_SYS_1 = 0,
        MN_DUAL_SYS_MAX
    } MN_DUAL_SYS_E;
    #define MN_SYS_NUMBER  1
#elif defined MULTI_SIM_SYS_DUAL 
    typedef enum
    {
        MN_DUAL_SYS_1 = 0,
        MN_DUAL_SYS_2 = 1,
        MN_DUAL_SYS_MAX
    } MN_DUAL_SYS_E;
    #define MN_SYS_NUMBER  2
#elif defined MULTI_SIM_SYS_TRI
    typedef enum
    {
        MN_DUAL_SYS_1 = 0,
        MN_DUAL_SYS_2 = 1,
        MN_DUAL_SYS_3 = 2,
        MN_DUAL_SYS_MAX
    } MN_DUAL_SYS_E;
    #define MN_SYS_NUMBER  3
#elif defined MULTI_SIM_SYS_QUAD
    typedef enum
    {
        MN_DUAL_SYS_1 = 0,
        MN_DUAL_SYS_2 = 1,
        MN_DUAL_SYS_3 = 2,
        MN_DUAL_SYS_4 = 3,
        MN_DUAL_SYS_MAX
    } MN_DUAL_SYS_E;
    #define MN_SYS_NUMBER  4
#else
    #error need MN_SYS_NUMBER define
#endif

#define MULTI_SYS_NUM   MN_SYS_NUMBER


 /*****************************************************************************
 **                        PHONEBOOK MODULE DEFINE                            *
 ******************************************************************************/

#define PHONEBOOK_DEFAULT_ENTRY_ID              0
#define PHONEBOOK_INVALID_ENTRY_ID              0xffff
#define PHONEBOOK_MAX_DN_ADDRESS_IDENTIFY_LEN   80
#define PHONEBOOK_MAX_DN_NUMBER_LEN             MN_MAX_ADDR_BCD_LEN
#define PHONEBOOK_MAX_NORMAL_DN_NUMBER_LEN      10  //the max num length in SIM normal PB file
#define PHONEBOOK_MAX_EXT_DN_NUMBER_LEN         (PHONEBOOK_MAX_DN_NUMBER_LEN - PHONEBOOK_MAX_NORMAL_DN_NUMBER_LEN)  //the max num length in ext PB file

#ifdef PRODUCT_DM
#define PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN     241
#else
#define PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN     20
#endif

typedef enum _PHONEBOOK_ALPHABET_TYPE_E
{
    PHONEBOOK_ALPHABET_ASCII,
    PHONEBOOK_ALPHABET_UCS2
} PHONEBOOK_ALPHABET_TYPE_E;


typedef enum _PHONEBOOK_STORAGE_E
{
    PHONEBOOK_SIM_STORAGE,       // SIM: ADN
    PHONEBOOK_FDN_STORAGE,       // SIM: Fix dialing number phonebook
    PHONEBOOK_NV_STORAGE,        // NV:  phonebook
    PHONEBOOK_LND_STORAGE,       // SIM: dialed calls list
    PHONEBOOK_MDN_STORAGE,       // NV:  missed calls list
    PHONEBOOK_RDN_STORAGE,       // NV:  received calls list
    PHONEBOOK_MSISDN_STORAGE,    // SIM: MSISDN
    PHONEBOOK_EMERGENCY_STORAGE, // SIM: emergence call
    PHONEBOOK_BDN_STORAGE,
    PHONEBOOK_SDN_STORAGE,         //SIM: Service Dialling Number phonebook 

    PHONEBOOK_2G_MAX_STORAGE_NUM,    // the max num for 2G

    PHONEBOOK_ACL_STORAGE = PHONEBOOK_2G_MAX_STORAGE_NUM,

    PHONEBOOK_USIM_TEL_STORAGE,  // USIM:ADN in DFtelecom/DFphonebook
    PHONEBOOK_USIM_ADF_STORAGE,  // USIM:ADN in DFusim/DFphonebook   
    /* +vcard dev binli */ 
    PHONEBOOK_IAP_TEL_STORAGE,// USIM:IAP in DFtelecom/DFphonebook
    PHONEBOOK_IAP_ADF_STORAGE,// USIM:IAP in DFusim/DFphonebook  
    PHONEBOOK_AAS_TEL_STORAGE,// USIM:AAS in DFtelecom/DFphonebook
    PHONEBOOK_AAS_ADF_STORAGE,// USIM:AAS in DFusim/DFphonebook  
    PHONEBOOK_EMAIL_TEL_STORAGE,// USIM:EMAIL in DFtelecom/DFphonebook
    PHONEBOOK_EMAIL_ADF_STORAGE,// USIM:EMAIL in DFusim/DFphonebook  
    PHONEBOOK_ANR_TEL_STORAGE,// USIM:ANR in DFtelecom/DFphonebook
    PHONEBOOK_ANR_ADF_STORAGE, // USIM:ANR in DFusim/DFphonebook  
    PHONEBOOK_SNE_TEL_STORAGE,  // USIM:SNE in DFtelecom/DFphonebook  /*CR179773 Eddie.Wang*/
    PHONEBOOK_SNE_ADF_STORAGE,  // USIM:SNE in DFtelecom/DFphonebook  /*CR179773 Eddie.Wang*/

    PHONEBOOK_GRP_TEL_STORAGE,
    PHONEBOOK_GRP_ADF_STORAGE,
    PHONEBOOK_MAX_STORAGE_NUM
/* -vcard dev binli */  
} PHONEBOOK_STORAGE_E;


typedef struct _PHONEBOOK_INFO_T
{
    BOOLEAN is_phonebook_exist;
    BOOLEAN is_user_need;       // 
    uint8   max_alpha_len;      //the max alpha id len that will be supported in our mobile
    uint8   max_normal_num_len; //the max num length in normal PB file.
    uint8   max_ext_num_len;    //the max num length in ext PB file.
    uint16  max_record_num;     //the max record num that will be supported in our mobile
    uint16  used_record_num;
    uint8   current_dn_empty_ext_num;//current DN's empty EXT records number
} PHONEBOOK_INFO_T;

typedef struct _PHONEBOOK_ENTRY_T
{
    uint8               alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN]; //the alpha id 
    MNBCD               number[PHONEBOOK_MAX_DN_NUMBER_LEN];        //the dialling number
    MN_NUMBER_TYPE_E    ton;                                        //type of number
    MN_NUMBER_PLAN_E    npi;                                        //the Number plan identifier
    uint32              time;
    uint16              entry_id;
    uint8               alpha_len;
    uint8               number_len;
} PHONEBOOK_ENTRY_T;

 /*****************************************************************************
 **                        MNNV MODULE DEFINE                                 *
 ******************************************************************************/


 /*****************************************************************************
 **                        MNSIM MODULE DEFINE                                *
 ******************************************************************************/
#define MNSIM_ICCID_ID_NUM_LEN      10
#define MNSIM_SPN_ID_NUM_LEN        16
#define MNSIM_PIN_MAX_LEN           8

typedef enum _MNSIM_PHASE_E
{
    MNSIM_PH_1      = 0x00,
    MNSIM_PH_2      = 0x02,
    MNSIM_PH_2_PD   = 0x03
} MNSIM_PHASE_E;

typedef enum
{
    MNSIM_PH2_PIN1_DISABLE,
    MNSIM_PH2_ADN,
    MNSIM_PH2_FDN,
    MNSIM_PH2_SMS,
    MNSIM_PH2_AOC,
    MNSIM_PH2_CCP,
    MNSIM_PH2_PLMN_SEL,
    MNSIM_PH2_RFU1,
    MNSIM_PH2_MSISDN,
    MNSIM_PH2_EXT1,
    MNSIM_PH2_EXT2,
    MNSIM_PH2_SMSP,
    MNSIM_PH2_LND,
    MNSIM_PH2_CBMI,
    MNSIM_PH2_GID1,
    MNSIM_PH2_GID2,
    MNSIM_PH2_SPN,
    MNSIM_PH2_SDN,
    MNSIM_PH2_EXT3,
    MNSIM_PH2_RFU2,
    MNSIM_PH2_VGCS,
    MNSIM_PH2_VBS,
    MNSIM_PH2_EMLPP,
    MNSIM_PH2_AAEM,
    MNSIM_PH2_SMS_CB_DL,
    MNSIM_PH2_SMS_PP_DL,
    MNSIM_PH2_MENU_SEL,
    MNSIM_PH2_CALL_CON,
    MNSIM_PH2_PROACTIVE_SIM,
    MNSIM_PH2_CBMIR,
    MNSIM_PH2_BDN,
    MNSIM_PH2_EXT4,
    MNSIM_PH2_DCK,
    MNSIM_PH2_CO_NL,
    MNSIM_PH2_SMS_STATUS_REPORT,
    MNSIM_PH2_NIA,
    MNSIM_PH2_SIM_CON_MO_SMS,
    MNSIM_PH2_GPRS,
    MNSIM_PH2_IMAGE,
    MNSIM_PH2_SOLSA,
    MNSIM_PH2_USSD_IN_CC,
    MNSIM_PH2_RUN_AT_COMMAND,
    MNSIM_PH2_PLMN_WACT,
    MNSIM_PH2_OPLMN_WACT,
    MNSIM_PH2_HPLMN_WACT,
    MNSIM_PH2_CPBCCH,
    MNSIM_PH2_INV_SCAN,
    MNSIM_PH2_EXT_CCP,
    MNSIM_PH2_MEXE,
    MNSIM_PH2_RESERVED,
    MNSIM_PH2_PNN,
    MNSIM_PH2_OPL,
    MNSIM_PH2_MDN,
    MNSIM_PH2_MWIS,
    MNSIM_PH2_CFIS,
    MNSIM_PH2_SPDI,
    MNSIM_PH2_MMS,
    MNSIM_PH2_EXT8,
    MNSIM_PH2_MMSUCP,
    MNSIM_PH2_2G_SST_END_NUM,
    MNSIM_PH2_LPB,
    MNSIM_PH2_OCI,
    MNSIM_PH2_ICI,
    MNSIM_PH2_CCP2,
    MNSIM_PH2_GSM_ACCESS,
    MNSIM_PH2_EST,
    MNSIM_PH2_ACL,
    MNSIM_PH2_GSM_SEC_CONTEXT,
    MNSIM_PH2_EXT5,
    MNSIM_PH2_CC_GPRS_BY_USIM,
    MNSIM_PH2_PSEUDONYM,
    MNSIM_PH2_UPLMN_WLAN,
    MNSIM_PH2_OPLMN_WLAN,
    MNSIM_PH2_UC_WSID,
    MNSIM_PH2_OC_WSID,
    MNSIM_PH2_VGCS_SEC,
    MNSIM_PH2_VBS_SEC,
    MNSIM_PH2_WLAN_REAUTH_ID,
    MNSIM_PH2_MMS_STORAGE,
    MNSIM_PH2_GBA,
    MNSIM_PH2_MBMS_SEC,
    MNSIM_PH2_DL_USSD,
    MNSIM_PH2_EHPLMN,
    MNSIM_PH2_ADD_TERMINAL_PROFILE,
    MNSIM_PH2_EHPLMNPI,
    MNSIM_PH2_LAST_RPLMN_SI,
    MNSIM_PH2_OMA_BCAST_PROFILE,
    MNSIM_PH2_GBA_LOCAL_KEY,
    MNSIM_PH2_TERMINAL_APP,
    MNSIM_PH2_SPN_ICON,
    MNSIM_PH2_PNN_ICON,
    MNSIM_PH2_CP_USIM_IP,
    MNSIM_PH2_HIWLAN_ID_LIST,
    MNSIM_PH2_IWLAN_EHPLMN,
    MNSIM_PH2_IWLAN_HPLMN_PRIORITY,
    MNSIM_PH2_IWLAN_LAST_RPLMN,
    MNSIM_PH2_EPS_MM,
    MNSIM_PH2_CSG_INDICATION,
    MNSIM_PH2_CC_EPS_BY_USIM,
    MNSIM_PH2_HPLMN_DA,
    MNSIM_PH2_ECALL_DATA,
    MNSIM_PH2_INFONUM,          // Information Numbers
    MNSIM_PH2_NUM
} MNSIM_SERVICE_TYPE_PH2_E;

typedef enum
{
    MNSIM_MS_NORMAL_OPERATION                   = 0x00,
    MNSIM_MS_TYPE_APPROVAL_OPERATION            = 0x80,
    MNSIM_MS_NORMAL_SPECIFIC_FACILITIES         = 0x01,
    MNSIM_MS_TYPE_APPROVAL_SPECIFIC_FACILITIES  = 0x81,
    MNSIM_MS_MAINTENANCE                        = 0x02,
    MNSIM_CELL_TEST                             = 0x04
} MNSIM_MS_OPERATION_MODE_E;

typedef enum _MNSIM_PIN_STATUS_E
{
    MNSIM_PIN_READY,
    MNSIM_PIN_PIN,
    MNSIM_PIN_PUK,
    MNSIM_PIN_BLOCKED
} MNSIM_PIN_STATUS_E;

typedef enum _MNSIM_PIN_INDICATION_E
{
    MNSIM_PIN1 = 0x01,
    MNSIM_PIN2 = 0x02
} MNSIM_PIN_INDICATION_E;

//the PIN status
typedef struct _MNSIM_PIN_STATUS_T
{
    BOOLEAN pin_verified;
    BOOLEAN secrect_initialized;    //secret code initialized or not
    BOOLEAN pin_blocked;            //PIN blocked or not
    uint8   false_remain;           //the number of false presentations remaining
} MNSIM_PIN_STATUS_T;

typedef struct _MNSIM_SERVICE_INFO_T
{
    BOOLEAN allocated;
    BOOLEAN activated;
} MNSIM_SERVICE_INFO_T;

#define MN_MAX_ECC_LEN  3
#define MN_MAX_ECC_NUM  15
#define MN_MAX_ECC_ALPHA_ID_LEN 25//the same as SIM_MAX_ECC_ALPHA_ID_LEN

typedef struct
{
    MNBCD ecc_code[MN_MAX_ECC_LEN];
    BOOLEAN alpha_id_flag;
    uint8 alpha_id_len;
    uint8 alpha_id[MN_MAX_ECC_ALPHA_ID_LEN];
    BOOLEAN ecc_category_flag;
    uint8 ecc_category;
} MN_ECC_NUM_T;

typedef struct
{
    uint8           validate_num;
    MN_ECC_NUM_T    call_codes[MN_MAX_ECC_NUM];
} MNSIM_ECC_T;

typedef struct _MNSIM_SST_T
{
    uint8                   service_num;
    MNSIM_SERVICE_INFO_T    service_table[MNSIM_PH2_NUM];
} MNSIM_SST_T;

typedef struct _MNSIM_ADMIN_T
{
    MNSIM_MS_OPERATION_MODE_E operation_mode;
    BOOLEAN                   ofm_actived;  //TRUE      OFM to be activated by the ME
                                            //FALSE     OFM to be disabled by the ME 
    BOOLEAN                 mnc_len_exist;  //TRUE      "length of MNC in the IMSI" exist
    uint8                   mnc_digit_num;  //default value is 2 digits
} MNSIM_ADMIN_T;

typedef enum _MNSIM_PIN_REQ_TYPE_E
{
    MNSIM_PIN_VERIFY,
    MNSIM_PIN_CHANGE,
    MNSIM_PIN_DISABLE,
    MNSIM_PIN_ENABLE,
    MNSIM_PIN_UNBLOCK
} MNSIM_PIN_REQ_TYPE_E;

typedef enum _MNSIM_PIN_REQ_RESULT_E
{
    MNSIM_PIN_REQ_OK,
    MNSIM_PIN_WRONG_REQ_TYPE,           //the PIN request type is wrong
    MNSIM_PIN_ERROR_WITH_BLOCKED,       //the PIN operation error and the PIN been blocked
    MNSIM_PIN_ERROR_CONTRADICT_STATUS,  //the PIN status is contradict to the PIN operation
    MNSIM_PIN_ERROR_NOT_INITIALIZED,    //the PIN still not been initialized
    MNSIM_PIN_ERROR_PIN_FAIL,           //do not fulfill the access condition of the PIN operation
    MNSIM_PIN_ERROR_UNKNOW              //unknow PIN operation error
} MNSIM_PIN_REQ_RESULT_E;

typedef struct
{
    uint8   currency_code[MN_MAX_CURRENCY_CODE_NUM + 1];        // CURRENCY_CODE_T;spec 中规定是3，但是为了mmi判断长度，需要在最后加上字符串结束符号，所以增加一个字节的长度
    BOOLEAN ex_sign;                // the sign of ex,TRUE positive,FALSE is negative
    uint8   ex;
    uint16  eppu;                   // price per unit,eppu*10^ex
} MNSIM_PUCT_INFO_T;

typedef struct _MNSIM_AOC_T
{
    BOOLEAN     aoc_is_support;
    uint32      acm_unit;
    uint32      acm_max;
    MNSIM_PUCT_INFO_T puct;    /*@@@king.li new adds for cr90738@@@ */
    uint32      line2_acm_unit;
} MNSIM_AOC_T;

typedef struct
{
    MNBCD id_num[MNSIM_ICCID_ID_NUM_LEN]; 
} MNSIM_ICCID_T;

typedef struct
{
    uint8 display_condition;
    MNBCD id_num[MNSIM_SPN_ID_NUM_LEN];     // coded as phonebook alpha
} MNSIM_SPN_T;

typedef struct MN_SPDI_PLMN_T_s
{
    BOOLEAN     present;
    MN_PLMN_T   plmn;
} MN_SPDI_PLMN_T;

#define MNSIM_SPDI_PLMN_MAX_NUM   10
typedef struct MNSIM_SPDI_T_s
{
    uint8           num;
    MN_SPDI_PLMN_T  plmn_arr[MNSIM_SPDI_PLMN_MAX_NUM];
} MNSIM_SPDI_T;

// SIM error cause
typedef enum _MNSIM_ERROR_E
{
    MNSIM_ERROR_POWER_OFF,
    MNSIM_ERROR_INSERTED_ERROR,           // SIM_INSERTED_ERROR/_INSERTED_WRONG_SIM 
    MNSIM_ERROR_REMOVED,                  // SIM_REMOVED/_REMOVED_UNDETECTED        
    MNSIM_ERROR_INVALID_STATUS_RSP,       // SIM_REMOVED_INVALID_STATUS_RSP         
    MNSIM_ERROR_INITIALISE_FAILED,        // SIM_INITIALISE_FAILED                  
    MNSIM_ERROR_INVALID_PARAMS,           // SIM_REQ_INVALID_PARAMS                 
    MNSIM_ERROR_MEMORY_PROBLEM,           // SIM_REQ_MEMORY_PROBLEM                 
    MNSIM_ERROR_FILE_NOT_FOUND,           // SIM_REQ_FILE_NOT_FOUND                 
    MNSIM_ERROR_SERVICE_NOT_AVAILABLE,    // SIM_REQ_SERVICE_NOT_AVAILABLE          
    MNSIM_ERROR_GENERAL_FAULT,            // SIM_REQ_SIM_GENERAL_FAULT              
    MNSIM_ERROR_FILE_INVALIDATED,         // SIM_REQ_FILE_INVALIDATED               
    MNSIM_ERROR_ALLOC_ERROR,              // SIM_REQ_ALLOC_ERROR                    
    MNSIM_ERROR_SM_FAULT,                 // SIM_REQ_SM_FAULT                       
    // codes resulting from info in PINFunctionCnf (pinStatus etc.) 
    // OR the Initialise failed                                         
    MNSIM_ERROR_PIN1_UNINITIALIZED,
    MNSIM_ERROR_PIN2_UNINITIALIZED,
    MNSIM_ERROR_PIN1_BLOCKED,
    MNSIM_ERROR_PIN2_BLOCKED,
    MNSIM_ERROR_PIN1_UNBLOCK_BLOCKED,
    MNSIM_ERROR_PIN2_UNBLOCK_BLOCKED,
    MNSIM_ERROR_NO_SECRETCODE_INITIALIZED,

    MNSIM_ERROR_REJECT_BY_NET,

    MNSIM_ERROR_SERIOUS,
    MNSIM_ERROR_UNKNOWN,
    MNSIM_ERROR_IMSI_INVALID,
    MNSIM_ERROR_SIM_UNRESPONSE,
    MNSIM_ERROR_SIM_PART_RESPONSE,
    MNSIM_ERROR_NOT_SUPPORT_GSM_ONLY,/* bin.li add for support gsm only or not */
    MNSIM_ERROR_SIM_NO_ERROR,   //27
    MNSIM_ERROR_SIM_DATA_INTEGRITY_FAIL,  //28
    MNSIM_ERROR_SIM_NETWORT_LOCKED,   //29
    MNSIM_ERROR_SIM_NETWORT_SUBSET_LOCKED,  //30
    MNSIM_ERROR_SIM_SP_LOCKED,   //31
    MNSIM_ERROR_SIM_CORPORATE_LOCKED, //32
    MNSIM_ERROR_SIM_USER_LOCKED,  //33
    MNSIM_ERROR_SIM_HOTSWAP_REMOVE,
    MNSIM_ERROR_NULL
} MNSIM_ERROR_E;

typedef enum _MNSIM_GID_E
{
    MNSIM_GID1 = 0x01,
    MNSIM_GID2 = 0x02
} MNSIM_GID_E;


typedef struct _MN_CSP_CALL_OFFERING_T
{
    BOOLEAN is_cfu;        // call forwarding unconditional
    BOOLEAN is_cfb;        // call forwarding on user busy
    BOOLEAN is_cfnry;      // call forwarding on no reply
    BOOLEAN is_cfnrc;      // call forwarding on user not reachable
    BOOLEAN is_ct;         // call transfer
    BOOLEAN reserved[3];     // reserved
} MN_CSP_CALL_OFFERING_T;

typedef struct _MN_CSP_CALL_RESTRICTION_T
{
    BOOLEAN is_boac;       // barring of all outgoing calls
    BOOLEAN is_boic;       // barring of outgoing international calls
    BOOLEAN is_boic_exhc;  // international calls except those directed to the home PLMN country
    BOOLEAN is_baic;       // barring of all incoming calls
    BOOLEAN is_bic_roam;   // barring of all incoming calls when roaming outside the HPLMN country
    BOOLEAN reserved[3];     // reserved
} MN_CSP_CALL_RESTRICTION_T;

typedef struct _MN_CSP_OTHER_SUPP_SERVICES_T
{
    BOOLEAN is_mpty;       // multi-party service
    BOOLEAN is_cug;        // closed user group
    BOOLEAN is_aoc;        // advice of charge
    BOOLEAN is_pref_cug;   // preferential CUG
    BOOLEAN is_cug_oa;     // CUG outgoing access
    BOOLEAN reserved[3];   // reserved
} MN_CSP_OTHER_SUPP_SERVICES_T;

typedef struct _MN_CSP_CALL_COMPLETION_T
{
    BOOLEAN is_hold;                  // call hold
    BOOLEAN is_cw;                    // call waiting
    BOOLEAN is_ccbs;                  // completionof call to busy subscriber
    BOOLEAN is_u2u_signalling;        // restriction of menus allowing use of user to user signalling
    BOOLEAN reserved[4];              // reserved
} MN_CSP_CALL_COMPLETION_T;

typedef struct _MN_CSP_TELESERVICES_T
{
    BOOLEAN is_sm_mt;            // short message - mobile terminated
    BOOLEAN is_sm_mo;            // short message - mobile originated
    BOOLEAN is_sm_cb;            // short message - cell broadcast
    BOOLEAN is_reply_path;       // restricts menu options for the ability to set reply path active on out going short messages
    BOOLEAN is_del_conf;         // SMS delivery confirmation
    BOOLEAN is_protocol_id;      // restriction of menus for SMS protocol ID options
    BOOLEAN is_validity_period;  // restriction of menus for SMS validity period options
    BOOLEAN reserved;            // reserved
} MN_CSP_TELESERVICES_T;

typedef struct _MN_CSP_CPHS_TELESERVICES_T
{
    BOOLEAN is_als;              // alternate line service
    BOOLEAN reserved[7];         // reserved
} MN_CSP_CPHS_TELESERVICES_T;

typedef struct _MN_CSP_CPHS_FEATURE_T
{
    BOOLEAN is_sst;              // phase 1 only
    BOOLEAN reserved[7];         // reserved
} MN_CSP_CPHS_FEATURE_T;

typedef struct _MN_CSP_NUMBER_IDENTIF_T
{
    BOOLEAN is_clip;             // call line identification presentation
    BOOLEAN reserved1;           // reserved
    BOOLEAN is_colr;             // connected line identification restriction
    BOOLEAN is_colp;             // connected line identification presentation
    BOOLEAN is_mci;              // malicious call indicator
    BOOLEAN reserved2;           // reserved
    BOOLEAN is_cli_send;         // CLI per call mode - default block CLI - menu to send CLI
    BOOLEAN is_cli_block;        // CLI per call mode - default send CLI - menu to block CLI
} MN_CSP_NUMBER_IDENTIF_T;

typedef struct _MN_CSP_PHASE_2P_SERVICES_T
{
    BOOLEAN is_gprs;                     // menus concerned with GPRS functionality
    BOOLEAN is_hscsd;                    // menus concerned with high speed circuit switched data functionality
    BOOLEAN is_vgc;                      // ASCI voice group call menus
    BOOLEAN is_vbs;                      // ASCI voice broadcast service menus
    BOOLEAN is_msp;                      // phase 2+ multiple subscriber profile menus
    BOOLEAN is_multi_band;               // restriction of menus allowing user to select a particular GSM 900/1800 or 1900 band
    BOOLEAN reserved[2];                 // reserved
} MN_CSP_PHASE_2P_SERVICES_T;

typedef struct _MN_CSP_VALUEADDED_SERVICES_T
{
    BOOLEAN is_plmn_mode;        // restriction of menu options for manual PLMN selection
    BOOLEAN is_vps;              // restriction of menu options for voice mail or other similar menus
    BOOLEAN is_sm_mo_paging;     // restriction of menu options for the ability to send short messages with type paging
    BOOLEAN is_sm_mo_email;      // restriction of menu options messages with type email
    BOOLEAN is_fax;              // restriction of menu options for Fax calls
    BOOLEAN is_data;             // restriction of menu options for Data calls
    BOOLEAN reserved;            // reserved
    BOOLEAN is_language;         // restriction of menus allowing the user to change language
} MN_CSP_VALUE_ADDED_SERVICES_T;

typedef struct _MN_CSP_INFO_NUMBERS_T
{
    uint8   info_numbers;          // the ME shall only present information numbers to the user if this field is set to FF
} MN_CSP_INFO_NUMBERS_T;

typedef struct _MN_CSP_SPID_NUMBERS_T
{
    uint8   spid;
} MN_CSP_SPID_NUMBERS_T;

typedef struct _MN_CSP_SERVICES_GROUP_T
{
    BOOLEAN is_call_offering_present;
    MN_CSP_CALL_OFFERING_T          call_offering;
    BOOLEAN is_call_restriction_present;
    MN_CSP_CALL_RESTRICTION_T       call_restriction;
    MN_CSP_OTHER_SUPP_SERVICES_T    other_supp_services;
    MN_CSP_CALL_COMPLETION_T        call_completion;
    MN_CSP_TELESERVICES_T           teleservices;
    BOOLEAN is_cphs_teleserv_present;
    MN_CSP_CPHS_TELESERVICES_T      cphs_teleservices;
    MN_CSP_CPHS_FEATURE_T           cphs_feature;
    MN_CSP_NUMBER_IDENTIF_T         number_identif;
    MN_CSP_PHASE_2P_SERVICES_T      phase_2p_services;
    MN_CSP_VALUE_ADDED_SERVICES_T   value_added_services;
    MN_CSP_INFO_NUMBERS_T           info_numbers;
} MN_CSP_SERVICES_GROUP_T;

typedef struct _MN_CSP_LINE2_SERVICES_GROUP_T
{
    BOOLEAN is_call_offering_present;
    MN_CSP_CALL_OFFERING_T          call_offering;
    BOOLEAN is_call_restriction_present;
    MN_CSP_CALL_RESTRICTION_T       call_restriction;
    MN_CSP_OTHER_SUPP_SERVICES_T    other_supp_services;
    MN_CSP_CALL_COMPLETION_T        call_completion;
    MN_CSP_TELESERVICES_T           teleservices;
    MN_CSP_CPHS_TELESERVICES_T      rfu;
    MN_CSP_CPHS_FEATURE_T           cphs_feature;
    MN_CSP_NUMBER_IDENTIF_T         number_identif;
    MN_CSP_PHASE_2P_SERVICES_T      phase_2p_services;
    MN_CSP_VALUE_ADDED_SERVICES_T   value_added_services;
    MN_CSP_INFO_NUMBERS_T           info_numbers;
    MN_CSP_SPID_NUMBERS_T           spid;
} MN_CSP_LINE2_SERVICES_GROUP_T;

 /*****************************************************************************
 **                        GPRS MODULE DEFINE                                 *
 ******************************************************************************/

#define MN_GPRS_MAX_PCO_LEN                  253         //the max length of the PCO
#define MN_GPRS_MAX_PDP_TYPE_LEN             7           //the max length of the PDP type string
#define MN_GPRS_MAX_APN_LEN                  100         //the max length of the APN string
#define MN_GPRS_MAX_PDP_ADDRESS_LEN          20          //the max length of the PDP ADDRESS string
#define MN_GPRS_MAX_PDP_ID_NUM               15          //the max number of PDP ID
#define MN_GPRS_MAX_PDP_CONTEXT_COUNT        11          //the max count of PDP CONTEXT simultaneous.
#define MN_GPRS_MAX_TFT_FILTERS         8

#define MN_GPRS_USER_MAX_LENGTH              0x40 //64
#define MN_GPRS_PASSWD_MAX_LENGTH            0x40
#define MN_GPRS_MAX_ADDR_TYPE_LEN            7    
#define MN_GPRS_MAX_ADDR_LEN                 20   // ipv4ipv6:ipv4(4byte)+ipv6(16byte)

#define MN_GPRS_PDP_ID_INVALID               0  // the end flag of PDP_ID array

#define MN_GPRS_QOS_PARA_NUM                 5
#define MN_GPRS_QOS_PARA_NUM_RL99            17

#define MN_GPRS_MAX_SRC_ADDR_SBNET_MASK_LEN  32 //16 octet-IPv6 address,16 octet-IPv6 address mask
#define MN_GPRS_DST_PORT_RANGE_LEN           2
#define MN_GPRS_SRC_PORT_RANGE_LEN           2               
#define MN_GPRS_TOS_TRAF_CLASS_MASK_LEN      2  //1 octet-type of service/traffic class,1 octet-type of service/traffic class mask 
#define MN_GPRS_PFI_EPI_LEN_PFC_LEN          3  /*the length of packet filter identifier,packer filter evaluation precedence 
                                                 *and Length of packet filter contents  
                                                 */
#define MN_GPRS_IPSEC_PARAM_INDX_LEN         4  //length of IPSec security parameter index 
#define MN_GPRS_FLOW_LABEL_TYPE_LEN          3  //length of flow label type
#define MN_GPRS_TFT_OPCODE_NUM_PF_LEN        1  //length of TFT opcode and Number of packet filters

#define MN_GPRS_TFT_PNNH_NOT_PRESENT         0xFFFF
#define MN_GPRS_TFT_IPSEC_NOT_PRESENT        FALSE
#define MN_GPRS_TFT_FL_NOT_PRESENT           0xFFFFFFFF

typedef enum
{
    MN_GPRS_EVENT_REPORT_MODE_1,    // 0 buffer unsolicited result codes in the MT; if MT result code buffer is full, 
                                    //   the oldest ones can be discarded. No codes are forwarded to the TE.
    MN_GPRS_EVENT_REPORT_MODE_2,    // 1 discard unsolicited result codes when MT TE link is reserved 
                                    //   (e.g. in on line data mode); otherwise forward them directly to the TE
    MN_GPRS_EVENT_REPORT_MODE_3     // 2 buffer unsolicited result codes in the MT when MT TE link is reserved 
                                    //   (e.g. in on line data mode) and flush them to the TE 
                                    //   when MT TE link becomes available; otherwise forward them directly to the TE
}MN_GPRS_EVENT_REPORT_MODE_E;

typedef enum
{
    MN_GPRS_EVENT_REPORT_BUFFER_1,// 0 MT buffer of unsolicited result codes defined within this command is 
                                    //   cleared when <mode> 1 or 2 is entered
    MN_GPRS_EVENT_REPORT_BUFFER_2 // 1 MT buffer of unsolicited result codes defined within this command is 
                                    //   flushed to the TE when <mode> 1 or 2 is entered 
                                    //   (OK response shall be given before flushing the codes)
}MN_GPRS_EVENT_REPORT_BUFFER_E;

typedef enum 
{
    MN_GPRS_ERR_SUCCESS = 0,     //操作成功
    MN_GPRS_ERR_SUCCESS_IPV4_ONLY = 1,      //操作成功。网络仅支持IPV4。
    MN_GPRS_ERR_SUCCESS_IPV6_ONLY = 2,      //操作成功。网络仅支持IPV6。
    MN_GPRS_ERR_SUCCESS_SINGLE_ONLY = 3,    //操作成功。网络仅支持一种IP类型。
    MN_GPRS_ERR_START = 10,      //失败原因的起始值
    MN_GPRS_ERR_PRIMITIVE,       //下层传上来的消息类型错误
    MN_GPRS_ERR_MEM_ALLOC,       //申请内存不成功
    MN_GPRS_ERR_NO_NSAPI,        //NSAPI已经用完
    MN_GPRS_ERR_SENDER,          //
    MN_GPRS_ERR_PDP_TYPE,        //PDP类型不支持
    MN_GPRS_ERR_ATC_PARAM,       //调用函数时传入的参数不对
    MN_GPRS_ERR_PDP_ID,          //暂时未用
    MN_GPRS_ERR_SPACE_NOT_ENOUGH,//暂时未用
    MN_GPRS_ERR_ACTIVE_REJCET,   //PDP上下文激活被拒绝
    MN_GPRS_ERR_SAME_PDP_CONTEXT,//PDP上下文已经激活，不需再激活
    MN_GPRS_ERR_NSAPI,           //这个NSAPI正在使用
    MN_GPRS_ERR_MODIFY_REJ,      //修改PDP上下文的请求被网络端拒绝
    MN_GPRS_ERR_READ_TYPE,       //没有指定类型的信息
    MN_GPRS_ERR_PDP_CONTEXT_ACTIVATED,   //PDP上下文已经激活，不能删除
    MN_GPRS_ERR_NO_PDP_CONTEXT,           // 没有找到指定的PDP上下文.
    MN_GPRS_ERR_PERMENANT_PROBLEM = 100,   //持续问题，MMI应提示用户间隔一段时间再试。
    MN_GPRS_ERR_TEMPORARILY_BLOCKED,       //底层正在处理信令流程，MMI可以间隔较短时间重试多次。
    MN_GPRS_ERR_RETRYING,                  //临时问题，MMI可以间隔一段时间再试，重试1~3次。
    MN_GPRS_ERR_UNKNOWN_ERROR,             //未知错误。
    MN_GPRS_ERR_SERVICE_OPTION_NOT_SUPPORTED=132,          //服务选项不支持
    MN_GPRS_ERR_REQUEST_SERVICE_OPTION_NOT_SUBSCRIBED=133, //没有指定服务选项
    MN_GPRS_ERR_SERVICE_OPTION_OUTOF_ORDER=134,            //服务选项顺序不对
    MN_GPRS_ERR_PDP_AUTHENTICATION_FAILED=149,             //鉴定失败
    MN_GPRS_ERR_MISSING_OR_UNKOWN_APN=533,                 //缺少活不能够识别apn
    MN_GPRS_ERR_OPERATION_NOT_ALLOWED,
    MN_GPRS_ERR_NO_SATISFIED_RESOURCE
}MN_GPRS_ERR_E;

typedef enum {
    MN_GPRS_TE_REJECT_ACTIVE,      // MT Reject network pdp context activate request by manual set
    MN_GPRS_MT_ACTIVE              // Based on MS itself
}MN_GPRS_TE_RESPONSE_TYPE_E;

typedef enum{
    MN_GPRS_MODE_TURN_OFF_CGANS,    //0. GPRS network requests are manually accepted or 
                                    //   rejected by the +CGANS command.
    MN_GPRS_MODE_TURN_ON_CGANS,     //1. GPRS network requests are automatically accepted 
                                    //   according to the description above
    MN_GPRS_MODE_GPRS_ONLY_S0,      //2. automatic acceptance of GPRS network requests is
                                    //   controlled by the 'S0' command
    MN_GPRS_MODE_GPRS_CS_S0         //3. automatic acceptance of both GPRS network requests 
                                    //   and incoming circuit switched calls is controlled by 
                                    //   the 'S0' command
}MN_GPRS_TE_RESPONSE_MODE_E;

typedef enum{
    MN_GPRS_RELIABILITY_SUBSCRIBED = 0,
    MN_GPRS_RELIABILITY_ACK_GTP_LLC_RLCPROTECTED_DATA = 1,    // Acknowledged GTP, LLC, and RLC; Protected data
    MN_GPRS_RELIABILITY_UNACK_GTP_ACK_LLC_RLC_PROTECTED_DATA, // Unacknowledged GTP; Acknowledged LLC and RLC, Protected data
    MN_GPRS_RELIABILITY_UNACK_GTP_LLC_ACK_RLC_PROTECTED_DATA, // Unacknowledged GTP and LLC; Acknowledged RLC, Protected data
    MN_GPRS_RELIABILITY_UNACK_GTP_LLC_RLC_PROTECTED_DATA,     //Unacknowledged GTP, LLC, and RLC, Protected data
    MN_GPRS_RELIABILITY_UNACK_GTP_LLC_RLC_UNPROTECTED_DATA,   //Unacknowledged GTP, LLC, and RLC, Unprotected data
    MN_GPRS_RELIABILITY_UNACK_GTP_LLC_ACK_RLC_PROTECTED_DATA_SIX,//Unacknowledged GTP and LLC; Acknowledged RLC, Protected data reliability = 6
    MN_GPRS_RELIABILITY_RESERVED = 7
}MN_GPRS_RELIABILITY_TYPE_E;

typedef enum{
    MN_GPRS_DELAY_SUBSCRIBED =0,
    MN_GPRS_DELAY_CLASS1 = 1,
    MN_GPRS_DELAY_CLASS2,
    MN_GPRS_DELAY_CLASS3,
    MN_GPRS_DELAY_CLASS4, //best effort         
    //other values are interpreted as best effort
    MN_GPRS_DELAY_RESERVED = 7
}MN_GPRS_DELAY_CLASS_E;

typedef enum{
    MN_GPRS_PEAK_SUBSCRIBED = 0,
    MN_GPRS_PEAK_THROUGHPUT_1000 = 1,//Up to 1000   octet/s
    MN_GPRS_PEAK_THROUGHPUT_2000,    //Up to 2000   octet/s
    MN_GPRS_PEAK_THROUGHPUT_4000,    //Up to 4000   octet/s
    MN_GPRS_PEAK_THROUGHPUT_8000,    //Up to 8000   octet/s
    MN_GPRS_PEAK_THROUGHPUT_16000,   //Up to 16000  octet/s
    MN_GPRS_PEAK_THROUGHPUT_32000,   //Up to 32000  octet/s
    MN_GPRS_PEAK_THROUGHPUT_64000,   //Up to 64000  octet/s
    MN_GPRS_PEAK_THROUGHPUT_128000,  //Up to 128000 octet/s
    MN_GPRS_PEAK_THROUGHPUT_256000,  //Up to 256000 octet/s
    //other values are interpreted as Up to 1000 octets/s
    MN_GPRS_PEAK_RESERVED = 0xF        
}MN_GPRS_PEAK_THROUGHPUT_TYPE_E;

typedef enum{
    MN_GPRS_MEAN_SUBSCRIBED = 0,
    MN_GPRS_MEAN_THRPUT_1h = 1,//Up to 100   octet/h
    MN_GPRS_MEAN_THRPUT_2h,    //Up to 200   octet/h
    MN_GPRS_MEAN_THRPUT_5h,    //Up to 500   octet/h
    MN_GPRS_MEAN_THRPUT_1k,    //Up to 1000  octet/h
    MN_GPRS_MEAN_THRPUT_2k,    //Up to 2000  octet/h
    MN_GPRS_MEAN_THRPUT_5k,    //Up to 5000  octet/h
    MN_GPRS_MEAN_THRPUT_10k,   //Up to 10,000 octet/h
    MN_GPRS_MEAN_THRPUT_20k,   //Up to 20,000 octet/h
    MN_GPRS_MEAN_THRPUT_50k,   //Up to 50,000 octet/h
    MN_GPRS_MEAN_THRPUT_100k,  //Up to 100,000 octet/h
    MN_GPRS_MEAN_THRPUT_200k,  //Up to 200,000 octet/h
    MN_GPRS_MEAN_THRPUT_500k,  //Up to 500,000 octet/h
    MN_GPRS_MEAN_THRPUT_1m,    //Up to 1,000,000  octet/h
    MN_GPRS_MEAN_THRPUT_2m,    //Up to 2,000,000  octet/h
    MN_GPRS_MEAN_THRPUT_5m,    //Up to 5,000,000  octet/h
    MN_GPRS_MEAN_THRPUT_10m,   //Up to 10,000,000 octet/h
    MN_GPRS_MEAN_THRPUT_20m,   //Up to 20,000,000 octet/h
    MN_GPRS_MEAN_THRPUT_50m,   //Up to 50,000,000 octet/h
    MN_GPRS_MEAN_THRPUT_RESERVED   = 0x1E,
    MN_GPRS_MEAN_THRPUT_BESTEFFORT = 0x1F
    //other values are interpreted as best effort 
}MN_GPRS_MEAN_THROUGHPUT_TYPE_E;

typedef enum{
    MN_GPRS_PRECEDENCE_SUBSCRIBED =0,
    MN_GPRS_PRECEDENCE_HIGH = 1,
    MN_GPRS_PRECEDENCE_NORMAL,
    MN_GPRS_PRECEDENCE_LOW,
    //other values are interpreted as precedence normal priority
    MN_GPRS_PRECEDENCE_RESERVED = 7
}MN_GPRS_PRECEDENCE_CLASS_E;


typedef enum{
    MN_GPRS_DELIVERY_OF_ERR_SDU_SUBSCRIBED = 0,
    MN_GPRS_NW2MS_DELIVERY_OF_ERR_SDU_RESERVED = 0,
    MN_GPRS_DELIVERY_OF_ERR_SDU_NODETECT = 1,   //No detect('-')
    MN_GPRS_DELIVERY_OF_ERR_SDU_YES,            //Erroneous SDUs are delivered('yes')
    MN_GPRS_DELIVERY_OF_ERR_SDU_NO,             //Erroneous SDUs are not delivered('no')      
    //other values are consider as reserved
    MN_GPRS_DELIVERY_OF_ERR_SDU_RESERVED = 7,
    MN_GPRS_DELIVERY_NO_DETECT = MN_GPRS_DELIVERY_OF_ERR_SDU_NODETECT,
    MN_GPRS_DELIVERY_WITH_ERR_SDU = MN_GPRS_DELIVERY_OF_ERR_SDU_YES,
    MN_GPRS_DELIVERY_WITHOUT_ERR_SDU = MN_GPRS_DELIVERY_OF_ERR_SDU_NO
}MN_GPRS_DELIVERY_OF_ERR_SDU_E;

typedef enum{
    MN_GPRS_DELIVERY_ORDER_SUBSCRIBED = 0,
    MN_GPRS_NW2MS_DELIVERY_ORDER_RESERVED = 0,
    MN_GPRS_DELIVERY_ORDER_YES = 1,   //With delivery order('yes')
    MN_GPRS_DELIVERY_ORDER_NO,        //Without delivery order('no')
    MN_GPRS_DELIVERY_ORDER_RESERVED,
    MN_GPRS_DELIVERY_WITH_ORDER = MN_GPRS_DELIVERY_ORDER_YES,
    MN_GPRS_DELIVERY_WITHOUT_ORDER = MN_GPRS_DELIVERY_ORDER_NO
}MN_GPRS_DELIVERY_ORDER_E;

typedef enum{
    MN_GPRS_TRAFFIC_CLASS_SUBSCRIBED = 0,
    MN_GPRS_NW2MS_TRAFFIC_CLASS_RESERVED = 0,
    MN_GPRS_TRAFFIC_CLASS_CONVERSATIONAL = 1,   //Conversational class
    MN_GPRS_TRAFFIC_CLASS_STREAMING,            //Streaming class 
    MN_GPRS_TRAFFIC_CLASS_INTERACTIVE,          //Interactive class      
    MN_GPRS_TRAFFIC_CLASS_BACKGROUD,            //Background class
    //other values are consider as reserved
    MN_GPRS_TRAFFIC_CLASS_RESERVED = 7,
    MN_GPRS_CONVERSATIONAL_CLASS = MN_GPRS_TRAFFIC_CLASS_CONVERSATIONAL,
    MN_GPRS_STREAMING_CLASS = MN_GPRS_TRAFFIC_CLASS_STREAMING,
    MN_GPRS_INTERACTIVE_CLASS = MN_GPRS_TRAFFIC_CLASS_INTERACTIVE,
    MN_GPRS_BACKGROUND_CLASS = MN_GPRS_TRAFFIC_CLASS_BACKGROUD
}MN_GPRS_TRAFFIC_CLASS_E;

typedef enum{
    MN_GPRS_MAX_SDU_SIZE_SUBSCRIBED = 0,
    MN_GPRS_NW2MS_MAX_SDU_SIZE_RESERVED = 0,
    MN_GPRS_MAX_SDU_SIZE_10OCTETS = 1,          //10   octets
    MN_GPRS_MAX_SDU_SIZE_1500OCTETS = 150,      //1500 octets
    MN_GPRS_MAX_SDU_SIZE_1502OCTETS = 151,      //1502 octets  
    MN_GPRS_MAX_SDU_SIZE_1510OCTETS = 152,      //1510 octets 
    MN_GPRS_MAX_SDU_SIZE_1520OCTETS = 153,      //1520 octets
    //other values are consider as reserved
    SM_GPRS_MAX_SDU_SIZE_RESERVED = 255
}MN_GPRS_MAX_SDU_SIZE_E;

typedef enum{
    MN_GPRS_RESIDUAL_BER_SUBSCRIBED = 0,
    MN_GPRS_NW2MS_RESIDUAL_BER_RESERVED = 0,
    MN_GPRS_RESIDUAL_BER_5m = 1,       //5000 000e-8
    MN_GPRS_RESIDUAL_BER_1m,           //1000 000e-8
    MN_GPRS_RESIDUAL_BER_500k,         //500 000e-8 
    MN_GPRS_RESIDUAL_BER_400k,         //400 000e-8
    MN_GPRS_RESIDUAL_BER_100k,         //100 000e-8
    MN_GPRS_RESIDUAL_BER_10k,          //10 000e-8
    MN_GPRS_RESIDUAL_BER_1k,           //1000e-8
    MN_GPRS_RESIDUAL_BER_100,          //100e-8
    MN_GPRS_RESIDUAL_BER_6,            //6e-8 
    //other values are consider as reserved
    MN_GPRS_RESIDUAL_BER_RESERVED = 15,
    MN_GPRS_BIT_ERROR_RATE_SUBSCRIBED = MN_GPRS_RESIDUAL_BER_SUBSCRIBED,
    MN_GPRS_BIT_ERROR_RATE1, /* 5*10-2 */
    MN_GPRS_BIT_ERROR_RATE2, /* 1*10-2  */
    MN_GPRS_BIT_ERROR_RATE3, /* 5*10-3  */
    MN_GPRS_BIT_ERROR_RATE4, /* 4*10-3  */
    MN_GPRS_BIT_ERROR_RATE5, /* 1*10-3  */
    MN_GPRS_BIT_ERROR_RATE6, /* 1*10-4  */
    MN_GPRS_BIT_ERROR_RATE7, /* 1*10-5  */
    MN_GPRS_BIT_ERROR_RATE8, /* 1*10-6  */
    MN_GPRS_BIT_ERROR_RATE9, /* 6*10-8  */
    MN_GPRS_BIT_ERROR_RATE_RESERVED = MN_GPRS_RESIDUAL_BER_RESERVED
}MN_GPRS_RESIDUAL_BER_E, MN_GPRS_RESIDUAL_BIT_ERROE_RATE_E;

typedef enum{
    MN_GPRS_SDU_ERR_RATIO_SUBSCRIBED = 0,
    MN_GPRS_NW2MS_SDU_ERR_RATIO_RESERVED = 0,
    MN_GPRS_SDU_ERR_RATIO_10k = 1,     //10 000e-6
    MN_GPRS_SDU_ERR_RATIO_7k,          //7 000e-6
    MN_GPRS_SDU_ERR_RATIO_1k,          //1 000e-6 
    MN_GPRS_SDU_ERR_RATIO_100,         //100e-6
    MN_GPRS_SDU_ERR_RATIO_10,          //10e-6
    MN_GPRS_SDU_ERR_RATIO_1,           //1e-6
    MN_GPRS_SDU_ERR_RATIO_100k,        //100 000e-6
    //other values are consider as reserved
    MN_GPRS_SDU_ERR_RATIO_RESERVED = 15,
    MN_GPRS_SDU_ERR_RATIO1 = 1, /* 1*10-2  */
    MN_GPRS_SDU_ERR_RATIO2, /* 7*10-3  */
    MN_GPRS_SDU_ERR_RATIO3, /* 1*10-3  */
    MN_GPRS_SDU_ERR_RATIO4, /* 1*10-4  */
    MN_GPRS_SDU_ERR_RATIO5, /* 1*10-5  */
    MN_GPRS_SDU_ERR_RATIO6, /* 1*10-6  */
    MN_GPRS_SDU_ERR_RATIO7 /* 1*10-1  */
}MN_GPRS_SDU_ERR_RATIO_E;

typedef enum{
    MN_GPRS_MAX_BIT_RATE_UPLINK_SUBSCRIBED = 0,
    MN_GPRS_NW2MS_MAX_BIT_RATE_UPLINK_RESERVED = 0,
    MN_GPRS_MAX_BIT_RATE_UPLINK1 = 1,
    MN_GPRS_MAX_BIT_RATE_UPLINK2 = 63,
    MN_GPRS_MAX_BIT_RATE_UPLINK3 = 64,
    MN_GPRS_MAX_BIT_RATE_UPLINK4 = 127,
    MN_GPRS_MAX_BIT_RATE_UPLINK5 = 128,
    MN_GPRS_MAX_BIT_RATE_UPLINK6 = 254,
    MN_GPRS_MAX_BIT_RATE_UPLINK7 = 255    //0kpbs
}MN_GPRS_MAX_BIT_RATE_UPLINK_E;

typedef enum{
    MN_GPRS_MAX_BIT_RATE_DOWNLINK_SUBSCRIBED= 0,
    MN_GPRS_NW2MS_MAX_BIT_RATE_DOWNLINK_RESERVED = 0,
    MN_GPRS_MAX_BIT_RATE_DOWNLINK1 = 1,
    MN_GPRS_MAX_BIT_RATE_DOWNLINK2 = 63,
    MN_GPRS_MAX_BIT_RATE_DOWNLINK3 = 64,
    MN_GPRS_MAX_BIT_RATE_DOWNLINK4 = 127,
    MN_GPRS_MAX_BIT_RATE_DOWNLINK5 = 128,
    MN_GPRS_MAX_BIT_RATE_DOWNLINK6 = 254,
    MN_GPRS_MAX_BIT_RATE_DOWNLINK7 = 255  //0kpbs
}MN_GPRS_MAX_BIT_RATE_DOWNLINK_E;

typedef enum{
    MN_GPRS_TRANSFER_DELAY_SUBSCRIBED = 0,
    MN_GPRS_NW2MS_TRANSFER_DELAY_RESERVED = 0,   
    MN_GPRS_TRANSFER_DELAY1 = 1,
    MN_GPRS_TRANSFER_DELAY2 = 15,
    MN_GPRS_TRANSFER_DELAY3 = 16,
    MN_GPRS_TRANSFER_DELAY4 = 31,
    MN_GPRS_TRANSFER_DELAY5 = 32,
    MN_GPRS_TRANSFER_DELAY6 = 62,
    MN_GPRS_TRANSFER_DELAY_RESERVED = 63 
}MN_GPRS_TRANSFER_DELAY_E;

typedef enum{
    MN_GPRS_TRAFFIC_HNDL_PRIORITY_SUBSCRIBED = 0,
    MN_GPRS_NW2MS_TRAFFIC_HNDL_PRIORITY_RESERVED = 0,
    MN_GPRS_TRAFFIC_HNDL_PRIORITY1,
    MN_GPRS_TRAFFIC_HNDL_PRIORITY2,
    MN_GPRS_TRAFFIC_HNDL_PRIORITY3
}MN_GPRS_TRAFFIC_HNDL_PRIORITY_E;

typedef enum {
    MN_GPRS_GUARANTD_BIT_RATE_UPLINK_SUBSCRIBED = 0,
    MN_GPRS_NW2MS_GUARANTD_BIT_RATE_UPLINK_RESERVED = 0,
    MN_GPRS_GUARANTD_BIT_RATE_UPLINK1 = 1,
    MN_GPRS_GUARANTD_BIT_RATE_UPLINK2 = 63,
    MN_GPRS_GUARANTD_BIT_RATE_UPLINK3 = 64,
    MN_GPRS_GUARANTD_BIT_RATE_UPLINK4 = 127,
    MN_GPRS_GUARANTD_BIT_RATE_UPLINK5 = 128,
    MN_GPRS_GUARANTD_BIT_RATE_UPLINK6 = 254,
    MN_GPRS_GUARANTD_BIT_RATE_UPLINK7 = 255
}MN_GPRS_GUARANTD_BIT_RATE_UPLINK_E;

typedef enum {
    MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK_SUBSCRIBED = 0,
    MN_GPRS_NW2MS_GUARANTD_BIT_RATE_DOWNLINK_RESERVED = 0,
    MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK1 = 1,
    MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK2 = 63,
    MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK3 = 64,
    MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK4 = 127,
    MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK5 = 128,
    MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK6 = 254,
    MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK7 = 255
}MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK_E;
//#endif

typedef enum
{
    MN_CONTEXT_ACTIVATED        = 0,
    MN_CONTEXT_ACTIVATED_PENDING,
    MN_CONTEXT_DEACTIVATED_PENDING,
    MN_CONTEXT_DEACTIVATED,
    MN_CONTEXT_MODIFY_PENDING,
    MN_CONTEXT_NOT_USED = 0xff
}MN_GPRS_PDP_STATE_E;

//Packet filter type enumerations 
typedef enum
{
    MN_GPRS_PF_TYPE_ONE = 0,
    MN_GPRS_PF_TYPE_TWO,
    MN_GPRS_PF_TYPE_THREE,
    MN_GPRS_PF_TYPE_NONE
}MN_GPRS_PACKET_FILTER_TYPE_E;

//Packet filter component type enumerations
typedef enum
{
    MN_GPRS_PF_IPV4_SRC_ADDR_TYPE              = 0x10,
    MN_GPRS_PF_IPV6_SRC_ADDR_TYPE              = 0x20,
    MN_GPRS_PF_PROT_ID_NEXT_HDR_TYPE           = 0x30,
    MN_GPRS_PF_SINGLE_DST_PORT_TYPE            = 0x40,
    MN_GPRS_PF_DST_PORT_RANGE_TYPE             = 0x41,
    MN_GPRS_PF_SINGLE_SRC_PORT_TYPE            = 0x50,
    MN_GPRS_PF_SRC_PORT_RANGE_TYPE             = 0x51,
    MN_GPRS_PF_SECURITY_PARAM_INDX_TYPE        = 0x60,
    MN_GPRS_PF_TYPE_OF_SERV_TRAFFIC_CLASS_TYPE = 0x70,
    MN_GPRS_PF_FLOW_LABEL_TYPE                 = 0x80    
}MN_GPRS_PACKET_FILTER_COMP_TYPE_E;

typedef enum
{
    MN_GPRS_IPV4_ADDR_SBNT_MASK_LEN = 8,
    MN_GPRS_IPV6_ADDR_SBNT_MASK_LEN = 16
}MN_GPRS_ADDR_SBNT_MASK_LEN_E;

//define TFT operation code enumerations
typedef enum
{
    MN_GPRS_NO_OP_TFT = 0,
    MN_GPRS_CREATE_NEW_TFT,
    MN_GPRS_DELETE_EXISTING_TFT,
    MN_GPRS_ADD_PKT_FILTER_TO_TFT,
    MN_GPRS_REPLACE_PKT_FILTER_IN_TFT,
    MN_GPRS_DELETE_PKT_FILTER_FROM_TFT
}MN_GPRS_TFT_OPCODE_E;

//total qos definition
typedef struct
{
    /* R97/98 */
    MN_GPRS_RELIABILITY_TYPE_E            reliability;
    MN_GPRS_PEAK_THROUGHPUT_TYPE_E        peak_thrput;
    MN_GPRS_DELAY_CLASS_E                 delay_class;
    MN_GPRS_PRECEDENCE_CLASS_E            precedence;
    MN_GPRS_MEAN_THROUGHPUT_TYPE_E        mean_thrput;
    
    /* R99 */
    BOOLEAN                               is_r99;
    MN_GPRS_DELIVERY_OF_ERR_SDU_E         delivery_of_err_sdu;
    MN_GPRS_DELIVERY_ORDER_E              delivery_order;
    MN_GPRS_TRAFFIC_CLASS_E               traffic_class;
    MN_GPRS_MAX_SDU_SIZE_E                max_sdu_size;
    MN_GPRS_MAX_BIT_RATE_UPLINK_E         max_bit_rate_for_uplink;
    MN_GPRS_MAX_BIT_RATE_DOWNLINK_E       max_bit_rate_for_downlink;
    MN_GPRS_RESIDUAL_BER_E                residual_ber;
    MN_GPRS_SDU_ERR_RATIO_E               sdu_err_ratio;
    MN_GPRS_TRAFFIC_HNDL_PRIORITY_E       traffic_handling_prio;
    MN_GPRS_TRANSFER_DELAY_E              transfer_delay;
    MN_GPRS_GUARANTD_BIT_RATE_UPLINK_E    guaranteed_bit_rate_for_uplink;
    MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK_E  guaranteed_bit_rate_for_downlink;
} MN_GPRS_TOTAL_QOS_T;

typedef struct
{
    uint8                       pdp_id;                         //pdp id
    uint8                       delivery_order;/* ##CTC#delivery_order##Elist 
                                                     'SUBSCRIBED/RESERVED(NW2MS)' = 0,
                                                     'DELIVERY_ORDER1' = 1, 
                                                     'DELIVERY_ORDER2' = 2,
                                                     'RESERVED_BIDIRECT_DELIVERY_ORDER' = 3*/ 
    uint8                       delivery_of_err_sdu;/* ##CTC#delivery_of_err_sdu##Elist 
                                                           'SUBSCRIBED/RESERVED(NW2MS)' = 0,
                                                           'DELIVERY_OF_ERR_SDU1' = 1, 
                                                           'DELIVERY_OF_ERR_SDU2' = 2,
                                                           'DELIVERY_OF_ERR_SDU3' = 3, 
                                                           'RESERVED_BIDIRECT_DELIVERY_OF_ERR_SDU' =7*/ 
    uint8                       traffic_handling_priority;/* ##CTC#traffic_handling_priority##Elist 
                                                                   'SUBSCRIBED/RESERVED(NW2MS)' = 0,
                                                                   'PRIORITY1' = 1,
                                                                   'PRIORITY2' = 2,
                                                                   'PRIORITY3' = 3*/
    MN_GPRS_TRAFFIC_CLASS_E     traffic_class;                  
    uint16                      max_bt_ul;/* ##CTC#max_bt_ul##Elist 
                                                'Subscribed max_bt_ul' = 0, 
                                                '8kbps' = 8, 
                                                '16kbps' = 16, 
                                                '32kbps' = 32, 
                                                '64kbps' = 64, 
                                                '128kbps' = 72, 
                                                '256kbps' = 88, 
                                                '384kbps' = 104, 
                                                '512kbps' = 120,
                                                '1024kbps' = 135,
                                                '2048kbps' = 151, 
                                                'Max Speed more than 8640kbps' = 254, 
                                                '0.0kbps' = 255*/
    uint16                      guaranteed_bt_ul;
    uint16                      max_bt_dl;/* ##CTC#max_bt_dl##Elist 
                                                'Subscribed max_bt_dl' = 0, 
                                                '64kbps' = 64, 
                                                '128kbps' = 72, 
                                                '256kbps' = 88,
                                                '384kbps' = 104,
                                                '512kbps' = 120,
                                                '1024kbps' = 135, 
                                                '2048kbps' = 151,
                                                '2624kbps' = 160,
                                                '2752kbps' = 162, 
                                                '2816kbps' = 163, 
                                                'Max Speed more than 8640kbps' = 254,
                                                '0.0kbps' = 255*/
    uint16                      guaranteed_bt_dl;
    uint32                      max_sdu_size; /* ##CTC#max_sdu_size##Elist 
                                                     'Subscribed maximum SDU size' = 0, 
                                                     '1502 octets' = 151, 
                                                     '1510 octets' = 152, 
                                                     '1520 octets' = 153, 
                                                     'Reserved' = 255*/
    uint8                       sdu_error_ratio[2];/* ##CTC#sdu_error_ratio##Arrlist 
                                                          [0] = { 'SUBSCRIBED/RESERVED(NW2MS)' = 0,
                                                                  'SDU_ERR_RATIO1' = 1, 
                                                                  'SDU_ERR_RATIO2' = 2,
                                                                  'SDU_ERR_RATIO3' = 3, 
                                                                  'SDU_ERR_RATIO4' =4,
                                                                  'SDU_ERR_RATIO5' = 5, 
                                                                  'SDU_ERR_RATIO6' = 6,
                                                                  'SDU_ERR_RATIO7' = 7, 
                                                                  'RESERVED_BIDIRECT_SDU_ERR_RATIO' =15 },*/
    uint8                       residual_ber[2];/* ##CTC#residual_ber##Arrlist 
                                                          [0] = { 'SUBSCRIBED/RESERVED(NW2MS)' = 0,
                                                                  'BER_RATE1' = 1, 
                                                                  'BER_RATE2' = 2,
                                                                  'BER_RATE3' = 3, 
                                                                  'BER_RATE4' =4,
                                                                  'BER_RATE5' = 5,
                                                                  'BER_RATE6' = 6,
                                                                  'BER_RATE7' = 7,
                                                                  'BER_RATE8' =8,
                                                                  'BER_RATE9' = 9, 
                                                                  'RESERVED_BIDIRECT_BIT_ERROR_RATE' },*/
    uint16                      transfer_delay;/* ##CTC#transfer_delay##Elist 
                                                      'Subscribed transfer delay' = 0,
                                                      '10ms' = 1,
                                                      '150ms' = 15,
                                                      '200ms' = 16,
                                                      '950ms' = 31,
                                                      '1000ms' = 32,
                                                      '4000 ms' = 62,
                                                      'Reserved' = 63 */ 
    uint8                       signal_indication; /* ##CTC#signal_indication##Elist 
                                                           'NOT_OPTIMISED_FOR_SIGNALLING_TRAFFIC' = 0,
                                                           'OPTIMISED_FOR_SIGNALLING_TRAFFIC' = 1*/ 
    uint8                       src_statistics_descriptor;/* ##CTC#src_statistics_descriptor##Elist 
                                                                   'UNKNOWN' = 0,'SPEECH' = 1*/ 
    uint16                      extended_max_bitrate_downlnk; 
    uint16                      extended_guarntd_bitrate_downlnk;
}MNGPRS_3G_EXTEND_QOS_SET_T, MN_GPRS_EXT_QOS_INFO_T;

typedef struct
{
    MN_GPRS_ERR_E          result;
    uint8                  qos_num;
    MN_GPRS_EXT_QOS_INFO_T qos_info[ MN_GPRS_MAX_PDP_CONTEXT_COUNT ];
} MN_GPRS_EXT_QOS_T;

/* +CR158261 */
typedef struct {   
    MN_GPRS_PACKET_FILTER_TYPE_E     type;
    uint8                            pdp_id;
    uint8                            pfi;        //Packet filter identifier
    uint8                            epi;        //Packet filter evaluation procedence identifier
    uint8                            src_addr_sbnet_mask_len;
    uint8                            src_addr_sbnet_mask[MN_GPRS_MAX_SRC_ADDR_SBNET_MASK_LEN];
    uint16                           prot_number_next_hdr;
    uint8                            src_port_len;
    uint8                            dst_port_len;
    uint16                           dst_port_range[MN_GPRS_DST_PORT_RANGE_LEN];
    uint16                           src_port_range[MN_GPRS_SRC_PORT_RANGE_LEN];
    uint8                            tos_len;
    uint8                            tos_traffic_class_mask[MN_GPRS_TOS_TRAF_CLASS_MASK_LEN];
    BOOLEAN                          ipsec_spi_present_flag;
    uint32                           ipsec_spi;
    uint32                           flow_label;
}  MN_GPRS_PACKET_FILTER_INFO_T;

typedef struct {
    uint8                         pfi_num;
    MN_GPRS_PACKET_FILTER_INFO_T *pfi_info;
} MN_GPRS_TFT_INFO_T;

typedef struct
{
    /* If result is NOT MN_GPRS_ERR_SUCCESS, 
     * all tft_info[] are set to NULL.
     */
    MN_GPRS_ERR_E       result;
    
    /* If no tft info in PDP, tft_info[pdp_id] is 
     * equal to NULL. Application shall release dynamical 
     * memory when tft_info[pdp_id]'s being not NULL. */
    MN_GPRS_TFT_INFO_T *tft_info[ MN_GPRS_MAX_PDP_CONTEXT_COUNT ];
} MN_GPRS_TFT_T;
/* -CR158261 */

typedef struct
{
  uint8 pdp_id;
  uint8 primary_pdp_id;
  uint8 dcomp_algo;
  uint8 pcomp_algo;
} MN_GPRS_SEC_PDP_CONTEXT_INFO_T;

typedef struct
{
    MN_GPRS_ERR_E                  result;
    uint8                          sec_pdp_num;
    MN_GPRS_SEC_PDP_CONTEXT_INFO_T sec_pdp_info[ MN_GPRS_MAX_PDP_CONTEXT_COUNT ];
} MN_GPRS_SEC_PDP_CONTEXT_T;


typedef struct
{
    uint32     pdp_id;
    MN_GPRS_TOTAL_QOS_T  qos;
}MN_GPRS_OUT_QOS_T;

typedef struct
{
    int16   length;                           //the valid length of the value string
    uint8   value_arr[MN_GPRS_MAX_ADDR_LEN];//a string parameter that identifies the MT in the address space applicable to the PDP
} MN_GPRS_PDP_ADDR_T;

typedef struct
{
	int16   length;                           
	uint8 	value_arr[MN_GPRS_MAX_APN_LEN];
} MN_GPRS_PDP_APN_T;

typedef struct
{
    uint32     pdp_id;
    MN_GPRS_PDP_STATE_E    pdp_state;
    
}MN_GPRS_CON_STATE_T;

typedef enum
{
    MN_PAP       = 0,
    MN_CHAP      = 1,
    MN_PAP_CHAP  = 2,
    MN_AUTH_NONE = 3
} MN_PCO_AUTH_TYPE_E;

typedef struct {
   
    uint32     pdp_id; 
    BOOLEAN    is_valid;
    uint8      user[MN_GPRS_USER_MAX_LENGTH+1];
    uint8      passwd[MN_GPRS_PASSWD_MAX_LENGTH+1];
    MN_PCO_AUTH_TYPE_E auth_type;
} MN_GPRS_PCO_T;

typedef struct MNGPRS_LLC_XID_PARAM_T_s
{
    BOOLEAN version_flag;
    BOOLEAN iov_i_flag;
    BOOLEAN t200_flag;
    BOOLEAN n200_flag;
    BOOLEAN n201_u_flag;
    BOOLEAN n201_i_flag;
    BOOLEAN md_flag;
    BOOLEAN mu_flag;
    BOOLEAN kd_flag;
    BOOLEAN ku_flag;

    uint32 iov_i;
    uint16 t200;
    uint16 n201_u;
    uint16 n201_i;
    uint16 md;
    uint16 mu;
    uint8   version;
    uint8   kd;
    uint8   ku;
    uint8   n200;
} MNGPRS_LLC_XID_PARAM_T;


typedef struct
{
    uint32                  pdp_id;     // 0,indicate it active by network
    uint8                   pdp_addr_type[MN_GPRS_MAX_ADDR_TYPE_LEN];
    uint8                   apn[MN_GPRS_MAX_APN_LEN];
    MN_GPRS_PDP_ADDR_T      pdp_addr_str;
    uint32                  d_comp;
    uint32                  h_comp;
}MN_GPRS_PDP_CON_T;

typedef struct {
    MN_GPRS_EVENT_REPORT_MODE_E     mt_event_report_mode;
    MN_GPRS_EVENT_REPORT_BUFFER_E   mt_event_report_buf;
}MN_GPRS_EVENT_REPROT_INFO_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPRS_ERR_E result;
    MN_DUAL_SYS_E dual_sys;
}APP_MN_GPRS_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPRS_ERR_E result;
    MN_DUAL_SYS_E dual_sys;
    uint8         pdp_id;
    uint8         nsapi;
} APP_MN_GPRS_EXT_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPRS_ERR_E result;
    MN_DUAL_SYS_E dual_sys;
    uint8         pdp_num;
    uint8         pdp_id[MN_GPRS_MAX_PDP_CONTEXT_COUNT];
} APP_MN_GPRS_DEACTIVATE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPRS_ERR_E result; /* cause */
    MN_DUAL_SYS_E dual_sys;
    uint8         pdp_num;
    uint8         pdp_id[MN_GPRS_MAX_PDP_CONTEXT_COUNT];
} APP_MN_GPRS_ACTIVATE_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPRS_ERR_E result; /* cause */
    MN_DUAL_SYS_E dual_sys;
    uint8          pdp_num;
    uint8         pdp_id[MN_GPRS_MAX_PDP_CONTEXT_COUNT];
} APP_MN_GPRS_DEACTIVATE_IND_T;

typedef struct
{
    SIGNAL_VARS
    uint16         nsapi;
} APP_MN_GPRS_RESUME_IND_SIG;

// the callback function for GPRS user when receiving data
typedef int32 (* MNGPRS_RECEIVEDATA_FPTR)(
        uint8* buff_ptr,	// pointer of received data buff
		uint32 buff_size,	// received data length
		uint8  nsapi
		);

// the callback function for GPRS user when receiving data
typedef int32 (* MNGPRS_RECEIVEDATA_INDICATION_FPTR)(
		uint32 buff_size,	// received data length
		uint8  nsapi
		);
		
//add for GPRS flow control 2003-12-29 
typedef void (* MNGPRS_FLOWCONTROL_FPTR)(uint8 nsapi);

typedef struct ppp_active_pdp_cfg
{
    SIGNAL_VARS
    uint32     pid;
    char *     username;
    char *    password;
    MN_PCO_AUTH_TYPE_E   auth_type;
    int          user_len;
    int          pw_len;
    char *    challenge;
    char *    chall_name;
    uint8 challenge_len;
    uint8 chall_name_len;
}PPP_ACTIVE_PDP_CFG_T;


 /*****************************************************************************
 **                        SMS MODULE DEFINE                                  *
 ******************************************************************************/
#define MN_SMS_IN_SIM_MAX_NUM         250       // the max SMS num in SIM card 
#define MN_SMS_STATUS_REPORT_LEN    30
#define MN_SMS_IN_NV_MAX_NUM        200      // the max SMS num in NV 
#define MN_SMS_VP_MAX_LEN           7        // the max validity period length 
#define MN_SMS_PARAM_MAX_NUM        10       // the max SMS parameter in SIM card
#define MN_SMS_TIME_STAMP_LEN       7        // the time stamp length
#define MN_SMS_CONCATENATED_MAX_NUM 255      // Maximum number of short messages in the concatenated short message

#define MN_SMS_USER_DATA_LENGTH     140      // the SMS user data length before decode or after encode
#define MN_SMS_MAX_USER_VALID_DATA_LENGTH       160      // the SMS user data length before decode or after encode
#define MN_SMS_HEAD_WIRL_CNTL_MESG_LEN			138
#define MN_SMS_HEAD_MAX_USER_DEF_SOUND_LEN		137
#define MN_SMS_HEAD_MAX_LARGE_ANI_PDU_LEN		137
#define MN_SMS_HEAD_MAX_SMALL_ANI_PDU_LEN		137
#define MN_SMS_HEAD_MAX_LARGE_PICT_PDU_LEN		137
#define MN_SMS_HEAD_MAX_SMALL_PICT_PDU_LEN		137
#define MN_SMS_HEAD_MAX_VAR_PICT_PDU_LEN		135
#define MN_SMS_HEAD_MAX_WCMP_PDU_LEN			138
#define MN_SMS_HEAD_MAX_WVG_STR_LEN				137
#define MN_SMS_HEAD_MAX_EXT_OBJ_LEN				131
#define MN_SMS_HEAD_MAX_CMPR_DATA_LEN			135
#define MN_SMS_HEAD_MAX_RELY_ADDR_LEN			138


#define MN_SMS_MAX_USER_HEADER_LENGTH   40       // the max length of user header data


#define MN_SMS_MAX_SUPPORT_LANG_LEN 100
#define MN_SMS_ASSERT_DEST_ADDR_LEN_MAX 8
#define MN_SMS_MAX_IEI_NUM  10

#define MN_SMS_NV_ADDR_LEN             12
#define MN_SMS_NV_STATUS_REPORT_LEN    30
#define MN_SMS_NV_MAX_SMS_TPDU_LEN          175
#define MN_SS_MAX_SIA_BCD_LEN           40


typedef uint8 MN_SMS_TIME_STAMP_ARR_T [ MN_SMS_TIME_STAMP_LEN ]; // the time stamp array
typedef uint8 MN_SMS_ASSERT_DEST_ADDR_ARR_T [MN_SMS_ASSERT_DEST_ADDR_LEN_MAX]; // assert SMS destination address

typedef uint16 MN_SMS_RECORD_ID_T;                                // the SMS position 
typedef uint8 MN_SMS_TP_MR_T;                                    // the message reference     

typedef uint8 MN_SMS_RELATIVE_FORMAT_VP_T;                       // the Relative Format Validity Period type       

typedef uint32 MN_SMS_TIME_SECOND_T;                              // the time express by second

typedef struct
{
    uint8   tp_mr;    
    BOOLEAN   memory_exceed_flag;                                 
}MN_SMS_SMSS_T;

typedef enum _MN_SMS_ERR_CODE_E
{
    MN_SMS_NO_ERR,              // API run successfully, no error 
    MN_SMS_NO_SC_ADDRESS,       // API run failure, user must write default SC_Address or input SC_Address while send message
    MN_SMS_NO_DEST_ADDRESS,     // API run failure, user must input default destination_address or input destination address while send message
    MN_SMS_NO_PID,              // API run failure, user must input default Protocol_indentify or input Protocol_indentify while send message
    MN_SMS_NO_VAILID_TIME,      // API run failure, user must input default valid_time or input valid_time while send message
    MN_SMS_WRITE_ME_FAILURE     // API run failure, the operation that user write default parameter value to ME is failure
}MN_SMS_ERR_CODE_E;

typedef enum _MN_SMS_STORAGE_E  
{
    MN_SMS_NO_STORED,       // the message is not stored in storage
    MN_SMS_STORAGE_ME,      // The message memory storage is "ME"
    MN_SMS_STORAGE_SIM,     // The message memory storage is "SIM"
    MN_SMS_STORAGE_ALL
} MN_SMS_STORAGE_E;

/*
 the protocol identify enum
*/

typedef enum _MN_SMS_PID_E
{
    MN_SMS_PID_DEFAULT_TYPE = 0x00,
    MN_SMS_PID_TELEX_TYPE   = 0x21, // bit7,bit6:0 0; bit 5: telematic interworking;bit4...0: device type
    MN_SMS_PID_GROUP3_FAX   = 0x22, 
    MN_SMS_PID_GROUP4_FAX   = 0x23, 
    MN_SMS_PID_VOICE_TYPE   = 0x24, 
    MN_SMS_PID_ERMES_TYPE   = 0x25,
    MN_SMS_PID_PAGING_TYPE  = 0x26,
    MN_SMS_PID_VIDEOTEX_TYPE= 0x27,
    MN_SMS_PID_TELETEX_TYPE = 0x28,
    MN_SMS_PID_UCI_TYPE     = 0x29,    // Universal Computer Interface
    MN_SMS_PID_X400_TYPE    = 0x31,
    MN_SMS_PID_EMAIL_TYPE   = 0x32, // bit7,bit6:0 0; bit 5: telematic interworking;bit4...0: device type
    MN_SMS_PID_ANSI_136_PID = 0x7C,
    MN_SMS_PID_SIM_DOWNLOAD = 0x7F,
    MN_SMS_PID_TELEMATIC_INTW = 0x20,
    MN_SMS_PID_TYPE_ZERO    = 0x40,
    MN_SMS_PID_REPLACE_ONE  = 0x41,
    MN_SMS_PID_REPLACE_TWO  = 0x42,
    MN_SMS_PID_REPLACE_THR  = 0x43,
    MN_SMS_PID_REPLACE_FOU  = 0x44,
    MN_SMS_PID_REPLACE_FIV  = 0x45,
    MN_SMS_PID_REPLACE_SIX  = 0x46,
    MN_SMS_PID_REPLACE_SEV  = 0x47,
    MN_SMS_PID_RETURN_CALL  = 0x5F
} MN_SMS_PID_E;

/*
 the tp status enum
*/
typedef enum _MN_SMS_TP_STATUS_E
{
    MN_SMS_RECEIVED_BY_SME,         // the SMS receive by SME successfully
    MN_SMS_NOT_CONFIRM_RECEIVED,    // the short message is forwarded by the SC to the SME but the SC is unable to confirm delivery
    MN_SMS_SM_REPLACED_BY_SC,       // short message replaced by the SC
    MN_SMS_SEND_LATER,              // temporary error happened while forward short message, transfer it again later
    MN_SMS_NOT_SEND_AGAIN,          // permanent error happened while forward short message, SC is not making any more transfer attempts
    MN_SMS_INVALID_STATUS_REPORT    // the invalid status report 
} MN_SMS_TP_STATUS_E;


/*
 the SMS operate cause
*/
typedef enum _MN_SMS_CAUSE_E
{
    MN_SMS_OPERATE_SUCCESS      ,   // operate right
    MN_SMS_UNSPEC_ERROR         ,   // the unspecific error 
    MN_SMS_SEND_FAILURE         ,   // send fail
    MN_SMS_DEFAULT_PARAM_ERROR  ,   // the default param which uplayer want to use not set
    MN_SMS_MN_BUSY              ,   // the MN is busy
    MN_SMS_WRITE_NV_FAILED      ,   // write NV failed
    MN_SMS_ME_MEM_FULL          ,   // the me memory is full
    MN_SMS_ME_NOT_SUPPORT       ,   // save in me not support
    MN_SMS_ME_DELETE_FAILURE    ,   // delete SMS in ME failed
    MN_SMS_SIM_NOT_READY        ,   // the SIM card is not ready
    MN_SMS_SIM_MEM_FULL         ,   // the SIM memory is full
    MN_SMS_SIM_PATTERN_NOT_FIND ,   // can not find any record that fulfill the condition
    MN_SMS_SIM_SEEK_FAILED      ,   // seek Failed
    MN_SMS_SIM_ACCESS_FAILED    ,   // do not have right to do access the SIM card file
    MN_SMS_SIM_READ_FAILED      ,   // read SIM file error
    MN_SMS_SIM_SELECT_FAILED    ,   // select SIM file error
    MN_SMS_SIM_UPDATE_FAILED    ,   // update SIM file error
    MN_SMS_SIM_CANNOT_UPDATE    ,   // the SIM file cannot be update (ADM)
    MN_SMS_SIM_PURGE_FAILED     ,   // purge EF_SMSR failed
    MN_SMS_SIM_SERVICE_DISABLED ,   // The corresponding service is not Enabled.
    MN_SMS_SIM_DELETE_FAILURE   ,   // delete SMS in SIM failed
    MN_SMS_RECORD_OUT_OF_RANGE  ,   // record id out of range
    MN_SMS_NOT_RIGHT_STORAGE    ,   // the storage is not right
    MN_SMS_SIM_SAVE_STATUS_FREE ,   // save long SMS in SIM, return save ok, but read status free space
    MN_SMS_SIM_STK_NOT_ALLOWED,
    MN_SMS_SIM_UNKNOW_FAILED,
    MN_SMS_SIM_FDN_FAILED,           /* cause of FDN check failure */
    MN_SMS_CANCEL_SUCCESS,           /* cause of cancel mo sms success*/
    MN_SMS_CANCEL_FAILED             /* cause of cancel mo sms failed*/
} MN_SMS_CAUSE_E;

typedef enum _MN_SMS_MO_FAIL_CAUSE_E
{
    MN_SMS_CAUSE_NO_VALUE                 = 0,
    
    // GSM 04.11 Annex E
    // E-2: RP-cause definition mobile originating SM-transfer
    MN_SMS_CAUSE_UNASSIGNED_NO            = 1,  // Unassigned (unallocated) number
                                                // the destination requested by the Mobile Station cannot be reached
                                                // because, although the number is in a valid format, it is not
                                                // currently assigned (allocated).

    MN_SMS_CAUSE_OPER_DETERM_BARRING      = 8,  // Operator determined barring
                                                // the MS has tried to send a mobile originating short message when
                                                // the MS's network operator or service provider has forbidden such
                                                // transactions.

    MN_SMS_CAUSE_CALL_BARRED              = 10, // Call barred
                                                // the outgoing call barred service applies to the short message
                                                // service for the called destination.

    MN_SMS_CAUSE_SMS_TRANSFER_REJECTED    = 21, // Short message transfer rejected
                                                // the equipment sending this cause does not wish to accept this short
                                                // message, although it could have accepted the short message since the 
                                                // equipment sending this cause is neither busy nor incompatible.

    MN_SMS_CAUSE_DEST_OUT_OF_ORDER        = 27, // Destination out of service
                                                // the destination indicated by the Mobile Station cannot be reached 
                                                // because the interface to the destination is not functioning correctly.
    MN_SMS_CAUSE_UNIDEN_SUBSCRIBED        = 28, // Unidentified subscriber
                                                // the subscriber is not registered in the PLMN.
    MN_SMS_CAUSE_FACILITY_REJECTED        = 29, // Facility rejected
                                                // the facility requested by the Mobile Station is not supported by the
                                                // PLMN.
    MN_SMS_CAUSE_UNKN_SUBSCRIBER          = 30, // Unknown subscriber
                                                // the subscriber is not registered in the HLR.

    MN_SMS_CAUSE_NET_OUT_OF_ORDER         = 38, // Network out of order
                                                // The network is not functioning correctly and the condition is likely
                                                // to last a relatively long period of time.
    MN_SMS_CAUSE_TEMP_FAILURE             = 41, // Temporary failure
                                                // the network is not functioning correctly and the condition is not
                                                // likely to last a long period of time.
    MN_SMS_CAUSE_CONGESTION               = 42, // Congestion
                                                // the short message service cannot be serviced because of high traffic.

    MN_SMS_CAUSE_RESOURCES_UNAV           = 47, // Resources unavailable, unspecified
                                                // this cause is used to report a resource unavailable event only when no
                                                // other cause applies.
    //MN_SMS_CAUSE_QOS_UNAV                 = 49,

    MN_SMS_CAUSE_REQ_FAC_NOT_SUBSCR       = 50, // Requested facility not subscribed
                                                // the requested short message service could not be provided by the 
                                                // network because the user has not completed the necessary administrative
                                                // arrangements with its supporting networks.

    MN_SMS_CAUSE_REQ_FACIL_NOT_IMPL       = 69, // Requested facility not implemented
                                                // the network is unable to provide the requested short message service.

    MN_SMS_CAUSE_INVALID_SMS_TRANS_REF_VAL= 81, // Invalid short message transfer reference value
                                                // the equipment sending this cause has received a message with a short 
                                                // message reference which is not currently in use on the MS-network
                                                // interface.

    MN_SMS_CAUSE_INVALID_MSG_UNSPECIFIED  = 95, // Invalid message, unspecified
                                                // this cause is used to report an invalid message event only when no 
                                                // other cause in the invalid message class applies.
    MN_SMS_CAUSE_MAND_IE_ERROR            = 96, // Invalid mandatory information
                                                // the equipment sending this cause has received a message where a
                                                // mandatory information element is missing and/or has a content error.
    MN_SMS_CAUSE_MSG_NONEXISTENT          = 97, // Message type non-existent or not implemented
                                                // the equipment sending this cause has received a message with a message
                                                // type it does not recognize either because this is a message not 
                                                // defined or defined but not implemented by the equipment sending this
                                                // cause.
    MN_SMS_CAUSE_MSG_GEN_ERROR            = 98, // Message not compatible with short message protocol state
                                                // the equipment sending this cause has received a message such that the
                                                // procedureds do not indicate that this is a permissible message to 
                                                // receive while in the short message transfer state.
    MN_SMS_CAUSE_IE_NONEXISTENT           = 99, // Information element non-existent or not implement
                                                // the equipment sending this cause has received a message which includes
                                                // information elements not recognized because the information element
                                                // identifier is not defined or it is defined but not implemented by the
                                                // equipment sending the cause.

    MN_SMS_CAUSE_PROTOCOL_ERROR           = 111,// Protocol error, unspecified
                                                // this cause is used to report a protocol error event only when no other
                                                // cause applies.
    MN_SMS_CAUSE_INTERWORKING             = 127,// Interworking, unspecified 
                                                // there has been interworking with a network which does not provide causes
                                                // for actions it takes; thus, the precise cause for a message which is 
                                                // being send cannot be ascertained.
    
    //for ps error:ti unavailable,invalid smi and memory request unacceptable.
    
    MN_SMS_CAUSE_PS_ERROR                 = 200,
    
    MN_SMS_CAUSE_SIM_NOT_READY,
    
    MN_SMS_CAUSE_PLMN_NO_SER,
    
    MN_SMS_CAUSE_MN_BUSY,
    
    MN_SMS_CAUSE_CANCEL_BY_USER
  
    
} MN_SMS_MO_FAIL_CAUSE_E;


/*
the SMS mem full status
*/
typedef enum _MN_SMS_MEM_FULL_STATUS_E
{
    MN_SMS_ME_FULL ,                // me memory full
    MN_SMS_SIM_FULL ,               // SIM memory full
    MN_SMS_ALL_FULL ,               // all memory full
    MN_SMS_MEM_AVAIL                // memory available
} MN_SMS_MEM_FULL_STATUS_E;

#ifdef GSM_CUSTOMER_AFP_SUPPORT
/* indicating the event when the memory of SMS module is full */
typedef enum _MN_SMS_MEM_FULL_EVENT_E
{
    MFE_NEW_SMS_INCOMING,           //new sms is incoming
    MFE_WRITTING_SMS,               //writting sms
    MFE_GETTING_SMS_NUM,            //getting sms number
    MFE_MAX
} MN_SMS_MEM_FULL_EVENT_E;
#endif

/*
 the SMS status in SIM
*/
typedef enum _MN_SMS_STATUS_E
{
    MN_SMS_FREE_SPACE ,                   // free space
    MN_SMS_MT_READED ,                    // MT SMS , message read
    MN_SMS_MT_TO_BE_READ ,                // MT SMS , message to be read
    MN_SMS_MO_TO_BE_SEND ,                // MO SMS , message to be send
    MN_SMS_MO_SR_NOT_REQUEST ,            // MO SMS , status report not requested 
    MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED , // MO SMS , status report request but not received
    MN_SMS_MO_SR_RECEIVED_NOT_STORE ,     // MO SMS , status report request,received but not store in SIM card
    MN_SMS_MO_SR_RECEIVED_AND_STORED ,    // MO SMS , status report request,received and stored in SIM card
    MN_SMS_MT_SR_READED ,                 // status report received and read
    MN_SMS_MT_SR_TO_BE_READ               // status report received to be read
} MN_SMS_STATUS_E;


typedef enum _MN_SMS_DELETE_ALL_STATUS_E
{
    MN_SMS_MT_READ,     // MT SMS, message readed
    MN_SMS_MT_UNREAD,   // MT SMS, message unreaded
    MN_SMS_MO_SENT,     // MO SMS, message send success
    MN_SMS_MO_UNSENT,   // MO SMS, message send fail 
    MN_SMS_ALL_MT,      // all MT SMS 
    MN_SMS_ALL_MO,      // all MO SMS
    MN_SMS_ALL_STATUS,  // all SMS
    MN_SMS_NO_STATUS    
} MN_SMS_DELETE_ALL_STATUS_E;

typedef struct 
{
    MN_SMS_STATUS_E     *mn_sms_status_arr_e_ptr;
    uint16              mn_sms_in_sim_nv_max_num;
} MN_SMS_STATUS_ARR_E;

/*
 the validity period format enum
*/
typedef enum  _MN_SMS_TP_VPF_E
{
    MN_SMS_TP_VPF_NOT_PRESENT     = 0 ,   // the validity period field not present 
    MN_SMS_TP_VPF_ENHANCE_FORMAT  = 1 ,   // the validity period field present--enhance format
    MN_SMS_TP_VPF_RELATIVE_FORMAT = 2 ,   // the validity period field present--relative format
    MN_SMS_TP_VPF_ABSOLUTE_FORMAT = 3     // the validity period field present--absolute format
} MN_SMS_TP_VPF_E;

/*
 the alphabet type being used
*/
typedef enum _MN_SMS_ALPHABET_TYPE_E
{
    MN_SMS_DEFAULT_ALPHABET = 0 ,        // default alphabet
    MN_SMS_8_BIT_ALPHBET        ,        // 8 bit data
    MN_SMS_UCS2_ALPHABET        ,        // UCS2(16bit) 
    MN_SMS_RESERVED_ALPHABET             // reserved
} MN_SMS_ALPHABET_TYPE_E;


/*
 the command type enum
*/
typedef enum _MN_SMS_COMMAND_TYPE_E
{
    MN_SMS_ENQUIRY_COMMAND,              // Enquiry relating to previously submitted SMS
    MN_SMS_CANCEL_STATUS_REPORT_COMMAND, // Cancel Status Report Request relating to 
                                         // previously submitted SMS
    MN_SMS_DELETE_COMMAND,               // Delete previously submitted SMS
    MN_SMS_ENABLE_STATUS_REPORT_COMMAND  // Enable Status Report Request relating to
                                         // previously submitted SMS
} MN_SMS_COMMAND_TYPE_E;

/*
 the SMS type enum
*/
typedef enum _MN_SMS_TYPE_E
{
    MN_SMS_TYPE_MO ,                     // the MO type SMS 
    MN_SMS_TYPE_MT                       // the MT type SMS 
} MN_SMS_TYPE_E;


/*
the SMS path enum
*/
typedef enum _MN_SMS_PATH_E
{
    MN_SMS_GSM_PATH,              // The GSM path
    MN_SMS_GPRS_PATH              // The GPRS path
} MN_SMS_PATH_E;

/*
the SMS user data head iei enum
*/
typedef enum _MN_SMS_IEI_ID_E
{
    MN_SMS_CONCAT_8_BIT_REF_NUM = 0x00,         //0x00,concatenated short message, 8-bit reference number
    MN_SMS_SPEC_MESS_IND,                       //0x01,special SMS message indication
    MN_SMS_NOT_AVOID_MISINTERPRETATION_LF = 0x03,//0x03,value not used to avoid misinterpretation as <LF> character
    MN_SMS_APP_PORT_8_BIT_ADDR,                 //0x04,Application port addressing scheme, 8 bit address
    MN_SMS_APP_PORT_16_BIT_ADDR,                //0x05,Application port addressing scheme, 16 bit address
    MN_SMS_SMSC_CNTL_PARAM,                     //0x06,SMSC Control Parameters
    MN_SMS_UDH_SOUR_IND,                        //0x07,UDH Source Indicator
    MN_SMS_CONCAT_16_BIT_REF_NUM,               //0x08,concatenated short message, 16-bit reference number
    MN_SMS_WIRELESS_CNTL_MESS_PROTOCOL,         //0x09,Wireless control Message Protocol
    MN_SMS_TEXT_FORMATTING,						//0xA,Text Formatting
    MN_SMS_PREDEF_SOUND,						//0xB,Predefined sound
    MN_SMS_USER_DEF_SOUND,						//0xC,User Defined Sound
    MN_SMS_PREDEF_ANIMATION,					//0xD,Predefined Animation
    MN_SMS_LARGE_ANIMATION,						//0xE,Large Animation
    MN_SMS_SMALL_ANIMATION,						//0xF,Small Animation
    MN_SMS_LARGE_PICTURE,						//0x10,Large Picture
    MN_SMS_SMALL_PICTURE,						//0x11,Small Picture
    MN_SMS_VAR_PICTURE,							//0x12,Variable Picture
    MN_SMS_USER_PROMPT_IND,						//0x13,User prompt indicator
    MN_SMS_EXT_OBJECT,							//0x14,Extended Object
    MN_SMS_REUSED_EXT_OBJECT,					//0x15,Reused Extended Object
    MN_SMS_COMPRESS_CONTROL,					//0x16,Compression Control
    MN_SMS_OBJECT_DISTRIBUT_IND,				//0x17,Object Distribution Indicator
    MN_SMS_STANDARD_WVG_OBJECT,					//0x18,Standard WVG Object
    MN_SMS_CHAR_SIZE_WVG_OBJECT,				//0x19,Character Size WVG Object
    MN_SMS_EXT_OBJECT_DATA_REQ_COMMAND,			//0x1A,Extended Object Data Request Command
    MN_SMS_RFC_822_EMAIL_HEADER = 0x20,			//0x20,RFC 822 E-Mail Header
    MN_SMS_HYPERLINK_FORMAT_ELEMENT,			//0x21,Hyperlink format element
    MN_SMS_REPLY_ADDR_ELEMENT,					//0x22,Reply Address Element
    MN_SMS_ENHANCED_VOICE_MAIL_INFO,			//0x23,Enhanced Voice Mail Information
    MN_SMS_NATIONAL_LANG_SINGLE_SHIFT,          //0x24,National language single shift
    MN_SMS_NATIONAL_LANG_LOCK_SHIFT,            //0x25,National language locking shift
    MN_SMS_SIM_TOOLKIT_SECUR_HEAD = 0x70,       //0x70-0x7F,SIM Toolkit Security Headers
    MN_SMS_SIM_TOOLKIT_SECUR_HEAD_LAST = 0X7F,
    MN_SMS_SME_TO_SME_SPEC_USE = 0x80,          //0x80-0x9F,SME to SME specific use
    MN_SMS_SME_TO_SME_SPEC_USE_LAST = 0x9F,
    MN_SMS_SC_SPEC_USE = 0xC0,                  //0xC0-0xDF,SC specific use
    MN_SMS_SC_SPEC_USE_LAST = 0xDF,
    MN_SMS_RESERVED_USE                         //reserved for future use
} MN_SMS_IEI_ID_E;

typedef enum _MN_SMS_MSG_IND_TYPE_E
{
    VOICE_MSG_WAITING           = 0,
    FAX_MSG_WAITING             = 1,
    ELECTRO_MIAL_MSG_WAITING    = 2,
    OHTER_MSG_WAITING           = 3
}MN_SMS_MSG_IND_TYPE_E;

typedef struct _MN_SMS_CANCAT_8BIT_REF_HEAD_T
{
    uint8   ref_num;
    uint8   segment_num;
    uint8   seq_num;
}MN_SMS_CANCAT_8BIT_REF_HEAD_T;

typedef struct _MN_SMS_CANCAT_16BIT_REF_HEAD_T
{
    uint16   ref_num;
    uint8   segment_num;
    uint8   seq_num;
}MN_SMS_CANCAT_16BIT_REF_HEAD_T;

typedef enum
{
    MN_SMS_VOICE_MSG_WAIT,
    MN_SMS_FAX_MSG_WAIT,
    MN_SMS_EMAIL_MSG_WAIT,
    MN_SMS_OTHER_MSG_WAIT
} MN_SMS_MSG_WAIT_E;

typedef struct _MN_SMS_SPEC_MESS_IND_HEAD_T
{
    BOOLEAN     is_store;
    uint8       profile_id;
    uint8       msg_count;
    MN_SMS_MSG_WAIT_E   msg_type;
}MN_SMS_SPEC_MESS_IND_HEAD_T;

typedef struct _MN_SMS_APP_PORT_8BIT_ADDR_HEAD_T
{
    uint8   des_port;
    uint8   org_port;
}MN_SMS_APP_PORT_8BIT_ADDR_HEAD_T;

typedef struct _MN_SMS_APP_PORT_16BIT_ADDR_HEAD_T
{
    uint16  des_port;
    uint16  org_port;
}MN_SMS_APP_PORT_16BIT_ADDR_HEAD_T;

typedef struct _MN_SMS_SMSC_CNTL_PARAM_HEAD_T
{
    BOOLEAN is_stat_tran_completed;
    BOOLEAN need_stat_for_perm_err;
    BOOLEAN need_stat_for_temp_err;
    BOOLEAN need_stat_for_temp_err_sc_trying;
    BOOLEAN need_udh_in_stat;
}MN_SMS_SMSC_CNTL_PARAM_HEAD_T;

typedef enum _MN_SMS_HEAD_SOURCE_IND_E
{
	MN_SMS_SOURCE_ORG_SENDER = 0x1,
	MN_SMS_SOURCE_ORG_RECEIVER,
	MN_SMS_SOURCE_SMSC
}MN_SMS_HEAD_SOURCE_IND_E;

typedef struct _MN_SMS_UDH_SOURCE_IND_HEAD_T
{
	MN_SMS_HEAD_SOURCE_IND_E	source_ind;
}MN_SMS_UDH_SOURCE_IND_HEAD_T;

typedef struct _MN_SMS_WIRELESS_CNTL_MESG_HEAD_T
{
	uint8	wcmp_pdu_len;
	uint8   wcmp_pdu[MN_SMS_HEAD_WIRL_CNTL_MESG_LEN];
}MN_SMS_WIRELESS_CNTL_MESG_HEAD_T;

typedef enum _MN_SMS_TEXT_ALIGN_E
{
	MN_SMS_TEXT_ALIGN_LEFT,
	MN_SMS_TEXT_ALIGN_CENTER,
	MN_SMS_TEXT_ALIGN_RIGHT,
	MN_SMS_TEXT_ALIGN_LANG_DEP
}MN_SMS_TEXT_ALIGN_E;

typedef enum _MN_SMS_TEXT_FONT_SIZE_E
{
	MN_SMS_TEXT_FONT_SIZE_NORMAL,
	MN_SMS_TEXT_FONT_SIZE_LARGE,
	MN_SMS_TEXT_FONT_SIZE_SMALL,
	MN_SMS_TEXT_FONT_SIZE_RESERVED
}MN_SMS_TEXT_FONT_SIZE_E;

typedef enum MN_SMS_TEXT_COLOUR_E
{
	MN_SMS_TEXT_COLOUR_BLACK,
	MN_SMS_TEXT_COLOUR_DARK_GREY,
	MN_SMS_TEXT_COLOUR_DARK_RED,
	MN_SMS_TEXT_COLOUR_DARK_YELLOW,
	MN_SMS_TEXT_COLOUR_DARK_GREEN,
	MN_SMS_TEXT_COLOUR_DARK_CYAN,
	MN_SMS_TEXT_COLOUR_DARK_BLUE,
	MN_SMS_TEXT_COLOUR_DARK_MAGENTA,
	MN_SMS_TEXT_COLOUR_GREY,
	MN_SMS_TEXT_COLOUR_WHITE,
	MN_SMS_TEXT_COLOUR_BRIGHT_RED,
	MN_SMS_TEXT_COLOUR_BRIGHT_YELLOW,
	MN_SMS_TEXT_COLOUR_BRIGHT_GREEN,
	MN_SMS_TEXT_COLOUR_BRIGHT_CYAN,
	MN_SMS_TEXT_COLOUR_BRIGHT_BLUE,
	MN_SMS_TEXT_COLOUR_BRIGHT_MAGENTA
}MN_SMS_TEXT_COLOUR_E;

typedef struct _MN_SMS_TEXT_FORMAT_HEAD_T
{
	uint8  start_pos;
	uint8  text_format_len;
	MN_SMS_TEXT_ALIGN_E	align_mode;
	MN_SMS_TEXT_FONT_SIZE_E font_size;
	BOOLEAN is_style_bold;
	BOOLEAN is_style_italic;
	BOOLEAN is_style_underlined;
	BOOLEAN is_style_strikethrough;
	MN_SMS_TEXT_COLOUR_E foreground_colour;
	MN_SMS_TEXT_COLOUR_E background_colour;
}MN_SMS_TEXT_FORMAT_HEAD_T;

typedef struct _MN_SMS_PREDEF_SOUND_HEAD_T
{
	uint8 position;
	uint8 sound_num;
}MN_SMS_PREDEF_SOUND_HEAD_T;

typedef struct _MN_SMS_USER_DEF_SOUND_HEAD_T
{
	uint8 position;
	uint8 pdu_len;
	uint8 pdu[MN_SMS_HEAD_MAX_USER_DEF_SOUND_LEN];
}MN_SMS_USER_DEF_SOUND_HEAD_T;

typedef struct _MN_SMS_PREDEF_ANIMATION_HEAD_T
{
	uint8 position;
	uint8 animation_num;
}MN_SMS_PREDEF_ANIMATION_HEAD_T;

typedef struct _MN_SMS_LARGE_ANIMATION_HEAD_T
{
	uint8 position;
	uint8 pdu_len;
	uint8 pdu[MN_SMS_HEAD_MAX_LARGE_ANI_PDU_LEN];
}MN_SMS_LARGE_ANIMATION_HEAD_T;

typedef struct _MN_SMS_SMALL_ANIMATION_HEAD_T
{
	uint8 position;
	uint8 pdu_len;
	uint8 pdu[MN_SMS_HEAD_MAX_SMALL_ANI_PDU_LEN];
}MN_SMS_SMALL_ANIMATION_HEAD_T;

typedef struct _MN_SMS_LARGE_PICTURE_HEAD_T
{
	uint8 position;
	uint8 pdu_len;	
	uint8 pdu[MN_SMS_HEAD_MAX_LARGE_PICT_PDU_LEN];
}MN_SMS_LARGE_PICTURE_HEAD_T;

typedef struct _MN_SMS_SMALL_PICTURE_HEAD_T
{
	uint8 position;
	uint8 pdu_len;	
	uint8 pdu[MN_SMS_HEAD_MAX_SMALL_PICT_PDU_LEN];
}MN_SMS_SMALL_PICTURE_HEAD_T;

typedef struct _MN_SMS_VAR_PICTURE_HEAD_T
{
	uint8 position;
	uint8 hori_dimen;
	uint8 vert_dimen;
	uint8 pdu_len;	
	uint8 pdu[MN_SMS_HEAD_MAX_VAR_PICT_PDU_LEN];
}MN_SMS_VAR_PICTURE_HEAD_T;

typedef struct _MN_SMS_USER_PROMPT_IND_HEAD_T
{
	uint8 obj_num;
}MN_SMS_USER_PROMPT_IND_HEAD_T;

typedef struct _MN_SMS_STANDARD_WVG_OBJECT_HEAD_T
{
	uint8 position;
	uint8 wvg_stream_len; 	
	uint8 wvg_stream[MN_SMS_HEAD_MAX_WVG_STR_LEN];
}MN_SMS_STANDARD_WVG_OBJECT_HEAD_T;

typedef struct _MN_SMS_CHAR_SIZE_WVG_OBJECT_HEAD_T
{
	uint8 position;
	uint8 wvg_stream_len; 
	uint8 wvg_stream[MN_SMS_HEAD_MAX_WVG_STR_LEN];
}MN_SMS_CHAR_SIZE_WVG_OBJECT_HEAD_T;

typedef enum _MN_SMS_EXT_OBJ_TYPE_E
{
	EXT_OBJ_PREDEF_SOUND,
	EXT_OBJ_IMELODY,
	EXT_OBJ_BLACK_WHITE_BITMAP,
	EXT_OBJ_2BIT_GREY_BITMAP,
	EXT_OBJ_6BIT_COLOUR_BITMAP,
	EXT_OBJ_PREDEF_ANIMATION,
	EXT_OBJ_BLACK_WHITE_ANIMATION,
	EXT_OBJ_2BIT_GREY_BITMAP_ANIMATION,
	EXT_OBJ_6BIT_COLOUR_BITMAP_ANIMATION,
	EXT_OBJ_VCARD,
	EXT_OBJ_VCALENDAR,
	EXT_OBJ_STAND_WVG_OBJ,
	EXT_OBJ_POLY_MELODY,
	EXT_OBJ_RESERVED = 0xD,
	EXT_OBJ_RESERVED_LAST = 0xFE,
	EXT_OBJ_DELIVER_REQ
}MN_SMS_EXT_OBJ_TYPE_E;

typedef struct _MN_SMS_EXT_OBJECT_HEAD_T
{
    uint8                   ref_num;
	uint16 	ext_obj_len;
    BOOLEAN                 is_obj_forward;
    BOOLEAN                 is_obj_hand_prompt;
    MN_SMS_EXT_OBJ_TYPE_E   obj_type;
    uint16                  ext_obj_position;
    uint8                   ext_obj_data[MN_SMS_HEAD_MAX_EXT_OBJ_LEN];
}MN_SMS_EXT_OBJECT_HEAD_T;

typedef struct _MN_SMS_REUSED_EXT_OBJECT_HEAD_T
{
	uint8	ref_num;
	uint16	obj_position;
}MN_SMS_REUSED_EXT_OBJECT_HEAD_T;

typedef struct _MN_SMS_COMPRESS_CONTROL_HEAD_T
{
	BOOLEAN is_lzss_compress;
    uint16  data_len;
    uint8   data[MN_SMS_HEAD_MAX_CMPR_DATA_LEN];
}MN_SMS_COMPRESS_CONTROL_HEAD_T;

typedef struct _MN_SMS_OBJECT_DISTRIBUT_IND_HEAD_T
{
	uint8   iei_number;
	BOOLEAN is_may_be_forward;
}MN_SMS_OBJECT_DISTRIBUT_IND_HEAD_T;

typedef struct _MN_SMS_REPLY_ADDR_ELEMENT_HEAD_T
{
	uint8	addr_len;
	uint8	reply_addr[MN_SMS_HEAD_MAX_RELY_ADDR_LEN];
}MN_SMS_REPLY_ADDR_ELEMENT_HEAD_T;

typedef struct _MN_SMS_RFC_822_EMAIL_HEADER_HEAD_T
{
	uint8	email_head_len;
}MN_SMS_RFC_822_EMAIL_HEADER_HEAD_T;

typedef struct _MN_SMS_HYPERLINK_FORMAT_ELEMENT_HEAD_T
{
	uint16	position;
	uint8	title_len;
	uint8	url_len;
}MN_SMS_HYPERLINK_FORMAT_ELEMENT_HEAD_T;

typedef struct _MN_SMS_ENHANCED_VOICE_MAIL_INFO_HEAD_T
{
	uint8 data;
}MN_SMS_ENHANCED_VOICE_MAIL_INFO_HEAD_T;

typedef struct _MN_SMS_NAT_LANG_SINGLE_SHIFT_HEAD_T
{
	uint8 national_lang_id;
}MN_SMS_NAT_LANG_SINGLE_SHIFT_HEAD_T;

typedef struct _MN_SMS_NAT_LANG_LOCK_SHIFT_HEAD_T
{
	uint8 national_lang_id;
}MN_SMS_NAT_LANG_LOCK_SHIFT_HEAD_T;

typedef struct _MN_SMS_SIM_TOOLKIT_SECUR_HEAD_T
{
	uint8 data;
}MN_SMS_SIM_TOOLKIT_SECUR_HEAD_T;
typedef struct _MN_SMS_SME_TO_SME_SPEC_USE_HEAD_T
{
	uint8 data;
}MN_SMS_SME_TO_SME_SPEC_USE_HEAD_T;

typedef struct _MN_SMS_SC_SPEC_USE_HEAD_T
{
	uint8 data;
}MN_SMS_SC_SPEC_USE_HEAD_T;

typedef struct _MN_SMS_USER_HEAD_STRUCT_T
{
    MN_SMS_IEI_ID_E     iei;
    union{
    MN_SMS_CANCAT_8BIT_REF_HEAD_T   cancat_8bit;
    MN_SMS_CANCAT_16BIT_REF_HEAD_T  cancat_16bit;
    MN_SMS_SPEC_MESS_IND_HEAD_T     msg_ind;
    MN_SMS_APP_PORT_8BIT_ADDR_HEAD_T    port_8bit;
    MN_SMS_APP_PORT_16BIT_ADDR_HEAD_T   port_16bit;
    MN_SMS_SMSC_CNTL_PARAM_HEAD_T   smsc_para;
    MN_SMS_UDH_SOURCE_IND_HEAD_T	udh_source_ind;
    MN_SMS_WIRELESS_CNTL_MESG_HEAD_T wireless_control;
    MN_SMS_TEXT_FORMAT_HEAD_T		text_format;
    MN_SMS_PREDEF_SOUND_HEAD_T		predef_sound;
    MN_SMS_USER_DEF_SOUND_HEAD_T 	userdef_sound;
    MN_SMS_PREDEF_ANIMATION_HEAD_T 	predef_animation;
    MN_SMS_LARGE_ANIMATION_HEAD_T	large_animation;
    MN_SMS_SMALL_ANIMATION_HEAD_T	small_animation;
    MN_SMS_LARGE_PICTURE_HEAD_T		large_picture;
    MN_SMS_SMALL_PICTURE_HEAD_T		small_picture;
    MN_SMS_VAR_PICTURE_HEAD_T		var_picture;
    MN_SMS_USER_PROMPT_IND_HEAD_T	user_prompt_ind;
    MN_SMS_EXT_OBJECT_HEAD_T		ext_object;
    MN_SMS_REUSED_EXT_OBJECT_HEAD_T	reused_ext_object;
    MN_SMS_COMPRESS_CONTROL_HEAD_T	comp_cntrl;
    MN_SMS_OBJECT_DISTRIBUT_IND_HEAD_T obj_distr_ind;
    MN_SMS_STANDARD_WVG_OBJECT_HEAD_T wvg_obj;
    MN_SMS_CHAR_SIZE_WVG_OBJECT_HEAD_T char_wvg_obj;			
    MN_SMS_RFC_822_EMAIL_HEADER_HEAD_T	rfc_email;
    MN_SMS_HYPERLINK_FORMAT_ELEMENT_HEAD_T	hyperlink_format;			
    MN_SMS_REPLY_ADDR_ELEMENT_HEAD_T	reply_addr;				
    MN_SMS_ENHANCED_VOICE_MAIL_INFO_HEAD_T	enhance_voice_mail;
    MN_SMS_NAT_LANG_SINGLE_SHIFT_HEAD_T nat_lang_single_shift;
    MN_SMS_NAT_LANG_LOCK_SHIFT_HEAD_T nat_lang_lock_shift;
    MN_SMS_SIM_TOOLKIT_SECUR_HEAD_T	stk_secur;
    MN_SMS_SME_TO_SME_SPEC_USE_HEAD_T	sme_spec_use;
    MN_SMS_SC_SPEC_USE_HEAD_T	spec_use;                  
    }head_union;
    
}MN_SMS_USER_HEAD_STRUCT_T;

typedef struct _MN_SMS_USER_HEAD_ARR_T
{
    uint8   num;
    MN_SMS_USER_HEAD_STRUCT_T   head_arr[MN_SMS_MAX_IEI_NUM];
}MN_SMS_USER_HEAD_ARR_T;


/* 
 the time stamp struct 
*/
typedef struct _MN_SMS_TIME_STAMP_T
{
    uint8 year    ;
    uint8 month   ;
    uint8 day     ;
    uint8 hour    ;
    uint8 minute  ;
    uint8 second  ;
    uint8 timezone;
} MN_SMS_TIME_STAMP_T , *MN_SMS_TIME_STAMP_PTR_T;


typedef union _MN_SMS_TIME_FORMAT_U
{
    MN_SMS_TIME_SECOND_T    time_second;    // the time express by second
    MN_SMS_TIME_STAMP_T     time_stamp_t;   //the time stamp struct
} MN_SMS_TIME_FORMAT_U;




// SMS support multiple language
typedef struct _MN_SMS_SUPPORT_LANG_T
{
    uint8       id_len;
    uint8       id[MN_SMS_MAX_SUPPORT_LANG_LEN];
} MN_SMS_SUPPORT_LANG_T;

/*
 the user data head structure after decode
*/
typedef struct _MN_SMS_HEAD_IEI_T
{
    MN_SMS_IEI_ID_E iei;
    uint8 length;           // the number of user data head iei
    uint8 *iei_data_t;      // the user data head iei structure 
} MN_SMS_HEAD_IEI_T;

/*
 the user data head structure after decode
*/
typedef struct _MN_SMS_USER_HEAD_T
{
    uint8 iei_num;                                          // the number of user data head iei
    MN_SMS_HEAD_IEI_T iei_arr [ MN_SMS_MAX_IEI_NUM ]; // the user data head iei structure 
} MN_SMS_USER_HEAD_T;

/*
 the user data head structure
*/
typedef struct _MN_SMS_USER_DATA_HEAD_T
{
    uint8 length;                                          // the length
    uint8 user_header_arr [ MN_SMS_USER_DATA_LENGTH ]; // the data buf 
} MN_SMS_USER_DATA_HEAD_T;

/*
 the user valid data structure
*/
typedef struct _MN_SMS_USER_VALID_DATA_T
{    
    uint8 length;                                          // the length
    uint8 user_valid_data_arr [ MN_SMS_MAX_USER_VALID_DATA_LENGTH ]; // the data buf 
} MN_SMS_USER_VALID_DATA_T;


typedef struct _MN_SMS_USER_DATA_T
{
    uint8 length;                                          // the length
    uint8 user_data_arr [ MN_SMS_USER_DATA_LENGTH + 1 ]; // the data buf 
} MN_SMS_USER_DATA_T;


/*
 the command SMS structure
*/
typedef struct _MN_SMS_COMMAND_SMS_T
{
    MN_CALLED_NUMBER_T      sc_addr_t;                // the sc address structure
    MN_CALLED_NUMBER_T      dest_addr_t;              // the destination address structure
    uint8                   tp_mr;                    // the message_reference of specified short message
    MN_SMS_PID_E            pid_e;                    // the pid value
    MN_SMS_COMMAND_TYPE_E   command_type_e;           // the command type
} MN_SMS_COMMAND_SMS_T , *MN_SMS_COMMAND_SMS_PTR_T;

/*
the short message class enum
*/
typedef enum _MN_SMS_CLASS_E
{
    MN_SMS_CLASS_0,
    MN_SMS_CLASS_1,
    MN_SMS_CLASS_2,
    MN_SMS_CLASS_3
}MN_SMS_CLASS_E;

typedef struct _MN_SMS_DCS_T
{
    BOOLEAN                 is_bit_67_set;     //bit 6 7 is 00 or 11, 0:00; 1:11;
    BOOLEAN                 class_is_present;  // whether use specified class
    MN_SMS_CLASS_E          sms_class;         // the SMS class type
    BOOLEAN                 msg_is_waiting;    // whether the DCS is to indicate message is waiting
    MN_SMS_MSG_WAIT_E       wait_type;         // the waiting message type
    BOOLEAN                 is_save_wait_msg;
    BOOLEAN                 is_active_wait_ind;
    MN_SMS_ALPHABET_TYPE_E  alphabet_type;     // the alphabet type
    uint8                   line1_waiting_count;    //indicating the voice waiting count of the default line no. 
    uint8                   line2_waiting_count;    //indicating the voice waiting count of the line 2
    uint8                   fax_waiting_count;      //indicating the count of waiting fax messages
    uint8                   data_waiting_count;     //indicating the count of waiting email messages
    uint8                   other_waiting_count;    //indicating the count of waiting other type of messages
} MN_SMS_DCS_T;

/*
 the MO SMS structure
*/
typedef struct _MN_SMS_MO_SMS_T
{
    BOOLEAN              user_head_is_exist;        // in---show whether the user data header is exit  
    BOOLEAN              reply_path_is_request;     // whether set reply path
    BOOLEAN              status_report_is_request;  // whether request the status report 
    MN_SMS_TP_VPF_E      time_format_e           ;
    MN_SMS_TIME_FORMAT_U time_valid_period       ;  // the validity period express by second
    MN_SMS_TP_MR_T       tp_mr                   ;  // message reference
    BOOLEAN              dest_addr_present       ;
    MN_CALLED_NUMBER_T   dest_addr_t             ;  // the destination address structure
    BOOLEAN              sc_addr_present         ;  // whether present sc address
    MN_CALLED_NUMBER_T   sc_addr_t               ;  // the sc address structure
    BOOLEAN              pid_present             ;
    MN_SMS_PID_E         pid_e                   ;  // the pid value
    MN_SMS_DCS_T         dcs;                       // the message class struct
    MN_SMS_USER_DATA_T   user_data_t             ;  // the user data structure
    MN_SMS_TIME_STAMP_T  time_stamp_t;              // the time when user edit the SMS 
} MN_SMS_MO_SMS_T;

/*
 the MT SMS structure
*/
typedef struct _MN_SMS_MT_SMS_T
{
    BOOLEAN             user_head_is_exist;         // in---show whether the user data header is exit  
    BOOLEAN             reply_path_is_exist;            // whether the reply path exist
    BOOLEAN             sme_status_report_is_request;   // whether the SMS status report request
    MN_CALLED_NUMBER_T  sc_addr_t;                      // the sc address structure    
    MN_CALLED_NUMBER_T  origin_addr_t;                  // the origin address structure 
    MN_SMS_PID_E        pid_e;                          // the pid value
    MN_SMS_DCS_T        dcs;                            // the SMS data coding scheme
    MN_SMS_TIME_STAMP_T time_stamp_t;                   // the time which the sc receive the SMS 
    MN_SMS_USER_DATA_T  user_data_t;                    // the user data structure
} MN_SMS_MT_SMS_T;


/*
 the status report structure
*/
typedef struct _MN_SMS_STATUS_REPORT_T
{
    MN_SMS_TP_MR_T      tp_mr;          // the message reference of the enquired message
    MN_SMS_TP_STATUS_E  tp_status;      // the status of the previously sent MO short message
    MN_CALLED_NUMBER_T  sc_addr_t;      // the sc address structure
    MN_CALLED_NUMBER_T  dest_addr_t;    // the destination address of the message sent to 
    MN_SMS_TIME_STAMP_T tp_scts;        // the time which the sc receive the SMS     
    MN_SMS_TIME_STAMP_T tp_dt;          // the time which the destination sme received the message
    MN_L3_MSG_UNIT_T    msg_data;       // Only used for ATC module, Not decoded message body. 
} MN_SMS_STATUS_REPORT_T ;

/*
 the SMS union include MT and MO SMS
*/
typedef union _MN_SMS_U_T   //modify by sunsome.ju cr5974 2003-12-1
{
    MN_SMS_MO_SMS_T mo_sms_t;           // the MO SMS structure
    MN_SMS_MT_SMS_T mt_sms_t;           // the MT SMS structure 
    MN_SMS_STATUS_REPORT_T report_sms_t;  // the status report SMS structure
} MN_SMS_U_T , *MN_SMS_PTR_U;


/*
 the tp validity period struct
*/
typedef struct  _MN_SMS_TP_VP_T
{
    MN_SMS_TP_VPF_E  vpf;                          // the validity period format          
    uint8         tp_vp_arr [ MN_SMS_VP_MAX_LEN ]; // the validity period value
} MN_SMS_TP_VP_T;

/*
 the parameter structure
*/
typedef struct _MN_SMS_PARAM_T
{
    BOOLEAN                 alpha_id_present      ;    // whether the alpha id exist
    MN_SMS_SUPPORT_LANG_T   alpha_id_t            ;    // the alpha id struct
    BOOLEAN                 tp_dest_addr_present  ;    // whether the tp destination exist
    MN_CALLED_NUMBER_T      tp_dest_addr_t        ;    // the destination address struct                  
    BOOLEAN                 tp_sc_addr_present    ;    // whether the sc address exist 
    MN_CALLED_NUMBER_T      tp_sc_addr_t          ;    // the sc address struct
    BOOLEAN                 tp_pid_present        ;    // if the protocol id exist
    MN_SMS_PID_E            tp_pid                ;    // the protocol id value
    BOOLEAN                 tp_dcs_present        ;    // whether data coding scheme exist
    MN_SMS_DCS_T            tp_dcs_t              ;    // the data coding scheme struct
    BOOLEAN                 tp_vp_present         ;    // whether validity period exist
    MN_SMS_TIME_SECOND_T    vp_second             ;    // the validity value express by second 
} MN_SMS_PARAM_T, *MN_SMS_PARAM_PTR_T;

typedef struct
{
    uint8   alpha_id_len;   //the alpha id length of the dialling number
    uint8   mailbox_number_len;         //the dialling number length
    uint8   ccp_id;         //the Capability/Configuration Identifier
    BOOLEAN is_ext_exist;   //the Extension exist or not
    uint8   ext_id;         //the Extension1 record identifier
    BOOLEAN is_ccp_exist;   //the Capability/Configuration exist or not
    BOOLEAN is_ton_npi_exist;   //is the TON and NPI exist
    uint8   alpha_id[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN];    //the alpha id of the dialling number
    uint8   mailbox_number[PHONEBOOK_MAX_NORMAL_DN_NUMBER_LEN];    //the dialling number
    MN_NUMBER_TYPE_E   ton;    //the type of the number(TON)
    MN_NUMBER_PLAN_E   npi;    //the numbering plan identification  
}MN_SMS_VMW_MBNUM_T;

typedef struct
{
    BOOLEAN is_datafax_flag_present;
    uint8 line1_waiting_count;    //indicating the voice waiting count of the default line no. 
    uint8 line2_waiting_count;    //indicating the voice waiting count of the line 2
    uint8 fax_waiting_count;      //indicating the count of waiting fax messages
    uint8 data_waiting_count;     //indicating the count of waiting email messages
    uint8 other_waiting_count;    //indicating the count of waiting other type of messages
}MN_SMS_VMW_IND_FLAG_T;

typedef struct _MN_SMS_MESSAGE_ITEM_T
{
    MN_DUAL_SYS_E       dual_sys;
    uint8               status;
    MN_SMS_TP_MR_T      tp_mr;
    uint8               sc_addr_buf[MN_SMS_ADDR_MAX_LEN];
    uint8               tpdu[MN_SMS_SIM_SMS_LEN];
    uint8               status_report_arr[MN_SMS_STATUS_REPORT_LEN];
    MN_SMS_TIME_STAMP_T time_stamp_t;          // the time when user edit the SMS,only exists in MO SMS 
} MN_SMS_MESSAGE_ITEM_T;

typedef enum _MN_SMS_DLVR_RPT_E
{
   MN_SMS_ACK_DELIVER_REPORT,
   MN_SMS_NACK_DELIVER_REPORT
}MN_SMS_DLVR_RPT_E; 

typedef enum 
{
    MN_SMSCB_GERMAN_ID      = 0,
    MN_SMSCB_ENGLISH_ID     = 1,
    MN_SMSCB_ITALIAN_ID     = 2,
    MN_SMSCB_FRENCH_ID      = 3,
    MN_SMSCB_SPANISH_ID     = 4,
    MN_SMSCB_DUTCH_ID       = 5,
    MN_SMSCB_SWEDISH_ID     = 6,
    MN_SMSCB_DANISH_ID      = 7,
    MN_SMSCB_PORTUGUESE_ID  = 8,
    MN_SMSCB_FINNISH_ID     = 9,
    MN_SMSCB_NORWEGIAN_ID   = 10,
    MN_SMSCB_GREEK_ID       = 11,
    MN_SMSCB_TURKISH_ID     = 12,
    MN_SMSCB_HUNGARIAN_ID   = 13,
    MN_SMSCB_POLISH_ID      = 14,
    MN_SMSCB_LANGUAGE_UNSPECIFIED =15,
    MN_SMSCB_CZECH_ID		= 0x20,
    MN_SMSCB_HEBREW_ID		= 0x21,
    MN_SMSCB_ARABIC_ID		= 0x22,
    MN_SMSCB_RUSSIAN_ID		= 0x23,
    MN_SMSCB_ICELANDIC_ID   = 0x24,
    MN_SMSCB_CHINESE_ID		= 0x48,
    MN_SMSCB_LANGUAGE_NUM = 0x7a7b //max language id = 0x7a7a(zz)
}MN_SMSCB_LANGUAGE_ID_E;

typedef struct _MN_SMSCB_DCS_T
{
    BOOLEAN                 class_is_present;  // whether use specified class
    MN_SMS_CLASS_E          sms_class;         // the SMS class type
    MN_SMS_ALPHABET_TYPE_E  alphabet_type;     // the alphabet type
    BOOLEAN                 language_id_present;
    MN_SMSCB_LANGUAGE_ID_E  language_id;
} MN_SMSCB_DCS_T;

typedef struct
{
    BOOLEAN               speech_is_support;
    BOOLEAN               unrestricted_digit_is_support;
    BOOLEAN               audio_is_support;
    BOOLEAN               facsimile_group3_is_support;
    BOOLEAN               other_ITC_is_support;
} MN_BEARER_CAPABILITY_T;

// used for SMS in NV
typedef BOOLEAN (*MNSMS_READNVSMS_PFUNC)(MN_SMS_RECORD_ID_T rid,MN_SMS_MESSAGE_ITEM_T   *item_ptr);

typedef BOOLEAN (*MNSMS_WRITENVSMS_PFUNC)(BOOLEAN   is_need_sync,MN_SMS_RECORD_ID_T rid,MN_SMS_MESSAGE_ITEM_T   *item_ptr);

typedef BOOLEAN (*MNSMS_SETMAXNVSMSNUM_PFUNC)(MN_SMS_RECORD_ID_T        *max_sms_num_ptr);

typedef BOOLEAN (*MNSMS_DELALLNVSMS_PFUNC)(MN_SMS_RECORD_ID_T       max_sms_num_ptr);

 /*****************************************************************************
 **                        SIM LOCK MODULE                                    *
 ******************************************************************************/
#define MN_SIM_LOCK_NONE                   0
#define MN_SIM_NETWORK_LOCK                1
#define MN_SIM_NETWORK_SUBSET_LOCK   (1 << 1)
#define MN_SIM_SP_LOCK               (1 << 2)
#define MN_SIM_CORPORATE_LOCK        (1 << 3)
#define MN_SIM_USER_LOCK             (1 << 4)
#define MN_SIM_ALL_LOCK              (MN_SIM_NETWORK_LOCK+MN_SIM_NETWORK_SUBSET_LOCK+MN_SIM_SP_LOCK+MN_SIM_CORPORATE_LOCK+MN_SIM_USER_LOCK)
#define MN_SIM_LOCK_UNDEFINED           0xFF
#define ORANGE_SPL_BIT   MN_SIM_NETWORK_LOCK + MN_SIM_SP_LOCK
#define ORANGE_LES_BIT                 (1 << 5)

#define SIM_LOCK_KEY_MAX_LEN              16
#define SIM_LOCK_PCK_MIN_LEN               6    // PCK: 6 to 16 digits
#ifdef GSM_CUSTOMER_AFP_SUPPORT
#define SIM_LOCK_OTHER_KEY_MIN_LEN         20   // the length of encryped key
#else
#define SIM_LOCK_OTHER_KEY_MIN_LEN         8    // NCK,NSCK,SPCK,CCK: 8 to 16 digits, raw key
#endif

#define INVALID_GID_VALUE               0xFF

#ifdef GSM_CUSTOMER_AFP_SUPPORT
#define MAX_PERSONALISATIONS              16
#else
#define MAX_PERSONALISATIONS              40
#endif

#define SIM_LOCK_ENCRYPTED_KEY_MIN_LEN    SIM_LOCK_OTHER_KEY_MIN_LEN
#define SIM_LOCK_KEY_LEN   (SIM_LOCK_KEY_MAX_LEN > SIM_LOCK_OTHER_KEY_MIN_LEN ? SIM_LOCK_KEY_MAX_LEN : SIM_LOCK_OTHER_KEY_MIN_LEN)

#define SIM_LOCK_ENCRYPTED_KEY_MAX_LEN  32    /* MAX length of control key in encrypted data format */

#define SIM_LOCK_STATIC_KEY_TYPE       0x00
#define SIM_LOCK_DYNAMIC_KEY_TYPE      0x01

#define SIM_LOCK_OPER_MODE0      0x00
#define SIM_LOCK_OPER_MODE1      0x01
#define SIM_LOCK_OPER_MODE2      0x02

typedef enum _MN_PHONE_COMPANY_ID_E
{
    MN_PHONE_COMPANY_UNKNOWN = 0x00,
    MN_PHONE_COMPANY_ORANGE,
    MN_PHONE_COMPANY_INVALID = 0xFFFF
} MN_PHONE_COMPANY_ID_E;

typedef enum _MN_SIM_LOCK_ERROR_CODE_E
{
    MN_SIM_LOCK_NO_ERROR,                      // (de-)personalisation success
    MN_SIM_LOCK_DATA_INTEGRITY_FAIL,           // The SIM lock data not pass Integrity check
    MN_SIM_LOCK_PARAMETER_ERROR,               // (de-)personalisation parameter error
    MN_SIM_LOCK_UPDATE_NV_ERROR,               // (de-)personalisation update nv data error
    MN_SIM_LOCK_ERROR_ALREADY_LOCKED,          // do personalisation but this item already locked
    MN_SIM_LOCK_ERROR_SIM_NOT_READY,           // SIM not ready, can't get SIM info
    MN_SIM_LOCK_ERROR_GID_NOT_SUPPORT,         // do SP/CORPORATE personalisation but GID not support
    MN_SIM_LOCK_ERROR_NETWORK_FAIL,            // Network (de-)personalisation fail
    MN_SIM_LOCK_ERROR_NETWORK_SUBSET_FAIL,     // Network subset (de-)personalisation fail
    MN_SIM_LOCK_ERROR_SP_FAIL,                 // Service Provider (de-)personalisation fail
    MN_SIM_LOCK_ERROR_CORPORATE_FAIL,          // Corporate (de-)personalisation fail
    MN_SIM_LOCK_ERROR_USER_FAIL                // (U)SIM (de-)personalisation fail
} MN_SIM_LOCK_ERROR_CODE_E;

typedef enum _MN_SIM_LOCK_TYPE_E
{
    SIM_LOCK_NONE = 0,
    SIM_NETWORK_LOCK = 1,
    SIM_NETWORK_SUBSET_LOCK = (1 << 1),
    SIM_SP_LOCK = (1 << 2),
    SIM_CORPORATE_LOCK = (1 << 3),
    SIM_USER_LOCK = (1 << 4),
    SIM_LOCK_UNDEFINED = 0xFF
} MN_SIM_LOCK_TYPE_E;

typedef struct _SIM_LOCK_KEY_T
{
    uint8                      key_len;    // PCK: 6 to 16 digits; NCK,NSCK,SPCK,CCK: 8 to 16 digits
    uint8                      keys[SIM_LOCK_KEY_LEN];  
} SIM_LOCK_KEY_T;

typedef struct _MN_SIM_CONTROL_KEYS_T
{
    SIM_LOCK_KEY_T   nck;
    SIM_LOCK_KEY_T   nsck;
    SIM_LOCK_KEY_T   spck;
    SIM_LOCK_KEY_T   cck;
    SIM_LOCK_KEY_T   pck;
} MN_SIM_CONTROL_KEYS_T;

typedef MN_PLMN_T  NETWORK_LOCK_DATA_T;

typedef struct _NETWORK_LOCKS_T
{
    uint8                       numLocks;
    uint8                       reserved;  // Byte alignment
    MN_PLMN_T                   locks[MAX_PERSONALISATIONS];
} NETWORK_LOCKS_T;

typedef struct _NETWORK_SUBSET_LOCK_DATA_T
{
    MN_PLMN_T                   plmn;
    uint8                       network_subset[2]; // IMSI digits 6 and 7
} NETWORK_SUBSET_LOCK_DATA_T;

typedef struct _NETWORK_SUBSET_LOCKS_T
{
    uint8                       numLocks;
    uint8                       reserved;  // Byte alignment
    NETWORK_SUBSET_LOCK_DATA_T  locks[MAX_PERSONALISATIONS];
} NETWORK_SUBSET_LOCKS_T;

typedef struct _SP_LOCK_DATA_T
{
    MN_PLMN_T                   plmn;
    uint8                       sp;   // GID1 1 byte
    uint8                       reserved;  // Byte alignment
} SP_LOCK_DATA_T;

typedef struct _SP_LOCKS_T
{
    uint8                       numLocks;
    uint8                       reserved;  // Byte alignment
    SP_LOCK_DATA_T              locks[MAX_PERSONALISATIONS];
} SP_LOCKS_T;

typedef struct _CORPORATE_LOCK_DATA_T
{
    MN_PLMN_T                   plmn;
    uint8                       sp;         // GID1 1 byte
    uint8                       corporate;  // GID2 1 byte
} CORPORATE_LOCK_DATA_T;

typedef struct _CORPORATE_LOCKS_T
{
    uint8                       numLocks;
    uint8                       reserved;  // Byte alignment
    CORPORATE_LOCK_DATA_T       locks[MAX_PERSONALISATIONS];
} CORPORATE_LOCKS_T;

typedef MN_IMSI_T  USER_LOCK_DATA_T;

typedef struct _USER_LOCKS_T
{
    uint8                       numLocks;
    USER_LOCK_DATA_T            locks[MAX_PERSONALISATIONS];
} USER_LOCKS_T;

typedef struct _MN_SIM_LOCK_CONTROL_DATA_T
{
    MN_IMEI_T                  imei;
    uint32                     rand[4];
    MN_SIM_CONTROL_KEYS_T      lock_keys;
    MN_SIM_CONTROL_KEYS_T      unlock_keys;
} MN_SIM_LOCK_CONTROL_DATA_T;

typedef struct _MN_SUBSET_T
{
    uint8    subset_code[2];                  
}MN_SUBSET_T;

#ifdef GSM_CUSTOMER_AFP_SUPPORT
typedef struct _MN_SIM_LOCK_XML_DATA_T
{
    uint32                      max_num_trials;
    uint8                       nw_numLocks;  
    MN_PLMN_T                   nw_locks[MAX_PERSONALISATIONS];
    uint8                       subset_numLocks; 
    MN_SUBSET_T                 subset_locks[MAX_PERSONALISATIONS]; 
    uint8                       sp_numLocks;  
    uint8                       sp_locks[MAX_PERSONALISATIONS]; 
    uint8                       cp_numLocks;  
    uint8                       cp_locks[MAX_PERSONALISATIONS];
} MN_SIM_LOCK_XML_DATA_T; // will be deleted after MMI merged new code
#endif

typedef struct _MN_SIM_LOCK_CUSTOMIZE_DATA_T
{
    MN_PHONE_COMPANY_ID_E      company_id;
    uint32                     SIM_lock_status;
    uint32                     max_num_trials;
    uint32                     time_interval;
    uint32                     time_multiplier;
    NETWORK_LOCKS_T            network_locks;
    NETWORK_SUBSET_LOCKS_T     network_subset_locks;
    SP_LOCKS_T                 SP_locks;
    CORPORATE_LOCKS_T          corporate_locks;
    uint32                     dummy1;        //reserve for future
    uint32                     dummy2;        //reserve for future
    uint32                     dummy3;        //reserve for future
    uint32                     dummy4;        //reserve for future
    uint32                     encrypted_CRC[4];
} MN_SIM_LOCK_CUSTOMIZE_DATA_T;

typedef struct _MN_SIM_LOCK_USER_DATA_T
{
    uint32                     SIM_lock_status;
    uint32                     nck_trials;
    uint32                     nck_unlock_time;
    uint32                     nsck_trials;
    uint32                     nsck_unlock_time;
    uint32                     spck_trials;
    uint32                     spck_unlock_time;
    uint32                     cck_trials;
    uint32                     cck_unlock_time;
    uint32                     pck_trials;
    uint32                     pck_unlock_time;
    uint32                     dummy1;        //reserve for future
    uint32                     dummy2;        //reserve for future
    uint32                     dummy3;        //reserve for future
    uint32                     dummy4;        //reserve for future
    USER_LOCKS_T               user_locks;
    uint32                     encrypted_CRC[4];
} MN_SIM_LOCK_USER_DATA_T;

typedef struct MN_SIM_LOCK_DETAL_DATA_T
{
    uint8 mck[SIM_LOCK_OTHER_KEY_MIN_LEN];
    uint8 nck[SIM_LOCK_OTHER_KEY_MIN_LEN];
    uint8 sck[SIM_LOCK_OTHER_KEY_MIN_LEN];
    uint8 spck[SIM_LOCK_OTHER_KEY_MIN_LEN];
    uint8 cpck[SIM_LOCK_OTHER_KEY_MIN_LEN];
    uint8 pck[SIM_LOCK_OTHER_KEY_MIN_LEN];
} MN_SIM_LOCK_DETAL_DATA_T;

typedef struct _USER_LOCKS2_T
{
    uint8                       numLocks;
    USER_LOCK_DATA_T            locks[3];  
} USER_LOCKS2_T;

typedef struct _MN_SIM_LOCK_USER_DATA2_T
{
    uint32                     SIM_lock_status;
    uint32                     nck_trials;
    uint32                     nck_unlock_time;
    uint32                     nsck_trials;
    uint32                     nsck_unlock_time;
    uint32                     spck_trials;
    uint32                     spck_unlock_time;
    uint32                     cck_trials;
    uint32                     cck_unlock_time;
    uint32                     pck_trials;
    uint32                     pck_unlock_time;
    uint32                     dummy1;        //reserve for future
    uint32                     dummy2;        //reserve for future
    uint32                     dummy3;        //reserve for future
    uint32                     dummy4;        //reserve for future
    USER_LOCKS2_T               user_locks;
    uint32                     encrypted_CRC[4];
} MN_SIM_LOCK_USER_DATA2_T;

typedef struct _MN_SIM_LOCK_CONTROL_CODES_T
{
    NETWORK_LOCKS_T          network_locks;
    NETWORK_SUBSET_LOCKS_T   network_subset_locks;
    SP_LOCKS_T               sp_locks;
    CORPORATE_LOCKS_T        corporate_locks;
    USER_LOCKS_T             user_locks;
} MN_SIM_LOCK_CONTROL_CODES_T;

typedef struct _MN_SIM_LOCK_STATUS_T
{
    MN_PHONE_COMPANY_ID_E   company_id;
    uint32    max_num_trials;
    uint32    time_interval;
    uint32    time_multiplier;
    BOOLEAN   network_locked;
    uint32    nck_trials;
    uint32    nck_unlock_time;
    BOOLEAN   network_subset_locked;
    uint32    nsck_trials;
    uint32    nsck_unlock_time;
    BOOLEAN   sp_locked;
    uint32    spck_trials;
    uint32    spck_unlock_time;
    BOOLEAN   corporate_locked;
    uint32    cck_trials;
    uint32    cck_unlock_time;
    BOOLEAN   user_locked;
    uint32    pck_trials;
    uint32    pck_unlock_time;
} MN_SIM_LOCK_STATUS_T;

typedef struct MN_SIM_LOCK_CONTROL_KEY_T
{
    uint32    simlock_support;
    uint32    oper_mode;
    uint32  control_key_type;
    MN_SIM_CONTROL_KEYS_T       unlock_keys;
    MN_SIM_CONTROL_KEYS_T       lock_keys;
    uint32                     encrypted_CRC[4];
} MN_SIM_LOCK_CONTROL_KEY_T;

/*****************************The signal structure****************************/
typedef enum
{
   APP_MN_VALIDITY_PER_NULL,
   APP_MN_VALIDITY_PER_RELATIVE
}app_mn_validity_period_format_enum;

/*
the signal indicate the mnsms module is ok!
*/
typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
    app_mn_validity_period_format_enum vp_format;
    uint8 relative_value;
} APP_MN_SMS_READY_IND_T;

/*
the signal indicate the memory of save short message is full
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_MEM_FULL_STATUS_E    mem_status;
    MN_DUAL_SYS_E               dual_sys;
}APP_MN_SMS_MEM_FULL_IND_T;

#ifdef GSM_CUSTOMER_AFP_SUPPORT
/*
the signal indicate the memory full status and the current event 
added for samsung
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_MEM_FULL_STATUS_E    mem_status;
    MN_SMS_MEM_FULL_EVENT_E     event;
    MN_DUAL_SYS_E               dual_sys;
}APP_MN_SMS_MEM_FULL_IND_EX_T;
#endif

/*
 the load SMS confirm structure , respond to the MNSMS_load_sms()
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E     cause;         // the operate cause
    MN_SMS_STORAGE_E   storage;
    MN_SMS_RECORD_ID_T record_id;     // the SMS position   
    MN_SMS_STATUS_E    status;        // the SMS state
    MN_SMS_U_T         sms_t_u;       // the SMS umion, MT or MO
    MN_DUAL_SYS_E      item_sys;      // which SIM card SMS flag
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_READ_SMS_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint8     sms_num;
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_SMS_NUM_IND_T;

/*
 the update SMS confirm structure , respond to the MNSMS_update_sms()
*/
typedef struct
{
    SIGNAL_VARS

    MN_SMS_CAUSE_E     cause;       // the operate cause
    MN_SMS_RECORD_ID_T record_id;   // the SMS position 
    MN_SMS_STORAGE_E   storage;     // where the short message is saved
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_UPDATE_SMS_STATUS_CNF_T;


/*
  now we do not use the signal temporary
*/

typedef struct 
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E          cause;          // the result of read status report 
    MN_SMS_STORAGE_E        storage;        // where the corresponded message is saved
    MN_SMS_RECORD_ID_T      record_id;      // the index of corresponded message
    MN_SMS_STATUS_REPORT_T  status_report;  // the status report according to a short message
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_READ_SMS_STATUS_REPORT_CNF_T;


/*
 the send SMS confirm structure , it respond the MNSMS_send_sms () or MNSMS_send_command_sms()
*/
typedef struct 
{
    SIGNAL_VARS 
    MN_SMS_CAUSE_E     cause;  // the operate cause
    //The TP_MR value corresponding to this MO SMS sending. Just for ATC module.
    MN_SMS_TP_MR_T     tp_mr;   
    MN_DUAL_SYS_E      dual_sys;
    MN_SMS_MO_FAIL_CAUSE_E rl_cause;
} APP_MN_SEND_SMS_CNF_T;

/*
 the cancel SMS confirm structure , it respond the APP_MN_CANCEL_SMS_REQ signal
*/
typedef struct 
{
    SIGNAL_VARS 
    MN_SMS_CAUSE_E     cause;       // the operate cause
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_CANCEL_SMS_CNF_T;

/*
 the save SMS confirm structure , it respond the MNSMS_save_mo_sms() or MNSMS_save_mt_sms() 
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E     cause;       // the operate cause
    MN_SMS_RECORD_ID_T record_id;   // the SMS position
    MN_SMS_STORAGE_E   storage;     // where the short message is saved
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_WRITE_SMS_CNF_T;

typedef struct APP_MN_DELETE_SMS_IND_T_s
{
    SIGNAL_VARS
    MN_SMS_STORAGE_E    storage;                                 // where the short message is saved
    uint8               valid_num;
    MN_SMS_RECORD_ID_T  record_id[MN_SMS_CONCATENATED_MAX_NUM];  // the SMS position, start from 0
    MN_DUAL_SYS_E       dual_sys;
} APP_MN_DELETE_SMS_IND_T;

/*
 the save SMS confirm structure , it respond the MNSMS_save_mo_sms() or MNSMS_save_mt_sms() 
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E          cause;      // the operate cause
    MN_SMS_VMW_IND_FLAG_T   vmw_flag;   // the SMS voice message indicator
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_GET_VMW_FLAG_CNF_T;

/*
 the save SMS confirm structure , it respond the MNSMS_save_mo_sms() or MNSMS_save_mt_sms() 
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E     cause;       // the operate cause
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_SET_VMW_FLAG_CNF_T;

/*
 the save SMS confirm structure , it respond the MNSMS_save_mo_sms() or MNSMS_save_mt_sms() 
*/
typedef enum MN_MULTI_SUBSCRIBER_ID_TYPE_E
{
    MN_MULTI_SUBSCRIBER_NULL,//for cphs datamailbox and faxmailbox
    MN_MULTI_SUBSCRIBER_ID_1,//line1 in cphs
    MN_MULTI_SUBSCRIBER_ID_2,//line2 in cphs
    MN_MULTI_SUBSCRIBER_MAX
}MN_MULTI_SUBSCRIBER_ID_TYPE_E;

typedef enum MN_VOICE_MAIL_TYPE_E
{
    MN_VOICE_MAIL,
    MN_VOICE_MAIL_FAX,
    MN_VOICE_MAIL_EMAIL,
    MN_VOICE_MAIL_OTHER
}MN_VOICE_MAIL_TYPE_E;
typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E     cause;       // the operate cause
    MN_MULTI_SUBSCRIBER_ID_TYPE_E line_id;   // record id
    MN_VOICE_MAIL_TYPE_E vm_type;   //voice mail type
    MN_SMS_VMW_MBNUM_T vmw_addr_t;  // the SMS voice message indicator
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_GET_VMW_ADDR_CNF_T;

/*
 the save SMS confirm structure , it respond the MNSMS_save_mo_sms() or MNSMS_save_mt_sms() 
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E     cause    ; // the operate cause
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_SET_VMW_ADDR_CNF_T;


typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E     cause;       // the operate cause
    MN_SMS_RECORD_ID_T record_id;   // the SMS position
    MN_SMS_STORAGE_E   storage;     // where the short message is saved
    MN_SMS_STATUS_E    status;      // the SMS state
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_EDIT_SMS_CNF_T;

/*
 set assert SMS destination address confirm structure 
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E     cause    ; // the operate cause
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_SET_ASSERT_SMS_DESTADDR_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E      cause;      // the operate cause
    MN_SMS_RECORD_ID_T  record_id;  // the SMS position     
    MN_SMS_STORAGE_E    storage;    // message class
    MN_SMS_MT_SMS_T     sms_t;      // the MT SMS structure   
    MN_DUAL_SYS_E       dual_sys;
} APP_MN_SMS_IND_T;

#define PROFILE_ID_1   0
#define PROFILE_ID_2   1
#define PROFILE_ID_3   2
#define PROFILE_ID_4   3
#define MAX_PROFILE_ID 4

typedef struct 
{
    uint8   msg_wait_ind_status;   
    uint8   voice_msg_wait_num;  
    uint8   fax_msg_wait_num;  
    uint8   email_msg_wait_num; 
    uint8   other_msg_wait_num;  
}MN_SMS_MWIS_T;

#define MNSMS_VMW_IN_NONE 0
#define MNSMS_VMW_IN_CPHS 1
#define MNSMS_VMW_IN_EFMWIS 2

typedef struct
{
    uint8   save_location;
    uint8   max_profile_num;
    MN_SMS_MWIS_T profile[MAX_PROFILE_ID];
}MN_SMS_VOICE_MSG_WAIT_T;

typedef struct
{
    BOOLEAN is_voice_ind;      //is_voice_ind &is_fax_ind &is_data_ind & is_other_ind can be TRUE in the same time
    BOOLEAN is_fax_ind;
    BOOLEAN is_data_ind;
    BOOLEAN is_other_ind;
    uint8   voice_cnt;
    uint8   fax_cnt;
    uint8   data_cnt;
    uint8   other_cnt;
}MN_SMS_VMW_IND_T;

typedef struct
{
    uint8   profile_ind;
    MN_SMS_VMW_IND_T profile[MAX_PROFILE_ID];
}MN_SMS_CUR_VMW_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_SMS_VMW_IND_FLAG_T   vmw_flag;
    MN_DUAL_SYS_E           dual_sys;
    MN_SMS_CUR_VMW_IND_T    vmw_ind;
} APP_MN_VMW_FALG_IND_T;

/*
 the state report indication structure
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_STORAGE_E        storage;        // where the SMS is saved
    MN_SMS_RECORD_ID_T      record_id;      // the relating SMS position
    MN_SMS_STATUS_REPORT_T  status_report;  // whether the short message is received by SME
    MN_L3_MSG_UNIT_T        msg_data;       // Only used for ATC module, Not decoded message body.
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_SMS_STATUS_REPORT_IND_T;

/*
 the load parameter confirm structure , it respond the MNSMS_load_param()
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E     cause    ;  // the operate cause
    MN_SMS_RECORD_ID_T record_id;  // the parameter position
    MN_SMS_PARAM_T     param_t;    // the parameter structure
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_READ_SMS_PARAM_CNF_T;

/*
 the update the paramter confirm sturcure , it respond the MNSMS_update_param()
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E     cause    ;  // the operate cause
    MN_SMS_RECORD_ID_T record_id;  // the parameter position
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_WRITE_SMS_PARAM_CNF_T;

/*
 give the result of setting SMS receive function , it respond the MNSMS_SetSmsReceiveFunc()
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E     cause    ;  // the operate cause
    BOOLEAN            is_receive_sms;  // the parameter position
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_SET_SMS_RECEIVE_FUNC_CNF_T;

/*
    the result of delete all short message
*/
typedef struct
{
    SIGNAL_VARS
    MN_SMS_CAUSE_E     cause    ;  // the operate cause
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_DELETE_ALL_SMS_CNF_T;


 /*****************************************************************************
 **                        SS MODULE DEFINE                                   *
 ******************************************************************************/

#define MN_SS_MAX_USSD_STRING_LENGTH_AFTER_ENCODE       160
#define MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE      182 
#define MN_SS_MAX_CALLING_NAME_LENGTH                   80 
#define MN_SS_MAX_NUM_OF_FEATURE_LIST       13
#define MN_SS_NUM_OF_PASSWORD           4

#define SS_MAX_RESULT_PARA_LEN      251


typedef enum _MN_SS_OPERATE_RESULT_E
{
    MN_SS_OPERATION_SUCC,   // operation success
    MN_SS_OPERATION_ERROR,  // operation error
    MN_SS_OPERATION_REJECT  // operation reject by network
} MN_SS_OPERATE_RESULT_E;   // the enum used to tell uplayer the result of operation

typedef enum _MN_SS_DCS_E
{
    MN_SS_DEFAULT_DCS_VALUE = 0x0F,
    MN_SS_7BIT_WITH_INDICATION_DCS = 0x10,
    MN_SS_UCS2_WITH_INDICATION_DCS = 0x11,
    MN_SS_USC2_NO_CLASS_DCS = 0x48,
    MN_SS_8BIT_NO_CLASS_DCS = 0x44,
    MN_SS_UTF8_NO_CLASS_DCS = 0x45,
    MN_SS_SPECIFIC_VALUE = 0xF0
} MN_SS_DCS_E;

/* defined in spec 29.002 Page 342, Supplementary Service codes */
typedef enum _MN_SS_CODE_E
{
    MN_SS_CLIP                          = 17,   // calling line identification presentation
    MN_SS_CLIR                          = 18,   // calling line identification restriction
    MN_SS_COLP                          = 19,   // connected line identification presentation
    MN_SS_COLR                          = 20,   // connected line identification restriction
    MN_SS_CNAP                          = 25,   // calling name presentation
    
    /*SS-Codes '00011010' to '00011111' are reserved for furture: name identification Supplementary Service*/
    MN_SS_ALL_FORWARD                   = 32,   // all call forwarding ss
    MN_SS_CFU                           = 33,   // call forwarding unconditional
    MN_SS_ALL_COND_FORWARD              = 40,   // all conditional forwarding SS
    MN_SS_CFB                           = 41,   // call forwarding on mobile subscriber busy
    MN_SS_CFNRY                         = 42,   // call forwarding on no reply
    MN_SS_CFNRC                         = 43,   // call forwarding on mobile subscriber not reachable
    MN_SS_CD                            = 44,  //communication deflection
    MN_SS_CFNL                        = 45,  //communication forwarding on not logged-in
    
    MN_SS_CW                            = 65,   // call waiting
    MN_SS_HOLD                          = 66,   // call hold
    MN_SS_CCBS                          = 67,   // completion of called busy subscriber   
    MN_SS_MC                            = 69,   // multicall
    
    MN_SS_MULTIPTY                      = 81,   // multiparty
    
    MN_SS_CUG                           = 97,   // closed user group
    
    MN_SS_AOCI                          = 113,  // advice of charge information
    MN_SS_AOCC                          = 114,  // advice of charge charging

    MN_SS_ALL_BAR_SERVICE               = 144,  // all barring service
    MN_SS_BAR_OUTGOING                  = 145,  // barring of outgoing calls
    MN_SS_BAOC                          = 146,  // barring of all outgoing calls
    MN_SS_BOIC                          = 147,  // barring of outgoing international calls 
    MN_SS_BOIC_EXHC                     = 148,  // barring of outgoing international calls except those directed to the home PLMN
    MN_SS_BAR_INCOMING                  = 153,  // barring of incoming calls
    MN_SS_BAIC                          = 154,  // barring of all incoming calls
    MN_SS_BICROAM                       = 155   // barring of incoming calls when roaming outside home PLMN Country
} MN_SS_CODE_E;

/* defined in spec 24.080 subclause 4.5 */
typedef enum _MN_SS_OPERATION_CODE_E
{
    MN_SS_REGISTERSS            = 10,
    MN_SS_ERASESS               = 11,
    MN_SS_ACTIVATESS            = 12,
    MN_SS_DEACTIVATESS          = 13,
    MN_SS_INTERROGATESS         = 14,
    MN_SS_NOTIFYSS              = 16,
    MN_SS_REGISTERPASSWORD      = 17,
    MN_SS_GETPASSWORD           = 18,
    MN_SS_PROCUNSTRUCTSS_DATA   = 19,
    MN_SS_FORWORKCHECK_IND      = 38,
    MN_SS_PROCUNSTRUCTSS_REQ    = 59,
    MN_SS_UNSTRUCTSS_REQ        = 60,
    MN_SS_UNSTRUCTSS_NOTIFY     = 61,
    MN_SS_ERASE_CCENTRY         = 77,
    MN_SS_CALL_DEFLECTION       = 117,
    MN_SS_USER_USERSERVICE      = 118,
    MN_SS_ACCESS_REGIST_CCENTRY= 119,
    MN_SS_FORWARDCUG_INFO       = 120,
    MN_SS_SPLIT_MPTY            = 121,
    MN_SS_RETRIEVE_MPTY         = 122,
    MN_SS_HOLD_MPTY             = 123,
    MN_SS_BUILD_MPTY            = 124,
    MN_SS_FORWARD_CHARGE_ADVICE = 125,
    MN_SS_EXPLICIT_CT           = 126,
    MN_SS_LCS_LOC_NOTIFY        = 116,
    MN_SS_LCS_MOLR              = 115
} MN_SS_OPERATION_CODE_E;

/* defined in spec 29.002 Page 358 */
typedef enum _MN_SS_BASIC_SERVICE_E
{
    MN_SS_ALL_SERVICE, 
    MN_SS_BEARER_SERVICE, 
    MN_SS_TELE_SERVICE
} MN_SS_BASIC_SERVICE_E; // only ss

/* defined in spec 29.002 Page 361 */
typedef enum _MN_SS_BEARER_SERVICE_CODE_E
{
    MN_SS_ALL_BEARER_SERVICES           = 0,
    MN_SS_ALL_DATADCA_SERVICES          = 16,
	MN_SS_ALL_DATADCS_SERVICES			= 24, /*servic code of vt in interrogat cnf*/
    MN_SS_ALL_PADACCESSCA_SERVICES      = 32,
    MN_SS_ALL_DATAPDS_SERVICES          = 40,
    MN_SS_ALL_ALTERNATE_SPEECH_DATACDA  = 48,
    MN_SS_ALL_ALTERNATE_SPEECH_DATACDS  = 56,
    MN_SS_SPEECH_FOLLOWED_DATACDA       = 64,
    MN_SS_SPEECH_FOLLOWED_DATACDS       = 72,
    MN_SS_ALL_DATA_CIRCUIT_ANSY         = 80,
    MN_SS_ALL_ANSY_SERVICES             = 96,
    MN_SS_ALL_DATA_CIRCUIT_SYNC         = 88,
    MN_SS_ALL_SYNC_SERVICES             = 104,
    MN_SS_ALL_PLMN_SPECIFIC_BS          = 208
} MN_SS_BEARER_SERVICE_CODE_E;

/* defined in spec 29.002 Page 360 */
typedef enum _MN_SS_TELE_SERVICE_CODE_E
{
    MN_SS_ALL_TELESERVICES              = 0,
    MN_SS_ALL_SPEECH_TRANS_SERVICES     = 16,
    MN_SS_TELEPHONE                     = 17,
    MN_SS_EMERGENCYCALL                 = 18,
    
    MN_SS_ALL_SHORT_MSG_SERVICE         = 32,
    MN_SS_SHORT_MSG_MTPP                = 33,
    MN_SS_SHORT_MSG_MOPP                = 34,
    
    MN_SS_ALL_FACSIMIL_TRANS_SERVICES   = 96,
    MN_SS_FACSIMIL3_ALTERNATE_SPEECH    = 97,
    MN_SS_AUTO_FACSIMIL3                = 98,
    MN_SS_FACSIMIL4                     = 99,
    
    MN_SS_ALL_DATA_TELESERVICES         = 112,
    MN_SS_TELESERVICES_EXP_SMS          = 128,
    
    MN_SS_AUXILIARY_SPEECH          = 208,
    MN_SS_AUXILIARY_TELEPHONY          = 209
} MN_SS_TELE_SERVICE_CODE_E;

/* defined in spec 29.002 Page 358 */
typedef struct _MN_SS_BASIC_SERVICE_T
{
    MN_SS_BASIC_SERVICE_E   service_type;   //bearerService or teleService
    uint16                  service_code;   // service code is bearerService code, while service_type = BEARER_SERVICE
                                            // service code is teleService code, while service_type = TELE_SERVICE
                                            // service code is invalid, while service_type = ALL_SERVICE
} MN_SS_BASIC_SERVICE_T;

typedef enum _MN_SS_PROBLEM_TAG_E 
{
    MN_SS_REJECT_PROBLEM_NONE = 0,      //default value, no problem code is included        
    MN_SS_GENERAL_PROBLEM_TAG = 128,
    MN_SS_INVOKE_PROBLEM_TAG = 129,
    MN_SS_RESULT_PROBLEM_TAG = 130,
    MN_SS_ERROR_PROBLEM_TAG = 131
}MN_SS_PROBLEM_TAG_E;

typedef enum _MN_SS_GENERAL_PROBLEM_CODE_E
{
	MN_SS_GENERAL_UNRECOGNIZED_COMPONENT_PROBLEM		= 0,
	MN_SS_GENERAL_MISTYPED_COMPONENT_PROBLEM			= 1,
	MN_SS_GENERAL_BADLY_STRUCTURED_COMPONENT_PROBLEM	= 2
} MN_SS_GENERAL_PROBLEM_CODE_E;

typedef enum _SS_INVOKE_PROBLEM_CODE_E
{
	MN_SS_INVOKE_DUPLICATE_INVOKE_ID_PROBLEM			= 0,
	MN_SS_INVOKE_UNRECOGNIZED_OPERATION_PROBLEM		= 1,
	MN_SS_INVOKE_MISTYPED_PARAM_PROBLEM				= 2,
	MN_SS_INVOKE_RESOURCE_LIMITATION_PROBLEM			= 3,
	MN_SS_INVOKE_INITIATING_RLEASE_PROBLEM				= 4,
	MN_SS_INVOKE_UNRECOGNIZED_LINKED_ID_PROBLEM		= 5,
	MN_SS_INVOKE_LINKED_RESPONSE_UNEXPECTED_PROBLEM	= 6,
	MN_SS_INVOKE_UNEXPECTED_LINKED_OPERATION			= 7
} SS_INVOKE_PROBLEM_CODE_E;

typedef enum _SS_RESULT_PROBLEM_CODE_E
{
	MN_SS_RESULT_UNRECOGNIZED_INVOKE_ID_PROBLEM	= 0,
	MN_SS_RESULT_RETURN_RESULT_UNEXPECTED_PROBLEM	= 1,
	MN_SS_RESULT_MISTYPED_PARAM_PROBLEM			= 2
} SS_RESULT_PROBLEM_CODE_E;

typedef enum SS_ERROR_PROBLEM_CODE_E
{
	MN_SS_ERROR_UNRECOGNIZED_INVOKE_ID_PROBLEM		= 0,
	MN_SS_ERROR_RETRURN_ERROR_UNEXPECTED_PROBLEM	= 1,
	MN_SS_ERROR_UNRECOGNIZED_ERROR_PROBLEM			= 2,
	MN_SS_ERROR_UNEXPECETED_ERROR_PROBLEM			= 3,
	MN_SS_ERROR_MISTYPED_PARAM_PROBLEM				= 4
} SS_ERROR_PROBLEM_CODE_E;


typedef struct _MN_SS_REJECT_T
{
    MN_SS_PROBLEM_TAG_E problem_tag;  //problem tag of err，for component result_type = reject
    uint8            problem_code;
}MN_SS_REJECT_T;


typedef enum _MN_SS_ERROR_CODE_E
{
    MN_SS_ERROR_NONE                                = 0, 
    MN_SS_ERROR_BEARER_SERVICE_NOT_PROVISIONED      = 10,
    MN_SS_ERROR_TELE_SERVICE_NOT_PROVISIONED        = 11,
    MN_SS_ERROR_CALL_BARRED                         = 13,
    MN_SS_ERROR_ILLEGAL_SS_OPERATION                = 16,
    MN_SS_ERROR_ERROR_SS_STATUS                     = 17,
    MN_SS_ERROR_SS_UNAVAILABLE                      = 18,
    MN_SS_ERROR_SS_SUBSCRIPTION_VIOLATION           = 19,
    MN_SS_ERROR_SS_INCOMPATIBILITY                  = 20,
    MN_SS_ERROR_FACILITY_NOT_SUPPORTED              = 21,
    MN_SS_ERROR_SS_ABSENT_SUBSCIRBER                = 27,
    MN_SS_ERROR_SHORT_TERMDENIAL                    = 29,
    MN_SS_ERROR_LONG_TERMDENIAL                     = 30,
    MN_SS_ERROR_SYSTEM_FAILURE                      = 34,
    MN_SS_ERROR_DATA_MISSING                        = 35,
    MN_SS_ERROR_UNEXPECTED_DATA_VALUE               = 36,
    MN_SS_ERROR_PW_REGISTRATION_FAILURE             = 37,
    MN_SS_ERROR_NEGATIVE_PW_CHECK                   = 38,
    MN_SS_ERROR_TEMP_FAILURE                        = 41,
    MN_SS_ERROR_NUMBER_OF_PW_ATTEMPTS_VIOLATION     = 43,
    MN_SS_ERROR_UNKNOW_ALPHABET                     = 71,
    MN_SS_ERROR_USSD_BUSY                           = 72,
    MN_SS_ERROR_INVAILD_DEFLECTED_NUMBER            = 125,
    MN_SS_ERROR_SS_MAX_NUM_MPTR_EXCEEDED            = 126,
    MN_SS_ERROR_RESOURCE_UNAVAILABLE                = 127
} MN_SS_ERROR_CODE_E;

/* defined in spec 29.002 Page 338 */
typedef enum _MN_SS_STATUS_E
{
    MN_SS_NOT_PROVITION    = 0,        // bit3 = 0;
    MN_SS_NOT_REGISTERED   = 0x04,      // bit3 = 1 but bit2 = 0;
    MN_SS_PROVITION        = 0x05,
    MN_SS_NOT_ACTIVE       = 0x06,      // bit3,bit2 all= 1, bit1 = 0;
    MN_SS_ACTIVE_OPERATIVE = 0x07,      // bit3,bit2,bit1 = 1, bit4 = 0;
    MN_SS_ACITVE_QUIESCENT = 0x0F      // bit4,bit3,bit2,bit1 all equal to 1
} MN_SS_STATUS_E;

/* defined in spec 24.080 Page 27 */
typedef enum
{
    MN_SS_OUTGOING_FORWARED_TO_C,   // outgoing call has been forwarded to C
    MN_SS_INCOMING_FORWARED_TO_C,   // incoming call has been forwarded to C
    MN_SS_INCOMING_IS_FORWARED      // incoming call is a forwarded call
} MN_SS_FORWARD_NOTIFY_E;

/* defined in spec 29.002 Page 340 */
typedef struct _MN_SS_USSD_DATA_T
{
    MN_SS_DCS_E             data_code_scheme;   // just MMI mode now, set to default value: 0x0F(default alphabet and Language unspecified)
    BOOLEAN               class_is_present;
    MN_SMS_CLASS_E  ussd_class;    // added for orange case
    uint8                   ussd_str[MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE];   // default alphabet: character table in 03.38 6.2.1
    uint16                  str_len;            // the length of ussd-string
}MN_SS_USSD_DATA_T;

/* defined in spec 29.002 Page 340 */
typedef enum _MN_SS_PASSWORD_GUIDANCE_INFO_E
{
    MN_SS_ENTER_PW              = 0,
    MN_SS_ENTER_NEW_PW          = 1,
    MN_SS_ENTER_NEW_PW_AGAIN    = 2
} MN_SS_PASSWORD_GUIDANCE_INFO_E;

typedef enum _MN_SS_CLIRESTRICTION_OPTION_E
{
    MN_SS_CLIR_PERMANENT,              // 主叫线路显示永久禁止
    MN_SS_CLIR_TMP_DEFAULT_RESTRICTED, // 主叫线路显示暂时禁止，缺省为显示禁止
    MN_SS_CLIR_TMP_DEFAULT_ALLOWED     // 主叫线路显示暂时禁止，缺省为显示允许
}   MN_SS_CLIRESTRICTION_OPTION_E;

typedef enum MN_GMMREG_RAT_E

{
    MN_GMMREG_RAT_GPRS = 0,
    MN_GMMREG_RAT_3G = 1,
    MN_GMMREG_RAT_TD = MN_GMMREG_RAT_3G,
    MN_GMMREG_RAT_GPRS_AND_3G,
    MN_GMMREG_RAT_4G,
    MN_GMMREG_RAT_UNKNOWN,
    /* MN_GMMREG_RAT_VDS_GSM is used for virtual dual standby funtion,refer to CR178284.
       In dual mode, ms will be always in gsm when there has none PDP activated and 
       can reselect TD cell when there has PDP activated. 
     */
    MN_GMMREG_RAT_VDS_GSM = 5   
} MN_GMMREG_RAT_E;

typedef enum MN_MM_CAUSE_E
{
    MN_MM_CAUSE_NONE = 0x00,
    MN_MM_IMSI_UNKNOWN_IN_HLR = 0x02, 
    MN_MM_ILLEGAL_MS = 0x03,
    MN_MM_IMSI_UNKNOWN_IN_VLR =     0x04,
    MN_MM_IMEI_NOT_ACCEPTED              =     0x05,
    MN_MM_ILLEGAL_ME                     =     0x06,
    MN_MM_GPRS_NOT_ALLOWED               =     0x07,
    MN_MM_GPRS_NON_GPRS_NOT_ALLOWED      =     0x08,
    MN_MM_MS_ID_NOT_DERIVED_BY_NW        =     0x09,
    MN_MM_IMPLICIT_DETACH                =     0x0a,
    MN_MM_PLMN_NOT_ALLOWED               =     0x0b,
    MN_MM_LOCATION_AREA_NOT_ALLOWED      =     0x0c,
    MN_MM_ROAMING_AREA_NOT_ALLOWED       =     0x0d,
    MN_MM_GPRS_NOT_ALLOWED_IN_PLMN       =     0x0e,
    MN_MM_NO_SUITABLE_CELL_IN_LA         =     0x0f,
    MN_MM_MSC_TEMP_NOT_REACHABLE         =     0x10,
    MN_MM_NETWORK_FAILURE                =     0x11,
    MN_MM_MAC_FAILURE                    =     0x14,
    MN_MM_SYNC_FAILURE                   =     0x15,
    MN_MM_CONGESTION                     =     0x16,
    MN_MM_GSM_AUTH_UNACCEPTABLE          =     0x17,
    MN_MM_SERV_OPTION_NOT_SUPPORTED      =     0x20,
    MN_MM_REQ_SERV_OPTION_NOT_SUBSCRIBED =     0x21,
    MN_MM_SERV_OPTION_TEMP_OUT_OF_ORDER  =     0x22,
    MN_MM_CALL_CANNOT_BE_IDENTIFIED      =     0x26,
    MN_MM_NO_PDP_CONTEXT_ACTIVATED       =     0x28,

    MN_MM_RETRY_UPON_ENTRY_INTO_NEWCELL_MIN =  0x30,  /* 30..3f */
    MN_MM_RETRY_UPON_ENTRY_INTO_NEWCELL_MAX =  0x3f,

    MN_MM_SEMANTICALLY_INCORRECT_MSG        =  0x5f,
    MN_MM_INVALID_MM_MAND_INFO              =  0x60,
    MN_MM_MSG_TYPE_NON_EXISTENT             =  0x61,
    MN_MM_MSG_TYPE_NOT_IMPLEMENTED          =  0x61,
    MN_MM_MSG_TYPE_INCOMPAT_WITH_PROTO_STATE = 0x62,
    MN_MM_IE_NOT_IMPLEMENTED                 = 0x63,
    MN_MM_CONDITIONAL_MM_IE_ERROR            = 0x64,
    MN_MM_MSG_NOT_COMPAT_WITH_PROTO_STATE    = 0x65,  /* 65..6f */
    MN_MM_PROTO_ERROR_UNSPECIFIED            = 0x6f,
    /* Incase if any more standard cause values are to be added then
     * the cause having the greatest value should be placed just before
     * the internal cause values.
     */
    /* The following cause are internal to Tcm and Gmm */
    MN_MM_FORBIDDEN_PLMN,
    MN_MM_ACCESS_CLASS_BARRED,
    MN_MM_NO_COVERAGE,
    MN_MM_GPRS_SERV_NOT_ALLOWED,
    MN_MM_TIMER_EXPIRY,
    MN_MM_SIM_INSERTED,
    MN_MM_SIM_REMOVED,
    MN_MM_SIM_ABSENT,
    MN_MM_SIM_INVALID_FOR_PS,
    MN_MM_SIM_INVALID_FOR_CS,
    MN_MM_SIM_INVALID_FOR_CS_AND_PS,
    MN_MM_LOW_LAYER_FAIL,
    MN_MM_PLMN_SEARCH_ABORT_DUE_TO_MT_CALL,
    MN_MM_PLMN_SEARCH_ABORT_DUE_TO_PS_PAGING,
    MN_MM_CELL_INVALID_FOR_CS,
    MN_MM_CELL_INVALID_FOR_CS_AND_PS,
    MN_MM_PLMN_SEL_ABORT_DUE_TO_ACTIVE_CALL,
    MN_MM_NO_COVERAGE_LIMITED_SERVICE,
    MN_MM_PLMN_SEL_ABORT_DUE_TO_AS_UNREADINESS, /* DM integ */
    MN_MM_BAND_SEL_ABORT_DUE_TO_ACTIVE_CALL,
    MN_MM_BAND_SEL_ABORT_DUE_TO_SIM_FAILURE
}MN_MM_CAUSE_E;


typedef struct _MN_SS_CFNRY_PARAM_T
{
    MN_CALLED_NUMBER_T                  forward_num;    // forward to called party BCD number
    uint8                               no_reply_time;  // no reply time of CFNRY service
}   MN_SS_CFNRY_PARAM_T;

typedef union _MN_SS_REGISTER_PARAM_U
{
    MN_CALLED_NUMBER_T      forward_num;    // the forward to number of all Forward service except call forward no reply
    MN_SS_CFNRY_PARAM_T     cfnry_param;    // parameter of call forward no reply service
    uint8                   Nbr_user;       // max number of Circuit Switch bearer
}   MN_SS_REGISTER_PARAM_U;

typedef struct
{
    uint8    para_array[SS_MAX_RESULT_PARA_LEN];
    uint16   len;
}MN_SS_RESULT_PARA_T;

typedef struct _MN_SS_INTERROGATE_INFO_T
{
    BOOLEAN                                 ss_status_present;
    BOOLEAN                                 forward_num_present;
    BOOLEAN                                 forward_subaddr_present;
    BOOLEAN                                 no_reply_time_present;
    BOOLEAN                                 clir_info_present;
    MN_SS_STATUS_E                          ss_status;
    MN_SS_BASIC_SERVICE_T                   basic_service;
    MN_CALLED_NUMBER_T                      forward_num;
    MN_SUBADDR_T                            forward_subaddr;
    uint8                                   no_reply_time;
    MN_SS_CLIRESTRICTION_OPTION_E           clir_info;
    uint8                                   ccbs_index; 
}  MN_SS_INTERROGATE_INFO_T;

typedef enum _MN_SS_CALL_WAITING_STATUS_E 
{
    MN_SS_CALL_WAITING_UNKNOWN,
    MN_SS_CALL_WAITING_ENABLE,
    MN_SS_CALL_WAITING_DISABLE
}MN_SS_CALL_WAITING_STATUS_E;

typedef struct
{
    SIGNAL_VARS
    MN_SS_OPERATE_RESULT_E  result_type;
    MN_SS_ERROR_CODE_E      err_code;
    MN_DUAL_SYS_E           dual_sys;
    MN_SS_REJECT_T          reject_problem;  //when result_type is reject, it is included
}   APP_MN_CCBS_ERASE_CNF_T;
typedef struct
{
    SIGNAL_VARS
    uint8                   call_id;
    MN_SS_OPERATE_RESULT_E  result_type;
    MN_SS_ERROR_CODE_E      err_code;
    uint8                   ccbs_index; /* 0: invalid, 1-5: valid */
    MN_SS_BASIC_SERVICE_T   basic_service;
    BOOLEAN                 remote_num_present;
    MN_CALLED_NUMBER_T      remote_num;
    BOOLEAN                 remote_sub_addr_present;
    MN_SUBADDR_T            remote_sub_addr;
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_CCBS_ACTIVATE_IND_T;
typedef struct
{
     SIGNAL_VARS 
     uint8                              call_id;
     uint8                              ccbs_index; /* 0: invalid, 1-5: valid */
     BOOLEAN                            remote_num_present;
     MN_CALLED_NUMBER_T                 remote_num;
     BOOLEAN                            remote_sub_addr_present;
     MN_SUBADDR_T                       remote_sub_addr;    
     uint8                              alertingPattern;
     MN_DUAL_SYS_E                      dual_sys;
} APP_MN_CCBS_RECALL_IND_T;

/* define signals between App-layer with MN-layer */
typedef struct{
    SIGNAL_VARS
    MN_DUAL_SYS_E      dual_sys;
}   APP_MN_BEGIN_REGISTER_SS_T,
    APP_MN_BEGIN_ERASE_SS_T,
    APP_MN_BEGIN_ACTIVATE_SS_T,
    APP_MN_BEGIN_DEACTIVATE_SS_T,
    APP_MN_BEGIN_INTERROGATE_SS_T,
    APP_MN_BEGIN_REGISTER_PASSWORD_T,
    APP_MN_SS_BUSYING_IND_T;

typedef struct{
    SIGNAL_VARS
    MN_SS_OPERATE_RESULT_E              result_type;    // success, error or reject
    uint8                               list_num;       // number of interrogate information structure
    MN_SS_INTERROGATE_INFO_T            info_list[MN_SS_MAX_NUM_OF_FEATURE_LIST];
    MN_SS_ERROR_CODE_E                  err_code;       // if cnf-type = cnf_error, the parameter is included
    MN_DUAL_SYS_E                       dual_sys;
    uint8                               result_octs_len;
    uint8                               result_octs[MN_SS_MAX_NUM_OF_ADD_DATA];         //octs result without decoding
    MN_SS_REJECT_T                      reject_problem;  //when result_type is reject, it is included
}   APP_MN_REGISTER_SS_CNF_T,
    APP_MN_ERASE_SS_CNF_T, 
    APP_MN_ACTIVATE_SS_CNF_T, 
    APP_MN_DEACTIVATE_SS_CNF_T;

typedef struct{
    SIGNAL_VARS
    MN_SS_OPERATE_RESULT_E              result_type;    // success, error or reject
    uint8                               list_num;       // number of interrogate information structure
    MN_SS_INTERROGATE_INFO_T            info_list[MN_SS_MAX_NUM_OF_FEATURE_LIST];
    MN_SS_ERROR_CODE_E                  err_code;       // if cnf-type = cnf_error, the parameter is included
    MN_DUAL_SYS_E                       dual_sys;
    uint8                               result_octs_len;
    uint8                               result_octs[MN_SS_MAX_NUM_OF_ADD_DATA];         //octs result without decoding
    MN_SS_CODE_E			ss_code;
    MN_SS_REJECT_T                      reject_problem;  //when result_type is reject, it is included
}   APP_MN_INTERROGATE_CNF_T;

typedef struct{
    SIGNAL_VARS
    MN_SS_OPERATE_RESULT_E  result_type;    // success, error or reject
    MN_SS_ERROR_CODE_E      err_code;       // if cnf-type = cnf_error, the parameter is included
    MN_DUAL_SYS_E           dual_sys;
    MN_SS_REJECT_T          reject_problem;  //when result_type is reject, it is included
}   APP_MN_REGISTER_PASSWORD_CNF_T;

typedef struct{
    SIGNAL_VARS
    MN_SS_PASSWORD_GUIDANCE_INFO_E      guidance_info;  // guide user to input password which is needed
    MN_DUAL_SYS_E                       dual_sys;
}   APP_MN_GET_PASSWORD_IND_T;

typedef struct{
    SIGNAL_VARS
    MN_SS_USSD_DATA_T                   ussd_data;
    MN_DUAL_SYS_E                       dual_sys;
	uint8                               ussd_action;
}   APP_MN_USSD_IND_T, 
    APP_MN_USSD_NOTIFY_IND_T,
    APP_MN_BEGIN_USSD_SERVICE_T;

typedef struct{
    SIGNAL_VARS
    MN_SS_OPERATE_RESULT_E              result_type;    // success, error or reject
    MN_SS_USSD_DATA_T                   ussd_data;      // result data
    MN_SS_ERROR_CODE_E                  err_code;       // if cnf-type = cnf_error, the parameter is included
    MN_DUAL_SYS_E                       dual_sys;
	uint8                               ussd_action;
    MN_SS_REJECT_T                      reject_problem;  //when result_type is reject, it is included
}   APP_MN_USSD_SERVICE_CNF_T;

typedef struct{
    SIGNAL_VARS
    BOOLEAN                             error_present;      // whether gsm cause present in signal
    MN_SS_ERROR_CODE_E                  error_code;         // release cause
    MN_DUAL_SYS_E                       dual_sys;
}   APP_MN_RELEASE_SS_IND_T;        // this signal used network release ss without ss operation

typedef struct
{
    SIGNAL_VARS
    MN_SS_ERROR_CODE_E cause;
}   APP_MN_REJ_USSD_SERVICE_REQ_T;

 /*****************************************************************************
 **                        PHONE MODULE DEFINE                                *
 ******************************************************************************/
 
// used for MAX_PLMN_INFO_LIST_NUM,max plmn num in list
#define MN_PHONE_MAX_PLMN_INFO_LIST_NUM 16

#define MN_PHONE_PLMN_SEL_LIST_NUM      85

#define MN_PHONE_EHPLMN_LIST_NUM        85

#define MN_PHONE_FPLMN_LIST_NUM         128

#define MN_PHONE_EPLMN_LIST_NUM         16  // ref:24.008(Release6) section4.4.1

typedef enum _MN_GMM_ATTACH_E
{
	MN_GMM_NOT_ATTACH,
	MN_GMM_ATTACHING,
	MN_GMM_ATTACHED
} MN_GMM_ATTACH_E;

typedef enum _MN_PHONE_GPRS_ATTACH_TYPE_E
{
    MN_PHONE_GPRS_ATTACH,
    MN_PHONE_GPRS_COMBINED_ATTACH
} MN_PHONE_GPRS_ATTACH_TYPE_E;

typedef enum _MN_PHONE_GPRS_DETACH_TYPE_E
{
    MN_PHONE_GPRS_DETACH ,
    MN_PHONE_GPRS_IMSI_DETACH,
    MN_PHONE_GPRS_COMBINED_DETACH
} MN_PHONE_GPRS_DETACH_TYPE_E;

typedef uint8 MN_PHONE_ROAMING_MODE_E;
#define MN_PHONE_ROAMING_DISABLE            0x00
#define MN_PHONE_ROAMING_ENABLE             0x01

// user service type
typedef uint8   MN_PHONE_USER_SERVICE_SELECT_E;
#define MN_PHONE_USER_SELECT_NONE           0x00
#define MN_PHONE_USER_SELECT_GSM_ONLY       0x01
#define MN_PHONE_USER_SELECT_GPRS_ONLY      0x02
#define MN_PHONE_USER_SELECT_GSM_GPRS_BOTH  (MN_PHONE_USER_SELECT_GSM_ONLY|MN_PHONE_USER_SELECT_GPRS_ONLY)
#define MN_PHONE_USER_SERVICE_MAX_NUM       0x04



typedef struct _MN_LOCI_T
{
    MN_TMSI_T           tmsi;
    MN_LAI_T            lai;
    uint8               tmsi_time;
    MN_LOCI_STATUS_E    update_status;
} MN_LOCI_T;

 typedef enum _MN_PHONE_PLMN_LIST_TYPE_E
{
	MN_PHONE_PLMN_LIST_UPLMN,
	MN_PHONE_PLMN_LIST_OPLMN,
	MN_PHONE_PLMN_LIST_HPLMN
}MN_PHONE_PLMN_LIST_TYPE_E;

 typedef enum _MN_OPN_DCS_E
 {
	 MN_OPN_DCS_8BIT,
	 MN_OPN_DCS_UCS2
 }MN_OPN_DCS_E;

typedef struct  _MN_PHONE_WRITE_PLMN_T
{
    BOOLEAN                         is_delete_item;
    MN_PHONE_PLMN_LIST_TYPE_E	    plmn_list_type;
    uint32                          order_of_plmn;
    MN_PLMN_T                       plmn;
    BOOLEAN                         is_gsm_act;
    BOOLEAN                         is_gsm_compact_act;
    BOOLEAN                         is_td_act;
} MN_PHONE_WRITE_PLMN_T;

typedef struct  _MN_PHONE_UPDATE_FPLMN_T
{
    BOOLEAN                         is_delete_item;
    MN_PLMN_T                       plmn;
} MN_PHONE_UPDATE_FPLMN_T;

typedef enum _MN_PHONE_PLMN_SELECT_MODE_E
{
    MN_PHONE_PLMN_SELECT_MANUAL_MODE,
    MN_PHONE_PLMN_SELECT_AUTO_MODE
} MN_PHONE_PLMN_SELECT_MODE_E;

typedef struct
{
    MN_PHONE_PLMN_SELECT_MODE_E     select_mode;
    MN_PLMN_T        manual_plmn;
    MN_GMMREG_RAT_E  rat;
} MN_PHONE_SELECT_PLMN_T;

// plmn detail
typedef struct _MN_PHONE_PLMN_DETAIL_T
{
    MN_PLMN_T        plmn;               // mnc and mcc
    BOOLEAN          plmn_is_registered; // registered plmn?
    BOOLEAN          plmn_is_forbidden;  // forbidden plmn?
    BOOLEAN          plmn_is_home;       // home plmn?
    BOOLEAN          plmn_is_weak;       // true when cell power is 0 
} MN_PHONE_PLMN_DETAIL_T;

// used in APP_MN_PLMN_LIST_CNF_T,information about all plmns
typedef struct _MN_PHONE_PLMN_INFO_LIST_T
{
    uint8                  plmn_num;                           // plmn_num
    MN_PHONE_PLMN_DETAIL_T plmn_details[MN_PHONE_MAX_PLMN_INFO_LIST_NUM]; // plmn_detail
    MN_GMMREG_RAT_E        rat[MN_PHONE_MAX_PLMN_INFO_LIST_NUM];
} MN_PHONE_PLMN_INFO_LIST_T;

// plmn status
typedef enum _MN_PLMN_STATUS_E
{
    PLMN_NO_SERVICE = 0 ,          // no service
    PLMN_NORMAL_GSM_ONLY = 0x01 ,     // voice service available
    PLMN_NORMAL_GPRS_ONLY = 0x02 ,    // data service available    
    PLMN_NORMAL_GSM_GPRS_BOTH = 0x03 ,// voice and data service available

    PLMN_NORMAL_CS_ONLY = 0x01 ,     // voice service available
    PLMN_NORMAL_PS_ONLY = 0x02 ,    // data service availabe    
    PLMN_NORMAL_CS_PS_BOTH = 0x03 ,// voice and data service available

    PLMN_EMERGENCY_ONLY = 0x04,       // emergency service available

    PLMN_EMERGENCY_SIM_INVALID = 0x05,        /* emergency; MM in limited service state and
                                               no further PLMN access allowed until power
                                             off or new SIM inserted */
    PLMN_EMERGENCY_GPRS_ONLY = 0x06,      // data service availabe but emergency; MM in limited service state 
    PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY = 0x07,   /* data service availabe but emergency; MM in limited service state and
                                                  no further PLMN access allowed until power
                                               off or new SIM inserted */

    PLMN_REGISTER_SERVICE = 0x08,    //attaching after camping on   
    PLMN_REGISTER_GPRS_ONLY = 0x09, // data service availabe but attaching;  
    PLMN_FULL_PS_SERVICE = 0x0A       /*full PS service, no cs service*/

//  PLMN_ACCESS_DIFFICULTY,   /* normal; could be in updated or not updated */
//  PLMN_FORBIDDEN,           // emergency; MM in limited service state */
//  PLMN_LA_FORBIDDEN,        /* emergency; MM in limited service state */
//  PLMN_NAT_ROAM_FORBIDDEN,  /* emergency; MM in limited service state */
//  PLMN_ALL_FORBIDDEN,       /* emergency; MM in limited service state and
//                                no further PLMN access allowed until power
//                              off or new SIM inserted */
//  PLMN_MM_CONNECTION_IN_USE /* normal; MM (MT call) conn in progress */
} MN_PHONE_PLMN_STATUS_E;

typedef struct _MN_PHONE_CURR_CELL_CAPABILITY_T
{
	BOOLEAN       mbms_support;
	BOOLEAN       hsdpa_support;
	BOOLEAN       hsupa_support;
	BOOLEAN       edge_support;
} MN_PHONE_CURR_CELL_CAPABILITY_T;

typedef struct _MN_PHONE_CELL_MEAS_T
{
    BOOLEAN cell_present;
    uint16  arfcn;
    uint8   bsic;
    uint8   rxlev;
    
    BOOLEAN identity_present;
    MN_LAI_T lai;
    uint32  cell_id;
    BOOLEAN  fake_cell_flag;
} MN_PHONE_CELL_MEAS_T;

typedef struct _MN_PHONE_SCELL_MEAS_T
{
    uint16  arfcn;
    uint8   bsic;
    BOOLEAN is_traffic_mode;
    // idle info
    uint8   rxlev;
    // traffic info
    BOOLEAN is_dtx_used;
    uint16  rxqual_full;
    uint16  rxqual_sub;
    uint16  rxlev_full;
    uint16  rxlev_sub;
    uint32  cell_id;
    uint16  lac;
} MN_PHONE_SCELL_MEAS_T;

typedef struct _MN_PHONE_PLMN_SEL_LIST_T
{
    uint32      plmn_validate_num;
    uint32      plmn_max_num;
    MN_PLMN_T   plmn_arr[MN_PHONE_PLMN_SEL_LIST_NUM];
    MN_GMMREG_RAT_E rat[MN_PHONE_PLMN_SEL_LIST_NUM];
} MN_PHONE_PLMN_SEL_LIST_T;

typedef struct _MN_PHONE_FPLMN_LIST_T
{
    uint32      plmn_num;
    MN_PLMN_T   plmn_arr[MN_PHONE_FPLMN_LIST_NUM];
    MN_GMMREG_RAT_E rat[MN_PHONE_FPLMN_LIST_NUM];
} MN_PHONE_FPLMN_LIST_T;

typedef struct _MN_PHONE_EHPLMN_LIST_T
{
    uint32      plmn_num;
    MN_PLMN_T   plmn_arr[MN_PHONE_EHPLMN_LIST_NUM];
} MN_PHONE_EHPLMN_LIST_T;

typedef struct _MN_PHONE_EPLMN_LIST_T
{
    uint32      plmn_num;
    MN_PLMN_T   plmn_arr[MN_PHONE_EPLMN_LIST_NUM];
} MN_PHONE_EPLMN_LIST_T;  

//  struct current plmn info
typedef struct _MN_CURRENT_PLMN_INFO_T
{
    MN_PHONE_PLMN_STATUS_E  plmn_status;        // plmn statue          
    BOOLEAN                 plmn_is_roaming;    // if plmn is roaming plmn  
    uint16                  mcc;
    uint16                  mnc;
    uint16                  mnc_digit_num;
    uint16                  lac;
    uint32                  cell_id;
    MN_GMMREG_RAT_E         rat;
    MN_PHONE_CURR_CELL_CAPABILITY_T             cell_capability;
} MN_PHONE_CURRENT_PLMN_INFO_T;

typedef uint8 MN_PHONE_PREF_MODE_E;
#define MN_PHONE_PREF_MODE_GSM              0x00
#define MN_PHONE_PREF_MODE_GPRS             0x01
#define MN_PHONE_PREF_MODE_MAX_NUM          0x02

typedef uint8 MN_PHONE_BAND_TYPE_E;
#define MN_PHONE_BAND_GSM                       0x00
#define MN_PHONE_BAND_DCS                       0x01
#define MN_PHONE_BAND_GSM_DCS                   0x02
#define MN_PHONE_BAND_PCS                       0x03
#define MN_PHONE_BAND_GSM850                    0x04
#define MN_PHONE_BAND_GSM_PCS                   0x05
#define MN_PHONE_BAND_GSM850_DCS                0x06
#define MN_PHONE_BAND_GSM850_PCS                0x07
#define MN_PHONE_BAND_GSM850_GSM                0x08
#define MN_PHONE_BAND_GSM850_GSM_PCS            0x09
#define MN_PHONE_BAND_GSM850_GSM_DCS            0x0a
#define MN_PHONE_BAND_GSM_DCS_PCS               0x0b
#define MN_PHONE_BAND_GSM850_GSM_DCS_PCS        0x0c
#define MN_PHONE_BAND_DCS_PCS                   0x0d
#define MN_PHONE_BAND_GSM850_DCS_PCS            0x0e
#define MN_PHONE_BAND_NULL                      0x0f
#define MN_PHONE_BAND_MAX_NUM                   0x10

typedef enum _PLMN_RESELECT_CAUSE_E
{
	MN_RESEL_FOR_PREF_CHG,
	MN_RESEL_FOR_BAND_CHG,
	MN_RESEL_FOR_SRV_TYPE_CHG,
	MN_RESEL_FOR_ROAM_MODE_CHG,
	MN_RESEL_FOR_BAND_CHG_WITHOUT_SAVING
} PLMN_RESELECT_CAUSE_E;

typedef enum _MN_OPERATION_E
{   
    MN_SS_OPERATION,
    MN_USSD_OPERATION,
    MN_SMS_OPERATION,
    MN_CALL_OPERATION,
    MN_UNKONOWN_OPERATION
} MN_OPERATION_E;

typedef enum _MN_CARD_TYPE_E
{
    MN_CARD_TYPE_SIM = 0x1,
    MN_CARD_TYPE_USIM,
    MN_CARD_TYPE_DUAL,
    MN_CARD_TYPE_UIM,
    MN_CARD_TYPE_NUM
} MN_CARD_TYPE_E;

typedef enum _MN_SIM_TYPE_CAUSE_E
{
    MN_SIM_TYPE_SUCCESS,
    MN_SIM_TYPE_NOT_READY,
    MN_SIM_TYPE_NO_SIM,
    MN_SIM_TYPE_FAILED
} MN_SIM_TYPE_CAUSE_E;

typedef enum _MN_UIM_IMSI_CAUSE_E
{
    MN_UIM_IMSI_RESULT_OK,
    MN_UIM_IMSI_RESULT_NO_CARD,
    MN_UIM_IMSI_RESULT_CARD_NOT_READY,
    MN_UIM_IMSI_RESULT_FILE_NOT_EXIST,        
    MN_UIM_IMSI_RESULT_FAILED
} MN_UIM_IMSI_CAUSE_E;

typedef struct
{
    uint16      code;          //ss service code
    uint8       old_pwd[MN_SS_NUM_OF_PASSWORD]; //old password
    uint8       new_pwd[MN_SS_NUM_OF_PASSWORD]; //new password
    uint8       new_pwd_two[MN_SS_NUM_OF_PASSWORD]; //new pasword
}MN_SS_PWD_INFO_T;

typedef struct _MN_SS_BASIC_INFO_T
{
    MN_SS_BASIC_SERVICE_T       basic_service;  // the supplementary service used on which type service(bearer or tele or all call type)
    MN_SS_REGISTER_PARAM_U      register_param;
} MN_SS_BASIC_INFO_T;

typedef struct _MN_SS_INFO_U
{
    MN_SS_BASIC_INFO_T          ss_basic_info;
    MN_SS_PWD_INFO_T            ss_pwd_info;
} MN_SS_INFO_U;

typedef struct _MN_SS_PARSE_INFO_T
{
    MN_SS_OPERATION_CODE_E      ss_oper;
    MN_SS_CODE_E                ss_code;        // supplementary service code
    MN_SS_INFO_U                ss_info_u;
} MN_SS_PARSE_INFO_T;

typedef struct _MN_CALL_PARSE_INFO_T
{
    uint8           len;
    uint8           dial_num_str[MN_MAX_ADDR_BCD_LEN];
} MN_CALL_PARSE_INFO_T;

typedef union _MN_MODULE_PARSE_INFO_U
{
    MN_SS_PARSE_INFO_T      ss_parse_info;
    MN_SS_USSD_DATA_T       ussd_parse_info;
    MN_CALL_PARSE_INFO_T    call_parse_info;
} MN_PARSE_INFO_U;

// used for SIM-phone
typedef enum _MN_MSISDN_OPERATE_TYPE_E
{
    MN_MSISDN_UPDATE,
    MN_MSISDN_ERASE
} MN_MSISDN_OPERATE_TYPE_E;

typedef enum _MN_ADN_OPERATE_TYPE_E
{
    MN_ADN_UPDATE,
    MN_ADN_ERASE
} MN_ADN_OPERATE_TYPE_E;

typedef enum _MN_FDN_OPERATE_TYPE_E
{
    MN_FDN_UPDATE,
    MN_FDN_ERASE
} MN_FDN_OPERATE_TYPE_E;

typedef enum _MN_LND_OPERATE_TYPE_E
{
    MN_LND_UPDATE,
    MN_LND_ERASE
} MN_LND_OPERATE_TYPE_E;

//according to SIM_RESULT_UPDATE_FILE_E enum
typedef enum
{
    MN_SIM_FILE_NO_ERR,
    MN_SIM_FILE_INVALID,
    MN_SIM_FILE_INCONSISTENT,
    MN_SIM_FILE_NOT_EXIST,
    MN_SIM_FILE_CHV1_NEEDED,
    MN_SIM_FILE_CHV2_NEEDED,
    MN_SIM_FILE_U_CHV1_NEEDED,
    MN_SIM_FILE_U_CHV2_NEEDED,
    MN_SIM_FILE_CHV_BLOCKED,
    MN_SIM_FILE_ACCESS_NEVER,
    MN_SIM_FILE_UNKNOW
} MN_RESULT_UPDATE_FILE_E;

// following signal only have signal header
typedef xSignalHeaderRec  SIGNALS_NO_PARA_T;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
} APP_MN_MS_POWERING_OFF_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
} APP_MN_DEACTIVE_PS_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
} APP_MN_PS_POWER_ON_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
} APP_MN_PS_POWER_OFF_CNF_T;

//  struct of signal: APP_MN_PS_READY_IND_T
//  this signal indicate MMI that MS waiting for MMI's command now
typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
} APP_MN_PS_READY_IND_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN  is_ok;
    MN_DUAL_SYS_E dual_sys;
} APP_MN_SET_PREF_MODE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN         is_ok;
    MN_DUAL_SYS_E   dual_sys;
} APP_MN_SET_BAND_CNF_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN  is_ok;
    MN_DUAL_SYS_E dual_sys;
} APP_MN_SET_SERVICE_TYPE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN         is_ok;
    MN_DUAL_SYS_E   dual_sys;
} APP_MN_SET_ROAM_MODE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_SIM_TYPE_CAUSE_E     cause;
    MN_CARD_TYPE_E          card_type;
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_GET_SIM_TYPE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN         is_ok;
    MN_DUAL_SYS_E   dual_sys;
} APP_MN_SIM_POWER_ON_CNF_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN         pin1_is_enabled;     // PIN1 enabled/disabled when power on
    MNSIM_PIN_STATUS_T     pin1_status;         // pin1 status
    MNSIM_PIN_STATUS_T     unblock_pin1_status; // blocked PIN1 status
    MN_DUAL_SYS_E   dual_sys;
} APP_MN_SIM_PIN_CHECK_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_UIM_IMSI_CAUSE_E cause;
    uint8               imsi[MN_MAX_UIM_IMSI_LEN];
    MN_DUAL_SYS_E       dual_sys;
} APP_MN_GET_UIM_IMSI_CNF_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN         is_ok;
    MN_DUAL_SYS_E   dual_sys;
} APP_MN_SIM_POWER_OFF_CNF_T;

//  struct of signal: APP_MN_DETACH_GSM_CNF
//  this signal indicate phone status to MMI 
typedef struct
{
    SIGNAL_VARS
    BOOLEAN         is_ok;
    MN_DUAL_SYS_E   dual_sys;
} APP_MN_DETACH_GSM_CNF_T;

//  struct of signal: APP_MN_AOC_INCREASE_ACM_IND
//  this signal indicate phone status to MMI 
typedef struct 
{
    SIGNAL_VARS
    BOOLEAN         is_ok;
    BOOLEAN         is_reach_max;
    uint32          acm_unit;
    uint32          acm_max;
    MN_DUAL_SYS_E   dual_sys;
} APP_MN_AOC_MODIFIED_IND_T;

//  struct of signal: APP_MN_FDN_SERVICE_IND
//  this signal indicate fdn enable/disable to MMI 
typedef struct
{
    SIGNAL_VARS
    BOOLEAN         is_fdn_enable;
    MN_DUAL_SYS_E   dual_sys;
    BOOLEAN         is_fdn_exist;
} APP_MN_FDN_SERVICE_IND_T, 
  APP_MN_FDN_CAPABILITY_IND_T;

typedef struct
{
    uint8  year;
    uint8  month;
    uint8  day;
    uint8  hour;
    uint8  minute;
    uint8  second;
    int16  time_zone;
}MN_UT_LT_ZONE_T;

typedef struct 
{
    BOOLEAN local_time_zone_flag;
    BOOLEAN ut_lt_zone_flag;
    BOOLEAN daylight_saving_time_flag;
    int8    local_time_zone;
    MN_UT_LT_ZONE_T ut_lt_zone;
    uint8   daylight_saving_time;
}MN_MM_NET_TIME_INFO_T;


typedef struct 
{   
    BOOLEAN     is_network_time_exist;
    BOOLEAN     is_summer_time;        //whether is summer time
    BOOLEAN     local_time_zone_flag;
    int8        gmt_offset;
    int8        local_gmt_offset;
    MN_UT_LT_ZONE_T ut_lt_zone;
}MN_APP_NET_TIME_INFO_T;

#define MAX_NETWORK_NAME_LEN  60

typedef struct _MN_NETWORK_NAME_T
{
    BOOLEAN   network_name_exist;
    uint8     code_scheme;
    BOOLEAN   add_ci;
    uint8     length;
    uint8     name[MAX_NETWORK_NAME_LEN];  //if Cell Broadcast data coding scheme, already Unpack 7bits To 8bits
} MN_NETWORK_NAME_T;

typedef struct _MN_LSA_ID_T
{
    BOOLEAN   lsa_exist;
    uint32    lsa_id;  //24bit, 24.008 Octet3 Octet4 Octet5
} MN_LSA_ID_T;

typedef struct 
{
    SIGNAL_VARS
    MN_PLMN_T              cur_plmn;
    MN_NETWORK_NAME_T      full_name;
    MN_NETWORK_NAME_T      short_name;
    MN_MM_NET_TIME_INFO_T  time_and_timezone;
    MN_LSA_ID_T            lsa_id;  //Identification of Localised Service Area
    MN_DUAL_SYS_E          dual_sys;
    BOOLEAN                is_from_cs;
} APP_MN_NETWORK_INFO_IND_T;

typedef struct APP_MN_NETNAME_READY_IND_T_s
{
    SIGNAL_VARS
    MN_DUAL_SYS_E   dual_sys;
} APP_MN_NETNAME_READY_IND_T;

typedef struct APP_MN_CPHS_READY_IND_T_s
{
    SIGNAL_VARS
    MN_DUAL_SYS_E   dual_sys;
    BOOLEAN is_cphs_exist;
} APP_MN_CPHS_READY_IND_T;

typedef enum _MN_ATTACH_STATE_E
{
	MN_INVALID_STATE, /* this field is invalid */
	MN_ATTACHED_STATE, /* cs or ps has been attached */
	MN_DETACHED_SATE, /* cs or ps has been detached */
	MN_NO_SERVICE  /* no service for cs or ps */
} MN_ATTACH_STATE_E;


#define MN_SPN_ID_NUM_LEN       16
#define MAX_ONSHORTFORM_LEN  60
//  struct of signal: APP_MN_NETWORK_STATUS_IND
//  this signal indicate phone status to MMI 
typedef struct 
{
    SIGNAL_VARS
    MN_PHONE_PLMN_STATUS_E  plmn_status;        // plmn statue          
    BOOLEAN                 plmn_is_selecting;  // if plmn is in selecting
    BOOLEAN                 plmn_is_roaming;    // if plmn is roaming plmn  
    uint16                  mcc;
    uint16                  mnc;
    uint16                  mnc_digit_num;
    uint16                  lac;
    uint32                  cell_id;
    BOOLEAN                 gprs_support;

    BOOLEAN                 mbms_support;		//TRUE means support
    BOOLEAN                 edge_support;		//TRUE means support
    BOOLEAN                 hsdpa_support;		//TRUE means support
    BOOLEAN                 hsupa_support;		//TRUE means support, always FALSE for Tianlong R5


    MN_DUAL_SYS_E           dual_sys;
    BOOLEAN                 is_spn_need;                //if the SPN exist, MMI will indicate if this item is true
    uint8                   spn_len;                    //length of spn content
    MNBCD                   spn[MN_SPN_ID_NUM_LEN];     //spn content
    BOOLEAN                 is_opn_need;                //if the OPN need to indicate by MMI
    BOOLEAN                 is_opn_exist;               //if the OPN exist  in this struct
    uint8                   opn_len;                    //length of opn content
    MN_OPN_DCS_E            opn_dcs;                    //dcs of the opn text string
    uint8                   opn[MAX_ONSHORTFORM_LEN];   //opn content, short name length:SIM_EF_MAX_ONSHORTFORM_LEN  10
    MN_APP_NET_TIME_INFO_T  net_time_info;
    MN_GMMREG_RAT_E         rat;
    BOOLEAN                 is_add_ci;                  //whether add the country initials
    MN_ATTACH_STATE_E       cs_attach_state;
    MN_ATTACH_STATE_E       ps_attach_state;
    MN_MM_CAUSE_E           mn_mm_cause;
    int8                    gmt_offset_quarter;         //Timezone Unit: 15min, same mean as "net_time_info.gmt_offset" except unit
    int8                    local_gmt_offset_quarter;   //Timezone Unit: 15min, same mean as "net_time_info.local_gmt_offset" except unit
} APP_MN_NETWORK_STATUS_IND_T;

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

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_DISPLAY_PLMN_IND_T;
#define MN_SIM_MAX_GID_LEN                          16
typedef struct
{
    uint8   len;             //the really length of the gid
    uint8   gid[MN_SIM_MAX_GID_LEN];
}MN_SIM_EF_MAPPING_GID_T;
typedef struct
{
    SIGNAL_VARS
    BOOLEAN is_ok;
    MN_SIM_EF_MAPPING_GID_T gid_mapping;
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_READ_GID_CNF_T;

typedef enum
{
    MN_PHONE_OPERATE_FAIL_FOR_BUSY,
    MN_PHONE_OPERATE_SUCCESS
} MN_PHONE_OPERATE_RESULT_E;



//  struct of signal: APP_MN_PLMN_LIST_CNF_T
//  this signal indicate plmns's detail to MMI
typedef struct 
{
    SIGNAL_VARS
    MN_PHONE_OPERATE_RESULT_E is_valid_result;
    MN_PHONE_PLMN_INFO_LIST_T plmn_detail_list; // PLMN detail list
    MN_DUAL_SYS_E             dual_sys;
} APP_MN_PLMN_LIST_CNF_T;

typedef struct MN_EMG_NUM_T_s
{
    uint8   num_len;
    uint8   party_num[MN_MAX_ADDR_BCD_LEN];
} MN_EMG_NUM_T;

#define MN_EMG_NUM_LIST_MAX_NUM     10

//  struct of signal: EV_MN_APP_EMG_NUM_LIST_IND_F
//  this signal indicate emergency number list to MMI
typedef struct APP_MN_EMG_NUM_LIST_IND_T_s
{
    SIGNAL_VARS
    uint16          mcc;
    uint8           list_num;
    MN_EMG_NUM_T    emg_num_list[MN_EMG_NUM_LIST_MAX_NUM];
    MN_DUAL_SYS_E   dual_sys;
} APP_MN_EMG_NUM_LIST_IND_T;

//  struct of signal: APP_MN_PLMN_SELECT_CNF_T
//  this signal indicate MMI that PLMN select complete
typedef struct 
{
    SIGNAL_VARS
    BOOLEAN                 plmn_is_selected;       // is selected?
    MN_PHONE_PLMN_STATUS_E  plmn_status;              // plmn status
    BOOLEAN                 mode_is_manual;         // is manual?
    uint16                  mcc;
    uint16                  mnc;
    uint16                  mnc_digit_num;
    uint16                  lac;
    uint32                  cell_id;
    MN_DUAL_SYS_E           dual_sys;       
    MN_GMMREG_RAT_E         rat;
    MN_MM_CAUSE_E           cause;   
} APP_MN_PLMN_SELECT_CNF_T;

//  struct of signal: APP_MN_SIM_GET_PIN_IND_T
//  this signal indicate MMI to input PIN1 value
typedef struct 
{
    SIGNAL_VARS
    MNSIM_PIN_INDICATION_E  pin_num;  // SIM_PIN_1 always
    BOOLEAN               pin_is_blocked;   // PIN1 or blocked PIN1
    uint8               remain_times;   // the value is attempt times of PIN while pin_is_blocked's value is FALSE; and the value is attempt times of PUK while pin_is_blocked's value is TRUE
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_SIM_GET_PIN_IND_T;

//  struct of signal: APP_MN_SIM_READY_IND_T
//  this signal indicate MMI that SIM card init OK
typedef struct 
{
    SIGNAL_VARS
    BOOLEAN                pin1_is_enabled;     // PIN1 enabled/disabled when power on
    BOOLEAN                pin1_is_blocked;     // is pin1 blocked?  
    MNSIM_PIN_STATUS_T     pin1_status;         // pin1 status
    MNSIM_PIN_STATUS_T     pin2_status;         // pin2 status
    MNSIM_PIN_STATUS_T     unblock_pin1_status; // blocked PIN1 status
    MNSIM_PIN_STATUS_T     unblock_pin2_Status; // blocked PIN2 status
    BOOLEAN                is_pin2_required_acm_max; // PIN2 is req'd for ACMmax update 
    BOOLEAN                is_pin2_Required_acm;     // PIN2 is req'd for Acm update 
    BOOLEAN                is_pin2_Required_puct;    // PIN2 is req'd for PUCT update 
    MN_IMSI_T              imsi;                // international mobile subscriber id 
    MN_PLMN_T              hplmn;
    MN_LOCI_T              location_info;       // Location area information 
    MNSIM_PHASE_E          phase;               // phase 1 or 2 
    MNSIM_SST_T            sim_service;         // SIM service table 
    MNSIM_ADMIN_T          admin;               
    BOOLEAN                AOC_is_support;
    MN_DUAL_SYS_E          dual_sys;
    MN_GMMREG_RAT_E        rat;
    BOOLEAN   is_usim;
} APP_MN_SIM_READY_IND_T;

//  struct of signal: APP_MN_SIM_NOT_READY_IND_T
//  this signal indicate MMI that SIM not ready
typedef struct 
{
    SIGNAL_VARS
    MNSIM_ERROR_E reason;  // reason why SIM not ready
    MN_DUAL_SYS_E dual_sys;
} APP_MN_SIM_NOT_READY_IND_T;


//this signal indicate mmi whether the sim is present
typedef struct
{
    SIGNAL_VARS
	BOOLEAN  is_sim_present;   //TRUE is present
	uint8 sms_num;
	MN_DUAL_SYS_E dual_sys;
}APP_MN_SIM_PRESENT_IND_T;

//this signal indicate mmi whether the sim is present
typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
}APP_MN_SIM_PLUG_IN_IND_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN                   is_lnd_ok;
    MN_DUAL_SYS_E         dual_sys;
} APP_MN_LND_READY_IND_T;


//  struct of signal: APP_MN_SIM_PIN_FUNC_CNF_T
//  this signal indicate MMI that operation on PIN compete
typedef struct 
{
    SIGNAL_VARS
    MNSIM_PIN_REQ_RESULT_E              pin_operate_result;
    MNSIM_PIN_REQ_TYPE_E                pin_function_type;  // Verify, change, disable, enable, or unblock.
    MNSIM_PIN_INDICATION_E              pin_num;            // pin1 or pin2
    MNSIM_PIN_STATUS_T                  pin_status;         // Number of remaining retries and initialisation status
    MNSIM_PIN_STATUS_T                  unblock_pin_status; // Number of remaining retries and initialisation status
    MN_DUAL_SYS_E                       dual_sys;
} APP_MN_SIM_PIN_FUNC_CNF_T;

//  struct of signal: APP_MN_GPRS_ATTACH_CNF_T
//  this signal indicate MMI that operation on PIN compete
//  xu.xiaogang 2003-9-27 add error cause for attach failed
typedef struct 
{
    SIGNAL_VARS
    MN_PHONE_GPRS_ATTACH_TYPE_E attach_type;
    BOOLEAN                     is_attach_ok;
    int                         err_cause;
    MN_DUAL_SYS_E               dual_sys;
} APP_MN_GPRS_ATTACH_RESULT_T;

//  struct of signal: APP_MN_GPRS_DETACH_CNF_T
//  this signal indicate MMI that operation on PIN compete
typedef struct 
{
    SIGNAL_VARS
    MN_PHONE_GPRS_DETACH_TYPE_E detach_type;
    BOOLEAN                     is_detach_ok;
    MN_DUAL_SYS_E               dual_sys;
} APP_MN_GPRS_DETACH_RESULT_T;

typedef struct 
{
    SIGNAL_VARS
    BOOLEAN                     is_detach_ok;
    MN_DUAL_SYS_E               dual_sys;
} APP_MN_GPRS_DETACH_IND_T;

//  struct of signal: APP_MN_SCELL_RSSI_IND_T
//  this signal indicate MMI service cell power level
typedef struct
{
    SIGNAL_VARS
    uint16  arfcn;
    uint8   bsic;
    BOOLEAN is_traffic_mode;
    // idle info
    uint8   rxlev;
    // traffic info
    BOOLEAN is_dtx_used;
    uint16  rxqual_full;
    uint16  rxqual_sub;
    uint16  rxlev_full;
    uint16  rxlev_sub;
    uint16  cellid;
    uint16  lac;
    MN_DUAL_SYS_E dual_sys;
    BOOLEAN is_paging_trigger;
} APP_MN_SCELL_RSSI_IND_T;

//  struct of signal: APP_MN_NCELL_RSSI_CNF_T
//  this signal indicate MMI neighbour cell power level
typedef struct
{
    SIGNAL_VARS
    MN_PHONE_CELL_MEAS_T            ncell_power[MN_MAX_NCELL_NUM]; // Neighbour cells measurement report 
    MN_DUAL_SYS_E                   dual_sys;
} APP_MN_NCELL_RSSI_CNF_T;

//  struct of signal: APP_MN_PUCT_INFO_CNF_T
//  this signal indicate puct info
typedef struct
{
    SIGNAL_VARS
    BOOLEAN            is_ok;
    BOOLEAN            is_service_support;
    MNSIM_PUCT_INFO_T  puct_info;
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_PUCT_INFO_CNF_T;


// used for SIM-phone

//  struct of signal: APP_MN_MSISDN_UPDATE_CNF_T
//  this signal is cnf msisdn update or delete or add
typedef struct
{
    SIGNAL_VARS
    MN_MSISDN_OPERATE_TYPE_E    operate_type;
    BOOLEAN                     is_ok;
    uint16                      entry_id;

    MN_DUAL_SYS_E               dual_sys;
    uint8                       dn_id;
} APP_MN_MSISDN_UPDATE_CNF_T;


//  struct of signal: APP_MN_ADN_UPDATE_CNF_T
//  this signal is cnf adn update or delete or add
typedef struct
{
    SIGNAL_VARS
    MN_ADN_OPERATE_TYPE_E       operate_type;
    BOOLEAN                     is_ok;
    uint16                      entry_id;

    MN_DUAL_SYS_E               dual_sys;
    uint8                       dn_id;
} APP_MN_ADN_UPDATE_CNF_T;


//  struct of signal: APP_MN_FDN_UPDATE_CNF_T
//  this signal is cnf fdn update or delete or add
typedef struct
{
    SIGNAL_VARS
    MN_FDN_OPERATE_TYPE_E       operate_type;
    BOOLEAN                     is_ok;
    uint16                      entry_id;

    MN_DUAL_SYS_E               dual_sys;
    uint8                       dn_id;
} APP_MN_FDN_UPDATE_CNF_T;

//  struct of signal: APP_MN_LND_UPDATE_CNF_T
//  this signal is cnf LND update or delete or add
typedef struct APP_MN_LND_UPDATE_CNF_T_s
{
    SIGNAL_VARS
    MN_LND_OPERATE_TYPE_E       operate_type;
    BOOLEAN                     is_ok;
    uint16                      entry_id;

    MN_DUAL_SYS_E               dual_sys;
} APP_MN_LND_UPDATE_CNF_T;

//  struct of signal: APP_MN_GET_FREE_EXT_INFO_CNF_T
//  this signal is cnf get free ext info from SIM
typedef struct
{
    SIGNAL_VARS
    BOOLEAN                 is_ok;
    PHONEBOOK_STORAGE_E     storage;
    uint8                   free_ext_record_num;
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_GET_FREE_EXT_INFO_CNF_T;

typedef struct
{
    SIGNAL_VARS
    mn_sim_restrict_access_struct   rsp_data;
    MN_DUAL_SYS_E                   dual_sys;
} APP_MN_CRSM_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_CPHS_INFO_T cphs_info;
    MN_DUAL_SYS_E  dual_sys;
} APP_MN_CPHS_INFO_IND_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN is_op_name_vaild;
    MN_OPER_TYPE_E type;
    uint8   length;
    uint8   buf[MN_MAX_OPER_NAME_LEN];
    MN_DUAL_SYS_E  dual_sys;
} APP_MN_OPER_NAME_IND_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN is_cff_valid;
    BOOLEAN cff_line_1;
    BOOLEAN cff_line_2;
    MN_DUAL_SYS_E  dual_sys;
} APP_MN_CFF_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_RESULT_UPDATE_FILE_E result;
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_DCS_UPDATE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_RESULT_UPDATE_FILE_E result;
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_CSP_UPDATE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_RESULT_UPDATE_FILE_E result;
    MN_DUAL_SYS_E           dual_sys;
}APP_MN_LINE_SELECTED_UPDATE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_RESULT_UPDATE_FILE_E result;
    MN_DUAL_SYS_E           dual_sys;
}APP_MN_LINE_LOCKSTATUS_UPDATE_CNF_T;

typedef enum
{
    MN_SIM_EF_S_TRANSPARENT = 0x00,
    MN_SIM_EF_S_LINEAR_FIXED    = 0x01,
    NN_SIM_EF_S_CYCLIC          = 0x03,
    MN_SIM_EF_S_UNKNOW
} MN_SIM_EF_STRUCTURE_E;

typedef enum
{
    MN_SIM_FILE_READ_OK,                 //read file succeeded
    MN_SIM_FILE_READ_INVALID_FILE,       //the file is invalid in the SIM card
    MN_SIM_FILE_READ_INCONSISTENT_FILE,  //the file type or structure is not consistent to the request
    MN_SIM_FILE_READ_NOT_EXIST,          //the file not exist in the SIM card
    MN_SIM_FILE_READ_CHV1_NEEDED,        //the read operation need the right CHV1
    MN_SIM_FILE_READ_CHV2_NEEDED,        //the read operation need the right CHV2
    MN_SIM_FILE_READ_U_CHV1_NEEDED,      //the read operation need the right UNBLOCK CHV1
    MN_SIM_FILE_READ_U_CHV2_NEEDED,      //the read operation need the right UNBLOCK CHV2
    MN_SIM_FILE_READ_CHV_BLOCKED,        //both the read operation relative chv and unblock chv blocked
    MN_SIM_FILE_READ_ACCESS_NEVER,       //the read operation access will never be fulfilled
    MN_SIM_FILE_READ_FAILED_UNKNOW       //the read file is failed for unknow reason
} MN_SIM_FILE_READ_RESULT_E;

#define MN_MAX_DEEP_SIM_FILE_LEVEL     4
#define MN_MAX_SIM_FILE_STRUCTURE_LEN  256

typedef struct
{
    uint8   record_num;                           //the record num of the record file,it is no meaning for transparent file                     
    uint16 file_id;                               //file id of the special file,it is no meaning for GSM normal file
    MN_SIM_EF_STRUCTURE_E file_struct;            //TRANSPARENT,LINEAR_FIXED or CYCLIC,it is for the special file,it is no meaning for GSM normal file
    uint8  file_level;                            //the file level of the file,such as EFimsi,the file path is 3F00/7F20/6F07,and the file level is 3 
    uint16 file_path[MN_MAX_DEEP_SIM_FILE_LEVEL]; //the path to access the special file,the top file must be MF(3F 00),such as
                                                  //the EFimsi,the file path is 3F00/7F20/6F07
} MN_SIM_FILE_PROFILE_T;

typedef struct
{
    SIGNAL_VARS
    uint16  file_id;                            //file id of the file
    MN_SIM_EF_STRUCTURE_E file_struct;          //TRANSPARENT,LINEAR_FIXED or CYCLIC
    uint8   record_num;                         //the record num of the record file,it is only used for LINEAR_FIXED or CYCLIC file                     
    MN_SIM_FILE_READ_RESULT_E result;           //the Read file result
    uint16  data_len;                           //the data len readed
    uint8 buf[MN_MAX_SIM_FILE_STRUCTURE_LEN];   //the Read file result buf              
    MN_DUAL_SYS_E dual_sys;
} APP_MN_READ_SIM_FILE_BUF_CNF_T;

typedef struct APP_MN_SIM_RECORD_NUM_CNF_T_s
{
    SIGNAL_VARS
    BOOLEAN                         is_ok;
    uint8                           max_num;
    MNSIM_SERVICE_TYPE_PH2_E        mn_file_name;
    MN_DUAL_SYS_E                   dual_sys;
} APP_MN_SIM_RECORD_NUM_CNF_T;

typedef struct MN_SIM_OPL_ENTRY_T_s
{
    MN_PLMN_T   plmn;
    uint16      lac_1;
    uint16      lac_2;
    uint8       pnn_index;
} MN_SIM_OPL_ENTRY_T;

#define MN_SIM_ICON_MAX_LEN     250
typedef union MN_SIM_ICON_U_s
{
    uint8      uri_arr[MN_SIM_ICON_MAX_LEN];
    uint8   img_id_arr[MN_SIM_ICON_MAX_LEN];
} MN_SIM_ICON_U;

typedef struct MN_SIM_ICON_LINK_s
{
    uint8           len;
    MN_SIM_ICON_U   icon;
} MN_SIM_ICON_LINK_T;

#define MN_SIM_ICON_URI 0X80
#define MN_SIM_ICON_IMG 0X81

typedef struct MN_SIM_ICON_INFO_T_s
{
    uint8               icon_tag;   //0x80 means URI; 0x81 means img
    uint8               qualifier;
    MN_SIM_ICON_LINK_T  icon_link;
} MN_SIM_ICON_INFO_T;

#define MN_SIM_MAX_VALIDATE_CBMI_NUM 30
#define MN_SIM_MAX_CBMIR_RANGE_NUM 8

typedef uint16 MN_CB_MSG_ID_T ;

typedef MN_CB_MSG_ID_T MN_SIM_CBMI_ARR[MN_SIM_MAX_VALIDATE_CBMI_NUM];

typedef struct MN_SIM_CBMI_INFO_T_s
{
	uint8 validate_num;
	MN_SIM_CBMI_ARR cbmis;
}MN_SIM_CBMI_INFO_T;

typedef struct
{
	MN_CB_MSG_ID_T	low_cbmi;
	MN_CB_MSG_ID_T	up_cbmi;
}MN_SIM_CBMI_RANGE_T;

typedef MN_SIM_CBMI_RANGE_T  MN_SIM_CBMI_RANGE_ARR[MN_SIM_MAX_CBMIR_RANGE_NUM];
typedef struct
{
	uint8 validate_num;
	MN_SIM_CBMI_RANGE_ARR	cbmi_ranges;
}MN_SIM_CBMIR_INFO_T;

typedef struct MN_APP_NET_NAME_INFO_T_s
{   
    BOOLEAN             is_full_name_flag;
    BOOLEAN             is_short_name_flag;
    MN_NETWORK_NAME_T   full_nt_name;
    MN_NETWORK_NAME_T   short_nt_name;
} MN_APP_NET_NAME_INFO_T;

#define MNSIM_MAX_ALPHA_IDENTIFIER_LEN 60

typedef struct MN_SIM_INFONUM_T_s
{
    uint8               alpha_id_len;	
    uint8               number_len;
    uint8               information_digits_number_len;
    uint8               entry_status;
    BOOLEAN             is_ton_npi_exist;   
    BOOLEAN             is_ext_exist;   //the Extension exist or not
    uint8               ext_id;         //the Extension1 record identifier                      
    uint8               alpha_id[MNSIM_MAX_ALPHA_IDENTIFIER_LEN];		
    uint8               information_digits_number[MNSIM_MAX_ALPHA_IDENTIFIER_LEN];	  
    MN_NUMBER_TYPE_E    ton;   
    MN_NUMBER_PLAN_E    npi;  
} MN_SIM_INFONUM_T;

typedef union MN_SIM_RECORD_FILE_BUF_U_s
{
    MN_APP_NET_NAME_INFO_T  pnn_entry;
    MN_SIM_OPL_ENTRY_T      opl_entry;
    MN_SIM_ICON_INFO_T      pnni_entry;
    MN_SIM_INFONUM_T        infonum_entry;
} MN_SIM_RECORD_FILE_BUF_U;

typedef struct APP_MN_SIM_RECORD_READ_CNF_T_s
{
    SIGNAL_VARS
    BOOLEAN                     is_ok;
    uint8                       record_id;  // started from 1
    MN_SIM_RECORD_FILE_BUF_U    buf;
    MNSIM_SERVICE_TYPE_PH2_E    mn_file_name;
    MN_DUAL_SYS_E               dual_sys;
} APP_MN_SIM_RECORD_READ_CNF_T;

typedef union MN_SIM_BINARY_FILE_BUF_U_s
{
    MN_SIM_ICON_INFO_T  spni;
    MN_SIM_CBMI_INFO_T  cbmi;
    MN_SIM_CBMIR_INFO_T cbmir;
} MN_SIM_BINARY_FILE_BUF_U;

typedef struct MN_SIM_BINARY_FILE_BUF_T_s
{
    MNSIM_SERVICE_TYPE_PH2_E mn_file_name;
    MN_SIM_BINARY_FILE_BUF_U buf;
}MN_SIM_BINARY_FILE_T;

typedef struct APP_MN_SIM_BINARY_READ_CNF_T_s
{
    SIGNAL_VARS
    BOOLEAN                     is_ok;
    MN_SIM_BINARY_FILE_BUF_U    buf;
    MNSIM_SERVICE_TYPE_PH2_E    mn_file_name;
    MN_DUAL_SYS_E               dual_sys;
} APP_MN_SIM_BINARY_READ_CNF_T,
  APP_MN_SIM_BINARY_UPDATE_CNF_T;

 /*****************************************************************************
 **                        CALL MODULE DEFINE                                *
 ******************************************************************************/

#define MN_CALL_MAX_CALL_NUM        7          // 
#define MN_CALL_MAX_UTU_LEN         131
#define MN_CALL_MAX_MINI_UTU_LEN    35
#define MN_CALL_MAX_LINE_NUM        2
// DTMF key
typedef int8    MN_CALL_DTMF_KEY_T;

typedef enum _MN_CALL_DISCONNECT_CALL_CAUSE_E
{
    MN_CAUSE_ERROR_NONE               = 0,  
    MN_CAUSE_UNASSIGNED_NO            = 1,
    MN_CAUSE_NO_ROUTE_TO_DEST         = 3,
    MN_CAUSE_CHAN_UNACCEPTABLE        = 6,
    MN_CAUSE_OPER_DETERM_BARRING      = 8,
    MN_CAUSE_NORMAL_CLEARING          = 16,
    MN_CAUSE_USER_BUSY                = 17,
    MN_CAUSE_NO_USER_RESPONDING       = 18,
    MN_CAUSE_ALERTING_NO_ANSWER       = 19,
    MN_CAUSE_CALL_REJECTED            = 21,
    MN_CAUSE_NUMBER_CHANGED           = 22,
    MN_CAUSE_PREEMPTION               = 25,
    MN_CAUSE_NONSEL_USER_CLRING       = 26,
    MN_CAUSE_DEST_OUT_OF_ORDER        = 27,
    MN_CAUSE_INVALID_NO_FORMAT        = 28,
    MN_CAUSE_FACILITY_REJECTED        = 29,
    MN_CAUSE_RSP_TO_STATUS_ENQ        = 30,
    MN_CAUSE_NORMAL_UNSPECIFIED       = 31,
    MN_CAUSE_NO_CIRC_CHAN_AV          = 34,
    MN_CAUSE_NET_OUT_OF_ORDER         = 38,
    MN_CAUSE_TEMP_FAILURE             = 41,
    MN_CAUSE_SWITCH_CONGESTION        = 42,
    MN_CAUSE_ACC_INFO_DISCARDED       = 43,
    MN_CAUSE_REQ_CIRC_CHAN_UNAV       = 44,
    MN_CAUSE_RESOURCES_UNAV           = 47,
    MN_CAUSE_QOS_UNAV                 = 49,
    MN_CAUSE_REQ_FAC_NOT_SUBSCR       = 50,
    MN_CAUSE_CUG_INCOMING_BARRED      = 55,
    MN_CAUSE_BEAR_CAP_NOT_AUTH        = 57,
    MN_CAUSE_BEAR_CAP_UNAV            = 58,
    MN_CAUSE_SERV_OPT_UNAV            = 63,
    MN_CAUSE_BEAR_SVC_NOT_IMPL        = 65,
    MN_CAUSE_ACM_EQ_OR_GT_ACMMAX      = 68,
    MN_CAUSE_REQ_FACIL_NOT_IMPL       = 69,
    MN_CAUSE_ONLY_RESTRIC_DIG_AV      = 70,
    MN_CAUSE_SVC_OPT_NOT_IMPL         = 79,
    MN_CAUSE_INVALID_TI               = 81,
    MN_CAUSE_USER_NOT_IN_CUG          = 87,
    MN_CAUSE_INCOMPAT_DEST            = 88,
    MN_CAUSE_INVALID_TRANSIT_NET      = 91,
    MN_CAUSE_INVALID_MSG_SEMANTIC     = 95,
    MN_CAUSE_MAND_IE_ERROR            = 96,
    MN_CAUSE_MSG_NONEXISTENT          = 97,
    MN_CAUSE_MSG_GEN_ERROR            = 98,
    MN_CAUSE_IE_NONEXISTENT           = 99,
    MN_CAUSE_INVALID_CONDITION_IE     = 100,
    MN_CAUSE_MSG_INCOMPAT_STATE       = 101,
    MN_CAUSE_RECOV_ON_TIMER_EXP       = 102,
    MN_CAUSE_PROTOCOL_ERROR           = 111,
    MN_CAUSE_INTERWORKING             = 127, 
    MN_CAUSE_CC_TIMER_EXPIRY          = 128, 
    MN_CAUSE_AUTHENTICATION_REJ       = 150,
    MN_CAUSE_EMERGENCY_CALL_ONLY      = 151,
    MN_CAUSE_IMSI_DETACH              = 152,
    MN_CAUSE_T3230_EXPIRY             = 153,
    MN_CAUSE_RR_CONNECTIN_ERROR       = 154,
    MN_CAUSE_CALL_HAS_DISCONNECTED    = 155,
    MN_CAUSE_REMOTE_USER_DISCONNECT   = 301,
    MN_CAUSE_LOW_FAILURE,
    MN_CAUSE_NETWORK_REJECT,
    MN_CAUSE_NO_CELL,
    MN_CAUSE_SUPPLEMENT_NOT_PROVIDE,
    MN_CAUSE_SIM_STK_NOT_ALLOWED,
    MN_CAUSE_ILLEGAL_CC_OPERATION,
    MN_CAUSE_ILLEGAL_CC_STATE
} MN_CALL_DISCONNECT_CALL_CAUSE_E, MN_CC_CAUSE_E;

typedef enum _MN_CALL_CHANNEL_MODE_E
{
    MN_CALL_CHANNEL_RATE_VER1,
    MN_CALL_CHANNEL_RATE_VER2,
    MN_CALL_CHANNEL_RATE_VER3,
    MN_CALL_CHANNEL_UNKNOWN
} MN_CALL_CHANNEL_MODE_E;

typedef enum _MN_CALL_TYPE_E
{
    MN_CALL_TYPE_NORMAL     = 0,     // gsm speech normal call
    MN_CALL_TYPE_EMERGENCY,           // emergency call
    MN_CALL_TYPE_DATA,
    MN_CALL_TYPE_DATA_PREFER,
    MN_CALL_TYPE_SPEECH_PREFER,
    MN_CALL_TYPE_FAKE_EMERGENCY,
    MN_CALL_TYPE_NONE                   //default value
} MN_CALL_TYPE_E;

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

// the call entity's state
typedef enum _MN_CALL_ENTITY_STATE_E
{   
    MN_CALL_IDLE_STATE      = 0,       // idle state there is no MT or MO call
    MN_CALL_OUTGOING_STATE,            //the user has originated a MO call
    MN_CALL_ACTIVE_STATE,              //the call entity has finish the setup procedure
    MN_CALL_INCOMING_STATE,            // incoming a MT call
    MN_CALL_ACCEPT_STATE,              // the user answer the incoming call
    MN_CALL_MODIFY_PENDING_STATE,      //the user try to modify the call's service type(e.g fax call to speed call)
    MN_CALL_RELEASE_STATE,             // the local user or remote user try to release the call entity
    MN_CALL_CCBS_RECALL_STATE,         // waiting the response from user for ccbs recall
#ifdef __MN_CALL_ADP_CFW__
    MN_CALL_WAITING_STATE,             //  incoming a MT call for call entity > 1
#endif
    MN_CALL_MAX_STATE                  //count the call entity's state
} MN_CALL_ENTITY_STATE_E; 

// the service type 
typedef enum _MN_SERVICE_TYPE_E
{
    MN_GSM_VOICE_SERVICE,        // speech call
    MN_GSM_DATA_SERVICE,         // data call
    MN_GSM_FAX_SERVICE,          // fax call
    MN_GSM_DUAL_SERVICE          // alternate service(e.g speech to fax)
} MN_SERVICE_TYPE_E;

typedef enum _MN_CALL_CODE_STD_E
{
    MN_CALL_CODE_ITU_STD,
    MN_CALL_CODE_INTERNATIONAL_STD,
    MN_CALL_CODE_NATION_STD,
    MN_CALL_CODE_GSM_STD
} MN_CALL_CODE_STD_E;

typedef enum _MN_CALL_LOCATION_EST_TYPE_E
{
    MN_CALL_LOCATION_USER,
    MN_CALL_PRIV_SER_LOC,
    MN_CALL_PUB_SER_LOC,
    MN_CALL_PUB_SER_REMOTE,
    MN_CALL_PRIV_SER_REMOTE,
    MN_CALL_NET_BYN_INTER
} MN_CALL_LOCATION_EST_TYPE_E;

typedef enum _MN_CALL_PROG_DES_E
{
    MN_CALL_PROG_NOT_END_TO_END  = 1,
    MN_CALL_PROG_DEST_NO_PLMN,
    MN_CALL_PROG_ORIG_NO_PLMN,
    MN_CALL_PROG_RTN_PLMN,
    MN_CALL_PROG_IN_BAND_INFO    = 8,
    MN_CALL_PROG_END_TO_END      = 32,
    MN_CALL_PROG_QUEUEING        = 64
} MN_CALL_PROG_DES_E;

// alerting type
typedef enum _MN_CALL_ALERT_TYPE_E
{
    MN_CALL_REMOTE_USER_ALERTING,  // the remote call has alerted
    MN_CALL_LOCAL_USER_ALERTING,   // the local user alert
    MN_CALL_IN_BAND_AVAILABLE      // the network has inband tone alerting
} MN_CALL_ALERT_TYPE_E;

typedef enum _MN_CALL_SYNC_CAUSE_E
{
    MN_CALL_SYNC_CIPHERING,
    MN_CALL_SYNC_RESOURCE_ASSIGNMENT,
    MN_CALL_SYNC_CHANNEL_MODE_MODIFICATION
} MN_CALL_SYNC_CAUSE_E;

typedef enum _MN_CALL_UTU_PD_E
{
    MN_CALL_UTU_USER_SPEC           = 0,    /*0,user specific protocol,The user information 
                                            is structure according to user needs.*/
    MN_CALL_UTU_OSI_HIGH_L,                 /*1,OSI high layer protocols*/
    MN_CALL_UTU_X244,                       /*2,the user information is structured according to 
                                            Rec.X.244 which specifies the structure of X.25 call user data*/
    MN_CALL_UTU_IA5_CHAR            = 4,    /*4,The user information consist of IA5 characters*/
    MN_CALL_UTU_CALL_CONTROL_MSG    = 8     /*8,Q.931 user_network call control messages*/
} MN_CALL_UTU_PD_E;

typedef enum _MN_CALL_CLIR_E
{
    MN_CALL_CLIR_SUPPRESSION,
    MN_CALL_CLIR_INVOCATION,
    MN_CALL_CLIR_NETWORK_DECIDE
} MN_CALL_CLIR_E;

typedef enum _MN_BEARER_CAPABILITY_TYPE_E
{
    MN_BEARER_CAPABILITY_LINE_1     = 0,
    MN_BEARER_CAPABILITY_LINE_2     = 1,
    MN_BEARER_CAPABILITY_LINE_MAX
} MN_BEARER_CAPABILITY_TYPE_E;

// user to user define
typedef struct _MN_CALL_UTU_T
{
    MN_CALL_UTU_PD_E    user_protocol;
    uint16              info_len;
    uint8               info[MN_CALL_MAX_UTU_LEN];
} MN_CALL_UTU_T;

typedef struct _MN_CALL_MINI_UTU_T
{
    MN_CALL_UTU_PD_E    user_protocol;
    uint16              info_len;
    uint8               info[MN_CALL_MAX_MINI_UTU_LEN];
} MN_CALL_MINI_UTU_T;

typedef struct
{
    BOOLEAN                   is_call_exist;
    BOOLEAN                   direction_flag;           // FALSE: MO call; TRUE: MT call
    MN_CALLED_NUMBER_T        mo_call_number;           // MO call number
    MN_CALLING_NUMBER_T       mt_call_number;           // MT call number
    BOOLEAN                   is_mpty_member;           // determine if the current call is the member of the MPTY call 
    MN_CALL_CLIR_E            clir_type;                // indicate the CLIR(Calling Line Identification Restriction) type
} MN_CALL_CONTEXT_T;

//********* Define the structure of signals sent to MMI task **************  


// indicate the service status of call forwarding to MMI 
typedef struct
{
    SIGNAL_VARS  
    MN_SS_CODE_E                   forward_operation;  // the type of call forwarding 
    MN_SS_STATUS_E                 forward_status;     // the status of the call forwarding service
    MN_DUAL_SYS_E                  dual_sys;
	uint8                          call_id;
} APP_MN_FORWARD_STATUS_IND_T;

// indicate to MMI that the MO call is ongoing 
typedef struct
{
    SIGNAL_VARS
    uint8                       call_id;
    MN_CALLED_NUMBER_T          called_num;     // the called address
    MN_DUAL_SYS_E               dual_sys;
    MN_CALL_TYPE_E              call_type;
} APP_MN_CALL_START_IND_T;

// indicate to local user the remote user has been alerting
typedef struct
{
    SIGNAL_VARS
    uint8                    call_id;  
    MN_CALL_ALERT_TYPE_E     alerting_type;  // indicate the alerting type i.e(inband tone or remote alert)
    MN_DUAL_SYS_E            dual_sys;
} APP_MN_ALERTING_IND_T;

// indicate the call has been setup completely
typedef struct
{
    SIGNAL_VARS
    uint8                   call_id;
    BOOLEAN                 connected_num_present;
    MN_CALLING_NUMBER_T     connected_num;              // the remote user's address
    BOOLEAN                 connected_sub_addr_present;
    MN_SUBADDR_T            connected_sub_addr;         // the remote user's subadress
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_SETUP_COMPLETE_CNF_T;

typedef struct
{
     SIGNAL_VARS 
     uint8                              call_id;
     MN_CALL_DISCONNECT_CALL_CAUSE_E    cause;
     MN_DUAL_SYS_E                      dual_sys;
     BOOLEAN                            is_ccbs_allowed;	 
} APP_MN_DISC_INFO_IND_T;

typedef struct
{
    SIGNAL_VARS
    uint8                   call_id;
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_SETUP_COMPLETE_IND_T;

// indicate the current call entity has been disconnected by network
typedef struct
{
    SIGNAL_VARS
    uint8                            call_id;
    MN_CALL_DISCONNECT_CALL_CAUSE_E  disconnected_cause;  
    MN_DUAL_SYS_E                    dual_sys;
}APP_MN_CALL_DISCONNECTED_IND_T;

// indicate that there is error in the current call entity
typedef struct
{
    SIGNAL_VARS
    uint8                            call_id;
    MN_CALL_DISCONNECT_CALL_CAUSE_E  error_cause;
    MN_DUAL_SYS_E                    dual_sys;
} APP_MN_CALL_ERR_IND_T;

// indicate there is a incoming call
typedef struct
{
    SIGNAL_VARS
    uint8                    call_id;
    BOOLEAN                  calling_num_present;       // indicate the calling_num is present or not
    MN_CALLING_NUMBER_T      calling_num;               // the remote user's address
    BOOLEAN                  calling_sub_addr_present;
    MN_SUBADDR_T             calling_sub_addr;          // the remote user's subaddress
    MN_DUAL_SYS_E            dual_sys;
	MN_CALL_TYPE_E			 call_type;
	MN_BEARER_CAPABILITY_TYPE_E     bc_type;      // the als line num, default is line 1
} APP_MN_SETUP_IND_T;

// the network accept or reject the DTMF tone
typedef struct
{
    SIGNAL_VARS
    uint8                 call_id;
    MN_CALL_DTMF_KEY_T    dtmf_key;         // the dtmf key's value
    BOOLEAN               req_is_accepted;  // indicate the request of START DTMF is accepted or rejected by network
    MN_DUAL_SYS_E         dual_sys;
    MN_SS_ERROR_CODE_E mn_ss_cause;
} APP_MN_START_DTMF_CNF_T;


typedef struct
{
	SIGNAL_VARS
    uint8            	call_id; 				// the call entity's ID
    MN_DUAL_SYS_E         dual_sys;
    BOOLEAN         req_is_accepted;		// indicate the request of stop DTMF is accepted or rejected
    MN_SS_ERROR_CODE_E mn_ss_cause;
} APP_MN_STOP_DTMF_CNF_T;


typedef struct
{
    SIGNAL_VARS
    uint8            call_id;   // the call entity's ID
    MN_DUAL_SYS_E    dual_sys;
} APP_MN_STOP_DTMF_REQ_T,       // local user stop transfer the DTMF tone
 //   APP_MN_STOP_DTMF_CNF_T,       // network has accept or reject the stopping DTMF
  APP_MN_CALL_HOLD_REQ_T,       // user hold the current active call or active MPTY
  APP_MN_CALL_HELD_IND_T,       // indicate to MMI task that the remote user has held the call entity
  APP_MN_CALL_RETRIEVED_IND_T,  // indicate to MMI task that the remote user has retrieved the call entity
  APP_MN_BUILD_MPTY_IND_T,      // indicate that the remote user has added the call to MPTY
  APP_MN_CALL_RETRIEVE_REQ_T,   // user retrieved the held call entity
  APP_MN_SPLIT_MPTY_REQ_T,      // user split the call from the MPTY 
  APP_MN_CALL_WAIT_IND_T,       // indicate call has been waited by the remote user 
  APP_MN_CLIR_SUPP_REJ_IND_T,     // indicate call clir suppression has been rejected by the network 
  APP_MN_CALL_DEFLECT_IND_T,     // indicate call has been deflected by the remote user 
  APP_MN_CALL_HOLDING_IND_T,    // inform clients the call is being holded
  APP_MN_CALL_RETRIEVING_IND_T, // inform clients that the call is being retrieved
  APP_MN_BUILDING_MPTY_IND_T,
  APP_MN_SPLITTING_MPTY_IND_T,
  APP_MN_ECT_INVOKING_IND_T;
  
typedef enum
{
    MN_ECT_ALLERTING,   //the ect call is in alerting state
    MN_ECT_ACTIVE       // the ect call is in active state
    
} MN_ECT_CALLSTATE_E;

typedef struct
{
    SIGNAL_VARS
    uint8               call_id;
    MN_ECT_CALLSTATE_E  ect_call_state;     //remote call state,alerting or active
    BOOLEAN             party_num_present;
    MN_CALLED_NUMBER_T  party_num;          //remote called num
    BOOLEAN             sub_addr_present;
    MN_SUBADDR_T        sub_addr;   
    MN_DUAL_SYS_E       dual_sys;
} APP_MN_ECT_INVOKE_IND_T;

typedef enum
{
    MN_SS_NAME_PRESENT_ALLOWED,
    MN_SS_PRESENT_RESTRICTED,
    MN_SS_NAME_UNAVAILABLE,
    MN_SS_NAME_PRESENT_RESTRICTED   // calling name identity and presentation restricted (for the case where the CNAP override category is provisioned).
                                    // that is to say: if calling_name = MN_SS_NAME_PRESENT_RESTRICTED, MMI should display nameset.
} MN_SS_CALLING_NAME_E;

typedef struct
{
    MN_SS_DCS_E   data_code_scheme;   // USSD-DataCodingScheme, 0x0F:default alphabet
    uint8         name_str[MN_SS_MAX_CALLING_NAME_LENGTH];   // default alphabet: character table in 03.38 6.2.1
    uint16        name_str_len;         
}MN_SS_NAMESET_T;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E         dual_sys;
    uint8                 call_id;
    MN_SS_CALLING_NAME_E  calling_name;
    MN_SS_NAMESET_T       name_set; // if calling_name=MN_SS_NAME_PRESENT_ALLOWED or MN_SS_NAME_PRESENT_RESTRICTED
                                    // name_set has valid value.
} APP_MN_CALLING_NAME_IND_T;

typedef struct _MN_CALL_PROGRESS_T
{
    MN_CALL_CODE_STD_E              code_std;
    MN_CALL_LOCATION_EST_TYPE_E     local;
    MN_CALL_PROG_DES_E              descrip;
} MN_CALL_PROGRESS_T;

// inform the MMI that the network has assigned TCH  
typedef struct
{
    SIGNAL_VARS
    MN_CALL_CHANNEL_MODE_E      channel_mode;  // the current channel mode
    MN_CALL_SYNC_CAUSE_E        sync_cause;    // indicate the cause of assigning resource
    MN_DUAL_SYS_E               dual_sys;
    uint8                       call_id;    /*CR223441 E removed macro*/	
} APP_MN_SYNC_IND_T;

// inform the network has rejected or accepted the HOLD request
typedef struct
{
    SIGNAL_VARS
    uint8           call_id;
    BOOLEAN         req_is_accepted; // Indicate the request of holding call is accepted or rejected
    MN_DUAL_SYS_E   dual_sys;
    MN_CC_CAUSE_E  mn_cc_cause;
} APP_MN_CALL_HELD_CNF_T;

// inform that the network has accepted or reject the RETRIEVE request 
typedef struct
{
    SIGNAL_VARS
    uint8                 call_id;
    BOOLEAN               req_is_accepted; // indicate the request of retrieving call is accepted or rejected
    MN_DUAL_SYS_E         dual_sys;
    MN_CC_CAUSE_E  mn_cc_cause;
} APP_MN_CALL_RETRIEVED_CNF_T;

// inform that the remote user has alter the service type
typedef struct
{
    SIGNAL_VARS
    uint8                    call_id;
    MN_SERVICE_TYPE_E        service_type;          // The service type the user want to change to be
    BOOLEAN                  direction_is_reverse;  // This Parameter indicate that the user want to reverse the the call setup directive
    MN_DUAL_SYS_E            dual_sys;
} APP_MN_CALL_MODIFY_IND_T; 

// inform the network accepted of reject the modification request
typedef struct
{
    SIGNAL_VARS
    uint8                  call_id;
    BOOLEAN                req_is_accepted;       // determine if the network has accepted the MODIFY request or not
    BOOLEAN                direction_is_reverse;  // TRUE: reverse the call's direction otherwise FALSE
    MN_DUAL_SYS_E          dual_sys;
    MN_CC_CAUSE_E  mn_cc_cause;
} APP_MN_CALL_MODIFY_CNF_T;

// inform the MMI task that the incoming call or the outgoing call is forwarded
typedef struct
{
    SIGNAL_VARS
    uint8                       call_id;
    MN_SS_FORWARD_NOTIFY_E      forward_type;     // indicate the outgoing call or the incoming call is forwarded
    MN_DUAL_SYS_E               dual_sys;
} APP_MN_CALL_FORWARD_IND_T;

// indicate to MMI task that the outgoing call is barred
typedef struct
{
    SIGNAL_VARS
    uint8              call_id;
    MN_SS_CODE_E       barred_operation;  // indicate local user's outgoing call is barred or remote
                                          // user's incoming call is barried
    MN_DUAL_SYS_E      dual_sys;
} APP_MN_CALL_BARRED_IND_T;

// this signal inform MMI the result of BUILDING MPTY request
typedef struct
{
    SIGNAL_VARS
    uint8                 call_id;
    BOOLEAN               req_is_accepted; // indicate the network accept or
                                           // reject the request of building MPTY
    MN_DUAL_SYS_E         dual_sys;
    MN_SS_ERROR_CODE_E      mn_ss_cause;
} APP_MN_BUILD_MPTY_CNF_T;

// this signal inform the MMI task the result of SPLITTING MPTY
typedef struct
{
    SIGNAL_VARS
    uint8                 call_id;
    BOOLEAN               req_is_accepted; // indicate the request of splitting MPTY is accepted
                                           // or rejected by network
    MN_DUAL_SYS_E         dual_sys;
    MN_SS_ERROR_CODE_E      mn_ss_cause;
} APP_MN_SPLIT_MPTY_CNF_T;

// this signal inform MMI the result of invoke ECT request
typedef struct
{
    SIGNAL_VARS
    uint8                 call_id;
    BOOLEAN               req_is_accepted; // indicate the network accept or
                                           // reject the request of ECT invoke
    MN_DUAL_SYS_E         dual_sys;
} APP_MN_ECT_INVOKE_CNF_T;

// this signal is to indicate that the value of ACM is equal or great to the max value
typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E         dual_sys;
} APP_MN_ACMMAX_IND_T;

//Indicate which message the reported progress indication ie is in
typedef enum
{
    MN_PROGRESS_ALERTING,
    MN_PROGRESS_CALL_PROCEEDING,
    MN_PROGRESS_PROGRESS,
    MN_PROGRESS_CONNECT,
    MN_PROGRESS_DISCONNECT
}MN_PROGRESS_MESSAGE;

// inform the progress of current call in the interwork
typedef struct
{
    SIGNAL_VARS
    uint8                     call_id;
    MN_CALL_PROGRESS_T        progress_ind;  // indicate the progress of the call in the network
    MN_DUAL_SYS_E             dual_sys;
    MN_PROGRESS_MESSAGE       from_message;
} APP_MN_PROGRESS_IND_T;

// transfer the user-to-user information to local user
typedef struct
{
    SIGNAL_VARS 
    uint8                 call_id;
    MN_CALL_UTU_T         user_to_user;  // user-to-user information
    MN_DUAL_SYS_E         dual_sys;
} APP_MN_USER_TO_USER_IND_T;

// inform the clients that the call entity is been connecting
typedef struct
{
    SIGNAL_VARS
    uint8                 call_id;
    MN_DUAL_SYS_E         dual_sys;
} APP_MN_CONNECTING_IND_T;

// inform the clients that the call entity is been disconnecting
typedef struct
{
    SIGNAL_VARS
    uint8                 call_id;
    MN_DUAL_SYS_E         dual_sys;
} APP_MN_DISCONNECTING_IND_T;

// this signal is used to report call server ready
typedef struct
{
    SIGNAL_VARS

    BOOLEAN              fdn_is_enable;
    BOOLEAN              adn_is_enable;
    BOOLEAN              is_usim;
    MN_DUAL_SYS_E        dual_sys;

} APP_MN_CALL_READY_IND_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN                  is_update_ok;
    MN_DUAL_SYS_E            dual_sys;
} APP_MN_LANGUAGE_UPDATE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN                  is_ok;
    MN_DUAL_SYS_E            dual_sys;
    MN_PHONE_PLMN_SEL_LIST_T new_prefer_plmn_list;
} APP_MN_PREFER_PLMN_CHANGE_CNF_T;

/*****************add the types of mnsmscb layer***********/
#ifdef  SMSCB_MSG_ID_MAX_NUM_35
#define MN_CB_MSG_ID_MAX_NUM      35 
#elif defined SMSCB_MSG_ID_MAX_NUM_30
#define MN_CB_MSG_ID_MAX_NUM      30 
#elif defined  SMSCB_MSG_ID_MAX_NUM_15
#define MN_CB_MSG_ID_MAX_NUM      15
#elif defined  SMSCB_MSG_ID_MAX_NUM_10
#define MN_CB_MSG_ID_MAX_NUM      10
#else
#define MN_CB_MSG_ID_MAX_NUM      10
#endif

#define MN_CB_LANGUAGE_ID_MAX_NUM 22

//If translate 7 bit alphabet to 8 bits, 82 bytes to 93 chars.
#define MN_CB_MAX_PAGE_NUM        15
#define SMSCB_PAGE_LENGTH         88
#define MN_CB_MSG_LEN             93
#define MN_CB_MACRO_MSG_LEN       93*15

typedef uint8  MN_CB_UPDATE_T;
typedef uint8  MN_MESSAGE_CODE_T ;
typedef uint16  MN_MSG_CODE_T ;

typedef uint8 MN_CB_MSG_ARR_T[ MN_CB_MSG_LEN ] ;
typedef uint8 MN_CB_MACRO_MSG_ARR_T[ MN_CB_MACRO_MSG_LEN ] ;


typedef struct
{
    uint8 num ;
    MN_CB_MSG_ID_T msg_id_arr [ MN_CB_MSG_ID_MAX_NUM ] ;
} MN_CB_MSG_ID_MUSTER_T ;

typedef struct
{
    uint8 num;
    MN_SMSCB_LANGUAGE_ID_E language_id_arr[ MN_CB_LANGUAGE_ID_MAX_NUM ] ;
} MN_CB_LANGUAGE_ID_MUSTER_T ;

typedef enum{
    MN_CB_DISPLAY_IMMEDIATE,
    MN_CB_DISPLAY_NORMAL
}MN_CB_DISPLAY_MODE_E;

typedef struct APP_MN_SMSCB_MSG_PAGE_IND_T_s
{
    SIGNAL_VARS
    uint8           total_pages;
    uint8           buf_len[MN_CB_MAX_PAGE_NUM];
    uint8           page_buf[MN_CB_MAX_PAGE_NUM][SMSCB_PAGE_LENGTH];
    MN_DUAL_SYS_E   dual_sys;
} APP_MN_SMSCB_MSG_PAGES_IND_T;
typedef struct APP_MN_GET_SMSCB_PARAM_CNF_T_s
{
    SIGNAL_VARS
    BOOLEAN is_smscb_active;    //TRUE: ACTIVE; FALSE:NULL
    uint8 is_all_channel_on;    //TRUE: All channels are set; FALSE: ONLY some channels are set
    MN_CB_MSG_ID_MUSTER_T msg_id_list; //mi number and mi array
    MN_DUAL_SYS_E   dual_sys;
}APP_MN_GET_SMSCB_PARAM_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_CB_MSG_ID_T  msg_id;
    MN_SMSCB_DCS_T  dcs;
    uint16          msg_len;
    MN_CB_MSG_ARR_T msg_arr;
    MN_DUAL_SYS_E   dual_sys;
    MN_CB_DISPLAY_MODE_E display_mode;
    uint16          msg_code; 
} APP_MN_SMSCB_MSG_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_CB_MSG_ID_T          msg_id;
    MN_SMSCB_DCS_T          dcs;
    uint16                  msg_len;
    MN_CB_MACRO_MSG_ARR_T   msg_arr;
    MN_DUAL_SYS_E           dual_sys;
    MN_CB_DISPLAY_MODE_E display_mode;
    uint16          msg_code; 
} APP_MN_SMSCB_MACRO_MSG_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E           dual_sys;
} APP_MN_SMSCB_REJ_IND_T;

typedef struct
{
    SIGNAL_VARS
    uint16      length;
    uint16      result;
    int16*      block_ptr;
} APP_MN_READ_DSP_MEM_CNF_T;

typedef struct
{
    uint32          plmn_status;
    BOOLEAN         plmn_is_roaming;    // if plmn is roaming plmn  
    uint16          mcc;
    uint16          mnc;
    uint16          mnc_digit_num;
    uint16          lac;
    uint32          cell_id;
    MN_GMMREG_RAT_E rat;
}MSG_CAMP_STATUS_T;


 /*****************************************************************************
 **                        ENG MODULE DEFINE                                  *
 ******************************************************************************/
typedef enum _MN_PHONE_STATUS_E
{
    MN_GSM_IN_IDLE,
    MN_GSM_IN_CALL
} MN_PHONE_STATUS_E;

typedef struct
{
    uint8       ncc;
    uint8       bcc;
} MN_BSIC_T;

typedef struct
{
    BOOLEAN     cell_present;
    uint16      arfcn;
    MN_BSIC_T   bsic;
    uint32      cell_id;
    MN_LAI_T    lai;
    uint32      cell_band;
} MN_ENG_CELL_BASIC_DATA_T;

typedef struct
{
    MN_ENG_CELL_BASIC_DATA_T     scell_info;
    MN_ENG_CELL_BASIC_DATA_T     ncell_info[MN_MAX_NCELL_NUM];
} MN_ENG_CELL_BASIC_INFO_T;

typedef struct
{
    int16       rxlev_access_min;
    uint8       ms_txpwr_maxcch;
    uint8       power_offset;
    uint8       p;
    uint8       ms_txpwr_dbm;
} MN_SEL_T;

typedef struct
{
    uint8       cell_resel_off;
    uint8       temporary_off;
    uint16      penalty_time;
    uint8       crh;
} MN_RESEL_T;

typedef enum
{
    MN_NO_CELL                          = 0,
    MN_ACCEPTABLE_CELL_IN_FORBID_LIST   = 1,
    MN_ACCEPTABLE_CELL                  = 2, 
    MN_LOW_PRIO_CELL                    = 3,
    MN_NORMAL_PRIO_CELL                 = 4
} MN_CELL_PRIO_E;

typedef struct
{
    int16           rssi;
    MN_SEL_T        sel_param;
    MN_RESEL_T      resel_param;
    int8            c1;
    int8            c2;
    int8            c31;
    int8            c32;
    BOOLEAN         gprs_ind_present;
    uint8           ra_colour;
    MN_CELL_PRIO_E  cell_prio;
    BOOLEAN         cbch_present;
    uint8           dsc_cur;
} MN_ENG_CELL_IDLE_DATA_T;

typedef struct
{
    MN_ENG_CELL_IDLE_DATA_T  scell_info;
    MN_ENG_CELL_IDLE_DATA_T  ncell_info[MN_MAX_NCELL_NUM];
} MN_ENG_CELL_IDLE_INFO_T;

typedef struct
{
    uint8       max_retrans;
    uint8       tx_integer;
    uint8       cba;
    BOOLEAN     re_est;
    uint16      access_class;
} MN_RACH_T;

typedef enum
{
    MN_PCH1_NC = 0,
    MN_PCH1_C  = 1,
    MN_PCH2_NC = 2,
    MN_PCH3_NC = 3,
    MN_PCH4_NC = 4
} MN_CCCH_CONFIG_E;

typedef struct
{
    uint8               bs_ag;
    uint8               bs_pa;
    uint16              t3212_value;
    BOOLEAN             att;
    MN_CCCH_CONFIG_E    ccch_config;
    BOOLEAN             ccch_conbined;
    uint8               ccch_channel_count;
    int16               ccch_group;
    int16               paging_group;
    uint16              paging_period;
    uint8               dsc;
    uint8               ccch_timeslot;
    uint8               pch_blk_per_mfrms;
    uint8               pch_blk_index;
} MN_CTL_CHN_T;

typedef enum
{
    MN_NC0 = 0,
    MN_NC1 = 1,
    MN_NC2 = 2,
    MN_NC_SPARE = 3
} MN_NC_ORDER_E;

typedef struct
{
    MN_NC_ORDER_E      nc_order;
    uint16             nc_non_drx_period;
    uint16             nc_reporting_period_i;
    uint16             nc_reporting_period_t;
} MN_NC_T;

typedef enum
{
    MN_PAGING_NORMAL = 0, 
    MN_PAGING_EXTENDED = 1,
    MN_PAGING_REORGANISATION = 2,
    MN_PAGING_SAME_AS_BEFORE = 3
} MN_PAGING_MODE_E;

typedef enum
{
    MN_GPRS_MNO_NONE = 0,
    MN_GPRS_MNO_ONE = 1,
    MN_GPRS_MNO_TWO = 2,
    MN_GPRS_MNO_THREE = 3
} MN_NETWORK_MODE_E;

typedef enum
{
    MN_BCCH_NORM = 0,
    MN_BCCH_EXT = 1
} MN_SI13_POSITION_E;

typedef struct
{
    uint8                   rac;
    MN_RACH_T               rach_param;
    uint8                   multiband_report;
    MN_CTL_CHN_T            ctl_chn_param;
    uint8                   ncc_permitted;
    MN_NC_T                 nc_param;
    BOOLEAN                 early_classmark_send;
    MN_PAGING_MODE_E        page_mode;
    MN_NETWORK_MODE_E       nmo;
    BOOLEAN                 pbcch_present;
    BOOLEAN                 dtx_allowed;
    BOOLEAN                 pwrc;
    uint8                   rlt;
    MN_SI13_POSITION_E      si13_position;
    uint8                   priority_access_thr;
} MN_ENG_SCELL_CTRL_INFO_T;

typedef struct
{
    uint32                  delta_fn;
    int16                   otd;
} MN_ENG_NCELL_CTRL_DATA_T;

typedef struct
{
    MN_ENG_NCELL_CTRL_DATA_T    ncell_info[MN_MAX_NCELL_NUM];
} MN_ENG_NCELL_CTRL_INFO_T;

typedef struct
{
    int16       rxlev_full;
    int16       rxlev_sub;
} MN_RXLEV_T;

typedef struct
{
    uint8       rxqual_full;
    uint8       rxqual_sub;
} MN_RXQUAL_T;

typedef enum
{
    MN_SACCH    = 0,
    MN_SDCCH    = 1,
    MN_FACCH_H  = 2,
    MN_FACCH_F  = 3,
    MN_BCCH     = 4,
    MN_PCH      = 5,
    MN_AGCH     = 6,
    MN_PBCCH    = 7,
    MN_PPCH     = 8,
    MN_PAGCH    = 9,
    MN_PDTCH    = 10,
    MN_PACCH    = 11,
    MN_NO_CHANNEL = 12
} MN_CHANNEL_TYPE_E;

typedef struct
{
    BOOLEAN             active;
    MN_CHANNEL_TYPE_E   channel_type;
} MN_SAPI_T;

typedef enum
{
    MN_TCH_H    = 0,
    MN_TCH_F    = 1,
    MN_SDCCH_4  = 2,
    MN_SDCCH_8  = 3
} MN_DCH_TYPE_E;

typedef struct
{
    MN_DCH_TYPE_E   dch_type;
    uint8           sub_channel;
    uint8           tn;
    uint8           tsc;
    BOOLEAN         hopping;
    uint8           maio;
    uint8           hsn;
    uint16          tch_arfcn;
} MN_CHANNEL_DESCRIPTION_T;

#define MN_MAX_BA_SIZE 64

typedef struct
{
    int16           num;
    uint16          arfcn_arr[MN_MAX_BA_SIZE];
} MN_MA_LIST_T;

// GSM 04.18 10.5.2.6
// The mode field is encoded as follows:
typedef enum
{
    MN_SIGNAL_ONLY      = 0,    /*signalling only*/
    MN_F_H_RATE_VER_1   = 1,    /*speech full rate or half rate version 1*/
    MN_F_H_RATE_VER_2   = 2,    /*speech full rate or half rate version 2*/
    MN_F_H_RATE_VER_3   = 3,    /*speech full rate or half rate version 3*/
    MN_DATA_43D_14U     = 4,    /*data, 43.5 kbit/s (downlink)+14.5 kbps (uplink)*/
    MN_DATA_29D_14U     = 5,    /*data, 29.0 kbit/s (downlink)+14.5 kbps (uplink)*/
    MN_DATA_43D_29U     = 6,    /*data, 43.5 kbit/s (downlink)+29.0 kbps (uplink)*/
    MN_DATA_14D_43U     = 7,    /*data, 14.5 kbit/s (downlink)+43.5 kbps (uplink)*/
    MN_DATA_14D_29U     = 8,    /*data, 14.5 kbit/s (downlink)+29.0 kbps (uplink)*/
    MN_DATA_29D_43U     = 9,    /*data, 29.0 kbit/s (downlink)+43.5 kbps (uplink)*/
    MN_DATA_43_RATE     = 10,   /*data, 43.5 kbit/s radio interface rate*/
    MN_DATA_32_RATE     = 11,   /*data, 32.0 kbit/s radio interface rate*/
    MN_DATA_29_RATE     = 12,   /*data, 29.0 kbit/s radio interface rate*/
    MN_DATA_14_RATE     = 13,   /*data, 14.5 kbit/s radio interface rate */
    MN_DATA_12_RATE     = 14,   /*data, 12.0 kbit/s radio interface rate*/
    MN_DATA_6_RATE      = 15,   /*data, 6.0 kbit/s radio interface rate*/
    MN_DATA_3_RATE      = 16    /*data, 3.6 kbit/s radio interface rate*/
} MN_CHANNEL_MODE_E;

// GSM 04.18 10.5.2.9
// algorithm identifier
typedef enum
{
    MN_CIPHER_WITH_A5_1 = 0,
    MN_CIPHER_WITH_A5_2 = 1,
    MN_CIPHER_WITH_A5_3 = 2,
    MN_CIPHER_WITH_A5_4 = 3,
    MN_CIPHER_WITH_A5_5 = 4,
    MN_CIPHER_WITH_A5_6 = 5,
    MN_CIPHER_WITH_A5_7 = 6
} MN_CIPHER_ALGORITHM_E;

typedef struct
{
    BOOLEAN                 cipher_mode;
    MN_CIPHER_ALGORITHM_E   cipher_algorithm;
} MN_CIPHER_PARAM_T;

typedef struct
{
    MN_RXLEV_T                  rxlev;
    MN_RXQUAL_T                 rxqual;
    uint8                       ta;
    uint8                       power_level;
    uint8                       ms_tx_cur;
    BOOLEAN                     dtx_used;
    uint8                       rlt_cur;
    MN_SAPI_T                   sapi0;
    MN_SAPI_T                   sapi3;
    MN_CHANNEL_DESCRIPTION_T    channel_dec;
    MN_MA_LIST_T                ma_list;
    MN_CHANNEL_MODE_E           channel_mode;
    MN_CIPHER_PARAM_T           cipher_param;
    BOOLEAN                     multislot_supported;
    BOOLEAN                     is_dedicate_mode;
} MN_ENG_SCELL_TRAF_INFO_T;

typedef struct
{
    int16       rxlev;
} MN_ENG_NCELL_TRAF_DATA_T;

typedef struct
{
    MN_ENG_NCELL_TRAF_DATA_T    ncell_info[MN_MAX_NCELL_NUM];
} MN_ENG_NCELL_TRAF_INFO_T;

typedef uint8 MN_KC_T[MN_MAX_KC_ARR_LEN];

typedef struct
{
    BOOLEAN     valid_flag;
    uint8       ptmsi_value[MN_MAX_PTMSI_ARR_LEN];
} MN_PTMSI_T;

typedef struct
{
    MN_PHONE_STATUS_E   phone_status;
    MN_TMSI_T           tmsi;
    MN_PTMSI_T          ptmsi;
    MN_KC_T             kc;
    uint8               cksn;
} MN_ENG_PHONE_INFO_T;

typedef struct
{
    // bsic search number
    uint32 bsic_search_total;       // 请求BSIC Search的总次数
    uint32 bsic_search_succ;        // BSIC Search成功的次数
    uint32 bsic_search_fail;        // BSIC Search Fail Number
    // bsic confirm number
    uint32 bsic_confirm_total;      // 请求BSIC confirm的总次数
    uint32 bsic_confirm_succ;       // BSIC confirm成功的次数
    uint32 bsic_confirm_fail;       // BSIC confirm Fail Number
    // bcch decode number
    uint32 scell_bcchdec_continue;  // 请求连续Decode服务小区BCCH的次数
    uint32 scell_bcchdec_single;    // 请求Decode服务小区单个BCCH块的次数
    uint32 scell_bcch_total;        // 进行对服务小区BCCH Decode的总的Block数
    uint32 scell_bad_bcch;          // 对服务小区BCCH Decode的Bad BCCH的Block数
    uint32 ncell_bcchdec_continue;  // 请求连续Decode临近小区BCCH的次数
    uint32 ncell_bcchdec_single;    // 请求Decode临近小区单个BCCH块的次数
    uint32 ncell_bcch_total;        // 进行对临近小区BCCH Decode的总的Block数
    uint32 ncell_bad_bcch;          // 对临近小区BCCH Decode的Bad BCCH的Block数
    // Rx_Lev number
    uint32 rxlev_total;             // Layer1进行测量的总的次数
    uint32 rxlev_bad;               // Layer1进行重新测量的次数
    // PCH Block Number
    uint32 pch_block_total;         // 收到的PCH总数
    uint32 bad_pch_num;             // Bad的PCH总数
    // RLA Sort Number
    uint32 rla_sort_total;          // Layer1进行六强排序的总数
    // RACH
    uint32 send_rach_num;           // 发送RACH的次数
    uint32 rx_agch_num;             // 正确接受的AGCH数目
    // PRACH
    uint32 send_prach_num;          // 发送PRACH的次数
    uint32 rx_pagch_num;            // 正确接收的PRACH数目
    
    // BCCH Camp Number
    uint32 bcch_camp_num;           // Layer1进行CampOn的总次数
    // PowerSweep Number
    uint32 pwrswp_num;              // Layer1进行PowerSweep的总次数
    // Handover number
    uint32 handover_total;          // Layer1进行Handover的总次数
    uint32 handover_fail;           // Layer1进行Handover失败的总次数
} MN_ENG_LAYER1_MONITOR_ITEM_T;

typedef enum
{
    MN_GU1_UPDATED          = 0,
    MN_GU2_NOT_UPDATED      = 1,
    MN_GU3_ROAM_NOT_ALLOWED = 3
} MN_GMM_STATE_UPDATED;

typedef struct
{
    uint8       uplink_ts;
    uint8       downlink_ts;
} MN_GPRS_ALLOC_TS;

typedef struct
{
    int16                   nsapi_num;
    MN_GMM_STATE_UPDATED    rau_status;
    uint32                  tlli_cur;
    BOOLEAN                 is_gprs_ciphered;
    MN_GPRS_ALLOC_TS        gprs_alloc_ts;
    uint8                   gprs_multislot_class;
} MN_ENG_GPRS_INFO_T;


typedef struct _SIMCHECK_GET_SIM_TYPE_CNF_T
{
	MN_SIM_TYPE_CAUSE_E cause;
	MN_CARD_TYPE_E card_type;
} SIMCHECK_GET_SIM_TYPE_CNF_T;

typedef struct
{
    uint16  arfcn; 
    int16   rssi;  
} MN_ENG_ARFCN_POWERLEVEL_T;

typedef struct
{
    int16                   	num;
    MN_ENG_ARFCN_POWERLEVEL_T  	pwr_result_list[MN_MAX_POWER_SWEEP_LEN];
} MN_ENG_POWER_SWEEP_INFO_T;

typedef struct
{
    uint8   len;                            
    MN_NUMBER_TYPE_E   ton;   
    MN_NUMBER_PLAN_E   npi;  
    uint8  ss_ptr[MN_MAX_SIMAT_SS_STR_LEN];
}SIMAT_DO_SS_STRING_T;

/*the pclink config struct*/
typedef struct
{
	BOOLEAN  is_pclink_open;  /* pclink function open flag, 1 - open, 0 - not open */
	unsigned long ipaddr;     /* the ip address */
	unsigned long snmask;     /* the subnet mask */
	unsigned long defgw;      /* the default gateway */
	unsigned long dns;        /* the DNS */    
}MN_GPRS_PCLINK_CFG_T;


typedef enum
{
	APP_MN_SIM_MBBMS_RESPONSE_SUCCESS,
    APP_MN_SIM_MBBMS_RESPONSE_CONTEXT_NOT_SUPPORT,
    APP_MN_SIM_MBBMS_RESPONSE_MAC_FAILURE,
	APP_MN_SIM_MBBMS_RESPONSE_SYNC_FAILURE,
	APP_MN_SIM_MBBMS_RESPONSE_OTHER_FAILURE
} APP_MN_SIM_MBBMS_RESPONSE_TYPE_E;
typedef struct APP_MN_SIM_MBBMS_ALGO_CNF_T
{
    SIGNAL_VARS
    MN_DUAL_SYS_E   dual_sys;
    APP_MN_SIM_MBBMS_RESPONSE_TYPE_E         ret;
    uint8 sec_context;    /*0--sim, 1--usim*/
    uint8 kc_ck[APP_MN_KC_CK_LEN]; /*LV style, first byte is len*/
    uint8 sres_res_autn[APP_MN_IK_LEN];/*LV style, first byte is len*/
    uint8 ik[APP_MN_SRES_AUTN_LEN];/*LV style, first byte is len*/
}APP_MN_SIM_MBBMS_ALGO_CNF_T;

#define MN_MAX_IMG_DATA_BODY_NUM 253

typedef enum
{
    MN_SIM_IMG_CODE_TYPE_BASIC = 0x11,
    MN_SIM_IMG_CODE_TYPE_COLOR = 0x21        
}MN_SIM_IMG_CODE_TYPE_E;

typedef struct
{
    uint8 img_width;
    uint8 img_height;
    MN_SIM_IMG_CODE_TYPE_E  img_cs;
    uint16 img_len;                  //for basic image,the len= pure image data len,for color image,len=pure image data len + 3*num_of_clut_entry
    uint8 bits_per_img_point; //only for color image
    uint8 num_of_clut_entry; //only for color image
    uint8 location_clut;          //only for color image,location of clut in the image body
    uint8  img_body[MN_MAX_IMG_DATA_BODY_NUM]; //for basic image,pure image data in it;for color image,include pure image data and CLUT
}MN_SIM_IMG_T;

typedef enum MN_SEND_SMS_CAUSE_TYPE_E
{
    MN_SEND_SMS_SUCCESS,           //send sms ok
    MN_SEND_SMS_CANCEL_SUCCESS, //cancel sms ok
    MN_SEND_SMS_PLMN_NO_SER,    //send sms failed for no ser
    MN_SEND_SMS_SIM_NOT_READY, //send sms fail for sim not ready
    MN_SEND_SMS_MN_BUSY,          // send sms fail for mn is busy
    MN_SEND_SMS_FDN_FAILED,    //send sms fail for fdn check fail
    MN_SEND_SMS_STK_NOT_ALLOWED,//send sms fail for stk call control failed
    MN_SEND_SMS_LOW_LAYER_FAIL,   // send sms fail for low layer fail
    MN_SEND_SMS_RP_ERROR,	            //send sms fail for rp-error
    MN_SEND_SMS_CP_ERROR             //send sms fail for cp-error
} MN_SEND_SMS_CAUSE_TYPE_E;

typedef struct MN_SEND_SMS_SMS_CAUSE_T
{
	MN_SEND_SMS_CAUSE_TYPE_E 			cause_type;
	uint16								cause_value;
}MN_SEND_SMS_SMS_CAUSE_T;

/*sms cause for cp-error,all other value shall treated as cause number 111*/
#define MN_SMS_CP_ERROR_NET_FAIL            17
#define MN_SMS_CP_ERROR_CONGESTION          22
#define MN_SMS_CP_ERROR_INVALID_TI          81
#define MN_SMS_CP_ERROR_SEM_ERR_MSG         95
#define MN_SMS_CP_ERROR_INVALID_M_INFO      96
#define MN_SMS_CP_ERROR_MSG_TYPE_NON_EXIST  97
#define MN_SMS_CP_ERROR_MSG_NOT_COMPATIBLE  98
#define MN_SMS_CP_ERROR_IE_NOT_EXIST        99
#define MN_SMS_CP_ERROR_TC1M_EXP            102
#define MN_SMS_CP_ERROR_PROTOCOL_ERROR      111

/*sms cause for rp-error,all other value shall treated as cause number 41*/
#define MN_SMS_RP_ERROR_UNSIGNED_NUMBER         1
#define MN_SMS_RP_ERROR_OPER_DETERMINED_BAR     8
#define MN_SMS_RP_ERROR_CALL_BARRED             10
#define MN_SMS_RP_ERROR_RESERVED                11
#define MN_SMS_RP_ERROR_SMS_TRANS_REJ           21
#define MN_SMS_RP_ERROR_SMS_MEM_EXCEED          22
#define MN_SMS_RP_ERROR_DES_OUTOF_ORDER         27
#define MN_SMS_RP_ERROR_UNDEFINE_SUBSCRIBER     28
#define MN_SMS_RP_ERROR_FACILITY_REJ            29
#define MN_SMS_RP_ERROR_UNKNOWN_SUBSCRIBER      30
#define MN_SMS_RP_ERROR_NET_OUTOF_ORDER         38
#define MN_SMS_RP_ERROR_TEMP_FAILURE            41
#define MN_SMS_RP_ERROR_CONGESTION              42
#define MN_SMS_RP_ERROR_RES_UNAVAILABLE         47
#define MN_SMS_RP_ERROR_FACILITY_NOT_SUB        50
#define MN_SMS_RP_ERROR_FACILITY_NOT_IMPLEM     69
#define MN_SMS_RP_ERROR_INVALID_TPMR            81
#define MN_SMS_RP_ERROR_SEM_ERR_MSG             95
#define MN_SMS_RP_ERROR_INVALID_M_INFO          96
#define MN_SMS_RP_ERROR_MSG_TYPE_NON_EXIST      97
#define MN_SMS_RP_ERROR_MSG_NOT_COMPATIBLE      98
#define MN_SMS_RP_ERROR_IE_NOT_EXIST            99
#define MN_SMS_RP_ERROR_TR1M_EXP                102
#define MN_SMS_RP_ERROR_PROTOCOL_ERROR          111
#define MN_SMS_RP_ERROR_INTERWORK_UNSPEC        127

/*sms cause for low layer error,all other value shall treated as number 10*/
#define MN_SMS_LOWLAYER_ERROR_AUTHENTICATION_REJ        150
#define MN_SMS_LOWLAYER_ERROR_EMERGENCY_CALL_ONLY    	151
#define MN_SMS_LOWLAYER_ERROR_IMSI_DETACH    			152
#define MN_SMS_LOWLAYER_ERROR_T3230_EXPIRY    			153
#define MN_SMS_LOWLAYER_ERROR_RR_CONNECTIN_ERROR        154
#define MN_SMS_LOWLAYER_ERROR_CALL_REJECTED             21
#define MN_SMS_LOWLAYER_ERROR_SERV_OPT_NOT_SUPPORTED    32
#define MN_SMS_LOWLAYER_ERROR_MM_CONNECTION_FAIL 	    10

typedef struct
{
	uint8 code[2];
}MN_SIM_EXT_LANGUAGE_CODE_T;

typedef MN_SIM_EXT_LANGUAGE_CODE_T MN_SIM_EXT_LANGUAGE_CODE_ARR[MN_SIM_MAX_VALIDATE_LANGUAGE_NUM];

typedef uint8 MN_SIM_LANGUAGE_CODE_ARR[MN_SIM_MAX_VALIDATE_LANGUAGE_NUM];

typedef struct _MN_SIM_LANGUAGE_LIST_T
{
    BOOLEAN is_1st_valid;
    uint8 elp_1st_max_num;
    uint8 elp_1st_valid_num;
    MN_SIM_EXT_LANGUAGE_CODE_ARR elp_1st_lang_codes;
    BOOLEAN is_lp_valid;
    uint8 lp_1st_max_num;
    uint8 lp_1st_valid_num;
    MN_SIM_LANGUAGE_CODE_ARR lp_1st_lang_codes;
    BOOLEAN is_2nd_valid;
    uint8 elp_2nd_max_num;
    uint8 elp_2nd_valid_num;
    MN_SIM_EXT_LANGUAGE_CODE_ARR elp_2nd_lang_codes;
}MN_SIM_LANGUAGE_LIST_T;

typedef struct _MN_SIM_LANGUAGE_INFO_T
{
    uint8 elp_valid_num; //number of ELP
    MN_SIM_EXT_LANGUAGE_CODE_ARR elp_lang_codes;
    uint8 lp_valid_num; //number of LP
    MN_SIM_LANGUAGE_CODE_ARR lp_lang_codes;
}MN_SIM_LANGUAGE_INFO_T;

typedef enum
{
    APP_MN_SIM_COMMON_ALGO_RESPONSE_SUCCESS,
    APP_MN_SIM_COMMON_ALGO_RESPONSE_CONTEXT_NOT_SUPPORT,
    APP_MN_SIM_COMMON_ALGO_RESPONSE_MAC_FAILURE,
    APP_MN_SIM_COMMON_ALGO_RESPONSE_SYNC_FAILURE,
    APP_MN_SIM_COMMON_ALGO_RESPONSE_OTHER_FAILURE
} APP_MN_SIM_COMMON_ALGO_RESPONSE_TYPE_E;

typedef struct APP_MN_SIM_COMMON_ALGO_CNF_T
{
    SIGNAL_VARS
    MN_DUAL_SYS_E   dual_sys;
    uint32 algo_index;
    APP_MN_SIM_COMMON_ALGO_RESPONSE_TYPE_E         ret;
    uint8 sec_context;                                                /*0--sim, 1--usim*/
    uint8 kc[APP_MN_KC_CK_LEN];                               /*LV style, first byte is len*/
    uint8 sres_res_autn[APP_MN_SRES_AUTN_LEN];     /*LV style, first byte is len*/
    uint8 ck[APP_MN_KC_CK_LEN];                               /*LV style, first byte is len*/
    uint8 ik[APP_MN_IK_LEN];                                       /*LV style, first byte is len*/
}APP_MN_SIM_COMMON_ALGO_CNF_T;

typedef struct _MN_IMSI_INFO_T
{
    uint8 parity_and_type;
    uint8 imsi_ascii_len;
    uint8 imsi_ascii_val[MN_MAX_IMSI_INFO_ARR_LEN];
    uint8 mcc_ascii[MN_MAX_MCC_MNC_ARR_LEN];
    uint8 mnc_ascii_len;
    uint8 mnc_ascii[MN_MAX_MCC_MNC_ARR_LEN];
}MN_IMSI_INFO_T;

//begin to adapt to the SS IPC
typedef struct
{
    uint8     *data_ptr;
    uint16    data_length;/* rec_len*num_records */
    uint8     num_records;/* rec numbers in one read or ... time*/
} MN_SIM_PARA_VALUE_T;

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

typedef enum
{
    MN_SIM_SERVICE_NOT_SUPPORT,
    MN_SIM_SERVICE_ENABLED,
    MN_SIM_SERVICE_DISABLED
} MN_SIM_SERVICE_E;

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

typedef struct
{
    SIGNAL_VARS
    BOOLEAN                              is_ok; //indicate whether successfully
    uint16	                             file_id;	//file id of the file
    MN_SIM_PARA_VALUE_T                  search_result;   //The record number given from SIM
    MN_DUAL_SYS_E                        dual_sys;
} APP_MN_SIM_SEARCH_RECORD_CNF_T;

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
//end to adapt to the SS IPC

typedef struct _MN_PHONE_NET_LOCK_PLMN_LIST_T
{
	uint8		plmn_num;
	MN_PLMN_T   plmn_arr[MN_NET_LOCK_PLMN_LIST_SIZE];//== NET_LOCK_PLMN_LIST_SIZE
							  //It cannot be changed!
} MN_PHONE_NET_LOCK_PLMN_LIST_T; 

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
	BOOLEAN result;
	uint8 data_len;
	uint8 data[255];
} APP_MN_CSIM_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E      dual_sys;
    uint16                      arfcn;
    BOOLEAN                 is_ok;
} APP_MN_SWITCH_ARFCN_CNF_T;

typedef enum
{
   MN_IDENTIFY_BS_MODE0,
   MN_IDENTIFY_BS_MODE1,
   MN_IDENTIFY_BS_MODE_MAX,
}MN_IDENTIFY_BS_MODE_E;

// TD related
#ifdef PRODUCT_DM
#include "mn_type_td.h"
#endif

typedef struct
{
    BOOLEAN     is_valid;
    // Channel mode supported
    BOOLEAN     channel_type_flag;          // 0: FULL_RATE_ONLY 1: DUAL_RATE
    BOOLEAN     half_rate_prefer_flag;      // TRUE: prefer half rate; FALSE: prefer full rate 
    BOOLEAN     full_rate_v1_supported;     // GSM full rate speech version 1,FS is must
    BOOLEAN     full_rate_v2_supported;     // GSM full rate speech version 2,EFS 
    BOOLEAN     full_rate_v3_supported;     // GSM full rate speech version 3,AFS 
    BOOLEAN     half_rate_v1_supported;     // GSM half rate speech version 1,HS
    BOOLEAN     half_rate_v3_supported;     // GSM half rate speech version 3,AHS
} MN_SPEECH_VERSION_INFO_T;

typedef enum {
    MN_IMS_SERVICE_STATE_INACTIVE = 0, // similar +CREG:0
    MN_IMS_SERVICE_STATE_ACTIVE,       // similar +CREG:1
    MN_IMS_SERVICE_STATE_IN_PROGRESS,  // similar +CREG:2
    MN_IMS_SERVICE_STATE_FAILED,       // similar +CREG:3
    MN_IMS_SERVICE_STATE_UNKNOWN,      // similar +CREG:4
    MN_IMS_SERVICE_STATE_ROAMING,      // similar +CREG:5
    MN_IMS_SERVICE_STATE_DEREGISTERING = 10, /* Specific state to indicte we are doing deregister. No mapping CREG value. */
    MN_IMS_SERVICE_STATE_REREGISTER_SUCCESS
} MN_IMS_STATE_E;

typedef struct
{
	SIGNAL_VARS
	MN_IMS_STATE_E nState;
	MN_DUAL_SYS_E dual_sys;
	
}APP_MN_IMS_STATE_IND_T;

typedef struct
{
	SIGNAL_VARS
	uint8 nPath;
	MN_DUAL_SYS_E dual_sys;
	
}APP_MN_CALL_PATH_IND_T;

typedef struct
{
	SIGNAL_VARS
	BOOLEAN nResult;
	MN_DUAL_SYS_E dual_sys;
}APP_MN_CHANGE_RAT_CNF_T;

typedef struct
{
	SIGNAL_VARS
	MN_DUAL_SYS_E dual_sys;
}APP_MN_CC_AUDIO_ON_IND_T;
typedef struct
{
	SIGNAL_VARS
	MN_DUAL_SYS_E dual_sys;
}APP_MN_CC_AUDIO_RESTART_IND_T;

typedef enum
{
    MN_PDP_CTX_TYPE_XCAP,
    MN_PDP_CTX_TYPE_IMS,
    MN_PDP_CTX_TYPE_IA,
    MN_PDP_CTX_TYPE_NUM,
    MN_PDP_CTX_TYPE_INVALID= 0xFFFFFFFF
}MN_INTERNAL_PDP_CTX_TYPE;

typedef struct
{
    SIGNAL_VARS
    /*
    0 Idle
    1 Connected
    */
    uint8 ConnMode;
    /*
    0   UTRAN URA_PCH state
    1   UTRAN Cell_PCH state
    2   UTRAN Cell_FACH state
    3   UTRAN Cell_DCH state
    4   GERAN CS connected state
    5   GERAN PS connected state
    6   GERAN CS and PS connected state
    7   E-UTRAN connected state
    */
    uint8 State;
    /*
    0 Indicates usage of radio access of type GERAN, see 3GPP TS 45.001 [146].
    1 Indicates usage of radio access of type UTRAN TDD, see 3GPP TS 25.212 [144].
    2 Indicates usage of radio access of type UTRAN FDD, see 3GPP TS 25.212 [144].
    3 Indicates usage of radio access of type E-UTRAN TDD, see 3GPP TS 36.300 [145].
    4 Indicates usage of radio access of type E-UTRAN FDD, see 3GPP TS 36.300 [145].
    */
    uint8 Access;

    uint8 rr_L1_ConnectState;
    uint16 Arfcn;
    uint8 Bsic;
    uint8 Lai[5];
    uint8 Rac;
    uint8 CellId[2];

    MN_DUAL_SYS_E dual_sys;
} APP_MN_NW_CONN_STATUS_INFO;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
} APP_MN_HSL_ABORT_IND_INFO;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
} APP_MN_HSL_RECOVERY_IND_INFO;

typedef struct
{
    SIGNAL_VARS
    uint8 nResult;
    MN_DUAL_SYS_E dual_sys;
} APP_MN_LTE_PRIORITY_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint8 nResult;
    MN_DUAL_SYS_E dual_sys;
} APP_MN_HSL_START_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint8 nResult;
    MN_DUAL_SYS_E dual_sys;
} APP_MN_HSL_STOP_CNF_T;

#endif /* end of _MN_TYPE_H */
