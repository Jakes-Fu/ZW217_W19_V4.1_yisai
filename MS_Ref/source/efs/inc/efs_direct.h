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
 ** File Name:      efs_direct.h                                              *
 ** Author:         lin.liu                                                   *
 ** DATE:           04/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 **                                                                           *
 ** Description:    This file include the interface that access flash directly*
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/12/2001     lin.liu          Create.                                   *
 ******************************************************************************/
#ifndef EFS_DIRECT_H
#define EFS_DIRECT_H

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
//  Access the flash dirctly.
//  We not extern the function in efs.h, because only be called by 
//  system task.
//

EFS_ERROR_E EFS_Direct( // return operation error code.
    BOOLEAN     read,   // transmit direction. if TRUE, read. otherwize write  */
    uint8       *buf,   // the buffer that contain data or store data to it.  
    uint32      addr,   // start addr
    uint32      len);   // length.


//
// Process the directly access request.
//
EFS_ERROR_E EFS_DoDirect(   // error code.
    BOOLEAN     read_write,  // read(TRUE) or write(FALSE).
    uint8       *buf,        // the buffer that contain data or we store data to it.  
    uint32      addr,        // start address.
    uint32      len);        // read/write length.

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // End EFS_DIRECT_H.