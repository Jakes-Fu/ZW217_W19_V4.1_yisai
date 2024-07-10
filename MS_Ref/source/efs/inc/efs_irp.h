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
 ** File Name:      efs_irp.h                                                 *
 ** Author:         lin.liu                                                   *
 ** DATE:           11/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file includes the definition and interface of IRP    *
 **                 IRP (IO Request Packet)
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 22/08/2001     Lin.liu          Create.                                   *
 ******************************************************************************/

#ifndef EFS_IRP_H
#define EFS_IRP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "efs_fcb.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         IRP type                                          *
 **---------------------------------------------------------------------------*/
typedef enum EFS_IRP_CMD_tag
{
        IRP_CREATE  = 0,  // create/open a file 
        IRP_CLOSE,        // close a file that opened by create.
        IRP_DELETE,       // delete a file
        IRP_READ,         // read from a file that has opened
        IRP_WRITE,        // write to a file that has opened
        IRP_RENAME,       // rename a file
        IRP_SEARCH,       // search  a file
        IRP_READ_ITEM,    // read item 
        IRP_WRITE_ITEM,   // write item , if not exist, malloc and write
        IRP_DELETE_ITEM,  // Delete the item that in given range
        IRP_DIRECT_READ,  // access flash device directly.
                          // only used by system services.
        IRP_DIRECT_WRITE, // access flash device directly.

        IRP_CREATE_DIR,
        IRP_DELETE_DIR,
        
        IRP_SEEK,         // seek the file pos
        IRP_CHANGE_SIZE,  // change the file size
        IRP_FLUSH_NVITEM, // flush the nvitem
        IRP_ERASE_PRODUCT_ITEM, // erase product field
  #ifdef EFS_SPEED_UP
        IRP_GET_REFRESH_STATUS //get refresh status and trig refresh
 #endif
}EFS_IRP_CMD_E;


typedef struct EFS_IRP_CREATE_tag{
    /* in */  uint8  *name;  // file name
    /* in */  uint16  mode;  // create mode.
}EFS_IRP_CREATE_T;

typedef struct
{
    EFS_FCB_T   *fcb_ptr;
}EFS_IRP_CLOSE_T;

typedef struct EFS_IRP_DELETE_tag{   
    uint8   *name;  // file name
}EFS_IRP_DELETE_T;

typedef struct EFS_IRP_TRANSMIT_tag{
    EFS_FCB_T     *fcb_ptr;   // if no, set NULL
    uint8   *buf;    // the pointer of buffer
    uint32  offset;  // the offset from the begin of the file.
    uint32  len;     // the length that being transmitted.
}EFS_IRP_TRANSMIT_T;

typedef struct EFS_IRP_RENAME_tag{
    uint8   *old_name;  // file name
    uint8   *new_name;  // new file name
}EFS_IRP_RENAME_T;

typedef struct EFS_IRP_TRANSMIT_ITEM_tag{
    ITEM_T  identifier;   // the identifier of the item.
    uint16  count;        // the count that being transmitted.
    uint8   *buf_ptr;     // the pointer of buffer.
    uint8   b_async;      // transmit mode. If async mode, shoud free the buf_ptr
}EFS_IRP_TRANSMIT_ITEM_T;

typedef struct EFS_IRP_DELETE_ITEM_tag{
    ITEM_T   start_identifier;  // delete the item from the start_identifier
    ITEM_T   end_identifier;    // stop delete the item when > end_identifier
}EFS_IRP_DELETE_ITEM_T;



/* this struct for first search  */
typedef struct EFS_IRP_SEARCH_tag{
	EFS_FIND_T          *find;      
    uint8               *search_str;
    EFS_FIND_DATA_T     *find_data;
}EFS_IRP_SEARCH_T;

typedef struct EFS_IRP_DIRECT_tag{
    uint8   *buf;
    uint32  addr;
    uint32  len;
}EFS_IRP_DIRECT_T;

typedef struct
{
    uint8 *path;
}EFS_IRP_DIR_T;

typedef struct
{
    EFS_FCB_T * fcb_ptr;
    uint32      offset;
}EFS_IRP_SEEK_T;

typedef struct
{
    EFS_FCB_T * fcb_ptr;
    uint32      size;
}EFS_IRP_CHANGE_SIZE_T;

#define EFS_IRP_FLAG        0x4952505F

/*
    //this struct MUST be uint8 boundary
*/
typedef struct EFS_IRP_TAG
{
    uint32                		irp_flag;
    EFS_IRP_CMD_E         		cmd;    
    EFS_COMPLETETION_ROUTINE  	callback;
    EFS_PARAM             		param;  // the param of overlapped
    union
    {
        EFS_IRP_CREATE_T        create;
        EFS_IRP_CLOSE_T         close;
        EFS_IRP_DELETE_T        del;
        EFS_IRP_TRANSMIT_T      transmit;
        EFS_IRP_RENAME_T        rename;
        EFS_IRP_SEARCH_T        search;        
        EFS_IRP_TRANSMIT_ITEM_T transmit_item;
        EFS_IRP_DELETE_ITEM_T   delete_item;
        EFS_IRP_DIRECT_T        direct;
        EFS_IRP_DIR_T           dir;
        EFS_IRP_SEEK_T          seek;
        EFS_IRP_CHANGE_SIZE_T   chsize;
    }irp_param;
}EFS_IRP_T;

typedef struct
{
    xSignalHeaderRec    sig;
    EFS_IRP_T          *irp_ptr;
}EFS_IRP_SIG_T;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

//
//  Allocate a new IRP-Packet.
//
EFS_IRP_T * EFS_MallocIRP(void);

//
//  Free the Packet returned by EFS_MallocIRP
//
void EFS_FreeIRP(EFS_IRP_T *irp_ptr);

// 
//  Send the IO Request Packet to EFS-Server.
//
BOOLEAN EFS_SendIRP(EFS_IRP_T *irp_ptr);

//
//  Get a IRP from the queue of the EFS-Server
//
EFS_IRP_T * EFS_GetHighestIRP(void);

EFS_IRP_T * EFS_PeekIRP( void );
    
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif         /*End efs_irp.h*/