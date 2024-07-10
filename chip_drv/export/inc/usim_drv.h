
/**************************************************************************

  Copyright (C) 2001 SPREADTRUM COMMUNICATION CO.
  All rights reserved.

  This document contains proprietary information blonging to
  SPREADTRUM. Passing on and copying this document, use and
  communication of its contents is not permitted without prior
  writed authorization.

***************************************************************************/

/***************************************************************************

  Reversion information:
  File Name:    sim_drv.h
  Version:      0.1
  Author:       Raislin.Kong
  Date:         2002.2
  Description:  This is the header file for the SIM driver
***************************************************************************/
/***************************************************************************

                Edit Histroy
  Date          Name            Description
  --------------------------------------------------------------------------
  2001.2        Raislin.Kong    Create
  2002.10       Raislin.Kong    change the SIM driver and move it under the new
                                driver framework
  2011.01       bin.li          MULTI card develop
***************************************************************************/
#ifndef _USIM_DRV_H_
#define _USIM_DRV_H_

#ifndef _GSM_IN_TDGSM_MODE

#ifdef   __cplusplus
extern   "C"
{
#endif
#include "tb_hal.h"
#include "usim_interface.h"

//chenbo for dt 20050310
#define CHANGE_FOR_DT

#ifdef CHANGE_FOR_DT
//add by chenbo do this for dt, change callback function
/********************************************************************
//    DESCRIPTION
//      set the callback function of usim_main
//    Global resource dependence :
//    Author:chenbo
//    Note:
***********************************************************************/
extern void UsimMain_SetCallBackFunc (
    TB_CALLBACK      handler
);
#endif

//---------------Macro define--------------------------
//#define SIM_ENHANCED_PPS_SUPPORT
#define SIM_SUPPORT_1D8V_3V

//the T = 0 protocol
#define TB_SIM_T_0_PROTOCOL                 0

#define TB_SIM_WARNNING_BYTE                0X62
#define TB_SIM_RESPONSE_GET_RESP_BYTE       0x61
#define TB_SIM_RESONSE_REP_CMD_BYTE         0x6c
#define TB_SIM_APDU_CHEADER_FIVE            0x05    //the 5 command header
#define TB_SIM_MAX_INF_BYTE_NUM             0xfe    //the inf byte should be 0-254 bytes
#define TB_SIM_MAX_RESENT_COUNT             0x02    //the max resent count when bwt time-out

//the interupt register buf len
/* +cr162983 */
#define TB_SIM_MAX_MSG_SIZE             1024        //the max Message len 256 + 5
/* -cr162983 */
#define TB_SIM_MAX_TX_BUF_SIZE          262     //the max Message len is (256+6)

//the times that the sim card can reset when it can not
//pass the ATR
#define TB_SIM_MAX_RESET_TIMES              3
#define TB_SIM_MAX_B_RESET_TIMES            3
#define TB_SIM_MAX_POWER_RESET_TIMES        3

//the ATR TA1 value that do not need the PPS procedure
#define TB_SIM_ATR_DEFAULT_A1_1             0x01
#define TB_SIM_ATR_DEFAULT_A1_2             0x11

//the always present ATR character byter number
#define TB_SIM_ATR_ALWAYS_CHARACTER_NUM     2
#define TB_SIM_ATR_HISTORY_CHARACTER_MARK   0x0f
#define TB_SIM_ATR_TA_CHARACTER_MARK        0x10
#define TB_SIM_ATR_TB_CHARACTER_MARK        0x20
#define TB_SIM_ATR_TC_CHARACTER_MARK        0x40
#define TB_SIM_ATR_TD_CHARACTER_MARK        0x80
#define TB_SIM_ATR_T_PROTOCAL_MARK          0x0f
#define TB_SIM_ATR_T0_POS                  1
#define TB_SIM_ATR_TS_VALUE1               0x3f
#define TB_SIM_ATR_TS_VALUE2               0x3b
#define TB_SIM_ATR_TS_VALUE3               0xfc //change bit convention of the 0x3f
#define TB_SIM_ATR_TS_VALUE4               0xdc //change bit convention of the 0x3b
#define TB_SIM_ATR_TS_VALUE5               0x03 //change bit convention & logic level of the 0x3f
#define TB_SIM_ATR_TS_VALUE6               0x23 //change bit convention & logic level of the 0x3b
#define TB_SIM_ATR_TS_VALUE7               0xc0 //change logic level of the 0x3f
#define TB_SIM_ATR_TS_VALUE8               0xc4 //change logic level of the 0x3b



//the default factor value for the SIM card
#define TB_SIM_DEFAULT_FACTOR_F            372
#define TB_SIM_DEFAULT_FACTOR_D            1
#define TB_SIM_DEFAULT_FACTOR_I            50
#define TB_SIM_DEFAULT_FACTOR_P            5
#define TB_SIM_DEFAULT_FACTOR_N            0
#define TB_SIM_DEFAULT_FACTOR_IFSC         32
#define TB_SIM_DEFAULT_FACTOR_BWI          4
#define TB_SIM_DEFAULT_FACTOR_CWI          13
#define TB_SIM_DEFAULT_FACTOR_EDC          0
#define TB_SIM_DEFAULT_FACTOR_X            0
#define TB_SIM_DEFAULT_FACTOR_U            1
#define TB_SIM_DEFAULT_FACTOR_BWT          1172     //F=372,D=1,BWI=4,fs=39M,Dvd=8
#define TB_SIM_RX_STATUS_WORD_NUM           2

#define SIM_ENHANCED_F                  512         //the F factor if the SIM support enhanced pps
#define SIM_ENHANCED_D                  8           //the D factor is the SIM support enhanced pps
#define SIM_ENHANCE_PPS_TA1_VALUE       0x94        //the TA1 value in the ATR indicate that the SIM is a enhanced one
#define UINT8_INVALIDE                  0xff
#define SIM_WAIT_LDO_TIMER_VALUE 150 // LDO timer
#define SIM_STATUS_WORD_TIMER_VALUE     300         //the value of the timer to judge the status works or SIM data in the rx expected length
#define SIM_WAIT_RESP_TIMER_VALUE       4000      //the value of the timer to wait for the response      /* CR218847 remove real nw flag  */
#define SIM_ATR_TIMER_VALUE             300         //the value of the timer to deicde if the there are any ATR data from SIM when reset the SIM
#define SIM_RESET_TIMER_VALUE           300         //the timer value to make sure the ATR data of the SIM will be all transmited between
//two continous SIM driver reset
#define SIM_BWT_TIMER_VALUE             1100        //the time of bwt time out
#define SIM_RX_TIMEOUT_VALUE            5           // the time of rx timeout
#define SIM_MAX_INSTR_REDO_TIMES        3           //the max redo times for the same instruction
#define SIM_DEFAULT_NAD                 0x0
#define SIM_DEFAULT_PCB_IBLOCK          0x0
#define SIM_DEFAULT_PCB_RBLOCK          0x80
#define SIM_DEFAULT_PCB_SBLOCK          0xc0
#define SIM_PCB_NS_ONE                  (1 << 7)

#define USIM_CONST_NUM0     0
#define USIM_CONST_NUM1     1
#define USIM_CONST_NUM2     2
#define USIM_CONST_NUM3     3
#define USIM_CONST_NUM4     4
#define USIM_CONST_NUM5     5
#define USIM_CONST_NUM6     6
#define USIM_CONST_NUM7     7

/* +cr168477 */
#ifndef WIN32
/*SIM API*/
#define HAL_InitSimDrv                  SIM_InitDriver
#define HAL_SendSimInstrCode            SIM_SendInstrCode
#define HAL_ResetSimCard                SIM_ResetSIMCard
#define HAL_WarmResetSimCard            SIM_WarmResetSimCard
#define HAL_PowerOffSIM             SIM_PowerOffSIM
#define HAL_ChangeCurrent               SIM_ChangeCurrent
#define HAL_SelMFAckClkVol              SIM_SelMFAckClkVol

#else

/*SIM API*/
#define SIM_InitDriver                  SIM_SimuInitDriver
#define SIM_SendInstrCode               SIM_SimuSendInstrCode
#define SIM_ResetSIMCard                SIM_SimuResetSIMCard
#define SIM_WarmResetSimCard            SIM_SimuWarmResetSimCard
#define SIM_PowerOffSIM                 SIM_SimuPowerOffSIM
#define SIM_ChangeCurrent               SIM_SimuChangeCurrent
#define SIM_SelMFAckClkVol              SIM_SimuSelMFAckClkVol

/*SIM API*/
#define HAL_InitSimDrv                  SIM_InitDriver
#define HAL_SendSimInstrCode            SIM_SendInstrCode
#define HAL_ResetSimCard                SIM_ResetSIMCard
#define HAL_WarmResetSimCard            SIM_WarmResetSimCard
#define HAL_PowerOffSIM                 SIM_PowerOffSIM
#define HAL_ChangeCurrent               SIM_ChangeCurrent
#define HAL_SelMFAckClkVol              SIM_SelMFAckClkVol

#define ReportCardStatus                SIM_SimuReportSIMInsertStatus
#define SetSIMPower                     SIM_SimuSetSIMPower

#endif
/* -cr168477 */

/* 20110210 binli add for dual  card */
#define USIMDRV_CHANGE_TIMER(card_id,timer_ptr,timer_fun,timer_expire) \
{\
    if (USIMDRV_CARD_ONE == card_id)\
    {\
        SCI_ChangeTimer(timer_ptr, Sim1##timer_fun, timer_expire);\
    }\
    else\
    {\
        SCI_ChangeTimer(timer_ptr, Sim2##timer_fun, timer_expire);\
    }\
}

#define USIMDRV_CREATE_TIMER(card_id,timer_ptr,timer_name,timer_fun,input,timer_expire,auto_activate) \
{\
    if (USIMDRV_CARD_ONE == card_id)\
    {\
        timer_ptr = SCI_CreateTimer(timer_name,Sim1##timer_fun,input,timer_expire,auto_activate);\
    }\
    else\
    {\
        timer_ptr = SCI_CreateTimer(timer_name,Sim2##timer_fun,input,timer_expire,auto_activate);\
    }\
}
#define TB_SIM_BLOCK_PRO_BYTE_NUM 3

#define GET_BIT_VALUE(byte,BIT_NUM)     ((byte >> BIT_NUM) & 0x01)

#define TB_SIM_APDU_CHEADER_FOUR_T1_CASE1  0x04/*when t_protocol=1, case1's header len is 4.*/
/* +multi card develop bin */
typedef enum
{
    USIMDRV_CARD_ONE,
    USIMDRV_CARD_TWO,
    USIMDRV_CARD_NUM
}
USIMDRV_CARD_ID_E;
/* -multi card develop bin */
//--------------structure define--------------------------
//the SIM clock mode


typedef enum
{
    TB_SIM_BLOCK_PRO_NAD,
    TB_SIM_BLOCK_PRO_PCB,
    TB_SIM_BLOCK_PRO_LEN,
    TB_SIM_BLOCK_EPI_EDC,
    TB_SIM_BLOCK_EXT_BYTE_NUM
} TB_SIM_BLOCK_PRO_BYTE_E;

/* +interrupt_refactor_dev */
#define SIM_INPUT_BUFFER_INIT(_MAX_SIZE)\
    static TB_SIM_DATA_BUF_T input_buffer[USIMDRV_CARD_NUM][_MAX_SIZE]; \
    static TB_SIM_DATA_BUF_T*  input_write_ptr[USIMDRV_CARD_NUM]  =\
           {&input_buffer[USIMDRV_CARD_ONE][0],&input_buffer[USIMDRV_CARD_TWO][0]} ;\
    static TB_SIM_DATA_BUF_T*  input_read_ptr[USIMDRV_CARD_NUM]  =\
           {&input_buffer[USIMDRV_CARD_ONE][0],&input_buffer[USIMDRV_CARD_TWO][0]} ;\
    static int MaxSize[USIMDRV_CARD_NUM] = {_MAX_SIZE, _MAX_SIZE};

#define SIM_ISR_WRITE_BUFFER_DEF\
    LOCAL void SimIsrWriteBuffer(TB_SIM_DATA_BUF_T value) \
    { \
        TB_SIM_DATA_BUF_T *save_ptr = input_write_ptr[value.card_id]; \
        SCI_ASSERT(value.card_id < USIMDRV_CARD_NUM);\
        *input_write_ptr[value.card_id]++ = value; \
        if (input_write_ptr[value.card_id] > &input_buffer[value.card_id][MaxSize[value.card_id]-1]) \
            input_write_ptr[value.card_id] = &input_buffer[value.card_id][0]; \
        if (input_write_ptr[value.card_id] == input_read_ptr[value.card_id]) \
            input_write_ptr[value.card_id] = save_ptr; \
    }
#define SIM_THREAD_READ_BUFFER_DEF \
    LOCAL BOOLEAN SimthreadReadBuffer(TB_SIM_DATA_BUF_T* value)\
    {\
        SCI_ASSERT(value->card_id < USIMDRV_CARD_NUM);\
        if (input_read_ptr[value->card_id] != input_write_ptr[value->card_id])\
        { \
            *value = *input_read_ptr[value->card_id]++; \
            if (input_read_ptr[value->card_id] > &input_buffer[value->card_id][MaxSize[value->card_id]-1])\
                input_read_ptr[value->card_id] = &input_buffer[value->card_id][0];\
            return FALSE;\
        } \
        return TRUE;\
   }
/* -interrupt_refactor_dev */

//SIM driver relative define
#define TB_SIM_MAX_HBYTE_SIZE   15              //the max history byte len
#define TRACE_CARD_ID           card_id     //the card id for TRACE

typedef struct  simAtrTag
{
    uint16      t_protocol; //The current protocol used by USIM card
    uint16      F;          //Transmission Factor F
    uint8       D;          // Transmission Factor D
    uint8       I;          // Program Current I
    uint8       P;          // Program Voltage P
    uint8       N;          // Protect time out of 8bits
    uint8       X;          // Indicate whether USIM supports clock stop mode or not,
    //if support, indicate which level type of CLK shows stop.
    uint8       U;          // Indicate which work condition supported by USIM
    uint8       IFSC;       // The Maximum INF length supported by USIM
    uint8       CWI;        // Used to calculate the Maximum time interval between
    // the two neighboring byte's lead edges
    uint8       BWI;        // Used to calculate the maximum time interval between
    // the lead edge of the first block sent by USIM and that of
    // the last block sent by the interface
    uint8       EDC;        //The error detection code used by USIM
    uint8       his_byte_len;   //The real length of the history byte
    uint8       history_byte[TB_SIM_MAX_HBYTE_SIZE];//The history byte received in the ATR process
} TB_SIM_ATR_DATA_T;


//the buf that used in the ATR
typedef struct
{
    uint8   Tai_pos;
    uint8   Tbi_pos;
    uint8   Tci_pos;
    uint8   Tdi_pos;
    uint8   td_count;
    uint8   hb_count;
    BOOLEAN first_occur_protocol;
    BOOLEAN is_support_t1;
    BOOLEAN is_t15_exist;
    BOOLEAN is_atr_data_end;
    BOOLEAN hisbyte_is_end;
    BOOLEAN first_ta1_after_t1;
    BOOLEAN first_tb1_after_t1;
    BOOLEAN first_tc1_after_t1;
    BOOLEAN first_ta1_after_t15;
    BOOLEAN is_ta2_exist;
    BOOLEAN is_x_u_present;
    uint8   ta2_byte;
    uint32  BWT;
    TB_SIM_ATR_DATA_T atr_data;
} TB_SIM_ATR_DATA_BUF_T;

typedef enum
{
    TB_SIM_BLOCK_I_BLOCK,
    TB_SIM_BLOCK_R_BLOCK,
    TB_SIM_BLOCK_S_BLOCK,
    TB_SIM_BLOCK_NOT_DEF
} TB_SIM_BLOCK_TYPE_E;

typedef enum
{
    BLOCK_TYPE_I_ZERO,
    BLOCK_TYPE_I_ONE,
    BLOCK_TYPE_R_OK,
    BLOCK_TYPE_R_ERROR,
    BLOCK_TYPE_S_REQ,
    BLOCK_TYPE_S_RES,
    BLOCK_TYPE_NOT_DEF,
    BLOCK_TYPE_TOTAL_NUM
} BLOCK_TYPE_E;

typedef enum
{
    BLOCK_VALID,            //receive all the data,now can check the receive buffer
    BLOCK_INVALID,          //block is invalid
    BLOCK_SEND_NEXT,        //now can send the next block
    BLOCK_SEND_R,           //need to send an r-block
    BLOCK_SEND_SREP,        //need to send an s(...response)
    BLOCK_RESEND            //need to resend the last i-block
} BLOCK_VALIDITY_E;

typedef enum
{
    TB_SIM_BLOCK_FIELD_VALID_NAD = 0x01,
    TB_SIM_BLOCK_FIELD_VALID_PCB = 0x02,
    TB_SIM_BLOCK_FIELD_VALID_LEN = 0x04,
    TB_SIM_BLOCK_FIELD_VALID_EDC = 0x08
} TB_SIM_BLOCK_FIELD_VALID_E;

typedef struct
{
    uint8   ns;
    BOOLEAN chain_M;
} TB_SIM_I_BLOCK_T;

typedef enum
{
    TB_SIM_R_BLOCK_ERROR_FREE,
    TB_SIM_R_BLOCK_EDC_PARITY_ERROR,
    TB_SIM_R_BLOCK_OTHER_ERROR,
    TB_SIM_R_BLOCK_RFU
} TB_SIM_R_BLOCK_TYPE_E;

typedef struct
{
    uint8   nr;
    TB_SIM_R_BLOCK_TYPE_E block_type;
} TB_SIM_R_BLOCK_T;


typedef enum
{
    TB_SIM_S_BLOCK_RESYN_REQ = 0x0,
    TB_SIM_S_BLOCK_INFO_FIELD_REQ = 0x1,
    TB_SIM_S_BLOCK_ABORTION_REQ = 0x2,
    TB_SIM_S_BLOCK_EXT_BWT_REQ = 0x3,
    TB_SIM_S_BLOCK_RESYN_RSP = 0x20,
    TB_SIM_S_BLOCK_INFO_FIELD_RSP = 0x21,
    TB_SIM_S_BLOCK_ABORTION_RSP = 0x22,
    TB_SIM_S_BLOCK_EXT_BWT_RSP = 0x23,
    TB_SIM_S_BLOCK_ERROR_VPP = 0x24,
    TB_SIM_S_BLOCK_RFU = 0x25
} TB_SIM_S_BLOCK_TYPE_E;


#if 0
typedef enum
{
    TB_SIM_S_BLOCK_RESYN,
    TB_SIM_S_BLOCK_INFO_FIELD,
    TB_SIM_S_BLOCK_ABORTION,
    TB_SIM_S_BLOCK_EXT_BWT,
    TB_SIM_S_BLOCK_ERROR_VPP,
    TB_SIM_S_BLOCK_RFU
} TB_SIM_S_BLOCK_TYPE_E;
#endif

typedef enum
{
    SEND_RBLOCK_FOR_IBLOCK_ERROR, //when an I-Block has been sent and BWT or invalid block received
    SEND_RBLOCK_FOR_RBLOCK_ERROR, //when an R-Block has been sent and BWT or invalid block received
    SEND_RBLOCK_FOR_SBLOCK_ERROR, //when an S(...response) has been sent and BWT or invalid block received
    SEND_RBLOCK_FOR_IBLOCK_ACK,   //when received an I(Ns(S),1)
    SEND_RBLOCK_REASON_NUM
} SEND_RBLOCK_REASON_E;

/*  ref to 7816-3 P19 figure 17 */
typedef enum
{
    RBLOCK_ERROR_CODE_ERROR_FREE,   /*  0x0 */
    RBLOCK_ERROR_CODE_PARITY_ERROR, /*  0x1 */
    RBLOCK_ERROR_CODE_OTHER_ERRORS, /*  0x2 */
    RBLOCK_ERROR_CODE_NUM
} RBLOCK_ERROR_CODE_E;

/*  the error code for errorhandler */
typedef enum
{
    T1_ERROR_CODE_NONE,
    T1_ERROR_CODE_EDC_ERROR,
    T1_ERROR_CODE_NAD_ERROR,
    T1_ERROR_CODE_LEN_ERROR,
    T1_ERROR_CODE_PCB_ERROR,
    T1_ERROR_CODE_WRONG_INSTR,
    T1_ERROR_CODE_BWT_TIMER_OUT,
    T1_ERROR_CODE_NUM
} T1_ERROR_CODE_E;


typedef enum
{
    SEND_SBLOCK_FOR_SBLOCK_REQUEST, //When want to sent an S(...request)
    SEND_SBLOCK_FOR_SBLOCK_ERROR,   //When an S(...request) has been sent and either
    //a BWT time-out occurs or the received response
    // is not an S(...response),S(...request) should be resent.
    SEND_SBLOCK_FOR_SBLOCK_RESPONSE, //when received an S(...request),an S(...response) should be resent.
    SEND_SBLOCK_REASON_NUM
} SEND_SBLOCK_REASON_E;

typedef struct
{
    BOOLEAN             is_response;
    TB_SIM_S_BLOCK_TYPE_E block_type;
} TB_SIM_S_BLOCK_T;

typedef struct
{
    TB_SIM_BLOCK_TYPE_E block_type;     //block type
    TB_SIM_I_BLOCK_T        i_block;    //structure of the i-block
    TB_SIM_R_BLOCK_T        r_block;    //structure of the r-block
    TB_SIM_S_BLOCK_T        s_block;    //structure of the s-block
    TB_SIM_BLOCK_FIELD_VALID_E   valid_block;    // if this i-block is valid
} TB_SIM_BASE_BLOCK_INFO_T;


typedef struct
{
    TB_SIM_BASE_BLOCK_INFO_T    base_info;  //base info of the block
    /*  some abnormal case we will receive s-block with length 8
        and this make overwrite */
    uint8       ext_byte[10];               //the length of prologue,epilogue byte of the block
    //and the info data length of the s-block
    uint8       cur_ext_byte_count;         //the current number of ext byte
    uint8       cur_info_data_count;
    uint16      info_data_len;          //data len in the info field
    BOOLEAN     have_edc;               //whether the current block have received its edc byte
} TB_SIM_RX_BLOCK_T;

typedef struct
{
    TB_SIM_BASE_BLOCK_INFO_T    base_info;  //base info of the block
    uint8       ext_byte[5];                //the length of prologue,epilogue byte of the block
    //and the info data length of the s-block
    uint8       cur_ext_byte_count;         //the current number of ext byte
    uint16      block_begin_pos;        //the begin pos of the current block in the tx buf
    uint16      block_end_pos;          //the end pos of the current block in the tx buf
} TB_SIM_TX_BLOCK_T;

//the buf that used in the RX
typedef struct
{
    BOOLEAN     is_locked;              //wheather the rx buf is locked.when the rx buf
    //is locked,can not put data into rx buf anymore
    uint16      rx_expected_len;        //expected to receive data's len
    uint16      cur_pos;                //the cur pos that will be process
    uint16      check_pos;
    uint8       buf[TB_SIM_MAX_MSG_SIZE];   //the buf used to save rx data
} TB_SIM_RX_BUF_T;
/* +interrupt_refactor_dev */
typedef struct
{
    int32 card_id; /* dual card dev */
    uint32      int_type;
    uint16      data_len;        //expected to receive data's len
    uint8        data[16];   //the buf used to save rx data
} TB_SIM_DATA_BUF_T;
/* -interrupt_refactor_dev */
//the buf that used in the TX
typedef struct SIM_TX_BUF_T
{
    uint16      tx_data_len;            //the tx data len need to send to the sim card
    uint16      cur_pos;                //the cur send data pos
    uint8       buf[TB_SIM_MAX_TX_BUF_SIZE];    //the buf that to save the tx data
} TB_SIM_TX_BUF_T;

typedef struct SIM_RX_BLOCK_BUF_T
{
    uint16      cur_pos;                //the cur data pos received
    uint8       buf[TB_SIM_MAX_TX_BUF_SIZE];    //the buf that to save the rx block data
} TB_SIM_RX_BLOCK_BUF_T;


//the sim card command status that indicate what the card is doing now
typedef enum sim_command_status_tag
{
    TB_SIM_NO_CARD,
    TB_SIM_NOT_ACTIVED,
    TB_SIM_WAITING_FOR_ATR,
    TB_SIM_PROCESSING_ATR,
    TB_SIM_PROCESSING_PPS,
    TB_SIM_RECEIVING_RESPONSE,
    TB_SIM_ERROR                    //a error status that the drive will not do any thing
} TB_SIM_STATUS_E;

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

#ifdef USIM_DRIVER_TEST
#define USIM_DEBUG_BUF_SIZE 256
//the test buf use to reacord down some data to used in debug interrupt
typedef struct
{
    uint32  cur_pos;
    uint8   buf[USIM_DEBUG_BUF_SIZE];
} TB_USIM_TEST_BUF_T;
#endif



/*SIM card reset status*/
typedef enum
{
    TB_SIM_RESETTED_OK,
#ifdef SIM_SIMULATOR
    TB_SIM_RESETTED_NO_SIM,
#endif
    TB_SIM_RESETTED_WRONG_SIM,
    TB_SIM_RESETTED_ERROR
} TB_SIM_RESET_STATUS_E;


/*modify in order to have identical value with clock stop indicator X in ATR.*/
typedef enum
{
    SIM_NO_CLOCK_STOP = 0, /*X=00(bit8 and bit7), Not supported*/
    SIM_CLOCK_STOP_LOW, /*X=01, State L   */
    SIM_CLOCK_STOP_HIGH, /*X=10, State H    */
    SIM_CLOCK_STOP_ANY, /*X=11, Clock stop allowed and no preferred level*/
    SIM_CLOCK_UNKNOWN
} sim_clock_phase_enum;

typedef enum
{
    SIM_VOLTAGE_5V = 1,
    SIM_VOLTAGE_3V,
    SIM_VOLTAGE_1_8_V,
    SIM_VOLTAGE_5V_3V, /*class AB*/
    SIM_VOLTAGE_3V_1_8_V,/*class BC */
    SIM_VOLTAGE_5V_3V_1_8_V,/* class ABC */
    SIM_VOLTAGE_UNKNOWN
} sim_supply_voltage_enum;

/*the card reseted information passed to the transport layer*/
typedef struct
{
    TB_SIM_WORKING_VOLT_E   working_volt;    /*The current supply voltage*/
    TB_SIM_RESET_STATUS_E    reset_status;   /*Card insert status*/
    int32 card_id;                          /* dual card dev */
} TB_SIM_CARD_RESET_T;

/* +cr168477 */
#ifndef WIN32
//golbal functions that defined in the usim_drv.c
/**********************************************************************
//    Description:
//      Initialize the Driver timer of the idle
//    Global resource dependence :
//    Arthor:Raislin.kong
//    Note:
***********************************************************************/
uint32 SIM_InitDriver (int32 card_id, TB_CALLBACK handler);

/**********************************************************************
//    Description:
//      Reset the SIM card without change the working voltage for
//    the SIM
//    Global resource dependence :
//    Arthor:Raislin.kong
//    Note:
***********************************************************************/
void SIM_ResetSIMCard (int32 card_id);

/**********************************************************************
//    Description:
//    Warm Reset the SIM card without change the working voltage for
//    the SIM
//    Global resource dependence :
//    Arthor:Raislin.kong
//    Note:
***********************************************************************/
void SIM_WarmResetSimCard (int32 card_id);

/**********************************************************************
//    Description:
//      Send a Instruction to the SIM card to start an operation on
//    The SIM card we needed
//    Global resource dependence :
//    Arthor:Raislin.kong
//    Note:
***********************************************************************/
SIM_SEND_INSTR_RESULT_E SIM_SendInstrCode (int32 card_id,TB_USIM_APDU_T *apdu_ptr);

/**********************************************************************
//    Description:
//      PowerOff the SIM card and disable the SIM module
//    Global resource dependence :
//    Arthor:Raislin.kong
//    Note:
***********************************************************************/
void SIM_PowerOffSIMEx(int32 card_id);
/**********************************************************************
//    Description:
//      PowerOff the SIM card and disable the SIM module
//    Global resource dependence :
//    Arthor:Raislin.kong
//    Note:
***********************************************************************/
void SIM_PowerOffSIM(void);

/**********************************************************************
//    Description:
//      Change the current of the SIM card
//    Global resource dependence :
//    Arthor:Raislin.kong
//    Note:
***********************************************************************/
void SIM_ChangeCurrent (int32 card_id,uint8 current);

/**********************************************************************
    Description:
       receive clock phase and supply voltage informations which are gotten from
           response FCP data of selecting MF, and do nothing for now but just the check
           whether it is the same or different as the one from ATR and print something.
    Global resource dependence :
    Author:amy.wang
    Note:
***********************************************************************/
void SIM_SelMFAckClkVol (int32 card_id,
                         sim_supply_voltage_enum  supply_voltage,
                         sim_clock_phase_enum     clk_phase,
                         BOOLEAN *is_need_reset);
#else
/**********************************************************************
//    Description:
//      Initialize the Driver timer of the idle
//    Global resource dependence :
//    Arthor:maddie.lai
//    Note:
***********************************************************************/
uint32 SIM_SimuInitDriver (int32 card_id, TB_CALLBACK handler);

/**********************************************************************
//    Description:
//      Reset the SIM card without change the working voltage for
//    the SIM
//    Global resource dependence :
//    Arthor:maddie.lai
//    Note:
***********************************************************************/
void SIM_SimuResetSIMCard (int32 card_id);

/**********************************************************************
//    Description:
//    Warm Reset the SIM card without change the working voltage for
//    the SIM
//    Global resource dependence :
//    Arthor:maddie.lai
//    Note:
***********************************************************************/
void SIM_SimuWarmResetSimCard (int32 card_id);

/**********************************************************************
//    Description:
//      Send a Instruction to the SIM card to start an operation on
//    The SIM card we needed
//    Global resource dependence :
//    Arthor:maddie.lai
//    Note:
***********************************************************************/
SIM_SEND_INSTR_RESULT_E SIM_SimuSendInstrCode (int32 card_id, TB_USIM_APDU_T *apdu_ptr);

/**********************************************************************
//    Description:
//      PowerOff the SIM card and disable the SIM module
//    Global resource dependence :
//    Arthor:maddie.lai
//    Note:
***********************************************************************/
void SIM_SimuPowerOffSIM (int32 card_id);
/**********************************************************************
//    Description:
//      Change the current of the SIM card
//    Global resource dependence :
//    Arthor:maddie.lai
//    Note:
***********************************************************************/
void SIM_SimuChangeCurrent (int32 card_id, uint8 current);

/**********************************************************************
    Description:
       receive clock phase and supply voltage informations which are gotten from
           response FCP data of selecting MF, and do nothing for now but just the check
           whether it is the same or different as the one from ATR and print something.
    Global resource dependence :
    Author:maddie.lai
    Note:
***********************************************************************/
void SIM_SimuSelMFAckClkVol (int32 card_id,
                             sim_supply_voltage_enum  supply_voltage,
                             sim_clock_phase_enum     clk_phase,
                             BOOLEAN *is_need_reset);


/*********************************************************************
    Description:
      report the SIM card reset status to the SIM task
    Global resource dependence :
    Author:maddie.lai
    Note:
***********************************************************************/
void SIM_SimuReportSIMInsertStatus (
    int32 card_id,
    TB_SIM_INSERT_STATUS_E insert_status   //the SIM card insert status
);

/**********************************************************************
//    Description:
//      Set the working voltage of the sim card.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_SimuSetSIMPower (int32 card_id, TB_SIM_WORKING_VOLT_E work_vol);


#endif
/* -cr168477 */
/* +CR218847 remove real nw flag */
/**********************************************************************
//    Description:
//
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
uint8 SIM_GetUiccTprotocol (
    int32 card_id
);
/* -CR218847 remove real nw flag */
#ifdef   __cplusplus
}
#endif

#endif //_GSM_IN_TDGSM_MODE
#endif //_USIM_DRV_H_

