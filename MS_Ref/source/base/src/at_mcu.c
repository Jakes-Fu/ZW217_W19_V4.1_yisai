/******************************************************************************
 ** File Name:      at_mcu.c                                                  *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           18/04/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file makes up the AT Command about MCU Interface     *
 **                 component. It handle all command about MCU. such as       *
 **                 readb, readw, readd, writeb, writew and writed.           * 
 ******************************************************************************
 
 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 18/04/2002     Xueliang.Wang    Create.                                   *
 ** 14/05/2002     Xueliang.Wang    Modify.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "os_api.h"
#include "cm.h"
#include "tasks_id.h"
#include "cmddef.h"
#include "server.h"
#include "keypad.h"
#include "power.h"

#include "at_common.h"
#include "at_cmd.h"
#include "at_mcu.h"

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
#define BYTE_TYPE_DATA      1
#define WORD_TYPE_DATA      2
#define DWORD_TYPE_DATA     4

#define BYTE_DISPLAY_SIZE   3
#define WORD_DISPLAY_SIZE   5
#define DWORD_DISPLAY_SIZE  9
#define PRINT_NUMBER        10

#define PACKET_TOTAL_LEN    450     // Serial port packet len is 500 now
#define BYTE_ACCESS_LEN     (PACKET_TOTAL_LEN / BYTE_DISPLAY_SIZE)
#define WORD_ACCESS_LEN     (PACKET_TOTAL_LEN / WORD_DISPLAY_SIZE)
#define DWORD_ACCESS_LEN    (PACKET_TOTAL_LEN / DWORD_DISPLAY_SIZE)
		
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Handle READB command, read bytes from memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleReadB(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    Handle READW command, read words from memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleReadW(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    Handle READD command, read doublewords from memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleReadD(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    Handle WRITEB command, write bytes to memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleWriteB(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    Handle WRITEW command, write words to memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleWriteW(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    Handle WRITED command, write doublewords to memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleWriteD(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    Handle PowerOn command, Power on the MSSim.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandlePowerOn(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    Handle PowerOff command, Power off the MSSim.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandlePowerOff(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    Convert string to a number.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  IsValidNum(   // Convert result, success or error code.
    uint8 *str_ptr,     // String to be converted.
    uint32 *value_ptr,  // Save the convert result value.
    uint8 type);        // Data type, byte/word/doubleword.


/*****************************************************************************/
//  Description:    Parse MCU Command parameter.
//	Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  ParseMcuParam(    // Parse result, success or error code.
    uint8 *str_ptr,             // Source string, Addr + Len
    MCUCMD_PARAM_T *cmd_ptr);   // To save paramter parsed from str_ptr.


/*****************************************************************************/
//  Description:    Check if MCU Command parameter is valid.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  IsValidMcuParam(  // Check result, success or error code.
    MCUCMD_PARAM_T *cmd_ptr);   // Parameter to checked.


/*****************************************************************************/
//  Description:    Read bytes from the memory, and convert to charater string.
//                  then generate the response message.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_ReadBMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr);   // Inputed command parameter struct.


/*****************************************************************************/
//  Description:    Read words from the memory, and convert to charater string.
//                  then generate the response message.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_ReadWMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr);   // Inputed command parameter struct.


/*****************************************************************************/
//  Description:    Read doublewords from the memory, and convert to charater string 
//                  then generate the response message.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_ReadDMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr);   // Inputed command parameter struct.


/*****************************************************************************/
//  Description:    Write bytes value into memory.
//	Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_WriteBMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr);   // Inputed command parameter struct.


/*****************************************************************************/
//  Description:    Write words value into memory.
//	Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_WriteWMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr);   // Inputed command parameter struct.


/*****************************************************************************/
//  Description:    Write doublewords value into memory.
//	Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_WriteDMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr);   // Inputed command parameter struct.


/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/

// MCU Table.
LOCAL const	AT_CMD_FUNC_T	MCU_CMD_TABLE_LIST[] = {
	{"READB",  MCU_HandleReadB},
	{"WRITEB", MCU_HandleWriteB},
	{"READW",  MCU_HandleReadW},
	{"WRITEW", MCU_HandleWriteW},
	{"READD",  MCU_HandleReadD},
	{"WRITED", MCU_HandleWriteD},
	{"POWERON", MCU_HandlePowerOn},
	{"POWEROFF", MCU_HandlePowerOff}

	// Add new type Command SYMBOL and correspond FUNCTION about MCU    
    // AT HERE. 
};

// The Number of Command about MCU
LOCAL const uint32 MCU_CMD_NUMBER = sizeof(MCU_CMD_TABLE_LIST) 
                            / sizeof(AT_CMD_FUNC_T);


// Used to save messge head for the lower function 
//      constructing response message.
LOCAL MSG_HEAD_T s_msg_head;


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
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
    uint16 src_len)         // Size of the source buffer in uint8.
{
	uint32	i               = 0;
	uint32	parsed_len      = 0;
	uint8	*temp_src_ptr   = SCI_NULL;
	uint32	status          = ATCMD_BADCOMMAND;
	uint8	cmd_symbol[_ATCMD_SYMBOL_MAX_LENGTH];

    // Check input parameter.
    SCI_ASSERT((SCI_NULL == *dest_ptr) && (SCI_NULL != src_ptr));/*assert verified*/

	// Save MSG_HEAD_T struct, to construct response message.
	SCI_MEMCPY((void*)&s_msg_head, (const void*)src_ptr, sizeof(MSG_HEAD_T));

    // Convert src_ptr from const uint8* to uint8*, and offset.
	temp_src_ptr = (uint8*)(src_ptr + sizeof(MSG_HEAD_T));
	
    // Convert source string to uppercase.
    temp_src_ptr = ATCOMMON_UpperStr(temp_src_ptr);	

    SCI_DisableIRQ();
    
    // Get "MCU".
	temp_src_ptr = (uint8*)strtok((char*)temp_src_ptr, 
        (char*)g_diag_atcmd_strtoken);
    strcpy((char*)cmd_symbol, (char*)temp_src_ptr);

    // Get Command Symbol.
	temp_src_ptr = (uint8*)strtok(SCI_NULL, 
        (char*)g_diag_atcmd_strtoken);
    
    SCI_RestoreIRQ();
    
	if ((temp_src_ptr != SCI_NULL) 
        && (strlen((char*)temp_src_ptr) <= _ATCMD_SYMBOL_MAX_LENGTH))
	{
		/*
		    |------------|-----|-|----------------|-|--------|-|
			| MSG_HEAD_T | MCU |0| Command Symbol |0| ...... |0|
			|--------------------|----------------|------------|
			|src                 |temp_ptr		  |
			|									               |
			|<-----------------temp_len----------------------->|
		*/
		strcpy((char*)cmd_symbol, (char*)temp_src_ptr);
		parsed_len = (temp_src_ptr - src_ptr) + strlen((char*)temp_src_ptr) + 1;

		// Call the function according to the Command Symbol, 
        //   which defined in mcu_cmd_table.
		for (i = 0; i < MCU_CMD_NUMBER; i++)
		{
			if (0 == strcmp((char*)MCU_CMD_TABLE_LIST[i].cmd, (char*)cmd_symbol))
			{
				status = (*MCU_CMD_TABLE_LIST[i].func_ptr)(
                                    dest_ptr, 
                                    dest_len_ptr, 
                                    src_ptr + parsed_len, 
                                    (uint16)(src_len - parsed_len));
				break;
			}
		}
	}

	return status;
}


/**---------------------------------------------------------------------------*
 **		        			LOCAL functions                                   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Handle READB command, read bytes from memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleReadB(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint8	*temp_src_ptr   = SCI_NULL;
	uint32	status          = ATCMD_SUCCESS; // Handle result,success or other.
    MCUCMD_PARAM_T	parameter_s;

    // Check input parameter.
    SCI_ASSERT((SCI_NULL == *dest_ptr) /*assert verified*/
        && (SCI_NULL != src_ptr) 
        && (src_len > 0));

    // Initialize struct parameter_s.
    strcpy((char*)parameter_s.cmd, "READB");
    parameter_s.len         = 0;
    parameter_s.addr        = 0;
    parameter_s.value_ptr  = SCI_NULL;

    // Parse parameters, Address and Length.
    temp_src_ptr = (uint8*)src_ptr ;  
	status = ParseMcuParam(temp_src_ptr, &parameter_s);
	if (ATCMD_SUCCESS == status)
	{
		// Check if parameters is valid.
		status = IsValidMcuParam(&parameter_s);
		if (ATCMD_SUCCESS == status)
		{
			// Read value from the memory, and convert to character string
            //  then generate response message if success.
            status = MCU_ReadBMemory(dest_ptr, dest_len_ptr, &parameter_s);
    
        } // End check parameter

    } // End parse parameter

	return status;

}

/*****************************************************************************/
//  Description:    Handle READW command, read words from memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleReadW(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint8	*temp_src_ptr   = SCI_NULL;
	uint32	status          = ATCMD_SUCCESS; // Handle result,success or other.
    MCUCMD_PARAM_T	parameter_s;

    // Check input parameter.
    SCI_ASSERT((SCI_NULL == *dest_ptr) /*assert verified*/
        && (SCI_NULL != src_ptr) 
        && (src_len > 0));

    // Initialize struct parameter_s.
    strcpy((char*)parameter_s.cmd, "READW");
    parameter_s.len         = 0;
    parameter_s.addr        = 0;
    parameter_s.value_ptr  = SCI_NULL;

    // Parse parameters, Address and Length.
    temp_src_ptr = (uint8*)src_ptr ;  
	status = ParseMcuParam(temp_src_ptr, &parameter_s);
	if (ATCMD_SUCCESS == status)
	{
		// Check if parameters is valid.
		status = IsValidMcuParam(&parameter_s);
		if (ATCMD_SUCCESS == status)
		{
			// Read value from the memory, and convert to character string
            //  then generate response message if success.
            status = MCU_ReadWMemory(dest_ptr, dest_len_ptr, &parameter_s);
    
        } // End check parameter

    } // End parse parameter

	return status;

}

/*****************************************************************************/
//  Description:    Handle READD command, read doublewords from memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleReadD(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint8	*temp_src_ptr   = SCI_NULL;
	uint32	status          = ATCMD_SUCCESS; // Handle result,success or other.
    MCUCMD_PARAM_T	parameter_s;

    // Check input parameter.
    SCI_ASSERT((SCI_NULL == *dest_ptr) /*assert verified*/
        && (SCI_NULL != src_ptr) 
        && (src_len > 0));

    // Initialize struct parameter_s.
    strcpy((char*)parameter_s.cmd, "READD");
    parameter_s.len         = 0;
    parameter_s.addr        = 0;
    parameter_s.value_ptr  = SCI_NULL;

    // Parse parameters, Address and Length.
    temp_src_ptr = (uint8*)src_ptr ;  
	status = ParseMcuParam(temp_src_ptr, &parameter_s);
	if (ATCMD_SUCCESS == status)
	{
		// Check if parameters is valid.
		status = IsValidMcuParam(&parameter_s);
		if (ATCMD_SUCCESS == status)
		{
			// Read value from the memory, and convert to character string
            //  then generate response message if success.
            status = MCU_ReadDMemory(dest_ptr, dest_len_ptr, &parameter_s);
    
        } // End check parameter

    } // End parse parameter

	return status;

}

/*****************************************************************************/
//  Description:    Handle WRITEB command, write bytes to memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleWriteB(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint8	*temp_src_ptr = SCI_NULL;
	uint32	status        = ATCMD_SUCCESS; // Handle result, success or other.
    MCUCMD_PARAM_T	parameter_s;

    // Check input parameter.
    SCI_ASSERT((SCI_NULL == *dest_ptr) /*assert verified*/
        && (SCI_NULL != src_ptr) 
        && (src_len > 0));

    // Initialize struct parameter_s.
	strcpy((char*)parameter_s.cmd, "WRITEB");
    parameter_s.len         = 0;
    parameter_s.addr        = 0;
    parameter_s.value_ptr  = SCI_NULL;

	// Parse parameters, Address and Length.
    temp_src_ptr = (uint8*)src_ptr;
	status = ParseMcuParam(temp_src_ptr, &parameter_s);
	if (ATCMD_SUCCESS == status)
	{
		// Check if parameters is valid.
		status = IsValidMcuParam(&parameter_s);
		if (ATCMD_SUCCESS == status)
		{
			// Write values to the memory, 
            //  and generate the response message if success.
            status = MCU_WriteBMemory(dest_ptr, dest_len_ptr, &parameter_s);
		}
	}

    // If success, generate response message.
	if (ATCMD_SUCCESS == status)
	{
        s_msg_head.subtype = (uint8)status;
    	ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, &s_msg_head);
    }

	return status;

}


/*****************************************************************************/
//  Description:    Handle WRITEW command, write words to memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleWriteW(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint8	*temp_src_ptr = SCI_NULL;
	uint32	status        = ATCMD_SUCCESS; // Handle result, success or other.
    MCUCMD_PARAM_T	parameter_s;

    // Check input parameter.
    SCI_ASSERT((SCI_NULL == *dest_ptr) /*assert verified*/
        && (SCI_NULL != src_ptr) 
        && (src_len > 0));

    // Initialize struct parameter_s.
	strcpy((char*)parameter_s.cmd, "WRITEW");
    parameter_s.len         = 0;
    parameter_s.addr        = 0;
    parameter_s.value_ptr  = SCI_NULL;

	// Parse parameters, Address and Length.
    temp_src_ptr = (uint8*)src_ptr;
	status = ParseMcuParam(temp_src_ptr, &parameter_s);
	if (ATCMD_SUCCESS == status)
	{
		// Check if parameters is valid.
		status = IsValidMcuParam(&parameter_s);
		if (ATCMD_SUCCESS == status)
		{
			// Write values to the memory, 
            //  and generate the response message if success.
            status = MCU_WriteWMemory(dest_ptr, dest_len_ptr, &parameter_s);
		}
	}

    // If success, generate response message.
	if (ATCMD_SUCCESS == status)
	{
        s_msg_head.subtype = (uint8)status;
    	ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, &s_msg_head);
    }

	return status;

}


/*****************************************************************************/
//  Description:    Handle WRITED command, write doublewords to memory.
//	Global resource dependence: None
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandleWriteD(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint8	*temp_src_ptr = SCI_NULL;
	uint32	status        = ATCMD_SUCCESS; // Handle result, success or other.
    MCUCMD_PARAM_T	parameter_s;

    // Check input parameter.
    SCI_ASSERT((SCI_NULL == *dest_ptr) /*assert verified*/
        && (SCI_NULL != src_ptr) 
        && (src_len > 0));

    // Initialize struct parameter_s.
	strcpy((char*)parameter_s.cmd, "WRITED");
    parameter_s.len         = 0;
    parameter_s.addr        = 0;
    parameter_s.value_ptr  = SCI_NULL;

	// Parse parameters, Address and Length.
    temp_src_ptr = (uint8*)src_ptr;
	status = ParseMcuParam(temp_src_ptr, &parameter_s);
	if (ATCMD_SUCCESS == status)
	{
		// Check if parameters is valid.
		status = IsValidMcuParam(&parameter_s);
		if (ATCMD_SUCCESS == status)
		{
			// Write values to the memory, 
            //  and generate the response message if success.
            status = MCU_WriteDMemory(dest_ptr, dest_len_ptr, &parameter_s);
		}
	}

    // If success, generate response message.
	if (ATCMD_SUCCESS == status)
	{
        s_msg_head.subtype = (uint8)status;
    	ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, &s_msg_head);
    }

	return status;

}

/*****************************************************************************/
//  Description:    Handle PowerOn command, Power on the MSSim.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandlePowerOn(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint32  status = ATCMD_SUCCESS;
	xSignalHeader power_signal_ptr;

    // Check if it is the status of POWER OFF.
    if (POWER_MS_STATUS_POWER_OFF == POWER_GetMsStatus())
    {
        // It is Power off.

        power_signal_ptr = (xSignalHeader)SCI_ALLOC_APP(sizeof(xSignalHeaderRec));
        SCI_ASSERT(SCI_NULL != power_signal_ptr);/*assert verified*/

    	// Send message to HANDSET to Power On it.
        power_signal_ptr->Sender = DIAG;
    	power_signal_ptr->Pre = SCI_NULL;
	    power_signal_ptr->Suc = SCI_NULL;
    	power_signal_ptr->SignalCode = KPD_SOFT_POWER_ON;
	    power_signal_ptr->SignalSize = sizeof(xSignalHeaderRec);

	    SCI_SendSignal(power_signal_ptr, KPDSVR);

        status = ATCMD_SUCCESS;
    }
    else
    {
        status = ATCMD_REPOWERON;
    }

    // If success, Generate a ATCMD_SUCCESS response message.
    if (ATCMD_SUCCESS == status)
    {        
        s_msg_head.subtype = (uint8)status;
      	ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, &s_msg_head);
    }
    
	return status;
}

/*****************************************************************************/
//  Description:    Handle PowerOff command, Power off the MSSim.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_HandlePowerOff(    // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint32  status = ATCMD_SUCCESS;
	xSignalHeader power_signal_ptr;

    // Check if it is the status of POWER ON.
    if (POWER_MS_STATUS_POWER_ON == POWER_GetMsStatus())
    {
        // It is Power on.

        power_signal_ptr = (xSignalHeader)SCI_ALLOC_APP(sizeof(xSignalHeaderRec));
        SCI_ASSERT(SCI_NULL != power_signal_ptr);/*assert verified*/

    	// Send message to HANDSET to Power On it.
        power_signal_ptr->Sender = DIAG;
    	power_signal_ptr->Pre = SCI_NULL;
	    power_signal_ptr->Suc = SCI_NULL;
    	power_signal_ptr->SignalCode = KPD_SOFT_POWER_OFF;
	    power_signal_ptr->SignalSize = sizeof(xSignalHeaderRec);

	    SCI_SendSignal(power_signal_ptr, KPDSVR);

        status = ATCMD_SUCCESS;
    }
    else
    {
        status = ATCMD_REPOWEROFF;
    }

    // If success, Generate a ATCMD_SUCCESS response message.
    if (ATCMD_SUCCESS == status)
    {        
        s_msg_head.subtype = (uint8)status;
      	ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, &s_msg_head);
    }
    
	return status;
}

/**---------------------------------------------------------------------------*
 **					NVItem operate functions                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **	    	        		Some local functions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Convert string to a number.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  IsValidNum(   // Convert result, success or error code.
    uint8 *str_ptr,         // String to be converted.
    uint32 *value_ptr,      // Save the convert result value.
    uint8 type)             // Data type, byte/word/doubleword.
{
	uint8	*end_ptr    = SCI_NULL;
	uint32	status      = ATCMD_BADPARAMETER;
	
	if((str_ptr == SCI_NULL) 
        || (strlen((char*)str_ptr) >= _ATCMD_PARAMETER_MAX_LENGTH))
	{
		return ATCMD_BADPARAMETER;
	}

    // Convert string to number.
    status = ATCOMMON_ConvertStrToUl(value_ptr, str_ptr, &end_ptr);
	if(ATCMD_SUCCESS == status)
	{
		// Check if the value is overflow.
		status = ATCMD_SUCCESS;
		switch (type)
		{
		case BYTE_TYPE_DATA:
			if (*value_ptr > 0xFF)
			{
				status = ATCMD_BADPARAMETER;
			}
			break;

		case WORD_TYPE_DATA:
			if (*value_ptr > 0xFFFF)
			{
				status = ATCMD_BADPARAMETER;
			}
			break;

		case DWORD_TYPE_DATA:
			break;

		default:
			{
				status = ATCMD_BADPARAMETER;
			}
			break;
		}
	}

	return status;
}

/*****************************************************************************/
//  Description:    Parse MCU Command parameter.
//	Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  ParseMcuParam(    // Parse result, success or error code.
    uint8 *str_ptr,             // Source string, Addr + Len
    MCUCMD_PARAM_T *cmd_ptr)    // To save paramter parsed from str_ptr.
{
	uint32	status      = ATCMD_SUCCESS;
	uint32	temp_value  = 0;
	uint8	*temp_ptr   = SCI_NULL;

    // Check input parameter.
    SCI_ASSERT((str_ptr != SCI_NULL) 
        && (cmd_ptr != SCI_NULL) 
        && (cmd_ptr->cmd != SCI_NULL));/*assert verified*/

	// Convert source string to upper case.
	ATCOMMON_UpperStr(str_ptr);

    SCI_DisableIRQ();
    
	// Get address.
	temp_ptr = (uint8*)strtok((char*)str_ptr, (char*)g_diag_atcmd_strtoken);
	status = IsValidNum(temp_ptr, &cmd_ptr->addr, DWORD_TYPE_DATA);
	if (status != ATCMD_SUCCESS)
	{
	    SCI_RestoreIRQ();
		return status;
	}

	// Get length.
	temp_ptr = (uint8*)strtok(SCI_NULL, (char*)g_diag_atcmd_strtoken);

	SCI_RestoreIRQ();

	status = IsValidNum(temp_ptr, &temp_value, WORD_TYPE_DATA);
	if (ATCMD_SUCCESS != status)
	{
		return status;
	}
	else
	{
		cmd_ptr->len = (uint16)temp_value;
	}

	// To support WRITEB/WRITEW/WRITED operate, point to the string of values.
	if (*(cmd_ptr->cmd) == 'W')
	{		
		cmd_ptr->value_ptr = (void*)(temp_ptr + strlen((char*)temp_ptr) + 1);
	}

	return status;

}

/*****************************************************************************/
//  Description:    Check if MCU Command parameter is valid.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  IsValidMcuParam(  // Check result, success or error code.
    MCUCMD_PARAM_T *cmd_ptr)    // Parameter to checked.

{
	uint32	status      = ATCMD_BADPARAMETER;
    uint32  addr        = 0;
    uint32  len         = 0;
    uint32  addr_len    = 0;

    // Check input parameter.
    SCI_ASSERT((cmd_ptr != SCI_NULL) && (cmd_ptr->cmd != SCI_NULL));/*assert verified*/

    addr        = cmd_ptr->addr;
    len         = cmd_ptr->len;
    addr_len    = addr + len;

    // Check according with operating type, uint8/uint16/uint32.
	if ((!strcmp((char*)cmd_ptr->cmd, "READB")) 
        || (!strcmp((char*)cmd_ptr->cmd, "WRITEB")))
	{
       	// Length less than BYTE_ACCESS_LEN.
        // Address and (Address + Length) range:
        //          0x00000000 - 0x0FFFFFFF
	    //		OR	0x80000000 - 0xBFFFFFFF

        // Check length.
        if (len <= BYTE_ACCESS_LEN)
        {
    		// Check address range.
	        if ((addr <= 0x0FFFFFFF)
	    		|| ((addr >= 0x80000000) && (addr <= 0xBFFFFFFF)))
    		{
	    		// Check (Address + Length) range.
    			if ((addr_len <= 0x0FFFFFFF)
	    			|| ((addr_len >= 0x80000000) && (addr_len <= 0xBFFFFFFF)))
			    {
				    status = ATCMD_SUCCESS;
                }
                else
                {
                    status = ATCMD_BADADDRLEN;
                } // End (address + length) range
            }
            else
            {
                status = ATCMD_BADADDRESS;
            } // End address range
        }
        else
        {
            status = ATCMD_BADLENGTH;
        } // End len range
	}
	else if ((!strcmp((char*)cmd_ptr->cmd, "READW")) 
            || (!strcmp((char*)cmd_ptr->cmd, "WRITEW")))
	{
       	// Address could be divided by 2.
        // Length less than WORD_ACCESS_LEN.
        // Address and (Address + Length) range:
   		//                 	0x00000000 - 0x0FFFFFFF
	    //				OR	0x10000000 - 0x1FFFFFFF
		//				OR	0x80000000 - 0xBFFFFFFF

        // Check length .
		if (len <= WORD_ACCESS_LEN)
		{
			// Check address range, and if the Address could be divided by 2.
			if ((0 == (addr & 0x1))
                && ((addr <= 0x0FFFFFFF)
				||  ((addr >= 0x10000000) && (addr <= 0x1FFFFFFF))
				||  ((addr >= 0x80000000) && (addr <= 0xBFFFFFFF))))
			{
				// Check (Address + Length) range.
				if ((addr_len <= 0x0FFFFFFF)
					|| ((addr_len >= 0x10000000) && (addr_len <= 0x1FFFFFFF))
					|| ((addr_len >= 0x80000000) && (addr_len <= 0xBFFFFFFF)))
				{
					status = ATCMD_SUCCESS;
                }
                else
                {
                    status = ATCMD_BADADDRLEN;
                } // End (address + length) range
            }
            else
            {
                status = ATCMD_BADADDRESS;
            } // End address range
        }
        else
        {
            status = ATCMD_BADLENGTH;
        } // End len range
    }
	else if ((!strcmp((char*)cmd_ptr->cmd, "READD")) 
            || (!strcmp((char*)cmd_ptr->cmd, "WRITED")))
	{		
       	// Address could be divided by 4.
        // Length less than DWORD_ACCESS_LEN.
        // Address and (Address + Length) range:
        //                  0x00000000 - 0x0FFFFFFF
	    //				OR	0x20000000 - 0x2FFFFFFF
		//				OR	0x80000000 - 0xBFFFFFFF

        // Check length.
		if (len <= WORD_ACCESS_LEN)
		{
			// Check Address range, and if the Address could be divided by 4.
			if ((0 == (addr & 0x3))
                &&((addr <= 0x0FFFFFFF)
				|| ((addr >= 0x20000000) && (addr <= 0x2FFFFFFF))
				|| ((addr >= 0x80000000) && (addr <= 0xBFFFFFFF))))
			{
				// Check Length range.
                // Length range:    the sum of Address and Length 
                //                  should in the Address range.
				if ((addr_len <= 0x0FFFFFFF)
					|| ((addr_len >= 0x20000000) && (addr_len <= 0x2FFFFFFF))
					|| ((addr_len >= 0x80000000) && (addr_len <= 0xBFFFFFFF)))
				{
					status = ATCMD_SUCCESS;
                }
                else
                {
                    status = ATCMD_BADADDRLEN;
                } // End (address + length) range
            }
            else
            {
                status = ATCMD_BADADDRESS;
            } // End address range
        }
        else
        {
            status = ATCMD_BADLENGTH;
        } // End len range
	}
	else
	{
		//Will not excute.
		status = ATCMD_BADPARAMETER;
	}

    // If the command is WRITEB/WRITEW/WRITED, should continue checking
    // because the area between 0x00000000 - 0x02000000 cann't write.
    if ((ATCMD_SUCCESS == status) 
        && (*(char*)cmd_ptr->cmd == 'W'))
    {
        // Address range should out of 0x00000000 - 0x02000000
        if (addr > 0x02000000)
        {
            // (Address + Len) range should out of 0x00000000 - 0x02000000
            if (addr_len > 0x02000000)
            {
                status = ATCMD_SUCCESS;
            }
            else
            {
                status = ATCMD_BADADDRLEN;
            } // End (address + length) range
        }
        else
        {
            status = ATCMD_BADADDRESS;
        } // End address range
    }

	return status;
}

/*****************************************************************************/
//  Description:    Read bytes from the memory, and convert to charater string.
//                  then generate the response message.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_ReadBMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr)    // Inputed command parameter struct.
{
#ifdef WIN32
    // Response OK.
    s_msg_head.subtype = ATCMD_SUCCESS;
	ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, &s_msg_head);
    return  ATCMD_SUCCESS;
#else

	uint32	i               = 0;
	uint8	*temp_dest_ptr  = SCI_NULL;
    uint8	*read_value_ptr = SCI_NULL;

    // Check input parameter.
    SCI_ASSERT((SCI_NULL == *dest_ptr) 
            && (SCI_NULL != cmd_ptr)
            && (SCI_NULL != cmd_ptr->cmd)
            && (cmd_ptr->len > 0));/*assert verified*/

    if (0 != strcmp((char*)cmd_ptr->cmd, "READB"))
    {
        return ATCMD_BADCOMMAND;
    }

	// Calculate dest length, and Malloc a buf for response buf.
    *dest_len_ptr = sizeof(MSG_HEAD_T) 
        + (cmd_ptr->len) * BYTE_DISPLAY_SIZE;

   	// Malloc a memory block for response buffer.
    *dest_ptr =(uint8*)SCI_ALLOC_APP(*dest_len_ptr);
	SCI_ASSERT(SCI_NULL != *dest_ptr);/*assert verified*/
    
    // Get message header.
	s_msg_head.len = *dest_len_ptr;
    SCI_MEMCPY((void*)(*dest_ptr), (void*)(&s_msg_head), sizeof(MSG_HEAD_T));
	
	// Read from memory, and convert it to charater string.
    temp_dest_ptr = *dest_ptr + sizeof(MSG_HEAD_T);	
	read_value_ptr = (uint8*)cmd_ptr->addr;
	
    for (i = 0; i < cmd_ptr->len; )
	{
        if ((cmd_ptr->len - i) >= PRINT_NUMBER)
        {
            // Printf PRINT_NUMBER values together.
		    sprintf((char*)temp_dest_ptr, 
                "%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X ", 
                *(read_value_ptr + i),
                *(read_value_ptr + i + 1),
                *(read_value_ptr + i + 2),
                *(read_value_ptr + i + 3),
                *(read_value_ptr + i + 4),
                *(read_value_ptr + i + 5),
                *(read_value_ptr + i + 6),
                *(read_value_ptr + i + 7),
                *(read_value_ptr + i + 8),
                *(read_value_ptr + i + 9));
		    temp_dest_ptr += PRINT_NUMBER * BYTE_DISPLAY_SIZE;
            i += PRINT_NUMBER;
        }
        else
        {
		    sprintf((char*)temp_dest_ptr, "%.2X ", *(read_value_ptr + i));
		    temp_dest_ptr += BYTE_DISPLAY_SIZE;
            i++;
        }
	}

	return ATCMD_SUCCESS;

#endif

}


/*****************************************************************************/
//  Description:    Read words from the memory, and convert to charater string.
//                  then generate the response message.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_ReadWMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr)    // Inputed command parameter struct.
{
#ifdef WIN32
    // Response OK.
    s_msg_head.subtype = ATCMD_SUCCESS;
	ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, &s_msg_head);
    return  ATCMD_SUCCESS;
#else

	uint32	i               = 0;
	uint8	*temp_dest_ptr  = SCI_NULL;
    uint16	*read_value_ptr = SCI_NULL;

    // Check input parameter.
    SCI_ASSERT((SCI_NULL == *dest_ptr) 
            && (SCI_NULL != cmd_ptr)
            && (SCI_NULL != cmd_ptr->cmd)
            && (cmd_ptr->len > 0));/*assert verified*/

    if (0 != strcmp((char*)cmd_ptr->cmd, "READW"))
    {
        return ATCMD_BADCOMMAND;
    }

	// Calculate dest length, and Malloc a buf for response buf.
    *dest_len_ptr = sizeof(MSG_HEAD_T) 
        + (cmd_ptr->len) * WORD_DISPLAY_SIZE;

   	// Malloc a memory block for response buffer.
    *dest_ptr =(uint8*)SCI_ALLOC_APP(*dest_len_ptr);
	SCI_ASSERT(SCI_NULL != *dest_ptr);/*assert verified*/

    // Get message header.
	s_msg_head.len = *dest_len_ptr;
    SCI_MEMCPY((void*)(*dest_ptr), (void*)(&s_msg_head), sizeof(MSG_HEAD_T));
	
	// Read from memory, and convert it to charater string.
    temp_dest_ptr = *dest_ptr + sizeof(MSG_HEAD_T);	
	read_value_ptr = (uint16*)cmd_ptr->addr;
	for (i = 0; i < cmd_ptr->len; )
	{
        if ((cmd_ptr->len - i) >= PRINT_NUMBER)
        {
            // Printf PRINT_NUMBER values together.
		    sprintf((char*)temp_dest_ptr, 
                "%.4X %.4X %.4X %.4X %.4X %.4X %.4X %.4X %.4X %.4X ", 
                *(read_value_ptr + i),
                *(read_value_ptr + i + 1),
                *(read_value_ptr + i + 2),
                *(read_value_ptr + i + 3),
                *(read_value_ptr + i + 4),
                *(read_value_ptr + i + 5),
                *(read_value_ptr + i + 6),
                *(read_value_ptr + i + 7),
                *(read_value_ptr + i + 8),
                *(read_value_ptr + i + 9));
		    temp_dest_ptr += PRINT_NUMBER * WORD_DISPLAY_SIZE;
            i += PRINT_NUMBER;
        }
        else
        {
		    sprintf((char*)temp_dest_ptr, "%.4X ", *(read_value_ptr + i));
		    temp_dest_ptr += WORD_DISPLAY_SIZE;
            i++;
        }
    }

	return ATCMD_SUCCESS;

#endif
}


/*****************************************************************************/
//  Description:    Read doublewords from the memory, and convert to charater string 
//                  then generate the response message.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_ReadDMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr)    // Inputed command parameter struct.
{
#ifdef WIN32
    // Response OK.
    s_msg_head.subtype = ATCMD_SUCCESS;
	ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, &s_msg_head);
    return  ATCMD_SUCCESS;
#else

	uint32	i               = 0;
	uint8	*temp_dest_ptr  = SCI_NULL;
    uint32	*read_value_ptr = SCI_NULL;

    // Check input parameter.
    SCI_ASSERT((SCI_NULL == *dest_ptr) 
            && (SCI_NULL != cmd_ptr)
            && (SCI_NULL != cmd_ptr->cmd)
            && (cmd_ptr->len > 0));/*assert verified*/

    if (0 != strcmp((char*)cmd_ptr->cmd, "READD"))
    {
        return ATCMD_BADCOMMAND;
    }

	// Calculate dest length, and Malloc a buf for response buf.
    *dest_len_ptr = sizeof(MSG_HEAD_T) 
        + (cmd_ptr->len) * DWORD_DISPLAY_SIZE;

   	// Malloc a memory block for response buffer.
    *dest_ptr =(uint8*)SCI_ALLOC_APP(*dest_len_ptr);
	SCI_ASSERT(SCI_NULL != *dest_ptr);/*assert verified*/

    // Get message header.
	s_msg_head.len = *dest_len_ptr;
    SCI_MEMCPY((void*)(*dest_ptr), (void*)(&s_msg_head), sizeof(MSG_HEAD_T));
	
	// Read from memory, and convert it to charater string.
    temp_dest_ptr = *dest_ptr + sizeof(MSG_HEAD_T);	
	read_value_ptr = (uint32*)cmd_ptr->addr;
	for (i = 0; i < cmd_ptr->len; )
	{
        if ((cmd_ptr->len - i) >= PRINT_NUMBER)
        {
            // Printf PRINT_NUMBER values together.
		    sprintf((char*)temp_dest_ptr, 
                "%.8lx %.8lx %.8lx %.8lx %.8lx %.8lx %.8lx %.8lx %.8lx %.8lx ", 
                (uint32)(*(read_value_ptr + i)),
                (uint32)(*(read_value_ptr + i + 1)),
                (uint32)(*(read_value_ptr + i + 2)),
                (uint32)(*(read_value_ptr + i + 3)),
                (uint32)(*(read_value_ptr + i + 4)),
                (uint32)(*(read_value_ptr + i + 5)),
                (uint32)(*(read_value_ptr + i + 6)),
                (uint32)(*(read_value_ptr + i + 7)),
                (uint32)(*(read_value_ptr + i + 8)),
                (uint32)(*(read_value_ptr + i + 9)));
		    temp_dest_ptr += PRINT_NUMBER * DWORD_DISPLAY_SIZE;
            i += PRINT_NUMBER;
        }
        else
        {
		    sprintf((char*)temp_dest_ptr, "%.8lx ", (uint32)(*(read_value_ptr + i)));
		    temp_dest_ptr += DWORD_DISPLAY_SIZE;
            i++;
        }
    }

	return ATCMD_SUCCESS;

#endif

}

/*****************************************************************************/
//  Description:    Write bytes value into memory.
//	Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_WriteBMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr)    // Inputed command parameter struct.
{
	int32	i                = 0;
	uint32	temp_value       = 0;
	uint32	status           = ATCMD_SUCCESS;
	uint8	*value_str_ptr   = SCI_NULL;
    uint8   *write_value_ptr = SCI_NULL;
	
    // Check input parameter.
    SCI_ASSERT((SCI_NULL != cmd_ptr)
            && (SCI_NULL != cmd_ptr->cmd)
            && (cmd_ptr->len > 0));/*assert verified*/

	if (0 != strcmp((char*)cmd_ptr->cmd, "WRITEB"))
	{
        return ATCMD_BADCOMMAND;
    }

	// Malloc a buffer to save value. 
    write_value_ptr = (uint8*)SCI_ALLOC_APP((cmd_ptr->len+1) * sizeof(uint8));
	SCI_ASSERT(SCI_NULL != write_value_ptr);/*assert verified*/
	
	SCI_DisableIRQ();
	
	// Get number one by one from src.
	value_str_ptr = (uint8*)strtok((char*)(cmd_ptr->value_ptr), 
        (char*)g_diag_atcmd_strtoken);
	status = IsValidNum(value_str_ptr, &temp_value, BYTE_TYPE_DATA);
	if (status == ATCMD_SUCCESS)
	{
		*write_value_ptr = (uint8)temp_value;
	
		for (i = 1; i < cmd_ptr->len; i++)		
		{
			value_str_ptr = (uint8*)strtok(SCI_NULL, (char*)g_diag_atcmd_strtoken);
			status = IsValidNum(value_str_ptr, &temp_value, BYTE_TYPE_DATA);
			if(status == ATCMD_SUCCESS)
			{
				*(write_value_ptr + i) = (uint8)temp_value;
			}
			else
			{
				break;
			}
		}
    } // End get number
    
    SCI_RestoreIRQ();

	if(ATCMD_SUCCESS == status)
	{
#ifndef WIN32
        // Write the all values to the memory together. 
		SCI_MEMCPY((void*)cmd_ptr->addr, 
            (void*)write_value_ptr, 
            (cmd_ptr->len) * sizeof(uint8));
#endif
	}

    // Free malloced buffer.
    SCI_FREE(write_value_ptr);

	return status;

}

/*****************************************************************************/
//  Description:    Write words value into memory.
//	Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_WriteWMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr)    // Inputed command parameter struct.
{
	int32	i                = 0;
	uint32	temp_value       = 0;
	uint32	status           = ATCMD_SUCCESS;
	uint8	*value_str_ptr   = SCI_NULL;
    uint16  *write_value_ptr = SCI_NULL;
	
    // Check input parameter.
    SCI_ASSERT((SCI_NULL != cmd_ptr)
            && (SCI_NULL != cmd_ptr->cmd)
            && (cmd_ptr->len > 0));/*assert verified*/

	if (0 != strcmp((char*)cmd_ptr->cmd, "WRITEW"))
	{
        return ATCMD_BADCOMMAND;
    }

	// Malloc a buffer to save value. 
    write_value_ptr = (uint16*)SCI_ALLOC_APP((cmd_ptr->len+1) * sizeof(uint16));
	SCI_ASSERT(SCI_NULL != write_value_ptr);/*assert verified*/
		
	SCI_DisableIRQ();
	
	// Get number one by one from src.
	value_str_ptr = (uint8*)strtok((char*)(cmd_ptr->value_ptr), 
        (char*)g_diag_atcmd_strtoken);
	status = IsValidNum(value_str_ptr, &temp_value, WORD_TYPE_DATA);
	if (status == ATCMD_SUCCESS)
	{
		*write_value_ptr = (uint16)temp_value;
	
		for (i = 1; i < cmd_ptr->len; i++)		
		{
			value_str_ptr = (uint8*)strtok(SCI_NULL, (char*)g_diag_atcmd_strtoken);
			status = IsValidNum(value_str_ptr, &temp_value, WORD_TYPE_DATA);
			if(status == ATCMD_SUCCESS)
			{
				*(write_value_ptr + i) = (uint16)temp_value;
			}
			else
			{
				break;
			}
		}
    } // End get number

    SCI_RestoreIRQ();

	if(ATCMD_SUCCESS == status)
	{
#ifndef WIN32
        // Write the all values to the memory together. 
		SCI_MEMCPY((void*)cmd_ptr->addr, 
            (void*)write_value_ptr, 
            (cmd_ptr->len) * sizeof(uint16));
#endif
	}

    // Free malloced buffer.
    SCI_FREE(write_value_ptr);

	return status;

}

/*****************************************************************************/
//  Description:    Write doublewords value into memory.
//	Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32  MCU_WriteDMemory(  // Handle result, success or error code.
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MCUCMD_PARAM_T *cmd_ptr)    // Inputed command parameter struct.
{
	int32	i                = 0;
	uint32	temp_value       = 0;
	uint32	status           = ATCMD_SUCCESS;
	uint8	*value_str_ptr   = SCI_NULL;
    uint32  *write_value_ptr = SCI_NULL;
	
    // Check input parameter.
    SCI_ASSERT((SCI_NULL != cmd_ptr)
            && (SCI_NULL != cmd_ptr->cmd)
            && (cmd_ptr->len > 0));/*assert verified*/

	if (0 != strcmp((char*)cmd_ptr->cmd, "WRITED"))
	{
        return ATCMD_BADCOMMAND;
    }

	// Malloc a buffer to save value. 
    write_value_ptr = (uint32*)SCI_ALLOC_APP((cmd_ptr->len+1) * sizeof(uint32));
	SCI_ASSERT(SCI_NULL != write_value_ptr);/*assert verified*/
		
	SCI_DisableIRQ();
	
	// Get number one by one from src.
	value_str_ptr = (uint8*)strtok((char*)(cmd_ptr->value_ptr), 
        (char*)g_diag_atcmd_strtoken);
	status = IsValidNum(value_str_ptr, &temp_value, DWORD_TYPE_DATA);
	if (status == ATCMD_SUCCESS)
	{
		*write_value_ptr = (uint32)temp_value;
	
		for (i = 1; i < cmd_ptr->len; i++)		
		{
			value_str_ptr = (uint8*)strtok(SCI_NULL, (char*)g_diag_atcmd_strtoken);
			status = IsValidNum(value_str_ptr, &temp_value, DWORD_TYPE_DATA);
			if(status == ATCMD_SUCCESS)
			{
				*(write_value_ptr + i) = (uint32)temp_value;
			}
			else
			{
				break;
			}
		}
    } // End get number

    SCI_RestoreIRQ();
    
	if(ATCMD_SUCCESS == status)
	{
#ifndef WIN32
        // Write the all values to the memory together. 
		SCI_MEMCPY((void*)cmd_ptr->addr, 
            (void*)write_value_ptr, 
            (cmd_ptr->len) * sizeof(uint32));
#endif
	}

    // Free malloced buffer.
    SCI_FREE(write_value_ptr);

	return status;

}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
