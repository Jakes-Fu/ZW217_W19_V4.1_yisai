/*********************************************************************

   (C) Spreadtrum Communications Co. Ltd, 
            All rights reserved.

*   It includes the definations and function prototypes that 
*   are common to release and debug versions of KAL on Nucleus.

*   Component-specific prefix : kal

*********************************************************************/

#ifndef _KAL_THREADX_COMMON_DEFS_H
#define _KAL_THREADX_COMMON_DEFS_H

/*****************
Include Statements
******************/

//#include "nucleus.h"
//#include "pm_defs.h"  /* For the declaration of PM_PCB, PM_HEADER_STRUCT */
//#include "dm_defs.h"  /* For the declaration of DM_HEADER */


//#include "rtos_types.h"   /* SCI OSA declaration for KAL         */
#include "threadx_os.h"
#include "threadx_os_kal.h"
#include "os_apiext.h"
#include "kal_interface.h"

/************************
Macros Without Parameters 
*************************/
#define TX_PLUS_Ticks_Per_Second        1000        /* ThreadX ticks increase one by one milli second */
#define KAL_TICKS_FOR_TEN_MILLI_SEC   ( TX_PLUS_Ticks_Per_Second / 100 )
#define KAL_TICKS_FOR_ONE_SEC          TX_PLUS_Ticks_Per_Second

/*****************
Enums Without Tags 
******************/

/***************
Type Definitions 
****************/
/*
typedef TX_THREAD               kal_os_task_type;
typedef TX_QUEUE                kal_os_queue_type;
typedef TX_MUTEX                kal_os_mutex_type;
typedef TX_SEMAPHORE            kal_os_sem_type;
typedef TX_TIMER                kal_os_timer_type;
typedef TX_BLOCK_POOL           kal_os_pool_type;
typedef TX_BYTE_POOL            kal_os_mem_pool_type;
*/

typedef RAL_THREAD_T               kal_os_task_type;    /* Include other component  */
typedef RAL_QUEUE_T                kal_os_queue_type;   /* Similar to TX_QUEUE      */
typedef RAL_MUTEX_T                kal_os_mutex_type;   /* Similar to TX_MUTEX      */
typedef RAL_SEMAPHORE_T            kal_os_sem_type;     /* Similar to TX_SEMAPHORE  */
typedef RAL_TIMER_T                kal_os_timer_type;   /* Similar to TX_TIMER      */
typedef RAL_BLOCK_POOL_T           kal_os_pool_type;    /* Similar to TX_BLOCK_POOL */
typedef RAL_BYTE_POOL_T            kal_os_mem_pool_type;/* Similar to TX_BYTE_POOL  */

/* VOID  and ULONG are THREADX defined data-types */
typedef VOID (*kal_tx_task_func_ptr) (ULONG);
typedef VOID (*kal_tx_timer_func_ptr) (ULONG);

/********
Constants 
*********/

/*********************
Macros With Parameters 
**********************/

/* Pass the pool-name to identify the mutex that protects the pool*/
#define KAL_INIT_MEM_PROTECT( entity_id, mem_type, pool_id, pool_name ){\
  pool_id->protecting_mutex = kal_create_mutex( entity_id, \
                                                mem_type, pool_name );\
}

#define KAL_BEGIN_MEM_PROTECT( pool_id ) {\
    kal_take_mutex( pool_id->protecting_mutex );\
}

#define KAL_END_MEM_PROTECT( pool_id ){\
    kal_give_mutex( pool_id->protecting_mutex );\
}

/***************************
Exported Function Prototypes 
****************************/
extern kal_uint32 kal_secs_to_ticks( kal_uint32    secs );
extern kal_uint32 kal_milli_secs_to_ticks( kal_uint32    milli_secs );
extern kal_uint32 kal_ticks_to_secs( kal_uint32    ticks );
extern kal_uint32 kal_ticks_to_milli_secs( kal_uint32    ticks );
extern void* kal_internal_alloc( kal_uint8 entity_id, kal_uint8 alloc_id, 
                                 kal_uint8 mem_type,  kal_uint32 size );
extern void  kal_internal_free( void* mem_ptr ); 
extern void  kal_allocate_resources( void );
extern kal_uint32 kal_get_next_free_task_index( void );/* cr229061 */
extern kal_taskid kal_get_task_tcb( kal_uint32 indx);/* cr229061 */
extern void kal_set_task_index_used( kal_uint32 index );/* cr229061 */
extern kal_bool kal_get_task_index_used( kal_uint32 index );/* cr229061 */
extern void kal_add_free_task_index( kal_taskid taskid );
extern kal_char* getenv( const kal_char* env );
extern int atoi ( const char *str );
extern kal_status kal_int_allocate_memory( kal_uint8 mem_type, void ** mem_ptr,
                                           kal_uint32 size );
extern kal_status kal_initialize_mem_pools( kal_mem_info * mem_pools_info );
#ifdef LOG_MEM_HISTORY
extern void kal_initialize_mem_stats_arrays( void );
extern void kal_deinitialize_mem_stats_arrays( void  );
#endif
extern kal_status kal_deinitialize_mem_pools( void );

/******************************
Declarations Of Exported Globals 
*******************************/

extern kal_uint16          kal_max_num_buff_pools_g;/* +cr229061 */
extern kal_uint16*         kal_task_ptrs_alloc_info_g;/* +cr229061 */
extern kal_mem_info *     kal_mem_pools_info_g;

#endif /*_KAL_THREADX_COMMON_DEFS_H*/
