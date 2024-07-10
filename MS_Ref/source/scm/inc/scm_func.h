#ifndef _SCM_FUNC_H_
#define _SCM_FUNC_H_
#include "sci_types.h"

PUBLIC void _InitSlotLst(void);


/*******************************************************************
	used in scm task.
*******************************************************************/

/*
	该函数被调用时必须和其它功能函数互斥，该函数目前在任务启动时被任务调用
	但目前该函数只在系统启动时被调用用于配置slot端口信息
*/
PUBLIC BOOLEAN _SCM_RegSlot(SCM_SLOT_DESCRIPTION_T* description);


/*
	delte slot from scm manager
*/
PUBLIC BOOLEAN _SCM_UnRegSlot(SCM_SLOT_NAME_E slotNO);


/*******************************************************************
	used in app task.
*******************************************************************/
/*
	scm has 3 service background.
	1 power san and power saving
	2 error process
	3 event indicate
	this api can start or stop service.

	param:
		slotNO		: slot number where you want to set
		serviceId	: service index
		onOf		: start or stop
*/
PUBLIC void _SCM_SetService(SCM_SLOT_NAME_E slotNO, SCM_SERVICE_ID serviceId, SCM_SERVICE_ONOFF onOff);

/*
	slotNO: slot number where you want to recieve msg from
	clientId: the taskId that want to recieve msg
	scmEvent: msg
	callback: when msg happend ,this callback will be called
*/
PUBLIC BOOLEAN _SCM_RegEvent(SCM_SLOT_NAME_E slotNO,BLOCK_ID clientId,SCM_EVENT_E scmEvent,SCM_EVENT_CALLBACK callback);

/*
	get status of the card that in slot
*/
PUBLIC SCM_STATUS _SCM_GetSlotStatus(SCM_SLOT_NAME_E slotNO);

/*
	used this function to access slot data
*/
PUBLIC BOOLEAN _SCM_IOCTL(SCM_SLOT_NAME_E slotNO,SCM_FUN_E cmd,SCM_PARAM_T* param);


/*******************************************************************
	used in scm_task.
*******************************************************************/

/*
	if plug event happened , use this function to process it
*/
PUBLIC _SCM_WAIT_TYPE _SCM_PlugEvtProcess(SCM_SLOT_NAME_E slotNO, uint32 evt);

/*
	when tick time arrised ,use this function to power down the idle slot
*/
PUBLIC _SCM_WAIT_TYPE _SCM_PwrScan(void);

#endif



