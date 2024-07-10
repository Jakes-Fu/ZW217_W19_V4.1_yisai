/*********************************************************************

   (C) Spreadtrum Communications Co. Ltd, 
            All rights reserved.

   This file contains definitions and prototypes internally used 
   by KAL that are common to both Release Version and  Debug version 
   of KAL that are common across different OS targets supported.

   Component-specific prefix : kal

*********************************************************************/

#ifndef _KAL_COMMON_DEFS_H
#define _KAL_COMMON_DEFS_H

/*****************
Include Statements
******************/

#include "kal_interface.h"

/************************
Macros Without Parameters 
*************************/

#define KAL_BUFF_FOOT_PRINT       (kal_uint8)0xF2
#define KAL_BUFF_HDR_STAMP        (kal_uint32)0xF1F1F1F1
#define KAL_BUFF_FOOT_STAMP       (kal_uint8)0xF3

#define INVALID_ENTITY_ID         (kal_uint8)(0-1) 
#define INVALID_MODULE_ID         (kal_uint8)(0-1) 

#if defined( LOG_MEM_HISTORY )
/* Defines the size of header for each chunk allocation with dynamic
 * memory allocation. */
#define KAL_MEM_HEADER_SIZE       offsetof( kal_mem_hdr_struct, usr_buff )
#endif

/*****************
Enums Without Tags 
******************/

/***************
Type Definitions 
****************/

#if defined( LOG_MEM_HISTORY )

/* Defines the KAL header for each allocated memory chunk. 
 * index          : The index into the array kal_mem_g[] at
 *                  which the information about this chunk
 *                  allocation/deallocation is logged.
 * usr_buff       : Starting address of the memory location
 *                  that is returned to the user.
 */
typedef struct
{
    kal_int32   index;
    kal_uint8   usr_buff[1];
}kal_mem_hdr_struct;

/* Identification of the source from where the memory transaction
 * was done, for dealloc transaction alloc_id field is not valid.
 */ 
typedef struct
{
    kal_uint8  alloc_ent_id;
    kal_uint8  alloc_id;
#ifdef DEBUG_KAL    
    kal_uint8   file_ent_id;
    kal_uint16  file_code;
    kal_uint16  line_no;
#endif    
}kal_mem_trans_src;    

/* Defines an entry in the array that keeps information about every
 * memory allocations. 
 * mem_state     : It can be allocate/deallocate.
 * size          : The request made.
 * trans_src     : Identifies the src from where the transaction was done.
 * ext_ent_id    : Significant for allocation which happen internal to kal.
 *                 If the internal allocation is made by external trigger e.g.
 *                 allocating stack size etc, then this field keeps track of
 *                 the external entity that was responsible for the trigger.
 *                 In this case entity_id in trans_src will be that of kal.
 *                 If the allocation is solely at discreation of kal, e.g.
 *                 those which are done during kal initialization, ext_ent_id 
 *                 field will be same as entity_id in trans_src (
 *                 kal_entity_id_g ), similarly if the allocation is solely 
 *                 at the descretion of user, then entity_id in trans_src
 *                 and ext_ent_id are same.
 * mem_type      : The memory handle from where allocation was done.
 * ptr           : details wrt actual allocated ptr.
 */ 

typedef struct
{
    kal_uint8            mem_state;
    kal_uint32           size;
    kal_taskid           task_id;
    kal_mem_trans_src    trans_src;
    kal_uint8            ext_ent_id;
    kal_uint8            mem_type;
    kal_mem_hdr_struct*  ptr;
}kal_mem_log_entry_struct;

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
#if defined(CORRUPTION_CHECK)
extern kal_bool
kal_is_buff_over_run(
                      const void*        dest_ptr,
                      kal_uint32   size,
                      kal_uint32   pool_index );

extern void
kal_check_over_run( 
                    const void*  dest_ptr,
                    kal_uint32   size );
#endif

extern 
void kal_initialize_pool_statistics ( kal_uint8           entity_id,
                                      kal_uint8           mem_type,
                                      kal_internal_poolid pool_id,
                                      kal_uint16          num_buffs, 
                                      const kal_char*     pool_name );

extern kal_uint16
kal_get_buff_num ( void *buff_ptr );


extern
void kal_update_buff_alloc_statistics( kal_buff_hdr_struct* buff_hdr_p,
                                       kal_uint16           size );

extern
void
kal_update_buff_free_statistics( kal_buff_hdr_struct* buff_hdr_p );

extern kal_bool 
kal_get_poolindex_from_usr_buff( const void*  buff_ptr,
                                       kal_uint32* index_p );

extern void
kal_validate_buff_hdr( kal_buff_hdr_struct* buff_hdr );

extern void
kal_validate_buff_footer( 
                                kal_uint8*    usr_buff,
                                kal_uint16    curr_alloc_size );
#ifdef DEBUG_KAL
void
kal_check_buff_corruption( kal_buff_hdr_struct *buff_hdr_p,
		                         kal_uint16 *buffer_num,
								 kal_uint16 *buffer_size );
#else
void
kal_check_buff_corruption( kal_buff_hdr_struct *buff_hdr_p);
#endif

#if defined( LOG_MEM_HISTORY )
extern void kal_init_mem_stack( void );

#ifdef DEBUG_KAL
extern void
kal_store_allocation_stats ( void        *mem_ptr,
                             kal_uint8   ext_ent_id,
                             kal_uint8   alloc_id,
                             kal_uint32  size_allocated,
                             kal_uint8   mem_type,
                             kal_bool    is_kal_int_alloc,
                             kal_uint8   file_ent_id, 
                             kal_uint16  file_code,
                             kal_uint16  line );
#else
extern void
kal_store_allocation_stats ( void       *mem_ptr,
                             kal_uint8  ext_ent_id,
	                         kal_uint8  alloc_id,
						     kal_uint32 size_allocated, 
                             kal_uint8  mem_type,
                             kal_bool   is_kal_int_alloc ); 
#endif

#ifdef DEBUG_KAL
extern kal_bool
kal_store_deletion_stats(  kal_mem_hdr_struct*  mem_hdr_ptr, 
                           kal_uint8            file_ent_id,
                           kal_uint16           file_code,
                           kal_uint16           line );
#else
extern kal_bool
kal_store_deletion_stats( kal_mem_hdr_struct*  mem_hdr_ptr);
#endif

extern void kal_push_mem_index( kal_int32 index );
extern kal_int32 kal_pop_mem_index( void );

#endif

extern void kal_update_mem_allocation_stats( 
                                 kal_mem_stats_struct ** trans_stats, 
                                 kal_uint8 entity_id, kal_uint8 trans_id,
                                 kal_uint32 size_allocated );

extern void kal_update_mem_free_stats( kal_mem_stats_struct ** trans_stats,
                                       kal_uint8 entity_id, kal_uint8 trans_id,
                                       kal_uint32 size );

/******************************
Declarations Of Exported Globals 
*******************************/

#if defined( LOG_MEM_HISTORY )
extern kal_uint32                 kal_max_num_mem_allocations_g;

extern kal_mem_log_entry_struct*  kal_mem_log_g;
extern kal_uint32*                kal_mem_index_stack_g;

extern kal_int32                  kal_mem_index_top_g;
extern kal_int32                  kal_lowest_allocated_index_g;

extern struct kal_internal_mutexid_struct kal_mem_mutex_g;
#endif

extern kal_uint8                  kal_default_mem_type_g;
extern kal_uint8                  kal_entity_id_g;
extern kal_uint8                  kal_no_mem_types_g;
extern kal_entity_info *          kal_entities_g;

#endif /* _KAL_COMMON_DEFS_H */
