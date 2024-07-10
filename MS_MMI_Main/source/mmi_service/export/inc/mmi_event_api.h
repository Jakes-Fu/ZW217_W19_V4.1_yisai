/*****************************************************************************
** File Name:      mmi_event_api.h                                           *
** Author:                                                                   *
** Date:           10/2009                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMI_EVENT_API_H_
#define _MMI_EVENT_API_H_

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
	EVENT_NONE = 0,
 
	//special event
	UDISK_MOUNT_EVENT,
	TIME_UPDATE_EVENT,
	PHONEBOOK_EVENT,
	APPSAMPLE_EVENT,	//other event
	SMS_NEWSMS_EVENT,
#ifdef DROPDOWN_NOTIFY_SHORTCUT
	SMS_READSMS_EVENT,  //短信APP读短信的event
	SMS_DELETESMS_EVENT, //delete sms --notification
#endif
	SMS_DATA_CHANGE_EVENT,
    CL_UPDATE_EVENT,
	CC_MISSED_CALL_EVENT,
	EVENT_MAX
}MMIEVENT_TYPE_E;

typedef enum
{
	DEV_NONE = 0,
	DEV_UDISK,
	DEV_SDCARD1,
	DEV_SDCARD2,
	DEV_MAX
}MMIEVENT_UDISK_DEV_E;

typedef enum
{
	MMIEVENT_PB_ADD = 0,//添加记录
	MMIEVENT_PB_DELETE, //删除记录
	MMIEVENT_PB_UPDATE, //更新记录
	MMIEVENT_PB_ENVENT_NUM //无效
}MMIEVENT_PB_EVENT_E;

typedef enum
{
	MMIEVENT_CL_MISSED_CALL = 0,//添加记录
	MMIEVENT_CL_EVENT_NUM //无效
}MMIEVENT_CL_EVENT_E;
#ifdef MMI_REFACTOR_SAMPLE
typedef enum
{
    MMISAMPLE_APP_INIT_OK = 0,
	MMISAMPLE_APP_UPDATE = 1, 
	MMISAMPLE_APP_READ_COMPLETED = 2,
	MMISAMPLE_APP_CHANGE_COMPLETED, 
	MMISAMPLE_APP_EVENT_MAX
}MMISAMPLE_APP_EVENT_E;
#endif

typedef struct
{
	MMIEVENT_TYPE_E event_type;
	void *param_ptr;
}MMIEVENT_INFO_T;

typedef void (*UDISKMOUNTCALLBACK)(MMIEVENT_UDISK_DEV_E device,BOOLEAN mount);
typedef void (*TIMEUPDATECALLBACK)(uint32 old_time, uint32 new_time);
typedef void (*PHONEBOOKCALLBACK)(MMIEVENT_PB_EVENT_E envent);
typedef void (*EVENTCALLBACK)(MMIEVENT_INFO_T *create_ptr);

#ifdef MMI_REFACTOR_SAMPLE
typedef void (*SAMPLECALLBACK)(MMISAMPLE_APP_EVENT_E envent,uint32 param_ptr,uint32 size);
#endif
/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

/**********************************以下是special event的对外接口*********************************/

/*****************************************************************************/
//  Description : 设置U盘挂起的listener
//  Global resource dependence : 
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_SetUdiskMountListener(
										   UDISKMOUNTCALLBACK call_back,
										   BOOLEAN  is_reg
										   );
/*****************************************************************************/
//  Description : 触发U盘挂起的事件
//  Global resource dependence : 
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_PumpUdiskMountEvent(
										 MMIEVENT_UDISK_DEV_E device,
										 BOOLEAN mount
										 );

/*******************************************************************/
//  Interface:		MMIFW_SetTimeUpdateListerner
//  Description : 	MMIFW_SetTimeUpdateListerner
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIFW_SetTimeUpdateListerner(
											TIMEUPDATECALLBACK call_back,
											BOOLEAN is_reg
											);

/*******************************************************************/
//  Interface:		MMIFW_PumpTimerMountEvent
//  Description : 	MMIFW_PumpTimerMountEvent
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIFW_PumpTimerUpdateEvent(
										  uint32 old_time,
										  uint32 new_time
										  );
	
/*******************************************************************/
//  Interface:		register pb event
//  Description : 	
//  Global resource dependence : 
//  Author: 
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_PBRegisterEvent(
								   PHONEBOOKCALLBACK call_back//IN									
									);

/*******************************************************************/
//  Interface:		unregister pb event
//  Description : 	
//  Global resource dependence : 
//  Author: 
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_PBUnregisterEvent(
									 PHONEBOOKCALLBACK call_back
									 );

/*******************************************************************/
//  Interface:	trigger pb event	
//  Description : 	
//  Global resource dependence : 
//  Author: 
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_PBTriggerEvent(MMIEVENT_PB_EVENT_E event);

#ifdef MMI_REFACTOR_SAMPLE
/**-------------------------SAMPLE Event API start -----------------------*/
/*******************************************************************/
//  Interface:		register sample event
//  Description : 	
//  Global resource dependence : 
//  Author: sam.hua
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_SampleRegisterEvent(SAMPLECALLBACK call_back);

/*******************************************************************/
//  Interface:		unregister sample event
//  Description : 	
//  Global resource dependence : 
//  Author: sam.hua
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_SampleUnregisterEvent(SAMPLECALLBACK call_back);

/*******************************************************************/
//  Interface:	Invoke Event	
//  Description : 	
//  Global resource dependence : 
//  Author: sample
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_SampleTriggerEvent(MMISAMPLE_APP_EVENT_E envent,uint32 param_ptr,uint32 param_size);
#endif

/**********************************以下是other event的对外接口*********************************/

/*******************************************************************/
//  Interface:		MMIFW_TriggerEvent
//  Description : 	
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIFW_TriggerEvent(
								  MMIEVENT_INFO_T *event_ptr
								  );

/*******************************************************************/
//  Interface:		MMIFW_SetEventListener
//  Description : 	
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIFW_SetEventListener(
									  MMIEVENT_TYPE_E event_type,
									  EVENTCALLBACK call_back,
									  BOOLEAN is_reg
									  );

PUBLIC void testMmiEvent();

#ifdef __cplusplus
}
#endif

#endif // _MMI_EVENT_API_H_


