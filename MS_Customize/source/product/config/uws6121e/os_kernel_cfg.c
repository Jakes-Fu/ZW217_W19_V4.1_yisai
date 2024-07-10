/******************************************************************************
 ** File Name:      oscfg.c                                                   *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           11/08/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines most of system's static tasks.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/08/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "priority_kernel.h"

enum
{
	PRI_RTOS_KERNEL_OFFSET = 0,
	PRI_HISR_PRIO_0_OFFSET = 0,
	PRI_HISR_PRIO_1_OFFSET = 1,
	PRI_HISR_PRIO_2_OFFSET = 2,
	PRI_CHNG_FREQ_TASK_OFFSET = 4,
	PRI_RTOS_MANAGE_OFFSET = 5,
	MAX_KERNEL_PRIORITY_OFFSET = MAX_KERNEL_PRI_NUM
};

//Kernel Task
SCI_DEFINE_TASK_PRI(PRI_RTOS_KERNEL   , SCI_KERNEL_PRI(PRI_RTOS_KERNEL_OFFSET));
SCI_DEFINE_TASK_PRI(PRI_HISR_PRIO_0   , SCI_KERNEL_PRI(PRI_HISR_PRIO_0_OFFSET));
SCI_DEFINE_TASK_PRI(PRI_HISR_PRIO_1   , SCI_KERNEL_PRI(PRI_HISR_PRIO_1_OFFSET));
SCI_DEFINE_TASK_PRI(PRI_HISR_PRIO_2   , SCI_KERNEL_PRI(PRI_HISR_PRIO_2_OFFSET));
SCI_DEFINE_TASK_PRI(PRI_CHNG_FREQ_TASK, SCI_KERNEL_PRI(PRI_CHNG_FREQ_TASK_OFFSET));
SCI_DEFINE_TASK_PRI(PRI_RTOS_MANAGE   , SCI_KERNEL_PRI(PRI_RTOS_MANAGE_OFFSET));
