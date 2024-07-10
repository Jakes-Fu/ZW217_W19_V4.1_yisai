/******************************************************************************
 ** File Name:      chip_phy_v3.c                                             *
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
#include "sc_reg.h"
#include "adi_hal_internal.h"
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
#define HW_RST_MASK       0x1F
#define HW_BL_MASK         BIT_5
#define HW_DSPEXCEPTION_MASK  BIT_6
#define HW_USB_MASK       BIT_7

#define HWRST_ADDR     (DMA_REG_BASE + 0x400 + 31 * 0x20 + 8)  //src addr reg in chn 31

#define ENABLE_DMA_MODULE \
                do{ \
                        if(!(CHIP_REG_GET(AHB_CTL0) & BIT_6)) \
                        { \
                            CHIP_REG_OR(AHB_CTL0, BIT_6);    \
                        } \
                    }while(0);

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
void CHIP_ResetMCU (void)  //reset interrupt disable??
{
    // This loop is very important to let the reset process work well on V3 board
    // @Richard
    uint32 i = 10000;

    ANA_REG_OR (ANA_AGEN, (AGEN_WDG_EN | AGEN_RTC_WDG_EN));
    WDG_TimerInit ();
    
    while (i--) {}    

    WDG_ResetMCU ();
    
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
//  Note :          Because there is no register which can restore information
//                  when watchdog resets the system, so we choose IRAM.
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHwRstAddr (void)
{
    // Returns a DWORD of IRAM shared with DCAM
    return HWRST_ADDR;
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
LOCAL uint32 CHIP_PHY_GetANAReg (void)
{
    uint32 adi_rd_data;

    // Set read command
    * (volatile uint32 *) ADI_ARM_RD_CMD = ANA_HWRST_STATUS;

    // Wait read operation complete, RD_data[31] will be cleared after the read 
    // operation complete
    do
    {
        adi_rd_data = * (volatile uint32 *) ADI_RD_DATA;
    }
    while (adi_rd_data & BIT_31);

    return ((adi_rd_data & 0x0000FFFF));
}

/*****************************************************************************/
//  Description:    This fuction returns the HW_RST value written before reset.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 CHIP_PHY_GetHWFlag (void)
{
    ENABLE_DMA_MODULE
        
    return CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ());
}

/*****************************************************************************/
//  Description:    PHY layer realization of BOOT_ResetHWFlag. It resets the HW
//                  reset register after system initialization.
//  Author:         Jeff.Li
//  Note:           The valid bit filed of analog register is from bit11 to bit0.
//                  | 11   10   9   8 |  7   6   5   4  |  3   2   1   0   |
//                  |ALL_HRST_MONITOR | POR_HRST_MONITOR| WDG_HRST_MONITOR |
//
//                  The valid bit filed of HW_RST is from bit11 to bit0.
/*****************************************************************************/
PUBLIC void CHIP_PHY_ResetHWFlag (uint32 val)
{
    //set WDG Mon to 0xF
    ANA_REG_AND (ANA_HWRST_STATUS, ~0xF);
    ANA_REG_OR (ANA_HWRST_STATUS, (val&0xF));
    
    ENABLE_DMA_MODULE

    CHIP_REG_AND(CHIP_PHY_GetHwRstAddr(), ~HW_RST_MASK);
    CHIP_REG_OR(CHIP_PHY_GetHwRstAddr(), (val & HW_RST_MASK));
}

/*****************************************************************************/
//  Description:    PHY layer realization of BOOT_SetWDGHWFlag. It Writes flag
//                  to the register which would not be reset by watchdog reset.
//  Author:         Jeff.Li
//  Note:           The valid bit filed is from bit15 to bit0
/*****************************************************************************/
PUBLIC uint32 CHIP_PHY_GetWDGHWFlag (WDG_HW_FLAG_T type)
{
    uint32 ret = 0;
    if(TYPE_RESET == type)
    {
        ENABLE_DMA_MODULE        

        ret = CHIP_REG_GET (CHIP_PHY_GetHwRstAddr ()); 
    }
    else
    {
        //wrong type, TODO
    }
    return ret;
}

/*****************************************************************************/
//  Description:    PHY layer realization of BOOT_SetWDGHWFlag. It Writes flag
//                  to the register which would not be reset by watchdog reset.
//  Author:         Jeff.Li
//  Note:           The valid bit filed is from bit15 to bit0
/*****************************************************************************/
PUBLIC void CHIP_PHY_SetWDGHWFlag (WDG_HW_FLAG_T type, uint32 val)
{
    if(TYPE_RESET == type)
    {
        ENABLE_DMA_MODULE        

        CHIP_REG_AND(CHIP_PHY_GetHwRstAddr(), ~HW_RST_MASK);
        CHIP_REG_OR(CHIP_PHY_GetHwRstAddr(), val & HW_RST_MASK);   
    }
    else
    {
        //wrong type, TODO
    }
}


/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_IRAM_EN.
//  Author:         Jeff.Li
//  Note:           Do nothing. There are 32KB internal ram dedicated for ARM.
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootIramEn ()
{
}

/*****************************************************************************/
// Description :    This function returns whether the watchdog reset is caused
//                  by software reset or system halted.
// Author :         Jeff.Li
// Note :           The valid bit filed is from bit15 to bit0
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsWDGRstByMCU (uint32 flag)
{
    ENABLE_DMA_MODULE
        
    if ((CHIP_PHY_GetHWFlag() & HW_RST_MASK) == (flag & HW_RST_MASK))
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
// Note :           | 11   10   9   8 |  7   6   5   4  |  3   2   1   0   |
//                  |ALL_HRST_MONITOR | POR_HRST_MONITOR| WDG_HRST_MONITOR |
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByPowerUp()
{
    if ((CHIP_PHY_GetANAReg () & 0xF0) == 0x0)
    {
        //HWRST reg is 0 when Power On, and to be 1 in other cases 
        ANA_REG_OR(ANA_HWRST_STATUS, (1 << 4));
        
        //clear reg in dma chn
        ENABLE_DMA_MODULE
        CHIP_REG_SET(CHIP_PHY_GetHwRstAddr(), 0);
        
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
// Note :           | 11   10   9   8 |  7   6   5   4  |  3   2   1   0   |
//                  |ALL_HRST_MONITOR | POR_HRST_MONITOR| WDG_HRST_MONITOR |
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByWatchDog()
{
    if ((CHIP_PHY_GetANAReg () & 0xF) == 0x0)
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
