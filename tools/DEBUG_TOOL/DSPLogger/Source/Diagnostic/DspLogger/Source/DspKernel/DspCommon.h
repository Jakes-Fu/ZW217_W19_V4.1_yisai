/*============================================================================*
 *
 *                          Copyright (c) 2001 Spreadtrum
 *                              All Rights Reserved
 *
 *-----------------------------------------------------------------------------
 *
 * File name:
 *      DspCommon.h
 *
 * Description:
 *      This header file defines all the common data types and constants used
 *      to communicate with DSP.
 *
 * Note:
 *      DspKernel.dll and DspLogger.exe both use this file.
 *
 *============================================================================*/


/*============================================================================*
 *                          Edit History
 *-----------------------------------------------------------------------------
 * 2002-08-01   Jun Liu     Created
 *-----------------------------------------------------------------------------
   2002-11-13   Kai Liu     Add new definitions
 *============================================================================*/


#ifndef __DSPCOMMON_H_
#define __DSPCOMMON_H_


#include "Dsp_logel_def.h"

/*============================================================================*
 *                          Macroes
 *============================================================================*/

// Size of the DSP frame, transferred via the UART
#define DSP_FRM_SIZE    120  // bytes

//dsp command number
#define DSPCMD_NUM 12

//dsp register number
#define DSPREG_NUM 42


//Uart operation error code
#define DSP_UART_NOERROR        0   //success
#define DSP_UART_NOT_READY      1   //uart is not ready for communicate
#define DSP_UART_READ_ERROR     2   //error occured in read operation
#define DSP_UART_WRITE_ERROR    3   //error cccured in write operation
#define DSP_UART_BAD_FRAME      4   //not a correct frame

//command build error code
#define BUILDCMD_SUCCESS        0   //successfully build a command
#define BUILDCMD_BADCMD         1   //not a valid command
#define BUILDCMD_BADREG         2   //bad register name
#define BUILDCMD_BADPARA        3   //invalid parameter
#define BUILDCMD_TOOMANYPARA    4   //too many parameters
#define BUILDCMD_NEEDINFO       5   //need more parameters
#define BUILDCMD_BADADDR        6   //not a valid address
#define BUILDCMD_BADBLKSZ       7   //not a valid block size
#define BUILDCMD_DATATOOLONG    8   //given data is too long

//frame types
#define CMD_TYPE            0       //CMD frame type
#define CMD_RSP_TYPE        1       //CMD responds type
#define OTHER_INFO_TYPE     2       //key info and test point type

#define KEY_INFO_TYPE 1         //key information frame sub type
#define TEST_PT_TYPE  2         //test point logger frame sub type


//key information subtypes
#define KEY_INTER_TYPE 1       //interrupt key information
#define KEY_FG_TYPE    2       //forground key information
#define KEY_BG_TYPE    3       //background key informatin
#define KEY_FRM_TYPE   4       //frame instruction key information
#define KEY_MCU_TYPE   5       //MCU Command key information
#define KEY_RSSI_TYPE  6       //RSSI key information
#define KEY_FRE_TYPE   7       //frequency tracked information

//frame size
#define TEST_PT_SIZE        8       //test pt frame length
#define KEY_FG_MSG_SIZE     2       //forground key frame message size
#define KEY_BG_MSG_SIZE     2       //background key frame message size
#define KEY_FRM_MSG_SIZE    18      //frame instruction frame message size
#define KEY_MCU_MSG_SIZE    10      //MCU command frame message size
#define KEY_RSSI_MSG_SIZE   2       //RSSI frame message size
#define KEY_FRE_MSG_SIZE    2       //frequency tracked frame message size

//command building status
#define CMD_NEEDBODY    0
#define CMD_NEEDPARA1   1
#define CMD_NEEDPARA2   2
#define CMD_NEEDDATA    3
#define CMD_FINISHED    4

//used for registering gui message
#define GUI_MSG_NAME    "Notify Gui Message"

// Test point buffer size
#define TP_BLOCK_SIZE 256

// Test point item size
#define TP_ITEM_SIZE 4

// Test point data size,
// The first four byte is data received from 
// uart,the rest is receive time value
//#define TP_DATA_SIZE 8

/*============================================================================*
 *                          Enum types
 *============================================================================*/
// DSP register commands
typedef enum DSP_CMD_TYPE_TAG
{
    RRA,        // 0x00 : read_register_all
    RR,         // 0x01 : read_register
    RBD,        // 0x02 : //* @Liu Jun
    RBP,        // 0x03
    WR,         // 0x04
    WBD,        // 0x05
    WBP,        // 0x06
    CKS,        // 0x07
    GO_BOOT,    // 0x08
    RXINJ,      // 0x09
    TXINJ,      // 0x0A
    KEYE,       // 0x0B
	BADCOMMAND,
} DSP_CMD_TYPE;

// DSP registers
typedef enum DSP_REG_TYPE_TAG
{
    R0,         // 0X00
    R1,         // 0X01
    R2,         // 0X02
    R3,         // 0X03
    R4,         // 0X04
    R5,         // 0X05
    RB,         // 0X06
    LC,         // 0X07
    ST0,        // 0X08
    A0H,        // 0X09
    A0L,        // 0X0A
    ST1,        // 0X0B
    A1H,        // 0X0C
    A1L,        // 0X0D
    PH ,        // 0X0E
    PL ,        // 0X0F
    PC ,        // 0X10
    SP ,        // 0X11
    ST2,        // 0X12
    CFGI,       // 0X13
    CFGJ,       // 0X14
    ICR,        // 0X15
    X,          // 0X16
    Y,          // 0X17
    B0E,        // 0X18
    B0H,        // 0X19
    B0L,        // 0X1A
    B1E,        // 0X1B
    B1H,        // 0X1C
    B1L,        // 0X1D
    SV ,        // 0X1E
    DVM,        // 0X1F
    CFGIB,      // 0X20
    R0B ,       // 0X21
    R1B ,       // 0X22
    R4B ,       // 0X23
    MIXP,       // 0X24
    REPC,       // 0X25
    ST0S,       // 0X26
    ST1S,       // 0X27
    ST2S,       // 0X28
    PGSW,       // 0X29
    BADREG,
} DSP_REG_TYPE;   


/*============================================================================*
 *                          Structures
 *============================================================================*/
struct DspFrame
{
	BYTE frame[DSP_FRM_SIZE];
};

struct ParsedInfo
{
    //frame type;
    DWORD dwType;
    
    //frame subtype,reserved for future use
    DWORD dwSubType;

	//qbc time value
    DWORD dwTime;

	//key information task id
    DWORD dwTaskId;

    //key information message content
    //or command respond content
	BYTE pMsgContent[19];

	//test point address
    //or msg content byte count for key
    //information
	DWORD dwAddr;

    //test point data
    //or RSSI value
    //or Frequency tracked value
	DWORD dwData;

    //foreground task struct
    //or interrupt task struct
    fg_fun_task stFgTask;

    //background task struct
    bg_fun_task stBgTask;

    //MCU Command struct
    CmdQ_T stMcuCmd;

    //Frame instruction struct
    FrameInst_T stFrmInst;
};

#endif  // __DSPCOMMON_H_