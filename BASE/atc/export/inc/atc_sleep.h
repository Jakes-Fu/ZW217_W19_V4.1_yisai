/******************************************************************************
 ** File Name:      atc_sleep.h                                               *
 ** Author:         Xueliang.Wang                                             *
 ** Date:           09/19/2003                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 09/19/2003     Xueliang.Wang    Create.                                   *
 ******************************************************************************/

#ifndef _ATC_SLEEP_H
#define _ATC_SLEEP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"

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

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to set atc sleep switch flag.
//                  If the flag is set to SCI_FALSE, the MCU willn't enter 
//                  sleep mode.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_ATC_SetSleepSwitchFlag(
    BOOLEAN b_enter_sleep   // The value used to set the flag.
    );

/*****************************************************************************/
//  Description:    This function is used to get atc sleep switch flag.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN SCI_ATC_GetSleepSwitchFlag(void);

/*****************************************************************************/
//  Description:    This function is used to check if hardware allow MCU 
//                  enter sleep mode.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN SCI_ATC_IsHwAllowEnterSleep(void);   // Return SCI_TRUE:  allow
                                                    // SCI_FALSE: don't allow

/*****************************************************************************/
//  Description:    This function is used to set the sleep status when it 
//                  enter sleep mode or waken up from sleep mode.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_ATC_SetSleepSts(
    BOOLEAN b_enter_sleep   // SCI_TRUE    enter sleep mode.
                            // SCI_FALSE   wake up from sleep mode.
    );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // _ATC_SLEEP_H