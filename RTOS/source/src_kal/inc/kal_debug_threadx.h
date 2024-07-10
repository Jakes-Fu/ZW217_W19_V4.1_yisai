/*********************************************************************

   (C) Spreadtrum Communications Co. Ltd, 
            All rights reserved.

*  This file contains the definitions and function prototypes,
*  that are specific to debug version of Nucleus port.

*   Component-specific prefix : kal

*********************************************************************/

#ifndef _KAL_DEBUG_THREADX_H
#define _KAL_DEBUG_THREADX_H

/*****************
Include Statements
******************/

#include <stddef.h>
#include "kal_threadx_common_defs.h"

/************************
Macros Without Parameters 
*************************/

/* Defines the buffer header size of KAL */
#define KAL_BUFF_HEADER_SIZE     offsetof(kal_buff_hdr_struct, usr_buff )

/* The foot print of KAL puts 4 bytes of magic numbers */
#define KAL_FOOT_PRINT_SIZE      4 
#define KAL_BUFF_FOOTER_SIZE     KAL_FOOT_PRINT_SIZE

/* Defines the overhead for each nucleus partition */ 
/* @rick.zhang 2005-07-06 */
#if 0
#define KAL_NU_BUFF_OVERHEAD     sizeof( PM_HEADER )
#else
#define KAL_TX_BUFF_OVERHEAD     sizeof( RAL_BLOCKPOOL_HEADER )
#endif

/* Defines the KAL Buffer over head. There is a buffer foot print
 * maintained in debug KAL to check for overreads and over writes.
 * So KAL_BUFFER_OVER_HEAD equals the overhead for 
 * the KAL buffer header and KAL footer. 
 */ 
#define KAL_BUFFER_OVERHEAD      (KAL_BUFF_HEADER_SIZE\
                                    + KAL_BUFF_FOOTER_SIZE)
/* Defines the total buffer over head. This is equal to the nucleus buffer
 * over-head + KAL buffer over head 
 */
/* @rick.zhang 2005-07-06 */
#if 0
#define KAL_TOTAL_BUFF_OVERHEAD  (KAL_NU_BUFF_OVERHEAD\
                                    +KAL_BUFFER_OVERHEAD)
#else
#define KAL_TOTAL_BUFF_OVERHEAD  (KAL_TX_BUFF_OVERHEAD\
                                    +KAL_BUFFER_OVERHEAD)
#endif

/*****************
Enums Without Tags 
******************/

/***************
Type Definitions 
****************/

/* queue_id     : The field for holding Nucleus Queue Control Data structure.
 *                While creating the queue the
 *                address of this field is passed to nucleus. The
 *                address of this field is subsequently used as
 *                the references to the Queue for any queue operations.
 * mem_add      : The pointer to memory location allocated for
 *                holding queue elements.
 * max_msg_size : The size of the maximum message in terms of unsigned
 *                data elements.
 * q_type       : Decides whether this is a fixed or variable length Q. 
 * q_stat       : Pointer to the memory location that holds Q statistics 
 */
typedef struct kal_internal_msgqid_struct
{
     kal_os_queue_type       queue_id;
     kal_uint16              max_msg_size;
     void *                  mem_add;
     kal_qtype               q_type;
     kal_queue_statistics    q_stat;
}* kal_internal_msgqid;

/* sem_id       : The field for holding Nucleus Semaphore control Data
 *                structure. While creating the semaphore, the address of
 *                this field is passed to nucleus. The address of this
 *                field is used as the reference to the semaphrore in
 *                subsequent semaphore operations.
 * sem_stat     : Pointer to the memory location that holds semaphore
 *                statistics. 
 */
typedef struct kal_internal_semid_struct
{
     kal_os_sem_type       sem_id;
     kal_sem_statistics    sem_stat;
}*kal_internal_semid;

/* mutex_id     : The field for holding Nucleus mutex Control
 *                data structure. While creating the mutex, the
 *                address of this field is passed to nucleus. The address
 *                of this field is subsequently used for any mutex
 *                operations.
 * owner_task   : Holds information about the task that has taken the
 *                mutex. According to POSIX standards the task that
 *                has taken the mutex can only release the mutex. This
 *                field is maintained to do this checking over mutex
 *                semaphores.
 * mutex_state  : Indicates whether the mutex is given or taken
 * mutex_stat   : Pointer to the memory location for holding mutex
 *                statistics 
 */
  
typedef struct kal_internal_mutexid_struct
{
     kal_os_mutex_type       mutex_id;
     kal_os_task_type        *owner_task;
     kal_mutex_state         mutex_state;
     kal_mutex_statistics    mutex_stat;
}*kal_internal_mutexid;

/* pool_id          : The field for holding Nucleus Partition Pool Control
 *                    data structure. While creating the pool, the
 *                    address of this field is passed to nucleus. The address
 *                    of this field is subsequently used for any pool
 *                    operations.
 * buff_size        : The size of the user buffer area in this pool. This
 *                    is equal to nucleus partition size - nucleus buffer
 *                    overhead.
 * num_buffs        : Number of buffers in the partition pool.
 * mem_addr         : The memory allocated for the partition pool memory.
 *                    This address is maintained to free the memory at
 *                    time of pool deletion.
 * pool_stat        : Pointer to the memory location that holds pool
 *                    statistics.
 * protecting_mutex : The mutex for protecting the pool data structures
 */
typedef struct kal_internal_poolid_struct
{
     kal_os_pool_type       pool_id;
     kal_uint16             buff_size;
     kal_uint16             num_buffs;
     kal_uint16             pool_index;
     kal_uint16             padding;        /* padding with pool_index */
     void *                 mem_add;
     kal_pool_statistics    pool_stat;
     kal_mutexid            protecting_mutex;
     kal_uint32             buff_max_alloced;
}*kal_internal_poolid;

typedef struct kal_internal_timerid_struct
{
     kal_os_timer_type       timer_id;
     kal_timer_func_ptr      func_ptr;
     void *                  timer_param_ptr;
     kal_timer_statistics    timer_stat;
}*kal_internal_timerid;

typedef struct kal_internal_taskid_struct
{
     kal_os_task_type       task_id;
     void*                  stack_addr;
     kal_task_statistics    task_stat;
} *kal_internal_taskid;

typedef struct kal_internal_mem_poolid_struct
{
     kal_os_mem_pool_type *  mem_poolid;
     kal_mem_pool_info *     mem_poolid_info;
} *kal_internal_mem_poolid;     


/*******************************************************************
  Pointer to   Threadx                     User buffer         User buffer
  TX_HEADER    partition                   starts              ends
  |            | starts                    |                   |
  V            V                           V                   V
   --------------------------------------------------------------------
  |  Threadx   |    Buff_tag    |HDR  |Pool|                   |FOOTER |
  |  over head |(Log alloc info)|STAMP|ID  |                   |STAMP  |
   --------------------------------------------------------------------
               ^                                                       ^
               | <----              Allocated size                ---->|
********************************************************************/

    /* In Debug version of KAL the buffer would look like this
     *  ---------------------
     * |  Buffer alloc info  | (24 byte)
     *  ---------------------
     * |  header bit Pattern | (4 byte)
     *  ---------------------
     * |  Owner Pool_id      | (4 " )
     *  ---------------------
     * |  Buffer             | ( buff_size bytes )
     *  ---------------------
     * |  Footer bit Pattern | (4 " )
     *  ---------------------
     *
     */
/*
reference:
byte_heap_hdr_struct in threadx_mem.c
MEM_HEADER_T in os_api.h
*/     
typedef struct kal_buff_hdr_tag{
    struct kal_buff_hdr_tag * pre;
    struct kal_buff_hdr_tag * succ;
/* be used in SCI_DumpAllocatedMemInfo */    
    kal_uint16              file_code;//alloced file
    kal_uint16              line;//alloced line
/* +CR170784 */ 
/* add two fields to log the file and line where buff is freed */
    kal_uint16              free_file_code;
    kal_uint16              free_line;
/* -CR170784 */     
    kal_uint16              size;
    kal_uint16              block_num;    
    kal_uint8               alloc_ent_id;
    kal_uint8               alloc_id;
    kal_uint8               file_ent_id;
    kal_uint8               padding;   
    kal_uint32              hdr_stamp;
    kal_internal_poolid     pool_id;
    kal_uint8               usr_buff[1];
} kal_buff_hdr_struct;


typedef struct
{
     kal_internal_timerid   timer_id;
} kal_timer_rec_struct;

typedef struct
{
     kal_timer_rec_struct *pool;
     kal_uint32    num_timers;
} kal_timer_pool_struct;

/********
Constants 
*********/

/*********************
Macros With Parameters 
**********************/

/***************************
Exported Function Prototypes 
****************************/

extern kal_char* kal_debug_str_dup( kal_char *string );
 
extern void kal_debug_free_str_dup( kal_char* string );
 
extern void kal_debug_reset_pool( kal_internal_poolid pool_id );
 
extern void kal_debug_reset_timer( kal_uint16 ctr );
 
extern void kal_debug_flush_msg_queue( kal_msgqid ext_msg_q_id );

extern void * kal_int_allocate_mem( kal_uint8  alloc_ent_id, 
                                    kal_uint8  alloc_id,
                                    kal_uint8  mem_type,
		                            kal_uint32 size, 
                                    kal_bool   is_kal_int_alloc,
                                    kal_uint8  file_ent_id,
								    kal_uint16 file_code, 
								    kal_uint16 line);
 
extern void kal_debug_print_queue_info(void);




/******************************
Declarations Of Exported Globals 
*******************************/

extern kal_timer_pool_struct             kal_timer_info_g;
extern kal_uint16                        kal_num_timers_created_g;
/* @rick.zhang 2005-07-07 */
#if 0
extern NU_TASK*                          kal_sys_init_task_ptr_g;
#else
extern kal_os_task_type*                 kal_sys_init_task_ptr_g;
#endif
extern kal_internal_taskid               kal_task_ptrs_g;

extern kal_buff_hdr_struct kal_buff_alloc_head_g;

#endif /*_KAL_DEBUG_THREADX_H*/
