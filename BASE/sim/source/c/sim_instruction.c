/******************************************************************************
** File Name:      sim_insturction.c                                         *
** Author:         Raislin.Kong                                              *
** Date:           2001.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the c file about the sim card instruction         *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2001.2         Raislin.Kong     Create                                    *
******************************************************************************/
#include "base_trc.h"
#include "sim_include.h"

#ifdef   __cplusplus
extern   "C"
{
#endif

//#define YWD_REF_ARR_SELF_TEST 1//self test code, wuding.

//In compact mode, we only deal READ/WRITE, ignore others.
#define SIM_AM_BYTE_MASK_READ       0x1
#define SIM_AM_BYTE_MASK_UPDATE     0x2
#define SIM_AM_BYTE_MASK_WRITE      0x4
#define SIM_AM_BYTE_MASK_DEACTIVATE 0x8
#define SIM_AM_BYTE_MASK_ACTIVATE   0x10
#define SIM_AM_BYTE_MASK_TERMINATE  0x20
#define SIM_AM_BYTE_MASK_DELETE     0x40
#define SIM_AM_BYTE_MASK_INCREASE   0x32

#define SIM_READ_MASK               0x0f
#define SIM_UPDATE_MASK             0xf0
#define SIM_ACTIVATE_MASK           0x0f
#define SIM_DEACTIVATE_MASK         0xf0
#define SIM_INCREASE_MASK           0x0f
#define SIM_RFU_MASK                0xf0
#define SIM_FILE_ROUTE_LEN          0x10

typedef enum
{
    SIM_SE_0,
    SIM_SE_1
}SIM_SEC_ENV_ENUM;

typedef struct
{
    BOOLEAN is_ref_exist;
    uint16 file_id;
    uint8 seid_valid_num;
    uint8 seid[2];// 2 is enough according to 3G code
    uint8 rec_index_valid_num;
    uint8 rec_index[2];// 2 is enough according to 3G code
} SIM_FCP_REF_ARR_T;

STATIC SIM_RSP_INFO_T      s_sim_rsp_info[SIM_CARD_NUM];

//STATIC variable
//the current file response
STATIC SIM_RESPONSE_DATA_OF_SELECT_T   s_sim_cur_file_response[SIM_CARD_NUM];
STATIC SIM_RSP_INFO_T                  s_select_sw_and_rsp[SIM_CARD_NUM];// the SW and response string of the variable s_sim_cur_file_response
// used only on the select command

//the instruction list used to save the instruction list to implement a operation
STATIC SIMINSTR_LIST_T s_sim_instruction_list[SIM_CARD_NUM];
// preset the SIM card instruction code reference to
//SPEC GSM11.11 section 9.2
STATIC SIMINSTR_DATA_BUF_T s_sim_instruction_data_buf[SIM_CARD_NUM];
//the instruction code talbe
STATIC const uint8  s_sim_InstrCodeTab[SIMINSTR_TOTAL_NUM][TB_SIM_APDU_CHEADER_NUM + 1] =
{
    /*+-----+-----+-----+-----+-----+---------------------------------+*/
    /*| CLA | INS | P1  | P2  | P3  |       Instruction type          |*/
    /*+-----+-----+-----+-----+-----+---------------------------------+*/
    { 0xA0, 0xA4, 0x00, 0x00, 0x02, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* SELECT instruction */
    { 0xA0, 0xF2, 0x00, 0x00, 0xFF, (uint16)SIMHAL_SIMINSTR_RECV_DATA}, /* STATUS        inst */
    { 0xA0, 0xB0, 0xFF, 0xFF, 0xFF, (uint16)SIMHAL_SIMINSTR_RECV_DATA}, /* READ Binary   inst */
    { 0xA0, 0xD6, 0xFF, 0xFF, 0xFF, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* UPDATE Binary inst */
    { 0xA0, 0xB2, 0xFF, 0xFF, 0xFF, (uint16)SIMHAL_SIMINSTR_RECV_DATA}, /* READ Record   inst */
    { 0xA0, 0xDC, 0xFF, 0xFF, 0xFF, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* UPDATE Record inst */
    { 0xA0, 0xA2, 0x00, 0xFF, 0xFF, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* SEEK          inst */
    { 0xA0, 0x32, 0x00, 0x00, 0x03, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* INCREASE      inst */
    { 0xA0, 0x20, 0x00, 0xFF, 0x08, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* VERIFY        inst */
    { 0xA0, 0x24, 0x00, 0xFF, 0x10, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* CHANGE        inst */
    { 0xA0, 0x26, 0x00, 0x01, 0x08, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* DISABLE       inst */
    { 0xA0, 0x28, 0x00, 0x01, 0x08, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* ENABLE        inst */
    { 0xA0, 0x2C, 0x00, 0xFF, 0x10, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* UNBLOCK       inst */
    { 0xA0, 0x04, 0x00, 0x00, 0x00, (uint16)SIMHAL_SIMINSTR_NONE_DATA}, /* INVALIDATE    inst */
    { 0xA0, 0x44, 0x00, 0x00, 0x00, (uint16)SIMHAL_SIMINSTR_NONE_DATA}, /* REHABILITATE  inst */
    { 0xA0, 0x88, 0x00, 0x00, 0x10, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* RUN A3        inst */
    { 0xA0, 0xFA, 0x00, 0x00, 0x00, (uint16)SIMHAL_SIMINSTR_NONE_DATA}, /* SLEEP         inst */
    { 0xA0, 0xC0, 0x00, 0x00, 0xFF, (uint16)SIMHAL_SIMINSTR_RECV_DATA}, /* GET RESPONSE  inst */
    { 0xA0, 0x10, 0x00, 0x00, 0xFF, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* TERMINAL PROFILE inst*/
    { 0xA0, 0xC2, 0x00, 0x00, 0xFF, (uint16)SIMHAL_SIMINSTR_SEND_DATA}, /* ENVELOPE      inst*/
    { 0xA0, 0x12, 0x00, 0x00, 0xFF, (uint16)SIMHAL_SIMINSTR_RECV_DATA}, /* FETCH         inst*/
    { 0xA0, 0x14, 0x00, 0x00, 0xFF, (uint16)SIMHAL_SIMINSTR_SEND_DATA} /* TEMINAL RESPONSE inst*/
};
//the temp signal header,used as a signal queue header that temporary save signal
//from other task when the SIM task is in comunication with SIM card
STATIC xSignalHeaderRec *s_temp_signal_queue[SIM_CARD_NUM];
//this queue use to temporary save the interrupt signal from SIM card
STATIC xSignalHeaderRec *s_temp_int_signal_queue[SIM_CARD_NUM];

extern uint32 g_sim_simat_task_id[SIM_CARD_NUM];

#ifdef SIM_3G_USIM_SUPPORT
//To store an USIM EFarr record for file arr check
static uint8 s_sim_cur_file_arr_record[SIM_CARD_NUM][256+2] = {0};//+2的2用来放0xAB 和len
/* This field stores the status of PIN1 */
#ifdef MULTI_SIM_SYS_QUAD
static SIM_PIN_STATUS_ENUM s_sim_usim_pin1_status[SIM_CARD_NUM] = {SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED};
static SIM_PIN_STATUS_ENUM s_sim_usim_univ_pin_status[SIM_CARD_NUM] = {SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED};
static BOOLEAN s_sim_usim_use_univ_pin[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
BOOLEAN s_sim_usim_adf_file_id_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
BOOLEAN s_sim_gain_chv_remain_times_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#elif defined (MULTI_SIM_SYS_TRI)
static SIM_PIN_STATUS_ENUM s_sim_usim_pin1_status[SIM_CARD_NUM] = {SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED};
static SIM_PIN_STATUS_ENUM s_sim_usim_univ_pin_status[SIM_CARD_NUM] = {SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED};
static BOOLEAN s_sim_usim_use_univ_pin[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
BOOLEAN s_sim_usim_adf_file_id_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
BOOLEAN s_sim_gain_chv_remain_times_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#else
static SIM_PIN_STATUS_ENUM s_sim_usim_pin1_status[SIM_CARD_NUM] = {SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED};
static SIM_PIN_STATUS_ENUM s_sim_usim_univ_pin_status[SIM_CARD_NUM] = {SIM_PIN_NOT_INITIALIZED, SIM_PIN_NOT_INITIALIZED};
static BOOLEAN s_sim_usim_use_univ_pin[SIM_CARD_NUM] = {FALSE, FALSE};
BOOLEAN s_sim_usim_adf_file_id_flag[SIM_CARD_NUM] = {FALSE, FALSE};
BOOLEAN s_sim_gain_chv_remain_times_flag[SIM_CARD_NUM] = {FALSE, FALSE};
#endif
#endif

#ifdef SIM_DROP_RESTORE_SUPPORT
static BOOLEAN s_sim_recv_new_auth_in_auth[4] = {FALSE,FALSE,FALSE,FALSE};

static BOOLEAN s_sim_recv_new_gmm_auth_in_auth[4] = {FALSE,FALSE,FALSE,FALSE};
#endif
//-------------------------------------the local funciton declare-------------------------
#ifndef SIM_TEST_IN_WIN32
/**********************************************************************
//    Description:
//      Send instruction code to the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void SendInstrCode(
    uint32 cur_sim_task,
    TB_SIM_APDU_T *apdu_ptr    //in:the instruction code to be sent to SIM card
);
#endif
/**********************************************************************
//    Description:
//      Receive the reponse data from the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E ReceiveInstrResponse(   //return value:the type of the singal that send by SIM card interrupt
    uint32 cur_sim_task,
    SIM_SIG_RESPONSE_IND_T **instr_response //the pointer to save the pointer of the self response signal
);

/**********************************************************************
//    Description:
//     Judge whether the current instruction is stk command or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_IsSTKCommand(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//     creat the card unresponse signal,the signal will add to the temp int signal of the sim
//    to restore the sim card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void SIMINSTR_AddRestoreSignal(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      get the return result of sim card response by the status word,
//    referrence to to GSM 11.11 section 9.4 "Staus conditions returned
//    by the card"
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIMINSTR_RESULT_TYPE_E SIMINSTR_GetStatusWordType(   //return value:the insturction result type of the status word
    uint32 cur_sim_task,
    uint8 sw1,      //the status word 1
    uint8 sw2,      //the status word 2
    uint16  data_len    //the response data len
);

/**********************************************************************
//    Description:
//      analysis the error reason of Select instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleSelectCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Status instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleStatusCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      Clear all the instructions in the instruction list
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ClearInstrList(uint32 cur_sim_task);

/*****************************************************************************/
//  Description : save the response information
//  Global resource dependence :
//  Author: Tracy.zhang
//  Note: used for CRSM
/*****************************************************************************/
STATIC void SIMINSTR_CRSM_SaveResponseInfo(
    uint32                  cur_sim_task,   //IN:
    SIM_FILE_NAME_E         file_name,      //IN:
    SIM_SIG_RESPONSE_IND_T  *instr_response //IN:
);

/**********************************************************************
//    Description:
//      recieve a signal from the SIM drive layer.if there comes service
//    request  signals from other layer,just save it to the temporary
//    queue for laterly use.and if there comes the exception of the SIM
//    card such as Rx Parity error or Card out,report error to the SIM
//    layer and save signal to the temporary interrupt signal queue for
//    laterly signal proccess.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E SIMINSTR_ReceiveInstrResponse(   //return value:the type of the singal that send by SIM card interrupt
    uint32 cur_sim_task,
    SIM_SIG_RESPONSE_IND_T **instr_response //the pointer to save the pointer of the self response signal
);

/**********************************************************************
//    Description:
//      analysis the error reason of Update Binary instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleUpdateBinaryCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      Get the card supply voltage form the byte 14(bit5-7) of the select response of the MF
//    see GSM11.14 section 9.2.1 and GSM11.18 section 4.3
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SUPPLY_VOLT_E GetCardSupplyVot(uint8 file_character);

/**********************************************************************
//    Description:
//      analysis the error reason of Update Record instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleUpdateRecordCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Read Binary instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleReadBinaryCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Read Record instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleReadRecordCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Seek instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleSeekCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Increase instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleIncreaseCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Verify CHV instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleVerifyCHVCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Change CHV instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleChangeCHVCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Disable CHV instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleDisableCHVCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Enable CHV instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleEnableCHVCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Unblock CHV instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleUnblockCHVCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Invalidate instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleInvalidateCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Rehabilitate instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleRehabilitateCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      Handle the command error that will happen to every command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleCommonCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word  //the status word
);
/**********************************************************************
//    Description:
//      analysis the error reason of GSM Algorithm instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleGSMAlgorithmCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Get Response instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleGetResponseCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

//the SIM application toolkit command function
/**********************************************************************
//    Description:
//      analysis the error reason of Terminal Profile instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleTerminalProfileCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Envelope Command instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleEnvelopeCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Fetch Command instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleFetchCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Termianl Response instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleTerminalResponseCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);

/**********************************************************************
//    Description:
//      analysis the error reason of Command instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleCommandsError(    //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_NAME_E instr_name, //the isntruction name
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
);


/**********************************************************************
//    Description:
//      Get the access fail status
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E GetAccFailResult(
    uint32 cur_sim_task,
    SIM_ACCESS_CONDITION_E acc_condition    //the access condition of the operation
);


/**********************************************************************
//    Description:
//      Get a instruction code of sim instruction by the instruction name
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC int16 GetInstructionCode(  //return value:the success or fail to get the instruction code
    uint32 cur_sim_task,
    SIMINSTR_CODE_T *sim_instr_code, //in:the pointer to save the instruction code
    SIMINSTR_NAME_E sim_instr_name   //the instruction name want to get
);

/**********************************************************************
//    Description:
//      check the service result,if the service result show the proactive
//    command exist,add the proactive command request signal to the service
//    request queue
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC BOOLEAN CheckPCExistence(
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E    ser_result, //the service result that need to check
    uint8 pc_len    //the length of the proactive command
);

/**********************************************************************
//    Description:
//      Clear the instruciton memory buf
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ClearInstrDataBuf(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      alloc memory for instrction from the instruction memory buf
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC uint8 *AllocFromInstrDataBuf(  //return value:the mempry pointer
    uint32 cur_sim_task,
    uint16 req_size //the alloc memory size
);

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//      Decode the File ID field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_FileID(uint8 *data_ptr, uint16 *data_postion, uint16 *file_id);

/**********************************************************************
//    Description:
//      Decode the AID field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_AID(uint8 *data_ptr,  uint16 *data_position);

/**********************************************************************
//    Description:
//      Decode the Proprietary Information field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_ProprietaryInfo(uint8 *data_ptr,  uint16 *data_position, uint8 *filecharacter);

/**********************************************************************
//    Description:
//      Decode the Life Cycle Status field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_LifeCycleStatus(uint8 *data_ptr,  uint16 *data_position, uint8 *life_cycle_status);

/**********************************************************************
//    Description:
//      Decode the Security Attribute in compact mode in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_SA_Compact(uint8 *data_ptr, uint16 *access_condition1, uint8 *access_condition2);

/**********************************************************************
//    Description:
//      Decode the Security Attribute in expanded mode in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_SA_Expanded(uint8 *data_ptr, uint16 *access_condition1, uint8 *access_condition2);

/**********************************************************************
//    Description:
//      Decode the Security Attribute in reference expanded mode in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_SA_RefExpanded(uint8 *data_ptr, SIM_FCP_REF_ARR_T *arr_info);

/**********************************************************************
//    Description:
//      Decode the Security Attribute field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_SecurityAttributes(uint8 *data_ptr,  uint16 *data_position, uint16 *access_condition1, uint8 *access_condition2, SIM_FCP_REF_ARR_T *arr_info);

/**********************************************************************
//    Description:
//      Decode the PIN status template DO field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_PSDO(uint32 cur_sim_task, uint8 *data_ptr, uint16 *data_position, BOOLEAN is_adf_usim, BOOLEAN *chv1_enabled);

/**********************************************************************
//    Description:
//      Decode the File Descriptor field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_FileDescriptor(uint8 *data_ptr,  uint16 *data_position, uint8 *efstructure, uint8 *recordlen);

/**********************************************************************
//    Description:
//      Decode the File size field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_FileSize(uint8 *data_ptr,  uint16 *data_position, uint16 *file_size);

/**********************************************************************
//    Description:
//      Decode the the FCP after a SELECT instruction.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP(uint32 cur_sim_task, SIM_FILE_NAME_E file_name, uint8 *data_ptr, uint16 data_len, SIM_FCP_REF_ARR_T *arr_info, SIM_RESPONSE_DATA_OF_SELECT_T *out_ptr);

/**********************************************************************
//    Description:
//      ETST TS102.221 9.3.1 Decide the security environment according to the PIN status.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC SIM_SEC_ENV_ENUM SIMINSTR_GET_CUR_SEC_ENV(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      When an EF's security attributes is in a reference expanded mode, we should
//       read the EFarr and decode the SA info.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_GetARR(uint32 cur_sim_task, SIM_FILE_NAME_E cur_file_name, SIM_FCP_REF_ARR_T arr_info, uint16 *access_condition1, uint8 *access_condition2);

/**********************************************************************
//    Description:
//      the implementation of select a USIM file by the specified file path
//    and get the select data
//    of the destionation file by using the SIM instructions
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E SIMINSTR_UsimSelectFileByPath(   //return value:the result of the operation
    uint32 cur_sim_task,
    uint16 file_id,    //the file to be selected
    uint8 route_len,  //the file route data length
    uint16 *file_route, //the file path to access the file
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response    //the select data of the file
);

#endif//SIM_3G_USIM_SUPPORT

/**********************************************************************
//    Description:
//      the implementation of select a SIM file by the specified file path
//    and get the select data
//    of the destionation file by using the SIM instructions
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E SIMINSTR_SimSelectFileByPath(   //return value:the result of the operation
    uint32 cur_sim_task,
    uint16 file_id,    //the file to be selected
    uint8 route_len,  //the file route data length
    uint16 *file_route, //the file path to access the file
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response    //the select data of the file
);

/**********************************************************************
//    Description:
//    Translate the sim-drv SELECT response into SIM_RESPONSE_DATA_OF_SELECT_T.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC void SIMINSTR_RemapSelectResposne(
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response, //out:the status of the file
    uint8 *data_buf,  //IN:
    uint8 data_len //IN:
);

#ifdef SIM_DROP_RESTORE_SUPPORT
/**********************************************************************
//    Description:
//    
//    Global resource dependence :
//    Author:wuding.yang
//    Note: 
***********************************************************************/
STATIC uint32 SIMINSTR_CountWaitTime(uint32 cur_sim_task,uint8 rx_expected_len);
#endif

//----------------------------the function define---------------------------

/**********************************************************************
//    Description:
//      check the service result,if the service result show the proactive
//    command exist,add the proactive command request signal to the service
//    request queue
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC BOOLEAN CheckPCExistence(
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E    ser_result, //the service result that need to check
    uint8 pc_len    //the length of the proactive command
)
{
    SIMAT_SIG_PC_IND_T *ppcsig;
    SIMAT_SIG_SESSION_TERMINATED_IND_T *pstsig;
    SIM_SERVICE_RESULT_E result;
#ifdef GSM_DPHONE_SUPPORT
    static uint32 enter_times[4] = {0};
    uint8 total_len, temp_len = 0;
    uint32 index = 2;
#endif

    if(SIM_STATUS_OK_PROACTIVE_INFO == ser_result)
    {
        SCI_CREATE_SIGNAL(ppcsig, SIM_PC_EXIST_IND, sizeof(SIM_SIG_PC_EXIST_IND_T), g_sim_task_id[cur_sim_task]);
        ppcsig->pc_data.data_len = pc_len;
        result = SIMINSTR_Fetch(cur_sim_task, &(ppcsig->pc_data));

        if((SIM_SERVICE_OK != result) ||
            (ppcsig->pc_data.data_len == 0))
        {
            SCI_FREE_SIGNAL(ppcsig);
            //SCI_TRACE_LOW:"SIM::Fetch Proactive Command Error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_712_112_2_17_22_59_17_4496, (uint8 *)"");
            SCI_CREATE_SIGNAL(pstsig, SIMAT_SESSION_TERMINATED_IND, sizeof(SIMAT_SIG_SESSION_TERMINATED_IND_T), g_sim_task_id[cur_sim_task]);
            pstsig->dual_sys = cur_sim_task;
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pstsig, g_sim_simat_task_id[cur_sim_task]);
            return FALSE;
        }

#ifdef GSM_DPHONE_SUPPORT
        {
            if((0 == s_dphone_reset_times[cur_sim_task])
                && (0 == enter_times[cur_sim_task]))
            {
                enter_times[cur_sim_task]++;

                if(0xD0 == ppcsig->pc_data.data_arr[0])
                {
                    total_len = MIN(ppcsig->pc_data.data_len, ppcsig->pc_data.data_arr[1]);

                    while(1)
                    {
                        temp_len = ppcsig->pc_data.data_arr[index+1];

                        if(0 == temp_len)
                        {
                            break;//invalid length
                        }

                        if(((0x1 == ppcsig->pc_data.data_arr[index]) || (0x81 == ppcsig->pc_data.data_arr[index]))
                            && (0x3 == temp_len))//command detail
                        {
                            if(0x23 == ppcsig->pc_data.data_arr[index+3]) //GetInput
                            {
                                s_sim_gsm_dphone_support[cur_sim_task] = TRUE;//Yes, this is a Dphone sim card.
                            }
                            //LAO SIM
                            else if ((0x05 == ppcsig->pc_data.data_arr[index+3]) 
							  && (0x19 == ppcsig->pc_data.data_arr[index+9]) 
							  && (0x00 == ppcsig->pc_data.data_arr[index+10]))
                            {
                                s_sim_gsm_dphone_support[cur_sim_task] = TRUE;//LAO Dphone SIM
                            }
                            break;
                        }
                        else
                        {
                            index++;//the length byte
                            index += temp_len;
                            index++;//move to the next tag

                            if(index >= total_len)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
#endif

        SCI_SEND_SIGNAL((xSignalHeaderRec *)ppcsig, g_sim_task_id[cur_sim_task]);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************
//    Description:
//      initialize the temporary signal queue
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMINSTR_InitTempSignalQueues(uint32 cur_sim_task)
{
    SCI_SIGLIST_INIT(&s_temp_signal_queue[cur_sim_task]);
    SCI_SIGLIST_INIT(&s_temp_int_signal_queue[cur_sim_task]);
}


/**********************************************************************
//    Description:
//      Get a instruction code of sim instruction by the instruction name
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC int16 GetInstructionCode(  //return value:the success or fail to get the instruction code
    uint32 cur_sim_task,
    SIMINSTR_CODE_T *sim_instr_code, //in:the pointer to save the instruction code
    SIMINSTR_NAME_E sim_instr_name   //the instruction name want to get
)
{
    SCI_ASSERT(NULL != sim_instr_code);/*assert verified: Null_pointer*/

    //error instruction name
    if(SIMINSTR_TOTAL_NUM <= sim_instr_name)
    {
        return (int16) - 1;
    }

    sim_instr_code->instr_name = sim_instr_name;
    //get the instruction type
    sim_instr_code->apdu.instr_type = (SIMHAL_SIMINSTR_TYPE_E)s_sim_InstrCodeTab[sim_instr_name][TB_SIM_APDU_CHEADER_NUM];
    //get the command code
    SCI_MEMCPY(sim_instr_code->apdu.instr_code, s_sim_InstrCodeTab[sim_instr_name], TB_SIM_APDU_CHEADER_NUM);

#ifdef SIM_3G_USIM_SUPPORT

    //If 3G USIM, we use the CLA as 0x00.
    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        if((0x10 == sim_instr_code->apdu.instr_code[1]) || (0x12 == sim_instr_code->apdu.instr_code[1])
            || (0x14 == sim_instr_code->apdu.instr_code[1]) || (0xC2 == sim_instr_code->apdu.instr_code[1])
            || (0xF2 == sim_instr_code->apdu.instr_code[1]) || (0x32 == sim_instr_code->apdu.instr_code[1]))
        {
            sim_instr_code->apdu.instr_code[0] = 0x80;
        }
        else
        {
            sim_instr_code->apdu.instr_code[0] = 0;
        }
    }

#endif

    return (int16)0;
}

/**********************************************************************
//    Description:
//      Get the coresponding access condition type of access condition of
//    a sim file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_ACCESS_CONDITION_E SIMAT_GetAccessCondition(
    SIM_ACCESS_CONDITION_TYPE_E acc_type,   //the access type
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data   //the response data of the sim file
)
{
    SIM_FILE_ACCESS_CONDITION_T file_acc;

    GET_FILE_ACCESS_CONDITION(file_acc, select_data);

    switch(acc_type)
    {
        case SIM_READ_ACCESS:
            return file_acc.seek_read_access;
        case SIM_UPDATE_ACCESS:
            return file_acc.update_access;
        case SIM_INCREASE_ACCESS:
            return file_acc.increase_access;
        case SIM_INVALIDATE_ACCESS:
            return file_acc.invalidate_access;
        case SIM_REHABILITATE_ACCESS:
            return file_acc.rehabilitate_access;
        default:
            SCI_TRACE_HIGH("SIM: SIMAT_GetAccessCondition acc_type = %d", acc_type);/*assert verified */
            return SIM_ACCESS_ALW;
    }
}


/**********************************************************************
//    Description:
//      Get the access fail status
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E GetAccFailResult(
    uint32 cur_sim_task,
    SIM_ACCESS_CONDITION_E acc_condition    //the access condition of the operation
)
{
    switch(acc_condition)
    {
        case SIM_ACCESS_ALW:
            return SIM_SERVICE_OK;
        case SIM_ACCESS_CHV1:

            if(!g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked)
            {
                return SIM_SERVICE_CHV1_NEEDED;
            }
            else if(!g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.chv_blocked)
            {
                return SIM_SERVICE_U_CHV1_NEEDED;
            }
            else
            {
                return SIM_SERVICE_CHV_BLOCKED;
            }

        case SIM_ACCESS_CHV2:

            if(!g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.chv_blocked)
            {
                return SIM_SERVICE_CHV2_NEEDED;
            }
            else if(!g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.chv_blocked)
            {
                return SIM_SERVICE_U_CHV2_NEEDED;
            }
            else
            {
                return SIM_SERVICE_CHV_BLOCKED;
            }

        case SIM_ACCESS_RFU:
        case SIM_ACCESS_ADM1:
        case SIM_ACCESS_ADM2:
        case SIM_ACCESS_ADM3:
        case SIM_ACCESS_ADM4:
        case SIM_ACCESS_ADM5:
        case SIM_ACCESS_ADM6:
        case SIM_ACCESS_ADM7:
        case SIM_ACCESS_ADM8:
        case SIM_ACCESS_ADM9:
        case SIM_ACCESS_ADM10:
        case SIM_ACCESS_ADM11:
        case SIM_ACCESS_NEVER:
            return SIM_SERVICE_ACCESS_NEVER;

        default:
            SCI_TRACE_HIGH("SIM: GetAccFailResult acc_condition = %d", acc_condition);/*assert verified */
            return SIM_SERVICE_ERROR_UNKNOW;
    }
}

/**********************************************************************
//    Description:
//      Clear the instruciton memory buf
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ClearInstrDataBuf(uint32 cur_sim_task)
{
    //clear the data buf used before
    s_sim_instruction_data_buf[cur_sim_task].cur_pos = 0;
}


/**********************************************************************
//    Description:
//      alloc memory for instrction from the instruction memory buf
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC uint8 *AllocFromInstrDataBuf(  //return value:the mempry pointer
    uint32 cur_sim_task,
    uint16 req_size //the alloc memory size
)
{
    uint8 *ptemp = NULL;

    //the buf is not enough
    if(req_size > SIMINSTR_DATA_BUF_T_LEN - s_sim_instruction_data_buf[cur_sim_task].cur_pos)
    {
        return NULL;
    }

    //alloc data from the buf
    ptemp = s_sim_instruction_data_buf[cur_sim_task].buf + s_sim_instruction_data_buf[cur_sim_task].cur_pos;
    s_sim_instruction_data_buf[cur_sim_task].cur_pos += req_size;

    return ptemp;
}

/**********************************************************************
//    Description:
//     creat the card unresponse signal,the signal will add to the temp int signal of the sim
//    to restore the sim card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void SIMINSTR_AddRestoreSignal(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig = NULL;

    //creat the card unresponse signal,the signal will add to the temp int signal of the sim
    //to restore the sim card
    SCI_CREATE_SIGNAL(psig, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[cur_sim_task]);

    ((SIM_SIG_INT_IND_T *)psig)->int_type = TB_SIMINT_CARD_UNRESPONSE;

    //save the interrupt signal to temp interrupt signal queue
    SCI_SIGLIST_ADDTAIL(&s_temp_int_signal_queue[cur_sim_task], psig);
}

/**********************************************************************
//    Description:
//      get the return result of sim card response by the status word,
//    referrence to to GSM 11.11 section 9.4 "Staus conditions returned
//    by the card"
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIMINSTR_RESULT_TYPE_E SIMINSTR_GetStatusWordType(   //return value:the insturction result type of the status word
    uint32 cur_sim_task,
    uint8 sw1,      //the status word 1
    uint8 sw2,      //the status word 2
    uint16  data_len    //the response data len
)
{
    SIMINSTR_RESULT_TYPE_E result_type = SIM_STATUS_WRONG_WORD;

    if((0x90 == sw1) && (0x00 == sw2))
    {
        return SIM_STATUS_OK_NORMAL;
    }

    if((0x9f == sw1) || (0x61 == sw1))
    {
        return SIM_STATUS_OK_RESPONSE_DATA;
    }

    if(0x91 == sw1)
    {
        return SIM_STATUS_OK_PROACTIVE_INFO;
    }

    if(0x6c == sw1)
    {
        return SIM_STATUS_OK_RESEND_ADPU;
    }

    if((0x93 == sw1) && (0x00 == sw2))
    {
        result_type = SIM_STATUS_TOOLKIT_BUSY;
    }

    if(0x9e == sw1)
    {
        result_type = SIM_STATUS_OK_DOWN_DATA_ERR;
    }

    if((0x92 == sw1) && ((sw2 & 0xf0) == 0))
    {
        return SIM_STATUS_OK_WITH_UPDATE;
    }
    else if((0x92 == sw1) && (0x40 == sw2))
    {
        result_type = SIM_STATUS_MEMORY_PROBLEM;
    }
    else if((0x94 == sw1) && (0x00 == sw2))
    {
        result_type = SIM_STATUS_NO_EF_SELECETED;
    }
    else if((0x94 == sw1) && (0x02 == sw2))
    {
        result_type = SIM_STATUS_INVALIDE_ADDRESS;
    }
    else if((0x94 == sw1) && (0x04 == sw2))
    {
        result_type = SIM_STATUS_INVALIDE_FILE_ID;
    }
    else if((0x94 == sw1) && (0x08 == sw2))
    {
        result_type = SIM_STATUS_INCONSISTENT_FILE;
    }
    else if((0x98 == sw1) && (0x02 == sw2))
    {
        result_type = SIM_STATUS_CHV_UNINITIALIZED;
    }
    else if((0x98 == sw1) && (0x04 == sw2))
    {
        result_type = SIM_STATUS_ACCESS_FAIL;
    }
    else if((0x98 == sw1) && (0x08 == sw2))
    {
        result_type = SIM_STATUS_CHV_CONTRADICTION;
    }
    else if((0x98 == sw1) && (0x10 == sw2))
    {
        result_type = SIM_STATUS_INVALIDATION_CONTRADICTION;
    }
    else if((0x98 == sw1) && (0x40 == sw2))
    {
        result_type = SIM_STATUS_CHV_VER_BLOCKED;
    }
    else if((0x98 == sw1) && (0x50 == sw2))
    {
        result_type = SIM_STATUS_MAX_RECORD_VALUE;
    }
    else if((0x98 == sw1) && (0x62 == sw2))
    {
        result_type = SIM_STATUS_AUTH_MAC_ERROR;
    }
    else if((0x98 == sw1) && (0x63 == sw2))
    {
        result_type = SIM_STATUS_SEC_ASSO_EXPIRED;
    }
    //here is something wrong
    else if((0x62 == sw1) && (0x00 == sw2))
    {
        result_type = SIM_STATUS_NO_INFO_GIVEN_62;
    }
    else if((0x62 == sw1) && (0x81 == sw2))
    {
        result_type = SIM_STATUS_DATA_CORRUPT;
    }
    else if((0x62 == sw1) && (0x82 == sw2))
    {
        result_type = SIM_STATUS_END_OF_FILE;
    }
    else if((0x62 == sw1) && (0x83 == sw2))
    {
        result_type = SIM_STATUS_SEL_FILE_INVALIDE;
    }
    else if((0x62 == sw1) && (0x85 == sw2))
    {
        result_type = SIM_STATUS_SEL_FILE_IN_TERMINATION;
    }
    else if((0x62 == sw1) && (0xF1 == sw2))
    {
        result_type = SIM_STATUS_MORE_DATA_AVAILABLE;
    }
    else if((0x62 == sw1) && (0xF2 == sw2))
    {
        result_type = SIM_STATUS_MORE_DATA_AVAILABLE_WITH_PC;
    }
    else if((0x62 == sw1) && (0xF3 == sw2))
    {
        result_type = SIM_STATUS_RESPONSE_DATA_AVAILABLE;
    }
    else if((0x63 == sw1) && (0xF1 == sw2))
    {
        result_type = SIM_STATUS_MORE_DATA_EXCEPTED;
    }
    else if((0x63 == sw1) && (0xF2 == sw2))
    {
        result_type = SIM_STATUS_MORE_DATA_EXCEPTED_WITH_PC;
    }
    else if(0x63 == sw1)
    {
        result_type = SIM_STATUS_SUCCESS_WITH_PROBLEM;
    }
    else if((0x64 == sw1) && (0x00 == sw2))
    {
        result_type = SIM_STATUS_NO_INFO_GIVEN_64;
    }
    else if((0x65 == sw1) && (0x00 == sw2))
    {
        result_type = SIM_STATUS_NO_INFO_GIVEN_65;
    }
    else if((0x65 == sw1) && (0x81 == sw2))
    {
        result_type = SIM_STATUS_MEMORY_PROBLEM_65;
    }
    else if((0x67 == sw1) && (0x0 == sw2))
    {
        result_type = SIM_STATUS_WRONG_LENGTH;
    }
    else if(0x67 == sw1)
    {
        result_type = SIM_STATUS_P3_INCORRECT;
    }
    else if((0x68 == sw1) && (0x00 == sw2))
    {
        result_type = SIM_STATUS_NO_INFO_GIVEN_68;
    }
    else if((0x68 == sw1) && (0x81 == sw2))
    {
        result_type = SIM_STATUS_LCHAN_NOT_SUPPORT;
    }
    else if((0x68 == sw1) && (0x82 == sw2))
    {
        result_type = SIM_STATUS_SECMSG_NOT_SUPPORT;
    }
    else if((0x69 == sw1) && (0x00 == sw2))
    {
        result_type = SIM_STATUS_NO_INFO_GIVEN_69;
    }
    else if((0x69 == sw1) && (0x81 == sw2))
    {
        result_type = SIM_STATUS_INCOM_WITH_STRUCTRUE;
    }
    else if((0x69 == sw1) && (0x82 == sw2))
    {
        result_type = SIM_STATUS_SECSTATUS_NOT_SATISFIED;
    }
    else if((0x69 == sw1) && (0x83 == sw2))
    {
        result_type = SIM_STATUS_AUTH_METHOD_BLOKCED;
    }
    else if((0x69 == sw1) && (0x84 == sw2))
    {
        result_type = SIM_STATUS_REF_DATA_INVALIDATE;
    }
    else if((0x69 == sw1) && (0x85 == sw2))
    {
        result_type = SIM_STATUS_USE_NOT_SATISFIED;
    }
    else if((0x69 == sw1) && (0x86 == sw2))
    {
        result_type = SIM_STATUS_CMD_NOT_ALLOWED;
    }
    else if((0x6a == sw1) && (0x80 == sw2))
    {
        result_type = SIM_STATUS_INCORRECT_PARA;
    }
    else if((0x6a == sw1) && (0x81 == sw2))
    {
        result_type = SIM_STATUS_FUNC_NOT_SUPPORT;
    }
    else if((0x6a == sw1) && (0x82 == sw2))
    {
        result_type = SIM_STATUS_FILE_NOT_FOUND;
    }
    else if((0x6a == sw1) && (0x83 == sw2))
    {
        result_type = SIM_STATUS_REC_NOT_FOUND;
    }
    else if((0x6a == sw1) && (0x84 == sw2))
    {
        result_type = SIM_STATUS_NOT_ENOUGH_MEM;
    }
    else if((0x6a == sw1) && (0x86 == sw2))
    {
        result_type = SIM_STATUS_P1P2_INCORRECT;
    }
    else if((0x6a == sw1) && (0x87 == sw2))
    {
        result_type = SIM_STATUS_P1P2_INCONSISTENT;
    }
    else if((0x6a == sw1) && (0x88 == sw2))
    {
        result_type = SIM_STATUS_REF_DATA_NOT_FOUND;
    }
    else if(0x6b == sw1)
    {
        result_type = SIM_STATUS_P1_OR_P2_INCORRECT;
    }
    else if(0x6d == sw1)
    {
        result_type = SIM_STATUS_UNKNOW_INSTRUCTION_CODE;
    }
    else if(0x6e == sw1)
    {
        result_type = SIM_STATUS_WRONG_INSTRUCTION_CLASS;
    }
    else if((0x6f == sw1) && (0x0 == sw2))
    {
        result_type = SIM_STATUS_TECHNICAL_PROBLEM;
    }
    //when select the three level ef file,the high byte of file id of the
    //file is 0x6f,so the status  word must be distinguished.
    else if(0x6f == sw1)
    {
        result_type = SIM_STATUS_UNKNOW_FAIL_REASON;
    }

#ifndef WIN32

    if(data_len == 0)
    {
        //return result_type;
    }
    else
    {
        if((0x90 == (0x90 & sw1)) || (0x60 == (0x60 & sw1)))
        {
            //status words is not defined in the spec, but is still 9X or 6X.
            result_type = SIM_STATUS_WRONG_WORD;
        }
        else
        {
            //add restore signal to the temp int queue
            SIMINSTR_AddRestoreSignal(cur_sim_task);
            result_type = SIM_STATUS_ERROR_DATA;
        }
    }

#endif
    return result_type;
}

/**********************************************************************
//    Description:
//      analysis the error reason of Select instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleSelectCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_INVALIDE_FILE_ID:
        case SIM_STATUS_FILE_NOT_FOUND:
            ser_result = SIM_SERVICE_SELECT_NO_SUCH_FILE;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        case SIM_STATUS_SEL_FILE_INVALIDE:
            ser_result = SIM_SERVICE_INVALIDATE_EF_ERROR;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Status instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleStatusCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}

/**********************************************************************
//    Description:
//      Handle the command error that will happen to every command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleCommonCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word  //the status word
)
{
    //SCI_TRACE_LOW:"SIM::Status_word = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_1302_112_2_17_22_59_18_4497, (uint8 *)"d", status_word);

    switch(status_word)
    {
        case SIM_STATUS_UNKNOW_FAIL_REASON:
            return SIM_SERVICE_INSTR_TECHNICAL_PROBLEM;
            //the below error should not appear,or the code it's self is wrong
        case SIM_STATUS_P3_INCORRECT:
            return SIM_SERVICE_INSTR_P3_ERROR;
        case SIM_STATUS_P1_OR_P2_INCORRECT:
            return SIM_SERVICE_INSTR_P2_P1_ERROR;
        case SIM_STATUS_UNKNOW_INSTRUCTION_CODE:
            return SIM_SERVICE_INSTR_UNKNOW_CODE;
        case SIM_STATUS_WRONG_INSTRUCTION_CLASS:
            return SIM_SERVICE_INSTR_CLASS_ERROR;
        default:
            return SIM_SERVICE_ERROR_UNKNOW;
    }
}

/**********************************************************************
//    Description:
//      analysis the error reason of Update Binary instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleUpdateBinaryCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_INVALIDATION_CONTRADICTION:
            ser_result = SIM_SERVICE_CONTRADICT_VALIDATION;
            break;
        case SIM_STATUS_ACCESS_FAIL:
            ser_result = GetAccFailResult(cur_sim_task, access_condition);
            break;
        case SIM_STATUS_INCONSISTENT_FILE:
            ser_result = SIM_SERVICE_INCONSISTENT_FILE;
            break;
        case SIM_STATUS_NO_EF_SELECETED:
            ser_result = SIM_SERVICE_NO_EF_SELECTED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        case SIM_STATUS_SECSTATUS_NOT_SATISFIED:
            ser_result = SIM_SEC_STATUS_NOT_SATISFIED;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Update Record instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleUpdateRecordCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_INVALIDE_ADDRESS:
            ser_result = SIM_SERVICE_INVALIDE_ADDRESS;
            break;
        case SIM_STATUS_INVALIDATION_CONTRADICTION:
            ser_result = SIM_SERVICE_CONTRADICT_VALIDATION;
            break;
        case SIM_STATUS_ACCESS_FAIL:
            ser_result = GetAccFailResult(cur_sim_task, access_condition);
            break;
        case SIM_STATUS_INCONSISTENT_FILE:
            ser_result = SIM_SERVICE_INCONSISTENT_FILE;
            break;
        case SIM_STATUS_NO_EF_SELECETED:
            ser_result = SIM_SERVICE_NO_EF_SELECTED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        case SIM_STATUS_SECSTATUS_NOT_SATISFIED:
            ser_result = SIM_SEC_STATUS_NOT_SATISFIED;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Read Binary instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleReadBinaryCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_INVALIDATION_CONTRADICTION:
            ser_result = SIM_SERVICE_CONTRADICT_VALIDATION;
            break;
        case SIM_STATUS_ACCESS_FAIL:
            ser_result = GetAccFailResult(cur_sim_task, access_condition);
            break;
        case SIM_STATUS_INCONSISTENT_FILE:
            ser_result = SIM_SERVICE_INCONSISTENT_FILE;
            break;
        case SIM_STATUS_NO_EF_SELECETED:
            ser_result = SIM_SERVICE_NO_EF_SELECTED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        case SIM_STATUS_SECSTATUS_NOT_SATISFIED:
            ser_result = SIM_SEC_STATUS_NOT_SATISFIED;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Read Record instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleReadRecordCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_INVALIDE_ADDRESS:
            ser_result = SIM_SERVICE_INVALIDE_ADDRESS;
            break;
        case SIM_STATUS_INVALIDATION_CONTRADICTION:
            ser_result = SIM_SERVICE_CONTRADICT_VALIDATION;
            break;
        case SIM_STATUS_ACCESS_FAIL:
            ser_result = GetAccFailResult(cur_sim_task, access_condition);
            break;
        case SIM_STATUS_INCONSISTENT_FILE:
            ser_result = SIM_SERVICE_INCONSISTENT_FILE;
            break;
        case SIM_STATUS_NO_EF_SELECETED:
            ser_result = SIM_SERVICE_NO_EF_SELECTED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        case SIM_STATUS_SECSTATUS_NOT_SATISFIED:
            ser_result = SIM_SEC_STATUS_NOT_SATISFIED;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Seek instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleSeekCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_INVALIDE_FILE_ID:
            ser_result = SIM_SERVICE_SEEK_NOT_FIND_PATTERN;
            break;
        case SIM_STATUS_INVALIDATION_CONTRADICTION:
            ser_result = SIM_SERVICE_CONTRADICT_VALIDATION;
            break;
        case SIM_STATUS_ACCESS_FAIL:
            ser_result = GetAccFailResult(cur_sim_task, access_condition);
            break;
        case SIM_STATUS_INCONSISTENT_FILE:
            ser_result = SIM_SERVICE_INCONSISTENT_FILE;
            break;
        case SIM_STATUS_NO_EF_SELECETED:
            ser_result = SIM_SERVICE_NO_EF_SELECTED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        case SIM_STATUS_SECSTATUS_NOT_SATISFIED:
            ser_result = SIM_SEC_STATUS_NOT_SATISFIED;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Increase instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleIncreaseCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_MAX_RECORD_VALUE:
            ser_result = SIM_SERVICE_INCREASE_MAX_VALUE;
            break;
        case SIM_STATUS_INVALIDATION_CONTRADICTION:
            ser_result = SIM_SERVICE_CONTRADICT_VALIDATION;
            break;
        case SIM_STATUS_ACCESS_FAIL:
            ser_result = GetAccFailResult(cur_sim_task, access_condition);
            break;
        case SIM_STATUS_INCONSISTENT_FILE:
            ser_result = SIM_SERVICE_INCONSISTENT_FILE;
            break;
        case SIM_STATUS_NO_EF_SELECETED:
            ser_result = SIM_SERVICE_NO_EF_SELECTED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        case SIM_STATUS_SECSTATUS_NOT_SATISFIED:
            ser_result = SIM_SEC_STATUS_NOT_SATISFIED;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Verify CHV instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleVerifyCHVCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_CHV_UNINITIALIZED:
            ser_result = SIM_SERVICE_CHV_NOT_INITIALIZED;
            break;
        case SIM_STATUS_ACCESS_FAIL:
        case SIM_STATUS_SUCCESS_WITH_PROBLEM:
            ser_result = SIM_SERVICE_CHV_VER_FAIL;
            break;
        case SIM_STATUS_CHV_CONTRADICTION:
            ser_result = SIM_SERVICE_CHV_CONTRADICT_STATUS;
            break;
        case SIM_STATUS_CHV_VER_BLOCKED:
        case SIM_STATUS_AUTH_METHOD_BLOKCED:
            ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        case SIM_STATUS_REF_DATA_NOT_FOUND:
            ser_result = SIM_SERVICE_REF_DATA_NOT_FOUND;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Change CHV instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleChangeCHVCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_CHV_UNINITIALIZED:
            ser_result = SIM_SERVICE_CHV_NOT_INITIALIZED;
            break;
        case SIM_STATUS_ACCESS_FAIL:
        case SIM_STATUS_SUCCESS_WITH_PROBLEM:
            ser_result = SIM_SERVICE_CHV_VER_FAIL;
            break;
        case SIM_STATUS_CHV_CONTRADICTION:
            ser_result = SIM_SERVICE_CHV_CONTRADICT_STATUS;
            break;
        case SIM_STATUS_CHV_VER_BLOCKED:
        case SIM_STATUS_AUTH_METHOD_BLOKCED:
            ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Disable CHV instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleDisableCHVCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_CHV_UNINITIALIZED:
            ser_result = SIM_SERVICE_CHV_NOT_INITIALIZED;
            break;
        case SIM_STATUS_ACCESS_FAIL:
        case SIM_STATUS_SUCCESS_WITH_PROBLEM:
            ser_result = SIM_SERVICE_CHV_VER_FAIL;
            break;
        case SIM_STATUS_CHV_CONTRADICTION:
            ser_result = SIM_SERVICE_CHV_CONTRADICT_STATUS;
            break;
        case SIM_STATUS_CHV_VER_BLOCKED:
        case SIM_STATUS_AUTH_METHOD_BLOKCED:
            ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Enable CHV instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleEnableCHVCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_CHV_UNINITIALIZED:
            ser_result = SIM_SERVICE_CHV_NOT_INITIALIZED;
            break;
        case SIM_STATUS_ACCESS_FAIL:
        case SIM_STATUS_SUCCESS_WITH_PROBLEM:
            ser_result = SIM_SERVICE_CHV_VER_FAIL;
            break;
        case SIM_STATUS_CHV_CONTRADICTION:
            ser_result = SIM_SERVICE_CHV_CONTRADICT_STATUS;
            break;
        case SIM_STATUS_CHV_VER_BLOCKED:
        case SIM_STATUS_AUTH_METHOD_BLOKCED:
            ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Unblock CHV instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleUnblockCHVCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_CHV_UNINITIALIZED:
            ser_result = SIM_SERVICE_CHV_NOT_INITIALIZED;
            break;
        case SIM_STATUS_ACCESS_FAIL:
        case SIM_STATUS_SUCCESS_WITH_PROBLEM:
            ser_result = SIM_SERVICE_CHV_VER_FAIL;
            break;
        case SIM_STATUS_CHV_CONTRADICTION:
            ser_result = SIM_SERVICE_CHV_CONTRADICT_STATUS;
            break;
        case SIM_STATUS_CHV_VER_BLOCKED:
        case SIM_STATUS_AUTH_METHOD_BLOKCED:
            ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}

/**********************************************************************
//    Description:
//      analysis the error reason of Invalidate instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleInvalidateCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_INVALIDATION_CONTRADICTION:
            ser_result = SIM_SERVICE_CONTRADICT_VALIDATION;
            break;
        case SIM_STATUS_ACCESS_FAIL:
            ser_result = GetAccFailResult(cur_sim_task, access_condition);
            break;
        case SIM_STATUS_NO_EF_SELECETED:
            ser_result = SIM_SERVICE_NO_EF_SELECTED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        case SIM_STATUS_SECSTATUS_NOT_SATISFIED:
            ser_result = SIM_SEC_STATUS_NOT_SATISFIED;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Rehabiliate instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleRehabilitateCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_INVALIDATION_CONTRADICTION:
            ser_result = SIM_SERVICE_CONTRADICT_VALIDATION;
            break;
        case SIM_STATUS_ACCESS_FAIL:
            ser_result = GetAccFailResult(cur_sim_task, access_condition);
            break;
        case SIM_STATUS_NO_EF_SELECETED:
            ser_result = SIM_SERVICE_NO_EF_SELECTED;
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        case SIM_STATUS_SECSTATUS_NOT_SATISFIED:
            ser_result = SIM_SEC_STATUS_NOT_SATISFIED;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Run GSM Algorithm instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleGSMAlgorithmCommandError(   //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_INCONSISTENT_FILE:
            ser_result = SIM_SERVICE_GSM_ALGORITHM_ERROR_DF;
            break;
        case SIM_STATUS_ACCESS_FAIL:
            ser_result = GetAccFailResult(cur_sim_task, access_condition);
            break;
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        case SIM_STATUS_AUTH_MAC_ERROR:
            ser_result = SIM_SERVICE_RUN_ALGORITHM_MAC_ERROR;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}

/**********************************************************************
//    Description:
//      analysis the error reason of Get Reponse instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleGetResponseCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //        SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}

//the SIM application toolkit function
/**********************************************************************
//    Description:
//      analysis the error reason of Terminal Profile instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleTerminalProfileCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Envelope Command instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleEnvelopeCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Fetch Command instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleFetchCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      analysis the error reason of Termianl Profile instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleTerminalResponseCommandError(   //the error reason of the command
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //handle the error reason
    switch(status_word)
    {
        case SIM_STATUS_MEMORY_PROBLEM:
            ser_result = SIM_SERVICE_MEMORY_PROBLEM;
            break;
        default:
            ser_result = HandleCommonCommandError(status_word);
            //SCI_ASSERT(FALSE);
            break;
    }

    return ser_result;
}

/**********************************************************************
//    Description:
//      analysis the error reason of Command instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E HandleCommandsError(    //the error reason of the command
    uint32 cur_sim_task,
    SIMINSTR_NAME_E instr_name, //the isntruction name
    SIMINSTR_RESULT_TYPE_E status_word,  //the status word
    SIM_ACCESS_CONDITION_E access_condition    //the access condition
)
{
    if(status_word == SIM_STATUS_ERROR_DATA)
    {
        return SIM_SERVICE_CARD_UNRESPONSE;
    }

    switch(instr_name)
    {
        case SIMINSTR_SELECT:
            return HandleSelectCommandError(status_word, access_condition);
        case SIMINSTR_STATUS:
            return HandleStatusCommandError(status_word, access_condition);
        case SIMINSTR_READ_BINARY:
            return HandleReadBinaryCommandError(cur_sim_task, status_word, access_condition);
        case SIMINSTR_UPDATE_BINARY:
            return HandleUpdateBinaryCommandError(cur_sim_task, status_word, access_condition);
        case SIMINSTR_READ_RECORD:
            return HandleReadRecordCommandError(cur_sim_task, status_word, access_condition);
        case SIMINSTR_UPDATE_RECORD:
            return HandleUpdateRecordCommandError(cur_sim_task, status_word, access_condition);
        case SIMINSTR_SEEK:
            return HandleSeekCommandError(cur_sim_task, status_word, access_condition);
        case SIMINSTR_INCREASE:
            return HandleIncreaseCommandError(cur_sim_task, status_word, access_condition);
        case SIMINSTR_VERIFY_CHV:
            return HandleVerifyCHVCommandError(status_word, access_condition);
        case SIMINSTR_CHANGE_CHV:
            return HandleChangeCHVCommandError(status_word, access_condition);
        case SIMINSTR_DISABLE_CHV:
            return HandleDisableCHVCommandError(status_word, access_condition);
        case SIMINSTR_ENABLE_CHV:
            return HandleEnableCHVCommandError(status_word, access_condition);
        case SIMINSTR_UNBLOCK_CHV:
            return HandleUnblockCHVCommandError(status_word, access_condition);
        case SIMINSTR_INVALIDATE:
            return HandleInvalidateCommandError(cur_sim_task, status_word, access_condition);
        case SIMINSTR_REHABILITATE:
            return HandleRehabilitateCommandError(cur_sim_task, status_word, access_condition);
        case SIMINSTR_RUN_ALGORITHM:
            return HandleGSMAlgorithmCommandError(cur_sim_task, status_word, access_condition);
        case SIMINSTR_GET_RESPONSE:
            return HandleGetResponseCommandError(status_word, access_condition);
            //the SIM application command
        case SIMINSTR_TERMINAL_PROFILE:
            return HandleTerminalProfileCommandError(status_word, access_condition);
        case SIMINSTR_ENVELOPE:
            return HandleEnvelopeCommandError(status_word, access_condition);
        case SIMINSTR_FETCH:
            return HandleFetchCommandError(status_word, access_condition);
        case SIMINSTR_TERMINAL_RESPONSE:
            return HandleTerminalResponseCommandError(status_word, access_condition);

        default:
            SCI_TRACE_HIGH("SIM: HandleCommandsError instr_name = %d", instr_name);/*assert verified */
            break;
    }

    return SIM_SERVICE_ERROR_UNKNOW;
}


/**********************************************************************
//    Description:
//      Clear all the instructions in the instruction list
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ClearInstrList(uint32 cur_sim_task)
{
    s_sim_instruction_list[cur_sim_task].cur_instr = 0;
    s_sim_instruction_list[cur_sim_task].instr_num = 0;
    ClearInstrDataBuf(cur_sim_task);
}

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
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    uint16 file_id = 0x3f00;
    uint16 data_len = sizeof(SIM_RESPONSE_DATA_OF_SELECT_T);
    int8 instr_num = 0;
    int8 i = 0;//this must be init alone. wuding
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_RESPONSE_IND_T *instr_response;
    SIMINSTR_RESULT_TYPE_E instr_result;
    /*@zhangyongxia,CR4436,2003-9-2*/
    SIM_FILE_NAME_E file_route[SIM_FILE_LEVEL_E_NUM*2] = {0};
#ifdef SIM_3G_USIM_SUPPORT
    uint8 pin_remain_times = 0;
    SIM_FCP_REF_ARR_T arr_info;
#endif

    SCI_ASSERT(NULL != select_response);/*assert verified: Null_pointer*/

    //if the current file is just the file you want to select,
    //you don't need to select it again
    if((SIMFS_GetCurFileName(cur_sim_task) == file_name)
        && (SIMFS_GetFileID(cur_sim_task, file_name) == s_sim_cur_file_response[cur_sim_task].file_id))
    {
        SCI_MEMCPY(select_response, &s_sim_cur_file_response[cur_sim_task], sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
        // copy the related SW and response string
        s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];
        return SIM_SERVICE_OK;
    }

    //SCI_TRACE_LOW("SIM to SELECT file %d, current_file is %d, file_id 0x%x, cur_file_id 0x%x",file_name,SIMFS_GetCurFileName(cur_sim_task),SIMFS_GetFileID(cur_sim_task,file_name),s_sim_cur_file_response[cur_sim_task].file_id);

    //add all the select instruction that needed to select desitination
    //file to the instruction list
    instr_num = SIMFS_SelectFileInstr(cur_sim_task, file_name, file_route);

    //can not select the file
    if(instr_num <= 0)
    {
        if(-1 == instr_num)
        {
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw1 = 0x94;
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw2 = 0x4;
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.data_len = 0;
            s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];

            return SIM_SERVICE_CARD_NOT_ACTIVE;
        }

        if(-2 == instr_num)
        {
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw1 = 0x94;
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw2 = 0x4;
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.data_len = 0;
            s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];

            return SIM_SERVICE_INVALIDATE_FILE_NAME;
        }
    }

    //add all the select instruction code coresponding the file int
    //the file route to instruction list
    while(i < instr_num)
    {
        //get the file id
        file_id = SIMFS_GetFileID(cur_sim_task, file_route[i]);

        if((0x7000 == (file_id & 0xF000))
            || (0x6000 == (file_id & 0xF000))
            || (0x5000 == (file_id & 0xF000))
            || ((0x4000 == (file_id & 0xF000)) && (0xFF != (file_id & 0xFF)))
            || (0x3000 == (file_id & 0xF000))
            || (0x2000 == (file_id & 0xF000)))
        {
        }
        else
        {
            ClearInstrList(cur_sim_task);
            return SIM_SERVICE_SELECT_NO_SUCH_FILE;
        }

        //add a select instruction to the instruction list
        GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                           SIMINSTR_SELECT);

#ifdef SIM_3G_USIM_SUPPORT

        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[3] = SIM_SELECT_CMD_P2_RET_FCP_TEMPLATE;
        }

#endif

#ifdef SIM_3G_USIM_SUPPORT

        //copy the file id to the select instruction data
        // 1, To SELECT ADFusim
        // 2, To SELECT a DF/EF under ADFusim, we should SELECT ADFusim firstly.
        if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task]) && (SIM_FILE_ADF_USIM == file_route[i]))
        {
            if(s_sim_usim_adf_file_id_flag[cur_sim_task])
            {
                //We've got the file id of ADFusim in FCP. So we can SELECT by file id, not by full file name.
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, 2);
                SCI_ASSERT(NULL != s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data);/*assert verified: Null_pointer*/
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[2] = SIM_SELECT_CMD_P1_BY_FID;
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[4] = 2;
                //the ADFusim file id is 0x7FFF, 3G guys told me.
                //And we find out an India BSNL usim is 0x7FAA in FCP, but it can only be SELECTed by 0x7FFF successfully.
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[0] = 0x7F;
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[1] = 0xFF;
            }
            else
            {
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, s_sim_usim_aid[cur_sim_task].len);
                SCI_ASSERT(NULL != s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data);/*assert verified: Null_pointer*/
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[2] = SIM_SELECT_CMD_P1_BY_DF_NAME;
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[4] = s_sim_usim_aid[cur_sim_task].len;
                SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, s_sim_usim_aid[cur_sim_task].value, s_sim_usim_aid[cur_sim_task].len);
            }
        }
        else
#endif
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, 2);
            ///make sure the memory is alloced
            SCI_ASSERT(NULL != s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data);/*assert verified: Null_pointer*/
            //Raislin.Kong 2002-8-14
            //add the transition of the fileid(uint16) to 2 byte uint8,not just memcpy it to the buf.
            //why do this is to make the sim task work in both little ended and big ended system
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[0] = (uint8)(file_id >> 8);
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[1] = (uint8)file_id;
            //the expected response data length from the SIM card may decided by the transmit protocol of the SIM card
        }

        if(TB_SIM_T_0_PROTOCOL == g_sim_card_setting[cur_sim_task].t_protocol)
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 0;
        }
        else
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = SIM_MAN_DF_SELECT_DATA_LEN;
        }

#ifdef SIM_3G_USIM_SUPPORT

        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            if(SIM_SELECT_CMD_P2_NO_DATA_RETURNED == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[3])
            {
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 0;//only SW1 SW2 will return
            }
            else
            {
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 256;//max
            }
        }

#endif

        s_sim_instruction_list[cur_sim_task].instr_num++;
        i++;
    }

    i = 0;      //back the index of the file route

    //SCI_TRACE_LOW:"SIM SELECT %d, %d %d, file_route 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_2338_112_2_17_22_59_20_4498, (uint8 *)"ddddddddddd", file_name, s_sim_instruction_list[cur_sim_task].cur_instr, s_sim_instruction_list[cur_sim_task].instr_num, file_route[0], file_route[1], file_route[2], file_route[3], file_route[4], file_route[5], file_route[6], file_route[7]);

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, file_name, instr_response);

        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result) &&
            (SIM_STATUS_OK_RESEND_ADPU != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);

            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //if a select instruction is successful
        if(SIMINSTR_SELECT == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name)
        {
            //update the cur select file
            SIMFS_SetCurDirectoryFile(cur_sim_task, file_route[i]);

            //update the cur select file
            SIMFS_SetCurFileName(cur_sim_task, file_route[i]);

            i++;
        }

        if((SIM_STATUS_OK_PROACTIVE_INFO == instr_result) && (instr_response->data_len == 0))
        {
            //SCI_TRACE_LOW:"SIM::sw is 91xx when do select instruction."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_2392_112_2_17_22_59_20_4499, (uint8 *)"");
            s_sim_instruction_list[cur_sim_task].instr_num++;    //first do the fetch instruction
        }

        if(SIM_STATUS_OK_RESEND_ADPU == instr_result)
        {
            //Re-Send the current instruction, with the new P3.
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].cur_instr--;//the current instruction should redo
        }

        //the last insturction return value is OK and have reponse data need to
        //be get by the GET RESPONSE instruction,so add the GET RESPONSE
        //instruction to the instruction list and set the P3 of GET RESPONSE instruction
        if (SIM_STATUS_OK_RESPONSE_DATA == instr_result)
        {
            if (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr)
            {
                GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                                   SIMINSTR_GET_RESPONSE);
                s_sim_instruction_list[cur_sim_task].instr_num++;

                //set the P3 of GET RESPONSE instruction
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
            }
#ifdef SIM_3G_USIM_SUPPORT
            else
            {
                if ((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                    && ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1)))//USIM special
                {
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name = SIMINSTR_GET_RESPONSE; 
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_type = SIMHAL_SIMINSTR_RECV_DATA;
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_INS] = 0xC0;
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P1] = 0;
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P2] = 0;
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P3] = 0;
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.rx_expected_len = instr_response->sw2;
                    s_sim_instruction_list[cur_sim_task].cur_instr--;
                    SCI_TRACE_LOW("SIM%d:SIMINSTR_SelectFile SELECT Get RSP. %d %d",cur_sim_task,\
                        s_sim_instruction_list[cur_sim_task].cur_instr,s_sim_instruction_list[cur_sim_task].instr_num);
                    continue;
                }
            }
#endif
        }

        //if the instruction is the last one of the instruction list
        //get the select structure data from the response data
        if(s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr)
        {
#ifdef SIM_3G_USIM_SUPPORT

            if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
            {
                //check the select respone's vailidation
                if(instr_response->data_len < SIM_SELECT_FILE_TYPE_POS)
                {
                    //free the response signal
                    SCI_FREE_SIGNAL(instr_response);
                    //clear the instruction list for next use
                    ClearInstrList(cur_sim_task);
                    return SIM_SERVICE_SELECT_DATA_ERROR;
                }

                //the file is a MF or DF
                if((SIM_FILE_T_MF == instr_response->data[SIM_SELECT_FILE_TYPE_POS - 1]) ||
                    (SIM_FILE_T_DF == instr_response->data[SIM_SELECT_FILE_TYPE_POS - 1]))
                {
                    //the response len less than the mandatory length of the
                    //select file response of a MF or DF,the response is invalidate
                    if(instr_response->data_len < SIM_SELECT_DF_MANDATORY_LEN)
                    {
                        //free the response signal
                        SCI_FREE_SIGNAL(instr_response);
                        //clear the instruction list for next use
                        ClearInstrList(cur_sim_task);
                        return SIM_SERVICE_SELECT_DATA_ERROR;
                    }
                }
                //the file is EF
                else if(SIM_FILE_T_EF == instr_response->data[SIM_SELECT_FILE_TYPE_POS - 1])
                {
                    //the response len less than the mandatory length of the
                    //select file response of a EF,the response is invalidate
                    if(instr_response->data_len < SIM_SELECT_EF_MANDATORY_LEN)
                    {
                        //free the response signal
                        SCI_FREE_SIGNAL(instr_response);
                        //clear the instruction list for next use
                        ClearInstrList(cur_sim_task);
                        return SIM_SERVICE_SELECT_DATA_ERROR;
                    }
                }
                //the file type is worng,the select response is invalidate
                else
                {
                    //free the response signal
                    SCI_FREE_SIGNAL(instr_response);
                    //clear the instruction list for next use
                    ClearInstrList(cur_sim_task);
                    return SIM_SERVICE_SELECT_DATA_ERROR;
                }

                //get the min value of length in instr response and the select data structure data len
                //used to copy the need data to the select data structure
                if(instr_response->data_len < sizeof(SIM_RESPONSE_DATA_OF_SELECT_T))
                {
                    data_len = instr_response->data_len;
                }

                //copy the output data to the select structure
                SCI_ASSERT(data_len <= sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));/*assert verified: Check_value */

                SIMINSTR_RemapSelectResposne(select_response, instr_response->data, data_len);

                //A Guangdong Unicom SIM card's EFkc's len is 0xFFFF. wuding
                if((SIM_FILE_EF_KC == file_name) && (select_response->freespace_filesize > 9))
                {
                    select_response->freespace_filesize = 9;//Length 9 is defined in the spec 11.11.
                }

                SCI_MEMCPY(&s_sim_cur_file_response[cur_sim_task], select_response, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
                s_select_sw_and_rsp[cur_sim_task] = s_sim_rsp_info[cur_sim_task];
            }

#ifdef SIM_3G_USIM_SUPPORT
            else// 3G USIM card FCP decode
            {
                if(SIM_SELECT_CMD_P2_NO_DATA_RETURNED == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[3])
                {
                    //No FCP data return, do nothing.
                }
                else
                {
                    if(SIMINSTR_DecodeUSIMFCP(cur_sim_task, file_name, instr_response->data, instr_response->data_len, &arr_info, select_response))
                    {
                        //A Guangdong 2G Unicom SIM card's EFkc's len is 0xFFFF. I'm afraid USIM has the same case. wuding
                        if((SIM_FILE_EF_USIM_GSM_KC == file_name) && (select_response->freespace_filesize > 9))
                        {
                            select_response->freespace_filesize = 9;//Length 9 is defined in the spec 11.11.
                        }

                        //For some DF/EF need specially operation, we should do here.
                        // 1, Check the CHV1/2 's remain times.
                        // 2, unblock pin status and remain times.
                        // Only VERIFY can get the remain times and the info of unblock pin.
                        // We do this operation only the first time
                        if(SIM_FILE_ADF_USIM == file_name)
                        {
                            select_response->chv1_status &= 0xF0;//clear before re-set.
                            select_response->chv2_status &= 0xF0;//clear before re-set.
                            select_response->unblock_chv1_status &= 0xF0;//clear before re-set.
                            select_response->unblock_chv2_status &= 0xF0;//clear before re-set.

                            //For first time select ADF, we have stored the PIN/UNBLOCKED false-remain times,
                            //so afterwards we just get the value from the global variables
                            if(!s_sim_gain_chv_remain_times_flag[cur_sim_task])
                            {
                                //we must clear it
                                s_sim_instruction_list[cur_sim_task].instr_num = 0;
                                s_sim_instruction_list[cur_sim_task].cur_instr = 0;
                                ser_result = SIMINSTR_USIMGetCHVRemainTimes(cur_sim_task, FALSE, SIM_CHV1, &pin_remain_times);

#ifdef GSM_CUSTOMER_AFP_SUPPORT
                                //Special case
                                //SCI_TRACE_LOW("SIM: special case %d == %d",ser_result,SIM_SERVICE_REF_DATA_NOT_FOUND);
                                if (SIM_SERVICE_REF_DATA_NOT_FOUND == ser_result)
                                {
            						SIMFS_SetCurFileName(cur_sim_task,file_name);
            						SIMFS_SetCurDirectoryFile(cur_sim_task,file_name);
            						SCI_MEMCPY(&s_sim_cur_file_response[cur_sim_task],select_response,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
            						s_select_sw_and_rsp[cur_sim_task] = s_sim_rsp_info[cur_sim_task];//for CRSM

            						SCI_FREE_SIGNAL(instr_response);
            						ClearInstrList(cur_sim_task);//clear the instruction list for next use
            						return SIM_SERVICE_REF_DATA_NOT_FOUND;
                                }
                                //Special case
#endif

                                if(SIM_SERVICE_OK == ser_result)
                                {
                                    select_response->chv1_status |= (pin_remain_times & 0xF);

                                    if(0 == pin_remain_times)
                                    {
                                        select_response->unblock_chv1_status |= BIT_7;
                                    }

                                    //SCI_TRACE_LOW("SIM: end PIN1 check 0x%x, %d",select_response->chv1_status,pin_remain_times);
                                }

                                if(SIM_SERVICE_OK == ser_result)
                                {
                                    ser_result = SIMINSTR_USIMGetCHVRemainTimes(cur_sim_task, FALSE, SIM_CHV2, &pin_remain_times);

                                    if(SIM_SERVICE_OK == ser_result)
                                    {
                                        select_response->chv2_status |= (pin_remain_times & 0xF);

                                        if(0 == pin_remain_times)
                                        {
                                            select_response->unblock_chv2_status |= BIT_7;
                                        }

                                        //SCI_TRACE_LOW("SIM: end PIN2 check 0x%x, %d",select_response->chv2_status,pin_remain_times);
                                    }
                                }

                                if(SIM_SERVICE_OK == ser_result)
                                {
                                    ser_result = SIMINSTR_USIMGetCHVRemainTimes(cur_sim_task, TRUE, SIM_CHV1, &pin_remain_times);

                                    if(SIM_SERVICE_OK == ser_result)
                                    {
                                        select_response->unblock_chv1_status |= (pin_remain_times & 0xF);
                                        //SCI_TRACE_LOW("SIM: end UNBLOCK PIN1 check 0x%x, %d",select_response->unblock_chv1_status,pin_remain_times);
                                    }
                                }

                                if(SIM_SERVICE_OK == ser_result)
                                {
                                    ser_result = SIMINSTR_USIMGetCHVRemainTimes(cur_sim_task, TRUE, SIM_CHV2, &pin_remain_times);

                                    if(SIM_SERVICE_OK == ser_result)
                                    {
                                        select_response->unblock_chv2_status |= (pin_remain_times & 0xF);
                                        //SCI_TRACE_LOW("SIM: end UNBLOCK PIN2 check 0x%x, %d",select_response->unblock_chv2_status,pin_remain_times);
                                    }
                                }

                                if (SIM_SERVICE_OK != ser_result)
                                {
                                    SCI_TRACE_LOW("SIM%d::SIMINSTR_USIMGetCHVRemainTimes failed for ser_result=%d.",ser_result);
            						SIMFS_SetCurFileName(cur_sim_task,file_name);
            						SIMFS_SetCurDirectoryFile(cur_sim_task,file_name);
            						SCI_MEMCPY(&s_sim_cur_file_response[cur_sim_task],select_response,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
            						s_select_sw_and_rsp[cur_sim_task] = s_sim_rsp_info[cur_sim_task];//for CRSM

            						SCI_FREE_SIGNAL(instr_response);
            						ClearInstrList(cur_sim_task);//clear the instruction list for next use
            						return ser_result;
                                }
                            }
                            else
                            {
                                select_response->chv1_status |= (g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain & 0xF);

                                if(0 == g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain)
                                {
                                    select_response->unblock_chv1_status |= BIT_7;
                                }

                                select_response->unblock_chv1_status |= (g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.false_remain & 0xF);

                                select_response->chv2_status |= (g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain & 0xF);

                                if(0 == g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain)
                                {
                                    select_response->unblock_chv2_status |= BIT_7;
                                }

                                select_response->unblock_chv2_status |= (g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.false_remain & 0xF);

                            }
                        }

                        //EFarr(6F06) of some EFs which SA tag is '8B'
                        //EFarr is READ always, and DF/ADF will not need to check the attribute.
#ifdef YWD_REF_ARR_SELF_TEST
                        //all the EFs need to check
                        else if((SIM_FILE_EF_USIM_ARR != file_name) && (SIM_FILE_EF_MF_ARR != file_name) && (SIM_FILE_EF_TELECOM_ARR != file_name)
                                && (file_name >= SIM_FILE_EF_ICCID))
#else
                        //only several EFs need to check
                        else if((SIM_FILE_EF_USIM_ACM == file_name) || (SIM_FILE_EF_USIM_ACMMAX == file_name) || (SIM_FILE_EF_USIM_PUCT == file_name)
                                || (SIM_FILE_EF_USIM_VGCSS == file_name) || (SIM_FILE_EF_USIM_VBSS == file_name))
#endif
                        {
                            if(arr_info.is_ref_exist)
                            {
                                //We want to get the EFacm/acmmax/puct/vgcss/vbss's access condition for UPDATE.
                                // Is the access condition is PIN/PIN2 , PIN/ADM?
                                // In fact, EFict,EFoct,EFmsucp,EFmsisdn(under ADFusim) are also need. But we donnot support these EFs now.
                                if(!SIMINSTR_GetARR(cur_sim_task, file_name, arr_info, &(select_response->access_condition1), &(select_response->access_condition2)))
                                {
                                    //If we decode EFarr fail, we set this as default.
                                    select_response->access_condition1 = 0x0444;//INCREASE,RFU,UPDATE is ADM, READ is ALW
                                    select_response->access_condition2 = 0x44;//ACTIVE/DEACTIVE is ADM
                                }
                            }
                        }

                        //update the cur select file For 3G USIM maybe SELECT EFarr...
                        SIMFS_SetCurFileName(cur_sim_task, file_name);
                        SIMFS_SetCurDirectoryFile(cur_sim_task, file_name);
                        SCI_MEMCPY(&s_sim_cur_file_response[cur_sim_task], select_response, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
                        s_select_sw_and_rsp[cur_sim_task] = s_sim_rsp_info[cur_sim_task];//for CRSM
                    }
                    else
                    {
                        SCI_FREE_SIGNAL(instr_response);
                        ClearInstrList(cur_sim_task);//clear the instruction list for next use
                        return SIM_SERVICE_SELECT_DATA_ERROR;
                    }
                }
            }

#endif//SIM_3G_USIM_SUPPORT
        }

        if((SIM_STATUS_OK_PROACTIVE_INFO == instr_result) && (instr_response->data_len == 0))
        {
            //SCI_TRACE_LOW("SIM::redo the select when sw is 91xx.");
            s_sim_instruction_list[cur_sim_task].instr_num--;    //the fetch instruction has done
            //s_sim_instruction_list[cur_sim_task].cur_instr--;    //the current instruction should redo
#ifdef SIM_3G_USIM_SUPPORT

            if(SIM_SELECT_CMD_P2_NO_DATA_RETURNED == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[3])
            {
                //No FCP data return, do nothing.
                //s_sim_instruction_list[cur_sim_task].cur_instr++;//the current instruction do not need to redo
            }
            else
#endif
            {
                s_sim_instruction_list[cur_sim_task].cur_instr--;    //the current instruction should redo
            }

            //SCI_TRACE_LOW:"SIM::redo the select when sw is 91xx. %d %d... 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_2666_112_2_17_22_59_21_4500, (uint8 *)"ddd", s_sim_instruction_list[cur_sim_task].instr_num, s_sim_instruction_list[cur_sim_task].cur_instr, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[3]);
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_SelectFile(cur_sim_task, file_name, select_response);
#endif
}

/*****************************************************************************/
//  Description : save the response information
//  Global resource dependence :
//  Author: Tracy.zhang
//  Note: used for CRSM
/*****************************************************************************/
STATIC void SIMINSTR_CRSM_SaveResponseInfo(
    uint32                  cur_sim_task,   //IN:
    SIM_FILE_NAME_E         file_name,      //IN:
    SIM_SIG_RESPONSE_IND_T  *instr_response //IN:
)
{
    SIM_RSP_INFO_T  *info_ptr = &(s_sim_rsp_info[cur_sim_task]);
    SIMINSTR_LIST_T *list_ptr = &(s_sim_instruction_list[cur_sim_task]);

    info_ptr->file_name = file_name;
    info_ptr->ins_name  = list_ptr->instr_list[list_ptr->cur_instr].instr_name;

    info_ptr->sw_rsp.sw1 = instr_response->sw1;
    info_ptr->sw_rsp.sw2 = instr_response->sw2;

    //wuding: 如果是READ 的EF 长度是大于256 的，应该是需要CRSM
    //自己来读两次。而我们的sim_instruction.c 又会自动把全部读出来。
    //所以这里截一下好了。
    //CRSM的流程应该是先发GetResponse，然后根据大小自己发READ BINARY 过来。
    if(instr_response->data_len > 512) //memory check
    {
        info_ptr->sw_rsp.data_len = 512;
    }
    else
    {
        info_ptr->sw_rsp.data_len = instr_response->data_len;
    }

    SCI_MEMCPY(info_ptr->sw_rsp.data,
               instr_response->data,
               info_ptr->sw_rsp.data_len);

    SCI_TRACE_LOW("SIM%d:file_name=%d, ins_name=%d, sw1=%x, sw2=%x, data_len=%d, data=0x%x 0x%x 0x%x 0x%x", cur_sim_task, file_name, info_ptr->ins_name, info_ptr->sw_rsp.sw1, info_ptr->sw_rsp.sw2, info_ptr->sw_rsp.data_len, info_ptr->sw_rsp.data[0], info_ptr->sw_rsp.data[1], info_ptr->sw_rsp.data[2], info_ptr->sw_rsp.data[3]);
}

#ifdef MODEM_PLATFORM
/*****************************************************************************/
//  Description : get the response information
//  Global resource dependence :
//  Author: Tracy.zhang
//  Note: used for CRSM
/*****************************************************************************/
void SIMINSTR_CRSM_GetResponseInfo(
    uint32 cur_sim_task,
    SIM_RSP_INFO_T    *info_ptr
)
{
    SCI_MEMCPY(info_ptr, &(s_sim_rsp_info[cur_sim_task]), sizeof(SIM_RSP_INFO_T));
    //SCI_TRACE_LOW:"SIM%d:get response: ins_name=%d, sw1=%x, sw2=%x, data_len=%d, data=%x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_2740_112_2_17_22_59_21_4502, (uint8 *)"dddddddd", cur_sim_task, info_ptr->ins_name, info_ptr->sw_rsp.sw1, info_ptr->sw_rsp.sw2, info_ptr->sw_rsp.data_len, info_ptr->sw_rsp.data[0], info_ptr->sw_rsp.data[1], info_ptr->sw_rsp.data[2]);
}
#endif

/**********************************************************************
//    Description:
//      save a service request signal into the temp queue when the SIM
//    layer is interactive with the SIM driver.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void TempServiceReq(
    uint32 cur_sim_task,
    xSignalHeaderRec *psig
)
{
    xSignalHeaderRec *pqueue = NULL, *ptemp = NULL;
    BOOLEAN is_new_signal_has_insterted = FALSE;

    //if the service request signal is from the MM layer,put it to the head
    //of the temp queue to make it be processed first
    if(
#ifdef _SUPPORT_GPRS_
        (GMM_SIM_AUTH_REQ == psig->SignalCode) ||
#endif
        (MM_SIM_DATA_REQ == psig->SignalCode) ||
#ifdef _SUPPORT_GPRS_
        (GMM_SIM_SAVE_CIPHER_INFO == psig->SignalCode) ||
        (GMM_SIM_SAVE_RAI_INFO == psig->SignalCode) ||
#endif
        (MM_SIM_FPLMN_UPD_REQ == psig->SignalCode) ||
        (MM_SIM_CIPHER_UPD_REQ == psig->SignalCode) ||
        (MM_SIM_STATUS_UPD_REQ == psig->SignalCode) ||
        (MM_SIM_BALIST_UPD_REQ == psig->SignalCode) ||
        (MM_SIM_AUTH_REQ == psig->SignalCode) ||
        (SIMAT_TERMINAL_RESPONSE_REQ == psig->SignalCode))
    {
#ifdef SIM_DROP_RESTORE_SUPPORT
        if (MM_SIM_AUTH_REQ == psig->SignalCode)
        {
            if (SIMMMSIG_IsInAuthentication(cur_sim_task))
            {
                SIMINSTR_SetAuthInAuthFlag(cur_sim_task,TRUE);
            }

            SCI_TRACE_LOW("SIM%d: TempServiceReq receive AUTH",cur_sim_task);
        }
#ifdef _SUPPORT_GPRS_
        else if (GMM_SIM_AUTH_REQ == psig->SignalCode)
        {
            if (SIMMMSIG_IsInGMMAuthentication(cur_sim_task))
            {
                SIMINSTR_SetGMMAuthInAuthFlag(cur_sim_task,TRUE);
            }

            SCI_TRACE_LOW("SIM%d: TempServiceReq receive GMM AUTH",cur_sim_task);
        }
#endif        
#endif

        //insert the new signal to the new temp queue,and copy the signal
        //from the old temp queue to the new temp queue
        SCI_SIGLIST_INIT(&pqueue);
        ptemp = SCI_SIGLIST_GETHEAD(&s_temp_signal_queue[cur_sim_task]);

        while(NULL != ptemp)
        {
            //if the new signal has inserted into the new temp queue,so copy the signal from the old queue
            //to the new queue tail directly
            if(is_new_signal_has_insterted)
            {
#ifdef SIM_DROP_RESTORE_SUPPORT
                if (SIMINSTR_GetAuthInAuthFlag(cur_sim_task) 
                    && (MM_SIM_AUTH_REQ == ptemp->SignalCode))
                {
                    SCI_TRACE_LOW("SIM:TempServiceReq new_auth OK. Free the old_auth.");
                    SCI_FREE_SIGNAL(ptemp);
                }
#ifdef _SUPPORT_GPRS_
                else if (SIMINSTR_GetGMMAuthInAuthFlag(cur_sim_task) 
                    && (GMM_SIM_AUTH_REQ == ptemp->SignalCode))
                {
                    SCI_TRACE_LOW("SIM:TempServiceReq new_gmm_auth OK. Free the old_gmm_auth.");
                    SCI_FREE_SIGNAL(ptemp);
                }
#endif
                else
#endif
                {
                    SCI_SIGLIST_ADDTAIL(&pqueue,ptemp);
                }
            }
            //if the signal in the old temp queue head is important signal,so the new signal should not
            //insert before the signal,so copy the old signal to the new temp queue
            else
            {
                if(
#ifdef _SUPPORT_GPRS_
                    (GMM_SIM_AUTH_REQ == ptemp->SignalCode) ||
#endif
                    (MM_SIM_DATA_REQ == ptemp->SignalCode) ||
#ifdef _SUPPORT_GPRS_
                    (GMM_SIM_SAVE_CIPHER_INFO == ptemp->SignalCode) ||
                    (GMM_SIM_SAVE_RAI_INFO == ptemp->SignalCode) ||
#endif
                    (MM_SIM_FPLMN_UPD_REQ == ptemp->SignalCode) ||
                    (MM_SIM_CIPHER_UPD_REQ == ptemp->SignalCode) ||
                    (MM_SIM_STATUS_UPD_REQ == ptemp->SignalCode) ||
                    (MM_SIM_BALIST_UPD_REQ == ptemp->SignalCode) ||
                    (MM_SIM_AUTH_REQ == ptemp->SignalCode) ||
                    (SIMAT_TERMINAL_RESPONSE_REQ == ptemp->SignalCode))
                {
#ifdef SIM_DROP_RESTORE_SUPPORT
                    //New_auth_req is coming, while the old_auth_req is still in temp queque.
                    //We should replace the old_auth_req with new_auth_req.
                    //MM is OK for this case.
                    if ((MM_SIM_AUTH_REQ == ptemp->SignalCode) 
                        && (MM_SIM_AUTH_REQ == psig->SignalCode))
                    {
                        SCI_TRACE_LOW("SIM:TempServiceReq new_auth replace the old_req");
                        SCI_FREE_SIGNAL(ptemp);

                        SCI_SIGLIST_ADDTAIL(&pqueue,psig);
                        is_new_signal_has_insterted = TRUE;
                    }
#ifdef _SUPPORT_GPRS_
                    else if ((GMM_SIM_AUTH_REQ == ptemp->SignalCode) 
                        && (GMM_SIM_AUTH_REQ == psig->SignalCode))
                    {
                        SCI_TRACE_LOW("SIM:TempServiceReq new_gmm_auth replace the old_gmm_req");
                        SCI_FREE_SIGNAL(ptemp);

                        SCI_SIGLIST_ADDTAIL(&pqueue,psig);
                        is_new_signal_has_insterted = TRUE;
                    }
#endif
                    else
                    {
                        if (MM_SIM_AUTH_REQ == psig->SignalCode)
                        {
                            SCI_SIGLIST_ADDTAIL(&pqueue,psig);
                            is_new_signal_has_insterted = TRUE;
                        }
                        else if (GMM_SIM_AUTH_REQ == psig->SignalCode)
                        {
                            if (MM_SIM_AUTH_REQ == ptemp->SignalCode)
                            {
                                SCI_TRACE_LOW("SIM gmm_auth_req low than mm_auth_req");
                            }
                            else
                            {
                                SCI_SIGLIST_ADDTAIL(&pqueue,psig);
                                is_new_signal_has_insterted = TRUE;
                            }
                        }
#endif
                        SCI_SIGLIST_ADDTAIL(&pqueue,ptemp);
#ifdef SIM_DROP_RESTORE_SUPPORT
                    }
#endif
                }
                //if the signal in the old temp queue head is not important signal,so the new signal should
                //insert before the old signal
                else
                {
                    SCI_SIGLIST_ADDTAIL(&pqueue, psig);
                    SCI_SIGLIST_ADDTAIL(&pqueue, ptemp);
                    is_new_signal_has_insterted = TRUE;
                }
            }

            ptemp = SCI_SIGLIST_GETHEAD(&s_temp_signal_queue[cur_sim_task]);
        }

        //insert the signal at the tail
        if(!is_new_signal_has_insterted)
        {
            SCI_SIGLIST_ADDTAIL(&pqueue, psig);
            is_new_signal_has_insterted = TRUE;
        }

        //copy the signal from the new temp queue to the old temp queue
        ptemp = SCI_SIGLIST_GETHEAD(&pqueue);

        while(NULL != ptemp)
        {
            SCI_SIGLIST_ADDTAIL(&s_temp_signal_queue[cur_sim_task], ptemp);
            ptemp = SCI_SIGLIST_GETHEAD(&pqueue);
        }
    }
    //put the service request signal to the end of the temp queue to wait
    //for process
    else
    {
        SCI_SIGLIST_ADDTAIL(&s_temp_signal_queue[cur_sim_task], psig);
    }
}

/**********************************************************************
//    Description:
//      recieve a signal from the SIM drive layer.if there comes service
//    request  signals from other layer,just save it to the temporary
//    queue for laterly use.and if there comes the exception of the SIM
//    card such as Rx Parity error or Card out,report error to the SIM
//    layer and save signal to the temporary interrupt signal queue for
//    laterly signal proccess.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E SIMINSTR_ReceiveInstrResponse(   //return value:the type of the signal that send by SIM card interrupt
    uint32 cur_sim_task,
    SIM_SIG_RESPONSE_IND_T **instr_response //the pointer to save the pointer of the self response signal
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    BOOLEAN is_need_waiting = FALSE;
    uint32 null_byte_times = 0;
    uint32 unresponse_null_byte_times_limit = 15;
    uint32 exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);
    xSignalHeaderRec *sig_ptr = PNULL; /* to receive the signal */
    uint32 rx_expected_len = TB_SIM_RESPONSE_DATA_EXPEND_LEN;
    uint32 total_wait_time_value = 0;

    total_wait_time_value = SIMHAL_GetWWT(exchange_card_id);

    if(SIMHAL_SIMINSTR_RECV_DATA == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_type)
    {
        rx_expected_len = (uint32)(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.rx_expected_len + TB_SIM_RESPONSE_DATA_EXPEND_LEN);
    }
    else
    {
        rx_expected_len = TB_SIM_RESPONSE_DATA_EXPEND_LEN;//the 2 byte status words.
    }
    total_wait_time_value = total_wait_time_value * rx_expected_len;
#ifdef SIM_DROP_RESTORE_SUPPORT
    total_wait_time_value = SIMINSTR_CountWaitTime(cur_sim_task,rx_expected_len);
#endif

    if ((SIMMMSIG_IsInAuthentication(cur_sim_task)) 
        || (SIMMMSIG_IsInGMMAuthentication(cur_sim_task)))
    {
        unresponse_null_byte_times_limit = 2;
#ifdef SIM_DROP_RESTORE_SUPPORT
        unresponse_null_byte_times_limit = 0;
#endif
        //SCI_TRACE_LOW("SIM: auth set unresponse_null_byte_times_limit 2");
    }

    for(;;)
    {
        SCI_RECEIVE_SIGNAL(sig_ptr, g_sim_task_id[cur_sim_task]);
        *instr_response = (SIM_SIG_RESPONSE_IND_T *)sig_ptr;

        //the signal is a sim task layer self signal from sim layer's interrupt handle function
        if(SIM_RESPONSE_IND == (*instr_response)->SignalCode)
        {
            break;
        }
        //there are some problem with the SIM card,such as Parity error or other things
        //when the SIM task is in service
        else if(SIM_INT_IND == (*instr_response)->SignalCode)
        {
            switch(((SIM_SIG_INT_IND_T *)(*instr_response))->int_type)
            {
                case TB_SIMINT_RX_PARITY_ERROR:
                    ser_result = SIM_SERVICE_RX_PARITY_ERROR;
                    break;
                case TB_SIMINT_TX_PARITY_ERROR:
                    ser_result = SIM_SERVICE_TX_PARITY_ERROR;
                    break;
                case TB_SIMINT_CARD_OUT:
                    ser_result = SIM_SERVICE_CARD_OUT;
                    break;
                case TB_SIMINT_CARD_UNRESPONSE:
#ifndef WIN32

                    //if the null byte number is not zero,just restart the instruction timer
                    if(SIMHAL_GetNullByteNum(exchange_card_id) == 0)
                    {
                        //SCI_TRACE_LOW:"SIM::timer expired and the card do not need waiting."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_2898_112_2_17_22_59_21_4503, (uint8 *)"");
                        ser_result = SIM_SERVICE_CARD_UNRESPONSE;
                        is_need_waiting = FALSE;
                    }
                    else
                    {
                        //A M-Zone STK has an err, it will send 0x60 continuously, never stop.
                        //So I set a limit 3, or the sim task will be blocked here endlessly. wuding.
                        null_byte_times++;

                        if (null_byte_times < unresponse_null_byte_times_limit)
                        {
                            //SCI_TRACE_LOW:"SIM%d::timer expired but the card need waiting."
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_2909_112_2_17_22_59_21_4504, (uint8 *)"d", cur_sim_task);
                            //restart the instruction timer
                            ActiveInstructionTimer(cur_sim_task, total_wait_time_value);

                            is_need_waiting = TRUE;
                        }
                        else
                        {
                            //SCI_TRACE_LOW:"SIM%d::timer expired and the card do not need waiting. %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_2916_112_2_17_22_59_21_4505, (uint8 *)"dd", cur_sim_task, null_byte_times);
                            is_need_waiting = FALSE;
                            ser_result = SIM_SERVICE_CARD_UNRESPONSE;
                        }
                    }

#else
                    ser_result = SIM_SERVICE_CARD_UNRESPONSE;
#endif
                    break;
                case TB_SIMINT_NEED_WAITTING:
                    null_byte_times++;

                    //another card, STK will send 0x60 continuously, and it needs 16 times reset timer to get the PC command we want. jacky
                    if(null_byte_times < 20) //Set a limit, or the sim task will be blocked here endlessly when some card with problem. wuding.
                    {
                        //restart the instruction timer
                        ActiveInstructionTimer(cur_sim_task, total_wait_time_value);
                        is_need_waiting = TRUE;
                    }
                    else
                    {
                        if(IsInstructionTimerActive(cur_sim_task))
                        {
                            //Huawei Xi'an Lab Test. A foreign SIM send 50 NULL bytes in about 100ms,
                            //we should continue to wait if the 4S timer is active, but I think we need not re-start timer.
                            is_need_waiting = TRUE;
                        }
                        else
                        {
                            //SCI_TRACE_LOW:"SIM%d::too many 0x60, do not need waiting. %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_2944_112_2_17_22_59_21_4506, (uint8 *)"dd", cur_sim_task, null_byte_times);
                            is_need_waiting = FALSE;
                            ser_result = SIM_SERVICE_CARD_UNRESPONSE;
                        }
                    }

                    break;
#ifdef SIM_PLUG_IN_SUPPORT
                case TB_SIMINT_PLUG_IN:
                    is_need_waiting = FALSE;//to add sim-plug-in in the queue
                    ser_result = SIM_SERVICE_CARD_OUT;//do not restore.
                    break;
                case TB_SIMINT_PULL_OUT:
                    is_need_waiting = FALSE;//to add sim-pull-out in the queue
                    ser_result = SIM_SERVICE_CARD_OUT;//do not restore.
                    break;
#endif
                    //orther wise,there shoud no other interrupt that send up signal
                default:
                    //SCI_TRACE_LOW:"SIM%d::SIMINSTR_ReceiveInstrResponse default %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_2962_112_2_17_22_59_21_4507, (uint8 *)"dd", cur_sim_task, (((SIM_SIG_INT_IND_T *)(*instr_response))->int_type));
                    ser_result = SIM_SERVICE_CARD_UNRESPONSE;
                    is_need_waiting = FALSE;
                    break;
            }

            if(!is_need_waiting)
            {
#ifndef WIN32
                SIM_PrintTxBuf(cur_sim_task);
                SIM_PrintRxBuf(cur_sim_task);
#endif

                if(((((SIM_SIG_INT_IND_T *)(*instr_response))->int_type) == TB_SIMINT_CARD_UNRESPONSE) 
                    || ((((SIM_SIG_INT_IND_T *)(*instr_response))->int_type) == TB_SIMINT_RX_PARITY_ERROR)
#ifdef SIM_PLUG_IN_SUPPORT
                    || ((((SIM_SIG_INT_IND_T *)(*instr_response))->int_type) == TB_SIMINT_PLUG_IN)
                    || ((((SIM_SIG_INT_IND_T *)(*instr_response))->int_type) == TB_SIMINT_PULL_OUT)
#endif
                  )
                {
                    //save the interrupt signal to temp interrupt signal queue
                    SCI_SIGLIST_ADDTAIL(&s_temp_int_signal_queue[cur_sim_task], (xSignalHeaderRec *)(*instr_response));
                    break;
                }
                else //free the signal pointer
                {
                    SCI_FREE_SIGNAL((*instr_response));//cr7993
                    is_need_waiting = FALSE;
                }
            }
            else //free the signal pointer
            {
                SCI_FREE_SIGNAL((*instr_response));//cr7993
                is_need_waiting = FALSE;
            }
        }
        //process the SIM_POWER_OFF_REQ signal when receice command response singal from SIM driver:
        //1.power off the sim card;
        //2.return card out error as the command response
        else if(SIM_POWER_OFF_REQ == (*instr_response)->SignalCode)
        {
            #ifdef MODEM_PLATFORM
            #ifdef SIM_PLUG_IN_SUPPORT
            {
            	xSignalHeaderRec *preq = NULL;
            	SIM_SIG_CRSM_CNF_T *pcnf = NULL;
            
            	preq = SIMINSTR_IsThisSIGNALInTempSignalQueue(cur_sim_task,SIM_SIG_CRSM_REQ);
            	if (preq)
            	{
            		SCI_CREATE_SIGNAL(pcnf,SIM_SIG_CRSM_CNF,sizeof(SIM_SIG_CRSM_CNF_T),g_sim_task_id[cur_sim_task]);
            		pcnf->file_id = ((SIM_SIG_CRSM_REQ_T*)preq)->file_id;
            		pcnf->sw1 = 0x94;
            		pcnf->sw2 = 0x4;
            		pcnf->data_len = 0;
            		pcnf->result = FALSE;
            		SCI_TRACE_LOW("SIM%d: Get CRSM from temp queue. file_id=0x%x",cur_sim_task,pcnf->file_id);
            		SCI_SEND_SIGNAL((xSignalHeaderRec*)pcnf,g_sim_info_task_id[cur_sim_task]);
            
            		SCI_FREE_SIGNAL(preq);
            	}
            }
            #endif
            #endif

            SIMINSTR_ClearTempSignalQueue(cur_sim_task);
            SCI_SIGLIST_ADDTAIL(&s_temp_signal_queue[cur_sim_task], (xSignalHeaderRec *)(*instr_response));

            //SCI_TRACE_LOW:"SIM%d::Rcv Power off req in Receive Response"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_3006_112_2_17_22_59_21_4508, (uint8 *)"d", cur_sim_task);
            SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
            ser_result = SIM_SERVICE_CARD_OUT;
            return ser_result;
        }
        //else save the signal to the temp signal queue for later used
        else
        {
            TempServiceReq(cur_sim_task, (xSignalHeaderRec *)(*instr_response));
        }
    }

    return ser_result;
}

/**********************************************************************
//    Description:
//      check the temp Int signal queue to see if there any abnormal Int from sim drv
//    signal from other layer and get it.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
xSignalHeaderRec *SIMINSTR_CheckTempIntSignalQueue(uint32 cur_sim_task)//return value:the Int signal
{
    xSignalHeaderRec *psig = NULL;

    //check the interrpter temporary signal queue
    psig = SCI_SIGLIST_GETHEAD(&s_temp_int_signal_queue[cur_sim_task]);
    return psig;
}

/**********************************************************************
//    Description:
//      check the temp signal queue to see if there any service request
//    signal from other layer and get it.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
xSignalHeaderRec *SIMINSTR_CheckTempSignalQueue(uint32 cur_sim_task)   //return value:the service request signal
{
    xSignalHeaderRec *psig = NULL;

    //first,check the interrpter temporary signal queue
    psig = SCI_SIGLIST_GETHEAD(&s_temp_int_signal_queue[cur_sim_task]);

    if(NULL == psig)
    {
        //if the interrupt temporary signal queue is empty,then check
        //the temporary signal queue
        psig = SCI_SIGLIST_GETHEAD(&s_temp_signal_queue[cur_sim_task]);
    }

    return psig;
}

/**********************************************************************
//    Description:
//      Clear the temporary signal queue
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMINSTR_ClearTempSignalQueue(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig = NULL;

    psig = SCI_SIGLIST_GETHEAD(&s_temp_signal_queue[cur_sim_task]);

    while(psig)
    {
        SCI_FREE_SIGNAL(psig);
        psig = SCI_SIGLIST_GETHEAD(&s_temp_signal_queue[cur_sim_task]);
    }
}

/**********************************************************************
//    Description:
//      Check is there the signal_code in temp queue?
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
xSignalHeaderRec* SIMINSTR_IsThisSIGNALInTempSignalQueue(uint32 cur_sim_task, uint32 simsigcode)
{
    xSignalHeaderRec *psig = NULL;

    psig = SCI_SIGLIST_GETHEAD(&s_temp_signal_queue[cur_sim_task]);

    while(psig)
    {
        if(simsigcode == psig->SignalCode)
        {
            return psig;
        }

		SCI_FREE_SIGNAL(psig);//FREE

        psig = SCI_SIGLIST_GETHEAD(&s_temp_signal_queue[cur_sim_task]);
    }

    return NULL;
}

#ifdef SIM_PLUG_IN_SUPPORT
/**********************************************************************
//    Description:
//      Check is there the signal_code in temp queue?
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
TB_SIM_INT_TYPE_E SIMINSTR_LatestHotSwapMessageInTempIntSignalQueue(uint32 cur_sim_task)
{
    uint8 i = 0;
    xSignalHeaderRec *psig = NULL;
    TB_SIM_INT_TYPE_E latest_hs_int = TB_SIMINT_PULL_OUT;

    psig = SCI_SIGLIST_GETHEAD(&s_temp_int_signal_queue[cur_sim_task]);

    while(psig)
    {
        if(SIM_INT_IND == psig->SignalCode)
        {
            if((TB_SIMINT_PULL_OUT == ((SIM_SIG_INT_IND_T *)psig)->int_type)
                || (TB_SIMINT_PLUG_IN == ((SIM_SIG_INT_IND_T *)psig)->int_type))
            {
                latest_hs_int = ((SIM_SIG_INT_IND_T *)psig)->int_type;
            }
        }

		SCI_FREE_SIGNAL(psig);

        psig = SCI_SIGLIST_GETHEAD(&s_temp_int_signal_queue[cur_sim_task]);

        //double-check for dead-loop
        i++;

        if(i > 30) //I think 30 is enough
        {
            break;
        }

        //double-check for dead-loop
    }

    return latest_hs_int;
}

/**********************************************************************
//    Description:
//      add this signal in sim temp INT queue
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMINSTR_AddThisSIGNALInTempIntSignalQueue(uint32 cur_sim_task, xSignalHeaderRec *psig)
{
    SCI_SIGLIST_ADDTAIL(&s_temp_int_signal_queue[cur_sim_task], psig);
    SCI_TRACE_LOW("SIM%d:AddThisSIGNALInTempIntSignalQueue 0x%x", cur_sim_task, psig->SignalCode);
}
#endif

/**********************************************************************
//    Description:
//      add this signal in sim temp queue
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMINSTR_AddThisSIGNALInTempSignalQueue(uint32 cur_sim_task, xSignalHeaderRec *psig)
{
    SCI_SIGLIST_ADDTAIL(&s_temp_signal_queue[cur_sim_task], psig);
    //SCI_TRACE_LOW:"SIM%d:AddThisSIGNALInTempSignalQueue 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_3113_112_2_17_22_59_22_4509, (uint8 *)"dd", cur_sim_task, psig->SignalCode);
}

/**********************************************************************
//    Description:
//      Clear the temporary signal queue
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMINSTR_ClearTempIntSignalQueue(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig = NULL;

    psig = SCI_SIGLIST_GETHEAD(&s_temp_int_signal_queue[cur_sim_task]);

    while(psig)
    {
        SCI_FREE_SIGNAL(psig);
        psig = SCI_SIGLIST_GETHEAD(&s_temp_int_signal_queue[cur_sim_task]);
    }
}

/**********************************************************************
//    Description:
//      Clear the STK signal in queue before Restore.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMINSTR_ClearPCExistSignalInQueue(uint32 cur_sim_task)
{
    xSignalHeaderRec *pqueue = NULL, *ptemp = NULL;

    SCI_SIGLIST_INIT(&pqueue);//init a temp queue

    //delete SIM_PC_EXIST_IND from the old queue
    //copy other SIG into the temp queue
    ptemp = SCI_SIGLIST_GETHEAD(&s_temp_signal_queue[cur_sim_task]);

    while(NULL != ptemp)
    {
        if(SIM_PC_EXIST_IND == ptemp->SignalCode)
        {
            SCI_FREE_SIGNAL(ptemp);
        }
        else
        {
            SCI_SIGLIST_ADDTAIL(&pqueue, ptemp);
        }

        ptemp = SCI_SIGLIST_GETHEAD(&s_temp_signal_queue[cur_sim_task]);
    }

    //copy the temp queue to the old queue
    ptemp = SCI_SIGLIST_GETHEAD(&pqueue);

    while(NULL != ptemp)
    {
        SCI_SIGLIST_ADDTAIL(&s_temp_signal_queue[cur_sim_task], ptemp);
        ptemp = SCI_SIGLIST_GETHEAD(&pqueue);
    }
}

/**********************************************************************
//    Description:
//      Get the card supply voltage form the byte 14(bit5-7) of the select response of the MF
//    see GSM11.14 section 9.2.1 and GSM11.18 section 4.3
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SUPPLY_VOLT_E GetCardSupplyVot(uint8 file_character)
{
    SIM_SUPPLY_VOLT_E supply_volt = SIM_SUPPLY_VOLT_FUTURE_CLASS;

    switch((file_character & SIM_GSM_SPECIFIC_CARD_VOLT) >> 4)
    {
        case 0:
            //SCI_TRACE_LOW:"SIM::volt 5v ony. 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_3186_112_2_17_22_59_22_4510, (uint8 *)"d", file_character);
            supply_volt = SIM_SUPPLY_VOLT_5V_ONLY;
            break;
        case 1:
            //SCI_TRACE_LOW:"SIM::volt 3v tech. 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_3190_112_2_17_22_59_22_4511, (uint8 *)"d", file_character);
            supply_volt = SIM_SUPPLY_VOLT_3V_TECH;
            break;
        case 3:
            //SCI_TRACE_LOW:"SIM::volt 1.8v tech. 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_3194_112_2_17_22_59_22_4512, (uint8 *)"d", file_character);
            supply_volt = SIM_SUPPLY_VOLT_1_8V_TECH;
            break;
        case 7:
            //SCI_TRACE_LOW:"SIM::volt future class. 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_3198_112_2_17_22_59_22_4513, (uint8 *)"d", file_character);
            supply_volt = SIM_SUPPLY_VOLT_FUTURE_CLASS;
            break;
        default:
            //SCI_TRACE_LOW:"SIM::volt not correct value. 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_3202_112_2_17_22_59_22_4514, (uint8 *)"d", file_character);
            supply_volt = SIM_SUPPLY_VOLT_FUTURE_CLASS;
            break;
    }

    //SCI_TRACE_LOW:"GetCardSupplyVot supply_volt = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_3207_112_2_17_22_59_22_4515, (uint8 *)"d", supply_volt);
    return supply_volt;
}

/**********************************************************************
//    Description:
//      Get the GSM specific data from the SIM card(Just get the DF_GSM
//    file select data,and decode it)
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_GetGSMSpecificData(   //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_GSM_SPECIFIC_DATA_T *gsm_data   //out:the gsm specific data
)
{
    SIM_SERVICE_RESULT_E    ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    BOOLEAN fail_flag = FALSE;
#ifdef SIM_CDMA_CARD_SUPPORT
    SIM_RESPONSE_DATA_OF_SELECT_T cdma_select_data;
    SIM_SERVICE_RESULT_E cdma_ser_result;
#endif

#ifdef SIM_3G_USIM_SUPPORT

    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
    {
        //select the DF_GSM to and get the select output data
        ser_result = SIMINSTR_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &select_data);

        if(ser_result != SIM_SERVICE_OK)
        {
            if(SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result)
            {
                ser_result = SIMINSTR_SelectFile(cur_sim_task, SIM_FILE_DF_DCS1800, &select_data);

                if(SIM_SERVICE_OK != ser_result)
                {
                    fail_flag = TRUE;
                }
                else
                {
                    SIMFS_SetDSC1800Flag(cur_sim_task, TRUE);
                    fail_flag = FALSE;
                }
            }
            else
            {
                fail_flag = TRUE;
            }
        }

#ifdef SIM_CDMA_CARD_SUPPORT

        if(SIM_SERVICE_OK != ser_result) //when SELECT DFgsm FAIL, we will check wether CDMA only card.
        {
            cdma_ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_CDMA, &cdma_select_data);

            if(SIM_SERVICE_OK == cdma_ser_result)
            {
                s_sim_cdma_flag[cur_sim_task] = TRUE;

                if(fail_flag)
                {
                    //there is no DFgsm, only DFcdma.
                    s_sim_cdma_only_flag[cur_sim_task] = TRUE;
                    SCI_MEMCPY(&select_data, &cdma_select_data, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));

                    ser_result = SIM_SERVICE_OK;//wuding add for CDMA only RUIM card
                }
                else
                {
                    s_sim_cdma_only_flag[cur_sim_task] = FALSE;
                }
            }
        }

#endif
    }

#ifdef SIM_3G_USIM_SUPPORT
    else
    {
        ser_result = SIMINSTR_SelectFile(cur_sim_task, SIM_FILE_ADF_USIM, &select_data);

        if(ser_result != SIM_SERVICE_OK)
        {
            return ser_result;
        }

        s_sim_gain_chv_remain_times_flag[cur_sim_task] = TRUE;
    }

#endif//SIM_3G_USIM_SUPPORT

    //get the needed information from the select response
    gsm_data->df_child_num = select_data.dfnumber_recordlen;
    gsm_data->ef_child_num = select_data.ef_number;
    //the info from the select response's file characteristic byte
    gsm_data->clock_stop_type = (SIM_CLOCK_STOP_TYPE_E)(SIM_GSM_SPECIFIC_CLOCK_STOP_MARK & select_data.filecharacter_efstructure);
    /*SCI_TRACE_LOW("SIM::CLOCK STOP:%x!",gsm_data->clock_stop_type);*/
    gsm_data->card_supply_volt = GetCardSupplyVot(select_data.filecharacter_efstructure);
    gsm_data->chv1_enabled = (BOOLEAN)((select_data.filecharacter_efstructure & SIM_GSM_SPECIFIC_CHV1_ENABLE_MARK) == 0);
    //the chv1 status from the select response data's chv1 status byte
    gsm_data->chv1_status.chv_blocked = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.chv1_status) == 0);
    gsm_data->chv1_status.false_remain = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.chv1_status;
    gsm_data->chv1_status.secrect_initialized = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.chv1_status) != 0);
#ifdef MODEM_PLATFORM

    if(gsm_data->chv1_enabled)
    {
        gsm_data->chv1_status.chv_verified = FALSE;
    }
    else
    {
        gsm_data->chv1_status.chv_verified = TRUE;//We assume this as verified.
    }

#endif
    //the chv2 status from the select response data's chv2 status byte
    gsm_data->chv2_status.chv_blocked = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.chv2_status) == 0);
    gsm_data->chv2_status.false_remain = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.chv2_status;
    gsm_data->chv2_status.secrect_initialized = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.chv2_status) != 0);
    //the unblock chv1 status from the select response data's unblock chv1 status byte
    gsm_data->unblock_chv1_status.chv_blocked = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.unblock_chv1_status) == 0);
    gsm_data->unblock_chv1_status.false_remain = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.unblock_chv1_status;
    gsm_data->unblock_chv1_status.secrect_initialized = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.unblock_chv1_status) != 0);
    //the unblock chv2 status from the select response data's unblock chv2 status byte
    gsm_data->unblock_chv2_status.chv_blocked = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.unblock_chv2_status) == 0);
    gsm_data->unblock_chv2_status.false_remain = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.unblock_chv2_status;
    gsm_data->unblock_chv2_status.secrect_initialized = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.unblock_chv2_status) != 0);

    /*
    SCI_TRACE_LOW("SIM get GSM chv 0x%x 0x%x 0x%x 0x%x. chv1 %d, 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x",\
        select_data.chv1_status,select_data.chv2_status,select_data.unblock_chv1_status,select_data.unblock_chv2_status,\
        gsm_data->chv1_enabled,\
        gsm_data->chv1_status.chv_blocked,gsm_data->chv1_status.false_remain,gsm_data->chv1_status.secrect_initialized,\
        gsm_data->chv2_status.chv_blocked,gsm_data->chv2_status.false_remain,gsm_data->chv2_status.secrect_initialized,\
        gsm_data->unblock_chv1_status.chv_blocked,gsm_data->unblock_chv1_status.false_remain,gsm_data->unblock_chv1_status.secrect_initialized,\
        gsm_data->unblock_chv2_status.chv_blocked,gsm_data->unblock_chv2_status.false_remain,gsm_data->unblock_chv2_status.secrect_initialized);
    */
    return ser_result;
}


/**********************************************************************
//    Description:
//      Read a EF file from the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_ReadEF(   //return value:the result of the read operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file to be read
    uint16  offset, //the offset that start to read in transprent file
    uint8   record_num, //the record num to read in recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of recordy file
    uint16 *filesize_recordlen, //out:the file len of transparent file and reocrd len of recordy file
    uint16 *data_len,   //out:the data len that readed from the file
    uint8  *data        //out:the data readef from the file
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E    ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SIG_RESPONSE_IND_T *instr_response;
    SIMINSTR_RESULT_TYPE_E instr_result;
    BOOLEAN is_large_binary_EF = FALSE;
    BOOLEAN is_large_binary_EF_strange_usim = FALSE;
    uint8 large_binary_read_times = 0;

    *filesize_recordlen = 0;
    //select the destination file to and get the select output data
    ser_result = SIMINSTR_SelectFile(cur_sim_task, file_name, &select_data);

    if(ser_result != SIM_SERVICE_OK)
    {
        return ser_result;
    }

    if((file_name == SIM_FILE_EF_EXT1) || (file_name == SIM_FILE_EF_TELECOM_PB_EXT1) || (file_name == SIM_FILE_EF_USIM_PB_EXT1))
    {
        uint8 ext_record_num = 0;

        SCI_TRACE_LOW("SIM%d:SIMINSTR_ReadEF, file_size=%d,record_len is %d.",cur_sim_task,select_data.freespace_filesize,select_data.dfnumber_recordlen);
        
        if(select_data.dfnumber_recordlen != 0)
        {
            ext_record_num = select_data.freespace_filesize/select_data.dfnumber_recordlen;
            
            if(record_num > ext_record_num)
            {
                SCI_TRACE_LOW("SIM%d:SIMINSTR_ReadEF error! to read record_num is %d,ext_record_num is %d.",cur_sim_task,record_num,ext_record_num);
                return SIM_SERVICE_READ_EF_ERROR;
            }
        }            
    }
    
    //if the file is not a EF file
    if(select_data.file_type != SIM_FILE_T_EF)
    {
        s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw1 = 0x94;
        s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw2 = 0x08;
        s_select_sw_and_rsp[cur_sim_task].sw_rsp.data_len = 0;
        s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //Xi'an Huawei foreign SIM Test. A wingtech' sim card, ADN's file_status is 0.
    //But it will last 3s before the SIM card send "98 10" out, it waste too much time.
    //So I will not read this EF if the file_status is 0 and "not readable or updatable when invalidated".
    if(0 == (select_data.file_status & SIM_FILE_INVALIDATE_MARK)) //this EF is invalidated
    {
        if(0 == (select_data.file_status & SIM_FILE_INVALIDATE_RU_MARK)) //this EF not readable or updatable when invalidated
        {
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw1 = 0x98;
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw2 = 0x10;
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.data_len = 0;
            s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];
            return SIM_SERVICE_CONTRADICT_VALIDATION;
        }
    }

    //Add the read instruction
    //the file want to read is a transparent file
    if(SIM_EF_S_TRANSPARENT == select_data.filecharacter_efstructure)
    {
        //get the file size
        *filesize_recordlen = select_data.freespace_filesize;

        GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                           SIMINSTR_READ_BINARY);

        //in Read Binary instruction P1 is the high byte of the offset
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = (uint8)(offset >> 8);
        //in Read Binary instruction P2 is the low byte of the offset
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = (uint8)offset;

        //in Read Binary instruction P3 is the length of the sting
        if(((0 == record_num)
            && (select_data.freespace_filesize < SIM_MAX_R_APDU_DATA_LEN))
            || (select_data.freespace_filesize < (uint16)record_num))
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = (uint8)select_data.freespace_filesize;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = (uint8)select_data.freespace_filesize;
        }
        else
        {
            if(select_data.freespace_filesize >= SIM_MAX_R_APDU_DATA_LEN) //A transparent EF size > 255, at least is 256.
            {
#ifdef MODEM_PLATFORM

                if(0 == record_num) //The upper layer want to read all the data.
                {
                    is_large_binary_EF = TRUE;
                    SCI_TRACE_LOW("SIM%d:large transparent EF %d, %d", cur_sim_task, file_name, select_data.freespace_filesize);
                    //P3 0 means the maxinum, 256bytes. ISO7816-3:2006(E) 12.2
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = 0;
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 256;
                }
                else
                {
                    //This is a large EF, but the upper layer only want to get part data (< 256) of it.
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = record_num;
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = record_num;
                }

#else
                is_large_binary_EF = TRUE;
                //SCI_TRACE_LOW:"SIM%d:large transparent EF %d, %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_3409_112_2_17_22_59_22_4516, (uint8 *)"ddd", cur_sim_task, file_name, select_data.freespace_filesize);
                //P3 0 means the maxinum, 256bytes. ISO7816-3:2006(E) 12.2
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = 0;
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 256;
#endif
            }
            else
            {
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = record_num;
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = record_num;
            }
        }
    }
    //the file is a fixed linear or cylic file
    else
    {
        //get the record len
        *filesize_recordlen = select_data.dfnumber_recordlen;
        GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                           SIMINSTR_READ_RECORD);
        //in Read Record instruction the P1 is ther record num
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = record_num;
        //in Read Record instruction the P2 is the mode
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = mode;
        //in Read Record instruction the P3 is the length of the record
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = select_data.dfnumber_recordlen;
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = select_data.dfnumber_recordlen;
    }

    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM, save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, file_name, instr_response);

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result) &&
            (SIM_STATUS_OK_RESEND_ADPU != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIMAT_GetAccessCondition(SIM_READ_ACCESS, (&select_data)));

            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        if(SIM_STATUS_OK_RESEND_ADPU == instr_result)
        {
            //Re-Send the current instruction, with the new P3.
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].cur_instr--;//the current instruction should redo
        }

        is_large_binary_EF_strange_usim = FALSE;

        //the last insturction return value is OK and have reponse data need to
        //be get by the GET RESPONSE instruction,so add the GET RESPONSE
        //instruction to the instruction list and set the P3 of GET RESPONSE instruction
        if((SIM_STATUS_OK_RESPONSE_DATA == instr_result) &&
            (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr))
        {
            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                               SIMINSTR_GET_RESPONSE);
            s_sim_instruction_list[cur_sim_task].instr_num++;

            //set the P3 of GET RESPONSE instruction
			if (0 == instr_response->sw2)
			{
				s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = 256;
			}
			else
			{
				s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
			}
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;

            if(is_large_binary_EF)
            {
                is_large_binary_EF_strange_usim = TRUE;
            }
        }

        if(is_large_binary_EF)
        {
            //Some SIM/USIM cards has a large binary EF(file size > 256), when you read the first 256 bytes,
            // 1, some card return 9000
            // 2, but other cards will return 61XX(a Unicom WCDMA), and you have to get the rest data.
            if(0xB0 == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_INS])
            {
                large_binary_read_times++;
                if(3 == large_binary_read_times) //max we read 768 bytes (total 3 times)
                {
                    *data_len = instr_response->data_len;
                    SCI_ASSERT(*data_len <= SIM_MAX_R_APDU_DATA_LEN); /*assert verified*/
                    SCI_MEMCPY((data + SIM_MAX_R_APDU_DATA_LEN + SIM_MAX_R_APDU_DATA_LEN), instr_response->data, instr_response->data_len);
                    *data_len += (SIM_MAX_R_APDU_DATA_LEN + SIM_MAX_R_APDU_DATA_LEN);

                    // for CRSM, save the response data to the global variable
                    instr_response->data_len = *data_len;
                    //the sim hal(s_sim_rx_buf.buf[256]), sowe should allocate memory
                    instr_response->data = (uint8 *)SCI_ALLOC_BASE(instr_response->data_len);
                    SCI_MEMCPY(instr_response->data, data, instr_response->data_len);//copy the total bytes(>256) for CRSM
                    SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, file_name, instr_response);
                    SCI_FREE(instr_response->data);//copy finished, free the memory.
                }
                else if(2 == large_binary_read_times)
                {
                    if (0 == instr_response->data_len)
                    {
                        is_large_binary_EF_strange_usim = TRUE;
                    }
                    else
                    {
                        SCI_MEMCPY((data + SIM_MAX_R_APDU_DATA_LEN), instr_response->data, instr_response->data_len);
                    }

                    if(is_large_binary_EF_strange_usim)
                    {
                        //the InstructionCode has been encoded.
                    }
                    else
                    {
                        if((select_data.freespace_filesize > 512)
                            && (SIM_MAX_R_APDU_DATA_LEN == instr_response->data_len))
                        {
                            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                                               SIMINSTR_READ_BINARY);
                            s_sim_instruction_list[cur_sim_task].instr_num++;

                            if((select_data.freespace_filesize - SIM_MAX_R_APDU_DATA_LEN - SIM_MAX_R_APDU_DATA_LEN) >= SIM_MAX_R_APDU_DATA_LEN) //the rest is still > 256
                            {
                                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = 0;
                                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = 256;
                            }
                            else
                            {
                                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = (uint8)(select_data.freespace_filesize - SIM_MAX_R_APDU_DATA_LEN - SIM_MAX_R_APDU_DATA_LEN);
                                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = (uint16)(select_data.freespace_filesize - SIM_MAX_R_APDU_DATA_LEN - SIM_MAX_R_APDU_DATA_LEN);
                            }

                            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P1] = 2;//offset from 512.
                            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P2] = 0;//offset must > 512
                        }
                        else
                        {
                            //All the data has been received
                            *data_len = instr_response->data_len;
                            SCI_ASSERT(*data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified*/
                            SCI_MEMCPY((data + SIM_MAX_R_APDU_DATA_LEN), instr_response->data, instr_response->data_len);
                            *data_len += SIM_MAX_R_APDU_DATA_LEN;

                            // for CRSM, save the response data to the global variable
                            instr_response->data_len = *data_len;
                            //the sim hal(s_sim_rx_buf.buf[256]), so we should allocate memory
                            instr_response->data = (uint8 *)SCI_ALLOC_BASE(instr_response->data_len);
                            SCI_MEMCPY(instr_response->data, data, instr_response->data_len);//copy the total bytes(>256) for CRSM
                            SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, file_name, instr_response);
                            SCI_FREE(instr_response->data);//copy finished, free the memory.
                        }
                    }
                }
                else if (1 == large_binary_read_times) //just finish reading the 1st time
                {
                    if (0 == instr_response->data_len)
                    {
                        is_large_binary_EF_strange_usim = TRUE;
                        *data_len = 0;
                    }
                    else
                    {
                        SCI_MEMCPY(data, instr_response->data, SIM_MAX_R_APDU_DATA_LEN);
                    }

                    if(is_large_binary_EF_strange_usim)
                    {
                        //the InstructionCode has been encoded.
                    }
                    else
                    {
                        if(SIM_MAX_R_APDU_DATA_LEN == select_data.freespace_filesize)
                        {
                            //All the data has been received
                            *data_len = instr_response->data_len;
                            SCI_ASSERT(*data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified*/

                            // for CRSM, save the response data to the global variable
                            SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, file_name, instr_response);
                        }
                        else
                        {
                            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                                               SIMINSTR_READ_BINARY);
                            s_sim_instruction_list[cur_sim_task].instr_num++;

                            if((select_data.freespace_filesize - SIM_MAX_R_APDU_DATA_LEN) >= SIM_MAX_R_APDU_DATA_LEN) //the rest is still > 256
                            {
                                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = 0;
                                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = 256;
                            }
                            else
                            {
                                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = (uint8)(select_data.freespace_filesize - SIM_MAX_R_APDU_DATA_LEN);
                                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = (uint16)(select_data.freespace_filesize - SIM_MAX_R_APDU_DATA_LEN);
                            }

                            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P1] = 1;//offset from 256.
                            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P2] = 0;//offset must > 256
                        }
                    }
                }
            }
        }
        //if the instruction is the last one of the instruction list get the select structure data from the response data
        else if(s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr)
        {
            *data_len = instr_response->data_len;
            SCI_ASSERT(*data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified: Check_value */
            SCI_MEMCPY(data, instr_response->data, *data_len);

            // for CRSM, save the response data to the global variable
            SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, file_name, instr_response);
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;
#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_ReadFile(cur_sim_task, file_name, offset, record_num, mode,
                           filesize_recordlen, data_len, data);
#endif
}


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
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E    ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SIG_RESPONSE_IND_T *instr_response;
    SIMINSTR_RESULT_TYPE_E instr_result;
    uint16 copy_data_len = (uint8)data_len;
    uint16 update_binary_len = 0;
    BOOLEAN is_large_binary_EF = FALSE;
    uint8 large_binary_update_times = 0;

    //procedure for data_len=0
    if(0 == data_len)
    {
        //SCI_TRACE_LOW:"SIM::SIMINSTR_UpdateEF,ERROR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_3615_112_2_17_22_59_23_4517, (uint8 *)"");
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    //select the destination file to and get the select output data
    ser_result = SIMINSTR_SelectFile(cur_sim_task, file_name, &select_data);

    if(ser_result != SIM_SERVICE_OK)
    {
        return ser_result;
    }

    //if the file is not a EF file
    if(select_data.file_type != SIM_FILE_T_EF)
    {
        s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw1 = 0x94;
        s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw2 = 0x08;
        s_select_sw_and_rsp[cur_sim_task].sw_rsp.data_len = 0;
        s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //Xi'an Huawei foreign SIM Test. A wingtech' sim card, read ADN return 98 10, for its file_status is 0.
    //But it will last 3s before the SIM card send "98 10" out, it waste too much time.
    //So I will not update this EF if the file_status is 0 and "not readable or updatable when invalidated".
    if(0 == (select_data.file_status & SIM_FILE_INVALIDATE_MARK)) //this EF is invalidated
    {
        if(0 == (select_data.file_status & SIM_FILE_INVALIDATE_RU_MARK)) //this EF not readable or updatable when invalidated
        {
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw1 = 0x98;
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw2 = 0x10;
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.data_len = 0;
            s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];
            return SIM_SERVICE_CONTRADICT_VALIDATION;
        }
    }

    //Add the read instruction
    //the file want to read is a transparent file
    if(SIM_EF_S_TRANSPARENT == select_data.filecharacter_efstructure)
    {
        GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                           SIMINSTR_UPDATE_BINARY);

        update_binary_len = MIN(select_data.freespace_filesize, data_len);

        if(update_binary_len > 255) //some transparent EF size is more than 255(EFplmnwact 0x6F60).
        {
            is_large_binary_EF = TRUE;

            if(0 != offset) //upper layer give the offset
            {
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = (uint8)(offset >> 8);
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = (uint8)offset;
                //we'd make sure "offset + to_be_update should not exceed the filesize"
                copy_data_len = MIN(255, (select_data.freespace_filesize - offset));
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = (uint8)copy_data_len;
            }
            else
            {
                //upper layer didn't give the offset, and the update_binary_len > 256
                //so we should update this EF for servral times (2 times we support)
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = 0;
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0;
                copy_data_len = 255;
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = 0xFF;//firstly,update 255bytes
            }
        }
        else
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = 0;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = update_binary_len;
            copy_data_len = update_binary_len;
        }

        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, copy_data_len);
        SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, data, copy_data_len);

        s_sim_instruction_list[cur_sim_task].instr_num++;
    }
    //the file is a fixed linear or cylic file
    else
    {
        GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                           SIMINSTR_UPDATE_RECORD);
        //in Read Record instruction the P2 is the mode
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = mode;

        //when the record mode is PREVIOUS or NEXT,the P1 must be set to '00'
        if(SIM_RECORD_ABSOLUTE == mode)
        {
            //in Read Record instruction the P1 is ther record num
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = record_num;
        }
        else
        {
            //in Read Record instruction the P1 is ther record num
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = 0;
        }

        if(select_data.dfnumber_recordlen <= data_len)
        {
            //in Read Record instruction the P3 is the length of the record
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = select_data.dfnumber_recordlen;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, (uint16)(select_data.dfnumber_recordlen));
            SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, data, select_data.dfnumber_recordlen);
        }
        else
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = data_len;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, (uint16)data_len);
            SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, data, data_len);
        }

        s_sim_instruction_list[cur_sim_task].instr_num++;
    }

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, file_name, instr_response);

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIMAT_GetAccessCondition(SIM_UPDATE_ACCESS, &select_data));

            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        if(is_large_binary_EF)
        {
            if(0 == large_binary_update_times) //we only update 2 times, which means 512 bytes at most.
            {
                large_binary_update_times++;//do not enter again
                update_binary_len -= copy_data_len;//we've update first 255 bytes successfully, count the remained data.

                if(update_binary_len > 0) //there has remained data
                {
                    ClearInstrDataBuf(cur_sim_task);//to save some code size

                    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                                       SIMINSTR_UPDATE_BINARY);

                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = 0;
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0xFF;

                    if(update_binary_len > 255) //the remained data is still more than 255 bytes
                    {
                        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = 0xFF;
                        copy_data_len = 255;
                    }
                    else
                    {
                        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = update_binary_len;
                        copy_data_len = update_binary_len;
                    }

                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, copy_data_len);
                    if (data_len > 255)//double check
                    {
                        SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, (data + 255), copy_data_len);
                        s_sim_instruction_list[cur_sim_task].instr_num++;
                    }
                    else
                    {
                        SCI_TRACE_LOW("SIM%d:SIMINSTR_UpdateEF data_len=%d, no data to update",cur_sim_task,data_len);
                    }
                }
            }
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;
#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_UpdateFile(cur_sim_task, file_name, offset, record_num, mode,
                             data_len, data);
#endif
}


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
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E                ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T   select_data;
    SIM_SIG_RESPONSE_IND_T     *instr_response;
    SIMINSTR_RESULT_TYPE_E          instr_result;

    //select the destination file to and get the select output data
    ser_result = SIMINSTR_SelectFile(cur_sim_task, file_name, &select_data);

    if(ser_result != SIM_SERVICE_OK)
    {
        return ser_result;
    }

    // 1,if the file is not a Cylic file
    // 2,b7=1 indicates that the INCREASE cmd is allowed...(GSM11.11 chapter9.2.1)
    if((select_data.filecharacter_efstructure != SIM_EF_S_CYCLIC)
        || (BIT_6 != (select_data.cyclic_full_check & BIT_6)))
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //get the instruction code
    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_INCREASE);
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, SIM_INCREASE_DATA_LEN);
    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, data.data, SIM_INCREASE_DATA_LEN);

    if(TB_SIM_T_0_PROTOCOL == g_sim_card_setting[cur_sim_task].t_protocol)
    {
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 0;
    }
    else
    {
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = SIM_INCREASE_RESPONSE_DATA_LEN;
    }

    s_sim_instruction_list[cur_sim_task].instr_num++;


    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, file_name, instr_response);

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result) &&
            (SIM_STATUS_OK_RESEND_ADPU != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIMAT_GetAccessCondition(SIM_INCREASE_ACCESS, &select_data));

            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        if(SIM_STATUS_OK_RESEND_ADPU == instr_result)
        {
            //Re-Send the current instruction, with the new P3.
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].cur_instr--;//the current instruction should redo
        }

        //the last insturction return value is OK and have reponse data need to
        //be get by the GET RESPONSE instruction,so add the GET RESPONSE
        //instruction to the instruction list and set the P3 of GET RESPONSE instruction
        if((SIM_STATUS_OK_RESPONSE_DATA == instr_result) &&
            (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr))
        {
            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                               SIMINSTR_GET_RESPONSE);
            s_sim_instruction_list[cur_sim_task].instr_num++;

            //set the P3 of GET RESPONSE instruction
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
        }

        //if the instruction is the last one of the instruction list
        //get the increase data from the response data
        if(s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr)
        {
            if(instr_response->data_len != 2 * SIM_INCREASE_DATA_LEN)
            {
                //free the response signal
                SCI_FREE_SIGNAL(instr_response);
                //clear the instruction list for next use
                ClearInstrList(cur_sim_task);
                return SIM_SERVICE_INCREASE_EF_ERROR;
            }

            SCI_MEMCPY(result->new_value.data, instr_response->data, SIM_INCREASE_DATA_LEN);
            SCI_MEMCPY(result->added_value.data, instr_response->data + SIM_INCREASE_DATA_LEN, SIM_INCREASE_DATA_LEN);
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_Increase(cur_sim_task, file_name, data, result);
#endif
}


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
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E                ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T   select_data;
    SIM_SIG_RESPONSE_IND_T     *instr_response;
    SIMINSTR_RESULT_TYPE_E          instr_result;

    //select the destination file to and get the select output data
    ser_result = SIMINSTR_SelectFile(cur_sim_task, file_name, &select_data);

    if(ser_result != SIM_SERVICE_OK)
    {
        return ser_result;
    }

    //can not invalidate a not EF file
    if(select_data.file_type != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //the EF is invalidate,so do not need do any thing
    if(!(select_data.file_status & SIM_SELECT_INVALIDATE_MARK))
    {
        return SIM_SERVICE_OK;
    }

    //add the Invalidate instruction code to the instruction list
    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_INVALIDATE);
    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIMAT_GetAccessCondition(SIM_INVALIDATE_ACCESS, &select_data));
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //If the invalidation is successfully, we will change the current select EF's file_status(set bit_0 0).
        if(SIMINSTR_INVALIDATE == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name)
        {
            s_sim_cur_file_response[cur_sim_task].file_status &= ~SIM_FILE_INVALIDATE_MARK;
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);

    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_InvalidateFile(cur_sim_task, file_name);
#endif
}

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
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E                ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T   select_data;
    SIM_SIG_RESPONSE_IND_T     *instr_response;
    SIMINSTR_RESULT_TYPE_E          instr_result;

    //select the destination file to and get the select output data
    ser_result = SIMINSTR_SelectFile(cur_sim_task, file_name, &select_data);

#ifdef GSM_CUSTOMER_AFP_SUPPORT
    if (ser_result == SIM_SERVICE_OK)
    {
	//can not rehabilitate a not EF file
	if (select_data.file_type != SIM_FILE_T_EF)
	{
            return SIM_SERVICE_INCONSISTENT_FILE;
	}
    }
    else if (ser_result == SIM_SERVICE_INVALIDATE_EF_ERROR)
    {
        //should be continued
    }
    else
    {
        return ser_result;
    }
#else
    if (ser_result != SIM_SERVICE_OK)
    {
        return ser_result;
    }
#endif

    //the EF is validate,so do not need do any thing
    //  if (select_data.file_status & SIM_SELECT_INVALIDATE_MARK)
    //  {
    //      return SIM_SERVICE_OK;
    //  }

    //add the Invalidate instruction code to the instruction list
    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_REHABILITATE);
    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIMAT_GetAccessCondition(SIM_REHABILITATE_ACCESS, &select_data));
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //If the rehabilitation is successfully, we will change the current select EF's file_status(set bit_0 1).
        if(SIMINSTR_REHABILITATE == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name)
        {
            s_sim_cur_file_response[cur_sim_task].file_status |= SIM_FILE_INVALIDATE_MARK;
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);

    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_RehabilitateFile(cur_sim_task, file_name);
#endif
}


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
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E                ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T   select_data;
    SIM_SIG_RESPONSE_IND_T     *instr_response;
    SIMINSTR_RESULT_TYPE_E          instr_result;
    uint8 len;

    SCI_ASSERT(0 < pattern_len);/*assert verified:Check_value */

    //select the destination file to and get the select output data
    ser_result = SIMINSTR_SelectFile(cur_sim_task, file_name, &select_data);

    if(ser_result != SIM_SERVICE_OK)
    {
        return ser_result;
    }

    //the file must be a Fixed Linear file
    if((select_data.file_type != SIM_FILE_T_EF) ||
        (select_data.filecharacter_efstructure != SIM_EF_S_LINEAR_FIXED))
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_SEEK);

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        len = pattern_len;
        //seek from the record id one, P1 set to 0x1
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = 0x1;
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = SIM_SEEK_FROM_RECORD_P1_FORWARD;
        //the P3 of the Seek instruction is the length of the pattern
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = pattern_len;

        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 2;
    }
    else
#endif
    {
        //the P2 of Seek instruciton is the type/mode of SEEK
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = seek_mode | seek_type;

        //change the pattern len that can be used to seek
        if(pattern_len > SIM_SEEK_PATTERN_MAX_LEN)
        {
            len = SIM_SEEK_PATTERN_MAX_LEN;

            if(len > select_data.dfnumber_recordlen)
            {
                len = select_data.dfnumber_recordlen;
            }
        }
        else
        {
            len = pattern_len;
        }

        //the P3 of the Seek instruction is the length of the pattern
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = len;

        //the length of the response data from the SIM card when the command is succeed is different when
        //the Transmit protocol of the SIM card is different
        if((TB_SIM_T_0_PROTOCOL == g_sim_card_setting[cur_sim_task].t_protocol)
            || (SIM_SEEK_TYPE_1 == seek_type))
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 0;
        }
        else
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = SIM_SEEK_RESPONSE_DATA_LEN;
        }
    }

    //copy patten data to the instruction data
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, (uint16)len);
    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, pattern, len);
    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, file_name, instr_response);

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result) &&
            (SIM_STATUS_OK_RESEND_ADPU != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIMAT_GetAccessCondition(SIM_READ_ACCESS, &select_data));
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        if(SIM_STATUS_OK_RESEND_ADPU == instr_result)
        {
            //Re-Send the current instruction, with the new P3.
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].cur_instr--;//the current instruction should redo
        }

        //the last insturction return value is OK and have reponse data need to
        //be get by the GET RESPONSE instruction,so add the GET RESPONSE
        //instruction to the instruction list and set the P3 of GET RESPONSE instruction
        if((SIM_STATUS_OK_RESPONSE_DATA == instr_result) &&
            (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr))
        {
            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                               SIMINSTR_GET_RESPONSE);
            s_sim_instruction_list[cur_sim_task].instr_num++;

            //set the P3 of GET RESPONSE instruction
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
        }

        //if the instruction is the last one of the instruction list and the seek is a TYPE2 Seek
        //get the select structure data from the response data
        if((s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr) &&
            (seek_type == SIM_SEEK_TYPE_2))
        {
#ifdef SIM_3G_USIM_SUPPORT

            if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
            {
                *record_num = (uint8)(instr_response->data_len);
                SCI_MEMCPY(record, instr_response->data , instr_response->data_len);
            }
            else
#endif
            {
                *record_num = 1;
                *record = *(instr_response->data);
            }
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);

    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_SeekInFile(cur_sim_task, file_name, seek_type, seek_mode,
                             pattern_len, pattern, record_num);
#endif
}

/**********************************************************************
//    Description:
//      Verify a CHV
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_VerifyCHV( //return vlaue:the result of verify operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E chv_indication,    //the CHV indication to verify
    SIM_CHV_T chv   //the chv use to verify
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E    ser_result;
    SIM_SIG_RESPONSE_IND_T     *instr_response;
    SIMINSTR_RESULT_TYPE_E          instr_result;

    //the CHV has been verified or do not need to run
    //Verify CHV instruction to verify

    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_VERIFY_CHV);
    //in Verify CHV instruction,the P2 is the CHV indication
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = chv_indication;

#ifdef SIM_3G_USIM_SUPPORT

    if((SIM_CHV2 == chv_indication) && (USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task]))
    {
        // 3G USIM PIN2 Verification P2 is 0x81
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0x81;
    }

#endif

    //copy the chv to the instruction data
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, SIM_CHV_MAX_LEN);
    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, chv.secret_code, SIM_CHV_MAX_LEN);

    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);

            if(SIM_SERVICE_CHV_VER_FAIL == ser_result)
            {
                if(SIM_CHV1 == chv_indication)
                {
#ifdef SIM_3G_USIM_SUPPORT

                    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
                    {
                        g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain--;
                    }

#ifdef SIM_3G_USIM_SUPPORT
                    else
                    {
                        if(0x63 == instr_response->sw1)
                        {
                            g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = ((instr_response->sw2) & 0xF);
                        }
                        else
                        {
                            g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain--;
                        }
                    }

#endif

                    //double check chv1 status
                    if(0 == g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain)
                    {
                        g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
                        ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
                    }
                }
                else
                {
#ifdef SIM_3G_USIM_SUPPORT

                    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
                    {
                        g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain--;
                    }

#ifdef SIM_3G_USIM_SUPPORT
                    else
                    {
                        if(0x63 == instr_response->sw1)
                        {
                            g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain = ((instr_response->sw2) & 0xF);
                        }
                        else
                        {
                            g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain--;
                        }
                    }

#endif

                    //double check chv2 status
                    if(0 == g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain)
                    {
                        g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.chv_blocked = TRUE;
                        ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
                    }
                }
            }

            if(SIM_SERVICE_CHV_FAIL_WITH_BLOCKED == ser_result)
            {
                if(SIM_CHV1 == chv_indication)
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = 0;
                    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
#ifdef SIM_3G_USIM_SUPPORT

                    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                    {
                        s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_ENABLED_AND_BLOCKED;
                    }

#endif
                }
                else
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain = 0;
                    g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.chv_blocked = TRUE;
                }
            }

            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    if(SIM_CHV1 == chv_indication)
    {
#ifdef SIM_3G_USIM_SUPPORT

        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_VERIFIED;
        }

#endif
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_VerifyCHV(cur_sim_task, chv_indication, chv);
#endif
}


/**********************************************************************
//    Description:
//      Change a CHV
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_ChangeCHV( //return value:the reuslt of change CHV operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E    chv_indication, //the chv indication
    SIM_CHV_T   old_chv,    //the old chv
    SIM_CHV_T   new_chv     //the new chv
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_RESPONSE_IND_T     *instr_response;
    SIMINSTR_RESULT_TYPE_E          instr_result;

    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_CHANGE_CHV);
    //in Verify CHV instruction,the P2 is the CHV indication
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = chv_indication;

#ifdef SIM_3G_USIM_SUPPORT

    if((SIM_CHV2 == chv_indication) && (USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task]))
    {
        // 3G USIM PIN2 Verification P2 is 0x81
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0x81;
    }

#endif

    //copy the chvs to the instruction data
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, 2 * SIM_CHV_MAX_LEN);
    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, old_chv.secret_code, SIM_CHV_MAX_LEN);
    SCI_MEMCPY((s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data + SIM_CHV_MAX_LEN), new_chv.secret_code, SIM_CHV_MAX_LEN);

    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);

            if(SIM_SERVICE_CHV_VER_FAIL == ser_result)
            {
                if(SIM_CHV1 == chv_indication)
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain--;

                    //double check chv1 status
                    if(0 == g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain)
                    {
                        g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
                        ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
                    }
                }
                else
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain--;

                    //double check chv2 status
                    if(0 == g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain)
                    {
                        g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.chv_blocked = TRUE;
                        ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
                    }
                }
            }

            if(SIM_SERVICE_CHV_FAIL_WITH_BLOCKED == ser_result)
            {
                if(SIM_CHV1 == chv_indication)
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = 0;
                    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
#ifdef SIM_3G_USIM_SUPPORT

                    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                    {
                        s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_ENABLED_AND_BLOCKED;
                    }

#endif
                }
                else
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain = 0;
                    g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.chv_blocked = TRUE;
                }
            }

            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    if(SIM_CHV1 == chv_indication)
    {
#ifdef SIM_3G_USIM_SUPPORT

        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_VERIFIED;
        }

#endif
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_ChangeCHV(cur_sim_task, chv_indication, old_chv, new_chv);
#endif
}


/**********************************************************************
//    Description:
//      Disable a CHV
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_DisableCHV(    //return value:the result of disavle CHV
    uint32 cur_sim_task,
    SIM_CHV_T   chv //the chv1 used to disable CHV1
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_RESPONSE_IND_T     *instr_response;
    SIMINSTR_RESULT_TYPE_E          instr_result;

    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_DISABLE_CHV);

    //copy the chvs to the instruction data
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, SIM_CHV_MAX_LEN);
    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, chv.secret_code, SIM_CHV_MAX_LEN);

    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);

            if(SIM_SERVICE_CHV_VER_FAIL == ser_result)
            {
                g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain--;

                //double check chv1 status
                if(0 == g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain)
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
                    ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
                }
            }

            if(SIM_SERVICE_CHV_FAIL_WITH_BLOCKED == ser_result)
            {
                g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = 0;
                g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
#ifdef SIM_3G_USIM_SUPPORT

                if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                {
                    s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_ENABLED_AND_BLOCKED;
                }

#endif
            }

            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled = FALSE;

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_DISABLED;
    }

#endif
    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_DisableCHV(cur_sim_task, chv);
#endif
}


/**********************************************************************
//    Description:
//      Enable a CHV
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_EnableCHV( //return value:the result of enable CHV operation
    uint32 cur_sim_task,
    SIM_CHV_T chv   //the CHV1 used to enable CHV1
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_RESPONSE_IND_T     *instr_response;
    SIMINSTR_RESULT_TYPE_E          instr_result;

    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_ENABLE_CHV);

    //copy the chvs to the instruction data
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, SIM_CHV_MAX_LEN);
    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, chv.secret_code, SIM_CHV_MAX_LEN);

    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);

            if(SIM_SERVICE_CHV_VER_FAIL == ser_result)
            {
                g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain--;

                //double check the chv1 status
                if(0 == g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain)
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
                    ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
                }
            }

            if(SIM_SERVICE_CHV_FAIL_WITH_BLOCKED == ser_result)
            {
                g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = 0;
                g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
#ifdef SIM_3G_USIM_SUPPORT

                if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                {
                    s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_ENABLED_AND_BLOCKED;
                }

#endif
            }

            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled = TRUE;

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_VERIFIED;
    }

#endif
    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_EnableCHV(cur_sim_task, chv);
#endif
}

/**********************************************************************
//    Description:
//      Unblock a CHV
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_UnblockCHV(    //return value:the result of unblock CHV operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E    chv_indication, //the chv indication
    SIM_CHV_T unblock_chv,  //the unblock chv
    SIM_CHV_T new_chv      //the new chv
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_RESPONSE_IND_T     *instr_response;
    SIMINSTR_RESULT_TYPE_E          instr_result;

    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_UNBLOCK_CHV);

    //in UNBLOCK CHV instruction,the P2 is the CHV indication
    //if Unblock CHV command applies to CHV1 then P2 = 0x00,
    //if the Unblock CHV command applies to CHV2 then P2 = 0x02
    if(SIM_CHV1 == chv_indication)
    {
#ifdef SIM_3G_USIM_SUPPORT

        if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0;
        }

#ifdef SIM_3G_USIM_SUPPORT
        else
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 1;
        }

#endif
    }
    else
    {
#ifdef SIM_3G_USIM_SUPPORT

        if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = chv_indication;
        }

#ifdef SIM_3G_USIM_SUPPORT
        else
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0x81;
        }

#endif
    }

    //copy the chvs to the instruction data
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, 2 * SIM_CHV_MAX_LEN);
    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, unblock_chv.secret_code, SIM_CHV_MAX_LEN);
    SCI_MEMCPY((s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data + SIM_CHV_MAX_LEN), new_chv.secret_code, SIM_CHV_MAX_LEN);

    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);

            if(SIM_SERVICE_CHV_VER_FAIL == ser_result)
            {
                if(SIM_CHV1 == chv_indication)
                {
#ifdef SIM_3G_USIM_SUPPORT

                    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
                    {
                        g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.false_remain--;
                    }

#ifdef SIM_3G_USIM_SUPPORT
                    else
                    {
                        if(0x63 == instr_response->sw1)
                        {
                            g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.false_remain = ((instr_response->sw2) & 0xF);
                        }
                        else
                        {
                            g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.false_remain--;
                        }
                    }

#endif

                    //double check the unblock chv1 status
                    if(0 == g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.false_remain)
                    {
                        g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.chv_blocked = TRUE;
                        ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
                    }
                }
                else
                {
#ifdef SIM_3G_USIM_SUPPORT

                    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
                    {
                        g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.false_remain--;
                    }

#ifdef SIM_3G_USIM_SUPPORT
                    else
                    {
                        if(0x63 == instr_response->sw1)
                        {
                            g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.false_remain = ((instr_response->sw2) & 0xF);
                        }
                        else
                        {
                            g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.false_remain--;
                        }
                    }

#endif

                    //double check the unblock chv2 status
                    if(0 == g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.false_remain)
                    {
                        g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.chv_blocked = TRUE;
                        ser_result = SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
                    }
                }
            }

            if(SIM_SERVICE_CHV_FAIL_WITH_BLOCKED == ser_result)
            {
                if(SIM_CHV1 == chv_indication)
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.false_remain = 0;
                    g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.chv_blocked = TRUE;
#ifdef SIM_3G_USIM_SUPPORT

                    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                    {
                        s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_ENABLED_AND_BLOCKED;
                    }

#endif
                }
                else
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.false_remain = 0;
                    g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.chv_blocked = TRUE;
                }
            }

            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    if(SIM_CHV1 == chv_indication)
    {
        //if the chv1 is unbloced,the chv1 is also enabled
        g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled = TRUE;
        g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = FALSE;
        g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.false_remain = SIM_MAX_UNBLOCK_CHV_ATTEMPT;
#ifdef SIM_3G_USIM_SUPPORT

        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_VERIFIED;
        }

#endif
    }
    else
    {
        g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.chv_blocked = FALSE;
        g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.false_remain = SIM_MAX_UNBLOCK_CHV_ATTEMPT;
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_UnblockCHV(cur_sim_task, chv_indication, unblock_chv, new_chv);
#endif
}

/**********************************************************************
//    Description:
//      Run the A3 or A8 algorithm of the sim, 3G network
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_RunUMTSAlgorithm(   //return vlaue:the result of the 3G algorithm operation
    uint32 cur_sim_task,
    BOOLEAN is_gsm_network,
    uint8 *alg_rand,    //the rand used in algorithm  IN
    uint8 *autn,          //IN
    uint8 *rsp_data_len, // OUT  response data length of algorithm
    uint8 *alg_result_data //OUT response data of algorithm
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_RESPONSE_IND_T     *instr_response;
    SIMINSTR_RESULT_TYPE_E          instr_result;

    SCI_TRACE_LOW("SIM%d:SIMINSTR_RunUMTSAlgorithm gsm context=%d, rand_len=%d, autn_len=%d", cur_sim_task, is_gsm_network, alg_rand[0], autn[0]);

    //is the DFgsm or any sub directory under DFgsm has been
    //select as the current directory and is chv1 verification
    //procedure has been performed
    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_RUN_ALGORITHM);

#ifdef SIM_3G_USIM_SUPPORT

    //copy the network rand
    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        // 3G UICC??¨¨?§?ê?¨oy?Y??¨o??1¨o?2?¨a?¨?¨22G sim??§??¨¨?§??ê
        //?¨22G ¨a???¨|??á?é2¨￠, 0x80;  ¨¨?1??¨23G ¨a????á?é2¨￠, 0x81.
        if(is_gsm_network)
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0x80;// 2G
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = alg_rand[0] + 1; //
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, alg_rand[0] + 1); //
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[0] = alg_rand[0];// 1st byte is length
            SCI_MEMCPY(&(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[1]), alg_rand + 1, alg_rand[0]);
        }
        else
        {
            /* First byte in rand_val_ptr and autn_val_ptr indicate lengths.
            * Additional 2 bytes for including the lengths in the data. Data
            * will be of the format RAND_LEN,RAND Value,AUTN_LEN, AUTN */
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0x81;// 3G
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = alg_rand[0] + 1 + autn[0] + 1; // 3G
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, (alg_rand[0] + 1 + autn[0] + 1)); // 3G
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[0] = alg_rand[0];
            SCI_MEMCPY(&(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[1]), alg_rand + 1, alg_rand[0]);
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[alg_rand[0] + 1] = autn[0];
            SCI_MEMCPY(&(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[alg_rand[0] + 2]), autn + 1, autn[0]);
        }
    }
    else
#endif
    {
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, SIM_NETWORK_RAND_LEN);
        SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, alg_rand + 1, SIM_NETWORK_RAND_LEN);
    }

    //we don't know what's the response data is, so the response data length is set to zero,  equal to 256
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 0;
    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, SIM_FILE_TOTAL_NUM, instr_response);
        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result)
            && (SIM_STATUS_OK_PROACTIVE_INFO != instr_result)
            && (SIM_STATUS_OK_RESPONSE_DATA != instr_result)
            && (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_CHV1);
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //the last insturction return value is OK and have reponse data need to
        //be get by the GET RESPONSE instruction,so add the GET RESPONSE
        //instruction to the instruction list and set the P3 of GET RESPONSE instruction
        if((SIM_STATUS_OK_RESPONSE_DATA == instr_result)
            && (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr))
        {
            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                               SIMINSTR_GET_RESPONSE);
            s_sim_instruction_list[cur_sim_task].instr_num++;

            //set the P3 of GET RESPONSE instruction
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
        }

        //if the instruction is the last one of the instruction list
        //get the select structure data from the response data
        if(s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr)
        {
            //the response data may contain four case:
            // 1. GSM network, usim card
            // 2. GSM network, sim card
            // 3. 3G network , usim card
            // 4. 3G network , sim card
            // here we just pass the response data up to the caller
            SCI_MEMCPY(alg_result_data, &(instr_response->data[0]), instr_response->data_len);
            *rsp_data_len = instr_response->data_len;
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
#else
    //the code that do the operation with files in hard disk under
    //windows test
#endif
    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      Run the A3 or A8 algorithm of the sim
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_RunGSMAlgorithm(   //return vlaue:the result of the GSM algorithm operation
    uint32 cur_sim_task,
    RAND_T alg_rand,    //the rand used in algorithm
    SIM_GSM_ALGORITHM_RESULT_T *result  //out:the result of the algorithm
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_RESPONSE_IND_T     *instr_response;
    SIMINSTR_RESULT_TYPE_E          instr_result;

    //is the DFgsm or any sub directory under DFgsm has been
    //select as the current directory and is chv1 verification
    //procedure has been performed
    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_RUN_ALGORITHM);

#ifdef SIM_3G_USIM_SUPPORT

    //copy the network rand
    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        //在2G 网络上注册, 0x80;  如果在3G 网络注册, 0x81.
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0x80;// 3G
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = SIM_NETWORK_RAND_LEN + 1; // 3G
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, SIM_NETWORK_RAND_LEN + 1); // 3G
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[0] = SIM_NETWORK_RAND_LEN;// 1st byte is length
        SCI_MEMCPY(&(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[1]), alg_rand, SIM_NETWORK_RAND_LEN);
    }
    else
#endif
    {
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, SIM_NETWORK_RAND_LEN);
        SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, alg_rand, SIM_NETWORK_RAND_LEN);
    }

    if(TB_SIM_T_0_PROTOCOL == g_sim_card_setting[cur_sim_task].t_protocol)
    {
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 0;
    }
    else
    {
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = SIM_RUN_GSM_ALGORITHM_RESPONSE_DATA_LEN;
    }

    //@yongxia.zhang,cr6835,2004-01-12,begin,add
    s_sim_instruction_list[cur_sim_task].instr_num++;
    //cr6835,end

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, SIM_FILE_TOTAL_NUM, instr_response);

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_CHV1);
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //the last insturction return value is OK and have reponse data need to
        //be get by the GET RESPONSE instruction,so add the GET RESPONSE
        //instruction to the instruction list and set the P3 of GET RESPONSE instruction
        if((SIM_STATUS_OK_RESPONSE_DATA == instr_result) &&
            (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr))
        {
            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                               SIMINSTR_GET_RESPONSE);
            s_sim_instruction_list[cur_sim_task].instr_num++;

            //set the P3 of GET RESPONSE instruction
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
        }

        //if the instruction is the last one of the instruction list
        //get the select structure data from the response data
        if(s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr)
        {
#ifdef SIM_3G_USIM_SUPPORT

            if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
            {
                /*
                                SCI_TRACE_LOW("SIM auth resp len %d, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x. sw 0x%x 0x%x",\
                                    instr_response->data_len, instr_response->data[0],instr_response->data[1],instr_response->data[2],instr_response->data[3],instr_response->data[4],\
                                    instr_response->data[5],instr_response->data[6],instr_response->data[7],instr_response->data[8],instr_response->data[9],instr_response->data[10],\
                                    instr_response->data[11],instr_response->data[12],instr_response->data[13]);
                */
                if(instr_response->data_len != (SIM_SERS_LEN + SIM_EF_KC_KC_LEN + 2))
                {
                    //free the response signal
                    SCI_FREE_SIGNAL(instr_response);
                    //clear the instruction list for next use
                    ClearInstrList(cur_sim_task);
                    return SIM_SERVICE_RUN_ALGORITHM_ERROR;
                }

                SCI_MEMCPY((uint8 *)result->sers, &(instr_response->data[1]), sizeof(SRES_T));
                SCI_MEMCPY(result->kc, &(instr_response->data[sizeof(SRES_T) + 1 + 1]), sizeof(KC_T));
            }
            else
#endif//SIM_3G_USIM_SUPPORT
            {
                if(instr_response->data_len != (SIM_SERS_LEN + SIM_EF_KC_KC_LEN))
                {
                    //free the response signal
                    SCI_FREE_SIGNAL(instr_response);
                    //clear the instruction list for next use
                    ClearInstrList(cur_sim_task);
                    return SIM_SERVICE_RUN_ALGORITHM_ERROR;
                }

                SCI_MEMCPY((uint8 *)result->sers, instr_response->data, sizeof(SRES_T));
                SCI_MEMCPY(result->kc, instr_response->data + sizeof(SRES_T), sizeof(KC_T));
            }
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;
#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_RunGSMAlgorithm(alg_rand, result);
#endif
}

/**********************************************************************
//    Description:
//      the envolop of instruction status.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_Status(   //return value:the result of the status opreation
    uint32 cur_sim_task,
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response, //out:the status of the file
    uint8 data_len,  //the data len that want to get
    BOOLEAN *fetch_flag //proactive instruction FETCH
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E    ser_result;
    SIM_SIG_RESPONSE_IND_T *instr_response;
    SIMINSTR_RESULT_TYPE_E instr_result;
#ifdef SIM_3G_USIM_SUPPORT
    SIM_FCP_REF_ARR_T arr_info;
    SIM_FILE_NAME_E file_name;
    uint8 temp_instr_response[255] = {0};
    uint16 temp_instr_response_pos = 0;
    BOOLEAN rsp_flag = FALSE;
    uint16 temp_fcp_len = 0;
    uint8 *temp_fcp_ptr = NULL;
#endif

    SCI_ASSERT(NULL != select_response);/*assert verified: Null_pointer*/

    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_STATUS);

    //set the P3 parameter of the STATUS isntruction
#ifdef SIM_3G_USIM_SUPPORT

    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
    {
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = data_len;
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = data_len;
    }

#ifdef SIM_3G_USIM_SUPPORT
    else
    {
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = 0;//0 means 256
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 256;//max
    }

#endif

    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, SIM_FILE_TOTAL_NUM, instr_response);

        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result) &&
            (SIM_STATUS_OK_RESEND_ADPU != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        if(SIM_STATUS_OK_RESEND_ADPU == instr_result)
        {
            //Re-Send the current instruction, with the new P3.
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].cur_instr--;//the current instruction should redo
        }

        if (SIM_STATUS_OK_PROACTIVE_INFO == instr_result)
        {
            *fetch_flag = TRUE;
        }

#ifdef SIM_3G_USIM_SUPPORT
        //Copy the data into a temp buffer
        if (0xC0 == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_INS])
        {
            uint16 temp_copy_len = 0;

            if (temp_instr_response_pos < 255)//temp buf is not full, you can copy.
            {
                temp_copy_len = 255 - temp_instr_response_pos;
                temp_copy_len = MIN(temp_copy_len,instr_response->data_len);
                SCI_MEMCPY(&temp_instr_response[temp_instr_response_pos],instr_response->data,temp_copy_len);
                temp_instr_response_pos += temp_copy_len;//data length
                rsp_flag = TRUE;
            }
        }

        if((SIM_STATUS_OK_RESPONSE_DATA == instr_result) &&
            (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr))
        {
            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                               SIMINSTR_GET_RESPONSE);
            s_sim_instruction_list[cur_sim_task].instr_num++;

            //set the P3 of GET RESPONSE instruction
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
        }
#endif

        //if the instruction is the last one of the instruction list
        //get the select structure data from the response data
        if(s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr)
        {
            //check the select respone's vailidation
            if(instr_response->data_len < SIM_SELECT_FILE_TYPE_POS)
            {
                SCI_TRACE_LOW("SIM%d: return STATUS rsp len is %d,instr_result=%d,sw2=0x%x.",cur_sim_task,instr_response->data_len,instr_result,instr_response->sw2);

                CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);
                
                //free the response signal
                SCI_FREE_SIGNAL(instr_response);
                //clear the instruction list for next use
                ClearInstrList(cur_sim_task);
                return SIM_SERVICE_SELECT_FILE_ERROR;
            }

            //should not check the status return value for the stauts command will point out the
            //return data length so the return value.

            //copy the output data to the select structure
#ifdef SIM_3G_USIM_SUPPORT

            if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
            {
                if (instr_response->data_len > 34)
                {
                    SCI_TRACE_LOW("SIM: STATUS the len is %d",instr_response->data_len);
                }
            }

            //[wuding] 这里也要判决下3G or 2G ，主要是外面要判决file_id 这项。

            //再考虑，看协议可以P2 = 1，只返回DF name.
#ifdef SIM_3G_USIM_SUPPORT

            if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
            {
                SIMINSTR_RemapSelectResposne(select_response, instr_response->data, instr_response->data_len);
            }

#ifdef SIM_3G_USIM_SUPPORT
            else
            {
                //这里。。。。。。
                file_name = SIMFS_GetCurDirectoryFile(cur_sim_task);

                if (rsp_flag)
                {
                    temp_fcp_len = temp_instr_response_pos;
                    temp_fcp_ptr = temp_instr_response;
                }
                else
                {
                    temp_fcp_len = instr_response->data_len;
                    temp_fcp_ptr = instr_response->data;
                }

                if(!SIMINSTR_DecodeUSIMFCP(cur_sim_task, file_name, temp_fcp_ptr, temp_fcp_len, &arr_info, select_response))
                {
                    SCI_FREE_SIGNAL(instr_response);
                    ClearInstrList(cur_sim_task);//clear the instruction list for next use
                    return SIM_SERVICE_SELECT_DATA_ERROR;
                }
            }

#endif
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;

#else
    //do not support status instruction under windows test
    return SIMTIW_SelectFile(cur_sim_task, SIMFS_GetCurDirectoryFile(cur_sim_task), select_response);
#endif
}

//the SIM application toolkit command envlope function
/**********************************************************************
//    Description:
//      the envelope function of the TERMINAL PROFILE command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_TerminalProfile(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_TERMINAL_PROFILE_DATA_T *profile_ptr //in:the terminal profile
)
{
#ifndef SIM_TEST_IN_WIN32
    SIM_SERVICE_RESULT_E ser_result;
    SIMINSTR_RESULT_TYPE_E instr_result;
    SIM_SIG_RESPONSE_IND_T *instr_response;

    SCI_ASSERT((0 < profile_ptr->profile_len) && (profile_ptr->profile_len <= SIM_MAX_C_APDU_DATA_LEN));/*assert verified: Check_value */

    //get the command code
    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_TERMINAL_PROFILE);
    //alloc the data buf
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, profile_ptr->profile_len);

    ///make sure the memory is allocated
    if(NULL == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data)
    {
        SCI_TRACE_HIGH("SIMINSTR_TerminalProfile null pointer");/*assert verified */
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    //copy the data
    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, profile_ptr->profile_arr, profile_ptr->profile_len);
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = (uint8)profile_ptr->profile_len;
    s_sim_instruction_list[cur_sim_task].instr_num++;

    //execute the instruction list
    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruction from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result) &&
            (SIM_STATUS_OK_DOWN_DATA_ERR != instr_result) &&
            (SIM_STATUS_MEMORY_PROBLEM_65 != instr_result))//A Russian SIM card
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_OK;
#endif  //SIM_TEST_IN_WIN32
}

/**********************************************************************
//    Description:
//      the envelope funtion of the ENVELOPE command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_Envelope(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_ENVELOPE_DATA_T *data_ptr,   //in:the envelope data to the SIM card
    SIM_ENVELOPE_RESPONSE_T *response_ptr, //out:the envelope response data
    SIMAT_ENVELOPE_RESULT_E *result,       //out:the result of the envelope data operation
    BOOLEAN                  delay_fetch,   //delay fetch,because there is proactive command not terminal response to sim
    uint8 *sw1,
    uint8 *sw2
)
{
#ifndef SIM_TEST_IN_WIN32
    SIM_SERVICE_RESULT_E ser_result;
    SIMINSTR_RESULT_TYPE_E instr_result;
    SIM_SIG_RESPONSE_IND_T *instr_response;

    *result = SIMAT_EN_OK;

    response_ptr->data_len = 0;

    //get the command code
    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_ENVELOPE);
    //alloc the data buf
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, data_ptr->data_len);
    ///make sure the memory is alloced
    SCI_ASSERT(NULL != s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data);/*assert verified: Check_value */
    SCI_ASSERT(data_ptr->data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified: Check_value */
    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, data_ptr->data_arr, data_ptr->data_len);
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = (uint8)data_ptr->data_len;
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 2;
    s_sim_instruction_list[cur_sim_task].instr_num++;

    //excute the instruction list
    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        *sw1 = instr_response->sw1;
        *sw2 = instr_response->sw2;

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, SIM_FILE_TOTAL_NUM, instr_response);

        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_DOWN_DATA_ERR != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            if(SIM_STATUS_TOOLKIT_BUSY == instr_result)
            {
                *result = SIMAT_EN_SIM_BUSY;
                ser_result = SIM_SERVICE_OK;
            }
            else
            {
                //An unicom 186xxx return 6F00 in SMS control and Call control
                if(((0xD4 == data_ptr->data_arr[0]) || (0xD5 == data_ptr->data_arr[0]))
                    && (0x0 != data_ptr->data_len)
                    && ((SIM_STATUS_TECHNICAL_PROBLEM == instr_result) || (SIM_STATUS_UNKNOW_INSTRUCTION_CODE == instr_result)))
                {
                    *result = SIMAT_EN_OK;
                    ser_result = SIM_SERVICE_OK;
                    instr_response->data_len = 0;
                }
                else
                {
                    *result = SIMAT_EN_ERROR_UNKNOW;
                    //analysis the failed reason
                    ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                                     instr_result, SIM_ACCESS_ALW);
                }
            }

            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        if(SIM_STATUS_OK_PROACTIVE_INFO == instr_result)
        {
            //if the first status word for envelope command is 9f xx,then the status word of get response is
            //(xx length of data) 91 yy,so the result of envelope should be envelope with data
            if(instr_response->data_len == 0)
            {
                *result = SIMAT_EN_OK_WITH_PC;
            }
        }

        //if the ME support the status workd '9e xx',and if the SIM reposne with '9e xx'
        //the ME shall use the GET RESPONSE command to get the response data
        if((SIM_STATUS_OK_DOWN_DATA_ERR == instr_result) &&
            (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr))
        {
            *result = SIMAT_EN_ERROR_WITH_DATA;
            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                               SIMINSTR_GET_RESPONSE);
            s_sim_instruction_list[cur_sim_task].instr_num++;

            //set the P3 of GET RESPONSE instruction
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
        }

        //the last insturction return value is OK and have reponse data need to
        //be get by the GET RESPONSE instruction,so add the GET RESPONSE
        //instruction to the instruction list and set the P3 of GET RESPONSE instruction
        if((SIM_STATUS_OK_RESPONSE_DATA == instr_result) &&
            (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr))
        {
            *result = SIMAT_EN_OK_WITH_DATA;
            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                               SIMINSTR_GET_RESPONSE);
            s_sim_instruction_list[cur_sim_task].instr_num++;

            //set the P3 of GET RESPONSE instruction
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
        }

        //if the instruction is the last one of the instruction list and the instruction is a
        //GET REPSONSE command,then get the response data from SIM to the ENVELOPE command response buf
        if((s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr) &&
            (s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name == SIMINSTR_GET_RESPONSE))
        {
            response_ptr->data_len = instr_response->data_len;
            SCI_ASSERT(response_ptr->data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified: Check_value */
            SCI_MEMCPY(response_ptr->data_arr, instr_response->data, response_ptr->data_len);
        }

        //if there are no more proactive command from the SIM,the current STK
        //application session is terminated
        if(!delay_fetch)
        {
            CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);
        }

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_OK;
#endif  //SIM_TEST_IN_WIN32    
}


/**********************************************************************
//    Description:
//      the envelope funtion of the FETCH command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_Fetch(    //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FETCH_DATA_T *data_ptr    //in/out:the proactive SIM command data from the SIM card to ME
)
{
#ifndef SIM_TEST_IN_WIN32
    SIM_SERVICE_RESULT_E ser_result;
    SIMINSTR_RESULT_TYPE_E instr_result;
    SIM_SIG_RESPONSE_IND_T *instr_response;
    SIMINSTR_CODE_T fetch_instr_code;

    SCI_ASSERT(data_ptr->data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified: Check_value */

    //get the command code
    GetInstructionCode(cur_sim_task, &fetch_instr_code, SIMINSTR_FETCH);

    //decide the P3 in instruction
    if(SIM_MAX_R_APDU_DATA_LEN == data_ptr->data_len || 0 == data_ptr->data_len)
    {
        fetch_instr_code.apdu.instr_code[TB_SIM_APDU_P3] = 0;
        fetch_instr_code.apdu.rx_expected_len = SIM_MAX_R_APDU_DATA_LEN;
    }
    else
    {
        fetch_instr_code.apdu.instr_code[TB_SIM_APDU_P3] = (uint8)data_ptr->data_len;
        fetch_instr_code.apdu.rx_expected_len = data_ptr->data_len;
    }

    //send a instruction to the sim card
    SendInstrCode(cur_sim_task, &(fetch_instr_code.apdu));

    //receive the response of the instruciton from the sim card
    ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);

    //the instruction is process error
    if((SIM_STATUS_OK_NORMAL != instr_result) &&
        (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
        (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
        (SIM_STATUS_OK_WITH_UPDATE != instr_result))
    {
        //analysis the failed reason
        ser_result = HandleCommandsError(cur_sim_task, fetch_instr_code.instr_name,
                                         instr_result, SIM_ACCESS_ALW);
        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
        return ser_result;
    }

    //if the instruction is the last one of the instruction list
    //get the fetch data from the response data
    data_ptr->data_len = instr_response->data_len;
    SCI_ASSERT(data_ptr->data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified: Check_value */
    SCI_MEMCPY(data_ptr->data_arr, instr_response->data, data_ptr->data_len);

    //free the response signal
    SCI_FREE_SIGNAL(instr_response);
    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_OK;
#endif  //SIM_TEST_IN_WIN32
}

/**********************************************************************
//    Description:
//      the envelope funtion of the TERMINAL RESPONSE command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_TerminalResponse( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_TERMINAL_RESPONSE_DATA_T *data_ptr,  //in:the Terminal response data from the ME to SIM
    uint8* sw1,
    uint8* sw2
)
{
#ifndef SIM_TEST_IN_WIN32
    SIM_SERVICE_RESULT_E ser_result;
    SIMINSTR_RESULT_TYPE_E instr_result;
    SIM_SIG_RESPONSE_IND_T *instr_response;

    SCI_ASSERT((0 < data_ptr->response_len) && (data_ptr->response_len <= SIM_MAX_C_APDU_DATA_LEN));/*assert verified: Check_value */

    //get the command code
    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_TERMINAL_RESPONSE);
    //alloc the data buf
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, data_ptr->response_len);
    ///make sure the memory is alloced
    SCI_ASSERT(NULL != s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data);/*assert verified: Null_pointer*/
    //copy the data
    SCI_ASSERT(data_ptr->response_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified: Check_value */
    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, data_ptr->data_arr, data_ptr->response_len);
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = (uint8)data_ptr->response_len;
    s_sim_instruction_list[cur_sim_task].instr_num++;

    //excute the instruction list
    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        *sw1 = instr_response->sw1;
        *sw2 = instr_response->sw2;

        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        if(!CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2))
        {
            SIMSIGNAL_IndStkSessionTerminate(cur_sim_task);
        }

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_OK;
#endif  //SIM_TEST_IN_WIN32
}

#ifdef SIM_TEST_IN_WIN32
/**********************************************************************
//    Description:
//      get the information from the file select data if the select data
//    is ef file info,the function return TRUE,else the function return
//    FALSE,and get the df or mf info
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMINSTR_DecodeSelectData(  //return value:the type of info that decode out
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data,   //the select data to be decoded
    SIM_EF_FILE_INFO_T *ef_info, //out:buf to save the ef data info
    SIM_DF_MF_INFO_T *df_info    //out bug to save the df data info
)
{
    BOOLEAN return_value;

    //the file type is a EF,it's a ef info
    if(SIM_FILE_T_EF == select_data->file_type)
    {
        return_value = TRUE;

        ef_info->file_id = select_data->file_id;
        ef_info->file_size = select_data->freespace_filesize;
        ef_info->file_type = SIM_FILE_T_EF;
        GET_FILE_ACCESS_CONDITION(ef_info->access_condition, select_data);
        ef_info->invalidate = (BOOLEAN)((select_data->file_status & SIM_FILE_INVALIDATE_MARK) == 0);
        ef_info->invalidate_ru = (BOOLEAN)((select_data->file_status & SIM_FILE_INVALIDATE_RU_MARK) != 0);
        ef_info->file_structure = (SIM_EF_STRUCTURE_E)select_data->filecharacter_efstructure;

        //the file is a fixed linear or cyclic file
        if((SIM_EF_S_LINEAR_FIXED == ef_info->file_structure) ||
            (SIM_EF_S_CYCLIC == ef_info->file_structure))
        {
            ef_info->record_len = select_data->dfnumber_recordlen;
        }

        //the file is a cyclic file
        if(SIM_EF_S_CYCLIC == ef_info->file_structure)
        {
            ef_info->increase_allowed = (BOOLEAN)((select_data->cyclic_full_check & SIM_FILE_INCREASE_ALLOWED_MARK) != 0);
        }
    }
    //else it is a DF or MF info
    else
    {
        return_value = FALSE;

        df_info->file_id = select_data->file_id;
        df_info->free_space = select_data->freespace_filesize;
        df_info->file_type = (SIM_FILE_TYPE_E)select_data->file_type;
        df_info->chv1_enable = (BOOLEAN)((select_data->filecharacter_efstructure & SIM_GSM_SPECIFIC_CHV1_ENABLE_MARK) == 0);
        df_info->clock_stop = (SIM_CLOCK_STOP_TYPE_E)(select_data->filecharacter_efstructure & SIM_GSM_SPECIFIC_CLOCK_STOP_MARK);
        df_info->df_child_num = select_data->dfnumber_recordlen;
        df_info->ef_child_num = select_data->ef_number;

        //the chv1 status from the select response data's chv1 status byte
        df_info->chv1_status.chv_blocked = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data->chv1_status) == 0);
        df_info->chv1_status.false_remain = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data->chv1_status;
        df_info->chv1_status.secrect_initialized = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data->chv1_status) != 0);
        //the chv2 status from the select response data's chv2 status byte
        df_info->chv2_status.chv_blocked = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data->chv2_status) == 0);
        df_info->chv2_status.false_remain = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data->chv2_status;
        df_info->chv2_status.secrect_initialized = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data->chv2_status) != 0);
        //the unblock chv1 status from the select response data's unblock chv1 status byte
        df_info->unblock_chv1_status.chv_blocked = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data->unblock_chv1_status) == 0);
        df_info->unblock_chv1_status.false_remain = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data->unblock_chv1_status;
        df_info->unblock_chv1_status.secrect_initialized = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data->unblock_chv1_status) != 0);
        //the unblock chv2 status from the select response data's unblock chv2 status byte
        df_info->unblock_chv2_status.chv_blocked = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data->unblock_chv2_status) == 0);
        df_info->unblock_chv2_status.false_remain = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data->unblock_chv2_status;
        df_info->unblock_chv2_status.secrect_initialized = (BOOLEAN)((SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data->unblock_chv2_status) != 0);
    }

    return return_value;
}

/**********************************************************************
//    Description:
//      Endcode a Ef info to a select data of the EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_EncodeEFInfo( //retyrn value:the result of the encode operation
    SIM_EF_FILE_INFO_T ef_info,   //the ef info to be encode in to select data
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data   //out:the pointer of the buf that save the encoded select data
)
{
    SCI_ASSERT(NULL != select_data);/*assert verified */

    //set the full data to 0xff
    SCI_MEMSET(select_data, 0xff, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));

    //encode the EF file info to the select data
    select_data->file_type = SIM_FILE_T_EF;
    select_data->file_id = ef_info.file_id;
    select_data->freespace_filesize = ef_info.file_size;

    //the encode will be different if the structure of EF file is different
    if(SIM_EF_S_CYCLIC == ef_info.file_structure)
    {
        select_data->filecharacter_efstructure  = SIM_EF_S_CYCLIC;

        if(!ef_info.increase_allowed)
        {
            select_data->cyclic_full_check &= ~SIM_FILE_INCREASE_ALLOWED_MARK;
        }

        select_data->dfnumber_recordlen = ef_info.record_len;
    }
    else if(SIM_EF_S_LINEAR_FIXED == ef_info.file_structure)
    {
        select_data->filecharacter_efstructure = SIM_EF_S_LINEAR_FIXED;
        select_data->dfnumber_recordlen = ef_info.record_len;
    }
    else
    {
        select_data->filecharacter_efstructure = SIM_EF_S_TRANSPARENT;
    }

    //the validate of the file
    if(ef_info.invalidate)
    {
        select_data->file_status &= ~SIM_FILE_INVALIDATE_MARK;
    }

    if(!ef_info.invalidate_ru)
    {
        select_data->file_status &= ~SIM_FILE_INVALIDATE_RU_MARK;
    }

    //encode the access contdition of the EF file
    select_data->access_condition1 = (((uint16)ef_info.access_condition.increase_access) << 12) + (0x000f << 8)
                                     + (((uint16)ef_info.access_condition.seek_read_access) << 4) + (uint16)ef_info.access_condition.update_access;
    select_data->access_condition2 = (((uint8)ef_info.access_condition.rehabilitate_access) << 4) + (uint8)ef_info.access_condition.invalidate_access;

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      Endcode a DF info to a select data of the DF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_EncodeDFInfo( //return value:the result of the Encode operation
    SIM_DF_MF_INFO_T df_info, //the DF info to be encoded in to select data
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data    //out:save the select data that encoded into
)
{
    SCI_ASSERT(NULL != select_data);/*assert verified */

    //set the full data to 0xff
    SCI_MEMSET(select_data, 0xff, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));

    //encode the CHV1 status of the DF
    if(df_info.chv1_status.secrect_initialized)
    {
        select_data->chv1_status &= ~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK;
        select_data->chv1_status |= df_info.chv1_status.false_remain;
    }
    else
    {
        select_data->chv1_status &= ~SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK;
        select_data->chv1_status &= ~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK;
        select_data->chv1_status |= df_info.chv1_status.false_remain;
    }

    //encode the CHV2 status of the DF
    if(df_info.chv2_status.secrect_initialized)
    {
        select_data->chv2_status &= ~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK;
        select_data->chv2_status |= df_info.chv2_status.false_remain;
    }
    else
    {
        select_data->chv2_status &= ~SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK;
        select_data->chv2_status &= ~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK;
        select_data->chv2_status |= df_info.chv2_status.false_remain;
    }

    //encode the Unblock CHV1 status of the DF
    if(df_info.unblock_chv1_status.secrect_initialized)
    {
        select_data->unblock_chv1_status &= ~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK;
        select_data->unblock_chv1_status |= df_info.unblock_chv1_status.false_remain;
    }
    else
    {
        select_data->unblock_chv1_status &= ~SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK;
        select_data->unblock_chv1_status &= ~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK;
        select_data->unblock_chv1_status |= df_info.unblock_chv1_status.false_remain;
    }

    //encode the Unblock CHV2 status of the DF
    if(df_info.unblock_chv2_status.secrect_initialized)
    {
        select_data->unblock_chv2_status &= ~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK;
        select_data->unblock_chv2_status |= df_info.unblock_chv2_status.false_remain;
    }
    else
    {
        select_data->unblock_chv2_status &= ~SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK;
        select_data->unblock_chv2_status &= ~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK;
        select_data->unblock_chv2_status |= df_info.unblock_chv2_status.false_remain;
    }

    //the DF info encode
    select_data->dfnumber_recordlen = df_info.df_child_num;
    select_data->ef_number = df_info.ef_child_num;
    select_data->file_id = df_info.file_id;
    select_data->file_type = df_info.file_type;
    select_data->freespace_filesize = df_info.free_space;

    //encode the file character of the df info
    if(df_info.chv1_enable)
    {
        select_data->filecharacter_efstructure &= ~SIM_GSM_SPECIFIC_CHV1_ENABLE_MARK;
    }

    select_data->filecharacter_efstructure &= ~SIM_GSM_SPECIFIC_CLOCK_STOP_MARK;
    select_data->filecharacter_efstructure |= df_info.clock_stop;


    return SIM_SERVICE_OK;
}
#endif//SIM_TEST_IN_WIN32

#ifndef SIM_TEST_IN_WIN32
/**********************************************************************
//    Description:
//      Send instruction code to the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void SendInstrCode(
    uint32 cur_sim_task,
    TB_SIM_APDU_T *apdu_ptr    //in:the instruction code to be sent to SIM card
)
{
    uint32 card_id = cur_sim_task;
    uint32 total_wait_time_value = 0;
    uint32 rx_expected_len = 0;

    //When notify the low layer,we should change the card_id when SIM slot changed!
    card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

    total_wait_time_value = SIMHAL_GetWWT(card_id);

    if(SIMHAL_SIMINSTR_NONE_DATA == apdu_ptr->instr_type)
    {
        //two SW bytes
        rx_expected_len = TB_SIM_RESPONSE_DATA_EXPEND_LEN;//the 2 byte status words.
    }
    else if(SIMHAL_SIMINSTR_RECV_DATA == apdu_ptr->instr_type)
    {
        //A procedure byte and Rx_data and two SW bytes.
        rx_expected_len = (uint32)(1 + apdu_ptr->rx_expected_len + TB_SIM_RESPONSE_DATA_EXPEND_LEN);
    }
    else
    {
        //A procedure byte and Tx_data and two SW bytes.
        rx_expected_len = (uint32)(1 + apdu_ptr->instr_code[TB_SIM_APDU_P3] + TB_SIM_RESPONSE_DATA_EXPEND_LEN);//the 2 byte status words.
    }

    rx_expected_len++;//increase for more compability

    total_wait_time_value = total_wait_time_value * rx_expected_len;
#ifdef SIM_DROP_RESTORE_SUPPORT
    total_wait_time_value = SIMINSTR_CountWaitTime(cur_sim_task,rx_expected_len);
#endif
    ActiveInstructionTimer(cur_sim_task, total_wait_time_value);

    SIMHAL_SendInstrCode(card_id, apdu_ptr);
}
#endif

/**********************************************************************
//    Description:
//      Receive the reponse data from the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E ReceiveInstrResponse(   //return value:the type of the singal that send by SIM card interrupt
    uint32 cur_sim_task,
    SIM_SIG_RESPONSE_IND_T **instr_response //the pointer to save the pointer of the self response signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    ser_result = SIMINSTR_ReceiveInstrResponse(cur_sim_task, instr_response);
    DeactiveInstructionTimer(cur_sim_task);
    return ser_result;
}

/**********************************************************************
//    Description:
//     Judge whether the current instruction is stk command or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_IsSTKCommand(uint32 cur_sim_task)
{
    if((s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name == SIMINSTR_TERMINAL_PROFILE) ||
        (s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name == SIMINSTR_ENVELOPE) ||
        (s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name == SIMINSTR_FETCH) ||
        (s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name == SIMINSTR_TERMINAL_RESPONSE))
    {
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************
//    Description:
//      the implementation of select a SIM file by the specified file path
//    and get the select data
//    of the destionation file by using the SIM instructions
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E SIMINSTR_SimSelectFileByPath(   //return value:the result of the operation
    uint32 cur_sim_task,
    uint16 file_id,    //the file to be selected
    uint8 route_len,  //the file route data length
    uint16 *file_route, //the file path to access the file
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response    //the select data of the file
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    uint16 data_len = sizeof(SIM_RESPONSE_DATA_OF_SELECT_T);
    int8 instr_num, i = 0;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_RESPONSE_IND_T *instr_response;
    SIMINSTR_RESULT_TYPE_E instr_result;
    uint16 temp_file_id;
    uint16 tmp_file_route[8] = {0};
    SIM_FILE_NAME_E file_name;
    uint8 route_data[SIM_FILE_ROUTE_LEN] = {0}; //file level will not exceed 8
    uint8 temp_path_len = 0;

    if(0 == route_len)
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //add all the select instruction that needed to select desitination
    //file to the instruction list
    instr_num = 0;

    //if the path route is not from MF, we add it
    if(file_route[0] != 0x3F00)
    {
        tmp_file_route[instr_num] = 0x3F00;
        instr_num++;
    }

    for(i = 0; i < route_len ; i++)
    {
        tmp_file_route[instr_num] = file_route[i];
        instr_num++;
    }

    //if the path route do not contain file id to be selected, we add it
    if(file_route[route_len-1] != file_id)
    {
        tmp_file_route[instr_num] = file_id;
        instr_num++;
    }

    for(i = 0; i < instr_num; i++)
    {
        route_data[i*2] = (uint8)(tmp_file_route[i] >> 8);
        route_data[i*2+1] = (uint8)tmp_file_route[i];
    }

    file_name = SIMFS_GetFileName(cur_sim_task, file_id, (i * 2), route_data);

    //if the current file is just the file you want to select, you don't need to select it again.
    if((SIMFS_GetCurFileName(cur_sim_task) == file_name)
        && (SIMFS_GetFileID(cur_sim_task, file_name) == s_sim_cur_file_response[cur_sim_task].file_id))
    {
        SCI_MEMCPY(select_response, &s_sim_cur_file_response[cur_sim_task], sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
        // copy the related SW and response string
        s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];
        return SIM_SERVICE_OK;
    }

    i = 0;

    //add all the select instruction code coresponding the file int
    //the file route to instruction list
    while(i < instr_num)
    {
        //get the file id
        temp_file_id = tmp_file_route[i];

        if((0x7000 == (temp_file_id & 0xF000))
            || (0x6000 == (temp_file_id & 0xF000))
            || (0x5000 == (temp_file_id & 0xF000))
            || ((0x4000 == (temp_file_id & 0xF000)) && (0xFF != (temp_file_id & 0xFF)))
            || (0x3000 == (temp_file_id & 0xF000))
            || (0x2000 == (temp_file_id & 0xF000)))
        {
        }
        else
        {
            ClearInstrList(cur_sim_task);
            return SIM_SERVICE_SELECT_NO_SUCH_FILE;
        }

        //add a select instruction to the instruction list
        GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                           SIMINSTR_SELECT);

        //copy the file id to the select instruction data
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, 2);

        //Raislin.Kong 2002-8-14
        //add the transition of the fileid(uint16) to 2 byte uint8,not just memcpy it to the buf.
        //why do this is to make the sim task work in both little ended and big ended system
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[0] = (uint8)(temp_file_id >> 8);
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[1] = (uint8)temp_file_id;

        //the expected response data length from the SIM card may decided by the transmit protocol of the SIM card
        if(TB_SIM_T_0_PROTOCOL == g_sim_card_setting[cur_sim_task].t_protocol)
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 0;
        }
        else
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = SIM_MAN_DF_SELECT_DATA_LEN;
        }

        s_sim_instruction_list[cur_sim_task].instr_num++;
        i++;
    }

    SCI_TRACE_LOW("SIM%d:SELECT %x by Path, %d %d, file_route 0x%x 0x%x 0x%x 0x%x 0x%x", cur_sim_task, file_id, s_sim_instruction_list[cur_sim_task].cur_instr,
                  s_sim_instruction_list[cur_sim_task].instr_num, tmp_file_route[0], tmp_file_route[1], tmp_file_route[2], tmp_file_route[3], tmp_file_route[4]);

    i = 0;      //back the index of the file route

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, file_name, instr_response);

        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
		
        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result) &&
            (SIM_STATUS_OK_RESEND_ADPU != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);

            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //if a select instruction is successful
        if(SIMINSTR_SELECT == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name)
        {
            SCI_TRACE_LOW("SIM:set cur file id 0x%x.", tmp_file_route[i]);

            //update the cur select file
            temp_path_len = i * 2;
            SIMFS_SetCurDirectoryFile(cur_sim_task, SIMFS_GetFileName(cur_sim_task, tmp_file_route[i], temp_path_len, route_data));

            //update the cur select file
            SIMFS_SetCurFileName(cur_sim_task, SIMFS_GetFileName(cur_sim_task, tmp_file_route[i], temp_path_len, route_data));

            i++;
        }

        if(SIM_STATUS_OK_RESEND_ADPU == instr_result)
        {
            //Re-Send the current instruction, with the new P3.
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].cur_instr--;//the current instruction should redo
        }

        //the last insturction return value is OK and have reponse data need to
        //be get by the GET RESPONSE instruction,so add the GET RESPONSE
        //instruction to the instruction list and set the P3 of GET RESPONSE instruction
        if((SIM_STATUS_OK_RESPONSE_DATA == instr_result) &&
            (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr))
        {
            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                               SIMINSTR_GET_RESPONSE);
            s_sim_instruction_list[cur_sim_task].instr_num++;

            //set the P3 of GET RESPONSE instruction
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
        }

        //if the instruction is the last one of the instruction list
        //get the select structure data from the response data
        if(s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr)
        {
            //file_name = SIMFS_GetFileName(cur_sim_task,file_id,(route_len*2),(uint8*)route_data);
            //check the select respone's vailidation
            if(instr_response->data_len < SIM_SELECT_FILE_TYPE_POS)
            {
                //free the response signal
                SCI_FREE_SIGNAL(instr_response);
                //clear the instruction list for next use
                ClearInstrList(cur_sim_task);
                return SIM_SERVICE_SELECT_DATA_ERROR;
            }

            //the file is a MF or DF
            if((SIM_FILE_T_MF == instr_response->data[SIM_SELECT_FILE_TYPE_POS - 1]) ||
                (SIM_FILE_T_DF == instr_response->data[SIM_SELECT_FILE_TYPE_POS - 1]))
            {
                //the response len less than the mandatory length of the
                //select file response of a MF or DF,the response is invalidate
                if(instr_response->data_len < SIM_SELECT_DF_MANDATORY_LEN)
                {
                    //free the response signal
                    SCI_FREE_SIGNAL(instr_response);
                    //clear the instruction list for next use
                    ClearInstrList(cur_sim_task);
                    return SIM_SERVICE_SELECT_DATA_ERROR;
                }
            }
            //the file is EF
            else if(SIM_FILE_T_EF == instr_response->data[SIM_SELECT_FILE_TYPE_POS - 1])
            {
                //the response len less than the mandatory length of the
                //select file response of a EF,the response is invalidate
                if(instr_response->data_len < SIM_SELECT_EF_MANDATORY_LEN)
                {
                    //free the response signal
                    SCI_FREE_SIGNAL(instr_response);
                    //clear the instruction list for next use
                    ClearInstrList(cur_sim_task);
                    return SIM_SERVICE_SELECT_DATA_ERROR;
                }
            }
            //the file type is worng,the select response is invalidate
            else
            {
                //free the response signal
                SCI_FREE_SIGNAL(instr_response);
                //clear the instruction list for next use
                ClearInstrList(cur_sim_task);
                return SIM_SERVICE_SELECT_DATA_ERROR;
            }

            //get the min value of length in instr response and the select data structure data len
            //used to copy the need data to the select data structure
            if(instr_response->data_len < sizeof(SIM_RESPONSE_DATA_OF_SELECT_T))
            {
                data_len = instr_response->data_len;
            }

            //copy the output data to the select structure
            SCI_ASSERT(data_len <= sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));  /*assert verified*/

            SIMINSTR_RemapSelectResposne(select_response, instr_response->data, data_len);

            SCI_MEMCPY(&s_sim_cur_file_response[cur_sim_task], select_response, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
            s_select_sw_and_rsp[cur_sim_task] = s_sim_rsp_info[cur_sim_task];
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIM_SERVICE_INVALIDATE_FILE_NAME;
#endif
}

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//      the implementation of select a USIM file by the specified file path
//    and get the select data
//    of the destionation file by using the SIM instructions
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E SIMINSTR_UsimSelectFileByPath(   //return value:the result of the operation
    uint32 cur_sim_task,
    uint16 file_id,    //the file to be selected
    uint8 route_len,  //the file route data length
    uint16 *file_route, //the file path to access the file
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response    //the select data of the file
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_RESPONSE_IND_T *instr_response;
    SIMINSTR_RESULT_TYPE_E instr_result;
    SIM_FILE_NAME_E file_name;
    uint8 i, path_len = 0;
    uint8 temp_data_h = 0;
    uint8 temp_data_l = 0;
    uint8 file_id_h, file_id_l;
    uint8 route_data_len = 0;
    uint8 route_data[SIM_FILE_ROUTE_LEN] = {0}; //file level will not exceed 8
    SIM_FCP_REF_ARR_T arr_info;

    if((0x7000 == (file_id & 0xF000))
        || (0x6000 == (file_id & 0xF000))
        || (0x5000 == (file_id & 0xF000))
        || ((0x4000 == (file_id & 0xF000)) && (0xFF != (file_id & 0xFF)))
        || (0x3000 == (file_id & 0xF000))
        || (0x2000 == (file_id & 0xF000)))
    {
    }
    else
    {
        ClearInstrList(cur_sim_task);
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

    file_id_h = (uint8)(file_id >> 8);
    file_id_l = (uint8) file_id;

    //record the path data
    for(i = 0 ; i < route_len; i++)
    {
        temp_data_h = (uint8)(file_route[i] >> 8);
        temp_data_l = (uint8)file_route[i];

        //according to protocol, select from MF should not contain file id of MF
        if((temp_data_h == 0x3F) && (temp_data_l == 0x0))
        {
            continue;
        }

        route_data[path_len] = temp_data_h;
        path_len ++;
        route_data[path_len] = temp_data_l;
        path_len ++;
    }


    if(path_len < 2)
    {
        path_len = 0;
        //Case1: file_id=2Fxx,path_len=2, path_id="3F00".
        //Case2: file_id=2Fxx,path_len=0, path_id="".
        route_data[0] = file_id_h;
        route_data[1] = file_id_l;
        route_data_len = 2;
    }
    else if((route_data[path_len-1] != file_id_l) || (route_data[path_len-2] != file_id_h))
    {
        route_data[path_len] = file_id_h;
        path_len++;
        route_data[path_len] = file_id_l;
        path_len++;
        route_data_len = path_len;
    }
    else if((route_data[path_len-1] == file_id_l) || (route_data[path_len-2] == file_id_h))
    {
        //Maybe the user will fill the file_id in the path
        route_data_len = path_len;
    }

    file_name = SIMFS_GetFileName(cur_sim_task, file_id, path_len, route_data);

    //special case to adaptive SIM-6f3a USIM-ADN_1
    if (SIM_FILE_TOTAL_NUM == file_name)
    {
        if ((0x6f3a == file_id) 
            && (0 != path_len)
            && (USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task]))
        {
            uint16 temp_remap_id = 0;
            file_name = SIM_FILE_EF_TELECOM_PB_ADN;
            route_data[0] = 0x7F;
            route_data[1] = 0x10;
            route_data[2] = 0x5F;
            route_data[3] = 0x3A;
            temp_remap_id = SIMFS_GetFileID(cur_sim_task,file_name);
            route_data[4] = (uint8)(temp_remap_id >> 8);
            route_data[5] = (uint8)temp_remap_id;
            route_data_len = 6;
            path_len = 6;
        }
    }

    //if the current file is just the file you want to select, you don't need to select it again.
    if((SIMFS_GetCurFileName(cur_sim_task) == file_name)
        && (SIMFS_GetFileID(cur_sim_task, file_name) == s_sim_cur_file_response[cur_sim_task].file_id))
    {
        SCI_MEMCPY(select_response, &s_sim_cur_file_response[cur_sim_task], sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
        // copy the related SW and response string
        s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];
        return SIM_SERVICE_OK;
    }

    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                       SIMINSTR_SELECT);

    if(route_data_len < 2)
    {
        SCI_TRACE_LOW("SIMINSTR_UsimSelectFileByPath fail route_data_len is 0. file_id=0x%x, 0x%x 0x%x, path_len=%d", file_id, route_data[0], route_data[1], path_len);
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, route_data_len);
    SCI_ASSERT(NULL != s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data); /*assert verified*/
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = SIM_SELECT_CMD_P1_PATH_FROM_MF;
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = SIM_SELECT_CMD_P2_RET_FCP_TEMPLATE;
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = route_data_len;
    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, route_data, route_data_len);

    s_sim_instruction_list[cur_sim_task].instr_num++;

    i = 0;

    SCI_TRACE_LOW("SIM%d:SELECT %x by Path, %d %d, file_route 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x", cur_sim_task, file_id, s_sim_instruction_list[cur_sim_task].cur_instr,
                  s_sim_instruction_list[cur_sim_task].instr_num, route_data[0], route_data[1], route_data[2], route_data[3], route_data[4], route_data[5], route_data[6], route_data[7]);

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, file_name, instr_response);

        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result) &&
            (SIM_STATUS_OK_RESEND_ADPU != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);

            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //if a select instruction is successful
        if(SIMINSTR_SELECT == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name)
        {
            SIM_FILE_NAME_E temp_name;
            uint8 temp_len = 0;
            uint8 temp_buf[16] = {0};

            //temp_name = SIMFS_GetFileName(cur_sim_task,file_id,(route_len*2),(uint8*)route_data);
            temp_name = file_name;
            SCI_TRACE_LOW("SIM:set cur file id 0x%x, file_name is %d", file_id, temp_name);

            if(SIM_FILE_TOTAL_NUM == temp_name) //cannot find the file_name
            {
                uint16 temp_df_id = 0;

                //To get the DF name
                temp_len = route_len * 2;
                SCI_MEMCPY(temp_buf, route_data, temp_len);

                if(temp_len > 2)
                {
                    if ((0x20 == (temp_buf[temp_len-2] & 0xF0)) 
                        || (0x40 == (temp_buf[temp_len-2] & 0xF0)) 
                        || (0x60 == (temp_buf[temp_len-2] & 0xF0)))
                    {
                        temp_len -= 2;//the last two bytes are file_id, remove it.
                    }

                    if(temp_len >= 2)
                    {
                        temp_df_id = temp_buf[temp_len-2];
                        temp_df_id <<= 8;
                        temp_df_id |= temp_buf[temp_len-1];
                        temp_name = SIMFS_GetFileName(cur_sim_task, temp_df_id, temp_len, temp_buf);
                    }
                }

                SIMFS_SetCurDirectoryFile(cur_sim_task, temp_name);
            }
            else
            {
                SIMFS_SetCurDirectoryFile(cur_sim_task, temp_name); //Set the current DF name
            }

            SIMFS_SetCurFileName(cur_sim_task, temp_name); //set the current EF name

            i++;
        }

        if(SIM_STATUS_OK_RESEND_ADPU == instr_result)
        {
            //Re-Send the current instruction, with the new P3.
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].cur_instr--;//the current instruction should redo
        }

        //the last insturction return value is OK and have reponse data need to
        //be get by the GET RESPONSE instruction,so add the GET RESPONSE
        //instruction to the instruction list and set the P3 of GET RESPONSE instruction
        if((SIM_STATUS_OK_RESPONSE_DATA == instr_result) &&
            (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr))
        {
            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                               SIMINSTR_GET_RESPONSE);
            s_sim_instruction_list[cur_sim_task].instr_num++;


            //set the P3 of GET RESPONSE instruction
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
        }

        //if the instruction is the last one of the instruction list
        //get the select structure data from the response data
        if(s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr)
        {
            //file_name = SIMFS_GetFileName(cur_sim_task,file_id,(route_len*2),(uint8*)route_data);
            if(SIM_SELECT_CMD_P2_NO_DATA_RETURNED == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P2])
            {
                //No FCP data return, do nothing.
            }
            else
            {
                if(SIMINSTR_DecodeUSIMFCP(cur_sim_task, file_name, instr_response->data, instr_response->data_len, &arr_info, select_response))
                {
#ifdef YWD_REF_ARR_SELF_TEST

                    //all the EFs need to check
                    if((SIM_FILE_EF_USIM_ARR != file_name) && (SIM_FILE_EF_MF_ARR != file_name) && (SIM_FILE_EF_TELECOM_ARR != file_name)
                        && (file_name >= SIM_FILE_EF_ICCID))
#else

                    //only several EFs need to check
                    if((SIM_FILE_EF_USIM_ACM == file_name) || (SIM_FILE_EF_USIM_ACMMAX == file_name) || (SIM_FILE_EF_USIM_PUCT == file_name)
                        || (SIM_FILE_EF_USIM_VGCSS == file_name) || (SIM_FILE_EF_USIM_VBSS == file_name))
#endif
                    {
                        if(arr_info.is_ref_exist)
                        {
                            //We want to get the EFacm/acmmax/puct/vgcss/vbss's access condition for UPDATE.
                            // Is the access condition is PIN/PIN2 , PIN/ADM?
                            // In fact, EFict,EFoct,EFmsucp,EFmsisdn(under ADFusim) are also need. But we donnot support these EFs now.
                            if(!SIMINSTR_GetARR(cur_sim_task, file_name, arr_info, &(select_response->access_condition1), &(select_response->access_condition2)))
                            {
                                //If we decode EFarr fail, we set this as default.
                                select_response->access_condition1 = 0x0444;//INCREASE,RFU,UPDATE is ADM, READ is ALW
                                select_response->access_condition2 = 0x44;//ACTIVE/DEACTIVE is ADM
                            }
                        }
                    }

                    //update the cur select file For 3G USIM maybe SELECT EFarr...
                    SIMFS_SetCurFileName(cur_sim_task, file_name);
                    SIMFS_SetCurDirectoryFile(cur_sim_task, file_name);


                    SCI_MEMCPY(&s_sim_cur_file_response[cur_sim_task], select_response, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
                    s_select_sw_and_rsp[cur_sim_task] = s_sim_rsp_info[cur_sim_task];//for CRSM
                }
                else
                {
                    SCI_FREE_SIGNAL(instr_response);
                    ClearInstrList(cur_sim_task);//clear the instruction list for next use
                    return SIM_SERVICE_SELECT_DATA_ERROR;
                }
            }
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;

#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIM_SERVICE_INVALIDATE_FILE_NAME;
#endif
}
#endif

/**********************************************************************
//    Description:
//      the implementation of select a SIM/USIM file by the specified file path
//    and get the select data
//    of the destionation file by using the SIM instructions
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_SelectFileByPath(   //return value:the result of the operation
    uint32 cur_sim_task,
    uint16 file_id,    //the file to be selected
    uint8 route_len,  //the file level under the MF
    uint16 *file_route, //the file path to access the file
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response    //the select data of the file
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //refer to the protocol, we can't select MF when use the method of "select from MF"
    if(0x3F00 == file_id)
    {
        ser_result = SIMINSTR_SelectFile(cur_sim_task, SIM_FILE_MF, select_response);
        return ser_result;
    }

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        ser_result = SIMINSTR_UsimSelectFileByPath(cur_sim_task, file_id, route_len, file_route, select_response);
    }
    else
#endif
    {
        ser_result = SIMINSTR_SimSelectFileByPath(cur_sim_task, file_id, route_len, file_route, select_response);
    }

    return ser_result;
}

/**********************************************************************
//    Description:
//     Read immediately  a EF file from the SIM card,the assumed condition is that the current file
//    is the file to read,do not need file routing
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note: the current file is the file to be readed
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_ImiReadEF(   //return value:the result of the read operation
    uint32 cur_sim_task,
    uint16 file_id,  //the file to be read
    uint8 path_len,//IN :
    uint8 *path_id,//IN:
    uint16  offset, //the offset that start to read in transprent file
    uint8   record_num, //the record num to read in recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of recordy file
    uint16 len_want_to_read,  //the length to be read
    uint16 *data_len,   //out:the data len that readed from the file
    uint8  *data        //out:the data readef from the file
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E    ser_result;
    SIM_SIG_RESPONSE_IND_T *instr_response;
    SIMINSTR_RESULT_TYPE_E instr_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_EF_STRUCTURE_E file_struct;
    uint8 temp_len_want_to_read = (uint8)len_want_to_read;
    uint8 i, temp_path_len = 0;
    uint16 temp_path_id[6] = {0};

    if(path_len > 10)
    {
        temp_path_len = 10;
    }
    else
    {
        temp_path_len = path_len;
    }

    for(i = 0; i < temp_path_len; i += 2)
    {
        temp_path_id[i/2] = (uint16)((((uint16)path_id[i]) << 8) | (path_id[i+1]));
    }

    SCI_TRACE_LOW("SIM: read CRSM path_id[0]= 0x%x, path_id[1]= 0x%x,path_id[2]= 0x%x,path_id[3]= 0x%x",
                  temp_path_id[0], temp_path_id[1], temp_path_id[2], temp_path_id[3]);
    ser_result = SIMINSTR_SelectFileByPath(cur_sim_task, file_id, (temp_path_len / 2), temp_path_id, &select_data);

    if(SIM_SERVICE_OK !=  ser_result)
    {
        return ser_result;
    }

    if(0 == (select_data.file_status & SIM_FILE_INVALIDATE_MARK)) //this EF is invalidated
    {
#ifdef GSM_DPHONE_VER2_SUPPORT
        if (((0x6f07  == file_id) || (0x6f7E  == file_id)) && (SIM_INITIALIZE_STATE == g_sim_card_state[cur_sim_task]))
        {
    		if(0x6f07  == file_id)
    		{
		        ser_result = SIMSER_RehabilitateEF(cur_sim_task, SIM_FILE_EF_IMSI);
    		}
    		else
    		{
		        ser_result = SIMSER_RehabilitateEF(cur_sim_task, SIM_FILE_EF_LOCI);
    		}
    		
            if(SIM_SERVICE_OK != ser_result)
            {
                if(IsFatalError(ser_result))
                {
                    return (SIM_INI_ERROR_CARD_OUT);
                }
                else
                {
    		        if (0 == (select_data.file_status & SIM_FILE_INVALIDATE_RU_MARK))//this EF not readable or updatable when invalidated
    		        {
    		            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw1 = 0x98;
    		            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw2 = 0x10;
    		            s_select_sw_and_rsp[cur_sim_task].sw_rsp.data_len = 0;
    		            s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];

    		            return SIM_SERVICE_CONTRADICT_VALIDATION;
    		        }
                }
            }
        }
    	 else	
#endif
    	{
	        if (0 == (select_data.file_status & SIM_FILE_INVALIDATE_RU_MARK))//this EF not readable or updatable when invalidated
	        {
	            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw1 = 0x98;
	            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw2 = 0x10;
	            s_select_sw_and_rsp[cur_sim_task].sw_rsp.data_len = 0;
	            s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];

	            return SIM_SERVICE_CONTRADICT_VALIDATION;
	        }
    	}
    }

    if(SIM_EF_S_TRANSPARENT == select_data.filecharacter_efstructure)
    {
        file_struct = SIM_EF_S_TRANSPARENT;

        if((0 == len_want_to_read) || (len_want_to_read > select_data.freespace_filesize))
        {
            temp_len_want_to_read = select_data.freespace_filesize;
        }
    }
    else
    {
        file_struct = SIM_EF_S_LINEAR_FIXED;
        temp_len_want_to_read = select_data.dfnumber_recordlen;//get the record len
    }

    s_sim_instruction_list[cur_sim_task].instr_num = 0;
    s_sim_instruction_list[cur_sim_task].cur_instr = 0;

    //Add the read instruction
    //the file want to read is a transparent file
    if(SIM_EF_S_TRANSPARENT == file_struct)
    {
        GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                           SIMINSTR_READ_BINARY);

        //in Read Binary instruction P1 is the high byte of the offset
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = (uint8)(offset >> 8);
        //in Read Binary instruction P2 is the low byte of the offset
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = (uint8)offset;
        //in Read Binary instruction P3 is the length of the sting
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = temp_len_want_to_read;
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = temp_len_want_to_read;

        s_sim_instruction_list[cur_sim_task].instr_num++;
    }
    //the file is a fixed linear or cylic file
    else
    {
        if ((SIM_RECORD_NEXT == mode) || (SIM_RECORD_PREVIOUS == mode) || (SIM_RECORD_ABSOLUTE == mode))
        {
            //'04' = absolute mode/current mode, the record number is given in P1 with P1='00' denoting the current record.
            //if (SIM_RECORD_ABSOLUTE != mode)//In fact all the SIM I tested return 6A82(6D00...... etc)
            {
                if (record_num == 0)
                {
                    return SIM_SERVICE_INSTR_P2_P1_ERROR;
                }
            }
        }
        else
        {
            return SIM_SERVICE_INSTR_P2_P1_ERROR;
        }

        GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                           SIMINSTR_READ_RECORD);
        //in Read Record instruction the P1 is ther record num
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = record_num;
        //in Read Record instruction the P2 is the mode
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = mode;
        //in Read Record instruction the P3 is the length of the record
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = temp_len_want_to_read;
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = temp_len_want_to_read;

        s_sim_instruction_list[cur_sim_task].instr_num++;
    }

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, SIMFS_GetFileName(cur_sim_task, file_id, path_len, path_id), instr_response);

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result) &&
            (SIM_STATUS_OK_RESEND_ADPU != instr_result))
        {
            //analysis the failed reason
            ser_result = SIM_SERVICE_ERROR_UNKNOW;

            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        if(SIM_STATUS_OK_RESEND_ADPU == instr_result)
        {
            //Re-Send the current instruction, with the new P3.
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].cur_instr--;//the current instruction should redo
        }

        //the last insturction return value is OK and have reponse data need to
        //be get by the GET RESPONSE instruction,so add the GET RESPONSE
        //instruction to the instruction list and set the P3 of GET RESPONSE instruction
        if((SIM_STATUS_OK_RESPONSE_DATA == instr_result) &&
            (s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr))
        {
            GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                               SIMINSTR_GET_RESPONSE);
            s_sim_instruction_list[cur_sim_task].instr_num++;

            //set the P3 of GET RESPONSE instruction
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.rx_expected_len = instr_response->sw2;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num - 1].apdu.instr_code[TB_SIM_APDU_P3] = instr_response->sw2;
        }

        //if the instruction is the last one of the instruction list
        //get the select structure data from the response data
        if(s_sim_instruction_list[cur_sim_task].instr_num - 1 == s_sim_instruction_list[cur_sim_task].cur_instr)
        {
            *data_len = instr_response->data_len;
            SCI_ASSERT(*data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified:Check_value */
            SCI_MEMCPY(data, instr_response->data, *data_len);
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;
#else
    //the code that do the operation with files in hard disk under
    //windows test
    return SIM_SERVICE_INVALIDATE_FILE_NAME;
#endif
}

/**********************************************************************
//    Description:
//     Update immediately  a EF file from the SIM card,the assumed condition is that the current file
//    is the file to read,do not need file routing
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note: the current file is the file to be readed
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_ImiUpdateEF(   //return value:the result of the read operation
    uint32 cur_sim_task,
    uint16 file_id,  //the file to be read
    uint8 path_len,//IN:
    uint8 *path_id,//IN:
    uint16  offset, //the offset to update in transparent file
    uint8       record_num, //the record num to update of recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of recordy file
    uint16       data_len,   //the update data len
    uint8  *data    //in:the data to update file
)
{
#ifndef SIM_TEST_IN_WIN32
    //the code that operation with really sim but not the test
    //file in hard disk under windows test
    SIM_SERVICE_RESULT_E    ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SIG_RESPONSE_IND_T *instr_response;
    SIMINSTR_RESULT_TYPE_E instr_result;
    uint16 copy_data_len = (uint8)data_len;
    uint16 update_binary_len = 0;
    BOOLEAN is_large_binary_EF = FALSE;
    uint8 large_binary_update_times = 0;
    SIM_EF_STRUCTURE_E file_struct;
    uint16 temp_data_len = data_len;
    uint8 i, temp_path_len = 0;
    uint16 temp_path_id[6] = {0};

    //procedure for data_len=0
    if(0 == data_len)
    {
        SCI_TRACE_LOW("SIM::SIMINSTR_UpdateEF,ERROR");
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    if(path_len > 10)
    {
        temp_path_len = 10;
    }
    else
    {
        temp_path_len = path_len;
    }

    for(i = 0; i < temp_path_len; i += 2)
    {
        temp_path_id[i/2] = (uint16)((((uint16)(path_id[i])) << 8) | (path_id[i+1]));
    }

    ser_result = SIMINSTR_SelectFileByPath(cur_sim_task, file_id, (temp_path_len / 2), temp_path_id, &select_data);

    if(SIM_SERVICE_OK !=  ser_result)
    {
        return ser_result;
    }

    if(0 == (select_data.file_status & SIM_FILE_INVALIDATE_MARK)) //this EF is invalidated
    {
        if(0 == (select_data.file_status & SIM_FILE_INVALIDATE_RU_MARK)) //this EF not readable or updatable when invalidated
        {
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw1 = 0x98;
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.sw2 = 0x10;
            s_select_sw_and_rsp[cur_sim_task].sw_rsp.data_len = 0;
            s_sim_rsp_info[cur_sim_task] = s_select_sw_and_rsp[cur_sim_task];

            return SIM_SERVICE_CONTRADICT_VALIDATION;
        }
    }

    if(SIM_EF_S_TRANSPARENT == select_data.filecharacter_efstructure)
    {
        file_struct = SIM_EF_S_TRANSPARENT;

        if(data_len > select_data.freespace_filesize)
        {
            temp_data_len = select_data.freespace_filesize;
        }
    }
    else
    {
        file_struct = SIM_EF_S_LINEAR_FIXED;

        if(data_len > select_data.dfnumber_recordlen)
        {
            temp_data_len = select_data.dfnumber_recordlen;
        }
    }

    //Add the read instruction
    //the file want to read is a transparent file
    if(SIM_EF_S_TRANSPARENT == file_struct)
    {
        GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                           SIMINSTR_UPDATE_BINARY);

        update_binary_len = MIN(255, temp_data_len);

        if(update_binary_len > 255) //some transparent EF size is more than 255(EFplmnwact 0x6F60).
        {
            is_large_binary_EF = TRUE;

            if(0 != offset) //upper layer give the offset
            {
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = (uint8)(offset >> 8);
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = (uint8)offset;

                //we'd make sure "offset + to_be_update should not exceed the filesize"
                if(offset < 255)
                {
                    copy_data_len = 255 - offset;
                }
                else//offset > 255
                {
                    if(offset < 510)
                    {
                        copy_data_len = 510 - offset;
                    }
                    else
                    {
                        //Too long... ...
                        return SIM_SERVICE_ERROR_UNKNOW;
                    }
                }

                copy_data_len = MIN(copy_data_len, temp_data_len);
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = (uint8)copy_data_len;
            }
            else
            {
                //upper layer didn't give the offset, and the update_binary_len > 256
                //so we should update this EF for servral times (2 times we support)
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = 0;
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0;
                copy_data_len = 255;
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = 0xFF;//firstly,update 255bytes
            }
        }
        else
        {
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = 0;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0;
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = update_binary_len;
            copy_data_len = update_binary_len;
        }

        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, copy_data_len);
        SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, data, copy_data_len);

        s_sim_instruction_list[cur_sim_task].instr_num++;
    }
    //the file is a fixed linear or cylic file
    else
    {
        GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                           SIMINSTR_UPDATE_RECORD);
        //in Read Record instruction the P2 is the mode
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = mode;

        //when the record mode is PREVIOUS or NEXT,the P1 must be set to '00'
        if(SIM_RECORD_ABSOLUTE == mode)
        {
            //in Read Record instruction the P1 is ther record num
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = record_num;
        }
        else
        {
            //in Read Record instruction the P1 is ther record num
            s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = 0;
        }

        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = (uint8)temp_data_len;
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, temp_data_len);
        SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, data, temp_data_len);

        s_sim_instruction_list[cur_sim_task].instr_num++;
    }

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        // for CRSM
        // save the response data to the global variable
        SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, SIMFS_GetFileName(cur_sim_task, file_id, path_len, path_id), instr_response);

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
        {
            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
            instr_response->sw2 = 0;
        }
#endif

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result))
        {
            //analysis the failed reason
            ser_result = SIM_SERVICE_ERROR_UNKNOW;
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        if(is_large_binary_EF)
        {
            if(0 == large_binary_update_times) //we only update 2 times, which means 512 bytes at most.
            {
                large_binary_update_times++;//do not enter again
                update_binary_len -= copy_data_len;//we've update first 255bytes successfully, count the remained data.

                if(update_binary_len > 0) //there has remained data
                {
                    ClearInstrDataBuf(cur_sim_task);//to save some code size

                    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                                       SIMINSTR_UPDATE_BINARY);

                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P1] = 0;
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0xFF;

                    if(update_binary_len > 255) //the remained data is still more than 255 bytes
                    {
                        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = 0xFF;
                        copy_data_len = 255;
                    }
                    else
                    {
                        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = update_binary_len;
                        copy_data_len = update_binary_len;
                    }
                    
                    if (data_len > 255)//double check CID:16045
                    {
                        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, copy_data_len);
                        SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data, (data + 255), copy_data_len);
                    }
                    s_sim_instruction_list[cur_sim_task].instr_num++;
                }
            }
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;
#else
    SIM_FILE_NAME_E file_name = SIM_FILE_TOTAL_NUM;
    //the code that do the operation with files in hard disk under
    //windows test
    return SIMTIW_UpdateFile(cur_sim_task, file_name, offset, record_num, mode,
                             data_len, data);
#endif
}

#ifndef WIN32
/**********************************************************************
//    Description:
//      Get the SIM voltage infomation (Get the DF_GSM file select data,and decode it)
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_GetSimVolt(uint8 card_id, SIM_SUPPLY_VOLT_E *sim_volt)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;

    return ser_result;
}
#endif

#ifdef SIM_TEST_IN_WIN32
/**********the following is for static button sim card setting---start**********/
SIM_SERVICE_RESULT_E SIMINSTR_UpdateEF_static( //return value:the file update operation result
    SIM_FILE_NAME_E file_name,  //the ef file to be updated
    uint16  offset, //the offset to update in transparent file
    uint8       record_num, //the record num to update of recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of recordy file
    uint8       data_len,   //the update data len
    uint8  *data,    //in:the data to update file
    int card_num
)
{
    return SIMTIW_UpdateFile_static(file_name, offset, record_num, mode,
                                    data_len, data, card_num);
}

SIM_SERVICE_RESULT_E SIMINSTR_ReadEF_static(   //return value:the result of the read operation
    SIM_FILE_NAME_E file_name,  //the file to be read
    uint16  offset, //the offset that start to read in transprent file
    uint8   record_num, //the record num to read in recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of recordy file
    uint16 *filesize_recordlen, //out:the file len of transparent file and reocrd len of recordy file
    uint16 *data_len,   //out:the data len that readed from the file
    uint8  *data,        //out:the data readef from the file
    int card_num
)
{
    return SIMTIW_ReadFile_static(file_name, offset, record_num, mode,
                                  filesize_recordlen, data_len, data, card_num);
}
/**********the above is for static button sim card setting---end**********/
#endif

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//      Get the USIM PIN/PUK remain times.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_USIMGetCHVRemainTimes(uint32 cur_sim_task, BOOLEAN is_unblock_pin_check, SIM_CHV_INDICATION_E chv_indication, uint8 *remain_times)
{
#ifndef SIM_TEST_IN_WIN32
    SIM_SERVICE_RESULT_E    ser_result;
    SIM_SIG_RESPONSE_IND_T *instr_response;
    SIMINSTR_RESULT_TYPE_E  instr_result;
    SIMINSTR_NAME_E         instr_name;

    if(is_unblock_pin_check)
    {
        instr_name = SIMINSTR_UNBLOCK_CHV;
    }
    else
    {
        instr_name = SIMINSTR_VERIFY_CHV;
    }

    GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]), instr_name);

    //in Verify CHV instruction,the P2 is the CHV indication
    //When the USIM has only one Application, 0x81 means CHV2; When the USIM has two Applications,
    //the 2nd Application's CHV2 use 0x82........
    //102.221 Annex J
    if(SIM_CHV2 == chv_indication)
    {
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = 0x81;
    }
    else
    {
        s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P2] = chv_indication;
    }

    //Get the Remain times only
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[TB_SIM_APDU_P3] = 0;
    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_type = SIMHAL_SIMINSTR_NONE_DATA;

    s_sim_instruction_list[cur_sim_task].instr_num++;

    for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
    {
        //send a instruction to the sim card
        SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));

        //receive the response of the instruciton from the sim card
        ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

        if(SIM_SERVICE_OK != ser_result)
        {
            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        //analyse the response staus word
        instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);

        //the instruction is process error
        if((SIM_STATUS_OK_NORMAL != instr_result) &&
            (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) &&
            (SIM_STATUS_OK_RESPONSE_DATA != instr_result) &&
            (SIM_STATUS_OK_WITH_UPDATE != instr_result) &&
            (SIM_STATUS_SUCCESS_WITH_PROBLEM != instr_result) &&
            (SIM_STATUS_AUTH_METHOD_BLOKCED != instr_result))
        {
            //analysis the failed reason
            ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name,
                                             instr_result, SIM_ACCESS_ALW);
            //free the response signal
            SCI_FREE_SIGNAL(instr_response);

            //clear the instruction list for next use
            ClearInstrList(cur_sim_task);
            return ser_result;
        }

        *remain_times = 0;// 69 83 or others

        if(0x63 == instr_response->sw1)
        {
            if(0xC0 == ((instr_response->sw2) & 0xF0))
            {
                *remain_times = (instr_response->sw2) & 0xF;
            }
        }

        CheckPCExistence(cur_sim_task, instr_result, instr_response->sw2);

        //free the response signal
        SCI_FREE_SIGNAL(instr_response);
    }

    //clear the instruction list for next use
    ClearInstrList(cur_sim_task);
    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_OK;
#endif
}

/**********************************************************************
//    Description:
//      Decode the File ID field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_FileID(uint8 *data_ptr, uint16 *data_postion, uint16 *file_id)
{
    uint8 offset = 1;
    uint16 temp_fileid = 0;

    if(SIM_FCP_FILE_ID == *data_ptr)
    {
        offset++;//Length len
        temp_fileid = *(data_ptr + offset);
        temp_fileid <<= 8;
        temp_fileid |= *(data_ptr + offset + 1);
        *file_id = temp_fileid;
        *data_postion += 4;//the total len is 4
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************
//    Description:
//      Decode the AID field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_AID(uint8 *data_ptr,  uint16 *data_position)
{
    BOOLEAN result = FALSE;
    uint8 len = 0;
    uint8 offset = 1;

    if(SIM_FCP_DF_NAME == *data_ptr)
    {
        len = *(data_ptr + offset);

        if((len <= 16) && (len > 0))
        {
            *data_position += 1;
            *data_position += len;
            *data_position += 1;//move to the next Tag
        }
    }

    return result;
}

/**********************************************************************
//    Description:
//      Decode the Proprietary Information field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_ProprietaryInfo(uint8 *data_ptr,  uint16 *data_position, uint8 *filecharacter)
{
    BOOLEAN result = FALSE;
    uint8 prop_info_len = 0;
    uint8 offset = 1;

    if(SIM_FCP_PROP_INFO == *data_ptr)
    {
        prop_info_len = *(data_ptr + offset);

        offset++;

        //Now we only deocode the UICC character to get the CLK and VOLT info.
        if(0x80 == *(data_ptr + offset)) //UICC characteristics Tag
        {
            offset++;//length byte
            offset++;
            *filecharacter = 0x80;//init without PIN1
            //CLK bits are identical
            *filecharacter |= (*(data_ptr + offset)) & (BIT_0 | BIT_2 | BIT_3); //Translate into 2G

            if(BIT_5 == ((*(data_ptr + offset)) & BIT_5)) // 3V is ok
            {
                *filecharacter |= BIT_4;//Translate into 2G
            }

            if(BIT_6 == ((*(data_ptr + offset)) & BIT_6)) // 1.8V is ok
            {
                *filecharacter |= BIT_5;//Translate into 2G
            }

            result = TRUE;//We must decode the CLK and VOLT info
        }

        //Translate into the 2G SIM format

        *data_position += 1;
        *data_position += prop_info_len;
        *data_position += 1;//move to the next Tag
        return result;
    }
    else
    {
        return result;
    }
}

/**********************************************************************
//    Description:
//      Decode the Life Cycle Status field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_LifeCycleStatus(uint8 *data_ptr,  uint16 *data_position, uint8 *life_cycle_status)
{
    uint8 offset = 0;

    if(SIM_FCP_LIFE_CYCLE_STATUS == *data_ptr)
    {
        *data_position += 3;

        offset++;//Length

        offset++;//LifeCycleStatusInteger
        *life_cycle_status = *(data_ptr + offset);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/**********************************************************************
//    Description:
//      Decode the Security Attribute in compact mode in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_SA_Compact(uint8 *data_ptr, uint16 *access_condition1, uint8 *access_condition2)
{
    uint8 read_condition, update_condition, am_byte = 0;
    uint8 sc_byte_indeix = 3;/*Security Tag, Length, AM, SC0, SC1...*/

    *access_condition1 = 0x0444;//init (INCREASE,UPDATE is ADM, READ is ALW)
    *access_condition2 = 0x44;//init with ADM (REHABILITATE, INVALIDATE)

    // 3G source code do not check the access condition of INCREASE.
    //according to ISO7816-4, compact didnot describe the INCREASE.
    if(SIM_FCP_SEC_ATTR_COMP == *data_ptr)
    {
        am_byte = *(data_ptr + 2);

        //AM byte bit7 --- bit0:
        //bit7,DELETE,TERMINATE,ACTIVE,DEACTIVE,WRITE,UPDATE,READ(SEEK).

        /* only the security bytes associated with the READ(SEEK) and UPDATE,
        * operations needs to be checked. Remaining sc bytes can be skipped.
        * Assuming that only user authentication will be needed.*/
        if(am_byte & SIM_AM_BYTE_MASK_DELETE)
        {
            ++sc_byte_indeix;
        }

        if(am_byte & SIM_AM_BYTE_MASK_TERMINATE)
        {
            ++sc_byte_indeix;
        }

        if(am_byte & SIM_AM_BYTE_MASK_ACTIVATE)
        {
            ++sc_byte_indeix;
        }

        if(am_byte & SIM_AM_BYTE_MASK_DEACTIVATE)
        {
            ++sc_byte_indeix;
        }

        if(am_byte & SIM_AM_BYTE_MASK_WRITE)
        {
            ++sc_byte_indeix;
        }

        if(am_byte & SIM_AM_BYTE_MASK_UPDATE)
        {
            update_condition = *(data_ptr + sc_byte_indeix);
            (*access_condition1) &= ~(BIT_11 | BIT_10 | BIT_9 | BIT_8); //clear it befroe re-set.

            if(SIM_SC_ALW == update_condition)
            {
                *access_condition1 &= ~(BIT_11 | BIT_10 | BIT_9 | BIT_8); //ALW
            }
            else if(SIM_SC_USER_AUTH == update_condition) //USER AUTH is CHV1 ??????????????
            {
                *access_condition1 |= BIT_8;//CHV1 "1"
            }
            else
            {
                *access_condition1 |= BIT_10;//others as ADM "4"
            }

            ++sc_byte_indeix;
        }
        else
        {
            //If AM byte has no UPDATE MASK, it means NEV.
            *access_condition1 |= BIT_10;//as ADM "4"
        }

        if(am_byte & SIM_AM_BYTE_MASK_READ)
        {
            read_condition = *(data_ptr + sc_byte_indeix);
            (*access_condition1) &= ~(BIT_15 | BIT_14 | BIT_13 | BIT_12); //clear it befroe re-set.

            if(SIM_SC_ALW == read_condition)
            {
                *access_condition1 &= ~(BIT_15 | BIT_14 | BIT_13 | BIT_12); //ALW "0"
            }
            else if(SIM_SC_USER_AUTH == read_condition) //USER AUTH is CHV1 ???????????
            {
                *access_condition1 |= BIT_12;//CHV1 "1"
            }
            else
            {
                *access_condition1 |= BIT_14;//others as ADM "4"
            }

            read_condition = *(data_ptr + sc_byte_indeix);
        }
    }

    return TRUE;
}

/**********************************************************************
//    Description:
//      Decode the Security Attribute in expanded mode in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_SA_Expanded(uint8 *data_ptr, uint16 *access_condition1, uint8 *access_condition2)
{
    uint8 exp_data_index = 2;
    uint8 exp_data_total_len, am_do_len, am_do_val, sc_do_total_len, sc_do_index = 0;
    SIM_SEC_TAG_ENUM am_do_tag, tmp_sc_do_tag;
    SIM_USAGE_QUAL_ENUM usage_qual_value = 0;
    SIM_CHV_PIN_ENUM key_value = SIM_PIN1_APPLN1;
    SIM_OPS_PIN_ENUM tmp_ops = SIM_INVALID_OP;
    uint8 op_allowed[3] = {0xFF, 0xFF, 0xFF};
    uint8 temp_op_allowed = 0;
    BOOLEAN ret_value = FALSE;

    *access_condition1 = 0x0444;//init (INCREASE,UPDATE is ADM, READ is ALW)
    *access_condition2 = 0x44;//init with ADM (REHABILITATE, INVALIDATE)

    /*
    SCI_TRACE_LOW("SIM Expanded 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",\
        *data_ptr,*(data_ptr + 1),*(data_ptr + 2),*(data_ptr + 3),*(data_ptr + 4),*(data_ptr + 5),*(data_ptr + 6),\
        *(data_ptr + 7),*(data_ptr + 8),*(data_ptr + 9),*(data_ptr + 10),*(data_ptr + 11),*(data_ptr + 12),\
        *(data_ptr + 13),*(data_ptr + 14),*(data_ptr + 15),*(data_ptr + 16),*(data_ptr + 17),*(data_ptr + 18),\
        *(data_ptr + 19),*(data_ptr + 20),*(data_ptr + 21),*(data_ptr + 22),*(data_ptr + 23));
    */
    if(SIM_FCP_SEC_ATTR_EXP == *data_ptr)
    {
        exp_data_total_len = *(data_ptr + 1);

        while((exp_data_index - 2) < exp_data_total_len)
        {
            //Access mode data object has 3 case according to ISO7816-4.
            // 1, 80 (AM byte)
            // 2, 81 to 8F (Command Header Description, we need INCREASE only)
            // 3, 9C
            if(((*(data_ptr + exp_data_index)) != SIM_SEC_AM_DO_AM_BYTE_TAG)
                && ((*(data_ptr + exp_data_index)) != SIM_SEC_AM_DO_INC_TAG))
            {
                break;
            }

            am_do_tag = (SIM_SEC_TAG_ENUM)(*(data_ptr + exp_data_index));
            exp_data_index++;
            am_do_len = *(data_ptr + exp_data_index);
            exp_data_index++;
            //maybe "am_do_val" is more than 1 byte? This is according to 3G source code.
            am_do_val = *(data_ptr + exp_data_index);

            /* Point to the SC_DO Tag */
            exp_data_index += am_do_len;

            //SCI_TRACE_LOW("SIM SA_Expanded1 0x%x 0x%x 0x%x 0x%x",am_do_tag,am_do_len,am_do_val,(*(data_ptr + exp_data_index)));
            if((*(data_ptr + exp_data_index)) == SIM_SEC_SC_DO_ALW_TAG)
            {
                tmp_ops = SIM_ALW_OP;
                exp_data_index += 2;
            }
            else if((*(data_ptr + exp_data_index)) == SIM_SEC_SC_DO_NEV_TAG)
            {
                tmp_ops = SIM_NEV;
                exp_data_index += 2;
            }
            else if(((*(data_ptr + exp_data_index)) == SIM_SEC_SC_DO_USR_VER_TAG)
                    || ((*(data_ptr + exp_data_index)) == SIM_SEC_SC_DO_OR_TAG)
                    || ((*(data_ptr + exp_data_index)) == SIM_SEC_SC_DO_AND_TAG))
            {
                tmp_sc_do_tag = (SIM_SEC_TAG_ENUM)(*(data_ptr + exp_data_index));
                exp_data_index++;
                sc_do_total_len = (*(data_ptr + exp_data_index));

                if(0xFF == sc_do_total_len)
                {
                    break;
                }

                sc_do_index = 0;
                exp_data_index++;

                while(sc_do_index < sc_do_total_len)
                {
                    //SCI_TRACE_LOW("SIM SA_Expanded2 0x%x 0x%x 0x%x",tmp_sc_do_tag,sc_do_total_len,(*(data_ptr + exp_data_index)));
                    if((tmp_sc_do_tag == SIM_SEC_SC_DO_OR_TAG)
                        || (tmp_sc_do_tag == SIM_SEC_SC_DO_AND_TAG))
                    {
                        /* Skip A4 tag and length index */
                        exp_data_index += 2;
                        sc_do_index += 2;
                    }

                    usage_qual_value = SIM_USAGE_QUAL_NO_VERIF;

                    if((*(data_ptr + exp_data_index)) == (uint8)SIM_SEC_USAGE_QUAL_TAG)
                    {
                        exp_data_index += 2;
                        sc_do_index += 2;
                        usage_qual_value = (SIM_USAGE_QUAL_ENUM)(*(data_ptr + exp_data_index));
                        exp_data_index++;
                        sc_do_index++;

                        /* Now the next byte will be pointing to the
                        * SIM_SEC_KEY_REF_TAG*/
                        exp_data_index += 2;
                        sc_do_index += 2;
                        key_value = (SIM_CHV_PIN_ENUM)(*(data_ptr + exp_data_index));
                        exp_data_index++;
                        sc_do_index++;
                    }
                    else if((*(data_ptr + exp_data_index)) == (uint8)SIM_SEC_KEY_REF_TAG) /* it is KEY_REF_TAG ie 83 */
                    {
                        exp_data_index += 2;
                        sc_do_index += 2;
                        key_value = (SIM_CHV_PIN_ENUM)(*(data_ptr + exp_data_index));
                        exp_data_index++;
                        sc_do_index++;

                        /* Now the next byte will be pointing to the
                        * SIM_SEC_USAGE_QUAL_TAG*/
                        if(SIM_SEC_USAGE_QUAL_TAG == (SIM_USAGE_QUAL_ENUM)(*(data_ptr + exp_data_index)))
                        {
                            exp_data_index += 2;
                            sc_do_index += 2;
                            usage_qual_value = (SIM_USAGE_QUAL_ENUM)(*(data_ptr + exp_data_index));
                            exp_data_index++;
                            sc_do_index++;
                        }
                    }
                    else
                    {
                        exp_data_index += 6;
                        sc_do_index += 6;
                        continue;
                    }

                    switch(tmp_sc_do_tag)
                    {
                        case SIM_SEC_SC_DO_USR_VER_TAG :
                            {
                                if((key_value >= SIM_PIN1_APPLN1)
                                    && (key_value <= SIM_PIN1_APPLN8))
                                {
                                    tmp_ops = SIM_OPS_PIN1;
                                }
                                else if((key_value >= SIM_PIN2_APPLN1)
                                        && (key_value <= SIM_PIN2_APPLN8))
                                {
                                    tmp_ops = SIM_OPS_PIN2;
                                }
                                else if(key_value == SIM_PIN_UNIV)
                                {
                                    tmp_ops = SIM_OPS_UNIV_PIN;
                                }
                                else
                                {
                                    tmp_ops = SIM_OPS_ADM;
                                }

                                break;
                            }

                        case SIM_SEC_SC_DO_OR_TAG :
                            {
                                if((tmp_ops == SIM_INVALID_OP)
                                    || ((tmp_ops == SIM_NEV)
                                        && (((key_value >= SIM_PIN1_APPLN1)
                                             && (key_value <= SIM_PIN1_APPLN8))
                                            || ((key_value >= SIM_PIN2_APPLN1)
                                                && (key_value <= SIM_PIN2_APPLN8))
                                            || (key_value == SIM_PIN_UNIV))))
                                {
                                    if((key_value >= SIM_PIN1_APPLN1)
                                        && (key_value <= SIM_PIN1_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN1;
                                    }
                                    else if((key_value >= SIM_PIN2_APPLN1)
                                            && (key_value <= SIM_PIN2_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN2;
                                    }
                                    else if(key_value == SIM_PIN_UNIV)
                                    {
                                        tmp_ops = SIM_OPS_UNIV_PIN;
                                    }
                                }
                                else if(tmp_ops == SIM_OPS_PIN1)
                                {
                                    if((key_value >= SIM_PIN2_APPLN1)
                                        && (key_value <= SIM_PIN2_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN1_OR_PIN2;
                                    }
                                    else if(key_value == SIM_PIN_UNIV)
                                    {
                                        tmp_ops = SIM_OPS_PIN1_OR_UNIV_PIN;
                                    }
                                }
                                else if(tmp_ops == SIM_OPS_PIN2)
                                {
                                    if((key_value >= SIM_PIN1_APPLN1)
                                        && (key_value <= SIM_PIN1_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN1_OR_PIN2;
                                    }
                                    else if(key_value == SIM_PIN_UNIV)
                                    {
                                        tmp_ops = SIM_OPS_PIN2_OR_UNIV_PIN;
                                    }
                                }
                                else if(tmp_ops == SIM_OPS_UNIV_PIN)
                                {
                                    if((key_value >= SIM_PIN1_APPLN1)
                                        && (key_value <= SIM_PIN1_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN1_OR_UNIV_PIN;
                                    }
                                    else if((key_value >= SIM_PIN2_APPLN1)
                                            && (key_value <= SIM_PIN2_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN2_OR_UNIV_PIN;
                                    }
                                }

                                break;
                            }

                        case SIM_SEC_SC_DO_AND_TAG :
                            {
                                if((tmp_ops == SIM_INVALID_OP)
                                    && (((key_value >= SIM_PIN1_APPLN1)
                                         && (key_value <= SIM_PIN1_APPLN8))
                                        || ((key_value >= SIM_PIN2_APPLN1)
                                            && (key_value <= SIM_PIN2_APPLN8))
                                        || (key_value == SIM_PIN_UNIV)))
                                {
                                    if((key_value >= SIM_PIN1_APPLN1)
                                        && (key_value <= SIM_PIN1_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN1;
                                    }
                                    else if((key_value >= SIM_PIN2_APPLN1)
                                            && (key_value <= SIM_PIN2_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN2;
                                    }
                                    else if(key_value == SIM_PIN_UNIV)
                                    {
                                        tmp_ops = SIM_OPS_UNIV_PIN;
                                    }
                                }
                                else if(tmp_ops == SIM_OPS_PIN1)
                                {
                                    if((key_value >= SIM_PIN2_APPLN1)
                                        && (key_value <= SIM_PIN2_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN1_AND_PIN2;
                                    }
                                    else if(key_value == SIM_PIN_UNIV)
                                    {
                                        tmp_ops = SIM_OPS_PIN1_AND_UNIV_PIN;
                                    }
                                    else
                                    {
                                        tmp_ops = SIM_NEV;
                                    }
                                }
                                else if(tmp_ops == SIM_OPS_PIN2)
                                {
                                    if((key_value >= SIM_PIN1_APPLN1)
                                        && (key_value <= SIM_PIN1_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN1_AND_PIN2;
                                    }
                                    else if(key_value == SIM_PIN_UNIV)
                                    {
                                        tmp_ops = SIM_OPS_PIN2_AND_UNIV_PIN;
                                    }
                                    else
                                    {
                                        tmp_ops = SIM_NEV;
                                    }
                                }
                                else if(tmp_ops == SIM_OPS_UNIV_PIN)
                                {
                                    if((key_value >= SIM_PIN1_APPLN1)
                                        && (key_value <= SIM_PIN1_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN1_AND_UNIV_PIN;
                                    }
                                    else if((key_value >= SIM_PIN2_APPLN1)
                                            && (key_value <= SIM_PIN2_APPLN8))
                                    {
                                        tmp_ops = SIM_OPS_PIN2_AND_UNIV_PIN;
                                    }
                                    else
                                    {
                                        tmp_ops = SIM_NEV;
                                    }
                                }

                                break;
                            }

                        default:
                            break;
                    }

                    if(usage_qual_value != SIM_USAGE_QUAL_PIN_VERIF) //In fact, no use.
                    {
                        //SCI_TRACE_LOW("SIM:USAGE QUALIFIER VALUE OTHER THAN 8");
                    }
                }
            }

            if(tmp_ops == SIM_INVALID_OP)
            {
                tmp_ops = SIM_NEV;
            }

            if((am_do_tag == SIM_SEC_AM_DO_INC_TAG) && (am_do_val == SIM_AM_BYTE_MASK_INCREASE))
            {
                op_allowed[SIM_INC_POS] = (uint8)tmp_ops;
            }
            else if(am_do_tag == SIM_SEC_AM_DO_AM_BYTE_TAG)
            {
                if(am_do_val & SIM_AM_BYTE_MASK_READ)
                {
                    /*Clear all the read value bits and then set it */
                    op_allowed[SIM_READ_UPDATE_POS] &= SIM_UPDATE_MASK;
                    op_allowed[SIM_READ_UPDATE_POS] = (op_allowed[SIM_READ_UPDATE_POS] | tmp_ops);
                }

                if(am_do_val & SIM_AM_BYTE_MASK_UPDATE)
                {
                    /*Clear all the update value bits and then set it */
                    op_allowed[SIM_READ_UPDATE_POS] &= SIM_READ_MASK;
                    op_allowed[SIM_READ_UPDATE_POS] = (op_allowed[SIM_READ_UPDATE_POS] | (tmp_ops << 4));
                }

                if(am_do_val & SIM_AM_BYTE_MASK_ACTIVATE)
                {
                    /*Clear all the activate value bits and then set it */
                    op_allowed[SIM_ACT_DEACT_POS] &= SIM_DEACTIVATE_MASK;
                    op_allowed[SIM_ACT_DEACT_POS] = (op_allowed[SIM_ACT_DEACT_POS] | tmp_ops);
                }

                if(am_do_val & SIM_AM_BYTE_MASK_DEACTIVATE)
                {
                    /*Clear all the activate value bits and then set it */
                    op_allowed[SIM_ACT_DEACT_POS] &= SIM_ACTIVATE_MASK;
                    op_allowed[SIM_ACT_DEACT_POS] = (op_allowed[SIM_ACT_DEACT_POS] | (tmp_ops << 4));
                }
            }
        }

        ret_value = TRUE;
    }
    else
    {
        ret_value = FALSE;
    }

    //SCI_TRACE_LOW("SIM Expanded decode 0x%x 0x%x 0x%x",op_allowed[SIM_READ_UPDATE_POS],op_allowed[SIM_ACT_DEACT_POS],op_allowed[SIM_INC_POS]);

    //Translate the 3G EF's attribute into 2G's format
    //UPDATE
    (*access_condition1) &= ~(BIT_11 | BIT_10 | BIT_9 | BIT_8); //clear before re-set it.
    temp_op_allowed = op_allowed[SIM_READ_UPDATE_POS] & 0xF0;
    temp_op_allowed >>= 4;

    if(SIM_ALW_OP == temp_op_allowed)
    {
        *access_condition1 &= ~(BIT_11 | BIT_10 | BIT_9 | BIT_8);
    }
    else if((SIM_OPS_PIN1 == temp_op_allowed)
            || (SIM_OPS_PIN1_OR_PIN2 == temp_op_allowed)
            || (SIM_OPS_PIN1_OR_UNIV_PIN == temp_op_allowed))
    {
        *access_condition1 |= BIT_8;//"1"
    }
    else if((SIM_OPS_PIN2 == temp_op_allowed)
            || (SIM_OPS_PIN2_OR_UNIV_PIN == temp_op_allowed))
    {
        *access_condition1 |= BIT_9;//"2"
    }
    else
    {
        *access_condition1 |= BIT_10;//others as ADM "4"
    }

    //READ(SEEK)
    (*access_condition1) &= ~(BIT_15 | BIT_14 | BIT_13 | BIT_12); //clear before re-set it.
    temp_op_allowed = 0;
    temp_op_allowed = (op_allowed[SIM_READ_UPDATE_POS] & 0xF);

    if(SIM_ALW_OP == temp_op_allowed)
    {
        *access_condition1 &= ~(BIT_15 | BIT_14 | BIT_13 | BIT_12);
    }
    else if((SIM_OPS_PIN1 == temp_op_allowed)
            || (SIM_OPS_PIN1_OR_PIN2 == temp_op_allowed)
            || (SIM_OPS_PIN1_OR_UNIV_PIN == temp_op_allowed))
    {
        *access_condition1 |= BIT_12;//"1"
    }
    else if((SIM_OPS_PIN2 == temp_op_allowed)
            || (SIM_OPS_PIN2_OR_UNIV_PIN == temp_op_allowed))
    {
        *access_condition1 |= BIT_13;//"2"
    }
    else
    {
        *access_condition1 |= BIT_14;//others as ADM "4"
    }

    //INCREASE
    (*access_condition1) &= ~(BIT_7 | BIT_6 | BIT_5 | BIT_4); //clear before re-set it.

    if(SIM_ALW_OP == op_allowed[SIM_INC_POS])
    {
        *access_condition1 &= ~(BIT_7 | BIT_6 | BIT_5 | BIT_4);
    }
    else if((SIM_OPS_PIN1 == op_allowed[SIM_INC_POS])
            || (SIM_OPS_PIN1_OR_PIN2 == op_allowed[SIM_INC_POS])
            || (SIM_OPS_PIN1_OR_UNIV_PIN == op_allowed[SIM_INC_POS]))
    {
        *access_condition1 |= BIT_4;//"1"
    }
    else if((SIM_OPS_PIN2 == op_allowed[SIM_INC_POS])
            || (SIM_OPS_PIN2_OR_UNIV_PIN == op_allowed[SIM_INC_POS]))
    {
        *access_condition1 |= BIT_5;//"2"
    }
    else
    {
        *access_condition1 |= BIT_6;//others as ADM "4"
    }

    //INVALIDATE (3G:DEACTIVE)
    (*access_condition2) &= ~(BIT_3 | BIT_2 | BIT_1 | BIT_0); //clear before re-set it.
    temp_op_allowed = 0;
    temp_op_allowed = op_allowed[SIM_ACT_DEACT_POS] & 0xF0;
    temp_op_allowed >>= 4;

    if(SIM_ALW_OP == temp_op_allowed)
    {
        *access_condition2 |= 0;
    }
    else if((SIM_OPS_PIN1 == temp_op_allowed)
            || (SIM_OPS_PIN1_OR_PIN2 == temp_op_allowed)
            || (SIM_OPS_PIN1_OR_UNIV_PIN == temp_op_allowed))
    {
        *access_condition2 |= 0x1;
    }
    else if((SIM_OPS_PIN2 == temp_op_allowed)
            || (SIM_OPS_PIN2_OR_UNIV_PIN == temp_op_allowed))
    {
        *access_condition2 |= 0x2;
    }
    else
    {
        *access_condition2 |= 0x4;//others as ADM
    }

    //REHABILITATE (3G:ACTIVE)
    (*access_condition2) &= ~(BIT_7 | BIT_6 | BIT_5 | BIT_4); //clear before re-set it.
    temp_op_allowed = 0;
    temp_op_allowed = op_allowed[SIM_ACT_DEACT_POS] & 0xF;

    if(SIM_ALW_OP == temp_op_allowed)
    {
        *access_condition2 |= 0;
    }
    else if((SIM_OPS_PIN1 == temp_op_allowed)
            || (SIM_OPS_PIN1_OR_PIN2 == temp_op_allowed)
            || (SIM_OPS_PIN1_OR_UNIV_PIN == temp_op_allowed))
    {
        *access_condition2 |= 0x10;
    }
    else if((SIM_OPS_PIN2 == temp_op_allowed)
            || (SIM_OPS_PIN2_OR_UNIV_PIN == temp_op_allowed))
    {
        *access_condition2 |= 0x20;
    }
    else
    {
        *access_condition2 |= 0x40;//others as ADM
    }

    return ret_value;
}

/**********************************************************************
//    Description:
//      Decode the Security Attribute in reference expanded mode in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_SA_RefExpanded(uint8 *data_ptr, SIM_FCP_REF_ARR_T *arr_info)
{
    uint8 i, len = 0;
    uint16 temp_fileid = 0;

    //Init
    arr_info->is_ref_exist = FALSE;
    arr_info->file_id = 0;
    arr_info->seid_valid_num = 0;
    arr_info->rec_index_valid_num = 0;

    if(SIM_FCP_SEC_ATTR_REF == *data_ptr)
    {
        //SCI_TRACE_LOW("SIM Reference ARR 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",*(data_ptr+1),*(data_ptr+2),*(data_ptr+3),*(data_ptr+4),*(data_ptr+5),*(data_ptr+6),*(data_ptr+7));
        arr_info->is_ref_exist = TRUE;

        if(3 == *(data_ptr + 1))
        {
            //get the EFarr's file id
            temp_fileid = *(data_ptr + 2);
            temp_fileid <<= 8;
            temp_fileid |= *(data_ptr + 3);
            arr_info->file_id = temp_fileid;
            //get the record index in EFarr
            arr_info->rec_index_valid_num = 1;
            arr_info->rec_index[0] = *(data_ptr + 4);
        }
        else
        {
            len = *(data_ptr + 1);
            len = (len - 2) / 2;

            if(len > 2)
            {
                len = 2;// 3G source code indicate that they support 2 SEID at most
            }

            //get the EFarr's file id
            temp_fileid = *(data_ptr + 2);
            temp_fileid <<= 8;
            temp_fileid |= *(data_ptr + 3);
            arr_info->file_id = temp_fileid;

            for(i = 0; i < len; i++)
            {
                arr_info->seid[i] = *(data_ptr + ((i + 1) * 4));

                if((0 == arr_info->seid[i]) || (1 == arr_info->seid[i])) //SEID's value must be 0 or 1
                {
                    arr_info->rec_index[i] = *(data_ptr + ((i + 1) * 5));
                    arr_info->seid_valid_num++;
                    arr_info->rec_index_valid_num++;
                }
                else
                {
                    break;//quit the loop
                }
            }
        }
    }

    return TRUE;
}

/**********************************************************************
//    Description:
//      Decode the Security Attribute field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_SecurityAttributes(uint8 *data_ptr,  uint16 *data_position, uint16 *access_condition1, uint8 *access_condition2, SIM_FCP_REF_ARR_T *arr_info)
{
    BOOLEAN ret_value = FALSE;
    uint8 SA_len = 0;
    uint8 offset = 1;

    SA_len = *(data_ptr + offset);

    switch(*data_ptr)
    {
        case 0x8B:
            SIMINSTR_DecodeUSIMFCP_SA_RefExpanded(data_ptr, arr_info);
            ret_value = TRUE;
            break;
        case 0x8C:
            SIMINSTR_DecodeUSIMFCP_SA_Compact(data_ptr, access_condition1, access_condition2);
            ret_value = TRUE;
            break;
        case 0xAB:
            SIMINSTR_DecodeUSIMFCP_SA_Expanded(data_ptr, access_condition1, access_condition2);
            ret_value = TRUE;
            break;
        default:
            ret_value = FALSE;
            break;
    }

    //Move the data_position only when the Security Att valid
    if(ret_value)
    {
        *data_position += 1;
        *data_position += SA_len;
        *data_position += 1;//Move to the next Tag
    }

    return ret_value;
}

/**********************************************************************
//    Description:
//      Decode the PIN status template DO field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_PSDO(uint32 cur_sim_task, uint8 *data_ptr, uint16 *data_position, BOOLEAN is_adf_usim, BOOLEAN *chv1_enabled)
{
    uint8 offset = 0;
    uint8 ps_total_len = 0;
    uint8 ps_do_len = 0;
    uint8 ps_do_byte = 0;
    uint8 ps_do_byte_index = 4;
    uint8 pin_count = 0;
    BOOLEAN usage_qual_flag = FALSE;//In fact, 2G donot use this.
    BOOLEAN pin_enabled = FALSE;
    BOOLEAN pin1_enabled = FALSE;
    BOOLEAN pin2_enabled = FALSE;

    //Init these PIN/PUK status
    *chv1_enabled = FALSE;//Set as FALSE

    if(SIM_FCP_PS_TEMPLATE_DO == *data_ptr)
    {
        ps_total_len = *(data_ptr + 1);

        if(SIM_SEC_PS_DO_TAG != *(data_ptr + 2)) //PS_DO tag
        {
            return FALSE;
        }

        ps_do_len = *(data_ptr + 3);//PS_DO length
        ps_do_byte = *(data_ptr + ps_do_byte_index);//Now PS DO byte index is 4
        offset += (4 + ps_do_len);

        while(offset < ps_total_len)
        {
            if((SIM_SEC_USAGE_QUAL_TAG == *(data_ptr + offset))
                || (SIM_SEC_KEY_REF_TAG == *(data_ptr + offset)))
            {
                if(SIM_SEC_USAGE_QUAL_TAG == *(data_ptr + offset))
                {
                    offset += 2;

                    if(SIM_USAGE_QUAL_PIN_VERIF == *(data_ptr + offset))
                    {
                        usage_qual_flag = TRUE;
                    }
                    else
                    {
                        usage_qual_flag = FALSE;
                    }

                    offset++;
                }

                if(SIM_SEC_KEY_REF_TAG == *(data_ptr + offset))
                {
                    offset += 2;

                    if((ps_do_byte & (1 << (7 - pin_count))) > 0)
                    {
                        pin_enabled = TRUE;
                    }
                    else
                    {
                        pin_enabled = FALSE;
                    }

                    if(SIM_PIN1 == *(data_ptr + offset))
                    {
                        if(pin_enabled)
                        {
                            pin1_enabled = TRUE;

                            if(is_adf_usim)
                            {
                                s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_ENABLED;
                            }
                        }
                        else
                        {
                            pin1_enabled = FALSE;

                            if(is_adf_usim)
                            {
                                s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_DISABLED;
                            }
                        }
                    }
                    else if(SIM_PIN2 == *(data_ptr + offset))
                    {
                        if(pin_enabled)
                        {
                            pin2_enabled = TRUE;
                        }
                        else
                        {
                            pin2_enabled = FALSE;
                        }
                    }
                    else if(SIM_UNIV == *(data_ptr + offset))
                    {
                        if(pin_enabled)
                        {
                            if(is_adf_usim)
                            {
                                s_sim_usim_univ_pin_status[cur_sim_task] = SIM_PIN_ENABLED;
                            }
                        }
                        else
                        {
                            if(is_adf_usim)
                            {
                                s_sim_usim_univ_pin_status[cur_sim_task] = SIM_PIN_DISABLED;
                            }
                        }

                        if(is_adf_usim)
                        {
                            s_sim_usim_use_univ_pin[cur_sim_task] = usage_qual_flag;
                        }
                    }

                    pin_count++;
                    offset++;//To the next loop
                }

                if(pin_count == 8)
                {
                    pin_count = 0;
                    ps_do_byte_index++;
                    ps_do_byte = *(data_ptr + ps_do_byte_index);//update the ps_byte
                }
            }
            else
            {
                break;//or dead loop.
            }
        }

        *data_position += 1;
        *data_position += ps_total_len;
        *data_position += 1;//Move to the next Tag

        //translate 3G PIN status into 2G PIN status
        if(pin1_enabled)
        {
            *chv1_enabled = TRUE;
        }

        if(pin2_enabled)
        {

        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************
//    Description:
//      Decode the File Descriptor field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_FileDescriptor(uint8 *data_ptr,  uint16 *data_position, uint8 *efstructure, uint8 *recordlen)
{
    uint8 len = 0;
    uint16 temp_recordlen = 0;
    uint8 temp_efstructure = 0;

    if(SIM_FCP_FILE_DESC == *data_ptr)
    {
        len = *(data_ptr + 1); //Length

        if((2 == len) || (5 == len)) //the len must be 2 or 5
        {
            temp_efstructure = (*(data_ptr + 2)) & (BIT_0 | BIT_1 | BIT_2);

            //There are only 3 types of EF structure in 2G.
            if(2 == temp_efstructure)
            {
                *efstructure = 1;// 1 indicates linear fixed in 2G
            }
            else if(6 == temp_efstructure)
            {
                *efstructure = 3;// 3 indicates cyclic in 2G
            }
            else
            {
                *efstructure = 0;// 0 indicates transparent in 2G
            }

            if(5 == len)
            {
                temp_recordlen = *(data_ptr + 4);
                temp_recordlen <<= 8;
                temp_recordlen |= *(data_ptr + 5);
                *recordlen = (uint8)temp_recordlen;// 2G will support 255 as max.

                *data_position += 7;
            }
            else
            {
                *recordlen = 0;

                *data_position += 4;
            }

            return TRUE;
        }
    }

    return FALSE;
}

/**********************************************************************
//    Description:
//      Decode the File size field in the FCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP_FileSize(uint8 *data_ptr,  uint16 *data_position, uint16 *file_size)
{
    uint8 len = 0;
    uint16 filesize = 0;

    if(SIM_FCP_FILE_SIZE == *data_ptr)
    {
        len = *(data_ptr + 1);
        //We regard this as uint16 only....
        filesize = *(data_ptr + 2);

        if(len >= 2) //In fact, the length must >= 2 according to the spec102.221 11.1.1.4.1
        {
            filesize <<= 8;
            filesize |= *(data_ptr + 3);
        }

        *file_size = filesize;
        *data_position += (len + 2);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifdef SIM_3G_FORMAT_TRANS
/**********************************************************************
//    Description:
//      Convert 3G response data to 2G format
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMINSTR_3GResponseTo2GFormat(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,
    uint8 *data_ptr,
    uint16 *data_len)
{
    SIM_RESPONSE_DATA_OF_SELECT_T select_response = {0};
    SIM_FCP_REF_ARR_T arr_info = {0};
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_ERROR_UNKNOW;
    uint8 pin_remain_times = 0;
    BOOLEAN result = FALSE;
    uint8 *ptemp = data_ptr;

    if((PNULL == data_ptr) || (PNULL == data_len))
    {
        return result;
    }

    //1. First convert to response data structure
    if(SIMINSTR_DecodeUSIMFCP(cur_sim_task, file_name, data_ptr, *data_len, &arr_info, &select_response))
    {
        //A Guangdong 2G Unicom SIM card's EFkc's len is 0xFFFF. I'm afraid USIM has the same case. wuding
        if((SIM_FILE_EF_USIM_GSM_KC == file_name) && (select_response.freespace_filesize > 9))
        {
            select_response.freespace_filesize = 9;//Length 9 is defined in the spec 11.11.
        }

        //For some DF/EF need specially operation, we should do here.
        // 1, Check the CHV1/2 's remain times.
        // 2, unblock pin status and remain times.
        // Only VERIFY can get the remain times and the info of unblock pin.
        // We do this operation only the first time
        if(SIM_FILE_ADF_USIM == file_name)
        {
            select_response.chv1_status &= 0xF0;//clear before re-set.
            select_response.chv2_status &= 0xF0;//clear before re-set.
            select_response.unblock_chv1_status &= 0xF0;//clear before re-set.
            select_response.unblock_chv2_status &= 0xF0;//clear before re-set.

            //For first time select ADF, we have stored the PIN/UNBLOCKED false-remain times,
            //so afterwards we just get the value from the global variables
            if(!s_sim_gain_chv_remain_times_flag[cur_sim_task])
            {
                //we must clear it
                s_sim_instruction_list[cur_sim_task].instr_num = 0;
                s_sim_instruction_list[cur_sim_task].cur_instr = 0;
                ser_result = SIMINSTR_USIMGetCHVRemainTimes(cur_sim_task, FALSE, SIM_CHV1, &pin_remain_times);

                if(SIM_SERVICE_OK == ser_result)
                {
                    select_response.chv1_status |= (pin_remain_times & 0xF);

                    if(0 == pin_remain_times)
                    {
                        select_response.unblock_chv1_status |= BIT_7;
                    }

                    //SCI_TRACE_LOW("SIM: end PIN1 check 0x%x, %d",select_response->chv1_status,pin_remain_times);
                }

                if(SIM_SERVICE_OK == ser_result)
                {
                    ser_result = SIMINSTR_USIMGetCHVRemainTimes(cur_sim_task, FALSE, SIM_CHV2, &pin_remain_times);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        select_response.chv2_status |= (pin_remain_times & 0xF);

                        if(0 == pin_remain_times)
                        {
                            select_response.unblock_chv2_status |= BIT_7;
                        }

                        //SCI_TRACE_LOW("SIM: end PIN2 check 0x%x, %d",select_response->chv2_status,pin_remain_times);
                    }
                }

                if(SIM_SERVICE_OK == ser_result)
                {
                    ser_result = SIMINSTR_USIMGetCHVRemainTimes(cur_sim_task, TRUE, SIM_CHV1, &pin_remain_times);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        select_response.unblock_chv1_status |= (pin_remain_times & 0xF);
                        //SCI_TRACE_LOW("SIM: end UNBLOCK PIN1 check 0x%x, %d",select_response->unblock_chv1_status,pin_remain_times);
                    }
                }

                if(SIM_SERVICE_OK == ser_result)
                {
                    ser_result = SIMINSTR_USIMGetCHVRemainTimes(cur_sim_task, TRUE, SIM_CHV2, &pin_remain_times);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        select_response.unblock_chv2_status |= (pin_remain_times & 0xF);
                        //SCI_TRACE_LOW("SIM: end UNBLOCK PIN2 check 0x%x, %d",select_response->unblock_chv2_status,pin_remain_times);
                    }
                }
            }
            else
            {
                select_response.chv1_status |= (g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain & 0xF);

                if(0 == g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain)
                {
                    select_response.unblock_chv1_status |= BIT_7;
                }

                select_response.unblock_chv1_status |= (g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.false_remain & 0xF);

                select_response.chv2_status |= (g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain & 0xF);

                if(0 == g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain)
                {
                    select_response.unblock_chv2_status |= BIT_7;
                }

                select_response.unblock_chv2_status |= (g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.false_remain & 0xF);

            }
        }
        else if((SIM_FILE_EF_USIM_ACM == file_name) || (SIM_FILE_EF_USIM_ACMMAX == file_name) || (SIM_FILE_EF_USIM_PUCT == file_name)
                || (SIM_FILE_EF_USIM_VGCSS == file_name) || (SIM_FILE_EF_USIM_VBSS == file_name))
        {
            if(arr_info.is_ref_exist)
            {
                //We want to get the EFacm/acmmax/puct/vgcss/vbss's access condition for UPDATE.
                // Is the access condition is PIN/PIN2 , PIN/ADM?
                // In fact, EFict,EFoct,EFmsucp,EFmsisdn(under ADFusim) are also need. But we donnot support these EFs now.
                if(!SIMINSTR_GetARR(cur_sim_task, file_name, arr_info, &(select_response.access_condition1), &(select_response.access_condition2)))
                {
                    //If we decode EFarr fail, we set this as default.
                    select_response.access_condition1 = 0x0444;//INCREASE,RFU,UPDATE is ADM, READ is ALW
                    select_response.access_condition2 = 0x44;//ACTIVE/DEACTIVE is ADM
                }
            }
        }

        result = TRUE;
    }

    //2.Then convert response structure to data flow
    if(result)
    {
        //clear
        SCI_MEMSET(data_ptr, 0x0, 256);
        *data_len = 0;

        //byte1-2 RFU
        *ptemp++ = 0;
        *ptemp++ = 0;
        //file size or total amount of memory
        *ptemp++ = (uint8)(select_response.freespace_filesize >> 8);
        *ptemp++ = (uint8)(select_response.freespace_filesize);
        //file id
        *ptemp++ = (uint8)(select_response.file_id >> 8);
        *ptemp++ = (uint8)(select_response.file_id);
        //Type of file
        *ptemp++ = select_response.file_type;

        if(SIM_FILE_T_EF == select_response.file_type)
        {
            //byte8 SIM_SELECT_DATA_CYC_INCREASE
            *ptemp++ = select_response.cyclic_full_check;
            //byte9 ~11 Access conditions
            *ptemp++ = (uint8)(select_response.access_condition1 >> 8);
            *ptemp++ = (uint8)(select_response.access_condition1);
            *ptemp++ = (uint8)(select_response.access_condition2);
            //byte12 file status
            *ptemp++ = (uint8)(select_response.file_status);
            //byte13 Length of the following data
            *ptemp++ = (uint8)(select_response.remain_len);
            //byte14 Structure of EF
            *ptemp++ = (uint8)(select_response.filecharacter_efstructure);
            //byte15 Length of a record
            *ptemp = (uint8)(select_response.dfnumber_recordlen);
            *data_len = 15;
        }
        else//DF /MF
        {
            //byte8 ~12 RFU
            *ptemp++ = 0;
            *ptemp++ = 0;
            *ptemp++ = 0;
            *ptemp++ = 0;
            *ptemp++ = 0;
            //byte13 Length of the following data
            *ptemp++ = (uint8)(select_response.remain_len);
            //byte14 File characteristics
            *ptemp++ = (uint8)(select_response.filecharacter_efstructure);
            //byte15 Number of DFs
            *ptemp++ = (uint8)(select_response.dfnumber_recordlen);
            //byte16 Number of EFs
            *ptemp++ = (uint8)(select_response.ef_number);
            //byte17 Number of CHVs, UNBLOCK CHVs and administrative codes
            *ptemp++ = (uint8)(select_response.chv_ad_code_num);
            //byte18 RFU
            *ptemp++ = (uint8)(select_response.sim_rfu2);
            //byte19 CHV1 status
            *ptemp++ = (uint8)(select_response.chv1_status);
            //byte20 UNBLOCK CHV1 status
            *ptemp++ = (uint8)(select_response.unblock_chv1_status);
            //byte21 CHV2 status
            *ptemp++ = (uint8)(select_response.chv2_status);
            //byte22 UNBLOCK CHV2 status
            *ptemp = (uint8)(select_response.unblock_chv2_status);
            *data_len = 22;
        }
    }

    return result;
}
#endif

/**********************************************************************
//    Description:
//      Decode the the FCP after a SELECT instruction.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_DecodeUSIMFCP(uint32 cur_sim_task, SIM_FILE_NAME_E file_name, uint8 *data_ptr, uint16 data_len, SIM_FCP_REF_ARR_T *arr_info, SIM_RESPONSE_DATA_OF_SELECT_T *out_ptr)
{
    BOOLEAN is_adf_usim = FALSE;
    BOOLEAN chv1_enabled = FALSE;
    uint8 dummy, dummy2 = 0;
    uint16 fcp_len = 0;
    uint16 data_ptr_position = 1;
    uint16 fcp_file_id = 0;
    uint16 file_size = 0;
    uint16 access_condition1 = 0;
    uint8 access_condition2 = 0;
    uint8 dfnumber_recordlen = 0;
    uint8 filecharacter_efstructure = 0;
    uint8 chv1_status = 0x80;
    uint8 unblock_chv1_status = 0x80;
    uint8 chv2_status = 0x80;
    uint8 unblock_chv2_status = 0x80;
    uint8 life_cycle_status = 0;
#ifdef MULTI_SIM_SYS_QUAD
    static uint8 strored_file_character[SIM_CARD_NUM] = {(BIT_7 | BIT_4 | BIT_3 | BIT_0), (BIT_7 | BIT_4 | BIT_3 | BIT_0), (BIT_7 | BIT_4 | BIT_3 | BIT_0), (BIT_7 | BIT_4 | BIT_3 | BIT_0)}; //init as no chv1, 3V, clk stop low prefer
#elif defined (MULTI_SIM_SYS_TRI)
    static uint8 strored_file_character[SIM_CARD_NUM] = {(BIT_7 | BIT_4 | BIT_3 | BIT_0), (BIT_7 | BIT_4 | BIT_3 | BIT_0), (BIT_7 | BIT_4 | BIT_3 | BIT_0)}; //init as no chv1, 3V, clk stop low prefer
#else
    static uint8 strored_file_character[SIM_CARD_NUM] = {(BIT_7 | BIT_4 | BIT_3 | BIT_0), (BIT_7 | BIT_4 | BIT_3 | BIT_0)}; //init as no chv1, 3V, clk stop low prefer
#endif

    //In some compile environment, there has a problem in the SCI_TRACE_LOW below.
    //So we check and return.
    if(0 == data_len)
    {
        SCI_TRACE_LOW("SIM fcp len = 0");
        return FALSE;
    }

    //SCI_TRACE_LOW:"SIM fcp 0x%x: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_7801_112_2_17_22_59_31_4520, (uint8 *)"ddddddddddddddddddddddddddddddddddddddddd", data_len, \
                 data_ptr[0], data_ptr[1], data_ptr[2], data_ptr[3], data_ptr[4], data_ptr[5], data_ptr[6], data_ptr[7], data_ptr[8], data_ptr[9], data_ptr[10], data_ptr[11], \
                 data_ptr[12], data_ptr[13], data_ptr[14], data_ptr[15], data_ptr[16], data_ptr[17], data_ptr[18], data_ptr[19], data_ptr[20], data_ptr[21], data_ptr[22], data_ptr[23], data_ptr[24], \
                 data_ptr[25], data_ptr[26], data_ptr[27], data_ptr[28], data_ptr[29], data_ptr[30], data_ptr[31], data_ptr[32], data_ptr[33], data_ptr[34], data_ptr[35], data_ptr[36], data_ptr[37], data_ptr[38], data_ptr[39]);

    out_ptr->sim_rfu1 = 0;//no use
    out_ptr->ef_number = 0;//no use
    out_ptr->chv_ad_code_num = 0;//no use
    out_ptr->sim_rfu2 = 0;//no use
    out_ptr->remain_len = 0;//no use
    out_ptr->access_condition2 = 0;//no use

    if(SIM_FCP_TEMPLATE == *data_ptr) //the 1st byte should be 0x62
    {
        //The len may be 1 byte or 2 bytes. How to get the exact bytes?
        fcp_len = *(data_ptr + data_ptr_position); //Length of FCP template
        data_ptr_position++;

        //Decode the FCP Template
        //Decode the MF/DF
        if(file_name < SIM_FILE_EF_ICCID)
        {
            if(SIM_FILE_ADF_USIM == file_name)
            {
                is_adf_usim = TRUE;
            }

            //File Descriptor
            //We will not decode the File Descriptor when MF/DF
            if(SIMINSTR_DecodeUSIMFCP_FileDescriptor((data_ptr + data_ptr_position), &data_ptr_position, &dummy, &dummy2))
            {

            }
            else
            {
                //SCI_TRACE_LOW:"SIM:SIMINSTR_DecodeUSIMFCP_FileDescriptor,ERROR"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_7833_112_2_17_22_59_31_4521, (uint8 *)"");
                return FALSE;
            }

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP1 %d, 0x%x",data_ptr_position, *(data_ptr+data_ptr_position));

            //File identifier
            if(SIMINSTR_DecodeUSIMFCP_FileID((data_ptr + data_ptr_position), &data_ptr_position, &fcp_file_id))
            {
                //out_ptr->file_id = fcp_file_id;
                if(SIM_FILE_ADF_USIM == file_name)
                {
                    //We find out an India BSNL usim the FCP file id is 0x7FAA.
                    //But we can SELECT it successfully by 0x7FFF. So we use 0x7FFF directly.
                    s_sim_usim_adf_file_id_flag[cur_sim_task] = TRUE;
                }
            }
            else
            {
                //ADF has no this field
                //SCI_ASSERT(0);
                if(SIM_FILE_ADF_USIM == file_name)
                {
                    s_sim_usim_adf_file_id_flag[cur_sim_task] = TRUE;
                }
            }

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP2 %d 0x%x",data_ptr_position,*(data_ptr+data_ptr_position));

            //DF name(ADF).
            SIMINSTR_DecodeUSIMFCP_AID((data_ptr + data_ptr_position), &data_ptr_position);

            //Proprietary information. This is mandatory for MF, but optional for DF/ADF.
            if(SIMINSTR_DecodeUSIMFCP_ProprietaryInfo((data_ptr + data_ptr_position), &data_ptr_position, &filecharacter_efstructure))
            {
                //the voltage and CLK info, stored it.
                strored_file_character[cur_sim_task] = filecharacter_efstructure;
            }
            else
            {
                //If DF/ADF has no ProprietaryInfo, we should set it according to MF.
                filecharacter_efstructure = strored_file_character[cur_sim_task];

                //The MF is madatory for this.
                if(SIM_FILE_MF == file_name) //debug only
                {
                    SCI_ASSERT(0);/*assert verified */
                }
            }

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP3 %d 0x%x",data_ptr_position,*(data_ptr+data_ptr_position));

            //Life Cycle Status Integer
            //We will not decode the life cycle when MF/DF
            if(SIMINSTR_DecodeUSIMFCP_LifeCycleStatus((data_ptr + data_ptr_position), &data_ptr_position, &life_cycle_status))
            {

            }
            else
            {
                //SCI_TRACE_LOW:"SIM:SIMINSTR_DecodeUSIMFCP_LifeCycleStatus,ERROR"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_7892_112_2_17_22_59_31_4522, (uint8 *)"");
                return FALSE;
            }

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP4 %d 0x%x",data_ptr_position,*(data_ptr+data_ptr_position));

            //Security attributes
            if(SIMINSTR_DecodeUSIMFCP_SecurityAttributes((data_ptr + data_ptr_position), &data_ptr_position, &access_condition1, &access_condition2, arr_info))
            {
            }
            else
            {
                //SCI_TRACE_LOW:"SIM:SIMINSTR_DecodeUSIMFCP_SecurityAttributes,ERROR"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_7904_112_2_17_22_59_31_4523, (uint8 *)"");
                return FALSE;
            }

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP5 %d 0x%x",data_ptr_position,*(data_ptr+data_ptr_position));

            //PIN Status Template DO
            if(SIMINSTR_DecodeUSIMFCP_PSDO(cur_sim_task,
                                           (data_ptr + data_ptr_position),
                                           &data_ptr_position,
                                           is_adf_usim,
                                           &chv1_enabled))
            {
                if(chv1_enabled)
                {
                    filecharacter_efstructure &= ~BIT_7;
                }
                else
                {
                    filecharacter_efstructure |= BIT_7;
                }
            }
            else
            {
                filecharacter_efstructure |= BIT_7;//We regard this as no CHV1
                //SCI_ASSERT(0);
            }

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP6 %d 0x%x",data_ptr_position,*(data_ptr+data_ptr_position));

            //Total file size//no use in 2G

            //translate 3G FCP into 2G response data
            out_ptr->freespace_filesize = 0;//no use for a DF

            if(SIM_FILE_MF == file_name)
            {
                out_ptr->file_type = 1;//MF
            }
            else
            {
                out_ptr->file_type = 2;//DF
            }

            if(SIM_FILE_ADF_USIM == file_name)
            {
                out_ptr->file_id = 0x7FFF;
            }
            else
            {
                out_ptr->file_id = fcp_file_id;
            }

            out_ptr->cyclic_full_check = 0;//no use for a DF
            out_ptr->access_condition1 = 0;//no use for a DF
            out_ptr->file_status = 0;//no use for a DF
            out_ptr->filecharacter_efstructure = filecharacter_efstructure;
            out_ptr->dfnumber_recordlen = 0;//no use for a DF
            out_ptr->chv1_status = chv1_status;//We cannot get the remain-times here
            out_ptr->unblock_chv1_status = unblock_chv1_status;//We cannot get the remain-times here
            out_ptr->chv2_status = chv2_status;//We cannot get the remain-times here
            out_ptr->unblock_chv2_status = unblock_chv2_status;//We cannot get the remain-times here
#ifndef SIM_TEST_IN_WIN32
            out_ptr->usim_life_cycle_status = life_cycle_status;
#endif
        }
        else//Decode the EF
        {
            //File Descriptor
            if(SIMINSTR_DecodeUSIMFCP_FileDescriptor((data_ptr + data_ptr_position), &data_ptr_position, &filecharacter_efstructure, &dfnumber_recordlen))
            {
            }
            else
            {
                //SCI_TRACE_LOW:"SIM:SIMINSTR_DecodeUSIMFCP_FileDescriptor,ERROR"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_7973_112_2_17_22_59_31_4524, (uint8 *)"");
                return FALSE;
            }

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP7 %d 0x%x",data_ptr_position,*(data_ptr+data_ptr_position));

            //File identifier
            if(SIMINSTR_DecodeUSIMFCP_FileID((data_ptr + data_ptr_position), &data_ptr_position, &fcp_file_id))
            {

            }
            else
            {
                //SCI_TRACE_LOW:"SIM:SIMINSTR_DecodeUSIMFCP_FileID,ERROR"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_7986_112_2_17_22_59_31_4525, (uint8 *)"");
                return FALSE;
            }

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP8 %d 0x%x",data_ptr_position,*(data_ptr+data_ptr_position));

            //Proprietary information
            //Proprietary information is optional for an EF
            //No use for an EF
            SIMINSTR_DecodeUSIMFCP_ProprietaryInfo((data_ptr + data_ptr_position), &data_ptr_position, &dummy);

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP9 %d 0x%x",data_ptr_position,*(data_ptr+data_ptr_position));

            //Life Cycle Status Integer
            if(SIMINSTR_DecodeUSIMFCP_LifeCycleStatus((data_ptr + data_ptr_position),  &data_ptr_position, &life_cycle_status))
            {
            }
            else
            {
                //SCI_TRACE_LOW:"SIM:SIMINSTR_DecodeUSIMFCP_LifeCycleStatus,ERROR"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_8005_112_2_17_22_59_31_4526, (uint8 *)"");
                return FALSE;
            }

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP10 %d 0x%x",data_ptr_position,*(data_ptr+data_ptr_position));

            //Security attributes
            if(SIMINSTR_DecodeUSIMFCP_SecurityAttributes((data_ptr + data_ptr_position),  &data_ptr_position, &access_condition1, &access_condition2, arr_info))
            {
            }
            else
            {
                //SCI_TRACE_LOW:"SIM:SIMINSTR_DecodeUSIMFCP_SecurityAttributes,ERROR"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_8017_112_2_17_22_59_31_4527, (uint8 *)"");
                return FALSE;
            }

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP11 %d 0x%x",data_ptr_position,*(data_ptr+data_ptr_position));

            //File size
            if(SIMINSTR_DecodeUSIMFCP_FileSize((data_ptr + data_ptr_position), &data_ptr_position, &file_size))
            {

            }
            else
            {
                //The file size is mandatory for EF
                //SCI_TRACE_LOW:"SIM:SIMINSTR_DecodeUSIMFCP_FileSize,ERROR"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_8031_112_2_17_22_59_31_4528, (uint8 *)"");
                return FALSE;
            }

            //SCI_TRACE_LOW("SIM:DecodeUSIMFCP12 %d 0x%x",data_ptr_position,*(data_ptr+data_ptr_position));

            //No use in 2G, so we will not decode this.
            //Total file size
            //Short File Identifier(SFI)

            //Translate 3G FCP into 2G response data
            out_ptr->freespace_filesize = file_size;
            out_ptr->file_type = 4;//EF
            out_ptr->file_id = fcp_file_id;

            out_ptr->cyclic_full_check = 0;// used only in 2G WINDOWS simulator
            //access_condition1 is UPDATE/READ/INCREASE
            out_ptr->access_condition1 = access_condition1;//If the EF is EFarr, we cannot make sure here.
            //file status decide wether a EF is validated or invalidated,
            //and wether the EF can be readed/updated when it is invalidated?
            out_ptr->file_status = BIT_0;//we cannot make sure here. Just set it as validated.

            out_ptr->filecharacter_efstructure = filecharacter_efstructure;
            out_ptr->dfnumber_recordlen = dfnumber_recordlen;
            out_ptr->chv1_status = 0;//no use for 2G
            out_ptr->unblock_chv1_status = 0;//no use for 2G
            out_ptr->chv2_status = 0;//no use for 2G
            out_ptr->unblock_chv2_status = 0;//no use for 2G
#ifndef SIM_TEST_IN_WIN32
            out_ptr->usim_life_cycle_status = life_cycle_status;
#endif
        }

        //SCI_TRACE_LOW:"SIM:DecodeUSIMFCP final: filesize 0x%x, fileid 0x%x, chara 0x%x, rec_len 0x%x, status 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_8063_112_2_17_22_59_31_4529, (uint8 *)"ddddd", out_ptr->freespace_filesize, out_ptr->file_id, out_ptr->filecharacter_efstructure, out_ptr->dfnumber_recordlen, out_ptr->file_status);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************
//    Description:
//      ETST TS102.221 9.3.1 Decide the security environment according to the PIN status.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC SIM_SEC_ENV_ENUM SIMINSTR_GET_CUR_SEC_ENV(uint32 cur_sim_task)
{
    SIM_SEC_ENV_ENUM curr_sec_env = 0;

    /* Currently we are assuming that whatever the PIN status is present
    * in the SIM context will be updated one. This will be upated whenver
    * a PIN related command is executed in the SIM.*/

    /* According to the table 9.1 in 102.221*/
    if((s_sim_usim_pin1_status[cur_sim_task] == SIM_PIN_ENABLED)
        || (s_sim_usim_pin1_status[cur_sim_task] == SIM_PIN_ENABLED_AND_BLOCKED)
        || (s_sim_usim_pin1_status[cur_sim_task] == SIM_PIN_VERIFIED))
    {
        curr_sec_env = SIM_SE_1;
    }
    else
    {
        /* Application PIN is disabled, hence look at UNIVERSAL PIN status
        * and UUP status also to get the sec env*/
        if(((s_sim_usim_univ_pin_status[cur_sim_task] == SIM_PIN_ENABLED)
            || (s_sim_usim_univ_pin_status[cur_sim_task] == SIM_PIN_ENABLED_AND_BLOCKED)
            || (s_sim_usim_univ_pin_status[cur_sim_task] == SIM_PIN_VERIFIED))
            && (s_sim_usim_use_univ_pin[cur_sim_task] == FALSE))
        {
            curr_sec_env = SIM_SE_1;
        }
        else
        {
            curr_sec_env = SIM_SE_0;
        }
    }

    return curr_sec_env;
}

/**********************************************************************
//    Description:
//      When an EF's security attributes is in a reference expanded mode, we should
//       read the EFarr and decode the SA info.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMINSTR_GetARR(uint32 cur_sim_task, SIM_FILE_NAME_E cur_file_name, SIM_FCP_REF_ARR_T arr_info, uint16 *access_condition1, uint8 *access_condition2)
{
#ifndef SIM_TEST_IN_WIN32
    SIM_FILE_NAME_E arr_file_name;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SEC_ENV_ENUM curr_sec_env = SIM_SE_0;
    int8 instr_num = 0;
    uint8 i = 0;
    uint8 arr_record_index = 1;
    uint16 cur_file_id = 0;
    uint16 arr_rec_len = 0;
    uint16 data_len = sizeof(SIM_RESPONSE_DATA_OF_SELECT_T);
    SIM_SIG_RESPONSE_IND_T *instr_response = NULL;
    SIM_FILE_NAME_E file_route[SIM_FILE_LEVEL_E_NUM*2] = {0};
    SIMINSTR_RESULT_TYPE_E instr_result;
    BOOLEAN decode_arr_result = FALSE;

    if(arr_info.is_ref_exist)
    {
        //We want to get the EFacm/acmmax/puct/vgcss/vbss's access condition for UPDATE.
        // Is the access condition is PIN/PIN2 , PIN/ADM?
        // In fact, EFict,EFoct,EFmsucp,EFmsisdn(under ADFusim) are also need. But we donnot support these EFs now.
        if(0 != arr_info.rec_index_valid_num)
        {
            if(0x2F06 == arr_info.file_id)
            {
                arr_file_name = SIM_FILE_EF_MF_ARR;
            }
            else
            {
#ifdef YWD_REF_ARR_SELF_TEST
                // now the current DF is ....?
                arr_file_name = SIMFS_GetCurDirectoryFile(cur_sim_task);

                if((SIM_FILE_DF_TELECOM == arr_file_name)
                    || (SIM_FILE_DF_TELECOM_PHONEBOOK == arr_file_name))
                {
                    //Because we only judge several EFs in ADFusim. So we check EFarr in ADFusim and MF.
                    arr_file_name = SIM_FILE_EF_TELECOM_ARR;//we didn't support DFtel's EFarr
                }
                else
#endif
                {
                    arr_file_name = SIM_FILE_EF_USIM_ARR;
                }
            }

            //SCI_TRACE_LOW("SIM EFarr test point1 0x%x",arr_info.file_id);//wuding test code

            //Maybe SEID is exist
            if(0 != arr_info.seid_valid_num)
            {
                //SEID is 2 at most, if more than 2, return FALSE.
                if(arr_info.seid_valid_num > 2)
                {
                    return FALSE;
                }
                else
                {
                    //get which SEID, then find out the correspond arr-record.
                    curr_sec_env = SIMINSTR_GET_CUR_SEC_ENV(cur_sim_task);

                    if(curr_sec_env == arr_info.seid[0])
                    {
                        arr_record_index = arr_info.rec_index[0];
                    }
                    else
                    {
                        if(2 == arr_info.seid_valid_num)
                        {
                            arr_record_index = arr_info.rec_index[1];
                        }
                        else
                        {
                            return FALSE;
                        }
                    }
                }
            }
            else
            {
                arr_record_index = arr_info.rec_index[0];//no SEID, arr rec index is in [0]
            }

            ClearInstrList(cur_sim_task);

            for(;;)
            {
                if(0 != arr_record_index)
                {
                    ser_result = SIMINSTR_ReadEF(cur_sim_task, arr_file_name, 0, arr_record_index, SIM_RECORD_ABSOLUTE, &arr_rec_len, &data_len, &(s_sim_cur_file_arr_record[cur_sim_task][2]));
#ifdef YWD_REF_ARR_SELF_TEST
#ifdef YWD_REF_ARR_ERROR_SELF_TEST
                    ser_result = SIM_SERVICE_SELECT_NO_SUCH_FILE;
                    //SCI_TRACE_LOW:"SIM EFarr test point2 %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_8212_112_2_17_22_59_32_4530, (uint8 *)"d", ser_result);
#endif
#endif
                }
                else
                {
                    return FALSE;//no change, return.
                }

                //READ has problem
                if(SIM_SERVICE_OK == ser_result)
                {
                    break;//READ successfully, quit the loop.
                }
                else
                {
                    //Maybe there is no EFarr in current DF. We should go to the upper level to
                    // get the EFarr until MF has been reached.
                    if(SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result)
                    {
                        //there are no EFarr even in MF, the sim card must have problem.
                        if(SIM_FILE_EF_MF_ARR == arr_file_name)
                        {
                            //we'd better SELECT the DF just now.
                            break;
                        }
                        else
                        {
                            //From the structrue we supported, we will SELECT MF EFarr if cannot get the ADFusim/DFtel EFarr.
                            arr_file_name = SIM_FILE_EF_MF_ARR;
                            continue;//continue the loop to SELECT MF EFarr
                        }
                    }
                    else
                    {
                        //unknown error, quit.
                        break;
                    }
                }
            }

            if(SIM_SERVICE_OK == ser_result)
            {
                //Re-map the EFarr record into Expanded Format, then decode.
                s_sim_cur_file_arr_record[cur_sim_task][0] = SIM_FCP_SEC_ATTR_EXP;
                s_sim_cur_file_arr_record[cur_sim_task][1] = (uint8)arr_rec_len;
                //Decode the access condition of READ/UPDATE/INCREASE/ACTIVE/DEACTIVE according to arr_record
                decode_arr_result = SIMINSTR_DecodeUSIMFCP_SA_Expanded(s_sim_cur_file_arr_record[cur_sim_task], access_condition1, access_condition2);
                //SCI_TRACE_LOW:"SIM EFarr decode %d,  0x%x 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_INSTRUCTION_8260_112_2_17_22_59_32_4531, (uint8 *)"ddd", decode_arr_result, *access_condition1, *access_condition2);
            }

            //Wether the operation of EFarr success or fail. We should SELECT back to the EFfile_name.
            instr_num = SIMFS_SelectFileInstr(cur_sim_task, cur_file_name, file_route);

            //can not select the file
            if(instr_num <= 0)
            {
                if(-1 == instr_num)
                {
                    return SIM_SERVICE_CARD_NOT_ACTIVE;
                }

                if(-2 == instr_num)
                {
                    return SIM_SERVICE_INVALIDATE_FILE_NAME;
                }
            }

            ClearInstrList(cur_sim_task);

            while(i < instr_num)
            {
                //get the file id
                cur_file_id = SIMFS_GetFileID(cur_sim_task, file_route[i]);
                //add a select instruction to the instruction list
                GetInstructionCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num]),
                                   SIMINSTR_SELECT);

                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_code[3] = SIM_SELECT_CMD_P2_NO_DATA_RETURNED;
                s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 0;//only SW1 SW2 will return

                //copy the file id to the select instruction data
                // 1, To SELECT ADFusim
                // 2, To SELECT a DF/EF under ADFusim, we should SELECT ADFusim firstly.
                if(((SIM_FILE_ADF_USIM == cur_file_name) || (SIM_FILE_ADF_USIM == file_route[i])))
                {
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, s_sim_usim_aid[cur_sim_task].len);
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[2] = 0x4;//when select ADF, this is 4.
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_code[4] = s_sim_usim_aid[cur_sim_task].len;
                    SCI_MEMCPY(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu.instr_data, s_sim_usim_aid[cur_sim_task].value, s_sim_usim_aid[cur_sim_task].len);
                }
                else
                {
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data = AllocFromInstrDataBuf(cur_sim_task, 2);
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[0] = (uint8)(cur_file_id >> 8);
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.instr_data[1] = (uint8)cur_file_id;
                }

                if(TB_SIM_T_0_PROTOCOL == g_sim_card_setting[cur_sim_task].t_protocol)
                {
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = 0;
                }
                else
                {
                    s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].instr_num].apdu.rx_expected_len = SIM_MAN_DF_SELECT_DATA_LEN;
                }

                s_sim_instruction_list[cur_sim_task].instr_num++;
                i++;
            }

            i = 0;

            for(; s_sim_instruction_list[cur_sim_task].cur_instr < s_sim_instruction_list[cur_sim_task].instr_num; s_sim_instruction_list[cur_sim_task].cur_instr++)
            {
                SendInstrCode(cur_sim_task, &(s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].apdu));
                ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

                if(SIM_SERVICE_OK != ser_result)
                {
                    ClearInstrList(cur_sim_task);
                    return FALSE;
                }

                instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);

                //the instruction is process error
                if((SIM_STATUS_OK_NORMAL != instr_result)
                    && (SIM_STATUS_OK_PROACTIVE_INFO != instr_result)
                    && (SIM_STATUS_OK_RESPONSE_DATA != instr_result)
                    && (SIM_STATUS_OK_WITH_UPDATE != instr_result))// should not SIM_STATUS_OK_RESEND_ADPU
                {
                    //analysis the failed reason
                    ser_result = HandleCommandsError(cur_sim_task, s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name, instr_result, SIM_ACCESS_ALW);
                    SCI_FREE_SIGNAL(instr_response);
                    ClearInstrList(cur_sim_task);
                    return FALSE;
                }

                //if a select instruction is successful
                if(SIMINSTR_SELECT == s_sim_instruction_list[cur_sim_task].instr_list[s_sim_instruction_list[cur_sim_task].cur_instr].instr_name)
                {
                    SIMFS_SetCurDirectoryFile(cur_sim_task, file_route[i]); //update the cur select file
                    SIMFS_SetCurFileName(cur_sim_task, file_route[i]); //update the cur select file
                    i++;
                }
            }

            SCI_FREE_SIGNAL(instr_response);
            ClearInstrList(cur_sim_task);
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;
#else//SIM_TEST_IN_WIN32
    *access_condition1 = 0x4000;//init (INCREASE, UPDAT,READ is ALW)
    *access_condition2 = 0x44;//init with ADM (REHABILITATE, INVALIDATE)
    return TRUE;
#endif//SIM_TEST_IN_WIN32
}
#endif//SIM_3G_USIM_SUPPORT

/**********************************************************************
//    Description:
//    Translate the sim-drv SELECT response into SIM_RESPONSE_DATA_OF_SELECT_T.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC void SIMINSTR_RemapSelectResposne(
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response, //out:the status of the file
    uint8 *data_buf,  //IN:
    uint8 data_len //IN:
)
{
    SCI_MEMSET(select_response, 0, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T)); //clear

    select_response->sim_rfu1 = 0;//RFU only
    select_response->freespace_filesize = (uint16)(data_buf[SIM_SELECT_DATA_FS_POS + 1]) + ((uint16)(data_buf[SIM_SELECT_DATA_FS_POS]) << 8);
    select_response->file_id = (uint16)(data_buf[SIM_SELECT_DATA_FILE_ID_POS + 1]) + ((uint16)(data_buf[SIM_SELECT_DATA_FILE_ID_POS]) << 8);
    select_response->file_type = data_buf[SIM_SELECT_DATA_FILE_TYPE_POS];

    if(data_len > SIM_SELECT_FILE_TYPE_POS) //Some STATUS instruction only return 7 bytes.
    {
        select_response->cyclic_full_check = data_buf[SIM_SELECT_DATA_CYC_INCREASE];//only for cyclic EF
        select_response->access_condition1 = (uint16)(data_buf[SIM_SELECT_DATA_ACC_COND1 + 1]) + ((uint16)(data_buf[SIM_SELECT_DATA_ACC_COND1]) << 8);
        select_response->access_condition2 = data_buf[SIM_SELECT_DATA_ACC_COND1 + 2];
        select_response->file_status = data_buf[SIM_SELECT_DATA_FILE_STATUS_POS];
        select_response->remain_len = data_buf[SIM_SELECT_DATA_REMAIN_LEN_POS];
        select_response->filecharacter_efstructure = data_buf[SIM_SELECT_DATA_FILECHA_EFSTR_POS];
        select_response->dfnumber_recordlen = data_buf[SIM_SELECT_DATA_DFNUM_RECLEN_POS];

        if(data_len > (SIM_SELECT_DATA_DFNUM_RECLEN_POS + 1)) //EF response finished, return. DF/MF contiue.
        {
            select_response->ef_number = data_buf[SIM_SELECT_DATA_EFNUM_POS];
            select_response->chv_ad_code_num = data_buf[SIM_SELECT_DATA_CHVNUM_POS];
            select_response->sim_rfu2 = data_buf[SIM_SELECT_DATA_RFU2_POS];
            select_response->chv1_status = data_buf[SIM_SELECT_DATA_CHV1_POS];
            select_response->unblock_chv1_status = data_buf[SIM_SELECT_DATA_PUK1_POS];
            select_response->chv2_status = data_buf[SIM_SELECT_DATA_CHV2_POS];
            select_response->unblock_chv2_status = data_buf[SIM_SELECT_DATA_PUK2_POS];
        }
    }

    return;
}

/**********************************************************************
//    Description:
//      Clear the global variables because of a different sim card inserted.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMINSTR_HotSwap_ClearStaticVariable(uint32 cur_sim_task)
{
#ifdef SIM_3G_USIM_SUPPORT
    s_sim_usim_pin1_status[cur_sim_task] = SIM_PIN_NOT_INITIALIZED;
    s_sim_usim_univ_pin_status[cur_sim_task] = SIM_PIN_NOT_INITIALIZED;
    s_sim_usim_use_univ_pin[cur_sim_task] = FALSE;
    s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
    s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;
#endif
}

#ifdef SIM_DROP_RESTORE_SUPPORT
/**********************************************************************
//    Description:
// 	   
//    Global resource dependence :
//    Author:wuding.yang
//    Note: 
***********************************************************************/
void SIMINSTR_SetAuthInAuthFlag(uint32 cur_sim_task, BOOLEAN flag)
{
    SCI_TRACE_LOW("SIM%d:SetAuthInAuthFlag %d",cur_sim_task,flag);
    s_sim_recv_new_auth_in_auth[cur_sim_task] = flag;
}

/**********************************************************************
//    Description:
// 	   
//    Global resource dependence :
//    Author:wuding.yang
//    Note: 
***********************************************************************/
BOOLEAN SIMINSTR_GetAuthInAuthFlag(uint32 cur_sim_task)
{
    SCI_TRACE_LOW("SIM%d:GetAuthInAuthFlag %d",cur_sim_task,s_sim_recv_new_auth_in_auth[cur_sim_task]);
    return s_sim_recv_new_auth_in_auth[cur_sim_task];
}

/**********************************************************************
//    Description:
// 	   
//    Global resource dependence :
//    Author:wuding.yang
//    Note: 
***********************************************************************/
void SIMINSTR_SetGMMAuthInAuthFlag(uint32 cur_sim_task, BOOLEAN flag)
{
    SCI_TRACE_LOW("SIM%d:SetGMMAuthInAuthFlag %d",cur_sim_task,flag);
    s_sim_recv_new_gmm_auth_in_auth[cur_sim_task] = flag;
}

/**********************************************************************
//    Description:
// 	   
//    Global resource dependence :
//    Author:wuding.yang
//    Note: 
***********************************************************************/
BOOLEAN SIMINSTR_GetGMMAuthInAuthFlag(uint32 cur_sim_task)
{
    SCI_TRACE_LOW("SIM%d:GetGMMAuthInAuthFlag %d",cur_sim_task,s_sim_recv_new_gmm_auth_in_auth[cur_sim_task]);
    return s_sim_recv_new_gmm_auth_in_auth[cur_sim_task];
}

/**********************************************************************
//    Description:
// 	   
//    Global resource dependence :
//    Author:wuding.yang
//    Note: 
***********************************************************************/
STATIC uint32 SIMINSTR_CountWaitTime(uint32 cur_sim_task,uint8 rx_expected_len)
{
    uint32 total_wait_time_value = SIMHAL_GetWWT(cur_sim_task);

    total_wait_time_value = total_wait_time_value*rx_expected_len;

//////////////////////////////////////////special for sim drop
    if ((SIMMMSIG_IsInAuthentication(cur_sim_task)) 
        || SIMMMSIG_IsInGMMAuthentication(cur_sim_task))
    {
        if (total_wait_time_value > 3500)
        {
            SCI_TRACE_LOW("SIM%d: auth change wait time %d to 3500",cur_sim_task,total_wait_time_value);
            total_wait_time_value = 3500;
        }
    }
    else
    {
        total_wait_time_value = 8000;
    }
//////////////////////////////////////////special for sim drop

    return total_wait_time_value;
}
#endif
#ifdef MODEM_PLATFORM
/**********************************************************************
//    Description:
//     
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_CSIM(uint32 cur_sim_task, uint8 data_len, uint8 *data_ptr)
{
#ifndef SIM_TEST_IN_WIN32
    TB_SIM_APDU_T temp_apdu;
    SIM_SIG_RESPONSE_IND_T *instr_response;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
	SIMINSTR_RESULT_TYPE_E instr_result;
    static uint8 temp_apdu_data[255];

    if (data_len >= 5)
    {
        temp_apdu.instr_code[TB_SIM_APDU_CLA] = data_ptr[0];
        temp_apdu.instr_code[TB_SIM_APDU_INS] = data_ptr[1];
        temp_apdu.instr_code[TB_SIM_APDU_P1] = data_ptr[2];
        temp_apdu.instr_code[TB_SIM_APDU_P2] = data_ptr[3];
        temp_apdu.instr_code[TB_SIM_APDU_P3] = data_ptr[4];
        temp_apdu.instr_data = &temp_apdu_data[0];
        if (data_len > 5)
        {
            SCI_MEMCPY(temp_apdu.instr_data,&data_ptr[5],(data_len-5));
        }
        temp_apdu.rx_expected_len = 255;
        if ((0xA4 == temp_apdu.instr_code[TB_SIM_APDU_INS]) 
            || (0xD6 == temp_apdu.instr_code[TB_SIM_APDU_INS]) 
            || (0xDC == temp_apdu.instr_code[TB_SIM_APDU_INS]) 
            || (0xA2 == temp_apdu.instr_code[TB_SIM_APDU_INS]) 
            || (0x32 == temp_apdu.instr_code[TB_SIM_APDU_INS]) 
            || (0x20 == temp_apdu.instr_code[TB_SIM_APDU_INS]) 
            || (0x24 == temp_apdu.instr_code[TB_SIM_APDU_INS])
            || (0x26 == temp_apdu.instr_code[TB_SIM_APDU_INS]) 
            || (0x28 == temp_apdu.instr_code[TB_SIM_APDU_INS]) 
            || (0x2C == temp_apdu.instr_code[TB_SIM_APDU_INS])
            || (0x88 == temp_apdu.instr_code[TB_SIM_APDU_INS])
            || (0x10 == temp_apdu.instr_code[TB_SIM_APDU_INS])
            || (0xC2 == temp_apdu.instr_code[TB_SIM_APDU_INS])
            || (0x14 == temp_apdu.instr_code[TB_SIM_APDU_INS]))
        {
            temp_apdu.instr_type = SIMHAL_SIMINSTR_SEND_DATA;
            temp_apdu.rx_expected_len = 0;
        }
        else if ((0xF2 == temp_apdu.instr_code[TB_SIM_APDU_INS]) 
            || (0xB0 == temp_apdu.instr_code[TB_SIM_APDU_INS])
            || (0xB2 == temp_apdu.instr_code[TB_SIM_APDU_INS])
            || (0xC0 == temp_apdu.instr_code[TB_SIM_APDU_INS])
            || (0x12 == temp_apdu.instr_code[TB_SIM_APDU_INS]))
        {
            temp_apdu.instr_type = SIMHAL_SIMINSTR_RECV_DATA;
            temp_apdu.rx_expected_len = temp_apdu.instr_code[TB_SIM_APDU_P3];
        }
        else if ((0x04 == temp_apdu.instr_code[TB_SIM_APDU_INS]) 
            || (0x44 == temp_apdu.instr_code[TB_SIM_APDU_INS])
            || (0xFA == temp_apdu.instr_code[TB_SIM_APDU_INS]))
        {
            temp_apdu.instr_type = SIMHAL_SIMINSTR_NONE_DATA;
        }
        else
        {
            //Unknown INS
#ifdef SOFTSIM_SUPPORT
            temp_apdu.instr_type = SIMHAL_SIMINSTR_SEND_DATA;
            temp_apdu.rx_expected_len = 0;
#else
            ser_result = SIM_SERVICE_ERROR_UNKNOW;
#endif
        }

        if (SIM_SERVICE_OK == ser_result)
        {
            //send a instruction to the sim card
            SendInstrCode(cur_sim_task, &temp_apdu);

            //receive the response of the instruciton from the sim card
            ser_result = ReceiveInstrResponse(cur_sim_task, &instr_response);

            if (SIM_SERVICE_OK == ser_result)
            {
                // save the response data to the global variable
                SIMINSTR_CRSM_SaveResponseInfo(cur_sim_task, SIM_FILE_TOTAL_NUM, instr_response);

				instr_result = SIMINSTR_GetStatusWordType(cur_sim_task, instr_response->sw1, instr_response->sw2, instr_response->data_len);
#ifdef SIM_3G_USIM_SUPPORT
		        if ((0x62 == instr_response->sw1) || (0x63 == instr_response->sw1))
		        {
		            instr_result = SIM_STATUS_OK_RESPONSE_DATA;
		            instr_response->sw2 = 0;
		        }
#endif

#if 0
				//the instruction is process error
		        if ((SIM_STATUS_OK_NORMAL != instr_result) 
					&& (SIM_STATUS_OK_PROACTIVE_INFO != instr_result) 
					&& (SIM_STATUS_OK_RESPONSE_DATA != instr_result) 
					&& (SIM_STATUS_OK_WITH_UPDATE != instr_result) 
					&& (SIM_STATUS_OK_RESEND_ADPU != instr_result))
		        {
		            ser_result = SIM_SERVICE_ERROR_UNKNOW;
		        }
                else
#endif
                {
					if  (0xA4 == temp_apdu.instr_code[TB_SIM_APDU_INS]
                     && ((0x94 != instr_response->sw1) && (0x04 != instr_response->sw2)))
					{
						SIM_FILE_NAME_E ef_name;
						uint16 file_id = 0;
						uint8 path_len = 0;
						uint8 path_id[50] = {0};//I think 50 is enough

						file_id = data_ptr[data_len-2];
						file_id <<= 8;
						file_id |= data_ptr[data_len-1];

						if (data_len > 7)
						{
							path_len = data_len - 5 - 2;
							SCI_MEMCPY(path_id,&data_ptr[5],path_len);
						}
						else
						{
							path_len = 0;
						}

	SCI_TRACE_LOW("SIMINSTR_CSIM file_id=0x%x, path_len=%d, 0x%x 0x%x 0x%x 0x%x",\
		file_id,path_len,path_id[0],path_id[1],path_id[2],path_id[3]);

						ef_name = SIMFS_GetFileName(cur_sim_task,file_id,path_len,path_id);

			            //update the cur select file
			            SIMFS_SetCurDirectoryFile(cur_sim_task, ef_name);

			            //update the cur select file
			            SIMFS_SetCurFileName(cur_sim_task, ef_name);
					}
                }
            }

			//free the response signal
			SCI_FREE_SIGNAL(instr_response);
        }
    }
    else
    {
        ser_result = SIM_SERVICE_ERROR_UNKNOW;
    }

    return ser_result;
#else
    return SIM_SERVICE_OK;
#endif
}
#endif

/*****************************************************************************/
//  Description : clear the current file response when re-initialize,so the SW will
//  re-select the ADF usim and get the pin status
//  Global resource dependence : none
//  Author: yongxia.zhang
//  Note:
/*****************************************************************************/
void SIMINS_ClearCurFileResponse(uint32 cur_sim_task)
{
    SCI_MEMSET(&s_sim_cur_file_response[cur_sim_task],0,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
}

#ifdef   __cplusplus
}
#endif
