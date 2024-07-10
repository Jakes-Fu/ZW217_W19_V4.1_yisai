/*********************************************************************

   (C) Spreadtrum Communications Co. Ltd, 
            All rights reserved.

*   This file contains the type definitions specific to 
*   release version of nucleus port.

*   Component-specific prefix : kal

*********************************************************************/

#ifndef _KAL_THREADX_H
#define _KAL_THREADX_H

/*****************
Include Statements
******************/

#include <stddef.h>
#include "kal_threadx_common_defs.h"

/************************
Macros Without Parameters 
*************************/


/* Defines the buffer header size of KAL */
#define KAL_BUFF_HEADER_SIZE           offsetof(kal_buff_hdr_struct, usr_buff )

/* Defines the overhead for each nucleus partition */
/* @rick.zhang 2005-07-06 */
#if 0
#define KAL_NU_BUFF_OVERHEAD           sizeof( PM_HEADER )
#else
#define KAL_TX_BUFF_OVERHEAD           sizeof( RAL_BLOCKPOOL_HEADER )
#endif

/* Defines the KAL Buffer over head. Since there is no buffer footer in the
 * release version. KAL_BUFFER_OVER_HEAD equals the overhead for the
 * KAL buffer header */

#if defined(CORRUPTION_CHECK)
#define KAL_FOOT_PRINT_SIZE            4
#define KAL_BUFF_FOOTER_SIZE           KAL_FOOT_PRINT_SIZE
#define KAL_BUFFER_OVERHEAD            ( KAL_BUFF_HEADER_SIZE \
                                         + KAL_BUFF_FOOTER_SIZE )
#else
#define KAL_BUFFER_OVERHEAD            KAL_BUFF_HEADER_SIZE
#endif

/* Defines the total buffer over head. This is equal to the nucleus buffer
 * over-head + KAL buffer over head */
/* @rick.zhang 2005-07-06 */
#if 0
#define KAL_TOTAL_BUFF_OVERHEAD        ( KAL_NU_BUFF_OVERHEAD    \
                                           + KAL_BUFFER_OVERHEAD )
#else
#define KAL_TOTAL_BUFF_OVERHEAD        ( KAL_TX_BUFF_OVERHEAD    \
                                           + KAL_BUFFER_OVERHEAD )

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
 *                data elements. */
typedef struct kal_internal_msgqid_struct
{
    kal_os_queue_type    queue_id; 
    void*                mem_add;
    kal_uint16           max_msg_size;
}* kal_internal_msgqid;

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
 * mutex_state  : Indicates whether the mutex is given or taken */
typedef struct kal_internal_mutexid_struct
{
    kal_os_mutex_type    mutex_id;
    kal_os_task_type     *owner_task;
    kal_mutex_state      mutex_state;
  #if defined ( LOG_MEM_HISTORY )
    kal_mutex_statistics   mutex_stat;
  #endif	
}*kal_internal_mutexid;


typedef struct kal_internal_semid_struct
{
  kal_os_sem_type sem_id;
}*kal_internal_semid;

/* timer_id        : The field for holding Nucleus timer control data
 *                   structure. While creating the timer, the 
 *                   address of this field is passed to nucleus. The address
 *                   of this field is subsequently used for any timer
 *                   operations.
 * func_ptr        : Pointer to the function that gets called on timer
 *                  expiry from timer handler.
 * timer_param_ptr : The parameter passed to the function pointed by
 *                   'func_ptr'.
 */
typedef struct kal_internal_timerid_struct
{ 
    kal_os_timer_type     timer_id;
    kal_timer_func_ptr    func_ptr;
    void*                 timer_param_ptr;
} * kal_internal_timerid;


/* pool_id         : The field for holding Nucleus Partition Pool Control 
 *                   data structure. While creating the pool, the 
 *                   address of this field is passed to nucleus. The address
 *                   of this field is subsequently used for any pool
 *                   operations.
 * mem_addr        : The memory allocated for the partition pool memory.
 *                   This address is maintained to free the memory at
 *                   time of pool deletion.
 */

typedef struct kal_internal_poolid_struct
{
    kal_os_pool_type    pool_id;
    void *              mem_add;
#if defined(CORRUPTION_CHECK)
    kal_uint16          buff_size;
    kal_uint16          pool_index;
    kal_pool_statistics pool_stat;
    //kal_uint32          buff_max_alloced;
#endif
}*kal_internal_poolid;

typedef struct kal_internal_taskid_struct
{
  kal_os_task_type task_id;
  void*            stack_addr;
}* kal_internal_taskid;

typedef struct kal_internal_mem_poolid_struct
{
     kal_os_mem_pool_type * mem_poolid;
     kal_mem_pool_info *    mem_poolid_info;
} *kal_internal_mem_poolid;     


#if (!defined(CORRUPTION_CHECK))
/*******************************************************************
  Pointer to               Threadx partition & User buffer
  TX_HEADER                starts here  
  |                             |
  V                             V
   -------------------------------------------------
  |  Threadx over head          |                   |
  |                             |                   |
   -------------------------------------------------
********************************************************************/
typedef struct kal_buff_hdr_tag{
    kal_uint8 usr_buff[1];
} kal_buff_hdr_struct;

#else
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
typedef struct kal_buff_hdr_tag{
    //struct kal_buff_hdr_tag * pre;
    //struct kal_buff_hdr_tag * succ;
    //kal_uint16              file_code;
    //kal_uint16              line;
    //unsigned long           size;
    //unsigned long           block_num;
    kal_uint32              size;
    kal_uint32              hdr_stamp;
    kal_internal_poolid     pool_id;
    kal_uint8               usr_buff[1];
} kal_buff_hdr_struct;

#endif

/********
Constants 
*********/

/*********************
Macros With Parameters 
**********************/

/***************************
Exported Function Prototypes 
****************************/
extern void *kal_int_allocate_mem ( kal_uint8 alloc_ent_id, 
                                    kal_uint8 alloc_id, 
                                    kal_uint8 mem_type,  kal_uint32 size,
                                    kal_bool  is_kal_int_alloc );


extern void kal_debug_print_queue_info(void);
/******************************
Declarations Of Exported Globals 
*******************************/
extern kal_internal_taskid               kal_task_ptrs_g;

#if defined (CORRUPTION_CHECK)
//extern kal_buff_hdr_struct kal_buff_alloc_head_g;
#endif

#endif /*_KAL_NUCLEUS_H*/
