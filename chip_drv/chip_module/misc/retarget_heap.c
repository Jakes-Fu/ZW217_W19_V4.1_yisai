/******************************************************************************
 ** File Name:      retarget_heap.c                                           *
 ** Author:         rick zhang                                                *
 ** DATE:           29/10/2005                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines functions to enable retargetting of     *
 **                 heap implementation                                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 29/10/2005     rick zhang       Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "stdio.h"
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
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define _RETARGET_HEAP_
#if defined (_RETARGET_HEAP_)
#include <rt_heap.h>
/*
 * This is the structure that defines the heap descriptor. The
 * first section of it is used by the C library and so the format
 * is fixed. Space after that can be used as the user wishes.
 */
struct __Heap_Descriptor
{
    void *my_first_free_mem;
    void *my_heap_limit;

};

/*
 * Define all these functions to override the heap. __Heap_DescSize
 * must return the size of the __Heap_Descriptor structure.
 */
int __Heap_DescSize (int zero)
{
    return sizeof (struct __Heap_Descriptor);
}

void __Heap_Initialize (struct __Heap_Descriptor *h)
{
    return ;
}

void __Heap_ProvideMemory (struct __Heap_Descriptor *h, void *base, size_t size)
{
    /* h    -- heap descriptor
     * base -- a pointer to a new 8-byte aligned block of memory
     * size -- the size of the block
     */

    /* We not used heap, so we not need to implement this function */
    return ;
}

/*
 *Note:The default implementation of this function allocates an 8-byte aligned
 *      block of memory. If you reimplement this function it is recommended that
 *      you return 8-byte aligned blocks of memory.
 */
void *__Heap_Alloc (struct __Heap_Descriptor *h, size_t size)
{
    return (void *) SCI_ALLOC_APP (size);
}

void __Heap_Free (struct __Heap_Descriptor *h, void *blk)
{
    SCI_FREE (blk);
    return ;
}

/*
 * changes the size of the object pointed to by ptr to the size specified by
 * size. The contents of the object shall be unchanged up to the lesser of
 * the new and old sizes. If the new size is larger, the value of the newly
 * allocated portion of the object is indeterminate. If ptr is a null
 * pointer, the realloc function behaves like a call to malloc for the
 * specified size. Otherwise, if ptr does not match a pointer earlier
 * returned by calloc, malloc or realloc, or if the space has been
 * deallocated by a call to free or realloc, the behaviour is undefined.
 * If the space cannot be allocated, the object pointed to by ptr is
 * unchanged. If size is zero and ptr is not a null pointer, the object it
 * points to is freed.
 * Returns: either a null pointer or a pointer to the possibly moved
 *          allocated space.
 */

void *__Heap_Realloc (struct __Heap_Descriptor *h, void *blk, size_t size)
{

    if (NULL == blk)
    {
        if (0 == size)
        {
            /* cann't allocate any memory */
            return NULL;
        }
        else /* size > 0 */
        {
            /* allocate memory for  */
            return (void *) SCI_ALLOC_APP (size);
        }
    }
    else /* NULL != blk */
    {
        if (0 == size)
        {
            /* release blk */
            SCI_FREE (blk);
            return NULL;
        }
        else /* size > 0 */
        {
            /* allocate new mem with size, copy contents from old ptr pointer
             * to new ptr pointer mem, release blk and return new ptr */
            void *ptr = (void *) SCI_ALLOC_APP (size);

            if (NULL != ptr)
            {
                /* If the new size is larger, the value of the newly
                 * allocated portion of the object is indeterminate.  */
                SCI_MEMCPY (ptr, blk, size);

                /* release old memory */
                SCI_FREE (blk);

                /* return new ptr */
                return ptr;
            }
            else  /* cann't allocate new mem*/
            {
                /* Don't change old boject */
                return blk;
            }
        }
    }

}

void *__Heap_Stats (struct __Heap_Descriptor *h,
                    int (* print) (void *, char const *format, ...),
                    void *printparam)
{
    void *rt_point = NULL;
    /* No need to implement this function */
    return rt_point;
}

int __Heap_Valid (struct __Heap_Descriptor *h,
                  int (* print) (void *, char const *format, ...),
                  void *printparam, int verbose)
{
    /* No need to implement this function */
    return 1;
}


PUBLIC void __push_ddtor__FPFv_v (void (*f) ())
{

}

#else   /* not defined (_RETARGET_HEAP_) */




extern void __use_no_heap (void);

void check_no_use_heap (void)
{
    /* C-library function, guards against use of malloc(), realloc(), free() */
    __use_no_heap();
}



#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

