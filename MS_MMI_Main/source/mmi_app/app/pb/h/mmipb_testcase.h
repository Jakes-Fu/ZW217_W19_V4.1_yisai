/****************************************************************************
** File Name:      mmipb_testcase.h                                          *
** Author:                                                                 *
** Date:           12/01/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with all test case code in pb             *
**                 pb模块所有的测试用例
****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        baokun yin       Create
******************************************************************************/

#ifndef _MMIPB_TESTCASE_H_
#define _MMIPB_TESTCASE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
//#define  CMCC_VCARD_SUPPORT
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#ifdef  CMCC_VCARD_SUPPORT
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
/*
typedef struct PHONEBOOK_EF_USAGE_T
{
    uint8 max_space_num; / *Total number* /
    uint8 free_space_num;  / *Unused number* /
    uint8 rec_len;  /* Length of each record* /
}PHONEBOOK_EF_USAGE_T;
*/

// typedef struct PHONEBOOK_STORAGE_USAGE_T
// {
//     PHONEBOOK_EF_USAGE_T adn_usage;
//     PHONEBOOK_EF_USAGE_T anr_usage[MNSIM_ANR_MAX_NUM];
//     PHONEBOOK_EF_USAGE_T email_usage;
//     PHONEBOOK_EF_USAGE_T sne_usage;
//     PHONEBOOK_EF_USAGE_T ext1_usage;
// }PHONEBOOK_STORAGE_USAGE_T;

// typedef struct PHONEBOOK_VCARD_STATUS_T
// {
//     uint8 storage_num;  /*Actual PBR number in BOTH TEL and ADF in current USIM*/
//     PHONEBOOK_STORAGE_USAGE_T storage_usage[PHONEBOOK_MAX_USAGE_STORAGE_NUM];
// }PHONEBOOK_VCARD_STATUS_T;

// typedef enum PHONEBOOK_ADD_UPDATE_OPERATION_E
// {
//     OPERATE_USIM_INSERT,
//     OPERATE_USIM_DELETE,
//     OPERATE_USIM_MODIFY,
//     OPERATE_USIM_NONE
// }PHONEBOOK_ADD_UPDATE_OPERATION_E;
// 
// typedef struct PHONEBOOK_IAP_ENTRY_T
// {
//     uint8   iap_data[MN_IAP_RECORD_MAX_LEN];	
//     uint8   iap_data_len;
// } PHONEBOOK_IAP_ENTRY_T; 
// 
// typedef struct PHONEBOOK_UPDATE_INFO_ENTRY_T
// {
//     PHONEBOOK_ENTRY_T adn_info;
//     PHONEBOOK_IAP_ENTRY_T iap_info;
// 		PHONEBOOK_ADD_UPDATE_OPERATION_E anr[MN_MAX_USIM_ANR_NUM];
//     PHONEBOOK_ADD_UPDATE_OPERATION_E email;
//     PHONEBOOK_ADD_UPDATE_OPERATION_E sne;  
// } PHONEBOOK_UPDATE_INFO_ENTRY_T;

/* +vcard dev binli */
 /*****************************************************************************
 **                        VCARD SUPPORT DEFINES                                  *
 ******************************************************************************/
// 
// typedef struct PHONEBOOK_ALPHA_STRING_T_s
// {
//     uint8   alpha[MN_PHONEBOOK_ALPHA_STRING_MAX_LEN];	
//     uint8   alpha_num;
// } PHONEBOOK_ALPHA_STRING_T;
// 
// typedef struct PHONEBOOK_USER_INFO_T
// {
//     uint8 		path_len;
//     uint8 		*path_ptr;
//     uint8 		rec_id;
// }PHONEBOOK_USER_INFO_T;
// 
// typedef struct PHONEBOOK_AAS_ENTRY_T
// {
//     uint8   alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN];
//     uint8			alpha_len;
// } PHONEBOOK_AAS_ENTRY_T;
// 
// typedef struct PHONEBOOK_READ_USER_INFO_ENTRY_T
// {
//     PHONEBOOK_USER_INFO_T 	read_file_info[PHONEBOOK_MAX_OPERATE_NUM];  /*CR179773 Eddie.Wang*/
//     uint8                                      adn_rec_id;
//     uint8 						file_num;
//     uint8						anr_file_num;
//     uint8						sne_file_num;  
//     uint8						email_file_num;   
// }PHONEBOOK_READ_USER_INFO_ENTRY_T;
// 
// typedef struct MN_USIM_EMAIL_STRING_T
// {
//     uint8   address[MN_USIM_EMAIL_STRING_MAX_LEN];	
//     uint8   address_len;
// } MN_USIM_EMAIL_STRING_T;
// 
// typedef struct PHONEBOOK_ANR_NUMBER_T
// {
//     MNBCD               number[PHONEBOOK_MAX_DN_NUMBER_LEN];
//     uint8               aas_id; //EFaas中的记录号
//     uint8               number_len;
//     MN_NUMBER_TYPE_E    ton;
//     MN_NUMBER_PLAN_E    npi;
//     BOOLEAN   			adn_id_exist; /*if byte 16 and 17 exist*/
//     uint8     adn_sfi; /*byte 16, sfi of ADN*/
//     uint8     id_in_adn; /*byte 17, corespond of rec. id in adn*/
// } PHONEBOOK_ANR_NUMBER_T;
// 
// typedef struct PHONEBOOK_ADDITIONAL_ENTRY_T
// {
//     PHONEBOOK_ANR_NUMBER_T          anr_number[MN_USIM_AAS_MAX_NUM];
//     MN_USIM_EMAIL_STRING_T            email_addr;
//     PHONEBOOK_ALPHA_STRING_T        sne_alpha;  
//     uint16                                          entry_id;	
// } PHONEBOOK_ADDITIONAL_ENTRY_T;
// 
// typedef struct PHONEBOOK_UPDATE_USER_INFO_ENTRY_T
// { 
//     PHONEBOOK_USER_INFO_T			anr_info[MN_MAX_USIM_ANR_NUM];  
//     PHONEBOOK_USER_INFO_T			adn_info;
//     PHONEBOOK_USER_INFO_T 			email_info;  
//     PHONEBOOK_USER_INFO_T 			sne_info;  
//     PHONEBOOK_ADDITIONAL_ENTRY_T  	user_info;  
//     BOOLEAN							anr_flag[MN_MAX_USIM_ANR_NUM];
//     BOOLEAN							email_flag;
//     BOOLEAN							sne_flag;  
//     BOOLEAN							adn_flag;
//     uint8                                      adn_rec_id;
// }PHONEBOOK_UPDATE_USER_INFO_ENTRY_T;
// 
// typedef struct PHONEBOOK_DEL_USER_INFO_ENTRY_T
// {  
//     PHONEBOOK_USER_INFO_T	anr_info[MN_MAX_USIM_ANR_NUM];
//     PHONEBOOK_USER_INFO_T 	adn_info;
//     PHONEBOOK_USER_INFO_T 	sne_info;  
//     PHONEBOOK_USER_INFO_T 	email_info;
//     PHONEBOOK_USER_INFO_T	iap_info;
//     BOOLEAN      			 	iap_flag;
// 
// }PHONEBOOK_DEL_USER_INFO_ENTRY_T;
// /* -vcard dev binli */
// 
// 
// 
// 
// typedef enum MN_PHONEBOOK_OPERATE_TYPE_E
// {
//     OPERATE_SUCCESS,
//     OPERATE_FILE_NOT_EXIST,
//     OPERATE_FILE_SIM_FAIL,
//     OPERATE_FILE_RECORDS_FULL
// }MN_PHONEBOOK_OPERATE_TYPE_E;
// 
// typedef struct APP_MN_UPDATE_USER_INFO_RESULT_T
// {
//     MN_PHONEBOOK_OPERATE_TYPE_E anr_result[MN_USIM_AAS_MAX_NUM];
//     MN_PHONEBOOK_OPERATE_TYPE_E adn_result;
//     MN_PHONEBOOK_OPERATE_TYPE_E email_result;
//     MN_PHONEBOOK_OPERATE_TYPE_E sne_result;  
//     BOOLEAN                 anr_flag[MN_USIM_AAS_MAX_NUM];
//     BOOLEAN                 adn_flag;
//     BOOLEAN                 email_flag;
//     BOOLEAN                 sne_flag;  
//     uint16                     adn_entry_id;
// } APP_MN_UPDATE_USER_INFO_RESULT_T;
// 
// 
// typedef struct APP_MN_UPDATE_USER_INFO_CNF_T
// {
//     SIGNAL_VARS
//     APP_MN_UPDATE_USER_INFO_RESULT_T update_result;
//     uint16                     adn_entry_id;	
//     MN_DUAL_SYS_E               dual_sys;
// } APP_MN_UPDATE_USER_INFO_CNF_T;
// 
// typedef struct APP_MN_DEL_USER_INFO_CNF_T
// {
//     SIGNAL_VARS
//     APP_MN_UPDATE_USER_INFO_RESULT_T update_result;
//     uint16                     adn_entry_id;	
//     MN_DUAL_SYS_E               dual_sys;
// } APP_MN_DEL_USER_INFO_CNF_T;
// 
// typedef struct APP_MN_READ_ADDITIONAL_ENTRY_T
// {
//     PHONEBOOK_ANR_NUMBER_T   	    anr_number[MN_MAX_USIM_ANR_NUM];
//     BOOLEAN                                     anr_ret[MN_MAX_USIM_ANR_NUM];
//     BOOLEAN                                     email_ret;
//     BOOLEAN                                     sne_ret;  
//     MN_USIM_EMAIL_STRING_T              email_addr;
//     PHONEBOOK_ALPHA_STRING_T	    sne_alpha; 
//     uint16                                          adn_entry_id;	
// } APP_MN_READ_ADDITIONAL_ENTRY_T;
// 
// 
// typedef struct APP_MN_READ_ADDITIONAL_ENTRY_CNF_T
// {
//     SIGNAL_VARS
//     APP_MN_READ_ADDITIONAL_ENTRY_T add_info;
//     uint16                     adn_entry_id;	
//     MN_DUAL_SYS_E               dual_sys;
// } APP_MN_READ_ADDITIONAL_ENTRY_CNF_T;
/*****************************************************************************/
// 	Description : get phonebook specify entry 
//	Global resource dependence : none
//  Author:       fancier.fan
//  Modify:       Tracy Zhang
//	Note:         
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_ReadUSIMPhonebookEntry(   // false if no entry about the id
    MN_DUAL_SYS_E       dual_sys,       // IN
    PHONEBOOK_STORAGE_E storage,    // IN:
    PHONEBOOK_ENTRY_T   *entry_ptr  // IN/OUT:entry data
);
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
PUBLIC void MMIPB_InterfeceTest(void);


#ifdef   __cplusplus
    }
#endif

#endif