/******************************************************************************
 ** File Name:      chip_internal.h                                           *
 ** Author:         Jeff Li                                                   *
 ** DATE:           31/08/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    Chip set phy interface declare for internal driver use.   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 31/08/2010     Jeff.Li          Create.                                   *
 ******************************************************************************/
#ifndef _CHIP_INTERNAL_H_
#define _CHIP_INTERNAL_H_

#include "pwm_drvapi.h"
#include "boot_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    PHY layer realization of BOOT_ResetHWFlag. It resets the HW
//                  reset register after system initialization.
//  Author:         Jeff.Li
//  Note:           The valid bit filed is from bit15 to bit0
/*****************************************************************************/
PUBLIC void CHIP_PHY_ResetHWFlag (uint32 val);

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
//  Description:    This fuction returns the HW_RST value written before reset.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
//PUBLIC uint32 CHIP_PHY_GetHWFlag (void);

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_InitModule.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootInitModule (void);

/*****************************************************************************/
//  Description:    PHY layer realization of __BOOT_IRAM_EN.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_PHY_BootIramEn (void);

/*****************************************************************************/
// Description :    This function returns whether the watchdog reset is caused
//                  by software reset or system halted.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsWDGRstByMCU (uint32 flag);

/*****************************************************************************/
// Description :    This function returns whether the reset is caused by power
//                  up.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByPowerUp (void);

/*****************************************************************************/
// Description :    This function returns whether the reset is caused by watch-
//                  dog reset.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_PHY_IsResetByWatchDog (void);

/*****************************************************************************/
// Description :    This function initializes the modules before assert while
//                  the system has just been reset.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void CHIP_PHY_WDGAssertInit (void);

/*****************************************************************************/
//  Description:    ADC Sample Prepare Enter
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM) || defined(PLATFORM_UWS6121E)
PUBLIC int CHIP_ADC_Sample_Prepare_Enter (void);
#else
static __inline int CHIP_ADC_Sample_Prepare_Enter (void)
{
    return 0;
}
#endif

/*****************************************************************************/
//  Description:    ADC Sample Prepare Exit
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)|| defined(PLATFORM_UWS6121E)
PUBLIC int CHIP_ADC_Sample_Prepare_Exit (void);
#else
static __inline int CHIP_ADC_Sample_Prepare_Exit (void)
{
    return 0;
}
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif

