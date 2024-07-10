/******************************************************************************
 ** File Name:      sim_dilling_number.h                                      *
 ** Author:         Raislin.Kong                                              *
 ** Date:           2002.2                                                    *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the header file that define the signals realated  *
 **					with dialling number EF files  				        	  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2002.2         Raislin.Kong      create                                   *
 ******************************************************************************/

#ifndef	_SIM_DIALLING_NUMBER_H_
#define	_SIM_DIALLING_NUMBER_H_


#ifdef   __cplusplus
    extern   "C" 
    {
#endif
#include "mn_type.h"   //need by #define SIM_MAX_DN_NUMBER_LEN				    MN_MAX_ADDR_BCD_LEN

//the dialling num type
typedef enum
{
    SIM_DN_T_ADN,
    SIM_DN_T_FDN,
    SIM_DN_T_MSISDN,
    SIM_DN_T_LND,
    SIM_DN_T_SDN,
    SIM_DN_T_BDN,
    SIM_DN_T_NUM
}SIM_DIALLING_NUM_TYPE_E;


typedef enum
{
    SIM_DN_OK,
    SIM_DN_ITEM_EMPY,
    SIM_DN_SERVICE_NOT_ENABLE,      //the dialling number service not enabled
    SIM_DN_SERVICE_TYPE_ERROR,
    SIM_DN_SEEK_EXT_ERROR,
    SIM_DN_EXT_MEM_FULL,          
    SIM_DN_SELECT_FILE_ERROR,
    SIM_DN_READ_FILE_ERROR,
    SIM_DN_UPDATE_FILE_ERROR,
    SIM_DN_INVALIDATE_FILE_ERROR,
    SIM_DN_REHABILITATE_FILE_ERROR
}SIM_DN_OPERATION_RESULT_E;

typedef enum
{
	USIM_PBR_TYPE_1=0xA8, /*EFadn, anr, email, grp, iap, pbc, sne, uid*/
	USIM_PBR_TYPE_2=0xA9, /*EFanr, email, sne*/  	
	USIM_PBR_TYPE_3=0xAA  /*EFaas, ext1, gas, ccp1*/ 
}SIM_USIM_PBR_TYPE_E;

typedef enum
{
	USIM_PBR_EF_ADN  = 0xC0, 
	USIM_PBR_EF_IAP   = 0xC1,
	USIM_PBR_EF_EXT1 = 0xC2,
	USIM_PBR_EF_SNE   = 0xC3,
	USIM_PBR_EF_ANR   = 0xC4,
	USIM_PBR_EF_PBC   = 0xC5,
	USIM_PBR_EF_GRP   = 0xC6,
	USIM_PBR_EF_AAS   = 0xC7,
	USIM_PBR_EF_GAS   = 0xC8,   
	USIM_PBR_EF_UID    = 0xC9,   
	USIM_PBR_EF_EMAIL= 0xCA,      
	USIM_PBR_EF_CCP1  = 0xCB,
	USIM_PBR_EF_MAX_SORT = 12 
}SIM_USIM_PBR_EF_TAG_E;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_DIALLING_NUM_TYPE_E dn_type;    //the dialling num type
}SIM_SIG_GET_FREE_EXT_INFO_REQ_T;


typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_DIALLING_NUM_TYPE_E dn_type;    //the dialling num type
    SIM_DN_OPERATION_RESULT_E    result;     //the operation result
    uint8 free_ext_record_num;
}SIM_SIG_GET_FREE_EXT_INFO_CNF_T;

//the dialling number update signal struct
typedef struct
{
    SIGNAL_VARS
	uint32	command_ref; 
    uint16   dn_id;         //the identifier of the dialling num to be update
    SIM_DIALLING_NUM_TYPE_E dn_type;    //the dialling num type
    SIM_DIALLING_NUMBER_T dn;   //the dialing number that update to SIM card
}SIM_SIG_UPDATE_DN_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    uint16   dn_id;         //the identifier of the dialling num to be update
    SIM_DIALLING_NUM_TYPE_E dn_type;    //the dialling num type
    SIM_DN_OPERATION_RESULT_E result;  //the update operatin result
    uint8 alpha_id_len;//To new PB record's alpha id len
    uint8 alpha_id[SIM_MAX_DN_ALPHA_IDENTIFY_LEN];//To notify the upper-layer PB name
    int8 increased_ext_num;//increased/decreased ext_records for this dn_id
}SIM_SIG_UPDATE_DN_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    uint16   dn_id;         //the identifier of the dialling num to be read
    SIM_DIALLING_NUM_TYPE_E dn_type;    //the dialling num type
}SIM_SIG_READ_DN_REQ_T;


typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    uint16   dn_id;         //the identifier of the dialling num to be read
    SIM_DIALLING_NUM_TYPE_E dn_type;    //the dialling num type
    SIM_DN_OPERATION_RESULT_E    result;     //the operation result
    SIM_DIALLING_NUMBER_T   dn; //the dialling number that been read out
    uint8 current_record_ext_num;//current dn_id has x ext_records
}SIM_SIG_READ_DN_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    uint16   dn_id;         //the identifier of the dialling num to be erasure
    SIM_DIALLING_NUM_TYPE_E dn_type;    //the dialling num type
}SIM_SIG_ERASE_DN_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    uint16   dn_id;         //the identifier of the dialling num to be erasure
    SIM_DIALLING_NUM_TYPE_E dn_type;    //the dialling num type
    SIM_DN_OPERATION_RESULT_E result; //the result of the operation
    uint8 decreased_ext_num;          //the decreased ext num for this dn_id
}SIM_SIG_ERASE_DN_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
}SIM_SIG_ERASE_LND_ALL_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_DN_OPERATION_RESULT_E result; //the result of the operation
    uint8 decreased_ext_num;          //the decreased ext num for this dn_id
}SIM_SIG_ERASE_LND_ALL_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_DIALLING_NUM_TYPE_E dn_type;    //the dialling num type
}SIM_SIG_PURGE_DN_REQ_T;


typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_DIALLING_NUM_TYPE_E dn_type;    //the dialling num type
    SIM_DN_OPERATION_RESULT_E result;      //the operation result
}SIM_SIG_PURGE_DN_CNF_T;


//the option type to the FDN and BDN service
typedef enum
{
    SIM_DN_CAPABILITY,
    SIM_DN_ENABLING,
    SIM_DN_DISALBING
}SIM_DN_SERVICE_OPERATION_TYPE_E;


typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_DN_SERVICE_OPERATION_TYPE_E service_type;   //the service operation type
}SIM_SIG_FDN_SERVICE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_DN_SERVICE_OPERATION_TYPE_E service_type;   //the service operation type
    SIM_DN_OPERATION_RESULT_E result;    //the result of the FDN service
    BOOLEAN is_enable;                  //is the FDN enabled or not     
    BOOLEAN is_fdn_exist;//Is the FDN exist in the (u)sim card?
}SIM_SIG_FDN_SERVICE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_DN_SERVICE_OPERATION_TYPE_E service_type;   //the service operation type
}SIM_SIG_BDN_SERVICE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    SIM_DN_SERVICE_OPERATION_TYPE_E service_type;   //the service operation type
    SIM_DN_OPERATION_RESULT_E result;    //the result of the FDN service
    BOOLEAN is_enable;                  //is the BDN enabled or not
}SIM_SIG_BDN_SERVICE_CNF_T;


typedef enum
{
    SIM_CCP_OK,
    SIM_CCP_SERVICE_NOT_ENABLE,
    SIM_CCP_UPDATE_ERROR,
    SIM_CCP_READ_ERROR
}SIM_CCP_OPERATION_RESULT_E;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    uint8   ccp_id;         //the ccp reacord id to be readed
}SIM_SIG_READ_CCP_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    uint8   ccp_id;         //the ccp reacord id to be readed
    SIM_CCP_OPERATION_RESULT_E result;    //the result of the operation
    SIM_EF_MAPPING_CCP_T    ccp;    //the ccp that has been read out
}SIM_SIG_READ_CCP_CNF_T;



typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    uint8   ccp_id;         //the ccp reacord id to be readed
    SIM_EF_MAPPING_CCP_T    ccp;    //the ccp that used to update
}SIM_SIG_UPDATE_CCP_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    uint8   ccp_id;         //the ccp reacord id to be update
    SIM_CCP_OPERATION_RESULT_E result;    //the result of the operation
}SIM_SIG_UPDATE_CCP_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    uint8   ccp_id;         //the ccp reacord id to be erasure
}SIM_SIG_ERASE_CCP_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint32  command_ref;
    uint8   ccp_id;         //the ccp reacord id to be erasure
    SIM_CCP_OPERATION_RESULT_E result;    //the result of the operation
}SIM_SIG_ERASE_CCP_CNF_T;

#ifdef   __cplusplus
    }
#endif

#endif	//_SIM_DIALLING_NUMBER_H_
