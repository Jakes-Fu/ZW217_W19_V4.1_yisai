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
 ** File Name:      efs_comm.h                                                *
 ** Author:         lin.liu                                                   *
 ** DATE:           04/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 **                                                                           *
 ** Description:    This file include some common interfaces:                 *
 **                     EFS_StrCpy                                            *
 **                     EFS_StrLen                                            *
 **                     EFS_CompareStr                                        *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/12/2001     lin.liu          Create.                                   *
 ** 16/12/2003     lin.liu          Modify the structure                      *
 ******************************************************************************/
#ifndef EFS_COMMON_H
#define EFS_COMMON_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "efs.h"
#include "efs_item.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


#ifdef _DEBUG
 #define EFS_ASSERT(EXP)  SCI_ASSERT(EXP)
#else
 #define EFS_ASSERT(...)  
#endif

//
//    EFS Block Define.
//
typedef struct
{
    ITEM_T    prev;      // the ID of prev block.
                         // If this block is the first one, the prev id is file header item.
    uint16    data_size; // The valid data size in data buffer.
                         // if > EFS_BLOCK_SIZE, means invalid block
    uint8     data[EFS_BLOCK_SIZE]; // The buffer store the data. the valid data is from 
                                    // data[0] to data[ data_size - 1 ].
}EFS_BLOCK_T;
  
//
//    EFS File Header Define.
// 
#define FILE_HEADER_FLAG      0x46464854    // FFHD
typedef struct
{
    uint32    flag;         // file header flag. should be equal with FILE_HEADER_FLAG.
                            // It is not used now.
    uint32    create_time;  // the current time when create the file.
    uint16    attr;         // file attribute. see efs.h to get more information.
    uint8     name[EFS_MAX_PATH + 2];  // +2, so can added terminal safely.
}EFS_HEADER_T;
 
//
// Copy string. (include the NULL-terminal.)
//
void EFS_StrCpy(
    char *dest,        //Destination string
    const char *sour); // Null-terminated source string
    
//
// Calculator the length of string.
//    
uint32 EFS_StrLen(      // return the length of a string.(include Null-terminated char)
    const char *name);  // Null-terminated string
    
//
// Compare two string.
//
BOOLEAN EFS_CompareStr( // If dest-string is equal to to sour-string, return TRUE.
    const char  *dest,  // Null-terminated string, can contain wildcard.('*', '?')
    const char  *sour); // Null-terminated string
    

//
// Parse the file name, and get the parts of it.
//    
BOOLEAN GetParts_Of_FileName(
	char *dest, const char *file, uint  parts);


/*
    Check the file name is valid format and no invalid chars in it.
*/
BOOLEAN check_name(const char *name);

//
// Malloc a Data block item.
// Return:
//    the item ID that allocated. If failed, return EFS_INVALD_ITEM(0xFFFF) 
// 
uint16 EFS_MallocDataBlock( EFS_BLOCK_T * init_data );

//
// Malloc a file header item.
// Return:
//    the item ID that allocated. If failed, return EFS_INVALD_ITEM(0xFFFF) 
//
uint16 EFS_MallocFileHeader( EFS_HEADER_T  * init_data );
    

//
// Get the next block ID.
// Input:
//    The current block ID.
// Return:
//    The next block ID, if not found, return EFS_INVALID_ITEM.   
//
uint16 get_next_block_id( uint16  block_id );

//
// Get the first block ID that belong to the file.
// Input:
//    The file header item ID.
// Return:
//    The first block ID. if not found, return EFS_INVALID_ITEM.
//
uint16 get_first_block_id( uint16  header_id );

//
// Get the last block ID.
// Input:
//    The file header item ID.
// Return:
//    The last block ID. if not found, return EFS_INVALID_ITEM
uint16 get_last_block_id( uint16  header_id );

//
// Add block ID to the logic table.
//    block_logic_table_next[ cur_block_id ] = next_block_id
//
void add_block_id_to_logic_table( uint16  cur_block_id,  uint16  next_block_id );

//
// Add first block ID to the logic table.
//    first_block_table[ header_id ] = first_block_id
//
void add_first_block_id_to_logic_table( uint16  header_id, uint16 first_block_id );

//
// Init the file system. build the logic table.
// Free all invalid block/file header.
//
void init_file_system( void );



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // End EFS_COMMON_H