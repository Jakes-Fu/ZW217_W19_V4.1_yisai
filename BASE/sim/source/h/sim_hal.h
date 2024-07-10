/******************************************************************************
** File Name:      sim_hal.h                                                 *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the header file for the SIM HAL                   *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/

#ifndef _SIM_HAL_H_
#define _SIM_HAL_H_
#ifdef   __cplusplus
extern   "C"
{
#endif

#include "sim_drv.h"

#ifdef WIN32
#include "sim_type.h"
#endif

//the interupt register buf len
#define TB_SIM_MAX_MSG_SIZE             261     //the max Message len 256 + 5

//the times that the sim card can reset when it can not
//pass the ATR
#define TB_SIM_MAX_B_RESET_TIMES            3
#define TB_SIM_MAX_POWER_RESET_TIMES        2//wuding 2009.9.5

//the ATR macros
#define TB_SIM_ATR_MAX_DATA_LEN         33

//the ATR TA1 value that do not need the PPS procedure
#define TB_SIM_ATR_DEFAULT_A1_1         0x01
#define TB_SIM_ATR_DEFAULT_A1_2         0x11

//the always present ATR character byter number
#define TB_SIM_ATR_ALWAYS_CHARACTER_NUM 2
#define TB_SIM_ATR_HISTORY_CHARACTER_MARK   0x0f
#define TB_SIM_ATR_TA_CHARACTER_MARK        0x10
#define TB_SIM_ATR_TB_CHARACTER_MARK        0x20
#define TB_SIM_ATR_TC_CHARACTER_MARK        0x40
#define TB_SIM_ATR_TD_CHARACTER_MARK        0x80
#define TB_SIM_ATR_T_PROTOCAL_MARK          0x0f
#define TB_SIM_ATR_TD_NO_MORE_CHARACTER 0x00
#define TB_SIM_ATR_Pl1_MARK             0x1f        //bit1~5
#define TB_SIM_ATR_T0_POS                  1
#define TB_SIM_ATR_TS_VALUE1               0x3f
#define TB_SIM_ATR_TS_VALUE2               0x3b
#define TB_SIM_ATR_TC1_VALUE1              0xff
#define TB_SIM_ATR_TC1_VALUE2              0x0
#define TB_SIM_PPS_RESPONSE_SIZE            3
#define TB_SIM_RESPONSE_NULL_BYTE          0x60

//the default factor value for the SIM card
#define TB_SIM_DEFAULT_FACTOR_I            50
#define TB_SIM_DEFAULT_FACTOR_P            5
#define TB_SIM_DEFAULT_FACTOR_N            0
#define TB_SIM_HIGH_FACTOR_F               512
#define TB_SIM_RX_STATUS_WORD_NUM           2

#define TB_SIM_RESPONSE_DATA_EXPEND_LEN    2       //the length of the byte that not really SIM data in
//the SIM response such as the 2 byte status words or
//the instruction code byte(may not count in)

//the T = 0 protocol
#define TB_SIM_T_0_PROTOCOL                 0

typedef enum
{
    SIMHAL_VOLT_CLASS_A_ONLY,
    SIMHAL_VOLT_CLASS_B_ONLY,
    SIMHAL_VOLT_CLASS_C_ONLY,
    SIMHAL_VOLT_CLASS_AB,
    SIMHAL_VOLT_CLASS_BC,
    SIMHAL_VOLT_CLASS_ABC,
    SIMHAL_VOLT_CLASS_RFU
}
SIMHAL_VOLT_CLASS_E;

typedef struct
{
    uint8 card_index;      //which sim card
    BOOLEAN is_reset_ok;
    BOOLEAN is_atr_volt_info;
    BOOLEAN is_atr_1_8v_only;
    uint16 t_protocol;
} TB_SIM_1_8V_DETECT_T;

//the position of bytes in the instruction header of the command TPDU
typedef enum
{
    TB_SIM_APDU_CLA = 0,        //Class byte in the command header
    TB_SIM_APDU_INS,            //Instruction bye in the command header
    TB_SIM_APDU_P1,         //The first Parameter byte in the command header
    TB_SIM_APDU_P2,         //The second Parameter byte in the command header
    TB_SIM_APDU_P3,         //The third Parameter byte in the command header
    TB_SIM_APDU_CHEADER_NUM //The byte number of the APDU command header
} TB_SIM_APDU_BYTE_E;

//the instruction types
typedef enum
{
    SIMHAL_SIMINSTR_NONE_DATA,          //the instructions that do not send data and recv data
    SIMHAL_SIMINSTR_SEND_DATA,          //the instructions that only send data
    SIMHAL_SIMINSTR_RECV_DATA,          //the instructions that only receive data
    SIMHAL_SIMINSTR_SEND_AND_RECV_DATA  //the instructions that both send data and recv data
} SIMHAL_SIMINSTR_TYPE_E;

//a instruction code including the command header and the command data
//that will be send to the SIM card to make the instruciton proccessed
typedef struct
{
    uint16  rx_expected_len;        //the expect data length that will receive from the SIM
    //card when the instruciton excute succeeded
    SIMHAL_SIMINSTR_TYPE_E instr_type;
    uint8   instr_code[TB_SIM_APDU_CHEADER_NUM];
    uint8  *instr_data;
} TB_SIM_APDU_T;

//the buf that used in the RX
typedef struct
{
    BOOLEAN     is_locked;              //wheather the rx buf is locked.when the rx buf
    //is locked,can not put data into rx buf anymore
    uint16      rx_expected_len;        //expected to receive data's len
    uint16      cur_pos;                //the cur pos that will be process
    uint16      check_pos;
    uint16      next_atr_td_pos;        //the next atr_td_pos
    uint32      dummy_cache_line1[8];
    uint8       buf[TB_SIM_MAX_MSG_SIZE];   //the buf used to save rx data
    uint32      dummy_cache_line2[8];
} TB_SIM_RX_BUF_T;

//the buf that used in the TX
typedef struct SIM_TX_BUF_T
{
    uint16      tx_data_len;            //the tx data len need to send to the sim card
    uint16      cur_pos;                //the cur send data pos
    uint8       buf[TB_SIM_MAX_MSG_SIZE];   //the buf that to save the tx data
} TB_SIM_TX_BUF_T;

//the sim card command status that indicate what the card is doing now
typedef enum sim_command_status_tag
{
    TB_SIM_NO_CARD,
    TB_SIM_NOT_ACTIVED,
    TB_SIM_1_8V_DETECT,
    TB_SIM_WAITING_FOR_ATR,
    TB_SIM_PROCESSING_ATR,
    TB_SIM_PROCESSING_PPS,
    TB_SIM_RECEIVING_RESPONSE,
    TB_SIM_ERROR                    //a error status that the drive will not do any thing
} TB_SIM_STATUS_E;

//the ATR data structure
typedef struct  simAtrTag
{
    uint16      t_protocol;
    uint16      TA1_pos;    //save down the TA1 position in the ATR data used to identity the really TA1 byte of ATR data
    uint16      TB1_pos;    //save down the TB1 position in the ATR data used to identity the really TB1 byte of ATR data
    uint16      TC1_pos;    //save down the TC1 position in the ATR data used to identity the really TC1 byte of ATR data
    uint16      TC2_pos;    //save down the TC1 position in the ATR data used to identity the really TC1 byte of ATR data
    uint16      F;  //the F factor of the SIM card
    uint8       D;  //the D factor of the SIM card
    uint8       I;  //the I factor of the SIM card
    uint8       P;  //the P factor of the SIM card
    uint8       N;  //the N factor of the SIM card,the extra guardtime request by the card.befor receive the next
    //next character,the card requires a delay of at least (12 + N) etu from the start leading edge
    //of the previous character
} TB_SIM_ATR_DATA_T;

//the PPS procedure speed
typedef enum
{
    TB_SIM_PPS_DEFAULT_SPEED = 3,
    TB_SIM_PPS_ENHANCED_SPEED = 4
} TB_SIM_PPS_TYPE_E;

//the PPS Info
typedef struct
{
    BOOLEAN is_current;
    uint8  *pps_data;
    TB_SIM_PPS_TYPE_E pps_type;
} TB_SIM_PPS_INFO_T;

typedef enum
{
    TB_SIM_RESET_NO_CHANGE,     //reset the SIM card without any change on the sim register
    TB_SIM_RESET_FOR_POWER,     //reset the sim card to change the power on the simc card
    TB_SIM_RESET_FOR_BT,        //reset the sim card to change the bit convetion and logic level setting
    TB_SIM_RESET_FOR_PARITY     //reset the sim card to change the parity check setting
} TB_SIM_RESET_TYPE_E;

typedef enum
{
    SIM_FACTOR_F_372 = 372,
    SIM_FACTOR_F_512 = 512,
    SIM_FACTOR_F_558 = 558,
    SIM_FACTOR_F_744 = 744,
    SIM_FACTOR_F_768 = 768,
    SIM_FACTOR_F_1024 = 1024,
    SIM_FACTOR_F_1116 = 1116,
    SIM_FACTOR_F_1488 = 1488,
    SIM_FACTOR_F_1536 = 1536,
    SIM_FACTOR_F_1860 = 1860,
    SIM_FACTOR_F_2048 = 2048
} TB_SIM_CLOCKRATE_FACTOR_E;

typedef enum
{
    SIM_FACTOR_D_1 = 1,
    SIM_FACTOR_D_2 = 2,
    SIM_FACTOR_D_4 = 4,
    SIM_FACTOR_D_8 = 8,
    SIM_FACTOR_D_12 = 12,
    SIM_FACTOR_D_16 = 16,
    SIM_FACTOR_D_20 = 20,
    SIM_FACTOR_D_32 = 32,
    SIM_FACTOR_D_64 = 64
} TB_SIM_BITRATE_FACTOR_E;

typedef enum
{
    SIMHAL_6302_REG_RST,
    SIMHAL_6302_REG_CLK,
    SIMHAL_6302_REG_DATA,
    SIMHAL_6302_REG_VSIM,
    SIMHAL_6302_REG_MAX
} SIMHAL_6302_REG_E;

typedef struct
{
    uint8   f_ta1;/* 4 uppermost bits of TA1 is FI */
    TB_SIM_CLOCKRATE_FACTOR_E   crFactor;/* F */
} TB_SIM_CLOCKRATE_FACTOR_T;

typedef struct
{
    uint8   d_ta1;/* 4 lowermost bits of TA1 is DI */
    TB_SIM_BITRATE_FACTOR_E     brFactor;/* D */
} TB_SIM_BITRATE_FACTOR_T;

#ifdef _DEBUG
#define SIM_DEBUG_BUF_SIZE  256
//the test buf use to reacord down some data to used in debug interrupt
typedef struct
{
    uint32  count;
    uint32  times;
    uint8   buf[SIM_DEBUG_BUF_SIZE];
} TB_SIM_TEST_BUF_T;
#endif

//send the SIM card exception interrupter problem to the SIM manager
typedef struct
{
    SIGNAL_VARS
    TB_SIM_INT_TYPE_E int_type;
} SIM_SIG_INT_IND_T;

//SIM card insert status

typedef enum
{
    TB_SIM_INSERTED_OK,
#ifdef SIM_SIMULATOR
    TB_SIM_INSERTED_NO_SIM,
#endif
    TB_SIM_INSERTED_WRONG_SIM,
    TB_SIM_INSERTED_ERROR,
    TB_SIM_INSERTED_CLASS_CHANGE
} SIMDRV_INSERT_STATUS_E;

typedef struct
{
    uint16 t_protocol;      //the t protocol of the SIM card
    SIMDRV_WORKING_VOLT_E   working_volt;
    SIMDRV_INSERT_STATUS_E insert_status;
} SIMDRV_CARD_INSERT_T;

typedef struct
{
    uint8   sw1;
    uint8   sw2;
    uint16  data_len;
    uint8  *data;
} SIMDRV_DATA_RESPONSE_T;

//the card insert signal to report the card insert status to the sim manager
typedef struct
{
    SIGNAL_VARS
    uint16 t_protocol;      //the t protocol of the SIM card
    BOOLEAN is_3v;      //is the sim working on 3v
    SIMDRV_INSERT_STATUS_E insert_status;
} SIM_SIG_CARD_INSERTED_IND_T;

typedef struct
{
    uint32   message;
    uint32   wparam;
    void     *lparam;
} SIMHAL_MSG;

/**********************************************************************
//    Description:
//      Initialize the Driver timer of the idle
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMHAL_Init(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Send a Instruction to the SIM card to start an operation on
//    The SIM card we needed
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_SendInstrCode(
    SIM_CARD_INDEX_E card_id,
    TB_SIM_APDU_T *apdu_ptr//in:the instruction code to be sent to SIM card
);

/**********************************************************************
//    Description:
//      Initialize the Driver timer of the idle mode
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_ResetDriver(BOOLEAN is_1_8v_detect, SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Reset the SIM card with or without change the working voltage for
//    the SIM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_ResetSIMCard(BOOLEAN is_1_8v_detect,
                            SIM_CARD_INDEX_E card_id,
                            BOOLEAN bChange //TRUE: reset the sim card with power change
                            //FALSE:reset the sim card without power change
                           );

/**********************************************************************
//    Description:
//      Make the SIM card to the Idle state,stop the clock of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_IdleSIMCard(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      PowerOff the SIM card and disable the SIM module
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_PowerOffSIM(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Set the logic level in the clock pin when the clock is disable
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_SetClockStopLevel(
    SIM_CARD_INDEX_E card_id,
    BOOLEAN bHighLevel  //is the logic level high
);

/**********************************************************************
//    Description:
//      judge whether the card is present or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_IsCardNotPresent(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Get the number of the null byte,it is used to judge whether the
//      ME need to waiting for the sim card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint8 SIMHAL_GetNullByteNum(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Get the status of the sim card,so the tool can see the status of the card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
TB_SIM_STATUS_E SIM_GetStatus(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      SIM task call this function to set the whether there is a 1.8V only SIM card inserted.
//      1, when ATR has 1.8V only infomation, call this func.
//      1, when RESP of DFgsm has 1.8V only infomation, call this func.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_SetVoltLimit(SIM_CARD_INDEX_E card_id, BOOLEAN is_1_8V_only);

/**********************************************************************
//    Description:
//      SIM task call this function to get the whether there is a 1.8V only SIM card inserted.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_GetVoltLimit(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//     get the SIM card's volt info in ATR data
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_GetATRVoltInfo(SIM_CARD_INDEX_E card_id, uint8 *atr_volt_byte);

/**********************************************************************
//    Description:
//      SIM task call this function to set the whether the 1.8V detect procedure finished
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_SetVoltDetectComplete(SIM_CARD_INDEX_E card_id, BOOLEAN is_complete);

/**********************************************************************
//    Description:
//      Reject the SIM card(deactive the SIM card and Power off the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_RejectSIMCard(SIM_CARD_INDEX_E card_id);

#ifdef SIM_6302_SWITCH
/**********************************************************************
//      Get the baseband chip is connected with 6302 or 6303. TRUE:6302, FALSE:6303.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_MultiSim_Is6302(void);

/**********************************************************************
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_MultiSim_Set_ParallelDual6302(BOOLEAN flag);

/**********************************************************************
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_MultiSim_IsParallelDual6302(void);
#endif

/**********************************************************************
//      Get the baseband chip is parallel dualsim or not. (6800H currrently)
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_IsParallelDualSIM(void);

/*****************************************************************************/
//  Description:
//     Clear the global variables because of a different sim card inserted.
//  Global resource dependence: NONE
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMHAL_HotSwap_ClearStaticVariable(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Trace the data in the sim rx buffer
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_PrintRxBuf(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Trace the data in the sim tx buffer
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMHAL_PrintTxBuf(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Trace the atr data sent by the sim card during the ATR
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_PrintATRDataBuf(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      trace the register value when exception occurs
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_PrintRegisterBuf(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:  get sim info about WWT(= 960*WI*Fi/f) in millisecond
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
uint32 SIMHAL_GetWWT(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:  
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_DisablePPSFeature(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Initialize the SIM card Driver, make it ready to warm reset
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_WARM_ResetDriver(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Warm RESET the SIM Card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_WARM_ResetSIMCard(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:  
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_GetParityResetFlag(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:  
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_SetParityResetFlag(SIM_CARD_INDEX_E card_id, BOOLEAN flag);

/**********************************************************************
//    Description:  
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_ChangeConvetion(SIM_CARD_INDEX_E card_id);

#ifdef   __cplusplus
}
#endif

#endif //_SIM_HAL_H_
