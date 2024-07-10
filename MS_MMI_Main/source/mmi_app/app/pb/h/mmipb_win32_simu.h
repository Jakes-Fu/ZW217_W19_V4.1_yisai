/*******************************************************************************
 ** FileName:      mmipb_win32_simu.c                                          *
 ** Author:        baokun.yin                                                  *
 ** Date:          2009/04/01                                                  *
 ** CopyRight:     2009, Spreadtrum, Incoporated, All right reserved            *
 ** Description:   define BT simu interface                                    *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2008/04/01     baokun.yin             Created                              *
 *******************************************************************************/
#include "mn_type.h"
#ifndef MMIPB_WIN32_SIMU_H
#define MMIPB_WIN32_SIMU_H


#ifdef CMCC_VCARD_SUPPORT
#define MNPHONE_NO_ERR_SPACE       0
#define MNPHONE_NO_ANR1_SPACE      1
#define MNPHONE_NO_ANR2_SPACE     	 2
#define MNPHONE_NO_ANR3_SPACE     	 4
#define MNPHONE_NO_ADN_SPACE       8
#define MNPHONE_NO_EMAIL_SPACE     16

#define MN_USIM_AAS_MAX_NUM                    3
//#define PHONEBOOK_MAX_DN_NUMBER_LEN	      20
#define MN_USIM_EMAIL_STRING_MAX_LEN           255
//#define PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN    241

typedef enum 
{
    OPERATE_SUCCESS,
    OPERATE_FILE_NOT_EXIST,
    OPERATE_FILE_SIM_FAIL,
    OPERATE_FILE_RECORDS_FULL
}MN_PHONEBOOK_OPERATE_TYPE_E;


typedef struct PHONEBOOK_ANR_NUMBER_T
{
    MNBCD               number[PHONEBOOK_MAX_DN_NUMBER_LEN];
    uint8               aas_id; //EFaasÖÐµÄ¼ÇÂ¼ºÅ
    uint8               number_len;
    MN_NUMBER_TYPE_E    ton;
    MN_NUMBER_PLAN_E    npi;
    BOOLEAN   			adn_id_exist; /*if byte 16 and 17 exist*/
    uint8     adn_sfi; /*byte 16, sfi of ADN*/
    uint8     id_in_adn; /*byte 17, corespond of rec. id in adn*/
} PHONEBOOK_ANR_NUMBER_T;

typedef struct _MN_USIM_EMAIL_STRING_T
{
    uint8   address_len;
    uint8   address[MN_USIM_EMAIL_STRING_MAX_LEN];	
}MN_USIM_EMAIL_STRING_T;

typedef struct _PHONEBOOK_ADDITIONAL_ENTRY_T
{
    PHONEBOOK_ANR_NUMBER_T   	anr_number[MN_USIM_AAS_MAX_NUM];
    MN_USIM_EMAIL_STRING_T             		email_addr;
	uint16                     entry_id;	
}PHONEBOOK_ADDITIONAL_ENTRY_T;

typedef struct _PHONEBOOK_AAS_ENTRY_T
{
    uint8			alpha_len;
    uint8   			alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN];
} PHONEBOOK_AAS_ENTRY_T;

typedef struct _APP_MN_READ_ADDITIONAL_ENTRY_T
{
    uint16                     	adn_entry_id;	
    BOOLEAN 				anr_ret[MN_USIM_AAS_MAX_NUM];
    PHONEBOOK_ANR_NUMBER_T   	anr_number[MN_USIM_AAS_MAX_NUM];
    BOOLEAN 				email_ret;
    MN_USIM_EMAIL_STRING_T        email_addr;
} APP_MN_READ_ADDITIONAL_ENTRY_T;

typedef struct _APP_MN_READ_ADDITIONAL_ENTRY_CNF_T
{
    SIGNAL_VARS
    APP_MN_READ_ADDITIONAL_ENTRY_T 	add_info;
	    uint16                     		adn_entry_id;	

} APP_MN_READ_ADDITIONAL_ENTRY_CNF_T;

typedef struct _APP_MN_UPDATE_USER_INFO_RESULT_T
{
    uint16                     	adn_entry_id;	
    BOOLEAN 				anr_flag[MN_USIM_AAS_MAX_NUM];
    BOOLEAN 				adn_flag;
    BOOLEAN 				email_flag;
    MN_PHONEBOOK_OPERATE_TYPE_E 	anr_result[MN_USIM_AAS_MAX_NUM];
    MN_PHONEBOOK_OPERATE_TYPE_E 	adn_result;
    MN_PHONEBOOK_OPERATE_TYPE_E 	email_result;
} APP_MN_UPDATE_USER_INFO_RESULT_T;

typedef struct _APP_MN_DEL_USER_INFO_CNF_T
{
    SIGNAL_VARS
	
    APP_MN_UPDATE_USER_INFO_RESULT_T 	update_result;
	    uint16                     		adn_entry_id;
} APP_MN_DEL_USER_INFO_CNF_T;



typedef struct _APP_MN_UPDATE_USER_INFO_CNF_T
{
    SIGNAL_VARS
    APP_MN_UPDATE_USER_INFO_RESULT_T 	update_result;
    uint16                     		adn_entry_id;	

 } APP_MN_UPDATE_USER_INFO_CNF_T;
uint8 PHONEBOOK_ReadUsimAas(
			uint8 			        rec_num,      //in/out
			PHONEBOOK_AAS_ENTRY_T	*aas_entry_ptr //in/out
			);

uint8 PHONEBOOK_Update3GPhonebook (
		    PHONEBOOK_ENTRY_T       			       *entry_ptr,  //in
		    BOOLEAN							anr_flag[MN_USIM_AAS_MAX_NUM],	 //in
		    BOOLEAN							email_flag, //in
		    BOOLEAN							adn_flag, //in
		    PHONEBOOK_ADDITIONAL_ENTRY_T	                    *additional_entry_ptr);


ERR_MNDATAMAG_CODE_E PHONEBOOK_Delete3GPhonebook (uint32  entry_id  /*in*/);

ERR_MNDATAMAG_CODE_E PHONEBOOK_ReadUsimAdditionalEntry (uint32 entry_id  /*in*/);

void MNSIM_SetEventAddInfoReadCnf(
   	 APP_MN_READ_ADDITIONAL_ENTRY_T       *add_info_ptr
	);

void MNSIM_SetEventUpdateusersInfoCnf(
    	APP_MN_UPDATE_USER_INFO_RESULT_T      *update_result_ptr
	);

void MNSIM_SetEventDelUserInfoCnf(
	APP_MN_UPDATE_USER_INFO_RESULT_T      *update_result_ptr
	);

PUBLIC BOOLEAN MNSIM_Is_Usim(MN_DUAL_SYS_E dual_sys);

#endif
#endif