/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/





#include "global_macros.h"
#include "sdmmc.h"
#include "sci_types.h"
#include "hal_sdmmc.h"
#include "chip_plf_export.h"
#include "ifc_drvapi.h"
#include "uws6121e_module_ver_extern.h"
#include "os_api.h"

#ifdef UART_DEBUG_POWER_ON

#ifdef SCI_ASSERT
#undef SCI_ASSERT
#define SCI_ASSERT(_EXP)  if((_EXP) == 0){SCI_TraceLow_Test("func:%s, line:%d", __func__, __LINE__);while(1);}
#endif

#ifdef SCI_PASSERT
#undef SCI_PASSERT
#define SCI_PASSERT(x, y)  if((x) == 0){SCI_TraceLow_Test("func:%s, line:%d", __func__, __LINE__);while(1);}
#endif

#else   //UART_DEBUG_POWER_ON

#ifdef SCI_ASSERT
#undef SCI_ASSERT
#define SCI_ASSERT(_EXP)   NULL
#endif

#endif //UART_DEBUG_POWER_ON


#define MCD_CARD_ID_0 0
//#define MCD_CARD_ID_1  1

#define MCD_CARD_ID_NO 1
 uint8 g_sdmmcId = MCD_CARD_ID_0;
HWP_SDMMC_T * g_hwpSdmmcCtrl;

uint32 hal_SysEnterCriticalSectionsdmmc(void)
{
    SCI_DisableIRQ();
    return 0;
}
void hal_SysExitCriticalSectionsdmmc(uint32 status)
{
    SCI_RestoreIRQ();
}



void sdmmc_event(uint32 e)
{
   //mon_Event(e);
}

void hal_McdDelay (int ms_delay)
{
    uint32 wait_time = 0;
    uint32 wait_time2 = 0;

    wait_time2 = ms_delay*16384/1000;
    wait_time = timer_TimRealTickGet(2);  //TIMER_2
    while(1)
    {
        if((timer_TimRealTickGet(2) - wait_time) > wait_time2)
        {
            return;
        }
    }

}


void sdmmc_Delay (int ticks)
{
    ticks *= 1000;

    while(ticks > 0) ticks--;
}


#define  HAL_ASSERT             //SCI_PASSERT
#define IFC_ASSERT(BOOL,param)  HAL_ASSERT(BOOL, ("Ifc param = %d", param))

// =============================================================================
//  Global variables
// =============================================================================


uint8 g_halSdmmcWriteCh = HAL_UNKNOWN_CHANNEL;
uint8 g_halSdmmcReadCh = HAL_UNKNOWN_CHANNEL;

/// SDMMC clock frequency.
 uint32 g_halSdmmcFreq = 200000;

//static HAL_CLK_T *gSdmmcClk = NULL;

//=============================================================================
// hal_SdmmcUpdateDivider
//-----------------------------------------------------------------------------
/// Update the SDMMC module divider to keep the desired frequency, whatever
/// the system frequency is.
/// @param sysFreq System Frequency currently applied or that will be applied
/// if faster.
//=============================================================================
//uint32 hal_SdmmcUpdateDivider(HAL_SYS_FREQ_T sysFreq);
void hal_set_sdmmcId(uint8 cardId)
{
    if (cardId == MCD_CARD_ID_0) {
        g_sdmmcId = MCD_CARD_ID_0;
        g_hwpSdmmcCtrl = hwp_sdmmc;
    }
}

//=============================================================================
// hal_SdmmcOpen
//-----------------------------------------------------------------------------
/// Open the SDMMC module. Take a resource.
//=============================================================================
  void hal_SdmmcOpen(uint8 clk_adj)
{
    uint32 value ;

    if (0)//(cardId == MCD_CARD_ID_0)
    {
        // In RDA8910, before open hwp_sdmmc,
        // iomux switch to function0
        hwp_iomux->sdmmc1_clk &= IOMUX_FUNC_SEL(0);
        hwp_iomux->sdmmc1_cmd &= IOMUX_FUNC_SEL(0);
        hwp_iomux->sdmmc1_data_0 &= IOMUX_FUNC_SEL(0);
        hwp_iomux->sdmmc1_data_1 &= IOMUX_FUNC_SEL(0);
        hwp_iomux->sdmmc1_data_2 &= IOMUX_FUNC_SEL(0);
        hwp_iomux->sdmmc1_data_3 &= IOMUX_FUNC_SEL(0);
        hwp_iomux->sdmmc1_data_4 &= IOMUX_FUNC_SEL(0);
        hwp_iomux->sdmmc1_data_5 &= IOMUX_FUNC_SEL(0);
        hwp_iomux->sdmmc1_data_6 &= IOMUX_FUNC_SEL(0);
        hwp_iomux->sdmmc1_data_7 &= IOMUX_FUNC_SEL(0);
    }
    _SdioReset();
    hal_McdDelay(1);
    _Sdio0Enable();
    hal_McdDelay(1);

    // We don't use interrupts.
    hwp_sdmmc->SDMMC_INT_MASK = 0x0;
    hwp_sdmmc->SDMMC_MCLK_ADJUST = clk_adj|(1<<4);// fprd electrical specification  inv clk to low(1<<4)  // chiptests do ont set 1<<4

}

  void hal_SdmmcSetClkMode(BOOLEAN auto_clk)
{
    //uint32 value ;
#if 0
    // Take the module out of reset
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
#ifdef CHIP_DIE_8909
    value = hwp_sysCtrl->lp_sys_cg0;
    hwp_sysCtrl->lp_sys_cg0 = auto_clk ?
        (value & ~SYS_CTRL_LP_CG_SDMMC) : (value | SYS_CTRL_LP_CG_SDMMC);
    hwp_sysCtrl->clk_sys_enable0 = SYS_CTRL_ENABLE_SDMMC;
#else
    value = hwp_sysCtrl->Clk_Per_Mode;
    hwp_sysCtrl->Clk_Per_Mode = auto_clk ?
        (value & ~SYS_CTRL_MODE_PER_SDMMC_MANUAL) : (value | SYS_CTRL_MODE_PER_SDMMC_MANUAL);
    hwp_sysCtrl->Clk_Per_Enable = SYS_CTRL_ENABLE_PER_SDMMC;
#endif
    hwp_sysCtrl->Sys_Rst_Clr = SYS_CTRL_CLR_RST_SDMMC;
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
#endif
}

//=============================================================================
// hal_SdmmcClose
//-----------------------------------------------------------------------------
/// Close the SDMMC module. Take a resource.
//=============================================================================
void hal_SdmmcClose(void)
{
    //pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
    // Free a resource
    _SdioReset();

    _Sdio0Disable();
    if(g_hwpSdmmcCtrl)
        g_hwpSdmmcCtrl->SDMMC_CONFIG = 0;

    g_hwpSdmmcCtrl = NULL;

    return;
#if 0
    if (1)//(g_sdmmcId == MCD_CARD_ID_0)
    {
        // In RDA8910, when closing hwp_sdmmc,
        // iomux also switch to function0
        hwp_iomux->pad_sdmmc1_clk_cfg_reg &= ~IOMUX_PAD_SDMMC1_CLK_SEL(0xF);
        hwp_iomux->pad_sdmmc1_cmd_cfg_reg &= ~IOMUX_PAD_SDMMC1_CMD_SEL(0xF);
        hwp_iomux->pad_sdmmc1_data_0_cfg_reg &= ~IOMUX_PAD_SDMMC1_DATA_0_SEL(0xF);
        hwp_iomux->pad_sdmmc1_data_1_cfg_reg &= ~IOMUX_PAD_SDMMC1_DATA_1_SEL(0xF);
        hwp_iomux->pad_sdmmc1_data_2_cfg_reg &= ~IOMUX_PAD_SDMMC1_DATA_2_SEL(0xF);
        hwp_iomux->pad_sdmmc1_data_3_cfg_reg &= ~IOMUX_PAD_SDMMC1_DATA_3_SEL(0xF);
    }
    else if (g_sdmmcId == MCD_CARD_ID_1)
    {
        // In RDA8910, when closing hwp_sdmmc2,
        // set sys_ctrl reserved1 bit[29]=0, and
        // iomux switch to function0
        hwp_sysCtrl->Cfg_Reserve1 &= ~SYS_CTRL_RESERVE1(1<<29);
        // sdmmc2_clk
        // hwp_iomux->pad_lcd_data_8_cfg_reg &= ~IOMUX_PAD_LCD_DATA_8_SEL(0xF);
        hwp_iomux->pad_gpio_0_cfg_reg &= ~IOMUX_PAD_GPIO_0_SEL(0xF);
        // sdmmc2_cmd
        // hwp_iomux->pad_lcd_data_9_cfg_reg &= ~IOMUX_PAD_LCD_DATA_9_SEL(0xF);
        hwp_iomux->pad_gpio_1_cfg_reg &= ~IOMUX_PAD_GPIO_1_SEL(0xF);
        // sdmmc2_data_0
        // hwp_iomux->pad_lcd_data_10_cfg_reg &= ~IOMUX_PAD_LCD_DATA_10_SEL(0xF);
        hwp_iomux->pad_gpio_2_cfg_reg &= ~IOMUX_PAD_GPIO_2_SEL(0xF);
        // sdmmc2_data_1
        // hwp_iomux->pad_lcd_data_11_cfg_reg &= ~IOMUX_PAD_LCD_DATA_11_SEL(0xF);
        hwp_iomux->pad_gpio_3_cfg_reg &= ~IOMUX_PAD_GPIO_3_SEL(0xF);
        // sdmmc2_data_2
        // hwp_iomux->pad_lcd_data_12_cfg_reg &= ~IOMUX_PAD_LCD_DATA_12_SEL(0xF);
        hwp_iomux->pad_gpio_4_cfg_reg &= ~IOMUX_PAD_GPIO_4_SEL(0xF);
        // sdmmc2_data_3
        // hwp_iomux->pad_lcd_data_13_cfg_reg &= ~IOMUX_PAD_LCD_DATA_13_SEL(0xF);
        hwp_iomux->pad_gpio_5_cfg_reg &= ~IOMUX_PAD_GPIO_5_SEL(0xF);
    }
#endif
    //g_sdmmcId = MCD_CARD_ID_NO;

}


//=============================================================================
// hal_SdmmcWakeUp
//-----------------------------------------------------------------------------
/// This function requests a resource of #HAL_SYS_FREQ_52M.
/// hal_SdmmcSleep() must be called before any other
//=============================================================================
  void hal_SdmmcWakeUp(void)
{
#if 0
    HAL_SYS_FREQ_T minFreq = HAL_SYS_FREQ_104M;
    // Take a resource (The idea is to be able to get a 25Mhz clock)
#ifdef MMI_CLK_IS_104M
    minFreq = HAL_SYS_FREQ_104M;
#endif  //104M
#ifdef MMI_CLK_IS_139M
    minFreq = HAL_SYS_FREQ_139M;
#endif  //156M
#ifdef MMI_CLK_IS_156M
    minFreq = HAL_SYS_FREQ_156M;
#endif  //156M
#ifdef MMI_CLK_IS_178M
    minFreq = HAL_SYS_FREQ_178M;
#endif  //156M
#ifdef MMI_CLK_IS_208M
    minFreq = HAL_SYS_FREQ_208M;
#endif  //208M
#ifdef MMI_CLK_IS_250M
    minFreq = HAL_SYS_FREQ_250M;
#endif  //250M
#ifdef MMI_CLK_IS_312M
    minFreq = HAL_SYS_FREQ_312M;
#endif  //312M

/*
    if (!hal_ClkIsEnabled(gSdmmcClk))
        hal_ClkEnable(gSdmmcClk);
    hal_ClkSetRate(gSdmmcClk, minFreq);
*/
#endif
}


//=============================================================================
// hal_SdmmcSleep
//-----------------------------------------------------------------------------
/// This function release the resource to #HAL_SYS_FREQ_32K.
//=============================================================================
  void hal_SdmmcSleep(void)
{
    // We just release the resource, because the clock gating in sdmmc controller
    // will disable the clock. We should wait for the clock to be actually disabled
    // but the module does not seam to have a status for that...

    // Free a resource

    //hal_ClkDisable(gSdmmcClk);
}

// =============================================================================
// hal_SdmmcSendCmd
// -----------------------------------------------------------------------------
/// Send a command to a SD card plugged to the sdmmc port.
/// @param cmd Command
/// @param arg Argument of the command
/// @param suspend Feature not implemented yet.
// =============================================================================
  void hal_SdmmcSendCmd(HAL_SDMMC_CMD_T cmd, uint32 arg, BOOLEAN suspend)
{
    uint32 configReg = 0;
#if !defined(GALLITE_IS_8805)
    hwp_sdmmc->SDMMC_CONFIG = SDMMC_AUTO_FLAG_EN;
#else
    hwp_sdmmc->SDMMC_CONFIG = 0x00000000;
#endif


    switch (cmd)
    {
        case HAL_SDMMC_CMD_GO_IDLE_STATE:
            configReg = SDMMC_SDMMC_SENDCMD;
            break;


        case HAL_SDMMC_CMD_ALL_SEND_CID:
            configReg = SDMMC_RSP_SEL_R2    | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x51
            break;

        case HAL_SDMMC_CMD_SEND_RELATIVE_ADDR:
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x11
            break;

        case HAL_SDMMC_CMD_SEND_IF_COND:
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x11
            break;

        case HAL_SDMMC_CMD_SET_DSR:
            configReg = SDMMC_SDMMC_SENDCMD;
            break;

        case HAL_SDMMC_CMD_SELECT_CARD:
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD;
            break;

        case HAL_SDMMC_CMD_SEND_CSD                :
            configReg = SDMMC_RSP_SEL_R2    | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD;
            break;

        case HAL_SDMMC_CMD_STOP_TRANSMISSION        :
            configReg = 0; // TODO
            break;

        case HAL_SDMMC_CMD_SEND_STATUS            :
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD;
            break;

        case HAL_SDMMC_CMD_SET_BLOCKLEN            :
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD;
            break;

        case HAL_SDMMC_CMD_ERASE_BLK_START            :
		case HAL_SDMMC_CMD_ERASE_BLK_END            :
		case HAL_SDMMC_CMD_MULTI_BLK_ERASE            :
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD;
            break;


        case HAL_SDMMC_CMD_READ_SINGLE_BLOCK        :
            configReg =  SDMMC_RD_WT_SEL_READ
                         | SDMMC_RD_WT_EN
                         | SDMMC_RSP_SEL_OTHER
                         | SDMMC_RSP_EN
                         | SDMMC_SDMMC_SENDCMD; // 0x111
            break;

        case HAL_SDMMC_CMD_READ_MULT_BLOCK        :
            configReg =  SDMMC_S_M_SEL_MULTIPLE
                         | SDMMC_RD_WT_SEL_READ
                         | SDMMC_RD_WT_EN
                         | SDMMC_RSP_SEL_OTHER
                         | SDMMC_RSP_EN
                         | SDMMC_SDMMC_SENDCMD
                         | SDMMC_AUTO_CMD12_EN; // 0x511;
            break;

        case HAL_SDMMC_CMD_WRITE_SINGLE_BLOCK    :
            configReg = SDMMC_RD_WT_SEL_WRITE
                        | SDMMC_RD_WT_EN
                        | SDMMC_RSP_SEL_OTHER
                        | SDMMC_RSP_EN
                        | SDMMC_SDMMC_SENDCMD; // 0x311
            break;

        case HAL_SDMMC_CMD_WRITE_MULT_BLOCK        :
            configReg =  SDMMC_S_M_SEL_MULTIPLE
                         | SDMMC_RD_WT_SEL_WRITE
                         | SDMMC_RD_WT_EN
                         | SDMMC_RSP_SEL_OTHER
                         | SDMMC_RSP_EN
                         | SDMMC_SDMMC_SENDCMD
                          | SDMMC_AUTO_CMD12_EN; // 0x711
            break;

        case HAL_SDMMC_CMD_APP_CMD                :
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x11
            break;

        case HAL_SDMMC_CMD_SET_BUS_WIDTH            :

            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x11
            break;

        case HAL_SDMMC_CMD_SEND_NUM_WR_BLOCKS    :
            configReg =  SDMMC_RD_WT_SEL_READ
                         | SDMMC_RD_WT_EN
                         | SDMMC_RSP_SEL_OTHER
                         | SDMMC_RSP_EN
                         | SDMMC_SDMMC_SENDCMD; // 0x111
            break;

        case HAL_SDMMC_CMD_SET_WR_BLK_COUNT        :
            configReg =  SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x11
            break;

        case HAL_SDMMC_CMD_MMC_SEND_OP_COND:
        case HAL_SDMMC_CMD_SEND_OP_COND:
            configReg =  SDMMC_RSP_SEL_R3    | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x31
            break;

        case HAL_SDMMC_CMD_SWITCH        :
            configReg =  SDMMC_RD_WT_EN | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x11
            break;

        default:
            break;
    }

    // TODO Add suspend management
    hwp_sdmmc->SDMMC_CMD_INDEX = SDMMC_COMMAND(cmd);
    hwp_sdmmc->SDMMC_CMD_ARG   = SDMMC_ARGUMENT(arg);

#if !defined(GALLITE_IS_8805)
    hwp_sdmmc->SDMMC_CONFIG    = configReg |SDMMC_AUTO_FLAG_EN;
#else
    hwp_sdmmc->SDMMC_CONFIG    = configReg ;
#endif

}


// =============================================================================
// hal_SdmmcNeedResponse
// -----------------------------------------------------------------------------
/// Tells if the given command waits for a reponse.
/// @return \c TRUE if the command needs an answer, \c FALSE otherwise.
// =============================================================================
  BOOLEAN hal_SdmmcNeedResponse(HAL_SDMMC_CMD_T cmd)
{
    switch (cmd)
    {
        case HAL_SDMMC_CMD_GO_IDLE_STATE:
        case HAL_SDMMC_CMD_SET_DSR:
        case HAL_SDMMC_CMD_STOP_TRANSMISSION:
            return FALSE;

        case HAL_SDMMC_CMD_ALL_SEND_CID:
        case HAL_SDMMC_CMD_SEND_RELATIVE_ADDR:
        case HAL_SDMMC_CMD_SEND_IF_COND:

        case HAL_SDMMC_CMD_SELECT_CARD:
        case HAL_SDMMC_CMD_SEND_CSD:
        case HAL_SDMMC_CMD_SEND_STATUS:
        case HAL_SDMMC_CMD_SET_BLOCKLEN:
        case HAL_SDMMC_CMD_READ_SINGLE_BLOCK:
        case HAL_SDMMC_CMD_READ_MULT_BLOCK:
        case HAL_SDMMC_CMD_WRITE_SINGLE_BLOCK:
        case HAL_SDMMC_CMD_WRITE_MULT_BLOCK:
        case HAL_SDMMC_CMD_APP_CMD:
        case HAL_SDMMC_CMD_SET_BUS_WIDTH:
        case HAL_SDMMC_CMD_SEND_NUM_WR_BLOCKS:
        case HAL_SDMMC_CMD_SET_WR_BLK_COUNT:
        case HAL_SDMMC_CMD_MMC_SEND_OP_COND:
        case HAL_SDMMC_CMD_SEND_OP_COND:
        case HAL_SDMMC_CMD_SWITCH:
        case HAL_SDMMC_CMD_ERASE_BLK_START:
        case HAL_SDMMC_CMD_ERASE_BLK_END:
        case HAL_SDMMC_CMD_MULTI_BLK_ERASE:
            return TRUE;

        default:
            // Dummy return, for the compiler to be pleased.
            return FALSE;
    }
}


// =============================================================================
// hal_SdmmcCmdDone
// -----------------------------------------------------------------------------
/// @return \c TRUE of there is not command pending or being processed.
// =============================================================================
  BOOLEAN hal_SdmmcCmdDone(void)
{
    return (!(hwp_sdmmc->SDMMC_STATUS & SDMMC_NOT_SDMMC_OVER));
}


// =============================================================================
// hal_SdmmcGetCardDetectPinLevel
// -----------------------------------------------------------------------------
/// @return \c TRUE if card detect (DAT3) line is high,
///         \c FALSE if the line is low.
/// User must check with SD spec and board pull-up/down resistor to
/// interpret this value.
// =============================================================================
  BOOLEAN hal_SdmmcGetCardDetectPinLevel(void)
{
    return FALSE;
}


//=============================================================================
// hal_SdmmcUpdateDivider
//-----------------------------------------------------------------------------
/// Update the SDMMC module divider to keep the desired frequency, whatever
/// the system frequency is.
/// @param sysFreq System Frequency currently applied or that will be applied
/// if faster.
//=============================================================================
#if 1
uint32 hal_SdmmcUpdateDivider(uint32 sysFreq)
{
    uint32 divider;
    return g_halSdmmcFreq;
#if 0
    g_halSdmmcFreq = sysFreq;
    if(sysFreq <= 200000)
	divider = 100;
	else
		divider = 10;
	/*
    if (g_halSdmmcFreq == 200000 )
    {
        uint32 divider = (sysFreq-1)/(2*g_halSdmmcFreq) + 1;
        if (divider>1)
        {
            divider = divider-1;
        }

        if (divider > 0xFF)
        {
            divider = 0xFF;
        }

        hwp_sdmmc->SDMMC_TRANS_SPEED = SDMMC_SDMMC_TRANS_SPEED(divider);
    }*/
        return g_halSdmmcFreq;
#endif
}
#endif

// =============================================================================
// hal_SdmmcSetClk
// -----------------------------------------------------------------------------
/// Set the SDMMC clock frequency to something inferior but close to that,
/// taking into account system frequency.
// =============================================================================
#if 0

#else
#define MMC_MAX_CLK 50000000
#define CONFIG_SYS_CLOCK 400000000  /* 400 MHz */
#define CONFIG_SYS_CLOCK_CHIPID3 200000000  /* 200 MHz chipid 3 the sd clk is mux 200MHz*/

void hal_SdmmcSetClk(uint32 clock)
{
    // Update the divider takes care of the registers configuration
    uint32 div;
    uint32 sd_sys_clock;

    if (clock > MMC_MAX_CLK) clock = MMC_MAX_CLK;

    // div = roundup(sys_clk / 2*mclk) - 1
    if (3 == CHIP_PHY_GetECOFlag())     /*chipid 3 sd clk is mux 200MHz*/
    {
        sd_sys_clock = CONFIG_SYS_CLOCK_CHIPID3;
    }
    else
    {
        sd_sys_clock = CONFIG_SYS_CLOCK;
    }

    div = sd_sys_clock/(2*clock) - 1;
    mon_Event(0x87220000|div);
    //sdmmc_event(0xdd000000|div);
    // rprintf("Set SDMMC clock DIV = (%d+(2*%d-1) / (2*%d) - 1 = %d\n", CONFIG_SYS_CLOCK * 1000000, clock, clock, div);
    // mclk   div  mclk  @ sys_clk=26MHz
    // 400K : 32   394Khz
    // 6.5M :  1   6.5Mhz
    if(div < 2)
        div=3;

    if(div > 0xff)
        div =0xff;

    hwp_sdmmc->SDMMC_TRANS_SPEED = SDMMC_SDMMC_TRANS_SPEED( div );
}
#endif
// =============================================================================
// hal_SdmmcGetOpStatus
// -----------------------------------------------------------------------------
/// @return The operational status of the SDMMC module.
// =============================================================================
  HAL_SDMMC_OP_STATUS_T hal_SdmmcGetOpStatus(void)
{
HAL_SDMMC_OP_STATUS_T r;
r.reg = (uint32)hwp_sdmmc->SDMMC_STATUS;
    return r;
}


// =============================================================================
// hal_SdmmcGetResp
// -----------------------------------------------------------------------------
/// This function is to be used to get the argument of the response of a
/// command. It is needed to provide the command index and its application
/// specific status, in order to determine the type of answer expected.
///
/// @param cmd Command to send
/// @param arg Pointer to a four 32 bit word array, where the argument will be
/// stored. Only the first case is set in case of a response of type R1, R3 or
/// R6, 4 if it is a R2 response.
/// @param suspend Unsupported
// =============================================================================
  void hal_SdmmcGetResp(HAL_SDMMC_CMD_T cmd, uint32* arg, BOOLEAN suspend)
{
    // TODO Check in the spec for all the commands response types
    switch (cmd)
    {
        // If they require a response, it is cargoed
        // with a 32 bit argument.
        case HAL_SDMMC_CMD_GO_IDLE_STATE       :
        case HAL_SDMMC_CMD_SEND_RELATIVE_ADDR  :
        case HAL_SDMMC_CMD_SEND_IF_COND        :
        case HAL_SDMMC_CMD_SET_DSR               :
        case HAL_SDMMC_CMD_SELECT_CARD           :
        case HAL_SDMMC_CMD_STOP_TRANSMISSION   :
        case HAL_SDMMC_CMD_SEND_STATUS           :
        case HAL_SDMMC_CMD_SET_BLOCKLEN           :
        case HAL_SDMMC_CMD_READ_SINGLE_BLOCK   :
        case HAL_SDMMC_CMD_READ_MULT_BLOCK       :
        case HAL_SDMMC_CMD_WRITE_SINGLE_BLOCK  :
        case HAL_SDMMC_CMD_WRITE_MULT_BLOCK       :
        case HAL_SDMMC_CMD_APP_CMD               :
        case HAL_SDMMC_CMD_SET_BUS_WIDTH       :
        case HAL_SDMMC_CMD_SEND_NUM_WR_BLOCKS  :
        case HAL_SDMMC_CMD_SET_WR_BLK_COUNT       :
        case HAL_SDMMC_CMD_MMC_SEND_OP_COND    :
        case HAL_SDMMC_CMD_SEND_OP_COND           :
        case HAL_SDMMC_CMD_SWITCH:
		case HAL_SDMMC_CMD_ERASE_BLK_START:
        case HAL_SDMMC_CMD_ERASE_BLK_END:
        case HAL_SDMMC_CMD_MULTI_BLK_ERASE:
            arg[0] = hwp_sdmmc->SDMMC_RESP_ARG3;
            arg[1] = 0;
            arg[2] = 0;
            arg[3] = 0;
            break;

        // Those response arguments are 128 bits
        case HAL_SDMMC_CMD_ALL_SEND_CID           :
        case HAL_SDMMC_CMD_SEND_CSD               :
            arg[0] = hwp_sdmmc->SDMMC_RESP_ARG0;
            arg[1] = hwp_sdmmc->SDMMC_RESP_ARG1;
            arg[2] = hwp_sdmmc->SDMMC_RESP_ARG2;
            arg[3] = hwp_sdmmc->SDMMC_RESP_ARG3;
            break;

        default:
            break;
    }
}


// =============================================================================
// hal_SdmmcGetResp32BitsArgument
// -----------------------------------------------------------------------------
/// This function is to be used to get the argument of the response of a
/// command triggerring a response with a 32 bits argument (typically,
/// R1, R3 or R6).
/// @return 32 bits of arguments of a 48 bits response token
// =============================================================================
  uint32 hal_SdmmcGetResp32BitsArgument(void)
{
    return hwp_sdmmc->SDMMC_RESP_ARG3;
}


// =============================================================================
// hal_SdmmcGetResp128BitsArgument
// -----------------------------------------------------------------------------
/// This function is to be used to get the argument of the response of a
/// command triggerring a response with a 128 bits argument (typically,
/// R2).
/// @param Pointer to a 4 case arrays of 32 bits where the argument of the
/// function will be stored.
// =============================================================================
  void hal_SdmmcGetResp128BitsArgument(uint32* buf)
{
    buf[0] = hwp_sdmmc->SDMMC_RESP_ARG0;
    buf[1] = hwp_sdmmc->SDMMC_RESP_ARG1;
    buf[2] = hwp_sdmmc->SDMMC_RESP_ARG2;
    buf[3] = hwp_sdmmc->SDMMC_RESP_ARG3;
}


// =============================================================================
// hal_SdmmcEnterDataTransferMode
// -----------------------------------------------------------------------------
/// Configure the SDMMC module to support data transfers
/// FIXME Find out why we need that out of the transfer functions...
// =============================================================================
  void hal_SdmmcEnterDataTransferMode(void)
{
    hwp_sdmmc->SDMMC_CONFIG |= SDMMC_RD_WT_EN;
}


// =============================================================================
// hal_SdmmcSetDataWidth
// -----------------------------------------------------------------------------
/// Set the data bus width
/// @param width Number of line of the SD data bus used.
// =============================================================================
  void hal_SdmmcSetDataWidth(HAL_SDMMC_DATA_BUS_WIDTH_T width)
{
    switch (width)
    {
        case HAL_SDMMC_DATA_BUS_WIDTH_1:
            hwp_sdmmc->SDMMC_DATA_WIDTH = 1;
            break;

        case HAL_SDMMC_DATA_BUS_WIDTH_4:
            hwp_sdmmc->SDMMC_DATA_WIDTH = 4;
            break;

        default:
	     break;
    }
}


// =============================================================================
// hal_SdmmcTransfer
// -----------------------------------------------------------------------------
/// Start the ifc transfer to feed the SDMMC controller fifo.
/// @param transfer Transfer to program.
/// @return HAL_ERR_NO or HAL_ERR_RESOURCE_BUSY.
// =============================================================================
  HAL_ERR_T hal_SdmmcTransfer(HAL_SDMMC_TRANSFER_T* transfer)
{
#if 1
    uint8 channel = 0;
    IFC_MANAGER_CHANNEL_LIST ifcReq = DMA_IFC_NO_REQWEST;
    uint32 length = 0;
    uint32 lengthExp = 0;

    length =  transfer->blockSize;

    // The block size register
    while (length != 1)
    {
        length >>= 1;
        lengthExp++;
    }

    // Configure amount of data
    hwp_sdmmc->SDMMC_BLOCK_CNT  = SDMMC_SDMMC_BLOCK_CNT(transfer->blockNum);
    hwp_sdmmc->SDMMC_BLOCK_SIZE = SDMMC_SDMMC_BLOCK_SIZE(lengthExp);

    // Configure Bytes reordering
    hwp_sdmmc->apbi_ctrl_sdmmc = SDMMC_SOFT_RST_L | SDMMC_L_ENDIAN(1);
    ifcReq = DMA_ID_RX_SDMMC;
    switch (transfer->direction)
    {
        case HAL_SDMMC_DIRECTION_READ:
            ifcReq = (g_sdmmcId == MCD_CARD_ID_0) ? DMA_ID_RX_SDMMC : 0;
            break;

        case HAL_SDMMC_DIRECTION_WRITE:
            ifcReq = (g_sdmmcId == MCD_CARD_ID_0) ? DMA_ID_TX_SDMMC : 0;
            break;

        default:
            break;
    }


//hal_ApIfcOpen;
    channel = hal_IfcTransferStart(ifcReq, transfer->sysMemAddr,
                                   transfer->blockNum*transfer->blockSize,
                                   HAL_IFC_SIZE_32_MODE_MANUAL);
   // sdmmc_event(0xfa000000|channel);
    if (channel == HAL_UNKNOWN_CHANNEL)
    {
       // sdmmc_event(0xfa001111);
        return HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Record Channel used.
        if (transfer->direction == HAL_SDMMC_DIRECTION_READ)
        {
            g_halSdmmcReadCh = channel;
			//sdmmc_event(0xfa001112);
        }
        else
        {
            g_halSdmmcWriteCh = channel;
			//sdmmc_event(0xfa001113);
        }
        return HAL_ERR_NO;
    }
	#endif
}


// =============================================================================
// hal_SdmmcTransferDone
// -----------------------------------------------------------------------------
/// Check the end of transfer status.
/// Attention: This means the SDMMC module has finished to transfer data.
/// In case of a read operation, the Ifc will finish its transfer shortly
/// after. Considering the way this function is used (after reading at least
/// 512 bytes, and flushing cache before releasing the data to the user), and
/// the fifo sizes, this is closely equivalent to the end of the transfer.
/// @return \c TRUE if a transfer is over, \c FALSE otherwise.
// =============================================================================
BOOLEAN hal_SdmmcTransferDone(void)
{
    // The link is not full duplex. We check both the
    // direction, but only one can be in progress at a time.
    IFC_MANAGER_CHANNEL_LIST ifcReqId = DMA_IFC_NO_REQWEST;
    SCI_DisableIRQ();

    //sdmmc_event(0xdd110001);sdmmc_event(g_halSdmmcReadCh);sdmmc_event(g_halSdmmcWriteCh);

    if (g_halSdmmcReadCh != HAL_UNKNOWN_CHANNEL)
    {
        ifcReqId = (g_sdmmcId == MCD_CARD_ID_0) ? DMA_ID_RX_SDMMC : 0;
        // Operation in progress is a read.
        // The SDMMC module itself can know it has finished
        if ((hwp_sdmmc->SDMMC_INT_STATUS & SDMMC_DAT_OVER_INT)
               && hal_IfcGetTc(ifcReqId) == 0)
        {
            // Transfer is over
            hwp_sdmmc->SDMMC_INT_CLEAR = SDMMC_DAT_OVER_CL;
            hal_IfcChannelRelease(ifcReqId);

            // We finished a read
            g_halSdmmcReadCh = HAL_UNKNOWN_CHANNEL;

            //  Put the FIFO in reset state.
            hwp_sdmmc->apbi_ctrl_sdmmc = 0 | SDMMC_L_ENDIAN(1);
            SCI_RestoreIRQ();

            return TRUE;
        }
    }

    if (g_halSdmmcWriteCh != HAL_UNKNOWN_CHANNEL)
    {
        ifcReqId = (g_sdmmcId == MCD_CARD_ID_0) ? DMA_ID_TX_SDMMC : 0;
        // Operation in progress is a write.
        // The SDMMC module itself can know it has finished
        // sdmmc_event(0xdd1100a1);sdmmc_event(g_halSdmmcWriteCh);
        if ((hwp_sdmmc->SDMMC_INT_STATUS & SDMMC_DAT_OVER_INT)
               && hal_IfcGetTc(ifcReqId) == 0)
        {
            // Transfer is over
            hwp_sdmmc->SDMMC_INT_CLEAR = SDMMC_DAT_OVER_CL;
            hal_IfcChannelRelease(ifcReqId);

            // We finished a write
            g_halSdmmcWriteCh = HAL_UNKNOWN_CHANNEL;

            //  Put the FIFO in reset state.
            hwp_sdmmc->apbi_ctrl_sdmmc = 0 | SDMMC_L_ENDIAN(1);
            SCI_RestoreIRQ();

            return TRUE;
        }
    }
    SCI_RestoreIRQ();

    // there's still data running through a pipe (or no transfer in progress ...)
    return FALSE;
}


// =============================================================================
// hal_SdmmcStopTransfer
// -----------------------------------------------------------------------------
/// Stop the ifc transfer feeding the SDMMC controller fifo.
/// @param transfer Transfer to program.
/// @return #HAL_ERR_NO
// =============================================================================
  HAL_ERR_T hal_SdmmcStopTransfer(HAL_SDMMC_TRANSFER_T* transfer)
{
    IFC_MANAGER_CHANNEL_LIST ifcReqId = DMA_IFC_NO_REQWEST;
    // Configure amount of data
    hwp_sdmmc->SDMMC_BLOCK_CNT  = SDMMC_SDMMC_BLOCK_CNT(0);
    hwp_sdmmc->SDMMC_BLOCK_SIZE = SDMMC_SDMMC_BLOCK_SIZE(0);

    //  Put the FIFO in reset state.
    hwp_sdmmc->apbi_ctrl_sdmmc = 0 | SDMMC_L_ENDIAN(1);


    if (transfer->direction == HAL_SDMMC_DIRECTION_READ)
    {
     ifcReqId = (g_sdmmcId == MCD_CARD_ID_0) ? DMA_ID_RX_SDMMC : 0;
        hal_IfcChannelFlush(ifcReqId);
        while(!hal_IfcChannelIsFifoEmpty(ifcReqId));
        hal_IfcChannelRelease(ifcReqId);
        g_halSdmmcReadCh = HAL_UNKNOWN_CHANNEL;
    }
    else
    {
    ifcReqId = (g_sdmmcId == MCD_CARD_ID_0) ? DMA_ID_TX_SDMMC : 0;
        hal_IfcChannelFlush(ifcReqId);
        while(!hal_IfcChannelIsFifoEmpty(ifcReqId));
        hal_IfcChannelRelease(ifcReqId);
        g_halSdmmcWriteCh = HAL_UNKNOWN_CHANNEL;
    }
    return HAL_ERR_NO;
}


// =============================================================================
// hal_SdmmcIsBusy
// -----------------------------------------------------------------------------
/// Check if the SD/MMC is busy.
///
/// @return \c TRUE if the SD/MMC controller is busy.
///         \c FALSE otherwise.
// =============================================================================
  BOOLEAN hal_SdmmcIsBusy(void)
{
    if (g_halSdmmcReadCh    != HAL_UNKNOWN_CHANNEL
            || g_halSdmmcWriteCh   != HAL_UNKNOWN_CHANNEL
            || ((hwp_sdmmc->SDMMC_STATUS & (SDMMC_NOT_SDMMC_OVER | SDMMC_BUSY | SDMMC_DL_BUSY)) != 0)
       )
    {
        // SD/MMc is busy doing something.
        SCI_TraceLow( "MCD: SdmmcIsBusy ReadCh=%d,WriteCh=%d,status=0x%x",g_halSdmmcReadCh,g_halSdmmcWriteCh,hwp_sdmmc->SDMMC_STATUS);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

