/******************************************************************************
 ** File Name:      boot_drvapi.h                                               *
 ** Author:         Jeff Li                                                   *
 ** DATE:           31/08/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the logic interfaces called during boot,*
 **                 including reset mode setting, initialization etc.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 31/08/2010     Jeff.Li          Create.                                   *
 ******************************************************************************/
#ifndef _BOOT_DRVAPI_H_
#define _BOOT_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

typedef enum {
    TYPE_RESET = 0,           //bit[4:0]
    TYPE_BACKLIGHT,         //bit[5]
    TYPE_DSPEXCEPTION,  //bit[6]
    TYPE_USB,                    //bit[7]
    TYPE_MAX
} WDG_HW_FLAG_T;

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
PUBLIC void BOOT_ResetHWFlag (void);

/*****************************************************************************/
//  Description:   Get HW Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetWDGHWFlag (WDG_HW_FLAG_T type);

/*****************************************************************************/
//  Description:    Before watchdog reset, writting HW_RST flag is uesed to j-
//                  udge differrent watchdog reset conditions between MCU reset
//                  and system-halted.
//  Author:         Jeff.Li
//  Modified by:  Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetWDGHWFlag (WDG_HW_FLAG_T type, uint32 val);

/*****************************************************************************/
//  Description:    After normal power on, the HW_RST flag should be reset in
//                  order to judge differrent reset conditions between normal
//                  power on reset and watchdog reset.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_InitResetMode (void);

/*****************************************************************************/
//  Description:   Get reset mode
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetResetMode (void);

/*****************************************************************************/
//  Description:    Set reset mode
//  Author:         Jeff.Li
//  Modified by:  Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetResetMode (uint32 val);

/*****************************************************************************/
//  Description:   Get BL status flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetBLStatus (void);

/*****************************************************************************/
//  Description:    Set BL status flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetBLStatus (uint32 val);

/*****************************************************************************/
//  Description:   Get DSP exception status Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetDSPExceptionStatus (void);

/*****************************************************************************/
//  Description:    Set DSP exception status Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetDSPExceptionStatus (uint32 val);

/*****************************************************************************/
//  Description:   Get USB connect status Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetUSBConnectStatus (void);

/*****************************************************************************/
//  Description:    Set USB connect status Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetUSBConnectStatus (uint32 val);

/*****************************************************************************/
//  Description:    Gets the flag.
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GetWatchdogMode (void);

/*****************************************************************************/
//  Description:    Sets the flag when watchdog resets the system in debug mode.
//  Author:
/*****************************************************************************/
PUBLIC void SetWatchdogMode (BOOLEAN mode);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_BOOT_DRVAPI_H_

