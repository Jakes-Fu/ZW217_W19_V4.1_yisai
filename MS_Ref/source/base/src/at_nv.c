/******************************************************************************
 ** File Name:      at_nv.c                                                   *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           18/04/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file makes up the AT Command about NV Interface      *
 **                 component. It is dummy now, to be added.                  *   
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 29/05/2002     Xueliang.Wang    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "os_api.h"
#include "cm.h"
#include "cmddef.h"
#include "server.h"

#include "at_common.h"
#include "at_cmd.h"
#include "at_nv.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
		
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
/*
// NVItem Table.
LOCAL const	AT_CMD_FUNC_T	NV_CMD_TABLE_LIST[] = {
	{"READ",  NV_HandleRead},
	{"WRITE", NV_HandleWrite}

	// Add new type Command SYMBOL and correspond FUNCTION about NVItem    
    // AT HERE. 

};

// The Number of Command about NVItem
LOCAL const uint32 NV_CMD_NUMBER = sizeof(NV_CMD_TABLE_LIST) 
                            / sizeof(AT_CMD_FUNC_T);

*/

// Used to save messge head for the lower function 
//      constructing response message.
LOCAL MSG_HEAD_T s_msg_head;


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
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
    uint16 src_len)         // Size of the source buffer in uint8.
{
    //Dummy handle.

	//Save MSG_HEAD_T struct, to construct response message.
	SCI_MEMCPY((void*)&s_msg_head, (void*)src_ptr, sizeof(MSG_HEAD_T));

	s_msg_head.subtype = ATCMD_SUCCESS;
	ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, &s_msg_head);

	return ATCMD_SUCCESS;
}

/**---------------------------------------------------------------------------*
 **		        			LOCAL functions                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
