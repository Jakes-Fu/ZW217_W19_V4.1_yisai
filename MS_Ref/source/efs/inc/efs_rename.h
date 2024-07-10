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
 ** File Name:      efs_rename.h                                              *
 ** Author:         lin.liu                                                   *
 ** DATE:           04/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 **                                                                           *
 ** Description:    This file include the interface about rename file.        *
 **                 ( Only internal part )
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/12/2001     lin.liu          Create.                                   *
 ******************************************************************************/
#ifndef EFS_RENAME_H
#define EFS_RENAME_H

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
// Process the rename-request.
//
EFS_ERROR_E EFS_DoRename(
    const char *sour,     // the file will be renamed.
    const char *dest);    // replace file name.

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // End EFS_RENAME_H 