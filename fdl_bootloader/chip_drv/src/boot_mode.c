/******************************************************************************
 ** File Name:      boot_mode.c                                               *
 ** Author:         Jeff Li                                                   *
 ** DATE:           31/08/2010                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the logic interfaces called during boot,*
 **                 including reset mode setting, initialization etc.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 31/08/2010     Jeff.Li          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sc_reg.h"
#include "chip_internal.h"
//#include "pwm_drvapi.h"
#include "boot_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN wdg_debug_mode = SCI_FALSE;

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    After normal power on, the HW_RST flag should be reset in
//                  order to judge differrent reset conditions between normal
//                  power on reset and watchdog reset.
//  Author:         Jeff.Li
//  Note:           
/*****************************************************************************/
PUBLIC void BOOT_ResetHWFlag (void)
{
    CHIP_PHY_ResetHWFlag (0x1FFF);
}

/*****************************************************************************/
//  Description:    Before watchdog reset, writting HW_RST flag is uesed to j-
//                  udge differrent watchdog reset conditions between MCU reset
//                  and system-halted.
//  Author:         Jeff.Li
//  Note:           
/*****************************************************************************/
PUBLIC void BOOT_SetWDGHWFlag (WDG_HW_FLAG_T type, uint32 val)
{
        CHIP_PHY_SetWDGHWFlag (type, val);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
