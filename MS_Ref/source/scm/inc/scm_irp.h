#ifndef _SCM_IRP_H_
#define _SCM_IRP_H_

#include "sci_types.h"
#include "scm_api.h"


//-------- For debug ----------------
#define SCM_IRP_DEBUG
#define SCM_FUNC_DEBUG
#define SCM_TASK_DEBUG

#ifdef SCM_IRP_DEBUG
#define SCM_IRP_PRINT(x) SCI_TRACE_LOW x
#else
#define SCM_IRP_PRINT(x)
#endif

#ifdef SCM_FUNC_DEBUG
#define SCM_FUNC_PRINT(x) SCI_TRACE_LOW x
#else
#define SCM_FUNC_PRINT(x)
#endif

#ifdef SCM_TASK_DEBUG
#define SCM_TASK_PRINT(x) SCI_TRACE_LOW x
#else
#define SCM_TASK_PRINT(x)
#endif

#if defined(OS_SELF_SIMULATOR)
#define SCM_IRQ_DECLARE OS_CPU_SR  cpu_sr
#define SCM_DISABLE_IRQ OS_ENTER_CRITICAL
#define SCM_ENABLE_IRQ OS_EXIT_CRITICAL
#else
#define SCM_IRQ_DECLARE
#define SCM_DISABLE_IRQ SCI_DisableIRQ
#define SCM_ENABLE_IRQ SCI_RestoreIRQ
#endif

extern uint32 SCM_TASK_STACK_SIZE;
extern uint32 SCM_TASK_TASK_PRIO;
extern SCM_SLOT_DESCRIPTION_T g_slotInformation[];
extern uint32 g_slotInformationNum;
//----------Module Function----------------------------
/*
	初始化SCM任务调度模块
*/
PUBLIC BOOLEAN _SCM_ScheInit(void);
/*
------------------------------------------------
	0	1	2	3	4	5	......	30	31
	s0I	s0O	s1I	s1O	s2I	s2O	......	tm	rd
------------------------------------------------
*/
/*
	当SCM模块初始化完毕时调用该函数
*/
PUBLIC void _SCM_InReady(void);

/*
	调用该函数等待SCM初始化完毕	
*/
PUBLIC void _SCM_WaitReady(void);

/*
	等待外部事件
*/
#define GetSlotEvent(evt,slotNO)	(0x3&(evt>>(slotNO<<1)))

typedef enum
{
	_SCM_WAIT_500MS,
	_SCM_WAIT_2SEC,
	_SCM_WAITFOREVER
}
_SCM_WAIT_TYPE;
PUBLIC uint32 _SCM_WaitEvt(_SCM_WAIT_TYPE waitType);

/*
	外部slot事件产生时调用该函数通知SCM模块
*/
typedef enum
{
	_SCM_PLUG_IN	= (1<<0),
	_SCM_PLUG_OUT	= (1<<1),
	_SCM_TIME_IND	= (1<<30)
}
_SCM_SLOT_EVENT;
PUBLIC void _SCM_SetSlotEvt(SCM_SLOT_NAME_E slotNO,_SCM_SLOT_EVENT evt);

PUBLIC void _SCM_SetTimeEvt(void);

#endif

