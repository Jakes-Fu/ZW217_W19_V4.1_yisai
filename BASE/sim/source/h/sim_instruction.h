/******************************************************************************
** File Name:      sim_instruction.h                                         *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the header file about the sim card instruciton    *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/

#ifndef _SIM_INSTRUCTION_H_
#define _SIM_INSTRUCTION_H_
#ifdef   __cplusplus
extern   "C"
{
#endif

#include "sim_hal.h"          //need by TB_SIM_APDU_T

#ifdef WIN32
#include "sim_to_simat.h"
#include "sim_file_system.h"
#include "sim_signal.h"
#endif

//get the EF file access condtion for the response data of SELECT
//instuction in the EF file
/*@zhangyongxia,CR4436,2003-9-2*/
/*#define GET_FILE_ACCESS_CONDITION(file_access,select_response)    file_access.update_access = (SIM_ACCESS_CONDITION_E)(select_response->access_condition1 & 0x000f);\
                                                                file_access.seek_read_access = (SIM_ACCESS_CONDITION_E)((select_response->access_condition1 & 0x00f0) >> 4);\
                                                                file_access.increase_access = (SIM_ACCESS_CONDITION_E)((select_response->access_condition1 & 0xf000) >> 12);\
                                                                file_access.invalidate_access = (SIM_ACCESS_CONDITION_E)(select_response->access_condition2 & 0x0f);\
                                                                file_access.rehabilitate_access = (SIM_ACCESS_CONDITION_E)((select_response->access_condition2 & 0xf0) >> 4);*/
#define GET_FILE_ACCESS_CONDITION(file_access,select_response)  file_access.update_access = (SIM_ACCESS_CONDITION_E)((select_response->access_condition1 & 0x0f00) >>8);\
file_access.seek_read_access = (SIM_ACCESS_CONDITION_E)((select_response->access_condition1 & 0xf000) >>12);\
file_access.increase_access = (SIM_ACCESS_CONDITION_E)((select_response->access_condition1 & 0x00f0) >> 4);\
file_access.invalidate_access = (SIM_ACCESS_CONDITION_E)(select_response->access_condition2 & 0x0f);\
file_access.rehabilitate_access = (SIM_ACCESS_CONDITION_E)((select_response->access_condition2 & 0xf0) >> 4);

//the instruction data buf use to buf the data send with the instruction
#define SIMINSTR_DATA_BUF_T_LEN         300

//the max instruction num in a instruction list
#define SIMINSTR_LIST_T_MAX_INSTR_NUM   6

//the file invalidate mark in the select data
#define SIM_FILE_INVALIDATE_MARK        0x01
//the mark show whether the file can be read or update when
//the file is invalidate
#define SIM_FILE_INVALIDATE_RU_MARK     0x04
//the cyclic file increase allowed mark
#define SIM_FILE_INCREASE_ALLOWED_MARK  0x80
//the mandatory select data of the DF file length
#define SIM_MAN_DF_SELECT_DATA_LEN      22
//the mandatory select data of the EF file length
#define SIM_MAN_EF_SELECT_DATA_LEN      14
//the length of the repsonse data from the SIM card when seek command succeed
#define SIM_SEEK_RESPONSE_DATA_LEN      1
//the least length of the response data from the sim card when increase command succeed
#define SIM_INCREASE_RESPONSE_DATA_LEN  4
//the length of the response data from the SIM card when Run GSM Algorithm command succeeded
#define SIM_RUN_GSM_ALGORITHM_RESPONSE_DATA_LEN     12

//the start posiiton of the special information in the select data
#define SIM_SELECT_DATA_RFU_POS         0
#define SIM_SELECT_DATA_FS_POS          2
#define SIM_SELECT_DATA_FILE_ID_POS     4
#define SIM_SELECT_DATA_FILE_TYPE_POS     6
#define SIM_SELECT_DATA_CYC_INCREASE    7
#define SIM_SELECT_DATA_ACC_COND1       8
#define SIM_SELECT_DATA_FILE_STATUS_POS 11
#define SIM_SELECT_DATA_REMAIN_LEN_POS 12
#define SIM_SELECT_DATA_FILECHA_EFSTR_POS 13
#define SIM_SELECT_DATA_DFNUM_RECLEN_POS 14
#define SIM_SELECT_DATA_EFNUM_POS 15
#define SIM_SELECT_DATA_CHVNUM_POS 16
#define SIM_SELECT_DATA_RFU2_POS 17
#define SIM_SELECT_DATA_CHV1_POS 18
#define SIM_SELECT_DATA_PUK1_POS 19
#define SIM_SELECT_DATA_CHV2_POS 20
#define SIM_SELECT_DATA_PUK2_POS 21

//the continuously CHV and unblock CHV error attempt
#define SIM_MAX_CHV_ATTEMPT             3// A tigo sim is 5 times.
#define SIM_MAX_UNBLOCK_CHV_ATTEMPT     10


//the sim access condtions of sim file
typedef struct
{
    SIM_ACCESS_CONDITION_E  update_access;
    SIM_ACCESS_CONDITION_E  seek_read_access;
    SIM_ACCESS_CONDITION_E  increase_access;
    SIM_ACCESS_CONDITION_E  invalidate_access;
    SIM_ACCESS_CONDITION_E  rehabilitate_access;
} SIM_FILE_ACCESS_CONDITION_T;


//the all instruction name,act as the instruction index in
//the instruciton info table
typedef enum
{
    SIMINSTR_SELECT,                //the SELECT instruction
    SIMINSTR_STATUS,                //the STATUS instruction
    SIMINSTR_READ_BINARY,           //the READ BINARY instruction
    SIMINSTR_UPDATE_BINARY,     //the UPDATE BINARY instruction
    SIMINSTR_READ_RECORD,           //the READ RECORD instruction
    SIMINSTR_UPDATE_RECORD,     //the UPDATE RECORD instruction
    SIMINSTR_SEEK,                  //the SEEK instruction
    SIMINSTR_INCREASE,              //the INCREASE instruction
    SIMINSTR_VERIFY_CHV,            //the VERIFY CHV instruction
    SIMINSTR_CHANGE_CHV,            //the CHANGE CHV instruction
    SIMINSTR_DISABLE_CHV,           //the DISABLE CHV instruction
    SIMINSTR_ENABLE_CHV,            //the ENABLE CHV instruction
    SIMINSTR_UNBLOCK_CHV,           //the UNBLOCK CHV instruction
    SIMINSTR_INVALIDATE,            //the INVALIDATE instruction
    SIMINSTR_REHABILITATE,          //the REHABILITATE instruction
    SIMINSTR_RUN_ALGORITHM,     //the RUN GSM ALGORITHM instruction
    SIMINSTR_SLEEP,             //the SLEEP instruction
    SIMINSTR_GET_RESPONSE,          //the GET RESPONSE instruction
    SIMINSTR_TERMINAL_PROFILE,      //the TEMINAL PROFILE instruction
    SIMINSTR_ENVELOPE,              //the ENVELOP instruction
    SIMINSTR_FETCH,             //the FETCH instruction
    SIMINSTR_TERMINAL_RESPONSE, //the TERMINAL RESPONSE instruction
    SIMINSTR_TOTAL_NUM              //the total number of sim instuction
} SIMINSTR_NAME_E;

typedef struct
{
    SIMINSTR_NAME_E instr_name;
    TB_SIM_APDU_T apdu;
} SIMINSTR_CODE_T;

typedef struct
{
    uint16 cur_pos;
    uint8 buf[SIMINSTR_DATA_BUF_T_LEN];
} SIMINSTR_DATA_BUF_T;

typedef SIMINSTR_CODE_T SIMINSTR_CODE_ARR[SIMINSTR_LIST_T_MAX_INSTR_NUM];
//the instruction code list that can inplment a request to the sim
typedef struct
{
    uint8 instr_num;    //the total instr in the list
    uint8 cur_instr;    //the instr that current process by the sim card
    SIMINSTR_CODE_ARR instr_list;
} SIMINSTR_LIST_T;



//the status word type in the  reponse of command,reference to
//SPEC GSM 11.11 Section 9.4
typedef enum simStatusWordTag
{
    SIM_STATUS_OK_NORMAL,           // 9000
    SIM_STATUS_OK_PROACTIVE_INFO,   // 91xx
    SIM_STATUS_OK_RESPONSE_DATA,    // 9fxx
    SIM_STATUS_OK_WITH_UPDATE,      // 920x
    SIM_STATUS_OK_RESEND_ADPU,         // 6cxx
    SIM_STATUS_MEMORY_PROBLEM,      // 9240
    SIM_STATUS_NO_EF_SELECETED,     // 9400
    SIM_STATUS_INVALIDE_ADDRESS,    // 9402
    SIM_STATUS_INVALIDE_FILE_ID,    // 9404
    SIM_STATUS_INCONSISTENT_FILE,   // 9408
    SIM_STATUS_CHV_UNINITIALIZED,   // 9802
    SIM_STATUS_ACCESS_FAIL,         // 9804
    SIM_STATUS_CHV_CONTRADICTION,   // 9808
    SIM_STATUS_INVALIDATION_CONTRADICTION,  //9810
    SIM_STATUS_CHV_VER_BLOCKED,     // 9840
    SIM_STATUS_MAX_RECORD_VALUE,    // 9850
    SIM_STATUS_AUTH_MAC_ERROR,  // 9862
    SIM_STATUS_SEC_ASSO_EXPIRED,      // 9863
    SIM_STATUS_NO_INFO_GIVEN_62,        // 6200
    SIM_STATUS_DATA_CORRUPT,        // 6281
    SIM_STATUS_END_OF_FILE,        // 6282
    SIM_STATUS_SEL_FILE_INVALIDE,        // 6283
    SIM_STATUS_SEL_FILE_IN_TERMINATION,       // 6285
    SIM_STATUS_MORE_DATA_AVAILABLE,       // 62F1
    SIM_STATUS_MORE_DATA_AVAILABLE_WITH_PC,       // 62F2
    SIM_STATUS_RESPONSE_DATA_AVAILABLE,       // 62F3
    SIM_STATUS_MORE_DATA_EXCEPTED,       // 63F1
    SIM_STATUS_MORE_DATA_EXCEPTED_WITH_PC,       // 63F2
    SIM_STATUS_SUCCESS_WITH_PROBLEM,        // 63Cx
    SIM_STATUS_NO_INFO_GIVEN_64,        // 6400
    SIM_STATUS_NO_INFO_GIVEN_65,        // 6500
    SIM_STATUS_MEMORY_PROBLEM_65,   // 6581
    SIM_STATUS_WRONG_LENGTH,            // 6700
    SIM_STATUS_P3_INCORRECT,            // 67xx
    SIM_STATUS_NO_INFO_GIVEN_68,        // 6800
    SIM_STATUS_LCHAN_NOT_SUPPORT,        // 6881
    SIM_STATUS_SECMSG_NOT_SUPPORT,        // 6882
    SIM_STATUS_NO_INFO_GIVEN_69,        // 6900
    SIM_STATUS_INCOM_WITH_STRUCTRUE,        // 6981
    SIM_STATUS_SECSTATUS_NOT_SATISFIED,        // 6982
    SIM_STATUS_AUTH_METHOD_BLOKCED,        // 6983
    SIM_STATUS_REF_DATA_INVALIDATE,        // 6984
    SIM_STATUS_USE_NOT_SATISFIED,        // 6985
    SIM_STATUS_CMD_NOT_ALLOWED,        // 6986
    SIM_STATUS_INCORRECT_PARA,        // 6a80
    SIM_STATUS_FUNC_NOT_SUPPORT,        // 6a81
    SIM_STATUS_FILE_NOT_FOUND,        // 6a82
    SIM_STATUS_REC_NOT_FOUND,        // 6a83
    SIM_STATUS_NOT_ENOUGH_MEM,        // 6a84
    SIM_STATUS_P1P2_INCORRECT,        // 6a86
    SIM_STATUS_P1P2_INCONSISTENT,        // 6a87
    SIM_STATUS_REF_DATA_NOT_FOUND,        // 6a88
    SIM_STATUS_P1_OR_P2_INCORRECT,  // 6bxx
    SIM_STATUS_UNKNOW_INSTRUCTION_CODE, // 6dxx
    SIM_STATUS_WRONG_INSTRUCTION_CLASS, // 6exx
    SIM_STATUS_TECHNICAL_PROBLEM,   // 6f00
    SIM_STATUS_UNKNOW_FAIL_REASON,  // 6fxx
    SIM_STATUS_OK_DOWN_DATA_ERR,    // 9exx
    SIM_STATUS_TOOLKIT_BUSY,        // 9300
    SIM_STATUS_WRONG_WORD,          //undefined status word, but still 9X or 6X
    SIM_STATUS_ERROR_DATA //other noise data
} SIMINSTR_RESULT_TYPE_E;



typedef struct
{
    uint16 file_id;     //the file id
    uint16 file_size;   //the file size
    SIM_FILE_TYPE_E file_type;  //the file type,it must be SIM_FILE_EF
    SIM_EF_STRUCTURE_E file_structure;  //the ef structure of the file
    SIM_FILE_ACCESS_CONDITION_T access_condition;   //the file access codition
    uint8 record_len;   //the record length,only valide when the file struture is
    //fixed linear or cyclic
    BOOLEAN invalidate; //the file's invalidate
    BOOLEAN invalidate_ru;  //whether this file can be read or update when the
    //file is invalide
    BOOLEAN increase_allowed;   //is increase allowed,only valide when the file
    //structure is a cyclic
} SIM_EF_FILE_INFO_T;


typedef struct
{
    uint16 file_id;     //the file id
    uint16 free_space;  //the free space of the DF or MF
    SIM_FILE_TYPE_E file_type;  //the file type,it must be SIM_FILE_DF or SIM_FILE_MF
    SIM_CLOCK_STOP_TYPE_E clock_stop;   //the clock stop mode
    BOOLEAN chv1_enable;        //chv1 enable or not
    uint8 df_child_num;
    uint8 ef_child_num;
    SIM_CHV_STATUS_T    chv1_status;
    SIM_CHV_STATUS_T    unblock_chv1_status;
    SIM_CHV_STATUS_T    chv2_status;
    SIM_CHV_STATUS_T    unblock_chv2_status;
} SIM_DF_MF_INFO_T;

//the sim seek type
typedef enum
{
    SIM_SEEK_TYPE_1 = 0x00,
    SIM_SEEK_TYPE_2 = 0x10
} SIM_SEEK_TYPE_E;

//the sim seek mode
typedef enum
{
    SIM_SEEK_BEGINNING_FORWARD  = 0x00,
    SIM_SEEK_ENDING_BACKWARD    = 0x01,
    SIM_SEEK_NEXT_FORWARD       = 0x02,
    SIM_SEEK_PREV_BACKWARD      = 0x03
} SIM_SEEK_MODE_E;

//the usim seek mode
typedef enum
{
    SIM_SEEK_FROM_RECORD_P1_FORWARD = 0x04,
    SIM_SEEK_FROM_RECORD_P1_BACKWARD    = 0x05,
    SIM_ENHANCED_SEEK_NEXT_FORWARD  = 0x06,
    SIM_ENHANCED_SEEK_PREV_BACKWARD = 0x07
} SIM_USIM_SEEK_MODE_E;

typedef struct
{
    SIM_INSTR_INCREASE_DATA_T new_value;
    SIM_INSTR_INCREASE_DATA_T added_value;
} SIM_INSTR_INCREASE_RESULT_T;

typedef struct SIM_RSP_INFO_T_s
{
    SIM_FILE_NAME_E         file_name;
    SIMINSTR_NAME_E         ins_name;
    SIM_SW_AND_RESPONSE_T   sw_rsp;
} SIM_RSP_INFO_T;


typedef enum
{
    SIM_SELECT_CMD_P1_PATH_FROM_MF = 0x08,
    SIM_SELECT_CMD_P1_PATH_FROM_CURRENT_DF = 0x09,
    SIM_SELECT_CMD_P1_BY_FID = 0x00,
    SIM_SELECT_CMD_P1_BY_DF_NAME = 0x04,
    SIM_SELECT_CMD_P1_PARENT_DF_CURR_DF = 0x03
} SIM_SELECT_CMD_P1_ENUM;

typedef enum
{
    SIM_SELECT_CMD_NO_PARA              = 0x00,
    SIM_SELECT_CMD_P2_RET_FCP_TEMPLATE  = 0x04,
    SIM_SELECT_CMD_P2_APPL_TERMINATION  = 0x40,
    SIM_SELECT_CMD_P2_NO_DATA_RETURNED  = 0x0C
} SIM_SELECT_CMD_P2_ENUM;

typedef enum
{
    /* The following enum will be used when the caller doesn't know the format of the security attributes */
    SIM_FCP_NO_TAG            = 0x0,
    SIM_FCP_SEC_ATTR_ANY      = 0x1,
    SIM_FCP_SEC_ATTR_REF_NO_SE_ID,
    SIM_FCP_TEMPLATE          = 0x62,
    SIM_FCP_FILE_DESC         = 0x82,
    SIM_FCP_FILE_ID           = 0x83,
    SIM_FCP_DF_NAME           = 0x84,
    SIM_FCP_PROP_INFO         = 0xA5,
    SIM_FCP_SEC_ATTR_COMP     = 0x8C,
    SIM_FCP_SEC_ATTR_EXP      = 0xAB,
    SIM_FCP_SEC_ATTR_REF      = 0x8B,
    SIM_FCP_SEC_ATTR          = 0x86,
    SIM_FCP_LIFE_CYCLE_STATUS = 0x8A,
    SIM_FCP_FILE_SIZE         = 0x80,
    SIM_FCP_TOT_FILE_SIZE     = 0x81,
    SIM_FCP_SFI               = 0x88,
    SIM_FCP_PS_TEMPLATE_DO    = 0xC6
} SIM_FCP_ENUM;

typedef enum
{
    SIM_READ_UPDATE_POS = 0x0,
    SIM_ACT_DEACT_POS       = 0x1,
    SIM_INC_POS         = 0x2
} SIM_OPS_POS_ENUM;

typedef enum
{
    SIM_SEC_PS_TEMPL_TAG      = 0xC6,
    SIM_SEC_PS_DO_TAG         = 0x90,
    SIM_SEC_USAGE_QUAL_TAG    = 0x95,
    SIM_SEC_KEY_REF_TAG       = 0x83,
    SIM_SEC_AM_DO_AM_BYTE_TAG = 0x80,
    SIM_SEC_AM_DO_INC_TAG     = 0x84,
    SIM_SEC_SC_DO_ALW_TAG     = 0x90,
    SIM_SEC_SC_DO_NEV_TAG     = 0x97,
    SIM_SEC_SC_DO_USR_VER_TAG = 0xA4,
    SIM_SEC_SC_DO_SC_BYTE_TAG = 0x9E,
    SIM_SEC_SC_DO_OR_TAG      = 0xA0,
    SIM_SEC_SC_DO_AND_TAG     = 0xAF,
    SIM_SEC_SC_DO_SM_B4_TAG   = 0xB4,
    SIM_SEC_SC_DO_SM_B6_TAG   = 0xB6,
    SIM_SEC_SC_DO_SM_B8_TAG   = 0xB8
} SIM_SEC_TAG_ENUM;

typedef enum
{
    SIM_SC_ALW      = 0x0,
    SIM_SC_NEV          = 0xFF,
    SIM_SC_USER_AUTH    = 0x10,
    SIM_SC_ADM          = 0x90
} SIM_SC_COND_ENUM;

typedef enum
{
    SIM_USAGE_QUAL_NO_VERIF   = 0x0,
    SIM_USAGE_QUAL_USE_VERIF  = 0x80,
    SIM_USAGE_QUAL_USE_COMP   = 0x40,
    SIM_USAGE_QUAL_SM_RSP     = 0x20,
    SIM_USAGE_QUAL_SM_CMD     = 0x10,
    SIM_USAGE_QUAL_PIN_VERIF  = 0x08,
    SIM_USAGE_QUAL_USER_AUTH  = 0x04
} SIM_USAGE_QUAL_ENUM;

typedef enum
{
    SIM_PIN1                     = 0x01,
    SIM_CHV_1                    = SIM_PIN1,
    SIM_CHV_2                    = 0x02,
    SIM_PIN2                     = 0x81,
    SIM_UNIV                     = 0x11,
    ADM_1                        = 0x0A,
    ADM_2                        = 0x0B,
    SIM_ADM_1                    = ADM_2,
    ADM_3                        = 0x0C,
    ADM_4                        = 0x0D,
    ADM_5                        = 0x0E,
    SIM_PIN2_UNIV_PIN,
    SIM_NO_PIN    ,
    /* USIM specific PIN values */
    SIM_PIN1_APPLN1              = 0x01,
    SIM_PIN1_APPLN2              = 0x02,
    SIM_PIN1_APPLN3              = 0x03,
    SIM_PIN1_APPLN4              = 0x04,
    SIM_PIN1_APPLN5              = 0x05,
    SIM_PIN1_APPLN6              = 0x06,
    SIM_PIN1_APPLN7              = 0x07,
    SIM_PIN1_APPLN8              = 0x08,
    SIM_ADM1_APPLN1              = 0x0A,
    SIM_ADM1_APPLN2              = 0x0B,
    SIM_ADM1_APPLN3              = 0x0C,
    SIM_ADM1_APPLN4              = 0x0D,
    SIM_ADM1_APPLN5              = 0x0E,
    SIM_PIN_UNIV                 = 0x11,
    SIM_PIN2_APPLN1              = 0x81,
    SIM_PIN2_APPLN2              = 0x82,
    SIM_PIN2_APPLN3              = 0x83,
    SIM_PIN2_APPLN4              = 0x84,
    SIM_PIN2_APPLN5              = 0x85,
    SIM_PIN2_APPLN6              = 0x86,
    SIM_PIN2_APPLN7              = 0x87,
    SIM_PIN2_APPLN8              = 0x88,
    SIM_ADM2_APPLN1              = 0x8A,
    SIM_ADM2_APPLN2              = 0x8B,
    SIM_ADM2_APPLN3              = 0x8C,
    SIM_ADM2_APPLN4              = 0x8D,
    SIM_ADM2_APPLN5              = 0x8E
} SIM_CHV_PIN_ENUM;

typedef enum
{
    SIM_ALW_OP                  = 0x0,
    SIM_OPS_PIN1                    = 0x1,
    SIM_OPS_PIN2                    = 0x2,
    SIM_OPS_PIN1_AND_PIN2           = 0x3,
    SIM_OPS_PIN1_OR_PIN2            = 0x4,
    SIM_OPS_UNIV_PIN                = 0x5,
    SIM_OPS_PIN1_AND_UNIV_PIN   = 0x6,
    SIM_OPS_PIN2_AND_UNIV_PIN   = 0x7,
    SIM_OPS_PIN1_OR_UNIV_PIN        = 0x8,
    SIM_OPS_PIN2_OR_UNIV_PIN        = 0x9,
    SIM_INVALID_OP                  = 0xE,  /* Used in expanded format handler */
    SIM_NEV                         = 0xF,
    SIM_FILE_INVALIDATED            = 0xA,
    SIM_SEC_ATTR_NOT_READ       = 0xC, /* Changed from 0xB to handle compact format*/
    SIM_OPS_ADM                 = 0xB,
    SIM_TOO_COMPLEX_RULE            = 0xD
} SIM_OPS_PIN_ENUM;

typedef enum
{
    SIM_PIN_ENABLED,
    SIM_PIN_DISABLED,
    SIM_PIN_ENABLED_AND_BLOCKED,
    SIM_PIN_DISABLED_AND_BLOCKED,
    SIM_PIN_VERIFIED,
    SIM_PIN_INITIALIZED,
    SIM_PIN_INITIALIZED_AND_BLOCKED,
    SIM_PIN_NOT_INITIALIZED
} SIM_PIN_STATUS_ENUM;

/**********************************************************************
//    Description:
//      the implementation of select a SIM file and get the select data
//    of the destionation file by using the SIM instructions
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_SelectFile(   //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,    //the file to be selected
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response    //the select data of the file
);

/**********************************************************************
//    Description:
//      Rehabilitate a EF file from the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_RehabilitateEF(   //return value:the result of rehabilitate operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name   //the EF file to be rehabilitated
);

/**********************************************************************
//    Description:
//      Read a EF file from the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_ReadEF(   //return value:the result of the read operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E            file_name,          //the file to be read
    uint16                     offset,             //the offset that start to read in transparent file
    uint8                      record_num,         //the record num to read in record file
    SIM_RECORD_ACCESS_MODE_E   mode,               //the record access mode of record file
    uint16                     *filesize_recordlen,//out:the file len of transparent file and record len of record file
    uint16                     *data_len,          //out:the data len that readed from the file
    uint8                      *data               //out:the data readef from the file
);

/**********************************************************************
//    Description:
//      Update a EF file from the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_UpdateEF( //return value:the file update operation result
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the ef file to be updated
    uint16  offset, //the offset to update in transparent file
    uint8       record_num, //the record num to update of recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of recordy file
    uint16      data_len,   //the update data len
    uint8  *data    //in:the data to update file
);

/**********************************************************************
//    Description:
//      Seek in a recordy EF file from the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_SeekInFile(    //return value:the result of seek operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the EF file to Seek in
    SIM_SEEK_TYPE_E seek_type,  //the seek opration tupe
    SIM_SEEK_MODE_E seek_mode,  //the seek mode
    uint8   pattern_len,    //the pattern len
    uint8 *pattern,     //in:the patter used in seek
    uint8 *record_num,  //out:the record num that the pattern in
    uint8 *record  //out:the record that the pattern in
);

/**********************************************************************
//    Description:
//      Increase a cyclic EF file from the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_IncreaseEF(    //return value:the increase file operation result
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the ef file to be updated
    SIM_INSTR_INCREASE_DATA_T data,   //the data to be increase
    SIM_INSTR_INCREASE_RESULT_T *result //the result that after increase
);

/**********************************************************************
//    Description:
//      invlalide a EF file from the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_InvalidateEF( //return value:the EF invalidate operation result
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name   //the EF file to be invalidated
);

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//      Get the USIM PIN/PUK remain times.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_USIMGetCHVRemainTimes(uint32 cur_sim_task, BOOLEAN is_unblock_pin_check, SIM_CHV_INDICATION_E chv_indication, uint8 *remain_times);
#endif


#ifdef   __cplusplus
}
#endif  //__cplusplus
#endif  //_SIM_INSTRUCTION_H_
