/***************************************************************************
** File Name:      sample_export.h                                             *
** Author:                                                                 *
** Date:           4/01/2009                                               *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2009         Xiaoqing.Lu      Create                                  *
**																		   *
****************************************************************************/
#ifndef _SPAMPLE_EXPORT_H_
#define _SPAMPLE_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#define ALARM_CLOCK_USE_FOR_TEST 0
#include "sci_types.h"
#include "mmk_type.h"
#include "dal_time.h"
#include "mmiacc_event.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

typedef enum
{
	SETTINGS_ALARM_ADD,
	SETTINGS_ALARM_EDIT
}SETTINGS_ALARM_EDIT_TYPE_E;

typedef struct
{
	uint32                      eventId;
	SETTINGS_ALARM_EDIT_TYPE_E  type;
}SETTINGS_ALARM_EDIT_WIN_T;

typedef struct
{
	uint32                      eventId;
	SETTINGS_ALARM_EDIT_TYPE_E  type;
	SCI_TIME_T                  time;
}SETTINGS_ALARM_REPEAT_WIN_T;

typedef struct
{
	MMIACC_SMART_EVENT_T		s_arrived_event;
	MMI_STRING_T				event_clock_time;
	uint16						event_id;
	uint16						sch_list_total;
}SETTINGS_ALARM_ARRIVED_WIN;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open windows
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIALARMCLOCK_CreateMainWin();

PUBLIC void MMIALARMCLOCK_CreateSelectWin(uint32 eventId);

PUBLIC void MMIALARMCLOCK_CreateAddAlarmWin();

PUBLIC void MMIALARMCLOCK_CreateEditAlarmWin(uint32 event_id);

PUBLIC void MMIALARMCLOCK_CreateAliveWin( SETTINGS_ALARM_ARRIVED_WIN arrived_event, MMI_WIN_PRIORITY_E  win_priority);

PUBLIC GUI_BG_T MMIALARMCLOCK_GETLISTBG(void);
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif//_HELLO_SPRD_H_