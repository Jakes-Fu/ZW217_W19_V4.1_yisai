/******************************************************************************
 ** File Name:      dal_power.h                                               *
 ** Author:         Richard.Yang                                              *
 ** DATE:           10/11/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 10/11/2004      Richard.Yang    Create.                                   *
 ******************************************************************************/
#ifndef _DAL_POWER_H
#define _DAL_POWER_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "cmddef.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef enum
{
    RESTART_BY_NONE,
    RESTART_BY_POWER_BUTTON,
    RESTART_BY_ALARM,
    RESTART_BY_CHARGE,
    RESTART_BY_SOFTWARE,
    RESTART_BY_ASSERT
} POWER_RESTART_CONDITION_E;

/*
* Enum for dsp running code type
* DSP_INIT_DUMMY_MODE is used for downloading dsp always sleep code in charging mode
*/
typedef enum
{
    DSP_INIT_NORMAL_MODE = 0x0,
    DSP_INIT_DUMMY_MODE,
    DSP_INIT_MAX_VAL
} DSP_INIT_CODE_TYPE_E;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    The function power off phone
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void  POWER_PowerOff(void);

/*****************************************************************************/
// Description :    This function gets restart condition.
// Global resource dependence : 
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
POWER_RESTART_CONDITION_E POWER_GetRestartCondition(void);

/*****************************************************************************/
// Description :    Handle reset operate.
// Global resource dependence : 
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void  POWER_Reset(void);

/*****************************************************************************/
//  Description:    The function used to set mcu reset mode.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void POWER_SetResetMode(MCU_MODE_E mode);

/*****************************************************************************/
//  Description:    The function used to get mcu reset mode.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC MCU_MODE_E POWER_GetResetMode(void);

/*****************************************************************************/
//  Description:    The function used to Get the frequency parameter set in calibration post mode.
//	Global resource dependence: 
//  Author:        king.li
//	Note:    2008.02.18              
/*****************************************************************************/
PUBLIC uint16 POWER_GetCalibrationPostModeFreq(void);

/*****************************************************************************/
//  Description:    The function used to set the frequency parameter in calibration post mode.
//	Global resource dependence: 
//  Author:        king.li
//	Note:    2008.02.18              
/*****************************************************************************/
PUBLIC void POWER_SetCalibrationPostModeFreq(uint16 freq);

/*****************************************************************************/
//  Description:    The function used to decide whether  run in auto test mode.
//	Global resource dependence: 
//  Author:        king.li
//	Note:    2008.02.18              
/*****************************************************************************/
PUBLIC BOOLEAN POWER_IsAutoTestMode(void);

/*****************************************************************************/
//  Description:    The function used to decide whether  run in auto test mode.
//	Global resource dependence: 
//  Author:        king.li
//	Note:    2008.02.18              
/*****************************************************************************/
PUBLIC BOOLEAN POWER_SetAutoTestMode();
PUBLIC BOOLEAN POWER_ClearAutoTestMode();

/*****************************************************************************/
//  Description:    The function used to check if in calibration mcu reset mode.
//	Global resource dependence: 
//  Author:         Liangwen.Zhen
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN POWER_IsCalibrationResetMode(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // _DAL_POWER_H
