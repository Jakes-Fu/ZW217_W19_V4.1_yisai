/*****************************************************************************
** File Name:      watch_alarm_edit.h      //alarm settings                  *
** Author:         qi.liu1                                                   *
** Date:           26/2/2021                                                 *
** Copyright:      All Rights Reserved.                                      *
** Description:  alarm edit window                                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2021      qi.liu1              Creat                                   *
******************************************************************************/

#ifndef _WATCH_ALARM_EDIT_H_
#define _WATCH_ALARM_EDIT_H_

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#ifdef ADULT_WATCH_SUPPORT
#define ALARM_TIME_EDIT_RECT_LEFT              DP2PX_VALUE(0)
#define ALARM_TIME_EDIT_RECT_RIGHT             DP2PX_VALUE(48)
#define WATCHCOM_CHECKLIST_CREATE              AdultWatchWatchCOM_CheckList_Left_Create
#else
#define ALARM_TIME_EDIT_RECT_LEFT              DP2PX_VALUE(60)
#define ALARM_TIME_EDIT_RECT_RIGHT             DP2PX_VALUE(180)
#define WATCHCOM_CHECKLIST_CREATE              WatchCOM_CheckList_Create
#endif

typedef enum
{
    WATCH_ALARM_ADD,
    WATCH_ALARM_EDIT
}
ALARM_EDIT_TYPE_E;

typedef struct
{
    uint32                      eventId;
    ALARM_EDIT_TYPE_E  type;
}
ALARM_EDIT_WIN_T;

typedef struct
{
    uint32                      eventId;
    ALARM_EDIT_TYPE_E  type;
    SCI_TIME_T                  time;
}
ALARM_REPEAT_WIN_T; //SETTINGS_ALARM_REPEAT_WIN_T

/**--------------------------------------------------------------------------*
 **                         PUBLIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : WatchAlarm_AddWin_Enter
//  Parameter: [In] event_id
//             [Out] None
//  Author: qi.liu1
//  Note:alarm add win的入口函数
/*****************************************************************************/
PUBLIC void WatchAlarm_AddWin_Enter( void );

/*****************************************************************************/
//  Description : WatchAlarm_RepeatWin_Enter
//  Parameter: [In] win_data_ptr
//             [Out] None
//  Author: qi.liu1
//  Note:alarm repeat win的入口函数
/*****************************************************************************/
PUBLIC void WatchAlarm_RepeatWin_Enter( ALARM_REPEAT_WIN_T* win_data_ptr );

/*****************************************************************************/
//  Description : WatchAlarm_EditWin_Enter
//  Parameter: [In] event_id
//             [Out] None
//  Author: qi.liu1
//  Note:alarm edit win的入口函数
/*****************************************************************************/
PUBLIC void WatchAlarm_EditWin_Enter( uint32 event_id );
#endif
