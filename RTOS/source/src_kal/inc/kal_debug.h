/*********************************************************************
   (C) Spreadtrum Communications Co. Ltd, 
            All rights reserved.
  
   This file exports APIs for the DEBUG version of
   KAL.

   Component-specific prefix : kal
*********************************************************************/

#ifndef _KAL_DEBUG_H
#define _KAL_DEBUG_H

#if (! defined( _INCLS_FROM_KAL_INTERFACE_H ) ) 
#error "kal_debug.h can't be included directly, instead include kal_interface.h for use of KAL"
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

/*_______________________    Task Management     ________________________*/
#define kal_get_task_statistics(task_id, stat_ptr )\
kal_debug_get_task_statistics(task_id, stat_ptr )
    
#define kal_get_all_task_statistics( stat_ptr )\
kal_debug_get_all_task_statistics( stat_ptr )

#define kal_print_task_statistics(task_id )\
kal_debug_print_task_statistics(task_id )
   
/*_______________________    End of Task Management _______________________*/

/*______________________        ITC Management     ________________________*/

#define kal_get_q_statistics(queue_id, stat_ptr )\
kal_debug_get_q_statistics(queue_id, stat_ptr )

#define kal_get_all_q_statistics( stat_ptr )\
kal_debug_get_all_q_statistics( stat_ptr )

#define kal_print_q_statistics(  queue_id )\
kal_debug_print_q_statistics(  queue_id )

#define kal_reset_q_statistics(  queue_id )\
kal_debug_reset_q_statistics(  queue_id )

#define kal_get_all_mutex_statistics( all_mutex_stat_p )\
  kal_debug_get_all_mutex_statistics( all_mutex_stat_p )

#define kal_get_all_sem_statistics( all_sem_stat_p )\
  kal_debug_get_all_sem_statistics( all_sem_stat_p )
/*______________________    End of ITC Management     _______________________*/

/*_______________________    Buffer Management     ________________________*/


#define kal_get_buffer( pool_id, alloc_ent_id, allocator_id, size, \
                        num_free_buffs_p, file_ent_id ) \
        kal_int_get_buffer( pool_id, alloc_ent_id, allocator_id, size, \
                            num_free_buffs_p, file_ent_id, \
                            _FILE_CODE_, __LINE__)

#define kal_release_buffer( buff_ptr, file_ent_id )     \
        kal_int_release_buffer( buff_ptr, file_ent_id, _FILE_CODE_, __LINE__)

#define kal_get_buffer_with_debuginfo( pool_id, alloc_ent_id, allocator_id,\
                                       size, num_free_buffs_p, file_ent_id, \
                                       file_code, line )     \
         kal_int_get_buffer( pool_id, alloc_ent_id, allocator_id, size, \
                             num_free_buffs_p, file_ent_id, file_code, line)

#define kal_release_buffer_with_debuginfo( buff_ptr, file_ent_id, file_code,\
                                           line ) \
        kal_int_release_buffer(  buff_ptr, file_ent_id, file_code, line)

#define kal_get_pool_statistics(  pool_id, stat_ptr) \
kal_debug_get_pool_statistics(  pool_id, stat_ptr) 

#define kal_get_all_buffer_statistics( pool_id, buff_stat_ptr) \
kal_debug_get_all_buff_statistics( pool_id , buff_stat_ptr)

#define kal_print_pool_statistics( pool_id )\
kal_debug_print_pool_statistics( pool_id )

#define kal_reset_pool_statistics(  pool_id)\
kal_debug_reset_pool_statistics(  pool_id)

#define kal_is_valid_buffer( buff_ptr, status_ptr )\
kal_debug_is_valid_buffer( buff_ptr, status_ptr )

#define kal_print_buff_leaks()\
kal_debug_print_buff_leaks()

#define kal_print_buff_leaks_by_alloc_id( entity_id, alloc_id )\
kal_debug_print_buff_leaks_by_alloc_id( entity_id, alloc_id )

#define kal_print_num_allocated_buffers_by_alloc_id( entity_id, alloc_id )\
kal_debug_print_num_allocated_buffers_by_alloc_id( entity_id, alloc_id )

#define kal_print_module_wise_allocation( pool_id) \
kal_debug_print_module_wise_allocation( pool_id )

#define kal_get_buffer_statistics( buffer_ptr, stat_ptr )\
kal_debug_get_buffer_statistics( buffer_ptr, stat_ptr )

#define kal_print_buffer_statistics( buffer_ptr )\
kal_debug_print_buffer_statistics( buffer_ptr )

#define kal_reset_buffer_statistics( buffer_ptr )\
kal_debug_reset_buffer_statistics( buffer_ptr )

#define kal_get_pool_index(pool_id,is_valid_poolid,pool_index_p)\
is_valid_poolid = kal_debug_get_pool_index( pool_id,pool_index_p )

#define kal_get_all_pool_statistics( all_pool_stat )\
  kal_debug_get_all_pool_statistics( all_pool_stat )

#define kal_buff_trace_start( size )\
  kal_debug_buff_trace_start( size )
  
#define kal_buff_trace_end( file )\
  kal_debug_buff_trace_end( file )

/* The status_ptr is a pointer to a variable of type kal_uint32.
 * If function call is success, the status_ptr content is 0 ,else
 * (*status_ptr) == KAL_ERROR_DEBUGBUFFMNGR_ALLOC_MEM_FOR_STAT_FAILED.
 */
  
#define kal_set_pool_subgroup( entity_id, mem_type, pool_id,lower_limit,\
                               upper_limit, sub_grp_sz, status_ptr)\
*status_ptr = kal_debug_set_pool_subgroup( entity_id, mem_type, \
                                           pool_id,lower_limit,\
                                           upper_limit,sub_grp_sz)

#define kal_reset_pool_subgroup( pool_id ) \
  kal_debug_reset_pool_subgroup( pool_id )

#define kal_print_pool_subgroup_statistics( pool_id ) \
  kal_debug_print_pool_subgroup_statistics ( pool_id )

/* The status_ptr is a pointer to a variable of type kal_uint32.
 * If function call is success, the status_ptr content is 0 ,else
 * (*status_ptr) == KAL_ERROR_DEBUGBUFFMNGR_ALLOC_MEM_FOR_STAT_FAILED.
 */
  
#define kal_initialize_buff_history( entity_id, mem_type, pool_id , \
                                     status_ptr ) \
*status_ptr = kal_debug_initialize_buff_history( entity_id, mem_type, pool_id )
  
#define kal_reset_buff_history( pool_id ) \
  kal_debug_reset_buff_history( pool_id )
  
#define kal_set_trans_log( ) \
  kal_debug_set_trans_log( )

#define kal_reset_trans_log( ) \
  kal_debug_reset_trans_log( )

#define kal_start_trans_log_for_pool( pool_id ) \
  kal_debug_start_trans_log_for_pool( pool_id )
  
#define kal_end_trans_log_for_pool( pool_id ) \
  kal_debug_end_trans_log_for_pool( pool_id )

#define kal_mark_scenario_end(  ) \
  kal_debug_mark_scenario_end(  )

#define kal_init_allocation_over(  ) \
  kal_debug_init_allocation_over(  )	
  
/*_____________________        End of Buffer Management     __________________*/

/*________________________    Timer Management     __________________________*/

#define kal_get_timer_statistics(  timer_id, stat_ptr ) \
kal_debug_get_timer_statistics(  timer_id, stat_ptr ) 

#define kal_print_timer_statistics( timer_id )\
kal_debug_print_timer_statistics( timer_id )

#define kal_reset_timer_statistics(timer_id )\
kal_debug_reset_timer_statistics(timer_id )

#define kal_get_all_timer_statistics( all_timer_stat_p )\
kal_debug_get_all_timer_statistics( all_timer_stat_p )


/*____________________    End of Timer Management______________________*/


/*_________________    Error and Debug Management     __________________*/

#define kal_register_print_string_function( to_be_registered_func)\
kal_debug_register_print_string_function( to_be_registered_func)

#define kal_print_int( int_to_be_printed )\
kal_debug_print_int( int_to_be_printed )

#define kal_print_uint( int_to_be_printed )\
kal_debug_print_uint( int_to_be_printed )

#define kal_print_uint_in_hex( int_to_be_printed )\
kal_debug_print_uint_in_hex( int_to_be_printed )

#define kal_reset() \
kal_debug_reset()


#define kal_print( string_to_be_printed )\
   kal_debug_print( string_to_be_printed )
/*_______________   End of Error and Debug Management   ________________*/


/*_________________ Memory Management ________________________*/

 #define kal_mem_alloc( alloc_ent_id, alloc_id, mem_type, size, file_ent_id )  \
          kal_int_mem_alloc( alloc_ent_id, alloc_id, mem_type, size,\
                             file_ent_id, _FILE_CODE_, __LINE__ )

#define kal_mem_free( ptr, file_ent_id ) \
         kal_int_mem_free( ptr, file_ent_id, _FILE_CODE_, __LINE__ )

#define kal_realloc( alloc_ent_id, alloc_id, ptr, mem_type, size, file_ent_id )\
        kal_int_realloc( alloc_ent_id, alloc_id, ptr, mem_type, size, \
                         file_ent_id, _FILE_CODE_, __LINE__ )

#define kal_mem_alloc_with_debuginfo( alloc_ent_id, alloc_id, mem_type, size, \
                                      file_ent_id, file_code, line) \
          kal_int_mem_alloc( alloc_ent_id, alloc_id, mem_type, size, \
                             file_ent_id, file_code, line )

#define kal_mem_free_with_debuginfo( ptr, file_ent_id, file_code, line )  \
          kal_int_mem_free( ptr, file_ent_id, file_code, line )

#define kal_realloc_with_debuginfo( alloc_ent_id, alloc_id, ptr, mem_type, \
                                    size, file_ent_id, file_code, line ) \
          kal_int_realloc( alloc_ent_id, alloc_id, ptr, mem_type, size, \
                           file_ent_id, file_code, line )

#define kal_print_mem_stats()\
  kal_debug_print_mem_stats()

#define kal_print_mem_leaks()\
  kal_debug_print_mem_leaks()
/*_______________    End of Memory Management     ________________*/

/***************************
Exported Function Prototypes 
****************************/

/*_______________________    Task Management     ________________________*/


/* This routine puts the calling task to sleep. This should be called
 * during the Execution phase */

void kal_debug_sleep_task( kal_uint32    time_in_milli_secs );

void kal_debug_get_task_statistics(kal_taskid           task_id,
                                   kal_task_statistics* stat_ptr );

void kal_debug_print_task_statistics(kal_taskid  task_id );

void
kal_debug_get_all_task_statistics( kal_all_task_statistics all_stat_p );  


/*________________    End of Task Management  ______________________*/


/*___________________________        ITC Management     ________________________*/

void kal_debug_get_q_statistics( kal_msgqid            queue_id,
                                 kal_queue_statistics* stat_ptr );

void kal_debug_print_q_statistics( kal_msgqid queue_id );

void kal_debug_reset_q_statistics( kal_msgqid    queue_id );

void
kal_debug_get_all_mutex_statistics ( kal_all_mutex_statistics all_mutex_stat );

void
kal_debug_get_all_sem_statistics ( kal_all_sem_statistics all_sem_stat );      

void
kal_debug_get_all_q_statistics ( kal_all_queue_statistics all_q_stat ) ;

/*______________________    End of ITC Management     _______________________*/


/*_______________________    Buffer Management     ________________________*/

void * kal_int_get_buffer( kal_poolid  pool_id,
                           kal_uint8   alloc_ent_id,
                           kal_uint8   allocator_id,
                           kal_uint16  size,
                           kal_uint32* num_free_buffs_p,
                           kal_uint8   file_ent_id,
                           kal_uint16 alloc_file, 
                           kal_uint16 alloc_line );

kal_uint32 /*@alt void@*/ kal_int_release_buffer(   
                                   void*      buff_ptr,
                                   kal_uint8  file_ent_id,
                                   kal_uint16  dealloc_file,
                                   kal_uint16 /*@alt int@*/ dealloc_line);
                            
void kal_debug_get_pool_statistics( kal_poolid    pool_id,
                                 kal_pool_statistics* stat_ptr );

void kal_debug_get_all_buff_statistics( kal_poolid    pool_id, 
                        kal_buff_stats_struct** buff_stat_ptr );

void kal_debug_print_pool_statistics( kal_poolid pool_id );

void kal_debug_reset_pool_statistics( kal_poolid    pool_id );

void kal_debug_is_valid_buffer( void* buff_ptr, kal_bool* status_ptr );

void kal_debug_print_buff_leaks( void );

void kal_debug_print_buff_leaks_by_alloc_id( kal_uint8 entity_id,
                                             kal_uint8 alloc_id );

void kal_debug_print_num_allocated_buffers_by_alloc_id( kal_uint8 entity_id,
                                                        kal_uint8 alloc_id );

void 
kal_debug_print_module_wise_allocation( kal_poolid ext_pool_id );

void kal_debug_get_buffer_statistics( void*    buffer_ptr,
                        kal_buff_info_struct**   buff_info, 
                        kal_buff_alloc_struct**  alloc_info );

void kal_debug_print_buffer_statistics( void* buffer_ptr );

void kal_debug_reset_buffer_statistics( void*     buffer_ptr );

kal_bool kal_debug_get_pool_index( kal_poolid     ext_pool_id,
                                   kal_uint32*    pool_index_p );

void
kal_debug_get_all_pool_statistics( kal_all_pool_statistics all_pool_stat );

void
kal_debug_buff_trace_start( kal_int32 size );

void
kal_debug_buff_trace_end( const char* file );

kal_uint32 kal_debug_set_pool_subgroup( kal_uint8     entity_id,
                                        kal_uint8     mem_type,
                                        kal_poolid    ext_pool_id,
                                        kal_uint16    lower_limit,
                                        kal_uint16    upper_limit,
                                        kal_uint16    sub_grp_sz );

void kal_debug_reset_pool_subgroup( kal_poolid ext_pool_id );

void kal_debug_print_pool_subgroup_statistics ( kal_poolid ext_pool_id );

kal_uint32 kal_debug_initialize_buff_history ( kal_uint8  entity_id, 
                                               kal_uint8  mem_type, 
                                               kal_poolid ext_pool_id );

void kal_debug_reset_buff_history ( kal_poolid   ext_pool_id );

void kal_debug_set_trans_log ( void );

void kal_debug_reset_trans_log ( void );

void kal_debug_start_trans_log_for_pool ( kal_poolid  ext_pool_id );

void kal_debug_end_trans_log_for_pool ( kal_poolid ext_pool_id );

void kal_debug_mark_scenario_end (void );

void kal_debug_init_allocation_over ( void  );

/*_____________________        End of Buffer Management     __________________*/

/*__________________________    Timer Management     ____________________________*/

void kal_debug_get_timer_statistics( kal_timerid    timer_id,
                               kal_timer_statistics* stat_ptr ) ;

void kal_debug_print_timer_statistics( kal_timerid timer_id );

void kal_debug_reset_timer_statistics( kal_timerid    timer_id );
void 
kal_debug_get_all_timer_statistics( kal_all_timer_statistics );

/*______________________    End of Timer Management _______________________*/


/*_________________    Error and Debug Management     __________________*/

void kal_debug_register_print_string_function(
               kal_print_string_func_ptr  to_be_registered_func );

void kal_debug_print( const kal_char*    string_to_be_printed );
void kal_debug_print_int( kal_int32 /*@alt kal_int8,kal_int16@*/
                          int_to_be_printed );
void kal_debug_print_uint( kal_uint32 /*@alt kal_uint8,kal_uint16@*/
                           int_to_be_printed );
void kal_debug_print_uint_in_hex( kal_uint32 int_to_be_printed );

void kal_debug_reset( void );

/*_______________    End of Error and Debug Management     ________________*/

/*_________________    Memory Management     __________________*/

void * kal_int_mem_alloc( kal_uint8   alloc_ent_id,
                          kal_uint8   alloc_id,
                          kal_uint8   mem_type,
                          kal_uint32  size, 
                          kal_uint8   file_ent_id, 
                          kal_uint16 file_code, 
                          kal_uint16 /*@alt int @*/ line );

void * kal_int_realloc( kal_uint8 alloc_ent_id, kal_uint8 alloc_id, 
                        void* old_ptr, kal_uint8 mem_type,  
                        kal_uint32  new_size, kal_uint8 file_ent_id,
                        kal_uint16  file_code, kal_uint16 /*@alt int @*/ line );

void kal_int_mem_free( void * mem_ptr, kal_uint8 file_ent_id,
                       kal_uint16 file_code, kal_uint16 /*@alt int @*/ line );
void kal_debug_print_mem_stats( void );
void kal_debug_print_mem_leaks( void );

/*_______________    End of Memory Management  ________________*/

typedef struct kal_context_struct{
  kal_uint32             kal_tmr_scaler;
  kal_uint16              kal_num_task_created;/* cr229061 */
  kal_uint16              kal_num_buff_pools_created;/* cr229061 */
  kal_bool               kal_mem_over_run_check;
  kal_bool               kal_buff_over_run_check;
  kal_uint32             kal_total_mem_allocated;
  kal_int32              kal_mem_fill_garbage;
  kal_int32              kal_buff_fill_garbage;
  kal_mem_stats_struct** kal_int_malloc_stats;      /*  internal to kal */
  kal_mem_stats_struct** kal_ext_malloc_stats;      /*  external to kal */
  kal_mem_stats_struct** kal_mem_type_malloc_stats; /*  per mem_type */
  kal_uint8*             kal_trans_arr; /* pointer to trans log */
  kal_uint32             kal_trans_arr_size; /* trans log size in bytes */
  kal_uint32             kal_trans_arr_nxt_indx;/*free indx in trans arr */
  kal_uint32             kal_num_buff_allocated;    /* Increment number of alloc buffer */
  kal_pool_info_struct*  kal_pool_info;/* holds start/end addr of pools*/
}kal_context_struct;

/******************************
Declarations Of Exported Globals 
*******************************/
/* Exported to support 'readvar' of these variables */
extern kal_context_struct kal_context_g;

#endif /* _KAL_DEBUG_H */
