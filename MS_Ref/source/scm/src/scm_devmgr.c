#include "sci_types.h"
#include "scm_api.h"
#include "scm_devmgr.h"
#include "sci_api.h"


typedef struct DEVICE_CONTEXT_TAG
{
	SCI_MUTEX_PTR device_mutex;
	SCM_PWR Pwr;
	uint32 cnt;
	BOOLEAN slotNO[SCM_MAX_SLOT_NO];
}DEVICE_CONTEXT_T;

LOCAL DEVICE_CONTEXT_T s_scmDevLst[MAX_DEVICE_COUNT];

PUBLIC void _InitDeviceMgr(void)
{
	uint32 i,j;

	for(i = 0; i < MAX_DEVICE_COUNT; i++)
	{
		s_scmDevLst[i].cnt = 0;
		s_scmDevLst[i].device_mutex = NULL;
		s_scmDevLst[i].Pwr = NULL;
		for(j = 0; j < SCM_MAX_SLOT_NO; j++)
		{
			s_scmDevLst[i].slotNO[j] = FALSE;
		}
	}
}

/*
	把slot 加载到设备上
	返回设备上slot 的数目
*/
PUBLIC uint32 _AddSlotToDevice(SCM_SLOT_NAME_E slotNO,SCM_DEVICE_NAME_E DevNO,SCM_PWR pwr)
{
	char mutexName[30];

	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/
	SCI_ASSERT(DevNO < MAX_DEVICE_COUNT);	/*assert verified*/
	SCI_ASSERT(FALSE == s_scmDevLst[DevNO].slotNO[slotNO]);	/*assert verified*/

	if(0 == s_scmDevLst[DevNO].cnt)
	{
		sprintf(mutexName,"_SCM_DEV[%d]_MUTXT",DevNO);/*lint !e718 !e746*/
		s_scmDevLst[DevNO].Pwr = pwr;
		s_scmDevLst[DevNO].device_mutex = SCI_CreateMutex(mutexName, SCI_INHERIT);
	}
	else
	{
		SCI_ASSERT( pwr == s_scmDevLst[DevNO].Pwr);	/*assert verified*/
		SCI_ASSERT(NULL != s_scmDevLst[DevNO].device_mutex);	/*assert verified*/
	}
	s_scmDevLst[DevNO].slotNO[slotNO] = TRUE;
	s_scmDevLst[DevNO].cnt++;

	return s_scmDevLst[DevNO].cnt;
}
/*
	把该slot 从设备上卸载
	返回设备上剩余slot的数目

*/
PUBLIC uint32 _DeleteSlotFromDevice(SCM_SLOT_NAME_E slotNO,SCM_DEVICE_NAME_E DevNO)
{
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/
	SCI_ASSERT(DevNO < MAX_DEVICE_COUNT);	/*assert verified*/
	SCI_ASSERT(TRUE == s_scmDevLst[DevNO].slotNO[slotNO]);	/*assert verified*/
	SCI_ASSERT(0 != s_scmDevLst[DevNO].cnt);	/*assert verified*/

	s_scmDevLst[DevNO].slotNO[slotNO] = FALSE;
	s_scmDevLst[DevNO].cnt--;

	if(0 == s_scmDevLst[DevNO].cnt)
	{
		s_scmDevLst[DevNO].Pwr = NULL;
		SCI_DeleteMutex(s_scmDevLst[DevNO].device_mutex);		
	}
	else
	{
		//do nothing
	}
	return s_scmDevLst[DevNO].cnt;

}

PUBLIC BOOLEAN _DevPwr(SCM_DEVICE_NAME_E DevNO,SCM_SLOT_NAME_E slotNO,SLOT_PWR_SWITCH_E pwron)
{
#if 0
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/
	SCI_ASSERT(DevNO < MAX_DEVICE_COUNT);	/*assert verified*/
	SCI_ASSERT(TRUE == s_scmDevLst[DevNO].slotNO[slotNO]);	/*assert verified*/

	return s_scmDevLst[DevNO].Pwr(slotNO,pwron);
    #else
    return TRUE;
    #endif
}

PUBLIC void _DevEntry(SCM_DEVICE_NAME_E DevNO)
{
	if(SCI_InThreadContext())
	{
		SCI_GetMutex(s_scmDevLst[DevNO].device_mutex, SCI_WAIT_FOREVER);
	}
}

PUBLIC void _DevExit(SCM_DEVICE_NAME_E DevNO)
{
	if(SCI_InThreadContext())
	{
		SCI_PutMutex(s_scmDevLst[DevNO].device_mutex);
	}
}


