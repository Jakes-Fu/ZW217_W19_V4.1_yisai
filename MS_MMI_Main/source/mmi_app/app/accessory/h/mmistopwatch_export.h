/*****************************************************************************
** File Name:      mmistopwatch_export.h                                               
** Author:         yaye.jiang    
** Date:           2011/7/1
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe stopwatch export interface   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/12/14       zhaohui          Create                                   *
******************************************************************************/

#ifndef _MMISTOPWATCH_EXPORT_H_
#define _MMISTOPWATCH_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmk_msg.h"
#include "mmidisplay_data.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define ST_COUNT_RESTORE_NUM 20//最多在list中记录20条计时信息
//#define ST_COUNT_DISP_NUM   4//开始最多显示数量每页

#if 0 //nv去掉了，此处不需要了
typedef struct MMINV_ST_TIME_CURREN_T 
{
    uint64  start_time;
    uint64  count_time;
    uint64  stop_time1;
    uint64  stop_time2;
    uint64  current_time1;
    uint64  current_time2;
} MMINV_ST_TIME_CURREN_T;

typedef struct MMINV_ST_TIME_COUNT_T 
{
    uint32  count_no;
    uint64  count_time;
} MMINV_ST_TIME_COUNT_T;
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIST_APIST_HandleTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : Open StopWatch Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIST_OpenStopWatchWin(void);

#if 0//nv去掉，不需要初始化了
/*****************************************************************************/
// 	Description : init stopwatch
//	Global resource dependence : 
//  Author:      yaye.jiang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISTOPWATCH_Init(void);//该函数在开机时调用
#endif

/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				      *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define  MMIST_APIST_HandleTimer  MMIAPIST_APIST_HandleTimer

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

#ifdef   __cplusplus
    }
#endif

#endif  

