/******************************************************************************
 ** File Name:      at_cmd.h                                                  *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           19/02/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file include some defines for at_cmd.c               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 19/02/2002     Xueliang.Wang    Create.                                   *
 ** 18/04/2002     Xueliang.Wang    Modify.                                   *
 ** 22/05/2002     Xueliang.Wang    Modify.                                   *
 ******************************************************************************/

#ifndef _AT_CMD_H
#define _AT_CMD_H

 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "at_common.h"

// Don't add head file at here.

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define ATCMD_EXT extern

// AT token string.
ATCMD_EXT const uint8 g_diag_atcmd_strtoken[];

// AT Command Table.
ATCMD_EXT const AT_CMD_FUNC_T	g_diag_atcmd_table[];
    
// AT command number.
ATCMD_EXT const uint32 g_diag_atcmd_number;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _AT_CMD_H

