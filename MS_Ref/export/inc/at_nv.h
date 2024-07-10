/******************************************************************************
 ** File Name:      at_nv.h                                                   *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           29/05/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file contains all the definitions necessary   *
 **                 to interface with at_nv.c.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 29/05/2002     Xueliang.Wang    Create.                                   *
 ******************************************************************************/

#ifndef _AT_NV_H
#define _AT_NV_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include    "at_common.h"

 /**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Handle all NVItem command. If success, make a response 
//                  message by dest_ptr, else don't make response message.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC	uint32 ATNV_HandleNv( // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len);        // Size of the source buffer in uint8.

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _AT_NV_H

