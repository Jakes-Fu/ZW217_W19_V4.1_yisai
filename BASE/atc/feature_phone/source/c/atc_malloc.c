/******************************************************************************
 ** File Name:      atc_malloc.c                                              *
 ** Author:         Steven.Yao                                                *
 ** Date:           05/20/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contain the definitions of memory allocation    *
 **                 functions. We create a private heap from system memory and*
 **                 alloc memory from this heap. By doing so, we can alloc    *
 **                 small pieces of memory and destroy the heap at last to    *
 **                 free all memory alloced.                                  *
 ******************************************************************************
 

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/20/2002     Steven.Yao       Created.                                  *
 ******************************************************************************/

/*******************************************************************************
 **                        ATC Header Files                                    *
 ******************************************************************************/
#include "base_trc.h"
#include "atc_malloc.h"

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef  __cplusplus
    extern  "C"
    {
#endif

static uint8 * g_atc_heap_base_ptr[ATC_MAX_HEAP_NUM] = 
{
    NULL, NULL, NULL /* point to the base address of every mem heap. */
};
static uint8 * g_atc_heap_end_ptr[ATC_MAX_HEAP_NUM] = 
{
    NULL, NULL, NULL /* point to the end address of every mem heap. */
};
static uint8 * g_atc_heap_free_ptr[ATC_MAX_HEAP_NUM] = 
{
    NULL, NULL, NULL /* point to the address of free space in every mem heap
                        that we can alloc from. */ 
};

static uint    g_atc_heap_size[ATC_MAX_HEAP_NUM] = 
{
    0, 0, 0 /* store the size of our private every heap in bytes. */
};

static int32    g_atc_cur_heap_index = 0;  /* index of current heap we can use */
                                       

LOCAL ATC_STATUS AddOneHeap( // return S_ATC_SUCCESS if success, 
                         // otherwise return error code
        uint size        // Heap size
        );
LOCAL void ATCM_InitHeapVar( void );
/******************************************************************************/
// Description : This function allocate a private heap from system memory.
//               We use this heap to allocate pieces of memory for ATC 
//               information struct.
// Global resource dependence : g_atc_heap_base_ptr
//                              g_atc_heap_end_ptr
//                              g_atc_heap_free_ptr
//                              g_atc_heap_size
//                              g_atc_cur_heap_index
// Author : Ivan.Yin
// Note : None
/******************************************************************************/
LOCAL void ATCM_InitHeapVar( void ) 
{
    int32            i;
    for (i = 0; i < ATC_MAX_HEAP_NUM; i++)
    {
        g_atc_heap_base_ptr[i]  = NULL;
        g_atc_heap_end_ptr[i]   = NULL;
        g_atc_heap_free_ptr[i]  = NULL;
        g_atc_heap_size[i]      = 0;
    }
    
    g_atc_cur_heap_index = 0;  
}
/******************************************************************************/
// Description : This function alloces a private heap from system memory.
//               We use this heap to alloc pieces of memory for ATC 
//               infomation struct.
// Global resource dependence : g_atc_heap_base_ptr
//                              g_atc_heap_end_ptr
//                              g_atc_heap_free_ptr
//                              g_atc_heap_size
//                              g_atc_cur_heap_index
// Author : Steven.Yao
// Note : None
/******************************************************************************/
ATC_STATUS ATCM_CreateHeap( void )  // return S_ATC_SUCCESS if success, 
                                    // otherwise return error code
{
    // init the vars related the atc heap
    ATCM_InitHeapVar();

    g_atc_cur_heap_index = -1;

    return AddOneHeap( ATC_INIT_HEAP_SIZE ); 
}

/******************************************************************************/
// Description : This function frees all the pieces of memory we alloc and
//               also frees the additional heap we add, but does not free
//               the first heap we create.
// Global resource dependence : g_atc_heap_base_ptr
//                              g_atc_heap_end_ptr
//                              g_atc_heap_free_ptr
//                              g_atc_heap_size
//                              g_atc_cur_heap_index
// Author : Steven.Yao
// Note : None
/******************************************************************************/
ATC_STATUS ATCM_FreeAll( void ) // return S_ATC_SUCCESS if success,
                                // otherwise return error code
{
    int32    i;

    /* Destroy additional heaps first */
    if ( 0 < g_atc_cur_heap_index )
    {
        for( i = 1; i <= g_atc_cur_heap_index; i++ )
        {
            SCI_FREE( g_atc_heap_base_ptr[i] );

            g_atc_heap_base_ptr[i] = NULL;
            g_atc_heap_end_ptr[i] = NULL;
            g_atc_heap_free_ptr[i] = NULL;

            g_atc_heap_size[i] = 0;
        }
    }

    g_atc_heap_free_ptr[0] = g_atc_heap_base_ptr[0];

    g_atc_cur_heap_index = 0;

    return S_ATC_SUCCESS;
}

#ifndef _BASE_COMPACT_CODE_
/******************************************************************************/
// Description : This function destroies all heaps we create.
// Global resource dependence : g_atc_heap_base_ptr
//                              g_atc_heap_end_ptr
//                              g_atc_heap_free_ptr
//                              g_atc_heap_size
//                              g_atc_cur_heap_index
// Author : Steven.Yao
// Note : None
/******************************************************************************/
ATC_STATUS ATCM_DestroyHeap( void ) // return S_ATC_SUCCESS if success,
                                    // otherwise return error code.
{
    ATCM_FreeAll();

    g_atc_heap_base_ptr[0] = NULL;
    g_atc_heap_end_ptr[0] = NULL;
    g_atc_heap_free_ptr[0] = NULL;

    g_atc_heap_size[0] = 0;

    return S_ATC_SUCCESS;
}
#endif

/******************************************************************************/
// Description : This function alloces a piece of memory from our private heap.
// Global resource dependence : g_atc_heap_base_ptr
//                              g_atc_heap_end_ptr
//                              g_atc_heap_free_ptr
//                              g_atc_heap_size
//                              g_atc_cur_heap_index
// Author : Steven.Yao
// Note : None
/******************************************************************************/
ATC_STATUS ATCM_Alloc( // return S_ATC_SUCCESS if success, 
                       // otherwise return error code.
        void ** ptr,   // point to a pointer which point the mem we alloc
        uint size      // mem size we want to alloc
        )
{
    ATC_STATUS  status = S_ATC_SUCCESS;

    uint        rounded_size = 0;

    SCI_ASSERT( NULL != ptr ); /*assert verified*/
    SCI_ASSERT( ATC_MAX_HEAP_NUM > g_atc_cur_heap_index ); /*assert verified*/
    SCI_ASSERT( NULL != g_atc_heap_base_ptr[g_atc_cur_heap_index] ); /*assert verified*/
    SCI_ASSERT( NULL != g_atc_heap_end_ptr[g_atc_cur_heap_index] );  /*assert verified*/
    SCI_ASSERT( NULL != g_atc_heap_free_ptr[g_atc_cur_heap_index] ); /*assert verified*/

    rounded_size = size % ATC_MEM_ALLOC_UNIT_SIZE;
    if ( 0 != rounded_size )
    {
        rounded_size = size + ATC_MEM_ALLOC_UNIT_SIZE - rounded_size;
    }
    else
    {
        rounded_size = size;
    }

    /* Check boundary */
    if ( (uint)(g_atc_heap_free_ptr[g_atc_cur_heap_index] + rounded_size + 1) > 
            (uint)g_atc_heap_end_ptr[g_atc_cur_heap_index] )
    {
        status = AddOneHeap( ATC_STEP_HEAP_SIZE );
        if ( S_ATC_SUCCESS != status )
        {
            *ptr = NULL;

            return status;
        } 
    }

    *ptr = g_atc_heap_free_ptr[g_atc_cur_heap_index];
    g_atc_heap_free_ptr[g_atc_cur_heap_index] = (uint8 *)*ptr + rounded_size;

    return status;
}

/******************************************************************************/
// Description : This function addes a private heap from system memory.
// Global resource dependence : g_atc_heap_base_ptr
//                              g_atc_heap_end_ptr
//                              g_atc_heap_free_ptr
//                              g_atc_heap_size
//                              g_atc_cur_heap_index
// Author : Steven.Yao
// Note : None
/******************************************************************************/
LOCAL ATC_STATUS AddOneHeap( // return S_ATC_SUCCESS if success, 
                               // otherwise return error code
        uint size        // Heap size
        )
{
    uint8   * new_heap_base_ptr = NULL;

    if ( ATC_MAX_HEAP_NUM <= ( g_atc_cur_heap_index + 1 ) )
    {
        return ERR_MEMORY_FULL;
    }

    g_atc_cur_heap_index++;
    
    SCI_ASSERT( NULL == g_atc_heap_base_ptr[g_atc_cur_heap_index] ); /*assert verified*/
    SCI_ASSERT( NULL == g_atc_heap_end_ptr[g_atc_cur_heap_index] );  /*assert verified*/
    SCI_ASSERT( NULL == g_atc_heap_free_ptr[g_atc_cur_heap_index] ); /*assert verified*/
    SCI_ASSERT( 0 == g_atc_heap_size[g_atc_cur_heap_index] ); /*assert verified*/

    new_heap_base_ptr = SCI_ALLOC_BASE( size );
    if ( NULL == new_heap_base_ptr )
    {
        g_atc_cur_heap_index--;

        return ERR_MEMORY_FULL;
    }

    g_atc_heap_base_ptr[g_atc_cur_heap_index] = new_heap_base_ptr;
    g_atc_heap_end_ptr[g_atc_cur_heap_index] = new_heap_base_ptr + size - 1;
    g_atc_heap_free_ptr[g_atc_cur_heap_index] = new_heap_base_ptr;

    g_atc_heap_size[g_atc_cur_heap_index] = size;

    //ATC_TRACE:"Add heap success, heap size = %d.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_MALLOC_263_112_2_17_22_17_44_744,(uint8*)"d", size);

    return S_ATC_SUCCESS;
}
/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef  __cplusplus
    }
#endif

