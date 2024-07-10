/*****************************************************************************
** File Name:      mmicountedtime_export.h                                               
** Author:           zhaohui    
** Date:            2007/12/14
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe gui common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/12/14       zhaohui          Create                                   *
******************************************************************************/

#ifndef _MMICOUTIME_EXPORT_H_
#define _MMICOUTIME_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiacc_position.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*
#include "sci_types.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmk_msg.h"
#include "mmidisplay_data.h"
#include "mmiacc_position.h"
*/

typedef enum
{
	APP_NOT_RUN,       /*!<倒计时未启动*/
	APP_IN_IDLE,       /*!<idle的倒计时*/
    APP_IN_MENU,	   /*!<menu的倒计时*/
} APP_POSITION_E;

typedef enum
{
	STATE_STOP,          /*!<停止状态*/
	STATE_RUNNING,       /*!<运行状态*/
    STATE_CANCEL,		 /*!<退出状态*/
	STATE_MAX            /*!<保留位*/
} COUNTEDTIME_STATE_E;
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Open Coutedtime Win in child window
//  Global resource dependence : 
//  Author:xinhe.yan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICT_CreateChildCoutimeWin(MMI_WIN_ID_T	parent_win_id);

/*****************************************************************************/
// 	Description : handle counted time timer over msg
//	Global resource dependence : none
//  Author: zhaohui
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICT_HandleTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : Open Coutedtime Win under parent window
//  Global resource dependence : 
//  Author:xinhe.yan
//  Note: 
/*****************************************************************************/
PUBLIC void _MMIAPICT_CreateChildCoutimeWin(MMI_WIN_ID_T parent_win_id,ADD_DATA add_data_ptr);


/*****************************************************************************/
// 	Description : Open Coutedtime Win
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICT_OpenCoutimeWin(void);

/*****************************************************************************/
// 	Description : Open Coutedtime Running Win
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICT_OpenCoutimeRunningWin(void);

/*****************************************************************************/
// 	Description : get Current Running Time
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC SCI_TIME_T MMIAPICT_GetCurTime(void);

/*****************************************************************************/
// 	Description : if show countedtime in idle or not
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_IsShowIdleCoutime(void);

/*****************************************************************************/
// 	Description : current Coutedtime is les than 1min
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_IsLessThanOnemin(void);

/*****************************************************************************/
// 	Description : CheckandStartDelayedEvent
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_CheckandStartDelayedCoutimeEvent(void);

/*****************************************************************************/
// 	Description : Check if counted arrive win is opened
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_IsArriveWinOpened(void);

/*****************************************************************************/
// 	Description : CheckDelayedEvent
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_CheckDelayedCoutimeEvent(void);

/*****************************************************************************/
//  Description : to handle the countedtime arrive window
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPICT_HandleCountedTimeArriveWin(
                                    MMI_WIN_ID_T        win_id,     //IN:
                                    MMI_MESSAGE_ID_E    msg_id,     //IN:
                                    DPARAM              param       //IN:
                                    );
/*****************************************************************************/
//  Description : MMIAPICT_StartCountedTimeArrive
//  Global resource dependence : 
//  Author:       zhaohui
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
PUBLIC void MMIAPICT_StartCountedTimeArrive(void);

/*****************************************************************************/
//  Description : stop the countedtimer
//  Global resource dependence : 
//  Author:junxue zhu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICT_StopCoutedTime(void);

/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				      *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMICT_HandleCountedTimeArriveWin  MMIAPICT_HandleCountedTimeArriveWin
#define MMICT_StartCountedTimeArrive       MMIAPICT_StartCountedTimeArrive

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

#ifdef   __cplusplus
    }
#endif

#endif  

