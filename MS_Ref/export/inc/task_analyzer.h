/******************************************************************************
 ** File Name:      task_monitor.h                                            *
 ** Author:         Johnson.sun                                              *
 ** DATE:           02/18/2008                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for deep sleep.                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/18/2008     Johnson.sun      Create.                                   *
 ******************************************************************************/
#ifndef _TASK_ANALYSE_H
#define _TASK_ANALYSE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#ifndef WIN32
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

typedef enum TASK_TYPE_tag
{
   STATIC_TASK = 0,
   IDLE,
   IRQ,
   FIQ,
   DYNAMIC_TASK,
   USER_MARK   
}TASK_TYPE_E;


void TA_InitTaskAnalyse(void);
void TA_ResetTaskAnalyseRequest(void);
void TA_RegisterTaskRequest(TASK_TYPE_E task_type, const char *thread_name, uint32 id);
void TA_UnRegisterTaskRequest(TASK_TYPE_E task_type, uint32 id);
void TA_EnterIdleRequest(void);
void *TA_EnerTaskRequest(void *thread_ptr);
void TA_EnterIrqRequest(uint32 irq_num);
void TA_EnterFiqRequest(uint32 fiq_num);
uint32 TA_RegisterMarkNode(uint32 param1, uint32 param2);
void TA_UnRegisterMarkNode(uint32 mark_id);
void TA_EnterMarkRequest(uint32 mark_id);
uint32 TA_LeaveMarkRequest(uint32 mark_id);


#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif //

#endif // End of _TASK_MONITOR_H

