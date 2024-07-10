/**************************************************************************

  Copyright (C) 2004 SPREADTRUM COMMUNICATION CO.
  All rights reserved.

  This document contains proprietary information blonging to
  SPREADTRUM. Passing on and copying this document, use and
  communication of its contents is not permitted without prior
  writed authorization.

***************************************************************************/

/***************************************************************************

  Reversion information:
  File Name:    usim_interface.h
  Version:      0.1
  Author:       yongxia.zhang
  Date:         2004.04
  Description:  This is the interface header file for the USIM driver
***************************************************************************/
/***************************************************************************

                Edit Histroy
  Date          Name            Description
  --------------------------------------------------------------------------
  2004.04        yongxia.zhang    Create
***************************************************************************/


#ifndef _USIM_INTERFACE_H_
#define _USIM_INTERFACE_H_

#ifndef _GSM_IN_TDGSM_MODE
#include "tb_hal.h"

#ifdef   __cplusplus
extern   "C"
{
#endif

//SIM API
#define HAL_InitSimDrv          SIM_InitDriver
#define HAL_SendSimInstrCode    SIM_SendInstrCode
#define HAL_ResetSimCard        SIM_ResetSIMCard
#define HAL_WarmResetSimCard    SIM_WarmResetSimCard
#define HAL_PowerOffSIM         SIM_PowerOffSIM
#define HAL_ChangeCurrent       SIM_ChangeCurrent



// @rick.zhang type disagreement with TB_ISR defined in tb_comm.h
//#define TB_SUCCESS                0

/*
typedef struct
{
    uint32  message;    //message type such as SIM_CARD_INSERT etc
    uint32  wparam;     //reserved
    void    *lparam;    //pointer to the message entity
}TB_MSG;
*/
/*
typedef void (* TB_CALLBACK)(TB_MSG *);
*/

//move from tb_comm.h by wuding
//SIM driver relative define
//the instruction types
typedef enum
{
    TB_SIMINSTR_NONE_DATA,          //the instructions that do not send data and recv data
    TB_SIMINSTR_SEND_DATA,          //the instructions that only send data
    TB_SIMINSTR_RECV_DATA,          //the instructions that only receive data
    TB_SIMINSTR_SEND_AND_RECV_DATA, //the instructions that both send data and recv data
    TB_SIMINSTR_MAX                 //the MAX value for pclint waring e685
}
TB_SIMINSTR_TYPE_E;

//the type of POWER that can power on the SIM card.
typedef enum
{
    TB_SIM_WORKING_VOLT_1_8V,
    TB_SIM_WORKING_VOLT_3V,
    TB_SIM_WORKING_VOLT_5V
} TB_SIM_WORKING_VOLT_E;

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
} TB_SIM_INSERT_STATUS_E;

typedef struct
{
    uint16 t_protocol;      //the t protocol of the SIM card
    TB_SIM_WORKING_VOLT_E   working_volt;
    TB_SIM_INSERT_STATUS_E insert_status;
    int32 card_id; /* dual card dev */
} TB_SIM_CARD_INSERT_T;

typedef struct
{
    uint8   sw1;
    uint8   sw2;
    uint16  data_len;
    int32  card_id; /* dual card dev */
    uint8  *data;
} TB_SIM_DATA_RESPONSE_T;

typedef enum
{
    SIM_ARM_CLK_DIV_2 = 0,
    SIM_ARM_CLK_DIV_4,
    SIM_ARM_CLK_DIV_8,
    SIM_ARM_CLK_DIV_16,
    SIM_ARM_CLK_DIV_32,
    SIM_ARM_CLK_DIV_64,
    SIM_ARM_CLK_DIV_128,
    SIM_ARM_CLK_DIV_256
} TB_SIM_CLK_MODE_E;


typedef enum
{
    SIM_SEND_INSTR_OK,
    SIM_SEND_INSTR_CARD_BUSY,
    SIM_SEND_INSTR_INVALID_PARA
}
SIM_SEND_INSTR_RESULT_E;


typedef enum
{
    TB_SIM_APDU_CLA,
    TB_SIM_APDU_INS,
    TB_SIM_APDU_P1,
    TB_SIM_APDU_P2,
    TB_SIM_APDU_P3,
    TB_SIM_APDU_CHEADER_NUM    //the byte number of the APDU command header
} TB_SIM_APDU_BYTE_E;
//the SIM interrupt type that may come form ARM
typedef enum
{
    TB_SIMINT_RX_PARITY_ERROR,
    TB_SIMINT_TX_PARITY_ERROR,
    TB_SIMINT_CARD_UNRESPONSE,
    TB_SIMINT_CARD_IN,
    TB_SIMINT_CARD_OUT,
    TB_SIMINT_NEED_WAITTING         //cr7095
} TB_SIM_INT_TYPE_E;


typedef struct
{
    TB_SIM_INT_TYPE_E int_type;
} TB_SIM_INT_T;

//the signal type
typedef enum
{
    SIM_CARD_INSERT,
    SIM_DATA_RESPONSE,
    SIM_EXCEPTION_INT,
    SIM_CARD_NEED_WAITING,
    SIM_CARD_RESET
} TB_SIM_SIG_TYPE_E;
//a instruction code including the command header and the command data
//that will be send to the SIM card to make the instruciton proccessed
typedef struct
{
    uint16  rx_expected_len;        //the expect data length that will receive from the SIM
    //card when the instruciton excute succeeded
    TB_SIMINSTR_TYPE_E instr_type;
    uint8   instr_code[TB_SIM_APDU_CHEADER_NUM];
    uint8  *instr_data;
} TB_SIM_APDU_T;

/* if we can not find g_usim_

//the supply voltage on the SIM card
typedef enum
{
    TB_SIM_WORKING_VOLT_1D8V,   //The current supply voltage is 1.8V
    TB_SIM_WORKING_VOLT_3V,     //The current supply voltage is 3V
    TB_SIM_WORKING_VOLT_5V      //The current supply voltage is 5V
}TB_SIM_WORKING_VOLT_E;

// status of SIM card insert
typedef enum
{
    TB_SIM_INSERTED_OK,             //Card insert OK
    TB_SIM_INSERTED_ERROR,          //A card is inserted, but the ATR process is                                        //not successful
    TB_SIM_INSERTED_NO_SIM          //No SIM card is inserted in
}TB_SIM_INSERT_STATUS_E;

//the card inserted information passed to the transport layer
typedef struct
{
    TB_SIM_ATR_DATA_T    atr_data;          //Data received during the ATR process
    TB_SIM_WORKING_VOLT_E   working_volt;   //The current supply voltage
    TB_SIM_INSERT_STATUS_E insert_status;   //Card insert status
}TB_SIM_CARD_INSERT_T;

//the response TPDU passed to the transport layer
typedef struct
{
    uint8   sw1;                //The status word sw1
    uint8   sw2;                //The status word sw2
    uint16  data_len;           //the length of the response data, sw1 and sw2 is not                                   //included in it
    uint8*  data;               //The pointer to the response data
}TB_SIM_DATA_RESPONSE_T;
*/

//exceptional case type of SIM card
typedef enum
{
    TB_SIMEXP_RX_PARITY_ERROR,      //SIM card receive parity error
    TB_SIMEXP_TX_PARITY_ERROR,      //SIM card transmit parity error
    TB_SIMEXP_CARD_UNRESPONSE,      //SIM card un-response
    TB_SIMEXP_CARD_IN,              //SIM card insert in
    TB_SIMEXP_CARD_OUT,              //SIM card pull out
    TB_SIMEXP_T1_CARD_UNRESPONSE    //SIM card un-response under T1, ONLY for TS31.121 case 6.2   //CR205519 Edie.Wang
} TB_SIM_EXCEPTION_TYPE_E;

//the exceptional cases passed to the transport layer
typedef struct
{
    TB_SIM_EXCEPTION_TYPE_E exp_type;
    int32 card_id;
} TB_SIM_EXCEPTION_T;

/*
//the position of bytes in the instruction header of the command TPDU
typedef enum
{
    TB_SIM_APDU_CLA = 0,            //Class byte in the command header
    TB_SIM_APDU_INS,            //Instruction bye in the command header
    TB_SIM_APDU_P1,         //The first Parameter byte in the command header
    TB_SIM_APDU_P2,         //The second Parameter byte in the command header
    TB_SIM_APDU_P3,         //The third Parameter byte in the command header
    TB_SIM_APDU_LE,         //The Le byte in the command header
    TB_SIM_APDU_CHEADER_NUM //The byte number of the APDU command header
}TB_SIM_APDU_BYTE_E;
*/

/*
// the type of the command TPDU
typedef enum
{
    TB_SIMINSTR_NONE_DATA, //the instructions that do not send data and receive data
    TB_SIMINSTR_SEND_DATA,   //the instructions that only send data
    TB_SIMINSTR_RECV_DATA,   //the instructions that only receive data
    TB_SIMINSTR_SEND_AND_RECV_DATA  //the instructions that both send data and receive data
}TB_SIMINSTR_TYPE_E;
*/

//the instruction TPDU format of the transport layer
typedef struct
{
    uint16  rx_expected_len;    //the expect data length that will receive from the SIM
    //card when the instruction has been executed successfully.
    TB_SIMINSTR_TYPE_E  instr_type; //Four types of instruction, corresponding to                                           //the case1 to case 4 in the spec.
    uint8   instr_code[TB_SIM_APDU_CHEADER_NUM];    //Header of the C-TPDU
    uint8  *instr_data;         //Pointer to the data field of the C-TPDU
} TB_USIM_APDU_T;

/*
// message type from Driver to transport layer
typedef enum
{
    SIM_CARD_INSERT,                    //When inserting the SIM card into the ME
    SIM_DATA_RESPONSE,              //when the ME has received the response                                             //data from the SIM card.
    SIM_EXCEPTION_ERROR             //when SIM card exception error occurs.
}TB_SIM_SIG_TYPE_E;
*/


/*****************************************************************************/
//  SIM API
/*****************************************************************************/
/**********************************************************************
//    Description:
//      This is the interrupt porcess function that should be register by the transport layer
//    and this function should be called before HAL_InitSimDrv();
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
ISR_EXE_T SIM_InterruptHandler (uint32 lparam);
/**********************************************************************
//    Description:
//    Register the call-back function of the transport layer, create timers
//    used by SIM driver,
//    and set the parameters and the setting of the SIM Driver to the default.
//    The MS must call this function to initiate SIM driver when power on or
//    when the SIM Driver run wrong.
***********************************************************************/
uint32 HAL_InitSimDrv (int32 card_id, TB_CALLBACK handler);

/**********************************************************************
//    Description:
//    Reset the SIM card. The operations are: Power down, Power On and Start.
***********************************************************************/
void HAL_ResetSimCard (int32 card_id);

/**********************************************************************
//    Description:
//    Warm Reset the SIM card.
***********************************************************************/
void HAL_WarmResetSimCard (int32 card_id);

/**********************************************************************
//    Description:
//    Send an Instruction to the SIM card to start an operation.
***********************************************************************/
SIM_SEND_INSTR_RESULT_E HAL_SendSimInstrCode (int32 card_id,TB_USIM_APDU_T *apdu_ptr);

/**********************************************************************
//    Description:
//    PowerOff the SIM card and disable the SIM module
***********************************************************************/
void HAL_PowerOffSIM (void);

/**********************************************************************
//    Description:
//    Change the current.
***********************************************************************/
void HAL_ChangeCurrent (int32 card_id,uint8 current);




#ifdef   __cplusplus
}
#endif

#endif //_USIM_INTERFACE_H_

#endif //_GSM_IN_TDGSM_MODE
