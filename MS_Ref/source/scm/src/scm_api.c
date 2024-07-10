#include "sci_types.h"
#include "scm_irp.h"
#include "scm_func.h"
#include "sci_api.h"
#include "scm_api.h"

PUBLIC BOOLEAN SCM_Init(void)
{
	BOOLEAN result;

	result = _SCM_ScheInit();
	SCI_ASSERT(TRUE == result);	/*assert verified*/
	return result;
}

PUBLIC BOOLEAN SCM_RegEvent(SCM_SLOT_NAME_E slot_name,BLOCK_ID clientId,SCM_EVENT_E scmEvent,SCM_EVENT_CALLBACK callback)
{
	return _SCM_RegEvent(slot_name, clientId, scmEvent, callback);
}

PUBLIC void SCM_SetService(SCM_SLOT_NAME_E slot_name, SCM_SERVICE_ID serviceId, SCM_SERVICE_ONOFF onOff)
{
	_SCM_SetService(slot_name, serviceId, onOff);
}

PUBLIC SCM_STATUS SCM_GetSlotStatus(SCM_SLOT_NAME_E slot_name)
{
	return _SCM_GetSlotStatus(slot_name);
}

PUBLIC void SCM_PlugIn(SCM_SLOT_NAME_E slot_name)
{
	_SCM_SetSlotEvt(slot_name, _SCM_PLUG_IN);
}

PUBLIC void SCM_PlugOut(SCM_SLOT_NAME_E slot_name)
{
	_SCM_SetSlotEvt(slot_name, _SCM_PLUG_OUT);
}

PUBLIC BOOLEAN SCM_IOCTL(SCM_SLOT_NAME_E slot_name,SCM_FUN_E cmd,SCM_PARAM_T* param)
{
	return _SCM_IOCTL(slot_name,cmd, param);
}

