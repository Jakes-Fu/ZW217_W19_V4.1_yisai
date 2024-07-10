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


#include "hal_gouda.h"
#include "gouda.h"
#include "lcm_cfg.h"
#include "chip_plf_export.h"
#include "sys_ctrl.h"
//#include "iomux_6121e.h"
#include "pinmap.h"
#include "ap_apb.h"

#ifdef TF_FDL_BUILD
#define SCI_TraceLow rprintf
#endif
void hal_LcdDelay (int ms_delay);
void gouda_event(uint32 e)
{
  // mon_Event(e);
}

//#define GOUDA_Delay( ticks) OS_TickDelay( ticks )

void GOUDA_Delay (int ticks)
{
    ticks *= 1000;

    while(ticks > 0) ticks--;
}

uint32 hal_SysEnterCriticalSection(void)
{
    SCI_DisableIRQ();
    //SCI_DisableFIQ();
    return 0;
}
void hal_SysExitCriticalSection(uint32 status)
{
    SCI_RestoreIRQ();
    //SCI_RestoreFIQ();
}

// ============================================================================
//  DEFINES
// ============================================================================

//#define  HAL_ASSERT   SCI_PASSERT


#define DEFERRED_TIMINGS_SET




HAL_GOUDA_SPI_LINE_T g_line_type;
uint16 g_gouda_fmark = 0;
uint32 g_gouda_fmark_delay = 0x10000;


#define LCDD_SPI_CAM_PREV_FREQ (30000000)
#define LCDD_SPI_FREQ (30000000)
#define LCDD_SPI_FREQ_READ (500000)


typedef struct __gouda_context
{
    //HAL_CLK_T *lcdClk;
    BOOLEAN goudaOpened;
    BOOLEAN fmarkBusy;
    HAL_GOUDA_IRQ_HANDLER_T userIrqHandler;
} goudaContext_t;

// ============================================================================
//  GLOBAL VARIABLES
// ============================================================================
static goudaContext_t g_gouda_ctx =
{
    //.lcdClk = NULL,
    /*.goudaOpened = */FALSE,
    /*.fmarkBusy = */FALSE,
    /*.userIrqHandler = */NULL,
};

/// Store the LCD configuration from tgt to add command value in blit
LOCAL HAL_GOUDA_LCD_CONFIG_T g_halGoudaLcdConfig;
LOCAL uint32 g_halGoudaEbcCsAddr;

/// Store the timings
LOCAL CONST HAL_GOUDA_LCD_TIMINGS_T* g_halGoudaLcdTimings = NULL;

/// Store the serial LCD configuration from tgt to
/// add command value in blit
LOCAL HAL_GOUDA_SERIAL_LCD_CONFIG_T g_halGoudaSlcdCfg = { /*.reg = */0 };
/// Store the SPI frequency for the serial LCD
LOCAL uint32 g_halGoudaSpiFreq = 0;
LOCAL uint32 g_halGoudaSpiFreqDivider = 0;


LOCAL uint32 g_halGoudaBlock = 1;

// =============================================================================
// g_halGoudaIrqHandler
// -----------------------------------------------------------------------------
/// Variable to store the user irq handler for the lcdc
/// interruption.
// =============================================================================


// ============================================================================
//  FUNCTIONS
// ============================================================================

void hal_GoudaSetFmarkBusy(BOOLEAN busy)
{
   //gouda_event(0x90000001);

    g_gouda_ctx.fmarkBusy = busy;
}

// =============================================================================
// hal_GoudaUpdateTimings
// -----------------------------------------------------------------------------
/// Update the timings used by the Gouda module to fit optimally with the
/// system frequency. After the call to the #hal_GoudaOpen() function, an
/// array of timings, one for each system frequency, is registered in to the
/// HAL GOUDA LCD driver.
/// @param sysFreq (New) System frequency for which new timings must be
/// configured.
// =============================================================================
uint32 hal_GoudaUpdateTimings(uint32 sysFreq)
{
   //gouda_event(0x90000002);
    return sysFreq;
}


// =============================================================================
// hal_GoudaUpdateSerialTiming
// -----------------------------------------------------------------------------
/// Update the timing used by the Gouda module to fit optimally with the
/// system frequency. After the call to the #hal_GoudaOpen() function,
/// the timing is registered in to the HAL GOUDA LCD driver.
/// @param sysFreq (New) System frequency for which new timing must be
/// configured.
// =============================================================================
uint32 hal_GoudaUpdateSerialTiming(uint32 sysFreq)
{
    uint32 status;
    uint32 divider = 10;

    if (g_halGoudaSpiFreq == 0)
        return 0;
   //gouda_event(0x9f000003);

    status = hal_SysEnterCriticalSection();
/*
    divider = sysFreq / g_halGoudaSpiFreq;
    if (divider == 0)
    {
        divider = 1;
    }
    else if (divider > GD_MAX_SLCD_CLK_DIVIDER)
    {
        divider = GD_MAX_SLCD_CLK_DIVIDER;
    }

    if(HAL_SYS_FREQ_139M == sysFreq)//fm using this freq.
    {
        divider += 1;
    }
*/
    if (g_halGoudaSpiFreq == LCDD_SPI_FREQ_READ)
    {
        divider = 250;
    }
    else if(g_halGoudaSpiFreq == LCDD_SPI_FREQ)
    {
	    if(g_gouda_fmark == ENABLE_FMARK_LCD)
	    {
	    divider =5;
	    hwp_iomux_6121e->pad_spi_lcd_clk &= ~(((15)&0xF)<<19) ;
	    hwp_iomux_6121e->pad_spi_lcd_clk |= (0x6)<<19;

	    hwp_iomux_6121e->pad_spi_lcd_sio &= ~(((15)&0xF)<<19) ;
	    hwp_iomux_6121e->pad_spi_lcd_sio |= (0x6)<<19;

	    hwp_iomux_6121e->pad_spi_lcd_sdc &= ~(((15)&0xF)<<19) ;
	    hwp_iomux_6121e->pad_spi_lcd_sdc |= (0x6)<<19;
	    }
	    else
	    {
		    divider =5; // 200Mhz  / divider
	    }
	SCI_TRACE_LOW("RUI.ZHANG LCDD_SPI_FREQ");
    }
    else
        divider = 250;

    g_halGoudaSpiFreqDivider = divider;
    g_halGoudaSlcdCfg.b.clkDivider = divider;

    hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;

    hal_SysExitCriticalSection(status);
   //gouda_event(0x9f000aa3);
  // gouda_event(hwp_gouda->gd_spilcd_config);
    return g_halGoudaSpiFreq;
}


void hal_GoudaChangeClkWhenRecord(BOOLEAN record)
{
    uint32 status;
	return;
#if 0
	if(g_line_type == HAL_GOUDA_SPI_LINE_3_2_LANE)
	{
		status = hal_SysEnterCriticalSection();
		if(record == SCI_TRUE)
		{
			g_halGoudaSlcdCfg.b.clkDivider = 6;
		}
		else
		{
			g_halGoudaSlcdCfg.b.clkDivider = g_halGoudaSpiFreqDivider;

		}
		hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;
		hal_SysExitCriticalSection(status);
	}
#endif
}


// =============================================================================
// hal_GoudaOpen
// -----------------------------------------------------------------------------
/// Common operations to open Gouda for a normal LCD or a serial LCD.
/// @param handler Function to update Gouda timings.
// =============================================================================
PUBLIC void hal_GoudatResetLcdPin(void);
LOCAL void hal_GoudaCommonOpen(CONST HAL_GOUDA_LCD_CONFIG_T* config, uint32 ebcCsAddr, HAL_SYS_FREQ_CHANGE_HANDLER_T handler)
{
    gouda_event(0x90000004);

    // Power on LCD
    //pmd_EnablePower(PMD_POWER_LCD, TRUE);

    handler(78000000);

    // Reset LCD after requesting freq since it cannot be interrupted by deep sleep
    hal_GoudatResetLcdPin();

    // This describes the way the LCD is plugged to the
    // controller.
    g_halGoudaLcdConfig.reg = config->reg;
    hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg;

    g_halGoudaEbcCsAddr = ebcCsAddr;

    hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;

    if(g_gouda_fmark == ENABLE_FMARK_LCD)
    {
        hwp_gouda->TECON = GOUDA_TE_EN|GOUDA_TE_EDGE_SEL;
        hwp_gouda->TECON2 = GOUDA_TE_COUNT1(g_gouda_fmark_delay); //GOUDA_TE_COUNT1(0x10000);
    }
    else
    {
        hwp_gouda->TECON = 0;
    }
}


// =============================================================================
// hal_GoudaOpen
// -----------------------------------------------------------------------------
/// Open the LCD controller of Gouda
/// Parameters are used to describe the screen connexion configuration and
/// the controller timings to use depending on the system clock frequency.
/// @param config Pointer to the controller configuration.
/// @param timings Controller timing array (must be provided and valid,
///   even if unused in case of #HAL_GOUDA_LCD_MEMORY_IF)
/// @param ebcCsAddr address of ebc CS for #HAL_GOUDA_LCD_MEMORY_IF,
///   can be 0 for gouda CS.
// =============================================================================

PUBLIC void hal_GoudaOpen(CONST HAL_GOUDA_LCD_CONFIG_T* config, CONST HAL_GOUDA_LCD_TIMINGS_T* timings, uint32 ebcCsAddr)
{
   gouda_event(0x90000005);

    // Set configuration register
    g_halGoudaSlcdCfg.b.isSlcd = FALSE;
    hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;

    // Register the timings
    g_halGoudaLcdTimings = timings;
    //hal_GoudaBlitRoi block default
    g_halGoudaBlock=1;

    // Common open operations
    hal_GoudaCommonOpen(config, ebcCsAddr, hal_GoudaUpdateTimings);

    g_gouda_ctx.goudaOpened = TRUE;
}

PUBLIC void hal_GoudaSetSpiFreq(uint32 spiFreq)
{
    g_halGoudaSpiFreq = spiFreq;
    hal_GoudaUpdateSerialTiming(spiFreq);//hal_SysGetFreq());
}

// =============================================================================
// hal_GoudaSerialOpen
// -----------------------------------------------------------------------------
/// Open the serial LCD controller of Gouda
/// Parameters are used to describe the screen connexion configuration and
/// the controller timings to use depending on the system clock frequency.
/// @param spiLineType The SPI line type of the serial LCD.
/// @param spiFreq The SPI frequency of the serial LCD.
/// @param config Pointer to the controller configuration.
/// @param ebcCsAddr address of ebc CS for #HAL_GOUDA_LCD_MEMORY_IF,
///   can be 0 for gouda CS.
// =============================================================================

PUBLIC void hal_SetGoudaClock(BOOLEAN bAuto)
{

    if (bAuto)
        hwp_apApb->clk_ap_mode1_clr = AP_APB_CLK_MODE_GOUDA;
    else
    {
        hwp_apApb->clk_ap_en1_set = AP_APB_CLK_EN_GOUDA;
        hwp_apApb->clk_ap_mode1_set = AP_APB_CLK_MODE_GOUDA;
    }
}

PUBLIC void hal_GoudaSerialOpen(HAL_GOUDA_SPI_LINE_T spiLineType, uint32 spiFreq, CONST HAL_GOUDA_LCD_CONFIG_T* config, uint32 ebcCsAddr)
{
    // TODO Add parameter validity check.
    // Set configuration register
    g_halGoudaSlcdCfg.b.isSlcd = TRUE;
    g_halGoudaSlcdCfg.b.clkDivider = 10;
    g_halGoudaSlcdCfg.b.lineType = spiLineType;
    g_halGoudaSlcdCfg.b.isRead = FALSE;
    hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;

    // Register the timings
    g_halGoudaSpiFreq = spiFreq;

    hal_GoudaCommonOpen(config, ebcCsAddr, hal_GoudaUpdateSerialTiming);
    g_gouda_ctx.goudaOpened = TRUE;
     hal_GoudaSetSpiFreq(spiFreq);
}

PUBLIC void hal_GoudaSerialMode(HAL_GOUDA_SPI_LINE_T spiLineType)
{
   gouda_event(0x90000008);

    if(spiLineType == HAL_GOUDA_SPI_LINE_3_2_LANE)
    {
        hwp_gouda->gd_spilcd_config = (hwp_gouda->gd_spilcd_config&(~GOUDA_SPI_LINE_MASK))|GOUDA_SPI_LINE_3_TWO_LANE;
    }
    else if(spiLineType == HAL_GOUDA_SPI_LINE_3)
    {
        hwp_gouda->gd_spilcd_config = (hwp_gouda->gd_spilcd_config&(~GOUDA_SPI_LINE_MASK))|GOUDA_SPI_LINE_3;
    }
    else if(spiLineType == HAL_GOUDA_SPI_LINE_4)
    {
        hwp_gouda->gd_spilcd_config = (hwp_gouda->gd_spilcd_config&(~GOUDA_SPI_LINE_MASK))|GOUDA_SPI_LINE_4;
    }
}

PUBLIC void hal_GoudaChangeSerialMode(HAL_GOUDA_SPI_LINE_T spiLineType)
{
   gouda_event(0x9f000008);

    if(spiLineType == HAL_GOUDA_SPI_LINE_3_2_LANE)
    {
        hwp_gouda->gd_spilcd_config = (hwp_gouda->gd_spilcd_config&(~GOUDA_SPI_LINE_MASK))|GOUDA_SPI_LINE_3_TWO_LANE;
    }
    else if(spiLineType == HAL_GOUDA_SPI_LINE_3)
    {
        hwp_gouda->gd_spilcd_config = (hwp_gouda->gd_spilcd_config&(~GOUDA_SPI_LINE_MASK))|GOUDA_SPI_LINE_3;
    }
    else if(spiLineType == HAL_GOUDA_SPI_LINE_4)
    {
        hwp_gouda->gd_spilcd_config = (hwp_gouda->gd_spilcd_config&(~GOUDA_SPI_LINE_MASK))|GOUDA_SPI_LINE_4;
    }
}

// =============================================================================
// hal_GoudaClose
// -----------------------------------------------------------------------------
/// Close Gouda.
// =============================================================================
PUBLIC void hal_GoudaClose(void)
{
   gouda_event(0x90000009);

    while(hal_GoudaIsActive());
    // FIXME Is this enough ?
    g_halGoudaSlcdCfg.reg = 0;
    hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;

    g_halGoudaLcdConfig.reg = 0;
    hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg;
    g_gouda_ctx.userIrqHandler = NULL;

   // hal_ClkDisable(g_gouda_ctx.lcdClk);
/*
    // Output low level on LCD reset pin to avoid leaking current after powering off LCD
    if (g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_IO ||
            g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_O)
    {
        hal_GpioClr(g_halCfg->goudaCfg.lcdResetPin);
    }
*/
    // Power off LCD
    //pmd_EnablePower(PMD_POWER_LCD, FALSE);
    g_gouda_ctx.goudaOpened = FALSE;
}


// =============================================================================
// hal_GoudaWriteReg
// -----------------------------------------------------------------------------
/// Write a command and then a data to the LCD screen (Or a value at a register
/// address, depending on the point of view).
///
/// @param addr Address of the register to write, or command to send
/// to the LCD screen
/// @param data Corresponding data to write to the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the GOUDA LCD controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaWriteReg(uint16 addr, uint16 data)
{
    uint32 status;
    HAL_ERR_T errorStatus = HAL_ERR_NO;

    DEFERRED_TIMINGS_SET;
   //gouda_event(0x9000000d);
    status = hal_SysEnterCriticalSection();

    if ((hwp_gouda->gd_status & GOUDA_LCD_BUSY) != 0)
    {
        // GOUDA LCD is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Set configuration register and commands
        hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg; // from tgt
        // GOUDA LCD is available, we use it

        hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;

        hwp_gouda->gd_lcd_single_access=GOUDA_START_WRITE|GOUDA_LCD_DATA(addr);

        // Unavoidable wait for the previous
        // command to be sent.
        while((hwp_gouda->gd_status & GOUDA_LCD_BUSY));
        hwp_gouda->gd_lcd_single_access=GOUDA_START_WRITE|GOUDA_TYPE|GOUDA_LCD_DATA(data);

        errorStatus = HAL_ERR_NO;
    }

    hal_SysExitCriticalSection(status);
   //gouda_event(0x9000009d);

    return errorStatus;
}



// =============================================================================
// hal_GoudaReadData
// -----------------------------------------------------------------------------
/// Read a data from the LCD screen (Or a value at a register
/// address, depending on the point of view).
///
/// @param addr Address of the register to read
/// @param pData Pointer of the corresponding data to read from the LCD.
/// @param len Data length to read from the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the GOUDA LCD controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaReadData(uint16 addr, uint8 *pData, uint32 len)
{
    uint32 status;

    int i;
    HAL_ERR_T errorStatus = HAL_ERR_NO;

    DEFERRED_TIMINGS_SET;
   gouda_event(0x9000000e);

    if (g_halGoudaSlcdCfg.b.isSlcd)
    {
          gouda_event(0x900000ee);
		//HAL_ASSERT(len <= GD_MAX_SLCD_READ_LEN, "Invalid SLCD read length: %d (max: %d)",len, GD_MAX_SLCD_READ_LEN);
    }
    status = hal_SysEnterCriticalSection();

    if ((hwp_gouda->gd_status & GOUDA_LCD_BUSY) != 0)
    {
        // GOUDA LCD is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Set configuration register and commands
        hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg; // from tgt
        // GOUDA LCD is available, we use it
        hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;

        if (g_halGoudaSlcdCfg.b.isSlcd)
        {
            HAL_GOUDA_SERIAL_LCD_CONFIG_T slcdCfg;
            slcdCfg.reg = g_halGoudaSlcdCfg.reg;
            slcdCfg.b.rxByte = len;
            slcdCfg.b.isRead = TRUE;


			slcdCfg.b.dummyCycle = 1;



			if((g_line_type == HAL_GOUDA_SPI_LINE_3_2_LANE)||(g_line_type == HAL_GOUDA_SPI_LINE_3))
			{
				slcdCfg.b.lineType = HAL_GOUDA_SPI_LINE_3;
			}
			else
			{
				slcdCfg.b.lineType = HAL_GOUDA_SPI_LINE_4;

			}
			slcdCfg.b.clkDivider = 250;




            hwp_gouda->gd_spilcd_config = slcdCfg.reg;
        }

        // Send register address
        hwp_gouda->gd_lcd_single_access=GOUDA_START_WRITE|GOUDA_LCD_DATA(addr);
        while((hwp_gouda->gd_status & GOUDA_LCD_BUSY));

        if (g_halGoudaSlcdCfg.b.isSlcd)
        {
            i = 0;
            while(i < len)
            {
                pData[i] = (hwp_gouda->gd_spilcd_rd>>(8*i)) & 0xff;
                i++;
            }
            // Recover the original register context
            hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;
        }
        else
        {
            // Start to read
            hwp_gouda->gd_lcd_single_access=GOUDA_START_READ|GOUDA_TYPE;
            while((hwp_gouda->gd_status & GOUDA_LCD_BUSY));

            // Get the data
            i = 0;
            while (i < len)
            {
                if (1)//g_halCfg->goudaCfg.lcdData16Bit)
                {
                    pData[i] = hwp_gouda->gd_lcd_single_access & 0xff;
                    pData[i+1] = (hwp_gouda->gd_lcd_single_access>>8) & 0xff;
                    i += 2;
                }
                else
                {
                    pData[i] = hwp_gouda->gd_lcd_single_access & 0xff;
                    i++;
                }
                hwp_gouda->gd_lcd_single_access=GOUDA_START_READ|GOUDA_TYPE;
                while((hwp_gouda->gd_status & GOUDA_LCD_BUSY));
            }
        }

        errorStatus = HAL_ERR_NO;
    }

    hal_SysExitCriticalSection(status);
    return errorStatus;
}


// =============================================================================
// hal_GoudaReadReg
// -----------------------------------------------------------------------------
/// Read a data from the LCD screen (Or a value at a register
/// address, depending on the point of view).
///
/// @param addr Address of the register to read
/// @param pData Pointer of the corresponding data to read from the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the GOUDA LCD controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaReadReg(uint16 addr, uint16 *pData)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    uint32 len;
   //gouda_event(0x9000000f);
    *pData = 0;
    if (g_halGoudaSlcdCfg.b.isSlcd)
    {
        len = 1;
    }
    else
    {
        len = 2;
    }

    errorStatus = hal_GoudaReadData(addr, (uint8 *)pData, len);
    if (errorStatus == HAL_ERR_NO)
    {
        // Construct 16-bit register content for 8-bit interface
        if(1)// (!g_halCfg->goudaCfg.lcdData16Bit)
        {
            // Swap the byte order
            *pData = (((*pData)&0xff)<<8) | (((*pData)>>8)&0xff);
        }
    }

    return errorStatus;
}

PUBLIC HAL_ERR_T hal_GoudaReadRegLength(uint16 addr, uint8 *pData,uint8 length)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    uint32 len;
   gouda_event(0x90000010);

    *pData = 0;
    if (g_halGoudaSlcdCfg.b.isSlcd)
    {
        len = 1;
    }
    else
    {
        len = length;
    }

    errorStatus = hal_GoudaReadData(addr, (uint8 *)pData, len);

    return errorStatus;
}


// =============================================================================
// hal_GoudaWriteCmd
// -----------------------------------------------------------------------------
/// Write a command to the LCD.
///
/// @param addr Command to write.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaWriteCmd(uint16 addr)
{
    uint32 status;
    HAL_ERR_T errorStatus = HAL_ERR_NO;

    DEFERRED_TIMINGS_SET;
    status = hal_SysEnterCriticalSection();

    if ((hwp_gouda->gd_status & GOUDA_LCD_BUSY) != 0)
    {
        // GOUDA LCD is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Set configuration register and commands
        hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg; // from tgt
        // GOUDA LCD is available, we use it
        hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;
        hwp_gouda->gd_lcd_single_access=GOUDA_START_WRITE|GOUDA_LCD_DATA(addr);
        errorStatus = HAL_ERR_NO;
    }

    hal_SysExitCriticalSection(status);
    return errorStatus;
}

// =============================================================================
// hal_GoudaWriteData
// -----------------------------------------------------------------------------
/// Write a data to the LCD screen.
///
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaWriteData(uint16 data)
{
    uint32 status;
    HAL_ERR_T errorStatus = HAL_ERR_NO;

    DEFERRED_TIMINGS_SET;
   //gouda_event(0x90000012);
    status = hal_SysEnterCriticalSection();

    if ((hwp_gouda->gd_status & GOUDA_LCD_BUSY) != 0)
    {
        // GOUDA LCD is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Set configuration register and commands
        hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg; // from tgt
        // GOUDA LCD is available, we use it
        hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;
        hwp_gouda->gd_lcd_single_access=GOUDA_START_WRITE|GOUDA_TYPE|GOUDA_LCD_DATA(data);
        errorStatus = HAL_ERR_NO;
    }

    hal_SysExitCriticalSection(status);
    return errorStatus;
}

// =============================================================================
// hal_GoudaBlitRoi
// -----------------------------------------------------------------------------
/// Send a block of pixel data to the LCD screen.
///
/// @param pRoi Pointer to the roi description to send to the screen.
/// @param nbCmd number of commands to send to LCD before pixel data
/// @param pCmds commands to send to LCD before pixel data can be \c NULL if \p nbCmd is 0
/// @param handler If not NULL, user function that is called when the transfer
/// is finished.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaBlitRoi(CONST HAL_GOUDA_WINDOW_T* pRoi, CONST uint32 nbCmd, CONST HAL_GOUDA_LCD_CMD_T* pCmds, HAL_GOUDA_IRQ_HANDLER_T handler)
{
    uint32 status;
    DEFERRED_TIMINGS_SET;
    //gouda_event(0x90000013);
    // check Ready
    status = hal_SysEnterCriticalSection();

    if ((hwp_gouda->gd_status & (GOUDA_IA_BUSY | GOUDA_LCD_BUSY)) != 0)
    {
        // GOUDA is busy, abort
        hal_SysExitCriticalSection(status);
        return HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        uint32 i;
        HAL_GOUDA_LCD_CONFIG_T goudaLcdConfig;
        // GOUDA LCD is available, we use it

        hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;

        // configure ROI in Gouda
        hwp_gouda->gd_roi_tl_ppos = GOUDA_X0(pRoi->tlPX) | GOUDA_Y0(pRoi->tlPY);
        hwp_gouda->gd_roi_br_ppos = GOUDA_X1(pRoi->brPX) | GOUDA_Y1(pRoi->brPY);

        // Set configuration register and commands
        goudaLcdConfig.reg = g_halGoudaLcdConfig.reg; // from tgt
        goudaLcdConfig.b.nbCmd = nbCmd; // from driver
        hwp_gouda->gd_lcd_ctrl = goudaLcdConfig.reg;
        for(i = 0; i < nbCmd; i++)
        {
            // it usses addressing to configure the command/data bit in Gouda
            if(pCmds[i].isData)
            {
                // datas are after workbuf and commands
                ((uint16*)hwp_goudaSram)[GD_NB_WORKBUF_WORDS+GD_NB_LCD_CMD_WORDS+i] = pCmds[i].value;
            }
            else
            {
                // commands are after workbuf
                ((uint16*)hwp_goudaSram)[GD_NB_WORKBUF_WORDS+i] = pCmds[i].value;
            }
        }

        // Record handler and enable interrupt if needed.
        g_gouda_ctx.userIrqHandler = handler;
        if (handler != NULL)
        {
            // Set Mask
            hwp_gouda->gd_eof_irq_mask = GOUDA_EOF_MASK;
        }
        else
        {
            // Disable IRQ
            hwp_gouda->gd_eof_irq_mask = 0;
        }

        // Run GOUDA
        hwp_gouda->gd_command = GOUDA_START;

        // that's it
        hal_SysExitCriticalSection(status);

        return HAL_ERR_NO;
    }
}

// =============================================================================
// hal_GoudaBlitRoi2Ram
// -----------------------------------------------------------------------------
/// Send a block of pixel data to a flat ram buffer in RGB565 format
/// WARNING: only even pixel number are supported, that is to say size must
/// be a mutiple of 4 bytes.
///
/// @param pRoi Pointer to the roi description to send to the screen.
/// @param handler If not NULL, user function that is called when the transfer
/// is finished.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaBlitRoi2Ram(uint32* pBuffer, CONST uint16 width, CONST HAL_GOUDA_WINDOW_T* pRoi, HAL_GOUDA_IRQ_HANDLER_T handler)
{
    uint32 status;
    int32 offset = 0; // default to 0 when width == 0

    DEFERRED_TIMINGS_SET;
   //gouda_event(0x90000014);

    // compute offset
    if (width != 0)
    {
        int32 roiWidth=pRoi->brPX - pRoi->tlPX + 1;
        //HAL_ASSERT(width >= roiWidth, "hal_GoudaBlitRoi2Ram width (%d) must be greater or equal to width of roi (%d)", width, roiWidth);
        // offset is in byte
        offset = (width-roiWidth)*2;
    }

    // check Ready
    status = hal_SysEnterCriticalSection();

    if ((hwp_gouda->gd_status & (GOUDA_IA_BUSY | GOUDA_LCD_BUSY)) != 0)
    {
        // GOUDA is busy, abort
        hal_SysExitCriticalSection(status);
        return HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        HAL_GOUDA_LCD_CONFIG_T goudaLcdConfig;
        // GOUDA LCD is available, we use it

        // configure ROI in Gouda
        hwp_gouda->gd_roi_tl_ppos = GOUDA_X0(pRoi->tlPX) | GOUDA_Y0(pRoi->tlPY);
        hwp_gouda->gd_roi_br_ppos = GOUDA_X1(pRoi->brPX) | GOUDA_Y1(pRoi->brPY);

        // Set configuration register
        goudaLcdConfig.reg = g_halGoudaLcdConfig.reg; // from tgt
        goudaLcdConfig.b.cs = HAL_GOUDA_LCD_IN_RAM;
        goudaLcdConfig.b.outputFormat = HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565;
        hwp_gouda->gd_lcd_ctrl = goudaLcdConfig.reg;

        // Record handler and enable interrupt if needed.
        g_gouda_ctx.userIrqHandler = handler;
        if (handler != NULL)
        {
            // Set Mask
            hwp_gouda->gd_eof_irq_mask = GOUDA_EOF_MASK;
        }
        else
        {
            // Disable IRQ
            hwp_gouda->gd_eof_irq_mask = 0;
        }

        hwp_gouda->gd_lcd_mem_address = (uint32)pBuffer;

        hwp_gouda->gd_lcd_stride_offset = offset;
        // Run GOUDA
        hwp_gouda->gd_command = GOUDA_START;

        // that's it
        hal_SysExitCriticalSection(status);
        return HAL_ERR_NO;
    }
}


PUBLIC HAL_ERR_T hal_GoudaRun(void)
{

    // check Ready
    uint32 status = hal_SysEnterCriticalSection();
   gouda_event(0x90000015);

    if ((hwp_gouda->gd_status & (GOUDA_IA_BUSY | GOUDA_LCD_BUSY)) != 0)
    {
        // GOUDA is busy, abort
        hal_SysExitCriticalSection(status);
        return HAL_ERR_RESOURCE_BUSY;
    }

    // Run GOUDA
    hwp_gouda->gd_command = GOUDA_START;

    // that's it
    hal_SysExitCriticalSection(status);

    return HAL_ERR_NO;
}
// =============================================================================
// hal_GoudaIsActive
// -----------------------------------------------------------------------------
/// Get the activity status
/// @return \c TRUE when active
// =============================================================================
PUBLIC BOOLEAN hal_GoudaIsActive(void)
{
/*
    if(g_gouda_ctx.fmarkBusy == TRUE)
    {
        return TRUE;
    }
*/
    uint32  status = hal_SysEnterCriticalSection();
    if ((hwp_gouda->gd_status & (GOUDA_IA_BUSY | GOUDA_LCD_BUSY)) != 0)
    {
        hal_SysExitCriticalSection(status);
        return TRUE;//bringup_tmp
    }
    else
    {
        hal_SysExitCriticalSection(status);
        return FALSE;
    }
}

// =============================================================================
// hal_GoudaSetBlock
// -----------------------------------------------------------------------------
/// Indicate whether to wait until gouda blit operations finish.
/// @param block The block flag, with a non-zero value to wait, 0 otherwise.
// =============================================================================
PUBLIC void hal_GoudaSetBlock(uint32 block)
{
   gouda_event(0x90000017);

    g_halGoudaBlock = block;
}

// =============================================================================
// hal_GoudaSetBgColor
// -----------------------------------------------------------------------------
/// Set the background color
/// @param color in RGB565 format
// =============================================================================
PUBLIC void hal_GoudaSetBgColor(uint16 color)
{
  // gouda_event(0x90000018);

    hwp_gouda->gd_roi_bg_color = color;
}

// =============================================================================
// hal_GoudaGetBgColor
// -----------------------------------------------------------------------------
/// Get the background color
/// @param color in RGB565 format
// =============================================================================
PUBLIC uint16 hal_GoudaGetBgColor(void)
{
   //gouda_event(0x90000019);

    return hwp_gouda->gd_roi_bg_color;
}

// =============================================================================
// hal_GoudaOvlLayerOpen
// -----------------------------------------------------------------------------
/// Open and configure an overlay layer.
///
/// @param id the layer Id
/// @param def the configuration
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the layer is already enabled.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaOvlLayerOpen(HAL_GOUDA_OVL_LAYER_ID_T id, HAL_GOUDA_OVL_LAYER_DEF_T* def)
{
    uint8 wpp = (def->fmt==HAL_GOUDA_IMG_FORMAT_RGBA)?2:1;
    // Note that stride could be different in case of padding
    uint16 stride;

    DEFERRED_TIMINGS_SET;
   //gouda_event(0x9000001a);
    if ((hwp_gouda->Overlay_Layer[id].gd_ol_input_fmt & GOUDA_ACTIVE) != 0)
    {
        return HAL_ERR_RESOURCE_BUSY;
    }

    if (def->stride)
    {
        stride = def->stride;
    }
    else
    {
        stride = (def->pos.brPX - def->pos.tlPX + 1) *wpp;
    }
    // TODO check supported formats
    hwp_gouda->Overlay_Layer[id].gd_ol_input_fmt =
        ((def->fmt==HAL_GOUDA_IMG_FORMAT_RGB565)?GOUDA_FORMAT(0):
         GOUDA_FORMAT(1)) |
        GOUDA_STRIDE(stride) |
        GOUDA_PREFETCH |
        GOUDA_ACTIVE;

    hwp_gouda->Overlay_Layer[id].gd_ol_tl_ppos =
        GOUDA_X0(def->pos.tlPX) | GOUDA_Y0(def->pos.tlPY);
    hwp_gouda->Overlay_Layer[id].gd_ol_br_ppos =
        GOUDA_X1(def->pos.brPX) | GOUDA_Y1(def->pos.brPY);
    hwp_gouda->Overlay_Layer[id].gd_ol_blend_opt =
        GOUDA_ALPHA(def->alpha) |
        GOUDA_CHROMA_KEY_COLOR(def->cKeyColor) |
        ((def->cKeyEn)?GOUDA_CHROMA_KEY_ENABLE:0) |
        GOUDA_CHROMA_KEY_MASK(def->cKeyMask);

    hwp_gouda->Overlay_Layer[id].gd_ol_rgb_src = (uint32)def->addr;

	g_gouda_ctx.goudaOpened = TRUE;
    return HAL_ERR_NO;
}

// =============================================================================
// hal_GoudaOvlLayerClose
// -----------------------------------------------------------------------------
/// Close an overlay layer.
// =============================================================================
PUBLIC void hal_GoudaOvlLayerClose(HAL_GOUDA_OVL_LAYER_ID_T id)
{
    DEFERRED_TIMINGS_SET;
    gouda_event(0x9000001b);
    hwp_gouda->Overlay_Layer[id].gd_ol_input_fmt = 0;
}


// =============================================================================
// hal_GoudaVidLayerOpen
// -----------------------------------------------------------------------------
/// Open and configure the video layer.
///
/// @param def the configuration
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the layer is already enabled.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaVidLayerOpen(HAL_GOUDA_VID_LAYER_DEF_T* def)
{
    uint32 xpitch, ypitch;
    uint8 bpp = (def->fmt==HAL_GOUDA_IMG_FORMAT_IYUV)?1:2;
    // Note that stride could be different in case of padding
    uint16 stride = def->stride?def->stride:def->width*bpp;
    uint16 outWidth = def->pos.brPX - def->pos.tlPX + 1;
    uint16 outHeight = def->pos.brPY - def->pos.tlPY + 1;

    DEFERRED_TIMINGS_SET;
   gouda_event(0x9000001c);

    if ((hwp_gouda->gd_vl_input_fmt & GOUDA_ACTIVE) != 0)
    {
        return HAL_ERR_RESOURCE_BUSY;
    }
    xpitch = (def->width<<8) / outWidth;
    ypitch = (def->height<<8) / outHeight;
    hwp_gouda->gd_vl_extents = GOUDA_MAX_COL(def->width-1) | GOUDA_MAX_LINE(def->height-1);

    hwp_gouda->gd_vl_y_src = (uint32)def->addrY;
    hwp_gouda->gd_vl_u_src = (uint32)def->addrU;
    hwp_gouda->gd_vl_v_src = (uint32)def->addrV;
    hwp_gouda->gd_vl_resc_ratio = GOUDA_XPITCH(xpitch) | GOUDA_YPITCH(ypitch) | GOUDA_YPITCH_SCALE_ENABLE;

    // TODO check supported formats
    hwp_gouda->gd_vl_input_fmt =
        ((def->fmt==HAL_GOUDA_IMG_FORMAT_RGB565)?GOUDA_FORMAT(0):
         (def->fmt==HAL_GOUDA_IMG_FORMAT_UYVY)?GOUDA_FORMAT(1):
         (def->fmt==HAL_GOUDA_IMG_FORMAT_YUYV)?GOUDA_FORMAT(2):
         (def->fmt==HAL_GOUDA_IMG_FORMAT_IYUV)?GOUDA_FORMAT(3):0) |
        GOUDA_STRIDE(stride) |
        GOUDA_ACTIVE;
    hwp_gouda->gd_vl_tl_ppos =
        GOUDA_X0(def->pos.tlPX) | GOUDA_Y0(def->pos.tlPY);
    hwp_gouda->gd_vl_br_ppos =
        GOUDA_X1(def->pos.brPX) | GOUDA_Y1(def->pos.brPY);

    hwp_gouda->gd_vl_blend_opt =
        GOUDA_DEPTH(def->depth) |GOUDA_ROTATION(def->rotation)|
        GOUDA_ALPHA(def->alpha) |
        GOUDA_CHROMA_KEY_COLOR(def->cKeyColor) |
        ((def->cKeyEn)?GOUDA_CHROMA_KEY_ENABLE:0) |
        GOUDA_CHROMA_KEY_MASK(def->cKeyMask);

    return HAL_ERR_NO;
}

// =============================================================================
// hal_GoudaVidLayerClose
// -----------------------------------------------------------------------------
/// Close the video layer.
// =============================================================================
PUBLIC void hal_GoudaVidLayerClose(void)
{
    DEFERRED_TIMINGS_SET;
   //gouda_event(0x9000001d);

    hwp_gouda->gd_vl_input_fmt = 0;
}

// =============================================================================
// hal_GoudaVidLayerState
// -----------------------------------------------------------------------------
/// Get an video layer state.
// =============================================================================
PUBLIC uint32 hal_GoudaVidLayerState(void)
{
    DEFERRED_TIMINGS_SET;
   //gouda_event(0x9000001f);

    return hwp_gouda->gd_vl_input_fmt;
}

// =============================================================================
// hal_GoudaInitResetPin
// -----------------------------------------------------------------------------
/// Initialize the status of LCD reset pin.
// =============================================================================
PUBLIC void hal_GoudaInitResetPin(void)
{

#if 0
    if (g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_IO ||
            g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_O)
    {
        if (g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_IO)
        {
            hal_GpioSetOut(g_halCfg->goudaCfg.lcdResetPin.gpioId);
        }
        // Keep LCD in reset state
        if (g_halCfg->goudaCfg.lcdResetActiveLow)
        {
            hal_GpioClr(g_halCfg->goudaCfg.lcdResetPin);
        }
        else
        {
            hal_GpioSet(g_halCfg->goudaCfg.lcdResetPin);
        }
    }
#endif
}

PUBLIC void hal_GoudatResetLcdPin(void)
{
    REG_GOUDA_GD_LCD_CTRL_T ctrl_lcd;

    ctrl_lcd.v = hwp_gouda->gd_lcd_ctrl;

    if(ctrl_lcd.b.lcd_resetb == 1)
    {
        ctrl_lcd.b.lcd_resetb= 0;
        hwp_gouda->gd_lcd_ctrl = ctrl_lcd.v;
        hal_LcdDelay(120); // 2  modified by bao for lcd reset
        ctrl_lcd.b.lcd_resetb= 1;
        hwp_gouda->gd_lcd_ctrl = ctrl_lcd.v;
    }
    else
    {
        ctrl_lcd.b.lcd_resetb= 1;
        hwp_gouda->gd_lcd_ctrl = ctrl_lcd.v;
        hal_LcdDelay(120); // 2 modified by bao for lcd reset
        ctrl_lcd.b.lcd_resetb= 0;
        hwp_gouda->gd_lcd_ctrl = ctrl_lcd.v;
    }
}

// =============================================================================
// hal_GoudaIrqHandler
// -----------------------------------------------------------------------------
/// Handles IRQ.
// =============================================================================
LOCAL void hal_GoudaIrqHandler(uint32 interruptId)
{
    uint32 tmpRead;
    hwp_gouda->gd_eof_irq = GOUDA_EOF_CAUSE; // Clear interrupt
    tmpRead = hwp_gouda->gd_eof_irq;

    DEFERRED_TIMINGS_SET;
   gouda_event(0x9000feed);

    // Call user handler
    if (g_gouda_ctx.userIrqHandler)
    {
        g_gouda_ctx.userIrqHandler();
    }
}


// =============================================================================
// hal_GoudaGetTiming
// -----------------------------------------------------------------------------
/// Get lcd timing.
// =============================================================================
PUBLIC uint32 hal_GoudaGetTiming(void)
{
   //gouda_event(0x90000023);

    return hwp_gouda->gd_lcd_timing;
}



// =============================================================================
// hal_GoudaSetTiming
// -----------------------------------------------------------------------------
/// Set lcd timing.
// =============================================================================
PUBLIC void hal_GoudaSetTiming(uint32 gouda_timing)
{
   //gouda_event(0x90000024);

    hwp_gouda->gd_lcd_timing = gouda_timing;
}

void hal_gouda_init(void)
{
    HAL_GOUDA_LCD_CONFIG_T gd_lcd_config = {0};

    gd_lcd_config.b.cs = HAL_GOUDA_LCD_CS_0;
    gd_lcd_config.b.outputFormat = HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565;
    gd_lcd_config.b.cs0Polarity = FALSE;
    gd_lcd_config.b.cs1Polarity = FALSE;
    gd_lcd_config.b.resetb = TRUE;
    gd_lcd_config.b.rsPolarity = FALSE;
    gd_lcd_config.b.wrPolarity = FALSE;
    gd_lcd_config.b.rdPolarity = FALSE;
    gd_lcd_config.b.highByte = FALSE;

    ISR_RegHandler(TB_GOUDA_INT, hal_GoudaIrqHandler);

    CHIPDRV_EnableIRQINT(TB_GOUDA_INT);

    hal_SetGoudaClock(FALSE);
    gouda_event(0x97000013);gouda_event(g_line_type);

    hal_GoudaSerialOpen(g_line_type, LCDD_SPI_FREQ, &gd_lcd_config, 0);
}

void hal_gouda_sim_init(uint16 two_dataLine,uint16 fmark,uint32 fdelay)
{
    HAL_GOUDA_LCD_CONFIG_T gd_lcd_config = {0};

    gd_lcd_config.b.cs = HAL_GOUDA_LCD_CS_0;
    gd_lcd_config.b.outputFormat = HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565;
    gd_lcd_config.b.cs0Polarity = FALSE;
    gd_lcd_config.b.cs1Polarity = FALSE;
    gd_lcd_config.b.resetb = TRUE;
    gd_lcd_config.b.rsPolarity = FALSE;
    gd_lcd_config.b.wrPolarity = FALSE;
    gd_lcd_config.b.rdPolarity = FALSE;
    gd_lcd_config.b.highByte = FALSE;
    g_gouda_fmark = fmark;
    g_gouda_fmark_delay =fdelay;

   SCI_TraceLow("hal_gouda_sim_init : gd_lcd_config = %x\n", gd_lcd_config.reg);

    hal_SetGoudaClock(FALSE);

    if(two_dataLine == 1)
    {
        g_line_type = HAL_GOUDA_SPI_LINE_3_2_LANE;
        hal_GoudaSerialOpen(HAL_GOUDA_SPI_LINE_3, LCDD_SPI_FREQ_READ, &gd_lcd_config, 0);
        gouda_event(0x9700000d);
    }
    else if(two_dataLine ==0)
    {
        g_line_type= HAL_GOUDA_SPI_LINE_4;
        hal_GoudaSerialOpen(g_line_type, LCDD_SPI_FREQ_READ, &gd_lcd_config, 0);
        gouda_event(0x9700000c);
    }
    else if(two_dataLine == 3)
    {
        g_line_type= HAL_GOUDA_SPI_LINE_3;
        hal_GoudaSerialOpen(g_line_type, LCDD_SPI_FREQ_READ, &gd_lcd_config, 0);
        gouda_event(0x9700000c);
    }
}

void hal_gouda_term(void)
{
    hal_GoudaClose();

    CHIPDRV_DisableIRQINT(TB_GOUDA_INT);
    ISR_UnRegHandler(TB_GOUDA_INT);
}

void hal_LcdDelay (int ms_delay)
{
    #if 1
    SCI_Sleep(ms_delay);
    #else
    uint32 wait_time = 0;
    uint32 wait_time2 = 0;
    wait_time2 = ms_delay*16384/1000;
    wait_time = timer_TimRealTickGet(1);  //TIMER_1

    while(1)
    {
        if((timer_TimRealTickGet(1) - wait_time) > wait_time2)
        {
            return;
        }
    }
    #endif
}

BOOLEAN  hal_gouda_is_2_date_line(void)
{

    if(g_line_type == HAL_GOUDA_SPI_LINE_3_2_LANE)
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

PUBLIC void hal_GoudaOvlLayerActive(HAL_GOUDA_OVL_LAYER_ID_T id)
{
    hwp_gouda->Overlay_Layer[id].gd_ol_input_fmt |= (1<<31);
}

PUBLIC void hal_GoudaOvlLayerNoActive(HAL_GOUDA_OVL_LAYER_ID_T id)
{
	hwp_gouda->Overlay_Layer[id].gd_ol_input_fmt &= ~(1<<31);
}

LOCAL void hal_gouda_reinit(void)
{
    HAL_GOUDA_LCD_CONFIG_T gd_lcd_config = {0};

    gd_lcd_config.b.cs = HAL_GOUDA_LCD_CS_0;
    gd_lcd_config.b.outputFormat = HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565;
    gd_lcd_config.b.cs0Polarity = FALSE;
    gd_lcd_config.b.cs1Polarity = FALSE;
    gd_lcd_config.b.resetb = TRUE;
    gd_lcd_config.b.rsPolarity = FALSE;
    gd_lcd_config.b.wrPolarity = FALSE;
    gd_lcd_config.b.rdPolarity = FALSE;
    gd_lcd_config.b.highByte = FALSE;
    g_halGoudaLcdConfig.reg = gd_lcd_config.reg;
    hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg;

    ISR_RegHandler(TB_GOUDA_INT, hal_GoudaIrqHandler);
    CHIPDRV_EnableIRQINT(TB_GOUDA_INT);

    hal_SetGoudaClock(FALSE);

    g_halGoudaSlcdCfg.b.isSlcd = TRUE;
    g_halGoudaSlcdCfg.b.clkDivider = g_halGoudaSpiFreqDivider;
    g_halGoudaSlcdCfg.b.lineType = g_line_type;
    g_halGoudaSlcdCfg.b.isRead = FALSE;
    hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;
    hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;

    if(g_gouda_fmark == ENABLE_FMARK_LCD)
    {
        hwp_gouda->TECON = GOUDA_TE_EN|GOUDA_TE_EDGE_SEL;
        hwp_gouda->TECON2 = GOUDA_TE_COUNT1(g_gouda_fmark_delay); //GOUDA_TE_COUNT1(0x10000);
    }
    else
    {
        hwp_gouda->TECON = 0;
    }

    g_gouda_ctx.goudaOpened = TRUE;
}


PUBLIC void hal_GoudaReset(void)
{
    //RST_GOUDA
    hwp_apApb->ap_rst0 |= (1 << 4);
    //RST_GOUDA_H2X_SYNC
    hwp_apApb->ap_rst1 |= (1 << 6);
    hal_LcdDelay(1);
    //RST_GOUDA_CLR
    hwp_apApb->ap_rst0_clr |= (1 << 4);
    //RST_GOUDA_H2X_SYNC_CLR
    hwp_apApb->ap_rst1_clr |= (1 << 6);
    //Gouda reinit
    hal_gouda_reinit();
}
