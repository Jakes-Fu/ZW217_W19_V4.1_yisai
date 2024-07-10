/******************************************************************************
 ** File Name:      chip_phy_v0.c                                                    *
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
//lint -e766 for "emc_drvapi.h"
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
    {SC6800H,              "SC6800H\n"},
    {SC6800H3,             "SC6800H3\n"}
};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*********************************************************************************************/
//Description:   This function is used to do some dsp init
//Author:
//Note:         
//Parameter:
/*********************************************************************************************/
LOCAL void _CHIP_DSP_Init(void);

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

/*********************************************************************************************/
//Description:   This function is used to initializtion system registers
//Author:
//Note:          Can't use trace and assert in this funciton ;
//Parameter:
/*********************************************************************************************/
LOCAL void _CHIP_PHY_Reg_Init(void)
{
    /*Dsp Auto Power and Switch All switch memory to dsp*/
    CHIP_REG_AND(GR_MISC0, ~(BIT_27 | BIT_28));
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

    chip_id = id;

    switch (chip_id)
    {
        case 0x68000000:
            chip_type = SC6800H;
            break;
        case 0x68030000:
            chip_type = SC6800H3;
            break;
        default:
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

    volatile uint32 clk_reg = REG32 (AHB_CLK_CFG0);

    if (clk_reg & BIT_0)
    {
        uint32 clk_arm_div = (clk_reg>> 1) & 0x3;
        if(SC6800H == CHIP_GetChipType())
        {
            return ( (CHIP_GetMPllClk() >> 1) / (clk_arm_div+1)) ;
        }
        else
        {
            return ( (CHIP_GetMPllClk() >> 2) / (clk_arm_div+1)) ;
        }
    }
    else
    {
        return  ARM_CLK_26M;
    }
}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Younger.yang
// Note :           6600M ARM_CLK/AHB_CLK = 1
/*****************************************************************************/
uint32 CHIP_GetAhbClk (void)
{
    volatile uint32 reg_val = REG32 (AHB_CLK_CFG0);

    if (reg_val & BIT_0)
    {
        return (reg_val & BIT_3) ? (CHIP_GetArmClk() >> 1) : CHIP_GetArmClk();
    }
    else
    {
        return ARM_CLK_26M;
    }
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
    return (
               CHIP_GetAPBClk()
               / ( ( (REG32 (GR_MPLL_MN)) & 0xFFFF0000) >>16) \
               * ( (REG32 (GR_MPLL_MN)) & 0x0000FFFF));
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

    //power down all the ldo
    REG32 (GR_LDO_PD)  = 0x5555555;

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

    REG32 (GR_IRQ) |= APB_MCU_FIQ_EN;

    //Wait for DSP and scheduler wake up
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 30)
        {
            tick2 = SCI_GetTickCount();
        }
    }



    CHIP_REG_OR (GR_SOFT_RST, SWRST_MCU_DSP_RST);
    // Indicate that DSP is resetted.
    REG16 (0x10000FFE) = 0;

    //Disable all fiq src.
    CHIP_DisableFIQ ();

    SCI_DisableIRQ();
    //SCI_DisableFIQ();
    WDG_ResetMCU();
    OS_TickDelay (100);

    //Clear FIQ and IRQ from DSP
    REG32 (GR_IRQ) |= (BIT_1|BIT_0);

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
    CHIP_REG_OR (GR_SOFT_RST, SWRST_MCU_DSP_RST);

    // Indicate that DSP is resetted.
    REG16 (0x10000FFE) = 0;

    //Disable all fiq src.
    CHIP_DisableFIQ ();

    //Clear FIQ and IRQ from DSP
    REG32 (GR_IRQ) |= (BIT_1|BIT_0);


    mcu_uart_reboot();

}


/*********************************************************************************************/
//Description:   This function is used to do some dsp init
//Author:
//Note:         
//Parameter:
/*********************************************************************************************/
LOCAL void _CHIP_DSP_Init(void)
{
    if(SC6800H == CHIP_GetChipType())
    {
        //check dsp status to see if dsp is in exception status
        DSP_PHY_ExceptionCheck();
    }
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
    
    MMU_Init_Phase1();
    /* Init system clock */
    CLOCK_Config();
    // Init Pin function and GPIOs setting
    PM_Init ();

    //init the module of dsp hold arm
    DSP_HoldArmInit();

    //do some dsp init, like dsp status check
    _CHIP_DSP_Init ();
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

    SCI_ASSERT (NULL != chip_info);/*assert verified*/

    for (i=0; i<sizeof (chip_info) /sizeof (CHIP_INFO); i++)
    {
        if (chip_info[i].type == chip_type)
        {
            info = (const char *) chip_info[i].info;
            break;
        }
    }

    SCI_ASSERT (info != NULL);/*assert verified*/

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
    REG32 (0x8b00004c) |= BIT_3;
    return ( ( (REG32 (0x8b000054)) & BIT_1) == BIT_1);
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

    CHIP_REG_OR (GR_IRQ,APB_MCU_FIQ_EN);

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
    addr_base = INTER_SHARE_MEM_BEGIN;
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
    REG32 (GR_MISC0) |= (MISC0_XRAM0_ARM_EN | MISC0_XRAM1_ARM_EN);
    return 0;
}
/*****************************************************************************/
//  Description:    Returns the HW_RST register address.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRstAddr (void)
{
    return GR_HWRST;
}

/*****************************************************************************/
//  Description:    This fuction returns the reset mode value.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRstFlag (void)
{
    return (CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ()) & HW_RST_MASK);
}

/*****************************************************************************/
//  Description:    This fuction returns the backlight mode value.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwBLFlag (void)
{
    return (CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ()) & HW_BL_MASK);
}

/*****************************************************************************/
//  Description:    This fuction returns the dsp exception mode value.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwDSPExceptionFlag (void)
{
    return (CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ()) & HW_DSPEXCEPTION_MASK);
}

/*****************************************************************************/
//  Description:    This fuction returns the usb mode value.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwUSBFlag (void)
{
    return (CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ()) & HW_USB_MASK);
}

/*****************************************************************************/
//  Description:  This fuction set the reset flag of HWRST reg
//  Author:         Jeff.Li
//  Modified by:  Yong.Li
//  Note:           The valid bit filed is from bit4 to bit0
/*****************************************************************************/
LOCAL void CHIP_PHY_SetHwRstFlag (uint32 val)
{
    CHIP_REG_AND (CHIP_PHY_GetHwRstAddr (), ~HW_RST_MASK);
    CHIP_REG_OR (CHIP_PHY_GetHwRstAddr (), (val&HW_RST_MASK));
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
        CHIP_REG_OR (CHIP_PHY_GetHwRstAddr (), HW_BL_MASK);
    }
    else
    {
        CHIP_REG_AND (CHIP_PHY_GetHwRstAddr (), ~HW_BL_MASK);
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
        CHIP_REG_OR (CHIP_PHY_GetHwRstAddr (), HW_DSPEXCEPTION_MASK);
    }
    else
    {
        CHIP_REG_AND (CHIP_PHY_GetHwRstAddr (), ~HW_DSPEXCEPTION_MASK);
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
        CHIP_REG_OR (CHIP_PHY_GetHwRstAddr (), HW_USB_MASK);
    }
    else
    {
        CHIP_REG_AND (CHIP_PHY_GetHwRstAddr (), ~HW_USB_MASK);
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
    CHIP_REG_AND (CHIP_PHY_GetHwRstAddr (), ~0xFFFF);
    CHIP_REG_OR (CHIP_PHY_GetHwRstAddr (), (val&0xFFFF));
}

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_InitModule.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootInitModule ()
{
    // First hold DSP, not let dsp run in unknown status when power up or reset
    DSP_in_Reset();

#ifdef NOR_SRAM_PLATFORM
    EMC_SysStartUp ();
#endif
    MMU_EnableICache();
}

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_IRAM_EN.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootIramEn ()
{
    CHIP_REG_OR (GR_MISC0, (MISC0_XRAM0_ARM_EN | MISC0_XRAM1_ARM_EN));
}

/*****************************************************************************/
// Description :    This function returns whether the watchdog reset is caused
//                  by software reset or system halted.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsWDGRstByMCU (uint32 flag)
{
    if ( (CHIP_PHY_GetHwRstFlag() & HW_RST_MASK) == (flag & HW_RST_MASK))
    {
        return SCI_FALSE;
    }
    else
    {
        return SCI_TRUE;
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
    if ( (CHIP_PHY_GetHwRstFlag() & HW_RST_MASK) == 0)
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
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
    if ( ( (CHIP_PHY_GetHwRstFlag () & HW_RST_MASK) != 0) &&
            ( (CHIP_PHY_GetHwRstFlag () & 0xFF00) == 0))
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
