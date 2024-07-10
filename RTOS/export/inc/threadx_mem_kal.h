/******************************************************************************
 ** File Name:      threadx_mem_kal.h                                         *
 ** Author:         rick.zhang                                                *
 ** DATE:           2006-05-25                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the memory config                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/25/2006     rick.zhang       Create.                                   *
 ******************************************************************************/
 #ifndef THREADX_MEM_KAL_H
 #define THREADX_MEM_KAL_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
 #include "sci_types.h"
 #include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Data & Function Prototype                         *
 **---------------------------------------------------------------------------*/
#ifndef UEIT_MOCOR
enum entity_enum{
  ENTITY_INFRA,
  ENTITY_STACK,
  ENTITY_GSM_STACK,
  /* Add OSA and USER entity for TARGET_ARM */
  ENTITY_OSA,
  ENTITY_USER,
  ENTITY_LAST
};

/* Defines the memory handle types used for KAL resource
   allocation */
enum entity_mem_hdl_enum{
  MEM_HDL_INFRA_DEFAULT,
  MEM_HDL_STACK_DEFAULT,
  /* Add OSA and USER mem type for TARGET_ARM */
  MEM_HDL_OSA_DEFAULT,
  MEM_HDL_USER_DEFAULT,
  MEM_HDL_LAST
};

/* Define the infra structure modules */
enum osa_module_enum
{
    MOD_OSA_NIL,
	MOD_OSA_BUFFMNGR,
	MOD_OSA_MEMMNGR,
	MOD_OSA_TASKMNGT,
	MOD_OSA_TIMER,
	MOD_OSA_ITC_QUEUE,
	MOD_OSA_ITC_MUTEX,
	MOD_OSA_ITC_SEM,
    MOD_OSA_LAST
};
#endif

/* @rick.zhang Add mem_pool_cfg for memory initialize */
typedef struct MEM_POOL_CFG_tag
{
    uint32  id;
    uint16  block_size;
    uint16  num_blocks;
    uint32  pool_size;
    uint32  *pool_addr;
    void    **pool_control_block;
    char    *pool_name;
    uint32  *pool_max_used;
} MEM_POOL_CFG_T;

#define MEM_INVALID_CFG_ID      0xFFFFFFFF
#define MEM_BYTEPOOL_CFG_ID     0x0
#define MEM_BLOCKPOOL_CFG_ID    0x1
#define BYTEPOOL_BLOCK_SIZE     0x0
#define BYTEPOOL_NUM_BLOCKS     0x0
#define MEM_NOT_INIT_ADDR       (uint32 *)0xFFFFFFFF

//
// Pool config
//
extern TX_BYTE_POOL  byte_static_heap;
extern TX_BYTE_POOL  byte_dynamic_base_heap;
extern TX_BYTE_POOL  byte_dynamic_app_heap;

extern void* pbyte_static_heap;
extern void* pbyte_dynamic_base_heap;
extern void* pbyte_dynamic_app_heap;

// Caculate the actual block size, which the pointer will be allocated at!
/*
#define PARTITION_POOL(SIZE) \
        (void *)( \
         (SIZE) <= block_100_size ? (SIZE) <= block_52_size ? ppool_52 : ppool_100 \
    	: (SIZE) <= block_300_size ? ppool_300 : (SIZE) <= block_600_size ? ppool_600 : \
        (SIZE) <= block_1600_size ? ppool_1600 : (SIZE) <= block_6000_size ? ppool_6000 : SCI_NULL \
        )
*/
extern uint32   byte_static_heap_max_used;
extern uint32   byte_dynamic_base_heap_max_used;
extern uint32   byte_dynamic_app_heap_max_used;
extern uint32   bytepool_max_used;

extern MEM_HEADER_T  dbg_head;
extern uint32        mem_block_count;

extern const uint32 g_block_pool_inx_start;
extern MEM_POOL_CFG_T mem_pool_cfg[];


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif  // THREADX_MEM_KAL_H

