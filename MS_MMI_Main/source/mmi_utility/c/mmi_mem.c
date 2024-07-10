/******************************************************************************
 ** File Name:      mmi_mem.c                                               *
 ** Author:         gang.tong                                              *
 ** DATE:           03/13/2008                                                *
 ** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 large block memory including malloc and free              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/13/2008     gang.tong        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_utility_trc.h"
#include "os_api.h"
#include "mmi_mem.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
#define MEM_SUCCESS                              1
#define MEM_NO_MEMORY                           -1 
    
#define MMIMEM_NO_MEM_CONFIG_STORAGE_ERRER      -1
#define MMIMEM_MEM_ADDR_INVALID_ERRER           -2


#define MEM_HEAP_BLOCK_ALLOC                     0xAAAAAAAAUL
#define MEM_HEAP_BLOCK_FREE                      0xFFFFEEEEUL
#define MEM_HEAP_BLOCK_MIN                       20
typedef uint8*                                   NEXT_PTR;
typedef unsigned long                            ULONG;/*lint !e761*/



typedef struct 
{
    MMIMEM_BLOCK_TYPE block_type;
    uint8* pool_buffer_start_addr;
    uint32 pool_buffer_size;
    uint8* available_block_list;    /* not distributed block list. */
    uint32 available_block_num;     /* the number of can distributed blocks */
}MMIMEM_POOL_CONTEXT;

typedef struct 
{
    MMIMEM_POOL_CONTEXT pool_context[MMIMEM_BLOCK_TYPE_MAX];    /* in this array, record the type of block */
    uint16 pool_context_num;                                  /* the number of type */
}MMIMEM_POOL_INFO;

typedef struct 
{
    uint8* heap_start_addr;
    uint32 heap_size;
    uint32 heap_available_size;
    uint32 heap_fragments;
    uint8* heap_search;
}MMIMEM_HEAP_INFO;

typedef struct 
{
    int16 manager_id;   /* >0: available; others: unavailable */
    uint8* mem_start_addr;
    uint32 mem_total_size;
    MMIMEM_POOL_INFO all_pool_info;
    MMIMEM_HEAP_INFO heap_info;    
}MMIMEM_CONFIG_T, *MMIMEM_CONFIG_T_PTR;

LOCAL int16 s_mem_manage_id = 1;
LOCAL MMIMEM_CONFIG_T s_mem_config_arr[MMIMEM_MEM_MAX] = {0};

/*****************************************************************************/
//  Description:  release heap buffer
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void RealeaseHeapPool(uint8* memory_ptr);
/*lint -e737*/
/*****************************************************************************/
//  Description:    PrintfHeapLog
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
/*lint -e737*/
LOCAL void PrintfHeapLog(MMIMEM_HEAP_INFO const * heap_info_ptr)
{
	NEXT_PTR work_ptr = heap_info_ptr->heap_start_addr;
	uint32 fragment_num = 0;
	
	//SCI_TRACE_LOW:"[M]:heap_start_addr = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_94_112_2_18_3_25_12_2,(uint8*)"d", heap_info_ptr->heap_start_addr);
	//SCI_TRACE_LOW:"[M]:heap_fragment = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_95_112_2_18_3_25_12_3,(uint8*)"d", heap_info_ptr->heap_fragments);
	//SCI_TRACE_LOW:"[M]:heap_available_size = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_96_112_2_18_3_25_12_4,(uint8*)"d", heap_info_ptr->heap_available_size);
	//SCI_TRACE_LOW:"[M]:heap_search = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_97_112_2_18_3_25_12_5,(uint8*)"d", heap_info_ptr->heap_search);
	while((uint32)(*(uint32*)(work_ptr+sizeof(NEXT_PTR))) != MEM_HEAP_BLOCK_ALLOC)
	{
     	//SCI_TRACE_LOW:"[M]:fragment = %d"
     	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_100_112_2_18_3_25_12_6,(uint8*)"d", fragment_num);
		//SCI_TRACE_LOW:"[M]:cur_ptr  = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_101_112_2_18_3_25_12_7,(uint8*)"d", work_ptr);
		//SCI_TRACE_LOW:"[M]:state = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_102_112_2_18_3_25_12_8,(uint8*)"d", (*(uint32*)(work_ptr+sizeof(NEXT_PTR))));
		//SCI_TRACE_LOW:"[M]:realy memory = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_103_112_2_18_3_25_12_9,(uint8*)"d", (*(uint32*)(work_ptr+sizeof(NEXT_PTR)+sizeof(uint32))));
		//SCI_TRACE_LOW:"[M]:realy memory2 = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_104_112_2_18_3_25_12_10,(uint8*)"d", (*(uint32*)(work_ptr+sizeof(NEXT_PTR)+sizeof(uint32)+sizeof(uint32))));
		//SCI_TRACE_LOW:"[M]:next_ptr = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_105_112_2_18_3_25_12_11,(uint8*)"d", *((NEXT_PTR*)work_ptr));
		work_ptr = *((NEXT_PTR*)work_ptr);
		fragment_num ++;
	}	
}
/*****************************************************************************/
//  Description:    chech heap memory frame
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void AssertCheckHeapInfo(MMIMEM_HEAP_INFO const * heap_info_ptr)
{
    NEXT_PTR work_ptr = heap_info_ptr->heap_start_addr;
	uint32 fragment_num = 0;	
    uint32 block_size = 0;
	
	while((uint32)(*(uint32*)(work_ptr+sizeof(NEXT_PTR))) != MEM_HEAP_BLOCK_ALLOC)
	{        
        if((work_ptr > heap_info_ptr->heap_start_addr + heap_info_ptr->heap_size) || (work_ptr < heap_info_ptr->heap_start_addr))
        {
            PrintfHeapLog(heap_info_ptr);
            SCI_ASSERT(0); /*assert verified*/
        }		
        block_size = *((uint32*) (work_ptr + sizeof(NEXT_PTR)));
        if((block_size != MEM_HEAP_BLOCK_FREE) && (block_size != MEM_HEAP_BLOCK_ALLOC) && (block_size != (uint32)heap_info_ptr))
        {
            PrintfHeapLog(heap_info_ptr);
            SCI_ASSERT(0); /*assert verified*/
        }
		work_ptr = *((NEXT_PTR*)work_ptr);
		fragment_num ++;
	}	
}

/*****************************************************************************/
//  Description:    init block available list
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void InitBlockPoolAvailableList(MMIMEM_CONFIG_T* mem_config_ptr, MMIMEM_BLOCK_TYPE const * block_type_arr, uint16 type_num)
{
    uint16 i = 0;
    uint32 num = 0;
    uint8* available_ptr = PNULL;
    MMIMEM_POOL_CONTEXT * pool_context_ptr = PNULL;

    SCI_ASSERT(PNULL != mem_config_ptr);    /*assert verified*/
    /* storage block info */
    SCI_ASSERT(type_num < MMIMEM_BLOCK_TYPE_MAX);    /*assert verified*/
    mem_config_ptr->all_pool_info.pool_context_num = type_num;    
    
    if(type_num > 0)
    {
        SCI_ASSERT(PNULL != block_type_arr); /*assert verified*/
    }

    for(i = 0; i < type_num; i ++)
    {        
        mem_config_ptr->all_pool_info.pool_context[i].block_type.size = block_type_arr[i].size;
        mem_config_ptr->all_pool_info.pool_context[i].block_type.num = block_type_arr[i].num;
    }

    /* config pool available list */
    for(i = 0; i < mem_config_ptr->all_pool_info.pool_context_num; i ++)
    {        
        pool_context_ptr = &(mem_config_ptr->all_pool_info.pool_context[i]);
        /* calculate pool start and end addr */
        if(0 == i)
        {
            pool_context_ptr->pool_buffer_start_addr = mem_config_ptr->mem_start_addr;
        }
        else
        {
            pool_context_ptr->pool_buffer_start_addr = mem_config_ptr->all_pool_info.pool_context[i-1].pool_buffer_start_addr + mem_config_ptr->all_pool_info.pool_context[i-1].pool_buffer_size;
        }
        pool_context_ptr->pool_buffer_size = (pool_context_ptr->block_type.size+sizeof(NEXT_PTR))*pool_context_ptr->block_type.num;

        SCI_ASSERT(pool_context_ptr->pool_buffer_start_addr + pool_context_ptr->pool_buffer_size <= mem_config_ptr->mem_start_addr + mem_config_ptr->mem_total_size); /*assert verified*/
        /* set the available list */
        pool_context_ptr->available_block_list = pool_context_ptr->pool_buffer_start_addr;
        available_ptr = pool_context_ptr->available_block_list;
        for(num = 0; num < pool_context_ptr->block_type.num; num ++)
        {
            /* increment the available block count */
            pool_context_ptr->available_block_num ++;
            /* setup the link to the next block */
            *((uint8**)available_ptr) = available_ptr + (pool_context_ptr->block_type.size + sizeof(NEXT_PTR));
            /* update to next block pointer */
            available_ptr = *((uint8**)available_ptr);
        }
        /* backup to the last block in the pool */
        *((uint8**)(available_ptr - (pool_context_ptr->block_type.size + sizeof(NEXT_PTR)))) = PNULL;
    }
}

/*****************************************************************************/
//  Description:    get mem config according to manager id
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL MMIMEM_CONFIG_T * GetMemConfigFromManagerID(int16 manager_id)
{
    uint16 array_i = 0;
    for(array_i = 0; array_i < MMIMEM_MEM_MAX; array_i ++)
    {
        if(s_mem_config_arr[array_i].manager_id == manager_id)
        {
            break;
        }
    }

    if(array_i >= MMIMEM_MEM_MAX)
    {
        SCI_PASSERT(0, ("error, invalid manager_id:%d!", manager_id)); /*assert verified*/
        return PNULL; /*lint !e527*/
    }
    return (&(s_mem_config_arr[array_i]));
}

/*****************************************************************************/
//  Description:    init heap pool
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void InitHeapPool(MMIMEM_CONFIG_T* mem_config_ptr)
{
    uint8 * block_ptr = PNULL;
    int32 aligment_padding = 0;

    SCI_ASSERT(PNULL != mem_config_ptr); /*assert verified*/
    if(mem_config_ptr->all_pool_info.pool_context_num > 0)
    {
        mem_config_ptr->heap_info.heap_start_addr = mem_config_ptr->all_pool_info.pool_context[mem_config_ptr->all_pool_info.pool_context_num-1].pool_buffer_start_addr
                                + mem_config_ptr->all_pool_info.pool_context[mem_config_ptr->all_pool_info.pool_context_num-1].pool_buffer_size;    
    }
    else
    {
        mem_config_ptr->heap_info.heap_start_addr = mem_config_ptr->mem_start_addr;
    }
    /* aligment padding size */
    aligment_padding = sizeof(uint32) - ((uint32)(mem_config_ptr->heap_info.heap_start_addr))%sizeof(uint32);
    aligment_padding = aligment_padding%sizeof(uint32);/*lint !e573*/
    /* round the start address by uint32, for aligment */
    mem_config_ptr->heap_info.heap_start_addr = mem_config_ptr->heap_info.heap_start_addr + aligment_padding;
    mem_config_ptr->heap_info.heap_size = mem_config_ptr->mem_total_size + mem_config_ptr->mem_start_addr - mem_config_ptr->heap_info.heap_start_addr - aligment_padding;    
    
    /* Initially, the pool will have two blocks.  One large block at the 
        beginning that is available and a small allocated block at the end
        of the pool that is there just for the algorithm.  Be sure to count
        the available block's header in the available bytes count.  */
    mem_config_ptr->heap_info.heap_available_size = mem_config_ptr->heap_info.heap_size;
    mem_config_ptr->heap_info.heap_fragments = 2;
    mem_config_ptr->heap_info.heap_search = mem_config_ptr->heap_info.heap_start_addr;

    /* Calculate the end of the pool's memory area.  */
    block_ptr = mem_config_ptr->heap_info.heap_start_addr + mem_config_ptr->heap_info.heap_available_size;
    /* Backup the end of the pool pointer and build the pre-allocated block.  */
    block_ptr =  block_ptr - sizeof(NEXT_PTR);
    *((uint32*) block_ptr) =  MEM_HEAP_BLOCK_ALLOC;
    block_ptr =  block_ptr - sizeof(NEXT_PTR);
    *((NEXT_PTR*) block_ptr) =  mem_config_ptr->heap_info.heap_start_addr;
    mem_config_ptr->heap_info.heap_available_size -= sizeof(NEXT_PTR) + sizeof(uint32);

    /* Now setup the large available block in the pool.  */
    *((NEXT_PTR*) mem_config_ptr->heap_info.heap_start_addr) =  block_ptr;
    block_ptr =  (NEXT_PTR) mem_config_ptr->heap_info.heap_start_addr;
    block_ptr =  block_ptr + sizeof(NEXT_PTR);
    *((uint32 *) block_ptr) =  MEM_HEAP_BLOCK_FREE;
    mem_config_ptr->heap_info.heap_available_size -= sizeof(NEXT_PTR) + sizeof(uint32);


    
}

/*****************************************************************************/
//  Description:    order block type arry, from little to great
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void OrderBlockPoolArry(MMIMEM_POOL_INFO* block_info_ptr)
{
    uint16 i = 0;
    uint16 j = 0;
    MMIMEM_POOL_CONTEXT temp_context = {0};
    SCI_ASSERT(PNULL != block_info_ptr); /*assert verified*/
    for(i = 0; i < block_info_ptr->pool_context_num -1; i ++ )
    {
        for(j = 0; j < block_info_ptr->pool_context_num -1 -i; j ++)
        {
            if(block_info_ptr->pool_context[j].block_type.size > block_info_ptr->pool_context[j+1].block_type.size)
            {
                temp_context = block_info_ptr->pool_context[j];
                block_info_ptr->pool_context[j] = block_info_ptr->pool_context[j+1];
                block_info_ptr->pool_context[j+1] = temp_context;
            }
        }
    }
}

/*****************************************************************************/
//  Description:    init the mem block info, memory start address, memory size
//                  and memory status.
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void MemPoolInit(MMIMEM_CONFIG_T* config_ptr, MMIMEM_BLOCK_TYPE const * block_type_arr, uint16 type_num)
{
    /* init pool available block list */
    InitBlockPoolAvailableList(config_ptr, block_type_arr, type_num);    
    /* order block type sequence */
    OrderBlockPoolArry(&(config_ptr->all_pool_info));
    /* init heap pool */
    InitHeapPool(config_ptr);
}

/*****************************************************************************/
//  Description:    check the mem buffer validate
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckMemAddrValidate(uint8 * mem_start_addr, uint32 mem_total_size)
{
    uint16 i = 0;
    uint8* start_addr = 0;
    uint8* end_addr = 0; 
    MMIMEM_CONFIG_T_PTR mem_config_ptr = PNULL;
    SCI_ASSERT(PNULL != mem_start_addr); /*assert verified*/
    for(i = 0; i < MMIMEM_MEM_MAX; i ++)
    {
        if(s_mem_config_arr[i].manager_id > 0)
        {
            mem_config_ptr = &(s_mem_config_arr[i]);
            /* calculate the min start address between the new memory and used memory */
            start_addr = MIN(mem_config_ptr->mem_start_addr, mem_start_addr);
            /* calculate the max end address between the new memory and used memory */
            end_addr = MAX(mem_config_ptr->mem_start_addr+mem_config_ptr->mem_total_size, mem_start_addr+mem_total_size);
            /* if the sum of two memory size is greater than the size of min start and max end, memory overlap! */
            if((uint32)(end_addr-start_addr) < (mem_total_size+mem_config_ptr->mem_total_size))
            {
                SCI_PASSERT(0, ("memory overlap with %d mem", i)); /*assert verified*/
                return FALSE; /*lint !e527*/
            }
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description:    config the mem block info, memory start address, memory size
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL int16 CreateNewManageID(void)
{    
    s_mem_manage_id ++;
    if(0 == s_mem_manage_id)
    {
        s_mem_manage_id = 1;
    }
    //SCI_TRACE_LOW:"[M]:s_mem_manage_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_356_112_2_18_3_25_12_12,(uint8*)"d", s_mem_manage_id);
    return s_mem_manage_id;
}

/*****************************************************************************/
//  Description:    config the mem block info, memory start address, memory size
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC int16 MMIAPIMEM_Config(uint8 * mem_start_addr, uint32 mem_total_size, MMIMEM_BLOCK_TYPE const * block_type_arr, uint16 type_num)
{
    uint16 i = 0;            
    SCI_ASSERT(PNULL != mem_start_addr); /*assert verified*/
    SCI_ASSERT(0 != mem_total_size);     /*assert verified*/
    
    /* try to config a new mem */
    for(i = 0; i < MMIMEM_MEM_MAX; i ++)
    {
        if(s_mem_config_arr[i].manager_id <= 0)
        {
            break;
        }
    }
    /* whether there is a not used config storage */
    if(i >= MMIMEM_MEM_MAX)
    {
        return MMIMEM_NO_MEM_CONFIG_STORAGE_ERRER;
    }

    /* check mem addr validate */    
    if(CheckMemAddrValidate(mem_start_addr, mem_total_size))
    {        
        /* storage config info */    
        s_mem_config_arr[i].mem_start_addr = mem_start_addr;
        s_mem_config_arr[i].mem_total_size = mem_total_size;
        s_mem_config_arr[i].manager_id = CreateNewManageID(); 
        MemPoolInit(&(s_mem_config_arr[i]), block_type_arr, type_num);
        
        return (s_mem_config_arr[i].manager_id);
    }
    return MMIMEM_MEM_ADDR_INVALID_ERRER;
}

/*****************************************************************************/
//  Description:  try to find the proper memory pool.
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL int16 FindProperPool(MMIMEM_POOL_INFO const * block_pool_info_ptr, uint32 size)
{
    uint16 index = 0;
    SCI_ASSERT(PNULL != block_pool_info_ptr); /*assert verified*/
    for(index = 0; index < block_pool_info_ptr->pool_context_num; index ++)
    {
        if(block_pool_info_ptr->pool_context[index].block_type.size >= size)
        {
            break;
        }
    }
    if(index < block_pool_info_ptr->pool_context_num)
    {
        return index;
    }
    else
    {
        return -1;
    }
}
/*****************************************************************************/
//  Description:  mallloc buffer from heap pool
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL int16 HeapPoolAllocate(MMIMEM_HEAP_INFO * heap_pool_ptr, uint8 ** memory_ptr, uint32 memory_size)
{    
    uint8 * current_ptr = PNULL;
    uint8 * next_ptr = PNULL;
    uint32 available_bytes = 0;
    uint32 examine_blocks = 0;

    SCI_ASSERT(PNULL != heap_pool_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != memory_ptr);	/*assert verified*/
    
    /* round the memory size up to the next size that is evenly divisable by
       an uint32.  This guarantees proper alignment */
    memory_size = ((memory_size + sizeof(uint32)-1)/sizeof(uint32)) * sizeof(uint32);
    
    /* search */
    if(memory_size > heap_pool_ptr->heap_available_size)
    {
        return MEM_NO_MEMORY;
    }
    
    /* walk through the memory pool in search for a large enough block */
    current_ptr =      heap_pool_ptr->heap_search;
    examine_blocks =   heap_pool_ptr->heap_fragments + 1;
    available_bytes =  0;
    
    do 
    {
         /* check to see if this block is free */
        if (*((uint32*) (current_ptr + sizeof(NEXT_PTR))) == MEM_HEAP_BLOCK_FREE)
        {
            /** block is free, see if it is large enough **/

            /* Pickup the next block's pointer.  */
            next_ptr =  *((NEXT_PTR*) current_ptr);

            SCI_ASSERT(next_ptr > current_ptr); /*assert verified*/
            SCI_ASSERT((heap_pool_ptr->heap_start_addr + heap_pool_ptr->heap_size) >= next_ptr); /*assert verified*/
            SCI_ASSERT((heap_pool_ptr->heap_start_addr) <= next_ptr); /*assert verified*/

            /* Calculate the number of byte available in this block.  */
            available_bytes =  next_ptr - current_ptr - sizeof(NEXT_PTR) - sizeof(ULONG);
            
            /* If this is large enough, we are done because our first-fit algorithm
               has been satisfied!  */
            if (available_bytes >= memory_size)
            {
                /* Get out of the search loop!  */
                break;
            }
            else
            {
                /* Clear the available bytes variable.  */
                available_bytes =  0;

                /* Not enough memory, check to see if the neighbor is 
                   free and can be merged.  */
                if (*((uint32 *) (next_ptr + sizeof(NEXT_PTR))) == MEM_HEAP_BLOCK_FREE)
                {

                    /* Yes, neighbor block can be merged!  This is quickly accomplished
                       by updating the current block with the next blocks pointer.  */
                    *((NEXT_PTR *) current_ptr) =  *((NEXT_PTR *) next_ptr);

                    /* Reduce the fragment total */
                    heap_pool_ptr->heap_fragments --;   
                    
                    /* increase the bytes available */
                    heap_pool_ptr->heap_available_size += sizeof(NEXT_PTR) + sizeof(uint32);

                    /* See if the search pointer is affected.  */
                    if (heap_pool_ptr->heap_search == next_ptr)
                    {
                        heap_pool_ptr->heap_search =  current_ptr;                        
                    }
                }
                else
                {
                    if(*((NEXT_PTR *) next_ptr) > current_ptr)
                    {
                        SCI_ASSERT(*((uint32*) (next_ptr + sizeof(NEXT_PTR))) == (uint32)heap_pool_ptr); /*assert verified*/
                    }
                    else
                    {
                        SCI_ASSERT(*((uint32*) (next_ptr + sizeof(NEXT_PTR))) == MEM_HEAP_BLOCK_ALLOC); /*assert verified*/
                    }
                    
                    /* Neighbor is not free so we can skip over it!  */
                    current_ptr =  *((NEXT_PTR *) next_ptr);                                        
                    /* Decrement the examined block count to account for this one.  */
                    if (examine_blocks)
                    {
                        examine_blocks--;
                    }
                    
                    
                }              
            }
        }
        else
        {   

            if(*((NEXT_PTR *) current_ptr) > current_ptr)
            {
                SCI_ASSERT(*((uint32*) (current_ptr + sizeof(NEXT_PTR))) == (uint32)heap_pool_ptr); /*assert verified*/
            }
            else
            {
                SCI_ASSERT(*((uint32*) (current_ptr + sizeof(NEXT_PTR))) == MEM_HEAP_BLOCK_ALLOC); /*assert verified*/
            }

            /* Block is not free, move to next block.  */
            current_ptr = *((NEXT_PTR *) current_ptr);              
            
            SCI_ASSERT((heap_pool_ptr->heap_start_addr + heap_pool_ptr->heap_size) >= current_ptr); /*assert verified*/
            SCI_ASSERT((heap_pool_ptr->heap_start_addr) <= current_ptr); /*assert verified*/
        } 	

        /* Another block has been searched... decrement counter.  */
        if (examine_blocks)
        {
            examine_blocks--;
        }

    } while(examine_blocks);
    
    /* Determine if a block was found.  If so, determine if it needs to be split.  */
    SCI_ASSERT(available_bytes <= heap_pool_ptr->heap_available_size); /*assert verified*/
    if (available_bytes)
    {
        /* Determine if we need to split this block.  */
        if ((available_bytes - memory_size) >= ((uint32)MEM_HEAP_BLOCK_MIN))
        {

            /* Split the block.  */
            next_ptr =  current_ptr + memory_size + sizeof(NEXT_PTR) + sizeof(uint32);

            /* Setup the new free block.  */
            *((NEXT_PTR *) next_ptr) =  *((NEXT_PTR *) current_ptr);
            *((uint32 *) (next_ptr + sizeof(NEXT_PTR))) =  MEM_HEAP_BLOCK_FREE;

            /* Increase the total fragment counter.  */
            heap_pool_ptr->heap_fragments ++;
            heap_pool_ptr->heap_available_size -= sizeof(NEXT_PTR) + sizeof(uint32);

            /* Update the current pointer to point at the newly created block.  */
            *((NEXT_PTR*) current_ptr) = next_ptr;

            /* Set available equal to memory size for subsequent calculation.  */
            available_bytes = memory_size;
        }

        /* In any case, mark the current block as allocated.  */
        *((MMIMEM_HEAP_INFO **) (current_ptr + sizeof(NEXT_PTR))) = heap_pool_ptr;

        /* Reduce the number of available bytes in the pool.  */
        heap_pool_ptr->heap_available_size -=  available_bytes;

        /* Update the search pointer to the next block.  */
        heap_pool_ptr->heap_search = *((NEXT_PTR*) current_ptr);
        
        /* Adjust the pointer for the application.  */
        current_ptr =  current_ptr + sizeof(NEXT_PTR) + sizeof(uint32);
    }
    else
    {
        /* Set current pointer to NULL to indicate nothing was found.  */
        current_ptr =  PNULL;
    }

    *memory_ptr = current_ptr;
	
    if(PNULL == current_ptr)
    {
        return MEM_NO_MEMORY;
    }
    return MEM_SUCCESS;        
}

/*****************************************************************************/
//  Description:  mallloc buffer from block pool
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL int16 BlockPoolAllocate(MMIMEM_POOL_CONTEXT * pool_context_ptr, uint8 ** block_ptr)
{
    uint8 * work_ptr = PNULL;
    SCI_ASSERT(PNULL != pool_context_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != block_ptr); /*assert verified*/
    /* determine if there is an available block */
    if(pool_context_ptr->available_block_num)
    {
        /* Yes, a block is available.  decrement the available count */
        pool_context_ptr->available_block_num --;
        
        /* pickup the current block pointer */
        work_ptr =  pool_context_ptr->available_block_list;

        /* return the first available block to the caller */
        *(block_ptr) = work_ptr + sizeof(NEXT_PTR);

        /* modify the available list to point at the next block in the pool */
        pool_context_ptr->available_block_list = *((uint8**) work_ptr);

        /* save the pool's address in the block for when it is released! */
        *((uint8**)work_ptr) = (uint8*)pool_context_ptr;
    
        /* return status to success */        
        return MEM_SUCCESS;

    }
    else
    {
        return MEM_NO_MEMORY;
    }
}

/*****************************************************************************/
//  Description:  malloc buffer
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC uint8* MMIAPIMEM_Malloc(int16 manager_id, uint32 size)
{
    MMIMEM_CONFIG_T_PTR mem_config_ptr = PNULL;
    MMIMEM_POOL_INFO  * block_pool_info_ptr = PNULL;
    MMIMEM_HEAP_INFO  * heap_pool_info_ptr = PNULL;
    int16 target_pool_index = 0;
    int16 alloc_ret = 0;    
    uint8* memory_ptr = PNULL;
    
    mem_config_ptr = GetMemConfigFromManagerID(manager_id);
    SCI_ASSERT(PNULL != mem_config_ptr); /*assert verified*/
    block_pool_info_ptr = &(mem_config_ptr->all_pool_info);
    heap_pool_info_ptr = &(mem_config_ptr->heap_info);

    /* try to find the proper memory pool */
    target_pool_index = FindProperPool(block_pool_info_ptr, size);
    /* block pool  */
    if(target_pool_index >= 0)
    {
        alloc_ret = BlockPoolAllocate(&(block_pool_info_ptr->pool_context[target_pool_index]), &memory_ptr);
    }
    else
    {    	
        alloc_ret = HeapPoolAllocate(heap_pool_info_ptr, &memory_ptr, size);   
        if(MEM_SUCCESS == alloc_ret)
        {
        	//SCI_TRACE_LOW:"[M]:HeapPoolAllocate :info_ptr = 0x%x, size = %d, return memory_ptr = 0x%x\n"
        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_671_112_2_18_3_25_13_13,(uint8*)"ddd",heap_pool_info_ptr, size, memory_ptr);
        }
    }

    /* treat with allocate result */    
    
    if(MEM_SUCCESS == alloc_ret)
    {
        //SCI_TRACE_LOW:"[M]:MMIAPIMEM_Malloc: size = %d, return = 0x%x\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_679_112_2_18_3_25_13_14,(uint8*)"dd", size, memory_ptr);
        return memory_ptr;
    }
    else if(MEM_NO_MEMORY == alloc_ret && target_pool_index >= 0)
    {
        alloc_ret = HeapPoolAllocate(heap_pool_info_ptr, &memory_ptr, size);        
        if(MEM_SUCCESS == alloc_ret)
        {
            //SCI_TRACE_LOW:"[M]:MMIAPIMEM_Malloc again: size = %d, return = 0x%x\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_687_112_2_18_3_25_13_15,(uint8*)"dd", size, memory_ptr);
            return memory_ptr;
        }        
    }
        
    /* no memory can be allocate, so return PNULL */    
    //SCI_TRACE_LOW:"[M]:malloc failed, no memory"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_693_112_2_18_3_25_13_16,(uint8*)"");
    return PNULL; 
    
}

/*****************************************************************************/
//  Description:  release block buffer
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void RealeaseBlockPool(uint8* block_ptr)
{
    uint8 * work_ptr = PNULL;
    MMIMEM_POOL_CONTEXT * pool_context_ptr;
    SCI_ASSERT(PNULL != block_ptr); /*assert verified*/
    /* Pickup the pool pointer which is just previous to the starting 
       address of block that the caller sees.  */
    work_ptr = block_ptr - sizeof(NEXT_PTR);
    pool_context_ptr = (MMIMEM_POOL_CONTEXT *)(*((NEXT_PTR*)work_ptr));

    /* check for an invalid pool pointer.  */
    SCI_ASSERT(PNULL != pool_context_ptr); /*assert verified*/
    
    /* Put the block back in the available list.  */
    *((NEXT_PTR*) work_ptr) = pool_context_ptr->available_block_list;
    
    /* Adjust the head pointer.  */
    pool_context_ptr->available_block_list = work_ptr;        
    
    /* Increment the count of available blocks.  */
    pool_context_ptr->available_block_num ++;
}


/*****************************************************************************/
//  Description:  release heap buffer
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void CheckHeapPoolMemory(uint8* memory_ptr)
{
    uint8 * work_ptr = PNULL;
    uint8 * next_work_ptr = PNULL;
    MMIMEM_HEAP_INFO * heap_pool_info_ptr = PNULL;
    uint i = 0;
    BOOLEAN is_heap_info_addr = FALSE;
    uint32* alloc_flag_ptr = PNULL;    
    
    SCI_ASSERT(PNULL != memory_ptr); /*assert verified*/
    /* Back off the memory pointer to pickup its header.  */
    work_ptr = memory_ptr - sizeof(NEXT_PTR) - sizeof(uint32);
    /* Pickup the pool pointer.  */
    SCI_ASSERT(*((NEXT_PTR *) (work_ptr)) > memory_ptr); /*assert verified*/
    //SCI_ASSERT(*((uint32*) (work_ptr + sizeof(NEXT_PTR))) != MEM_HEAP_BLOCK_FREE);
    heap_pool_info_ptr = (MMIMEM_HEAP_INFO*) *((MMIMEM_HEAP_INFO**)(work_ptr + sizeof(NEXT_PTR)));
    SCI_ASSERT(PNULL != heap_pool_info_ptr); /*assert verified*/
    
    for(i = 0; i < MMIMEM_MEM_MAX; i ++)
    {
        if(0 != s_mem_config_arr[i].manager_id)
        {
            if(0 != s_mem_config_arr[i].mem_total_size)
            {
                if(heap_pool_info_ptr == &s_mem_config_arr[i].heap_info)
                {
                    alloc_flag_ptr = (uint32*)(s_mem_config_arr[i].heap_info.heap_start_addr + s_mem_config_arr[i].heap_info.heap_size - 4);
                    SCI_ASSERT(*alloc_flag_ptr == MEM_HEAP_BLOCK_ALLOC); /*assert verified*/
                    is_heap_info_addr = TRUE;
                    break;
                }
            }
        }
    }
    SCI_ASSERT(is_heap_info_addr); /*assert verified*/
    SCI_ASSERT(heap_pool_info_ptr->heap_size >= (*((NEXT_PTR *) (work_ptr)) - memory_ptr));/*lint !e574*/ /*assert verified*/
    /* check next block pointer */

    next_work_ptr = *((NEXT_PTR *) (work_ptr));
    next_work_ptr = *((NEXT_PTR *) (work_ptr));
    heap_pool_info_ptr = (MMIMEM_HEAP_INFO*) *((MMIMEM_HEAP_INFO**)(next_work_ptr + sizeof(NEXT_PTR)));

    SCI_ASSERT(i < MMIMEM_MEM_MAX); /*assert verified*/
    if((heap_pool_info_ptr != &s_mem_config_arr[i].heap_info) 
        &&((uint32)heap_pool_info_ptr != MEM_HEAP_BLOCK_ALLOC)
        &&((uint32)heap_pool_info_ptr != MEM_HEAP_BLOCK_FREE))/*lint !e662*/
    {
        SCI_ASSERT(0); /*assert verified*/
    }

}
/*****************************************************************************/
//  Description:  release heap buffer
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void RealeaseHeapPool(uint8* memory_ptr)
{
    uint8 * work_ptr = PNULL;
    MMIMEM_HEAP_INFO * heap_pool_info_ptr = PNULL;
    
    SCI_ASSERT(PNULL != memory_ptr); /*assert verified*/
    /* Back off the memory pointer to pickup its header.  */
    work_ptr = memory_ptr - sizeof(NEXT_PTR) - sizeof(uint32);
    /* Pickup the pool pointer.  */
    SCI_ASSERT(*((uint32*) (work_ptr + sizeof(NEXT_PTR))) != MEM_HEAP_BLOCK_FREE); /*assert verified*/
    heap_pool_info_ptr = (MMIMEM_HEAP_INFO*) *((MMIMEM_HEAP_INFO**)(work_ptr + sizeof(NEXT_PTR)));
    SCI_ASSERT(PNULL != heap_pool_info_ptr); /*assert verified*/
	
    /* Release the memory */
    *((uint32*) (work_ptr + sizeof(NEXT_PTR))) =  MEM_HEAP_BLOCK_FREE;
    
    /* Update the number of available bytes in the pool.  */
    heap_pool_info_ptr->heap_available_size += (*((NEXT_PTR *) (work_ptr)) - memory_ptr);
        
}

/*****************************************************************************/
//  Description:  free buffer
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMEM_Free(uint8* memory_ptr)
{
    MMIMEM_CONFIG_T_PTR mem_config_ptr = PNULL;
    MMIMEM_POOL_INFO  * block_pool_info_ptr = PNULL;
    MMIMEM_HEAP_INFO  * heap_pool_info_ptr = PNULL;
    int16 array_i = 0;
    
    //SCI_TRACE_LOW:"[M]: free memory_ptr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_817_112_2_18_3_25_13_17,(uint8*)"d", memory_ptr);
    if(PNULL == memory_ptr)
    {
        return;
    }
    /* Determine witch mem the memory_ptr be in */
    for(array_i = 0; array_i < MMIMEM_MEM_MAX; array_i ++)
    {
        mem_config_ptr = &(s_mem_config_arr[array_i]);
        if(mem_config_ptr->manager_id > 0)
        {
            if((memory_ptr > mem_config_ptr->mem_start_addr) && ((uint32)(memory_ptr - mem_config_ptr->mem_start_addr) < mem_config_ptr->mem_total_size))
            {
                break;
            }
        }        
    }
    if(array_i >= MMIMEM_MEM_MAX)
    {        
        //SCI_TRACE_LOW:"[M]:free pointer failed: 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MEM_836_112_2_18_3_25_13_18,(uint8*)"d", memory_ptr);
        //SCI_ASSERT(0);
        /* this buffer dont belong to mem, so call system release function */
        //SCI_Free(memory_ptr);
        return;
    }
    else
    {
        /* check the memory_ptr is in heap or block pool */    
        mem_config_ptr = &(s_mem_config_arr[array_i]);
        block_pool_info_ptr = &(mem_config_ptr->all_pool_info);
        heap_pool_info_ptr = &(mem_config_ptr->heap_info);  
        
        /* check memory pointer belongs to heap pool*/
        if(memory_ptr > heap_pool_info_ptr->heap_start_addr)
        {
            CheckHeapPoolMemory(memory_ptr);
            RealeaseHeapPool(memory_ptr);
        }
        /* The memory pointer belongs to block pool. */
        else
        {
            RealeaseBlockPool(memory_ptr);
        }
    }

}
/*****************************************************************************/
//  Description:  destroy memory manager
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMEM_Destroy(int16 manager_id)
{
    MMIMEM_CONFIG_T_PTR mem_config_ptr = PNULL;
    MMIMEM_POOL_INFO  * block_pool_info_ptr = PNULL;
    MMIMEM_HEAP_INFO  * heap_pool_info_ptr = PNULL;
    uint16 i = 0;

    /* get mem config */
    mem_config_ptr = GetMemConfigFromManagerID(manager_id);
    SCI_ASSERT(PNULL != mem_config_ptr); /*assert verified*/

    block_pool_info_ptr = &(mem_config_ptr->all_pool_info);
    heap_pool_info_ptr = &(mem_config_ptr->heap_info);

    if(mem_config_ptr->manager_id > 0)
    {
        mem_config_ptr->manager_id = 0;    
        /* check block pool memory leak */
        for(i = 0; i < block_pool_info_ptr->pool_context_num; i ++)
        {
            if(block_pool_info_ptr->pool_context[i].available_block_num != block_pool_info_ptr->pool_context[i].block_type.num)
            {
                /* memory leak */
                SCI_PASSERT(0, ("block pool memory leak"));   /*assert verified*/
            }
        }
        /* check heap pool memory leak */
        if(heap_pool_info_ptr->heap_available_size != (heap_pool_info_ptr->heap_size - heap_pool_info_ptr->heap_fragments*(sizeof(NEXT_PTR)+sizeof(uint32))))
        {
            /* memory leak */
            SCI_PASSERT(0, ("heap pool memory leak"));        /*assert verified*/
        }
        SCI_MEMSET(mem_config_ptr, 0, sizeof(MMIMEM_CONFIG_T));
    }
    else
    {
        SCI_PASSERT(0, ("this mem is not used, manager_id is invalidate"));        /*assert verified*/
    }
}
#if 0
//#include "block_mem.h"
void MMIAPIMEM_test(void)
{
    uint8 * data_ptr = PNULL;
    uint32 data_size = 0;
    uint8 * buf_ptr[30] = {0};
    uint8 data_buf[1024*10] = {0};
    MMIMEM_BLOCK_TYPE block_type_arr[10] = {{10, 6}, {20, 4}, {30, 8}}; 
    int16 manager_id = 0;
    uint32 number = 15;
    uint32 size = 20*1024;
    int16 i = 0;
    return;
    if(1)//NOT_USE == BL_GetStatus(BLOCK_MEM_POOL_MULTIM))
    {
        data_ptr = data_buf;//BL_Malloc(BLOCK_MEM_POOL_MULTIM);
        data_size = 1024*10;//BL_GetSize(BLOCK_MEM_POOL_MULTIM);
        SCI_ASSERT(PNULL != data_ptr);        /*assert verified*/
        manager_id = MMIAPIMEM_Config(data_ptr, data_size, block_type_arr, 3);
        if(manager_id > 0)
        {            
            for(i = 0; i < number; i ++)
            {
                buf_ptr[i] = MMIAPIMEM_Malloc(manager_id, size);                
                if(PNULL != buf_ptr[i])
                {
                    SCI_MEMSET(buf_ptr[i], 0, size);
                }
            }
            
            for(i = 0; i < number; i ++)
            {
                MMIAPIMEM_Free(buf_ptr[i]);
            }

            for(i = 0; i < number*3; i ++)
            {
                buf_ptr[i] = MMIAPIMEM_Malloc(manager_id, size*1.2);                
                if(PNULL != buf_ptr[i])
                {
                    SCI_MEMSET(buf_ptr[i], 0, size);
                }
            }
            PrintfHeapLog(&(s_mem_config_arr[0].heap_info));
            for(i = 0; i < number; i ++)
            {
                MMIAPIMEM_Free(buf_ptr[i]);
            }

            PrintfHeapLog(&(s_mem_config_arr[0].heap_info));
            /*
            for(i = 0; i < 200; i ++)
            {
                buf_ptr[i] = MMIAPIMEM_Malloc(manager_id, 52);
            }
            for(i = 0; i < 200; i ++)
            {
                MMIAPIMEM_Free(buf_ptr[i]);
            }
            for(i = 0; i < 40; i ++)
            {
                buf_ptr[i] = MMIAPIMEM_Malloc(manager_id, 1024);
            }2
            for(i = 0; i < 40; i ++)
            {
                MMIAPIMEM_Free(buf_ptr[i]);
            }
            */
            MMIAPIMEM_Destroy(manager_id);
        }
    }
}
#endif
/*lint +e737*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

