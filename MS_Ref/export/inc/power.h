/******************************************************************************
 ** File Name:      power.h                                                   *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           06/03/2002                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/03/2002     Xueliang.Wang    Create.                                   *
 ******************************************************************************/

#ifndef _POWER_H
#define _POWER_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "dal_power.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum
{
    POWER_MS_STATUS_INIT,
    POWER_MS_STATUS_HANDLE_POWER_ON,
    POWER_MS_STATUS_HANDLE_POWER_OFF,
    POWER_MS_STATUS_POWER_ON,
    POWER_MS_STATUS_POWER_OFF
} POWER_MS_STATUS_E;

typedef void (* NET_EXIT_FPTR)(int);


typedef void (* PCALCULATE_STANDBY_TIME)(void);

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    Handle power on operate.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void  POWER_PowerOn(void);

PUBLIC void  POWER_FOTA_Update_Reset(void);

/*****************************************************************************/
// Description :    Handle reset operate, reset from boot mode.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void  POWER_BootReset(void);

/*****************************************************************************/
// Description :    TCP/IP regist power off function.
//                  Function prototype: void func_name(int)
// Global resource dependence :
// Author : Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void POWER_RegNetExit(NET_EXIT_FPTR net_exit);

/*****************************************************************************/
// Description :    This function sets restart condition.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void POWER_SetRestartCondition(POWER_RESTART_CONDITION_E condition);

/*****************************************************************************/
// Description :    Get the status of MS.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC POWER_MS_STATUS_E POWER_GetMsStatus(void);

/*****************************************************************************/
// Description :    Set the status of MS.
// Global resource dependence :
// Author : Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void POWER_SetMsStatus(POWER_MS_STATUS_E status);



PUBLIC MCU_MODE_E POWER_GetResetMode(void);

/*****************************************************************************/
//  Description:  The function used to register the calculate standby time function
//	Global resource dependence:
//  Author: rex yang
//	Note:
/*****************************************************************************/
PUBLIC void Power_RegFunCalculateStandby(PCALCULATE_STANDBY_TIME pcalculatestandby);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // End power.h
