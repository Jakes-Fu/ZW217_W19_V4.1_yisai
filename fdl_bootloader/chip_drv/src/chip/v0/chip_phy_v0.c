/******************************************************************************
 ** File Name:      chip_phy_v0.c                                                    *
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
//#include "pwm_drvapi.h"
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
    // This loop is very important to let the reset process work well on V3 board
    // @Richard
    uint32 i = 10000;

    CHIP_REG_OR (GR_GEN0,  GEN0_WDG_EN);
    CHIP_REG_OR (GR_GEN0,  GEN0_WDG_RTC_EN);
    
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
//  Description:    This fuction returns the HW_RST value written before reset.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHWFlag (void)
{
    return CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ());
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
