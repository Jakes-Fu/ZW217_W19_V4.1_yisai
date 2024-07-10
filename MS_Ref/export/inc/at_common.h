/******************************************************************************
 ** File Name:      at_common.h                                               *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           18/04/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file include some defines and data structures        *
 **                 for all of AT Command files.                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 18/04/2002     Xueliang.Wang    Create.                                   *
 ******************************************************************************/

#ifndef _AT_COMMON_H
#define _AT_COMMON_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "cm.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define     _ATCMD_SYMBOL_MAX_LENGTH    15	// MAX length of AT Command Symbol
#define     _ATCMD_PARAMETER_MAX_LENGTH 11	// MAX length of Parameter

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef struct AT_CMD_FUNC_tag
{
	//Command symbol.
	uint8	cmd[_ATCMD_SYMBOL_MAX_LENGTH];		

	//Function to process the command.
    uint32  (*func_ptr)( 
        uint8 **dest_ptr,       // Pointer of the response message.
        uint16 *dest_len_ptr,   // Pointer of size of the response message
        const uint8 *src_ptr,   // Pointer of the input message. 
        uint16 src_len);        // Size of the source buffer in uint8.

} AT_CMD_FUNC_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description:    Make a response packet, using the inputed message header.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC  void    ATCOMMON_GenerateRspMsg(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MSG_HEAD_T *msg_head_ptr);  // Inputed pointer of message header struct.


/*****************************************************************************/
// 	Description:    Convert a string to uppercase.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC  uint8   *ATCOMMON_UpperStr( // Pointer to the string altered. 
    uint8 *str_ptr);                // Pointer to string to be capitalized.


/*****************************************************************************/
// 	Description:    Convert strings to an unsigned long integer value.
//                  the base of value can be 2, 8, 10, 16
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC  uint32  ATCOMMON_ConvertStrToUl(   // Return if convert is successful.
    uint32 *value_ptr,  // Pointer to the value represented in the string.
    uint8 *src_ptr,     // Null-terminated value string to convert.
    uint8 **end_ptr);   // Pointer to character that stops scan.

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _AT_COMMON_H

