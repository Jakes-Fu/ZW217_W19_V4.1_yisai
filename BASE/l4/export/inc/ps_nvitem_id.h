/******************************************************************************
 ** File Name:      ps_nvitem_id.h                                            *
 ** Author:         Tracy Zhang                                               *
 ** Date:           09/18/2002                                                *
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
 ** 09/18/2002    Tracy Zhang      Create.                                    *
 ******************************************************************************/
 
 
#ifndef _PS_NVITEM_ID_H_
#define _PS_NVITEM_ID_H_

#include "nv_item_id.h"

// The stored equivalent PLMN list shall not be deleted when the MS is switched off. 
#define PS_NV_EPLMN_LIST    NV_PS_BASE          //470
#define PS_NV_EPLMN_LIST_1 (NV_PS_BASE + 1)     //471
#define PS_NV_EPLMN_LIST_2 (NV_PS_BASE + 2)     //472

#define NV_SMS_TP_MR       (NV_PS_BASE + 3)     //473
#define NV_SMS_TP_MR1      (NV_PS_BASE + 4)     //474
#define NV_SMS_TP_MR2      (NV_PS_BASE + 5)     //475

#define NV_SIM_IMSI_2      (NV_PS_BASE + 6)     //476
#define NV_SIM_GPRS_KC_2   (NV_PS_BASE + 7)     //477
#define NV_SIM_GPRS_LOCI_2 (NV_PS_BASE + 8)     //478

#define PS_NV_EPLMN_LIST_3 (NV_PS_BASE + 9)     //479
#define NV_SMS_TP_MR3      (NV_PS_BASE + 10)    //480
#define NV_SIM_IMSI_3      (NV_PS_BASE + 11)    //481
#define NV_SIM_GPRS_KC_3   (NV_PS_BASE + 12)    //482
#define NV_SIM_GPRS_LOCI_3 (NV_PS_BASE + 13)    //483
#define NV_IMEI3           (NV_PS_BASE + 14)    //484
#define NV_2BIS_BA3                      (NV_PS_BASE + 15)        //485
#define NV_VOICE_MAIL_ADDR_SYS4          (NV_PS_BASE + 16)        //486
#define NV_SIM_CFG                  (NV_PS_BASE + 17)        //487 //should equal to 487, nv_type used 
// defined in nv_item_id.h
// NV_LOGSAVE_IQ_SWITCH              = NV_L1_EXT_BASE_ID-2,       //488 //should equal to 488, nv_type used 
// NV_LOGSAVE_SETTING                = NV_L1_EXT_BASE_ID-1,       //489 //should equal to 489, nv_type used 
#define NV_VOICE_MAIL_ENABLE_FLAG3       (NV_L1_EXT_BASE_ID)      //490 //LAYER1 SPACE
#define NV_PLMN_SELECT_MODE3             (NV_L1_EXT_BASE_ID + 1)  //491 //LAYER1 SPACE

#define NV_PHONE_CONTROL_PLMN            (NV_L1_EXT_BASE_ID + 2)  //492 //LAYER1 SPACE
#define NV_PHONE_CONTROL_PLMN_1          (NV_L1_EXT_BASE_ID + 3)  //493 //LAYER1 SPACE
#define NV_PHONE_CONTROL_PLMN_2          (NV_L1_EXT_BASE_ID + 4)  //494 //LAYER1 SPACE
#define NV_PHONE_CONTROL_PLMN_3          (NV_L1_EXT_BASE_ID + 5)  //495 //LAYER1 SPACE

#define NV_SIM_LOCK_CUSTOMIZE_DATA_ID    (NV_L1_EXT_BASE_ID + 6)  //496 //LAYER1 SPACE
#define NV_SIM_LOCK_USER_DATA_ID         (NV_L1_EXT_BASE_ID + 7)  //497 //LAYER1 SPACE
#define NV_SIM_LOCK_CONTROL_KEY_ID      (NV_L1_EXT_BASE_ID + 8)  //498 //LAYER1 SPACE 

typedef struct
{
	uint8	multips_create_set;
	uint32	sim_slot_map;
	uint32	reserve0;
	uint32	reserve1;
	uint32	reserve2;
	uint32	reserve3;   //the bit0 means whether the AP side check FDN,          0 -- FALSE, 1 -- TRUE
                        //the bit1 means whether the SCA num needs to check FDN, 0 -- TRUE, 1 -- FALSE
}MULTIPS_CREATE_PARAM_T;

#endif
