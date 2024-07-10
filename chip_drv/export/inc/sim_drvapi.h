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
  2008.4.7  wuding.yang  divide the chip_drv and the ISO7816-3 protocol into 2 parts
***************************************************************************/
#ifndef _SIM_DRVAPI_H_
#define _SIM_DRVAPI_H_

#ifdef   __cplusplus
extern   "C"
{
#endif

#define TB_SIM_DEFAULT_FACTOR_F            372
#define TB_SIM_DEFAULT_FACTOR_D            1

typedef enum
{
    SIMDRV_CARD_ONE,
    SIMDRV_CARD_TWO,
    SIMDRV_CARD_NUM
}
SIMDRV_CARD_ID_E;

//the type of POWER that can power on the SIM card.
typedef enum
{
    SIMDRV_WORKING_VOLT_1_8V,
    SIMDRV_WORKING_VOLT_3V,
    SIMDRV_WORKING_VOLT_5V
} SIMDRV_WORKING_VOLT_E;

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
} SIMDRV_CLK_MODE_E;

//the SIM interrupt type that may come form ARM
typedef enum
{
    TB_SIMINT_TX_EMPTY,
    TB_SIMINT_RX_FULL,
    TB_SIMINT_RX_PARITY_ERROR,
    TB_SIMINT_TX_PARITY_ERROR,
    TB_SIMINT_CARD_UNRESPONSE,
    TB_SIMINT_CARD_IN,
    TB_SIMINT_CARD_OUT,
    TB_SIMINT_EARLY_ATR,
    TB_SIMINT_ACTIVE_DONE,
    TB_SIMINT_NEED_WAITTING,         //cr7095
    TB_SIMINT_PLUG_IN,
    TB_SIMINT_PULL_OUT
} TB_SIM_INT_TYPE_E;

typedef struct
{
    TB_SIM_INT_TYPE_E int_type;
} TB_SIM_INT_T;

//the signal type
typedef enum
{
    SIM_CARD_1_8V_DETECT,
    SIM_CARD_INSERT,
    SIM_DATA_RESPONSE,
    SIM_EXCEPTION_INT,
    SIM_CARD_NEED_WAITING ,   //cr7095,add
    SIM_CARD_RESET
} TB_SIM_SIG_TYPE_E;

//golbal functions that defined in the sim_driver.c
/**********************************************************************
//    Description:
//      Initialize the Driver timer of the idle
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint32 SIMDRV_InitDriver (uint8 card_id, TB_CALLBACK handler);

/**********************************************************************
//    Description:
//      Enable SIM Tx Empty Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_EnableSIMTXEmptyInt (uint8 card_id);

/**********************************************************************
//    Description:
//      Disable SIM Tx Empty Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIMTXEmptyInt (uint8 card_id);

/**********************************************************************
//    Description:
//      Enable SIM Rx Full and Time Out Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_EnableSIMRXInt (uint8 card_id);

/**********************************************************************
//    Description:
//      Disable SIM Rx Full and Time Out Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIMRXInt (uint8 card_id);

/**********************************************************************
//    Description:
//      Disable SIM Tx Parity Err Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIM_TxParityInt (uint8 card_id);

/**********************************************************************
//    Description:
//      Disable All the SIM Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIMAllInt (uint8 card_id);

/**********************************************************************
//    Description:
//      Enable the SIM clock
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_EnableSIMCLK (uint8 card_id);

/**********************************************************************
//    Description:
//      Disable the SIM clock
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIMCLK (uint8 card_id);

/**********************************************************************
//    Description:
//      Check wether the SIM clock is enabled
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDRV_IsSIMCLKEnabled (uint8 card_id);

/**********************************************************************
//    Description:
//      Send a data to SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SendData (uint8 card_id, uint32 data);

/**********************************************************************
//    Description:
//      Receive a data from SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMDRV_RecvData (uint8 card_id);

/**********************************************************************
//    Description:
//      Enable SIM Tx
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_EnableTx (uint8 card_id);

/**********************************************************************
//    Description:
//      Disable SIM Tx
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableTx (uint8 card_id);

/**********************************************************************
//    Description:
//      Reset the chip's SIM Tx FIFO
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_ResetTxFifo (uint8 card_id);

/**********************************************************************
//    Description:
//      Reset the chip's SIM Rx FIFO
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_ResetRxFifo (uint8 card_id);

/**********************************************************************
//    Description:
//      Set the Rx water mark. When Baseband chip get the num of data from sim card,
//      it will occour a SIM Rx Full Int.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetRxWaterMark (uint8 card_id, uint8 water_mark);

/**********************************************************************
//    Description:
//      Enable SIM RX
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDRV_EnableRx (uint8 card_id);

/**********************************************************************
//    Description:
//      Disable SIM RX
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableRx (uint8 card_id);

/**********************************************************************
//    Description:
//      Get the num of data in the SIM Tx FIFO.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint8 SIMDRV_GetTxFifoCount (uint8 card_id);

/**********************************************************************
//    Description:
//      Get the num of data in the SIM Rx FIFO.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint8 SIMDRV_GetRxFifoCount (uint8 card_id);

/**********************************************************************
//    Description:
//      Disable the Baseband chip's retransmit function.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableRetransmit (uint8 card_id);

/**********************************************************************
//    Description:
//      Set the SIM RST pin
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetSIMRST (uint8 card_id, BOOLEAN value);

/**********************************************************************
//    Description:
//      To get wether the SIM RST pin high?
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDRV_IsSIMRSTHigh (uint8 card_id);

/**********************************************************************
//    Description:
//      Set SIM active
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetSIMActive (uint8 card_id);

/**********************************************************************
//    Description:
//      Set SIM deactive
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetSIMDeactive (uint8 card_id);

/**********************************************************************
//    Description:
//      change the convention and logic level of baseband chip
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_ChangeConvetion (uint8 card_id);

/**********************************************************************
//    Description:
//      change the parity of baseband chip
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_ChangeParity (uint8 card_id);

/**********************************************************************
//    Description:
//      Set the SIM clock's divide factor
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetSIMClkDivisor (uint8 card_id, uint32 clk_divisor);

/**********************************************************************
//    Description:
//      config the SIM clock
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIMDRV_CLK_MODE_E SIMDRV_ConfigSIMClk (uint8 card_id);

/**********************************************************************
//    Description:
//      Set the logic level of SIM CLK pin when it stoped
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetSIMClkStopLevel (uint8 card_id, BOOLEAN bHighLevel);

/**********************************************************************
//    Description:
//      Set the working voltage of the sim card.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDRV_SetSIMVoltage (uint8 card_id, SIMDRV_WORKING_VOLT_E work_vol);

/**********************************************************************
//    Description:
//      .get the ctl0 register's value
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMDRV_GetCTL0Value (uint8 card_id);

/**********************************************************************
//    Description:
//       set the ctl0 register's value
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetCTL0Value (uint8 card_id, uint32 data);

/**********************************************************************
//    Description:
//      .get the FIFO size
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint16 SIMDRV_GetFIFOBufSize (uint8 card_id);

/**********************************************************************
//    Description:
//      .To get wether the SIM card is outlatched
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDRV_IsCardOutLatched (uint8 card_id);

/**********************************************************************
//    Description:
//      .clear some regs before sim reset
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_ClearBeforeReset (uint8 card_id);

/**********************************************************************
//    Description:
//      .begin to reset sim card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_StartToReset (uint8 card_id);

/**********************************************************************
//    Description:
//      .clear some regs before send a command to SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMDRV_BeforeSendData (uint8 card_id);

/**********************************************************************
//    Description:
//      Get the SIM LDO voltage stable time in ms.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMDRV_GetLDOStableTime (void);

/**********************************************************************
//    Description:
//      When Vsim stable, init the sim regs.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIMDRV_CLK_MODE_E SIMDRV_InitAfterLDOValid (uint8 card_id);

/**********************************************************************
//    Description:
//      power off the SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_Poweroff (uint8 card_id);

/**********************************************************************
//    Description:
//      Reject the SIM card(deactive the SIM card and Power off the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDRV_RejectSIMCard (uint8 card_id);

/**********************************************************************
//    Description:
//      Have received all the data from sim card, so clear some regs about Rx.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_CompleteRX (uint8 card_id);

#ifndef  WIN32
/**********************************************************************
//    Description:
//      SIM DMA channel init
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_Init (uint8 card_id,
                      DMA_HAL_ISR_HANDLER sim_tx_handler,
                      DMA_HAL_ISR_HANDLER sim_rx_handler,
                      DMA_CHNCFG_PTR sim_tx_dma_cfg,
                      DMA_CHNCFG_PTR sim_rx_dma_cfg);

/**********************************************************************
//    Description:
//      SIM DMA TX channel config
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_TX_Cfg (uint8 card_id, DMA_CHNCFG_PTR tx_dma_cfg);

/**********************************************************************
//    Description:
//      SIM DMA RX channel config
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_RX_Cfg (uint8 card_id, DMA_CHNCFG_PTR rx_dma_cfg);

#endif

/**********************************************************************
//    Description:
//      SIM DMA TX channel enable
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_TX_Enable (uint8 card_id);

/**********************************************************************
//    Description:
//      SIM DMA TX channel disable
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_TX_Disable (uint8 card_id);

/**********************************************************************
//    Description:
//      SIM DMA RX channel enable
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_RX_Enable (uint8 card_id);

/**********************************************************************
//    Description:
//      SIM DMA RX channel disable
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_RX_Disable (uint8 card_id);

/**********************************************************************
//    Description:
//      clear SIM DMA Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_ClearAllInt (uint8 card_id);

/**********************************************************************
//    Description:
//      Get the transfered bytes in one SIM DMA transfer.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMDRV_DMA_RX_GetTransferedBytes (uint8 card_id);

/**********************************************************************
//    Description:
//      SC6600L chip, SIM0 and SIM1 switch
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_L_Switch (uint8 card_id);

/**********************************************************************
//    Description:
//     Enable the SIM's LDO.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_EnableSIMLDO (uint8 card_id);

/**********************************************************************
//    Description:
//     Disable the SIM's LDO.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIMLDO (uint8 card_id);

/**********************************************************************
//    Description:
//     Set the SIM I/O pin's state.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_Set_IO_State (uint8 card_id, BOOLEAN state);

/**********************************************************************
//    Description:
//      trace the register value
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_PrintRegisterBuf (uint8 card_id);

/**********************************************************************
//    Description:
//      PowerOff all the SIM card and disable the SIM module
//    Global resource dependence :
//    Author:
//    Note:
***********************************************************************/
void SIM_PowerOffSIM (void);

/**********************************************************************
//    Description:
//      Get one data from sim_rx_int_buf[*].
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
uint8 SIMDRV_ReadIntRxBufData (uint8 card_id);

/**********************************************************************
//    Description:
//      Return data number of sim_rx_int_buf[*].
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
uint32 SIMDRV_GetRxIntBufCount (uint8 card_id);

/**********************************************************************
//    Description:
//      Reset sim_rx_int_buf[*].
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
void SIMDRV_ClearRxIntBuf (uint8 card_id);


#ifdef   __cplusplus
}
#endif

#endif //_SIM_DRVAPI_H_


