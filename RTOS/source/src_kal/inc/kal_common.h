/*********************************************************************

   (C) Spreadtrum Communications Co. Ltd, 
            All rights reserved.

  This file exports 
     1.  APIs whose prototypes are same for DEBUG and
         RELEASE versions of KAL.
     2.  All Type declaraions 

  Component-specific prefix : kal

*********************************************************************/

#ifndef _KAL_COMMON_H
#define _KAL_COMMON_H

#ifndef WIN32
#include "os_api.h"
#endif

#if (defined( PRJ_TDPS_UEIT ) && ( !defined (TIANLONG_MOCOR) )) || defined( UEIT_MOCOR )
typedef unsigned char		BOOLEAN;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned long int	uint32;
#ifndef UEIT_MOCOR
typedef unsigned long int 	uint64;
#endif
typedef unsigned int		uint;

typedef signed char			int8;
typedef signed short		int16;
typedef signed long int		int32;
#endif

#if (! defined( _INCLS_FROM_KAL_INTERFACE_H ) ) 
  # error " kal_common.h can't be included directly, instead include kal_interface.h for use of KAL "
#endif

/*+CR97669*/
#if ( ! defined( TARGET_ARM ) ) || defined( WIN32 )
//#ifndef TARGET_ARM
#ifndef SCI_TRACE_LOW
#define SCI_TRACE_LOW    SCI_Trace
#endif
#ifndef SCI_TRACE_ID
#define SCI_TRACE_ID    SCI_TraceEx
#endif
#ifndef SCI_MEMSET
#define SCI_MEMSET       kal_mem_set
#endif
#ifndef SCI_MEMCPY
#define SCI_MEMCPY       kal_mem_cpy
#endif
#ifndef SCI_FREE
#define SCI_FREE         free_ctrl_buffer
#endif
#ifndef SCI_ALLOC
#define SCI_ALLOC        get_ctrl_buffer
#endif
#ifndef SCI_ASSERT
#define SCI_ASSERT(_EXP)    (_EXP) ? (void)0 : SCI_Assert(#_EXP, __FILE__, __LINE__)
#endif
#ifndef SCI_CREATE_SIGNAL
#define SCI_CREATE_SIGNAL(_SIG_PTR, _SIG_CODE, _SIG_SIZE, _SENDER) ;
#endif
#endif
/*-CR97669*/

/*****************
Include Statements
******************/


/************************
Macros Without Parameters 
*************************/

#define KAL_NILTASK_ID          (kal_taskid)NULL
#define KAL_NILTIMER_ID         (kal_timerid)NULL
#define KAL_NILPOOL_ID          (kal_poolid)NULL
#define KAL_NILMUTEX_ID         (kal_mutexid)NULL
#define KAL_NILSEM_ID           (kal_semid)NULL
#define KAL_NILQ_ID             (kal_msgqid)NULL

/* The following declaraion is required since KAL user does not
 * include ANSI C header files */
#ifndef NULL
#define NULL 0
#endif


/* If buffer transaction logging is enabled, 12 bytes of
 * data is required per buffer operation. The following 
 * macro definition represents this.
 */
#define KAL_TRANS_REC_SIZE        13


/* CR85547 */
#define kal_fatal_error_handler(para1, para2, para3) \
    kal_fatal_error_handler_info(para1, para2, para3, __FILE__, __LINE__)
/* CR85547 END */

/*****************
Enums Without Tags 
******************/


/* KAL wait mode signifies whether a KAL API is supposed to block
 * or do not block. */
typedef enum
{
    KAL_NO_WAIT,
    KAL_INFINITE_WAIT
}kal_wait_mode;


/* Signifies the return value of ITC( Inter Thread Communication ) calls.
 */
typedef enum
{
    KAL_SUCCESS,
    KAL_ERROR,
    KAL_Q_FULL,
    KAL_Q_EMPTY,
    KAL_SEM_NOT_AVAILABLE
}kal_status;

/* Define the error values passed by KAL to the user registered
 * routine to be called on encountering a fatal error. The errors
 * are defined with the following naming convention.
 * KAL_ERROR_<KAL Module where error happens>_<Error description> */
typedef enum
{
    KAL_ERROR_USER_OPERATION_FAILED = 0x0,
	
	KAL_ERROR_USER_CODE_START,
	KAL_ERROR_USER_CODE_END = 0x64,
	
    KAL_ERROR_KAL_INTERNAL_FAIL = 0x65,

    KAL_ERROR_INIT_FAILED = 0x0101,
    KAL_ERROR_INIT_MEM_INIT_FAILED,
    KAL_ERROR_INIT_CACHE_INIT_FAILED,
    KAL_ERROR_INIT_ALLOCATE_RESOURCES_FAILED,

    KAL_ERROR_TASKMNGT_CREATE_MEM_FAILED = 0x0201,
    KAL_ERROR_TASKMNGT_CREATE_STACK_FAILED,
    KAL_ERROR_TASKMNGT_CREATE_FAILED,
    KAL_ERROR_TASKMNGT_DELETE_FAILED,
    KAL_ERROR_TASKMNGT_SLEEP_FAILED,

    KAL_ERROR_ITC_QCREATE_MEM_FAILED = 0x0301,
    KAL_ERROR_ITC_QCREATE_DATA_MEM_FAILED,
    KAL_ERROR_ITC_QCREATE_FAILED,
    KAL_ERROR_ITC_QDELETE_FAILED,
    KAL_ERROR_ITC_QENQUE_FAILED,
    KAL_ERROR_ITC_QENQUE_MEM_FAILED,
    KAL_ERROR_ITC_QDEQUE_FAILED,

    KAL_ERROR_ITC_MCREATE_MEMFAILED = 0x0401,
    KAL_ERROR_ITC_MDELETE_FAILED,
    KAL_ERROR_ITC_MCREATE_FAILED,
    KAL_ERROR_ITC_MTAKE_FAILED,
    KAL_ERROR_ITC_MGIVE_FAILED,

    KAL_ERROR_ITC_SCREATE_MEMFAILED = 0x0501,
    KAL_ERROR_ITC_SDELETE_FAILED,
    KAL_ERROR_ITC_SCREATE_FAILED,
    KAL_ERROR_ITC_STAKE_FAILED,
    KAL_ERROR_ITC_SGIVE_FAILED,

    KAL_ERROR_TIMER_CREATE_MEM_FAILED = 0x0601,
    KAL_ERROR_TIMER_CREATE_FAILED,
    KAL_ERROR_TIMER_DELETE_FAILED,
    KAL_ERROR_TIMER_SET_FAILED,
    KAL_ERROR_TIMER_CANCEL_FAILED,
    KAL_ERROR_TIMER_GETTIME_FAILED,
    KAL_ERROR_TIMER_GET_TIME_REMAINING_FAILED,
    KAL_ERROR_TIMER_SCHED_FAILED,

    KAL_ERROR_MEMMNGT_ALLOC_FAILED = 0x0701,
    KAL_ERROR_MEMMNGT_DEALLOC_FAILED,
    KAL_ERROR_MEMMNGT_MEMCPY_FAILED,
    KAL_ERROR_MEMMNGT_MEMSET_FAILED,
    KAL_ERROR_MEMMNGT_MEMMOVE_FAILED,
   
    KAL_ERROR_BUFFMNGR_CREATE_MEM_FAILED = 0x0801,
    KAL_ERROR_BUFFMNGR_CREATE_FAILED,
    KAL_ERROR_BUFFMNGR_DELETE_FAILED,
    KAL_ERROR_BUFFMNGR_GETBUFF_FAILED,
    KAL_ERROR_BUFFMNGR_RELEASEBUFF_FAILED,
    KAL_ERROR_BUFFMNGR_ISVALID_FAILED,

    KAL_ERROR_ERRORMNGR_FATALHANDLER_FAILED = 0x0901,
    KAL_ERROR_ERRORMNGR_REGISTERHANDLER_FAILED,

    KAL_ERROR_DEBUGBUFFMNGR_GETPOOLSTAT_FAILED = 0x0A01,
    KAL_ERROR_DEBUGBUFFMNGR_RESETPOOLSTAT_FAILED,
    KAL_ERROR_DEBUGBUFFMNGR_GETBUFFSTAT_FAILED,
    KAL_ERROR_DEBUGBUFFMNGR_RESETBUFFSTAT_FAILED,
    KAL_ERROR_DEBUGBUFFMNGR_POOLID_VALIDATION_FAILED,
    KAL_ERROR_DEBUGBUFFMNGR_ALLOC_MEM_FOR_STAT_FAILED,

    KAL_ERROR_DEBUGERRORMNGR_REGISTERPRINT_FAILED = 0x0B01,
    KAL_ERROR_DEBUGERRORMNGR_PRINT_FAILED,

    KAL_ERROR_DEBUGITC_GETQSTAT_FAILED = 0x0C01,
    KAL_ERROR_DEBUGITC_RESETQSTAT_FAILED,

    KAL_ERROR_DEBUGTIMER_GETTIMERSTAT_FAILED = 0x0D01,
    KAL_ERROR_DEBUGTIMER_RESETTIMERSTAT_FAILED,
   
    KAL_ERROR_DEBUGTASK_GETTASKSTAT_FAILED = 0xE01,    
   
    KAL_ERROR_DEINIT_RESOURCES_FAILED = 0xF01

} kal_error_type;


/* Define the task priorities. */
typedef enum
{
    KAL_HEIGHEST_PRIORITY =  0,
    KAL_PRIORITY_ONE      ,
    KAL_PRIORITY_TWO      ,
    KAL_PRIORITY_THREE    ,
    KAL_PRIORITY_FOUR     ,
    KAL_PRIORITY_FIVE     ,
    KAL_PRIORITY_SIX      ,
    KAL_PRIORITY_SEVEN    ,
    KAL_PRIORITY_EIGHT    ,
    KAL_PRIORITY_NINE     ,
    KAL_PRIORITY_TEN      ,
    KAL_PRIORITY_ELEVEN   ,
    KAL_PRIORITY_TWELVE   ,
    KAL_PRIORITY_THIRTEEN ,
    KAL_PRIORITY_FOURTEEN ,
    KAL_PRIORITY_FIFTEEN  ,
    KAL_PRIORITY_SIXTEEN  ,
    KAL_PRIORITY_SEVENTEEN,
    KAL_PRIORITY_EIGHTEEN ,
    KAL_LOWEST_PRIORITY   
} kal_priority_type;

/* Defines the state of the mutex variable */
typedef enum
{
    KAL_MUTEX_CREATED,
    KAL_MUTEX_GIVEN,
    KAL_MUTEX_TAKEN
} kal_mutex_state;  

/* The state of a buffer in a buffer pool.
 * To optimize the space occupied by the buffer info structure, the
 * buffer state is defined as kal_uint8. This enum is kept because
 * the code does comparison of buffer state with these enum values.
 * Buffer state is returned in the buffer statistics calls
 */
typedef enum
{
    KAL_BUFF_CREATED,
    KAL_BUFF_DEALLOCATED,
    KAL_BUFF_ALLOCATED
}kal_buff_state;

typedef enum 
{
    MEM_INVALID_STATE,
    KAL_MEM_DEALLOCATED,
    KAL_MEM_ALLOCATED
} kal_mem_state;

typedef enum
{
    KAL_TIMER_CREATED,
    KAL_TIMER_SET,
    KAL_TIMER_CANCELED,
    KAL_TIMER_EXPIRED
} kal_timer_state;                

typedef enum
{
	KAL_BUFFMNGR_COMPONENT = 1,
	KAL_TASKMNGT_COMPONENT,
	KAL_TIMER_COMPONENT,
	KAL_ITC_QUEUE_COMPONENT,
	KAL_ITC_MUTEX_COMPONENT,
	KAL_ITC_SEM_COMPONENT,
	KAL_MEMMNGT_COMPONENT,
	KAL_LAST_COMPONENT
} kal_alloc_id;		

typedef enum
{
	KAL_CLOCK_FREE_MODE,
	KAL_CLOCK_SYNC_MODE
} kal_clock_mode;		
		
/***************
Type Definitions 
****************/

/* KAL Basic Types */
                                            /* Defined in OSA */
typedef unsigned char         kal_uint8;    /* uint8    */
typedef signed char           kal_int8;     /* int8     */
typedef char                  kal_char;     /* char     */
typedef unsigned short int    kal_uint16;   /* uint16   */
typedef signed short int      kal_int16;    /* int16    */
typedef unsigned long         kal_uint32;   /* uint32   */
typedef signed   long         kal_int32;    /* int32    */
typedef int                   kal_int;      /* int      */
typedef unsigned int          kal_uint;     /* uint     */

/* @rick.zhang Tianji2_systest1 */
/* Modify it for compile lib file in windows */
#if (defined(WIN32_GEN ) ) || defined( WIN32 )
typedef unsigned char         kal_bool;
/* +cr223726 */
#define KAL_FALSE             ((kal_bool)0)
#define KAL_TRUE              ((kal_bool)1)
/* -cr223726 */
#else
typedef enum 
{ 
    KAL_FALSE, 
    KAL_TRUE 
} kal_bool;
#endif  /* end of ifdef WIN32 */

/*  Define data structures with all internal information
 *  hidden. Exporting the internal information will result in
 *  inclusion of system header files. So this is avoided. */

typedef struct kal_taskid_struct
{
    kal_uint8   unused;
}*kal_taskid;

typedef struct kal_semid_struct
{
    kal_uint8   unused;
}*kal_semid;

typedef struct kal_msgqid_struct
{
    kal_uint8   unused;
}* kal_msgqid;

typedef struct kal_mutexid_struct
{
    kal_uint8   unused;
}*kal_mutexid; 


typedef struct kal_poolid_struct
{
    kal_uint8   unused;
} *kal_poolid;

typedef struct kal_timerid_struct
{
    kal_uint8 unused;
}* kal_timerid;

/* no_of_entities       : Total number of entities which will use services of 
 *                        kal.
 * entity_max_alloc_ids : The base address of array indexed with entity_id. 
 *                        Each index contains the maximum allocator id for
 *                        each entity. Then allocator ids for each entity
 *                        range from 0 <->( max_alloc_id (for given entity)-1)
 */                        
typedef struct
{
    kal_uint8    no_of_entities;
    kal_uint8*   entity_max_alloc_ids;
} kal_entity_info;

typedef struct
{
    kal_uint16 /*@alt int@*/  max_num_timers;
    kal_uint16  /*@alt int@*/  max_num_queues;    /* cr229061 */
    kal_uint16  /*@alt int@*/  max_num_buff_pools;/* cr229061 */
    kal_uint16  /*@alt int@*/  max_num_tasks;     /* cr229061 */
    kal_uint16  /*@alt int@*/  max_num_mutexes;   /* cr229061 */
    kal_uint16 /*@alt int@*/  max_num_sems;
    kal_entity_info *         entity_info;   
} kal_resource_info_struct;

/* name            : Name of dynamic memory pool
 * start_address   : The starting address in memory.
 * size            : Number of bytes available in this pool
 */ 
typedef struct
{
    kal_char *       name;
    void*            start_address;
    kal_uint32       size;
} kal_mem_pool_info;    

/* no_mem_pools  : Gives Number of memory pools in given type. Then valid
 *                 pool numbers in each memory type are,
 *                 0<->(no_mem_pools -1).
 * mem_pool_info : The base address of array indexed with pool number.
 *                 Each index contains information about each memory pool.
 */                 

typedef struct
{
    kal_uint8           no_mem_pools;
    kal_mem_pool_info*  mem_pool_info;
} kal_mem_type_info;

/* no_mem_types                  : No. of different memory types from  which 
 *                                 memory allocations occur. Then valid range
 *                                 for memory type is 0<->(no_mem_types-1)
 * mem_type_for_kal_allocations  : memory type from which allocations internal
 *                                 to kal are satisfied. Also this type is
 *                                 used for allocating the control blocks for
 *                                 all kal components.
 * max_num_mem_allocations       : total number of dynamic memory allocations 
 *                                 from memory pools for all types.
 * mem_info                      : The base address of array indexed with mem 
 *                                 type. Every position contains information 
 *                                 about each memory type.
 */ 
typedef struct
{
    kal_uint8           no_mem_types;
    kal_uint8           mem_type_for_kal_allocations;
    kal_uint32          max_num_mem_allocations;
    kal_mem_type_info*  mem_info;
} kal_mem_info;


typedef void (* kal_task_func_ptr)(void *);

/* CR85447 */
#if 0
typedef void (* kal_error_func_ptr)(kal_char *, kal_error_type, kal_int32);
#else
typedef void (* kal_error_func_ptr)(kal_char *, kal_error_type, kal_int32, void *, kal_uint32);
#endif
/*CR85447*/

typedef void (* kal_timer_func_ptr)(void *);

/* max_num_bytes_allocated  : Maximum number of bytes allocated from the heap.
 * curr_num_bytes_allocated : Current number of bytes allocated from the heap.
 * total_times_allocated    : Total number of times allocation happened from
 *                            the heap. 
 */
typedef struct kal_mem_stats_struct
{
    kal_uint32    max_num_bytes_allocated;
    kal_uint32    curr_num_bytes_allocated;
    kal_uint32    total_times_allocated; 
}kal_mem_stats_struct;

/* max_num_buffs_allocated : Maximum number of buffers allocated from the pool.
 * curr_num_buffs_allocated: Current number of buffers allocated from the pool.
 * total_times_allocated   : Total number of times a buffer was allocated from 
 *                           the pool 
 */
typedef struct
{
    kal_uint16    max_num_buffs_allocated;
    kal_uint16    curr_num_buffs_allocated;
    kal_uint32    total_times_allocated;
}kal_alloc_stats_struct;


/* allocated_stack_sz   : The stack size allocated for a task at
 *                        task creation time.
 * used_stack_sz        : The maximum stack size used by
 *                        the task at any point of time in execution 
 */
typedef struct kal_task_statistics_struct
{
    const kal_char*     task_name;
    kal_uint32          allocated_stack_sz;
    kal_uint32          used_stack_sz;
}* kal_task_statistics;

typedef struct kal_all_task_statistics_struct
{
   kal_uint8             num_of_tasks;
   kal_task_statistics*  task_stat;
}* kal_all_task_statistics;

/* queue_name           : The name of the queue.
 * max_msg_size         : Indicates the size of the maximum message enqueued
 *                        for a variable length queue.
 * current_msgs_enqued  : The current number of messages enqued.
 * max_msgs_enqued      : Maximum number of messages enqueued in the queue
 *                        at any point of time.
 * max_msgs_possible    : The limit on number of messages that can be enqueued
 *                        in the queue at any point of time.
 */
typedef struct kal_queue_statistics_struct
{
   const kal_char*     queue_name;
   kal_uint16          max_msg_size;
   kal_uint16          current_msgs_enqued;
   kal_uint16          max_msgs_enqued; 
   kal_uint16          max_msgs_possible; 
}* kal_queue_statistics;

typedef struct kal_all_queue_statistics_struct
{
   kal_uint8             num_of_queues;
   kal_queue_statistics* q_stat;
}* kal_all_queue_statistics;

typedef struct kal_mutex_statistics_struct
{
   const kal_char*     mutex_name;
   const kal_char*     task_name;
   kal_mutex_state     mutex_state;
}* kal_mutex_statistics;

typedef struct kal_all_mutex_statistics_struct
{
   kal_uint16            num_of_mutexs;
   kal_mutex_statistics* mutex_stat;
}* kal_all_mutex_statistics;

typedef struct kal_sem_statistics_struct
{
   const kal_char*     sem_name;
   kal_uint16          current_value;
   kal_uint16          max_value;
}* kal_sem_statistics;

typedef struct kal_all_sem_statistics_struct
{
   kal_uint16          num_of_sems;
   kal_sem_statistics* sem_stat;
}* kal_all_sem_statistics;

/* buff_state           :  The state of the buffer
 *                         ( CREATED / DEALLOCATED / ALLOCATED ).
 * size                 :  The requested size in bytes ( allocated
 *                         size in debug version as the footer is
 *                         appended to the end of the requested
 *                         size ).
 */

typedef struct
{
    kal_uint8  buff_state;
    kal_uint16      size;
    
} kal_buff_info_struct;

/* alloc_ent_id         :  The owner entity for module. 
 * alloc_id             :  The owner module.
 * file_ent_id          :  The owner entity for file.
 * file_code            :  The file code from which allocation
 *                         is done.
 * line_no              :  The line number in the file where 
 *                         allocation is done.
 */

typedef struct
{
    kal_uint8       alloc_ent_id;
    kal_uint8       alloc_id;
    kal_uint8       file_ent_id;
    kal_uint16      file_code;
    kal_uint16      line_no;
    
} kal_buff_alloc_struct;

/* buff_info            :  Pointer to the array which keeps the buffer
 *                         allocation information for all the buffers
 *                         in the pool. This is created for the pool
 *                         by default.
 * alloc_info           :  Pointer to the array which keeps the buffer
 *                         owner information for all the buffers in
 *                         the pool. This can be created at run time.
 */

typedef struct
{
    kal_buff_info_struct*  buff_info;
    kal_buff_alloc_struct* alloc_info;
} kal_buff_stats_struct; 

/* max_num_buff_allocated  : The maximum number of buffers allocated
 *                           in a particular subgroup.
 * curr_num_buff_allocated : The number of buffers currently 
 *                           allocated in a particular subgroup.
 */

typedef struct
{
    kal_uint16       max_num_buff_allocated;
    kal_uint16       curr_num_buff_allocated;
} kal_subgroup_stats_struct;

/* logtobase2_of_range  :  The log to the base 2 of the range ( the 
 *                         buffer size for which data to be collected ).
 * lower_limit          :  The lowest requested buffer size value for 
 *                         which the allocation info to be logged.
 * upper_limit          :  The maximum requested buffer size for which 
 *                         the allocation info to be logged.
 * subgroup_stat        :  The array which stores the group wise 
 *                         allocation information for the pool.
 */

typedef struct 
{
    kal_uint8                    logtobase2_of_range;
    kal_uint16                   lower_limit;
    kal_uint16                   upper_limit;
    kal_subgroup_stats_struct*   subgroup_stat;
} kal_group_stats_struct;

/* pool_name            :  Name of the Buffer Pool.
 * num_buffs            :  Total number of buffers in the pool.
 * buff_size            :  The size of a buffer in the pool.
 *                         This doesnot include the overhead for
 *                         the buffer header or footer. The buffer
 *                         size equals the buffer size requested
 *                         by the user at pool creation time.
 * total_num_allocation :  Total number of times a successful 
 *                         allocation happened from this pool.
 * current_allocation   :  Current number of buffers allocated from
 *                         the pool.
 * max_num_allocated    :  Maximum number of buffers allocated from
 *                         the pool since buffer pool creation.
 * max_size_requested   :  The maximum size of the buffer requested
 *                         by the user from this pool.
 * buff_stat            :  Pointer to the structure that
 *                         keeps both buffer allocation info and
 *                         buffer owner info. (see comment on 
 *                         kal_buff_stats_struct ).
 * group_stat           :  Pointer to the structure that keeps 
 *                         allocation information for the buffers
 *                         based on subgroups decided by the 
 *                         application. This can be initialized at
 *                         run time.
 * alloc_stat           :  Array of pointers that stores the allocation info
 *                         for the buffers based on the (entity, module) which
 *                         allocated it. This info is kept only if 
 *                         buffer owner info logging is enabled.
 */

typedef struct kal_pool_statistics_struct
{
    const kal_char*            pool_name;
    kal_uint16                 num_buffs;
    kal_uint16                 buff_size;
    kal_uint16                 total_num_allocation;
    kal_uint16                 current_allocation;
    kal_uint16                 max_num_allocated;
    kal_uint16                 max_size_requested;
    kal_bool                   log_trans;
    kal_buff_stats_struct*     buff_stat;
    kal_group_stats_struct*    group_stat;
    kal_alloc_stats_struct**   alloc_stat;
}* kal_pool_statistics;

/* num_of_pools        :  Provides the number of buffer pools
 *                        created. The array pool_stat should be
 *                        accessed for 'num_of_pools' entries.
 * pool_stat           :  The pointer to the base of the array
 *                        that stores pointers to the statistics of
 *                        inidividual pools.
 */

typedef struct kal_all_pool_statistics_struct
{
    kal_uint16             num_of_pools;/* cr229061 */
    kal_pool_statistics*  pool_stat;
}* kal_all_pool_statistics;

/* pool_id       : Value returned by kal_create_buff_pool().
 * start_address : The lowest address to which the user 
 *                 can write data in this buffer.
 * end_address   : The heighest address to which the user can
 *                 write data in this buffer pool.
 */
typedef struct kal_pool_info_struct {
  kal_poolid  pool_id;
  void*       start_address;
  void*       end_address;
} kal_pool_info_struct;

typedef struct kal_timer_statistics_struct
{
   kal_timer_state    timer_state;
   const kal_char*    timer_name;
   kal_uint16         num_times_expired;
   kal_uint16         num_times_canceled;
}* kal_timer_statistics;

typedef struct kal_all_timer_statistics_struct
{
   kal_uint16            num_of_timers;
   kal_timer_statistics* timer_stat;
}* kal_all_timer_statistics;

typedef struct kal_all_int_mem_statistics_struct
{
    kal_uint8               no_entities;
    kal_uint8               no_kal_comp; 
    kal_mem_stats_struct ** int_mem_stats;
}*kal_all_int_mem_statistics;    

typedef struct kal_all_ext_mem_statistics_struct
{
    kal_uint8               no_entities;
    kal_uint8*              entity_max_module_ids;
    kal_mem_stats_struct ** ext_mem_stats;
}* kal_all_ext_mem_statistics;

typedef struct kal_all_mem_type_mem_statistics_struct
{
    kal_uint8               no_entities;
    kal_uint8               no_mem_types;
    kal_mem_stats_struct ** mem_type_mem_stats;
}* kal_all_mem_type_mem_statistics;    

typedef struct kal_all_mem_statistics_struct
{
   struct kal_all_int_mem_statistics_struct       int_mem_stat;
   struct kal_all_ext_mem_statistics_struct       ext_mem_stat;
   struct kal_all_mem_type_mem_statistics_struct  mem_type_stat;
}*kal_all_mem_statistics;

typedef void(*kal_print_string_func_ptr)(const kal_char*);

/********
Constants 
*********/

/*********************
Macros With Parameters 
**********************/

/***************************
Exported Function Prototypes 
****************************/

/*__________________        Kal Initialize     ________________________*/

/* These routines are to be called during the initialization phase. 
 * Call the following three routines in the very beginning.  */

/* Initializes the kal layer.  */
void kal_initialize( kal_resource_info_struct*  resource_max_list, kal_clock_mode clk_mode );

/* Initializes kal for the Memory requirement.  */
void kal_initialize_mem( kal_mem_info * mem_info );

/* This routine stores the pointer to the fatal error handler.  */
void kal_register_fatal_error_handler( 
                             kal_error_func_ptr fatal_handler_ptr );
/*_________________    End of Kal Initialize     _______________________*/



/*_______________________    Task Management     ________________________*/


/* This is to be called during creation phase */
kal_taskid kal_create_task(
                          kal_uint8            entity_id,
                          kal_uint8            mem_type,
                          const kal_char*      task_name_ptr,
                          kal_priority_type    priority,
                          kal_uint32           stack_size,  
                          kal_task_func_ptr    entry_func_ptr,
                          void*                entry_param_ptr,
                          kal_uint8            options );

void kal_get_my_task_index ( /*@out@*/ kal_int32* my_index );

/*  This should be called during termination phase */
void kal_delete_task( kal_taskid task_id );

void kal_sleep_task( kal_uint32 milli_secs );

kal_taskid kal_get_task_self_id( void );                

/*_________________    End of Task Management     ________________________*/


/*_____________________        ITC Management     ________________________*/


/* This routine creates a variable size queue . This should be called
 * during the Creation phase */

kal_msgqid kal_create_msg_q( 
                          kal_uint8       entity_id,
                          kal_uint8       mem_type,
                          const kal_char* queue_name_ptr,
                          kal_uint16      msg_size,
                          kal_uint16      max_msgs );


/* This should be called during the Creation phase */
kal_msgqid kal_create_fix_size_msg_q( 
                                     kal_uint8       entity_id, 
                                     kal_uint8       mem_type, 
                                     const kal_char* queue_name_ptr,
                                     kal_uint16      msg_size,
                                     kal_uint16      max_msgs );

/* This should be called during termination phase */

void kal_delete_msg_q( kal_msgqid queue_id );

/* To Enqueue message : Execution Phase */
kal_status kal_enque_msg(
                        kal_msgqid     ext_msg_q_id,
                        void*          buffer_ptr,
                        kal_uint16     msg_size,
                        kal_wait_mode  wait_mode );

/* To Enqueue message to fixed size queue : Execution Phase */
kal_status kal_enque_fix_size_msg_q(
                        kal_msgqid     ext_msg_q_id,
                        void*          buffer_ptr,
                        kal_wait_mode  wait_mode );

/* To De-queue message : Execution Phase */
kal_status kal_deque_msg(
                        kal_msgqid       ext_msg_q_id,
                        void*            buffer_ptr,
                        kal_uint16       *msg_size,
                        kal_wait_mode    wait_mode );


/* To De-queue message from fixed size queue : Execution Phase */
kal_status kal_deque_fix_size_msg_q(
                        kal_msgqid       ext_msg_q_id,
                        void*            buffer_ptr,
                        kal_wait_mode    wait_mode );

/* This routine creates a Mutex. This should be called during the
 * Creation phase */
kal_mutexid kal_create_mutex( kal_uint8 entity_id, kal_uint8 mem_type,
                              const kal_char*    mutex_name_ptr );

/* This should be called during the termination phase */
void kal_delete_mutex( kal_mutexid mutex_id  );

/* This routine takes the created mutex. This should be called during 
 * the Execution phase */
void kal_take_mutex( kal_mutexid    mutex_id );

/* This routine releases the mutex. This should be called during the 
 * Execution phase */
void kal_give_mutex( kal_mutexid    mutex_id );

/* This routine creates a Semaphore. This should be called during the
 * Creation phase */
kal_semid kal_create_sem( kal_uint8 entity_id, kal_uint8 mem_type,
                          const kal_char*  sem_name_ptr );

/* This routine creates a Semaphore with the initial value sepcified by
 * initial_count. The semaphore counts down from this value. 
 * This should be called during the
 * Creation phase */
kal_semid kal_create_count_sem( kal_uint8 entity_id, kal_uint8 mem_type,
                                const kal_char *sem_name, 
                                kal_uint32  initial_count );

/* This routine deletes the specified Sem. This should be called during the
 * termination phase */
void kal_delete_sem( kal_semid sem_id );

/* This routine captures the created sem. This should be called during the
 * Execution phase */
kal_status kal_take_sem( 
                       kal_semid        sem_id,
                       kal_wait_mode    wait_mode );

/* This routine releases the sem. This should be called during the 
 * Execution phase */
void kal_give_sem( kal_semid    sem_id );


/*_________________    End of ITC Management     ______________________*/





/*___________________    Memory Management     ________________________*/

/* This routine allocates memory from the heap.
 * This should be called during the Initialization phase */
        
/* This routine copies memory from src_ptr to dest_ptr.  
 * This should be called during the Execution phase. Semantics is same 
 * as memcpy(). */
void kal_mem_cpy( void*         dest_ptr,
                  const void*   src_ptr,
                  kal_uint32  /*@alt kal_uint8,kal_uint16,size_t@*/ size  );

/* This routine initializes the memory with given value. This should be 
 * called during the Execution phase. Semantics is same as memset() */
void kal_mem_set( void*         dest_ptr,
                  kal_uint8     value,
                  kal_uint32    size );

/* The  kal_mem_cmp()  function  compares the first n bytes of the memory
 * areas  src_ptr1 and src_ptr2. Semantics same as memcmp() */
kal_int32 kal_mem_cmp( const void*   src_ptr1,
                       const void*   src_ptr2,
                       kal_uint32 /*@alt kal_uint8,kal_uint16,size_t@*/ size );

/* This routine moves memory from src to dst. The source and destination
 * can overlap. This should be called during the Execution phase */ 
void kal_mem_move( void* dst, const void* src, 
                   kal_uint32 /*@alt kal_uint8,kal_uint16,size_t@*/ len );

/* This routine helps to extract different memory statistics */
void kal_get_all_mem_statistics( kal_all_mem_statistics mem_stats );
/*_________________    End of Memory Management _______________________*/


/*___________________    Buffer Management     ________________________*/

/* This routine creates a buffer pool. This should be called during the 
 * Creation phase */
kal_poolid kal_create_buff_pool( kal_uint8       entity_id,
                                 kal_uint8       mem_type,
                                 const kal_char* pool_name,
                                 kal_uint16      buff_size,
                                 kal_uint16      num_buffs );

/* This routine deletes the specified buffer pool. This should be called
 * during the termination phase */
void kal_delete_buff_pool( kal_poolid pool_id );

/* This routine returns the number of buffers available for allocation
 * in a buffer pool. To be called during execution phase */
kal_uint32 kal_get_num_free_buffs( kal_poolid pool_id );


/* +cr253426 */
/* This routine returns the actual size of 'buff_pt' */
kal_uint32 kal_get_buffs_size(  void *buff_pt );

/* This routine returns the pool ID of the buffer pool to which 'buff_pt'
 * belongs to quickly */
kal_poolid kal_get_buffs_pool_id(  void *buff_pt );
/* -cr253426 */

/* This routine returns the pool ID of the buffer pool to which 'usr_buff_p'
 * belongs to */
kal_poolid kal_get_poolid_from_usr_buff( void* usr_buff_p );

/*__________________        End of Buffer Management     ______________*/





/*_____________________    Timer Management     _______________________*/


/* This routine Creates a timer. This should be called during the
 * Creation phase */
kal_timerid    kal_create_timer( kal_uint8 entity_id, kal_uint8 mem_type, 
                                 const kal_char*   timer_name_ptr );

/* This routine deletes the specified timer. This should be called during 
 * the termination phase */
void    kal_delete_timer( kal_timerid   timer_id  );

/* This routine starts the timer with specified delay. This should be
 * called during the Execution phase */
void kal_set_timer( 
                  kal_timerid           timer_id,
                  kal_timer_func_ptr    handler_func_ptr,
                  void*                 handler_param_ptr,
                  kal_uint32            delay );

/* This routine cancels the created timer. This should be called during
 * the Execution phase */
void kal_cancel_timer( kal_timerid    timer_id );

/* This routine returns the time elaplsed from the system start time. This
 * should be called during the Execution phase */
void kal_get_time( kal_uint32*     secs_ptr,
                   kal_uint16*     milli_secs_ptr );

/* This routine returns the remaining time for the expiration 
 * of a given timer. */
kal_uint32 kal_get_time_remaining(kal_timerid    timer_id );

/* This routine updates the kal timer, when it runs in sync mode, if
 * it runs in free mode, this function is void function */
void kal_indicate_clk_tick(  void );
/* +CR191921 */ 
kal_uint32 kal_get_systime_tickcount ( kal_uint32 secs, kal_uint16 milli_secs);
/* -CR191921 */ 
/*___________________    End of Timer Management _______________________*/





/*_______________    Error Handling and Debug Management  ______________*/

void kal_exit( void );

void kal_abort( void );

int kal_sprintf( kal_char* str, const kal_char* fmt, ... );

/* CR85547 */
#if 0
void kal_fatal_error_handler( kal_char*         error_message_ptr,
                              kal_error_type    error_code,
                              kal_int32         os_error_code
                                                                                            );
#else
void kal_fatal_error_handler_info( kal_char*         error_message_ptr,
                              kal_error_type    error_code,
                              kal_int32         os_error_code,
                              void              *file,
                              kal_uint32        line
                              );
#endif
/* CR85547 END */

kal_char*    kal_get_env( const kal_char* str );
kal_int      kal_atoi( const kal_char* str );
long         kal_ntohl( long nw_len);
long         kal_htonl( long host_len);
short        kal_htons( short host_len );
short        kal_ntohs( short nw_len );

/*_________    End of Error Handling ________ */



/*__________________       Kal De-Initialize     ________________________*/

/* These routines are to be called during the deinitialization phase. 
 */

void kal_deinitialize( void );  

/*__________________  End of Kal De-Initialize    ________________________*/


/******************************
Declarations Of Exported Globals 
*******************************/

#endif /* _KAL_COMMON_H */
