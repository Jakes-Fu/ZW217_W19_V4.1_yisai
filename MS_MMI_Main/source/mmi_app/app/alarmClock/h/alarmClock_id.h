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
    ALARMCLOCK_WIN_ID_START = (MMI_MODULE_ALARMCLOCK << 16),

	MMI_ALARMCLOCK_MAIN_WIN_ID,//闹钟主界面

	MMI_ALARMCLOCK_ALARM_MAX_WIN_ID,//闹钟数已满

	MMI_ALARMCLOCK_SET_ALARM_WIN_ID,//设置闹钟状态

	MMI_ALARMCLOCK_SET_TIME_WIN_ID,//设置时间界面

	MMI_ALARMCLOCK_SET_DATE_WIN_ID,//设置日期界面

	MMI_ALARMCLOCK_TIME_REMAIN_ID,//剩余时间界面

	MMI_ALARMCLOCK_ALARM_CLOCK_WIN_ID,//闹钟响起界面

    ALARMCLOCK_MAX_WIN_ID
}MMIHELLO_WINDOW_ID_E;

#undef WIN_ID_DEF

// 控件ID
typedef enum
{
    PEDOMETER_CTRL_ID_ID_START = ALARMCLOCK_MAX_WIN_ID,

	//ctrl id
	MMI_ALARMCLOCK_SETLIST_CTRL_ID,//闹钟列表

	MMI_ALARMCLOCK_ADD_CLOCK_CTRL_ID,//添加闹钟按钮

	MMI_ALARMCLOCK_SAVE_BUTTON_CTRL_ID,//保存按钮

	MMI_ALARMCLOCK_NEXT_PAGE_CTRL_ID,//下一页按钮

	MMI_ALARMCLOCK_SELECT_LIST_CTR_ID,//设置、打开/关闭、删除

	MMI_ALARMCLOCK_SELECT_TIME_CTRL_ID,//设置时间

	MMI_ALARMCLOCK_SELECT_WEEK_CTRL_ID,//设置日期

	MMI_ALARMCLOCK_EXTEND_CTRL_ID,//延长按钮

	MMI_ALARMCLOCK_CLOSE_CTRL_ID,//关闭按钮

	MMI_ALARMCLOCK_CLOSE2_CTRL_ID,//关闭按钮

    ALARMCLOCK_MAX_CTRL_ID
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
