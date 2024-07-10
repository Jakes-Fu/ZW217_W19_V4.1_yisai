/******************************************************************************
 ** File Name:      atc_main.c                                                *
 ** Author:         Xueli.lv                                                  *
 ** Date:           07/02/2003                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 07/02/2003     Xueli.lv         Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_api.h"
#include "tasks_id.h"
#include "env_atc_signal.h"
#include "atc_port.h"
#include "priority_base.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

extern void ATC_Task(uint32 argc, void * argv);


// ATC thread define.
#define   P_ATC_STACK_SIZE              4608
static uint32    P_ATC_THREAD_ADDR[P_ATC_STACK_SIZE >> 2];
#define   P_ATC_QUEUE_NUM               60 
static uint32    P_ATC_QUEUE_ADDR[P_ATC_QUEUE_NUM * SCI_QUEUE_ITEM_SIZE];



// Create static thread macro.
#define CREATE_ATC_STATIC_THREAD(NAME, THREAD_ENTRY, ARGC, PRIO) \
        SCI_CREATE_STATIC_THREAD( NAME, "T_"#NAME, THREAD_ENTRY, ARGC, 0, \
            NAME##_THREAD_ADDR, NAME##_STACK_SIZE, PRIO, SCI_PREEMPT, \
            "Q_"#NAME, NAME##_QUEUE_ADDR, NAME##_QUEUE_NUM, SCI_AUTO_START);            


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
// Init PS.
uint32 SCI_InitAtc(void)
{
	CREATE_ATC_STATIC_THREAD(P_ATC, ATC_Task, 0, PRI_BASE_ATC_TASK);
	return 0;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
