/******************************************************************************
 ** File Name:      chip_phy_v5.c                                                    *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/14/2002                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic information on chip.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/14/2002     Richard.Yang     Create.                                   *
 ** 09/16/2003     Xueliang.Wang    Modify CR4013                             *
 ** 08/23/2004     JImmy.Jia        Modify for SC6600D                        *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//lint -e766 for "emc_drvapi.h" "sci_types.h" "sci_api.h" "sio.h"
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "power_manager.h"
#include "fiq_drvapi.h"
#include "busmonitor_drvapi.h"
#include "sim_drv.h"
#include "os_param.h"
#include "pinmap_drvapi.h"
#include "mmu_drvapi.h"
#include "analog_drvapi.h"
#include "mcu_reboot.h"
#include "clock_drvapi.h"
#include "Ref_outport.h"
#include "boot_drvapi.h"
#include "cmddef.h"
#include "dal_power.h"
#include "gpio_prod_api.h"
#include "emc_drvapi.h"
#include "dsp_drvapi.h"
#include "../chip_phy.h"

#include "osc_drvapi.h"
#include "../osc/osc_phy.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/
#define HW_RST_MASK       0x1F
#define HW_BL_MASK         BIT_5
#define HW_DSPEXCEPTION_MASK  BIT_6
#define HW_USB_MASK       BIT_7

/* boot flag, defined the same with romcode*/
#define BOOT_FLAG_EXT_RESET          	(0x8A)
#define BOOT_FLAG_WDG_RESET		(0x9A)

#define ADI_ARMREG_ADDR_MASK (0xFFF)//[11:0]
#define ADI_ARMREG_FLAG_MASK (0<<12)

#define ADIE_CHIPID			0x1130C000
#define CHIPID_HIGHPART_SHIFT		0x10

#pragma arm section rwdata = "CHIP_RESET_CODE"
volatile  uint32 reset_data[] =
{
    0xe59f1004,
    0xe5810000,
    0xe3a0f000,
    0x205000e0
};
#pragma arm section rwdata
/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 chip_id = 0;
LOCAL CHIP_TYPE_E chip_type = CHIP_TYPE_MAX;

const CHIP_INFO chip_info[] =
{
    {CHIP_ID_SC6530,              "SC6530\n"},
    {CHIP_ID_SC6530C,			  "SC6530C\n"},
    {CHIP_ID_SC6531,			  "SC6531\n"}
};

LOCAL uint32 ana_chip_id = 0;

const CHIP_INFO ana_chip_info[] =
{
	{CHIP_ID_SR1130,				 "SR1130\n"},
	{CHIP_ID_SR1130C,			     "SR1130C\n"}
};
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
LOCAL void CHIP_PHY_SetANAReg (uint32 addr, uint32 value);
LOCAL uint32 CHIP_PHY_GetANAReg (uint32 addr);
/*****************************************************************************/
//  Description:   Get HW Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHIP_PHY_GetWDGHWFlag (WDG_HW_FLAG_T type);

/*****************************************************************************/
//  Description:    Before watchdog reset, writting HW_RST flag is uesed to j-
//                  udge differrent watchdog reset conditions between MCU reset
//                  and system-halted.
//  Author:         Jeff.Li
//  Modified by:  Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_SetWDGHWFlag (WDG_HW_FLAG_T type, uint32 val);

/*****************************************************************************/
//  Description:  Restore Jtag Pin Function selection after Assert  
//  Author:         Yong.Li
//  Modified by:  
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_Restore_Jtag_Function (void)
{
    // restore jtag pin function select
    CHIP_REG_SET(PIN_MTDO_REG,  ( PIN_Z_EN | PIN_SPU_EN | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1  ));   // 0x101  JTAG
    CHIP_REG_SET(PIN_MTDI_REG,  ( PIN_Z_EN | PIN_SPU_EN | PIN_FUNC_DEF | PIN_FPU_EN | PIN_DS_1  ));   // 0x18A  JTAG  
    CHIP_REG_SET(PIN_MTCK_REG,  ( PIN_Z_EN | PIN_SPU_EN | PIN_FUNC_DEF | PIN_FPU_EN | PIN_DS_1   ) );   // 0x18A  JTAG  
    CHIP_REG_SET(PIN_MTMS_REG,  ( PIN_Z_EN | PIN_SPU_EN | PIN_FUNC_DEF | PIN_FPU_EN | PIN_DS_1   ));   // 0x18A  JTAG  
    CHIP_REG_SET(PIN_MTRSTN_REG, ( PIN_Z_EN | PIN_SPU_EN | PIN_FUNC_DEF | PIN_FPU_EN | PIN_DS_1   ));   // 0x146  JTAG  
}

/*********************************************************************************************/
//Description:   This function is used to initializtion system registers
//Author:
//Note:          Can't use trace and assert in this funciton ;
//Parameter:
/*********************************************************************************************/
LOCAL void _CHIP_PHY_Reg_Init(void)
{
    //set the emc port's ahb master priority, dma>arm
    CHIP_REG_OR(AHB_SYS_CTL4, (BIT_6|BIT_5));
}

/*****************************************************************************/
//  Description:    The function is used to get the chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:           It must be called before any hardware related settings
/*****************************************************************************/
PUBLIC void CHIP_InitHWChipID (void)
{
    uint32 id =  REG32 (CHIP_TYPE);

    ana_chip_id = (CHIP_PHY_GetANAReg(ANA_CHIP_ID_P0) & ANA_CHIP_ID_H16_MSK) << CHIPID_HIGHPART_SHIFT;
    ana_chip_id |= (CHIP_PHY_GetANAReg(ANA_CHIP_ID_P1) & ANA_CHIP_ID_L16_MSK);

    chip_id = id;

    switch (chip_id)
    {
        case 0x65300000:
        case 0x6530C000:
            chip_type = SC6530;
            break;

        case 0x65310000:
            chip_type = SC6531;
            break;	
			
        default:
#ifdef FPGA_SUPPORT_SC6531		
			chip_type = SC6531;
#endif			            
            break;
    }

    return;
}

/*****************************************************************************/
//  Description:    The function is used to detect the memory type
//  Global resource dependence:
//  Author:         Nick.Zhao
//  Return value:
//                1      CS0 connects to SDRAM
//                0      CS0 connects to FLASH
/*****************************************************************************/
PUBLIC uint32 CHIP_DetectMemType (void)
{
    uint32 bSDRAM;

#ifdef NOR_SRAM_PLATFORM
    bSDRAM = FALSE;
#else
    bSDRAM = TRUE;
#endif

    return bSDRAM;
}
/*****************************************************************************/
//  Description:    The function is used to return chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC  uint32 CHIP_GetHWChipID (void)   // return chip ID number.
{
    return chip_id;
}

/*****************************************************************************/
//  Description:    The function change the core voltage
//  Global resource dependence:
//  Author:         Nick.Zhao
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_ChangeCoreVoltage (CORE_VOLTAGE_E core_voltage)
{

}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
uint32 CHIP_GetArmClk (void)
{
#ifdef FPGA_VERIFICATION
    return  ARM_CLK_36M;
#else
    uint32 arm_clk = ARM_CLK_26M;
#ifdef CHIP_VER_6530
    volatile uint32 clk_reg = REG32 (APB_CLK_CFG0);

    switch ((clk_reg & CLK_MCU_SEL_MSK) >> CLK_MCU_SEL_SHIFT)
    {
    case 0:
        arm_clk = ARM_CLK_26M;
        break;
    case 1:
        arm_clk = ARM_CLK_208M;
        break;
    case 2:
        arm_clk = ARM_CLK_104M;
        break;
    case 3:
        arm_clk = ARM_CLK_156M;
        break;
    default:
        break;
    }
#elif defined(CHIP_VER_6531)
    volatile uint32 clk_reg = REG32 (APB_CLK_CFG3);

    switch ((clk_reg & CLK_MCU_SEL_MSK) >> CLK_MCU_SEL_SHIFT)
    {
    case 0:
        arm_clk = ARM_CLK_26M;
        break;
    case 1:
        arm_clk = ARM_CLK_208M;
        break;
    case 2:
        arm_clk = ARM_CLK_249_6M;
        break;
    case 3:
        arm_clk = ARM_CLK_312M;
        break;
    default:
        break;
    }
#elif define(CHIP_VER_6531EFM)
    volatile uint32 clk_reg = REG32(APB_CLK_CFG3);

    switch ((clk_reg & CLK_MCU_SEL_MSK) >> CLK_MCU_SEL_SHIFT)
    {
    //CPLL
    case 0:
        arm_clk = ARM_CLK_26M;
        break;
    case 1:
        arm_clk = ARM_CLK_104M;
        break;
    case 2:
        arm_clk = ARM_CLK_208M;
        break;
    //APLL    
    case 3:
        arm_clk = ARM_CLK_208M;
        break;
    default:
        break;
    }   
#endif
    return arm_clk;
#endif
}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Younger.yang
// Note :           6600M ARM_CLK/AHB_CLK = 1
/*****************************************************************************/
uint32 CHIP_GetAhbClk (void)
{
#ifdef FPGA_VERIFICATION
    return  ARM_CLK_36M;
#else
#ifdef CHIP_VER_6530
    volatile uint32 reg_val = REG32 (APB_CLK_CFG0);

    return (reg_val & CLK_AHB_DIV2_EN) ? (CHIP_GetArmClk() >> 1) : CHIP_GetArmClk();
#elif defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)  
    volatile uint32 reg_val = REG32 (APB_CLK_CFG3);

    return (reg_val & CLK_MCU_RATIO) ? (CHIP_GetArmClk() >> 1) : CHIP_GetArmClk();

#endif
#endif
}


/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
uint32 CHIP_GetAPBClk (void)
{
    return ARM_CLK_26M;
}


/*****************************************************************************/
// Description :    This function is used to disable all fiqs on sc6600d
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           every bit indicates one fiq src
/*****************************************************************************/
void CHIP_DisableFIQ (void)
{
    CHIPDRV_DisableFIQINTALL();
}

/*****************************************************************************/
// Description :    This function is used to get current PLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetPllClk (void)
{
    return CHIP_GetMPllClk();
}


uint32 CHIP_GetMPllClk (void)
{
    return (CHIP_GetAPBClk() / 2) * ( (* (volatile uint32 *) APB_PLL_CTL2) & 0x0000FFFF);
}


/*****************************************************************************/
// Description :    This function is used to get current VPLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           Daniel.ding add it for SC6800 .
/*****************************************************************************/
uint32 CHIP_GetVPllClk (void)
{
    return NULL;
}

/******************************************************************************/
//  Description:   Get product mode
//  Parameter:
//      None
//    Return:
//           1     In product mode
//           0     Product mode close
/******************************************************************************/
PUBLIC BOOLEAN CHIP_IsSPINorMode(void)
{
#ifdef CHIP_VER_6531
  return TRUE;
#else
  volatile unsigned int Value = 0;

  /*Purpose:To get the bonding vaule of SPI Nor mode*/
  Value = CHIP_REG_GET(AHB_SYS_CTL5);

  if(Value & ((unsigned int)PNOR_OPTION))
  {
  	return FALSE;
  }
  else
  {
  	return TRUE;
  }
#endif
}
/*****************************************************************************/
// Description :    This function is used to Open self 32k clock.
// Global resource dependence :
// Author :         Kui.Wang
// Note :
/*****************************************************************************/
LOCAL void CHIP_OpenOSCSelfBeforePD(void)
{
	OSC_HAL_EnableClkSource(CLOCK_SELF_32K,SCI_TRUE);
	OSC_HAL_SELF_EnableCalibrate(SCI_TRUE);
	OSC_HAL_SELF_StartCalibrate(0x100000);
	OSC_HAL_SelectClkSource(CLOCK_SELF_32K);
	
	OSC_HAL_EnableClkSource(CLOCK_EXTE_32K,SCI_FALSE);
	CHIP_PHY_SetANAReg(ANA_LDO_PD_RST1,0x0000);
	CHIP_PHY_SetANAReg(ANA_LDO_PD_RST0,0x0000);
	CHIP_PHY_SetANAReg(ANA_LDO_PD_SET1,0x001F);
	CHIP_PHY_SetANAReg(ANA_LDO_PD_SET0,0xFFFF);
	for (;;) {}
}
/*****************************************************************************/
// Description :    This function is used to Open dcxo 32k clock.
// Global resource dependence :
// Author :         Kui.Wang
// Note :
/*****************************************************************************/
LOCAL void CHIP_OpenDcxo32KBeforePD(void)
{
	OSC_HAL_EnableClkSource(CLOCK_DCXO_32K,SCI_TRUE);
	OSC_HAL_SelectClkSource(CLOCK_DCXO_32K);
	
	OSC_HAL_EnableClkSource(CLOCK_SELF_32K,SCI_FALSE);
	OSC_HAL_EnableClkSource(CLOCK_EXTE_32K,SCI_FALSE);

	CHIP_PHY_SetANAReg(ANA_LDO_PD_RST1,0x0000);
	CHIP_PHY_SetANAReg(ANA_LDO_PD_RST0,0x0010);
	CHIP_PHY_SetANAReg(ANA_LDO_PD_SET1,0x001F);
	CHIP_PHY_SetANAReg(ANA_LDO_PD_SET0,0x7FEF);
	for (;;) {}	
}
/*****************************************************************************/
// Description :    This function is used to Open external 32k clock.
// Global resource dependence :
// Author :         Kui.Wang
// Note :
/*****************************************************************************/
LOCAL void CHIP_OpenExternal32KBeforePD(void)
{
	OSC_HAL_EnableClkSource(CLOCK_EXTE_32K,SCI_TRUE);
	OSC_HAL_SelectClkSource(CLOCK_EXTE_32K);

	OSC_HAL_EnableClkSource(CLOCK_SELF_32K,SCI_FALSE);

	CHIP_PHY_SetANAReg(ANA_LDO_PD_RST1,0x0000);
	CHIP_PHY_SetANAReg(ANA_LDO_PD_RST0,0x0000);
	CHIP_PHY_SetANAReg(ANA_LDO_PD_SET1,0x001F);
	CHIP_PHY_SetANAReg(ANA_LDO_PD_SET0,0xFFFF);
	for (;;) {}	
}
/*****************************************************************************/
/*****************************************************************************/
// Description :    This function is used to turn off the power.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_TurnOffPower (void)
{
    // Disable FIQ
    CHIPDRV_DisableFIQINTALL();
    // Disable IRQ
    CHIPDRV_DisableIRQINTALL();


	if(ADIE_CHIPID == CHIP_GetADIEChipID())
	{
#ifdef OSC32K_CLOCK_SELECT_DCXO	
		CHIP_OpenDcxo32KBeforePD();
#endif

#ifdef OSC32K_CLOCK_SELECT_SELF
		CHIP_OpenOSCSelfBeforePD();	
#endif

#ifdef OSC32K_CLOCK_SELECT_EXTE
		CHIP_OpenExternal32KBeforePD();
#endif
	}

    //power down all the ldo
    REG32 (ANA_LDO_PD_SET1)  = 0x1F;
    REG32 (ANA_LDO_PD_SET0)  = 0x3FFF;

    for (;;) {}
}


/*****************************************************************************/
// Description :    This function is used to reset MCU.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_ResetMCU (void)
{
    // This loop is very important to let the reset process work well on V3 board
    // @Richard
    uint32 i = 10000;

    while (i--) {}

    /* disable bus monitor if any */
    if (CHIP_DetectMemType())
    {
        /* disable bus monitor if any */
        bm_disable();
    }

    // Send Fiq to DSP to wake up scheduler
    // Especially when charger is connected
    cmd_ptr->RefCmd_block.cmd_done = 0;
    cmd_ptr->RefCmd_block.Cmd_ID = REF_CMD_ASSERT;

    REG32 (APB_INT_SET_CLR0) = MCU_FRQ_SET;

    //Wait for DSP and scheduler wake up
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 30)
        {
            tick2 = SCI_GetTickCount();
        }
    }



    DSP_HAL_DspinResetMode(TRUE);
    
    // Indicate that DSP is resetted.
    REG16 (0x10000FFE) = 0;

    //Disable all fiq src.
    CHIP_DisableFIQ ();

    SCI_DisableIRQ();
    //SCI_DisableFIQ();
    WDG_ResetMCU();
    OS_TickDelay (100);
    //Clear FIQ and IRQ from DSP
    DSP_HAL_ClrDspIrqInt(DSP_INT_IRQ0);
    DSP_HAL_ClrDspIrqInt(DSP_INT_FRQ0);
    
    if (CHIP_DetectMemType())
    {
        mcu_nand_reboot();
    }

}

/*****************************************************************************/
// Description :    This function is used to reset MCU from boot code.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_BootReset (void)
{
    WDG_TimerStop ();

    /* Disable bus monitor */
    if (CHIP_DetectMemType())
    {
        /* Disable bus monitor */
        bm_disable();
    }

    // Hold dsp
    DSP_HAL_DspinResetMode(TRUE);

    // Indicate that DSP is resetted.
    REG16 (0x10000FFE) = 0;

    //Disable all fiq src.
    CHIP_DisableFIQ ();

    //Clear FIQ and IRQ from DSP
    DSP_HAL_ClrDspIrqInt(DSP_INT_IRQ0);
    DSP_HAL_ClrDspIrqInt(DSP_INT_FRQ0);

    mcu_uart_reboot();

#ifdef FLASH_TYPE_SPI
    // Dummy operation, to prevent "CHIP_Reset" be optimized
    //SCI_TraceLow:"%x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHIP_PHY_V5_414_112_2_17_23_3_42_497,(uint8*)"dd", reset_data[0], reset_data[1]);
#endif

}

LOCAL void CHIP_OpenOSCDcxoClk(void)
{
    OSC_HAL_EnableClkSource(CLOCK_DCXO_32K, SCI_TRUE);
    OSC_HAL_SelectClkSource(CLOCK_DCXO_32K);
    
    OSC_HAL_EnableClkSource(CLOCK_EXTE_32K, SCI_FALSE);
    OSC_HAL_EnableClkSource(CLOCK_SELF_32K, SCI_FALSE);
}
/*********************************************************************************************/
//Description:   This function is used to initializtion system clk and other
//Author:
//Note:          Can't use trace and assert in this funciton ;
//Parameter:
/*********************************************************************************************/
void CHIP_Init (void)
{
    /* Init system registers */
    _CHIP_PHY_Reg_Init();

    /* Init system clock */
    CLOCK_Config();

//first enable the target 32k clock source after switch to it and close other clock source
	if(ADIE_CHIPID == CHIP_GetADIEChipID())
	{
#if defined(OSC32K_CLOCK_SELECT_DCXO) || defined(OSC32K_CLOCK_SELECT_SELF)
		CHIP_OpenOSCDcxoClk();
		ANA_REG_OR(ANA_SLP_LDO_CTL2,SLP_PWR_SET_EN);		
		OSC_HAL_EnableDcxoAutoPD(SCI_FALSE);
		
		OSC_HAL_EnableRefAutoSleep(DCXO_REF1,SCI_TRUE);
		OSC_HAL_EnableRefAutoSleep(DCXO_REF2,SCI_TRUE);
#endif

#ifdef OSC32K_CLOCK_SELECT_EXTE
		OSC_HAL_EnableClkSource(CLOCK_EXTE_32K,SCI_TRUE);
		OSC_HAL_SelectClkSource(CLOCK_EXTE_32K);
		OSC_HAL_EnableDcxoAutoPD(SCI_TRUE);
		OSC_HAL_EnableClkSource(CLOCK_SELF_32K, SCI_FALSE);
#endif
	}


#ifndef  FPGA_VERIFICATION
    // Init Pin function and GPIOs setting
    PM_Init ();

    //init the module of dsp hold arm
    DSP_HoldArmInit();

#endif
}

/*****************************************************************************/
// Description :    This function is used get the chip type
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC CHIP_TYPE_E CHIP_GetChipType (void)
{
    return chip_type;
}

/*****************************************************************************/
// Description :    This function is used get the chip type
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC const char *CHIP_GetChipInfo (void)
{
    uint8 i;
    const char *info = NULL;


	SCI_ASSERT(NULL != chip_id);/*assert verified*/

	for(i=0;i<sizeof(chip_info)/sizeof(CHIP_INFO);i++)
	{
		if(chip_info[i].chip_id == chip_id)
		{
			info = (const char *)chip_info[i].info;
			break;
		}
	}
    SCI_ASSERT (info != NULL);/*assert verified*/

    return info;
}

/*****************************************************************************/
// Description :    This function is used get the a die chip type
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC const char *CHIP_GetADieChipInfo (void)
{
    uint8 i;
    const char *info = NULL;
	SCI_ASSERT(NULL != ana_chip_id);/*assert verified*/

	for(i=0;i<sizeof(ana_chip_info)/sizeof(CHIP_INFO);i++)
	{
		if(ana_chip_info[i].chip_id == ana_chip_id)
		{
			info = (const char *)ana_chip_info[i].info;
			break;
		}
	}
	SCI_ASSERT(info != NULL);/*assert verified*/
	return info;
}

/*****************************************************************************/
// Description :    This function is active/deactive share-memory clock
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC void CHIP_ActiveSMCLK (BOOLEAN is_active)
{

}

/*****************************************************************************/
// Description :    This function is Get Usb DM Value
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_GetUsbDMValue (void)
{
    REG32 (APB_MISC_CTL0) |= USB_STATE_REN;
    return ( ( (REG32 (APB_MISC_STS0)) & USB_RX_DM) == USB_RX_DM);
}


/*below are removed from driver_export.c, wait clean up*/
// The function is used to reset dsp when arm system enter assert mode
PUBLIC BOOLEAN CHIP_ResetDspInAssertMode (void)
{
    BOOLEAN is_dsp_respond = SCI_FALSE;

    //@Steven
    //SEND_REQ_TO_DSP1(REF_CMD_ASSERT);
    cmd_ptr->RefCmd_block.cmd_done = 0;
    cmd_ptr->RefCmd_block.Cmd_ID = REF_CMD_ASSERT;

    REG32 (APB_INT_SET_CLR0) |= MCU_FRQ_SET;

    //Wait for DSP and scheduler wake up
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 30)
        {
            tick2 = SCI_GetTickCount();
        }
    }

    if (1 == cmd_ptr->RefCmd_block.cmd_done)
    {
        // dsp can reponse to FIQ
        is_dsp_respond = SCI_TRUE;
    }
    else
    {
        // dsp can not reponse to FIQ and run away
        is_dsp_respond = SCI_FALSE;
    }

    return is_dsp_respond;
}

/*****************************************************************************/
//  Description:   This function is used to do something before assert.
//  Global resource dependence:
//  Author:         Zhengjiang.Lei
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_BeforeAssert (void)
{
    uint8 assert_val;
    //in other platform, deal relative case here ...

    OSPARAM_GetAssertMode (&assert_val);

    if (SCI_ASSERT_DEBUG_MODE == assert_val)
    {
        // If audio device is being controlled by DSP now, switch it back to ARM.
        VB_SWTICH_ARM_CTL (SCI_TRUE);

        // Shut down the speaker PA.
        GPIO_EnableAmplifier (SCI_FALSE);

        // power off sim
        SIM_PowerOffSIM();
    }

    bm_disable();

    CHIP_PHY_Restore_Jtag_Function();

}

// The function is used to reset and set dsp to output dsp assert log to pc by uart1
PUBLIC void CHIP_DspOutPutAssertInfo (void)
{

}
// The function is used to get the IRAM/SHARE-MEMORY buf to store assert information
// in the assert mode.
// The buffer size must be larger than size(IRAM_buf) structure
PUBLIC void *CHIP_GetAssertInfoBufBase (void)
{
    uint32 addr_base = 0xffffffff;
    addr_base = SHARE_MEM_BEGIN;
    return (void *) addr_base;
}

/******************************************************************************/
// Description:   Enable MCU can access switched-ram and shared-ram.
// Dependence:    NONE
// Author:        Haifeng.Yang
// Note:          Beforce invoking this function, the DSP/ISP should be hold on!
/******************************************************************************/
PUBLIC uint32 Chip_SwitchIRAM2Arm (void)
{
    REG32 (APB_MCU_CTL0) |= (ARM_MEM0_EN | ARM_MEM1_EN | ARM_MEM2_EN | ARM_MEM3_EN);
    return 0;
}

PUBLIC uint32 Chip_SwitchIRAMDefault (void)
{
    /* IRAM0 and IRAM1 for arm by default */
    REG32 (APB_MCU_CTL0) |= (ARM_MEM0_EN | ARM_MEM1_EN);
    /* IRAM2 and IRAM3 for dsp by default */
    REG32 (APB_MCU_CTL0) &= ~(ARM_MEM2_EN | ARM_MEM3_EN);
    return 0;
}

/*****************************************************************************/
//  Description:    Gets the register in analog die to judge the reset mode.
//  Author:         Jeff.Li
//  Note:           !It is called before __main now, so it can not call the adi
//                  interface because it contains SCI_DisableIRQ inside, below
//                  writes the adi read interface individually. Because the la-
//                  ckless of SCI_DisableIRQ, so this function must be called
//                  before system interrupt is turnned on!
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetANAReg (uint32 addr)
{
    uint32 adi_rd_data;

    // Set read command
    * (volatile uint32 *) ADI_ARM_RD_CMD = ((addr)&ADI_ARMREG_ADDR_MASK) |
                                            ADI_ARMREG_FLAG_MASK;

    // Wait read operation complete, RD_data[31] will be cleared after the read
    // operation complete
    do
    {
        adi_rd_data = * (volatile uint32 *) ADI_ARM_RD_DATA;
    }
    while (adi_rd_data & BIT_31);

    return ( (adi_rd_data & 0x0000FFFF));
}

/*****************************************************************************/
//  Description:    The function is used to return ADIE chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHIP_GetADIEChipID(void)
{
	return ana_chip_id; 
}


/*****************************************************************************/
//  Description:    Sets the register in analog die.
//  Author:         Yong.Li
//  Note:           !It is called before __main now, so it can not call the adi
//                  interface because it contains SCI_DisableIRQ inside, below
//                  writes the adi read interface individually. Because the la-
//                  ckless of SCI_DisableIRQ, so this function must be called
//                  before system interrupt is turnned on!
/*****************************************************************************/
LOCAL void CHIP_PHY_SetANAReg (uint32 addr, uint32 value)
{
    for (;;)             ////ADI_wait_fifo_empty
    {
        if(((*(volatile uint32*)ADI_ARM_STS) & ADI_STS_FIFO_FULL_MASK )  == 0)
        {
            break;
        }
    }

    CHIP_REG_SET (addr, value);
}

/*****************************************************************************/
//  Description:    Returns the HW_RST register address.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRstMon0Addr (void)
{
    return ANA_HW_RST0;
}

/*****************************************************************************/
//  Description:    Returns the HW_RST register address.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRstMon1Addr (void)
{
    return ANA_HW_RST1;
}


/*****************************************************************************/
//  Description:    Returns the HW_RST register address.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRstMon2Addr (void)
{
    return ANA_HW_RST2;
}

#define HW_RST_ADDR   ANA_HW_RST2
/*****************************************************************************/
//  Description:    Returns the HW_RST register address.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRstAddr (void)
{
    return CHIP_PHY_GetHwRstMon2Addr();
}


/*****************************************************************************/
//  Description:    Returns the HW_RST register address.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRst1Addr (void)
{
    return APB_MISC_CTL2;
}

/*****************************************************************************/
//  Description:    Returns the HW_RST register address.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetExtRstFlagAddr (void)
{
    return ANA_MISC_STS0;
}

/*****************************************************************************/
//  Description:    Returns the HW_RST register address.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetExtRstFlag (void)
{
    return CHIP_PHY_GetANAReg(CHIP_PHY_GetExtRstFlagAddr()) & EXT_RSTN_FLAG;
}

/*****************************************************************************/
//  Description:    Returns the HW_RST register address.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL void CHIP_PHY_ClearExtRstFlag (void)
{
    CHIP_PHY_SetANAReg(ANA_MISC_CTL3, EXT_RSTN_FLAG_CLR);
}

/*****************************************************************************/
//  Description:    This fuction returns the reset mode value.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRstFlag (void)
{
    return (ANA_REG_GET (HW_RST_ADDR) & HW_RST_MASK);
}

/*****************************************************************************/
//  Description:    This fuction returns the backlight mode value.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwBLFlag (void)
{
    return (ANA_REG_GET (HW_RST_ADDR) & HW_BL_MASK);
}

/*****************************************************************************/
//  Description:    This fuction returns the dsp exception mode value.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwDSPExceptionFlag (void)
{
    return (ANA_REG_GET (HW_RST_ADDR) & HW_DSPEXCEPTION_MASK);
}

/*****************************************************************************/
//  Description:    This fuction returns the usb mode value.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwUSBFlag (void)
{
    return (ANA_REG_GET (HW_RST_ADDR) & HW_USB_MASK);
}

/*****************************************************************************/
//  Description:  This fuction set the reset flag of HWRST reg
//  Author:         Jeff.Li
//  Modified by:  Yong.Li
//  Note:           The valid bit filed is from bit4 to bit0
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwRstFlag (uint32 val)
{
    ANA_REG_AND (HW_RST_ADDR, ~HW_RST_MASK);
    ANA_REG_OR (HW_RST_ADDR, (val&HW_RST_MASK));
}

/*****************************************************************************/
//  Description:  This fuction set the backlight flag of HWRST reg
//  Author:         Yong.Li
//  Note:           The valid bit filed is bit5
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwBLFlag (BOOLEAN val)
{
    if (val)
    {
        ANA_REG_OR (HW_RST_ADDR, HW_BL_MASK);
    }
    else
    {
        ANA_REG_AND (HW_RST_ADDR, ~HW_BL_MASK);
    }
}

/*****************************************************************************/
//  Description:   This fuction set the dsp exception flag of HWRST reg
//  Author:         Yong.Li
//  Note:           The valid bit filed is bit5
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwDSPExceptionFlag (BOOLEAN val)
{
    if (val)
    {
        ANA_REG_OR (HW_RST_ADDR, HW_DSPEXCEPTION_MASK);
    }
    else
    {
        ANA_REG_AND (HW_RST_ADDR, ~HW_DSPEXCEPTION_MASK);
    }
}

/*****************************************************************************/
//  Description:  This fuction set the usb flag of HWRST reg
//  Author:         Yong.Li
//  Note:           The valid bit filed is bit5
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwUSBFlag (BOOLEAN val)
{
    if (val)
    {
        ANA_REG_OR (HW_RST_ADDR, HW_USB_MASK);
    }
    else
    {
        ANA_REG_AND (HW_RST_ADDR, ~HW_USB_MASK);
    }
}

/*****************************************************************************/
//  Description:    PHY layer realization of BOOT_ResetHWFlag. It resets the HW
//                  reset register after system initialization.
//  Author:         Jeff.Li
//  Note:           The valid bit filed is from bit15 to bit0
/*****************************************************************************/
PUBLIC void CHIP_PHY_ResetHWFlag (uint32 val)
{
    ANA_REG_AND (ANA_HW_RST0, ~0xFFFF);
    ANA_REG_OR (ANA_HW_RST0, (val&0xFFFF));
    ANA_REG_AND (ANA_HW_RST1, ~0xFFFF);
    ANA_REG_OR (ANA_HW_RST1, 0x7EE7);
    ANA_REG_AND (HW_RST_ADDR, ~0xFFFF);
    ANA_REG_OR (HW_RST_ADDR, (val&0xFFFF));

    CHIP_REG_AND (CHIP_PHY_GetHwRst1Addr (), ~HWRST1_REG_MSK);
    CHIP_REG_OR (CHIP_PHY_GetHwRst1Addr (), ((val&0xFF) << HWRST1_REG_SHIFT));
}

#ifdef CHIP_VER_6531
#pragma arm section code = "MCU_CLK_INIT_IN_IRAM"
PUBLIC void CHIP_PHY_MCU_CLK_Init(void)
{
	 uint32 j;
        
        j = REG32 (APB_CLK_CFG3);
        
        //set ARM clk and AHB clk ratio to be 2
        j &=  ~CLK_MCU_RATIO;
        REG32 (APB_CLK_CFG3) = j;

        //set CLK_ARM to 312MHz
        j &= ~ (CLK_MCU_SEL_MSK);      
        j |= 0x3 << CLK_MCU_SEL_SHIFT;        
        
        REG32 (APB_CLK_CFG3) = j;

        for (j=0; j<100; j++) {}
}
#pragma arm section code
#endif

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_InitModule.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootInitModule ()
{
#ifdef NOR_SRAM_PLATFORM
#ifdef CHIP_VER_6530
    EMC_SysStartUp ();
#elif defined(CHIP_VER_6531)
    CHIP_PHY_MCU_CLK_Init();
#endif
#endif

   // MMU_EnableICache();


}

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_IRAM_EN.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootIramEn ()
{
    CHIP_REG_AND (APB_MCU_CTL0, ~(ARM_MEM1_EN | ARM_MEM2_EN | ARM_MEM3_EN));
}

/*****************************************************************************/
// Description :    This function returns whether the watchdog reset is caused
//                  by software reset or system halted.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsWDGRstByMCU (uint32 flag)
{
    if ( (CHIP_PHY_GetANAReg (HW_RST_ADDR) & HW_RST_MASK) == (flag & HW_RST_MASK))
    {
        return SCI_FALSE;
    }
    else
    {
        return SCI_TRUE;
    }
}

/*****************************************************************************/
// Description :    This function returns whether the reset is caused by ext
//                  reset.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByExtReset(void)
{
    uint32 reset_mode = 0;

    if(CHIP_IsSPINorMode()) //spi-flash
    {
        /* the reset mode has been stored in APB_MISC_CTL2 register  */
        reset_mode = (CHIP_REG_GET(APB_MISC_CTL2) & HWRST1_REG_MSK) >> HWRST1_REG_SHIFT;
        if(BOOT_FLAG_EXT_RESET == reset_mode)
        {
            return SCI_TRUE;
        }
        else
        {
            return SCI_FALSE;
        }
    }
    else // nor-flash
    {
        if(CHIP_PHY_GetExtRstFlag())
        {
            CHIP_PHY_ClearExtRstFlag();
            // record ext-reset mode in HWRST1_REG
            CHIP_REG_AND (CHIP_PHY_GetHwRst1Addr (), ~HWRST1_REG_MSK);
            CHIP_REG_OR (CHIP_PHY_GetHwRst1Addr (), (BOOT_FLAG_EXT_RESET << HWRST1_REG_SHIFT));

            return SCI_TRUE;
        }
        else
        {
            return SCI_FALSE;
        }
    }

}


/*****************************************************************************/
// Description :    This function returns whether the reset is caused by power
//                  up.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByPowerUp()
{
    uint32 reset_mode = 0;

    if(CHIP_IsSPINorMode()) //spi-flash
    {
        /* the reset mode has been stored in APB_MISC_CTL2 register in romcode process */
        reset_mode = (CHIP_REG_GET(APB_MISC_CTL2) & HWRST1_REG_MSK) >> HWRST1_REG_SHIFT;
        if((BOOT_FLAG_EXT_RESET == reset_mode) || (BOOT_FLAG_WDG_RESET == reset_mode))
        {
            return SCI_FALSE;
        }
        else
        {
            return SCI_TRUE;
        }
    }
	else // nor-flash
	{
        if ( (CHIP_PHY_GetANAReg (HW_RST_ADDR) & HW_RST_MASK) == 0)  // power on or ext-reset
        {
            if(CHIP_PHY_GetExtRstFlag())  //ext reset
            {
                // clear ext reset flag
                CHIP_PHY_ClearExtRstFlag();

                // record ext-reset mode in HWRST1_REG
                CHIP_REG_AND (CHIP_PHY_GetHwRst1Addr (), ~HWRST1_REG_MSK);
                CHIP_REG_OR (CHIP_PHY_GetHwRst1Addr (), (BOOT_FLAG_EXT_RESET << HWRST1_REG_SHIFT));

                return SCI_FALSE;
            }
            else  // power on
            {
                return SCI_TRUE;
            }
        }
        else  // wdg reset
        {
            return SCI_FALSE;
        }
    }

}

/*****************************************************************************/
// Description :    This function returns whether the reset is caused by watch-
//                  dog reset.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByWatchDog()
{
    if((CHIP_PHY_GetANAReg(HW_RST_ADDR) != 0) &&
        (CHIP_PHY_GetANAReg(ANA_HW_RST1) == 0))
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

/*****************************************************************************/
// Description :    This function initializes the modules before assert while
//                  the system has just been reset.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void CHIP_PHY_WDGAssertInit ()
{
    // Get current chip type
    CHIP_InitHWChipID();

    CLOCK_Config();
    PM_Init ();
    DMA_HAL_Init ();
    GPIO_Init ();
    ADC_Init ();

    // Open LCD backlight
    ANA_SetDevValule (ANA_DEV_ID_LCM_BL, ANA_DEV_CLOSE);
    ANA_SetDevValule (ANA_DEV_ID_LCM_BL, ANA_DEV_OPEN);
}

/*****************************************************************************/
//  Description:   Get HW Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHIP_PHY_GetWDGHWFlag (WDG_HW_FLAG_T type)
{
    uint32 val = 0;

    switch (type)
    {
        case TYPE_RESET:
            {
                val = CHIP_PHY_GetHwRstFlag ();
            }
            break;

        case TYPE_BACKLIGHT:
            {
                val = CHIP_PHY_GetHwBLFlag ();
            }
            break;

        case TYPE_DSPEXCEPTION:
            {
                val = CHIP_PHY_GetHwDSPExceptionFlag ();
            }
            break;

        case TYPE_USB:
            {
                val = CHIP_PHY_GetHwUSBFlag ();
            }
            break;

        default:
            {
                SCI_ASSERT (0); /*assert verified*/
            }
            break;
    }

    return val;
}

/*****************************************************************************/
//  Description:    Before watchdog reset, writting HW_RST flag is uesed to j-
//                  udge differrent watchdog reset conditions between MCU reset
//                  and system-halted.
//  Author:         Jeff.Li
//  Modified by:  Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_SetWDGHWFlag (WDG_HW_FLAG_T type, uint32 val)
{
    switch (type)
    {
        case TYPE_RESET:
            {
                CHIP_PHY_SetHwRstFlag (val);
            }
            break;

        case TYPE_BACKLIGHT:
            {
                CHIP_PHY_SetHwBLFlag (val == 0 ? 0:1);
            }
            break;

        case TYPE_DSPEXCEPTION:
            {
                CHIP_PHY_SetHwDSPExceptionFlag (val == 0 ? 0:1);
            }
            break;

        case TYPE_USB:
            {
                CHIP_PHY_SetHwUSBFlag (val == 0 ? 0:1);
            }
            break;

        default:
            {
                SCI_ASSERT (0); /*assert verified*/
            }
            break;
    }

}



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
