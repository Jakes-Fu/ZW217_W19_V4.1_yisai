/******************************************************************************
 ** File Name:      sdhost_phy_v5.c
 ** Author:         Jason.wu
 ** DATE:           09/17/2007
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.
 ** Description:    This file describe operation of SD host.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION
 ** 09/17/2007     Jason.wu        Create.
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "sdhost_drv.h"
#include "ldo_drvapi.h"
#include "threadx_assert.h"

#ifndef OS_NONE
#include "clock_drvapi.h"
#endif
#include "ref_outport.h"
#define SDHOST_PRINT(x) SCI_TRACE_LOW x

/*****************************************************************************/
//  Description:   Handle of sdhost
//  Author: Jason.wu
//  Param
//      host_cfg: start register of host controller
//      open_flag: Indicate whether this port has be used by other application
//      baseClock: the basic frequence that host work on
//      sdClock: the frequence that card work on
//      capbility: the function that canbe supported by host
//      err_filter: the error code that application want to watched
//      sigCallBack: the point of application that used to anounce that some signal happened
//  Note:
/*****************************************************************************/
typedef struct SDHOST_PORT_T_TAG
{
    volatile SDIO_REG_CFG *host_cfg;
    BOOLEAN open_flag;
    uint32 baseClock;
    uint32 sdClock;
    SDHOST_CAPBILIT_T capbility;
    uint32 err_filter;
    SDIO_CALLBACK sigCallBack;
} SDHOST_PORT_T;

typedef struct
{
    uint32 msg;
    uint32 errCode;
    SDHOST_SLOT_NO slotNum;
    SDHOST_HANDLE pSdhost_handler;

} ISR_Buffer_T;

INPUT_BUFFER_INIT (ISR_Buffer_T, SDHOST_SLOT_MAX_NUM)

LOCAL SDHOST_PORT_T sdio_port_ctl[SDHOST_SLOT_MAX_NUM];
#ifndef OS_NONE
LOCAL DEVICE_HANDLE s_dev_sdio = SCI_NULL;
#endif

LOCAL void  SdhostHisrFunc (uint32 cnt, void *pData);

#define PIN_SD_INFO_MAX 8
#define PIN_SD_INFO_INVALID 0xffffffff
LOCAL  PIN_SD_INFO_T pin_sd_info[PIN_SD_INFO_MAX] = 
{
#if !defined(PLATFORM_UWS6121E)
    {PIN_SD_CLK0, PIN_SD_CLK_REG, 0},
#if defined( CHIP_VER_6530) || defined( CHIP_VER_6531)
    {PIN_SD_CLK1, PIN_SD_CLK1_REG, 0},
#endif
    {PIN_SD_CMD, PIN_SD_CMD_REG, 0},
    {PIN_SD_D0, PIN_SD_D0_REG, 0},
    {PIN_SD_D1, PIN_SD_D1_REG, 0},
    {PIN_SD_D2, PIN_SD_D2_REG, 0},
    {PIN_SD_D3, PIN_SD_D3_REG, 0},
#else
    {PIN_SD_CLK0, REG_IOMUX_PAD_SDMMC1_CLK_CFG_REG, 0},
    {PIN_SD_CMD, REG_IOMUX_PAD_SDMMC1_CMD_CFG_REG, 0},
    {PIN_SD_D0, REG_IOMUX_PAD_SDMMC1_DATA_0_CFG_REG, 0},
    {PIN_SD_D1, REG_IOMUX_PAD_SDMMC1_DATA_1_CFG_REG, 0},
    {PIN_SD_D2, REG_IOMUX_PAD_SDMMC1_DATA_2_CFG_REG, 0},
    {PIN_SD_D3, REG_IOMUX_PAD_SDMMC1_DATA_3_CFG_REG, 0},
#endif
    {PIN_SD_INFO_INVALID, PIN_SD_INFO_INVALID, PIN_SD_INFO_INVALID}
};//lint !e785

/*****************************************************************************/
//  Description:  To confirm whether the handle is valid
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      TRUE the handle is valid
//      FALSE:the handle is not valid
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _RegisterVerifyHOST (SDHOST_HANDLE sdhost_handler)
{
    uint32 index;

    for (index = 0; index < SDHOST_SLOT_MAX_NUM; index++)
    {
        if ( (sdhost_handler == &sdio_port_ctl[index]) && (TRUE == sdio_port_ctl[index].open_flag))
        {
            return TRUE;
        }
    }

    return FALSE;
}

PUBLIC uint32 SDHOST_GetPinState (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    return sdhost_handler->host_cfg->PRESENT_STAT;
}

//---Power Control Register---
/*****************************************************************************/
//  Description:  power on/off led
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      onoff:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_Led (SDHOST_HANDLE sdhost_handler,SDHOST_LED_ONOFF_E onoff)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    //===
    switch (onoff)
    {
        case SDIO_LED_ON:
            {
                sdhost_handler->host_cfg->HOST_CTL0 |= 0x1;
            }
            break;

        case SDIO_LED_OFF:
            {
                sdhost_handler->host_cfg->HOST_CTL0 &= (~0x1);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    //===
}

/*****************************************************************************/
//  Description:  set data bus width
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      width: data bus width,only 1bit ,4bit and 8bit canbe used
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_Cfg_BusWidth (SDHOST_HANDLE sdhost_handler,SDHOST_BIT_WIDTH_E width)
{
    uint32 tmpReg =  sdhost_handler->host_cfg->HOST_CTL0;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg &= (~ (0x1<<5));
    tmpReg &= (~ (0x1<<1));

    switch (width)
    {
        case SDIO_1BIT_WIDTH:
            {
                //do nothing
            }
            break;

        case SDIO_4BIT_WIDTH:
            {
                tmpReg |= (0x1<<1);
            }
            break;

        case SDIO_8BIT_WIDTH:
            {
                tmpReg |= (0x1<<5);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    sdhost_handler->host_cfg->HOST_CTL0 = tmpReg;
}

/*****************************************************************************/
//  Description:  set bus speed mode
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      speed: speed mode ,only low speed mode and high speed mode canbe used
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_Cfg_SpeedMode (SDHOST_HANDLE sdhost_handler,SDHOST_SPEED_E speed)
{
    uint32 tmpReg =  sdhost_handler->host_cfg->HOST_CTL0;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg &= (~ (0x1 <<2));

    switch (speed)
    {
        case SDIO_HIGHSPEED:
            {
                tmpReg |= (0x1<<2);
            }
            break;

        case SDIO_LOWSPEED:
            {
                //do nothing
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    sdhost_handler->host_cfg->HOST_CTL0 = tmpReg;
}

/*****************************************************************************/
//  Description:  Set operation voltage of card(mmc \sd\sdio card.etc.)
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      voltage:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_Cfg_Voltage (SDHOST_HANDLE sdhost_handler,SDHOST_VOL_RANGE_E voltage)
{
#if defined(CHIP_VER_6531)
	SCI_ASSERT(TRUE == _RegisterVerifyHOST(sdhost_handler));/*assert verified*/
	
	switch(voltage)
	{
		case VOL_1_8:
			LDO_SetVoltLevel(LDO_LDO_SDIO, LDO_VOLT_LEVEL3);
			break;
		case VOL_2_65:
			LDO_SetVoltLevel(LDO_LDO_SDIO, LDO_VOLT_LEVEL0);
			break;
		case VOL_2_8:
			LDO_SetVoltLevel(LDO_LDO_SDIO, LDO_VOLT_LEVEL0);
			break;
		case VOL_3_0:
			LDO_SetVoltLevel(LDO_LDO_SDIO, LDO_VOLT_LEVEL0);
			break;
		default:
			break;
	}
#endif
}

/*****************************************************************************/
//  Description:  Open or close power supply of card(mmc \sd\sdio card.etc.)
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      on_off:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SD_POWER (SDHOST_HANDLE sdhost_handler,SDHOST_PWR_ONOFF_E on_off)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    ///added by mingwei, Andy need to check again.
    if (POWR_ON == on_off)
    {
        LDO_TurnOnLDO (LDO_LDO_SDIO);
    }
    else
    {
        LDO_TurnOffLDO (LDO_LDO_SDIO);
    }
    /* To wait for the stability of LDO before CLK output
     * DVT CR:638623 */
    SCI_Sleep(1);
}
//---Block Gap Control Register---
/*****************************************************************************/
//  Description:  Set the position of break point
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_BlkGapIntPosSet (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

}

/*****************************************************************************/
//  Description:  Enable pause function of host. the card must support this function,then this function can be worked
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_EnableReadWaitCtl (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL0 |= BIT_18;
}

/*****************************************************************************/
//  Description:  Set break point during the transmition
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_StopAtBlkGap (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL0 |= (BIT_16);
}
PUBLIC void SDHOST_ClearBlkGap (SDHOST_HANDLE sdhost_handler)
{
    sdhost_handler->host_cfg->HOST_CTL0 &= (~ (0x01<<16));
}
PUBLIC void SDHOST_BlkGapIntEn (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler)); /*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL0 |= (BIT_19);
}
PUBLIC uint32 SDHOST_GetTransStat (SDHOST_HANDLE sdhost_handler)
{
    return (sdhost_handler->host_cfg->PRESENT_STAT & (BIT_8|BIT_9));
}

/*****************************************************************************/
//  Description:  When transmission is paused ,this function can resume the transmission
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_Continue (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL0 &= (~ (0x03<<16));
    sdhost_handler->host_cfg->HOST_CTL0 |= BIT_17;

}



//----Clock Control Register---
/*****************************************************************************/
//  Description:  Open or close internal clk.when this clk is disable ,the host will enter in sleep mode
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      onoff:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_internalClk_OnOff (SDHOST_HANDLE sdhost_handler,SDHOST_CLK_ONOFF_E onoff)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    switch (onoff)
    {
        case CLK_ON:
            {
                //Enable internal clock
                sdhost_handler->host_cfg->HOST_CTL1 |= BIT_0;

                //Wait internal clock stable
                while (0 == (sdhost_handler->host_cfg->HOST_CTL1 & BIT_1)) {}
            }
            break;

        case CLK_OFF:
            {
                sdhost_handler->host_cfg->HOST_CTL1 &= (~BIT_0);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }
}

/*****************************************************************************/
//  Description:  Open or close card clk.
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      onoff:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SD_clk_OnOff (SDHOST_HANDLE sdhost_handler,SDHOST_CLK_ONOFF_E onoff)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    switch (onoff)
    {
        case CLK_ON:
            {
                SDHOST_internalClk_OnOff (sdhost_handler,CLK_ON);
                //Enable internal clock
                sdhost_handler->host_cfg->HOST_CTL1 |= BIT_2;

                //Wait internal clock stable
                while (0 == (sdhost_handler->host_cfg->HOST_CTL1 & BIT_1)) {} //maybe it is not nessarry
            }
            break;

        case CLK_OFF:
            {
                sdhost_handler->host_cfg->HOST_CTL1 &= (~BIT_2);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }
}

/*****************************************************************************/
//  Description:  Set the frequence of Card clock
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      sdio_clk: the frequence of card working clock
//  Return:
//      uint32 value : the frequency that be used acctually
//  Note:
/*****************************************************************************/
PUBLIC uint32 SDHOST_SD_Clk_Freq_Set (SDHOST_HANDLE sdhost_handler,uint32 sdio_clk)
{

    volatile uint32 tmpReg;
    uint32 clkDiv;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/
    SCI_ASSERT (0 != sdio_clk);/*assert verified*/

    sdio_clk = (sdio_clk > SDIO_SD_MAX_CLK) ? (SDIO_SD_MAX_CLK) : (sdio_clk);

    SDHOST_SD_clk_OnOff (sdhost_handler,CLK_OFF);

    //SDCLK Frequency Select ,Configure SDCLK select
    clkDiv = sdhost_handler->baseClock/sdio_clk;

    if (0 != sdhost_handler->baseClock%sdio_clk)
    {
        clkDiv++;
    }

    //SDHOST_PRINT:"   clkDiv: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SDHOST_PHY_V5_443_112_2_17_23_6_19_1128,(uint8*)"d",clkDiv);

    tmpReg = sdhost_handler->host_cfg->HOST_CTL1;
    tmpReg &= (~ (0xff<<8));

    if (256 < clkDiv)
    {
        //SDHOST_PRINT:"   clkDiv: %d is too big!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SDHOST_PHY_V5_450_112_2_17_23_6_19_1129,(uint8*)"d",clkDiv);
        //SCI_ASSERT (0);/*assert to do*/
        clkDiv = 256;
        tmpReg |= (0x80 << 8);
    }
    else if (128 < clkDiv)
    {
        clkDiv = 256;
        tmpReg |= (0x80 << 8);
    }
    else if (64 < clkDiv)
    {
        clkDiv = 128;
        tmpReg |= (0x40<<8);
    }
    else if (32 < clkDiv)
    {
        clkDiv = 64;
        tmpReg |= (0x20<<8);
    }
    else if (16 < clkDiv)
    {
        clkDiv = 32;
        tmpReg |= (0x10<<8);
    }
    else if (8 < clkDiv)
    {
        clkDiv = 16;
        tmpReg |= (0x8<<8);
    }
    else if (4 < clkDiv)
    {
        clkDiv = 8;
        tmpReg |= (0x4<<8);
    }
    else if (2 < clkDiv)
    {
        clkDiv = 4;
        tmpReg |= (0x2<<8);
    }
    else if (1 < clkDiv)
    {
        clkDiv = 2;
        tmpReg |= (0x1<<8);
    }
    else if (0 < clkDiv)
    {
        clkDiv = 1;
        //nothing
    }
    else //if (0 == clkDiv)
    {
        SCI_ASSERT (0);/*assert to do*/
    }

    sdhost_handler->host_cfg->HOST_CTL1 = tmpReg;
    sdhost_handler->sdClock = sdhost_handler->baseClock/clkDiv;

    //SDHOST_PRINT:"sd clk: %d KHz."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SDHOST_PHY_V5_506_112_2_17_23_6_20_1130,(uint8*)"d",sdhost_handler->sdClock/1000);
    return sdhost_handler->sdClock;
}

//---Timeout Control Register--
/*****************************************************************************/
//  Description:  Set timeout value ,this value is used during the data transmission
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      clk_cnt: the value is (2 ^ (clkcnt+13))*T_BSCLK,T_BSCLK is working frequence of host
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetDataTimeOutValue (SDHOST_HANDLE sdhost_handler,uint8 clk_cnt) // (2 ^ (clkcnt+13))*T_BSCLK
{
    volatile uint32 tmpReg,tmpIntReg;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpIntReg = sdhost_handler->host_cfg->INT_STA_EN;
    // cfg the data timeout clk----------
    sdhost_handler->host_cfg->INT_STA_EN &= ~BIT_20;

    tmpReg = sdhost_handler->host_cfg->HOST_CTL1;
    tmpReg &= ~ (0xF << 16);
    tmpReg |= (clk_cnt << 16);
    sdhost_handler->host_cfg->HOST_CTL1 = tmpReg;

    sdhost_handler->host_cfg->INT_STA_EN = tmpIntReg;
}

//---Software Reset Register---
/*****************************************************************************/
//  Description: Reset data line of host
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void _Reset_DAT (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= BIT_26;

    while (0!= (sdhost_handler->host_cfg->HOST_CTL1 & BIT_26)) {}
}
/*****************************************************************************/
//  Description: Reset command line of host
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void _Reset_CMD (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= BIT_25;

    while (0!= (sdhost_handler->host_cfg->HOST_CTL1 & BIT_25)) {}
}

/*****************************************************************************/
//  Description: Reset command line and data line of host
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void _Reset_DAT_CMD (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= (BIT_25|BIT_26);

    while (0!= (sdhost_handler->host_cfg->HOST_CTL1 & (BIT_25|BIT_26))) {}


}

/*****************************************************************************/
//  Description: Reset all the module of host
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void _Reset_ALL (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= BIT_24;

    while (0 != (sdhost_handler->host_cfg->HOST_CTL1 & BIT_24)) {}

}

/*****************************************************************************/
//  Description: Reset  module of host
//  Author: Wenjun.Shi
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void _Reset_MODULE (SDHOST_HANDLE sdhost_handler)
{
       
#ifdef DUAL_TCARD_SUPPORT
    uint32 i;
   uint32 sdhost_reg0[0x20];
   uint32 sdhost_reg1[0x20];
#endif
	SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/
#ifdef DUAL_TCARD_SUPPORT	
    for(i=0;i<0x20;i++)
    	{
	 sdhost_reg0[i] = CHIP_REG_GET(SDIO0_BASE_ADDR+4*i);
	}
    for(i=0;i<0x20;i++)
    	{
	 sdhost_reg1[i] = CHIP_REG_GET(SDIO1_BASE_ADDR+4*i);
	}
#endif


	CHIP_REG_SET (AHB_RST0_SET, SDIO_SOFT_RST_SET);
	CHIP_REG_SET (AHB_RST0_CLR, SDIO_SOFT_RST_CLR);

#ifdef DUAL_TCARD_SUPPORT
if((uint32*)(sdhost_handler ->host_cfg) == (uint32*)SDIO1_BASE_ADDR)
{
      for(i=0;i<0x20;i++)
    	{
	CHIP_REG_SET((SDIO0_BASE_ADDR+4*i),sdhost_reg0[i]);
	} 
}
if((uint32*)(sdhost_handler ->host_cfg) == (uint32*)SDIO0_BASE_ADDR)
{
     for(i=0;i<0x20;i++)
    	{
	CHIP_REG_SET((SDIO1_BASE_ADDR+4*i),sdhost_reg1[i]);
	} 
}
#endif
}


/*****************************************************************************/
//  Description: Reset the specify module of host
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      rst_type: indicate which module will be reset(command lin\data line\all the module)
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_RST (SDHOST_HANDLE sdhost_handler,SDHOST_RST_TYPE_E rst_type)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    switch (rst_type)
    {
        case RST_CMD_LINE:
            {
                _Reset_CMD (sdhost_handler);
            }
            break;

        case RST_DAT_LINE:
            {
                _Reset_DAT (sdhost_handler);
            }
            break;

        case RST_CMD_DAT_LINE:
            {
                _Reset_DAT_CMD (sdhost_handler);
            }
            break;

        case RST_ALL:
            {
                _Reset_ALL (sdhost_handler);
            }
            break;

        case RST_MODULE:
            {
                _Reset_MODULE (sdhost_handler);
            }
            break;
        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }
}

/*****************************************************************************/
//  Description: Set block size \count of block\and Dma Buffer size
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      block_size: size of block( 0 <= block size <=0x0800)
//      block_cnt: the count of block(0 <= block_cnt <= 0xFFFF)
//      DMAsize:buffer size of DMA(4K,8K,16K,32K,64K,128K,256K,512K)
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetDataParam (SDHOST_HANDLE sdhost_handler,uint32 block_size,uint32 block_cnt,SDHOST_DMA_BUF_SIZE_E DMAsize)
{
    volatile uint32 tmpReg;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/
    SCI_ASSERT (0x1000 >= block_size);/*assert verified*/
    SCI_ASSERT (0xFFFF >= block_cnt);/*assert verified*/

    tmpReg = sdhost_handler->host_cfg->BLK_SIZE_COUNT;

    // Set Block Size
    tmpReg &= (~BIT_15);
    tmpReg &= (~ (0xFFF));

    if (0x1000 == block_size)
    {
        tmpReg |= BIT_15;
    }
    else
    {
        tmpReg |= block_size;
    }

    // Set Block Cnt
    tmpReg &= (~0xFFFF0000);
    tmpReg |= (block_cnt << 16);

    // Set DMA Buf Size
    tmpReg &= (~ (0x07<<12));

    switch (DMAsize)
    {
        case SDIO_DMA_4K:
            {
                //do nothing
            }
            break;

        case SDIO_DMA_8K:
            {
                tmpReg |= (0x01<<12);
            }
            break;

        case SDIO_DMA_16K:
            {
                tmpReg |= (0x02<<12);
            }
            break;

        case SDIO_DMA_32K:
            {
                tmpReg |= (0x03<<12);
            }
            break;

        case SDIO_DMA_64K:
            {
                tmpReg |= (0x04<<12);
            }
            break;

        case SDIO_DMA_128K:
            {
                tmpReg |= (0x05<<12);
            }
            break;

        case SDIO_DMA_256K:
            {
                tmpReg |= (0x06<<12);
            }
            break;

        case SDIO_DMA_512K:
            {
                tmpReg |= (0x07<<12);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;

    }

    sdhost_handler->host_cfg->BLK_SIZE_COUNT = tmpReg;
}

PUBLIC void SDHOST_GetDataParam (SDHOST_HANDLE sdhost_handler,uint32 *block_size,uint32 *block_cnt, uint32 *dmaAddr)
{
    uint32 sizecnt;

    sizecnt = sdhost_handler->host_cfg->BLK_SIZE_COUNT;
    *block_size = sizecnt&0xFFF;
    *block_cnt  = ( (sizecnt&0xFFFF0000) >>16);
    *dmaAddr    = sdhost_handler->host_cfg->DMA_SYS_ADD;
}

/*****************************************************************************/
//  Description: Set start address of DMA buffer
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      dmaAddr: start address of DMA buffer
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetDmaAddr (SDHOST_HANDLE sdhost_handler, uint32 dmaAddr)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->DMA_SYS_ADD = dmaAddr;

}

/*****************************************************************************/
//  Description: Get stop address of DMA buffer,when buffer is used ,the dma will stop at last address of buffer.this function will get this address
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      uint32 value: address that DMA stoped at
//  Note:
/*****************************************************************************/
PUBLIC uint32 SDHOST_GetDmaAddr (SDHOST_HANDLE sdhost_handler)
{
    uint32 dmaAddr;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    dmaAddr = sdhost_handler->host_cfg->DMA_SYS_ADD;
    return dmaAddr;
}
//---Argument Register---
/*****************************************************************************/
//  Description: Set the argument of command
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      argument
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetCmdArg (SDHOST_HANDLE sdhost_handler,uint32 argument)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->CMD_ARGUMENT = argument;
}

//---CMD Register---
/*****************************************************************************/
//  Description: Set the mode of command
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      cmdIndex:   command
//      transmode:  transfer mode
//      cmd_type:   comand type ,it may be normal comman ,resume comman etc.
//      Response:   the inspect response from card. if this comman is performed by card successly ,this response will be return by card
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetCmd (SDHOST_HANDLE sdhost_handler,uint32 cmdIndex,uint32 transmode,SDHOST_CMD_TYPE_E cmd_type,CMD_RSP_TYPE_E Response)
{
    volatile uint32 tmpReg;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg = sdhost_handler->host_cfg->CMD_TRANSMODE;
    tmpReg &= (~ (0x7F|0x30000|0x3FF80000));

    if (0 != (TRANS_MODE_ATA_CMPLETE_SIG_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_COMP_ATA;
    }

    if (0 != (TRANS_MODE_MULTI_BLOCK&transmode))
    {
        tmpReg |= SDIO_TRANS_MULTIBLK;
    }

    if (0 != (TRANS_MODE_READ&transmode))
    {
        tmpReg |= SDIO_TRANS_DIR_READ;
    }

    if (0 != (TRANS_MODE_CMD12_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_AUTO_CMD12_EN;
    }

    if (0 != (TRANS_MODE_BLOCK_COUNT_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_BLK_CNT_EN;
    }

    if (0 != (TRANS_MODE_DMA_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_DMA_EN;
    }

    if (0 != (CMD_HAVE_DATA&transmode))
    {
        tmpReg |= SDIO_CMD_DATA_PRESENT;
    }

    switch (cmd_type)
    {
        case CMD_TYPE_NORMAL:
            {
                tmpReg |= SDIO_CMD_TYPE_NML;
            }
            break;

        case CMD_TYPE_SUSPEND:
            {
                tmpReg |= SDIO_CMD_TYPE_SUSPEND;
            }
            break;

        case CMD_TYPE_RESUME:
            {
                tmpReg |= SDIO_CMD_TYPE_RESUME;
            }
            break;

        case CMD_TYPE_ABORT:
            {
                tmpReg |= SDIO_CMD_TYPE_ABORT;
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    switch (Response)
    {
        case CMD_NO_RSP:
            {
                tmpReg |= SDIO_NO_RSP;
            }
            break;

        case CMD_RSP_R1:
            {
                tmpReg |= SDIO_R1;
            }
            break;

        case CMD_RSP_R2:
            {
                tmpReg |= SDIO_R2;
            }
            break;

        case CMD_RSP_R3:
            {
                tmpReg |= SDIO_R3;
            }
            break;

        case CMD_RSP_R4:
            {
                tmpReg |= SDIO_R4;
            }
            break;

        case CMD_RSP_R5:
            {
                tmpReg |= SDIO_R5;
            }
            break;

        case CMD_RSP_R6:
            {
                tmpReg |= SDIO_R6;
            }
            break;

        case CMD_RSP_R7:
            {
                tmpReg |= SDIO_R7;
            }
            break;

        case CMD_RSP_R1B:
            {
                tmpReg |= SDIO_R1B;
            }
            break;

        case CMD_RSP_R5B:
            {
                tmpReg |= SDIO_R5B;
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;

    }

    tmpReg |= (cmdIndex<<24);

    sdhost_handler->host_cfg->CMD_TRANSMODE = tmpReg;

}


//==
/*****************************************************************************/
//  Description: Get content from host response register
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      Response:   the type of response
//      rspBuf:     the content will be stored in this place
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_GetRspFromBuf (SDHOST_HANDLE sdhost_handler,CMD_RSP_TYPE_E Response,uint8 *rspBuf)
{
    uint32 tmpRspBuf[4];
    uint32 i;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpRspBuf[0] = sdhost_handler->host_cfg->RSP0;
    tmpRspBuf[1] = sdhost_handler->host_cfg->RSP1;
    tmpRspBuf[2] = sdhost_handler->host_cfg->RSP2;
    tmpRspBuf[3] = sdhost_handler->host_cfg->RSP3;

    //  SDHOST_PRINT(("RSP %x %x %x %x",tmpRspBuf[0],tmpRspBuf[1],tmpRspBuf[2],tmpRspBuf[3]));

    for (i = 0; i < 4; i++)
    {
        rspBuf[0+ (i<<2) ] = (uint8) ( (tmpRspBuf[i]>>24) &0xFF);
        rspBuf[1+ (i<<2) ] = (uint8) ( (tmpRspBuf[i]>>16) &0xFF);
        rspBuf[2+ (i<<2) ] = (uint8) ( (tmpRspBuf[i]>>8) &0xFF);
        rspBuf[3+ (i<<2) ] = (uint8) (tmpRspBuf[i]&0xFF);
    }

    switch (Response)
    {
        case CMD_NO_RSP:
            break;

        case CMD_RSP_R1:
        case CMD_RSP_R1B:
        case CMD_RSP_R3:
        case CMD_RSP_R4:
        case CMD_RSP_R5:
        case CMD_RSP_R6:
        case CMD_RSP_R7:
        case CMD_RSP_R5B:
            {
                rspBuf[0] = (uint8) ( (tmpRspBuf[0]>>24) &0xFF);
                rspBuf[1] = (uint8) ( (tmpRspBuf[0]>>16) &0xFF);
                rspBuf[2] = (uint8) ( (tmpRspBuf[0]>>8) &0xFF);
                rspBuf[3] = (uint8) (tmpRspBuf[0]&0xFF);
            }
            break;

        case CMD_RSP_R2:
            {
                rspBuf[0] = (uint8) ( (tmpRspBuf[3]>>16) &0xFF);
                rspBuf[1] = (uint8) ( (tmpRspBuf[3]>>8) &0xFF);
                rspBuf[2] = (uint8) (tmpRspBuf[3]&0xFF);

                rspBuf[3] = (uint8) ( (tmpRspBuf[2]>>24) &0xFF);
                rspBuf[4] = (uint8) ( (tmpRspBuf[2]>>16) &0xFF);
                rspBuf[5] = (uint8) ( (tmpRspBuf[2]>>8) &0xFF);
                rspBuf[6] = (uint8) (tmpRspBuf[2]&0xFF);

                rspBuf[7] = (uint8) ( (tmpRspBuf[1]>>24) &0xFF);
                rspBuf[8] = (uint8) ( (tmpRspBuf[1]>>16) &0xFF);
                rspBuf[9] = (uint8) ( (tmpRspBuf[1]>>8) &0xFF);
                rspBuf[10] = (uint8) (tmpRspBuf[1]&0xFF);

                rspBuf[11] = (uint8) ( (tmpRspBuf[0]>>24) &0xFF);
                rspBuf[12] = (uint8) ( (tmpRspBuf[0]>>16) &0xFF);
                rspBuf[13] = (uint8) ( (tmpRspBuf[0]>>8) &0xFF);
                rspBuf[14] = (uint8) (tmpRspBuf[0]&0xFF);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    return;
}


//====

//----
/*****************************************************************************/
//  Description: Get function that host can be support
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      capbility
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void _GetSDHOSTCapbility (SDHOST_HANDLE sdhost_handler,SDHOST_CAPBILIT_T *capbility)
{
    volatile uint32 tmpReg;
    SCI_MEMSET (capbility,0,sizeof (SDHOST_CAPBILIT_T));

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg = sdhost_handler->host_cfg->CAPBILITY;

    if (0 != (tmpReg &  BIT_26))
    {
        capbility->capbility_function |= CAP_VOL_1_8V;
    }

    if (0 != (tmpReg &  BIT_25))
    {
        capbility->capbility_function |= CAP_VOL_3_0V;
    }

    if (0 != (tmpReg &  BIT_24))
    {
        capbility->capbility_function |= CAP_VOL_3_3V;
    }

    if (0 != (tmpReg &  BIT_23))
    {
        capbility->capbility_function |= SPD_RESU;
    }

    if (0 != (tmpReg &  BIT_22))
    {
        capbility->capbility_function |= DMA_SPT;
    }

    if (0 != (tmpReg &  BIT_21))
    {
        capbility->capbility_function |= HIGH_SPEED;
    }

    switch (tmpReg & (0x3<<16))
    {
        case (0x00<<16) :
            capbility->cability_Max_BlkLen = 512;
            break;

        case (0x01<<16) :
            capbility->cability_Max_BlkLen = 1024;
            break;

        case (0x02<<16) :
            capbility->cability_Max_BlkLen = 2048;
            break;

        case (0x03<<16) :
            capbility->cability_Max_BlkLen = 4096;
            break;

        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    capbility->sd_Base_Max_Clk = ( (tmpReg & (0x3F<<8)) >>8) * 1000000;

    if (0!= (tmpReg & BIT_7))
    {
        capbility->timeOut_Clk_unit = 1000000;
    }
    else
    {
        capbility->timeOut_Clk_unit = 1000;
    }

    capbility->timeOut_Base_Clk = (tmpReg & 0x3F) *capbility->timeOut_Clk_unit;

    tmpReg = sdhost_handler->host_cfg->CURR_CAPBILITY;
    capbility->max_current_for_1_8 = ( (tmpReg & (0xFF<<16)) >>16);
    capbility->max_current_for_3_0 = ( (tmpReg & (0xFF<<8)) >>8);
    capbility->max_current_for_3_3 = (tmpReg & 0xFF);

    //SDHOST_PRINT:"capbility_function = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SDHOST_PHY_V5_1175_112_2_17_23_6_21_1131,(uint8*)"d",sdhost_handler->capbility.capbility_function);
    //SDHOST_PRINT:"cability_Max_BlkLen = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SDHOST_PHY_V5_1176_112_2_17_23_6_21_1132,(uint8*)"d",sdhost_handler->capbility.cability_Max_BlkLen);
    //SDHOST_PRINT:"sd_Base_Max_Clk = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SDHOST_PHY_V5_1177_112_2_17_23_6_21_1133,(uint8*)"d",sdhost_handler->capbility.sd_Base_Max_Clk);
    //SDHOST_PRINT:"timeOut_Clk_unit = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SDHOST_PHY_V5_1178_112_2_17_23_6_21_1134,(uint8*)"d",sdhost_handler->capbility.timeOut_Clk_unit);
    //SDHOST_PRINT:"timeOut_Base_Clk = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SDHOST_PHY_V5_1179_112_2_17_23_6_21_1135,(uint8*)"d",sdhost_handler->capbility.timeOut_Base_Clk);
    //SDHOST_PRINT:"max_current_for_1_8 = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SDHOST_PHY_V5_1180_112_2_17_23_6_21_1136,(uint8*)"d",sdhost_handler->capbility.max_current_for_1_8);
    //SDHOST_PRINT:"max_current_for_3_0 = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SDHOST_PHY_V5_1181_112_2_17_23_6_21_1137,(uint8*)"d",sdhost_handler->capbility.max_current_for_3_0);
    //SDHOST_PRINT:"max_current_for_3_3 = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SDHOST_PHY_V5_1182_112_2_17_23_6_21_1138,(uint8*)"d",sdhost_handler->capbility.max_current_for_3_3);
}


/*****************************************************************************/
//  Description: Clear Normal int Status register ,if event is happed ,host will Set status in register
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        that int you want to clear
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_NML_IntStatus_Clr (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntStatus_Clr (sdhost_handler,ERR_ALL);
        tmpReg |= (BIT_28|BIT_24|BIT_23|BIT_22|BIT_21|
                   BIT_20|BIT_19|BIT_18|BIT_17|BIT_16);

    }

    if (0 != (msg&SIG_CARD_IN))
    {
        //sdhost_handler->host_cfg->INT_STA =BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
        tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
        tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
        tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
        tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
        tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
        tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
        tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
        tmpReg |= BIT_0;
    }

    sdhost_handler->host_cfg->INT_STA = tmpReg;

}

/*****************************************************************************/
//  Description: Enable Normal int Signal register ,
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        that int you want to Enable
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_NML_IntStatus_En (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    uint32 err_msg;
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntStatus_En (sdhost_handler,sdhost_handler->err_filter);
        err_msg = sdhost_handler->err_filter;

        if (0 != (err_msg&ERR_RSP))
        {
            tmpReg |= BIT_28;
        }

        if (0 != (err_msg&ERR_CMD12))
        {
            tmpReg |= BIT_24;
        }

        if (0 != (err_msg&ERR_CUR_LIMIT))
        {
            tmpReg |= BIT_23;
        }

        if (0 != (err_msg&ERR_DATA_END))
        {
            tmpReg |= BIT_22;
        }

        if (0 != (err_msg&ERR_DATA_CRC))
        {
            tmpReg |= BIT_21;
        }

        if (0 != (err_msg&ERR_DATA_TIMEOUT))
        {
            tmpReg |= BIT_20;
        }

        if (0 != (err_msg&ERR_CMD_INDEX))
        {
            tmpReg |= BIT_19;
        }

        if (0 != (err_msg&ERR_CMD_END))
        {
            tmpReg |= BIT_18;
        }

        if (0 != (err_msg&ERR_CMD_CRC))
        {
            tmpReg |= BIT_17;
        }

        if (0 != (err_msg&ERR_CMD_TIMEOUT))
        {
            tmpReg |= BIT_16;
        }

    }

    if (0 != (msg&SIG_CARD_IN))
    {
        tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
        tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
        tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
        tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
        tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
        tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
        tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
        tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
        tmpReg |= BIT_0;
    }

    sdhost_handler->host_cfg->INT_STA_EN |= tmpReg;

}

/*****************************************************************************/
//  Description: Disable Normal int Signal register
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        that int you want to Disable
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_NML_IntStatus_Dis (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntStatus_Dis (sdhost_handler,ERR_ALL);
        tmpReg |= (BIT_28|BIT_24|BIT_23|BIT_22|BIT_21|
                   BIT_20|BIT_19|BIT_18|BIT_17|BIT_16);

    }

    if (0 != (msg&SIG_CARD_IN))
    {
        tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
        tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
        tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
        tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
        tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
        tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
        tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
        tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
        tmpReg |= BIT_0;
    }

    sdhost_handler->host_cfg->INT_STA_EN &= ~tmpReg;

}

/*****************************************************************************/
//  Description: Enable Normal int Signal register ,if normal event  is happed ,host will send interrupt to Arm
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        that int you want to Enable
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_NML_IntSig_En (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    uint32 err_msg;
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntSig_En (sdhost_handler,sdhost_handler->err_filter);
        err_msg = sdhost_handler->err_filter;

        if (0 != (err_msg&ERR_RSP))
        {
            tmpReg |= BIT_28;
        }

        if (0 != (err_msg&ERR_CMD12))
        {
            tmpReg |= BIT_24;
        }

        if (0 != (err_msg&ERR_CUR_LIMIT))
        {
            tmpReg |= BIT_23;
        }

        if (0 != (err_msg&ERR_DATA_END))
        {
            tmpReg |= BIT_22;
        }

        if (0 != (err_msg&ERR_DATA_CRC))
        {
            tmpReg |= BIT_21;
        }

        if (0 != (err_msg&ERR_DATA_TIMEOUT))
        {
            tmpReg |= BIT_20;
        }

        if (0 != (err_msg&ERR_CMD_INDEX))
        {
            tmpReg |= BIT_19;
        }

        if (0 != (err_msg&ERR_CMD_END))
        {
            tmpReg |= BIT_18;
        }

        if (0 != (err_msg&ERR_CMD_CRC))
        {
            tmpReg |= BIT_17;
        }

        if (0 != (err_msg&ERR_CMD_TIMEOUT))
        {
            tmpReg |= BIT_16;
        }

    }

    if (0 != (msg&SIG_CARD_IN))
    {
        tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
        tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
        tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
        tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
        tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
        tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
        tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
        tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
        tmpReg |= BIT_0;
    }

    sdhost_handler->host_cfg->INT_SIG_EN |= tmpReg;

}

/*****************************************************************************/
//  Description: Disable Normal int Signal register ,if normal event  is happed ,host will not send interrupt to Arm
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        that int you want to Disable
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_NML_IntSig_Dis (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntSig_Dis (sdhost_handler,ERR_ALL);
        tmpReg |= (BIT_28|BIT_24|BIT_23|BIT_22|BIT_21|
                   BIT_20|BIT_19|BIT_18|BIT_17|BIT_16);

    }

    if (0 != (msg&SIG_CARD_IN))
    {
        tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
        tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
        tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
        tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
        tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
        tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
        tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
        tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
        tmpReg |= BIT_0;
    }

    sdhost_handler->host_cfg->INT_SIG_EN &= ~tmpReg;

}

/*****************************************************************************/
//  Description: Get normal int status register ,to confirm which normal event has happened
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      uint32 value: indicate which event happened
//  Note:
/*****************************************************************************/
PUBLIC uint32 SDHOST_GetNMLIntStatus (SDHOST_HANDLE sdhost_handler)
{
    volatile uint32 tmpReg;
    uint32 msg;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg = sdhost_handler->host_cfg->INT_STA;
    msg = 0;

    if (0 != (tmpReg & BIT_15))
    {
        msg |= SIG_ERR;
    }

    if (0 != (tmpReg & BIT_8))
    {
        msg |= SIG_CARD_IN;
    }

    if (0 != (tmpReg & BIT_6))
    {
        msg |= SIG_CARD_INSERT;
    }

    if (0 != (tmpReg & BIT_7))
    {
        msg |= SIG_CARD_REMOVE;
    }

    if (0 != (tmpReg & BIT_5))
    {
        msg |= SIG_BUF_RD_RDY;
    }

    if (0 != (tmpReg & BIT_4))
    {
        msg |= SIG_BUF_WD_RDY;
    }

    if (0 != (tmpReg & BIT_3))
    {
        msg |= SIG_DMA_INT;
    }

    if (0 != (tmpReg & BIT_2))
    {
        msg |= SIG_BLK_CAP;
    }

    if (0 != (tmpReg & BIT_1))
    {
        msg |= SIG_TRANS_CMP;
    }

    if (0 != (tmpReg & BIT_0))
    {
        msg |= SIG_CMD_CMP;
    }

    return msg;
}


/*****************************************************************************/
//  Description: if error interrupt happened ,this function is used to confirm which error event happened
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      uint32 value: indicate which error event happened
//  Note:
/*****************************************************************************/
PUBLIC uint32 SDHOST_GetErrCode (SDHOST_HANDLE sdhost_handler)
{
    volatile uint32 tmpReg;
    uint32 err_msg;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg = sdhost_handler->host_cfg->INT_STA;
    err_msg = 0;

    if (0 != (tmpReg & BIT_28))
    {
        err_msg |= ERR_RSP;
    }

    if (0 != (tmpReg & BIT_24))
    {
        err_msg |= ERR_CMD12;
    }

    if (0 != (tmpReg & BIT_23))
    {
        err_msg |= ERR_CUR_LIMIT;
    }

    if (0 != (tmpReg & BIT_22))
    {
        err_msg |= ERR_DATA_END;
    }

    if (0 != (tmpReg & BIT_21))
    {
        err_msg |= ERR_DATA_CRC;
    }

    if (0 != (tmpReg & BIT_20))
    {
        err_msg |= ERR_DATA_TIMEOUT;
    }

    if (0 != (tmpReg & BIT_19))
    {
        err_msg |= ERR_CMD_INDEX;
    }

    if (0 != (tmpReg & BIT_18))
    {
        err_msg |= ERR_CMD_END;
    }

    if (0 != (tmpReg & BIT_17))
    {
        err_msg |= ERR_CMD_CRC;
    }

    if (0 != (tmpReg & BIT_16))
    {
        err_msg |= ERR_CMD_TIMEOUT;
    }

    return err_msg;
}

/*****************************************************************************/
//  Description: this function is used to set which error event you want to watched ,other error event will be ignored if happened
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        the event you want to watched
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetErrCodeFilter (SDHOST_HANDLE sdhost_handler,uint32 err_msg)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->err_filter = err_msg;
}

/*****************************************************************************/
//  Description: To indicate event that happened from which slot
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      uint32 value: indicate which slot event happened
//  Note:
/*****************************************************************************/
LOCAL SDHOST_SLOT_NO _GetIntSDHOSTSlotNum (void)
{
    uint32 tmpReg;
    SDHOST_SLOT_NO ret;

    tmpReg = REG32 (SDIO1_SLOT_INT_STS);

    if ( (tmpReg& (0x01<<0)))
    {
        ret = SDHOST_SLOT_0;
    }
    else if ( (tmpReg& (0x01<<1)))
    {
        ret = SDHOST_SLOT_1;
    }
    else if ( (tmpReg& (0x01<<2)))
    {
        ret = SDHOST_SLOT_2;
    }
    else if ( (tmpReg& (0x01<<3)))
    {
        ret = SDHOST_SLOT_3;
    }
    else if ( (tmpReg& (0x01<<4)))
    {
        ret = SDHOST_SLOT_4;
    }
    else if ( (tmpReg& (0x01<<5)))
    {
        ret = SDHOST_SLOT_5;
    }
    else if ( (tmpReg& (0x01<<6)))
    {
        ret = SDHOST_SLOT_6;
    }
    else if ( (tmpReg& (0x01<<7)))
    {
        ret = SDHOST_SLOT_7;
    }
    else
    {
        ret = SDHOST_SLOT_MAX_NUM;
        SCI_ASSERT (0);/*assert to do*/
    }

    return ret;
}

/*****************************************************************************/
//  Description: This function is called by interrupt service .is event happened ,this funtion will
//  clear the event and call the callback of application
//  Author: Jason.wu
//  Param
//      isrnum: the number of arm interrupt
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC ISR_EXE_T _SDHOST_IrqHandle (uint32 isrnum)
{
    ISR_Buffer_T buffer;
    SDHOST_HANDLE sdhost_handler;

    buffer.slotNum = _GetIntSDHOSTSlotNum();
    sdhost_handler = &sdio_port_ctl[buffer.slotNum];
    buffer.pSdhost_handler = &sdio_port_ctl[buffer.slotNum];

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (FALSE == sdhost_handler->open_flag)
    {
        SDHOST_NML_IntSig_Dis (sdhost_handler, SIG_ALL);
        SDHOST_NML_IntStatus_Dis (sdhost_handler, SIG_ALL);
        SDHOST_NML_IntStatus_Clr (sdhost_handler, SIG_ALL);
        return ISR_DONE;
    }

    if (NULL == sdhost_handler->sigCallBack)
    {
        SDHOST_NML_IntSig_Dis (sdhost_handler, SIG_ALL);
        SDHOST_NML_IntStatus_Dis (sdhost_handler, SIG_ALL);
        SDHOST_NML_IntStatus_Clr (sdhost_handler, SIG_ALL);
        return ISR_DONE;
    }

    buffer.msg = SDHOST_GetNMLIntStatus (sdhost_handler);
    buffer.errCode = SDHOST_GetErrCode (sdhost_handler);

    SDHOST_NML_IntSig_Dis (sdhost_handler, buffer.msg);
    SDHOST_NML_IntStatus_Dis (sdhost_handler, buffer.msg);
    SDHOST_NML_IntStatus_Clr (sdhost_handler, buffer.msg);
	if(FALSE == SCI_GetAssertFlag())
	{
        IsrWriteBuffer (buffer);
	}
  else
  {
      sdhost_handler->sigCallBack (buffer.msg, buffer.errCode, buffer.slotNum);
  }

    return CALL_HISR;
}

/*****************************************************************************/
//  Description:    This function is SDIO 's HISR.
//                  1. THE priority is higher than normal task.
//                  2. Is not real ISR.
//  Author:         steve.zhan
//  Note:
/*****************************************************************************/
LOCAL void  SdhostHisrFunc (uint32 cnt, void *pData)
{
    ISR_Buffer_T buffer;

    while (!threadReadBuffer (&buffer))
    {
        buffer.pSdhost_handler->sigCallBack (buffer.msg, buffer.errCode, buffer.slotNum);
    }
}
/*****************************************************************************/
//  Description: Regist host slot
//  Author: Jason.wu
//  Param
//      slot_NO:    which slot you want to used
//      fun:        this function will be called when event happened
//  Return:
//      Not zero: succes
//      zeror: fail
//  Note:
/*****************************************************************************/
PUBLIC SDHOST_HANDLE SDHOST_Register (SDHOST_SLOT_NO slot_NO,SDIO_CALLBACK fun)
{
    uint32 status = 0, i = 0;

    SCI_ASSERT (slot_NO < SDHOST_SLOT_MAX_NUM);/*assert verified*/

    if (TRUE == sdio_port_ctl[slot_NO].open_flag)
    {
        return NULL;
    }

    for (i = 0; i < SDHOST_SLOT_MAX_NUM; i ++)
    {
        if (TRUE == sdio_port_ctl[i].open_flag)
        {
            break;
        }
    }
#if 0
    if (SDHOST_SLOT_MAX_NUM == i)
    {

#ifndef OS_NONE
        // Get SDIO device handle, enable SDIO
        SCI_ASSERT (DEVICE_Find ("SDIO0", &s_dev_sdio) == CLOCK_SUCCESS);/*assert verified*/
        DEVICE_Open (s_dev_sdio);
        DEVICE_Reset (s_dev_sdio);
#else
        REG32 (AHB_EB0_SET) = (SDIO_EB_SET); //AHB ENABLE SDIO
        //REG32(REG_AHB_EB0) |= (AHB_EB0_SET_SDIO_EB_SET); //AHB ENABLE SDIO
        REG32 (AHB_RST0_SET) = (SDIO_SOFT_RST_SET); //AHB reset SDIO
        REG32 (AHB_RST0_CLR) = (SDIO_SOFT_RST_CLR);
#endif
    }
#endif
    sdio_port_ctl[slot_NO].open_flag = TRUE;
    SDHOST_Slot_select (slot_NO);
    //SDHOST_Pin_select (slot_NO);
    // base clk select
    /*6800H3 ahb clk = 90MHz, which is less than 2*48MHz,
     *would cause potential problem in SD Host Controller
     *what about SC6530?
    */
#if 0
    //sdio_port_ctl[slot_NO].baseClock = SDHOST_BaseClk_Set (SDIO_SD_MAX_CLK);

    switch (slot_NO)
    {
        case SDHOST_SLOT_0:
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) SDIO0_BASE_ADDR);
            }
            break;

        case SDHOST_SLOT_1:
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) SDIO1_BASE_ADDR);
            }
            break;

        case SDHOST_SLOT_2:
        case SDHOST_SLOT_3:
        case SDHOST_SLOT_4:
        case SDHOST_SLOT_5:
        case SDHOST_SLOT_6:
        case SDHOST_SLOT_7:

        default:
            {
                SCI_ASSERT (0);/*assert to do*/
            }
            break;
    }

    sdio_port_ctl[slot_NO].sigCallBack = fun;
    sdio_port_ctl[slot_NO].err_filter = 0;
    SDHOST_RST (&sdio_port_ctl[slot_NO],RST_ALL);
    SDHOST_internalClk_OnOff (&sdio_port_ctl[slot_NO],CLK_ON);
    _GetSDHOSTCapbility (&sdio_port_ctl[slot_NO],& (sdio_port_ctl[slot_NO].capbility));
    SDHOST_RST (&sdio_port_ctl[slot_NO],RST_ALL);
#if  1  //ndef OS_NONE
    status = ISR_RegHandler_Ex (TB_SDIO_INT, (TB_ISR) _SDHOST_IrqHandle, SdhostHisrFunc, CHIPDRV_HISR_PRIO_1, NULL);

    if (TB_SUCCESS == status)
    {
        CHIPDRV_EnableIRQINT (TB_SDIO_INT);
    }

#endif
#endif
    return &sdio_port_ctl[slot_NO];
}

/*****************************************************************************/
//  Description: Free the slot resource
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handler of host driver ,this is returned by SDHOST_Register
//  Return:
//      Not zero: succes
//      zeror: fail
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN SDHOST_UnRegister (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    SDHOST_SD_POWER (sdhost_handler,POWR_OFF);
    SDHOST_RST (sdhost_handler,RST_ALL);
    SDHOST_SD_clk_OnOff (sdhost_handler,CLK_OFF);
    SDHOST_internalClk_OnOff (sdhost_handler,CLK_OFF);
    sdhost_handler->sigCallBack = NULL;
    sdhost_handler->open_flag = FALSE;
    return TRUE;
}

//====================
//---External function---
//====================
/*****************************************************************************/
//  Description: Set basic clk of host,card clk will divided from this clk
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handler of host driver ,this is returned by SDHOST_Register
//      sdio_base_clk: the frequecy that you want to set
//  Return:
//      uint32 value :the frequency that be used acctually
//  Note: This function must be applied according different platform
/*****************************************************************************/
#ifndef OS_NONE
PUBLIC uint32 SDHOST_BaseClk_Set (uint32 sdio_base_clk)
{
    uint32 clk = 0;

    //Select the clk source of SDIO
    if (sdio_base_clk >= SDIO_BASE_CLK_104M)
    {
        DEVICE_SetClock (s_dev_sdio, SDIO_BASE_CLK_104M);
        clk = SDIO_BASE_CLK_104M;
    }
    else
    {
        DEVICE_SetClock (s_dev_sdio, SDIO_BASE_CLK_48M);
        clk = SDIO_BASE_CLK_48M;
    }

    return clk;
}
#else
PUBLIC uint32 SDHOST_BaseClk_Set (uint32 sdio_base_clk)
{
#if defined(CHIP_VER_6531EFM)
    uint32 clk = 0;
    uint32 tmpReg;
    //Select the clk source of SDIO
    tmpReg = REG32 (CGM_SDIO_CFG);
    tmpReg &= ~CGM_SDIO_SEL_MSK;

    if (sdio_base_clk >= SDIO_BASE_CLK_104M)
    {
        tmpReg |= 0x4 << CGM_SDIO_SEL_SHIFT;
        clk = SDIO_BASE_CLK_104M;
    }
    else
    {
        tmpReg |= 0x2 << CGM_SDIO_SEL_SHIFT;
        clk = SDIO_BASE_CLK_48M;
    }

    REG32 (CGM_SDIO_CFG) = tmpReg;
    return clk;
#else
#if defined(CHIP_VER_6531)
#define APB_CLK_CFGDEFINE	APB_CLK_CFG1
#else
#define APB_CLK_CFGDEFINE	APB_CLK_CFG0
#endif

    uint32 clk = 0;
    uint32 tmpReg;
    //Select the clk source of SDIO
    tmpReg = REG32 (APB_CLK_CFGDEFINE);
    tmpReg &= ~CLK_SDIO_SEL_MSK;

    if (sdio_base_clk >= SDIO_BASE_CLK_104M)
    {
        tmpReg |= 0x0 << CLK_SDIO_SEL_SHIFT;
        clk = SDIO_BASE_CLK_104M;
    }
    else
    {
        tmpReg |= 0x1 << CLK_SDIO_SEL_SHIFT;
        clk = SDIO_BASE_CLK_48M;
    }

    REG32 (APB_CLK_CFGDEFINE) = tmpReg;
    return clk;
#endif	
}
#endif
/*****************************************************************************/
//  Description: Switch between available basic clk of host,card clk will divided from this clk
//  Author: Wenjun.Shi
//  Param
//      slot_NO:    which slot you want to used
//      sdio_base_clk: the frequecy that you want to set
//  Return:
//      uint32 value :the frequency that be used acctually
//  Note: This function must be applied according different platform
/*****************************************************************************/
PUBLIC uint32 SDHOST_BaseClk_Switch (SDHOST_SLOT_NO slot_NO, uint32 sdio_base_clk)
{
    uint32 clk = 0;
    SCI_ASSERT (slot_NO < SDHOST_SLOT_MAX_NUM);/*assert verified*/
    if ( sdio_port_ctl[slot_NO].baseClock != sdio_base_clk )
    {
        clk = sdio_port_ctl[slot_NO].baseClock = SDHOST_BaseClk_Set (sdio_base_clk);
    }
    return clk;
}

/*****************************************************************************/
//  Description: Config sdio pins according to specified parameter.
//  Author: Wenjun.Shi
//  Param
//      PIN_SD_NAME_E:    which pin you want to used
//      PIN_SD_CFG_E: the parameter that you want to set
//  Return:
//      none
//  Note: This function must be applied according different platform
/*****************************************************************************/
LOCAL void _SDHOST_SetPad (PIN_SD_NAME_E pin_sd_name, PIN_SD_CFG_E pin_sd_cfg)
{
    uint32 i = 0;
    uint32 pin_reg_val;
    SCI_ASSERT (pin_sd_name < PIN_SD_MAX);/*assert verified*/

    for(i = 0; i < PIN_SD_INFO_MAX; i++)
    {
        if ( (uint32)(pin_sd_info[i].pin_sd_name) == PIN_SD_INFO_INVALID )
        {
            break;
        }
        if ( pin_sd_info[i].pin_sd_name == pin_sd_name )
        {
            switch (pin_sd_cfg)
            {
                case PIN_SD_PULL_UP:
                    pin_reg_val = CHIP_REG_GET(pin_sd_info[i].addr);
                    pin_reg_val &= ~(PIN_FPD_EN | PIN_FPU_EN);
                    pin_reg_val |= PIN_FPU_EN;
                    CHIP_REG_SET(pin_sd_info[i].addr, pin_reg_val);
                break;
                case PIN_SD_PULL_DOWN:
                    pin_reg_val = CHIP_REG_GET(pin_sd_info[i].addr);
                    pin_reg_val &= ~(PIN_FPD_EN | PIN_FPU_EN);
                    pin_reg_val |= PIN_FPD_EN;
                    CHIP_REG_SET(pin_sd_info[i].addr, pin_reg_val);
                break;
                default:
                SCI_ASSERT(0);/*assert to do*/
            }
            break;
        }
    }
}

/*****************************************************************************/
//  Description: Restore sdio pins config parameter according to specified parameter.
//  Author: Wenjun.Shi
//  Param
//      PIN_SD_NAME_E:    which pin you want to used
//  Return:
//      none
//  Note: This function must be applied according different platform
/*****************************************************************************/
LOCAL void _SDHOST_RestorePad (PIN_SD_NAME_E pin_sd_name)
{
    uint32 i = 0;
    SCI_ASSERT (pin_sd_name < PIN_SD_MAX);/*assert verified*/
    
    for(i = 0; i < PIN_SD_INFO_MAX; i++)
    {   
        if ( (uint32)(pin_sd_info[i].pin_sd_name) == PIN_SD_INFO_INVALID )
        {
            break;
        }
        if ( pin_sd_info[i].pin_sd_name == pin_sd_name )
        {
            CHIP_REG_SET(pin_sd_info[i].addr, pin_sd_info[i].pin_reg_reserved);
            break;
        }
    }
}

/*****************************************************************************/
//  Description: Save sdio pins config parameter.
//  Author: Wenjun.Shi
//  Param
//  Return:
//      none
//  Note: This function must be applied according different platform
/*****************************************************************************/
PUBLIC void SDHOST_SavePad (void)
{
    uint32 i = 0;
    
    for(i = 0; i < PIN_SD_INFO_MAX; i++)
    {   
        if ( (uint32)(pin_sd_info[i].pin_sd_name) == PIN_SD_INFO_INVALID )
        {
            break;
        }
        pin_sd_info[i].pin_reg_reserved = CHIP_REG_GET(pin_sd_info[i].addr);
    }
}
/*****************************************************************************/
//  Description: Set sdio pins to logical '0' before power down.
//  Author: Wenjun.Shi
//  Param
//      slot_NO: slot number
//      PIN_SD_STS_E: pin status to be set
//  Return:
//      none
//  Note: This function must be applied according different platform
/*****************************************************************************/
PUBLIC void SDHOST_SetPad_Status (SDHOST_SLOT_NO slot_NO, PIN_SD_STS_E pin_sd_sts)
{
    uint32 i;
    SCI_ASSERT (slot_NO < SDHOST_SLOT_MAX_NUM);/*assert verified*/
    if ( PAD_SD_STS_LOW_LEVEL_BEFORE_POWR_DOWN == pin_sd_sts )
    {
         switch (slot_NO)
         {
            case SDHOST_SLOT_0:
            _SDHOST_SetPad(PIN_SD_CLK0, PIN_SD_PULL_DOWN);
            break;
            case SDHOST_SLOT_1:
            _SDHOST_SetPad(PIN_SD_CLK1, PIN_SD_PULL_DOWN);
            break;
            default:
            SCI_ASSERT(0);/*assert to do*/
         }
        _SDHOST_SetPad(PIN_SD_CMD, PIN_SD_PULL_DOWN);
        _SDHOST_SetPad(PIN_SD_D0, PIN_SD_PULL_DOWN);
        _SDHOST_SetPad(PIN_SD_D1, PIN_SD_PULL_DOWN);
        _SDHOST_SetPad(PIN_SD_D2, PIN_SD_PULL_DOWN);
        _SDHOST_SetPad(PIN_SD_D3, PIN_SD_PULL_DOWN);
    }
    else if ( PAD_SD_STS_RESTORE_BEFORE_POWR_UP == pin_sd_sts )
    {
         switch (slot_NO)
         {
            case SDHOST_SLOT_0:
           // _SDHOST_RestorePad(PIN_SD_CLK0);
	     _SDHOST_SetPad(PIN_SD_CLK0, PIN_SD_PULL_UP);		
            break;
            case SDHOST_SLOT_1:
            //_SDHOST_RestorePad(PIN_SD_CLK1);
	    _SDHOST_SetPad(PIN_SD_CLK1, PIN_SD_PULL_UP);
            break;
            default:
            SCI_ASSERT(0);/*assert to do*/
         }
        //_SDHOST_RestorePad(PIN_SD_CMD);
        //_SDHOST_RestorePad(PIN_SD_D0);
        //_SDHOST_RestorePad(PIN_SD_D1);
        //_SDHOST_RestorePad(PIN_SD_D2);
        //_SDHOST_RestorePad(PIN_SD_D3);
        _SDHOST_SetPad(PIN_SD_CMD, PIN_SD_PULL_UP);
        _SDHOST_SetPad(PIN_SD_D0, PIN_SD_PULL_UP);
        _SDHOST_SetPad(PIN_SD_D1, PIN_SD_PULL_UP);
        _SDHOST_SetPad(PIN_SD_D2, PIN_SD_PULL_UP);
        _SDHOST_SetPad(PIN_SD_D3, PIN_SD_PULL_UP);
		
    }
    for (i = 0; i < 100; i++){;}
}

/*****************************************************************************/
//  Description: select witch slot to work
//  Author: Jason.wu
//  Param
//      slot_NO: slot number
//  Return:
//      NONE
//  Note: This function must be applied according different platform
/*****************************************************************************/
PUBLIC void SDHOST_Slot_select (SDHOST_SLOT_NO slot_NO)
{
    uint32 tmpReg;
    return;
    SCI_ASSERT (slot_NO < SDHOST_SLOT_MAX_NUM);/*assert verified*/

#if defined(CHIP_VER_6530)
    switch (slot_NO)
    {
        case SDHOST_SLOT_0:
            {
                tmpReg = * (volatile uint32 *) AHB_SYS_CTL0;
                tmpReg &= ~ (BIT_2|BIT_3);
                * (volatile uint32 *) AHB_SYS_CTL0 = tmpReg;
            }
            break;

        case SDHOST_SLOT_1:
            {
                tmpReg = * (volatile uint32 *) AHB_SYS_CTL0;
                tmpReg &= ~ (BIT_2|BIT_3);
                tmpReg |= (BIT_2);
                * (volatile uint32 *) AHB_SYS_CTL0 = tmpReg;
            }
            break;

        case SDHOST_SLOT_2:
        case SDHOST_SLOT_3:
        case SDHOST_SLOT_4:
        case SDHOST_SLOT_5:
        case SDHOST_SLOT_6:
        case SDHOST_SLOT_7:

        default:
            {
                SCI_ASSERT (0); /*assert to do*/
            }
            break;
    }
#elif defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
    switch (slot_NO)
    {
        case SDHOST_SLOT_0:
            {
                tmpReg = * (volatile uint32 *) AHB_SYS_CTL0;
                tmpReg &= ~SDIO_SLOT_SEL_MSK;
                * (volatile uint32 *) AHB_SYS_CTL0 = tmpReg;
            }
            break;

        case SDHOST_SLOT_1:
            {
                tmpReg = * (volatile uint32 *) AHB_SYS_CTL0;
                tmpReg &= ~SDIO_SLOT_SEL_MSK;
                tmpReg |= (BIT_0);
                * (volatile uint32 *) AHB_SYS_CTL0 = tmpReg;
            }
            break;

        case SDHOST_SLOT_2:
        case SDHOST_SLOT_3:
        case SDHOST_SLOT_4:
        case SDHOST_SLOT_5:
        case SDHOST_SLOT_6:
        case SDHOST_SLOT_7:

        default:
            {
                SCI_ASSERT (0); /*assert to do*/
            }
            break;
    }
#endif
}

//===end===
