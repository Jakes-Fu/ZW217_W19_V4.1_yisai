/******************************************************************************
 ** File Name:      chip_phy_v2.c                                             *
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
#include "sc_reg.h"
#include "chip.h"
#include "wdg_drvapi.h"
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
/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to reset MCU.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_ResetMCU (void)
{
    uint32 i = 10000;

    CHIP_REG_OR (GR_GEN0,  GEN0_WDG_EN);
    
    while (i--){}

    WDG_ResetMCU();
    
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 500)
        {
            tick2 = SCI_GetTickCount();
        }
    }

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
//  Description:    Returns the HW_RST1 register address.
//  Author:         Jeff.Li
//  Note:           Just defined in SC8800H platform
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRst1Addr (void)
{
    return GR_HWRST1;
}

/*****************************************************************************/
//  Description:    This fuction returns the HW_RST value written before reset.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHWFlag (void)
{
    CHIP_REG_OR (AHB_CTL0, BIT_6);
    return CHIP_REG_GET (CHIP_PHY_GetHwRst1Addr ());
}

/*****************************************************************************/
//  Description:    PHY layer realization of BOOT_SetRstMode.
//  Author:         Jeff.Li
//  Note:           The "mask" and "val" should be the same bit filed
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
    // Set the watchdog reset flag, which would not be cleared by watchdog reset
    REG32 (GR_CLK_EN) |= BIT_2;

    CHIP_REG_AND (CHIP_PHY_GetHwRstAddr (), ~0xFFFF);
    CHIP_REG_OR (CHIP_PHY_GetHwRstAddr (), (val&0xFFFF));

    if (CHIP_REG_GET (AHB_CTL0) & BIT_6)
    {
        CHIP_REG_AND (CHIP_PHY_GetHwRst1Addr (), ~0xFFFF);
        CHIP_REG_OR (CHIP_PHY_GetHwRst1Addr (), (val&0xFFFF));
    }
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

        if (CHIP_REG_GET (AHB_CTL0) & BIT_6)
        {
            CHIP_REG_AND (CHIP_PHY_GetHwRst1Addr (), ~0xFF);
            CHIP_REG_OR (CHIP_PHY_GetHwRst1Addr (), (val&0xFF));
        }
    }
    else
    {
        //wrong type, TODO
    }
}

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_IRAM_EN.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootIramEn ()
{
    // Enable internal RAM from ISP, AHB ctrl1 reg bit[0,1,2,3]
    CHIP_REG_OR (AHB_CTL1, 0x0F);
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
    if (!(CHIP_REG_GET (GR_CLK_EN) & BIT_2))
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
    if (CHIP_REG_GET (GR_CLK_EN) & BIT_2)
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
