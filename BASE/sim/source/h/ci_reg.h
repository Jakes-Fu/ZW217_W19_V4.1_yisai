/******************************************************************************
 ** File Name:      sim_driver.h                                              *
 ** Author:         Raislin.Kong                                              *
 ** Date:           2002.2                                                    *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    The hardware relative define in for the new CI chip       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2002.2         Raislin.Kong     Create                                    *
 ******************************************************************************/

#ifndef _CI_REG_H_
#define _CI_REG_H_
//#define the system to pass compile
#ifdef SIM_SIMULATOR

#define  system

#define SIMCLC     *((volatile unsigned int system *)  (0x0EF40))    //the clock contorl register
#define SIMID      *((volatile unsigned int system *)  (0x0EF48))
#define SIMCTRL    *((volatile unsigned int system *)  (0x0EF50))   //the SIM card control rgister
#define SIMBRF     *((volatile unsigned int system *)  (0x0EF52))   //the data retransmit bit baudrate control register[6:0]
#define SIMSTATUS  *((volatile unsigned int system *)  (0x0EF54))   //the status control register  
#define SIMIRQEN   *((volatile unsigned int system *)  (0x0EF56))   //the IRQ Enable register 
#define SIMTX      *((volatile unsigned int system *)  (0x0EF58))   //the TX register
#define SIMRX      *((volatile unsigned int system *)  (0x0EF5a))   //the RX register
#define SIMINS     *((volatile unsigned int system *)  (0x0EF5c))   //the INS register used to   
#define SIMP3      *((volatile unsigned int system *)  (0x0EF5e))
#define SIMSW1     *((volatile unsigned int system *)  (0x0EF60))
#define SIMSW2     *((volatile unsigned int system *)  (0x0EF62))
#define SIMRXSPC   *((volatile unsigned int system *)  (0x0EF64))   //Space between the last Rx character and the first Tx charact
//of the succeeding command
#define SIMTXSPC   *((volatile unsigned int system *)  (0x0EF66))   //Spacing between TX characters

#endif

//the bit define for the SIM control register
#define SIMCTL_B_BIT_CONVENTION                         0x0001      //0:Send LSB first,'1' is logic high
//1:send MSB first,'1' is logic low
#define SIMCTL_B_T0_MODE                                0x0002      //0:normal character mode;
//1:T0 Instruction Module;
#define SIMCTL_B_IO_ENABLE                              0x0004      //0:Disable the IO of SIM 
//1:Enable the IO of SIM
#define SIMCTL_B_SIM_INTERFACE                          0x0008
#define SIMCTL_B_VOTL_POWER                             0x0010
#define SIMCTL_B_RESET                                  0x0020      //0:reset low locgic level;1:reset high logic level
#define SIMCTL_B_OFF_PARITY_ERR                         0x0040      //0:parity error on;1:parity error off
#define SIMCTL_B_OFF_DATA_RETRANSMIT                    0x0080      //0:retransmit enable;1:retransmit disable
#define SIMCTL_B_AP_DOWN                                0x0100      //???
#define SIMCTL_B_AUTO_DEACITVE                          0x0200
#define SIMCTL_B_ACTIVE_CLK                             0x0400
#define SIMCTL_B_LOW_FREQ_CLK                           0x0800      //0:set the clock to SIM at 1.08MHz(the low frequent clock)
//1:set the clock to SIM at 3.25MHz(the high frequent clock)
#define SIMCTL_B_HIGH_CLK_STOP_LEVEL                    0x1000      //0:clock stop at low logic level;1:clock stop at high logic level          
#define SIMCTL_B_URAT_CLK_ON                            0x2000      //0:urat and clock stop;1:urat and clock on

//the bit define for the SIM status register
#define SIMSTS_B_CHARACTER_OK                           0x0001
#define SIMSTS_B_PARITY_ERROR                           0x0002
#define SIMSTS_B_OVERRUN_ERROR                          0x0004
#define SIMSTS_B_T0_DATA_END                            0x0008
#define SIMSTS_B_SIM_DET                                0x0010      //???

//the bit define for the SIM IRQ Enable register
#define SIMIE_B_CHARACTER_OK                            0x0001
#define SIMIE_B_PARITY_ERROR                            0x0002
#define SIMIE_B_OVERRUN_ERROR                           0x0004
#define SIMIE_B_TO_DATA_END                             0x0008
#define SIMIE_B_SIM_DET                                 0x0010     //???


typedef enum
{
    SIM_CMD_IDLE,
    SIM_CMD_HEADER,
    SIM_TX_FIRST_BLOCK,
    SIM_TX_LAST_BLOCK,
    SIM_RX_FIRST_BLOCK,
    SIM_RX_LAST_BLOCK,
    SIM_CMD_RESULT,
    SIM_CMD_SUCCEED,
    SIM_CMD_FAILED
} SIM_CI_COMMAND_STATUS_E;
#endif  //_CI_REG_H_
