/****************************************************************************
** File Name:      mmi_event_api.c                                              *
** Author:         gang.tong                                              *
** Date:           03/11/2009                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the event interface.  *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2009        gang.tong        Create
**
****************************************************************************/

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "os_api.h"
#include "mmi_app_kernel_trc.h"
//#ifdef WIN32
//#include "std_header.h"
//#endif

#include "mmi_event_manager.h"
#include "mmi_event_api.h"

/**---------------------------------------------------------------------------*
**                         Macro Declaration                                 *
**---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 设置U盘挂起的listener
//  Global resource dependence :
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_EventCallBack(
								   uint32 event_type,
								   void * call_back,
								   uint32 arvc,
								   uint32 argv[]
								   )
{
	switch(event_type)
	{
	case UDISK_MOUNT_EVENT:
		if (arvc >= 2)
		{
			((UDISKMOUNTCALLBACK)call_back)((MMIEVENT_UDISK_DEV_E)argv[0], (BOOLEAN)(argv[1])); //lint !e611
		}

		break;
    case TIME_UPDATE_EVENT:
		if (arvc >= 2)
		{
			((TIMEUPDATECALLBACK)call_back)(argv[0], argv[1]);  //lint !e611
		}

		break;
    case PHONEBOOK_EVENT:
		if (arvc >= 1)
		{
			if(argv != PNULL && call_back != PNULL)
			{
				((PHONEBOOKCALLBACK)call_back)(argv[0]);  //lint !e611
			}
		}
		break;
#ifdef MMI_REFACTOR_SAMPLE
	case APPSAMPLE_EVENT:
		if (arvc >= 3)
		{
			if(argv != PNULL && call_back != PNULL)
			{
				((SAMPLECALLBACK)call_back)(argv[0],(argv[1]),(argv[2]));
			}
		}
        break;
#endif
	default:
		if (arvc >= 1)
		{
			if(argv != PNULL && call_back != PNULL)
			{
				((EVENTCALLBACK)call_back)((MMIEVENT_INFO_T*)argv[0]);  //lint !e611
			}
		}
		break;
	}

	return TRUE;
}

/*****************************************************************************/
//  Description : 设置U盘挂起的listener
//  Global resource dependence :
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_SetUdiskMountListener(
										   UDISKMOUNTCALLBACK call_back,
										   BOOLEAN is_reg
										   )
{
	BOOLEAN bret = FALSE;

	if (is_reg)
	{
		bret = MMIFW_RegisterEvent(UDISK_MOUNT_EVENT, (void*)call_back); //lint !e611
	}
	else
	{
		bret = MMIFW_UnregisterEvent(UDISK_MOUNT_EVENT, (void*)call_back); //lint !e611
	}

	return bret;
}

/*****************************************************************************/
//  Description : 触发U盘挂起的事件
//  Global resource dependence :
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_PumpUdiskMountEvent(
										 MMIEVENT_UDISK_DEV_E device,
										 BOOLEAN mount
										 )
{
	uint32 argv[2] = {0};
	argv[0] = device;
	argv[1] = mount;

	return MMIFW_PumpEvent(UDISK_MOUNT_EVENT, argv, sizeof(argv)/sizeof(uint32));
}

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
											)
{
	BOOLEAN bret = FALSE;

	if (is_reg)
	{
		bret = MMIFW_RegisterEvent(TIME_UPDATE_EVENT, (void*)call_back); //lint !e611
	}
	else
	{
		bret = MMIFW_UnregisterEvent(TIME_UPDATE_EVENT, (void*)call_back); //lint !e611
	}

	return bret;
}

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
										  )
{
	uint32 argv[2] = {0};
	argv[0] = old_time;
	argv[1] = new_time;

	return MMIFW_PumpEvent(TIME_UPDATE_EVENT, argv, sizeof(argv)/sizeof(uint32));
}


/*******************************************************************/
//  Interface:		MMIFW_TriggerEvent
//  Description :
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIFW_TriggerEvent(
								  MMIEVENT_INFO_T *event_ptr
								  )
{
    BOOLEAN result = FALSE;
	uint32 argv[1] = {0};


	if (PNULL == event_ptr)
	{
		SCI_TRACE_LOW("MMIFW_TriggerEvent() event_ptr == PNULL!");
		return FALSE;
	}
	SCI_TRACE_LOW("MMIFW_TriggerEvent() event_type = %d", event_ptr->event_type);
	argv[0] = (uint32)event_ptr;

	result = MMIFW_PumpEvent(event_ptr->event_type, argv, sizeof(argv)/sizeof(uint32));
	return result;
}

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
									  )
{
	BOOLEAN bret = FALSE;

	if (is_reg)
	{
		bret = MMIFW_RegisterEvent(event_type, (void*)call_back); //lint !e611
	}
	else
	{
		bret = MMIFW_UnregisterEvent(event_type, (void*)call_back); //lint !e611
	}

	return bret;
}


/**-------------------------PHONEBOOK Event API start -----------------------*/
/*******************************************************************/
//  Interface:		register pb event
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_PBRegisterEvent(
								   PHONEBOOKCALLBACK call_back//IN
									)
{
	return MMIFW_RegisterEvent(PHONEBOOK_EVENT, (void*)call_back); //lint !e611
}

/*******************************************************************/
//  Interface:		unregister pb event
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_PBUnregisterEvent(
									 PHONEBOOKCALLBACK call_back
									 )
{
	return MMIFW_UnregisterEvent(PHONEBOOK_EVENT, (void*)call_back); //lint !e611
}

/*******************************************************************/
//  Interface:
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_PBTriggerEvent(MMIEVENT_PB_EVENT_E event)
{
	uint32 argv[1] = {0};

	argv[0] = event;

	return MMIFW_PumpEvent(PHONEBOOK_EVENT, argv, sizeof(argv)/sizeof(uint32));
}
/**-------------------------PHONEBOOK Event API end -----------------------*/

#ifdef MMI_REFACTOR_SAMPLE
/*******************************************************************/
//  Interface:		register sample event
//  Description :
//  Global resource dependence :
//  Author: sam.hua
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_SampleRegisterEvent(
								   SAMPLECALLBACK call_back//IN
									)
{
	return MMIFW_RegisterEvent(APPSAMPLE_EVENT, (void*)call_back); //lint !e611
}
/*******************************************************************/
//  Interface:		unregister sample event
//  Description :
//  Global resource dependence :
//  Author: sam.hua
//  Note:
/*******************************************************************/

PUBLIC BOOLEAN MMK_SampleUnregisterEvent(
									 SAMPLECALLBACK call_back
									 )
{
	return MMIFW_UnregisterEvent(APPSAMPLE_EVENT, (void*)call_back); //lint !e611
}

/*******************************************************************/
//  Interface: Invoke Event
//  Description :
//  Global resource dependence :
//  Author: sam.hua
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_SampleTriggerEvent(MMISAMPLE_APP_EVENT_E envent,uint32 param_ptr,uint32 param_size)
{
	uint32 argv[3] = {0};

	argv[0] = envent;
	argv[1] = param_ptr;
    argv[2] = param_size;

	return MMIFW_PumpEvent(APPSAMPLE_EVENT, argv, sizeof(argv)/sizeof(uint32));
}
#endif
//////////////////////////////////////////////////////////////////////////
// test code

#if 0
#include "os_api.h"
LOCAL void updatetime1(uint32 old_time, uint32 new_time)
{
	//SCI_TRACE_LOW:"updatetime 1.\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_EVENT_API_193_112_2_18_2_0_8_211,(uint8*)"");
}

LOCAL void updatetime2(uint32 old_time, uint32 new_time)
{
	//SCI_TRACE_LOW:"updatetime 2.\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_EVENT_API_198_112_2_18_2_0_8_212,(uint8*)"");
}

LOCAL void updatetime3(uint32 old_time, uint32 new_time)
{
	//SCI_TRACE_LOW:"updatetime 3.\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_EVENT_API_203_112_2_18_2_0_8_213,(uint8*)"");
}

LOCAL void udiskmount1(MMIEVENT_UDISK_DEV_E device, BOOLEAN mount)
{
	//SCI_TRACE_LOW:"udiskmount 1.\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_EVENT_API_208_112_2_18_2_0_8_214,(uint8*)"");
}

LOCAL void udiskmount2(MMIEVENT_UDISK_DEV_E device, BOOLEAN mount)
{
	//SCI_TRACE_LOW:"udiskmount 2.\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_EVENT_API_213_112_2_18_2_0_8_215,(uint8*)"");
}

PUBLIC void testMmiEvent()
{
	MMIFW_SetTimeUpdateListerner(updatetime1, TRUE);
	MMIFW_SetTimeUpdateListerner(updatetime2, TRUE);
	MMIFW_SetTimeUpdateListerner(updatetime1, FALSE);
	MMIFW_SetTimeUpdateListerner(updatetime2, FALSE);

	MMIFW_SetUdiskMountListener(udiskmount1, TRUE);
	MMIFW_SetUdiskMountListener(udiskmount1, FALSE);

	MMIFW_SetTimeUpdateListerner(updatetime1, TRUE);
	MMIFW_SetTimeUpdateListerner(updatetime2, TRUE);
	MMIFW_SetTimeUpdateListerner(updatetime3, TRUE);

	MMIFW_SetUdiskMountListener(udiskmount1, TRUE);
	MMIFW_SetUdiskMountListener(udiskmount2, TRUE);
}
#endif


