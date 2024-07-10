#include "external_trc.h"
#include "fake.h"
#include "os_api.h"

#define SPLIBC_CUSTOM_POOL 1

#ifdef SPLIBC_CUSTOM_POOL

// memory
#define  SPLIBC_MEM_INFO		(sizeof(size_t))
static __inline size_t LIBC_MSize( void* memblock );

#define	 SPLIBC_POOL_SIZE		(20*1024*1024)
#define  SPLIBC_POOL_REGION		(120*1024)
#define  SPLIBC_MEM_MAX_SIZE	(5*1024*1024)

LOCAL uint32 *s_libc_pool_ptr = NULL;
LOCAL uint32 s_libc_pool_handle = 0;
LOCAL size_t s_libc_pool_alloc_count = 0;

/*******************************************************************/
LOCAL void LIBC_PoolFinaliz(BOOLEAN is_force)
{
	BOOLEAN is_free = FALSE;
	
	if (0 != s_libc_pool_handle)
	{
		if (0 == SCI_DeleteMemPool(s_libc_pool_handle, is_force))
		{
			s_libc_pool_handle = 0;
			is_free = TRUE;
		}
	}
	
	if (is_free 
		&& (0 != s_libc_pool_ptr))
	{	
		SCI_FREE(s_libc_pool_ptr);
		//SCI_TRACE_LOW:"LIBC_PoolFinaliz."
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,FAKE_MEMORY_38_112_2_18_0_22_31_0,(uint8*)"");
	}
}

/*******************************************************************/
//  Interface:		LIBC_PoolInital
//  Description : 	LIBC_PoolInital
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN LIBC_PoolInital()
{
	APP_MEM_CFG_T mem_cfg = {0};
	
	LIBC_PoolFinaliz(FALSE);
	
	s_libc_pool_ptr = SCI_ALLOCA(SPLIBC_POOL_SIZE);
	if (NULL == s_libc_pool_ptr)
	{
		return FALSE;
	}
	
	mem_cfg.min_pool_size = 16;
	mem_cfg.pool_space_size = SPLIBC_POOL_REGION;
	mem_cfg.pool_mode = MEM_POOL_RELEASE_MODE;
	s_libc_pool_handle = SCI_CreateMemPool("libc_pool", (uint8*)s_libc_pool_ptr,\
		SPLIBC_POOL_SIZE, &mem_cfg);
	
	//SCI_TRACE_LOW:"LIBC_PoolInital, s_libc_pool_handle=%d."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FAKE_MEMORY_65_112_2_18_0_22_31_1,(uint8*)"d",s_libc_pool_handle);
	
	return (0 != s_libc_pool_handle);
}

/*******************************************************************/
//  Interface:		SPLIBC_malloc
//  Description : 	SPLIBC_malloc
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
void *SPLIBC_malloc( size_t size )
{
	BOOLEAN is_pool = TRUE;
	size_t* mem_ptr = NULL;
	
	if (size > SPLIBC_MEM_MAX_SIZE)
	{
		is_pool = FALSE;
	}
	else if (0 == s_libc_pool_handle)
	{
		if (0 == s_libc_pool_alloc_count)
		{
			is_pool = LIBC_PoolInital();
		}
	}
	
	if (is_pool)
	{
		mem_ptr = SCI_MPALLOC(SPLIBC_MEM_INFO + size, s_libc_pool_handle);
		if (0 == mem_ptr)
		{
			//SCI_TRACE_LOW:"SPLIBC_malloc, pool is full, size=%d."
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,FAKE_MEMORY_97_112_2_18_0_22_31_2,(uint8*)"d", size);
			is_pool = FALSE;
		}
	}
	if (!is_pool)
	{
		mem_ptr = SCI_ALLOCA(SPLIBC_MEM_INFO + size);
	}
	
	SPLIBC_FAKE_LOG("SPLIBC_malloc size = %d \n", size);
	if(mem_ptr)
	{
		*mem_ptr++ = size;
		if (is_pool)
		{
			s_libc_pool_alloc_count += size;
		}	
	}
	
	return (void*)mem_ptr;
}

/*******************************************************************/
//  Interface:		SPLIBC_free
//  Description : 	SPLIBC_free
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
void SPLIBC_free( void *memblock )
{
	BOOLEAN is_pool = FALSE;
	uint8* pool_ptr = (uint8*)s_libc_pool_ptr;
	
	if (PNULL != s_libc_pool_ptr
		&& (uint8*)memblock >= (uint8*)pool_ptr
		&& (uint8*)memblock < (uint8*)(pool_ptr + SPLIBC_POOL_SIZE))
	{
		is_pool = TRUE;
	}
	
	if (is_pool)
	{
		SCI_MemPoolFree((uint8 *)memblock - SPLIBC_MEM_INFO);
		s_libc_pool_alloc_count -= LIBC_MSize(memblock);
		if (0 == s_libc_pool_alloc_count)
		{
			LIBC_PoolFinaliz(FALSE);
		}
	}
	else if	(PNULL != memblock)
	{
		SCI_Free((uint8 *)memblock - SPLIBC_MEM_INFO);
	}
	
	SPLIBC_FAKE_LOG("SPLIBC_free memblock = 0x%x \n", memblock);
}

void* SPLIBC_alloc(size_t size )
{
	void*  ptr = SPLIBC_malloc(size);
    if(NULL != ptr) {
	    SCI_MEMSET(ptr, 0, size);
    }

    return ptr;
}

/*******************************************************************/
//  Interface:		SPLIBC_calloc
//  Description : 	SPLIBC_calloc
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
void *SPLIBC_calloc( size_t num, size_t size )
{
	void * ptr = SPLIBC_malloc(num * size);
	SPLIBC_FAKE_LOG("SPLIBC_calloc num= %d,size = %d \n", num, size);
	SCI_MEMSET(ptr, 0, num * size);
	
	return ptr;
}

/*******************************************************************/
//  Interface:		LIBC_MSize
//  Description : 	LIBC_MSize
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
static __inline size_t LIBC_MSize( void* memblock )
{
	if( 0 == memblock )	
	{
		return 0;
	}
	
	return *(size_t*)((char *)memblock - SPLIBC_MEM_INFO);
}

/*******************************************************************/
//  Interface:		SPLIBC_realloc
//  Description : 	SPLIBC_realloc
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
void *SPLIBC_realloc( void *memblock, size_t size )
{
	void * new_mem_ptr = 0;
	size_t old_mem_size = 0;
	
	if(0 == size)
	{
		SPLIBC_free(memblock);
		return 0;
	}
	SPLIBC_FAKE_LOG("SPLIBC_realloc size = %d \n", size);
	if (0 == memblock)
	{
		return SPLIBC_malloc(size);
	}
	
	old_mem_size = LIBC_MSize(memblock);
	if (size <= old_mem_size)
	{
		new_mem_ptr = memblock;
	}
	else
	{
		new_mem_ptr = SPLIBC_malloc(size);
		if (0 != new_mem_ptr)
		{
			SCI_MEMSET(new_mem_ptr, 0, size);
			SCI_MEMCPY(new_mem_ptr, memblock, old_mem_size);
			SPLIBC_free(memblock);
		}
	}
	
	return new_mem_ptr; 
}

void* SPLIBC_aligned_malloc(size_t alignment, size_t size)
{
    void* ptr = SPLIBC_malloc(size);
    SPLIBC_FAKE_LOG("error: !!! SPRD_aligned_malloc size= %d,ptr= 0x%x \n", size, ptr);
    return ptr;
    //return CFL_MspaceAlignedMalloc(s_memHandle, alignment, size);
}

#else /*LIBC_CUSTOM_POOL*/

/*******************************************************************/
//  Interface:		SPLIBC_malloc
//  Description : 	SPLIBC_malloc
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
void *SPLIBC_malloc( size_t size )
{
    size_t* mem_ptr = (uint32*)SCI_ALLOCA(size);   
    SPLIBC_FAKE_LOG("SPLIBC_malloc size = %d \n", size);/*lint !e522*/
    
    return (void*)mem_ptr;
}

/*******************************************************************/
//  Interface:		SPLIBC_free
//  Description : 	SPLIBC_free
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
void SPLIBC_free( void *memblock )
{
    if (0 != memblock)
    {
        SCI_Free(memblock);
        SPLIBC_FAKE_LOG("SPLIBC_free memblock = 0x%x \n", memblock);/*lint !e522*/
        memblock = 0;
    }
}

/*******************************************************************/
//  Interface:		SPLIBC_calloc
//  Description : 	SPLIBC_calloc
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
void *SPLIBC_calloc( size_t num, size_t size )
{
     uint32 alloc_size = num*size;
    void* ptr = SCI_ALLOCAZ(alloc_size);    
    SPLIBC_FAKE_LOG("SPLIBC_calloc num= %d,size= %d,ptr= 0x%x \n", num, size, ptr);/*lint !e522*/
    
    return ptr;
}

/*******************************************************************/
//  Interface:		SPLIBC_realloc
//  Description : 	SPLIBC_realloc
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
void *SPLIBC_realloc( void *memblock, size_t size )
{
    void* new_ptr = SCI_REALLOC(memblock, size);    
    SPLIBC_FAKE_LOG("SPLIBC_realloc size= %d, memblock= 0x%x, new_ptr= 0x%x \n", size, memblock, new_ptr);/*lint !e522*/
    
    return new_ptr; 
}

#endif /*LIBC_CUSTOM_POOL*/



