#include "sci_types.h"
#include "sci_api.h"
#include "tasks_id.h"
#include "scm_irp.h"
#include "priority_system.h"
extern void SCM_Task(uint32 argc, void*argv);

#define SCM_LOCAL_EVENT
#ifdef SCM_LOCAL_EVENT
LOCAL SCI_EVENT_GROUP_PTR s_scmIRP = PNULL;
#endif

LOCAL BLOCK_ID s_scm_taskid = SCI_INVALID_BLOCK_ID;

PUBLIC BOOLEAN _SCM_ScheInit(void)
{
#ifdef SCM_LOCAL_EVENT
	s_scmIRP = SCI_CreateEvent(  "SCM_IRP" );
	if(SCI_NULL == s_scmIRP)
	{
		SCI_ASSERT(0);	/*assert verified*/
		return FALSE;     /*lint !e527*/
	}
	SCI_SetEvent(s_scmIRP,0,SCI_AND);
#endif
	s_scm_taskid = SCI_CreateThread(	// If successful, returns the thread ID
							// of the new created thread, 
							// else returns SCI_INVALID_BLOCK_ID
		"SCM_TASK",				// Name string of the thread
		"SCM_QUEUE",				// Queue name string of the thread
		SCM_Task,				// Entry function of the thread
		0,							// First parameter for entry function,
		0,							// Second parameter for entry function,
		SCM_TASK_STACK_SIZE,		// Size of the thread stack in bytes
		1,							// Number of messages which can be enqueued
		PRI_SCM,		// Prority of the thread.
		SCI_PREEMPT,				// Indicates if the thread is preemptable.
		SCI_AUTO_START			// Specifies whether the thread starts 
								// immediately or stays in a pure suspended
								// state. Legal options are SCI_AUTO_START
								// and SCI_DONT_START.
	);
	if(SCI_INVALID_BLOCK_ID == s_scm_taskid)
	{
		SCI_ASSERT(0);	/*assert verified*/
		return FALSE;     /*lint !e527*/
	}
	return TRUE;
}



/*
------------------------------------------------
	0	1	2	3	4	5	......	30	31
	s0I	s0O	s1I	s1O	s2I	s2O	......	tm	rd
------------------------------------------------
*/
PUBLIC void _SCM_InReady(void)
{
#ifdef SCM_LOCAL_EVENT
	if(SCI_SUCCESS != SCI_SetEvent(s_scmIRP,(((uint32)1)<<31),SCI_OR))
	{
		SCI_ASSERT(0);	/*assert verified*/
	}
#else
	if(SCI_SUCCESS != SCI_SetThreadEvent(s_scm_taskid, (((uint32)1)<<31), SCI_OR))
	{
		SCI_ASSERT(0);	/*assert verified*/
	}
#endif
}

PUBLIC void _SCM_WaitReady(void)
{
	uint32 actual_flags_ptr;
#ifdef SCM_LOCAL_EVENT
	if(SCI_SUCCESS == SCI_GetEvent(s_scmIRP,(((uint32)1)<<31), SCI_OR, &actual_flags_ptr, SCI_WAIT_FOREVER))
#else
	if(SCI_SUCCESS == SCI_GetThreadEvent(s_scm_taskid, (((uint32)1)<<31), SCI_OR, &actual_flags_ptr, SCI_WAIT_FOREVER))
#endif
	{
		return;
	}
	else
	{
		SCI_ASSERT(0);	/*assert verified*/
		return;     /*lint !e527*/
	}
}

PUBLIC uint32 _SCM_WaitEvt(_SCM_WAIT_TYPE waitType)
{
	uint32 actual_flags_ptr = 0;
	uint32 waitOption = 0;

	if(_SCM_WAIT_2SEC == waitType)
	{
		waitOption = 2000;
	}
	else if(_SCM_WAITFOREVER == waitType)
	{
		waitOption = SCI_WAIT_FOREVER;
	}
	else
	{
		SCI_ASSERT(0);	/*assert verified*/
	}
#ifdef SCM_LOCAL_EVENT
	if(SCI_SUCCESS == SCI_GetEvent(s_scmIRP,0x7FFFFFFF, SCI_OR_CLEAR, &actual_flags_ptr, waitOption))
#else
	if(SCI_SUCCESS == SCI_GetThreadEvent(s_scm_taskid, 0x7FFFFFFF, SCI_OR_CLEAR, &actual_flags_ptr, waitOption))
#endif
	{
		return (0x7FFFFFFF&actual_flags_ptr);
	}
	else
	{
		return 0;
	}

}

PUBLIC void _SCM_SetSlotEvt(SCM_SLOT_NAME_E slotNO,_SCM_SLOT_EVENT evt)
{
	uint32 evtBit;

	switch(evt)
	{
		case _SCM_PLUG_IN:
		case _SCM_PLUG_OUT:
		{
			evtBit = (evt << (slotNO<<1));
		}
		break;

		default:
		{
			evtBit = 0;
			SCI_ASSERT(0);	/*assert verified*/
		}
		break;
	}
#ifdef SCM_LOCAL_EVENT
	if(SCI_SUCCESS != SCI_SetEvent(s_scmIRP,evtBit,SCI_OR))
	{
		SCI_ASSERT(0);	/*assert verified*/
		return;		/*lint !e527*/
	}
#else
	if(SCI_SUCCESS != SCI_SetThreadEvent(s_scm_taskid, evtBit, SCI_OR))
	{
		SCI_ASSERT(0);	/*assert verified*/
		return;		/*lint !e527*/
	}
#endif
}

PUBLIC void _SCM_SetTimeEvt(void)
{
#ifdef SCM_LOCAL_EVENT
	if(SCI_SUCCESS != SCI_SetEvent(s_scmIRP,_SCM_TIME_IND,SCI_OR))
	{
		SCI_ASSERT(0);	/*assert verified*/
		return;			/*lint !e527*/
	}
#else
	if(SCI_SUCCESS != SCI_SetThreadEvent(s_scm_taskid, _SCM_TIME_IND, SCI_OR))
	{
		SCI_ASSERT(0);	/*assert verified*/
		return;			/*lint !e527*/
	}
#endif
}




