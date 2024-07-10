#include "ms_ref_scm_trc.h"
#include "sci_types.h"
#include "scm_api.h"
#include "scm_irp.h"
#include "scm_func.h"
#include "scm_devmgr.h"
#include "sci_api.h"
#include "tasks_id.h"

LOCAL BOOLEAN _SCM_RegSlotInformation()
{
	uint32 i;
	BOOLEAN ret;

	for(i = 0; i < g_slotInformationNum; i++)
	{
		ret = _SCM_RegSlot(&(g_slotInformation[i]));
		SCI_ASSERT(TRUE == ret);	/*assert verified*/
	}
	return TRUE;
}

#if 0
THREAD_ENTRY(SCM)
#else
void SCM_Task(uint32 argc, void*argv)
#endif
{
	uint32 evt,slotNO;
	_SCM_WAIT_TYPE waitType;

	_InitSlotLst();
	_InitDeviceMgr();
	_SCM_RegSlotInformation();

	_SCM_InReady();

	waitType = _SCM_WAIT_2SEC;
	while(1)  /*lint !e716*/
	{
		evt = _SCM_WaitEvt(waitType);

		if(0 == evt)
		{
			//SCM_TASK_PRINT:"SCM Pwr scan"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_TASK_44_112_2_18_1_52_25_20,(uint8*)"");
			waitType = _SCM_PwrScan();
		}
		if(0 != (_SCM_TIME_IND&evt))
		{
			//SCM_TASK_PRINT:"SCM some slot wake up"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_TASK_49_112_2_18_1_52_25_21,(uint8*)"");
			waitType = _SCM_WAIT_2SEC;
		}
		for(slotNO = 0; slotNO < SCM_MAX_SLOT_NO; slotNO++)
		{
			if(0 != ((_SCM_PLUG_IN|_SCM_PLUG_OUT)&GetSlotEvent(evt,slotNO)))   /*lint !e655*/
			{
				_SCM_PlugEvtProcess(slotNO,((_SCM_PLUG_IN|_SCM_PLUG_OUT)&GetSlotEvent(evt,slotNO)));   /*lint !e655*/
			}
		}
	}
}



