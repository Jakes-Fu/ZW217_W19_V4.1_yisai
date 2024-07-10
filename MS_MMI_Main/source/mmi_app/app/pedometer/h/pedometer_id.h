/*****************************************************************************
** File Name:      mmiacc_id.h                                               *
** Author:         xiaoqing.lu                                               *
** Date:           2009/04/03                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ACC win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/04/03     xiaoqing.lu       Create                                   *
******************************************************************************/

#ifndef _MMIHELLO_ID_H_
#define _MMIHELLO_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

#define WIN_ID_DEF(win_id)          win_id

// 窗口ID
typedef enum
{
    PEDOMETER_WIN_ID_START = (MMI_MODULE_PEDOMETER << 16),

	MMI_PEDOMETER_MAIN_WIN_ID,

	MMI_PEDOMETER_LOADING_WIN_ID,//加载界面
        
    MMI_PEDOMETER_TODAY_EXERCISE_WIN_ID,//今日运动

	MMI_PEDOMETER_STEP_WEEK_WIN_ID,//周步数统计

	MMI_PEDOMETER_STEP_LIST_WIN_ID,//好友步数排行榜

    PEDOMETER_MAX_WIN_ID
}MMIHELLO_WINDOW_ID_E;

#undef WIN_ID_DEF

// 控件ID
typedef enum
{
    PEDOMETER_CTRL_ID_ID_START = PEDOMETER_MAX_WIN_ID,

	//ctrl id
	MMI_PEDOMETER_TITLE_BG_CTRL_ID,

	MMI_PEDOMETER_WEEK_1_CTRL_ID,
	MMI_PEDOMETER_WEEK_2_CTRL_ID,
	MMI_PEDOMETER_WEEK_3_CTRL_ID,
	MMI_PEDOMETER_WEEK_4_CTRL_ID,
	MMI_PEDOMETER_WEEK_5_CTRL_ID,
	MMI_PEDOMETER_WEEK_6_CTRL_ID,
	MMI_PEDOMETER_WEEK_7_CTRL_ID,
	MMI_PEDOMETER_WEEK_DAY_CTRL_ID,
	MMI_PEDOMETER_WEEK_STEP_CTRL_ID,

	MMI_PEDOMETER_STEP_LIST_CTRL_ID,

	MMI_PEDOMETER_LOADING_TEXT_CTRL_ID,

    PEDOMETER_MAX_CTRL_ID
}MMIHELLO_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIHELLO_ID_H_
