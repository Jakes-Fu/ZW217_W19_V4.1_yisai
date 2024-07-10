/******************************************************************************
 ** File Name:      task_monitor.h                                            *
 ** Author:         Richard Yang                                              *
 ** DATE:           03/04/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for deep sleep.                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/04/2004     Richard.Yang     Create.                                   *
 ******************************************************************************/
#ifndef _TASK_MONITOR_H
#define _TASK_MONITOR_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
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
#define TM_IDLE_REQ            0x10
#define TM_TASK_REQ            0x20
#define TM_ENTER_INT_REQ       0x30
#define TM_LEAVE_INT_REQ       0x40
#define TM_ENTER_SLEEP_REQ     0x50
#define TM_LEAVE_SLEEP_REQ     0x60
#define TM_TEST_POINT_REQ      0x70
#define TM_ENTER_FIQ       	   0x80
#define TM_LEAVE_FIQ       	   0x90

void TM_SendInitRequest(void);
void TM_SendIdleRequest(void);
void * TM_SendTaskRequest(void *thread_ptr);
void TM_SendIrqRequest(uint8 id, uint32 irq_status);
void TM_SendSleepRequest(uint8 id);
void TM_SendTestPointRequest(uint32 param1, uint32 param2);
void DumpSwitchMemory(void);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif //

#endif // End of _TASK_MONITOR_H

