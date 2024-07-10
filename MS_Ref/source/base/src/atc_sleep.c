/******************************************************************************
 ** File Name:      atc_sleep.c                                               *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           09/22/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file include the functions used to control saving    *
 **                 power.                                                    *
 ******************************************************************************
 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/22/2003     Xueliang.Wang    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "atc_sleep.h"
#include "deep_sleep.h"
#include "arm_reg.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

//#define ATCSLEEP_DEBUG

#ifdef ATCSLEEP_DEBUG
	#define ATC_PRINT		SCI_TRACE_LOW
#else
	#define ATC_PRINT	
#endif	// ATCSLEEP_DEBUG

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN s_sleep_switch_flag;
 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
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
    )
{
    return;
}

/*****************************************************************************/
//  Description:    This function is used to get atc sleep switch flag.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN SCI_ATC_GetSleepSwitchFlag(void)
{
    return SCI_FALSE;
}

/*****************************************************************************/
//  Description:    This function is used to check if hardware allow MCU 
//                  enter sleep mode.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN SCI_ATC_IsHwAllowEnterSleep(void)    // Return SCI_TRUE:  allow
                                                    // SCI_FALSE: don't allow
{
    return TRUE;
}

/*****************************************************************************/
//  Description:    This function is used to set the sleep status when it 
//                  enter sleep mode or waken up from sleep mode.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_ATC_SetSleepSts(
    BOOLEAN b_enter_sleep   // SCI_TRUE    enter sleep mode.
                            // SCI_FALSE   wake up from sleep mode.
    )
{
    return;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
