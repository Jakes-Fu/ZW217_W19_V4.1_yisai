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
 ** File Name:      efs_length.h                                              *
 ** Author:         lin.liu                                                   *
 ** DATE:           04/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 **                                                                           *
 ** Description:    This file include the interface about file length.        *
 **                 (only internal-part)
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/12/2001     lin.liu          Create.                                   *
 ******************************************************************************/
#ifndef EFS_LENGTH_H
#define EFS_LENGTH_H

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
//  calculator the given file's length.
//
uint32 EFS_CalcuFileLength(
    ITEM_T header_item);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // End EFS_LENGTH_H 