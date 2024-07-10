/*********************************************************************

   (C) Spreadtrum Communications Co. Ltd, 
            All rights reserved.

   This file contains definitions and prototypes for Debug version 
   of KAL that are common across different OS targets supported.

   Component-specific prefix : kal

*********************************************************************/

#ifndef _KAL_DEBUG_COMMON_DEFS_H
#define _KAL_DEBUG_COMMON_DEFS_H

/*****************
Include Statements
******************/

#include "kal_interface.h"

/************************
Macros Without Parameters 
*************************/

/* Buffer size is of type kal_uint16. So user can request only for 
 * buffer size which when added with the buffer overhead will not cause
 * overflow.Thus KAL_MAX_BUFF_SIZE is set. */ 
#define KAL_MAX_BUFF_SIZE         ((kal_uint16)(~0)- \
                                        (KAL_TOTAL_BUFF_OVERHEAD + 1))
  
/*****************
Enums Without Tags 
******************/

/***************
Type Definitions 
****************/

typedef struct 
{
  kal_internal_msgqid           queue_id;
}kal_qinfo_struct;


/********
Constants 
*********/

/*********************
Macros With Parameters 
**********************/

/***************************
Exported Function Prototypes 
****************************/

extern void 
kal_debug_uint_2_hex_str( kal_uint32 num, kal_char* string );

extern void
kal_debug_uint_2_str( kal_uint32 num, kal_char* string );

extern kal_bool
kal_debug_validate_poolid( kal_internal_poolid pool_id,
                           kal_uint32*         pool_index_p );

extern kal_bool
kal_debug_is_mem_over_run( const void* mem_ptr, kal_uint32 size );

extern kal_bool
kal_debug_is_buff_allocated ( kal_internal_poolid pool_id,
                              void                *usr_buff_ptr );

extern
void
kal_debug_update_buff_alloc_history ( kal_buff_hdr_struct* buff_hdr_p, 
                                      kal_uint16           size,
                                      kal_uint16           alloc_file, 
                                      kal_uint16           alloc_line, 
                                      kal_uint8            alloc_id,
                                      kal_uint8            alloc_ent_id,
                                      kal_uint8            file_ent_id );

extern
void
kal_debug_update_buff_free_history ( kal_uint8            file_ent_id,
                                     kal_buff_hdr_struct* buff_hdr_p,
                                     kal_uint16           dealloc_file, 
                                     kal_uint16           dealloc_line );

extern
void 
kal_debug_check_mem_overlap( void* ptr1, const void* ptr2, kal_uint32 size );

extern void* kal_internal_stat_alloc( kal_uint8 alloc_ent_id, 
                                      kal_uint8 alloc_id, kal_uint8 mem_type, 
                                      kal_uint32 size ); 
extern void  kal_internal_stat_free( void *mem_ptr );

extern void kal_debug_store_trans_rec( void*          ptr,
                                       kal_buff_state trans_type,
                                       kal_uint8      alloc_ent_id,
                                       kal_uint8      alloc_id,
                                       kal_uint16     size,
                                       kal_uint8      file_ent_id,
                                       kal_uint16     file_code,
                                       kal_uint16     line_no
                                      );

/*+cr231817*/
extern kal_uint32
kal_debug_get_pool_buff_max_alloced ( kal_poolid ext_pool_id );
/*-cr231817*/

/******************************
Declarations Of Exported Globals 
*******************************/

extern kal_uint16                  kal_max_num_queues_g;/* +cr229061 */
extern kal_uint16                 kal_max_num_sems_g;
extern kal_uint16                 kal_max_num_mutexs_g;

extern kal_qinfo_struct*          kal_queue_info_g;
extern kal_uint16                  kal_num_queues_created_g; /* +cr229061 */                 
extern kal_uint16                 kal_num_sems_created_g;                  
extern kal_uint16                 kal_num_mutex_created_g;                  

extern kal_pool_statistics*       kal_all_pool_stat_g;
extern kal_sem_statistics*        kal_all_sem_stat_g;
extern kal_mutex_statistics*      kal_all_mutex_stat_g;
extern kal_timer_statistics*      kal_all_timer_stat_g;
extern kal_queue_statistics*      kal_all_queue_stat_g;
extern kal_task_statistics*       kal_all_task_stat_g;

/*
 * kal_trans_arr_size_g is the number of transaction records 
 * that can be stored in the transaction log array without 
 * overwritting the existing record.
 */

extern kal_uint16                 kal_trans_arr_size_g;

#endif /* _KAL_DEBUG_COMMON_DEFS_H */
