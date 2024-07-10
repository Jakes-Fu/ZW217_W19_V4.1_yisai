/******************************************************************************
 ** File Name:      atp_malloc.h                                              *
 ** Author:         Steven.Yao                                                *
 ** Date:           05/15/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains the MICROS and memory allocation       *
 **                 functions' proto type that are of use to other modules.   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/15/2002     Steven.Yao       Create.                                   *
 ******************************************************************************/

#ifndef ATP_MALLOC_H
#define ATP_MALLOC_H

#define ATP_INIT_MEMORY_SIZE    (0x100*0x1024)  
#define ATP_MAX_MEMORY_SIZE     (0x200*0x1024)

void atp_mem_create(void);

void atp_mem_destroy(void);

void *  atp_malloc(size_t);

int     atp_mfree(void *);
#endif
