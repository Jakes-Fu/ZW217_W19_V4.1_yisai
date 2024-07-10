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
 ** File Name:      efs_base.h                                                *
 ** Author:         lin.liu                                                   *
 ** DATE:           05/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 Basic macro define                                        *
 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/12/2001     Lin.liu          Create.                                   *
 ******************************************************************************/

#ifndef EFS_BASE_H
#define EFS_BASE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"   /* basic type  */

//#include "flash.h"
#include "tb_flash.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef INPUT
#undef INPUT
#endif
#define INPUT

#ifdef OUTPUT
#undef OUTPUT
#endif
#define OUTPUT

#ifdef IN_OUT
#undef IN_OUT
#endif
#define IN_OUT

#define EFS_INVALID_ADDR           HAL_INVALID_FLASH_ADDR
#define EFS_INVALID_LENGTH         0xFFFF
#define EFS_INVALID_SECTOR         0xFFFF

//@lin.liu(2003-12-04). CR: MS5446
//#define EFS_ITEM_HEAD_SIZE         4      // Item length and identifier.(uint16 + uint16)
//@end.

#define MAKE_LONG( _high, _low )   ( ((uint32)(_high) << 16) + (uint32)(_low) )

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  /*  end EFS_BASE_H  */