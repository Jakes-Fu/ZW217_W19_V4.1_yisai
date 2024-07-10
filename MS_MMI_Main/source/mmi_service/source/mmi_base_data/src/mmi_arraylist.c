/********************************************************************************
** File Name:   mmi_arraylist.c                                                 *
** Author:      Hongbing Ju                                                     *
** Date:        06/25/2012                                                      *
** Copyright:   2003 Spreadtrum Communications Incorporated. All Right Reserved *
** Description: This file is used to describe a base data structure of doubly   *
**              linked list                                                     *
*********************************************************************************
**                            Important Edit history                            *
**-----------------------------------------------------------------------------**
** DATE            NAME            DESCRIPTION                                  *
** 06/25/2012      Hongbing Ju     Created                                      *
*********************************************************************************/

/*------------------------------------------------------------------------------*
**                            Included Files                                    *
**------------------------------------------------------------------------------*/
#include "os_api.h"
#include "mmi_arraylist.h"
#include "mmisrv_adapt.h"
/*
 * Interface section used for `makeheaders`.
 */

typedef struct MMI_ALIST_Tag 
{
    void**        element;    // Pointer to allocated memory for items (of size capacity * sizeof(void*))
    int32         error;
    uint32        size;       // Count of items currently in list
    uint32        capacity;   // Allocated memory size, in items
#ifdef DEBUG
    const char    *id;
#endif
} MMI_ALIST_T;

/*------------------------------------------------------------------------------*
**                            MACRO DEFINITIONS                                 *
**------------------------------------------------------------------------------*/
#define MMI_ALST_ERROR_MASK(err) ((err) | __LINE__ << 8)

// Initial capacity of the arraylist
#define ARRAYLIST_INITIAL_CAPACITY 32

#define MMI_ALIST_memshift(s, offset, length) memmove((s) + (offset), (s), (length)* sizeof(s));


/*------------------------------------------------------------------------------*
**                            TYPES AND CONSTANTS                               *
**------------------------------------------------------------------------------*/
#ifdef DEBUG
/* make sure both data and the pointer to the data are in read-only section */
const char * const alist_id = "array_list";
#endif

/*------------------------------------------------------------------------------*
**                            STATIC DEFINITIONS                                *
**------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*
**                            EXTERNAL DECLARE                                  *
**------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*
**                            Constant Variables                                *
**------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*
**                            GLOBAL DEFINITIONS                                *
**------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*
**                            LOCAL FUCTION DECLARE                             *
**------------------------------------------------------------------------------*/
/********************************************************************************
** Description: This function is used to Allocate sufficient array capacity for *
**              at least `size` elements                                        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
LOCAL int32 ALIST_Allocate(MMI_ALISTHANDLE_T lst, uint32 size);

/********************************************************************************
** Description: This function is used to find nearest power 2 integer which is  *
**              bigger than size                                                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
LOCAL uint32 NearstPower2(MMI_ALISTHANDLE_T lst, uint32 size, int32 is_trim);

/********************************************************************************
** Description: This function is used to binary search a list with cmp_func     *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
LOCAL int32 my_bsearch(
                void* key, 
                void* base, 
                uint32 num, 
                uint32 width, 
                MMI_ALIST_CmpFunc cmp_func, 
                ALIST_SEARCH_MODE_T mode);
/*------------------------------------------------------------------------------*
**                            FUNCTIONS                                         *
**------------------------------------------------------------------------------*/

/********************************************************************************
** Description: This function is used to create a arraylist                     *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_ALISTHANDLE_T MMI_ALIST_Create()
{
    MMI_ALISTHANDLE_T new_list = (MMI_ALISTHANDLE_T)SCI_ALLOCA(sizeof(MMI_ALIST_T));
    void** element = (void **) SCI_ALLOCA(sizeof(void*) * ARRAYLIST_INITIAL_CAPACITY);
    
    if(PNULL == new_list || PNULL == element)
    {
        SCI_FREE(new_list);
        SCI_FREE(element);
    }
    else
    {
        SCI_MEMSET(new_list, 0x00, sizeof(MMI_ALIST_T));
        SCI_MEMSET(element, 0x00, sizeof(void*) * ARRAYLIST_INITIAL_CAPACITY);
        //new_list->size = 0;
        new_list->element = element;
        //new_list->error = 0;
        new_list->capacity = ARRAYLIST_INITIAL_CAPACITY;
#ifdef DEBUG
        new_list->id = alist_id;
#endif
    }
    return (new_list);
}

/********************************************************************************
** Description: This function is used to destroy a arraylist                     *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_ALIST_Destroy(MMI_ALISTHANDLE_T *lst, MMI_ALIST_RemoveFunc rmv_func)
{
    MMIBD_ASSERT_DEBUG(lst && *lst);

    if (PNULL == lst || PNULL == *lst)
    {
        return;
    }
    
    MMI_ALIST_RemoveAllItems(*lst, rmv_func);

    SCI_FREE((*lst)->element);
    SCI_FREE(*lst);
    *lst = PNULL;
}

/********************************************************************************
** Description: This function is used to Allocate sufficient array capacity for *
**              at least `size` elements                                        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
LOCAL int32 ALIST_Allocate(MMI_ALISTHANDLE_T lst, uint32 size)
{
    int32 ret_val = MMI_ALST_ERROR_NULL_HANDLE;

    MMIBD_ASSERT_DEBUG(lst);

    MMIBD_ASSERT_DEBUG(size > 0);

    if (lst) 
    {
        lst->error = MMI_ALST_ERROR_NO_ERROR;

        if(size > lst->capacity)
        {
            uint32 new_capacity = NearstPower2(lst, size, 0);
            void** newmem_ptr = PNULL;

            newmem_ptr = (void **)SCI_ReAlloc(lst->element, sizeof(void*) * new_capacity, __FILE__, __LINE__);
            MMIBD_ASSERT_DEBUG(newmem_ptr);
            if(PNULL == newmem_ptr)
            {
                lst->error = MMI_ALST_ERROR_OUT_OF_MEMORY;
            }
            else
            {
                lst->element = newmem_ptr;
                lst->capacity = new_capacity;
            }
        }
        ret_val = lst->error;
    }
    return ret_val;
}

/********************************************************************************
** Description: This function is used to Add item at the end of the list.       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note: return no error if success, error code if failure                                                                       *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_Append(MMI_ALISTHANDLE_T lst, void* item)
{
    int32 ret_val = MMI_ALST_ERROR_NULL_HANDLE;

    MMIBD_ASSERT_DEBUG(lst);
    if (lst)
    {
        ret_val = ALIST_Allocate(lst, lst->size + 1);
        if(MMI_ALST_ERROR_NO_ERROR == ret_val)
        {
            lst->element[lst->size++] = item;
        }
    }
    return ret_val;
}

/********************************************************************************
** Description: This function is used to Pop (remove and return) an item off    *
**              the end of the list.                                            *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void* MMI_ALIST_Pop(MMI_ALISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    MMIBD_ASSERT_DEBUG(lst->size > 0);
    
    if (lst && lst->size > 0)
    {
        return lst->element[--lst->size];
    }
    return(PNULL);
}

/********************************************************************************
** Description: This function is used to Return item located at index           *
**              of the list.                                                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void* MMI_ALIST_Get(MMI_ALISTHANDLE_T lst, uint32 index)
{
    MMIBD_ASSERT_DEBUG(lst);

    MMIBD_ASSERT_DEBUG(index < lst->size);
    
    if (lst && index < lst->size)
    {
        return lst->element[index];
    }
    return (PNULL);
}

/********************************************************************************
** Description: This function is used to Replace item at index with given value *
**              of the list.                                                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_ALIST_Set(MMI_ALISTHANDLE_T lst, uint32 index, void* value)
{
    MMIBD_ASSERT_DEBUG(lst);

    MMIBD_ASSERT_DEBUG(index < lst->size);

    if (lst && index < lst->size)
    {
        lst->element[index] = value;
    }
}

/********************************************************************************
** Description: This function is used to Insert item at index, shifting the     *
**              following items by one spot.                                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_Insert(MMI_ALISTHANDLE_T lst, uint32 index, void* value)
{
    int32 ret_val = MMI_ALST_ERROR_NULL_HANDLE;
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        // Reallocate, if needed
        ret_val = ALIST_Allocate(lst, lst->size + 1);
        // Move data to create a spot for the new value
        if(MMI_ALST_ERROR_NO_ERROR == ret_val)
        {
            MMI_ALIST_memshift(lst->element + index, 1, lst->size - index);
            lst->element[index] = value;
            lst->size++;
        }
    }
    return ret_val;
}

/********************************************************************************
** Description: This function is used to Remove the item at index, shifting the *
**              following items by one spot.                                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void* MMI_ALIST_RemoveItem(MMI_ALISTHANDLE_T lst, uint32 index, MMI_ALIST_RemoveFunc rmv_func)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (PNULL == lst || index >= lst->size || lst->size == 0)
    {
        if(PNULL != lst)
        {
            lst->error = MMI_ALST_ERROR_INVALID_PARAM;
        }
    }
    else
    {
        void* value = lst->element[index];

        MMI_ALIST_memshift(lst->element + index + 1, -1, lst->size - index);
        lst->size--;
        if(rmv_func)
        {
            rmv_func(value);
        }
        return value;
    }
    return (PNULL);
}

/********************************************************************************
** Description: This function is used to Remove all item in the array list      *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_RemoveAllItems(MMI_ALISTHANDLE_T lst, MMI_ALIST_RemoveFunc func)
{
    int32 ret_val = MMI_ALST_ERROR_NULL_HANDLE;

    MMIBD_ASSERT_DEBUG(lst);

    if (PNULL == lst || lst->size == 0)
    {
        if(PNULL != lst)
        {
            lst->error = MMI_ALST_ERROR_INVALID_PARAM;
        }
    }
    else
    {
        int32 i = 0;

        for(i = lst->size - 1; i >= 0; i--)
        {
            MMI_ALIST_RemoveItem(lst, i, func);
        }
    }
    return ret_val;
}

/********************************************************************************
** Description: This function is used to Clear list of all items                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note: Remove all elements before you can do this to avoid memory leakage     *
*********************************************************************************/
PUBLIC void MMI_ALIST_Clear(MMI_ALISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        lst->size = 0;
    }
}

/********************************************************************************
** Description: This function is used to Return a slice of the list (of given   *
**              length starting at index) as a new arraylist                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note: User should take care of memeory allocated here!!!                     *
*********************************************************************************/
PUBLIC MMI_ALISTHANDLE_T MMI_ALIST_Slice(MMI_ALISTHANDLE_T lst, uint32 index, uint32 length)
{
    int32 ret_val = MMI_ALST_ERROR_NULL_HANDLE;
    MMIBD_ASSERT_DEBUG(lst);

    if(lst)
    {
        MMI_ALISTHANDLE_T new_list = MMI_ALIST_Create();
    
        MMIBD_ASSERT_DEBUG(new_list);

        MMIBD_ASSERT_DEBUG(index + length <= lst->size);

        if (PNULL == new_list || index + length <= lst->size)
        {
            if(PNULL == new_list)
            {
                lst->error = MMI_ALST_ERROR_OUT_OF_MEMORY;
            }
            else
            {
                lst->error = MMI_ALST_ERROR_INVALID_PARAM;
            }
        }
        else
        {
            ret_val = ALIST_Allocate(new_list, length);
            if(MMI_ALST_ERROR_NO_ERROR == ret_val)
            {
                SCI_MEMCPY(new_list->element, lst->element + index, length * sizeof(void*));
                new_list->size = length;
                return new_list;
            }
        }
    }
    return (PNULL);
}

/********************************************************************************
** Description: This function is used to Return a slice of the list (from index *
**              to the end) as a new arraylist.                                 *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:  User should take care of memeory allocated in MMI_ALIST_Slice(...)!!! *
*********************************************************************************/
PUBLIC MMI_ALISTHANDLE_T MMI_ALIST_SliceEnd(MMI_ALISTHANDLE_T lst, uint32 index)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return MMI_ALIST_Slice(lst, index, lst->size - index);
    }
    return (PNULL);
}

/********************************************************************************
** Description: This function is used to Return a copy of the arraylist         *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_ALISTHANDLE_T MMI_ALIST_Copy(MMI_ALISTHANDLE_T lst)
{
    return MMI_ALIST_SliceEnd(lst, 0);
}

/********************************************************************************
** Description: This function is used to Append a list onto another, in-place.  *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_Join(MMI_ALISTHANDLE_T lst, MMI_ALISTHANDLE_T source)
{
    int32 ret_val = MMI_ALST_ERROR_NULL_HANDLE;
    MMIBD_ASSERT_DEBUG(lst && source);

    if (lst && source)
    {
        ret_val = MMI_ALIST_Splice(lst, source, lst->size);
    }
    return ret_val;
}

/********************************************************************************
** Description: This function is used to Insert a list into another at the      *
**              given index, in-place.                                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_Splice(MMI_ALISTHANDLE_T lst, MMI_ALISTHANDLE_T source, uint32 index)
{
    int32 ret_val = MMI_ALST_ERROR_NULL_HANDLE;

    MMIBD_ASSERT_DEBUG(lst && source);

    if (lst)
    {
        // Reallocate, if needed
        ret_val = ALIST_Allocate(lst, lst->size + source->size);
        // Move data to the right
        if(MMI_ALST_ERROR_NO_ERROR == ret_val)
        {
            MMI_ALIST_memshift(lst->element + index, source->size, lst->size - index);
            // Copy the data over
            SCI_MEMCPY(lst->element + index, source->element, source->size * sizeof(void*));
            lst->size += source->size;
        }
    }
    return ret_val;
}

/********************************************************************************
** Description: This function is used to Return the number of items contained   *
**              in the list                                                     *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC uint32 MMI_ALIST_GetSize(MMI_ALISTHANDLE_T lst) 
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (lst->size);
    }
    return (0);
}

/********************************************************************************
** Description: This function is used to check if index reaches the end item    *
**              in the list                                                     *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC BOOLEAN MMI_ALIST_HasNext(MMI_ALISTHANDLE_T lst, uint32 index)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst && lst->size > 0)
    {
        return (index < lst->size);
    }
    return (0);
}

/********************************************************************************
** Description: This function is used to trim the capacity to current size      *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_TrimToSize(MMI_ALISTHANDLE_T lst)
{
    int32 ret_val = MMI_ALST_ERROR_NULL_HANDLE;

    MMIBD_ASSERT_DEBUG(lst);

    if (PNULL != lst)
    {
        uint32 new_capacity = NearstPower2(lst, 0, 1);

        if(new_capacity < lst->capacity)
        {
            void* newmem_ptr = PNULL;

            newmem_ptr = SCI_ReAlloc(lst->element, sizeof(void*) * new_capacity, __FILE__, __LINE__);

            MMIBD_ASSERT_DEBUG(newmem_ptr);

            if(newmem_ptr)
            {
                // trim is done successfully
                lst->element = newmem_ptr;
                lst->capacity = new_capacity;
                lst->error = MMI_ALST_ERROR_NO_ERROR;
            }
            else
            {
                // no memory left for trim operation
                lst->error = MMI_ALST_ERROR_OUT_OF_MEMORY;
            }
        }
        else
        {
            // no trim operation can be performed
            lst->error = MMI_ALST_ERROR_TRIM_FAILURE;
        }
        ret_val = lst->error;
    }
    return ret_val;
}

/********************************************************************************
** Description: This function is used to traverse through a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_ALIST_Traverse(MMI_ALISTHANDLE_T lst, MMI_ALIST_Func func)
{
    MMIBD_ASSERT_DEBUG(lst && func);

    if (lst && func)
    {
        uint32 index = 0;
        
        for (index = 0; index < lst->size; index++)
        {
            func(lst->element[index]);
        }
    }
}

/********************************************************************************
** Description: This function is used to find nearest power 2 integer which is  *
**              bigger than size                                                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
LOCAL uint32 NearstPower2(MMI_ALISTHANDLE_T lst, uint32 size, int32 is_trim)
{
    MMIBD_ASSERT_DEBUG(lst);

    if(lst)
    {
        uint32 new_capacity = lst->capacity;

        // trim is used to decrease the allocated buffer size to save memory
        if(is_trim == 1)
        {
            size = lst->size*2;
            while (new_capacity > size) 
            {
                new_capacity >>= 1;
            }
        }
        else if(size > new_capacity)
        {
            // this is to increase the size when a bigger array buffer is needed
            while (new_capacity < size) 
            {
                new_capacity *= 2;
            }
        }
        return new_capacity;
    }
    return(0);
}

/********************************************************************************
** Description: This function is used to sort a list with cmp_func             *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC void MMI_ALIST_Sort(MMI_ALISTHANDLE_T lst, MMI_ALIST_CmpFunc func)
{
    MMIBD_ASSERT_DEBUG(lst && func);

    if (lst && lst->size && func)
    {
        qsort(lst->element, lst->size, sizeof(void *), func);
    }
}

/********************************************************************************
** Description: This function is used for binary search of a list with cmp_func *
**              and direction                                                   *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Return: item pos found in database, negative if not found or invalid param   *
** Note:                                                                        *
*********************************************************************************/
LOCAL int32 my_bsearch(
                void* key,                      //IN: key used for search
                void* base,                     //IN: base pointer for data base
                uint32 num,                     //IN: data base size
                uint32 width,                   //IN: element size in data base
                MMI_ALIST_CmpFunc cmp_func,     //IN: compare function used for search
                ALIST_SEARCH_MODE_T mode)       //IN: search mode
{
    char *lo = (char *)base;
    char *hi = (char *)base + (num - 1) * width;
    char *mid = PNULL;
    size_t half = 0;
    char* ret_ptr = PNULL;
    int32 result = 0;

    /* validation section */
    MMIBD_ASSERT_DEBUG(base != PNULL);
    MMIBD_ASSERT_DEBUG(width > 0);
    MMIBD_ASSERT_DEBUG(cmp_func != PNULL);

    if(base == PNULL || width == 0 || cmp_func == PNULL)
    {
       return -MMI_ALST_ERROR_INVALID_PARAM;
    }
    /*
    We allow a NULL key here because it breaks some older code and because we do not dereference
    this ourselves so we can't be sure that it's a problem for the comparison function
    */

    while (lo <= hi)
    {
        if ((half = num / 2) != 0)
        {
            mid = lo + (num & 1 ? half : (half - 1)) * width;
            result = cmp_func(key, mid);
            if (!result)
            {
                ret_ptr = mid;
                if(mode == ALIST_SEARCH_FIRST)
                {
                    hi = mid - width;
                    num = num & 1 ? half : half-1;
                }
                else if(mode == ALIST_SEARCH_LAST)
                {
                    lo = mid + width;
                    num = half;
                }
                else
                {
                    break;
                }
            }
            else if (result < 0)
            {
                hi = mid - width;
                num = num & 1 ? half : half-1;
            }
            else
            {
                lo = mid + width;
                num = half;
            }
        }
        else 
        { 
            if (num && cmp_func(key, lo) == 0)
            {
                ret_ptr = lo;
            }
            break;
        }
    }

    if(PNULL != ret_ptr)
    {
        result = (int32)((uint32)(ret_ptr - (char *)base) / width);
    }
    else
    {
        result = -MMI_ALST_ERROR_NOT_FOUND;
    }

    return result;

}

/********************************************************************************
** Description: This function is used to search an item in a sorted list        *
**              with key and compare func                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_Find(MMI_ALISTHANDLE_T lst, void* key, MMI_ALIST_CmpFunc func)
{
    int32 ret_val = -MMI_ALST_ERROR_NULL_HANDLE;

    MMIBD_ASSERT_DEBUG(lst && func);

    if (PNULL == lst || lst->size == 0 || func == PNULL)
    {
        if(lst)
        {
            lst->error = MMI_ALST_ERROR_INVALID_PARAM;
            ret_val = -MMI_ALST_ERROR_INVALID_PARAM;
        }
    }
    else
    {
        ret_val = my_bsearch(key, lst->element, lst->size, sizeof(void *), func, ALIST_SEARCH_NORMAL);
        if(ret_val < 0)
        {
            lst->error = -ret_val;
        }
        else
        {
            lst->error = MMI_ALST_ERROR_NO_ERROR;
        }
    }
    return ret_val;
}

/********************************************************************************
** Description: This function is used to sort a list with func                  *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_FindFirst(MMI_ALISTHANDLE_T lst, void* key, MMI_ALIST_CmpFunc func)
{
    MMIBD_ASSERT_DEBUG(lst && func);

    if (lst && lst->size && func)
    {
        return my_bsearch(key, lst->element, lst->size, sizeof(void *), func, ALIST_SEARCH_FIRST);
    }
    return -1;
}

/********************************************************************************
** Description: This function is used to sort a list with func                  *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_FindLast(MMI_ALISTHANDLE_T lst, void* key, MMI_ALIST_CmpFunc func)
{
    MMIBD_ASSERT_DEBUG(lst && func);

    if (lst && lst->size && func)
    {
        return my_bsearch(key, lst->element, lst->size, sizeof(void *), func, ALIST_SEARCH_LAST);
    }
    return -1;
}

/********************************************************************************
** Description: This function is used to get the error code of action           *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_ALIST_GetCurrErrorCode(MMI_ALISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (lst->error);
    }

    return (0);    
}

#ifdef DEBUG
/********************************************************************************
** Description: This function is used to validate a list                        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
PUBLIC int32 MMI_ALIST_Validate(MMI_ALISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (!strcmp(lst->id, alist_id));
    }

    return (0);
}
#endif

#ifdef _MMI_ALST_UNIT_TEST_
typedef struct test_element
{
    char    c;
    int16   s;
    int32   i;
    char    str[128];
} test_element;
/********************************************************************************
** Description: This function is used to print an element value for unit test   *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void PrintElement(void * val)
{
    printf("%c ", ((test_element *) val)->c);
    printf("%d ", ((test_element *) val)->s);
    printf("%d ", ((test_element *) val)->i);
    printf("%s \r\n", ((test_element *) val)->str);
}

void DeleteElement(void *val)
{
    SCI_FREE(val);
}

int32 CompareElement(void *src, void *dst)
{
    test_element *data1 = *(test_element **)src;
    test_element *data2 = *(test_element **)dst;

    if(data1->c > data2->c)
    {
        return -1;
    }
    else if(data1->c < data2->c)
    {
        return 1;
    }
    return 0;
}

/********************************************************************************
** Description: This function is used to provide test functions for unit test   *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void main(void)
{
    MMI_ALISTHANDLE_T lst = MMI_ALIST_Create();

    if (lst)
    {
        int32 i;
        test_element *test_data[32];

#ifdef DEBUG
        MMIBD_ASSERT_DEBUG(MMI_ALIST_Validate(lst));
#endif

        /* starting from empty list and appending at tail are tested with this loop */
        for (i = 0; i < 10; i++)
        {
            test_data[i] = SCI_ALLOCA(sizeof(test_element));
            SCI_MEMSET(test_data[i], 0x00, sizeof(test_element));
            test_data[i]->c = i + '0';
            test_data[i]->s = i + 0x3fff;
            test_data[i]->i = i + 0x3fffffff;
            strcpy(test_data[i]->str, "test_string", 11);
            test_data[i]->str[11] = test_data[i]->c;
            test_data[i]->str[12] = '\0';
            MMI_ALIST_Append(lst, test_data[i]);
        }

        test_data[i] = SCI_ALLOCA(sizeof(test_element));
        SCI_MEMSET(test_data[i], 0x00, sizeof(test_element));
        test_data[i]->c = i + '0';
        test_data[i]->s = i + 0x3fff;
        test_data[i]->i = i + 0x3fffffff;
        strcpy(test_data[i]->str, "test_string", 11);
        test_data[i]->str[11] = test_data[i]->c;
        test_data[i]->str[12] = '\0';
        MMI_ALIST_Insert(lst, i, test_data[i]);
        printf("The list is created like this:\n");
        MMI_ALIST_Traverse(lst, PrintElement);

        i++;
        test_data[i] = SCI_ALLOCA(sizeof(test_element));
        SCI_MEMSET(test_data[i], 0x00, sizeof(test_element));
        test_data[i]->c = i + '0';
        test_data[i]->s = i + 0x3fff;
        test_data[i]->i = i + 0x3fffffff;
        strcpy(test_data[i]->str, "test_string", 11);
        test_data[i]->str[11] = test_data[i]->c;
        test_data[i]->str[12] = '\0';
        MMI_ALIST_Insert(lst, i, test_data[i]);
        printf("The second element is: ");
        PrintElement(MMI_ALIST_Get(lst, 1));

        MMI_ALIST_Sort(lst, CompareElement);
        printf("After sorting, the list has %d elements:\n", MMI_ALIST_GetSize(lst));
        MMI_ALIST_Traverse(lst, PrintElement);

        MMI_ALIST_RemoveItem(lst, 1, DeleteElement);
        printf("After deleting the second element the list has %d elements:\n", MMI_ALIST_GetSize(lst));
        MMI_ALIST_Traverse(lst, PrintElement);

        /* boundary condition tests */
        MMI_ALIST_RemoveItem(lst, i-1, DeleteElement);
        printf("After deleting the tail the list has %d elements:\n", MMI_ALIST_GetSize(lst));
        MMI_ALIST_Traverse(lst, PrintElement);

        MMI_ALIST_RemoveItem(lst, 0, DeleteElement);
        printf("After deleting the head the list has %d elements:\n", MMI_ALIST_GetSize(lst));
        MMI_ALIST_Traverse(lst, PrintElement);

        MMI_ALIST_Destroy(&lst, DeleteElement);
        SCI_FREE(test_data);
    }
}
#endif
