/******************************************************************************
 ** File Name:      atp_malloc.c                                              *
 ** Author:         Steven.Yao                                                *
 ** Date:           05/15/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains the definitions of memory allocation   *
 **                 functions. We create a private heap from system memory and*
 **                 alloc and free memory from this heap. By doing that, we   *
 **                 can alloc small pieces of memory and destory the heap at  *
 **                 last to free all memory alloced.                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/15/2002     Steven.Yao       Create.                                   *
 ******************************************************************************/

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include "atp_malloc.h"

#define SCI_ASSERT(_bool)  assert(_bool)

HANDLE  atp_mem = NULL;

void atp_mem_create(void)
{
    if ( NULL != atp_mem )
    {
        HeapDestroy( atp_mem );
    }

    atp_mem = HeapCreate( HEAP_NO_SERIALIZE,
                          ATP_INIT_MEMORY_SIZE,
                          ATP_MAX_MEMORY_SIZE);

    if ( NULL == atp_mem )
    {
        printf("Error! Can not create heap memory!\n");

        exit(1);
    }
}

void atp_mem_destroy(void)
{
    if ( NULL != atp_mem )
    {
        HeapDestroy( atp_mem );
    }
}

void * atp_malloc(size_t size)
{
    void * p_tmp = NULL;

    SCI_ASSERT( NULL != atp_mem );

    p_tmp = HeapAlloc( atp_mem, HEAP_NO_SERIALIZE, size );

    if ( NULL == p_tmp )
    {
        printf("Error! Can not alloc memory from heap\n");

        if ( NULL != atp_mem )
        {
            HeapDestroy( atp_mem );
        }
        
        exit(1);
    }

    return p_tmp; 
}

int atp_mfree(void * p)
{

    SCI_ASSERT( NULL != atp_mem );

    return HeapFree( atp_mem, HEAP_NO_SERIALIZE, p );
}
