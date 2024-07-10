/******************************************************************************
 **
 **  Copyright (C) 2001 SPREADTRUM COMMUNICATION Ltd.
 **  All rights reserved.
 **
 **  This document contains proprietary information blonging to 
 **  SPREADTRUM. Passing on and copying this document, use and 
 **  communication of its contents is not permitted without prior
 **  writed authorization.
 ******************************************************************************/


/******************************************************************************
 ** File Name:      efs_delete.h                                              *
 ** Author:         lin.liu                                                   *
 ** DATE:           04/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 **                                                                           *
 ** Description:    This file include the internal interface of delete operation
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/12/2001     lin.liu          Create.                                   *
 ******************************************************************************/
#ifndef EFS_DELETE_H
#define EFS_DELETE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

// 
//  release the given block and all of the next block.
//
void delete_data_block( uint16  block_id );

//
//  Process the request of delete a file.
//
EFS_ERROR_E EFS_DoDelete( // return the operation status.
    const char *name);    // file name.

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // End EFS_DELETE_H 