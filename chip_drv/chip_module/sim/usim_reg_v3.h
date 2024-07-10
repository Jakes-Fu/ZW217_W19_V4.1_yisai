/******************************************************************************
 ** File Name:      usim_reg_v3.h                                                 *
 ** Author:         yongxia.zhang                                             *
 ** DATE:           04/28/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file definition USIM Registers address.            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/28/2004     yongxia.zhang    Create.                                   *
 ******************************************************************************/

#ifndef _USIM_REG_H_
#define _USIM_REG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif





/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/


//#define INTCTL_ICLR_ALL         0xFFFFFFFF

//The corresponding bit of UART_STS1 register.
#define RX_FIFO_CNT_MSK 0x000F
#define TX_FIFO_CNT_MSK 0x00F0

//-----------------------------------------------
//SIM Card Interface Register
//The corresponding bit of SIM_STS0 register.
#define SIMSTS0_B_RX_FIFO_FULL          (1 << 0)        //rx fifo data number bigger than rx_int_mark
#define SIMSTS0_B_TX_FIFO_EMPTY         (1 << 1)        //tx fifo data number bigger than tx_int_mark
#define SIMSTS0_B_RX_PARITY_ERR         (1 << 2)        //Int status bit for rx parity error
#define SIMSTS0_B_TX_PARITY_ERR         (1 << 3)        //Int status bit for tx parity error
#define SIMSTS0_B_UNRESP_CARD           (1 << 4)        //Int status bit for card unresponsive
#define SIMSTS0_B_CARD_IN               (1 << 5)        //Int status bit for card inserted
#define SIMSTS0_B_CARD_OUT              (1 << 6)        //Int status bit for card removed
#define SIMSTS0_B_EARLY_ATR             (1 << 7)        //Int status bit for early answer to reset
#define SIMSTS0_B_ACTIVE_DONE           (1 << 8)        //Int status bit to show activation is done

//The corresponding bit of SIM_STS1 register.
//Get the RX byte number in the rx fifo,rx_fifo_count--SIM_STS1[3:0]
#define GET_RX_FIFO_COUNT(sim_register_map)     ((sim_register_map)->sts1 & 0x001f)
//Get the TX byte number in the TX fifo,rx_fifo_count--SIM_STS1[8:5]
#define GET_TX_FIFO_COUNT(sim_register_map)     (((sim_register_map)->sts1 & 0x03e0) >> 5)
#define SIMSTS1_B_CARD_INSERTED         (1 << 10)        //Reflect of card in input pin
#define SIMSTS1_B_SIM_DATA              (1 << 11)        //Reflect of sim data io pin
#define SIMSTS1_B_SIM_ACTIVE_STS        (1 << 12)        //Activation status, 1: activated. 0: not activated.
#define SIMSTS1_B_ACTIVE_ON             (1 << 13)        //Busy in activation process
#define SIMSTS1_B_DEACTIVE              (1 << 14)        //Busy in deactivation process

//The corresponding bit of SIM_IE register.
#define SIMIE_B_RX_FULL                 (1 << 0)        //Enable bit for rx full int
#define SIMIE_B_TX_EMPTY                (1 << 1)        //Enable bit for tx empty int
#define SIMIE_B_RX_PARITY_ERR           (1 << 2)        //Enable bit for rx_parity_error
#define SIMIE_B_TX_PARITY_ERR           (1 << 3)        //Enable bit for tx_parity_error
#define SIMIE_B_UNRESP_CARD             (1 << 4)        //Enable bit for unresp_card
#define SIMIE_B_CARD_IN                 (1 << 5)        //Enable bit for card_in
#define SIMIE_B_CARD_OUT                (1 << 6)        //Enable bit for card_out
#define SIMIE_B_EARLY_ATR               (1 << 7)        //Enable bit for early answer to reset
#define SIMIE_B_ACTIVE_DONE             (1 << 8)        //Enable bit for active done

//The corresponding bit of SIM_ICLR register.
#define SIMICLR_B_RX_FULL               (1 << 0)        //Int clear bit for rx full int
#define SIMICLR_B_TX_EMPTY              (1 << 1)        //Int clear bit for tx empty int
#define SIMICLR_B_RX_PARITY_ERR         (1 << 2)        //Int clear bit for rx_parity_error
#define SIMICLR_B_TX_PARITY_ERR         (1 << 3)        //Int clear bit for tx_parity_error
#define SIMICLR_B_UNRESP_CARD           (1 << 4)        //Int clear bit for unresp_card
#define SIMICLR_B_CARD_IN               (1 << 5)        //Int clear bit for card_in
#define SIMICLR_B_CARD_OUT              (1 << 6)        //Int clear bit for card_out
#define SIMICLR_B_EARLY_ATR             (1 << 7)        //Int clear bit for early answer to reset
#define SIMICLR_B_ACTIVE_DONE           (1 << 8)        //Int clear bit bit for active done

//The corresponding bit of SIM_CTL0 register.
#define SIMCTL0_B_BIT_CONVENTION        (1 << 0)        //Bit Convention:0-MSB Transmitted first;1-LSM transmitted first                         1
#define SIMCTL0_B_LOGIC_LEVEL           (1 << 1)        //Logic Level:0-high logic level represet "0";1-high logic level represet "1"
#define SIMCTL0_B_EVEN_PARITY           (1 << 2)        //0:Even Parity;1:Odd parity
#define SIMCTL0_B_LOOPBACK_MODE         (1 << 3)        //Transmit data looped back to receive.
#define SIMCTL0_B_RX_FIFO_RST           (1 << 4)        //Reset the rx fifo 
#define SIMCTL0_B_TX_FIFO_RST           (1 << 5)        //Reset the tx fifo
#define SIMCTL0_B_SIM_RST               (1 << 6)        //Reset the sim card module 
#define SIMCTL0_B_POWER_EN              (1 << 7)        //Enable the Power supply to sim card 
#define SIMCTL0_B_CARD_OUT_LATCHED      (1 << 8)        //This bit will be 1 if card_out happened.
#define SIMCTL0_B_RX_EN                 (1 << 9)        //Enable the sim rx
#define SIMCTL0_B_TX_EN                 (1 << 10)       //Enable the sim tx
#define SIMCTL0_B_EN_ACTIVE             (1 << 11)       //Enactive the SIM card
#define SIMCTL0_B_DE_ACTIVE             (1 << 12)       //Deactive the SIM card
#define SIMCTL0_B_ACT_DEACT_EN          (1 << 13)       //Enable the active/deactive procedure
#define SIMCTL0_B_AUTO_ACT_DEACT        (1 << 14)       //Enable the auto start of active/deactive when card_in or card_out happen.
#define SIMCTL0_B_TX_DATA_OUT_LOW       (1 << 15)       //Force the tx data to low logic level.

#define SET_CLOCK_MODE(sim_register_map,clock_mode)     (((sim_register_map)->ctl1 & 0xf8ff) | (clock_mode << 8))
#define SIMCTL1_B_CLK_ENABLE            (1 << 11)       //Enable or Disable the SIM clock
#define SIMCTL1_B_CLK_POLARITY          (1 << 12)       //The logic Level when the SIM clock is Disabled

//the default register value of SIM register that used to initialize the
//sim register at the drive start,the value may be changed
#define SIM_DEFAULT_STS0                0x0
#define SIM_DEFAULT_STS1                0x0
#define SIM_DEFAULT_IE                  0x01fc
#define SIM_DEFAULT_ICLR                0x01ff
#define SIM_DEFAULT_CTL0                0x2003

/* +binli opt sim */
/* -binli opt sim */
#define SIM_13M_CK_DVD                  (372 * 4)       //default F = 372,D =1
/* +binli opt sim */
#define SIM_39M_CTL1                    0x0201
/* -binli opt sim */
#define SIM_39M_CK_DVD                  (372 * 8)       //default F = 372,D =1
#define SIM_78M_CTL1                    0x0300
#define SIM_78M_CK_DVD                  (372 * 16)      //default F = 372,D =1

#define SIM_DEFAULT_SHE                 0x00         //enable the rx and tx retransmit to handle the rx and tx parity error
#define SIM_ENABLED_SHE                 0x33        //the retransmit function enabled she register value
#undef  SIM_DEFAULT_TGC
#define SIM_DEFAULT_TGC                 0x1605
#undef  SIM_DEFAULT_WDT
#define SIM_DEFAULT_WDT                 0x0         //the watch dog default in the repeat mode
#define SIM_DEFAULT_IMASKED             0x0

//the sim card fifo buf size
#define SIM_FIFO_BUF_SIZE               16

#define GEN0_SIM                BIT_3   //SIM module enable bit

#ifdef __cplusplus
}
#endif

#endif /* _USIM_REG_H_*/
