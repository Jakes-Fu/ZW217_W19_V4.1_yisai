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
 ** File Name:      efs_rw.h                                                  *
 ** Author:         lin.liu                                                   *
 ** DATE:           04/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 **                                                                           *
 ** Description:    This file include the interface of access file.           *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/12/2001     lin.liu          Create.                                   *
 ******************************************************************************/
#ifndef EFS_READ_WRITE_H
#define EFS_READ_WRITE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "efs_fcb.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

//
//  Process read file request.
//
EFS_ERROR_E EFS_DoRead(
    EFS_FCB_T   *fcb,      // file handler that returned by EFS_Create
    uint8       *buf,
    uint32      offset,
    uint32      len);
    
//
// Process the write file request.
//    
EFS_ERROR_E EFS_DoWrite(
    EFS_FCB_T   *fcb,
    uint8       *buf,
    uint32      offset,
    uint32      len);



//
// Handle the change file size request.
//
uint32 EFS_DoChangeSize( EFS_FCB_T  * fcb_ptr, uint32  size );
    
//
//
//
BOOLEAN write_back_cache(  EFS_FCB_T * fcb );




BOOLEAN expend_file_size( EFS_FCB_T * fcb, uint32  size );

    

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // End efs_rw.h 