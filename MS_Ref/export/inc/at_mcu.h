/******************************************************************************
 ** File Name:      at_mcu.h                                                  *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           18/04/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file contains all the definitions necessary   *
 **                 to interface with at_mcu.c                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 18/04/2002     Xueliang.Wang    Create.                                   *
 ******************************************************************************/

#ifndef _AT_MCU_H
#define _AT_MCU_H

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

typedef struct MCUCMD_PARAM_tag
{
	uint8	cmd[_ATCMD_SYMBOL_MAX_LENGTH];  // Command symbol.
	uint16	len;                            // Read or write memory length
                                            // in uint8/uint16/uint32.
	uint16  padding;
	uint32	addr;                           // Access address.
	void	*value_ptr;                     // Pointer of value string.
} MCUCMD_PARAM_T;


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Handle all MCU command. If success, make a response message
//                  by dest_ptr, else don't make response message.
//	Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC	uint32 ATMCU_HandleMcu( // Handle result, success or error code.
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

#endif  // End of _AT_MCU_H

