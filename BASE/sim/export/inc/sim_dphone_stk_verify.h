/******************************************************************************
 ** File Name:      simat_main.h                                              *
 ** Author:         Raislin.Kong                                              *
 ** Date:           2002.10                                                   *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header have the decaleration of the global functions *
 **					that defined in SIM Application module and only be call   *
 ** 				in SIM Application Toolkit module or SIM module	      	  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2002.10        Raislin.Kong      create                                   *
 ******************************************************************************/
#ifndef _SIM_DPHONE_STK_VERIFY_H_
#define _SIM_DPHONE_STK_VERIFY_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

#define DPHONE_DES_KEY_MASK 0x07

#define DPHONE_DES_KEY_ARRAY_NUM 5
#define DPHONE_DES_ICCID_NUM 8
#define DPHONE_DES_RAND_NUM 20
#define DPHONE_DES_KEY_NUM 16
#define DPHONE_DES_CRYP_NUM 16
#define DPHONE_IND_PUK2_TEXT_LEN 20
#define DPHONE_CRYP_ERROR_TEXT_LEN 42

//for datang v2 jiangsu card
#define DPHONE_DATANG_V2_RAND_NUM 20
#define DPHONE_DATANG_V2_RAND1_NUM 4
#define DPHONE_DATANG_V2_RAND2_NUM 16

//for datang v21 liaoning card
#define DPHONE_DATANG_V21_RAND_NUM 8

//for datang v3 jiangsu card
#define DPHONE_DATANG_V3_RAND_NUM 20
#define DPHONE_DATANG_V3_RAND1_NUM 4
#define DPHONE_DATANG_V3_RAND2_NUM 16

//for woqi card
#define DPHONE_WOQI_DEFAULT_TEXT_LENGTH 28
#define DPHONE_WOQI_RAND_TAG_NUM 4
#define DPHONE_WOQI_DES_RAND_NUM 8
#define DPHONE_WOQI_DATA_TAG_NUM 4

#define DPHONE_SIM_ICCID_NUM_LEN 10

typedef enum
{
	NORMAL_CARD,
	DATANG_V2_JIANGSU_CARD,
	DATANG_V21_LIAONING_CARD,
	DATANG_V3_JIANGSU_CARD,
	WOQI_CARD,
	MAX_CARD_NUM
} DPHONE_DES_SIMCARD_TYPE_E;

STATIC DPHONE_DES_SIMCARD_TYPE_E    s_des_simcard_type = NORMAL_CARD;
	
STATIC BOOLEAN s_is_received_stk_verify_req = FALSE;
STATIC BOOLEAN s_is_stk_verify_ok = FALSE;
STATIC BOOLEAN s_is_tr9000 = FALSE;
STATIC BOOLEAN s_is_run_gsm_failed = FALSE;
STATIC uint8 s_sim_iccid_array[DPHONE_SIM_ICCID_NUM_LEN];
STATIC uint32 s_get_input_count = 0;

STATIC uint32 g_pc_count = 0;
STATIC BOOLEAN g_is_des_card_flag = FALSE;
STATIC BOOLEAN g_imsi_is_ff = FALSE;

typedef struct
{
	DPHONE_DES_SIMCARD_TYPE_E card_type;
	uint8 rand[DPHONE_DES_RAND_NUM];
	uint8 key[DPHONE_DES_KEY_NUM]; //16 byte key
	uint8 iccid[DPHONE_DES_ICCID_NUM];
} DPHONE_DES_PARAMETER_T;


typedef struct
{
    uint8 cryp_len;
	uint8 cryp_array[DPHONE_DES_CRYP_NUM];
} DPHONE_DES_RESULT_T;


extern DPHONE_DES_SIMCARD_TYPE_E MMI_GetDesSimCardType(void);

/********************************************************************
//    DESCRIPTION
//      init the get input command count to zero
//    after the temianl profile
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMAT_InitGetInputCount(void);

/********************************************************************
//    DESCRIPTION
//     get the get input command count
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint32 SIMAT_GetGetInputCount(void);

/********************************************************************
//    DESCRIPTION
//     get the get input command count
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMAT_IncreaseGetInputCount(void);

/********************************************************************
//    DESCRIPTION
//      Set whether ME has received the get input command which require the ME pass the
//    cryptograph to the SIM card as the response
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMAT_SetSTKVerifyReqFlag(BOOLEAN flag);

/********************************************************************
//    DESCRIPTION
//    read the flag to judge whether the ME has received the encryption
//    request from the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMAT_GetSTKVerifyReqFlag(void);
/********************************************************************
//    DESCRIPTION
//      Set whether ME has received the get input command which require the ME pass the
//    cryptograph to the SIM card as the response
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_SetSTKVerifyResult(BOOLEAN result);

/********************************************************************
//    DESCRIPTION
//    read the flag to judge whether the ME has received the encryption
//    request from the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIM_GetSTKVerifyResult(void);
/********************************************************************
//    DESCRIPTION
//      Set whether ME run gsm failed
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMAT_SetRunGsmFailed(BOOLEAN flag);
/********************************************************************
//    DESCRIPTION
//    read the flag to judge whether the ME has run gsm failed
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMAT_GetRunGsmFailed(void);
/********************************************************************
//    DESCRIPTION
//    set the terminal response result is 0x90 0x00
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMINSTR_SetTR9000(BOOLEAN flag);

/********************************************************************
//    DESCRIPTION
//    get the terminal response result to judge whether it is  0x90 0x00
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMINSTR_GetTR9000(void);

/********************************************************************
//    DESCRIPTION
//    read the type of the des sim card from mmi
//    Global resource dependence :
//    Author:yuhua.shi
//    Note:
***********************************************************************/
void SIM_DesSimCardTypeInit(void);

/********************************************************************
//    DESCRIPTION
//    read the type of the des sim card
//    Global resource dependence :
//    Author:yuhua.shi
//    Note:
***********************************************************************/
DPHONE_DES_SIMCARD_TYPE_E SIM_GetDesSimCardType(void);
 
/********************************************************************
//    DESCRIPTION
//    set the flag to judge whether it is a des card
//    Global resource dependence :
//    Author:yuhua.shi
//    Note:
***********************************************************************/
void SIMAT_SetDesCardFlag(BOOLEAN flag);

                         
/********************************************************************
//    DESCRIPTION
//    get the flag to judge whether it is a des card
//    Global resource dependence :
//    Author:yuhua.shi
//    Note:
***********************************************************************/
BOOLEAN SIMAT_GetDesCardFlag(void);
 
/********************************************************************
//    DESCRIPTION
//    init the proactive command count to zero
//    Global resource dependence :
//    Author:yuhua.shi
//    Note:
***********************************************************************/
void SIMAT_InitPCCount(void);

/********************************************************************
//    DESCRIPTION
//    set the count of proactive command which received
//    Global resource dependence :
//    Author:yuhua.shi
//    Note:
***********************************************************************/
void SIMAT_IncreasePCCount(void);
                    
/********************************************************************
//    DESCRIPTION
//    read the count of proactive command which received
//    Global resource dependence :
//    Author:yuhua.shi
//    Note:
***********************************************************************/
uint32 SIMAT_GetPCCount(void);

/********************************************************************
//    DESCRIPTION
//    set the flag to judge whether the imsi is full 0xff
//    Global resource dependence :
//    Author:yuhua.shi
//    Note:
***********************************************************************/
void SIM_SetImsiIsFfFlag(BOOLEAN flag);

                         
/********************************************************************
//    DESCRIPTION
//    get the flag to judge whether the imsi is full 0xff
//    Global resource dependence :
//    Author:yuhua.shi
//    Note:
***********************************************************************/
BOOLEAN SIM_GetImsiIsFfFlag(void);
    
/********************************************************************
//    DESCRIPTION
//    do the initialization for des sim card
//    Global resource dependence :
//    Author:yuhua.shi
//    Note:
***********************************************************************/
void SIM_DesSimCardVariableInit(void);
                                
      
#ifdef __cplusplus
    }
#endif
#endif  //_SIM_DPHONE_STK_VERIFY_H_