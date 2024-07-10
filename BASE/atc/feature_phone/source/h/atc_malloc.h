/******************************************************************************
 ** File Name:      atc_malloc.h                                              *
 ** Author:         Steven.Yao                                                *
 ** DATE:           05/20/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains MICROS and memory allocation functions'*
 **                 proto type declarations. These functions are used by other*
 **                 ATC modules to alloc pieces of memory from a private heap.*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/20/2002     Steven.Yao       Created.                                  *
 ******************************************************************************/

#ifndef _ATC_MALLOC_H
#define _ATC_MALLOC_H

/*******************************************************************************
 **                        ATC Header Files                                    *
 ******************************************************************************/
#include "atc.h"
#include "atc_common.h"

#define ATC_MAX_HEAP_NUM        3

#define ATC_MEM_ALLOC_UNIT_SIZE   4

#define ATC_INIT_HEAP_SIZE        (1280) //(512)  //we should augment this value to 1280 or above(not more than 1600),
                                                  //this setting will avoid re-malloc in lex parse while SIO receive a long command, such as AT+SDATASEND=1,1000 command 
                                                  //or the memory will re-malloc a segment to pad this memory,
                                                  //but the bad things is that it will occur memory covered or something others in adding heap memory.
#define ATC_STEP_HEAP_SIZE        (128)

ATC_STATUS ATCM_CreateHeap( void ); // return S_ATC_SUCCESS if success, 
                                    // otherwise return error code
ATC_STATUS ATCM_FreeAll( void );// return S_ATC_SUCCESS if success,
                                // otherwise return error code
ATC_STATUS ATCM_DestroyHeap( void );// return S_ATC_SUCCESS if success,
                                    // otherwise return error code.
ATC_STATUS ATCM_Alloc( // return S_ATC_SUCCESS if success, 
                       // otherwise return error code.
        void ** ptr,   // point to a pointer which point the mem we alloc
        uint size      // mem size we want to alloc
        );
#endif /* End of #ifndef _ATC_MALLOC_H */
