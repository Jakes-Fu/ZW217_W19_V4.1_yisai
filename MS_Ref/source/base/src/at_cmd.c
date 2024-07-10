/******************************************************************************
 ** File Name:      at_cmd.c                                                  *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           07/06/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines some common data for all of             *
 **                 AT Command.                                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/06/2002     Xueliang.Wang    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#define _ATCMD_C_ENTRY
//#include "at_cmd.h"

#include "at_mcu.h"		//AT Command about MCU
#include "at_nv.h"		//AT Command about NVItem
#include "at_dsp.h"     //AT Command about DSP

// Add new head files for new Command at here.

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

// AT token string.
const uint8 g_diag_atcmd_strtoken[] =
    {' ',',','{','}','[',']','(',')',':',';','!','?','<','>',0};

// AT Command Table.
const AT_CMD_FUNC_T g_diag_atcmd_table[] = 
{
	{"MCU", ATMCU_HandleMcu},
	{"NV",  ATNV_HandleNv},
	{"DSP", ATDSP_HandleDsp}
	
	// Add new type Command SYMBOL and correspond FUNCTION at here.

};

// AT command number.
const uint32 g_diag_atcmd_number = 
    (sizeof(g_diag_atcmd_table) / sizeof(AT_CMD_FUNC_T));

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

