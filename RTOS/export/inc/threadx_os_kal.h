/******************************************************************************
 ** File Name:      threadx_os_kal.h                                          *
 ** Author:         rick.zhang                                                *
 ** DATE:           05/24/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file will provide advanced os api and data struct    *
 **                 for osa_kal source code to access KAL layer¡£             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/24/2006     rick.zhang       Create.                                   *
 ******************************************************************************/

 
#ifndef _THREADX_OS_KAL_H
#define _THREADX_OS_KAL_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"

// Define RTOS kernel API.
#ifdef WIN32
    #include "tx_api_win32.h"
#else
	#ifndef _FEATURE_ARM
    	#include "tx_api_thumb.h"
	#else
    	#include "tx_api_arm.h"
	#endif
#endif

/* Import KAL header files */
#include "kal_interface.h"
#include "kal_internal.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Type Define                                      *
 **---------------------------------------------------------------------------*/




typedef struct kal_internal_poolid_struct       KAL_SCI_BLOCK_POOL_T;
typedef struct kal_internal_mem_poolid_struct   KAL_SCI_BYTE_POOL_T;
typedef        RAL_EVENT_GROUP_T                KAL_SCI_EVENT_GROUP_T;
typedef struct kal_internal_msgqid_struct       KAL_SCI_QUEUE_T;
typedef struct kal_internal_timerid_struct      KAL_SCI_TIMER_T;
typedef struct kal_internal_mutexid_struct      KAL_SCI_MUTEX_T;
typedef struct kal_internal_semid_struct        KAL_SCI_SEMAPHORE_T;
typedef struct kal_internal_taskid_struct       KAL_SCI_THREAD_T;

typedef TX_BLOCK_POOL           RAL_BLOCK_POOL_T;
typedef TX_BYTE_POOL            RAL_BYTE_POOL_T;
typedef TX_EVENT_FLAGS_GROUP    RAL_EVENT_GROUP_T;
typedef TX_QUEUE                RAL_QUEUE_T;
typedef TX_TIMER                RAL_TIMER_T;
typedef TX_MUTEX                RAL_MUTEX_T;
typedef TX_SEMAPHORE            RAL_SEMAPHORE_T;


/* ThreadX byte memory head structure */
typedef struct RAL_BYTEPOOL_HEADER_STRUCT
{
    CHAR_PTR    tx_next_memory;
    ULONG       tx_mem_flag;
} RAL_BYTEPOOL_HEADER;

/* ThreadX block memory head structure */
typedef struct RAL_BLOCKPOOL_HEADER_STRUCT
{
    CHAR_PTR    tx_next_block;
} RAL_BLOCKPOOL_HEADER;


typedef enum KAL_PREEMPT_E
{
    KAL_DISABLE_PREEMPT,
    KAL_ENABLE_PREEMPT
} KAL_PREEMPT_T;

typedef void (* tx_entry_function)(ULONG);

/**---------------------------------------------------------------------------*
 **                         MACRO Define                                      *
 **---------------------------------------------------------------------------*/

#define         SCI_BYTEPOOL_OVERHEAD   sizeof(RAL_BYTEPOOL_HEADER) 

/* Threads that have higher priorities than the one thread's 
 * preemption threshold will be allowed to preempt this thread. 
 * A thread can disable all preemption by setting its preemtion
 * threshold to 0. */
#define TX_DISABLE_PREEMPT  0
#define CONVERT_PREEMPT_THRESHOLD(_OPTION)  \
        ((_OPTION) == KAL_DISABLE_PREEMPT) ? TX_DISABLE_PREEMPT : tx_priority;




/* Defined such macro for KAL task type definition */
#define KAL_STATIC_THREAD           0x20021204
#define KAL_DYNAMIC_THREAD          0x20021208
#define KAL_INVALID_THREAD          0x0
#define SCI_INVALID_THREAD          0x0

/* Get thread location from SCI_THREAD_T */
#define RAL_THREAD_GAP  8   /* block_type + thread_id */

/* Get queue location from SCI_THREAD_T */
#define RAL_QUEUE_GAP   (RAL_THREAD_GAP + sizeof(TX_THREAD))

/* Get event location from SCI_THREAD_T */
#define RAL_EVENT_GAP   (RAL_THREAD_GAP + sizeof(TX_QUEUE))


/**---------------------------------------------------------------------------*
 **                         Global Value                                      *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Prototype                                *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description:    The InitOSABuffer function is used to create memory buffer 
//                  pool for OSA entity. 
//	Global resource dependence: 
//  Author: rick.zhang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_InitOSABuffer(void);



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _THREADX_OS_KAL_H


