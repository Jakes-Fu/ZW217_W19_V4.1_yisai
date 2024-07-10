/******************************************************************************
 ** File Name:      chip_phy_v1.c                                                    *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/14/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
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
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "fiq_drvapi.h"
#include "busmonitor_drvapi.h"
#include "chip.h"

#include "mcu_reboot.h"
#include "chng_freq.h"
#include "sim_drv.h"
#include "os_param.h"
#include "dma_drv_internal.h"
#include "wdg_drvapi.h"
#include "vb_drvapi.h"
#include "pwm_drvapi.h"
#include "boot_drvapi.h"
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


/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/

typedef enum
{
    CORE_VOLTAGE_1800MV = 0,
    CORE_VOLTAGE_1700MV,
    CORE_VOLTAGE_1600MV,
    CORE_VOLTAGE_1500MV
}
CORE_VOLTAGE_E;

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 chip_id = 0;
LOCAL CHIP_TYPE_E chip_type = CHIP_TYPE_MAX;

const CHIP_INFO chip_info[] =
{
    {SC6600R1,      "SC6600R1\n"        },
    {SC6600R2,      "SC6600R2\n"        },
    {SC6600R2A,     "SC6600R2A\n"       },
    {SC6600R3,      "SC6600R3\n"        },
    {SC6600R3A,     "SC6600R3A\n"       },
    {SC6600L,       "SC6600L\n"         },
    {SC6600L2,      "SC6600L2\n"        },
    {SC6600H3A,     "SC6600H3A\n"       },
    {SC6600W2,      "SC6600W2\n"        },
    {SC6600L7,      "SC6600L7\n"        }
};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
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
        case 0x66007000:
            chip_type = SC6600L;
            break;

        case 0x66007001:
            chip_type = SC6600L2;
            break;

        case 0x66007009:
            chip_type = SC6600L7;
            break;

        case 0x66007005:
            chip_type = SC6600H3A;
            break;

        case 0x66007002:
            chip_type = SC6600W2;
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

    bSDRAM = FALSE;//only support norflash
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
PUBLIC void CHIP_ChangeCoreVoltage (CORE_VOLTAGE_E core_voltage) //lint !e765
{
    uint32 ldo_ctl1_val;

    ldo_ctl1_val = REG32 (GR_LDO_CTL1) & 0xfffffff0;

    switch (core_voltage)
    {

        case CORE_VOLTAGE_1800MV:
            ldo_ctl1_val |= (BIT_1 | BIT_3);
            break;
        case CORE_VOLTAGE_1700MV:
            ldo_ctl1_val |= (BIT_0 | BIT_3);
            break;
        case CORE_VOLTAGE_1600MV:
            ldo_ctl1_val |= (BIT_1 | BIT_2);
            break;
        case CORE_VOLTAGE_1500MV:
            ldo_ctl1_val |= (BIT_0 | BIT_2);
            break;
        default:
            break;

    }

    REG32 (GR_LDO_CTL1) = ldo_ctl1_val;

}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
uint32 CHIP_GetArmClk (void)
{
    uint32 clk_mcu_sel = 0;
    uint8  pll_sel     = 0;
    uint32 clk_mcu     = ARM_CLK_26M;

    pll_sel     =  REG32 (GR_GEN0) & GEN0_PLL_SEL; /*lint !e737*/
    clk_mcu_sel = REG32 (GR_PCTL) & 0x03;

    switch (clk_mcu_sel)
    {
        case 0:
            clk_mcu = ARM_CLK_26M;
            break;
        case 1:
            clk_mcu = ARM_CLK_48M;
            break;
        case 2:

            if (pll_sel)
            {
                clk_mcu = ARM_CLK_72M;
            }
            else
            {
                clk_mcu = ARM_CLK_60M;
            }

            break;
        case 3:

            if (pll_sel)
            {
                clk_mcu = ARM_CLK_72M;
            }
            else
            {
                clk_mcu = ARM_CLK_80M;
            }

            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;

    }

    return clk_mcu;

}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Younger.yang
// Note :           6600M ARM_CLK/AHB_CLK = 1
/*****************************************************************************/
uint32 CHIP_GetAhbClk (void)
{
    return (CHIP_GetArmClk());
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
               / ( (REG32 (GR_MPLL_MN) & 0xFFFF0000) >>16) \
               * (REG32 (GR_MPLL_MN) & 0x0000FFFF));
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

    //close otp
    REG32 (GR_LDO_CTL1) |= BIT_31;
    //power down all the ldo
    REG32 (GR_LDO_CTL0) = 0x40055555;

    while (1){}

}


/*****************************************************************************/
// Description :    This function is used to reset MCU.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
/*****************************************************************************/
// Description :    This function is used to reset MCU.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
extern uint32 OS_TickDelay (uint32 ticks);
void CHIP_ResetMCU (void)
{
    // This loop is very important to let the reset process work well on V3 board
    // @Richard
    uint32 i = 10000;

    while (i--){}

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

    REG32 (GR_IRQ) |= 0x8;



    //Wait for DSP and scheduler wake up
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 30)
        {
            tick2 = SCI_GetTickCount();
        }
    }



    // Hold dsp
    CHIP_REG_OR (GR_GEN0, GEN0_MCU_DSP_RST);
    // Indicate that DSP is resetted.
    REG16 (0x10000FFE) = 0;

    //Disable all fiq src.
    CHIP_DisableFIQ ();
    //#ifdef PLATFORM_SC6600L
    SCI_DisableIRQ();

    OS_TickDelay (1);
    //SCI_DisableFIQ();
    WDG_ResetMCU();
    OS_TickDelay (100);

    REG32 (GR_ICLR) |= (BIT_1|BIT_0);


    //Reset DMA  //Reset DMA
    REG32 (AHB_SOFT_RST) |= (uint32) AHB_DMA_SOFT_RST;

    for (i=0; i<10; i++);//lint !e722

    REG32 (AHB_SOFT_RST) &=~ (uint32) AHB_DMA_SOFT_RST;


    if (CHIP_DetectMemType())
    {
        mcu_nand_reboot();
    }
    else
    {
        EMC_Close();
        __tx_reset();   // Jump to 0
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
    CHIP_REG_OR (GR_GEN0, GEN0_MCU_DSP_RST);

    // Indicate that DSP is resetted.
    REG16 (0x10000FFE) = 0;

    //Disable all fiq src.
    CHIP_DisableFIQ ();

    //Clear FIQ and IRQ from DSP
    REG32 (GR_ICLR) |= (BIT_1|BIT_0);
    REG32 (GR_GEN2) |= BIT_0;        //Enable on-chip ram 0x40000000

    if (!CHIP_DetectMemType())
    {
        EMC_Close();
    }

    mcu_uart_reboot();

}

/*********************************************************************************************/
//Description:   This function is used to initializtion system clk and other
//Author:
//Note:          Can't use trace and assert in this funciton ;
//Parameter:
/*********************************************************************************************/
void CHIP_Init (void)
{
    // Init Pin function and GPIOs setting
    PM_Init ();
    /* Init system clock */
    CLOCK_Config();
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

    if (is_active)
    {
        REG32 (0x8B00000C) |= BIT_9;
    }
    else
    {
        REG32 (0x8B00000C) &= ~BIT_9;
    }

}

/*****************************************************************************/
// Description :    This function is Get Usb DM Value
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_GetUsbDMValue (void)
{
    REG32 (0x8b00001c) |= BIT_27;
    return ( (REG32 (0x8b00001c) & BIT_12) == BIT_12);
}

/*****************************************************************************/
//  Description:   This function is used to do something before assert.
//  Global resource dependence:
//  Author:         Zhengjiang.Lei
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_BeforeAssert (void)
{
#ifndef WIN32
    uint8 assert_val;
    //in other platform, deal relative case here ...

    OSPARAM_GetAssertMode (&assert_val);

    if (SCI_ASSERT_DEBUG_MODE == assert_val)
    {
        // If audio device is being controlled by DSP now, switch it back to
        // ARM, and close the device. If it is not being controlled by DSP,
        // the function retruns FALSE.
        AUDDEV_Close (AUDDEV_FUNC_DSP_CTL, SCI_TRUE);

        // power off sim
        SIM_PowerOffSIM();
    }

    bm_disable();

#endif
}

/*below 3 functions are removed from driver_export.c, wait clean up*/
// The function is used to reset dsp when arm system enter assert mode
PUBLIC BOOLEAN CHIP_ResetDspInAssertMode (void)
{
    BOOLEAN is_dsp_respond = SCI_FALSE;

#ifndef WIN32
    //@Steven
    //SEND_REQ_TO_DSP1(REF_CMD_ASSERT);
    cmd_ptr->RefCmd_block.cmd_done = 0;
    cmd_ptr->RefCmd_block.Cmd_ID = REF_CMD_ASSERT;

    * (volatile uint32 *) (0x8b000010) |= 0x8;

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

#endif // WIN32

    return is_dsp_respond;
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
    REG32 (GR_GEN2) |= BIT_0;
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
//  Description:    Returns the reset mode register address.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetRstModeAddr (void)
{
    return GR_HWRST;
}

/*****************************************************************************/
//  Description:    This fuction returns the HW_RST value written before reset.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHWFlag (void)
{
    return CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ());
}

/*****************************************************************************/
//  Description:    PHY layer realization of BOOT_SetRstMode.
//  Author:         Jeff.Li
//  Note:           The valid bit filed is from bit15 to bit0
/*****************************************************************************/
PUBLIC void CHIP_PHY_SetRstMode (uint32 val)
{
    CHIP_REG_AND (CHIP_PHY_GetRstModeAddr (), ~0xFFFF);
    CHIP_REG_OR (CHIP_PHY_GetRstModeAddr (), (val&0xFFFF));
}

/*****************************************************************************/
//  Description:    This fuction returns the reset mode value.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHIP_PHY_GetRstMode (void)
{
    return (CHIP_REG_GET (CHIP_PHY_GetRstModeAddr ()) & 0xFFFF);
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
//  Description:    PHY layer realization of BOOT_SetWDGHWFlag. It Writes flag
//                  to the register which would not be reset by watchdog reset.
//  Author:         Jeff.Li
//  Note:           The valid bit filed is from bit7 to bit0
/*****************************************************************************/
PUBLIC void CHIP_PHY_SetWDGHWFlag (WDG_HW_FLAG_T type, uint32 val)
{
    if(TYPE_RESET == type)
    {
        CHIP_REG_AND (CHIP_PHY_GetHwRstAddr (), ~0xFF);
        CHIP_REG_OR (CHIP_PHY_GetHwRstAddr (), (val&0xFF));
    }
    else
    {
        //wrong type, TODO
    }
}
    

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_InitModule.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootInitModule ()
{
    EMC_SysStartUp ();
}

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_IRAM_EN.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootIramEn ()
{
    CHIP_REG_OR (GR_GEN2, BIT_0);
}

/*****************************************************************************/
// Description :    This function returns whether the watchdog reset is caused
//                  by software reset or system halted.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsWDGRstByMCU (uint32 flag)
{
    // Copy the value of HW_RST register to the register specific to reset mode
    CHIP_REG_SET (CHIP_PHY_GetRstModeAddr (),
                  CHIP_PHY_GetHWFlag ());

    if ( (CHIP_PHY_GetHWFlag () & 0xFF) == (flag & 0xFF))
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
    if ( (CHIP_PHY_GetHWFlag () & 0xFF) == 0)
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
    if ( ( (CHIP_PHY_GetHWFlag () & 0xFF) != 0) &&
            ( (CHIP_PHY_GetHWFlag () & 0xFF00) == 0))
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
