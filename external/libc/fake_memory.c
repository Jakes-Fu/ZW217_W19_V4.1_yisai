#include "fake.h"
#include "os_api.h"
#include "cfl_dlmalloc.h"

//#define SPLIBC_CUSTOM_POOL 1

#ifdef SPLIBC_CUSTOM_POOL

#define SPLIBC_MEM_POOL_SIZE         (10*1024*1024) //20M

static void*             s_memBuffer = NULL;
static CFL_MSPACE_HANDLE s_memHandle = NULL;

#define  LIBC_MEM_INFO		(sizeof(size_t))

int SPLIBC_Init()
{
    s_memBuffer = SCI_Malloc(SPLIBC_MEM_POOL_SIZE, __FILE__, __LINE__);
    if(NULL == s_memBuffer) {
        SCI_TRACE_LOW("[libc error] func:%s, memBuffer:%p", __FUNCTION__, s_memBuffer);
        return 0;
    }

    s_memHandle = CFL_CreateMspace(s_memBuffer, SPLIBC_MEM_POOL_SIZE);
    if(NULL == s_memHandle) {
        SCI_TRACE_LOW("[libc error] func:%s, memHandle:%p", __FUNCTION__, s_memHandle);
        SPLIBC_Finaliz();
        return 0;
    }

    return 1;
}

void SPLIBC_Finaliz()
{
    if(NULL != s_memHandle) {
        CFL_DestroyMspace(s_memHandle);
        s_memHandle = SCI_NULL;
    }

    if(NULL != s_memBuffer) {
        SCI_FREE(s_memBuffer);
        s_memBuffer = NULL;
    }
}

void* SPLIBC_malloc( size_t size )
{
    if (s_memHandle == NULL)
    {
        SPLIBC_Init();
    }
    return CFL_MspaceMalloc(s_memHandle, size);
}
   
void SPLIBC_free( void* memblock )
{
    if(memblock)
        CFL_MspaceFree(s_memHandle, memblock);
}

void* SPLIBC_calloc( size_t n_elements, size_t elem_size )
{
    size_t size = n_elements * elem_size;
    void*  ptr = NULL;   
    if (s_memHandle == NULL)
    {
        SPLIBC_Init();
    }
    
    ptr = CFL_MspaceMalloc(s_memHandle, size);
    if(NULL != ptr) {
	    SCI_MEMSET(ptr, 0, size);
    }

	return ptr;
}

size_t _MSize( void* memblock )
{
	if( 0 == memblock )	
	{
		return 0;
	}

	return *(size_t*)((char *)memblock - LIBC_MEM_INFO);
}

void* SPLIBC_realloc( void* memblock, size_t size )
{
    if (s_memHandle == NULL)
    {
        SPLIBC_Init();
    }

    return CFL_MspaceRealloc(s_memHandle, memblock, size);
}

void* alloc( size_t size )
{
	return SPLIBC_alloc(size);
}


void* SPLIBC_alloc( size_t size )
{
    void*  ptr = NULL;
    if (s_memHandle == NULL)
    {
        SPLIBC_Init();
    }

    ptr = CFL_MspaceMalloc(s_memHandle, size);
    if(NULL != ptr) {
	    SCI_MEMSET(ptr, 0, size);
    }

    return ptr;
}

void* SPLIBC_aligned_malloc(size_t alignment, size_t size)
{
    if (s_memHandle == NULL)
    {
        SPLIBC_Init();
    }

    return CFL_MspaceAlignedMalloc(s_memHandle, alignment, size);
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
    size_t* mem_ptr = NULL;
    mem_ptr = (uint32*)SCI_ALLOCA(size);   
    SCI_TRACE_LOW("SPLIBC_malloc size = %d \n", size);/*lint !e522*/
    
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
        SCI_TRACE_LOW("SPLIBC_free memblock = 0x%x \n", memblock);/*lint !e522*/
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
    void* ptr = NULL;
    ptr = SCI_ALLOCAZ(alloc_size);
    SCI_TRACE_LOW("SPLIBC_calloc num= %d,size= %d,ptr= 0x%x \n", num, size, ptr);/*lint !e522*/
    
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
    void* new_ptr = NULL;
    new_ptr = SCI_REALLOC(memblock, size);    
    SCI_TRACE_LOW("SPLIBC_realloc size= %d, memblock= 0x%x, new_ptr= 0x%x \n", size, memblock, new_ptr);/*lint !e522*/
    
    return new_ptr; 
}

#endif /*LIBC_CUSTOM_POOL*/

char* SPLIBC_strdup(const char *s)
{
    uint32 strLen = 0;
    char*  dupstr = NULL;

    if (s == NULL)
        return NULL;

    strLen = strlen(s);

    dupstr = SPLIBC_malloc(strLen+1);

    if (dupstr == NULL)
        return NULL;
    SCI_MEMSET(dupstr, 0, strLen+1);
    SCI_MEMCPY(dupstr, s, strLen);
    
    return dupstr;
}

