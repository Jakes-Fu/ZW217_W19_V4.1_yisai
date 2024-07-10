/*********************************************************************

   (C) Spreadtrum Communications Co. Ltd, 
            All rights reserved.

  This file exports APIs for the RELEASE version of KAL.

  Component-specific prefix : kal

*********************************************************************/

#ifndef _KAL_RELEASE_H
#define _KAL_RELEASE_H


#if (! defined( _INCLS_FROM_KAL_INTERFACE_H ) ) 
#error "kal_release.h can't be included directly, instead include kal_interface.h for use of KAL"
#endif

/*****************
Include Statements
******************/

/************************
Macros Without Parameters 
*************************/

/*****************
Enums Without Tags 
******************/

/***************
Type Definitions 
****************/

/********
Constants 
*********/

/*********************
Macros With Parameters 
**********************/

#define kal_mem_alloc( alloc_ent_id, alloc_id, mem_type, size, file_ent_id ) \
          kal_int_mem_alloc( alloc_ent_id, alloc_id, mem_type, size )
#define kal_mem_free( ptr, file_ent_id )               kal_int_mem_free( ptr )
#define kal_realloc( alloc_ent_id, alloc_id, ptr, mem_type, size, \
                     file_ent_id ) \
          kal_int_realloc( alloc_ent_id, alloc_id, ptr, mem_type, size )

#define kal_mem_alloc_with_debuginfo( alloc_ent_id, alloc_id, mem_type, size, \
                                      file_ent_id, file_code, line) \
          kal_int_mem_alloc( alloc_ent_id, alloc_id, mem_type, size )
#define kal_mem_free_with_debuginfo( ptr, file_ent_id, file_code, line )  \
          kal_int_mem_free( ptr )
#define kal_realloc_with_debuginfo( alloc_ent_id, alloc_id, ptr, mem_type, \
                                    size, file_ent_id, file_code, line )\
          kal_int_realloc( alloc_ent_id, alloc_id, ptr, mem_type, size )

#define kal_get_buffer( pool_id, alloc_ent_id, allocator_id, size, \
                        num_free_buffs_p, file_ent_id )\
        kal_int_get_buffer( pool_id, size, num_free_buffs_p)

#define kal_release_buffer( buff_ptr, file_ent_id )  \
        kal_int_release_buffer( buff_ptr )

#define kal_get_buffer_with_debuginfo( pool_id, alloc_ent_id, allocator_id, \
                                       size, num_free_buffs_p, \
                                       file_ent_id, file_code, line )     \
         kal_int_get_buffer( pool_id, size, num_free_buffs_p)

#define kal_release_buffer_with_debuginfo( buff_ptr, file_ent_id, file_code, \
                                           line ) \
         kal_int_release_buffer( buff_ptr )


/*----------  The following APIs map to NULL -------------------------*/
/*----------- in the RELEASE version of KAL  -------------------------*/

/*_______________________    Task Management     ________________________*/

/* This routine puts the calling task to sleep. This should be called 
 * during the execution phase 
 */
#define kal_get_task_statistics(task_id, stat_ptr) {}
#define kal_get_all_task_statistics( stat_ptr) {}
#define kal_print_task_statistics(task_id ) {}
    
/*_______________________    End of Task Management _______________________*/

/*______________________        ITC Management     ________________________*/

#define kal_get_q_statistics(queue_id, stat_ptr ) {}
#define kal_get_all_q_statistics( stat_ptr ) {}
#define kal_print_q_statistics(  queue_id ) {}
#define kal_reset_q_statistics(  queue_id ) {}
#define kal_get_all_mutex_statistics( all_mutex_stat_p ) {}
#define kal_get_all_sem_statistics( all_sem_stat_p ) {}

/*______________________    End of ITC Management     _______________________*/

/*_______________________    Buffer Management     ________________________*/

#define kal_get_pool_statistics(  pool_id, stat_ptr)  {}
#define kal_get_all_buffer_statistics(  pool_id, buff_stat_ptr)  {}
#define kal_print_pool_statistics( pool_id ) {}
#define kal_reset_pool_statistics(  pool_id) {}
#define kal_is_valid_buffer( buff_ptr, status_ptr ) {*status_ptr=KAL_TRUE;}
#define kal_print_buff_leaks() {}
#define kal_print_buff_leaks_by_alloc_id( entity_id, alloc_id ) {}
#define kal_print_num_allocated_buffers_by_alloc_id( entity_id, alloc_id ) {}
#define kal_print_module_wise_allocation( pool_id) {}
#define kal_get_buffer_statistics( buffer_ptr, stat_ptr ) {}
#define kal_print_buffer_statistics( buffer_ptr ) {}
#define kal_reset_buffer_statistics( buffer_ptr ) {}
#define kal_get_pool_index(pool_id,is_valid_poolid,pool_index_p) {}
#define kal_get_all_pool_statistics( all_pool_stat ) {}
#define kal_buff_trace_start( size ) {}
#define kal_buff_trace_end( file ) {}
#define kal_set_pool_subgroup(entity_id, mem_type, pool_id,lower_limit,\
                              upper_limit, logtobase2_of_range,status_ptr )   {}
#define kal_reset_pool_subgroup( pool_id )   {}
#define kal_print_pool_subgroup_statistics( pool_id )  {}
#define kal_initialize_buff_history( entity_id, mem_type, pool_id ,\
                                     status_ptr ) {}
#define kal_reset_buff_history( pool_id ) {}
#define kal_set_trans_log( ) {}
#define kal_reset_trans_log( ) {}
#define kal_start_trans_log_for_pool( pool_id ) {}
#define kal_end_trans_log_for_pool( pool_id )   {}
#define kal_mark_scenario_end( ) {}
#define kal_init_allocation_over( ) {}

/*_____________________        End of Buffer Management     __________________*/

/*________________________    Timer Management     __________________________*/

#define kal_get_timer_statistics(  timer_id, stat_ptr )  {}
#define kal_print_timer_statistics( timer_id ) {}
#define kal_reset_timer_statistics(timer_id ) {}
#define kal_get_all_timer_statistics( all_timer_stat_p ) {}

/*____________________    End of Timer Management______________________*/


/*_________________    Error and Debug Management     __________________*/

#define kal_register_print_string_function( to_be_registered_func) {}
#define kal_print_int( int_to_be_printed )  {}
#define kal_print_uint( int_to_be_printed )  {}
#define kal_print_uint_in_hex( int_to_be_printed )  {}
#define kal_reset() {}
#define kal_print( string_to_be_printed )  {}

/*_______________   End of Error and Debug Management   ________________*/


/*_________________ Memory Management ________________________*/

#define kal_print_mem_stats() {}
#define kal_print_mem_leaks() {}

/*_______________    End of Memory Management     ________________*/

/***************************
Exported Function Prototypes 
****************************/


/*___________________    Memory Management     ________________________*/

/* This routine allocates memory from the heap.
 * This should be called during the Initialization phase */

void * kal_int_mem_alloc( kal_uint8 alloc_ent_id, kal_uint8  alloc_id, 
                          kal_uint8 mem_type,  kal_uint32 size );
void * kal_int_realloc( kal_uint8 alloc_ent_id, kal_uint8 alloc_id, 
                        void* old_ptr, kal_uint8 mem_type,  
                        kal_uint32  new_size );

/* This routine de-allocates the memory. This could be called during
 * termination phase */
void kal_int_mem_free( void* mem_ptr );

/*_________________    End of Memory Management _______________________*/


/*___________________    Buffer Management     ________________________*/

void * kal_int_get_buffer( kal_poolid     pool_id,
                           kal_uint16     size,
                           kal_uint32*    num_free_buffs_p
                            );
kal_uint32 kal_int_release_buffer( void*  buff_ptr );

/*__________________        End of Buffer Management     ______________*/


typedef struct kal_context_struct{
    kal_uint32             kal_tmr_scaler;
    kal_uint16              kal_num_task_created;      /* cr229061 */
    kal_uint16              kal_num_buff_pools_created;/* cr229061 */
    kal_uint16             padding;                   /* padding for align 4 */
    kal_pool_info_struct*  kal_pool_info;
    kal_mem_stats_struct** kal_int_malloc_stats;      /*  internal to kal */
    kal_mem_stats_struct** kal_ext_malloc_stats;      /*  external to kal */
    kal_mem_stats_struct** kal_mem_type_malloc_stats; /*  per mem_type */
    kal_uint32             kal_total_mem_allocated;
}kal_context_struct;

/******************************
Declarations Of Exported Globals 
*******************************/
extern kal_context_struct kal_context_g;

#endif /* _KAL_RELEASE_H */
