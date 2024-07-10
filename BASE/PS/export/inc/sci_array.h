/******************************************************************************
 ** File Name:      sci_array.h                                               *
 ** Author:         fancier fan                                               *
 ** Date:           07/23/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file contains general types and macros that   *
 **         		are of use to all modules.The values or definitions are   *
 ** 				dependent on the specified target.                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 07/23/2001     fancier fan     Create.                                    *
 ******************************************************************************/
#ifndef SCI_ARRAY_H
#define SCI_ARRAY_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/
#include "sci_types.h"



/*****************************************************************************/
// 	Description : find specified item in a array
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
/*
BOOLEAN SCI_FindArrayItem(   // true means found the item ,false not found
         void*    array_ptr, // in: array point 
         uint32   array_len, // in: array length
         uint32   item_len,  // in: item length
         void*    item_ptr,  // in: which item should be deleted
         uint32*  item_pos_ptr  // out:item position 
         );
*/
/*****************************************************************************/
// 	Description : delete item from a array,and modify its length
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
BOOLEAN SCI_DeleteArrayItem(   // true means deleted,false means no item to delete
         void*    array_ptr,      // in/out: array value changed
         uint8*  array_len_ptr,  // in/out: array length
         uint32   item_len,       // in:  item length
         void*    item_ptr        // in:  which item should be deleted
         );

/*****************************************************************************/
// 	Description : insert item into a array,and modify its length
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
BOOLEAN SCI_InsertArrayItem(   // true means insert success,false means fail
         void*    array_ptr,      // in/out: array changed
         uint8*  array_len_ptr,  // in/out: array length
         uint32   item_len,       // in:  item length
         void*    item_ptr,       // in:  which item should be insert
         uint32   max_array_len   // in:  max array len
         );


#endif  /* SCI_ARRAY_H */
