/******************************************************************************
 ** File Name:      ref_engineering.h                                         *
 ** Author:         Sunsome.Ju                                                *
 ** DATE:           10/21/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for engineering mode.                         *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/21/2004     Sunsome.Ju       Create.                                   *
 ******************************************************************************/
#ifndef _REF_ENGINEERING_H_
#define _REF_ENGINEERING_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#ifndef WIN32
#include "com_drvapi.h"
#include "chip.h"
#include "tx_api_thumb.h"
//#define SCI_DEEP_SLEEP
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

// Enable Audio deep sleep
void SCI_ENGINEER_EnableDeepSleep (uint32 mode);

// Get Arm Sleep Flag
PUBLIC BOOLEAN SCI_ENGINEER_GetEnableArmSleepFlag (void);

/*****************************************************************************/
//  Description:    This function handle log switch for engineering mode use.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_SetArmLogFlag (
    BOOLEAN b_log   // SCI_TRUE: log        SCI_FALSE: not log
);

#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif //

#endif // End of _DEEP_SLEEP_H