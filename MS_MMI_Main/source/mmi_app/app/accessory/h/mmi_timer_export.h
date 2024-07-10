/*****************************************************************************
** File Name:      mmi_timer_export.h                                               
** Author:         yaye.jiang    
** Date:           2011/8/1
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe gui common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/12/14     zhaohui          Create   															 *
** 2012/02/09     murphyxie        Change name to mmi_timer_export.h 	     *
******************************************************************************/

#ifndef _MMITIMER_EXPORT_H_
#define _MMITIMER_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmk_msg.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//#define TIMER_TAB_HEIGHT 50//设定tab高度，用于设置RECT
//#define TIMER_TAB_HEIGHT ((MMITHEME_IsIstyle())?MMI_ISTYLE_TAB_HEIGHT:MMITAB_DEFAULT_ITEM_HEIGHT)
typedef  enum 
{ 
#ifdef WORLD_CLOCK_SUPPORT
    MMI_WC_TIMER,//world clock
#endif
    MMI_AL_TIMER, //alarm
    MMI_ST_TIMER,//stopwatch
    MMI_CT_TIMER, //countdown
	MMI_ACC_TIMER_MAX = 4
} MMI_CUR_TIMER_E;

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#if (!defined MMI_ALARM_MINI_SUPPORT) && (!defined MMI_WORLDCLOCK_MINI_SUPPORT) && (!defined MMI_COUNTEDTIME_MINI_SUPPORT)
/*****************************************************************************/
//  Description : Open StopWatch Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPI_OpenTimerWin( uint32 index );
#endif
/*****************************************************************************/
//  Description : Open StopWatch Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPI_OpenTimerWinByType( MMI_CUR_TIMER_E type );

#ifdef   __cplusplus
    }
#endif

#endif  

