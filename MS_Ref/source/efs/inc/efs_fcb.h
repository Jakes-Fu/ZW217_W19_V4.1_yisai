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
 ** File Name:      efs_fcb.h                                                 *
 ** Author:         lin.liu                                                   *
 ** DATE:           11/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 This file include the defintion and interface of FCB.     *
 **                 FCB ( File Control Block )                                *
 **
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/12/2001     lin.liu          Create.                                   *
 ******************************************************************************/

#ifndef EFS_FCB_H
#define EFS_FCB_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "efs_comm.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif



//
// use these flag to indicate FCB , FIND or free.
//
#define EFS_FCB_FREE        0xFFFFFFFF
#define EFS_FILE_FLAG       0x4646494C    // FFIL
#define EFS_FIND_FLAG       0x46464E44    // FFND


typedef struct
{
    uint32  flag;

    uint32         length;       // file length
    uint32         cur_pos;      // current postion of the file.
    
#ifdef EFS_CACHE
    EFS_BLOCK_T   *cache;       // the content include from pos to pos + block_size            
    uint32         cache_pos;   // the base pos of this cache
    ITEM_T         cache_item;
    BOOLEAN        write_back;  // the content of cache need write back to device?
#endif

    ITEM_T         header_item;  // the item's id of file header item
    uint16         mode;         // mode
}EFS_FCB_T;

typedef struct
{
    uint32         flag;   // = EFS_FIND_FLAG. must be first.
    ITEM_T         next_item;
}EFS_FIND_T;   // the header_item of the file.

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
 
//
//    Init FCB table
//
BOOLEAN EFS_InitFCB(void); 


//
//    Check the FCB , if it is valid, return true
//
BOOLEAN EFS_CheckFCB(
    EFS_FCB_T *fcb_ptr);  // the pointer of FCB.

//
//   check the file has opened by other operation ?
//
BOOLEAN EFS_FileIsOpen(   // If the file opened by others, return TRUE.
    ITEM_T header_item);  // the header item of the file.


//
//   Check access mode.
//
BOOLEAN EFS_CheckAccess( // If don't access the file with given mode, return FALSE.
    ITEM_T header_item,  // the header item of the file.
    uint16 mode);        // access mode.

//
//    try to allocate a free fcb 
//
EFS_FCB_T *EFS_MallocFCB(void); // the pointer of FCB we allocated.
                                // NULL means allocated failed.

//
//    free the FCB returned by EFS_MallocFCB.
//
void EFS_FreeFCB(
    EFS_FCB_T *fcb_ptr); // the pointer of the FCB that will be free-ed.


EFS_FIND_T  *EFS_MallocFIND(void);

void EFS_FreeFIND(EFS_FIND_T *);






/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // End EFS_FCB_H   