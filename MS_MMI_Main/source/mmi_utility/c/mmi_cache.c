/******************************************************************************
 ** File Name:      mmi_cache.h                                               *
 ** Author:         cheng.luo                                              *
 ** DATE:           01/20/2012                                                *
 ** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 general cache											 *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/20/2012     cheng.luo        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_utility_trc.h"
#include "mmi_cache.h"
#include "mmk_hash.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define CACHE_MAGIC_NUMBER      0x95ba6fec
#define IS_CACHE_PTR(ptr)      (CACHE_MAGIC_NUMBER == (ptr)->magic)       
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct 
{
    uint32			key_id;
    uint32          reserve;
//    uint8*			data1_ptr;
    uint32          data1_len;
//    uint8*	        data2_ptr;
    uint32			data2_len;
    
    UT_hash_handle	hh;
}MMI_CACHE_INT_KEY_HASH_ITEM_T;

typedef struct  
{
	char*			key_name;
    uint32          reserve;
//    uint8*			data1_ptr;
    uint32          data1_len;
//    uint8*	        data2_ptr;
    uint32			data2_len;
	
	UT_hash_handle	hh;
}MMI_CACHE_STR_KEY_HASH_ITEM_T;

typedef struct 
{
    void*           key_ptr;
    uint32          key_len;

    uint32          reserve;
    uint32          data1_len;
    uint32          data2_len;

    UT_hash_handle	hh;
}MMI_CACHE_PTR_KEY_HASH_ITEM_T;

typedef	union
{
	MMI_CACHE_INT_KEY_HASH_ITEM_T*	int_items;
	MMI_CACHE_STR_KEY_HASH_ITEM_T*	str_items;
    MMI_CACHE_PTR_KEY_HASH_ITEM_T*  ptr_items;
}MMI_CACHE_ITEMS_U;

struct MMI_CACHE_INFO_T
{
    uint32                  magic;
	const char*				name;
	uint32					size;
	MMI_CACHE_KEY_TYPE_E	type;

	uint32				pool_handle;
	MMI_CACHE_ITEMS_U	lst;
};
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                                                             */
/*---------------------------------------------------------------------------*/
/*******************************************************************/
//  Interface:		MMICACHE_Malloc
//  Description : 	cache和hash的内存分配，lparam为NULL，从heap分；非NULL，从pool分
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
// PUBLIC void* MMICACHE_Malloc(
//                              void       *lparam, 
//                              uint32     size
//                              );

/*******************************************************************/
//  Interface:		MMICACHE_Free
//  Description : 	MMICACHE_Free
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
// PUBLIC void MMICACHE_Free(
//                           void      *lparam, 
//                           void      *addr_ptr, 
//                           uint32    size
//                           );

/*lint -save -w1*/

/*******************************************************************/
//  Interface:		GetCacheCount
//  Description : 	GetCacheCount
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL uint32 GetCacheCount(
                           MMI_CACHE_INFO_T*   cache_ptr
                           )
{
    uint32 count = 0;
    
    if (PNULL != cache_ptr && IS_CACHE_PTR(cache_ptr))
    {
        if (CKT_INT == cache_ptr->type 
            && PNULL != cache_ptr->lst.int_items)
        {
            count = HASH_COUNT(cache_ptr->lst.int_items);
        }
        else if (CKT_STRING == cache_ptr->type
            && PNULL != cache_ptr->lst.str_items)
        {
            count = HASH_COUNT(cache_ptr->lst.str_items);
        } 
        else if (CKT_POINTER == cache_ptr->type
            && PNULL != cache_ptr->lst.ptr_items)
        {
            count = HASH_COUNT(cache_ptr->lst.ptr_items);
        } 
    }
    
    return count;
}

/*******************************************************************/
//  Interface:		CacheRecycle
//  Description : 	cache回收
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void CacheRecycle(
                        MMI_CACHE_INFO_T*   cache_ptr
                        )
{
    if (PNULL != cache_ptr && IS_CACHE_PTR(cache_ptr))
    {
        if (CKT_INT == cache_ptr->type 
            && PNULL != cache_ptr->lst.int_items)
        {
            MMI_CACHE_INT_KEY_HASH_ITEM_T   *cur_item_ptr = cache_ptr->lst.int_items;
            HASH_DEL(cache_ptr->lst.int_items, cur_item_ptr, cache_ptr);
            MMICACHE_Free(cache_ptr, cur_item_ptr, \
                sizeof(MMI_CACHE_INT_KEY_HASH_ITEM_T) + cur_item_ptr->data1_len + cur_item_ptr->data2_len);
        }
        else if (CKT_STRING == cache_ptr->type
            && PNULL != cache_ptr->lst.str_items)
        {
            MMI_CACHE_STR_KEY_HASH_ITEM_T   *cur_item_ptr = cache_ptr->lst.str_items;
            HASH_DEL(cache_ptr->lst.str_items, cur_item_ptr, cache_ptr);
            MMICACHE_Free(cache_ptr, cur_item_ptr, \
                sizeof(MMI_CACHE_STR_KEY_HASH_ITEM_T) + cur_item_ptr->data1_len + cur_item_ptr->data2_len);
        } 
        else if (CKT_POINTER == cache_ptr->type
            && PNULL != cache_ptr->lst.ptr_items)
        {
            MMI_CACHE_PTR_KEY_HASH_ITEM_T   *cur_item_ptr = cache_ptr->lst.ptr_items;
            HASH_DEL(cache_ptr->lst.ptr_items, cur_item_ptr, cache_ptr);
            MMICACHE_Free(cache_ptr, cur_item_ptr, \
                sizeof(MMI_CACHE_PTR_KEY_HASH_ITEM_T) + cur_item_ptr->key_len + cur_item_ptr->data1_len + cur_item_ptr->data2_len);
        } 
    }
}

/*******************************************************************/
//  Interface:		MMICACHE_Malloc
//  Description : 	cache和hash的内存分配，lparam为NULL，从heap分；非NULL，从pool分
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void* MMICACHE_Malloc(
                             void       *lparam, 
                             uint32     size
                             )
{
    void *mem_ptr = PNULL;
    uint32 count = 0;
    
    if (size > 0)
    {
        if (PNULL != lparam)
        {
            // in pool
            MMI_CACHE_INFO_T   *cache_ptr = (MMI_CACHE_INFO_T*)lparam;
            if (IS_CACHE_PTR(cache_ptr) && PNULL != cache_ptr->pool_handle)
            {
                do 
                {
                    mem_ptr = SCI_MPALLOC(size, cache_ptr->pool_handle);
                    count = GetCacheCount(cache_ptr);
                    if (PNULL != mem_ptr 
                        || 0 == count)
                    {
                        //SCI_TRACE_LOW:"@@Cache count = %d."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_CACHE_152_112_2_18_3_25_6_0,(uint8*)"d", count);
                        break;
                    }
                    
                    CacheRecycle(cache_ptr);
                } while(1);
            }
        }
        else
        {
            // in heap
            mem_ptr = SCI_ALLOCA(size);
        }     
    }
    
    // if sucess,init;
    if(PNULL != mem_ptr)
    {
        SCI_MEMSET(mem_ptr, 0, size);
    }
    
    return mem_ptr;
}

/*******************************************************************/
//  Interface:		MMICACHE_Free
//  Description : 	MMICACHE_Free
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMICACHE_Free(
                          void      *lparam, 
                          void      *addr_ptr, 
                          uint32    size
                          )
{
    if (PNULL != addr_ptr && 0 < size)
    {
        if (PNULL != lparam)
        {
            SCI_MPFREE(addr_ptr);
        }
        else
        {
            SCI_FREE(addr_ptr);
        }
    }
}

/*******************************************************************/
//  Interface:		MMICACHE_Init
//  Description : 	MMICACHE_Init
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC MMI_CACHE_INFO_T* MMICACHE_Init(
									   const char			*name_ptr,
									   uint32				size,
									   MMI_CACHE_KEY_TYPE_E type
									   )
{
    MMI_CACHE_INFO_T	*cache_ptr = PNULL;
    uint8               *pool_addr = PNULL; 
    APP_MEM_CFG_T       mem_cfg = {0};
    
    if (PNULL == name_ptr)
    {
        return PNULL;
    }
    
    cache_ptr = (MMI_CACHE_INFO_T*)SCI_ALLOCA(sizeof(MMI_CACHE_INFO_T) + size);
    if (PNULL == cache_ptr)
    {
        return PNULL;
    }
    
    SCI_MEMSET(cache_ptr, 0, sizeof(MMI_CACHE_INFO_T) + size);
    
    cache_ptr->magic = CACHE_MAGIC_NUMBER;
    pool_addr = (uint8*)cache_ptr + sizeof(MMI_CACHE_INFO_T);
    mem_cfg.pool_mode = MEM_POOL_RELEASE_MODE;
    cache_ptr->pool_handle = SCI_CreateMemPool(name_ptr, pool_addr, size, &mem_cfg);
    
    if (0 != cache_ptr->pool_handle)
    {
        cache_ptr->name = name_ptr;
        cache_ptr->size = size;
        cache_ptr->type = type;
    }
    else
    {
        SCI_FREE(cache_ptr);
    }
    
    return cache_ptr;
}

/*******************************************************************/
//  Interface:		MMICACHE_Destroy
//  Description : 	MMICACHE_Destroy
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMICACHE_Destroy(
							 MMI_CACHE_INFO_T	*cache_ptr
							 )
{
    if (PNULL != cache_ptr && IS_CACHE_PTR(cache_ptr))
    {      
        SCI_DeleteMemPool(cache_ptr->pool_handle, TRUE);
        cache_ptr->magic = 0x0;
        SCI_FREE(cache_ptr);
    }
}

/*******************************************************************/
//  Interface:		MMICACHE_RemoveItemByInt
//  Description : 	MMICACHE_RemoveItemByInt
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_RemoveItemByInt(
                                        MMI_CACHE_INFO_T			*cache_ptr,
                                        uint32						key_id
                                        )
{
    MMI_CACHE_INT_KEY_HASH_ITEM_T   *cur_item_ptr = PNULL;
    
    if (PNULL == cache_ptr
        || ! IS_CACHE_PTR(cache_ptr)
        || CKT_INT != cache_ptr->type)
    {
        return FALSE;
    }
    
    HASH_FIND_INT(cache_ptr->lst.int_items, &key_id, cur_item_ptr); 
    if (PNULL != cur_item_ptr)
    {
        HASH_DEL(cache_ptr->lst.int_items, cur_item_ptr, cache_ptr);
        MMICACHE_Free(cache_ptr, cur_item_ptr, \
            sizeof(MMI_CACHE_INT_KEY_HASH_ITEM_T) + cur_item_ptr->data1_len + cur_item_ptr->data2_len);
        cur_item_ptr = PNULL;
    }
    
    return TRUE;
}

/*******************************************************************/
//  Interface:		MMICACHE_PutItemByInt
//  Description : 	MMICACHE_PutItemByInt
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_PutItemByInt(
                                     MMI_CACHE_INFO_T					*cache_ptr,
                                     const MMI_CACHE_INT_KEY_ITEM_T	    *in_ptr,
                                     BOOLEAN                            is_update
                                     )
{
    MMI_CACHE_INT_KEY_HASH_ITEM_T   *cur_item_ptr = PNULL;
    uint32  cur_key_id = in_ptr->key_id;
    BOOLEAN ret = FALSE;
    
    if (PNULL == cache_ptr || PNULL == in_ptr
        || ! IS_CACHE_PTR(cache_ptr)
        || CKT_INT != cache_ptr->type)
    {
        return FALSE;
    }
    
    HASH_FIND_INT(cache_ptr->lst.int_items, &cur_key_id, cur_item_ptr); 
    if (PNULL != cur_item_ptr && !is_update)
    {
        return FALSE;
    }
    
    if (PNULL != cur_item_ptr)
    {
        // remove old first
        HASH_DEL(cache_ptr->lst.int_items, cur_item_ptr, cache_ptr);
        MMICACHE_Free(cache_ptr, cur_item_ptr, \
            sizeof(MMI_CACHE_INT_KEY_HASH_ITEM_T) + cur_item_ptr->data1_len + cur_item_ptr->data2_len);
        cur_item_ptr = PNULL;
    }
    
    // add new
    cur_item_ptr = MMICACHE_Malloc(cache_ptr, \
        sizeof(MMI_CACHE_INT_KEY_HASH_ITEM_T) + in_ptr->data1_len + in_ptr->data2_len);
    if (PNULL != cur_item_ptr)
    {
        cur_item_ptr->key_id = in_ptr->key_id;
        cur_item_ptr->reserve = in_ptr->reserve;
        if (PNULL != in_ptr->data1_ptr && in_ptr->data1_len > 0)
        {
            uint8* data1_ptr = (uint8*)cur_item_ptr + sizeof(MMI_CACHE_INT_KEY_HASH_ITEM_T);
            cur_item_ptr->data1_len = in_ptr->data1_len;
            SCI_MEMCPY(data1_ptr, in_ptr->data1_ptr, in_ptr->data1_len);
        }      
        if (PNULL != in_ptr->data2_ptr && in_ptr->data2_len > 0)
        {
            uint8* data2_ptr = (uint8*)cur_item_ptr + sizeof(MMI_CACHE_INT_KEY_HASH_ITEM_T) \
                + cur_item_ptr->data1_len;
            cur_item_ptr->data2_len = in_ptr->data2_len;
            SCI_MEMCPY(data2_ptr, in_ptr->data2_ptr, in_ptr->data2_len);
        }
        
        HASH_ADD_INT(cache_ptr->lst.int_items, key_id, cur_item_ptr, cache_ptr);
        ret = TRUE;
    }
    
    return ret;
}

/*******************************************************************/
//  Interface:		MMICACHE_RemoveItemByString
//  Description : 	MMICACHE_RemoveItemByString
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_RemoveItemByString(
                                           MMI_CACHE_INFO_T		*cache_ptr,
                                           const char			*key_name
                                           )
{
    MMI_CACHE_STR_KEY_HASH_ITEM_T   *cur_item_ptr = PNULL;
    
    if (PNULL == cache_ptr
        || ! IS_CACHE_PTR(cache_ptr)
        || CKT_STRING != cache_ptr->type)
    {
        return FALSE;
    }
    
    HASH_FIND_STR(cache_ptr->lst.str_items, key_name, cur_item_ptr); 
    if (PNULL != cur_item_ptr)
    {
        // remove old first
        HASH_DEL(cache_ptr->lst.str_items, cur_item_ptr, cache_ptr);
        MMICACHE_Free(cache_ptr, cur_item_ptr, \
            sizeof(MMI_CACHE_STR_KEY_HASH_ITEM_T) + cur_item_ptr->data1_len + cur_item_ptr->data2_len);
        cur_item_ptr = PNULL;
    }
    
    return TRUE;
}

/*******************************************************************/
//  Interface:		MMICAHCE_GetItemByInt
//  Description : 	MMICAHCE_GetItemByInt
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_GetItemByInt(
									 MMI_CACHE_INFO_T			*cache_ptr,
									 uint32						key_id,
									 MMI_CACHE_INT_KEY_ITEM_T	*out_ptr
									 )
{
    MMI_CACHE_INT_KEY_HASH_ITEM_T   *cur_item_ptr = PNULL;
    BOOLEAN ret = FALSE;
      
    if (PNULL == cache_ptr || PNULL == out_ptr
        || ! IS_CACHE_PTR(cache_ptr)
        || CKT_INT != cache_ptr->type)
    {
        return FALSE;
    }
    
    HASH_FIND_INT(cache_ptr->lst.int_items, &key_id, cur_item_ptr);
    if (PNULL != cur_item_ptr)
    {
        //调整位置,删除后重新加入
        if (PNULL != cur_item_ptr->hh.next)
        {
            HASH_DEL(cache_ptr->lst.int_items, cur_item_ptr, cache_ptr);
            HASH_ADD_INT(cache_ptr->lst.int_items, key_id, cur_item_ptr, cache_ptr);
        }
        
        out_ptr->key_id = cur_item_ptr->key_id;
        out_ptr->reserve = cur_item_ptr->reserve;
        if( 0 < (out_ptr->data1_len = cur_item_ptr->data1_len))
        {
            out_ptr->data1_ptr = (uint8*)cur_item_ptr + sizeof(MMI_CACHE_INT_KEY_HASH_ITEM_T);
        }
        
        if( 0 < (out_ptr->data2_len = cur_item_ptr->data2_len))
        {
            out_ptr->data2_ptr = (uint8*)cur_item_ptr + sizeof(MMI_CACHE_INT_KEY_HASH_ITEM_T) \
                + cur_item_ptr->data1_len;
        }
        
        ret = TRUE;
    }
    
    return ret;
}

/*******************************************************************/
//  Interface:		MMICACHE_PutItemByString
//  Description : 	MMICACHE_PutItemByString
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_PutItemByString(
                                        MMI_CACHE_INFO_T					*cache_ptr,
                                        const MMI_CACHE_STR_KEY_ITEM_T	    *in_ptr,
                                        BOOLEAN                             is_update
                                        )
{
    MMI_CACHE_STR_KEY_HASH_ITEM_T   *cur_item_ptr = PNULL;
    BOOLEAN ret = FALSE;
    
    if (PNULL == cache_ptr || PNULL == in_ptr
        || ! IS_CACHE_PTR(cache_ptr)
        || CKT_STRING != cache_ptr->type)
    {
        return FALSE;
    }
    
    HASH_FIND_STR(cache_ptr->lst.str_items, in_ptr->key_name, cur_item_ptr); 
    if (PNULL != cur_item_ptr && !is_update)
    {
        return FALSE;
    }
    
    if (PNULL != cur_item_ptr)
    {
        // remove old first
        HASH_DEL(cache_ptr->lst.str_items, cur_item_ptr, cache_ptr);
        MMICACHE_Free(cache_ptr, cur_item_ptr, \
            sizeof(MMI_CACHE_STR_KEY_HASH_ITEM_T) + cur_item_ptr->data1_len + cur_item_ptr->data2_len);
        cur_item_ptr = PNULL;
    }
    
    // add new
    cur_item_ptr = MMICACHE_Malloc(cache_ptr, \
        sizeof(MMI_CACHE_STR_KEY_HASH_ITEM_T) + in_ptr->data1_len + in_ptr->data2_len);
    if (PNULL != cur_item_ptr)
    {
        cur_item_ptr->key_name = in_ptr->key_name;
        cur_item_ptr->reserve = in_ptr->reserve;
        
        if (PNULL != in_ptr->data1_ptr && in_ptr->data1_len > 0)
        {
            uint8* data1_ptr = (uint8*)cur_item_ptr + sizeof(MMI_CACHE_STR_KEY_HASH_ITEM_T);
            cur_item_ptr->data1_len = in_ptr->data1_len;
            SCI_MEMCPY(data1_ptr, in_ptr->data1_ptr, in_ptr->data1_len);
        }
        
        if (PNULL != in_ptr->data2_ptr && in_ptr->data2_len > 0)
        {
            uint8* data2_ptr = (uint8*)cur_item_ptr + sizeof(MMI_CACHE_STR_KEY_HASH_ITEM_T) \
                + cur_item_ptr->data1_len;
            cur_item_ptr->data2_len = in_ptr->data2_len;
            SCI_MEMCPY(data2_ptr, in_ptr->data2_ptr, in_ptr->data2_len);
        }
        
        HASH_ADD_KEYPTR(hh, cache_ptr->lst.str_items, cur_item_ptr->key_name, \
            strlen(cur_item_ptr->key_name), cur_item_ptr, cache_ptr);
        ret = TRUE;
    }
    
    return ret;   
}

/*******************************************************************/
//  Interface:		MMICACHE_GetItemByString
//  Description : 	MMICACHE_GetItemByString
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_GetItemByString(
                                        MMI_CACHE_INFO_T		*cache_ptr,
                                        const char					*key_name,
                                        MMI_CACHE_STR_KEY_ITEM_T	*out_ptr
                                        )
{
    MMI_CACHE_STR_KEY_HASH_ITEM_T   *cur_item_ptr = PNULL;
    BOOLEAN ret = FALSE;
    
    if (PNULL == cache_ptr || PNULL == out_ptr
        || ! IS_CACHE_PTR(cache_ptr)
        || CKT_STRING != cache_ptr->type)
    {
        return FALSE;
    }
    
    HASH_FIND_STR(cache_ptr->lst.str_items, key_name, cur_item_ptr);
    if (PNULL != cur_item_ptr)
    {
        //调整位置,删除后重新加入
        if (PNULL != cur_item_ptr->hh.next)
        {
            HASH_DEL(cache_ptr->lst.str_items, cur_item_ptr, cache_ptr);
            HASH_ADD_KEYPTR(hh, cache_ptr->lst.str_items, cur_item_ptr->key_name, \
                strlen(cur_item_ptr->key_name), cur_item_ptr, cache_ptr);
        }
        
        out_ptr->key_name = cur_item_ptr->key_name;
        out_ptr->reserve = cur_item_ptr->reserve;
        if( 0 < (out_ptr->data1_len = cur_item_ptr->data1_len))
        {
            out_ptr->data1_ptr = (uint8*)cur_item_ptr + sizeof(MMI_CACHE_STR_KEY_HASH_ITEM_T);
        }
        
        if( 0 < (out_ptr->data2_len = cur_item_ptr->data2_len))
        {
            out_ptr->data2_ptr = (uint8*)cur_item_ptr + sizeof(MMI_CACHE_STR_KEY_HASH_ITEM_T) \
                + cur_item_ptr->data1_len;
        }
        
        ret = TRUE;
    }
    
    return ret;
}

/*******************************************************************/
//  Interface:		MMICACHE_RemoveItemByPtr
//  Description : 	MMICACHE_RemoveItemByPtr
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_RemoveItemByPtr(
                                        MMI_CACHE_INFO_T			*cache_ptr,
                                        void                        *key_ptr,
                                        uint32                      key_len
                                        )
{
    MMI_CACHE_PTR_KEY_HASH_ITEM_T   *cur_item_ptr = PNULL;
    
    if (PNULL == cache_ptr
        || ! IS_CACHE_PTR(cache_ptr)
        || CKT_POINTER != cache_ptr->type)
    {
        return FALSE;
    }
    
    HASH_FIND(hh, cache_ptr->lst.ptr_items, key_ptr, key_len, cur_item_ptr); 
    if (PNULL != cur_item_ptr)
    {
        HASH_DEL(cache_ptr->lst.ptr_items, cur_item_ptr, cache_ptr);
        MMICACHE_Free(cache_ptr, cur_item_ptr, \
            sizeof(MMI_CACHE_PTR_KEY_HASH_ITEM_T) + cur_item_ptr->key_len + cur_item_ptr->data1_len + cur_item_ptr->data2_len);
        cur_item_ptr = PNULL;
    }
    
    return TRUE;
}

/*******************************************************************/
//  Interface:		MMICACHE_PutItemByPtr
//  Description : 	MMICACHE_PutItemByPtr
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_PutItemByPtr(
                                        MMI_CACHE_INFO_T					*cache_ptr,
                                        const MMI_CACHE_PTR_KEY_ITEM_T	    *in_ptr,
                                        BOOLEAN                             is_update
                                        )
{
    MMI_CACHE_PTR_KEY_HASH_ITEM_T   *cur_item_ptr = PNULL;
    BOOLEAN ret = FALSE;
    
    if (PNULL == cache_ptr || PNULL == in_ptr
        || ! IS_CACHE_PTR(cache_ptr)
        || CKT_POINTER != cache_ptr->type)
    {
        return FALSE;
    }
    
    HASH_FIND(hh, cache_ptr->lst.ptr_items, in_ptr->key_ptr, in_ptr->key_len, cur_item_ptr); 
    if (PNULL != cur_item_ptr && !is_update)
    {
        return FALSE;
    }
    
    if (PNULL != cur_item_ptr)
    {
        // remove old first
        HASH_DEL(cache_ptr->lst.ptr_items, cur_item_ptr, cache_ptr);
        MMICACHE_Free(cache_ptr, cur_item_ptr, \
            sizeof(MMI_CACHE_PTR_KEY_HASH_ITEM_T) + cur_item_ptr->key_len + cur_item_ptr->data1_len + cur_item_ptr->data2_len);
        cur_item_ptr = PNULL;
    }
    
    // add new
    cur_item_ptr = MMICACHE_Malloc(cache_ptr, \
        sizeof(MMI_CACHE_PTR_KEY_HASH_ITEM_T) + in_ptr->key_len + in_ptr->data1_len + in_ptr->data2_len);
    if (PNULL != cur_item_ptr)
    {    
        cur_item_ptr->key_ptr = (void*)((uint8*)cur_item_ptr + sizeof(MMI_CACHE_PTR_KEY_HASH_ITEM_T));
        cur_item_ptr->key_len = in_ptr->key_len;
        cur_item_ptr->reserve = in_ptr->reserve;
        
        SCI_MEMCPY(cur_item_ptr->key_ptr, in_ptr->key_ptr, in_ptr->key_len);
        if (PNULL != in_ptr->data1_ptr && in_ptr->data1_len > 0)
        {
            uint8* data1_ptr = (uint8*)cur_item_ptr + in_ptr->key_len + sizeof(MMI_CACHE_PTR_KEY_HASH_ITEM_T);
            cur_item_ptr->data1_len = in_ptr->data1_len;
            SCI_MEMCPY(data1_ptr, in_ptr->data1_ptr, in_ptr->data1_len);
        }
        
        if (PNULL != in_ptr->data2_ptr && in_ptr->data2_len > 0)
        {
            uint8* data2_ptr = (uint8*)cur_item_ptr + sizeof(MMI_CACHE_PTR_KEY_HASH_ITEM_T) \
                + in_ptr->key_len + cur_item_ptr->data1_len;
            cur_item_ptr->data2_len = in_ptr->data2_len;
            SCI_MEMCPY(data2_ptr, in_ptr->data2_ptr, in_ptr->data2_len);
        }
        
        HASH_ADD_KEYPTR(hh, cache_ptr->lst.ptr_items, cur_item_ptr->key_ptr, \
            cur_item_ptr->key_len, cur_item_ptr, cache_ptr);
        ret = TRUE;
    }
    
    return ret;   
}

/*******************************************************************/
//  Interface:		MMICACHE_GetItemByPtr
//  Description : 	MMICACHE_GetItemByPtr
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_GetItemByPtr(
                                        MMI_CACHE_INFO_T		*cache_ptr,
                                        const void				*key_ptr,
                                        uint32                      key_len,
                                        MMI_CACHE_PTR_KEY_ITEM_T	*out_ptr
                                        )
{
    MMI_CACHE_PTR_KEY_HASH_ITEM_T   *cur_item_ptr = PNULL;
    BOOLEAN ret = FALSE;
    
    if (PNULL == cache_ptr || PNULL == out_ptr
        || ! IS_CACHE_PTR(cache_ptr)
        || CKT_POINTER != cache_ptr->type)
    {
        return FALSE;
    }
    
    HASH_FIND(hh, cache_ptr->lst.ptr_items, key_ptr, key_len, cur_item_ptr);
    if (PNULL != cur_item_ptr)
    {
        //调整位置,删除后重新加入
        if (PNULL != cur_item_ptr->hh.next)
        {
            HASH_DEL(cache_ptr->lst.ptr_items, cur_item_ptr, cache_ptr);
            HASH_ADD_KEYPTR(hh, cache_ptr->lst.ptr_items, cur_item_ptr->key_ptr, \
                cur_item_ptr->key_len, cur_item_ptr, cache_ptr);
        }
        
        out_ptr->key_ptr = cur_item_ptr->key_ptr;
        out_ptr->key_len = cur_item_ptr->key_len;
        out_ptr->reserve = cur_item_ptr->reserve;
        if( 0 < (out_ptr->data1_len = cur_item_ptr->data1_len))
        {
            out_ptr->data1_ptr = (uint8*)cur_item_ptr + cur_item_ptr->key_len + sizeof(MMI_CACHE_PTR_KEY_HASH_ITEM_T);
        }
        
        if( 0 < (out_ptr->data2_len = cur_item_ptr->data2_len))
        {
            out_ptr->data2_ptr = (uint8*)cur_item_ptr + cur_item_ptr->key_len + sizeof(MMI_CACHE_PTR_KEY_HASH_ITEM_T) \
                + cur_item_ptr->data1_len;
        }
        
        ret = TRUE;
    }
    
    return ret;
}

/*lint -restore */
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

